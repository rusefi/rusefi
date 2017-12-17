/**
 * @file    trigger_emulator.h
 * @brief   Position sensor(s) emulation header
 *
 * @date Dec 9, 2012
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef DIST_EMULATOR_H_
#define DIST_EMULATOR_H_

#include "main.h"

#include "trigger_structure.h"
#include "engine.h"

void initTriggerEmulator(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX);
void setTriggerEmulatorRPM(int value DECLARE_ENGINE_PARAMETER_SUFFIX);
void onConfigurationChangeRpmEmulatorCallback(engine_configuration_s *previousConfiguration);

#endif /* DIST_EMULATOR_H_ */
