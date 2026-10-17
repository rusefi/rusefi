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
  BoardForcedOn, // 11
  AcPressure // 12
};

struct FanController : public EngineModule, public fan_control_s {
	void onSlowCallback() override;
	void setDefaultConfiguration() override;

private:
	void updateConditions(bool acActive);
	bool getState(bool lastState);
	bool isHardInhibited();
	void initPwm();
	void onSlowCallbackPwm(bool acActive);

	bool enabledForAcByPressure(bool lastState);
	float getAcPressureDemand() const;

	SimplePwm m_pwm;
	bool m_pwmInitialized = false;
	// Slew-limited speed demand, 0..100 (0 = off, 100 = full speed). Slewing happens in demand
	// space rather than raw duty so soft-start behaves the same whether fan1MinPwm/fan1MaxPwm
	// describe a "normal" (min < max) or hardware-inverted (min > max) PWM signal.
	float m_currentDemand = 0.0f;

protected:
	virtual OutputPin& getPin() = 0;
	virtual float getFanOnTemp() = 0;
	virtual float getFanOffTemp() = 0;
	virtual fan_ac_mode_e getAcMode() const = 0;
	virtual bool disableWhenStopped() = 0;
	virtual int disableAtSpeed() = 0;
	virtual int disableAtSpeedHysteresis() = 0;

	// PWM mode virtuals
	virtual bool isPwmEnabled() const = 0;
	virtual brain_pin_e getConfigPin() const = 0;
	virtual float getPwmFrequency() const = 0;
	virtual float computeCurvePwm(float tempC) const = 0;
	virtual float getMinPwm() const = 0;
	virtual float getMaxPwm() const = 0;
	virtual float getSoftStartSec() const = 0;

	virtual float getAcPressureFanOnThreshold() const = 0;
	virtual float getAcPressureFanOffThreshold() const = 0;
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

	fan_ac_mode_e getAcMode() const override {
		return engineConfiguration->fan1AcMode;
	}

	bool disableWhenStopped() {
		return engineConfiguration->disableFan1WhenStopped;
	}

	int disableAtSpeed() {
		return engineConfiguration->disableFan1AtSpeed;
	}

	int disableAtSpeedHysteresis() {
		return engineConfiguration->disableFan1AtSpeedHysteresis;
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

	float getSoftStartSec() const override {
		return engineConfiguration->fan1SoftStartSec;
	}

	float getAcPressureFanOnThreshold() const override {
		return engineConfiguration->fan1AcPressureOn;
	}

	float getAcPressureFanOffThreshold() const override {
		return engineConfiguration->fan1AcPressureOff;
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

	fan_ac_mode_e getAcMode() const override {
		return engineConfiguration->fan2AcMode;
	}

	bool disableWhenStopped() {
		return engineConfiguration->disableFan2WhenStopped;
	}

	int disableAtSpeed() {
		return engineConfiguration->disableFan2AtSpeed;
	}

	int disableAtSpeedHysteresis() {
		return engineConfiguration->disableFan2AtSpeedHysteresis;
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

	float getSoftStartSec() const override {
		return engineConfiguration->fan2SoftStartSec;
	}

	float getAcPressureFanOnThreshold() const override {
		return engineConfiguration->fan2AcPressureOn;
	}

	float getAcPressureFanOffThreshold() const override {
		return engineConfiguration->fan2AcPressureOff;
	}
};
