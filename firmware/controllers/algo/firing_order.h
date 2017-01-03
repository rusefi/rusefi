/*
 * @file firing_order.h
 *
 * @date Jul 20, 2016
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef CONTROLLERS_ALGO_FIRING_ORDER_H_
#define CONTROLLERS_ALGO_FIRING_ORDER_H_

// for now I want most enums to be 32 bit integers. At some point maybe we will make the one-byte
// this is about offsets and sizes in TunerStudio
#define ENUM_32_BITS 2000000000

typedef enum {
	FO_1 = 0,

	// 2 cylinder
	FO_1_2 = 8,

	// 3 cylinder
	FO_1_2_3 = 10,

	// 4 cylinder
	FO_1_3_4_2 = 1, // typical inline 4
	FO_1_2_4_3 = 2,
	FO_1_3_2_4 = 3, // for example horizontally opposed engine
	// 8 cylinder
	FO_1_8_4_3_6_5_7_2 = 5,
	FO_1_8_7_2_6_5_4_3 = 11,
	FO_1_5_4_2_6_3_7_8 = 12,
	// 5 cylinder
	FO_1_2_4_5_3 = 6,

	// 6 cylinder
	FO_1_5_3_6_2_4 = 4,
	FO_1_4_2_5_3_6 = 7,
	FO_1_2_3_4_5_6 = 9,

	Force_4b_firing_order = ENUM_32_BITS,
} firing_order_e;


#endif /* CONTROLLERS_ALGO_FIRING_ORDER_H_ */
