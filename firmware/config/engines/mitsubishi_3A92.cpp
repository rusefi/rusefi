#include "pch.h"

void setMitsubishi3A92() {
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	engineConfiguration->cylindersCount = 3.0;
	engineConfiguration->displacement = 1.2;
	engineConfiguration->vvtMode[0] = VVT_MITSUBISHI_3A92;

	engineConfiguration->maxCamPhaseResolveRpm = 1500.0;
	engineConfiguration->crankingInjectionMode = IM_SEQUENTIAL;
}
