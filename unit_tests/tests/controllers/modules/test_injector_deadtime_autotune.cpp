/*
 * @file test_injector_deadtime_autotune.cpp
 *
 */
#include "pch.h"

TEST(InjectorDeadtimeAutotune, moduleRegisteredWithDefaultState) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto module = engine->module<InjectorDeadtimeAutotune>();
	module->onSlowCallback();
	EXPECT_FALSE(module->dtAutotuneActive);
}
