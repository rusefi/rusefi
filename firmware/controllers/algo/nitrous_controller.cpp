//
// Created by kifir on 11/25/24.
//

#include "pch.h"

#if EFI_LAUNCH_CONTROL
#include "nitrous_controller.h"

void NitrousController::onSlowCallback() {
    if (engineConfiguration->nitrousControlEnabled) {
        updateArmingState();
        updateSpeedConditionSatisfied();
        updateTpsConditionSatisfied();
        updateCltConditionSatisfied();
        updateMapConditionSatisfied();
        updateAfrConditionSatisfied();
        updateRpmConditionSatisfied();
        isNitrousCondition = (
            isNitrousArmed && isNitrousSpeedCondition && isNitrousTpsCondition && isNitrousCltCondition
                && isNitrousMapCondition && isNitrousAfrCondition && isNitrousRpmCondition
        );
    } else {
        isNitrousCondition = false;
    }
    enginePins.nitrousRelay.setValue(isNitrousCondition);
}

float NitrousController::getFuelCoefficient() const {
    float result = 1.0f;
    if (engineConfiguration->nitrousControlEnabled && isNitrousCondition) {
        result += engineConfiguration->nitrousFuelAdderPercent / 100.0f;
    }
    return result;
}

void NitrousController::updateArmingState() {
    switch (engineConfiguration->nitrousControlArmingMethod) {
        case DIGITAL_SWITCH_INPUT: {
            isNitrousArmed = checkTriggerPinState();
            break;
        }
        case LUA_GAUGE: {
            isNitrousArmed = checkLuaGauge();
            break;
        }
        default: { // Unexpected value!!!
            isNitrousArmed = false;
            break;
        }
    }
}

void NitrousController::updateSpeedConditionSatisfied() {
    if (engineConfiguration->nitrousMinimumVehicleSpeed != 0) {
        const expected<float> speed = Sensor::get(SensorType::VehicleSpeed);
        isNitrousSpeedCondition = speed.Valid && (engineConfiguration->nitrousMinimumVehicleSpeed <= speed.Value);
    } else {
        isNitrousSpeedCondition = true;
    }
}

void NitrousController::updateTpsConditionSatisfied() {
    if (engineConfiguration->nitrousMinimumTps != 0) {
        const expected<float> tps = Sensor::get(SensorType::DriverThrottleIntent);
        isNitrousTpsCondition = tps.Valid && (engineConfiguration->nitrousMinimumTps <= tps.Value);
    } else {
        isNitrousTpsCondition = true;
    }
}

void NitrousController::updateCltConditionSatisfied() {
    if (engineConfiguration->nitrousMinimumClt != 0) {
        const expected<float> clt = Sensor::get(SensorType::Clt);
        isNitrousCltCondition = clt.Valid && (engineConfiguration->nitrousMinimumClt <= clt.Value);
    } else {
        isNitrousCltCondition = true;
    }
}

void NitrousController::updateMapConditionSatisfied() {
    if (engineConfiguration->nitrousMaximumMap != 0) {
        const expected<float> map = Sensor::get(SensorType::Map);
        isNitrousMapCondition = map.Valid && (map.Value <= engineConfiguration->nitrousMaximumMap);
    } else {
        isNitrousMapCondition = true;
    }
}

void NitrousController::updateAfrConditionSatisfied() {
    if (static_cast<float>(engineConfiguration->nitrousMaximumAfr) != 0.0f) {
        const expected<float> lambda1 = Sensor::get(SensorType::Lambda1);
        if (lambda1.Valid) {
            const float afr = lambda1.Value * STOICH_RATIO;
            isNitrousAfrCondition = (afr <= static_cast<float>(engineConfiguration->nitrousMaximumAfr));
        } else {
            isNitrousAfrCondition = false;
        }
    } else {
        isNitrousAfrCondition = true;
    }
}

namespace {
    MaxLimitWithHysteresis<UnstrictChecker> rpmHysteresis;
}

void NitrousController::updateRpmConditionSatisfied() {
    const expected<float> rpmSensorReading = Sensor::get(SensorType::Rpm);
    if (rpmSensorReading.Valid) {
        const float rpm = rpmSensorReading.Value;
        if (rpmHysteresis.checkIfLimitIsExceeded(
                rpm,
                engineConfiguration->nitrousDeactivationRpm,
                engineConfiguration->nitrousDeactivationRpmWindow
        )) {
            isNitrousRpmCondition = false;
        } else {
            isNitrousRpmCondition = (engineConfiguration->nitrousActivationRpm <= rpm);
        }
    } else {
        isNitrousRpmCondition = false;
    }
}

bool NitrousController::checkTriggerPinState() const {
    bool result = false;
#if !EFI_SIMULATOR
    const switch_input_pin_e triggerPin = engineConfiguration->nitrousControlTriggerPin;
    const pin_input_mode_e triggerPinMode = engineConfiguration->nitrousControlTriggerPinMode;
    if (isBrainPinValid(triggerPin)) {
        result = efiReadPin(triggerPin, triggerPinMode);
    }
#endif // !EFI_SIMULATOR
    return result;
}

bool NitrousController::checkLuaGauge() const {
    bool result = false;
    const SensorResult currentSensorResult = Sensor::get(getLuaGauge());
    if (currentSensorResult.Valid) {
        const float currentValue = currentSensorResult.Value;
        const float armingValue = engineConfiguration->nitrousLuaGaugeArmingValue;
        switch (engineConfiguration->nitrousLuaGaugeMeaning) {
            case LUA_GAUGE_LOWER_BOUND: {
                result = (armingValue <= currentValue);
                break;
            }
            case LUA_GAUGE_UPPER_BOUND: {
                result = (currentValue <= armingValue);
                break;
            }
        }
    }
    return result;
}

SensorType NitrousController::getLuaGauge() const {
    SensorType result = SensorType::LuaGauge1;
    switch (engineConfiguration->nitrousLuaGauge) {
        case LUA_GAUGE_1: {
            break;
        }
        case LUA_GAUGE_2: {
            result = SensorType::LuaGauge2;
            break;
        }
        case LUA_GAUGE_3: {
            result = SensorType::LuaGauge3;
            break;
        }
        case LUA_GAUGE_4: {
            result = SensorType::LuaGauge4;
            break;
        }
        case LUA_GAUGE_5: {
            result = SensorType::LuaGauge5;
            break;
        }
        case LUA_GAUGE_6: {
            result = SensorType::LuaGauge6;
            break;
        }
        case LUA_GAUGE_7: {
            result = SensorType::LuaGauge7;
            break;
        }
        case LUA_GAUGE_8: {
            result = SensorType::LuaGauge8;
            break;
        }
    }
    return result;
}

#endif // EFI_LAUNCH_CONTROL