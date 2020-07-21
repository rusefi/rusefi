#include "debounce.h"

ButtonDebounce::ButtonDebounce (int t, brain_pin_e p, iomode_t m) {
    threshold = t;
    timeLast = 0;
    pin = p;
    mode = m;
    efiSetPadMode(m);
}

bool ButtonDebounce::readPin() {
    if (!checkThreshold()) {
        return readValue;
    }
    timeLast = timeNow;
    readValue = efiReadPin(pin);
    if (mode == PAL_MODE_INPUT_PULLDOWN) {
        return readValue;
    } else {
        return !readValue;
    }
}

bool ButtonDebounce::readEvent() {
    if (!checkThreshold()) {
        return false;
    }    
    timeLast = timeNow;
    readValue = efiReadPin(pin);
    if (mode == PAL_MODE_INPUT_PULLDOWN) {
        return readValue;
    } else {
        return !readValue;
    }
}

bool ButtonDebounce::checkThreshold() {
    efitimems_t timeNow = currentTimeMilllis();
    if ((timeNow - timeLast) < threshold) {
        return false;
    }
    return true;
}
