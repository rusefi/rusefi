/**
 * @file DcMotor.cpp
 * @brief DC motor controller
 * 
 * @date Dec 22, 2018
 * @author Matthew Kennedy
 */

#include "dc_motor.h"

TwoPinDcMotor::TwoPinDcMotor(OutputPin& disablePin)
	: m_disable(&disablePin)
{
	disable();
}

void TwoPinDcMotor::configure(IPwm& enable, IPwm& dir1, IPwm& dir2) {
	m_enable = &enable;
	m_dir1 = &dir1;
	m_dir2 = &dir2;
}

void TwoPinDcMotor::enable() {
	if (m_disable) {
		m_disable->setValue(false);
	}
}

void TwoPinDcMotor::disable() {
	if (m_disable) {
		m_disable->setValue(true);
	}

	// Also set the duty to zero
	set(0);
}

bool TwoPinDcMotor::isOpenDirection() const {
	return m_value >= 0;
}

float TwoPinDcMotor::get() const {
	return m_value;
}

/**
 * @param duty value between -1.0 and 1.0
 */
bool TwoPinDcMotor::set(float duty)
{
	m_value = duty;

	// If not init, don't try to set
	if (!m_dir1 || !m_dir2 || !m_enable) {
		if (m_disable) {
			m_disable->setValue(true);
		}

		return false;
	}

	bool isPositive = duty > 0;

	if (!isPositive) {
		duty = -duty;
	}

	// below here 'duty' is a not negative

	// Clamp to 100%
	if (duty > 1.0f) {
		duty = 1.0f;
	}
	// Disable for very small duty
	else if (duty < 0.01f)
	{
		duty = 0.0f;
	}

	// If we're in two pin mode, force 100%, else use this pin to PWM
	float enableDuty = m_type == ControlType::PwmEnablePin ? duty : 1;

	// Direction pins get 100% duty unless we're in PwmDirectionPins mode
	float dirDuty = m_type == ControlType::PwmDirectionPins ? duty : 1;

	m_enable->setSimplePwmDutyCycle(enableDuty);
	m_dir1->setSimplePwmDutyCycle(isPositive ? dirDuty : 0);
	m_dir2->setSimplePwmDutyCycle(isPositive ? 0 : dirDuty);

	// This motor has no fault detection, so always return false (indicate success).
	return false;
}
