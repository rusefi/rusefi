/**
 * @file trigger_emulator_algo.cpp
 *
 * @date Mar 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
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

void TriggerEmulatorHelper::handleEmulatorCallback(PwmConfig *state, int stateIndex) {
	int newPrimaryWheelState = state->multiWave.waves[0].pinStates[stateIndex];
	int newSecondaryWheelState = state->multiWave.waves[1].pinStates[stateIndex];
	int new3rdWheelState = state->multiWave.waves[2].pinStates[stateIndex];

	if (primaryWheelState != newPrimaryWheelState) {
		primaryWheelState = newPrimaryWheelState;
		hwHandleShaftSignal(primaryWheelState ? SHAFT_PRIMARY_UP : SHAFT_PRIMARY_DOWN);
	}

	if (secondaryWheelState != newSecondaryWheelState) {
		secondaryWheelState = newSecondaryWheelState;
		hwHandleShaftSignal(secondaryWheelState ? SHAFT_SECONDARY_UP : SHAFT_SECONDARY_DOWN);
	}

	if (thirdWheelState != new3rdWheelState) {
		thirdWheelState = new3rdWheelState;
		hwHandleShaftSignal(thirdWheelState ? SHAFT_3RD_UP : SHAFT_3RD_DOWN);
	}

	//	print("hello %d\r\n", chTimeNow());
}

EXTERN_ENGINE;

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

static float swtchTms[PWM_PHASE_MAX_COUNT];

PwmConfig triggerSignal(swtchTms, sr);

#define DO_NOT_STOP 999999999

static int stopEmulationAtIndex = DO_NOT_STOP;
static bool isEmulating = true;

static Logging *logger;
static LocalVersionHolder localVersion;

EXTERN_ENGINE;

void setTriggerEmulatorRPM(int rpm, Engine *engine) {
	engineConfiguration->bc.triggerSimulatorFrequency = rpm;
	/**
	 * All we need to do here is to change the periodMs
	 * togglePwmState() would see that the periodMs has changed and act accordingly
	 */
	if (rpm == 0) {
		triggerSignal.periodNt = NAN;
	} else {
		float gRpm = rpm * engineConfiguration->rpmMultiplier / 60.0; // per minute converted to per second
		triggerSignal.periodNt = US2NT(frequency2periodUs(gRpm));
	}
	scheduleMsg(logger, "Emulating position sensor(s). RPM=%d", rpm);
}

static void updateTriggerShapeIfNeeded(PwmConfig *state) {
	if (localVersion.isOld()) {
		scheduleMsg(logger, "Stimulator: updating trigger shape: %d/%d %d", localVersion.getVersion(),
				getGlobalConfigurationVersion(), currentTimeMillis());

		applyNonPersistentConfiguration(logger, engine);

		TriggerShape *s = &engine->triggerShape;
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

	TriggerShape *s = &engine->triggerShape;
	setTriggerEmulatorRPM(engineConfiguration->bc.triggerSimulatorFrequency, engine);
	pin_state_t *pinStates[PWM_PHASE_MAX_WAVE_PER_PWM] = { s->wave.waves[0].pinStates, s->wave.waves[1].pinStates,
			s->wave.waves[2].pinStates };
	triggerSignal.weComplexInit("position sensor", s->getSize(), s->wave.switchTimes, PWM_PHASE_MAX_WAVE_PER_PWM,
			pinStates, updateTriggerShapeIfNeeded, emulatorApplyPinState);

	addConsoleActionIP("rpm", (VoidIntVoidPtr)setTriggerEmulatorRPM, engine);
	addConsoleActionIP("stop_stimulator_at_index", (VoidIntVoidPtr)setEmulatorAtIndex, engine);
	addConsoleActionP("resume_stimulator", (VoidPtr) resumeStimulator, engine);
}
#endif
