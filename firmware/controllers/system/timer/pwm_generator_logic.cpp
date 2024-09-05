/**
 * @file    pwm_generator_logic.cpp
 *
 * This PWM implementation keep track of when it would be the next time to toggle the signal.
 * It constantly sets timer to that next toggle time, then sets the timer again from the callback, and so on.
 *
 * @date Mar 2, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"


#if EFI_PROD_CODE
#include "mpu_util.h"
#include "gpio_ext.h"
#endif // EFI_PROD_CODE

// 1% duty cycle
#define ZERO_PWM_THRESHOLD 0.01
// 99% duty cycle
#define FULL_PWM_THRESHOLD 0.99

SimplePwm::SimplePwm()
{
	seq.waveCount = 1;
	seq.phaseCount = 2;
}

SimplePwm::SimplePwm(const char *name) : SimplePwm()  {
	m_name = name;
}

PwmConfig::PwmConfig() {
	memset((void*)&scheduling, 0, sizeof(scheduling));
	memset((void*)&safe, 0, sizeof(safe));
	dbgNestingLevel = 0;
	periodNt = NAN;
	mode = PM_NORMAL;
	memset(&outputPins, 0, sizeof(outputPins));
	m_name = "[noname]";
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
	if (std::isnan(dutyCycle)) {
		warning(ObdCode::CUSTOM_DUTY_INVALID, "%s spwd:dutyCycle %.2f", m_name, dutyCycle);
		return;
	} else if (dutyCycle < 0) {
		warning(ObdCode::CUSTOM_DUTY_TOO_LOW, "%s dutyCycle too low %.2f", m_name, dutyCycle);
		dutyCycle = 0;
	} else if (dutyCycle > 1) {
		warning(ObdCode::CUSTOM_PWM_DUTY_TOO_HIGH, "%s duty too high %.2f", m_name, dutyCycle);
		dutyCycle = 1;
	}

#if EFI_PROD_CODE
	if (hardPwm) {
		hardPwm->setDuty(dutyCycle);
		return;
	}
#endif

	// Handle near-zero and near-full duty cycle.  This will cause the PWM output to behave like a plain digital output.
	if (dutyCycle < ZERO_PWM_THRESHOLD) {
		mode = PM_ZERO;

		if (m_stateChangeCallback) {
			// Manually fire falling edge
			m_stateChangeCallback(0, this);
		}
	} else if (dutyCycle > FULL_PWM_THRESHOLD) {
		mode = PM_FULL;

		if (m_stateChangeCallback) {
			// Manually fire rising edge
			m_stateChangeCallback(1, this);
		}
	} else {
		mode = PM_NORMAL;
		seq.setSwitchTime(0, dutyCycle);
	}
}

/**
 * returns absolute timestamp of state change
 */
static efitick_t getNextSwitchTimeNt(PwmConfig *state) {
	efiAssert(ObdCode::CUSTOM_ERR_ASSERT, state->safe.phaseIndex < PWM_PHASE_MAX_COUNT, "phaseIndex range", 0);
	int iteration = state->safe.iteration;
	// we handle PM_ZERO and PM_FULL separately
	float switchTime = state->mode == PM_NORMAL ? state->multiChannelStateSequence->getSwitchTime(state->safe.phaseIndex) : 1;
	float periodNt = state->safe.periodNt;
#if DEBUG_PWM
	efiPrintf("iteration=%d switchTime=%.2f period=%.2f", iteration, switchTime, period);
#endif /* DEBUG_PWM */

	/**
	 * Once 'iteration' gets relatively high, we might lose calculation precision here.
	 * This is addressed by iterationLimit below, using any many cycles as possible without overflowing timeToSwitchNt
	 * Shall we reuse 'sumTickAndFloat' here?
	 */
	uint32_t timeToSwitchNt = (uint32_t)((iteration + switchTime) * periodNt);

#if DEBUG_PWM
	efiPrintf("start=%d timeToSwitch=%d", state->safe.start, timeToSwitch);
#endif /* DEBUG_PWM */
	return state->safe.startNt + timeToSwitchNt;
}

void PwmConfig::setFrequency(float frequency) {
	if (std::isnan(frequency)) {
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
		firmwareError(ObdCode::CUSTOM_PWM_CYCLE_START, "handleCycleStart %d", safe.phaseIndex);
		return;
	}

	if (m_pwmCycleCallback) {
		m_pwmCycleCallback(this);
	}

	// Compute the maximum number of iterations without overflowing a uint32_t worth of timestamp
	uint32_t iterationLimitInt32 = (0xFFFFFFFF / periodNt) - 2;

	// Maximum number of iterations that don't lose precision due to 32b float (~7 decimal significant figures)
	// We want at least 0.01% timing precision (aka 1/10000 cycle, 0.072 degree for trigger stimulator), which
	// means we can't do any more than 2^23 / 10000 cycles = 838 iterations before a reset
	uint32_t iterationLimitFloat = 838;

	uint32_t iterationLimit = minI(iterationLimitInt32, iterationLimitFloat);

	efiAssertVoid(ObdCode::CUSTOM_ERR_6580, periodNt != 0, "period not initialized");
	efiAssertVoid(ObdCode::CUSTOM_ERR_6580, iterationLimit > 0, "iterationLimit invalid");
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

	if (std::isnan(periodNt)) {
		// NaN period means PWM is paused, we also set the pin low
		if (m_stateChangeCallback) {
			m_stateChangeCallback(0, this);
		}

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
		cbStateIndex = safe.phaseIndex == 0 ? multiChannelStateSequence->phaseCount - 1 : safe.phaseIndex - 1;
	} else if (mode == PM_ZERO) {
		cbStateIndex = 0;
	} else {
		cbStateIndex = 1;
	}

	{
		ScopePerf perf(PE::PwmConfigStateChangeCallback);
		if (m_stateChangeCallback) {
			m_stateChangeCallback(cbStateIndex, this);
		}
	}

	efitick_t nextSwitchTimeNt = getNextSwitchTimeNt(this);
#if DEBUG_PWM
	efiPrintf("%s: nextSwitchTime %d", state->m_name, nextSwitchTime);
#endif /* DEBUG_PWM */

	// If we're very far behind schedule, restart the cycle fresh to avoid scheduling a huge pile of events all at once
	// This can happen during config write or debugging where CPU is halted for multiple seconds
	bool isVeryBehindSchedule = nextSwitchTimeNt < getTimeNowNt() - MS2NT(10);

	safe.phaseIndex++;
	if (isVeryBehindSchedule || safe.phaseIndex == multiChannelStateSequence->phaseCount || mode != PM_NORMAL) {
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
	efiAssertVoid(ObdCode::CUSTOM_ERR_6581, state->dbgNestingLevel < 25, "PWM nesting issue");

	efitick_t switchTimeNt = state->togglePwmState();
	if (switchTimeNt == 0) {
		// we are here when PWM gets stopped
		return;
	}
	if (state->m_executor == nullptr) {
		firmwareError(ObdCode::CUSTOM_NULL_EXECUTOR, "exec on %s", state->m_name);
		return;
	}

	state->m_executor->schedule(state->m_name, &state->scheduling, switchTimeNt, { timerCallback, state });
	state->dbgNestingLevel--;
}

/**
 * Incoming parameters are potentially just values on current stack, so we have to copy
 * into our own permanent storage, right?
 */
void copyPwmParameters(PwmConfig *state, MultiChannelStateSequence const * seq) {
	state->multiChannelStateSequence = seq;
	if (state->mode == PM_NORMAL) {
		state->multiChannelStateSequence->checkSwitchTimes(1);
	}
}

/**
 * this method also starts the timer cycle
 * See also startSimplePwm
 */
void PwmConfig::weComplexInit(Scheduler *executor,
		MultiChannelStateSequence const * seq,
		pwm_cycle_callback *pwmCycleCallback, pwm_gen_callback *stateChangeCallback) {
	m_executor = executor;
	isStopRequested = false;

    // NaN is 'not initialized' but zero is not expected
	criticalAssertVoid(periodNt != 0, "period is not initialized");
	criticalAssertVoid(seq->phaseCount != 0, "signal length cannot be zero");
	criticalAssertVoid(seq->phaseCount <= PWM_PHASE_MAX_COUNT, "too many phases in PWM");
	criticalAssertVoid(seq->waveCount > 0, "waveCount should be positive");

	m_pwmCycleCallback = pwmCycleCallback;
	m_stateChangeCallback = stateChangeCallback;

	copyPwmParameters(this, seq);

	safe.phaseIndex = 0;
	safe.periodNt = -1;
	safe.iteration = -1;

	// let's start the indefinite callback loop of PWM generation
	timerCallback(this);
}

void startSimplePwm(SimplePwm *state, const char *msg,
		Scheduler *executor,
		OutputPin *output, float frequency, float dutyCycle, pwm_gen_callback *callback) {
	efiAssertVoid(ObdCode::CUSTOM_ERR_PWM_STATE_ASSERT, state != NULL, "state");
	efiAssertVoid(ObdCode::CUSTOM_ERR_PWM_DUTY_ASSERT, dutyCycle >= 0 && dutyCycle <= PWM_MAX_DUTY, "dutyCycle");
	if (frequency < 1) {
		warning(ObdCode::CUSTOM_OBD_LOW_FREQUENCY, "low frequency %.2f %s", frequency, msg);
		return;
	}

#if EFI_PROD_CODE
#if (BOARD_EXT_GPIOCHIPS > 0)
	if (!callback) {
		/* No specific scheduler, we can try enabling HW PWM */
		if (brain_pin_is_ext(output->brainPin)) {
			/* this pin is driven by external gpio chip, let's see if it can PWM */
			state->hardPwm = gpiochip_tryInitPwm(msg, output->brainPin, frequency, dutyCycle);
		}
		/* TODO: sohuld we try to init MCU PWM on on-chip brainPin?
		 * Or this should be done only on startSimplePwmHard() call? */
	}

	/* We have succesufully started HW PWM on this output, no need to continue with SW */
	if (state->hardPwm) {
		return;
	}
#endif
#endif

	/* Set default executor for SW PWM */
	if (!callback) {
		callback = applyPinState;
	}

	state->seq.setSwitchTime(0, dutyCycle);
	state->seq.setSwitchTime(1, PWM_MAX_DUTY);
	state->seq.setChannelState(0, 0, TriggerValue::FALL);
	state->seq.setChannelState(0, PWM_MAX_DUTY, TriggerValue::RISE);

	state->outputPins[0] = output;

	state->setFrequency(frequency);
	state->setSimplePwmDutyCycle(dutyCycle);
	state->weComplexInit(executor, &state->seq, nullptr, callback);
}

void startSimplePwmExt(SimplePwm *state, const char *msg,
		Scheduler *executor,
		brain_pin_e brainPin, OutputPin *output, float frequency,
		float dutyCycle, pwm_gen_callback *callback) {

	output->initPin(msg, brainPin);

	startSimplePwm(state, msg, executor, output, frequency, dutyCycle, callback);
}

/**
 * @param dutyCycle value between 0 and 1
 */
void startSimplePwmHard(SimplePwm *state, const char *msg,
		Scheduler *executor,
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
 * default implementation of pwm_gen_callback which simply toggles the pins
 *
 */
void PwmConfig::applyPwmValue(OutputPin *output, int stateIndex, /* weird argument order to facilitate default parameter value */int channelIndex) {
	TriggerValue value = multiChannelStateSequence->getChannelState(channelIndex, stateIndex);
	output->setValue(value == TriggerValue::RISE);
}

/**
 * This method controls the actual hardware pins
 */
void applyPinState(int stateIndex, PwmConfig *state) /* pwm_gen_callback */ {
#if EFI_PROD_CODE
	if (!engine->isPwmEnabled) {
		for (int channelIndex = 0; channelIndex < state->multiChannelStateSequence->waveCount; channelIndex++) {
			OutputPin *output = state->outputPins[channelIndex];
			output->setValue(0);
		}
		return;
	}
#endif // EFI_PROD_CODE

	efiAssertVoid(ObdCode::CUSTOM_ERR_6663, stateIndex < PWM_PHASE_MAX_COUNT, "invalid stateIndex");
	efiAssertVoid(ObdCode::CUSTOM_ERR_6664, state->multiChannelStateSequence->waveCount <= PWM_PHASE_MAX_WAVE_PER_PWM, "invalid waveCount");
	for (int channelIndex = 0; channelIndex < state->multiChannelStateSequence->waveCount; channelIndex++) {
		OutputPin *output = state->outputPins[channelIndex];
		state->applyPwmValue(output, stateIndex, channelIndex);
	}
}
