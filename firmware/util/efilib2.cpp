/**
 * @file	efilib2.cpp
 *
 * @date Apr 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#if (EFI_PROD_CODE || EFI_SIMULATOR)
 #define GET_VALUE() hal_lld_get_counter_value()
#else
 #define GET_VALUE() 0
#endif

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
	state.highBits = 0;
	state.lowBits = 0;
}

/**
 * in order to have atomic writes this should be invoked within a critical section
 */
void updateAndSet(State64 *state, uint32_t value) {
	if (value < state->lowBits) {
		// new value less than previous value means there was an overflow in that 32 bit counter
		state->highBits += 0x100000000LL;
	}
	state->lowBits = value;
}

uint64_t Overflow64Counter::update(uint32_t value) {
	updateAndSet(&state, value);
	return state.highBits + state.lowBits;
}

// todo: make this a macro? always inline?
uint64_t Overflow64Counter::get() {
	/**
	 * this method is lock-free and thread-safe, that's because the 'update' method
	 * is atomic with a critical zone requirement.
	 *
	 * http://stackoverflow.com/questions/5162673/how-to-read-two-32bit-counters-as-a-64bit-integer-without-race-condition
	 */
	// these are local copies for thread-safery
	// todo: this is still not atomic, so technically not thread safe.
	uint64_t localH;
	uint32_t localLow;
	while (true) {
		localH = state.highBits;
		localLow = state.lowBits;
		uint64_t localH2 = state.highBits;
		if (localH == localH2)
			break;
	}
	/**
	 * We need to take current counter after making a local 64 bit snapshot
	 */
	uint32_t value = GET_VALUE();

	if (value < localLow) {
		// new value less than previous value means there was an overflow in that 32 bit counter
		localH += 0x100000000LL;
	}

	return localH + value;
}
