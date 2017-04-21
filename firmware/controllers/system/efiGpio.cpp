/**
 * @file	efiGpio.cpp
 *
 * @date Sep 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "main.h"
#if EFI_GPIO || defined(__DOXYGEN__)
#include "efiGpio.h"
#include "io_pins.h"

pin_output_mode_e OUTPUT_MODE_DEFAULT = OM_DEFAULT;

// todo: clean this mess, this should become 'static'/private
EnginePins enginePins;
extern LoggingWithStorage sharedLogger;

NamedOutputPin::NamedOutputPin() : OutputPin() {
	name = NULL;
}

NamedOutputPin::NamedOutputPin(const char *name) : OutputPin() {
	this->name = name;
}

InjectorOutputPin::InjectorOutputPin() : NamedOutputPin() {
	reset();
	injectorIndex = -1;
}

static const char *sparkNames[IGNITION_PIN_COUNT] = { "c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8",
		"c9", "cA", "cB", "cD"};

static const char *injectorNames[INJECTION_PIN_COUNT] = { "i1", "i2", "i3", "i4", "i5", "i6", "i7", "i8",
		"j9", "iA", "iB", "iC"};


EnginePins::EnginePins() {
	dizzyOutput.name = DIZZY_NAME;
	tachOut.name = TACH_NAME;

	for (int i = 0; i < IGNITION_PIN_COUNT;i++) {
		enginePins.coils[i].name = sparkNames[i];
	}
	for (int i = 0; i < INJECTION_PIN_COUNT;i++) {
		enginePins.injectors[i].injectorIndex = i;
		enginePins.injectors[i].name = injectorNames[i];
	}
}

bool EnginePins::stopPins() {
	bool result = false;
	for (int i = 0; i < IGNITION_PIN_COUNT; i++) {
		result |= coils[i].stop();
	}
	for (int i = 0; i < INJECTION_PIN_COUNT; i++) {
		result |= injectors[i].stop();
	}
	return result;
}

void EnginePins::reset() {
	for (int i = 0; i < INJECTION_PIN_COUNT;i++) {
		injectors[i].reset();
	}
	for (int i = 0; i < IGNITION_PIN_COUNT;i++) {
		coils[i].reset();
	}
}

bool NamedOutputPin::stop() {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	if (isInitialized() && getLogicValue()) {
		setValue(false);
		scheduleMsg(&sharedLogger, "turning off %s", name);
		return true;
	}
#endif
	return false;
}

void InjectorOutputPin::reset() {
	overlappingScheduleOffTime = 0;
	cancelNextTurningInjectorOff = false;
	overlappingCounter = 0;
	// todo: this could be refactored by calling some super-reset method
	currentLogicValue = INITIAL_PIN_STATE;
}

IgnitionOutputPin::IgnitionOutputPin() {
	reset();
}

void IgnitionOutputPin::reset() {
	outOfOrder = false;
	signalFallSparkId = 0;
}

OutputPin::OutputPin() {
	modePtr = &OUTPUT_MODE_DEFAULT;
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	port = NULL;
	pin = 0;
#endif
	currentLogicValue = INITIAL_PIN_STATE;
}

bool OutputPin::isInitialized() {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	return port != NULL;
#else
	return false;
#endif
}

void OutputPin::setValue(int logicValue) {
	doSetOutputPinValue2(this, logicValue);
}

bool OutputPin::getLogicValue() {
	return currentLogicValue;
}

void OutputPin::unregister() {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	port = NULL;
#endif
}

void OutputPin::setDefaultPinState(pin_output_mode_e *outputMode) {
	pin_output_mode_e mode = *outputMode;
	assertOMode(mode);
	this->modePtr = outputMode;
	setValue(false); // initial state
}

#endif /* EFI_GPIO */
