#include "debounce.h"

ButtonDebounce::ButtonDebounce (int t, brain_pin_e p, pin_input_mode_e m) {
    threshold = t;
    timeLast = 0;
    pin = p;
    mode = (m == PI_PULLUP) ? PAL_MODE_INPUT_PULLUP : PAL_MODE_INPUT_PULLDOWN;
//    efiSetPadMode("Button", p, mode);
efiSetPadMode("Button", p, PAL_HIGH);

}

bool ButtonDebounce::readPinEvent() {
    efitimems_t timeNow = currentTimeMillis();
    if ((timeNow - timeLast) < threshold) {
        return false;
    }
    timeLast = timeNow;
    readValue = false;
 //   readValue = efiReadPin(pin);
    if (mode != PAL_MODE_INPUT_PULLDOWN) {
        return !readValue;
    }
    return readValue;
}
