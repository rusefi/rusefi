/**
 * @file	efilib2.cpp
 *
 * @date Apr 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "efilib2.h"

/**
 * See also getRemainingStack()
 */
int getMaxUsedStack(uint8_t *ptr, int size) {
	/**
	 * maximum used stack size total stack buffer size minus position of first modified byte
	 */
#if ! EFI_UNIT_TEST
	int used = 0;
	for (int i = 0; i < size; i++) {
		if (ptr[i] != CH_DBG_STACK_FILL_VALUE) {
			return size - i;
		}
	}
#endif /* EFI_UNIT_TEST */
	return 0;
}
