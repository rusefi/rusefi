/**
 * @file dc_motors.cpp
 *
 * @date March 3, 2020
 * @author Matthew Kennedy (c) 2020
 */

#include "pch.h"

#include "periodic_task.h"

#include "dc_motors.h"
#include "dc_motor.h"

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

public:
	DcHardware() : dcMotor(m_disablePin) {}

	TwoPinDcMotor dcMotor;
	
	void setFrequency(int frequency) {
		m_pwm1.setFrequency(frequency);
		m_pwm2.setFrequency(frequency);
	}

	void start(bool useTwoWires, 
			brain_pin_e pinEnable,
			brain_pin_e pinDir1,
			brain_pin_e pinDir2,
			brain_pin_e pinDisable,
			ExecutorInterface* executor,
			int frequency) {
		dcMotor.setType(useTwoWires ? TwoPinDcMotor::ControlType::PwmDirectionPins : TwoPinDcMotor::ControlType::PwmEnablePin);

		// Configure the disable pin first - ensure things are in a safe state
		m_disablePin.initPin("ETB Disable", pinDisable);
		m_disablePin.setValue(0);

		// Clamp to >100hz
		int clampedFrequency = maxI(100, frequency);

		if (useTwoWires) {
// no need to complicate event queue with ETB PWM in unit tests
#if ! EFI_UNIT_TEST
			startSimplePwmHard(&m_pwm1, "ETB Dir 1",
				executor,
				pinDir1,
				&m_pinDir1,
				clampedFrequency,
				0
			);

			startSimplePwmHard(&m_pwm1, "ETB Dir 2",
				executor,
				pinDir2,
				&m_pinDir2,
				clampedFrequency,
				0
			);
#endif // EFI_UNIT_TEST

			dcMotor.configure(wrappedEnable, m_pwm1, m_pwm2);
		} else {
// no need to complicate event queue with ETB PWM in unit tests
#if ! EFI_UNIT_TEST
			startSimplePwmHard(&m_pwm1, "ETB Enable",
				executor,
				pinEnable,
				&m_pinEnable,
				clampedFrequency,
				0
			);
#endif // EFI_UNIT_TEST

			dcMotor.configure(m_pwm1, wrappedDir1, wrappedDir2);
		}
	}
};

static DcHardware dcHardware[ETB_COUNT + DC_PER_STEPPER];

DcMotor* initDcMotor(const dc_io& io, size_t index, bool useTwoWires DECLARE_ENGINE_PARAMETER_SUFFIX) {
	auto& hw = dcHardware[index];

	hw.start(
		useTwoWires,
		io.controlPin,
		io.directionPin1,
		io.directionPin2,
		io.disablePin,
		&ENGINE(executor),
		CONFIG(etbFreq)
	);

	return &hw.dcMotor;
}

void setDcMotorFrequency(size_t index, int hz) {
	dcHardware[index].setFrequency(hz);
}

void setDcMotorDuty(size_t index, float duty) {
	dcHardware[index].dcMotor.set(duty);
}


void showDcMotorInfo(int i) {
	DcHardware *dc = &dcHardware[i];

	efiPrintf(" motor: dir=%d DC=%f", dc->dcMotor.isOpenDirection(), dc->dcMotor.get());
}

