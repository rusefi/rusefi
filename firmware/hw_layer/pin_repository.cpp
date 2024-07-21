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

#include "pch.h"

static size_t getBrainPinTotalNum() {
	return BRAIN_PIN_TOTAL_PINS;
}

const char* & getBrainUsedPin(size_t index) {
	return engine->pinRepository.getBrainUsedPin(index);
}

/* Common for firmware and unit tests */
bool isBrainPinValid(brain_pin_e brainPin) {
	if ((brainPin == Gpio::Unassigned) || (brainPin == Gpio::Invalid))
		return false;

	if (brainPin > BRAIN_PIN_LAST)
		/* something terribly wrong */
		return false;

	return true;
}

int brainPin_to_index(Gpio brainPin) {
	if (brainPin < Gpio::A0)
		return -1;

	size_t i = brainPin - Gpio::A0;

	if (i >= getBrainPinTotalNum())
		return -1;

	return i;
}

/**
 * See also brain_pin_markUnused()
 * @return true if this pin was already used, false otherwise
 */

bool brain_pin_markUsed(Gpio brainPin, const char *msg) {
#ifndef EFI_BOOTLOADER
	efiPrintf("pin_markUsed: %s on %s", msg, hwPortname(brainPin));
#endif

	int index = brainPin_to_index(brainPin);
	if (index < 0)
		return true;

	if (engine->pinRepository.getBrainUsedPin(index) != nullptr) {
		// hwPortname and share a buffer behind the scenes, even while they probably never use it for different
		// values here let's have an explicit second buffer to make this more reliable
		char physicalPinName[32];
		strncpy(physicalPinName, hwPhysicalPinName(brainPin), sizeof(physicalPinName) - 1);
		criticalError("Pin \"%s\" (%s) required by \"%s\" but is used by \"%s\"",
				hwPortname(brainPin),
				physicalPinName,
				msg,
				getBrainUsedPin(index));
		return true;
	}

	getBrainUsedPin(index) = msg;
	return false;
}

/**
 * See also brain_pin_markUsed()
 */

void brain_pin_markUnused(brain_pin_e brainPin) {
#ifndef EFI_BOOTLOADER
	efiPrintf("pin_markUnused: %s", hwPortname(brainPin));
#endif
	int index = brainPin_to_index(brainPin);
	if (index < 0)
		return;

	getBrainUsedPin(index) = nullptr;
}

#if EFI_PROD_CODE
#include "memstreams.h"
static MemoryStream portNameStream;
static char portNameBuffer[20];
#endif /* EFI_PROD_CODE */

PinRepository::PinRepository() {
#if EFI_PROD_CODE
	msObjectInit(&portNameStream, (uint8_t*) portNameBuffer, sizeof(portNameBuffer), 0);
#endif /* EFI_PROD_CODE */

	memset(PIN_USED, 0, sizeof(PIN_USED));
}

#if EFI_PROD_CODE

#include "eficonsole.h"
#include "drivers/gpio/gpio_ext.h"
#include "smart_gpio.h"
#include "hardware.h"

void pinDiag2string(char *buffer, size_t size, brain_pin_diag_e pin_diag) {
	/* use autogeneraged helpers here? */
	if (pin_diag == PIN_OK) {
		chsnprintf(buffer, size, "Ok");
	} else if (pin_diag != PIN_UNKNOWN) {
		chsnprintf(buffer, size, "%s%s%s%s%s%s",
			pin_diag & PIN_DRIVER_OFF ? "driver_off " : "",
			pin_diag & PIN_OPEN ? "open_load " : "",
			pin_diag & PIN_SHORT_TO_GND ? "short_to_gnd " : "",
			pin_diag & PIN_SHORT_TO_BAT ? "short_to_bat " : "",
			pin_diag & PIN_OVERLOAD ? "overload " : "",
			pin_diag & PIN_DRIVER_OVERTEMP ? "overtemp": "");
	} else {
		chsnprintf(buffer, size, "INVALID");
	}
}

static brain_pin_e index_to_brainPin(unsigned int i)
{
	if (i < getBrainPinTotalNum())
		return Gpio::A0 + i;

	return Gpio::Invalid;
}

static void reportPins() {
	int totalPinsUsed = 0;

	for (unsigned int i = 0; i < getBrainPinOnchipNum(); i++) {
		const char *pin_user = getBrainUsedPin(i);

		/* show used pins */
		if (pin_user != NULL) {
			static char pin_state[64];
			brain_pin_e brainPin = index_to_brainPin(i);
			int pin = getBrainPinIndex(brainPin);
			ioportid_t port = getBrainPinPort(brainPin);
			debugBrainPin(pin_state, sizeof(pin_state), brainPin);

            const char *boardPinName = getBoardSpecificPinName(brainPin);
			efiPrintf("pin %s%d (%s): %s %s", portname(port), pin, boardPinName, pin_user, pin_state);
			totalPinsUsed++;
		}
	}

	#if (BOARD_EXT_GPIOCHIPS > 0)
		for (unsigned int i = getBrainPinOnchipNum() ; i < getBrainPinTotalNum(); i++) {
			static char pin_error[64];
			brain_pin_e brainPin = index_to_brainPin(i);

			const char *pin_name = gpiochips_getPinName(brainPin);
			const char *pin_user = getBrainUsedPin(i);
			brain_pin_diag_e pin_diag = gpiochips_getDiag(brainPin);

			pinDiag2string(pin_error, sizeof(pin_error), pin_diag);

			/* here show all pins, unused too */
			if (pin_name != NULL) {
				// this probably uses a lot of output buffer!
				efiPrintf("ext %s: %s diagnostic: %s",
					pin_name, pin_user ? pin_user : "free", pin_error);
			} else {
				const char *chip_name = gpiochips_getChipName(brainPin);
				/* if chip exist */
				if (chip_name != NULL) {
					efiPrintf("ext %s.%d: %s diagnostic: %s",
						chip_name, gpiochips_getPinOffset(brainPin), pin_user ? pin_user : "free", pin_error);
				}
			}
			if (pin_user != NULL) {
				totalPinsUsed++;
			}
		}
	#endif

	efiPrintf("Total pins used: %d", totalPinsUsed);

	gpiochips_debug();
}

__attribute__((weak)) const char * getBoardSpecificPinName(brain_pin_e /*brainPin*/) {
	return nullptr;
}

static const char *hwOnChipPhysicalPinName(ioportid_t hwPort, int hwPin) {
  portNameStream.eos = 0; // reset
	if (hwPort == GPIO_NULL) {
		return "NONE";
	}
	chprintf((BaseSequentialStream *) &portNameStream, "%s%d", portname(hwPort), hwPin);
	portNameStream.buffer[portNameStream.eos] = 0; // need to terminate explicitly
  return portNameBuffer;
}

const char *hwPhysicalPinName(Gpio brainPin) {
	if (brainPin == Gpio::Invalid) {
		return "INVALID";
	}
	if (brainPin == Gpio::Unassigned) {
		return "NONE";
	}

	if (brain_pin_is_onchip(brainPin)) {
		ioportid_t hwPort = getHwPort("hostname", brainPin);
		int hwPin = getHwPin("hostname", brainPin);
	  return hwOnChipPhysicalPinName(hwPort, hwPin);
	}
	#if (BOARD_EXT_GPIOCHIPS > 0)
		else {
	    portNameStream.eos = 0; // reset
			const char *pin_name = gpiochips_getPinName(brainPin);

			if (pin_name) {
				chprintf((BaseSequentialStream *) &portNameStream, "ext:%s",
					pin_name);
			} else {
				chprintf((BaseSequentialStream *) &portNameStream, "ext:%s.%d",
					gpiochips_getChipName(brainPin), gpiochips_getPinOffset(brainPin));
			}
	    portNameStream.buffer[portNameStream.eos] = 0; // need to terminate explicitly
	    return portNameBuffer;
		}
	#endif
  return "unexpected";
}

const char *hwPortname(brain_pin_e brainPin) {
	const char * boardSpecificPinName = getBoardSpecificPinName(brainPin);
	if (boardSpecificPinName != nullptr) {
		return boardSpecificPinName;
	}
	return hwPhysicalPinName(brainPin);
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
	if ((brainPin < Gpio::A0) || (brainPin > BRAIN_PIN_ONCHIP_LAST))
		return false;

	return true;
}

bool brain_pin_is_ext(brain_pin_e brainPin)
{
	if (brainPin > BRAIN_PIN_ONCHIP_LAST)
		return true;

	return false;
}

/**
 * Marks on-chip gpio port-pin as used. Works only for on-chip gpios
 * To be replaced with brain_pin_markUsed later
 */

bool gpio_pin_markUsed(ioportid_t port, ioportmask_t pin, const char *msg) {
	int index = getPortPinIndex(port, pin);
#ifndef EFI_BOOTLOADER
	efiPrintf("pin_markUsed: %s on %s", msg, hwOnChipPhysicalPinName(port, pin));
#endif

	if (getBrainUsedPin(index) != NULL) {
		/**
		 * todo: the problem is that this warning happens before the console is even
		 * connected, so the warning is never displayed on the console and that's quite a problem!
		 */
//		warning(ObdCode::OBD_PCM_Processor_Fault, "%s%d req by %s used by %s", portname(port), pin, msg, getBrainUsedPin(index));
		firmwareError(ObdCode::CUSTOM_ERR_PIN_ALREADY_USED_1, "%s%d req by %s used by %s", portname(port), (int)pin, msg, getBrainUsedPin(index));
		return true;
	}
	getBrainUsedPin(index) = msg;
	return false;
}

/**
 * Marks on-chip gpio port-pin as UNused. Works only for on-chip gpios
 * To be replaced with brain_pin_markUnused later
 */

void gpio_pin_markUnused(ioportid_t port, ioportmask_t pin) {
	int index = getPortPinIndex(port, pin);

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
const char *hwPhysicalPinName(Gpio brainPin) {
	return "N/A";
}
const char *hwPortname(Gpio brainPin) {
	(void)brainPin;
	return "N/A";
}
#endif /* EFI_PROD_CODE */
