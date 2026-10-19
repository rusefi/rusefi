/**
 * @file fuel_schedule.cpp
 *
 * Handles injection scheduling
 */

#include "pch.h"

void startInjection(scheduler_arg_t arg) {
	const InjectorContext ctx(arg);
	const auto nowNt = getTimeNowNt();
	CylinderMask mask = ctx.outputsMask();
	for (uint16_t i = 0; mask; mask >>= 1, i++) {
		if (mask & 1u) {
			enginePins.injectors[i].open(nowNt);
			if (ctx.stage2Active()) {
				enginePins.injectorsStage2[i].open(nowNt);
			}
		}
	}
}

void endInjection(scheduler_arg_t arg) {
	const InjectorContext ctx(arg);
	const auto nowNt = getTimeNowNt();
	CylinderMask mask = ctx.outputsMask();
	for (uint16_t i = 0; mask; mask >>= 1, i++) {
		if (mask & 1u) {
			enginePins.injectors[i].close(nowNt);
		}
	}

	// Priming also uses this helper on boards without engine control. Only
	// scheduled fuel events need their injection angle refreshed afterward.
#if EFI_ENGINE_CONTROL
	if (ctx.eventIndex() < MAX_CYLINDER_COUNT) {
		getFuelSchedule()->elements[ctx.eventIndex()].update();
	}
#endif
}

void endInjectionStage2(scheduler_arg_t arg) {
	const InjectorContext ctx(arg);
	const auto nowNt = getTimeNowNt();
	CylinderMask mask = ctx.outputsMask();
	for (uint16_t i = 0; mask; mask >>= 1, i++) {
		if (mask & 1u) {
			enginePins.injectorsStage2[i].close(nowNt);
		}
	}
}

#if EFI_ENGINE_CONTROL

CylinderMask InjectionEvent::calculateInjectorOutputMask() const {
	switch (m_injectionMode) {
		case IM_SIMULTANEOUS:
			return (1u << engineConfiguration->cylindersCount) - 1;
		case IM_SINGLE_POINT:
			return 1;
		case IM_BATCH: {
			const auto count = engineConfiguration->cylindersCount;
			const auto twin = getCylinderNumberAtIndex((ownIndex + count / 2) % count);
			return (1u << cylinderNumber) | (1u << twin);
		}
		case IM_SEQUENTIAL:
			return 1u << cylinderNumber;
		default:
			return 0;
	}
}

FuelSchedule::FuelSchedule() {
	for (int cylinderIndex = 0; cylinderIndex < MAX_CYLINDER_COUNT; cylinderIndex++) {
		elements[cylinderIndex].setIndex(cylinderIndex);
	}
}

void FuelSchedule::invalidate() {
	isReady = false;
}

// Determines how much to adjust injection opening angle based on the injection's duration and the current phasing mode
static float getInjectionAngleCorrection(float fuelMs, float oneDegreeUs) {
	auto mode = engineConfiguration->injectionTimingMode;
	if (mode == InjectionTimingMode::Start) {
		// Start of injection gets no correction for duration
		return 0;
	}

	efiAssert(ObdCode::CUSTOM_ERR_ASSERT, !std::isnan(fuelMs), "NaN fuelMs", false);

	angle_t injectionDurationAngle = MS2US(fuelMs) / oneDegreeUs;
	efiAssert(ObdCode::CUSTOM_ERR_ASSERT, !std::isnan(injectionDurationAngle), "NaN injectionDurationAngle", false);
	assertAngleRange(injectionDurationAngle, "injectionDuration_r", ObdCode::CUSTOM_INJ_DURATION);

	if (mode == InjectionTimingMode::Center) {
		// Center of injection is half-corrected for duration
		return injectionDurationAngle * 0.5f;
	} else {
			// End of injection gets "full correction" so we advance opening by the full duration
			return injectionDurationAngle;
	}
}

// Returns the start angle of this injector in engine coordinates (0-720 for a 4 stroke),
// or unexpected if unable to calculate the start angle due to missing information.
expected<float> InjectionEvent::computeInjectionAngle() const {
	floatus_t oneDegreeUs = getEngineRotationState()->getOneDegreeUs(); // local copy
	if (std::isnan(oneDegreeUs)) {
		// in order to have fuel schedule we need to have current RPM
		// wonder if this line slows engine startup?
		return unexpected;
	}

	float fuelMs = getEngineState()->injectionDuration;
	if (std::isnan(fuelMs)) {
		return unexpected;
	}

	// injection phase may be scheduled by injection end, so we need to step the angle back
	// for the duration of the injection
	angle_t injectionDurationAngle = getInjectionAngleCorrection(fuelMs, oneDegreeUs);

	// The table sets Start/Center/End of the nominal
	// pulse in crank degrees after this cylinder's compression TDC. Negative table
	// values wrap through the engine cycle; subtract the selected duration correction
	// before applying the cylinder's firing-order offset below. See Injection Timing help.
	floatus_t injectionOffset = getEngineState()->injectionOffset;
	if (std::isnan(injectionOffset)) {
		// injection offset map not ready - we are not ready to schedule fuel events
		return unexpected;
	}

	angle_t openingAngle = injectionOffset - injectionDurationAngle;
	assertAngleRange(openingAngle, "openingAngle_r", ObdCode::CUSTOM_ERR_6554);
	wrapAngle(openingAngle, "addFuel#1", ObdCode::CUSTOM_ERR_6555);
	// TODO: should we log per-cylinder injection timing? #76
	getTunerStudioOutputChannels()->injectionOffset = openingAngle;

	// Convert from cylinder-relative to cylinder-1-relative
	openingAngle += getPerCylinderFiringOrderOffset(ownIndex, cylinderNumber);

	efiAssert(ObdCode::CUSTOM_ERR_ASSERT, !std::isnan(openingAngle), "findAngle#3", false);
	assertAngleRange(openingAngle, "findAngle#a33", ObdCode::CUSTOM_ERR_6544);

	wrapAngle(openingAngle, "addFuel#2", ObdCode::CUSTOM_ERR_6555);

#if EFI_UNIT_TEST
//	printf("registerInjectionEvent openingAngle=%.2f inj %d\r\n", openingAngle, cylinderNumber);
#endif

	return openingAngle;
}

bool InjectionEvent::updateInjectionAngle() {
	auto result = computeInjectionAngle();

	if (result) {
		// If injector duty cycle is high, lock injection SOI so that we
		// don't miss injections at or above 100% duty
		if (getEngineState()->shouldUpdateInjectionTiming) {
			injectionStartAngle = result.Value;
		}

		return true;
	} else {
		return false;
	}
}

/**
 * @returns false in case of error, true if success
 */
bool InjectionEvent::update() {
	bool updatedAngle = updateInjectionAngle();

	if (!updatedAngle) {
		return false;
	}

	injection_mode_e mode = getCurrentInjectionMode();
	engine->outputChannels.currentInjectionMode = static_cast<uint8_t>(mode);

	int injectorIndex;
	if (mode == IM_SIMULTANEOUS || mode == IM_SINGLE_POINT) {
		// These modes only have one injector
		injectorIndex = 0;
	} else if (mode == IM_SEQUENTIAL || mode == IM_BATCH) {
		// Map order index -> cylinder index (firing order)
		injectorIndex = getCylinderNumberAtIndex(ownIndex);
	} else {
		firmwareError(ObdCode::CUSTOM_OBD_UNEXPECTED_INJECTION_MODE, "Unexpected injection mode %d", mode);
		injectorIndex = 0;
	}

	InjectorOutputPin *output = &enginePins.injectors[injectorIndex];
	isSimultaneous = mode == IM_SIMULTANEOUS;
	m_injectionMode = mode;
	// Stash the cylinder number so we can select the correct fueling bank later
	cylinderNumber = injectorIndex;

	if (!isSimultaneous && !output->isInitialized()) {
		// todo: extract method for this index math
		warning(ObdCode::CUSTOM_OBD_INJECTION_NO_PIN_ASSIGNED, "no_pin_inj #%s", output->getName());
	}

	return true;
}

void FuelSchedule::addFuelEvents() {
	for (size_t cylinderIndex = 0; cylinderIndex < engineConfiguration->cylindersCount; cylinderIndex++) {
		bool result = elements[cylinderIndex].update();

		if (!result) {
			invalidate();
			return;
		}
	}

	// We made it through all cylinders, mark the schedule as ready so it can be used
	isReady = true;
}

void FuelSchedule::onTriggerTooth(efitick_t nowNt, float currentPhase, float nextPhase) {
	// Wait for schedule to be built - this happens the first time we get RPM
	if (!isReady) {
		return;
	}

	for (size_t i = 0; i < engineConfiguration->cylindersCount; i++) {
		elements[i].onTriggerTooth(nowNt, currentPhase, nextPhase);
	}
}

#endif // EFI_ENGINE_CONTROL
