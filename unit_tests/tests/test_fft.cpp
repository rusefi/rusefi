#include "pch.h"

#include "software_knock.h"
#include "fft/fft.hpp"

#define KNOCK_SAMPLE_RATE 16

TEST(knock, fft) {
  SpectrogramData data;
  adcsample_t sampleBuffer[KNOCK_SIZE];

  float ratio = 1;

  for (int i = 0;i<KNOCK_SIZE;i++) {
    data.window[i] = 35;
    sampleBuffer[i] = i;
  }

  fft::fft_adc_sample(data.window, ratio, sampleBuffer, data.fftBuffer, KNOCK_SIZE);
  fft::fft_freq(data.frequencies, KNOCK_SIZE, KNOCK_SAMPLE_RATE);

	fft::fft_amp(data.fftBuffer, data.amplitudes, KNOCK_SIZE);

	float mainFreq = fft::get_main_freq(data.amplitudes, data.frequencies, KNOCK_SIZE / 2);

	ASSERT_NEAR(mainFreq, -4.2955f, EPS4D);

}
