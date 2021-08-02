/**
 * @file	io_pins.h
 * @brief	this file is about general input/output utility methods, not much EFI-specifics
 *
 * @date Jan 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"
#include "engine_ptr.h"

#define INITIAL_PIN_STATE -1
#define GPIO_NULL NULL

// mode >= 0  is always true since that's an unsigned
#define assertOMode(mode) { \
	efiAssertVoid(CUSTOM_INVALID_MODE_SETTING, mode <= OM_OPENDRAIN_INVERTED, "invalid pin_output_mode_e"); \
 }


EXTERNC void efiSetPadMode(const char *msg, brain_pin_e pin, iomode_t mode DECLARE_ENGINE_PARAMETER_SUFFIX);
EXTERNC void efiSetPadModeWithoutOwnershipAcquisition(const char *msg, brain_pin_e brainPin, iomode_t mode);
EXTERNC void efiSetPadUnused(brain_pin_e brainPin DECLARE_ENGINE_PARAMETER_SUFFIX);

#if EFI_GPIO_HARDWARE
EXTERNC bool efiReadPin(brain_pin_e pin);

EXTERNC iomode_t getInputMode(pin_input_mode_e mode);
#if HAL_USE_ICU
EXTERNC void efiIcuStart(const char *msg, ICUDriver *icup, const ICUConfig *config);
#endif /* HAL_USE_ICU */

#endif /* EFI_GPIO_HARDWARE */

void writePad(const char *msg, brain_pin_e pin, int bit);

#if ! EFI_PROD_CODE
#define BRAIN_PIN_COUNT (1 << 8 * sizeof(brain_pin_e))
extern bool mockPinStates[BRAIN_PIN_COUNT];

void setMockState(brain_pin_e pin, bool state);
#endif
