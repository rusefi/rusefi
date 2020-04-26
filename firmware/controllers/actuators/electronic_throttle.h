/**
 * @file	electronic_throttle.h
 *
 * @date Dec 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

// https://en.wikipedia.org/wiki/Nyquist%E2%80%93Shannon_sampling_theorem
#define DEFAULT_ETB_LOOP_FREQUENCY 200
#define DEFAULT_ETB_PWM_FREQUENCY 300

#include "engine.h"
#include "closed_loop_controller.h"
#include "expected.h"
#include "periodic_task.h"

class DcMotor;
class Logging;

class IEtbController : public PeriodicTimerController, public ClosedLoopController<percent_t, percent_t> {
public:
	DECLARE_ENGINE_PTR;
	virtual void init(DcMotor *motor, int ownIndex, pid_s *pidParameters, const ValueProvider3D* pedalMap) = 0;
	virtual void reset() = 0;
	virtual void setIdlePosition(percent_t pos) = 0;
};

class EtbController final : public IEtbController {
public:
	void init(DcMotor *motor, int ownIndex, pid_s *pidParameters, const ValueProvider3D* pedalMap) override;
	void setIdlePosition(percent_t pos) override;

	// PeriodicTimerController implementation
	int getPeriodMs() override;
	void PeriodicTask() override;
	void reset() override;

	// Called when the configuration may have changed.  Controller will
	// reset if necessary.
	void onConfigurationChange(pid_s* previousConfiguration);
	
	// Print this throttle's status.
	void showStatus(Logging* logger);

	// Helpers for individual parts of throttle control
	expected<percent_t> observePlant() const override;
	expected<percent_t> getSetpoint() const override;

	expected<percent_t> getOpenLoop(percent_t target) const override;
	expected<percent_t> getClosedLoop(percent_t setpoint, percent_t target) override;
	expected<percent_t> getClosedLoopAutotune(percent_t actualThrottlePosition);

	void setOutput(expected<percent_t> outputValue) override;

	// Used to inspect the internal PID controller's state
	const pid_state_s* getPidState() const { return &m_pid; };

private:
	int m_myIndex = 0;
	DcMotor *m_motor = nullptr;
	Pid m_pid;
	bool m_shouldResetPid = false;

	// Pedal -> target map
	const ValueProvider3D* m_pedalMap = nullptr;

	float m_idlePosition = 0;

	// Autotune helpers
	bool m_lastIsPositive = false;
	efitick_t m_cycleStartTime = 0;
	float m_minCycleTps = 0;
	float m_maxCycleTps = 0;
	float m_a = 0;
	float m_tu = 0;
};

void initElectronicThrottle(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void doInitElectronicThrottle(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void setEtbIdlePosition(percent_t pos DECLARE_ENGINE_PARAMETER_SUFFIX);

void setDefaultEtbBiasCurve(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setDefaultEtbParameters(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setBoschVNH2SP30Curve(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setEtbPFactor(float value);
void setEtbIFactor(float value);
void setEtbDFactor(float value);
void setEtbOffset(int value);
void setThrottleDutyCycle(percent_t level);
void onConfigurationChangeElectronicThrottleCallback(engine_configuration_s *previousConfiguration);
void unregisterEtbPins();
