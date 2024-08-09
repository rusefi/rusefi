/**
 * @file	mpu_util.cpp
 *
 * @date Nov 3, 2023
 * @author Andrey Gusakov, (c) 2023
 */
#include "pch.h"
#include "flash_int.h"

bool mcuCanFlashWhileRunning() {
    /* TODO: check for actual flash configuration? */
    /* currently we support only AT32F43X with dual-bank flash, so allow flashing to second bank */
    /* TODO: Seems AT32 is still freezes even write is happen to second bank, while executing code from first */
	return true;
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
