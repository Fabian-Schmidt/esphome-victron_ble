#include "victron_binary_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace victron_ble {

static const char *const TAG = "victron_ble.binary_sensor";

// void VictronBinarySensor::dump_config() {
//   LOG_BINARY_SENSOR("", "Victron Binary Sensor", this);
//   ESP_LOGCONFIG(TAG, "  Type '%s'", enum_to_c_str(this->type_));
// }

void VictronBinarySensor::register_callback() {
  this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
    switch (this->type_) {
      case VICTRON_BINARY_SENSOR_TYPE::ALARM:
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
            ESP_LOGW(TAG, "[%s] Device has no `alarm` field.", this->parent_->address_str().c_str());
            this->publish_state("");
            break;
        }
        break;

      case VICTRON_BINARY_SENSOR_TYPE::CHARGER_ERROR:
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
          case VICTRON_BLE_RECORD_TYPE::AC_CHARGER:
            this->publish_state(msg->data.ac_charger.charger_error != VE_REG_CHR_ERROR_CODE::NO_ERROR);
            break;
          case VICTRON_BLE_RECORD_TYPE::SMART_BATTERY_PROTECT:
            this->publish_state(msg->data.smart_battery_protect.error_code != VE_REG_CHR_ERROR_CODE::NO_ERROR);
            break;
          case VICTRON_BLE_RECORD_TYPE::MULTI_RS:
            this->publish_state(msg->data.multi_rs.charger_error != VE_REG_CHR_ERROR_CODE::NO_ERROR);
            break;
          case VICTRON_BLE_RECORD_TYPE::ORION_XS:
            this->publish_state(msg->data.orion_xs.charger_error != VE_REG_CHR_ERROR_CODE::NO_ERROR);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no `charger error` field.", this->parent_->address_str().c_str());
            this->publish_state("");
            break;
        }
        break;

      case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_OFF:
      case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_LOW_POWER:
      case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_FAULT:
      case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_BULK:
      case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_ABSORPTION:
      case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_FLOAT:
      case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_STORAGE:
      case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_EQUALIZE_MANUAL:
      case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_INVERTING:
      case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_POWER_SUPPLY:
      case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_SUSTAIN:
      case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_STARTING_UP:
      case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_REPEATED_ABSORPTION:
      case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_AUTO_EQUALIZE:
      case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_BATTERY_SAFE:
      case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_LOAD_DETECT:
      case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_BLOCKED:
      case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_TEST:
      case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_EXTERNAL_CONTROL:
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
          case VICTRON_BLE_RECORD_TYPE::AC_CHARGER:
            this->publish_state_(msg->data.ac_charger.device_state);
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
          case VICTRON_BLE_RECORD_TYPE::ORION_XS:
            this->publish_state_(msg->data.orion_xs.device_state);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no `device state` field.", this->parent_->address_str().c_str());
            this->publish_state("");
            break;
        }
        break;

      case VICTRON_BINARY_SENSOR_TYPE::BMS_ALARM_OVER_VOLTAGE:
      case VICTRON_BINARY_SENSOR_TYPE::BMS_ALARM_UNDER_VOLTAGE:
      case VICTRON_BINARY_SENSOR_TYPE::BMS_WARN_UNDER_VOLTAGE:
      case VICTRON_BINARY_SENSOR_TYPE::BMS_ALARM_OVER_TEMPERATURE:
      case VICTRON_BINARY_SENSOR_TYPE::BMS_ALARM_UNDER_TEMPERATURE:
      case VICTRON_BINARY_SENSOR_TYPE::BMS_ALARM_HARDWARE_FAILURE:
      case VICTRON_BINARY_SENSOR_TYPE::BMS_ALLOWED_TO_CHARGE:
      case VICTRON_BINARY_SENSOR_TYPE::BMS_ALLOWED_TO_DISCHARGE:
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SMART_LITHIUM:
            this->publish_state_(msg->data.smart_lithium.bms_flags);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no `bms_flags` field.", this->parent_->address_str().c_str());
            this->publish_state("");
            break;
        }
        break;

      default:
        break;
    }
  });
}

void VictronBinarySensor::publish_state_(VE_REG_DEVICE_STATE device_state) {
  switch (this->type_) {
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_OFF:
      this->publish_state(device_state == VE_REG_DEVICE_STATE::OFF);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_LOW_POWER:
      this->publish_state(device_state == VE_REG_DEVICE_STATE::LOW_POWER);
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
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_STORAGE:
      this->publish_state(device_state == VE_REG_DEVICE_STATE::STORAGE);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_EQUALIZE_MANUAL:
      this->publish_state(device_state == VE_REG_DEVICE_STATE::EQUALIZE_MANUAL);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_INVERTING:
      this->publish_state(device_state == VE_REG_DEVICE_STATE::INVERTING);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_POWER_SUPPLY:
      this->publish_state(device_state == VE_REG_DEVICE_STATE::POWER_SUPPLY);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_SUSTAIN:
      this->publish_state(device_state == VE_REG_DEVICE_STATE::SUSTAIN);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_STARTING_UP:
      this->publish_state(device_state == VE_REG_DEVICE_STATE::STARTING_UP);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_REPEATED_ABSORPTION:
      this->publish_state(device_state == VE_REG_DEVICE_STATE::REPEATED_ABSORPTION);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_AUTO_EQUALIZE:
      this->publish_state(device_state == VE_REG_DEVICE_STATE::AUTO_EQUALIZE);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_BATTERY_SAFE:
      this->publish_state(device_state == VE_REG_DEVICE_STATE::BATTERY_SAFE);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_LOAD_DETECT:
      this->publish_state(device_state == VE_REG_DEVICE_STATE::LOAD_DETECT);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_BLOCKED:
      this->publish_state(device_state == VE_REG_DEVICE_STATE::BLOCKED);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_TEST:
      this->publish_state(device_state == VE_REG_DEVICE_STATE::TEST);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_EXTERNAL_CONTROL:
      this->publish_state(device_state == VE_REG_DEVICE_STATE::EXTERNAL_CONTROL);
      break;
    default:
      break;
  }
}

void VictronBinarySensor::publish_state_(VE_REG_BMS_FLAGs flags) {
  switch (this->type_) {
    case VICTRON_BINARY_SENSOR_TYPE::BMS_ALARM_OVER_VOLTAGE:
      this->publish_state((flags & VE_REG_BMS_FLAGs::ALARM_OVER_VOLTAGE) != VE_REG_BMS_FLAGs::NONE);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::BMS_ALARM_UNDER_VOLTAGE:
      this->publish_state((flags & VE_REG_BMS_FLAGs::ALARM_UNDER_VOLTAGE) != VE_REG_BMS_FLAGs::NONE);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::BMS_WARN_UNDER_VOLTAGE:
      this->publish_state((flags & VE_REG_BMS_FLAGs::WARN_UNDER_VOLTAGE) != VE_REG_BMS_FLAGs::NONE);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::BMS_ALARM_OVER_TEMPERATURE:
      this->publish_state((flags & VE_REG_BMS_FLAGs::ALARM_OVER_TEMPERATURE) != VE_REG_BMS_FLAGs::NONE);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::BMS_ALARM_UNDER_TEMPERATURE:
      this->publish_state((flags & VE_REG_BMS_FLAGs::ALARM_UNDER_TEMPERATURE) != VE_REG_BMS_FLAGs::NONE);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::BMS_ALARM_HARDWARE_FAILURE:
      this->publish_state((flags & VE_REG_BMS_FLAGs::ALARM_HARDWARE_FAILURE) != VE_REG_BMS_FLAGs::NONE);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::BMS_ALLOWED_TO_CHARGE:
      this->publish_state((flags & VE_REG_BMS_FLAGs::ALLOWED_TO_CHARGE) != VE_REG_BMS_FLAGs::NONE);
      break;
    case VICTRON_BINARY_SENSOR_TYPE::BMS_ALLOWED_TO_DISCHARGE:
      this->publish_state((flags & VE_REG_BMS_FLAGs::ALLOWED_TO_DISCHARGE) != VE_REG_BMS_FLAGs::NONE);
      break;
    default:
      break;
  }
}

}  // namespace victron_ble
}  // namespace esphome