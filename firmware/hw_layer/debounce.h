/**
 * @file        debounce.h
 * @brief       Generic button debounce class
 *              https://en.wikipedia.org/wiki/Switch#Contact_bounce
 *              If we don't 'debounce' our button inputs, we may mistakenly
 *              read a single button press as multiple events.
 *
 * @date Aug 31, 2020
 * @author David Holdeman, (c) 2020
 */
#pragma once

#include "globalaccess.h"
#include "io_pins.h"

class ButtonDebounce {
public:
	explicit ButtonDebounce(const char* name);
    void init(efitimems_t threshold, brain_pin_e &pin, pin_input_mode_e &mode);
    void stopConfiguration();
    void startConfiguration();
    bool readPinEvent();
    bool readPinState();
    static void stopConfigurationList();
    static void startConfigurationList();
    static void debug();
private:
    const char* const m_name;
    efitick_t m_threshold;
    efitick_t timeLast;
    brain_pin_e *m_pin;
    brain_pin_e active_pin = GPIO_UNASSIGNED;
    pin_input_mode_e *m_mode;
    pin_input_mode_e active_mode = PI_DEFAULT;
    bool storedValue = false;
    bool isInstanceRegisteredInGlobalList = false;
    bool needsPinInitialization = true;
    ButtonDebounce *nextDebounce = nullptr;
    static ButtonDebounce* s_firstDebounce;
};

void initButtonDebounce();
