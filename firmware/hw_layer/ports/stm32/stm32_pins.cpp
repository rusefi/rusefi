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

#define PORT_SIZE 16

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
#if STM32_HAS_GPIOE
	if (GPIOx == GPIOE)
		return "PE";
#endif /* STM32_HAS_GPIOE */
#if STM32_HAS_GPIOF
	if (GPIOx == GPIOF)
		return "PF";
#endif /* STM32_HAS_GPIOF */
#if STM32_HAS_GPIOG
	if (GPIOx == GPIOG)
		return "PG";
#endif /* STM32_HAS_GPIOG */
#if STM32_HAS_GPIOH
	if (GPIOx == GPIOH)
		return "PH";
#endif /* STM32_HAS_GPIOH */
#if STM32_HAS_GPIOI
	if (GPIOx == GPIOI)
		return "PI";
#endif /* STM32_HAS_GPIOI */
#if STM32_HAS_GPIOJ
	if (GPIOx == GPIOJ)
		return "PJ";
#endif /* STM32_HAS_GPIOJ */
#if STM32_HAS_GPIOK
	if (GPIOx == GPIOK)
		return "PK";
#endif /* STM32_HAS_GPIOK */
	return "unknown";
}

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
#if STM32_HAS_GPIOI
	if (port == GPIOI)
		return 8;
#endif /* STM32_HAS_GPIOI */
#if STM32_HAS_GPIOJ
	if (port == GPIOJ)
		return 9;
#endif /* STM32_HAS_GPIOJ */
#if STM32_HAS_GPIOK
	if (port == GPIOK)
		return 10;
#endif /* STM32_HAS_GPIOK */
	firmwareError(CUSTOM_ERR_UNKNOWN_PORT, "unknown port");
	return -1;
}

ioportid_t getBrainPinPort(brain_pin_e brainPin) {
	return ports[(brainPin - GPIOA_0) / PORT_SIZE];
}

int getBrainPinIndex(brain_pin_e brainPin) {
	return (brainPin - GPIOA_0) % PORT_SIZE;
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
		firmwareError(CUSTOM_ERR_INVALID_PIN, "%s: Invalid brain_pin_e: %d", msg, brainPin);
 */
		return GPIO_NULL;
	}
	return ports[(brainPin - GPIOA_0) / PORT_SIZE];
}

/**
 * this method returns the numeric part of pin name. For instance, for PC13 this would return '13'
 */
ioportmask_t getHwPin(const char *msg, brain_pin_e brainPin)
{
	if (!isBrainPinValid(brainPin))
			return EFI_ERROR_CODE;

	if (brain_pin_is_onchip(brainPin))
		return getBrainPinIndex(brainPin);

	firmwareError(CUSTOM_ERR_INVALID_PIN, "%s: Invalid on-chip brain_pin_e: %d", msg, brainPin);
	return EFI_ERROR_CODE;
}

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
		basePin = (brain_pin_e) ((int) GPIOA_0 + PORT_SIZE * (port - 'a'));
	} else if (port >= 'A' && port <= 'Z') {
		basePin = (brain_pin_e) ((int) GPIOA_0 + PORT_SIZE * (port - 'A'));
	} else {
		return GPIO_INVALID;
	}
	const char *pinStr = str + 2;
	int pin = atoi(pinStr);
	return (brain_pin_e)(basePin + pin);
}

unsigned int getBrainPinOnchipNum(void) {
	return BRAIN_PIN_ONCHIP_PINS;
}

#endif /* EFI_GPIO_HARDWARE */
