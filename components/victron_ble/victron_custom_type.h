#pragma once

// 0.01 V, -327.68 .. 327.66 V
typedef int16_t battery_voltage_16bit_0_01V;

// 0.01 V, 0 .. 81.90 V
typedef u_int16_t battery_voltage_13bit_0_01V_positiv;

// 0.1 A, 0 .. 204.6 A
typedef u_int16_t battery_current_11bit_0_1A_positiv;

// 1 °C, -40 .. 86 °C - Temperature = Record value - 40
typdef u_int8_t temperature_7bit;

// 0.1 A, 0 .. 51.0 A
typedef u_int16_t ac_current_9bit_0_1A_positiv;