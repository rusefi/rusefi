/*
 * @file VvlController.h
 *
 * @date: ago 25, 2025
 * @author FDSoftware
 */

#pragma once

#include "vvl_controller_state_generated.h"

class VvlController : public vvl_controller_state_s, public EngineModule {
public:
    using interface_t = VvlController;

    void onSlowCallback() override;
	float getFuelCoefficient() const;
	float getTimingModifier() const;

private:
    void updateTpsConditionSatisfied();
    void updateCltConditionSatisfied();
    void updateMapConditionSatisfied();
    void updateAfrConditionSatisfied();
    void updateRpmConditionSatisfied();
};
