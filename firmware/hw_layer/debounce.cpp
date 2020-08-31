#include "debounce.h"

ButtonDebounce::ButtonDebounce (int t, brain_pin_e p, pin_input_mode_e m) {
    threshold = t;
    timeLast = 0;
    pin = p;
//#ifdef PAL_MODE_INPUT_PULLDOWN
    mode = getInputMode(m);
    efiSetPadMode("Button", p, mode);
//#endif
}

bool ButtonDebounce::readPinEvent() {
/*    efitimems_t timeNow = currentTimeMillis();
    if ((timeNow - timeLast) < threshold) {
        return false;
    }*/
    timeLast = timeNow;
    readValue = false;
//#ifdef PAL_MODE_INPUT_PULLDOWN
    readValue = efiReadPin(pin);
    /*if (mode != PAL_MODE_INPUT_PULLDOWN) {
        return !readValue;
    }*/
//#endif
    return readValue;
}
