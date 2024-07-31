//
// Created by kifir on 7/30/24.
//

#include "pch.h"

#include "boost_test_base.h"

namespace {
    class OpenLoopMultipliersTest : public BoostTestBase {
    protected:
        static constexpr float TEST_BOOST_CONTROL_DUTY_CYCLE = 17.0f;
        static constexpr float TEST_CLT_BOOST_CORR_BINS[BOOST_CURVE_SIZE] = { 5.9f, 10.8f, 20.7f, 40.6f, 80.5f };
        static constexpr float TEST_CLT_BOOST_CORR[BOOST_CURVE_SIZE] = { 1.9f, 1.4f, 1.0f, 0.7f, 0.5f };
        static constexpr float TEST_IAT_BOOST_CORR_BINS[BOOST_CURVE_SIZE] = { 0.1f, 5.2f, 15.3f, 35.4f, 75.5f };
        static constexpr float TEST_IAT_BOOST_CORR[BOOST_CURVE_SIZE] = { 0.4f, 0.5f, 0.7f, 1.0f, 1.4f };

        virtual void SetUp() override;

        void initTestCltBoostCorr();
        void initTestIatBoostCorr();
        void initLuaOpenLoopAdd(const float value);
        static std::optional<float> getTestCltBoostBin(const int index);
        static std::optional<float> getTestIatBoostBin(const int index);

        void checkOpenLoop(
            ValueByIndexRetriever cltExtractorByIndex,
            ValueByIndexRetriever iatExtractorByIndex,
            std::function<float(int)> expectedOpenLoopExtractorByIndex
        );

        void checkOpenLoop(
            const std::optional<float> clt,
            const std::optional<float> iat,
            const float expectedOpenLoop
        );
    };

    void OpenLoopMultipliersTest::SetUp() {
        BoostTestBase::SetUp();

        setTable(config->boostTableOpenLoop, TEST_BOOST_CONTROL_DUTY_CYCLE);

        Sensor::setMockValue(SensorType::Tps1, 42.1f);
    }

    void OpenLoopMultipliersTest::initTestCltBoostCorr() {
        initTestBoostCurve(TEST_CLT_BOOST_CORR_BINS, config->cltBoostCorrBins, TEST_CLT_BOOST_CORR, config->cltBoostCorr);
    }

    void OpenLoopMultipliersTest::initTestIatBoostCorr() {
        initTestBoostCurve(TEST_IAT_BOOST_CORR_BINS, config->iatBoostCorrBins, TEST_IAT_BOOST_CORR, config->iatBoostCorr);
    }

    void OpenLoopMultipliersTest::initLuaOpenLoopAdd(const float value) {
        bc->luaOpenLoopAdd = value;
    }

    std::optional<float> OpenLoopMultipliersTest::getTestCltBoostBin(const int index) {
        return {TEST_CLT_BOOST_CORR_BINS[index] };
    }

    std::optional<float> OpenLoopMultipliersTest::getTestIatBoostBin(const int index) {
        return {TEST_IAT_BOOST_CORR_BINS[index] };
    }

    void OpenLoopMultipliersTest::checkOpenLoop(
        ValueByIndexRetriever cltExtractorByIndex,
        ValueByIndexRetriever iatExtractorByIndex,
        std::function<float(int)> expectedOpenLoopExtractorByIndex
    ) {
        for (int i = 0; i< BOOST_CURVE_SIZE; i++) {
            checkOpenLoop(cltExtractorByIndex(i), iatExtractorByIndex(i), expectedOpenLoopExtractorByIndex(i));
        }
    }

    void OpenLoopMultipliersTest::checkOpenLoop(
        const std::optional<float> clt,
        const std::optional<float> iat,
        const float expectedOpenLoop
    ) {
        if (clt.has_value()) {
            Sensor::setMockValue(SensorType::Clt, clt.value());
        }
        if (iat.has_value()) {
            Sensor::setMockValue(SensorType::Iat, iat.value());
        }
        const expected<percent_t> openLoop = bc->getOpenLoop(0.0f);
        EXPECT_TRUE(openLoop.Valid) << "clt: " << clt.value_or(-1) << ", iat: " << iat.value_or(-1) ;
        EXPECT_EQ(openLoop.Value, expectedOpenLoop) << "clt: " << clt.value_or(-1) << ", iat: " << iat.value_or(-1) ;
    }

    TEST_F(OpenLoopMultipliersTest, openLoopWithDefaultCurves) {
        for (int i = 0; i < BOOST_CURVE_SIZE; i++) {
            EXPECT_EQ(config->cltBoostCorr[i], 1.0f) << "index: " << i; // check default multiplier
            EXPECT_EQ(config->iatBoostCorr[i], 1.0f) << "index: " << i; // check default multiplier
        }

        checkOpenLoop({}, {}, TEST_BOOST_CONTROL_DUTY_CYCLE);
    }

    TEST_F(OpenLoopMultipliersTest, openLoopWithUninitializedCurves) {
        // Emulate configuration created with old version of TunerStudio, that doesn't support CLT and IAT boost curves:
        setArrayValues(config->cltBoostCorr, 0.0f);
        setArrayValues(config->cltBoostCorrBins, 0.0f);
        setArrayValues(config->iatBoostCorr, 0.0f);
        setArrayValues(config->iatBoostCorrBins, 0.0f);

        checkOpenLoop({}, {}, TEST_BOOST_CONTROL_DUTY_CYCLE);
    }

    TEST_F(OpenLoopMultipliersTest, openLoopWithCltCorrection) {
        initTestCltBoostCorr();

        checkOpenLoop(
            OpenLoopMultipliersTest::getTestCltBoostBin,
            OpenLoopMultipliersTest::getTestIatBoostBin,
            [](const int i) -> float { return TEST_BOOST_CONTROL_DUTY_CYCLE * TEST_CLT_BOOST_CORR[i]; }
        );
    }

    TEST_F(OpenLoopMultipliersTest, openLoopWithIatCorrection) {
        initTestIatBoostCorr();

        checkOpenLoop(
            OpenLoopMultipliersTest::getTestCltBoostBin,
            OpenLoopMultipliersTest::getTestIatBoostBin,
            [](const int i) -> float { return TEST_BOOST_CONTROL_DUTY_CYCLE * TEST_IAT_BOOST_CORR[i]; }
        );
    }

    TEST_F(OpenLoopMultipliersTest, openLoopWithBothCltAndIatCorrection) {
        initTestCltBoostCorr();
        initTestIatBoostCorr();

        checkOpenLoop(
            OpenLoopMultipliersTest::getTestCltBoostBin,
            OpenLoopMultipliersTest::getTestIatBoostBin,
            [](const int i) -> float {
                return TEST_BOOST_CONTROL_DUTY_CYCLE * TEST_CLT_BOOST_CORR[i] * TEST_IAT_BOOST_CORR[i];
            }
        );
    }

    TEST_F(OpenLoopMultipliersTest, openLoopWithBothCltAndIatCorrectionWithMissedIatSensor) {
        initTestCltBoostCorr();
        initTestIatBoostCorr();

        checkOpenLoop(
            OpenLoopMultipliersTest::getTestCltBoostBin,
            emptyValue,
            [](const int i) -> float { return TEST_BOOST_CONTROL_DUTY_CYCLE * TEST_CLT_BOOST_CORR[i]; }
        );
    }

    TEST_F(OpenLoopMultipliersTest, openLoopWithBothCltAndIatCorrectionWithMissedCltSensor) {
        initTestCltBoostCorr();
        initTestIatBoostCorr();

        checkOpenLoop(
            emptyValue,
            OpenLoopMultipliersTest::getTestIatBoostBin,
            [](const int i) -> float { return TEST_BOOST_CONTROL_DUTY_CYCLE * TEST_IAT_BOOST_CORR[i]; }
        );
    }

    TEST_F(OpenLoopMultipliersTest, openLoopWithBothCltAndIatCorrectionAndLuaOpenLoopAdd) {
        initTestCltBoostCorr();
        initTestIatBoostCorr();

        constexpr float TEST_LUA_OPEN_LOOP_ADD = 239.17;
        initLuaOpenLoopAdd(TEST_LUA_OPEN_LOOP_ADD);

        checkOpenLoop(
            OpenLoopMultipliersTest::getTestCltBoostBin,
            OpenLoopMultipliersTest::getTestIatBoostBin,
            [](const int i) -> float {
                return TEST_LUA_OPEN_LOOP_ADD
                    + TEST_BOOST_CONTROL_DUTY_CYCLE * TEST_CLT_BOOST_CORR[i] * TEST_IAT_BOOST_CORR[i];
            }
        );
    }
}