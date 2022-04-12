#pragma once

#include "tcu.h"

class Gm4l6xTransmissionController: public TransmissionControllerBase  {
public:
	void update(gear_e);
	void init();
	TransmissionControllerMode mode = TransmissionControllerMode::Gm4l6x;
private:
	gear_e setCurrentGear(gear_e gear);
	void setTccState();
};

Gm4l6xTransmissionController* getGm4l6xTransmissionController();
