#include "victron_ble.h"
#include "esphome/core/log.h"

#ifdef USE_ESP32

namespace esphome {
namespace victron_ble {

static const char *const TAG = "victron_ble";
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

VictronBle::VictronBle() : PollingComponent(60000 /* 60 seconds */) {}

void VictronBle::dump_config() {
  ESP_LOGCONFIG(TAG, "Mopeka Pro Check");
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

void VictronBle::update() {
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

void VictronBle::gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                                     esp_ble_gattc_cb_param_t *param) {
  switch (event) {
    case ESP_GATTC_OPEN_EVT:
      if (param->open.status == ESP_GATT_OK) {
        ESP_LOGI(TAG, "[%s] Connected successfully", this->get_name().c_str());
        this->read_request_started_ = 0;
      }
      break;

    case ESP_GATTC_DISCONNECT_EVT:
      if (this->notify_) {
        ESP_LOGW(TAG, "[%s] Disconnected", this->get_name().c_str());
      } else {
        ESP_LOGI(TAG, "[%s] Disconnected", this->get_name().c_str());
      }
      this->read_request_started_ = 0;
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
      this->handle_keep_alive_ = this->find_handle_and_read_(&CHARACTERISTIC_UUID_KEEP_ALIVE);
      if (this->state_of_charge_ != nullptr) {
        this->handle_state_of_charge_ = this->find_handle_and_read_(&CHARACTERISTIC_UUID_SOC);
      }
      if (this->voltage_ != nullptr) {
        this->handle_voltage_ = this->find_handle_and_read_(&CHARACTERISTIC_UUID_VOLTAGE);
      }
      if (this->power_ != nullptr) {
        this->handle_power_ = this->find_handle_and_read_(&CHARACTERISTIC_UUID_POWER);
      }
      if (this->current_ != nullptr) {
        this->handle_current_ = this->find_handle_and_read_(&CHARACTERISTIC_UUID_CURRENT);
      }
      if (this->ah_ != nullptr) {
        this->handle_ah_ = this->find_handle_and_read_(&CHARACTERISTIC_UUID_AH);
      }
      if (this->starter_battery_voltage_ != nullptr) {
        this->handle_starter_battery_voltage_ = this->find_handle_and_read_(&CHARACTERISTIC_UUID_VOLTAGE2);
      }
      if (this->val2_ != nullptr) {
        this->handle_val2_ = this->find_handle_and_read_(&CHARACTERISTIC_UUID_VAL2);
      }
      if (this->val3_ != nullptr) {
        this->handle_val3_ = this->find_handle_and_read_(&CHARACTERISTIC_UUID_VAL3);
      }
      if (this->val4_ != nullptr) {
        this->handle_val4_ = this->find_handle_and_read_(&CHARACTERISTIC_UUID_VAL4);
      }
      if (this->remaining_time_ != nullptr) {
        this->handle_remaining_time_ = this->find_handle_and_read_(&CHARACTERISTIC_UUID_REMAINING_TIME);
      }
      if (this->notify_ && this->read_request_started_ > 0) {
        // Register keep alive interval.
        this->set_interval(KEEP_ALIVE_INTERVAL, 20000 /* 20 seconds */, [this]() { this->send_keep_alive_(); });
        this->node_state = esp32_ble_tracker::ClientState::ESTABLISHED;
      }
      break;

    case ESP_GATTC_READ_CHAR_EVT:
      if (param->read.conn_id != this->parent_->get_conn_id()) {
        break;
      }
      this->node_state = esp32_ble_tracker::ClientState::ESTABLISHED;
      if (param->read.status == ESP_GATT_OK) {
        ESP_LOGV(TAG, "[%s] Reading char at handle %d, status=%d", this->get_name().c_str(), param->read.handle,
                 param->read.status);
      } else {
        ESP_LOGW(TAG, "[%s] Error reading char at handle %d, status=%d", this->get_name().c_str(), param->read.handle,
                 param->read.status);
        break;
      }

      this->read_value_(param->read.handle, param->read.value, param->read.value_len,
                        /*register_for_notify*/ this->notify_);

      if (this->read_request_started_ == 0) {
        // I got all requested values. Submit initial sensors.
        this->set_timeout(UPDATE_SENSOR_TIMEOUT, 100 /*msec */, [this]() { this->update_sensors_(); });
        if (!this->notify_) {
          // Not using notification so I can disconnect.
          this->parent_->set_enabled(false);
        }
      }

      break;

    case ESP_GATTC_REG_FOR_NOTIFY_EVT:
      this->node_state = esp32_ble_tracker::ClientState::ESTABLISHED;
      if (param->reg_for_notify.status == ESP_GATT_OK) {
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

uint16_t VictronBle::find_handle_and_read_(const esp32_ble_tracker::ESPBTUUID *characteristic) {
  auto *chr = this->parent_->get_characteristic(SERVICE_UUID, *characteristic);
  if (chr == nullptr) {
    ESP_LOGW(TAG, "[%s] No characteristic found at service %s char %s", this->get_name().c_str(),
             SERVICE_UUID.to_string().c_str(), (*characteristic).to_string().c_str());
    return 0;
  }

  // Auth options: ESP_GATT_AUTH_REQ_NONE, ESP_GATT_AUTH_REQ_MITM, ESP_GATT_AUTH_REQ_SIGNED_MITM
  auto status = esp_ble_gattc_read_char(this->parent_->get_gattc_if(), this->parent_->get_conn_id(), chr->handle,
                                        ESP_GATT_AUTH_REQ_SIGNED_MITM);

  if (status) {
    ESP_LOGW(TAG, "[%s] Error sending read request for service %s char %s, status=%d", this->get_name().c_str(),
             SERVICE_UUID.to_string().c_str(), (*characteristic).to_string().c_str(), status);
  } else {
    this->read_request_started_++;
  }

  return chr->handle;
}

void VictronBle::read_value_(const uint16_t handle, const uint8_t *value, const uint16_t value_len,
                             const bool register_for_notify) {
  bool handle_found = false;

  if (handle == this->handle_state_of_charge_ && value_len == sizeof(u_int16_t)) {
    // handle_state_of_charge_
    handle_found = true;
    this->read_request_started_--;
    this->value_state_of_charge_ = *reinterpret_cast<const u_int16_t *>(value);
    ESP_LOGD(TAG, "[%s] State of Charge: %u", this->get_name().c_str(), this->value_state_of_charge_);
  } else if (handle == this->handle_voltage_ && value_len == sizeof(int16_t)) {
    // handle_voltage_
    handle_found = true;
    this->read_request_started_--;
    this->value_voltage_ = *reinterpret_cast<const int16_t *>(value);
    ESP_LOGD(TAG, "[%s] Voltage: %i", this->get_name().c_str(), this->value_voltage_);
  } else if (handle == this->handle_power_ && value_len == sizeof(int16_t)) {
    // handle_power_
    handle_found = true;
    this->read_request_started_--;
    this->value_power_ = *reinterpret_cast<const int16_t *>(value);
    ESP_LOGD(TAG, "[%s] Power: %i", this->get_name().c_str(), this->value_power_);
  } else if (handle == this->handle_current_ && value_len == sizeof(int32_t)) {
    // handle_current_
    handle_found = true;
    this->read_request_started_--;
    this->value_current_ = *reinterpret_cast<const int32_t *>(value);
    ESP_LOGD(TAG, "[%s] Current: %i", this->get_name().c_str(), this->value_current_);
  } else if (handle == this->handle_ah_ && value_len == sizeof(int32_t)) {
    // handle_ah_
    handle_found = true;
    this->read_request_started_--;
    this->value_ah_ = *reinterpret_cast<const int32_t *>(value);
    ESP_LOGD(TAG, "[%s] Ah: %i", this->get_name().c_str(), this->value_ah_);
  } else if (handle == this->handle_starter_battery_voltage_ && value_len == sizeof(int16_t)) {
    // handle_starter_battery_voltage_
    handle_found = true;
    this->read_request_started_--;
    this->value_starter_battery_voltage_ = *reinterpret_cast<const int16_t *>(value);
    ESP_LOGD(TAG, "[%s] Starter Battery Voltage: %i", this->get_name().c_str(), this->value_starter_battery_voltage_);
  } else if (handle == this->handle_val2_ && value_len == sizeof(u_int16_t)) {
    // handle_val2_
    handle_found = true;
    this->read_request_started_--;
    this->value_val2_ = *reinterpret_cast<const u_int16_t *>(value);
    ESP_LOGD(TAG, "[%s] Value 2: %u", this->get_name().c_str(), this->value_val2_);
  } else if (handle == this->handle_val3_ && value_len == sizeof(u_int16_t)) {
    // handle_val3_
    handle_found = true;
    this->read_request_started_--;
    this->value_val3_ = *reinterpret_cast<const u_int16_t *>(value);
    ESP_LOGD(TAG, "[%s] Value 3: %u", this->get_name().c_str(), this->value_val3_);
  } else if (handle == this->handle_val4_ && value_len == sizeof(int16_t)) {
    // handle_val4_
    handle_found = true;
    this->read_request_started_--;
    this->value_val4_ = *reinterpret_cast<const int16_t *>(value);
    ESP_LOGD(TAG, "[%s] Value 4: %i", this->get_name().c_str(), this->value_val4_);
  } else if (handle == this->handle_remaining_time_ && value_len == sizeof(u_int16_t)) {
    // handle_remaining_time_
    handle_found = true;
    this->read_request_started_--;
    this->value_remaining_time_ = *reinterpret_cast<const u_int16_t *>(value);
    ESP_LOGD(TAG, "[%s] Remaining Time: %u", this->get_name().c_str(), this->value_remaining_time_);
  }

  if (register_for_notify && handle_found) {
    auto status =
        esp_ble_gattc_register_for_notify(this->parent_->get_gattc_if(), this->parent_->get_remote_bda(), handle);

    if (status) {
      ESP_LOGW(TAG, "[%s] Error sending notify request for service %s handle 0x%04x, status=%d",
               this->get_name().c_str(), SERVICE_UUID.to_string().c_str(), handle, status);
    }
  }
}

void VictronBle::send_keep_alive_() {
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

void VictronBle::update_sensors_() {
  if (this->state_of_charge_ != nullptr) {
    if (this->value_state_of_charge_ != std::numeric_limits<u_int16_t>::max()) {
      this->state_of_charge_->publish_state(static_cast<float>(this->value_state_of_charge_) / 100.0f);
    } else {
      this->state_of_charge_->publish_state(NAN);
      ESP_LOGW(TAG, "[%s] Recieved no or invalid data for State of Charge.");
    }
  }

  if (this->voltage_ != nullptr) {
    if (this->value_voltage_ != std::numeric_limits<int16_t>::max()) {
      this->voltage_->publish_state(static_cast<float>(this->value_voltage_) / 100.0f);
    } else {
      this->voltage_->publish_state(NAN);
      ESP_LOGW(TAG, "[%s] Recieved no or invalid data for Voltage.");
    }
  }

  if (this->power_ != nullptr) {
    if (this->value_power_ != std::numeric_limits<int16_t>::max()) {
      this->power_->publish_state(static_cast<float>(this->value_power_));
    } else {
      this->power_->publish_state(NAN);
      ESP_LOGW(TAG, "[%s] Recieved no or invalid data for Power consumption.");
    }
  }

  if (this->current_ != nullptr) {
    if (this->value_current_ != std::numeric_limits<int32_t>::max()) {
      this->current_->publish_state(static_cast<float>(this->value_current_) / 1000.0f);
    } else {
      this->current_->publish_state(NAN);
      ESP_LOGW(TAG, "[%s] Recieved no or invalid data for Current.");
    }
  }

  if (this->ah_ != nullptr) {
    if (this->value_ah_ != std::numeric_limits<int32_t>::max()) {
      this->ah_->publish_state(static_cast<float>(this->value_ah_) / 10.0f);
    } else {
      this->ah_->publish_state(NAN);
      ESP_LOGW(TAG, "[%s] Recieved no or invalid data for Consumed Ah.");
    }
  }

  if (this->starter_battery_voltage_ != nullptr) {
    if (this->value_starter_battery_voltage_ != std::numeric_limits<int16_t>::max()) {
      this->starter_battery_voltage_->publish_state(static_cast<float>(this->value_starter_battery_voltage_) / 100.0f);
    } else {
      this->starter_battery_voltage_->publish_state(NAN);
      ESP_LOGW(TAG, "[%s] Recieved no or invalid data for Starter Battery Voltage.");
    }
  }

  if (this->val2_ != nullptr) {
    if (this->value_val2_ != std::numeric_limits<u_int16_t>::max()) {
      this->val2_->publish_state(static_cast<float>(this->value_val2_));
    } else {
      this->val2_->publish_state(NAN);
      ESP_LOGW(TAG, "[%s] Recieved no or invalid data for Val2.");
    }
  }

  if (this->val3_ != nullptr) {
    if (this->value_val3_ != std::numeric_limits<u_int16_t>::max()) {
      this->val3_->publish_state(static_cast<float>(this->value_val3_));
    } else {
      this->val3_->publish_state(NAN);
      ESP_LOGW(TAG, "[%s] Recieved no or invalid data for Val3.");
    }
  }

  if (this->val4_ != nullptr) {
    if (this->value_val4_ != std::numeric_limits<int16_t>::max()) {
      this->val4_->publish_state(static_cast<float>(this->value_val4_));
    } else {
      this->val4_->publish_state(NAN);
      ESP_LOGW(TAG, "[%s] Recieved no or invalid data for Val4.");
    }
  }

  if (this->remaining_time_ != nullptr) {
    if (this->value_remaining_time_ != std::numeric_limits<u_int16_t>::max()) {
      this->remaining_time_->publish_state(static_cast<float>(this->value_remaining_time_));
    } else {
      this->remaining_time_->publish_state(NAN);
      ESP_LOGW(TAG, "[%s] Recieved no or invalid data for Remaining Time.");
    }
  }
}

// bool VictronBle::parse_device(const esp32_ble_tracker::ESPBTDevice &device) {
//   const auto &manu_datas = device.get_manufacturer_datas();
//   if (manu_datas.size() != 1) {
//     return false;
//   }
//   const auto &manu_data = manu_datas[0];

//   if (manu_data.uuid != esp32_ble_tracker::ESPBTUUID::from_uint16(MANUFACTURER_ID)) {
//     return true;
//   }

//   //   if (manu_data.data.size() < MANUFACTURER_DATA_MIN_LENGTH) {
//   //     return false;
//   //   }

// //   const auto ad_flag = device.ad_flag_.has_value();
//   const auto name = device.get_name();

//   //   if (!device.get_name().empty()) {
//   //     ESP_LOGD(TAG, "  Name: '%s'", device.get_name().c_str());
//   //   }

//   ESP_LOGI(TAG, "Victron Device found: %s (%s) %d", device.address_str().c_str(), name.c_str(),
//            manu_data.data.size());

//   return false;
// }

// bool VictronBle::parse_sync_button_(const std::vector<uint8_t> &message) { return (message[2] & 0x80) != 0; }

}  // namespace victron_ble
}  // namespace esphome

#endif
