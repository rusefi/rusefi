#include "pch.h"

#include "init.h"
#include "AemXSeriesEgt.h"
#include "max3185x.h"

#if EFI_CAN_SUPPORT
static AemXSeriesEgt aemEgt1(0, SensorType::EGT1);
static AemXSeriesEgt aemEgt2(1, SensorType::EGT2);
static AemXSeriesEgt aemEgt3(2, SensorType::EGT3);
static AemXSeriesEgt aemEgt4(3, SensorType::EGT4);
static AemXSeriesEgt aemEgt5(4, SensorType::EGT5);
static AemXSeriesEgt aemEgt6(5, SensorType::EGT6);
#endif

void initEgt() {
#if EFI_CAN_SUPPORT
	if (engineConfiguration->enableAemXSeriesEgt) {
		if (!engineConfiguration->canReadEnabled) {
			criticalError("CAN read is required to use CAN EGT.");
			return;
		}

		registerCanSensor(aemEgt1);
		registerCanSensor(aemEgt2);
		registerCanSensor(aemEgt3);
		registerCanSensor(aemEgt4);
		registerCanSensor(aemEgt5);
		registerCanSensor(aemEgt6);

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
