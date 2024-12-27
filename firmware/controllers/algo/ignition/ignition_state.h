#pragma once

#include "ignition_state_generated.h"

class IgnitionState : public ignition_state_s {
public:
	floatms_t getSparkDwell(float rpm);
};
