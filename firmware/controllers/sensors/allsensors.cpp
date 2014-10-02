/**
 * @file    allsensors.cpp
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "allsensors.h"

extern Engine engine;

void initSensors(void) {
	initThermistors(&engine);
}
