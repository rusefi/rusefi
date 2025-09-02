/*
 * @file test_utils.cpp
 *
 * @date: may 15, 2025
 * @author FDSoftware
 */

#include "pch.h"
#include "util/injection_crank_helper.h"
#include "prime_injection.h"
#include "engine_test_helper.h"

TEST(test_utils, assertEventExistsAtEnginePhase){
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);
    setTable(config->injectionPhase, -180.0f);
    engineConfiguration->isFasterEngineSpinUpEnabled = false;
    engine->tdcMarkEnabled = false;
    setTestFuelCrankingTable(12);

    setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);
    ASSERT_EQ( 0,  Sensor::getOrZero(SensorType::Rpm)) << "RPM=0";

    eth.fireTriggerEventsWithDuration(/* durationMs */ 200);
    eth.fireRise(/* delayMs */ 200);
    ASSERT_EQ(300, Sensor::getOrZero(SensorType::Rpm));
    // two simultaneous injections
    ASSERT_EQ( 4,  engine->scheduler.size()) << "plain#2";

    auto const startSimultaneousInjectionAction{ action_s::make<startSimultaneousInjection>() };

    // old event assert:
    eth.assertEvent5("sim start", 0, startSimultaneousInjectionAction, 100000 - 1625);
    // new event assert:
    bool injectionDone = eth.assertEventExistsAtEnginePhase("sim start", startSimultaneousInjectionAction, static_cast<angle_t>(177.08));
    EXPECT_TRUE(injectionDone);
};


TEST(test_utils, spin60_2UntilDeg){
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);
    eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);
    testSpinEngineUntilData testSpinInfo = { 0, 0, 0 };

    eth.spin60_2UntilDeg(testSpinInfo, 300, 720);
    ASSERT_NEAR(300, Sensor::getOrZero(SensorType::Rpm), 1) << "RPM";

    eth.spin60_2UntilDeg(testSpinInfo, 1000, 720);
    ASSERT_NEAR(1000, Sensor::getOrZero(SensorType::Rpm), 1) << "RPM";

    eth.spin60_2UntilDeg(testSpinInfo, 1200, 720);
    ASSERT_NEAR(1200, Sensor::getOrZero(SensorType::Rpm), 1) << "RPM";

}

TEST(CyclicBuffer, Contains) {
	using tbuffer = cyclic_buffer<int, 4>;

	tbuffer buf;

	buf.add(1);
	EXPECT_TRUE(buf.contains(1));
	EXPECT_FALSE(buf.contains(2));
	EXPECT_FALSE(buf.contains(3));
	EXPECT_FALSE(buf.contains(4));

	buf.add(2);
	buf.add(3);
	buf.add(4);

	EXPECT_FALSE(buf.contains(1)); // huh?
	EXPECT_FALSE(buf.contains(2)); // huh?
	EXPECT_FALSE(buf.contains(3)); // huh?
	EXPECT_FALSE(buf.contains(4)); // huh?

	buf.add(5);
	buf.add(6);
	buf.add(7);

	EXPECT_FALSE(buf.contains(1));
	EXPECT_FALSE(buf.contains(2));
	EXPECT_FALSE(buf.contains(3));
	EXPECT_FALSE(buf.contains(4)); // huh?
	EXPECT_TRUE(buf.contains(5));
	EXPECT_TRUE(buf.contains(6));
	EXPECT_TRUE(buf.contains(7));
}
