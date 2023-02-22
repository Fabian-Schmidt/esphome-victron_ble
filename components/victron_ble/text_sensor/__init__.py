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
    "BATTERY_MONITOR_ALARM_REASON":  {
        CONF_TYPE: VICTRON_TEXT_SENSOR_TYPE.BATTERY_MONITOR_ALARM_REASON,
    },

    "SOLAR_CHARGER_DEVICE_STATE":  {
        CONF_TYPE: VICTRON_TEXT_SENSOR_TYPE.SOLAR_CHARGER_DEVICE_STATE,
    },
    "SOLAR_CHARGER_CHARGER_ERROR":  {
        CONF_TYPE: VICTRON_TEXT_SENSOR_TYPE.SOLAR_CHARGER_CHARGER_ERROR,
    },

    "INVERTER_DEVICE_STATE":  {
        CONF_TYPE: VICTRON_TEXT_SENSOR_TYPE.INVERTER_DEVICE_STATE,
    },
    "INVERTER_ALARM_REASON":  {
        CONF_TYPE: VICTRON_TEXT_SENSOR_TYPE.INVERTER_ALARM_REASON,
    },

    "DCDC_CONVERTER_DEVICE_STATE":  {
        CONF_TYPE: VICTRON_TEXT_SENSOR_TYPE.DCDC_CONVERTER_DEVICE_STATE,
    },
    "DCDC_CONVERTER_CHARGER_ERROR":  {
        CONF_TYPE: VICTRON_TEXT_SENSOR_TYPE.DCDC_CONVERTER_CHARGER_ERROR,
    },
    "DCDC_CONVERTER_OFF_REASON":  {
        CONF_TYPE: VICTRON_TEXT_SENSOR_TYPE.DCDC_CONVERTER_OFF_REASON,
    },

    "MULTI_RS_DEVICE_STATE":  {
        CONF_TYPE: VICTRON_TEXT_SENSOR_TYPE.MULTI_RS_DEVICE_STATE,
    },
    "MULTI_RS_CHARGER_ERROR":  {
        CONF_TYPE: VICTRON_TEXT_SENSOR_TYPE.MULTI_RS_CHARGER_ERROR,
    },
    "MULTI_RS_ACTIVE_AC_IN":  {
        CONF_TYPE: VICTRON_TEXT_SENSOR_TYPE.MULTI_RS_ACTIVE_AC_IN,
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
