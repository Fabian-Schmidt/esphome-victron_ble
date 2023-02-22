from esphome.components import text_sensor
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import (
    CONF_TYPE,
)
from .. import victron_ble_ns, CONF_VICTRON_BLE_ID, VictronBle

DEPENDENCIES = ["victron_ble"]
CODEOWNERS = ["@Fabian-Schmidt"]

VictronTextSensor = victron_ble_ns.class_(
    "VictronTextSensor", text_sensor.TextSensor, cg.Component)

VICTRON_TEXT_SENSOR_TYPE = victron_ble_ns.namespace(
    "VICTRON_TEXT_SENSOR_TYPE")

CONF_SUPPORTED_TYPE = {
    "ALARM_REASON":  {
        CONF_TYPE: VICTRON_TEXT_SENSOR_TYPE.ALARM_REASON,
    },
    "CHARGER_ERROR":  {
        CONF_TYPE: VICTRON_TEXT_SENSOR_TYPE.CHARGER_ERROR,
    },
    "DEVICE_STATE":  {
        CONF_TYPE: VICTRON_TEXT_SENSOR_TYPE.DEVICE_STATE,
    },

    "DCDC_CONVERTER_OFF_REASON":  {
        CONF_TYPE: VICTRON_TEXT_SENSOR_TYPE.DCDC_CONVERTER_OFF_REASON,
    },

    "SMART_BATTERY_PROTECT_ERROR_CODE":  {
        CONF_TYPE: VICTRON_TEXT_SENSOR_TYPE.SMART_BATTERY_PROTECT_ERROR_CODE,
    },
    "SMART_BATTERY_PROTECT_WARNING_REASON":  {
        CONF_TYPE: VICTRON_TEXT_SENSOR_TYPE.SMART_BATTERY_PROTECT_WARNING_REASON,
    },
    "SMART_BATTERY_PROTECT_OFF_REASON":  {
        CONF_TYPE: VICTRON_TEXT_SENSOR_TYPE.SMART_BATTERY_PROTECT_OFF_REASON,
    },

    "MULTI_RS_ACTIVE_AC_IN":  {
        CONF_TYPE: VICTRON_TEXT_SENSOR_TYPE.MULTI_RS_ACTIVE_AC_IN,
    },

    "VE_BUS_ACTIVE_AC_IN":  {
        CONF_TYPE: VICTRON_TEXT_SENSOR_TYPE.VE_BUS_ACTIVE_AC_IN,
    },
    "VE_BUS_ALARM":  {
        CONF_TYPE: VICTRON_TEXT_SENSOR_TYPE.VE_BUS_ALARM,
    },
}


CONFIG_SCHEMA = (
    text_sensor.text_sensor_schema(VictronTextSensor)
    .extend({
        cv.GenerateID(CONF_VICTRON_BLE_ID): cv.use_id(VictronBle),
        cv.Required(CONF_TYPE): cv.enum(CONF_SUPPORTED_TYPE, upper=True),
    }).extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    var = await text_sensor.new_text_sensor(config)
    await cg.register_component(var, config)
    await cg.register_parented(var, config[CONF_VICTRON_BLE_ID])

    cg.add(var.set_type(CONF_SUPPORTED_TYPE[config[CONF_TYPE]][CONF_TYPE]))
