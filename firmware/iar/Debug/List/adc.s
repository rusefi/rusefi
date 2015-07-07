///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:38 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\adc.c                                            /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\adc.c -lCN F:\stuff\rusefi_sourceforge\firmware\ /
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
//                       adc.s                                                /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME `adc`

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN adc_lld_init
        EXTERN adc_lld_start
        EXTERN adc_lld_start_conversion
        EXTERN adc_lld_stop
        EXTERN adc_lld_stop_conversion
        EXTERN chDbgCheckClassI
        EXTERN chDbgPanic
        EXTERN chDbgPanic3
        EXTERN chMtxInit
        EXTERN chMtxLock
        EXTERN chMtxUnlock
        EXTERN chSchGoSleepS
        EXTERN chSchReadyI
        EXTERN chSchWakeupS
        EXTERN dbg_check_lock
        EXTERN dbg_check_unlock
        EXTERN rlist

        PUBLIC adcAcquireBus
        PUBLIC adcConvert
        PUBLIC adcInit
        PUBLIC adcObjectInit
        PUBLIC adcReleaseBus
        PUBLIC adcStart
        PUBLIC adcStartConversion
        PUBLIC adcStartConversionI
        PUBLIC adcStop
        PUBLIC adcStopConversion
        PUBLIC adcStopConversionI
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\src\adc.c
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
//   29  * @file    adc.c
//   30  * @brief   ADC Driver code.
//   31  *
//   32  * @addtogroup ADC
//   33  * @{
//   34  */
//   35 
//   36 #include "ch.h"
//   37 #include "hal.h"
//   38 
//   39 #if HAL_USE_ADC || defined(__DOXYGEN__)
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
//   62  * @brief   ADC Driver initialization.
//   63  * @note    This function is implicitly invoked by @p halInit(), there is
//   64  *          no need to explicitly initialize the driver.
//   65  *
//   66  * @init
//   67  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function adcInit
        THUMB
//   68 void adcInit(void) {
adcInit:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   69 
//   70   adc_lld_init();
          CFI FunCall adc_lld_init
        BL       adc_lld_init
//   71 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0
//   72 
//   73 /**
//   74  * @brief   Initializes the standard part of a @p ADCDriver structure.
//   75  *
//   76  * @param[out] adcp     pointer to the @p ADCDriver object
//   77  *
//   78  * @init
//   79  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function adcObjectInit
        THUMB
//   80 void adcObjectInit(ADCDriver *adcp) {
adcObjectInit:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   81 
//   82   adcp->state    = ADC_STOP;
        MOVS     R0,#+1
        STRB     R0,[R4, #+0]
//   83   adcp->config   = NULL;
        MOVS     R0,#+0
        STR      R0,[R4, #+4]
//   84   adcp->samples  = NULL;
        MOVS     R0,#+0
        STR      R0,[R4, #+8]
//   85   adcp->depth    = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+12]
//   86   adcp->grpp     = NULL;
        MOVS     R0,#+0
        STR      R0,[R4, #+16]
//   87 #if ADC_USE_WAIT
//   88   adcp->thread   = NULL;
        MOVS     R0,#+0
        STR      R0,[R4, #+20]
//   89 #endif /* ADC_USE_WAIT */
//   90 #if ADC_USE_MUTUAL_EXCLUSION
//   91 #if CH_USE_MUTEXES
//   92   chMtxInit(&adcp->mutex);
        ADDS     R0,R4,#+24
          CFI FunCall chMtxInit
        BL       chMtxInit
//   93 #else
//   94   chSemInit(&adcp->semaphore, 1);
//   95 #endif
//   96 #endif /* ADC_USE_MUTUAL_EXCLUSION */
//   97 #if defined(ADC_DRIVER_EXT_INIT_HOOK)
//   98   ADC_DRIVER_EXT_INIT_HOOK(adcp);
//   99 #endif
//  100 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1
//  101 
//  102 /**
//  103  * @brief   Configures and activates the ADC peripheral.
//  104  *
//  105  * @param[in] adcp      pointer to the @p ADCDriver object
//  106  * @param[in] config    pointer to the @p ADCConfig object. Depending on
//  107  *                      the implementation the value can be @p NULL.
//  108  *
//  109  * @api
//  110  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function adcStart
        THUMB
//  111 void adcStart(ADCDriver *adcp, const ADCConfig *config) {
adcStart:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  112 
//  113   chDbgCheck(adcp != NULL, "adcStart");
        CMP      R4,#+0
        BNE.N    ??adcStart_0
        MOVS     R2,#+113
        LDR.N    R1,??DataTable7
        LDR.N    R0,??DataTable7_1
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  114 
//  115   chSysLock();
??adcStart_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  116   chDbgAssert((adcp->state == ADC_STOP) || (adcp->state == ADC_READY),
//  117               "adcStart(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BEQ.N    ??adcStart_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??adcStart_1
        LDR.N    R0,??DataTable7_2
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  118   adcp->config = config;
??adcStart_1:
        STR      R5,[R4, #+4]
//  119   adc_lld_start(adcp);
        MOVS     R0,R4
          CFI FunCall adc_lld_start
        BL       adc_lld_start
//  120   adcp->state = ADC_READY;
        MOVS     R0,#+2
        STRB     R0,[R4, #+0]
//  121   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  122 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2
//  123 
//  124 /**
//  125  * @brief   Deactivates the ADC peripheral.
//  126  *
//  127  * @param[in] adcp      pointer to the @p ADCDriver object
//  128  *
//  129  * @api
//  130  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function adcStop
        THUMB
//  131 void adcStop(ADCDriver *adcp) {
adcStop:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  132 
//  133   chDbgCheck(adcp != NULL, "adcStop");
        CMP      R4,#+0
        BNE.N    ??adcStop_0
        MOVS     R2,#+133
        LDR.N    R1,??DataTable7
        LDR.N    R0,??DataTable7_3
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  134 
//  135   chSysLock();
??adcStop_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  136   chDbgAssert((adcp->state == ADC_STOP) || (adcp->state == ADC_READY),
//  137               "adcStop(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BEQ.N    ??adcStop_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??adcStop_1
        LDR.N    R0,??DataTable7_4
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  138   adc_lld_stop(adcp);
??adcStop_1:
        MOVS     R0,R4
          CFI FunCall adc_lld_stop
        BL       adc_lld_stop
//  139   adcp->state = ADC_STOP;
        MOVS     R0,#+1
        STRB     R0,[R4, #+0]
//  140   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  141 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//  142 
//  143 /**
//  144  * @brief   Starts an ADC conversion.
//  145  * @details Starts an asynchronous conversion operation.
//  146  * @note    The buffer is organized as a matrix of M*N elements where M is the
//  147  *          channels number configured into the conversion group and N is the
//  148  *          buffer depth. The samples are sequentially written into the buffer
//  149  *          with no gaps.
//  150  *
//  151  * @param[in] adcp      pointer to the @p ADCDriver object
//  152  * @param[in] grpp      pointer to a @p ADCConversionGroup object
//  153  * @param[out] samples  pointer to the samples buffer
//  154  * @param[in] depth     buffer depth (matrix rows number). The buffer depth
//  155  *                      must be one or an even number.
//  156  *
//  157  * @api
//  158  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function adcStartConversion
        THUMB
//  159 void adcStartConversion(ADCDriver *adcp,
//  160                         const ADCConversionGroup *grpp,
//  161                         adcsample_t *samples,
//  162                         size_t depth) {
adcStartConversion:
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
//  163 
//  164   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  165   adcStartConversionI(adcp, grpp, samples, depth);
        MOVS     R3,R7
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall adcStartConversionI
        BL       adcStartConversionI
//  166   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  167 }
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock4
//  168 
//  169 /**
//  170  * @brief   Starts an ADC conversion.
//  171  * @details Starts an asynchronous conversion operation.
//  172  * @post    The callbacks associated to the conversion group will be invoked
//  173  *          on buffer fill and error events.
//  174  * @note    The buffer is organized as a matrix of M*N elements where M is the
//  175  *          channels number configured into the conversion group and N is the
//  176  *          buffer depth. The samples are sequentially written into the buffer
//  177  *          with no gaps.
//  178  *
//  179  * @param[in] adcp      pointer to the @p ADCDriver object
//  180  * @param[in] grpp      pointer to a @p ADCConversionGroup object
//  181  * @param[out] samples  pointer to the samples buffer
//  182  * @param[in] depth     buffer depth (matrix rows number). The buffer depth
//  183  *                      must be one or an even number.
//  184  *
//  185  * @iclass
//  186  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function adcStartConversionI
        THUMB
//  187 void adcStartConversionI(ADCDriver *adcp,
//  188                          const ADCConversionGroup *grpp,
//  189                          adcsample_t *samples,
//  190                          size_t depth) {
adcStartConversionI:
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
//  191 
//  192   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  193   chDbgCheck((adcp != NULL) && (grpp != NULL) && (samples != NULL) &&
//  194              ((depth == 1) || ((depth & 1) == 0)),
//  195              "adcStartConversionI");
        CMP      R4,#+0
        BEQ.N    ??adcStartConversionI_0
        CMP      R5,#+0
        BEQ.N    ??adcStartConversionI_0
        CMP      R6,#+0
        BEQ.N    ??adcStartConversionI_0
        CMP      R7,#+1
        BEQ.N    ??adcStartConversionI_1
        LSLS     R0,R7,#+31
        BPL.N    ??adcStartConversionI_1
??adcStartConversionI_0:
        MOVS     R2,#+195
        LDR.N    R1,??DataTable7
        LDR.N    R0,??DataTable7_5
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  196   chDbgAssert((adcp->state == ADC_READY) ||
//  197               (adcp->state == ADC_COMPLETE) ||
//  198               (adcp->state == ADC_ERROR),
//  199               "adcStartConversionI(), #1", "not ready");
??adcStartConversionI_1:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??adcStartConversionI_2
        LDRB     R0,[R4, #+0]
        CMP      R0,#+4
        BEQ.N    ??adcStartConversionI_2
        LDRB     R0,[R4, #+0]
        CMP      R0,#+5
        BEQ.N    ??adcStartConversionI_2
        LDR.N    R0,??DataTable7_6
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  200 
//  201   adcp->samples  = samples;
??adcStartConversionI_2:
        STR      R6,[R4, #+8]
//  202   adcp->depth    = depth;
        STR      R7,[R4, #+12]
//  203   adcp->grpp     = grpp;
        STR      R5,[R4, #+16]
//  204   adcp->state    = ADC_ACTIVE;
        MOVS     R0,#+3
        STRB     R0,[R4, #+0]
//  205   adc_lld_start_conversion(adcp);
        MOVS     R0,R4
          CFI FunCall adc_lld_start_conversion
        BL       adc_lld_start_conversion
//  206 }
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock5
//  207 
//  208 /**
//  209  * @brief   Stops an ongoing conversion.
//  210  * @details This function stops the currently ongoing conversion and returns
//  211  *          the driver in the @p ADC_READY state. If there was no conversion
//  212  *          being processed then the function does nothing.
//  213  *
//  214  * @param[in] adcp      pointer to the @p ADCDriver object
//  215  *
//  216  * @api
//  217  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function adcStopConversion
        THUMB
//  218 void adcStopConversion(ADCDriver *adcp) {
adcStopConversion:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  219 
//  220   chDbgCheck(adcp != NULL, "adcStopConversion");
        CMP      R4,#+0
        BNE.N    ??adcStopConversion_0
        MOVS     R2,#+220
        LDR.N    R1,??DataTable7
        LDR.N    R0,??DataTable7_7
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  221 
//  222   chSysLock();
??adcStopConversion_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  223   chDbgAssert((adcp->state == ADC_READY) ||
//  224               (adcp->state == ADC_ACTIVE),
//  225               "adcStopConversion(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??adcStopConversion_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+3
        BEQ.N    ??adcStopConversion_1
        LDR.N    R0,??DataTable7_8
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  226   if (adcp->state != ADC_READY) {
??adcStopConversion_1:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??adcStopConversion_2
//  227     adc_lld_stop_conversion(adcp);
        MOVS     R0,R4
          CFI FunCall adc_lld_stop_conversion
        BL       adc_lld_stop_conversion
//  228     adcp->grpp  = NULL;
        MOVS     R0,#+0
        STR      R0,[R4, #+16]
//  229     adcp->state = ADC_READY;
        MOVS     R0,#+2
        STRB     R0,[R4, #+0]
//  230     _adc_reset_s(adcp);
        LDR      R0,[R4, #+20]
        CMP      R0,#+0
        BEQ.N    ??adcStopConversion_2
        LDR      R0,[R4, #+20]
        MOVS     R5,R0
        MOVS     R0,#+0
        STR      R0,[R4, #+20]
        MVNS     R1,#+1
        MOVS     R0,R5
          CFI FunCall chSchWakeupS
        BL       chSchWakeupS
//  231   }
//  232   chSysUnlock();
??adcStopConversion_2:
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  233 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock6
//  234 
//  235 /**
//  236  * @brief   Stops an ongoing conversion.
//  237  * @details This function stops the currently ongoing conversion and returns
//  238  *          the driver in the @p ADC_READY state. If there was no conversion
//  239  *          being processed then the function does nothing.
//  240  *
//  241  * @param[in] adcp      pointer to the @p ADCDriver object
//  242  *
//  243  * @iclass
//  244  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function adcStopConversionI
        THUMB
//  245 void adcStopConversionI(ADCDriver *adcp) {
adcStopConversionI:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  246 
//  247   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  248   chDbgCheck(adcp != NULL, "adcStopConversionI");
        CMP      R4,#+0
        BNE.N    ??adcStopConversionI_0
        MOVS     R2,#+248
        LDR.N    R1,??DataTable7
        LDR.N    R0,??DataTable7_9
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  249   chDbgAssert((adcp->state == ADC_READY) ||
//  250               (adcp->state == ADC_ACTIVE) ||
//  251               (adcp->state == ADC_COMPLETE),
//  252               "adcStopConversionI(), #1", "invalid state");
??adcStopConversionI_0:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??adcStopConversionI_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+3
        BEQ.N    ??adcStopConversionI_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+4
        BEQ.N    ??adcStopConversionI_1
        LDR.N    R0,??DataTable7_10
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  253 
//  254   if (adcp->state != ADC_READY) {
??adcStopConversionI_1:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??adcStopConversionI_2
//  255     adc_lld_stop_conversion(adcp);
        MOVS     R0,R4
          CFI FunCall adc_lld_stop_conversion
        BL       adc_lld_stop_conversion
//  256     adcp->grpp  = NULL;
        MOVS     R0,#+0
        STR      R0,[R4, #+16]
//  257     adcp->state = ADC_READY;
        MOVS     R0,#+2
        STRB     R0,[R4, #+0]
//  258     _adc_reset_i(adcp);
        LDR      R0,[R4, #+20]
        CMP      R0,#+0
        BEQ.N    ??adcStopConversionI_2
        LDR      R0,[R4, #+20]
        MOVS     R5,R0
        MOVS     R0,#+0
        STR      R0,[R4, #+20]
        MVNS     R0,#+1
        STR      R0,[R5, #+40]
        MOVS     R0,R5
          CFI FunCall chSchReadyI
        BL       chSchReadyI
//  259   }
//  260 }
??adcStopConversionI_2:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock7
//  261 
//  262 #if ADC_USE_WAIT || defined(__DOXYGEN__)
//  263 /**
//  264  * @brief   Performs an ADC conversion.
//  265  * @details Performs a synchronous conversion operation.
//  266  * @note    The buffer is organized as a matrix of M*N elements where M is the
//  267  *          channels number configured into the conversion group and N is the
//  268  *          buffer depth. The samples are sequentially written into the buffer
//  269  *          with no gaps.
//  270  *
//  271  * @param[in] adcp      pointer to the @p ADCDriver object
//  272  * @param[in] grpp      pointer to a @p ADCConversionGroup object
//  273  * @param[out] samples  pointer to the samples buffer
//  274  * @param[in] depth     buffer depth (matrix rows number). The buffer depth
//  275  *                      must be one or an even number.
//  276  * @return              The operation result.
//  277  * @retval RDY_OK       Conversion finished.
//  278  * @retval RDY_RESET    The conversion has been stopped using
//  279  *                      @p acdStopConversion() or @p acdStopConversionI(),
//  280  *                      the result buffer may contain incorrect data.
//  281  * @retval RDY_TIMEOUT  The conversion has been stopped because an hardware
//  282  *                      error.
//  283  *
//  284  * @api
//  285  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function adcConvert
        THUMB
//  286 msg_t adcConvert(ADCDriver *adcp,
//  287                  const ADCConversionGroup *grpp,
//  288                  adcsample_t *samples,
//  289                  size_t depth) {
adcConvert:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
//  290   msg_t msg;
//  291 
//  292   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  293   chDbgAssert(adcp->thread == NULL, "adcConvert(), #1", "already waiting");
        LDR      R0,[R4, #+20]
        CMP      R0,#+0
        BEQ.N    ??adcConvert_0
        LDR.N    R0,??DataTable7_11
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  294   adcStartConversionI(adcp, grpp, samples, depth);
??adcConvert_0:
        MOVS     R3,R7
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall adcStartConversionI
        BL       adcStartConversionI
//  295   adcp->thread = chThdSelf();
        LDR.N    R0,??DataTable7_12
        LDR      R0,[R0, #+24]
        STR      R0,[R4, #+20]
//  296   chSchGoSleepS(THD_STATE_SUSPENDED);
        MOVS     R0,#+2
          CFI FunCall chSchGoSleepS
        BL       chSchGoSleepS
//  297   msg = chThdSelf()->p_u.rdymsg;
        LDR.N    R0,??DataTable7_12
        LDR      R0,[R0, #+24]
        LDR      R0,[R0, #+40]
        MOV      R8,R0
//  298   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  299   return msg;
        MOV      R0,R8
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock8
//  300 }
//  301 #endif /* ADC_USE_WAIT */
//  302 
//  303 #if ADC_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
//  304 /**
//  305  * @brief   Gains exclusive access to the ADC peripheral.
//  306  * @details This function tries to gain ownership to the ADC bus, if the bus
//  307  *          is already being used then the invoking thread is queued.
//  308  * @pre     In order to use this function the option
//  309  *          @p ADC_USE_MUTUAL_EXCLUSION must be enabled.
//  310  *
//  311  * @param[in] adcp      pointer to the @p ADCDriver object
//  312  *
//  313  * @api
//  314  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function adcAcquireBus
        THUMB
//  315 void adcAcquireBus(ADCDriver *adcp) {
adcAcquireBus:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  316 
//  317   chDbgCheck(adcp != NULL, "adcAcquireBus");
        CMP      R4,#+0
        BNE.N    ??adcAcquireBus_0
        MOVW     R2,#+317
        LDR.N    R1,??DataTable7
        LDR.N    R0,??DataTable7_13
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  318 
//  319 #if CH_USE_MUTEXES
//  320   chMtxLock(&adcp->mutex);
??adcAcquireBus_0:
        ADDS     R0,R4,#+24
          CFI FunCall chMtxLock
        BL       chMtxLock
//  321 #elif CH_USE_SEMAPHORES
//  322   chSemWait(&adcp->semaphore);
//  323 #endif
//  324 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock9
//  325 
//  326 /**
//  327  * @brief   Releases exclusive access to the ADC peripheral.
//  328  * @pre     In order to use this function the option
//  329  *          @p ADC_USE_MUTUAL_EXCLUSION must be enabled.
//  330  *
//  331  * @param[in] adcp      pointer to the @p ADCDriver object
//  332  *
//  333  * @api
//  334  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function adcReleaseBus
        THUMB
//  335 void adcReleaseBus(ADCDriver *adcp) {
adcReleaseBus:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  336 
//  337   chDbgCheck(adcp != NULL, "adcReleaseBus");
        CMP      R4,#+0
        BNE.N    ??adcReleaseBus_0
        MOVW     R2,#+337
        LDR.N    R1,??DataTable7
        LDR.N    R0,??DataTable7_14
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  338 
//  339 #if CH_USE_MUTEXES
//  340   (void)adcp;
//  341   chMtxUnlock();
??adcReleaseBus_0:
          CFI FunCall chMtxUnlock
        BL       chMtxUnlock
//  342 #elif CH_USE_SEMAPHORES
//  343   chSemSignal(&adcp->semaphore);
//  344 #endif
//  345 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock10

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_1:
        DC32     `?<Constant "\\"adcStart\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_2:
        DC32     `?<Constant "adcStart(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_3:
        DC32     `?<Constant "\\"adcStop\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_4:
        DC32     `?<Constant "adcStop(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_5:
        DC32     `?<Constant "\\"adcStartConversionI\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_6:
        DC32     `?<Constant "adcStartConversionI()...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_7:
        DC32     `?<Constant "\\"adcStopConversion\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_8:
        DC32     `?<Constant "adcStopConversion(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_9:
        DC32     `?<Constant "\\"adcStopConversionI\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_10:
        DC32     `?<Constant "adcStopConversionI(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_11:
        DC32     `?<Constant "adcConvert(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_12:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_13:
        DC32     `?<Constant "\\"adcAcquireBus\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_14:
        DC32     `?<Constant "\\"adcReleaseBus\\"()">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"adcStart\\"()">`:
        DATA
        DC8 "\"adcStart\"()"
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
        DC8 61H, 64H, 63H, 2EH, 63H, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "adcStart(), #1">`:
        DATA
        DC8 "adcStart(), #1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"adcStop\\"()">`:
        DATA
        DC8 "\"adcStop\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "adcStop(), #1">`:
        DATA
        DC8 "adcStop(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"adcStartConversionI\\"()">`:
        DATA
        DC8 "\"adcStartConversionI\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "adcStartConversionI()...">`:
        DATA
        DC8 "adcStartConversionI(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"adcStopConversion\\"()">`:
        DATA
        DC8 "\"adcStopConversion\"()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "adcStopConversion(), #1">`:
        DATA
        DC8 "adcStopConversion(), #1"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"adcStopConversionI\\"()">`:
        DATA
        DC8 "\"adcStopConversionI\"()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "adcStopConversionI(), #1">`:
        DATA
        DC8 "adcStopConversionI(), #1"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "adcConvert(), #1">`:
        DATA
        DC8 "adcConvert(), #1"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"adcAcquireBus\\"()">`:
        DATA
        DC8 "\"adcAcquireBus\"()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"adcReleaseBus\\"()">`:
        DATA
        DC8 "\"adcReleaseBus\"()"
        DC8 0, 0

        END
//  346 #endif /* ADC_USE_MUTUAL_EXCLUSION */
//  347 
//  348 #endif /* HAL_USE_ADC */
//  349 
//  350 /** @} */
// 
// 336 bytes in section .rodata
// 710 bytes in section .text
// 
// 710 bytes of CODE  memory
// 336 bytes of CONST memory
//
//Errors: none
//Warnings: none
