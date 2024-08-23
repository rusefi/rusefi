//
// Created by kifir on 6/18/24.
//

#include "pch.h"

#include "ac_test_base.h"
#include "engine_configuration_defaults.h"

void AcTestBase::updateAcPressure(float acPressure) {
    Sensor::setMockValue(SensorType::AcPressure, acPressure);
    periodicSlowCallback();
}

void AcTestBase::setUpTestConfig(const AcTestConfig& config) {
    configureAcDelay(config.getAcDelay());
    configureMinAcPressure(config.getMinAcPressure());
    configureMaxAcPressure(config.getMaxAcPressure());
    configureAcPressureEnableHysteresis(config.getAcPressureEnableHysteresis());
}

void AcTestBase::configureAcDelay(const std::optional<float> acDelay) {
    if (acDelay.has_value()) {
        engineConfiguration->acDelay = acDelay.value();
    } else {
        ASSERT_EQ(engineConfiguration->acDelay, DEFAULT_AC_DELAY); // check default value
    }
}

void AcTestBase::configureMinAcPressure(const std::optional<uint16_t> minAcPressure) {
    if (minAcPressure.has_value()) {
        engineConfiguration->minAcPressure = minAcPressure.value();
    } else {
        ASSERT_EQ(engineConfiguration->minAcPressure, DEFAULT_MIN_AC_PRESSURE); // check default value
    }
}

void AcTestBase::configureMaxAcPressure(const std::optional<uint16_t> maxAcPressure) {
    if (maxAcPressure.has_value()) {
        engineConfiguration->maxAcPressure = maxAcPressure.value();
    } else {
        ASSERT_EQ(engineConfiguration->maxAcPressure, DEFAULT_MAX_AC_PRESSURE); // check default value
    }
}

void AcTestBase::configureAcPressureEnableHysteresis(std::optional<float> acPressureEnableHysteresis) {
    if (acPressureEnableHysteresis.has_value()) {
        engineConfiguration->acPressureEnableHyst = acPressureEnableHysteresis.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->acPressureEnableHyst,
            engine_configuration_defaults::AC_PRESSURE_ENABLE_HYST
        ); // check default value
    }
}