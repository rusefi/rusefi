#pragma once

#include "gear_controller.h"

#if EFI_TCU
class AutomaticGearController: public GearControllerBase {
public:
  AutomaticGearController();

	void update();
	void findDesiredGear();
	GearControllerMode getMode() const {
		return GearControllerMode::Automatic;
	}
private:
	void shift(float speed, float throttle, uint8_t (*curve)[TCU_TABLE_WIDTH], gear_e gear);
	void shift(float speed, float throttle, uint8_t (*curve)[TCU_TABLE_WIDTH], gear_e gear, bool down);
};

AutomaticGearController* getAutomaticGearController();
#endif // EFI_TCU
