/*
 * boost_control.h
 *
 *  Created on: 18. aug. 2019
 *      Author: Ola Ruud
 */
#pragma once

#include "periodic_task.h"
#include "closed_loop_controller.h"
#include "efi_pid.h"
#include "boost_control_generated.h"

struct IPwm;

class BoostController : public boost_control_s, public ClosedLoopController<float, percent_t>  {
public:
	void init(IPwm* pmw, const ValueProvider3D* openLoopMap, const ValueProvider3D* closedLoopTargetMap, pid_s* pidParams);
	void update();
	void resetLua();

	// Called when the configuration may have changed.  Controller will
	// reset if necessary.
	void onConfigurationChange(pid_s* previousConfiguration);

	// Helpers for individual parts of boost control
	expected<float> observePlant() const override;
	expected<float> getSetpoint() override;

	expected<percent_t> getOpenLoop(float target) override;
	expected<percent_t> getClosedLoop(float target, float manifoldPressure) override;

	void setOutput(expected<percent_t> outputValue) override;

private:
	percent_t getClosedLoopImpl(float target, float manifoldPressure);

	Pid m_pid;

	const ValueProvider3D* m_openLoopMap = nullptr;
	const ValueProvider3D* m_closedLoopTargetMap = nullptr;
	IPwm* m_pwm = nullptr;
};

void startBoostPin();
void initBoostCtrl();
void setDefaultBoostParameters();
void onConfigurationChangeBoostCallback(engine_configuration_s *previousConfiguration);

void updateBoostControl();
