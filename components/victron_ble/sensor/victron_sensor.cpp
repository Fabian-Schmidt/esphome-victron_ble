#include "victron_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace victron_ble {

static const char *const TAG = "victron_ble.sensor";

void VictronSensor::dump_config() {
  LOG_SENSOR("", "Victron Sensor", this);
  ESP_LOGCONFIG(TAG, "Type %u", this->type_);
}

void VictronSensor::setup() {
  switch (this->type_) {
    case VICTRON_SENSOR_TYPE::AC_OUT_POWER:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::INVERTER_RS:
            this->publish_state((u_int16_t) msg->data.inverter_rs.ac_out_power);
            break;
          case VICTRON_BLE_RECORD_TYPE::MULTI_RS:
            this->publish_state((u_int16_t) msg->data.multi_rs.active_ac_out_power);
            break;
          case VICTRON_BLE_RECORD_TYPE::VE_BUS:
            this->publish_state((u_int16_t) msg->data.ve_bus.active_ac_out_power);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no ac out power.", this->parent_->address_str().c_str());
            this->publish_state(NAN);
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::ALARM_REASON:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::BATTERY_MONITOR:
            this->publish_state((u_int16_t) msg->data.battery_monitor.alarm_reason);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER:
            this->publish_state((u_int16_t) msg->data.inverter.alarm_reason);
            break;
          case VICTRON_BLE_RECORD_TYPE::SMART_BATTERY_PROTECT:
            this->publish_state((u_int16_t) msg->data.smart_battery_protect.alarm_reason);
            break;
          case VICTRON_BLE_RECORD_TYPE::DC_ENERGY_METER:
            this->publish_state((u_int16_t) msg->data.dc_energy_meter.alarm_reason);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no alarm reason.", this->parent_->address_str().c_str());
            this->publish_state(NAN);
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::AUX_VOLTAGE:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::BATTERY_MONITOR:
            if (msg->data.battery_monitor.aux_input_type == VE_REG_BMV_AUX_INPUT::VE_REG_DC_CHANNEL2_VOLTAGE) {
              this->publish_state(0.01f * msg->data.battery_monitor.aux_input.aux_voltage);
            } else {
              ESP_LOGW(TAG, "[%s] Incorrect Aux input configuration.", this->parent_->address_str().c_str());
              this->publish_state(NAN);
            }
            break;
          case VICTRON_BLE_RECORD_TYPE::DC_ENERGY_METER:
            if (msg->data.dc_energy_meter.aux_input_type == VE_REG_BMV_AUX_INPUT::VE_REG_DC_CHANNEL2_VOLTAGE) {
              this->publish_state(0.01f * msg->data.dc_energy_meter.aux_input.aux_voltage );
            } else {
              ESP_LOGW(TAG, "[%s] Incorrect Aux input configuration.", this->parent_->address_str().c_str());
              this->publish_state(NAN);
            }
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no aux voltage.", this->parent_->address_str().c_str());
            this->publish_state(NAN);
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::BATTERY_CURRENT:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SOLAR_CHARGER:
            this->publish_state(0.1f * msg->data.solar_charger.battery_current);
            break;
          case VICTRON_BLE_RECORD_TYPE::BATTERY_MONITOR:
            this->publish_state(0.001f * msg->data.battery_monitor.battery_current);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER_RS:
            this->publish_state(0.1f * msg->data.inverter_rs.battery_current);
            break;
          case VICTRON_BLE_RECORD_TYPE::LYNX_SMART_BMS:
            this->publish_state(0.1f * msg->data.lynx_smart_bms.battery_current);
            break;
          case VICTRON_BLE_RECORD_TYPE::MULTI_RS:
            this->publish_state(0.1f * msg->data.multi_rs.battery_current);
            break;
          case VICTRON_BLE_RECORD_TYPE::VE_BUS:
            this->publish_state(0.1f * msg->data.ve_bus.battery_current);
            break;
          case VICTRON_BLE_RECORD_TYPE::DC_ENERGY_METER:
            this->publish_state(0.001f * msg->data.dc_energy_meter.battery_current);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no battery current.", this->parent_->address_str().c_str());
            this->publish_state(NAN);
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::BATTERY_VOLTAGE:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SOLAR_CHARGER:
            this->publish_state(0.01f * msg->data.solar_charger.battery_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::BATTERY_MONITOR:
            this->publish_state(0.01f * msg->data.battery_monitor.battery_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER:
            this->publish_state(0.01f * msg->data.inverter.battery_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::SMART_LITHIUM:
            this->publish_state(0.01f * msg->data.smart_lithium.battery_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER_RS:
            this->publish_state(0.01f * msg->data.inverter_rs.battery_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::LYNX_SMART_BMS:
            this->publish_state(0.01f * msg->data.lynx_smart_bms.battery_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::MULTI_RS:
            this->publish_state(0.01f * msg->data.multi_rs.battery_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::VE_BUS:
            this->publish_state(0.01f * msg->data.ve_bus.battery_voltage);
            break;
          case VICTRON_BLE_RECORD_TYPE::DC_ENERGY_METER:
            this->publish_state(0.01f * msg->data.dc_energy_meter.battery_voltage);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no battery voltage.", this->parent_->address_str().c_str());
            this->publish_state(NAN);
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::CHARGER_ERROR:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SOLAR_CHARGER:
            this->publish_state((u_int8_t) msg->data.solar_charger.charger_error);
            break;
          case VICTRON_BLE_RECORD_TYPE::DCDC_CONVERTER:
            this->publish_state((u_int8_t) msg->data.dcdc_converter.charger_error);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER_RS:
            this->publish_state((u_int8_t) msg->data.inverter_rs.charger_error);
            break;
          case VICTRON_BLE_RECORD_TYPE::SMART_BATTERY_PROTECT:
            this->publish_state((u_int8_t) msg->data.smart_battery_protect.error_code);
            break;
          case VICTRON_BLE_RECORD_TYPE::MULTI_RS:
            this->publish_state((u_int8_t) msg->data.multi_rs.charger_error);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no charger error.", this->parent_->address_str().c_str());
            this->publish_state(NAN);
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::CONSUMED_AH:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::BATTERY_MONITOR:
            this->publish_state(-0.1f * msg->data.battery_monitor.consumed_ah);
            break;
          case VICTRON_BLE_RECORD_TYPE::LYNX_SMART_BMS:
            this->publish_state(-0.1f * msg->data.lynx_smart_bms.consumed_ah);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no consumed Ah.", this->parent_->address_str().c_str());
            this->publish_state(NAN);
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::DEVICE_STATE:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SOLAR_CHARGER:
            this->publish_state((u_int8_t) msg->data.solar_charger.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER:
            this->publish_state((u_int8_t) msg->data.inverter.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::DCDC_CONVERTER:
            this->publish_state((u_int8_t) msg->data.dcdc_converter.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER_RS:
            this->publish_state((u_int8_t) msg->data.inverter_rs.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::SMART_BATTERY_PROTECT:
            this->publish_state((u_int8_t) msg->data.smart_battery_protect.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::MULTI_RS:
            this->publish_state((u_int8_t) msg->data.multi_rs.device_state);
            break;
          case VICTRON_BLE_RECORD_TYPE::VE_BUS:
            this->publish_state((u_int8_t) msg->data.ve_bus.device_state);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no device state.", this->parent_->address_str().c_str());
            this->publish_state(NAN);
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::LOAD_CURRENT:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SOLAR_CHARGER:
            this->publish_state(-0.1f * msg->data.solar_charger.load_current);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no load current.", this->parent_->address_str().c_str());
            this->publish_state(NAN);
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::MID_VOLTAGE:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::BATTERY_MONITOR:
            if (msg->data.battery_monitor.aux_input_type == VE_REG_BMV_AUX_INPUT::VE_REG_BATTERY_MID_POINT_VOLTAGE) {
              this->publish_state(0.01f * msg->data.battery_monitor.aux_input.mid_voltage);
            } else {
              ESP_LOGW(TAG, "[%s] Incorrect Aux input configuration.", this->parent_->address_str().c_str());
              this->publish_state(NAN);
            }
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no mid voltage.", this->parent_->address_str().c_str());
            this->publish_state(NAN);
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::PV_POWER:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SOLAR_CHARGER:
            this->publish_state(msg->data.solar_charger.yield_today);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER_RS:
            this->publish_state(msg->data.inverter_rs.yield_today);
            break;
          case VICTRON_BLE_RECORD_TYPE::MULTI_RS:
            this->publish_state(msg->data.multi_rs.yield_today);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no PV power.", this->parent_->address_str().c_str());
            this->publish_state(NAN);
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::STATE_OF_CHARGE:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::BATTERY_MONITOR:
            this->publish_state(0.1f * msg->data.battery_monitor.state_of_charge);
            break;
          case VICTRON_BLE_RECORD_TYPE::LYNX_SMART_BMS:
            this->publish_state(0.1f * msg->data.lynx_smart_bms.soc);
            break;
          case VICTRON_BLE_RECORD_TYPE::VE_BUS:
            this->publish_state(0.1f * msg->data.ve_bus.soc);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no state of charge.", this->parent_->address_str().c_str());
            this->publish_state(NAN);
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::TEMPERATURE:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::BATTERY_MONITOR:
            if (msg->data.battery_monitor.aux_input_type == VE_REG_BMV_AUX_INPUT::VE_REG_BAT_TEMPERATURE) {
              this->publish_state(0.01f * msg->data.battery_monitor.aux_input.temperature - 273.15f);
            } else {
              ESP_LOGW(TAG, "[%s] Incorrect Aux input configuration.", this->parent_->address_str().c_str());
              this->publish_state(NAN);
            }
            break;
          case VICTRON_BLE_RECORD_TYPE::SMART_LITHIUM:
            this->publish_state(-40.0f + msg->data.smart_lithium.battery_temperature);
            break;
          case VICTRON_BLE_RECORD_TYPE::LYNX_SMART_BMS:
            this->publish_state(-40.0f + msg->data.lynx_smart_bms.temperature);
            break;
          case VICTRON_BLE_RECORD_TYPE::VE_BUS:
            this->publish_state(-40.0f + msg->data.ve_bus.battery_temperature);
            break;
          case VICTRON_BLE_RECORD_TYPE::DC_ENERGY_METER:
            if (msg->data.dc_energy_meter.aux_input_type == VE_REG_BMV_AUX_INPUT::VE_REG_BAT_TEMPERATURE) {
              this->publish_state(0.01f * msg->data.dc_energy_meter.aux_input.temperature - 273.15f);
            } else {
              ESP_LOGW(TAG, "[%s] Incorrect Aux input configuration.", this->parent_->address_str().c_str());
              this->publish_state(NAN);
            }
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no temperature.", this->parent_->address_str().c_str());
            this->publish_state(NAN);
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::TIME_TO_GO:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::BATTERY_MONITOR:
            this->publish_state(msg->data.battery_monitor.time_to_go);
            break;
          case VICTRON_BLE_RECORD_TYPE::LYNX_SMART_BMS:
            this->publish_state(msg->data.lynx_smart_bms.ttg);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no time to go.", this->parent_->address_str().c_str());
            this->publish_state(NAN);
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::YIELD_TODAY:
      this->parent_->add_on_message_callback([this](const VictronBleData *msg) {
        switch (msg->record_type) {
          case VICTRON_BLE_RECORD_TYPE::SOLAR_CHARGER:
            this->publish_state(0.01f * msg->data.solar_charger.yield_today);
            break;
          case VICTRON_BLE_RECORD_TYPE::INVERTER_RS:
            this->publish_state(0.01f * msg->data.inverter_rs.yield_today);
            break;
          case VICTRON_BLE_RECORD_TYPE::MULTI_RS:
            this->publish_state(0.01f * msg->data.multi_rs.yield_today);
            break;
          default:
            ESP_LOGW(TAG, "[%s] Device has no yield today.", this->parent_->address_str().c_str());
            this->publish_state(NAN);
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::INVERTER_AC_APPARENT_POWER:
    case VICTRON_SENSOR_TYPE::INVERTER_AC_VOLTAGE:
    case VICTRON_SENSOR_TYPE::INVERTER_AC_CURRENT:
      this->parent_->add_on_inverter_message_callback([this](const VICTRON_BLE_RECORD_INVERTER *val) {
        switch (this->type_) {
          case VICTRON_SENSOR_TYPE::INVERTER_AC_APPARENT_POWER:
            this->publish_state(val->ac_apparent_power);
            break;
          case VICTRON_SENSOR_TYPE::INVERTER_AC_VOLTAGE:
            this->publish_state(0.01f * val->ac_voltage);
            break;
          case VICTRON_SENSOR_TYPE::INVERTER_AC_CURRENT:
            this->publish_state(0.1f * val->ac_current);
            break;
          default:
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::DCDC_CONVERTER_INPUT_VOLTAGE:
    case VICTRON_SENSOR_TYPE::DCDC_CONVERTER_OUTPUT_VOLTAGE:
    case VICTRON_SENSOR_TYPE::DCDC_CONVERTER_OFF_REASON:
      this->parent_->add_on_dcdc_converter_message_callback([this](const VICTRON_BLE_RECORD_DCDC_CONVERTER *val) {
        switch (this->type_) {
          case VICTRON_SENSOR_TYPE::DCDC_CONVERTER_INPUT_VOLTAGE:
            this->publish_state(0.01f * val->input_voltage);
            break;
          case VICTRON_SENSOR_TYPE::DCDC_CONVERTER_OUTPUT_VOLTAGE:
            this->publish_state(0.01f * val->output_voltage);
            break;
          case VICTRON_SENSOR_TYPE::DCDC_CONVERTER_OFF_REASON:
            this->publish_state((u_int32_t) val->off_reason);
            break;
          default:
            break;
        }
      });
      break;
    case VICTRON_SENSOR_TYPE::SMART_LITHIUM_BMS_FLAGS:
    case VICTRON_SENSOR_TYPE::SMART_LITHIUM_ERROR:
    case VICTRON_SENSOR_TYPE::SMART_LITHIUM_CELL1:
    case VICTRON_SENSOR_TYPE::SMART_LITHIUM_CELL2:
    case VICTRON_SENSOR_TYPE::SMART_LITHIUM_CELL3:
    case VICTRON_SENSOR_TYPE::SMART_LITHIUM_CELL4:
    case VICTRON_SENSOR_TYPE::SMART_LITHIUM_CELL5:
    case VICTRON_SENSOR_TYPE::SMART_LITHIUM_CELL6:
    case VICTRON_SENSOR_TYPE::SMART_LITHIUM_CELL7:
    case VICTRON_SENSOR_TYPE::SMART_LITHIUM_CELL8:
    case VICTRON_SENSOR_TYPE::SMART_LITHIUM_BALANCER_STATUS:
      this->parent_->add_on_smart_lithium_message_callback([this](const VICTRON_BLE_RECORD_SMART_LITHIUM *val) {
        switch (this->type_) {
          case VICTRON_SENSOR_TYPE::SMART_LITHIUM_BMS_FLAGS:
            this->publish_state((u_int32_t) val->bms_flags);
            break;
          case VICTRON_SENSOR_TYPE::SMART_LITHIUM_ERROR:
            this->publish_state((u_int16_t) val->SmartLithium_error);
            break;
          case VICTRON_SENSOR_TYPE::SMART_LITHIUM_CELL1:
            if (val->cell1 == 0x7F) {
              this->publish_state(NAN);
            } else {
              this->publish_state(0.01f * val->cell1 + 2.60f);
            }
            break;
          case VICTRON_SENSOR_TYPE::SMART_LITHIUM_CELL2:
            if (val->cell2 == 0x7F) {
              this->publish_state(NAN);
            } else {
              this->publish_state(0.01f * val->cell2 + 2.60f);
            }
            break;
          case VICTRON_SENSOR_TYPE::SMART_LITHIUM_CELL3:
            if (val->cell3 == 0x7F) {
              this->publish_state(NAN);
            } else {
              this->publish_state(0.01f * val->cell3 + 2.60f);
            }
            break;
          case VICTRON_SENSOR_TYPE::SMART_LITHIUM_CELL4:
            if (val->cell4 == 0x7F) {
              this->publish_state(NAN);
            } else {
              this->publish_state(0.01f * val->cell4 + 2.60f);
            }
            break;
          case VICTRON_SENSOR_TYPE::SMART_LITHIUM_CELL5:
            if (val->cell5 == 0x7F) {
              this->publish_state(NAN);
            } else {
              this->publish_state(0.01f * val->cell5 + 2.60f);
            }
            break;
          case VICTRON_SENSOR_TYPE::SMART_LITHIUM_CELL6:
            if (val->cell6 == 0x7F) {
              this->publish_state(NAN);
            } else {
              this->publish_state(0.01f * val->cell6 + 2.60f);
            }
            break;
          case VICTRON_SENSOR_TYPE::SMART_LITHIUM_CELL7:
            if (val->cell7 == 0x7F) {
              this->publish_state(NAN);
            } else {
              this->publish_state(0.01f * val->cell7 + 2.60f);
            }
            break;
          case VICTRON_SENSOR_TYPE::SMART_LITHIUM_CELL8:
            if (val->cell8 == 0x7F) {
              this->publish_state(NAN);
            } else {
              this->publish_state(0.01f * val->cell8 + 2.60f);
            }
            break;
          case VICTRON_SENSOR_TYPE::SMART_LITHIUM_BALANCER_STATUS:
            this->publish_state((u_int8_t) val->balancer_status);
            break;
          default:
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::SMART_BATTERY_PROTECT_OUTPUT_STATE:
    case VICTRON_SENSOR_TYPE::SMART_BATTERY_PROTECT_ERROR_CODE:
    case VICTRON_SENSOR_TYPE::SMART_BATTERY_PROTECT_WARNING_REASON:
    case VICTRON_SENSOR_TYPE::SMART_BATTERY_PROTECT_INPUT_VOLTAGE:
    case VICTRON_SENSOR_TYPE::SMART_BATTERY_PROTECT_OUTPUT_VOLTAGE:
    case VICTRON_SENSOR_TYPE::SMART_BATTERY_PROTECT_OFF_REASON:
      this->parent_->add_on_smart_battery_protect_message_callback(
          [this](const VICTRON_BLE_RECORD_SMART_BATTERY_PROTECT *val) {
            switch (this->type_) {
              case VICTRON_SENSOR_TYPE::SMART_BATTERY_PROTECT_OUTPUT_STATE:
                this->publish_state((u_int8_t) val->output_state);
                break;
              case VICTRON_SENSOR_TYPE::SMART_BATTERY_PROTECT_ERROR_CODE:
                this->publish_state((u_int8_t) val->error_code);
                break;
              case VICTRON_SENSOR_TYPE::SMART_BATTERY_PROTECT_WARNING_REASON:
                this->publish_state((u_int16_t) val->warning_reason);
                break;
              case VICTRON_SENSOR_TYPE::SMART_BATTERY_PROTECT_INPUT_VOLTAGE:
                this->publish_state(0.01f * val->input_voltage);
                break;
              case VICTRON_SENSOR_TYPE::SMART_BATTERY_PROTECT_OUTPUT_VOLTAGE:
                this->publish_state(0.01f * val->output_voltage);
                break;
              case VICTRON_SENSOR_TYPE::SMART_BATTERY_PROTECT_OFF_REASON:
                this->publish_state((u_int32_t) val->off_reason);
                break;
              default:
                break;
            }
          });
      break;
    case VICTRON_SENSOR_TYPE::LYNX_SMART_BMS_ERROR:
    case VICTRON_SENSOR_TYPE::LYNX_SMART_BMS_IO_STATUS:
    case VICTRON_SENSOR_TYPE::LYNX_SMART_BMS_WARNINGS_ALARMS:
      this->parent_->add_on_lynx_smart_bms_message_callback([this](const VICTRON_BLE_RECORD_LYNX_SMART_BMS *val) {
        switch (this->type_) {
          case VICTRON_SENSOR_TYPE::LYNX_SMART_BMS_ERROR:
            this->publish_state((u_int8_t) val->error);
            break;
          case VICTRON_SENSOR_TYPE::LYNX_SMART_BMS_IO_STATUS:
            this->publish_state((u_int16_t) val->io_status);
            break;
          case VICTRON_SENSOR_TYPE::LYNX_SMART_BMS_WARNINGS_ALARMS:
            this->publish_state((u_int32_t) val->warnings_alarms);
            break;
          default:
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::MULTI_RS_ACTIVE_AC_IN:
    case VICTRON_SENSOR_TYPE::MULTI_RS_ACTIVE_AC_IN_POWER:
      this->parent_->add_on_multi_rs_message_callback([this](const VICTRON_BLE_RECORD_MULTI_RS *val) {
        switch (this->type_) {
          case VICTRON_SENSOR_TYPE::MULTI_RS_ACTIVE_AC_IN:
            this->publish_state((u_int8_t) val->active_ac_in);
            break;
          case VICTRON_SENSOR_TYPE::MULTI_RS_ACTIVE_AC_IN_POWER:
            this->publish_state(val->active_ac_in_power);
            break;
          default:
            break;
        }
      });
      break;

    case VICTRON_SENSOR_TYPE::VE_BUS_ERROR:
    case VICTRON_SENSOR_TYPE::VE_BUS_ACTIVE_AC_IN:
    case VICTRON_SENSOR_TYPE::VE_BUS_ACTIVE_AC_IN_POWER:
    case VICTRON_SENSOR_TYPE::VE_BUS_ALARM:
      this->parent_->add_on_ve_bus_message_callback([this](const VICTRON_BLE_RECORD_VE_BUS *val) {
        switch (this->type_) {
          case VICTRON_SENSOR_TYPE::VE_BUS_ERROR:
            this->publish_state((u_int16_t) val->ve_bus_error);
            break;
          case VICTRON_SENSOR_TYPE::VE_BUS_ACTIVE_AC_IN:
            this->publish_state((u_int8_t) val->active_ac_in);
            break;
          case VICTRON_SENSOR_TYPE::VE_BUS_ACTIVE_AC_IN_POWER:
            this->publish_state(val->active_ac_in_power);
            break;
          case VICTRON_SENSOR_TYPE::VE_BUS_ALARM:
            this->publish_state((u_int8_t) val->alarm);
            break;
          default:
            break;
        }
      });
      break;
    case VICTRON_SENSOR_TYPE::DC_ENERGY_METER_BMV_MONITOR_MODE:
      this->parent_->add_on_dc_energy_meter_message_callback([this](const VICTRON_BLE_RECORD_DC_ENERGY_METER *val) {
        switch (this->type_) {
          case VICTRON_SENSOR_TYPE::DC_ENERGY_METER_BMV_MONITOR_MODE:
            this->publish_state((int16_t) val->bmv_monitor_mode);
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