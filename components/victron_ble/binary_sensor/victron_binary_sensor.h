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
  DEVICE_STATE_STARTING_UP,
  DEVICE_STATE_REPEATED_ABSORPTION,
  DEVICE_STATE_AUTO_EQUALIZE,
  DEVICE_STATE_BATTERY_SAFE,
  DEVICE_STATE_EXTERNAL_CONTROL,
};

#ifdef ESPHOME_LOG_HAS_CONFIG
static const char *enum_to_c_str(const VICTRON_BINARY_SENSOR_TYPE val) {
  switch (val) {
    case VICTRON_BINARY_SENSOR_TYPE::UNSET:
      return "UNSET";
      break;
    case VICTRON_BINARY_SENSOR_TYPE::ALARM:
      return "ALARM";
      break;
    case VICTRON_BINARY_SENSOR_TYPE::CHARGER_ERROR:
      return "CHARGER_ERROR";
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_OFF:
      return "DEVICE_STATE_OFF";
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_LOW_POWER:
      return "DEVICE_STATE_LOW_POWER";
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_FAULT:
      return "DEVICE_STATE_FAULT";
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_BULK:
      return "DEVICE_STATE_BULK";
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_ABSORPTION:
      return "DEVICE_STATE_ABSORPTION";
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_FLOAT:
      return "DEVICE_STATE_FLOAT";
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_STORAGE:
      return "DEVICE_STATE_STORAGE";
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_EQUALIZE_MANUAL:
      return "DEVICE_STATE_EQUALIZE_MANUAL";
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_INVERTING:
      return "DEVICE_STATE_INVERTING";
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_POWER_SUPPLY:
      return "DEVICE_STATE_POWER_SUPPLY";
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_STARTING_UP:
      return "DEVICE_STATE_STARTING_UP";
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_REPEATED_ABSORPTION:
      return "DEVICE_STATE_REPEATED_ABSORPTION";
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_AUTO_EQUALIZE:
      return "DEVICE_STATE_AUTO_EQUALIZE";
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_BATTERY_SAFE:
      return "DEVICE_STATE_BATTERY_SAFE";
      break;
    case VICTRON_BINARY_SENSOR_TYPE::DEVICE_STATE_EXTERNAL_CONTROL:
      return "DEVICE_STATE_EXTERNAL_CONTROL";
      break;
    default:
      return "";
      break;
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
};
}  // namespace victron_ble
}  // namespace esphome