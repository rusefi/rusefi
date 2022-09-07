/*
 * injection_gpio.cpp
 */

#include "injection_gpio.h"
#include "engine_state.h"
#include "tooth_logger.h"
#include "tunerstudio_outputs.h"

extern bool printFuelDebug;

InjectorOutputPin::InjectorOutputPin() : NamedOutputPin() {
	overlappingCounter = 1; // Force update in reset
	reset();
	injectorIndex = -1;
}

void InjectorOutputPin::open(efitick_t nowNt) {
	// per-output counter for error detection
	overlappingCounter++;
	// global counter for logging
	getEngineState()->fuelInjectionCounter++;

#if FUEL_MATH_EXTREME_LOGGING
	if (printFuelDebug) {
		printf("InjectorOutputPin::open %s %d now=%0.1fms\r\n", name, overlappingCounter, (int)getTimeNowUs() / 1000.0);
	}
#endif /* FUEL_MATH_EXTREME_LOGGING */

	if (overlappingCounter > 1) {
//		/**
//		 * #299
//		 * this is another kind of overlap which happens in case of a small duty cycle after a large duty cycle
//		 */
#if FUEL_MATH_EXTREME_LOGGING
		if (printFuelDebug) {
			printf("overlapping, no need to touch pin %s %d\r\n", name, (int)getTimeNowUs());
		}
#endif /* FUEL_MATH_EXTREME_LOGGING */
	} else {
#if EFI_TOOTH_LOGGER
		LogTriggerInjectorState(nowNt, true);
#endif // EFI_TOOTH_LOGGER
		setHigh();
	}
}

void InjectorOutputPin::close(efitick_t nowNt) {
#if FUEL_MATH_EXTREME_LOGGING
	if (printFuelDebug) {
		printf("InjectorOutputPin::close %s %d %d\r\n", name, overlappingCounter, (int)getTimeNowUs());
	}
#endif /* FUEL_MATH_EXTREME_LOGGING */

	overlappingCounter--;
	if (overlappingCounter > 0) {
#if FUEL_MATH_EXTREME_LOGGING
		if (printFuelDebug) {
			printf("was overlapping, no need to touch pin %s %d\r\n", name, (int)getTimeNowUs());
		}
#endif /* FUEL_MATH_EXTREME_LOGGING */
	} else {
#if EFI_TOOTH_LOGGER
	LogTriggerInjectorState(nowNt, false);
#endif // EFI_TOOTH_LOGGER
		setLow();
	}

	// Don't allow negative overlap count
	if (overlappingCounter < 0) {
		overlappingCounter = 0;
	}
}

