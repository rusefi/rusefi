/**
 * @file	io_pins.h
 *
 * @date Jan 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef STATUS_LEDS_H_
#define STATUS_LEDS_H_

#include "rusefi_enums.h"

#define GPIO_NULL NULL

#define NAMED_PIN_COUNT 24

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
 * Logical pins. See brain_pin_e for physical pins.
 */
typedef enum {
	SPARKOUT_1_OUTPUT,
	SPARKOUT_2_OUTPUT,
	SPARKOUT_3_OUTPUT,
	SPARKOUT_4_OUTPUT,
	SPARKOUT_5_OUTPUT,
	SPARKOUT_6_OUTPUT,
	SPARKOUT_7_OUTPUT,
	SPARKOUT_8_OUTPUT,
	SPARKOUT_9_OUTPUT,
	SPARKOUT_10_OUTPUT,
	SPARKOUT_11_OUTPUT,
	SPARKOUT_12_OUTPUT,

	INJECTOR_1_OUTPUT,
	INJECTOR_2_OUTPUT,
	INJECTOR_3_OUTPUT,
	INJECTOR_4_OUTPUT,
	INJECTOR_5_OUTPUT,
	INJECTOR_6_OUTPUT,
	INJECTOR_7_OUTPUT,
	INJECTOR_8_OUTPUT,
	INJECTOR_9_OUTPUT,
	INJECTOR_10_OUTPUT,
	INJECTOR_11_OUTPUT,
	INJECTOR_12_OUTPUT,

	IO_INVALID,

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

} io_pin_e;

#define IO_PIN_COUNT 24

void initPrimaryPins(void);
void initOutputPins(void);

io_pin_e getPinByName(const char *name);

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if EFI_GPIO
void turnAllPinsOff(void);
#else
#define turnAllPinsOff() {}
#endif
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STATUS_LEDS_H_ */
