// file software_knock.h

#pragma once

#include "pch.h"
#include <cstdint>

#include "fft/fft.h"

void initSoftwareKnock();
void knockSamplingCallback(uint8_t cylinderIndex, efitick_t nowNt);

#define KNOCK_SIZE 512

#ifdef KNOCK_SPECTROGRAM
void knockSpectrogramEnable();
void knockSpectrogramDisable();
#endif
