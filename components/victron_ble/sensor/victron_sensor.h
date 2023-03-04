#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/components/victron_ble/victron_ble.h"

namespace esphome {
namespace victron_ble {

enum class VICTRON_SENSOR_TYPE {
  ACTIVE_AC_IN,
  ACTIVE_AC_IN_POWER,
  AC_APPARENT_POWER,
  AC_CURRENT,
  AC_OUT_POWER,
  AC_VOLTAGE,
  ALARM_REASON,
  AUX_VOLTAGE,
  BATTERY_CURRENT,
  BATTERY_VOLTAGE,
  BATTERY_POWER,
  CHARGER_ERROR,
  CONSUMED_AH,
  DEVICE_STATE,
  ERROR,
  INPUT_VOLTAGE,
  LOAD_CURRENT,
  MID_VOLTAGE,
  OFF_REASON,
  OUTPUT_VOLTAGE,
  PV_POWER,
  STATE_OF_CHARGE,
  TEMPERATURE,
  TIME_TO_GO,
  YIELD_TODAY,

  // SMART_LITHIUM
  BALANCER_STATUS,
  BMS_FLAGS,
  CELL1,
  CELL2,
  CELL3,
  CELL4,
  CELL5,
  CELL6,
  CELL7,
  CELL8,

  // SMART_BATTERY_PROTECT
  OUTPUT_STATE,
  WARNING_REASON,

  // LYNX_SMART_BMS
  IO_STATUS,
  WARNINGS_ALARMS,

  // VE_BUS
  ALARM,

  // DC_ENERGY_METER
  BMV_MONITOR_MODE,
};

class VictronSensor : public Component, public sensor::Sensor, public Parented<VictronBle> {
 public:
  void dump_config() override;
  void setup() override;

  void set_type(VICTRON_SENSOR_TYPE val) { this->type_ = val; }

 protected:
  VICTRON_SENSOR_TYPE type_;
};
}  // namespace victron_ble
}  // namespace esphome