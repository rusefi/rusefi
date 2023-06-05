/**
 * @file	efi_gpio.h
 * @brief	EFI-related GPIO code
 *
 *
 * @date Sep 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"
#include "io_pins.h"
#include "engine_configuration.h"
#include "injection_gpio.h"

void initPrimaryPins();
void initOutputPins();

#if EFI_GPIO_HARDWARE
void turnAllPinsOff(void);
#else /* EFI_GPIO_HARDWARE */
#define turnAllPinsOff() {}
#endif /* EFI_GPIO_HARDWARE */


#ifdef __cplusplus


class IgnitionOutputPin : public NamedOutputPin {
public:
	IgnitionOutputPin();
	void setHigh() override;
	void setLow() override;
	void reset();
	int signalFallSparkId;
	bool outOfOrder; // https://sourceforge.net/p/rusefi/tickets/319/
	int8_t coilIndex;
};

/**
 * OutputPin with semi-automated init/deinit on configuration change
 */
class RegisteredOutputPin : public virtual OutputPin {
public:
	RegisteredOutputPin(const char *registrationName, size_t pinOffset, size_t pinModeOffset);
	RegisteredOutputPin(const char *registrationName, size_t pinOffset);
	void init();
	void unregister();
	RegisteredOutputPin* const next;
	const char *registrationName;
private:
	const uint16_t m_pinOffset;
	const bool m_hasPinMode;
	const uint16_t m_pinModeOffset;
	bool isPinConfigurationChanged();
};

class RegisteredNamedOutputPin : public RegisteredOutputPin, public NamedOutputPin {
public:
		RegisteredNamedOutputPin(const char *name, size_t pinOffset, size_t pinModeOffset);
};

class EnginePins {
public:
	EnginePins();
	void startPins();
	void reset();
	static void debug();
	bool stopPins();
	void unregisterPins();
	RegisteredOutputPin mainRelay;
	/**
	 * High Pressure Fuel Pump valve control
	 */
	RegisteredNamedOutputPin hpfpValve;
	// this one cranks engine
	RegisteredOutputPin starterControl;
	// this one prevents driver from cranking engine
	RegisteredOutputPin starterRelayDisable;

	RegisteredOutputPin fanRelay;
	RegisteredOutputPin fanRelay2;

	// see acRelayPin
	RegisteredOutputPin acRelay;
	RegisteredOutputPin fuelPumpRelay;
	RegisteredOutputPin harleyAcr;
	RegisteredOutputPin harleyAcr2;
	OutputPin o2heater;
	OutputPin luaOutputPins[LUA_PWM_COUNT];

	/**
	 * brain board RED LED by default
	 */
	OutputPin errorLedPin;
	OutputPin communicationLedPin; // blue LED on brain board by default
	OutputPin warningLedPin; // orange LED on brain board by default
	OutputPin runningLedPin; // green LED on brain board by default

	OutputPin debugTriggerSync;
	RegisteredOutputPin boostPin;
	RegisteredOutputPin idleSolenoidPin;
	RegisteredOutputPin secondIdleSolenoidPin;
	RegisteredOutputPin alternatorPin;
	/**
	 * this one is usually on the gauge cluster, not on the ECU
	 */
	RegisteredOutputPin checkEnginePin;

	RegisteredNamedOutputPin tachOut;

	OutputPin sdCsPin;
	OutputPin accelerometerCs;

	InjectorOutputPin injectors[MAX_CYLINDER_COUNT];
	IgnitionOutputPin coils[MAX_CYLINDER_COUNT];
	IgnitionOutputPin trailingCoils[MAX_CYLINDER_COUNT];
	NamedOutputPin auxValve[AUX_DIGITAL_VALVE_COUNT];
	OutputPin tcuSolenoids[TCU_SOLENOID_COUNT];
	OutputPin tcuTccOnoffSolenoid;
	OutputPin tcuTccPwmSolenoid;
	OutputPin tcuPcSolenoid;
	OutputPin tcu32Solenoid;

private:
	void startInjectionPins();
	void startIgnitionPins();
	void startAuxValves();

	void stopInjectionPins();
	void stopIgnitionPins();
	void stopAuxValves();
};

#endif /* __cplusplus */

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

#if EFI_GPIO_HARDWARE

EXTERNC ioportmask_t getHwPin(const char *msg, brain_pin_e brainPin);
EXTERNC ioportid_t getHwPort(const char *msg, brain_pin_e brainPin);
const char *portname(ioportid_t GPIOx);

#endif /* EFI_GPIO_HARDWARE */

void printSpiConfig(const char *msg, spi_device_e device);
brain_pin_e parseBrainPin(const char *str);

extern EnginePins enginePins;

#ifndef LED_PIN_MODE
#define LED_PIN_MODE OM_DEFAULT
#endif /* LED_PIN_MODE */
