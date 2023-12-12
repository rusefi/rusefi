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


static Reset_Cause_t readMCUResetCause() {
	uint32_t cause = RCC->CSR; // Read the Control/Status Register
	// Clear reset flags for future reset detection
	RCC->CSR |= RCC_CSR_RMVF;

	if (cause & RCC_CSR_IWDGRSTF) {
		return Reset_Cause_IWatchdog;
    } else if (cause & RCC_CSR_WWDGRSTF) {
        return Reset_Cause_WWatchdog;
    } else if (cause & RCC_CSR_SFTRSTF) {
    	return Reset_Cause_Soft_Reset;
    } else if (cause & RCC_CSR_PINRSTF) {
		return Reset_Cause_NRST_Pin;
	} else if (cause & RCC_CSR_LPWRRSTF) {
		return Reset_Cause_Illegal_Mode;
	} else if (cause & RCC_CSR_BORRSTF) {
		return Reset_Cause_BOR;
	}
    return Reset_Cause_Unknown;
}

// we need to read the reset cause on the early stage, before we setup the rest of MCU hardware
static Reset_Cause_t resetCause = readMCUResetCause();

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

