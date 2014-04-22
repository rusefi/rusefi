/**
 * @file    injector_central.h
 * @brief	Utility methods related to fuel injection.
 *
 *
 * @date Sep 8, 2013
 * @author Andrey Belomutskiy, (c) 2012-2013
 */


#ifndef INJECTOR_CENTRAL_H_
#define INJECTOR_CENTRAL_H_

#include "signal_executor.h"

void initInjectorCentral(void);
int isInjectionEnabled(void);
int isInjectorEnabled(int cylinderId);
void assertCylinderId(int cylinderId, char *msg);

#endif /* INJECTOR_CENTRAL_H_ */
