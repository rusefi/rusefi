#include "debounce.h"

ButtonDebounce::ButtonDebounce (int t, brain_pin_e p, iomode_t m) {
    threshold = t;
    timeLast = 0;
    pin = p;
    mode = m;
    efiSetPadMode(m);
}

bool ButtonDebounce::readPin(brain_pin_e pin) {
    efitimems_t timeNow = currentTimeMilllis();
    if ((timeNow - timeLast) < threshold) {
        return false;
    }
    timeLast = timeNow;
    bool read = efiReadPin(pin);
    if (mode == PAL_MODE_INPUT_PULLDOWN) {
        return read;
    } else {
        return !read;
    }
}
