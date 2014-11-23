/**
 * @file	io_pins.c
 * @brief	It could be that the main purpose of this file is the status LED blinking
 *
 * @date Jan 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
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

#if EFI_ENGINE_CONTROL
#include "main_trigger_callback.h"
#endif /* EFI_ENGINE_CONTROL */

extern board_configuration_s *boardConfiguration;

static Logging logger;

extern pin_output_mode_e *pinDefaultState[IO_PIN_COUNT];
extern OutputPin outputs[IO_PIN_COUNT];
static io_pin_e leds[] = { LED_WARNING, LED_RUNNING, LED_ERROR, LED_COMMUNICATION_1, LED_DEBUG, LED_EXT_1,
		LED_CHECK_ENGINE };

static GPIO_TypeDef *PORTS[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH };

static pin_output_mode_e DEFAULT_OUTPUT = OM_DEFAULT;

inline static void assertOMode(pin_output_mode_e mode) {
	// mode >= 0  is always true since that's an unsigned
	efiAssertVoid(mode <= OM_OPENDRAIN_INVERTED, "invalid pin_output_mode_e");
}

void setDefaultPinState(io_pin_e pin, pin_output_mode_e *outputMode) {
	pin_output_mode_e mode = *outputMode;
	assertOMode(mode);
	pinDefaultState[pin] = outputMode;
	setOutputPinValue(pin, FALSE); // initial state
}

static void outputPinRegisterExt(const char *msg, io_pin_e ioPin, GPIO_TypeDef *port, uint32_t pin,
		pin_output_mode_e *outputMode) {
	efiAssertVoid((int)ioPin < IO_PIN_COUNT, "io pin out of range");
	if (port == GPIO_NULL) {
		// that's for GRIO_NONE
		outputs[ioPin].port = port;
		return;
	}

	assertOMode(*outputMode);
	iomode_t mode = (*outputMode == OM_DEFAULT || *outputMode == OM_INVERTED) ?
	PAL_MODE_OUTPUT_PUSHPULL :
																				PAL_MODE_OUTPUT_OPENDRAIN;

	initOutputPinExt(msg, &outputs[ioPin], port, pin, mode);

	setDefaultPinState(ioPin, outputMode);
}

GPIO_TypeDef * getHwPort(brain_pin_e brainPin) {
	if (brainPin == GPIO_UNASSIGNED)
		return GPIO_NULL;
	if (brainPin > GPIO_UNASSIGNED || brainPin < 0) {
		firmwareError("Invalid brain_pin_e: %d", brainPin);
		return GPIO_NULL;
	}
	return PORTS[brainPin / 16];
}

ioportmask_t getHwPin(brain_pin_e brainPin) {
	if (brainPin == GPIO_UNASSIGNED)
		return EFI_ERROR_CODE;
	if (brainPin > GPIO_UNASSIGNED || brainPin < 0) {
		firmwareError("Invalid brain_pin_e: %d", brainPin);
		return EFI_ERROR_CODE;
	}
	return brainPin % 16;
}

void outputPinRegisterExt2(const char *msg, io_pin_e ioPin, brain_pin_e brainPin, pin_output_mode_e *outputMode) {
	GPIO_TypeDef *hwPort = getHwPort(brainPin);
	int hwPin = getHwPin(brainPin);

	outputPinRegisterExt(msg, ioPin, hwPort, hwPin, outputMode);
}

void outputPinRegister(const char *msg, io_pin_e ioPin, GPIO_TypeDef *port, uint32_t pin) {
	outputPinRegisterExt(msg, ioPin, port, pin, &DEFAULT_OUTPUT);
}

/**
 * This method would blink all the LEDs just to test them
 */
static void initialLedsBlink(void) {
	int size = sizeof(leds) / sizeof(leds[0]);
	for (int i = 0; i < size; i++)
		setOutputPinValue(leds[i], 1);

	chThdSleepMilliseconds(100);

	for (int i = 0; i < size; i++)
		setOutputPinValue(leds[i], 0);
}

void initPrimaryPins(void) {
	outputPinRegister("LED_ERROR", LED_ERROR, LED_ERROR_PORT, LED_ERROR_PIN);
}

static void getPinValue(const char *name) {
	io_pin_e pin = getPinByName(name);
	if (pin == IO_INVALID) {
		return;
	}
	OutputPin * outputPin = &outputs[pin];
	int value = getLogicPinValue(outputPin);
	scheduleMsg(&logger, "pin_value %s %d", name, value);
}

void initOutputPins(void) {
	initLogging(&logger, "io_pins");

	outputPinRegister("warning", LED_WARNING, LED_WARNING_PORT, LED_WARNING_PIN);
	outputPinRegister("is running status", LED_RUNNING, LED_RUNNING_STATUS_PORT, LED_RUNNING_STATUS_PIN);
	outputPinRegister("communication status 1", LED_COMMUNICATION_1, LED_COMMUNICATION_PORT, LED_COMMUNICATION_PIN);

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
	outputPinRegisterExt2("MalfunctionIndicator", LED_CHECK_ENGINE, boardConfiguration->malfunctionIndicatorPin, &DEFAULT_OUTPUT);

// todo: are these needed here? todo: make configurable
//	outputPinRegister("spi CS1", SPI_CS_1, SPI_CS1_PORT, SPI_CS1_PIN);
//	outputPinRegister("spi CS2", SPI_CS_2, SPI_CS2_PORT, SPI_CS2_PIN);
//	outputPinRegister("spi CS3", SPI_CS_3, SPI_CS3_PORT, SPI_CS3_PIN);
//	outputPinRegister("spi CS4", SPI_CS_4, SPI_CS4_PORT, SPI_CS4_PIN);
	outputPinRegister("spi CS5", SPI_CS_SD_MODULE, SPI_SD_MODULE_PORT, SPI_SD_MODULE_PIN);

	// todo: should we move this code closer to the fuel pump logic?
	outputPinRegisterExt2("fuel pump relay", FUEL_PUMP_RELAY, boardConfiguration->fuelPumpPin, &DEFAULT_OUTPUT);

	outputPinRegisterExt2("fan relay", FAN_RELAY, boardConfiguration->fanPin, &DEFAULT_OUTPUT);
	outputPinRegisterExt2("o2 heater", O2_HEATER, boardConfiguration->o2heaterPin, &DEFAULT_OUTPUT);
	outputPinRegisterExt2("trg_err", LED_TRIGGER_ERROR, boardConfiguration->triggerErrorPin, &boardConfiguration->triggerErrorPinMode);
	outputPinRegisterExt2("A/C relay", AC_RElAY, boardConfiguration->acRelayPin, &boardConfiguration->acRelayPinMode);

	initialLedsBlink();

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

	addConsoleActionS("get_pin_value", getPinValue);
}

static io_pin_e TO_BE_TURNED_OFF_ON_ERROR[] = { SPARKOUT_1_OUTPUT, SPARKOUT_2_OUTPUT, SPARKOUT_3_OUTPUT,
		SPARKOUT_4_OUTPUT, SPARKOUT_5_OUTPUT, SPARKOUT_6_OUTPUT, SPARKOUT_7_OUTPUT, SPARKOUT_8_OUTPUT,
		SPARKOUT_9_OUTPUT, SPARKOUT_10_OUTPUT, SPARKOUT_11_OUTPUT, SPARKOUT_12_OUTPUT,

		INJECTOR_1_OUTPUT, INJECTOR_2_OUTPUT, INJECTOR_3_OUTPUT, INJECTOR_4_OUTPUT, INJECTOR_5_OUTPUT,
		INJECTOR_6_OUTPUT, INJECTOR_7_OUTPUT, INJECTOR_8_OUTPUT, INJECTOR_9_OUTPUT, INJECTOR_10_OUTPUT,
		INJECTOR_11_OUTPUT, INJECTOR_12_OUTPUT, FUEL_PUMP_RELAY };

/**
 * This method is part of fatal error handling.
 * Please note that worst case scenario the pins might get re-enabled by some other code :(
 * The whole method is pretty naive, but that's at least something.
 */
void turnAllPinsOff(void) {
	int l = sizeof(TO_BE_TURNED_OFF_ON_ERROR) / sizeof(io_pin_e);
	for (int i = 0; i < l; i++) {
		turnOutputPinOff(l);
	}
}

