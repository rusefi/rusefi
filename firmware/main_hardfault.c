/**
 * @file main_hardfault.c
 *
 * This black magic improves hard fault debugging experience
 *
 * http://www.chibios.com/forum/viewtopic.php?t=2506
 */

#include "global.h"

/**
 * Executes the BKPT instruction that causes the debugger to stop.
 * If no debugger is attached, this will be ignored
 */
#define bkpt() __asm volatile("BKPT #0\n")

void NMI_Handler(void) {
    //TODO
    while(1);
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

void HardFault_Handler(void) {
    //Copy to local variables (not pointers) to allow GDB "i loc" to directly show the info
    //Get thread context. Contains main registers including PC and LR
    struct port_extctx ctx;
    memcpy(&ctx, (void*)__get_PSP(), sizeof(struct port_extctx));
    (void)ctx;
    //Interrupt status register: Which interrupt have we encountered, e.g. HardFault?
    FaultType faultType = (FaultType)__get_IPSR();
    (void)faultType;
    //For HardFault/BusFault this is the address that was accessed causing the error
    uint32_t faultAddress = SCB->BFAR;
    (void)faultAddress;
    //Flags about hardfault / busfault
    //See http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0552a/Cihdjcfc.html for reference
    bool isFaultPrecise = ((SCB->CFSR >> SCB_CFSR_BUSFAULTSR_Pos) & (1 << 1) ? true : false);
    bool isFaultImprecise = ((SCB->CFSR >> SCB_CFSR_BUSFAULTSR_Pos) & (1 << 2) ? true : false);
    bool isFaultOnUnstacking = ((SCB->CFSR >> SCB_CFSR_BUSFAULTSR_Pos) & (1 << 3) ? true : false);
    bool isFaultOnStacking = ((SCB->CFSR >> SCB_CFSR_BUSFAULTSR_Pos) & (1 << 4) ? true : false);
    bool isFaultAddressValid = ((SCB->CFSR >> SCB_CFSR_BUSFAULTSR_Pos) & (1 << 7) ? true : false);
    (void)isFaultPrecise;
    (void)isFaultImprecise;
    (void)isFaultOnUnstacking;
    (void)isFaultOnStacking;
    (void)isFaultAddressValid;
    //Cause debugger to stop. Ignored if no debugger is attached
    bkpt();
    NVIC_SystemReset();
}

void BusFault_Handler(void) __attribute__((alias("HardFault_Handler")));

void UsageFault_Handler(void) {
    //Copy to local variables (not pointers) to allow GDB "i loc" to directly show the info
    //Get thread context. Contains main registers including PC and LR
    struct port_extctx ctx;
    memcpy(&ctx, (void*)__get_PSP(), sizeof(struct port_extctx));
    (void)ctx;
    //Interrupt status register: Which interrupt have we encountered, e.g. HardFault?
    FaultType faultType = (FaultType)__get_IPSR();
    (void)faultType;
    //Flags about hardfault / busfault
    //See http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0552a/Cihdjcfc.html for reference
    bool isUndefinedInstructionFault = ((SCB->CFSR >> SCB_CFSR_USGFAULTSR_Pos) & (1 << 0) ? true : false);
    bool isEPSRUsageFault = ((SCB->CFSR >> SCB_CFSR_USGFAULTSR_Pos) & (1 << 1) ? true : false);
    bool isInvalidPCFault = ((SCB->CFSR >> SCB_CFSR_USGFAULTSR_Pos) & (1 << 2) ? true : false);
    bool isNoCoprocessorFault = ((SCB->CFSR >> SCB_CFSR_USGFAULTSR_Pos) & (1 << 3) ? true : false);
    bool isUnalignedAccessFault = ((SCB->CFSR >> SCB_CFSR_USGFAULTSR_Pos) & (1 << 8) ? true : false);
    bool isDivideByZeroFault = ((SCB->CFSR >> SCB_CFSR_USGFAULTSR_Pos) & (1 << 9) ? true : false);
    (void)isUndefinedInstructionFault;
    (void)isEPSRUsageFault;
    (void)isInvalidPCFault;
    (void)isNoCoprocessorFault;
    (void)isUnalignedAccessFault;
    (void)isDivideByZeroFault;
    bkpt();
    NVIC_SystemReset();
}

void MemManage_Handler(void) {
    //Copy to local variables (not pointers) to allow GDB "i loc" to directly show the info
    //Get thread context. Contains main registers including PC and LR
    struct port_extctx ctx;
    memcpy(&ctx, (void*)__get_PSP(), sizeof(struct port_extctx));
    (void)ctx;
    //Interrupt status register: Which interrupt have we encountered, e.g. HardFault?
    FaultType faultType = (FaultType)__get_IPSR();
    (void)faultType;
    //For HardFault/BusFault this is the address that was accessed causing the error
    uint32_t faultAddress = SCB->MMFAR;
    (void)faultAddress;
    //Flags about hardfault / busfault
    //See http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0552a/Cihdjcfc.html for reference
    bool isInstructionAccessViolation = ((SCB->CFSR >> SCB_CFSR_MEMFAULTSR_Pos) & (1 << 0) ? true : false);
    bool isDataAccessViolation = ((SCB->CFSR >> SCB_CFSR_MEMFAULTSR_Pos) & (1 << 1) ? true : false);
    bool isExceptionUnstackingFault = ((SCB->CFSR >> SCB_CFSR_MEMFAULTSR_Pos) & (1 << 3) ? true : false);
    bool isExceptionStackingFault = ((SCB->CFSR >> SCB_CFSR_MEMFAULTSR_Pos) & (1 << 4) ? true : false);
    bool isFaultAddressValid = ((SCB->CFSR >> SCB_CFSR_MEMFAULTSR_Pos) & (1 << 7) ? true : false);
    (void)isInstructionAccessViolation;
    (void)isDataAccessViolation;
    (void)isExceptionUnstackingFault;
    (void)isExceptionStackingFault;
    (void)isFaultAddressValid;
    bkpt();
    NVIC_SystemReset();
}
