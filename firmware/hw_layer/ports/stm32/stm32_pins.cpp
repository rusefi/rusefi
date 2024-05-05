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

static const ioportid_t ports[] = {GPIOA,
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

int getBrainPinIndex(brain_pin_e brainPin) {
	return (brainPin - Gpio::A0) % PORT_SIZE;
}

ioportid_t getBrainPinPort(brain_pin_e brainPin) {
	return ports[(brainPin - Gpio::A0) / PORT_SIZE];
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
	return ports[(brainPin - Gpio::A0) / PORT_SIZE];
}

/**
 * this method returns the numeric part of pin name. For instance, for PC13 this would return '13'
 */
ioportmask_t getHwPin(const char *msg, brain_pin_e brainPin) {
	if (!isBrainPinValid(brainPin))
			return EFI_ERROR_CODE;

	if (brain_pin_is_onchip(brainPin))
		return getBrainPinIndex(brainPin);


// huh why conditional on EFI_BOOTLOADER? some weird technical debt while does it fail only with debug options?
#if ! EFI_BOOTLOADER
	criticalError("%s: Invalid on-chip Gpio: %d", msg, brainPin);
#endif // EFI_BOOTLOADER
	return EFI_ERROR_CODE;
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
