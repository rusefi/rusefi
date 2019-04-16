/**
 * @file	gpio/core.c
 * @brief	EFI-related GPIO code for external gpio chips
 *
 * @date Mar 8, 2019
 * @author Andrey Gusakov, (c) 2019
 */

#include "global.h"
#include "gpio/gpio_ext.h"

#define STRING2(x) #x
#define STRING(x) STRING2(x)
#pragma message(STRING(BOARD_EXT_GPIOCHIPS))

#if (BOARD_EXT_GPIOCHIPS > 0)

/*==========================================================================*/
/* Local definitions.														*/
/*==========================================================================*/

/* fist available gpio number after on-chip gpios */
#define EXT_GPIOS_FIRST (BRAIN_PIN_LAST_ONCHIP + 1)
static brain_pin_e gpio_base_free = EXT_GPIOS_FIRST;

/*==========================================================================*/
/* Exported variables.														*/
/*==========================================================================*/

/*==========================================================================*/
/* Local variables and types.												*/
/*==========================================================================*/

/* TODO: chnage array to list? */
struct gpiochip {
	brain_pin_e			base;
	size_t				size;
	struct gpiochip_ops	*ops;
	const char			*name;
	/* optional names of each gpio */
	const char			**gpio_names;
	/* private driver data passed to ops */
	void				*priv;
};

static struct gpiochip chips[BOARD_EXT_GPIOCHIPS];

/*==========================================================================*/
/* Local functions.															*/
/*==========================================================================*/

static struct gpiochip *gpiochip_find(unsigned int pin)
{
	int i;

	for (i = 0; i < BOARD_EXT_GPIOCHIPS; i++) {
		struct gpiochip *chip = &chips[i];

		if ((pin >= chip->base) && (pin < (chip->base + chip->size)))
			return chip;
	}

	return NULL;
}

/*==========================================================================*/
/* Exported functions.														*/
/*==========================================================================*/

/**
 * return numeric part of EXTERNAL pin name.
 * @details
 */

int gpiochips_getPinOffset(unsigned int pin)
{
	struct gpiochip *chip = gpiochip_find(pin);

	if (chip)
		return pin - chip->base;

	return EFI_ERROR_CODE;
}


/**
 * @brief Get external chip name
 * @details return gpiochip name
 */

const char *gpiochips_getChipName(unsigned int pin)
{
	struct gpiochip *chip = gpiochip_find(pin);

	if (chip)
		return chip->name;

	return NULL;
}

/**
 * @brief Get external chip port name
 * @details return pin name or gpiochip name (if no pins names provided)
 */

const char *gpiochips_getPinName(unsigned int pin)
{
	int offset;
	struct gpiochip *chip = gpiochip_find(pin);

	if (chip) {
		offset = pin - chip->base;
		if ((chip->gpio_names) && (chip->gpio_names[offset]))
			return chip->gpio_names[offset];
		else
			return chip->name;
	}

	return NULL;
}

void gpiochip_use_gpio_base(int size) {
	gpio_base_free += size;
}


/**
 * @brief Register gpiochip
 * @details should be called from board file. Can be called before os ready.
 * All chips should be registered before gpiochips_init() called.
 * returns -1 in case of no free chips left
 * returns -1 in case of no ops provided, incorrect chip size
 * else returns chip base
 */

int gpiochip_register(const char *name, struct gpiochip_ops *ops, size_t size, void *priv)
{
	int i;
	struct gpiochip *chip = NULL;

	/* no ops provided, zero size? */
	if ((!ops) || (!size))
		return -1;

	if ((!ops->writePad) && (!ops->readPad))
		return -1;

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
	chip->base = gpio_base_free;
	chip->size = size;
	chip->priv = priv;

	gpiochip_use_gpio_base(size);

	return (chip->base);
}

/**
 * @brief Set pins names for registered gpiochip
 * @details should be called after chip registration. May be called several times
 * Names array size should be aqual to chip gpio count
 */

int gpiochips_setPinNames(brain_pin_e pin, const char **names)
{
	struct gpiochip *chip = gpiochip_find(pin);

	if (!chip)
		return -1;

	chip->gpio_names = names;

	return 0;
}

/**
 * @brief Init all registered gpiochips
 * @details will call gpiochip init ops for all registered chips
 * calles when OS is ready, so gpiochip can start threads, use drivers and so on.
 */

int gpiochips_init(void)
{
	int i;
	int ret = -1;
	int pins_added = 0;

	for (i = 0; i < BOARD_EXT_GPIOCHIPS; i++) {
		struct gpiochip *chip = &chips[i];

		if (!chip->base)
			continue;

		if (chip->ops->init)
			ret = chip->ops->init(chip->priv);

		if (ret < 0) {
			/* remove chip if it fails to init */
			/* TODO: we will have a gap, is it ok? */
			chip->base = 0;
		} else {
			pins_added += chip->size;
		}
	}

	return pins_added;
}

/**
 * @brief Set pin mode of gpiochip
 * @details set pad mode for given pin.
 * return -1 if driver does not implemet setPadMode ops
 * else return value from gpiochip driver.
 */

int gpiochips_setPadMode(brain_pin_e pin, int mode)
{
	struct gpiochip *chip = gpiochip_find(pin);

	if (!chip)
		return -1;

	if ((chip->ops->setPadMode))
		return chip->ops->setPadMode(chip->priv, pin - chip->base, mode);

	return -1;
}

/**
 * @brief Set value to gpio of gpiochip
 * @details actual output value depent on current gpiochip implementation
 * for smart switch inactive supposed to be closed switch (no current flows)
 * returns -1 in case of pin not belong to any gpio chip
 * returns -1 in case of chip does not support seting output value (input only)
 * else return value from gpiochip driver;
 */

int gpiochips_writePad(brain_pin_e pin, int value)
{
	struct gpiochip *chip = gpiochip_find(pin);

	if (!chip)
		return -1;

	if (chip->ops->writePad)
		return chip->ops->writePad(chip->priv, pin - chip->base, value);

	return -1;
}

/**
 * @brief Get value to gpio of gpiochip
 * @details actual input value depent on current gpiochip implementation
 * returns -1 in case of pin not belong to any gpio chip
 * returns -1 in case of chip does not support getting output value (output only)
 * else return value from gpiochip driver;
 */

int gpiochips_readPad(brain_pin_e pin)
{
	struct gpiochip *chip = gpiochip_find(pin);

	if (!chip)
		return -1;

	if (chip->ops->readPad)
		return chip->ops->readPad(chip->priv, pin - chip->base);

	return -1;
}

/**
 * @brief Get total pin count allocated for external gpio chips.
 * @details Will also include unused pins for chips that was registred
 * but later fails to init.
 */

int gpiochips_get_total_pins(void)
{
	return (gpio_base_free - EXT_GPIOS_FIRST);
}

#else /* BOARD_EXT_GPIOCHIPS > 0 */

int gpiochips_getPinOffset(unsigned int pin)
{
	(void)pin;

	return -1;
}

const char *gpiochips_getChipName(unsigned int pin)
{
	(void)pin;

	return NULL;
}

const char *gpiochips_getPinName(unsigned int pin)
{
	(void)pin;

	return NULL;
}

void gpiochip_use_gpio_base(int size)
{
	(void)size;
}

int gpiochip_register(const char *name, struct gpiochip_ops *ops, size_t size, void *priv)
{
	(void)name; (void)ops; (void)size; (void)priv;

	return 0;
}

int gpiochips_setPinNames(brain_pin_e pin, const char **names)
{
	(void)pin; (void)names;

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
