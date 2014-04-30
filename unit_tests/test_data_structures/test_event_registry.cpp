/*
 * test_event_registry.c
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

void initOutputSignal(OutputSignal *signal, io_pin_e ioPin) {
	signal->io_pin = ioPin;
}

extern int outputSignalCount;

void testEventRegistry(void) {
	printf("*************************************** testEventRegistry\r\n");

	printf("resetting\r\n");
	resetEventList(&eventList);
	OutputSignalList list;

	printf("registering 0\r\n");

	registerActuatorEvent(&eventList, 0, list.add((io_pin_e)10), 0);
	registerActuatorEvent(&eventList, 0, list.add((io_pin_e)20), 10);
	assertEquals(2, eventList.size);

	printf("registering 1\r\n");
	registerActuatorEvent(&eventList, 1, list.add((io_pin_e)30), 0);
	registerActuatorEvent(&eventList, 1, list.add((io_pin_e)40), 10);
	assertEquals(4, eventList.size);

	printf("Looking for 0\r\n");
	findEvents(0, &eventList, &result);
	assertEquals(2, result.size);
	assertEquals(4, eventList.size);

	printf("Validating pins\r\n");
	assertEquals(10, result.events[0].actuator->io_pin);
	assertEquals(20, result.events[1].actuator->io_pin);

	printf("Looking for 1\r\n");
	findEvents(1, &eventList, &result);
	assertEquals(2, result.size);
	assertEquals(4, eventList.size);

	assertEquals(30, result.events[0].actuator->io_pin);
	assertEquals(40, result.events[1].actuator->io_pin);
}
