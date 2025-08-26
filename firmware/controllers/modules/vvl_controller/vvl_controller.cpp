/*
 * @file VvlController.cpp
 *
 * @date: ago 25, 2025
 * @author FDSoftware
 */

#include "pch.h"
#include "vvl_controller.h"

void VvlController::onSlowCallback() {
  	if (engineConfiguration->vvlControlEnabled) {
		updateTpsConditionSatisfied();
        updateCltConditionSatisfied();
        updateMapConditionSatisfied();
        updateAfrConditionSatisfied();
        updateRpmConditionSatisfied();
        isVvlCondition = (
			isVvlTpsCondition && isVvlCltCondition && isVvlMapCondition && isVvlAfrCondition && isVvlRpmCondition
        );
    } else {
        isVvlCondition = false;
    }
	enginePins.vvlRelay.setValue(isVvlCondition);
}

float VvlController::getFuelCoefficient() const {
    float result = 1.0f;
    if (engineConfiguration->vvlControlEnabled && isVvlCondition) {
        result += engineConfiguration->vvlController.fuelAdderPercent / 100.0f;
    }
    return result;
}

float VvlController::getTimingModifier() const {
    if (engineConfiguration->vvlControlEnabled && isVvlCondition) {
		// note the "-" here to remove timing!
        return -engineConfiguration->vvlController.ignitionRetard;
    }
    return 0.0f;
}

void VvlController::updateTpsConditionSatisfied() {
    if (engineConfiguration->vvlController.minimumTps != 0) {
        const expected<float> tps = Sensor::get(SensorType::DriverThrottleIntent);
        isVvlTpsCondition = tps.Valid && (engineConfiguration->vvlController.minimumTps <= tps.Value);
    } else {
        isVvlTpsCondition = true;
    }
}

void VvlController::updateCltConditionSatisfied() {
    if (engineConfiguration->vvlController.minimumClt != 0) {
        const expected<float> clt = Sensor::get(SensorType::Clt);
        isVvlCltCondition = clt.Valid && (engineConfiguration->vvlController.minimumClt <= clt.Value);
    } else {
        isVvlCltCondition = true;
    }
}

void VvlController::updateMapConditionSatisfied() {
    if (engineConfiguration->vvlController.maximumMap != 0) {
        const expected<float> map = Sensor::get(SensorType::Map);
        isVvlMapCondition = map.Valid && (map.Value <= engineConfiguration->vvlController.maximumMap);
    } else {
        isVvlMapCondition = true;
    }
}

void VvlController::updateAfrConditionSatisfied() {
    if (static_cast<float>(engineConfiguration->vvlController.maximumAfr) != 0.0f) {
        const expected<float> lambda1 = Sensor::get(SensorType::Lambda1);
        if (lambda1.Valid) {
            const float afr = lambda1.Value * STOICH_RATIO;
            isVvlAfrCondition = (afr <= static_cast<float>(engineConfiguration->vvlController.maximumAfr));
        } else {
            isVvlAfrCondition = false;
        }
    } else {
        isVvlAfrCondition = true;
    }
}

namespace {
    MaxLimitWithHysteresis<UnstrictChecker> rpmHysteresis;
}

void VvlController::updateRpmConditionSatisfied() {
    const expected<float> rpmSensorReading = Sensor::get(SensorType::Rpm);
    if (rpmSensorReading.Valid) {
        const float rpm = rpmSensorReading.Value;
        if (rpmHysteresis.checkIfLimitIsExceeded(
                rpm,
                engineConfiguration->vvlController.deactivationRpm,
                engineConfiguration->vvlController.deactivationRpmWindow
        )) {
            isVvlRpmCondition = false;
        } else {
            isVvlRpmCondition = (engineConfiguration->vvlController.activationRpm <= rpm);
        }
    } else {
        isVvlRpmCondition = false;
    }
}