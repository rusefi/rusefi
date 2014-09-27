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

#include "pin_repository.h"
#include "gpio_helper.h"
#include "status_loop.h"
#include "engine_configuration.h"
#include "console_io.h"
#include "flash_main.h"

#if EFI_ENGINE_CONTROL
#include "main_trigger_callback.h"
#endif /* EFI_ENGINE_CONTROL */

extern board_configuration_s *boardConfiguration;

static Logging logger;

static pin_output_mode_e *pinDefaultState[IO_PIN_COUNT];
extern OutputPin outputs[IO_PIN_COUNT];
static io_pin_e leds[] = { LED_WARNING, LED_RUNNING, LED_ERROR, LED_COMMUNICATION_1, LED_DEBUG, LED_EXT_1,
		LED_CHECK_ENGINE };

static GPIO_TypeDef *PORTS[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH };

static pin_output_mode_e DEFAULT_OUTPUT = OM_DEFAULT;

/**
 * blinking thread to show that we are alive
 */
static THD_WORKING_AREA(comBlinkingStack, UTILITY_THREAD_STACK_SIZE);

/**
 * error thread to show error condition (blinking LED means non-fatal error)
 */
static THD_WORKING_AREA(errBlinkingStack, UTILITY_THREAD_STACK_SIZE);

void turnOutputPinOn(io_pin_e pin) {
	setOutputPinValue(pin, TRUE);
}

void turnOutputPinOff(io_pin_e pin) {
	setOutputPinValue(pin, FALSE);
}

inline static void assertOMode(pin_output_mode_e mode) {
	// mode >= 0  is always true since that's an unsigned
	efiAssertVoid(mode <= OM_OPENDRAIN_INVERTED, "invalid pin_output_mode_e");
}

/**
 * @brief Sets the value according to current electrical settings
 */
void setOutputPinValue(io_pin_e pin, int logicValue) {
	if (outputs[pin].port == GPIO_NULL)
		return;
	efiAssertVoid(pinDefaultState[pin]!=NULL, "pin mode not initialized");
	pin_output_mode_e mode = *pinDefaultState[pin];
	setPinValue(&outputs[pin], getElectricalValue(logicValue, mode), logicValue);
}

void setDefaultPinState(io_pin_e pin, pin_output_mode_e *outputMode) {
	pin_output_mode_e mode = *outputMode;
	assertOMode(mode);
	pinDefaultState[pin] = outputMode;
	setOutputPinValue(pin, FALSE); // initial state
}

static void comBlinkingThread(void *arg) {
	(void) arg;
	chRegSetThreadName("communication blinking");
	while (TRUE) {
		int delay;
		if (getNeedToWriteConfiguration()) {
			delay = isConsoleReady() ? 200 : 66;
		} else {
			delay = isConsoleReady() ? 100 : 33;
		}

		setOutputPinValue(LED_COMMUNICATION_1, 0);
		setOutputPinValue(LED_EXT_1, 1);
//		setOutputPinValue(LED_EXT_2, 1);
//		setOutputPinValue(LED_EXT_3, 1);
		chThdSleepMilliseconds(delay);

		setOutputPinValue(LED_COMMUNICATION_1, 1);
		setOutputPinValue(LED_EXT_1, 0);
//		setOutputPinValue(LED_EXT_2, 0);
//		setOutputPinValue(LED_EXT_3, 0);
		chThdSleepMilliseconds(delay);
	}
}

// todo: fix this, should be a proper declaration in a .h file
int isTriggerDecoderError(void);

static void errBlinkingThread(void *arg) {
	(void) arg;
	chRegSetThreadName("err blinking");
#if EFI_ENGINE_CONTROL
	while (TRUE) {
		int delay = 33;
		if (isTriggerDecoderError() || isIgnitionTimingError())
			setOutputPinValue(LED_WARNING, 1);
		chThdSleepMilliseconds(delay);
		setOutputPinValue(LED_WARNING, 0);
		chThdSleepMilliseconds(delay);
	}
#endif /* EFI_ENGINE_CONTROL */
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
	if (brainPin == GPIO_NONE)
		return GPIO_NULL;
	if (brainPin > GPIO_NONE || brainPin < 0) {
		firmwareError("Invalid brain_pin_e: %d", brainPin);
		return GPIO_NULL;
	}
	return PORTS[brainPin / 16];
}

ioportmask_t getHwPin(brain_pin_e brainPin) {
	if (brainPin == GPIO_NONE)
		return EFI_ERROR_CODE;
	if (brainPin > GPIO_NONE || brainPin < 0) {
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
	outputPinRegister("MalfunctionIndicator", LED_CHECK_ENGINE, getHwPort(boardConfiguration->malfunctionIndicatorPin),
			getHwPin(boardConfiguration->malfunctionIndicatorPin));

// todo: are these needed here? todo: make configurable
//	outputPinRegister("spi CS1", SPI_CS_1, SPI_CS1_PORT, SPI_CS1_PIN);
//	outputPinRegister("spi CS2", SPI_CS_2, SPI_CS2_PORT, SPI_CS2_PIN);
//	outputPinRegister("spi CS3", SPI_CS_3, SPI_CS3_PORT, SPI_CS3_PIN);
//	outputPinRegister("spi CS4", SPI_CS_4, SPI_CS4_PORT, SPI_CS4_PIN);
	outputPinRegister("spi CS5", SPI_CS_SD_MODULE, SPI_SD_MODULE_PORT, SPI_SD_MODULE_PIN);

	// todo: should we move this code closer to the fuel pump logic?
	outputPinRegister("fuel pump relay", FUEL_PUMP_RELAY, getHwPort(boardConfiguration->fuelPumpPin),
			getHwPin(boardConfiguration->fuelPumpPin));

	outputPinRegister("fan relay", FAN_RELAY, getHwPort(boardConfiguration->fanPin),
			getHwPin(boardConfiguration->fanPin));

	outputPinRegister("o2 heater", O2_HEATER, getHwPort(boardConfiguration->o2heaterPin),
			getHwPin(boardConfiguration->o2heaterPin));

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

	chThdCreateStatic(comBlinkingStack, sizeof(comBlinkingStack), NORMALPRIO, (tfunc_t) comBlinkingThread, NULL);
	chThdCreateStatic(errBlinkingStack, sizeof(errBlinkingStack), NORMALPRIO, (tfunc_t) errBlinkingThread, NULL);

	addConsoleActionS("get_pin_value", getPinValue);
}
