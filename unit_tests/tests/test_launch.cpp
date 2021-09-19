#include "pch.h"

#include "launch_control.h"

TEST(LaunchControl, TpsCondition) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	LaunchControlBase dut;
	INJECT_ENGINE_REFERENCE(&dut);

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
	INJECT_ENGINE_REFERENCE(&dut);

	// Test Speed trashold
	engineConfiguration->launchActivationMode = ALWAYS_ACTIVE_LAUNCH;
    engineConfiguration->launchSpeedTreshold = 30; 
	engineConfiguration->launchDisableBySpeed = 1;
	Sensor::setMockValue(SensorType::VehicleSpeed, 10.0);
    EXPECT_TRUE(dut.isInsideSpeedCondition());

	Sensor::setMockValue(SensorType::VehicleSpeed, 40.0);
	EXPECT_FALSE(dut.isInsideSpeedCondition());

}

TEST(LaunchControl, RPMCondition) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	LaunchControlBase dut;
	INJECT_ENGINE_REFERENCE(&dut);

	engineConfiguration->launchRpm = 3000;

	EXPECT_FALSE(dut.isInsideRPMCondition(2900));

	EXPECT_TRUE(dut.isInsideRPMCondition(3100));
}

TEST(LaunchControl, SwitchInputCondition) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	LaunchControlBase dut;
	INJECT_ENGINE_REFERENCE(&dut);

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
	engine->updateSwitchInputs(PASS_ENGINE_PARAMETER_SIGNATURE);
	EXPECT_TRUE(dut.isInsideSwitchCondition());

	setMockState(engineConfiguration->clutchDownPin, false);
	engine->updateSwitchInputs(PASS_ENGINE_PARAMETER_SIGNATURE);
	EXPECT_FALSE(dut.isInsideSwitchCondition());

	engineConfiguration->clutchDownPinMode = PI_PULLDOWN;
	engineConfiguration->clutchDownPinInverted = true;
	setMockState(engineConfiguration->clutchDownPin, false);
	engine->updateSwitchInputs(PASS_ENGINE_PARAMETER_SIGNATURE);
	EXPECT_TRUE(dut.isInsideSwitchCondition());

	setMockState(engineConfiguration->clutchDownPin, true);
	engine->updateSwitchInputs(PASS_ENGINE_PARAMETER_SIGNATURE);
	EXPECT_FALSE(dut.isInsideSwitchCondition());

}

TEST(LaunchControl, CombinedCondition) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	LaunchControlBase dut;
	INJECT_ENGINE_REFERENCE(&dut);

	//check VSS normal usage
	engineConfiguration->launchActivationMode=ALWAYS_ACTIVE_LAUNCH;
    engineConfiguration->launchSpeedTreshold = 30; 
	engineConfiguration->launchDisableBySpeed = 1;
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

TEST(LaunchControl, CompleteRun) {
	bool spark, fuel;
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	initLaunchControl(PASS_ENGINE_PARAMETER_SIGNATURE);

	//load default config
	setDefaultLaunchParameters(PASS_CONFIG_PARAMETER_SIGNATURE);

	//check VSS normal usage
	engineConfiguration->launchActivationMode = ALWAYS_ACTIVE_LAUNCH;
    engineConfiguration->launchSpeedTreshold = 30; 
	engineConfiguration->launchDisableBySpeed = 1;
	engineConfiguration->launchRpm = 3000;
	engineConfiguration->launchTpsTreshold = 10;
	engineConfiguration->launchControlEnabled = 1; 
	//valid TPS
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 20.0f);
	
	Sensor::setMockValue(SensorType::VehicleSpeed, 10.0);
	engine->rpmCalculator.mockRpm = 1200;

	//update condition check
    updateLaunchConditions(PASS_ENGINE_PARAMETER_SIGNATURE);

	//check if we have some sort of cut? we should not have at this point
	spark = false;
	fuel = false;
	applyLaunchControlLimiting(&spark, &fuel PASS_ENGINE_PARAMETER_SUFFIX);
	EXPECT_FALSE(spark);
	EXPECT_FALSE(fuel);


	engine->rpmCalculator.mockRpm = 3510;
	//update condition check
    updateLaunchConditions(PASS_ENGINE_PARAMETER_SIGNATURE);


	//we have a 3 seconds delay to actually enable it!
	eth.moveTimeForwardAndInvokeEventsSec(1);
	updateLaunchConditions(PASS_ENGINE_PARAMETER_SIGNATURE);
	spark = false;
	fuel = false;
	applyLaunchControlLimiting(&spark, &fuel PASS_ENGINE_PARAMETER_SUFFIX);
	
	EXPECT_FALSE(spark);
	EXPECT_FALSE(fuel);

	eth.moveTimeForwardAndInvokeEventsSec(3);
	updateLaunchConditions(PASS_ENGINE_PARAMETER_SIGNATURE);
	spark = false;
	fuel = false;
	applyLaunchControlLimiting(&spark, &fuel PASS_ENGINE_PARAMETER_SUFFIX);

	EXPECT_TRUE(spark);
	EXPECT_FALSE(fuel);

	Sensor::setMockValue(SensorType::VehicleSpeed, 40.0);
	updateLaunchConditions(PASS_ENGINE_PARAMETER_SIGNATURE);
	spark = false;
	fuel = false;
	applyLaunchControlLimiting(&spark, &fuel PASS_ENGINE_PARAMETER_SUFFIX);
	EXPECT_FALSE(spark);
	EXPECT_FALSE(fuel);

}
