#pragma once

#include "esphome/core/log.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/victron_ble/victron_ble.h"

namespace esphome {
namespace victron_ble {

enum class VICTRON_BINARY_SENSOR_TYPE {
  UNSET,
  ALARM,
  CHARGER_ERROR,
  DEVICE_STATE_OFF,
  DEVICE_STATE_LOW_POWER,
  DEVICE_STATE_FAULT,
  DEVICE_STATE_BULK,
  DEVICE_STATE_ABSORPTION,
  DEVICE_STATE_FLOAT,
  DEVICE_STATE_STORAGE,
  DEVICE_STATE_EQUALIZE_MANUAL,
  DEVICE_STATE_INVERTING,
  DEVICE_STATE_POWER_SUPPLY,
  DEVICE_STATE_SUSTAIN,
  DEVICE_STATE_STARTING_UP,
  DEVICE_STATE_REPEATED_ABSORPTION,
  DEVICE_STATE_AUTO_EQUALIZE,
  DEVICE_STATE_BATTERY_SAFE,
  DEVICE_STATE_LOAD_DETECT,
  DEVICE_STATE_BLOCKED,
  DEVICE_STATE_TEST,
  DEVICE_STATE_EXTERNAL_CONTROL,
  // BMS
  BMS_ALARM_OVER_VOLTAGE,
  BMS_ALARM_UNDER_VOLTAGE,
  BMS_WARN_UNDER_VOLTAGE,
  BMS_ALARM_OVER_TEMPERATURE,
  BMS_ALARM_UNDER_TEMPERATURE,
  BMS_ALARM_HARDWARE_FAILURE,
  BMS_ALLOWED_TO_CHARGE,
  BMS_ALLOWED_TO_DISCHARGE,
};

#ifdef ESPHOME_LOG_HAS_CONFIG
static const char *enum_to_c_str(const VICTRON_BINARY_SENSOR_TYPE val) {
  switch (val) {
    case VICTRON_BINARY_SENSOR_TYPE::UNSET:
      return "UNSET";
    case VICTRON_BINARY_SENSOR_TYPE::ALARM:
      return "ALARM";
    case VICTRON_BINARY_SENSOR_TYPE::CHARGER_ERROR:
      return "CHARGER_ERROR";
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_OFF:
      return "DEVICE_STATE_OFF";
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_LOW_POWER:
      return "DEVICE_STATE_LOW_POWER";
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_FAULT:
      return "DEVICE_STATE_FAULT";
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_BULK:
      return "DEVICE_STATE_BULK";
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_ABSORPTION:
      return "DEVICE_STATE_ABSORPTION";
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_FLOAT:
      return "DEVICE_STATE_FLOAT";
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_STORAGE:
      return "DEVICE_STATE_STORAGE";
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_EQUALIZE_MANUAL:
      return "DEVICE_STATE_EQUALIZE_MANUAL";
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_INVERTING:
      return "DEVICE_STATE_INVERTING";
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_POWER_SUPPLY:
      return "DEVICE_STATE_POWER_SUPPLY";
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_SUSTAIN:
      return "DEVICE_STATE_SUSTAIN";
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_STARTING_UP:
      return "DEVICE_STATE_STARTING_UP";
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_REPEATED_ABSORPTION:
      return "DEVICE_STATE_REPEATED_ABSORPTION";
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_AUTO_EQUALIZE:
      return "DEVICE_STATE_AUTO_EQUALIZE";
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_BATTERY_SAFE:
      return "DEVICE_STATE_BATTERY_SAFE";
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_LOAD_DETECT:
      return "DEVICE_STATE_LOAD_DETECT";
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_BLOCKED:
      return "DEVICE_STATE_BLOCKED";
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_TEST:
      return "DEVICE_STATE_TEST";
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_EXTERNAL_CONTROL:
      return "DEVICE_STATE_EXTERNAL_CONTROL";
    //BMS
    case VICTRON_BINARY_SENSOR_TYPE::BMS_ALARM_OVER_VOLTAGE:
      return "BMS_ALARM_OVER_VOLTAGE";
    case VICTRON_BINARY_SENSOR_TYPE::BMS_ALARM_UNDER_VOLTAGE:
      return "BMS_ALARM_UNDER_VOLTAGE";
    case VICTRON_BINARY_SENSOR_TYPE::BMS_WARN_UNDER_VOLTAGE:
      return "BMS_WARN_UNDER_VOLTAGE";
    case VICTRON_BINARY_SENSOR_TYPE::BMS_ALARM_OVER_TEMPERATURE:
      return "BMS_ALARM_OVER_TEMPERATURE";
    case VICTRON_BINARY_SENSOR_TYPE::BMS_ALARM_UNDER_TEMPERATURE:
      return "BMS_ALARM_UNDER_TEMPERATURE";
    case VICTRON_BINARY_SENSOR_TYPE::BMS_ALARM_HARDWARE_FAILURE:
      return "BMS_ALARM_HARDWARE_FAILURE";
    case VICTRON_BINARY_SENSOR_TYPE::BMS_ALLOWED_TO_CHARGE:
      return "BMS_ALLOWED_TO_CHARGE";
    case VICTRON_BINARY_SENSOR_TYPE::BMS_ALLOWED_TO_DISCHARGE:
      return "BMS_ALLOWED_TO_DISCHARGE";

    default:
      return "";
  }
}
#endif  // ESPHOME_LOG_HAS_CONFIG

class VictronBinarySensor : public Component, public binary_sensor::BinarySensor, public Parented<VictronBle> {
 public:
  void dump_config() override;
  void setup() override;

  void set_type(VICTRON_BINARY_SENSOR_TYPE val) { this->type_ = val; }

 protected:
  VICTRON_BINARY_SENSOR_TYPE type_;

  void publish_state_(VE_REG_DEVICE_STATE device_state);
  void publish_state_(VE_REG_BMS_FLAGs flags);
};
}  // namespace victron_ble
}  // namespace esphome