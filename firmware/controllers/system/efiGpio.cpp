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
pin_output_mode_e *pinDefaultState[IO_PIN_COUNT];

int getOutputPinValue(io_pin_e pin) {
	return getLogicPinValue(&outputs[pin]);
}

/**
 * @brief Sets the value according to current electrical settings
 */
void setOutputPinValue(io_pin_e pin, int logicValue) {
#if EFI_PROD_CODE
	if (outputs[pin].port == GPIO_NULL)
		return;
	efiAssertVoid(pinDefaultState[pin]!=NULL, "pin mode not initialized");
	pin_output_mode_e mode = *pinDefaultState[pin];
	efiAssert(mode <= OM_OPENDRAIN_INVERTED, "invalid pin_output_mode_e", -1);
#else
	pin_output_mode_e mode = OM_DEFAULT;
#endif
	OutputPin *output = &outputs[pin];
	int eValue = getElectricalValue(logicValue, mode);
	setPinValue(output, eValue, logicValue);
}

bool isPinAssigned(io_pin_e pin) {
#if EFI_PROD_CODE
	return outputs[pin].port != GPIO_NULL;
#else
	return true;
#endif
}

