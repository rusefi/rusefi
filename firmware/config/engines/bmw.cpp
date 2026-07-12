#include "pch.h"
#include "bmw.h"

void bmwM52() {
    engineConfiguration->ignitionMode = IM_WASTED_SPARK;
    engineConfiguration->cylindersCount = 6;
    engineConfiguration->displacement = 3;
    engineConfiguration->firingOrder = FO_1_5_3_6_2_4;
}

void bmwN52() {
    strcpy(engineConfiguration->engineMake, ENGINE_MAKE_BMW);
    strcpy(engineConfiguration->engineCode, "N52");
    engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
    engineConfiguration->cylindersCount = 6;
    engineConfiguration->displacement = 3;
    engineConfiguration->firingOrder = FO_1_5_3_6_2_4;
    engineConfiguration->trigger.type = trigger_type_e::TT_TOOTHED_WHEEL_60_2;
}
