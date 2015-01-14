/*
 * @file listener_array.cpp
 *
 * @date Jan 1, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "listener_array.h"
#include "main.h"

void IntListenerArray::registerCallback(IntListener handler, void *arg) {
	efiAssertVoid(currentListenersCount < MAX_INT_LISTENER_COUNT, "Too many callbacks");
	int index = currentListenersCount++;
	callbacks[index] = handler;
	args[index] = arg;
}

void IntListenerArray::registerCallback(Void listener) {

}

void invokeCallbacks(IntListenerArray *array, int value) {
	for (int i = 0; i < array->currentListenersCount; i++)
		(array->callbacks[i])(value);
}

void IntListenerArray::invokeJustArgCallbacks() {
	for (int i = 0; i < currentListenersCount; i++) {
		ArgListener listener = (ArgListener)callbacks[i];
		void *arg = args[i];
		(listener)(arg);
	}
}

void invokeArgIntCallbacks(IntListenerArray *array, int value) {
	for (int i = 0; i < array->currentListenersCount; i++) {
		ArgIntListener listener = (ArgIntListener)array->callbacks[i];
		void *arg = array->args[i];
		(listener)(arg, value);
	}
}

void invokeIntIntCallbacks(IntListenerArray *array, int value, int value2) {
	for (int i = 0; i < array->currentListenersCount; i++) {
		IntIntListener listener = (IntIntListener)array->callbacks[i];
		(listener)(value, value2);
	}
}

void invokeIntIntVoidCallbacks(IntListenerArray *array, int value, int value2) {
	for (int i = 0; i < array->currentListenersCount; i++) {
		IntIntVoidListener listener = (IntIntVoidListener)array->callbacks[i];
		(listener)(value, value2, array->args[i]);
	}
}

void clearCallbacks(IntListenerArray *array) {
	array->currentListenersCount = 0;
}
