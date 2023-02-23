#pragma once

#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/victron_ble/victron_ble.h"

namespace esphome {
namespace victron_ble {

enum class VICTRON_BINARY_SENSOR_TYPE {
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