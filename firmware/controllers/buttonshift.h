#include "tcu.h"
#include "debounce.h"
#include "globalaccess.h"

class ButtonShiftController: public GearControllerBase {
public:
    void update();
    ButtonShiftController (DECLARE_CONFIG_PARAMETER_SIGNATURE);
private:
    ButtonDebounce debounceUp;
    ButtonDebounce debounceDown;
};
