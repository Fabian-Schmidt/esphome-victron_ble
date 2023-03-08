#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/components/victron_ble/victron_ble.h"

namespace esphome {
namespace victron_ble {

enum class VICTRON_SENSOR_TYPE {
  UNSET,

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

static const char *enum_to_c_str(const VICTRON_SENSOR_TYPE val) {
  switch (val) {
    case VICTRON_SENSOR_TYPE::UNSET:
      return "UNSET";
      break;
    case VICTRON_SENSOR_TYPE::ACTIVE_AC_IN:
      return "ACTIVE_AC_IN";
      break;
    case VICTRON_SENSOR_TYPE::ACTIVE_AC_IN_POWER:
      return "ACTIVE_AC_IN_POWER";
      break;
    case VICTRON_SENSOR_TYPE::AC_APPARENT_POWER:
      return "AC_APPARENT_POWER";
      break;
    case VICTRON_SENSOR_TYPE::AC_CURRENT:
      return "AC_CURRENT";
      break;
    case VICTRON_SENSOR_TYPE::AC_OUT_POWER:
      return "AC_OUT_POWER";
      break;
    case VICTRON_SENSOR_TYPE::AC_VOLTAGE:
      return "AC_VOLTAGE";
      break;
    case VICTRON_SENSOR_TYPE::ALARM_REASON:
      return "ALARM_REASON";
      break;
    case VICTRON_SENSOR_TYPE::AUX_VOLTAGE:
      return "AUX_VOLTAGE";
      break;
    case VICTRON_SENSOR_TYPE::BATTERY_CURRENT:
      return "BATTERY_CURRENT";
      break;
    case VICTRON_SENSOR_TYPE::BATTERY_VOLTAGE:
      return "BATTERY_VOLTAGE";
      break;
    case VICTRON_SENSOR_TYPE::BATTERY_POWER:
      return "BATTERY_POWER";
      break;
    case VICTRON_SENSOR_TYPE::CHARGER_ERROR:
      return "CHARGER_ERROR";
      break;
    case VICTRON_SENSOR_TYPE::CONSUMED_AH:
      return "CONSUMED_AH";
      break;
    case VICTRON_SENSOR_TYPE::DEVICE_STATE:
      return "DEVICE_STATE";
      break;
    case VICTRON_SENSOR_TYPE::ERROR:
      return "ERROR";
      break;
    case VICTRON_SENSOR_TYPE::INPUT_VOLTAGE:
      return "INPUT_VOLTAGE";
      break;
    case VICTRON_SENSOR_TYPE::LOAD_CURRENT:
      return "LOAD_CURRENT";
      break;
    case VICTRON_SENSOR_TYPE::MID_VOLTAGE:
      return "MID_VOLTAGE";
      break;
    case VICTRON_SENSOR_TYPE::OFF_REASON:
      return "OFF_REASON";
      break;
    case VICTRON_SENSOR_TYPE::OUTPUT_VOLTAGE:
      return "OUTPUT_VOLTAGE";
      break;
    case VICTRON_SENSOR_TYPE::PV_POWER:
      return "PV_POWER";
      break;
    case VICTRON_SENSOR_TYPE::STATE_OF_CHARGE:
      return "STATE_OF_CHARGE";
      break;
    case VICTRON_SENSOR_TYPE::TEMPERATURE:
      return "TEMPERATURE";
      break;
    case VICTRON_SENSOR_TYPE::TIME_TO_GO:
      return "TIME_TO_GO";
      break;
    case VICTRON_SENSOR_TYPE::YIELD_TODAY:
      return "YIELD_TODAY";
      break;

      // SMART_LITHIUM
    case VICTRON_SENSOR_TYPE::BALANCER_STATUS:
      return "BALANCER_STATUS";
      break;
    case VICTRON_SENSOR_TYPE::BMS_FLAGS:
      return "BMS_FLAGS";
      break;
    case VICTRON_SENSOR_TYPE::CELL1:
      return "CELL1";
      break;
    case VICTRON_SENSOR_TYPE::CELL2:
      return "CELL2";
      break;
    case VICTRON_SENSOR_TYPE::CELL3:
      return "CELL3";
      break;
    case VICTRON_SENSOR_TYPE::CELL4:
      return "CELL4";
      break;
    case VICTRON_SENSOR_TYPE::CELL5:
      return "CELL5";
      break;
    case VICTRON_SENSOR_TYPE::CELL6:
      return "CELL6";
      break;
    case VICTRON_SENSOR_TYPE::CELL7:
      return "CELL7";
      break;
    case VICTRON_SENSOR_TYPE::CELL8:
      return "CELL8";
      break;

      // SMART_BATTERY_PROTECT
    case VICTRON_SENSOR_TYPE::OUTPUT_STATE:
      return "OUTPUT_STATE";
      break;
    case VICTRON_SENSOR_TYPE::WARNING_REASON:
      return "WARNING_REASON";
      break;

      // LYNX_SMART_BMS
    case VICTRON_SENSOR_TYPE::IO_STATUS:
      return "IO_STATUS";
      break;
    case VICTRON_SENSOR_TYPE::WARNINGS_ALARMS:
      return "WARNINGS_ALARMS";
      break;

      // VE_BUS
    case VICTRON_SENSOR_TYPE::ALARM:
      return "ALARM";
      break;

      // DC_ENERGY_METER
    case VICTRON_SENSOR_TYPE::BMV_MONITOR_MODE:
      return "BMV_MONITOR_MODE";
      break;

    default:
      return "";
      break;
  }
}

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