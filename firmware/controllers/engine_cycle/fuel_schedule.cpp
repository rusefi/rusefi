/**
 * @file fuel_schedule.cpp
 *
 * Handles injection scheduling
 */

#include "global.h"
#include "engine.h"
#include "engine_math.h"
#include "event_registry.h"
#include "perf_trace.h"

EXTERN_ENGINE;

#if EFI_ENGINE_CONTROL

FuelSchedule::FuelSchedule() {
	for (int cylinderIndex = 0; cylinderIndex < MAX_INJECTION_OUTPUT_COUNT; cylinderIndex++) {
		InjectionEvent *ev = &elements[cylinderIndex];
		ev->ownIndex = cylinderIndex;
	}
}

void FuelSchedule::resetOverlapping() {
	for (size_t i = 0; i < efi::size(enginePins.injectors); i++) {
		enginePins.injectors[i].reset();
	}
}

/**
 * @returns false in case of error, true if success
 */
bool FuelSchedule::addFuelEventsForCylinder(int i, angle_t baseAngle, injection_mode_e mode DECLARE_ENGINE_PARAMETER_SUFFIX) {
	efiAssert(CUSTOM_ERR_ASSERT, engine!=NULL, "engine is NULL", false);

	int injectorIndex;
	if (mode == IM_SIMULTANEOUS || mode == IM_SINGLE_POINT) {
		// These modes only have one injector
		injectorIndex = 0;
	} else if (mode == IM_SEQUENTIAL || (mode == IM_BATCH && CONFIG(twoWireBatchInjection))) {
		// Map order index -> cylinder index (firing order)
		injectorIndex = getCylinderId(i PASS_ENGINE_PARAMETER_SUFFIX) - 1;
	} else if (mode == IM_BATCH) {
		// Loop over the first half of the firing order twice
		injectorIndex = i % (engineConfiguration->specs.cylindersCount / 2);
	} else {
		firmwareError(CUSTOM_OBD_UNEXPECTED_INJECTION_MODE, "Unexpected injection mode %d", mode);
		injectorIndex = 0;
	}

	assertAngleRange(baseAngle, "addFbaseAngle", CUSTOM_ADD_BASE);

	int cylindersCount = CONFIG(specs.cylindersCount);
	if (cylindersCount < 1) {
	    // May 2020 this somehow still happens with functional tests, maybe race condition?
		warning(CUSTOM_OBD_ZERO_CYLINDER_COUNT, "Invalid cylinder count: %d", cylindersCount);
		return false;
	}

	float angle = baseAngle
			+ i * ENGINE(engineCycle) / cylindersCount;

	InjectorOutputPin *secondOutput;
	if (mode == IM_BATCH && CONFIG(twoWireBatchInjection)) {
		/**
		 * also fire the 2nd half of the injectors so that we can implement a batch mode on individual wires
		 */
		// Compute the position of this cylinder's twin in the firing order
		// Each injector gets fired as a primary (the same as sequential), but also
		// fires the injector 360 degrees later in the firing order.
		int secondOrder = (i + (CONFIG(specs.cylindersCount) / 2)) % CONFIG(specs.cylindersCount);
		int secondIndex = getCylinderId(secondOrder PASS_ENGINE_PARAMETER_SUFFIX) - 1;
		secondOutput = &enginePins.injectors[secondIndex];
	} else {
		secondOutput = nullptr;
	}

	InjectorOutputPin *output = &enginePins.injectors[injectorIndex];
	bool isSimultanious = mode == IM_SIMULTANEOUS;

	if (!isSimultanious && !output->isInitialized()) {
		// todo: extract method for this index math
		warning(CUSTOM_OBD_INJECTION_NO_PIN_ASSIGNED, "no_pin_inj #%s", output->name);
	}

	InjectionEvent *ev = &elements[i];
	ev->ownIndex = i;
	INJECT_ENGINE_REFERENCE(ev);
	fixAngle(angle, "addFuel#1", CUSTOM_ERR_6554);

	ev->outputs[0] = output;
	ev->outputs[1] = secondOutput;

	ev->isSimultanious = isSimultanious;

	if (TRIGGER_WAVEFORM(getSize()) < 1) {
		warning(CUSTOM_ERR_NOT_INITIALIZED_TRIGGER, "uninitialized TriggerWaveform");
		return false;
	}

	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(angle), "findAngle#3", false);
	assertAngleRange(angle, "findAngle#a33", CUSTOM_ERR_6544);
	ev->injectionStart.setAngle(angle PASS_ENGINE_PARAMETER_SUFFIX);
#if EFI_UNIT_TEST
	printf("registerInjectionEvent angle=%.2f trgIndex=%d inj %d\r\n", angle, ev->injectionStart.triggerEventIndex, injectorIndex);
#endif
	return true;
}

void FuelSchedule::addFuelEvents(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	ScopePerf perf(PE::Temporary1);

	floatus_t oneDegreeUs = ENGINE(rpmCalculator.oneDegreeUs); // local copy
	if (cisnan(oneDegreeUs)) {
		// in order to have fuel schedule we need to have current RPM
		// wonder if this line slows engine startup?
		isReady = false;
		return;
	}

	/**
	 * injection phase is scheduled by injection end, so we need to step the angle back
	 * for the duration of the injection
	 *
	 * todo: since this method is not invoked within trigger event handler and
	 * engineState.injectionOffset is calculated from the same utility timer should we more that logic here?
	 */
	floatms_t fuelMs = ENGINE(injectionDuration);
	efiAssertVoid(CUSTOM_ERR_ASSERT, !cisnan(fuelMs), "NaN fuelMs");
	angle_t injectionDurationAngle = MS2US(fuelMs) / oneDegreeUs;
	efiAssertVoid(CUSTOM_ERR_ASSERT, !cisnan(injectionDurationAngle), "NaN injectionDuration");
	assertAngleRange(injectionDurationAngle, "injectionDuration_r", CUSTOM_INJ_DURATION);

	floatus_t injectionOffset = ENGINE(engineState.injectionOffset);
	if (cisnan(injectionOffset)) {
		// injection offset map not ready - we are not ready to schedule fuel events
		isReady = false;
		return;
	}

	angle_t baseAngle = injectionOffset - injectionDurationAngle;
	efiAssertVoid(CUSTOM_ERR_ASSERT, !cisnan(baseAngle), "NaN baseAngle");
	assertAngleRange(baseAngle, "baseAngle_r", CUSTOM_ERR_6554);

	injection_mode_e mode = engine->getCurrentInjectionMode(PASS_ENGINE_PARAMETER_SIGNATURE);

	for (int cylinderIndex = 0; cylinderIndex < CONFIG(specs.cylindersCount); cylinderIndex++) {
		InjectionEvent *ev = &elements[cylinderIndex];
		ev->ownIndex = cylinderIndex;  // todo: is this assignment needed here? we now initialize in constructor
		bool result = addFuelEventsForCylinder(cylinderIndex, baseAngle, mode PASS_ENGINE_PARAMETER_SUFFIX);

		if (!result) {
			isReady = false;
			return;
		}
	}

	isReady = true;
}

void FuelSchedule::onTriggerTooth(size_t toothIndex, int rpm, efitick_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX) {
	// Wait for schedule to be built - this happens the first time we get RPM
	if (!isReady) {
		return;
	}
	
	for (int i = 0; i < CONFIG(specs.cylindersCount); i++) {
		elements[i].onTriggerTooth(toothIndex, rpm, nowNt);
	}
}

#endif
