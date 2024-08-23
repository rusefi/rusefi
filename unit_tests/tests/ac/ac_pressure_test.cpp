//
// Created by kifir on 6/18/24.
//

#include "pch.h"

#include "ac_test_base.h"
#include "engine_configuration_defaults.h"

namespace {
    constexpr uint16_t TEST_MIN_AC_PRESSURE = 17;
    constexpr uint16_t TEST_MAX_AC_PRESSURE = 239;
    constexpr float TEST_AC_PRESSURE_ENABLE_HYSTERESIS = 2.0f;
    constexpr uint16_t DEADBAND_WIDTH = AcController::PRESSURE_DEADBAND_WIDTH;

    class AcPressureTestConfig : public AcTestConfig {
    public:
        AcPressureTestConfig(
            std::optional<uint16_t> minAcPressure,
            std::optional<uint16_t> maxAcPressure,
            std::optional<float> acPressureEnableHysteresis
        );
    };

    AcPressureTestConfig::AcPressureTestConfig(
            const std::optional<uint16_t> minAcPressure,
            const std::optional<uint16_t> maxAcPressure,
            const std::optional<float> acPressureEnableHysteresis
    ) {
        setMinAcPressure(minAcPressure);
        setMaxAcPressure(maxAcPressure);
        setAcPressureEnableHysteresis(acPressureEnableHysteresis);
    }

    struct AcPressureTestData {
        const std::string context;
        const float acPressure;
        const bool expectedAcPressureTooLow;
        const bool expectedAcPressureTooHigh;
    };

    class AcPressureTest : public AcTestBase {
    protected:
        static const AcPressureTestConfig DEFAULT_AC_PRESSURE_CONFIG;
        static const AcPressureTestConfig TEST_AC_PRESSURE_CONFIG;

        void doTest(const AcPressureTestConfig& config, const std::vector<AcPressureTestData>& testData);
    private:
        void checkPersistentIndicators();
    };

    const AcPressureTestConfig AcPressureTest::DEFAULT_AC_PRESSURE_CONFIG = { {}, {}, {} };

    const AcPressureTestConfig AcPressureTest::TEST_AC_PRESSURE_CONFIG = {
            { TEST_MIN_AC_PRESSURE },
            { TEST_MAX_AC_PRESSURE },
            { TEST_AC_PRESSURE_ENABLE_HYSTERESIS }
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
            /* config = */ TEST_AC_PRESSURE_CONFIG,
            /* testData = */ {
               {
                   "acPressure = (TEST_MIN_AC_PRESSURE + TEST_MAX_AC_PRESSURE) / 2.0",
                   (TEST_MIN_AC_PRESSURE + TEST_MAX_AC_PRESSURE) / 2.0,
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
            /* config = */ TEST_AC_PRESSURE_CONFIG,
            /* testData = */ {
                {
                    "acPressure = (TEST_MIN_AC_PRESSURE + TEST_MAX_AC_PRESSURE) / 2.0",
                    (TEST_MIN_AC_PRESSURE + TEST_MAX_AC_PRESSURE) / 2.0,
                    false,
                    false
                },
                {
                    "acPressure = TEST_MAX_AC_PRESSURE",
                    TEST_MAX_AC_PRESSURE,
                    false,
                    false
                },
                {
                    "acPressure = TEST_MAX_AC_PRESSURE + EPS5D",
                    TEST_MAX_AC_PRESSURE + EPS5D,
                    false,
                    true
                },
                {
                    "acPressure = TEST_MAX_AC_PRESSURE - TEST_AC_PRESSURE_ENABLE_HYSTERESIS",
                    TEST_MAX_AC_PRESSURE - TEST_AC_PRESSURE_ENABLE_HYSTERESIS,
                    false,
                    true
                },
                {
                    "acPressure = TEST_MAX_AC_PRESSURE - TEST_AC_PRESSURE_ENABLE_HYSTERESIS - EPS5D",
                    TEST_MAX_AC_PRESSURE - TEST_AC_PRESSURE_ENABLE_HYSTERESIS - EPS5D,
                    false,
                    false
                }
            }
        );
    }


    TEST_F(AcPressureTest, pressureTooHighWithZeroHysteresis) {
        doTest(
            /* config = */ { {}, {}, { 0.0f } },
            /* testData = */ {
               {
                   "acPressure = (DEFAULT_MIN_AC_PRESSURE + DEFAULT_MAX_AC_PRESSURE) / 2.0",
                   (DEFAULT_MIN_AC_PRESSURE + DEFAULT_MAX_AC_PRESSURE) / 2.0,
                   false,
                   false
               },
               {
                   "acPressure = DEFAULT_MAX_AC_PRESSURE",
                   DEFAULT_MAX_AC_PRESSURE,
                   false,
                   false
               },
               {
                   "acPressure = DEFAULT_MAX_AC_PRESSURE + EPS4D",
                   DEFAULT_MAX_AC_PRESSURE + EPS4D,
                   false,
                   true
               },
                {
                    "acPressure = DEFAULT_MAX_AC_PRESSURE",
                    DEFAULT_MAX_AC_PRESSURE,
                    false,
                    true
                },
                {
                   "acPressure = DEFAULT_MAX_AC_PRESSURE - EPS4D",
                   DEFAULT_MAX_AC_PRESSURE - EPS4D,
                   false,
                   false
               }
            }
        );
    }

    TEST_F(AcPressureTest, defaultMinAcPressure) {
        doTest(
            /* config = */ DEFAULT_AC_PRESSURE_CONFIG,
            /* testData = */ {
               {
                   "acPressure = (DEFAULT_MIN_AC_PRESSURE + DEFAULT_MAX_AC_PRESSURE) / 2.0",
                   (DEFAULT_MIN_AC_PRESSURE + DEFAULT_MAX_AC_PRESSURE) / 2.0,
                   false,
                   false
               },
               {
                   "acPressure = DEFAULT_MIN_AC_PRESSURE - DEADBAND_WIDTH",
                       DEFAULT_MIN_AC_PRESSURE - DEADBAND_WIDTH,
                   false,
                   false
               },
               {
                   "acPressure = DEFAULT_MIN_AC_PRESSURE - DEADBAND_WIDTH - EPS5D",
                   DEFAULT_MIN_AC_PRESSURE - DEADBAND_WIDTH - EPS5D,
                   true,
                   false
               },
               {
                   "acPressure = DEFAULT_MIN_AC_PRESSURE",
                   DEFAULT_MIN_AC_PRESSURE,
                   true,
                   false
               },
               {
                   "acPressure = DEFAULT_MIN_AC_PRESSURE + DEADBAND_WIDTH",
                   DEFAULT_MIN_AC_PRESSURE + DEADBAND_WIDTH,
                   true,
                   false
               },
               {
                   "acPressure = DEFAULT_MIN_AC_PRESSURE + DEADBAND_WIDTH + EPS5D",
                   DEFAULT_MIN_AC_PRESSURE + DEADBAND_WIDTH + EPS5D,
                   false,
                   false
               }
           }
        );
    }

    TEST_F(AcPressureTest, defaultMaxAcPressure) {
        doTest(
            /* config = */ DEFAULT_AC_PRESSURE_CONFIG,
            /* testData = */ {
               {
                   "acPressure = (DEFAULT_MIN_AC_PRESSURE + DEFAULT_MAX_AC_PRESSURE) / 2.0",
                   (DEFAULT_MIN_AC_PRESSURE + DEFAULT_MAX_AC_PRESSURE) / 2.0,
                   false,
                   false
               },
               {
                   "acPressure = DEFAULT_MAX_AC_PRESSURE",
                   DEFAULT_MAX_AC_PRESSURE,
                   false,
                   false
               },
               {
                   "acPressure = DEFAULT_MAX_AC_PRESSURE + EPS4D",
                   DEFAULT_MAX_AC_PRESSURE + EPS4D,
                   false,
                   true
               },
                {
                    "acPressure = DEFAULT_MAX_AC_PRESSURE - engine_configuration_defaults::AC_PRESSURE_ENABLE_HYST",
                    DEFAULT_MAX_AC_PRESSURE - engine_configuration_defaults::AC_PRESSURE_ENABLE_HYST,
                    false,
                    true
                },
                {
                   "acPressure = DEFAULT_MAX_AC_PRESSURE - engine_configuration_defaults::AC_PRESSURE_ENABLE_HYST - EPS4D",
                   DEFAULT_MAX_AC_PRESSURE - engine_configuration_defaults::AC_PRESSURE_ENABLE_HYST - EPS4D,
                   false,
                   false
               }
            }
        );
    }
}