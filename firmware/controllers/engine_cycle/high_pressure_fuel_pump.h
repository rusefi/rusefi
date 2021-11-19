/*
 * @file high_pressure_fuel_pump.h
 * @brief High Pressure Fuel Pump controller for GDI applications
 *
 * @date Nov 6, 2021
 * @author Scott Smith, (c) 2021
 */

/*
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

class HpfpLobe {
public:
	uint8_t m_lobe_index = 0; ///< 0-based index of the last lobe returned

	angle_t findNextLobe(); ///< Calculate the angle (after crank TDC) for the top of the next lobe
};

class HpfpQuantity {
public:
	float m_I_sum_percent = 0;
	float m_pressureTarget_kPa = 0;

	/**
	 * Calculate where the pump should become active, in degrees before pump lobe TDC
	 */
	angle_t pumpAngleFuel(int rpm);

	/**
	 * Calculate the percent of the pump stroke needed to replace the fuel injected.  Also
	 * includes pump compensation calculations.
	 *
	 * This is used by internal tests and shouldn't be called directly.  Instead use
	 * pumpAngleFuel.
	 *
	 * Return value is nominally 0-100, but may be outside that range (including negative) if
	 * model parameters are not accurate.
	 */
	float calcFuelPercent(int rpm);

	/**
	 * Calculates the PI controller contribution as a percent.  This amount should be added to
	 * calcFuelPercent() above.
	 *
	 * This is used by internal tests and shouldn't be called directly.  Instead use
	 * pumpAngleFuel.
	 *
	 * Return value is nominally 0-100, but may be outside that range (including negative) if
	 * model parameters are not accurate.  The sum of this and calc_fuel_percent will be 0-100.
	 */
	float calcPI(int rpm, float calc_fuel_percent);

	/**
	 * Reset internal state due to a stopped engine.
	 */
	void reset() {
		m_I_sum_percent = 0;
		m_pressureTarget_kPa = 0;
	}
};

class HpfpController {
public:
	void onFastCallback();

private:
	AngleBasedEvent m_open;
	AngleBasedEvent m_close;

	HpfpLobe     m_lobe;
	HpfpQuantity m_quantity;

	volatile angle_t m_requested_pump = 0; ///< Computed requested pump duration in degrees (not including deadtime)
	volatile angle_t m_deadtime = 0; ///< Computed solenoid deadtime in degrees
	volatile bool m_running = false; ///< Whether events are being scheduled or not

	void scheduleNextCycle();

	static void hpfpPinTurnOn(HpfpController *) {
		enginePins.hpfpValve.setHigh();
	}

	static void hpfpPinTurnOff(HpfpController *self) {
		enginePins.hpfpValve.setLow();

		self->scheduleNextCycle();
	}
};
