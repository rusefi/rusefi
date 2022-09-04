/**
 * @file        buttonshift.cpp
 * @brief       Polls pins for gear changes
 *
 * @date Aug 31, 2020
 * @author David Holdeman, (c) 2020
 */
#pragma once

#include "gear_controller.h"
#include "debounce.h"

#if EFI_TCU
class ButtonShiftController: public GearControllerBase {
public:
	ButtonShiftController();

	void update() override;
  void init() override;
	GearControllerMode getMode() const {
		return GearControllerMode::ButtonShift;
	}
private:
	ButtonDebounce debounceUp;
	ButtonDebounce debounceDown;
};

ButtonShiftController* getButtonShiftController();
#endif // EFI_TCU
