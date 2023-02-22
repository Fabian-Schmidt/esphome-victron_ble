#pragma once

#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/victron_ble/victron_ble.h"

namespace esphome {
namespace victron_ble {

enum class VICTRON_TEXT_SENSOR_TYPE {
  BATTERY_MONITOR_ALARM_REASON,

  SOLAR_CHARGER_DEVICE_STATE,
  SOLAR_CHARGER_CHARGER_ERROR,

  INVERTER_DEVICE_STATE,
  INVERTER_ALARM_REASON,

  DCDC_CONVERTER_DEVICE_STATE,
  DCDC_CONVERTER_CHARGER_ERROR,
  DCDC_CONVERTER_OFF_REASON,

  INVERTER_RS_DEVICE_STATE,
  INVERTER_RS_CHARGER_ERROR,

  SMART_BATTERY_PROTECT_DEVICE_STATE,
  SMART_BATTERY_PROTECT_ERROR_CODE,
  SMART_BATTERY_PROTECT_ALARM_REASON,
  SMART_BATTERY_PROTECT_WARNING_REASON,
  SMART_BATTERY_PROTECT_OFF_REASON,

  MULTI_RS_DEVICE_STATE,
  MULTI_RS_CHARGER_ERROR,
  MULTI_RS_ACTIVE_AC_IN,
};

class VictronTextSensor : public Component, public text_sensor::TextSensor, public Parented<VictronBle> {
 public:
  void dump_config() override;
  void setup() override;

  void set_type(VICTRON_TEXT_SENSOR_TYPE val) { this->type_ = val; }

 protected:
  VICTRON_TEXT_SENSOR_TYPE type_;

  void publish_state_(VE_REG_ALARM_REASON val);
  void publish_state_(VE_REG_DEVICE_STATE val);
  void publish_state_(VE_REG_CHR_ERROR_CODE val);
  void publish_state_(VE_REG_DEVICE_OFF_REASON_2 val);
  void publish_state_(VE_REG_AC_IN_ACTIVE val);
};
}  // namespace victron_ble
}  // namespace esphome