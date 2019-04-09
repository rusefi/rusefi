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

static ioportid_t ports[] = {GPIOA,
		GPIOB,
		GPIOC,
		GPIOD,
#if STM32_HAS_GPIOE
		GPIOE,
#else
		nullptr,
#endif /* STM32_HAS_GPIOE */
#if STM32_HAS_GPIOF
		GPIOF,
#else
		nullptr,
#endif /* STM32_HAS_GPIOF */
#if STM32_HAS_GPIOG
		GPIOG,
#else
		nullptr,
#endif /* STM32_HAS_GPIOG */
#if STM32_HAS_GPIOH
		GPIOH,
#else
		nullptr,
#endif /* STM32_HAS_GPIOH */
};

#define PIN_REPO_SIZE (sizeof(ports) / sizeof(ports[0])) * PORT_SIZE
// todo: move this into PinRepository class
const char *PIN_USED[PIN_REPO_SIZE];
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

static brain_pin_e index_to_brainPin(int i)
{
	return (brain_pin_e)((int)GPIOA_0 + i);
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
#if STM32_HAS_GPIOE
	if (port == GPIOE)
		return 4;
#endif /* STM32_HAS_GPIOE */
#if STM32_HAS_GPIOF
	if (port == GPIOF)
		return 5;
#endif /* STM32_HAS_GPIOF */
#if STM32_HAS_GPIOG
	if (port == GPIOG)
		return 6;
#endif /* STM32_HAS_GPIOG */
#if STM32_HAS_GPIOH
	if (port == GPIOH)
		return 7;
#endif /* STM32_HAS_GPIOH */
	firmwareError(CUSTOM_ERR_UNKNOWN_PORT, "unknown port");
	return -1;
}

static void reportPins(void) {
	unsigned int i;

	for (i = 0; i < PIN_REPO_SIZE; i++) {
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

	memset(PIN_USED, 0, sizeof(PIN_USED));

	initialized = true;
	addConsoleAction("pins", reportPins);
}

static int getIndex(ioportid_t port, ioportmask_t pin) {
	int portIndex = getPortIndex(port);
	return portIndex * PORT_SIZE + pin;
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
		/* TODO: get readable name of brainPin... */
		/**
		 * todo: the problem is that this warning happens before the console is even
		 * connected, so the warning is never displayed on the console and that's quite a problem!
		 */
//		warning(OBD_PCM_Processor_Fault, "brain pin %d req by %s used by %s", brainPin, msg, PIN_USED[index]);
		firmwareError(CUSTOM_ERR_PIN_ALREADY_USED_1, "brain pin %d req by %s used by %s", brainPin, msg, PIN_USED[index]);
		return true;
	}

	PIN_USED[index] = msg;
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

	if (PIN_USED[index] != NULL)
		totalPinsUsed--;
	PIN_USED[index] = NULL;
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
 * Marks on-chip gpio port-pin as UNused. Works only for on-chip gpios
 * To be replaced with brain_pin_markUnused later
 */

void gpio_pin_markUnused(ioportid_t port, ioportmask_t pin) {
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

#endif
