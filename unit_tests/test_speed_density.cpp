/**
 * @file test_speed_density.cpp
 *
 * @date Jun 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "engine_test_helper.h"
#include "speed_density.h"
#include "test_speed_density.h"

void testSpeedDensity(void) {
	printf("*************************************************** testSpeedDensity\r\n");
	EngineTestHelper eth(FORD_INLINE_6_1995);
	Engine *engine = &eth.engine;
	engine_configuration_s *engineConfiguration = engine->engineConfiguration;

	eth.ec->trigger.customTotalToothCount = 8;
	eth.initTriggerShapeAndRpmCalculator();

	eth.fireTriggerEvents();
	assertEqualsM("RPM", 1500, eth.engine.rpmCalculator.rpm(PASS_ENGINE_PARAMETER_F));

	// 427 cubic inches, that's a LOT of engine
	eth.ec->specs.displacement = 6.99728;
	eth.ec->cylindersCount = 8;

	eth.ec->injector.flow = gramm_second_to_cc_minute(5.303);

	// 0.01414 sec or 14.14 ms
	assertEquals(0.01414, sdMath(eth.ec, 0.92, 98, 12.5, 293.16));
}
