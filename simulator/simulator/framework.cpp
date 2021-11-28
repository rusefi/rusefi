/**
 * @file	framework.cpp
 *
 * @date Sep 25, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

// Since all the time logic in the firmware is centered around this function, we only provide this
// function in the firmware.  It forces us to exercise the functions that build on this one.
uint32_t getTimeNowLowerNt(void) {
	return US2NT(chVTGetSystemTimeX() * (1000000 / CH_CFG_ST_FREQUENCY));
}
