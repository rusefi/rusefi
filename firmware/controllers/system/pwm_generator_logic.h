/**
 * @file    pwm_generator_logic.h
 *
 * @date Mar 2, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef PWM_GENERATOR_LOGIC_H_
#define PWM_GENERATOR_LOGIC_H_

#include "global.h"
#include "EfiWave.h"
#include "scheduler.h"
#include "efiGpio.h"

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
typedef void (pwm_gen_callback)(PwmConfig *state, int stateIndex);

/**
 * @brief   Multi-channel software PWM output configuration
 */
class PwmConfig {
public:
	PwmConfig();
	PwmConfig(float *switchTimes, single_wave_s *waves);
	void baseConstructor();
	void init(float *switchTimes, single_wave_s *waves);

	void weComplexInit(const char *msg,
			int phaseCount, float *swithcTimes, int waveCount, pin_state_t **pinStates,
			pwm_cycle_callback *pwmCycleCallback,
			pwm_gen_callback *callback);

	/**
	 * @param use NAN frequency to pause PWM
	 */
	void setFrequency(float frequency);

	void handleCycleStart();


	OutputPin *outputPins[PWM_PHASE_MAX_WAVE_PER_PWM];
	multi_wave_s multiWave;
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
	pwm_gen_callback *stateChangeCallback;
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
	void setSimplePwmDutyCycle(float dutyCycle);
	pin_state_t pinStates[2];
	single_wave_s sr[1];
	float _switchTimes[2];
private:
	single_wave_s waveInstance;
};

void copyPwmParameters(PwmConfig *state, int phaseCount, float *switchTimes,
		int waveCount, pin_state_t **pinStates);

#endif /* PWM_GENERATOR_LOGIC_H_ */
