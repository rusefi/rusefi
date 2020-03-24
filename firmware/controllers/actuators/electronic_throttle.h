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
#include "periodic_task.h"

class DcMotor;
class Logging;

class IEtbController : public PeriodicTimerController {
public:
	DECLARE_ENGINE_PTR;
	virtual void init(DcMotor *motor, int ownIndex, pid_s *pidParameters) = 0;
	virtual void reset() = 0;
};

class EtbController final : public IEtbController {
public:
	void init(DcMotor *motor, int ownIndex, pid_s *pidParameters) override;

	// PeriodicTimerController implementation
	int getPeriodMs() override;
	void PeriodicTask() override;
	void reset() override;

	// Called when the configuration may have changed.  Controller will
	// reset if necessary.
	void onConfigurationChange(pid_s* previousConfiguration);
	
	// Print this throttle's status.
	void showStatus(Logging* logger);

	// Used to inspect the internal PID controller's state
	const pid_state_s* getPidState() const { return &m_pid; };

private:
	int m_myIndex;
	DcMotor *m_motor;
	Pid m_pid;
	bool m_shouldResetPid = false;
};

void initElectronicThrottle(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void doInitElectronicThrottle(DECLARE_ENGINE_PARAMETER_SIGNATURE);

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
