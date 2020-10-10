#pragma once

#include "tcu.h"

class SimpleTransmissionController: public TransmissionControllerBase {
public:
    DECLARE_ENGINE_PTR;

    void update(gear_e);
    void init();
};
