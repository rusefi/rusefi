#include "debounce.h"

ButtonDebounce::ButtonDebounce (int t, brain_pin_e p, pin_input_mode_e m) {
    threshold = t;
    timeLast = 0;
    pin = p;
#ifdef PAL_MODE_INPUT_PULLDOWN
    mode = (m == PI_PULLUP) ? PAL_MODE_INPUT_PULLUP : PAL_MODE_INPUT_PULLDOWN;
//    efiSetPadMode("Button", p, mode);
efiSetPadMode("Button", p, PAL_HIGH);
#endif
}

bool ButtonDebounce::readPinEvent() {
    efitimems_t timeNow = currentTimeMillis();
    if ((timeNow - timeLast) < threshold) {
        return false;
    }
    timeLast = timeNow;
    readValue = false;
 //   readValue = efiReadPin(pin);
#ifdef PAL_MODE_INPUT_PULLDOWN
    if (mode != PAL_MODE_INPUT_PULLDOWN) {
        return !readValue;
    }
#endif
    return readValue;
}
