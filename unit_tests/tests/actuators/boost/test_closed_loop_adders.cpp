//
// Created by kifir on 7/31/24.
//

#include "pch.h"

#include "boost_test_base.h"

namespace {
    class ClosedLoopAddersTest : public BoostTestBase {
    protected:
        static constexpr float TEST_BOOST_CONTROL_TARGET = 14.0f;
        static constexpr float TEST_CLT_BOOST_ADDER_BINS[BOOST_CURVE_SIZE] = { 4.8f, 9.7f, 19.6f, 39.5f, 79.4f };
        static constexpr float TEST_CLT_BOOST_ADDER[BOOST_CURVE_SIZE] = { 1.8f, 1.3f, 0.9f, 0.6f, 0.4f };
        static constexpr float TEST_IAT_BOOST_ADDER_BINS[BOOST_CURVE_SIZE] = { 2.7f, 7.5f, 17.4f, 37.3f, 77.2f };
        static constexpr float TEST_IAT_BOOST_ADDER[BOOST_CURVE_SIZE] = { 0.5f, 0.4f, 0.6f, 0.9f, 1.3f };

        virtual void SetUp() override;

        void initTestCltBoostAdder();
        void initTestIatBoostAdder();
        void initLuaTargetCorrections(const float luaTargetMult, const int luaTargetAdd);
        static std::optional<float> getTestCltBoostBin(const int index);
        static std::optional<float> getTestIatBoostBin(const int index);

        void checkClosedLoopSetPoint(
            std::function<std::optional<float>(int)> cltExtractorByIndex,
            std::function<std::optional<float>(int)> iatExtractorByIndex,
            std::function<float(int)> expectedClosedLoopExtractorByIndex
        );

        void checkClosedLoopSetPoint(
            const std::optional<float> clt,
            const std::optional<float> iat,
            const float expectedClosedLoop
        );
    };

    void ClosedLoopAddersTest::SetUp() {
        BoostTestBase::SetUp();

        engineConfiguration->boostType = CLOSED_LOOP;

        setTable(config->boostTableClosedLoop, TEST_BOOST_CONTROL_TARGET);

        Sensor::setMockValue(SensorType::DriverThrottleIntent, 11.2f);
    }

    void ClosedLoopAddersTest::initTestCltBoostAdder() {
        initTestBoostCurve(
            TEST_CLT_BOOST_ADDER_BINS,
            config->cltBoostAdderBins,
            TEST_CLT_BOOST_ADDER,
            config->cltBoostAdder
        );
    }

    void ClosedLoopAddersTest::initTestIatBoostAdder() {
        initTestBoostCurve(
            TEST_IAT_BOOST_ADDER_BINS,
            config->iatBoostAdderBins,
            TEST_IAT_BOOST_ADDER,
            config->iatBoostAdder
        );
    }

    void ClosedLoopAddersTest::initLuaTargetCorrections(const float luaTargetMult, const int luaTargetAdd) {
        getBoostController().luaTargetMult = luaTargetMult;
        getBoostController().luaTargetAdd = luaTargetAdd;
    }

    std::optional<float> ClosedLoopAddersTest::getTestCltBoostBin(const int index) {
        return { TEST_CLT_BOOST_ADDER_BINS[index] };
    }

    std::optional<float> ClosedLoopAddersTest::getTestIatBoostBin(const int index) {
        return { TEST_IAT_BOOST_ADDER_BINS[index] };
    }

    void ClosedLoopAddersTest::checkClosedLoopSetPoint(
        std::function<std::optional<float>(int)> cltExtractorByIndex,
        std::function<std::optional<float>(int)> iatExtractorByIndex,
        std::function<float(int)> expectedClosedLoopExtractorByIndex
    ) {
        for (int i = 0; i< BOOST_CURVE_SIZE; i++) {
            checkClosedLoopSetPoint(
                cltExtractorByIndex(i),
                iatExtractorByIndex(i),
                expectedClosedLoopExtractorByIndex(i)
            );
        }
    }

    void ClosedLoopAddersTest::checkClosedLoopSetPoint(
            const std::optional<float> clt,
            const std::optional<float> iat,
            const float expectedSetPoint
    ) {
        if (clt.has_value()) {
            Sensor::setMockValue(SensorType::Clt, clt.value());
        }
        if (iat.has_value()) {
            Sensor::setMockValue(SensorType::Iat, iat.value());
        }
        const expected<percent_t> setPoint = getBoostController().getSetpoint();
        EXPECT_TRUE(setPoint.Valid) << "clt: " << clt.value_or(-1) << ", iat: " << iat.value_or(-1) ;
        ASSERT_NEAR(setPoint.Value, expectedSetPoint, EPS5D)
            << "clt: " << clt.value_or(-1) << ", iat: " << iat.value_or(-1) ;
    }

    TEST_F(ClosedLoopAddersTest, closedLoopWithDefaultCurves) {
        for (int i = 0; i < BOOST_CURVE_SIZE; i++) {
            EXPECT_EQ(config->cltBoostAdder[i], 0.0f) << "index: " << i; // check default adder
            EXPECT_EQ(config->iatBoostAdder[i], 0.0f) << "index: " << i; // check default adder
        }

        checkClosedLoopSetPoint({}, {}, TEST_BOOST_CONTROL_TARGET);
    }

    TEST_F(ClosedLoopAddersTest, closedLoopWithUninitializedCurves) {
        // Emulate configuration created with old version of TunerStudio, that doesn't support CLT and IAT boost curves:
        setArrayValues(config->cltBoostAdder, 0.0f);
        setArrayValues(config->cltBoostAdderBins, 0.0f);
        setArrayValues(config->iatBoostAdder, 0.0f);
        setArrayValues(config->iatBoostAdderBins, 0.0f);

        checkClosedLoopSetPoint({}, {}, TEST_BOOST_CONTROL_TARGET);
    }

    TEST_F(ClosedLoopAddersTest, closedLoopWithCltAdder) {
        initTestCltBoostAdder();

        checkClosedLoopSetPoint(
            ClosedLoopAddersTest::getTestCltBoostBin,
            ClosedLoopAddersTest::getTestIatBoostBin,
            [](const int i) -> float { return TEST_BOOST_CONTROL_TARGET + TEST_CLT_BOOST_ADDER[i]; }
        );
    }

    TEST_F(ClosedLoopAddersTest, closedLoopWithIatAdder) {
        initTestIatBoostAdder();

        checkClosedLoopSetPoint(
            ClosedLoopAddersTest::getTestCltBoostBin,
            ClosedLoopAddersTest::getTestIatBoostBin,
            [](const int i) -> float { return TEST_BOOST_CONTROL_TARGET + TEST_IAT_BOOST_ADDER[i]; }
        );
    }

    TEST_F(ClosedLoopAddersTest, closedLoopWithBothCltAndIatAdders) {
        initTestCltBoostAdder();
        initTestIatBoostAdder();

        checkClosedLoopSetPoint(
            ClosedLoopAddersTest::getTestCltBoostBin,
            ClosedLoopAddersTest::getTestIatBoostBin,
            [](const int i) -> float {
                return TEST_BOOST_CONTROL_TARGET + TEST_CLT_BOOST_ADDER[i] + TEST_IAT_BOOST_ADDER[i];
            }
        );
    }

    TEST_F(ClosedLoopAddersTest, closedLoopWithBothCltAndIatCorrectionWithMissedIatSensor) {
        initTestCltBoostAdder();
        initTestIatBoostAdder();

        checkClosedLoopSetPoint(
            ClosedLoopAddersTest::getTestCltBoostBin,
            emptyValue,
            [](const int i) -> float { return TEST_BOOST_CONTROL_TARGET + TEST_CLT_BOOST_ADDER[i]; }
        );
    }

    TEST_F(ClosedLoopAddersTest, closedLoopWithBothCltAndIatCorrectionWithMissedCltSensor) {
        initTestCltBoostAdder();
        initTestIatBoostAdder();

        checkClosedLoopSetPoint(
            emptyValue,
            ClosedLoopAddersTest::getTestIatBoostBin,
            [](const int i) -> float { return TEST_BOOST_CONTROL_TARGET + TEST_IAT_BOOST_ADDER[i]; }
        );
    }

    TEST_F(ClosedLoopAddersTest, closeLoopWithBothCltAndIatCorrectionAndLuaCorrections) {
        initTestCltBoostAdder();
        initTestIatBoostAdder();

        constexpr float TEST_LUA_TARGET_MULT = 1.2f;
        constexpr int TEST_LUA_TARGET_ADD = 3;
        initLuaTargetCorrections(TEST_LUA_TARGET_MULT, TEST_LUA_TARGET_ADD);

        checkClosedLoopSetPoint(
            getTestCltBoostBin,
            getTestIatBoostBin,
            [](const int i) -> float {
                return TEST_BOOST_CONTROL_TARGET * TEST_LUA_TARGET_MULT + static_cast<float>(TEST_LUA_TARGET_ADD)
                    + TEST_CLT_BOOST_ADDER[i] + TEST_IAT_BOOST_ADDER[i];
            }
        );
    }
}