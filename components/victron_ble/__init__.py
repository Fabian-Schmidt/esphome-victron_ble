import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import esp32_ble_tracker
from esphome.const import (
    CONF_BINDKEY,
    CONF_ID,
    CONF_MAC_ADDRESS,
    CONF_TRIGGER_ID,
    CONF_ON_MESSAGE,
)
from esphome.yaml_util import ESPHomeDumper

CODEOWNERS = ["@Fabian-Schmidt"]
DEPENDENCIES = ["esp32_ble_tracker"]

CONF_VICTRON_BLE_ID = "victron_ble_id"
CONF_ON_BATTERY_MONITOR_MESSAGE = "on_battery_monitor_message"
CONF_ON_SOLAR_CHARGER_MESSAGE = "on_solar_charger_message"
CONF_ON_INVERTER_MESSAGE = "on_inverter_message"
CONF_ON_DCDC_CONVERTER_MESSAGE = "on_dcdc_converter_message"
CONF_ON_SMART_LITHIUM_MESSAGE = "on_smart_lithium_message"
CONF_ON_INVERTER_RS_MESSAGE = "on_inverter_rs_message"
CONF_ON_SMART_BATTERY_PROTECT_MESSAGE = "on_smart_battery_protect_message"
CONF_ON_LYNX_SMART_BMS_MESSAGE = "on_lynx_smart_bms_message"
CONF_ON_MULTI_RS_MESSAGE = "on_multi_rs_message"
CONF_ON_VE_BUS_MESSAGE = "on_ve_bus_message"
CONF_ON_DC_ENERGY_METER_MESSAGE = "on_dc_energy_meter_message"

MULTI_CONF = True

victron_ble_ns = cg.esphome_ns.namespace("victron_ble")
VictronBle = victron_ble_ns \
    .class_("VictronBle", esp32_ble_tracker.ESPBTDeviceListener, cg.PollingComponent)

VictronBleDataConstPtr = victron_ble_ns \
    .struct("VictronBleData") \
    .operator("ptr").operator("const")
MessageTrigger = victron_ble_ns \
    .class_("MessageTrigger", automation.Trigger.template(VictronBleDataConstPtr))

BatteryMonitorMessageConstPtr = victron_ble_ns \
    .struct("VICTRON_BLE_RECORD_BATTERY_MONITOR") \
    .operator("ptr").operator("const")
BatteryMonitorMessageTrigger = victron_ble_ns \
    .class_("BatteryMonitorMessageTrigger", automation.Trigger.template(BatteryMonitorMessageConstPtr))

SolarChargerMessageConstPtr = victron_ble_ns \
    .struct("VICTRON_BLE_RECORD_SOLAR_CHARGER") \
    .operator("ptr").operator("const")
SolarChargerMessageTrigger = victron_ble_ns \
    .class_("SolarChargerMessageTrigger", automation.Trigger.template(SolarChargerMessageConstPtr))

InverterMessageConstPtr = victron_ble_ns \
    .struct("VICTRON_BLE_RECORD_INVERTER") \
    .operator("ptr").operator("const")
InverterMessageTrigger = victron_ble_ns \
    .class_("InverterMessageTrigger", automation.Trigger.template(InverterMessageConstPtr))

DcDcConverterMessageConstPtr = victron_ble_ns \
    .struct("VICTRON_BLE_RECORD_DCDC_CONVERTER") \
    .operator("ptr").operator("const")
DcDcConverterMessageTrigger = victron_ble_ns \
    .class_("DcDcConverterMessageTrigger", automation.Trigger.template(DcDcConverterMessageConstPtr))

SmartLithiumMessageConstPtr = victron_ble_ns \
    .struct("VICTRON_BLE_RECORD_SMART_LITHIUM") \
    .operator("ptr").operator("const")
SmartLithiumMessageTrigger = victron_ble_ns \
    .class_("SmartLithiumMessageTrigger", automation.Trigger.template(SmartLithiumMessageConstPtr))

InverterRsMessageConstPtr = victron_ble_ns \
    .struct("VICTRON_BLE_RECORD_INVERTER_RS") \
    .operator("ptr").operator("const")
InverterRsMessageTrigger = victron_ble_ns \
    .class_("InverterRsMessageTrigger", automation.Trigger.template(InverterRsMessageConstPtr))

SmartBatteryProtectMessageConstPtr = victron_ble_ns \
    .struct("VICTRON_BLE_RECORD_SMART_BATTERY_PROTECT") \
    .operator("ptr").operator("const")
SmartBatteryProtectMessageTrigger = victron_ble_ns \
    .class_("SmartBatteryProtectMessageTrigger", automation.Trigger.template(SmartBatteryProtectMessageConstPtr))

LyncSmartBmsMessageConstPtr = victron_ble_ns \
    .struct("VICTRON_BLE_RECORD_LYNX_SMART_BMS") \
    .operator("ptr").operator("const")
LyncSmartBmsMessageTrigger = victron_ble_ns \
    .class_("LyncSmartBmsMessageTrigger", automation.Trigger.template(LyncSmartBmsMessageConstPtr))

MultiRsMessageConstPtr = victron_ble_ns \
    .struct("VICTRON_BLE_RECORD_MULTI_RS") \
    .operator("ptr").operator("const")
MultiRsMessageTrigger = victron_ble_ns \
    .class_("MultiRsMessageTrigger", automation.Trigger.template(MultiRsMessageConstPtr))

VeBusMessageConstPtr = victron_ble_ns \
    .struct("VICTRON_BLE_RECORD_VE_BUS") \
    .operator("ptr").operator("const")
VeBusMessageTrigger = victron_ble_ns \
    .class_("VeBusMessageTrigger", automation.Trigger.template(VeBusMessageConstPtr))

DcEnergyMeterMessageConstPtr = victron_ble_ns \
    .struct("VICTRON_BLE_RECORD_DC_ENERGY_METER") \
    .operator("ptr").operator("const")
DcEnergyMeterMessageTrigger = victron_ble_ns \
    .class_("DcEnergyMeterMessageTrigger", automation.Trigger.template(DcEnergyMeterMessageConstPtr))


class Array:
    def __init__(self, *parts):
        self.parts = parts

    def __str__(self):
        return "".join(f"{part:02X}" for part in self.parts)

    @property
    def as_array(self):
        from esphome.cpp_generator import RawExpression

        num = ", 0x".join(f"{part:02X}" for part in self.parts)
        return RawExpression(f"{{0x{num}}}")


ESPHomeDumper.add_multi_representer(Array, ESPHomeDumper.represent_stringify)


def bind_key_array(value):
    value = cv.bind_key(value)
    parts = [value[i: i + 2] for i in range(0, len(value), 2)]
    parts_int = []
    for part in parts:
        parts_int.append(int(part, 16))
    return Array(*parts_int)


CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(VictronBle),
            # TODO: Optional
            cv.Required(CONF_MAC_ADDRESS): cv.mac_address,
            # TODO: Optional
            cv.Required(CONF_BINDKEY): bind_key_array,
            cv.Optional(CONF_ON_MESSAGE): automation.validate_automation({
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(MessageTrigger),
            }),
            cv.Optional(CONF_ON_BATTERY_MONITOR_MESSAGE): automation.validate_automation({
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(BatteryMonitorMessageTrigger),
            }),
            cv.Optional(CONF_ON_SOLAR_CHARGER_MESSAGE): automation.validate_automation({
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(SolarChargerMessageTrigger),
            }),
            cv.Optional(CONF_ON_INVERTER_MESSAGE): automation.validate_automation({
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(InverterMessageTrigger),
            }),
            cv.Optional(CONF_ON_DCDC_CONVERTER_MESSAGE): automation.validate_automation({
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(DcDcConverterMessageTrigger),
            }),
            cv.Optional(CONF_ON_SMART_LITHIUM_MESSAGE): automation.validate_automation({
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(SmartLithiumMessageTrigger),
            }),
            cv.Optional(CONF_ON_INVERTER_RS_MESSAGE): automation.validate_automation({
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(InverterRsMessageTrigger),
            }),
            cv.Optional(CONF_ON_SMART_BATTERY_PROTECT_MESSAGE): automation.validate_automation({
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(SmartBatteryProtectMessageTrigger),
            }),
            cv.Optional(CONF_ON_LYNX_SMART_BMS_MESSAGE): automation.validate_automation({
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(LyncSmartBmsMessageTrigger),
            }),
            cv.Optional(CONF_ON_MULTI_RS_MESSAGE): automation.validate_automation({
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(MultiRsMessageTrigger),
            }),
            cv.Optional(CONF_ON_VE_BUS_MESSAGE): automation.validate_automation({
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(VeBusMessageTrigger),
            }),
            cv.Optional(CONF_ON_DC_ENERGY_METER_MESSAGE): automation.validate_automation({
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(DcEnergyMeterMessageTrigger),
            }),
        }
    )
    .extend(esp32_ble_tracker.ESP_BLE_DEVICE_SCHEMA)
    .extend(cv.polling_component_schema("30s"))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await esp32_ble_tracker.register_ble_device(var, config)

    if CONF_MAC_ADDRESS in config:
        cg.add(var.set_address(config[CONF_MAC_ADDRESS].as_hex))
    if CONF_BINDKEY in config:
        cg.add(var.set_bindkey(config[CONF_BINDKEY].as_array))

    for conf in config.get(CONF_ON_MESSAGE, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [(VictronBleDataConstPtr, "message")], conf)

    for conf in config.get(CONF_ON_BATTERY_MONITOR_MESSAGE, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [(BatteryMonitorMessageConstPtr, "message")], conf)

    for conf in config.get(CONF_ON_SOLAR_CHARGER_MESSAGE, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [(SolarChargerMessageConstPtr, "message")], conf)

    for conf in config.get(CONF_ON_INVERTER_MESSAGE, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [(InverterMessageConstPtr, "message")], conf)

    for conf in config.get(CONF_ON_DCDC_CONVERTER_MESSAGE, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [(DcDcConverterMessageConstPtr, "message")], conf)

    for conf in config.get(CONF_ON_SMART_LITHIUM_MESSAGE, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [(SmartLithiumMessageConstPtr, "message")], conf)

    for conf in config.get(CONF_ON_INVERTER_RS_MESSAGE, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [(InverterRsMessageConstPtr, "message")], conf)

    for conf in config.get(CONF_ON_SMART_BATTERY_PROTECT_MESSAGE, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [(SmartBatteryProtectMessageConstPtr, "message")], conf)

    for conf in config.get(CONF_ON_LYNX_SMART_BMS_MESSAGE, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [(LyncSmartBmsMessageConstPtr, "message")], conf)

    for conf in config.get(CONF_ON_MULTI_RS_MESSAGE, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [(MultiRsMessageConstPtr, "message")], conf)

    for conf in config.get(CONF_ON_VE_BUS_MESSAGE, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [(VeBusMessageConstPtr, "message")], conf)

    for conf in config.get(CONF_ON_DC_ENERGY_METER_MESSAGE, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [(DcEnergyMeterMessageConstPtr, "message")], conf)
