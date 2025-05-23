/*
 * @file vvt.h
 *
 * @date Jun 26, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "closed_loop_controller.h"
#include "pwm_generator_logic.h"
#include "efi_pid.h"
#include "vvt_generated.h"

class ValueProvider3D;

void initVvtActuators();
void startVvtControlPins();
void stopVvtControlPins();
OutputPin* getVvtOutputPin(int index);

#define BANK_BY_INDEX(index) (index / CAMS_PER_BANK)
#define CAM_BY_INDEX(index) (index % CAMS_PER_BANK)
#define INDEX_BY_BANK_CAM(bank, cam) ((bank) * CAMS_PER_BANK + (cam))

class VvtController : public EngineModule, public ClosedLoopController<angle_t, percent_t>, public vvt_s {
public:
	VvtController(int index);

	void init(const ValueProvider3D* targetMap, IPwm* pwm);

	// EngineModule implementation
	void onFastCallback() override;
	void onConfigurationChange(engine_configuration_s const * previousConfig) override;

	// ClosedLoopController implementation
	expected<angle_t> observePlant() override;

	expected<angle_t> getSetpoint() override;
	expected<percent_t> getOpenLoop(angle_t target) override;
	expected<percent_t> getClosedLoop(angle_t setpoint, angle_t observation) override;
	void setOutput(expected<percent_t> outputValue) override;

	uint8_t getCamIndex() {
		return m_cam;
	}

private:
	const int index;
	// Bank index, 0 or 1
	const uint8_t m_bank;
	// Cam index, 0 = intake, 1 = exhaust
	const uint8_t m_cam;

	Pid m_pid;

  // todo: live data?
	bool m_engineRunningLongEnough = false;
	bool m_isRpmHighEnough = false;
	bool m_isCltWarmEnough = false;

	const ValueProvider3D* m_targetMap = nullptr;
	Hysteresis m_targetHysteresis;
	IPwm* m_pwm = nullptr;
};

// Unique types for each VVT so they can be engine modules
struct VvtController1 : public VvtController {
	VvtController1() : VvtController(0) { }
};

struct VvtController2 : public VvtController {
	VvtController2() : VvtController(1) { }
};

struct VvtController3 : public VvtController {
	VvtController3() : VvtController(2) { }
};

struct VvtController4 : public VvtController {
	VvtController4() : VvtController(3) { }
};
