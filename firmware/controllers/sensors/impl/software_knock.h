// file software_knock.h

#pragma once

#include "pch.h"
#include <cstdint>

void initSoftwareKnock();
void knockSamplingCallback(uint8_t cylinderIndex, efitick_t nowNt);

#define KNOCK_SIZE 512

#ifdef KNOCK_SPECTROGRAM
void knockSpectrogramEnable();
void knockSpectrogramDisable();
#endif
