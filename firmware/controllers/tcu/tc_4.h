#pragma once

#include "tcu.h"

#if EFI_TCU
class Generic4TransmissionController: public SimpleTransmissionController {
public:
	void update(gear_e);
	void init();
	TransmissionControllerMode getMode() const {
		return TransmissionControllerMode::Generic4;
	}
private:
	void setTccState(gear_e gear);
	void setPcState(gear_e gear);
	bool isShifting = false;
	gear_e shiftingFrom;
};

Generic4TransmissionController* getGeneric4TransmissionController();
#endif // EFI_TCU
