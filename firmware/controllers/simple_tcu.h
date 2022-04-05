#pragma once

#define SIMPLE_TRANSMISSION_CONTROLLER

#include "tcu.h"

class SimpleTransmissionController: public TransmissionControllerBase  {
public:
    void update(gear_e);
    void init();
};

SimpleTransmissionController* getSimpleTransmissionController();
