/**
 * @file        debounce.h
 * @brief       Generic button debounce class
 *
 * @date Aug 31, 2020
 * @author David Holdeman, (c) 2020
 */
#ifndef DEBOUNCE_INC
#define DEBOUNCE_INC

#include "globalaccess.h"
#include "io_pins.h"

class ButtonDebounce;

class PointerListNode {
public:
    ButtonDebounce *pointer = NULL;
    PointerListNode *next = NULL;
};

extern PointerListNode buttonDebounceListHead;

class ButtonDebounce {
public:
    void init(int t, DECLARE_CONFIG_POINTERS(brain_pin_e, p), DECLARE_CONFIG_POINTERS(pin_input_mode_e, m));
    void updateConfiguration();
    bool readPinEvent();
    PointerListNode thisNode;
private:
    int threshold;
    efitick_t timeLast;
    brain_pin_e *pin;
    brain_pin_e *active_pin;
    brain_pin_e oldPin;
    pin_input_mode_e *mode;
    pin_input_mode_e *active_mode;
    bool readValue;
};

#endif /* DEBOUNCE_INC */
