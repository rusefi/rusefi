#include "pch.h"

#include "fan_control.h"

#include "bench_test.h"

bool FanController::getState(bool acActive, bool lastState) {
	auto [cltValid, clt] = Sensor::get(SensorType::Clt);

	cranking = engine->rpmCalculator.isCranking();
	notRunning = !engine->rpmCalculator.isRunning();

	disabledWhileEngineStopped = notRunning && disableWhenStopped();
	brokenClt = !cltValid;
	enabledForAc = enableWithAc() && acActive;
	hot = clt > getFanOnTemp();
	cold = clt < getFanOffTemp();

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

struct FanControl1 : public FanController {
	OutputPin& getPin() {
		return enginePins.fanRelay;
	}

	float getFanOnTemp() {
		return engineConfiguration->fanOnTemperature;
	}

	float getFanOffTemp() {
		return engineConfiguration->fanOffTemperature;
	}

	bool enableWithAc() {
		return engineConfiguration->enableFan1WithAc;
	}

	bool disableWhenStopped() {
		return engineConfiguration->disableFan1WhenStopped;
	}
};

struct FanControl2 : public FanController {
	OutputPin& getPin() {
		return enginePins.fanRelay2;
	}

	float getFanOnTemp() {
		return engineConfiguration->fan2OnTemperature;
	}

	float getFanOffTemp() {
		return engineConfiguration->fan2OffTemperature;
	}

	bool enableWithAc() {
		return engineConfiguration->enableFan2WithAc;
	}

	bool disableWhenStopped() {
		return engineConfiguration->disableFan2WhenStopped;
	}
};

static FanControl1 fan1;
static FanControl2 fan2;

void updateFans(bool acActive) {
#if EFI_PROD_CODE
	if (isRunningBenchTest()) {
		return; // let's not mess with bench testing
	}
#endif

	fan1.update(acActive);
	fan2.update(acActive);
}
