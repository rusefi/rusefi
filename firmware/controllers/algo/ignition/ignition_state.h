#pragma once

#include "ignition_state_generated.h"

class IgnitionState : public ignition_state_s {
public:
	void updateDwell(float rpm, bool isCranking);
	void updateAdvanceCorrections(float engineLoad);

  floatms_t getDwell() const;
  angle_t getWrappedAdvance(const float rpm, const float engineLoad);
  angle_t getTrailingSparkAngle(const float rpm, const float engineLoad);
	angle_t getSparkHardwareLatencyCorrection();

	static angle_t getInterpolatedIgnitionAngle(float rpm, float ignitionLoad);
	static angle_t getInterpolatedIgnitionTrim(size_t cylinderNumber, float rpm, float ignitionLoad);

private:
  angle_t getAdvance(float rpm, float engineLoad);
	floatms_t getSparkDwell(float rpm, bool isCranking);
};
