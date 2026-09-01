/**
 * @file    main_trigger_callback.cpp
 * @brief   Main logic is here!
 *
 * See http://rusefi.com/docs/html/
 *
 * @date Feb 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
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

#if EFI_PRINTF_FUEL_DETAILS
	bool printFuelDebug = false;
#endif // EFI_PRINTF_FUEL_DETAILS

#if EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT

#include "main_trigger_callback.h"
#include "trigger_central.h"
#include "spark_logic.h"
#include "advance_map.h"
#include "cyclic_buffer.h"
#include "fuel_math.h"
#include "cdm_ion_sense.h"
#include "local_version_holder.h"
#include "event_queue.h"
#include "injector_model.h"
#include "injection_gpio.h"
#include "angle_clock.h"

#if EFI_LAUNCH_CONTROL
#include "launch_control.h"
#endif // EFI_LAUNCH_CONTROL

#include "backup_ram.h"

void endSimultaneousInjection(InjectionEvent* event) {
	endSimultaneousInjectionOnlyTogglePins();
	event->update();
}

void turnInjectionPinLow(InjectionEvent *event) {
	efitick_t nowNt = getTimeNowNt();

	for (size_t i = 0; i < efi::size(event->outputs); i++) {
		InjectorOutputPin *output = event->outputs[i];
		if (output) {
			output->close(nowNt);
		}
	}
	event->update();
}

static void turnInjectionPinLowStage2(InjectionEvent* event) {
	efitick_t nowNt = getTimeNowNt();

	for (size_t i = 0; i < efi::size(event->outputsStage2); i++) {
		InjectorOutputPin *output = event->outputsStage2[i];
		if (output) {
			output->close(nowNt);
		}
	}
}

void InjectionEvent::onTriggerTooth(efitick_t nowNt, float currentPhase, float nextPhase, float nextNextPhase) {
#if !EFI_ANGLE_CLOCK
	UNUSED(nextNextPhase);
#endif // !EFI_ANGLE_CLOCK
	auto eventAngle = injectionStartAngle;

	// A previous arm whose target is still in the future covers this window
	// (the one-tooth-ahead arm of the previous tooth, or the time-based arm
	// of the same cycle): do not schedule twice. Once the armed target has
	// passed, the arm has fired (or was lost) and a window match is the NEXT
	// cycle's scheduling - at high duty the next window can arrive before
	// the injection end recomputes the angle, and the pre-existing behavior
	// schedules with the stale angle.
	if (injectionStartArmed && (injectionStartArmedAt - nowNt) > 0) {
		return;
	}

	// The injection start is scheduled one tooth ahead: the early window
	// [nextPhase, nextNextPhase) gives the handoff a full tooth of slack
	// instead of the old 0-1 tooth lead. The current-tooth window is kept for
	// the first cycle after a (re)sync, when the start angle can already be
	// inside the current tooth.
#if EFI_ANGLE_CLOCK
	// The early window is only meaningful with a distinct next-next tooth:
	// useOnlyRisingEdges wheels store the falling edge at the preceding
	// rise's angle, so nextNextPhase == nextPhase and isPhaseInRange would
	// match every angle (the arm would refuse every tooth). Fall back to the
	// current-tooth window, as the fire arm's guard already does.
	bool scheduleEarly = nextNextPhase != nextPhase && isPhaseInRange(eventAngle, nextPhase, nextNextPhase);
#else
	bool scheduleEarly = false;
#endif // EFI_ANGLE_CLOCK
	bool scheduleNow = isPhaseInRange(eventAngle, currentPhase, nextPhase);

	if (!scheduleEarly && !scheduleNow) {
		return;
	}

#if ROTATIONAL_IDLE_CONTROLLER
	if (engine->rotationalIdleController.shouldSkipFuelRotationalIdle()) {
		return;
	}
#endif // ROTATIONAL_IDLE_CONTROLLER

	// Select fuel mass from the correct cylinder
	auto injectionMassGrams = getEngineState()->injectionMass[this->cylinderNumber];

	// Perform wall wetting adjustment on fuel mass, not duration, so that
	// it's correct during fuel pressure (injector flow) or battery voltage (deadtime) transients
	// TODO: is it correct to wall wet on both pulses?
	injectionMassGrams = wallFuel.adjust(injectionMassGrams);

	// Disable staging in simultaneous mode
	float stage2Fraction = isSimultaneous ? 0 : getEngineState()->injectionStage2Fraction;

	// Compute fraction of fuel on stage 2, remainder goes on stage 1
	const float injectionMassStage2 = stage2Fraction * injectionMassGrams;
	float injectionMassStage1 = injectionMassGrams - injectionMassStage2;

#if EFI_VEHICLE_SPEED
	{
		// Log this fuel as consumed

		bool isCranking = getEngineRotationState()->isCranking();
		int numberOfInjections = isCranking ? getNumberOfInjections(engineConfiguration->crankingInjectionMode) : getNumberOfInjections(engineConfiguration->injectionMode);

		float actualInjectedMass = numberOfInjections * (injectionMassStage1 + injectionMassStage2);

#ifdef MODULE_ODOMETER
		engine->module<TripOdometer>()->consumeFuel(actualInjectedMass, nowNt);
#endif // MODULE_ODOMETER

	}
#endif // EFI_VEHICLE_SPEED

	const floatms_t injectionDurationStage1 = engine->module<InjectorModelPrimary>()->getInjectionDuration(injectionMassStage1);
	const floatms_t injectionDurationStage2 = injectionMassStage2 > 0 ? engine->module<InjectorModelSecondary>()->getInjectionDuration(injectionMassStage2) : 0;

#if EFI_PRINTF_FUEL_DETAILS
	if (printFuelDebug) {
		printf("fuel injectionDuration=%.2fms adjusted=%.2fms\n",
				getEngineState()->injectionDuration,
		  injectionDurationStage1);
	}
#endif /*EFI_PRINTF_FUEL_DETAILS */

	if (this->cylinderNumber == 0) {
		if (engine->outputChannels.actualLastInjection) {
			engine->outputChannels.actualLastInjectionRatio = injectionDurationStage1 / engine->outputChannels.actualLastInjection;
		} else {
			engine->outputChannels.actualLastInjectionRatio = 0;
		}
		if (engine->outputChannels.actualLastInjectionStage2) {
			engine->outputChannels.actualLastInjectionRatioStage2 = injectionDurationStage2 / engine->outputChannels.actualLastInjectionStage2;
		} else {
			engine->outputChannels.actualLastInjectionRatioStage2 = 0;
		}
		engine->outputChannels.actualLastInjection = injectionDurationStage1;
		engine->outputChannels.actualLastInjectionStage2 = injectionDurationStage2;
	}

	if (std::isnan(injectionDurationStage1) || std::isnan(injectionDurationStage2)) {
		warning(ObdCode::CUSTOM_OBD_NAN_INJECTION, "NaN injection pulse");
		return;
	}
	if (injectionDurationStage1 < 0) {
		warning(ObdCode::CUSTOM_OBD_NEG_INJECTION, "Negative injection pulse %.2f", injectionDurationStage1);
		return;
	}

	// If somebody commanded an impossibly short injection, do nothing.
	// Durations under 50us-ish aren't safe for the scheduler
	// as their order may be swapped, resulting in a stuck open injector
	// see https://github.com/rusefi/rusefi/pull/596 for more details
	if (injectionDurationStage1 < 0.050f)
	{
		// Zero duration means zero commanded fuel mass - an intentional "no injection"
		// (deceleration fuel cut-off, zero-fuel transients), not a misconfiguration.
		// Only a nonzero-but-impossibly-short pulse deserves a warning. See #9874.
		if (injectionDurationStage1 > 0) {
			warning(ObdCode::CUSTOM_OBD_impossibly_short_INJECTION, "Short pulse %.2f", injectionDurationStage1);
		}
		return;
	}

	floatus_t durationUsStage1 = MS2US(injectionDurationStage1);
	floatus_t durationUsStage2 = MS2US(injectionDurationStage2);

	// Only bother with the second stage if it's long enough to be relevant
	bool hasStage2Injection = durationUsStage2 > 50;

#if EFI_PRINTF_FUEL_DETAILS
	if (printFuelDebug) {
		InjectorOutputPin *output = outputs[0];
		printf("handleFuelInjectionEvent fuelout %s injection_duration %dus engineCycleDuration=%.1fms\t\n", output->getName(), (int)durationUsStage1,
				(int)MS2US(getCrankshaftRevolutionTimeMs(Sensor::getOrZero(SensorType::Rpm))) / 1000.0);
	}
#endif /*EFI_PRINTF_FUEL_DETAILS */

	action_s startAction, endActionStage1, endActionStage2;
	// We use different callbacks based on whether we're running sequential mode or not - everything else is the same
	if (isSimultaneous) {
		startAction = action_s::make<startSimultaneousInjection>();
		endActionStage1 = action_s::make<endSimultaneousInjection>(this);
	} else {
		auto const taggedPointer{TaggedPointer<decltype(this)>::make(this, hasStage2Injection)};

		// sequential or batch
		startAction = action_s::make<turnInjectionPinHigh>( taggedPointer.getRaw() );
		endActionStage1 = action_s::make<turnInjectionPinLow>( this );
		endActionStage2 = action_s::make<turnInjectionPinLowStage2>( this );
	}

	// Correctly wrap injection start angle
	float angleFromNow = eventAngle - currentPhase;
	if (angleFromNow < 0) {
		angleFromNow += getEngineState()->engineCycle;
	}

	// Schedule opening (stage 1 + stage 2 open together). The delay is
	// computed from THIS edge; with the one-tooth-ahead window it covers
	// 1-2 teeth. Try the hardware angle clock first (armed in the angle
	// domain, per-tooth refresh keeps it accurate at any rpm), fall back
	// to the time-based executor.
	float delayUs = engine->rpmCalculator.oneDegreeUs * angleFromNow;
	efitick_t startTime = sumTickAndFloat(nowNt, USF2NT(delayUs));

#if EFI_ANGLE_CLOCK
	if (angleClockArmInjection(this->cylinderNumber, eventAngle, startAction, currentPhase, nextPhase)) {
		// armed on TMR3 (injection timer) - the ends below still follow in
		// the time domain, computed from the intended start moment.
	} else
#endif // EFI_ANGLE_CLOCK
	{
		getScheduler()->schedule("inj", nullptr, startTime, startAction);
	}

	injectionStartArmed = true;
	injectionStartArmedAt = startTime;

	// Schedule closing stage 1
	efitick_t turnOffTimeStage1 = startTime + US2NT((int)durationUsStage1);
	getScheduler()->schedule("inj", nullptr, turnOffTimeStage1, endActionStage1);

	// Schedule closing stage 2 (if applicable)
	if (hasStage2Injection && endActionStage2) {
		efitick_t turnOffTimeStage2 = startTime + US2NT((int)durationUsStage2);
		getScheduler()->schedule("inj stage 2", nullptr, turnOffTimeStage2, endActionStage2);
	}

#if EFI_DETAILED_LOGGING
	printf("scheduling injection angle=%.2f/delay=%d injectionDuration=%d %d\r\n", angleFromNow, (int)NT2US(startTime - nowNt), (int)durationUsStage1, (int)durationUsStage2);
#endif
#if EFI_DETAILED_LOGGING
	efiPrintf("handleFuel pin=%s eventIndex %d duration=%.2fms %u", outputs[0]->getName(),
			cylinderNumber,
			injectionDurationStage1,
			(unsigned)getRevolutionCounter());
	efiPrintf("handleFuel pin=%s delay=%.2f %u", outputs[0]->getName(), NT2USF(startTime - nowNt),
			(unsigned)getRevolutionCounter());
#endif /* EFI_DETAILED_LOGGING */
}

static void handleFuel(efitick_t nowNt, float currentPhase, float nextPhase, float nextNextPhase) {
	ScopePerf perf(PE::HandleFuel);

	efiAssertVoid(ObdCode::CUSTOM_STACK_6627, hasLotsOfRemainingStack(), "lowstck#3");

	LimpState limitedFuelState = getLimpManager()->allowInjection();

	// todo: eliminate state copy logic by giving limpManager it's owm limp_manager.txt and leveraging LiveData
	engine->outputChannels.fuelCutReason = (int8_t)limitedFuelState.reason;
	bool limitedFuel = !limitedFuelState.value;
	if (limitedFuel) {
		return;
	}

	// This is called in the fast callback already, but since we may have just achieved engine sync (and RPM)
	// for the first time, force update the schedule so that we can inject immediately if necessary
	FuelSchedule *fs = getFuelSchedule();
	if (!fs->isReady) {
		fs->addFuelEvents();
	}

#if FUEL_MATH_EXTREME_LOGGING
	if (printFuelDebug) {
		efiPrintf("handleFuel [%.1f, %.1f) %d", currentPhase, nextPhase, getRevolutionCounter());
	}
#endif /* FUEL_MATH_EXTREME_LOGGING */

	fs->onTriggerTooth(nowNt, currentPhase, nextPhase, nextNextPhase);
}

/**
 * This is the main trigger event handler.
 * Both injection and ignition are controlled from this method.
 */
void mainTriggerCallback(uint32_t trgEventIndex, efitick_t edgeTimestamp, angle_t currentPhase, angle_t nextPhase, angle_t nextNextPhase) {
	ScopePerf perf(PE::MainTriggerCallback);

	if (hasFirmwareError()) {
		/**
		 * In case on a major error we should not process any more events.
		 * Armed angle-clock events are deliberately left armed: they fire at
		 * their bounded ticks exactly like the time-based events of the FALSE
		 * build, and the charge-anchored overdwell rescue remains the coil
		 * safety net. Engine safety after a fatal error comes from
		 * limpManager's cut, not from cancelling already-scheduled events.
		 */
		return;
	}

#if EFI_ANGLE_CLOCK
	{
		// Angle->time basis for this tooth's arming and the per-tooth
		// refresh: the 90-degree-window rpm average (oneDegreeUs) - the SAME
		// basis the proven time-based path converts angles with. During
		// spin-up oneDegreeUs is the InstantRpmCalculator's ~90-degree tooth
		// window (calculateInstantRpm hunts the tooth ~90 deg back), when
		// running it is the full-cycle average - both smooth. The per-tooth
		// basis (toothDurations[0]) was tried and rejected: at cranking the
		// compression oscillation makes a single tooth a 2-3x-stretched
		// predictor (see angle_clock.h).
		//
		// THIS RUNS BEFORE THE rpm==0 GATE: during the catch trigger storm
		// the rpm sensor flaps 0/nonzero at ~1 kHz and the gate returns
		// early, so a refresh gated behind it never runs on the flap teeth -
		// the armed events kept their PRE-CATCH ticks (computed at ~250 rpm)
		// while the engine accelerated to ~800+, the fires landed ms-late,
		// the rescues discharged (overdwell n~50, C935x) and the engine ran
		// on wrong-angle sparks until it stalled. The refresh must track the
		// true speed on EVERY tooth; angleClockOnTooth keeps the last good
		// basis when the flap makes oneDegreeUs NaN.
		angleClockOnTooth(edgeTimestamp, currentPhase, engine->engineState.engineCycle,
			US2NT(engine->rpmCalculator.oneDegreeUs));
	}
#endif // EFI_ANGLE_CLOCK

	float rpm = engine->rpmCalculator.getCachedRpm();
	if (rpm == 0) {
		// this happens while we just start cranking - and on EVERY flap of the
		// catch trigger storm, where the rpm sensor flaps 0/nonzero at ~1 kHz.
		// Armed angle-clock events are NOT cancelled here: cancelling them
		// turned every storm flap into a lost fire (the C935x rescue cluster
		// on the car) and a fire on a never-charged coil (C9012). The armed
		// ticks are bounded (angle-domain arming + per-tooth refresh) and the
		// charge-anchored overdwell rescue bounds any charge, so they fire
		// exactly like the time-based events of the FALSE build - which fire
		// by time regardless of the rpm flap. The refresh still runs on this
		// tooth (below) so the armed events keep tracking the true speed
		// through the storm.
#if EFI_ANGLE_CLOCK
		angleClockRefresh();
#endif // EFI_ANGLE_CLOCK

		// todo: check for 'trigger->is_synchnonized?'
		return;
	}

#if EFI_ANGLE_CLOCK
	// Arm dwell starts EARLY: before handleFuel so the handoff has only
	// ~20-30 µs elapsed. This gives TMR2 arm margin even at 7000 rpm where
	// the same arm attempted at the end of onTriggerEventSparkLogic (~250 µs
	// elapsed) always fails because the delay for 6° = 143 µs < 250 µs.
	scheduleDwellEarlyIfDue(rpm, edgeTimestamp, currentPhase, nextPhase, nextNextPhase);
#endif // EFI_ANGLE_CLOCK

	if (trgEventIndex == 0) {

		if (getTriggerCentral()->checkIfTriggerConfigChanged()) {
			getIgnitionEvents()->isReady = false; // we need to rebuild complete ignition schedule
			getFuelSchedule()->isReady = false;
			// moved 'triggerIndexByAngle' into trigger initialization (why was it invoked from here if it's only about trigger shape & optimization?)
			// see updateTriggerConfiguration() -> prepareOutputSignals()

			// we need this to apply new 'triggerIndexByAngle' values
			engine->periodicFastCallback();
		}
	}

	engine->engineModules.apply_all([=](auto & m) {
		m.onEnginePhase(rpm, edgeTimestamp, currentPhase, nextPhase);
	});

	/**
	 * For fuel we schedule start of injection based on trigger angle, and then inject for
	 * specified duration of time
	 */
	handleFuel(edgeTimestamp, currentPhase, nextPhase, nextNextPhase);

	engine->module<TriggerScheduler>()->scheduleEventsUntilNextTriggerTooth(
		rpm, edgeTimestamp, currentPhase, nextPhase, nextNextPhase);

	/**
	 * For spark we schedule both start of coil charge and actual spark based on trigger angle
	 */
	onTriggerEventSparkLogic(rpm, edgeTimestamp, currentPhase, nextPhase, nextNextPhase);

#if EFI_ANGLE_CLOCK
	// Re-anchor every armed channel from this tooth's freshest basis and apply
	// the stale-event policy: an event whose angle has arrived fires NOW (both
	// kinds - the time-based build fires a due dwell/injection too), a stale
	// phase basis (desync) leaves the channel armed to fire by time.
	angleClockRefresh();
#endif // EFI_ANGLE_CLOCK
}

#endif /* EFI_ENGINE_CONTROL */
