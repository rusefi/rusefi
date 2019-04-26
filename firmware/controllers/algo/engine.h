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
#include "pid.h"
#include "rpm_calculator.h"
#include "event_registry.h"
#include "trigger_structure.h"
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

#define MOCK_ADC_SIZE 16

class MockAdcState {
public:
	MockAdcState();
	bool hasMockAdc[MOCK_ADC_SIZE];
	int fakeAdcValues[MOCK_ADC_SIZE];

	void setMockVoltage(int hwChannel, float voltage);
	int getMockAdcValue(int hwChannel) const;
};

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

class ThermistorMath {
public:
	void setConfig(thermistor_conf_s *config);
	void prepareThermistorCurve(thermistor_conf_s *tc);
	float getKelvinTemperatureByResistance(float resistance) const;
	float s_h_a = 0;
	float s_h_b = 0;
	float s_h_c = 0;
private:
	thermistor_conf_s currentConfig = {};
};

class Accelerometer {
public:
	float x = 0; // G value
	float y = 0;
	float z = 0;
};

class SensorsState {
public:
	SensorsState();
	/**
	 * Performance optimization:
	 * log() function needed for thermistor logic is relatively heavy, to avoid it we have these
	 * pre-calculated values
	 * Access to these two fields is not synchronized in any way - that should work since float read/write are atomic.
	 *
	 * values are in Celsius
	 */
	float iat = NAN;
#if EFI_UNIT_TEST
	float mockClt = NAN;
#endif
	float clt = NAN;

	/**
	 * Oil pressure in kPa
	 */
	float oilPressure;

	Accelerometer accelerometer;

	float vBatt = 0;
	float currentAfr;
	/**
	 * that's fuel in tank - just a gauge
	 */
	percent_t fuelTankLevel = 0;
};

class FuelConsumptionState {
public:
	FuelConsumptionState();
	void addData(float durationMs);
	void update(efitick_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX);
	float perSecondConsumption = 0;
	float perMinuteConsumption = 0;
	float perSecondAccumulator = 0;
	float perMinuteAccumulator = 0;
	efitick_t accumulatedSecondPrevNt;
	efitick_t accumulatedMinutePrevNt;
};

class TransmissionState {
public:
	TransmissionState();
	gear_e gearSelectorPosition;
};

class WarningCodeState {
public:
	WarningCodeState();
	void addWarningCode(obd_code_e code);
	bool isWarningNow(efitimesec_t now, bool forIndicator DECLARE_ENGINE_PARAMETER_SUFFIX) const;
	void clear();
	int warningCounter;
	int lastErrorCode;
	efitimesec_t timeOfPreviousWarning;
	// todo: we need a way to post multiple recent warnings into TS
	cyclic_buffer<int, 8> recentWarnings;
};

class EngineState {
public:
	EngineState();
	void periodicFastCallback(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	void updateSlowSensors(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	void updateTChargeK(int rpm, float tps DECLARE_ENGINE_PARAMETER_SUFFIX);

	FuelConsumptionState fuelConsumption;

	efitick_t crankingTime = 0;
	efitick_t timeSinceCranking = 0;

	WarningCodeState warnings;

	/**
	 * speed-density logic, calculated air mass in grams
	 */
	float airMass = 0;
	/**
	 * speed-density logic, calculated air flow in kg/h for tCharge Air-Interp. method
	 */
	float airFlow = 0;

	float engineNoiseHipLevel = 0;

	float auxValveStart = 0;
	float auxValveEnd = 0;

	ThermistorMath iatCurve;
	ThermistorMath cltCurve;

	/**
	 * MAP averaging angle start, in relation to 'mapAveragingSchedulingAtIndex' trigger index index
	 */
	angle_t mapAveragingStart[INJECTION_PIN_COUNT];
	angle_t mapAveragingDuration = 0;

	// spark-related
	floatms_t sparkDwell = 0;
	angle_t timingAdvance = 0;

	/**
	 * ignition dwell duration as crankshaft angle
	 * NAN if engine is stopped
	 */
	angle_t dwellAngle = NAN;

	angle_t cltTimingCorrection = 0;

	// fuel-related;
	float iatFuelCorrection = 0;
	float cltFuelCorrection = 0;
	float postCrankingFuelCorrection = 0;
	float fuelCutoffCorrection = 0;
	efitick_t coastingFuelCutStartTime = 0;
	/**
	 * injectorLag(VBatt)
	 *
	 * this value depends on a slow-changing VBatt value, so
	 * we update it once in a while
	 */
	floatms_t injectorLag = 0;

	/**
	 * See useWarmupPidAfr
	 */
	Pid warmupAfrPid;
	float warmupTargetAfr = 0;

	float baroCorrection = 0;

	// speed density
	// Rate-of-change limiter is applied to degrees, so we store both Kelvin and degrees.
	float tCharge = 0;
	float tChargeK = 0;
	efitick_t timeSinceLastTChargeK;

	float currentVE = 0;
	float targetAFR = 0;

	int vssEventCounter = 0;
	int totalLoggedBytes = 0;


	/**
	 * pre-calculated value from simple fuel lookup
	 */
	floatms_t baseTableFuel = 0;
	/**
	 * Raw fuel injection duration produced by current fuel algorithm, without any correction
	 */
	floatms_t baseFuel = 0;

	/**
	 * closed-loop fuel correction
	 */
	floatms_t fuelPidCorrection = 0;

	/**
	 * Total fuel with CLT, IAT and TPS acceleration corrections per cycle,
	 * as squirt duration.
	 * Without injector lag.
	 * @see baseFuel
	 * @see actualLastInjection
	 */
	floatms_t runningFuel = 0;

	/**
	 * TPS acceleration: extra fuel amount
	 */
	floatms_t tpsAccelEnrich = 0;

	angle_t injectionOffset = 0;

#if EFI_ENABLE_MOCK_ADC
	MockAdcState mockAdcState;
#endif /* EFI_ENABLE_MOCK_ADC */
};

class RpmCalculator;

#define MAF_DECODING_CACHE_SIZE 256

#define MAF_DECODING_CACHE_MULT (MAF_DECODING_CACHE_SIZE / 5.0)

typedef struct {
	uint32_t beforeMainTrigger;
	uint32_t mainTriggerCallbackTime;

	uint32_t beforeIgnitionSch;
	uint32_t ignitionSchTime;

	uint32_t beforeInjectonSch;
	uint32_t injectonSchTime;

	uint32_t beforeZeroTest;
	uint32_t zeroTestTime;

	uint32_t beforeAdvance;
	uint32_t advanceLookupTime;

	uint32_t beforeFuelCalc;
	uint32_t fuelCalcTime;

	uint32_t beforeMapAveragingCb;
	uint32_t mapAveragingCbTime;

	uint32_t beforeHipCb;
	uint32_t hipCbTime;

	uint32_t beforeRpmCb;
	uint32_t rpmCbTime;
} monitoring_timestamps_s;

class FsioState {
public:
	FsioState();
	float fsioTimingAdjustment = 0;
	float fsioIdleTargetRPMAdjustment = 0;
	float servoValues[SERVO_COUNT];
	float fsioLastValue[FSIO_COMMAND_COUNT];

#if EFI_ENABLE_ENGINE_WARNING
	/**
	 * Shall we purposely miss on some cylinders in order to attract driver's attention to some problem
	 * like getting too hot
	 */
	float isEngineWarning;
#endif /* EFI_ENABLE_ENGINE_WARNING */

#if EFI_ENABLE_CRITICAL_ENGINE_STOP
	/**
	 * Shall we stop engine due to some critical condition in order to save the engine
	 */
	float isCriticalEngineCondition;
#endif /* EFI_ENABLE_CRITICAL_ENGINE_STOP */
};

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
	Engine(persistent_config_s *config);
	Engine();
	void setConfig(persistent_config_s *config);
	injection_mode_e getCurrentInjectionMode(DECLARE_ENGINE_PARAMETER_SIGNATURE);

	InjectionSignalPair fuelActuators[INJECTION_PIN_COUNT];
	IgnitionEventList ignitionEvents;
	LocalVersionHolder versionForConfigurationListeners;
	LocalVersionHolder auxParametersVersion;

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
	bool needToStopEngine(efitick_t nowNt);
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
	 * This counter is incremented every time user adjusts ECU parameters online (either via dev console or other
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
	 * fuel injection time correction to account for wall wetting effect, for current cycle
	 */
	floatms_t wallFuelCorrection = 0;

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

	/**
	 * fast spark dwell time interpolation helper
	 * todo: finish the implementation and
	 */
	Table2D<DWELL_CURVE_SIZE> sparkTable;

	/**
	 * fast kg/hour MAF decoding lookup table with ~0.2 volt step
	 * This table is build based on MAF decoding curve
	 */
	float mafDecodingLookup[MAF_DECODING_CACHE_SIZE];

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
	bool isInShutdownMode();

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

/**
 * 6 crossing over 50% TPS means pressing and releasing three times
 */
#define PUMPS_TO_PRIME 6

class StartupFuelPumping {
public:
	StartupFuelPumping();
	void update(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	bool isTpsAbove50;
	int pumpsCounter;
private:
	void setPumpsCounter(int newValue);
};

void prepareShapes(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void resetConfigurationExt(Logging * logger, engine_type_e engineType DECLARE_ENGINE_PARAMETER_SUFFIX);
void applyNonPersistentConfiguration(Logging * logger DECLARE_ENGINE_PARAMETER_SUFFIX);
void prepareOutputSignals(DECLARE_ENGINE_PARAMETER_SIGNATURE);

void validateConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void doScheduleStopEngine(DECLARE_ENGINE_PARAMETER_SIGNATURE);

typedef void (*configuration_callback_t)(Engine*);

#endif /* H_ENGINE_H_ */
