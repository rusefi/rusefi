/**
 * @file	io_pins.cpp
 * @brief	his file is about general input/output utility methods, not much EFI-specifics
 *
 *
 * @date Jan 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "global.h"
#include "io_pins.h"

#if EFI_PROD_CODE
#include "efi_gpio.h"
#include "drivers/gpio/gpio_ext.h"

#include "pin_repository.h"
#include "status_loop.h"
#include "engine_configuration.h"
#include "console_io.h"

EXTERN_ENGINE;

#if EFI_ENGINE_CONTROL
#include "main_trigger_callback.h"
#endif /* EFI_ENGINE_CONTROL */

static LoggingWithStorage logger("io_pins");

extern EnginePins enginePins;

bool efiReadPin(brain_pin_e pin) {
	if (brain_pin_is_onchip(pin))
		return palReadPad(getHwPort("readPin", pin), getHwPin("readPin", pin));
	#if (BOARD_EXT_GPIOCHIPS > 0)
		else if (brain_pin_is_ext(pin))
			return (gpiochips_readPad(pin) > 0);
	#endif

	/* incorrect pin */
	return false;
}

/**
 * This method would set an error condition if pin is already used
 */
void efiSetPadMode(const char *msg, brain_pin_e brainPin, iomode_t mode)
{
	bool wasUsed = brain_pin_markUsed(brainPin, msg);

	if (!wasUsed) {
		/*check if on-chip pin or external */
		if (brain_pin_is_onchip(brainPin)) {
			/* on-cip */
			ioportid_t port = getHwPort(msg, brainPin);
			ioportmask_t pin = getHwPin(msg, brainPin);
			/* paranoid */
			if (port == GPIO_NULL)
				return;

			palSetPadMode(port, pin, mode);
		}
		#if (BOARD_EXT_GPIOCHIPS > 0)
			else {
				gpiochips_setPadMode(brainPin, mode);
			}
		#endif
	}
}

void efiSetPadUnused(brain_pin_e brainPin)
{
	/* input with pull up, is it safe? */
	iomode_t mode = PAL_STM32_MODE_INPUT | PAL_STM32_PUPDR_PULLUP;

	if (brain_pin_is_onchip(brainPin)) {
		ioportid_t port = getHwPort("unused", brainPin);
		ioportmask_t pin = getHwPin("unused", brainPin);

		/* input with pull up, is it safe? */
		palSetPadMode(port, pin, mode);
	}
	#if (BOARD_EXT_GPIOCHIPS > 0)
		else {
			gpiochips_setPadMode(brainPin, mode);
		}
	#endif

	brain_pin_markUnused(brainPin);
}

iomode_t getInputMode(pin_input_mode_e mode) {
	switch (mode) {
	case PI_PULLUP:
		return PAL_MODE_INPUT_PULLUP;
	case PI_PULLDOWN:
		return PAL_MODE_INPUT_PULLDOWN;
	case PI_DEFAULT:
	default:
		return PAL_MODE_INPUT;
	}
}

#if HAL_USE_ICU
void efiIcuStart(const char *msg, ICUDriver *icup, const ICUConfig *config) {
	if (icup->state != ICU_STOP && icup->state != ICU_READY) {
		static char icuError[30];
		sprintf(icuError, "ICU already used %s", msg);
		firmwareError(CUSTOM_ERR_6679, icuError);
		return;
	}
	icuStart(icup, config);
}
#endif /* HAL_USE_ICU */

#else
bool efiReadPin(brain_pin_e pin) {
	return false;
}
#endif /* EFI_PROD_CODE */
