/**
 * @file	framework.cpp
 *
 * @date Sep 25, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

efitick_t getTimeNowNt(void) {
	return getTimeNowUs() * US_TO_NT_MULTIPLIER;
}

uint32_t getTimeNowLowerNt(void) {
	return getTimeNowNt();
}

efitimeus_t getTimeNowUs(void) {
	return chVTGetSystemTimeX() * (1000000 / CH_CFG_ST_FREQUENCY);
}
