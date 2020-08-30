#include "debounce.h"
#include "io_pins.h"

ButtonDebounce::ButtonDebounce (int t, brain_pin_e p, iomode_t m) {
    threshold = t;
    timeLast = 0;
    pin = p;
    mode = m;
#ifdef PI_PULLDOWN
    efiSetPadMode("Button", p, m);
#endif
}

bool ButtonDebounce::readPinEvent() {
    efitimems_t timeNow = currentTimeMillis();
    if ((timeNow - timeLast) < threshold) {
        return false;
    }
    timeLast = timeNow;
    readValue = false;
#ifdef PI_PULLDOWN
    readValue = efiReadPin(pin);
    if (mode != PI_PULLDOWN) {
        return !readValue;
    }
#endif
    return readValue;
}
