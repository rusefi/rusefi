#include "pch.h"
#include "bmw_m52.h"

void bmwM52() {
    engineConfiguration->ignitionMode = IM_WASTED_SPARK;
    engineConfiguration->cylindersCount = 6;
    engineConfiguration->displacement = 3;
}
