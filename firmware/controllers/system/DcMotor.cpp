/**
 *  @file	DcMotor.cpp
 *  @author Matthew Kennedy, (c) 2018
 */
#include "DcMotor.h"

TwoPinDcMotor::TwoPinDcMotor(SimplePwm* pwm, OutputPin* dir1, OutputPin* dir2)
    : m_dir1(dir1)
    , m_dir2(dir2)
    , m_pwm(pwm)
{

}

void TwoPinDcMotor::Set(float duty)
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

    // Clamp
    if(duty > 0.95f)
    {
        duty = 0.95f;
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
}
