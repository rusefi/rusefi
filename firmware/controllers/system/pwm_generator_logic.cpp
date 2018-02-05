/**
 * @file    pwm_generator_logic.cpp
 *
 * This PWM implementation keep track of when it would be the next time to toggle the signal.
 * It constantly sets timer to that next toggle time, then sets the timer again from the callback, and so on.
 *
 * @date Mar 2, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"
#include "pwm_generator_logic.h"

/**
 * We need to limit the number of iterations in order to avoid precision loss while calculating
 * next toggle time
 */
#define ITERATION_LIMIT 1000

SimplePwm::SimplePwm() {
	waveInstance.init(pinStates);
	sr[0] = waveInstance;
	init(_switchTimes, sr);
}

void PwmConfig::baseConstructor() {
	memset(&scheduling, 0, sizeof(scheduling));
	memset(&safe, 0, sizeof(safe));
	dbgNestingLevel = 0;
	periodNt = NAN;
	memset(&outputPins, 0, sizeof(outputPins));
	phaseCount = 0;
	pwmCycleCallback = NULL;
	stateChangeCallback = NULL;
}

PwmConfig::PwmConfig() {
	baseConstructor();
}

PwmConfig::PwmConfig(float *st, single_wave_s *waves) {
	baseConstructor();
	multiWave.init(st, waves);
}

void PwmConfig::init(float *st, single_wave_s *waves) {
	multiWave.init(st, waves);
}

/**
 * This method allows you to change duty cycle on the fly
 * @param dutyCycle value between 0 and 1
 */
void SimplePwm::setSimplePwmDutyCycle(float dutyCycle) {
	if (dutyCycle < 0 || dutyCycle > 1) {
		firmwareError(CUSTOM_ERR_ASSERT_VOID, "spwd:dutyCycle %.2f", dutyCycle);
	}
	multiWave.setSwitchTime(0, dutyCycle);
}

static efitimeus_t getNextSwitchTimeUs(PwmConfig *state) {
	efiAssert(state->safe.phaseIndex < PWM_PHASE_MAX_COUNT, "phaseIndex range", 0);
	int iteration = state->safe.iteration;
	float switchTime = state->multiWave.getSwitchTime(state->safe.phaseIndex);
	float periodNt = state->safe.periodNt;
#if DEBUG_PWM
	scheduleMsg(&logger, "iteration=%d switchTime=%.2f period=%.2f", iteration, switchTime, period);
#endif

	/**
	 * Once 'iteration' gets relatively high, we might lose calculation precision here.
	 * This is addressed by ITERATION_LIMIT
	 */
	efitime_t timeToSwitchNt = (efitime_t) ((iteration + switchTime) * periodNt);

#if DEBUG_PWM
	scheduleMsg(&logger, "start=%d timeToSwitch=%d", state->safe.start, timeToSwitch);
#endif
	return NT2US(state->safe.startNt + timeToSwitchNt);
}

void PwmConfig::setFrequency(float frequency) {
	if (cisnan(frequency)) {
		// explicit code just to be sure
		periodNt = NAN;
		return;
	}
	/**
	 * see #handleCycleStart()
	 */
	periodNt = US2NT(frequency2periodUs(frequency));
}

void PwmConfig::handleCycleStart() {
	if (safe.phaseIndex == 0) {
		if (pwmCycleCallback != NULL) {
			pwmCycleCallback(this);
		}
		efiAssertVoid(periodNt != 0, "period not initialized");
		if (safe.periodNt != periodNt || safe.iteration == ITERATION_LIMIT) {
			/**
			 * period length has changed - we need to reset internal state
			 */
			safe.startNt = getTimeNowNt();
			safe.iteration = 0;
			safe.periodNt = periodNt;
#if DEBUG_PWM
			scheduleMsg(&logger, "state reset start=%d iteration=%d", state->safe.start, state->safe.iteration);
#endif
		}
	}
}

/**
 * @return Next time for signal toggle
 */
efitimeus_t PwmConfig::togglePwmState() {
#if DEBUG_PWM
	scheduleMsg(&logger, "togglePwmState phaseIndex=%d iteration=%d", safe.phaseIndex, safe.iteration);
	scheduleMsg(&logger, "period=%.2f safe.period=%.2f", period, safe.period);
#endif

	if (cisnan(periodNt)) {
		/**
		 * NaN period means PWM is paused
		 */
		return getTimeNowUs() + MS2US(100);
	}

	handleCycleStart();

	/**
	 * Here is where the 'business logic' - the actual pin state change is happening
	 */
	// callback state index is offset by one. todo: why? can we simplify this?
	int cbStateIndex = safe.phaseIndex == 0 ? phaseCount - 1 : safe.phaseIndex - 1;
	stateChangeCallback(this, cbStateIndex);

	efitimeus_t nextSwitchTimeUs = getNextSwitchTimeUs(this);
#if DEBUG_PWM
	scheduleMsg(&logger, "%s: nextSwitchTime %d", state->name, nextSwitchTime);
#endif /* DEBUG_PWM */
	// signed value is needed here
//	int64_t timeToSwitch = nextSwitchTimeUs - getTimeNowUs();
//	if (timeToSwitch < 1) {
//		/**
//		 * We are here if we are late for a state transition.
//		 * At 12000RPM=200Hz with a 60 toothed wheel we need to change state every
//		 * 1000000 / 200 / 120 = ~41 uS. We are kind of OK.
//		 *
//		 * We are also here after a flash write. Flash write freezes the whole chip for a couple of seconds,
//		 * so PWM generation and trigger simulation generation would have to recover from this time lag.
//		 */
//		//todo: introduce error and test this error handling		warning(OBD_PCM_Processor_Fault, "PWM: negative switch time");
//		timeToSwitch = 10;
//	}

	safe.phaseIndex++;
	if (safe.phaseIndex == phaseCount) {
		safe.phaseIndex = 0; // restart
		safe.iteration++;
	}
	return nextSwitchTimeUs;
}

/**
 * Main PWM loop: toggle pin & schedule next invocation
 */
static void timerCallback(PwmConfig *state) {
	state->dbgNestingLevel++;
	efiAssertVoid(state->dbgNestingLevel < 25, "PWM nesting issue");

	efitimeus_t switchTimeUs = state->togglePwmState();
	scheduleByTimestamp(&state->scheduling, switchTimeUs, (schfunc_t) timerCallback, state);
	state->dbgNestingLevel--;
}

/**
 * Incoming parameters are potentially just values on current stack, so we have to copy
 * into our own permanent storage, right?
 */
void copyPwmParameters(PwmConfig *state, int phaseCount, float *switchTimes, int waveCount, pin_state_t **pinStates) {
	state->phaseCount = phaseCount;

	for (int phaseIndex = 0; phaseIndex < phaseCount; phaseIndex++) {
		state->multiWave.setSwitchTime(phaseIndex, switchTimes[phaseIndex]);

		for (int waveIndex = 0; waveIndex < waveCount; waveIndex++) {
//			print("output switch time index (%d/%d) at %.2f to %d\r\n", phaseIndex,waveIndex,
//					switchTimes[phaseIndex], pinStates[waveIndex][phaseIndex]);
			state->multiWave.waves[waveIndex].pinStates[phaseIndex] = pinStates[waveIndex][phaseIndex];
		}
	}
}

void PwmConfig::weComplexInit(const char *msg, int phaseCount, float *switchTimes, int waveCount,
		pin_state_t **pinStates, pwm_cycle_callback *pwmCycleCallback, pwm_gen_callback *stateChangeCallback) {

	efiAssertVoid(periodNt != 0, "period is not initialized");
	if (phaseCount == 0) {
		firmwareError(CUSTOM_ERR_PWM_1, "signal length cannot be zero");
		return;
	}
	if (phaseCount > PWM_PHASE_MAX_COUNT) {
		firmwareError(CUSTOM_ERR_PWM_2, "too many phases in PWM");
		return;
	}
	efiAssertVoid(waveCount > 0, "waveCount should be positive");
	checkSwitchTimes2(phaseCount, switchTimes);

	this->pwmCycleCallback = pwmCycleCallback;
	this->stateChangeCallback = stateChangeCallback;

	multiWave.waveCount = waveCount;

	copyPwmParameters(this, phaseCount, switchTimes, waveCount, pinStates);

	safe.phaseIndex = 0;
	safe.periodNt = -1;
	safe.iteration = -1;

	// let's start the indefinite callback loop of PWM generation
	timerCallback(this);
}
