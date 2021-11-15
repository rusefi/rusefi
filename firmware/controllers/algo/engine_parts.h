/*
 * @file engine_parts.h
 *
 * @date May 27, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "cyclic_buffer.h"
#include "timer.h"

#define MOCK_ADC_SIZE 26

class MockAdcState {
public:
	MockAdcState();
	bool hasMockAdc[MOCK_ADC_SIZE];
	int fakeAdcValues[MOCK_ADC_SIZE];

	void setMockVoltage(int hwChannel, float voltage DECLARE_ENGINE_PARAMETER_SUFFIX);
	int getMockAdcValue(int hwChannel) const;
};

class Accelerometer {
public:
	float x = 0; // G value
	float y = 0;
	float z = 0;
	float yaw = 0;
	float roll = 0;
};

class SensorsState {
public:
	SensorsState();

	Accelerometer accelerometer;
};

class FuelConsumptionState {
public:
	void consumeFuel(float grams, efitick_t nowNt);

	float getConsumedGrams() const;
	float getConsumptionGramPerSecond() const;

private:
	float m_consumedGrams = 0;
	float m_rate = 0;

	Timer m_timer;
};

class TransmissionState {
public:
	gear_e gearSelectorPosition;
};

typedef cyclic_buffer<int, 8> warningBuffer_t;

class WarningCodeState {
public:
	WarningCodeState();
	void addWarningCode(obd_code_e code);
	bool isWarningNow(efitimesec_t now, bool forIndicator DECLARE_ENGINE_PARAMETER_SUFFIX) const;
	void clear();
	int warningCounter;
	int lastErrorCode;
	efitimesec_t timeOfPreviousWarning;
	// todo: we need a way to post multiple recent warnings into TS
	warningBuffer_t recentWarnings;
};

class FsioState {
public:
	FsioState();

#if EFI_UNIT_TEST
	float mockFan = 0;
	float mockRpm = 0;
	float mockCrankingRpm = 0;
	float mockTimeSinceBoot = 0;
	float mockTimeSinceTrigger = 0;
	int mockAcToggle = 0;
#endif

#if EFI_ENABLE_ENGINE_WARNING
	/**
	 * Shall we purposely miss on some cylinders in order to attract driver's attention to some problem
	 * like getting too hot
	 */
	float isEngineWarning;
#endif /* EFI_ENABLE_ENGINE_WARNING */

#if EFI_ENABLE_CRITICAL_ENGINE_STOP
	/**
	 * Shall we stop engine due to some critical condition in order to save the engine
	 */
	float isCriticalEngineCondition;
#endif /* EFI_ENABLE_CRITICAL_ENGINE_STOP */
};

/**
 * 6 crossing over 50% TPS means pressing and releasing three times
 * TODO: looks like this code is not finished / not used?
 */
#define PUMPS_TO_PRIME 6

class StartupFuelPumping {
public:
	StartupFuelPumping();
	void update(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	bool isTpsAbove50;
	int pumpsCounter;
private:
	void setPumpsCounter(int newValue);
};

struct multispark_state
{
	efitick_t delay = 0;
	efitick_t dwell = 0;
	uint8_t count = 0;
};
