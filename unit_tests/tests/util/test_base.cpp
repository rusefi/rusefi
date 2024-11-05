//
// Created by kifir on 6/18/24.
//

#include "pch.h"

#include "test_base.h"

void TestBase::SetUp() {
    eth = std::make_unique<EngineTestHelper>(engine_type_e::TEST_ENGINE);
}

void TestBase::TearDown() {
    eth.reset();
}

TestEngineConfiguration& TestBase::getTestEngineConfiguration() {
    return TestEngineConfiguration::getInstance();
}

TestPersistentConfiguration& TestBase::getTestPersistentConfiguration() {
    return TestPersistentConfiguration::getInstance();
}

TestEngineState& TestBase::getTestEngineState() {
    return TestEngineState::getInstance();
}

void TestBase::periodicFastCallback() {
    // run the ignition math
    engine->periodicFastCallback();
}

void TestBase::periodicSlowCallback() {
    engine->periodicSlowCallback();
}

void TestBase::updateRpm(const float rpm) {
    Sensor::setMockValue(SensorType::Rpm, rpm);
    periodicFastCallback();
}

void TestBase::updateApp(const std::optional<float> app) {
    if (app.has_value()) {
        Sensor::setMockValue(SensorType::DriverThrottleIntent, app.value());
    } else {
        Sensor::resetMockValue(SensorType::DriverThrottleIntent);
    }
    periodicFastCallback();
}
