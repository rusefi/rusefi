/*
 * @file tachometer.cpp
 *
 * @date Aug 18, 2015
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "tachometer.h"

EXTERN_ENGINE;

void initTachometer(void) {
	if (boardConfiguration->tachOutputPin == GPIO_UNASSIGNED)
		return;

}
