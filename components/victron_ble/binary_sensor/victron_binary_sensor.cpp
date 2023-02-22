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
    case VICTRON_BINARY_SENSOR_TYPE::BATTERY_MONITOR_ALARM:
      this->parent_->add_on_battery_monitor_message_callback(
          [this](const VICTRON_BLE_RECORD_BATTERY_MONITOR *battery_monitor) {
            switch (this->type_) {
              case VICTRON_BINARY_SENSOR_TYPE::BATTERY_MONITOR_ALARM:
                this->publish_state(battery_monitor->alarm_reason != VE_REG_ALARM_REASON::NO_ALARM);
                break;
              default:
                break;
            }
          });
      break;
    case VICTRON_BINARY_SENSOR_TYPE::SOLAR_CHARGER_DEVICE_STATE_OFF:
    case VICTRON_BINARY_SENSOR_TYPE::SOLAR_CHARGER_DEVICE_STATE_FAULT:
    case VICTRON_BINARY_SENSOR_TYPE::SOLAR_CHARGER_DEVICE_STATE_BULK:
    case VICTRON_BINARY_SENSOR_TYPE::SOLAR_CHARGER_DEVICE_STATE_ABSORPTION:
    case VICTRON_BINARY_SENSOR_TYPE::SOLAR_CHARGER_DEVICE_STATE_FLOAT:
    case VICTRON_BINARY_SENSOR_TYPE::SOLAR_CHARGER_DEVICE_STATE_EQUALIZE_MANUAL:
    case VICTRON_BINARY_SENSOR_TYPE::SOLAR_CHARGER_DEVICE_STATE_STARTING_UP:
    case VICTRON_BINARY_SENSOR_TYPE::SOLAR_CHARGER_DEVICE_STATE_AUTO_EQUALIZE:
    case VICTRON_BINARY_SENSOR_TYPE::SOLAR_CHARGER_DEVICE_STATE_EXTERNAL_CONTROL:
    case VICTRON_BINARY_SENSOR_TYPE::SOLAR_CHARGER_CHARGER_ERROR:
      this->parent_->add_on_solar_charger_message_callback([this](const VICTRON_BLE_RECORD_SOLAR_CHARGER *solar) {
        switch (this->type_) {
          case VICTRON_BINARY_SENSOR_TYPE::SOLAR_CHARGER_DEVICE_STATE_OFF:
            this->publish_state(solar->device_state == VE_REG_DEVICE_STATE::OFF);
            break;
          case VICTRON_BINARY_SENSOR_TYPE::SOLAR_CHARGER_DEVICE_STATE_FAULT:
            this->publish_state(solar->device_state == VE_REG_DEVICE_STATE::FAULT);
            break;
          case VICTRON_BINARY_SENSOR_TYPE::SOLAR_CHARGER_DEVICE_STATE_BULK:
            this->publish_state(solar->device_state == VE_REG_DEVICE_STATE::BULK);
            break;
          case VICTRON_BINARY_SENSOR_TYPE::SOLAR_CHARGER_DEVICE_STATE_ABSORPTION:
            this->publish_state(solar->device_state == VE_REG_DEVICE_STATE::ABSORPTION);
            break;
          case VICTRON_BINARY_SENSOR_TYPE::SOLAR_CHARGER_DEVICE_STATE_FLOAT:
            this->publish_state(solar->device_state == VE_REG_DEVICE_STATE::FLOAT);
            break;
          case VICTRON_BINARY_SENSOR_TYPE::SOLAR_CHARGER_DEVICE_STATE_EQUALIZE_MANUAL:
            this->publish_state(solar->device_state == VE_REG_DEVICE_STATE::EQUALIZE_MANUAL);
            break;
          case VICTRON_BINARY_SENSOR_TYPE::SOLAR_CHARGER_DEVICE_STATE_STARTING_UP:
            this->publish_state(solar->device_state == VE_REG_DEVICE_STATE::STARTING_UP);
            break;
          case VICTRON_BINARY_SENSOR_TYPE::SOLAR_CHARGER_DEVICE_STATE_AUTO_EQUALIZE:
            this->publish_state(solar->device_state == VE_REG_DEVICE_STATE::AUTO_EQUALIZE);
            break;
          case VICTRON_BINARY_SENSOR_TYPE::SOLAR_CHARGER_DEVICE_STATE_EXTERNAL_CONTROL:
            this->publish_state(solar->device_state == VE_REG_DEVICE_STATE::EXTERNAL_CONTROL);
            break;
          case VICTRON_BINARY_SENSOR_TYPE::SOLAR_CHARGER_CHARGER_ERROR:
            this->publish_state(solar->charger_error != VE_REG_CHR_ERROR_CODE::NO_ERROR);
            break;
          default:
            break;
        }
      });
      break;
    case VICTRON_BINARY_SENSOR_TYPE::INVERTER_ALARM:
      this->parent_->add_on_inverter_message_callback([this](const VICTRON_BLE_RECORD_INVERTER *val) {
        switch (this->type_) {
          case VICTRON_BINARY_SENSOR_TYPE::INVERTER_ALARM:
            this->publish_state(val->alarm_reason != VE_REG_ALARM_REASON::NO_ALARM);
            break;
          default:
            break;
        }
      });
    default:
      break;
  }
}

}  // namespace victron_ble
}  // namespace esphome