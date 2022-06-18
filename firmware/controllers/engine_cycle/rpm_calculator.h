/**
 * @file    rpm_calculator.h
 * @brief   Shaft position sensor(s) decoder header
 *
 * @date Jan 1, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "globalaccess.h"
#include "scheduler.h"
#include "stored_value_sensor.h"
#include "timer.h"

// we use this value in case of noise on trigger input lines
#define NOISY_RPM -1
#define UNREALISTIC_RPM 30000

typedef enum {
	/**
	 * The engine is not spinning, RPM=0
	 */
	STOPPED,
	/**
	 * The engine is spinning up (reliable RPM is not detected yet).
	 * In this state, rpmValue is >= 0 (can be zero).
	 */
	SPINNING_UP,
	/**
	 * The engine is cranking (0 < RPM < cranking.rpm)
	 */
	CRANKING,
	/**
	 * The engine is running (RPM >= cranking.rpm)
	 */
	RUNNING,
} spinning_state_e;

/**
 * Most consumers should access value via Sensor framework by SensorType::Rpm key
 */
class RpmCalculator : public StoredValueSensor {
public:
	RpmCalculator();

	void onSlowCallback();

	/**
	 * Returns true if the engine is not spinning (RPM==0)
	 */
	bool isStopped() const;
	/**
	 * Returns true if the engine is spinning up
	 */
	bool isSpinningUp() const;
	/**
	 * Returns true if the engine is cranking OR spinning up
	 */
	bool isCranking() const;
	/**
	 * Returns true if the engine is running and not cranking
	 */
	bool isRunning() const;

	bool checkIfSpinning(efitick_t nowNt) const;

	/**
	 * This accessor is used in unit-tests.
	 */
	spinning_state_e getState() const;

	/**
	 * Should be called on every trigger event when the engine is just starting to spin up.
	 */
	void setSpinningUp(efitick_t nowNt );
	/**
	 * Called if the synchronization is lost due to a trigger timeout.
	 */
	void setStopSpinning();

	/**
	 * Just a quick getter for rpmValue
	 * Should be same exact value as Sensor::get(SensorType::Rpm).Value just quicker.
	 * Open question if we have any cases where this opimization is needed.
	 */
	float getCachedRpm() const;
	/**
	 * This method is invoked once per engine cycle right after we calculate new RPM value
	 */
	void onNewEngineCycle();
	uint32_t getRevolutionCounterM(void) const;
	void setRpmValue(float value);
	/**
	 * The same as setRpmValue() but without state change.
	 * We need this to be public because of calling rpmState->assignRpmValue() from rpmShaftPositionCallback()
	 */
	void assignRpmValue(float value);
	uint32_t getRevolutionCounterSinceStart(void) const;
	/**
	 * RPM rate of change between current RPM and RPM measured during previous engine cycle
	 * see also SC_RPM_ACCEL
	 */
	float getRpmAcceleration() const;

	// Get elapsed time since the engine transitioned to the running state.
	float getSecondsSinceEngineStart(efitick_t nowNt) const;

	/**
	 * this is RPM on previous engine cycle.
	 */
	int previousRpmValue = 0;
	/**
	 * This is a performance optimization: let's pre-calculate this each time RPM changes
	 * NaN while engine is not spinning
	 */
	volatile floatus_t oneDegreeUs = NAN;

	Timer lastTdcTimer;

	// RPM rate of change, in RPM per second
	float rpmRate = 0;

protected:
	// Print sensor info - current RPM state
	void showInfo(const char* sensorName) const override;

private:
	/**
	 * At this point this value is same exact value as in private m_value variable
	 * At this point all this is performance optimization?
	 * Open question is when do we need it for performance reasons.
	 */
	 float cachedRpmValue = 0;

	/**
	 * Should be called once we've realized engine is not spinning any more.
	 */
	void setStopped();

	/**
	 * This counter is incremented with each revolution of one of the shafts. Could be
	 * crankshaft could be camshaft.
	 */
	volatile uint32_t revolutionCounterSinceBoot = 0;
	/**
	 * Same as the above, but since the engine started spinning
	 */
	volatile uint32_t revolutionCounterSinceStart = 0;

	spinning_state_e state = STOPPED;

	/**
	 * True if the engine is spinning (regardless of its state), i.e. if shaft position changes.
	 * Needed by spinning-up logic.
	 */
	bool isSpinning = false;

	Timer engineStartTimer;
};

#define isValidRpm(rpm) ((rpm) > 0 && (rpm) < UNREALISTIC_RPM)

void rpmShaftPositionCallback(trigger_event_e ckpSignalType, uint32_t index, efitick_t edgeTimestamp);

void tdcMarkCallback(
		uint32_t index0, efitick_t edgeTimestamp);

/**
 * @brief   Initialize RPM calculator
 */
void initRpmCalculator();

#define getRevolutionCounter() (engine->rpmCalculator.getRevolutionCounterM())

#if EFI_ENGINE_SNIFFER
#define addEngineSnifferEvent(name, msg) { if (engine->isEngineSnifferEnabled) { waveChart.addEvent3((name), (msg)); } }
 #else
#define addEngineSnifferEvent(n, msg) {}
#endif /* EFI_ENGINE_SNIFFER */

efitick_t scheduleByAngle(scheduling_s *timer, efitick_t edgeTimestamp, angle_t angle, action_s action);

