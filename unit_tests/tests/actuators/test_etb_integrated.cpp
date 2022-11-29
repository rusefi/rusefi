#include "pch.h"
#include "init.h"
#include "electronic_throttle_impl.h"
#include "live_data.h"

TEST(etb, integrated) {
	EngineTestHelper eth(TEST_ENGINE);
	etbPidReset(); // ETB controlles are global shared instances :(

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_3;
	engineConfiguration->tps1_2AdcChannel = EFI_ADC_3;

	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_3;
	engineConfiguration->throttlePedalPositionSecondAdcChannel = EFI_ADC_3;

	Sensor::setMockValue(SensorType::AcceleratorPedalPrimary, 40);
	Sensor::setMockValue(SensorType::AcceleratorPedalSecondary, 40);

	Sensor::setMockValue(SensorType::Tps1, 25.0f, true);

	initTps();
	doInitElectronicThrottle();

	EtbController *etb = (EtbController*)engine->etbControllers[0];
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
	ASSERT_EQ(destination, 0); // huh? what is broken here?
}
