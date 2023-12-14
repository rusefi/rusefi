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
#endif /* EFI_PROD_CODE */

#if EFI_PROD_CODE

static void reset_and_jump(void) {
	#ifdef STM32H7XX
		// H7 needs a forcible reset of the USB peripheral(s) in order for the bootloader to work properly.
		// If you don't do this, the bootloader will execute, but USB doesn't work (nobody knows why)
		// See https://community.st.com/s/question/0D53W00000vQEWsSAO/stm32h743-dfu-entry-doesnt-work-unless-boot0-held-high-at-poweron
		RCC->AHB1ENR &= ~(RCC_AHB1ENR_USB1OTGHSEN | RCC_AHB1ENR_USB2OTGFSEN);
	#endif

	// and now reboot
	NVIC_SystemReset();
}

void jump_to_bootloader() {
	// leave DFU breadcrumb which assembly startup code would check, see [rusefi][DFU] section in assembly code

	*((unsigned long *)0x2001FFF0) = 0xDEADBEEF; // End of RAM

	reset_and_jump();
}

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
// ? efiPrintf("[wdgStart]");
	wdgStart(&WDGD1, &wdgcfg);
#endif // HAL_USE_WDG
}

static efitimems_t watchdogResetPeriodMs = 0;

void setWatchdogResetPeriod(int resetMs) {
  efiPrintf("[dev] wd %d", resetMs);
	watchdogResetPeriodMs = (efitimems_t)resetMs;
}

void tryResetWatchdog() {
#if HAL_USE_WDG
	static Timer lastTimeWasReset;
	// check if it's time to reset the watchdog
	if (lastTimeWasReset.hasElapsedMs(watchdogResetPeriodMs)) {
		// we assume tryResetWatchdog() is called from a timer callback
		wdgResetI(&WDGD1);
		lastTimeWasReset.reset();
	}
#endif // HAL_USE_WDG
}

void baseMCUInit(void) {
	// looks like this holds a random value on start? Let's set a nice clean zero
	DWT->CYCCNT = 0;

	BOR_Set(BOR_Level_1); // one step above default value

	setWatchdogResetPeriod(WATCHDOG_RESET_MS);
	startWatchdog();
}

extern uint32_t __main_stack_base__;

typedef struct port_intctx intctx_t;

EXTERNC int getRemainingStack(thread_t *otp) {
#if CH_DBG_ENABLE_STACK_CHECK
	// this would dismiss coverity warning - see http://rusefi.com/forum/viewtopic.php?f=5&t=655
	// coverity[uninit_use]
	register intctx_t *r13 asm ("r13");
	otp->activeStack = r13;

	int remainingStack;
    if (ch.dbg.isr_cnt > 0) {
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

#if defined(STM32F4) || defined(STM32F7) || defined(STM32H7)

#define HWREG(x)                                                              \
        (*((volatile unsigned long *)(x)))

#define NVIC_FAULT_STAT         0xE000ED28  // Configurable Fault Status
#define NVIC_FAULT_STAT_BFARV   0x00008000  // Bus Fault Address Register Valid
#define NVIC_CFG_CTRL_BFHFNMIGN 0x00000100  // Ignore Bus Fault in NMI and
                                            // Fault
#define NVIC_CFG_CTRL           0xE000ED14  // Configuration and Control


/**
 * @brief Probe an address to see if can be read without generating a bus fault
 * @details This function must be called with the processor in privileged mode.
 *          It:
 *          - Clear any previous indication of a bus fault in the BFARV bit
 *          - Temporarily sets the processor to Ignore Bus Faults with all interrupts and fault handlers disabled
 *          - Attempt to read from read_address, ignoring the result
 *          - Checks to see if the read caused a bus fault, by checking the BFARV bit is set
 *          - Re-enables Bus Faults and all interrupts and fault handlers
 * @param[in] read_address The address to try reading a byte from
 * @return Returns true if no bus fault occurred reading from read_address, or false if a bus fault occurred.
 */
bool ramReadProbe(volatile const char *read_address) {
    bool address_readable = true;

    /* Clear any existing indication of a bus fault - BFARV is write one to clear */
    HWREG (NVIC_FAULT_STAT) |= NVIC_FAULT_STAT_BFARV;

    HWREG (NVIC_CFG_CTRL) |= NVIC_CFG_CTRL_BFHFNMIGN;
    asm volatile ("  CPSID f;");
    *read_address;
    if ((HWREG (NVIC_FAULT_STAT) & NVIC_FAULT_STAT_BFARV) != 0)
    {
        address_readable = false;
    }
    asm volatile ("  CPSIE f;");
    HWREG (NVIC_CFG_CTRL) &= ~NVIC_CFG_CTRL_BFHFNMIGN;

    return address_readable;
}

#endif

#if defined(STM32F4)
bool isStm32F42x() {
	// really it's enough to just check 0x20020010
	return ramReadProbe((const char *)0x20000010) && ramReadProbe((const char *)0x20020010) && !ramReadProbe((const char *)0x20070010);
}

#endif


// Stubs for per-board low power helpers
__attribute__((weak)) void boardPrepareForStop() {
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

__attribute__((weak)) void boardPrepareForStandby() {
	boardPreparePA0ForStandby();
}

#endif // EFI_PROD_CODE
