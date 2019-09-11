#include "function_pointer_sensor.h"

#include <gtest/gtest.h>

class SensorFunctionPointer : public ::testing::Test
{
protected:
    void SetUp() override
    {
        Sensor::ResetRegistry();
    }

    void TearDown() override
    {
        Sensor::ResetRegistry();
    }
};

float testFunc()
{
    return 23;
}

TEST_F(SensorFunctionPointer, TestValue)
{
    FunctionPointerSensor dut(SensorType::Clt, testFunc);
    dut.Register();

    auto result = Sensor::Get(SensorType::Clt);
    EXPECT_TRUE(result.Valid);
    EXPECT_FLOAT_EQ(result.Value, 23);
}
