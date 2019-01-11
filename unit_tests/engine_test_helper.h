/**
 * @file	engine_test_helper.h
 *
 * @date Jun 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */
#ifndef ENGINE_TEST_HELPER_H_
#define ENGINE_TEST_HELPER_H_

#include "engine.h"
#include "trigger_central.h"
#include "rpm_calculator.h"
#include "main_trigger_callback.h"
#include "unit_test_framework.h"


/**
 * Mock engine with trigger signal simulation infrastructure
 */
class EngineTestHelper {
public:
	EngineTestHelper(engine_type_e engineType);
	void applyTriggerShape();
	void fireRise(int delayMs);
	void fireFall(int delayMs);

	void firePrimaryTriggerRise();
	void firePrimaryTriggerFall();
	void fireTriggerEvents(int count);
	void fireTriggerEventsWithDuration(int delayMs);
	void fireTriggerEvents2(int count, int delayMs);
	void clearQueue();

	scheduling_s * assertEvent5(TestExecutor *executor, const char *msg, int index, void *callback, efitime_t start, efitime_t momentX);
	void assertEvent(TestExecutor *executor, const char *msg, int index, void *callback, efitime_t start, efitime_t momentX, long param);
	void assertInjectorUpEvent(const char *msg, int eventIndex, efitime_t momentX, long injectorIndex);
	void assertInjectorDownEvent(const char *msg, int eventIndex, efitime_t momentX, long injectorIndex);

	int executeActions();
	void moveTimeForwardUs(int deltaTimeUs);
	efitimeus_t getTimeNowUs(void);

	Engine engine;
	persistent_config_s persistentConfig;
};

#endif /* ENGINE_TEST_HELPER_H_ */
