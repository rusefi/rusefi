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

float PUBLIC_API_WEAK boardAdjustVoltage(float voltage, adc_channel_e /* hwChannel */) {
	// a hack useful when we do not trust voltage just after board EN was turned on. is this just hiding electrical design flaws?
	return voltage;
}

/* overall analog health state
 * return negative in case of any problems
 * return 0 if everything is ok or no diagnostic is available */
int PUBLIC_API_WEAK boardGetAnalogDiagnostic() {
	return 0;
}

/* simple implementation if board does not provide advanced diagnostic */
int PUBLIC_API_WEAK boardGetAnalogInputDiagnostic(adc_channel_e channel, float) {
#if EFI_PROD_CODE
	/* for on-chip ADC inputs we check common analog health */
	if (isAdcChannelOnChip(channel)) {
		return boardGetAnalogDiagnostic();
	}
#endif // EFI_PROD_CODE

	/* input is outside chip/ECU */
	return 0;
}

/* Get analog part diagnostic */
int analogGetDiagnostic()
{
	/* TODO: debounce? */
	return boardGetAnalogDiagnostic();
}

#if HAL_USE_ADC

#include "adc_subscription.h"
#include "AdcDevice.h"
#include "mpu_util.h"
#include "protected_gpio.h"

extern AdcDevice fastAdc;

/* TODO: Drop NO_CACHE for F4 and F7 couse with ADCv2 driver CPU does averaging and CPU stores result to this array */
/* TODO: store summ of samples is this array and divide on oversample factor only when converting to float - this will increase accuracity */
static volatile NO_CACHE adcsample_t slowAdcSamples[SLOW_ADC_CHANNEL_COUNT];

static uint32_t slowAdcConversionCount = 0;

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
	int adcValue = adcGetRawValue("print", (adc_channel_e)channel);
	float voltsInput = adcRawValueToScaledVoltage(adcValue, (adc_channel_e)channel);
	efiPrintf("adc %d input %.3fV", channel, voltsInput);
}

static void printAdcChannedReport(const char *prefix, int internalIndex, adc_channel_e hwChannel)
{
	if (isAdcChannelValid(hwChannel)) {
		ioportid_t port = getAdcChannelPort("print", hwChannel);
		int pin = getAdcChannelPin(hwChannel);
		int adcValue = adcGetRawValue("print", hwChannel);
		float volts = adcGetRawVoltage("print", hwChannel);
		float voltsInput = adcGetScaledVoltage("print", hwChannel);
		/* Human index starts from 1 */
		efiPrintf(" %s ch[%2d] @ %s%d ADC%d 12bit=%4d %.3fV input %.3fV",
			prefix, internalIndex, portname(port), pin,
			/* TODO: */ hwChannel - EFI_ADC_0 + 1,
			adcValue, volts, voltsInput);
	}
}

void printFullAdcReport(void) {
#if EFI_USE_FAST_ADC
	efiPrintf("fast %u samples", engine->outputChannels.fastAdcConversionCount);

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

void updateSlowAdc(efitick_t nowNt) {
	{
		ScopePerf perf(PE::AdcConversionSlow);

		/* drop volatile type qualifier - this is safe */
		if (!readSlowAnalogInputs((adcsample_t *)slowAdcSamples)) {
			engine->outputChannels.slowAdcErrorCount++;
			return;
		}

		// Ask the port to sample the MCU temperature
		mcuTemperature = getMcuTemperature();
		if (mcuTemperature > 150.0f || mcuTemperature < -50.0f) {
			/*
			 * we have a sporadic issue with this check todo https://github.com/rusefi/rusefi/issues/2552
			 */
			//criticalError("Invalid CPU temperature measured %f", degrees);
		}
	}

	{
		ScopePerf perf(PE::AdcProcessSlow);

		slowAdcConversionCount++;

		AdcSubscription::UpdateSubscribers(nowNt);

		protectedGpio_check(nowNt);
	}
}

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

void initAdcInputs() {
	efiPrintf("initAdcInputs()");

	configureInputs();

	// migrate to 'enable adcdebug'
	addConsoleActionI("adcdebug", &setAdcDebugReporting);

#if EFI_INTERNAL_ADC
	portInitAdc();

#if EFI_USE_FAST_ADC
	// After this point fastAdc is not allowed to add channels
	fastAdc.init();
#endif // EFI_USE_FAST_ADC

	addConsoleActionI("adc", (VoidInt) printAdcValue);
#else // ! EFI_INTERNAL_ADC
	efiPrintf("ADC disabled");
#endif // EFI_INTERNAL_ADC

	// Workaround to pre-feed all sensors with some data...
	chThdSleepMilliseconds(1);
	updateSlowAdc(getTimeNowNt());
}

void printFullAdcReportIfNeeded(void) {
	if (!adcDebugReporting)
		return;
	printFullAdcReport();
}

#else /* not HAL_USE_ADC */

// voltage in MCU universe, from zero to VDD
__attribute__((weak)) float adcGetRawVoltage(const char*, adc_channel_e) {
	return 0;
}

// voltage in ECU universe, with all input dividers and OpAmps gains taken into account, voltage at ECU connector pin
__attribute__((weak)) float adcGetScaledVoltage(const char*, adc_channel_e) {
	return 0;
}

#endif
