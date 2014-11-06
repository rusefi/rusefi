/**
 * @file	mpu_util.cpp
 *
 * @date Jul 27, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "mpu_util.h"
#include "error_handling.h"

extern "C" {
int getRemainingStack(Thread *otp);
}

extern stkalign_t __main_stack_base__;

int getRemainingStack(Thread *otp) {
#if CH_DBG_ENABLE_STACK_CHECK
	register struct intctx *r13 asm ("r13");
	otp->activeStack = r13;

	int rs;
	if (dbg_isr_cnt > 0) {
		// ISR context
		rs = (stkalign_t *) (r13 - 1) - &__main_stack_base__;
	} else {

		rs = (stkalign_t *) (r13 - 1) - otp->p_stklimit;
	}
	otp->remainingStack = rs;
	return rs;
#else
	return 99999;
#endif /* CH_DBG_ENABLE_STACK_CHECK */
}


void baseHardwareInit(void) {
	// looks like this holds a random value on start? Let's set a nice clean zero
	DWT_CYCCNT = 0;
}

void DebugMonitorVector(void) {

	chDbgPanic3("DebugMonitorVector", __FILE__, __LINE__);

	while (TRUE)
		;
}

void UsageFaultVector(void) {

	chDbgPanic3("UsageFaultVector", __FILE__, __LINE__);

	while (TRUE)
		;
}

void BusFaultVector(void) {

	chDbgPanic3("BusFaultVector", __FILE__, __LINE__);

	while (TRUE) {
        }
}

void HardFaultVector(void) {

	chDbgPanic3("HardFaultVector", __FILE__, __LINE__);

	while (TRUE) {
        }
}



