/**
 * @file    trigger_emulator.cpp
 * @brief   Position sensor(s) emulation code
 *
 * This file is mostly about initialization, the configuration is
 * provided by the configureShaftPositionEmulatorShape method
 *
 * @date Dec 9, 2012
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "trigger_emulator.h"
#include "eficonsole.h"
#include "main_trigger_callback.h"
#include "io_pins.h"
#include "trigger_emulator_algo.h"
#include "trigger_central.h"
#include "settings.h"

#if EFI_PROD_CODE
#include "pwm_generator.h"
#include "pin_repository.h"
#endif /* EFI_PROD_CODE */

extern PwmConfig triggerSignal;

static OutputPin emulatorOutputs[3];

EXTERN_ENGINE;

void onConfigurationChangeRpmEmulatorCallback(engine_configuration_s *previousConfiguration) {
	if (engineConfiguration->triggerSimulatorFrequency ==
			previousConfiguration->triggerSimulatorFrequency) {
		return;
	}
	setTriggerEmulatorRPM(engineConfiguration->triggerSimulatorFrequency);
}

void initTriggerEmulator(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if EFI_EMULATE_POSITION_SENSORS
	print("Emulating %s\r\n", getConfigurationName(engineConfiguration->engineType));

	triggerSignal.outputPins[0] = &emulatorOutputs[0];
	triggerSignal.outputPins[1] = &emulatorOutputs[1];
	triggerSignal.outputPins[2] = &emulatorOutputs[2];

#if EFI_PROD_CODE
	// todo: refactor, make this a loop
	triggerSignal.outputPins[0]->initPin("Trigger emulator 1", CONFIG(triggerSimulatorPins)[0],
			&CONFIG(triggerSimulatorPinModes)[0]);

	triggerSignal.outputPins[1]->initPin("Trigger emulator 2", CONFIG(triggerSimulatorPins)[1],
			&CONFIG(triggerSimulatorPinModes)[1]);

	triggerSignal.outputPins[2]->initPin("Trigger emulator 3", CONFIG(triggerSimulatorPins)[2],
			&CONFIG(triggerSimulatorPinModes)[2]);
#endif /* EFI_PROD_CODE */

	initTriggerEmulatorLogic(sharedLogger);
#else
	print("No position sensor(s) emulation\r\n");
#endif /* EFI_EMULATE_POSITION_SENSORS */
}
