/*
 * @file aux_pid.h
 *
 * @date Jun 26, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"

void initAuxPid(Logging *sharedLogger);
void startVvtControlPins();
void stopVvtControlPins();

class VvtController : public PeriodicTimerController, public ClosedLoopController<angle_t, percent_t> {
public:
	void init(int index);

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
	ValueProvider3D* m_targetMap = nullptr;

public:
	// todo: encapsulate or these
	SimplePwm m_pwm;
	OutputPin m_pin;
};
