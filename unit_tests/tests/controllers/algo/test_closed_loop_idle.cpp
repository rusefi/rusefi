/*
 * @file test_close_loop_idle.cpp
 *
 * @date: jun 18, 2025
 * @author FDSoftware
 */

#include "pch.h"
#include "closed_loop_idle.h"

using ::testing::StrictMock;
using ::testing::Eq;
using ::testing::_;

using ICP = IIdleController::Phase;
using TgtInfo = IIdleController::TargetInfo;

class MockIdleLTIT : public MockIdleController {
public:
    bool useClosedLoop = true;
	ICP m_lastPhase = ICP::Cranking;

    ICP getCurrentPhase() const {
        return m_lastPhase;
    }
    TgtInfo getTargetRpm(float clt){
        TgtInfo targetInfo;
        targetInfo.ClosedLoopTarget = 950;
        targetInfo.IdleEntryRpm = 900;
        targetInfo.IdleExitRpm = 1100;

        return targetInfo;
    }
};

TEST(LongTermIdleTrim, isValidConditionsForLearning){
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);
    // idle config
    engineConfiguration->idleMode = idle_mode_e::IM_AUTO;

    // ltit config
    engineConfiguration->ltitEnabled = true;
    engineConfiguration->ltitStableRpmThreshold = 50; // +-50 rpm
    engineConfiguration->ltitStableTime = 1; // second
    engineConfiguration->ltitIgnitionOnDelay = 1; // second
    engineConfiguration->ltitIntegratorThreshold = 4; // % ?

    constexpr int mocked_rpm = 920;

    StrictMock<MockIdleLTIT> idler;
    engine->engineModules.get<IdleController>().set(&idler);
	idler.m_lastPhase = ICP::Idling;

    // LTIT not initialized
    EXPECT_FALSE(engine->m_ltit.isValidConditionsForLearning(4.5f));
    engine->m_ltit.loadLtitFromConfig();
    engine->m_ltit.onIgnitionStateChanged(true);

    advanceTimeUs(MS2US(500));
    // not enough time has passed yet to fulfill ltitIgnitionOnDelay
    EXPECT_FALSE(engine->m_ltit.isValidConditionsForLearning(4.5f));

    // integrator too low
    EXPECT_FALSE(engine->m_ltit.isValidConditionsForLearning(3.0f));

    // integrator too high
    EXPECT_FALSE(engine->m_ltit.isValidConditionsForLearning(-30.f));

    // isStableIdle update
    advanceTimeUs(MS2US(1000));
    engine->m_ltit.update(mocked_rpm,0,false,false,false,0.45);
    EXPECT_TRUE(engine->m_ltit.isValidConditionsForLearning(4.5f));
};

TEST(LongTermIdleTrim, onIgnitionStateChanged) {
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);
    engine->m_ltit.isStableIdle = true;
    engine->m_ltit.onIgnitionStateChanged(true);
    EXPECT_FALSE(engine->m_ltit.isStableIdle);

    engine->m_ltit.updatedLtit = true;
    engine->m_ltit.onIgnitionStateChanged(false);
    EXPECT_TRUE(engine->m_ltit.m_pendingSave);
    EXPECT_FALSE(engine->m_ltit.updatedLtit);
}


TEST(LongTermIdleTrim, checkIfShouldSave) {
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);
    engineConfiguration->ltitIgnitionOffSaveDelay = 2;

    // update m_pendingSave
    engine->m_ltit.updatedLtit = true;
    engine->m_ltit.onIgnitionStateChanged(false);

    // not enough time has passed yet to fulfill ltitIgnitionOffSaveDelay
    engine->m_ltit.checkIfShouldSave();
    EXPECT_TRUE(engine->m_ltit.m_pendingSave);

    advanceTimeUs(MS2US(2500));

    // now we can save the table
    engine->m_ltit.checkIfShouldSave();
    EXPECT_FALSE(engine->m_ltit.m_pendingSave);
}

TEST(LongTermIdleTrim, hasValidData) {
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    // controller not initialized
    EXPECT_FALSE(engine->m_ltit.hasValidData());

    setArrayValues(config->ltitTable, 100);

    // correct data
    EXPECT_TRUE(engine->m_ltit.hasValidData());

    // Now we break almost the entire table with invalid values
    for (int i = 0; i < LTIT_TABLE_SIZE - 2 ; i++) {
        config->ltitTable[i] = 200;
    }
    EXPECT_FALSE(engine->m_ltit.hasValidData());
}

TEST(LongTermIdleTrim, loadLtitFromConfig) {
   	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
    setArrayValues(config->ltitTable, 105);
    engine->m_ltit.loadLtitFromConfig();
    EXPECT_TRUE(engine->m_ltit.ltitTableInitialized);
    EXPECT_EQ(engine->m_ltit.ltitTableHelper[0], 105);
}

TEST(LongTermIdleTrim, getLtitFactor) {
   	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	constexpr auto mocked_rpm = 920;
	constexpr auto mocked_temp = 45.5;
	// default is false pre-load, but we need to be explicit for LCOV
	engine->m_ltit.ltitTableInitialized = false;

	EXPECT_EQ(engine->m_ltit.getLtitFactor(mocked_rpm, mocked_temp), 1);

	setArrayValues(config->ltitTable, 105);
	engine->m_ltit.loadLtitFromConfig();
	EXPECT_NEAR(engine->m_ltit.getLtitFactor(mocked_rpm, mocked_temp), 1.05, EPS4D);
}

// from here to the bottom of the file we test all the branches of "update" function, so expect a lot of test more or less similar!

class mockedLTIT : public LongTermIdleTrim {
public:
	MOCK_METHOD(void, loadLtitFromConfig, (), (override));
};

TEST(LongTermIdleTrim, update_not_enabled) {
 	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
 	engineConfiguration->ltitEnabled = false;
	constexpr int mocked_rpm = 920;

	engine->m_ltit.update(mocked_rpm,0,false,false,false,0.45);
	EXPECT_FALSE(engine->m_ltit.updatedLtit);
}

TEST(LongTermIdleTrim, update_not_Initialized) {
 	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
 	engineConfiguration->ltitEnabled = true;
	constexpr int mocked_rpm = 920;
	mockedLTIT ltit_controller;

	EXPECT_CALL(ltit_controller, loadLtitFromConfig());
	ltit_controller.update(mocked_rpm,0,false,false,false,0.45);
	EXPECT_FALSE(ltit_controller.updatedLtit);
}

TEST(LongTermIdleTrim, update_ignition_delay) {
 	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->ltitIgnitionOnDelay = 1; // second
 	engineConfiguration->ltitEnabled = true;
	engine->m_ltit.isStableIdle = true;
	constexpr int mocked_rpm = 920;

	engine->m_ltit.loadLtitFromConfig();
	engine->m_ltit.onIgnitionStateChanged(true);
	engine->m_ltit.update(mocked_rpm,0,false,false,false,0.45);

	EXPECT_FALSE(engine->m_ltit.updatedLtit);
	EXPECT_FALSE(engine->m_ltit.isStableIdle);
}

TEST(LongTermIdleTrim, update_idle) {
 	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->ltitIgnitionOnDelay = 1; // second
	engineConfiguration->ltitStableTime = 2;
	engineConfiguration->ltitStableRpmThreshold = 50; // +-50 rpm
 	engineConfiguration->ltitEnabled = true;
	engine->m_ltit.isStableIdle = true;
	constexpr int mocked_rpm = 920;

	StrictMock<MockIdleLTIT> idler;
    engine->engineModules.get<IdleController>().set(&idler);

	engine->m_ltit.loadLtitFromConfig();
	engine->m_ltit.onIgnitionStateChanged(true);

	advanceTimeUs(MS2US(1500));
	idler.m_lastPhase = ICP::Running;

	// Phase::Idling expect
	engine->m_ltit.update(mocked_rpm, 0, false, false, false, 0.45);

	EXPECT_FALSE(engine->m_ltit.updatedLtit);
	EXPECT_FALSE(engine->m_ltit.isStableIdle);

	// rpm range expect
	idler.m_lastPhase = ICP::Idling;
	// rpm too high for learn!
	engine->m_ltit.update(mocked_rpm + 200, 0, false, false, false, 0.45);

	EXPECT_FALSE(engine->m_ltit.updatedLtit);
	EXPECT_FALSE(engine->m_ltit.isStableIdle);

	// stable idle check (false & still not in time)
	engine->m_ltit.isStableIdle = false;
	engine->m_ltit.update(mocked_rpm, 0, false, false, false, 0.45);

	EXPECT_FALSE(engine->m_ltit.updatedLtit);
	EXPECT_FALSE(engine->m_ltit.isStableIdle);

	// stable idle check (false & still now in time)
	engine->m_ltit.isStableIdle = false;
	engine->m_ltit.onIgnitionStateChanged(true);
	advanceTimeUs(MS2US(2500));
	engine->m_ltit.update(mocked_rpm, 0, false, false, false, 0.45);

	EXPECT_FALSE(engine->m_ltit.updatedLtit);
	EXPECT_TRUE(engine->m_ltit.isStableIdle);

}

TEST(LongTermIdleTrim, update) {
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);
    // idle config
    engineConfiguration->idleMode = idle_mode_e::IM_AUTO;

    // ltit config
    engineConfiguration->ltitEnabled = true;
    engineConfiguration->ltitStableRpmThreshold = 50; // +-50 rpm
    engineConfiguration->ltitStableTime = 1; // second
    engineConfiguration->ltitIgnitionOnDelay = 1; // second
    engineConfiguration->ltitIntegratorThreshold = 4; // % ?
	setArrayValues(config->ltitTable, 105);

    constexpr int mocked_rpm = 920;
	constexpr int mocked_temp = 45.5;

    StrictMock<MockIdleLTIT> idler;
    engine->engineModules.get<IdleController>().set(&idler);
	idler.m_lastPhase = ICP::Idling;
	idler.useClosedLoop = true;

    // LTIT not initialized
    EXPECT_FALSE(engine->m_ltit.isValidConditionsForLearning(4.5f));
    engine->m_ltit.loadLtitFromConfig();
    engine->m_ltit.onIgnitionStateChanged(true);

    // idle controller isIdleClosedLoop
	engine->engineModules.get<IdleController>()->isIdleClosedLoop = true;

    advanceTimeUs(MS2US(2500));
   	engine->m_ltit.update(mocked_rpm, mocked_temp, false, false, false, 4.5);
	EXPECT_NEAR(engine->m_ltit.ltitTableHelper[0], 105, EPS4D);
	EXPECT_TRUE(engine->m_ltit.updatedLtit);
}

//TODO: this func is unused:
TEST(LongTermIdleTrim, smoothLtitTable){
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
    // ltit config
    engineConfiguration->ltitEnabled = true;
	setArrayValues(engine->m_ltit.ltitTableHelper, 100);

	// randomize a bit the table:
    for(size_t i = 0; i < LTIT_TABLE_SIZE; i++){
      engine->m_ltit.ltitTableHelper[i] = engine->m_ltit.ltitTableHelper[i] + (5 * i);
    }

    // invalid factor:
    engine->m_ltit.smoothLtitTable(180);
    EXPECT_FALSE(engine->m_ltit.m_pendingSave);

    // valid factor
    engine->m_ltit.smoothLtitTable(45);
	EXPECT_EQ(engine->m_ltit.ltitTableHelper[0], 101.125);
    EXPECT_EQ(engine->m_ltit.ltitTableHelper[1], 105);
	EXPECT_EQ(engine->m_ltit.ltitTableHelper[3], 115);
	EXPECT_TRUE(engine->m_ltit.m_pendingSave);
}
