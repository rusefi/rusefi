#pragma once

#include "pch.h"
#include <cstdint>

void initSoftwareKnock();
void knockSamplingCallback(uint8_t cylinderIndex, efitick_t nowNt);
void processLastKnockEvent();
