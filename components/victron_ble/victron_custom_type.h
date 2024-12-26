#pragma once

// Using `enum` instead of typedef` to allow using in `publish_state_` as parameter without overload error.

// 0.001A, -4194 .. 4194 A
enum vic_22bit_0_001 : int32_t;

// 0.1 Ah, -104,857 .. 0 Ah - Consumed Ah = -Record value
enum vic_20bit_0_1_negative : u_int32_t;

// 1 W, -262,144 .. 262,142 W
enum vic_19bit_1 : int32_t;

// 0.01 V, -327.68 .. 327.66 V
enum vic_16bit_0_01 : int16_t;
enum vic_16bit_0_01_noNAN : int16_t;

// 0.01 kWh, 0 .. 655.34 kWh
// 0.01 V, 0 .. 655.34 V
enum vic_16bit_0_01_positive : u_int16_t;

// 0.1 A, -3276.8 .. 3276.6 A
enum vic_16bit_0_1 : int16_t;

// 0.1 A, 0 .. 6553.4 A
enum vic_16bit_0_1_positive : u_int16_t;

// 1 W, -32,768 .. 32,766 W
enum vic_16bit_1 : int16_t;

// 1 W, 0 .. 65534 W
// 1 min, 0 .. 65534 min (~1092 hours, ~45.5 days)
// 1 VA, 0 .. 65534 VA
enum vic_16bit_1_positive : u_int16_t;

// 1 °C, 0 .. 655.34 K
enum vic_temperature_16bit : u_int16_t;

// 0.01 V, 0 .. 327.66 V
enum vic_15bit_0_01_positive : u_int16_t;

// 0.01 V, 0 .. 163.83 V
enum vic_14bit_0_01_positive : u_int16_t;

// 0.01 V, 0 .. 81.90 V
enum vic_13bit_0_01_positive : u_int16_t;

// 0.01 V, 0 .. 40.94 V
enum vic_12bit_0_01_positive : u_int16_t;

// 0.1 A, 0 .. 204.6 A
enum vic_11bit_0_1_positive : u_int16_t;

// 0.1% 0 .. 100.0%
enum vic_10bit_0_1_positive : u_int16_t;

// 0.1 A, 0 .. 51.0 A
enum vic_9bit_0_1_negative : u_int16_t;

// 0.1 A, 0 .. 51.0 A
enum vic_9bit_0_1_positive : u_int16_t;

// 0.01 V, 2.60 .. 3.86 V
// 0x00 ( 0) when cell voltage < 2.61V
// 0x01 ( 1) when cell voltage == 2.61V
// 0x7D (125) when cell voltage == 3.85V
// 0x7E (126) when cell voltage > 3.85
// 0x7F (127) when cell voltage is not available / unknown
enum vic_cell_7bit_0_01 : u_int16_t;

// 1 %, 0 .. 126 %
enum vic_7bit_1 : u_int16_t;

// 1 °C, -40 .. 86 °C - Temperature = Record value - 40
enum vic_temperature_7bit : u_int16_t;