/**
 * @file	arm_common.cpp
 * @brief	Low level common ARM code
 *
 * @date Nov 14, 2023
 * @author Andrey Belomutskiy, (c) 2012-2023
 * @author  andreika <prometheus.pcb@gmail.com>
 */

#include "pch.h"

#if EFI_PROD_CODE
#include "mpu_util.h"
#endif  /* EFI_PROD_CODE */

void causeHardFault() {
#if EFI_PROD_CODE
	// Set the function pointer to an invalid address
	void (*invalidFunction)() = (void(*)())0xDEADBEEF;
	// Calling the invalid function will trigger a hard fault
	invalidFunction();
#endif  /* EFI_PROD_CODE */
}
