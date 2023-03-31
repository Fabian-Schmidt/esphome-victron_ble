#pragma once

#include "esphome/core/log.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/victron_ble/victron_ble.h"

namespace esphome {
namespace victron_ble {

enum class VICTRON_TEXT_SENSOR_TYPE {
  UNSET,
  ALARM,
  ACTIVE_AC_IN,
  ALARM_REASON,
  CHARGER_ERROR,
  DEVICE_STATE,
  ERROR_CODE,
  OFF_REASON,
  WARNING_REASON,
};

#ifdef ESPHOME_LOG_HAS_CONFIG
static const char *enum_to_c_str(const VICTRON_TEXT_SENSOR_TYPE val) {
  switch (val) {
    case VICTRON_TEXT_SENSOR_TYPE::UNSET:
      return "UNSET";
      break;
    case VICTRON_TEXT_SENSOR_TYPE::ALARM:
      return "ALARM";
      break;
    case VICTRON_TEXT_SENSOR_TYPE::ACTIVE_AC_IN:
      return "ACTIVE_AC_IN";
      break;
    case VICTRON_TEXT_SENSOR_TYPE::ALARM_REASON:
      return "ALARM_REASON";
      break;
    case VICTRON_TEXT_SENSOR_TYPE::CHARGER_ERROR:
      return "CHARGER_ERROR";
      break;
    case VICTRON_TEXT_SENSOR_TYPE::DEVICE_STATE:
      return "DEVICE_STATE";
      break;
    case VICTRON_TEXT_SENSOR_TYPE::ERROR_CODE:
      return "ERROR_CODE";
      break;
    case VICTRON_TEXT_SENSOR_TYPE::OFF_REASON:
      return "OFF_REASON";
      break;
    case VICTRON_TEXT_SENSOR_TYPE::WARNING_REASON:
      return "WARNING_REASON";
      break;
    default:
      return "";
      break;
  }
}
#endif  // ESPHOME_LOG_HAS_CONFIG

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
  void publish_state_(VE_REG_ALARM_NOTIFICATION val);
};
}  // namespace victron_ble
}  // namespace esphome