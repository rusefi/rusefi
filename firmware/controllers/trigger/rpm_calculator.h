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

/**
 * The engine state stored in RpmCalculator. Accessed via isStopped(), isCranking(), isRunning().
 */
typedef enum {
	/**
	 * The engine is not spinning, RPM=0
	 */
	STOPPED,
	/**
	 * The engine is cranking (RPM < cranking.rpm)
	 */
	CRANKING,
	/**
	 * The engine is running (RPM >= cranking.rpm)
	 */
	RUNNING,
} engine_state_e;

class Engine;

class RpmCalculator {
public:
#if !EFI_PROD_CODE
	int mockRpm;
#endif
	RpmCalculator();
	/**
	 * Returns true if the engine is not spinning (RPM==0)
	 */
	bool isStopped();
	/**
	 * Returns true if the engine is cranking
	 */
	bool isCranking();
	/**
	 * Returns true if the engine is running and not cranking
	 */
	bool isRunning();
	/**
	 * Check if there was a full shaft revolution within the last second (and returns true if so)
	 * Please note that this is a relatively heavy method due to getTimeNowNt() usage
	 */
	bool checkIfSpinning(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	/**
	 * This is a fast accessor method.
	 */
	int getRpm(void);
	/**
	 * This method is invoked once per engine cycle right after we calculate new RPM value
	 */
	void onNewEngineCycle();
	uint32_t getRevolutionCounter(void);
	void setRpmValue(int value DECLARE_ENGINE_PARAMETER_SUFFIX);
	uint32_t getRevolutionCounterSinceStart(void);
	float getRpmAcceleration();
	/**
	 * This is a performance optimization: let's pre-calulate this each time RPM changes
	 */
	volatile floatus_t oneDegreeUs;
	volatile efitime_t lastRpmEventTimeNt;
private:
	void assignRpmValue(int value);
	/**
	 * Called from checkIfSpinning() if no revolutions occurred.
	 */
	void stopRunning(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	/**
	 * Update engine state
	 */
	void updateState(DECLARE_ENGINE_PARAMETER_SIGNATURE);

	/**
	 * This counter is incremented with each revolution of one of the shafts. Could be
	 * crankshaft could be camshaft.
	 */
	volatile uint32_t revolutionCounterSinceBoot;
	/**
	 * Same as the above, but since the engine started spinning
	 */
	volatile uint32_t revolutionCounterSinceStart;
	/**
	 * Zero if engine is not running
	 */
	volatile int rpmValue;
	int previousRpmValue;

	/**
	 * Current engine state
	 */
	engine_state_e state;
};

/**
 * @brief   Current RPM
 */
#define getRpmE(engine) (engine)->rpmCalculator.getRpm()

void rpmShaftPositionCallback(trigger_event_e ckpSignalType, uint32_t index DECLARE_ENGINE_PARAMETER_SUFFIX);
/**
 * @brief   Initialize RPM calculator
 */
void initRpmCalculator(Logging *sharedLogger, Engine *engine);

float getCrankshaftAngleNt(efitime_t timeNt DECLARE_ENGINE_PARAMETER_SUFFIX);
#endif


int getRevolutionCounter(void);

#define isValidRpm(rpm) ((rpm) > 0 && (rpm) < UNREALISTIC_RPM)

#if EFI_ENGINE_SNIFFER
#define addEngineSniffferEvent(name, msg) waveChart.addEvent3((name), (msg))
 #else
#define addEngineSniffferEvent(n, msg) {}
#endif /* EFI_ENGINE_SNIFFER */

void scheduleByAngle(int rpm, scheduling_s *timer, angle_t angle, schfunc_t callback, void *param, RpmCalculator *calc);

#endif /* RPM_REPORTER_H_ */
