/**
 * @file	mpu_util.cpp
 *
 * @date Jul 27, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */
#include "pch.h"
#include "flash_int.h"

bool allowFlashWhileRunning() {
	// Never allow flash while running on F4, dual bank not implemented.
	return false;
}

size_t flashSectorSize(flashsector_t sector) {
	// sectors 0..11 are the 1st memory bank (1Mb), and 12..23 are the 2nd (the same structure).
	if (sector <= 3 || (sector >= 12 && sector <= 15))
		return 16 * 1024;
	else if (sector == 4 || sector == 16)
		return 64 * 1024;
	else if ((sector >= 5 && sector <= 11) || (sector >= 17 && sector <= 23))
		return 128 * 1024;
	return 0;
}

#define TM_ID_GetFlashSize()	(*(__IO uint16_t *) (FLASHSIZE_BASE))

uintptr_t getFlashAddrFirstCopy() {
	/* last 128K sector on 512K devices */
	if (TM_ID_GetFlashSize() <= 512)
		return 0x08060000;
	return 0x080E0000;
}

uintptr_t getFlashAddrSecondCopy() {
	/* no second copy on 512K devices */
	if (TM_ID_GetFlashSize() <= 512)
		return 0x000000000;
	return 0x080C0000;
}

/*
 * Standby for both F4 & F7 works perfectly, with very little current consumption.
 * Downside is that there is a limited amount of pins that can wakeup F7, and only PA0 for F4XX.
*/
void stm32_standby() {
	// Don't get bothered by interrupts
	__disable_irq();

	SysTick->CTRL = 0;
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	PWR->CR |= PWR_CR_PDDS;	// PDDS = use standby mode (not stop mode)
	PWR->CR |= PWR_CR_CSBF;	// Clear standby flag

	// Do anything the board wants to prepare for standby mode - enabling wakeup sources!
	boardPrepareForStandby();

	__WFI();
}
