/**
 * @file fuel_schedule.cpp
 *
 * Handles injection scheduling
 */

#include "pch.h"
#include "event_registry.h"

#if EFI_ENGINE_CONTROL

FuelSchedule::FuelSchedule() {
	for (int cylinderIndex = 0; cylinderIndex < MAX_CYLINDER_COUNT; cylinderIndex++) {
		elements[cylinderIndex].ownIndex = cylinderIndex;
	}
}

void FuelSchedule::invalidate() {
	isReady = false;
}

void FuelSchedule::resetOverlapping() {
	for (size_t i = 0; i < efi::size(enginePins.injectors); i++) {
		enginePins.injectors[i].reset();
	}
}

// Determines how much to adjust injection opening angle based on the injection's duration and the current phasing mode
static float getInjectionAngleCorrection(float fuelMs, float oneDegreeUs) {
	auto mode = engineConfiguration->injectionTimingMode;
	if (mode == InjectionTimingMode::Start) {
		// Start of injection gets no correction for duration
		return 0;
	}

	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(fuelMs), "NaN fuelMs", false);

	angle_t injectionDurationAngle = MS2US(fuelMs) / oneDegreeUs;
	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(injectionDurationAngle), "NaN injectionDurationAngle", false);
	assertAngleRange(injectionDurationAngle, "injectionDuration_r", CUSTOM_INJ_DURATION);

	if (mode == InjectionTimingMode::Center) {
		// Center of injection is half-corrected for duration
		return injectionDurationAngle * 0.5f;
	} else {
			// End of injection gets "full correction" so we advance opening by the full duration
			return injectionDurationAngle;
	}
}

/**
 * @returns false in case of error, true if success
 */
bool FuelSchedule::addFuelEventsForCylinder(int i ) {
	floatus_t oneDegreeUs = engine->rpmCalculator.oneDegreeUs; // local copy
	if (cisnan(oneDegreeUs)) {
		// in order to have fuel schedule we need to have current RPM
		// wonder if this line slows engine startup?
		return false;
	}

	// injection phase may be scheduled by injection end, so we need to step the angle back
	// for the duration of the injection
	angle_t injectionDurationAngle = getInjectionAngleCorrection(engine->injectionDuration, oneDegreeUs);

	// User configured offset - degrees after TDC combustion
	floatus_t injectionOffset = engine->engineState.injectionOffset;
	if (cisnan(injectionOffset)) {
		// injection offset map not ready - we are not ready to schedule fuel events
		return false;
	}

	angle_t openingAngle = injectionOffset - injectionDurationAngle;
	assertAngleRange(openingAngle, "openingAngle_r", CUSTOM_ERR_6554);

	injection_mode_e mode = engine->getCurrentInjectionMode();

	// We need two outputs if:
	// - we are running batch fuel, and have "use two wire batch" enabled
	// - running mode is sequential, but cranking mode is batch, so we should run two wire batch while cranking
	//     (if we didn't, only half of injectors would fire while cranking)
	bool isTwoWireBatch = engineConfiguration->twoWireBatchInjection || (engineConfiguration->injectionMode == IM_SEQUENTIAL);

	int injectorIndex;
	if (mode == IM_SIMULTANEOUS || mode == IM_SINGLE_POINT) {
		// These modes only have one injector
		injectorIndex = 0;
	} else if (mode == IM_SEQUENTIAL || (mode == IM_BATCH && isTwoWireBatch)) {
		// Map order index -> cylinder index (firing order)
		injectorIndex = getCylinderId(i) - 1;
	} else if (mode == IM_BATCH) {
		// Loop over the first half of the firing order twice
		injectorIndex = i % (engineConfiguration->specs.cylindersCount / 2);
	} else {
		firmwareError(CUSTOM_OBD_UNEXPECTED_INJECTION_MODE, "Unexpected injection mode %d", mode);
		injectorIndex = 0;
	}

	InjectorOutputPin *secondOutput;

	if (mode == IM_BATCH && isTwoWireBatch) {
		/**
		 * also fire the 2nd half of the injectors so that we can implement a batch mode on individual wires
		 */
		// Compute the position of this cylinder's twin in the firing order
		// Each injector gets fired as a primary (the same as sequential), but also
		// fires the injector 360 degrees later in the firing order.
		int secondOrder = (i + (engineConfiguration->specs.cylindersCount / 2)) % engineConfiguration->specs.cylindersCount;
		int secondIndex = getCylinderId(secondOrder) - 1;
		secondOutput = &enginePins.injectors[secondIndex];
	} else {
		secondOutput = nullptr;
	}

	InjectorOutputPin *output = &enginePins.injectors[injectorIndex];
	bool isSimultanious = mode == IM_SIMULTANEOUS;

	InjectionEvent *ev = &elements[i];

	ev->outputs[0] = output;
	ev->outputs[1] = secondOutput;
	ev->isSimultanious = isSimultanious;
	// Stash the cylinder number so we can select the correct fueling bank later
	ev->cylinderNumber = injectorIndex;

	if (!isSimultanious && !output->isInitialized()) {
		// todo: extract method for this index math
		warning(CUSTOM_OBD_INJECTION_NO_PIN_ASSIGNED, "no_pin_inj #%s", output->name);
	}

	// Convert from cylinder-relative to cylinder-1-relative
	openingAngle += getCylinderAngle(i, ev->cylinderNumber);

	if (TRIGGER_WAVEFORM(getSize()) < 1) {
		warning(CUSTOM_ERR_NOT_INITIALIZED_TRIGGER, "uninitialized TriggerWaveform");
		return false;
	}

	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(openingAngle), "findAngle#3", false);
	assertAngleRange(openingAngle, "findAngle#a33", CUSTOM_ERR_6544);

	wrapAngle2(openingAngle, "addFuel#2", CUSTOM_ERR_6555, getEngineCycle(engine->triggerCentral.triggerShape.getOperationMode()));
	ev->injectionStartAngle = openingAngle;

#if EFI_UNIT_TEST
	printf("registerInjectionEvent openingAngle=%.2f inj %d\r\n", openingAngle, injectorIndex);
#endif
	return true;
}

void FuelSchedule::addFuelEvents() {
	for (size_t cylinderIndex = 0; cylinderIndex < engineConfiguration->specs.cylindersCount; cylinderIndex++) {
		bool result = addFuelEventsForCylinder(cylinderIndex);

		if (!result) {
			invalidate();
			return;
		}
	}

	// We made it through all cylinders, mark the schedule as ready so it can be used
	isReady = true;
}

void FuelSchedule::onTriggerTooth(int rpm, efitick_t nowNt, float currentPhase, float nextPhase) {
	// Wait for schedule to be built - this happens the first time we get RPM
	if (!isReady) {
		return;
	}

	for (size_t i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
		elements[i].onTriggerTooth(rpm, nowNt, currentPhase, nextPhase);
	}
}

#endif
