/*
 * gpio_ext.h
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

#ifndef BOARD_EXT_GPIOCHIPS
	#define BOARD_EXT_GPIOCHIPS	0
#endif

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

struct gpiochip_ops {
	int (*setPadMode)(void *data, unsigned int pin, int mode);
	int (*writePad)(void *data, unsigned int pin, int value);
	int (*readPad)(void *data, unsigned int pin);
	int (*getDiag)(void *data, unsigned int pin);
	int (*init)(void *data);
	int (*deinit)(void *data);
};

int getHwPinExt(unsigned int pin);
const char *portNameExt(unsigned int pin);

/* register gpio cgip */
int gpiochip_register(const char *name, struct gpiochip_ops *ops, size_t size, void *priv);

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
