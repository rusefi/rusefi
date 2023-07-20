/*
 * @file vvt.h
 *
 * @date Jun 26, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "periodic_task.h"
#include "closed_loop_controller.h"
#include "pwm_generator_logic.h"
#include "efi_pid.h"
#include "vvt_generated.h"

class ValueProvider3D;

void initVvtActuators();
void startVvtControlPins();
void stopVvtControlPins();

class VvtController : public EngineModule, public ClosedLoopController<angle_t, percent_t>, public vvt_s {
public:
	VvtController(int index, int bankIndex, int camIndex);

	void init(const ValueProvider3D* targetMap, IPwm* pwm);

	// EngineModule implementation
	void onFastCallback() override;

	// ClosedLoopController implementation
	expected<angle_t> observePlant() const override;

	expected<angle_t> getSetpoint() override;
	expected<percent_t> getOpenLoop(angle_t target) override;
	expected<percent_t> getClosedLoop(angle_t setpoint, angle_t observation) override;
	void setOutput(expected<percent_t> outputValue) override;

private:
	const int index = 0;
	// Bank index, 0 or 1
	const uint8_t m_bank = 0;
	// Cam index, 0 = intake, 1 = exhaust
	const uint8_t m_cam = 0;

	Pid m_pid;

	const ValueProvider3D* m_targetMap = nullptr;
	IPwm* m_pwm = nullptr;
};

// Unique types for each VVT so they can be engine modules
struct VvtController1 : public VvtController {
	VvtController1() : VvtController(0, 0, 0) { }
};

struct VvtController2 : public VvtController {
	VvtController2() : VvtController(1, 0, 1) { }
};

struct VvtController3 : public VvtController {
	VvtController3() : VvtController(2, 1, 0) { }
};

struct VvtController4 : public VvtController {
	VvtController4() : VvtController(3, 1, 1) { }
};
