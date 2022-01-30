/**
 * @author Andrey Belomutskiy, (c) 2012-2020
 */
#include "pch.h"

void grabTPSIsClosed() {
#if EFI_PROD_CODE
	printTPSInfo();
	engineConfiguration->tpsMin = convertVoltageTo10bitADC(Sensor::getRaw(SensorType::Tps1));
	printTPSInfo();
#endif /* EFI_PROD_CODE */
}

void grabTPSIsWideOpen() {
#if EFI_PROD_CODE
	printTPSInfo();
	engineConfiguration->tpsMax = convertVoltageTo10bitADC(Sensor::getRaw(SensorType::Tps1));
	printTPSInfo();
#endif /* EFI_PROD_CODE */
}

void grabPedalIsUp() {
#if EFI_PROD_CODE
	/**
	 * search for 'maintainConstantValue' to find how this TS magic works
	 */
	engine->outputChannels.calibrationMode = (uint8_t)TsCalMode::PedalMin;
	engine->outputChannels.calibrationValue = Sensor::getRaw(SensorType::AcceleratorPedalPrimary);
	engine->outputChannels.calibrationValue2 = Sensor::getRaw(SensorType::AcceleratorPedalSecondary);
#endif /* EFI_PROD_CODE */
}

void grabPedalIsWideOpen() {
#if EFI_PROD_CODE
	engine->outputChannels.calibrationMode = (uint8_t)TsCalMode::PedalMax;
	engine->outputChannels.calibrationValue = Sensor::getRaw(SensorType::AcceleratorPedalPrimary);
	engine->outputChannels.calibrationValue2 = Sensor::getRaw(SensorType::AcceleratorPedalSecondary);
#endif /* EFI_PROD_CODE */
}
