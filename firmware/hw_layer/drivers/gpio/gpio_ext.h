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

#include <hal.h>
#include "rusefi_enums.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

struct gpiochip_ops {
	int (*setPadMode)(void *data, unsigned int sspad, iomode_t mode);
	int (*setPad)(void *data, unsigned int sspad);
	int (*clearPad)(void *data, unsigned int sspad);
	int (*getPad)(void *data, unsigned int sspad);
	int (*getDiag)(void *data, unsigned int sspad);
	int (*init)(void *data);
	int (*deinit)(void *data);
};

int getHwPinExt(unsigned int pin);
const char *portNameExt(unsigned int pin);

/* register gpio cgip */
int gpiochip_register(const char *name, struct gpiochip_ops *ops, int base, size_t size, void *priv);

/* gpio extenders subsystem init */
int gpiochips_init(void);

int gpiochips_setPadMode(brain_pin_e pin, iomode_t mode);
int gpiochips_setPad(brain_pin_e pin);
int gpiochips_clearPad(brain_pin_e pin);
int gpiochips_writePad(brain_pin_e pin, int value);

/* return total number of external gpios */
int gpiochips_get_total_pins(void);

#ifdef __cplusplus
}
#endif

#endif /* GPIO_EXT_H_ */
