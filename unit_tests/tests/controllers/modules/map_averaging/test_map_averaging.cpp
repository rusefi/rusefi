/*
 * @file test_map_averaging.cpp
 *
 * @date: may 12, 2025
 * @author FDSoftware
 */

#include "pch.h"
#include "map_averaging.h"

TEST(EngineModules, MapAveragingModule_onEnginePhase) {
    EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
    engineConfiguration->isMapAveragingEnabled = true;
    engineConfiguration->measureMapOnlyInOneCylinder = true;

    // trigger events at crank speed
    for (size_t i = 0; i < 9; i++) {
        eth.fireTriggerEventsWithDuration(200);
        eth.executeActions();
    }

    // TODO: assert current angle? we need refactor "sampler" var for AngleBasedEvent
    EXPECT_TRUE(engine->outputChannels.isMapAveraging);

    // move forward
    eth.fireRise(200);
    eth.executeActions();
    EXPECT_FALSE(engine->outputChannels.isMapAveraging);
}

TEST(EngineModules, MapAveragingModule_onFastCallback) {
    EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
    engineConfiguration->isMapAveragingEnabled = true;

    MapAveragingModule mapModule;

    // trigger events at crank speed
    for (size_t i = 0; i < 9; i++) {
        eth.fireTriggerEventsWithDuration(200);
        eth.executeActions();
    }
    ASSERT_EQ(150,  Sensor::getOrZero(SensorType::Rpm));

    mapModule.onFastCallback();

    EXPECT_EQ(engine->engineState.mapAveragingStart[0], 100);
    EXPECT_EQ(engine->engineState.mapAveragingStart[1], 280);
    EXPECT_EQ(engine->engineState.mapAveragingStart[2], 460);
    EXPECT_EQ(engine->engineState.mapAveragingStart[3], 640);

    EXPECT_EQ(engine->engineState.mapAveragingDuration, 50);
}

TEST(EngineModules, MapAveragingModule_60_2) {
  	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
  	engineConfiguration->isMapAveragingEnabled = true;
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	engineConfiguration->isIgnitionEnabled = false;
	engineConfiguration->isInjectionEnabled = false;

	engine->engineState.mapAveragingDuration = 50;

	testSpinEngineUntilData testSpinInfo;
	eth.spin60_2UntilDeg(testSpinInfo, 1200, 720);
	ASSERT_NEAR(1200, Sensor::getOrZero(SensorType::Rpm), 1) << "RPM";

    // spin after first sync & search for map averaging call
    eth.spin60_2UntilDeg(testSpinInfo, 1200, 200);
	bool averageDone = eth.assertEventExistsAtEnginePhase("startMapAveraging callback", (void*)startAveraging, static_cast<angle_t>(65));
    EXPECT_TRUE(averageDone);

}