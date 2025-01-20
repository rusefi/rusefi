#pragma once

#include "ignition_state_generated.h"

class IgnitionState : public ignition_state_s {
public:
	void updateDwell(float rpm, bool isCranking);

  floatms_t getDwell() const;
  angle_t getWrappedAdvance(const float rpm, const float engineLoad);
private:
  angle_t getAdvance(float rpm, float engineLoad);
	floatms_t getSparkDwell(float rpm, bool isCranking);
};
