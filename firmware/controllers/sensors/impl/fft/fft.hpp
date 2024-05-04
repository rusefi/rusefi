#pragma once

namespace fft {

#ifndef M_PI
#define M_PI 3.1415926535897932
#endif


inline bool isPow(const size_t num)
{
    return num && (!(num & (num - 1)));
}

void rerrange(complex_type* data, const size_t num_elements)
{
    size_t target_index = 0;
    size_t bit_mask;

    complex_type buffer;
    for (size_t i = 0; i < num_elements; ++i)
    {
        if (target_index > i)
        {
            buffer = data[target_index];
            data[target_index] = data[i];
            data[i]= buffer;
        }

        bit_mask = num_elements;

        while (target_index & (bit_mask >>= 1)) 
        {
            target_index &= ~bit_mask;
        }

        target_index |= bit_mask;
    }
}

bool transform(complex_type* data, const size_t count)
{
    double local_pi = -M_PI;

    size_t next, match;
    real_type sine;
    real_type delta;
    complex_type mult, factor, product;

    for (size_t i = 1; i < count; i <<= 1)
    {
        next = i << 1;
        delta = local_pi / i;
        sine = sin(0.5 * delta);

        mult = complex_type(-2.0 * sine * sine, sin(delta));
        factor = 1.0;

        for (size_t j = 0; j < i; ++j)
        {
            for (size_t k = j; k < count; k += next) 
            {
                match = k + i;

                product = data[match] * factor;
                data[match] = data[k] - product;
                data[k] += product;
            }

            factor = mult * factor + factor;
        }
    }

    return true;
}

static bool ffti(complex_type* data, const size_t size)
{
    if(!isPow(size)) {
        return false;
    }

    rerrange(data, size);

    return transform(data, size);
}

bool fft_adc_sample(float * w, float ratio, const adcsample_t* data_in, complex_type* data_out, const size_t size)
{
    for(size_t i = 0; i < size; ++i) {
		float voltage = ratio * data_in[i];
		//float db = 10 * log10(voltage * voltage);
		//db = clampF(-100, db, 100);
        data_out[i] = complex_type(voltage * w[i], 0.0);
    }

    return ffti(data_out, size);
}

bool fft(const real_type* data_in, complex_type* data_out, const size_t size)
{
    for(size_t i = 0; i < size; ++i) {
        data_out[i] = complex_type(data_in[i], 0.0);
    }

    return ffti(data_out, size);
}

void fft_freq(real_type* freq, const size_t size, const size_t sampleFreq)
{
    for (size_t i = 0; i < size/2; i++)
    {
        freq[i] = ((real_type)i * sampleFreq) / size;
    }
}

void fft_amp(const complex_type* fft_data, real_type* amplitude, const size_t size)
{
    for (size_t i = 0; i < size/2; ++i)
    {
        amplitude[i] = abs(fft_data[i].imag());
    }
}

void fft_db(real_type* amplitude, const size_t size)
{
    for (size_t i = 0; i < size/2; ++i)
    {
        amplitude[i] = log10(amplitude[i] * amplitude[i]) * 10;
    }
}

void cosine_window(float * w, unsigned n, const float * coeff, unsigned ncoeff, bool sflag)
{
    if (n == 1)
    {
        w[0] = 1.0;
    }
    else
    {
        const unsigned wlength = sflag ? (n - 1) : n;

        for (unsigned i = 0; i < n; ++i)
        {
            float wi = 0.0;

            for (unsigned j = 0; j < ncoeff; ++j)
            {
                wi += coeff[j] * cos(i * j * 2.0 * M_PI / wlength);
            }

            w[i] = wi;
        }
    }
}

void rectwin(float * w, unsigned n)
{
    for (unsigned i = 0; i < n; ++i)
    {
        w[i] = 1.0;
    }
}

void hann(float * w, unsigned n, bool sflag)
{
    const float coeff[2] = { 0.5, -0.5 };

    cosine_window(w, n, coeff, sizeof(coeff) / sizeof(float), sflag);
}

void hamming(float * w, unsigned n, bool sflag)
{
    const float coeff[2] = { 0.54, -0.46 };
    cosine_window(w, n, coeff, sizeof(coeff) / sizeof(float), sflag);
}

void blackman(float * w, unsigned n, bool sflag)
{
    const float coeff[3] = { 0.42, -0.5, 0.08 };
    cosine_window(w, n, coeff, sizeof(coeff) / sizeof(float), sflag);
}

void blackmanharris(float * w, unsigned n, bool sflag)
{
    const float coeff[4] = { 0.35875, -0.48829, 0.14128, -0.01168 };
    cosine_window(w, n, coeff, sizeof(coeff) / sizeof(float), sflag);
}

float get_main_freq(float* amplitudes, float* frequencies, size_t size)
{
    float peaks_amp = -100.f;
    size_t peaks_index = 0;

    for (size_t i = 0; i < size; ++i)
    {
        float amp = amplitudes[i];
        if(amp > peaks_amp) {
            peaks_amp = amp;
            peaks_index = i;
        }
    }

    float sum_amps =    amplitudes[peaks_index - 2] +
                        amplitudes[peaks_index - 1] +  
                        amplitudes[peaks_index] + 
                        amplitudes[peaks_index + 1] +
                        amplitudes[peaks_index + 2];

    float sum_wighted = amplitudes[peaks_index - 2] * frequencies[peaks_index - 2] + 
                        amplitudes[peaks_index - 1] * frequencies[peaks_index - 1] + 
                        amplitudes[peaks_index]     * frequencies[peaks_index] + 
                        amplitudes[peaks_index + 1] * frequencies[peaks_index + 1] +
                        amplitudes[peaks_index + 2] * frequencies[peaks_index + 2];

    float mean_freq = sum_wighted / sum_amps;
    return mean_freq;
}

}
