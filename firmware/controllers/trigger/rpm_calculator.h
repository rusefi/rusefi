/**
 * @file    rpm_calculator.h
 * @brief   Shaft position sensor(s) decoder header
 *
 * @date Jan 1, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef RPM_REPORTER_H_
#define RPM_REPORTER_H_

#include <global.h>

#define WC_DOWN "d"
#define WC_UP "u"
#define WC_CRANK1 "c1"
#define WC_CRANK2 "c2"
#define WC_CRANK3 "c3"

#define NOISY_RPM -1

#ifdef __cplusplus
#include "engine.h"

class RpmCalculator {
public:
	RpmCalculator();
	int rpm(void);
	volatile int rpmValue;
	volatile uint64_t lastRpmEventTimeUs;
	/**
	 * This counter is incremented with each revolution of one of the shafts. Could be
	 * crankshaft could be camshaft.
	 */
	volatile int revolutionCounter;
	bool isRunning(void);
};

#define getRpm() getRpmE(&engine)

/**
 * @brief   Current RPM
 */
int getRpmE(Engine *engine);
void rpmShaftPositionCallback(trigger_event_e ckpSignalType, int index, RpmCalculator *rpmState);
#endif

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @brief   Initialize RPM calculator
 */
void initRpmCalculator(void);
bool isCranking(void);
uint64_t getLastRpmEventTime(void);

int getRevolutionCounter(void);
float getCrankshaftAngle(uint64_t timeUs);
/**
 * @return true if engine is running
 */
bool isRunning(void);
bool isValidRpm(int rpm);
void addWaveChartEvent(const char *name, const char *msg, const char *msg2);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RPM_REPORTER_H_ */
