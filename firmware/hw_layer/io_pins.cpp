/**
 * @file	io_pins.cpp
 * @brief	his file is about general input/output utility methods, not much EFI-specifics
 *
 *
 * @date Jan 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include <board.h>
#include "main.h"
#include "io_pins.h"
#include "efiGpio.h"

#include "pin_repository.h"
#include "status_loop.h"
#include "engine_configuration.h"
#include "console_io.h"

EXTERN_ENGINE;

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
#include "main_trigger_callback.h"
#endif /* EFI_ENGINE_CONTROL */

static LoggingWithStorage logger("io_pins");

extern EnginePins enginePins;

#if defined(STM32F4XX)
static ioportid_t PORTS[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH };
#else
static ioportid_t PORTS[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOF};
#endif

ioportid_t getHwPort(brain_pin_e brainPin) {
	if (brainPin == GPIO_UNASSIGNED)
		return GPIO_NULL;
	if (brainPin > GPIO_UNASSIGNED || brainPin < 0) {
		firmwareError(CUSTOM_ERR_INVALID_PIN, "Invalid brain_pin_e: %d", brainPin);
		return GPIO_NULL;
	}
	return PORTS[brainPin / PORT_SIZE];
}

ioportmask_t getHwPin(brain_pin_e brainPin) {
	if (brainPin == GPIO_UNASSIGNED)
		return EFI_ERROR_CODE;
	if (brainPin > GPIO_UNASSIGNED || brainPin < 0) {
		firmwareError(CUSTOM_ERR_INVALID_PIN, "Invalid brain_pin_e: %d", brainPin);
		return EFI_ERROR_CODE;
	}
	return brainPin % PORT_SIZE;
}

bool efiReadPin(brain_pin_e pin) {
	return palReadPad(getHwPort(pin), getHwPin(pin));
}

/**
 * This method would set an error condition if pin is already used
 */
void efiSetPadMode(const char *msg, brain_pin_e brainPin, iomode_t mode) {
	ioportid_t port = getHwPort(brainPin);
	ioportmask_t pin = getHwPin(brainPin);

	if (port == GPIO_NULL) {
		return;
	}

	scheduleMsg(&logger, "%s on %s%d", msg, portname(port), pin);

	bool wasUsed = markUsed(port, pin, msg);
	if (wasUsed) {
		return;
	}

	palSetPadMode(port, pin, mode);
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
void efiIcuStart(ICUDriver *icup, const ICUConfig *config) {
	  efiAssertVoid((icup->state == ICU_STOP) || (icup->state == ICU_READY),
	              "input already used?");

	icuStart(icup, config);
}
#endif
