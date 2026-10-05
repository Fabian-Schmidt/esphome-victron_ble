import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

try:
    from esphome.components import ble_device_base
except ImportError as err:  # ESPHome < 2026.8.0
    raise ImportError(
        "victron_scanner requires ESPHome 2026.8.0 or newer (ble_device_base BLE layer)"
    ) from err

CODEOWNERS = ["@Fabian-Schmidt"]
AUTO_LOAD = ["ble_device_base"]

victron_scanner_ns = cg.esphome_ns.namespace("victron_scanner")
VictronListener = victron_scanner_ns.class_(
    "VictronListener", ble_device_base.ESPBTDeviceListener
)

CONFIG_SCHEMA = cv.All(
    cv.require_esphome_version(2026, 8, 0),
    ble_device_base.rename_legacy_hub_id("victron_scanner"),
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(VictronListener),
        }
    ).extend(ble_device_base.BLE_DEVICE_SCHEMA),
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await ble_device_base.register_ble_device(var, config)
