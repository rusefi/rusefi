#pragma once

#include "pch.h"
#include <cstddef>
#include <math.h>
#include <vector>
#include <complex>
#include "biquad.h"

#if EFI_UNIT_TEST
typedef uint16_t adcsample_t;
#endif

namespace fft {

typedef float real_type;
typedef std::complex<real_type> complex_type;

bool fft_adc_sample(float * w, float ratio, float sensitivity, const adcsample_t* data_in, complex_type* data_out, const size_t size);
bool fft_adc_sample_filtered(Biquad& knockFilter, float * w, float ratio, float sensitivity, const adcsample_t* data_in, complex_type* data_out, const size_t size);
bool fft(const real_type* data_in, complex_type* data_out, const size_t size);

void rectwin(float * w, unsigned n);
void hann(float * w, unsigned n, bool sflag);
void hamming(float * w, unsigned n, bool sflag);
void blackman(float * w, unsigned n, bool sflag);
void blackmanharris(float * w, unsigned n, bool sflag);

float fast_sqrt(float x);
float amplitude(const complex_type& fft);

}

