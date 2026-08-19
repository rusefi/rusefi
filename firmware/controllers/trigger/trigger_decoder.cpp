/**
 * @file	trigger_decoder.cpp
 *
 * @date Dec 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 *
 *
 * enable trigger_details
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

#include "global_shared.h"
#include "engine_configuration.h"
#include "transition_events.h"
#include "board_overrides.h"

std::optional<setup_custom_board_overrides_type> custom_board_TriggerResetState;

/**
 * decoder uses TriggerStimulatorHelper in findTriggerZeroEventIndex
 */
#include "trigger_simulator.h"

#ifndef NOISE_RATIO_THRESHOLD
#define NOISE_RATIO_THRESHOLD 3000
#endif

TriggerDecoderBase::TriggerDecoderBase(const char* p_name)
	: name(p_name)
{
	TriggerDecoderBase::resetState();
}

bool TriggerDecoderBase::getShaftSynchronized() const {
	return shaft_is_synchronized;
}

void TriggerDecoderBase::setShaftSynchronized(bool value) {
#if EFI_TOOTH_LOGGER
	if (value != shaft_is_synchronized) {
		LogTriggerSync(getTimeNowNt(), value);
	}
#endif /* EFI_TOOTH_LOGGER */

	if (value) {
		if (!shaft_is_synchronized) {
			// just got synchronized
			mostRecentSyncTime = getTimeNowNt();
		}
	} else {
		// sync loss
		mostRecentSyncTime = 0;
	}
	shaft_is_synchronized = value;
	onShaftSynchronized(value);
}

/**
 * Resets the base decoder state.
 *
 * This handles fields common to all trigger inputs (primary, VVT, etc),
 * such as tooth durations and basic synchronization flags.
 *
 * This is called during initialization, when engine rotation stops,
 * or during trigger re-sync search.
 */
void TriggerDecoderBase::resetState() {
	setShaftSynchronized(false);
	toothed_previous_time = 0;

	setArrayValues(toothDurations, 0);

	synchronizationCounter = 0;
	totalTriggerErrorCounter = 0;
	orderingErrorCounter = 0;
	m_timeSinceDecodeError.init();

	prevSignal = SHAFT_PRIMARY_FALLING;
	startOfCycleNt = {};
	lastFullRevolutionDurationNt = 0;

	resetCurrentCycleState();

	totalEventCountBase = 0;
	isFirstEvent = true;

	vvtToothDurations0 = 0;
	vvtCurrentPosition = 0;
	setArrayValues(vvtToothPosition, 0);
	triggerSyncGapRatio = 0;
	triggerStateIndex = 0;

	call_board_override(custom_board_TriggerResetState);
}

void TriggerDecoderBase::setTriggerErrorState(int errorIncrement) {
	m_timeSinceDecodeError.reset();
	totalTriggerErrorCounter += errorIncrement;
	onTransitionEvent(TransitionEvent::TriggerError);
}

void TriggerDecoderBase::resetCurrentCycleState() {
	setArrayValues(currentCycle.eventCount, 0);
	currentCycle.current_index = 0;
}

#if EFI_SHAFT_POSITION_INPUT

PrimaryTriggerDecoder::PrimaryTriggerDecoder(const char* p_name)
	: TriggerDecoderBase(p_name)
{
}

#if ! EFI_PROD_CODE
bool printTriggerDebug = false;
bool printTriggerTrace = false;
#endif /* ! EFI_PROD_CODE */

void TriggerWaveform::initializeSyncPoint(TriggerDecoderBase& state,
			const TriggerConfiguration& triggerConfiguration) {
	triggerShapeSynchPointIndex = state.findTriggerZeroEventIndex(*this, triggerConfiguration);
}

void TriggerFormDetails::prepareEventAngles(TriggerWaveform *shape) {
	int triggerShapeSynchPointIndex = shape->triggerShapeSynchPointIndex;
	if (triggerShapeSynchPointIndex == (int)EFI_ERROR_CODE) {
		return;
	}
	angle_t firstAngle = shape->getAngle(triggerShapeSynchPointIndex);
	assertAngleRange(firstAngle, "firstAngle", ObdCode::CUSTOM_TRIGGER_SYNC_ANGLE);

	size_t length = shape->getLength();

	setArrayValues(eventAngles, 0);

	// this may be <length for some triggers like symmetrical crank Miata NB
	size_t triggerShapeLength = shape->getSize();

	assertAngleRange(triggerShapeSynchPointIndex, "triggerShapeSynchPointIndex", ObdCode::CUSTOM_TRIGGER_SYNC_ANGLE2);
	efiAssertVoid(ObdCode::CUSTOM_TRIGGER_CYCLE, getTriggerCentral()->engineCycleEventCount != 0, "zero engineCycleEventCount");

	float lastAnglePrimary = 0.0;
	float lastAngleSecondary = 0.0;

	for (size_t eventIndex = 0; eventIndex < length; eventIndex++) {
		if (eventIndex == 0) {
			// explicit check for zero to avoid issues where logical zero is not exactly zero due to float nature
			eventAngles[0] = 0;
			// this value would be used in case of front-only
			eventAngles[1] = 0;
		} else {
			// Rotate the trigger around so that the sync point is at position 0
			auto wrappedIndex = (triggerShapeSynchPointIndex + eventIndex) % length;

			// Compute this tooth's position within the trigger definition
			// (wrap, as the trigger def may be smaller than total trigger length)
			auto triggerDefinitionIndex = wrappedIndex % triggerShapeLength;

			// Compute the relative angle of this tooth to the sync point's tooth
			float angle = shape->getAngle(wrappedIndex) - firstAngle;

			efiAssertVoid(ObdCode::CUSTOM_TRIGGER_CYCLE, !std::isnan(angle), "trgSyncNaN");
			// Wrap the angle back in to [0, 720)
			wrapAngle(angle, "trgSync", ObdCode::CUSTOM_TRIGGER_SYNC_ANGLE_RANGE);

			if (shape->useOnlyRisingEdges) {
				criticalAssertVoid(triggerDefinitionIndex < triggerShapeLength, "trigger shape fail");
				assertIsInBounds(triggerDefinitionIndex, shape->isRiseEvent, "isRise");
				bool primary = shape->getWheel(triggerDefinitionIndex) == TriggerWheel::T_PRIMARY ? true : false;
				// In case this is a rising event, store the angle to be used for the next falling event.
				if (shape->isRiseEvent[triggerDefinitionIndex]) {
					eventAngles[eventIndex] = angle;
					if (primary) {
						lastAnglePrimary = angle;
					} else {
						lastAngleSecondary = angle;
					}
					// If this is a falling event, apply the angle of the last rising event on this channel.
				} else if (primary) {
					eventAngles[eventIndex] = lastAnglePrimary;
				} else {
					eventAngles[eventIndex] = lastAngleSecondary;
				}
			} else {
				eventAngles[eventIndex] = angle;
			}
		}
	}
}

int64_t TriggerDecoderBase::getTotalEventCounter() const {
	return totalEventCountBase + currentCycle.current_index;
}

/**
 * @return number of completed wheel synchronization cycles for this decoder since reset.
 * Incremented once per full trigger cycle via incrementShaftSynchronizationCounter().
 * For the primary (crank) decoder this effectively counts engine revolutions/cycles
 * and is used widely (e.g. crank-vs-cam phase alignment, knock indexing, VVT validation).
 */
int TriggerDecoderBase::getSynchronizationCounter() const {
	return synchronizationCounter;
}

/**
 * Resets the primary decoder state, including its phase sync state.
 *
 * This overrides TriggerDecoderBase::resetState() to handle primary-specific
 * state like crank/phase sync flags, while calling the base implementation
 * to clear common fields.
 *
 * This is called during initialization, when engine rotation stops,
 * or during trigger re-sync search.
 */
void PrimaryTriggerDecoder::resetState() {
	TriggerDecoderBase::resetState();

	/**
	 * resetHasFullSync() resets m_hasSynchronizedPhase to true OR false
	 * depending on m_needsDisambiguation
	 */
	resetHasFullSync();
	m_hasSynchronizedCrank = false;
	m_phaseAdjustment = 0;
}


bool TriggerDecoderBase::isValidIndex(const TriggerWaveform& triggerShape) const {
	return currentCycle.current_index < triggerShape.getSize();
}

static TriggerWheel eventIndex[4] = { TriggerWheel::T_PRIMARY, TriggerWheel::T_PRIMARY, TriggerWheel::T_SECONDARY, TriggerWheel::T_SECONDARY };
static TriggerValue eventType[4] = { TriggerValue::FALL, TriggerValue::RISE, TriggerValue::FALL, TriggerValue::RISE };

#if EFI_UNIT_TEST
#define PRINT_INC_INDEX 		if (printTriggerTrace) {\
		printf("nextTriggerEvent index=%d\r\n", currentCycle.current_index); \
		}
#else
#define PRINT_INC_INDEX {}
#endif /* EFI_UNIT_TEST */

#define nextTriggerEvent() \
 { \
	if (useOnlyRisingEdgeForTrigger) {currentCycle.current_index++;} \
	currentCycle.current_index++; \
	PRINT_INC_INDEX; \
}

int TriggerDecoderBase::getCurrentIndex() const {
	return currentCycle.current_index;
}

angle_t PrimaryTriggerDecoder::syncEnginePhase(int divider, int remainder, angle_t engineCycle) {
	efiAssert(ObdCode::OBD_PCM_Processor_Fault, divider > 1, "syncEnginePhase divider", false);
	efiAssert(ObdCode::OBD_PCM_Processor_Fault, remainder < divider, "syncEnginePhase remainder", false);
	angle_t totalShift = 0;
	while (getSynchronizationCounter() % divider != remainder) {
		/**
		 * we are here if we've detected the cam sensor within the wrong crank phase
		 * let's increase the trigger event counter, that would adjust the state of
		 * virtual crank-based trigger
		 */
		incrementShaftSynchronizationCounter();
		totalShift += engineCycle / divider;
	}

	// Allow injection/ignition to happen, we've now fully sync'd the crank based on new cam information
	m_hasSynchronizedPhase = true;

	if (totalShift > 0) {
		m_phaseAdjustment = totalShift;
		// Resync angle changed - count how many times this happens
		phaseResyncCounter++;
		onTransitionEvent(TransitionEvent::EngineResync);
	}

	return totalShift;
}

void TriggerDecoderBase::incrementShaftSynchronizationCounter() {
	synchronizationCounter++;
}

void PrimaryTriggerDecoder::onShaftSynchronized(bool value) {
	enginePins.debugTriggerState.setValue(value);
	m_hasSynchronizedCrank = value;
}

void PrimaryTriggerDecoder::onTriggerError() {
	// On trigger error, we've lost full sync
	resetHasFullSync();

	// Ignore the warning that engine is never null - it might be in unit tests
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Waddress"
		if (engine) {
			// Instant RPM data is now also probably trash, discard it
			engine->triggerCentral.instantRpm.resetInstantRpm();
			engine->rpmCalculator.lastTdcTimer.init();
		}
	#pragma GCC diagnostic pop
}

void PrimaryTriggerDecoder::onNotEnoughTeeth(int /*actual*/, int /*expected*/) {
	warning(ObdCode::CUSTOM_PRIMARY_NOT_ENOUGH_TEETH, "primary trigger error: not enough teeth between sync points: expected %d/%d got %d/%d",
			getTriggerCentral()->triggerShape.getExpectedEventCount(TriggerWheel::T_PRIMARY),
			getTriggerCentral()->triggerShape.getExpectedEventCount(TriggerWheel::T_SECONDARY),
		currentCycle.eventCount[0],
		currentCycle.eventCount[1]);
}

#if EFI_UNIT_TEST
// weird: we count trigger error on next sync, but if we never sync - counter does not increase?!
int tooManyTeethCounter = 0;
#endif // EFI_UNIT_TEST

void PrimaryTriggerDecoder::onTooManyTeeth(int /*actual*/, int /*expected*/) {
	warning(ObdCode::CUSTOM_PRIMARY_TOO_MANY_TEETH, "primary trigger error: too many teeth between sync points: expected %d/%d got %d/%d",
			getTriggerCentral()->triggerShape.getExpectedEventCount(TriggerWheel::T_PRIMARY),
			getTriggerCentral()->triggerShape.getExpectedEventCount(TriggerWheel::T_SECONDARY),
		currentCycle.eventCount[0],
		currentCycle.eventCount[1]);
}

const char *getTrigger_event_e(trigger_event_e value){
switch(value) {
case SHAFT_PRIMARY_FALLING:
  return "SHAFT_PRIMARY_FALLING";
case SHAFT_PRIMARY_RISING:
  return "SHAFT_PRIMARY_RISING";
case SHAFT_SECONDARY_FALLING:
  return "SHAFT_SECONDARY_FALLING";
case SHAFT_SECONDARY_RISING:
  return "SHAFT_SECONDARY_RISING";
  }
 return NULL;
}
const char *getTrigger_value_e(TriggerValue value){
switch(value) {
case TriggerValue::FALL:
  return "TriggerValue::FALL";
case TriggerValue::RISE:
  return "TriggerValue::RISE";
  }
 return NULL;
}

void VvtTriggerDecoder::onNotEnoughTeeth(int actual, int expected) {
	warning(ObdCode::CUSTOM_CAM_NOT_ENOUGH_TEETH, "cam %s trigger error: not enough teeth between sync points: actual %d expected %d", name, actual, expected);
}

void VvtTriggerDecoder::onTooManyTeeth(int actual, int expected) {
	warning(ObdCode::CUSTOM_CAM_TOO_MANY_TEETH, "cam %s trigger error: too many teeth between sync points: %d > %d", name, actual, expected);
}

PUBLIC_API_WEAK bool isTriggerCounterError(int8_t triggerCountersError) {
  return triggerCountersError != 0;
}

int TriggerDecoderBase::getEventCountersError(const TriggerWaveform& triggerShape) const {
	// We can check if things are fine by comparing the number of events in a cycle with the expected number of event.
	int countersError = 0;
	for (int i = 0;i < PWM_PHASE_MAX_WAVE_PER_PWM;i++) {
	  countersError = currentCycle.eventCount[i] - triggerShape.getExpectedEventCount((TriggerWheel)i);
	  if (countersError != 0) {
	    break;
	  }
	}

#if EFI_DETAILED_LOGGING
	printf("getEventCountersError: isDecodingError=%d\n", (countersError != 0));
	if (countersError != 0) {
		for (int i = 0;i < PWM_PHASE_MAX_WAVE_PER_PWM;i++) {
			printf("  count: cur=%d exp=%d\n", currentCycle.eventCount[i],  triggerShape.getExpectedEventCount((TriggerWheel)i));
		}
	}
#endif /* EFI_UNIT_TEST */

	return countersError;
}

void TriggerDecoderBase::onShaftSynchronization(
		bool wasSynchronized,
		const efitick_t nowNt,
		const TriggerWaveform& triggerShape) {
	if (wasSynchronized) {
		// a full trigger revolution just closed: remember its duration for
		// the minimum-elapsed-time sync gate (decodeTriggerEvent)
		lastFullRevolutionDurationNt = nowNt - startOfCycleNt;
	}

	startOfCycleNt = nowNt;
	resetCurrentCycleState();

	if (wasSynchronized) {
		incrementShaftSynchronizationCounter();
	} else {
		// We have just synchronized, this is the zeroth revolution
		synchronizationCounter = 0;
	}

	totalEventCountBase += triggerShape.getSize();

#if EFI_UNIT_TEST
	if (printTriggerDebug) {
		printf("onShaftSynchronization index=%d %d\r\n",
				currentCycle.current_index,
				synchronizationCounter);
	}
#endif /* EFI_UNIT_TEST */
}

static bool shouldConsiderEdge(const TriggerWaveform& triggerShape, TriggerWheel triggerWheel, TriggerValue edge) {
	if (triggerWheel != TriggerWheel::T_PRIMARY && triggerShape.useOnlyPrimaryForSync) {
		// Non-primary events ignored
		return false;
	}

	switch (triggerShape.syncEdge) {
		case SyncEdge::Both: return true;
		case SyncEdge::RiseOnly:
		case SyncEdge::Rise: return edge == TriggerValue::RISE;
		case SyncEdge::Fall: return edge == TriggerValue::FALL;
	}

	// how did we get here?
	// assert(false)?

	return false;
}

void TriggerDecoderBase::printGaps(const char * prefix,
  const TriggerConfiguration& triggerConfiguration,
  const TriggerWaveform& triggerShape) {
				for (int i = 0;i<triggerShape.gapTrackingLength;i++) {
					float ratioFrom = triggerShape.synchronizationRatioFrom[i];
					if (std::isnan(ratioFrom)) {
						// we do not track gap at this depth
						continue;
					}

					float gap = 1.0 * toothDurations[i] / toothDurations[i + 1];
					if (std::isnan(gap)) {
						efiPrintf("%s index=%d NaN gap, you have noise issues?", prefix, i);
					} else {
						float ratioTo = triggerShape.synchronizationRatioTo[i];

						bool gapOk = isInRange(ratioFrom, gap, ratioTo);

						efiPrintf("%s %srpm=%d time=%d eventIndex=%lu gapIndex=%d: %s gap=%.3f expected from %.3f to %.3f error=%s",
								prefix,
								triggerConfiguration.PrintPrefix,
								(int)Sensor::getOrZero(SensorType::Rpm),
							/* cast is needed to make sure we do not put 64 bit value to stack*/ (int)getTimeNowS(),
							currentCycle.current_index,
							i,
							gapOk ? "Y" : "n",
							gap,
							ratioFrom,
							ratioTo,
							boolToString(someSortOfTriggerError()));
					}
				}
}

/**
 * @brief Trigger decoding happens here
 * VR falls are filtered out and some VR noise detection happens prior to invoking this method, for
 * Hall this method is invoked every time we have a fall or rise on one of the trigger sensors.
 * This method changes the state of trigger_state_s data structure according to the trigger event
 * @param signal type of event which just happened
 * @param nowNt current time
 */
expected<TriggerDecodeResult> TriggerDecoderBase::decodeTriggerEvent(
		const char *msg,
		const TriggerWaveform& triggerShape,
		TriggerStateListener* triggerStateListener,
		const TriggerConfiguration& triggerConfiguration,
		const trigger_event_e signal,
		const efitick_t nowNt) {
	ScopePerf perf(PE::DecodeTriggerEvent);

	// Non-sync events clear the flag: it is only ever true for the exact sync
	// event that validated a real crank revolution (see the sync point branch).
	lastSyncWasClean = false;

#if EFI_PROD_CODE
  getTriggerCentral()->triggerElapsedUs = previousEventTimer.getElapsedUs();
#endif

	if (previousEventTimer.getElapsedSecondsAndReset(nowNt) > 1) {
		/**
		 * We are here if there is a time gap between now and previous shaft event - that means the engine is not running.
		 * That means we have lost synchronization since the engine is not running :)
		 */
		setShaftSynchronized(false);
		if (triggerStateListener) {
			triggerStateListener->OnTriggerSynchronizationLost();
		}
	}

	bool useOnlyRisingEdgeForTrigger = triggerShape.useOnlyRisingEdges;

	efiAssert(ObdCode::CUSTOM_TRIGGER_UNEXPECTED, signal <= SHAFT_SECONDARY_RISING, "unexpected signal", unexpected);

	TriggerWheel triggerWheel = eventIndex[signal];
	TriggerValue type = eventType[signal];

	// Check that we didn't get the same edge twice in a row - that should be impossible
	if (!useOnlyRisingEdgeForTrigger && prevSignal == signal) {
		orderingErrorCounter++;
	}

	prevSignal = signal;

	currentCycle.eventCount[(int)triggerWheel]++;

	if (toothed_previous_time > nowNt) {
		firmwareError(ObdCode::CUSTOM_OBD_93, "[%s] toothed_previous_time after nowNt prev=%lu now=%lu", msg, (uint32_t)toothed_previous_time, (uint32_t)nowNt);
	}

	efidur_t currentDurationLong = isFirstEvent ? 0 : (nowNt - toothed_previous_time);

	/**
	 * For performance reasons, we want to work with 32 bit values. If there has been more then
	 * 10 seconds since previous trigger event we do not really care.
	 */
	toothDurations[0] =
			currentDurationLong > 10 * NT_PER_SECOND ? 10 * NT_PER_SECOND : currentDurationLong;

	if (!shouldConsiderEdge(triggerShape, triggerWheel, type)) {
#if EFI_UNIT_TEST
		if (printTriggerTrace) {
			printf("%s isLessImportant %s now=%d index=%d\r\n",
					getTrigger_type_e(triggerConfiguration.TriggerType.type),
					getTrigger_event_e(signal),
					(int)nowNt,
					currentCycle.current_index);
		}
#endif /* EFI_UNIT_TEST */

		// For less important events we simply increment the index.
		nextTriggerEvent();
	} else {
#if !EFI_PROD_CODE
		if (printTriggerTrace) {
			printf("%s event %s %lld\r\n",
					getTrigger_type_e(triggerConfiguration.TriggerType.type),
					getTrigger_event_e(signal),
					(long long)nowNt);
			printf("decodeTriggerEvent ratio %.2f: current=%d previous=%d\r\n", 1.0 * toothDurations[0] / toothDurations[1],
					toothDurations[0], toothDurations[1]);
		}
#endif

		isFirstEvent = false;
		bool isSynchronizationPoint;
		bool wasSynchronized = getShaftSynchronized();

		if (triggerShape.isSynchronizationNeeded) {
			triggerSyncGapRatio = (float)toothDurations[0] / toothDurations[1];

			if (wasSynchronized && triggerSyncGapRatio > NOISE_RATIO_THRESHOLD) {
			    setTriggerErrorState(100);
			}

			// Once synchronized, only the expected gap position may re-sync.
			// Otherwise a stretched-tooth pair mid-revolution passes the ratio
			// windows and kicks a running engine out of sync with C9003 - see the
			// m74_9 logs: the false pair is 1.7-2.0 at ~300 rpm but grows to
			// 2.476 at 402 rpm (firing), overlapping the real-gap window, so no
			// ratio window can separate them. The gate is wheel-aware and
			// asymmetric: a real gap can only arrive LATE (noise inserts spurious
			// teeth, shifting the count up), never early - so a candidate before
			// expectedEventCount - 2 is necessarily a false pair and is ignored
			// (the decoder keeps counting, the real gap re-syncs cleanly). A
			// candidate at or after the expected position is accepted, and the
			// existing too-many/few-teeth counter machinery reports any count
			// mismatch exactly like the ratio-only decoder did. Note: eventCount
			// was already incremented for the current event (same condition as
			// the noise filter's isGapExpected, which runs pre-increment). NOT
			// keyed on useOnlyPrimaryForSync: the 60-2 wheel leaves that flag
			// false when a cam/VVT input is configured (secondary events must
			// reach the decoder), and the bypass disabled the gate exactly where
			// it was needed.
			uint32_t expectedCount = triggerShape.getExpectedEventCount(triggerWheel);
			uint32_t eventsSinceSync = currentCycle.eventCount[(int)triggerWheel];
			bool atExpectedGapPosition = !wasSynchronized ||
				(expectedCount == 0 || eventsSinceSync + 2 >= expectedCount);

			// Minimum-elapsed-time gate: a REAL gap can only arrive roughly one
			// full revolution after the previous sync point. A noise storm
			// injects many edges in a few ms, so the event count can reach
			// expectedCount at a storm edge mid-revolution; that edge then
			// passes the ratio windows (chaotic spacings occasionally land
			// inside), the position gate and the count check and false-syncs
			// the decoder "cleanly" - no error, no desync, just a silent +1 to
			// the sync counter and a shifted phase basis. The m74_9 car logs
			// show the result: ~9 syncs/s at 270 rpm (the crank makes 4.5
			// rev/s), and the engine-cycle counter racing ~2-3x. Require at
			// least a quarter of the previous revolution's duration - storm
			// syncs arrive within a few percent of a revolution, and no real
			// engine accelerates 4x within one revolution (the first-combustion
			// catch, the fastest real transition, compresses the next revolution
			// to ~0.3-0.5x). The first sync and re-syncs after a desync have no
			// previous revolution to compare against and stay exempt.
			bool tooEarlyForSync = wasSynchronized
				&& lastFullRevolutionDurationNt > 0
				&& (nowNt - startOfCycleNt) < lastFullRevolutionDurationNt / 4;

			// Real-tooth-before-the-gap gate: the missing-teeth gap always
			// follows a REAL tooth whose period is close to the revolution's
			// mean (compression ripple moves it maybe 0.6-1.4x, never below a
			// quarter). A storm-inflated candidate follows a storm edge - a
			// few hundred us vs ~4 ms at cranking - no matter WHERE in the
			// revolution the storm lands, so this gate rejects storm syncs the
			// elapsed-time gate cannot reach (storms late in the revolution).
			// Board opt-in (m74_9): wheels with irregular tooth patterns or
			// real cranking logs with extreme per-tooth variation would trip
			// this on legitimate gaps.
			bool gapHardening = get_board_override_result(custom_board_syncGapHardening, false);
			efitick_t minRealToothNt = (lastFullRevolutionDurationNt > 0 && expectedCount > 0)
				? lastFullRevolutionDurationNt / expectedCount / 4
				: 0;
			bool previousToothNotReal = gapHardening && wasSynchronized
				&& minRealToothNt > 0
				&& toothDurations[1] < minRealToothNt;

			isSynchronizationPoint = !tooEarlyForSync && !previousToothNotReal && atExpectedGapPosition && isSyncPoint(triggerShape, triggerConfiguration.TriggerType.type);

			// First-combustion protection (board opt-in via
			// custom_board_syncByPositionWhileCranking, m74_9): at the catch the
			// crank accelerates so hard that the REAL missing-teeth gap can
			// compress below the ratio window (documented below 2.25 for 36-2,
			// same physics for 60-2) - the decoder would reject the sync at the
			// exact gap position, count another full revolution and fire C9002
			// right when the engine just caught. At the exact expected position
			// the tooth count already proves this IS the gap, so accept it
			// unconditionally while the engine is still in the cranking band
			// (rpm < 2 * crankingRpm). The only way this mis-syncs is a single
			// noise-inserted/missed tooth shifting the count by one: the
			// resulting 6-degree phase error is silent (the counter check
			// passes), but it self-corrects within one revolution - the next
			// real gap arrives at count expected-1, syncs with a count mismatch
			// (C9003) and the decoder re-synchronizes cleanly. Harmless at
			// cranking speed, so the skip is off once the engine is running.
			// Conditions are evaluated lazily: mock tests run the decoder with
			// engineConfiguration = nullptr.
			if (!isSynchronizationPoint && !tooEarlyForSync && !previousToothNotReal && wasSynchronized && atExpectedGapPosition &&
					get_board_override_result(custom_board_syncByPositionWhileCranking, false) &&
					currentCycle.eventCount[(int)triggerWheel] == triggerShape.getExpectedEventCount(triggerWheel) &&
					// the missing-teeth gap is physically 3 tooth slots; even the
					// hardest first-combustion acceleration cannot compress it
					// below ~1.0 tooth times (the crank speed would have to more
					// than triple across a single 6-degree tooth), and the m74_9
					// catch compressed it below 1.2 - which rejected the REAL gap
					// and produced the 00:05 C9002 "expected 58 got 58". A much
					// shorter "gap" is a noise edge right after a real tooth
					// (e.g. 250 us after tooth 57: ratio 0.07), which the count
					// check alone cannot distinguish from the real gap. The 0.8
					// floor sits below any physical gap and 11x above the noise
					// case. Board opt-in (m74_9): some wheels legitimately
					// override gaps at ratios below 0.8 (see subaru.overrideGap).
					(!gapHardening || triggerSyncGapRatio >= 0.8f) &&
#if EFI_UNIT_TEST
					// mock tests run the decoder with engineConfiguration = nullptr
					engineConfiguration != nullptr &&
#endif
					Sensor::getOrZero(SensorType::Rpm) < 2 * engineConfiguration->cranking.rpm) {
				isSynchronizationPoint = true;
			}

			if (isSynchronizationPoint) {
				enginePins.debugTriggerSync.toggle();
			}

			/**
			 * todo: technically we can afford detailed logging even with 60/2 as long as low RPM
			 * todo: figure out exact threshold as a function of RPM and tooth count?
			 * Open question what is 'triggerShape.getSize()' for 60/2 is it 58 or 58*2 or 58*4?
			 */
#if EFI_PROD_CODE || EFI_SIMULATOR
			bool silentTriggerError = triggerShape.getSize() > 40 && engineConfiguration->silentTriggerError;

			bool verbose = getTriggerCentral()->isEngineSnifferEnabled && triggerConfiguration.VerboseTriggerSynchDetails;

			if (verbose || (someSortOfTriggerError() && !silentTriggerError)) {
			    const char * prefix = verbose ? "[vrb]" : "[err]";
			    printGaps(prefix, triggerConfiguration, triggerShape);
			}
#else
			if (printTriggerTrace) {
				for (int i = 0;i<triggerShape.gapTrackingLength;i++) {
					float gap = 1.0 * toothDurations[i] / toothDurations[i + 1];
					printf("%sindex=%d: gap=%.2f expected from %.2f to %.2f error=%s\r\n",
							triggerConfiguration.PrintPrefix,
							i,
							gap,
							triggerShape.synchronizationRatioFrom[i],
							triggerShape.synchronizationRatioTo[i],
							boolToString(someSortOfTriggerError()));
				}
			}
#endif /* EFI_PROD_CODE */
		} else {
			/**
			 * We are here in case of a wheel without synchronization - we just need to count events,
			 * synchronization point simply happens once we have the right number of events
			 *
			 * in case of noise the counter could be above the expected number of events, that's why 'more or equals' and not just 'equals'
			 */

			unsigned int endOfCycleIndex = triggerShape.getSize() - (useOnlyRisingEdgeForTrigger ? 2 : 1);

			isSynchronizationPoint = !getShaftSynchronized() || (currentCycle.current_index >= endOfCycleIndex);

#if EFI_UNIT_TEST
			if (printTriggerTrace) {
				printf("decodeTriggerEvent sync=%d isSynchronizationPoint=%d index=%d size=%d\r\n",
						getShaftSynchronized(),
					isSynchronizationPoint,
					currentCycle.current_index,
					(int)triggerShape.getSize());
			}
#endif /* EFI_UNIT_TEST */
		}
#if EFI_UNIT_TEST
		if (printTriggerTrace) {
			printf("decodeTriggerEvent gap %s isSynchronizationPoint=%d index=%d %s\r\n",
					getTrigger_type_e(triggerConfiguration.TriggerType.type),
					isSynchronizationPoint, currentCycle.current_index,
					getTrigger_event_e(signal));
		}
#endif /* EFI_UNIT_TEST */

		if (isSynchronizationPoint) {
		  triggerCountersError = getEventCountersError(triggerShape);
			bool isDecodingError = isTriggerCounterError(triggerCountersError);

			// Only a sync with the exact expected tooth count is a validated crank
			// revolution - the only kind the engine-cycle counter must count.
			// Wheels without gap sync (isSynchronizationNeeded == false) sync on
			// every cycle boundary and have no false-gap mechanism, so their
			// syncs always count (legacy behavior for single-tooth wheels).
			lastSyncWasClean = !triggerShape.isSynchronizationNeeded || !isDecodingError;

			if (triggerStateListener) {
				triggerStateListener->OnTriggerSynchronization(wasSynchronized, isDecodingError);
			}

			// If we got a sync point, but the wrong number of events since the last sync point
			// One of two things has happened:
			//  - We missed a tooth, and this is the real sync point
			//  - Due to some mistake in timing, we found what looks like a sync point but actually isn't
			// In either case, we should wait for another sync point before doing anything to try and run an engine,
			// so we clear the synchronized flag.
			if (wasSynchronized && isDecodingError) {
				setTriggerErrorState();
				onNotEnoughTeeth(currentCycle.current_index, triggerShape.getSize());

				// Something wrong, no longer synchronized
				setShaftSynchronized(false);

				// This is a decoding error
				onTriggerError();
				printGaps("newerr", triggerConfiguration, triggerShape);
			} else {
				// If this was the first sync point OR no decode error, we're synchronized!
				setShaftSynchronized(true);
			}

			// this call would update duty cycle values
			nextTriggerEvent();

			onShaftSynchronization(wasSynchronized, nowNt, triggerShape);
		} else {	/* if (!isSynchronizationPoint) */
			nextTriggerEvent();
		}

		for (int i = triggerShape.gapTrackingLength; i > 0; i--) {
			toothDurations[i] = toothDurations[i - 1];
		}

		toothed_previous_time = nowNt;

#if EFI_UNIT_TEST
        if (wasSynchronized) {
            int uiGapIndex = (currentCycle.current_index) % triggerShape.getLength();
            gapRatio[uiGapIndex] = triggerSyncGapRatio;
        }
#endif // EFI_UNIT_TEST
	}

	if (getShaftSynchronized() && !isValidIndex(triggerShape)) {
		// We've had too many events since the last sync point, we should have seen a sync point by now.
		// This is a trigger error.

		// let's not show a warning if we are just starting to spin
		if (Sensor::getOrZero(SensorType::Rpm) != 0) {
			setTriggerErrorState();
			onTooManyTeeth(currentCycle.current_index, triggerShape.getSize());
		}

		onTriggerError();

		setShaftSynchronized(false);

		return unexpected;
	} else if (!isValidIndex(triggerShape)) {
#if EFI_UNIT_TEST
		tooManyTeethCounter++;
#endif // EFI_UNIT_TEST
	}

	triggerStateIndex = currentCycle.current_index;

	// Needed for early instant-RPM detection
	TriggerStateListener * l = triggerStateListener;
	while (l) {
		l->OnTriggerStateProperState(nowNt, triggerStateIndex);
		l = l->nextListener();
	}

	if (getShaftSynchronized()) {
		return TriggerDecodeResult{ currentCycle.current_index };
	} else {
		return unexpected;
	}
}

bool TriggerDecoderBase::isSyncPoint(const TriggerWaveform& triggerShape, trigger_type_e triggerType) const {
	// Miata NB needs a special decoder.
	// The problem is that the crank wheel only has 4 teeth, also symmetrical, so the pattern
	// is long-short-long-short for one crank rotation.
	// A quick acceleration can result in two successive "short gaps", so we see
	// long-short-short-short-long instead of the correct long-short-long-short-long
	// This logic expands the lower bound on a "long" tooth, then compares the last
	// tooth to the current one.

	// Instead of detecting short/long, this logic first checks for "maybe short" and "maybe long",
	// then simply tests longer vs. shorter instead of absolute value.
	if (triggerType == trigger_type_e::TT_MIATA_VVT) {
		auto secondGap = (float)toothDurations[1] / toothDurations[2];

		bool currentGapOk = isInRange(triggerShape.synchronizationRatioFrom[0], (float)triggerSyncGapRatio, triggerShape.synchronizationRatioTo[0]);
		bool secondGapOk  = isInRange(triggerShape.synchronizationRatioFrom[1], secondGap,  triggerShape.synchronizationRatioTo[1]);

		// One or both teeth was impossible range, this is not the sync point
		if (!currentGapOk || !secondGapOk) {
			return false;
		}

		// If both teeth are in the range of possibility, return whether this gap is
		// shorter than the last or not.  If it is, this is the sync point.
		return triggerSyncGapRatio < secondGap;
	}

	// Normalize tooth durations by the learned per-tooth profile when a board
	// provides one (m74_9): the systematic compression ripple is divided out,
	// so the missing-teeth gap ratio stays ~3.0 even under combustion instead
	// of wandering outside the window. The weak default factor is 1.0 - no
	// behavior change for boards without a learned profile.
	int step = triggerShape.useOnlyRisingEdges ? 2 : 1;
	int teethPerRev = (int)triggerShape.getSize() / step;
	int currentTooth = currentCycle.current_index / step;

	for (int i = 0; i < triggerShape.gapTrackingLength; i++) {
		auto from = triggerShape.synchronizationRatioFrom[i];
		auto to = triggerShape.synchronizationRatioTo[i];

		if (std::isnan(from)) {
			// don't check this gap, skip it
			continue;
		}

		// duration i belongs to the tooth (currentTooth - 1 - i), wrapping
		// around one crank revolution.
		int toothI = (currentTooth - 1 - i + teethPerRev * 2) % teethPerRev;
		int toothI1 = (currentTooth - 2 - i + teethPerRev * 2) % teethPerRev;

		float factorI = triggerGetToothProfileFactor(toothI);
		float factorI1 = triggerGetToothProfileFactor(toothI1);
		if (factorI <= 0.01f || factorI1 <= 0.01f) {
			// Not learned yet - fall back to the raw durations.
			factorI = 1.0f;
			factorI1 = 1.0f;
		}

		// Normalized comparison in the multiply form (no division in the ISR):
		//   (d[i]/fI) > (d[i+1]/fI1) * from  <=>  d[i] * fI1 > d[i+1] * fI * from
		// is an equivalent comparison to
		// toothDurations[i] > toothDurations[i+1] * from
		bool isGapCondition =
			  (toothDurations[i] * factorI1 > toothDurations[i + 1] * factorI * from
			&& toothDurations[i] * factorI1 < toothDurations[i + 1] * factorI * to);

		if (!isGapCondition) {
			return false;
		}
	}

	return true;
}

/**
 * Trigger shape is defined in a way which is convenient for trigger shape definition
 * On the other hand, trigger decoder indexing begins from synchronization event.
 *
 * This function finds the index of synchronization event within TriggerWaveform
 */
uint32_t TriggerDecoderBase::findTriggerZeroEventIndex(
		TriggerWaveform& shape,
		const TriggerConfiguration& triggerConfiguration) {
#if EFI_PROD_CODE
	efiAssert(ObdCode::CUSTOM_ERR_ASSERT, hasLotsOfRemainingStack(), "findPos", -1);
#endif


	resetState();

	if (shape.shapeDefinitionError) {
		return 0;
	}

	expected<uint32_t> syncIndex = TriggerStimulatorHelper::findTriggerSyncPoint(shape,
			triggerConfiguration,
			*this);
	if (!syncIndex) {
		return EFI_ERROR_CODE;
	}

	// Assert that we found the sync point on the very first revolution
	efiAssert(ObdCode::CUSTOM_ERR_ASSERT, getSynchronizationCounter() == 0, "findZero_revCounter", EFI_ERROR_CODE);

#if EFI_UNIT_TEST
	if (printTriggerDebug) {
		printf("findTriggerZeroEventIndex: syncIndex located %u!\r\n", syncIndex.Value);
	}
#endif /* EFI_UNIT_TEST */

	TriggerStimulatorHelper::assertSyncPosition(triggerConfiguration,
			syncIndex.Value, *this, shape);

	return syncIndex.Value % shape.getSize();
}

#endif /* EFI_SHAFT_POSITION_INPUT */

