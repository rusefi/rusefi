/*
 * listener_array.c
 *
 * @date Jan 1, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "listener_array.h"
#include "main.h"

void registerCallback(IntListenerArray *array, IntListener handler, void *arg) {
	if (array->currentListenersCount == MAX_INT_LISTENER_COUNT)
		fatal("Too many callbacks");
	int index = array->currentListenersCount++;
	array->callbacks[index] = handler;
	array->args[index] = arg;
}

void invokeCallbacks(IntListenerArray *array, int value) {
	for (int i = 0; i < array->currentListenersCount; i++)
		(array->callbacks[i])(value);
}

void invokeJustArgCallbacks(IntListenerArray *array) {
	for (int i = 0; i < array->currentListenersCount; i++) {
		ArgListener listener = (ArgListener)array->callbacks[i];
		void *arg = array->args[i];
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

