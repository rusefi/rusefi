/**
 * @file malfunction_indicator.cpp
 * @brief We can blink out OBD-II error codes using Malfunction Indicator Light (MIL)
 *
 *
 * @date Dec 20, 2013
 * @author Konstantin Nikonenko
 * @author Andrey Belomutskiy, (c) 2012-2020
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

#include "pch.h"

#if EFI_MALFUNCTION_INDICATOR
#include "malfunction_central.h"
#include "malfunction_indicator.h"
#include "os_access.h"
#include "periodic_thread_controller.h"

#define TEST_MIL_CODE FALSE

#define MFI_LONG_BLINK	1500
#define MFI_SHORT_BLINK	400
#define MFI_BLINK_SEPARATOR 400
#define MFI_CHECKENGINE_LIGHT 10000

static void blink_digits(int digit, int duration) {
	for (int iter = 0; iter < digit; iter++) {
		// todo: why we set LOW and then HIGH? not the other way around?
		enginePins.checkEnginePin.setValue(0);
		chThdSleepMilliseconds(duration);
		enginePins.checkEnginePin.setValue(1);
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
		int ourDigit = (int) efiPow10(iter);		// 10^0 = 1, 10^1 = 10, 10^2=100, 10^3 = 1000, ....
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

class MILController : public PeriodicController<UTILITY_THREAD_STACK_SIZE> {
public:
	MILController()	: PeriodicController("MFIndicator") { }
private:
	void PeriodicTask(efitick_t nowNt) override	{
		UNUSED(nowNt);

		validateStack("MIL", STACK_USAGE_MIL, 128);

		if (nowNt - engine->triggerCentral.triggerState.mostRecentSyncTime < MS2NT(500)) {
			enginePins.checkEnginePin.setValue(1);
			chThdSleepMilliseconds(500);
			enginePins.checkEnginePin.setValue(0);
		}

		static error_codes_set_s localErrorCopy;
		// todo: why do I not see this on a real vehicle? is this whole blinking logic not used?
		getErrorCodes(&localErrorCopy);
		for (int p = 0; p < localErrorCopy.count; p++) {
			// Calculate how many digits in this integer and display error code from start to end
			int code = localErrorCopy.error_codes[p];
			DisplayErrorCode(DigitLength(code), code);
		}
	}
};

static MILController instance;

#if TEST_MIL_CODE
static void testMil(void) {
	addError(OBD_Engine_Coolant_Temperature_Circuit_Malfunction);
	addError(OBD_Intake_Air_Temperature_Circuit_Malfunction);
}
#endif /* TEST_MIL_CODE */

bool isMilEnabled() {
	return isBrainPinValid(CONFIG(malfunctionIndicatorPin));
}

void initMalfunctionIndicator(void) {
	if (!isMilEnabled()) {
		return;
	}
	instance.setPeriod(10 /*ms*/);
	instance.Start();

#if	TEST_MIL_CODE
	addConsoleAction("testmil", testMil);
#endif /* TEST_MIL_CODE */
}

#endif /* EFI_MALFUNCTION_INDICATOR */
