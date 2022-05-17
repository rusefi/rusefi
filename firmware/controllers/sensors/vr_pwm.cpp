#include "pch.h"

#include "vr_pwm.h"

static OutputPin pins[VR_THRESHOLD_COUNT];
static SimplePwm pwms[VR_THRESHOLD_COUNT];

static void updateVrPwm(int rpm, size_t index) {
	auto& cfg = engineConfiguration->vrThreshold[index];

	if (!isBrainPinValid(cfg.pin)) {
		return;
	}

	float thresholdVoltage = interpolate2d(rpm, cfg.rpmBins, cfg.values);

	// 0v   threshold voltage = 3.3v output from mcu = 100% duty
	// 2.5v threshold voltage = 0v   output from mcu = 0% duty
	float duty = interpolateClamped(0, 1, 2.5f, 0, thresholdVoltage);

	pwms[index].setSimplePwmDutyCycle(duty);
}

void updateVrPwm() {
	auto rpm = Sensor::getOrZero(SensorType::Rpm);

	for (size_t i = 0; i < efi::size(engineConfiguration->vrThreshold); i++) {
		updateVrPwm(rpm, i);
	}
}

void initVrPwm() {
	for (size_t i = 0; i < efi::size(engineConfiguration->vrThreshold); i++) {
		auto& cfg = engineConfiguration->vrThreshold[i];

		if (!isBrainPinValid(cfg.pin)) {
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

void setDefaultVrThresholds() {
	for (int i = 0;i<VR_THRESHOLD_COUNT;i++) {
		setLinearCurve(engineConfiguration->vrThreshold[i].rpmBins, 600, 7000, 100);
		setLinearCurve(engineConfiguration->vrThreshold[i].values, 0.6, 1.2, 0.1);
	}
}
