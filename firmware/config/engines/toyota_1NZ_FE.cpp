#include "pch.h"

void setToyota1NZFE() {
    engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
    engineConfiguration->displacement = 1.5;
    engineConfiguration->globalTriggerAngleOffset = 230.0;
    engineConfiguration->vvtMode[0] = VVT_TOYOTA_4_1;
    engineConfiguration->vvtOffsets[0] = 316.0;
}
