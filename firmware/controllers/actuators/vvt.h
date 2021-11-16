/*
 * @file vvt.h
 *
 * @date Jun 26, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_ptr.h"
#include "periodic_task.h"
#include "closed_loop_controller.h"
#include "pwm_generator_logic.h"
#include "pid.h"

class ValueProvider3D;

void initAuxPid();
void startVvtControlPins();
void stopVvtControlPins();

class VvtController : public PeriodicTimerController, public ClosedLoopController<angle_t, percent_t> {
public:
	void init(int index, int bankIndex, int camIndex, const ValueProvider3D* targetMap);

	// PeriodicTimerController implementation
	int getPeriodMs() override;
	void PeriodicTask() override;

	// ClosedLoopController implementation
	expected<angle_t> observePlant() const override;

	expected<angle_t> getSetpoint() const override;
	expected<percent_t> getOpenLoop(angle_t target) const override;
	expected<percent_t> getClosedLoop(angle_t setpoint, angle_t observation) override;
	void setOutput(expected<percent_t> outputValue) override;

private:
	Pid m_pid;
	const ValueProvider3D* m_targetMap = nullptr;
	int index = 0;

	// Bank index, 0 or 1
	uint8_t m_bank = 0;

	// Cam index, 0 = intake, 1 = exhaust
	uint8_t m_cam = 0;

public:
	// todo: encapsulate or inject these
	SimplePwm m_pwm;
	OutputPin m_pin;
};
