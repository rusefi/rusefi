/*
 * @file	tunerstudio_configuration.h
 * @brief	Tuner Studio connectivity configuration
 *
 * In this file the configuration of TunerStudio is defined
 *
 * @date Oct 22, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef TUNERSTUDIO_CONFIGURATION_H_
#define TUNERSTUDIO_CONFIGURATION_H_

#include "rusefi_types.h"

#include "tunerstudio_debug_struct.h"

// This class lets us transparently store something at a ratio inside an integer type
// Just use it like a float - you can read and write to it, like this:
// scaled_channel<uint8_t, 10> myVar;
// myVar = 2.4f;	// converts to an int, stores 24
// float x = myVar; // converts back to float, returns 2.4f
template <typename T, int mult = 1>
class scaled_channel {
public:
	scaled_channel() : m_value(static_cast<T>(0)) { }
	scaled_channel(float val)
		: m_value(val * mult)
	{
	}

	// Allow reading back out as a float (note: this may be lossy!)
	operator float() const {
		return m_value / (float)mult;
	}

private:
	T m_value;
};

static_assert(sizeof(scaled_channel<uint8_t>) == 1);
static_assert(sizeof(scaled_channel<uint16_t>) == 2);
static_assert(sizeof(scaled_channel<uint32_t>) == 4);
static_assert(sizeof(scaled_channel<float>) == 4);

using scaled_temperature = scaled_channel<int16_t, 100>;	// +-327 deg C at 0.01 deg resolution
using scaled_ms = scaled_channel<int16_t, 300>;				// +- 100ms at 0.003ms precision
using scaled_percent = scaled_channel<int16_t, 100>;		// +-327% at 0.01% resolution
using scaled_pressure = scaled_channel<uint16_t, 30>;		// 0-2000kPa (~300psi) at 0.03kPa resolution
using scaled_angle = scaled_channel<int16_t, 50>;			// +-655 degrees at 0.02 degree resolution
using scaled_voltage = scaled_channel<uint16_t, 1000>;		// 0-65v at 1mV resolution
using scaled_afr = scaled_channel<uint16_t, 1000>;			// 0-65afr at 0.001 resolution

#define PAGE_COUNT 1

typedef struct {
	uint16_t values[EGT_CHANNEL_COUNT];
} egt_values_s;

/**
 * please be aware that 'float' (F32) type requires TunerStudio version 2.6 and later
 */
typedef struct {
	/* see also [OutputChannels] in rusefi.input */

	/**
	 * Yes, I do not really enjoy packing bits into integers but we simply have too many boolean flags and I cannot
	 * water 4 bytes per traffic - I want gauges to work as fast as possible
	 */
	unsigned int hasSdCard : 1; // bit 0, 72
	unsigned int isIgnitionEnabled : 1; // bit 1
	unsigned int isInjectionEnabled : 1; // bit 2
	unsigned int isCylinderCleanupEnabled : 1; // bit 3
	unsigned int isCylinderCleanupActivated : 1; // bit 4
	unsigned int isFuelPumpOn : 1; // bit 5
	unsigned int isFanOn : 1; // bit 6
	unsigned int isO2HeaterOn : 1; // bit 7
	unsigned int checkEngine : 1; // bit 8
	unsigned int needBurn : 1; // bit 9
	unsigned int secondTriggerChannelEnabled : 1; // bit 10
	unsigned int clutchUpState : 1; // bit 11
	unsigned int clutchDownState : 1; // bit 12
	unsigned int knockEverIndicator : 1; // bit 13
	unsigned int knockNowIndicator : 1; // bit 14
	unsigned int brakePedalState : 1; // bit 15. 0 - not pressed, 1 = pressed
	unsigned int toothLogReady : 1; // bit 16
	unsigned int acSwitchState : 1; // bit 17. 0 - not pressed, 1 = pressed
	unsigned int isTpsError : 1; // bit 18
	unsigned int isCltError : 1; // bit 19
	unsigned int isMapError : 1; // bit 20
	unsigned int isIatError : 1; // bit 21
	unsigned int isAcSwitchEngaged : 1; // bit 22
	unsigned int isTriggerError : 1; // bit 23
	unsigned int hasFatalError : 1; // bit 24
	unsigned int isWarnNow : 1; // bit 25
	unsigned int unused80b8 : 1; // bit 26
	unsigned int isKnockChipOk : 1; // bit 27

	// RPM, vss
	scaled_channel<uint16_t, 4> rpm; // 4
	scaled_percent rpmAcceleration; // 6
	scaled_percent speedToRpmRatio; // 8
	scaled_channel<uint8_t> vehicleSpeedKph; // 10
	
	// temperatures
	scaled_channel<uint8_t> internalMcuTemperature; // offset 11
	scaled_temperature coolantTemperature;   // offset 12
	scaled_temperature intakeAirTemperature; // offset 14
	scaled_temperature auxTemp1;             // offset 16
	scaled_temperature auxTemp2;             // offset 18

	// throttle, pedal
	scaled_percent throttlePosition;    // 20
	scaled_percent pedalPosition;       // 22
	uint16_t tpsADC;                    // 24

	// air flow/mass measurment
	scaled_voltage massAirFlowVoltage; // 26
	scaled_channel<uint16_t, 100> massAirFlow; // 28
	scaled_pressure manifoldAirPressure; // 30
	scaled_pressure baroPressure; // 32

	scaled_afr airFuelRatio; // 34
	scaled_channel<uint16_t, 100> engineLoad; // 36

	scaled_voltage vBatt; // 38
	scaled_pressure oilPressure; // 40
	scaled_angle vvtPosition; // 42

	// Fuel math
	scaled_channel<uint16_t, 1000> chargeAirMass; // 44
	scaled_ms crankingFuelMs; // 46
	scaled_afr currentTargetAfr; // 48
	// This is the raw value we take from the fuel map or base fuel algorithm, before the corrections
	scaled_ms fuelBase; // 50
	// Total fuel with CLT, IAT and TPS acceleration without injector lag corrections per cycle, as pulse per cycle
	scaled_ms fuelRunning; // 52
	// Actual last injection time - including all compensation and injection mode
	scaled_ms actualLastInjection; // 54
	scaled_channel<uint8_t, 2> injectorDutyCycle; // 56
	scaled_channel<uint8_t, 2> veValue; // 57
	scaled_angle injectionOffset; // 58
	scaled_temperature tCharge; // 60

	// Corrections
	scaled_ms injectorLagMs; // 62
	scaled_percent iatCorrection; // 64
	scaled_percent cltCorrection; // 66
	scaled_percent baroCorrection; // 68
	scaled_ms fuelPidCorrection; // 70

	// Wall model AE
	scaled_ms wallFuelAmount; // 72
	scaled_channel<int16_t, 1000> wallFuelCorrection; // 74
	
	// TPS/load AE
	scaled_percent engineLoadDelta; // 76
	scaled_percent deltaTps; // 78
	scaled_ms engineLoadAccelExtra; // 80
	scaled_ms tpsAccelFuel; // 82

	// Ignition
	scaled_angle ignitionAdvance; // 84
	scaled_ms sparkDwell; // 86
	scaled_percent coilDutyCycle; // 88

	// Idle & ETB
	scaled_percent idlePosition; // 90
	scaled_percent etbTarget; // 92
	scaled_percent etb1DutyCycle; // 94
	scaled_percent etb1Error; // 96

	// Fuel system
	scaled_percent fuelTankLevel; // 98
	float fuelConsumptionPerHour; // 100

	// Knock
	uint32_t knockCount; // 104
	float knockLevel; // 108

	// Mode, firmware, protocol, run time
	uint32_t timeSeconds; // 112
	uint32_t engineMode; // 116
	uint32_t firmwareVersion; // 120
	uint32_t tsConfigVersion; // 124

	// EGT
	egt_values_s egtValues; // 128

	int16_t accelerationX; // 144
	int16_t accelerationY; // 148

	// Errors
	int16_t warningCounter; // 150
	int triggerErrorsCounter; // 152
	int totalTriggerErrorCounter; // 156
	int16_t lastErrorCode; // 160
	int16_t recentErrorCodes[8]; // 162

	// Debug
	float debugFloatField1; // 180
	float debugFloatField2;
	float debugFloatField3;
	float debugFloatField4;
	float debugFloatField5;
	float debugFloatField6;
	float debugFloatField7;
	int debugIntField1;
	int debugIntField2;
	int debugIntField3;
	int16_t debugIntField4;
	int16_t debugIntField5;

	// Temporary - will remove soon
	TsDebugChannels* getDebugChannels() {
		return reinterpret_cast<TsDebugChannels*>(&debugFloatField1);
	}

	/* see also [OutputChannels] in rusefi.input */
} TunerStudioOutputChannels;

extern TunerStudioOutputChannels tsOutputChannels;

#endif /* TUNERSTUDIO_CONFIGURATION_H_ */
