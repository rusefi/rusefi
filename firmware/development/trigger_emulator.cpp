/**
 * @file    trigger_emulator.cpp
 * @brief   Position sensor(s) emulation code
 *
 * This file is mostly about initialization, the configuration is
 * provided by the configureShaftPositionEmulatorShape method
 *
 * @date Dec 9, 2012
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "trigger_emulator.h"
#include "eficonsole.h"
#include "main_trigger_callback.h"
#include "datalogging.h"
#include "engine_configuration.h"
#include "io_pins.h"
#include "trigger_emulator_algo.h"
#include "trigger_central.h"
#include "settings.h"

#if EFI_PROD_CODE || defined(__DOXYGEN__)
#include "pwm_generator.h"
#include "pin_repository.h"
#endif /* EFI_PROD_CODE */

extern PwmConfig triggerSignal;

static OutputPin emulatorOutputs[3];

EXTERN_ENGINE;

void onConfigurationChangeRpmEmulatorCallback(engine_configuration_s *previousConfiguration) {
	if (engineConfiguration->bc.triggerSimulatorFrequency ==
			previousConfiguration->bc.triggerSimulatorFrequency) {
		return;
	}
	setTriggerEmulatorRPM(engineConfiguration->bc.triggerSimulatorFrequency);
}

void initTriggerEmulator(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX) {


#if EFI_EMULATE_POSITION_SENSORS || defined(__DOXYGEN__)
	print("Emulating %s\r\n", getConfigurationName(engineConfiguration->engineType));

	triggerSignal.outputPins[0] = &emulatorOutputs[0];
	triggerSignal.outputPins[1] = &emulatorOutputs[1];
	triggerSignal.outputPins[2] = &emulatorOutputs[2];

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	// todo: refactor, make this a loop
	triggerSignal.outputPins[0]->initPin("trg emulator ch1", boardConfiguration->triggerSimulatorPins[0],
			&boardConfiguration->triggerSimulatorPinModes[0]);

	triggerSignal.outputPins[1]->initPin("trg emulator ch2", boardConfiguration->triggerSimulatorPins[1],
			&boardConfiguration->triggerSimulatorPinModes[1]);

	triggerSignal.outputPins[2]->initPin("trg emulator ch3", boardConfiguration->triggerSimulatorPins[2],
			&boardConfiguration->triggerSimulatorPinModes[2]);
#endif /* EFI_PROD_CODE */

	initTriggerEmulatorLogic(sharedLogger);
#else
	print("No position sensor(s) emulation\r\n");
#endif /* EFI_EMULATE_POSITION_SENSORS */
}
