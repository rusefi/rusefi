/**
 * @file    pin_repository.cpp
 * @brief   I/O pin registry code
 *
 * This job of this class is to make sure that we are not using same hardware pin for two
 * different purposes.
 *
 * @date Jan 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "global.h"
#if EFI_PROD_CODE
#include "os_access.h"
#include "pin_repository.h"
#include "eficonsole.h"
#include "memstreams.h"
#include "drivers/gpio/gpio_ext.h"
#include "smart_gpio.h"
#include "hardware.h"

static LoggingWithStorage logger("pin repos");

static PinRepository pinRepository;

static int brainPin_to_index(brain_pin_e brainPin)
{
	int index;

	if (brainPin < GPIOA_0)
		return -1;

	index = brainPin - GPIOA_0;

	if ((unsigned)index < getNumBrainPins())
		return index;

	/* gpiochips magic: skip gates for absent chips */
#ifdef TLE8888_PIN_1
	if ((brainPin >= TLE8888_PIN_1) && (BOARD_TLE8888_COUNT == 0))
		index -= (TLE8888_PIN_28 -TLE8888_PIN_1 + 1);
#endif

#ifdef MC33972_PIN_1
	if ((brainPin >= MC33972_PIN_1) && (BOARD_MC33972_COUNT == 0))
		index -= (MC33972_PIN_22 - MC33972_PIN_1 + 1);
#endif

#ifdef TLE6240_PIN_1
	if ((brainPin >= TLE6240_PIN_1) && (BOARD_TLE6240_COUNT == 0))
		index -= (TLE6240_PIN_16 - TLE6240_PIN_1 + 1);
#endif

	/* if index outside array boundary */
	if ((unsigned)index >= getNumBrainPins() + BOARD_EXT_PINREPOPINS)
		return -1;

	return index;
}

static brain_pin_e index_to_brainPin(unsigned int i)
{
	brain_pin_e brainPin = (brain_pin_e)((int)GPIOA_0 + i);

	/* on-chip pins */
	if (i < getNumBrainPins())
		return brainPin;

	/* gpiochips magic: skip absent chips */
#ifdef TLE6240_PIN_1
	if (BOARD_TLE6240_COUNT == 0)
		brainPin += (TLE6240_PIN_16 - TLE6240_PIN_1 + 1);
#endif

#ifdef MC33972_PIN_1
	if (BOARD_MC33972_COUNT == 0)
		brainPin += (MC33972_PIN_22 - MC33972_PIN_1 + 1);
#endif

#ifdef TLE8888_PIN_1
	if (BOARD_TLE8888_COUNT == 0)
		brainPin += (TLE8888_PIN_28 -TLE8888_PIN_1 + 1);
#endif

	return brainPin;
}

static MemoryStream portNameStream;
static char portNameBuffer[20];

PinRepository::PinRepository() {
	msObjectInit(&portNameStream, (uint8_t*) portNameBuffer, sizeof(portNameBuffer), 0);

	initBrainUsedPins();
}

#if (BOARD_TLE8888_COUNT > 0)
void tle8888_dump_regs(void)
{
	// since responses are always in the NEXT transmission we will have this one first
	tle8888_read_reg(0, NULL);

	scheduleMsg(&logger, "register: data");
	for (int request = 0; request <= 0x7e + 1; request++) {
		uint16_t tmp;
		tle8888_read_reg(request < (0x7e + 1) ? request : 0x7e, &tmp);
		uint8_t reg = getRegisterFromResponse(tmp);
		uint8_t data = getDataFromResponse(tmp);

		scheduleMsg(&logger, "%02x: %02x", reg, data);
	}
}
#endif

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
			static char pin_error[64];
			const char *pin_name;
			const char *pin_user;
			brain_pin_diag_e pin_diag;
			brain_pin_e brainPin = index_to_brainPin(i);

			pin_name = gpiochips_getPinName(brainPin);
			pin_user = getBrainUsedPin(i);
			pin_diag = gpiochips_getDiag(brainPin);

			/* use autogeneraged helpers here? */
			if (pin_diag == PIN_OK) {
				chsnprintf(pin_error, sizeof(pin_error), "Ok");
			} else if (pin_diag != PIN_INVALID) {
				chsnprintf(pin_error, sizeof(pin_error), "%s%s%s%s%s",
					pin_diag & PIN_OPEN ? "open_load " : "",
					pin_diag & PIN_SHORT_TO_GND ? "short_to_gnd " : "",
					pin_diag & PIN_SHORT_TO_BAT ? "short_to_bat " : "",
					pin_diag & PIN_OVERLOAD ? "overload " : "",
					pin_diag & PIN_DRIVER_OVERTEMP ? "overtemp": "");
			} else {
				chsnprintf(pin_error, sizeof(pin_error), "INVALID");
			}

			/* here show all pins, unused too */
			if (pin_name != NULL) {
				// this probably uses a lot of output buffer!
				scheduleMsg(&logger, "ext %s: %s diagnostic: %s",
					pin_name, pin_user ? pin_user : "free", pin_error);
			} else {
				const char *chip_name = gpiochips_getChipName(brainPin);
				/* if chip exist */
				if (chip_name != NULL) {
					scheduleMsg(&logger, "ext %s.%d: %s diagnostic: %s",
						chip_name, gpiochips_getPinOffset(brainPin), pin_user ? pin_user : "free", pin_error);
				}
			}
		}
	#endif

	scheduleMsg(&logger, "Total pins count: %d", pinRepository.totalPinsUsed);
}

void printSpiConfig(Logging *logging, const char *msg, spi_device_e device) {
	scheduleMsg(logging, "%s %s mosi=%s", msg, getSpi_device_e(device), hwPortname(getMosiPin(device)));
	scheduleMsg(logging, "%s %s miso=%s", msg, getSpi_device_e(device), hwPortname(getMisoPin(device)));
	scheduleMsg(logging, "%s %s sck=%s",  msg, getSpi_device_e(device), hwPortname(getSckPin(device)));
}


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

	addConsoleAction(CMD_PINS, reportPins);

#if (BOARD_TLE8888_COUNT > 0)
	addConsoleAction("tle8888", tle8888_dump_regs);
	addConsoleAction("tle8888init", tle8888_req_init);
#endif
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
		firmwareError(CUSTOM_ERR_PIN_ALREADY_USED_1, "Pin \"%s\" required by \"%s\" but is used by \"%s\"", hwPortname(brainPin), msg, getBrainUsedPin(index));
		return true;
	}

	getBrainUsedPin(index) = msg;
	pinRepository.totalPinsUsed++;
	return false;
}

/**
 * See also brain_pin_markUsed()
 */

void brain_pin_markUnused(brain_pin_e brainPin) {
	int index = brainPin_to_index(brainPin);
	if (index < 0)
		return;

	if (getBrainUsedPin(index) != NULL)
		pinRepository.totalPinsUsed--;
	getBrainUsedPin(index) = nullptr;
}

/**
 * Marks on-chip gpio port-pin as used. Works only for on-chip gpios
 * To be replaced with brain_pin_markUsed later
 */

bool gpio_pin_markUsed(ioportid_t port, ioportmask_t pin, const char *msg) {
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
	pinRepository.totalPinsUsed++;
	return false;
}

/**
 * Marks on-chip gpio port-pin as UNused. Works only for on-chip gpios
 * To be replaced with brain_pin_markUnused later
 */

void gpio_pin_markUnused(ioportid_t port, ioportmask_t pin) {
	int index = getBrainIndex(port, pin);

	if (getBrainUsedPin(index) != NULL)
		pinRepository.totalPinsUsed--;
	getBrainUsedPin(index) = nullptr;
}

const char *getPinFunction(brain_input_pin_e brainPin) {
	int index;

	index = brainPin_to_index(brainPin);
	if (index < 0)
		return NULL;

	return getBrainUsedPin(index);
}
#else
const char *hwPortname(brain_pin_e brainPin) {
	(void)brainPin;
	return "N/A";
}
#endif /* EFI_PROD_CODE */
