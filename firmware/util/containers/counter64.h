/*
 * counter64.h
 *
 *  Created on: Mar 31, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#ifndef UTIL_CONTAINERS_COUNTER64_H_
#define UTIL_CONTAINERS_COUNTER64_H_

#include "global.h"

typedef struct {
	// todo: would probably be better to keep the high bits as 32 bit field to be sure
	volatile efitime_t highBits;
	volatile uint32_t lowBits;
} State64;

void updateAndSet(State64 *state, uint32_t value);

class Overflow64Counter
{
  public:
	Overflow64Counter();

	efitime_t get();
#if EFI_UNIT_TEST
	efitime_t update(uint32_t value);
#endif

	State64 state;
};

#endif /* UTIL_CONTAINERS_COUNTER64_H_ */
