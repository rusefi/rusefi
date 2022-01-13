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

uintptr_t getFlashAddrFirstCopy() {
	return 0x080E0000;
}

uintptr_t getFlashAddrSecondCopy() {
	return 0x080C0000;
}

void stm32_stop() {
	SysTick->CTRL = 0;
	RCC->AHB1RSTR = RCC_AHB1RSTR_GPIOERST;
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	enginePins.errorLedPin.setValue(0);
	enginePins.runningLedPin.setValue(0);
	enginePins.communicationLedPin.setValue(0);
	enginePins.warningLedPin.setValue(0);


	// Do anything the board wants to prepare for stop mode - enabling wakeup sources!
	
	boardPrepareForStop();
	PWR->CR &= ~PWR_CR_PDDS;	// cleared PDDS means stop mode (not standby) 
	PWR->CR |= PWR_CR_FPDS;	// turn off flash in stop mode
	PWR->CR |= PWR_CR_UDEN;	// regulator underdrive in stop mode *
	PWR->CR |= PWR_CR_LPUDS;	// low power regulator in under drive mode
	PWR->CR |= PWR_CR_LPDS;	// regulator in low power mode

	// enable Deepsleep mode
	__disable_irq();
	__WFI();

	// Lastly, reboot
	NVIC_SystemReset();
}

void stm32_standby() {
	SysTick->CTRL = 0;
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	PWR->CR |= PWR_CR_PDDS;	// PDDS = use standby mode (not stop mode)
	PWR->CR |= PWR_CR_CSBF;	// Clear standby flag
	

	// Do anything the board wants to prepare for standby mode - enabling wakeup sources!
	boardPrepareForStandby();

	__disable_irq();
	__WFI();
}
