/**
 * @file	fifo_buffer.h
 * @brief	A FIFO buffer (base on cyclic_buffer)
 *
 * https://en.wikipedia.org/wiki/FIFO_(computing_and_electronics)
 *
 * @date Aug 6, 2020
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy
 *
*/

#ifndef FIFO_BUFFER_H
#define FIFO_BUFFER_H

#include "cyclic_buffer.h"

// todo: this is not a thread-safe version!
template<typename T, size_t maxSize = CB_MAX_SIZE>
class fifo_buffer : public cyclic_buffer<T, maxSize> {
public:
	fifo_buffer() : currentIndexRead(0) {
	}

	void put(T item);
	T get();
	void clear() /*override*/;

	void put(const T *items, int numItems);
	
	bool isEmpty() const {
		return getCount() == 0;
	}

	bool isFull() const {
		return getCount() >= getSize();
	}

public:
    volatile int currentIndexRead;	// FIFO "tail"
};

template<typename T, size_t maxSize>
void fifo_buffer<T, maxSize>::put(T item) {
	// check if full
	if (!isFull()) {
		cyclic_buffer::add(item);
	}
}

template<typename T, size_t maxSize>
void fifo_buffer<T, maxSize>::put(const T *items, int numItems) {
	for (int i = 0; i < numItems; i++) {
		put(items[i]);
	}
}

template<typename T, size_t maxSize>
T fifo_buffer<T, maxSize>::get() {
	auto ret = elements[currentIndexRead];
	if (!isEmpty()) {
		currentIndexRead = (currentIndexRead + 1) % size;
		count--;
	}
	return ret;
}

template<typename T, size_t maxSize>
void fifo_buffer<T, maxSize>::clear() {
	cyclic_buffer::clear();
	currentIndexRead = 0;
}


#endif /* FIFO_BUFFER_H */