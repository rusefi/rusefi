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

#if EFI_UNIT_TEST
#include "global.h"
#endif // EFI_UNIT_TEST

// todo: this is not a thread-safe version!
template<typename T, size_t maxSize = CB_MAX_SIZE>
class fifo_buffer : public cyclic_buffer<T, maxSize> {
	using cyclic_buffer<T, maxSize>::add;
	using cyclic_buffer<T, maxSize>::getSize;
	using cyclic_buffer<T, maxSize>::elements;
	using cyclic_buffer<T, maxSize>::m_size, cyclic_buffer<T, maxSize>::count;

public:
	fifo_buffer() : currentIndexRead(0) {
	}

	virtual bool put(T item);
	T get();
	void clear() /*override*/;

	virtual bool put(const T *items, int numItems);
	
	bool isEmpty() const {
		return getCount() == 0;
	}

	bool isFull() const {
		return getCount() >= getSize();
	}

    int getCount() const {
        return cyclic_buffer<T, maxSize>::getCount();
    }

    int getSize() const {
        return cyclic_buffer<T, maxSize>::getSize();
    }

    const volatile T* getElements() const {
        return elements;
    }

public:
    int currentIndexRead;	// FIFO "tail"
};

template<typename T, size_t maxSize>
bool fifo_buffer<T, maxSize>::put(T item) {
	// check if full
	if (!isFull()) {
		add(item);
		return true;
	}
	return false;
}

template<typename T, size_t maxSize>
bool fifo_buffer<T, maxSize>::put(const T *items, int numItems) {
	for (int i = 0; i < numItems; i++) {
		if (!put(items[i]))
			return false;
	}
	return true;
}

template<typename T, size_t maxSize>
T fifo_buffer<T, maxSize>::get() {
	T &ret = (T &)elements[currentIndexRead];
	if (!isEmpty()) {
		currentIndexRead = (currentIndexRead + 1) % m_size;
		count--;
	}
	return ret;
}

template<typename T, size_t maxSize>
void fifo_buffer<T, maxSize>::clear() {
	cyclic_buffer<T, maxSize>::clear();
	currentIndexRead = 0;
}

template<typename T, size_t maxSize = CB_MAX_SIZE>
class fifo_buffer_sync : public fifo_buffer<T, maxSize> {
public:
	fifo_buffer_sync() {
#if !EFI_UNIT_TEST
		osalThreadQueueObjectInit(&q_waiting);
#endif // EFI_UNIT_TEST
	}

	bool put(T item) override {
		chSysLock();
		if (fifo_buffer<T, maxSize>::isFull()) {
			chSysUnlock();
			return false;
		}
		fifo_buffer<T, maxSize>::put(item);
		osalThreadDequeueNextI(&q_waiting, MSG_OK);
		chSysUnlock();
		return true;
	}

	bool put(const T *items, int numItems) override {
		for (int i = 0; i < numItems; i++) {
			if (!put(items[i]))
				return false;
		}
		return true;
	}

	bool get(T &item, int timeout) {
		chSysLock();
#if !EFI_UNIT_TEST
		while (fifo_buffer<T, maxSize>::isEmpty()) {
			msg_t msg = osalThreadEnqueueTimeoutS(&q_waiting, timeout);
			if (msg != MSG_OK) {
				chSysUnlock();
				return false;
			}
		}
#endif // EFI_UNIT_TEST
		item = fifo_buffer<T, maxSize>::get();
		chSysUnlock();
		return true;
    }

	void clear() {
		chSysLock();
		fifo_buffer<T, maxSize>::clear();
#if !EFI_UNIT_TEST
		osalThreadDequeueAllI(&q_waiting, MSG_RESET);
#endif // EFI_UNIT_TEST
		chSysUnlock();
	}

protected:
#if !EFI_UNIT_TEST
	threads_queue_t q_waiting;
#endif // EFI_UNIT_TEST
};

#endif /* FIFO_BUFFER_H */
