/**
* @file    alternator_controller.h
 * @brief   alternator controller
 *
 * @date Apr 6, 2014
 * @author Dmitry Sidin
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 */

#pragma once

#include "engine_module.h"
#include "closed_loop_controller.h"
#include "engine_configuration.h"

void initAlternatorCtrl();

class AlternatorController : public EngineModule, public ClosedLoopController<float, percent_t> {
public:
	AlternatorController();

	// EngineModule implementation
	void onFastCallback() override;
	void onConfigurationChange(engine_configuration_s const* previousConfiguration) override;
	// public only for tests on test_alternator_voltage_target_set_point.cpp
	expected<float> getSetpoint() override;
	expected<float> observePlant() override;
	expected<percent_t> getOpenLoop(float target) override;
	expected<percent_t> getClosedLoop(float setpoint, float observation) override;
	void setOutput(expected<percent_t> outputValue) override;
private:
	Pid alternatorPid;
};