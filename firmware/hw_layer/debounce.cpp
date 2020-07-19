#include "debounce.h"

ButtonDebounce::ButtonDebounce (int t, brain_pin_e p, iomode_t m) {
    threshold = t;
    time = 0;
    pin = p;
    mode = m;
    efiSetPadMode(m);
}

bool ButtonDebounce::readPin(brain_pin_e pin) {
    efitimems_t timeNow = currentTimeMilllis();
    if ((timeNow - time) < threshold) {
        if (mode == PAL_MODE_INPUT_PULLDOWN) {
            return false;
        } else {
            return true;
        }
    }
    time = timeNow;
    return efiReadPin(pin);
}
