/*
 * @file rusefi_hw_enums.h
 *
 * @date Jun 2, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#ifndef RUSEFI_HW_ENUMS_H_
#define RUSEFI_HW_ENUMS_H_

// todo: migrate/unify with pin_output_mode_e? rename? something is messy here
// this enum is currently only used for SPI pins
typedef enum __attribute__ ((__packed__)) {
// todo: here we have a rare example of stm32-specific enum, todo: make this not stm32 specific?
	PO_DEFAULT = 0,
	PO_OPENDRAIN = 4, // PAL_STM32_OTYPE_OPENDRAIN
	PO_PULLUP = 32, // PAL_STM32_PUDR_PULLUP
	PO_PULLDOWN = 64 // PAL_STM32_PUPDR_PULLDOWN
} pin_mode_e;

#endif /* RUSEFI_HW_ENUMS_H_ */
