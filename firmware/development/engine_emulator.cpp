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

#include "poten.h"
#include "trigger_emulator_algo.h"

void initEngineEmulator(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (hasFirmwareError())
		return;

#if EFI_POTENTIOMETER && HAL_USE_SPI
	initPotentiometers(PASS_ENGINE_PARAMETER_SIGNATURE);
#endif /* EFI_POTENTIOMETER && HAL_USE_SPI*/

	initTriggerEmulator(PASS_ENGINE_PARAMETER_SIGNATURE);
}
