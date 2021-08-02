#include "flex_sensor.h"
#include "engine_test_helper.h" // Used for timesourcing
#include "global.h"

#include <gtest/gtest.h>

class FlexSensorForTests : public ::testing::Test
{

protected:

    FlexSensorForTests () :
            dut ()
    {
    }
    ;

    void
    SetUp () override
    {
        // If somehow prodcode will be unwrapped for test it MAYBE! will fire with error.
        // At least we must init FlexSensor somehow
        dut.init (GPIO_INVALID);

        Sensor::resetRegistry (); // IDK why i must use this. Just copypaste.
    }

    void
    TearDown () override
    {
        Sensor::resetRegistry ();
    }

    /*
     *  This method must simulate some amount periods of square-wave
     *  and fire callback on every falling edge.
     *  (as Sensor works by falling edge)
     */
    void
    WithEngineHelperGenerateInputPWM (EngineTestHelper &eth, float freqHz)
    {
        auto const PERIODS_TO_SIMULATE = 50;
        auto period = (1 / freqHz);

        std::cout << "PERIOD: " << period << std::endl;
        for (auto i = PERIODS_TO_SIMULATE; i > 0; i--)
            {
                eth.moveTimeForwardSec (period);
                dut.onEdge (getTimeNowNt ());
            }
    }

    // todo: some function to convert output value to frequency?
    float
    ConvertExpectedValueToFrequency (float expectedValue)
    {
        return VALID_LOW_FREQ;
    }

    const float INVALID_LOW_FREQ = 44.0f;
    const float VALID_LOW_FREQ = 46.0f;
    const float INVALID_HIGH_FREQ = 156.0f;
    const float VALID_HIGH_FREQ = 154.0f;
    const float VALID_FREQ = 100.0f;

    FlexFuelSensor dut;
};

/*
 *  Sensor must take PWM input on "valid" frequency and generate any input.
 */
TEST_F(FlexSensorForTests, testCreationAndGeneratingAnyValidResult)
{

    ASSERT_TRUE(dut.Register ());
    WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

    // Should be invalid - not set yet
        {
            auto s = Sensor::get (SensorType::FuelEthanolPercent);
            EXPECT_FALSE(s.Valid);
        }

    WithEngineHelperGenerateInputPWM (eth, VALID_FREQ);
    // Should be valid
        {
            auto s = Sensor::get (SensorType::FuelEthanolPercent);
            EXPECT_TRUE(s.Valid);
        }
}

/*
 *  Sensor must take PWM input on "invalid" frequency and generate no input.
 */
TEST_F(FlexSensorForTests, testInvalidFrequency)
{

    ASSERT_TRUE(dut.Register ());
    WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

    // Should be invalid - not set yet
        {
            auto s = Sensor::get (SensorType::FuelEthanolPercent);
            EXPECT_FALSE(s.Valid);
        }

    WithEngineHelperGenerateInputPWM (eth, INVALID_LOW_FREQ);
    // Should be invalid - too low freq
        {
            auto s = Sensor::get (SensorType::FuelEthanolPercent);
            EXPECT_FALSE(s.Valid);
        }

    WithEngineHelperGenerateInputPWM (eth, INVALID_HIGH_FREQ);
    // Should be invalid - too high freq
        {
            auto s = Sensor::get (SensorType::FuelEthanolPercent);
            EXPECT_FALSE(s.Valid);
        }
}

/*
 *  Sensor must take PWM input on frequency with "known output value"
 *  and generate similiar output.
 */
TEST_F(FlexSensorForTests, testFrequencyConversion)
{

    ASSERT_TRUE(dut.Register ());
    WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

    // Should be invalid - not set yet
        {
            auto s = Sensor::get (SensorType::FuelEthanolPercent);
            EXPECT_FALSE(s.Valid);
        }

    float expectedValue = 75; // Any between 0 and 100
    float freq = ConvertExpectedValueToFrequency (expectedValue);

    WithEngineHelperGenerateInputPWM (eth, freq);
    // Should be invalid - too low freq
        {
            auto s = Sensor::get (SensorType::FuelEthanolPercent);
            EXPECT_TRUE(s.Valid);
            EXPECT_FLOAT_EQ(s.Value, expectedValue);
        }
}
