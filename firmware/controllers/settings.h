/**
 * @file settings.h
 * @brief This file is about configuring engine via the human-readable protocol
 *
 * @date Dec 30, 2012
 * @author Andrey Belomutskiy, (c) 2012-2013
 */

#ifndef INJECTOR_CONTROL_H_
#define INJECTOR_CONTROL_H_

void initSettings(void);
void pokeControl(void);
int getInjectionPeriod(void);
int getInjectionOffset(void);
int getInjectionDivider(void);

#endif /* INJECTOR_CONTROL_H_ */
