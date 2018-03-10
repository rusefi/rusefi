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

class EngineTestHelper {
public:
	EngineTestHelper(engine_type_e engineType);
	void applyTriggerShape();
	void firePrimaryTriggerRise();
	void firePrimaryTriggerFall();
	void fireTriggerEvents(int count);
	void fireTriggerEvents2(int count, int durationUs);
	void clearQueue();

	persistent_config_s persistentConfig;
	Engine engine;

	engine_configuration_s *ec;
	engine_configuration_s *engineConfiguration;
};

#endif /* ENGINE_TEST_HELPER_H_ */
