/**
 * @file	adc_inputs.cpp
 * @brief	Low level ADC code
 *
 * @date Jan 14, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

float PUBLIC_API_WEAK getAnalogInputDividerCoefficient(adc_channel_e) {
    return engineConfiguration->analogInputDividerCoefficient;
}

#if HAL_USE_ADC

#include "adc_device.h"
#include "adc_subscription.h"
#include "mpu_util.h"
#include "periodic_thread_controller.h"
#include "protected_gpio.h"

extern AdcDevice fastAdc;

static volatile NO_CACHE adcsample_t slowAdcSamples[SLOW_ADC_CHANNEL_COUNT];

static uint32_t slowAdcConversionCount = 0;
static uint32_t slowAdcErrorsCount = 0;

static float mcuTemperature;

static AdcChannelMode adcHwChannelMode[EFI_ADC_TOTAL_CHANNELS];

// todo: move this flag to Engine god object
static int adcDebugReporting = false;

AdcChannelMode getAdcMode(adc_channel_e hwChannel) {
	return adcHwChannelMode[hwChannel];
}

float getMCUInternalTemperature() {
	return mcuTemperature;
}

int getInternalAdcValue(const char *msg, adc_channel_e hwChannel) {
	if (!isAdcChannelValid(hwChannel)) {
		warning(ObdCode::CUSTOM_OBD_ANALOG_INPUT_NOT_CONFIGURED, "ADC: %s input is not configured", msg);
		return -1;
	}

#if EFI_USE_FAST_ADC
	if (adcHwChannelMode[hwChannel] == AdcChannelMode::Fast) {
		return fastAdc.getAvgAdcValue(hwChannel);
	}
#endif // EFI_USE_FAST_ADC

	return slowAdcSamples[hwChannel - EFI_ADC_0];
}

static void printAdcValue(int channel) {
	/* Do this check before conversion to adc_channel_e that is uint8_t based */
	if ((channel < EFI_ADC_NONE) || (channel >= EFI_ADC_TOTAL_CHANNELS)) {
		efiPrintf("Invalid ADC channel %d", channel);
		return;
	}
	int value = getAdcValue("print", (adc_channel_e)channel);
	float volts = adcToVoltsDivided(value, (adc_channel_e)channel);
	efiPrintf("adc %d voltage : %.3f", channel, volts);
}

static void printAdcChannedReport(const char *prefix, int internalIndex, adc_channel_e hwChannel)
{
	if (isAdcChannelValid(hwChannel)) {
		ioportid_t port = getAdcChannelPort("print", hwChannel);
		int pin = getAdcChannelPin(hwChannel);
		int adcValue = getAdcValue("print", hwChannel);
		float volts = getVoltage("print", hwChannel);
		float voltsDivided = getVoltageDivided("print", hwChannel);
		/* Human index starts from 1 */
		efiPrintf(" %s ch[%2d] @ %s%d ADC%d 12bit=%4d %.3fV (input %.3fV)",
			prefix, internalIndex, portname(port), pin,
			/* TODO: */ hwChannel - EFI_ADC_0 + 1,
			adcValue, volts, voltsDivided);
	}
}

void printFullAdcReport(void) {
#if EFI_USE_FAST_ADC
	efiPrintf("fast %lu samples", fastAdc.conversionCount);

	for (int internalIndex = 0; internalIndex < fastAdc.size(); internalIndex++) {
		adc_channel_e hwChannel = fastAdc.getAdcChannelByInternalIndex(internalIndex);

		printAdcChannedReport("F", internalIndex, hwChannel);
	}
#endif // EFI_USE_FAST_ADC
	efiPrintf("slow %lu samples", slowAdcConversionCount);

	/* we assume that all slow ADC channels are enabled */
	for (int internalIndex = 0; internalIndex < ADC_MAX_CHANNELS_COUNT; internalIndex++) {
		adc_channel_e hwChannel = static_cast<adc_channel_e>(internalIndex + EFI_ADC_0);

		printAdcChannedReport("S", internalIndex, hwChannel);
	}
}

static void setAdcDebugReporting(int value) {
	adcDebugReporting = value;
	efiPrintf("adcDebug=%d", adcDebugReporting);
}

class SlowAdcController : public PeriodicController<UTILITY_THREAD_STACK_SIZE> {
public:
	SlowAdcController()
		: PeriodicController("ADC", PRIO_ADC, SLOW_ADC_RATE)
	{
	}

	void PeriodicTask(efitick_t nowNt) override {
		{
			ScopePerf perf(PE::AdcConversionSlow);

			/* drop volatile type qualifier - this is safe */
			if (!readSlowAnalogInputs((adcsample_t *)slowAdcSamples)) {
				slowAdcErrorsCount++;
				return;
			}

			// Ask the port to sample the MCU temperature
			mcuTemperature = getMcuTemperature();
		}

		{
			ScopePerf perf(PE::AdcProcessSlow);

			AdcSubscription::UpdateSubscribers(nowNt);

			slowAdcConversionCount++;

			protectedGpio_check(nowNt);
		}
	}
};

void addFastAdcChannel(const char*, adc_channel_e hwChannel) {
	if (!isAdcChannelValid(hwChannel)) {
		return;
	}

#if EFI_USE_FAST_ADC
	fastAdc.enableChannel(hwChannel);
#endif

	adcHwChannelMode[hwChannel] = AdcChannelMode::Fast;
	// Nothing to do for slow channels, input is mapped to analog in init_sensors.cpp
}

void removeChannel(const char*, adc_channel_e hwChannel) {
	if (!isAdcChannelValid(hwChannel)) {
		return;
	}
#if EFI_USE_FAST_ADC
	if (adcHwChannelMode[hwChannel] == AdcChannelMode::Fast) {
		/* TODO: */
		//fastAdc.disableChannel(hwChannel);
	}
#endif

	adcHwChannelMode[hwChannel] = AdcChannelMode::Off;
}

// Weak link a stub so that every board doesn't have to implement this function
__attribute__((weak)) void setAdcChannelOverrides() { }

static void configureInputs() {
	memset(adcHwChannelMode, (int)AdcChannelMode::Off, sizeof(adcHwChannelMode));

	/**
	 * order of analog channels here is totally random and has no meaning
	 * we also have some weird implementation with internal indices - that all has no meaning, it's just a random implementation
	 * which does not mean anything.
	 */

	addFastAdcChannel("MAP", engineConfiguration->map.sensor.hwChannel);

	addFastAdcChannel("HIP9011", engineConfiguration->hipOutputChannel);

	// not currently used	addFastAdcChannel("Vref", engineConfiguration->vRefAdcChannel, ADC_SLOW);

	addFastAdcChannel("AUXF#1", engineConfiguration->auxFastSensor1_adcChannel);

	setAdcChannelOverrides();
}

void waitForSlowAdc(uint32_t lastAdcCounter) {
	// note that having ADC reading is one thing while having new sensor API is a totally different thing!
	// todo: use sync.objects?
	while (slowAdcConversionCount <= lastAdcCounter) {
		chThdSleepMilliseconds(1);
	}
}

static SlowAdcController slowAdcController;

void initAdcInputs() {
	efiPrintf("initAdcInputs()");

	configureInputs();

	// migrate to 'enable adcdebug'
	addConsoleActionI("adcdebug", &setAdcDebugReporting);

#if EFI_INTERNAL_ADC
	portInitAdc();

	// Start the slow ADC thread
	slowAdcController.start();

#if EFI_USE_FAST_ADC
	// After this point fastAdc is not allowed to add channels
	fastAdc.init();
#endif // EFI_USE_FAST_ADC

	addConsoleActionI("adc", (VoidInt) printAdcValue);
#else // ! EFI_INTERNAL_ADC
	efiPrintf("ADC disabled");
#endif // EFI_INTERNAL_ADC
}

void printFullAdcReportIfNeeded(void) {
	if (!adcDebugReporting)
		return;
	printFullAdcReport();
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
