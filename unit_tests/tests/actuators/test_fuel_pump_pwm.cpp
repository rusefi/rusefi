#include "pch.h"
#include "bench_test.h"

class FuelPumpModes : public testing::Test {
protected:
	EngineTestHelper eth{engine_type_e::TEST_ENGINE};
	FuelPumpController& pump = *engine->module<FuelPumpController>();

	void SetUp() override {
		setOutputOnTheBenchTestForUnitTest(nullptr);
		engineConfiguration->fuelPumpPin = Gpio::A0;
		engineConfiguration->fuelPump2Pin = Gpio::B0;
		engineConfiguration->startUpFuelPumpDuration = 0;
		engineConfiguration->fuelPumpMinDuty = 20;
		engineConfiguration->fuelPumpMaxDuty = 90;
		engineConfiguration->fuelPumpControl.pFactor = 1;
		engineConfiguration->fuelPumpControl.iFactor = 0;
		engineConfiguration->fuelPumpControl.dFactor = 0;
		setTable(config->fuelPressureTargetTable, 300.0f);
		setTable(config->fuelPumpBaseDutyTable, 50.0f);
		setTimeNowUs(500000);
		Sensor::setMockValue(SensorType::Rpm, 4000);
		Sensor::setMockValue(SensorType::Tps1, 70);
		getEngineState()->fuelingLoad = 120;
		Sensor::setMockValue(SensorType::FuelPressureLow, 300);
	}

	void TearDown() override {
		hasCriticalFirmwareErrorFlag = false;
		setOutputOnTheBenchTestForUnitTest(nullptr);
		pump.stopHardware();
	}

	void configure(fuel_pump_mode_e mode) {
		engineConfiguration->fuelPumpMode = mode;
		// Use the hardware burn ordering: cancel PWM before GPIO reassignment.
		stopFuelPump();
		enginePins.unregisterPins();
		enginePins.fuelPumpRelay.init();
		enginePins.fuelPumpRelay2.init();
		pump.onConfigurationChange(&activeConfiguration);
		activeConfiguration = *engineConfiguration;
	}

	void running() {
		engine->triggerCentral.handleShaftSignal(SHAFT_PRIMARY_FALLING, getTimeNowNt());
		pump.onSlowCallback();
		ASSERT_TRUE(pump.isFuelPumpOn);
		ASSERT_FALSE(pump.isPrime);
	}

	void pressure(float kpa) {
		Sensor::setMockValue(SensorType::FuelPressureLow, kpa);
		pump.onFastCallback();
	}

	void expectDuty(float percent) {
		EXPECT_NEAR(percent, pump.fuelPumpDuty, 0.11f);
	}
};

TEST_F(FuelPumpModes, DefaultIsSingleRelayAndSecondOutputStaysOff) {
	EXPECT_EQ(fuel_pump_mode_e::Single, engineConfiguration->fuelPumpMode);
	configure(fuel_pump_mode_e::Single);
	running();
	pump.onFastCallback();
	EXPECT_TRUE(efiReadPin(Gpio::A0));
	EXPECT_FALSE(efiReadPin(Gpio::B0));
	advanceTimeUs(10e6);
	pump.onSlowCallback();
	EXPECT_FALSE(efiReadPin(Gpio::A0));
}

TEST_F(FuelPumpModes, DualRequiresEveryThresholdAndReleasesOnAnyHysteresis) {
	configure(fuel_pump_mode_e::Dual);
	running();
	EXPECT_TRUE(efiReadPin(Gpio::A0));
	EXPECT_TRUE(efiReadPin(Gpio::B0));

	// Exercise every input separately, including equality at both boundaries.
	for (int input = 0; input < 3; input++) {
		auto setInput = [input](float value) {
			if (input == 0) Sensor::setMockValue(SensorType::Rpm, value);
			if (input == 1) getEngineState()->fuelingLoad = value;
			if (input == 2) Sensor::setMockValue(SensorType::Tps1, value);
		};
		const float activation[] = {3000, 100, 50};
		const float release[] = {2500, 90, 40};
		setInput(release[input]);
		pump.onSlowCallback();
		EXPECT_TRUE(efiReadPin(Gpio::B0));
		setInput(release[input] - 1);
		pump.onSlowCallback();
		EXPECT_FALSE(efiReadPin(Gpio::B0));
		setInput(activation[input]);
		pump.onSlowCallback();
		EXPECT_FALSE(efiReadPin(Gpio::B0));
		setInput(activation[input] + 1);
		pump.onSlowCallback();
		EXPECT_TRUE(efiReadPin(Gpio::B0));
	}
	advanceTimeUs(10e6);
	pump.onSlowCallback();
	EXPECT_FALSE(efiReadPin(Gpio::A0));
	EXPECT_FALSE(efiReadPin(Gpio::B0));
}

TEST_F(FuelPumpModes, DualInvalidSensorsReleaseSecondary) {
	configure(fuel_pump_mode_e::Dual);
	running();
	EXPECT_TRUE(pump.isSecondaryPumpOn);
	Sensor::setInvalidMockValue(SensorType::Tps1);
	pump.onSlowCallback();
	EXPECT_FALSE(pump.isSecondaryPumpOn);
	EXPECT_TRUE(efiReadPin(Gpio::A0));
	Sensor::setMockValue(SensorType::Tps1, 70);
	pump.onSlowCallback();
	EXPECT_TRUE(pump.isSecondaryPumpOn);
	getEngineState()->fuelingLoad = NAN;
	pump.onSlowCallback();
	EXPECT_FALSE(efiReadPin(Gpio::B0));
}

TEST_F(FuelPumpModes, PrimeUsesMaximumAndStoppedPumpUsesZeroDespiteMinimumDuty) {
	engineConfiguration->startUpFuelPumpDuration = 2;
	configure(fuel_pump_mode_e::Pwm);
	pump.onIgnitionStateChanged(true);
	pump.onSlowCallback();
	pressure(450);
	EXPECT_TRUE(pump.isPrime);
	expectDuty(90);
	EXPECT_FALSE(pump.isFpPidActive);
	eth.moveTimeForwardAndInvokeEventsUs(3000000);
	pump.onSlowCallback();
	pump.onFastCallback();
	EXPECT_FALSE(pump.isFuelPumpOn);
	expectDuty(0);
	EXPECT_FALSE(efiReadPin(Gpio::A0));
	EXPECT_FALSE(pump.isFpReliefActive);
}

TEST_F(FuelPumpModes, FeedbackAddsSignedCorrectionAndClampsTotalDuty) {
	configure(fuel_pump_mode_e::Pwm);
	running();
	pressure(280);
	expectDuty(70); // 50% base + 20% correction
	EXPECT_TRUE(pump.isFpPidActive);
	pressure(320);
	expectDuty(30);
	pressure(200);
	expectDuty(90);
	pressure(400);
	expectDuty(20);
	EXPECT_NEAR(300, pump.fuelPressureTarget, 0.1f);
}

TEST_F(FuelPumpModes, BothMapsInterpolateTheirOwnAxes) {
	setLinearCurve(config->fuelPressureTargetRpmBins, 0, 6000);
	setLinearCurve(config->fuelPressureTargetLoadBins, 0, 300);
	setLinearCurve(config->fuelPumpBaseDutyRpmBins, 0, 6000);
	setLinearCurve(config->fuelPumpBaseDutyFpBins, 0, 600);
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			config->fuelPressureTargetTable[y][x] = 200 + y * 100 + x * 100;
			config->fuelPumpBaseDutyTable[y][x] = 10 + y * 20 + x * 10;
		}
	}
	configure(fuel_pump_mode_e::Pwm);
	running();
	Sensor::setMockValue(SensorType::Rpm, 1000);
	getEngineState()->fuelingLoad = 50;
	Sensor::setInvalidMockValue(SensorType::FuelPressureLow);
	pump.onFastCallback();
	EXPECT_NEAR(300, pump.fuelPressureTarget, 0.1f);
	expectDuty(45); // target=300 halfway between pressure bins 200/400, RPM=1000
}

TEST_F(FuelPumpModes, MissingOrNonfinitePressureUsesBaseDutyAndResetsIntegrator) {
	engineConfiguration->fuelPumpControl.iFactor = 10;
	configure(fuel_pump_mode_e::Pwm);
	running();
	for (int i = 0; i < 100; i++) pressure(280);
	EXPECT_GT(float(pump.fuelPumpDuty), 70);
	Sensor::setInvalidMockValue(SensorType::FuelPressureLow);
	pump.onFastCallback();
	expectDuty(50);
	EXPECT_FALSE(pump.isFpPidActive);
	pressure(NAN);
	expectDuty(50);
	EXPECT_FALSE(pump.isFpPidActive);
	pressure(INFINITY);
	expectDuty(50);
	pressure(300);
	expectDuty(50);
	EXPECT_TRUE(pump.isFpPidActive);
}

TEST_F(FuelPumpModes, ReliefLatchesThroughPressureBleedAndResumesWithoutWindup) {
	engineConfiguration->fuelPumpAggressiveRelief = true;
	engineConfiguration->fuelPumpControl.iFactor = 10;
	engine->fuelComputer.running.fuel = 2;
	configure(fuel_pump_mode_e::Pwm);
	running();
	pressure(350); // equality at engage threshold does not latch
	EXPECT_FALSE(pump.isFpReliefActive);
	pressure(360);
	EXPECT_TRUE(pump.isFpReliefActive);
	for (float kpa = 358; kpa > 320; kpa -= 2) {
		pressure(kpa);
		expectDuty(20);
		EXPECT_TRUE(pump.isFpReliefActive);
		EXPECT_FALSE(pump.isFpPidActive);
	}
	pressure(320); // equality at recovery threshold releases
	EXPECT_FALSE(pump.isFpReliefActive);
	EXPECT_TRUE(pump.isFpPidActive);
	pressure(300);
	expectDuty(49); // one 5 ms integration step at -20 kPa, not the whole bleed
}

TEST_F(FuelPumpModes, ReliefClearsOnDemandIncreaseDisableSensorLossAndStop) {
	engineConfiguration->fuelPumpAggressiveRelief = true;
	engine->fuelComputer.running.fuel = 2;
	configure(fuel_pump_mode_e::Pwm);
	running();
	pressure(360);
	EXPECT_TRUE(pump.isFpReliefActive);
	engine->fuelComputer.running.fuel = 10;
	pressure(360);
	EXPECT_FALSE(pump.isFpReliefActive);
	engine->fuelComputer.running.fuel = 2;
	pressure(360);
	EXPECT_TRUE(pump.isFpReliefActive);
	engineConfiguration->fuelPumpAggressiveRelief = false;
	pressure(360);
	EXPECT_FALSE(pump.isFpReliefActive);
	engineConfiguration->fuelPumpAggressiveRelief = true;
	pressure(360);
	Sensor::setInvalidMockValue(SensorType::FuelPressureLow);
	pump.onFastCallback();
	EXPECT_FALSE(pump.isFpReliefActive);
	expectDuty(50);
	pressure(360);
	advanceTimeUs(10e6);
	pump.onSlowCallback();
	pump.onFastCallback();
	EXPECT_FALSE(pump.isFpReliefActive);
	expectDuty(0);
}

TEST_F(FuelPumpModes, InvalidDutyLimitsStopOutputEvenDuringPrime) {
	configure(fuel_pump_mode_e::Pwm);
	running();
	engineConfiguration->fuelPumpMinDuty = 95;
	pressure(280);
	expectDuty(0);
	engineConfiguration->fuelPumpMinDuty = 20;
	engineConfiguration->fuelPumpMaxDuty = 101;
	engineConfiguration->startUpFuelPumpDuration = 2;
	pump.onIgnitionStateChanged(true);
	pump.onSlowCallback();
	pump.onFastCallback();
	expectDuty(0);
	EXPECT_FALSE(efiReadPin(Gpio::A0));
}

TEST_F(FuelPumpModes, PwmDrivesPinAndBenchOwnsItBetweenControllerTicks) {
	configure(fuel_pump_mode_e::Pwm);
	running();
	pressure(300); // 50% at 100 Hz
	eth.moveTimeForwardAndInvokeEventsUs(11000);
	EXPECT_TRUE(efiReadPin(Gpio::A0));
	eth.moveTimeForwardAndInvokeEventsUs(5000);
	EXPECT_FALSE(efiReadPin(Gpio::A0));
	setOutputOnTheBenchTestForUnitTest(&enginePins.fuelPumpRelay);
	enginePins.fuelPumpRelay.setValue(true, /*isForce*/ true);
	pump.onSlowCallback();
	pump.onFastCallback();
	eth.moveTimeForwardAndInvokeEventsUs(30000);
	EXPECT_TRUE(efiReadPin(Gpio::A0));
	enginePins.fuelPumpRelay.setValue(false, /*isForce*/ true);
	eth.moveTimeForwardAndInvokeEventsUs(30000);
	EXPECT_FALSE(efiReadPin(Gpio::A0));
	setOutputOnTheBenchTestForUnitTest(nullptr);
	pump.onFastCallback();
	eth.moveTimeForwardAndInvokeEventsUs(5000);
	EXPECT_TRUE(efiReadPin(Gpio::A0));
}

TEST_F(FuelPumpModes, BurnChangesModePinAndFrequencyWithoutOldPwmEvents) {
	configure(fuel_pump_mode_e::Pwm);
	running();
	pressure(300);
	eth.moveTimeForwardAndInvokeEventsUs(11000);
	EXPECT_TRUE(efiReadPin(Gpio::A0));
	configure(fuel_pump_mode_e::Dual);
	getEngineState()->fuelingLoad = 120;
	EXPECT_FALSE(efiReadPin(Gpio::A0));
	pump.onSlowCallback();
	EXPECT_TRUE(efiReadPin(Gpio::A0));
	EXPECT_TRUE(efiReadPin(Gpio::B0));
	eth.moveTimeForwardAndInvokeEventsUs(20000);
	EXPECT_TRUE(efiReadPin(Gpio::A0)); // no old PWM falling edge

	engineConfiguration->fuelPumpPin = Gpio::A1;
	engineConfiguration->fuelPumpPwmFrequency = 200;
	configure(fuel_pump_mode_e::Pwm);
	EXPECT_FALSE(enginePins.fuelPumpRelay2.getLogicValue());
	pump.onFastCallback();
	eth.moveTimeForwardAndInvokeEventsUs(6000);
	EXPECT_TRUE(efiReadPin(Gpio::A1));
	eth.moveTimeForwardAndInvokeEventsUs(2500);
	EXPECT_FALSE(efiReadPin(Gpio::A1));

	// Frequency-only and pin-only burns must also restart the PWM, even though
	// the control mode itself is unchanged.
	engineConfiguration->fuelPumpPwmFrequency = 400;
	configure(fuel_pump_mode_e::Pwm);
	pump.onFastCallback();
	eth.moveTimeForwardAndInvokeEventsUs(3000);
	EXPECT_TRUE(efiReadPin(Gpio::A1));
	eth.moveTimeForwardAndInvokeEventsUs(1250);
	EXPECT_FALSE(efiReadPin(Gpio::A1));
	engineConfiguration->fuelPumpPin = Gpio::A2;
	configure(fuel_pump_mode_e::Pwm);
	pump.onFastCallback();
	eth.moveTimeForwardAndInvokeEventsUs(3000);
	EXPECT_TRUE(efiReadPin(Gpio::A2));
	pump.stopHardware();
	eth.moveTimeForwardAndInvokeEventsUs(20000);
	EXPECT_FALSE(efiReadPin(Gpio::A2));
}

TEST_F(FuelPumpModes, CriticalErrorStopsPwmBetweenControllerTicks) {
	configure(fuel_pump_mode_e::Pwm);
	running();
	pressure(300);
	eth.moveTimeForwardAndInvokeEventsUs(11000);
	EXPECT_TRUE(efiReadPin(Gpio::A0));
	hasCriticalFirmwareErrorFlag = true;
	// Normal periodic control can be suspended on fatal errors. Timer events
	// must not turn the output back on after the global shutdown.
	eth.moveTimeForwardAndInvokeEventsUs(20000);
	EXPECT_FALSE(efiReadPin(Gpio::A0));
	pump.onFastCallback();
	expectDuty(0);
	EXPECT_FALSE(pump.isFpPidActive);
}
