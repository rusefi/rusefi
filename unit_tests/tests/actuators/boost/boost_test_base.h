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

    BoostController& getBoostController() const;
private:
    void initBoostCurveArray(const float (&src)[BOOST_CURVE_SIZE], float (&dst)[BOOST_CURVE_SIZE]);
};