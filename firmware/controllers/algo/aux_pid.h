/*
 * @file aux_pid.h
 *
 * @date Jun 26, 2016
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef CONTROLLERS_ALGO_AUX_PID_H_
#define CONTROLLERS_ALGO_AUX_PID_H_

#include "engine.h"

void initAuxPid(Logging *sharedLogger);
void startAuxPins(void);
void stopAuxPins(void);

#endif /* CONTROLLERS_ALGO_AUX_PID_H_ */
