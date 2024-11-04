//
// Created by kifir on 6/18/24.
//

#pragma once

#include "test_engine_configuration.h"
#include "test_engine_state.h"

class TestBase : public testing::Test {
protected:
    void SetUp() override;
    void TearDown() override;

    TestEngineConfiguration& getTestEngineConfiguration();
    TestEngineState& getTestEngineState();

    void periodicFastCallback();
    void periodicSlowCallback();

    void updateRpm(float rpm);
    void updateApp(std::optional<float> app);
private:
    std::unique_ptr<EngineTestHelper> eth;
};
