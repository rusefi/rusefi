/**
 * @file        debounce.cpp
 * @brief       Generic button debounce class
 *
 * @date Aug 31, 2020
 * @author David Holdeman, (c) 2020
 */
#include "debounce.h"
#include "pin_repository.h"
#include "engine_configuration.h"
#include "hardware.h"

void ButtonDebounce::init (int t, brain_pin_e p, pin_input_mode_e m) {
    if (!initialized) {
        ButtonDebounce *listItem = s_firstDebounce;
        if (listItem == nullptr) {
            s_firstDebounce = this;
        } else {
            while (listItem->nextDebounce != nullptr) {
                listItem = listItem->nextDebounce;
            }
            listItem->nextDebounce = this;
        }
    }
    threshold = MS2NT(t);
    timeLast = 0;
    pin = p;
    active_pin = *p;
    mode = m;
    active_mode = *m;
#ifdef PAL_MODE_INPUT_PULLDOWN
    // getInputMode converts from pin_input_mode_e to iomode_t
    efiSetPadMode("Button", active_pin, getInputMode(active_mode));
#endif
    initialized = true;
}

void ButtonDebounce::updateConfigurationList () {
    ButtonDebounce *listItem = s_firstDebounce;
    while (listItem != nullptr) {
        listItem->updateConfiguration();
        if (listItem->nextDebounce != nullptr) {
            listItem = listItem->nextDebounce;
        } else {
            break;
        }
    }
}

void ButtonDebounce::updateConfiguration () {
#ifndef EFI_ACTIVE_CONFIGURATION_IN_FLASH
    if (*pin != active_pin || *mode != active_mode) {
#else
    if (*pin != active_pin || *mode != active_mode || (isActiveConfigurationVoid && (*pin != 0 || *mode != 0))) {
#endif /* EFI_ACTIVE_CONFIGURATION_IN_FLASH */
        brain_pin_markUnused(active_pin);
        efiSetPadMode("Button", *pin, getInputMode(*mode));
    }
}

/**
@returns true if the button is pressed, and will not return true again within the set timeout
*/
bool ButtonDebounce::readPinEvent() {
    if (!pin) {
        return false;
    }
    efitick_t timeNow = getTimeNowNt();
    // If it's been less than the threshold since we were last called
    if ((timeNow - timeLast) < threshold) {
        return false;
    }
    // readValue is a class variable, so it needs to be reset.
    // We don't actually need it to be a class variable in this method,
    //  but when a method is implemented to actually get the pin's state,
    //  for example to implement long button presses, it will be needed.
    readValue = false;
#ifdef PAL_MODE_INPUT_PULLDOWN
    readValue = efiReadPin(oldPin);
    // Invert
    if (getInputMode(active_mode) == PAL_MODE_INPUT_PULLUP) {
        readValue = !readValue;
    }
#endif
    if (readValue) {
        timeLast = timeNow;
    }
    return readValue;
}
