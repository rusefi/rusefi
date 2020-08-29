#pragma once

#include "tcu.h"
#include "globalaccess.h"
#include "debounce.h"

class ButtonShiftController: public GearControllerBase {
public:
    void update(DECLARE_ENGINE_PARAMETER_SIGNATURE);
    void init();
private:
    ButtonDebounce *debounceUp;
    ButtonDebounce *debounceDown;
};

void initButtonShift();
