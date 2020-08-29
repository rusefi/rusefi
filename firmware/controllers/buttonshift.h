#pragma once

#include "tcu.h"
#include "globalaccess.h"
#include "debounce.h"

class ButtonShiftController: public GearControllerBase {
public:
    void update();
    void init(DECLARE_ENGINE_PARAMETER_SIGNATURE);
private:
    ButtonDebounce *debounceUp;
    ButtonDebounce *debounceDown;
};

void initButtonShift(DECLARE_ENGINE_PARAMETER_SIGNATURE);
