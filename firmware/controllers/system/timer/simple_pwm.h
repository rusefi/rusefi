#pragma once

#include "efitime.h"
#include "scheduler.h"

class OutputPin;

class SimplePwm {
public:
	SimplePwm();
	SimplePwm(const char* name);

	void init(ExecutorInterface* executor, OutputPin* pin, float frequency, float dutyCycle);

	virtual void setSimplePwmDutyCycle(float dutyCycle);
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
	ExecutorInterface* m_executor;
	OutputPin* m_pin;

	uint32_t m_lowTime;
	uint32_t m_highTime;

	float m_duty = 0;
	float m_frequency = 100;

	scheduling_s m_sched;
};
