/**
 * @file	engine_emulator.cpp
 * @brief	Entry point for all the emulation and analysis code
 *
 * there is a chance that 90% of the code here is dead
 *
 * @date Mar 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "global.h"
#include "engine_emulator.h"

#include "advance_map.h"
#include "fuel_math.h"

#include "status_loop.h"
#include "logic_analyzer.h"
#include "pin_repository.h"
#include "pwm_generator_logic.h"

#include "poten.h"
#include "trigger_emulator_algo.h"

EXTERN_ENGINE;

void initEngineEmulator(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (hasFirmwareError())
		return;

#if EFI_POTENTIOMETER && HAL_USE_SPI
	initPotentiometers(sharedLogger PASS_ENGINE_PARAMETER_SUFFIX);
#endif /* EFI_POTENTIOMETER && HAL_USE_SPI*/

	initTriggerEmulator(sharedLogger PASS_ENGINE_PARAMETER_SUFFIX);
}
