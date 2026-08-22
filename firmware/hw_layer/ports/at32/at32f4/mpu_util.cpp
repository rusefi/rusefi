/**
 * @file	mpu_util.cpp
 *
 * @date Nov 3, 2023
 * @author Andrey Gusakov, (c) 2023
 */
#include "pch.h"
#include "flash_int.h"

bool mcuCanFlashWhileRunning() {
    /* The AT32F43X has dual-bank flash, but programming the second bank still
     * freezes the CPU (no true read-while-write on this silicon - verified the
     * hard way: a 2.4 s MFS GC erase mid-run stalled the engine and wedged the
     * NT clock). Report false so settings writes take the
     * custom_board_allowFlashNow deferral path in storage.cpp. */
	return false;
}

/* TODO: fix name! */
void stm32_standby() {
    // Don't get bothered by interrupts
    __disable_irq();

    /* TODO: validate! */
#if 0
    SysTick->CTRL = 0;
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    PWR->CR |= PWR_CR_PDDS; // PDDS = use standby mode (not stop mode)
    PWR->CR |= PWR_CR_CSBF; // Clear standby flag
#endif

    // Do anything the board wants to prepare for standby mode - enabling wakeup sources!
    boardPrepareForStandby();

    __WFI();
}

uintptr_t getFlashAddrFirstCopy() {
	return 0;
}

uintptr_t getFlashAddrSecondCopy() {
	return 0;
}

void assertInterruptPriority(const char* func, uint8_t expectedPrio) {
}

void printWRPBits() {
}

void printOptBytes() {
}

void removeWRP() {
}
