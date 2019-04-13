/**
 * @file	io_pins.h
 * @brief	his file is about general input/output utility methods, not much EFI-specifics
 *
 * @date Jan 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef IO_PINS_H_
#define IO_PINS_H_

#include "global.h"

#define INITIAL_PIN_STATE -1
#define GPIO_NULL NULL

// mode >= 0  is always true since that's an unsigned
#define assertOMode(mode) { \
	efiAssertVoid(CUSTOM_ERR_6578, mode <= OM_OPENDRAIN_INVERTED, "invalid pin_output_mode_e"); \
 }


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

#if EFI_GPIO_HARDWARE
void efiSetPadMode(const char *msg, brain_pin_e pin, iomode_t mode);

bool efiReadPin(brain_pin_e pin);

iomode_t getInputMode(pin_input_mode_e mode);
#if HAL_USE_ICU
void efiIcuStart(const char *msg, ICUDriver *icup, const ICUConfig *config);
#endif /* HAL_USE_ICU */
#endif /* EFI_GPIO_HARDWARE */


#endif /* IO_PINS_H_ */
