#include "pch.h"

#include "fan_control.h"

#include "bench_test.h"

PUBLIC_API_WEAK bool fansDisabledByBoardStatus() {
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

	if (cranking) {
		// Inhibit while cranking
		tempCode = 31;
		return false;
	} else if (disabledWhileEngineStopped) {
		// Inhibit while not running (if so configured)
		tempCode = 32;
		return false;
	} else if (disabledBySpeed) {
		// Inhibit while driving fast
		tempCode = 33;
		return false;
  } else if (fansDisabledByBoardStatus()) {
    tempCode = 34;
		return false;
	} else if (brokenClt) {
		// If CLT is broken, turn the fan on
		tempCode = 35;
		return true;
	} else if (enabledForAc) {
	  tempCode = 36;
		return true;
	} else if (hot) {
		// If hot, turn the fan on
		tempCode = 37;
		return true;
	} else if (cold) {
		// If cold, turn the fan off
		tempCode = 38;
		return false;
	} else {
	  tempCode = 40;
		// no condition met, maintain previous state
		return lastState;
	}
}

void FanController::onSlowCallback() {
#if EFI_PROD_CODE
	if (isRunningBenchTest()) {
	  tempCode = 30;
		return; // let's not mess with bench testing
	}
#endif

	bool acActive = engine->module<AcController>()->isAcEnabled();

	auto& pin = getPin();

	bool result = getState(acActive, pin.getLogicValue());
	tempAlive = tempAlive + 1;

	pin.setValue(result);
}
