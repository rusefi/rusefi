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
#include "pin_repository.h"
#include "eficonsole.h"
#include "memstreams.h"
#include "chprintf.h"
#include "drivers/gpio/gpio_ext.h"

#ifndef BOARD_EXT_PINREPOPINS
	#define BOARD_EXT_PINREPOPINS 0
#endif

static ioportid_t ports[] = {GPIOA,
		GPIOB,
		GPIOC,
		GPIOD,
		GPIOE,
		GPIOF,
		GPIOG,
		GPIOH,
};

#define PIN_REPO_SIZE (sizeof(ports) / sizeof(ports[0])) * PORT_SIZE
// todo: move this into PinRepository class
const char *PIN_USED[PIN_REPO_SIZE + BOARD_EXT_PINREPOPINS];
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
	if ((unsigned)index > (sizeof(PIN_USED) / sizeof(PIN_USED[0])))
		return -1;

	return index;
}

PinRepository::PinRepository() {
}

static PinRepository instance;

static int getPortIndex(ioportid_t port) {
	efiAssert(CUSTOM_ERR_ASSERT, port != NULL, "null port", -1);
	if (port == GPIOA)
		return 0;
	if (port == GPIOB)
		return 1;
	if (port == GPIOC)
		return 2;
	if (port == GPIOD)
		return 3;
#if defined(STM32F4XX) || defined(STM32F7XX)
	if (port == GPIOE)
		return 4;
#endif /* defined(STM32F4XX) */
	if (port == GPIOF)
		return 5;
#if defined(STM32F4XX) || defined(STM32F7XX)
	if (port == GPIOG)
		return 6;
#endif /* defined(STM32F4XX) */
#if defined(STM32F4XX) || defined(STM32F7XX)
	if (port == GPIOH)
		return 7;
#endif /* defined(STM32F4XX) */
	firmwareError(CUSTOM_ERR_UNKNOWN_PORT, "unknown port");
	return -1;
}

static void reportPins(void) {
	unsigned int i;

	for (i = 0; i < PIN_REPO_SIZE; i++) {
		const char *pin_user = PIN_USED[i];

		/* show used pins */
		if (pin_user != NULL) {
			int portIndex = i / PORT_SIZE;
			int pin = i % PORT_SIZE;
			ioportid_t port = ports[portIndex];

			scheduleMsg(&logger, "pin %s%d: %s\n", portname(port), pin, pin_user);
		}
	}

	for (i = PIN_REPO_SIZE ; i < PIN_REPO_SIZE + BOARD_EXT_PINREPOPINS /* gpiochips_get_total_pins()*/ ; i++) {
		const char *port_name = portNameExt(i);
		const char *pin_user = PIN_USED[i];

		/* here show all pins, unused too */
		if (port_name != NULL) {
			int pin = getHwPinExt(i);

			scheduleMsg(&logger, "epin %s_%d: %s\n", port_name, pin, pin_user ? pin_user : "free");
		}
	}

	scheduleMsg(&logger, "Total pins count: %d", totalPinsUsed);
}

static MemoryStream portNameStream;
static char portNameBuffer[20];

/**
 * Parse string representation of physical pin into brain_pin_e ordinal.
 *
 * @return GPIO_UNASSIGNED for "none", GPIO_INVALID for invalid entry
 */
brain_pin_e parseBrainPin(const char *str) {
	if (strEqual(str, "none"))
		return GPIO_UNASSIGNED;
	// todo: create method toLowerCase?
	if (str[0] != 'p' && str[0] != 'P') {
		return GPIO_INVALID;
	}
	char port = str[1];
	brain_pin_e basePin;
	if (port >= 'a' && port <= 'z') {
		basePin = (brain_pin_e) ((int) GPIOA_0 + 16 * (port - 'a'));
	} else if (port >= 'A' && port <= 'Z') {
		basePin = (brain_pin_e) ((int) GPIOA_0 + 16 * (port - 'A'));
	} else {
		return GPIO_INVALID;
	}
	const char *pinStr = str + 2;
	int pin = atoi(pinStr);
	return (brain_pin_e)(basePin + pin);
}

const char *hwPortname(brain_pin_e brainPin) {
	if (brainPin == GPIO_INVALID) {
		return "INVALID";
	}
	ioportid_t hwPort = getHwPort("hostname", brainPin);
	if (hwPort == GPIO_NULL) {
		return "NONE";
	}
	int hwPin = getHwPin("hostname", brainPin);
	portNameStream.eos = 0; // reset
	chprintf((BaseSequentialStream *) &portNameStream, "%s%d", portname(hwPort), hwPin);
	portNameStream.buffer[portNameStream.eos] = 0; // need to terminate explicitly
	return portNameBuffer;
}

void initPinRepository(void) {
	/**
	 * this method cannot use console because this method is invoked before console is initialized
	 */

	msObjectInit(&portNameStream, (uint8_t*) portNameBuffer, sizeof(portNameBuffer), 0);

	memset(PIN_USED, 0, sizeof(PIN_USED));

	initialized = true;

	addConsoleAction("pins", reportPins);

	/* external chip init */
	gpiochips_init();
}

static int getIndex(ioportid_t port, ioportmask_t pin) {
	int portIndex = getPortIndex(port);
	return portIndex * PORT_SIZE + pin;
}

bool brain_pin_is_onchip(brain_pin_e brainPin)
{
	if ((brainPin < GPIOA_0) || (brainPin > GPIOH_15))
		return false;

	return true;
}

/**
 * See also unmarkPin()
 * @return true if this pin was already used, false otherwise
 */
bool brain_pin_markUsed(brain_pin_e brainPin, const char *msg)
{
	int index;

	if (!initialized) {
		firmwareError(CUSTOM_ERR_PIN_REPO, "repository not initialized");
		return false;
	}

	index = brainPin_to_index(brainPin);
	if (index < 0)
		return true;

	if (PIN_USED[index] != NULL) {
		/* TODO: cleanup this */
		ioportid_t port = getHwPort(msg, brainPin);
		ioportmask_t pin = getHwPin(msg, brainPin);
		/**
		 * todo: the problem is that this warning happens before the console is even
		 * connected, so the warning is never displayed on the console and that's quite a problem!
		 */
//		warning(OBD_PCM_Processor_Fault, "%s%d req by %s used by %s", portname(port), pin, msg, PIN_USED[index]);
		firmwareError(CUSTOM_ERR_PIN_ALREADY_USED_1, "%s%d req by %s used by %s", portname(port), pin, msg, PIN_USED[index]);
		return true;
	}

	PIN_USED[index] = msg;
	totalPinsUsed++;
	return false;
}

bool markUsed(ioportid_t port, ioportmask_t pin, const char *msg) {
	if (!initialized) {
		firmwareError(CUSTOM_ERR_PIN_REPO, "repository not initialized");
		return false;
	}
	int index = getIndex(port, pin);

	if (PIN_USED[index] != NULL) {
		/**
		 * todo: the problem is that this warning happens before the console is even
		 * connected, so the warning is never displayed on the console and that's quite a problem!
		 */
//		warning(OBD_PCM_Processor_Fault, "%s%d req by %s used by %s", portname(port), pin, msg, PIN_USED[index]);
		firmwareError(CUSTOM_ERR_PIN_ALREADY_USED_1, "%s%d req by %s used by %s", portname(port), pin, msg, PIN_USED[index]);
		return true;
	}
	PIN_USED[index] = msg;
	totalPinsUsed++;
	return false;
}

/**
 * See also markUsed()
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

	if (PIN_USED[index] != NULL)
		totalPinsUsed--;
	PIN_USED[index] = NULL;
}

void markUnused(ioportid_t port, ioportmask_t pin) {
	if (!initialized) {
		firmwareError(CUSTOM_ERR_PIN_REPO, "repository not initialized");
		return;
	}
	int index = getIndex(port, pin);

	if (PIN_USED[index] != NULL)
		totalPinsUsed--;
	PIN_USED[index] = NULL;
}

const char *getPinFunction(brain_input_pin_e brainPin) {
	int index;

	index = brainPin_to_index(brainPin);
	if (index < 0)
		return NULL;

	return PIN_USED[index];
}


void unmarkPin(brain_pin_e brainPin) {
	if (brainPin == GPIO_UNASSIGNED) {
		return;
	}
	ioportid_t port = getHwPort("unmark", brainPin);
	ioportmask_t pin = getHwPin("unmark", brainPin);

	int index = getIndex(port, pin);

	if (PIN_USED[index] != NULL) {
		PIN_USED[index] = NULL;
		totalPinsUsed--;
	}
}

#endif
