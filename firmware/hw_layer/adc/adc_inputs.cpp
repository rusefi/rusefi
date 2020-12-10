/**
 * @file	adc_inputs.cpp
 * @brief	Low level ADC code
 *
 * rusEfi uses two ADC devices on the same 16 pins at the moment. Two ADC devices are used in orde to distinguish between
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

#include "global.h"

#if HAL_USE_ADC
#include "os_access.h"

#include "engine.h"
#include "adc_inputs.h"
#include "adc_subscription.h"
#include "AdcConfiguration.h"
#include "mpu_util.h"
#include "periodic_thread_controller.h"

#include "pin_repository.h"
#include "engine_math.h"
#include "engine_controller.h"
#include "maf.h"
#include "perf_trace.h"

// on F7 this must be aligned on a 32-byte boundary, and be a multiple of 32 bytes long.
// When we invalidate the cache line(s) for ADC samples, we don't want to nuke any
// adjacent data.
// F4 does not care
static __ALIGNED(32) adcsample_t slowAdcSampleBuf[ADC_BUF_DEPTH_SLOW * ADC_MAX_CHANNELS_COUNT];
static __ALIGNED(32) adcsample_t fastAdcSampleBuf[ADC_BUF_DEPTH_FAST * ADC_MAX_CHANNELS_COUNT];

static_assert(sizeof(slowAdcSampleBuf) % 32 == 0, "Slow ADC sample buffer size must be a multiple of 32 bytes");
static_assert(sizeof(fastAdcSampleBuf) % 32 == 0, "Fast ADC sample buffer size must be a multiple of 32 bytes");

static adc_channel_mode_e adcHwChannelEnabled[HW_MAX_ADC_INDEX];

EXTERN_ENGINE;

// Board voltage, with divider coefficient accounted for
float getVoltageDivided(const char *msg, adc_channel_e hwChannel DECLARE_ENGINE_PARAMETER_SUFFIX) {
	return getVoltage(msg, hwChannel PASS_ENGINE_PARAMETER_SUFFIX) * engineConfiguration->analogInputDividerCoefficient;
}

// voltage in MCU universe, from zero to VDD
float getVoltage(const char *msg, adc_channel_e hwChannel DECLARE_ENGINE_PARAMETER_SUFFIX) {
	return adcToVolts(getAdcValue(msg, hwChannel));
}

AdcDevice::AdcDevice(ADCConversionGroup* hwConfig, adcsample_t *buf, size_t buf_len) {
	this->hwConfig = hwConfig;
	this->samples = buf;
	this->buf_len = buf_len;

	hwConfig->sqr1 = 0;
	hwConfig->sqr2 = 0;
	hwConfig->sqr3 = 0;
#if ADC_MAX_CHANNELS_COUNT > 16
	hwConfig->sqr4 = 0;
	hwConfig->sqr5 = 0;
#endif /* ADC_MAX_CHANNELS_COUNT */
	memset(hardwareIndexByIndernalAdcIndex, EFI_ADC_NONE, sizeof(hardwareIndexByIndernalAdcIndex));
	memset(internalAdcIndexByHardwareIndex, 0xFFFFFFFF, sizeof(internalAdcIndexByHardwareIndex));
}

#if !defined(GPT_FREQ_FAST) || !defined(GPT_PERIOD_FAST)
/**
 * 8000 RPM is 133Hz
 * If we want to sample MAP once per 5 degrees we need 133Hz * (360 / 5) = 9576Hz of fast ADC
 */
// todo: migrate to continues ADC mode? probably not - we cannot afford the callback in
// todo: continues mode. todo: look into our options
#define GPT_FREQ_FAST 100000   /* PWM clock frequency. I wonder what does this setting mean?  */
#define GPT_PERIOD_FAST 10  /* PWM period (in PWM ticks).    */
#endif /* GPT_FREQ_FAST GPT_PERIOD_FAST */

// is there a reason to have this configurable at runtime?
#ifndef ADC_SLOW_DEVICE
#define ADC_SLOW_DEVICE ADCD1
#endif /* ADC_SLOW_DEVICE */

// is there a reason to have this configurable at runtime?
#ifndef ADC_FAST_DEVICE
#define ADC_FAST_DEVICE ADCD2
#endif /* ADC_FAST_DEVICE */

static volatile int slowAdcCounter = 0;
static LoggingWithStorage logger("ADC");

// todo: move this flag to Engine god object
static int adcDebugReporting = false;

EXTERN_ENGINE;

static adcsample_t getAvgAdcValue(int index, adcsample_t *samples, int bufDepth, int numChannels) {
	uint32_t result = 0;
	for (int i = 0; i < bufDepth; i++) {
		result += samples[index];
		index += numChannels;
	}

	// this truncation is guaranteed to not be lossy - the average can't be larger than adcsample_t
	return static_cast<adcsample_t>(result / bufDepth);
}


// See https://github.com/rusefi/rusefi/issues/976 for discussion on these values
#define ADC_SAMPLING_SLOW ADC_SAMPLE_56
#define ADC_SAMPLING_FAST ADC_SAMPLE_28
/*
 * ADC conversion group.
 */
static ADCConversionGroup adcgrpcfgSlow = {
	.circular			= FALSE,
	.num_channels		= 0,
	.end_cb				= nullptr,
	.error_cb			= nullptr,
	/* HW dependent part.*/
	.cr1				= 0,
	.cr2				= ADC_CR2_SWSTART,
	/**
	 * here we configure all possible channels for slow mode. Some channels would not actually
	 * be used hopefully that's fine to configure all possible channels.
	 */
	// sample times for channels 10...18
	.smpr1 =
		ADC_SMPR1_SMP_AN10(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN11(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN12(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN13(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN14(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN15(ADC_SAMPLING_SLOW) |
#if defined(STM32F7XX)
		ADC_SMPR1_SMP_VBAT(ADC_SAMPLE_144)    |	/* input18 - temperature and vbat input on some STM32F7xx */
#endif
		ADC_SMPR1_SMP_SENSOR(ADC_SAMPLE_144),	/* input16 - temperature sensor input on STM32F4xx */
	// In this field must be specified the sample times for channels 0...9
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

AdcDevice slowAdc(&adcgrpcfgSlow, slowAdcSampleBuf, ARRAY_SIZE(slowAdcSampleBuf));

void adc_callback_fast(ADCDriver *adcp, adcsample_t *buffer, size_t n);

static ADCConversionGroup adcgrpcfgFast = {
	.circular			= FALSE,
	.num_channels		= 0,
	.end_cb				= adc_callback_fast,
	.error_cb			= nullptr,
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

AdcDevice fastAdc(&adcgrpcfgFast, fastAdcSampleBuf, ARRAY_SIZE(fastAdcSampleBuf));

#if HAL_USE_GPT
static void fast_adc_callback(GPTDriver*) {
#if EFI_INTERNAL_ADC
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

	adcStartConversionI(&ADC_FAST_DEVICE, &adcgrpcfgFast, fastAdc.samples, ADC_BUF_DEPTH_FAST);
	chSysUnlockFromISR()
	;
	fastAdc.conversionCount++;
#endif /* EFI_INTERNAL_ADC */
}
#endif /* HAL_USE_GPT */

float getMCUInternalTemperature() {
#if defined(ADC_CHANNEL_SENSOR)
	float TemperatureValue = adcToVolts(slowAdc.getAdcValueByHwChannel(EFI_ADC_TEMP_SENSOR));
	TemperatureValue -= 0.760f; // Subtract the reference voltage at 25 deg C
	TemperatureValue /= 0.0025f; // Divide by slope 2.5mV

	TemperatureValue += 25.0; // Add the 25 deg C

	if (TemperatureValue > 150.0f || TemperatureValue < -50.0f) {
		firmwareError(OBD_PCM_Processor_Fault, "Invalid CPU temperature measured %f", TemperatureValue);
	}

	return TemperatureValue;
#else
	return 0;
#endif /* ADC_CHANNEL_SENSOR */
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
	    // todo: make this not happen during hardware continues integration
		warning(CUSTOM_OBD_WRONG_ADC_MODE, "ADC is off [%s] index=%d", msg, hwChannel);
	}

	return slowAdc.getAdcValueByHwChannel(hwChannel);
}

#if HAL_USE_GPT
static GPTConfig fast_adc_config = {
	GPT_FREQ_FAST,
	fast_adc_callback,
	0, 0
};
#endif /* HAL_USE_GPT */

adc_channel_mode_e getAdcMode(adc_channel_e hwChannel) {
	if (slowAdc.isHwUsed(hwChannel)) {
		return ADC_SLOW;
	}
	if (fastAdc.isHwUsed(hwChannel)) {
		return ADC_FAST;
	}
	return ADC_OFF;
}

int AdcDevice::size() const {
	return channelCount;
}

int AdcDevice::getAdcValueByHwChannel(adc_channel_e hwChannel) const {
	int internalIndex = internalAdcIndexByHardwareIndex[hwChannel];
	return values.adc_data[internalIndex];
}

int AdcDevice::getAdcValueByIndex(int internalIndex) const {
	return values.adc_data[internalIndex];
}

void AdcDevice::invalidateSamplesCache() {
#if defined(STM32F7XX)
	// The STM32F7xx has a data cache
	// DMA operations DO NOT invalidate cache lines, since the ARM m7 doesn't have 
	// anything like a CCI that maintains coherency across multiple bus masters.
	// As a result, we have to manually invalidate the D-cache any time we (the CPU)
	// would like to read something that somebody else wrote (ADC via DMA, in this case)
	SCB_InvalidateDCache_by_Addr(reinterpret_cast<uint32_t*>(samples), sizeof(*samples) * buf_len);
#endif /* STM32F7XX */
}

void AdcDevice::init(void) {
	hwConfig->num_channels = size();
	/* driver does this internally */
	//hwConfig->sqr1 += ADC_SQR1_NUM_CH(size());
}

bool AdcDevice::isHwUsed(adc_channel_e hwChannelIndex) const {
	for (size_t i = 0; i < channelCount; i++) {
		if (hardwareIndexByIndernalAdcIndex[i] == hwChannelIndex) {
			return true;
		}
	}
	return false;
}

void AdcDevice::enableChannel(adc_channel_e hwChannel) {
	if (channelCount >= efi::size(values.adc_data)) {
		firmwareError(OBD_PCM_Processor_Fault, "Too many ADC channels configured");
		return;
	}

	int logicChannel = channelCount++;

	size_t channelAdcIndex = hwChannel - 1;
#if defined(STM32F7XX)
	/* the temperature sensor is internally connected to ADC1_IN18 */
	if (hwChannel == EFI_ADC_TEMP_SENSOR)
		channelAdcIndex = 18;
#endif

	internalAdcIndexByHardwareIndex[hwChannel] = logicChannel;
	hardwareIndexByIndernalAdcIndex[logicChannel] = hwChannel;
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
}

void AdcDevice::enableChannelAndPin(const char *msg, adc_channel_e hwChannel) {
	enableChannel(hwChannel);

	brain_pin_e pin = getAdcChannelBrainPin(msg, hwChannel);
	efiSetPadMode(msg, pin, PAL_MODE_INPUT_ANALOG);
}

static void printAdcValue(int channel) {
	int value = getAdcValue("print", (adc_channel_e)channel);
	float volts = adcToVoltsDivided(value);
	scheduleMsg(&logger, "adc voltage : %.2f", volts);
}

adc_channel_e AdcDevice::getAdcHardwareIndexByInternalIndex(int index) const {
	return hardwareIndexByIndernalAdcIndex[index];
}

static void printFullAdcReport(Logging *logger) {
	scheduleMsg(logger, "fast %d slow %d", fastAdc.conversionCount, slowAdc.conversionCount);

	for (int index = 0; index < fastAdc.size(); index++) {
		appendMsgPrefix(logger);

		adc_channel_e hwIndex = fastAdc.getAdcHardwareIndexByInternalIndex(index);

		if (hwIndex != EFI_ADC_NONE && hwIndex != EFI_ADC_ERROR) {
			ioportid_t port = getAdcChannelPort("print", hwIndex);
			int pin = getAdcChannelPin(hwIndex);

			int adcValue = getAvgAdcValue(hwIndex, fastAdc.samples, ADC_BUF_DEPTH_FAST, fastAdc.size());
			logger->appendPrintf(" F ch%d %s%d", index, portname(port), pin);
			logger->appendPrintf(" ADC%d 12bit=%d", hwIndex, adcValue);
			float volts = adcToVolts(adcValue);
			logger->appendPrintf(" v=%.2f", volts);

			appendMsgPostfix(logger);
			scheduleLogging(logger);
		}
	}

	for (int index = 0; index < slowAdc.size(); index++) {
		appendMsgPrefix(logger);

		adc_channel_e hwIndex = slowAdc.getAdcHardwareIndexByInternalIndex(index);

		if (hwIndex != EFI_ADC_NONE && hwIndex != EFI_ADC_ERROR) {
			ioportid_t port = getAdcChannelPort("print", hwIndex);
			int pin = getAdcChannelPin(hwIndex);

			int adcValue = slowAdc.getAdcValueByIndex(index);
			logger->appendPrintf(" S ch%d %s%d", index, portname(port), pin);
			logger->appendPrintf(" ADC%d 12bit=%d", hwIndex, adcValue);
			float volts = adcToVolts(adcValue);
			logger->appendPrintf(" v=%.2f", volts);

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
	// don't halt the firmware if there are no slow channels assigned
	if (slowAdc.size() < 1)
		return;

	// we use slowAdcCounter instead of slowAdc.conversionCount because we need ADC_COMPLETE state
	// todo: use sync.objects?
	while (slowAdcCounter <= lastAdcCounter) {
		chThdSleepMilliseconds(1);
	}
}

int getSlowAdcCounter() {
	return slowAdcCounter;
}


class SlowAdcController : public PeriodicController<256> {
public:
	SlowAdcController() 
		: PeriodicController("ADC", NORMALPRIO + 5, SLOW_ADC_RATE)
	{
	}

	void PeriodicTask(efitick_t nowNt) override {
		{
			ScopePerf perf(PE::AdcConversionSlow);

			slowAdc.conversionCount++;
			msg_t result = adcConvert(&ADC_SLOW_DEVICE, &adcgrpcfgSlow, slowAdc.samples, ADC_BUF_DEPTH_SLOW);

			// If something went wrong - try again later
			if (result == MSG_RESET || result == MSG_TIMEOUT) {
				slowAdc.errorsCount++;
				return;
			}

#ifdef USE_ADC3_VBATT_HACK
			void proteusAdcHack();
			proteusAdcHack();
#endif
		}

		{
			ScopePerf perf(PE::AdcProcessSlow);

			slowAdc.invalidateSamplesCache();

			/* Calculates the average values from the ADC samples.*/
			for (int i = 0; i < slowAdc.size(); i++) {
				adcsample_t value = getAvgAdcValue(i, slowAdc.samples, ADC_BUF_DEPTH_SLOW, slowAdc.size());
				adcsample_t prev = slowAdc.values.adc_data[i];
				float result = (slowAdcCounter == 0) ? value :
						CONFIG(slowAdcAlpha) * value + (1 - CONFIG(slowAdcAlpha)) * prev;

				slowAdc.values.adc_data[i] = (adcsample_t)result;
			}
			slowAdcCounter++;

			AdcSubscription::UpdateSubscribers(nowNt);
		}
	}
};

void addChannel(const char *name, adc_channel_e setting, adc_channel_mode_e mode) {
	if (setting == EFI_ADC_NONE) {
		return;
	}
	if (/*type-limited (int)setting < 0 || */(int)setting>=HW_MAX_ADC_INDEX) {
		firmwareError(CUSTOM_INVALID_ADC, "Invalid ADC setting %s", name);
		return;
	}

	adcHwChannelEnabled[setting] = mode;

	AdcDevice& dev = (mode == ADC_SLOW) ? slowAdc : fastAdc;
	dev.enableChannelAndPin(name, setting);
}

void removeChannel(const char *name, adc_channel_e setting) {
	(void)name;
	if (setting == EFI_ADC_NONE) {
		return;
	}
	adcHwChannelEnabled[setting] = ADC_OFF;
}

static void configureInputs(void) {
	memset(adcHwChannelEnabled, 0, sizeof(adcHwChannelEnabled));

	/**
	 * order of analog channels here is totally random and has no meaning
	 * we also have some weird implementation with internal indices - that all has no meaning, it's just a random implementation
	 * which does not mean anything.
	 */

	addChannel("MAP", engineConfiguration->map.sensor.hwChannel, ADC_FAST);
	addChannel("MAF", engineConfiguration->mafAdcChannel, ADC_FAST);

	addChannel("HIP9011", engineConfiguration->hipOutputChannel, ADC_FAST);

	addChannel("Baro Press", engineConfiguration->baroSensor.hwChannel, ADC_SLOW);

	addChannel("TPS 1 Primary", engineConfiguration->tps1_1AdcChannel, ADC_SLOW);
	addChannel("TPS 1 Secondary", engineConfiguration->tps1_2AdcChannel, ADC_SLOW);
	addChannel("TPS 2 Primary", engineConfiguration->tps2_1AdcChannel, ADC_SLOW);
	addChannel("TPS 2 Secondary", engineConfiguration->tps2_2AdcChannel, ADC_SLOW);

	addChannel("Wastegate Position", engineConfiguration->wastegatePositionSensor, ADC_SLOW);
	addChannel("Idle Position Sensor", engineConfiguration->idlePositionSensor, ADC_SLOW);

	addChannel("Fuel Level", engineConfiguration->fuelLevelSensor, ADC_SLOW);
	addChannel("Acc Pedal1", engineConfiguration->throttlePedalPositionAdcChannel, ADC_SLOW);
	addChannel("Acc Pedal2", engineConfiguration->throttlePedalPositionSecondAdcChannel, ADC_SLOW);
	addChannel("VBatt", engineConfiguration->vbattAdcChannel, ADC_SLOW);
	// not currently used	addChannel("Vref", engineConfiguration->vRefAdcChannel, ADC_SLOW);
	addChannel("CLT", engineConfiguration->clt.adcChannel, ADC_SLOW);
	addChannel("IAT", engineConfiguration->iat.adcChannel, ADC_SLOW);
	addChannel("AUX Temp 1", engineConfiguration->auxTempSensor1.adcChannel, ADC_SLOW);
	addChannel("AUX Temp 2", engineConfiguration->auxTempSensor2.adcChannel, ADC_SLOW);

	addChannel("AUXF#1", engineConfiguration->auxFastSensor1_adcChannel, ADC_FAST);

	addChannel("AFR", engineConfiguration->afr.hwChannel, ADC_SLOW);
	addChannel("Oil Pressure", engineConfiguration->oilPressure.hwChannel, ADC_SLOW);

	addChannel("LFP", engineConfiguration->lowPressureFuel.hwChannel, ADC_SLOW);
	addChannel("HFP", engineConfiguration->highPressureFuel.hwChannel, ADC_SLOW);


	if (CONFIG(isCJ125Enabled)) {
		addChannel("CJ125 UR", engineConfiguration->cj125ur, ADC_SLOW);
		addChannel("CJ125 UA", engineConfiguration->cj125ua, ADC_SLOW);
	}

	for (int i = 0; i < FSIO_ANALOG_INPUT_COUNT ; i++) {
		addChannel("FSIOadc", engineConfiguration->fsioAdc[i], ADC_SLOW);
	}

	setAdcChannelOverrides();
}

static SlowAdcController slowAdcController;

void initAdcInputs() {
	scheduleMsg(&logger, "initAdcInputs()");
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

#if defined(ADC_CHANNEL_SENSOR)
	// Internal temperature sensor, Available on ADC1 only
	slowAdc.enableChannel(EFI_ADC_TEMP_SENSOR);
#endif /* ADC_CHANNEL_SENSOR */

	slowAdc.init();

	// Start the slow ADC thread
	slowAdcController.Start();

	fastAdc.init();
	/*
		* Initializes the PWM driver.
		*/
#if HAL_USE_GPT
	gptStart(EFI_INTERNAL_FAST_ADC_GPT, &fast_adc_config);
	gptStartContinuous(EFI_INTERNAL_FAST_ADC_GPT, GPT_PERIOD_FAST);
#endif /* HAL_USE_GPT */

	addConsoleActionI("adc", (VoidInt) printAdcValue);
#else
	scheduleMsg(&logger, "ADC disabled");
#endif
}

void printFullAdcReportIfNeeded(Logging *logger) {
	if (!adcDebugReporting)
		return;
	printFullAdcReport(logger);
}

#endif /* HAL_USE_ADC */
