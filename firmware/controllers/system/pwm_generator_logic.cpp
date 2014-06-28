/**
 * @file    pwm_generator_logic.cpp
 *
 * This PWM implementation keep track of when it would be the next time to toggle the signal.
 * It constantly sets timer to that next toggle time, then sets the timer again from the callback, and so on.
 *
 * @date Mar 2, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "pwm_generator_logic.h"

/**
 * We need to limit the number of iterations in order to avoid precision loss while calculating
 * next toggle time
 */
#define ITERATION_LIMIT 10000

SimplePwm::SimplePwm() {
	wave.init(pinStates);
	sr[0] = wave;
	init(_switchTimes, sr);
}

PwmConfig::PwmConfig() {
	scheduling.name = "PwmConfig";
}

PwmConfig::PwmConfig(float *st, single_wave_s *waves) {
	multiWave.init(st, waves);
	scheduling.name = "PwmConfig";
}

void PwmConfig::init(float *st, single_wave_s *waves) {
	multiWave.init(st, waves);
}

/**
 * @param dutyCycle value between 0 and 1
 */
void SimplePwm::setSimplePwmDutyCycle(float dutyCycle) {
	multiWave.setSwitchTime(0, dutyCycle);
}

static uint64_t getNextSwitchTimeUs(PwmConfig *state) {
	efiAssert(state->safe.phaseIndex < PWM_PHASE_MAX_COUNT, "phaseIndex range", 0);
	int iteration = state->safe.iteration;
	float switchTime = state->multiWave.switchTimes[state->safe.phaseIndex];
	float periodMs = state->safe.periodMs;
#if DEBUG_PWM
	scheduleMsg(&logger, "iteration=%d switchTime=%f period=%f", iteration, switchTime, period);
#endif

	/**
	 * todo: once 'iteration' gets relatively high, we might lose calculation precision here
	 * todo: double-check this spot
	 */
	uint64_t timeToSwitchUs = (iteration + switchTime) * periodMs * 1000;

#if DEBUG_PWM
	scheduleMsg(&logger, "start=%d timeToSwitch=%d", state->safe.start, timeToSwitch);
#endif
	return state->safe.startUs + timeToSwitchUs;
}

/**
 * @return Next time for signal toggle
 */
static uint64_t togglePwmState(PwmConfig *state) {
#if DEBUG_PWM
	scheduleMsg(&logger, "togglePwmState phaseIndex=%d iteration=%d", state->safe.phaseIndex, state->safe.iteration);
	scheduleMsg(&logger, "state->period=%f state->safe.period=%f", state->period, state->safe.period);
#endif

	if (state->safe.phaseIndex == 0) {
		if (cisnan(state->periodMs)) {
			/**
			 * zero period means PWM is paused
			 */
			return 1;
		}
		if (state->cycleCallback != NULL)
			state->cycleCallback(state);
		efiAssert(state->periodMs != 0, "period not initialized", 0);
		if (state->safe.periodMs != state->periodMs || state->safe.iteration == ITERATION_LIMIT) {
			/**
			 * period length has changed - we need to reset internal state
			 */
			state->safe.startUs = getTimeNowUs();
			state->safe.iteration = 0;
			state->safe.periodMs = state->periodMs;
#if DEBUG_PWM
			scheduleMsg(&logger, "state reset start=%d iteration=%d", state->safe.start, state->safe.iteration);
#endif
		}
	}

	state->stateChangeCallback(state,
			state->safe.phaseIndex == 0 ? state->phaseCount - 1 : state->safe.phaseIndex - 1);

	uint64_t nextSwitchTimeUs = getNextSwitchTimeUs(state);
#if DEBUG_PWM
	scheduleMsg(&logger, "%s: nextSwitchTime %d", state->name, nextSwitchTime);
#endif
	// signed value is needed here
	int64_t timeToSwitch = nextSwitchTimeUs - getTimeNowUs();
	if (timeToSwitch < 1) {
//todo: introduce error and test this error handling		warning(OBD_PCM_Processor_Fault, "PWM: negative switch time");
		timeToSwitch = 1000;
	}

	state->safe.phaseIndex++;
	if (state->safe.phaseIndex == state->phaseCount) {
		state->safe.phaseIndex = 0; // restart
		state->safe.iteration++;
	}
	return timeToSwitch;
}

static void timerCallback(PwmConfig *state) {
	time_t timeToSleepUs = togglePwmState(state);
	scheduleTask(&state->scheduling, timeToSleepUs, (schfunc_t) timerCallback, state);
}

/**
 * Incoming parameters are potentially just values on current stack, so we have to copy
 * into our own permanent storage, right?
 */
void copyPwmParameters(PwmConfig *state, int phaseCount, float *switchTimes, int waveCount, int **pinStates) {
	state->phaseCount = phaseCount;

	for (int phaseIndex = 0; phaseIndex < phaseCount; phaseIndex++) {
		state->multiWave.switchTimes[phaseIndex] = switchTimes[phaseIndex];

		for (int waveIndex = 0; waveIndex < waveCount; waveIndex++) {
//			print("output switch time index (%d/%d) at %f to %d\r\n", phaseIndex,waveIndex,
//					switchTimes[phaseIndex], pinStates[waveIndex][phaseIndex]);
			state->multiWave.waves[waveIndex].pinStates[phaseIndex] = pinStates[waveIndex][phaseIndex];
		}
	}
}

void weComplexInit(const char *msg, PwmConfig *state, int phaseCount, float *switchTimes, int waveCount,
		int **pinStates, pwm_cycle_callback *cycleCallback, pwm_gen_callback *stateChangeCallback) {

	efiAssertVoid(state->periodMs != 0, "period is not initialized");
	if (phaseCount == 0) {
		firmwareError("signal length cannot be zero");
		return;
	}
	if (phaseCount > PWM_PHASE_MAX_COUNT) {
		firmwareError("too many phases in PWM");
		return;
	}
	if (switchTimes[phaseCount - 1] != 1) {
		firmwareError("last switch time has to be 1");
		return;
	}
	efiAssertVoid(waveCount > 0, "waveCount should be positive");
	checkSwitchTimes2(phaseCount, switchTimes);

	state->multiWave.waveCount = waveCount;

	copyPwmParameters(state, phaseCount, switchTimes, waveCount, pinStates);

	state->cycleCallback = cycleCallback;
	state->stateChangeCallback = stateChangeCallback;

	state->safe.phaseIndex = 0;
	state->safe.periodMs = -1;
	state->safe.iteration = -1;
	state->name = msg;

	timerCallback(state);
}
