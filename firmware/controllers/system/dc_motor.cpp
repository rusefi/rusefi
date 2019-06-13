/**
 * @file DcMotor.cpp
 * @brief DC motor controller
 * 
 * @date Dec 22, 2018
 * @author Matthew Kennedy
 */

#include "dc_motor.h"

TwoPinDcMotor::TwoPinDcMotor(SimplePwm* enable, SimplePwm* dir1, SimplePwm* dir2)
    : m_enable(enable)
    , m_dir1(dir1)
    , m_dir2(dir2)
{
}

bool TwoPinDcMotor::isOpenDirection() const {
	return m_value >= 0;
}

float TwoPinDcMotor::Get() const {
	return m_value;
}

/**
 * @param duty value between -1.0 and 1.0
 */
bool TwoPinDcMotor::Set(float duty)
{
	m_value = duty;

    bool isPositive = duty > 0;

    if(!isPositive)
    {
        duty = -duty;
    }

    // below here 'duty' is a not negative

    // Clamp to 100%
    if(duty > 1.0f)
    {
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
