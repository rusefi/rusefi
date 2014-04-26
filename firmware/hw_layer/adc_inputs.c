/**
 * @file	adc_inputs.c
 * @brief	Low level ADC code
 *
 * @date Jan 14, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "adc_inputs.h"

#include "pin_repository.h"
#include "engine_math.h"
#include "map_averaging.h"
#include "engine_configuration.h"


#define ADC_GRP1_BUF_DEPTH_FAST      1

#define ADC_NUMBER_CHANNELS_FAST		1

#define PWM_FREQ_SLOW 5000   /* PWM clock frequency. I wonder what does this setting mean?  */
#define PWM_PERIOD_SLOW 500  /* PWM period (in PWM ticks).    */

#define PWM_FREQ_FAST 1500000   /* PWM clock frequency. I wonder what does this setting mean?  */
#define PWM_PERIOD_FAST 50  /* PWM period (in PWM ticks).    */

#define ADC_SLOW ADCD1

#define ADC_FAST ADCD2

#define ADC_DEBUG_KEY "adcDebug"

static char LOGGING_BUFFER[500];
static Logging logger;
static int adcCallbackCounter_slow = 0;

static int adcDebugReporting = FALSE;

static int internalAdcIndexByHardwareIndex[20];
static int hardwareIndexByIndernalAdcIndex[20];

static int fastAdcValue;
extern engine_configuration_s *engineConfiguration;

static adc_hw_helper_s slowAdcState;

/*
 * ADC samples buffer.
 */
static adcsample_t samples_fast[ADC_NUMBER_CHANNELS_FAST * ADC_GRP1_BUF_DEPTH_FAST];

static adcsample_t getAvgAdcValue(int index, adcsample_t *samples, int bufDepth, int numChannels) {
	adcsample_t result = 0;
	int i;
	for (i = 0; i < bufDepth; i++) {
		result += samples[index];
		index += numChannels;
	}
	return result / bufDepth;
}

static adc_state newState;

static void adc_callback_slow(ADCDriver *adcp, adcsample_t *buffer, size_t n) {
	(void) buffer;
	(void) n;
	/* Note, only in the ADC_COMPLETE state because the ADC driver fires an
	 intermediate callback when the buffer is half full.*/
	if (adcp->state == ADC_COMPLETE) {
		/* Calculates the average values from the ADC samples.*/

		adcCallbackCounter_slow++;

//		newState.time = chimeNow();
		for (int i = 0; i < EFI_ADC_SLOW_CHANNELS_COUNT; i++) {
			int value = getAvgAdcValue(i, slowAdcState.samples, ADC_GRP1_BUF_DEPTH_SLOW, EFI_ADC_SLOW_CHANNELS_COUNT);
			newState.adc_data[i] = value;
		}
	}
}

static void adc_callback_fast(ADCDriver *adcp, adcsample_t *buffer, size_t n) {
	(void) buffer;
	(void) n;
//	/* Note, only in the ADC_COMPLETE state because the ADC driver fires an
//	 intermediate callback when the buffer is half full.*/
	if (adcp->state == ADC_COMPLETE) {
		fastAdcValue = getAvgAdcValue(0, samples_fast, ADC_GRP1_BUF_DEPTH_FAST, ADC_NUMBER_CHANNELS_FAST);
		mapAveragingCallback(fastAdcValue);
	}
}

#define MY_SAMPLING_SLOW ADC_SAMPLE_480
#define MY_SAMPLING_FAST ADC_SAMPLE_28

/*
 * ADC conversion group.
 */
static ADCConversionGroup adcgrpcfg_slow = { FALSE, EFI_ADC_SLOW_CHANNELS_COUNT, adc_callback_slow, NULL,
/* HW dependent part.*/
ADC_TwoSamplingDelay_20Cycles,   // cr1
		ADC_CR2_SWSTART, // cr2

		ADC_SMPR1_SMP_AN10(MY_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN11(MY_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN12(MY_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN13(MY_SAMPLING_SLOW), // sample times for channels 10...18
		ADC_SMPR2_SMP_AN0(MY_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN1(MY_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN3(MY_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN4(MY_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN5(MY_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN6(MY_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN7(MY_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN8(MY_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN9(MY_SAMPLING_SLOW)

		, // In this field must be specified the sample times for channels 0...9

		ADC_SQR1_NUM_CH(EFI_ADC_SLOW_CHANNELS_COUNT), // Conversion group sequence 13...16 + sequence length

		0
//		| ADC_SQR2_SQ7_N(ADC_CHANNEL_IN12) /* PC2 - green */
//				| ADC_SQR2_SQ8_N(ADC_CHANNEL_IN13) /* PC3 - yellow maf? */

		,// Conversion group sequence 7...12
		0
//		| ADC_SQR3_SQ1_N(ADC_CHANNEL_IN6) /* PA6 - white */
//		| ADC_SQR3_SQ2_N(ADC_CHANNEL_IN7) /* PA7 - blue */
//				| ADC_SQR3_SQ3_N(ADC_CHANNEL_IN14) /* PC4 - green */
//				| ADC_SQR3_SQ4_N(ADC_CHANNEL_IN15) /* PC5 - yellow */
//				| ADC_SQR3_SQ5_N(ADC_CHANNEL_IN8) /* PB0 - blue */
//				| ADC_SQR3_SQ6_N(ADC_CHANNEL_IN9) /* PB1 - white */
// Conversion group sequence 1...6
		};

static const ADCConversionGroup adcgrpcfg_fast = { FALSE, ADC_NUMBER_CHANNELS_FAST, adc_callback_fast, NULL,
/* HW dependent part.*/
ADC_TwoSamplingDelay_5Cycles,   // cr1
		ADC_CR2_SWSTART, // cr2

		0, // sample times for channels 10...18
		ADC_SMPR2_SMP_AN0(MY_SAMPLING_FAST), // In this field must be specified the sample times for channels 0...9

		ADC_SQR1_NUM_CH(ADC_NUMBER_CHANNELS_FAST), // Conversion group sequence 13...16 + sequence length

		0, // Conversion group sequence 7...12
		0 | ADC_SQR3_SQ1_N(ADC_CHANNEL_IN3) /* PA3 */
// maf		| ADC_SQR3_SQ1_N(ADC_CHANNEL_IN13) /* PC3 */

		// Conversion group sequence 1...6
		};

static void pwmpcb_slow(PWMDriver *pwmp) {
#ifdef EFI_INTERNAL_ADC
	(void) pwmp;

	/* Starts an asynchronous ADC conversion operation, the conversion
	 will be executed in parallel to the current PWM cycle and will
	 terminate before the next PWM cycle.*/chSysLockFromIsr()
	;
	adcStartConversionI(&ADC_SLOW, &adcgrpcfg_slow, slowAdcState.samples, ADC_GRP1_BUF_DEPTH_SLOW);
	chSysUnlockFromIsr()
	;
#endif
}

static void pwmpcb_fast(PWMDriver *pwmp) {
#ifdef EFI_INTERNAL_ADC
	(void) pwmp;

	/* Starts an asynchronous ADC conversion operation, the conversion
	 will be executed in parallel to the current PWM cycle and will
	 terminate before the next PWM cycle.*/chSysLockFromIsr()
	;
	adcStartConversionI(&ADC_FAST, &adcgrpcfg_fast, samples_fast, ADC_GRP1_BUF_DEPTH_FAST);
	chSysUnlockFromIsr()
	;
#endif
}

int getAdcValueByIndex(int internalIndex) {
	return newState.adc_data[internalIndex];
}

int getInternalAdcValue(int hwChannel) {
//	if (hwIndex==ADC_NUMBER_CHANNELS_FAST)
//		return fastAdcValue;

	int internalIndex = internalAdcIndexByHardwareIndex[hwChannel];
	return getAdcValueByIndex(internalIndex);
}

static PWMConfig pwmcfg_slow = { PWM_FREQ_SLOW, PWM_PERIOD_SLOW, pwmpcb_slow, { {
PWM_OUTPUT_DISABLED, NULL }, { PWM_OUTPUT_DISABLED, NULL }, {
PWM_OUTPUT_DISABLED, NULL }, { PWM_OUTPUT_DISABLED, NULL } },
/* HW dependent part.*/
0 };

static PWMConfig pwmcfg_fast = { PWM_FREQ_FAST, PWM_PERIOD_FAST, pwmpcb_fast, { {
PWM_OUTPUT_DISABLED, NULL }, { PWM_OUTPUT_DISABLED, NULL }, {
PWM_OUTPUT_DISABLED, NULL }, { PWM_OUTPUT_DISABLED, NULL } },
/* HW dependent part.*/
0 };

static void initAdcPin(ioportid_t port, int pin, char *msg) {
	print("adc %s\r\n", msg);
	mySetPadMode("adc input", port, pin, PAL_MODE_INPUT_ANALOG);
}

GPIO_TypeDef* getAdcChannelPort(int hwChannel) {
	// todo: replace this with an array :)
	switch (hwChannel) {
	case ADC_CHANNEL_IN0:
		return GPIOA;
	case ADC_CHANNEL_IN1:
		return GPIOA;
	case ADC_CHANNEL_IN2:
		return GPIOA;
	case ADC_CHANNEL_IN3:
		return GPIOA;
	case ADC_CHANNEL_IN4:
		return GPIOA;
	case ADC_CHANNEL_IN5:
		return GPIOA;
	case ADC_CHANNEL_IN6:
		return GPIOA;
	case ADC_CHANNEL_IN7:
		return GPIOA;
	case ADC_CHANNEL_IN8:
		return GPIOB;
	case ADC_CHANNEL_IN9:
		return GPIOB;
	case ADC_CHANNEL_IN10:
		return GPIOC;
	case ADC_CHANNEL_IN11:
		return GPIOC;
	case ADC_CHANNEL_IN12:
		return GPIOC;
	case ADC_CHANNEL_IN13:
		return GPIOC;
	case ADC_CHANNEL_IN14:
		return GPIOC;
	case ADC_CHANNEL_IN15:
		return GPIOC;
	default:
		fatal("Unknown hw channel");
		return NULL;
	}
}

int getAdcChannelPin(int hwChannel) {
	// todo: replace this with an array :)
	switch (hwChannel) {
	case ADC_CHANNEL_IN0:
		return 0;
	case ADC_CHANNEL_IN1:
		return 1;
	case ADC_CHANNEL_IN2:
		return 2;
	case ADC_CHANNEL_IN3:
		return 3;
	case ADC_CHANNEL_IN4:
		return 4;
	case ADC_CHANNEL_IN5:
		return 5;
		break;
	case ADC_CHANNEL_IN6:
		return 6;
	case ADC_CHANNEL_IN7:
		return 7;
	case ADC_CHANNEL_IN8:
		return 0;
	case ADC_CHANNEL_IN9:
		return 1;
	case ADC_CHANNEL_IN10:
		return 0;
	case ADC_CHANNEL_IN11:
		return 1;
	case ADC_CHANNEL_IN12:
		return 2;
	case ADC_CHANNEL_IN13:
		return 3;
	case ADC_CHANNEL_IN14:
		return 4;
	case ADC_CHANNEL_IN15:
		return 5;
	default:
		fatal("Unknown hw channel");
		return -1;
	}
}

static void initAdcHwChannel(int hwChannel) {
	GPIO_TypeDef* port = getAdcChannelPort(hwChannel);
	int pin = getAdcChannelPin(hwChannel);

	initAdcPin(port, pin, "hw");
}

void initSlowChannel(int logicChannel, int hwChannel) {
	internalAdcIndexByHardwareIndex[hwChannel] = logicChannel;
	hardwareIndexByIndernalAdcIndex[logicChannel] = hwChannel;
	if (logicChannel < 6) {
		adcgrpcfg_slow.sqr3 += (hwChannel) << (5 * logicChannel);
	} else {
		adcgrpcfg_slow.sqr2 += (hwChannel) << (5 * (logicChannel - 6));
	}

	initAdcHwChannel(hwChannel);
}

static void printAdcValue(int channel) {
	int value = getAdcValue(channel);
	float volts = adcToVoltsDivided(value);
	scheduleMsg(&logger, "adc voltage : %f", volts);
}

int getAdcHardwareIndexByInternalIndex(int index) {
	return hardwareIndexByIndernalAdcIndex[index];
}

static void printFullAdcReport(void) {

	for (int index = 0; index < EFI_ADC_SLOW_CHANNELS_COUNT; index++) {
		appendMsgPrefix(&logger);

		int hwIndex = getAdcHardwareIndexByInternalIndex(index);
		GPIO_TypeDef* port = getAdcChannelPort(hwIndex);
		int pin = getAdcChannelPin(hwIndex);

		int adcValue = getAdcValueByIndex(index);
		appendPrintf(&logger, " ch%d %s%d", index, portname(port), pin);
		appendPrintf(&logger, " ADC%d 12bit=%d", hwIndex, adcValue);
		float volts = adcToVolts(adcValue);
		appendPrintf(&logger, " v=%f", volts);

		appendMsgPostfix(&logger);
		scheduleLogging(&logger);
	}
}

static void printStatus(void) {
	scheduleIntValue(&logger, ADC_DEBUG_KEY, adcDebugReporting);
}

static void setAdcDebugReporting(int value) {
	adcDebugReporting = value;
	printStatus();
}

void initAdcInputs() {

	initLoggingExt(&logger, "ADC", LOGGING_BUFFER, sizeof(LOGGING_BUFFER));

	printStatus();

	addConsoleActionI(ADC_DEBUG_KEY, &setAdcDebugReporting);

#ifdef EFI_INTERNAL_ADC
	/*
	 * Initializes the ADC driver.
	 */
	adcStart(&ADC_SLOW, NULL);
	adcStart(&ADC_FAST, NULL);

	adcgrpcfg_slow.sqr2 = 0;
	adcgrpcfg_slow.sqr3 = 0;

	int index = 0;

#if EFI_USE_ADC_CHANNEL_IN0
	initSlowChannel(index++, ADC_CHANNEL_IN0); // PA0
#endif
#if EFI_USE_ADC_CHANNEL_IN1
	initSlowChannel(index++, ADC_CHANNEL_IN1); // PA1
#endif
#if EFI_USE_ADC_CHANNEL_IN2
	initSlowChannel(index++, ADC_CHANNEL_IN2); // PA2
#endif
#if EFI_USE_ADC_CHANNEL_IN3
	initSlowChannel(index++, ADC_CHANNEL_IN3); // PA3
#endif
#if EFI_USE_ADC_CHANNEL_IN4
	initSlowChannel(index++, ADC_CHANNEL_IN4); // PA4
#endif
#if EFI_USE_ADC_CHANNEL_IN5
	initSlowChannel(index++, ADC_CHANNEL_IN5); // PA5 - this is also TIM2_CH1
#endif
#if EFI_USE_ADC_CHANNEL_IN6
	initSlowChannel(index++, ADC_CHANNEL_IN6); // PA6
#endif
#if EFI_USE_ADC_CHANNEL_IN7
	initSlowChannel(index++, ADC_CHANNEL_IN7); // PA7
#endif
#if EFI_USE_ADC_CHANNEL_IN8
	initSlowChannel(index++, ADC_CHANNEL_IN8); // PB0
#endif
#if EFI_USE_ADC_CHANNEL_IN9
	initSlowChannel(index++, ADC_CHANNEL_IN9); // PB1
#endif
#if EFI_USE_ADC_CHANNEL_IN10
	initSlowChannel(index++, ADC_CHANNEL_IN10); // PC0
#endif
#if EFI_USE_ADC_CHANNEL_IN11
	initSlowChannel(index++, ADC_CHANNEL_IN11); // PC1
#endif
#if EFI_USE_ADC_CHANNEL_IN12
	initSlowChannel(index++, ADC_CHANNEL_IN12); // PC2
#endif
#if EFI_USE_ADC_CHANNEL_IN13
	initSlowChannel(index++, ADC_CHANNEL_IN13); // PC3
#endif
#if EFI_USE_ADC_CHANNEL_IN14
	initSlowChannel(index++, ADC_CHANNEL_IN14); // PC4
#endif
#if EFI_USE_ADC_CHANNEL_IN15
	initSlowChannel(index++, ADC_CHANNEL_IN15); // PC5
#endif

	if (index != EFI_ADC_SLOW_CHANNELS_COUNT)
		fatal("Invalud internal ADC config");

	/*
	 * Initializes the PWM driver.
	 */
	pwmStart(EFI_INTERNAL_SLOW_ADC_PWM, &pwmcfg_slow);
	pwmStart(EFI_INTERNAL_FAST_ADC_PWM, &pwmcfg_fast);
	addConsoleActionI("adc", printAdcValue);
	addConsoleAction("fadc", printFullAdcReport);
#else
	printSimpleMsg(&logger, "ADC disabled", 0);
#endif
}

void pokeAdcInputs() {
	if (!adcDebugReporting)
		return;
	printFullAdcReport();
}

