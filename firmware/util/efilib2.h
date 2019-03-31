/**
 * @file	efilib2.h
 *
 * @date Apr 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef EFILIB2_H_
#define EFILIB2_H_

#include "global.h"

/**
 * @return for a given stack memory region returns how much stack was ever used
 */
int getMaxUsedStack(uint8_t *ptr, int size);

#if (EFI_PROD_CODE || EFI_SIMULATOR)
 #define GET_TIMESTAMP() port_rt_get_counter_value()
#else
 #define GET_TIMESTAMP() 0
#endif

#endif /* EFILIB2_H_ */
