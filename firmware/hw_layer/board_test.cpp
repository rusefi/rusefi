/**
 * @file	board_test.cpp
 * @brief	This is a simple board testing utility
 *
 * By default this is enabled by grounding PB0
 *
 * @date Mar 12, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
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

#include "main.h"
#include "board_test.h"
#include "rusefi_enums.h"
#include "pin_repository.h"
#include "gpio_helper.h"
#include "adc_inputs.h"
#include "AdcConfiguration.h"

static volatile int stepCoutner = 0;
static volatile brain_pin_e currentPin = GPIO_NONE;

extern AdcConfiguration slowAdc;
extern AdcConfiguration fastAdc;

static bool isTimeForNextStep(int copy) {
	return copy != stepCoutner;
}

static void processAdcPin(AdcConfiguration *adc, int index, const char *prefix) {
	adc_channel_e hwIndex = adc->getAdcHardwareIndexByInternalIndex(index);
	GPIO_TypeDef* port = getAdcChannelPort(hwIndex);
	int pin = getAdcChannelPin(hwIndex);

	int copy = stepCoutner;

	int c = 0;

	while (!isTimeForNextStep(copy)) {
		print("%s ch%d hwIndex=%d %s%d\r\n", prefix, index, hwIndex, portname(port), pin);
		int adcValue = adc->getAdcValueByIndex(index);

//		print("ADC%d val= %d%s", hwIndex, value, DELIMETER);
		float volts = adcToVolts(adcValue) * 2;
		print("v=%f  adc=%d  c=%d (hit 'n'<ENTER> for next step\r\n", volts, adcValue, c++);

		chThdSleepMilliseconds(300);

	}
}

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

static brain_pin_e BLINK_PINS[] = { GPIOE_8, // HIGH DRIVER 1
		GPIOE_10, // HIGH DRIVER 2
		GPIOE_12, // HIGH DRIVER 3
		GPIOE_14, // HIGH DRIVER 4
		GPIOC_9, // HIGH DRIVER 5
		GPIOC_7, // HIGH DRIVER 6
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
		GPIOD_7, // OUT3 Frankenso
		GPIOB_7, // OUT12 Frankenso
		};

static THD_WORKING_AREA(btThreadStack, UTILITY_THREAD_STACK_SIZE);

static msg_t ivThread(int param) {
	chRegSetThreadName("board test blinking");

	int value = 0;

	while (TRUE) {
		chThdSleepMilliseconds(250);
		value = 1 - value;
		GPIO_TypeDef *hwPort = getHwPort(currentPin);
		uint32_t hwPin = getHwPin(currentPin);
		palWritePad(hwPort, hwPin, value);
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
	print("ADC count: slow %d/fast %d\r\n", slowAdc.size(), fastAdc.size());

	if (currentPin != GPIO_NONE) {
		print("Blinking %s\r\n", hwPortname(currentPin));
	}
}

void initBoardTest(void) {
	is_board_test_mode = true;
	addConsoleAction("n", nextStep);
	addConsoleActionI("set", setIndex);

	chThdCreateStatic(btThreadStack, sizeof(btThreadStack), NORMALPRIO, (tfunc_t) ivThread, NULL);

	// this code is ugly as hell, I had no time to think. Todo: refactor

	processAdcPin(&fastAdc, 0, "fast");
	while (currentIndex < slowAdc.size()) {
		processAdcPin(&slowAdc, currentIndex, "slow");
		currentIndex++;
	}

	currentIndex = 0;

	int pinsCount = sizeof(BLINK_PINS) / sizeof(brain_pin_e);
	while (currentIndex < pinsCount) {
		currentPin = BLINK_PINS[currentIndex];

		GPIO_TypeDef *hwPort = getHwPort(currentPin);
		uint32_t hwPin = getHwPin(currentPin);

		printBoardTestState();
		mySetPadMode("test", hwPort, hwPin, PAL_STM32_MODE_OUTPUT);

		currentIndex++;
		waitForKey();
	}
}
