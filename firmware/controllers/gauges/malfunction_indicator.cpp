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
#include "limp_manager.h"

#include "periodic_thread_controller.h"

#define TEST_MIL_CODE TRUE

// Duration of the OEM-style MIL bulb-check pulse at ECU boot (ms).
// Set to 0 to disable the boot pulse entirely.
#define MIL_BOOT_PULSE_MS 3000

// Half-period of the critical-fault attention blink (ms)
#define MFI_CRITICAL_BLINK 250

// Task period while the lamp is solid-on or off - just needs to be responsive
#define MFI_IDLE_PERIOD 100

/**
 * A "critical" fault is one that is genuinely damaging and warrants the driver's
 * immediate attention (blinking lamp) rather than a steady "something logged" lamp.
 * We derive this from the LimpManager: if fuel or spark is currently being cut for
 * one of these reasons, the lamp blinks.
 */
static bool isCriticalClearReason(ClearReason reason) {
	switch (reason) {
		case ClearReason::Fatal:
		case ClearReason::FatalErrorRevLimit:
		case ClearReason::OilPressure:
		case ClearReason::BoostCut:
		case ClearReason::LambdaProtection:
		case ClearReason::InjectorDutyCycle:
		case ClearReason::EtbJammedRevLimit:
		case ClearReason::EtbProblem:
			return true;
		default:
			// Normal operational cuts (rev limiter, ignition off, launch, stall, Lua, etc.)
			// are not "critical" - they should not trigger the attention blink.
			return false;
	}
}

static bool isCriticalFault() {
#if EFI_ENGINE_CONTROL
	LimpManager* limp = getLimpManager();

	LimpState injection = limp->allowInjection();
	if (!injection.value && isCriticalClearReason(injection.reason)) {
		return true;
	}

	LimpState ignition = limp->allowIgnition();
	if (!ignition.value && isCriticalClearReason(ignition.reason)) {
		return true;
	}
#endif // EFI_ENGINE_CONTROL
	return false;
}

class MILController : public PeriodicController<UTILITY_THREAD_STACK_SIZE> {
public:
	MILController()	: PeriodicController("MFIndicator") { }
private:
	void PeriodicTask(efitick_t nowNt) override	{
		UNUSED(nowNt);

		assertStackVoid("MIL", ObdCode::STACK_USAGE_MIL, EXPECTED_REMAINING_STACK);

		if (isCriticalFault()) {
			// Critical fault -> blink for attention (~2 Hz)
			enginePins.checkEnginePin.setValue(1);
			chThdSleepMilliseconds(MFI_CRITICAL_BLINK);
			enginePins.checkEnginePin.setValue(0);
			chThdSleepMilliseconds(MFI_CRITICAL_BLINK);
		} else if (hasErrorCodes()) {
			// Non-critical DTC(s) present -> steady lamp
			enginePins.checkEnginePin.setValue(1);
		} else {
			// All clear -> lamp off
			enginePins.checkEnginePin.setValue(0);
		}
	}
};

static MILController instance;

#if TEST_MIL_CODE
static void testMil() {
	addError(ObdCode::OBD_Clt_High);
	addError(ObdCode::OBD_Iat_High);
}
#endif /* TEST_MIL_CODE */

bool isMilEnabled() {
	return isBrainPinValid(engineConfiguration->malfunctionIndicatorPin);
}

void initMalfunctionIndicator(void) {
	if (!isMilEnabled()) {
		return;
	}

	// OEM-style bulb check: light the MIL for a short configurable pulse at boot
	if (MIL_BOOT_PULSE_MS > 0) {
		enginePins.checkEnginePin.setValue(1);
		chThdSleepMilliseconds(MIL_BOOT_PULSE_MS);
		enginePins.checkEnginePin.setValue(0);
	}

	instance.setPeriod(MFI_IDLE_PERIOD /*ms*/);
	instance.start();

#if	TEST_MIL_CODE
	addConsoleAction("testmil", testMil);
#endif /* TEST_MIL_CODE */
}

#endif /* EFI_MALFUNCTION_INDICATOR */
