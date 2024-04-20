#pragma once

#include "gear_controller.h"
#include "gc_auto.h"

#if EFI_TCU
class GenericGearController: public AutomaticGearController {
public:
  GenericGearController();

	void update();
  void init();
	GearControllerMode getMode() const {
		return GearControllerMode::Generic;
	}
private:
	Timer shiftTimer;
	SelectedGear lastRange;
	bool isNearest(float value, int pinIndex, float* rangeStates);
	SensorType getAnalogSensorType(int zeroBasedSensorIndex);
};

GenericGearController* getGenericGearController();
#endif // EFI_TCU
