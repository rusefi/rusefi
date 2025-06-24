#include "pch.h"

#include "fan_control.h"

#include "bench_test.h"

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

void FanController::onSlowCallback() {
#if EFI_PROD_CODE
	if (isRunningBenchTest()) {
	  radiatorFanStatus = (int)RadiatorFanState::Bench;
		return; // let's not mess with bench testing
	}
#endif

	bool acActive = engine->module<AcController>()->isAcEnabled();

	auto& pin = getPin();

	bool result = getState(acActive, pin.getLogicValue());

	m_state = result;

	pin.setValue(result);
}

void FanController::onDefaultEngineConfiguration() {
	engineConfiguration->fanOnTemperature = 92;
	engineConfiguration->fanOffTemperature = 88;
	engineConfiguration->fan2OnTemperature = 95;
	engineConfiguration->fan2OffTemperature = 91;
}