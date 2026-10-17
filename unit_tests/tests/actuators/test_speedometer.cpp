#include "pch.h"

#include "bench_test.h"
#include "speedometer.h"

class SpeedometerBenchTest : public ::testing::Test {
protected:
	EngineTestHelper eth{engine_type_e::TEST_ENGINE};

	void SetUp() override {
		engine->scheduler.clear();
		setTimeNowUs(0);
		engineConfiguration->speedometerPulsePerKm = 3600;
		Sensor::setMockValue(SensorType::VehicleSpeed, 20); // 20 Hz normal output
		configureOutput();
	}

	void configureOutput() {
		engineConfiguration->speedometerOutputPin = Gpio::D7;
		enginePins.speedoOut.initPin("Speedometer test", Gpio::D7);
		initSpeedometer();
		speedoUpdate();
	}

	void TearDown() override {
		// Finish any active override before releasing the file-static PWM's scheduler/pin.
		eth.moveTimeForwardSec(4);
		speedoUpdate();
		engine->scheduler.clear();
		engineConfiguration->speedometerOutputPin = Gpio::Unassigned;
		initSpeedometer();
		enginePins.speedoOut.deInit();
	}

	void requestTest(int frequency) {
		engineConfiguration->speedometerBenchTestFrequency = frequency;
		executeTSCommand(TS_BENCH_CATEGORY, BENCH_SPEEDO_TEST);
		speedoUpdate();
	}

	int countEdges(int durationUs) {
		const int before = enginePins.speedoOut.pinToggleCounter;
		eth.moveTimeForwardAndInvokeEventsUs(durationUs);
		return enginePins.speedoOut.pinToggleCounter - before;
	}
};

TEST_F(SpeedometerBenchTest, tsCommandUsesRequestedFrequency) {
	EXPECT_EQ(100, engineConfiguration->speedometerBenchTestFrequency);
	Sensor::setMockValue(SensorType::VehicleSpeed, 0);
	requestTest(125);
	EXPECT_EQ(nullptr, takePendingBenchRequestForUnitTest().pin);

	// Let the initially paused PWM wake up, then count actual output transitions.
	eth.moveTimeForwardAndInvokeEventsUs(200000);
	EXPECT_EQ(250, countEdges(1000000));
	EXPECT_EQ(0, eth.getWarningCounter());
}

TEST_F(SpeedometerBenchTest, ignoresVehicleSpeedUntilThreeSecondsThenResumes) {
	requestTest(100);
	Sensor::setMockValue(SensorType::VehicleSpeed, 40);
	eth.setTimeAndInvokeEventsUs(2900000);
	// A fast callback just before expiry must still use the bench frequency.
	speedoUpdate();
	EXPECT_EQ(20, countEdges(100000));

	eth.setTimeAndInvokeEventsUs(3001000);
	speedoUpdate();
	eth.moveTimeForwardAndInvokeEventsUs(100000);
	EXPECT_EQ(80, countEdges(1000000));
}

TEST_F(SpeedometerBenchTest, repeatedCommandRestartsTheTimer) {
	requestTest(100);
	eth.setTimeAndInvokeEventsUs(2000000);
	requestTest(200);
	eth.setTimeAndInvokeEventsUs(4900000);
	speedoUpdate();
	EXPECT_EQ(40, countEdges(100000));

	eth.setTimeAndInvokeEventsUs(5001000);
	speedoUpdate();
	eth.moveTimeForwardAndInvokeEventsUs(100000);
	EXPECT_EQ(40, countEdges(1000000));
}

TEST_F(SpeedometerBenchTest, unconfiguredOutputRejectsCommand) {
	engine->scheduler.clear();
	engineConfiguration->speedometerOutputPin = Gpio::Unassigned;
	initSpeedometer();
	enginePins.speedoOut.deInit();
	requestTest(100);
	EXPECT_EQ(0, engine->scheduler.size());

	// The rejected request must not become active when an output is later configured.
	configureOutput();
	eth.moveTimeForwardAndInvokeEventsUs(200000);
	EXPECT_EQ(40, countEdges(1000000));
}

TEST_F(SpeedometerBenchTest, zeroFrequency) {
	requestTest(0);
	// Execute one callback explicitly: the reproduction asserted that the imported
	// command rescheduled it at the same timestamp, which loops the scheduler forever.
	auto* event = engine->scheduler.getHead();
	ASSERT_NE(nullptr, event);
	const auto action = event->action;
	setTimeNowUs(event->getMomentUs());
	engine->scheduler.cancel(event);
	action.execute();
	ASSERT_NE(nullptr, engine->scheduler.getHead());
	ASSERT_GT(engine->scheduler.getHead()->getMomentNt(), getTimeNowNt());
	EXPECT_FALSE(enginePins.speedoOut.getLogicValue());
	EXPECT_EQ(0, countEdges(1000000));

	eth.setTimeAndInvokeEventsUs(3100000);
	speedoUpdate();
	eth.moveTimeForwardAndInvokeEventsUs(100000);
	EXPECT_EQ(40, countEdges(1000000));
}
