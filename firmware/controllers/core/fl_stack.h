/**
 * @file fl_stack.h
 * @brief Fixed-length stack
 *
 * @date Jul 9, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef FL_STACK_H_
#define FL_STACK_H_

#include "error_handling.h"

template<typename T, int MAXSIZE>
class FLStack {
public:
	FLStack();
	void push(T value);
	void reset();
	T pop();
	T get(int index);
	int size();
	bool isEmpty();
private:
	int index;
	T values[MAXSIZE];
};

template<typename T, int MAXSIZE>
FLStack<T, MAXSIZE>::FLStack() {
	reset();
}

template<typename T, int MAXSIZE>
bool FLStack<T, MAXSIZE>::isEmpty() {
	return index == 0;
}

template<typename T, int MAXSIZE>
void FLStack<T, MAXSIZE>::reset() {
	index = 0;
}

template<typename T, int MAXSIZE>
void FLStack<T, MAXSIZE>::push(T value) {
	values[index++] = value;
}

template<typename T, int MAXSIZE>
T FLStack<T, MAXSIZE>::pop() {
	if (index == 0) {
		firmwareError("FLStack is empty");
	}
	return values[--index];
}

template<typename T, int MAXSIZE>
T FLStack<T, MAXSIZE>::get(int index) {
	return values[index];
}

template<typename T, int MAXSIZE>
int FLStack<T, MAXSIZE>::size() {
	return index;
}

#endif /* FL_STACK_H_ */
