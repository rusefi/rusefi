#include "pch.h"
#include "init.h"
#include "electronic_throttle_impl.h"
#include "mocks.h"
#include "live_data.h"
#include "live_data_ids.h"
#include <memory>

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

extern int ebtResetCounter;

// [tag:etb_test_lifetime] Static controllers must not outlive the configuration
// owned by a helper. Inspect public state without dereferencing the stale PID.
TEST(EtbLifetime, ControllersAfterHelperDestruction) {
	EtbController* controllers[ETB_COUNT];
	{
		EngineTestHelper eth(engine_type_e::TEST_ENGINE);
		Sensor::setMockValue(SensorType::Tps1, 25, true);
		Sensor::setMockValue(SensorType::Tps2, 30, true);
		doInitElectronicThrottle(true);

		for (size_t i = 0; i < ETB_COUNT; i++) {
			controllers[i] = static_cast<EtbController*>(engine->etbControllers[i]);
			ASSERT_EQ(static_cast<uint8_t>(EtbState::SuccessfulInit), controllers[i]->state);
			controllers[i]->setLuaAdjustment(12 + i);
		}
	}

	for (size_t i = 0; i < ETB_COUNT; i++) {
		EXPECT_EQ(DC_None, controllers[i]->getFunction());
		EXPECT_EQ(static_cast<uint8_t>(EtbState::Uninitialized), controllers[i]->state);
		EXPECT_FLOAT_EQ(0, controllers[i]->getLuaAdjustment());
	}

	// Heap allocation prevents a new stack helper from masking the expired
	// configuration's address. This is the same setup path as LuaAccelEnrichment.
	{
		auto next = std::make_unique<EngineTestHelper>(engine_type_e::TEST_ENGINE);
		ASSERT_FALSE(Sensor::hasSensor(SensorType::Tps1));
		ASSERT_FALSE(Sensor::hasSensor(SensorType::Tps2));
		setupSimpleTestEngineWithMafAndTT_ONE_trigger(next.get());

		// Also exercise teardown of a heap-owned configuration, then a helper
		// whose disabled slots bypass controller initialization entirely.
		Sensor::setMockValue(SensorType::Tps1, 25, true);
		Sensor::setMockValue(SensorType::Tps2, 30, true);
		doInitElectronicThrottle(true);
		for (auto controller : controllers) {
			ASSERT_EQ(static_cast<uint8_t>(EtbState::SuccessfulInit), controller->state);
		}
	}
	{
		auto next = std::make_unique<EngineTestHelper>(engine_type_e::TEST_ENGINE,
			[](engine_configuration_s* cfg) {
				for (auto& function : cfg->etbFunctions) {
					function = DC_None;
				}
			});
		setupSimpleTestEngineWithMafAndTT_ONE_trigger(next.get());
		for (auto controller : controllers) {
			EXPECT_EQ(DC_None, controller->getFunction());
		}
	}
}

static EtbController * initEtbIntegratedTest() {
	etbPidReset(); // ETB controlles are global shared instances :(

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_3;
	engineConfiguration->tps1_2AdcChannel = EFI_ADC_3;

	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_3;
	engineConfiguration->throttlePedalPositionSecondAdcChannel = EFI_ADC_3;


	Sensor::setMockValue(SensorType::Tps1, 25.0f, true);

	initTps();
	doInitElectronicThrottle(/*isInit*/true);

	engine->etbControllers[0]->setIdlePosition(0);

	return (EtbController*)engine->etbControllers[0];
}

TEST(etb, integrated) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE); // we have a destructor so cannot move EngineTestHelper into utility method
  etbPidReset();
  ASSERT_EQ(0, ebtResetCounter);
	EtbController *etb = initEtbIntegratedTest();
  ASSERT_EQ(1, ebtResetCounter);
	doInitElectronicThrottle(/*isInit*/false);
  ASSERT_EQ(1, ebtResetCounter);

	Sensor::setMockValue(SensorType::AcceleratorPedalPrimary, 40);
	Sensor::setMockValue(SensorType::AcceleratorPedalSecondary, 40);

	etb->update();

	ASSERT_EQ(etb->m_adjustedTarget, 40);

	Sensor::setMockValue(SensorType::AcceleratorPedal, 10, true);
	etb->update();
}

TEST(etb, failedPedalDisarmsAntilagAndClosesEtbTarget) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->antiLagEnabled = true;
	engineConfiguration->antiLagActivationMode = ALWAYS_ON_ANTILAG;
	engineConfiguration->ALSMinRPM = 400;
	engineConfiguration->ALSMaxRPM = 3200;
	engineConfiguration->ALSMinCLT = 50;
	engineConfiguration->ALSMaxCLT = 105;
	engineConfiguration->ALSMaxTPS = 5;
	// Keep anti-lag available across the six-second sensor startup delay below.
	// Duration-limit behavior is covered by AntilagStopsAfterConfiguredTimeout.
	engineConfiguration->ALSMaxDuration = 0;
	engineConfiguration->ALSEtbAdd = 20;
	engineConfiguration->etbIdleThrottleRange = 0;
	engineConfiguration->etbMinimumPosition = 1;
	engineConfiguration->etbMaximumPosition = 100;
	Sensor::setMockValue(SensorType::Rpm, 2000);
	Sensor::setMockValue(SensorType::Clt, 80);
	Sensor::setMockValue(SensorType::Tps1, 10, true);
	Sensor::setMockValue(SensorType::AcceleratorPedal, 20, true);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 20);
	StrictMock<MockVp3d> pedalMap;
	EXPECT_CALL(pedalMap, getValue(_, _))
		.WillRepeatedly([](float, float pedal) { return pedal; });
	StrictMock<MockMotor> motor;
	EtbController etb;
	ASSERT_TRUE(etb.init(DC_Throttle1, &motor, &engineConfiguration->etb, &pedalMap));
	engine->antilagController.update();
	ASSERT_FALSE(engine->antilagController.isAntilagCondition);
	ASSERT_NEAR(20, etb.getSetpoint().value_or(-1), 0.01f);

	auto& checker = engine->module<SensorChecker>().unmock();
	checker.onIgnitionStateChanged(true);
	eth.moveTimeForwardSec(6);
	checker.onSlowCallback();
	ASSERT_TRUE(checker.analogSensorsShouldWork());

	Sensor::setMockValue(SensorType::AcceleratorPedal, 0, true);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0);
	engine->antilagController.update();
	ASSERT_TRUE(engine->antilagController.ALSTimerCondition);
	ASSERT_TRUE(engine->antilagController.isAntilagCondition);
	ASSERT_NEAR(20, etb.getSetpoint().value_or(-1), 0.01f);

	Sensor::setInvalidMockValue(SensorType::DriverThrottleIntent);
	// A failed intent reading must not keep the previous anti-lag target.
	EXPECT_NEAR(1, etb.getSetpoint().value_or(-1), 0.01f);
	engine->antilagController.update();
	EXPECT_FALSE(engine->antilagController.isAntilagCondition);

	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0);
	engine->antilagController.update();
	ASSERT_TRUE(engine->antilagController.isAntilagCondition);
	Sensor::setInvalidMockValue(SensorType::AcceleratorPedal);
	// ETB must use its fault fallback even before anti-lag updates its state.
	EXPECT_NEAR(1, etb.getSetpoint().value_or(-1), 0.01f);
	Sensor::setInvalidMockValue(SensorType::DriverThrottleIntent);
	checker.onSlowCallback();
	engine->antilagController.update();
	EXPECT_FALSE(engine->antilagController.ALSMaxThrottleIntentCondition);
	EXPECT_FALSE(engine->antilagController.isAntilagCondition);
	EXPECT_NEAR(1, etb.getSetpoint().value_or(-1), 0.01f);
	EXPECT_CALL(motor, enable());
	EXPECT_CALL(motor, set(::testing::Lt(0.0f))).WillOnce(Return(false));
	etb.update();
}

TEST(etb, intermittentTps) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE); // we have a destructor so cannot move EngineTestHelper into utility method
	EtbController *etb = initEtbIntegratedTest();
	warningBuffer_t *recentWarnings = getRecentWarnings();
	recentWarnings->clear();

	// Tell the sensor checker that the ignition is on
	engine->module<SensorChecker>()->onIgnitionStateChanged(true);
	engine->module<SensorChecker>()->onSlowCallback();
	// checks start 5 seconds after ignition on
	advanceTimeUs(MS2US(6000));
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
	doInitElectronicThrottle(/*isInit*/true);
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
