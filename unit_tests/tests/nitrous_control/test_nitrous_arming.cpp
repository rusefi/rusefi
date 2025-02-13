//
// Created by kifir on 11/27/24.
//

#include "pch.h"

#include "util/test_base.h"

namespace {
    class NitrousArmingTest: public TestBase<> {
    protected:
        static constexpr switch_input_pin_e TEST_NITROUS_CONTROL_ARMING_PIN = Gpio::A13;
        static constexpr lua_gauge_e TEST_NITROUS_LUA_GAUGE = LUA_GAUGE_3;
        static constexpr SensorType TEST_NITROUS_LUA_GAUGE_SENSOR = SensorType::LuaGauge3;
        static constexpr float TEST_NITROUS_LUA_GAUGE_ARMING_VALUE = 239.17;

        static const EngineConfig TEST_ENGINE_CONFIG;

        void checkArmingAfterPeriodicFastCallback(bool shouldBeArmed, const char* context);
    };

    const EngineConfig NitrousArmingTest::TEST_ENGINE_CONFIG = EngineConfig()
            .setNitrousControlTriggerPin({ TEST_NITROUS_CONTROL_ARMING_PIN })
            .setNitrousLuaGauge({ TEST_NITROUS_LUA_GAUGE })
            .setNitrousLuaGaugeArmingValue({ TEST_NITROUS_LUA_GAUGE_ARMING_VALUE });

    void NitrousArmingTest::checkArmingAfterPeriodicFastCallback(const bool shouldBeArmed, const char* const context) {
        periodicSlowCallback();

        EXPECT_EQ(getModule<NitrousController>().isNitrousArmed, shouldBeArmed) << context;
    }

    TEST_F(NitrousArmingTest, checkDefault) {
        setUpEngineConfiguration(EngineConfig());

        checkArmingAfterPeriodicFastCallback(false, "default");
    }

    TEST_F(NitrousArmingTest, checkDigitalSwitchInputArmingMethod) {
        setUpEngineConfiguration(
        TEST_ENGINE_CONFIG.clone()
                .setNitrousControlEnabled({ true })
                .setNitrousControlArmingMethod({ DIGITAL_SWITCH_INPUT })
        );

        checkArmingAfterPeriodicFastCallback(false, "default");

        setMockState(TEST_NITROUS_CONTROL_ARMING_PIN, true);
        checkArmingAfterPeriodicFastCallback(true, "arming pin is turned on");

        setMockState(TEST_NITROUS_CONTROL_ARMING_PIN, false);
        checkArmingAfterPeriodicFastCallback(false, "arming pin is turned off");
    }

    TEST_F(NitrousArmingTest, checkDigitalSwitchInputArmingMethodInverted) {
        setUpEngineConfiguration(
        TEST_ENGINE_CONFIG.clone()
                .setNitrousControlEnabled({ true })
                .setNitrousControlArmingMethod({ DIGITAL_SWITCH_INPUT })
                .setNitrousControlTriggerPinMode({ PI_INVERTED_DEFAULT })
        );

        checkArmingAfterPeriodicFastCallback(true, "default");

        setMockState(TEST_NITROUS_CONTROL_ARMING_PIN, true);
        checkArmingAfterPeriodicFastCallback(false, "arming pin is turned on");

        setMockState(TEST_NITROUS_CONTROL_ARMING_PIN, false);
        checkArmingAfterPeriodicFastCallback(true, "arming pin is turned off");
    }

    TEST_F(NitrousArmingTest, checkDisabledNitrousControl) {
        setUpEngineConfiguration(
        TEST_ENGINE_CONFIG.clone()
                .setNitrousControlEnabled({ false })
                .setNitrousControlArmingMethod({ DIGITAL_SWITCH_INPUT })
        );

        checkArmingAfterPeriodicFastCallback(false, "default");

        setMockState(TEST_NITROUS_CONTROL_ARMING_PIN, false);
        checkArmingAfterPeriodicFastCallback(false, "arming pin is turned on");

        setMockState(TEST_NITROUS_CONTROL_ARMING_PIN, false);
        checkArmingAfterPeriodicFastCallback(false, "arming pin is turned off");
    }

    TEST_F(NitrousArmingTest, checkLuaGaugeLowerBound) {
        setUpEngineConfiguration(
        TEST_ENGINE_CONFIG.clone()
                .setNitrousControlEnabled({ true })
                .setNitrousControlArmingMethod({ LUA_GAUGE })
                .setNitrousLuaGaugeMeaning({ LUA_GAUGE_LOWER_BOUND })
        );

        checkArmingAfterPeriodicFastCallback(false, "default");

        Sensor::setMockValue(TEST_NITROUS_LUA_GAUGE_SENSOR, TEST_NITROUS_LUA_GAUGE_ARMING_VALUE - EPS5D);
        checkArmingAfterPeriodicFastCallback(false, "under Lua arming value");

        Sensor::setMockValue(TEST_NITROUS_LUA_GAUGE_SENSOR, TEST_NITROUS_LUA_GAUGE_ARMING_VALUE);
        checkArmingAfterPeriodicFastCallback(true, "Lua arming value");

        Sensor::setMockValue(TEST_NITROUS_LUA_GAUGE_SENSOR, TEST_NITROUS_LUA_GAUGE_ARMING_VALUE + EPS5D);
        checkArmingAfterPeriodicFastCallback(true, "above Lua arming value");
    }

    TEST_F(NitrousArmingTest, checkLuaGaugeUpperBound) {
        setUpEngineConfiguration(
        TEST_ENGINE_CONFIG.clone()
                .setNitrousControlEnabled({ true })
                .setNitrousControlArmingMethod({ LUA_GAUGE })
                .setNitrousLuaGaugeMeaning({ LUA_GAUGE_UPPER_BOUND })
        );

        checkArmingAfterPeriodicFastCallback(false, "default");

        Sensor::setMockValue(TEST_NITROUS_LUA_GAUGE_SENSOR, TEST_NITROUS_LUA_GAUGE_ARMING_VALUE - EPS5D);
        checkArmingAfterPeriodicFastCallback(true, "under Lua arming value");

        Sensor::setMockValue(TEST_NITROUS_LUA_GAUGE_SENSOR, TEST_NITROUS_LUA_GAUGE_ARMING_VALUE);
        checkArmingAfterPeriodicFastCallback(true, "Lua arming value");

        Sensor::setMockValue(TEST_NITROUS_LUA_GAUGE_SENSOR, TEST_NITROUS_LUA_GAUGE_ARMING_VALUE + EPS5D);
        checkArmingAfterPeriodicFastCallback(false, "above Lua arming value");
    }
}