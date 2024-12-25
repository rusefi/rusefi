//
// Created by kifir on 11/28/24.
//

#include "pch.h"

#include "engine_configuration_defaults.h"

#include "util/test_base.h"

namespace {
    struct SpeedConditionTestData {
        const std::optional<float> speed;
        const bool expectedSpeedCondition;
        const char* const context;
    };

    class NitrousSpeedConditionTest : public TestBase<> {
    protected:
        static constexpr uint16_t TEST_MIN_VEHICLE_SPEED = 33;

        void checkSpeedCondition(const std::vector<SpeedConditionTestData>& testData);
    };

    void NitrousSpeedConditionTest::checkSpeedCondition(const std::vector<SpeedConditionTestData>& testData) {
        for (const SpeedConditionTestData& item: testData) {
            updateVehicleSpeed(item.speed, &TestBase::periodicSlowCallback);
            EXPECT_EQ(getModule<NitrousController>().isNitrousSpeedCondition, item.expectedSpeedCondition)
                << item.context;
        }
    }

    TEST_F(NitrousSpeedConditionTest, checkDefault) {
        checkSpeedCondition({
            { {}, false, "default" },
            { { 0.0f }, false, "0.0" },
            { { TEST_MIN_VEHICLE_SPEED - EPS5D }, false, "TEST_MIN_VEHICLE_SPEED - EPS5D" },
            { { TEST_MIN_VEHICLE_SPEED }, false, "TEST_MIN_VEHICLE_SPEED" },
            { { TEST_MIN_VEHICLE_SPEED + EPS5D }, false, "TEST_MIN_VEHICLE_SPEED + EPS5D" },
        });
    }

    TEST_F(NitrousSpeedConditionTest, checkDefaultWithDisabledNitrousControl) {
        setUpEngineConfiguration(EngineConfig().setNitrousControlEnabled({ false }));
        checkSpeedCondition({
            { {}, false, "default" },
            { { 0.0f }, false, "0.0" },
            { { TEST_MIN_VEHICLE_SPEED - EPS5D }, false, "TEST_MIN_VEHICLE_SPEED - EPS5D" },
            { { TEST_MIN_VEHICLE_SPEED }, false, "TEST_MIN_VEHICLE_SPEED" },
            { { TEST_MIN_VEHICLE_SPEED + EPS5D }, false, "TEST_MIN_VEHICLE_SPEED + EPS5D" },
        });
    }

    TEST_F(NitrousSpeedConditionTest, checkZeroMinimumSpeed) {
        setUpEngineConfiguration(
            EngineConfig()
                .setNitrousControlEnabled({ true })
                .setNitrousMinimumVehicleSpeed(0.0f)
        );
        checkSpeedCondition({
            { {}, true, "default" },
            { { 0.0f }, true, "0.0" },
            { { TEST_MIN_VEHICLE_SPEED - EPS5D }, true, "TEST_MIN_VEHICLE_SPEED - EPS5D" },
            { { TEST_MIN_VEHICLE_SPEED }, true, "TEST_MIN_VEHICLE_SPEED" },
            { { TEST_MIN_VEHICLE_SPEED + EPS5D }, true, "TEST_MIN_VEHICLE_SPEED + EPS5D" },
        });
    }

    TEST_F(NitrousSpeedConditionTest, checkMinimumSpeed) {
        setUpEngineConfiguration(
            EngineConfig()
                .setNitrousControlEnabled({ true })
                .setNitrousMinimumVehicleSpeed({ TEST_MIN_VEHICLE_SPEED })
        );
        checkSpeedCondition({
            { {}, false, "default" },
            { { 0.0f }, false, "0.0" },
            { { TEST_MIN_VEHICLE_SPEED - EPS5D }, false, "TEST_MIN_VEHICLE_SPEED - EPS5D" },
            { { TEST_MIN_VEHICLE_SPEED }, true, "TEST_MIN_VEHICLE_SPEED" },
            { { TEST_MIN_VEHICLE_SPEED + EPS5D }, true, "TEST_MIN_VEHICLE_SPEED + EPS5D" },
        });
    }
}