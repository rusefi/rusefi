/**
 * @file	electronic_throttle_impl.h
 *
 * @date Dec 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

// include the "public" ETB interface
#include "electronic_throttle.h"

#include "sensor.h"
#include "pid.h"
#include "error_accumulator.h"

/**
 * Hard code ETB update speed.
 * Since this is a safety critical system with no real reason for a user to ever need to change the update rate,
 * it's locked to 500hz, along with the ADC.
 * https://en.wikipedia.org/wiki/Nyquist%E2%80%93Shannon_sampling_theorem
 */
#define ETB_LOOP_FREQUENCY 500
#define DEFAULT_ETB_PWM_FREQUENCY 800

class EtbController : public IEtbController {
public:
	bool init(etb_function_e function, DcMotor *motor, pid_s *pidParameters, const ValueProvider3D* pedalMap, bool initializeThrottles) override;
	void setIdlePosition(percent_t pos) override;
	void setWastegatePosition(percent_t pos) override;
	void reset() override;

	// Update the controller's state: read sensors, send output, etc
	void update() override;

	// Called when the configuration may have changed.  Controller will
	// reset if necessary.
	void onConfigurationChange(pid_s* previousConfiguration);
	
	// Print this throttle's status.
	void showStatus();

	// Helpers for individual parts of throttle control
	expected<percent_t> observePlant() const override;

	expected<percent_t> getSetpoint() const override;
	expected<percent_t> getSetpointEtb() const;
	expected<percent_t> getSetpointWastegate() const;
	expected<percent_t> getSetpointIdleValve() const;

	expected<percent_t> getOpenLoop(percent_t target) const override;
	expected<percent_t> getClosedLoop(percent_t setpoint, percent_t observation) override;
	expected<percent_t> getClosedLoopAutotune(percent_t setpoint, percent_t actualThrottlePosition);

	void setOutput(expected<percent_t> outputValue) override;

	// Used to inspect the internal PID controller's state
	const pid_state_s* getPidState() const override { return &m_pid; };

	// Use the throttle to automatically calibrate the relevant throttle position sensor(s).
	void autoCalibrateTps() override;

protected:
	// This is set if an automatic TPS calibration should be run
	bool m_isAutocal = false;

	etb_function_e getFunction() const { return m_function; }
	DcMotor* getMotor() { return m_motor; }

private:
	etb_function_e m_function = ETB_None;
	SensorType m_positionSensor = SensorType::Invalid;
	DcMotor *m_motor = nullptr;
	Pid m_pid;
	bool m_shouldResetPid = false;
	ErrorAccumulator m_errorAccumulator;

	// Pedal -> target map
	const ValueProvider3D* m_pedalMap = nullptr;

	float m_idlePosition = 0;
	float m_wastegatePosition = 0;

	// This is set if automatic PID cal shoudl be run
	bool m_isAutotune = false;

	// Autotune helpers
	bool m_lastIsPositive = false;
	efitick_t m_cycleStartTime = 0;
	float m_minCycleTps = 0;
	float m_maxCycleTps = 0;
	// Autotune measured parameters: gain and ultimate period
	// These are set to correct order of magnitude starting points
	// so we converge more quickly on the correct values
	float m_a = 8;
	float m_tu = 0.1f; 

	uint8_t m_autotuneCounter = 0;
	uint8_t m_autotuneCurrentParam = 0;
};
