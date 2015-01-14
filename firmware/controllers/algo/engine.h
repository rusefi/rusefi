/**
 * @file	engine.h
 *
 * @date May 21, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */
#ifndef ENGINE_H_
#define ENGINE_H_

#include "main.h"
#include "engine_configuration.h"
#include "rpm_calculator.h"

#include "global.h"
#include "engine_configuration.h"
#include "event_registry.h"
#include "trigger_structure.h"
#include "table_helper.h"

/**
 * This class knows about when to inject fuel
 */
class FuelSchedule {
public:
	FuelSchedule();
	ActuatorEventList events;

	/**
	 * this method schedules all fuel events for an engine cycle
	 */
	void addFuelEvents(injection_mode_e mode DECLARE_ENGINE_PARAMETER_S);

	/**
	 * This is a performance optimization for https://sourceforge.net/p/rusefi/tickets/64/
	 * TODO: better data structure? better algorithm?
	 */
	uint8_t hasEvents[PWM_PHASE_MAX_COUNT];
private:
	void clear();
	void registerInjectionEvent(NamedOutputPin *output, float angle, bool_t isSimultanious DECLARE_ENGINE_PARAMETER_S);
};

/**
 * this part of the structure is separate just because so far
 * these fields are not integrated with Tuner Studio. Step by step :)
 */
class engine_configuration2_s {
	// todo: move these fields into Engine class, eliminate this class
public:
	engine_configuration2_s();

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
	FuelSchedule crankingInjectionEvents;
	FuelSchedule injectionEvents;
#endif

	float fsioLastValue[LE_COMMAND_COUNT];

	/**
	 * We are alternating two event lists in order to avoid a potential issue around revolution boundary
	 * when an event is scheduled within the next revolution.
	 */
	IgnitionEventList ignitionEvents[2];
};


class EngineState {
public:
	/**
	 * WIP: accessing these values here would be a performance optimization since log() function needed for
	 * thermistor logic is relatively heavy
	 * Access to these two fields is not synchronized in any way - that should work since float read/write are atomic.
	 */
	float iat;
	float clt;
};

class RpmCalculator;

class Engine {
public:
	Engine();
	void init();
	RpmCalculator rpmCalculator;
	engine_configuration_s *engineConfiguration;
	engine_configuration2_s *engineConfiguration2;

	/**
	 * this is about 'stopengine' command
	 */
	uint64_t stopEngineRequestTimeNt;

	Thermistor iat;
	Thermistor clt;

	/**
	 * ignition dwell duration as crankshaft angle
	 */
	angle_t dwellAngle;
	angle_t advance;

	bool_t clutchUpState;
	bool_t clutchDownState;

	TriggerShape triggerShape;

	float angleExtra[IGNITION_PIN_COUNT];
	NamedOutputPin *ignitionPin[IGNITION_PIN_COUNT];

	void onTriggerEvent(uint64_t nowNt);
	EngineState engineState;
	uint64_t lastTriggerEventTimeNt;

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
	 * pre-calculating this value is a performance optimization
	 */
	int engineCycleEventCount;

	uint32_t beforeIgnitionMath;
	uint32_t ignitionMathTime;

	uint32_t beforeIgnitionSch;
	uint32_t ignitionSchTime;

	float sparkAtable[DWELL_CURVE_SIZE];
	float sparkBtable[DWELL_CURVE_SIZE];

	Table2D sparkTable;
	void precalc(engine_configuration_s *engineConfiguration);

	void updateSlowSensors();
	void watchdog();
private:
	/**
	 * By the way:
	 * 'cranking' means engine is not stopped and the rpm are below crankingRpm
	 * 'running' means RPM are above crankingRpm
	 * 'spinning' means the engine is not stopped
	 */
	bool isSpinning;
	bool stopPins();
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

void prepareShapes(Engine *engine);
void resetConfigurationExt(Logging * logger, engine_type_e engineType,
		Engine *engine);
void applyNonPersistentConfiguration(Logging * logger, Engine *engine);
void prepareOutputSignals(DECLARE_ENGINE_PARAMETER_F);

#endif /* ENGINE_H_ */
