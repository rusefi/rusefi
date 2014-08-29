/**
 * @file	efilib2.cpp
 *
 * @date Apr 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "efilib2.h"

/**
 * The main use-case of this class is to keep track of a 64-bit global number of CPU ticks from reset.
 *
 * stm32f4 hardware has a 32-bit Cycle Count Register (CYCCNT), which is incremented with every CPU cycle.
 * With 32 bits and 168MHz speed this counter overflows every 4B/168M = 23 seconds. The job of this class is to
 * keep track of the current CYCCNT value, detect these overflows, and provide a nice,
 * clean 64 bit global cycle counter.
 *
 * In order for this to function, it's your responsibility to invoke offer() method at least once a second.
 */
Overflow64Counter::Overflow64Counter() {
	currentBase = 0;
	currentValue = 0;
}

uint64_t Overflow64Counter::get(uint32_t value, int isPrimaryThread) {
	// this method is lock-free, only one thread is allowed to commit state
	// these are local copies for thread-safery
	// todo: this is still not atomic, so technically not thread safe.
	uint32_t localValue = currentValue;
	uint64_t localBase = currentBase;
	if (value < localValue) {
		// new value less than previous value means there was an overflow in that 32 bit counter
		localBase += 0x100000000LL;
	}
	if (isPrimaryThread) {
		currentValue = value;
		currentBase = localBase;
	}

	return localBase + value;
}
