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
public:
    void periodicSlowCallback();
protected:
    void updateVehicleSpeed(
        std::optional<float> speed,
        void (TestBase::*postAction)() = &TestBase::periodicFastCallback
    );
    void updateRpm(std::optional<float> rpm, void (TestBase::*postAction)() = &TestBase::periodicFastCallback);
    void updateApp(std::optional<float> app, void (TestBase::*postAction)() = &TestBase::periodicFastCallback);
    void updateClt(std::optional<float> clt, void (TestBase::*postAction)() = &TestBase::periodicFastCallback);
    void updateMap(std::optional<float> map, void (TestBase::*postAction)() = &TestBase::periodicFastCallback);
    void updateLambda1(std::optional<float> lambda1, void (TestBase::*postAction)() = &TestBase::periodicFastCallback);

    template<typename ModuleType> ModuleType& getModule();
private:
    void updateSensor(SensorType sensor, std::optional<float> sensorReading, void (TestBase::*postAction)());

    std::unique_ptr<EngineTestHelper> eth;
};

template<typename ModuleType> ModuleType& TestBase::getModule() {
    return engine->module<ModuleType>().unmock();
}
