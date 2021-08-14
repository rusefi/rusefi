/**
 * @file fl_stack.h
 * @brief Fixed-length stack
 *
 * @date Jul 9, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

template<typename T, int MAXSIZE>
class FLStack {
public:
	FLStack();
	void push(T value);
	void reset();
	T pop();
	T get(int index);
	bool remove(T value);
	int size();
	bool isEmpty();
private:
	int currentSize;
	T values[MAXSIZE];
};

template<typename T, int MAXSIZE>
FLStack<T, MAXSIZE>::FLStack() {
	reset();
	memset(values, 0, sizeof(values));
}

template<typename T, int MAXSIZE>
bool FLStack<T, MAXSIZE>::isEmpty() {
	return currentSize == 0;
}

template<typename T, int MAXSIZE>
void FLStack<T, MAXSIZE>::reset() {
	currentSize = 0;
}

template<typename T, int MAXSIZE>
bool FLStack<T, MAXSIZE>::remove(T value) {
	for (int i = 0; i < currentSize; i++) {
		if (values[i] == value) {
			values[0] = values[currentSize - 1];
			currentSize--;
			return true;
		}
	}
	return false;
}

template<typename T, int MAXSIZE>
void FLStack<T, MAXSIZE>::push(T value) {
	if (currentSize >= MAXSIZE) {
		firmwareError(ERROR_FL_STACK_OVERFLOW, "FLstack overflow");
		return;
		//warning()
	}
	values[currentSize++] = value;
}

template<typename T, int MAXSIZE>
T FLStack<T, MAXSIZE>::pop() {
	if (currentSize == 0) {
		firmwareError(CUSTOM_FLSTACK, "FLStack is empty");
	}
	return values[--currentSize];
}

/**
 * @return element at the specified index
 */
template<typename T, int MAXSIZE>
T FLStack<T, MAXSIZE>::get(int index) {
	efiAssert(CUSTOM_ERR_ASSERT, index >= 0 && index < MAXSIZE, "FLget", values[0]);
	return values[index];
}

template<typename T, int MAXSIZE>
int FLStack<T, MAXSIZE>::size() {
	return currentSize;
}

template<class Type, int Dimention>
class ArrayList {
public:
	ArrayList();
	int size;
	Type elements[Dimention];
	void reset(void);
	Type *add(void);
	void removeAt(int index);
};

template<class Type, int Dimention>
ArrayList<Type, Dimention>::ArrayList(void) {
	memset(&elements, 0, sizeof(elements));
	reset();
}

template<class Type, int Dimention>
void ArrayList<Type, Dimention>::removeAt(int index) {
	efiAssertVoid(CUSTOM_ERR_ARRAY_REMOVE, index >= 0 && index < size, "invalid index");
	memcpy(&elements[index], &elements[size - 1], sizeof(Type));
	memset(&elements[size - 1], 0, sizeof(Type));
	size--;
}

template<class Type, int Dimention>
void ArrayList<Type, Dimention>::reset(void) {
	size = 0;
}

template<class Type, int Dimention>
Type * ArrayList<Type, Dimention>::add(void) {
	efiAssert(CUSTOM_ERR_ASSERT, size < Dimention, "add() too many elements", (Type *)NULL);
	return &elements[size++];
}
