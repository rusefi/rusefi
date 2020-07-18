include "debounce.h"

ButtonDebounce::ButtonDebounce (int t) {
    threshold = t;
    time = 0;
}

bool ButtonDebounce::readPin(brain_pin_e pin) {
    efitimems_t timeNow = currentTimeMilllis();
    if ((timeNow - time) < threshold) {
        return false;
    }
    time = timeNow;
    return efiReadPin(pin);
}
