//
// Created by kifir on 6/18/24.
//

#include "pch.h"

#include "ac_test_base.h"

namespace {
    constexpr uint16_t TEST_MIN_AC_PRESSURE = 17;
    constexpr uint16_t TEST_MAX_AC_PRESSURE = 239;
    constexpr uint16_t DEADBAND_WIDTH = AcController::PRESSURE_DEADBAND_WIDTH;

    class AcPressureTestConfig : public AcTestConfig {
    public:
        AcPressureTestConfig();
    };

    AcPressureTestConfig::AcPressureTestConfig() {
        setAcDelay(0.0f);
        setMinAcPressure(TEST_MIN_AC_PRESSURE);
        setMaxAcPressure(TEST_MAX_AC_PRESSURE);
    }

    struct AcPressureTestData {
        const std::string context;
        const float acPressure;
        const bool expectedAcPressureTooLow;
        const bool expectedAcPressureTooHigh;
    };

    class AcPressureTest : public AcTestBase {
    protected:
        void doTest(const AcPressureTestConfig& config, const std::vector<AcPressureTestData>& testData);
    private:
        void checkPersistentIndicators();
    };

    void AcPressureTest::doTest(const AcPressureTestConfig& config, const std::vector<AcPressureTestData>& testData) {
        setUpTestConfig(config);

        engineConfiguration->acDelay = 0;
        Sensor::setMockValue(SensorType::Rpm, 500); // to avoid engineTooSlow
        engine->engineState.lua.acRequestState = true; // to emulate pressed A/C button

        for (const AcPressureTestData& testDataItem: testData) {
            updateAcPressure(testDataItem.acPressure);

            checkPersistentIndicators();
            EXPECT_EQ(
                engine->engineModules.get<AcController>()->acPressureTooLow,
                testDataItem.expectedAcPressureTooLow
            ) << testDataItem.context;
            EXPECT_EQ(
                engine->engineModules.get<AcController>()->acPressureTooHigh,
                testDataItem.expectedAcPressureTooHigh
            ) << testDataItem.context;
            const bool shouldAcBeEnabled = !testDataItem.expectedAcPressureTooLow
                && !testDataItem.expectedAcPressureTooHigh;
            EXPECT_EQ(engine->engineModules.get<AcController>()->isAcEnabled(), shouldAcBeEnabled)
                << testDataItem.context;
            EXPECT_EQ(engine->engineModules.get<AcController>()->acCompressorState, shouldAcBeEnabled)
                << testDataItem.context;
            EXPECT_EQ(enginePins.acRelay.getLogicValue(), shouldAcBeEnabled) << testDataItem.context;
        }
    }

    void AcPressureTest::checkPersistentIndicators() {
        EXPECT_TRUE(engine->engineModules.get<AcController>()->acButtonState);
        EXPECT_FALSE(engine->engineModules.get<AcController>()->engineTooSlow);
        EXPECT_FALSE(engine->engineModules.get<AcController>()->engineTooFast);
        EXPECT_FALSE(engine->engineModules.get<AcController>()->noClt);
        EXPECT_FALSE(engine->engineModules.get<AcController>()->engineTooHot);
        EXPECT_FALSE(engine->engineModules.get<AcController>()->tpsTooHigh);
        EXPECT_FALSE(engine->engineModules.get<AcController>()->isDisabledByLua);
    }

    TEST_F(AcPressureTest, pressureTooLow) {
        doTest(
            /* config = */ {},
            /* testData = */ {
               {
                   "acPressure = (TEST_MIN_AC_PRESSURE + TEST_MAX_AC_PRESSURE) / 2",
                   (TEST_MIN_AC_PRESSURE + TEST_MAX_AC_PRESSURE) / 2,
                   false,
                   false
               },
               {
                       "acPressure = TEST_MIN_AC_PRESSURE - DEADBAND_WIDTH",
                       TEST_MIN_AC_PRESSURE - DEADBAND_WIDTH,
                       false,
                       false
               },
               {
                   "acPressure = TEST_MIN_AC_PRESSURE - DEADBAND_WIDTH - EPS5D",
                   TEST_MIN_AC_PRESSURE - DEADBAND_WIDTH - EPS5D,
                   true,
                   false
               },
               {
                   "acPressure = TEST_MIN_AC_PRESSURE",
                   TEST_MIN_AC_PRESSURE,
                   true,
                   false
               },
               {
                   "acPressure = TEST_MIN_AC_PRESSURE + DEADBAND_WIDTH",
                   TEST_MIN_AC_PRESSURE + DEADBAND_WIDTH,
                   true,
                   false
               },
               {
                   "acPressure = TEST_MIN_AC_PRESSURE + DEADBAND_WIDTH + EPS5D",
                   TEST_MIN_AC_PRESSURE + DEADBAND_WIDTH + EPS5D,
                   false,
                   false
               }
            }
        );
    }

    TEST_F(AcPressureTest, pressureTooHigh) {
        doTest(
            /* config = */ {},
            /* testData = */ {
                {
                    "acPressure = (TEST_MIN_AC_PRESSURE + TEST_MAX_AC_PRESSURE) / 2",
                    (TEST_MIN_AC_PRESSURE + TEST_MAX_AC_PRESSURE) / 2,
                    false,
                    false
                },
                {
                    "acPressure = TEST_MAX_AC_PRESSURE + DEADBAND_WIDTH",
                    TEST_MAX_AC_PRESSURE + DEADBAND_WIDTH,
                    false,
                    false
                },
                {
                    "acPressure = TEST_MAX_AC_PRESSURE + DEADBAND_WIDTH + EPS5D",
                    TEST_MAX_AC_PRESSURE + DEADBAND_WIDTH + EPS5D,
                    false,
                    true
                },
                {
                    "acPressure = TEST_MAX_AC_PRESSURE",
                    TEST_MAX_AC_PRESSURE,
                    false,
                    true
                },
                {
                    "acPressure = TEST_MAX_AC_PRESSURE - DEADBAND_WIDTH",
                    TEST_MAX_AC_PRESSURE - DEADBAND_WIDTH,
                    false,
                    true
                },
                {
                    "acPressure = TEST_MAX_AC_PRESSURE - DEADBAND_WIDTH - EPS5D",
                    TEST_MAX_AC_PRESSURE - DEADBAND_WIDTH - EPS5D,
                    false,
                    false
                }
            }
        );
    }
}