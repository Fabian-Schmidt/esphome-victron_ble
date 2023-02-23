# ESPHome victron_ble component

**This repository is in no way approved by or affiliated with the official Victron Energy repository.**

**I am not responsible for any problems or damages with your devices or this code**

This [ESPHome](https://esphome.io) component supports both official Victron Bluetooth BLE Protocols:

- (Recommended) [Bluetooth accouncement protocol](https://community.victronenergy.com/questions/187303/victron-bluetooth-advertising-protocol.html) - Component `victron_ble`.
  - Supported Devices:
    - Smart Shunt
    - Smart Solar
    - Inverter
    - DC/DC convert
    - SmartLithium
      - Missing definition for `bms_flags`, `error` and `balancer_status`
    - Inverter RS
    - Smart Batery Protect
      - Missing definition for `output_state`
    - (Lynx Smart) BMS
      - Missing definition for `error`, `io_status` and `warnings_alarms`
    - Multi RS
    - VE.Bus
      - Missing definition for `ve_bus_error`
    - DC Energy Meter
      - Missing definition for `bmv_monitor_mode`

- [Bluetooth connection protocol](https://community.victronenergy.com/questions/93919/victron-bluetooth-ble-protocol-publication.html) - Component `victron_ble_connect`.
  - This solution is using the [PR#4258 of ESPHome](https://github.com/esphome/esphome/pull/4258).
  - Supported Devices:
    - Smart Shunt

## `victron_ble` component (Recommended)

### Steps

1. Update your Victron device.
2. Enable `Instant readout via Bluetooth` via Victron app.
3. Get Mac address and Encryption key from Victron Software.

### Fetching Mac address and Encryption key

See <https://github.com/keshavdv/victron-ble> for details.

### Example ESPHome configuration for `victron_ble`

See [victron_ble.yaml](/victron_ble.yaml) for a full example.

```yaml
esphome:
  name: "victron-ble"

external_components:
  - source: github://Fabian-Schmidt/esphome-victron_ble

esp32:
  board: mhetesp32devkit

logger:
  level: DEBUG

esp32_ble_tracker:

victron_ble:
  - id: MySmartShunt
    mac_address: "MY SMART SHUNT MAC"    
    bindkey: "MY AES ENCRYPTION KEY"
  - id: MySmartSolar
    mac_address: "MY SMART SOLAR MAC"
    bindkey: "MY AES ENCRYPTION KEY"

sensor:
  # MySmartShunt
  - platform: victron_ble
    victron_ble_id: MySmartShunt
    name: "Time remaining"
    type: BATTERY_MONITOR_TIME_TO_GO
  - platform: victron_ble
    victron_ble_id: MySmartShunt
    name: "Battery voltage"
    type: BATTERY_VOLTAGE
  - platform: victron_ble
    victron_ble_id: MySmartShunt
    name: "Starter Battery"
    # BATTERY_MONITOR_AUX_VOLTAGE or BATTERY_MONITOR_MID_VOLTAGE or BATTERY_MONITOR_TEMPERATURE.
    # Depending on configuration of SmartShunt
    type: BATTERY_MONITOR_AUX_VOLTAGE
  - platform: victron_ble
    victron_ble_id: MySmartShunt
    name: "Current"
    type: BATTERY_MONITOR_BATTERY_CURRENT
  - platform: victron_ble
    victron_ble_id: MySmartShunt
    name: "Consumed Ah"
    type: BATTERY_MONITOR_CONSUMED_AH
  - platform: victron_ble
    victron_ble_id: MySmartShunt
    name: "State of charge"
    type: BATTERY_MONITOR_STATE_OF_CHARGE

  # MySmartSolar
  - platform: victron_ble
    victron_ble_id: MySmartSolar
    name: "Battery Voltage"
    type: BATTERY_VOLTAGE
  - platform: victron_ble
    victron_ble_id: MySmartSolar
    name: "Battery Current"
    type: SOLAR_CHARGER_BATTERY_CURRENT
  - platform: victron_ble
    victron_ble_id: MySmartSolar
    name: "Yield Today"
    type: SOLAR_CHARGER_YIELD_TODAY
  - platform: victron_ble
    victron_ble_id: MySmartSolar
    name: "PV Power"
    type: SOLAR_CHARGER_PV_POWER
  - platform: victron_ble
    victron_ble_id: MySmartSolar
    name: "Load Current"
    type: SOLAR_CHARGER_LOAD_CURRENT

binary_sensor:
  - platform: victron_ble
    victron_ble_id: MySmartShunt
    name: "Battery has Alarm"
    type: ALARM

  - platform: victron_ble
    victron_ble_id: MySmartSolar
    name: "MPPT is in Fault state"
    type: DEVICE_STATE_FAULT
  - platform: victron_ble
    victron_ble_id: MySmartSolar
    name: "MPPT has Error"
    type: CHARGER_ERROR

text_sensor:
  - platform: victron_ble
    victron_ble_id: MySmartShunt
    name: "Battery Alarm reason"
    type: ALARM_REASON

  - platform: victron_ble
    victron_ble_id: MySmartSolar
    name: "MPPT state"
    type: DEVICE_STATE
  - platform: victron_ble
    victron_ble_id: MySmartSolar
    name: "MPPT Error reason"
    type: CHARGER_ERROR
```

### Sensor

The following `type` are supported by the `sensor` component:

|                   | Solar charger | Battery monitor | Inverter | DC/DC converter | SmartLithium | Inverter RS | Smart Battery Protect | (Lynx Smart) BMS | Multi RS | VE.Bus | DC Energy Meter |
| ----------------- | ------------- | --------------- | -------- | --------------- | ------------ | ----------- | --------------------- | ---------------- | -------- | ------ | --------------- |
| `BATTERY_VOLTAGE` | X             | X               | X        |                 | X            | X           |                       | X                | X        | X      | X               |
| `CHARGER_ERROR`   | X             |                 |          | X               |              | X           |                       |                  | X        |        |                 |
| `DEVICE_STATE`    | X             |                 | X        | X               |              | X           | X                     |                  | X        | X      |                 |
| ``                |               |                 |          |                 |              |             |                       |                  |          |        |                 |
| ``                |               |                 |          |                 |              |             |                       |                  |          |        |                 |
| ``                |               |                 |          |                 |              |             |                       |                  |          |        |                 |

- `BATTERY_MONITOR_TIME_TO_GO`
- `BATTERY_MONITOR_ALARM_REASON`
- `BATTERY_MONITOR_AUX_VOLTAGE`
- `BATTERY_MONITOR_MID_VOLTAGE`
- `BATTERY_MONITOR_TEMPERATURE`
- `BATTERY_MONITOR_BATTERY_CURRENT`
- `BATTERY_MONITOR_CONSUMED_AH`
- `BATTERY_MONITOR_STATE_OF_CHARGE`
- `SOLAR_CHARGER_BATTERY_CURRENT`
- `SOLAR_CHARGER_YIELD_TODAY`
- `SOLAR_CHARGER_PV_POWER`
- `SOLAR_CHARGER_LOAD_CURRENT`
- `INVERTER_ALARM_REASON`
- `INVERTER_AC_APPARENT_POWER`
- `INVERTER_AC_VOLTAGE`
- `INVERTER_AC_CURRENT`
- `DCDC_CONVERTER_INPUT_VOLTAGE`
- `DCDC_CONVERTER_OUTPUT_VOLTAGE`
- `DCDC_CONVERTER_OFF_REASON`
- `SMART_LITHIUM_BMS_FLAGS`
- `SMART_LITHIUM_ERROR`
- `SMART_LITHIUM_CELL1`
- `SMART_LITHIUM_CELL2`
- `SMART_LITHIUM_CELL3`
- `SMART_LITHIUM_CELL4`
- `SMART_LITHIUM_CELL5`
- `SMART_LITHIUM_CELL6`
- `SMART_LITHIUM_CELL7`
- `SMART_LITHIUM_CELL8`
- `SMART_LITHIUM_BALANCER_STATUS`
- `SMART_LITHIUM_BATTERY_TEMPERATURE`
- `INVERTER_RS_BATTERY_CURRENT`
- `INVERTER_RS_PV_POWER`
- `INVERTER_RS_YIELD_TODAY`
- `INVERTER_RS_AC_OUT_POWER`
- `SMART_BATTERY_PROTECT_OUTPUT_STATE`
- `SMART_BATTERY_PROTECT_ERROR_CODE`
- `SMART_BATTERY_PROTECT_ALARM_REASON`
- `SMART_BATTERY_PROTECT_WARNING_REASON`
- `SMART_BATTERY_PROTECT_INPUT_VOLTAGE`
- `SMART_BATTERY_PROTECT_OUTPUT_VOLTAGE`
- `SMART_BATTERY_PROTECT_OFF_REASON`
- `LYNX_SMART_BMS_ERROR`
- `LYNX_SMART_BMS_TIME_TO_GO`
- `LYNX_SMART_BMS_BATTERY_CURRENT`
- `LYNX_SMART_BMS_IO_STATUS`
- `LYNX_SMART_BMS_WARNINGS_ALARMS`
- `LYNX_SMART_BMS_STATE_OF_CHARGE`
- `LYNX_SMART_BMS_CONSUMED_AH`
- `LYNX_SMART_BMS_TEMPERATURE`
- `MULTI_RS_BATTERY_CURRENT`
- `MULTI_RS_ACTIVE_AC_IN`
- `MULTI_RS_ACTIVE_AC_IN_POWER`
- `MULTI_RS_ACTIVE_AC_OUT_POWER`
- `MULTI_RS_PV_POWER`
- `MULTI_RS_YIELD_TODAY`
- `VE_BUS_ERROR`
- `VE_BUS_BATTERY_CURRENT`
- `VE_BUS_ACTIVE_AC_IN`
- `VE_BUS_ACTIVE_AC_IN_POWER`
- `VE_BUS_ACTIVE_AC_OUT_POWER`
- `VE_BUS_ALARM`
- `VE_BUS_BATTERY_TEMPERATURE`
- `VE_BUS_STATE_OF_CHARGE`
- `DC_ENERGY_METER_BMV_MONITOR_MODE`
- `DC_ENERGY_METER_ALARM_REASON`
- `DC_ENERGY_METER_AUX_VOLTAGE`
- `DC_ENERGY_METER_TEMPERATURE`
- `DC_ENERGY_METER_BATTERY_CURRENT`

### Binary Sensor

The following `type` are supported by the `binary_sensor` component:

|                                 | Solar charger | Battery monitor | Inverter | DC/DC converter | SmartLithium | Inverter RS | Smart Battery Protect | (Lynx Smart) BMS | Multi RS | VE.Bus | DC Energy Meter |
| ------------------------------- | ------------- | --------------- | -------- | --------------- | ------------ | ----------- | --------------------- | ---------------- | -------- | ------ | --------------- |
| `ALARM`                         |               | X               | X        |                 |              |             | X                     |                  |          |        | X               |
| `CHARGER_ERROR`                 | X             |                 |          | X               |              | X           |                       |                  | X        |        |                 |
| `DEVICE_STATE_OFF`              | X             |                 | X        | X               |              | X           | X                     |                  | X        | X      |                 |
| `DEVICE_STATE_FAULT`            | X             |                 | X        | X               |              | X           | X                     |                  | X        | X      |                 |
| `DEVICE_STATE_BULK`             | X             |                 |          |                 |              | ?           |                       |                  | ?        | ?      |                 |
| `DEVICE_STATE_ABSORPTION`       | X             |                 |          |                 |              | ?           |                       |                  | ?        | ?      |                 |
| `DEVICE_STATE_FLOAT`            | X             |                 |          |                 |              | ?           |                       |                  | ?        | ?      |                 |
| `DEVICE_STATE_EQUALIZE_MANUAL`  | X             |                 |          |                 |              | ?           |                       |                  | ?        | ?      |                 |
| `DEVICE_STATE_STARTING_UP`      | X             |                 |          |                 |              | ?           |                       |                  | ?        | ?      |                 |
| `DEVICE_STATE_AUTO_EQUALIZE`    | X             |                 |          |                 |              | ?           |                       |                  | ?        | ?      |                 |
| `DEVICE_STATE_EXTERNAL_CONTROL` | X             |                 |          |                 |              | ?           |                       |                  | ?        | ?      |                 |

### Text Sensor

The following `type` are supported by the `text_sensor` component:

|                 | Solar charger | Battery monitor | Inverter | DC/DC converter | SmartLithium | Inverter RS | Smart Battery Protect | (Lynx Smart) BMS | Multi RS | VE.Bus | DC Energy Meter |
| --------------- | ------------- | --------------- | -------- | --------------- | ------------ | ----------- | --------------------- | ---------------- | -------- | ------ | --------------- |
| `ALARM_REASON`  |               | X               | X        |                 |              |             | X                     |                  |          |        | X               |
| `CHARGER_ERROR` | X             |                 |          | X               |              | X           |                       |                  | X        |        |                 |
| `DEVICE_STATE`  | X             |                 | X        | X               |              | X           | X                     |                  | X        | X      |                 |

Device specific:

- `DCDC_CONVERTER_OFF_REASON`
- `SMART_BATTERY_PROTECT_ERROR_CODE`
- `SMART_BATTERY_PROTECT_WARNING_REASON`
- `SMART_BATTERY_PROTECT_OFF_REASON`
- `MULTI_RS_ACTIVE_AC_IN`
- `VE_BUS_ACTIVE_AC_IN`
- `VE_BUS_ALARM`

### Trigger

The following ESP Home actions exists for advanced users:

- `on_battery_monitor_message`
- `on_solar_charger_message`
- `on_inverter_message`
- `on_dcdc_converter_message`
- `on_smart_lithium_message`
- `on_inverter_rs_message`
- `on_smart_battery_protect_message`
- `on_lynx_smart_bms_message`
- `on_multi_rs_message`
- `on_ve_bus_message`
- `on_dc_energy_meter_message`

## `victron_ble_connect` component

### Steps

1. Use a SmartShunt, other devices don't support this yet.
2. Use VictronConnect v5.42 or newer.
3. Update the SmartShunt to version v2.31 or later.
4. Connect to the SmartShunt using VictronConnect, and enable this protocol (screenshot below)
5. Power cycle the SmartShunt
6. Use `victron_scanner` to find mac address of your SmartShunt.

![The setting to enable this "third party implementation"-protocol](/img/VictronAppEnableGATT.png)

### Find the MAC Adress of your Smart Shunt

Use the Victron scanner component to find the MAC address of your Victron Smart Shunt.

```yaml
esphome:
  name: "victron-scanner"

external_components:
  # https://github.com/esphome/esphome/pull/4258
  - source: github://pr#4258
    components: [ble_client, esp32_ble, esp32_ble_client, esp32_ble_tracker]
    #refresh: always
  - source: github://Fabian-Schmidt/esphome-victron_ble

esp32:
  board: mhetesp32devkit

logger:
  level: INFO

esp32_ble_tracker:

victron_scanner:
```

Check the console of ESP Home to find the message like this:

```txt
[I][victron_scanner:044]: FOUND SMART SHUNT 500A/50mV 'My SmartShunt' at AB:CD:EF:01:02:03
```

### Example ESPHome configuration for `victron_ble_connect`

```yaml
esphome:
  name: "victron-ble"

external_components:
  # https://github.com/esphome/esphome/pull/4258
  - source: github://pr#4258
    components: [ble_client, esp32_ble, esp32_ble_client, esp32_ble_tracker]
    refresh: always
  - source: github://Fabian-Schmidt/esphome-victron_ble


esp32_ble:
  io_capability: keyboard_only

ble_client:
- mac_address: <MY VICTORN SMART SHUNT MAC ADDRESS>
  id: victron_smart_shunt_ble_client_id
  on_passkey_request:
    then:
      - ble_client.passkey_reply:
          id: victron_smart_shunt_ble_client_id
          passkey: !secret ble_passkey

sensor:
  - platform: victron_ble_connect
    ble_client_id: victron_smart_shunt_ble_client_id
    state_of_charge: 
      name: "State of Charge"
    voltage: 
      name: "Voltage"
    power: 
      name: "Power Consumption"
    current: 
      name: "Current"
    ah: 
      name: "Consumed Ah"
    starter_battery_voltage: 
      name: "Starter Battery Voltage"
    remaining_time: 
      name: "Remaining Time"
```

See [victron_ble_all.yaml](/victron_ble_all.yaml) for a full example.
Assumption is you are having a `secret.yaml` in the same folder.
