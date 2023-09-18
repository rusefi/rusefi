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

static void flexCallback(efitick_t nowNt, bool value) {
	if (value) {
		float frequency = 1 / flexFreq.getElapsedSecondsAndReset(nowNt);
		flexSensor.postRawValue(frequency, nowNt);

		// Start timing pulse width on rising edge
		flexPulse.reset(nowNt);
	} else {
		// End pulse timing on falling edge
		float pulseWidthUs = flexPulse.getElapsedUs(nowNt);

		if (pulseWidthUs < 900) {
			flexFuelTemp.invalidate(UnexpectedCode::Low);
		} else if (pulseWidthUs > 5100) {
			flexFuelTemp.invalidate(UnexpectedCode::High);
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

static void flexCallback(void*, efitick_t nowNt) {
#if EFI_PROD_CODE
	flexCallback(nowNt, efiReadPin(flexPin));
#endif
}

// https://rusefi.com/forum/viewtopic.php?p=37452&sid=829804c90d5b2e1fecd1b900cf1b1811#p37452

void initFlexSensor() {
	auto flexPin = engineConfiguration->flexSensorPin;
	if (!isBrainPinValid(flexPin)) {
		return;
	}

	// 0.01 means filter bandwidth of ~1hz with ~100hz sensor
	flexTempFilter.configureLowpass(1, 0.01f);

#if EFI_PROD_CODE
	efiExtiEnablePin("flex", flexPin,
		PAL_EVENT_MODE_BOTH_EDGES,
		flexCallback, nullptr);
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
