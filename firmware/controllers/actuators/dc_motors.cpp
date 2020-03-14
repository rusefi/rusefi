/**
 * @file dc_motors.cpp
 *
 * @date March 3, 2020
 * @author Matthew Kennedy (c) 2020
 */

#include "engine.h"
#include "io_pins.h"
#include "engine_configuration.h"
#include "engine_controller.h"
#include "periodic_task.h"

#include "dc_motors.h"
#include "dc_motor.h"

#include "efi_gpio.h"
#include "pwm_generator.h"

EXTERN_ENGINE;

class EtbHardware {
private:
	OutputPin m_pinEnable;
	OutputPin m_pinDir1;
	OutputPin m_pinDir2;

	SimplePwm m_pwmEnable;
	SimplePwm m_pwmDir1;
	SimplePwm m_pwmDir2;

	SimplePwm etbPwmUp;

public:
	EtbHardware() : etbPwmUp("etbUp"), dcMotor(&m_pwmEnable, &m_pwmDir1, &m_pwmDir2) {}

	TwoPinDcMotor dcMotor;
	
	void setFrequency(int frequency) {
		m_pwmEnable.setFrequency(frequency);
		m_pwmDir1.setFrequency(frequency);
		m_pwmDir2.setFrequency(frequency);
	}

	void start(bool useTwoWires, 
			brain_pin_e pinEnable,
			// since we have pointer magic here we cannot simply have value parameter
			const pin_output_mode_e *pinEnableMode,
			brain_pin_e pinDir1,
			brain_pin_e pinDir2,
			ExecutorInterface* executor,
			int frequency) {
		dcMotor.setType(useTwoWires ? TwoPinDcMotor::ControlType::PwmDirectionPins : TwoPinDcMotor::ControlType::PwmEnablePin);

		m_pinEnable.initPin("ETB Enable", pinEnable, pinEnableMode);
		m_pinDir1.initPin("ETB Dir 1", pinDir1);
		m_pinDir2.initPin("ETB Dir 2", pinDir2);

		// Clamp to >100hz
		int clampedFrequency = maxI(100, frequency);

// no need to complicate event queue with ETB PWM in unit tests
#if ! EFI_UNIT_TEST
		startSimplePwm(&m_pwmEnable, "ETB Enable",
			executor,
			&m_pinEnable,
			clampedFrequency,
			0,
			(pwm_gen_callback*)applyPinState
		);

		startSimplePwm(&m_pwmDir1, "ETB Dir 1",
			executor,
			&m_pinDir1,
			clampedFrequency,
			0,
			(pwm_gen_callback*)applyPinState
		);

		startSimplePwm(&m_pwmDir2, "ETB Dir 2",
			executor,
			&m_pinDir2,
			clampedFrequency,
			0,
			(pwm_gen_callback*)applyPinState
		);
#endif /* EFI_UNIT_TEST */
	}
};

static EtbHardware etbHardware[ETB_COUNT];

DcMotor* initDcMotor(size_t index DECLARE_ENGINE_PARAMETER_SUFFIX)
{
	const auto& io = engineConfiguration->etbIo[index];
	auto& hw = etbHardware[index];

	// controlPinMode is a strange feature - it's simply because I am short on 5v I/O on Frankenso with Miata NB2 test mule
	hw.start(
		CONFIG(etb_use_two_wires),
		io.controlPin1,
		&io.controlPinMode,
		io.directionPin1,
		io.directionPin2,
		&ENGINE(executor),
		CONFIG(etbFreq)
	);

	return &hw.dcMotor;
}

void setDcMotorFrequency(size_t index, int hz) {
	etbHardware[index].setFrequency(hz);
}

void setDcMotorDuty(size_t index, float duty) {
	etbHardware[index].dcMotor.set(duty);
}

#if EFI_PROD_CODE
void showDcMotorInfo(Logging* logger) {
	for (int i = 0 ; i < engine->etbActualCount; i++) {
		EtbHardware *etb = &etbHardware[i];

		scheduleMsg(logger, "ETB %d", i);
		scheduleMsg(logger, "Motor: dir=%d DC=%f", etb->dcMotor.isOpenDirection(), etb->dcMotor.get());
	}
}
#endif