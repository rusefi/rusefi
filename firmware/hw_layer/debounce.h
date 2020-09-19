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

class ButtonDebounce {
public:
    void init(efitimems_t t, brain_pin_e *p, pin_input_mode_e *m);
    void updateConfiguration();
    bool readPinEvent();
    bool readPinState();
    static void updateConfigurationList();
private:
    efitick_t threshold;
    efitick_t timeLast;
    brain_pin_e *pin;
    brain_pin_e active_pin;
    pin_input_mode_e *mode;
    pin_input_mode_e active_mode;
    bool readValue;
    bool initialized = false;
    ButtonDebounce *nextDebounce = nullptr;
    static ButtonDebounce* s_firstDebounce;
};

#endif /* DEBOUNCE_INC */
