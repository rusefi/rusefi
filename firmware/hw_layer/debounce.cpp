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

PointerListNode buttonDebounceListHead;

void ButtonDebounce::init (int t, DECLARE_CONFIG_POINTERS(brain_pin_e, p), DECLARE_CONFIG_POINTERS(pin_input_mode_e, m)) {
    if (!initialized) {
        thisNode.pointer = this;
        PointerListNode *listItem = &buttonDebounceListHead;
        if (listItem->pointer == NULL) {
            buttonDebounceListHead = thisNode;
        } else {
            while (listItem->next != NULL) {
                listItem = listItem->next;
            }
            listItem->next = &thisNode;
        }
    }
    threshold = MS2NT(t);
    timeLast = 0;
    pin = p;
    active_pin = active_p;
    mode = m;
    active_mode = active_m;
#ifdef PAL_MODE_INPUT_PULLDOWN
    // getInputMode converts from pin_input_mode_e to iomode_t
    efiSetPadMode("Button", *p, getInputMode(*m));
#endif
    initialized = true;
}

void ButtonDebounce::updateConfiguration () {
    if (isConfigurationChangedPointers(pin, active_pin) || isConfigurationChangedPointers(mode, active_mode)) {
        brain_pin_markUnused(*active_pin);
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
    readValue = efiReadPin(*active_pin);
    // Invert
    if (getInputMode(*active_mode) == PAL_MODE_INPUT_PULLUP) {
        readValue = !readValue;
    }
#endif
    if (readValue) {
        timeLast = timeNow;
    }
    return readValue;
}
