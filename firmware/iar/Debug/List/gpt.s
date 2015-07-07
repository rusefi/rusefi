///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:05 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\gpt.c                                            /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\gpt.c -lCN F:\stuff\rusefi_sourceforge\firmware\ /
//                       iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge\firm /
//                       ware\iar\Debug\List\ -o F:\stuff\rusefi_sourceforge\ /
//                       firmware\iar\Debug\Obj\ --no_cse --no_unroll         /
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
//                       -On                                                  /
//    MISRA C version =  2004                                                 /
//      Enabled       =  1.2-1.4 2.3 3.4 4.2 6.5 7 8.2 8.3 9.3 12.3 13.6      /
//                       13.7 15.5 17.3 19.17 20.7-20.9 20.11                 /
//      Checked       =  1.4 2.3 4.2 6.5 7 8.2 8.3 9.3 12.3 13.7 15.5         /
//                       20.7-20.9 20.11                                      /
//      Not checked   =  1.1-1.3 1.5-2.2 2.4-4.1 5-6.4 8.1 8.4-9.2 10-12.2    /
//                       12.4-13.6 14-15.4 16-20.6 20.10 20.12 21             /
//    List file       =  F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                       gpt.s                                                /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME gpt

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN chDbgCheckClassI
        EXTERN chDbgPanic
        EXTERN chDbgPanic3
        EXTERN dbg_check_lock
        EXTERN dbg_check_unlock
        EXTERN gpt_lld_init
        EXTERN gpt_lld_polled_delay
        EXTERN gpt_lld_start
        EXTERN gpt_lld_start_timer
        EXTERN gpt_lld_stop
        EXTERN gpt_lld_stop_timer

        PUBLIC gptChangeInterval
        PUBLIC gptInit
        PUBLIC gptObjectInit
        PUBLIC gptPolledDelay
        PUBLIC gptStart
        PUBLIC gptStartContinuous
        PUBLIC gptStartContinuousI
        PUBLIC gptStartOneShot
        PUBLIC gptStartOneShotI
        PUBLIC gptStop
        PUBLIC gptStopTimer
        PUBLIC gptStopTimerI
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\src\gpt.c
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
//   29  * @file    gpt.c
//   30  * @brief   GPT Driver code.
//   31  *
//   32  * @addtogroup GPT
//   33  * @{
//   34  */
//   35 
//   36 #include "ch.h"
//   37 #include "hal.h"
//   38 
//   39 #if HAL_USE_GPT || defined(__DOXYGEN__)
//   40 
//   41 /*===========================================================================*/
//   42 /* Driver local definitions.                                                 */
//   43 /*===========================================================================*/
//   44 
//   45 /*===========================================================================*/
//   46 /* Driver exported variables.                                                */
//   47 /*===========================================================================*/
//   48 
//   49 /*===========================================================================*/
//   50 /* Driver local variables and types.                                         */
//   51 /*===========================================================================*/
//   52 
//   53 /*===========================================================================*/
//   54 /* Driver local functions.                                                   */
//   55 /*===========================================================================*/
//   56 
//   57 /*===========================================================================*/
//   58 /* Driver exported functions.                                                */
//   59 /*===========================================================================*/
//   60 
//   61 /**
//   62  * @brief   GPT Driver initialization.
//   63  * @note    This function is implicitly invoked by @p halInit(), there is
//   64  *          no need to explicitly initialize the driver.
//   65  *
//   66  * @init
//   67  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function gptInit
        THUMB
//   68 void gptInit(void) {
gptInit:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   69 
//   70   gpt_lld_init();
          CFI FunCall gpt_lld_init
        BL       gpt_lld_init
//   71 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0
//   72 
//   73 /**
//   74  * @brief   Initializes the standard part of a @p GPTDriver structure.
//   75  *
//   76  * @param[out] gptp     pointer to the @p GPTDriver object
//   77  *
//   78  * @init
//   79  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function gptObjectInit
          CFI NoCalls
        THUMB
//   80 void gptObjectInit(GPTDriver *gptp) {
//   81 
//   82   gptp->state  = GPT_STOP;
gptObjectInit:
        MOVS     R1,#+1
        STRB     R1,[R0, #+0]
//   83   gptp->config = NULL;
        MOVS     R1,#+0
        STR      R1,[R0, #+4]
//   84 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   85 
//   86 /**
//   87  * @brief   Configures and activates the GPT peripheral.
//   88  *
//   89  * @param[in] gptp      pointer to the @p GPTDriver object
//   90  * @param[in] config    pointer to the @p GPTConfig object
//   91  *
//   92  * @api
//   93  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function gptStart
        THUMB
//   94 void gptStart(GPTDriver *gptp, const GPTConfig *config) {
gptStart:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   95 
//   96   chDbgCheck((gptp != NULL) && (config != NULL), "gptStart");
        CMP      R4,#+0
        BEQ.N    ??gptStart_0
        CMP      R5,#+0
        BNE.N    ??gptStart_1
??gptStart_0:
        MOVS     R2,#+96
        LDR.N    R1,??DataTable6
        LDR.N    R0,??DataTable6_1
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//   97 
//   98   chSysLock();
??gptStart_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//   99   chDbgAssert((gptp->state == GPT_STOP) || (gptp->state == GPT_READY),
//  100               "gptStart(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BEQ.N    ??gptStart_2
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??gptStart_2
        LDR.N    R0,??DataTable6_2
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  101   gptp->config = config;
??gptStart_2:
        STR      R5,[R4, #+4]
//  102   gpt_lld_start(gptp);
        MOVS     R0,R4
          CFI FunCall gpt_lld_start
        BL       gpt_lld_start
//  103   gptp->state = GPT_READY;
        MOVS     R0,#+2
        STRB     R0,[R4, #+0]
//  104   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  105 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2
//  106 
//  107 /**
//  108  * @brief   Deactivates the GPT peripheral.
//  109  *
//  110  * @param[in] gptp      pointer to the @p GPTDriver object
//  111  *
//  112  * @api
//  113  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function gptStop
        THUMB
//  114 void gptStop(GPTDriver *gptp) {
gptStop:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  115 
//  116   chDbgCheck(gptp != NULL, "gptStop");
        CMP      R4,#+0
        BNE.N    ??gptStop_0
        MOVS     R2,#+116
        LDR.N    R1,??DataTable6
        LDR.N    R0,??DataTable6_3
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  117 
//  118   chSysLock();
??gptStop_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  119   chDbgAssert((gptp->state == GPT_STOP) || (gptp->state == GPT_READY),
//  120               "gptStop(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BEQ.N    ??gptStop_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??gptStop_1
        LDR.N    R0,??DataTable6_4
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  121   gpt_lld_stop(gptp);
??gptStop_1:
        MOVS     R0,R4
          CFI FunCall gpt_lld_stop
        BL       gpt_lld_stop
//  122   gptp->state = GPT_STOP;
        MOVS     R0,#+1
        STRB     R0,[R4, #+0]
//  123   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  124 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//  125 
//  126 /**
//  127  * @brief   Changes the interval of GPT peripheral.
//  128  * @details This function changes the interval of a running GPT unit.
//  129  * @pre     The GPT unit must have been activated using @p gptStart().
//  130  * @pre     The GPT unit must have been running in continuous mode using
//  131  *          @p gptStartContinuous().
//  132  * @post    The GPT unit interval is changed to the new value.
//  133  *
//  134  * @param[in] gptp      pointer to a @p GPTDriver object
//  135  * @param[in] interval  new cycle time in timer ticks
//  136  *
//  137  * @api
//  138  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function gptChangeInterval
        THUMB
//  139 void gptChangeInterval(GPTDriver *gptp, gptcnt_t interval) {
gptChangeInterval:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  140 
//  141   chDbgCheck(gptp != NULL, "gptChangeInterval");
        CMP      R4,#+0
        BNE.N    ??gptChangeInterval_0
        MOVS     R2,#+141
        LDR.N    R1,??DataTable6
        LDR.N    R0,??DataTable6_5
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  142 
//  143   chSysLock();
??gptChangeInterval_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  144   chDbgAssert(gptp->state == GPT_CONTINUOUS,
//  145               "gptChangeInterval(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+3
        BEQ.N    ??gptChangeInterval_1
        LDR.N    R0,??DataTable6_6
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  146   gptChangeIntervalI(gptp, interval);
??gptChangeInterval_1:
        LDR      R0,[R4, #+12]
        SUBS     R1,R5,#+1
        STR      R1,[R0, #+44]
//  147   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  148 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock4
//  149 
//  150 /**
//  151  * @brief   Starts the timer in continuous mode.
//  152  *
//  153  * @param[in] gptp      pointer to the @p GPTDriver object
//  154  * @param[in] interval  period in ticks
//  155  *
//  156  * @api
//  157  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function gptStartContinuous
        THUMB
//  158 void gptStartContinuous(GPTDriver *gptp, gptcnt_t interval) {
gptStartContinuous:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  159 
//  160   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  161   gptStartContinuousI(gptp, interval);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall gptStartContinuousI
        BL       gptStartContinuousI
//  162   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  163 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock5
//  164 
//  165 /**
//  166  * @brief   Starts the timer in continuous mode.
//  167  *
//  168  * @param[in] gptp      pointer to the @p GPTDriver object
//  169  * @param[in] interval  period in ticks
//  170  *
//  171  * @iclass
//  172  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function gptStartContinuousI
        THUMB
//  173 void gptStartContinuousI(GPTDriver *gptp, gptcnt_t interval) {
gptStartContinuousI:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  174 
//  175   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  176   chDbgCheck(gptp != NULL, "gptStartContinuousI");
        CMP      R4,#+0
        BNE.N    ??gptStartContinuousI_0
        MOVS     R2,#+176
        LDR.N    R1,??DataTable6
        LDR.N    R0,??DataTable6_7
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  177   chDbgAssert(gptp->state == GPT_READY,
//  178               "gptStartContinuousI(), #1", "invalid state");
??gptStartContinuousI_0:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??gptStartContinuousI_1
        LDR.N    R0,??DataTable6_8
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  179 
//  180   gptp->state = GPT_CONTINUOUS;
??gptStartContinuousI_1:
        MOVS     R0,#+3
        STRB     R0,[R4, #+0]
//  181   gpt_lld_start_timer(gptp, interval);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall gpt_lld_start_timer
        BL       gpt_lld_start_timer
//  182 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock6
//  183 
//  184 /**
//  185  * @brief   Starts the timer in one shot mode.
//  186  *
//  187  * @param[in] gptp      pointer to the @p GPTDriver object
//  188  * @param[in] interval  time interval in ticks
//  189  *
//  190  * @api
//  191  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function gptStartOneShot
        THUMB
//  192 void gptStartOneShot(GPTDriver *gptp, gptcnt_t interval) {
gptStartOneShot:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  193 
//  194   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  195   gptStartOneShotI(gptp, interval);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall gptStartOneShotI
        BL       gptStartOneShotI
//  196   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  197 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock7
//  198 
//  199 /**
//  200  * @brief   Starts the timer in one shot mode.
//  201  *
//  202  * @param[in] gptp      pointer to the @p GPTDriver object
//  203  * @param[in] interval  time interval in ticks
//  204  *
//  205  * @api
//  206  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function gptStartOneShotI
        THUMB
//  207 void gptStartOneShotI(GPTDriver *gptp, gptcnt_t interval) {
gptStartOneShotI:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  208 
//  209   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  210   chDbgCheck(gptp != NULL, "gptStartOneShotI");
        CMP      R4,#+0
        BNE.N    ??gptStartOneShotI_0
        MOVS     R2,#+210
        LDR.N    R1,??DataTable6
        LDR.N    R0,??DataTable6_9
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  211   chDbgAssert(gptp->state == GPT_READY,
//  212               "gptStartOneShotI(), #1", "invalid state");
??gptStartOneShotI_0:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??gptStartOneShotI_1
        LDR.N    R0,??DataTable6_10
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  213 
//  214   gptp->state = GPT_ONESHOT;
??gptStartOneShotI_1:
        MOVS     R0,#+4
        STRB     R0,[R4, #+0]
//  215   gpt_lld_start_timer(gptp, interval);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall gpt_lld_start_timer
        BL       gpt_lld_start_timer
//  216 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock8
//  217 
//  218 /**
//  219  * @brief   Stops the timer.
//  220  *
//  221  * @param[in] gptp      pointer to the @p GPTDriver object
//  222  *
//  223  * @api
//  224  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function gptStopTimer
        THUMB
//  225 void gptStopTimer(GPTDriver *gptp) {
gptStopTimer:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  226 
//  227   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  228   gptStopTimerI(gptp);
        MOVS     R0,R4
          CFI FunCall gptStopTimerI
        BL       gptStopTimerI
//  229   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  230 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock9
//  231 
//  232 /**
//  233  * @brief   Stops the timer.
//  234  *
//  235  * @param[in] gptp      pointer to the @p GPTDriver object
//  236  *
//  237  * @api
//  238  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function gptStopTimerI
        THUMB
//  239 void gptStopTimerI(GPTDriver *gptp) {
gptStopTimerI:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  240 
//  241   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  242   chDbgCheck(gptp != NULL, "gptStopTimerI");
        CMP      R4,#+0
        BNE.N    ??gptStopTimerI_0
        MOVS     R2,#+242
        LDR.N    R1,??DataTable6
        LDR.N    R0,??DataTable6_11
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  243   chDbgAssert((gptp->state == GPT_READY) || (gptp->state == GPT_CONTINUOUS) ||
//  244               (gptp->state == GPT_ONESHOT),
//  245               "gptStopTimerI(), #1", "invalid state");
??gptStopTimerI_0:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??gptStopTimerI_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+3
        BEQ.N    ??gptStopTimerI_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+4
        BEQ.N    ??gptStopTimerI_1
        LDR.N    R0,??DataTable6_12
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  246 
//  247   gptp->state = GPT_READY;
??gptStopTimerI_1:
        MOVS     R0,#+2
        STRB     R0,[R4, #+0]
//  248   gpt_lld_stop_timer(gptp);
        MOVS     R0,R4
          CFI FunCall gpt_lld_stop_timer
        BL       gpt_lld_stop_timer
//  249 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock10
//  250 
//  251 /**
//  252  * @brief   Starts the timer in one shot mode and waits for completion.
//  253  * @details This function specifically polls the timer waiting for completion
//  254  *          in order to not have extra delays caused by interrupt servicing,
//  255  *          this function is only recommended for short delays.
//  256  * @note    The configured callback is not invoked when using this function.
//  257  *
//  258  * @param[in] gptp      pointer to the @p GPTDriver object
//  259  * @param[in] interval  time interval in ticks
//  260  *
//  261  * @api
//  262  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function gptPolledDelay
        THUMB
//  263 void gptPolledDelay(GPTDriver *gptp, gptcnt_t interval) {
gptPolledDelay:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  264 
//  265   chDbgAssert(gptp->state == GPT_READY,
//  266               "gptPolledDelay(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??gptPolledDelay_0
        LDR.N    R0,??DataTable6_13
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  267 
//  268   gptp->state = GPT_ONESHOT;
??gptPolledDelay_0:
        MOVS     R0,#+4
        STRB     R0,[R4, #+0]
//  269   gpt_lld_polled_delay(gptp, interval);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall gpt_lld_polled_delay
        BL       gpt_lld_polled_delay
//  270   gptp->state = GPT_READY;
        MOVS     R0,#+2
        STRB     R0,[R4, #+0]
//  271 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock11

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_1:
        DC32     `?<Constant "\\"gptStart\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_2:
        DC32     `?<Constant "gptStart(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_3:
        DC32     `?<Constant "\\"gptStop\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_4:
        DC32     `?<Constant "gptStop(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_5:
        DC32     `?<Constant "\\"gptChangeInterval\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_6:
        DC32     `?<Constant "gptChangeInterval(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_7:
        DC32     `?<Constant "\\"gptStartContinuousI\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_8:
        DC32     `?<Constant "gptStartContinuousI()...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_9:
        DC32     `?<Constant "\\"gptStartOneShotI\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_10:
        DC32     `?<Constant "gptStartOneShotI(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_11:
        DC32     `?<Constant "\\"gptStopTimerI\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_12:
        DC32     `?<Constant "gptStopTimerI(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_13:
        DC32     `?<Constant "gptPolledDelay(), #1">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"gptStart\\"()">`:
        DATA
        DC8 "\"gptStart\"()"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`:
        DATA
        DC8 46H, 3AH, 5CH, 73H, 74H, 75H, 66H, 66H
        DC8 5CH, 72H, 75H, 73H, 65H, 66H, 69H, 5FH
        DC8 73H, 6FH, 75H, 72H, 63H, 65H, 66H, 6FH
        DC8 72H, 67H, 65H, 5CH, 66H, 69H, 72H, 6DH
        DC8 77H, 61H, 72H, 65H, 5CH, 63H, 68H, 69H
        DC8 62H, 69H, 6FH, 73H, 5CH, 6FH, 73H, 5CH
        DC8 68H, 61H, 6CH, 5CH, 73H, 72H, 63H, 5CH
        DC8 67H, 70H, 74H, 2EH, 63H, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "gptStart(), #1">`:
        DATA
        DC8 "gptStart(), #1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"gptStop\\"()">`:
        DATA
        DC8 "\"gptStop\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "gptStop(), #1">`:
        DATA
        DC8 "gptStop(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"gptChangeInterval\\"()">`:
        DATA
        DC8 "\"gptChangeInterval\"()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "gptChangeInterval(), #1">`:
        DATA
        DC8 "gptChangeInterval(), #1"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"gptStartContinuousI\\"()">`:
        DATA
        DC8 "\"gptStartContinuousI\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "gptStartContinuousI()...">`:
        DATA
        DC8 "gptStartContinuousI(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"gptStartOneShotI\\"()">`:
        DATA
        DC8 "\"gptStartOneShotI\"()"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "gptStartOneShotI(), #1">`:
        DATA
        DC8 "gptStartOneShotI(), #1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"gptStopTimerI\\"()">`:
        DATA
        DC8 "\"gptStopTimerI\"()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "gptStopTimerI(), #1">`:
        DATA
        DC8 "gptStopTimerI(), #1"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "gptPolledDelay(), #1">`:
        DATA
        DC8 "gptPolledDelay(), #1"
        DC8 0, 0, 0

        END
//  272 
//  273 #endif /* HAL_USE_GPT */
//  274 
//  275 /** @} */
// 
// 336 bytes in section .rodata
// 576 bytes in section .text
// 
// 576 bytes of CODE  memory
// 336 bytes of CONST memory
//
//Errors: none
//Warnings: none
