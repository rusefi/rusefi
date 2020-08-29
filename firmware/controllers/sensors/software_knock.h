#pragma once

#include <cstdint>

void initSoftwareKnock();
void startKnockSampling(uint8_t cylinderIndex);
void processLastKnockEvent();
