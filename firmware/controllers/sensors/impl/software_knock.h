// file software_knock.h

#pragma once

#include "pch.h"
#include <cstdint>

#include "fft/fft.h"

void initSoftwareKnock();
void knockSamplingCallback(uint8_t cylinderIndex, efitick_t nowNt);

#define KNOCK_SIZE 512

struct SpectrogramData {
	fft::complex_type fftBuffer[KNOCK_SIZE];
	float frequencies[KNOCK_SIZE/2];
	float amplitudes[KNOCK_SIZE/2];
	float window[KNOCK_SIZE];
};

#ifdef KNOCK_SPECTROGRAM
void knockSpectrogramEnable();
void knockSpectrogramDisable();
#endif
