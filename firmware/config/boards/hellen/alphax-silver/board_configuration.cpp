#include "pch.h"
#include "hellen_meta.h"
#include "defaults.h"
#include "hellen_leds_100.cpp"

//static void setInjectorPins() {
//
//}


static void setupDefaultSensorInputs() {
	engineConfiguration->clt.adcChannel = H144_IN_CLT;
	engineConfiguration->iat.adcChannel = H144_IN_IAT;
}

void setBoardConfigOverrides() {
	setHellenVbatt();
  hellenMegaSdWithAccelerometer();
  setHellenMegaEnPin();
  setDefaultHellenAtPullUps();
}

void setBoardDefaultConfiguration() {
  setHellenMMbaro();
  setupDefaultSensorInputs();
	setInline4();
}
