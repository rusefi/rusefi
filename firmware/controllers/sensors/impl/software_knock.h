#pragma once

#include "pch.h"
#include <cstdint>

#include "fft/fft.h"

#define FFT_SIZE 1024

struct SpectrogramData {
	fft::complex_type fftBuffer[FFT_SIZE];
	float window[FFT_SIZE];
};

void initSoftwareKnock();
void knockSamplingCallback(uint8_t cylinderIndex, efitick_t nowNt);
