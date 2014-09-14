/**
 * @file    trigger_emulator.cpp
 * @brief   Position sensor(s) emulation code
 *
 * This file is mostly about initialization, the configuration is
 * provided by the configureShaftPositionEmulatorShape method
 *
 * @date Dec 9, 2012
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "trigger_emulator.h"
#include "eficonsole.h"
#include "main_trigger_callback.h"
#include "datalogging.h"
#include "engine_configuration.h"
#if EFI_PROD_CODE
#include "pwm_generator.h"
#include "pin_repository.h"
#endif
#include "io_pins.h"
#include "trigger_emulator_algo.h"
#include "trigger_central.h"

extern engine_configuration_s *engineConfiguration;
extern board_configuration_s *boardConfiguration;

extern PwmConfig triggerSignal;

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

static TriggerEmulatorHelper helper;

#if EFI_EMULATE_POSITION_SENSORS || defined(__DOXYGEN__)

static void emulatorApplyPinState(PwmConfig *state, int stateIndex) {
	applyPinState(state, stateIndex);
	if (engineConfiguration->directSelfStimulation) {
		helper.handleEmulatorCallback(state, stateIndex);
	}
}
#endif /* EFI_EMULATE_POSITION_SENSORS */

void initTriggerEmulator(void) {
#if EFI_EMULATE_POSITION_SENSORS || defined(__DOXYGEN__)
	print("Emulating %s\r\n", getConfigurationName(engineConfiguration->engineType));

	triggerSignal.outputPins[0] = TRIGGER_EMULATOR_PRIMARY;
	triggerSignal.outputPins[1] = TRIGGER_EMULATOR_SECONDARY;
	triggerSignal.outputPins[2] = TRIGGER_EMULATOR_3RD;

	// todo: refactor, make this a loop
	outputPinRegisterExt2("distributor ch1", triggerSignal.outputPins[0], boardConfiguration->triggerSimulatorPins[0],
			&boardConfiguration->triggerSimulatorPinModes[0]);

	outputPinRegisterExt2("distributor ch2", triggerSignal.outputPins[1], boardConfiguration->triggerSimulatorPins[1],
			&boardConfiguration->triggerSimulatorPinModes[1]);

	outputPinRegisterExt2("distributor ch3", triggerSignal.outputPins[2], boardConfiguration->triggerSimulatorPins[2],
			&boardConfiguration->triggerSimulatorPinModes[2]);


	initTriggerEmulatorLogic(emulatorApplyPinState);
#else
	print("No position sensor(s) emulation\r\n");
#endif /* EFI_EMULATE_POSITION_SENSORS */
}
