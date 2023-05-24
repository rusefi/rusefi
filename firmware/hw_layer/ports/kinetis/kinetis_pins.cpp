/**
 * @file	kinetis_pins.cpp
 * @brief	Kinetis-compatible GPIO code
 *
 * @date Jun 02, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 * @author andreika <prometheus.pcb@gmail.com>
 */

#include "pch.h"

#if EFI_GPIO_HARDWARE

static ioportid_t ports[] = {GPIOA,
		GPIOB,
		GPIOC,
		GPIOD,
		GPIOE
};

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
	firmwareError(ObdCode::CUSTOM_ERR_UNKNOWN_PORT, "unknown port");
	return -1;
}

ioportid_t * getGpioPorts() {
    return ports;
}

int getPortPinIndex(ioportid_t port, ioportmask_t pin) {
	int portIndex = getPortIndex(port);
	return portIndex * PORT_SIZE + pin;
}

/**
 * Parse string representation of physical pin into Gpio ordinal.
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
	brain_pin_e basePin;
	if (port >= 'a' && port <= 'z') {
		basePin = (Gpio::A0 + PORT_SIZE * (port - 'a'));
	} else if (port >= 'A' && port <= 'Z') {
		basePin = (Gpio::A0 + PORT_SIZE * (port - 'A'));
	} else {
		return Gpio::Invalid;
	}
	const char *pinStr = str + 2;
	int pin = atoi(pinStr);
	return basePin + pin;
}

unsigned int getBrainPinOnchipNum(void) {
	return BRAIN_PIN_ONCHIP_PINS;
}

#endif /* EFI_GPIO_HARDWARE */
