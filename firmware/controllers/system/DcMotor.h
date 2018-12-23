/**
 *  @file	DcMotor.h
 *  @author Matthew Kennedy, (c) 2018
 */
#pragma once

#include "pwm_generator_logic.h"

class DcMotor
{
public:
    virtual void Set(float duty) = 0;
};

class TwoPinDcMotor : public DcMotor
{
private:
    OutputPin* const m_dir1;
    OutputPin* const m_dir2;
    SimplePwm* const m_pwm;
public:
    TwoPinDcMotor(SimplePwm* pwm, OutputPin* dir1, OutputPin* dir2);

    virtual void Set(float duty) override;
};
