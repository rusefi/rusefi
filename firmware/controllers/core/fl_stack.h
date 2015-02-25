/**
 * @file fl_stack.h
 * @brief Fixed-length stack
 *
 * @date Jul 9, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef FL_STACK_H_
#define FL_STACK_H_

#include "main.h"
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
	if(index >= MAXSIZE) {
		firmwareError("FLstack overflow");
		return;
		//warning()
	}
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
	efiAssert(index >= 0 && index < MAXSIZE, "FLget", values[0]);
	return values[index];
}

template<typename T, int MAXSIZE>
int FLStack<T, MAXSIZE>::size() {
	return index;
}

template <class Type, int Dimention>
class ArrayList {
public:
	ArrayList();
	int size;
	Type elements[Dimention];
	void reset(void);
	Type *add(void);
};

template <class Type, int Dimention>
ArrayList< Type, Dimention>::ArrayList(void) {
	memset(&elements, 0, sizeof(elements));
	reset();
}

template <class Type, int Dimention>
void ArrayList< Type, Dimention>::reset(void) {
	size = 0;
}

template <class Type, int Dimention>
Type * ArrayList< Type, Dimention>::add(void) {
	efiAssert(size < Dimention, "add() too many elements", (Type *)NULL);
	return &elements[size++];
}

#endif /* FL_STACK_H_ */
