/*
 * @file high_pressure_fuel_pump.cpp
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

#include "pch.h"

#include "high_pressure_fuel_pump.h"
#include "spark_logic.h"
#include "fuel_computer.h"

#if EFI_HPFP

angle_t HpfpLobe::findNextLobe() {
	// TODO: Ideally we figure out where we are in the engine cycle and pick the next lobe
	// based on that.  At least we should do that when cranking, so we can start that much
	// sooner.

	auto lobes = engineConfiguration->hpfpCamLobes;
	if (!lobes) {
		return 0;
	}

	// Which lobe are we on?
	int next_index = m_lobe_index + 1;
	// Note, this will be insufficient if the # of cam lobes is
	// dynamically changed rapidly by more than 2x, but it will
	// correct itself rather quickly.
	if (next_index >= lobes) {
		next_index -= lobes;
	}
	m_lobe_index = next_index;

	// Calculate impact of VVT
	angle_t vvt = 0;
	if (engineConfiguration->hpfpCam != HPFP_CAM_NONE) {
		// TODO: Is the sign correct here?  + means ATDC?
		vvt = engine->triggerCentral.getVVTPosition(
			(engineConfiguration->hpfpCam - 1) / 2 & 1, // Bank
			(engineConfiguration->hpfpCam - 1) & 1);    // Cam
	}

	return engineConfiguration->hpfpPeakPos + vvt + next_index * 720 / lobes;
}

// As a percent of the full pump stroke
float HpfpQuantity::calcFuelPercent(int rpm) {
	float fuel_requested_cc_per_cycle =
		engine->injectionMass[0] * (1.f / fuelDensity) * engineConfiguration->specs.cylindersCount;
	float fuel_requested_cc_per_lobe = fuel_requested_cc_per_cycle / engineConfiguration->hpfpCamLobes;
	return 100.f *
		fuel_requested_cc_per_lobe / engineConfiguration->hpfpPumpVolume +
		interpolate3d(engineConfiguration->hpfpCompensation,
			      engineConfiguration->hpfpCompensationLoadBins, fuel_requested_cc_per_lobe,
			      engineConfiguration->hpfpCompensationRpmBins, rpm);
}

float HpfpQuantity::calcPI(int rpm, float calc_fuel_percent) {
	m_pressureTarget_kPa = std::max<float>(
		m_pressureTarget_kPa - (engineConfiguration->hpfpTargetDecay *
					(FAST_CALLBACK_PERIOD_MS / 1000.)),
		interpolate3d(engineConfiguration->hpfpTarget,
			      engineConfiguration->hpfpTargetLoadBins, Sensor::get(SensorType::Map).Value, // TODO: allow other load axis, like we claim to
			      engineConfiguration->hpfpTargetRpmBins, rpm));

	float pressureError_kPa =
		m_pressureTarget_kPa - Sensor::get(SensorType::FuelPressureHigh).Value;

	float p_control_percent = pressureError_kPa * engineConfiguration->hpfpPidP;
	float i_factor_divisor =
		1000. * // ms/sec
		60. *   // sec/min -> ms/min
		2.;     // rev/cycle -> (rev * ms) / (min * cycle)
	float i_factor =
		engineConfiguration->hpfpPidI * // % / (kPa * lobe)
		rpm * // (% * revs) / (kPa * lobe * min)
		engineConfiguration->hpfpCamLobes * // lobes/cycle -> (% * revs) / (kPa * min * cycles)
		(FAST_CALLBACK_PERIOD_MS / // (% * revs * ms) / (kPa * min * cycles)
		 i_factor_divisor); // % / kPa
	float i_control_percent = m_I_sum_percent + pressureError_kPa * i_factor;
	calc_fuel_percent += p_control_percent;
	i_control_percent = clampF(-calc_fuel_percent, i_control_percent,
				   100.f - calc_fuel_percent);
	m_I_sum_percent = i_control_percent;
	return p_control_percent + i_control_percent;
}

angle_t HpfpQuantity::pumpAngleFuel(int rpm) {
	// Math based on fuel requested
	float fuel_requested_percent = calcFuelPercent(rpm);

	// Apply PI control
	fuel_requested_percent += calcPI(rpm, fuel_requested_percent);

	// Convert to degrees
	return interpolate2d(fuel_requested_percent,
			     engineConfiguration->hpfpLobeProfileQuantityBins,
			     engineConfiguration->hpfpLobeProfileAngle);
}

void HpfpController::onFastCallback() {
	// Pressure current/target calculation
	int rpm = engine->rpmCalculator.getRpm();

	// What conditions can we not handle?
	if (rpm < 60 ||
	    engineConfiguration->hpfpCamLobes == 0 ||
	    engineConfiguration->hpfpPumpVolume == 0 ||
	    !enginePins.hpfpValve.isInitialized()) {
		m_quantity.reset();
		m_requested_pump = 0;
		m_deadtime = 0;
	} else {
		// Convert deadtime from ms to degrees based on current RPM
		float deadtime_ms = interpolate2d(
			Sensor::get(SensorType::BatteryVoltage).value_or(VBAT_FALLBACK_VALUE),
			engineConfiguration->hpfpDeadtimeVoltsBins,
			engineConfiguration->hpfpDeadtimeMS);
		m_deadtime = deadtime_ms * rpm * (360.f / 60.f / 1000.f);

		// We set deadtime first, then pump, in case pump used to be 0.  Pump is what
		// determines whether we do anything or not.
		m_requested_pump = m_quantity.pumpAngleFuel(rpm);

		if (!m_running) {
			m_running = true;
			scheduleNextCycle();
		}
	}
}

void HpfpController::scheduleNextCycle() {
	if (!enginePins.hpfpValve.isInitialized()) {
		m_running = false;
		return;
	}

	angle_t lobe = m_lobe.findNextLobe();
	angle_t angle_requested = m_requested_pump;

	if (angle_requested > engineConfiguration->hpfpMinAngle) {
		engine->module<TriggerScheduler>()->scheduleOrQueue(
			&m_open, TRIGGER_EVENT_UNDEFINED, 0,
			lobe - angle_requested - m_deadtime,
			{ hpfpPinTurnOn, this });
	}

	// Always schedule this, even if we aren't opening the valve this
	// time, since this will schedule the next lobe.
	engine->module<TriggerScheduler>()->scheduleOrQueue(
		&m_close, TRIGGER_EVENT_UNDEFINED, 0,
		lobe - angle_requested + engineConfiguration->hpfpActivationAngle,
		{ hpfpPinTurnOff, this });
}

#endif // EFI_HPFP
