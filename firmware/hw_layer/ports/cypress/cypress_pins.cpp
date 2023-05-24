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
	if (port == GPIOE)
		return 4;
	if (port == GPIOF)
		return 5;
	if (port == GPIOG)
		return 6;
	if (port == GPIOH)
		return 7;
	if (port == GPIOI)
		return 8;
	if (port == GPIOJ)
		return 9;
	if (port == GPIOK)
		return 10;
	firmwareError(ObdCode::CUSTOM_ERR_UNKNOWN_PORT, "unknown port");
	return -1;
}

int getBrainPinIndex(ioportid_t port, ioportmask_t pin) {
	int portIndex = getPortIndex(port);
	return portIndex * PORT_SIZE + pin;
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

#endif /* EFI_GPIO_HARDWARE */
