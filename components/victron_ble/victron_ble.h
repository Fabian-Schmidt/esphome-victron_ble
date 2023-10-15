#pragma once

#include <array>
#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"

#ifdef USE_ESP32

// Max documented message size is 16 byte. Maximum length of a record is 20 bytes = 4 byte header
// (VICTRON_BLE_RECORD_BASE minus VICTRON_BLE_MANUFACTURER_DATA) + 16 byte payload
#define VICTRON_ENCRYPTED_DATA_MAX_SIZE 16

namespace esphome {
namespace victron_ble {

// `Victron Energy BV`
static const uint16_t VICTRON_MANUFACTURER_ID = 0x02E1;

enum class VICTRON_MANUFACTURER_RECORD_TYPE : u_int8_t {
  PRODUCT_ADVERTISEMENT = 0x10,
};

enum class VICTRON_PRODUCT_ID : u_int16_t {
  // BMV-700
  BMV_700 = 0x0203,
  // BMV-702
  BMV_702 = 0x0204,
  // BMV-700H
  BMV_700H = 0x0205,
  // BlueSolar MPPT 75|15
  BlueSolar_MPPT_75_15 = 0xA042,
  // BlueSolar MPPT 100|15
  BlueSolar_MPPT_100_15 = 0xA043,
  // BlueSolar MPPT 150|70
  BlueSolar_MPPT_150_70 = 0xA046,
  // BlueSolar MPPT 150|100
  BlueSolar_MPPT_150_100 = 0xA047,
  // BlueSolar MPPT 100|50 rev2
  BlueSolar_MPPT_100_50_REV2 = 0xA049,
  // BlueSolar MPPT 100|30 rev2
  BlueSolar_MPPT_100_30_REV2 = 0xA04A,
  // BlueSolar MPPT 150|35 rev2
  BlueSolar_MPPT_150_35_REV2 = 0xA04B,
  // BlueSolar MPPT 75|10
  BlueSolar_MPPT_75_10 = 0xA04C,
  // BlueSolar MPPT 150|45
  BlueSolar_MPPT_150_45 = 0xA04D,
  // BlueSolar MPPT 150|60
  BlueSolar_MPPT_150_60 = 0xA04E,
  // BlueSolar MPPT 150|85
  BlueSolar_MPPT_150_85 = 0xA04F,
  // SmartSolar MPPT 250|100
  SmartSolar_MPPT_250_100 = 0xA050,
  // SmartSolar MPPT 75|15
  SmartSolar_MPPT_75_15 = 0xA053,
  // SmartSolar MPPT 75|10
  SmartSolar_MPPT_75_10 = 0xA054,
  // SmartSolar MPPT 100|15
  SmartSolar_MPPT_100_15 = 0xA055,
  // SmartSolar MPPT 100|30
  SmartSolar_MPPT_100_30 = 0xA056,
  // SmartSolar MPPT 100|50
  SmartSolar_MPPT_100_50 = 0xA057,
  // SmartSolar MPPT 150|35
  SmartSolar_MPPT_150_35 = 0xA058,
  // SmartSolar MPPT 150|100 rev2
  SmartSolar_MPPT_150_100_REV2 = 0xA059,
  // SmartSolar MPPT 150|85 rev2
  SmartSolar_MPPT_150_85_REV2 = 0xA05A,
  // SmartSolar MPPT 250|70
  SmartSolar_MPPT_250_70 = 0xA05B,
  // SmartSolar MPPT 250|85
  SmartSolar_MPPT_250_85 = 0xA05C,
  // SmartSolar MPPT 250|60
  SmartSolar_MPPT_250_60 = 0xA05D,
  // SmartSolar MPPT 250|45
  SmartSolar_MPPT_250_45 = 0xA05E,
  // SmartSolar MPPT 100|20
  SmartSolar_MPPT_100_20 = 0xA05F,
  // SmartSolar MPPT 100|20 48V
  SmartSolar_MPPT_100_20_48V = 0xA060,
  // SmartSolar MPPT 150|45
  SmartSolar_MPPT_150_45 = 0xA061,
  // SmartSolar MPPT 150|60
  SmartSolar_MPPT_150_60 = 0xA062,
  // SmartSolar MPPT 150|70
  SmartSolar_MPPT_150_70 = 0xA063,
  // SmartSolar MPPT 250|85 rev2
  SmartSolar_MPPT_250_85_REV2 = 0xA064,
  // SmartSolar MPPT 250|100 rev2
  SmartSolar_MPPT_250_100_REV2 = 0xA065,
  // BlueSolar MPPT 100|20
  BlueSolar_MPPT_100_20 = 0xA066,
  // BlueSolar MPPT 100|20 48V
  BlueSolar_MPPT_100_20_48V = 0xA067,
  // SmartSolar MPPT 250|60 rev2
  SmartSolar_MPPT_250_60_REV2 = 0xA068,
  // SmartSolar MPPT 250|70 rev2
  SmartSolar_MPPT_250_70_REV2 = 0xA069,
  // SmartSolar MPPT 150|45 rev2
  SmartSolar_MPPT_150_45_REV2 = 0xA06A,
  // SmartSolar MPPT 150|60 rev2
  SmartSolar_MPPT_150_60_REV2 = 0xA06B,
  // SmartSolar MPPT 150|70 rev2
  SmartSolar_MPPT_150_70_REV2 = 0xA06C,
  // SmartSolar MPPT 150|85 rev3
  SmartSolar_MPPT_150_85_REV3 = 0xA06D,
  // SmartSolar MPPT 150|100 rev3
  SmartSolar_MPPT_150_100_REV3 = 0xA06E,
  // BlueSolar MPPT 150|45 rev2
  BlueSolar_MPPT_150_45_REV2 = 0xA06F,
  // BlueSolar MPPT 150|60 rev2
  BlueSolar_MPPT_150_60_REV2 = 0xA070,
  // BlueSolar MPPT 150|70 rev2
  BlueSolar_MPPT_150_70_REV2 = 0xA071,
  // BlueSolar MPPT 150/45 rev3
  BlueSolar_MPPT_150_45_REV3 = 0xA072,
  // SmartSolar MPPT 150/45 rev3
  SmartSolar_MPPT_150_45_REV3 = 0xA073,
  // SmartSolar MPPT 75/10 rev2
  SmartSolar_MPPT_75_10_REV2 = 0xA074,
  // SmartSolar MPPT 75/15 rev2
  SmartSolar_MPPT_75_15_REV2 = 0xA075,
  // BlueSolar MPPT 100/30 rev3
  BlueSolar_MPPT_100_30_REV3 = 0xA076,
  // BlueSolar MPPT 100/50 rev3
  BlueSolar_MPPT_100_50_REV3 = 0xA077,
  // BlueSolar MPPT 150/35 rev3
  BlueSolar_MPPT_150_35_REV3 = 0xA078,
  // BlueSolar MPPT 75/10 rev2
  BlueSolar_MPPT_75_10_REV2 = 0xA079,
  // BlueSolar MPPT 75/15 rev2
  BlueSolar_MPPT_75_15_REV2 = 0xA07A,
  // BlueSolar MPPT 100/15 rev2
  BlueSolar_MPPT_100_15_REV2 = 0xA07B,
  // BlueSolar MPPT 75/10 rev3
  BlueSolar_MPPT_75_10_REV3 = 0xA07C,
  // BlueSolar MPPT 75/15 rev3
  BlueSolar_MPPT_75_15_REV3 = 0xA07D,
  // SmartSolar MPPT 100/30 12V
  SmartSolar_MPPT_100_30_12V = 0xA07E,
  // All-In-1 SmartSolar MPPT 75/15 12V
  All_in_1_SmartSolar_MPPT_75_15_12V = 0xA07F,
  // SmartSolar MPPT VE.Can 150/70
  SmartSolar_MPPT_VE_CAN_150_70 = 0xA102,
  // SmartSolar MPPT VE.Can 150/45
  SmartSolar_MPPT_VE_CAN_150_45 = 0xA103,
  // SmartSolar MPPT VE.Can 150/60
  SmartSolar_MPPT_VE_CAN_150_60 = 0xA104,
  // SmartSolar MPPT VE.Can 150/85
  SmartSolar_MPPT_VE_CAN_150_85 = 0xA105,
  // SmartSolar MPPT VE.Can 150/100
  SmartSolar_MPPT_VE_CAN_150_100 = 0xA106,
  // SmartSolar MPPT VE.Can 250/45
  SmartSolar_MPPT_VE_CAN_250_45 = 0xA107,
  // SmartSolar MPPT VE.Can 250/60
  SmartSolar_MPPT_VE_CAN_250_60 = 0xA108,
  // SmartSolar MPPT VE.Can 250/70
  SmartSolar_MPPT_VE_CAN_250_70 = 0xA109,
  // SmartSolar MPPT VE.Can 250/85
  SmartSolar_MPPT_VE_CAN_250_85 = 0xA10A,
  // SmartSolar MPPT VE.Can 250/100
  SmartSolar_MPPT_VE_CAN_250_100 = 0xA10B,
  // SmartSolar MPPT VE.Can 150/70 rev2
  SmartSolar_MPPT_VE_CAN_150_70_REV2 = 0xA10C,
  // SmartSolar MPPT VE.Can 150/85 rev2
  SmartSolar_MPPT_VE_CAN_150_85_REV2 = 0xA10D,
  // SmartSolar MPPT VE.Can 150/100 rev2
  SmartSolar_MPPT_VE_CAN_150_100_REV2 = 0xA10E,
  // BlueSolar MPPT VE.Can 150/100
  BlueSolar_MPPT_VE_CAN_150_100 = 0xA10F,
  // BlueSolar MPPT VE.Can 250/70
  BlueSolar_MPPT_VE_CAN_250_70 = 0xA112,
  // BlueSolar MPPT VE.Can 250/100
  BlueSolar_MPPT_VE_CAN_250_100 = 0xA113,
  // SmartSolar MPPT VE.Can 250/70 rev2
  SmartSolar_MPPT_VE_CAN_250_70_REV2 = 0xA114,
  // SmartSolar MPPT VE.Can 250/100 rev2
  SmartSolar_MPPT_VE_CAN_250_100_REV2 = 0xA115,
  // SmartSolar MPPT VE.Can 250/85 rev2
  SmartSolar_MPPT_VE_CAN_250_85_REV2 = 0xA116,
  // BlueSolar MPPT VE.Can 150/100 rev2
  BlueSolar_MPPT_VE_CAN_150_100_REV2 = 0xA117,
  // Phoenix Inverter 12V 250VA 230V
  PHOENIX_INVERTER_12V_250VA_230V = 0xA231,
  // Phoenix Inverter 24V 250VA 230V
  PHOENIX_INVERTER_24V_250VA_230V = 0xA232,
  // Phoenix Inverter 48V 250VA 230V
  PHOENIX_INVERTER_48V_250VA_230V = 0xA234,
  // Phoenix Inverter 12V 250VA 120V
  PHOENIX_INVERTER_12V_250VA_120V = 0xA239,
  // Phoenix Inverter 24V 250VA 120V
  PHOENIX_INVERTER_24V_250VA_120V = 0xA23A,
  // Phoenix Inverter 48V 250VA 120V
  PHOENIX_INVERTER_48V_250VA_120V = 0xA23C,
  // Phoenix Inverter 12V 375VA 230V
  PHOENIX_INVERTER_12V_375VA_230V = 0xA241,
  // Phoenix Inverter 24V 375VA 230V
  PHOENIX_INVERTER_24V_375VA_230V = 0xA242,
  // Phoenix Inverter 48V 375VA 230V
  PHOENIX_INVERTER_48V_375VA_230V = 0xA244,
  // Phoenix Inverter 12V 375VA 120V
  PHOENIX_INVERTER_12V_375VA_120V = 0xA249,
  // Phoenix Inverter 24V 375VA 120V
  PHOENIX_INVERTER_24V_375VA_120V = 0xA24A,
  // Phoenix Inverter 48V 375VA 120V
  PHOENIX_INVERTER_48V_375VA_120V = 0xA24C,
  // Phoenix Inverter 12V 500VA 230V
  PHOENIX_INVERTER_12V_500VA_230V = 0xA251,
  // Phoenix Inverter 24V 500VA 230V
  PHOENIX_INVERTER_24V_500VA_230V = 0xA252,
  // Phoenix Inverter 48V 500VA 230V
  PHOENIX_INVERTER_48V_500VA_230V = 0xA254,
  // Phoenix Inverter 12V 500VA 120V
  PHOENIX_INVERTER_12V_500VA_120V = 0xA259,
  // Phoenix Inverter 24V 500VA 120V
  PHOENIX_INVERTER_24V_500VA_120V = 0xA25A,
  // Phoenix Inverter 48V 500VA 120V
  PHOENIX_INVERTER_48V_500VA_120V = 0xA25C,
  // Phoenix Inverter 12V 800VA 230V
  PHOENIX_INVERTER_12V_800VA_230V = 0xA261,
  // Phoenix Inverter 24V 800VA 230V
  PHOENIX_INVERTER_24V_800VA_230V = 0xA262,
  // Phoenix Inverter 48V 800VA 230V
  PHOENIX_INVERTER_48V_800VA_230V = 0xA264,
  // Phoenix Inverter 12V 800VA 120V
  PHOENIX_INVERTER_12V_800VA_120V = 0xA269,
  // Phoenix Inverter 24V 800VA 120V
  PHOENIX_INVERTER_24V_800VA_120V = 0xA26A,
  // Phoenix Inverter 48V 800VA 120V
  PHOENIX_INVERTER_48V_800VA_120V = 0xA26C,
  // Phoenix Inverter 12V 1200VA 230V
  PHOENIX_INVERTER_12V_1200VA_230V = 0xA271,
  // Phoenix Inverter 24V 1200VA 230V
  PHOENIX_INVERTER_24V_1200VA_230V = 0xA272,
  // Phoenix Inverter 48V 1200VA 230V
  PHOENIX_INVERTER_48V_1200VA_230V = 0xA274,
  // Phoenix Inverter 12V 1200VA 120V
  PHOENIX_INVERTER_12V_1200VA_120V = 0xA279,
  // Phoenix Inverter 24V 1200VA 120V
  PHOENIX_INVERTER_24V_1200VA_120V = 0xA27A,
  // Phoenix Inverter 48V 1200VA 120V
  PHOENIX_INVERTER_48V_1200VA_120V = 0xA27C,
  // Phoenix Inverter 12V 1600VA 230V
  PHOENIX_INVERTER_12V_1600VA_230V = 0xA281,
  // Phoenix Inverter 24V 1600VA 230V
  PHOENIX_INVERTER_24V_1600VA_230V = 0xA282,
  // Phoenix Inverter 48V 1600VA 230V
  PHOENIX_INVERTER_48V_1600VA_230V = 0xA284,
  // Phoenix Inverter 12V 2000VA 230V
  PHOENIX_INVERTER_12V_2000VA_230V = 0xA291,
  // Phoenix Inverter 24V 2000VA 230V
  PHOENIX_INVERTER_24V_2000VA_230V = 0xA292,
  // Phoenix Inverter 48V 2000VA 230V
  PHOENIX_INVERTER_48V_2000VA_230V = 0xA294,
  // Phoenix Inverter 12V 3000VA 230V
  PHOENIX_INVERTER_12V_3000VA_230V = 0xA2A1,
  // Phoenix Inverter 24V 3000VA 230V
  PHOENIX_INVERTER_24V_3000VA_230V = 0xA2A2,
  // Phoenix Inverter 48V 3000VA 230V
  PHOENIX_INVERTER_48V_3000VA_230V = 0xA2A4,
  // Phoenix Smart IP43 Charger 12|50 (1+1)
  PHOENIX_INVERTER_SMART_IP43_CHARGER_12_50 = 0xA340,
  // Phoenix Smart IP43 Charger 12|50 (3)
  PHOENIX_INVERTER_SMART_IP43_CHARGER_12_50_3 = 0xA341,
  // Phoenix Smart IP43 Charger 24|25 (1+1)
  PHOENIX_INVERTER_SMART_IP43_CHARGER_24_50 = 0xA342,
  // Phoenix Smart IP43 Charger 24|25 (3)s
  PHOENIX_INVERTER_SMART_IP43_CHARGER_24_50_3 = 0xA343,
  // Phoenix Smart IP43 Charger 12|30 (1+1)
  PHOENIX_INVERTER_SMART_IP43_CHARGER_12_30 = 0xA344,
  // Phoenix Smart IP43 Charger 12|30 (3)
  PHOENIX_INVERTER_SMART_IP43_CHARGER_12_30_3 = 0xA345,
  // Phoenix Smart IP43 Charger 24|16 (1+1)
  PHOENIX_INVERTER_SMART_IP43_CHARGER_24_16 = 0xA346,
  // Phoenix Smart IP43 Charger 24|16 (3)
  PHOENIX_INVERTER_SMART_IP43_CHARGER_24_16_3 = 0xA347,
  // BMV-712 Smart
  BMV_712_SMART = 0xA381,
  // BMV-710H Smart
  BMV_710H_SMART = 0xA382,
  // BMV-712 Smart Rev2
  BMV_712_SMART_REV2 = 0xA383,
  // SmartShunt 500A/50mV
  SMARTSHUNT_500A_50MV = 0xA389,
  // SmartShunt 1000A/50mV
  SMARTSHUNT_1000A_50MV = 0xA38A,
  // SmartShunt 2000A/50mV
  SMARTSHUNT_2000A_50MV = 0xA38B,
  // Smart BuckBoost 12V/12V-50A
  SMART_BUCKBOOST_12V_12V_50A = 0xA3F0,
};

struct VICTRON_BLE_MANUFACTURER_DATA {  // NOLINT(readability-identifier-naming,altera-struct-pack-align)
  VICTRON_MANUFACTURER_RECORD_TYPE manufacturer_record_type;
  u_int8_t manufacturer_record_length;
  VICTRON_PRODUCT_ID product_id;
} __attribute__((packed));

// source:
// - extra-manufacturer-data-2022-12-14.pdf
enum class VICTRON_BLE_RECORD_TYPE : u_int8_t {
  // VICTRON_BLE_RECORD_TEST
  TEST_RECORD = 0x00,
  // VICTRON_BLE_RECORD_SOLAR_CHARGER
  SOLAR_CHARGER = 0x01,
  // VICTRON_BLE_RECORD_BATTERY_MONITOR
  BATTERY_MONITOR = 0x02,
  // VICTRON_BLE_RECORD_INVERTER
  INVERTER = 0x03,
  // VICTRON_BLE_RECORD_DCDC_CONVERTER
  DCDC_CONVERTER = 0x04,
  // VICTRON_BLE_RECORD_SMART_LITHIUM
  SMART_LITHIUM = 0x05,
  // VICTRON_BLE_RECORD_INVERTER_RS
  INVERTER_RS = 0x06,
  // Not defined
  GX_DEVICE = 0x07,
  // Not defined
  AC_CHARGER = 0x08,
  // VICTRON_BLE_RECORD_SMART_BATTERY_PROTECT
  SMART_BATTERY_PROTECT = 0x09,
  // VICTRON_BLE_RECORD_LYNX_SMART_BMS
  LYNX_SMART_BMS = 0x0A,
  // VICTRON_BLE_RECORD_MULTI_RS
  MULTI_RS = 0x0B,
  // VICTRON_BLE_RECORD_VE_BUS
  VE_BUS = 0x0C,
  // VICTRON_BLE_RECORD_DC_ENERGY_METER
  DC_ENERGY_METER = 0x0D,
};

struct VICTRON_BLE_RECORD_BASE {  // NOLINT(readability-identifier-naming,altera-struct-pack-align)
  VICTRON_BLE_MANUFACTURER_DATA manufacturer_base;
  VICTRON_BLE_RECORD_TYPE record_type;
  u_int8_t data_counter_lsb;
  u_int8_t data_counter_msb;
  // Byte 0 of the encryption key (bindkey)
  u_int8_t encryption_key_0;
} __attribute__((packed));

struct VICTRON_BLE_RECORD_TEST {  // NOLINT(readability-identifier-naming,altera-struct-pack-align)
  // 1 s, 0 .. 34 year
  u_int32_t uptime : 30;
  // 1 °C, -40 .. 86 °C
  u_int16_t temperature : 7;
} __attribute__((packed));

// For the following devices: MPPT, Inverter, Charger
// source:
// - https://www.victronenergy.com/upload/documents/VE.Direct-Protocol-3.33.pdf
// - https://github.com/victronenergy/venus-html5-app/blob/master/src/app/utils/constants.js
enum class VE_REG_DEVICE_STATE : u_int8_t {
  // Off
  OFF = 0x00,
  // Low power
  LOW_POWER = 0x01,
  // Fault
  FAULT = 0x02,
  // Bulk
  BULK = 0x03,
  // Absorption
  ABSORPTION = 0x04,
  // Float
  FLOAT = 0x05,
  // Storage
  STORAGE = 0x06,
  // Equalize (manual)
  EQUALIZE_MANUAL = 0x07,
  // Pass Thru
  PASSTHRU = 0x08,
  // Inverting
  INVERTING = 0x09,
  // Assisting
  ASSISTING = 0x0A,
  // Power supply
  POWER_SUPPLY = 0x0B,
  // Starting-up
  STARTING_UP = 0xF5,
  // Repeated absorption
  REPEATED_ABSORPTION = 0xF6,
  // Auto equalize / Recondition
  AUTO_EQUALIZE = 0xF7,
  // BatterySafe
  BATTERY_SAFE = 0xF8,
  // External Control
  EXTERNAL_CONTROL = 0xFC,
};

// Source: VE.Direct-Protocol-3.32.pdf & https://www.victronenergy.com/live/mppt-error-codes
enum class VE_REG_CHR_ERROR_CODE : u_int8_t {
  // No error
  NO_ERROR = 0,
  // Err 1 - Battery temperature too high
  TEMPERATURE_BATTERY_HIGH = 1,
  // Err 2 - Battery voltage too high
  VOLTAGE_HIGH = 2,
  // Err 3 - Remote temperature sensor failure (auto-reset)
  REMOTE_TEMPERATURE_A = 3,
  // Err 4 - Remote temperature sensor failure (auto-reset)
  REMOTE_TEMPERATURE_B = 4,
  // Err 5 - Remote temperature sensor failure (not auto-reset)
  REMOTE_TEMPERATURE_C = 5,
  // Err 6 - Remote battery voltage sense failure
  REMOTE_BATTERY_A = 6,
  // Err 7 - Remote battery voltage sense failure
  REMOTE_BATTERY_B = 7,
  // Err 8 - Remote battery voltage sense failure
  REMOTE_BATTERY_C = 8,
  // Err 11 - Battery high ripple voltage
  HIGH_RIPPLE = 11,
  // Err 14 - Battery temperature too low
  TEMPERATURE_BATTERY_LOW = 14,
  // Err 17 - Charger temperature too high
  TEMPERATURE_CHARGER = 17,
  // Err 18 - Charger over current
  OVER_CURRENT = 18,
  // Err 20 - Bulk time limit exceeded
  BULK_TIME = 20,
  // Err 21 - Current sensor issue (sensor bias/sensor broken)
  CURRENT_SENSOR = 21,
  // Err 22 - Internal temperature sensor failure
  INTERNAL_TEMPERATURE_A = 22,
  // Err 23 - Internal temperature sensor failure
  INTERNAL_TEMPERATURE_B = 23,
  // Err 24 - Fan failure
  FAN = 24,
  // Err 26 - Terminals overheated
  OVERHEATED = 26,
  // Err 27 - Charger short circuit
  SHORT_CIRCUIT = 27,
  // Err 28 - Power stage issue, Converter issue (dual converter models only)
  CONVERTER_ISSUE = 28,
  // Err 29 - Over-Charge protection
  OVER_CHARGE = 29,
  // Err 33 - Input voltage too high (solar panel), PV over-voltage
  INPUT_VOLTAGE = 33,
  // Err 34 - Input current too high (solar panel), PV over-current
  INPUT_CURRENT = 34,
  // Err 35 - PV over-power
  INPUT_POWER = 35,
  // Err 38 - Input shutdown (due to excessive battery voltage)
  INPUT_SHUTDOWN_VOLTAGE = 38,
  // Err 39 - Input shutdown (due to current flow during off mode)
  INPUT_SHUTDOWN_CURRENT = 39,
  // Err 40 - PV Input failed to shutdown
  INPUT_SHUTDOWN_FAILURE = 40,
  // Err 41 - Inverter shutdown (PV isolation)
  INVERTER_SHUTDOWN_41 = 41,
  // Err 42 - Inverter shutdown (PV isolation)
  INVERTER_SHUTDOWN_42 = 42,
  // Err 43 - Inverter shutdown (Ground Fault)
  INVERTER_SHUTDOWN_43 = 43,
  // Err 50 - Inverter overload
  INVERTER_OVERLOAD = 50,
  // Err 51 - Inverter temperature too high
  INVERTER_TEMPERATURE = 51,
  // Err 52 - Inverter peak current
  INVERTER_PEAK_CURRENT = 52,
  // Err 53 - Inverter output voltage
  INVERTER_OUPUT_VOLTAGE_A = 53,
  // Err 54 - Inverter output voltage
  INVERTER_OUPUT_VOLTAGE_B = 54,
  // Err 55 - Inverter self test failed
  INVERTER_SELF_TEST_A = 55,
  // Err 56 - Inverter self test failed
  INVERTER_SELF_TEST_B = 56,
  // Err 57 - Inverter ac voltage on output
  INVERTER_AC = 57,
  // Err 58 - Inverter self test failed
  INVERTER_SELF_TEST_C = 58,
  // Information 65 - Communication warning, Lost communication with one of devices
  COMMUNICATION = 65,
  // Information 66 - Incompatible device, Synchronised charging device configuration issue
  SYNCHRONISATION = 66,
  // Err 67 - BMS Connection lost
  BMS = 67,
  // Err 68 - Network misconfigured
  NETWORK_A = 68,
  // Err 69 - Network misconfigured
  NETWORK_B = 69,
  // Err 70 - Network misconfigured
  NETWORK_C = 70,
  // Err 71 - Network misconfigured
  NETWORK_D = 71,
  // Err 80 - PV Input shutdown
  PV_INPUT_SHUTDOWN_80 = 80,
  // Err 81 - PV Input shutdown
  PV_INPUT_SHUTDOWN_81 = 81,
  // Err 82 - PV Input shutdown
  PV_INPUT_SHUTDOWN_82 = 82,
  // Err 83 - PV Input shutdown
  PV_INPUT_SHUTDOWN_83 = 83,
  // Err 84 - PV Input shutdown
  PV_INPUT_SHUTDOWN_84 = 84,
  // Err 85 - PV Input shutdown
  PV_INPUT_SHUTDOWN_85 = 85,
  // Err 86 - PV Input shutdown
  PV_INPUT_SHUTDOWN_86 = 86,
  // Err 87 - PV Input shutdown
  PV_INPUT_SHUTDOWN_87 = 87,
  // Err 114 - CPU temperature too high
  CPU_TEMPERATURE = 114,
  // Err 116 - Factory calibration data lost
  CALIBRATION_LOST = 116,
  // Err 117 - Invalid/incompatible firmware
  FIRMWARE = 117,
  // Err 119 - Settings data lost
  SETTINGS = 119,
  // Err 121 - Tester fail
  TESTER_FAIL = 121,
  // Err 200 - Internal DC voltage error
  INTERNAL_DC_VOLTAGE_A = 200,
  // Err 201 - Internal DC voltage error
  INTERNAL_DC_VOLTAGE_B = 201,
  // Err 202 - PV residual current sensor self-test failure, Internal GFCI sensor error
  SELF_TEST = 202,
  // Err 203 - Internal supply voltage error
  INTERNAL_SUPPLY_A = 203,
  // Err 205 - Internal supply voltage error
  INTERNAL_SUPPLY_B = 205,
  // Err 212 - Internal supply voltage error
  INTERNAL_SUPPLY_C = 212,
  // Err 215 - Internal supply voltage error
  INTERNAL_SUPPLY_D = 215,
};

struct VICTRON_BLE_RECORD_SOLAR_CHARGER {  // NOLINT(readability-identifier-naming,altera-struct-pack-align)
  VE_REG_DEVICE_STATE device_state;
  VE_REG_CHR_ERROR_CODE charger_error;
  // 0.01 V, -327.68 .. 327.66 V
  int16_t battery_voltage;
  // 0.1 A, -3276.8 .. 3276.6 A
  int16_t battery_current;
  // 0.01 kWh, 0 .. 655.34 kWh
  u_int16_t yield_today;
  // 1 W, 0 .. 65534 W
  u_int16_t pv_power;
  // 0.1 A, 0 .. 51.0 A
  u_int16_t load_current : 9;
} __attribute__((packed));

// For the following devices: BMV, Inverter
// source: VE.Direct-Protocol-3.32.pdf
enum class VE_REG_ALARM_REASON : u_int16_t {
  NO_ALARM = 0x00,
  // Low Voltage
  LOW_VOLTAGE = 0x01,
  // High Voltage
  HIGH_VOLTAGE = 0x02,
  // Low SOC
  LOW_SOC = 0x04,
  // Low Starter Voltage
  LOW_STARTER_VOLTAGE = 0x08,
  // High Starter Voltage
  HIGH_STARTER_VOLTAGE = 0x10,
  // Low Temperature
  LOW_TEMPERATURE = 0x20,
  // High Temperature
  HIGH_TEMPERATURE = 0x40,
  // Mid Voltage
  MID_VOLTAGE = 0x80,
  // Overload
  OVERLOAD = 0x100,
  // DC-ripple
  DC_RIPPLE = 0x200,
  // Low V AC out
  LOW_V_AC_OUT = 0x400,
  // High V AC out
  HIGH_V_AC_OUT = 0x800,
  // Short Circuit
  SHORT_CIRCUIT = 0x1000,
  // BMS Lockout
  BMS_LOCKOUT = 0x2000,
  UNKNOWN_A = 0x4000,
  UNKNOWN_B = 0x8000,
};

// source: extra-manufacturer-data-2022-12-14.pdf
enum class VE_REG_BMV_AUX_INPUT : u_int8_t {
  VE_REG_DC_CHANNEL2_VOLTAGE = 0x0,
  VE_REG_BATTERY_MID_POINT_VOLTAGE = 0x1,
  VE_REG_BAT_TEMPERATURE = 0x2,
  NONE = 0x3,
};

struct VICTRON_BLE_RECORD_BATTERY_MONITOR {  // NOLINT(readability-identifier-naming,altera-struct-pack-align)
  // 1min, 0 .. 45.5days
  u_int16_t time_to_go;
  // 0.01 V, -327.68 .. 327.66 V
  int16_t battery_voltage;
  VE_REG_ALARM_REASON alarm_reason;
  union {
    // Aux voltage -327.68 .. 327.64 V
    int16_t aux_voltage;
    // Mid voltage 0 .. 655.34 V
    u_int16_t mid_voltage;
    // Temperature 0 .. 655.34 K
    u_int16_t temperature;
  } aux_input;
  VE_REG_BMV_AUX_INPUT aux_input_type : 2;
  // 0.001A, -4194 .. 4194 A
  int32_t battery_current : 22;
  // 0.1 Ah, -104,857 .. 0 Ah
  u_int32_t consumed_ah : 20;
  // 0.1% 0 .. 100.0%
  u_int16_t state_of_charge : 10;
} __attribute__((packed));

struct VICTRON_BLE_RECORD_INVERTER {  // NOLINT(readability-identifier-naming,altera-struct-pack-align)
  VE_REG_DEVICE_STATE device_state;
  VE_REG_ALARM_REASON alarm_reason;
  // 0.01 V, -327.68 .. 327.66 V
  int16_t battery_voltage;
  // 1 VA, 0 .. 65534 VA
  u_int16_t ac_apparent_power;
  // 0.01 V, 0 .. 327.66 V
  u_int16_t ac_voltage : 15;
  // 0.1 A, 0 .. 204.6 A
  u_int16_t ac_current : 11;
} __attribute__((packed));

// source: VE.Direct-Protocol-3.32.pdf
enum struct VE_REG_DEVICE_OFF_REASON_2 : u_int32_t {
  NOTHING = 0,
  // No input power
  NO_INPUT_POWER = 0x00000001,
  // Switched off (power switch)
  SWITCHED_OFF_SWITCH = 0x00000002,
  // Switched off (device mode register)
  SWITCHED_OFF_REGISTER = 0x00000004,
  // Remote input
  REMOTE_INPUT = 0x00000008,
  // Protection active
  PROTECTION = 0x00000010,
  // Paygo
  PAYGO = 0x00000020,
  // BMS
  BMS = 0x00000040,
  // Engine shutdown detection
  ENGINE = 0x00000080,
  // Analysing input voltage
  INPUT_VOLTATE = 0x00000100,
};

struct VICTRON_BLE_RECORD_DCDC_CONVERTER {  // NOLINT(readability-identifier-naming,altera-struct-pack-align)
  VE_REG_DEVICE_STATE device_state;
  VE_REG_CHR_ERROR_CODE charger_error;
  // 0.01 V, 0 .. 655.34 V
  u_int16_t input_voltage;
  // 0.01 V, -327.68 .. 327.66 V
  int16_t output_voltage;
  VE_REG_DEVICE_OFF_REASON_2 off_reason;
} __attribute__((packed));

// source: https://github.com/Fabian-Schmidt/esphome-victron_ble/issues/25
// source: https://www.victronenergy.com/upload/documents/Lithium_Battery_Smart/15958-Manual_Lithium_Smart_Battery-pdf-en.pdf - Page 43
// 0..15 - 4 bit
enum struct VE_REG_BALANCER_STATUS : u_int8_t {
  // unknown (need to charge once to 100%)
  UNKNOWN = 0x0,
  // Balanced / ok
  BALANCED = 0x1,
  // Balancing
  BALANCING = 0x2,
  // Imbalanced
  IMBALANCE = 0x3,
};

struct VICTRON_BLE_RECORD_SMART_LITHIUM {  // NOLINT(readability-identifier-naming,altera-struct-pack-align)
  // TODO
  u_int32_t bms_flags;
  // TODO
  u_int16_t SmartLithium_error;
  // 0.01 V, 2.60 .. 3.86 V
  // 0x00 ( 0) when cell voltage < 2.61V
  // 0x01 ( 1) when cell voltage == 2.61V
  // 0x7D (125) when cell voltage == 3.85V
  // 0x7E (126) when cell voltage > 3.85
  // 0x7F (127) when cell voltage is not available / unknown
  u_int16_t cell1 : 7;
  // 0.01 V, 2.60 .. 3.86 V - details see cell1
  u_int16_t cell2 : 7;
  // 0.01 V, 2.60 .. 3.86 V - details see cell1
  u_int16_t cell3 : 7;
  // 0.01 V, 2.60 .. 3.86 V - details see cell1
  u_int16_t cell4 : 7;
  // 0.01 V, 2.60 .. 3.86 V - details see cell1
  u_int16_t cell5 : 7;
  // 0.01 V, 2.60 .. 3.86 V - details see cell1
  u_int16_t cell6 : 7;
  // 0.01 V, 2.60 .. 3.86 V - details see cell1
  u_int16_t cell7 : 7;
  // 0.01 V, 2.60 .. 3.86 V - details see cell1
  u_int16_t cell8 : 7;
  // 0.01 V, 0 .. 40.94 V
  u_int16_t battery_voltage : 12;
  // 0 .. 15
  VE_REG_BALANCER_STATUS balancer_status : 4;
  // 1 °C,  -40 .. 86 °C - Temperature = Record value - 40
  u_int8_t battery_temperature : 7;
} __attribute__((packed));

struct VICTRON_BLE_RECORD_INVERTER_RS {  // NOLINT(readability-identifier-naming,altera-struct-pack-align)
  VE_REG_DEVICE_STATE device_state;
  VE_REG_CHR_ERROR_CODE charger_error;
  // 0.01 V, -327.68 .. 327.66 V
  int16_t battery_voltage;
  // 0.1 A, -3276.8 .. 3276.6 A
  int16_t battery_current;
  // 1 W, 0 .. 65,534 W 0
  u_int16_t pv_power;
  // 0.01 kWh, 0 .. 655.34 kWh
  u_int16_t yield_today;
  // 1 W, -32,768 .. 32,766 W
  int16_t ac_out_power;
} __attribute__((packed));

struct VICTRON_BLE_RECORD_SMART_BATTERY_PROTECT {  // NOLINT(readability-identifier-naming,altera-struct-pack-align)
  VE_REG_DEVICE_STATE device_state;
  // TODO
  u_int8_t output_state;
  VE_REG_CHR_ERROR_CODE error_code;
  VE_REG_ALARM_REASON alarm_reason;
  // Warnings always represent the current status of the measured parameter.
  // This is different than for alarm reason AR. AR remembers the reason what caused the inverter to switch off (active
  // protection) until it is switched on again.
  VE_REG_ALARM_REASON warning_reason;
  // 0.01 V, 327.68 .. 327.66 V
  int16_t input_voltage;
  // 0.01 V, 0 .. 655.34 V
  u_int16_t output_voltage;
  VE_REG_DEVICE_OFF_REASON_2 off_reason;
} __attribute__((packed));

struct VICTRON_BLE_RECORD_LYNX_SMART_BMS {  // NOLINT(readability-identifier-naming,altera-struct-pack-align)
  // TODO
  u_int8_t error;
  // 1 min, 0 .. 45.5 days
  u_int16_t ttg;
  // 0.01 V, -327.68 .. 327.66 V
  int16_t battery_voltage;
  // 0.1A -3276.8 .. 3276.6 A
  int16_t battery_current;
  // TODO
  u_int16_t io_status;
  // TODO
  u_int32_t warnings_alarms : 18;
  // 0.1%, 0 .. 100.0%
  u_int16_t soc : 10;
  // 0.1 Ah -104,857 .. 0 Ah - Consumed Ah = -Record value
  u_int32_t consumed_ah : 20;
  // 1 °C, -40 .. 86 °C - Temperature = Record value - 40
  u_int8_t temperature : 7;
} __attribute__((packed));

enum class VE_REG_AC_IN_ACTIVE : u_int8_t {
  // AC in 1
  AC_IN_1 = 0,
  // AC in 2
  AC_IN_2 = 1,
  // Not connected
  NOT_CONNECTED = 2,
  UNKNOWN = 3,
};

struct VICTRON_BLE_RECORD_MULTI_RS {  // NOLINT(readability-identifier-naming,altera-struct-pack-align)
  VE_REG_DEVICE_STATE device_state;
  VE_REG_CHR_ERROR_CODE charger_error;
  // 0.1 A, -3276.8 .. 3276.6 A
  int16_t battery_current;
  // 0.01 V, 0 .. 163.83 V
  u_int16_t battery_voltage : 14;
  VE_REG_AC_IN_ACTIVE active_ac_in : 2;
  // 1 W, -32,768 .. 32,766 W
  int16_t active_ac_in_power;
  // 1 W, -32,768 .. 32,766 W
  int16_t active_ac_out_power;
  // 1 W, 0 .. 65534 W
  u_int16_t pv_power;
  // 0.01 kWh, 0 .. 655.34 kWh
  u_int16_t yield_today;
} __attribute__((packed));

enum class VE_REG_ALARM_NOTIFICATION : u_int8_t {
  NO_ALARM = 0,
  WARNING = 1,
  ALARM = 2,
};

struct VICTRON_BLE_RECORD_VE_BUS {  // NOLINT(readability-identifier-naming,altera-struct-pack-align)
  VE_REG_DEVICE_STATE device_state;
  // TODO
  u_int16_t ve_bus_error;
  // 0.1 A, -3276.8 .. 3276.6 A
  int16_t battery_current;
  // 0.01 V, 0 .. 163.83 V
  u_int16_t battery_voltage : 14;
  VE_REG_AC_IN_ACTIVE active_ac_in : 2;
  // 1 W, -262,144 .. 262,142 W
  int32_t active_ac_in_power : 19;
  // 1 W, -262,144 .. 262,142 W
  int32_t active_ac_out_power : 19;
  VE_REG_ALARM_NOTIFICATION alarm : 2;
  // 1 °C, -40 .. 86 °C - Temperature = Record value - 40
  u_int8_t battery_temperature : 7;
  // 1 %, 0 .. 126 %
  u_int16_t soc : 7;
} __attribute__((packed));

struct VICTRON_BLE_RECORD_DC_ENERGY_METER {  // NOLINT(readability-identifier-naming,altera-struct-pack-align)
  // TODO
  int16_t bmv_monitor_mode;
  // 0.01 V, -327.68 .. 327.66 V
  int16_t battery_voltage;
  VE_REG_ALARM_REASON alarm_reason;
  union {
    // Aux voltage -327.68 .. 327.64 V
    int16_t aux_voltage;
    // Temperature 0 .. 655.34 K
    u_int16_t temperature;
  } aux_input;
  // VE_REG_BATTERY_MID_POINT_VOLTAGE not valid.
  VE_REG_BMV_AUX_INPUT aux_input_type : 2;
  //  0.001A, -4194 .. 4194 A
  int32_t battery_current : 22;
} __attribute__((packed));

struct VictronBleData {
  u_int16_t data_counter = 0;
  VICTRON_BLE_RECORD_TYPE record_type = VICTRON_BLE_RECORD_TYPE::TEST_RECORD;
  // Assumption: One device (class instance) will send only one type of record.
  union {
    VICTRON_BLE_RECORD_BATTERY_MONITOR battery_monitor;
    VICTRON_BLE_RECORD_SOLAR_CHARGER solar_charger;
    VICTRON_BLE_RECORD_INVERTER inverter;
    VICTRON_BLE_RECORD_DCDC_CONVERTER dcdc_converter;
    VICTRON_BLE_RECORD_SMART_LITHIUM smart_lithium;
    VICTRON_BLE_RECORD_INVERTER_RS inverter_rs;
    VICTRON_BLE_RECORD_SMART_BATTERY_PROTECT smart_battery_protect;
    VICTRON_BLE_RECORD_LYNX_SMART_BMS lynx_smart_bms;
    VICTRON_BLE_RECORD_MULTI_RS multi_rs;
    VICTRON_BLE_RECORD_VE_BUS ve_bus;
    VICTRON_BLE_RECORD_DC_ENERGY_METER dc_energy_meter;
    u_int8_t raw[VICTRON_ENCRYPTED_DATA_MAX_SIZE];
  } data;
};

class VictronBle : public esp32_ble_tracker::ESPBTDeviceListener, public Component {
 public:
  void dump_config() override;

  bool parse_device(const esp32_ble_tracker::ESPBTDevice &device) override;

  void set_address(uint64_t address) { this->address_ = address; }

  inline std::string address_str() const {
    if (this->address_ == 0) {
      return "";
    } else {
      return str_snprintf("%02X:%02X:%02X:%02X:%02X:%02X", 17, (uint8_t) (this->address_ >> 40) & 0xff,
                          (uint8_t) (this->address_ >> 32) & 0xff, (uint8_t) (this->address_ >> 24) & 0xff,
                          (uint8_t) (this->address_ >> 16) & 0xff, (uint8_t) (this->address_ >> 8) & 0xff,
                          (uint8_t) (this->address_ >> 0) & 0xff);
    }
  }

  void set_bindkey(std::array<uint8_t, 16> key) { this->bindkey_ = key; }

  void add_on_battery_monitor_message_callback(
      std::function<void(const VICTRON_BLE_RECORD_BATTERY_MONITOR *)> callback) {
    this->on_battery_monitor_message_callback_.add(std::move(callback));
  }
  void add_on_solar_charger_message_callback(std::function<void(const VICTRON_BLE_RECORD_SOLAR_CHARGER *)> callback) {
    this->on_solar_charger_message_callback_.add(std::move(callback));
  }
  void add_on_inverter_message_callback(std::function<void(const VICTRON_BLE_RECORD_INVERTER *)> callback) {
    this->on_inverter_message_callback_.add(std::move(callback));
  }
  void add_on_dcdc_converter_message_callback(std::function<void(const VICTRON_BLE_RECORD_DCDC_CONVERTER *)> callback) {
    this->on_dcdc_converter_message_callback_.add(std::move(callback));
  }
  void add_on_smart_lithium_message_callback(std::function<void(const VICTRON_BLE_RECORD_SMART_LITHIUM *)> callback) {
    this->on_smart_lithium_message_callback_.add(std::move(callback));
  }
  void add_on_inverter_rs_message_callback(std::function<void(const VICTRON_BLE_RECORD_INVERTER_RS *)> callback) {
    this->on_inverter_rs_message_callback_.add(std::move(callback));
  }
  void add_on_smart_battery_protect_message_callback(
      std::function<void(const VICTRON_BLE_RECORD_SMART_BATTERY_PROTECT *)> callback) {
    this->on_smart_battery_protect_message_callback_.add(std::move(callback));
  }
  void add_on_lynx_smart_bms_message_callback(std::function<void(const VICTRON_BLE_RECORD_LYNX_SMART_BMS *)> callback) {
    this->on_lynx_smart_bms_message_callback_.add(std::move(callback));
  }
  void add_on_multi_rs_message_callback(std::function<void(const VICTRON_BLE_RECORD_MULTI_RS *)> callback) {
    this->on_multi_rs_message_callback_.add(std::move(callback));
  }
  void add_on_ve_bus_message_callback(std::function<void(const VICTRON_BLE_RECORD_VE_BUS *)> callback) {
    this->on_ve_bus_message_callback_.add(std::move(callback));
  }
  void add_on_dc_energy_meter_message_callback(
      std::function<void(const VICTRON_BLE_RECORD_DC_ENERGY_METER *)> callback) {
    this->on_dc_energy_meter_message_callback_.add(std::move(callback));
  }
  void add_on_message_callback(std::function<void(const VictronBleData *)> callback) {
    this->on_message_callback_.add(std::move(callback));
  }

 protected:
  uint64_t address_;
  std::array<uint8_t, 16> bindkey_;

  VictronBleData last_package_{};

#define VICTRON_MESSAGE_STORAGE_BL(name) bool name##_updated_ = false;
#define VICTRON_MESSAGE_STORAGE_CB(name, type) CallbackManager<void(const type *)> on_##name##_message_callback_{};

  bool last_package_updated_ = false;
  VICTRON_MESSAGE_STORAGE_BL(battery_monitor)
  VICTRON_MESSAGE_STORAGE_BL(solar_charger)
  VICTRON_MESSAGE_STORAGE_BL(inverter)
  VICTRON_MESSAGE_STORAGE_BL(dcdc_converter)
  VICTRON_MESSAGE_STORAGE_BL(smart_lithium)
  VICTRON_MESSAGE_STORAGE_BL(inverter_rs)
  VICTRON_MESSAGE_STORAGE_BL(smart_battery_protect)
  VICTRON_MESSAGE_STORAGE_BL(lynx_smart_bms)
  VICTRON_MESSAGE_STORAGE_BL(multi_rs)
  VICTRON_MESSAGE_STORAGE_BL(ve_bus)
  VICTRON_MESSAGE_STORAGE_BL(dc_energy_meter)

  CallbackManager<void(const VictronBleData *)> on_message_callback_{};
  VICTRON_MESSAGE_STORAGE_CB(battery_monitor, VICTRON_BLE_RECORD_BATTERY_MONITOR)
  VICTRON_MESSAGE_STORAGE_CB(solar_charger, VICTRON_BLE_RECORD_SOLAR_CHARGER)
  VICTRON_MESSAGE_STORAGE_CB(inverter, VICTRON_BLE_RECORD_INVERTER)
  VICTRON_MESSAGE_STORAGE_CB(dcdc_converter, VICTRON_BLE_RECORD_DCDC_CONVERTER)
  VICTRON_MESSAGE_STORAGE_CB(smart_lithium, VICTRON_BLE_RECORD_SMART_LITHIUM)
  VICTRON_MESSAGE_STORAGE_CB(inverter_rs, VICTRON_BLE_RECORD_INVERTER_RS)
  VICTRON_MESSAGE_STORAGE_CB(smart_battery_protect, VICTRON_BLE_RECORD_SMART_BATTERY_PROTECT)
  VICTRON_MESSAGE_STORAGE_CB(lynx_smart_bms, VICTRON_BLE_RECORD_LYNX_SMART_BMS)
  VICTRON_MESSAGE_STORAGE_CB(multi_rs, VICTRON_BLE_RECORD_MULTI_RS)
  VICTRON_MESSAGE_STORAGE_CB(ve_bus, VICTRON_BLE_RECORD_VE_BUS)
  VICTRON_MESSAGE_STORAGE_CB(dc_energy_meter, VICTRON_BLE_RECORD_DC_ENERGY_METER)

#undef VICTRON_MESSAGE_STORAGE_BL
#undef VICTRON_MESSAGE_STORAGE_CB

  bool encrypt_message_(const u_int8_t *crypted_data, const u_int8_t crypted_len, u_int8_t encrypted_data[32],
                        const u_int8_t data_counter_lsb, const u_int8_t data_counter_msb);

  bool is_record_type_supported_(const VICTRON_BLE_RECORD_TYPE record_type, const u_int8_t crypted_len);
  void handle_record_(const VICTRON_BLE_RECORD_TYPE record_type, const u_int8_t encrypted_data[32]);
  void update();
};

}  // namespace victron_ble
}  // namespace esphome

#endif
