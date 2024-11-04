#pragma once

#include "esphome/core/log.h"
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

  // AC_CHARGER
  BATTERY_CURRENT_2,
  BATTERY_VOLTAGE_2,
  BATTERY_POWER_2,
  BATTERY_CURRENT_3,
  BATTERY_VOLTAGE_3,
  BATTERY_POWER_3,

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

  // ORION_XS
  OUTPUT_CURRENT,
  INPUT_CURRENT,
};

#ifdef ESPHOME_LOG_HAS_CONFIG
static const char *enum_to_c_str(const VICTRON_SENSOR_TYPE val) {
  switch (val) {
    case VICTRON_SENSOR_TYPE::UNSET:
      return "UNSET";
    case VICTRON_SENSOR_TYPE::ACTIVE_AC_IN:
      return "ACTIVE_AC_IN";
    case VICTRON_SENSOR_TYPE::ACTIVE_AC_IN_POWER:
      return "ACTIVE_AC_IN_POWER";
    case VICTRON_SENSOR_TYPE::AC_APPARENT_POWER:
      return "AC_APPARENT_POWER";
    case VICTRON_SENSOR_TYPE::AC_CURRENT:
      return "AC_CURRENT";
    case VICTRON_SENSOR_TYPE::AC_OUT_POWER:
      return "AC_OUT_POWER";
    case VICTRON_SENSOR_TYPE::AC_VOLTAGE:
      return "AC_VOLTAGE";
    case VICTRON_SENSOR_TYPE::ALARM_REASON:
      return "ALARM_REASON";
    case VICTRON_SENSOR_TYPE::AUX_VOLTAGE:
      return "AUX_VOLTAGE";
    case VICTRON_SENSOR_TYPE::BATTERY_CURRENT:
      return "BATTERY_CURRENT";
    case VICTRON_SENSOR_TYPE::BATTERY_VOLTAGE:
      return "BATTERY_VOLTAGE";
    case VICTRON_SENSOR_TYPE::BATTERY_POWER:
      return "BATTERY_POWER";
    case VICTRON_SENSOR_TYPE::CHARGER_ERROR:
      return "CHARGER_ERROR";
    case VICTRON_SENSOR_TYPE::CONSUMED_AH:
      return "CONSUMED_AH";
    case VICTRON_SENSOR_TYPE::DEVICE_STATE:
      return "DEVICE_STATE";
    case VICTRON_SENSOR_TYPE::ERROR:
      return "ERROR";
    case VICTRON_SENSOR_TYPE::INPUT_VOLTAGE:
      return "INPUT_VOLTAGE";
    case VICTRON_SENSOR_TYPE::LOAD_CURRENT:
      return "LOAD_CURRENT";
    case VICTRON_SENSOR_TYPE::MID_VOLTAGE:
      return "MID_VOLTAGE";
    case VICTRON_SENSOR_TYPE::OFF_REASON:
      return "OFF_REASON";
    case VICTRON_SENSOR_TYPE::OUTPUT_VOLTAGE:
      return "OUTPUT_VOLTAGE";
    case VICTRON_SENSOR_TYPE::PV_POWER:
      return "PV_POWER";
    case VICTRON_SENSOR_TYPE::STATE_OF_CHARGE:
      return "STATE_OF_CHARGE";
    case VICTRON_SENSOR_TYPE::TEMPERATURE:
      return "TEMPERATURE";
    case VICTRON_SENSOR_TYPE::TIME_TO_GO:
      return "TIME_TO_GO";
    case VICTRON_SENSOR_TYPE::YIELD_TODAY:
      return "YIELD_TODAY";

      // SMART_LITHIUM
    case VICTRON_SENSOR_TYPE::BALANCER_STATUS:
      return "BALANCER_STATUS";
    case VICTRON_SENSOR_TYPE::BMS_FLAGS:
      return "BMS_FLAGS";
    case VICTRON_SENSOR_TYPE::CELL1:
      return "CELL1";
    case VICTRON_SENSOR_TYPE::CELL2:
      return "CELL2";
    case VICTRON_SENSOR_TYPE::CELL3:
      return "CELL3";
    case VICTRON_SENSOR_TYPE::CELL4:
      return "CELL4";
    case VICTRON_SENSOR_TYPE::CELL5:
      return "CELL5";
    case VICTRON_SENSOR_TYPE::CELL6:
      return "CELL6";
    case VICTRON_SENSOR_TYPE::CELL7:
      return "CELL7";
    case VICTRON_SENSOR_TYPE::CELL8:
      return "CELL8";

      //AC_CHARGER
    case VICTRON_SENSOR_TYPE::BATTERY_CURRENT_2:
      return "BATTERY_CURRENT_2";
    case VICTRON_SENSOR_TYPE::BATTERY_VOLTAGE_2:
      return "BATTERY_VOLTAGE_2";
    case VICTRON_SENSOR_TYPE::BATTERY_POWER_2:
      return "BATTERY_POWER_2";
    case VICTRON_SENSOR_TYPE::BATTERY_CURRENT_3:
      return "BATTERY_CURRENT_3";
    case VICTRON_SENSOR_TYPE::BATTERY_VOLTAGE_3:
      return "BATTERY_VOLTAGE_3";
    case VICTRON_SENSOR_TYPE::BATTERY_POWER_3:
      return "BATTERY_POWER_3";
    
      // SMART_BATTERY_PROTECT
    case VICTRON_SENSOR_TYPE::OUTPUT_STATE:
      return "OUTPUT_STATE";
    case VICTRON_SENSOR_TYPE::WARNING_REASON:
      return "WARNING_REASON";

      // LYNX_SMART_BMS
    case VICTRON_SENSOR_TYPE::IO_STATUS:
      return "IO_STATUS";
    case VICTRON_SENSOR_TYPE::WARNINGS_ALARMS:
      return "WARNINGS_ALARMS";

      // VE_BUS
    case VICTRON_SENSOR_TYPE::ALARM:
      return "ALARM";

      // DC_ENERGY_METER
    case VICTRON_SENSOR_TYPE::BMV_MONITOR_MODE:
      return "BMV_MONITOR_MODE";

    // ORION_XS
    case VICTRON_SENSOR_TYPE::OUTPUT_CURRENT:
      return "OUTPUT_CURRENT";
    case VICTRON_SENSOR_TYPE::INPUT_CURRENT:
      return "INPUT_CURRENT";

    default:
      return "";
  }
}
#endif  // ESPHOME_LOG_HAS_CONFIG

class VictronSensor : public Component, public sensor::Sensor, public Parented<VictronBle> {
 public:
  void dump_config() override;
  void setup() override;

  void set_type(VICTRON_SENSOR_TYPE val) { this->type_ = val; }

 protected:
  VICTRON_SENSOR_TYPE type_;

  inline void publish_state_(battery_voltage_16bit_0_01V val) {
    if ((int16_t)val == 0x7FFF) {
      this->publish_state(NAN);
    } else {
      this->publish_state(0.01f * (int16_t)val);
    }
  };

  inline void publish_state_(battery_voltage_13bit_0_01V_positiv val) {
    if ((u_int16_t)val == 0x1FFF) {
      this->publish_state(NAN);
    } else {
      this->publish_state(0.01f * (u_int16_t)val);
    }
  };

  inline void publish_state_(battery_current_11bit_0_1A_positiv val) {
    if ((u_int16_t)val == 0x7FF) {
      this->publish_state(NAN);
    } else {
      this->publish_state(0.1f * (u_int16_t)val);
    }
  };

  inline void publish_state_(battery_current_11bit_0_1A_positiv val_C, battery_voltage_13bit_0_01V_positiv val_V) {
    if ((u_int16_t)val_C == 0x7FF || (u_int16_t)val_V == 0x1FFF) {
      this->publish_state(NAN);
    } else {
      this->publish_state((0.1f * (u_int16_t)val_C) * (0.01f * (u_int16_t)val_V));
    }
  };

  inline void publish_state_(temperature_7bit val) {
    if ((u_int8_t)val == 0x7F) {
      this->publish_state(NAN);
    } else {
      this->publish_state(-40.0f + (u_int8_t)val);
    }
  };

  inline void publish_state_(ac_current_9bit_0_1A_positiv val) {
    if ((u_int16_t)val == 0x1FF) {
      this->publish_state(NAN);
    } else {
      this->publish_state(0.1f * (u_int16_t)val);
    }
  };
  
};
}  // namespace victron_ble
}  // namespace esphome
