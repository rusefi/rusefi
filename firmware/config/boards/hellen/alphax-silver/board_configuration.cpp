#include "pch.h"
#include "hellen_meta.h"
#include "defaults.h"

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
