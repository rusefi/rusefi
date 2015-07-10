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

class EngineTestHelper {
public:
	EngineTestHelper(engine_type_e engineType);
	void initTriggerShapeAndRpmCalculator();
	void fireTriggerEvents();

	persistent_config_s persistentConfig;
	engine_configuration2_s ec2;
	Engine engine;

	engine_configuration_s *ec;
	engine_configuration_s *engineConfiguration;

	TriggerCentral triggerCentral;
};

#endif /* ENGINE_TEST_HELPER_H_ */
