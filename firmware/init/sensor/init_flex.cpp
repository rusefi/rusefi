#include "pch.h"

#include "init.h"
#include "functional_sensor.h"
#include "flex_sensor.h"

#include "digital_input_exti.h"

static FunctionalSensor flexSensor(SensorType::FuelEthanolPercent, MS2NT(500));
static StoredValueSensor flexFuelTemp(SensorType::FuelTemperature, MS2NT(500));
static FlexConverter converter;

static Biquad flexTempFilter;

static Timer flexFreq, flexPulse;

static int flexCallbackCounter = 0;
static int lowFlexCallbackCounter = 0;
static float frequency;
static float pulseWidthUs;
static efitick_t latestCallbackTime;

static void flexCallback(efitick_t nowNt, bool value) {
    latestCallbackTime = nowNt;
    flexCallbackCounter++;
	if (value) {
		frequency = 1 / flexFreq.getElapsedSecondsAndReset(nowNt);
		flexSensor.postRawValue(frequency, nowNt);

		// Start timing pulse width on rising edge
		flexPulse.reset(nowNt);
	} else {
	    lowFlexCallbackCounter++;
		// End pulse timing on falling edge
		pulseWidthUs = flexPulse.getElapsedUs(nowNt);

		if (pulseWidthUs < 900) {
			flexFuelTemp.invalidate(UnexpectedCode::Low);
			warning(ObdCode::CUSTOM_OBD_6003, "flex low %f", pulseWidthUs);
		} else if (pulseWidthUs > 5100) {
			flexFuelTemp.invalidate(UnexpectedCode::High);
			warning(ObdCode::CUSTOM_OBD_6004, "flex high %f", pulseWidthUs);
		} else {
			// -40C = 1000us
			// 125C = 5000us
			float tempC = interpolateClamped(1000, -40, 5000, 125, pulseWidthUs);
			tempC = flexTempFilter.filter(tempC);
			flexFuelTemp.setValidValue(tempC, nowNt);
		}
	}
}

static Gpio flexPin = Gpio::Unassigned;

static void flexExtiCallback(void*, efitick_t nowNt) {
#if EFI_PROD_CODE
	flexCallback(nowNt, efiReadPin(flexPin) ^ engineConfiguration->flexSensorInverted);
#endif
}

// https://rusefi.com/forum/viewtopic.php?p=37452#p37452

void initFlexSensor(bool isFirstTime) {
#if EFI_PROD_CODE
	if (efiExtiEnablePin("flex", engineConfiguration->flexSensorPin,
		PAL_EVENT_MODE_BOTH_EDGES, flexExtiCallback, nullptr) < 0) {
		return;
	}
#endif
	flexPin = engineConfiguration->flexSensorPin;

	// 0.01 means filter bandwidth of ~1hz with ~100hz sensor
	flexTempFilter.configureLowpass(1, 0.01f);
	flexSensor.setFunction(converter);

#if EFI_PROD_CODE
    if (isFirstTime) {
	    addConsoleAction("flexinfo", []() {
	        efiPrintf("flex counter %d", flexCallbackCounter);
	        efiPrintf("lowFlexCallbackCounter counter %d", lowFlexCallbackCounter);
	        efiPrintf("flex freq %f", frequency);
	        efiPrintf("pulseWidthUs %f", pulseWidthUs);
	        efiPrintf("latestCallbackTime %d", latestCallbackTime);
	    });
	}

#endif // EFI_PROD_CODE

	flexSensor.Register();

	// If an analog fuel temp sensor is configured, don't use the flex sensor for fuel temp
	if (!isAdcChannelValid(engineConfiguration->fuelTempSensor.adcChannel)) {
		flexFuelTemp.Register();
	}
}

void deInitFlexSensor() {
	flexSensor.unregister();
	flexFuelTemp.unregister();

	if (!isBrainPinValid(flexPin)) {
		return;
	}

#if EFI_PROD_CODE
	efiExtiDisablePin(flexPin);
#endif

	flexPin = Gpio::Unassigned;
}
