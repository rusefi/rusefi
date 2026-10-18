/**
 * @file fuel_pump.h
 */

#pragma once

#include "engine_module.h"
#include "fuel_pump_control_generated.h"
#include "closed_loop_controller.h"
#include "efi_pid.h"
#include "pwm_generator_logic.h"
#include <rusefi/timer.h>
#include <optional>

class FuelPumpController : public EngineModule, public fuel_pump_control_s,
                           public ClosedLoopController<float, percent_t> {
public:
	void setDefaultConfiguration() override;
	void onSlowCallback() override;
	void onFastCallback() override;
	void onIgnitionStateChanged(bool ignitionOn) override;
	void onConfigurationChange(engine_configuration_s const* previous) override;

	void initHardware();
	void stopHardware();
	bool isHardwareConfigurationChanged(engine_configuration_s const* previous) const;

	expected<float> getSetpoint() override;
	expected<float> observePlant() override;
	expected<percent_t> getOpenLoop(float target) override;
	expected<percent_t> getClosedLoop(float target, float pressure) override;
	void setOutput(expected<percent_t> output) override;

private:
	void resetFeedback();
	void updateDualRelay();
	bool hasValidDutyLimits() const;

	Timer m_ignOnTimer;
	Pid m_pid;
	std::optional<SimplePwm> m_pwm;
	fuel_pump_mode_e m_mode = fuel_pump_mode_e::Single;
	bool m_hardwareInitialized = false;
};

void initFuelPump();
void stopFuelPump();
