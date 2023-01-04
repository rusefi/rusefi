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
STOP mode for F7 is needed for wakeup from multiple EXTI pins. For example PD0, which is CAN rx.
However, for F40X & F42X this may be useless. STOP in itself eats more current than standby. 
With F4 only having PA0 available for wakeup, this negates its need.
*/
/*
void stm32_stop() {
	// Don't get bothered by interrupts
	__disable_irq();

	SysTick->CTRL = 0;
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	enginePins.errorLedPin.setValue(0);
	enginePins.runningLedPin.setValue(0);
	enginePins.communicationLedPin.setValue(0);
	enginePins.warningLedPin.setValue(0);

	PWR->CR &= ~PWR_CR_PDDS;	// cleared PDDS means stop mode (not standby) 
	PWR->CR |= PWR_CR_FPDS;	// turn off flash in stop mode
	#ifdef STM32F429xx //F40X Does not have these regulators available.
	PWR->CR |= PWR_CR_UDEN;	// regulator underdrive in stop mode *
	PWR->CR |= PWR_CR_LPUDS;	// low power regulator in under drive mode
	#endif
	PWR->CR |= PWR_CR_LPDS;	// regulator in low power mode

	// Do anything the board wants to prepare for stop mode - enabling wakeup sources!
	boardPrepareForStop();

	// enable Deepsleep mode
	__WFI();

	// Lastly, reboot
	NVIC_SystemReset();
}
*/
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
