/**
 * @file	idle_controller.h
 *
 * @date May 22, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef IDLE_CONTROLLER_H_
#define IDLE_CONTROLLER_H_

// 700‰ duty cycle by default
#define DEFAULT_IDLE_DUTY 600

#define DEFAULT_TARGET_RPM 1000
//#define DEFAULT_TARGET_RPM 2000

#define IDLE_PERIOD 1

// Per mil (1/1000) values
#define MIN_IDLE 100
#define MAX_IDLE 900
#define IDLE_INCREASE_STEP 5
#define IDLE_DECREASE_STEP 5


typedef struct {
	int time;

	int targetRpmRangeLeft, targetRpmRangeRight;

	int value;
	int timeOfLastIdleChange;
} IdleValveState;

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void idleInit(IdleValveState *idle);
int getIdle(IdleValveState *idle, int currentRpm, int time);
void setIdleRpm(IdleValveState *idle, int targetRpm);

void idleDebug(char *msg, int value);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* IDLE_CONTROLLER_H_ */
