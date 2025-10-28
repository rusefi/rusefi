/**
 * @file	stm32_common.cpp
 * @brief	Low level common STM32 code
 *
 * @date Mar 28, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include <rusefi/expected.h>
#include "hardware.h"
#include "os_util.h"

#ifdef STM32F4XX
#include "stm32f4xx_hal_flash.h"
#elif defined(STM32F7XX)
#include "stm32f7xx_hal_flash.h"
#elif defined(STM32H7XX)
#include "stm32h7xx_hal_flash.h"
#endif

#if EFI_USE_OPENBLT
/* communication with OpenBLT that is plain C, not to modify external file */
extern "C" {
	#include "openblt/shared_params.h"
};
#endif

#if EFI_PROD_CODE
#include "mpu_util.h"
#include "backup_ram.h"

#ifndef ALLOW_JUMP_WITH_IGNITION_VOLTAGE
// stm32 bootloader might touch uart ports which we cannot allow on boards where uart pins are used to control engine coils etc
#define ALLOW_JUMP_WITH_IGNITION_VOLTAGE TRUE
#endif

static void reset_and_jump(void) {
#if !ALLOW_JUMP_WITH_IGNITION_VOLTAGE
  if (isIgnVoltage()) {
    criticalError("Not allowed with ignition power");
    return;
  }
#endif

	#ifdef STM32H7XX
		// H7 needs a forcible reset of the USB peripheral(s) in order for the bootloader to work properly.
		// If you don't do this, the bootloader will execute, but USB doesn't work (nobody knows why)
		// See https://community.st.com/s/question/0D53W00000vQEWsSAO/stm32h743-dfu-entry-doesnt-work-unless-boot0-held-high-at-poweron
	#ifdef STM32H723xx
		RCC->AHB1ENR &= ~(RCC_AHB1ENR_USB1OTGHSEN);
	#else
		RCC->AHB1ENR &= ~(RCC_AHB1ENR_USB1OTGHSEN | RCC_AHB1ENR_USB2OTGFSEN);
	#endif
	#endif

	// and now reboot
	NVIC_SystemReset();
}

#if EFI_DFU_JUMP
void jump_to_bootloader() {
	// leave DFU breadcrumb which assembly startup code would check, see [rusefi][DFU] section in assembly code

	*((unsigned long *)0x2001FFF0) = 0xDEADBEEF; // End of RAM

	reset_and_jump();
}
#endif

void jump_to_openblt() {
#if EFI_USE_OPENBLT
	/* safe to call on already inited shares area */
	SharedParamsInit();
	/* Store sing to stay in OpenBLT */
	SharedParamsWriteByIndex(0, 0x01);

	reset_and_jump();
#endif
}
#endif /* EFI_PROD_CODE */

#if EFI_PROD_CODE

BOR_Level_t BOR_Get(void) {
	FLASH_OBProgramInitTypeDef FLASH_Handle;

	/* Read option bytes */
	HAL_FLASHEx_OBGetConfig(&FLASH_Handle);

	/* Return BOR value */
	return (BOR_Level_t) FLASH_Handle.BORLevel;
}

BOR_Result_t BOR_Set(BOR_Level_t BORValue) {
	if (BOR_Get() == BORValue) {
		return BOR_Result_Ok;
	}


	FLASH_OBProgramInitTypeDef FLASH_Handle;

	FLASH_Handle.BORLevel = (uint32_t)BORValue;
	FLASH_Handle.OptionType = OPTIONBYTE_BOR;

	HAL_FLASH_OB_Unlock();

	HAL_FLASHEx_OBProgram(&FLASH_Handle);

	HAL_StatusTypeDef status = HAL_FLASH_OB_Launch();

	HAL_FLASH_OB_Lock();

	if (status != HAL_OK) {
		return BOR_Result_Error;
	}

	return BOR_Result_Ok;
}

void startWatchdog(int timeoutMs) {
#if HAL_USE_WDG
	// RL is a 12-bit value so we use a "2 ms" prescaler to support long timeouts (> 4.095 sec)
	static WDGConfig wdgcfg;
	wdgcfg.pr = STM32_IWDG_PR_64;	// t = (1/32768) * 64 = ~2 ms
	wdgcfg.rlr = STM32_IWDG_RL((uint32_t)((32.768f / 64.0f) * timeoutMs));
#if STM32_IWDG_IS_WINDOWED
	wdgcfg.winr = 0xfff; // don't use window
#endif

#ifndef __OPTIMIZE__ // gcc-specific built-in define
	// if no optimizations, then it's most likely a debug version,
	// and we need to enable a special watchdog feature to allow debugging
	efiPrintf("Enabling 'debug freeze' watchdog feature...");
#ifdef STM32H7XX
    DBGMCU->APB4FZ1 |= DBGMCU_APB4FZ1_DBG_IWDG1;
#else // F4 & F7
	DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_IWDG_STOP;
#endif // STM32H7XX
#endif // __OPTIMIZE__

    static bool isStarted = false;
    if (!isStarted) {
		efiPrintf("Starting watchdog with timeout %d ms...", timeoutMs);
		wdgStart(&WDGD1, &wdgcfg);
		isStarted = true;
	} else {
		efiPrintf("Changing watchdog timeout to %d ms...", timeoutMs);
		// wdgStart() uses kernel lock, thus we cannot call it here from locked or ISR code
		wdg_lld_start(&WDGD1);
	}
#endif // HAL_USE_WDG
}

static efitimems_t watchdogResetPeriodMs = 0;
// Reset watchod reset counted in SharedParams after this delay
static const efitimems_t watchdogCounterResetDelay = 3000;

void setWatchdogResetPeriod(int resetMs) {
#if 0
  efiPrintf("[dev] wd %d", resetMs);
#endif
	watchdogResetPeriodMs = (efitimems_t)resetMs;
}

void tryResetWatchdog() {
#if HAL_USE_WDG
	static Timer lastTimeWasReset;
	static efitimems_t wdUptime = 0;
	// check if it's time to reset the watchdog
	if (lastTimeWasReset.hasElapsedMs(watchdogResetPeriodMs)) {
		// we assume tryResetWatchdog() is called from a timer callback
		wdgResetI(&WDGD1);
		lastTimeWasReset.reset();
		// with 100 ms WD
		if (wdUptime < watchdogCounterResetDelay) {
			wdUptime += watchdogResetPeriodMs;
			// we just crossed the treshold
			if (wdUptime >= watchdogCounterResetDelay) {
#if EFI_USE_OPENBLT
				SharedParamsWriteByIndex(1, 0);
#endif
			}
		}
	}
#endif // HAL_USE_WDG
}

uint32_t getMcuSerial() {
	uint32_t *uid = ((uint32_t *)UID_BASE);
	return uid[0] + uid[1] + uid[2];
}

void baseMCUInit() {
	// looks like this holds a random value on start? Let's set a nice clean zero
	DWT->CYCCNT = 0;


#ifndef EFI_SKIP_BOR
	BOR_Set(BOR_Level_1); // one step above default value
#else
  BOR_Set(BOR_Level_None);
#endif

#ifndef EFI_BOOTLOADER
	engine->outputChannels.mcuSerial = getMcuSerial();
#endif // EFI_BOOTLOADER
}

extern uint32_t __main_stack_base__;

typedef struct port_intctx intctx_t;

int getRemainingStack(thread_t *otp) {
#if CH_DBG_ENABLE_STACK_CHECK
	// this would dismiss coverity warning - see http://rusefi.com/forum/viewtopic.php?f=5&t=655
	// coverity[uninit_use]
	register intctx_t *r13 asm ("r13");
	otp->activeStack = r13;

	int remainingStack;
    if (ch0.dbg.isr_cnt > 0) {
		// ISR context
		remainingStack = (int)(r13 - 1) - (int)&__main_stack_base__;
	} else {
		remainingStack = (int)(r13 - 1) - (int)otp->wabase;
	}
	otp->remainingStack = remainingStack;
	return remainingStack;
#else
	UNUSED(otp);
	return 99999;
#endif /* CH_DBG_ENABLE_STACK_CHECK */
}

#if defined(STM32F4)
bool isStm32F42x() {
	// Device identifier
	// 0x419 for STM32F42xxx and STM32F43xxx
	// 0x413 for STM32F405xx/07xx and STM32F415xx/17xx
	return ((DBGMCU->IDCODE & DBGMCU_IDCODE_DEV_ID_Msk) == 0x419);
}
#endif

// Stubs for per-board low power helpers
PUBLIC_API_WEAK void boardPrepareForStop() {
	// Default implementation - wake up on PA0 - boards should override this
	palEnableLineEvent(PAL_LINE(GPIOA, 0), PAL_EVENT_MODE_RISING_EDGE);
}

/**
 Standby uses special low power hardware - it always wakes on rising edge
*/

void boardPreparePA0ForStandby() {
#ifdef STM32F4XX
	//Enable Wakeup Pin for PA0
	PWR->CSR |= PWR_CSR_EWUP;

	// Clear wakeup flag - it may be set if PA0 is already
	// high when we enable it as a wake source
	PWR->CR |= PWR_CR_CWUF; //Clear Wakeup Pin flag for PA0
#endif

#ifdef STM32F7XX
	PWR->CSR2 |= PWR_CSR2_EWUP1; //EWUP1: Enable Wakeup pin for PA0
	PWR->CR2 |= PWR_CR2_CWUPF1; //Clear Wakeup Pin flag for PA0
#endif

#ifdef STM32H7XX
	// Wake on wakeup pin 0 - PA0
	PWR->WKUPEPR = PWR_WKUPEPR_WKUPEN1;

	// clear all possible wakeup bits
	PWR->WKUPCR = 0xFFFFFFFF;
#endif
}

PUBLIC_API_WEAK void boardPrepareForStandby() {
	boardPreparePA0ForStandby();
}

void assertInterruptPriority(const char* func, uint8_t expectedPrio) {
	auto isr = static_cast<uint8_t>(SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) - 16;

	auto actualMask = NVIC->IP[isr];
	auto expectedMask = NVIC_PRIORITY_MASK(expectedPrio);

	if (actualMask != expectedMask) {
		firmwareError(ObdCode::RUNTIME_CRITICAL_WRONG_IRQ_PRIORITY, "bad isr priority at %s expected %02x got %02x", func, expectedMask, actualMask);
	}
}

#endif // EFI_PROD_CODE
