#pragma once

#include "fan_control_generated.h"

enum class RadiatorFanState : uint8_t {
  None, // 0
  Cranking, // 1
  EngineStopped, // 2
  VehicleIsTooFast, // 3
  BoardStatus, // 4
  CltBroken, // 5
  AC, // 6
  Hot, // 7
  Cold, // 8
  Previous, // 9
  Bench
};

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
	virtual int disableAtSpeed() = 0;
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

	int disableAtSpeed() {
		return engineConfiguration->disableFan1AtSpeed;
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

	int disableAtSpeed() {
		return engineConfiguration->disableFan2AtSpeed;
	}
};
