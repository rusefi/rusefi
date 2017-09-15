/*
 * test_pid_auto.cpp
 *
 *  Created on: Sep 14, 2017
 * @author Andrey Belomutskiy, (c) 2012-2017
 */


#include "main.h"
#include "test_pid_auto.h"

efitimems_t mockTimeMs = 0;

efitimems_t currentTimeMillis(void) {
	return mockTimeMs;
}

void testPidAutoZigZag() {
	mockTimeMs = 0;

}
