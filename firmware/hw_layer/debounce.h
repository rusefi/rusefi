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
    void init(int t, engine_configuration_s p, engine_configuration_s m);
    bool readPinEvent();
private:
    int threshold;
    efitick_t timeLast;
    brain_pin_e pin;
    iomode_t mode;
    bool readValue;
};
