//
// Created by kifir on 7/31/24.
//

#pragma once

#include "util/test_base.h"

class BoostTestBase : public TestBase {
protected:
    using ValueByIndexRetriever = std::function<std::optional<float>(int)>;

    static const ValueByIndexRetriever emptyValue;

    virtual void SetUp() override;
    virtual void TearDown() override;

    void initTestBoostCurve(
        const float (&testBins)[BOOST_CURVE_SIZE],
        float (&dstBins)[BOOST_CURVE_SIZE],
        const float (&testValues)[BOOST_CURVE_SIZE],
        float (&dstValues)[BOOST_CURVE_SIZE]
    );

    std::unique_ptr<BoostController> bc;
private:
    void initBoostCurveArray(const float (&src)[BOOST_CURVE_SIZE], float (&dst)[BOOST_CURVE_SIZE]);
    void initBoostControllerTables();

    Map3D<BOOST_RPM_COUNT, BOOST_LOAD_COUNT, uint8_t, uint8_t, uint8_t> boostMapOpen{ "bo" };
    Map3D<BOOST_RPM_COUNT, BOOST_LOAD_COUNT, uint8_t, uint8_t, uint8_t> boostMapClosed{ "bc" };
    Map2D<BOOST_CURVE_SIZE, float, float> boostCltCorr { "clt" };
    Map2D<BOOST_CURVE_SIZE, float, float> boostIatCorr { "iat" };
    Map2D<BOOST_CURVE_SIZE, float, float> boostCltAdder { "clt (adder)" };
    Map2D<BOOST_CURVE_SIZE, float, float> boostIatAdder { "iat (adder)" };
    SimplePwm boostPwmControl { "boost" };
};