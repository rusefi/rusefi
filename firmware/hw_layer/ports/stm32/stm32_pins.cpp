/**
 * @file	stm32_pins.cpp
 * @brief	STM32-compatible GPIO code
 *
 * @date Jun 02, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "smart_gpio.h"

#if EFI_GPIO_HARDWARE

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
#if STM32_HAS_GPIOI
		GPIOI,
#else
		nullptr,
#endif /* STM32_HAS_GPIOI */
#if STM32_HAS_GPIOJ
		GPIOJ,
#else
		nullptr,
#endif /* STM32_HAS_GPIOJ */
#if STM32_HAS_GPIOK
		GPIOK,
#else
		nullptr,
#endif /* STM32_HAS_GPIOK */
};

ioportid_t * getGpioPorts() {
    return ports;
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
		basePin = Gpio::A0 + PORT_SIZE * (port - 'a');
	} else if (port >= 'A' && port <= 'Z') {
		basePin = Gpio::A0 + PORT_SIZE * (port - 'A');
	} else {
		return Gpio::Invalid;
	}
	const char *pinStr = str + 2;
	int pin = atoi(pinStr);
	return (brain_pin_e)(basePin + pin);
}

unsigned int getBrainPinOnchipNum(void) {
	return BRAIN_PIN_ONCHIP_PINS;
}

#endif /* EFI_GPIO_HARDWARE */
