/**
 * @file	efilib2.cpp
 *
 * @date Apr 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
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

#if EFI_UNIT_TEST
efitime_t Overflow64Counter::update(uint32_t value) {
	updateAndSet(&state, value);
	return state.highBits + state.lowBits;
}
#endif
