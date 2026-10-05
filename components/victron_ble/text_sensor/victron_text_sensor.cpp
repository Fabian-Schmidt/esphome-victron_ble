#include "victron_text_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace victron_ble {

static const char *const TAG = "victron_ble.text_sensor";

// void VictronTextSensor::dump_config() {
//   LOG_TEXT_SENSOR("", "Victron Text Sensor", this);
//   ESP_LOGCONFIG(TAG, "  Type '%s'", enum_to_c_str(this->type_));
// }

void VictronTextSensor::publish_if_changed_(const std::string &state) {
  if (this->has_state() && this->get_state() == state) {
    return;
  }
  this->publish_state(state);
}

void VictronTextSensor::append_flag_(std::string &out, bool active, const char *text) {
  if (!active) {
    return;
  }
  if (!out.empty()) {
    out += ", ";
  }
  out += text;
}

void VictronTextSensor::register_callback() {
  this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
    switch (this->type_) {
      case VICTRON_TEXT_SENSOR_TYPE::ALARM:
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::VE_BUS:
            this->publish_state_(msg->data.ve_bus.alarm);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no `alarm` field.", this->parent_->address_str());
            this->publish_if_changed_("");
            break;
        }
        break;

      case VICTRON_TEXT_SENSOR_TYPE::ACTIVE_AC_IN:
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::MULTI_RS:
            this->publish_state_(msg->data.multi_rs.active_ac_in);
            break;
          case VICTRON_BLE_RECORD_TYPE::VE_BUS:
            this->publish_state_(msg->data.ve_bus.active_ac_in);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no `active ac in` field.", this->parent_->address_str());
            this->publish_if_changed_("");
            break;
        }
        break;

      case VICTRON_TEXT_SENSOR_TYPE::ALARM_REASON:
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::BATTERY_MONITOR:
            this->publish_state_(msg->data.battery_monitor.alarm_reason);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER:
            this->publish_state_(msg->data.inverter.alarm_reason);
            break;
          case VICTRON_BLE_RECORD_TYPE::SMART_BATTERY_PROTECT:
            this->publish_state_(msg->data.smart_battery_protect.alarm_reason);
            break;
          case VICTRON_BLE_RECORD_TYPE::DC_ENERGY_METER:
            this->publish_state_(msg->data.dc_energy_meter.alarm_reason);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no `alarm reason` field.", this->parent_->address_str());
            this->publish_if_changed_("");
            break;
        }
        break;

      case VICTRON_TEXT_SENSOR_TYPE::CHARGER_ERROR:
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SOLAR_CHARGER:
            this->publish_state_(msg->data.solar_charger.charger_error);
            break;
          case VICTRON_BLE_RECORD_TYPE::DCDC_CONVERTER:
            this->publish_state_(msg->data.dcdc_converter.charger_error);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER_RS:
            this->publish_state_(msg->data.inverter_rs.charger_error);
            break;
          case VICTRON_BLE_RECORD_TYPE::AC_CHARGER:
            this->publish_state_(msg->data.ac_charger.charger_error);
            break;
          case VICTRON_BLE_RECORD_TYPE::MULTI_RS:
            this->publish_state_(msg->data.multi_rs.charger_error);
            break;
          case VICTRON_BLE_RECORD_TYPE::ORION_XS:
            this->publish_state_(msg->data.orion_xs.charger_error);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no `charger error` field.", this->parent_->address_str());
            this->publish_if_changed_("");
            break;
        }
        break;

      case VICTRON_TEXT_SENSOR_TYPE::DEVICE_STATE:
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SOLAR_CHARGER:
            this->publish_state_(msg->data.solar_charger.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER:
            this->publish_state_(msg->data.inverter.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::DCDC_CONVERTER:
            this->publish_state_(msg->data.dcdc_converter.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER_RS:
            this->publish_state_(msg->data.inverter_rs.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::AC_CHARGER:
            this->publish_state_(msg->data.ac_charger.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::SMART_BATTERY_PROTECT:
            this->publish_state_(msg->data.smart_battery_protect.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::MULTI_RS:
            this->publish_state_(msg->data.multi_rs.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::VE_BUS:
            this->publish_state_(msg->data.ve_bus.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::ORION_XS:
            this->publish_state_(msg->data.orion_xs.device_state);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no `device state` field.", this->parent_->address_str());
            this->publish_if_changed_("");
            break;
        }
        break;

      case VICTRON_TEXT_SENSOR_TYPE::ERROR_CODE:
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SMART_BATTERY_PROTECT:
            this->publish_state_(msg->data.smart_battery_protect.error_code);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no `error code` field.", this->parent_->address_str());
            this->publish_if_changed_("");
            break;
        }
        break;

      case VICTRON_TEXT_SENSOR_TYPE::OFF_REASON:
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::DCDC_CONVERTER:
            this->publish_state_(msg->data.dcdc_converter.off_reason);
            break;
          case VICTRON_BLE_RECORD_TYPE::SMART_BATTERY_PROTECT:
            this->publish_state_(msg->data.smart_battery_protect.off_reason);
            break;
          case VICTRON_BLE_RECORD_TYPE::ORION_XS:
            this->publish_state_(msg->data.orion_xs.off_reason);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no `off reason` field.", this->parent_->address_str());
            this->publish_if_changed_("");
            break;
        }
        break;

      case VICTRON_TEXT_SENSOR_TYPE::WARNING_REASON:
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SMART_BATTERY_PROTECT:
            this->publish_state_(msg->data.smart_battery_protect.warning_reason);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no `warning reason` field.", this->parent_->address_str());
            this->publish_if_changed_("");
            break;
        }
        break;

      case VICTRON_TEXT_SENSOR_TYPE::OUTPUT_STATE:
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SMART_BATTERY_PROTECT:
            this->publish_state_(msg->data.smart_battery_protect.output_state);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no `output state` field.", this->parent_->address_str());
            this->publish_if_changed_("");
            break;
        }
        break;

      case VICTRON_TEXT_SENSOR_TYPE::BALANCER_STATUS:
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SMART_LITHIUM:
            this->publish_state_(msg->data.smart_lithium.balancer_status);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no `balancer status` field.", this->parent_->address_str());
            this->publish_if_changed_("");
            break;
        }
        break;

      default:
        break;
    }
  });
}

void VictronTextSensor::publish_state_(VE_REG_ALARM_REASON val) {
  std::string text;
  auto has = [val](VE_REG_ALARM_REASON flag) { return (val & flag) != VE_REG_ALARM_REASON::NO_ALARM; };
  append_flag_(text, has(VE_REG_ALARM_REASON::LOW_VOLTAGE), "Low Voltage");
  append_flag_(text, has(VE_REG_ALARM_REASON::HIGH_VOLTAGE), "High Voltage");
  append_flag_(text, has(VE_REG_ALARM_REASON::LOW_SOC), "Low SOC");
  append_flag_(text, has(VE_REG_ALARM_REASON::LOW_STARTER_VOLTAGE), "Low Starter Voltage");
  append_flag_(text, has(VE_REG_ALARM_REASON::HIGH_STARTER_VOLTAGE), "High Starter Voltage");
  append_flag_(text, has(VE_REG_ALARM_REASON::LOW_TEMPERATURE), "Low Temperature");
  append_flag_(text, has(VE_REG_ALARM_REASON::HIGH_TEMPERATURE), "High Temperature");
  append_flag_(text, has(VE_REG_ALARM_REASON::MID_VOLTAGE), "Mid Voltage");
  append_flag_(text, has(VE_REG_ALARM_REASON::OVERLOAD), "Overload");
  append_flag_(text, has(VE_REG_ALARM_REASON::DC_RIPPLE), "DC-ripple");
  append_flag_(text, has(VE_REG_ALARM_REASON::LOW_V_AC_OUT), "Low V AC out");
  append_flag_(text, has(VE_REG_ALARM_REASON::HIGH_V_AC_OUT), "High V AC out");
  append_flag_(text, has(VE_REG_ALARM_REASON::SHORT_CIRCUIT), "Short Circuit");
  append_flag_(text, has(VE_REG_ALARM_REASON::BMS_LOCKOUT), "BMS Lockout");
  append_flag_(text, has(VE_REG_ALARM_REASON::UNKNOWN_A), "Unknown error (0x4000)");
  append_flag_(text, has(VE_REG_ALARM_REASON::UNKNOWN_B), "Unknown error (0x8000)");
  this->publish_if_changed_(text);
}

void VictronTextSensor::publish_state_(VE_REG_DEVICE_STATE val) {
  switch (val) {
    case VE_REG_DEVICE_STATE::OFF:
      this->publish_if_changed_("Off");
      break;
    case VE_REG_DEVICE_STATE::LOW_POWER:
      this->publish_if_changed_("Low power");
      break;
    case VE_REG_DEVICE_STATE::FAULT:
      this->publish_if_changed_("Fault");
      break;
    case VE_REG_DEVICE_STATE::BULK:
      this->publish_if_changed_("Bulk");
      break;
    case VE_REG_DEVICE_STATE::ABSORPTION:
      this->publish_if_changed_("Absorption");
      break;
    case VE_REG_DEVICE_STATE::FLOAT:
      this->publish_if_changed_("Float");
      break;
    case VE_REG_DEVICE_STATE::STORAGE:
      this->publish_if_changed_("Storage");
      break;
    case VE_REG_DEVICE_STATE::EQUALIZE_MANUAL:
      this->publish_if_changed_("Equalize (manual)");
      break;
    case VE_REG_DEVICE_STATE::PASSTHRU:
      this->publish_if_changed_("Pass Thru");
      break;
    case VE_REG_DEVICE_STATE::INVERTING:
      this->publish_if_changed_("Inverting");
      break;
    case VE_REG_DEVICE_STATE::ASSISTING:
      this->publish_if_changed_("Assisting");
      break;
    case VE_REG_DEVICE_STATE::POWER_SUPPLY:
      this->publish_if_changed_("Power supply");
      break;
    case VE_REG_DEVICE_STATE::SUSTAIN:
      this->publish_if_changed_("Sustain");
      break;
    case VE_REG_DEVICE_STATE::STARTING_UP:
      this->publish_if_changed_("Starting-up");
      break;
    case VE_REG_DEVICE_STATE::REPEATED_ABSORPTION:
      this->publish_if_changed_("Repeated absorption");
      break;
    case VE_REG_DEVICE_STATE::AUTO_EQUALIZE:
      this->publish_if_changed_("Auto equalize / Recondition");
      break;
    case VE_REG_DEVICE_STATE::BATTERY_SAFE:
      this->publish_if_changed_("BatterySafe");
      break;
    case VE_REG_DEVICE_STATE::LOAD_DETECT:
      this->publish_if_changed_("Load detect");
      break;
    case VE_REG_DEVICE_STATE::BLOCKED:
      this->publish_if_changed_("Blocked");
      break;
    case VE_REG_DEVICE_STATE::TEST:
      this->publish_if_changed_("Test");
      break;
    case VE_REG_DEVICE_STATE::EXTERNAL_CONTROL:
      this->publish_if_changed_("External Control");
      break;
    case VE_REG_DEVICE_STATE::NOT_AVAILABLE:
      this->publish_if_changed_("Not available");
      break;
    default:
      ESP_LOGW(TAG, "[%s] Unknown device state (%u).", this->parent_->address_str(), (uint8_t) val);
      this->publish_if_changed_(to_string((uint8_t) val));
      break;
  }
}

void VictronTextSensor::publish_state_(VE_REG_CHR_ERROR_CODE val) {
  switch (val) {
    case VE_REG_CHR_ERROR_CODE::NO_ERROR:
      this->publish_if_changed_("");
      break;
    case VE_REG_CHR_ERROR_CODE::TEMPERATURE_BATTERY_HIGH:
      this->publish_if_changed_("Err 1 - Battery temperature too high");
      break;
    case VE_REG_CHR_ERROR_CODE::VOLTAGE_HIGH:
      this->publish_if_changed_("Err 2 - Battery voltage too high");
      break;
    case VE_REG_CHR_ERROR_CODE::REMOTE_TEMPERATURE_A:
      this->publish_if_changed_("Err 3 - Remote temperature sensor failure (auto-reset)");
      break;
    case VE_REG_CHR_ERROR_CODE::REMOTE_TEMPERATURE_B:
      this->publish_if_changed_("Err 4 - Remote temperature sensor failure (auto-reset)");
      break;
    case VE_REG_CHR_ERROR_CODE::REMOTE_TEMPERATURE_C:
      this->publish_if_changed_("Err 5 - Remote temperature sensor failure (not auto-reset)");
      break;
    case VE_REG_CHR_ERROR_CODE::REMOTE_BATTERY_A:
      this->publish_if_changed_("Err 6 - Remote battery voltage sense failure");
      break;
    case VE_REG_CHR_ERROR_CODE::REMOTE_BATTERY_B:
      this->publish_if_changed_("Err 7 - Remote battery voltage sense failure");
      break;
    case VE_REG_CHR_ERROR_CODE::REMOTE_BATTERY_C:
      this->publish_if_changed_("Err 8 - Remote battery voltage sense failure");
      break;
    case VE_REG_CHR_ERROR_CODE::HIGH_RIPPLE:
      this->publish_if_changed_("Err 14 - Battery temperature too low");
      break;
    case VE_REG_CHR_ERROR_CODE::TEMPERATURE_CHARGER:
      this->publish_if_changed_("Err 17 - Charger temperature too high");
      break;
    case VE_REG_CHR_ERROR_CODE::OVER_CURRENT:
      this->publish_if_changed_("Err 18 - Charger over current");
      break;
    case VE_REG_CHR_ERROR_CODE::POLARITY:
      this->publish_if_changed_("Err 19 - Charger current polarity reversed");
      break;
    case VE_REG_CHR_ERROR_CODE::BULK_TIME:
      this->publish_if_changed_("Err 20 - Bulk time limit exceeded");
      break;
    case VE_REG_CHR_ERROR_CODE::CURRENT_SENSOR:
      this->publish_if_changed_("Err 21 - Current sensor issue (sensor bias/sensor broken)");
      break;
    case VE_REG_CHR_ERROR_CODE::INTERNAL_TEMPERATURE_A:
      this->publish_if_changed_("Err 22 - Internal temperature sensor failure");
      break;
    case VE_REG_CHR_ERROR_CODE::INTERNAL_TEMPERATURE_B:
      this->publish_if_changed_("Err 23 - Internal temperature sensor failure");
      break;
    case VE_REG_CHR_ERROR_CODE::FAN:
      this->publish_if_changed_("Err 24 - Fan failure");
      break;
    case VE_REG_CHR_ERROR_CODE::OVERHEATED:
      this->publish_if_changed_("Err 26 - Terminals overheated");
      break;
    case VE_REG_CHR_ERROR_CODE::SHORT_CIRCUIT:
      this->publish_if_changed_("Err 27 - Charger short circuit");
      break;
    case VE_REG_CHR_ERROR_CODE::CONVERTER_ISSUE:
      this->publish_if_changed_("Err 28 - Power stage issue");
      break;
    case VE_REG_CHR_ERROR_CODE::OVER_CHARGE:
      this->publish_if_changed_("Err 29 - Over-Charge protection");
      break;
    case VE_REG_CHR_ERROR_CODE::INPUT_VOLTAGE:
      this->publish_if_changed_("Err 33 - PV over-voltage");
      break;
    case VE_REG_CHR_ERROR_CODE::INPUT_CURRENT:
      this->publish_if_changed_("Err 34 - PV over-current");
      break;
    case VE_REG_CHR_ERROR_CODE::INPUT_POWER:
      this->publish_if_changed_("PV over-power");
      break;
    case VE_REG_CHR_ERROR_CODE::INPUT_SHUTDOWN_VOLTAGE:
      this->publish_if_changed_("Err 38 - Input shutdown (due to excessive battery voltage)");
      break;
    case VE_REG_CHR_ERROR_CODE::INPUT_SHUTDOWN_CURRENT:
      this->publish_if_changed_("Err 39 - Input shutdown (due to current flow during off mode)");
      break;
    case VE_REG_CHR_ERROR_CODE::INPUT_SHUTDOWN_FAILURE:
      this->publish_if_changed_("Err 40 - PV Input failed to shutdown");
      break;
    case VE_REG_CHR_ERROR_CODE::INVERTER_SHUTDOWN_41:
      this->publish_if_changed_("Err 41 - Inverter shutdown (PV isolation)");
      break;
    case VE_REG_CHR_ERROR_CODE::INVERTER_SHUTDOWN_42:
      this->publish_if_changed_("Err 42 - Inverter shutdown (PV isolation)");
      break;
    case VE_REG_CHR_ERROR_CODE::INVERTER_SHUTDOWN_43:
      this->publish_if_changed_("Err 43 - Inverter shutdown (Ground Fault)");
      break;
    case VE_REG_CHR_ERROR_CODE::INVERTER_OVERLOAD:
      this->publish_if_changed_("Err 50 - Inverter overload");
      break;
    case VE_REG_CHR_ERROR_CODE::INVERTER_TEMPERATURE:
      this->publish_if_changed_("Err 51 - Inverter temperature too high");
      break;
    case VE_REG_CHR_ERROR_CODE::INVERTER_PEAK_CURRENT:
      this->publish_if_changed_("Err 52 - Inverter peak current");
      break;
    case VE_REG_CHR_ERROR_CODE::INVERTER_OUPUT_VOLTAGE_A:
      this->publish_if_changed_("Err 53 - Inverter output voltage");
      break;
    case VE_REG_CHR_ERROR_CODE::INVERTER_OUPUT_VOLTAGE_B:
      this->publish_if_changed_("Err 54 - Inverter output voltage");
      break;
    case VE_REG_CHR_ERROR_CODE::INVERTER_SELF_TEST_A:
      this->publish_if_changed_("Err 55 - Inverter self test failed");
      break;
    case VE_REG_CHR_ERROR_CODE::INVERTER_SELF_TEST_B:
      this->publish_if_changed_("Err 56 - Inverter self test failed");
      break;
    case VE_REG_CHR_ERROR_CODE::INVERTER_AC:
      this->publish_if_changed_("Err 57 - Inverter ac voltage on output");
      break;
    case VE_REG_CHR_ERROR_CODE::INVERTER_SELF_TEST_C:
      this->publish_if_changed_("Err 58 - Inverter self test failed");
      break;
    case VE_REG_CHR_ERROR_CODE::COMMUNICATION:
      this->publish_if_changed_("Information 65 - Communication warning");
      break;
    case VE_REG_CHR_ERROR_CODE::SYNCHRONISATION:
      this->publish_if_changed_("Information 66 - Incompatible device");
      break;
    case VE_REG_CHR_ERROR_CODE::BMS:
      this->publish_if_changed_("Err 67 - BMS Connection lost");
      break;
    case VE_REG_CHR_ERROR_CODE::NETWORK_A:
      this->publish_if_changed_("Err 68 - Network misconfigured");
      break;
    case VE_REG_CHR_ERROR_CODE::NETWORK_B:
      this->publish_if_changed_("Err 69 - Network misconfigured");
      break;
    case VE_REG_CHR_ERROR_CODE::NETWORK_C:
      this->publish_if_changed_("Err 70 - Network misconfigured");
      break;
    case VE_REG_CHR_ERROR_CODE::NETWORK_D:
      this->publish_if_changed_("Err 71 - Network misconfigured");
      break;
    case VE_REG_CHR_ERROR_CODE::PV_INPUT_SHUTDOWN_80:
      this->publish_if_changed_("Err 80 - PV Input shutdown");
      break;
    case VE_REG_CHR_ERROR_CODE::PV_INPUT_SHUTDOWN_81:
      this->publish_if_changed_("Err 81 - PV Input shutdown");
      break;
    case VE_REG_CHR_ERROR_CODE::PV_INPUT_SHUTDOWN_82:
      this->publish_if_changed_("Err 82 - PV Input shutdown");
      break;
    case VE_REG_CHR_ERROR_CODE::PV_INPUT_SHUTDOWN_83:
      this->publish_if_changed_("Err 83 - PV Input shutdown");
      break;
    case VE_REG_CHR_ERROR_CODE::PV_INPUT_SHUTDOWN_84:
      this->publish_if_changed_("Err 84 - PV Input shutdown");
      break;
    case VE_REG_CHR_ERROR_CODE::PV_INPUT_SHUTDOWN_85:
      this->publish_if_changed_("Err 85 - PV Input shutdown");
      break;
    case VE_REG_CHR_ERROR_CODE::PV_INPUT_SHUTDOWN_86:
      this->publish_if_changed_("Err 86 - PV Input shutdown");
      break;
    case VE_REG_CHR_ERROR_CODE::PV_INPUT_SHUTDOWN_87:
      this->publish_if_changed_("Err 87 - PV Input shutdown");
      break;
    case VE_REG_CHR_ERROR_CODE::CPU_TEMPERATURE:
      this->publish_if_changed_("Err 114 - CPU temperature too high");
      break;
    case VE_REG_CHR_ERROR_CODE::CALIBRATION_LOST:
      this->publish_if_changed_("Err 116 - Factory calibration data lost");
      break;
    case VE_REG_CHR_ERROR_CODE::FIRMWARE:
      this->publish_if_changed_("Err 117 - Invalid/incompatible firmware");
      break;
    case VE_REG_CHR_ERROR_CODE::SETTINGS:
      this->publish_if_changed_("Err 119 - Settings data lost");
      break;
    case VE_REG_CHR_ERROR_CODE::TESTER_FAIL:
      this->publish_if_changed_("Err 121 - Tester fail");
      break;
    case VE_REG_CHR_ERROR_CODE::INTERNAL_DC_VOLTAGE_A:
      this->publish_if_changed_("Err 200 - Internal DC voltage error");
      break;
    case VE_REG_CHR_ERROR_CODE::INTERNAL_DC_VOLTAGE_B:
      this->publish_if_changed_("Err 201 - Internal DC voltage error");
      break;
    case VE_REG_CHR_ERROR_CODE::SELF_TEST:
      this->publish_if_changed_("Err 202 - PV residual current sensor self-test failure");
      break;
    case VE_REG_CHR_ERROR_CODE::INTERNAL_SUPPLY_A:
      this->publish_if_changed_("Err 203 - Internal supply voltage error");
      break;
    case VE_REG_CHR_ERROR_CODE::INTERNAL_SUPPLY_B:
      this->publish_if_changed_("Err 205 - Internal supply voltage error");
      break;
    case VE_REG_CHR_ERROR_CODE::INTERNAL_SUPPLY_C:
      this->publish_if_changed_("Err 212 - Internal supply voltage error");
      break;
    case VE_REG_CHR_ERROR_CODE::INTERNAL_SUPPLY_D:
      this->publish_if_changed_("Err 215 - Internal supply voltage error");
      break;
    case VE_REG_CHR_ERROR_CODE::NOT_AVAILABLE:
      this->publish_if_changed_("Not available");
      break;
    default:
      ESP_LOGW(TAG, "[%s] Unknown device error (%u).", this->parent_->address_str(), (uint8_t) val);
      this->publish_if_changed_(to_string((uint8_t) val));
      break;
  }
}

void VictronTextSensor::publish_state_(VE_REG_DEVICE_OFF_REASON_2 val) {
  std::string text;
  auto has = [val](VE_REG_DEVICE_OFF_REASON_2 flag) {
    return (val & flag) != VE_REG_DEVICE_OFF_REASON_2::NOTHING;
  };
  append_flag_(text, has(VE_REG_DEVICE_OFF_REASON_2::NO_INPUT_POWER), "No input power");
  append_flag_(text, has(VE_REG_DEVICE_OFF_REASON_2::SWITCHED_OFF_SWITCH), "Switched off (power switch)");
  append_flag_(text, has(VE_REG_DEVICE_OFF_REASON_2::SWITCHED_OFF_REGISTER), "Switched off (device mode register)");
  append_flag_(text, has(VE_REG_DEVICE_OFF_REASON_2::REMOTE_INPUT), "Remote input");
  append_flag_(text, has(VE_REG_DEVICE_OFF_REASON_2::PROTECTION), "Protection active");
  append_flag_(text, has(VE_REG_DEVICE_OFF_REASON_2::PAYGO), "Paygo");
  append_flag_(text, has(VE_REG_DEVICE_OFF_REASON_2::BMS), "BMS");
  append_flag_(text, has(VE_REG_DEVICE_OFF_REASON_2::ENGINE), "Engine shutdown detection");
  append_flag_(text, has(VE_REG_DEVICE_OFF_REASON_2::INPUT_VOLTATE), "Analysing input voltage");
  append_flag_(text, has(VE_REG_DEVICE_OFF_REASON_2::TEMPERATURE), "Battery temperature too low");
  this->publish_if_changed_(text);
}

void VictronTextSensor::publish_state_(VE_REG_AC_IN_ACTIVE val) {
  switch (val) {
    case VE_REG_AC_IN_ACTIVE::AC_IN_1:
      this->publish_if_changed_("AC in 1");
      break;
    case VE_REG_AC_IN_ACTIVE::AC_IN_2:
      this->publish_if_changed_("AC in 2");
      break;
    case VE_REG_AC_IN_ACTIVE::NOT_CONNECTED:
      this->publish_if_changed_("Not connected");
      break;
    case VE_REG_AC_IN_ACTIVE::UNKNOWN:
      this->publish_if_changed_("Unknown");
      break;
    default:
      break;
  }
}

void VictronTextSensor::publish_state_(VE_REG_ALARM_NOTIFICATION val) {
  switch (val) {
    case VE_REG_ALARM_NOTIFICATION::NO_ALARM:
      this->publish_if_changed_("");
      break;
    case VE_REG_ALARM_NOTIFICATION::WARNING:
      this->publish_if_changed_("Warning");
      break;
    case VE_REG_ALARM_NOTIFICATION::ALARM:
      this->publish_if_changed_("Alarm");
      break;
    default:
      break;
  }
}

void VictronTextSensor::publish_state_(VE_REG_BALANCER_STATUS val) {
  switch (val) {
    case VE_REG_BALANCER_STATUS::UNKNOWN:
      this->publish_if_changed_("Unknown");
      break;
    case VE_REG_BALANCER_STATUS::BALANCED:
      this->publish_if_changed_("Balanced");
      break;
    case VE_REG_BALANCER_STATUS::BALANCING:
      this->publish_if_changed_("Balancing");
      break;
    case VE_REG_BALANCER_STATUS::IMBALANCE:
      this->publish_if_changed_("Imbalance");
      break;
    default:
      break;
  }
}

void VictronTextSensor::publish_state_(VE_REG_DC_OUTPUT_STATUS val) {
  switch (val) {
    case VE_REG_DC_OUTPUT_STATUS::OFF:
      this->publish_if_changed_("Off");
      break;
    case VE_REG_DC_OUTPUT_STATUS::AUTO:
      this->publish_if_changed_("Auto");
      break;
    case VE_REG_DC_OUTPUT_STATUS::ALT1:
      this->publish_if_changed_("Alternative control 1");
      break;
    case VE_REG_DC_OUTPUT_STATUS::ALT2:
      this->publish_if_changed_("Alternative control 2");
      break;
    case VE_REG_DC_OUTPUT_STATUS::ON:
      this->publish_if_changed_("On");
      break;
    case VE_REG_DC_OUTPUT_STATUS::USER1:
      this->publish_if_changed_("User defined settings 1");
      break;
    case VE_REG_DC_OUTPUT_STATUS::USER2:
      this->publish_if_changed_("User defined settings 2");
      break;
    case VE_REG_DC_OUTPUT_STATUS::AES:
      this->publish_if_changed_("Automatic Energy Selector");
      break;
    default:
      break;
  }
}

}  // namespace victron_ble
}  // namespace esphome
