import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import ble_client, sensor
from esphome.const import (
    CONF_ID,
    CONF_VOLTAGE,
    CONF_POWER,
    CONF_CURRENT,
    UNIT_AMPERE,
    UNIT_MINUTE,
    UNIT_PERCENT,
    UNIT_VOLT,
    UNIT_WATT,
    ICON_BATTERY,
    DEVICE_CLASS_BATTERY,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_DURATION,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_VOLTAGE,
    STATE_CLASS_MEASUREMENT,
)
import esphome.core as CORE

DEPENDENCIES = ["esp32_ble_tracker"]

CONF_STATE_OF_CHARGE = "state_of_charge"
CONF_NOTIFY = "notify"
CONF_AH = "ah"
CONF_STARTER_BATTERY_VOLTAGE = "starter_battery_voltage"
CONF_VAL2 = "val2"
CONF_VAL3 = "val3"
CONF_VAL4 = "val4"
CONF_REMAINING_TIME = "remaining_time"
UNIT_AMPERE_HOURS = "Ah"

victron_ble_ns = cg.esphome_ns.namespace("victron_ble")
VictronBle = victron_ble_ns.class_(
    "VictronBle",  cg.PollingComponent, ble_client.BLEClientNode
)


def final_validate_maximum_number_of_sensors_if_notify(
    name: str,
    *,
    configuration: str,
    max_number_of_sensors: int,
):
    def inherit_property(config):
        # Check if `configuration` ist set
        if config[configuration]: 
            # Count number of sensors
            count_sensors = 0
            for config_item in config:
                if isinstance(config[config_item], dict):
                    if isinstance(config[config_item][CONF_ID], CORE.ID):
                        if config[config_item][CONF_ID].type == sensor.Sensor:
                            count_sensors += 1
            
            if count_sensors > max_number_of_sensors:
                raise cv.Invalid(f"{name} supports maximum of {max_number_of_sensors} sensors with `{configuration}` active. Currently {count_sensors} sensors are configured.")

        return config

    return inherit_property


CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(VictronBle),
            cv.Optional(CONF_NOTIFY, default=True): cv.boolean,
            cv.Optional(CONF_STATE_OF_CHARGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_PERCENT,
                icon=ICON_BATTERY,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_BATTERY,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT,
                icon=ICON_BATTERY,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_VOLTAGE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_POWER): sensor.sensor_schema(
                unit_of_measurement=UNIT_WATT,
                icon=ICON_BATTERY,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_POWER,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_CURRENT): sensor.sensor_schema(
                unit_of_measurement=UNIT_AMPERE,
                icon=ICON_BATTERY,
                accuracy_decimals=3,
                device_class=DEVICE_CLASS_CURRENT,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_AH): sensor.sensor_schema(
                unit_of_measurement=UNIT_AMPERE_HOURS,
                icon=ICON_BATTERY,
                accuracy_decimals=1,
                # device_class=???,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_STARTER_BATTERY_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT,
                icon=ICON_BATTERY,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_VOLTAGE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_VAL2): sensor.sensor_schema(
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_VAL3): sensor.sensor_schema(
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_VAL4): sensor.sensor_schema(
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_REMAINING_TIME): sensor.sensor_schema(
                unit_of_measurement=UNIT_MINUTE,
                icon=ICON_BATTERY,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_DURATION,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
        }
    )
    .extend(cv.polling_component_schema("2min"))
    .extend(ble_client.BLE_CLIENT_SCHEMA),
)
FINAL_VALIDATE_SCHEMA = cv.All(
    final_validate_maximum_number_of_sensors_if_notify(
        "victron_ble", configuration=CONF_NOTIFY, max_number_of_sensors=7)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await ble_client.register_ble_node(var, config)
    cg.add(var.set_notify(config[CONF_NOTIFY]))

    if CONF_STATE_OF_CHARGE in config:
        sens = await sensor.new_sensor(config[CONF_STATE_OF_CHARGE])
        cg.add(var.set_state_of_charge(sens))
    if CONF_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_VOLTAGE])
        cg.add(var.set_voltage(sens))
    if CONF_POWER in config:
        sens = await sensor.new_sensor(config[CONF_POWER])
        cg.add(var.set_power(sens))
    if CONF_CURRENT in config:
        sens = await sensor.new_sensor(config[CONF_CURRENT])
        cg.add(var.set_current(sens))
    if CONF_AH in config:
        sens = await sensor.new_sensor(config[CONF_AH])
        cg.add(var.set_ah(sens))
    if CONF_STARTER_BATTERY_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_STARTER_BATTERY_VOLTAGE])
        cg.add(var.set_starter_battery_voltage(sens))
    if CONF_VAL2 in config:
        sens = await sensor.new_sensor(config[CONF_VAL2])
        cg.add(var.set_val2(sens))
    if CONF_VAL3 in config:
        sens = await sensor.new_sensor(config[CONF_VAL3])
        cg.add(var.set_val3(sens))
    if CONF_VAL4 in config:
        sens = await sensor.new_sensor(config[CONF_VAL4])
        cg.add(var.set_val4(sens))
    if CONF_REMAINING_TIME in config:
        sens = await sensor.new_sensor(config[CONF_REMAINING_TIME])
        cg.add(var.set_remaining_time(sens))
