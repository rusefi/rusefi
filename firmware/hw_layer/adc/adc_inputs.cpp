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
#include "adc_onchip.h"
#include "adc_subscription.h"
#include "mpu_util.h"
#include "periodic_thread_controller.h"
#include "protected_gpio.h"

extern AdcSlowOnchipDevice slowAdc;
static uint32_t slowAdcConversionCount = 0;

#if EFI_USE_FAST_ADC
extern AdcFastOnchipDevice fastAdc;
#endif

static AdcTockenInternal adcHwChannelMode[EFI_ADC_TOTAL_CHANNELS];

// todo: move this flag to Engine god object
static int adcDebugReporting = false;

static int adcGetFirstDriver(uint16_t adc_mask)
{
	// return index of first bit set
	// return index of first ADC driver enabled for this hwChannel (few drivers can digitize same analog input)
	return (adc_channel_mode_e)(__builtin_ffs(adc_mask) - 1);
}

adc_channel_mode_e getAdcMode(adc_channel_e hwChannel) {
	uint16_t adc_mask = adcHwChannelMode[hwChannel].adc_mask;

	if (adc_mask == 0) {
		return ADC_OFF;
	}

	return (adc_channel_mode_e)adcGetFirstDriver(adc_mask);
}

static adcsample_t getAdcValueByToken(AdcTockenInternal token)
{
	int idx = adcGetFirstDriver(token.adc_mask);

	if (idx < 0) {
		warning(ObdCode::CUSTOM_OBD_ANALOG_INPUT_NOT_CONFIGURED, "Incalid ADC token");

		/* Is it a safe value? */
		return 0;
	}

	return adcDevices[idx]->getAvg(token.channel);
};

int getInternalAdcValue(const char *msg, adc_channel_e hwChannel) {
	if (!isAdcChannelValid(hwChannel)) {
		warning(ObdCode::CUSTOM_OBD_ANALOG_INPUT_NOT_CONFIGURED, "ADC: %s input is not configured", msg);
		return -1;
	}

	return getAdcValueByToken(adcHwChannelMode[hwChannel]);
}

AdcToken getAdcChannelToken(adc_channel_e hwChannel) {
	return adcHwChannelMode[hwChannel].token;
}

#if EFI_USE_FAST_ADC

AdcToken enableFastAdcChannel(const char *msg, adc_channel_e hwChannel) {
	if (!isAdcChannelValid(hwChannel)) {
		return invalidAdcToken;
	}

	if (adcHwChannelMode[hwChannel].adc_mask & BIT(ADC_FAST)) {
		warning(ObdCode::CUSTOM_OBD_ANALOG_INPUT_NOT_CONFIGURED, "ADC: %s input was not propery initialized", msg);
		return addChannel(msg, hwChannel, ADC_FAST);
	}

	return getAdcChannelToken(hwChannel);
}

adcsample_t getFastAdc(AdcToken token) {
	if (token == invalidAdcToken) {
		return 0;
	}

	AdcTockenInternal intToken = {
		.token = token
	};

	return getAdcValueByToken(intToken);
}

#endif

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

static void printAdcChannedReport(adc_channel_e hwChannel)
{
	if (isAdcChannelValid(hwChannel)) {
		if (getAdcMode(hwChannel) == ADC_OFF) {
			return;
		}

		ioportid_t port = getAdcChannelPort("print", hwChannel);
		int pin = getAdcChannelPin(hwChannel);
		int adcValue = getAdcValue("print", hwChannel);
		float volts = getVoltage("print", hwChannel);
		float voltsDivided = getVoltageDivided("print", hwChannel);
		/* Human index starts from 1 */
		efiPrintf(" %s ch[%2d] @ %s%d ADC%d 12bit=%4d %.3fV (input %.3fV)",
			getAdcMode(hwChannel) == ADC_FAST ? "F" : "S", hwChannel - EFI_ADC_0, portname(port), pin,
			/* TODO: */ hwChannel - EFI_ADC_0 + 1,
			adcValue, volts, voltsDivided);
	}
}

void printFullAdcReport(void) {
#if EFI_USE_FAST_ADC
	efiPrintf("fast %lu samples", fastAdc.conversionCount);
#endif // EFI_USE_FAST_ADC
	efiPrintf("slow %lu samples", slowAdcConversionCount);

	/* we assume that all slow ADC channels are enabled */
	for (int hwChannel = EFI_ADC_0; hwChannel <= EFI_ADC_TOTAL_CHANNELS; hwChannel++) {
		printAdcChannedReport(static_cast<adc_channel_e>(hwChannel));
	}
}

static void setAdcDebugReporting(int value) {
	adcDebugReporting = value;
	efiPrintf("adcDebug=%d", adcDebugReporting);
}

static float mcuTemperature;

float getMCUInternalTemperature() {
	return mcuTemperature;
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

			slowAdc.doConversion();

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

AdcToken addChannel(const char*, adc_channel_e hwChannel, adc_channel_mode_e mode) {
	if (!isAdcChannelValid(hwChannel)) {
		return invalidAdcToken;
	}

#if EFI_USE_FAST_ADC
	if (mode == ADC_FAST) {
		if (adcHwChannelMode[hwChannel].adc_mask & BIT(ADC_FAST)) {
			// Channel is already configured in correct mode
			return adcHwChannelMode[hwChannel].token;
		}

		int ret = fastAdc.enableChannel(hwChannel);
		if (ret >= 0) {
			adcHwChannelMode[hwChannel].channel = ret;
			adcHwChannelMode[hwChannel].adc_mask = BIT(mode);
		}
	}
#endif

	if (mode == ADC_SLOW) {
		int ret = getAdcInternalChannel(ADCD1.adc, hwChannel);
		if (ret >= 0) {
			adcHwChannelMode[hwChannel].channel = ret;
			adcHwChannelMode[hwChannel].adc_mask = BIT(mode);
		}
	}
	// Nothing to do for slow channels, input is mapped to analog in init_sensors.cpp

	return adcHwChannelMode[hwChannel].token;
}

AdcToken addFastAdcChannel(const char *msg, adc_channel_e hwChannel) {
	return addChannel(msg, hwChannel, ADC_FAST);
}

void removeChannel(const char*, adc_channel_e hwChannel) {
	if (!isAdcChannelValid(hwChannel)) {
		return;
	}
#if EFI_USE_FAST_ADC
	if (adcHwChannelMode[hwChannel].adc_mask & BIT(ADC_FAST)) {
		fastAdc.disableChannel(hwChannel);
	}
#endif

	adcHwChannelMode[hwChannel].adc_mask = 0;
}

// Weak link a stub so that every board doesn't have to implement this function
__attribute__((weak)) void setAdcChannelOverrides() { }

static void configureInputs() {
	memset(adcHwChannelMode, 0x0, sizeof(adcHwChannelMode));

	/* This is workaround for slow ADC lazy initialization - consumers does not call any init for channel before start using it */
	for (int hwChannel = EFI_ADC_0; hwChannel <= EFI_ADC_15; hwChannel++) {
		addChannel("auto", static_cast<adc_channel_e>(hwChannel), ADC_SLOW);
	}

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

#if EFI_USE_FAST_ADC
	// After this point fastAdc is not allowed to add channels
	fastAdc.init();
#endif // EFI_USE_FAST_ADC

	if (slowAdc.start() == 0) {
		adcDevices[ADC_SLOW] = &slowAdc;
	}

	// Start the slow ADC thread
	slowAdcController.start();

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
