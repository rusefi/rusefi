/*
 * @file	test_tcu.cpp
 *
 * @date Oct 24, 2025
 * @author David Holdeman, (c) 2025
*/

#include "gear_controller.h"
#include "pch.h"

void blipGearControllerPin(EngineTestHelper* eth, brain_pin_e pin, int time) {
	engine->gearController->update();
	// Close switch/button
	setMockState(pin, false);
	engine->gearController->update();

	// update gearController every ms
	for (int i = 0; i < time; i = i+1000) {
		eth->moveTimeForwardAndInvokeEventsUs(minI(time - i, 1000));
		engine->gearController->update();
	}
	// And release
	setMockState(pin, true);
	engine->gearController->update();
}

TEST(tcu, testButtonshift) {
	EngineTestHelper eth(engine_type_e::TCU_4R70W);
	engineConfiguration->gearControllerMode = GearControllerMode::ButtonShift;
	initGearController();

	// pinMode is PI_PULLUP, so true = off
	setMockState(engineConfiguration->tcuUpshiftButtonPin, true);
	setMockState(engineConfiguration->tcuDownshiftButtonPin, true);

	ASSERT_NE(nullptr, engine->gearController);
	ASSERT_EQ(NEUTRAL, engine->gearController->getDesiredGear());

	// Press upshift button for 200ms
	blipGearControllerPin(&eth, engineConfiguration->tcuUpshiftButtonPin, 200000);

	// and now a bounce
	eth.moveTimeForwardAndInvokeEventsUs(20);
	blipGearControllerPin(&eth, engineConfiguration->tcuUpshiftButtonPin, 20);

	ASSERT_EQ(GEAR_1, engine->gearController->getDesiredGear());

	// Wait 500ms
	eth.moveTimeForwardAndInvokeEventsUs(500000);
	// Press upshift button for 200ms
	blipGearControllerPin(&eth, engineConfiguration->tcuUpshiftButtonPin, 200000);

	ASSERT_EQ(GEAR_2, engine->gearController->getDesiredGear());

	// Wait 500ms
	eth.moveTimeForwardAndInvokeEventsUs(500000);
	// Press upshift button for 1ms
	blipGearControllerPin(&eth, engineConfiguration->tcuUpshiftButtonPin, 1000);

	ASSERT_EQ(GEAR_3, engine->gearController->getDesiredGear());

	// Wait 500ms
	eth.moveTimeForwardAndInvokeEventsUs(500000);
	// Press upshift button for 3s
	blipGearControllerPin(&eth, engineConfiguration->tcuUpshiftButtonPin, 3000000);

	ASSERT_EQ(GEAR_4, engine->gearController->getDesiredGear());

	// Wait 10ms
	// Because this is a different pin, the 500ms debounce timeout is not in play.
	eth.moveTimeForwardAndInvokeEventsUs(10000);
	// Press downshift button for 200ms
	blipGearControllerPin(&eth, engineConfiguration->tcuDownshiftButtonPin, 200000);

	ASSERT_EQ(GEAR_3, engine->gearController->getDesiredGear());

	// Wait 10ms
	// This shouldn't be long enough for the debounce to have reset,
	// so the downshift won't trigger until ~490ms after pressing the button.
	eth.moveTimeForwardAndInvokeEventsUs(10000);
	// Press downshift button for 1.2s
	blipGearControllerPin(&eth, engineConfiguration->tcuDownshiftButtonPin, 1200000);

	ASSERT_EQ(GEAR_1, engine->gearController->getDesiredGear());
}

TEST(tcu, testGenericGC) {
	EngineTestHelper eth(engine_type_e::TCU_4R70W);
	engineConfiguration->gearControllerMode = GearControllerMode::Generic;
	initGearController();

	// Need to set some engine settings for airmass calc
	engineConfiguration->cylindersCount = 8.0;

	// pinMode is PI_PULLUP, so true = off
	setMockState(engineConfiguration->tcuUpshiftButtonPin, true);
	setMockState(engineConfiguration->tcuDownshiftButtonPin, true);
	setMockState(engineConfiguration->tcu_rangeInput[1], true);
	setMockState(engineConfiguration->tcu_rangeInput[2], true);

	ASSERT_NE(nullptr, engine->gearController);
	ASSERT_EQ(NEUTRAL, engine->gearController->getDesiredGear());

	Sensor::setMockValue(SensorType::VehicleSpeed, 55);
	Sensor::setMockValue(SensorType::Rpm, 2500);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 15);
	Sensor::setMockValue(SensorType::Maf, 0.1f);

	engine->gearController->update();
	// Make sure we stay in neutral with undefined range selector pins
	ASSERT_EQ(NEUTRAL, engine->gearController->getDesiredGear());

	Sensor::setMockValue(SensorType::RangeInput1, 2000);
	engine->gearController->update();
	ASSERT_EQ(GEAR_2, engine->gearController->getDesiredGear());
}

// ---------------------------------------------------------------------------
// #6380: coverage for the parts of the TCU which had none - the automatic
// shift decisions and the shift timing helpers of the transmission controller.
// ---------------------------------------------------------------------------

static void setTcuCurve(uint8_t (&curve)[TCU_TABLE_WIDTH], uint8_t speed) {
	for (size_t i = 0; i < efi::size(curve); i++) {
		curve[i] = speed;
	}
}

// Flat curves: the shift point does not depend on throttle, which keeps these tests about
// the gear state machine rather than about interpolation.
static void setupAutomaticShiftCurves() {
	for (size_t i = 0; i < efi::size(config->tcu_shiftTpsBins); i++) {
		config->tcu_shiftTpsBins[i] = i * 10;
	}
	setTcuCurve(config->tcu_shiftSpeed12, 20);
	setTcuCurve(config->tcu_shiftSpeed23, 40);
	setTcuCurve(config->tcu_shiftSpeed34, 60);
	setTcuCurve(config->tcu_shiftSpeed43, 50);
	setTcuCurve(config->tcu_shiftSpeed32, 30);
	setTcuCurve(config->tcu_shiftSpeed21, 10);
}

TEST(tcu, automaticGearControllerLeavesNeutralOnFirstUpdate) {
	EngineTestHelper eth(engine_type_e::TCU_4R70W);
	setupAutomaticShiftCurves();

	AutomaticGearController gc;
	ASSERT_EQ(NEUTRAL, gc.getDesiredGear());

	// no valid speed or throttle yet, but neutral is still left behind
	gc.update();
	ASSERT_EQ(GEAR_1, gc.getDesiredGear());
}

TEST(tcu, automaticGearControllerUpshiftsThroughTheGears) {
	EngineTestHelper eth(engine_type_e::TCU_4R70W);
	setupAutomaticShiftCurves();

	AutomaticGearController gc;
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 25);

	Sensor::setMockValue(SensorType::VehicleSpeed, 5);
	gc.update();
	ASSERT_EQ(GEAR_1, gc.getDesiredGear());

	// still below the 1-2 shift speed
	gc.update();
	ASSERT_EQ(GEAR_1, gc.getDesiredGear());

	Sensor::setMockValue(SensorType::VehicleSpeed, 25);
	gc.update();
	ASSERT_EQ(GEAR_2, gc.getDesiredGear());

	// between the 2-1 and the 2-3 shift speeds, so it holds
	gc.update();
	ASSERT_EQ(GEAR_2, gc.getDesiredGear());

	Sensor::setMockValue(SensorType::VehicleSpeed, 45);
	gc.update();
	ASSERT_EQ(GEAR_3, gc.getDesiredGear());

	Sensor::setMockValue(SensorType::VehicleSpeed, 65);
	gc.update();
	ASSERT_EQ(GEAR_4, gc.getDesiredGear());

	// top gear, nothing above it
	gc.update();
	ASSERT_EQ(GEAR_4, gc.getDesiredGear());
}

TEST(tcu, automaticGearControllerDownshiftsThroughTheGears) {
	EngineTestHelper eth(engine_type_e::TCU_4R70W);
	setupAutomaticShiftCurves();

	AutomaticGearController gc;
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 25);

	// climb to top gear first
	Sensor::setMockValue(SensorType::VehicleSpeed, 65);
	for (int i = 0; i < 4; i++) {
		gc.update();
	}
	ASSERT_EQ(GEAR_4, gc.getDesiredGear());

	Sensor::setMockValue(SensorType::VehicleSpeed, 45);
	gc.update();
	ASSERT_EQ(GEAR_3, gc.getDesiredGear());

	Sensor::setMockValue(SensorType::VehicleSpeed, 25);
	gc.update();
	ASSERT_EQ(GEAR_2, gc.getDesiredGear());

	Sensor::setMockValue(SensorType::VehicleSpeed, 5);
	gc.update();
	ASSERT_EQ(GEAR_1, gc.getDesiredGear());

	// bottom gear, nothing below it
	gc.update();
	ASSERT_EQ(GEAR_1, gc.getDesiredGear());
}

TEST(tcu, automaticGearControllerHoldsGearWithoutValidSensors) {
	EngineTestHelper eth(engine_type_e::TCU_4R70W);
	setupAutomaticShiftCurves();

	AutomaticGearController gc;
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 25);
	Sensor::setMockValue(SensorType::VehicleSpeed, 25);
	gc.update();
	ASSERT_EQ(GEAR_2, gc.getDesiredGear());

	// speed way above the 2-3 shift point, but the sensor is no longer trustworthy
	Sensor::resetMockValue(SensorType::VehicleSpeed);
	gc.update();
	ASSERT_EQ(GEAR_2, gc.getDesiredGear());

	Sensor::setMockValue(SensorType::VehicleSpeed, 100);
	Sensor::resetMockValue(SensorType::DriverThrottleIntent);
	gc.update();
	ASSERT_EQ(GEAR_2, gc.getDesiredGear());
}

// measureShiftTime()/isShiftCompleted() are protected; expose them rather than reaching
// into the class from the test.
class TestTransmissionController : public TransmissionControllerBase {
public:
	using TransmissionControllerBase::isShiftCompleted;
	using TransmissionControllerBase::measureShiftTime;
};

TEST(tcu, shiftIsNotCompletedBeforeItStarts) {
	EngineTestHelper eth(engine_type_e::TCU_4R70W);

	TestTransmissionController tc;
	ASSERT_FLOAT_EQ(0, tc.isShiftCompleted());
}

TEST(tcu, shiftCompletesWhenTheTargetGearIsDetected) {
	EngineTestHelper eth(engine_type_e::TCU_4R70W);

	TestTransmissionController tc;
	Sensor::setMockValue(SensorType::InputShaftSpeed, 1500);
	Sensor::setMockValue(SensorType::DetectedGear, GEAR_1);

	tc.measureShiftTime(GEAR_2);
	ASSERT_FLOAT_EQ(0, tc.isShiftCompleted());

	eth.moveTimeForwardAndInvokeEventsUs(300000);
	// still in the old gear
	ASSERT_FLOAT_EQ(0, tc.isShiftCompleted());

	Sensor::setMockValue(SensorType::DetectedGear, GEAR_2);
	ASSERT_NEAR(0.3, tc.isShiftCompleted(), 0.01);

	// the shift is only reported once
	ASSERT_FLOAT_EQ(0, tc.isShiftCompleted());
}

TEST(tcu, shiftFallsBackToConfiguredTimeWithoutInputShaftSpeed) {
	EngineTestHelper eth(engine_type_e::TCU_4R70W);

	TestTransmissionController tc;
	Sensor::resetMockValue(SensorType::InputShaftSpeed);
	Sensor::resetMockValue(SensorType::DetectedGear);
	config->tcu_shiftTime = 500;

	tc.measureShiftTime(GEAR_2);
	eth.moveTimeForwardAndInvokeEventsUs(400000);
	ASSERT_FLOAT_EQ(0, tc.isShiftCompleted());

	eth.moveTimeForwardAndInvokeEventsUs(200000);
	ASSERT_FLOAT_EQ(0.5, tc.isShiftCompleted());

	ASSERT_FLOAT_EQ(0, tc.isShiftCompleted());
}
