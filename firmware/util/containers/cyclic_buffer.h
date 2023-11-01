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

#pragma once

#include <limits>
#include <string.h>
#include <stdint.h>

static const short CB_MAX_SIZE = 128;

template<typename T, size_t maxSize = CB_MAX_SIZE>
class cyclic_buffer
{
  public:
	cyclic_buffer();
    explicit cyclic_buffer(int size);

  public:
    void add(T value);
    T get(int index) const;
    T sum(size_t length) const;
    T maxValue(size_t length) const;
    T minValue(size_t length) const;
    void setSize(size_t size);
    bool contains(T value) const;
    int getSize() const;
    int getCount() const;
    void clear();
    T elements[maxSize];
    uint16_t currentIndex;

  protected:
    size_t m_size;
    /**
     * number of elements added into this buffer, would be eventually bigger then size
     */
    size_t count;
};

template<typename T, size_t maxSize>
cyclic_buffer<T, maxSize>::cyclic_buffer() : cyclic_buffer(maxSize) {
}

template<typename T, size_t maxSize>
cyclic_buffer<T, maxSize>::cyclic_buffer(int size) {
	setSize(size);
}

template<typename T, size_t maxSize>
void cyclic_buffer<T, maxSize>::add(T value) {
	// Too lazy to make this thread safe, but at the very least let's never let currentIndex
	// become invalid.  And yes I did see a crash due to an overrun here.
	uint16_t idx = currentIndex;

	((T &)elements[idx]) = value;

	if (++idx == m_size) {
		idx = 0;
	}
	currentIndex = idx;

	count = count + 1;
}

template<typename T, size_t maxSize>
bool cyclic_buffer<T, maxSize>::contains(T value) const {
	for (int i = 0; i < currentIndex ; i++) {
		if (elements[i] == value) {
			return true;
		}
	}
	return false;
}

template<typename T, size_t maxSize>
void cyclic_buffer<T, maxSize>::setSize(size_t size) {
	clear();
	m_size = size < maxSize ? size : maxSize;
}

template<typename T, size_t maxSize>
int cyclic_buffer<T, maxSize>::getSize() const {
	return m_size;
}

template<typename T, size_t maxSize>
int cyclic_buffer<T, maxSize>::getCount() const {
	return count;
}

template<typename T, size_t maxSize>
T cyclic_buffer<T, maxSize>::get(int index) const {
	while (index < 0) {
		index += m_size;
	}
	while (index >= m_size) {
		index -= m_size;
	}
	return elements[index];
}

template<typename T, size_t maxSize>
T cyclic_buffer<T, maxSize>::maxValue(size_t length) const {
	if (length > count) {
		// not enough data in buffer
		length = count;
	}
	int ci = currentIndex; // local copy to increase thread-safety
	T result = std::numeric_limits<T>::min();
	for (size_t i = 0; i < length; ++i) {
		int index = ci - i - 1;
		while (index < 0) {
			index += m_size;
		}

		if (elements[index] > result) {
			result = elements[index];
		}
	}
	return result;
}

template<typename T, size_t maxSize>
T cyclic_buffer<T, maxSize>::minValue(size_t length) const {
	if (length > count) {
		length = count;
	}
	int ci = currentIndex; // local copy to increase thread-safety
	T result = std::numeric_limits<T>::max();
	for (size_t i = 0; i < length; ++i) {
		int index = ci - i - 1;
		while (index < 0) {
			index += m_size;
		}

		if (elements[index] < result) {
			result = elements[index];
		}
	}
	return result;
}

template<typename T, size_t maxSize>
T cyclic_buffer<T, maxSize>::sum(size_t length) const {
	if (length > count) {
		length = count;
	}

	int ci = currentIndex; // local copy to increase thread-safety
	T result = 0;

	for (size_t i = 0; i < length; ++i) {
		int index = ci - i - 1;
		while (index < 0) {
			index += m_size;
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
