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

bool FanController::getState(bool acActive, bool lastState) {
	auto clt = Sensor::get(SensorType::Clt);
	auto vss = Sensor::get(SensorType::VehicleSpeed);

#if EFI_SHAFT_POSITION_INPUT
	cranking = engine->rpmCalculator.isCranking();
	notRunning = !engine->rpmCalculator.isRunning();
#else
	cranking = false;
	notRunning = true;
#endif

	disabledBySpeed = disableAtSpeed() > 0 && vss.Valid && vss.Value > disableAtSpeed();
	disabledWhileEngineStopped = notRunning && disableWhenStopped();
	brokenClt = !clt;
	enabledForAc = enableWithAc() && acActive;
	hot = clt.value_or(0) > getFanOnTemp();
	cold = clt.value_or(0) < getFanOffTemp();

	if (fansEnabledByBoardStatus()) {
		radiatorFanStatus = (int)RadiatorFanState::BoardForcedOn;
		return true;
	} else if (cranking) {
		// Inhibit while cranking
		radiatorFanStatus = (int)RadiatorFanState::Cranking;
		return false;
	} else if (disabledWhileEngineStopped) {
		// Inhibit while not running (if so configured)
		radiatorFanStatus = (int)RadiatorFanState::EngineStopped;
		return false;
	} else if (disabledBySpeed) {
		// Inhibit while driving fast
		radiatorFanStatus = (int)RadiatorFanState::VehicleIsTooFast;
		return false;
  } else if (fansDisabledByBoardStatus()) {
    radiatorFanStatus = (int)RadiatorFanState::BoardStatus;
		return false;
	} else if (brokenClt) {
		// If CLT is broken, turn the fan on
		radiatorFanStatus = (int)RadiatorFanState::CltBroken;
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

void FanController::initPwm() {
	if (m_pwmInitialized) {
		return;
	}
#ifndef EFI_UNIT_TEST
	if (!isBrainPinValid(getConfigPin())) {
		return;
	}
	startSimplePwm(&m_pwm, "Fan PWM", &engine->scheduler, &getPin(), getPwmFrequency(), 0);
	m_pwmInitialized = true;
#endif
}

void FanController::onSlowCallbackPwm(bool acActive) {
	initPwm();

	auto clt = Sensor::get(SensorType::Clt);

	if (!clt) {
		// Sensor invalid – run fan at max PWM
		float safeDuty = getMaxPwm();
		pwmCurvePwm = safeDuty;
		pwmTargetPwm = safeDuty;
		m_currentPwm = safeDuty;
		pwmAppliedPwm = safeDuty;
		m_state = (safeDuty > 0);
#ifndef EFI_UNIT_TEST
		if (m_pwmInitialized) {
			m_pwm.setSimplePwmDutyCycle(safeDuty / 100.0f);
		}
#endif
		return;
	}

	pwmCurvePwm = computeCurvePwm(clt.Value);

	float target = pwmCurvePwm;
	if (acActive) {
		target += getPwmAcAdder();
	}
	target = clampF(getMinPwm(), target, getMaxPwm());
	pwmTargetPwm = target;

	// Soft-start: limit upward slew rate so ramp from 0-100 takes softStartSec seconds
	float softSec = getSoftStartSec();
	if (softSec > 0 && target > m_currentPwm) {
		float maxStep = 100.0f / (softSec * 20.0f);
		target = minF(target, m_currentPwm + maxStep);
	}

	m_currentPwm = target;
	pwmAppliedPwm = m_currentPwm;
	m_state = (m_currentPwm > 0);

#ifndef EFI_UNIT_TEST
	if (m_pwmInitialized) {
		m_pwm.setSimplePwmDutyCycle(m_currentPwm / 100.0f);
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

	pwmActive = isPwmEnabled();
	if (isPwmEnabled()) {
		onSlowCallbackPwm(acActive);
		return;
	}

	auto& pin = getPin();

	bool result = getState(acActive, pin.getLogicValue());

	m_state = result;

	pin.setValue(result);
}

void FanController::setDefaultConfiguration() {
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

	engineConfiguration->fan1MinPwm = 20;
	engineConfiguration->fan1MaxPwm = 100;
	engineConfiguration->fan2MinPwm = 20;
	engineConfiguration->fan2MaxPwm = 100;

	engineConfiguration->fan1AcAdder = 10;
	engineConfiguration->fan2AcAdder = 10;

	engineConfiguration->fan1SoftStartSec = 2.0f;
	engineConfiguration->fan2SoftStartSec = 2.0f;
}
