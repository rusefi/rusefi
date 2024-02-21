/*
 * injection_gpio.cpp
 */

#include "pch.h"

extern bool printFuelDebug;

void startSimultaneousInjection(void*) {
	efitick_t nowNt = getTimeNowNt();
	for (size_t i = 0; i < engineConfiguration->cylindersCount; i++) {
		enginePins.injectors[i].open(nowNt);
	}
}

void endSimultaneousInjectionOnlyTogglePins() {
	efitick_t nowNt = getTimeNowNt();
	for (size_t i = 0; i < engineConfiguration->cylindersCount; i++) {
		enginePins.injectors[i].close(nowNt);
	}
}

InjectorOutputPin::InjectorOutputPin() : NamedOutputPin() {
	m_overlappingCounter = 1; // Force update in reset
	reset();
	injectorIndex = -1;
}

void InjectorOutputPin::open(efitick_t nowNt) {
	// per-output counter for error detection
	m_overlappingCounter++;
	// global counter for logging
	getEngineState()->fuelInjectionCounter++;

#if FUEL_MATH_EXTREME_LOGGING
	if (printFuelDebug) {
		printf("InjectorOutputPin::open %s %d now=%0.1fms\r\n", getName(), m_overlappingCounter, (int)getTimeNowUs() / 1000.0);
	}
#endif /* FUEL_MATH_EXTREME_LOGGING */

	if (m_overlappingCounter > 1) {
//		/**
//		 * #299
//		 * this is another kind of overlap which happens in case of a small duty cycle after a large duty cycle
//		 */
#if FUEL_MATH_EXTREME_LOGGING
		if (printFuelDebug) {
			printf("overlapping, no need to touch pin %s %d\r\n", getName(), (int)getTimeNowUs());
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
		printf("InjectorOutputPin::close %s %d %d\r\n", getName(), m_overlappingCounter, (int)getTimeNowUs());
	}
#endif /* FUEL_MATH_EXTREME_LOGGING */

	m_overlappingCounter--;
	if (m_overlappingCounter > 0) {
#if FUEL_MATH_EXTREME_LOGGING
		if (printFuelDebug) {
			printf("was overlapping, no need to touch pin %s %d\r\n", getName(), (int)getTimeNowUs());
		}
#endif /* FUEL_MATH_EXTREME_LOGGING */
	} else {
#if EFI_TOOTH_LOGGER
	LogTriggerInjectorState(nowNt, false);
#endif // EFI_TOOTH_LOGGER
		setLow();
	}

	// Don't allow negative overlap count
	if (m_overlappingCounter < 0) {
		m_overlappingCounter = 0;
	}
}

void InjectorOutputPin::setHigh() {
	NamedOutputPin::setHigh();
	TunerStudioOutputChannels *state = getTunerStudioOutputChannels();
	// this is NASTY but what's the better option? bytes? At cost of 22 extra bytes in output status packet?
	switch (injectorIndex) {
	case 0:
		state->injectorState1 = true;
		break;
	case 1:
		state->injectorState2 = true;
		break;
	case 2:
		state->injectorState3 = true;
		break;
	case 3:
		state->injectorState4 = true;
		break;
	}
}

void InjectorOutputPin::setLow() {
	NamedOutputPin::setLow();
	TunerStudioOutputChannels *state = getTunerStudioOutputChannels();
	// this is NASTY but what's the better option? bytes? At cost of 22 extra bytes in output status packet?
	switch (injectorIndex) {
	case 0:
		state->injectorState1 = false;
		break;
	case 1:
		state->injectorState2 = false;
		break;
	case 2:
		state->injectorState3 = false;
		break;
	case 3:
		state->injectorState4 = false;
		break;
	}
}

