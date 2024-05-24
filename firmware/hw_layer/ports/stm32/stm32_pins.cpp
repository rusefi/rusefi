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

static const struct port_io {
	const ioportid_t port;
	const char *name;
} ports[] = {
	{ GPIOA, "PA" },
	{ GPIOB, "PB" },
	{ GPIOC, "PC" },
	{ GPIOD, "PD" },
#if STM32_HAS_GPIOE
	{ GPIOE, "PE" },
#endif /* STM32_HAS_GPIOE */
#if STM32_HAS_GPIOF
	{ GPIOF, "PF" },
#endif /* STM32_HAS_GPIOF */
#if STM32_HAS_GPIOG
	{ GPIOG, "PG" },
#endif /* STM32_HAS_GPIOG */
#if STM32_HAS_GPIOH
	{ GPIOH, "PH" },
#endif /* STM32_HAS_GPIOH */
#if STM32_HAS_GPIOI
	{ GPIOI, "PI" },
#endif /* STM32_HAS_GPIOI */
#if STM32_HAS_GPIOJ
	{ GPIOJ, "PJ" },
#endif /* STM32_HAS_GPIOJ */
#if STM32_HAS_GPIOK
	{ GPIOK, "PK" },
#endif /* STM32_HAS_GPIOK */
};

int getBrainPinIndex(brain_pin_e brainPin) {
	return (brainPin - Gpio::A0) % PORT_SIZE;
}

ioportid_t getBrainPinPort(brain_pin_e brainPin) {
	size_t idx = (brainPin - Gpio::A0) / PORT_SIZE;
	if (idx < efi::size(ports)) {
		return ports[idx].port;
	}

	return nullptr;
}

const char *portname(ioportid_t port) {
	efiAssert(ObdCode::CUSTOM_ERR_ASSERT, port != NULL, "null port", nullptr);
	for (size_t idx = 0; idx < efi::size(ports); idx++) {
		if (ports[idx].port == port) {
			return ports[idx].name;
		}
	}
	return nullptr;
}

static int getPortIndex(ioportid_t port) {
	for (size_t idx = 0; idx < efi::size(ports); idx++) {
		if (ports[idx].port == port) {
			return idx;
		}
	}

#ifndef EFI_BOOTLOADER
	firmwareError(ObdCode::CUSTOM_ERR_UNKNOWN_PORT, "unknown port");
#endif

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
	size_t idx = (brainPin - Gpio::A0) / PORT_SIZE;
	if (idx < efi::size(ports)) {
		return ports[idx].port;
	}

#ifndef EFI_BOOTLOADER
	firmwareError(ObdCode::CUSTOM_ERR_UNKNOWN_PORT, "unknown port");
#endif

	return nullptr;
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
#ifndef EFI_BOOTLOADER
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

static const char *mode_names[4] = { "Input", "GP out", "Alt", "Analog" };
static const char *od_names[2] = { "Push-Pull", "Open-Drain" };
static const char *speed_names[4] = { "Low", "Medium", "High", "Very High" };
static const char *pull_names[4] = { "No pull", "Pull-up", "Pull-down", "Reserved" };

void debugBrainPin(char *buffer, size_t size, brain_pin_e brainPin) {
	ioportid_t port = getBrainPinPort(brainPin);
	int pin = getBrainPinIndex(brainPin);

	if (port == nullptr) {
		buffer[0] = '\0';
		return;
	}

	uint32_t af = (pin < 8) ? port->AFRL : port->AFRH;
	af = (af >> (4 * (pin & 0x07))) & 0x0f;

	chsnprintf(buffer, size, "Mode %s AF%d %s %s Speed %s IN %d OUT %d",
			mode_names[(port->MODER >> (pin * 2)) & 0x03],
			af,
			od_names[(port->OTYPER >> pin) & 0x01],
			pull_names[(port->PUPDR >> (pin * 2)) & 0x03],
			speed_names[(port->OSPEEDR >> (pin * 2)) & 0x03],
			(port->IDR >> pin) & 0x01,
			(port->ODR >> pin) & 0x01);
}

#endif /* EFI_GPIO_HARDWARE */
