#include "pch.h"

#include "hardware.h"
#include "stored_value_sensor.h"

namespace {

class TriggerRpmStartup : public testing::Test {
protected:
	EngineTestHelper eth{engine_type_e::TEST_ENGINE};
	StoredValueSensor externalRpm{SensorType::Rpm, 0};

	void SetUp() override {
		// [tag:trigger_rpm_startup] #4709: prior self-stimulation hid a missing
		// hardware RPM registration. The host helper also registers RPM, so remove
		// that shortcut and require initHardware() to populate an empty RPM slot.
		engine->rpmCalculator.unregister();
		Sensor::resetMockValue(SensorType::Rpm);
		engine->triggerCentral.directSelfStimulation = false;
		engine->triggerCentral.hwTriggerInputEnabled = true;
		for (auto& pin : engineConfiguration->triggerInputPins) {
			pin = Gpio::Unassigned;
		}
		for (auto& pin : engineConfiguration->camInputs) {
			pin = Gpio::Unassigned;
		}
		for (auto& pin : engineConfiguration->triggerSimulatorPins) {
			pin = Gpio::Unassigned;
		}
		ASSERT_EQ(nullptr, Sensor::getSensorOfType(SensorType::Rpm));
		ASSERT_FALSE(Sensor::hasSensor(SensorType::Rpm));
		ASSERT_FALSE(Sensor::get(SensorType::Rpm).Valid);
	}

	void TearDown() override {
		externalRpm.unregister();
	}
};

TEST_F(TriggerRpmStartup, PrimaryInputRegistersAndPublishesRpmWithoutSelfStimulation) {
	engineConfiguration->triggerInputPins[0] = Gpio::A5;
	engineConfiguration->trigger.type = trigger_type_e::TT_HALF_MOON;
	engine->updateTriggerConfiguration();
	// Startup must register RPM even if the configured pin has not changed.
	rememberCurrentConfiguration();
	initHardware();

	ASSERT_EQ(&engine->rpmCalculator, Sensor::getSensorOfType(SensorType::Rpm));
	ASSERT_TRUE(Sensor::hasSensor(SensorType::Rpm));
	EXPECT_FALSE(engine->triggerCentral.directSelfStimulation);

	// Half-moon has one rise/fall pair per 720 degrees. 40 ms between
	// edges gives 120 / 0.080 = 1500 RPM through the hardware-input handler.
	for (int edge = 0; edge < 4; edge++) {
		eth.moveTimeForwardAndInvokeEventsUs(40000);
		hwHandleShaftSignal(0, edge % 2 == 0, getTimeNowNt());
	}
	ASSERT_TRUE(engine->triggerCentral.triggerState.shaft_is_synchronized);
	EXPECT_EQ(0u, engine->triggerCentral.triggerState.totalTriggerErrorCounter);
	auto rpm = Sensor::get(SensorType::Rpm);
	ASSERT_TRUE(rpm.Valid);
	EXPECT_NEAR(1500, rpm.Value, 0.01f);

	// Repeated initialization must also preserve the registered provider.
	initHardware();
	EXPECT_EQ(&engine->rpmCalculator, Sensor::getSensorOfType(SensorType::Rpm));
	EXPECT_NEAR(1500, Sensor::getOrZero(SensorType::Rpm), 0.01f);
}

TEST_F(TriggerRpmStartup, NoTriggerPinsLeavesRpmUnregistered) {
	initHardware();
	EXPECT_EQ(nullptr, Sensor::getSensorOfType(SensorType::Rpm));
	EXPECT_FALSE(Sensor::hasSensor(SensorType::Rpm));
	EXPECT_FALSE(Sensor::get(SensorType::Rpm).Valid);
}

TEST_F(TriggerRpmStartup, SecondaryInputWithoutPrimaryIsRejectedWithoutRegisteringRpm) {
	engineConfiguration->triggerInputPins[1] = Gpio::A5;
	EXPECT_FATAL_ERROR(initHardware());
	EXPECT_EQ(nullptr, Sensor::getSensorOfType(SensorType::Rpm));
	EXPECT_FALSE(Sensor::hasSensor(SensorType::Rpm));
	EXPECT_FALSE(Sensor::get(SensorType::Rpm).Valid);
}

TEST_F(TriggerRpmStartup, CamAndSimulatorPinsDoNotReplacePrimaryInput) {
	engineConfiguration->camInputs[0] = Gpio::A6;
	engineConfiguration->triggerSimulatorPins[0] = Gpio::A7;
	initHardware();
	EXPECT_EQ(nullptr, Sensor::getSensorOfType(SensorType::Rpm));
	EXPECT_FALSE(Sensor::hasSensor(SensorType::Rpm));
	EXPECT_FALSE(Sensor::get(SensorType::Rpm).Valid);
}

TEST_F(TriggerRpmStartup, NoPrimaryInputPreservesAnExternalRpmProvider) {
	ASSERT_TRUE(externalRpm.Register());
	externalRpm.setValidValue(2345, getTimeNowNt());
	initHardware();
	EXPECT_EQ(&externalRpm, Sensor::getSensorOfType(SensorType::Rpm));
	EXPECT_FLOAT_EQ(2345, Sensor::getOrZero(SensorType::Rpm));
}

} // namespace
