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

	warningBuffer_t *recentWarnings();
	int getWarningCounter();

	void applyTriggerWaveform();
	void setTriggerType(trigger_type_e trigger DECLARE_ENGINE_PARAMETER_SUFFIX);
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
	void executeUntil(int timeUs);
	void moveTimeForwardAndInvokeEventsSec(int deltaTimeSeconds);
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

	scheduling_s * assertEvent5(const char *msg, int index, void *callback, efitime_t expectedTimestamp);
	scheduling_s * assertScheduling(const char *msg, int index, scheduling_s *expected, void *callback, efitime_t expectedTimestamp);

	AngleBasedEvent * assertTriggerEvent(const char *msg, int index, AngleBasedEvent *expected, void *callback, int triggerEventIndex, angle_t angleOffsetFromTriggerEvent);

	void assertEvent(const char *msg, int index, void *callback, efitime_t momentX, InjectionEvent *event);
	void assertInjectorUpEvent(const char *msg, int eventIndex, efitime_t momentX, long injectorIndex);
	void assertInjectorDownEvent(const char *msg, int eventIndex, efitime_t momentX, long injectorIndex);
	// todo: open question if this is worth a helper method or should be inlined?
	void assertRpm(int expectedRpm, const char *msg);

	int executeActions();
	void moveTimeForwardMs(float deltaTimeMs);
	void moveTimeForwardSec(float deltaTimeSec);
	efitimeus_t getTimeNowUs(void);

	Engine engine;
	persistent_config_s persistentConfig;

	::testing::NiceMock<MockAirmass> mockAirmass;

private:
	void writeEvents(const char *fileName);
};

void setupSimpleTestEngineWithMafAndTT_ONE_trigger(EngineTestHelper *eth, injection_mode_e injMode = IM_BATCH);
void setupSimpleTestEngineWithMaf(EngineTestHelper *eth, injection_mode_e injectionMode, trigger_type_e trigger);

void setVerboseTrigger(bool isEnabled);
