#include "victron_sensor.h"
#include "esphome/core/log.h"
#include <cmath>

namespace esphome {
namespace victron_ble {

static const char *const TAG = "victron_ble.sensor";

// void VictronSensor::dump_config() {
//   LOG_SENSOR("", "Victron Sensor", this);
//   ESP_LOGCONFIG(TAG, "  Type '%s'", enum_to_c_str(this->type_));
// }

// Victron devices advertise about once a second. When a sensor has no value to report, only log and publish NaN
// when it goes from valid (or no value yet) to unavailable; a later valid value re-arms this.
// Uses the raw (pre-filter) state, as has_state() is not set while filters such as throttle_average hold values back.
bool VictronSensor::should_report_unavailable_() {
  if (this->invalid_reported_ && std::isnan(this->get_raw_state())) {
    return false;
  }
  this->invalid_reported_ = true;
  return true;
}

// The configured type does not exist on this device (e.g. a SmartShunt field on a solar charger).
void VictronSensor::publish_invalid_(const char *message) {
  if (!this->should_report_unavailable_()) {
    return;
  }
  ESP_LOGW(TAG, "[%s] %s: %s Publishing NaN until this changes.", this->parent_->address_str(),
           this->get_name().c_str(), message);
  this->publish_state(NAN);
}

static const char *aux_input_to_str(VE_REG_BMV_AUX_INPUT aux_input) {
  switch (aux_input) {
    case VE_REG_BMV_AUX_INPUT::VE_REG_DC_CHANNEL2_VOLTAGE:
      return "starter battery voltage";
    case VE_REG_BMV_AUX_INPUT::VE_REG_BATTERY_MID_POINT_VOLTAGE:
      return "midpoint voltage";
    case VE_REG_BMV_AUX_INPUT::VE_REG_BAT_TEMPERATURE:
      return "temperature";
    default:
      return "none";
  }
}

// AUX_VOLTAGE / MID_VOLTAGE / TEMPERATURE share the device's single aux input. Whichever one the device is set to
// reports values; the others are simply not in use, which is a normal configuration rather than an error.
void VictronSensor::publish_aux_not_used_(VE_REG_BMV_AUX_INPUT aux_input) {
  if (!this->should_report_unavailable_()) {
    return;
  }
  ESP_LOGI(TAG, "[%s] %s: not in use, device aux input is set to %s.", this->parent_->address_str(),
           this->get_name().c_str(), aux_input_to_str(aux_input));
  this->publish_state(NAN);
}

void VictronSensor::register_callback() {
  switch (this->type_) {
    case VICTRON_SENSOR_TYPE::ACTIVE_AC_IN:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::MULTI_RS:
            this->publish_state((uint8_t) msg->data.multi_rs.active_ac_in);
            break;
          case VICTRON_BLE_RECORD_TYPE::VE_BUS:
            this->publish_state((uint8_t) msg->data.ve_bus.active_ac_in);
            break;
          default:
            this->publish_invalid_("Device has no `active ac in` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::ACTIVE_AC_IN_POWER:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::MULTI_RS:
            this->publish_state_(msg->data.multi_rs.active_ac_in_power);
            break;
          case VICTRON_BLE_RECORD_TYPE::VE_BUS:
            this->publish_state_(msg->data.ve_bus.active_ac_in_power);
            break;
          default:
            this->publish_invalid_("Device has no `ac in power` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::AC_APPARENT_POWER:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::INVERTER:
            this->publish_state_(msg->data.inverter.ac_apparent_power);
            break;
          default:
            this->publish_invalid_("Device has no `ac apparent power` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::AC_CURRENT:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::INVERTER:
            this->publish_state_(msg->data.inverter.ac_current);
            break;
          case VICTRON_BLE_RECORD_TYPE::AC_CHARGER:
            this->publish_state_(msg->data.ac_charger.ac_current);
            break;
          default:
            this->publish_invalid_("Device has no `ac current` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::AC_OUT_POWER:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::INVERTER_RS:
            this->publish_state_(msg->data.inverter_rs.ac_out_power);
            break;
          case VICTRON_BLE_RECORD_TYPE::MULTI_RS:
            this->publish_state_(msg->data.multi_rs.active_ac_out_power);
            break;
          case VICTRON_BLE_RECORD_TYPE::VE_BUS:
            this->publish_state_(msg->data.ve_bus.ac_out_power);
            break;
          default:
            this->publish_invalid_("Device has no `ac out power` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::AC_VOLTAGE:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::INVERTER:
            this->publish_state_(msg->data.inverter.ac_voltage);
            break;
          default:
            this->publish_invalid_("Device has no `ac voltage` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::ALARM_REASON:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::BATTERY_MONITOR:
            this->publish_state((uint16_t) msg->data.battery_monitor.alarm_reason);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER:
            this->publish_state((uint16_t) msg->data.inverter.alarm_reason);
            break;
          case VICTRON_BLE_RECORD_TYPE::SMART_BATTERY_PROTECT:
            this->publish_state((uint16_t) msg->data.smart_battery_protect.alarm_reason);
            break;
          case VICTRON_BLE_RECORD_TYPE::DC_ENERGY_METER:
            this->publish_state((uint16_t) msg->data.dc_energy_meter.alarm_reason);
            break;
          default:
            this->publish_invalid_("Device has no `alarm reason` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::AUX_VOLTAGE:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::BATTERY_MONITOR:
            if (msg->data.battery_monitor.aux_input_type == VE_REG_BMV_AUX_INPUT::VE_REG_DC_CHANNEL2_VOLTAGE) {
              this->publish_state_(msg->data.battery_monitor.aux_input.aux_voltage);
            } else {
              this->publish_aux_not_used_(msg->data.battery_monitor.aux_input_type);
            }
            break;
          case VICTRON_BLE_RECORD_TYPE::DC_ENERGY_METER:
            if (msg->data.dc_energy_meter.aux_input_type == VE_REG_BMV_AUX_INPUT::VE_REG_DC_CHANNEL2_VOLTAGE) {
              this->publish_state_(msg->data.dc_energy_meter.aux_input.aux_voltage);
            } else {
              this->publish_aux_not_used_(msg->data.dc_energy_meter.aux_input_type);
            }
            break;
          default:
            this->publish_invalid_("Device has no `aux voltage` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::BATTERY_CURRENT:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SOLAR_CHARGER:
            this->publish_state_(msg->data.solar_charger.battery_current);
            break;
          case VICTRON_BLE_RECORD_TYPE::BATTERY_MONITOR:
            this->publish_state_(msg->data.battery_monitor.battery_current);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER_RS:
            this->publish_state_(msg->data.inverter_rs.battery_current);
            break;
          case VICTRON_BLE_RECORD_TYPE::AC_CHARGER:
            this->publish_state_(msg->data.ac_charger.battery_current_1);
            break;
          case VICTRON_BLE_RECORD_TYPE::LYNX_SMART_BMS:
            this->publish_state_(msg->data.lynx_smart_bms.battery_current);
            break;
          case VICTRON_BLE_RECORD_TYPE::MULTI_RS:
            this->publish_state_(msg->data.multi_rs.battery_current);
            break;
          case VICTRON_BLE_RECORD_TYPE::VE_BUS:
            this->publish_state_(msg->data.ve_bus.battery_current);
            break;
          case VICTRON_BLE_RECORD_TYPE::DC_ENERGY_METER:
            this->publish_state_(msg->data.dc_energy_meter.battery_current);
            break;
          default:
            this->publish_invalid_("Device has no `battery current` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::BATTERY_VOLTAGE:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SOLAR_CHARGER:
            this->publish_state_(msg->data.solar_charger.battery_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::BATTERY_MONITOR:
            this->publish_state_(msg->data.battery_monitor.battery_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER:
            this->publish_state_(msg->data.inverter.battery_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::SMART_LITHIUM:
            this->publish_state_(msg->data.smart_lithium.battery_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER_RS:
            this->publish_state_(msg->data.inverter_rs.battery_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::AC_CHARGER:
            this->publish_state_(msg->data.ac_charger.battery_voltage_1);
            break;
          case VICTRON_BLE_RECORD_TYPE::LYNX_SMART_BMS:
            this->publish_state_(msg->data.lynx_smart_bms.battery_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::MULTI_RS:
            this->publish_state_(msg->data.multi_rs.battery_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::VE_BUS:
            this->publish_state_(msg->data.ve_bus.battery_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::DC_ENERGY_METER:
            this->publish_state_(msg->data.dc_energy_meter.battery_voltage);
            break;
          default:
            this->publish_invalid_("Device has no `battery voltage` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::BATTERY_POWER:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SOLAR_CHARGER:
            this->publish_state_(msg->data.solar_charger.battery_current, msg->data.solar_charger.battery_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::BATTERY_MONITOR:
            this->publish_state_(msg->data.battery_monitor.battery_current, msg->data.battery_monitor.battery_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER_RS:
            this->publish_state_(msg->data.inverter_rs.battery_current, msg->data.inverter_rs.battery_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::AC_CHARGER:
            this->publish_state_(msg->data.ac_charger.battery_current_1, msg->data.ac_charger.battery_voltage_1);
            break;
          case VICTRON_BLE_RECORD_TYPE::LYNX_SMART_BMS:
            this->publish_state_(msg->data.lynx_smart_bms.battery_current, msg->data.lynx_smart_bms.battery_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::MULTI_RS:
            this->publish_state_(msg->data.multi_rs.battery_current, msg->data.multi_rs.battery_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::VE_BUS:
            this->publish_state_(msg->data.ve_bus.battery_current, msg->data.ve_bus.battery_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::DC_ENERGY_METER:
            this->publish_state_(msg->data.dc_energy_meter.battery_current, msg->data.dc_energy_meter.battery_voltage);
            break;
          default:
            this->publish_invalid_("Device has no `battery power` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::CHARGER_ERROR:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SOLAR_CHARGER:
            this->publish_state((uint8_t) msg->data.solar_charger.charger_error);
            break;
          case VICTRON_BLE_RECORD_TYPE::DCDC_CONVERTER:
            this->publish_state((uint8_t) msg->data.dcdc_converter.charger_error);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER_RS:
            this->publish_state((uint8_t) msg->data.inverter_rs.charger_error);
            break;
          case VICTRON_BLE_RECORD_TYPE::AC_CHARGER:
            this->publish_state((uint8_t) msg->data.ac_charger.charger_error);
            break;
          case VICTRON_BLE_RECORD_TYPE::SMART_BATTERY_PROTECT:
            this->publish_state((uint8_t) msg->data.smart_battery_protect.error_code);
            break;
          case VICTRON_BLE_RECORD_TYPE::MULTI_RS:
            this->publish_state((uint8_t) msg->data.multi_rs.charger_error);
            break;
          case VICTRON_BLE_RECORD_TYPE::ORION_XS:
            this->publish_state((uint8_t) msg->data.orion_xs.charger_error);
            break;
          default:
            this->publish_invalid_("Device has no `charger error` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::CONSUMED_AH:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::BATTERY_MONITOR:
            this->publish_state_(msg->data.battery_monitor.consumed_ah);
            break;
          case VICTRON_BLE_RECORD_TYPE::LYNX_SMART_BMS:
            this->publish_state_(msg->data.lynx_smart_bms.consumed_ah);
            break;
          default:
            this->publish_invalid_("Device has no `consumed Ah` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::DEVICE_STATE:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SOLAR_CHARGER:
            this->publish_state((uint8_t) msg->data.solar_charger.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER:
            this->publish_state((uint8_t) msg->data.inverter.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::DCDC_CONVERTER:
            this->publish_state((uint8_t) msg->data.dcdc_converter.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER_RS:
            this->publish_state((uint8_t) msg->data.inverter_rs.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::AC_CHARGER:
            this->publish_state((uint8_t) msg->data.ac_charger.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::SMART_BATTERY_PROTECT:
            this->publish_state((uint8_t) msg->data.smart_battery_protect.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::MULTI_RS:
            this->publish_state((uint8_t) msg->data.multi_rs.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::VE_BUS:
            this->publish_state((uint8_t) msg->data.ve_bus.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::ORION_XS:
            this->publish_state((uint8_t) msg->data.orion_xs.device_state);
            break;
          default:
            this->publish_invalid_("Device has no `device state` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::ERROR:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SMART_LITHIUM:
            this->publish_state((uint16_t) msg->data.smart_lithium.SmartLithium_error);
            break;
          case VICTRON_BLE_RECORD_TYPE::SMART_BATTERY_PROTECT:
            this->publish_state((uint8_t) msg->data.smart_battery_protect.error_code);
            break;
          case VICTRON_BLE_RECORD_TYPE::LYNX_SMART_BMS:
            this->publish_state((uint8_t) msg->data.lynx_smart_bms.error);
            break;
          case VICTRON_BLE_RECORD_TYPE::VE_BUS:
            this->publish_state((uint8_t) msg->data.ve_bus.ve_bus_error);
            break;
          default:
            this->publish_invalid_("Device has no `error` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::INPUT_VOLTAGE:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::DCDC_CONVERTER:
            this->publish_state_(msg->data.dcdc_converter.input_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::SMART_BATTERY_PROTECT:
            this->publish_state_(msg->data.smart_battery_protect.input_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::ORION_XS:
            this->publish_state_(msg->data.orion_xs.input_voltage);
            break;
          default:
            this->publish_invalid_("Device has no `input voltage` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::LOAD_CURRENT:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SOLAR_CHARGER:
            this->publish_state_(msg->data.solar_charger.load_current);
            break;
          default:
            this->publish_invalid_("Device has no `load current` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::LOAD_POWER:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SOLAR_CHARGER:
            this->publish_state_(msg->data.solar_charger.load_current, msg->data.solar_charger.battery_voltage);
            break;
          default:
            this->publish_invalid_("Device has no `load current` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::MID_VOLTAGE:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::BATTERY_MONITOR:
            if (msg->data.battery_monitor.aux_input_type == VE_REG_BMV_AUX_INPUT::VE_REG_BATTERY_MID_POINT_VOLTAGE) {
              this->publish_state_(msg->data.battery_monitor.aux_input.mid_voltage);
            } else {
              this->publish_aux_not_used_(msg->data.battery_monitor.aux_input_type);
            }
            break;
          default:
            this->publish_invalid_("Device has no `mid voltage` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::OUTPUT_VOLTAGE:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::DCDC_CONVERTER:
            this->publish_state_(msg->data.dcdc_converter.output_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::SMART_BATTERY_PROTECT:
            this->publish_state_(msg->data.smart_battery_protect.output_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::ORION_XS:
            this->publish_state_(msg->data.orion_xs.output_voltage);
            break;
          default:
            this->publish_invalid_("Device has no `output voltage` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::OFF_REASON:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::DCDC_CONVERTER:
            this->publish_state((uint32_t) msg->data.dcdc_converter.off_reason);
            break;
          case VICTRON_BLE_RECORD_TYPE::SMART_BATTERY_PROTECT:
            this->publish_state((uint32_t) msg->data.smart_battery_protect.off_reason);
            break;
          case VICTRON_BLE_RECORD_TYPE::ORION_XS:
            this->publish_state((uint32_t) msg->data.orion_xs.off_reason);
            break;
          default:
            this->publish_invalid_("Device has no `off reason` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::PV_POWER:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SOLAR_CHARGER:
            this->publish_state_(msg->data.solar_charger.pv_power);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER_RS:
            this->publish_state_(msg->data.inverter_rs.pv_power);
            break;
          case VICTRON_BLE_RECORD_TYPE::MULTI_RS:
            this->publish_state_(msg->data.multi_rs.pv_power);
            break;
          default:
            this->publish_invalid_("Device has no `PV power` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::STATE_OF_CHARGE:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::BATTERY_MONITOR:
            this->publish_state_(msg->data.battery_monitor.state_of_charge);
            break;
          case VICTRON_BLE_RECORD_TYPE::LYNX_SMART_BMS:
            this->publish_state_(msg->data.lynx_smart_bms.soc);
            break;
          case VICTRON_BLE_RECORD_TYPE::VE_BUS:
            this->publish_state_(msg->data.ve_bus.soc);
            break;
          default:
            this->publish_invalid_("Device has no `state of charge` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::TEMPERATURE:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::BATTERY_MONITOR:
            if (msg->data.battery_monitor.aux_input_type == VE_REG_BMV_AUX_INPUT::VE_REG_BAT_TEMPERATURE) {
              this->publish_state_(msg->data.battery_monitor.aux_input.temperature);
            } else {
              this->publish_aux_not_used_(msg->data.battery_monitor.aux_input_type);
            }
            break;
          case VICTRON_BLE_RECORD_TYPE::AC_CHARGER:
            this->publish_state_(msg->data.ac_charger.temperature);
            break;
          case VICTRON_BLE_RECORD_TYPE::SMART_LITHIUM:
            this->publish_state_(msg->data.smart_lithium.battery_temperature);
            break;
          case VICTRON_BLE_RECORD_TYPE::LYNX_SMART_BMS:
            this->publish_state_(msg->data.lynx_smart_bms.temperature);
            break;
          case VICTRON_BLE_RECORD_TYPE::VE_BUS:
            this->publish_state_(msg->data.ve_bus.battery_temperature);
            break;
          case VICTRON_BLE_RECORD_TYPE::DC_ENERGY_METER:
            if (msg->data.dc_energy_meter.aux_input_type == VE_REG_BMV_AUX_INPUT::VE_REG_BAT_TEMPERATURE) {
              this->publish_state_(msg->data.dc_energy_meter.aux_input.temperature);
            } else {
              this->publish_aux_not_used_(msg->data.dc_energy_meter.aux_input_type);
            }
            break;
          default:
            this->publish_invalid_("Device has no `temperature` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::TIME_TO_GO:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::BATTERY_MONITOR:
            this->publish_state_(msg->data.battery_monitor.time_to_go);
            break;
          case VICTRON_BLE_RECORD_TYPE::LYNX_SMART_BMS:
            this->publish_state_(msg->data.lynx_smart_bms.ttg);
            break;
          default:
            this->publish_invalid_("Device has no `time to go` field.");
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::YIELD_TODAY:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SOLAR_CHARGER:
            this->publish_state_(msg->data.solar_charger.yield_today);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER_RS:
            this->publish_state_(msg->data.inverter_rs.yield_today);
            break;
          case VICTRON_BLE_RECORD_TYPE::MULTI_RS:
            this->publish_state_(msg->data.multi_rs.yield_today);
            break;
          default:
            this->publish_invalid_("Device has no `yield today` field.");
            break;
        }
      });
      break;

      // SMART_LITHIUM
    case VICTRON_SENSOR_TYPE::BMS_FLAGS:
    case VICTRON_SENSOR_TYPE::CELL1:
    case VICTRON_SENSOR_TYPE::CELL2:
    case VICTRON_SENSOR_TYPE::CELL3:
    case VICTRON_SENSOR_TYPE::CELL4:
    case VICTRON_SENSOR_TYPE::CELL5:
    case VICTRON_SENSOR_TYPE::CELL6:
    case VICTRON_SENSOR_TYPE::CELL7:
    case VICTRON_SENSOR_TYPE::CELL8:
    case VICTRON_SENSOR_TYPE::BALANCER_STATUS:
      this->parent_->add_on_smart_lithium_message_callback([this](const VICTRON_BLE_RECORD_SMART_LITHIUM *val) {
        switch (this->type_) {
          case VICTRON_SENSOR_TYPE::BMS_FLAGS:
            this->publish_state((uint32_t) val->bms_flags);
            break;
          case VICTRON_SENSOR_TYPE::CELL1:
            this->publish_state_(val->cell1);
            break;
          case VICTRON_SENSOR_TYPE::CELL2:
            this->publish_state_(val->cell2);
            break;
          case VICTRON_SENSOR_TYPE::CELL3:
            this->publish_state_(val->cell3);
            break;
          case VICTRON_SENSOR_TYPE::CELL4:
            this->publish_state_(val->cell4);
            break;
          case VICTRON_SENSOR_TYPE::CELL5:
            this->publish_state_(val->cell5);
            break;
          case VICTRON_SENSOR_TYPE::CELL6:
            this->publish_state_(val->cell6);
            break;
          case VICTRON_SENSOR_TYPE::CELL7:
            this->publish_state_(val->cell7);
            break;
          case VICTRON_SENSOR_TYPE::CELL8:
            this->publish_state_(val->cell8);
            break;
          case VICTRON_SENSOR_TYPE::BALANCER_STATUS:
            if ((uint8_t) val->balancer_status == 0xF) {
              this->publish_state(NAN);
            } else {
              this->publish_state((uint8_t) val->balancer_status);
            }
            break;
          default:
            break;
        }
      });
      break;

      // AC_CHARGER
    case VICTRON_SENSOR_TYPE::BATTERY_CURRENT_2:
    case VICTRON_SENSOR_TYPE::BATTERY_VOLTAGE_2:
    case VICTRON_SENSOR_TYPE::BATTERY_POWER_2:
    case VICTRON_SENSOR_TYPE::BATTERY_CURRENT_3:
    case VICTRON_SENSOR_TYPE::BATTERY_VOLTAGE_3:
    case VICTRON_SENSOR_TYPE::BATTERY_POWER_3:
      this->parent_->add_on_ac_charger_message_callback([this](const VICTRON_BLE_RECORD_AC_CHARGER *val) {
        switch (this->type_) {
          case VICTRON_SENSOR_TYPE::BATTERY_CURRENT_2:
            this->publish_state_(val->battery_current_2);
            break;
          case VICTRON_SENSOR_TYPE::BATTERY_VOLTAGE_2:
            this->publish_state_(val->battery_voltage_2);
            break;
          case VICTRON_SENSOR_TYPE::BATTERY_POWER_2:
            this->publish_state_(val->battery_current_2, val->battery_voltage_2);
            break;
          case VICTRON_SENSOR_TYPE::BATTERY_CURRENT_3:
            this->publish_state_(val->battery_current_3);
            break;
          case VICTRON_SENSOR_TYPE::BATTERY_VOLTAGE_3:
            this->publish_state_(val->battery_voltage_3);
            break;
          case VICTRON_SENSOR_TYPE::BATTERY_POWER_3:
            this->publish_state_(val->battery_current_3, val->battery_voltage_3);
            break;
          default:
            break;
        }
      });
      break;

      // SMART_BATTERY_PROTECT
    case VICTRON_SENSOR_TYPE::OUTPUT_STATE:
    case VICTRON_SENSOR_TYPE::WARNING_REASON:
      this->parent_->add_on_smart_battery_protect_message_callback(
          [this](const VICTRON_BLE_RECORD_SMART_BATTERY_PROTECT *val) {
            switch (this->type_) {
              case VICTRON_SENSOR_TYPE::OUTPUT_STATE:
                this->publish_state((uint8_t) val->output_state);
                break;
              case VICTRON_SENSOR_TYPE::WARNING_REASON:
                this->publish_state((uint16_t) val->warning_reason);
                break;
              default:
                break;
            }
          });
      break;

      // LYNX_SMART_BMS
    case VICTRON_SENSOR_TYPE::IO_STATUS:
    case VICTRON_SENSOR_TYPE::WARNINGS_ALARMS:
      this->parent_->add_on_lynx_smart_bms_message_callback([this](const VICTRON_BLE_RECORD_LYNX_SMART_BMS *val) {
        switch (this->type_) {
          case VICTRON_SENSOR_TYPE::IO_STATUS:
            this->publish_state((uint16_t) val->io_status);
            break;
          case VICTRON_SENSOR_TYPE::WARNINGS_ALARMS:
            this->publish_state((uint32_t) val->warnings_alarms);
            break;
          default:
            break;
        }
      });
      break;

      // VE_BUS
    case VICTRON_SENSOR_TYPE::ALARM:
      this->parent_->add_on_ve_bus_message_callback([this](const VICTRON_BLE_RECORD_VE_BUS *val) {
        switch (this->type_) {
          case VICTRON_SENSOR_TYPE::ALARM:
            this->publish_state((uint8_t) val->alarm);
            break;
          default:
            break;
        }
      });
      break;

      // DC_ENERGY_METER
    case VICTRON_SENSOR_TYPE::BMV_MONITOR_MODE:
      this->parent_->add_on_dc_energy_meter_message_callback([this](const VICTRON_BLE_RECORD_DC_ENERGY_METER *val) {
        switch (this->type_) {
          case VICTRON_SENSOR_TYPE::BMV_MONITOR_MODE:
            this->publish_state((int16_t) val->bmv_monitor_mode);
            break;
          default:
            break;
        }
      });
      break;

      // ORION_XS
    case VICTRON_SENSOR_TYPE::OUTPUT_CURRENT:
    case VICTRON_SENSOR_TYPE::INPUT_CURRENT:
    case VICTRON_SENSOR_TYPE::OUTPUT_POWER:
    case VICTRON_SENSOR_TYPE::INPUT_POWER:
      this->parent_->add_on_orion_xs_message_callback([this](const VICTRON_BLE_RECORD_ORION_XS *val) {
        switch (this->type_) {
          case VICTRON_SENSOR_TYPE::OUTPUT_CURRENT:
            this->publish_state_(val->output_current);
            break;
          case VICTRON_SENSOR_TYPE::INPUT_CURRENT:
            this->publish_state_(val->input_current);
            break;
          case VICTRON_SENSOR_TYPE::OUTPUT_POWER:
            this->publish_state_(val->output_current, val->output_voltage);
            break;
          case VICTRON_SENSOR_TYPE::INPUT_POWER:
            this->publish_state_(val->input_current, val->input_voltage);
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

}  // namespace victron_ble
}  // namespace esphome
