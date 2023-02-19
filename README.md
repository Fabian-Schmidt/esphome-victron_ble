# ESPHome victron_ble component

Using the [official Victron Bluetooth BLE Protocol](https://community.victronenergy.com/questions/93919/victron-bluetooth-ble-protocol-publication.html) for fetching data from Victron SmartShunt via Bluetooth LE via ESPHome.

This solution is using the [PR#4258 of ESPHome](https://github.com/esphome/esphome/pull/4258) for connecting to Victron Smart Shunt.

## Steps

1. Use a SmartShunt, other devices don't support this yet.
2. Use VictronConnect v5.42 or newer.
3. Update the SmartShunt to version v2.31 or later.
4. Connect to the SmartShunt using VictronConnect, and enable this protocol (screenshot below)
5. Power cycle the SmartShunt
6. Use `victron_scanner` to find mac address of your SmartShunt.

![The setting to enable this "third party implementation"-protocol](/img/VictronAppEnableGATT.png)

## Find the MAC Adress of your Smart Shunt

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

## Example ESPHome config

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
  - platform: victron_ble
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

## TODO

- In `notify` mode Victron SmartShunt submits on change max every second one value. It send to the sensor api with `update_interval` (default 2min). Currently the last value is submitted. Implement an average function between sensor submits.  
