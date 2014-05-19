/**
 * @file    rpm_calculator.h
 * @brief   Shaft position sensor(s) decoder header
 *
 * @date Jan 1, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef RPM_REPORTER_H_
#define RPM_REPORTER_H_

#include <time.h>

#define NOISY_RPM -1

typedef struct {
	volatile int rpm;
	volatile uint64_t lastRpmEventTimeUs;
	/**
	 * This counter is incremented with each revolution of one of the shafts. Could be
	 * crankshaft could be camshaft.
	 */
	volatile int revolutionCounter;
} rpm_s;

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @brief   Initialize RPM calculator
 */
void initRpmCalculator(void);
/**
 * @brief   Current RPM
 */
int getRpm(void);
bool_t isCranking(void);
uint64_t getLastRpmEventTime(void);

int getRevolutionCounter(void);
float getCrankshaftAngle(uint64_t timeUs);
bool_t isRunning(void);
bool_t isValidRpm(int rpm);
void addWaveChartEvent(const char *name, const char *msg, const char *msg2);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RPM_REPORTER_H_ */
