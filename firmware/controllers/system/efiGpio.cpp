/**
 * @file	efiGpio.cpp
 *
 * @date Sep 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "main.h"
#if EFI_GPIO
#include "efiGpio.h"
#include "io_pins.h"

pin_output_mode_e OUTPUT_MODE_DEFAULT = OM_DEFAULT;

// todo: clean this mess, this should become 'static'/private
OutputPin outputs[IO_PIN_COUNT];
engine_pins_s enginePins;

const char *namedPinsArray[NAMED_PIN_COUNT] = { "spa1", "spa2", "spa3", "spa4", "spa5", "spa6", "spa7", "spa8",
		"spa9", "spa10", "spa11", "spa12", "inj1", "inj2", "inj3", "inj4", "inj5", "inj6", "inj7", "inj8", "inj9",
		"inj10", "inj11", "inj12", };

const char *getPinName(io_pin_e io_pin) {
	switch (io_pin) {
	// todo: refactor this hell - introduce arrays & checks?
	case SPARKOUT_1_OUTPUT:
	case SPARKOUT_2_OUTPUT:
	case SPARKOUT_3_OUTPUT:
	case SPARKOUT_4_OUTPUT:
	case SPARKOUT_5_OUTPUT:
	case SPARKOUT_6_OUTPUT:
	case SPARKOUT_7_OUTPUT:
	case SPARKOUT_8_OUTPUT:
	case SPARKOUT_9_OUTPUT:
	case SPARKOUT_10_OUTPUT:
	case SPARKOUT_11_OUTPUT:
	case SPARKOUT_12_OUTPUT:
	case INJECTOR_1_OUTPUT:
	case INJECTOR_2_OUTPUT:
	case INJECTOR_3_OUTPUT:
	case INJECTOR_4_OUTPUT:
	case INJECTOR_5_OUTPUT:
	case INJECTOR_6_OUTPUT:
	case INJECTOR_7_OUTPUT:
	case INJECTOR_8_OUTPUT:
	case INJECTOR_9_OUTPUT:
	case INJECTOR_10_OUTPUT:
	case INJECTOR_11_OUTPUT:
	case INJECTOR_12_OUTPUT:
		return namedPinsArray[io_pin];

	default:
		return "Pin needs name";
	}
}

OutputPin::OutputPin() {
	modePtr = &OUTPUT_MODE_DEFAULT;
}

void OutputPin::setValue(int logicValue) {
	doSetOutputPinValue2(this, logicValue);
}

bool_t OutputPin::getLogicValue() {
	return currentLogicValue;
}

void OutputPin::setDefaultPinState(pin_output_mode_e *outputMode) {
#if EFI_GPIO
	pin_output_mode_e mode = *outputMode;
	assertOMode(mode);
	this->modePtr = outputMode;
#endif
	setValue(false); // initial state
}

/**
 * @brief Sets the value according to current electrical settings
 *
 * This method costs about 85 ticks
 */
void setOutputPinValue(io_pin_e pin, int logicValue) {
	doSetOutputPinValue(pin, logicValue);
}

#endif /* EFI_GPIO */
