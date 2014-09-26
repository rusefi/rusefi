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


int getLogicPinValue(OutputPin * outputPin) {
	return outputPin->currentLogicValue;
}
