//
// Created by kifir on 6/18/24.
//

#pragma once


class TestBase : public testing::Test {
protected:
    void SetUp() override;
    void TearDown() override;

    void periodicFastCallback();
    void periodicSlowCallback();

    void updateRpm(int rpm);
private:
    std::unique_ptr<EngineTestHelper> eth;
};
