//
// Created by kifir on 9/27/24.
//

#pragma once

#include "shift_torque_reduction_state_generated.h"

class ShiftTorqueReductionController : public shift_torque_reduction_state_s {
public:
	void update();
private:
	void updateTriggerPinState();
	void updateTriggerPinState(switch_input_pin_e pin, bool isPinInverted);

	void updateTimeConditionSatisfied();
	void updateRpmConditionSatisfied();

	Timer m_pinTriggeredTimer;
};
