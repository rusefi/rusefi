/*
 * @file test_idle_controller.cpp
 *
 * @date Oct 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include <stdio.h>
#include "idle_controller.h"
#include "efitime.h"
#include "engine_test_helper.h"

void idleDebug(const char *msg, percent_t value) {
	printf("%s\r\n", msg);
}

static IdleValveState is;

void testIdleController(void) {
	EngineTestHelper eth(FORD_INLINE_6_1995);
	Engine *engine = &eth.engine;
	engine_configuration_s *engineConfiguration = engine->engineConfiguration;
	persistent_config_s *config = engine->config;

	engineConfiguration->targetIdleRpm = 1200;

	is.init(PASS_ENGINE_PARAMETER_F);

	efitimems_t time = 0;

	assertEquals(60, is.getIdle(900, time PASS_ENGINE_PARAMETER));

	time += 2000;
	assertEquals(60.5, is.getIdle(900, time PASS_ENGINE_PARAMETER));

	time += 2000;
	assertEquals(60.6, is.getIdle(1050, time PASS_ENGINE_PARAMETER));
}
