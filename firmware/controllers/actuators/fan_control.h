#pragma once

#include "fan_control_generated.h"

struct FanController : public EngineModule, public fan_control_s {
	void onSlowCallback() override;

private:
	bool getState(bool acActive, bool lastState);

protected:
	virtual OutputPin& getPin() = 0;
	virtual float getFanOnTemp() = 0;
	virtual float getFanOffTemp() = 0;
	virtual bool enableWithAc() = 0;
	virtual bool disableWhenStopped() = 0;
};

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
