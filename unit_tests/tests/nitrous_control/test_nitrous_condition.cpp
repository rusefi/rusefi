//
// Created by kifir on 12/2/24.
//

#include "pch.h"

#include "nitrous_test_base.h"
#include "limp_manager.h"

namespace {
    class NitrousConditionTest : public NitrousTestBase {
    protected:
        void SetUp() override;
    };

    void NitrousConditionTest::SetUp() {
        TestBase::SetUp();

        setUpTestConfiguration();

        EXPECT_FALSE(getModule<NitrousController>().isNitrousArmed);
        EXPECT_FALSE(getModule<NitrousController>().isNitrousSpeedCondition);
        EXPECT_FALSE(getModule<NitrousController>().isNitrousTpsCondition);
        EXPECT_FALSE(getModule<NitrousController>().isNitrousCltCondition);
        EXPECT_FALSE(getModule<NitrousController>().isNitrousMapCondition);
        EXPECT_FALSE(getModule<NitrousController>().isNitrousAfrCondition);
        EXPECT_FALSE(getModule<NitrousController>().isNitrousRpmCondition);

        checkNitrousCondition(false, "No conditions are satisfied");

        armNitrousControl();
        checkNitrousCondition(false, "Armed condition is satisfied");

        satisfySpeedCondition();
        checkNitrousCondition(false, "Armed + Speed conditions are satisfied");

        satisfyTpsCondition();
        checkNitrousCondition(false, "Armed + Speed + TPS conditions are satisfied");

        satisfyCltCondition();
        checkNitrousCondition(false, "Armed + Speed + TPS + CLT conditions are satisfied");

        satisfyMapCondition();
        checkNitrousCondition(false, "Armed + Speed + TPS + CLT + MAP conditions are satisfied");

        satisfyAfrCondition();
        checkNitrousCondition(false, "Armed + Speed + TPS + CLT + MAP + AFR conditions are satisfied");

        satisfyRpmCondition();
        checkNitrousCondition(
            true,
            "Armed + Speed + TPS + CLT + MAP + AFR + RPM conditions are satisfied"
        );
    }

    TEST_F(NitrousConditionTest, checkWithoutArmedNitrousControl) {
        unarmNitrousControl();
        checkNitrousCondition(false, "Without armed condition");
    }

    TEST_F(NitrousConditionTest, checkWithoutSatisfiedSpeedCondition) {
        unsatisfySpeedCondition();
        checkNitrousCondition(false, "Without speed condition");
    }

    TEST_F(NitrousConditionTest, checkWithoutSatisfiedTpsCondition) {
        unsatisfyTpsCondition();
        checkNitrousCondition(false, "Without TPS condition");
    }

    TEST_F(NitrousConditionTest, checkWithoutSatisfiedCltCondition) {
        unsatisfyCltCondition();
        checkNitrousCondition(false, "Without CLT condition");
    }

    TEST_F(NitrousConditionTest, checkWithoutSatisfiedMapCondition) {
        unsatisfyMapCondition();
        checkNitrousCondition(false, "Without MAP condition");
    }

    TEST_F(NitrousConditionTest, checkWithoutSatisfiedAfrCondition) {
        unsatisfyAfrCondition();
        checkNitrousCondition(false, "Without AFR condition");
    }

    TEST_F(NitrousConditionTest, checkWithoutSatisfiedRpmCondition) {
        unsatisfyRpmCondition();
        checkNitrousCondition(false, "Without RPM condition");
    }

    // Reproduces ECU-025: the relay remains on while boost protection forbids injection.
    TEST(NitrousControl, NitrousRelayRemainsOnDuringBoostFuelCut) {
        EngineTestHelper eth(engine_type_e::TEST_ENGINE);
        engineConfiguration->nitrousControlEnabled = true;
        engineConfiguration->nitrousControlArmingMethod = LUA_GAUGE;
        engineConfiguration->nitrousLuaGauge = LUA_GAUGE_1;
        engineConfiguration->nitrousLuaGaugeMeaning = LUA_GAUGE_LOWER_BOUND;
        engineConfiguration->nitrousLuaGaugeArmingValue = 1;
        engineConfiguration->nitrousActivationRpm = 1000;
        engineConfiguration->nitrousDeactivationRpm = 6000;
        engineConfiguration->nitrousMinimumTps = 0;
        engineConfiguration->nitrousMaximumMap = 300;
        engineConfiguration->nitrousMaximumAfr = 0;
        engineConfiguration->nitrousFuelAdderPercent = 25;
        engineConfiguration->boostCutPressure = 180;
        Sensor::setMockValue(SensorType::LuaGauge1, 1);
        Sensor::setMockValue(SensorType::Map, 200);
        engine->rpmCalculator.setRpmValue(2000);
        auto& nitrous = engine->module<NitrousController>().unmock();
        nitrous.onSlowCallback();
        EXPECT_TRUE(nitrous.isNitrousArmed);
        EXPECT_TRUE(nitrous.isNitrousSpeedCondition);
        EXPECT_TRUE(nitrous.isNitrousTpsCondition);
        EXPECT_TRUE(nitrous.isNitrousCltCondition);
        EXPECT_TRUE(nitrous.isNitrousMapCondition);
        EXPECT_TRUE(nitrous.isNitrousAfrCondition);
        EXPECT_TRUE(nitrous.isNitrousRpmCondition);
        ASSERT_TRUE(nitrous.isNitrousCondition);
        ASSERT_TRUE(enginePins.nitrousRelay.getLogicValue());
        ASSERT_FLOAT_EQ(1.25f, nitrous.getFuelCoefficient());

        getLimpManager()->updateState(2000, getTimeNowNt());
        ASSERT_EQ(ClearReason::BoostCut, getLimpManager()->allowInjection().reason);
        // handleFuel() now returns before injecting the nitrous fuel adder, but the
        // relay remains commanded on while all nitrous arming inputs stay valid.
        nitrous.onSlowCallback();
        EXPECT_TRUE(nitrous.isNitrousCondition);
        EXPECT_TRUE(enginePins.nitrousRelay.getLogicValue());
    }

}