/**
 * @file pulse_width_sensor.cpp
 *
 * Captures duty cycle from a fixed-frequency PWM signal using EXTI interrupts on
 * both edges.  The duty cycle [0, 1] is passed through a SensorConverter so callers
 * can linearise it to any physical unit.
 *
 * Both "always-low" (no edges) and "always-high" (rising but never falling) failure
 * modes result in no valid readings being posted, which causes FunctionalSensor's
 * built-in timeout to invalidate the sensor automatically.
 */

#include "pch.h"

#include "pulse_width_sensor.h"

#if EFI_PROD_CODE
#include "digital_input_exti.h"

static void pulseWidthSensorExtiCallback(void* arg, efitick_t nowNt) {
	reinterpret_cast<PulseWidthSensor*>(arg)->onEdge(nowNt);
}
#endif // EFI_PROD_CODE

void PulseWidthSensor::initIfValid(brain_pin_e pin, SensorConverter &converter) {
	if (!isBrainPinValid(pin)) {
		return;
	}

	setFunction(converter);

#if EFI_PROD_CODE
	if (efiExtiEnablePin(getSensorName(), pin, PAL_EVENT_MODE_BOTH_EDGES,
			pulseWidthSensorExtiCallback, reinterpret_cast<void*>(this)) < 0) {
		return;
	}
#endif // EFI_PROD_CODE

	m_pin = pin;

	Register();
}

void PulseWidthSensor::deInit() {
	if (!isBrainPinValid(m_pin)) {
		return;
	}

#if EFI_PROD_CODE
	efiExtiDisablePin(m_pin);
#endif

	m_pin = Gpio::Unassigned;
}

void PulseWidthSensor::onEdge(efitick_t nowNt) {
	eventCounter++;

#if EFI_PROD_CODE
	bool isHigh = efiReadPin(m_pin);
#else
	// In simulator / unit-test builds the real pin cannot be read; track the
	// edge direction with a simple toggle so tests can drive onEdge() manually.
	m_lastWasHigh = !m_lastWasHigh;
	bool isHigh = m_lastWasHigh;
#endif

	if (isHigh) {
		// ---- Rising edge -------------------------------------------------------
		// Capture the period from the previous rising edge (= full signal period).
		// On the very first edge we just initialise the timer and wait for the next
		// cycle before we trust the period value.
		if (m_seenRisingEdge) {
			m_lastPeriodSec = m_periodTimer.getElapsedSecondsAndReset(nowNt);
		} else {
			m_periodTimer.reset(nowNt);
			m_seenRisingEdge = true;
		}

		// Begin timing the high portion of the pulse
		m_pulseWidthTimer.reset(nowNt);
	} else {
		// ---- Falling edge -------------------------------------------------------
		if (!m_seenRisingEdge) {
			// Signal started low before we saw the first rising edge – ignore.
			return;
		}

		float pulseWidthSec = m_pulseWidthTimer.getElapsedSeconds(nowNt);
		float periodSec     = m_lastPeriodSec;

		// Require at least one complete cycle to have elapsed (period > 0) and
		// that the period falls within the configured frequency window.
		if (periodSec <= 0.0f
		    || periodSec < m_minPeriodSec
		    || periodSec > m_maxPeriodSec) {
			// Frequency out of range or no previous rising edge yet – discard this
			// reading but keep the timers running so we recover automatically.
			return;
		}

		// Duty cycle is defined as high-time / period.
		float dutyCycle = pulseWidthSec / periodSec;

		// Clamp to [0, 1] to absorb jitter at the extremes.
		dutyCycle = clampF(0.0f, dutyCycle, 1.0f);

		postRawValue(dutyCycle, nowNt);
	}
}

void PulseWidthSensor::showInfo(const char* sensorName) const {
	efiPrintf("PWM duty sensor \"%s\": pin=%s events=%d lastPeriod=%.4fs",
	          sensorName,
	          hwPortname(m_pin),
	          eventCounter,
	          m_lastPeriodSec);
}
