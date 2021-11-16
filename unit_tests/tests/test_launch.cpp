#include "pch.h"

#include "launch_control.h"

TEST(LaunchControl, TpsCondition) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	LaunchControlBase dut;
	dut.inject();

	engineConfiguration->launchTpsTreshold = 10;

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
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	LaunchControlBase dut;
	dut.inject();

	// Test Speed trashold
	engineConfiguration->launchActivationMode = ALWAYS_ACTIVE_LAUNCH;
    engineConfiguration->launchSpeedThreshold = 30;

	Sensor::setMockValue(SensorType::VehicleSpeed, 10.0);
    EXPECT_TRUE(dut.isInsideSpeedCondition());

	Sensor::setMockValue(SensorType::VehicleSpeed, 40.0);
	EXPECT_FALSE(dut.isInsideSpeedCondition());

}

TEST(LaunchControl, RPMCondition) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	LaunchControlBase dut;
	dut.inject();

	engineConfiguration->launchRpm = 3000;

	EXPECT_FALSE(dut.isInsideRPMCondition(2900));

	EXPECT_TRUE(dut.isInsideRPMCondition(3100));
}

TEST(LaunchControl, SwitchInputCondition) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	LaunchControlBase dut;
	dut.inject();

	//activation based on VSS
	engineConfiguration->launchActivationMode = ALWAYS_ACTIVE_LAUNCH;
	EXPECT_TRUE(dut.isInsideSwitchCondition());

	//active by switch
	engineConfiguration->launchActivationMode = SWITCH_INPUT_LAUNCH;
	engineConfiguration->launchActivatePin = GPIOG_1;
	setMockState(engineConfiguration->launchActivatePin, true);
	EXPECT_TRUE(dut.isInsideSwitchCondition());

	setMockState(engineConfiguration->launchActivatePin, false);
	EXPECT_FALSE(dut.isInsideSwitchCondition());

	//by clutch
	engineConfiguration->launchActivationMode = CLUTCH_INPUT_LAUNCH;
	engineConfiguration->clutchDownPin = GPIOG_2;
	engineConfiguration->clutchDownPinMode = PI_PULLUP;
	setMockState(engineConfiguration->clutchDownPin, true);
	engine->updateSwitchInputs();
	EXPECT_TRUE(dut.isInsideSwitchCondition());

	setMockState(engineConfiguration->clutchDownPin, false);
	engine->updateSwitchInputs();
	EXPECT_FALSE(dut.isInsideSwitchCondition());

	engineConfiguration->clutchDownPinMode = PI_PULLDOWN;
	engineConfiguration->clutchDownPinInverted = true;
	setMockState(engineConfiguration->clutchDownPin, false);
	engine->updateSwitchInputs();
	EXPECT_TRUE(dut.isInsideSwitchCondition());

	setMockState(engineConfiguration->clutchDownPin, true);
	engine->updateSwitchInputs();
	EXPECT_FALSE(dut.isInsideSwitchCondition());

}

TEST(LaunchControl, CombinedCondition) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	LaunchControlBase dut;
	dut.inject();

	//check VSS normal usage
	engineConfiguration->launchActivationMode = ALWAYS_ACTIVE_LAUNCH;

	engineConfiguration->launchRpm = 3000;
	engineConfiguration->launchTpsTreshold = 10;
	//valid TPS
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 20.0f);
	
	Sensor::setMockValue(SensorType::VehicleSpeed, 10.0);
	engine->rpmCalculator.mockRpm = 1200;

    EXPECT_FALSE(dut.isLaunchConditionMet(1200));

	engine->rpmCalculator.mockRpm = 3200;
	EXPECT_TRUE(dut.isLaunchConditionMet(3200));

	Sensor::setMockValue(SensorType::VehicleSpeed, 40.0);
	EXPECT_FALSE(dut.isLaunchConditionMet(3200));

}

static void setDefaultLaunchParameters() {
	engineConfiguration->launchRpm = 4000;    // Rpm to trigger Launch condition
//	engineConfiguration->launchTimingRetard = 10; // retard in absolute degrees ATDC
	engineConfiguration->launchTimingRpmRange = 500; // Rpm above Launch triggered for full retard
	engineConfiguration->launchSparkCutEnable = true;
	engineConfiguration->launchFuelCutEnable = false;
	engineConfiguration->hardCutRpmRange = 500; //Rpm above Launch triggered +(if retard enabled) launchTimingRpmRange to hard cut
	engineConfiguration->launchSpeedThreshold = 10; //maximum speed allowed before disable launch
	engineConfiguration->launchFuelAdded = 10; // Extra fuel in % when launch are triggered
	engineConfiguration->launchBoostDuty = 70; // boost valve duty cycle at launch
	engineConfiguration->launchActivateDelay = 3; // Delay in seconds for launch to kick in
//	engineConfiguration->enableLaunchRetard = true;
// dead code todo	engineConfiguration->enableLaunchBoost = true;
	engineConfiguration->launchSmoothRetard = true; //interpolates the advance linear from launchrpm to fully retarded at launchtimingrpmrange
	// dead code todo	engineConfiguration->antiLagRpmTreshold = 3000;
}

TEST(LaunchControl, CompleteRun) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	initLaunchControl();

	//load default config
	setDefaultLaunchParameters();

	//check VSS normal usage
	engineConfiguration->launchActivationMode = ALWAYS_ACTIVE_LAUNCH;
    engineConfiguration->launchSpeedThreshold = 30;

	engineConfiguration->launchRpm = 3000;
	engineConfiguration->launchTpsTreshold = 10;
	engineConfiguration->launchControlEnabled = true;
	//valid TPS
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 20.0f);
	
	Sensor::setMockValue(SensorType::VehicleSpeed, 10.0);
	engine->rpmCalculator.mockRpm = 1200;

	engine->launchController.update();


	//check if we have some sort of cut? we should not have at this point
	EXPECT_FALSE(engine->launchController.isLaunchSparkRpmRetardCondition());
	EXPECT_FALSE(engine->launchController.isLaunchFuelRpmRetardCondition());


	engine->rpmCalculator.mockRpm = 3510;
	//update condition check
	engine->launchController.update();


	//we have a 3 seconds delay to actually enable it!
	eth.moveTimeForwardAndInvokeEventsSec(1);
	engine->launchController.update();
	
	EXPECT_FALSE(engine->launchController.isLaunchSparkRpmRetardCondition());
	EXPECT_FALSE(engine->launchController.isLaunchFuelRpmRetardCondition());

	eth.moveTimeForwardAndInvokeEventsSec(3);
	engine->launchController.update();


	EXPECT_TRUE(engine->launchController.isLaunchSparkRpmRetardCondition());
	EXPECT_FALSE(engine->launchController.isLaunchFuelRpmRetardCondition());

	Sensor::setMockValue(SensorType::VehicleSpeed, 40.0);
	engine->launchController.update();


	EXPECT_FALSE(engine->launchController.isLaunchSparkRpmRetardCondition());
	EXPECT_FALSE(engine->launchController.isLaunchFuelRpmRetardCondition());

}
