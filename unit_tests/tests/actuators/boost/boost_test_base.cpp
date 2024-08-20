//
// Created by kifir on 7/31/24.
//

#include "pch.h"

#include "boost_test_base.h"

const BoostTestBase::ValueByIndexRetriever BoostTestBase::emptyValue = [](const int) -> std::optional<float> {
    return {};
};

void BoostTestBase::SetUp() {
    TestBase::SetUp();

    initBoostCtrl();

    Sensor::resetAllMocks();
}

void BoostTestBase::TearDown() {
    TestBase::TearDown();
}

void BoostTestBase::initTestBoostCurve(
    const float (&testBins)[BOOST_CURVE_SIZE],
    float (&dstBins)[BOOST_CURVE_SIZE],
    const float (&testValues)[BOOST_CURVE_SIZE],
    float (&dstValues)[BOOST_CURVE_SIZE]
) {
    initBoostCurveArray(testBins, dstBins);
    initBoostCurveArray(testValues, dstValues);
}

BoostController& BoostTestBase::getBoostController() const {
    return engine->module<BoostController>().unmock();
}

void BoostTestBase::initBoostCurveArray(const float (&src)[BOOST_CURVE_SIZE], float (&dst)[BOOST_CURVE_SIZE]) {
    std::copy(std::begin(src), std::end(src), std::begin(dst));
}