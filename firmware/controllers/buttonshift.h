#include "tcu.h"
#include "debounce.h"

class ButtonShiftController: public GearControllerBase {
private:
    ButtonDebounce buttonDebounce;
};
