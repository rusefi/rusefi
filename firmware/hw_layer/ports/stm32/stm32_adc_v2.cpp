/**
 * @file        stm32_adc_v2.cpp
 * @brief       Port implementation for the STM32 "v2" ADC found on the STM32F4 and STM32F7
 *
 * @date February 9, 2021
 * @author Matthew Kennedy, (c) 2021
 */

#include "pch.h"

#ifdef EFI_SOFTWARE_KNOCK
#include "knock_config.h"
#endif

#if HAL_USE_ADC

/* HW channels count per ADC */
constexpr size_t adcChannelCount = 16;
constexpr size_t adcAux1ChannelCount = 2;
constexpr size_t adcAux2ChannelCount = 1;

/* Depth of the conversion buffer, channels are sampled X times each.*/
#define SLOW_ADC_OVERSAMPLE      8

#ifndef EFI_INTERNAL_SLOW_ADC_BACKGROUND
#define EFI_INTERNAL_SLOW_ADC_BACKGROUND FALSE
#endif

#ifdef ADC_MUX_PIN
// https://github.com/rusefi/alphax-4chan is the reference board with ADC mux
static OutputPin muxControl;
#ifndef ADC_MUX_PIN_INVERTED
#define ADC_MUX_PIN_INVERTED 0
#endif
#define ADC_MUX_PRIMARY_VALUE (ADC_MUX_PIN_INVERTED ? 1 : 0)
#define ADC_MUX_MUXED_VALUE   (ADC_MUX_PIN_INVERTED ? 0 : 1)
#endif // ADC_MUX_PIN

#if (EFI_INTERNAL_SLOW_ADC_BACKGROUND == TRUE)
static void slowAdcEndCB(ADCDriver *adcp);
#endif
static void slowAdcErrorCB(ADCDriver *, adcerror_t);

/*
 * ADC conversion group.
 */
static const ADCConversionGroup aux1ConvGroup = {
	.circular			= FALSE,
	.num_channels		= adcAux1ChannelCount,
#if (EFI_INTERNAL_SLOW_ADC_BACKGROUND == TRUE)
	.end_cb				= slowAdcEndCB,
#else
	.end_cb				= nullptr,
#endif
	.error_cb			= slowAdcErrorCB,
	/* HW dependent part below */
	.cr1				= 0,
	.cr2				= ADC_CR2_SWSTART,
	// sample times for channels 10...18
	.smpr1 =
		ADC_SMPR1_SMP_VBAT(ADC_SAMPLE_144) |	/* input18 - temperature and vbat input on some STM32F7xx */
		ADC_SMPR1_SMP_SENSOR(ADC_SAMPLE_144) |	/* input16 - temperature sensor input on STM32F4xx */
		ADC_SMPR1_SMP_VREF(ADC_SAMPLE_144),		/* input17 - Vrefint input */
	.smpr2 = 0,
	.htr = 0, .ltr = 0,
	.sqr1 = 0,
	.sqr2 = 0,
	.sqr3 =
#if defined(STM32F4XX)
		ADC_SQR3_SQ1_N(16) |
#endif
#if defined(STM32F7XX)
		ADC_SQR3_SQ1_N(18) |
#endif
		ADC_SQR3_SQ2_N(17),
};

static const ADCConversionGroup aux2ConvGroup = {
	.circular			= FALSE,
	.num_channels		= adcAux2ChannelCount,
#if (EFI_INTERNAL_SLOW_ADC_BACKGROUND == TRUE)
	.end_cb				= slowAdcEndCB,
#else
	.end_cb				= nullptr,
#endif
	.error_cb			= slowAdcErrorCB,
	/* HW dependent part below */
	.cr1				= 0,
	.cr2				= ADC_CR2_SWSTART,
	// sample times for channels 10...18
	.smpr1 =
		ADC_SMPR1_SMP_VBAT(ADC_SAMPLE_144),		/* input18 - vbat input on STM32F4xx and STM32F7xx */
	.smpr2 = 0,
	.htr = 0, .ltr = 0,
	.sqr1 = 0,
	.sqr2 = 0,
	.sqr3 =
		ADC_SQR3_SQ1_N(18),
};
// 4x oversample is plenty
static constexpr int auxSensorOversample = 4;
static volatile NO_CACHE adcsample_t aux1SensorSamples[adcAux1ChannelCount * auxSensorOversample];
static volatile NO_CACHE adcsample_t aux2SensorSamples[adcAux2ChannelCount * auxSensorOversample];

float getMcuTemperature() {
	uint32_t sum = 0;
	for (size_t i = 0; i < auxSensorOversample; i++) {
		sum += aux1SensorSamples[0 + adcAux1ChannelCount * i];
	}

	float volts = (float)sum / (ADC_MAX_VALUE * auxSensorOversample);
	volts *= engineConfiguration->adcVcc;

	volts -= 0.760f; // Subtract the reference voltage at 25 deg C
	float degrees = volts / 0.0025f; // Divide by slope 2.5mV

	degrees += 25.0; // Add the 25 deg C

	return degrees;
}

float getMcuVrefVoltage() {
	uint32_t sum = 0;
	for (size_t i = 0; i < auxSensorOversample; i++) {
		sum += aux1SensorSamples[1 + adcAux1ChannelCount * i];
	}

	// TODO: apply calibration value from OTP (if exists)
	// vrefint should be 1.21V
	// Let's calculate external Vref+
	// sum / (ADC_MAX_VALUE * auxSensorOversample) * Vref+ = 1.21;
	float Vref = 1.21f * auxSensorOversample * ADC_MAX_VALUE / sum;

	return Vref;
}

float getMcuVbatVoltage() {
	uint32_t sum = 0;
	for (size_t i = 0; i < auxSensorOversample; i++) {
		sum += aux2SensorSamples[0 + adcAux2ChannelCount * i];
	}

#if defined(STM32F4XX)
	// VBAT/2 on STM32F40xx and STM32F41xx devices, VBAT/4 on STM32F42xx and STM32F43xx devices
	int mult = 2;
	if (isStm32F42x()) {
		mult = 4;
	}
#endif
#if defined(STM32F7XX)
	int mult = 4;
#endif

	float Vbat = (float)sum * mult / (ADC_MAX_VALUE * auxSensorOversample);
	Vbat *= engineConfiguration->adcVcc;

	return Vbat;
}

// See https://github.com/rusefi/rusefi/issues/976 for discussion on these values
// ...  there is no reason to use a longer sampling time than 56 cycles with the current clock ...
#ifndef ADC_SAMPLING_SLOW
#define ADC_SAMPLING_SLOW ADC_SAMPLE_56
#endif
// see also ADC_SAMPLING_FAST in adc_inputs.cpp
// ADC clock is 21MHz on F4 and 27MHz on F7
// We want 500 Hz refresh rate for 16 (32) channels + MCU temperature
// 21 MHz / 500 = 42000 clocks for all channels including oversampling
// We want SLOW_ADC_OVERSAMPLE
// 42000 / 8 / 16 = 328.125 clocks / channel
// 42000 / 8 / 32 = 164 clocks / channel
// This ^ does not include additional MCU temperatur conversions

#ifndef ADC1_SLOW_MUXED
#if defined(ADC_MUX_PIN)
#define ADC1_SLOW_MUXED 1
#else
#define ADC1_SLOW_MUXED 0
#endif
#endif

#ifndef ADC3_SLOW_MUXED
#if defined(ADC_MUX_PIN) && defined(ADC3_SLOW_CHANNEL_COUNT)
#define ADC3_SLOW_MUXED 1
#else
#define ADC3_SLOW_MUXED 0
#endif
#endif

// Slow ADC has 16 channels we can sample, or 32 if ADC mux mode is enabled.
static volatile NO_CACHE adcsample_t slowSampleBuffer[SLOW_ADC_OVERSAMPLE * adcChannelCount];
#if ADC1_SLOW_MUXED
static volatile NO_CACHE adcsample_t slowSampleBufferMuxed[SLOW_ADC_OVERSAMPLE * adcChannelCount];
#endif

#ifdef ADC3_SLOW_CHANNEL_COUNT
// ADC3-only channels: PF6(IN4)=EFI_ADC_32, PF7(IN5)=_33, PF8(IN6)=_34, PF9(IN7)=_35,
//                     PF10(IN8)=_36, PF3(IN9)=_37, PF4(IN14)=_38, PF5(IN15)=_39
// mux=1 counterparts land at EFI_ADC_40-47
static_assert(ADC3_SLOW_CHANNEL_COUNT == 8, "convGroupSlowAdc3 hardcodes exactly 8 channels (AN4-AN9, AN14, AN15)");
static volatile NO_CACHE adcsample_t slowSampleBufferAdc3[SLOW_ADC_OVERSAMPLE * ADC3_SLOW_CHANNEL_COUNT];
#if ADC3_SLOW_MUXED
static volatile NO_CACHE adcsample_t slowSampleBufferAdc3Muxed[SLOW_ADC_OVERSAMPLE * ADC3_SLOW_CHANNEL_COUNT];
#endif

static const ADCConversionGroup convGroupSlowAdc3 = {
	.circular        = FALSE,
	.num_channels    = ADC3_SLOW_CHANNEL_COUNT,
#if (EFI_INTERNAL_SLOW_ADC_BACKGROUND == TRUE)
	.end_cb          = slowAdcEndCB,
#else
	.end_cb          = nullptr,
#endif
	.error_cb        = slowAdcErrorCB,
	.cr1             = 0,
	.cr2             = ADC_CR2_SWSTART,
	.smpr1           =
		ADC_SMPR1_SMP_AN14(ADC_SAMPLING_SLOW) |  // PF4
		ADC_SMPR1_SMP_AN15(ADC_SAMPLING_SLOW),    // PF5
	.smpr2           =
		ADC_SMPR2_SMP_AN4(ADC_SAMPLING_SLOW) |    // PF6
		ADC_SMPR2_SMP_AN5(ADC_SAMPLING_SLOW) |    // PF7
		ADC_SMPR2_SMP_AN6(ADC_SAMPLING_SLOW) |    // PF8
		ADC_SMPR2_SMP_AN7(ADC_SAMPLING_SLOW) |    // PF9
		ADC_SMPR2_SMP_AN8(ADC_SAMPLING_SLOW) |    // PF10
		ADC_SMPR2_SMP_AN9(ADC_SAMPLING_SLOW),     // PF3
	.htr = 0, .ltr = 0,
	// Positions 1-6 in SQR3, positions 7-8 in SQR2
	.sqr1 = 0,
	.sqr2 = ADC_SQR2_SQ7_N(14) | ADC_SQR2_SQ8_N(15),
	.sqr3 =
		ADC_SQR3_SQ1_N(4) | ADC_SQR3_SQ2_N(5) | ADC_SQR3_SQ3_N(6) |
		ADC_SQR3_SQ4_N(7) | ADC_SQR3_SQ5_N(8) | ADC_SQR3_SQ6_N(9),
};

static bool readBatchAdc3(adcsample_t* convertedSamples, adcsample_t* b) {
#if (EFI_INTERNAL_SLOW_ADC_BACKGROUND == FALSE)
	msg_t result = adcConvert(&ADCD3, &convGroupSlowAdc3, b, SLOW_ADC_OVERSAMPLE);
	if (result != MSG_OK) {
		return false;
	}
#endif
	for (size_t i = 0; i < ADC3_SLOW_CHANNEL_COUNT; i++) {
		uint32_t sum = 0;
		size_t index = i;
		for (size_t j = 0; j < SLOW_ADC_OVERSAMPLE; j++) {
			sum += b[index];
			index += ADC3_SLOW_CHANNEL_COUNT;
		}
		convertedSamples[i] = static_cast<adcsample_t>(sum / SLOW_ADC_OVERSAMPLE);
	}
	return true;
}
#endif // ADC3_SLOW_CHANNEL_COUNT

static void slowAdcErrorCB(ADCDriver *, adcerror_t err) {
	engine->outputChannels.slowAdcErrorCount++;
	if (err == ADC_ERR_OVERFLOW) {
		engine->outputChannels.slowAdcOverrunCount++;
	}
	// TODO: restart?
}

// Conversion group for slow channels
// This simply samples every channel in sequence
static /* constexpr */ ADCConversionGroup convGroupSlow = {
	.circular			= FALSE,
	.num_channels		= adcChannelCount,
#if (EFI_INTERNAL_SLOW_ADC_BACKGROUND == TRUE)
	.end_cb				= slowAdcEndCB,
#else
	.end_cb				= nullptr,
#endif
	.error_cb			= slowAdcErrorCB,
	/* HW dependent part.*/
	.cr1				= 0,
	.cr2				= ADC_CR2_SWSTART,
	// Configure all channels to ADC_SAMPLING_SLOW sample time
	.smpr1 =
		ADC_SMPR1_SMP_AN10(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN11(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN12(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN13(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN14(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN15(ADC_SAMPLING_SLOW),
	.smpr2 =
		ADC_SMPR2_SMP_AN0(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN1(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN2(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN3(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN4(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN5(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN6(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN7(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN8(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN9(ADC_SAMPLING_SLOW),
	.htr	= 0,
	.ltr	= 0,
	// Simply sequence every channel in order
	.sqr1	= ADC_SQR1_SQ13_N(12) | ADC_SQR1_SQ14_N(13) | ADC_SQR1_SQ15_N(14) | ADC_SQR1_SQ16_N(15), // Conversion group sequence 13...16
	.sqr2	=   ADC_SQR2_SQ7_N(6) |   ADC_SQR2_SQ8_N(7) |   ADC_SQR2_SQ9_N(8) | ADC_SQR2_SQ10_N(9) | ADC_SQR2_SQ11_N(10) | ADC_SQR2_SQ12_N(11), // Conversion group sequence 7...12
	.sqr3	=   ADC_SQR3_SQ1_N(0) |   ADC_SQR3_SQ2_N(1) |   ADC_SQR3_SQ3_N(2) |  ADC_SQR3_SQ4_N(3) |   ADC_SQR3_SQ5_N(4) |   ADC_SQR3_SQ6_N(5), // Conversion group sequence 1...6
};

#if (EFI_INTERNAL_SLOW_ADC_BACKGROUND == TRUE)

typedef enum {
	convertPrimary,
#ifdef ADC3_SLOW_CHANNEL_COUNT
	convertAdc3Primary,
#endif
#if ADC1_SLOW_MUXED
	convertMuxed,
#endif
#if ADC3_SLOW_MUXED
	convertAdc3Muxed,
#endif
	convertAux,
	convertAux2,
} slowAdcState_t;

static slowAdcState_t slowAdcGetNextState(slowAdcState_t state)
{
	switch (state) {
	case convertPrimary:
#ifdef ADC3_SLOW_CHANNEL_COUNT
		return convertAdc3Primary;
#elif ADC1_SLOW_MUXED
		return convertMuxed;
#elif ADC3_SLOW_MUXED
		return convertAdc3Muxed;
#else
		return convertAux;
#endif
#ifdef ADC3_SLOW_CHANNEL_COUNT
	case convertAdc3Primary:
#if ADC1_SLOW_MUXED
		return convertMuxed;
#elif ADC3_SLOW_MUXED
		return convertAdc3Muxed;
#else
		return convertAux;
#endif
#endif
#if ADC1_SLOW_MUXED
	case convertMuxed:
#if ADC3_SLOW_MUXED
		return convertAdc3Muxed;
#else
		return convertAux;
#endif
#endif
#if ADC3_SLOW_MUXED
	case convertAdc3Muxed:
		return convertAux;
#endif
	case convertAux:
		return convertAux2;
	case convertAux2:
		return convertPrimary;
	}
	return convertPrimary;
}

static slowAdcState_t slowAdcState = convertPrimary;

static void slowAdcEndCB(ADCDriver *adcp) {
	if (adcIsBufferComplete(adcp)) {
		chSysLockFromISR();
		// Switch state to ready to allow starting new conversion from here
		adcp->state = ADC_READY;
		// get next state
		slowAdcState = slowAdcGetNextState(slowAdcState);
		switch (slowAdcState) {
		case convertPrimary:
			#ifdef ADC_MUX_PIN
			muxControl.setValue(ADC_MUX_PRIMARY_VALUE, /*force*/true);
			#endif
			adcStartConversionI(&ADCD1, &convGroupSlow, (adcsample_t *)slowSampleBuffer, SLOW_ADC_OVERSAMPLE);
			break;
		#ifdef ADC3_SLOW_CHANNEL_COUNT
		case convertAdc3Primary:
			// mux already 0; start ADC3 primary
			adcStartConversionI(&ADCD3, &convGroupSlowAdc3, (adcsample_t *)slowSampleBufferAdc3, SLOW_ADC_OVERSAMPLE);
			break;
		#endif
		#if ADC1_SLOW_MUXED
		case convertMuxed:
			muxControl.setValue(ADC_MUX_MUXED_VALUE, /*force*/true);
			adcStartConversionI(&ADCD1, &convGroupSlow, (adcsample_t *)slowSampleBufferMuxed, SLOW_ADC_OVERSAMPLE);
			break;
		#endif
		#if ADC3_SLOW_MUXED
		case convertAdc3Muxed:
			#if (ADC1_SLOW_MUXED == 0)
			// If ADC1 wasn't muxed, the mux pin hasn't been set high yet
			muxControl.setValue(ADC_MUX_MUXED_VALUE, /*force*/true);
			#endif
			adcStartConversionI(&ADCD3, &convGroupSlowAdc3, (adcsample_t *)slowSampleBufferAdc3Muxed, SLOW_ADC_OVERSAMPLE);
			break;
		#endif
		case convertAux:
			adcSTM32DisableVBATE();
			adcStartConversionI(&ADCD1, &aux1ConvGroup, (adcsample_t *)aux1SensorSamples, auxSensorOversample);
			break;
		case convertAux2:
			adcSTM32EnableVBATE();
			adcStartConversionI(&ADCD1, &aux2ConvGroup, (adcsample_t *)aux2SensorSamples, auxSensorOversample);
			break;
		}
		chSysUnlockFromISR();
	}
}
#endif // EFI_INTERNAL_SLOW_ADC_BACKGROUND

static bool readBatch(adcsample_t* convertedSamples, adcsample_t* b) {
#if (EFI_INTERNAL_SLOW_ADC_BACKGROUND == FALSE)
	msg_t result = adcConvert(&ADCD1, &convGroupSlow, b, SLOW_ADC_OVERSAMPLE);

	// If something went wrong - try again later
	if (result != MSG_OK) {
		return false;
	}

	// Temperature sensor is only physically wired to ADC1
	adcConvert(&ADCD1, &aux1ConvGroup, (adcsample_t *)aux1SensorSamples, auxSensorOversample);

	// Switch IN18 input to Vbat
	adcSTM32EnableVBATE();
	adcConvert(&ADCD1, &aux2ConvGroup, (adcsample_t *)aux2SensorSamples, auxSensorOversample);
	adcSTM32DisableVBATE();
#endif

	// Average samples to get some noise filtering and oversampling
	for (size_t i = 0; i < adcChannelCount; i++) {
		uint32_t sum = 0;
		size_t index = i;
		for (size_t j = 0; j < SLOW_ADC_OVERSAMPLE; j++) {
			sum += b[index];
			index += adcChannelCount;
		}

		adcsample_t value = static_cast<adcsample_t>(sum / SLOW_ADC_OVERSAMPLE);
		convertedSamples[i] = value;
	}

	return true;
}

bool readSlowAnalogInputs(adcsample_t* convertedSamples) {
	bool result = true;

	// mux=0: ADC1 primary channels (EFI_ADC_0-15)
	result &= readBatch(convertedSamples, (adcsample_t *)slowSampleBuffer);
#ifdef ADC3_SLOW_CHANNEL_COUNT
	// mux=0: ADC3 primary channels (EFI_ADC_32-39)
	result &= readBatchAdc3(&convertedSamples[EFI_ADC_32 - EFI_ADC_0], (adcsample_t *)slowSampleBufferAdc3);
#endif

#if ADC1_SLOW_MUXED
	#if (EFI_INTERNAL_SLOW_ADC_BACKGROUND == FALSE)
		muxControl.setValue(ADC_MUX_MUXED_VALUE, /*force*/true);
	#endif
	// mux=1: ADC1 muxed channels (EFI_ADC_16-31)
	result &= readBatch(&convertedSamples[adcChannelCount], (adcsample_t *)slowSampleBufferMuxed);
#endif

#if ADC3_SLOW_MUXED
	#if (EFI_INTERNAL_SLOW_ADC_BACKGROUND == FALSE)
		#if (ADC1_SLOW_MUXED == 0)
			muxControl.setValue(ADC_MUX_MUXED_VALUE, /*force*/true);
		#endif
	#endif
	// mux=1: ADC3 muxed channels (EFI_ADC_40-47)
	result &= readBatchAdc3(&convertedSamples[EFI_ADC_40 - EFI_ADC_0], (adcsample_t *)slowSampleBufferAdc3Muxed);
#endif

#ifdef ADC_MUX_PIN
	#if (EFI_INTERNAL_SLOW_ADC_BACKGROUND == FALSE)
		muxControl.setValue(ADC_MUX_PRIMARY_VALUE, /*force*/true);
	#endif
#endif

	return result;
}

#if EFI_USE_FAST_ADC

#include "adc_device.h"
#include "adc_onchip.h"

extern AdcDevice fastAdc;

// See: https://github.com/rusefi/rusefi/issues/8445
// We need to disable Slow ADC access to pins that are handled by fast ADC to avoid additional noise
static void slowAdcEnableDisableChannel(adc_channel_e hwChannel, bool en)
{
	if (!isAdcChannelValid(hwChannel)) {
		return;
	}

	/* TODO: following is correct for STM32 ADC1/2.
	 * ADC3 has another input to gpio mapping
	 * and should be handled separately */
	uint32_t channelAdcIndex = hwChannel - EFI_ADC_0;
	// Switch disabled channel to internal Vrefint channel
	adcConversionGroupSetSeqInput(&convGroupSlow, channelAdcIndex, en ? channelAdcIndex : 17);
}

AdcToken enableFastAdcChannel(const char*, adc_channel_e hwChannel) {
	if (!isAdcChannelValid(hwChannel)) {
		return invalidAdcToken;
	}

	// Do not run slow ADC for fast ADC inputs
	slowAdcEnableDisableChannel(hwChannel, false);

	return fastAdc.getAdcChannelToken(hwChannel);
}

adcsample_t getFastAdc(AdcToken token) {
	if (token == invalidAdcToken) {
		return 0;
	}

	return fastAdc.getAdcValueByToken(token);
}

#endif // EFI_USE_FAST_ADC

#ifdef EFI_SOFTWARE_KNOCK

static void knockCompletionCallback(ADCDriver* adcp) {
	if (adcIsBufferComplete(adcp)) {
		onKnockSamplingComplete();
	}

	assertInterruptPriority(__func__, EFI_IRQ_ADC_PRIORITY);
}

static void knockErrorCallback(ADCDriver*, adcerror_t) {
}

static const uint32_t smpr1 =
	ADC_SMPR1_SMP_AN10(KNOCK_SAMPLE_TIME) |
	ADC_SMPR1_SMP_AN11(KNOCK_SAMPLE_TIME) |
	ADC_SMPR1_SMP_AN12(KNOCK_SAMPLE_TIME) |
	ADC_SMPR1_SMP_AN13(KNOCK_SAMPLE_TIME) |
	ADC_SMPR1_SMP_AN14(KNOCK_SAMPLE_TIME) |
	ADC_SMPR1_SMP_AN15(KNOCK_SAMPLE_TIME);

static const uint32_t smpr2 =
	ADC_SMPR2_SMP_AN0(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN1(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN2(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN3(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN4(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN5(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN6(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN7(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN8(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN9(KNOCK_SAMPLE_TIME);

static const ADCConversionGroup adcConvGroupCh1 = {
	.circular = FALSE,
	.num_channels = 1,
	.end_cb = &knockCompletionCallback,
	.error_cb = &knockErrorCallback,
	.cr1 = 0,
	.cr2 = ADC_CR2_SWSTART,
	// sample times for channels 10...18
	.smpr1 = smpr1,
	// sample times for channels 0...9
	.smpr2 = smpr2,

	.htr = 0,
	.ltr = 0,

	.sqr1 = 0,
	.sqr2 = 0,
	.sqr3 = ADC_SQR3_SQ1_N(KNOCK_ADC_CH1)
};

// Not all boards have a second channel - configure it if it exists
#if KNOCK_HAS_CH2
static const ADCConversionGroup adcConvGroupCh2 = {
	.circular = FALSE,
	.num_channels = 1,
	.end_cb = &knockCompletionCallback,
	.error_cb = &knockErrorCallback,
	.cr1 = 0,
	.cr2 = ADC_CR2_SWSTART,
	// sample times for channels 10...18
	.smpr1 = smpr1,
	// sample times for channels 0...9
	.smpr2 = smpr2,

	.htr = 0,
	.ltr = 0,

	.sqr1 = 0,
	.sqr2 = 0,
	.sqr3 = ADC_SQR3_SQ1_N(KNOCK_ADC_CH2)
};
#endif // KNOCK_HAS_CH2

const ADCConversionGroup* getKnockConversionGroup(uint8_t channelIdx) {
#if KNOCK_HAS_CH2
	if (channelIdx == 1) {
		return &adcConvGroupCh2;
	}
#else
	(void)channelIdx;
#endif // KNOCK_HAS_CH2

	return &adcConvGroupCh1;
}

#endif // EFI_SOFTWARE_KNOCK

void portInitAdc() {
#ifdef ADC_MUX_PIN
	muxControl.initPin("ADC Mux", ADC_MUX_PIN);
#endif //ADC_MUX_PIN

	// Init slow ADC
	adcStart(&ADCD1, NULL);

	// Enable internal temperature reference
	adcSTM32EnableTSVREFE(); // Internal temperature sensor

#if (EFI_INTERNAL_SLOW_ADC_BACKGROUND == TRUE)
	adcStartConversion(&ADCD1, &convGroupSlow, (adcsample_t *)slowSampleBuffer, SLOW_ADC_OVERSAMPLE);
#endif

#if EFI_USE_FAST_ADC
	// Init fast ADC (MAP sensor)
	adcStart(&ADCD2, NULL);
#endif

#if defined(STM32F7XX)
	/* the temperature sensor is internally
	 * connected to the same input channel as VBAT. Only one conversion,
	 * temperature sensor or VBAT, must be selected at a time. */
	adcSTM32DisableVBATE();
#endif

	/* Enable this code only when you absolutly sure
	 * that there is no possible errors from ADC */
#if 0
	/* All ADC use DMA and DMA calls end_cb from its IRQ
	 * If none of ADC users need error callback - we can disable
	 * shared ADC IRQ and save some CPU ticks */
	if ((adcgrpcfgSlow.error_cb == NULL) &&
		(adcgrpcfgFast.error_cb == NULL)
		/* TODO: Add ADC3? */) {
		nvicDisableVector(STM32_ADC_NUMBER);
	}
#endif

#ifdef EFI_SOFTWARE_KNOCK
	adcStart(&KNOCK_ADC, nullptr);
#endif // EFI_SOFTWARE_KNOCK

#ifdef ADC3_SLOW_CHANNEL_COUNT
	adcStart(&ADCD3, nullptr);
#endif
}

#endif // HAL_USE_ADC
