/**
 * @file	gpio/core.c
 * @brief	EFI-related GPIO code for external gpio chips
 *
 * @date Mar 8, 2019
 * @author Andrey Gusakov, (c) 2019
 */

#include "pch.h"
#include "gpio/gpio_ext.h"
#include "smart_gpio.h"

#define STRING2(x) #x
#define STRING(x) STRING2(x)
#pragma message(STRING(BOARD_EXT_GPIOCHIPS))

#if (BOARD_EXT_GPIOCHIPS > 0)

/*==========================================================================*/
/* Local definitions.														*/
/*==========================================================================*/

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
	GpioChip			*chip;
	const char			*name;
	/* optional names of each gpio */
	const char			**gpio_names;
};

static gpiochip chips[BOARD_EXT_GPIOCHIPS];

/*==========================================================================*/
/* Local functions.															*/
/*==========================================================================*/

/**
 * @return pointer to GPIO device for specified pin
 */
static gpiochip *gpiochip_find(brain_pin_e pin)
{
	for (int i = 0; i < BOARD_EXT_GPIOCHIPS; i++) {
		gpiochip *chip = &chips[i];

		if ((pin >= chip->base) && (pin < (chip->base + chip->size)))
			return chip;
	}

	return nullptr;
}

/*==========================================================================*/
/* Exported functions.														*/
/*==========================================================================*/

/**
 * return numeric part of EXTERNAL pin name.
 * @details
 */

int gpiochips_getPinOffset(brain_pin_e pin)
{
	gpiochip *chip = gpiochip_find(pin);

	if (chip)
		return pin - chip->base;

	return EFI_ERROR_CODE;
}


/**
 * @brief Get external chip name
 * @details return gpiochip name
 */

const char *gpiochips_getChipName(brain_pin_e pin) {
	gpiochip *chip = gpiochip_find(pin);

	if (chip)
		return chip->name;

	return nullptr;
}

/**
 * @brief Get external chip port name
 * @details return pin name or gpiochip name (if no pins names provided)
 */

const char *gpiochips_getPinName(brain_pin_e pin)
{
	int offset;
	gpiochip *chip = gpiochip_find(pin);

	if (chip) {
		offset = pin - chip->base;
		if ((chip->gpio_names) && (chip->gpio_names[offset]))
			return chip->gpio_names[offset];
	}

	return nullptr;
}

/**
 * @brief Register gpiochip
 * @details should be called from board file. Can be called before os ready.
 * All chips should be registered before gpiochips_init() called.
 * returns -1 in case of no free chips left
 * returns -1 in case of no ops provided, incorrect chip size
 * else returns chip base
 */

int gpiochip_register(brain_pin_e base, const char *name, GpioChip& gpioChip, size_t size)
{
	/* zero size? */
	if (!size)
		return -1;

	/* outside? */
	if (((size_t)base + size - 1 > BRAIN_PIN_LAST) || (base <= BRAIN_PIN_ONCHIP_LAST))
		return -1;

	/* check for overlap with other chips */
	for (int i = 0; i < BOARD_EXT_GPIOCHIPS; i++) {
		if (chips[i].base != 0) {
			#define in_range(a, b, c)	(((a) > (b)) && ((a) < (c)))
			if (in_range(base, chips[i].base, chips[i].base + chips[i].size))
				return -1;
			if (in_range(base + size, chips[i].base, chips[i].base + chips[i].size))
				return -1;
		}
	}

	gpiochip *chip = nullptr;

	/* find free gpiochip struct */
	for (int i = 0; i < BOARD_EXT_GPIOCHIPS; i++) {
		if (chips[i].base == 0) {
			chip = &chips[i];
			break;
		}
	}

	/* no free chips left */
	if (!chip) {
		return -1;
	}

	/* register chip */
	chip->name = name;
	chip->chip = &gpioChip;
	chip->base = base;
	chip->size = size;
	chip->gpio_names = nullptr;

	return base;
}


/**
 * @brief Unregister gpiochip
 * @details removes chip from list
 * TODO: call deinit?
 */

int gpiochip_unregister(brain_pin_e base)
{
	gpiochip *chip = gpiochip_find(base);

	if (!chip)
		return -1;

	/* gpiochip_find - returns chip if base within its range, but we need it to be base */
	if (chip->base != base)
		return -1;

	/* unregister chip */
	chip->name = nullptr;
	chip->chip = nullptr;
	chip->base = GPIO_UNASSIGNED;
	chip->size = 0;
	chip->gpio_names = nullptr;

	return 0;
}

/**
 * @brief Set pins names for registered gpiochip
 * @details should be called after chip registration. May be called several times
 * Names array size should be aqual to chip gpio count
 */

int gpiochips_setPinNames(brain_pin_e base, const char **names)
{
	gpiochip *chip = gpiochip_find(base);

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
	int pins_added = 0;

	for (int i = 0; i < BOARD_EXT_GPIOCHIPS; i++) {
		gpiochip *chip = &chips[i];

		if (!chip->base)
			continue;

		if (chip->chip->init() < 0) {
			/* remove chip if it fails to init */
			/* TODO: we will have a gap, is it ok? */
			chip->base = GPIO_UNASSIGNED;
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
/* this fuction uses iomode_t that is related to STM32 (or other MCU)
 * output modes. Use some common enums? */
int gpiochips_setPadMode(brain_pin_e pin, iomode_t mode)
{
	gpiochip *chip = gpiochip_find(pin);

	if (!chip)
		return -1;

	return chip->chip->setPadMode(pin - chip->base, mode);
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
	gpiochip *chip = gpiochip_find(pin);

	if (!chip)
		return -1;

	return chip->chip->writePad(pin - chip->base, value);
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
	gpiochip *chip = gpiochip_find(pin);

	if (!chip)
		return -1;

	return chip->chip->readPad(pin - chip->base);
}

/**
 * @brief Get diagnostic for given gpio
 * @details actual output value depent on gpiochip capabilities
 * returns -1 in case of pin not belong to any gpio chip
 * returns PIN_OK in case of chip does not support getting diagnostic
 * else return brain_pin_diag_e from gpiochip driver;
 */

brain_pin_diag_e gpiochips_getDiag(brain_pin_e pin)
{
	gpiochip *chip = gpiochip_find(pin);

	if (!chip)
		return PIN_INVALID;

	return chip->chip->getDiag(pin - chip->base);
}

/**
 * @brief Get total pin count allocated for external gpio chips.
 * @details Will also include unused pins for chips that was registered
 * but later fails to init.
 */

int gpiochips_get_total_pins(void)
{
	int i;
	int cnt = 0;

	for (i = 0; i < BOARD_EXT_GPIOCHIPS; i++) {
		gpiochip *chip = &chips[i];

		if (!chip->base)
			continue;

		cnt += chip->size;
	}

	return cnt;
}

#else /* BOARD_EXT_GPIOCHIPS > 0 */

int gpiochips_getPinOffset(brain_pin_e pin) {
	(void)pin;

	return -1;
}

const char *gpiochips_getChipName(brain_pin_e pin) {
	(void)pin;

	return nullptr;
}

const char *gpiochips_getPinName(brain_pin_e pin) {
	(void)pin;

	return nullptr;
}

int gpiochip_register(brain_pin_e base, const char *name, GpioChip&, size_t size)
{
	(void)base; (void)name; (void)size;

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
