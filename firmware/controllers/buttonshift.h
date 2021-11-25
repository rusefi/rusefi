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

    void update() override;
    void init() override;
private:
    ButtonDebounce debounceUp;
    ButtonDebounce debounceDown;
};

void initButtonShift();
