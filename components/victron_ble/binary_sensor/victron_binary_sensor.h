#pragma once

#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/victron_ble/victron_ble.h"

namespace esphome {
namespace victron_ble {

enum class VICTRON_BINARY_SENSOR_TYPE {
  BATTERY_MONITOR_ALARM,

  SOLAR_CHARGER_DEVICE_STATE_OFF,
  SOLAR_CHARGER_DEVICE_STATE_FAULT,
  SOLAR_CHARGER_DEVICE_STATE_BULK,
  SOLAR_CHARGER_DEVICE_STATE_ABSORPTION,
  SOLAR_CHARGER_DEVICE_STATE_FLOAT,
  SOLAR_CHARGER_DEVICE_STATE_EQUALIZE_MANUAL,
  SOLAR_CHARGER_DEVICE_STATE_STARTING_UP,
  SOLAR_CHARGER_DEVICE_STATE_AUTO_EQUALIZE,
  SOLAR_CHARGER_DEVICE_STATE_EXTERNAL_CONTROL,
  SOLAR_CHARGER_CHARGER_ERROR,

  INVERTER_ALARM,
};

class VictronBinarySensor : public Component, public binary_sensor::BinarySensor, public Parented<VictronBle> {
 public:
  void dump_config() override;
  void setup() override;

  void set_type(VICTRON_BINARY_SENSOR_TYPE val) { this->type_ = val; }

 protected:
  VICTRON_BINARY_SENSOR_TYPE type_;
};
}  // namespace victron_ble
}  // namespace esphome