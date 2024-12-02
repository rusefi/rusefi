//
// Created by kifir on 6/18/24.
//

#pragma once

#include "test_engine_configuration.h"
#include "test_engine_state.h"
#include "test_persistent_configuration.h"
#include "engine_config.h"

class TestBase : public testing::Test {
protected:
    void SetUp() override;
    void TearDown() override;

    TestEngineConfiguration& getTestEngineConfiguration();
    TestEngineState& getTestEngineState();
    TestPersistentConfiguration& getTestPersistentConfiguration();

    void setUpEngineConfiguration(const EngineConfig& config);

    void periodicFastCallback();
    void periodicSlowCallback();

    void updateRpm(std::optional<float> rpm);
    void updateApp(std::optional<float> app);
    void updateClt(std::optional<float> clt);
    void updateMap(std::optional<float> map);
    void updateLambda1(std::optional<float> lambda1);

    template<typename ModuleType> ModuleType& getModule();
private:
    void updateSensor(SensorType sensor, std::optional<float> sensorReading);

    std::unique_ptr<EngineTestHelper> eth;
};

template<typename ModuleType> ModuleType& TestBase::getModule() {
    return engine->module<ModuleType>().unmock();
}
