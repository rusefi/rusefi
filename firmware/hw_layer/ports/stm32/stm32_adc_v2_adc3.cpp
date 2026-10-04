#include "pch.h"

#if HAL_USE_ADC
#include "adc_onchip.h"

#ifndef EFI_SLOW_ADC
#define EFI_SLOW_ADC ADCD1
#endif
#ifndef ADC_FAST_DEVICE
#define ADC_FAST_DEVICE ADCD2
#endif

#if EFI_ADC3_SLOW
#include "adc3_slow_config.h"
#include "adc_shared_sampler.h"
#include "ch.hpp"

// Sharing requires non-preemptible HAL transitions: the scheduler must not
// interrupt a DMA/error handler between stopping DMA and publishing ADC_READY.
// ADCv2 has a shared error IRQ, so all ADC DMA IRQs must match it as well.
static_assert(STM32_ADC_IRQ_PRIORITY == EFI_IRQ_SCHEDULING_TIMER_PRIORITY);
static_assert(STM32_ADC_ADC1_DMA_IRQ_PRIORITY == STM32_ADC_IRQ_PRIORITY);
static_assert(STM32_ADC_ADC2_DMA_IRQ_PRIORITY == STM32_ADC_IRQ_PRIORITY);
static_assert(STM32_ADC_ADC3_DMA_IRQ_PRIORITY == STM32_ADC_IRQ_PRIORITY);
static_assert(&ADCD3 != &EFI_SLOW_ADC);
#if EFI_USE_FAST_ADC
static_assert(&ADCD3 != &ADC_FAST_DEVICE);
#endif

static constexpr adc_channel_e adc3SlowChannels[] = ADC3_SLOW_CHANNELS;
static constexpr size_t adc3SlowCount = efi::size(adc3SlowChannels);
static constexpr size_t adc3SlowDepth = 8;
static_assert(adc3SlowCount > 0 && adc3SlowCount <= 16);
static AdcSharedSampler<adc3SlowCount, adc3SlowDepth> adc3SlowSampler;
static NO_CACHE adcsample_t adc3SlowBuffer[adc3SlowCount * adc3SlowDepth];
static bool adc3SlowInitialized = false;

static void adc3SlowComplete(ADCDriver*) {
	chibios_rt::CriticalSectionLocker csl;
	adc3SlowSampler.complete(adc3SlowBuffer, getTimeNowNt());
}

static void adc3SlowError(ADCDriver*, adcerror_t error) {
	chibios_rt::CriticalSectionLocker csl;
	adc3SlowSampler.fail();
	engine->outputChannels.slowAdcErrorCount++;
	if (error == ADC_ERR_OVERFLOW) {
		engine->outputChannels.slowAdcOverrunCount++;
	}
}

static ADCConversionGroup adc3SlowGroup = {
	.circular = FALSE,
	.num_channels = adc3SlowCount,
	.end_cb = adc3SlowComplete,
	.error_cb = adc3SlowError,
	.cr1 = 0,
	.cr2 = ADC_CR2_SWSTART,
	.smpr1 = 0,
	.smpr2 = 0,
	.htr = 0,
	.ltr = 0,
	.sqr1 = 0,
	.sqr2 = 0,
	.sqr3 = 0,
};

void initAdc3Slow() {
	for (size_t i = 0; i < adc3SlowCount; i++) {
		auto channel = adc3SlowChannels[i];
		int input = getAdcInternalChannel(ADC3, channel);
		// This path handles the pins that cannot be acquired through ADC1/2.
		if (channel < EFI_ADC_32 || channel > EFI_ADC_39 || input < 0 || input > 15) {
			criticalError("Invalid ADC3 slow input %d", channel);
			return;
		}
		adcConversionGroupSetSeqInput(&adc3SlowGroup, i, input);
		if (input < 10) {
			adc3SlowGroup.smpr2 |= ADC_SAMPLE_144 << (3 * input);
		} else {
			adc3SlowGroup.smpr1 |= ADC_SAMPLE_144 << (3 * (input - 10));
		}
	}
	// Software knock may already have started the driver. It uses its own
	// conversion group and DMA buffer; never restart an active driver here.
	if (ADCD3.state == ADC_STOP) {
		adcStart(&ADCD3, nullptr);
	}
	adc3SlowInitialized = true;
}

void adc3SlowPreemptI(ADCDriver* adc) {
	if (adc == &ADCD3 && adc3SlowSampler.active()) {
		adcStopConversionI(adc);
		adc3SlowSampler.cancel();
	}
}

void adc3SlowUpdate() {
	chibios_rt::CriticalSectionLocker csl;
	if (!adc3SlowInitialized) {
		return;
	}
	auto now = getTimeNowNt();
	// A batch normally needs much less than one 2 ms slow-sensor period.
	// Recover an interrupted/lost DMA transfer without touching a knock batch.
	if (adc3SlowSampler.timedOut(now, MS2NT(2))) {
		adc3SlowPreemptI(&ADCD3);
		adc3SlowSampler.fail();
		engine->outputChannels.slowAdcErrorCount++;
	}
	if (adc3SlowSampler.tryStart(ADCD3.state == ADC_READY, now)) {
		adcStartConversionI(&ADCD3, &adc3SlowGroup, adc3SlowBuffer, adc3SlowDepth);
	}
}

int adc3SlowRead(adc_channel_e channel) {
	chibios_rt::CriticalSectionLocker csl;
	for (size_t i = 0; i < adc3SlowCount; i++) {
		if (adc3SlowChannels[i] == channel) {
			// Re-reading a batch must never renew its age. Beyond three normal
			// sensor periods, return invalid and let subscribers time out.
			return adc3SlowSampler.read(i, getTimeNowNt(), MS2NT(6));
		}
	}
	return -1;
}
#endif // EFI_ADC3_SLOW
#endif // HAL_USE_ADC
