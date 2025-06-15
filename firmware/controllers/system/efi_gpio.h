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
void initMiscOutputPins();

#if EFI_GPIO_HARDWARE
void turnAllPinsOff();
#else /* EFI_GPIO_HARDWARE */
#define turnAllPinsOff() {}
#endif /* EFI_GPIO_HARDWARE */

class IgnitionOutputPin : public NamedOutputPin {
public:
	IgnitionOutputPin();
	void setHigh() override;
	void setLow() override;
	void reset();
	// 6000 RPM is 100Hz we can fit a few years worth of sparks into 32 bits, right?
	// 2_000_000_000 / 100 = 20_000_000 seconds = 231 days?
	// [tag:duration_limit]
	int signalFallSparkId;
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
	const char* getRegistrationName() const {
		return registrationName;
	}
private:
	const char* const registrationName;
	const uint16_t m_pinOffset;
	const bool m_hasPinMode;
	const uint16_t m_pinModeOffset;
	bool isPinConfigurationChanged();
};

class RegisteredNamedOutputPin : public RegisteredOutputPin, public NamedOutputPin {
public:
	RegisteredNamedOutputPin(const char* name, size_t pinOffset, size_t pinModeOffset);
	RegisteredNamedOutputPin(const char* name, size_t pinOffset);
};

class EnginePins {
public:
	EnginePins();
	void startPins();
	void reset();
	static void debug();
	bool stopPins();
	void unregisterPins();
	OutputPin *getOutputPinForBenchMode(bench_mode_e idx);

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
	RegisteredOutputPin nitrousRelay;
#if EFI_HD_ACR
	RegisteredNamedOutputPin harleyAcr;
	RegisteredOutputPin harleyAcr2;
#endif // EFI_HD_ACR
	// todo: shall we rename this to narrowBandO2heater or even remove the whole questionable feature?
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

	RegisteredOutputPin tachOut;
	RegisteredOutputPin triggerDecoderErrorPin;
	RegisteredOutputPin speedoOut;

	OutputPin sdCsPin;
	OutputPin accelerometerCs;

	InjectorOutputPin injectors[MAX_CYLINDER_COUNT];
	InjectorOutputPin injectorsStage2[MAX_CYLINDER_COUNT];
	IgnitionOutputPin coils[MAX_CYLINDER_COUNT];
	IgnitionOutputPin trailingCoils[MAX_CYLINDER_COUNT];
	NamedOutputPin auxValve[AUX_DIGITAL_VALVE_COUNT];
	OutputPin tcuSolenoids[TCU_SOLENOID_COUNT];
	OutputPin tcuTccOnoffSolenoid;
	OutputPin tcuTccPwmSolenoid;
	OutputPin tcuPcSolenoid;
	OutputPin tcu32Solenoid;

#ifdef EFI_UNIT_TEST
	void resetForUnitTest();
#endif

private:
	void startInjectionPins();
	void startIgnitionPins();
	void startAuxValves();

	void stopInjectionPins();
	void stopIgnitionPins();
	void stopAuxValves();
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

#if EFI_GPIO_HARDWARE

ioportmask_t getHwPin(const char *msg, brain_pin_e brainPin);
ioportid_t getHwPort(const char *msg, brain_pin_e brainPin);
/* Should return valid pointer in any case, not null, return "unknown" if argument is invalid */
const char *portname(ioportid_t GPIOx);

#endif /* EFI_GPIO_HARDWARE */

brain_pin_e parseBrainPin(const char *str);

extern EnginePins enginePins;

#ifndef LED_PIN_MODE
#define LED_PIN_MODE OM_DEFAULT
#endif /* LED_PIN_MODE */
