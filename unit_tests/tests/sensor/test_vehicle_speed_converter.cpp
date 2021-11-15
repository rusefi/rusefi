#include "pch.h"
#include "vehicle_speed_converter.h"

static constexpr engine_type_e ENGINE_TEST_HELPER = TEST_ENGINE;

class VehicleSpeedConverterTest : public ::testing::Test {

public:
	EngineTestHelper eth;
	VehicleSpeedConverter dut;

	VehicleSpeedConverterTest() : eth(ENGINE_TEST_HELPER) {
		
	}

	void SetUp() override {
		dut.inject(PASS_ENGINE_PARAMETER_SIGNATURE);
	}

	void SetCoef(float new_coef) {
		engineConfiguration->vehicleSpeedCoef = new_coef;
	}

	float GetFrequencyBySpeedAndCoef(float speed, float coef) {
		return (speed / coef);
	}

	void TestForSpeedWithCoef(float expectedSpeed, float coef)
	{
		SetCoef(coef);
		auto inputFreq = GetFrequencyBySpeedAndCoef(expectedSpeed, coef);
		auto result = dut.convert(inputFreq);
		ASSERT_TRUE(result.Valid);
		ASSERT_NEAR(expectedSpeed, result.Value, 0.01f);
	}
};

/*
 *  Converter must return valid and expected result for setted coef
 */
TEST_F(VehicleSpeedConverterTest, returnExpectedResultForSettedCoef) {
	
	TestForSpeedWithCoef(0.0f, 0.5f);
	TestForSpeedWithCoef(0.5f, 0.5f);
	TestForSpeedWithCoef(10.0f, 0.5f);
	TestForSpeedWithCoef(0.0f, 10.0f);
	TestForSpeedWithCoef(0.5f, 10.0f);
	TestForSpeedWithCoef(10.0f, 10.0f);
}

/*
 *  Converter must always return strong float zero if coef == 0.0f
 */
TEST_F(VehicleSpeedConverterTest, zeroCoefReturnsZeroSpeedOnAnyInput) {
	
	SetCoef(0.0f);

	{
		auto result = dut.convert(0.0f);
		ASSERT_TRUE(result.Valid);
		ASSERT_FLOAT_EQ(0.0f, result.Value);
	}

	{
		auto result = dut.convert(0.5f);
		ASSERT_TRUE(result.Valid);
		ASSERT_FLOAT_EQ(0.0f, result.Value);
	}

	{
		auto result = dut.convert(10.0f);
		ASSERT_TRUE(result.Valid);
		ASSERT_FLOAT_EQ(0.0f, result.Value);
	}
}
