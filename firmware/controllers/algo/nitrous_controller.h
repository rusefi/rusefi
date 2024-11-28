//
// Created by kifir on 11/25/24.
//

#pragma once

#include "nitrous_control_state_generated.h"

class NitrousController : public nitrous_control_state_s {
public:
    void update();
private:
    void updateArmingState();
    void updateTpsConditionSatisfied();
    void updateCltConditionSatisfied();
    void updateMapConditionSatisfied();

    bool checkTriggerPinState() const;
    bool checkLuaGauge() const;

    SensorType getLuaGauge() const;
};
