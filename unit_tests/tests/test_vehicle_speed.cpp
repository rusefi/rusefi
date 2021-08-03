#include "pch.h"

#include "vehicle_speed.h"

extern void vsCallback(DECLARE_ENGINE_PARAMETER_SIGNATURE);
typedef void(*vss_callback_fp)(DECLARE_ENGINE_PARAMETER_SIGNATURE) ;

/*
 * 	Used to convert expected speed into simulation frequency
 */
static float speedToSimulationFrequency(float speedCoef, float speed)
{
	return speed/speedCoef;
}

/*
 * 	Use engine test helper as time source,
 * 	to simulate periodic signal on input pin with passed callback.
 */
static void simulatePeriodicSignalForCallback(
	EngineTestHelper& eth,
	float freqHz,
	vss_callback_fp cb
	DECLARE_ENGINE_PARAMETER_SUFFIX)
{
	constexpr auto periods = 50;
	auto period = (1 / freqHz);

	for (auto i = 0; i < periods; i++) {
		cb(PASS_ENGINE_PARAMETER_SIGNATURE);
		// Time rewind after the callback, due internal vehicle_speed.cpp logic
		// (last signal time check for stop tracking)
		eth.moveTimeForwardSec(period);
	}

}


TEST(VehicleSpeedSensor, testValidSpeedDetection) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	constexpr float expectedSpeed = 15.0f;
	constexpr float speedCoef = 0.5f;

	// Init global variables
	CONFIG(vehicleSpeedSensorInputPin) = GPIOA_0;
	brain_pin_markUsed(GPIOA_0, "VSS" PASS_ENGINE_PARAMETER_SUFFIX);
	engineConfiguration->vehicleSpeedCoef = speedCoef;

	float freq = speedToSimulationFrequency(engineConfiguration->vehicleSpeedCoef, expectedSpeed);
	simulatePeriodicSignalForCallback(eth, freq, vsCallback PASS_ENGINE_PARAMETER_SUFFIX);
	float measuredSpeed = getVehicleSpeed(PASS_ENGINE_PARAMETER_SIGNATURE);
	EXPECT_NEAR(expectedSpeed, measuredSpeed, 0.01);

}

TEST(VehicleSpeedSensor, testInvalidSpeed) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	constexpr float freq = 0.5f; // Invalid period > 1sec
	constexpr float returnedOnInvalidFreq = 0.0f;
	constexpr float speedCoef = 0.5f;

	// Init global variables
	CONFIG(vehicleSpeedSensorInputPin) = GPIOA_0;
	brain_pin_markUsed(GPIOA_0, "VSS" PASS_ENGINE_PARAMETER_SUFFIX);
	engineConfiguration->vehicleSpeedCoef = speedCoef;

	simulatePeriodicSignalForCallback(eth, freq, vsCallback PASS_ENGINE_PARAMETER_SUFFIX);
	float measuredSpeed = getVehicleSpeed(PASS_ENGINE_PARAMETER_SIGNATURE);
	EXPECT_EQ(returnedOnInvalidFreq, measuredSpeed);
}

