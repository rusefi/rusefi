/**
 * @file dc_motors.h
 *
 * @date March 3, 2020
 * @author Matthew Kennedy (c) 2020
 */

#pragma once

#include "dc_motor.h"

#include <cstddef>

DcMotor* initDcMotor(const dc_io& io, size_t index, bool useTwoWires);
DcMotor* initDcMotor(brain_pin_e coil_p, brain_pin_e coil_m, size_t index);

// Manual control of motors for use by console commands
void setDcMotorFrequency(size_t index, int hz);
void setDcMotorDuty(size_t index, float duty);

void showDcMotorInfo(int i);

// Simple wrapper to use an OutputPin as "PWM" that can only do 0 or 1
struct PwmWrapper : public IPwm {
	OutputPin& m_pin;

	PwmWrapper(OutputPin& pin) : m_pin(pin) { }

	void setSimplePwmDutyCycle(float dutyCycle) override {
		m_pin.setValue(dutyCycle > 0.5f);
	}
};

class DcHardware {
private:
	OutputPin m_pinEnable;
	OutputPin m_pinDir1;
	OutputPin m_pinDir2;
	OutputPin m_disablePin;

	PwmWrapper wrappedEnable{m_pinEnable};
	PwmWrapper wrappedDir1{m_pinDir1};
	PwmWrapper wrappedDir2{m_pinDir2};

	SimplePwm m_pwm1;
	SimplePwm m_pwm2;

	bool isStarted = false;
public:

	DcHardware() : dcMotor(m_disablePin) {}

	void start(bool useTwoWires,
				brain_pin_e pinEnable,
				brain_pin_e pinDir1,
				brain_pin_e pinDir2,
				brain_pin_e pinDisable,
				bool isInverted,
				ExecutorInterface* executor,
				int frequency);

	TwoPinDcMotor dcMotor;

	void setFrequency(int frequency) {
		m_pwm1.setFrequency(frequency);
		m_pwm2.setFrequency(frequency);
	}

	const char *msg() {
	    return dcMotor.msg;
	}

	void stop() {
		// todo: replace 'isStarted' with 'stop'
	}

};
