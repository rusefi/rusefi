/**
 * @file    pwm_generator_logic.h
 *
 * @date Mar 2, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "state_sequence.h"
#include "global.h"
#include "scheduler.h"
#include "efi_gpio.h"

#define PERCENT_TO_DUTY(x) (x) * 0.01

#define NAN_FREQUENCY_SLEEP_PERIOD_MS 100

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
typedef void (pwm_gen_callback)(int stateIndex, PwmConfig* pwm);

typedef enum {
	// duty cycle below 1% (ZERO_PWM_THRESHOLD): output held constantly inactive, no pulses generated
	PM_ZERO,
	PM_NORMAL,
	// duty cycle above 99% (FULL_PWM_THRESHOLD): output held constantly active
	PM_FULL
} pwm_mode_e;

/**
 * @brief   Multi-channel software PWM output configuration
 */
class PwmConfig {
public:
	PwmConfig();

	void weComplexInit(
			Scheduler *executor,
			MultiChannelStateSequence const * seq,
			pwm_cycle_callback *pwmCycleCallback,
			pwm_gen_callback *callback);

	Scheduler *m_executor = nullptr;

	/**
	 * Number of times this PWM's executor callback has fired (i.e. how many
	 * times it re-armed itself on the microsecond executor). This is the
	 * soft-PWM load contribution of this channel. Hard-PWM channels never
	 * increment it (they have no executor callback).
	 */
	uint32_t executorFireCount = 0;

	/**
	 * Intrusive self-registering list of all active soft/full-PWM channels
	 * (see weComplexInit). Lets a console command enumerate the running PWM
	 * sources and their load - the executorFireCount distinguishes the
	 * channels even though they all share the static timerCallback address.
	 */
	PwmConfig* m_nextInPwmList = nullptr;

	/**
	 * We need to handle zero duty cycle and 100% duty cycle in a special way
	 */
	pwm_mode_e mode;
	bool isStopRequested = false;

	/**
	 * @param frequency positive frequency in Hz, or NAN to pause PWM: while paused, the timer loop
	 * drives the pin inactive and re-checks every NAN_FREQUENCY_SLEEP_PERIOD_MS until a real
	 * frequency is set again. Zero is NOT a valid way to stop output - it produces an infinite
	 * period and trips an assert in handleCycleStart(); convert anything below 1 Hz to NAN instead.
	 */
	void setFrequency(float frequency);

	void handleCycleStart();
	const char *m_name;

	/** current PWM frequency in Hz (0 if not set / NaN period) */
	float getFrequencyHz() const { return (std::isnan(periodNt) || periodNt <= 0) ? 0.0f : (1000000.0f / NT2US(periodNt)); }

	// todo: 'outputPins' should be extracted away from here since technically one can want PWM scheduler without actual pin output
	OutputPin *outputPins[PWM_PHASE_MAX_WAVE_PER_PWM];
	MultiChannelStateSequence const * multiChannelStateSequence = nullptr;
	efitick_t togglePwmState();
	void stop();
	void applyPwmValue(OutputPin *output, int stateIndex, int channelIndex = 0);

	int dbgNestingLevel;

	scheduling_s scheduling;

	pwm_config_safe_state_s safe;

	/**
	 * this callback is invoked before each wave generation cycle
	 */
	pwm_cycle_callback *m_pwmCycleCallback = nullptr;

	/**
	 * this main callback is invoked when it's time to switch level on any of the output channels
	 */
	pwm_gen_callback *m_stateChangeCallback = nullptr;
private:
	/**
	 * float value of PWM period
	 * PWM generation is not happening while this value is NAN
	 */
	float periodNt;

	// Set if we are very far behind schedule and need to reset back to the beginning of a cycle to find our way
	bool forceCycleStart = true;
};

#include "hardware_pwm.h"

struct IPwm {
	virtual void setSimplePwmDutyCycle(float dutyCycle) = 0;
};

class SimplePwm : public PwmConfig, public IPwm {
public:
	SimplePwm();
	SimplePwm(const char *name);
	void setSimplePwmDutyCycle(float dutyCycle) override;
	/**
	 * Hides PwmConfig::setFrequency(): a SimplePwm backed by hardware PWM (see startSimplePwmHard() and the
	 * gpio-chip path in startSimplePwm()) reprograms the timer instead of the software period.
	 * @return false only when hardware PWM is attached and cannot do that frequency; the old frequency stays.
	 */
	bool setFrequency(float frequency);
	MultiChannelStateSequenceWithData<2> seq;
	hardware_pwm* hardPwm = nullptr;
};

/**
 * default implementation of pwm_gen_callback which simply toggles the pins
 *
 */
void applyPinState(int stateIndex, PwmConfig* state) /* pwm_gen_callback */;

#define PWM_MAX_DUTY 1

/**
 * Start a one-channel software PWM driver.
 *
 * @param dutyCycle value in the [0, 1] range, inclusive. Starting with zero duty cycle is legal and
 * common (e.g. fan and GPPWM outputs start "off"): any value below ZERO_PWM_THRESHOLD (1%) puts the
 * driver into PM_ZERO mode where the pin is held inactive with no pulses, mirrored by PM_FULL above
 * 99%. The timer loop still ticks once per period in these modes, so a later setSimplePwmDutyCycle()
 * call resumes normal pulse generation.
 *
 * @param frequency must be at least 1 Hz - zero included, anything lower is rejected with a warning
 * and the driver is NOT started, leaving the pin untouched and later duty cycle updates inert.
 * A running PWM can be paused with setFrequency(NAN), never with zero.
 *
 * This method should be called after scheduling layer is started by initSignalExecutor()
 */
void startSimplePwm(SimplePwm *state, const char *msg,
		Scheduler *executor,
		OutputPin *output,
		float frequency, float dutyCycle, pwm_gen_callback *callback = NULL);

/**
 * initialize GPIO pin and start a one-channel software PWM driver.
 *
 * This method should be called after scheduling layer is started by initSignalExecutor()
 */
void startSimplePwmExt(SimplePwm *state,
		const char *msg,
		Scheduler *executor,
		brain_pin_e brainPin, OutputPin *output,
		float frequency, float dutyCycle, pwm_gen_callback *callback = NULL);

void startSimplePwmHard(SimplePwm *state, const char *msg,
		Scheduler *executor,
		brain_pin_e brainPin, OutputPin *output, float frequency,
		float dutyCycle);

void copyPwmParameters(PwmConfig *state, MultiChannelStateSequence const * seq);

/**
 * Print a table of all active PWM channels (name, hard/soft, frequency,
 * cumulative executor fire count / event rate) - the lockstats addition that
 * names which soft-PWM channel is loading the microsecond executor.
 */
void printPwmStats();

