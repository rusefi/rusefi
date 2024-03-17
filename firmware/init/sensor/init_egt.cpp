#include "pch.h"

#include "init.h"
#include "AemXSeriesEgt.h"
#include "max3185x.h"

#if EFI_CAN_SUPPORT
static AemXSeriesEgt aemEgt1(0, SensorType::EGT1);
static AemXSeriesEgt aemEgt2(1, SensorType::EGT2);
#endif

void initEgt() {
#if EFI_CAN_SUPPORT
	if (engineConfiguration->enableAemXSeriesEgt) {
		if (!engineConfiguration->canWriteEnabled || !engineConfiguration->canReadEnabled) {
			criticalError("CAN read and write are required to use CAN EGT.");
			return;
		}

		registerCanSensor(aemEgt1);
		registerCanSensor(aemEgt2);

		return;
	}
#endif

#if EFI_MAX_31855
	initMax3185x(engineConfiguration->max31855spiDevice, engineConfiguration->max31855_cs);
#endif /* EFI_MAX_31855 */
}

void stopEgt(void)
{
	/* TODO: also stop CAN sensors */
#if EFI_MAX_31855
	stopMax3185x();
#endif /* EFI_MAX_31855 */
}

void startEgt(void)
{
	/* TODO: also start CAN sensors */
#if EFI_MAX_31855
	startMax3185x(engineConfiguration->max31855spiDevice, engineConfiguration->max31855_cs);
#endif /* EFI_MAX_31855 */
}
