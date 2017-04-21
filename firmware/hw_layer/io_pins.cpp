/**
 * @file	io_pins.cpp
 * @brief	It could be that the main purpose of this file is the status LED blinking
 *
 * @date Jan 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include <board.h>
#include "main.h"
#include "io_pins.h"
#include "efiGpio.h"

#include "pin_repository.h"
#include "gpio_helper.h"
#include "status_loop.h"
#include "engine_configuration.h"
#include "console_io.h"

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
#include "main_trigger_callback.h"
#endif /* EFI_ENGINE_CONTROL */

extern board_configuration_s *boardConfiguration;

static LoggingWithStorage logger("io_pins");


extern EnginePins enginePins;

#if defined(STM32F4XX)
static ioportid_t PORTS[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH };
#else
static ioportid_t PORTS[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOF};
#endif

pin_output_mode_e DEFAULT_OUTPUT = OM_DEFAULT;
pin_output_mode_e OPENDRAIN_OUTPUT = OM_OPENDRAIN;

/**
 * This method is used for digital GPIO pins only, for peripheral pins see mySetPadMode
 */
static void outputPinRegisterExt(const char *msg, OutputPin *output, ioportid_t port, uint32_t pin,
		pin_output_mode_e *outputMode) {
#if EFI_GPIO
	if (port == GPIO_NULL) {
		// that's for GRIO_NONE
		output->port = port;
		return;
	}

	assertOMode(*outputMode);
	iomode_t mode = (*outputMode == OM_DEFAULT || *outputMode == OM_INVERTED) ?
	PAL_MODE_OUTPUT_PUSHPULL :
																				PAL_MODE_OUTPUT_OPENDRAIN;

	initOutputPinExt(msg, output, port, pin, mode);

	output->setDefaultPinState(outputMode);
#endif
}

ioportid_t getHwPort(brain_pin_e brainPin) {
	if (brainPin == GPIO_UNASSIGNED)
		return GPIO_NULL;
	if (brainPin > GPIO_UNASSIGNED || brainPin < 0) {
		firmwareError(CUSTOM_ERR_INVALID_PIN, "Invalid brain_pin_e: %d", brainPin);
		return GPIO_NULL;
	}
	return PORTS[brainPin / PORT_SIZE];
}

ioportmask_t getHwPin(brain_pin_e brainPin) {
	if (brainPin == GPIO_UNASSIGNED)
		return EFI_ERROR_CODE;
	if (brainPin > GPIO_UNASSIGNED || brainPin < 0) {
		firmwareError(CUSTOM_ERR_INVALID_PIN, "Invalid brain_pin_e: %d", brainPin);
		return EFI_ERROR_CODE;
	}
	return brainPin % PORT_SIZE;
}

void outputPinRegisterExt2(const char *msg, OutputPin *output, brain_pin_e brainPin, pin_output_mode_e *outputMode) {
	if (brainPin == GPIO_UNASSIGNED)
		return;
	ioportid_t hwPort = getHwPort(brainPin);
	int hwPin = getHwPin(brainPin);

	outputPinRegisterExt(msg, output, hwPort, hwPin, outputMode);
}

void outputPinRegister(const char *msg, OutputPin *output, ioportid_t port, uint32_t pin) {
	outputPinRegisterExt(msg, output, port, pin, &DEFAULT_OUTPUT);
}


void initPrimaryPins(void) {
	outputPinRegister("led: ERROR status", &enginePins.errorLedPin, LED_ERROR_PORT, LED_ERROR_PIN);
}

void initOutputPins(void) {
	/**
	 * want to make sure it's all zeros so that we can compare in initOutputPinExt() method
	 */
// todo: it's too late to clear now? this breaks default status LEDs
// todo: fix this?
//	memset(&outputs, 0, sizeof(outputs));
//	outputPinRegister("ext led 1", LED_EXT_1, EXTRA_LED_1_PORT, EXTRA_LED_1_PIN);
//	outputPinRegister("ext led 2", LED_EXT_2, EXTRA_LED_2_PORT, EXTRA_LED_2_PIN);
//	outputPinRegister("ext led 3", LED_EXT_3, EXTRA_LED_2_PORT, EXTRA_LED_3_PIN);
//	outputPinRegister("alive1", LED_DEBUG, GPIOD, 6);

#if HAL_USE_SPI || defined(__DOXYGEN__)
	outputPinRegisterExt2("spi CS5", &enginePins.sdCsPin, boardConfiguration->sdCardCsPin, &DEFAULT_OUTPUT);
#endif

	// todo: should we move this code closer to the fuel pump logic?
	outputPinRegisterExt2("fuel pump relay", &enginePins.fuelPumpRelay, boardConfiguration->fuelPumpPin, &DEFAULT_OUTPUT);

	outputPinRegisterExt2("main relay", &enginePins.mainRelay, boardConfiguration->mainRelayPin, &boardConfiguration->mainRelayPinMode);

	outputPinRegisterExt2("fan relay", &enginePins.fanRelay, boardConfiguration->fanPin, &DEFAULT_OUTPUT);
	outputPinRegisterExt2("o2 heater", &enginePins.o2heater, boardConfiguration->o2heaterPin, &DEFAULT_OUTPUT);
	outputPinRegisterExt2("A/C relay", &enginePins.acRelay, boardConfiguration->acRelayPin, &boardConfiguration->acRelayPinMode);

	// digit 1
	/*
	 ledRegister(LED_HUGE_0, GPIOB, 2);
	 ledRegister(LED_HUGE_1, GPIOE, 7);
	 ledRegister(LED_HUGE_2, GPIOE, 8);
	 ledRegister(LED_HUGE_3, GPIOE, 9);
	 ledRegister(LED_HUGE_4, GPIOE, 10);
	 ledRegister(LED_HUGE_5, GPIOE, 11);
	 ledRegister(LED_HUGE_6, GPIOE, 12);

	 // digit 2
	 ledRegister(LED_HUGE_7, GPIOE, 13);
	 ledRegister(LED_HUGE_8, GPIOE, 14);
	 ledRegister(LED_HUGE_9, GPIOE, 15);
	 ledRegister(LED_HUGE_10, GPIOB, 10);
	 ledRegister(LED_HUGE_11, GPIOB, 11);
	 ledRegister(LED_HUGE_12, GPIOB, 12);
	 ledRegister(LED_HUGE_13, GPIOB, 13);

	 // digit 3
	 ledRegister(LED_HUGE_14, GPIOE, 0);
	 ledRegister(LED_HUGE_15, GPIOE, 2);
	 ledRegister(LED_HUGE_16, GPIOE, 4);
	 ledRegister(LED_HUGE_17, GPIOE, 6);
	 ledRegister(LED_HUGE_18, GPIOE, 5);
	 ledRegister(LED_HUGE_19, GPIOE, 3);
	 ledRegister(LED_HUGE_20, GPIOE, 1);
	 */
}

#if EFI_GPIO

/**
 * This method is part of fatal error handling.
 * Please note that worst case scenario the pins might get re-enabled by some other code :(
 * The whole method is pretty naive, but that's at least something.
 */
void turnAllPinsOff(void) {
	for (int i = 0; i < INJECTION_PIN_COUNT; i++) {
		enginePins.injectors[i].setValue(false);
	}
	for (int i = 0; i < IGNITION_PIN_COUNT; i++) {
		enginePins.coils[i].setValue(false);
	}
}
#endif
