/*
 * listener_array.h
 *
 * @date Jan 1, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef LISTENER_ARRAY_H_
#define LISTENER_ARRAY_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define MAX_INT_LISTENER_COUNT 15

typedef void (*IntListener)(int value);
typedef void (*IntIntListener)(int value1, int value2);
// todo: reorder parameters for consistency?
typedef void (*IntIntVoidListener)(int value1, int value2, void *arg);

// todo: rename 'Arg' to 'Void'?
typedef void (*ArgListener)(void *arg);
typedef void (*ArgIntListener)(void *arg, int value);

typedef struct {
	int currentListenersCount;
	IntListener callbacks[MAX_INT_LISTENER_COUNT];
	void * args[MAX_INT_LISTENER_COUNT];
} IntListenerArray;

void registerCallback(IntListenerArray *array, IntListener handler, void *arg);
void invokeCallbacks(IntListenerArray *array, int value);
void invokeJustArgCallbacks(IntListenerArray *array);
void invokeArgIntCallbacks(IntListenerArray *array, int value);
void invokeIntIntCallbacks(IntListenerArray *array, int value, int value2);
void invokeIntIntVoidCallbacks(IntListenerArray *array, int value, int value2);
void clearCallbacks(IntListenerArray *array);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LISTENER_ARRAY_H_ */
