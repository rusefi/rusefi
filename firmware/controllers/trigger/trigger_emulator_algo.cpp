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

#include "pch.h"

int getPreviousIndex(const int currentIndex, const int size) {
	return (currentIndex + size - 1) % size;
}

bool needEvent(const int currentIndex, const MultiChannelStateSequence & mcss, int channelIndex) {
	int prevIndex = getPreviousIndex(currentIndex, mcss.phaseCount);
	pin_state_t previousValue = mcss.getChannelState(channelIndex, /*phaseIndex*/prevIndex);
	pin_state_t currentValue = mcss.getChannelState(channelIndex, /*phaseIndex*/currentIndex);

	return previousValue != currentValue;
}

#if EFI_EMULATE_POSITION_SENSORS

#if !EFI_SHAFT_POSITION_INPUT
	fail("EFI_SHAFT_POSITION_INPUT required to have EFI_EMULATE_POSITION_SENSORS")
#endif

#include "trigger_emulator_algo.h"
#include "trigger_central.h"
#include "trigger_simulator.h"

TriggerEmulatorHelper::TriggerEmulatorHelper() {
}

static OutputPin emulatorOutputs[NUM_EMULATOR_CHANNELS][PWM_PHASE_MAX_WAVE_PER_PWM];

void TriggerEmulatorHelper::handleEmulatorCallback(int ch, const MultiChannelStateSequence& multiChannelStateSequence, int stateIndex) {
	efitick_t stamp = getTimeNowNt();
	
	// todo: code duplication with TriggerStimulatorHelper::feedSimulatedEvent?
#if EFI_SHAFT_POSITION_INPUT
	for (size_t i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++) {
		if (needEvent(stateIndex, multiChannelStateSequence, i)) {
			bool isRise = TriggerValue::RISE == multiChannelStateSequence.getChannelState(/*phaseIndex*/i, stateIndex);

			isRise ^= (i == 0 && engineConfiguration->invertPrimaryTriggerSignal);
			isRise ^= (i == 1 && engineConfiguration->invertSecondaryTriggerSignal);

			if (ch == 0) {
				handleShaftSignal(i, isRise, stamp);
			} else {
				handleVvtCamSignal(isRise ? TriggerValue::RISE : TriggerValue::FALL, stamp, INDEX_BY_BANK_CAM(ch - 1, i));
			}
		}
	}
#endif // EFI_SHAFT_POSITION_INPUT
}

// same is used for either self or external trigger simulation
PwmConfig triggerEmulatorSignals[NUM_EMULATOR_CHANNELS];
TriggerWaveform *triggerEmulatorWaveforms[NUM_EMULATOR_CHANNELS];

static int atTriggerVersions[NUM_EMULATOR_CHANNELS] = { 0 };

/**
 * todo: why is this method NOT reciprocal to getCrankDivider?!
 * todo: oh this method has only one usage? there must me another very similar method!
 */
static float getRpmMultiplier(operation_mode_e mode) {
	if (mode == FOUR_STROKE_THREE_TIMES_CRANK_SENSOR) {
		return SYMMETRICAL_THREE_TIMES_CRANK_SENSOR_DIVIDER / 2;
	} else if (mode == FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR) {
		return SYMMETRICAL_CRANK_SENSOR_DIVIDER / 2;
	} else if (mode == FOUR_STROKE_TWELVE_TIMES_CRANK_SENSOR) {
		return SYMMETRICAL_TWELVE_TIMES_CRANK_SENSOR_DIVIDER / 2;
	} else if (mode == FOUR_STROKE_CAM_SENSOR) {
		return 0.5;
	} else if (mode == FOUR_STROKE_CRANK_SENSOR) {
		// unit test coverage still runs if the value below is changed to '2' not a great sign!
		return 1;
	}

	return 1;
}

void setTriggerEmulatorRPM(int rpm) {
	engineConfiguration->triggerSimulatorRpm = rpm;
	/**
	 * All we need to do here is to change the periodMs
	 * togglePwmState() would see that the periodMs has changed and act accordingly
	 */
	float rPerSecond = NAN;
	if (rpm != 0) {
		float rpmM = getRpmMultiplier(getEngineRotationState()->getOperationMode());
		rPerSecond = rpm * rpmM / 60.0; // per minute converted to per second
	}
	for (int ch = 0; ch < NUM_EMULATOR_CHANNELS; ch++) {
		triggerEmulatorSignals[ch].setFrequency(rPerSecond);
	}

	engine->resetEngineSnifferIfInTestMode();

	efiPrintf("Emulating position sensor(s). RPM=%d", rpm);
}

static void updateTriggerWaveformIfNeeded(PwmConfig *state) {
	for (int ch = 0; ch < NUM_EMULATOR_CHANNELS; ch++) {
		if (state != &triggerEmulatorSignals[ch])
			continue;
		if (atTriggerVersions[ch] < triggerEmulatorWaveforms[ch]->version) {
			atTriggerVersions[ch] = triggerEmulatorWaveforms[ch]->version;
			efiPrintf("Stimulator: updating trigger shape for ch%d: %d/%d %d", ch, atTriggerVersions[ch],
				engine->getGlobalConfigurationVersion(), getTimeNowMs());

			copyPwmParameters(state, &triggerEmulatorWaveforms[ch]->wave);
			state->safe.periodNt = -1; // this would cause loop re-initialization
		}
	}
}

static TriggerEmulatorHelper helper;
static bool hasStimPins = false;

static bool hasInitTriggerEmulator = false;

# if !EFI_UNIT_TEST

static void emulatorApplyPinState(int stateIndex, PwmConfig *state) /* pwm_gen_callback */ {
    assertStackVoid("emulator", ObdCode::STACK_USAGE_MISC, EXPECTED_REMAINING_STACK);
	if (engine->triggerCentral.directSelfStimulation) {
		/**
		 * this callback would invoke the input signal handlers directly
		 */
		for (int ch = 0; ch < NUM_EMULATOR_CHANNELS; ch++) {
			if (state != &triggerEmulatorSignals[ch])
				continue;
			helper.handleEmulatorCallback(ch,
				*state->multiChannelStateSequence,
				stateIndex);
		}
	}

#if EFI_PROD_CODE
	// Only set pins if they're configured - no need to waste the cycles otherwise
	else if (hasStimPins) {
		applyPinState(stateIndex, state);
	}
#endif /* EFI_PROD_CODE */
}

static void startSimulatedTriggerSignal() {
	// No need to start more than once
	if (hasInitTriggerEmulator) {
		return;
	}

	// store the crank+cam waveforms
	triggerEmulatorWaveforms[0] = &engine->triggerCentral.triggerShape;
	for (int cami = 0; cami < CAMS_PER_BANK; cami++) {
		triggerEmulatorWaveforms[1 + cami] = &engine->triggerCentral.vvtShape[cami];
	}

	setTriggerEmulatorRPM(engineConfiguration->triggerSimulatorRpm);
	
	for (int ch = 0; ch < NUM_EMULATOR_CHANNELS; ch++) {
		TriggerWaveform *s = triggerEmulatorWaveforms[ch];
		if (s->getSize() == 0)
			continue;
		triggerEmulatorSignals[ch].weComplexInit(
			&engine->executor,
			&s->wave,
			updateTriggerWaveformIfNeeded, emulatorApplyPinState);
	}
	hasInitTriggerEmulator = true;
}

// self-stimulation
// see below for trigger output generator
void enableTriggerStimulator(bool incGlobalConfiguration) {
	startSimulatedTriggerSignal();
	engine->triggerCentral.directSelfStimulation = true;
    engine->rpmCalculator.Register();
    if (incGlobalConfiguration) {
        incrementGlobalConfigurationVersion("trgSim");
    }
}

// start generating trigger signal on physical outputs
// similar but different from self-stimulation
void enableExternalTriggerStimulator() {
	startSimulatedTriggerSignal();
	engine->triggerCentral.directSelfStimulation = false;
    incrementGlobalConfigurationVersion("extTrg");
}

void disableTriggerStimulator() {
	engine->triggerCentral.directSelfStimulation = false;
	for (int ch = 0; ch < NUM_EMULATOR_CHANNELS; ch++) {
		triggerEmulatorSignals[ch].stop();
	}
	hasInitTriggerEmulator = false;
    incrementGlobalConfigurationVersion("disTrg");
}

void onConfigurationChangeRpmEmulatorCallback(engine_configuration_s *previousConfiguration) {
	if (engineConfiguration->triggerSimulatorRpm ==
			previousConfiguration->triggerSimulatorRpm) {
		return;
	}
	setTriggerEmulatorRPM(engineConfiguration->triggerSimulatorRpm);
}

void initTriggerEmulator() {
	efiPrintf("Emulating %s", getEngine_type_e(engineConfiguration->engineType));

	startTriggerEmulatorPins();

	addConsoleActionI(CMD_RPM, setTriggerEmulatorRPM);
}

#endif /* EFI_UNIT_TEST */

void startTriggerEmulatorPins() {
	hasStimPins = false;
	for (int ch = 0; ch < NUM_EMULATOR_CHANNELS; ch++) {
		for (size_t i = 0; i < efi::size(emulatorOutputs[ch]); i++) {
			triggerEmulatorSignals[ch].outputPins[i] = &emulatorOutputs[ch][i];

			// todo: add pin configs for cam simulator channels
			if (ch != 0)
				continue;
			brain_pin_e pin = engineConfiguration->triggerSimulatorPins[i];

			// Only bother trying to set output pins if they're configured
			if (isBrainPinValid(pin)) {
				hasStimPins = true;
			}

#if EFI_PROD_CODE
			if (isConfigurationChanged(triggerSimulatorPins[i])) {
				triggerEmulatorSignals[ch].outputPins[i]->initPin("Trigger emulator", pin,
					engineConfiguration->triggerSimulatorPinModes[i]);
			}
#endif // EFI_PROD_CODE
		}
	}
}

void stopTriggerEmulatorPins() {
#if EFI_PROD_CODE
	for (int ch = 0; ch < NUM_EMULATOR_CHANNELS; ch++) {
		// todo: add pin configs for cam simulator channels
		if (ch != 0)
			continue;
		for (size_t i = 0; i < efi::size(emulatorOutputs[ch]); i++) {
			if (isConfigurationChanged(triggerSimulatorPins[i])) {
				triggerEmulatorSignals[ch].outputPins[i]->deInit();
			}
		}
	}
#endif // EFI_PROD_CODE
}

#endif /* EFI_EMULATE_POSITION_SENSORS */
