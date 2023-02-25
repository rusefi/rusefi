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
};

GenericGearController* getGenericGearController();
#endif // EFI_TCU
