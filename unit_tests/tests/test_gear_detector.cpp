#include "pch.h"
#include "frequency_sensor.h"
#include "init.h"
#include "output_shaft_speed_converter.h"
#include "tunerstudio.h"

float GetGearRatioFor(float revPerKm, float axle, float kph, float rpm) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->driveWheelRevPerKm = revPerKm;
	engineConfiguration->finalGearRatio = axle;

	Sensor::setMockValue(SensorType::VehicleSpeed, kph);
	Sensor::setMockValue(SensorType::Rpm, rpm);

	engine->periodicSlowCallback();

	auto& dut = engine->module<GearDetector>().unmock();
	return dut.getGearboxRatio();
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
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->driveWheelRevPerKm = 507;
	engineConfiguration->finalGearRatio = 4.10f;

	// real gears from Volvo racecar
	engineConfiguration->totalGearsCount = 5;
	engineConfiguration->gearRatio[0] = 3.35f;
	engineConfiguration->gearRatio[1] = 1.99f;
	engineConfiguration->gearRatio[2] = 1.33f;
	engineConfiguration->gearRatio[3] = 1.00f;
	engineConfiguration->gearRatio[4] = 0.72f;

	auto& dut = engine->module<GearDetector>().unmock();

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
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& dut = engine->module<GearDetector>().unmock();

	engineConfiguration->totalGearsCount = 1;
	engineConfiguration->gearRatio[0] = 2;

	dut.onConfigurationChange(nullptr);

	// Super high ratios indicate clutch slip or idling in neutral or something
	EXPECT_EQ(0u, dut.determineGearFromRatio(100));
	EXPECT_EQ(0u, dut.determineGearFromRatio(4));

	// Check exactly on the gear
	EXPECT_EQ(1u, dut.determineGearFromRatio(2));

	// Check near the gear
	EXPECT_EQ(1u, dut.determineGearFromRatio(2.1));
	EXPECT_EQ(1u, dut.determineGearFromRatio(1.9));

	// Extremely low ratio suggests stopped engine at speed?
	EXPECT_EQ(0u, dut.determineGearFromRatio(1.0));
}

TEST(GearDetector, DetermineGear5Speed) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& dut = engine->module<GearDetector>().unmock();

	engineConfiguration->totalGearsCount = 5;
	engineConfiguration->gearRatio[0] = 3.35;
	engineConfiguration->gearRatio[1] = 1.99;
	engineConfiguration->gearRatio[2] = 1.33;
	engineConfiguration->gearRatio[3] = 1.00;
	engineConfiguration->gearRatio[4] = 0.72;

	dut.onConfigurationChange(nullptr);

	// Super high ratios indicate clutch slip or idling in neutral or something
	EXPECT_EQ(0u, dut.determineGearFromRatio(100));
	EXPECT_EQ(0u, dut.determineGearFromRatio(6));

	// Check exactly on gears
	EXPECT_EQ(1u, dut.determineGearFromRatio(3.35));
	EXPECT_EQ(2u, dut.determineGearFromRatio(1.99));
	EXPECT_EQ(3u, dut.determineGearFromRatio(1.33));
	EXPECT_EQ(4u, dut.determineGearFromRatio(1.00));
	EXPECT_EQ(5u, dut.determineGearFromRatio(0.72));

	// Check near each gear
	EXPECT_EQ(1u, dut.determineGearFromRatio(3.45));
	EXPECT_EQ(1u, dut.determineGearFromRatio(3.25));

	EXPECT_EQ(2u, dut.determineGearFromRatio(2.2));
	EXPECT_EQ(2u, dut.determineGearFromRatio(1.8));

	EXPECT_EQ(3u, dut.determineGearFromRatio(1.45));
	EXPECT_EQ(3u, dut.determineGearFromRatio(1.25));

	EXPECT_EQ(4u, dut.determineGearFromRatio(1.1));
	EXPECT_EQ(4u, dut.determineGearFromRatio(0.9));

	EXPECT_EQ(5u, dut.determineGearFromRatio(0.8));
	EXPECT_EQ(5u, dut.determineGearFromRatio(0.6));

	// Extremely low ratio suggests stopped engine at speed?
	EXPECT_EQ(0u, dut.determineGearFromRatio(0.1));
}

TEST(GearDetector, MiataNb6Speed) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& dut = engine->module<GearDetector>().unmock();

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

	EXPECT_EQ(0u, dut.determineGearFromRatio(5.85));
	EXPECT_EQ(1u, dut.determineGearFromRatio(5.51));

	// Check exactly on gears
	EXPECT_EQ(1u, dut.determineGearFromRatio(3.76));
	EXPECT_EQ(2u, dut.determineGearFromRatio(2.27));
	EXPECT_EQ(3u, dut.determineGearFromRatio(1.65));
	EXPECT_EQ(4u, dut.determineGearFromRatio(1.26));
	EXPECT_EQ(5u, dut.determineGearFromRatio(1.00));
	EXPECT_EQ(6u, dut.determineGearFromRatio(0.84));
}

TEST(GearDetector, DetermineGear8Speed) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& dut = engine->module<GearDetector>().unmock();

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
	EXPECT_EQ(0u, dut.determineGearFromRatio(100));
	EXPECT_EQ(0u, dut.determineGearFromRatio(8));

	// Check exactly on gears - only test the ends, the middle works
	EXPECT_EQ(1u, dut.determineGearFromRatio(4.69));
	EXPECT_EQ(2u, dut.determineGearFromRatio(3.13));

	EXPECT_EQ(7u, dut.determineGearFromRatio(0.84));
	EXPECT_EQ(8u, dut.determineGearFromRatio(0.67));

	// Check near each gear - only test the ends, the middle works
	EXPECT_EQ(1u, dut.determineGearFromRatio(4.75));
	EXPECT_EQ(1u, dut.determineGearFromRatio(4.3));

	EXPECT_EQ(8u, dut.determineGearFromRatio(0.71));
	EXPECT_EQ(8u, dut.determineGearFromRatio(0.6));

	// Extremely low ratio suggests stopped engine at speed?
	EXPECT_EQ(0u, dut.determineGearFromRatio(0.1));
}

TEST(GearDetector, ParameterValidation) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& dut = engine->module<GearDetector>().unmock();

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

TEST(GearDetector, ClutchDisengagementInvalidatesGear) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->totalGearsCount = 1;
	engineConfiguration->gearRatio[0] = 2;
	engineConfiguration->driveWheelRevPerKm = 600;
	engineConfiguration->finalGearRatio = 1;
	Sensor::setMockValue(SensorType::VehicleSpeed, 60);
	Sensor::setMockValue(SensorType::Rpm, 1200);
	auto& dut = engine->module<GearDetector>().unmock();

	engineConfiguration->clutchDownPin = Gpio::A1;
	engineConfiguration->clutchUpPin = Gpio::Unassigned;
	engine->engineState.clutchDownState = false;
	dut.onSlowCallback();
	EXPECT_EQ(1, dut.get().Value);

	engine->engineState.clutchDownState = true;
	dut.onSlowCallback();
	// A disconnected clutch invalidates gear inference for either switch.
	EXPECT_EQ(0, dut.get().Value);
	EXPECT_FLOAT_EQ(0, dut.getGearboxRatio());

	engineConfiguration->clutchDownPin = Gpio::Unassigned;
	engineConfiguration->clutchUpPin = Gpio::A2;
	engine->engineState.clutchUpState = 0;
	dut.onSlowCallback();
	EXPECT_EQ(0, dut.get().Value);
	EXPECT_FLOAT_EQ(0, dut.getGearboxRatio());

	engine->engineState.clutchUpState = 1;
	dut.onSlowCallback();
	EXPECT_EQ(1, dut.get().Value);
}

// Speed Source = Output Shaft Speed (explicit choice): OSS RPM is used directly as driveshaft RPM,
// no VSS/wheel-size math at all.
TEST(GearDetector, DriveshaftRpmUsesOutputShaftSpeedWhenSelected) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->totalGearsCount = 1;
	engineConfiguration->gearRatio[0] = 1.0f;
	// Deliberately implausible VSS-path config, to prove it's NOT used when OSS is selected.
	engineConfiguration->driveWheelRevPerKm = 1;
	engineConfiguration->finalGearRatio = 1;
	Sensor::setMockValue(SensorType::VehicleSpeed, 0);

	engineConfiguration->gearDetectionUseOutputShaftSpeed = true;
	Sensor::setMockValue(SensorType::OutputShaftSpeed, 4200.0f);

	auto& dut = engine->module<GearDetector>().unmock();

	EXPECT_NEAR(4200.0f, dut.getRpmInGear(1), 1e-2);
}

// Speed Source = Main Vehicle Speed (default): converts SensorType::VehicleSpeed back to driveshaft
// RPM via driveWheelRevPerKm/finalGearRatio, regardless of which physical sensor feeds VehicleSpeed.
TEST(GearDetector, DriveshaftRpmUsesMainVehicleSpeedByDefault) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->totalGearsCount = 1;
	engineConfiguration->gearRatio[0] = 3.35f;
	engineConfiguration->finalGearRatio = 4.1f;
	engineConfiguration->driveWheelRevPerKm = 507;

	Sensor::setMockValue(SensorType::VehicleSpeed, 29.45f / 0.6214f);

	auto& dut = engine->module<GearDetector>().unmock();
	// Same numbers as the Volvo racecar case in ComputeGearRatio (507 revs/km, 4.1 final drive).
	EXPECT_NEAR(5500, dut.getRpmInGear(1), 1);
}

// RPM Source (Engine RPM vs Input Shaft Speed) only takes effect when Speed Source = Output Shaft
// Speed - selecting Input Shaft Speed there uses ISS instead of plain engine RPM as the numerator.
TEST(GearDetector, RpmSourceOnlyAppliesWithOutputShaftSpeed) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->totalGearsCount = 1;
	engineConfiguration->gearRatio[0] = 1.0f;

	engineConfiguration->gearDetectionUseOutputShaftSpeed = true;
	engineConfiguration->gearDetectionRpmSourceIsInputShaftSpeed = true;
	Sensor::setMockValue(SensorType::OutputShaftSpeed, 2000.0f);
	Sensor::setMockValue(SensorType::Rpm, 9999.0f);
	Sensor::setMockValue(SensorType::InputShaftSpeed, 3000.0f);

	engine->periodicSlowCallback();
	auto& dut = engine->module<GearDetector>().unmock();

	// 3000 ISS / 2000 OSS - proves ISS (not the deliberately-wrong 9999 RPM) fed the numerator.
	EXPECT_NEAR_M3(1.5f, dut.getGearboxRatio());

	engineConfiguration->gearDetectionRpmSourceIsInputShaftSpeed = false;
	dut.onSlowCallback();
	EXPECT_NEAR(9999.0f / 2000, dut.getGearboxRatio(), 0.001f);

	// The selector does not change legacy VSS's automatic ISS preference.
	engineConfiguration->gearDetectionUseOutputShaftSpeed = false;
	engineConfiguration->driveWheelRevPerKm = 600;
	engineConfiguration->finalGearRatio = 1;
	Sensor::setMockValue(SensorType::VehicleSpeed, 100);
	dut.onSlowCallback();
	EXPECT_FLOAT_EQ(3, dut.getGearboxRatio());
}

static GearDetector& setupSlipDetection() {
	engineConfiguration->totalGearsCount = 1;
	engineConfiguration->gearRatio[0] = 1;
	engineConfiguration->driveWheelRevPerKm = 600;
	engineConfiguration->finalGearRatio = 1;
	engineConfiguration->gearDetectionUseOutputShaftSpeed = true;
	engineConfiguration->transmissionSlipDetectionEnabled = true;
	engineConfiguration->transmissionSlipMinVss = 5;
	Sensor::setMockValue(SensorType::VehicleSpeed, 100);
	Sensor::setMockValue(SensorType::OutputShaftSpeed, 1000);
	Sensor::setMockValue(SensorType::Rpm, 1100);
	return engine->module<GearDetector>().unmock();
}

TEST(GearDetector, SlipSignedPercentAndTelemetry) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& dut = setupSlipDetection();
	dut.onSlowCallback();
	ASSERT_TRUE(dut.isSlipValid());
	EXPECT_NEAR(10, dut.getSlipPercent(), 0.001f);
	updateTunerStudioState();
	EXPECT_TRUE(engine->outputChannels.transmissionSlipValid);
	EXPECT_NEAR(10, engine->outputChannels.transmissionSlipPercent, 0.001f);
	EXPECT_FLOAT_EQ(1000, engine->outputChannels.outputShaftSpeed);

	Sensor::setMockValue(SensorType::Rpm, 900);
	dut.onSlowCallback();
	ASSERT_TRUE(dut.isSlipValid());
	EXPECT_NEAR(-10, dut.getSlipPercent(), 0.001f);

	Sensor::setMockValue(SensorType::Rpm, 1000);
	dut.onSlowCallback();
	ASSERT_TRUE(dut.isSlipValid());
	EXPECT_FLOAT_EQ(0, dut.getSlipPercent());

	engineConfiguration->transmissionSlipDetectionEnabled = false;
	dut.onSlowCallback();
	EXPECT_FALSE(dut.isSlipValid());
	EXPECT_FLOAT_EQ(0, dut.getSlipPercent());
	updateTunerStudioState();
	EXPECT_FALSE(engine->outputChannels.transmissionSlipValid);
	EXPECT_FLOAT_EQ(0, engine->outputChannels.transmissionSlipPercent);
}

TEST(GearDetector, SlipRpmSourceIndependentOfGearRpmSource) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& dut = setupSlipDetection();
	engineConfiguration->gearDetectionRpmSourceIsInputShaftSpeed = true;
	Sensor::setMockValue(SensorType::InputShaftSpeed, 1000);
	Sensor::setMockValue(SensorType::Rpm, 6000);
	dut.onSlowCallback();
	EXPECT_FLOAT_EQ(1, dut.getGearboxRatio());
	ASSERT_TRUE(dut.isSlipValid());
	EXPECT_FLOAT_EQ(500, dut.getSlipPercent());
	updateTunerStudioState();
	// A large engine/shaft difference must not wrap the telemetry representation.
	EXPECT_FLOAT_EQ(500, engine->outputChannels.transmissionSlipPercent);

	engineConfiguration->transmissionSlipRpmSourceIsInputShaftSpeed = true;
	Sensor::setInvalidMockValue(SensorType::Rpm);
	dut.onSlowCallback();
	ASSERT_TRUE(dut.isSlipValid());
	EXPECT_FLOAT_EQ(0, dut.getSlipPercent());

	// Gear RPM is valid, but the separately selected slip RPM is unavailable.
	engineConfiguration->gearDetectionRpmSourceIsInputShaftSpeed = false;
	Sensor::setMockValue(SensorType::Rpm, 1000);
	Sensor::setInvalidMockValue(SensorType::InputShaftSpeed);
	dut.onSlowCallback();
	EXPECT_EQ(1, dut.get().Value);
	EXPECT_FALSE(dut.isSlipValid());
	EXPECT_FLOAT_EQ(0, dut.getSlipPercent());
}

TEST(GearDetector, SlipRequiresOssSpeedFloorAndValidSources) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& dut = setupSlipDetection();
	dut.onSlowCallback();
	ASSERT_TRUE(dut.isSlipValid());

	// An otherwise valid gear from legacy VSS cannot establish OSS provenance.
	engineConfiguration->gearDetectionUseOutputShaftSpeed = false;
	dut.onSlowCallback();
	EXPECT_EQ(1, dut.get().Value);
	EXPECT_FALSE(dut.isSlipValid());
	engineConfiguration->gearDetectionUseOutputShaftSpeed = true;

	Sensor::setMockValue(SensorType::VehicleSpeed, 4.99f);
	dut.onSlowCallback();
	EXPECT_FALSE(dut.isSlipValid());
	Sensor::setMockValue(SensorType::VehicleSpeed, 5);
	dut.onSlowCallback();
	EXPECT_TRUE(dut.isSlipValid());
	Sensor::setInvalidMockValue(SensorType::VehicleSpeed);
	dut.onSlowCallback();
	EXPECT_FALSE(dut.isSlipValid());
	Sensor::setMockValue(SensorType::VehicleSpeed, 100);

	Sensor::setMockValue(SensorType::OutputShaftSpeed, 0);
	dut.onSlowCallback();
	EXPECT_EQ(0, dut.get().Value);
	EXPECT_FALSE(dut.isSlipValid());
	Sensor::setInvalidMockValue(SensorType::OutputShaftSpeed);
	dut.onSlowCallback();
	EXPECT_FALSE(dut.isSlipValid());
	Sensor::setMockValue(SensorType::OutputShaftSpeed, 1000);
	Sensor::setInvalidMockValue(SensorType::Rpm);
	dut.onSlowCallback();
	EXPECT_FALSE(dut.isSlipValid());
	EXPECT_FLOAT_EQ(0, dut.getSlipPercent());

	Sensor::setMockValue(SensorType::Rpm, NAN);
	dut.onSlowCallback();
	EXPECT_FALSE(dut.isSlipValid());
	EXPECT_FLOAT_EQ(0, dut.getSlipPercent());

	// Excessive ratio cannot identify a gear, so it also cannot identify slip.
	Sensor::setMockValue(SensorType::Rpm, 2000);
	dut.onSlowCallback();
	EXPECT_EQ(0, dut.get().Value);
	EXPECT_FALSE(dut.isSlipValid());
}

TEST(GearDetector, ClutchClearsAndRestoresSlip) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& dut = setupSlipDetection();
	engineConfiguration->clutchDownPin = Gpio::A1;
	dut.onSlowCallback();
	ASSERT_TRUE(dut.isSlipValid());
	engine->engineState.clutchDownState = true;
	dut.onSlowCallback();
	EXPECT_EQ(0, dut.get().Value);
	EXPECT_FALSE(dut.isSlipValid());
	EXPECT_FLOAT_EQ(0, dut.getSlipPercent());
	engine->engineState.clutchDownState = false;
	dut.onSlowCallback();
	EXPECT_TRUE(dut.isSlipValid());

	engineConfiguration->clutchUpPin = Gpio::A2;
	engine->engineState.clutchUpState = 0;
	dut.onSlowCallback();
	EXPECT_FALSE(dut.isSlipValid());
	engine->engineState.clutchUpState = 1;
	dut.onSlowCallback();
	EXPECT_TRUE(dut.isSlipValid());

	// Unconfigured switches must not suppress a vehicle without clutch inputs.
	engineConfiguration->clutchDownPin = Gpio::Unassigned;
	engineConfiguration->clutchUpPin = Gpio::Unassigned;
	engine->engineState.clutchDownState = true;
	engine->engineState.clutchUpState = 0;
	dut.onSlowCallback();
	EXPECT_TRUE(dut.isSlipValid());
}

TEST(OutputShaftSpeed, FrequencyConversion) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	OutputShaftSpeedConverter converter;
	engineConfiguration->outputShaftSpeedSensorTeeth = 20;
	EXPECT_FLOAT_EQ(300, converter.convert(100).value_or(-1));
	EXPECT_FLOAT_EQ(0, converter.convert(0).value_or(-1));
	engineConfiguration->outputShaftSpeedSensorTeeth = 40;
	EXPECT_FLOAT_EQ(150, converter.convert(100).value_or(-1));
	EXPECT_FALSE(converter.convert(-1).Valid);
	EXPECT_FALSE(converter.convert(NAN).Valid);
	engineConfiguration->outputShaftSpeedSensorTeeth = 0;
	EXPECT_FALSE(converter.convert(100).Valid);
}

TEST(GearDetector, MissingSelectedShaftSourceDoesNotFallBack) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& dut = setupSlipDetection();
	engineConfiguration->gearDetectionRpmSourceIsInputShaftSpeed = true;
	engineConfiguration->transmissionSlipRpmSourceIsInputShaftSpeed = true;
	Sensor::setMockValue(SensorType::InputShaftSpeed, 1000);
	dut.onSlowCallback();
	ASSERT_EQ(1, dut.get().Value);
	ASSERT_TRUE(dut.isSlipValid());

	// Valid engine RPM/VSS must not conceal loss of an explicitly selected shaft sensor.
	for (auto source : {SensorType::InputShaftSpeed, SensorType::OutputShaftSpeed}) {
		SCOPED_TRACE(static_cast<int>(source));
		Sensor::setInvalidMockValue(source);
		dut.onSlowCallback();
		EXPECT_FLOAT_EQ(0, dut.getGearboxRatio());
		EXPECT_EQ(0, dut.get().Value);
		EXPECT_FALSE(dut.isSlipValid());
		EXPECT_FLOAT_EQ(0, dut.getSlipPercent());
		Sensor::setMockValue(source, 1000);
		dut.onSlowCallback();
		EXPECT_EQ(1, dut.get().Value);
		EXPECT_TRUE(dut.isSlipValid());
	}
}

TEST(GearDetector, NonfiniteShaftRatioReportsNeutral) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& dut = setupSlipDetection();
	engineConfiguration->totalGearsCount = 2;
	engineConfiguration->gearRatio[0] = 2;
	engineConfiguration->gearRatio[1] = 1;
	engineConfiguration->gearDetectionRpmSourceIsInputShaftSpeed = true;
	dut.onConfigurationChange(nullptr);
	Sensor::resetMockValue(SensorType::OutputShaftSpeed);
	StoredValueSensor inputShaft(SensorType::InputShaftSpeed, MS2NT(500));
	StoredValueSensor outputShaft(SensorType::OutputShaftSpeed, MS2NT(500));
	ASSERT_TRUE(inputShaft.Register());
	ASSERT_TRUE(outputShaft.Register());
	inputShaft.setValidValue(1000, getTimeNowNt());
	outputShaft.setValidValue(1000, getTimeNowNt());
	dut.onSlowCallback();
	ASSERT_EQ(2, dut.get().Value);
	ASSERT_TRUE(dut.isSlipValid());
	for (float ratio : {NAN, INFINITY, -INFINITY}) {
		EXPECT_EQ(0u, dut.determineGearFromRatio(ratio));
	}

	// External providers, including Lua sensors, publish via StoredValueSensor.
	for (auto* source : {&inputShaft, &outputShaft}) {
		SCOPED_TRACE(static_cast<int>(source->type()));
		source->setValidValue(NAN, getTimeNowNt());
		dut.onSlowCallback();
		EXPECT_TRUE(std::isnan(dut.getGearboxRatio()));
		// Nonfinite ratios cannot establish a gear, even though comparisons with
		// NaN would otherwise fall through to first gear.
		EXPECT_EQ(0, dut.get().Value);
		EXPECT_EQ(0, Sensor::getOrZero(SensorType::DetectedGear));
		EXPECT_FALSE(dut.isSlipValid());
		EXPECT_FLOAT_EQ(0, dut.getSlipPercent());
		source->setValidValue(1000, getTimeNowNt());
		dut.onSlowCallback();
		EXPECT_EQ(2, dut.get().Value);
		EXPECT_TRUE(dut.isSlipValid());
	}
	inputShaft.unregister();
	outputShaft.unregister();
}

TEST(GearDetector, OutputShaftTimeoutClearsGearAndSlipThenRecovers) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& dut = setupSlipDetection();
	Sensor::resetMockValue(SensorType::OutputShaftSpeed);
	StoredValueSensor shaft(SensorType::OutputShaftSpeed, MS2NT(500));
	ASSERT_TRUE(shaft.Register());
	shaft.setValidValue(1000, getTimeNowNt());
	dut.onSlowCallback();
	ASSERT_EQ(1, dut.get().Value);
	ASSERT_TRUE(dut.isSlipValid());

	eth.moveTimeForwardMs(501);
	dut.onSlowCallback();
	EXPECT_EQ(0, dut.get().Value);
	EXPECT_FALSE(dut.isSlipValid());
	updateTunerStudioState();
	EXPECT_FALSE(engine->outputChannels.transmissionSlipValid);
	EXPECT_FLOAT_EQ(0, engine->outputChannels.transmissionSlipPercent);
	EXPECT_FLOAT_EQ(0, engine->outputChannels.outputShaftSpeed);

	shaft.setValidValue(1000, getTimeNowNt());
	dut.onSlowCallback();
	EXPECT_EQ(1, dut.get().Value);
	EXPECT_TRUE(dut.isSlipValid());
	EXPECT_NEAR(10, dut.getSlipPercent(), 0.001f);
	shaft.unregister();
}

TEST(OutputShaftSpeed, PulseInputTimeoutAndReconfiguration) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	extern FrequencySensor outputShaftSpeedSensor;
	// Start this fixture with fresh acquisition history after the clock rewind.
	// Do not reset again below: the in-test reconfiguration must exercise production init.
	outputShaftSpeedSensor.resetHistoryForUnitTest();
	engineConfiguration->outputShaftSpeedSensorPin = Gpio::A3;
	engineConfiguration->outputShaftSpeedSensorTeeth = 20;
	engineConfiguration->ossFilterReciprocal = 5;
	initOutputShaftSpeedSensor();
	ASSERT_TRUE(Sensor::hasSensor(SensorType::OutputShaftSpeed));
	EXPECT_FALSE(Sensor::get(SensorType::OutputShaftSpeed).Valid);
	for (int i = 0; i < 100; i++) {
		eth.moveTimeForwardUs(10000);
		outputShaftSpeedSensor.onEdge(getTimeNowNt());
	}
	EXPECT_NEAR(300, Sensor::get(SensorType::OutputShaftSpeed).value_or(-1), 0.1f);
	eth.moveTimeForwardUs(501000);
	EXPECT_FALSE(Sensor::get(SensorType::OutputShaftSpeed).Valid);

	deinitOutputShaftSpeedSensor();
	EXPECT_FALSE(Sensor::hasSensor(SensorType::OutputShaftSpeed));
	initOutputShaftSpeedSensor();
	EXPECT_FALSE(Sensor::get(SensorType::OutputShaftSpeed).Valid);
	deinitOutputShaftSpeedSensor();
	engineConfiguration->outputShaftSpeedSensorPin = Gpio::Unassigned;
	initOutputShaftSpeedSensor();
	EXPECT_FALSE(Sensor::hasSensor(SensorType::OutputShaftSpeed));

	// Deinitializing the unconfigured hardware provider leaves a Lua/CAN owner intact.
	StoredValueSensor externalProvider(SensorType::OutputShaftSpeed, MS2NT(500));
	ASSERT_TRUE(externalProvider.Register());
	externalProvider.setValidValue(2000, getTimeNowNt());
	deinitOutputShaftSpeedSensor();
	EXPECT_FLOAT_EQ(2000, Sensor::get(SensorType::OutputShaftSpeed).value_or(-1));
	externalProvider.unregister();
}
