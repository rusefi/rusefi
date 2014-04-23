/**
 * @file settings.h
 * @brief This file is about configuring engine via the human-readable protocol
 *
 * @date Dec 30, 2012
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef INJECTOR_CONTROL_H_
#define INJECTOR_CONTROL_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void initSettings(void);
void pokeControl(void);
int getInjectionPeriod(void);
int getInjectionOffset(void);
int getInjectionDivider(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* INJECTOR_CONTROL_H_ */
