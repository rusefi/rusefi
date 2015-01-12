/**
 * @file    rpm_calculator.h
 * @brief   Shaft position sensor(s) decoder header
 *
 * @date Jan 1, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef RPM_REPORTER_H_
#define RPM_REPORTER_H_

#include <global.h>
#include "engine_configuration.h"

#define WC_DOWN "d"
#define WC_UP "u"
#define WC_CRANK1 "c1"
#define WC_CRANK2 "c2"
#define WC_CRANK3 "c3"

#define NOISY_RPM -1
#define UNREALISTIC_RPM 30000

#ifdef __cplusplus

class Engine;

class RpmCalculator {
public:
#if !EFI_PROD_CODE
	int mockRpm;
#endif
	RpmCalculator();
	/**
	 * Please note that this is a relatively heavy method due to getTimeNowNt() usage
	 */
	bool isRunning(DECLARE_ENGINE_PARAMETER_F);
	int rpm(DECLARE_ENGINE_PARAMETER_F);
	void onNewEngineCycle();
	uint32_t getRevolutionCounter(void);
	void setRpmValue(int value);
	uint32_t getRevolutionCounterSinceStart(void);
	/**
	 * This is public because sometimes we cannot afford to call isRunning() and the value is good enough
	 */
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
#define getRpmE(engine) (engine)->rpmCalculator.rpm(PASS_ENGINE_PARAMETER_F)

bool isCrankingE(Engine *engine);
void rpmShaftPositionCallback(trigger_event_e ckpSignalType, uint32_t index DECLARE_ENGINE_PARAMETER_S);
/**
 * @brief   Initialize RPM calculator
 */
void initRpmCalculator(Engine *engine);

float getCrankshaftAngleNt(uint64_t timeNt DECLARE_ENGINE_PARAMETER_S);
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

bool isCranking(void);

int getRevolutionCounter(void);

#define isValidRpm(rpm) ((rpm) > 0 && (rpm) < UNREALISTIC_RPM)

#if EFI_WAVE_CHART
#define addWaveChartEvent(name, msg) waveChart.addWaveChartEvent3((name), (msg))
 #else
#define addWaveChartEvent(n, msg) {}
#endif /* EFI_WAVE_CHART */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RPM_REPORTER_H_ */
