///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:59 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\stm32f /
//                       4\mpu_util.cpp                                       /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\stm32f /
//                       4\mpu_util.cpp -lCN F:\stuff\rusefi_sourceforge\firm /
//                       ware\iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge /
//                       \firmware\iar\Debug\List\ -o                         /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\Debug\Obj\  /
//                       --no_cse --no_unroll --no_inline --no_code_motion    /
//                       --no_tbaa --no_clustering --no_scheduling --debug    /
//                       --endian=little --cpu=Cortex-M4 -e --fpu=VFPv4_sp    /
//                       --dlib_config "C:\Program Files (x86)\IAR            /
//                       Systems\Embedded Workbench                           /
//                       6.5\arm\INC\c\DLib_Config_Normal.h"                  /
//                       --misrac2004=1.2-1.4,2.3,3.4,4.2,6.5-7,8.2-8.3,9.3,  /
//                       12.3,13.6-13,15.5,17.3,19.17,20.7-20.9,20.11 -I      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\ -I      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\kernel\include\ -I F:\stuff\rusefi_sourceforge\fi /
//                       rmware\iar\..\chibios\os\ports\common\ARMCMx\ -I     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\ports\common\ARMCMx\CMSIS\include\ -I             /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\ports\IAR\ARMCMx\ -I F:\stuff\rusefi_sourceforge\ /
//                       firmware\iar\..\chibios\os\ports\IAR\ARMCMx\STM32F4x /
//                       x\ -I F:\stuff\rusefi_sourceforge\firmware\iar\..\ch /
//                       ibios\os\hal\include\ -I F:\stuff\rusefi_sourceforge /
//                       \firmware\iar\..\chibios\os\hal\platforms\STM32\ -I  /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\DMAv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\GPIOv2\ -I                    /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\I2Cv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\SPIv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\TIMv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\RTCv2\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\USARTv1\ -I                   /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\OTGv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32F4xx\ -I                       /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\various\ -I F:\stuff\rusefi_sourceforge\firmware\ /
//                       iar\..\chibios\os\various\devices_lib\accel\ -I      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       boards\ST_STM32F4_DISCOVERY\ -I                      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\controll /
//                       ers\ -I F:\stuff\rusefi_sourceforge\firmware\iar\..\ /
//                       controllers\algo\ -I F:\stuff\rusefi_sourceforge\fir /
//                       mware\iar\..\controllers\core\ -I                    /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\controll /
//                       ers\system\ -I F:\stuff\rusefi_sourceforge\firmware\ /
//                       iar\..\controllers\sensors\ -I                       /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\controll /
//                       ers\math\ -I F:\stuff\rusefi_sourceforge\firmware\ia /
//                       r\..\controllers\trigger\ -I                         /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\console\ /
//                        -I F:\stuff\rusefi_sourceforge\firmware\iar\..\conf /
//                       ig\ -I F:\stuff\rusefi_sourceforge\firmware\iar\..\c /
//                       onfig\engines\ -I F:\stuff\rusefi_sourceforge\firmwa /
//                       re\iar\..\config\stm32f4ems\ -I                      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\console\ /
//                       binary\ -I F:\stuff\rusefi_sourceforge\firmware\iar\ /
//                       ..\console_util\ -I F:\stuff\rusefi_sourceforge\firm /
//                       ware\iar\..\development\ -I                          /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\developm /
//                       ent\test\ -I F:\stuff\rusefi_sourceforge\firmware\ia /
//                       r\..\development\hw_layer\ -I                        /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\hw_layer /
//                       \algo\ -I F:\stuff\rusefi_sourceforge\firmware\iar\. /
//                       .\hw_layer\lcd\ -I F:\stuff\rusefi_sourceforge\firmw /
//                       are\iar\..\hw_layer\stm32f4\ -I                      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\hw_layer /
//                       \serial_over_usb\ -I F:\stuff\rusefi_sourceforge\fir /
//                       mware\iar\..\hw_layer\ -I                            /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\ext\ -I  /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\ext_algo /
//                       \ -I F:\stuff\rusefi_sourceforge\firmware\iar\..\dev /
//                       elopment\ -I F:\stuff\rusefi_sourceforge\firmware\ia /
//                       r\..\development\hw_layer\ -I                        /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\util\    /
//                       -On --eec++                                          /
//    MISRA C version =  2004                                                 /
//      Enabled       =  1.2-1.4 2.3 3.4 4.2 6.5 7 8.2 8.3 9.3 12.3 13.6      /
//                       13.7 15.5 17.3 19.17 20.7-20.9 20.11                 /
//      Checked       =  1.4 2.3 4.2 6.5 7 8.2 8.3 9.3 12.3 13.7 15.5         /
//                       20.7-20.9 20.11                                      /
//      Not checked   =  1.1-1.3 1.5-2.2 2.4-4.1 5-6.4 8.1 8.4-9.2 10-12.2    /
//                       12.4-13.6 14-15.4 16-20.6 20.10 20.12 21             /
//    List file       =  F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                       mpu_util.s                                           /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME mpu_util

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN `IRQSTACK$$Base`
        EXTERN SPID1
        EXTERN SPID2
        EXTERN SPID3
        EXTERN _Z12mySetPadModePKcP12GPIO_TypeDefjj
        EXTERN _Z13mySetPadMode2PKc11brain_pin_ej
        EXTERN _Z8getHwPin11brain_pin_e
        EXTERN _Z9getHwPort11brain_pin_e
        EXTERN boardConfiguration
        EXTERN chDbgPanic3
        EXTERN dbg_isr_cnt

        PUBLIC BusFaultVector
        PUBLIC DebugMonitorVector
        PUBLIC HardFaultVector
        PUBLIC UsageFaultVector
        PUBLIC _Z16baseHardwareInitv
        PUBLIC _Z9turnOnSpi12spi_device_e
        PUBLIC getRemainingStack
        PUBLIC initSpiCs
        PUBLIC initSpiModule
        PUBLIC postmortem_AFSR
        PUBLIC postmortem_BFAR
        PUBLIC postmortem_CFSR
        PUBLIC postmortem_DFSR
        PUBLIC postmortem_HFSR
        PUBLIC postmortem_MMAR
        PUBLIC postmortem_SCB_SHCSR
        PUBLIC postmortem_lr
        PUBLIC postmortem_pc
        PUBLIC postmortem_psr
        PUBLIC postmortem_r0
        PUBLIC postmortem_r1
        PUBLIC postmortem_r12
        PUBLIC postmortem_r2
        PUBLIC postmortem_r3
        PUBLIC prvGetRegistersFromStack
        
          CFI Names cfiNames0
          CFI StackFrame CFA R13 DATA
          CFI Resource R0:32, R1:32, R2:32, R3:32, R4:32, R5:32, R6:32, R7:32
          CFI Resource R8:32, R9:32, R10:32, R11:32, R12:32, R13:32, R14:32
          CFI Resource D0:64, D1:64, D2:64, D3:64, D4:64, D5:64, D6:64, D7:64
          CFI Resource D8:64, D9:64, D10:64, D11:64, D12:64, D13:64, D14:64
          CFI Resource D15:64
          CFI EndNames cfiNames0
        
          CFI Common cfiCommon0 Using cfiNames0
          CFI CodeAlign 2
          CFI DataAlign 4
          CFI ReturnAddress R14 CODE
          CFI CFA R13+0
          CFI R0 Undefined
          CFI R1 Undefined
          CFI R2 Undefined
          CFI R3 Undefined
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R6 SameValue
          CFI R7 SameValue
          CFI R8 SameValue
          CFI R9 SameValue
          CFI R10 SameValue
          CFI R11 SameValue
          CFI R12 Undefined
          CFI R14 SameValue
          CFI D0 Undefined
          CFI D1 Undefined
          CFI D2 Undefined
          CFI D3 Undefined
          CFI D4 Undefined
          CFI D5 Undefined
          CFI D6 Undefined
          CFI D7 Undefined
          CFI D8 SameValue
          CFI D9 SameValue
          CFI D10 SameValue
          CFI D11 SameValue
          CFI D12 SameValue
          CFI D13 SameValue
          CFI D14 SameValue
          CFI D15 SameValue
          CFI EndCommon cfiCommon0
        
// F:\stuff\rusefi_sourceforge\firmware\hw_layer\stm32f4\mpu_util.cpp
//    1 /**
//    2  * @file	mpu_util.cpp
//    3  *
//    4  * @date Jul 27, 2014
//    5  * @author Andrey Belomutskiy, (c) 2012-2015
//    6  */
//    7 
//    8 #include "main.h"
//    9 #include "mpu_util.h"
//   10 #include "error_handling.h"
//   11 #include "engine.h"
//   12 #include "pin_repository.h"
//   13 
//   14 EXTERN_ENGINE;
//   15 
//   16 extern "C" {
//   17 int getRemainingStack(Thread *otp);
//   18 void prvGetRegistersFromStack(uint32_t *pulFaultStackAddress);
//   19 }
//   20 
//   21 extern uint32_t __main_stack_base__;
//   22 
//   23 #define GET_CFSR() (*((volatile uint32_t *) (0xE000ED28)))
//   24 
//   25 #if defined __GNUC__
//   26 // GCC version
//   27 
//   28 int getRemainingStack(Thread *otp) {
//   29 
//   30 #if CH_DBG_ENABLE_STACK_CHECK
//   31 	register struct intctx *r13 asm ("r13");
//   32 	otp->activeStack = r13;
//   33 
//   34 	int remainingStack;
//   35 	if (dbg_isr_cnt > 0) {
//   36 		// ISR context
//   37 		remainingStack = (int)(r13 - 1) - (int)&__main_stack_base__;
//   38 	} else {
//   39 		remainingStack = (int)(r13 - 1) - (int)otp->p_stklimit;
//   40 	}
//   41 	otp->remainingStack = remainingStack;
//   42 	return remainingStack;
//   43 #else
//   44 	return 99999;
//   45 #endif /* CH_DBG_ENABLE_STACK_CHECK */
//   46 }
//   47 
//   48 #else /* __GNUC__ */
//   49 
//   50 extern uint32_t CSTACK$$Base; /* symbol created by the IAR linker */
//   51 extern uint32_t IRQSTACK$$Base; /* symbol created by the IAR linker */
//   52 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function getRemainingStack
          CFI NoCalls
        THUMB
//   53 int getRemainingStack(Thread *otp) {
getRemainingStack:
        MOVS     R1,R0
//   54 #if CH_DBG_ENABLE_STACK_CHECK || defined(__DOXYGEN__)
//   55 	int remainingStack;
//   56 	if (dbg_isr_cnt > 0) {
        LDR.N    R2,??DataTable10
        LDR      R2,[R2, #+0]
        CMP      R2,#+1
        BLT.N    ??getRemainingStack_0
//   57 		remainingStack = (__get_SP() - sizeof(struct intctx)) - (int)&IRQSTACK$$Base;
        MOV      R2,SP
        SUBS     R2,R2,#+100
        LDR.N    R3,??DataTable10_1
        SUBS     R2,R2,R3
        MOVS     R0,R2
        B.N      ??getRemainingStack_1
//   58 	} else {
//   59 		remainingStack = (__get_SP() - sizeof(struct intctx)) - (int)otp->p_stklimit;
??getRemainingStack_0:
        MOV      R2,SP
        SUBS     R2,R2,#+100
        LDR      R3,[R1, #+28]
        SUBS     R2,R2,R3
        MOVS     R0,R2
//   60 	}
//   61 	otp->remainingStack = remainingStack;
??getRemainingStack_1:
        STR      R0,[R1, #+72]
//   62 	return remainingStack;
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//   63 #else
//   64 	return 999999;
//   65 #endif  
//   66 }
//   67 
//   68 // IAR version
//   69 
//   70 #endif
//   71 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z16baseHardwareInitv
          CFI NoCalls
        THUMB
//   72 void baseHardwareInit(void) {
//   73 	// looks like this holds a random value on start? Let's set a nice clean zero
//   74 	DWT_CYCCNT = 0;
_Z16baseHardwareInitv:
        LDR.N    R0,??DataTable10_2  ;; 0xe0001004
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//   75 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   76 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function DebugMonitorVector
        THUMB
//   77 void DebugMonitorVector(void) {
DebugMonitorVector:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   78 
//   79 	chDbgPanic3("DebugMonitorVector", __FILE__, __LINE__);
        MOVS     R2,#+79
        LDR.N    R1,??DataTable10_3
        LDR.N    R0,??DataTable10_4
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//   80 
//   81 	while (TRUE)
??DebugMonitorVector_0:
        B.N      ??DebugMonitorVector_0
          CFI EndBlock cfiBlock2
//   82 		;
//   83 }
//   84 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function UsageFaultVector
        THUMB
//   85 void UsageFaultVector(void) {
UsageFaultVector:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   86 
//   87 	chDbgPanic3("UsageFaultVector", __FILE__, __LINE__);
        MOVS     R2,#+87
        LDR.N    R1,??DataTable10_3
        LDR.N    R0,??DataTable10_5
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//   88 
//   89 	while (TRUE)
??UsageFaultVector_0:
        B.N      ??UsageFaultVector_0
          CFI EndBlock cfiBlock3
//   90 		;
//   91 }
//   92 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function BusFaultVector
        THUMB
//   93 void BusFaultVector(void) {
BusFaultVector:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   94 
//   95 	chDbgPanic3("BusFaultVector", __FILE__, __LINE__);
        MOVS     R2,#+95
        LDR.N    R1,??DataTable10_3
        LDR.N    R0,??DataTable10_6
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//   96 
//   97 	while (TRUE) {
??BusFaultVector_0:
        B.N      ??BusFaultVector_0
          CFI EndBlock cfiBlock4
//   98 	}
//   99 }
//  100 
//  101 /**
//  102  + * @brief   Register values for postmortem debugging.
//  103  + */

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  104 volatile uint32_t postmortem_r0;
postmortem_r0:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  105 volatile uint32_t postmortem_r1;
postmortem_r1:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  106 volatile uint32_t postmortem_r2;
postmortem_r2:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  107 volatile uint32_t postmortem_r3;
postmortem_r3:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  108 volatile uint32_t postmortem_r12;
postmortem_r12:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  109 volatile uint32_t postmortem_lr; /* Link register. */
postmortem_lr:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  110 volatile uint32_t postmortem_pc; /* Program counter. */
postmortem_pc:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  111 volatile uint32_t postmortem_psr;/* Program status register. */
postmortem_psr:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  112 volatile uint32_t postmortem_CFSR;
postmortem_CFSR:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  113 volatile uint32_t postmortem_HFSR;
postmortem_HFSR:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  114 volatile uint32_t postmortem_DFSR;
postmortem_DFSR:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  115 volatile uint32_t postmortem_AFSR;
postmortem_AFSR:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  116 volatile uint32_t postmortem_BFAR;
postmortem_BFAR:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  117 volatile uint32_t postmortem_MMAR;
postmortem_MMAR:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  118 volatile uint32_t postmortem_SCB_SHCSR;
postmortem_SCB_SHCSR:
        DS8 4
//  119 
//  120 /**
//  121  * @brief   Evaluates to TRUE if system runs under debugger control.
//  122  * @note    This bit resets only by power reset.
//  123  */
//  124 #define is_under_debugger() (((CoreDebug)->DHCSR) & \ 
//  125                             CoreDebug_DHCSR_C_DEBUGEN_Msk)
//  126 
//  127 /**
//  128  *
//  129  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function prvGetRegistersFromStack
          CFI NoCalls
        THUMB
//  130 void prvGetRegistersFromStack(uint32_t *pulFaultStackAddress) {
//  131 
//  132 	postmortem_r0 = pulFaultStackAddress[0];
prvGetRegistersFromStack:
        LDR.N    R1,??DataTable10_7
        LDR      R2,[R0, #+0]
        STR      R2,[R1, #+0]
//  133 	postmortem_r1 = pulFaultStackAddress[1];
        LDR      R1,[R0, #+4]
        LDR.N    R2,??DataTable10_8
        STR      R1,[R2, #+0]
//  134 	postmortem_r2 = pulFaultStackAddress[2];
        LDR      R1,[R0, #+8]
        LDR.N    R2,??DataTable10_9
        STR      R1,[R2, #+0]
//  135 	postmortem_r3 = pulFaultStackAddress[3];
        LDR      R1,[R0, #+12]
        LDR.N    R2,??DataTable10_10
        STR      R1,[R2, #+0]
//  136 	postmortem_r12 = pulFaultStackAddress[4];
        LDR      R1,[R0, #+16]
        LDR.N    R2,??DataTable10_11
        STR      R1,[R2, #+0]
//  137 	postmortem_lr = pulFaultStackAddress[5];
        LDR      R1,[R0, #+20]
        LDR.N    R2,??DataTable10_12
        STR      R1,[R2, #+0]
//  138 	postmortem_pc = pulFaultStackAddress[6];
        LDR      R1,[R0, #+24]
        LDR.N    R2,??DataTable10_13
        STR      R1,[R2, #+0]
//  139 	postmortem_psr = pulFaultStackAddress[7];
        LDR      R1,[R0, #+28]
        LDR.N    R2,??DataTable10_14
        STR      R1,[R2, #+0]
//  140 
//  141 	/* Configurable Fault Status Register. Consists of MMSR, BFSR and UFSR */
//  142 	postmortem_CFSR = GET_CFSR();
        LDR.N    R1,??DataTable10_15
        LDR.N    R2,??DataTable10_16  ;; 0xe000ed28
        LDR      R2,[R2, #+0]
        STR      R2,[R1, #+0]
//  143 
//  144 	/* Hard Fault Status Register */
//  145 	postmortem_HFSR = (*((volatile uint32_t *) (0xE000ED2C)));
        LDR.N    R1,??DataTable10_17
        LDR.N    R2,??DataTable10_18  ;; 0xe000ed2c
        LDR      R2,[R2, #+0]
        STR      R2,[R1, #+0]
//  146 
//  147 	/* Debug Fault Status Register */
//  148 	postmortem_DFSR = (*((volatile uint32_t *) (0xE000ED30)));
        LDR.N    R1,??DataTable10_19
        LDR.N    R2,??DataTable10_20  ;; 0xe000ed30
        LDR      R2,[R2, #+0]
        STR      R2,[R1, #+0]
//  149 
//  150 	/* Auxiliary Fault Status Register */
//  151 	postmortem_AFSR = (*((volatile uint32_t *) (0xE000ED3C)));
        LDR.N    R1,??DataTable10_21
        LDR.N    R2,??DataTable10_22  ;; 0xe000ed3c
        LDR      R2,[R2, #+0]
        STR      R2,[R1, #+0]
//  152 
//  153 	/* Read the Fault Address Registers. These may not contain valid values.
//  154 	 Check BFARVALID/MMARVALID to see if they are valid values
//  155 	 MemManage Fault Address Register */
//  156 	postmortem_MMAR = (*((volatile uint32_t *) (0xE000ED34)));
        LDR.N    R1,??DataTable10_23
        LDR.N    R2,??DataTable10_24  ;; 0xe000ed34
        LDR      R2,[R2, #+0]
        STR      R2,[R1, #+0]
//  157 	/* Bus Fault Address Register */
//  158 	postmortem_BFAR = (*((volatile uint32_t *) (0xE000ED38)));
        LDR.N    R1,??DataTable10_25
        LDR.N    R2,??DataTable10_26  ;; 0xe000ed38
        LDR      R2,[R2, #+0]
        STR      R2,[R1, #+0]
//  159 
//  160 	postmortem_SCB_SHCSR = SCB->SHCSR;
        LDR.N    R1,??DataTable10_27
        LDR.N    R2,??DataTable10_28  ;; 0xe000ed24
        LDR      R2,[R2, #+0]
        STR      R2,[R1, #+0]
//  161 
//  162 	if (is_under_debugger()) {
        LDR.N    R1,??DataTable10_29  ;; 0xe000edf0
        LDR      R1,[R1, #+0]
        LSLS     R1,R1,#+31
        BPL.N    ??prvGetRegistersFromStack_0
//  163 		__asm("BKPT #0\n");
        BKPT #0
//  164 		// Break into the debugger
//  165 	}
//  166 
//  167 	/* harmless infinite loop */
//  168 	while (1) {
??prvGetRegistersFromStack_0:
        B.N      ??prvGetRegistersFromStack_0
          CFI EndBlock cfiBlock5
//  169 		;
//  170 	}
//  171 }
//  172 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function HardFaultVector
        THUMB
//  173 void HardFaultVector(void) {
HardFaultVector:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  174 #if 0 && defined __GNUC__
//  175 	__asm volatile (
//  176 			" tst lr, #4                                                \n"
//  177 			" ite eq                                                    \n"
//  178 			" mrseq r0, msp                                             \n"
//  179 			" mrsne r0, psp                                             \n"
//  180 			" ldr r1, [r0, #24]                                         \n"
//  181 			" ldr r2, handler2_address_const                            \n"
//  182 			" bx r2                                                     \n"
//  183 			" handler2_address_const: .word prvGetRegistersFromStack    \n"
//  184 	);
//  185 
//  186 #else
//  187 #endif        
//  188 
//  189 	int cfsr = GET_CFSR();
        LDR.N    R0,??DataTable10_16  ;; 0xe000ed28
        LDR      R4,[R0, #+0]
//  190 	if (cfsr & 0x1) {
        LSLS     R0,R4,#+31
        BPL.N    ??HardFaultVector_0
//  191 		chDbgPanic3("H IACCVIOL", __FILE__, __LINE__);
        MOVS     R2,#+191
        LDR.N    R1,??DataTable10_3
        LDR.N    R0,??DataTable10_30
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
        B.N      ??HardFaultVector_1
//  192 	} else if (cfsr & 0x100) {
??HardFaultVector_0:
        LSLS     R0,R4,#+23
        BPL.N    ??HardFaultVector_2
//  193 		chDbgPanic3("H IBUSERR", __FILE__, __LINE__);
        MOVS     R2,#+193
        LDR.N    R1,??DataTable10_3
        LDR.N    R0,??DataTable10_31
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
        B.N      ??HardFaultVector_1
//  194 	} else if (cfsr & 0x20000) {
??HardFaultVector_2:
        LSLS     R0,R4,#+14
        BPL.N    ??HardFaultVector_3
//  195 		chDbgPanic3("H INVSTATE", __FILE__, __LINE__);
        MOVS     R2,#+195
        LDR.N    R1,??DataTable10_3
        LDR.N    R0,??DataTable10_32
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
        B.N      ??HardFaultVector_1
//  196 	} else {
//  197 		chDbgPanic3("HardFaultVector", __FILE__, __LINE__);
??HardFaultVector_3:
        MOVS     R2,#+197
        LDR.N    R1,??DataTable10_3
        LDR.N    R0,??DataTable10_33
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  198 	}
//  199 
//  200 	while (TRUE) {
??HardFaultVector_1:
        B.N      ??HardFaultVector_1
          CFI EndBlock cfiBlock6
//  201 	}
//  202 }
//  203 
//  204 #if HAL_USE_SPI || defined(__DOXYGEN__)

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  205 static bool isSpiInitialized[5] = { false, false, false, false, false };
isSpiInitialized:
        DS8 8
//  206 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z8getSpiAfP9SPIDriver
          CFI NoCalls
        THUMB
//  207 static int getSpiAf(SPIDriver *driver) {
_Z8getSpiAfP9SPIDriver:
        MOVS     R1,R0
//  208 #if STM32_SPI_USE_SPI1
//  209 	if (driver == &SPID1) {
        LDR.N    R0,??DataTable10_34
        CMP      R1,R0
        BNE.N    ??getSpiAf_0
//  210 		return EFI_SPI1_AF;
        MOVS     R0,#+5
        B.N      ??getSpiAf_1
//  211 	}
//  212 #endif
//  213 #if STM32_SPI_USE_SPI2
//  214 	if (driver == &SPID2) {
??getSpiAf_0:
        LDR.N    R0,??DataTable10_35
        CMP      R1,R0
        BNE.N    ??getSpiAf_2
//  215 		return EFI_SPI2_AF;
        MOVS     R0,#+5
        B.N      ??getSpiAf_1
//  216 	}
//  217 #endif
//  218 #if STM32_SPI_USE_SPI3
//  219 	if (driver == &SPID3) {
??getSpiAf_2:
        LDR.N    R0,??DataTable10_36
        CMP      R1,R0
        BNE.N    ??getSpiAf_3
//  220 		return EFI_SPI3_AF;
        MOVS     R0,#+6
        B.N      ??getSpiAf_1
//  221 	}
//  222 #endif
//  223 	return -1;
??getSpiAf_3:
        MOVS     R0,#-1
??getSpiAf_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock7
//  224 }
//  225 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z9turnOnSpi12spi_device_e
        THUMB
//  226 void turnOnSpi(spi_device_e device) {
_Z9turnOnSpi12spi_device_e:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  227 	if (isSpiInitialized[device])
        LDR.N    R0,??DataTable10_37
        LDRB     R0,[R4, R0]
        CMP      R0,#+0
        BNE.N    ??turnOnSpi_0
//  228 		return; // already initialized
//  229 	isSpiInitialized[device] = true;
??turnOnSpi_1:
        LDR.N    R0,??DataTable10_37
        MOVS     R1,#+1
        STRB     R1,[R4, R0]
//  230 	if (device == SPI_DEVICE_1) {
        CMP      R4,#+1
        BNE.N    ??turnOnSpi_2
//  231 #if STM32_SPI_USE_SPI1
//  232 //	scheduleMsg(&logging, "Turning on SPI1 pins");
//  233 		initSpiModule(&SPID1, boardConfiguration->spi1sckPin,
//  234 				boardConfiguration->spi1misoPin,
//  235 				boardConfiguration->spi1mosiPin);
        LDR.N    R0,??DataTable10_38
        LDR      R0,[R0, #+0]
        LDR      R3,[R0, #+724]
        LDR.N    R0,??DataTable10_38
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+728]
        LDR.N    R0,??DataTable10_38
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+732]
        LDR.N    R0,??DataTable10_34
          CFI FunCall initSpiModule
        BL       initSpiModule
//  236 #endif /* STM32_SPI_USE_SPI1 */
//  237 	}
//  238 	if (device == SPI_DEVICE_2) {
??turnOnSpi_2:
        CMP      R4,#+2
        BNE.N    ??turnOnSpi_3
//  239 #if STM32_SPI_USE_SPI2
//  240 //	scheduleMsg(&logging, "Turning on SPI2 pins");
//  241 		initSpiModule(&SPID2, boardConfiguration->spi2sckPin,
//  242 				boardConfiguration->spi2misoPin,
//  243 				boardConfiguration->spi2mosiPin);
        LDR.N    R0,??DataTable10_38
        LDR      R0,[R0, #+0]
        LDR      R3,[R0, #+736]
        LDR.N    R0,??DataTable10_38
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+740]
        LDR.N    R0,??DataTable10_38
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+744]
        LDR.N    R0,??DataTable10_35
          CFI FunCall initSpiModule
        BL       initSpiModule
//  244 #endif /* STM32_SPI_USE_SPI2 */
//  245 	}
//  246 	if (device == SPI_DEVICE_3) {
??turnOnSpi_3:
        CMP      R4,#+3
        BNE.N    ??turnOnSpi_4
//  247 #if STM32_SPI_USE_SPI3
//  248 //	scheduleMsg(&logging, "Turning on SPI3 pins");
//  249 		initSpiModule(&SPID3, boardConfiguration->spi3sckPin,
//  250 				boardConfiguration->spi3misoPin,
//  251 				boardConfiguration->spi3mosiPin);
        LDR.N    R0,??DataTable10_38
        LDR      R0,[R0, #+0]
        LDR      R3,[R0, #+748]
        LDR.N    R0,??DataTable10_38
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+752]
        LDR.N    R0,??DataTable10_38
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+756]
        LDR.N    R0,??DataTable10_36
          CFI FunCall initSpiModule
        BL       initSpiModule
//  252 #endif /* STM32_SPI_USE_SPI3 */
//  253 	}
//  254 }
??turnOnSpi_4:
??turnOnSpi_0:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock8
//  255 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function initSpiModule
        THUMB
//  256 void initSpiModule(SPIDriver *driver, brain_pin_e sck, brain_pin_e miso,
//  257 		brain_pin_e mosi) {
initSpiModule:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
//  258 
//  259 	mySetPadMode2("SPI clock", sck,	PAL_MODE_ALTERNATE(getSpiAf(driver)));
        MOVS     R0,R4
          CFI FunCall _Z8getSpiAfP9SPIDriver
        BL       _Z8getSpiAfP9SPIDriver
        LSLS     R0,R0,#+7
        ORRS     R2,R0,#0x2
        MOVS     R1,R5
        LDR.N    R0,??DataTable10_39
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
//  260 
//  261 	mySetPadMode2("SPI master out", mosi, PAL_MODE_ALTERNATE(getSpiAf(driver)));
        MOVS     R0,R4
          CFI FunCall _Z8getSpiAfP9SPIDriver
        BL       _Z8getSpiAfP9SPIDriver
        LSLS     R0,R0,#+7
        ORRS     R2,R0,#0x2
        MOVS     R1,R7
        LDR.N    R0,??DataTable10_40
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
//  262 	mySetPadMode2("SPI master in ", miso, PAL_MODE_ALTERNATE(getSpiAf(driver)));
        MOVS     R0,R4
          CFI FunCall _Z8getSpiAfP9SPIDriver
        BL       _Z8getSpiAfP9SPIDriver
        LSLS     R0,R0,#+7
        ORRS     R2,R0,#0x2
        MOVS     R1,R6
        LDR.N    R0,??DataTable10_41
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
//  263 }
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock9
//  264 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function initSpiCs
        THUMB
//  265 void initSpiCs(SPIConfig *spiConfig, brain_pin_e csPin) {
initSpiCs:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  266 	spiConfig->end_cb = NULL;
        MOVS     R0,#+0
        STR      R0,[R4, #+0]
//  267 	ioportid_t port = getHwPort(csPin);
        MOVS     R0,R5
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOVS     R6,R0
//  268 	ioportmask_t pin = getHwPin(csPin);
        MOVS     R0,R5
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        MOVS     R7,R0
//  269 	spiConfig->ssport = port;
        STR      R6,[R4, #+4]
//  270 	spiConfig->sspad = pin;
        STRH     R7,[R4, #+8]
//  271 	mySetPadMode("chip select", port, pin, PAL_STM32_MODE_OUTPUT);
        MOVS     R3,#+1
        MOVS     R2,R7
        MOVS     R1,R6
        LDR.N    R0,??DataTable10_42
          CFI FunCall _Z12mySetPadModePKcP12GPIO_TypeDefjj
        BL       _Z12mySetPadModePKcP12GPIO_TypeDefjj
//  272 }
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock10

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10:
        DC32     dbg_isr_cnt

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_1:
        DC32     `IRQSTACK$$Base`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_2:
        DC32     0xe0001004

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_3:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_4:
        DC32     `?<Constant "DebugMonitorVector">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_5:
        DC32     `?<Constant "UsageFaultVector">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_6:
        DC32     `?<Constant "BusFaultVector">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_7:
        DC32     postmortem_r0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_8:
        DC32     postmortem_r1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_9:
        DC32     postmortem_r2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_10:
        DC32     postmortem_r3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_11:
        DC32     postmortem_r12

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_12:
        DC32     postmortem_lr

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_13:
        DC32     postmortem_pc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_14:
        DC32     postmortem_psr

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_15:
        DC32     postmortem_CFSR

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_16:
        DC32     0xe000ed28

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_17:
        DC32     postmortem_HFSR

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_18:
        DC32     0xe000ed2c

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_19:
        DC32     postmortem_DFSR

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_20:
        DC32     0xe000ed30

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_21:
        DC32     postmortem_AFSR

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_22:
        DC32     0xe000ed3c

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_23:
        DC32     postmortem_MMAR

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_24:
        DC32     0xe000ed34

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_25:
        DC32     postmortem_BFAR

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_26:
        DC32     0xe000ed38

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_27:
        DC32     postmortem_SCB_SHCSR

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_28:
        DC32     0xe000ed24

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_29:
        DC32     0xe000edf0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_30:
        DC32     `?<Constant "H IACCVIOL">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_31:
        DC32     `?<Constant "H IBUSERR">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_32:
        DC32     `?<Constant "H INVSTATE">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_33:
        DC32     `?<Constant "HardFaultVector">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_34:
        DC32     SPID1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_35:
        DC32     SPID2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_36:
        DC32     SPID3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_37:
        DC32     isSpiInitialized

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_38:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_39:
        DC32     `?<Constant "SPI clock">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_40:
        DC32     `?<Constant "SPI master out">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_41:
        DC32     `?<Constant "SPI master in ">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_42:
        DC32     `?<Constant "chip select">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "DebugMonitorVector">`:
        DATA
        DC8 "DebugMonitorVector"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`:
        DATA
        DC8 46H, 3AH, 5CH, 73H, 74H, 75H, 66H, 66H
        DC8 5CH, 72H, 75H, 73H, 65H, 66H, 69H, 5FH
        DC8 73H, 6FH, 75H, 72H, 63H, 65H, 66H, 6FH
        DC8 72H, 67H, 65H, 5CH, 66H, 69H, 72H, 6DH
        DC8 77H, 61H, 72H, 65H, 5CH, 68H, 77H, 5FH
        DC8 6CH, 61H, 79H, 65H, 72H, 5CH, 73H, 74H
        DC8 6DH, 33H, 32H, 66H, 34H, 5CH, 6DH, 70H
        DC8 75H, 5FH, 75H, 74H, 69H, 6CH, 2EH, 63H
        DC8 70H, 70H, 0
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "UsageFaultVector">`:
        DATA
        DC8 "UsageFaultVector"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "BusFaultVector">`:
        DATA
        DC8 "BusFaultVector"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "H IACCVIOL">`:
        DATA
        DC8 "H IACCVIOL"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "H IBUSERR">`:
        DATA
        DC8 "H IBUSERR"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "H INVSTATE">`:
        DATA
        DC8 "H INVSTATE"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "HardFaultVector">`:
        DATA
        DC8 "HardFaultVector"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SPI clock">`:
        DATA
        DC8 "SPI clock"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SPI master out">`:
        DATA
        DC8 "SPI master out"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SPI master in ">`:
        DATA
        DC8 "SPI master in "
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "chip select">`:
        DATA
        DC8 "chip select"

        END
//  273 
//  274 #endif
// 
//  68 bytes in section .bss
// 232 bytes in section .rodata
// 716 bytes in section .text
// 
// 716 bytes of CODE  memory
// 232 bytes of CONST memory
//  68 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
