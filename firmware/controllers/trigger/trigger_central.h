/*
 * @file	trigger_central.h
 *
 * @date Feb 23, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef TRIGGER_CENTRAL_H_
#define TRIGGER_CENTRAL_H_

#include "trigger_structure.h"

typedef void (*ShaftPositionListener)(ShaftEvents signal, int index);

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
void registerShaftPositionListener(ShaftPositionListener handler, char *msg);
int getCrankEventCounter(void);
int isSignalDecoderError(void);
void hwHandleShaftSignal(ShaftEvents signal);
void initTriggerCentral(void);
void printAllCallbacksHistogram(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TRIGGER_CENTRAL_H_ */
