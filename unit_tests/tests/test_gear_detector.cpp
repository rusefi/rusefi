#include "pch.h"

float GetGearRatioFor(float revPerKm, float axle, float kph, float rpm) {
	EngineTestHelper eth(TEST_ENGINE);

	engineConfiguration->driveWheelRevPerKm = revPerKm;
	engineConfiguration->finalGearRatio = axle;

	Sensor::setMockValue(SensorType::VehicleSpeed, kph);
	Sensor::setMockValue(SensorType::Rpm, rpm);

	GearDetector dut;
	dut.onSlowCallback();

	return Sensor::getOrZero(SensorType::DetectedGear);
}

TEST(GearDetector, ComputeGearRatio) {
	// real gears from Volvo racecar
	EXPECT_NEAR_M3(3.35f, GetGearRatioFor(507, 4.1, 29.45f / 0.6214f, 5500));
	EXPECT_NEAR_M3(1.99f, GetGearRatioFor(507, 4.1, 49.57f / 0.6214f, 5500));
	EXPECT_NEAR_M3(1.33f, GetGearRatioFor(507, 4.1, 74.18f / 0.6214f, 5500));
	EXPECT_NEAR_M3(1.00f, GetGearRatioFor(507, 4.1, 98.65f / 0.6214f, 5500));
	EXPECT_NEAR_M3(0.72f, GetGearRatioFor(507, 4.1, 137.02f / 0.6214f, 5500));

	// Idling, car stopped, check no div/0
	EXPECT_EQ(0, GetGearRatioFor(507, 4.1, 0, 800));
}


TEST(GearDetector, GetRpmInGear) {
	EngineTestHelper eth(TEST_ENGINE);

	engineConfiguration->driveWheelRevPerKm = 507;
	engineConfiguration->finalGearRatio = 4.10f;

	// real gears from Volvo racecar
	engineConfiguration->totalGearsCount = 5;
	engineConfiguration->gearRatio[0] = 3.35f;
	engineConfiguration->gearRatio[1] = 1.99f;
	engineConfiguration->gearRatio[2] = 1.33f;
	engineConfiguration->gearRatio[3] = 1.00f;
	engineConfiguration->gearRatio[4] = 0.72f;

	GearDetector dut;

	Sensor::setMockValue(SensorType::VehicleSpeed, 29.45f / 0.6214f);
	EXPECT_NEAR(5500, dut.getRpmInGear(1), 1);
	Sensor::setMockValue(SensorType::VehicleSpeed, 49.57f / 0.6214f);
	EXPECT_NEAR(5500, dut.getRpmInGear(2), 1);
	Sensor::setMockValue(SensorType::VehicleSpeed, 74.18f / 0.6214f);
	EXPECT_NEAR(5500, dut.getRpmInGear(3), 1);
	Sensor::setMockValue(SensorType::VehicleSpeed, 98.65f / 0.6214f);
	EXPECT_NEAR(5500, dut.getRpmInGear(4), 1);
	Sensor::setMockValue(SensorType::VehicleSpeed, 137.02f / 0.6214f);
	EXPECT_NEAR(5500, dut.getRpmInGear(5), 1);

	// Test some invalid cases
	EXPECT_FLOAT_EQ(0, dut.getRpmInGear(0));
	EXPECT_FLOAT_EQ(0, dut.getRpmInGear(10));

	// Zero vehicle speed shouldn't cause a problem
	Sensor::setMockValue(SensorType::VehicleSpeed, 0);
	EXPECT_FLOAT_EQ(0, dut.getRpmInGear(0));
	EXPECT_FLOAT_EQ(0, dut.getRpmInGear(1));
	EXPECT_FLOAT_EQ(0, dut.getRpmInGear(5));
	EXPECT_FLOAT_EQ(0, dut.getRpmInGear(10));
}

TEST(GearDetector, DetermineGearSingleSpeed) {
	EngineTestHelper eth(TEST_ENGINE);
	GearDetector dut;

	engineConfiguration->totalGearsCount = 1;
	engineConfiguration->gearRatio[0] = 2;

	dut.onConfigurationChange(nullptr);

	// Super high ratios indicate clutch slip or idling in neutral or something
	EXPECT_EQ(0, dut.determineGearFromRatio(100));
	EXPECT_EQ(0, dut.determineGearFromRatio(4));

	// Check exactly on the gear
	EXPECT_EQ(1, dut.determineGearFromRatio(2));

	// Check near the gear
	EXPECT_EQ(1, dut.determineGearFromRatio(2.1));
	EXPECT_EQ(1, dut.determineGearFromRatio(1.9));

	// Extremely low ratio suggests stopped engine at speed?
	EXPECT_EQ(0, dut.determineGearFromRatio(1.0));
}

TEST(GearDetector, DetermineGear5Speed) {
	EngineTestHelper eth(TEST_ENGINE);
	GearDetector dut;

	engineConfiguration->totalGearsCount = 5;
	engineConfiguration->gearRatio[0] = 3.35;
	engineConfiguration->gearRatio[1] = 1.99;
	engineConfiguration->gearRatio[2] = 1.33;
	engineConfiguration->gearRatio[3] = 1.00;
	engineConfiguration->gearRatio[4] = 0.72;

	dut.onConfigurationChange(nullptr);

	// Super high ratios indicate clutch slip or idling in neutral or something
	EXPECT_EQ(0, dut.determineGearFromRatio(100));
	EXPECT_EQ(0, dut.determineGearFromRatio(6));

	// Check exactly on gears
	EXPECT_EQ(1, dut.determineGearFromRatio(3.35));
	EXPECT_EQ(2, dut.determineGearFromRatio(1.99));
	EXPECT_EQ(3, dut.determineGearFromRatio(1.33));
	EXPECT_EQ(4, dut.determineGearFromRatio(1.00));
	EXPECT_EQ(5, dut.determineGearFromRatio(0.72));

	// Check near each gear
	EXPECT_EQ(1, dut.determineGearFromRatio(3.45));
	EXPECT_EQ(1, dut.determineGearFromRatio(3.25));

	EXPECT_EQ(2, dut.determineGearFromRatio(2.2));
	EXPECT_EQ(2, dut.determineGearFromRatio(1.8));

	EXPECT_EQ(3, dut.determineGearFromRatio(1.45));
	EXPECT_EQ(3, dut.determineGearFromRatio(1.25));

	EXPECT_EQ(4, dut.determineGearFromRatio(1.1));
	EXPECT_EQ(4, dut.determineGearFromRatio(0.9));

	EXPECT_EQ(5, dut.determineGearFromRatio(0.8));
	EXPECT_EQ(5, dut.determineGearFromRatio(0.6));

	// Extremely low ratio suggests stopped engine at speed?
	EXPECT_EQ(0, dut.determineGearFromRatio(0.1));
}

TEST(GearDetector, MiataNb6Speed) {
	EngineTestHelper eth(TEST_ENGINE);
	GearDetector dut;

	engineConfiguration->totalGearsCount = 6;
	engineConfiguration->gearRatio[0] = 3.76;
	engineConfiguration->gearRatio[1] = 2.27;
	engineConfiguration->gearRatio[2] = 1.65;
	engineConfiguration->gearRatio[3] = 1.26;
	engineConfiguration->gearRatio[4] = 1.00;
	engineConfiguration->gearRatio[5] = 0.84;
	engineConfiguration->gearRatio[6] = 0.84;
	engineConfiguration->gearRatio[7] = 0.84;

	dut.onConfigurationChange(nullptr);

	EXPECT_EQ(0, dut.determineGearFromRatio(5.85));
	EXPECT_EQ(1, dut.determineGearFromRatio(5.51));

	// Check exactly on gears
	EXPECT_EQ(1, dut.determineGearFromRatio(3.76));
	EXPECT_EQ(2, dut.determineGearFromRatio(2.27));
	EXPECT_EQ(3, dut.determineGearFromRatio(1.65));
	EXPECT_EQ(4, dut.determineGearFromRatio(1.26));
	EXPECT_EQ(5, dut.determineGearFromRatio(1.00));
	EXPECT_EQ(6, dut.determineGearFromRatio(0.84));
}

TEST(GearDetector, DetermineGear8Speed) {
	EngineTestHelper eth(TEST_ENGINE);
	GearDetector dut;

	// ZF 8HP 70
	engineConfiguration->totalGearsCount = 8;
	engineConfiguration->gearRatio[0] = 4.69;
	engineConfiguration->gearRatio[1] = 3.13;
	engineConfiguration->gearRatio[2] = 2.10;
	engineConfiguration->gearRatio[3] = 1.67;
	engineConfiguration->gearRatio[4] = 1.28;
	engineConfiguration->gearRatio[5] = 1;
	engineConfiguration->gearRatio[6] = 0.84;
	engineConfiguration->gearRatio[7] = 0.67;

	dut.onConfigurationChange(nullptr);

	// Super high ratios indicate clutch slip or idling in neutral or something
	EXPECT_EQ(0, dut.determineGearFromRatio(100));
	EXPECT_EQ(0, dut.determineGearFromRatio(8));

	// Check exactly on gears - only test the ends, the middle works
	EXPECT_EQ(1, dut.determineGearFromRatio(4.69));
	EXPECT_EQ(2, dut.determineGearFromRatio(3.13));

	EXPECT_EQ(7, dut.determineGearFromRatio(0.84));
	EXPECT_EQ(8, dut.determineGearFromRatio(0.67));

	// Check near each gear - only test the ends, the middle works
	EXPECT_EQ(1, dut.determineGearFromRatio(4.75));
	EXPECT_EQ(1, dut.determineGearFromRatio(4.3));

	EXPECT_EQ(8, dut.determineGearFromRatio(0.71));
	EXPECT_EQ(8, dut.determineGearFromRatio(0.6));

	// Extremely low ratio suggests stopped engine at speed?
	EXPECT_EQ(0, dut.determineGearFromRatio(0.1));
}

TEST(GearDetector, ParameterValidation) {
	EngineTestHelper eth(TEST_ENGINE);
	GearDetector dut;

	// Defaults should work
	EXPECT_NO_FATAL_ERROR(dut.onConfigurationChange(nullptr));

	// Invalid gear count
	engineConfiguration->totalGearsCount = 25;
	EXPECT_FATAL_ERROR(dut.onConfigurationChange(nullptr));

	// Valid gears
	engineConfiguration->totalGearsCount = 2;
	engineConfiguration->gearRatio[0] = 3;
	engineConfiguration->gearRatio[1] = 2;
	EXPECT_NO_FATAL_ERROR(dut.onConfigurationChange(nullptr));

	// Invalid gear ratio
	engineConfiguration->gearRatio[1] = 0;
	EXPECT_FATAL_ERROR(dut.onConfigurationChange(nullptr));

	// Out of order gear ratios
	engineConfiguration->gearRatio[0] = 2;
	engineConfiguration->gearRatio[1] = 3;
	EXPECT_FATAL_ERROR(dut.onConfigurationChange(nullptr));

	// No gears at all is a valid configuration
	engineConfiguration->totalGearsCount = 0;
	EXPECT_NO_FATAL_ERROR(dut.onConfigurationChange(nullptr));
}
