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

void assertInterruptPriority(const char* func, uint8_t expectedPrio) {
	auto isr = static_cast<uint8_t>(SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) - 16;

	auto actualMask = NVIC->IP[isr];
	auto expectedMask = NVIC_PRIORITY_MASK(expectedPrio);

	if (actualMask != expectedMask) {
		firmwareError(ObdCode::RUNTIME_CRITICAL_WRONG_IRQ_PRIORITY, "bad isr priority at %s expected %02x got %02x", func, expectedMask, actualMask);
	}
}
#endif  /* EFI_PROD_CODE */

void causeHardFault() {
#if EFI_PROD_CODE
	// Set the function pointer to an invalid address
	void (*invalidFunction)() = (void(*)())0xDEADBEEF;
	// Calling the invalid function will trigger a hard fault
	invalidFunction();
#endif  /* EFI_PROD_CODE */
}
