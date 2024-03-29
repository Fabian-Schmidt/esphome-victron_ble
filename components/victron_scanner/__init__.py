import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import esp32_ble_tracker
from esphome.const import CONF_ID

CODEOWNERS = ["@Fabian-Schmidt"]
DEPENDENCIES = ["esp32_ble_tracker"]

victron_scanner_ns = cg.esphome_ns.namespace("victron_scanner")
VictronListener = victron_scanner_ns.class_(
    "VictronListener", esp32_ble_tracker.ESPBTDeviceListener
)

CONFIG_SCHEMA = cv.All(
    cv.only_on_esp32,
    cv.require_esphome_version(2023, 8, 0),
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(VictronListener),
        }
    ).extend(esp32_ble_tracker.ESP_BLE_DEVICE_SCHEMA),
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await esp32_ble_tracker.register_ble_device(var, config)
