#pragma once

#include <cstddef>
#include <math.h>
#include <vector>
#include <complex>

namespace fft {

typedef float real_type;
typedef std::complex<real_type> complex_type;

bool fft_adc_sample(float * w, float ratio, const adcsample_t* data_in, complex_type* data_out, const size_t size);
bool fft(const real_type* data_in, complex_type* data_out, const size_t size);
void fft_freq(real_type* freq, const size_t size, const size_t sampleFreq);
void fft_amp(const complex_type* fft_data, real_type* amplitude, const size_t size);
void fft_db(real_type* amplitude, const size_t size);

void rectwin(float * w, unsigned n);
void hann(float * w, unsigned n, bool sflag);
void hamming(float * w, unsigned n, bool sflag);
void blackman(float * w, unsigned n, bool sflag);
void blackmanharris(float * w, unsigned n, bool sflag);

float get_main_freq(float* amplitudes, float* frequencies);

} 

#include "fft.hpp"
