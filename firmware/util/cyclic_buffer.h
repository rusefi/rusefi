/**
 * @file	cyclic_buffer.h
 * @brief	A cyclic buffer is a data structure that uses a single, fixed-size buffer as if it were connected end-to-end.
 *
 * http://en.wikipedia.org/wiki/Circular_buffer
 *
 * @date Dec 8, 2013
 * @author Andrey Belomutskiy, Daniel Hill
 *
 * Daniel Hill - Modified to use C++ - Mar 2, 2014
*/

#ifndef CYCLIC_BUFFER_H
#define CYCLIC_BUFFER_H

#include <string.h>

static const short CB_MAX_SIZE = 128;

#define BUFFER_MAX_VALUE 200123

template<typename T, size_t maxSize = CB_MAX_SIZE>
class cyclic_buffer
{
  public:
	cyclic_buffer();
    cyclic_buffer(int size);
  //cpctor
    cyclic_buffer(const cyclic_buffer& cb);
  //dtor
    ~cyclic_buffer();

  public:
  //overloaded =operator
    cyclic_buffer& operator=(const cyclic_buffer& rhCb);

  public:
    void add(T value);
    T get(int index);
    T sum(int length);
    T maxValue(int length);
    T minValue(int length);
    void setSize(int size);
    int getSize();
    int getCount();
    void clear();
    volatile T elements[maxSize];
    volatile int currentIndex;

  private:
    void baseC(int size);
    /**
     * number of elements added into this buffer, would be eventually bigger then size
     */
    volatile int count;
    int size;
};

template<typename T, size_t maxSize>
cyclic_buffer<T, maxSize>::cyclic_buffer() {
  baseC(maxSize);
}

template<typename T, size_t maxSize>
cyclic_buffer<T, maxSize>::cyclic_buffer(int size) {
  baseC(size);
}

template<typename T, size_t maxSize>
void cyclic_buffer<T, maxSize>::baseC(int size) {
	currentIndex = 0;
	setSize(size);
}

template<typename T, size_t maxSize>
cyclic_buffer<T, maxSize>::cyclic_buffer(const cyclic_buffer& cb) {
	//Deep copy the data
	currentIndex = cb.currentIndex;
	count = cb.count;
	size = cb.size;
	for (int i = 0; i < size; ++i) {
		elements[i] = cb.elements[i];
	}
}

template<typename T, size_t maxSize>
cyclic_buffer<T, maxSize>::~cyclic_buffer() {
	//No dynamic allocation - safe to leave
}

//template<typename T, size_t maxSize>
//cyclic_buffer& cyclic_buffer<T, maxSize>::operator=(const cyclic_buffer<T, maxSize>& rhCb) {
//	//Deep copy
//	currentIndex = rhCb.currentIndex;
//	count = rhCb.count;
//	for (int i = 0; i < size; ++i) {
//		elements[i] = rhCb.elements[i];
//	}
//	return *this;
//}

template<typename T, size_t maxSize>
void cyclic_buffer<T, maxSize>::add(T value) {
	elements[currentIndex] = value;

	++currentIndex;
	if (currentIndex == size) {
		currentIndex = 0;
	}

	++count;
}

template<typename T, size_t maxSize>
void cyclic_buffer<T, maxSize>::setSize(int size) {
	clear();
	this->size = size < maxSize ? size : maxSize;
}

template<typename T, size_t maxSize>
int cyclic_buffer<T, maxSize>::getSize() {
	return size;
}

template<typename T, size_t maxSize>
int cyclic_buffer<T, maxSize>::getCount() {
	return count;
}

template<typename T, size_t maxSize>
T cyclic_buffer<T, maxSize>::get(int index) {
	while (index < 0) {
		index += size;
	}
	while (index >= size) {
		index -= size;
	}
	return elements[index];
}

template<typename T, size_t maxSize>
T cyclic_buffer<T, maxSize>::maxValue(int length) {
	if (length > count) {
		// not enough data in buffer
		length = count;
	}
	int ci = currentIndex; // local copy to increase thread-safety
	T result = -BUFFER_MAX_VALUE; // todo: better min value?
	for (int i = 0; i < length; ++i) {
		int index = ci - i - 1;
		while (index < 0) {
			index += size;
		}

		if (elements[index] > result) {
			result = elements[index];
		}
	}
	return result;
}

template<typename T, size_t maxSize>
T cyclic_buffer<T, maxSize>::minValue(int length) {
	if (length > count) {
		length = count;
	}
	int ci = currentIndex; // local copy to increase thread-safety
	T result = +BUFFER_MAX_VALUE; // todo: better max value?
	for (int i = 0; i < length; ++i) {
		int index = ci - i - 1;
		while (index < 0) {
			index += size;
		}

		if (elements[index] < result) {
			result = elements[index];
		}
	}
	return result;
}

template<typename T, size_t maxSize>
T cyclic_buffer<T, maxSize>::sum(int length) {
	if (length > count) {
		length = count;
	}

	int ci = currentIndex; // local copy to increase thread-safety
	T result = 0;

	for (int i = 0; i < length; ++i) {
		int index = ci - i - 1;
		while (index < 0) {
			index += size;
		}

		result += elements[index];
	}

	return result;
}

template<typename T, size_t maxSize>
void cyclic_buffer<T, maxSize>::clear() {
	memset((void*) elements, 0, sizeof(elements)); // I would usually use static_cast, but due to the elements being volatile we cannot.
	count = 0;
	currentIndex = 0;
}

#endif //CYCLIC_BUFFER_H
