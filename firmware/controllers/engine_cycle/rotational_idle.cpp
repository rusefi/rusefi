#include "pch.h"
#include "rotational_idle.h"

RotationalIdle::RotationalIdle(){

}

bool RotationalIdle::shouldEngageRotationalIdle() {
	rotationalIdleEngaged = false;
	rotIdleEngineTooSlow = false;
	rotIdleEngineTooFast = false;
	rotIdleEngineTooLowClt = false;
	rotIdleEngineTooHot = false;
	rotIdleTpsTooHigh = false;

	if (!engineConfiguration->rotationalIdleController.enabled) {
		return false;
	}

	//TODO: we need to use deadbands on this!! (and on the tps/pps)
	if (engineConfiguration->rotationalIdleController.auto_engage_clt_enable) {
		if (Sensor::getOrZero(SensorType::Clt) > engineConfiguration->rotationalIdleController.auto_engage_clt) {
			rotationalIdleEngaged = true;
			return true;
		} else {
			rotIdleEngineTooLowClt = true;
		}
	}

	// TODO: auto_engage sounds too generic, maybe better auto_engage_pps_enable?
	if (engineConfiguration->rotationalIdleController.auto_engage) {
		if (Sensor::getOrZero(SensorType::DriverThrottleIntent) <= engineConfiguration->rotationalIdleController.max_tps) {
			rotationalIdleEngaged = true;
			return true; // tps is below the maximum
		} else {
			rotIdleTpsTooHigh = true;
		}
	}

	return false;
}

static bool calculateSkip(uint32_t counter, uint8_t max, uint8_t adder) {
	if (max == 0) {
		return false;
	}
	return ((counter % max) + adder) >= max;
}

/**
 * Determines whether to skip spark based on rotational idle skip pattern configuration.
 *
 * The skip pattern works using modulo arithmetic with configurable accumulators:
 * - Each accumulator has: acc_max (pattern period), acc_adder (phase shift), acc_offset (cylinder offset)
 * - For each accumulator: counter = globalSparkCounter + offset + acc_offset
 * - Skip condition: ((counter % acc_max) + acc_adder) >= acc_max
 *
 * Example with acc_max=2, acc_adder=1, acc_offset=0:
 *   counter=0: (0 % 2) + 1 = 1 < 2  -> fire spark
 *   counter=1: (1 % 2) + 1 = 2 >= 2 -> skip spark
 *   counter=2: (2 % 2) + 1 = 1 < 2  -> fire spark
 *   counter=3: (3 % 2) + 1 = 2 >= 2 -> skip spark
 *
 * If acc_max=0, that accumulator is disabled and won't skip.
 *
 * @return True if should skip a spark event.
 */
bool RotationalIdle::shouldSkipSparkRotationalIdle() {
	uint32_t counter = sparkPatternCounter++;

	auto cutMode = engineConfiguration->rotationalIdleController.cut_mode;

	if (cutMode == RotationalCutMode::Fuel) {
		return false;
	}

	if (!shouldEngageRotationalIdle()) {
		return false;
	}

	bool result = false;

	for (size_t i = 0; i < efi::size(engineConfiguration->rotationalIdleController.accumulators); i++){
		const auto& accConfig = engineConfiguration->rotationalIdleController.accumulators[i];

		result |= calculateSkip(counter + accConfig.acc_offset, accConfig.acc_max, accConfig.acc_adder);
	}

	return result;
}

/**
 * Determines whether to skip fuel injection based on rotational idle cut configuration.
 * Uses the same accumulator pattern as spark skip, but driven by an independent fuel event counter.
 * The fuel counter always increments on every injection event to keep the pattern consistent.
 *
 * @return True if should skip fuel injection for this event.
 */
bool RotationalIdle::shouldSkipFuelRotationalIdle() {
	uint32_t counter = fuelPatternCounter++;

	auto cutMode = engineConfiguration->rotationalIdleController.cut_mode;

	if (cutMode == RotationalCutMode::Spark) {
		return false;
	}

	if (!shouldEngageRotationalIdle()) {
		return false;
	}

	bool result = false;

	for (size_t i = 0; i < efi::size(engineConfiguration->rotationalIdleController.accumulators); i++) {
		const auto& accConfig = engineConfiguration->rotationalIdleController.accumulators[i];

		result |= calculateSkip(counter + accConfig.acc_offset, accConfig.acc_max, accConfig.acc_adder);
	}

	return result;
}