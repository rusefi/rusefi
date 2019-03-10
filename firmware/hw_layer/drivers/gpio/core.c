/**
 * @file	gpio/core.c
 * @brief	EFI-related GPIO code for external gpio chips
 *
 * @date Mar 8, 2019
 * @author Andrey Gusakov, (c) 2019
 */

#include "global.h"
#include "drivers/gpio/gpio_ext.h"

#ifndef BOARD_EXT_GPIOCHIPS
	#define BOARD_EXT_GPIOCHIPS	0
#endif

#if (BOARD_EXT_GPIOCHIPS > 0)

/* fist available gpio number after on-chip gpios */
#define EXT_GPIOS_FIRST (GPIOH_15 + 1)
static size_t gpio_base_free = EXT_GPIOS_FIRST;

/* TODO: chnage array to list? */
struct gpiochip {
	size_t				base;
	size_t				size;
	struct gpiochip_ops	*ops;
	const char			*name;
	/* optional names of each gpio */
	const char			**gpio_names;
	/* private driver data passed to ops */
	void				*priv;
};

static struct gpiochip chips[BOARD_EXT_GPIOCHIPS];

static struct gpiochip *gpiochip_find(unsigned int pin)
{
	int i;

	for (i = 0; i < BOARD_EXT_GPIOCHIPS; i++) {
		struct gpiochip *chip = &chips[i];

		if (pin >= chip->base && pin < (chip->base + chip->size))
			return chip;
	}

	return NULL;
}

/**
 * return numeric part of EXTERNAL pin name.
 * TODO: external chips can have != pin count
 */
int getHwPinExt(unsigned int pin)
{
	struct gpiochip *chip = gpiochip_find(pin);

	if (chip)
		return pin - chip->base;

	return EFI_ERROR_CODE;
}

const char *portNameExt(unsigned int pin)
{
	struct gpiochip *chip = gpiochip_find(pin);

	if (chip) {
		if (chip->gpio_names)
			return chip->gpio_names[pin - chip->base];
		else
			return chip->name;
	}

	return NULL;
}

int gpiochip_register(const char *name, struct gpiochip_ops *ops, int base, size_t size, void *priv)
{
	int i;
	struct gpiochip *chip = NULL;

	if (base < 0)
		base = gpio_base_free;

	/* TODO: check is this chip gpio range overlaps something */

	/* find free gpiochip struct */
	for (i = 0; i < BOARD_EXT_GPIOCHIPS; i++) {
		if (chips[i].base == 0) {
			chip = &chips[i];
			break;
		}
	}

	/* no free chips left */
	if (chip == NULL)
		return -1;

	/* register chip */
	chip->name = name;
	chip->ops  = ops;
	chip->base = base;
	chip->size = size;
	chip->priv = priv;

	gpio_base_free += size;

	return (chip->base);
}

int gpiochips_init(void)
{
	int i;
	int ret = 0;
	int pins_added = 0;

	for (i = 0; i < BOARD_EXT_GPIOCHIPS; i++) {
		struct gpiochip *chip = &chips[i];

		if (!chip->base)
			continue;

		if ((chip->ops) && (chip->ops->init))
			ret = chip->ops->init(chip->priv);

		if (ret)
			break;
		else
			pins_added += chip->size;
	}

	if (ret)
		return ret;

	return pins_added;
}

int gpiochips_setPadMode(brain_pin_e pin, iomode_t mode)
{
	struct gpiochip *chip = gpiochip_find(pin);

	if (!chip)
		return -1;

	if ((chip->ops) && (chip->ops->setPadMode))
		return chip->ops->setPadMode(chip->priv, pin - chip->base, mode);

	return 0;
}

int gpiochips_setPad(brain_pin_e pin)
{
	struct gpiochip *chip = gpiochip_find(pin);

	if (!chip)
		return -1;

	if ((chip->ops) && (chip->ops->setPad))
		return chip->ops->setPad(chip->priv, pin - chip->base);

	return 0;
}

int gpiochips_clearPad(brain_pin_e pin)
{
	struct gpiochip *chip = gpiochip_find(pin);

	if (!chip)
		return -1;

	if ((chip->ops) && (chip->ops->clearPad))
		return chip->ops->clearPad(chip->priv, pin - chip->base);

	return 0;
}

int gpiochips_writePad(brain_pin_e pin, int value)
{
	if (value)
		return gpiochips_setPad(pin);
	else
		return gpiochips_clearPad(pin);

	return 0;
}

int gpiochips_get_total_pins(void)
{
	return (gpio_base_free - EXT_GPIOS_FIRST);
}

#else /* BOARD_EXT_GPIOCHIPS > 0 */

int getHwPinExt(unsigned int pin)
{
	(void)pin;

	return -1;
}

const char *portNameExt(unsigned int pin)
{
	(void)brainPin;

	return NULL;
}

int gpiochip_register(const char *name, struct gpiochip_ops *ops, int base, size_t size, void *priv)
{
	(void)name; (void)ops; (void)base; (void)size; (void)priv;

	return 0;
}

int gpiochips_init(void)
{
	return 0;
}

int gpiochips_get_total_pins(void)
{
	return 0;
}

#endif /* BOARD_EXT_GPIOCHIPS > 0 */
