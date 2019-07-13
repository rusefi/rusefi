/**
 * @file	engine.h
 *
 * @date May 21, 2014
 * @author Andrey Belomutskiy, (c) 2012-2019
 */
#ifndef H_ENGINE_H_
#define H_ENGINE_H_

#include "global.h"
#include "globalaccess.h"
#include "engine_state.h"
#include "rpm_calculator.h"
#include "event_registry.h"
#include "table_helper.h"
#include "listener_array.h"
#include "accel_enrichment.h"
#include "trigger_central.h"
#include "local_version_holder.h"

#if EFI_SIGNAL_EXECUTOR_ONE_TIMER
// PROD real firmware uses this implementation
#include "single_timer_executor.h"
#endif /* EFI_SIGNAL_EXECUTOR_ONE_TIMER */
#if EFI_SIGNAL_EXECUTOR_SLEEP
#include "signal_executor_sleep.h"
#endif /* EFI_SIGNAL_EXECUTOR_SLEEP */
#if EFI_UNIT_TEST
#include "global_execution_queue.h"
#endif /* EFI_UNIT_TEST */

#define MAX_INJECTION_OUTPUT_COUNT INJECTION_PIN_COUNT
#define FAST_CALLBACK_PERIOD_MS 20

/**
 * This class knows about when to inject fuel
 */
class FuelSchedule {
public:
	FuelSchedule();
	/**
	 * this method schedules all fuel events for an engine cycle
	 */
	void addFuelEvents(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	bool addFuelEventsForCylinder(int cylinderIndex DECLARE_ENGINE_PARAMETER_SUFFIX);

	InjectionEvent elements[MAX_INJECTION_OUTPUT_COUNT];
	bool isReady;

private:
	void clear();
};

class RpmCalculator;

#define MAF_DECODING_CACHE_SIZE 256

#define MAF_DECODING_CACHE_MULT (MAF_DECODING_CACHE_SIZE / 5.0)

/**
 * I am not sure if this needs to be configurable.
 *
 * Also technically the whole feature might be implemented as cranking fuel coefficient curve by TPS.
 */
// todo: not great location for these
#define CLEANUP_MODE_TPS 90
#define STEPPER_PARKING_TPS CLEANUP_MODE_TPS

class Engine {
public:
	explicit Engine(persistent_config_s *config);
	Engine();
	void setConfig(persistent_config_s *config);
	injection_mode_e getCurrentInjectionMode(DECLARE_ENGINE_PARAMETER_SIGNATURE);

	InjectionSignalPair fuelActuators[INJECTION_PIN_COUNT];
	IgnitionEventList ignitionEvents;
	LocalVersionHolder versionForConfigurationListeners;
	LocalVersionHolder auxParametersVersion;

	int globalSparkIdCoutner = 0;

#if !EFI_PROD_CODE
	float mockMapValue = 0;
	// for historical reasons we have options to mock TPS on different layers :(
	int mockTpsAdcValue = 0;
	float mockTpsValue = NAN;
#endif

	int getGlobalConfigurationVersion(void) const;
	/**
	 * true if a recent configuration change has changed any of the trigger settings which
	 * we have not adjusted for yet
	 */
	bool isTriggerConfigChanged = false;
	LocalVersionHolder triggerVersion;

	// a pointer with interface type would make this code nicer but would carry extra runtime
	// cost to resolve pointer, we use instances as a micro optimization
#if EFI_SIGNAL_EXECUTOR_ONE_TIMER
	SingleTimerExecutor executor;
#endif
#if EFI_SIGNAL_EXECUTOR_SLEEP
	SleepExecutor executor;
#endif
#if EFI_UNIT_TEST
	TestExecutor executor;
#endif

#if EFI_ENGINE_CONTROL
	FuelSchedule injectionEvents;
#endif /* EFI_ENGINE_CONTROL */

	WallFuel wallFuel;
	bool needToStopEngine(efitick_t nowNt) const;
	bool etbAutoTune = false;
	/**
	 * That's the list of pending spark firing events
	 */
	IgnitionEvent *iHead = NULL;
	/**
	 * this is based on isEngineChartEnabled and engineSnifferRpmThreshold settings
	 */
	bool isEngineChartEnabled = false;
	/**
	 * this is based on sensorChartMode and sensorSnifferRpmThreshold settings
	 */
	sensor_chart_e sensorChartMode = SC_OFF;
	/**
	 * based on current RPM and isAlternatorControlEnabled setting
	 */
	bool isAlternatorControlEnabled = false;

	bool isCltBroken = false;
	bool slowCallBackWasInvoked = false;


//	floatms_t callToPitEndTime;

	/**
	 * remote telemetry: if not zero, time to stop flashing 'CALL FROM PIT STOP' light
	 */
	efitime_t callFromPitStopEndTime = 0;

	// timestamp of most recent time RPM hard limit was triggered
	efitime_t rpmHardLimitTimestamp = 0;

	// todo: should be a field on some other class, not Engine?
	bool isInitializingTrigger = false;

	/**
	 * This flag indicated a big enough problem that engine control would be
	 * prohibited if this flag is set to true.
	 */
	bool withError = false;

	RpmCalculator rpmCalculator;
	persistent_config_s *config = NULL;
	/**
	 * we use funny unique name to make sure that compiler is not confused between global variable and class member
	 * todo: this variable is probably a sign of some problem, should we even have it?
	 */
	engine_configuration_s *engineConfigurationPtr = NULL;

	/**
	 * this is about 'stopengine' command
	 */
	efitick_t stopEngineRequestTimeNt = 0;

	/**
	 * This counter is incremented every time user adjusts ECU parameters online (either via rusEfi console or other
	 * tuning software)
	 */
	volatile int globalConfigurationVersion = 0;

	/**
	 * always 360 or 720, never zero
	 */
	angle_t engineCycle;

	AccelEnrichmemnt engineLoadAccelEnrichment;
	AccelEnrichmemnt tpsAccelEnrichment;

	TriggerCentral triggerCentral;

	/**
	 * Each individual fuel injection duration for current engine cycle, without wall wetting
	 * including everything including injector lag, both cranking and running
	 * @see getInjectionDuration()
	 */
	floatms_t injectionDuration = 0;

	/**
	 * This one with wall wetting accounted for, used for logging.
	 */
	floatms_t actualLastInjection = 0;

	void periodicFastCallback(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	void periodicSlowCallback(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	void updateSlowSensors(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	void initializeTriggerShape(Logging *logger DECLARE_ENGINE_PARAMETER_SUFFIX);

	bool clutchUpState = false;
	bool clutchDownState = false;
	bool brakePedalState = false;
	bool acSwitchState = false;

	bool isRunningPwmTest = false;


	FsioState fsioState;

	/**
	 * Are we experiencing knock right now?
	 */
	bool knockNow = false;
	/**
	 * Have we experienced knock since engine was started?
	 */
	bool knockEver = false;
	/**
     * KnockCount is directly proportional to the degrees of ignition
     * advance removed
     */
    int knockCount = 0;

    float knockVolts = 0;

    bool knockDebug = false;

	efitimeus_t timeOfLastKnockEvent = 0;

	/**
	 * are we running any kind of functional test? this affect
	 * some areas
	 */
	bool isTestMode = false;


	/**
	 * pre-calculated offset for given sequence index within engine cycle
	 * (not cylinder ID)
	 */
	angle_t ignitionPositionWithinEngineCycle[IGNITION_PIN_COUNT];
	/**
	 * pre-calculated reference to which output pin should be used for
	 * given sequence index within engine cycle
	 * todo: update documentation
	 */
	int ignitionPin[IGNITION_PIN_COUNT];

	// Store current ignition mode for prepareIgnitionPinIndices()
	ignition_mode_e ignitionModeForPinIndices = Force_4_bytes_size_ignition_mode;

	/**
	 * this is invoked each time we register a trigger tooth signal
	 */
	void onTriggerSignalEvent(efitick_t nowNt);
	EngineState engineState;
	SensorsState sensors;
	efitick_t lastTriggerToothEventTimeNt = 0;


	/**
	 * This coefficient translates ADC value directly into voltage adjusted according to
	 * voltage divider configuration with just one multiplication. This is a future (?) performance optimization.
	 */
	float adcToVoltageInputDividerCoefficient = NAN;

	/**
	 * This field is true if we are in 'cylinder cleanup' state right now
	 * see isCylinderCleanupEnabled
	 */
	bool isCylinderCleanupMode = false;

	/**
	 * value of 'triggerShape.getLength()'
	 * pre-calculating this value is a performance optimization
	 */
	uint32_t engineCycleEventCount = 0;

	void preCalculate(DECLARE_ENGINE_PARAMETER_SIGNATURE);

	void watchdog();

	/**
	 * Needed by EFI_MAIN_RELAY_CONTROL to shut down the engine correctly.
	 */
	void checkShutdown();
	
	/**
	 * Allows to finish some long-term shutdown procedures (stepper motor parking etc.)
	   Returns true if some operations are in progress on background.
	 */
	bool isInShutdownMode() const;

	monitoring_timestamps_s m;

	void knockLogic(float knockVolts DECLARE_ENGINE_PARAMETER_SUFFIX);
	void printKnockState(void);

private:
	/**
	 * By the way:
	 * 'cranking' means engine is not stopped and the rpm are below crankingRpm
	 * 'running' means RPM are above crankingRpm
	 * 'spinning' means the engine is not stopped
	 */
	bool isSpinning = false;
	void reset();
};

void prepareShapes(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void resetConfigurationExt(Logging * logger, engine_type_e engineType DECLARE_ENGINE_PARAMETER_SUFFIX);
void applyNonPersistentConfiguration(Logging * logger DECLARE_ENGINE_PARAMETER_SUFFIX);
void prepareOutputSignals(DECLARE_ENGINE_PARAMETER_SIGNATURE);

void validateConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void doScheduleStopEngine(DECLARE_ENGINE_PARAMETER_SIGNATURE);

typedef void (*configuration_callback_t)(Engine*);

#endif /* H_ENGINE_H_ */
