/**
 * @author Andrey Belomutskiy, (c) 2012-2020
 */
#include "pch.h"
#include "sent.h"

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
	/**
	 * search for 'maintainConstantValue' to find how this TS magic works
	 */
	engine->outputChannels.calibrationMode = (uint8_t)TsCalMode::PedalMin;
	engine->outputChannels.calibrationValue = Sensor::getRaw(SensorType::AcceleratorPedalPrimary);
	engine->outputChannels.calibrationValue2 = Sensor::getRaw(SensorType::AcceleratorPedalSecondary);
}

void grabPedalIsWideOpen() {
	engine->outputChannels.calibrationMode = (uint8_t)TsCalMode::PedalMax;
	engine->outputChannels.calibrationValue = Sensor::getRaw(SensorType::AcceleratorPedalPrimary);
	engine->outputChannels.calibrationValue2 = Sensor::getRaw(SensorType::AcceleratorPedalSecondary);
}

bool isTps1Error() {
	return !Sensor::get(SensorType::Tps1).Valid;
}

bool isTps2Error() {
    return !Sensor::get(SensorType::Tps2).Valid && Sensor::hasSensor(SensorType::Tps2Primary);
}

bool isPedalError() {
    return !Sensor::get(SensorType::AcceleratorPedal).Valid && Sensor::hasSensor(SensorType::AcceleratorPedalPrimary);
}

extern SentTps sentTps;

void sentTpsDecode() {
#if EFI_SENT_SUPPORT
    if (!isDigitalTps1()) {
        return;
    }
    // todo: move away from weird float API
    float sentValue = getSentValue(0);
    float tpsValue;
    switch (engineConfiguration->sentEtbType) {
        case SentEtbType::GM_TYPE_1:
            tpsValue = interpolateClamped(/*x1*/0xE48,                                /*y1*/0, /*x2*/0x1A0,                                  /*y2*/100, /*x*/sentValue);
            break;
        default:
            tpsValue = interpolateClamped(/*x1*/engineConfiguration->customSentTpsMin, /*y1*/0, /*x2*/engineConfiguration->customSentTpsMax, /*y2*/100, /*x*/sentValue);
            break;
    }

    sentTps.setValidValue(tpsValue, getTimeNowNt());
#endif // EFI_SENT_SUPPORT
}