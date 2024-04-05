#pragma once

#include "global.h"
#include "io_pins.h"
#include "persistent_configuration.h"
#include "generated_lookup_engine_configuration.h"
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
	TransmissionControllerBase *transmissionController;
protected:
	virtual gear_e setDesiredGear(gear_e);
	void initTransmissionController();
	float* getRangeStateArray(int);
private:
	gear_e desiredGear = NEUTRAL;
	void postState();
};

void initGearController();
#endif // EFI_TCU
