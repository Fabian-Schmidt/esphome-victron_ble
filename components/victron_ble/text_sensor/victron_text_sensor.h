#pragma once

#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/victron_ble/victron_ble.h"

namespace esphome {
namespace victron_ble {

enum class VICTRON_TEXT_SENSOR_TYPE {
  BATTERY_MONITOR_ALARM_REASON,

  SOLAR_CHARGER_DEVICE_STATE,
  SOLAR_CHARGER_CHARGER_ERROR,
};

class VictronTextSensor : public Component, public text_sensor::TextSensor, public Parented<VictronBle> {
 public:
  void dump_config() override;
  void setup() override;

  void set_type(VICTRON_TEXT_SENSOR_TYPE val) { this->type_ = val; }

 protected:
  VICTRON_TEXT_SENSOR_TYPE type_;
};
}  // namespace victron_ble
}  // namespace esphome