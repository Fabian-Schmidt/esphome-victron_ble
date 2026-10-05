#include "victron_listener.h"
#include "esphome/core/log.h"

namespace esphome {
namespace victron_scanner {

static const char *const TAG = "victron_scanner";

// `Victron Energy BV`
static const uint16_t MANUFACTURER_ID = 0x02E1;

/**
 * Parse all incoming BLE payloads to see if it is a Victron BLE advertisement.
 * Currently this supports the following products:
 *
 *   SMART SHUNT 500A/50mV
 */

bool VictronListener::parse_device(const ble_device_base::ESPBTDevice &device) {
  if (device.get_name().size() == 0) {
    return false;
  }

  const auto &manu_datas = device.get_manufacturer_datas();

  if (manu_datas.size() != 1) {
    return false;
  }

  const auto &manu_data = manu_datas[0];

  if (manu_data.uuid != ble_device_base::ESPBTUUID::from_uint16(MANUFACTURER_ID)) {
    return false;
  }
  if (manu_data.data.size() < sizeof(VICTRON_BT_MANUFACTURER_DATA)) {
    return false;
  }

  const auto victron_data = (VICTRON_BT_MANUFACTURER_DATA *) manu_data.data.data();

  if (victron_data->product_id == VICTRON_PRODUCT_ID::SMARTSHUNT_500A_50MV) {
    char addr_buf[ble_device_base::ESPBTDevice::MAC_ADDRESS_PRETTY_BUFFER_SIZE];
    ESP_LOGI(TAG, "FOUND SMART SHUNT 500A/50mV '%s' at %s", device.get_name().c_str(),
             device.address_str_to(addr_buf));
  }

  return false;
}

}  // namespace victron_scanner
}  // namespace esphome

