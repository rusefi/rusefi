/**
 * @file	status_loop.h
 *
 * @date Mar 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef CONSOLE_LOOP_H_
#define CONSOLE_LOOP_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void printState(int currentCkpEventCounter);

void initStatusLoop(void);
void updateDevConsoleState(void);
int getFullLog(void);
void printSensors(void);
void finishStatusLine(void);
void setFullLog(int value);
void startStatusThreads(void);
void sayOsHello(void);
int hasFatalError(void);
void onDbgPanic(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CONSOLE_LOOP_H_ */
