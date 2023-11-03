#include "pch.h"

#include "hella_oil_level.h"

#include "digital_input_exti.h"

static void hellaSensorExtiCallback(void* arg, efitick_t nowNt) {
	reinterpret_cast<HellaOilLevelSensor*>(arg)->onEdge(nowNt);
}

void HellaOilLevelSensor::init(brain_pin_e pin) {
	if (!isBrainPinValid(pin)) {
		return;
	}

	m_pin = pin;

#if EFI_PROD_CODE
	efiExtiEnablePin(getSensorName(), pin, 
		PAL_EVENT_MODE_BOTH_EDGES,
		hellaSensorExtiCallback, reinterpret_cast<void*>(this));
#endif // EFI_PROD_CODE

	Register();
}

void HellaOilLevelSensor::onEdge(efitick_t nowNt) {
#if EFI_PROD_CODE
	onEdge(nowNt, efiReadPin(m_pin));
#endif
}

void HellaOilLevelSensor::onEdge(efitick_t nowNt, bool value) {
	if (value) {
		// Start pulse width timing at the rising edge
		m_pulseTimer.reset(nowNt);

		float timeBetweenPulses = m_betweenPulseTimer.getElapsedSecondsAndReset(nowNt);

		if (timeBetweenPulses > 0.89 * 0.780 && timeBetweenPulses < 1.11 * 0.780) {
			// 780ms nominal between Diag and next Temp pulse start, +-10%

			// This was the "long gap" break, next pulse is temperature.
			m_nextPulse = NextPulse::Temp;
		} else if (timeBetweenPulses > 0.89 * 0.110 && timeBetweenPulses < 1.11 * 0.110) {
			// 110ms nominal between each pulse (other than break)

			// Advance the state machine to decode the next pulse in the sequence
			switch (m_nextPulse) {
				case NextPulse::Temp:
					m_nextPulse = NextPulse::Level;
					break;
				case NextPulse::Level:
					m_nextPulse = NextPulse::Diag;
					break;
				default:
					// We don't know how we got here, reset to safe state
					m_nextPulse = NextPulse::None;
					break;
			}
		} else {
			// The break was too long, ignore it for now.
			m_nextPulse = NextPulse::None;
		}
	} else {
		// Stop timing at the falling edge
		float lastPulseMs = 1000 * m_pulseTimer.getElapsedSeconds(nowNt);

		if (lastPulseMs > 100 || lastPulseMs < 20) {
			// Impossibly short or long pulse, something went wrong
			m_nextPulse = NextPulse::None;
			return;
		}

		if (m_nextPulse == NextPulse::Diag) {
			// TODO: decode diag pulse?
			return;
		} else if (m_nextPulse == NextPulse::Temp) {
			// 22ms = Short circuit temp sensor
			// 23ms = -40C
			// 87ms = 160C
			// 88ms = Temp sensor defective

			if (lastPulseMs < 22.8) {
				// Short circuit
				// invalidate(UnexpectedCode::Low);
			} else if (lastPulseMs > 87.2) {
				// Defective
				// invalidate(UnexpectedCode::High);
			} else {
				// float tempC = interpolateClamped(23, -40, 87, 160, lastPulseMs);
				// setValidValue(tempC, nowNt);
			}
		} else if (m_nextPulse == NextPulse::Level) {
			// 22ms = Unreliable signal
			// 23ms = level 0mm
			// 87.86ms = level 150mm

			if (lastPulseMs < 22.8f) {
				// Unreliable
				invalidate(UnexpectedCode::Low);
			} else {
				float levelMm = interpolateClamped(23, 0, 87.86, 150, lastPulseMs);
				setValidValue(levelMm, nowNt);
			}
		}
	}
}
