/**
 * @file    pin_repository.c
 * @brief   I/O pin registry code
 *
 * This job of this class is to make sure that we are not using same hardware pin for two
 * different purposes.
 *
 * @date Jan 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "pin_repository.h"
#include "eficonsole.h"
#include "memstreams.h"
#include "chprintf.h"
#include "rusefi.h"

#define PIN_REPO_SIZE 7 * 16
const char *PIN_USED[PIN_REPO_SIZE];
static int initialized = FALSE;

static Logging logger;
static int totalPinsUsed = 0;

/**
 * @deprecated - use hwPortname() instead
 */
char *portname(GPIO_TypeDef* GPIOx) {
	if (GPIOx == GPIOA)
		return "PA";
	if (GPIOx == GPIOB)
		return "PB";
	if (GPIOx == GPIOC)
		return "PC";
	if (GPIOx == GPIOD)
		return "PD";
	if (GPIOx == GPIOE)
		return "PE";
	if (GPIOx == GPIOF)
		return "PF";
	if (GPIOx == GPIOH)
		return "PH";
	return "unknown";
}

static int getPortIndex(GPIO_TypeDef* port) {
	if (port == GPIOA)
		return 0;
	if (port == GPIOB)
		return 1;
	if (port == GPIOC)
		return 2;
	if (port == GPIOD)
		return 3;
	if (port == GPIOE)
		return 4;
	if (port == GPIOF)
		return 5;
	if (port == GPIOH)
		return 6;
	firmwareError("portindex");
	return -1;
}

static void reportPins(void) {
	for (int i = 0; i < PIN_REPO_SIZE; i++) {
		const char *name = PIN_USED[i];
		if (name != NULL)
			print("ping %d: %s\r\n", i, name);
	}

	print("Total pins count: %d\r\n", totalPinsUsed);
}

static MemoryStream portNameStream;
static char portNameBuffer[20];

/**
 * Parse string representation of physical pin into brain_pin_e ordinal.
 *
 * @return GPIO_NONE for "none", GPIO_INVALID for invalid entry
 */
brain_pin_e parseBrainPin(const char *str) {
	if (strEqual(str, "none"))
		return GPIO_NONE;
	// todo: create method toLowerCase?
	if (str[0] != 'p' && str[0] != 'p') {
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
	return basePin + pin;
}

char *hwPortname(brain_pin_e brainPin) {
	if (brainPin == GPIO_INVALID) {
		return "INVALID";
	}
	GPIO_TypeDef *hwPort = getHwPort(brainPin);
	if (hwPort == GPIO_NULL) {
		return "NONE";
	}
	int hwPin = getHwPin(brainPin);
	portNameStream.eos = 0; // reset
	chprintf((BaseSequentialStream *) &portNameStream, "%s%d", portname(hwPort), hwPin);
	portNameStream.buffer[portNameStream.eos] = 0; // need to terminate explicitly
	return portNameBuffer;
}

void initPinRepository(void) {
	/**
	 * this method cannot use console because this method is invoked before console is initialized
	 */
	initLogging(&logger, "pin repos");

	msObjectInit(&portNameStream, (uint8_t*)portNameBuffer, sizeof(portNameBuffer), 0);

	for (int i = 0; i < PIN_REPO_SIZE; i++)
		PIN_USED[i] = 0;
	initialized = TRUE;
	addConsoleAction("pins", reportPins);
}

static inline void markUsed(int index, const char *msg) {
	PIN_USED[index] = msg;
	totalPinsUsed++;
}

void mySetPadMode2(const char *msg, brain_pin_e pin, iomode_t mode) {
	mySetPadMode(msg, getHwPort(pin), getHwPin(pin), mode);
}

/**
 * This method would set an error condition if pin is already used
 */
void mySetPadMode(const char *msg, ioportid_t port, ioportmask_t pin, iomode_t mode) {
	if (!initialized) {
		firmwareError("repository not initialized");
		return;
	}
	print("%s on %s:%d\r\n", msg, portname(port), pin);

	appendPrintf(&logger, "msg,%s", msg);
	appendPrintf(&logger, " on %s%d%s", portname(port), pin, DELIMETER);
	printLine(&logger);

	int portIndex = getPortIndex(port);
	int index = portIndex * 16 + pin;

	if (PIN_USED[index] != NULL) {
		warning(OBD_PCM_Processor_Fault, "%s%d req by %s used by %s", portname(port), pin, msg, PIN_USED[index]);
		return;
	}
	markUsed(index, msg);

	palSetPadMode(port, pin, mode);
}

/**
 * This method would crash the program if pin is already in use
 */
void registedFundamentralIoPin(char *msg, ioportid_t port, ioportmask_t pin, iomode_t mode) {
	efiAssertVoid(initialized, "repo not initialized");

	int portIndex = getPortIndex(port);
	int index = portIndex * 16 + pin;

	if (PIN_USED[index] != NULL) {
		print("!!!!!!!!!!!!! Already used [%s] %d\r\n", msg, pin);
		print("!!!!!!!!!!!!! Already used by [%s]\r\n", PIN_USED[index]);
		firmwareError("pin already used");
		return;
	}
	markUsed(index, msg);
	palSetPadMode(port, pin, mode);
}

