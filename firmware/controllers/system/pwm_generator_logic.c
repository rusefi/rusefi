/*
 * @file    pwm_generator_logic.c
 *
 * @date Mar 2, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "pwm_generator_logic.h"
#include "engine_math.h"

static time_t getNextSwitchTime(PwmConfig *state) {
	chDbgAssert(state->safe.phaseIndex < PWM_PHASE_MAX_COUNT, "phaseIndex range", NULL);
	int iteration = state->safe.iteration;
	float switchTime = state->multiWave.switchTimes[state->safe.phaseIndex];
	float period = state->safe.period;
#if DEBUG_PWM
	scheduleMsg(&logger, "iteration=%d switchTime=%f period=%f", iteration, switchTime, period);
#endif

	systime_t timeToSwitch = (systime_t) ((iteration + switchTime) * period);

#if DEBUG_PWM
	scheduleMsg(&logger, "start=%d timeToSwitch=%d", state->safe.start, timeToSwitch);
#endif
	return state->safe.start + timeToSwitch;
}

static time_t togglePwmState(PwmConfig *state) {
#if DEBUG_PWM
	scheduleMsg(&logger, "togglePwmState phaseIndex=%d iteration=%d", state->safe.phaseIndex, state->safe.iteration);
	scheduleMsg(&logger, "state->period=%f state->safe.period=%f", state->period, state->safe.period);
#endif

	if (state->safe.phaseIndex == 0) {
		if (cisnan(state->period)) {
			/**
			 * zero period means PWM is paused
			 */
			return TICKS_IN_MS;
		}
		if (state->cycleCallback != NULL )
			state->cycleCallback(state);
		chDbgAssert(state->period != 0, "period not initialized", NULL);
		if (state->safe.period != state->period) {
			/**
			 * period length has changed - we need to reset internal state
			 */
			state->safe.start = chTimeNow();
			state->safe.iteration = 0;
			state->safe.period = state->period;
#if DEBUG_PWM
			scheduleMsg(&logger, "state reset start=%d iteration=%d", state->safe.start, state->safe.iteration);
#endif
		}
	}

	state->stateChangeCallback(state,
			state->safe.phaseIndex == 0 ? state->multiWave.phaseCount - 1 : state->safe.phaseIndex - 1);

	time_t nextSwitchTime = getNextSwitchTime(state);
#if DEBUG_PWM
	scheduleMsg(&logger, "%s: nextSwitchTime %d", state->name, nextSwitchTime);
#endif
	time_t timeToSwitch = nextSwitchTime - chTimeNow();

	state->safe.phaseIndex++;
	if (state->safe.phaseIndex == state->multiWave.phaseCount) {
		state->safe.phaseIndex = 0; // restart
		state->safe.iteration++;
	}
	return timeToSwitch;
}

static void timerCallback(PwmConfig *state) {
	// todo: use this implementation! but something is wrong with it :(
	time_t timeToSleep = togglePwmState(state);
	scheduleTask(&state->scheduling, timeToSleep, (schfunc_t) timerCallback, state);
}

static msg_t deThread(PwmConfig *state) {
	chRegSetThreadName("Wave");

#if DEBUG_PWM
	scheduleMsg(&logger, "Thread started for %s", state->name);
#endif

//	setPadValue(state, state->idleState); todo: currently pin is always zero at first iteration.
// we can live with that for now
	// todo: figure out overflow

	while (TRUE) {
		time_t timeToSwitch = togglePwmState(state);
#if DEBUG_PWM
		scheduleMsg(&logger, "%s: sleep %d", state->name, timeToSwitch);
#endif
		chThdSleep(timeToSwitch);
	}
#if defined __GNUC__
	return -1;
#endif
}

/**
 * Incoming parameters are potentially just values on current stack, so we have to copy
 * into our own permanent storage, right?
 */
void copyPwmParameters(PwmConfig *state, int phaseCount, float *switchTimes, int waveCount, int **pinStates) {
	state->multiWave.phaseCount = phaseCount;

	for (int phaseIndex = 0; phaseIndex < phaseCount; phaseIndex++) {
		state->multiWave.switchTimes[phaseIndex] = switchTimes[phaseIndex];

		for (int waveIndex = 0; waveIndex < waveCount; waveIndex++) {
//			print("output switch time index (%d/%d) at %f to %d\r\n", phaseIndex,waveIndex,
//					switchTimes[phaseIndex], pinStates[waveIndex][phaseIndex]);
			state->multiWave.waves[waveIndex].pinStates[phaseIndex] = pinStates[waveIndex][phaseIndex];
		}
	}
}

void weComplexInit(char *msg, PwmConfig *state, int phaseCount, float *switchTimes, int waveCount, int **pinStates,
		pwm_cycle_callback *cycleCallback, pwm_gen_callback *stateChangeCallback) {

	chDbgCheck(state->period != 0, "period is not initialized");
	chDbgCheck(phaseCount > 1, "count is too small");
	chDbgCheck(phaseCount <= PWM_PHASE_MAX_COUNT, "count is too large");
	chDbgCheck(switchTimes[phaseCount - 1] == 1, "last switch time has to be 1");
	chDbgCheck(waveCount > 0, "waveCount should be positive");
	checkSwitchTimes(phaseCount, switchTimes);

	state->multiWave.waveCount = waveCount;

	copyPwmParameters(state, phaseCount, switchTimes, waveCount, pinStates);

	state->cycleCallback = cycleCallback;
	state->stateChangeCallback = stateChangeCallback;

	state->safe.phaseIndex = 0;
	state->safe.period = -1;
	state->safe.iteration = -1;
	state->name = msg;
	chThdCreateStatic(state->deThreadStack, sizeof(state->deThreadStack), NORMALPRIO, (tfunc_t) deThread, state);

//	timerCallback(state);

}
