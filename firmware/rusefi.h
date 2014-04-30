/**
 * @file	rusefi.h
 *
 * @date Dec 25, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef RUSEFI_H_
#define RUSEFI_H_

void runRusEfi(void);
void scheduleReset(void);

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

// these need to be declared C style for the linker magic to work

void DebugMonitorVector(void);
void UsageFaultVector(void);
void BusFaultVector(void);
void HardFaultVector(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RUSEFI_H_ */
