#include "pch.h"

// set engine_type 12
void setMitsubishi3A92() {
	engineConfiguration->trigger.type = trigger_type_e::TT_36_2_1_1;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	engineConfiguration->cylindersCount = 3.0;
	engineConfiguration->displacement = 1.2;
    engineConfiguration->globalTriggerAngleOffset = -360.0;
	engineConfiguration->vvtMode[0] = VVT_MITSUBISHI_3A92;
    engineConfiguration->vvtOffsets[0] = 316.0;

	engineConfiguration->maxCamPhaseResolveRpm = 1500.0;
	engineConfiguration->crankingInjectionMode = IM_SEQUENTIAL;
}
