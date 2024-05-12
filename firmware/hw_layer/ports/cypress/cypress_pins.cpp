/**
 * @file	cypress_pins.cpp
 * @brief	Cypress-compatible GPIO code
 *
 * @date Jun 02, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 * @author andreika <prometheus.pcb@gmail.com>
 */

#include "pch.h"

#if EFI_GPIO_HARDWARE

static ioportid_t ports[] = {
	GPIOA,
	GPIOB,
	GPIOC,
	GPIOD,
	GPIOE,
	GPIOF,
	GPIOG,
	GPIOH,
	GPIOI,
	GPIOJ,
	GPIOK,
};

static brain_pin_e portMap[16] = {
	Gpio::A0, Gpio::B0, Gpio::C0, Gpio::D0, Gpio::E0, Gpio::F0, Gpio::Invalid, Gpio::G0, Gpio::Invalid, Gpio::Invalid, Gpio::H0, Gpio::I0, Gpio::J0, Gpio::Invalid, Gpio::Invalid, Gpio::K0
};

#include "pin_repository.h"
#include "io_pins.h"

ioportid_t * getGpioPorts() {
    return ports;
}

int getBrainPinIndex(brain_pin_e brainPin) {
	return (brainPin - Gpio::A0) % PORT_SIZE;
}

ioportid_t getBrainPinPort(brain_pin_e brainPin) {
	return getGpioPorts()[(brainPin - Gpio::A0) / PORT_SIZE];
}

/**
 * @deprecated - use hwPortname() instead
 */
const char *portname(ioportid_t GPIOx) {
	if (GPIOx == GPIOA)
		return "PA";
	if (GPIOx == GPIOB)
		return "PB";
	if (GPIOx == GPIOC)
		return "PC";
	if (GPIOx == GPIOD)
		return "PD";
#if defined(GPIOF)
	if (GPIOx == GPIOE)
		return "PE";
#endif /* GPIOE */
#if defined(GPIOF)
	if (GPIOx == GPIOF)
		return "PF";
#endif /* GPIOF */
#if defined(GPIOG)
	if (GPIOx == GPIOG)
		return "PG";
#endif /* GPIOG */
#if defined(GPIOH)
	if (GPIOx == GPIOH)
		return "PH";
#endif /* GPIOH */
#if defined(GPIOI)
	if (GPIOx == GPIOI)
		return "PI";
#endif /* GPIOI */
#if defined(GPIOJ_BASE)
	if (GPIOx == GPIOJ)
		return "PJ";
#endif /* GPIOJ_BASE */
#if defined(GPIOK_BASE)
	if (GPIOx == GPIOK)
		return "PK";
#endif /* GPIOK_BASE */
	return "unknown";
}

static int getPortIndex(ioportid_t port) {
	efiAssert(ObdCode::CUSTOM_ERR_ASSERT, port != NULL, "null port", -1);
	if (port == GPIOA)
		return 0;
	if (port == GPIOB)
		return 1;
	if (port == GPIOC)
		return 2;
	if (port == GPIOD)
		return 3;
#if defined(GPIOF)
	if (port == GPIOE)
		return 4;
#endif /* GPIOE */
#if defined(GPIOF)
	if (port == GPIOF)
		return 5;
#endif /* GPIOF */
#if defined(GPIOG)
	if (port == GPIOG)
		return 6;
#endif /* GPIOG */
#if defined(GPIOH)
	if (port == GPIOH)
		return 7;
#endif /* GPIOH */
#if defined(GPIOI)
	if (port == GPIOI)
		return 8;
#endif /* STM32_HAS_GPIOI */
#if defined(GPIOJ_BASE)
	if (port == GPIOJ)
		return 9;
#endif /* GPIOJ_BASE */
#if defined(GPIOK_BASE)
	if (port == GPIOK)
		return 10;
#endif /* GPIOK_BASE */
	firmwareError(ObdCode::CUSTOM_ERR_UNKNOWN_PORT, "unknown port");
	return -1;
}

int getPortPinIndex(ioportid_t port, ioportmask_t pin) {
	int portIndex = getPortIndex(port);
	return portIndex * PORT_SIZE + pin;
}

ioportid_t getHwPort(const char *msg, brain_pin_e brainPin) {
	(void)msg;

	if (!isBrainPinValid(brainPin)) {
/*
 *  https://github.com/dron0gus please help
		firmwareError(ObdCode::CUSTOM_ERR_INVALID_PIN, "%s: Invalid Gpio: %d", msg, brainPin);
 */
		return GPIO_NULL;
	}
	return getGpioPorts()[(brainPin - Gpio::A0) / PORT_SIZE];
}

/**
 * this method returns the numeric part of pin name. For instance, for PC13 this would return '13'
 */
ioportmask_t getHwPin(const char *msg, brain_pin_e brainPin) {
	if (!isBrainPinValid(brainPin))
			return EFI_ERROR_CODE;

	if (brain_pin_is_onchip(brainPin))
		return getBrainPinIndex(brainPin);

	firmwareError(ObdCode::CUSTOM_ERR_INVALID_PIN, "%s: Invalid on-chip Gpio: %d", msg, brainPin);
	return EFI_ERROR_CODE;
}

/**
 * Parse string representation of physical pin into brain_pin_e ordinal.
 *
 * @return Gpio::Unassigned for "none", Gpio::Invalid for invalid entry
 */
brain_pin_e parseBrainPin(const char *str) {
	if (strEqual(str, "none"))
		return Gpio::Unassigned;
	// todo: create method toLowerCase?
	if (str[0] != 'p' && str[0] != 'P') {
		return Gpio::Invalid;
	}
	char port = str[1];
	if (port >= 'a' && port <= 'z') {
		port = 10 + (port - 'a');
	} else if (port >= 'A' && port <= 'Z') {
		port = 10 + (port - 'A');
	} else if (port >= '0' && port <= '9') {
// cypress-specific code
		port = 0 + (port - '0');
	} else {
		return Gpio::Invalid;
	}
	brain_pin_e basePin = portMap[(int)port];
	if (basePin == Gpio::Invalid)
		return Gpio::Invalid;
	const char *pinStr = str + 2;
	int pin = atoi(pinStr);
	return basePin + pin;
}

unsigned int getBrainPinOnchipNum(void) {
	return BRAIN_PIN_ONCHIP_PINS;
}

void debugBrainPin(char *buffer, size_t, brain_pin_e)
{
	buffer[0] = '\0';
}

#endif /* EFI_GPIO_HARDWARE */
