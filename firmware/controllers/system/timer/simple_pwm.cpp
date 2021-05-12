
#include "simple_pwm.h"
#include "efi_gpio.h"
#include "perf_trace.h"

SimplePwm::SimplePwm(const char* name)
	: m_name(name)
{
}

void SimplePwm::setSimplePwmDutyCycle(float duty) {
	/*if (isStopRequested) {
		// we are here in order to not change pin once PWM stop was requested
		return;
	}*/

	if (cisnan(duty)) {
		//warning(CUSTOM_DUTY_INVALID, "spwd:dutyCycle %.2f", dutyCycle);
		return;
	} else if (duty < 0) {
		//warning(CUSTOM_DUTY_TOO_LOW, "spwd:dutyCycle %.2f", dutyCycle);
		duty = 0;
	} else if (duty > 1) {
		//warning(CUSTOM_DUTY_TOO_HIGH, "spwd:dutyCycle %.2f", dutyCycle);
		duty = 1;
	}

	m_duty = duty;

	update();
}

void SimplePwm::setFrequency(float hz) {
	if (cisnan(hz)) {
		return;
	}

	// Don't allow really low frequencies
	if (hz < 1.0f) {
		hz = 1.0f;
	}

	m_frequency = hz;

	update();
}

void SimplePwm::update() {
	floatus_t period = 1e6 / m_frequency;

	efitick_t periodNt = US2NT(period);

	auto duty = m_duty;

	if (duty > 0.99f) {
		m_highTime = periodNt;
		m_lowTime = 0;
	} else if (duty < 0.01f) {
		m_highTime = 0;
		m_lowTime = periodNt;
	} else {
		floatus_t highTime = period * duty;

		m_highTime = US2NT(highTime);
		m_lowTime = periodNt - m_highTime;
	}
}

/*static*/ void SimplePwm::setLowAdapter(SimplePwm* pwm) {
	pwm->setLow();
}

/*static*/ void SimplePwm::setHighAdapter(SimplePwm* pwm) {
	pwm->setHigh();
}

void SimplePwm::scheduleFall() {
	// 0 low time -> 100% duty, so don't schedule the fall.  Reschedule another rise in a full period.
	action_s action = m_lowTime == 0
		? action_s(SimplePwm::setHighAdapter, this)
		: action_s(SimplePwm::setLowAdapter, this);

	m_executor->scheduleByTimestampNt(&m_sched, getTimeNowNt() + m_highTime, action);
}

void SimplePwm::scheduleRise() {
	// 0 high time -> 0% duty, so don't schedule the rise.  Reschedule another fall in a full period.
	action_s action = m_highTime == 0
		? action_s(SimplePwm::setLowAdapter, this)
		: action_s(SimplePwm::setHighAdapter, this);

	m_executor->scheduleByTimestampNt(&m_sched, getTimeNowNt() + m_lowTime, action);
}

// Callback on rising edge
void SimplePwm::setHigh() {
	ScopePerf perf(PE::Temporary1);

	m_pin->setValue(true);
	scheduleFall();
}

// Callback on falling edge
void SimplePwm::setLow() {
	ScopePerf perf(PE::Temporary2);

	m_pin->setValue(false);
	scheduleRise();
}

bool SimplePwm::initHard(brain_pin_e pin, float frequency, float duty) {
	m_hardPwm = hardware_pwm::tryInitPin(m_name, pin, frequency, duty);

	return m_hardPwm;
}

void SimplePwm::init(ExecutorInterface* executor, OutputPin* pin, float frequency, float duty) {
	m_executor = executor;
	m_pin = pin;
	m_frequency = frequency;
	m_duty = duty;

	update();
	scheduleRise();
}
