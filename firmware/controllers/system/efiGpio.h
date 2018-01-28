/**
 * @file	efiGpio.h
 * @brief	EFI-related GPIO code
 *
 *
 * @date Sep 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */
#ifndef EFIGPIO_H_
#define EFIGPIO_H_

#include "io_pins.h"
#include "main.h"

void initPrimaryPins(void);
void initOutputPins(void);

#if EFI_GPIO_HARDWARE || defined(__DOXYGEN__)
void turnAllPinsOff(void);
#else /* EFI_GPIO_HARDWARE */
#define turnAllPinsOff() {}
#endif /* EFI_GPIO_HARDWARE */

/**
 * @brief   Single output pin reference and state
 */
class OutputPin {
public:
	OutputPin();
	/**
	 * initializes pin & registers it in pin repository
	 */
	void initPin(const char *msg, brain_pin_e brainPin, pin_output_mode_e *outputMode);
	/**
	 * same as above, with DEFAULT_OUTPUT mode
	 */
	void initPin(const char *msg, brain_pin_e brainPin);
	/**
	 * dissociates pin from this output and un-registers it in pin repository
	 */
	void unregisterOutput(brain_pin_e oldPin, brain_pin_e newPin);

	bool isInitialized();

	void setValue(int logicValue);
	void toggle();
	bool getLogicValue();


#if EFI_GPIO_HARDWARE || defined(__DOXYGEN__)
	ioportid_t port;
	uint8_t pin;
#endif /* EFI_GPIO_HARDWARE */
	int8_t currentLogicValue;
	/**
	 * we track current pin status so that we do not touch the actual hardware if we want to write new pin bit
	 * which is same as current pin value. This maybe helps in case of status leds, but maybe it's a total over-engineering
	 */
private:
	// todo: inline this method?
	void setDefaultPinState(pin_output_mode_e *defaultState);

	// 4 byte pointer is a bit of a memory waste here
	pin_output_mode_e *modePtr;
};


class NamedOutputPin : public OutputPin {
public:
	NamedOutputPin();
	NamedOutputPin(const char *name);
	void setHigh();
	void setLow();
	/**
	 * @return true if pin was stopped
	 */
	bool stop();
	// todo: char pointer is a bit of a memory waste here, we can reduce RAM usage by software-based getName() method
	const char *name;
};

class InjectorOutputPin : public NamedOutputPin {
public:
	InjectorOutputPin();
	void reset();
	efitimeus_t overlappingScheduleOffTime;
	// todo: implement this via address manipulation to reduce memory usage
	int8_t injectorIndex;
	bool cancelNextTurningInjectorOff;
	int8_t overlappingCounter;
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
	void unregisterPins();
	void startInjectionPins();
	void startIgnitionPins();
	void startAuxValves();
	void stopInjectionPins();
	void stopIgnitionPins();
	OutputPin mainRelay;
	OutputPin fanRelay;
	OutputPin acRelay;
	OutputPin fuelPumpRelay;
	OutputPin o2heater;
	/**
	 * brain board RED LED by default
	 */
	OutputPin errorLedPin;
	OutputPin communicationPin; // blue LED on brain board by default
	OutputPin warningPin; // orange LED on brain board by default
	OutputPin runningPin; // green LED on brain board by default

	OutputPin idleSolenoidPin;
	OutputPin alternatorPin;
	/**
	 * this one is usually on the gauge cluster, not on the ECU
	 */
	OutputPin checkEnginePin;

	NamedOutputPin tachOut;
	NamedOutputPin dizzyOutput;

	OutputPin etbOutput1;
	OutputPin etbOutput2;
	OutputPin fsioOutputs[FSIO_COMMAND_COUNT];
	OutputPin triggerDecoderErrorPin;
	OutputPin hipCs;
	OutputPin sdCsPin;
	OutputPin accelerometerCs;

	InjectorOutputPin injectors[INJECTION_PIN_COUNT];
	IgnitionOutputPin coils[IGNITION_PIN_COUNT];
	NamedOutputPin auxValve[AUX_DIGITAL_VALVE_COUNT];
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

#define getElectricalValue(logicalValue, mode) \
	(logicalValue ? getElectricalValue1(mode) : getElectricalValue0(mode))

#if EFI_GPIO_HARDWARE || defined(__DOXYGEN__)

ioportmask_t getHwPin(const char *msg, brain_pin_e brainPin);
ioportid_t getHwPort(const char *msg, brain_pin_e brainPin);
const char *portname(ioportid_t GPIOx);

#endif /* EFI_GPIO_HARDWARE */

brain_pin_e parseBrainPin(const char *str);
const char *hwPortname(brain_pin_e brainPin);

#endif /* EFIGPIO_H_ */
