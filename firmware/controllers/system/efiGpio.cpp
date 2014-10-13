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

int getElectricalValue(int logicalValue, pin_output_mode_e mode) {
	efiAssert(mode <= OM_OPENDRAIN_INVERTED, "invalid pin_output_mode_e", -1);

	return logicalValue ? getElectricalValue1(mode) : getElectricalValue0(mode);
}

/**
 * Set's the value of the pin. On this layer the value is assigned as is, without any conversion.
 */
void setPinValue(OutputPin * outputPin, int electricalValue, int logicValue) {
	if (getLogicPinValue(outputPin) == logicValue)
		return;

#if EFI_PROD_CODE
	palWritePad(outputPin->port, outputPin->pin, electricalValue);
#endif
	outputPin->currentLogicValue = logicValue;
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
#else
	pin_output_mode_e mode = OM_DEFAULT;
#endif
	setPinValue(&outputs[pin], getElectricalValue(logicValue, mode), logicValue);
}

bool isPinAssigned(io_pin_e pin) {
#if EFI_PROD_CODE
	return outputs[pin].port != GPIO_NULL;
#else
	return true;
#endif
}

