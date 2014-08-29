/**
 * @file trigger_emulator_algo.h
 *
 * @date Mar 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef TRIGGER_EMULATOR_ALGO_H_
#define TRIGGER_EMULATOR_ALGO_H_

#include "pwm_generator_logic.h"

/**
 * this is RPM. 10000 rpm is only 166Hz, 800 rpm is 13Hz
 */
#define DEFAULT_EMULATION_RPM 1200

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void initTriggerEmulatorLogic(pwm_gen_callback *callback);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TRIGGER_EMULATOR_ALGO_H_ */
