/**
 * @file	adc_inputs_onchip.cpp
 * @brief	Low level ADC code
 *
 * rusEfi uses two ADC devices on the same 16 pins at the moment. Two ADC devices are used in order to distinguish between
 * fast and slow devices. The idea is that but only having few channels in 'fast' mode we can sample those faster?
 *
 * Slow ADC group is used for IAT, CLT, AFR, VBATT etc - this one is currently sampled at 500Hz
 *
 * Fast ADC group is used for MAP, MAF HIP - this one is currently sampled at 10KHz
 *  We need frequent MAP for map_averaging.cpp
 *
 * 10KHz equals one measurement every 3.6 degrees at 6000 RPM
 *
 * PS: analog muxes allow to double number of analog inputs
 * oh, and ADC3 is dedicated for knock
 *
 * @date Jan 14, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#if HAL_USE_ADC

#include "adc_device.h"
#include "adc_subscription.h"
#include "mpu_util.h"
#include "periodic_thread_controller.h"
#include "protected_gpio.h"
#include "fast_adc_diagnostics.h"

#ifndef ADC_MAX_CHANNELS_COUNT
#define ADC_MAX_CHANNELS_COUNT 16
#endif /* ADC_MAX_CHANNELS_COUNT */

#if EFI_USE_FAST_ADC

// is there a reason to have this configurable at runtime?
#ifndef ADC_FAST_DEVICE
#define ADC_FAST_DEVICE		ADCD2
#endif

// Depth of the conversion buffer, channels are sampled X times each
#ifndef ADC_BUF_DEPTH_FAST
#define ADC_BUF_DEPTH_FAST	4
#endif

// See https://github.com/rusefi/rusefi/issues/976 for discussion on this value
#ifndef ADC_SAMPLING_FAST
#define ADC_SAMPLING_FAST	ADC_SAMPLE_28
#endif

AdcDevice::AdcDevice(ADCDriver *p_adcp, ADCConversionGroup* p_hwConfig, volatile adcsample_t *p_buf, size_t p_depth) {
	adcp = p_adcp;
	depth = p_depth;
	hwConfig = p_hwConfig;
	samples = p_buf;

	hwConfig->sqr1 = 0;
	hwConfig->sqr2 = 0;
	hwConfig->sqr3 = 0;
#if ADC_MAX_CHANNELS_COUNT > 16
	hwConfig->sqr4 = 0;
	hwConfig->sqr5 = 0;
#endif /* ADC_MAX_CHANNELS_COUNT */
	memset(internalAdcIndexByHardwareIndex, 0xFF, sizeof(internalAdcIndexByHardwareIndex));
}

static void fastAdcDoneCB(ADCDriver *adcp);
static void fastAdcErrorCB(ADCDriver *, adcerror_t err);

static ADCConversionGroup adcgrpcfgFast = {
#if defined(EFI_INTERNAL_FAST_ADC_PWM)
	.circular			= TRUE,
#elif defined (EFI_INTERNAL_FAST_ADC_GPT)
	.circular			= FALSE,
#endif
	.num_channels		= 0,
	.end_cb				= fastAdcDoneCB,
	.error_cb			= fastAdcErrorCB,
	/* HW dependent part.*/
	.cr1				= 0,
#if defined(EFI_INTERNAL_FAST_ADC_PWM)
	/* HW start using TIM8 CC 1 event rising edge
	 * See "External trigger for regular channels" for magic 13 number
	 * NOTE: Currently only TIM8 in PWM mode is supported */
	.cr2				= ADC_CR2_EXTEN_0 | (13 << ADC_CR2_EXTSEL_Pos),
#elif defined (EFI_INTERNAL_FAST_ADC_GPT)
	/* SW start through GPT callback and SW kick */
	.cr2				= ADC_CR2_SWSTART,
#endif
		/**
		 * here we configure all possible channels for fast mode. Some channels would not actually
         * be used hopefully that's fine to configure all possible channels.
		 *
		 */
	// sample times for channels 10...18
	.smpr1 =
		ADC_SMPR1_SMP_AN10(ADC_SAMPLING_FAST) |
		ADC_SMPR1_SMP_AN11(ADC_SAMPLING_FAST) |
		ADC_SMPR1_SMP_AN12(ADC_SAMPLING_FAST) |
		ADC_SMPR1_SMP_AN13(ADC_SAMPLING_FAST) |
		ADC_SMPR1_SMP_AN14(ADC_SAMPLING_FAST) |
		ADC_SMPR1_SMP_AN15(ADC_SAMPLING_FAST),
	// In this field must be specified the sample times for channels 0...9
	.smpr2 =
		ADC_SMPR2_SMP_AN0(ADC_SAMPLING_FAST) |
		ADC_SMPR2_SMP_AN1(ADC_SAMPLING_FAST) |
		ADC_SMPR2_SMP_AN2(ADC_SAMPLING_FAST) |
		ADC_SMPR2_SMP_AN3(ADC_SAMPLING_FAST) |
		ADC_SMPR2_SMP_AN4(ADC_SAMPLING_FAST) |
		ADC_SMPR2_SMP_AN5(ADC_SAMPLING_FAST) |
		ADC_SMPR2_SMP_AN6(ADC_SAMPLING_FAST) |
		ADC_SMPR2_SMP_AN7(ADC_SAMPLING_FAST) |
		ADC_SMPR2_SMP_AN8(ADC_SAMPLING_FAST) |
		ADC_SMPR2_SMP_AN9(ADC_SAMPLING_FAST),
	.htr				= 0,
	.ltr				= 0,
	.sqr1				= 0, // Conversion group sequence 13...16 + sequence length
	.sqr2				= 0, // Conversion group sequence 7...12
	.sqr3				= 0, // Conversion group sequence 1...6
#if ADC_MAX_CHANNELS_COUNT > 16
	.sqr4				= 0, // Conversion group sequence 19...24
	.sqr5				= 0  // Conversion group sequence 25...30
#endif /* ADC_MAX_CHANNELS_COUNT */
};

static volatile NO_CACHE adcsample_t fastAdcSampleBuf[ADC_BUF_DEPTH_FAST * ADC_MAX_CHANNELS_COUNT];

AdcDevice fastAdc(&ADC_FAST_DEVICE, &adcgrpcfgFast, fastAdcSampleBuf, ADC_BUF_DEPTH_FAST);

static efitick_t lastTick = 0;

#if defined(STM32F4XX) || defined(STM32F7XX)
static FastAdcDiagnostics fastAdcDiagnostics;

// Read-only snapshot: never acknowledge DMA flags here, the HAL owns them.
static void captureFastAdcNotReady(ADCDriver* adcp) {
	if (adcp->state != ADC_ACTIVE || adcp->dmastp == nullptr) {
		fastAdcDiagnostics.record(false, 0, false, false, false);
		return;
	}
	const auto* stream = adcp->dmastp;
	const auto remaining = dmaStreamGetTransactionSize(stream);
	const bool enabled = (stream->stream->CR & STM32_DMA_CR_EN) != 0;
	const auto* dma = stream->selfindex < 8 ? DMA1 : DMA2;
	const uint32_t status = (stream->selfindex & 4) ? dma->HISR : dma->LISR;
	const uint32_t flags = (status >> stream->shift) & STM32_DMA_ISR_MASK;
	const bool hardwareError = (flags & (STM32_DMA_ISR_TEIF | STM32_DMA_ISR_DMEIF | STM32_DMA_ISR_FEIF))
		|| (adcp->adc->SR & (ADC_SR_OVR | ADC_SR_AWD));
	fastAdcDiagnostics.record(adcp->state == ADC_ACTIVE, remaining,
		(flags & STM32_DMA_ISR_TCIF) != 0, enabled, hardwareError);
}
#endif

void AdcDevice::reportErrors() {
#if defined(STM32F4XX) || defined(STM32F7XX)
	uint8_t pending;
	{
		chibios_rt::CriticalSectionLocker csl;
		pending = fastAdcDiagnostics.takePending();
	}

	// Keep warning formatting and publication out of the 10 kHz interrupt.
	if (pending & FastAdcDiagnostics::CompletionPending) {
		warning(ObdCode::CUSTOM_FAST_ADC_DMA_COMPLETION_PENDING, "Fast ADC DMA complete, callback pending");
	}
	if (pending & FastAdcDiagnostics::ConversionBusy) {
		warning(ObdCode::CUSTOM_FAST_ADC_CONVERSION_BUSY, "Fast ADC conversion still busy at next start");
	}
	if (pending & FastAdcDiagnostics::Other) {
		warning(ObdCode::CUSTOM_INVALID_ADC, "Fast ADC not ready: unexpected state or hardware fault");
	}
#endif
}

static void fastAdcDoneCB(ADCDriver *adcp) {
	// State may not be complete if we get a callback for "half done"
	if (adcIsBufferComplete(adcp)) {
		efitick_t nowTick = getTimeNowNt();
		efitick_t diff = nowTick - lastTick;
		lastTick = nowTick;

		engine->outputChannels.fastAdcPeriod = (uint32_t)diff;
		engine->outputChannels.fastAdcConversionCount++;

		onFastAdcComplete(adcp->samples);
	}

	assertInterruptPriority(__func__, EFI_IRQ_ADC_PRIORITY);
}

static void fastAdcErrorCB(ADCDriver *, adcerror_t err) {
	engine->outputChannels.fastAdcLastError = (uint8_t)err;
	engine->outputChannels.fastAdcErrorCount++;
	if (err == ADC_ERR_OVERFLOW) {
		engine->outputChannels.fastAdcOverrunCount++;
	}
	// TODO: restart?
}

#if defined(EFI_INTERNAL_FAST_ADC_PWM)

static const PWMConfig pwmcfg = {
	/* on each trigger event regular group of channels is converted,
	 * to get whole buffer filled we need ADC_BUF_DEPTH_FAST trigger events */
	.frequency = GPT_FREQ_FAST * ADC_BUF_DEPTH_FAST,
	.period = GPT_PERIOD_FAST,
	.callback = nullptr,
	.channels = {
		{PWM_OUTPUT_ACTIVE_HIGH, nullptr},
		{PWM_OUTPUT_ACTIVE_HIGH, nullptr},
		{PWM_OUTPUT_ACTIVE_HIGH, nullptr},
		{PWM_OUTPUT_ACTIVE_HIGH, nullptr}
	},
	.cr2 = 0,
	.bdtr = 0,
	.dier = 0,
};

#elif defined (EFI_INTERNAL_FAST_ADC_GPT)

static void fastAdcStartTrigger(GPTDriver*)
{
#if EFI_INTERNAL_ADC
	/*
	 * Starts an asynchronous ADC conversion operation, the conversion
	 * will be executed in parallel to the current PWM cycle and will
	 * terminate before the next PWM cycle.
	 */
	fastAdc.startConversionI();
#endif /* EFI_INTERNAL_ADC */
	assertInterruptPriority(__func__, EFI_IRQ_ADC_PRIORITY);
}

static const GPTConfig fast_adc_config = {
	.frequency = GPT_FREQ_FAST,
	.callback = fastAdcStartTrigger,
	.cr2 = 0,
	.dier = 0,
};

#else
	#error Please define EFI_INTERNAL_FAST_ADC_PWM or EFI_INTERNAL_FAST_ADC_GPT for Fast ADC
#endif

int AdcDevice::size() const {
	return channelCount;
}

void AdcDevice::init(void) {
	hwConfig->num_channels = size();
	/* driver does this internally */
	//hwConfig->sqr1 += ADC_SQR1_NUM_CH(size());

#if defined(EFI_INTERNAL_FAST_ADC_PWM)
	// Start the timer running
	pwmStart(EFI_INTERNAL_FAST_ADC_PWM, &pwmcfg);
	pwmEnableChannel(EFI_INTERNAL_FAST_ADC_PWM, 0, /* width */ 1);
	adcStartConversion(adcp, hwConfig, (adcsample_t *)samples, depth);
#elif defined (EFI_INTERNAL_FAST_ADC_GPT)
	gptStart(EFI_INTERNAL_FAST_ADC_GPT, &fast_adc_config);
	gptStartContinuous(EFI_INTERNAL_FAST_ADC_GPT, GPT_PERIOD_FAST);
#endif
}

int AdcDevice::enableChannel(adc_channel_e hwChannel) {
	if ((channelCount + 1) >= ADC_MAX_CHANNELS_COUNT) {
		criticalError("Too many ADC channels configured");
		return -1;
	}

	int logicChannel = channelCount++;

	/* TODO: following is correct for STM32 ADC1/2.
	 * ADC3 has another input to gpio mapping
	 * and should be handled separately */
	size_t channelAdcIndex = hwChannel - EFI_ADC_0;

	internalAdcIndexByHardwareIndex[hwChannel] = logicChannel;
	if (logicChannel < 6) {
		hwConfig->sqr3 |= channelAdcIndex << (5 * logicChannel);
	} else if (logicChannel < 12) {
		hwConfig->sqr2 |= channelAdcIndex << (5 * (logicChannel - 6));
	} else if (logicChannel < 18) {
		hwConfig->sqr1 |= channelAdcIndex << (5 * (logicChannel - 12));
	}
#if ADC_MAX_CHANNELS_COUNT > 16
	else if (logicChannel < 24) {
		hwConfig->sqr4 |= channelAdcIndex << (5 * (logicChannel - 18));
	}
	else if (logicChannel < 30) {
		hwConfig->sqr5 |= channelAdcIndex << (5 * (logicChannel - 24));
	}
#endif /* ADC_MAX_CHANNELS_COUNT */

	return channelAdcIndex;
}

void AdcDevice::startConversionI()
{
	chSysLockFromISR();
	if ((ADC_FAST_DEVICE.state == ADC_READY) ||
		(ADC_FAST_DEVICE.state == ADC_ERROR)) {
		/* drop volatile type qualifier - this is safe */
		adcStartConversionI(adcp, hwConfig, (adcsample_t *)samples, depth);
	} else {
		engine->outputChannels.fastAdcErrorCount++;
#if defined(STM32F4XX) || defined(STM32F7XX)
		// Distinguish the two timing paths in https://github.com/rusefi/rusefi/issues/6399.
		captureFastAdcNotReady(adcp);
#endif
	}
	chSysUnlockFromISR();
}

adcsample_t AdcDevice::getAvgAdcValue(adc_channel_e hwChannel) {
	uint32_t result = 0;
	int numChannels = size();
	int index = fastAdc.internalAdcIndexByHardwareIndex[hwChannel];
	if (index == 0xff) {
		criticalError("Fast ADC attempt to read unconfigured input %d.", hwChannel);
		return 0;
	}

	for (size_t i = 0; i < depth; i++) {
		adcsample_t sample = samples[index];
		if (sample > ADC_MAX_VALUE) {
		  // 12bit ADC expected right now. An error here usually means major RAM corruption?
 			criticalError("ADC unexpected sample %d at %ld uptime.",
				sample,
				(uint32_t)getTimeNowS());
		}
		result += sample;
		index += numChannels;
	}

	// this truncation is guaranteed to not be lossy - the average can't be larger than adcsample_t
	return static_cast<adcsample_t>(result / depth);
}

adc_channel_e AdcDevice::getAdcChannelByInternalIndex(int hwChannel) const {
	for (size_t idx = EFI_ADC_0; idx < EFI_ADC_TOTAL_CHANNELS; idx++) {
		if (internalAdcIndexByHardwareIndex[idx] == hwChannel) {
			return (adc_channel_e)idx;
		}
	}
	return EFI_ADC_NONE;
}

AdcToken AdcDevice::getAdcChannelToken(adc_channel_e hwChannel) {
	return fastAdc.internalAdcIndexByHardwareIndex[hwChannel];
}

#endif // EFI_USE_FAST_ADC

#endif // HAL_USE_ADC
