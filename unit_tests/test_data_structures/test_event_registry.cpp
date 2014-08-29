/*
 * test_event_registry.cpp
 *
 *  Created on: Nov 27, 2013
 *      Author: Andrey Belomutskiy, (c) 2012-2013
 */

#include "event_registry.h"
#include "test_event_registry.h"
#include "main.h"
#include "OutputSignalList.h"

static ActuatorEventList eventList;
static ActuatorEventList result;

int pinDefaultState[IO_PIN_COUNT];

extern int outputSignalCount;

void testEventRegistry(void) {
}
