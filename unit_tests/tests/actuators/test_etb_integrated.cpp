#include "pch.h"
#include "init.h"
#include "electronic_throttle_impl.h"
#include "live_data.h"

static EtbController * initEtbIntegratedTest() {
	etbPidReset(); // ETB controlles are global shared instances :(

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_3;
	engineConfiguration->tps1_2AdcChannel = EFI_ADC_3;

	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_3;
	engineConfiguration->throttlePedalPositionSecondAdcChannel = EFI_ADC_3;


	Sensor::setMockValue(SensorType::Tps1, 25.0f, true);

	initTps();
	doInitElectronicThrottle();

	engine->etbControllers[0]->setIdlePosition(0);

	return (EtbController*)engine->etbControllers[0];
}

TEST(etb, integrated) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE); // we have a destructor so cannot move EngineTestHelper into utility method
	EtbController *etb = initEtbIntegratedTest();

	Sensor::setMockValue(SensorType::AcceleratorPedalPrimary, 40);
	Sensor::setMockValue(SensorType::AcceleratorPedalSecondary, 40);

	etb->update();

	ASSERT_EQ(engine->outputChannels.etbTarget, 40);
	ASSERT_NEAR(etb->prevOutput, 120.363, EPS3D);
	ASSERT_NEAR(etb->etbDutyAverage, 60.1813, EPS3D);

	Sensor::setMockValue(SensorType::AcceleratorPedal, 10, true);
	etb->update();
	ASSERT_NEAR(etb->etbDutyAverage, 70.0741, EPS3D);
	ASSERT_NEAR(etb->etbDutyRateOfChange, 130.2554, EPS3D);

	float destination;
	int offset = ELECTRONIC_THROTTLE_BASE_ADDRESS + offsetof(electronic_throttle_s, etbDutyRateOfChange);
	copyRange((uint8_t*)&destination, getLiveDataFragments(), offset, sizeof(destination));
	ASSERT_NEAR(destination, 130.2554, EPS3D);
}

extern int timeNowUs;

TEST(etb, intermittentTps) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE); // we have a destructor so cannot move EngineTestHelper into utility method
	EtbController *etb = initEtbIntegratedTest();

	// Tell the sensor checker that the ignition is on
	engine->module<SensorChecker>()->onIgnitionStateChanged(true);
	engine->module<SensorChecker>()->onSlowCallback();
	timeNowUs += 10e6;
	engine->module<SensorChecker>()->onSlowCallback();

	ASSERT_TRUE(engine->module<SensorChecker>()->analogSensorsShouldWork());

	ASSERT_FALSE(isTps1Error());

	etb->update();

	EXPECT_EQ(0, etb->etbTpsErrorCounter);
	EXPECT_EQ(0, etb->etbErrorCode);

	int badCount = 0;

	// Do some bad/good/bad/good cycles, make sure count keeps up
	for (size_t i = 0; i < 50; i++) {
		Sensor::setInvalidMockValue(SensorType::Tps1);
		ASSERT_TRUE(isTps1Error());
		etb->update();

		badCount++;
		EXPECT_EQ(badCount, etb->etbTpsErrorCounter);
		EXPECT_EQ(0, etb->etbErrorCode);

		Sensor::setMockValue(SensorType::Tps1, 20);
		ASSERT_FALSE(isTps1Error());
		etb->update();
	}

	// 51st bad TPS should set etbErrorCode
	Sensor::setInvalidMockValue(SensorType::Tps1);
	ASSERT_TRUE(isTps1Error());
	etb->update();

	EXPECT_NE(0, etb->etbErrorCode);
}

TEST(etb, intermittentPps) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE); // we have a destructor so cannot move EngineTestHelper into utility method

	Sensor::setMockValue(SensorType::AcceleratorPedal, 10, true);
	EtbController *etb = initEtbIntegratedTest();

	// Tell the sensor checker that the ignition is on
	engine->module<SensorChecker>()->onIgnitionStateChanged(true);
	engine->module<SensorChecker>()->onSlowCallback();
	timeNowUs += 10e6;
	engine->module<SensorChecker>()->onSlowCallback();

	ASSERT_TRUE(engine->module<SensorChecker>()->analogSensorsShouldWork());

	ASSERT_FALSE(isPedalError());

	etb->update();

	EXPECT_EQ(0, etb->etbPpsErrorCounter);
	EXPECT_EQ(0, etb->etbErrorCode);

	int badCount = 0;

	// Do some bad/good/bad/good cycles, make sure count keeps up
	for (size_t i = 0; i < 50; i++) {
		Sensor::setInvalidMockValue(SensorType::AcceleratorPedal);
		ASSERT_TRUE(isPedalError());
		etb->update();

		badCount++;
		EXPECT_EQ(badCount, etb->etbPpsErrorCounter);
		EXPECT_EQ(0, etb->etbErrorCode);

		Sensor::setMockValue(SensorType::AcceleratorPedal, 20);
		ASSERT_FALSE(isPedalError());
		etb->update();
	}

	// 51st bad TPS should set etbErrorCode
	Sensor::setInvalidMockValue(SensorType::AcceleratorPedal);
	ASSERT_TRUE(isPedalError());
	etb->update();
	EXPECT_NE(0, etb->etbErrorCode);
}
