/**
 * @file	adc_inputs_onchip_slow.cpp
 * @brief	Low level ADC code
 *
 * @date Aug 22, 2025
 * @author Andrey Belomutskiy, (c) 2012-2025
 */

#include "pch.h"

#if HAL_USE_ADC

#include "adc_device.h"

#ifndef SLOW_ADC_CHANNEL_COUNT
#ifdef ADC_MUX_PIN
#define SLOW_ADC_CHANNEL_COUNT 32
#else // not ADC_MUX_PIN
#define SLOW_ADC_CHANNEL_COUNT 16
#endif // def ADC_MUX_PIN
#endif // SLOW_ADC_CHANNEL_COUNT

/* Optional ADC3 slow sampling: on boards whose analog inputs live on
 * ADC3-only pins (EFI_ADC_32..39, e.g. F-port pins on STM32F4/AT32F435)
 * the v2 port fills channels 32..39 into the upper part of the buffer.
 * The buffer is indexed by hwChannel - EFI_ADC_0, so with ADC3 sampling
 * enabled it must cover indices up to 39. */
#ifndef SLOW_ADC3_CHANNEL_COUNT
#if EFI_ADC3_SLOW
#define SLOW_ADC3_CHANNEL_COUNT 8
#define SLOW_ADC_SAMPLES_TOTAL 40
#else
#define SLOW_ADC3_CHANNEL_COUNT 0
#define SLOW_ADC_SAMPLES_TOTAL SLOW_ADC_CHANNEL_COUNT
#endif
#endif // SLOW_ADC3_CHANNEL_COUNT

/* TODO: Drop NO_CACHE for F4 and F7 couse with ADCv2 driver CPU does averaging and CPU stores result to this array */
/* TODO: store summ of samples is this array and divide on oversample factor only when converting to float - this will increase accuracity */
static volatile NO_CACHE adcsample_t slowAdcSamples[SLOW_ADC_SAMPLES_TOTAL];

static uint32_t slowAdcConversionCount = 0;

static float mcuTemperature;
static float mcuVrefVoltage;

void adcOnchipSlowUpdate(efitick_t nowNt) {
	UNUSED(nowNt);

	ScopePerf perf(PE::AdcConversionSlow);

	/* drop volatile type qualifier - this is safe */
	if (!readSlowAnalogInputs((adcsample_t *)slowAdcSamples)) {
		engine->outputChannels.slowAdcErrorCount++;
		return;
	}

	// TODO:
	//engine->outputChannels.slowAdcConversionCount++;
	slowAdcConversionCount++;

	// Ask the port to sample the MCU temperature
	mcuTemperature = getMcuTemperature();
	if (mcuTemperature > 150.0f || mcuTemperature < -50.0f) {
		/*
		 * we have a sporadic issue with this check todo https://github.com/rusefi/rusefi/issues/2552
		 */
		//criticalError("Invalid CPU temperature measured %f", degrees);
	}

	mcuVrefVoltage = getMcuVrefVoltage();
}

adcsample_t adcOnchipSlowGetAvgRaw(adc_channel_e hwChannel)
{
	return slowAdcSamples[hwChannel - EFI_ADC_0];
}

extern void adcPrintChannelReport(const char *prefix, int internalIndex, adc_channel_e hwChannel);

void adcOnchipSlowShowReport()
{
	efiPrintf("slow %lu samples", slowAdcConversionCount);

	/* we assume that all slow ADC channels are enabled */
	for (int internalIndex = 0; internalIndex < SLOW_ADC_CHANNEL_COUNT; internalIndex++) {
		adc_channel_e hwChannel = static_cast<adc_channel_e>(internalIndex + EFI_ADC_0);

		adcPrintChannelReport("S", internalIndex, hwChannel);
	}

#if SLOW_ADC3_CHANNEL_COUNT > 0
	for (int internalIndex = 0; internalIndex < SLOW_ADC3_CHANNEL_COUNT; internalIndex++) {
		adc_channel_e hwChannel = static_cast<adc_channel_e>(internalIndex + EFI_ADC_32);

		adcPrintChannelReport("S", internalIndex + EFI_ADC_32, hwChannel);
	}
#endif
}

float getMCUInternalTemperature() {
	return mcuTemperature;
}

float getMCUVref() {
	return mcuVrefVoltage;
}

/* TODO: kill this */
void waitForSlowAdc(uint32_t lastAdcCounter) {
	// note that having ADC reading is one thing while having new sensor API is a totally different thing!
	// todo: use sync.objects?
	while (slowAdcConversionCount <= lastAdcCounter) {
		chThdSleepMilliseconds(1);
	}
}

#endif // HAL_USE_ADC
