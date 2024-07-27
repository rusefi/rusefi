/**
 * @file	adc_inputs_onchip.cpp
 * @brief	Low level ADC code
 *
 * rusEfi uses two ADC devices on the same 16 pins at the moment. Two ADC devices are used in order to distinguish between
 * fast and slow devices. The idea is that but only having few channels in 'fast' mode we can sample those faster?
 *
 * At the moment rusEfi does not allow to have more than 16 ADC channels combined. At the moment there is no flexibility to use
 * any ADC pins, only the hardcoded choice of 16 pins.
 *
 * Slow ADC group is used for IAT, CLT, AFR, VBATT etc - this one is currently sampled at 500Hz
 *
 * Fast ADC group is used for MAP, MAF HIP - this one is currently sampled at 10KHz
 *  We need frequent MAP for map_averaging.cpp
 *
 * 10KHz equals one measurement every 3.6 degrees at 6000 RPM
 *
 * @date Jan 14, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#if HAL_USE_ADC

#include "adc_subscription.h"
#include "AdcDevice.h"
#include "mpu_util.h"
#include "periodic_thread_controller.h"
#include "protected_gpio.h"

// Board voltage, with divider coefficient accounted for
float getVoltageDivided(const char *msg, adc_channel_e hwChannel) {
	return getVoltage(msg, hwChannel) * getAnalogInputDividerCoefficient(hwChannel);
}

float PUBLIC_API_WEAK boardAdjustVoltage(float voltage, adc_channel_e hwChannel) {
  // a hack useful when we do not trust voltage just after board EN was turned on. is this just hiding electrical design flaws?
  return voltage;
}

// voltage in MCU universe, from zero to VDD
float getVoltage(const char *msg, adc_channel_e hwChannel) {
	float voltage = adcToVolts(getAdcValue(msg, hwChannel));
	return boardAdjustVoltage(voltage, hwChannel);
}

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
	.circular			= FALSE,
	.num_channels		= 0,
	.end_cb				= fastAdcDoneCB,
	.error_cb			= fastAdcErrorCB,
	/* HW dependent part.*/
	.cr1				= 0,
	.cr2				= ADC_CR2_SWSTART,
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

static void fastAdcDoneCB(ADCDriver *adcp) {
	// State may not be complete if we get a callback for "half done"
	if (adcp->state == ADC_COMPLETE) {
		fastAdc.conversionCount++;
		onFastAdcComplete(adcp->samples);
	}
}

static volatile adcerror_t fastAdcLastError;

static void fastAdcErrorCB(ADCDriver *, adcerror_t err) {
	fastAdcLastError = err;
	engine->outputChannels.fastAdcErrorCallbackCount++;
}

static void fastAdcTrigger(GPTDriver*) {
#if EFI_INTERNAL_ADC
	/*
	 * Starts an asynchronous ADC conversion operation, the conversion
	 * will be executed in parallel to the current PWM cycle and will
	 * terminate before the next PWM cycle.
	 */
	fastAdc.startConversionI();
#endif /* EFI_INTERNAL_ADC */
}

static GPTConfig fast_adc_config = {
	.frequency = GPT_FREQ_FAST,
	.callback = fastAdcTrigger,
	.cr2 = 0,
	.dier = 0,
};

int AdcDevice::size() const {
	return channelCount;
}

void AdcDevice::init(void) {
	hwConfig->num_channels = size();
	/* driver does this internally */
	//hwConfig->sqr1 += ADC_SQR1_NUM_CH(size());

	gptStart(EFI_INTERNAL_FAST_ADC_GPT, &fast_adc_config);
	gptStartContinuous(EFI_INTERNAL_FAST_ADC_GPT, GPT_PERIOD_FAST);
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
	if ((ADC_FAST_DEVICE.state != ADC_READY) &&
		(ADC_FAST_DEVICE.state != ADC_COMPLETE) &&
		(ADC_FAST_DEVICE.state != ADC_ERROR)) {
		engine->outputChannels.fastAdcErrorsCount++;
		// todo: when? why? criticalError("ADC fast not ready?");
		// see notes at https://github.com/rusefi/rusefi/issues/6399
	} else {
		/* drop volatile type qualifier - this is safe */
		adcStartConversionI(adcp, hwConfig, (adcsample_t *)samples, depth);
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
//		if (sample > 0x1FFF) {
//			// 12bit ADC expected right now, make this configurable one day
//			criticalError("fast ADC unexpected sample %d", sample);
//		} else
		if (sample > ADC_MAX_VALUE) {
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

#endif
