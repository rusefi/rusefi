/**
 * @file pulse_width_sensor.h
 *
 * Sensor that reads duty cycle from a fixed-frequency PWM signal (e.g. 100–200 Hz).
 * The raw value posted to the FunctionalSensor pipeline is the duty cycle in the
 * range [0, 1]. A SensorConverter then maps it to the desired physical quantity.
 *
 * Failure modes handled:
 *  - Signal always LOW  – no edges arrive, FunctionalSensor timeout invalidates the sensor.
 *  - Signal always HIGH – a rising edge is recorded but no falling edge follows within
 *                         the expected period window, so no valid reading is ever posted
 *                         and the FunctionalSensor timeout invalidates the sensor.
 *  - Frequency out of range – readings from cycles whose period falls outside
 *                              [minFreqHz, maxFreqHz] are silently discarded.
 */
#pragma once

#include "functional_sensor.h"
#include <rusefi/timer.h>

class PulseWidthSensor : public FunctionalSensor {
public:
	/**
	 * @param type          sensor type
	 * @param timeoutPeriod how long without a valid reading before the sensor is
	 *                      considered failed (passed straight to FunctionalSensor)
	 * @param minFreqHz     minimum expected signal frequency [Hz] – cycles slower than
	 *                      this are discarded as out-of-range
	 * @param maxFreqHz     maximum expected signal frequency [Hz] – cycles faster than
	 *                      this are discarded as out-of-range
	 */
	PulseWidthSensor(SensorType type, efidur_t timeoutPeriod,
	                 float minFreqHz = 50.0f, float maxFreqHz = 500.0f)
		: FunctionalSensor(type, timeoutPeriod)
		, m_minPeriodSec(1.0f / maxFreqHz)
		, m_maxPeriodSec(1.0f / minFreqHz)
	{ }

	/**
	 * Configure and register the sensor. Does nothing if @p pin is not valid.
	 *
	 * @param pin       brain pin wired to the PWM signal
	 * @param converter maps raw duty cycle (0–1) to the physical sensor value
	 */
	void initIfValid(brain_pin_e pin, SensorConverter &converter);

	/** Disable the EXTI interrupt and unregister the sensor. */
	void deInit();

	void showInfo(const char* sensorName) const override;

	/**
	 * Called by the EXTI interrupt on every edge (both rising and falling).
	 * Public so the static trampoline callback can reach it.
	 */
	void onEdge(efitick_t nowNt);

	/** Diagnostic: total edge count since init. */
	int eventCounter = 0;

private:
	// Reciprocal frequency limits stored as period limits for faster comparison
	const float m_minPeriodSec;
	const float m_maxPeriodSec;

	brain_pin_e m_pin = Gpio::Unassigned;

	// Measures elapsed time from the rising edge to the falling edge (= high pulse width)
	Timer m_pulseWidthTimer;

	// Measures elapsed time from rising edge to rising edge (= signal period)
	Timer m_periodTimer;

	// Period of the most-recently completed full cycle (rising→rising), seconds
	float m_lastPeriodSec = 0;

	// True once at least one rising edge has been observed
	bool m_seenRisingEdge = false;

#if !EFI_PROD_CODE
	// Simulator / unit-test: tracks simulated edge direction since efiReadPin
	// is unavailable in those builds.
	bool m_lastWasHigh = false;
#endif
};
