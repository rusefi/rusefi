///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:01 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\icu.c                                            /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\icu.c -lCN F:\stuff\rusefi_sourceforge\firmware\ /
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
//                       icu.s                                                /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME icu

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN chDbgPanic
        EXTERN chDbgPanic3
        EXTERN dbg_check_lock
        EXTERN dbg_check_unlock
        EXTERN icu_lld_disable
        EXTERN icu_lld_enable
        EXTERN icu_lld_init
        EXTERN icu_lld_start
        EXTERN icu_lld_stop

        PUBLIC icuDisable
        PUBLIC icuEnable
        PUBLIC icuInit
        PUBLIC icuObjectInit
        PUBLIC icuStart
        PUBLIC icuStop
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\src\icu.c
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
//   29  * @file    icu.c
//   30  * @brief   ICU Driver code.
//   31  *
//   32  * @addtogroup ICU
//   33  * @{
//   34  */
//   35 
//   36 #include "ch.h"
//   37 #include "hal.h"
//   38 
//   39 #if HAL_USE_ICU || defined(__DOXYGEN__)
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
//   62  * @brief   ICU Driver initialization.
//   63  * @note    This function is implicitly invoked by @p halInit(), there is
//   64  *          no need to explicitly initialize the driver.
//   65  *
//   66  * @init
//   67  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function icuInit
        THUMB
//   68 void icuInit(void) {
icuInit:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   69 
//   70   icu_lld_init();
          CFI FunCall icu_lld_init
        BL       icu_lld_init
//   71 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0
//   72 
//   73 /**
//   74  * @brief   Initializes the standard part of a @p ICUDriver structure.
//   75  *
//   76  * @param[out] icup     pointer to the @p ICUDriver object
//   77  *
//   78  * @init
//   79  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function icuObjectInit
          CFI NoCalls
        THUMB
//   80 void icuObjectInit(ICUDriver *icup) {
//   81 
//   82   icup->state  = ICU_STOP;
icuObjectInit:
        MOVS     R1,#+1
        STRB     R1,[R0, #+0]
//   83   icup->config = NULL;
        MOVS     R1,#+0
        STR      R1,[R0, #+4]
//   84 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   85 
//   86 /**
//   87  * @brief   Configures and activates the ICU peripheral.
//   88  *
//   89  * @param[in] icup      pointer to the @p ICUDriver object
//   90  * @param[in] config    pointer to the @p ICUConfig object
//   91  *
//   92  * @api
//   93  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function icuStart
        THUMB
//   94 void icuStart(ICUDriver *icup, const ICUConfig *config) {
icuStart:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   95 
//   96   chDbgCheck((icup != NULL) && (config != NULL), "icuStart");
        CMP      R4,#+0
        BEQ.N    ??icuStart_0
        CMP      R5,#+0
        BNE.N    ??icuStart_1
??icuStart_0:
        MOVS     R2,#+96
        LDR.N    R1,??DataTable3
        LDR.N    R0,??DataTable3_1
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//   97 
//   98   chSysLock();
??icuStart_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//   99   chDbgAssert((icup->state == ICU_STOP) || (icup->state == ICU_READY),
//  100               "icuStart(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BEQ.N    ??icuStart_2
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??icuStart_2
        LDR.N    R0,??DataTable3_2
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  101   icup->config = config;
??icuStart_2:
        STR      R5,[R4, #+4]
//  102   icu_lld_start(icup);
        MOVS     R0,R4
          CFI FunCall icu_lld_start
        BL       icu_lld_start
//  103   icup->state = ICU_READY;
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
//  108  * @brief   Deactivates the ICU peripheral.
//  109  *
//  110  * @param[in] icup      pointer to the @p ICUDriver object
//  111  *
//  112  * @api
//  113  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function icuStop
        THUMB
//  114 void icuStop(ICUDriver *icup) {
icuStop:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  115 
//  116   chDbgCheck(icup != NULL, "icuStop");
        CMP      R4,#+0
        BNE.N    ??icuStop_0
        MOVS     R2,#+116
        LDR.N    R1,??DataTable3
        LDR.N    R0,??DataTable3_3
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  117 
//  118   chSysLock();
??icuStop_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  119   chDbgAssert((icup->state == ICU_STOP) || (icup->state == ICU_READY),
//  120               "icuStop(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BEQ.N    ??icuStop_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??icuStop_1
        LDR.N    R0,??DataTable3_4
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  121   icu_lld_stop(icup);
??icuStop_1:
        MOVS     R0,R4
          CFI FunCall icu_lld_stop
        BL       icu_lld_stop
//  122   icup->state = ICU_STOP;
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
//  127  * @brief   Enables the input capture.
//  128  *
//  129  * @param[in] icup      pointer to the @p ICUDriver object
//  130  *
//  131  * @api
//  132  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function icuEnable
        THUMB
//  133 void icuEnable(ICUDriver *icup) {
icuEnable:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  134 
//  135   chDbgCheck(icup != NULL, "icuEnable");
        CMP      R4,#+0
        BNE.N    ??icuEnable_0
        MOVS     R2,#+135
        LDR.N    R1,??DataTable3
        LDR.N    R0,??DataTable3_5
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  136 
//  137   chSysLock();
??icuEnable_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  138   chDbgAssert(icup->state == ICU_READY, "icuEnable(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??icuEnable_1
        LDR.N    R0,??DataTable3_6
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  139   icu_lld_enable(icup);
??icuEnable_1:
        MOVS     R0,R4
          CFI FunCall icu_lld_enable
        BL       icu_lld_enable
//  140   icup->state = ICU_WAITING;
        MOVS     R0,#+3
        STRB     R0,[R4, #+0]
//  141   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  142 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock4
//  143 
//  144 /**
//  145  * @brief   Disables the input capture.
//  146  *
//  147  * @param[in] icup      pointer to the @p ICUDriver object
//  148  *
//  149  * @api
//  150  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function icuDisable
        THUMB
//  151 void icuDisable(ICUDriver *icup) {
icuDisable:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  152 
//  153   chDbgCheck(icup != NULL, "icuDisable");
        CMP      R4,#+0
        BNE.N    ??icuDisable_0
        MOVS     R2,#+153
        LDR.N    R1,??DataTable3
        LDR.N    R0,??DataTable3_7
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  154 
//  155   chSysLock();
??icuDisable_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  156   chDbgAssert((icup->state == ICU_READY) || (icup->state == ICU_WAITING) ||
//  157               (icup->state == ICU_ACTIVE) || (icup->state == ICU_IDLE),
//  158               "icuDisable(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??icuDisable_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+3
        BEQ.N    ??icuDisable_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+4
        BEQ.N    ??icuDisable_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+5
        BEQ.N    ??icuDisable_1
        LDR.N    R0,??DataTable3_8
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  159   icu_lld_disable(icup);
??icuDisable_1:
        MOVS     R0,R4
          CFI FunCall icu_lld_disable
        BL       icu_lld_disable
//  160   icup->state = ICU_READY;
        MOVS     R0,#+2
        STRB     R0,[R4, #+0]
//  161   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  162 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock5

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_1:
        DC32     `?<Constant "\\"icuStart\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_2:
        DC32     `?<Constant "icuStart(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_3:
        DC32     `?<Constant "\\"icuStop\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_4:
        DC32     `?<Constant "icuStop(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_5:
        DC32     `?<Constant "\\"icuEnable\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_6:
        DC32     `?<Constant "icuEnable(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_7:
        DC32     `?<Constant "\\"icuDisable\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_8:
        DC32     `?<Constant "icuDisable(), #1">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"icuStart\\"()">`:
        DATA
        DC8 "\"icuStart\"()"
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
        DC8 69H, 63H, 75H, 2EH, 63H, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "icuStart(), #1">`:
        DATA
        DC8 "icuStart(), #1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"icuStop\\"()">`:
        DATA
        DC8 "\"icuStop\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "icuStop(), #1">`:
        DATA
        DC8 "icuStop(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"icuEnable\\"()">`:
        DATA
        DC8 "\"icuEnable\"()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "icuEnable(), #1">`:
        DATA
        DC8 "icuEnable(), #1"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"icuDisable\\"()">`:
        DATA
        DC8 "\"icuDisable\"()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "icuDisable(), #1">`:
        DATA
        DC8 "icuDisable(), #1"
        DC8 0, 0, 0

        END
//  163 
//  164 #endif /* HAL_USE_ICU */
//  165 
//  166 /** @} */
// 
// 192 bytes in section .rodata
// 340 bytes in section .text
// 
// 340 bytes of CODE  memory
// 192 bytes of CONST memory
//
//Errors: none
//Warnings: none
