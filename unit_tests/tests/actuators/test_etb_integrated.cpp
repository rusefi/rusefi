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
	return (EtbController*)engine->etbControllers[0];
}

TEST(etb, integrated) {
	EngineTestHelper eth(TEST_ENGINE); // we have a distractor so cannot move EngineTestHelper into utility method
	EtbController *etb = initEtbIntegratedTest();

	Sensor::setMockValue(SensorType::AcceleratorPedalPrimary, 40);
	Sensor::setMockValue(SensorType::AcceleratorPedalSecondary, 40);

	etb->update();

	ASSERT_EQ(engine->outputChannels.etbTarget, 40);
	ASSERT_EQ(etb->prevOutput, 100);
	ASSERT_EQ(etb->etbDutyAverage, 50);

	Sensor::setMockValue(SensorType::AcceleratorPedal, 10, true);
	etb->update();
	ASSERT_EQ(etb->etbDutyAverage, -25);
	ASSERT_EQ(etb->etbDutyRateOfChange, -75);

	float destination;
	int offset = ELECTRONIC_THROTTLE_BASE_ADDRESS + offsetof(electronic_throttle_s, etbDutyRateOfChange);
	copyRange((uint8_t*)&destination, getLiveDataFragments(), offset, sizeof(destination));
	ASSERT_EQ(destination, -75);
}



TEST(etb, integratedTpsJitter) {
	EngineTestHelper eth(TEST_ENGINE); // we have a distractor so cannot move EngineTestHelper into utility method
	EtbController *etb = initEtbIntegratedTest();

	ASSERT_FALSE(isTps1Error());
	ASSERT_FALSE(isTps2Error());
	ASSERT_TRUE(isPedalError());

	etb->update();

	Sensor::setInvalidMockValue(SensorType::AcceleratorPedal);
	ASSERT_TRUE(isPedalError());
	etb->update();

	ASSERT_EQ(1, etb->etbInputErrorCounter);

	Sensor::setInvalidMockValue(SensorType::Tps1);
	etb->update();

	ASSERT_EQ(2, etb->etbInputErrorCounter);
}
