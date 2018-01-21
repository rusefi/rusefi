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

#include "main.h"
#include "pin_repository.h"
#include "eficonsole.h"
#include "memstreams.h"
#include "chprintf.h"
#include "rusefi.h"

#define PIN_REPO_SIZE 7 * PORT_SIZE
// todo: move this into PinRepository class
const char *PIN_USED[PIN_REPO_SIZE];
static int initialized = FALSE;

static LoggingWithStorage logger("pin repos");
static int totalPinsUsed = 0;

static ioportid_t ports[7] = {GPIOA,
		GPIOB,
		GPIOC,
		GPIOD,
		GPIOE,
		GPIOF,
		GPIOH,
};

PinRepository::PinRepository() {
}

static int getPortIndex(ioportid_t port) {
	efiAssert(port != NULL, "null port", -1);
	if (port == GPIOA)
		return 0;
	if (port == GPIOB)
		return 1;
	if (port == GPIOC)
		return 2;
	if (port == GPIOD)
		return 3;
#if defined(STM32F4XX)
	if (port == GPIOE)
		return 4;
#endif /* defined(STM32F4XX) */
	if (port == GPIOF)
		return 5;
#if defined(STM32F4XX)
	if (port == GPIOH)
		return 6;
#endif /* defined(STM32F4XX) */
	firmwareError(CUSTOM_ERR_UNKNOWN_PORT, "unknown port");
	return -1;
}

static void reportPins(void) {
	for (int i = 0; i < PIN_REPO_SIZE; i++) {
		const char *name = PIN_USED[i];
		int portIndex = i / PORT_SIZE;
		int pin = i % PORT_SIZE;
		ioportid_t port = ports[portIndex];
		if (name != NULL) {
			scheduleMsg(&logger, "pin %s%d: %s", portname(port), pin, name);
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

	for (int i = 0; i < PIN_REPO_SIZE; i++)
		PIN_USED[i] = 0;
	initialized = true;
	addConsoleAction("pins", reportPins);
}

static int getIndex(ioportid_t port, ioportmask_t pin) {
	int portIndex = getPortIndex(port);
	return portIndex * PORT_SIZE + pin;
}

/**
 * See also unmarkPin()
 * @return true if this pin was already used, false otherwise
 */
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

const char * getPinFunction(brain_input_pin_e brainPin) {
	ioportid_t port = getHwPort("getF", brainPin);
	ioportmask_t pin = getHwPin("getF", brainPin);

	int index = getIndex(port, pin);
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

