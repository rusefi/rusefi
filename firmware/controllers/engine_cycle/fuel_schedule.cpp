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
		InjectionEvent *ev = &elements[cylinderIndex];
		ev->ownIndex = cylinderIndex;
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

/**
 * @returns false in case of error, true if success
 */
bool FuelSchedule::addFuelEventsForCylinder(int i ) {
	efiAssert(CUSTOM_ERR_ASSERT, engine!=NULL, "engine is NULL", false);

	floatus_t oneDegreeUs = ENGINE(rpmCalculator.oneDegreeUs); // local copy
	if (cisnan(oneDegreeUs)) {
		// in order to have fuel schedule we need to have current RPM
		// wonder if this line slows engine startup?
		return false;
	}

	/**
	 * injection phase is scheduled by injection end, so we need to step the angle back
	 * for the duration of the injection
	 *
	 * todo: since this method is not invoked within trigger event handler and
	 * engineState.injectionOffset is calculated from the same utility timer should we more that logic here?
	 */
	floatms_t fuelMs = ENGINE(injectionDuration);
	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(fuelMs), "NaN fuelMs", false);
	angle_t injectionDurationAngle = MS2US(fuelMs) / oneDegreeUs;
	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(injectionDurationAngle), "NaN injectionDurationAngle", false);
	assertAngleRange(injectionDurationAngle, "injectionDuration_r", CUSTOM_INJ_DURATION);
	floatus_t injectionOffset = ENGINE(engineState.injectionOffset);
	if (cisnan(injectionOffset)) {
		// injection offset map not ready - we are not ready to schedule fuel events
		return false;
	}
	angle_t baseAngle = injectionOffset - injectionDurationAngle;
	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(baseAngle), "NaN baseAngle", false);
	assertAngleRange(baseAngle, "baseAngle_r", CUSTOM_ERR_6554);

	injection_mode_e mode = engine->getCurrentInjectionMode();

	int injectorIndex;
	if (mode == IM_SIMULTANEOUS || mode == IM_SINGLE_POINT) {
		// These modes only have one injector
		injectorIndex = 0;
	} else if (mode == IM_SEQUENTIAL || (mode == IM_BATCH && CONFIG(twoWireBatchInjection))) {
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
	if (mode == IM_BATCH && CONFIG(twoWireBatchInjection)) {
		/**
		 * also fire the 2nd half of the injectors so that we can implement a batch mode on individual wires
		 */
		// Compute the position of this cylinder's twin in the firing order
		// Each injector gets fired as a primary (the same as sequential), but also
		// fires the injector 360 degrees later in the firing order.
		int secondOrder = (i + (CONFIG(specs.cylindersCount) / 2)) % CONFIG(specs.cylindersCount);
		int secondIndex = getCylinderId(secondOrder) - 1;
		secondOutput = &enginePins.injectors[secondIndex];
	} else {
		secondOutput = nullptr;
	}

	InjectorOutputPin *output = &enginePins.injectors[injectorIndex];
	bool isSimultanious = mode == IM_SIMULTANEOUS;

	InjectionEvent *ev = &elements[i];

	ev->ownIndex = i;
	ev->outputs[0] = output;
	ev->outputs[1] = secondOutput;
	ev->isSimultanious = isSimultanious;
	// Stash the cylinder number so we can select the correct fueling bank later
	ev->cylinderNumber = injectorIndex;

	if (!isSimultanious && !output->isInitialized()) {
		// todo: extract method for this index math
		warning(CUSTOM_OBD_INJECTION_NO_PIN_ASSIGNED, "no_pin_inj #%s", output->name);
	}

	angle_t ignitionPositionWithinEngineCycle = ENGINE(ignitionPositionWithinEngineCycle[i]);

	float angle = baseAngle + ignitionPositionWithinEngineCycle;

	if (TRIGGER_WAVEFORM(getSize()) < 1) {
		warning(CUSTOM_ERR_NOT_INITIALIZED_TRIGGER, "uninitialized TriggerWaveform");
		return false;
	}

	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(angle), "findAngle#3", false);
	assertAngleRange(angle, "findAngle#a33", CUSTOM_ERR_6544);
	ev->injectionStart.setAngle(angle);
#if EFI_UNIT_TEST
	printf("registerInjectionEvent angle=%.2f trgIndex=%d inj %d\r\n", angle, ev->injectionStart.triggerEventIndex, injectorIndex);
#endif
	return true;
}

void FuelSchedule::addFuelEvents() {
	for (size_t cylinderIndex = 0; cylinderIndex < CONFIG(specs.cylindersCount); cylinderIndex++) {
		InjectionEvent *ev = &elements[cylinderIndex];
		ev->ownIndex = cylinderIndex;  // todo: is this assignment needed here? we now initialize in constructor
		bool result = addFuelEventsForCylinder(cylinderIndex);
		if (!result) {
			invalidate();
			return;
		}
	}

	// We made it through all cylinders, mark the schedule as ready so it can be used
	isReady = true;
}

void FuelSchedule::onTriggerTooth(size_t toothIndex, int rpm, efitick_t nowNt) {
	// Wait for schedule to be built - this happens the first time we get RPM
	if (!isReady) {
		return;
	}

	for (size_t i = 0; i < CONFIG(specs.cylindersCount); i++) {
		elements[i].onTriggerTooth(toothIndex, rpm, nowNt);
	}
}

#endif
