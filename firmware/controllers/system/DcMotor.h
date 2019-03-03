/**
 *  @file DcMotor.h
 * 
 *  @date Dec 22, 2018
 *  @author Matthew Kennedy, (c) 2018
 */

#pragma once

#include "pwm_generator_logic.h"

/**
 * @brief Brushed or brushless DC motor interface
 * 
 * Represents a DC motor (brushed or brushless) that provides simple
 * torque/power/current/duty cycle control, but not accurate absolute position control.
 */
class DcMotor
{
public:
    /**
     * @brief Sets the motor duty cycle.
     * @param duty +1.0f represents full power forward, and -1.0f represents full power backward.
     * @return True if any fault was detected driving the motor, and false if successful.
     */
    virtual bool Set(float duty) = 0;
};

/**
 * @brief Represents a DC motor controller (H bridge) with one pin for enable (PWM),
 * and two pins for direction control.
 * 
 * The enable pin is used for PWM and disable, and the two direction pins are used
 * to set the polarity of each half of the H bridge.  setting {dir1,dir2} = 10 should,
 * turn the motor one direction (positive duty), and = 01 should turn the other way (negative
 * duty).
 */
class TwoPinDcMotor : public DcMotor
{
private:
    SimplePwm* const m_pwm;
    OutputPin* const m_dir1;
    OutputPin* const m_dir2;
    float value = 0;
    bool isPositiveOrZero = false;
public:
    /**
     * @param pwm SimplePwm driver for enable pin, for PWM speed control.
     * @param dir1 Enable 1 or direction 1 pin.  Gets set high to rotate forward.
     * @param dir2 Enable 2 or direction 2 pin.  Gets set high to rotate backward.
     */
    TwoPinDcMotor(SimplePwm* pwm, OutputPin* dir1, OutputPin* dir2);

    virtual bool Set(float duty) override;
    float Get();
    void Break();
    bool isOpenDirection();
};
