#pragma once

#include "ignition_state_generated.h"

class IgnitionState : public ignition_state_s {
public:
	void updateDwell(float rpm, bool isCranking);
  floatms_t getDwell() const;
private:
	floatms_t getSparkDwell(float rpm, bool isCranking);
};
