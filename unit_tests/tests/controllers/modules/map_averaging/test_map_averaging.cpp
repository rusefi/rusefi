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
    engine->engineState.mapAveragingDuration = 0;
    engine->rpmCalculator.setRpmValue(150);

    // trigger events at crank speed
    for (size_t i = 0; i < 9; i++) {
        eth.fireTriggerEventsWithDuration(200);
        eth.executeActions();
    }

    EXPECT_TRUE(engine->outputChannels.isMapAveraging);

    // move forward
    eth.fireRise(200);
    eth.executeActions();
    EXPECT_FALSE(engine->outputChannels.isMapAveraging);
}

TEST(EngineModules, MapAveragingModule_onEnginePhase_oneCylinder) {
    EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
    engineConfiguration->isMapAveragingEnabled = true;
    engineConfiguration->measureMapOnlyInOneCylinder = true;
    engine->rpmCalculator.setRpmValue(150);

    // trigger events at crank speed
    for (size_t i = 0; i < 4; i++) {
        eth.fireTriggerEventsWithDuration(200);
        eth.executeActions();
    }

    // map averaging onEnginePhase & startMapAveraging will trigger here:
    eth.fireTriggerEventsWithDuration(200);

    auto & mapAverager = *engine->module<MapAveragingModule>();
    TestExecutor *executor = &engine->scheduler;
    // this test is relative fragile as it depend on the number of events scheduled
    // see also comment on 7868 for tests with the same problem, maybe we need a "getForUnitTest(some_event_identifier)"?
    scheduling_s *event = executor->getForUnitTest(5);

    EXPECT_TRUE((void*)event->action.getCallback() == (void*)startMapAveraging) << "startMapAveraging callback";

    auto m_event = mapAverager.samplers[0][0].startTimer;

    // should be equal to 100° since currentPhase is 0;
    EXPECT_EQ(m_event.getAngle(), 100);

    eth.executeActions();

    // move forward
    eth.fireTriggerEventsWithDuration(200);
    eth.executeActions();
    EXPECT_FALSE(engine->outputChannels.isMapAveraging);
}

TEST(EngineModules, MapAveragingModule_onEnginePhase_MultipleCylinder) {
    EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
    engineConfiguration->isMapAveragingEnabled = true;
    engineConfiguration->measureMapOnlyInOneCylinder = false;
    engineConfiguration->cylindersCount = 4;

    engine->rpmCalculator.setRpmValue(150);

    // trigger events at crank speed
    for (size_t i = 0; i < 4; i++) {
        eth.fireTriggerEventsWithDuration(200);
        eth.executeActions();
    }

    // map averaging onEnginePhase & startMapAveraging will trigger here:
    for (size_t i = 0; i < 10; i++) {
        eth.fireTriggerEventsWithDuration(200);
    }

    auto & mapAverager = *engine->module<MapAveragingModule>();
    TestExecutor *executor = &engine->scheduler;

    // we expect 4 items, rest of the calls are ign/iny
    for (size_t i = 36; i < 39; i++) {
        scheduling_s *event = executor->getForUnitTest(i);
        EXPECT_TRUE((void*)event->action.getCallback() == (void*)startMapAveraging) << "startMapAveraging callback" << i;
    }

    auto m_event_cil_1 = mapAverager.samplers[0][0].startTimer;
    auto m_event_cil_2 = mapAverager.samplers[1][0].startTimer;
    auto m_event_cil_3 = mapAverager.samplers[2][0].startTimer;
    auto m_event_cil_4 = mapAverager.samplers[3][0].startTimer;

    // should be equal to mapAveragingStart since currentPhase is 0;
    EXPECT_EQ(m_event_cil_1.getAngle(), 100);
    EXPECT_EQ(m_event_cil_2.getAngle(), 100);
    EXPECT_EQ(m_event_cil_3.getAngle(), 100);
    EXPECT_EQ(m_event_cil_4.getAngle(), 100);
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