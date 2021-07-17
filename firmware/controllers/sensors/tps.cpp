/**
 * @author Andrey Belomutskiy, (c) 2012-2020
 */
#include "tps.h"
#if EFI_PROD_CODE
#include "settings.h"
#endif /* EFI_PROD_CODE */

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
	engineConfiguration->throttlePedalUpVoltage = Sensor::getRaw(SensorType::AcceleratorPedal);
	printTPSInfo();
#endif /* EFI_PROD_CODE */
}

void grabPedalIsWideOpen() {
#if EFI_PROD_CODE
	engineConfiguration->throttlePedalWOTVoltage = Sensor::getRaw(SensorType::AcceleratorPedal);
	printTPSInfo();
#endif /* EFI_PROD_CODE */
}
