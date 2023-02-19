#pragma once

#include "esphome/core/component.h"
#include "esphome/components/ble_client/ble_client.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"
#include "esphome/components/sensor/sensor.h"

#ifdef USE_ESP32

namespace esphome {
namespace victron_ble {

static const esp32_ble_tracker::ESPBTUUID SERVICE_UUID =
    esp32_ble_tracker::ESPBTUUID::from_raw("65970000-4bda-4c1e-af4b-551c4cf74769");
// A keep alive is required. Without that, the device will automatically disconnect after one minute.
// Keep-alive: uint16 - 0,001s - 0xFFFF
static const esp32_ble_tracker::ESPBTUUID CHARACTERISTIC_UUID_KEEP_ALIVE =
    esp32_ble_tracker::ESPBTUUID::from_raw("6597FFFF-4BDA-4C1E-AF4B-551C4CF74769");
// State of Charge: uint16 - 0.01% - 0xFFFF
static const esp32_ble_tracker::ESPBTUUID CHARACTERISTIC_UUID_SOC =
    esp32_ble_tracker::ESPBTUUID::from_raw("65970FFF-4BDA-4C1E-AF4B-551C4CF74769");
// Voltage: int16 - 0.01V - 0x7FFF
static const esp32_ble_tracker::ESPBTUUID CHARACTERISTIC_UUID_VOLTAGE =
    esp32_ble_tracker::ESPBTUUID::from_raw("6597ED8D-4BDA-4C1E-AF4B-551C4CF74769");
// Power consumption: int16 - 1W - 0x7FFF
static const esp32_ble_tracker::ESPBTUUID CHARACTERISTIC_UUID_POWER =
    esp32_ble_tracker::ESPBTUUID::from_raw("6597ED8E-4BDA-4C1E-AF4B-551C4CF74769");
// Current: int32 - 0.001A - 0x7FFFFFFF
static const esp32_ble_tracker::ESPBTUUID CHARACTERISTIC_UUID_CURRENT =
    esp32_ble_tracker::ESPBTUUID::from_raw("6597ED8C-4BDA-4C1E-AF4B-551C4CF74769");
// Consumed Ah: int32 - 0.1Ah - 0x7FFFFFFF
static const esp32_ble_tracker::ESPBTUUID CHARACTERISTIC_UUID_AH =
    esp32_ble_tracker::ESPBTUUID::from_raw("6597EEFF-4BDA-4C1E-AF4B-551C4CF74769");
// Starter Battery Voltage: int16 - 0.01V - 0x7FFF
static const esp32_ble_tracker::ESPBTUUID CHARACTERISTIC_UUID_VOLTAGE2 =
    esp32_ble_tracker::ESPBTUUID::from_raw("6597ED7D-4BDA-4C1E-AF4B-551C4CF74769");
// ??: uint16 - ?? - 0xFFFF
static const esp32_ble_tracker::ESPBTUUID CHARACTERISTIC_UUID_VAL2 =
    esp32_ble_tracker::ESPBTUUID::from_raw("6597EDEC-4BDA-4C1E-AF4B-551C4CF74769");
// ??: uint16 - ?? - 0xFFFF
static const esp32_ble_tracker::ESPBTUUID CHARACTERISTIC_UUID_VAL3 =
    esp32_ble_tracker::ESPBTUUID::from_raw("65970382-4BDA-4C1E-AF4B-551C4CF74769");
// ?Temperature?: int16 - ?? - 0x7FFF
static const esp32_ble_tracker::ESPBTUUID CHARACTERISTIC_UUID_VAL4 =
    esp32_ble_tracker::ESPBTUUID::from_raw("65970383-4BDA-4C1E-AF4B-551C4CF74769");
// Remaining time: uint16 - 1 minute - 0xFFFF
static const esp32_ble_tracker::ESPBTUUID CHARACTERISTIC_UUID_REMAINING_TIME =
    esp32_ble_tracker::ESPBTUUID::from_raw("65970FFE-4BDA-4C1E-AF4B-551C4CF74769");

class VictronBle : public PollingComponent, public ble_client::BLEClientNode {
 public:
  VictronBle();
  void dump_config() override;
  void update() override;

  void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                           esp_ble_gattc_cb_param_t *param) override;
  //   void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) override;
  //   void connect() override;

  //   // ESPBTDeviceListener
  //   void on_scan_end() override;
  //   bool parse_device(const esp32_ble_tracker::ESPBTDevice &device) override;
  std::string get_name() {
    if (this->name_.empty()) {
      return this->parent_->address_str();
    } else {
      return this->name_;
    }
  }
  void set_notify(bool val) { this->notify_ = val; }
  void set_state_of_charge(sensor::Sensor *val) { this->state_of_charge_ = val; }
  void set_voltage(sensor::Sensor *val) { this->voltage_ = val; }
  void set_power(sensor::Sensor *val) { this->power_ = val; }
  void set_current(sensor::Sensor *val) { this->current_ = val; }
  void set_ah(sensor::Sensor *val) { this->ah_ = val; }
  void set_starter_battery_voltage(sensor::Sensor *val) { this->starter_battery_voltage_ = val; }
  void set_val2(sensor::Sensor *val) { this->val2_ = val; }
  void set_val3(sensor::Sensor *val) { this->val3_ = val; }
  void set_val4(sensor::Sensor *val) { this->val4_ = val; }
  void set_remaining_time(sensor::Sensor *val) { this->remaining_time_ = val; }

 protected:
  bool notify_ = true;

  std::string name_;
  uint16_t handle_keep_alive_;

#define BLE_DATA_STORAGE(name, type) \
  sensor::Sensor * name {nullptr}; \
  uint16_t handle_##name = 0; \
  type value_##name = std::numeric_limits<type>::max(); \
  bool value_is_set_##name = false;
  
  BLE_DATA_STORAGE(state_of_charge_, u_int16_t)
  BLE_DATA_STORAGE(voltage_, int16_t)
  BLE_DATA_STORAGE(power_, int16_t)
  BLE_DATA_STORAGE(current_, int32_t)
  BLE_DATA_STORAGE(ah_, int32_t)
  BLE_DATA_STORAGE(starter_battery_voltage_, int16_t)
  BLE_DATA_STORAGE(val2_, u_int16_t)
  BLE_DATA_STORAGE(val3_, u_int16_t)
  BLE_DATA_STORAGE(val4_, int16_t)
  BLE_DATA_STORAGE(remaining_time_, u_int16_t)
#undef BLE_DATA_STORAGE

  uint8_t read_request_started_ = 0;
  uint16_t find_handle_(const esp32_ble_tracker::ESPBTUUID *characteristic, bool read_value =  true);
  void read_value_(const uint16_t handle, const uint8_t *value, const uint16_t value_len,
                   const bool register_for_notify = false);
  void send_keep_alive_();
  void update_sensors_();
};

}  // namespace victron_ble
}  // namespace esphome

#endif
