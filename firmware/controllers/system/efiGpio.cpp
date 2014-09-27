/**
 * @file	efiGpio.cpp
 *
 * @date Sep 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "efiGpio.h"
#include "io_pins.h"

// todo: clean this mess, this should become 'static'/private
OutputPin outputs[IO_PIN_COUNT];

int getOutputPinValue(io_pin_e pin) {
	return getLogicPinValue(&outputs[pin]);
}

int getLogicPinValue(OutputPin * outputPin) {
	return outputPin->currentLogicValue;
}

/**
 * @return 0 for OM_DEFAULT and OM_OPENDRAIN
 */

inline static int getElectricalValue0(pin_output_mode_e mode) {
	return mode == OM_INVERTED || mode == OM_OPENDRAIN_INVERTED;
}

/**
 * @return 1 for OM_DEFAULT and OM_OPENDRAIN
 */
inline static int getElectricalValue1(pin_output_mode_e mode) {
	return mode == OM_DEFAULT || mode == OM_OPENDRAIN;
}

// todo: this method is here for unit test visibility. todo: move to a bette place!
int getElectricalValue(int logicalValue, pin_output_mode_e mode) {
	efiAssert(mode <= OM_OPENDRAIN_INVERTED, "invalid pin_output_mode_e", -1);

	return logicalValue ? getElectricalValue1(mode) : getElectricalValue0(mode);
}

