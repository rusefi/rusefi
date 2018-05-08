/**
 * @file	adc_inputs.cpp
 * @brief	Low level ADC code
 *
 * We are using two ADC devices here.
 * Slow ADC group is used for IAT, CLT, AFR, VBATT etc - this one is currently sampled at 20Hz
 *
 * Fast ADC group is used for TPS, MAP, MAF HIP - this one is currently sampled at 10KHz
 *  We need frequent MAP for map_averaging.cpp
 *  We need frequent TPS for ???
 *
 * @date Jan 14, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"

#if HAL_USE_ADC || defined(__DOXYGEN__)

#include "engine.h"
#include "adc_inputs.h"
#include "AdcConfiguration.h"
#include "mpu_util.h"

#include "pin_repository.h"
#include "engine_math.h"
#include "board_test.h"
#include "engine_controller.h"
#include "maf.h"
#include "biquad.h"

/* Depth of the conversion buffer, channels are sampled X times each.*/
#define ADC_BUF_DEPTH_SLOW      8
#define ADC_BUF_DEPTH_FAST      4

Biquad biq[ADC_MAX_CHANNELS_COUNT];

static adc_channel_mode_e adcHwChannelEnabled[HW_MAX_ADC_INDEX];
static const char * adcHwChannelUsage[HW_MAX_ADC_INDEX];

AdcDevice::AdcDevice(ADCConversionGroup* hwConfig) {
	this->hwConfig = hwConfig;
	channelCount = 0;
	conversionCount = 0;
	errorsCount = 0;

	hwConfig->sqr1 = 0;
	hwConfig->sqr2 = 0;
	hwConfig->sqr3 = 0;
	memset(hardwareIndexByIndernalAdcIndex, 0, sizeof(hardwareIndexByIndernalAdcIndex));
	memset(internalAdcIndexByHardwareIndex, 0xFFFFFFFF, sizeof(internalAdcIndexByHardwareIndex));
}

// todo: migrate from hardware timer to software ADC conversion triggering
// todo: I guess we would have to use ChibiOS timer and not our own timer because
// todo: adcStartConversionI requires OS lock. currently slow ADC is 20Hz
#define PWM_FREQ_SLOW 5000   /* PWM clock frequency. I wonder what does this setting mean?  */
#define PWM_PERIOD_SLOW 250  /* PWM period (in PWM ticks).    */

/**
 * 8000 RPM is 133Hz
 * If we want to sample MAP once per 5 degrees we need 133Hz * (360 / 5) = 9576Hz of fast ADC
 */
// todo: migrate to continues ADC mode? probably not - we cannot afford the callback in
// todo: continues mode. todo: look into our options
#define PWM_FREQ_FAST 100000   /* PWM clock frequency. I wonder what does this setting mean?  */
#define PWM_PERIOD_FAST 10  /* PWM period (in PWM ticks).    */

// is there a reason to have this configurable?
#define ADC_SLOW_DEVICE ADCD1

// is there a reason to have this configurable?
#define ADC_FAST_DEVICE ADCD2

static volatile int slowAdcCounter = 0;
static LoggingWithStorage logger("ADC");

// todo: move this flag to Engine god object
static int adcDebugReporting = false;

EXTERN_ENGINE;

static adcsample_t getAvgAdcValue(int index, adcsample_t *samples, int bufDepth, int numChannels) {
	adcsample_t result = 0;
	for (int i = 0; i < bufDepth; i++) {
		result += samples[index];
		index += numChannels;
	}
	return result / bufDepth;
}

static void adc_callback_slow(ADCDriver *adcp, adcsample_t *buffer, size_t n);

#define ADC_SAMPLING_SLOW ADC_SAMPLE_480
#define ADC_SAMPLING_FAST ADC_SAMPLE_28
/*
 * ADC conversion group.
 */
static ADCConversionGroup adcgrpcfgSlow = { FALSE, 0, adc_callback_slow, NULL,
/* HW dependent part.*/
ADC_TwoSamplingDelay_20Cycles,   // cr1
		ADC_CR2_SWSTART, // cr2
/**
 * here we configure all possible channels for slow mode. Some channels would not actually
 * be used hopefully that's fine to configure all possible channels.
 */
		ADC_SMPR1_SMP_AN10(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN11(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN12(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN13(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN14(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN15(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_SENSOR(ADC_SAMPLE_144)
		, // sample times for channels 10...18
		ADC_SMPR2_SMP_AN0(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN1(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN2(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN3(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN4(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN5(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN6(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN7(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN8(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN9(ADC_SAMPLING_SLOW)

		, // In this field must be specified the sample times for channels 0...9

		0, // Conversion group sequence 13...16 + sequence length
		0, // Conversion group sequence 7...12
		0  // Conversion group sequence 1...6
		};

AdcDevice slowAdc(&adcgrpcfgSlow);

static ADCConversionGroup adcgrpcfg_fast = { FALSE, 0 /* num_channels */, adc_callback_fast, NULL,
/* HW dependent part.*/
ADC_TwoSamplingDelay_5Cycles,   // cr1
		ADC_CR2_SWSTART, // cr2

		/**
		 * here we configure all possible channels for fast mode. Some channels would not actually
         * be used hopefully that's fine to configure all possible channels.
		 *
		 */
		ADC_SMPR1_SMP_AN10(ADC_SAMPLING_FAST) |
		ADC_SMPR1_SMP_AN11(ADC_SAMPLING_FAST) |
		ADC_SMPR1_SMP_AN12(ADC_SAMPLING_FAST) |
		ADC_SMPR1_SMP_AN13(ADC_SAMPLING_FAST) |
		ADC_SMPR1_SMP_AN14(ADC_SAMPLING_FAST) |
		ADC_SMPR1_SMP_AN15(ADC_SAMPLING_FAST)
		, // sample times for channels 10...18
		ADC_SMPR2_SMP_AN0(ADC_SAMPLING_FAST) |
		ADC_SMPR2_SMP_AN1(ADC_SAMPLING_FAST) |
		ADC_SMPR2_SMP_AN2(ADC_SAMPLING_FAST) |
		ADC_SMPR2_SMP_AN3(ADC_SAMPLING_FAST) |
		ADC_SMPR2_SMP_AN4(ADC_SAMPLING_FAST) |
		ADC_SMPR2_SMP_AN5(ADC_SAMPLING_FAST) |
		ADC_SMPR2_SMP_AN6(ADC_SAMPLING_FAST) |
		ADC_SMPR2_SMP_AN7(ADC_SAMPLING_FAST) |
		ADC_SMPR2_SMP_AN8(ADC_SAMPLING_FAST) |
		ADC_SMPR2_SMP_AN9(ADC_SAMPLING_FAST), // In this field must be specified the sample times for channels 0...9

		0, // Conversion group sequence 13...16 + sequence length

		0, // Conversion group sequence 7...12
		0

// Conversion group sequence 1...6
		};

AdcDevice fastAdc(&adcgrpcfg_fast);

void doSlowAdc(void) {

	efiAssertVoid(getRemainingStack(chThdGetSelfX())> 32, "lwStAdcSlow");

#if EFI_INTERNAL_ADC

	/* Starts an asynchronous ADC conversion operation, the conversion
	 will be executed in parallel to the current PWM cycle and will
	 terminate before the next PWM cycle.*/
	slowAdc.conversionCount++;
	chSysLockFromISR()
	;
	if (ADC_SLOW_DEVICE.state != ADC_READY &&
	ADC_SLOW_DEVICE.state != ADC_COMPLETE &&
	ADC_SLOW_DEVICE.state != ADC_ERROR) {
		// todo: why and when does this happen? firmwareError(OBD_PCM_Processor_Fault, "ADC slow not ready?");
		slowAdc.errorsCount++;
		chSysUnlockFromISR()
		;
		return;
	}
	adcStartConversionI(&ADC_SLOW_DEVICE, &adcgrpcfgSlow, slowAdc.samples, ADC_BUF_DEPTH_SLOW);
	chSysUnlockFromISR()
	;
#endif /* EFI_INTERNAL_ADC */
}

static void pwmpcb_slow(PWMDriver *pwmp) {
	(void) pwmp;
	doSlowAdc();
}

static void pwmpcb_fast(PWMDriver *pwmp) {
	efiAssertVoid(getRemainingStack(chThdGetSelfX())> 32, "lwStAdcFast");
#if EFI_INTERNAL_ADC
	(void) pwmp;

	/*
	 * Starts an asynchronous ADC conversion operation, the conversion
	 * will be executed in parallel to the current PWM cycle and will
	 * terminate before the next PWM cycle.
	 */
	chSysLockFromISR()
	;
	if (ADC_FAST_DEVICE.state != ADC_READY &&
	ADC_FAST_DEVICE.state != ADC_COMPLETE &&
	ADC_FAST_DEVICE.state != ADC_ERROR) {
		fastAdc.errorsCount++;
		// todo: when? why? firmwareError(OBD_PCM_Processor_Fault, "ADC fast not ready?");
		chSysUnlockFromISR()
		;
		return;
	}
	adcStartConversionI(&ADC_FAST_DEVICE, &adcgrpcfg_fast, fastAdc.samples, ADC_BUF_DEPTH_FAST);
	chSysUnlockFromISR()
	;
	fastAdc.conversionCount++;
#endif /* EFI_INTERNAL_ADC */
}

float getMCUInternalTemperature(void) {
	float TemperatureValue = adcToVolts(slowAdc.getAdcValueByHwChannel(ADC_CHANNEL_SENSOR));
	TemperatureValue -= 0.760; // Subtract the reference voltage at 25°C
	TemperatureValue /= .0025; // Divide by slope 2.5mV

	TemperatureValue += 25.0; // Add the 25°C
	return TemperatureValue;
}

int getInternalAdcValue(const char *msg, adc_channel_e hwChannel) {
	if (hwChannel == EFI_ADC_NONE) {
		warning(CUSTOM_OBD_ANALOG_INPUT_NOT_CONFIGURED, "ADC: %s input is not configured", msg);
		return -1;
	}
#if EFI_ENABLE_MOCK_ADC
	if (engine->engineState.mockAdcState.hasMockAdc[hwChannel])
		return engine->engineState.mockAdcState.getMockAdcValue(hwChannel);

#endif /* EFI_ENABLE_MOCK_ADC */


	if (adcHwChannelEnabled[hwChannel] == ADC_FAST) {
		int internalIndex = fastAdc.internalAdcIndexByHardwareIndex[hwChannel];
// todo if ADC_BUF_DEPTH_FAST EQ 1
//		return fastAdc.samples[internalIndex];
		int value = getAvgAdcValue(internalIndex, fastAdc.samples, ADC_BUF_DEPTH_FAST, fastAdc.size());
		return value;
	}
	if (adcHwChannelEnabled[hwChannel] != ADC_SLOW) {
		warning(CUSTOM_OBD_39, "ADC is off [%s] index=%d", msg, hwChannel);
	}

	return slowAdc.getAdcValueByHwChannel(hwChannel);
}

static PWMConfig pwmcfg_slow = { PWM_FREQ_SLOW, PWM_PERIOD_SLOW, pwmpcb_slow, { {
PWM_OUTPUT_DISABLED, NULL }, { PWM_OUTPUT_DISABLED, NULL }, {
PWM_OUTPUT_DISABLED, NULL }, { PWM_OUTPUT_DISABLED, NULL } },
/* HW dependent part.*/
0, 0 };

static PWMConfig pwmcfg_fast = { PWM_FREQ_FAST, PWM_PERIOD_FAST, pwmpcb_fast, { {
PWM_OUTPUT_DISABLED, NULL }, { PWM_OUTPUT_DISABLED, NULL }, {
PWM_OUTPUT_DISABLED, NULL }, { PWM_OUTPUT_DISABLED, NULL } },
/* HW dependent part.*/
0, 0 };

static void initAdcPin(brain_pin_e pin, const char *msg) {
	// todo: migrate to scheduleMsg if we want this back print("adc %s\r\n", msg);

	efiSetPadMode("adc input", pin, PAL_MODE_INPUT_ANALOG);
}

brain_pin_e getAdcChannelBrainPin(const char *msg, adc_channel_e hwChannel) {
	// todo: replace this with an array :)
	switch (hwChannel) {
	case ADC_CHANNEL_IN0:
		return GPIOA_0;
	case ADC_CHANNEL_IN1:
		return GPIOA_1;
	case ADC_CHANNEL_IN2:
		return GPIOA_2;
	case ADC_CHANNEL_IN3:
		return GPIOA_3;
	case ADC_CHANNEL_IN4:
		return GPIOA_4;
	case ADC_CHANNEL_IN5:
		return GPIOA_5;
	case ADC_CHANNEL_IN6:
		return GPIOA_6;
	case ADC_CHANNEL_IN7:
		return GPIOA_7;
	case ADC_CHANNEL_IN8:
		return GPIOB_0;
	case ADC_CHANNEL_IN9:
		return GPIOB_1;
	case ADC_CHANNEL_IN10:
		return GPIOC_0;
	case ADC_CHANNEL_IN11:
		return GPIOC_1;
	case ADC_CHANNEL_IN12:
		return GPIOC_2;
	case ADC_CHANNEL_IN13:
		return GPIOC_3;
	case ADC_CHANNEL_IN14:
		return GPIOC_4;
	case ADC_CHANNEL_IN15:
		return GPIOC_5;
	default:
		firmwareError(CUSTOM_ERR_ADC_UNKNOWN_CHANNEL, "Unknown hw channel %d [%s]", hwChannel, msg);
		return GPIO_INVALID;
	}
}

adc_channel_e getAdcChannel(brain_pin_e pin) {
	switch (pin) {
	case GPIOA_0:
		return EFI_ADC_0;
	case GPIOA_1:
		return EFI_ADC_1;
	case GPIOA_2:
		return EFI_ADC_2;
	case GPIOA_3:
		return EFI_ADC_3;
	case GPIOA_4:
		return EFI_ADC_4;
	case GPIOA_5:
		return EFI_ADC_5;
	case GPIOA_6:
		return EFI_ADC_6;
	case GPIOA_7:
		return EFI_ADC_7;
	case GPIOB_0:
		return EFI_ADC_8;
	case GPIOB_1:
		return EFI_ADC_9;
	case GPIOC_0:
		return EFI_ADC_10;
	case GPIOC_1:
		return EFI_ADC_11;
	case GPIOC_2:
		return EFI_ADC_12;
	case GPIOC_3:
		return EFI_ADC_13;
	case GPIOC_4:
		return EFI_ADC_14;
	case GPIOC_5:
		return EFI_ADC_15;
	default:
		return EFI_ADC_ERROR;
	}
}

// deprecated - migrate to 'getAdcChannelBrainPin'
ioportid_t getAdcChannelPort(const char *msg, adc_channel_e hwChannel) {
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
		firmwareError(CUSTOM_ERR_ADC_UNKNOWN_CHANNEL, "Unknown hw channel %d [%s]", hwChannel, msg);
		return NULL;
	}
}

const char * getAdcMode(adc_channel_e hwChannel) {
	if (slowAdc.isHwUsed(hwChannel)) {
		return "slow";
	}
	if (fastAdc.isHwUsed(hwChannel)) {
		return "fast";
	}
	return "INACTIVE - need restart";
}

// deprecated - migrate to 'getAdcChannelBrainPin'
int getAdcChannelPin(adc_channel_e hwChannel) {
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
		// todo: better error handling, that's input parameter validation
		firmwareError(CUSTOM_ERR_ADC_CHANNEL, "Unknown hw channel %d", hwChannel);
		return -1;
	}
}

static void initAdcHwChannel(adc_channel_e hwChannel) {
	brain_pin_e pin = getAdcChannelBrainPin("adc", hwChannel);

	initAdcPin(pin, "hw");
}

int AdcDevice::size() {
	return channelCount;
}

int AdcDevice::getAdcValueByHwChannel(int hwChannel) {
	int internalIndex = internalAdcIndexByHardwareIndex[hwChannel];
	return values.adc_data[internalIndex];
}

int AdcDevice::getAdcValueByIndex(int internalIndex) {
	return values.adc_data[internalIndex];
}

void AdcDevice::init(void) {
	hwConfig->num_channels = size();
	hwConfig->sqr1 += ADC_SQR1_NUM_CH(size());
}

bool AdcDevice::isHwUsed(adc_channel_e hwChannelIndex) {
	for (int i = 0; i < channelCount; i++) {
		if (hardwareIndexByIndernalAdcIndex[i] == hwChannelIndex) {
			return true;
		}
	}
	return false;
}

void AdcDevice::enableChannel(adc_channel_e hwChannel) {
	int logicChannel = channelCount++;

	internalAdcIndexByHardwareIndex[hwChannel] = logicChannel;
	hardwareIndexByIndernalAdcIndex[logicChannel] = hwChannel;
	if (logicChannel < 6) {
		hwConfig->sqr3 += (hwChannel) << (5 * logicChannel);
	} else if (logicChannel < 12) {
		hwConfig->sqr2 += (hwChannel) << (5 * (logicChannel - 6));
	} else {
		hwConfig->sqr1 += (hwChannel) << (5 * (logicChannel - 12));
	}
	// todo: support for more then 12 channels? not sure how needed it would be
}

void AdcDevice::enableChannelAndPin(adc_channel_e hwChannel) {
	enableChannel(hwChannel);

	initAdcHwChannel(hwChannel);
}

static void printAdcValue(adc_channel_e channel) {
	int value = getAdcValue("print", channel);
	float volts = adcToVoltsDivided(value);
	scheduleMsg(&logger, "adc voltage : %.2f", volts);
}

adc_channel_e AdcDevice::getAdcHardwareIndexByInternalIndex(int index) {
	return hardwareIndexByIndernalAdcIndex[index];
}

static void printFullAdcReport(Logging *logger) {
	scheduleMsg(logger, "fast %d slow %d", fastAdc.conversionCount, slowAdc.conversionCount);

	for (int index = 0; index < slowAdc.size(); index++) {
		appendMsgPrefix(logger);

		adc_channel_e hwIndex = slowAdc.getAdcHardwareIndexByInternalIndex(index);

		if(hwIndex != EFI_ADC_NONE && hwIndex != EFI_ADC_ERROR)
		{
			ioportid_t port = getAdcChannelPort("print", hwIndex);
			int pin = getAdcChannelPin(hwIndex);

			int adcValue = slowAdc.getAdcValueByIndex(index);
			appendPrintf(logger, " ch%d %s%d", index, portname(port), pin);
			appendPrintf(logger, " ADC%d 12bit=%d", hwIndex, adcValue);
			float volts = adcToVolts(adcValue);
			appendPrintf(logger, " v=%.2f", volts);

			appendMsgPostfix(logger);
			scheduleLogging(logger);
		}
	}
}

static void setAdcDebugReporting(int value) {
	adcDebugReporting = value;
	scheduleMsg(&logger, "adcDebug=%d", adcDebugReporting);
}

void waitForSlowAdc(int lastAdcCounter) {
	// we use slowAdcCounter instead of slowAdc.conversionCount because we need ADC_COMPLETE state
	// todo: use sync.objects?
	while (slowAdcCounter <= lastAdcCounter) {
		chThdSleepMilliseconds(1);
	}
}

int getSlowAdcCounter() {
	return slowAdcCounter;
}

static void adc_callback_slow(ADCDriver *adcp, adcsample_t *buffer, size_t n) {
	(void) buffer;
	(void) n;
	efiAssertVoid(getRemainingStack(chThdGetSelfX()) > 128, "lowstck#9c");
	/* Note, only in the ADC_COMPLETE state because the ADC driver fires
	 * an intermediate callback when the buffer is half full. */
	if (adcp->state == ADC_COMPLETE) {
		/* Calculates the average values from the ADC samples.*/
		for (int i = 0; i < slowAdc.size(); i++) {
			int value = getAvgAdcValue(i, slowAdc.samples, ADC_BUF_DEPTH_SLOW, slowAdc.size());
			adcsample_t prev = slowAdc.values.adc_data[i];
			float result = (slowAdcCounter == 0) ? value :
					CONFIG(slowAdcAlpha) * value + (1 - CONFIG(slowAdcAlpha)) * prev;
//			if (slowAdcCounter == 0) {
//				biq[i].initValue(value);
//			}
//			float result = biq[i].getValue(value);

			slowAdc.values.adc_data[i] = (int)result;
		}
		slowAdcCounter++;
	}
}

static char errorMsgBuff[10];

static void addChannel(const char *name, adc_channel_e setting, adc_channel_mode_e mode) {
	if (setting == EFI_ADC_NONE) {
		return;
	}
	if (adcHwChannelEnabled[setting] != ADC_OFF) {
		getPinNameByAdcChannel(name, setting, errorMsgBuff);
		firmwareError(CUSTOM_ERR_ADC_USED, "ADC mapping error: input %s for %s already used by %s?", errorMsgBuff, name, adcHwChannelUsage[setting]);
	}

	adcHwChannelUsage[setting] = name;
	adcHwChannelEnabled[setting] = mode;
}

static void configureInputs(void) {
	memset(adcHwChannelEnabled, 0, sizeof(adcHwChannelEnabled));
	memset(adcHwChannelUsage, 0, sizeof(adcHwChannelUsage));

	addChannel("MAP", engineConfiguration->map.sensor.hwChannel, ADC_FAST);
	if (hasMafSensor()) {
		addChannel("MAF", engineConfiguration->mafAdcChannel, ADC_FAST);
	}
	addChannel("hip", engineConfiguration->hipOutputChannel, ADC_FAST);

	addChannel("TPS", engineConfiguration->tpsAdcChannel, ADC_SLOW);
	addChannel("fuel", engineConfiguration->fuelLevelSensor, ADC_SLOW);
	addChannel("pPS", engineConfiguration->pedalPositionChannel, ADC_SLOW);
	addChannel("VBatt", engineConfiguration->vbattAdcChannel, ADC_SLOW);
	// not currently used	addChannel("Vref", engineConfiguration->vRefAdcChannel, ADC_SLOW);
	addChannel("CLT", engineConfiguration->clt.adcChannel, ADC_SLOW);
	addChannel("IAT", engineConfiguration->iat.adcChannel, ADC_SLOW);
	addChannel("AFR", engineConfiguration->afr.hwChannel, ADC_SLOW);
	addChannel("OilP", engineConfiguration->oilPressure.hwChannel, ADC_SLOW);
	addChannel("AC", engineConfiguration->acSwitchAdc, ADC_SLOW);

	if (boardConfiguration->isCJ125Enabled) {
		addChannel("cj125ur", engineConfiguration->cj125ur, ADC_SLOW);
		addChannel("cj125ua", engineConfiguration->cj125ua, ADC_SLOW);
	}

	for (int i = 0; i < FSIO_ANALOG_INPUT_COUNT ; i++) {
		addChannel("FSIOadc", engineConfiguration->fsioAdc[i], ADC_SLOW);
	}
}

void initAdcInputs(bool boardTestMode) {
	printMsg(&logger, "initAdcInputs()");
	if (ADC_BUF_DEPTH_FAST > MAX_ADC_GRP_BUF_DEPTH)
		firmwareError(CUSTOM_ERR_ADC_DEPTH_FAST, "ADC_BUF_DEPTH_FAST too high");
	if (ADC_BUF_DEPTH_SLOW > MAX_ADC_GRP_BUF_DEPTH)
		firmwareError(CUSTOM_ERR_ADC_DEPTH_SLOW, "ADC_BUF_DEPTH_SLOW too high");

	configureInputs();

	// migrate to 'enable adcdebug'
	addConsoleActionI("adcdebug", &setAdcDebugReporting);

#if EFI_INTERNAL_ADC
	/*
	 * Initializes the ADC driver.
	 */
	adcStart(&ADC_SLOW_DEVICE, NULL);
	adcStart(&ADC_FAST_DEVICE, NULL);
	adcSTM32EnableTSVREFE(); // Internal temperature sensor

	for (int adc = 0; adc < HW_MAX_ADC_INDEX; adc++) {
		adc_channel_mode_e mode = adcHwChannelEnabled[adc];

		/**
		 * in board test mode all currently enabled ADC channels are running in slow mode
		 */
		if (mode == ADC_SLOW || (boardTestMode && mode == ADC_FAST)) {
			slowAdc.enableChannelAndPin((adc_channel_e) (ADC_CHANNEL_IN0 + adc));
		} else if (mode == ADC_FAST) {
			fastAdc.enableChannelAndPin((adc_channel_e) (ADC_CHANNEL_IN0 + adc));
		}
	}

	// Internal temperature sensor, Available on ADC1 only
	slowAdc.enableChannel((adc_channel_e)ADC_CHANNEL_SENSOR);

	slowAdc.init();
	pwmStart(EFI_INTERNAL_SLOW_ADC_PWM, &pwmcfg_slow);
	pwmEnablePeriodicNotification(EFI_INTERNAL_SLOW_ADC_PWM);

	if (boardConfiguration->isFastAdcEnabled) {
		fastAdc.init();
		/*
		 * Initializes the PWM driver.
		 */
		pwmStart(EFI_INTERNAL_FAST_ADC_PWM, &pwmcfg_fast);
		pwmEnablePeriodicNotification(EFI_INTERNAL_FAST_ADC_PWM);
	}

	// ADC_CHANNEL_IN0 // PA0
	// ADC_CHANNEL_IN1 // PA1
	// ADC_CHANNEL_IN2 // PA2
	// ADC_CHANNEL_IN3 // PA3
	// ADC_CHANNEL_IN4 // PA4
	// ADC_CHANNEL_IN5 // PA5 - this is also TIM2_CH1
	// ADC_CHANNEL_IN6 // PA6
	// ADC_CHANNEL_IN7 // PA7
	// ADC_CHANNEL_IN8 // PB0
	// ADC_CHANNEL_IN9 // PB1
	// ADC_CHANNEL_IN10 // PC0
	// ADC_CHANNEL_IN11 // PC1
	// ADC_CHANNEL_IN12 // PC2
	// ADC_CHANNEL_IN13 // PC3
	// ADC_CHANNEL_IN14 // PC4
	// ADC_CHANNEL_IN15 // PC5

	//if(slowAdcChannelCount > ADC_MAX_SLOW_CHANNELS_COUNT) // todo: do we need this logic? do we need this check

	addConsoleActionI("adc", (VoidInt) printAdcValue);
#else
	printMsg(&logger, "ADC disabled");
#endif
}

void printFullAdcReportIfNeeded(Logging *logger) {
	if (!adcDebugReporting)
		return;
	printFullAdcReport(logger);
}

#endif /* HAL_USE_ADC */
