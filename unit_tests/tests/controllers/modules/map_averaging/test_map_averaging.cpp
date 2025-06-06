/*
 * @file test_map_averaging.cpp
 *
 * @date: may 12, 2025
 * @author FDSoftware
 */

#include "pch.h"
#include "map_averaging.h"
#include "harley.h"

TEST(EngineModules, MapAveragingModule_onEnginePhase) {
    EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
    engineConfiguration->isMapAveragingEnabled = true;
    engineConfiguration->measureMapOnlyInOneCylinder = true;

    // trigger events at crank speed
    for (size_t i = 0; i < 9; i++) {
        eth.fireTriggerEventsWithDuration(200);
        eth.executeActions();
    }

    EXPECT_TRUE(engine->outputChannels.isMapAveraging);
    bool averageDone = eth.assertEventExistsAtEnginePhase("startMapAveraging callback", (void*)startAveraging, static_cast<angle_t>(50));
    EXPECT_TRUE(averageDone);

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

    // we expect here than the map start angles correspond to the phase of the cylinder + 100 of samplingAngle
   	EXPECT_EQ(engine->engineState.mapAveragingStart[0], 100);
    EXPECT_EQ(engine->engineState.mapAveragingStart[1], 640);
    EXPECT_EQ(engine->engineState.mapAveragingStart[2], 280);
    EXPECT_EQ(engine->engineState.mapAveragingStart[3], 460);

    EXPECT_EQ(engine->engineState.mapAveragingDuration, 50);
}

TEST(EngineModules, MapAveragingModule_onFastCallbackCustomSampleWindow) {
    EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
    engineConfiguration->isMapAveragingEnabled = true;
	setArrayValues(engineConfiguration->map.samplingAngle, 75);

    MapAveragingModule mapModule;

    // trigger events at crank speed
    for (size_t i = 0; i < 9; i++) {
        eth.fireTriggerEventsWithDuration(200);
        eth.executeActions();
    }
    ASSERT_EQ(150,  Sensor::getOrZero(SensorType::Rpm));

    mapModule.onFastCallback();

    // we expect here than the map start angles correspond to the phase of the cylinder + 75 of samplingAngle
    EXPECT_EQ(engine->engineState.mapAveragingStart[0], 75);  // 0 + 75
    EXPECT_EQ(engine->engineState.mapAveragingStart[1], 615); // 540 + 75
    EXPECT_EQ(engine->engineState.mapAveragingStart[2], 255); // 180 + 75
    EXPECT_EQ(engine->engineState.mapAveragingStart[3], 435); // 360 + 75

    EXPECT_EQ(engine->engineState.mapAveragingDuration, 50);
}

TEST(EngineModules, MapAveragingModule_onFastCallbackOddFire) {
    EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
    engineConfiguration->isMapAveragingEnabled = true;
	setArrayValues(engineConfiguration->map.samplingAngle, 75);

	setHarley();
	engineConfiguration->timing_offset_cylinder[0] = -HARLEY_V_TWIN / 2;
	engineConfiguration->timing_offset_cylinder[1] = +HARLEY_V_TWIN / 2;

    MapAveragingModule mapModule;

    // trigger events at crank speed
    for (size_t i = 0; i < 9; i++) {
        eth.fireTriggerEventsWithDuration(200);
        eth.executeActions();
    }
    ASSERT_EQ(150,  Sensor::getOrZero(SensorType::Rpm));

    mapModule.onFastCallback();

    // 0 from getAngleOffset + 75 custom sampling start - 22.5 from timing_offset_cylinder
    EXPECT_EQ(engine->engineState.mapAveragingStart[0], 52.5);
    // 540 from getAngleOffset + 75 custom sampling start + 22.5 from timing_offset_cylinder
    EXPECT_EQ(engine->engineState.mapAveragingStart[1], 637.5);

    EXPECT_EQ(engine->engineState.mapAveragingDuration, 50);
}

TEST(EngineModules, MapAveragingModule_onEnginePhase60_2_one_cylinder) {
    EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
    engineConfiguration->isMapAveragingEnabled = true;
    testSpinEngineUntilData testSpinInfo = { 0, 0, 0 };
    engineConfiguration->measureMapOnlyInOneCylinder = true;
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);
	MapAveragingModule mapModule;
	engine->rpmCalculator.setRpmValue(200);

	engine->module<MapAveragingModule>()->onEnginePhase(200, getTimeNowNt(), 0.f, 180.f);

	// we expect offset of enginePhase (0 since we call onEnginePhase directy) + 100° of samplingAngle (default setting)
	bool averageDone = eth.assertEventExistsAtEnginePhase("startMapAveraging callback", (void*)startAveraging, static_cast<angle_t>(100));
    EXPECT_TRUE(averageDone);
}

TEST(EngineModules, MapAveragingModule_onEnginePhase60_2_one_cylinderCustomSampleWindow) {
    EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
    testSpinEngineUntilData testSpinInfo = { 0, 0, 0 };
    engineConfiguration->isMapAveragingEnabled = true;
    engineConfiguration->measureMapOnlyInOneCylinder = true;
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);
	MapAveragingModule mapModule;
	engine->rpmCalculator.setRpmValue(200);
	setArrayValues(engineConfiguration->map.samplingAngle, 75);

	engine->module<MapAveragingModule>()->onFastCallback();
	engine->module<MapAveragingModule>()->onEnginePhase(200, getTimeNowNt(), 0.f, 180.f);

	// we expect offset of enginePhase (0 since we call onEnginePhase directy) + 75° of samplingAngle
	bool averageDone = eth.assertEventExistsAtEnginePhase("startMapAveraging callback", (void*)startAveraging, static_cast<angle_t>(75));
    EXPECT_TRUE(averageDone);
}