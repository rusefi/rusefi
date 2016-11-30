/**
 * @file	engine.h
 *
 * @date May 21, 2014
 * @author Andrey Belomutskiy, (c) 2012-2016
 */
#ifndef H_ENGINE_H_
#define H_ENGINE_H_

#include "global.h"
#include "main.h"
#include "pid.h"
#include "engine_configuration.h"
#include "rpm_calculator.h"
#include "engine_configuration.h"
#include "event_registry.h"
#include "trigger_structure.h"
#include "table_helper.h"
#include "listener_array.h"
#include "accel_enrichment.h"
#include "trigger_central.h"

#define MOCK_ADC_SIZE 16

class MockAdcState {
public:
	MockAdcState();
	bool hasMockAdc[MOCK_ADC_SIZE];
	int fakeAdcValues[MOCK_ADC_SIZE];

	void setMockVoltage(int hwChannel, float voltage);
	int getMockAdcValue(int hwChannel);
};

/**
 * This class knows about when to inject fuel
 */
class FuelSchedule {
public:
	FuelSchedule();
	/**
	 * this method schedules all fuel events for an engine cycle
	 */
	void addFuelEvents(injection_mode_e mode DECLARE_ENGINE_PARAMETER_S);

	uint32_t usedAtEngineCycle;

	InjectionEventList injectionEvents;

	/**
	 * This is a performance optimization for https://sourceforge.net/p/rusefi/tickets/64/
	 * TODO: better data structure? better algorithm?
	 */
	uint8_t hasEvents[PWM_PHASE_MAX_COUNT];
	/**
	 * How many trigger events have injection? This depends on fuel strategy & trigger shape
	 */
	int eventsCount;
private:
	void clear();
	void registerInjectionEvent(int injectorIndex, float angle, angle_t injectionDuration, bool isSimultanious DECLARE_ENGINE_PARAMETER_S);
};

/**
 * This structure is still separate from Engine simply because this goes into CCM memory and Engine is in main memory
 * todo: re-arrange global variables to put something else into CCM so that this can go into main
 * so that this could be mergeed into Engine
 * todo: move these fields into Engine class, eliminate this class
 */
class engine_configuration2_s {
public:
	engine_configuration2_s();

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
	/**
	 * Lock-free multithreading: two instances, while one is being modified another one is used read-only
	 */
	FuelSchedule injectionEvents0;
	FuelSchedule injectionEvents1;
	/**
	 * this points at an instance we use to run the engine
	 */
	FuelSchedule *injectionEvents;
	/**
	 * this variable is pointing at the instance which is being modified
	 */
	FuelSchedule *processing;
#endif

	OutputSignal fuelActuators[INJECTION_PIN_COUNT];
	OutputSignalPair overlappingFuelActuator[INJECTION_PIN_COUNT];

	bool wasOverlapping[INJECTION_PIN_COUNT];

	float fsioLastValue[LE_COMMAND_COUNT];

	IgnitionEventList ignitionEvents;
};

class ThermistorMath {
public:
	ThermistorMath();
	thermistor_curve_s curve;
	void init(thermistor_conf_s *config);
private:
	thermistor_conf_s currentConfig;
};

class EngineState {
public:
	EngineState();
	void periodicFastCallback(DECLARE_ENGINE_PARAMETER_F);
	void updateSlowSensors(DECLARE_ENGINE_PARAMETER_F);

	/**
	 * Performance optimization:
	 * log() function needed for thermistor logic is relatively heavy, to avoid it we have these
	 * pre-calculated values
	 * Access to these two fields is not synchronized in any way - that should work since float read/write are atomic.
	 *
	 * values are in Celsius
	 */
	float iat;
	float clt;

	int warningCounter;
	int lastErrorCode;
	efitimesec_t timeOfPreviousWarning;

	float airMass;

	float engineNoiseHipLevel;

	/**
	 * that's fuel in tank - just a gauge
	 */
	percent_t fuelTankGauge;

	ThermistorMath iatCurve;
	ThermistorMath cltCurve;

	/**
	 * MAP averaging angle start, in relation to 'mapAveragingSchedulingAtIndex' trigger index index
	 */
	angle_t mapAveragingStart[INJECTION_PIN_COUNT];
	angle_t mapAveragingDuration;

	// spark-related
	floatms_t sparkDwell;
	angle_t timingAdvance;

	/**
	 * ignition dwell duration as crankshaft angle
	 */
	angle_t dwellAngle;

	// fuel-related;
	float iatFuelCorrection;
	float cltFuelCorrection;
	/**
	 * Global injector lag + injectorLag(VBatt)
	 *
	 * this value depends on a slow-changing VBatt value, so
	 * we update it once in a while
	 */
	floatms_t injectorLag;

	/**
	 * See useWarmupPidAfr
	 */
	Pid warmupAfrPid;
	float warmupTargetAfr;

	float baroCorrection;

	// speed density
	float tChargeK;
	float currentVE;
	float targetAFR;

	/**
	 * pre-calculated value from simple fuel lookup
	 */
	floatms_t baseTableFuel;
	/**
	 * fuel injection duration regardless of fuel logic mode
	 */
	floatms_t baseFuel;

	/**
	 * Total fuel with CLT, IAT and TPS acceleration corrections per cycle,
	 * as squirt duration.
	 * Without injector lag.
	 * @see baseFuel
	 * @see actualLastInjection
	 */
	floatms_t runningFuel;

	/**
	 * TPS acceleration: extra fuel amount
	 */
	floatms_t tpsAccelEnrich;

	angle_t injectionOffset;

#if EFI_ENABLE_MOCK_ADC || defined(__DOXYGEN__)
	MockAdcState mockAdcState;
#endif /* EFI_ENABLE_MOCK_ADC */
};

class RpmCalculator;

#define MAF_DECODING_CACHE_SIZE 256

#define MAF_DECODING_CACHE_MULT (MAF_DECODING_CACHE_SIZE / 5.0)

typedef struct {
	uint32_t beforeMainTrigger;
	uint32_t mainTriggerCallbackTime;

	uint32_t beforeIgnitionMath;
	uint32_t ignitionMathTime;

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

class Engine;
class WallFuel;

typedef void (*configuration_callback_t)(Engine*);

class Engine {
public:
	Engine(persistent_config_s *config);
	void init(persistent_config_s *config);
	void prepareFuelSchedule(DECLARE_ENGINE_PARAMETER_F);
	IgnitionEventList *ignitionList(); // todo: inline/rename/refactor

	WallFuel wallFuel;

	/**
	 * we have a background thread preparing new fuel schedule while engine is running using existing
	 * copy of fuel schedule. This pointer allows us to use the same schedule for the whole duration of an
	 * engine cycle.
	 *
	 */
	FuelSchedule *fuelScheduleForThisEngineCycle;

	/**
	 * That's the list of pending spark firing events
	 */
	IgnitionEvent *iHead;
	/**
	 * this is based on isEngineChartEnabled and engineSnifferRpmThreshold settings
	 */
	bool isEngineChartEnabled;
	/**
	 * this is based on sensorChartMode and sensorSnifferRpmThreshold settings
	 */
	sensor_chart_e sensorChartMode;
	/**
	 * based on current RPM and isAlternatorControlEnabled setting
	 */
	bool isAlternatorControlEnabled;

	RpmCalculator rpmCalculator;
	persistent_config_s *config;
	engine_configuration_s *engineConfiguration;
	engine_configuration2_s *engineConfiguration2;

	/**
	 * this is about 'stopengine' command
	 */
	efitick_t stopEngineRequestTimeNt;

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
	 */
	floatms_t fuelMs;
	/**
	 * fuel injection time correction to account for wall wetting effect, for current cycle
	 */
	floatms_t wallFuelCorrection;

	/**
	 * This one with wall wetting accounted for, used for logging.
	 */
	floatms_t actualLastInjection;

	void periodicFastCallback(DECLARE_ENGINE_PARAMETER_F);
	void updateSlowSensors(DECLARE_ENGINE_PARAMETER_F);

	bool clutchUpState;
	bool clutchDownState;

	bool isRunningPwmTest;

	/**
	 * Are we experiencing knock right now?
	 */
	bool knockNow;
	/**
	 * Have we experienced knock since engine was started?
	 */
	bool knockEver;
	/**
     * KnockCount is directly proportional to the degrees of ignition
     * advance removed
     */
    int knockCount;

    float knockVolts;

    bool knockDebug;

	efitimeus_t timeOfLastKnockEvent;

	/**
	 * are we running any kind of functional test? this affect
	 * some areas
	 */
	bool isTestMode;

	TriggerShape triggerShape;

	/**
	 * pre-calculated offset for given sequence index within engine cycle
	 * (not cylinder ID)
	 * todo: better name?
	 */
	angle_t angleExtra[IGNITION_PIN_COUNT];
	/**
	 * pre-calculated reference to which output pin should be used for
	 * given sequence index within engine cycle
	 * todo: update documentation
	 */
	int ignitionPin[IGNITION_PIN_COUNT];

	void onTriggerEvent(efitick_t nowNt);
	EngineState engineState;
	efitick_t lastTriggerEventTimeNt;


	/**
	 * This coefficient translates ADC value directly into voltage adjusted according to
	 * voltage divider configuration. This is a future (?) performance optimization.
	 */
	float adcToVoltageInputDividerCoefficient;

	/**
	 * This field is true if we are in 'cylinder cleanup' state right now
	 * see isCylinderCleanupEnabled
	 */
	bool isCylinderCleanupMode;

	/**
	 * value of 'triggerShape.getLength()'
	 * pre-calculating this value is a performance optimization
	 */
	int engineCycleEventCount;

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

	void preCalculate();

	void watchdog();

	monitoring_timestamps_s m;

	void knockLogic(float knockVolts);
	void printKnockState(void);

private:
	/**
	 * By the way:
	 * 'cranking' means engine is not stopped and the rpm are below crankingRpm
	 * 'running' means RPM are above crankingRpm
	 * 'spinning' means the engine is not stopped
	 */
	bool isSpinning;
};

/**
 * 6 crossing over 50% TPS means pressing and releasing three times
 */
#define PUMPS_TO_PRIME 6

class StartupFuelPumping {
public:
	StartupFuelPumping();
	void update(DECLARE_ENGINE_PARAMETER_F);
	bool isTpsAbove50;
	int pumpsCounter;
private:
	void setPumpsCounter(engine_configuration_s *engineConfiguration, int newValue);
};

void prepareShapes(DECLARE_ENGINE_PARAMETER_F);
void resetConfigurationExt(Logging * logger, engine_type_e engineType DECLARE_ENGINE_PARAMETER_S);
void applyNonPersistentConfiguration(Logging * logger DECLARE_ENGINE_PARAMETER_S);
void prepareOutputSignals(DECLARE_ENGINE_PARAMETER_F);

#endif /* H_ENGINE_H_ */
