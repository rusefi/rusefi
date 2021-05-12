#pragma once

#include "efitime.h"
#include "scheduler.h"

class OutputPin;
struct hardware_pwm;

class SimplePwm {
public:
	SimplePwm() = default;
	explicit SimplePwm(const char* name);

	bool initHard(brain_pin_e pin, float frequency, float duty);
	void init(ExecutorInterface* executor, OutputPin* pin, float frequency, float duty);

	virtual void setSimplePwmDutyCycle(float duty);
	virtual void setFrequency(float hz);

private:
	void update();

	void setLow();
	void setHigh();

	static void setLowAdapter(SimplePwm* pwm);
	static void setHighAdapter(SimplePwm* pwm);

	void scheduleRise();
	void scheduleFall();

private:
	const char* const m_name = "pwm";
	ExecutorInterface* m_executor;
	OutputPin* m_pin;

#if EFI_PROD_CODE && HAL_USE_PWM
	hardware_pwm* m_hardPwm = nullptr;
#endif

	uint32_t m_lowTime;
	uint32_t m_highTime;

	float m_duty = 0;
	float m_frequency = 100;

	scheduling_s m_sched;
};
