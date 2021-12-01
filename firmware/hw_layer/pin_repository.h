/**
 * @file    pin_repository.h
 * @brief   I/O pin registry header
 *
 *
 *
 * @date Jan 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"
#include "io_pins.h"

#ifdef __cplusplus
#include "efi_gpio.h"

class PinRepository {
	public:
	/**
	 * Class constructors are a great way to have simple initialization sequence
	 */
	PinRepository();
	int totalPinsUsed = 0;
	const char *PIN_USED[BRAIN_PIN_TOTAL_PINS];
};

#endif /* __cplusplus */

bool isBrainPinValid(brain_pin_e brainPin);

void initPinRepository(void);
EXTERNC bool brain_pin_is_onchip(brain_pin_e brainPin);
EXTERNC bool brain_pin_is_ext(brain_pin_e brainPin);

/**
 * Usually high-level code would invoke efiSetPadMode, not this method directly
 */
EXTERNC bool brain_pin_markUsed(brain_pin_e brainPin, const char *msg);
/**
 * See also efiSetPadUnused
 */
EXTERNC void brain_pin_markUnused(brain_pin_e brainPin);
const char * getPinFunction(brain_input_pin_e brainPin);

#if EFI_PROD_CODE
/* For on-chip gpios only */
EXTERNC bool gpio_pin_markUsed(ioportid_t port, ioportmask_t pin, const char *msg);
EXTERNC void gpio_pin_markUnused(ioportid_t port, ioportmask_t pin);
#endif /* EFI_PROD_CODE*/

/* defined in ports/ */
int getPortPinIndex(ioportid_t port, ioportmask_t pin);
ioportid_t getBrainPinPort(brain_pin_e brainPin);
int getBrainPinIndex(brain_pin_e brainPin);
int brainPin_to_index(brain_pin_e brainPin);
unsigned int getBrainPinOnchipNum(void);
unsigned int getBrainPinTotalNum(void);

#ifdef __cplusplus
const char* & getBrainUsedPin(unsigned int idx);
#endif

