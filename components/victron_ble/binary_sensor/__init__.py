from esphome.components import binary_sensor
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import (
    CONF_TYPE,
)
from .. import victron_ble_ns, CONF_VICTRON_BLE_ID, VictronBle

DEPENDENCIES = ["victron_ble"]
CODEOWNERS = ["@Fabian-Schmidt"]

VictronBinarySensor = victron_ble_ns.class_(
    "VictronBinarySensor", binary_sensor.BinarySensor, cg.Component)

VICTRON_BINARY_SENSOR_TYPE = victron_ble_ns.namespace(
    "VICTRON_BINARY_SENSOR_TYPE")

CONF_SUPPORTED_TYPE = {
    "BATTERY_MONITOR_ALARM":  {
        CONF_TYPE: VICTRON_BINARY_SENSOR_TYPE.BATTERY_MONITOR_ALARM,
    },

    "SOLAR_CHARGER_DEVICE_STATE_OFF":  {
        CONF_TYPE: VICTRON_BINARY_SENSOR_TYPE.SOLAR_CHARGER_DEVICE_STATE_OFF,
    },
    "SOLAR_CHARGER_DEVICE_STATE_FAULT":  {
        CONF_TYPE: VICTRON_BINARY_SENSOR_TYPE.SOLAR_CHARGER_DEVICE_STATE_FAULT,
    },
    "SOLAR_CHARGER_DEVICE_STATE_BULK":  {
        CONF_TYPE: VICTRON_BINARY_SENSOR_TYPE.SOLAR_CHARGER_DEVICE_STATE_BULK,
    },
    "SOLAR_CHARGER_DEVICE_STATE_ABSORPTION":  {
        CONF_TYPE: VICTRON_BINARY_SENSOR_TYPE.SOLAR_CHARGER_DEVICE_STATE_ABSORPTION,
    },
    "SOLAR_CHARGER_DEVICE_STATE_FLOAT":  {
        CONF_TYPE: VICTRON_BINARY_SENSOR_TYPE.SOLAR_CHARGER_DEVICE_STATE_FLOAT,
    },
    "SOLAR_CHARGER_DEVICE_STATE_EQUALIZE_MANUAL":  {
        CONF_TYPE: VICTRON_BINARY_SENSOR_TYPE.SOLAR_CHARGER_DEVICE_STATE_EQUALIZE_MANUAL,
    },
    "SOLAR_CHARGER_DEVICE_STATE_STARTING_UP":  {
        CONF_TYPE: VICTRON_BINARY_SENSOR_TYPE.SOLAR_CHARGER_DEVICE_STATE_STARTING_UP,
    },
    "SOLAR_CHARGER_DEVICE_STATE_AUTO_EQUALIZE":  {
        CONF_TYPE: VICTRON_BINARY_SENSOR_TYPE.SOLAR_CHARGER_DEVICE_STATE_AUTO_EQUALIZE,
    },
    "SOLAR_CHARGER_DEVICE_STATE_EXTERNAL_CONTROL":  {
        CONF_TYPE: VICTRON_BINARY_SENSOR_TYPE.SOLAR_CHARGER_DEVICE_STATE_EXTERNAL_CONTROL,
    },
    "SOLAR_CHARGER_CHARGER_ERROR":  {
        CONF_TYPE: VICTRON_BINARY_SENSOR_TYPE.SOLAR_CHARGER_CHARGER_ERROR,
    },
}


CONFIG_SCHEMA = (
    binary_sensor.binary_sensor_schema(VictronBinarySensor)
    .extend({
        cv.GenerateID(CONF_VICTRON_BLE_ID): cv.use_id(VictronBle),
        cv.Required(CONF_TYPE): cv.enum(CONF_SUPPORTED_TYPE, upper=True),
    }).extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    var = await binary_sensor.new_binary_sensor(config)
    await cg.register_component(var, config)
    await cg.register_parented(var, config[CONF_VICTRON_BLE_ID])

    cg.add(var.set_type(CONF_SUPPORTED_TYPE[config[CONF_TYPE]][CONF_TYPE]))