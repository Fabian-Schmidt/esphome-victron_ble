#pragma once

// Using `enum` instead of  typedef` to allow using in `publish_state_` as parameter without overload error.

// 0.01 V, -327.68 .. 327.66 V
typedef int16_t battery_voltage_16bit_0_01V;

// 0.01 V, 0 .. 81.90 V
enum battery_voltage_13bit_0_01V_positiv : u_int16_t { };

// 0.1 A, 0 .. 204.6 A
enum battery_current_11bit_0_1A_positiv : u_int16_t { };

// 1 °C, -40 .. 86 °C - Temperature = Record value - 40
enum temperature_7bit : u_int8_t { };

// 0.1 A, 0 .. 51.0 A
enum ac_current_9bit_0_1A_positiv : u_int16_t { };