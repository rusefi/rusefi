//
// Created by kifir on 11/18/24.
//

#include "pch.h"

#include "util/test_base.h"

namespace {
    constexpr uint8_t TEST_STAGE2_FRACTION = 17;
    constexpr float TEST_INJECTION_MASS = 12.34f;

    constexpr float TEST_PRIMARY_INJECTOR_DEAD_TIME = 5.6f;
    constexpr float TEST_PRIMARY_INJECTOR_FLOW = 239.17f;
    constexpr float TEST_PRIMARY_INJECTOR_BASE_DURATION = TEST_INJECTION_MASS / TEST_PRIMARY_INJECTOR_FLOW * 1000;

    constexpr float TEST_SECONDARY_INJECTOR_DEAD_TIME = 7.8;
    constexpr float TEST_SECONDARY_INJECTOR_FLOW = 174.17f;
    constexpr float TEST_SECONDARY_INJECTOR_BASE_DURATION = TEST_INJECTION_MASS / TEST_SECONDARY_INJECTOR_FLOW * 1000;

    BattLagCorrTable generateConstantBattLagCorrCurve(const float value) {
        BattLagCorrTable testBattLagCorrTable;
        testBattLagCorrTable[0].fill(value);
        testBattLagCorrTable[1].fill(value);
        return testBattLagCorrTable;
    }

    const BattLagCorrTable TEST_PRIMARY_INJECTOR_BATT_LAG_CORR_CURVE = generateConstantBattLagCorrCurve(
        TEST_PRIMARY_INJECTOR_DEAD_TIME
    );
    const BattLagCorrTable TEST_SECONDARY_INJECTOR_BATT_LAG_CORR_CURVE = generateConstantBattLagCorrCurve(
        TEST_SECONDARY_INJECTOR_DEAD_TIME
    );

    const EngineConfig TEST_ENGINE_CONFIG = EngineConfig()
        .setInjectorFlowAsMassFlow(true)
        .setInjectorFlow(TEST_PRIMARY_INJECTOR_FLOW)
        .setInjectorBattLagCorr(TEST_PRIMARY_INJECTOR_BATT_LAG_CORR_CURVE)
        .setInjectorSecondaryFlow(TEST_SECONDARY_INJECTOR_FLOW)
        .setInjectorSecondaryBattLagCorr(TEST_SECONDARY_INJECTOR_BATT_LAG_CORR_CURVE);

    class StagedInjectionTest : public TestBase<> {
    protected:
        void SetUp() override;
    };

    void StagedInjectionTest::SetUp() {
        TestBase::SetUp();

        InjectorStagingTable testInjectorStagingTable;
        for (int i = 0; i < INJ_STAGING_COUNT; i++) {
            for (int j = 0; j < INJ_STAGING_COUNT; j++) {
                testInjectorStagingTable[i][j] = TEST_STAGE2_FRACTION;
            }
        };
        getTestPersistentConfiguration().setInjectorStagingTable(testInjectorStagingTable);

        getTestLuaScriptExecutor().setFuelAdd(TEST_INJECTION_MASS);
    }

    TEST_F(StagedInjectionTest, checkDisabledStagedInjection) {
        setUpEngineConfiguration(TEST_ENGINE_CONFIG);

        periodicFastCallback();

        EXPECT_EQ(getTestEngineState().getInjectionStage2Fraction(), 0.0f);
        EXPECT_NEAR(
            getTestEngineState().getInjectionDuration(),
            TEST_PRIMARY_INJECTOR_BASE_DURATION + TEST_PRIMARY_INJECTOR_DEAD_TIME,
            EPS5D
        );
        EXPECT_EQ(getTestEngineState().getInjectionDurationStage2(), 0.0f);
    }

    TEST_F(StagedInjectionTest, checkEnabledStagedInjection) {
        setUpEngineConfiguration(TEST_ENGINE_CONFIG.clone().setStagedInjectionEnabled(true));

        periodicFastCallback();

        const float injectionStage2Fraction = getTestEngineState().getInjectionStage2Fraction();
        EXPECT_EQ(injectionStage2Fraction, TEST_STAGE2_FRACTION * 0.01f);
        EXPECT_NEAR(
            getTestEngineState().getInjectionDuration(),
            TEST_PRIMARY_INJECTOR_BASE_DURATION * (1 - injectionStage2Fraction) + TEST_PRIMARY_INJECTOR_DEAD_TIME,
            EPS5D
        );
        EXPECT_NEAR(
            getTestEngineState().getInjectionDurationStage2(),
            TEST_SECONDARY_INJECTOR_BASE_DURATION * injectionStage2Fraction + TEST_SECONDARY_INJECTOR_DEAD_TIME,
            EPS5D
        );
    }
}