#include "pch.h"

#include "fan_control.h"

#include "bench_test.h"

bool FanController::getState(bool acActive, bool lastState) {
	auto clt = Sensor::get(SensorType::Clt);

	cranking = engine->rpmCalculator.isCranking();
	notRunning = !engine->rpmCalculator.isRunning();

	disabledWhileEngineStopped = notRunning && disableWhenStopped();
	brokenClt = !clt;
	enabledForAc = enableWithAc() && acActive;
	hot = clt.value_or(0) > getFanOnTemp();
	cold = clt.value_or(0) < getFanOffTemp();

	if (cranking) {
		// Inhibit while cranking
		return false;
	} else if (disabledWhileEngineStopped) {
		// Inhibit while not running (if so configured)
		return false;
	} else if (brokenClt) {
		// If CLT is broken, turn the fan on
		return true;
	} else if (enabledForAc) {
		return true;
	} else if (hot) {
		// If hot, turn the fan on
		return true;
	} else if (cold) {
		// If cold, turn the fan off
		return false;
	} else {
		// no condition met, maintain previous state
		return lastState;
	}
}

void FanController::update(bool acActive) {
	auto& pin = getPin();

	bool result = getState(acActive, pin.getLogicValue());

	pin.setValue(result);
}

void updateFans(bool acActive) {
#if EFI_PROD_CODE
	if (isRunningBenchTest()) {
		return; // let's not mess with bench testing
	}
#endif

	engine->fan1.update(acActive);
	engine->fan2.update(acActive);
}
