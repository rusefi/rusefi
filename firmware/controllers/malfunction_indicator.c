/**
 * @file malfunction_indicator.c
 * @brief We can blink out OBD-II error codes using Malfunction Indicator Light (MIL)
 *
 *
 * @date Dec 20, 2013
 * @author Konstantin Nikonenko
 * @author Andrey Belomutskiy, (c) 2012-2014
 * we show 4 digit error code - 1,5sec * (4xxx+1) digit + 0,4sec * (x3xxx+1) + ....
 * ATTENTION!!! 0 = 1 blink, 1 = 2 blinks, ...., 9 = 10 blinks
 * sequence is the constant!!!
 *
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
#include "io_pins.h"
#include "malfunction_central.h"
#include "malfunction_indicator.h"

#if EFI_MALFUNCTION_INDICATOR

#define MFI_LONG_BLINK	1500
#define MFI_SHORT_BLINK	400
#define MFI_BLINK_SEPARATOR 400
#define MFI_CHECKENGINE_LIGHT 10000

static THD_WORKING_AREA(mfiThreadStack, UTILITY_THREAD_STACK_SIZE);	// declare thread

static void blink_digits(int digit, int duration) {
	for (int iter = 0; iter < digit; iter++) {
		turnOutputPinOn(LED_CHECK_ENGINE);
		chThdSleepMilliseconds(duration);
		turnOutputPinOff(LED_CHECK_ENGINE);
		chThdSleepMilliseconds(MFI_BLINK_SEPARATOR);
	}
}

// calculate how many digits our code have
static int DigitLength(int digit) {
	int i = 0;
	while (digit > 0) {
		digit = digit / 10;
		++i;
	}
	return i;
}

// display code
static void DisplayErrorCode(int length, int code) {
	// todo: I suggest we use 'itoa' method to simplify this logic
	for (int iter = length - 1; iter >= 0; iter--) {
		int ourDigit = (int)pow(10, iter);		// 10^0 = 1, 10^1 = 10, 10^2=100, 10^3 = 1000, ....
		int digit = 1;						// as we remember "0" we show as one blink
		while (code >= ourDigit) {
			code = code - ourDigit;
			digit++;
		}
		if (iter % 2 == 0)
			blink_digits(digit, MFI_SHORT_BLINK);		// even 2,0 - long blink
		else
			blink_digits(digit, MFI_LONG_BLINK); 		// odd  3,1 - short blink
	}
}

//  our main thread for show check engine error
#if defined __GNUC__
__attribute__((noreturn))   static msg_t mfiThread(void)
#else
static msg_t mfiThread(void)
#endif
{
	chRegSetThreadName("MFIndicator");
	error_codes_set_s localErrorCopy;

	while (TRUE) {
		chThdSleepSeconds(10);

		getErrorCodes(&localErrorCopy);
		for (int p = 0; p < localErrorCopy.count; p++) {
			// Calculate how many digits in this integer and display error code from start to end
			int code = localErrorCopy.error_codes[p];
			DisplayErrorCode(DigitLength(code), code);
		}
	}
}

void initMalfunctionIndicator(void) {
	// create static thread
	chThdCreateStatic(mfiThreadStack, sizeof(mfiThreadStack), LOWPRIO, (tfunc_t) mfiThread, NULL);
	// only for debug
	addError(OBD_Engine_Coolant_Temperature_Circuit_Malfunction);
	addError(OBD_Intake_Air_Temperature_Circuit_Malfunction);
}

#endif /* EFI_MALFUNCTION_INDICATOR */
