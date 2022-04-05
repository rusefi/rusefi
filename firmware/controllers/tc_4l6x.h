#pragma once

#include "tcu.h"

class Gm4l6xTransmissionController: public TransmissionControllerBase  {
public:
    void update(gear_e);
    void init();
};

Gm4l6xTransmissionController* getGm4l6xTransmissionController();
