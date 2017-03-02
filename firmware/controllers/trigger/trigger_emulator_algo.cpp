/**
 * @file trigger_emulator_algo.cpp
 *
 * This file is about producing real electrical signals which emulate trigger signal based on
 * a known TriggerShape.
 *
 * Historically this implementation was implemented based on PwmConfig which is maybe not the
 * best way to implement it. (todo: why is not the best way?)
 *
 * A newer implementation of pretty much the same thing is TriggerStimulatorHelper
 * todo: one emulator should be enough! another one should be eliminated
 *
 * @date Mar 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */
#include "main.h"

#if EFI_EMULATE_POSITION_SENSORS || defined(__DOXYGEN__)

#include "trigger_emulator_algo.h"
#include "engine_configuration.h"
#include "LocalVersionHolder.h"
#include "trigger_central.h"

#if EFI_PROD_CODE
#include "pwm_generator.h"
#endif

TriggerEmulatorHelper::TriggerEmulatorHelper() {
	primaryWheelState = false;
	secondaryWheelState = false;
	thirdWheelState = false;
}

// this is not the only place where we have 'isUpEvent'. todo: reuse
static bool isRisingEdge[6] = { false, true, false, true, false, true };

EXTERN_ENGINE
;

static void fireShaftSignal(trigger_event_e signal) {
	if (!engineConfiguration->useOnlyRisingEdgeForTrigger || isRisingEdge[(int) signal])
		hwHandleShaftSignal(signal);
}

void TriggerEmulatorHelper::handleEmulatorCallback(PwmConfig *state, int stateIndex) {
	int prevIndex = (stateIndex + state->phaseCount - 1) % state->phaseCount;

	bool primaryWheelState = state->multiWave.waves[0].pinStates[prevIndex];
	int newPrimaryWheelState = state->multiWave.waves[0].pinStates[stateIndex];

	bool secondaryWheelState = state->multiWave.waves[1].pinStates[prevIndex];
	int newSecondaryWheelState = state->multiWave.waves[1].pinStates[stateIndex];

	bool thirdWheelState = state->multiWave.waves[2].pinStates[prevIndex];
	int new3rdWheelState = state->multiWave.waves[2].pinStates[stateIndex];

	if (primaryWheelState != newPrimaryWheelState) {
		primaryWheelState = newPrimaryWheelState;
		fireShaftSignal(primaryWheelState ? SHAFT_PRIMARY_RISING : SHAFT_PRIMARY_FALLING);
	}

	if (secondaryWheelState != newSecondaryWheelState) {
		secondaryWheelState = newSecondaryWheelState;
		fireShaftSignal(secondaryWheelState ? SHAFT_SECONDARY_RISING : SHAFT_SECONDARY_FALLING);
	}

	if (thirdWheelState != new3rdWheelState) {
		thirdWheelState = new3rdWheelState;
		fireShaftSignal(thirdWheelState ? SHAFT_3RD_RISING : SHAFT_3RD_FALLING);
	}

	//	print("hello %d\r\n", chTimeNow());
}

/*
 * todo: should we simply re-use instances used by trigger_decoder?
 * todo: since we are emulating same shape we are decoding
 */
static pin_state_t pinStates1[PWM_PHASE_MAX_COUNT];
static pin_state_t pinStates2[PWM_PHASE_MAX_COUNT];
static pin_state_t pinStates3[PWM_PHASE_MAX_COUNT];
static single_wave_s waves[PWM_PHASE_MAX_WAVE_PER_PWM] = { single_wave_s(pinStates1), single_wave_s(pinStates2),
		single_wave_s(pinStates3) };
static single_wave_s sr[PWM_PHASE_MAX_WAVE_PER_PWM] = { waves[0], waves[1], waves[2] };

static float switchTimesBuffer[PWM_PHASE_MAX_COUNT];

PwmConfig triggerSignal(switchTimesBuffer, sr);

#define DO_NOT_STOP 999999999

static int stopEmulationAtIndex = DO_NOT_STOP;
static bool isEmulating = true;

static Logging *logger;
static LocalVersionHolder emulatorConfigVersion;

#if EFI_ENGINE_SNIFFER
#include "engine_sniffer.h"
extern WaveChart waveChart;
#endif /* EFI_ENGINE_SNIFFER */

void setTriggerEmulatorRPM(int rpm, Engine *engine) {
	engineConfiguration->bc.triggerSimulatorFrequency = rpm;
	/**
	 * All we need to do here is to change the periodMs
	 * togglePwmState() would see that the periodMs has changed and act accordingly
	 */
	if (rpm == 0) {
		triggerSignal.periodNt = NAN;
	} else {
		float rpmM = getRpmMultiplier(engineConfiguration->operationMode);
		float gRpm = rpm * rpmM / 60.0; // per minute converted to per second
		triggerSignal.periodNt = US2NT(frequency2periodUs(gRpm));
	}
#if EFI_ENGINE_SNIFFER
	if (engine->isTestMode)
		waveChart.reset();
#endif /* EFI_ENGINE_SNIFFER */

	scheduleMsg(logger, "Emulating position sensor(s). RPM=%d", rpm);
}

static void updateTriggerShapeIfNeeded(PwmConfig *state) {
	if (emulatorConfigVersion.isOld()) {
		scheduleMsg(logger, "Stimulator: updating trigger shape: %d/%d %d", emulatorConfigVersion.getVersion(),
				getGlobalConfigurationVersion(), currentTimeMillis());

		applyNonPersistentConfiguration(logger PASS_ENGINE_PARAMETER);

		TriggerShape *s = &engine->triggerCentral.triggerShape;
		pin_state_t *pinStates[PWM_PHASE_MAX_WAVE_PER_PWM] = { s->wave.waves[0].pinStates, s->wave.waves[1].pinStates,
				s->wave.waves[2].pinStates };
		copyPwmParameters(state, s->getSize(), s->wave.switchTimes, PWM_PHASE_MAX_WAVE_PER_PWM, pinStates);
		state->safe.periodNt = -1; // this would cause loop re-initialization
	}
}

static TriggerEmulatorHelper helper;

static void emulatorApplyPinState(PwmConfig *state, int stateIndex) {
	if (stopEmulationAtIndex == stateIndex) {
		isEmulating = false;
	}
	if (!isEmulating) {
		return;
	}
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	applyPinState(state, stateIndex);
#endif /* EFI_PROD_CODE */
	if (engineConfiguration->directSelfStimulation) {
		/**
		 * this callback would invoke the input signal handlers directly
		 */
		helper.handleEmulatorCallback(state, stateIndex);
	}
}

static void setEmulatorAtIndex(int index, Engine *engine) {
	stopEmulationAtIndex = index;
}

static void resumeStimulator(Engine *engine) {
	isEmulating = true;
	stopEmulationAtIndex = DO_NOT_STOP;
}

void initTriggerEmulatorLogic(Logging *sharedLogger, Engine *engine) {
	logger = sharedLogger;

	TriggerShape *s = &engine->triggerCentral.triggerShape;
	setTriggerEmulatorRPM(engineConfiguration->bc.triggerSimulatorFrequency, engine);
	pin_state_t *pinStates[PWM_PHASE_MAX_WAVE_PER_PWM] = { s->wave.waves[0].pinStates, s->wave.waves[1].pinStates,
			s->wave.waves[2].pinStates };
	triggerSignal.weComplexInit("position sensor", s->getSize(), s->wave.switchTimes, PWM_PHASE_MAX_WAVE_PER_PWM,
			pinStates, updateTriggerShapeIfNeeded, emulatorApplyPinState);

	addConsoleActionIP("rpm", (VoidIntVoidPtr) setTriggerEmulatorRPM, engine);
	addConsoleActionIP("stop_stimulator_at_index", (VoidIntVoidPtr) setEmulatorAtIndex, engine);
	addConsoleActionP("resume_stimulator", (VoidPtr) resumeStimulator, engine);
}
#endif
