/*
 * @file firing_order.h
 *
 * See also FiringOrderTSLogic.java
 *
 * @date Jul 20, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "rusefi_enums.h"

#pragma once

typedef enum {
	FO_1 = 0,

	// 2 cylinder
	FO_1_2 = 8,

	// 3 cylinder
	FO_1_2_3 = 10,
	FO_1_3_2 = 24,

	// 4 cylinder
	FO_1_3_4_2 = 1, // typical inline 4
	FO_1_2_4_3 = 2,
	FO_1_3_2_4 = 3, // for example horizontally opposed engine
	FO_1_4_3_2 = 17, // for example VW aircooled boxer engine

	// 5 cylinder
	FO_1_2_4_5_3 = 6,

	// 6 cylinder
	FO_1_5_3_6_2_4 = 4, // VAG VR6
	FO_1_4_2_5_3_6 = 7,
	FO_1_2_3_4_5_6 = 9, // all Nissan v6
	FO_1_6_3_2_5_4 = 13, // EG33
	FO_1_4_3_6_2_5 = 27, // VAG v6 different from VAG VR6

	// todo: one day we shall support 7 cylinder radial, probably not before one actually approaches us

	// 8 cylinder
	FO_1_8_4_3_6_5_7_2 = 5,
	FO_1_8_7_2_6_5_4_3 = 11,
	FO_1_5_4_2_6_3_7_8 = 12,
	FO_1_2_7_8_4_5_6_3 = 19,
	FO_1_3_7_2_6_5_4_8 = 20, // Ford 5.0 HO and 351W
	FO_1_2_3_4_5_6_7_8 = 25, // linearly incrementing, for V8 testing
	FO_1_5_4_8_6_3_7_2 = 26, // Audi 4.2 40v V8
	FO_1_8_7_3_6_5_4_2 = 28, // VH41DE (Japaneese Y32 Variant)

	// 9 cylinder - for instance radial :)
	// PS: Matt says that 9cyl is actually 1-3-5-7-9-2-4-6-8 or 1-8-6-4-2-9-7-5-3 for reverse rotation
	FO_1_2_3_4_5_6_7_8_9 = 21,

	// 10 cylinder
	FO_1_10_9_4_3_6_5_8_7_2 = 14, // dodge and viper ram v10

	// 12 cylinder
	FO_1_7_5_11_3_9_6_12_2_8_4_10 = 15, // bmw M70 & M73 etc
	FO_1_7_4_10_2_8_6_12_3_9_5_11 = 16, // lamborghini, typical rusEfi use-case
	FO_1_12_5_8_3_10_6_7_2_11_4_9 = 18, // VAG W12
	FO_1_2_3_4_5_6_7_8_9_10_11_12 = 23, // mostly for hardware testing purposes


	// 16 cylinder
	// unfortunately not supported by default firmware because MAX_CYLINDER_COUNT=12 by default
	FO_1_14_9_4_7_12_15_6_13_8_3_16_11_2_5_10 = 22, // WR16

	// max used = 28

	Force_4b_firing_order = ENUM_32_BITS,
} firing_order_e;
