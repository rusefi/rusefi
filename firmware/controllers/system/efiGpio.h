/**
 * @file	efiGpio.h
 *
 * @date Sep 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */
#ifndef EFIGPIO_H_
#define EFIGPIO_H_

#include "main.h"
#include "io_pins.h"

#define INITIAL_PIN_STATE -1

// mode >= 0  is always true since that's an unsigned
#define assertOMode(mode) { \
	efiAssertVoid(mode <= OM_OPENDRAIN_INVERTED, "invalid pin_output_mode_e"); \
 }

/**
 * @brief   Single output pin reference and state
 */
class OutputPin {
public:
	OutputPin();
	bool isInitialized();
	void setValue(int logicValue);
	void setDefaultPinState(pin_output_mode_e *defaultState);
	bool getLogicValue();
	void unregister();
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	ioportid_t port;
	int pin;
#endif /* EFI_PROD_CODE */
	pin_output_mode_e *modePtr;
	/**
	 * we track current pin status so that we do not touch the actual hardware if we want to write new pin bit
	 * which is same as current pin value. This maybe helps in case of status leds, but maybe it's a total over-engineering
	 */
	int currentLogicValue;
};

class NamedOutputPin : public OutputPin {
public:
	NamedOutputPin();
	NamedOutputPin(const char *name);
	/**
	 * @return true if pin was stopped
	 */
	bool stop();
	const char *name;
};

class InjectorOutputPin : public NamedOutputPin {
public:
	InjectorOutputPin();
	void reset();
	efitimeus_t overlappingScheduleOffTime;
	int injectorIndex;
	bool cancelNextTurningInjectorOff;
	int overlappingCounter;
};

class IgnitionOutputPin : public NamedOutputPin {
public:
	IgnitionOutputPin();
	void reset();
	int signalFallSparkId;
	bool outOfOrder; // https://sourceforge.net/p/rusefi/tickets/319/
};

class EnginePins {
public:
	EnginePins();
	void reset();
	bool stopPins();
	OutputPin mainRelay;
	OutputPin fanRelay;
	OutputPin acRelay;
	OutputPin fuelPumpRelay;
	OutputPin o2heater;
//	OutputPin alternatorField;
	OutputPin errorLedPin;
	OutputPin idleSolenoidPin;
	OutputPin alternatorPin;
	OutputPin checkEnginePin;
	NamedOutputPin tachOut;
	OutputPin etbOutput1;
	OutputPin etbOutput2;
	OutputPin fsioOutputs[LE_COMMAND_COUNT];
	OutputPin triggerDecoderErrorPin;
	OutputPin hipCs;
	OutputPin sdCsPin;

	InjectorOutputPin injectors[INJECTION_PIN_COUNT];
	IgnitionOutputPin coils[IGNITION_PIN_COUNT];
	NamedOutputPin dizzyOutput;
};

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

/**
 * Sets the value of the pin. On this layer the value is assigned as is, without any conversion.
 */

#if EFI_PROD_CODE                                                                  \

#define setPinValue(outputPin, electricalValue, logicValue)                        \
  {                                                                                \
    if ((outputPin)->currentLogicValue != (logicValue)) {                          \
	  palWritePad((outputPin)->port, (outputPin)->pin, (electricalValue));         \
	  (outputPin)->currentLogicValue = (logicValue);                               \
    }                                                                              \
  }
#else /* EFI_PROD_CODE */
#define setPinValue(outputPin, electricalValue, logicValue)                        \
  {                                                                                \
    if ((outputPin)->currentLogicValue != (logicValue)) {                          \
	  (outputPin)->currentLogicValue = (logicValue);                               \
    }                                                                              \
  }
#endif /* EFI_PROD_CODE */

#define getElectricalValue(logicalValue, mode) \
	(logicalValue ? getElectricalValue1(mode) : getElectricalValue0(mode))

#if EFI_PROD_CODE
 #define isPinAssigned(output) ((output)->port != GPIO_NULL)
#else
 #define isPinAssigned(output) (true)
#endif

#define doSetOutputPinValue(pin, logicValue) doSetOutputPinValue2((&outputs[pin]), logicValue)


#if EFI_PROD_CODE
#define doSetOutputPinValue2(output, logicValue) {                                      \
		if ((output)->port != GPIO_NULL) {                                                \
			efiAssertVoid((output)->modePtr!=NULL, "pin mode not initialized");           \
			pin_output_mode_e mode = *(output)->modePtr;                                  \
			efiAssertVoid(mode <= OM_OPENDRAIN_INVERTED, "invalid pin_output_mode_e");  \
			int eValue = getElectricalValue(logicValue, mode);                          \
			setPinValue(output, eValue, logicValue);                                    \
		}                                                                               \
    }
#else
		#define doSetOutputPinValue2(output, logicValue) {                              \
				pin_output_mode_e mode = OM_DEFAULT;                                    \
				int eValue = getElectricalValue(logicValue, mode);                      \
				setPinValue(output, eValue, logicValue);                                \
		}
#endif

void outputPinRegisterExt2(const char *msg, OutputPin *output, brain_pin_e brainPin, pin_output_mode_e *outputMode);

void turnPinHigh(NamedOutputPin *output);
void turnPinLow(NamedOutputPin *output);

#endif /* EFIGPIO_H_ */
