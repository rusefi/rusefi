/**
 * @file	engine.h
 *
 * @date May 21, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */
#ifndef ENGINE_H_
#define ENGINE_H_

#include "main.h"
#include "engine_configuration.h"
#include "ec2.h"
#include "rpm_calculator.h"

class EngineState {
public:
	/**
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

	uint64_t stopEngineRequestTimeNt;

	Thermistor iat;
	Thermistor clt;

	float dwellAngle;
	float advance;

	trigger_shape_s triggerShape;

	float angleExtra[IGNITION_PIN_COUNT];
	io_pin_e ignitionPin[IGNITION_PIN_COUNT];

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

	uint32_t time2;
	uint32_t time3;
	uint32_t time4;
	uint32_t time5;
	uint32_t time6;

	uint32_t before2;
	uint32_t before3;
	uint32_t before4;
	uint32_t before5;
	uint32_t before6;

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
