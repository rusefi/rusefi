/**
 * @file trigger_emulator_algo.cpp
 *
 * This file is about producing real electrical signals which emulate trigger signal based on
 * a known TriggerWaveform.
 *
 * Historically this implementation was implemented based on PwmConfig which is maybe not the
 * best way to implement it. (todo: why is not the best way?)
 *
 * A newer implementation of pretty much the same thing is TriggerStimulatorHelper
 * todo: one emulator should be enough! another one should be eliminated
 *
 * @date Mar 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */
#include "state_sequence.h"
#include "global.h"
#include "efi_gpio.h"

int getPreviousIndex(const int currentIndex, const int size) {
	return (currentIndex + size - 1) % size;
}

bool needEvent(const int currentIndex, const int size, const MultiChannelStateSequence& mcss, int channelIndex) {
	int prevIndex = getPreviousIndex(currentIndex, size);
	pin_state_t previousValue = mcss.getChannelState(channelIndex, /*phaseIndex*/prevIndex);
	pin_state_t currentValue = mcss.getChannelState(channelIndex, /*phaseIndex*/currentIndex);

	return previousValue != currentValue;
}

#if EFI_EMULATE_POSITION_SENSORS

#include "engine.h"
#include "trigger_emulator_algo.h"
#include "engine_configuration.h"
#include "trigger_central.h"
#include "trigger_simulator.h"
#include "settings.h"
#include "pwm_generator_logic.h"

TriggerEmulatorHelper::TriggerEmulatorHelper() {
}

EXTERN_ENGINE;

static OutputPin emulatorOutputs[PWM_PHASE_MAX_WAVE_PER_PWM];

void TriggerEmulatorHelper::handleEmulatorCallback(PwmConfig *state, int stateIndex) {
	efitick_t stamp = getTimeNowNt();
	
	// todo: code duplication with TriggerStimulatorHelper::feedSimulatedEvent?
	MultiChannelStateSequence *multiChannelStateSequence = &state->multiChannelStateSequence;

	for (size_t i = 0; i < efi::size(emulatorOutputs); i++)
	{
		if (needEvent(stateIndex, state->phaseCount, state->multiChannelStateSequence, i)) {
			pin_state_t currentValue = multiChannelStateSequence->getChannelState(/*phaseIndex*/i, stateIndex);
			
			constexpr trigger_event_e riseEvents[] = { SHAFT_PRIMARY_RISING, SHAFT_SECONDARY_RISING, SHAFT_3RD_RISING };
			constexpr trigger_event_e fallEvents[] = { SHAFT_PRIMARY_FALLING, SHAFT_SECONDARY_FALLING, SHAFT_3RD_FALLING };

			trigger_event_e event = (currentValue ? riseEvents : fallEvents)[i];

			hwHandleShaftSignal(event, stamp);
		}
	}
}

/*
 * todo: should we simply re-use instances used by trigger_decoder?
 * todo: since we are emulating same shape we are decoding
 */
static pin_state_t pinStates1[PWM_PHASE_MAX_COUNT];
static pin_state_t pinStates2[PWM_PHASE_MAX_COUNT];
static pin_state_t pinStates3[PWM_PHASE_MAX_COUNT];
static SingleChannelStateSequence waves[PWM_PHASE_MAX_WAVE_PER_PWM] = { SingleChannelStateSequence(pinStates1), SingleChannelStateSequence(pinStates2),
		SingleChannelStateSequence(pinStates3) };
static SingleChannelStateSequence sr[PWM_PHASE_MAX_WAVE_PER_PWM] = { waves[0], waves[1], waves[2] };

static float pwmSwitchTimesBuffer[PWM_PHASE_MAX_COUNT];

PwmConfig triggerSignal(pwmSwitchTimesBuffer, sr);

#define DO_NOT_STOP 999999999

static int stopEmulationAtIndex = DO_NOT_STOP;
static bool isEmulating = true;

static Logging *logger;
static int atTriggerVersion = 0;

#if EFI_ENGINE_SNIFFER
#include "engine_sniffer.h"
extern WaveChart waveChart;
#endif /* EFI_ENGINE_SNIFFER */

void setTriggerEmulatorRPM(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX) {
	engineConfiguration->triggerSimulatorFrequency = rpm;
	/**
	 * All we need to do here is to change the periodMs
	 * togglePwmState() would see that the periodMs has changed and act accordingly
	 */
	if (rpm == 0) {
		triggerSignal.setFrequency(NAN);
	} else {
		float rpmM = getRpmMultiplier(engine->getOperationMode(PASS_ENGINE_PARAMETER_SIGNATURE));
		float rPerSecond = rpm * rpmM / 60.0; // per minute converted to per second
		triggerSignal.setFrequency(rPerSecond);
	}
	engine->resetEngineSnifferIfInTestMode();

	scheduleMsg(logger, "Emulating position sensor(s). RPM=%d", rpm);
}

static void updateTriggerWaveformIfNeeded(PwmConfig *state) {
	if (atTriggerVersion < engine->triggerCentral.triggerShape.version) {
		atTriggerVersion = engine->triggerCentral.triggerShape.version;
		scheduleMsg(logger, "Stimulator: updating trigger shape: %d/%d %d", atTriggerVersion,
				engine->getGlobalConfigurationVersion(), currentTimeMillis());


		TriggerWaveform *s = &engine->triggerCentral.triggerShape;
		pin_state_t *pinStates[PWM_PHASE_MAX_WAVE_PER_PWM] = {
				s->wave.channels[0].pinStates,
				s->wave.channels[1].pinStates,
				s->wave.channels[2].pinStates };
		copyPwmParameters(state, s->getSize(), s->wave.switchTimes, PWM_PHASE_MAX_WAVE_PER_PWM, pinStates);
		state->safe.periodNt = -1; // this would cause loop re-initialization
	}
}

static TriggerEmulatorHelper helper;
static bool hasStimPins = false;

static void emulatorApplyPinState(int stateIndex, PwmConfig *state) /* pwm_gen_callback */ {
	if (stopEmulationAtIndex == stateIndex) {
		isEmulating = false;
	}
	if (!isEmulating) {
		return;
	}

	if (engine->directSelfStimulation) {
		/**
		 * this callback would invoke the input signal handlers directly
		 */
		helper.handleEmulatorCallback(state, stateIndex);
	}

#if EFI_PROD_CODE
	// Only set pins if they're configured - no need to waste the cycles otherwise
	else if (hasStimPins) {
		applyPinState(stateIndex, state);
	}
#endif /* EFI_PROD_CODE */

}

static void setEmulatorAtIndex(int index) {
	stopEmulationAtIndex = index;
}

static void resumeStimulator() {
	isEmulating = true;
	stopEmulationAtIndex = DO_NOT_STOP;
}

void initTriggerEmulatorLogic(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX) {
	logger = sharedLogger;

	TriggerWaveform *s = &engine->triggerCentral.triggerShape;
	setTriggerEmulatorRPM(engineConfiguration->triggerSimulatorFrequency PASS_ENGINE_PARAMETER_SUFFIX);
	pin_state_t *pinStates[PWM_PHASE_MAX_WAVE_PER_PWM] = {
			s->wave.channels[0].pinStates,
			s->wave.channels[1].pinStates,
			s->wave.channels[2].pinStates };
	triggerSignal.weComplexInit("position sensor",
			&engine->executor,
			s->getSize(), s->wave.switchTimes, PWM_PHASE_MAX_WAVE_PER_PWM,
			pinStates, updateTriggerWaveformIfNeeded, (pwm_gen_callback*)emulatorApplyPinState);

	addConsoleActionI(CMD_RPM, setTriggerEmulatorRPM);
	addConsoleActionI("stop_stimulator_at_index", setEmulatorAtIndex);
	addConsoleAction("resume_stimulator", resumeStimulator);
}

void onConfigurationChangeRpmEmulatorCallback(engine_configuration_s *previousConfiguration) {
	if (engineConfiguration->triggerSimulatorFrequency ==
			previousConfiguration->triggerSimulatorFrequency) {
		return;
	}
	setTriggerEmulatorRPM(engineConfiguration->triggerSimulatorFrequency);
}

void initTriggerEmulator(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX) {
	scheduleMsg(sharedLogger, "Emulating %s", getConfigurationName(engineConfiguration->engineType));

	initTriggerEmulatorLogic(sharedLogger);
}

void startTriggerEmulatorPins() {
	hasStimPins = false;
	for (size_t i = 0; i < efi::size(emulatorOutputs); i++) {
		triggerSignal.outputPins[i] = &emulatorOutputs[i];

		brain_pin_e pin = CONFIG(triggerSimulatorPins)[i];

		// Only bother trying to set output pins if they're configured
		if (pin != GPIO_UNASSIGNED) {
			hasStimPins = true;
		}

#if EFI_PROD_CODE
		triggerSignal.outputPins[i]->initPin("Trigger emulator", pin,
					&CONFIG(triggerSimulatorPinModes)[i]);
#endif // EFI_PROD_CODE
	}
}

void stopTriggerEmulatorPins() {
	for (size_t i = 0; i < efi::size(emulatorOutputs); i++) {
		brain_pin_e brainPin = activeConfiguration.triggerSimulatorPins[i];
		if (brainPin != GPIO_UNASSIGNED) {
#if EFI_PROD_CODE
			efiSetPadUnused(brainPin);
#endif // EFI_PROD_CODE
		}
	}
}

#endif /* EFI_EMULATE_POSITION_SENSORS */
