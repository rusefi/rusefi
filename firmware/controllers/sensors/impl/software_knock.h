#pragma once

#include "pch.h"
#include <cstdint>

#include "fft/fft.h"

void initSoftwareKnock();
void knockSamplingCallback(uint8_t cylinderIndex, efitick_t nowNt);
