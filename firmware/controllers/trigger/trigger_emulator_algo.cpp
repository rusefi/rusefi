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

void TriggerEmulatorHelper::handleEmulatorCallback(int channel, const MultiChannelStateSequence& multiChannelStateSequence, int stateIndex) {
	efitick_t stamp = getTimeNowNt();

	// todo: code duplication with TriggerStimulatorHelper::feedSimulatedEvent?
#if EFI_SHAFT_POSITION_INPUT
	for (size_t i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++) {
		if (needEvent(stateIndex, multiChannelStateSequence, i)) {
			bool isRise = TriggerValue::RISE == multiChannelStateSequence.getChannelState(/*phaseIndex*/i, stateIndex);

			isRise ^= (i == 0 && engineConfiguration->invertPrimaryTriggerSignal);
			isRise ^= (i == 1 && engineConfiguration->invertSecondaryTriggerSignal);

			if (channel == 0) {
				handleShaftSignal(i, isRise, stamp);
			} else {
				handleVvtCamSignal(isRise ? TriggerValue::RISE : TriggerValue::FALL, stamp, INDEX_BY_BANK_CAM(channel - 1, i));
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
  switch (mode) {
    case FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR:
  	case FOUR_STROKE_THREE_TIMES_CRANK_SENSOR:
    case FOUR_STROKE_SIX_TIMES_CRANK_SENSOR:
	  case FOUR_STROKE_TWELVE_TIMES_CRANK_SENSOR:
	  case FOUR_STROKE_CRANK_SENSOR:
	  case FOUR_STROKE_CAM_SENSOR:
	  case OM_NONE:
  		return getCrankDivider(mode) / 2.0;
	  case TWO_STROKE:
		  // unit test coverage still runs if the value below is changed to '2' not a great sign!
		  // but HW CI insists that we have '1' here
		  return 1;
	};
	criticalError("We should not have reach this line");
	return 1;
}

void setTriggerEmulatorRPM(int rpm) {
  criticalAssertVoid(rpm >= 0 && rpm <= 30000, "emulator RPM out of range");

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
	for (int channel = 0; channel < NUM_EMULATOR_CHANNELS; channel++) {
		triggerEmulatorSignals[channel].setFrequency(rPerSecond);
	}

	engine->resetEngineSnifferIfInTestMode();

	efiPrintf("Emulating position sensor(s). RPM=%d", rpm);
}

static void updateTriggerWaveformIfNeeded(PwmConfig *state) {
	for (int channel = 0; channel < NUM_EMULATOR_CHANNELS; channel++) {
		if (state != &triggerEmulatorSignals[channel])
			continue;

    if (atTriggerVersions[channel] < triggerEmulatorWaveforms[channel]->version) {
			atTriggerVersions[channel] = triggerEmulatorWaveforms[channel]->version;
			efiPrintf("Stimulator: updating trigger shape for ch%d: %d/%d %ld", channel, atTriggerVersions[channel],
				engine->getGlobalConfigurationVersion(), getTimeNowMs());

			copyPwmParameters(state, &triggerEmulatorWaveforms[channel]->wave);
			state->safe.periodNt = -1; // this would cause loop re-initialization
		}
	}
}

static TriggerEmulatorHelper helper;
static bool hasStimPins = false;

static bool hasInitTriggerEmulator = false;

#if EFI_PROD_CODE
PUBLIC_API_WEAK void onTriggerEmulatorPinState(int, int) { }
#endif /* EFI_PROD_CODE */

# if !EFI_UNIT_TEST

static void emulatorApplyPinState(int stateIndex, PwmConfig *state) /* pwm_gen_callback */ {
    assertStackVoid("emulator", ObdCode::STACK_USAGE_MISC, EXPECTED_REMAINING_STACK);
	if (engine->triggerCentral.directSelfStimulation) {
		/**
		 * this callback would invoke the input signal handlers directly
		 */
		for (int channel = 0; channel < NUM_EMULATOR_CHANNELS; channel++) {
			if (state != &triggerEmulatorSignals[channel])
				continue;
			helper.handleEmulatorCallback(channel,
				*state->multiChannelStateSequence,
				stateIndex);
		}
	}

#if EFI_PROD_CODE
	// Only set pins if they're configured - no need to waste the cycles otherwise
	else if (hasStimPins) {
		applyPinState(stateIndex, state);

		// this allows any arbitrary code to synchronize with the trigger emulator
		for (int channel = 0; channel < NUM_EMULATOR_CHANNELS; channel++) {
			if (state != &triggerEmulatorSignals[channel])
				continue;
			onTriggerEmulatorPinState(stateIndex, channel);
		}
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

	for (int channel = 0; channel < NUM_EMULATOR_CHANNELS; channel++) {
		TriggerWaveform *s = triggerEmulatorWaveforms[channel];
		if (s->getSize() == 0)
			continue;
		triggerEmulatorSignals[channel].weComplexInit(
			&engine->scheduler,
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
	for (int channel = 0; channel < NUM_EMULATOR_CHANNELS; channel++) {
		triggerEmulatorSignals[channel].stop();
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
	startTriggerEmulatorPins();

	addConsoleActionI(CMD_RPM, setTriggerEmulatorRPM);
}

#endif /* EFI_UNIT_TEST */

void startTriggerEmulatorPins() {
	hasStimPins = false;
	for (int channel = 0; channel < NUM_EMULATOR_CHANNELS; channel++) {
		for (size_t i = 0; i < efi::size(emulatorOutputs[channel]); i++) {
			triggerEmulatorSignals[channel].outputPins[i] = &emulatorOutputs[channel][i];

#if EFI_PROD_CODE
      brain_pin_e pin;

      pin_output_mode_e outputMode;
			if (channel == 0) {
  			pin = engineConfiguration->triggerSimulatorPins[i];
  			outputMode = engineConfiguration->triggerSimulatorPinModes[i];
  		} else if (channel == 1 && i == 0) {
  		  pin = engineConfiguration->camSimulatorPin;
  		  outputMode = engineConfiguration->camSimulatorPinMode;
  		} else {
			  // todo: add pin configs for cam simulator channels
  		  continue;
  		}

			// Only bother trying to set output pins if they're configured
			if (isBrainPinValid(pin)) {
				hasStimPins = true;
			}

			if (isConfigurationChanged(triggerSimulatorPins[i])) {
				triggerEmulatorSignals[channel].outputPins[i]->initPin("Trigger emulator", pin,
					outputMode);
			}
#endif // EFI_PROD_CODE
		}
	}
}

void stopTriggerEmulatorPins() {
#if EFI_PROD_CODE
	for (int channel = 0; channel < NUM_EMULATOR_CHANNELS; channel++) {
		// todo: add pin configs for cam simulator channels
		if (channel != 0)
			continue;
		for (size_t i = 0; i < efi::size(emulatorOutputs[channel]); i++) {
			if (isConfigurationChanged(triggerSimulatorPins[i])) {
				triggerEmulatorSignals[channel].outputPins[i]->deInit();
			}
		}
	}
#endif // EFI_PROD_CODE
}

#endif /* EFI_EMULATE_POSITION_SENSORS */
