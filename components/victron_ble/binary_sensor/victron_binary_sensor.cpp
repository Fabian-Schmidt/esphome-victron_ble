#include "victron_binary_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace victron_ble {

static const char *const TAG = "victron_ble.binary_sensor";

void VictronBinarySensor::dump_config() {
  LOG_BINARY_SENSOR("", "Victron Binary Sensor", this);
  ESP_LOGCONFIG(TAG, "Type %u", this->type_);
}

void VictronBinarySensor::setup() {
  switch (this->type_) {
    case VICTRON_BINARY_SENSOR_TYPE::ALARM:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::BATTERY_MONITOR:
            this->publish_state(msg->data.battery_monitor.alarm_reason != VE_REG_ALARM_REASON::NO_ALARM);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER:
            this->publish_state(msg->data.inverter.alarm_reason != VE_REG_ALARM_REASON::NO_ALARM);
            break;
          case VICTRON_BLE_RECORD_TYPE::SMART_BATTERY_PROTECT:
            this->publish_state(msg->data.smart_battery_protect.alarm_reason != VE_REG_ALARM_REASON::NO_ALARM);
            break;
          case VICTRON_BLE_RECORD_TYPE::DC_ENERGY_METER:
            this->publish_state(msg->data.dc_energy_meter.alarm_reason != VE_REG_ALARM_REASON::NO_ALARM);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no alarm.", this->parent_->address_str().c_str());
            this->publish_state("");
            break;
        }
      });
      break;

    case VICTRON_BINARY_SENSOR_TYPE::CHARGER_ERROR:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SOLAR_CHARGER:
            this->publish_state(msg->data.solar_charger.charger_error != VE_REG_CHR_ERROR_CODE::NO_ERROR);
            break;
          case VICTRON_BLE_RECORD_TYPE::DCDC_CONVERTER:
            this->publish_state(msg->data.dcdc_converter.charger_error != VE_REG_CHR_ERROR_CODE::NO_ERROR);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER_RS:
            this->publish_state(msg->data.inverter_rs.charger_error != VE_REG_CHR_ERROR_CODE::NO_ERROR);
            break;
          case VICTRON_BLE_RECORD_TYPE::SMART_BATTERY_PROTECT:
            this->publish_state(msg->data.smart_battery_protect.error_code != VE_REG_CHR_ERROR_CODE::NO_ERROR);
            break;
          case VICTRON_BLE_RECORD_TYPE::MULTI_RS:
            this->publish_state(msg->data.multi_rs.charger_error != VE_REG_CHR_ERROR_CODE::NO_ERROR);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no charger error.", this->parent_->address_str().c_str());
            this->publish_state("");
            break;
        }
      });
      break;

    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_OFF:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SOLAR_CHARGER:
            this->publish_state_(msg->data.solar_charger.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER:
            this->publish_state_(msg->data.inverter.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::DCDC_CONVERTER:
            this->publish_state_(msg->data.dcdc_converter.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER_RS:
            this->publish_state_(msg->data.inverter_rs.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::SMART_BATTERY_PROTECT:
            this->publish_state_(msg->data.smart_battery_protect.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::MULTI_RS:
            this->publish_state_(msg->data.multi_rs.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::VE_BUS:
            this->publish_state_(msg->data.ve_bus.device_state);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no device state.", this->parent_->address_str().c_str());
            this->publish_state("");
            break;
        }
      });
      break;

    default:
      break;
  }
}

void VictronBinarySensor::publish_state_(VE_REG_DEVICE_STATE device_state) {
  switch (this->type_) {
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_OFF:
      this->publish_state(device_state == VE_REG_DEVICE_STATE::OFF);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_FAULT:
      this->publish_state(device_state == VE_REG_DEVICE_STATE::FAULT);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_BULK:
      this->publish_state(device_state == VE_REG_DEVICE_STATE::BULK);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_ABSORPTION:
      this->publish_state(device_state == VE_REG_DEVICE_STATE::ABSORPTION);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_FLOAT:
      this->publish_state(device_state == VE_REG_DEVICE_STATE::FLOAT);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_EQUALIZE_MANUAL:
      this->publish_state(device_state == VE_REG_DEVICE_STATE::EQUALIZE_MANUAL);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_STARTING_UP:
      this->publish_state(device_state == VE_REG_DEVICE_STATE::STARTING_UP);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_AUTO_EQUALIZE:
      this->publish_state(device_state == VE_REG_DEVICE_STATE::AUTO_EQUALIZE);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_EXTERNAL_CONTROL:
      this->publish_state(device_state == VE_REG_DEVICE_STATE::EXTERNAL_CONTROL);
      break;
    default:
      break;
  }
}

}  // namespace victron_ble
}  // namespace esphome