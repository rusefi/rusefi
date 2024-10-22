#include "pch.h"

#include "software_knock.h"
#include "fft/fft.hpp"

#define KNOCK_SAMPLE_RATE 16

TEST(knock, fft) {
  SpectrogramData data;
  adcsample_t sampleBuffer[FFT_SIZE];

  float ratio = 1;
  float sensetivity = 1;

  for (int i = 0;i<FFT_SIZE;i++) {
    data.window[i] = 35;
    sampleBuffer[i] = i;
  }

  fft::fft_adc_sample(data.window, ratio, sensetivity, sampleBuffer, data.fftBuffer, FFT_SIZE);

  ASSERT_NEAR(data.fftBuffer[2].imag(), 730090, 1);
  ASSERT_NEAR(data.fftBuffer[3].imag(), 486696, 1);
  ASSERT_NEAR(data.fftBuffer[4].imag(), 364990, 1);

  ASSERT_NEAR(data.fftBuffer[2].real(), -8960, 1);
  ASSERT_NEAR(data.fftBuffer[3].real(), -8960, 1);
  ASSERT_NEAR(data.fftBuffer[4].real(), -8960, 1);
}
