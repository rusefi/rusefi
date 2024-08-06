#include "pch.h"

#ifdef TRIGGER_SCOPE

#include "trigger_scope.h"
#include "trigger_scope_config.h"

static BigBufferHandle buffer;

static bool isRunning = false;

static void completionCallback(ADCDriver* adcp) {
	if (isRunning && adcp->state == ADC_COMPLETE) {
		engine->outputChannels.triggerScopeReady = true;
	}
}

static const uint32_t smpr1 =
	ADC_SMPR1_SMP_AN10(TRIGGER_SCOPE_SAMPLE_TIME) |
	ADC_SMPR1_SMP_AN11(TRIGGER_SCOPE_SAMPLE_TIME) |
	ADC_SMPR1_SMP_AN12(TRIGGER_SCOPE_SAMPLE_TIME) |
	ADC_SMPR1_SMP_AN13(TRIGGER_SCOPE_SAMPLE_TIME) |
	ADC_SMPR1_SMP_AN14(TRIGGER_SCOPE_SAMPLE_TIME) |
	ADC_SMPR1_SMP_AN15(TRIGGER_SCOPE_SAMPLE_TIME);

static const uint32_t smpr2 =
	ADC_SMPR2_SMP_AN0(TRIGGER_SCOPE_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN1(TRIGGER_SCOPE_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN2(TRIGGER_SCOPE_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN3(TRIGGER_SCOPE_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN4(TRIGGER_SCOPE_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN5(TRIGGER_SCOPE_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN6(TRIGGER_SCOPE_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN7(TRIGGER_SCOPE_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN8(TRIGGER_SCOPE_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN9(TRIGGER_SCOPE_SAMPLE_TIME);

static const ADCConversionGroup adcConvGroupCh1 = { FALSE, 2, &completionCallback, nullptr,
	ADC_CR1_RES_1,	// Sample in 8-bit mode
	ADC_CR2_SWSTART,
	// sample times for channels 10...18
	smpr1,
	// sample times for channels 0...9
	smpr2,

	0,	// htr
	0,	// ltr

	0,	// sqr1
	0,	// sqr2
	ADC_SQR3_SQ1_N(TRIGGER_SCOPE_ADC_CH1) | ADC_SQR3_SQ2_N(TRIGGER_SCOPE_ADC_CH2)
};

static constexpr size_t sampleCount = BIG_BUFFER_SIZE / (2 * sizeof(uint8_t));

static void startSampling(void* = nullptr) {
	chibios_rt::CriticalSectionLocker csl;

	if (buffer && !engineConfiguration->enableSoftwareKnock) {
		// Cancel if ADC isn't ready
		if (!((TRIGGER_SCOPE_ADC.state == ADC_READY) ||
				(TRIGGER_SCOPE_ADC.state == ADC_COMPLETE) ||
				(TRIGGER_SCOPE_ADC.state == ADC_ERROR))) {
			triggerScopeDisable();
			return;
		}

		adcStartConversionI(&TRIGGER_SCOPE_ADC, &adcConvGroupCh1, buffer.get<adcsample_t>(), sampleCount);
	}
}

// Enable one buffer's worth of perf tracing, and retrieve the buffer size in bytes
void triggerScopeEnable() {
	buffer = getBigBuffer(BigBufferUser::TriggerScope);

	isRunning = true;

	startSampling();
}

void triggerScopeDisable() {
	// we're done with the buffer - let somebody else have it
	buffer = {};

	isRunning = false;
	engine->outputChannels.triggerScopeReady = false;
}

static scheduling_s restartTimer;

// Retrieve the trace buffer
const BigBufferHandle& triggerScopeGetBuffer() {
	engine->outputChannels.triggerScopeReady = false;

	// Start the next sample once we've read out this one
	if (isRunning) {
		engine->scheduler.schedule("trigger scope", &restartTimer, getTimeNowNt() + MS2NT(10), startSampling);
	}

	return buffer;
}

void initTriggerScope() {
	// Trigger scope and knock currently mutually exclusive
	if (!engineConfiguration->enableSoftwareKnock) {
		adcStart(&TRIGGER_SCOPE_ADC, nullptr);

		// Manually set ADC DMA to byte mode, as we'll be taking 8 bit samples
		// Defaults are to sample at 12 bits, and DMA 16-bit words
		ADCD3.dmamode &= ~(STM32_DMA_CR_PSIZE_MASK | STM32_DMA_CR_MSIZE_MASK);
		ADCD3.dmamode |= STM32_DMA_CR_PSIZE_BYTE | STM32_DMA_CR_MSIZE_BYTE;

		efiSetPadMode("trg ch1", TRIGGER_SCOPE_PIN_CH1, PAL_MODE_INPUT_ANALOG);
#if TRIGGER_SCOPE_HAS_CH2
		efiSetPadMode("trg ch2", TRIGGER_SCOPE_PIN_CH2, PAL_MODE_INPUT_ANALOG);
#endif
	}
}

#endif // TRIGGER_SCOPE
