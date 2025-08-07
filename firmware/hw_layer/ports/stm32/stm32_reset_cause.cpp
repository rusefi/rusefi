/**
 * @file	stm32_reset_cause.cpp
 * @brief	Get Reset Cause for STM32 MCUs
 *
 * @date Dec 10, 2023
 * @author Andrey Belomutskiy, (c) 2012-2023
 * @author  andreika <prometheus.pcb@gmail.com>
 */

#include "pch.h"

#if EFI_PROD_CODE
#include "mpu_util.h"
#endif  /* EFI_PROD_CODE */

#ifdef STM32H7XX
// Reset Status Register flags for H7
#define BORRSTF RCC_RSR_BORRSTF
#define PINRSTF RCC_RSR_PINRSTF
#define PORRSTF RCC_RSR_PORRSTF
#define SFTRSTF RCC_RSR_SFTRSTF
#define IWDGRSTF RCC_RSR_IWDG1RSTF
#define WWDGRSTF RCC_RSR_WWDG1RSTF
#define LPWRRSTF RCC_RSR_LPWRRSTF
#else
// Control/Status Register flags for F4/F7
#define BORRSTF RCC_CSR_BORRSTF
#define PINRSTF RCC_CSR_PINRSTF
#define PORRSTF RCC_CSR_PORRSTF
#define SFTRSTF RCC_CSR_SFTRSTF
#define IWDGRSTF RCC_CSR_IWDGRSTF
#define WWDGRSTF RCC_CSR_WWDGRSTF
#define LPWRRSTF RCC_CSR_LPWRRSTF
#endif // STM32H7XX

static Reset_Cause_t readMCUResetCause() {
#ifdef STM32H7XX
	uint32_t cause = RCC->RSR; // Read the Reset Status Register
	// keep reset reason visible to main app
#ifndef EFI_BOOTLOADER
	// Clear reset flags for future reset detection
	RCC->RSR |= RCC_RSR_RMVF;
#endif
#else
	uint32_t cause = RCC->CSR; // Read the Control/Status Register
	// keep reset reason visible to main app
#ifndef EFI_BOOTLOADER
	// Clear reset flags for future reset detection
	RCC->CSR |= RCC_CSR_RMVF;
#endif
#endif

	if (cause & BORRSTF) {
		return Reset_Cause_BOR;
	} else if (cause & PORRSTF) {
		return Reset_Cause_POR;
	} else if (cause & SFTRSTF) {
		return Reset_Cause_Soft_Reset;
	} else if (cause & IWDGRSTF) {
		return Reset_Cause_IWatchdog;
    } else if (cause & WWDGRSTF) {
        return Reset_Cause_WWatchdog;
	} else if (cause & LPWRRSTF) {
		return Reset_Cause_Illegal_Mode;
	} else if (cause & PINRSTF) {
		// See STM32 datasheet "Simplified diagram of the reset circuit"
		// All internal resets happens through driving NRST pin low
		// This cause rise of PINRSTF bit in CSR register
		return Reset_Cause_NRST_Pin;
	}
    return Reset_Cause_Unknown;
}

// we need to read the reset cause on the early stage, before we setup the rest of MCU hardware
static const volatile Reset_Cause_t resetCause = readMCUResetCause();

Reset_Cause_t getMCUResetCause() {
	return resetCause;
}

const char *getMCUResetCause(Reset_Cause_t cause) {
	switch (cause) {
	case Reset_Cause_IWatchdog:
		return "Independent hardware watchdog";
	case Reset_Cause_WWatchdog:
		return "Window watchdog";
	case Reset_Cause_Soft_Reset:
		return "NVIC_SystemReset or by debugger";
	case Reset_Cause_NRST_Pin:
		return "Reset from NRST pin";
	case Reset_Cause_POR:
		return "Power on/power-down reset";
	case Reset_Cause_Illegal_Mode:
		return "Reset after illegal Stop, Standby or Shutdown mode entry";
	case Reset_Cause_BOR:
		return "BOR reset";
	case Reset_Cause_Firewall:
		return "Firewall reset";
	case Reset_Cause_Option_Byte:
		return "Option byte load reset";
	default:
		return "Unknown";
	}
}

