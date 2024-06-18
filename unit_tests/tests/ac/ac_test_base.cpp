//
// Created by kifir on 6/18/24.
//

#include "pch.h"

#include "ac_test_base.h"

void AcTestBase::updateAcPressure(float acPressure) {
    Sensor::setMockValue(SensorType::AcPressure, acPressure);
    periodicSlowCallback();
}

void AcTestBase::setUpTestConfig(const AcTestConfig& config) {
    configureMinAcPressure(config.getMinAcPressure());
    configureMaxAcPressure(config.getMaxAcPressure());
}

void AcTestBase::configureMinAcPressure(const std::optional<uint16_t> minAcPressure) {
    if (minAcPressure.has_value()) {
        engineConfiguration->minAcPressure = minAcPressure.value();
    } else {
        ASSERT_EQ(engineConfiguration->minAcPressure, 0); // check default value
    }
}

void AcTestBase::configureMaxAcPressure(const std::optional<uint16_t> maxAcPressure) {
    if (maxAcPressure.has_value()) {
        engineConfiguration->maxAcPressure = maxAcPressure.value();
    } else {
        ASSERT_EQ(engineConfiguration->maxAcPressure, 0); // check default value
    }
}
