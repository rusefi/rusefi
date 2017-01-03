/**
 * @file    rpm_calculator.h
 * @brief   Shaft position sensor(s) decoder header
 *
 * @date Jan 1, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef RPM_REPORTER_H_
#define RPM_REPORTER_H_

#include <global.h>
#include "engine_configuration.h"
#include "scheduler.h"

#define TOP_DEAD_CENTER_MESSAGE "r"

#define WC_DOWN "d"
#define WC_UP "u"
/**
 * See also 'CRANK1' in java dev console code
 */
#define CRANK1 "t1"
#define CRANK2 "t2"
#define CRANK3 "t3"

#define VVT_NAME "VVT"

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
	int getRpm(DECLARE_ENGINE_PARAMETER_F);
	/**
	 * This method is invoked once per engine cycle right after we calculate new RPM value
	 */
	void onNewEngineCycle();
	uint32_t getRevolutionCounter(void);
	void setRpmValue(int value DECLARE_ENGINE_PARAMETER_S);
	uint32_t getRevolutionCounterSinceStart(void);
	float getRpmAcceleration();
	/**
	 * This is public because sometimes we cannot afford to call isRunning() and the value is good enough
	 * Zero if engine is not running
	 */
	volatile int rpmValue;
	int previousRpmValue;
	/**
	 * This is a performance optimization: let's pre-calulate this each time RPM changes
	 */
	volatile floatus_t oneDegreeUs;
	volatile efitime_t lastRpmEventTimeNt;
private:
	void assignRpmValue(int value);
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

/**
 * @brief   Current RPM
 */
#define getRpmE(engine) (engine)->rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_F)

bool isCrankingE(Engine *engine);
void rpmShaftPositionCallback(trigger_event_e ckpSignalType, uint32_t index DECLARE_ENGINE_PARAMETER_S);
/**
 * @brief   Initialize RPM calculator
 */
void initRpmCalculator(Logging *sharedLogger, Engine *engine);

float getCrankshaftAngleNt(efitime_t timeNt DECLARE_ENGINE_PARAMETER_S);
#endif


int getRevolutionCounter(void);

bool isCranking(void);

#define isValidRpm(rpm) ((rpm) > 0 && (rpm) < UNREALISTIC_RPM)

#if EFI_ENGINE_SNIFFER
#define addEngineSniffferEvent(name, msg) waveChart.addEvent3((name), (msg))
 #else
#define addEngineSniffferEvent(n, msg) {}
#endif /* EFI_ENGINE_SNIFFER */

void scheduleByAngle(int rpm, scheduling_s *timer, angle_t angle, schfunc_t callback, void *param, RpmCalculator *calc);

#endif /* RPM_REPORTER_H_ */
