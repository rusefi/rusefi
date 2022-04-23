#pragma once

#include "tcu.h"

#if EFI_TCU
class Gm4l6xTransmissionController: public TransmissionControllerBase  {
public:
	void update(gear_e);
	void init();
	TransmissionControllerMode getMode() const {
		return TransmissionControllerMode::Gm4l6x;
	}
private:
	gear_e setCurrentGear(gear_e gear);
	void setTccState();
	void setPcState(gear_e gear);
	void set32State(gear_e gear);
	bool isShifting = false;
	gear_e shiftingFrom;
};

Gm4l6xTransmissionController* getGm4l6xTransmissionController();
#endif // EFI_TCU
