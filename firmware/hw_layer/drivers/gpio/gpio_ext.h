/*
 * @file gpio_ext.h
 *
 * Abstraction layer definitions for extrenal gpios
 *
 * @date Mar 8, 2019
 * @author Andrey Gusakov, (c) 2019
 */

#ifndef GPIO_EXT_H_
#define GPIO_EXT_H_

#if EFI_PROD_CODE
#include "board.h"
#endif

#include "rusefi_enums.h"

/*==========================================================================*/
/* Checks																	*/
/*==========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

struct gpiochip_ops {
	int (*setPadMode)(void *data, brain_pin_e pin, int mode);
	int (*writePad)(void *data, brain_pin_e pin, int value);
	int (*readPad)(void *data, brain_pin_e pin);
	int (*getDiag)(void *data, brain_pin_e pin);
	int (*init)(void *data);
	int (*deinit)(void *data);
};

int gpiochips_getPinOffset(brain_pin_e pin);
const char *gpiochips_getChipName(brain_pin_e pin);
const char *gpiochips_getPinName(brain_pin_e pin);

/* register GPIO chip */
int gpiochip_register(const char *name, struct gpiochip_ops *ops, size_t size, void *priv);

void gpiochip_use_gpio_base(int size);

/* Set individual names for pins */
int gpiochips_setPinNames(brain_pin_e pin, const char **names);

/* gpio extenders subsystem init */
int gpiochips_init(void);

int gpiochips_setPadMode(brain_pin_e pin, int mode);
int gpiochips_writePad(brain_pin_e pin, int value);
int gpiochips_readPad(brain_pin_e pin);

/* return total number of external gpios */
int gpiochips_get_total_pins(void);

#ifdef __cplusplus
}
#endif

#endif /* GPIO_EXT_H_ */
