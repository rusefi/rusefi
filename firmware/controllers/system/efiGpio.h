/**
 * @file	efiGpio.h
 *
 * @date Sep 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */
#ifndef EFIGPIO_H_
#define EFIGPIO_H_

#include "main.h"
#include "io_pins.h"

/**
 * @brief   Single output pin reference and state
 */
typedef struct {
#if EFI_PROD_CODE
	GPIO_TypeDef *port;
	int pin;
#endif /* EFI_PROD_CODE */
	/**
	 * we track current pin status so that we do not touch the actual hardware if we want to write new pin bit
	 * which is same as current pin value. This maybe helps in case of status leds, but maybe it's a total over-engineering
	 */
	int currentLogicValue;
} OutputPin;

/**
 * it's a macro to be sure that stack is not used
 * @return 0 for OM_DEFAULT and OM_OPENDRAIN
 */

#define getElectricalValue0(mode) ((mode) == OM_INVERTED || (mode) == OM_OPENDRAIN_INVERTED)


/**
 * it's a macro to be sure that stack is not used
 * @return 1 for OM_DEFAULT and OM_OPENDRAIN
 */
#define getElectricalValue1(mode) ((mode) == OM_DEFAULT || (mode) == OM_OPENDRAIN)

#define getLogicPinValue(outputPin) ((outputPin)->currentLogicValue)

/**
 * Sets the value of the pin. On this layer the value is assigned as is, without any conversion.
 */

#if EFI_PROD_CODE                                                                  \

#define setPinValue(outputPin, electricalValue, logicValue)                        \
  {                                                                                \
    if (getLogicPinValue(outputPin) != (logicValue)) {                             \
	  palWritePad((outputPin)->port, (outputPin)->pin, (electricalValue));         \
	  (outputPin)->currentLogicValue = (logicValue);                               \
    }                                                                              \
  }
#else /* EFI_PROD_CODE */
#define setPinValue(outputPin, electricalValue, logicValue)                        \
  {                                                                                \
    if (getLogicPinValue(outputPin) != (logicValue)) {                             \
	  (outputPin)->currentLogicValue = (logicValue);                               \
    }                                                                              \
  }
#endif /* EFI_PROD_CODE */

#define turnOutputPinOn(pin) setOutputPinValue((pin), true)
#define turnOutputPinOff(pin) setOutputPinValue((pin), false)

#define getElectricalValue(logicalValue, mode) \
	(logicalValue ? getElectricalValue1(mode) : getElectricalValue0(mode))

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

int getOutputPinValue(io_pin_e pin);
void setOutputPinValue(io_pin_e pin, int logicValue);
const char *getPinName(io_pin_e io_pin);

#if EFI_PROD_CODE
 #define isPinAssigned(pin) (outputs[(pin)].port != GPIO_NULL)
#else
 #define isPinAssigned(pin) (true)
#endif

#if EFI_PROD_CODE
#define doSetOutputPinValue(pin, logicValue) {                                          \
		if (outputs[(pin)].port != GPIO_NULL) {                                         \
			efiAssertVoid(pinDefaultState[pin]!=NULL, "pin mode not initialized");      \
			pin_output_mode_e mode = *pinDefaultState[pin];                             \
			efiAssertVoid(mode <= OM_OPENDRAIN_INVERTED, "invalid pin_output_mode_e");  \
			OutputPin *output = &outputs[pin];                                          \
			int eValue = getElectricalValue(logicValue, mode);                          \
			setPinValue(output, eValue, logicValue);                                    \
		}                                                                               \
    }
#else
		#define doSetOutputPinValue(pin, logicValue) {                                  \
				pin_output_mode_e mode = OM_DEFAULT;                                    \
				OutputPin *output = &outputs[pin];                                      \
				int eValue = getElectricalValue(logicValue, mode);                      \
				setPinValue(output, eValue, logicValue);                                \
		}
#endif


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EFIGPIO_H_ */
