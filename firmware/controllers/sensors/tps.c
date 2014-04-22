#include "main.h"
#include "boards.h"
#include "engine_configuration.h"
#include "interpolation.h"
#include "adc_inputs.h"

extern engine_configuration_s *engineConfiguration;

/*
 * Return current TPS position based on configured ADC levels, and adc
 *
 * */
float getTpsValue(int adc) {
	if (adc < engineConfiguration->tpsMin)
		return 0;
	if (adc > engineConfiguration->tpsMax)
		return 100;
	return interpolate(engineConfiguration->tpsMin, 0, engineConfiguration->tpsMax, 100, adc);
}

/*
 * Return voltage on TPS AND channel
 * */
float getTPSVoltage(void) {
	return getVoltageDivided(engineConfiguration->tpsAdcChannel);
}

/*
 * Return TPS ADC readings.
 * We need ADC value because TunerStudio has a nice TPS configuration wizard, and this wizard
 * wants a TPS value.
 */
int getTPS10bitAdc(void) {
	int adc = getAdcValue(engineConfiguration->tpsAdcChannel);
	return (int) adc / 4; // Only for TunerStudio compatibility. Max TS adc value in 1023
}

/**
 * @brief Position on physical primary TPS
 */
static float getPrimatyRawTPS(void) {
	// blue, 1st board
	/* PA7 - blue TP */
	float tpsValue = getTpsValue(getTPS10bitAdc());
	return tpsValue;
}

// todo: static float getSecondaryRawTPS

/*
 * In case of dual TPS this function would return logical TPS position
 *
 * @return Current TPS position. 0 means idle and 100 means Wide Open Throttle
 */
float getTPS(void) {
	// todo: if (config->isDualTps)
	// todo: blah blah
	// todo: if two TPS do not match - show OBD code via malfunction_central.c

	return getPrimatyRawTPS();
}

