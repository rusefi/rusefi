
/**
 * @file DcMotor.cpp
 * @brief DC motor controller
 * 
 * @date Dec 22, 2018
 * @author Matthew Kennedy
 */

#include "DcMotor.h"

TwoPinDcMotor::TwoPinDcMotor(SimplePwm* pwm, OutputPin* dir1, OutputPin* dir2)
    : m_pwm(pwm)
    , m_dir1(dir1)
    , m_dir2(dir2)
{
}

bool TwoPinDcMotor::Set(float duty)
{
    bool dir;

    if(duty < 0)
    {
        dir = false;
        duty = -duty;
    }
    else
    {
        dir = true;
    }

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

    if(duty < 0.01f)
    {
        m_dir1->setValue(false);
        m_dir2->setValue(false);
    }
    else
    {
        m_dir1->setValue(dir);
        m_dir2->setValue(!dir);
    }

    m_pwm->setSimplePwmDutyCycle(duty);

    // This motor has no fault detection, so always return false (indicate success).
    return false;
}
