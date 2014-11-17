/**
 * @file	efilib2.h
 *
 * @date Apr 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef EFILIB2_H_
#define EFILIB2_H_

#include <stdint.h>

typedef struct {
	uint64_t highBits;
	uint32_t lowBits;
} State64;

void updateAndSet(State64 *state, uint32_t value);

class Overflow64Counter
{
  public:
	Overflow64Counter();

	uint64_t get();
	uint64_t update(uint32_t value);

	State64 state;
};

#endif /* EFILIB2_H_ */
