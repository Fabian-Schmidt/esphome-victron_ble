## ESPHome 2026.8+ compatibility, multi-platform BLE and quieter logs

Brings `victron_ble` up to date with current ESPHome. It moves to the new platform-neutral BLE layer and drops the ESP32-only AES dependency. Several sources of log noise are also fixed. Tested on hardware with ESPHome 2026.9.1 on an ESP32 (Shelly Plus 1PM as BLE proxy) and a SmartShunt.

### ⚠️ Breaking changes

- **Requires ESPHome 2026.8.0 or newer.** Older versions get a clear error at config validation. Stay on tag `2024-12-27` if you can't upgrade.
- **`esp32_ble_id:` is now `ble_hub_id:`** on `victron_ble` and `victron_scanner` entries. The old key still works for now, with a deprecation warning.

### ✨ New

- **Not limited to ESP32 any more.** `victron_ble` works with any ESPHome BLE tracker: `esp32_ble_tracker`, `rp2_ble_tracker` (Raspberry Pi Pico W), `bk72xx_ble_tracker`, `ln882h_ble_tracker`. ESP32 and Pico W are built in CI. (`victron_ble_connect` still needs ESP32.)
- **Aux input sensors follow the device setting.** `AUX_VOLTAGE`, `MID_VOLTAGE` and `TEMPERATURE` share the SmartShunt / BMV aux input, so you can configure all three. The one matching the device's aux mode reports values. The others stay *unknown* and log one INFO line saying what the aux input is set to, with no warnings.

### 🐛 Fixes

- **Alarm reason / off reason with several flags.** When more than one alarm was active (e.g. low voltage and low SOC), only the last one was kept. They are now combined, e.g. `Low Voltage, Low SOC`. "No alarm" is still `""`, so existing automations keep working.
- **Text sensors publish only on change.** Previously every advert (about 1/s) republished the same value, e.g. an empty *Battery Alarm reason* every second in logs and Home Assistant.
- **Sensor type/device mismatch warns once.** A sensor type the device doesn't provide used to log a warning and publish NaN on every advert. It now warns once, and this also works with filters such as `throttle_average`.

### 🔧 Under the hood

- **Decryption uses ESPHome's portable AES** (`ble_device_base`) instead of ESP-IDF's `esp_aes`. That interface is ESP32-only and is expected to go away with ESP-IDF 6 / mbedTLS 4. Victron adverts are AES-128-CTR, and output is byte-for-byte identical.
- **Moved off `esp32_ble_tracker` internals** to `ble_device_base` (`ESPBTDeviceListener`, `BLE_DEVICE_SCHEMA`, `register_ble_device`), following the in-tree BLE sensors.
- **Removed deprecated / soon-removed APIs.** `ESPBTDevice::address_str()` is replaced by `address_str_to()`, and the `str_snprintf` / `std::string format_hex_pretty` helpers are no longer used. The device address string is formatted once instead of per log line.
- **Sensor defaults** (unit, device class, accuracy, state class) are applied before validation instead of by editing the config in `FINAL_VALIDATE_SCHEMA`. The resulting config is unchanged.
- **Code cleanup.** The main component is declared as `Component`, which it always was in C++. Standard `uintN_t` types replace `u_intN_t`.
- **CI** uses Python 3.12 and current GitHub Actions. It builds every test config against ESPHome 2026.9.1 and the latest release, and adds a Pico W build.

### Upgrading

```yaml
external_components:
  - source: github://darek-margas/esphome-victron_ble@2026-10-05.8
    components: [victron_ble]
    refresh: 0s
```

Rename any `esp32_ble_id:` under `victron_ble:` to `ble_hub_id:`, if you use it. No other config changes are needed.
