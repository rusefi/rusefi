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

#include "pch.h"

float __attribute__((weak)) getAnalogInputDividerCoefficient(adc_channel_e) {
    return engineConfiguration->analogInputDividerCoefficient;
}

#if HAL_USE_ADC

#include "adc_subscription.h"
#include "AdcConfiguration.h"
#include "mpu_util.h"
#include "periodic_thread_controller.h"
#include "protected_gpio.h"

/* Depth of the conversion buffer, channels are sampled X times each.*/
#ifndef ADC_BUF_DEPTH_FAST
#define ADC_BUF_DEPTH_FAST      4
#endif

static NO_CACHE adcsample_t slowAdcSamples[SLOW_ADC_CHANNEL_COUNT];

static AdcChannelMode adcHwChannelEnabled[HW_MAX_ADC_INDEX];

// Board voltage, with divider coefficient accounted for
float getVoltageDivided(const char *msg, adc_channel_e hwChannel) {
	return getVoltage(msg, hwChannel) * getAnalogInputDividerCoefficient(hwChannel);
}

// voltage in MCU universe, from zero to VDD
float getVoltage(const char *msg, adc_channel_e hwChannel) {
	return adcToVolts(getAdcValue(msg, hwChannel));
}

#if EFI_USE_FAST_ADC
AdcDevice::AdcDevice(ADCConversionGroup* hwConfig, adcsample_t *buf, size_t buf_len)
	: m_samples(buf)
	, m_hwConfig(hwConfig)
	, m_buf_len(buf_len)
{
	m_hwConfig->sqr1 = 0;
	m_hwConfig->sqr2 = 0;
	m_hwConfig->sqr3 = 0;
#if ADC_MAX_CHANNELS_COUNT > 16
	m_hwConfig->sqr4 = 0;
	m_hwConfig->sqr5 = 0;
#endif /* ADC_MAX_CHANNELS_COUNT */
	memset(hardwareIndexByIndernalAdcIndex, EFI_ADC_NONE, sizeof(hardwareIndexByIndernalAdcIndex));
	memset(internalAdcIndexByHardwareIndex, 0xFF, sizeof(internalAdcIndexByHardwareIndex));
}

#endif // EFI_USE_FAST_ADC

// is there a reason to have this configurable at runtime?
#ifndef ADC_FAST_DEVICE
#define ADC_FAST_DEVICE ADCD2
#endif /* ADC_FAST_DEVICE */

static uint32_t slowAdcCounter = 0;

static adcsample_t getAvgAdcValue(int index, adcsample_t *samples, int bufDepth, int numChannels) {
	uint32_t result = 0;
	for (int i = 0; i < bufDepth; i++) {
		result += samples[index];
		index += numChannels;
	}

	// this truncation is guaranteed to not be lossy - the average can't be larger than adcsample_t
	return static_cast<adcsample_t>(result / bufDepth);
}


// See https://github.com/rusefi/rusefi/issues/976 for discussion on this value
#define ADC_SAMPLING_FAST ADC_SAMPLE_28

#if EFI_USE_FAST_ADC
static void adc_callback_fast(ADCDriver *adcp) {
	// State may not be complete if we get a callback for "half done"
	if (adcp->state == ADC_COMPLETE) {
		onFastAdcComplete(adcp->samples);
	}
}

static ADCConversionGroup adcgrpcfgFast = {
	.circular			= FALSE,
	.num_channels		= 0,
	.end_cb				= adc_callback_fast,
	.error_cb			= nullptr,
	/* HW dependent part.*/
	.cr1				= 0,
	.cr2				= ADC_CR2_SWSTART,
	// Configure sample time for all channels. We'll only actually use
	// one or two (MAP sensors, etc), but setting sample time for unused
	// channels doesn't do anything.
	.smpr1 =
		ADC_SMPR1_SMP_AN10(ADC_SAMPLING_FAST) |
		ADC_SMPR1_SMP_AN11(ADC_SAMPLING_FAST) |
		ADC_SMPR1_SMP_AN12(ADC_SAMPLING_FAST) |
		ADC_SMPR1_SMP_AN13(ADC_SAMPLING_FAST) |
		ADC_SMPR1_SMP_AN14(ADC_SAMPLING_FAST) |
		ADC_SMPR1_SMP_AN15(ADC_SAMPLING_FAST),
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
};

static NO_CACHE adcsample_t fastAdcSampleBuf[ADC_BUF_DEPTH_FAST * ADC_MAX_CHANNELS_COUNT];
AdcDevice fastAdc(&adcgrpcfgFast, fastAdcSampleBuf, efi::size(fastAdcSampleBuf));

static void fast_adc_callback(GPTDriver*) {
#if EFI_INTERNAL_ADC
	chibios_rt::CriticalSectionLocker csl;

	if (ADC_FAST_DEVICE.state != ADC_READY &&
		ADC_FAST_DEVICE.state != ADC_COMPLETE &&
		ADC_FAST_DEVICE.state != ADC_ERROR) {
		fastAdc.errorsCount++;
		return;
	}

	adcStartConversionI(&ADC_FAST_DEVICE, &adcgrpcfgFast, fastAdc.m_samples, ADC_BUF_DEPTH_FAST);
	fastAdc.conversionCount++;
#endif /* EFI_INTERNAL_ADC */
}
#endif // EFI_USE_FAST_ADC

static float mcuTemperature;

float getMCUInternalTemperature() {
	return mcuTemperature;
}

int getInternalAdcValue(const char *msg, adc_channel_e hwChannel) {
	if (!isAdcChannelValid(hwChannel)) {
		warning(ObdCode::CUSTOM_OBD_ANALOG_INPUT_NOT_CONFIGURED, "ADC: %s input is not configured", msg);
		return -1;
	}

#if EFI_USE_FAST_ADC
	if (adcHwChannelEnabled[hwChannel] == AdcChannelMode::Fast) {
		int internalIndex = fastAdc.internalAdcIndexByHardwareIndex[hwChannel];
// todo if ADC_BUF_DEPTH_FAST EQ 1
//		return fastAdc.samples[internalIndex];
		int value = getAvgAdcValue(internalIndex, fastAdc.m_samples, ADC_BUF_DEPTH_FAST, fastAdc.size());
		return value;
	}
#endif // EFI_USE_FAST_ADC

	return slowAdcSamples[hwChannel - EFI_ADC_0];
}

#if EFI_USE_FAST_ADC
static GPTConfig fast_adc_config = {
	GPT_FREQ_FAST,
	fast_adc_callback,
	0, 0
};
#endif /* EFI_USE_FAST_ADC */

#if EFI_USE_FAST_ADC

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

void AdcDevice::init() {
	m_hwConfig->num_channels = size();
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
	if ((channelCount + 1) >= ADC_MAX_CHANNELS_COUNT) {
		firmwareError(ObdCode::OBD_PCM_Processor_Fault, "Too many ADC channels configured");
		return;
	}

	// hwChannel = which external pin are we using
	// adcChannelIndex = which ADC channel are we using
	// adcIndex = which index does that get in sampling order
	size_t adcChannelIndex = hwChannel - EFI_ADC_0;
	size_t adcIndex = channelCount++;

	internalAdcIndexByHardwareIndex[hwChannel] = adcIndex;
	hardwareIndexByIndernalAdcIndex[adcIndex] = hwChannel;

	if (adcIndex < 6) {
		m_hwConfig->sqr3 |= adcChannelIndex << (5 * adcIndex);
	} else if (adcIndex < 12) {
		m_hwConfig->sqr2 |= adcChannelIndex << (5 * (adcIndex - 6));
	} else if (adcIndex < 18) {
		m_hwConfig->sqr1 |= adcChannelIndex << (5 * (adcIndex - 12));
	}
}

adc_channel_e AdcDevice::getAdcHardwareIndexByInternalIndex(int index) const {
	return hardwareIndexByIndernalAdcIndex[index];
}

#endif // EFI_USE_FAST_ADC

static void printAdcValue(int channel) {
	int value = getAdcValue("print", (adc_channel_e)channel);
	float volts = adcToVoltsDivided(value, (adc_channel_e)channel);
	efiPrintf("adc voltage : %.2f", volts);
}

void waitForSlowAdc(uint32_t lastAdcCounter) {
	// we use slowAdcCounter instead of slowAdc.conversionCount because we need ADC_COMPLETE state
	// todo: use sync.objects?
	while (slowAdcCounter <= lastAdcCounter) {
		chThdSleepMilliseconds(1);
	}
}

class SlowAdcController : public PeriodicController<256> {
public:
	SlowAdcController() 
		: PeriodicController("ADC", PRIO_ADC, SLOW_ADC_RATE)
	{
	}

	void PeriodicTask(efitick_t nowNt) override {
		{
			ScopePerf perf(PE::AdcConversionSlow);

			if (!readSlowAnalogInputs(slowAdcSamples)) {
				return;
			}

			// Ask the port to sample the MCU temperature
			mcuTemperature = getMcuTemperature();
		}

		{
			ScopePerf perf(PE::AdcProcessSlow);

			slowAdcCounter++;

			AdcSubscription::UpdateSubscribers(nowNt);

			protectedGpio_check(nowNt);
		}
	}
};

void addFastAdcChannel(const char* /*name*/, adc_channel_e setting) {
	if (!isAdcChannelValid(setting)) {
		return;
	}

	adcHwChannelEnabled[setting] = AdcChannelMode::Fast;

#if EFI_USE_FAST_ADC
	fastAdc.enableChannel(setting);
#endif
}

void removeFastAdcChannel(const char *name, adc_channel_e setting) {
	(void)name;
	if (!isAdcChannelValid(setting)) {
		return;
	}

	adcHwChannelEnabled[setting] = AdcChannelMode::Off;
}

// Weak link a stub so that every board doesn't have to implement this function
__attribute__((weak)) void setAdcChannelOverrides() { }

static CCM_OPTIONAL SlowAdcController slowAdcController;

void initAdcInputs() {
	efiPrintf("initAdcInputs()");

	memset(adcHwChannelEnabled, 0, sizeof(adcHwChannelEnabled));

	addFastAdcChannel("MAP", engineConfiguration->map.sensor.hwChannel);
	addFastAdcChannel("AUXF#1", engineConfiguration->auxFastSensor1_adcChannel);

	setAdcChannelOverrides();

#if EFI_INTERNAL_ADC
	portInitAdc();

	// Start the slow ADC thread
	slowAdcController.start();

#if EFI_USE_FAST_ADC
	fastAdc.init();

	gptStart(EFI_INTERNAL_FAST_ADC_GPT, &fast_adc_config);
	gptStartContinuous(EFI_INTERNAL_FAST_ADC_GPT, GPT_PERIOD_FAST);
#endif // EFI_USE_FAST_ADC

	addConsoleActionI("adc", (VoidInt) printAdcValue);
#endif
}

#else /* not HAL_USE_ADC */

__attribute__((weak)) float getVoltageDivided(const char*, adc_channel_e) {
	return 0;
}

// voltage in MCU universe, from zero to VDD
__attribute__((weak)) float getVoltage(const char*, adc_channel_e) {
	return 0;
}

#endif
