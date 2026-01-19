#include "pch.h"
#include "rotational_idle.h"

RotationalIdle::RotationalIdle(){

}

bool RotationalIdle::shouldEngageRotationalIdle() {
	rotationalIdleEngaged = false;

	if (!engineConfiguration->rotationalIdleController.enabled) {
		return false;
	}

	//TODO: we need to use deadbands ond this!! (and on the tps/pps)
	if (engineConfiguration->rotationalIdleController.auto_engage_clt_enable && Sensor::getOrZero(SensorType::Clt) > engineConfiguration->rotationalIdleController.auto_engage_clt) {
		rotationalIdleEngaged = true;
		return true;
	}

	// TODO: auto_engage sounds too generic, maybe better auto_engage_pps_enable?
	if (engineConfiguration->rotationalIdleController.auto_engage && Sensor::getOrZero(SensorType::DriverThrottleIntent) <= engineConfiguration->rotationalIdleController.max_tps) {
		rotationalIdleEngaged = true;
		return true; // tps is below the maximum
	}

	return false;
}

/**
 * @param offset The offset value to adjust the global spark counter
 *               in skip calculations.
 * @return True if rotational idle operations should be skipped, false otherwise.
 */
bool RotationalIdle::shouldSkipSparkRotationalIdle(int8_t offset) {
	bool result = false;

	return result;
}