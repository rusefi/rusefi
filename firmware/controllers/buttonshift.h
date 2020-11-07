/**
 * @file        buttonshift.cpp
 * @brief       Polls pins for gear changes
 *
 * @date Aug 31, 2020
 * @author David Holdeman, (c) 2020
 */
#pragma once

#include "gear_controller.h"
#include "globalaccess.h"
#include "debounce.h"

class ButtonShiftController: public GearControllerBase {
public:
	ButtonShiftController();
    DECLARE_ENGINE_PTR;
    void update();
    void init(DECLARE_ENGINE_PARAMETER_SIGNATURE);
private:
    ButtonDebounce debounceUp;
    ButtonDebounce debounceDown;
};

void initButtonShift(DECLARE_ENGINE_PARAMETER_SIGNATURE);
