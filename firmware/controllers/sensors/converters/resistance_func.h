/**
 * @author Matthew Kennedy, (c) 2019
 * 
 * A function to convert input voltage to resistance in a voltage divider.
 * Configured with the value of the pullup resistor, and the voltage to which
 * it's connected.
 */

#pragma once

#include "sensor_converter_func.h"

class ResistanceFunc final : public SensorConverter {
public:
	void configure(float supplyVoltage, float pullupResistor, bool isPulldown);

	/**
	 * Battery-tracking bias mode (m74_9 VTRK1/2 = VBATT/2.5): the divider's
	 * supply voltage is not a fixed rail but the measured battery voltage
	 * times `ratio` (0.4 = VBATT/2.5). The fixed voltage configured via
	 * configure() is kept as the fallback for the first ~100 ms after boot,
	 * while the battery sensor is not valid yet. ratio 0 = fixed supply.
	 */
	void configureTrackingBias(float ratio, float pullupResistor, bool isPulldown);

	SensorResult convert(float inputValue) const override;

	void showInfo(float testInputValue) const override;

	// Last resistance computed by convert(), in ohms. Zero if the last conversion failed
	// (dead short / open circuit) or no conversion happened yet. Exposed so the measured
	// resistance can be shown live during thermistor calibration.
	float getLastResistance() const {
		return m_lastResistance;
	}

private:
	float m_supplyVoltage = 5.0f;
	float m_pullupResistor = 1000.0f;
	bool m_isPulldown = false;

	// >0: supply = measured VBATT x ratio (battery-tracking bias rail).
	float m_trackingRatio = 0;

	// convert() is const as it's logically a pure conversion; caching the last result for
	// monitoring purposes doesn't change that, hence mutable.
	mutable float m_lastResistance = 0;
};
