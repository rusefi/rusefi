/**
 * @file	io_pins.h
 *
 * @date Jan 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef IO_PINS_H_
#define IO_PINS_H_

#include "rusefi_enums.h"

#define GPIO_NULL NULL

typedef enum {
	INJECTOR_1,
	INJECTOR_2,
	INJECTOR_3,
	INJECTOR_4,
	INJECTOR_5,
	INJECTOR_6,
	INJECTOR_7,
	INJECTOR_8,
	INJECTOR_9_,
	INJECTOR_10,
	INJECTOR_11,
	INJECTOR_12,

	INJECTOR_NONE,
} injector_channel_e;


/**
 * these seven segment display pins are related to unused external tachometer code
 * I still have the hardware so maybe one day I will fix it, but for now it's just dead code
 * See https://www.youtube.com/watch?v=YYiHoN6MBqE
 * todo: this should be re-implemented in a smarter way with some sort of multiplexing anyway
 */
/* digit 1 */
//	LED_HUGE_0, // B2
//	LED_HUGE_1,
//	LED_HUGE_2,
//	LED_HUGE_3,
//	LED_HUGE_4,
//	LED_HUGE_5,
//	LED_HUGE_6,
//	/* digit 2 */
//	LED_HUGE_7,
//	LED_HUGE_8,
//	LED_HUGE_9, // E15
//	LED_HUGE_10,
//	LED_HUGE_11,
//	LED_HUGE_12,
//	LED_HUGE_13,
//	/* digit 3 */
//	LED_HUGE_14,
//	LED_HUGE_15,
//	LED_HUGE_16,
//	LED_HUGE_17,
//	LED_HUGE_18,
//	LED_HUGE_19,
//	LED_HUGE_20,

void initPrimaryPins(void);
void initOutputPins(void);

#if EFI_GPIO
void turnAllPinsOff(void);
#else
#define turnAllPinsOff() {}
#endif

#endif /* IO_PINS_H_ */
