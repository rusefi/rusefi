/**
 * @file	board_test.cpp
 * @brief	This is a simple board testing utility
 *
 * By default this is enabled by grounding PB0
 *
 * @date Mar 12, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#define ADC_VCC 3.3f

#include "main.h"
#include "board_test.h"
#include "rusefi_enums.h"
#include "pin_repository.h"
#include "efiGpio.h"
#include "adc_inputs.h"
#include "AdcConfiguration.h"
#include "engine.h"

static volatile int stepCoutner = 0;
static volatile brain_pin_e currentPin = GPIO_UNASSIGNED;

static bool isTimeForNextStep(int copy) {
	return copy != stepCoutner;
}

#if HAL_USE_ADC || defined(__DOXYGEN__)
extern AdcDevice slowAdc;
extern AdcDevice fastAdc;

EXTERN_ENGINE;

static void processAdcPin(AdcDevice *adc, int index) {
	adc_channel_e hwIndex = adc->getAdcHardwareIndexByInternalIndex(index);
	ioportid_t port = getAdcChannelPort("test", hwIndex);
	int pin = getAdcChannelPin(hwIndex);

	int copy = stepCoutner;

	int c = 0;

	while (!isTimeForNextStep(copy)) {
		print("ch%d hwIndex=%d %s%d\r\n", index, hwIndex, portname(port), pin);
		int adcValue = adc->getAdcValueByIndex(index);

//		print("ADC%d val= %d%s", hwIndex, value, DELIMETER);
		float volts = adcToVolts(adcValue) * 2;
		print("v=%f  adc=%d  c=%d (hit 'n'<ENTER> for next step\r\n", volts, adcValue, c++);

		chThdSleepMilliseconds(300);

	}
}
#endif
static volatile int currentIndex = 0;

static void waitForKey(void) {
	print("Please hit N<ENTER> to continue\r\n");
	int copy = stepCoutner;
	while (!isTimeForNextStep(copy)) {
		chThdSleepMilliseconds(200);
	}
}

static void nextStep(void) {
	stepCoutner++;
}

static void setIndex(int index) {
	currentIndex = index;
	nextStep();
}

static brain_pin_e BLINK_PINS[] = {
		GPIOE_8, // HIGH DRIVER 1
		GPIOE_10, // HIGH DRIVER 2
		GPIOE_12, // HIGH DRIVER 3
		GPIOE_14, // HIGH DRIVER 4
		GPIOC_9, // HIGH DRIVER 5
		GPIOC_7, // HIGH DRIVER 6
		GPIOD_8, // HIGH DRIVER 7
		GPIOD_9, // HIGH DRIVER 8
		// index = 6
		GPIOC_14, // OUT 1
		GPIOC_15, // OUT2
		GPIOE_6,  // OUT3
		GPIOC_13, // OUT4
		GPIOE_4, // OUT5
		GPIOE_5, // OUT6
		GPIOE_2, // OUT7
		GPIOE_3, // OUT8
		GPIOE_0, // OUT9
		GPIOE_1, // OUT10
		GPIOB_8, // OUT11
		GPIOB_9, // OUT12
		GPIOD_3, // Frankenso PE1 reroute
		GPIOD_5, // Frankenso PE0 reroute
		GPIOD_7, // OUT3 Frankenso
		GPIOB_7, // OUT12 Frankenso
		};

int pinsCount = sizeof(BLINK_PINS) / sizeof(brain_pin_e);

static THD_WORKING_AREA(btThreadStack, UTILITY_THREAD_STACK_SIZE);

static void setCurrentPinValue(bool value) {
	ioportid_t hwPort = getHwPort("test", currentPin);
	uint32_t hwPin = getHwPin("test", currentPin);
	palWritePad(hwPort, hwPin, value);
}

static msg_t ivThread(int param) {
	(void) param;
	chRegSetThreadName("board test blinking");

	int value = 0;

	while (TRUE) {
		chThdSleepMilliseconds(250);
		value = 1 - value;
		setCurrentPinValue(value);
	}
#if defined __GNUC__
	return 0;
#endif        
}

static bool is_board_test_mode = false;

bool isBoardTestMode(void) {
	return is_board_test_mode;
}

void printBoardTestState(void) {
	print("Current index=%d\r\n", currentIndex);
	print("'n' for next step and 'set X' to return to step X\r\n");
#if HAL_USE_ADC || defined(__DOXYGEN__)
	print("ADC count: slow %d/fast %d\r\n", slowAdc.size(), fastAdc.size());
#endif

	if (currentPin != GPIO_UNASSIGNED) {
		print("Blinking %s\r\n", hwPortname(currentPin));
	}
}

static void btInitOutputPins() {
	for (int i = 0; i < pinsCount; i++) {
		currentPin = BLINK_PINS[i];
		efiSetPadMode("test", currentPin, PAL_STM32_MODE_OUTPUT);
	}
}

static void blinkAllOutputPins() {
	int msgCounter = 0;
	for (int k = 0; k < 6; k++) {
		for (int i = 0; i < pinsCount; i++) {
			currentPin = BLINK_PINS[i];
			setCurrentPinValue(k % 2);
		}
		print("blinking %d\r\n", msgCounter++);
		chThdSleepMilliseconds(250);
	}
	currentPin = GPIO_UNASSIGNED;
	/**
	 * Now let's blink all pins one by one
	 */
	for (int k = 0; k < 2; k++) {
		for (int i = 0; i < pinsCount; i++) {
			if (currentPin != GPIO_UNASSIGNED)
				setCurrentPinValue(false); // turn off previous pin

			currentPin = BLINK_PINS[i];
			setCurrentPinValue(true);
			print("blinking %d!\r\n", msgCounter++);
			chThdSleepMilliseconds(250);
		}
	}
	setCurrentPinValue(false);
	currentPin = GPIO_UNASSIGNED;
}

void initBoardTest(void) {
	is_board_test_mode = true;
	addConsoleAction("n", nextStep);
	addConsoleActionI("set", setIndex);

	btInitOutputPins();
	blinkAllOutputPins();

	chThdCreateStatic(btThreadStack, sizeof(btThreadStack), NORMALPRIO, (tfunc_t) ivThread, NULL);
	// this code is ugly as hell, I had no time to think. Todo: refactor

#if HAL_USE_ADC || defined(__DOXYGEN__)
	/**
	 * in board test mode all currently enabled ADC channels are running in slow mode
	 */
	while (currentIndex < slowAdc.size()) {
		processAdcPin(&slowAdc, currentIndex);
		currentIndex++;
	}
#endif

	currentIndex = 0;

	while (currentIndex < pinsCount) {
		currentPin = BLINK_PINS[currentIndex];

		printBoardTestState();

		currentIndex++;
		waitForKey();
	}
	// no buffered logger still, just plain old stdout
	while (1) {
		print("Board test done, thank you! Time to remove that jumper and reboot\r\n");
		print("Bye!\r\n");
		chThdSleepSeconds(1);
	}
}
