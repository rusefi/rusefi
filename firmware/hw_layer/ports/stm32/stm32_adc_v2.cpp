/**
 * @file        stm32_adc_v2.cpp
 * @brief       Port implementation for the STM32 "v2" ADC found on the STM32F4 and STM32F7
 *
 * @date February 9, 2021
 * @author Matthew Kennedy, (c) 2021
 */

#include "pch.h"

#ifndef EFI_SLOW_ADC
#define EFI_SLOW_ADC ADCD1
#endif

#ifndef ADC_FAST_DEVICE
#define ADC_FAST_DEVICE ADCD2
#endif

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

// Slow ADC has 16 channels we can sample, or 32 if ADC mux mode is enabled.
static volatile NO_CACHE adcsample_t slowSampleBuffer[SLOW_ADC_OVERSAMPLE * adcChannelCount];
#ifdef ADC_MUX_PIN
static volatile NO_CACHE adcsample_t slowSampleBufferMuxed[SLOW_ADC_OVERSAMPLE * adcChannelCount];
#endif

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

#if EFI_ADC3_SLOW
/* Slow sampling of the ADC3-only pins (EFI_ADC_32..39). The rusEFI ADC3
 * index order follows the adcChannels[] table in stm32_adc.cpp:
 * F6(IN4), F7(IN5), F8(IN6), F9(IN7), F10(IN8), F3(IN9), F4(IN14), F5(IN15).
 * When EFI_SOFTWARE_KNOCK is enabled the same ADC3 also serves the
 * interrupt-driven knock windows - see readSlowAnalogInputs(). */
static const ADCConversionGroup convGroupSlowAdc3 = {
	.circular			= FALSE,
	.num_channels		= 8,
	.end_cb				= nullptr,
	.error_cb			= slowAdcErrorCB,
	/* HW dependent part.*/
	.cr1				= 0,
	.cr2				= ADC_CR2_SWSTART,
	.smpr1 =
		ADC_SMPR1_SMP_AN14(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN15(ADC_SAMPLING_SLOW),
	.smpr2 =
		ADC_SMPR2_SMP_AN4(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN5(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN6(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN7(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN8(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN9(ADC_SAMPLING_SLOW),
	.htr	= 0,
	.ltr	= 0,
	/* EFI_ADC_32..39 in sequence order: IN4, IN5, IN6, IN7, IN8, IN9, IN14, IN15 */
	.sqr1	= 0,
	.sqr2	= ADC_SQR2_SQ7_N(14) | ADC_SQR2_SQ8_N(15),
	.sqr3	= ADC_SQR3_SQ1_N(4) | ADC_SQR3_SQ2_N(5) | ADC_SQR3_SQ3_N(6) | ADC_SQR3_SQ4_N(7) | ADC_SQR3_SQ5_N(8) | ADC_SQR3_SQ6_N(9),
};
#endif // EFI_ADC3_SLOW

#if (EFI_INTERNAL_SLOW_ADC_BACKGROUND == TRUE)

typedef enum {
	convertPrimary,
#ifdef ADC_MUX_PIN
	convertMuxed,
#endif
	convertAux,
	convertAux2,
} slowAdcState_t;

static slowAdcState_t slowAdcGetNextState(slowAdcState_t state)
{
	switch (state) {
	case convertPrimary:
		#ifdef ADC_MUX_PIN
		return convertMuxed;
		#else
		return convertAux;
		#endif
	break;
#ifdef ADC_MUX_PIN
	case convertMuxed:
		return convertAux;
	break;
#endif
	case convertAux:
		return convertAux2;
	case convertAux2:
		return convertPrimary;
	break;
	}
	return convertPrimary;
}

static slowAdcState_t slowAdcState = convertPrimary;

/* Start the conversion batch for the state that slowAdcState currently holds.
 * Must be called with the system locked (ISR context in both users: the batch
 * end callback and the knock resume path). */
static void slowAdcStartCurrentBatch() {
	switch (slowAdcState) {
	case convertPrimary:
		#ifdef ADC_MUX_PIN
		muxControl.setValue(0, /*force*/true);
		#endif
		adcStartConversionI(&EFI_SLOW_ADC, &convGroupSlow, (adcsample_t *)slowSampleBuffer, SLOW_ADC_OVERSAMPLE);
		break;
	#ifdef ADC_MUX_PIN
	case convertMuxed:
		muxControl.setValue(1, /*force*/true);
		// convert second half
		adcStartConversionI(&EFI_SLOW_ADC, &convGroupSlow, (adcsample_t *)slowSampleBufferMuxed, SLOW_ADC_OVERSAMPLE);
		break;
	#endif
	case convertAux:
		adcSTM32DisableVBATE();
		adcStartConversionI(&EFI_SLOW_ADC, &aux1ConvGroup, (adcsample_t *)aux1SensorSamples, auxSensorOversample);
		break;
	case convertAux2:
		adcSTM32EnableVBATE();
		adcStartConversionI(&EFI_SLOW_ADC, &aux2ConvGroup, (adcsample_t *)aux2SensorSamples, auxSensorOversample);
		break;
	}
}

static void slowAdcEndCB(ADCDriver *adcp) {
	if (adcIsBufferComplete(adcp)) {
		chSysLockFromISR();
		// Switch state to ready to allow starting new conversion from here
		adcp->state = ADC_READY;
		// get next state
		slowAdcState = slowAdcGetNextState(slowAdcState);
		slowAdcStartCurrentBatch();
		chSysUnlockFromISR();
	}
}
#endif

static bool readBatch(adcsample_t* convertedSamples, adcsample_t* b) {
#if (EFI_INTERNAL_SLOW_ADC_BACKGROUND == FALSE)
	msg_t result = adcConvert(&EFI_SLOW_ADC, &convGroupSlow, b, SLOW_ADC_OVERSAMPLE);

	// If something went wrong - try again later
	if (result != MSG_OK) {
		return false;
	}

	// MCU Temperature sensor is only physically wired to ADC1
	// todo: disable MCU Temperature sensor if EFI_SLOW_ADC is not ADC1?
	adcConvert(&EFI_SLOW_ADC, &auxConvGroup, (adcsample_t *)auxSensorSamples, auxSensorOversample);

	// Switch IN18 input to Vbat
	adcSTM32EnableVBATE();
	adcConvert(adcp, &aux2ConvGroup, (adcsample_t *)aux2SensorSamples, auxSensorOversample);
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

#if EFI_ADC3_SLOW
/* Blocking conversion of the 8 ADC3-only channels (order per convGroupSlowAdc3).
 * ADCD3 is shared between the 500 Hz slow loop (readSlowAnalogInputs) and the
 * board-local 'knockpin' diagnostic, which run in DIFFERENT threads. A plain
 * adcConvert() here asserts ("already waiting" / "not ready") whenever it
 * collides with the slow-loop conversion and halts the ECU - so check-and-start
 * must be atomic with respect to the slow loop, and a busy ADC3 means "skip,
 * previous values stand" (a missed 2 ms update is invisible). */
static bool adc3SlowConvert(adcsample_t* out) {
	osalSysLock();
	if ((ADCD3.state == ADC_READY) ||
			(ADCD3.state == ADC_ERROR)) {
		adcStartConversionI(&ADCD3, &convGroupSlowAdc3, out, 1);
		msg_t result = osalThreadSuspendS(&ADCD3.thread);
		osalSysUnlock();
		return result == MSG_OK;
	}
	osalSysUnlock();
	return false;
}

bool readSlowAdc3All(adcsample_t samples[8]) {
	return adc3SlowConvert(samples);
}

/* One-shot conversion of the slow ADC1 channels in convGroupSlow order
 * (IN0..IN15 = EFI_ADC_0..15) - used by the board-local 'knockpin'
 * diagnostic. Guarded the same way as the slow read: skip when a conversion
 * is in progress (only relevant with EFI_INTERNAL_SLOW_ADC_BACKGROUND). */
bool readSlowAdc1All(adcsample_t samples[16]) {
	osalSysLock();
	adcstate_t state = EFI_SLOW_ADC.state;
	if ((state == ADC_READY) ||
			(state == ADC_ERROR)) {
		adcStartConversionI(&EFI_SLOW_ADC, &convGroupSlow, samples, 1);
		msg_t result = osalThreadSuspendS(&EFI_SLOW_ADC.thread);
		osalSysUnlock();
		if (result != MSG_OK) {
			static int printed = 0;
			if (printed++ < 3) {
				efiPrintf("readSlowAdc1All: suspend result=%d", (int)result);
			}
		}
		return result == MSG_OK;
	}
	osalSysUnlock();
	static int printed = 0;
	if (printed++ < 3) {
		efiPrintf("readSlowAdc1All: ADCD1 state=%d (READY=%d ERROR=%d ACTIVE=%d COMPLETE=%d)",
			(int)state, (int)ADC_READY, (int)ADC_ERROR, (int)ADC_ACTIVE, (int)ADC_COMPLETE);
		efiPrintf("readSlowAdc1All: ADC1 SR=0x%08x CR1=0x%08x CR2=0x%08x SQR1=0x%08x",
			(unsigned)ADC1->SR, (unsigned)ADC1->CR1, (unsigned)ADC1->CR2, (unsigned)ADC1->SQR1);
		const stm32_dma_stream_t* s = EFI_SLOW_ADC.dmastp;
		if ((s != nullptr) && (s->channel != nullptr)) {
			efiPrintf("readSlowAdc1All: ADCD1 DMA CCR=0x%08x CNDTR=%u",
				(unsigned)s->channel->CCR, (unsigned)s->channel->CNDTR);
			if (s->dma != nullptr) {
				efiPrintf("readSlowAdc1All: ADCD1 DMA ISR=0x%08x IFCR=0x%08x MUXSEL=0x%08x",
					(unsigned)s->dma->ISR, (unsigned)s->dma->IFCR, (unsigned)s->dma->MUXSEL);
			}
		} else {
			efiPrintf("readSlowAdc1All: ADCD1 dmastp is NULL");
		}
	}
	return false;
}
#endif // EFI_ADC3_SLOW

bool readSlowAnalogInputs(adcsample_t* convertedSamples) {
	bool result = true;

	result &= readBatch(convertedSamples, (adcsample_t *)slowSampleBuffer);

#ifdef ADC_MUX_PIN
	#if (EFI_INTERNAL_SLOW_ADC_BACKGROUND == FALSE)
		muxControl.setValue(1, /*force*/true);
	#endif
		// read the second batch, starting where we left off
		result &= readBatch(&convertedSamples[adcChannelCount], (adcsample_t *)slowSampleBufferMuxed);
	#if (EFI_INTERNAL_SLOW_ADC_BACKGROUND == FALSE)
		muxControl.setValue(0, /*force*/true);
	#endif
#endif

#if EFI_ADC3_SLOW
	/* Sample the ADC3-only channels (EFI_ADC_32..39) into the upper part of
	 * the slow buffer. Blocking conversion in thread context - 8 channels at
	 * ADC_SAMPLING_SLOW take a few microseconds, negligible at the slow rate. */
	if (!adc3SlowConvert((adcsample_t *)&convertedSamples[EFI_ADC_32 - EFI_ADC_0])) {
		/* keep the previous values */
	}
#endif // EFI_ADC3_SLOW

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

#else /* !EFI_USE_FAST_ADC */

#include "adc_inputs.h"

/* Fast ADC (second ADC unit + GPT trigger) is disabled for this build.
 * Provide stubs so that generic code (calcFastAdcIndexes etc.) still links.
 * Analog inputs are then sampled by the slow ADC only - see m74_9 board which
 * uses this because the AT32 fast ADC path (TIM6 -> ADC2) does not run. */

AdcToken enableFastAdcChannel(const char*, adc_channel_e) {
	return invalidAdcToken;
}

adcsample_t getFastAdc(AdcToken) {
	return 0;
}

#endif // EFI_USE_FAST_ADC

#ifdef EFI_SOFTWARE_KNOCK

#if (EFI_INTERNAL_SLOW_ADC_BACKGROUND == TRUE)
/* Resume the slow background chain after the ADC has been handed back by a
 * knock window or by the knocktest diagnostic. slowAdcState still points at
 * the batch that was aborted, so re-run that batch from scratch; its buffer
 * may be partially written, which the slow loop tolerates (it averages
 * asynchronously anyway). Must be called with the system locked. */
static void slowAdcResumeAfterKnockWindowI() {
	KNOCK_ADC.state = ADC_READY;
	slowAdcStartCurrentBatch();
}

static void slowAdcResumeAfterKnockWindow() {
	chSysLockFromISR();
	slowAdcResumeAfterKnockWindowI();
	chSysUnlockFromISR();
}
#endif // EFI_INTERNAL_SLOW_ADC_BACKGROUND

static void knockCompletionCallback(ADCDriver* adcp) {
	if (adcIsBufferComplete(adcp)) {
		onKnockSamplingComplete();

#if (EFI_INTERNAL_SLOW_ADC_BACKGROUND == TRUE)
		/* If the knock window stole the ADC from the slow background chain
		 * (shared driver), resume the chain. Runtime pointer compare: boards
		 * with a dedicated knock ADC (proteus/f407/hellen use ADC3) must not
		 * touch the slow chain from here. */
		if (&KNOCK_ADC == &EFI_SLOW_ADC) {
			slowAdcResumeAfterKnockWindow();
		}
#endif // EFI_INTERNAL_SLOW_ADC_BACKGROUND
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

bool isKnockAdcSharedWithSlowAdc() {
	/* m74_9: the knock input (PA0 = ADC1 IN0) lives on the same ADC driver as
	 * the slow sampling. Only then may a knock window abort the in-flight slow
	 * conversion; boards with a dedicated knock ADC (proteus, f407-discovery,
	 * hellen all use ADC3) never steal. The steal is only safe against the
	 * background chain - with blocking slow reads (EFI_INTERNAL_SLOW_ADC_
	 * BACKGROUND == FALSE) a stolen conversion would leave the waiting thread
	 * suspended forever, so those builds keep the old skip-if-busy behavior. */
	return (&KNOCK_ADC == &EFI_SLOW_ADC) && (EFI_INTERNAL_SLOW_ADC_BACKGROUND == TRUE);
}

/* One-shot knock-style burst conversion - used by the board-local 'knocktest'
 * diagnostic to identify the knock input pin. Local group copy with
 * end_cb = nullptr: these bursts must not feed the knock processing pipeline.
 * When the ADC is shared with the slow background chain (m74_9) the burst
 * steals it exactly like a real knock window and hands it back afterwards. */
bool knockBurstSample(uint32_t adcInChannel, adcsample_t* buf, size_t count) {
	ADCConversionGroup group = adcConvGroupCh1;
	group.sqr3 = ADC_SQR3_SQ1_N(adcInChannel);
	group.end_cb = nullptr;

	msg_t result;

	osalSysLock();
	if ((KNOCK_ADC.state == ADC_READY) ||
			(KNOCK_ADC.state == ADC_ERROR)) {
		adcStartConversionI(&KNOCK_ADC, &group, buf, count);
	} else if (((KNOCK_ADC.state == ADC_ACTIVE) ||
				(KNOCK_ADC.state == ADC_COMPLETE)) &&
				isKnockAdcSharedWithSlowAdc()) {
		/* The slow background chain keeps the ADC busy continuously - steal
		 * it (see onStartKnockSampling). */
		adcStopConversionI(&KNOCK_ADC);
		adcStartConversionI(&KNOCK_ADC, &group, buf, count);
	} else {
		osalSysUnlock();
		return false;
	}
	result = osalThreadSuspendS(&KNOCK_ADC.thread);

#if (EFI_INTERNAL_SLOW_ADC_BACKGROUND == TRUE)
	/* Hand the ADC back to the slow background chain: it was either aborted
	 * by this burst (steal above) or already stalled (READY/ERROR start) -
	 * re-running the current batch is correct in both cases. */
	if (isKnockAdcSharedWithSlowAdc()) {
		slowAdcResumeAfterKnockWindowI();
	}
#endif // EFI_INTERNAL_SLOW_ADC_BACKGROUND

	osalSysUnlock();
	return result == MSG_OK;
}

#endif // EFI_SOFTWARE_KNOCK

void portInitAdc() {
#ifdef ADC_MUX_PIN
	muxControl.initPin("ADC Mux", ADC_MUX_PIN);
#endif //ADC_MUX_PIN

	/* Init slow ADC. When software knock uses the same ADC driver (m74_9:
	 * the knock input PA0 lives on ADC1), the driver is started once via the
	 * knock branch below - a second adcStart() would re-allocate the DMA
	 * stream and wedge the boot. */
#if defined(EFI_SOFTWARE_KNOCK)
	if (&EFI_SLOW_ADC != &KNOCK_ADC) {
		adcStart(&EFI_SLOW_ADC, NULL);
	}
#else
	adcStart(&EFI_SLOW_ADC, NULL);
#endif

#ifdef EFI_SOFTWARE_KNOCK
	/* Knock driver. On m74_9 this is the same ADCD1 as the slow ADC and its
	 * start above was skipped, so this is the one and only adcStart() for it;
	 * it also enables the ADC1 clock, which adcSTM32EnableTSVREFE() below
	 * depends on. Must run before the background chain start. */
	adcStart(&KNOCK_ADC, nullptr);
#endif // EFI_SOFTWARE_KNOCK

	// Enable internal temperature reference
	adcSTM32EnableTSVREFE(); // Internal temperature sensor

#if EFI_ADC3_SLOW
	/* Init ADC3 for slow sampling of the ADC3-only pins (CLT/IAT live on the
	 * F-port pins, EFI_ADC_32..39). On this board knock is on ADC1, so ADCD3
	 * has no other users and is started exactly once here. */
	adcStart(&ADCD3, nullptr);
#endif // EFI_ADC3_SLOW

#if (EFI_INTERNAL_SLOW_ADC_BACKGROUND == TRUE)
	adcStartConversion(&EFI_SLOW_ADC, &convGroupSlow, (adcsample_t *)slowSampleBuffer, SLOW_ADC_OVERSAMPLE);
#endif

#if EFI_USE_FAST_ADC
	// Init fast ADC (MAP sensor)
	adcStart(&ADC_FAST_DEVICE, NULL);
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
}

#endif // HAL_USE_ADC
