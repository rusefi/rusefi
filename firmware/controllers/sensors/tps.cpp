/**
 * @author Andrey Belomutskiy, (c) 2012-2020
 */
#include "pch.h"
#include "sent.h"

/*
void grabTPSIsClosed() {
#if EFI_PROD_CODE
	printTPSInfo();
	engineConfiguration->tpsMin = convertVoltageTo10bitADC(Sensor::getRaw(SensorType::Tps1));
	printTPSInfo();
#endif // EFI_PROD_CODE
}

void grabTPSIsWideOpen() {
#if EFI_PROD_CODE
	printTPSInfo();
	engineConfiguration->tpsMax = convertVoltageTo10bitADC(Sensor::getRaw(SensorType::Tps1));
	printTPSInfo();
#endif // EFI_PROD_CODE
}
*/

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

float decodeTpsSentValue(float sentValue) {
    switch (engineConfiguration->sentEtbType) {
        case SentEtbType::GM_TYPE_1:
            return interpolateMsg("tps", /*x1*/0xE48,                                 /*y1*/0, /*x2*/0x1A0,                                 /*y2*/POSITION_FULLY_OPEN, /*x*/sentValue);
        case SentEtbType::FORD_TYPE_1:
            return interpolateMsg("tps", /*x1*/  250,                                 /*y1*/0, /*x2*/ 3560,                                 /*y2*/POSITION_FULLY_OPEN, /*x*/sentValue);
        default:
            return interpolateMsg("tps", /*x1*/engineConfiguration->customSentTpsMin, /*y1*/0, /*x2*/engineConfiguration->customSentTpsMax, /*y2*/POSITION_FULLY_OPEN, /*x*/sentValue);
    }
}

void sentTpsDecode() {
#if EFI_SENT_SUPPORT
    if (!isDigitalTps1()) {
        return;
    }
    // todo: move away from weird float API
    float sentValue = getSentValue(0);
    float tpsValue = decodeTpsSentValue(sentValue);

    sentTps.setValidValue(tpsValue, getTimeNowNt());
#endif // EFI_SENT_SUPPORT
}
