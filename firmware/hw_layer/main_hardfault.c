/**
 * @file main_hardfault.c
 *
 * This black magic improves hard fault debugging experience
 *
 * http://www.chibios.com/forum/viewtopic.php?t=2506
 */

#include "hal.h"

#include <string.h>

/**
 * Executes the BKPT instruction that causes the debugger to stop.
 * If no debugger is attached, this will be ignored
 */
#define bkpt() __asm volatile("BKPT #0\n")

void NMI_Handler(void) {
	NVIC_SystemReset();
}

//See http://infocenter.arm.com/help/topic/com.arm.doc.dui0552a/BABBGBEC.html
typedef enum  {
	Reset = 1,
	NMI = 2,
	HardFault = 3,
	MemManage = 4,
	BusFault = 5,
	UsageFault = 6,
} FaultType;

void logHardFault(uint32_t type, uintptr_t faultAddress, struct port_extctx* ctx, uint32_t csfr);

void HardFault_Handler_C(void* sp) {
	//Copy to local variables (not pointers) to allow GDB "i loc" to directly show the info
	//Get thread context. Contains main registers including PC and LR
	struct port_extctx ctx;
	memcpy(&ctx, sp, sizeof(struct port_extctx));

	//Interrupt status register: Which interrupt have we encountered, e.g. HardFault?
	volatile FaultType faultType = (FaultType)__get_IPSR();
	(void)faultType;
	//For HardFault/BusFault this is the address that was accessed causing the error
	volatile uint32_t faultAddress = SCB->BFAR;

	//Flags about hardfault / busfault
	//See http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0552a/Cihdjcfc.html for reference
	volatile bool isFaultPrecise = ((SCB->CFSR >> SCB_CFSR_BUSFAULTSR_Pos) & (1 << 1) ? true : false);
	volatile bool isFaultImprecise = ((SCB->CFSR >> SCB_CFSR_BUSFAULTSR_Pos) & (1 << 2) ? true : false);
	volatile bool isFaultOnUnstacking = ((SCB->CFSR >> SCB_CFSR_BUSFAULTSR_Pos) & (1 << 3) ? true : false);
	volatile bool isFaultOnStacking = ((SCB->CFSR >> SCB_CFSR_BUSFAULTSR_Pos) & (1 << 4) ? true : false);
	volatile bool isFaultAddressValid = ((SCB->CFSR >> SCB_CFSR_BUSFAULTSR_Pos) & (1 << 7) ? true : false);
	(void)isFaultPrecise;
	(void)isFaultImprecise;
	(void)isFaultOnUnstacking;
	(void)isFaultOnStacking;
	(void)isFaultAddressValid;

	logHardFault(faultType, faultAddress, &ctx, SCB->CFSR >> SCB_CFSR_BUSFAULTSR_Pos);

	//Cause debugger to stop. Ignored if no debugger is attached
	bkpt();
	NVIC_SystemReset();
}

void UsageFault_Handler_C(void* sp) {
	//Copy to local variables (not pointers) to allow GDB "i loc" to directly show the info
	//Get thread context. Contains main registers including PC and LR
	struct port_extctx ctx;
	memcpy(&ctx, sp, sizeof(struct port_extctx));

	//Interrupt status register: Which interrupt have we encountered, e.g. HardFault?
	volatile FaultType faultType = (FaultType)__get_IPSR();
	(void)faultType;
	//Flags about hardfault / busfault
	//See http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0552a/Cihdjcfc.html for reference
	volatile bool isUndefinedInstructionFault = ((SCB->CFSR >> SCB_CFSR_USGFAULTSR_Pos) & (1 << 0) ? true : false);
	volatile bool isEPSRUsageFault = ((SCB->CFSR >> SCB_CFSR_USGFAULTSR_Pos) & (1 << 1) ? true : false);
	volatile bool isInvalidPCFault = ((SCB->CFSR >> SCB_CFSR_USGFAULTSR_Pos) & (1 << 2) ? true : false);
	volatile bool isNoCoprocessorFault = ((SCB->CFSR >> SCB_CFSR_USGFAULTSR_Pos) & (1 << 3) ? true : false);
	volatile bool isUnalignedAccessFault = ((SCB->CFSR >> SCB_CFSR_USGFAULTSR_Pos) & (1 << 8) ? true : false);
	volatile bool isDivideByZeroFault = ((SCB->CFSR >> SCB_CFSR_USGFAULTSR_Pos) & (1 << 9) ? true : false);
	(void)isUndefinedInstructionFault;
	(void)isEPSRUsageFault;
	(void)isInvalidPCFault;
	(void)isNoCoprocessorFault;
	(void)isUnalignedAccessFault;
	(void)isDivideByZeroFault;

	logHardFault(faultType, 0, &ctx, SCB->CFSR);

	bkpt();
	NVIC_SystemReset();
}

void MemManage_Handler_C(void* sp) {
	//Copy to local variables (not pointers) to allow GDB "i loc" to directly show the info
	//Get thread context. Contains main registers including PC and LR
	struct port_extctx ctx;
	memcpy(&ctx, sp, sizeof(struct port_extctx));

	//Interrupt status register: Which interrupt have we encountered, e.g. HardFault?
	FaultType faultType = (FaultType)__get_IPSR();
	(void)faultType;
	//For HardFault/BusFault this is the address that was accessed causing the error
	volatile uint32_t faultAddress = SCB->MMFAR;

	//Flags about hardfault / busfault
	//See http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0552a/Cihdjcfc.html for reference
	volatile bool isInstructionAccessViolation = ((SCB->CFSR >> SCB_CFSR_MEMFAULTSR_Pos) & (1 << 0) ? true : false);
	volatile bool isDataAccessViolation = ((SCB->CFSR >> SCB_CFSR_MEMFAULTSR_Pos) & (1 << 1) ? true : false);
	volatile bool isExceptionUnstackingFault = ((SCB->CFSR >> SCB_CFSR_MEMFAULTSR_Pos) & (1 << 3) ? true : false);
	volatile bool isExceptionStackingFault = ((SCB->CFSR >> SCB_CFSR_MEMFAULTSR_Pos) & (1 << 4) ? true : false);
	volatile bool isFaultAddressValid = ((SCB->CFSR >> SCB_CFSR_MEMFAULTSR_Pos) & (1 << 7) ? true : false);
	(void)isInstructionAccessViolation;
	(void)isDataAccessViolation;
	(void)isExceptionUnstackingFault;
	(void)isExceptionStackingFault;
	(void)isFaultAddressValid;

	logHardFault(faultType, faultAddress, &ctx, SCB->CFSR);

	bkpt();
	NVIC_SystemReset();
}
