//
// Created by kifir on 7/15/24.
//
#include "pch.h"

#include "util/test_base.h"

namespace {
    constexpr int TEST_CRANKING_RPM = 500;
    constexpr int TEST_LOAD = 17;
    constexpr float DEFAULT_ALTERNATOR_VOLTAGE_TARGET = 14.0f;

    using AlternatorVoltageTargetValues = float[ALTERNATOR_VOLTAGE_TARGET_SIZE][ALTERNATOR_VOLTAGE_TARGET_SIZE];
    using AlternatorVoltageTargetBins = uint16_t[ALTERNATOR_VOLTAGE_TARGET_SIZE];

    class AlternatorVoltageTargetSetPointTest : public TestBase {
    protected:
        virtual void SetUp() override;

        void initAlternatorVoltageTargetLoadBins(const AlternatorVoltageTargetBins& values);
        void initAlternatorVoltageTargetRpmBins(const AlternatorVoltageTargetBins& values);
        void initAlternatorVoltageTargetTable(const AlternatorVoltageTargetValues& values);

        void enableAlternatorControl();
        void disableAlternatorControl();

        void setRpmAndLoad(const float rpm, const float load);

        expected<float> getSetpoint();
    private:
        static void initAlternatorVoltageTargetBins(
            AlternatorVoltageTargetBins& dst,
            const AlternatorVoltageTargetBins& values
        );
    };

    void AlternatorVoltageTargetSetPointTest::SetUp() {
        TestBase::SetUp();
        engineConfiguration->cranking.rpm = TEST_CRANKING_RPM;
        enableAlternatorControl();
    }

    void AlternatorVoltageTargetSetPointTest::initAlternatorVoltageTargetLoadBins(
        const AlternatorVoltageTargetBins& values
    ) {
        initAlternatorVoltageTargetBins(config->alternatorVoltageTargetLoadBins, values);
    }

    void AlternatorVoltageTargetSetPointTest::initAlternatorVoltageTargetRpmBins(
        const AlternatorVoltageTargetBins& values
    ) {
        initAlternatorVoltageTargetBins(config->alternatorVoltageTargetRpmBins, values);
    }

    void AlternatorVoltageTargetSetPointTest::initAlternatorVoltageTargetTable(
        const AlternatorVoltageTargetValues& values
    ) {
        for (int loadIdx = 0; loadIdx < ALTERNATOR_VOLTAGE_TARGET_SIZE; loadIdx++) {
            for (int rpmIdx = 0; rpmIdx < ALTERNATOR_VOLTAGE_TARGET_SIZE; rpmIdx ++) {
                config->alternatorVoltageTargetTable[loadIdx][rpmIdx] = values[loadIdx][rpmIdx];
            }
        }
    }

    void AlternatorVoltageTargetSetPointTest::enableAlternatorControl() {
        engineConfiguration->isAlternatorControlEnabled = true;
    }

    void AlternatorVoltageTargetSetPointTest::disableAlternatorControl() {
        engineConfiguration->isAlternatorControlEnabled = false;
    }

    void AlternatorVoltageTargetSetPointTest::setRpmAndLoad(const float rpm, const float load = TEST_LOAD) {
        Sensor::setMockValue(SensorType::Rpm, rpm);
        engine->engineState.fuelingLoad = load;
    }

    void AlternatorVoltageTargetSetPointTest::initAlternatorVoltageTargetBins(
            AlternatorVoltageTargetBins& dst,
            const AlternatorVoltageTargetBins& values
    ) {
        for (int binIdx = 0; binIdx < ALTERNATOR_VOLTAGE_TARGET_SIZE; binIdx++) {
            dst[binIdx] = values[binIdx];
        }
    }

    expected<float> AlternatorVoltageTargetSetPointTest::getSetpoint() {
        return engine->engineModules.get<AlternatorController>()->getSetpoint();
    }

    TEST_F(AlternatorVoltageTargetSetPointTest, HomogeneousAlternatorVoltageTargetTable) {
        constexpr float TEST_ALTERNATOR_VOLTAGE_TARGET = 14.2f;
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

    TEST_F(AlternatorVoltageTargetSetPointTest, CustomAlternatorVoltageTargetTable) {
        constexpr AlternatorVoltageTargetBins TEST_ALTERNATOR_VOLTAGE_TARGET_LOAD_BINS = { 10, 60, 110, 160 };
        constexpr AlternatorVoltageTargetBins TEST_ALTERNATOR_VOLTAGE_TARGET_RPM_BINS = { 650, 2000, 3500, 7000 };
        constexpr AlternatorVoltageTargetValues TEST_ALTERNATOR_VOLTAGE_TARGETS {
                { 12.0f, 12.2f, 12.4f, 12.6f },
                { 12.8f, 13.0f, 13.2f, 13.4f },
                { 13.6f, 13.8f, 14.0f, 14.2f },
                { 14.4f, 14.6f, 14.8f, 15.0f },
        };

        initAlternatorVoltageTargetLoadBins(TEST_ALTERNATOR_VOLTAGE_TARGET_LOAD_BINS);
        initAlternatorVoltageTargetRpmBins(TEST_ALTERNATOR_VOLTAGE_TARGET_RPM_BINS);
        initAlternatorVoltageTargetTable(TEST_ALTERNATOR_VOLTAGE_TARGETS);

        /* Check alternator voltage target inside the table: */
        for (int loadIdx = 0; loadIdx < ALTERNATOR_VOLTAGE_TARGET_SIZE; loadIdx++) {
            for (int rpmIdx = 0; rpmIdx < ALTERNATOR_VOLTAGE_TARGET_SIZE; rpmIdx ++) {
                const float testLoad = TEST_ALTERNATOR_VOLTAGE_TARGET_LOAD_BINS[loadIdx];
                const int testRpm = TEST_ALTERNATOR_VOLTAGE_TARGET_RPM_BINS[rpmIdx];
                setRpmAndLoad(testRpm, testLoad);
                EXPECT_NEAR(TEST_ALTERNATOR_VOLTAGE_TARGETS[loadIdx][rpmIdx], getSetpoint().value_or(-1), EPS5D)
                    << "Unexpected alternator voltage target for load[" << loadIdx << "]=" << testLoad
                    << " and rpm[" << rpmIdx << "]=" << testRpm;
            }
        }
        /* Check alternator voltage target outside the table: */
        {
            const float testLoad = TEST_ALTERNATOR_VOLTAGE_TARGET_LOAD_BINS[0] - 10;
            const int testRpm = TEST_ALTERNATOR_VOLTAGE_TARGET_RPM_BINS[0] - 10;
            setRpmAndLoad(testRpm, testLoad);
            EXPECT_NEAR(TEST_ALTERNATOR_VOLTAGE_TARGETS[0][0], getSetpoint().value_or(-1), EPS5D)
            << "Unexpected alternator voltage target for load=" << testLoad
            << " and rpm=" << testRpm;
        }
        {
            const float testLoad = TEST_ALTERNATOR_VOLTAGE_TARGET_LOAD_BINS[0] - 10;
            const int testRpm = TEST_ALTERNATOR_VOLTAGE_TARGET_RPM_BINS[ALTERNATOR_VOLTAGE_TARGET_SIZE - 1] + 10;
            setRpmAndLoad(testRpm, testLoad);
            EXPECT_NEAR(
                    TEST_ALTERNATOR_VOLTAGE_TARGETS[0][ALTERNATOR_VOLTAGE_TARGET_SIZE - 1],
                    getSetpoint().value_or(-1),
                    EPS5D
            ) << "Unexpected alternator voltage target for load=" << testLoad << " and rpm=" << testRpm;
        }
        {
            const float testLoad = TEST_ALTERNATOR_VOLTAGE_TARGET_LOAD_BINS[ALTERNATOR_VOLTAGE_TARGET_SIZE - 1] + 10;
            const int testRpm = TEST_ALTERNATOR_VOLTAGE_TARGET_RPM_BINS[0] - 10;
            setRpmAndLoad(testRpm, testLoad);
            EXPECT_NEAR(
                TEST_ALTERNATOR_VOLTAGE_TARGETS[ALTERNATOR_VOLTAGE_TARGET_SIZE - 1][0],
                getSetpoint().value_or(-1),
                EPS5D
            ) << "Unexpected alternator voltage target for load=" << testLoad << " and rpm=" << testRpm;
        }
        {
            const float testLoad = TEST_ALTERNATOR_VOLTAGE_TARGET_LOAD_BINS[ALTERNATOR_VOLTAGE_TARGET_SIZE - 1] + 10;
            const int testRpm = TEST_ALTERNATOR_VOLTAGE_TARGET_RPM_BINS[ALTERNATOR_VOLTAGE_TARGET_SIZE - 1] + 10;
            setRpmAndLoad(testRpm, testLoad);
            EXPECT_NEAR(
                TEST_ALTERNATOR_VOLTAGE_TARGETS[ALTERNATOR_VOLTAGE_TARGET_SIZE - 1][ALTERNATOR_VOLTAGE_TARGET_SIZE - 1],
                getSetpoint().value_or(-1),
                EPS5D
            ) << "Unexpected alternator voltage target for load=" << testLoad << " and rpm=" << testRpm;
        }
    }
}
