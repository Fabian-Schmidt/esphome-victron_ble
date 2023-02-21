#include "victron_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace victron_ble {

static const char *const TAG = "victron_ble.sensor";

void VictronSensor::dump_config() {
  LOG_SENSOR("", "Victron Sensor", this);
  ESP_LOGCONFIG(TAG, "Type %u", this->type_);
}

void VictronSensor::setup() {
  switch (this->type_) {
    case VICTRON_SENSOR_TYPE::BATTERY_MONITOR_TIME_TO_GO:
    case VICTRON_SENSOR_TYPE::BATTERY_MONITOR_BATTERY_VOLTAGE:
    case VICTRON_SENSOR_TYPE::BATTERY_MONITOR_ALARM_REASON:
    case VICTRON_SENSOR_TYPE::BATTERY_MONITOR_AUX_VOLTAGE:
    case VICTRON_SENSOR_TYPE::BATTERY_MONITOR_MID_VOLTAGE:
    case VICTRON_SENSOR_TYPE::BATTERY_MONITOR_TEMPERATURE:
    case VICTRON_SENSOR_TYPE::BATTERY_MONITOR_BATTERY_CURRENT:
    case VICTRON_SENSOR_TYPE::BATTERY_MONITOR_CONSUMED_AH:
    case VICTRON_SENSOR_TYPE::BATTERY_MONITOR_STATE_OF_CHARGE:
      this->parent_->add_on_battery_monitor_message_callback(
          [this](const VICTRON_BLE_RECORD_BATTERY_MONITOR *battery_monitor) {
            switch (this->type_) {
              case VICTRON_SENSOR_TYPE::BATTERY_MONITOR_TIME_TO_GO:
                this->publish_state(0.1f * battery_monitor->time_to_go);
                break;
              case VICTRON_SENSOR_TYPE::BATTERY_MONITOR_BATTERY_VOLTAGE:
                this->publish_state(0.01f * battery_monitor->battery_voltage);
                break;
              case VICTRON_SENSOR_TYPE::BATTERY_MONITOR_ALARM_REASON:
                this->publish_state(battery_monitor->alarm_reason);
                break;
              case VICTRON_SENSOR_TYPE::BATTERY_MONITOR_AUX_VOLTAGE:
                if (battery_monitor->aux_input_type == VICTRON_AUX_INPUT_TYPE::VE_REG_DC_CHANNEL2_VOLTAGE) {
                  this->publish_state(0.01f * battery_monitor->aux_input.aux_voltage);
                } else {
                  ESP_LOGW(TAG, "[%s] Incorrect Aux input configuration in Smart Shunt.",
                           this->parent_->address_str().c_str());
                  this->publish_state(NAN);
                }
                break;
              case VICTRON_SENSOR_TYPE::BATTERY_MONITOR_MID_VOLTAGE:
                if (battery_monitor->aux_input_type == VICTRON_AUX_INPUT_TYPE::VE_REG_BATTERY_MID_POINT_VOLTAGE) {
                  this->publish_state(0.01f * battery_monitor->aux_input.mid_voltage);
                } else {
                  ESP_LOGW(TAG, "[%s] Incorrect Aux input configuration in Smart Shunt.",
                           this->parent_->address_str().c_str());
                  this->publish_state(NAN);
                }
                break;
              case VICTRON_SENSOR_TYPE::BATTERY_MONITOR_TEMPERATURE:
                if (battery_monitor->aux_input_type == VICTRON_AUX_INPUT_TYPE::VE_REG_BAT_TEMPERATURE) {
                  this->publish_state(0.01f * battery_monitor->aux_input.temperature - 273.15f);
                } else {
                  ESP_LOGW(TAG, "[%s] Incorrect Aux input configuration in Smart Shunt.",
                           this->parent_->address_str().c_str());
                  this->publish_state(NAN);
                }
                break;
              case VICTRON_SENSOR_TYPE::BATTERY_MONITOR_BATTERY_CURRENT:
                this->publish_state(0.001f * battery_monitor->battery_current);
                break;
              case VICTRON_SENSOR_TYPE::BATTERY_MONITOR_CONSUMED_AH:
                this->publish_state(-0.1f * battery_monitor->consumed_ah);
                break;
              case VICTRON_SENSOR_TYPE::BATTERY_MONITOR_STATE_OF_CHARGE:
                this->publish_state(0.1f * battery_monitor->state_of_charge);
                break;
              default:
                break;
            }
          });
      break;

    default:
      break;
  }
}

}  // namespace victron_ble
}  // namespace esphome