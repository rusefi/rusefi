#pragma once

#include "tcu.h"
#include "globalaccess.h"
#include "debounce.h"

class ButtonShiftController: public GearControllerBase {
public:
    void update();
    ButtonShiftController();
private:
    ButtonDebounce *debounceUp;
    ButtonDebounce *debounceDown;
};

void initButtonShift();
