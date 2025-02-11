#include "pch.h"

#include "launch_control.h"

TEST(LaunchControl, TpsCondition) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	LaunchControlBase dut;

	engineConfiguration->launchTpsThreshold = 10;

	// Should return false with failed sensor
	Sensor::resetMockValue(SensorType::DriverThrottleIntent);
	EXPECT_FALSE(dut.isInsideTpsCondition());

	// Should return false when throttle is closed
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 5.0f);
	EXPECT_FALSE(dut.isInsideTpsCondition());

	// Should return true when throttle is opened past the threshold
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 20.0f);
	EXPECT_TRUE(dut.isInsideTpsCondition());
}


TEST(LaunchControl, VSSCondition) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	LaunchControlBase dut;

	// Test Speed threshold
	engineConfiguration->launchActivationMode = ALWAYS_ACTIVE_LAUNCH;
    engineConfiguration->launchSpeedThreshold = 30;

	Sensor::setMockValue(SensorType::VehicleSpeed, 10.0);
    EXPECT_TRUE(dut.isInsideSpeedCondition());

	Sensor::setMockValue(SensorType::VehicleSpeed, 40.0);
	EXPECT_FALSE(dut.isInsideSpeedCondition());

}

TEST(LaunchControl, ZeroVSSCondition) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	LaunchControlBase dut;

	// Test Speed threshold
	engineConfiguration->launchActivationMode = ALWAYS_ACTIVE_LAUNCH;
    engineConfiguration->launchSpeedThreshold = 0;

	Sensor::setMockValue(SensorType::VehicleSpeed, 10.0);
	EXPECT_TRUE(dut.isInsideSpeedCondition());
}

TEST(LaunchControl, VSSConditionWithSwitch) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	LaunchControlBase dut;

	// Test Speed threshold
	engineConfiguration->launchActivationMode = SWITCH_INPUT_LAUNCH;
	engineConfiguration->launchActivatePin = Gpio::G1;
	setMockState(engineConfiguration->launchActivatePin, true);
    engineConfiguration->launchSpeedThreshold = 30;

	Sensor::setMockValue(SensorType::VehicleSpeed, 10.0);
    EXPECT_TRUE(dut.isInsideSpeedCondition());

	Sensor::setMockValue(SensorType::VehicleSpeed, 40.0);
	EXPECT_FALSE(dut.isInsideSpeedCondition());
}

TEST(LaunchControl, RPMCondition) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	LaunchControlBase dut;

	engineConfiguration->launchRpm = 3000;

	EXPECT_EQ(engineConfiguration->launchRpmWindow, 500);

	EXPECT_EQ(dut.calculateRPMLaunchCondition(2499), LaunchCondition::NotMet);
	EXPECT_EQ(dut.calculateRPMLaunchCondition(2500), LaunchCondition::PreLaunch);
	EXPECT_EQ(dut.calculateRPMLaunchCondition(2900), LaunchCondition::PreLaunch);
	EXPECT_EQ(dut.calculateRPMLaunchCondition(2999), LaunchCondition::PreLaunch);
	EXPECT_EQ(dut.calculateRPMLaunchCondition(3000), LaunchCondition::Launch);
	EXPECT_EQ(dut.calculateRPMLaunchCondition(3100), LaunchCondition::Launch);
}

TEST(LaunchControl, SwitchInputCondition) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	LaunchControlBase dut;

	//activation based on VSS
	engineConfiguration->launchActivationMode = ALWAYS_ACTIVE_LAUNCH;
	EXPECT_TRUE(dut.isInsideSwitchCondition());

	//active by switch
	engineConfiguration->launchActivationMode = SWITCH_INPUT_LAUNCH;
	engineConfiguration->launchActivatePin = Gpio::G1;
	setMockState(engineConfiguration->launchActivatePin, true);
	EXPECT_TRUE(dut.isInsideSwitchCondition());

	setMockState(engineConfiguration->launchActivatePin, false);
	EXPECT_FALSE(dut.isInsideSwitchCondition());

	//by clutch
	engineConfiguration->launchActivationMode = CLUTCH_INPUT_LAUNCH;
	engineConfiguration->clutchDownPin = Gpio::G2;
	engineConfiguration->clutchDownPinMode = PI_PULLUP;
	setMockState(engineConfiguration->clutchDownPin, true);
	engine->updateSwitchInputs();
	EXPECT_TRUE(dut.isInsideSwitchCondition());

	setMockState(engineConfiguration->clutchDownPin, false);
	engine->updateSwitchInputs();
	EXPECT_FALSE(dut.isInsideSwitchCondition());

	engineConfiguration->clutchDownPinMode = PI_INVERTED_PULLDOWN;
	setMockState(engineConfiguration->clutchDownPin, false);
	engine->updateSwitchInputs();
	EXPECT_TRUE(dut.isInsideSwitchCondition());

	setMockState(engineConfiguration->clutchDownPin, true);
	engine->updateSwitchInputs();
	EXPECT_FALSE(dut.isInsideSwitchCondition());

}

TEST(LaunchControl, CombinedCondition) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	LaunchControlBase dut;

	//check VSS normal usage
	engineConfiguration->launchActivationMode = ALWAYS_ACTIVE_LAUNCH;

	engineConfiguration->launchRpm = 3000;
	engineConfiguration->launchTpsThreshold = 10;
	//valid TPS
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 20.0f);

	Sensor::setMockValue(SensorType::VehicleSpeed, 10.0);
	Sensor::setMockValue(SensorType::Rpm,  1200);

	EXPECT_EQ(dut.calculateLaunchCondition(1200), LaunchCondition::NotMet);

	Sensor::setMockValue(SensorType::Rpm,  3200);
	EXPECT_EQ(dut.calculateLaunchCondition(3200), LaunchCondition::Launch);

	Sensor::setMockValue(SensorType::VehicleSpeed, 40.0);
	EXPECT_EQ(dut.calculateLaunchCondition(3200), LaunchCondition::NotMet);
}

static void setDefaultLaunchParameters() {
	engineConfiguration->launchRpm = 4000;    // Rpm to trigger Launch condition
//	engineConfiguration->launchTimingRetard = 10; // retard in absolute degrees ATDC
	engineConfiguration->launchRpmWindow = 500; // RPM window (Launch RPM - Window) for transitioning to full retard
	engineConfiguration->launchSparkCutEnable = true;
	engineConfiguration->launchFuelCutEnable = false;
	engineConfiguration->launchSpeedThreshold = 10; //maximum speed allowed before disable launch
	engineConfiguration->launchFuelAdderPercent = 10; // Extra fuel in % when launch are triggered
//	engineConfiguration->launchBoostDuty = 70; // boost valve duty cycle at launch
//	engineConfiguration->enableLaunchRetard = true;
// dead code todo	engineConfiguration->enableLaunchBoost = true;
	engineConfiguration->launchSmoothRetard = true; //interpolates the advance linear from launchrpm to fully retarded at launchtimingrpmrange
	// dead code todo	engineConfiguration->antiLagRpmTreshold = 3000;
}

TEST(LaunchControl, CompleteRun) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	//load default config
	setDefaultLaunchParameters();

	//check VSS normal usage
	engineConfiguration->launchActivationMode = ALWAYS_ACTIVE_LAUNCH;
    engineConfiguration->launchSpeedThreshold = 30;

	engineConfiguration->launchRpm = 3000;
	engineConfiguration->launchTpsThreshold = 10;
	engineConfiguration->launchControlEnabled = true;
	//valid TPS
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 20.0f);

	Sensor::setMockValue(SensorType::VehicleSpeed, 10.0);
	Sensor::setMockValue(SensorType::Rpm, 1200);

	engine->launchController.update();


	//check if we have some sort of cut? we should not have at this point
	EXPECT_FALSE(engine->launchController.isLaunchSparkRpmRetardCondition());
	EXPECT_FALSE(engine->launchController.isLaunchFuelRpmRetardCondition());


	Sensor::setMockValue(SensorType::Rpm, 3510);
	//update condition check
	engine->launchController.update();


	eth.moveTimeForwardAndInvokeEventsSec(3);
	engine->launchController.update();


	EXPECT_TRUE(engine->launchController.isLaunchSparkRpmRetardCondition());
	EXPECT_FALSE(engine->launchController.isLaunchFuelRpmRetardCondition());

	Sensor::setMockValue(SensorType::VehicleSpeed, 40.0);
	engine->launchController.update();


	EXPECT_FALSE(engine->launchController.isLaunchSparkRpmRetardCondition());
	EXPECT_FALSE(engine->launchController.isLaunchFuelRpmRetardCondition());

}

TEST(LaunchControl, hardSkip) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	SoftSparkLimiter hardSparkLimiter(true);
	ASSERT_FALSE(hardSparkLimiter.shouldSkip());


	hardSparkLimiter.updateTargetSkipRatio(1.0f, 0.0f);
	// open question if we need special handling of '1' or random would just work?
	ASSERT_TRUE(hardSparkLimiter.shouldSkip());

	int counter = 0;
	hardSparkLimiter.updateTargetSkipRatio(0.5f, 0.0f);
	for (int i =0;i<1000;i++) {
		if (hardSparkLimiter.shouldSkip()) {
			counter++;
		}

	}
	ASSERT_TRUE(counter > 400 && counter < 600) << "How good is random " << counter;
}












