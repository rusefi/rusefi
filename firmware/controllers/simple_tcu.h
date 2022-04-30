#pragma once

#define SIMPLE_TRANSMISSION_CONTROLLER

#include "tcu.h"

#if EFI_TCU
class SimpleTransmissionController: public TransmissionControllerBase  {
public:
  void update(gear_e);
	void init();
	TransmissionControllerMode getMode() const {
		return TransmissionControllerMode::SimpleTransmissionController;
	}
};

SimpleTransmissionController* getSimpleTransmissionController();
#endif // EFI_TCU
