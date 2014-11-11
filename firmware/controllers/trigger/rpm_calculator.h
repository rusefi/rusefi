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

class Engine;

class RpmCalculator {
public:
#if !EFI_PROD_CODE
	int mockRpm;
#endif
	RpmCalculator();
	bool isRunning(void);
	int rpm(void);
	void onNewEngineCycle();
	uint32_t getRevolutionCounter(void);
	void setRpmValue(int value);
	uint32_t getRevolutionCounterSinceStart(void);
	volatile int rpmValue;
	/**
	 * This is a performance optimization: let's pre-calulate this each time RPM changes
	 */
	volatile float oneDegreeUs;
	volatile uint64_t lastRpmEventTimeNt;
private:
	/**
	 * This counter is incremented with each revolution of one of the shafts. Could be
	 * crankshaft could be camshaft.
	 */
	volatile uint32_t revolutionCounterSinceBoot;
	/**
	 * Same as the above, but since the engine started spinning
	 */
	volatile uint32_t revolutionCounterSinceStart;
};

#define getRpm() getRpmE(engine)

/**
 * @brief   Current RPM
 */
int getRpmE(Engine *engine);
bool isCrankingE(Engine *engine);
void rpmShaftPositionCallback(trigger_event_e ckpSignalType, uint32_t index, Engine *engine);
/**
 * @brief   Initialize RPM calculator
 */
void initRpmCalculator(Engine *engine);

float getCrankshaftAngleNt(Engine *engine, uint64_t timeNt);
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

bool isCranking(void);

int getRevolutionCounter(void);
bool isValidRpm(int rpm);
void addWaveChartEvent(const char *name, const char *msg, const char *msg2);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RPM_REPORTER_H_ */
