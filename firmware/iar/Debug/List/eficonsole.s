///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:35 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\console\eficons /
//                       ole.cpp                                              /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\console\eficons /
//                       ole.cpp -lCN F:\stuff\rusefi_sourceforge\firmware\ia /
//                       r\Debug\List\ -lA F:\stuff\rusefi_sourceforge\firmwa /
//                       re\iar\Debug\List\ -o F:\stuff\rusefi_sourceforge\fi /
//                       rmware\iar\Debug\Obj\ --no_cse --no_unroll           /
//                       --no_inline --no_code_motion --no_tbaa               /
//                       --no_clustering --no_scheduling --debug              /
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
//                       eficonsole.s                                         /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME eficonsole

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z12startConsoleP7LoggingPFvPcE
        EXTERN _Z14isConsoleReadyv
        EXTERN _Z17getConsoleChannelv
        EXTERN _ZN18LoggingWithStorageC1EPKc
        EXTERN addConsoleAction
        EXTERN chDbgPanic3
        EXTERN chRegFirstThread
        EXTERN chRegNextThread
        EXTERN chThdSleep
        EXTERN chvprintf
        EXTERN firmwareError
        EXTERN getRusEfiVersion
        EXTERN handleConsoleLine
        EXTERN initConsoleLogic
        EXTERN initIntermediateLoggingBuffer
        EXTERN scheduleMsg
        EXTERN strncpy

        PUBLIC _Z6fatal3PcS_i
        PUBLIC initializeConsole
        PUBLIC print
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\console\eficonsole.cpp
//    1 /**
//    2  * @file    eficonsole.cpp
//    3  * @brief   Console package entry point code
//    4  *
//    5  *
//    6  * @date Nov 15, 2012
//    7  * @author Andrey Belomutskiy, (c) 2012-2015
//    8  *
//    9  *
//   10  * This file is part of rusEfi - see http://rusefi.com
//   11  *
//   12  * rusEfi is free software; you can redistribute it and/or modify it under the terms of
//   13  * the GNU General Public License as published by the Free Software Foundation; either
//   14  * version 3 of the License, or (at your option) any later version.
//   15  *
//   16  * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
//   17  * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   18  * GNU General Public License for more details.
//   19  *
//   20  * You should have received a copy of the GNU General Public License along with this program.
//   21  * If not, see <http://www.gnu.org/licenses/>.
//   22  */
//   23 
//   24 #include <stdarg.h>
//   25 #include <stdbool.h>
//   26 #include "main.h"
//   27 #include <chprintf.h>
//   28 #include "eficonsole.h"
//   29 #include "console_io.h"
//   30 #include "svnversion.h"
//   31 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   32 static LoggingWithStorage logger("console");
        LDR.N    R1,??DataTable6
        LDR.N    R0,??DataTable6_1
          CFI FunCall _ZN18LoggingWithStorageC1EPKc
        BL       _ZN18LoggingWithStorageC1EPKc
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
logger:
        DS8 260
//   33 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   34 static char fatalErrorMessage[200];
fatalErrorMessage:
        DS8 200
//   35 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z6fatal3PcS_i
        THUMB
//   36 void fatal3(char *msg, char *file, int line) {
_Z6fatal3PcS_i:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   37 	strncpy(fatalErrorMessage, msg, sizeof(fatalErrorMessage) - 1);
        MOVS     R2,#+199
        MOVS     R1,R4
        LDR.N    R0,??DataTable6_2
          CFI FunCall strncpy
        BL       strncpy
//   38 	fatalErrorMessage[sizeof(fatalErrorMessage) - 1] = 0; // just to be sure
        LDR.N    R1,??DataTable6_2
        MOVS     R2,#+0
        STRB     R2,[R1, #+199]
//   39 #if EFI_CUSTOM_PANIC_METHOD
//   40 	chDbgPanic3(fatalErrorMessage, file, line);
        MOVS     R2,R6
        MOVS     R1,R5
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//   41 #else
//   42 	chDbgPanic(fatalErrorMessage);
//   43 #endif
//   44 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock1
//   45 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z7myfatalv
        THUMB
//   46 static void myfatal(void) {
_Z7myfatalv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   47 	chDbgCheck(0, "my fatal");
        MOVS     R2,#+47
        LDR.N    R1,??DataTable6_3
        LDR.N    R0,??DataTable6_4
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//   48 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock2
//   49 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z7myerrorv
        THUMB
//   50 static void myerror(void) {
_Z7myerrorv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   51 	firmwareError("firmwareError: %d", getRusEfiVersion());
          CFI FunCall getRusEfiVersion
        BL       getRusEfiVersion
        MOVS     R1,R0
        LDR.N    R0,??DataTable6_5
          CFI FunCall firmwareError
        BL       firmwareError
//   52 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock3
//   53 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z10sayNothingv
          CFI NoCalls
        THUMB
//   54 static void sayNothing(void) {
//   55 	/**
//   56 	 * @see EngineState#TS_PROTOCOL_TAG
//   57 	 * this empty response is part of protocol check
//   58 	 * todo: make this logic smarter?
//   59 	 */
//   60 }
_Z10sayNothingv:
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//   61 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z8sayHellov
        THUMB
//   62 static void sayHello(void) {
_Z8sayHellov:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   63 	scheduleMsg(&logger, "*** rusEFI (c) Andrey Belomutskiy 2012-2015. All rights reserved.");
        LDR.N    R1,??DataTable6_6
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   64 	scheduleMsg(&logger, "rusEFI v%d@%s", getRusEfiVersion(), VCS_VERSION);
          CFI FunCall getRusEfiVersion
        BL       getRusEfiVersion
        LDR.N    R3,??DataTable6_7
        MOVS     R2,R0
        LDR.N    R1,??DataTable6_8
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   65 	scheduleMsg(&logger, "*** Chibios Kernel:       %s", CH_KERNEL_VERSION);
        LDR.N    R2,??DataTable6_9
        LDR.N    R1,??DataTable6_10
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   66 	scheduleMsg(&logger, "*** Compiled:     " __DATE__ " - " __TIME__ "");
        LDR.N    R1,??DataTable6_11
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   67 	scheduleMsg(&logger, "COMPILER=%s", __VERSION__);
        LDR.N    R2,??DataTable6_12
        LDR.N    R1,??DataTable6_13
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   68 	scheduleMsg(&logger, "CH_FREQUENCY=%d", CH_FREQUENCY);
        MOV      R2,#+1000
        LDR.N    R1,??DataTable6_14
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   69 #ifdef SERIAL_SPEED
//   70 	scheduleMsg(&logger, "SERIAL_SPEED=%d", SERIAL_SPEED);
        MOVS     R2,#+115200
        LDR.N    R1,??DataTable6_15
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   71 #endif
//   72 
//   73 #ifdef CORTEX_MAX_KERNEL_PRIORITY
//   74 	scheduleMsg(&logger, "CORTEX_MAX_KERNEL_PRIORITY=%d", CORTEX_MAX_KERNEL_PRIORITY);
        MOVS     R2,#+2
        LDR.N    R1,??DataTable6_16
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   75 #endif
//   76 
//   77 #ifdef STM32_ADCCLK
//   78 	scheduleMsg(&logger, "STM32_ADCCLK=%d", STM32_ADCCLK);
        LDR.N    R2,??DataTable6_17  ;; 0x1406f40
        LDR.N    R1,??DataTable6_18
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   79 	scheduleMsg(&logger, "STM32_TIMCLK1=%d", STM32_TIMCLK1);
        LDR.N    R2,??DataTable6_19  ;; 0x501bd00
        LDR.N    R1,??DataTable6_20
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   80 	scheduleMsg(&logger, "STM32_TIMCLK2=%d", STM32_TIMCLK2);
        LDR.N    R2,??DataTable6_21  ;; 0xa037a00
        LDR.N    R1,??DataTable6_22
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   81 #endif
//   82 #ifdef STM32_PCLK1
//   83 	scheduleMsg(&logger, "STM32_PCLK1=%d", STM32_PCLK1);
        LDR.N    R2,??DataTable6_23  ;; 0x280de80
        LDR.N    R1,??DataTable6_24
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   84 	scheduleMsg(&logger, "STM32_PCLK2=%d", STM32_PCLK2);
        LDR.N    R2,??DataTable6_19  ;; 0x501bd00
        LDR.N    R1,??DataTable6_25
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   85 #endif
//   86 
//   87 	scheduleMsg(&logger, "PORT_IDLE_THREAD_STACK_SIZE=%d", PORT_IDLE_THREAD_STACK_SIZE);
        MOV      R2,#+1024
        LDR.N    R1,??DataTable6_26
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   88 
//   89 	scheduleMsg(&logger, "CH_DBG_ENABLE_ASSERTS=%d", CH_DBG_ENABLE_ASSERTS);
        MOVS     R2,#+1
        LDR.N    R1,??DataTable6_27
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   90 	scheduleMsg(&logger, "CH_DBG_ENABLED=%d", CH_DBG_ENABLED);
        MOVS     R2,#+1
        LDR.N    R1,??DataTable6_28
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   91 	scheduleMsg(&logger, "CH_DBG_SYSTEM_STATE_CHECK=%d", CH_DBG_SYSTEM_STATE_CHECK);
        MOVS     R2,#+1
        LDR.N    R1,??DataTable6_29
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   92 	scheduleMsg(&logger, "CH_DBG_ENABLE_STACK_CHECK=%d", CH_DBG_ENABLE_STACK_CHECK);
        MOVS     R2,#+1
        LDR.N    R1,??DataTable6_30
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   93 
//   94 #ifdef EFI_WAVE_ANALYZER
//   95 	scheduleMsg(&logger, "EFI_WAVE_ANALYZER=%d", EFI_WAVE_ANALYZER);
        MOVS     R2,#+1
        LDR.N    R1,??DataTable6_31
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   96 #endif
//   97 #ifdef EFI_TUNER_STUDIO
//   98 	scheduleMsg(&logger, "EFI_TUNER_STUDIO=%d", EFI_TUNER_STUDIO);
        MOVS     R2,#+1
        LDR.N    R1,??DataTable6_32
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   99 #else
//  100 	scheduleMsg(&logger, "EFI_TUNER_STUDIO=%d", 0);
//  101 #endif
//  102 
//  103 #ifdef EFI_SIGNAL_EXECUTOR_SLEEP
//  104 	scheduleMsg(&logger, "EFI_SIGNAL_EXECUTOR_SLEEP=%d", EFI_SIGNAL_EXECUTOR_SLEEP);
        MOVS     R2,#+0
        LDR.N    R1,??DataTable6_33
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  105 #endif
//  106 
//  107 #ifdef EFI_SIGNAL_EXECUTOR_HW_TIMER
//  108 	scheduleMsg(&logger, "EFI_SIGNAL_EXECUTOR_HW_TIMER=%d", EFI_SIGNAL_EXECUTOR_HW_TIMER);
        MOVS     R2,#+0
        LDR.N    R1,??DataTable6_34
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  109 #endif
//  110 
//  111 #if defined(EFI_SHAFT_POSITION_INPUT) || defined(__DOXYGEN__)
//  112 	scheduleMsg(&logger, "EFI_SHAFT_POSITION_INPUT=%d", EFI_SHAFT_POSITION_INPUT);
        MOVS     R2,#+1
        LDR.N    R1,??DataTable6_35
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  113 #endif
//  114 #ifdef EFI_INTERNAL_ADC
//  115 	scheduleMsg(&logger, "EFI_INTERNAL_ADC=%d", EFI_INTERNAL_ADC);
        MOVS     R2,#+1
        LDR.N    R1,??DataTable6_36
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  116 #endif
//  117 
//  118 //	printSimpleMsg(&logger, "", );
//  119 //	printSimpleMsg(&logger, "", );
//  120 
//  121 	/**
//  122 	 * Time to finish output. This is needed to avoid mix-up of this methods output and console command confirmation
//  123 	 */
//  124 	chThdSleepMilliseconds(5);
        MOVS     R0,#+5
          CFI FunCall chThdSleep
        BL       chThdSleep
//  125 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock5
//  126 
//  127 /**
//  128  * This methods prints all threads and their total times
//  129  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z11cmd_threadsv
        THUMB
//  130 static void cmd_threads(void) {
_Z11cmd_threadsv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        SUB      SP,SP,#+24
          CFI CFA R13+32
//  131 #if CH_DBG_THREADS_PROFILING || defined(__DOXYGEN__)
//  132   static const char *states[] = { THD_STATE_NAMES };
//  133   Thread *tp;
//  134   
//  135   scheduleMsg(&logger, "    addr    stack prio refs     state time");
        LDR.N    R1,??DataTable6_37
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  136   tp = chRegFirstThread();
          CFI FunCall chRegFirstThread
        BL       chRegFirstThread
        MOVS     R4,R0
//  137   while (tp != NULL) {    
??cmd_threads_0:
        CMP      R4,#+0
        BEQ.N    ??cmd_threads_1
//  138     scheduleMsg(&logger, "%.8lx [%.8lx] %4lu %4lu %9s %lu %s", (uint32_t) tp, 0, (uint32_t) tp->p_prio,
//  139 		(uint32_t) (0), states[tp->p_state], (uint32_t) tp->p_time, tp->p_name);
        LDR      R0,[R4, #+24]
        STR      R0,[SP, #+16]
        LDR      R0,[R4, #+36]
        STR      R0,[SP, #+12]
        LDRB     R0,[R4, #+32]
        LDR.N    R1,??DataTable6_38
        LDR      R0,[R1, R0, LSL #+2]
        STR      R0,[SP, #+8]
        MOVS     R0,#+0
        STR      R0,[SP, #+4]
        LDR      R0,[R4, #+8]
        STR      R0,[SP, #+0]
        MOVS     R3,#+0
        MOVS     R2,R4
        LDR.N    R1,??DataTable6_39
        LDR.N    R0,??DataTable6_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  140     tp = chRegNextThread(tp);
        MOVS     R0,R4
          CFI FunCall chRegNextThread
        BL       chRegNextThread
        MOVS     R4,R0
        B.N      ??cmd_threads_0
//  141   } 
//  142   
//  143 #else
//  144   scheduleMsg(&logger, "CH_DBG_THREADS_PROFILING is not enabled");
//  145 #endif
//  146 }
??cmd_threads_1:
        ADD      SP,SP,#+24
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock6

        SECTION `.data`:DATA:REORDER:NOROOT(2)
??states:
        DATA
        DC32 `?<Constant "READY">`, `?<Constant "CURRENT">`
        DC32 `?<Constant "SUSPENDED">`, `?<Constant "WTSEM">`
        DC32 `?<Constant "WTMTX">`, `?<Constant "WTCOND">`
        DC32 `?<Constant "SLEEPING">`, `?<Constant "WTEXIT">`
        DC32 `?<Constant "WTOREVT">`, `?<Constant "WTANDEVT">`
        DC32 `?<Constant "SNDMSGQ">`, `?<Constant "SNDMSG">`
        DC32 `?<Constant "WTMSG">`, `?<Constant "WTQUEUE">`
        DC32 `?<Constant "FINAL">`
//  147 
//  148 /**
//  149  * This methods prints the message to whatever is configured as our primary console
//  150  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function print
        THUMB
//  151 void print(const char *format, ...) {
print:
        PUSH     {R1-R3}
          CFI CFA R13+12
        PUSH     {R4,R5,LR}
          CFI R14 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
//  152 #if !EFI_UART_ECHO_TEST_MODE
//  153 	if (!isConsoleReady()) {
          CFI FunCall _Z14isConsoleReadyv
        BL       _Z14isConsoleReadyv
        CMP      R0,#+0
        BEQ.N    ??print_0
//  154 		return;
//  155 	}
//  156 	va_list ap;
//  157 	va_start(ap, format);
??print_1:
        ADD      R0,SP,#+12
        MOVS     R5,R0
//  158 	chvprintf((BaseSequentialStream*) getConsoleChannel(), format, ap);
          CFI FunCall _Z17getConsoleChannelv
        BL       _Z17getConsoleChannelv
        MOVS     R2,R5
        MOVS     R1,R4
          CFI FunCall chvprintf
        BL       chvprintf
//  159 	va_end(ap);
//  160 #endif /* EFI_UART_ECHO_TEST_MODE */
//  161 }
??print_0:
        POP      {R4,R5}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI CFA R13+16
        LDR      PC,[SP], #+16    ;; return
          CFI EndBlock cfiBlock7
//  162 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function initializeConsole
        THUMB
//  163 void initializeConsole(Logging *sharedLogger) {
initializeConsole:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  164 	initIntermediateLoggingBuffer();
          CFI FunCall initIntermediateLoggingBuffer
        BL       initIntermediateLoggingBuffer
//  165 	initConsoleLogic(sharedLogger);
        MOVS     R0,R4
          CFI FunCall initConsoleLogic
        BL       initConsoleLogic
//  166 
//  167 	startConsole(sharedLogger, &handleConsoleLine);
        LDR.N    R1,??DataTable6_40
        MOVS     R0,R4
          CFI FunCall _Z12startConsoleP7LoggingPFvPcE
        BL       _Z12startConsoleP7LoggingPFvPcE
//  168 
//  169 	sayHello();
          CFI FunCall _Z8sayHellov
        BL       _Z8sayHellov
//  170 	addConsoleAction("test", sayNothing);
        LDR.N    R1,??DataTable6_41
        LDR.N    R0,??DataTable6_42
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  171 	addConsoleAction("hello", sayHello);
        LDR.N    R1,??DataTable6_43
        LDR.N    R0,??DataTable6_44
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  172 #if EFI_HAS_RESET
//  173 	addConsoleAction("reset", scheduleReset);
//  174 #endif
//  175 
//  176 	addConsoleAction("fatal", myfatal);
        LDR.N    R1,??DataTable6_45
        LDR.N    R0,??DataTable6_46
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  177 	addConsoleAction("error", myerror);
        LDR.N    R1,??DataTable6_47
        LDR.N    R0,??DataTable6_48
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  178 	addConsoleAction("threadsinfo", cmd_threads);
        LDR.N    R1,??DataTable6_49
        LDR.N    R0,??DataTable6_50
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  179 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6:
        DC32     `?<Constant "console">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_1:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_2:
        DC32     fatalErrorMessage

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_3:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_4:
        DC32     `?<Constant "\\"my fatal\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_5:
        DC32     `?<Constant "firmwareError: %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_6:
        DC32     `?<Constant "*** rusEFI (c) Andrey...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_7:
        DC32     `?<Constant "8529">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_8:
        DC32     `?<Constant "rusEFI v%d@%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_9:
        DC32     `?<Constant "2.6.8">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_10:
        DC32     `?<Constant "*** Chibios Kernel:  ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_11:
        DC32     `?<Constant "*** Compiled:     Jun...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_12:
        DC32     `?<Constant "IAR ANSI C/C++ Compil...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_13:
        DC32     `?<Constant "COMPILER=%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_14:
        DC32     `?<Constant "CH_FREQUENCY=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_15:
        DC32     `?<Constant "SERIAL_SPEED=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_16:
        DC32     `?<Constant "CORTEX_MAX_KERNEL_PRI...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_17:
        DC32     0x1406f40

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_18:
        DC32     `?<Constant "STM32_ADCCLK=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_19:
        DC32     0x501bd00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_20:
        DC32     `?<Constant "STM32_TIMCLK1=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_21:
        DC32     0xa037a00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_22:
        DC32     `?<Constant "STM32_TIMCLK2=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_23:
        DC32     0x280de80

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_24:
        DC32     `?<Constant "STM32_PCLK1=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_25:
        DC32     `?<Constant "STM32_PCLK2=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_26:
        DC32     `?<Constant "PORT_IDLE_THREAD_STAC...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_27:
        DC32     `?<Constant "CH_DBG_ENABLE_ASSERTS=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_28:
        DC32     `?<Constant "CH_DBG_ENABLED=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_29:
        DC32     `?<Constant "CH_DBG_SYSTEM_STATE_C...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_30:
        DC32     `?<Constant "CH_DBG_ENABLE_STACK_C...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_31:
        DC32     `?<Constant "EFI_WAVE_ANALYZER=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_32:
        DC32     `?<Constant "EFI_TUNER_STUDIO=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_33:
        DC32     `?<Constant "EFI_SIGNAL_EXECUTOR_S...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_34:
        DC32     `?<Constant "EFI_SIGNAL_EXECUTOR_H...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_35:
        DC32     `?<Constant "EFI_SHAFT_POSITION_IN...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_36:
        DC32     `?<Constant "EFI_INTERNAL_ADC=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_37:
        DC32     `?<Constant "    addr    stack pri...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_38:
        DC32     ??states

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_39:
        DC32     `?<Constant "%.8lx [%.8lx] %4lu %4...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_40:
        DC32     handleConsoleLine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_41:
        DC32     _Z10sayNothingv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_42:
        DC32     `?<Constant "test">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_43:
        DC32     _Z8sayHellov

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_44:
        DC32     `?<Constant "hello">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_45:
        DC32     _Z7myfatalv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_46:
        DC32     `?<Constant "fatal">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_47:
        DC32     _Z7myerrorv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_48:
        DC32     `?<Constant "error">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_49:
        DC32     _Z11cmd_threadsv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_50:
        DC32     `?<Constant "threadsinfo">`

        SECTION `.init_array`:CODE:ROOT(2)
        SECTION_TYPE SHT_INIT_ARRAY, 0
        DATA
        DC32    RELOC_ARM_TARGET1 __sti__routine

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"my fatal\\"()">`:
        DATA
        DC8 "\"my fatal\"()"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`:
        DATA
        DC8 46H, 3AH, 5CH, 73H, 74H, 75H, 66H, 66H
        DC8 5CH, 72H, 75H, 73H, 65H, 66H, 69H, 5FH
        DC8 73H, 6FH, 75H, 72H, 63H, 65H, 66H, 6FH
        DC8 72H, 67H, 65H, 5CH, 66H, 69H, 72H, 6DH
        DC8 77H, 61H, 72H, 65H, 5CH, 63H, 6FH, 6EH
        DC8 73H, 6FH, 6CH, 65H, 5CH, 65H, 66H, 69H
        DC8 63H, 6FH, 6EH, 73H, 6FH, 6CH, 65H, 2EH
        DC8 63H, 70H, 70H, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "firmwareError: %d">`:
        DATA
        DC8 "firmwareError: %d"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "*** rusEFI (c) Andrey...">`:
        DATA
        DC8 2AH, 2AH, 2AH, 20H, 72H, 75H, 73H, 45H
        DC8 46H, 49H, 20H, 28H, 63H, 29H, 20H, 41H
        DC8 6EH, 64H, 72H, 65H, 79H, 20H, 42H, 65H
        DC8 6CH, 6FH, 6DH, 75H, 74H, 73H, 6BH, 69H
        DC8 79H, 20H, 32H, 30H, 31H, 32H, 2DH, 32H
        DC8 30H, 31H, 35H, 2EH, 20H, 41H, 6CH, 6CH
        DC8 20H, 72H, 69H, 67H, 68H, 74H, 73H, 20H
        DC8 72H, 65H, 73H, 65H, 72H, 76H, 65H, 64H
        DC8 2EH, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "rusEFI v%d@%s">`:
        DATA
        DC8 "rusEFI v%d@%s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "8529">`:
        DATA
        DC8 "8529"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "*** Chibios Kernel:  ...">`:
        DATA
        DC8 "*** Chibios Kernel:       %s"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "2.6.8">`:
        DATA
        DC8 "2.6.8"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "*** Compiled:     Jun...">`:
        DATA
        DC8 "*** Compiled:     Jun 30 2015 - 23:09:35"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "COMPILER=%s">`:
        DATA
        DC8 "COMPILER=%s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "IAR ANSI C/C++ Compil...">`:
        DATA
        DC8 "IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "CH_FREQUENCY=%d">`:
        DATA
        DC8 "CH_FREQUENCY=%d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SERIAL_SPEED=%d">`:
        DATA
        DC8 "SERIAL_SPEED=%d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "CORTEX_MAX_KERNEL_PRI...">`:
        DATA
        DC8 "CORTEX_MAX_KERNEL_PRIORITY=%d"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "STM32_ADCCLK=%d">`:
        DATA
        DC8 "STM32_ADCCLK=%d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "STM32_TIMCLK1=%d">`:
        DATA
        DC8 "STM32_TIMCLK1=%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "STM32_TIMCLK2=%d">`:
        DATA
        DC8 "STM32_TIMCLK2=%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "STM32_PCLK1=%d">`:
        DATA
        DC8 "STM32_PCLK1=%d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "STM32_PCLK2=%d">`:
        DATA
        DC8 "STM32_PCLK2=%d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "PORT_IDLE_THREAD_STAC...">`:
        DATA
        DC8 "PORT_IDLE_THREAD_STACK_SIZE=%d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "CH_DBG_ENABLE_ASSERTS=%d">`:
        DATA
        DC8 "CH_DBG_ENABLE_ASSERTS=%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "CH_DBG_ENABLED=%d">`:
        DATA
        DC8 "CH_DBG_ENABLED=%d"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "CH_DBG_SYSTEM_STATE_C...">`:
        DATA
        DC8 "CH_DBG_SYSTEM_STATE_CHECK=%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "CH_DBG_ENABLE_STACK_C...">`:
        DATA
        DC8 "CH_DBG_ENABLE_STACK_CHECK=%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EFI_WAVE_ANALYZER=%d">`:
        DATA
        DC8 "EFI_WAVE_ANALYZER=%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EFI_TUNER_STUDIO=%d">`:
        DATA
        DC8 "EFI_TUNER_STUDIO=%d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EFI_SIGNAL_EXECUTOR_S...">`:
        DATA
        DC8 "EFI_SIGNAL_EXECUTOR_SLEEP=%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EFI_SIGNAL_EXECUTOR_H...">`:
        DATA
        DC8 "EFI_SIGNAL_EXECUTOR_HW_TIMER=%d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EFI_SHAFT_POSITION_IN...">`:
        DATA
        DC8 "EFI_SHAFT_POSITION_INPUT=%d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EFI_INTERNAL_ADC=%d">`:
        DATA
        DC8 "EFI_INTERNAL_ADC=%d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "    addr    stack pri...">`:
        DATA
        DC8 "    addr    stack prio refs     state time"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "%.8lx [%.8lx] %4lu %4...">`:
        DATA
        DC8 "%.8lx [%.8lx] %4lu %4lu %9s %lu %s"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "READY">`:
        DATA
        DC8 "READY"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "CURRENT">`:
        DATA
        DC8 "CURRENT"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SUSPENDED">`:
        DATA
        DC8 "SUSPENDED"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "WTSEM">`:
        DATA
        DC8 "WTSEM"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "WTMTX">`:
        DATA
        DC8 "WTMTX"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "WTCOND">`:
        DATA
        DC8 "WTCOND"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SLEEPING">`:
        DATA
        DC8 "SLEEPING"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "WTEXIT">`:
        DATA
        DC8 "WTEXIT"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "WTOREVT">`:
        DATA
        DC8 "WTOREVT"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "WTANDEVT">`:
        DATA
        DC8 "WTANDEVT"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SNDMSGQ">`:
        DATA
        DC8 "SNDMSGQ"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SNDMSG">`:
        DATA
        DC8 "SNDMSG"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "WTMSG">`:
        DATA
        DC8 "WTMSG"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "WTQUEUE">`:
        DATA
        DC8 "WTQUEUE"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FINAL">`:
        DATA
        DC8 "FINAL"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "test">`:
        DATA
        DC8 "test"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "hello">`:
        DATA
        DC8 "hello"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fatal">`:
        DATA
        DC8 "fatal"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "error">`:
        DATA
        DC8 "error"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "threadsinfo">`:
        DATA
        DC8 "threadsinfo"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "console">`:
        DATA
        DC8 "console"

        END
// 
//   460 bytes in section .bss
//    60 bytes in section .data
//     4 bytes in section .init_array
// 1 052 bytes in section .rodata
//   722 bytes in section .text
// 
//   726 bytes of CODE  memory
// 1 052 bytes of CONST memory
//   520 bytes of DATA  memory
//
//Errors: none
//Warnings: none
