#include "pch.h"

#include "fan_control.h"

#include "bench_test.h"
#include "table_helper.h"
#include "rusefi/interpolation.h"

PUBLIC_API_WEAK bool fansDisabledByBoardStatus() {
  return false;
}

PUBLIC_API_WEAK bool fansEnabledByBoardStatus() {
  return false;
}

// Refresh every condition before evaluating output priority, including while inhibited.
void FanController::updateConditions(bool acActive) {
	auto vss = Sensor::get(SensorType::VehicleSpeed);

#if EFI_SHAFT_POSITION_INPUT
	cranking = engine->rpmCalculator.isCranking();
	notRunning = !engine->rpmCalculator.isRunning();
#else
	cranking = false;
	notRunning = true;
#endif

	if (disableAtSpeed() > 0 && vss.Valid) {
		if (vss.Value > disableAtSpeed()) {
			disabledBySpeed = true;
		} else if (vss.Value < (disableAtSpeed() - disableAtSpeedHysteresis())) {
			disabledBySpeed = false;
		}
		// else: in hysteresis band — maintain previous disabledBySpeed
	} else {
		disabledBySpeed = false;
	}
	disabledWhileEngineStopped = notRunning && disableWhenStopped();

	auto clt = Sensor::get(SensorType::Clt);
	brokenClt = !clt;
	enabledForAc = getAcMode() == fan_ac_mode_e::Relay && acActive;
	hot = clt.value_or(0) > getFanOnTemp();
	cold = clt.value_or(0) < getFanOffTemp();
	enabledForAcPressure = getAcMode() == fan_ac_mode_e::Pressure &&
		(isPwmEnabled() ? getAcPressureDemand() > 0 : enabledForAcByPressure(getPin().getLogicValue()));
}

// Both output modes use the same inhibit priority.
bool FanController::isHardInhibited() {
	if (cranking) {
		radiatorFanStatus = (int)RadiatorFanState::Cranking;
		return true;
	} else if (disabledWhileEngineStopped) {
		radiatorFanStatus = (int)RadiatorFanState::EngineStopped;
		return true;
	} else if (disabledBySpeed) {
		radiatorFanStatus = (int)RadiatorFanState::VehicleIsTooFast;
		return true;
	} else if (fansDisabledByBoardStatus()) {
		radiatorFanStatus = (int)RadiatorFanState::BoardStatus;
		return true;
	}
	return false;
}

bool FanController::getState(bool lastState) {
	if (fansEnabledByBoardStatus()) {
		radiatorFanStatus = (int)RadiatorFanState::BoardForcedOn;
		return true;
	}
	if (isHardInhibited()) {
		// radiatorFanStatus already set inside isHardInhibited()
		return false;
	}

	if (brokenClt) {
		// If CLT is broken, turn the fan on
		radiatorFanStatus = (int)RadiatorFanState::CltBroken;
		return true;
	} else if (enabledForAcPressure) {
		radiatorFanStatus = (int)RadiatorFanState::AcPressure;
		return true;
	} else if (enabledForAc) {
	  radiatorFanStatus = (int)RadiatorFanState::AC;
		return true;
	} else if (hot) {
		radiatorFanStatus = (int)RadiatorFanState::Hot;
		return true;
	} else if (cold) {
		radiatorFanStatus = (int)RadiatorFanState::Cold;
		return false;
	} else {
	  radiatorFanStatus = (int)RadiatorFanState::Previous;
		// no condition met, maintain previous state
		return lastState;
	}
}

bool FanController::enabledForAcByPressure(bool lastState) {
	// Pressure is the command here: high-side pressure drives the fan regardless of
	// whether the compressor is currently engaged (eg static heat soak with clutch open).
	auto acPressure = Sensor::get(SensorType::AcPressure);
	if (!acPressure.Valid) {
		enabledForAcPressure = false;
		return false;
	}

	float pressure = acPressure.Value;
	if (pressure >= getAcPressureFanOnThreshold()) {
		enabledForAcPressure = true;
	} else if (pressure < getAcPressureFanOffThreshold()) {
		enabledForAcPressure = false;
	} else {
		// hysteresis band: maintain previous fan state
		enabledForAcPressure = lastState;
	}

	return enabledForAcPressure;
}

float FanController::getAcPressureDemand() const {
	auto pressure = Sensor::get(SensorType::AcPressure);
	float on = getAcPressureFanOnThreshold();
	float off = getAcPressureFanOffThreshold();
	if (!pressure || on <= off) {
		return 0;
	}
	return clampF(0, 100.0f * (pressure.Value - off) / (on - off), 100);
}

void FanController::initPwm() {
	if (m_pwmInitialized) {
		return;
	}
#if !EFI_UNIT_TEST
	if (!isBrainPinValid(getConfigPin())) {
		return;
	}
	float frequency = getPwmFrequency();
	if (frequency < 1) {
		// Guard against a corrupt/zero frequency (e.g. stale tune data after a config layout
		// change, or a bad manual edit): startSimplePwm() silently refuses to start below 1 Hz,
		// and without this guard initPwm() would still mark itself initialized, permanently
		// freezing the fan output pin at its resting level with no further diagnostic once the
		// one-time low-frequency warning scrolls by.
		warning(ObdCode::CUSTOM_OBD_LOW_FREQUENCY, "Fan PWM: invalid frequency %.0f, using 250Hz fallback", frequency);
		frequency = 250;
	}
	// getPin() (enginePins.fanRelay/fanRelay2) is already initialized under the "Fan Relay" name by
	// the generic RegisteredOutputPin::init() registration pass at boot - do NOT re-initPin() it
	// here under a different name ("Fan PWM"), that trips the pin repository's duplicate-ownership
	// check (CUSTOM_OBD_PIN_CONFLICT) since it looks like two features are fighting over the pin.
	startSimplePwm(&m_pwm, "Fan PWM", &engine->scheduler, &getPin(), frequency, 0);
	m_pwmInitialized = true;
#endif
}

void FanController::onSlowCallbackPwm(bool acActive) {
	initPwm();

	auto clt = Sensor::get(SensorType::Clt);
	auto acMode = getAcMode();

	// The temperature curve is the only source of truth for PWM speed - there is no separate
	// on/off temperature threshold here (unlike the relay path, which has no curve and still
	// needs one). Its lowest bin doubles as "fan off" and its highest bin as "fan full speed" for
	// the special-case demands below, gotten by reusing computeCurvePwm()'s own out-of-range
	// clamping (interpolate2d) with an extreme temperature rather than a separate accessor.
	float rawDemand;
	if (fansEnabledByBoardStatus()) {
		radiatorFanStatus = (int)RadiatorFanState::BoardForcedOn;
		rawDemand = computeCurvePwm(1000.0f);
	} else if (isHardInhibited()) {
		// radiatorFanStatus already set inside isHardInhibited()
		rawDemand = computeCurvePwm(-1000.0f);
	} else if (!clt) {
		// Fail safe: run at the curve's own "full speed" value rather than reading a stale/invalid
		// temperature.
		radiatorFanStatus = (int)RadiatorFanState::CltBroken;
		rawDemand = computeCurvePwm(1000.0f);
	} else {
		rawDemand = computeCurvePwm(clt.Value);
		radiatorFanStatus = (int)RadiatorFanState::Previous;
		if (acMode == fan_ac_mode_e::Relay && acActive) {
			// Compressor assistance requests the curve's full-speed demand before soft-start.
			radiatorFanStatus = (int)RadiatorFanState::AC;
			rawDemand = computeCurvePwm(1000.0f);
		} else if (acMode == fan_ac_mode_e::Pressure) {
			// Ramp demand from 0% at the Off pressure threshold to 100% at the On threshold, and
			// take the max with the temperature curve's own demand: pressure can only push the fan
			// faster than the curve already wants, never slower.
			float pressureDemand = getAcPressureDemand();
			if (pressureDemand > rawDemand) {
				radiatorFanStatus = (int)RadiatorFanState::AcPressure;
			}
			rawDemand = maxF(rawDemand, pressureDemand);
		}
	}
	rawDemand = clampF(0, rawDemand, 100);
	fanSpeedTarget = rawDemand; // 0% = fan off, 100% = full fan - table + AC/inhibit, pre-ramp
	m_state = rawDemand > 0;

	// Soft-start: limit upward slew rate in demand space (not raw duty) so ramping from 0 to full
	// demand takes softStartSec seconds the same way regardless of whether fan1MinPwm/fan1MaxPwm
	// describe a "normal" (min < max) or hardware-inverted (min > max) PWM signal. Skip the ramp
	// entirely on a broken sensor - fail-safe should be immediate, not gradual.
	float softSec = getSoftStartSec();
	if (clt && softSec > 0 && rawDemand > m_currentDemand) {
		float maxStep = 100.0f / (softSec * 20.0f);
		m_currentDemand = minF(rawDemand, m_currentDemand + maxStep);
	} else {
		m_currentDemand = rawDemand;
	}
	fanSpeedApplied = m_currentDemand; // same as target, but after soft-start ramping

	// fan1MinPwm is the duty cycle that means "fan off" (0% demand); fan1MaxPwm is the duty cycle
	// that means "fan full speed" (100% demand). Interpolating between them - rather than clamping
	// the demand value directly against [min, max] - lets min be numerically greater than max,
	// describing hardware that drives the fan with an inverted PWM signal (e.g. an NPN transistor
	// + pull-up: high duty = off, low duty = full speed).
	float minPwm = getMinPwm();
	float maxPwm = getMaxPwm();
	pwmAppliedPwm = minPwm + (m_currentDemand / 100.0f) * (maxPwm - minPwm);

#if !EFI_UNIT_TEST
	if (m_pwmInitialized) {
		m_pwm.setSimplePwmDutyCycle(pwmAppliedPwm / 100.0f);
	}
#endif
}

void FanController::onSlowCallback() {
#if EFI_PROD_CODE
	if (isRunningBenchTest()) {
	  radiatorFanStatus = (int)RadiatorFanState::Bench;
		return; // let's not mess with bench testing
	}
#endif

	bool acActive = engine->module<AcController>()->isAcEnabled();
	updateConditions(acActive);

	pwmActive = isPwmEnabled();
	if (isPwmEnabled()) {
		onSlowCallbackPwm(acActive);
		return;
	}

	auto& pin = getPin();

	bool result = getState(pin.getLogicValue());

	m_state = result;

	pin.setValue(result);
}

void FanController::setDefaultConfiguration() {
	engineConfiguration->disableFan1AtSpeedHysteresis = 5;
	engineConfiguration->disableFan2AtSpeedHysteresis = 5;

	engineConfiguration->fanOnTemperature = 92;
	engineConfiguration->fanOffTemperature = 88;
	engineConfiguration->fan2OnTemperature = 95;
	engineConfiguration->fan2OffTemperature = 91;

	engineConfiguration->fan1PwmFrequency = 250;
	engineConfiguration->fan2PwmFrequency = 250;

	setLinearCurve(engineConfiguration->fan1TempBins, 80, 110);
	setLinearCurve(engineConfiguration->fan1PwmValues, 0, 100);
	setLinearCurve(engineConfiguration->fan2TempBins, 85, 115);
	setLinearCurve(engineConfiguration->fan2PwmValues, 0, 100);

	// fan1MinPwm/fan1MaxPwm are the duty cycles meaning "off" and "full speed" respectively -
	// default assumes a non-inverted PWM signal path. See the field comment for how to set these
	// for hardware that drives the fan through an inverting stage (min > max).
	engineConfiguration->fan1MinPwm = 0;
	engineConfiguration->fan1MaxPwm = 100;
	engineConfiguration->fan2MinPwm = 0;
	engineConfiguration->fan2MaxPwm = 100;

	engineConfiguration->fan1SoftStartSec = 2.0f;
	engineConfiguration->fan2SoftStartSec = 2.0f;

	engineConfiguration->fan1AcMode = fan_ac_mode_e::Disabled;
	engineConfiguration->fan2AcMode = fan_ac_mode_e::Disabled;
	// A/C high-side pressure thresholds, in kPa.
	engineConfiguration->fan1AcPressureOn  = 1400;
	engineConfiguration->fan1AcPressureOff = 1100;
	engineConfiguration->fan2AcPressureOn  = 1800;
	engineConfiguration->fan2AcPressureOff = 1400;
}
