# Intro

Victron BLE Connect is the initally only support way by Victron to read data from the Victron Smart Shunt.

It uses [Bluetooth connection protocol](https://community.victronenergy.com/questions/93919/victron-bluetooth-ble-protocol-publication.html).

Since then they moved on and most devices support Bluetooth advertising protocol by the `victron_ble` component.

This is mainly here for archive reasons.

## `victron_ble_connect` component

### Steps

1. Use a SmartShunt, other devices don't support this yet.
2. Use VictronConnect v5.42 or newer.
3. Update the SmartShunt to version v2.31 or later.
4. Connect to the SmartShunt using VictronConnect, and enable this protocol (screenshot below)
5. Disconnect VictronConnect from the SmartShunt.
6. Use `victron_scanner` to find mac address of your SmartShunt.

![The setting to enable this "third party implementation"-protocol](/img/VictronAppEnableGATT.png)

### Find the MAC Adress of your Smart Shunt

Use the Victron scanner component to find the MAC address of your Victron Smart Shunt.

```yaml
esphome:
  name: "victron-scanner"

external_components:
  - source: github://Fabian-Schmidt/esphome-victron_ble

esp32:
  board: esp32dev

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
  - source: github://Fabian-Schmidt/esphome-victron_ble


esp32_ble:
  io_capability: keyboard_only

ble_client:
- mac_address: <MY VICTRON SMART SHUNT MAC ADDRESS>
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