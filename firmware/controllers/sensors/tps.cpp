/**
 * @author Andrey Belomutskiy, (c) 2012-2020
 */
#include "pch.h"
#include "sent.h"
#include "tunerstudio.h"

/*
void grabTPSIsClosed() {
#if EFI_PROD_CODE
	engineConfiguration->tpsMin = convertVoltageTo10bitADC(Sensor::getRaw(SensorType::Tps1));
#endif // EFI_PROD_CODE
}

void grabTPSIsWideOpen() {
#if EFI_PROD_CODE
	engineConfiguration->tpsMax = convertVoltageTo10bitADC(Sensor::getRaw(SensorType::Tps1));
#endif // EFI_PROD_CODE
}
*/

static void onGrabPedal() {
  static uint8_t grabPedalCounter = 0;
	grabPedalCounter++;
	if (grabPedalCounter % 2 == 0) {
	  // todo fix root cause! work-around: make sure not to write bad tune since that would brick requestBurn();
	}
}

void grabPedalIsUp() {
	/**
	 * search for 'maintainConstantValue' to find how this TS magic works
	 */
	engine->outputChannels.calibrationMode = (uint8_t)TsCalMode::PedalMin;
	engine->outputChannels.calibrationValue = Sensor::getRaw(SensorType::AcceleratorPedalPrimary);
	engine->outputChannels.calibrationValue2 = Sensor::getRaw(SensorType::AcceleratorPedalSecondary);
	onGrabPedal();
}

void grabPedalIsWideOpen() {
	engine->outputChannels.calibrationMode = (uint8_t)TsCalMode::PedalMax;
	engine->outputChannels.calibrationValue = Sensor::getRaw(SensorType::AcceleratorPedalPrimary);
	engine->outputChannels.calibrationValue2 = Sensor::getRaw(SensorType::AcceleratorPedalSecondary);
	onGrabPedal();
}

// In case of cable throttle we need support calibration of primary sensor of first throttle only
void grapTps1PrimaryIsClosed()
{
	engine->outputChannels.calibrationMode = (uint8_t)TsCalMode::Tps1Min;
	engine->outputChannels.calibrationValue = Sensor::getRaw(SensorType::Tps1Primary);
}

void grapTps1PrimaryIsOpen()
{
	engine->outputChannels.calibrationMode = (uint8_t)TsCalMode::Tps1Max;
	engine->outputChannels.calibrationValue = Sensor::getRaw(SensorType::Tps1Primary);
}

#if EFI_SENT_SUPPORT

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

#if EFI_PROD_CODE
void sentTpsDecode(SentInput sentCh) {
    if ((!isDigitalTps1()) || (engineConfiguration->EtbSentInput != sentCh)) {
        return;
    }
    // todo: move away from weird float API
    float sentValue = getSentValue(sentCh);
    float tpsValue = decodeTpsSentValue(sentValue);

    sentTps.setValidValue(tpsValue, getTimeNowNt());
}
#endif // EFI_PROD_CODE

bool isDigitalTps1() {
    return (engineConfiguration->sentEtbType != SentEtbType::NONE);
}

#endif /* EFI_SENT_SUPPORT */
