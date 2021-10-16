/**
 *  @file dc_motor.h
 *
 * ETB user documentation at https://github.com/rusefi/rusefi/wiki/HOWTO_electronic_throttle_body
 * 
 *
 *  @date Dec 22, 2018
 *  @author Matthew Kennedy, (c) 2018
 */

#pragma once

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
	virtual bool set(float duty) = 0;

	/**
	 * @brief Get the current motor duty cycle.
	 * @return The current duty cycle setting. +1.0f represents full power forward, and -1.0f represents full power backward.
	 */
	virtual float get() const = 0;

	virtual void disable() = 0;
	virtual void enable() = 0;

	virtual bool isOpenDirection() const = 0;
};

struct IPwm;
class OutputPin;

/**
 * @brief Represents a DC motor controller (H-bridge) with some combination of PWM and on/off control pins.
 *
 */
class TwoPinDcMotor : public DcMotor
{
public:
	enum class ControlType
	{
		/**
		 * For example TLE7209 - two control wires:
		 * PWM on both wires - one to open, another to close
		 */
		PwmDirectionPins,
		/**
		  * The control/enable pin is used for PWM and disable, and the two direction pins are used
		  * to set the polarity of each half of the H bridge.  setting {dir1,dir2} = 10 should,
		  * turn the motor one direction (positive duty), and = 01 should turn the other way (negative
		  * duty).
		 *
		 * For example VNH2SP30 - three control wires:
		 * PWM on 'enable' PIN, two binary pins for direction
		 *
		 * TLE9201 with two wire control also uses this mode
		 * PWM on one pin, open/close using one binary direction pin, second direction pin unused
		 */
		PwmEnablePin,
	};

private:
	IPwm* m_enable = nullptr;
	IPwm* m_dir1 = nullptr;
	IPwm* m_dir2 = nullptr;
	OutputPin* const m_disable;
	float m_value = 0;
	bool m_isInverted = false;

	ControlType m_type = ControlType::PwmDirectionPins;
public:
	/**
	 * @param enable IPwm driver for enable pin, for PWM speed control.
	 * @param dir1 Enable 1 or direction 1 pin.  Gets set high to rotate forward.
	 * @param dir2 Enable 2 or direction 2 pin.  Gets set high to rotate backward.
	 */
	TwoPinDcMotor(OutputPin& disable);

	void configure(IPwm& enable, IPwm& dir1, IPwm& dir2, bool isInverted);

	virtual bool set(float duty) override;
	float get() const override;
	bool isOpenDirection() const override;

	void enable() override;
	void disable() override;

	void setType(ControlType type) { m_type = type; }
};
