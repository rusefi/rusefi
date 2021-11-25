/*
 * listener_array.h
 *
 * @date Jan 1, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include <stddef.h>
#include "rusefi_types.h"

// todo: reorder parameters for consistency?
typedef void (*IntIntVoidListener)(int value1, int value2, void *arg);

// todo: rename 'Arg' to 'Void'?
typedef void (*ArgListener)(void *arg);
typedef void (*ArgIntListener)(void *arg, int value);

// todo: rename this class, that's not just 'callback(int param) anymore
template<int MAX_INT_LISTENER_COUNT>
class IntListenerArray {
public:
	IntListenerArray();
	void clear();
	void registerCallback(VoidInt handler, void *arg);
	void registerCallback(Void listener);
	void invokeJustArgCallbacks() const;
	int currentListenersCount = 0;
	VoidInt callbacks[MAX_INT_LISTENER_COUNT];
	void * args[MAX_INT_LISTENER_COUNT];
};

//template<int MAX_INT_LISTENER_COUNT>
//void invokeCallbacks(IntListenerArray *array, int value);
//
//template<int MAX_INT_LISTENER_COUNT>
//void invokeJustArgCallbacks(IntListenerArray *array);
//
//template<int MAX_INT_LISTENER_COUNT>
//void invokeArgIntCallbacks(IntListenerArray *array, int value);
//
//template<int MAX_INT_LISTENER_COUNT>
//void invokeIntIntCallbacks(IntListenerArray *array, int value, int value2);
//
//template<int MAX_INT_LISTENER_COUNT>
//void invokeIntIntVoidCallbacks(IntListenerArray *array, int value, int value2);

//template<int MAX_INT_LISTENER_COUNT>
//void clearCallbacks(IntListenerArray *array);

template<int MAX_INT_LISTENER_COUNT>
IntListenerArray<MAX_INT_LISTENER_COUNT>::IntListenerArray() {
	clear();
}

template<int MAX_INT_LISTENER_COUNT>
void IntListenerArray<MAX_INT_LISTENER_COUNT>::clear() {
	currentListenersCount = 0;
	memset(&args, 0, sizeof(args));
	memset(&callbacks, 0, sizeof(callbacks));
}

template<int MAX_INT_LISTENER_COUNT>
void IntListenerArray<MAX_INT_LISTENER_COUNT>::registerCallback(VoidInt handler, void *arg) {
	efiAssertVoid(CUSTOM_ERR_6606, currentListenersCount < MAX_INT_LISTENER_COUNT, "Too many callbacks");
	int index = currentListenersCount++;
	callbacks[index] = handler;
	args[index] = arg;
}

template<int MAX_INT_LISTENER_COUNT>
void IntListenerArray<MAX_INT_LISTENER_COUNT>::registerCallback(Void listener) {
	registerCallback((VoidInt)listener, NULL);
}

template<int MAX_INT_LISTENER_COUNT>
void invokeCallbacks(IntListenerArray<MAX_INT_LISTENER_COUNT> *array, int value) {
	for (int i = 0; i < array->currentListenersCount; i++)
		(array->callbacks[i])(value);
}

template<int MAX_INT_LISTENER_COUNT>
void IntListenerArray<MAX_INT_LISTENER_COUNT>::invokeJustArgCallbacks() const {
	for (int i = 0; i < currentListenersCount; i++) {
		VoidPtr listener = (VoidPtr)(void*)callbacks[i];
		void *arg = args[i];
		(listener)(arg);
	}
}

template<int MAX_INT_LISTENER_COUNT>
void invokeArgIntCallbacks(IntListenerArray<MAX_INT_LISTENER_COUNT> *array, int value) {
	for (int i = 0; i < array->currentListenersCount; i++) {
		ArgIntListener listener = (ArgIntListener)array->callbacks[i];
		void *arg = array->args[i];
		(listener)(arg, value);
	}
}

template<int MAX_INT_LISTENER_COUNT>
void invokeIntIntCallbacks(IntListenerArray<MAX_INT_LISTENER_COUNT> *array, int value, int value2) {
	for (int i = 0; i < array->currentListenersCount; i++) {
		VoidIntInt listener = (VoidIntInt)array->callbacks[i];
		(listener)(value, value2);
	}
}

template<int MAX_INT_LISTENER_COUNT>
void invokeIntIntVoidCallbacks(IntListenerArray<MAX_INT_LISTENER_COUNT> *array, int value, int value2) {
	for (int i = 0; i < array->currentListenersCount; i++) {
		IntIntVoidListener listener = (IntIntVoidListener)array->callbacks[i];
		(listener)(value, value2, array->args[i]);
	}
}

template<int MAX_INT_LISTENER_COUNT>
void clearCallbacks(IntListenerArray<MAX_INT_LISTENER_COUNT> *array) {
	array->currentListenersCount = 0;
}

