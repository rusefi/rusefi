#include "pch.h"
#include "init.h"
#include "electronic_throttle_impl.h"

TEST(etb, integrated) {
	EngineTestHelper eth(TEST_ENGINE);

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

}
