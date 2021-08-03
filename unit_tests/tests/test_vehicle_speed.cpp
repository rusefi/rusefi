#include "pch.h"

#include "vehicle_speed.h"

extern void vsCallback(DECLARE_ENGINE_PARAMETER_SIGNATURE);
typedef void(*vss_callback_fp)(DECLARE_ENGINE_PARAMETER_SIGNATURE) ;

static constexpr brain_pin_e anyPin = GPIOA_0;
static constexpr const char* const vehicleSpeedSensorMessage = "VSS";

/*
 * 	Used to convert expected speed into simulation frequency
 */
static float speedToSimulationFrequency(float speedCoef, float speed) {
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

	// Init global variables
	CONFIG(vehicleSpeedSensorInputPin) = anyPin;
	brain_pin_markUsed(anyPin, vehicleSpeedSensorMessage PASS_ENGINE_PARAMETER_SUFFIX);
	engineConfiguration->vehicleSpeedCoef = 0.5f;

	// Valid speed 15kmh should be returned
	float freq = speedToSimulationFrequency(engineConfiguration->vehicleSpeedCoef, 15.0f);
	simulatePeriodicSignalForCallback(eth, freq, vsCallback PASS_ENGINE_PARAMETER_SUFFIX);
	float measuredSpeed = getVehicleSpeed(PASS_ENGINE_PARAMETER_SIGNATURE);
	EXPECT_NEAR(15.0f, measuredSpeed, 0.01);

}

TEST(VehicleSpeedSensor, testInvalidSpeed) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	// Init global variables
	CONFIG(vehicleSpeedSensorInputPin) = anyPin;
	brain_pin_markUsed(anyPin, vehicleSpeedSensorMessage PASS_ENGINE_PARAMETER_SUFFIX);
	engineConfiguration->vehicleSpeedCoef = 0.5f;

	// Invalid (slow) interval, should return 0 speed
	simulatePeriodicSignalForCallback(eth, 0.5f, vsCallback PASS_ENGINE_PARAMETER_SUFFIX);
	float measuredSpeed = getVehicleSpeed(PASS_ENGINE_PARAMETER_SIGNATURE);
	EXPECT_EQ(0.0f, measuredSpeed);
}

