#include "pch.h"

#if EFI_SOFTWARE_KNOCK

#include "biquad.h"
#include "thread_controller.h"
#include "knock_logic.h"
#include "software_knock.h"
#include "knock_config.h"
#include "ch.hpp"
#include "error_handling.h"

#ifdef KNOCK_SPECTROGRAM
#include "fft/fft.hpp"

#define COMPRESSED_SPECTRUM_PROTOCOL_SIZE 16 // 16 * 4 = 64 byte for transport to TS
#define START_SPECTRORGAM_FREQUENCY 4000 // magic minimum Hz for draw spectrogram, use near value +next 64 freqs from fft

static size_t spectrogramStartIndex = 0;
static SpectrogramData spectrogramData0;
static SpectrogramData* spectrogramData = &spectrogramData0;

// TODO: use big_buffer
//static volatile bool enableKnockSpectrogram = false;
//static BigBufferHandle buffer;
//static SpectrogramData* spectrogramData = nullptr;
#endif //KNOCK_SPECTROGRAM


static NO_CACHE adcsample_t sampleBuffer[1800];
static int8_t currentCylinderNumber = 0;
static int8_t channelNumber = 0;
static efitick_t lastKnockSampleTime = 0;
static Biquad knockFilter;

static volatile bool knockIsSampling = false;
static volatile bool knockNeedsProcess = false;
static volatile size_t sampleCount = 0;

chibios_rt::BinarySemaphore knockSem(/* taken =*/ true);

void onKnockSamplingComplete() {
	knockNeedsProcess = true;

	// Notify the processing thread that it's time to process this sample
	chSysLockFromISR();
	knockSem.signalI();
	chSysUnlockFromISR();
}

void onStartKnockSampling(uint8_t cylinderNumber, float samplingSeconds, uint8_t channelIdx) {
	if (!engineConfiguration->enableSoftwareKnock) {
		return;
	}

	// Cancel if ADC isn't ready
	if (!((KNOCK_ADC.state == ADC_READY) ||
			(KNOCK_ADC.state == ADC_ERROR))) {
		return;
	}

	// If there's pending processing, skip this event
	if (knockNeedsProcess) {
		return;
	}

	// Convert sampling time to number of samples
	constexpr int sampleRate = KNOCK_SAMPLE_RATE;
	sampleCount = 0xFFFFFFFE & static_cast<size_t>(clampF(100, samplingSeconds * sampleRate, efi::size(sampleBuffer)));

	// Select the appropriate conversion group - it will differ depending on which sensor this cylinder should listen on
	auto conversionGroup = getKnockConversionGroup(channelIdx);

  //current chanel number for spectrum TS plugin
	channelNumber = channelIdx;

	// Stash the current cylinder's number so we can store the result appropriately
	currentCylinderNumber = cylinderNumber;

	adcStartConversionI(&KNOCK_ADC, conversionGroup, sampleBuffer, sampleCount);
	lastKnockSampleTime = getTimeNowNt();
}

class KnockThread : public ThreadController<UTILITY_THREAD_STACK_SIZE> {
public:
	KnockThread() : ThreadController("knock", PRIO_KNOCK_PROCESS) {}
	void ThreadTask() override;
};

static KnockThread kt;

void initSoftwareKnock() {
	if (engineConfiguration->enableSoftwareKnock) {

		float frequencyHz;

		if (engineConfiguration->knockFrequency > 0.01) {
			frequencyHz = engineConfiguration->knockFrequency;
		} else {
		  frequencyHz = 1000 * bore2frequency(engineConfiguration->cylinderBore);
      frequencyHz = engineConfiguration->knockDetectionUseDoubleFrequency ? 2 * frequencyHz : frequencyHz;
		}

		knockFilter.configureBandpass(KNOCK_SAMPLE_RATE, frequencyHz, 3);

	#ifdef KNOCK_SPECTROGRAM
		if (engineConfiguration->enableKnockSpectrogram) {

			// TODO: use big buffer
			//buffer = getBigBuffer(BigBufferUser::KnockSpectrogram);
			// if (!buffer) {
			// 	engineConfiguration->enableKnockSpectrogram = false;
			//  	return;
			//  }
			//spectrogramData = buffer.get<SpectrogramData>();

			fft::blackmanharris(spectrogramData->window, FFT_SIZE, true);

			int freqStartConst = START_SPECTRORGAM_FREQUENCY;
			int minFreqDiff = freqStartConst;
			int freqStart = 0;
			float freqStep = 0;

			for (size_t i = 0; i < FFT_SIZE/2; i++)
			{
				float freq = float(i * KNOCK_SAMPLE_RATE) / FFT_SIZE;
				int min = abs(freq - freqStartConst);

				// next after freq start index
				if(i == spectrogramStartIndex + 1) {
					freqStep = abs(freq - freqStart);
				}

				if(min < minFreqDiff) {
					minFreqDiff = min;
					spectrogramStartIndex = i;
					freqStart = freq;
				}
			}

			engine->module<KnockController>()->m_knockFrequencyStart = (uint16_t)freqStart;
			engine->module<KnockController>()->m_knockFrequencyStep = freqStep;
		}
  #else // KNOCK_SPECTROGRAM
    criticalAssertVoid(!engineConfiguration->enableKnockSpectrogram, "KNOCK_SPECTROGRAM not enabled");
	#endif // KNOCK_SPECTROGRAM

  // fun fact: we do not offer any ADC channel flexibility like we have for many other kinds of inputs
		efiSetPadMode("knock ch1", KNOCK_PIN_CH1, PAL_MODE_INPUT_ANALOG);
#if KNOCK_HAS_CH2
		efiSetPadMode("knock ch2", KNOCK_PIN_CH2, PAL_MODE_INPUT_ANALOG);
#endif
		kt.start();
	}
}

#ifdef KNOCK_SPECTROGRAM
static uint8_t toDb(const float& voltage) {
	float db = 200 * log10(voltage*voltage) + 40; // best scaling for view
	db = clampF(0, db, 255);
	return uint8_t(db);
}
#endif

static void processLastKnockEvent() {
	if (!knockNeedsProcess) {
		return;
	}

	float sumSq = 0;

	// todo: reduce magic constants. engineConfiguration->adcVcc?
	constexpr float ratio = 3.3f / 4095.0f;

	size_t localCount = sampleCount;

	// Prepare the steady state at vcc/2 so that there isn't a step
	// when samples begin
	// todo: reduce magic constants. engineConfiguration->adcVcc?
	knockFilter.cookSteadyState(3.3f / 2);

	// Compute the sum of squares
	for (size_t i = 0; i < localCount; i++) {
		float volts = ratio * sampleBuffer[i];

		float filtered = knockFilter.filter(volts);
		if (i == localCount - 1 && engineConfiguration->debugMode == DBG_KNOCK) {
			engine->outputChannels.debugFloatField1 = volts;
			engine->outputChannels.debugFloatField2 = filtered;
		}

		sumSq += filtered * filtered;
	}

	// take a local copy
	auto lastKnockTime = lastKnockSampleTime;

	// We're done with inspecting the buffer, another sample can be taken
	knockNeedsProcess = false;

#ifdef KNOCK_SPECTROGRAM
	if (engineConfiguration->enableKnockSpectrogram) {
		ScopePerf perf(PE::KnockAnalyzer);

		if (engineConfiguration->enableKnockSpectrogramFilter) {
			fft::fft_adc_sample_filtered(knockFilter, spectrogramData->window, ratio, engineConfiguration->knockSpectrumSensitivity, sampleBuffer, spectrogramData->fftBuffer, FFT_SIZE);
		} else {
			fft::fft_adc_sample(spectrogramData->window, ratio, engineConfiguration->knockSpectrumSensitivity, sampleBuffer, spectrogramData->fftBuffer, FFT_SIZE);
		}

		auto* spectrum = &engine->module<KnockController>()->m_knockSpectrum[0];
		for(uint8_t i = 0; i < COMPRESSED_SPECTRUM_PROTOCOL_SIZE; ++i) {

			uint8_t startIndex = spectrogramStartIndex + (i * 4);

			uint8_t a = toDb(fft::amplitude(spectrogramData->fftBuffer[startIndex]));
			uint8_t b = toDb(fft::amplitude(spectrogramData->fftBuffer[startIndex + 1]));
			uint8_t c = toDb(fft::amplitude(spectrogramData->fftBuffer[startIndex + 2]));
			uint8_t d = toDb(fft::amplitude(spectrogramData->fftBuffer[startIndex + 3]));

			uint32_t compressed = uint32_t(a << 24 | b << 16 | c << 8 | d);

      {
		    chibios_rt::CriticalSectionLocker csl;
			  spectrum[i] = compressed;
			}
		}

		uint16_t compressedChannelCyl = uint16_t(channelNumber << 8 | currentCylinderNumber);

		{
		  chibios_rt::CriticalSectionLocker csl;
		  engine->module<KnockController>()->m_knockSpectrumChannelCyl = compressedChannelCyl;
		}
	}

#endif

	// mean of squares (not yet root)
	float meanSquares = sumSq / localCount;

	// RMS
	float db = 10 * log10(meanSquares);

	// clamp to reasonable range
	db = clampF(-100, db, 100);

	engine->module<KnockController>()->onKnockSenseCompleted(currentCylinderNumber, db, lastKnockTime);
}

void KnockThread::ThreadTask() {
	while (1) {
		knockSem.wait();

		ScopePerf perf(PE::SoftwareKnockProcess);
		processLastKnockEvent();
	}
}


#endif // EFI_SOFTWARE_KNOCK
