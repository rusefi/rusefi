/*
 * @file gpio_ext.h
 *
 * Abstraction layer definitions for extrenal gpios
 *
 * @date Mar 8, 2019
 * @author Andrey Gusakov, (c) 2019
 */

#pragma once

#if EFI_PROD_CODE
#include "board.h"
#endif /* EFI_PROD_CODE */

#include "rusefi_enums.h"

/*==========================================================================*/
/* Checks																	*/
/*==========================================================================*/

struct GpioChip {
	virtual int init() = 0;

	// These functions need not be implemented if not supported by the particular chip.
	/* pin argument is pin number within gpio chip, not a global number */
	virtual int setPadMode(size_t /*pin*/, iomode_t /*mode*/) { return -1; }
	virtual int writePad(size_t /*pin*/, int /*value*/) { return -1; }
	virtual int readPad(size_t /*pin*/) { return -1; }
	virtual brain_pin_diag_e getDiag(size_t /*pin*/) { return PIN_OK; }
	virtual int deinit() { return 0; }
};

int gpiochips_getPinOffset(brain_pin_e pin);
const char *gpiochips_getChipName(brain_pin_e pin);
const char *gpiochips_getPinName(brain_pin_e pin);

/* register/unregister GPIO chip */
int gpiochip_register(brain_pin_e base, const char *name, GpioChip& chip, size_t size);
int gpiochip_unregister(brain_pin_e base);

/* Set individual names for pins */
int gpiochips_setPinNames(brain_pin_e base, const char **names);

/* gpio extenders subsystem init */
int gpiochips_init(void);

int gpiochips_setPadMode(brain_pin_e pin, iomode_t mode);
int gpiochips_writePad(brain_pin_e pin, int value);
int gpiochips_readPad(brain_pin_e pin);
brain_pin_diag_e gpiochips_getDiag(brain_pin_e pin);

/* return total number of external gpios */
int gpiochips_get_total_pins(void);
