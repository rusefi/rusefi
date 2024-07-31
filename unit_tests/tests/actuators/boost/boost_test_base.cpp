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

    bc = std::make_unique<BoostController>();
    initBoostControllerTables();

    Sensor::resetAllMocks();
}

void BoostTestBase::TearDown() {
    bc.reset();
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

void BoostTestBase::initBoostCurveArray(const float (&src)[BOOST_CURVE_SIZE], float (&dst)[BOOST_CURVE_SIZE]) {
    std::copy(std::begin(src), std::end(src), std::begin(dst));
}

void BoostTestBase::initBoostControllerTables() {
    // The code below is very similar to code in file boost_control.cpp
    // TODO: think how we can get rid of duplicated code

    // Set up open & closed loop tables
    boostMapOpen.initTable(config->boostTableOpenLoop, config->boostRpmBins, config->boostTpsBins);
    boostMapClosed.initTable(config->boostTableClosedLoop, config->boostRpmBins, config->boostTpsBins);
    boostCltCorr.initTable(config->cltBoostCorr, config->cltBoostCorrBins);
    boostIatCorr.initTable(config->iatBoostCorr, config->iatBoostCorrBins);
    boostCltAdder.initTable(config->cltBoostAdder, config->cltBoostAdderBins);
    boostIatAdder.initTable(config->iatBoostAdder, config->iatBoostAdderBins);

    // Set up boost controller instance
    bc->init(
            &boostPwmControl,
            &boostMapOpen,
            &boostMapClosed,
            boostCltCorr,
            boostIatCorr,
            boostCltAdder,
            boostIatAdder,
            &engineConfiguration->boostPid
    );
}