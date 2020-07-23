#include "debounce.h"
#include "io_pins.h"

ButtonDebounce::ButtonDebounce (int t, brain_pin_e p, iomode_t m) {
    threshold = t;
    timeLast = 0;
    pin = p;
    mode = m;
    efiSetPadMode("Button", p, m);
}

bool ButtonDebounce::readPin() {
    efitimems_t timeNow = currentTimeMillis();
    if ((timeNow - timeLast) < threshold) {
        return readValue;
    }
    timeLast = timeNow;
    readValue = efiReadPin(pin);
#ifdef PAL_MODE_INPUT_PULLDOWN
    if (mode != PAL_MODE_INPUT_PULLDOWN) {
        return !readValue;
    }
#endif
    return readValue;
}

bool ButtonDebounce::readEvent() {
    efitimems_t timeNow = currentTimeMillis();
    if ((timeNow - timeLast) < threshold) {
        return false;
    }
    timeLast = timeNow;
    readValue = efiReadPin(pin);
#ifdef PAL_MODE_INPUT_PULLDOWN
    if (mode != PAL_MODE_INPUT_PULLDOWN) {
        return !readValue;
    }
#endif
    return !readValue;
}
