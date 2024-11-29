//
// Created by kifir on 11/25/24.
//

#include "pch.h"

#if EFI_LAUNCH_CONTROL
#include "nitrous_controller.h"

void NitrousController::update() {
    if (engineConfiguration->nitrousControlEnabled) {
        updateArmingState();
        updateTpsConditionSatisfied();
        updateCltConditionSatisfied();
        updateMapConditionSatisfied();
        updateAfrConditionSatisfied();
    }
}

void NitrousController::updateArmingState() {
    switch (engineConfiguration->nitrousControlArmingMethod) {
        case DIGITAL_SWITCH_INPUT: {
            isArmed = checkTriggerPinState();
            break;
        }
        case LUA_GAUGE: {
            isArmed = checkLuaGauge();
            break;
        }
        default: { // Unexpected value!!!
            isArmed = false;
            break;
        }
    }
}

void NitrousController::updateTpsConditionSatisfied() {
    const expected<float> tps = Sensor::get(SensorType::DriverThrottleIntent);
    if (engineConfiguration->nitrousMinimumTps != 0) {
        isTpsConditionSatisfied = tps.Valid && (engineConfiguration->nitrousMinimumTps <= tps.Value);
    } else {
        isTpsConditionSatisfied = true;
    }
}

void NitrousController::updateCltConditionSatisfied() {
    const expected<float> clt = Sensor::get(SensorType::Clt);
    if (engineConfiguration->nitrousMinimumClt != 0) {
        isCltConditionSatisfied = clt.Valid && (engineConfiguration->nitrousMinimumClt <= clt.Value);
    } else {
        isCltConditionSatisfied = true;
    }
}

void NitrousController::updateMapConditionSatisfied() {
    const expected<float> map = Sensor::get(SensorType::Map);
    if (engineConfiguration->nitrousMaximumMap != 0) {
        isMapConditionSatisfied = map.Valid && (map.Value <= engineConfiguration->nitrousMaximumMap);
    } else {
        isMapConditionSatisfied = true;
    }
}

void NitrousController::updateAfrConditionSatisfied() {
    if (static_cast<float>(engineConfiguration->nitrousMaximumAfr) != 0.0f) {
        const expected<float> lambda1 = Sensor::get(SensorType::Lambda1);
        if (lambda1.Valid) {
            const float afr = lambda1.Value * STOICH_RATIO;
            isAfrConditionSatisfied = (afr <= static_cast<float>(engineConfiguration->nitrousMaximumAfr));
        } else {
            isAfrConditionSatisfied = false;
        }
    } else {
        isAfrConditionSatisfied = true;
    }
}

bool NitrousController::checkTriggerPinState() const {
    bool result = false;
#if !EFI_SIMULATOR
    const switch_input_pin_e triggerPin = engineConfiguration->nitrousControlTriggerPin;
    if (isBrainPinValid(triggerPin)) {
        result = engineConfiguration->nitrousControlTriggerPinInverted ^ efiReadPin(triggerPin);
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