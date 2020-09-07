/**
 * @file        debounce.h
 * @brief       Generic button debounce class
 *
 * @date Aug 31, 2020
 * @author David Holdeman, (c) 2020
 */
#include "globalaccess.h"
#include "io_pins.h"

class ButtonDebounce {
public:
    void init(int t, brain_pin_e p, pin_input_mode_e m);
    bool readPinEvent();
private:
    int threshold;
    efitick_t timeLast;
    brain_pin_e pin;
    iomode_t mode;
    bool readValue;
};
