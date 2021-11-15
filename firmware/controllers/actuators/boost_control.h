/*
 * boost_control.h
 *
 *  Created on: 18. aug. 2019
 *      Author: Ola Ruud
 */
#pragma once

#include "periodic_task.h"
#include "closed_loop_controller.h"
#include "pid.h"

struct IPwm;

class BoostController : public ClosedLoopController<float, percent_t>, public EnginePtr  {
public:
	void init(IPwm* pmw, const ValueProvider3D* openLoopMap, const ValueProvider3D* closedLoopTargetMap, pid_s* pidParams);
	void update();

	// Called when the configuration may have changed.  Controller will
	// reset if necessary.
	void onConfigurationChange(pid_s* previousConfiguration);

	// Helpers for individual parts of boost control
	expected<float> observePlant() const override;
	expected<float> getSetpoint() const override;

	expected<percent_t> getOpenLoop(float target) const override;
	expected<percent_t> getClosedLoop(float target, float manifoldPressure) override;

	void setOutput(expected<percent_t> outputValue) override;

private:
	percent_t getClosedLoopImpl(float target, float manifoldPressure);

	bool m_shouldResetPid = false;
	Pid m_pid;

	const ValueProvider3D* m_openLoopMap = nullptr;
	const ValueProvider3D* m_closedLoopTargetMap = nullptr;
	IPwm* m_pwm = nullptr;
};

void startBoostPin();
void initBoostCtrl(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void setDefaultBoostParameters(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void onConfigurationChangeBoostCallback(engine_configuration_s *previousConfiguration);

void updateBoostControl();
