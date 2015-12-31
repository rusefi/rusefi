/*
 * @file listener_array.cpp
 *
 * @date Jan 1, 2013
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "listener_array.h"
#include "main.h"

IntListenerArray::IntListenerArray() {
	currentListenersCount = 0;
	memset(&args, 0, sizeof(args));
	memset(&callbacks, 0, sizeof(callbacks));
}

void IntListenerArray::registerCallback(VoidInt handler, void *arg) {
	efiAssertVoid(currentListenersCount < MAX_INT_LISTENER_COUNT, "Too many callbacks");
	int index = currentListenersCount++;
	callbacks[index] = handler;
	args[index] = arg;
}

void IntListenerArray::registerCallback(Void listener) {
	registerCallback((VoidInt)listener, NULL);
}

void invokeCallbacks(IntListenerArray *array, int value) {
	for (int i = 0; i < array->currentListenersCount; i++)
		(array->callbacks[i])(value);
}

void IntListenerArray::invokeJustArgCallbacks() {
	for (int i = 0; i < currentListenersCount; i++) {
		VoidPtr listener = (VoidPtr)callbacks[i];
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
		VoidIntInt listener = (VoidIntInt)array->callbacks[i];
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
