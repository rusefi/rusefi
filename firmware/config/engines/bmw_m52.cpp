#include "pch.h"
#include "bmw_m52.h"

void bmwM52() {
    engineConfiguration->ignitionMode = IM_WASTED_SPARK;
    engineConfiguration->cylindersCount = 6;
    engineConfiguration->displacement = 3;
    engineConfiguration->firingOrder = FO_1_5_3_6_2_4;
}
