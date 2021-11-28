#pragma once

#include "tcu.h"

class SimpleTransmissionController: public TransmissionControllerBase  {
public:
    void update(gear_e);
    void init();
};
