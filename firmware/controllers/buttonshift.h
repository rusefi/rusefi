#include "tcu.h"
#include "debounce.h"
#include "globalaccess.h"

EXTERN_ENGINE_CONFIGURATION;

class ButtonShiftController: public GearControllerBase {
public:
    void update();
private:
    ButtonDebounce debounceUp = ButtonDebounce(10, CONFIG(tcuUpshiftButtonPin), CONFIG(tcuUpshiftButtonPinMode));
    ButtonDebounce debounceDown = ButtonDebounce(10, CONFIG(tcuDownshiftButtonPin), CONFIG(tcuDownshiftButtonPinMode));
};
