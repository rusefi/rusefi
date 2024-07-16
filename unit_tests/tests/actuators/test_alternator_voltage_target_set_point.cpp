//
// Created by kifir on 7/15/24.
//
#include "pch.h"

#include "util/test_base.h"

namespace {
    constexpr int TEST_CRANKING_RPM = 500;
    constexpr int TEST_LOAD = 17;
    const float DEFAULT_ALTERNATOR_VOLTAGE_TARGET = 14.0f;

    class AlternatorVoltageTargetSetPointTest : public TestBase {
    protected:
        virtual void SetUp() override;

        void enableAlternatorControl();
        void disableAlternatorControl();

        void setRpmAndLoad(int rpm, int load);

        expected<float> getSetpoint();
    };

    void AlternatorVoltageTargetSetPointTest::SetUp() {
        TestBase::SetUp();
        engineConfiguration->cranking.rpm = TEST_CRANKING_RPM;
        enableAlternatorControl();
    }

    void AlternatorVoltageTargetSetPointTest::enableAlternatorControl() {
        engineConfiguration->isAlternatorControlEnabled = true;
    }

    void AlternatorVoltageTargetSetPointTest::disableAlternatorControl() {
        engineConfiguration->isAlternatorControlEnabled = false;
    }

    void AlternatorVoltageTargetSetPointTest::setRpmAndLoad(int rpm, int load = TEST_LOAD) {
        Sensor::setMockValue(SensorType::Rpm, rpm);
    }

    expected<float> AlternatorVoltageTargetSetPointTest::getSetpoint() {
        return engine->engineModules.get<AlternatorController>()->getSetpoint();
    }

    TEST_F(AlternatorVoltageTargetSetPointTest, HomogeneousAlternatorVoltageTargetTable) {
        const float TEST_ALTERNATOR_VOLTAGE_TARGET = 14.2f;
        setTable(config->alternatorVoltageTargetTable, TEST_ALTERNATOR_VOLTAGE_TARGET);

        setRpmAndLoad(TEST_CRANKING_RPM);
        // disabled if rpm <= cranking.rpm
        EXPECT_FALSE(getSetpoint().Valid);

        setRpmAndLoad(TEST_CRANKING_RPM + 1);
        // enabled!
        EXPECT_EQ(TEST_ALTERNATOR_VOLTAGE_TARGET, getSetpoint().value_or(-1));

        disableAlternatorControl();
        // disabled manually
        EXPECT_FALSE(getSetpoint().Valid);
    }

    TEST_F(AlternatorVoltageTargetSetPointTest, DefaultAlternatorVoltageTargetTable) {
        setRpmAndLoad(TEST_CRANKING_RPM + 1);
        EXPECT_EQ(DEFAULT_ALTERNATOR_VOLTAGE_TARGET, getSetpoint().value_or(-1));
    }
}