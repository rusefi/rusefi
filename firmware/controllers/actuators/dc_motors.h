/**
 * @file dc_motors.h
 *
 * @date March 3, 2020
 * @author Matthew Kennedy (c) 2020
 */

#pragma once

#include "dc_motor.h"

#include <cstddef>

DcMotor* initDcMotor(const char *disPinMsg, const dc_io& io, size_t index, bool useTwoWires);
DcMotor* initDcMotor(brain_pin_e coil_p, brain_pin_e coil_m, size_t index);

// Manual control of motors for use by console commands
bool setDcMotorFrequency(size_t index, int hz);
void setDcMotorDuty(size_t index, float duty);

#if EFI_UNIT_TEST
DcMotor* getDcMotorForUnitTest(size_t index);
void resetDcHardwareForUnitTest();
void setDcHardwarePwmForUnitTest(size_t index, hardware_pwm* pwm);
#endif // EFI_UNIT_TEST

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
    			const char *disPinMsg,
    			brain_pin_e pinDisable,
    			bool isInverted,
    			Scheduler* executor,
    			int frequency);

	TwoPinDcMotor dcMotor;

	// Reprograms the running bridge PWM; bypasses the ETB clamps applied by start().
	// @return false if the hardware refused the frequency (both channels share the limits).
	bool setFrequency(int frequency) {
		bool ok = m_pwm1.setFrequency(frequency);
		ok = m_pwm2.setFrequency(frequency) && ok;
		return ok;
	}

	const char *msg() {
		return dcMotor.msg();
	}

	void stop() {
		// todo: replace 'isStarted' with 'stop'
	}

#if EFI_UNIT_TEST
	// Unit tests never start hardware PWM (see start()); this stands in for the timer channel
	// that startSimplePwmHard() attaches on real hardware.
	void setHardwarePwmForUnitTest(hardware_pwm* pwm) {
		m_pwm1.hardPwm = pwm;
	}
#endif // EFI_UNIT_TEST
};

DcHardware *getPrimaryDCHardwareForLogging();
