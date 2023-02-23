#pragma once

#include "esphome/core/component.h"
#include "victron_ble.h"

namespace esphome {
namespace victron_ble {

class MessageTrigger : public Trigger<const VictronBleData *> {
 public:
  explicit MessageTrigger(VictronBle *parent) {
    parent->add_on_message_callback(
        [this](const VictronBleData *message) { this->trigger(message); });
  }
};

class BatteryMonitorMessageTrigger : public Trigger<const VICTRON_BLE_RECORD_BATTERY_MONITOR *> {
 public:
  explicit BatteryMonitorMessageTrigger(VictronBle *parent) {
    parent->add_on_battery_monitor_message_callback(
        [this](const VICTRON_BLE_RECORD_BATTERY_MONITOR *message) { this->trigger(message); });
  }
};

class SolarChargerMessageTrigger : public Trigger<const VICTRON_BLE_RECORD_SOLAR_CHARGER *> {
 public:
  explicit SolarChargerMessageTrigger(VictronBle *parent) {
    parent->add_on_solar_charger_message_callback(
        [this](const VICTRON_BLE_RECORD_SOLAR_CHARGER *message) { this->trigger(message); });
  }
};

class InverterMessageTrigger : public Trigger<const VICTRON_BLE_RECORD_INVERTER *> {
 public:
  explicit InverterMessageTrigger(VictronBle *parent) {
    parent->add_on_inverter_message_callback(
        [this](const VICTRON_BLE_RECORD_INVERTER *message) { this->trigger(message); });
  }
};

class DcDcConverterMessageTrigger : public Trigger<const VICTRON_BLE_RECORD_DCDC_CONVERTER *> {
 public:
  explicit DcDcConverterMessageTrigger(VictronBle *parent) {
    parent->add_on_dcdc_converter_message_callback(
        [this](const VICTRON_BLE_RECORD_DCDC_CONVERTER *message) { this->trigger(message); });
  }
};

class SmartLithiumMessageTrigger : public Trigger<const VICTRON_BLE_RECORD_SMART_LITHIUM *> {
 public:
  explicit SmartLithiumMessageTrigger(VictronBle *parent) {
    parent->add_on_smart_lithium_message_callback(
        [this](const VICTRON_BLE_RECORD_SMART_LITHIUM *message) { this->trigger(message); });
  }
};

class InverterRsMessageTrigger : public Trigger<const VICTRON_BLE_RECORD_INVERTER_RS *> {
 public:
  explicit InverterRsMessageTrigger(VictronBle *parent) {
    parent->add_on_inverter_rs_message_callback(
        [this](const VICTRON_BLE_RECORD_INVERTER_RS *message) { this->trigger(message); });
  }
};

class SmartBatteryProtectMessageTrigger : public Trigger<const VICTRON_BLE_RECORD_SMART_BATTERY_PROTECT *> {
 public:
  explicit SmartBatteryProtectMessageTrigger(VictronBle *parent) {
    parent->add_on_smart_battery_protect_message_callback(
        [this](const VICTRON_BLE_RECORD_SMART_BATTERY_PROTECT *message) { this->trigger(message); });
  }
};

class LyncSmartBmsMessageTrigger : public Trigger<const VICTRON_BLE_RECORD_LYNX_SMART_BMS *> {
 public:
  explicit LyncSmartBmsMessageTrigger(VictronBle *parent) {
    parent->add_on_lynx_smart_bms_message_callback(
        [this](const VICTRON_BLE_RECORD_LYNX_SMART_BMS *message) { this->trigger(message); });
  }
};

class MultiRsMessageTrigger : public Trigger<const VICTRON_BLE_RECORD_MULTI_RS *> {
 public:
  explicit MultiRsMessageTrigger(VictronBle *parent) {
    parent->add_on_multi_rs_message_callback(
        [this](const VICTRON_BLE_RECORD_MULTI_RS *message) { this->trigger(message); });
  }
};

class VeBusMessageTrigger : public Trigger<const VICTRON_BLE_RECORD_VE_BUS *> {
 public:
  explicit VeBusMessageTrigger(VictronBle *parent) {
    parent->add_on_ve_bus_message_callback(
        [this](const VICTRON_BLE_RECORD_VE_BUS *message) { this->trigger(message); });
  }
};

class DcEnergyMeterMessageTrigger : public Trigger<const VICTRON_BLE_RECORD_DC_ENERGY_METER *> {
 public:
  explicit DcEnergyMeterMessageTrigger(VictronBle *parent) {
    parent->add_on_dc_energy_meter_message_callback(
        [this](const VICTRON_BLE_RECORD_DC_ENERGY_METER *message) { this->trigger(message); });
  }
};

}  // namespace victron_ble
}  // namespace esphome