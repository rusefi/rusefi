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
#include "efi_gpio.h"

struct PinRepository {
public:
	PinRepository();
	const char *PIN_USED[BRAIN_PIN_TOTAL_PINS];
};

bool isBrainPinValid(brain_pin_e brainPin);

void initPinRepository(void);
bool brain_pin_is_onchip(brain_pin_e brainPin);
bool brain_pin_is_ext(brain_pin_e brainPin);
void pinDiag2string(char *buffer, size_t size, brain_pin_diag_e pin_diag);

/**
 * Usually high-level code would invoke efiSetPadMode, not this method directly
 */
bool brain_pin_markUsed(brain_pin_e brainPin, const char *msg);
/**
 * See also efiSetPadUnused
 */
void brain_pin_markUnused(brain_pin_e brainPin);
const char * getPinFunction(brain_input_pin_e brainPin);

#if EFI_PROD_CODE
/* For on-chip gpios only */
bool gpio_pin_markUsed(ioportid_t port, ioportmask_t pin, const char *msg);
void gpio_pin_markUnused(ioportid_t port, ioportmask_t pin);
#endif /* EFI_PROD_CODE*/

/* defined in ports/ */
int getPortPinIndex(ioportid_t port, ioportmask_t pin);
ioportid_t getBrainPinPort(brain_pin_e brainPin);
int getBrainPinIndex(brain_pin_e brainPin);
int brainPin_to_index(brain_pin_e brainPin);
unsigned int getBrainPinOnchipNum(void);
unsigned int getBrainPinTotalNum(void);
const char *hwPortname(brain_pin_e brainPin);
// the main usage for human-readable board-specific pin reference is convenience of error messages in case of pin conflict.
const char * getBoardSpecificPinName(brain_pin_e brainPin);

const char* & getBrainUsedPin(unsigned int idx);
