#include "pch.h"

#include "frequency_sensor.h"

class FrequencySensorTest : public ::testing::Test {
protected:
	void SetUp() override {
		// If somehow prodcode will be unwrapped for test it MAYBE! will fire with error.
		// At least we must init FlexSensor somehow
		dut.init(GPIO_INVALID);

		Sensor::resetRegistry();
	}

	void TearDown() override {
		Sensor::resetRegistry();
	}

	/*
	 *  This method must simulate some amount periods of square-wave
	 *  and fire callback on every falling edge.
	 *  (as Sensor works by falling edge)
	 */
	void generatePwm(EngineTestHelper &eth, float freqHz) {
		auto const PERIODS_TO_SIMULATE = 50;
		auto period = (1 / freqHz);

		std::cout << "PERIOD: " << period << std::endl;

		for (auto i = PERIODS_TO_SIMULATE; i > 0; i--) {
			eth.moveTimeForwardSec(period);
			dut.onEdge(getTimeNowNt());
		}
	}

	FrequencySensor dut(SensorType::FuelEthanolPercent, MS2NT(50));
};

/*
 *  Sensor must take PWM input on "valid" frequency and generate any input.
 */
TEST_F(FrequencySensorTest, testValidWithPwm) {
	ASSERT_TRUE(dut.Register());
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	// Should be invalid - not set yet
	{
		auto s = Sensor::get(SensorType::FuelEthanolPercent);
		EXPECT_FALSE(s.Valid);
	}

	generatePwm(eth, 10);

	// Should be valid
	{
		auto s = Sensor::get(SensorType::FuelEthanolPercent);
		EXPECT_TRUE(s.Valid);
		EXPECT_FLOAT_EQ(s.Value, 10);
	}
}
