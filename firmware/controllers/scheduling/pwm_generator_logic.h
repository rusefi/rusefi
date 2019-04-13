/**
 * @file    pwm_generator_logic.h
 *
 * @date Mar 2, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef PWM_GENERATOR_LOGIC_H_
#define PWM_GENERATOR_LOGIC_H_

#include "global.h"
#include "efi_wave.h"
#include "scheduler.h"
#include "efi_gpio.h"

#define NAN_FREQUENCY_SLEEP_PERIOD_MS 100

// 99% duty cycle
#define FULL_PWM_THRESHOLD 0.99

typedef struct {
	/**
	 * a copy so that all phases are executed on the same period, even if another thread
	 * would be adjusting PWM parameters
	 */
	float periodNt;
	/**
	 * Iteration counter
	 */
	int iteration;
	/**
	 * Start time of current iteration
	 */
	efitick_t startNt;
	int phaseIndex;
} pwm_config_safe_state_s;

class PwmConfig;

typedef void (pwm_cycle_callback)(PwmConfig *state);
typedef void (pwm_gen_callback)(int stateIndex, void *arg);

typedef enum {
	PM_ZERO,
	PM_NORMAL,
	PM_FULL
} pwm_mode_e;

/**
 * @brief   Multi-channel software PWM output configuration
 */
class PwmConfig {
public:
	PwmConfig();
	PwmConfig(float *switchTimes, SingleWave *waves);
	void init(float *switchTimes, SingleWave *waves);
	void *arg = NULL;

	void weComplexInit(const char *msg,
			ExecutorInterface *executor,
			const int phaseCount, float const *swithcTimes, const int waveCount, pin_state_t *const*pinStates,
			pwm_cycle_callback *pwmCycleCallback,
			pwm_gen_callback *callback);

	ExecutorInterface *executor;

	/**
	 * We need to handle zero duty cycle and 100% duty cycle in a special way
	 */
	pwm_mode_e mode;

	/**
	 * @param use NAN frequency to pause PWM
	 */
	void setFrequency(float frequency);

	void handleCycleStart();
	const char *name;

	// todo: 'outputPins' should be extracted away from here since technically one can want PWM scheduler without actual pin output
	OutputPin *outputPins[PWM_PHASE_MAX_WAVE_PER_PWM];
	MultiWave multiWave;
	efitimeus_t togglePwmState();

	int dbgNestingLevel;

	scheduling_s scheduling;

	pwm_config_safe_state_s safe;
	/**
	 * Number of events in the cycle
	 */
	int phaseCount;

	/**
	 * this callback is invoked before each wave generation cycle
	 */
	pwm_cycle_callback *pwmCycleCallback;

	/**
	 * this main callback is invoked when it's time to switch level on any of the output channels
	 */
	pwm_gen_callback *stateChangeCallback = NULL;
private:
	/**
	 * float value of PWM period
	 * PWM generation is not happening while this value is NAN
	 */
	float periodNt;
};


class SimplePwm : public PwmConfig {
public:
	SimplePwm();
	SimplePwm(const char *name);
	void setSimplePwmDutyCycle(float dutyCycle);
	pin_state_t pinStates[2];
	SingleWave sr[1];
	float _switchTimes[2];
private:
	SingleWave waveInstance;
};

/**
 * Start a one-channel software PWM driver.
 *
 * This method should be called after scheduling layer is started by initSignalExecutor()
 */
void startSimplePwm(SimplePwm *state, const char *msg,
		ExecutorInterface *executor,
		OutputPin *output,
		float dutyCycle, float frequency, pwm_gen_callback *stateChangeCallback);

/**
 * initialize GPIO pin and start a one-channel software PWM driver.
 *
 * This method should be called after scheduling layer is started by initSignalExecutor()
 */
void startSimplePwmExt(SimplePwm *state,
		const char *msg,
		ExecutorInterface *executor,
		brain_pin_e brainPin, OutputPin *output,
		float frequency, float dutyCycle, pwm_gen_callback *stateChangeCallback);

void copyPwmParameters(PwmConfig *state, int phaseCount, float const *switchTimes,
		int waveCount, pin_state_t *const *pinStates);

#endif /* PWM_GENERATOR_LOGIC_H_ */
