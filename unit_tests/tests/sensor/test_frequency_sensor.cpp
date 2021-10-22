#include "pch.h"

#include "frequency_sensor.h"
#include "sensor_type.h"

// Fake converter just passes value straight though
struct IdentityFunction : public SensorConverter {
	SensorResult convert(float raw) const {
		return raw;
	}
};

static IdentityFunction identityFunc;

class FrequencySensorTest : public ::testing::Test {
public:
	FrequencySensorTest()
		: dut(SensorType::FuelEthanolPercent, MS2NT(50))
	{
	}

	void SetUp() override {
		// If somehow prodcode will be unwrapped for test it MAYBE! will fire with error.
		// At least we must init FlexSensor somehow
		dut.init(GPIO_INVALID);
		dut.setFunction(identityFunc);
	}

	/*
	 *  This method must simulate some amount periods of square-wave
	 *  and fire callback on every falling edge.
	 *  (as Sensor works by falling edge)
	 */
	void generatePwm(EngineTestHelper &eth, float freqHz) {
		constexpr auto periods = 50;
		auto period = (1 / freqHz);

		std::cout << "PERIOD: " << period << std::endl;

		for (auto i = 0; i < periods; i++) {
			eth.moveTimeForwardSec(period);
			dut.onEdge(getTimeNowNt());
		}
	}

	FrequencySensor dut;
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

	// Should be valid, correct frequency
	{
		auto s = Sensor::get(SensorType::FuelEthanolPercent);
		EXPECT_TRUE(s.Valid);
		EXPECT_FLOAT_EQ(s.Value, 10);
	}
}
