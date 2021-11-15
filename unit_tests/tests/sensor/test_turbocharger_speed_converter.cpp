#include "pch.h"
#include "turbocharger_speed_converter.h"

static constexpr engine_type_e ENGINE_TEST_HELPER = TEST_ENGINE;

class TurbochargerSpeedConverterTest : public ::testing::Test {

public:
	TurbochargerSpeedConverter dut;

	TurbochargerSpeedConverterTest(){
		
	}

	void SetUp() override {
		WITH_ENGINE_TEST_HELPER(ENGINE_TEST_HELPER);
		dut.inject(PASS_ENGINE_PARAMETER_SIGNATURE);
	}

	void SetCoef(float new_coef) {
		dut.engineConfiguration->turboSpeedSensorMultiplier = new_coef;
	}

	float GetFrequencyBySpeedAndCoef(float speed, float coef) {
		return (speed / coef) / 60;
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
TEST_F(TurbochargerSpeedConverterTest, returnExpectedResultForSettedCoef) {
	
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
TEST_F(TurbochargerSpeedConverterTest, zeroCoefReturnsZeroSpeedOnAnyInput) {
	
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