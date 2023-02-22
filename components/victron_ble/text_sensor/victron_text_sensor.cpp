#include "victron_text_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace victron_ble {

static const char *const TAG = "victron_ble.text_sensor";

void VictronTextSensor::dump_config() {
  LOG_TEXT_SENSOR("", "Victron Text Sensor", this);
  ESP_LOGCONFIG(TAG, "Type %u", this->type_);
}

void VictronTextSensor::setup() {
  switch (this->type_) {
    case VICTRON_TEXT_SENSOR_TYPE::BATTERY_MONITOR_ALARM_REASON:
      this->parent_->add_on_battery_monitor_message_callback(
          [this](const VICTRON_BLE_RECORD_BATTERY_MONITOR *battery_monitor) {
            switch (this->type_) {
              case VICTRON_TEXT_SENSOR_TYPE::BATTERY_MONITOR_ALARM_REASON:
                this->publish_state_(battery_monitor->alarm_reason);
                break;
              default:
                break;
            }
          });
      break;
    case VICTRON_TEXT_SENSOR_TYPE::SOLAR_CHARGER_DEVICE_STATE:
    case VICTRON_TEXT_SENSOR_TYPE::SOLAR_CHARGER_CHARGER_ERROR:
      this->parent_->add_on_solar_charger_message_callback([this](const VICTRON_BLE_RECORD_SOLAR_CHARGER *solar) {
        switch (this->type_) {
          case VICTRON_TEXT_SENSOR_TYPE::SOLAR_CHARGER_DEVICE_STATE:
            this->publish_state_(solar->device_state);
            break;
          case VICTRON_TEXT_SENSOR_TYPE::SOLAR_CHARGER_CHARGER_ERROR:
            this->publish_state_(solar->charger_error);
            break;
          default:
            break;
        }
      });
      break;
    case VICTRON_TEXT_SENSOR_TYPE::INVERTER_DEVICE_STATE:
    case VICTRON_TEXT_SENSOR_TYPE::INVERTER_ALARM_REASON:
      this->parent_->add_on_inverter_message_callback([this](const VICTRON_BLE_RECORD_INVERTER *val) {
        switch (this->type_) {
          case VICTRON_TEXT_SENSOR_TYPE::INVERTER_DEVICE_STATE:
            this->publish_state_(val->device_state);
            break;
          case VICTRON_TEXT_SENSOR_TYPE::INVERTER_ALARM_REASON:
            this->publish_state_(val->alarm_reason);
            break;
          default:
            break;
        }
      });
      break;
    case VICTRON_TEXT_SENSOR_TYPE::DCDC_CONVERTER_DEVICE_STATE:
    case VICTRON_TEXT_SENSOR_TYPE::DCDC_CONVERTER_CHARGER_ERROR:
    case VICTRON_TEXT_SENSOR_TYPE::DCDC_CONVERTER_OFF_REASON:
      this->parent_->add_on_dcdc_converter_message_callback([this](const VICTRON_BLE_RECORD_DCDC_CONVERTER *val) {
        switch (this->type_) {
          case VICTRON_TEXT_SENSOR_TYPE::DCDC_CONVERTER_DEVICE_STATE:
            this->publish_state_(val->device_state);
            break;
          case VICTRON_TEXT_SENSOR_TYPE::DCDC_CONVERTER_CHARGER_ERROR:
            this->publish_state_(val->charger_error);
            break;
          case VICTRON_TEXT_SENSOR_TYPE::DCDC_CONVERTER_OFF_REASON:
            this->publish_state_(val->off_reason);
            break;
          default:
            break;
        }
      });
      break;
    case VICTRON_TEXT_SENSOR_TYPE::INVERTER_RS_DEVICE_STATE:
    case VICTRON_TEXT_SENSOR_TYPE::INVERTER_RS_CHARGER_ERROR:
      this->parent_->add_on_inverter_rs_message_callback([this](const VICTRON_BLE_RECORD_INVERTER_RS *val) {
        switch (this->type_) {
          case VICTRON_TEXT_SENSOR_TYPE::INVERTER_RS_DEVICE_STATE:
            this->publish_state_(val->device_state);
            break;
          case VICTRON_TEXT_SENSOR_TYPE::INVERTER_RS_CHARGER_ERROR:
            this->publish_state_(val->charger_error);
            break;
          default:
            break;
        }
      });
      break;
    case VICTRON_TEXT_SENSOR_TYPE::SMART_BATTERY_PROTECT_DEVICE_STATE:
    case VICTRON_TEXT_SENSOR_TYPE::SMART_BATTERY_PROTECT_ERROR_CODE:
    case VICTRON_TEXT_SENSOR_TYPE::SMART_BATTERY_PROTECT_ALARM_REASON:
    case VICTRON_TEXT_SENSOR_TYPE::SMART_BATTERY_PROTECT_WARNING_REASON:
    case VICTRON_TEXT_SENSOR_TYPE::SMART_BATTERY_PROTECT_OFF_REASON:
      this->parent_->add_on_smart_battery_protect_message_callback(
          [this](const VICTRON_BLE_RECORD_SMART_BATTERY_PROTECT *val) {
            switch (this->type_) {
              case VICTRON_TEXT_SENSOR_TYPE::SMART_BATTERY_PROTECT_DEVICE_STATE:
                this->publish_state_(val->device_state);
                break;
              case VICTRON_TEXT_SENSOR_TYPE::SMART_BATTERY_PROTECT_ERROR_CODE:
                this->publish_state_(val->error_code);
                break;
              case VICTRON_TEXT_SENSOR_TYPE::SMART_BATTERY_PROTECT_ALARM_REASON:
                this->publish_state_(val->alarm_reason);
                break;
              case VICTRON_TEXT_SENSOR_TYPE::SMART_BATTERY_PROTECT_WARNING_REASON:
                this->publish_state_(val->warning_reason);
                break;
              case VICTRON_TEXT_SENSOR_TYPE::SMART_BATTERY_PROTECT_OFF_REASON:
                this->publish_state_(val->off_reason);
                break;
              default:
                break;
            }
          });
      break;
    case VICTRON_TEXT_SENSOR_TYPE::MULTI_RS_DEVICE_STATE:
    case VICTRON_TEXT_SENSOR_TYPE::MULTI_RS_CHARGER_ERROR:
    case VICTRON_TEXT_SENSOR_TYPE::MULTI_RS_ACTIVE_AC_IN:
      this->parent_->add_on_multi_rs_message_callback([this](const VICTRON_BLE_RECORD_MULTI_RS *val) {
        switch (this->type_) {
          case VICTRON_TEXT_SENSOR_TYPE::MULTI_RS_DEVICE_STATE:
            this->publish_state_(val->device_state);
            break;
          case VICTRON_TEXT_SENSOR_TYPE::MULTI_RS_CHARGER_ERROR:
            this->publish_state_(val->charger_error);
            break;
          case VICTRON_TEXT_SENSOR_TYPE::MULTI_RS_ACTIVE_AC_IN:
            this->publish_state_(val->active_ac_in);
            break;
          default:
            break;
        }
      });
      break;
    case VICTRON_TEXT_SENSOR_TYPE::VE_BUS_DEVICE_STATE:
    case VICTRON_TEXT_SENSOR_TYPE::VE_BUS_ACTIVE_AC_IN:
    case VICTRON_TEXT_SENSOR_TYPE::VE_BUS_ALARM:
      this->parent_->add_on_ve_bus_message_callback([this](const VICTRON_BLE_RECORD_VE_BUS *val) {
        switch (this->type_) {
          case VICTRON_TEXT_SENSOR_TYPE::VE_BUS_DEVICE_STATE:
            this->publish_state_(val->device_state);
            break;
          case VICTRON_TEXT_SENSOR_TYPE::VE_BUS_ACTIVE_AC_IN:
            this->publish_state_(val->active_ac_in);
            break;
          case VICTRON_TEXT_SENSOR_TYPE::VE_BUS_ALARM:
            this->publish_state_(val->alarm);
            break;
          default:
            break;
        }
      });
      break;
    case VICTRON_TEXT_SENSOR_TYPE::DC_ENERGY_METER_ALARM_REASON:
      this->parent_->add_on_dc_energy_meter_message_callback([this](const VICTRON_BLE_RECORD_DC_ENERGY_METER *val) {
        switch (this->type_) {
          case VICTRON_TEXT_SENSOR_TYPE::DC_ENERGY_METER_ALARM_REASON:
            this->publish_state_(val->alarm_reason);
            break;
          default:
            break;
        }
      });
      break;
    default:
      break;
  }
}

void VictronTextSensor::publish_state_(VE_REG_ALARM_REASON val) {
  if ((u_int16_t) val == (u_int16_t) VE_REG_ALARM_REASON::NO_ALARM) {
    this->publish_state("");
    return;
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_ALARM_REASON::LOW_VOLTAGE) != 0) {
    this->publish_state("Low Voltage");
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_ALARM_REASON::HIGH_VOLTAGE) != 0) {
    this->publish_state("High Voltage");
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_ALARM_REASON::LOW_SOC) != 0) {
    this->publish_state("Low SOC");
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_ALARM_REASON::LOW_STARTER_VOLTAGE) != 0) {
    this->publish_state("Low Starter Voltage");
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_ALARM_REASON::HIGH_STARTER_VOLTAGE) != 0) {
    this->publish_state("High Starter Voltage");
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_ALARM_REASON::LOW_TEMPERATURE) != 0) {
    this->publish_state("Low Temperature");
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_ALARM_REASON::HIGH_TEMPERATURE) != 0) {
    this->publish_state("High Temperature");
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_ALARM_REASON::MID_VOLTAGE) != 0) {
    this->publish_state("Mid Voltage");
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_ALARM_REASON::OVERLOAD) != 0) {
    this->publish_state("Overload");
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_ALARM_REASON::DC_RIPPLE) != 0) {
    this->publish_state("DC-ripple");
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_ALARM_REASON::LOW_V_AC_OUT) != 0) {
    this->publish_state("Low V AC out");
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_ALARM_REASON::HIGH_V_AC_OUT) != 0) {
    this->publish_state("High V AC out");
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_ALARM_REASON::SHORT_CIRCUIT) != 0) {
    this->publish_state("Short Circuit");
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_ALARM_REASON::BMS_LOCKOUT) != 0) {
    this->publish_state("BMS Lockout");
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_ALARM_REASON::UNKOWN_A) != 0) {
    this->publish_state("Unkown error (0x4000)");
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_ALARM_REASON::UNKOWN_B) != 0) {
    this->publish_state("Unkown error (0x8000)");
  }
}

void VictronTextSensor::publish_state_(VE_REG_DEVICE_STATE val) {
  switch (val) {
    case VE_REG_DEVICE_STATE::OFF:
      this->publish_state("Off");
      break;
    case VE_REG_DEVICE_STATE::LOW_POWER:
      this->publish_state("Low power");
      break;
    case VE_REG_DEVICE_STATE::FAULT:
      this->publish_state("Fault");
      break;
    case VE_REG_DEVICE_STATE::BULK:
      this->publish_state("Bulk");
      break;
    case VE_REG_DEVICE_STATE::ABSORPTION:
      this->publish_state("Absorption");
      break;
    case VE_REG_DEVICE_STATE::FLOAT:
      this->publish_state("Float");
      break;
    case VE_REG_DEVICE_STATE::STORAGE:
      this->publish_state("Storage");
      break;
    case VE_REG_DEVICE_STATE::EQUALIZE_MANUAL:
      this->publish_state("Equalize (manual)");
      break;
    case VE_REG_DEVICE_STATE::INVERTING:
      this->publish_state("Inverting");
      break;
    case VE_REG_DEVICE_STATE::POWER_SUPPLY:
      this->publish_state("Power supply");
      break;
    case VE_REG_DEVICE_STATE::STARTING_UP:
      this->publish_state("Starting-up");
      break;
    case VE_REG_DEVICE_STATE::REPEATED_ABSORPTION:
      this->publish_state("Repeated absorption");
      break;
    case VE_REG_DEVICE_STATE::AUTO_EQUALIZE:
      this->publish_state("Auto equalize / Recondition");
      break;
    case VE_REG_DEVICE_STATE::BATTERY_SAFE:
      this->publish_state("BatterySafe");
      break;
    case VE_REG_DEVICE_STATE::EXTERNAL_CONTROL:
      this->publish_state("External Control");
      break;
    default:
      ESP_LOGW(TAG, "[%s] Unkown device state (%u).", this->parent_->address_str().c_str(), (u_int8_t) val);
      this->publish_state(to_string((u_int8_t) val));
      break;
  }
}

void VictronTextSensor::publish_state_(VE_REG_CHR_ERROR_CODE val) {
  switch (val) {
    case VE_REG_CHR_ERROR_CODE::NO_ERROR:
      this->publish_state("");
      break;
    case VE_REG_CHR_ERROR_CODE::TEMPERATURE_BATTERY_HIGH:
      this->publish_state("Err 1 - Battery temperature too high");
      break;
    case VE_REG_CHR_ERROR_CODE::VOLTAGE_HIGH:
      this->publish_state("Err 2 - Battery voltage too high");
      break;
    case VE_REG_CHR_ERROR_CODE::REMOTE_TEMPERATURE_A:
      this->publish_state("Err 3 - Remote temperature sensor failure (auto-reset)");
      break;
    case VE_REG_CHR_ERROR_CODE::REMOTE_TEMPERATURE_B:
      this->publish_state("Err 4 - Remote temperature sensor failure (auto-reset)");
      break;
    case VE_REG_CHR_ERROR_CODE::REMOTE_TEMPERATURE_C:
      this->publish_state("Err 5 - Remote temperature sensor failure (not auto-reset)");
      break;
    case VE_REG_CHR_ERROR_CODE::REMOTE_BATTERY_A:
      this->publish_state("Err 6 - Remote battery voltage sense failure");
      break;
    case VE_REG_CHR_ERROR_CODE::REMOTE_BATTERY_B:
      this->publish_state("Err 7 - Remote battery voltage sense failure");
      break;
    case VE_REG_CHR_ERROR_CODE::REMOTE_BATTERY_C:
      this->publish_state("Err 8 - Remote battery voltage sense failure");
      break;
    case VE_REG_CHR_ERROR_CODE::HIGH_RIPPLE:
      this->publish_state("Err 14 - Battery temperature too low");
      break;
    case VE_REG_CHR_ERROR_CODE::TEMPERATURE_CHARGER:
      this->publish_state("Err 17 - Charger temperature too high");
      break;
    case VE_REG_CHR_ERROR_CODE::OVER_CURRENT:
      this->publish_state("Err 18 - Charger over current");
      break;
    case VE_REG_CHR_ERROR_CODE::BULK_TIME:
      this->publish_state("Err 20 - Bulk time limit exceeded");
      break;
    case VE_REG_CHR_ERROR_CODE::CURRENT_SENSOR:
      this->publish_state("Err 21 - Current sensor issue (sensor bias/sensor broken)");
      break;
    case VE_REG_CHR_ERROR_CODE::INTERNAL_TEMPERATURE_A:
      this->publish_state("Err 22 - Internal temperature sensor failure");
      break;
    case VE_REG_CHR_ERROR_CODE::INTERNAL_TEMPERATURE_B:
      this->publish_state("Err 23 - Internal temperature sensor failure");
      break;
    case VE_REG_CHR_ERROR_CODE::FAN:
      this->publish_state("Err 24 - Fan failure");
      break;
    case VE_REG_CHR_ERROR_CODE::OVERHEATED:
      this->publish_state("Err 26 - Terminals overheated");
      break;
    case VE_REG_CHR_ERROR_CODE::SHORT_CIRCUIT:
      this->publish_state("Err 27 - Charger short circuit");
      break;
    case VE_REG_CHR_ERROR_CODE::CONVERTER_ISSUE:
      this->publish_state("Err 28 - Power stage issue");
      break;
    case VE_REG_CHR_ERROR_CODE::OVER_CHARGE:
      this->publish_state("Err 29 - Over-Charge protection");
      break;
    case VE_REG_CHR_ERROR_CODE::INPUT_VOLTAGE:
      this->publish_state("Err 33 - PV over-voltage");
      break;
    case VE_REG_CHR_ERROR_CODE::INPUT_CURRENT:
      this->publish_state("Err 34 - PV over-current");
      break;
    case VE_REG_CHR_ERROR_CODE::INPUT_POWER:
      this->publish_state("PV over-power");
      break;
    case VE_REG_CHR_ERROR_CODE::INPUT_SHUTDOWN_VOLTAGE:
      this->publish_state("Err 38 - Input shutdown (due to excessive battery voltage)");
      break;
    case VE_REG_CHR_ERROR_CODE::INPUT_SHUTDOWN_CURRENT:
      this->publish_state("Err 39 - Input shutdown (due to current flow during off mode)");
      break;
    case VE_REG_CHR_ERROR_CODE::INPUT_SHUTDOWN_FAILURE:
      this->publish_state("Err 40 - PV Input failed to shutdown");
      break;
    case VE_REG_CHR_ERROR_CODE::INVERTER_SHUTDOWN_41:
      this->publish_state("Err 41 - Inverter shutdown (PV isolation)");
      break;
    case VE_REG_CHR_ERROR_CODE::INVERTER_SHUTDOWN_42:
      this->publish_state("Err 42 - Inverter shutdown (PV isolation)");
      break;
    case VE_REG_CHR_ERROR_CODE::INVERTER_SHUTDOWN_43:
      this->publish_state("Err 43 - Inverter shutdown (Ground Fault)");
      break;
    case VE_REG_CHR_ERROR_CODE::INVERTER_OVERLOAD:
      this->publish_state("Err 50 - Inverter overload");
      break;
    case VE_REG_CHR_ERROR_CODE::INVERTER_TEMPERATURE:
      this->publish_state("Err 51 - Inverter temperature too high");
      break;
    case VE_REG_CHR_ERROR_CODE::INVERTER_PEAK_CURRENT:
      this->publish_state("Err 52 - Inverter peak current");
      break;
    case VE_REG_CHR_ERROR_CODE::INVERTER_OUPUT_VOLTAGE_A:
      this->publish_state("Err 53 - Inverter output voltage");
      break;
    case VE_REG_CHR_ERROR_CODE::INVERTER_OUPUT_VOLTAGE_B:
      this->publish_state("Err 54 - Inverter output voltage");
      break;
    case VE_REG_CHR_ERROR_CODE::INVERTER_SELF_TEST_A:
      this->publish_state("Err 55 - Inverter self test failed");
      break;
    case VE_REG_CHR_ERROR_CODE::INVERTER_SELF_TEST_B:
      this->publish_state("Err 56 - Inverter self test failed");
      break;
    case VE_REG_CHR_ERROR_CODE::INVERTER_AC:
      this->publish_state("Err 57 - Inverter ac voltage on output");
      break;
    case VE_REG_CHR_ERROR_CODE::INVERTER_SELF_TEST_C:
      this->publish_state("Err 58 - Inverter self test failed");
      break;
    case VE_REG_CHR_ERROR_CODE::COMMUNICATION:
      this->publish_state("Information 65 - Communication warning");
      break;
    case VE_REG_CHR_ERROR_CODE::SYNCHRONISATION:
      this->publish_state("Information 66 - Incompatible device");
      break;
    case VE_REG_CHR_ERROR_CODE::BMS:
      this->publish_state("Err 67 - BMS Connection lost");
      break;
    case VE_REG_CHR_ERROR_CODE::NETWORK_A:
      this->publish_state("Err 68 - Network misconfigured");
      break;
    case VE_REG_CHR_ERROR_CODE::NETWORK_B:
      this->publish_state("Err 69 - Network misconfigured");
      break;
    case VE_REG_CHR_ERROR_CODE::NETWORK_C:
      this->publish_state("Err 70 - Network misconfigured");
      break;
    case VE_REG_CHR_ERROR_CODE::NETWORK_D:
      this->publish_state("Err 71 - Network misconfigured");
      break;
    case VE_REG_CHR_ERROR_CODE::PV_INPUT_SHUTDOWN_80:
      this->publish_state("Err 80 - PV Input shutdown");
      break;
    case VE_REG_CHR_ERROR_CODE::PV_INPUT_SHUTDOWN_81:
      this->publish_state("Err 81 - PV Input shutdown");
      break;
    case VE_REG_CHR_ERROR_CODE::PV_INPUT_SHUTDOWN_82:
      this->publish_state("Err 82 - PV Input shutdown");
      break;
    case VE_REG_CHR_ERROR_CODE::PV_INPUT_SHUTDOWN_83:
      this->publish_state("Err 83 - PV Input shutdown");
      break;
    case VE_REG_CHR_ERROR_CODE::PV_INPUT_SHUTDOWN_84:
      this->publish_state("Err 84 - PV Input shutdown");
      break;
    case VE_REG_CHR_ERROR_CODE::PV_INPUT_SHUTDOWN_85:
      this->publish_state("Err 85 - PV Input shutdown");
      break;
    case VE_REG_CHR_ERROR_CODE::PV_INPUT_SHUTDOWN_86:
      this->publish_state("Err 86 - PV Input shutdown");
      break;
    case VE_REG_CHR_ERROR_CODE::PV_INPUT_SHUTDOWN_87:
      this->publish_state("Err 87 - PV Input shutdown");
      break;
    case VE_REG_CHR_ERROR_CODE::CPU_TEMPERATURE:
      this->publish_state("Err 114 - CPU temperature too high");
      break;
    case VE_REG_CHR_ERROR_CODE::CALIBRATION_LOST:
      this->publish_state("Err 116 - Factory calibration data lost");
      break;
    case VE_REG_CHR_ERROR_CODE::FIRMWARE:
      this->publish_state("Err 117 - Invalid/incompatible firmware");
      break;
    case VE_REG_CHR_ERROR_CODE::SETTINGS:
      this->publish_state("Err 119 - Settings data lost");
      break;
    case VE_REG_CHR_ERROR_CODE::TESTER_FAIL:
      this->publish_state("Err 121 - Tester fail");
      break;
    case VE_REG_CHR_ERROR_CODE::INTERNAL_DC_VOLTAGE_A:
      this->publish_state("Err 200 - Internal DC voltage error");
      break;
    case VE_REG_CHR_ERROR_CODE::INTERNAL_DC_VOLTAGE_B:
      this->publish_state("Err 201 - Internal DC voltage error");
      break;
    case VE_REG_CHR_ERROR_CODE::SELF_TEST:
      this->publish_state("Err 202 - PV residual current sensor self-test failure");
      break;
    case VE_REG_CHR_ERROR_CODE::INTERNAL_SUPPLY_A:
      this->publish_state("Err 203 - Internal supply voltage error");
      break;
    case VE_REG_CHR_ERROR_CODE::INTERNAL_SUPPLY_B:
      this->publish_state("Err 205 - Internal supply voltage error");
      break;
    case VE_REG_CHR_ERROR_CODE::INTERNAL_SUPPLY_C:
      this->publish_state("Err 212 - Internal supply voltage error");
      break;
    case VE_REG_CHR_ERROR_CODE::INTERNAL_SUPPLY_D:
      this->publish_state("Err 215 - Internal supply voltage error");
      break;
    default:
      ESP_LOGW(TAG, "[%s] Unkown device error (%u).", this->parent_->address_str().c_str(), (u_int8_t) val);
      this->publish_state(to_string((u_int8_t) val));
      break;
  }
}

void VictronTextSensor::publish_state_(VE_REG_DEVICE_OFF_REASON_2 val) {
  if ((u_int16_t) val == (u_int16_t) VE_REG_DEVICE_OFF_REASON_2::NOTHING) {
    this->publish_state("");
    return;
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_DEVICE_OFF_REASON_2::NO_INPUT_POWER) != 0) {
    this->publish_state("No input power");
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_DEVICE_OFF_REASON_2::SWITCHED_OFF_SWITCH) != 0) {
    this->publish_state("Switched off (power switch)");
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_DEVICE_OFF_REASON_2::SWITCHED_OFF_REGISTER) != 0) {
    this->publish_state("Switched off (device mode register)");
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_DEVICE_OFF_REASON_2::REMOTE_INPUT) != 0) {
    this->publish_state("Remote input");
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_DEVICE_OFF_REASON_2::PROTECTION) != 0) {
    this->publish_state("Protection active");
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_DEVICE_OFF_REASON_2::PAYGO) != 0) {
    this->publish_state("Paygo");
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_DEVICE_OFF_REASON_2::BMS) != 0) {
    this->publish_state("BMS");
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_DEVICE_OFF_REASON_2::ENGINE) != 0) {
    this->publish_state("Engine shutdown detection");
  }
  if (((u_int16_t) val & (u_int16_t) VE_REG_DEVICE_OFF_REASON_2::INPUT_VOLTATE) != 0) {
    this->publish_state("Analysing input voltage");
  }
}

void VictronTextSensor::publish_state_(VE_REG_AC_IN_ACTIVE val) {
  switch (val) {
    case VE_REG_AC_IN_ACTIVE::AC_IN_1:
      this->publish_state("AC in 1");
      break;
    case VE_REG_AC_IN_ACTIVE::AC_IN_2:
      this->publish_state("AC in 2");
      break;
    case VE_REG_AC_IN_ACTIVE::NOT_CONNECTED:
      this->publish_state("Not connected");
      break;
    case VE_REG_AC_IN_ACTIVE::UNKOWN:
      this->publish_state("Unkown");
      break;
    default:
      break;
  }
}

void VictronTextSensor::publish_state_(VE_REG_ALARM_NOTIFICATION val) {
  switch (val) {
    case VE_REG_ALARM_NOTIFICATION::NO_ALARM:
      this->publish_state("");
      break;
    case VE_REG_ALARM_NOTIFICATION::WARNING:
      this->publish_state("Warning");
      break;
    case VE_REG_ALARM_NOTIFICATION::ALARM:
      this->publish_state("Alarm");
      break;
    default:
      break;
  }
}

}  // namespace victron_ble
}  // namespace esphome