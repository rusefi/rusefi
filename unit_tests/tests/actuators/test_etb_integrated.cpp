#include "pch.h"
#include "init.h"
#include "electronic_throttle_impl.h"
#include "live_data.h"
#include "live_data_ids.h"

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

	ASSERT_EQ(etb->m_adjustedTarget, 40);

	Sensor::setMockValue(SensorType::AcceleratorPedal, 10, true);
	etb->update();
}

TEST(etb, intermittentTps) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE); // we have a destructor so cannot move EngineTestHelper into utility method
	EtbController *etb = initEtbIntegratedTest();
	warningBuffer_t *recentWarnings = getRecentWarnings();
	recentWarnings->clear();

	// Tell the sensor checker that the ignition is on
	engine->module<SensorChecker>()->onIgnitionStateChanged(true);
	engine->module<SensorChecker>()->onSlowCallback();
	advanceTimeUs(MS2US(1000));
	engine->module<SensorChecker>()->onSlowCallback();
	// todo: fix me https://github.com/rusefi/rusefi/issues/5233
	// EXPECT_EQ( 3,  recentWarnings.getCount()) << "intermittentTps";
	EXPECT_TRUE( recentWarnings->getCount() > 0) << "intermittentTps";

	ASSERT_TRUE(engine->module<SensorChecker>()->analogSensorsShouldWork());

	ASSERT_TRUE(Sensor::get(SensorType::Tps1).Valid);

	etb->update();

	EXPECT_EQ(0, etb->etbTpsErrorCounter);
	EXPECT_EQ(0, etb->etbErrorCode);

	int badCount = 0;

	// Do some bad/good/bad/good cycles, make sure count keeps up
	for (size_t i = 0; i < 50; i++) {
		Sensor::setInvalidMockValue(SensorType::Tps1);
		ASSERT_FALSE(Sensor::get(SensorType::Tps1).Valid);
		etb->update();

		badCount++;
		EXPECT_EQ(badCount, etb->etbTpsErrorCounter);
		EXPECT_EQ((int)EtbStatus::TpsError, etb->etbErrorCode);

		Sensor::setMockValue(SensorType::Tps1, 20);
		ASSERT_TRUE(Sensor::get(SensorType::Tps1).Valid);
		etb->update();
	}

	// 51st bad TPS should set etbErrorCode
	Sensor::setInvalidMockValue(SensorType::Tps1);
	ASSERT_FALSE(Sensor::get(SensorType::Tps1).Valid);
	etb->update();

	EXPECT_NE(0, etb->etbErrorCode);

	// todo: fix me https://github.com/rusefi/rusefi/issues/5233
	// EXPECT_EQ( 3,  recentWarnings.getCount()) << "intermittentTps";
	EXPECT_TRUE( recentWarnings->getCount() > 0) << "intermittentTps";
	// todo: fix me https://github.com/rusefi/rusefi/issues/5233
//	EXPECT_EQ(OBD_PPS_Correlation, recentWarnings.get(0).Code);
//	EXPECT_EQ(OBD_TPS1_Primary_Timeout, recentWarnings.get(1).Code);
//	EXPECT_EQ(OBD_PPS_Primary_Timeout, recentWarnings.get(2).Code);
}

TEST(etb, intermittentPps) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE); // we have a destructor so cannot move EngineTestHelper into utility method

	Sensor::setMockValue(SensorType::AcceleratorPedal, 10, true);
	EtbController *etb = initEtbIntegratedTest();

	// Tell the sensor checker that the ignition is on
	engine->module<SensorChecker>()->onIgnitionStateChanged(true);
	engine->module<SensorChecker>()->onSlowCallback();
	advanceTimeUs(10e6);
	engine->module<SensorChecker>()->onSlowCallback();

	ASSERT_TRUE(engine->module<SensorChecker>()->analogSensorsShouldWork());

	ASSERT_TRUE(Sensor::get(SensorType::AcceleratorPedal).Valid);

	etb->update();

	EXPECT_EQ(0, etb->etbPpsErrorCounter);
	EXPECT_EQ(0, etb->etbErrorCode);

	int badCount = 0;

	// Do some bad/good/bad/good cycles, make sure count keeps up
	for (size_t i = 0; i < 50; i++) {
		Sensor::setInvalidMockValue(SensorType::AcceleratorPedal);
		ASSERT_FALSE(Sensor::get(SensorType::AcceleratorPedal).Valid);
		etb->update();

		badCount++;
		EXPECT_EQ(badCount, etb->etbPpsErrorCounter);
		EXPECT_EQ(0, etb->etbErrorCode);

		Sensor::setMockValue(SensorType::AcceleratorPedal, 20);
		ASSERT_TRUE(Sensor::get(SensorType::AcceleratorPedal).Valid);
		etb->update();
	}

	// 51st bad TPS should set etbErrorCode
	Sensor::setInvalidMockValue(SensorType::AcceleratorPedal);
	ASSERT_FALSE(Sensor::get(SensorType::AcceleratorPedal).Valid);
	etb->update();
	EXPECT_NE(0, etb->etbErrorCode);
}

TEST(etb, sentTpsIntegrated) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE); // we have a destructor so cannot move EngineTestHelper into utility method

	engineConfiguration->sentInputPins[0] = Gpio::B3;
	engineConfiguration->sentEtbType = SentEtbType::GM_TYPE_1;

	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_3;
	engineConfiguration->throttlePedalPositionSecondAdcChannel = EFI_ADC_3;

	Sensor::setMockValue(SensorType::Tps1, 25.0f, true);

	initTps();
	doInitElectronicThrottle();
}

TEST(etb, sentTpsIntegratedDecode) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE); // we have a destructor so cannot move EngineTestHelper into utility method

	engineConfiguration->sentEtbType = SentEtbType::GM_TYPE_1;

	ASSERT_NEAR(20.246, decodeTpsSentValue(3000), EPS2D);

	engineConfiguration->sentEtbType = SentEtbType::CUSTOM;
	engineConfiguration->customSentTpsMin = 5000;
	engineConfiguration->customSentTpsMax = 1000;
	ASSERT_NEAR(75, decodeTpsSentValue(2000), EPS2D);
}
