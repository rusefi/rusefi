#pragma once

#include "fan_control_generated.h"
#include "pwm_generator_logic.h"

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
  Bench, // 10
  BoardForcedOn // 11
};

struct FanController : public EngineModule, public fan_control_s {
	void onSlowCallback() override;
	void setDefaultConfiguration() override;

private:
	bool getState(bool acActive, bool lastState);
	void initPwm();
	void onSlowCallbackPwm(bool acActive);

	SimplePwm m_pwm;
	bool m_pwmInitialized = false;
	float m_currentPwm = 0.0f;

protected:
	virtual OutputPin& getPin() = 0;
	virtual float getFanOnTemp() = 0;
	virtual float getFanOffTemp() = 0;
	virtual bool enableWithAc() = 0;
	virtual bool disableWhenStopped() = 0;
	virtual int disableAtSpeed() = 0;

	// PWM mode virtuals
	virtual bool isPwmEnabled() const = 0;
	virtual brain_pin_e getConfigPin() const = 0;
	virtual float getPwmFrequency() const = 0;
	virtual float computeCurvePwm(float tempC) const = 0;
	virtual float getMinPwm() const = 0;
	virtual float getMaxPwm() const = 0;
	virtual float getPwmAcAdder() const = 0;
	virtual float getSoftStartSec() const = 0;
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

	bool isPwmEnabled() const override {
		return engineConfiguration->fan1PwmEnabled;
	}

	brain_pin_e getConfigPin() const override {
		return engineConfiguration->fanPin;
	}

	float getPwmFrequency() const override {
		return engineConfiguration->fan1PwmFrequency;
	}

	float computeCurvePwm(float tempC) const override {
		return interpolate2d(tempC, engineConfiguration->fan1TempBins, engineConfiguration->fan1PwmValues);
	}

	float getMinPwm() const override {
		return engineConfiguration->fan1MinPwm;
	}

	float getMaxPwm() const override {
		return engineConfiguration->fan1MaxPwm;
	}

	float getPwmAcAdder() const override {
		return engineConfiguration->fan1AcAdder;
	}

	float getSoftStartSec() const override {
		return engineConfiguration->fan1SoftStartSec;
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

	bool isPwmEnabled() const override {
		return engineConfiguration->fan2PwmEnabled;
	}

	brain_pin_e getConfigPin() const override {
		return engineConfiguration->fan2Pin;
	}

	float getPwmFrequency() const override {
		return engineConfiguration->fan2PwmFrequency;
	}

	float computeCurvePwm(float tempC) const override {
		return interpolate2d(tempC, engineConfiguration->fan2TempBins, engineConfiguration->fan2PwmValues);
	}

	float getMinPwm() const override {
		return engineConfiguration->fan2MinPwm;
	}

	float getMaxPwm() const override {
		return engineConfiguration->fan2MaxPwm;
	}

	float getPwmAcAdder() const override {
		return engineConfiguration->fan2AcAdder;
	}

	float getSoftStartSec() const override {
		return engineConfiguration->fan2SoftStartSec;
	}
};
