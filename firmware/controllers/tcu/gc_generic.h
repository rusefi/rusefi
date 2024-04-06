#pragma once

#include "gear_controller.h"

#if EFI_TCU
class GenericGearController: public GearControllerBase {
public:
  GenericGearController();

	void update();
  void init();
	GearControllerMode getMode() const {
		return GearControllerMode::Generic;
	}
private:
	bool isNearest(float value, int pinIndex, float* rangeStates);
	SensorType getAnalogSensorType(int zeroBasedSensorIndex);
};

GenericGearController* getGenericGearController();
#endif // EFI_TCU
