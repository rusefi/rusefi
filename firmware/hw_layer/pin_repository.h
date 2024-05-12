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
#include "plain_pin_repository.h"

bool isBrainPinValid(Gpio brainPin);

void initPinRepository();
bool brain_pin_is_onchip(Gpio brainPin);
bool brain_pin_is_ext(Gpio brainPin);
void pinDiag2string(char *buffer, size_t size, brain_pin_diag_e pin_diag);

/**
 * Usually high-level code would invoke efiSetPadMode, not this method directly
 */
bool brain_pin_markUsed(brain_pin_e brainPin, const char *msg);

const char * getPinFunction(brain_input_pin_e brainPin);

#if EFI_PROD_CODE
/* For on-chip gpios only */
bool gpio_pin_markUsed(ioportid_t port, ioportmask_t pin, const char *msg);
void gpio_pin_markUnused(ioportid_t port, ioportmask_t pin);
#endif /* EFI_PROD_CODE*/

/* defined in ports/ */
int getPortPinIndex(ioportid_t port, ioportmask_t pin);
ioportid_t getBrainPinPort(brain_pin_e brainPin);
int getBrainPinIndex(Gpio brainPin);
size_t getBrainPinOnchipNum();
const char *hwPortname(Gpio brainPin);
const char *hwPhysicalPinName(Gpio brainPin);
// the main usage for human-readable board-specific pin reference is convenience of error messages in case of pin conflict.
const char * getBoardSpecificPinName(Gpio brainPin);

void debugBrainPin(char *buffer, size_t size, brain_pin_e brainPin);

const char* & getBrainUsedPin(size_t idx);
