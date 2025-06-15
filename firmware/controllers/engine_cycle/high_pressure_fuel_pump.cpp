/*
 * @file high_pressure_fuel_pump.cpp
 * @brief High Pressure Fuel Pump controller for GDI applications
 *
 * TL,DR: we have constant displacement mechanical pump driven by camshaft
 * here we control desired fuel high pressure by controlling relief/strain/spill valve electronically
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
#if !EFI_SHAFT_POSITION_INPUT
	fail("EFI_SHAFT_POSITION_INPUT required to have EFI_EMULATE_POSITION_SENSORS")
#endif

// A constant we use; doesn't seem important to hoist into engineConfiguration.
static constexpr int rpm_spinning_cutoff = 60; // Below this RPM, we don't run the logic

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
		// pump operates in cam-angle domain which is different speed from crank-angle domain on 4 stroke engines
		int mult = (int)getEngineCycle(getEngineRotationState()->getOperationMode()) / 360;
		int camIndex = engineConfiguration->hpfpCam - 1;
		// TODO: Is the sign correct here?  + means ATDC?
		vvt = engine->triggerCentral.getVVTPosition(
			BANK_BY_INDEX(camIndex),
			CAM_BY_INDEX(camIndex)) / mult;
	}

  // heh, looks like we assume 4 stroke here?
	return engineConfiguration->hpfpPeakPos + vvt + next_index * 720 / lobes;
}

// As a percent of the full pump stroke
float HpfpQuantity::calcFuelPercent(float rpm) {
	float fuel_requested_cc_per_cycle =
		engine->engineState.injectionMass[0] * (1.f / fuelDensity) * engineConfiguration->cylindersCount;
	float fuel_requested_cc_per_lobe = fuel_requested_cc_per_cycle / engineConfiguration->hpfpCamLobes;
	return 100.f *
		fuel_requested_cc_per_lobe / engineConfiguration->hpfpPumpVolume +
		interpolate3d(config->hpfpCompensation,
				config->hpfpCompensationLoadBins, fuel_requested_cc_per_lobe,
				config->hpfpCompensationRpmBins, rpm);
}

static float getLoad() {
    switch(engineConfiguration->fuelAlgorithm) {
    // TODO: allow other load axis, like we claim to
    case LM_ALPHA_N:
        return Sensor::getOrZero(SensorType::DriverThrottleIntent);
    default:
       return Sensor::getOrZero(SensorType::Map);
    }
}

float HpfpQuantity::calcPI(float rpm, float calc_fuel_percent, HpfpController *model) {
	float load = getLoad();

	float possibleValue = model->m_pressureTarget_kPa - (engineConfiguration->hpfpTargetDecay *
			(FAST_CALLBACK_PERIOD_MS / 1000.));

	model->m_pressureTarget_kPa = std::max<float>(possibleValue,
		interpolate3d(config->hpfpTarget,
				config->hpfpTargetLoadBins, load,
				config->hpfpTargetRpmBins, rpm));

	auto fuelPressure = Sensor::get(SensorType::FuelPressureHigh);
	if (!fuelPressure) {
		return 0;
	}

	float pressureError_kPa =
		model->m_pressureTarget_kPa - fuelPressure.Value;

	model->hpfp_p_control_percent = pressureError_kPa * engineConfiguration->hpfpPidP;
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
	float unclamped_i_control_percent = model->hpfp_i_control_percent + pressureError_kPa * i_factor;
	// Clamp the output so that calc_fuel_percent+i_control_percent is within 0% to 100%
	// That way the I term can override any fuel calculations over the long term.
	// The P term is still allowed to drive the total output over 100% or under 0% to react to
	// short term errors.
	model->hpfp_i_control_percent = clampF(-calc_fuel_percent, unclamped_i_control_percent,
				   100.f - calc_fuel_percent);
	return model->hpfp_p_control_percent + model->hpfp_i_control_percent;
}

angle_t HpfpQuantity::pumpAngleFuel(float rpm, HpfpController *model) {
	// Math based on fuel requested
	model->fuel_requested_percent = calcFuelPercent(rpm);

	model->fuel_requested_percent_pi = calcPI(rpm, model->fuel_requested_percent, model);
	// Apply PI control
	float fuel_requested_percentTotal = model->fuel_requested_percent + model->fuel_requested_percent_pi;

	// Convert to degrees
	return interpolate2d(fuel_requested_percentTotal,
			config->hpfpLobeProfileQuantityBins,
			config->hpfpLobeProfileAngle);
}

void HpfpController::onFastCallback() {
	// Pressure current/target calculation
	float rpm = Sensor::getOrZero(SensorType::Rpm);

	isHpfpActive = !(rpm < rpm_spinning_cutoff ||
		    !isGdiEngine() ||
		    engineConfiguration->hpfpPumpVolume == 0 ||
		    !enginePins.hpfpValve.isInitialized());
	// What conditions can we not handle?
	if (!isHpfpActive) {
		resetQuantity();
		m_requested_pump = 0;
		m_deadangle = 0;
	} else {
#if EFI_PROD_CODE && EFI_SHAFT_POSITION_INPUT
		criticalAssertVoid(engine->triggerCentral.triggerShape.getSize() > engineConfiguration->hpfpCamLobes * 6, "Too few trigger tooth for this number of HPFP lobes");
#endif // EFI_PROD_CODE
		// Convert deadtime from ms to degrees based on current RPM
		float deadtime_ms = interpolate2d(
			Sensor::get(SensorType::BatteryVoltage).value_or(VBAT_FALLBACK_VALUE),
			config->hpfpDeadtimeVoltsBins,
			config->hpfpDeadtimeMS);
		m_deadangle = deadtime_ms * rpm * (360.f / 60.f / 1000.f);

		// We set deadtime first, then pump, in case pump used to be 0.  Pump is what
		// determines whether we do anything or not.
		m_requested_pump = m_quantity.pumpAngleFuel(rpm, this);

		if (!m_running) {
			m_running = true;
			scheduleNextCycle();
		}
	}
}

#define HPFP_CONTROLLER "hpfp"

void HpfpController::pinTurnOn(HpfpController *self) {
	enginePins.hpfpValve.setHigh(HPFP_CONTROLLER);
	self->HpfpValveState = true;
	self->HpfdActivationPhase = getTriggerCentral()->getCurrentEnginePhase(getTimeNowNt()).value_or(-1);

	// By scheduling the close after we already open, we don't have to worry if the engine
	// stops, the valve will be turned off in a certain amount of time regardless.
	//
	// We do not need precise control of valve _duration_ since
	// For the solenoid type pump, the pump has a certain volume. You can activate the solenoid to request that the pump start pressurizing.
	// Once it reaches a certain pressure, it is effectively self running and won't unlatch until the pump reaches the top.
  // Since the solenoid latches itself, you don't have to keep it activated for the whole lobe. You just need to activate it until it latches and then let it do the rest of the work.
  // see also https://rusefi.com/forum/viewtopic.php?f=5&t=2192

	scheduleByAngle(&self->m_event.eventScheduling,
			self->m_event.eventScheduling.getMomentNt(),
			self->m_deadangle + engineConfiguration->hpfpActivationAngle,
			action_s::make<pinTurnOff>( self ));
}

void HpfpController::pinTurnOff(HpfpController *self) {
	enginePins.hpfpValve.setLow(HPFP_CONTROLLER);
	self->HpfpValveState = false;
	self->HpfdDeactivationPhase = getTriggerCentral()->getCurrentEnginePhase(getTimeNowNt()).value_or(-1);

	self->scheduleNextCycle();
}

void HpfpController::scheduleNextCycle() {
	noValve = !enginePins.hpfpValve.isInitialized();
	if (noValve) {
		m_running = false;
		return;
	}

	angle_t lobeAngle = m_lobe.findNextLobe();
	//TODO: integrate livedata into HpfpLobe
	nextLobe = m_lobe.m_lobe_index;
	angle_t angle_requested = m_requested_pump;

	angleAboveMin = angle_requested > engineConfiguration->hpfpMinAngle;
	if (angleAboveMin) {
		// TODO: some manuals suggest also substracting peak time (converted to angle)
		di_nextStart = lobeAngle - angle_requested - m_deadangle;
		wrapAngle(di_nextStart, "di_nextStart", ObdCode::CUSTOM_ERR_6557);


		/**
		 * We are good to use just one m_event instance because new events are scheduled when we turn off valve.
		 */
		engine->module<TriggerScheduler>()->schedule(
		    "hpfp",
			&m_event,
			di_nextStart,
			action_s::make<pinTurnOn>( this ));

		// Off will be scheduled after turning the valve on
	} else {
	    wrapAngle(lobeAngle, "lobe", ObdCode::CUSTOM_ERR_6557);
		// Schedule this, even if we aren't opening the valve this time, since this
		// will schedule the next lobe.
		// todo: schedule only 'scheduleNextCycle' directly since we do not need output control part of the logic here!
		engine->module<TriggerScheduler>()->schedule(
			HPFP_CONTROLLER,
			&m_event, lobeAngle,
			action_s::make<pinTurnOff>( this ));
	}
}

#endif // EFI_HPFP

bool isGdiEngine() {
#if EFI_PROD_CODE
    return enginePins.hpfpValve.isInitialized();
#else
    return engineConfiguration->hpfpCamLobes > 0;
#endif
}
