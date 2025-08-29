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
ObdCode PUBLIC_API_WEAK boardGetAnalogDiagnostic() {
	return ObdCode::None;
}

/* simple implementation if board does not provide advanced diagnostic */
int PUBLIC_API_WEAK boardGetAnalogInputDiagnostic(adc_channel_e channel, float) {
#if EFI_PROD_CODE
	/* for on-chip ADC inputs we check common analog health */
	if (isAdcChannelOnChip(channel)) {
		return (boardGetAnalogDiagnostic() == ObdCode::None) ? 0 : -1;
	}
#endif // EFI_PROD_CODE

	/* input is outside chip/ECU */
	return 0;
}

static ObdCode analogGetVrefDiagnostic()
{
#if HAL_USE_ADC
	float vref = getMCUVref();

	// TODO: +/-10% is way too big?
	if (vref > engineConfiguration->adcVcc * 1.1) {
		return ObdCode::OBD_Sensor_Refence_Voltate_A_High;
	}

	if (vref < engineConfiguration->adcVcc * 0.9) {
		return ObdCode::OBD_Sensor_Refence_Voltate_A_Low;
	}
#endif

	return ObdCode::None;
}

/* Get analog part diagnostic */
ObdCode analogGetDiagnostic()
{
	/* TODO: debounce? */
	auto code = analogGetVrefDiagnostic();
	if (code != ObdCode::None) {
		return code;
	}

	return boardGetAnalogDiagnostic();
}

#if HAL_USE_ADC

#include "adc_device.h"
#include "adc_subscription.h"
#include "mpu_util.h"
#include "protected_gpio.h"

// voltage in MCU universe, from zero to Vref
expected<float> adcGetRawVoltage(const char *msg, adc_channel_e hwChannel) {
	float rawVoltage = adcRawValueToRawVoltage(adcGetRawValue(msg, hwChannel));
	int inputStatus = boardGetAnalogInputDiagnostic(hwChannel, rawVoltage);

	if (inputStatus == 0) {
		return expected(rawVoltage);
	}

	/* TODO: convert inputStatus to unexpected? */
	return unexpected;
}

// voltage in ECU universe, with all input dividers and OpAmps gains taken into account, voltage at ECU connector pin
expected<float> adcGetScaledVoltage(const char *msg, adc_channel_e hwChannel) {
	auto rawVoltage = adcGetRawVoltage(msg, hwChannel);

	if (rawVoltage) {
		// TODO: merge getAnalogInputDividerCoefficient() and boardAdjustVoltage() into single board hook?
		float voltage = rawVoltage.value_or(0) * getAnalogInputDividerCoefficient(hwChannel);
		return expected(boardAdjustVoltage(voltage, hwChannel));
	}

	return expected(rawVoltage);
}

extern AdcDevice fastAdc;

static AdcChannelMode adcHwChannelMode[EFI_ADC_TOTAL_CHANNELS];

// todo: move this flag to Engine god object
static int adcDebugReporting = false;

AdcChannelMode getAdcMode(adc_channel_e hwChannel) {
	return adcHwChannelMode[hwChannel];
}

extern adcsample_t adcOnchipSlowGetAvgRaw(adc_channel_e hwChannel);

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

	return adcOnchipSlowGetAvgRaw(hwChannel);
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

void adcPrintChannelReport(const char *prefix, int internalIndex, adc_channel_e hwChannel)
{
	if (isAdcChannelValid(hwChannel)) {
		ioportid_t port = getAdcChannelPort("print", hwChannel);
		int pin = getAdcChannelPin(hwChannel);
		int adcValue = adcGetRawValue("print", hwChannel);
		auto volts = adcGetRawVoltage("print", hwChannel);
		auto voltsInput = adcGetScaledVoltage("print", hwChannel);
		/* Human index starts from 1 */
		efiPrintf(" %s ch[%2d] @ %s%d ADC%d 12bit=%4d %.3fV input %.3fV %s",
			prefix, internalIndex, portname(port), pin,
			/* TODO: */ hwChannel - EFI_ADC_0 + 1,
			adcValue, volts.value_or(0), voltsInput.value_or(0), volts ? "valid" : "INVALID");
	}
}

extern void adcOnchipSlowShowReport();

void printFullAdcReport(void) {
#if EFI_USE_FAST_ADC
	efiPrintf("fast %u samples", engine->outputChannels.fastAdcConversionCount);

	for (int internalIndex = 0; internalIndex < fastAdc.size(); internalIndex++) {
		adc_channel_e hwChannel = fastAdc.getAdcChannelByInternalIndex(internalIndex);

		adcPrintChannelReport("F", internalIndex, hwChannel);
	}
#endif // EFI_USE_FAST_ADC

	adcOnchipSlowShowReport();
}

static void setAdcDebugReporting(int value) {
	adcDebugReporting = value;
	efiPrintf("adcDebug=%d", adcDebugReporting);
}

extern void adcOnchipSlowUpdate(efitick_t nowNt);

void adcInputsUpdateSubscribers(efitick_t nowNt) {
	adcOnchipSlowUpdate(nowNt);

	{
		ScopePerf perf(PE::AdcProcessSlow);

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

void initAdcInputs() {
	efiPrintf("initAdcInputs()");

	configureInputs();

	// migrate to 'enable adcdebug'
	addConsoleActionI("adcdebug", &setAdcDebugReporting);

#if EFI_INTERNAL_ADC
	// This will start HW for all used ADCs
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
	adcInputsUpdateSubscribers(getTimeNowNt());
}

void printFullAdcReportIfNeeded(void) {
	if (!adcDebugReporting)
		return;
	printFullAdcReport();
}

#else /* not HAL_USE_ADC */

// voltage in MCU universe, from zero to VDD
__attribute__((weak)) expected<float> adcGetRawVoltage(const char*, adc_channel_e) {
	return expected(0.0f);
}

// voltage in ECU universe, with all input dividers and OpAmps gains taken into account, voltage at ECU connector pin
__attribute__((weak)) expected<float> adcGetScaledVoltage(const char*, adc_channel_e) {
	return expected(0.0f);
}

#endif
