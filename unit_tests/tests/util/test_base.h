//
// Created by kifir on 6/18/24.
//

#pragma once


class TestBase : public testing::Test {
protected:
    virtual void SetUp() override;
    virtual void TearDown() override;

    void periodicFastCallback();
    void periodicSlowCallback();

    void updateRpm(const int rpm);
private:
    std::unique_ptr<EngineTestHelper> eth;
};
