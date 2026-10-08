#include "pch.h"

#include "software_knock.h"
#include "fft/fft.hpp"

#include <algorithm>
#include <array>
#include <sstream>
#include <vector>

namespace {
constexpr double pi = 3.14159265358979323846;

void recordError(const char* name, double error) {
	std::ostringstream value;
	value << std::scientific << error;
	::testing::Test::RecordProperty(name, value.str());
}

// Independent O(N^2) double DFT: no FFT butterflies or float twiddle recurrence.
std::complex<double> referenceBin(const std::vector<double>& input, size_t bin) {
	std::complex<double> result{};
	for (size_t i = 0; i < input.size(); i++) {
		const double phase = -2 * pi * bin * i / input.size();
		result += input[i] * std::complex<double>(std::cos(phase), std::sin(phase));
	}
	return result;
}

// Normalize by input L1 norm, not each bin: silent bins still get a useful
// absolute error limit. This bounds error relative to the largest possible bin.
void checkSpectrum(const std::vector<double>& input, const fft::complex_type* actual,
		double& maxError) {
	double scale = 0;
	for (double sample : input) {
		scale += std::abs(sample);
	}
	ASSERT_GT(scale, 0);
	for (size_t bin = 0; bin < input.size(); bin++) {
		SCOPED_TRACE(bin);
		const auto expected = referenceBin(input, bin);
		const double error = std::abs(std::complex<double>(actual[bin]) - expected) / scale;
		ASSERT_TRUE(std::isfinite(error));
		maxError = std::max(maxError, error);
	}
}
}

TEST(FftPrecision, TransformAgainstDoubleDft) {
	double maxError = 0;
	// Include the production size, DC, Nyquist, coherent/off-bin tones and noise.
	for (size_t size : {16u, 64u, static_cast<unsigned>(FFT_SIZE)}) {
		for (int signal = 0; signal < 6; signal++) {
			for (double amplitude : {1e-4, 1.0, 4095.0}) {
				SCOPED_TRACE(::testing::Message() << "size=" << size << " signal=" << signal
					<< " amplitude=" << amplitude);
				std::vector<float> input(size);
				std::vector<double> reference(size);
				std::vector<fft::complex_type> output(size);
				uint32_t noise = 12345;
				for (size_t i = 0; i < size; i++) {
					double sample;
					switch (signal) {
					case 0: sample = 1; break;
					case 1: sample = i == size / 3 ? 1 : 0; break;
					case 2: sample = i % 2 ? -1 : 1; break;
					case 3: sample = std::sin(2 * pi * 3 * i / size + 0.37); break;
					case 4: sample = std::cos(2 * pi * 5.25 * i / size)
						+ 0.01 * std::sin(2 * pi * 1.5 * i / size); break;
					default:
						noise = noise * 1664525u + 1013904223u;
						sample = static_cast<double>(noise >> 8) / 8388608.0 - 1;
						break;
					}
					input[i] = amplitude * sample;
					// Compare the same rounded input to isolate transform precision.
					reference[i] = input[i];
				}
				ASSERT_TRUE(fft::fft(input.data(), output.data(), size));
				checkSpectrum(reference, output.data(), maxError);
			}
		}
	}
	recordError("max_normalized_complex_error", maxError);
	EXPECT_LT(maxError, 2e-6);
}

TEST(FftPrecision, WindowsAgainstDoubleCosine) {
	struct Window {
		const char* name;
		void (*generate)(float*, unsigned, bool);
		std::array<double, 4> coefficients;
	};
	const Window windows[] = {
		{"hann", fft::hann, {0.5, -0.5, 0, 0}},
		{"hamming", fft::hamming, {0.54, -0.46, 0, 0}},
		{"blackman", fft::blackman, {0.42, -0.5, 0.08, 0}},
		{"blackmanharris", fft::blackmanharris, {0.35875, -0.48829, 0.14128, -0.01168}},
	};
	double maxError = 0;
	for (const auto& window : windows) {
		for (unsigned size : {1u, 2u, 17u, static_cast<unsigned>(FFT_SIZE)}) {
			for (bool symmetric : {false, true}) {
				SCOPED_TRACE(::testing::Message() << window.name << " size=" << size
					<< " symmetric=" << symmetric);
				std::vector<float> actual(size);
				window.generate(actual.data(), size, symmetric);
				for (unsigned i = 0; i < size; i++) {
					double expected = size == 1 ? 1 : 0;
					if (size > 1) {
						for (size_t j = 0; j < window.coefficients.size(); j++) {
							expected += window.coefficients[j]
								* std::cos(2 * pi * i * j / (symmetric ? size - 1 : size));
						}
					}
					ASSERT_TRUE(std::isfinite(actual[i]));
					maxError = std::max(maxError, std::abs(actual[i] - expected));
				}
			}
		}
	}
	recordError("max_absolute_window_error", maxError);
	// Absolute error also protects the near-zero ends of the windows.
	// Float phase arithmetic and cosf peak around 3.78e-7; allow rounding
	// margin across hosts while keeping the independent double reference.
	EXPECT_LT(maxError, 5e-7);
}

TEST(FftPrecision, FastSqrtAgainstDoubleSqrt) {
	EXPECT_FLOAT_EQ(fft::fast_sqrt(0), 0);
	double maxError = 0;
	// Dense mantissa sweep across small signals through squared ADC-scale bins.
	// Negative inputs, subnormals and overflow are outside this amplitude domain.
	for (int exponent = -40; exponent <= 48; exponent++) {
		for (int mantissa = 0; mantissa < 256; mantissa++) {
			const float input = std::ldexp(1.0f + mantissa / 256.0f, exponent);
			const double expected = std::sqrt(static_cast<double>(input));
			const float actual = fft::fast_sqrt(input);
			ASSERT_TRUE(std::isfinite(actual));
			maxError = std::max(maxError, std::abs(actual / expected - 1));
		}
	}
	recordError("max_relative_sqrt_error", maxError);
	// Master peaks around 1.006e-4 (0.0101%); leave rounding margin across hosts.
	EXPECT_LT(maxError, 1.1e-4);
}

TEST(FftPrecision, WindowedAdcSpectrumAndAmplitude) {
	SpectrogramData data;
	std::array<adcsample_t, FFT_SIZE> samples;
	std::vector<double> reference(FFT_SIZE);
	const float ratio = 3.3f / 4095;
	const float sensitivity = 0.75f;
	fft::blackmanharris(data.window, FFT_SIZE, true);
	for (size_t i = 0; i < FFT_SIZE; i++) {
		// A strong off-bin component plus a weaker coherent tone, with ADC bias.
		samples[i] = static_cast<adcsample_t>(2048 + 1500 * std::sin(2 * pi * 113.25 * i / FFT_SIZE)
			+ 30 * std::cos(2 * pi * 217 * i / FFT_SIZE));
		const double phase = 2 * pi * i / (FFT_SIZE - 1);
		const double window = 0.35875 - 0.48829 * std::cos(phase)
			+ 0.14128 * std::cos(2 * phase) - 0.01168 * std::cos(3 * phase);
		reference[i] = samples[i] * static_cast<double>(ratio) * sensitivity * window;
	}
	ASSERT_TRUE(fft::fft_adc_sample(data.window, ratio, sensitivity,
		samples.data(), data.fftBuffer, FFT_SIZE));
	double maxError = 0;
	checkSpectrum(reference, data.fftBuffer, maxError);
	recordError("max_normalized_adc_complex_error", maxError);
	EXPECT_LT(maxError, 2e-6);

	double maxAmplitudeError = 0;
	for (size_t bin : {0u, 112u, 113u, 114u, 216u, 217u, 218u}) {
		const double expected = std::abs(referenceBin(reference, bin));
		const float actual = fft::amplitude(data.fftBuffer[bin]);
		ASSERT_TRUE(std::isfinite(actual));
		maxAmplitudeError = std::max(maxAmplitudeError, std::abs(actual / expected - 1));
	}
	EXPECT_FLOAT_EQ(fft::amplitude({0, 0}), 0);
	recordError("max_relative_amplitude_error", maxAmplitudeError);
	EXPECT_LT(maxAmplitudeError, 2e-4);
}
