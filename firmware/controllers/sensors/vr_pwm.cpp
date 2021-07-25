#include "pwm_generator_logic.h"
#include "interpolation.h"
#include "rpm_calculator.h"
#include "engine.h"

#include "vr_pwm.h"

static OutputPin pins[2];
static SimplePwm pwms[2];

static void updateVrPwm(int rpm, size_t index DECLARE_ENGINE_PARAMETER_SUFFIX) {
	auto& cfg = CONFIG(vrThreshold)[index];

	if (cfg.pin == GPIO_UNASSIGNED) {
		return;
	}

	float thresholdVoltage = interpolate2d(rpm * 0.02f, cfg.rpmBins, cfg.values) * 100.0f;

	// 0v   threshold voltage = 3.3v output from mcu
	// 2.5v threshold voltage = 0v   output from mcu
	float mcuVoltage = interpolateClamped(0, 3.3f, 2.5f, 0, thresholdVoltage);
	float duty = mcuVoltage / 3.3f;

	pwms[index].setSimplePwmDutyCycle(duty);
}

void updateVrPwm(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	auto rpm = GET_RPM();

	for (size_t i = 0; i < efi::size(CONFIG(vrThreshold)); i++) {
		updateVrPwm(rpm, i PASS_CONFIG_PARAMETER_SUFFIX);
	}
}

void initVrPwm(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	for (size_t i = 0; i < efi::size(CONFIG(vrThreshold)); i++) {
		auto& cfg = CONFIG(vrThreshold)[i];

		if (cfg.pin == GPIO_UNASSIGNED) {
			continue;
		}

		startSimplePwmHard(&pwms[i], "VR PWM",
			&engine->executor,
			cfg.pin,
			&pins[i],
			10000,	// it's guaranteed to be hardware PWM, the faster the PWM, the less noise makes it through
			0
		);
	}
}
