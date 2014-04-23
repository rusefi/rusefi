/**
 * @file    trigger_emulator.c
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
#include "pwm_generator.h"
#include "io_pins.h"
#include "pin_repository.h"
#include "trigger_emulator_algo.h"

extern engine_configuration_s *engineConfiguration;
extern board_configuration_s *boardConfiguration;

extern PwmConfig configuration;

void initTriggerEmulator(void) {
#if EFI_EMULATE_POSITION_SENSORS
	print("Emulating %s\r\n", getConfigurationName(engineConfiguration));

	configuration.outputPins[0] = TRIGGER_EMILATOR_PRIMARY;
	configuration.outputPins[1] = TRIGGER_EMILATOR_SECONDARY;

	outputPinRegisterExt2("distributor ch1", configuration.outputPins[0], boardConfiguration->triggerSimulatorPins[0],
			&boardConfiguration->triggerSimulatorPinModes[0]);

	outputPinRegisterExt2("distributor ch2", configuration.outputPins[1], boardConfiguration->triggerSimulatorPins[1],
			&boardConfiguration->triggerSimulatorPinModes[1]);

	initTriggerEmulatorLogic(applyPinState);
#else
	print("No position sensor(s) emulation\r\n");
#endif /* EFI_EMULATE_POSITION_SENSORS */
}
