#pragma once

#include "global.h"
#include "io_pins.h"
#include "persistent_configuration.h"
#include "engine_configuration_generated_structures.h"
#include "simple_tcu.h"
#include "tc_4l6x.h"

#if EFI_TCU
class GearControllerBase {
public:
	virtual void update();
	virtual gear_e getDesiredGear() const;
	virtual void init();
	virtual GearControllerMode getMode() const {
		return GearControllerMode::ButtonShift;
	}
protected:
	virtual gear_e setDesiredGear(gear_e);
	void initTransmissionController();
	TransmissionControllerBase *transmissionController;
private:
	gear_e desiredGear = NEUTRAL;
	void postState();
};

void initGearController();
#endif // EFI_TCU
