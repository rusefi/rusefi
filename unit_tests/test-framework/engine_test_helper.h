/**
 * @file	engine_test_helper.h
 *
 * @date Jun 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "trigger_central.h"
#include "main_trigger_callback.h"
#include "unit_test_framework.h"
#include "engine.h"

#include <unordered_map>

extern EnginePins enginePins;

class EngineTestHelperBase
{
public:
	// we have the base method and base constructor in order to better control order if initialization
	// base constructor contains things which need to be executed first
	EngineTestHelperBase(Engine * eng, engine_configuration_s * config, persistent_config_s * pers);
	~EngineTestHelperBase();
};

/**
 * Mock engine with trigger signal simulation infrastructure
 */
class EngineTestHelper : public EngineTestHelperBase {
public:
	explicit EngineTestHelper(engine_type_e engineType);
	EngineTestHelper(engine_type_e engineType, const std::unordered_map<SensorType, float>& sensorValues);
	EngineTestHelper(engine_type_e engineType, configuration_callback_t boardCallback);
	EngineTestHelper(engine_type_e engineType, configuration_callback_t boardCallback, const std::unordered_map<SensorType, float>& sensorValues);
	~EngineTestHelper();

  // convert ms time to angle at current RPM
  angle_t timeToAngle(float timeMs);
  float angleToTimeUs(angle_t angle) {
    return angle * engine.rpmCalculator.oneDegreeUs;
  }

  float angleToTimeMs(angle_t angle) {
    return US2MS(angleToTimeUs(angle));
  }

	warningBuffer_t *recentWarnings();
	int getWarningCounter();

	void applyTriggerWaveform();
	void setTriggerType(trigger_type_e trigger);
	/**
	 * DEPRECATED these methods do not execute events on the queue
	 */
	void fireRise(float delayMs);
	void fireFall(float delayMs);
	void moveTimeForwardUs(int deltaTimeUs);
	void fireTriggerEvents2(int count, float delayMs);

	/**
	 * these methods execute events while moving time forward
	 * todo: better naming convention?
	 */
	void smartFireRise(float delayMs);
	void smartFireFall(float delayMs);
	void moveTimeForwardAndInvokeEventsUs(int deltaTimeUs);
	void setTimeAndInvokeEventsUs(int timeNowUs);
	void moveTimeForwardAndInvokeEventsSec(int deltaTimeSeconds);
	/**
	 * both Rise and Fall
	 */
	void smartFireTriggerEvents2(int count, float delayMs);

	/**
	 * See also #fireRise() which would also move time forward
	 */
	void firePrimaryTriggerRise();
	/**
	 * See also #fireFall() which would also move time forward
	 */
	void firePrimaryTriggerFall();
	void fireTriggerEvents(int count);
	void fireTriggerEventsWithDuration(float delayMs);
	/**
	 * todo: better method name since this method executes events in the FUTURE
	 * looks like such a method should be used only in some pretty narrow circumstances
	 * a healthy test should probably use executeActions instead?
	 */
	void clearQueue();

	scheduling_s * assertEvent5(const char *msg, int index, action_s const& action, efitimeus_t expectedTimestamp);
	scheduling_s * assertScheduling(const char *msg, int index, scheduling_s *expected, action_s const& action, efitimeus_t expectedTimestamp);

	const AngleBasedEvent* assertTriggerEvent(const char *msg, int index, AngleBasedEvent *expected, action_s const& action, angle_t enginePhase);

	void assertEvent(const char *msg, int index, action_s const& action, efitimeus_t momentUs, InjectionEvent *event);
	void assertInjectorUpEvent(const char *msg, int eventIndex, efitimeus_t momentUs, long injectorIndex);
	void assertInjectorDownEvent(const char *msg, int eventIndex, efitimeus_t momentUs, long injectorIndex);
	// todo: open question if this is worth a helper method or should be inlined?
	void assertRpm(int expectedRpm, const char *msg = "RPM");

	// read all scheluder queue and search for the requested callback, then asserts the expected angle, return true if we found the callback
	bool assertEventExistsAtEnginePhase(const char *msg, action_s const& action, angle_t expectedEventEnginePhase);

	// spins the engine using 60-2 trigger pattern, at target RPM, by X crank degree, not engine phase degree
	void spin60_2UntilDeg(struct testSpinEngineUntilData& spinInfo, int targetRpm, float targetDegree);

	int executeActions();
	void moveTimeForwardMs(float deltaTimeMs);
	void moveTimeForwardSec(float deltaTimeSec);

	Engine engine;
	persistent_config_s persistentConfig;

	std::unique_ptr<::testing::NiceMock<MockAirmass>> mockAirmass;

private:
	void writeEventsLogicData(const char *fileName);
	void writeEvents2(const char *fileName);
};

void setupSimpleTestEngineWithMafAndTT_ONE_trigger(EngineTestHelper *eth, injection_mode_e injMode = IM_BATCH);
void setupSimpleTestEngineWithMaf(EngineTestHelper *eth, injection_mode_e injectionMode, trigger_type_e trigger);

void setVerboseTrigger(bool isEnabled);

warningBuffer_t * getRecentWarnings();

// used by EngineTestHelper::spin60_2UntilDeg func
struct testSpinEngineUntilData {
	float currentDegree;
	int currentTooth;
	int toothCount;
};