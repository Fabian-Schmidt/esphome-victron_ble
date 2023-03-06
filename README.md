# ESPHome victron_ble component

**This repository is in no way approved by or affiliated with the official Victron Energy repository.**

**I am not responsible for any problems or damages with your devices or this code**

This [ESPHome](https://esphome.io) component supports both official Victron Bluetooth BLE Protocols:

- (Recommended) [Bluetooth advertising protocol](https://community.victronenergy.com/questions/187303/victron-bluetooth-advertising-protocol.html) - Component `victron_ble`.
  - Supported Devices:
    - Smart Shunt
    - Smart Solar
    - Inverter
    - DC/DC converter
    - SmartLithium
      - Missing definition for `bms_flags`, `error` and `balancer_status`.
    - Inverter RS
    - GX-Device
      - Not suported. Awaiting final documentation.
    - AC Charger
      - Not suported. Awaiting final documentation.
    - Smart Batery Protect
      - Missing definition for `output_state`.
    - (Lynx Smart) BMS
      - Missing definition for `error`, `io_status` and `warnings_alarms`.
    - Multi RS
    - VE.Bus
      - Missing definition for `ve_bus_error`.
    - DC Energy Meter
      - Missing definition for `bmv_monitor_mode`.

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
    type: TIME_TO_GO
  - platform: victron_ble
    victron_ble_id: MySmartShunt
    name: "Battery voltage"
    type: BATTERY_VOLTAGE
  - platform: victron_ble
    victron_ble_id: MySmartShunt
    name: "Starter Battery"
    # AUX_VOLTAGE or MID_VOLTAGE or TEMPERATURE.
    # Depending on configuration of SmartShunt.
    type: AUX_VOLTAGE
  - platform: victron_ble
    victron_ble_id: MySmartShunt
    name: "Current"
    type: BATTERY_CURRENT
  - platform: victron_ble
    victron_ble_id: MySmartShunt
    name: "Consumed Ah"
    type: CONSUMED_AH
  - platform: victron_ble
    victron_ble_id: MySmartShunt
    name: "State of charge"
    type: STATE_OF_CHARGE

  # MySmartSolar
  - platform: victron_ble
    victron_ble_id: MySmartSolar
    name: "Battery Voltage"
    type: BATTERY_VOLTAGE
  - platform: victron_ble
    victron_ble_id: MySmartSolar
    name: "Battery Current"
    type: BATTERY_CURRENT
  - platform: victron_ble
    victron_ble_id: MySmartSolar
    name: "Yield Today"
    type: YIELD_TODAY
  - platform: victron_ble
    victron_ble_id: MySmartSolar
    name: "PV Power"
    type: PV_POWER
  - platform: victron_ble
    victron_ble_id: MySmartSolar
    name: "Load Current"
    type: LOAD_CURRENT

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

### `victron_ble` component configuration

The following settings exist:

- `mac_address` - The MAC address of your Victron device.
- `bindkey` - The AES encryption key of your Victron device.
- `submit_sensor_data_asap` boolean to configure if values are submitted as soon as they are available (true). Or only with the `update_interval` (false - default).
- [Automation Triggers](https://esphome.io/guides/automations.html):
  - `on_message` - When any message is recieved.
  - `on_battery_monitor_message` - When a battery monitor message is recieved.
  - `on_solar_charger_message` - When a solar charger message is recieved.
  - `on_inverter_message` - When a inverter message is recieved.
  - `on_dcdc_converter_message` - When a DC/DC converter message is recieved.
  - `on_smart_lithium_message` - When a smart Lithium message is recieved.
  - `on_inverter_rs_message` - When a Inverter RS message is recieved.
  - `on_smart_battery_protect_message` - When a Smart Battery Protect message is recieved.
  - `on_lynx_smart_bms_message` - When a Lynx (Smart BMS) message is recieved.
  - `on_multi_rs_message` - When a Multi RS message is recieved.
  - `on_ve_bus_message` - When a VE.Bus message is recieved.
  - `on_dc_energy_meter_message` - When a DC Energy meter message is recieved.

### Sensor

The following `type` are supported by the `sensor` component:

|                      | Unit | Solar charger | Battery monitor | Inverter | DC/DC converter | SmartLithium | Inverter RS | Smart Battery Protect | (Lynx Smart) BMS | Multi RS | VE.Bus | DC Energy Meter |
| -------------------- | ---- | ------------- | --------------- | -------- | --------------- | ------------ | ----------- | --------------------- | ---------------- | -------- | ------ | --------------- |
| `ACTIVE_AC_IN`       |      |               |                 |          |                 |              |             |                       |                  | X        | X      |                 |
| `ACTIVE_AC_IN_POWER` | W    |               |                 |          |                 |              |             |                       |                  | X        | X      |                 |
| `AC_APPARENT_POWER`  | VA   |               |                 | X        |                 |              |             |                       |                  |          |        |                 |
| `AC_CURRENT`         | A    |               |                 | X        |                 |              |             |                       |                  |          |        |                 |
| `AC_OUT_POWER`       | W    |               |                 |          |                 |              | X           |                       |                  | X        | X      |                 |
| `AC_VOLTAGE`         | V    |               |                 | X        |                 |              |             |                       |                  |          |        |                 |
| `ALARM_REASON`       |      |               | X               | X        |                 |              |             | X                     |                  |          |        | X               |
| `AUX_VOLTAGE`        | V    |               | X(1)            |          |                 |              |             |                       |                  |          |        | X(1)            |
| `BATTERY_CURRENT`    | A    | X             | X               |          |                 |              | X           |                       | X                | X        | X      | X               |
| `BATTERY_VOLTAGE`    | V    | X             | X               | X        |                 | X            | X           |                       | X                | X        | X      | X               |
| `BATTERY_POWER`      | W    | X             | X               |          |                 |              | X           |                       | X                | X        | X      | X               |
| `CHARGER_ERROR`      |      | X             |                 |          | X               |              | X           |                       |                  | X        |        |                 |
| `CONSUMED_AH`        | Ah   |               | X               |          |                 |              |             |                       | X                |          |        |                 |
| `DEVICE_STATE`       |      | X             |                 | X        | X               |              | X           | X                     |                  | X        | X      |                 |
| `ERROR`              |      |               |                 |          |                 | X            |             | X                     | X                |          | X      |                 |
| `INPUT_VOLTAGE`      | V    |               |                 |          | X               |              |             | X                     |                  |          |        |                 |
| `LOAD_CURRENT`       | A    | X             |                 |          |                 |              |             |                       |                  |          |        |                 |
| `OFF_REASON`         |      |               |                 |          | X               |              |             | X                     |                  |          |        |                 |
| `OUTPUT_VOLTAGE`     | V    |               |                 |          | X               |              |             | X                     |                  |          |        |                 |
| `MID_VOLTAGE`        | V    |               | X(1)            |          |                 |              |             |                       |                  |          |        |                 |
| `PV_POWER`           | W    | X             |                 |          |                 |              | X           |                       |                  | X        |        |                 |
| `STATE_OF_CHARGE`    | %    |               | X               |          |                 |              |             |                       | X                |          | X      |                 |
| `TEMPERATURE`        | °C   |               | X(1)            |          |                 | X            |             |                       | X                |          | X      | X(1)            |
| `TIME_TO_GO`         | min  |               | X               |          |                 |              |             |                       | X                |          |        |                 |
| `YIELD_TODAY`        | kWh  | X             |                 |          |                 |              | X           |                       |                  | X        |        |                 |
| `BALANCER_STATUS`    |      |               |                 |          |                 | X            |             |                       |                  |          |        |                 |
| `BMS_FLAGS`          |      |               |                 |          |                 | X            |             |                       |                  |          |        |                 |
| `CELL1`              | V    |               |                 |          |                 | X            |             |                       |                  |          |        |                 |
| `CELL2`              | V    |               |                 |          |                 | X            |             |                       |                  |          |        |                 |
| `CELL3`              | V    |               |                 |          |                 | X            |             |                       |                  |          |        |                 |
| `CELL4`              | V    |               |                 |          |                 | X            |             |                       |                  |          |        |                 |
| `CELL5`              | V    |               |                 |          |                 | X            |             |                       |                  |          |        |                 |
| `CELL6`              | V    |               |                 |          |                 | X            |             |                       |                  |          |        |                 |
| `CELL7`              | V    |               |                 |          |                 | X            |             |                       |                  |          |        |                 |
| `CELL8`              | V    |               |                 |          |                 | X            |             |                       |                  |          |        |                 |
| `OUTPUT_STATE`       |      |               |                 |          |                 |              |             | X                     |                  |          |        |                 |
| `WARNING_REASON`     |      |               |                 |          |                 |              |             | X                     |                  |          |        |                 |
| `IO_STATUS`          |      |               |                 |          |                 |              |             |                       | X                |          |        |                 |
| `WARNINGS_ALARMS`    |      |               |                 |          |                 |              |             |                       | X                |          |        |                 |
| `ALARM`              |      |               |                 |          |                 |              |             |                       |                  |          | X      |                 |
| `BMV_MONITOR_MODE`   |      |               |                 |          |                 |              |             |                       |                  |          |        | X               |

(1) - Available if device aux port is configured.

### Binary Sensor

The following `type` are supported by the `binary_sensor` component:

|                                    | Solar charger | Battery monitor | Inverter | DC/DC converter | SmartLithium | Inverter RS | Smart Battery Protect | (Lynx Smart) BMS | Multi RS | VE.Bus | DC Energy Meter |
| ---------------------------------- | ------------- | --------------- | -------- | --------------- | ------------ | ----------- | --------------------- | ---------------- | -------- | ------ | --------------- |
| `ALARM`                            |               | X               | X        |                 |              |             | X                     |                  |          |        | X               |
| `CHARGER_ERROR`                    | X             |                 |          | X               |              | X           |                       |                  | X        |        |                 |
| `DEVICE_STATE_OFF`                 | X             |                 | X        | X               |              | X           | X                     |                  | X        | X      |                 |
| `DEVICE_STATE_LOW_POWER`           |               |                 |          |                 |              | ?           |                       |                  | ?        | ?      |                 |
| `DEVICE_STATE_FAULT`               | X             |                 | X        | X               |              | X           | X                     |                  | X        | X      |                 |
| `DEVICE_STATE_BULK`                | X             |                 |          |                 |              | ?           |                       |                  | ?        | ?      |                 |
| `DEVICE_STATE_ABSORPTION`          | X             |                 |          |                 |              | ?           |                       |                  | ?        | ?      |                 |
| `DEVICE_STATE_FLOAT`               | X             |                 |          |                 |              | ?           |                       |                  | ?        | ?      |                 |
| `DEVICE_STATE_STORAGE`             |               |                 |          |                 |              | ?           |                       |                  | ?        | ?      |                 |
| `DEVICE_STATE_EQUALIZE_MANUAL`     | X             |                 |          |                 |              | ?           |                       |                  | ?        | ?      |                 |
| `DEVICE_STATE_INVERTING`           |               |                 |          |                 |              | ?           |                       |                  | ?        | ?      |                 |
| `DEVICE_STATE_POWER_SUPPLY`        |               |                 |          |                 |              | ?           |                       |                  | ?        | ?      |                 |
| `DEVICE_STATE_STARTING_UP`         | X             |                 |          |                 |              | ?           |                       |                  | ?        | ?      |                 |
| `DEVICE_STATE_REPEATED_ABSORPTION` |               |                 |          |                 |              | ?           |                       |                  | ?        | ?      |                 |
| `DEVICE_STATE_AUTO_EQUALIZE`       | X             |                 |          |                 |              | ?           |                       |                  | ?        | ?      |                 |
| `DEVICE_STATE_BATTERY_SAFE`        |               |                 |          |                 |              | ?           |                       |                  | ?        | ?      |                 |
| `DEVICE_STATE_EXTERNAL_CONTROL`    | X             |                 |          |                 |              | ?           |                       |                  | ?        | ?      |                 |

? - Device submits a device state. Not sure if it can enter this stage.

### Text Sensor

The following `type` are supported by the `text_sensor` component:

|                  | Solar charger | Battery monitor | Inverter | DC/DC converter | SmartLithium | Inverter RS | Smart Battery Protect | (Lynx Smart) BMS | Multi RS | VE.Bus | DC Energy Meter |
| ---------------- | ------------- | --------------- | -------- | --------------- | ------------ | ----------- | --------------------- | ---------------- | -------- | ------ | --------------- |
| `ACTIVE_AC_IN`   |               |                 |          |                 |              |             |                       |                  | X        | X      |                 |
| `ALARM_REASON`   |               | X               | X        |                 |              |             | X                     |                  |          |        | X               |
| `CHARGER_ERROR`  | X             |                 |          | X               |              | X           |                       |                  | X        |        |                 |
| `DEVICE_STATE`   | X             |                 | X        | X               |              | X           | X                     |                  | X        | X      |                 |
| `OFF_REASON`     |               |                 |          | X               |              |             | X                     |                  |          |        |                 |
| `ERROR_CODE`     |               |                 |          |                 |              |             | X                     |                  |          |        |                 |
| `WARNING_REASON` |               |                 |          |                 |              |             | X                     |                  |          |        |                 |
| `ALARM`          |               |                 |          |                 |              |             |                       |                  |          | X      |                 |

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

See [victron_ble_connect_all.yaml](/victron_ble_connect_all.yaml) for a full example.
Assumption is you are having a `secret.yaml` in the same folder.
