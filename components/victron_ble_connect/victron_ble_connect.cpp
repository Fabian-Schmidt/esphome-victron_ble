#include "victron_ble_connect.h"
#include "esphome/core/log.h"

#ifdef USE_ESP32

namespace esphome {
namespace victron_ble_connect {

static const char *const TAG = "victron_ble_connect";
static const std::string KEEP_ALIVE_INTERVAL = "victron_ble_keep_alive";
static const std::string UPDATE_SENSOR_TIMEOUT = "victron_ble_update_sensors";
/**
 * Expected steps:
 * 1. ESP_GATTC_OPEN_EVT
 * 2. ESP_GATTC_SEARCH_CMPL_EVT -> `esp_ble_gattc_read_char` - with ESP_GATT_AUTH_REQ_SIGNED_MITM
 * - Auth happens if necessary
 * 3. ESP_GATTC_READ_CHAR_EVT -> first value & (if notify) `esp_ble_gattc_register_for_notify`
 * 4. ESP_GATTC_REG_FOR_NOTIFY_EVT
 * 5. ESP_GATTC_NOTIFY_EVT -> continous values
 * if notify: every 20 seconds (polling frequency) a 60 second keep alive is send.
 */

VictronBleConnect::VictronBleConnect() : PollingComponent(60000 /* 60 seconds */) {}

void VictronBleConnect::dump_config() {
  ESP_LOGCONFIG(TAG, "Victron ble connect");
  LOG_SENSOR("  ", "State of Charge", this->state_of_charge_);
  LOG_SENSOR("  ", "Voltage", this->voltage_);
  LOG_SENSOR("  ", "Current", this->current_);
  LOG_SENSOR("  ", "Ah", this->ah_);
  LOG_SENSOR("  ", "Starter Battery Voltage", this->starter_battery_voltage_);
  LOG_SENSOR("  ", "Val2", this->val2_);
  LOG_SENSOR("  ", "Val3", this->val3_);
  LOG_SENSOR("  ", "Val4", this->val4_);
  LOG_SENSOR("  ", "Remaining Time", this->remaining_time_);
  ESP_LOGCONFIG(TAG, "  Notify: %s", YESNO(this->notify_));
}

void VictronBleConnect::update() {
  if (this->notify_) {
    if (this->parent_->enabled && this->node_state == esp32_ble_tracker::ClientState::ESTABLISHED) {
      this->update_sensors_();
    }
  } else {
    // Polling of connection
    if (this->node_state != esp32_ble_tracker::ClientState::ESTABLISHED) {
      if (!this->parent_->enabled) {
        ESP_LOGI(TAG, "[%s] Reconnecting to device", this->get_name().c_str());
        this->parent_->set_enabled(true);
        this->parent_->connect();
      } else {
        ESP_LOGI(TAG, "[%s] Connection in progress", this->get_name().c_str());
      }
    }
  }
}

void VictronBleConnect::gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                                            esp_ble_gattc_cb_param_t *param) {
  switch (event) {
    case ESP_GATTC_OPEN_EVT:
      if (param->open.status == ESP_GATT_OK) {
        ESP_LOGI(TAG, "[%s] Connected successfully", this->get_name().c_str());
        this->reset_state();
      }
      break;

    case ESP_GATTC_DISCONNECT_EVT:
      if (this->notify_) {
        ESP_LOGW(TAG, "[%s] Disconnected", this->get_name().c_str());
      } else {
        ESP_LOGI(TAG, "[%s] Disconnected", this->get_name().c_str());
      }
      if (this->notify_) {
        cancel_interval(KEEP_ALIVE_INTERVAL);
      }
      // {
      //   // Remove device association
      //   esp_bd_addr_t remote_bda;
      //   memcpy(remote_bda, this->parent_->get_remote_bda(), sizeof(esp_bd_addr_t));
      //   esp_ble_remove_bond_device(remote_bda);
      // }
      break;

    case ESP_GATTC_SEARCH_CMPL_EVT:
      this->node_state = esp32_ble_tracker::ClientState::ESTABLISHED;
      this->handle_keep_alive_ = this->find_handle_(&CHARACTERISTIC_UUID_KEEP_ALIVE);
      if (this->state_of_charge_ != nullptr) {
        this->handle_state_of_charge_ = this->find_handle_(&CHARACTERISTIC_UUID_SOC);
      }
      if (this->voltage_ != nullptr) {
        this->handle_voltage_ = this->find_handle_(&CHARACTERISTIC_UUID_VOLTAGE);
      }
      if (this->power_ != nullptr) {
        this->handle_power_ = this->find_handle_(&CHARACTERISTIC_UUID_POWER);
      }
      if (this->current_ != nullptr) {
        this->handle_current_ = this->find_handle_(&CHARACTERISTIC_UUID_CURRENT);
      }
      if (this->ah_ != nullptr) {
        this->handle_ah_ = this->find_handle_(&CHARACTERISTIC_UUID_AH);
      }
      if (this->starter_battery_voltage_ != nullptr) {
        this->handle_starter_battery_voltage_ = this->find_handle_(&CHARACTERISTIC_UUID_VOLTAGE2);
      }
      if (this->val2_ != nullptr) {
        this->handle_val2_ = this->find_handle_(&CHARACTERISTIC_UUID_VAL2);
      }
      if (this->val3_ != nullptr) {
        this->handle_val3_ = this->find_handle_(&CHARACTERISTIC_UUID_VAL3);
      }
      if (this->val4_ != nullptr) {
        this->handle_val4_ = this->find_handle_(&CHARACTERISTIC_UUID_VAL4);
      }
      if (this->remaining_time_ != nullptr) {
        this->handle_remaining_time_ = this->find_handle_(&CHARACTERISTIC_UUID_REMAINING_TIME);
      }

      this->request_read_next_value_();
      break;

    case ESP_GATTC_READ_CHAR_EVT:
      if (param->read.conn_id != this->parent_->get_conn_id()) {
        break;
      }
      if (param->read.status == ESP_GATT_OK) {
        this->node_state = esp32_ble_tracker::ClientState::ESTABLISHED;
        ESP_LOGV(TAG, "[%s] Reading char at handle %d, status=%d", this->get_name().c_str(), param->read.handle,
                 param->read.status);
      } else {
        ESP_LOGW(TAG, "[%s] Error reading char at handle %d, status=%d", this->get_name().c_str(), param->read.handle,
                 param->read.status);
        break;
      }

      this->read_value_(param->read.handle, param->read.value, param->read.value_len,
                        /*register_for_notify*/ this->notify_);

      this->request_read_next_value_();

      if (this->read_request_started_ == 0) {
        // I got all requested values. Submit initial sensors.
        this->set_timeout(UPDATE_SENSOR_TIMEOUT, 100 /*msec */, [this]() { this->update_sensors_(); });
        if (this->notify_) {
          // Register keep alive interval.
          this->set_interval(KEEP_ALIVE_INTERVAL, 20000 /* 20 seconds */, [this]() { this->send_keep_alive_(); });
        } else {
          // Not using notification so I can disconnect.
          this->parent_->set_enabled(false);
        }
      }

      break;

    case ESP_GATTC_REG_FOR_NOTIFY_EVT:
      if (param->reg_for_notify.status == ESP_GATT_OK) {
        this->node_state = esp32_ble_tracker::ClientState::ESTABLISHED;
        ESP_LOGV(TAG, "[%s] Register notify 0x%04x status=%d", this->get_name().c_str(), param->reg_for_notify.handle,
                 param->reg_for_notify.status);
      } else {
        // param->reg_for_notify.status == ESP_GATT_NO_RESOURCES
        ESP_LOGW(TAG, "[%s] Register notify 0x%04x status=%d", this->get_name().c_str(), param->reg_for_notify.handle,
                 param->reg_for_notify.status);
      }
      break;

    case ESP_GATTC_NOTIFY_EVT:
      ESP_LOGV(TAG, "[%s] Notify to handle 0x%04x is notify=%s", this->get_name().c_str(), param->notify.handle,
               param->notify.is_notify ? "true" : "false");

      if (param->notify.is_notify) {
        this->read_value_(param->notify.handle, param->notify.value, param->notify.value_len);
      }
      break;

    case ESP_GATTC_WRITE_CHAR_EVT:
    case ESP_GATTC_WRITE_DESCR_EVT:
      if (param->write.status == ESP_GATT_OK) {
        ESP_LOGV(TAG, "[%s] Write to handle 0x%04x status=%d", this->get_name().c_str(), param->write.handle,
                 param->write.status);
      } else {
        // status 0x05 - ESP_GATT_INSUF_AUTHENTICATION
        ESP_LOGW(TAG, "[%s] Write to handle 0x%04x status=%d", this->get_name().c_str(), param->write.handle,
                 param->write.status);
      }
      break;

    default:
      break;
  }
}

uint16_t VictronBleConnect::find_handle_(const esp32_ble_tracker::ESPBTUUID *characteristic) {
  auto *chr = this->parent_->get_characteristic(SERVICE_UUID, *characteristic);
  if (chr == nullptr) {
    ESP_LOGW(TAG, "[%s] No characteristic found at service %s char %s", this->get_name().c_str(),
             SERVICE_UUID.to_string().c_str(), (*characteristic).to_string().c_str());
    return 0;
  }
  return chr->handle;
}

#define REQUEST_READ_NEXT_VALUE(name) \
  if (this->handle_##name != 0 && !this->request_read_##name) { \
    this->request_read_##name = true; \
    if (this->request_read_(this->handle_##name)) { \
      /* Request was okay. Wait for response. */ \
      return; \
    } \
  }

void VictronBleConnect::request_read_next_value_() {
  REQUEST_READ_NEXT_VALUE(state_of_charge_)
  REQUEST_READ_NEXT_VALUE(voltage_)
  REQUEST_READ_NEXT_VALUE(power_)
  REQUEST_READ_NEXT_VALUE(current_)
  REQUEST_READ_NEXT_VALUE(ah_)
  REQUEST_READ_NEXT_VALUE(starter_battery_voltage_)
  REQUEST_READ_NEXT_VALUE(val2_)
  REQUEST_READ_NEXT_VALUE(val3_)
  REQUEST_READ_NEXT_VALUE(val4_)
  REQUEST_READ_NEXT_VALUE(remaining_time_)
}
#undef REQUEST_READ_NEXT_VALUE

bool VictronBleConnect::request_read_(const uint16_t handle) {
  // Auth options: ESP_GATT_AUTH_REQ_NONE, ESP_GATT_AUTH_REQ_MITM, ESP_GATT_AUTH_REQ_SIGNED_MITM
  auto status = esp_ble_gattc_read_char(this->parent_->get_gattc_if(), this->parent_->get_conn_id(), handle,
                                        ESP_GATT_AUTH_REQ_SIGNED_MITM);

  if (status) {
    ESP_LOGW(TAG, "[%s] Error sending read request for service %s handle 0x%04x, status=%d", this->get_name().c_str(),
             SERVICE_UUID.to_string().c_str(), handle, status);
    return false;
  } else {
    this->read_request_started_++;
    return true;
  }
}

void VictronBleConnect::read_value_(const uint16_t handle, const uint8_t *value, const uint16_t value_len,
                                    const bool register_for_notify) {
  bool handle_found = false;

  if (handle == this->handle_state_of_charge_ && value_len == sizeof(u_int16_t)) {
    // handle_state_of_charge_
    handle_found = true;
    this->value_state_of_charge_ = *reinterpret_cast<const u_int16_t *>(value);
    this->value_is_set_state_of_charge_ = true;
    ESP_LOGD(TAG, "[%s] State of Charge: %u", this->get_name().c_str(), this->value_state_of_charge_);
  } else if (handle == this->handle_voltage_ && value_len == sizeof(int16_t)) {
    // handle_voltage_
    handle_found = true;
    this->value_voltage_ = *reinterpret_cast<const int16_t *>(value);
    this->value_is_set_voltage_ = true;
    ESP_LOGD(TAG, "[%s] Voltage: %i", this->get_name().c_str(), this->value_voltage_);
  } else if (handle == this->handle_power_ && value_len == sizeof(int16_t)) {
    // handle_power_
    handle_found = true;
    this->value_power_ = *reinterpret_cast<const int16_t *>(value);
    this->value_is_set_power_ = true;
    ESP_LOGD(TAG, "[%s] Power: %i", this->get_name().c_str(), this->value_power_);
  } else if (handle == this->handle_current_ && value_len == sizeof(int32_t)) {
    // handle_current_
    handle_found = true;
    this->value_current_ = *reinterpret_cast<const int32_t *>(value);
    this->value_is_set_current_ = true;
    ESP_LOGD(TAG, "[%s] Current: %i", this->get_name().c_str(), this->value_current_);
  } else if (handle == this->handle_ah_ && value_len == sizeof(int32_t)) {
    // handle_ah_
    handle_found = true;
    this->value_ah_ = *reinterpret_cast<const int32_t *>(value);
    this->value_is_set_ah_ = true;
    ESP_LOGD(TAG, "[%s] Ah: %i", this->get_name().c_str(), this->value_ah_);
  } else if (handle == this->handle_starter_battery_voltage_ && value_len == sizeof(int16_t)) {
    // handle_starter_battery_voltage_
    handle_found = true;
    this->value_starter_battery_voltage_ = *reinterpret_cast<const int16_t *>(value);
    this->value_is_set_starter_battery_voltage_ = true;
    ESP_LOGD(TAG, "[%s] Starter Battery Voltage: %i", this->get_name().c_str(), this->value_starter_battery_voltage_);
  } else if (handle == this->handle_val2_ && value_len == sizeof(u_int16_t)) {
    // handle_val2_
    handle_found = true;
    this->value_val2_ = *reinterpret_cast<const u_int16_t *>(value);
    this->value_is_set_val2_ = true;
    ESP_LOGD(TAG, "[%s] Value 2: %u", this->get_name().c_str(), this->value_val2_);
  } else if (handle == this->handle_val3_ && value_len == sizeof(u_int16_t)) {
    // handle_val3_
    handle_found = true;
    this->value_val3_ = *reinterpret_cast<const u_int16_t *>(value);
    this->value_is_set_val3_ = true;
    ESP_LOGD(TAG, "[%s] Value 3: %u", this->get_name().c_str(), this->value_val3_);
  } else if (handle == this->handle_val4_ && value_len == sizeof(int16_t)) {
    // handle_val4_
    handle_found = true;
    this->value_val4_ = *reinterpret_cast<const int16_t *>(value);
    this->value_is_set_val4_ = true;
    ESP_LOGD(TAG, "[%s] Value 4: %i", this->get_name().c_str(), this->value_val4_);
  } else if (handle == this->handle_remaining_time_ && value_len == sizeof(u_int16_t)) {
    // handle_remaining_time_
    handle_found = true;
    this->value_remaining_time_ = *reinterpret_cast<const u_int16_t *>(value);
    this->value_is_set_remaining_time_ = true;
    ESP_LOGD(TAG, "[%s] Remaining Time: %u", this->get_name().c_str(), this->value_remaining_time_);
  }

  if (handle_found) {
    this->read_request_started_--;
    if (register_for_notify) {
      auto status =
          esp_ble_gattc_register_for_notify(this->parent_->get_gattc_if(), this->parent_->get_remote_bda(), handle);

      if (status) {
        ESP_LOGW(TAG, "[%s] Error sending notify request for service %s handle 0x%04x, status=%d",
                 this->get_name().c_str(), SERVICE_UUID.to_string().c_str(), handle, status);
      }
    }
  } else {
    ESP_LOGW(TAG, "[%s] Error recieved data with unknown handle for service %s handle 0x%04x", this->get_name().c_str(),
             SERVICE_UUID.to_string().c_str(), handle);
  }
}

void VictronBleConnect::send_keep_alive_() {
  if (this->node_state == esp32_ble_tracker::ClientState::ESTABLISHED && this->handle_keep_alive_ != 0) {
    // A keep alive is required. Without that, the device will automatically disconnect after one minute.
    u_int16_t keep_alive = 30000 /* 30 seconds */;
    auto value = reinterpret_cast<u_int8_t *>(&keep_alive);
    u_int16_t value_len = sizeof(keep_alive);
    auto status =
        esp_ble_gattc_write_char(this->parent_->get_gattc_if(), this->parent_->get_conn_id(), this->handle_keep_alive_,
                                 value_len, value, ESP_GATT_WRITE_TYPE_RSP, ESP_GATT_AUTH_REQ_SIGNED_MITM);
    if (status) {
      ESP_LOGW(TAG, "[%s] sending keep alive failed, status=%d", this->get_name().c_str(), status);
    }
  }
}

void VictronBleConnect::update_sensors_() {
  if (this->state_of_charge_ != nullptr) {
    if (this->value_is_set_state_of_charge_) {
      this->state_of_charge_->publish_state(static_cast<float>(this->value_state_of_charge_) / 100.0f);
    } else {
      this->state_of_charge_->publish_state(NAN);
      ESP_LOGW(TAG, "[%s] Recieved no or invalid data for State of Charge.", this->get_name().c_str());
    }
  }

  if (this->voltage_ != nullptr) {
    if (this->value_is_set_voltage_) {
      this->voltage_->publish_state(static_cast<float>(this->value_voltage_) / 100.0f);
    } else {
      this->voltage_->publish_state(NAN);
      ESP_LOGW(TAG, "[%s] Recieved no or invalid data for Voltage.", this->get_name().c_str());
    }
  }

  if (this->power_ != nullptr) {
    if (this->value_is_set_power_) {
      this->power_->publish_state(static_cast<float>(this->value_power_));
    } else {
      this->power_->publish_state(NAN);
      ESP_LOGW(TAG, "[%s] Recieved no or invalid data for Power consumption.", this->get_name().c_str());
    }
  }

  if (this->current_ != nullptr) {
    if (this->value_is_set_current_) {
      this->current_->publish_state(static_cast<float>(this->value_current_) / 1000.0f);
    } else {
      this->current_->publish_state(NAN);
      ESP_LOGW(TAG, "[%s] Recieved no or invalid data for Current.", this->get_name().c_str());
    }
  }

  if (this->ah_ != nullptr) {
    if (this->value_is_set_ah_) {
      this->ah_->publish_state(static_cast<float>(this->value_ah_) / 10.0f);
    } else {
      this->ah_->publish_state(NAN);
      ESP_LOGW(TAG, "[%s] Recieved no or invalid data for Consumed Ah.", this->get_name().c_str());
    }
  }

  if (this->starter_battery_voltage_ != nullptr) {
    if (this->value_is_set_starter_battery_voltage_) {
      this->starter_battery_voltage_->publish_state(static_cast<float>(this->value_starter_battery_voltage_) / 100.0f);
    } else {
      this->starter_battery_voltage_->publish_state(NAN);
      ESP_LOGW(TAG, "[%s] Recieved no or invalid data for Starter Battery Voltage.", this->get_name().c_str());
    }
  }

  if (this->val2_ != nullptr) {
    if (this->value_is_set_val2_) {
      this->val2_->publish_state(static_cast<float>(this->value_val2_));
    } else {
      this->val2_->publish_state(NAN);
      ESP_LOGW(TAG, "[%s] Recieved no or invalid data for Val2.", this->get_name().c_str());
    }
  }

  if (this->val3_ != nullptr) {
    if (this->value_is_set_val3_) {
      this->val3_->publish_state(static_cast<float>(this->value_val3_));
    } else {
      this->val3_->publish_state(NAN);
      ESP_LOGW(TAG, "[%s] Recieved no or invalid data for Val3.", this->get_name().c_str());
    }
  }

  if (this->val4_ != nullptr) {
    if (this->value_is_set_val4_) {
      this->val4_->publish_state(static_cast<float>(this->value_val4_));
    } else {
      this->val4_->publish_state(NAN);
      ESP_LOGW(TAG, "[%s] Recieved no or invalid data for Val4.", this->get_name().c_str());
    }
  }

  if (this->remaining_time_ != nullptr) {
    if (this->value_is_set_remaining_time_) {
      this->remaining_time_->publish_state(static_cast<float>(this->value_remaining_time_));
    } else {
      this->remaining_time_->publish_state(NAN);
      ESP_LOGW(TAG, "[%s] Recieved no or invalid data for Remaining Time.", this->get_name().c_str());
    }
  }
}

}  // namespace victron_ble_connect
}  // namespace esphome

#endif
