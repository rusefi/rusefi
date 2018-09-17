/**
 * @file	efilib2.h
 *
 * @date Apr 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef EFILIB2_H_
#define EFILIB2_H_

#include <stdint.h>
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

#if (EFI_PROD_CODE || EFI_SIMULATOR)
 #define GET_TIMESTAMP() port_rt_get_counter_value()
#else
 #define GET_TIMESTAMP() 0
#endif

#endif /* EFILIB2_H_ */
