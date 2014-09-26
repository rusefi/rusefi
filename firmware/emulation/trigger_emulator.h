/**
 * @file    trigger_emulator.h
 * @brief   Position sensor(s) emulation header
 *
 * @date Dec 9, 2012
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef DIST_EMULATOR_H_
#define DIST_EMULATOR_H_

#include "main.h"

#include "trigger_structure.h"
#include "engine_configuration.h"
#include "pwm_generator_logic.h"

void initTriggerEmulator(void);
void setTriggerEmulatorRPM(int value);

#endif /* DIST_EMULATOR_H_ */
