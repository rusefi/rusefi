/**
 * @file flex_sensor.cpp
 */

#include "pch.h"
#include "flex_sensor.h"

void FlexSensor::callback(efitick_t nowNt, bool value) {
    latestCallbackTime = nowNt;
    flexCallbackCounter++;
	if (value) {
		if (gotRising) {
			frequency = 1 / flexFreq.getElapsedSecondsAndReset(nowNt);
			flexSensor.postRawValue(frequency, nowNt);
		} else {
			flexFreq.reset(nowNt);
		}

		// Start timing pulse width on rising edge
		flexPulse.reset(nowNt);
		// got rising edge
		gotRising = true;
	} else {
	    lowFlexCallbackCounter++;
	    if (gotRising) {
			// End pulse timing on falling edge
			pulseWidthUs = flexPulse.getElapsedUs(nowNt);

			if (pulseWidthUs < 900) {
				flexFuelTemp.invalidate(UnexpectedCode::Low);
				warning(ObdCode::CUSTOM_FLEX_LOW, "flex low %f", pulseWidthUs);
			} else if (pulseWidthUs > 5100) {
				flexFuelTemp.invalidate(UnexpectedCode::High);
				warning(ObdCode::CUSTOM_FLEX_HIGH, "flex high %f", pulseWidthUs);
			} else {
				// -40C = 1000us
				// 125C = 5000us
				float tempC = interpolateClamped(1000, -40, 5000, 125, pulseWidthUs);
				tempC = flexTempFilter.filter(tempC);
				flexFuelTemp.setValidValue(tempC, nowNt);
			}
		}
	}
#if EFI_PROD_CODE
	// TODO: confusing output channel name!
	engine->outputChannels.rawFlexFreq = flexCallbackCounter;
#endif // EFI_PROD_CODE
}

void FlexSensor::debug() {
	efiPrintf("flex counter %d", flexCallbackCounter);
	efiPrintf("lowFlexCallbackCounter counter %d", lowFlexCallbackCounter);
	efiPrintf("flex freq %f", frequency);
	efiPrintf("pulseWidthUs %f", pulseWidthUs);
	efiPrintf("latestCallbackTime %lld", latestCallbackTime);
}

void FlexSensor::Register(bool withTempSensor) {
	flexSensor.Register();

	if (withTempSensor) {
		flexFuelTemp.Register();
	}
}

void FlexSensor::unregister() {
	flexSensor.unregister();
	flexFuelTemp.unregister();
}
