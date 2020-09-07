/**
 * @file        debounce.h
 * @brief       Generic button debounce class
 *
 * @date Aug 31, 2020
 * @author David Holdeman, (c) 2020
 */
#pragma once

#include "globalaccess.h"
#include "io_pins.h"

class ButtonDebounce;

class PointerListNode {
public:
    ButtonDebounce *pointer = nullptr;
    PointerListNode *next = nullptr;
};

class ButtonDebounce {
public:
    void init(int t, DECLARE_CONFIG_POINTERS(brain_pin_e, p), DECLARE_CONFIG_POINTERS(pin_input_mode_e, m));
    void updateConfiguration();
    bool readPinEvent();
    PointerListNode thisNode;
private:
    int threshold;
    efitick_t timeLast;
    brain_pin_e *pin = nullptr;
    brain_pin_e *active_pin = nullptr;
    brain_pin_e oldPin;
    pin_input_mode_e *mode = nullptr;
    pin_input_mode_e *active_mode = nullptr;
    bool readValue = false;
    bool initialized = false;
};
