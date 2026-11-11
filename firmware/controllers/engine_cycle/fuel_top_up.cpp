/**
 * @file fuel_top_up.cpp
 *
 * [tag:fuel_top_up] Top-up pulse: fuel that the main pulse of a cylinder did not
 * deliver because the requested mass rose after that pulse was scheduled (a throttle
 * opening after the injection timing). The window belongs to one main pulse: it opens
 * when that pulse opens and closes at the first limit angle after it (normally intake
 * valve closing), so a top-up can never be attributed to the previous cycle's pulse.
 * On each trigger tooth in the window the mass requested now is compared with the mass
 * registered for the main pulse, and the difference is injected once, as late as
 * possible so that it covers as much of the rise as the window allows:
 *  - while the main pulse is still open, by extending it at the last tooth before it
 *    closes: the injector opens again (overlappingCounter 2) and the extra close comes
 *    after the main close, so the pin stays high without a second dead time;
 *  - after the main pulse closed, as a separate pulse at the last tooth from which the
 *    pulse still fits, at least fuelTopUpMinOffTime after the main close.
 * When the whole difference no longer fits before the limit, the part that fits is
 * injected. While the top-up pulse is scheduled or open, a request that keeps rising
 * moves its close later by the plain flow time of the new fuel (fuelTopUpFollowRequest),
 * without a second dead time; once the injector closed it is not reopened.
 *
 * Safety invariants, see the tests in test_fuel_top_up.cpp:
 *  - top-up events use their own scheduling_s, never the shared pool;
 *  - the close is scheduled before the open, and onTopUpOpen() refuses to open unless
 *    its close is armed, so a lost close cannot leave an injector open;
 *  - moving the close cancels and reschedules it inside one critical section, only while
 *    it is still pending, so a close is armed at every instant;
 *  - a separate top-up pulse opens only on an idle injector (overlappingCounter 0) and
 *    an extension only on an open one; a mismatch cancels the close, so it can neither
 *    leave the injector open nor cut a main pulse short;
 *  - top-up actions never call InjectionEvent::update(), which the main close uses to
 *    move the next start angle;
 *  - the pulse must end before the limit angle and before the next main pulse start;
 *  - evaluation runs inside handleFuel(), after LimpManager::allowInjection(), and is
 *    skipped while cranking, above fuelTopUpMaxRpm, at high injector duty (start
 *    angle locked) and outside sequential injection.
 */

#include "pch.h"

#if EFI_ENGINE_CONTROL

#include "injector_model.h"

static constexpr float FUEL_TOP_UP_EXTENSION_MARGIN_US = 200;
// Shortest pulse the scheduler handles safely (see onTriggerTooth()).
static constexpr float FUEL_TOP_UP_MIN_SCHEDULER_MS = 0.05f;
// Late decision: wait for the next tooth only if a pulse this much longer would still
// fit from there, since the missing fuel usually keeps growing.
static constexpr float FUEL_TOP_UP_WAIT_GROWTH = 1.25f;
// Smallest move of a top-up close: smaller growth waits for the next tooth.
static constexpr float FUEL_TOP_UP_MIN_MOVE_US = 20;

static TpsAccelEnrichment& topUpState() {
	return engine->module<TpsAccelEnrichment>().unmock();
}

static float wrapToCycle(float angle) {
	float cycle = getEngineState()->engineCycle;
	while (angle < 0) {
		angle += cycle;
	}
	while (angle >= cycle) {
		angle -= cycle;
	}
	return angle;
}

static void setSkip(FuelTopUpSkip reason) {
	topUpState().fuelTopUpSkipReason = static_cast<uint8_t>(reason);
}

static void topUpOpenAction(InjectionEvent* event) {
	event->onTopUpOpen();
}

static void topUpCloseAction(InjectionEvent* event) {
	event->onTopUpClose();
}

void InjectionEvent::registerMainPulse(float massGrams, float startAngle, efitick_t startNt, efitick_t closeNt) {
	m_registeredMass = massGrams;
	m_mainMass = massGrams;
	m_mainStartAngle = startAngle;
	// A top-up of the previous main pulse keeps its close but no longer follows.
	m_topUpFollowing = false;
	m_mainStartNt = startNt;
	m_mainCloseNt = closeNt;
	m_topUpPending = config->fuelTopUpMode != FUEL_TOP_UP_OFF;
	if (ownIndex == 0) {
		topUpState().fuelTopUpRegistered = massGrams * 1000;
	}
}

void InjectionEvent::resetTopUp() {
	m_topUpPending = false;
	m_topUpFollowing = false;
	m_registeredMass = 0;
	m_mainMass = 0;
}

void FuelSchedule::resetTopUp() {
	for (auto& element : elements) {
		element.resetTopUp();
	}
}

void InjectionEvent::evaluateTopUp(efitick_t nowNt, float currentPhase, float nextPhase) {
	if (!m_topUpPending && !m_topUpFollowing) {
		return;
	}
	if (config->fuelTopUpMode == FUEL_TOP_UP_OFF) {
		m_topUpPending = false;
		m_topUpFollowing = false;
		return;
	}
	if (nowNt < m_mainStartNt) {
		// The window opens with the main pulse.
		return;
	}

	float rpm = engine->rpmCalculator.getCachedRpm();
	float limitAngle = interpolate2d(rpm, config->fuelTopUpRpmBins, config->fuelTopUpLimitAngle);
	// The limit is cylinder-relative like the injection timing table. The window ends at
	// the first limit angle after this main pulse opened.
	float cylinderOffset = getPerCylinderFiringOrderOffset(ownIndex, cylinderNumber);
	float limitFromStart = wrapToCycle(limitAngle + cylinderOffset - m_mainStartAngle);
	float sinceStart = wrapToCycle(currentPhase - m_mainStartAngle);
	if (sinceStart >= limitFromStart) {
		// The intake valve has closed on this main pulse: its top-up chance is over.
		m_topUpPending = false;
		m_topUpFollowing = false;
		return;
	}

	float requested = getEngineState()->injectionMass[cylinderNumber];
	auto& state = topUpState();
	if (ownIndex == 0) {
		state.fuelTopUpRequested = requested * 1000;
	}

	floatus_t oneDegreeUs = getEngineRotationState()->getOneDegreeUs();
	if (!std::isfinite(oneDegreeUs) || oneDegreeUs <= 0) {
		return;
	}
	float usToLimit = (limitFromStart - sinceStart) * oneDegreeUs;
	float usToNextStart = wrapToCycle(injectionStartAngle - currentPhase) * oneDegreeUs;

	if (m_topUpFollowing) {
		float minOffUs = config->fuelTopUpMinOffTime * 1000;
		followTopUp(nowNt, requested, minF(usToLimit, usToNextStart - minOffUs));
		return;
	}

	// fuelTopUpMaxPercent bounds the whole top-up of this main pulse.
	float missing = minF(requested - m_registeredMass,
		m_mainMass * config->fuelTopUpMaxPercent * 0.01f - (m_registeredMass - m_mainMass));
	if (!(missing > 0)) {
		return;
	}

	if (m_injectionMode != IM_SEQUENTIAL) {
		setSkip(FuelTopUpSkip::NotSequential);
		return;
	}
	if (engine->rpmCalculator.isCranking()) {
		setSkip(FuelTopUpSkip::Cranking);
		return;
	}
	if (rpm > config->fuelTopUpMaxRpm) {
		setSkip(FuelTopUpSkip::RpmTooHigh);
		return;
	}
	if (!getEngineState()->shouldUpdateInjectionTiming) {
		setSkip(FuelTopUpSkip::DutyTooHigh);
		return;
	}
	if (state.fuelTopUpFault) {
		setSkip(FuelTopUpSkip::Fault);
		return;
	}
	if (missing * 1000 < config->fuelTopUpMinMass) {
		// The request can still grow before the limit.
		setSkip(FuelTopUpSkip::BelowMinimum);
		return;
	}

	float usToNextTooth = wrapToCycle(nextPhase - currentPhase) * oneDegreeUs;
	scheduleTopUp(nowNt, missing, usToLimit, usToNextStart, usToNextTooth);
}

bool InjectionEvent::followTopUp(efitick_t nowNt, float requestedGrams, float endBudgetUs) {
	auto& state = topUpState();
	if (state.fuelTopUpFault || config->fuelTopUpMode != FUEL_TOP_UP_INJECT) {
		m_topUpFollowing = false;
		return false;
	}
	float extra = minF(requestedGrams - m_registeredMass,
		m_mainMass * config->fuelTopUpMaxPercent * 0.01f - (m_registeredMass - m_mainMass));
	if (!(extra > 0)) {
		return false;
	}
	auto& injector = engine->module<InjectorModelPrimary>();
	float flowPerMs = injector->getFuelMassForDuration(1);
	if (!(flowPerMs > 0)) {
		return false;
	}
	float commanded = wallFuel.getTopUpCommandedMass(extra);
	float filmFactor = commanded / extra;
	// The injector is (or will be) open already: plain flow time, no dead time.
	float extraMs = commanded / flowPerMs;
	float availableMs = (endBudgetUs - NT2USF(m_topUpCloseNt - nowNt)) / 1000;
	if (extraMs > availableMs) {
		extraMs = availableMs;
		commanded = extraMs * flowPerMs;
	}
	if (!(extraMs * 1000 >= FUEL_TOP_UP_MIN_MOVE_US)) {
		if (!(availableMs * 1000 >= FUEL_TOP_UP_MIN_MOVE_US)) {
			// No room left before the limit or the next main pulse, and it only shrinks.
			m_topUpFollowing = false;
		}
		return false;
	}
	efitick_t newCloseNt = sumTickAndFloat(m_topUpCloseNt, MSF2NT(extraMs));

	{
		chibios_rt::CriticalSectionLocker csl;
		if (!m_topUpCloseScheduling.action) {
			// Already closed (or cancelled): reopening would cost another dead time.
			m_topUpFollowing = false;
			return false;
		}
		// Cancel and reschedule under one lock, so a close is armed at every instant.
		getScheduler()->cancel(&m_topUpCloseScheduling);
		getScheduler()->schedule("top-up close", &m_topUpCloseScheduling, newCloseNt, action_s::make<topUpCloseAction>(this));
	}
	m_topUpCloseNt = newCloseNt;

	wallFuel.commitTopUp(commanded);
	m_registeredMass += commanded / filmFactor;
	state.fuelTopUpLastMass += commanded * 1000;
	state.fuelTopUpLastPulse += extraMs;
	state.fuelTopUpMoveCount++;

#if EFI_VEHICLE_SPEED && defined(MODULE_ODOMETER)
	engine->module<TripOdometer>()->consumeFuel(commanded, nowNt);
#endif
	return true;
}

bool InjectionEvent::scheduleTopUp(efitick_t nowNt, float massGrams, float usToLimit, float usToNextStart, float usToNextTooth) {
	auto& state = topUpState();
	auto& injector = engine->module<InjectorModelPrimary>();
	float flowPerMs = injector->getFuelMassForDuration(1);
	if (!(flowPerMs > 0)) {
		return false;
	}
	float commanded = wallFuel.getTopUpCommandedMass(massGrams);
	// Commanded mass per pre-film mass, to book a clamped top-up consistently.
	float filmFactor = commanded / massGrams;
	float minOffUs = config->fuelTopUpMinOffTime * 1000;
	// Both pulse types must end before the limit and leave the minimum off time before
	// the next main pulse of this cylinder.
	float endBudgetUs = minF(usToLimit, usToNextStart - minOffUs);
	float usToMainClose = NT2USF(m_mainCloseNt - nowNt);

	bool extension = config->fuelTopUpExtendOpen && usToMainClose > FUEL_TOP_UP_EXTENSION_MARGIN_US;

	float openUs;
	float pulseMs;
	if (extension) {
		// The injector is already open: no dead time, plain flow rate.
		pulseMs = commanded / flowPerMs;
		// Wait while the next tooth still comes before the main close and the extension
		// would still fit from there.
		if (usToNextTooth + FUEL_TOP_UP_EXTENSION_MARGIN_US < usToMainClose
				&& usToMainClose + MS2US(pulseMs) * FUEL_TOP_UP_WAIT_GROWTH <= endBudgetUs) {
			setSkip(FuelTopUpSkip::Waiting);
			return false;
		}
		openUs = 0;
		float availableMs = (endBudgetUs - usToMainClose) / 1000;
		if (pulseMs > availableMs) {
			// Inject what fits.
			pulseMs = availableMs;
			commanded = pulseMs * flowPerMs;
		}
	} else {
		pulseMs = injector->getInjectionDuration(commanded);
		if (pulseMs - injector->getDeadtime() < config->fuelTopUpMinPulse) {
			// The request can still grow before the limit.
			setSkip(FuelTopUpSkip::BelowMinimum);
			return false;
		}
		float earliestUs = maxF(0, usToMainClose + minOffUs);
		float openFromNextToothUs = maxF(usToNextTooth, earliestUs);
		if (openFromNextToothUs + MS2US(pulseMs) * FUEL_TOP_UP_WAIT_GROWTH <= endBudgetUs) {
			setSkip(FuelTopUpSkip::Waiting);
			return false;
		}
		openUs = earliestUs;
		float availableMs = (endBudgetUs - openUs) / 1000;
		if (pulseMs > availableMs) {
			// Inject what fits: shorten the flow time, the dead time stays.
			float deadtimeMs = injector->getDeadtime();
			for (int i = 0; i < 3 && pulseMs > availableMs; i++) {
				float flowMs = pulseMs - deadtimeMs;
				float wantedFlowMs = availableMs - deadtimeMs;
				if (!(flowMs > 0) || !(wantedFlowMs > 0)) {
					commanded = 0;
					break;
				}
				// Small pulse corrections are not linear: scale and check again.
				commanded *= wantedFlowMs / flowMs;
				pulseMs = injector->getInjectionDuration(commanded);
			}
			if (pulseMs - deadtimeMs < config->fuelTopUpMinPulse) {
				commanded = 0;
			}
		}
	}

	// Pre-film mass of what is injected, compared with the same minimum as the request.
	float bookedMass = commanded / filmFactor;
	if (!(std::isfinite(pulseMs) && pulseMs >= FUEL_TOP_UP_MIN_SCHEDULER_MS)
			|| !(commanded > 0 && bookedMass * 1000 >= config->fuelTopUpMinMass)
			|| openUs + MS2US(pulseMs) > endBudgetUs + 1) {
		// Not even a minimum pulse fits, and later teeth only have less room.
		setSkip(FuelTopUpSkip::PastLimit);
		m_topUpPending = false;
		return false;
	}

	efitick_t openNt;
	efitick_t closeNt;
	if (extension) {
		openNt = nowNt;
		closeNt = sumTickAndFloat(m_mainCloseNt, MSF2NT(pulseMs));
	} else {
		openNt = sumTickAndFloat(nowNt, USF2NT(openUs));
		closeNt = sumTickAndFloat(openNt, MSF2NT(pulseMs));
	}

	if (config->fuelTopUpMode == FUEL_TOP_UP_LOG_ONLY) {
		state.fuelTopUpWouldInject = true;
		state.fuelTopUpLastMass = commanded * 1000;
		state.fuelTopUpLastPulse = pulseMs;
		setSkip(FuelTopUpSkip::None);
		m_topUpPending = false;
		return true;
	}

	{
		chibios_rt::CriticalSectionLocker csl;
		if (m_topUpOpenScheduling.action || m_topUpCloseScheduling.action) {
			setSkip(FuelTopUpSkip::SchedulerBusy);
			return false;
		}
		m_topUpMask = calculateInjectorOutputMask();
		m_topUpIsExtension = extension;
		// Close first: onTopUpOpen() opens only while this close is armed.
		getScheduler()->schedule("top-up close", &m_topUpCloseScheduling, closeNt, action_s::make<topUpCloseAction>(this));
		getScheduler()->schedule("top-up open", &m_topUpOpenScheduling, openNt, action_s::make<topUpOpenAction>(this));
	}

	wallFuel.commitTopUp(commanded);
	m_registeredMass += bookedMass;
	m_topUpPending = false;
	m_topUpCloseNt = closeNt;
	m_topUpFollowing = config->fuelTopUpFollowRequest;

	state.fuelTopUpWouldInject = false;
	state.fuelTopUpLastMass = commanded * 1000;
	state.fuelTopUpLastPulse = pulseMs;
	state.fuelTopUpCount++;
	if (extension) {
		state.fuelTopUpExtensionCount++;
	}
	setSkip(FuelTopUpSkip::None);

#if EFI_VEHICLE_SPEED && defined(MODULE_ODOMETER)
	engine->module<TripOdometer>()->consumeFuel(commanded, nowNt);
#endif
	return true;
}

void InjectionEvent::onTopUpOpen() {
	auto& state = topUpState();
	if (!m_topUpCloseScheduling.action) {
		// Never open without a pending close.
		state.fuelTopUpFault = true;
		return;
	}

	bool injectorStateMatches = true;
	CylinderMask mask = m_topUpMask;
	for (uint16_t i = 0; mask; mask >>= 1, i++) {
		if (mask & 1u) {
			bool isOpen = enginePins.injectors[i].getOverlappingCounter() > 0;
			if (isOpen != m_topUpIsExtension) {
				injectorStateMatches = false;
			}
		}
	}

	if (!injectorStateMatches) {
		// The close must not run either: for an extension the main pulse already closed,
		// for a separate pulse another pulse holds the injector and would be cut short.
		getScheduler()->cancel(&m_topUpCloseScheduling);
		m_topUpFollowing = false;
		if (m_topUpIsExtension) {
			setSkip(FuelTopUpSkip::LateForExtension);
		} else {
			state.fuelTopUpFault = true;
			setSkip(FuelTopUpSkip::Fault);
		}
		return;
	}

	efitick_t nowNt = getTimeNowNt();
	mask = m_topUpMask;
	for (uint16_t i = 0; mask; mask >>= 1, i++) {
		if (mask & 1u) {
			enginePins.injectors[i].open(nowNt);
		}
	}
}

void InjectionEvent::onTopUpClose() {
	m_topUpFollowing = false;
	efitick_t nowNt = getTimeNowNt();
	CylinderMask mask = m_topUpMask;
	for (uint16_t i = 0; mask; mask >>= 1, i++) {
		if (mask & 1u) {
			enginePins.injectors[i].close(nowNt);
		}
	}
}

#endif // EFI_ENGINE_CONTROL
