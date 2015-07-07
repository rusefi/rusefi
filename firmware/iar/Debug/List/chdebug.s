///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:41 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chdebug.c                                     /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chdebug.c -lCN F:\stuff\rusefi_sourceforge\fi /
//                       rmware\iar\Debug\List\ -lA                           /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                        -o F:\stuff\rusefi_sourceforge\firmware\iar\Debug\O /
//                       bj\ --no_cse --no_unroll --no_inline                 /
//                       --no_code_motion --no_tbaa --no_clustering           /
//                       --no_scheduling --debug --endian=little              /
//                       --cpu=Cortex-M4 -e --fpu=VFPv4_sp --dlib_config      /
//                       "C:\Program Files (x86)\IAR Systems\Embedded         /
//                       Workbench 6.5\arm\INC\c\DLib_Config_Normal.h"        /
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
//                       -On                                                  /
//    MISRA C version =  2004                                                 /
//      Enabled       =  1.2-1.4 2.3 3.4 4.2 6.5 7 8.2 8.3 9.3 12.3 13.6      /
//                       13.7 15.5 17.3 19.17 20.7-20.9 20.11                 /
//      Checked       =  1.4 2.3 4.2 6.5 7 8.2 8.3 9.3 12.3 13.7 15.5         /
//                       20.7-20.9 20.11                                      /
//      Not checked   =  1.1-1.3 1.5-2.2 2.4-4.1 5-6.4 8.1 8.4-9.2 10-12.2    /
//                       12.4-13.6 14-15.4 16-20.6 20.10 20.12 21             /
//    List file       =  F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                       chdebug.s                                            /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME chdebug

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN chDbgPanic3
        EXTERN maxNesting
        EXTERN rlist
        EXTERN vtlist

        PUBLIC _trace_init
        PUBLIC chDbgCheckClassI
        PUBLIC chDbgCheckClassS
        PUBLIC chDbgPanic
        PUBLIC dbg_check_disable
        PUBLIC dbg_check_enable
        PUBLIC dbg_check_enter_isr
        PUBLIC dbg_check_leave_isr
        PUBLIC dbg_check_lock
        PUBLIC dbg_check_lock_from_isr
        PUBLIC dbg_check_suspend
        PUBLIC dbg_check_unlock
        PUBLIC dbg_check_unlock_from_isr
        PUBLIC dbg_isr_cnt
        PUBLIC dbg_lock_cnt
        PUBLIC dbg_panic_msg
        PUBLIC dbg_trace
        PUBLIC dbg_trace_buffer
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\kernel\src\chdebug.c
//    1 /*
//    2     ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
//    3                  2011,2012,2013 Giovanni Di Sirio.
//    4 
//    5     This file is part of ChibiOS/RT.
//    6 
//    7     ChibiOS/RT is free software; you can redistribute it and/or modify
//    8     it under the terms of the GNU General Public License as published by
//    9     the Free Software Foundation; either version 3 of the License, or
//   10     (at your option) any later version.
//   11 
//   12     ChibiOS/RT is distributed in the hope that it will be useful,
//   13     but WITHOUT ANY WARRANTY; without even the implied warranty of
//   14     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   15     GNU General Public License for more details.
//   16 
//   17     You should have received a copy of the GNU General Public License
//   18     along with this program.  If not, see <http://www.gnu.org/licenses/>.
//   19 
//   20                                       ---
//   21 
//   22     A special exception to the GPL can be applied should you wish to distribute
//   23     a combined work that includes ChibiOS/RT, without being obliged to provide
//   24     the source code for any proprietary components. See the file exception.txt
//   25     for full details of how and when the exception can be applied.
//   26 */
//   27 
//   28 /**
//   29  * @file    chdebug.c
//   30  * @brief   ChibiOS/RT Debug code.
//   31  *
//   32  * @addtogroup debug
//   33  * @details Debug APIs and services:
//   34  *          - Runtime system state and call protocol check. The following
//   35  *            panic messages can be generated:
//   36  *            - SV#1, misplaced @p chSysDisable().
//   37  *            - SV#2, misplaced @p chSysSuspend()
//   38  *            - SV#3, misplaced @p chSysEnable().
//   39  *            - SV#4, misplaced @p chSysLock().
//   40  *            - SV#5, misplaced @p chSysUnlock().
//   41  *            - SV#6, misplaced @p chSysLockFromIsr().
//   42  *            - SV#7, misplaced @p chSysUnlockFromIsr().
//   43  *            - SV#8, misplaced @p CH_IRQ_PROLOGUE().
//   44  *            - SV#9, misplaced @p CH_IRQ_EPILOGUE().
//   45  *            - SV#10, misplaced I-class function.
//   46  *            - SV#11, misplaced S-class function.
//   47  *            .
//   48  *          - Trace buffer.
//   49  *          - Parameters check.
//   50  *          - Kernel assertions.
//   51  *          - Kernel panics.
//   52  *          .
//   53  * @note    Stack checks are not implemented in this module but in the port
//   54  *          layer in an architecture-dependent way.
//   55  * @{
//   56  */
//   57 
//   58 #include "ch.h"
//   59 
//   60 /*===========================================================================*/
//   61 /* System state checker related code and variables.                          */
//   62 /*===========================================================================*/
//   63 
//   64 #if CH_DBG_SYSTEM_STATE_CHECK || defined(__DOXYGEN__)
//   65 
//   66 /**
//   67  * @brief   ISR nesting level.
//   68  */

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   69 cnt_t dbg_isr_cnt;
dbg_isr_cnt:
        DS8 4
//   70 
//   71 /**
//   72  * @brief   Lock nesting level.
//   73  */

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   74 cnt_t dbg_lock_cnt;
dbg_lock_cnt:
        DS8 4
//   75 
//   76 /**
//   77  * @brief   Guard code for @p chSysDisable().
//   78  *
//   79  * @notapi
//   80  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function dbg_check_disable
        THUMB
//   81 void dbg_check_disable(void) {
dbg_check_disable:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   82 
//   83   if ((dbg_isr_cnt != 0) || (dbg_lock_cnt != 0))
        LDR.N    R0,??DataTable13
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable13_1
        LDR      R1,[R1, #+0]
        ORRS     R0,R1,R0
        CMP      R0,#+0
        BEQ.N    ??dbg_check_disable_0
//   84     chDbgPanic("SV#1");
        LDR.N    R0,??DataTable13_2
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//   85 }
??dbg_check_disable_0:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0
//   86 
//   87 /**
//   88  * @brief   Guard code for @p chSysSuspend().
//   89  *
//   90  * @notapi
//   91  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function dbg_check_suspend
        THUMB
//   92 void dbg_check_suspend(void) {
dbg_check_suspend:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   93 
//   94   if ((dbg_isr_cnt != 0) || (dbg_lock_cnt != 0))
        LDR.N    R0,??DataTable13
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable13_1
        LDR      R1,[R1, #+0]
        ORRS     R0,R1,R0
        CMP      R0,#+0
        BEQ.N    ??dbg_check_suspend_0
//   95     chDbgPanic("SV#2");
        LDR.N    R0,??DataTable13_3
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//   96 }
??dbg_check_suspend_0:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock1
//   97 
//   98 /**
//   99  * @brief   Guard code for @p chSysEnable().
//  100  *
//  101  * @notapi
//  102  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function dbg_check_enable
        THUMB
//  103 void dbg_check_enable(void) {
dbg_check_enable:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  104 
//  105   if ((dbg_isr_cnt != 0) || (dbg_lock_cnt != 0))
        LDR.N    R0,??DataTable13
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable13_1
        LDR      R1,[R1, #+0]
        ORRS     R0,R1,R0
        CMP      R0,#+0
        BEQ.N    ??dbg_check_enable_0
//  106     chDbgPanic("SV#3");
        LDR.N    R0,??DataTable13_4
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  107 }
??dbg_check_enable_0:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock2
//  108 
//  109 /**
//  110  * @brief   Guard code for @p chSysLock().
//  111  *
//  112  * @notapi
//  113  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function dbg_check_lock
        THUMB
//  114 void dbg_check_lock(void) {
dbg_check_lock:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  115 
//  116   if ((dbg_isr_cnt != 0) || (dbg_lock_cnt != 0))
        LDR.N    R0,??DataTable13
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable13_1
        LDR      R1,[R1, #+0]
        ORRS     R0,R1,R0
        CMP      R0,#+0
        BEQ.N    ??dbg_check_lock_0
//  117     chDbgPanic("SV#4 misplaced chSysLock()");
        LDR.N    R0,??DataTable13_5
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  118   dbg_enter_lock();
??dbg_check_lock_0:
        LDR.N    R0,??DataTable13_1
        MOVS     R1,#+1
        STR      R1,[R0, #+0]
//  119 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock3
//  120 
//  121 /**
//  122  * @brief   Guard code for @p chSysUnlock().
//  123  *
//  124  * @notapi
//  125  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function dbg_check_unlock
        THUMB
//  126 void dbg_check_unlock(void) {
dbg_check_unlock:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  127 
//  128   if ((dbg_isr_cnt != 0) || (dbg_lock_cnt <= 0))
        LDR.N    R0,??DataTable13
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??dbg_check_unlock_0
        LDR.N    R0,??DataTable13_1
        LDR      R0,[R0, #+0]
        CMP      R0,#+1
        BGE.N    ??dbg_check_unlock_1
//  129     chDbgPanic("SV#5");
??dbg_check_unlock_0:
        LDR.N    R0,??DataTable13_6
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  130   dbg_leave_lock();
??dbg_check_unlock_1:
        LDR.N    R0,??DataTable13_1
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  131 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock4
//  132 
//  133 /**
//  134  * @brief   Guard code for @p chSysLockFromIsr().
//  135  *
//  136  * @notapi
//  137  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function dbg_check_lock_from_isr
        THUMB
//  138 void dbg_check_lock_from_isr(void) {
dbg_check_lock_from_isr:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  139 
//  140   if ((dbg_isr_cnt <= 0) || (dbg_lock_cnt != 0))
        LDR.N    R0,??DataTable13
        LDR      R0,[R0, #+0]
        CMP      R0,#+1
        BLT.N    ??dbg_check_lock_from_isr_0
        LDR.N    R0,??DataTable13_1
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??dbg_check_lock_from_isr_1
//  141     chDbgPanic("SV#6 misplaced chSysLockFromIsr");
??dbg_check_lock_from_isr_0:
        LDR.N    R0,??DataTable13_7
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  142   dbg_enter_lock();
??dbg_check_lock_from_isr_1:
        LDR.N    R0,??DataTable13_1
        MOVS     R1,#+1
        STR      R1,[R0, #+0]
//  143 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock5
//  144 
//  145 /**
//  146  * @brief   Guard code for @p chSysUnlockFromIsr().
//  147  *
//  148  * @notapi
//  149  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function dbg_check_unlock_from_isr
        THUMB
//  150 void dbg_check_unlock_from_isr(void) {
dbg_check_unlock_from_isr:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  151 
//  152   if ((dbg_isr_cnt <= 0) || (dbg_lock_cnt <= 0))
        LDR.N    R0,??DataTable13
        LDR      R0,[R0, #+0]
        CMP      R0,#+1
        BLT.N    ??dbg_check_unlock_from_isr_0
        LDR.N    R0,??DataTable13_1
        LDR      R0,[R0, #+0]
        CMP      R0,#+1
        BGE.N    ??dbg_check_unlock_from_isr_1
//  153     chDbgPanic("SV#7");
??dbg_check_unlock_from_isr_0:
        LDR.N    R0,??DataTable13_8
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  154   dbg_leave_lock();
??dbg_check_unlock_from_isr_1:
        LDR.N    R0,??DataTable13_1
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  155 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock6
//  156 
//  157 void firmwareError(const char *fmt, ...);
//  158 
//  159 extern int maxNesting;
//  160 
//  161 /**
//  162  * @brief   Guard code for @p CH_IRQ_PROLOGUE().
//  163  *
//  164  * @notapi
//  165  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function dbg_check_enter_isr
        THUMB
//  166 void dbg_check_enter_isr(void) {
dbg_check_enter_isr:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  167   port_lock_from_isr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
//  168   if ((dbg_isr_cnt < 0) || (dbg_lock_cnt != 0))
        LDR.N    R0,??DataTable13
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BMI.N    ??dbg_check_enter_isr_0
        LDR.N    R0,??DataTable13_1
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??dbg_check_enter_isr_1
//  169     chDbgPanic("SV#8");
??dbg_check_enter_isr_0:
        LDR.N    R0,??DataTable13_9
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  170   dbg_isr_cnt++;
??dbg_check_enter_isr_1:
        LDR.N    R0,??DataTable13
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable13
        STR      R0,[R1, #+0]
//  171   if (dbg_isr_cnt > maxNesting)
        LDR.N    R0,??DataTable13_10
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable13
        LDR      R1,[R1, #+0]
        CMP      R0,R1
        BGE.N    ??dbg_check_enter_isr_2
//  172 	  maxNesting = dbg_isr_cnt;
        LDR.N    R0,??DataTable13_10
        LDR.N    R1,??DataTable13
        LDR      R1,[R1, #+0]
        STR      R1,[R0, #+0]
//  173   port_unlock_from_isr();
??dbg_check_enter_isr_2:
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  174 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock7
//  175 
//  176 /**
//  177  * @brief   Guard code for @p CH_IRQ_EPILOGUE().
//  178  *
//  179  * @notapi
//  180  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function dbg_check_leave_isr
        THUMB
//  181 void dbg_check_leave_isr(void) {
dbg_check_leave_isr:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  182 
//  183   port_lock_from_isr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
//  184   if ((dbg_isr_cnt <= 0) || (dbg_lock_cnt != 0))
        LDR.N    R0,??DataTable13
        LDR      R0,[R0, #+0]
        CMP      R0,#+1
        BLT.N    ??dbg_check_leave_isr_0
        LDR.N    R0,??DataTable13_1
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??dbg_check_leave_isr_1
//  185     chDbgPanic("SV#9");
??dbg_check_leave_isr_0:
        LDR.N    R0,??DataTable13_11
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  186   dbg_isr_cnt--;
??dbg_check_leave_isr_1:
        LDR.N    R0,??DataTable13
        LDR      R0,[R0, #+0]
        SUBS     R0,R0,#+1
        LDR.N    R1,??DataTable13
        STR      R0,[R1, #+0]
//  187   port_unlock_from_isr();
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  188 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock8
//  189 
//  190 /**
//  191  * @brief   I-class functions context check.
//  192  * @details Verifies that the system is in an appropriate state for invoking
//  193  *          an I-class API function. A panic is generated if the state is
//  194  *          not compatible.
//  195  *
//  196  * @api
//  197  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function chDbgCheckClassI
        THUMB
//  198 void chDbgCheckClassI(void) {
chDbgCheckClassI:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  199 
//  200   if ((dbg_isr_cnt < 0) || (dbg_lock_cnt <= 0))
        LDR.N    R0,??DataTable13
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BMI.N    ??chDbgCheckClassI_0
        LDR.N    R0,??DataTable13_1
        LDR      R0,[R0, #+0]
        CMP      R0,#+1
        BGE.N    ??chDbgCheckClassI_1
//  201     chDbgPanic("SV#10 misplaced I-class function");
??chDbgCheckClassI_0:
        LDR.N    R0,??DataTable13_12
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  202 }
??chDbgCheckClassI_1:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock9
//  203 
//  204 /**
//  205  * @brief   S-class functions context check.
//  206  * @details Verifies that the system is in an appropriate state for invoking
//  207  *          an S-class API function. A panic is generated if the state is
//  208  *          not compatible.
//  209  *
//  210  * @api
//  211  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function chDbgCheckClassS
        THUMB
//  212 void chDbgCheckClassS(void) {
chDbgCheckClassS:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  213 
//  214   if ((dbg_isr_cnt != 0) || (dbg_lock_cnt <= 0))
        LDR.N    R0,??DataTable13
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??chDbgCheckClassS_0
        LDR.N    R0,??DataTable13_1
        LDR      R0,[R0, #+0]
        CMP      R0,#+1
        BGE.N    ??chDbgCheckClassS_1
//  215     chDbgPanic("SV#11");
??chDbgCheckClassS_0:
        LDR.N    R0,??DataTable13_13
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  216 }
??chDbgCheckClassS_1:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock10
//  217 
//  218 #endif /* CH_DBG_SYSTEM_STATE_CHECK */
//  219 
//  220 /*===========================================================================*/
//  221 /* Trace related code and variables.                                         */
//  222 /*===========================================================================*/
//  223 
//  224 #if CH_DBG_ENABLE_TRACE || defined(__DOXYGEN__)
//  225 /**
//  226  * @brief   Public trace buffer.
//  227  */

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  228 ch_trace_buffer_t dbg_trace_buffer;
dbg_trace_buffer:
        DS8 1032
//  229 
//  230 /**
//  231  * @brief   Trace circular buffer subsystem initialization.
//  232  * @note    Internal use only.
//  233  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _trace_init
          CFI NoCalls
        THUMB
//  234 void _trace_init(void) {
//  235 
//  236   dbg_trace_buffer.tb_size = CH_TRACE_BUFFER_SIZE;
_trace_init:
        LDR.N    R0,??DataTable13_14
        MOVS     R1,#+64
        STR      R1,[R0, #+0]
//  237   dbg_trace_buffer.tb_ptr = &dbg_trace_buffer.tb_buffer[0];
        LDR.N    R0,??DataTable13_14
        LDR.N    R1,??DataTable13_15
        STR      R1,[R0, #+4]
//  238 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock11
//  239 
//  240 /**
//  241  * @brief   Inserts in the circular debug trace buffer a context switch record.
//  242  *
//  243  * @param[in] otp       the thread being switched out
//  244  *
//  245  * @notapi
//  246  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function dbg_trace
          CFI NoCalls
        THUMB
//  247 void dbg_trace(Thread *otp) {
//  248 
//  249   dbg_trace_buffer.tb_ptr->se_time   = chTimeNow();
dbg_trace:
        LDR.N    R1,??DataTable13_14
        LDR      R1,[R1, #+4]
        LDR.N    R2,??DataTable13_16
        LDR      R2,[R2, #+12]
        STR      R2,[R1, #+0]
//  250   dbg_trace_buffer.tb_ptr->se_tp     = currp;
        LDR.N    R1,??DataTable13_14
        LDR      R1,[R1, #+4]
        LDR.N    R2,??DataTable13_17
        LDR      R2,[R2, #+24]
        STR      R2,[R1, #+4]
//  251   dbg_trace_buffer.tb_ptr->se_wtobjp = otp->p_u.wtobjp;
        LDR.N    R1,??DataTable13_14
        LDR      R1,[R1, #+4]
        LDR      R2,[R0, #+40]
        STR      R2,[R1, #+8]
//  252   dbg_trace_buffer.tb_ptr->se_state  = (uint8_t)otp->p_state;
        LDR.N    R1,??DataTable13_14
        LDR      R1,[R1, #+4]
        LDRB     R2,[R0, #+32]
        STRB     R2,[R1, #+12]
//  253   if (++dbg_trace_buffer.tb_ptr >=
//  254       &dbg_trace_buffer.tb_buffer[CH_TRACE_BUFFER_SIZE])
        LDR.N    R1,??DataTable13_14
        LDR      R1,[R1, #+4]
        ADDS     R1,R1,#+16
        LDR.N    R2,??DataTable13_14
        STR      R1,[R2, #+4]
        LDR.N    R2,??DataTable13_18
        CMP      R1,R2
        BCC.N    ??dbg_trace_0
//  255     dbg_trace_buffer.tb_ptr = &dbg_trace_buffer.tb_buffer[0];
        LDR.N    R1,??DataTable13_14
        LDR.N    R2,??DataTable13_15
        STR      R2,[R1, #+4]
//  256 }
??dbg_trace_0:
        BX       LR               ;; return
          CFI EndBlock cfiBlock12
//  257 #endif /* CH_DBG_ENABLE_TRACE */
//  258 
//  259 /*===========================================================================*/
//  260 /* Panic related code and variables.                                         */
//  261 /*===========================================================================*/
//  262 
//  263 #if CH_DBG_ENABLED || defined(__DOXYGEN__)
//  264 /**
//  265  * @brief   Pointer to the panic message.
//  266  * @details This pointer is meant to be accessed through the debugger, it is
//  267  *          written once and then the system is halted.
//  268  */

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  269 const char *dbg_panic_msg;
dbg_panic_msg:
        DS8 4
//  270 
//  271 /**
//  272  * @brief   Prints a panic message on the console and then halts the system.
//  273  *
//  274  * @param[in] msg       the pointer to the panic message string
//  275  */
//  276 
//  277 void chDbgPanic3(const char *msg, const char * file, int line);
//  278 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function chDbgPanic
        THUMB
//  279 void chDbgPanic(const char *msg) {
chDbgPanic:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  280 	chDbgPanic3(msg, __FILE__, __LINE__);
        MOV      R2,#+280
        LDR.N    R1,??DataTable13_19
        MOVS     R0,R4
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  281 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock13

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13:
        DC32     dbg_isr_cnt

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_1:
        DC32     dbg_lock_cnt

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_2:
        DC32     `?<Constant "SV#1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_3:
        DC32     `?<Constant "SV#2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_4:
        DC32     `?<Constant "SV#3">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_5:
        DC32     `?<Constant "SV#4 misplaced chSysL...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_6:
        DC32     `?<Constant "SV#5">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_7:
        DC32     `?<Constant "SV#6 misplaced chSysL...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_8:
        DC32     `?<Constant "SV#7">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_9:
        DC32     `?<Constant "SV#8">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_10:
        DC32     maxNesting

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_11:
        DC32     `?<Constant "SV#9">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_12:
        DC32     `?<Constant "SV#10 misplaced I-cla...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_13:
        DC32     `?<Constant "SV#11">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_14:
        DC32     dbg_trace_buffer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_15:
        DC32     dbg_trace_buffer+0x8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_16:
        DC32     vtlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_17:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_18:
        DC32     dbg_trace_buffer+0x408

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_19:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SV#1">`:
        DATA
        DC8 "SV#1"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SV#2">`:
        DATA
        DC8 "SV#2"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SV#3">`:
        DATA
        DC8 "SV#3"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SV#4 misplaced chSysL...">`:
        DATA
        DC8 "SV#4 misplaced chSysLock()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SV#5">`:
        DATA
        DC8 "SV#5"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SV#6 misplaced chSysL...">`:
        DATA
        DC8 "SV#6 misplaced chSysLockFromIsr"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SV#7">`:
        DATA
        DC8 "SV#7"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SV#8">`:
        DATA
        DC8 "SV#8"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SV#9">`:
        DATA
        DC8 "SV#9"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SV#10 misplaced I-cla...">`:
        DATA
        DC8 "SV#10 misplaced I-class function"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SV#11">`:
        DATA
        DC8 "SV#11"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`:
        DATA
        DC8 46H, 3AH, 5CH, 73H, 74H, 75H, 66H, 66H
        DC8 5CH, 72H, 75H, 73H, 65H, 66H, 69H, 5FH
        DC8 73H, 6FH, 75H, 72H, 63H, 65H, 66H, 6FH
        DC8 72H, 67H, 65H, 5CH, 66H, 69H, 72H, 6DH
        DC8 77H, 61H, 72H, 65H, 5CH, 63H, 68H, 69H
        DC8 62H, 69H, 6FH, 73H, 5CH, 6FH, 73H, 5CH
        DC8 6BH, 65H, 72H, 6EH, 65H, 6CH, 5CH, 73H
        DC8 72H, 63H, 5CH, 63H, 68H, 64H, 65H, 62H
        DC8 75H, 67H, 2EH, 63H, 0
        DC8 0, 0, 0

        END
//  282 #endif /* CH_DBG_ENABLED */
//  283 
//  284 /** @} */
// 
// 1 044 bytes in section .bss
//   232 bytes in section .rodata
//   540 bytes in section .text
// 
//   540 bytes of CODE  memory
//   232 bytes of CONST memory
// 1 044 bytes of DATA  memory
//
//Errors: none
//Warnings: none
