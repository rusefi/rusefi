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

struct gpiochip_ops {
	/* pin argument is pin number within gpio chip, not a global number */
	int (*setPadMode)(void *data, unsigned int pin, iomode_t mode);
	int (*writePad)(void *data, unsigned int pin, int value);
	int (*readPad)(void *data, unsigned int pin);
	brain_pin_diag_e (*getDiag)(void *data, unsigned int pin);
	int (*init)(void *data);
	int (*deinit)(void *data);
};

int gpiochips_getPinOffset(brain_pin_e pin);
const char *gpiochips_getChipName(brain_pin_e pin);
const char *gpiochips_getPinName(brain_pin_e pin);

/* register/unregister GPIO chip */
int gpiochip_register(brain_pin_e base, const char *name, gpiochip_ops *ops, size_t size, void *priv);
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
