/*
 * @file rolling_launch.cpp
 *
 * See rolling_launch.h for the feature overview.
 */

#include "pch.h"
#include "rolling_launch.h"
#include "io_pins.h"

#if EFI_LAUNCH_CONTROL

void RollingLaunchControl::setDefaultConfiguration() {
	engineConfiguration->rollingLaunchEnabled = false;
	engineConfiguration->rollingLaunchActivatePin = Gpio::Unassigned;
	engineConfiguration->rollingLaunchActivatePinMode = PI_DEFAULT;
	engineConfiguration->rollingLaunchRpmWindow = 500;
	engineConfiguration->rollingLaunchMinArmRpm = 2500;
	engineConfiguration->rollingLaunchMaxRpm = 7000;
	engineConfiguration->rollingLaunchMinVss = 10;
	engineConfiguration->rollingLaunchTimingRetard = 10;
	engineConfiguration->rollingLaunchFuelAdderPercent = 5;
	engineConfiguration->rollingLaunchRampOutTime = 1.0f;
}

void RollingLaunchControl::reset() {
	isRollingLaunchArmed = false;
	isRollingLaunchActive = false;
	rollingLaunchSwitchState = false;
	isRollingLaunchRampActive = false;
	rollingLaunchSparkSkipRatio = 0;
	rollingLaunchTimingRetardOut = 0;
	rollingLaunchRampElapsed = 0;
	capturedRpm = 0;
	rollingLaunchTargetRpm = 0;
	m_prevArmed = false;
	m_rampStartRetard = 0;
}

void RollingLaunchControl::onEngineStop() {
	reset();
}

void RollingLaunchControl::onConfigurationChange(engine_configuration_s const* /*previousConfig*/) {
	// A burn may change the captured window or the trigger pin. Re-arm using the new settings.
	reset();
}

void RollingLaunchControl::onSlowCallback() {
	auto cfg = engineConfiguration;

	if (!cfg->rollingLaunchEnabled) {
		reset();
		return;
	}

	const auto rpmSensor = Sensor::get(SensorType::Rpm);
	const auto vssSensor = Sensor::get(SensorType::VehicleSpeed);
	const float rpm = rpmSensor.value_or(0);
	const float vss = vssSensor.value_or(0);

	// Activation is the hardware switch OR the Lua trigger (setRollingLaunchTrigger).
#if EFI_GPIO_HARDWARE
	rollingLaunchSwitchState = isBrainPinValid(cfg->rollingLaunchActivatePin)
		&& efiReadPin(cfg->rollingLaunchActivatePin, cfg->rollingLaunchActivatePinMode);
#else
	rollingLaunchSwitchState = false;
#endif
	isRollingLaunchActive = rollingLaunchSwitchState || luaRollingLaunchState;

	// Arm only while moving (min VSS) and above the minimum engagement RPM. The VSS gate is what
	// makes this "rolling" rather than a standstill 2-step.
	const bool armed = isRollingLaunchActive
		&& rpmSensor.Valid && vssSensor.Valid
		&& std::isfinite(rpm) && std::isfinite(vss)
		&& rpm > 0
		&& rpm >= cfg->rollingLaunchMinArmRpm
		&& vss >= cfg->rollingLaunchMinVss;

	// Rising edge: snapshot the operating point and set the hold target, clamped to the ceiling.
	if (armed && !m_prevArmed) {
		capturedRpm = rpm;
		float target = rpm + cfg->rollingLaunchRpmWindow;
		if (target > cfg->rollingLaunchMaxRpm) {
			target = cfg->rollingLaunchMaxRpm;
		}
		rollingLaunchTargetRpm = target;
	}

	if (armed) {
		isRollingLaunchRampActive = false;
		rollingLaunchRampElapsed = 0;

		// Soft spark cut ramps in across [capturedRpm, targetRpm] so the ceiling is approached,
		// not slammed. A collapsed window cuts at the capped target even if capture was above it.
		if (rollingLaunchTargetRpm > capturedRpm) {
			rollingLaunchSparkSkipRatio = interpolateClamped(capturedRpm, 0.0f, rollingLaunchTargetRpm, 1.0f, rpm);
		} else {
			rollingLaunchSparkSkipRatio = (rpm >= rollingLaunchTargetRpm) ? 1.0f : 0.0f;
		}

		rollingLaunchTimingRetardOut = cfg->rollingLaunchTimingRetard;
		m_rampStartRetard = rollingLaunchTimingRetardOut;
	} else {
		rollingLaunchSparkSkipRatio = 0;

		// Falling edge: start the flat timing ramp-out so the pulled timing returns to zero
		// over rollingLaunchRampOutTime seconds, easing boost back in.
		if (m_prevArmed) {
			m_rampTimer.reset();
			isRollingLaunchRampActive = true;
		}

		if (isRollingLaunchRampActive) {
			const float elapsed = m_rampTimer.getElapsedSeconds();
			rollingLaunchRampElapsed = elapsed;
			if (elapsed >= cfg->rollingLaunchRampOutTime) {
				isRollingLaunchRampActive = false;
				rollingLaunchTimingRetardOut = 0;
			} else {
				rollingLaunchTimingRetardOut = interpolateClamped(0.0f, m_rampStartRetard, cfg->rollingLaunchRampOutTime, 0.0f, elapsed);
			}
		} else {
			rollingLaunchTimingRetardOut = 0;
		}
	}

	isRollingLaunchArmed = armed;
	m_prevArmed = armed;
}

float RollingLaunchControl::getSparkSkipRatio() const {
	return rollingLaunchSparkSkipRatio;
}

float RollingLaunchControl::getFuelCoefficient() const {
	if (!isRollingLaunchArmed) {
		return 1.0f;
	}
	return 1.0f + engineConfiguration->rollingLaunchFuelAdderPercent / 100.0f;
}

float RollingLaunchControl::getTimingRetard() const {
	return rollingLaunchTimingRetardOut;
}

#endif // EFI_LAUNCH_CONTROL
