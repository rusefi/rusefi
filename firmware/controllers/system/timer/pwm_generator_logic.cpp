/**
 * @file    pwm_generator_logic.cpp
 *
 * This PWM implementation keep track of when it would be the next time to toggle the signal.
 * It constantly sets timer to that next toggle time, then sets the timer again from the callback, and so on.
 *
 * @date Mar 2, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "os_access.h"
#include "perf_trace.h"

#if EFI_PROD_CODE
#include "mpu_util.h"
#endif // EFI_PROD_CODE

// 1% duty cycle
#define ZERO_PWM_THRESHOLD 0.01

SimplePwm::SimplePwm() {
	waveInstance.init(pinStates);
	sr[0] = waveInstance;
	init(_switchTimes, sr);
}

SimplePwm::SimplePwm(const char *name) : SimplePwm()  {
	this->name = name;
}

PwmConfig::PwmConfig() {
	memset((void*)&scheduling, 0, sizeof(scheduling));
	memset((void*)&safe, 0, sizeof(safe));
	dbgNestingLevel = 0;
	periodNt = NAN;
	mode = PM_NORMAL;
	memset(&outputPins, 0, sizeof(outputPins));
	phaseCount = 0;
	pwmCycleCallback = nullptr;
	stateChangeCallback = nullptr;
	executor = nullptr;
	name = "[noname]";
	arg = this;
}

PwmConfig::PwmConfig(float *st, SingleChannelStateSequence *waves) : PwmConfig() {
	multiChannelStateSequence.init(st, waves);
}

void PwmConfig::init(float *st, SingleChannelStateSequence *waves) {
	multiChannelStateSequence.init(st, waves);
}

/**
 * This method allows you to change duty cycle on the fly
 * @param dutyCycle value between 0 and 1
 * See also setFrequency
 */
void SimplePwm::setSimplePwmDutyCycle(float dutyCycle) {
	if (isStopRequested) {
		// we are here in order to not change pin once PWM stop was requested
		return;
	}
	if (cisnan(dutyCycle)) {
		warning(CUSTOM_DUTY_INVALID, "%s spwd:dutyCycle %.2f", name, dutyCycle);
		return;
	} else if (dutyCycle < 0) {
		warning(CUSTOM_DUTY_TOO_LOW, "%s dutyCycle %.2f", name, dutyCycle);
		dutyCycle = 0;
	} else if (dutyCycle > 1) {
		warning(CUSTOM_PWM_DUTY_TOO_HIGH, "%s duty %.2f", name, dutyCycle);
		dutyCycle = 1;
	}

#if EFI_PROD_CODE
	if (hardPwm) {
		hardPwm->setDuty(dutyCycle);
		return;
	}
#endif

	// Handle zero and full duty cycle.  This will cause the PWM output to behave like a plain digital output.
	if (dutyCycle == 0.0f && stateChangeCallback) {
		// Manually fire falling edge
		stateChangeCallback(0, arg);
	} else if (dutyCycle == 1.0f && stateChangeCallback) {
		// Manually fire rising edge
		stateChangeCallback(1, arg);
	}

	if (dutyCycle < ZERO_PWM_THRESHOLD) {
		mode = PM_ZERO;
	} else if (dutyCycle > FULL_PWM_THRESHOLD) {
		mode = PM_FULL;
	} else {
		mode = PM_NORMAL;
		multiChannelStateSequence.setSwitchTime(0, dutyCycle);
	}
}

/**
 * returns absolute timestamp of state change
 */
static efitick_t getNextSwitchTimeNt(PwmConfig *state) {
	efiAssert(CUSTOM_ERR_ASSERT, state->safe.phaseIndex < PWM_PHASE_MAX_COUNT, "phaseIndex range", 0);
	int iteration = state->safe.iteration;
	// we handle PM_ZERO and PM_FULL separately
	float switchTime = state->mode == PM_NORMAL ? state->multiChannelStateSequence.getSwitchTime(state->safe.phaseIndex) : 1;
	float periodNt = state->safe.periodNt;
#if DEBUG_PWM
	efiPrintf("iteration=%d switchTime=%.2f period=%.2f", iteration, switchTime, period);
#endif /* DEBUG_PWM */

	/**
	 * Once 'iteration' gets relatively high, we might lose calculation precision here.
	 * This is addressed by iterationLimit below, using any many cycles as possible without overflowing timeToSwitchNt
	 */
	uint32_t timeToSwitchNt = (uint32_t)((iteration + switchTime) * periodNt);

#if DEBUG_PWM
	efiPrintf("start=%d timeToSwitch=%d", state->safe.start, timeToSwitch);
#endif /* DEBUG_PWM */
	return state->safe.startNt + timeToSwitchNt;
}

void PwmConfig::setFrequency(float frequency) {
	if (cisnan(frequency)) {
		// explicit code just to be sure
		periodNt = NAN;
		return;
	}
	/**
	 * see #handleCycleStart()
	 * 'periodNt' is below 10 seconds here so we use 32 bit type for performance reasons
	 */
	periodNt = USF2NT(frequency2periodUs(frequency));
}

void PwmConfig::stop() {
	isStopRequested = true;
}

void PwmConfig::handleCycleStart() {
	if (safe.phaseIndex != 0) {
		// https://github.com/rusefi/rusefi/issues/1030
		firmwareError(CUSTOM_PWM_CYCLE_START, "handleCycleStart %d", safe.phaseIndex);
		return;
	}

	if (pwmCycleCallback != NULL) {
		pwmCycleCallback(this);
	}
		// Compute the maximum number of iterations without overflowing a uint32_t worth of timestamp
		uint32_t iterationLimit = (0xFFFFFFFF / periodNt) - 2;

		efiAssertVoid(CUSTOM_ERR_6580, periodNt != 0, "period not initialized");
		efiAssertVoid(CUSTOM_ERR_6580, iterationLimit > 0, "iterationLimit invalid");
		if (forceCycleStart || safe.periodNt != periodNt || safe.iteration == iterationLimit) {
			/**
			 * period length has changed - we need to reset internal state
			 */
			safe.startNt = getTimeNowNt();
			safe.iteration = 0;
			safe.periodNt = periodNt;

			forceCycleStart = false;
#if DEBUG_PWM
			efiPrintf("state reset start=%d iteration=%d", state->safe.start, state->safe.iteration);
#endif
		}
}

/**
 * @return Next time for signal toggle
 */
efitick_t PwmConfig::togglePwmState() {
	if (isStopRequested) {
		return 0;
	}

#if DEBUG_PWM
	efiPrintf("togglePwmState phaseIndex=%d iteration=%d", safe.phaseIndex, safe.iteration);
	efiPrintf("period=%.2f safe.period=%.2f", period, safe.periodNt);
#endif

	if (cisnan(periodNt)) {
		/**
		 * NaN period means PWM is paused, we also set the pin low
		 */
		stateChangeCallback(0, arg);
		return getTimeNowNt() + MS2NT(NAN_FREQUENCY_SLEEP_PERIOD_MS);
	}
	if (mode != PM_NORMAL) {
		// in case of ZERO or FULL we are always at starting index
		safe.phaseIndex = 0;
	}

	if (safe.phaseIndex == 0) {
		handleCycleStart();
	}

	/**
	 * Here is where the 'business logic' - the actual pin state change is happening
	 */
	int cbStateIndex;
	if (mode == PM_NORMAL) {
		// callback state index is offset by one. todo: why? can we simplify this?
		cbStateIndex = safe.phaseIndex == 0 ? phaseCount - 1 : safe.phaseIndex - 1;
	} else if (mode == PM_ZERO) {
		cbStateIndex = 0;
	} else {
		cbStateIndex = 1;
	}

	{
		ScopePerf perf(PE::PwmConfigStateChangeCallback);
		stateChangeCallback(cbStateIndex, arg);
	}

	efitick_t nextSwitchTimeNt = getNextSwitchTimeNt(this);
#if DEBUG_PWM
	efiPrintf("%s: nextSwitchTime %d", state->name, nextSwitchTime);
#endif /* DEBUG_PWM */

	// If we're very far behind schedule, restart the cycle fresh to avoid scheduling a huge pile of events all at once
	// This can happen during config write or debugging where CPU is halted for multiple seconds
	bool isVeryBehindSchedule = nextSwitchTimeNt < getTimeNowNt() - MS2NT(10);

	safe.phaseIndex++;
	if (isVeryBehindSchedule || safe.phaseIndex == phaseCount || mode != PM_NORMAL) {
		safe.phaseIndex = 0; // restart
		safe.iteration++;

		if (isVeryBehindSchedule) {
			forceCycleStart = true;
		}
	}
#if EFI_UNIT_TEST
	printf("PWM: nextSwitchTimeNt=%d phaseIndex=%d iteration=%d\r\n", nextSwitchTimeNt,
			safe.phaseIndex,
			safe.iteration);
#endif /* EFI_UNIT_TEST */
	return nextSwitchTimeNt;
}

/**
 * Main PWM loop: toggle pin & schedule next invocation
 *
 * First invocation happens on application thread
 */
static void timerCallback(PwmConfig *state) {
	ScopePerf perf(PE::PwmGeneratorCallback);

	state->dbgNestingLevel++;
	efiAssertVoid(CUSTOM_ERR_6581, state->dbgNestingLevel < 25, "PWM nesting issue");

	efitick_t switchTimeNt = state->togglePwmState();
	if (switchTimeNt == 0) {
		// we are here when PWM gets stopped
		return;
	}
	if (state->executor == nullptr) {
		firmwareError(CUSTOM_NULL_EXECUTOR, "exec on %s", state->name);
		return;
	}

	state->executor->scheduleByTimestampNt("pwm", &state->scheduling, switchTimeNt, { timerCallback, state });
	state->dbgNestingLevel--;
}

/**
 * Incoming parameters are potentially just values on current stack, so we have to copy
 * into our own permanent storage, right?
 */
void copyPwmParameters(PwmConfig *state, int phaseCount, float const *switchTimes, int waveCount, pin_state_t *const *pinStates) {
	state->phaseCount = phaseCount;

	for (int phaseIndex = 0; phaseIndex < phaseCount; phaseIndex++) {
		state->multiChannelStateSequence.setSwitchTime(phaseIndex, switchTimes[phaseIndex]);

		for (int channelIndex = 0; channelIndex < waveCount; channelIndex++) {
//			print("output switch time index (%d/%d) at %.2f to %d\r\n", phaseIndex, channelIndex,
//					switchTimes[phaseIndex], pinStates[waveIndex][phaseIndex]);
			pin_state_t value = pinStates[channelIndex][phaseIndex];
			state->multiChannelStateSequence.channels[channelIndex].setState(phaseIndex, value);
		}
	}
	if (state->mode == PM_NORMAL) {
		state->multiChannelStateSequence.checkSwitchTimes(phaseCount, 1);
	}
}

/**
 * this method also starts the timer cycle
 * See also startSimplePwm
 */
void PwmConfig::weComplexInit(const char *msg, ExecutorInterface *executor,
		const int phaseCount,
		float const *switchTimes,
		const int waveCount,
		pin_state_t *const*pinStates, pwm_cycle_callback *pwmCycleCallback, pwm_gen_callback *stateChangeCallback) {
	UNUSED(msg);
	this->executor = executor;
	isStopRequested = false;

	efiAssertVoid(CUSTOM_ERR_6582, periodNt != 0, "period is not initialized");
	if (phaseCount == 0) {
		firmwareError(CUSTOM_ERR_PWM_1, "signal length cannot be zero");
		return;
	}
	if (phaseCount > PWM_PHASE_MAX_COUNT) {
		firmwareError(CUSTOM_ERR_PWM_2, "too many phases in PWM");
		return;
	}
	efiAssertVoid(CUSTOM_ERR_6583, waveCount > 0, "waveCount should be positive");

	this->pwmCycleCallback = pwmCycleCallback;
	this->stateChangeCallback = stateChangeCallback;

	multiChannelStateSequence.waveCount = waveCount;

	copyPwmParameters(this, phaseCount, switchTimes, waveCount, pinStates);

	safe.phaseIndex = 0;
	safe.periodNt = -1;
	safe.iteration = -1;

	// let's start the indefinite callback loop of PWM generation
	timerCallback(this);
}

void startSimplePwm(SimplePwm *state, const char *msg, ExecutorInterface *executor,
		OutputPin *output, float frequency, float dutyCycle) {
	efiAssertVoid(CUSTOM_ERR_PWM_STATE_ASSERT, state != NULL, "state");
	efiAssertVoid(CUSTOM_ERR_PWM_DUTY_ASSERT, dutyCycle >= 0 && dutyCycle <= 1, "dutyCycle");
	if (frequency < 1) {
		warning(CUSTOM_OBD_LOW_FREQUENCY, "low frequency %.2f", frequency);
		return;
	}

	float switchTimes[] = { dutyCycle, 1 };
	pin_state_t pinStates0[] = { TV_FALL, TV_RISE };
	state->setSimplePwmDutyCycle(dutyCycle);

	pin_state_t *pinStates[1] = { pinStates0 };

	state->outputPins[0] = output;

	state->setFrequency(frequency);
	state->weComplexInit(msg, executor, 2, switchTimes, 1, pinStates, NULL, (pwm_gen_callback*)applyPinState);
}

void startSimplePwmExt(SimplePwm *state, const char *msg,
		ExecutorInterface *executor,
		brain_pin_e brainPin, OutputPin *output, float frequency,
		float dutyCycle) {

	output->initPin(msg, brainPin);

	startSimplePwm(state, msg, executor, output, frequency, dutyCycle);
}

void startSimplePwmHard(SimplePwm *state, const char *msg,
		ExecutorInterface *executor,
		brain_pin_e brainPin, OutputPin *output, float frequency,
		float dutyCycle) {
#if EFI_PROD_CODE && HAL_USE_PWM
	auto hardPwm = hardware_pwm::tryInitPin(msg, brainPin, frequency, dutyCycle);

	if (hardPwm) {
		state->hardPwm = hardPwm;
	} else {
#endif
		startSimplePwmExt(state, msg, executor, brainPin, output, frequency, dutyCycle);
#if EFI_PROD_CODE && HAL_USE_PWM
	}
#endif
}

/**
 * This method controls the actual hardware pins
 *
 * This method takes ~350 ticks.
 */
void applyPinState(int stateIndex, PwmConfig *state) /* pwm_gen_callback */ {
#if EFI_PROD_CODE
	if (!engine->isPwmEnabled) {
		for (int channelIndex = 0; channelIndex < state->multiChannelStateSequence.waveCount; channelIndex++) {
			OutputPin *output = state->outputPins[channelIndex];
			output->setValue(0);
		}
		return;
	}
#endif // EFI_PROD_CODE

	efiAssertVoid(CUSTOM_ERR_6663, stateIndex < PWM_PHASE_MAX_COUNT, "invalid stateIndex");
	efiAssertVoid(CUSTOM_ERR_6664, state->multiChannelStateSequence.waveCount <= PWM_PHASE_MAX_WAVE_PER_PWM, "invalid waveCount");
	for (int channelIndex = 0; channelIndex < state->multiChannelStateSequence.waveCount; channelIndex++) {
		OutputPin *output = state->outputPins[channelIndex];
		int value = state->multiChannelStateSequence.getChannelState(channelIndex, stateIndex);
		output->setValue(value);
	}
}
