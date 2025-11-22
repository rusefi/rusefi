//
// Created by kifir on 9/27/24.
//

#pragma once

#include "shift_torque_reduction_state_generated.h"

class ShiftTorqueReductionController : public shift_torque_reduction_state_s {
public:
	void update();

	float getSparkSkipRatio();

	float getTorqueReductionIgnitionRetard();

private:
	void updateTriggerPinState();
	void updateTriggerPinState(switch_input_pin_e pin, pin_input_mode_e mode, const bool invertPhysicalPin, bool invalidPinState);

	void updateTimeConditionSatisfied();
	void updateRpmConditionSatisfied();
	void updateAppConditionSatisfied();

	Timer m_pinTriggeredTimer;
};
