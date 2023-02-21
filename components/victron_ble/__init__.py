import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import esp32_ble_tracker
from esphome.const import (
    CONF_BINDKEY,
    CONF_ID,
    CONF_MAC_ADDRESS,
)
from esphome.yaml_util import ESPHomeDumper

CODEOWNERS = ["@Fabian-Schmidt"]
DEPENDENCIES = ["esp32_ble_tracker"]

CONF_VICTRON_BLE_ID = "victron_ble_id"

victron_ble_ns = cg.esphome_ns.namespace("victron_ble")
VictronBle = victron_ble_ns.class_(
    "VictronBle", esp32_ble_tracker.ESPBTDeviceListener, cg.PollingComponent
)

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
    parts = [value[i : i + 2] for i in range(0, len(value), 2)]
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


