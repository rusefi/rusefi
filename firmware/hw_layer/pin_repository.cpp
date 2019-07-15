/**
 * @file    pin_repository.cpp
 * @brief   I/O pin registry code
 *
 * This job of this class is to make sure that we are not using same hardware pin for two
 * different purposes.
 *
 * @date Jan 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "global.h"
#if EFI_PROD_CODE
#include "os_access.h"
#include "pin_repository.h"
#include "eficonsole.h"
#include "memstreams.h"
#include "drivers/gpio/gpio_ext.h"

#ifndef BOARD_EXT_PINREPOPINS
	#define BOARD_EXT_PINREPOPINS 0
#endif

static int initialized = FALSE;

static LoggingWithStorage logger("pin repos");
static int totalPinsUsed = 0;

static int brainPin_to_index(brain_pin_e brainPin)
{
	int index;

	if (brainPin < GPIOA_0)
		return -1;

	index = brainPin - GPIOA_0;

	/* if index outside array boundary */
	if ((unsigned)index >= getNumBrainPins() + BOARD_EXT_PINREPOPINS)
		return -1;

	return index;
}

static brain_pin_e index_to_brainPin(int i)
{
	return (brain_pin_e)((int)GPIOA_0 + i);
}

PinRepository::PinRepository() {
}

static PinRepository instance;

static void reportPins(void) {
	for (unsigned int i = 0; i < getNumBrainPins(); i++) {
		const char *pin_user = getBrainUsedPin(i);

		/* show used pins */
		if (pin_user != NULL) {
			brain_pin_e brainPin = index_to_brainPin(i);
			int pin = getBrainPinIndex(brainPin);
			ioportid_t port = getBrainPort(brainPin);

			scheduleMsg(&logger, "pin %s%d: %s", portname(port), pin, pin_user);
		}
	}

	#if (BOARD_EXT_GPIOCHIPS > 0)
		for (unsigned int i = getNumBrainPins() ; i < getNumBrainPins() + BOARD_EXT_PINREPOPINS /* gpiochips_get_total_pins()*/ ; i++) {
			const char *pin_name;
			const char *pin_user;
			brain_pin_e brainPin = index_to_brainPin(i);

			pin_name = gpiochips_getPinName(brainPin);
			pin_user = getBrainUsedPin(i);

			/* here show all pins, unused too */
			if (pin_name != NULL) {
				// this probably uses a lot of output buffer!
				scheduleMsg(&logger, "ext %s: %s",
					pin_name, pin_user ? pin_user : "free");
			} else {
				scheduleMsg(&logger, "ext %s.%d: %s",
					gpiochips_getChipName(brainPin), gpiochips_getPinOffset(brainPin), pin_user ? pin_user : "free");
			}
		}
	#endif

	scheduleMsg(&logger, "Total pins count: %d", totalPinsUsed);
}

static MemoryStream portNameStream;
static char portNameBuffer[20];

const char *hwPortname(brain_pin_e brainPin) {
	if (brainPin == GPIO_INVALID) {
		return "INVALID";
	}
	if (brainPin == GPIO_UNASSIGNED) {
		return "NONE";
	}
	portNameStream.eos = 0; // reset
	if (brain_pin_is_onchip(brainPin)) {

		ioportid_t hwPort = getHwPort("hostname", brainPin);
		if (hwPort == GPIO_NULL) {
			return "NONE";
		}
		int hwPin = getHwPin("hostname", brainPin);
		chprintf((BaseSequentialStream *) &portNameStream, "%s%d", portname(hwPort), hwPin);
	}
	#if (BOARD_EXT_GPIOCHIPS > 0)
		else {
			const char *pin_name = gpiochips_getPinName(brainPin);

			if (pin_name) {
				chprintf((BaseSequentialStream *) &portNameStream, "ext:%s",
					pin_name);
			} else {
				chprintf((BaseSequentialStream *) &portNameStream, "ext:%s.%d",
					gpiochips_getChipName(brainPin), gpiochips_getPinOffset(brainPin));
			}
		}
	#endif
	portNameStream.buffer[portNameStream.eos] = 0; // need to terminate explicitly

	return portNameBuffer;
}

void initPinRepository(void) {
	/**
	 * this method cannot use console because this method is invoked before console is initialized
	 */

	msObjectInit(&portNameStream, (uint8_t*) portNameBuffer, sizeof(portNameBuffer), 0);

	initBrainUsedPins();

	initialized = true;

	addConsoleAction("pins", reportPins);
}

bool brain_pin_is_onchip(brain_pin_e brainPin)
{
	if ((brainPin < GPIOA_0) || (brainPin > BRAIN_PIN_LAST_ONCHIP))
		return false;

	return true;
}

bool brain_pin_is_ext(brain_pin_e brainPin)
{
	if (brainPin > BRAIN_PIN_LAST_ONCHIP)
		return true;

	return false;
}

/**
 * See also brain_pin_markUnused()
 * @return true if this pin was already used, false otherwise
 */

bool brain_pin_markUsed(brain_pin_e brainPin, const char *msg) {
	if (!initialized) {
		firmwareError(CUSTOM_ERR_PIN_REPO, "repository not initialized");
		return false;
	}

#if ! EFI_BOOTLOADER
	scheduleMsg(&logger, "%s on %s", msg, hwPortname(brainPin));
#endif

	int index = brainPin_to_index(brainPin);
	if (index < 0)
		return true;

	if (getBrainUsedPin(index) != NULL) {
		/* TODO: get readable name of brainPin... */
		/**
		 * todo: the problem is that this warning happens before the console is even
		 * connected, so the warning is never displayed on the console and that's quite a problem!
		 */
//		warning(OBD_PCM_Processor_Fault, "brain pin %d req by %s used by %s", brainPin, msg, getBrainUsedPin(index));
		firmwareError(CUSTOM_ERR_PIN_ALREADY_USED_1, "brain pin %s req by %s used by %s", hwPortname(brainPin), msg, getBrainUsedPin(index));
		return true;
	}

	getBrainUsedPin(index) = msg;
	totalPinsUsed++;
	return false;
}

/**
 * See also brain_pin_markUsed()
 */

void brain_pin_markUnused(brain_pin_e brainPin)
{
	int index;

	if (!initialized) {
		firmwareError(CUSTOM_ERR_PIN_REPO, "repository not initialized");
		return;
	}

	index = brainPin_to_index(brainPin);
	if (index < 0)
		return;

	if (getBrainUsedPin(index) != NULL)
		totalPinsUsed--;
	getBrainUsedPin(index) = NULL;
}

/**
 * Marks on-chip gpio port-pin as used. Works only for on-chip gpios
 * To be replaced with brain_pin_markUsed later
 */

bool gpio_pin_markUsed(ioportid_t port, ioportmask_t pin, const char *msg) {
	if (!initialized) {
		firmwareError(CUSTOM_ERR_PIN_REPO, "repository not initialized");
		return false;
	}
	int index = getBrainIndex(port, pin);

	if (getBrainUsedPin(index) != NULL) {
		/**
		 * todo: the problem is that this warning happens before the console is even
		 * connected, so the warning is never displayed on the console and that's quite a problem!
		 */
//		warning(OBD_PCM_Processor_Fault, "%s%d req by %s used by %s", portname(port), pin, msg, getBrainUsedPin(index));
		firmwareError(CUSTOM_ERR_PIN_ALREADY_USED_1, "%s%d req by %s used by %s", portname(port), pin, msg, getBrainUsedPin(index));
		return true;
	}
	getBrainUsedPin(index) = msg;
	totalPinsUsed++;
	return false;
}

/**
 * Marks on-chip gpio port-pin as UNused. Works only for on-chip gpios
 * To be replaced with brain_pin_markUnused later
 */

void gpio_pin_markUnused(ioportid_t port, ioportmask_t pin) {
	if (!initialized) {
		firmwareError(CUSTOM_ERR_PIN_REPO, "repository not initialized");
		return;
	}
	int index = getBrainIndex(port, pin);

	if (getBrainUsedPin(index) != NULL)
		totalPinsUsed--;
	getBrainUsedPin(index) = NULL;
}

const char *getPinFunction(brain_input_pin_e brainPin) {
	int index;

	index = brainPin_to_index(brainPin);
	if (index < 0)
		return NULL;

	return getBrainUsedPin(index);
}

#endif
