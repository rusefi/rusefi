///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:46 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32\TIMv1\icu_lld.c                      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32\TIMv1\icu_lld.c -lCN                 /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                        -lA F:\stuff\rusefi_sourceforge\firmware\iar\Debug\ /
//                       List\ -o F:\stuff\rusefi_sourceforge\firmware\iar\De /
//                       bug\Obj\ --no_cse --no_unroll --no_inline            /
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
//                       icu_lld.s                                            /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME icu_lld

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _port_irq_epilogue
        EXTERN chDbgPanic
        EXTERN dbg_check_enter_isr
        EXTERN dbg_check_leave_isr
        EXTERN icuObjectInit
        EXTERN nvicDisableVector
        EXTERN nvicEnableVector

        PUBLIC ICUD1
        PUBLIC ICUD2
        PUBLIC ICUD3
        PUBLIC ICUD9
        PUBLIC VectorA0
        PUBLIC VectorA4
        PUBLIC VectorAC
        PUBLIC VectorB0
        PUBLIC VectorB4
        PUBLIC icu_lld_disable
        PUBLIC icu_lld_enable
        PUBLIC icu_lld_init
        PUBLIC icu_lld_start
        PUBLIC icu_lld_stop
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\platforms\STM32\TIMv1\icu_lld.c
//    1 /*
//    2     ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
//    3 
//    4     Licensed under the Apache License, Version 2.0 (the "License");
//    5     you may not use this file except in compliance with the License.
//    6     You may obtain a copy of the License at
//    7 
//    8         http://www.apache.org/licenses/LICENSE-2.0
//    9 
//   10     Unless required by applicable law or agreed to in writing, software
//   11     distributed under the License is distributed on an "AS IS" BASIS,
//   12     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   13     See the License for the specific language governing permissions and
//   14     limitations under the License.
//   15 */
//   16 /*
//   17    Concepts and parts of this file have been contributed by Fabio Utzig and
//   18    Xo Wang.
//   19  */
//   20 
//   21 /**
//   22  * @file    STM32/icu_lld.c
//   23  * @brief   STM32 ICU subsystem low level driver header.
//   24  *
//   25  * @addtogroup ICU
//   26  * @{
//   27  */
//   28 
//   29 #include "ch.h"
//   30 #include "hal.h"
//   31 
//   32 #if HAL_USE_ICU || defined(__DOXYGEN__)
//   33 
//   34 /*===========================================================================*/
//   35 /* Driver local definitions.                                                 */
//   36 /*===========================================================================*/
//   37 
//   38 /*===========================================================================*/
//   39 /* Driver exported variables.                                                */
//   40 /*===========================================================================*/
//   41 
//   42 /**
//   43  * @brief   ICUD1 driver identifier.
//   44  * @note    The driver ICUD1 allocates the complex timer TIM1 when enabled.
//   45  */
//   46 #if STM32_ICU_USE_TIM1 || defined(__DOXYGEN__)

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   47 ICUDriver ICUD1;
ICUD1:
        DS8 24
//   48 #endif
//   49 
//   50 /**
//   51  * @brief   ICUD2 driver identifier.
//   52  * @note    The driver ICUD1 allocates the timer TIM2 when enabled.
//   53  */
//   54 #if STM32_ICU_USE_TIM2 || defined(__DOXYGEN__)

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   55 ICUDriver ICUD2;
ICUD2:
        DS8 24
//   56 #endif
//   57 
//   58 /**
//   59  * @brief   ICUD3 driver identifier.
//   60  * @note    The driver ICUD1 allocates the timer TIM3 when enabled.
//   61  */
//   62 #if STM32_ICU_USE_TIM3 || defined(__DOXYGEN__)

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   63 ICUDriver ICUD3;
ICUD3:
        DS8 24
//   64 #endif
//   65 
//   66 /**
//   67  * @brief   ICUD4 driver identifier.
//   68  * @note    The driver ICUD4 allocates the timer TIM4 when enabled.
//   69  */
//   70 #if STM32_ICU_USE_TIM4 || defined(__DOXYGEN__)
//   71 ICUDriver ICUD4;
//   72 #endif
//   73 
//   74 /**
//   75  * @brief   ICUD5 driver identifier.
//   76  * @note    The driver ICUD5 allocates the timer TIM5 when enabled.
//   77  */
//   78 #if STM32_ICU_USE_TIM5 || defined(__DOXYGEN__)
//   79 ICUDriver ICUD5;
//   80 #endif
//   81 
//   82 /**
//   83  * @brief   ICUD8 driver identifier.
//   84  * @note    The driver ICUD8 allocates the timer TIM8 when enabled.
//   85  */
//   86 #if STM32_ICU_USE_TIM8 || defined(__DOXYGEN__)
//   87 ICUDriver ICUD8;
//   88 #endif
//   89 
//   90 /**
//   91  * @brief   ICUD9 driver identifier.
//   92  * @note    The driver ICUD9 allocates the timer TIM9 when enabled.
//   93  */
//   94 #if STM32_ICU_USE_TIM9 || defined(__DOXYGEN__)

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   95 ICUDriver ICUD9;
ICUD9:
        DS8 24
//   96 #endif
//   97 
//   98 /*===========================================================================*/
//   99 /* Driver local variables and types.                                         */
//  100 /*===========================================================================*/
//  101 
//  102 /*===========================================================================*/
//  103 /* Driver local functions.                                                   */
//  104 /*===========================================================================*/
//  105 
//  106 /**
//  107  * @brief   Shared IRQ handler.
//  108  *
//  109  * @param[in] icup      pointer to the @p ICUDriver object
//  110  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function icu_lld_serve_interrupt
        THUMB
//  111 static void icu_lld_serve_interrupt(ICUDriver *icup) {
icu_lld_serve_interrupt:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  112   uint16_t sr;
//  113 
//  114   sr  = icup->tim->SR;
        LDR      R0,[R4, #+12]
        LDR      R0,[R0, #+16]
        MOVS     R5,R0
//  115   sr &= icup->tim->DIER & STM32_TIM_DIER_IRQ_MASK;
        LDR      R0,[R4, #+12]
        LDR      R0,[R0, #+12]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        ANDS     R5,R0,R5
//  116   icup->tim->SR = ~sr;
        LDR      R0,[R4, #+12]
        UXTH     R5,R5            ;; ZeroExt  R5,R5,#+16,#+16
        MVNS     R1,R5
        STR      R1,[R0, #+16]
//  117   if (icup->config->channel == ICU_CHANNEL_1) {
        LDR      R0,[R4, #+4]
        LDRB     R0,[R0, #+20]
        CMP      R0,#+0
        BNE.N    ??icu_lld_serve_interrupt_0
//  118     if ((sr & STM32_TIM_SR_CC1IF) != 0)
        LSLS     R0,R5,#+30
        BPL.N    ??icu_lld_serve_interrupt_1
//  119       _icu_isr_invoke_period_cb(icup);
        LDRB     R0,[R4, #+0]
        MOVS     R6,R0
        MOVS     R0,#+4
        STRB     R0,[R4, #+0]
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+3
        BEQ.N    ??icu_lld_serve_interrupt_1
        MOVS     R0,R4
        LDR      R1,[R4, #+4]
        LDR      R1,[R1, #+12]
          CFI FunCall
        BLX      R1
//  120     if ((sr & STM32_TIM_SR_CC2IF) != 0)
??icu_lld_serve_interrupt_1:
        LSLS     R0,R5,#+29
        BPL.N    ??icu_lld_serve_interrupt_2
//  121       _icu_isr_invoke_width_cb(icup);
        LDRB     R0,[R4, #+0]
        CMP      R0,#+3
        BEQ.N    ??icu_lld_serve_interrupt_2
        MOVS     R0,#+5
        STRB     R0,[R4, #+0]
        MOVS     R0,R4
        LDR      R1,[R4, #+4]
        LDR      R1,[R1, #+8]
          CFI FunCall
        BLX      R1
        B.N      ??icu_lld_serve_interrupt_2
//  122   } else {
//  123     if ((sr & STM32_TIM_SR_CC1IF) != 0)
??icu_lld_serve_interrupt_0:
        LSLS     R0,R5,#+30
        BPL.N    ??icu_lld_serve_interrupt_3
//  124       _icu_isr_invoke_width_cb(icup);
        LDRB     R0,[R4, #+0]
        CMP      R0,#+3
        BEQ.N    ??icu_lld_serve_interrupt_3
        MOVS     R0,#+5
        STRB     R0,[R4, #+0]
        MOVS     R0,R4
        LDR      R1,[R4, #+4]
        LDR      R1,[R1, #+8]
          CFI FunCall
        BLX      R1
//  125     if ((sr & STM32_TIM_SR_CC2IF) != 0)
??icu_lld_serve_interrupt_3:
        LSLS     R0,R5,#+29
        BPL.N    ??icu_lld_serve_interrupt_2
//  126       _icu_isr_invoke_period_cb(icup);
        LDRB     R0,[R4, #+0]
        MOVS     R6,R0
        MOVS     R0,#+4
        STRB     R0,[R4, #+0]
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+3
        BEQ.N    ??icu_lld_serve_interrupt_2
        MOVS     R0,R4
        LDR      R1,[R4, #+4]
        LDR      R1,[R1, #+12]
          CFI FunCall
        BLX      R1
//  127   }
//  128   if ((sr & STM32_TIM_SR_UIF) != 0)
??icu_lld_serve_interrupt_2:
        LSLS     R0,R5,#+31
        BPL.N    ??icu_lld_serve_interrupt_4
//  129     _icu_isr_invoke_overflow_cb(icup);
        MOVS     R0,R4
        LDR      R1,[R4, #+4]
        LDR      R1,[R1, #+16]
          CFI FunCall
        BLX      R1
//  130 }
??icu_lld_serve_interrupt_4:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock0
//  131 
//  132 /*===========================================================================*/
//  133 /* Driver interrupt handlers.                                                */
//  134 /*===========================================================================*/
//  135 
//  136 #if STM32_ICU_USE_TIM1
//  137 #if !defined(STM32_TIM1_UP_HANDLER)
//  138 #error "STM32_TIM1_UP_HANDLER not defined"
//  139 #endif
//  140 /**
//  141  * @brief   TIM1 compare interrupt handler.
//  142  * @note    It is assumed that the various sources are only activated if the
//  143  *          associated callback pointer is not equal to @p NULL in order to not
//  144  *          perform an extra check in a potentially critical interrupt handler.
//  145  *
//  146  * @isr
//  147  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function VectorA4
        THUMB
//  148 CH_IRQ_HANDLER(STM32_TIM1_UP_HANDLER) {
VectorA4:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  149 
//  150   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  151 
//  152   icu_lld_serve_interrupt(&ICUD1);
        LDR.N    R0,??DataTable7
          CFI FunCall icu_lld_serve_interrupt
        BL       icu_lld_serve_interrupt
//  153 
//  154   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  155 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock1
//  156 
//  157 #if !defined(STM32_TIM1_CC_HANDLER)
//  158 #error "STM32_TIM1_CC_HANDLER not defined"
//  159 #endif
//  160 /**
//  161  * @brief   TIM1 compare interrupt handler.
//  162  * @note    It is assumed that the various sources are only activated if the
//  163  *          associated callback pointer is not equal to @p NULL in order to not
//  164  *          perform an extra check in a potentially critical interrupt handler.
//  165  *
//  166  * @isr
//  167  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function VectorAC
        THUMB
//  168 CH_IRQ_HANDLER(STM32_TIM1_CC_HANDLER) {
VectorAC:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  169 
//  170   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  171 
//  172   icu_lld_serve_interrupt(&ICUD1);
        LDR.N    R0,??DataTable7
          CFI FunCall icu_lld_serve_interrupt
        BL       icu_lld_serve_interrupt
//  173 
//  174   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  175 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock2
//  176 #endif /* STM32_ICU_USE_TIM1 */
//  177 
//  178 #if STM32_ICU_USE_TIM2
//  179 #if !defined(STM32_TIM2_HANDLER)
//  180 #error "STM32_TIM2_HANDLER not defined"
//  181 #endif
//  182 /**
//  183  * @brief   TIM2 interrupt handler.
//  184  * @note    It is assumed that the various sources are only activated if the
//  185  *          associated callback pointer is not equal to @p NULL in order to not
//  186  *          perform an extra check in a potentially critical interrupt handler.
//  187  *
//  188  * @isr
//  189  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function VectorB0
        THUMB
//  190 CH_IRQ_HANDLER(STM32_TIM2_HANDLER) {
VectorB0:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  191 
//  192   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  193 
//  194   icu_lld_serve_interrupt(&ICUD2);
        LDR.N    R0,??DataTable7_1
          CFI FunCall icu_lld_serve_interrupt
        BL       icu_lld_serve_interrupt
//  195 
//  196   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  197 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock3
//  198 #endif /* STM32_ICU_USE_TIM2 */
//  199 
//  200 #if STM32_ICU_USE_TIM3
//  201 #if !defined(STM32_TIM3_HANDLER)
//  202 #error "STM32_TIM3_HANDLER not defined"
//  203 #endif
//  204 /**
//  205  * @brief   TIM3 interrupt handler.
//  206  * @note    It is assumed that the various sources are only activated if the
//  207  *          associated callback pointer is not equal to @p NULL in order to not
//  208  *          perform an extra check in a potentially critical interrupt handler.
//  209  *
//  210  * @isr
//  211  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function VectorB4
        THUMB
//  212 CH_IRQ_HANDLER(STM32_TIM3_HANDLER) {
VectorB4:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  213 
//  214   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  215 
//  216   icu_lld_serve_interrupt(&ICUD3);
        LDR.N    R0,??DataTable7_2
          CFI FunCall icu_lld_serve_interrupt
        BL       icu_lld_serve_interrupt
//  217 
//  218   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  219 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock4
//  220 #endif /* STM32_ICU_USE_TIM3 */
//  221 
//  222 #if STM32_ICU_USE_TIM4
//  223 #if !defined(STM32_TIM4_HANDLER)
//  224 #error "STM32_TIM4_HANDLER not defined"
//  225 #endif
//  226 /**
//  227  * @brief   TIM4 interrupt handler.
//  228  * @note    It is assumed that the various sources are only activated if the
//  229  *          associated callback pointer is not equal to @p NULL in order to not
//  230  *          perform an extra check in a potentially critical interrupt handler.
//  231  *
//  232  * @isr
//  233  */
//  234 CH_IRQ_HANDLER(STM32_TIM4_HANDLER) {
//  235 
//  236   CH_IRQ_PROLOGUE();
//  237 
//  238   icu_lld_serve_interrupt(&ICUD4);
//  239 
//  240   CH_IRQ_EPILOGUE();
//  241 }
//  242 #endif /* STM32_ICU_USE_TIM4 */
//  243 
//  244 #if STM32_ICU_USE_TIM5
//  245 #if !defined(STM32_TIM5_HANDLER)
//  246 #error "STM32_TIM5_HANDLER not defined"
//  247 #endif
//  248 /**
//  249  * @brief   TIM5 interrupt handler.
//  250  * @note    It is assumed that the various sources are only activated if the
//  251  *          associated callback pointer is not equal to @p NULL in order to not
//  252  *          perform an extra check in a potentially critical interrupt handler.
//  253  *
//  254  * @isr
//  255  */
//  256 CH_IRQ_HANDLER(STM32_TIM5_HANDLER) {
//  257 
//  258   CH_IRQ_PROLOGUE();
//  259 
//  260   icu_lld_serve_interrupt(&ICUD5);
//  261 
//  262   CH_IRQ_EPILOGUE();
//  263 }
//  264 #endif /* STM32_ICU_USE_TIM5 */
//  265 
//  266 #if STM32_ICU_USE_TIM8
//  267 #if !defined(STM32_TIM8_UP_HANDLER)
//  268 #error "STM32_TIM8_UP_HANDLER not defined"
//  269 #endif
//  270 /**
//  271  * @brief   TIM8 compare interrupt handler.
//  272  * @note    It is assumed that the various sources are only activated if the
//  273  *          associated callback pointer is not equal to @p NULL in order to not
//  274  *          perform an extra check in a potentially critical interrupt handler.
//  275  *
//  276  * @isr
//  277  */
//  278 CH_IRQ_HANDLER(STM32_TIM8_UP_HANDLER) {
//  279 
//  280   CH_IRQ_PROLOGUE();
//  281 
//  282   icu_lld_serve_interrupt(&ICUD8);
//  283 
//  284   CH_IRQ_EPILOGUE();
//  285 }
//  286 
//  287 #if !defined(STM32_TIM8_CC_HANDLER)
//  288 #error "STM32_TIM8_CC_HANDLER not defined"
//  289 #endif
//  290 /**
//  291  * @brief   TIM8 compare interrupt handler.
//  292  * @note    It is assumed that the various sources are only activated if the
//  293  *          associated callback pointer is not equal to @p NULL in order to not
//  294  *          perform an extra check in a potentially critical interrupt handler.
//  295  *
//  296  * @isr
//  297  */
//  298 CH_IRQ_HANDLER(STM32_TIM8_CC_HANDLER) {
//  299 
//  300   CH_IRQ_PROLOGUE();
//  301 
//  302   icu_lld_serve_interrupt(&ICUD8);
//  303 
//  304   CH_IRQ_EPILOGUE();
//  305 }
//  306 #endif /* STM32_ICU_USE_TIM8 */
//  307 
//  308 #if STM32_ICU_USE_TIM9
//  309 #if !defined(STM32_TIM9_HANDLER)
//  310 #error "STM32_TIM9_HANDLER not defined"
//  311 #endif
//  312 /**
//  313  * @brief   TIM9 interrupt handler.
//  314  * @note    It is assumed that the various sources are only activated if the
//  315  *          associated callback pointer is not equal to @p NULL in order to not
//  316  *          perform an extra check in a potentially critical interrupt handler.
//  317  *
//  318  * @isr
//  319  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function VectorA0
        THUMB
//  320 CH_IRQ_HANDLER(STM32_TIM9_HANDLER) {
VectorA0:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  321 
//  322   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  323 
//  324   icu_lld_serve_interrupt(&ICUD9);
        LDR.N    R0,??DataTable7_3
          CFI FunCall icu_lld_serve_interrupt
        BL       icu_lld_serve_interrupt
//  325 
//  326   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  327 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock5
//  328 #endif /* STM32_ICU_USE_TIM9 */
//  329 
//  330 /*===========================================================================*/
//  331 /* Driver exported functions.                                                */
//  332 /*===========================================================================*/
//  333 
//  334 /**
//  335  * @brief   Low level ICU driver initialization.
//  336  *
//  337  * @notapi
//  338  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function icu_lld_init
        THUMB
//  339 void icu_lld_init(void) {
icu_lld_init:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  340 
//  341 #if STM32_ICU_USE_TIM1
//  342   /* Driver initialization.*/
//  343   icuObjectInit(&ICUD1);
        LDR.N    R0,??DataTable7
          CFI FunCall icuObjectInit
        BL       icuObjectInit
//  344   ICUD1.tim = STM32_TIM1;
        LDR.N    R0,??DataTable7
        LDR.N    R1,??DataTable7_4  ;; 0x40010000
        STR      R1,[R0, #+12]
//  345 #endif
//  346 
//  347 #if STM32_ICU_USE_TIM2
//  348   /* Driver initialization.*/
//  349   icuObjectInit(&ICUD2);
        LDR.N    R0,??DataTable7_1
          CFI FunCall icuObjectInit
        BL       icuObjectInit
//  350   ICUD2.tim = STM32_TIM2;
        LDR.N    R0,??DataTable7_1
        MOVS     R1,#+1073741824
        STR      R1,[R0, #+12]
//  351 #endif
//  352 
//  353 #if STM32_ICU_USE_TIM3
//  354   /* Driver initialization.*/
//  355   icuObjectInit(&ICUD3);
        LDR.N    R0,??DataTable7_2
          CFI FunCall icuObjectInit
        BL       icuObjectInit
//  356   ICUD3.tim = STM32_TIM3;
        LDR.N    R0,??DataTable7_2
        LDR.N    R1,??DataTable7_5  ;; 0x40000400
        STR      R1,[R0, #+12]
//  357 #endif
//  358 
//  359 #if STM32_ICU_USE_TIM4
//  360   /* Driver initialization.*/
//  361   icuObjectInit(&ICUD4);
//  362   ICUD4.tim = STM32_TIM4;
//  363 #endif
//  364 
//  365 #if STM32_ICU_USE_TIM5
//  366   /* Driver initialization.*/
//  367   icuObjectInit(&ICUD5);
//  368   ICUD5.tim = STM32_TIM5;
//  369 #endif
//  370 
//  371 #if STM32_ICU_USE_TIM8
//  372   /* Driver initialization.*/
//  373   icuObjectInit(&ICUD8);
//  374   ICUD8.tim = STM32_TIM8;
//  375 #endif
//  376 
//  377 #if STM32_ICU_USE_TIM9
//  378   /* Driver initialization.*/
//  379   icuObjectInit(&ICUD9);
        LDR.N    R0,??DataTable7_3
          CFI FunCall icuObjectInit
        BL       icuObjectInit
//  380   ICUD9.tim = STM32_TIM9;
        LDR.N    R0,??DataTable7_3
        LDR.N    R1,??DataTable7_6  ;; 0x40014000
        STR      R1,[R0, #+12]
//  381 #endif
//  382 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock6
//  383 
//  384 /**
//  385  * @brief   Configures and activates the ICU peripheral.
//  386  *
//  387  * @param[in] icup      pointer to the @p ICUDriver object
//  388  *
//  389  * @notapi
//  390  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function icu_lld_start
        THUMB
//  391 void icu_lld_start(ICUDriver *icup) {
icu_lld_start:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  392   uint32_t psc;
//  393 
//  394   chDbgAssert((icup->config->channel == ICU_CHANNEL_1) ||
//  395               (icup->config->channel == ICU_CHANNEL_2),
//  396               "icu_lld_start(), #1", "invalid input");
        LDR      R0,[R4, #+4]
        LDRB     R0,[R0, #+20]
        CMP      R0,#+0
        BEQ.N    ??icu_lld_start_0
        LDR      R0,[R4, #+4]
        LDRB     R0,[R0, #+20]
        CMP      R0,#+1
        BEQ.N    ??icu_lld_start_0
        LDR.N    R0,??DataTable7_7
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  397 
//  398   if (icup->state == ICU_STOP) {
??icu_lld_start_0:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BNE.N    ??icu_lld_start_1
//  399     /* Clock activation and timer reset.*/
//  400 #if STM32_ICU_USE_TIM1
//  401     if (&ICUD1 == icup) {
        LDR.N    R0,??DataTable7
        CMP      R0,R4
        BNE.N    ??icu_lld_start_2
//  402       rccEnableTIM1(FALSE);
        LDR.N    R0,??DataTable7_8  ;; 0x40023844
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x1
        LDR.N    R1,??DataTable7_8  ;; 0x40023844
        STR      R0,[R1, #+0]
//  403       rccResetTIM1();
        LDR.N    R0,??DataTable7_9  ;; 0x40023824
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x1
        LDR.N    R1,??DataTable7_9  ;; 0x40023824
        STR      R0,[R1, #+0]
        LDR.N    R0,??DataTable7_9  ;; 0x40023824
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  404       nvicEnableVector(STM32_TIM1_UP_NUMBER,
//  405                        CORTEX_PRIORITY_MASK(STM32_ICU_TIM1_IRQ_PRIORITY));
        MOVS     R1,#+48
        MOVS     R0,#+25
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  406       nvicEnableVector(STM32_TIM1_CC_NUMBER,
//  407                        CORTEX_PRIORITY_MASK(STM32_ICU_TIM1_IRQ_PRIORITY));
        MOVS     R1,#+48
        MOVS     R0,#+27
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  408 #if defined(STM32_TIM1CLK)
//  409       icup->clock = STM32_TIM1CLK;
//  410 #else
//  411       icup->clock = STM32_TIMCLK2;
        LDR.N    R0,??DataTable7_10  ;; 0xa037a00
        STR      R0,[R4, #+8]
//  412 #endif
//  413     }
//  414 #endif
//  415 #if STM32_ICU_USE_TIM2
//  416     if (&ICUD2 == icup) {
??icu_lld_start_2:
        LDR.N    R0,??DataTable7_1
        CMP      R0,R4
        BNE.N    ??icu_lld_start_3
//  417       rccEnableTIM2(FALSE);
        LDR.N    R0,??DataTable7_11  ;; 0x40023840
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x1
        LDR.N    R1,??DataTable7_11  ;; 0x40023840
        STR      R0,[R1, #+0]
//  418       rccResetTIM2();
        LDR.N    R0,??DataTable7_12  ;; 0x40023820
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x1
        LDR.N    R1,??DataTable7_12  ;; 0x40023820
        STR      R0,[R1, #+0]
        LDR.N    R0,??DataTable7_12  ;; 0x40023820
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  419       nvicEnableVector(STM32_TIM2_NUMBER,
//  420                        CORTEX_PRIORITY_MASK(STM32_ICU_TIM2_IRQ_PRIORITY));
        MOVS     R1,#+48
        MOVS     R0,#+28
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  421       icup->clock = STM32_TIMCLK1;
        LDR.N    R0,??DataTable7_13  ;; 0x501bd00
        STR      R0,[R4, #+8]
//  422     }
//  423 #endif
//  424 #if STM32_ICU_USE_TIM3
//  425     if (&ICUD3 == icup) {
??icu_lld_start_3:
        LDR.N    R0,??DataTable7_2
        CMP      R0,R4
        BNE.N    ??icu_lld_start_4
//  426       rccEnableTIM3(FALSE);
        LDR.N    R0,??DataTable7_11  ;; 0x40023840
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x2
        LDR.N    R1,??DataTable7_11  ;; 0x40023840
        STR      R0,[R1, #+0]
//  427       rccResetTIM3();
        LDR.N    R0,??DataTable7_12  ;; 0x40023820
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x2
        LDR.N    R1,??DataTable7_12  ;; 0x40023820
        STR      R0,[R1, #+0]
        LDR.N    R0,??DataTable7_12  ;; 0x40023820
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  428       nvicEnableVector(STM32_TIM3_NUMBER,
//  429                        CORTEX_PRIORITY_MASK(STM32_ICU_TIM3_IRQ_PRIORITY));
        MOVS     R1,#+48
        MOVS     R0,#+29
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  430       icup->clock = STM32_TIMCLK1;
        LDR.N    R0,??DataTable7_13  ;; 0x501bd00
        STR      R0,[R4, #+8]
//  431     }
//  432 #endif
//  433 #if STM32_ICU_USE_TIM4
//  434     if (&ICUD4 == icup) {
//  435       rccEnableTIM4(FALSE);
//  436       rccResetTIM4();
//  437       nvicEnableVector(STM32_TIM4_NUMBER,
//  438                        CORTEX_PRIORITY_MASK(STM32_ICU_TIM4_IRQ_PRIORITY));
//  439       icup->clock = STM32_TIMCLK1;
//  440     }
//  441 #endif
//  442 #if STM32_ICU_USE_TIM5
//  443     if (&ICUD5 == icup) {
//  444       rccEnableTIM5(FALSE);
//  445       rccResetTIM5();
//  446       nvicEnableVector(STM32_TIM5_NUMBER,
//  447                        CORTEX_PRIORITY_MASK(STM32_ICU_TIM5_IRQ_PRIORITY));
//  448       icup->clock = STM32_TIMCLK1;
//  449     }
//  450 #endif
//  451 #if STM32_ICU_USE_TIM8
//  452     if (&ICUD8 == icup) {
//  453       rccEnableTIM8(FALSE);
//  454       rccResetTIM8();
//  455       nvicEnableVector(STM32_TIM8_UP_NUMBER,
//  456                        CORTEX_PRIORITY_MASK(STM32_ICU_TIM8_IRQ_PRIORITY));
//  457       nvicEnableVector(STM32_TIM8_CC_NUMBER,
//  458                        CORTEX_PRIORITY_MASK(STM32_ICU_TIM8_IRQ_PRIORITY));
//  459 #if defined(STM32_TIM8CLK)
//  460       icup->clock = STM32_TIM8CLK;
//  461 #else
//  462       icup->clock = STM32_TIMCLK2;
//  463 #endif
//  464     }
//  465 #endif
//  466 #if STM32_ICU_USE_TIM9
//  467     if (&ICUD9 == icup) {
??icu_lld_start_4:
        LDR.N    R0,??DataTable7_3
        CMP      R0,R4
        BNE.N    ??icu_lld_start_5
//  468       rccEnableTIM9(FALSE);
        LDR.N    R0,??DataTable7_8  ;; 0x40023844
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x10000
        LDR.N    R1,??DataTable7_8  ;; 0x40023844
        STR      R0,[R1, #+0]
//  469       rccResetTIM9();
        LDR.N    R0,??DataTable7_9  ;; 0x40023824
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x10000
        LDR.N    R1,??DataTable7_9  ;; 0x40023824
        STR      R0,[R1, #+0]
        LDR.N    R0,??DataTable7_9  ;; 0x40023824
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  470       nvicEnableVector(STM32_TIM9_NUMBER,
//  471                        CORTEX_PRIORITY_MASK(STM32_ICU_TIM9_IRQ_PRIORITY));
        MOVS     R1,#+48
        MOVS     R0,#+24
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  472       icup->clock = STM32_TIMCLK2;
        LDR.N    R0,??DataTable7_10  ;; 0xa037a00
        STR      R0,[R4, #+8]
        B.N      ??icu_lld_start_5
//  473     }
//  474 #endif
//  475   }
//  476   else {
//  477     /* Driver re-configuration scenario, it must be stopped first.*/
//  478     icup->tim->CR1    = 0;                  /* Timer disabled.              */
??icu_lld_start_1:
        LDR      R0,[R4, #+12]
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  479     icup->tim->CCR[0] = 0;                  /* Comparator 1 disabled.       */
        LDR      R0,[R4, #+12]
        MOVS     R1,#+0
        STR      R1,[R0, #+52]
//  480     icup->tim->CCR[1] = 0;                  /* Comparator 2 disabled.       */
        LDR      R0,[R4, #+12]
        MOVS     R1,#+0
        STR      R1,[R0, #+56]
//  481     icup->tim->CNT    = 0;                  /* Counter reset to zero.       */
        LDR      R0,[R4, #+12]
        MOVS     R1,#+0
        STR      R1,[R0, #+36]
//  482   }
//  483 
//  484   /* Timer configuration.*/
//  485   icup->tim->SR   = 0;                     /* Clear eventual pending IRQs. */
??icu_lld_start_5:
        LDR      R0,[R4, #+12]
        MOVS     R1,#+0
        STR      R1,[R0, #+16]
//  486   icup->tim->DIER = icup->config->dier &   /* DMA-related DIER settings.   */
//  487                       ~STM32_TIM_DIER_IRQ_MASK;
        LDR      R0,[R4, #+4]
        LDR      R0,[R0, #+24]
        LSRS     R0,R0,#+8
        LSLS     R0,R0,#+8
        LDR      R1,[R4, #+12]
        STR      R0,[R1, #+12]
//  488   psc = (icup->clock / icup->config->frequency) - 1;
        LDR      R0,[R4, #+8]
        LDR      R1,[R4, #+4]
        LDR      R1,[R1, #+4]
        UDIV     R0,R0,R1
        SUBS     R0,R0,#+1
        MOVS     R5,R0
//  489   chDbgAssert((psc <= 0xFFFF) &&
//  490               ((psc + 1) * icup->config->frequency) == icup->clock,
//  491               "icu_lld_start(), #1", "invalid frequency");
        CMP      R5,#+65536
        BCS.N    ??icu_lld_start_6
        ADDS     R0,R5,#+1
        LDR      R1,[R4, #+4]
        LDR      R1,[R1, #+4]
        MULS     R0,R1,R0
        LDR      R1,[R4, #+8]
        CMP      R0,R1
        BEQ.N    ??icu_lld_start_7
??icu_lld_start_6:
        LDR.N    R0,??DataTable7_7
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  492   icup->tim->PSC  = (uint16_t)psc;
??icu_lld_start_7:
        LDR      R0,[R4, #+12]
        MOVS     R1,R5
        UXTH     R1,R1            ;; ZeroExt  R1,R1,#+16,#+16
        STR      R1,[R0, #+40]
//  493   icup->tim->ARR  = 0xFFFF;
        LDR      R0,[R4, #+12]
        MOVW     R1,#+65535
        STR      R1,[R0, #+44]
//  494 
//  495   if (icup->config->channel == ICU_CHANNEL_1) {
        LDR      R0,[R4, #+4]
        LDRB     R0,[R0, #+20]
        CMP      R0,#+0
        BNE.N    ??icu_lld_start_8
//  496     /* Selected input 1.
//  497        CCMR1_CC1S = 01 = CH1 Input on TI1.
//  498        CCMR1_CC2S = 10 = CH2 Input on TI1.*/
//  499     icup->tim->CCMR1 = STM32_TIM_CCMR1_CC1S(1) | STM32_TIM_CCMR1_CC2S(2);
        LDR      R0,[R4, #+12]
        MOVW     R1,#+513
        STR      R1,[R0, #+24]
//  500 
//  501     /* SMCR_TS  = 101, input is TI1FP1.
//  502        SMCR_SMS = 100, reset on rising edge.*/
//  503     icup->tim->SMCR  = STM32_TIM_SMCR_TS(5) | STM32_TIM_SMCR_SMS(4);
        LDR      R0,[R4, #+12]
        MOVS     R1,#+84
        STR      R1,[R0, #+8]
//  504 
//  505     /* The CCER settings depend on the selected trigger mode.
//  506        ICU_INPUT_ACTIVE_HIGH: Active on rising edge, idle on falling edge.
//  507        ICU_INPUT_ACTIVE_LOW:  Active on falling edge, idle on rising edge.*/
//  508     if (icup->config->mode == ICU_INPUT_ACTIVE_HIGH)
        LDR      R0,[R4, #+4]
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??icu_lld_start_9
//  509       icup->tim->CCER = STM32_TIM_CCER_CC1E |
//  510                         STM32_TIM_CCER_CC2E | STM32_TIM_CCER_CC2P;
        LDR      R0,[R4, #+12]
        MOVS     R1,#+49
        STR      R1,[R0, #+32]
        B.N      ??icu_lld_start_10
//  511     else
//  512       icup->tim->CCER = STM32_TIM_CCER_CC1E | STM32_TIM_CCER_CC1P |
//  513                         STM32_TIM_CCER_CC2E;
??icu_lld_start_9:
        LDR      R0,[R4, #+12]
        MOVS     R1,#+19
        STR      R1,[R0, #+32]
//  514 
//  515     /* Direct pointers to the capture registers in order to make reading
//  516        data faster from within callbacks.*/
//  517     icup->wccrp = &icup->tim->CCR[1];
??icu_lld_start_10:
        LDR      R0,[R4, #+12]
        ADDS     R0,R0,#+56
        STR      R0,[R4, #+16]
//  518     icup->pccrp = &icup->tim->CCR[0];
        LDR      R0,[R4, #+12]
        ADDS     R0,R0,#+52
        STR      R0,[R4, #+20]
        B.N      ??icu_lld_start_11
//  519   } else {
//  520     /* Selected input 2.
//  521        CCMR1_CC1S = 10 = CH1 Input on TI2.
//  522        CCMR1_CC2S = 01 = CH2 Input on TI2.*/
//  523     icup->tim->CCMR1 = STM32_TIM_CCMR1_CC1S(2) | STM32_TIM_CCMR1_CC2S(1);
??icu_lld_start_8:
        LDR      R0,[R4, #+12]
        MOV      R1,#+258
        STR      R1,[R0, #+24]
//  524 
//  525     /* SMCR_TS  = 110, input is TI2FP2.
//  526        SMCR_SMS = 100, reset on rising edge.*/
//  527     icup->tim->SMCR  = STM32_TIM_SMCR_TS(6) | STM32_TIM_SMCR_SMS(4);
        LDR      R0,[R4, #+12]
        MOVS     R1,#+100
        STR      R1,[R0, #+8]
//  528 
//  529     /* The CCER settings depend on the selected trigger mode.
//  530        ICU_INPUT_ACTIVE_HIGH: Active on rising edge, idle on falling edge.
//  531        ICU_INPUT_ACTIVE_LOW:  Active on falling edge, idle on rising edge.*/
//  532     if (icup->config->mode == ICU_INPUT_ACTIVE_HIGH)
        LDR      R0,[R4, #+4]
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??icu_lld_start_12
//  533       icup->tim->CCER = STM32_TIM_CCER_CC1E | STM32_TIM_CCER_CC1P |
//  534                         STM32_TIM_CCER_CC2E;
        LDR      R0,[R4, #+12]
        MOVS     R1,#+19
        STR      R1,[R0, #+32]
        B.N      ??icu_lld_start_13
//  535     else
//  536       icup->tim->CCER = STM32_TIM_CCER_CC1E |
//  537                         STM32_TIM_CCER_CC2E | STM32_TIM_CCER_CC2P;
??icu_lld_start_12:
        LDR      R0,[R4, #+12]
        MOVS     R1,#+49
        STR      R1,[R0, #+32]
//  538 
//  539     /* Direct pointers to the capture registers in order to make reading
//  540        data faster from within callbacks.*/
//  541     icup->wccrp = &icup->tim->CCR[0];
??icu_lld_start_13:
        LDR      R0,[R4, #+12]
        ADDS     R0,R0,#+52
        STR      R0,[R4, #+16]
//  542     icup->pccrp = &icup->tim->CCR[1];
        LDR      R0,[R4, #+12]
        ADDS     R0,R0,#+56
        STR      R0,[R4, #+20]
//  543   }
//  544 }
??icu_lld_start_11:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock7
//  545 
//  546 /**
//  547  * @brief   Deactivates the ICU peripheral.
//  548  *
//  549  * @param[in] icup      pointer to the @p ICUDriver object
//  550  *
//  551  * @notapi
//  552  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function icu_lld_stop
        THUMB
//  553 void icu_lld_stop(ICUDriver *icup) {
icu_lld_stop:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  554 
//  555   if (icup->state == ICU_READY) {
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BNE.N    ??icu_lld_stop_0
//  556     /* Clock deactivation.*/
//  557     icup->tim->CR1  = 0;                    /* Timer disabled.              */
        LDR      R0,[R4, #+12]
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  558     icup->tim->DIER = 0;                    /* All IRQs disabled.           */
        LDR      R0,[R4, #+12]
        MOVS     R1,#+0
        STR      R1,[R0, #+12]
//  559     icup->tim->SR   = 0;                    /* Clear eventual pending IRQs. */
        LDR      R0,[R4, #+12]
        MOVS     R1,#+0
        STR      R1,[R0, #+16]
//  560 
//  561 #if STM32_ICU_USE_TIM1
//  562     if (&ICUD1 == icup) {
        LDR.N    R0,??DataTable7
        CMP      R0,R4
        BNE.N    ??icu_lld_stop_1
//  563       nvicDisableVector(STM32_TIM1_UP_NUMBER);
        MOVS     R0,#+25
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  564       nvicDisableVector(STM32_TIM1_CC_NUMBER);
        MOVS     R0,#+27
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  565       rccDisableTIM1(FALSE);
        LDR.N    R0,??DataTable7_8  ;; 0x40023844
        LDR      R0,[R0, #+0]
        LSRS     R0,R0,#+1
        LSLS     R0,R0,#+1
        LDR.N    R1,??DataTable7_8  ;; 0x40023844
        STR      R0,[R1, #+0]
//  566     }
//  567 #endif
//  568 #if STM32_ICU_USE_TIM2
//  569     if (&ICUD2 == icup) {
??icu_lld_stop_1:
        LDR.N    R0,??DataTable7_1
        CMP      R0,R4
        BNE.N    ??icu_lld_stop_2
//  570       nvicDisableVector(STM32_TIM2_NUMBER);
        MOVS     R0,#+28
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  571       rccDisableTIM2(FALSE);
        LDR.N    R0,??DataTable7_11  ;; 0x40023840
        LDR      R0,[R0, #+0]
        LSRS     R0,R0,#+1
        LSLS     R0,R0,#+1
        LDR.N    R1,??DataTable7_11  ;; 0x40023840
        STR      R0,[R1, #+0]
//  572     }
//  573 #endif
//  574 #if STM32_ICU_USE_TIM3
//  575     if (&ICUD3 == icup) {
??icu_lld_stop_2:
        LDR.N    R0,??DataTable7_2
        CMP      R0,R4
        BNE.N    ??icu_lld_stop_3
//  576       nvicDisableVector(STM32_TIM3_NUMBER);
        MOVS     R0,#+29
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  577       rccDisableTIM3(FALSE);
        LDR.N    R0,??DataTable7_11  ;; 0x40023840
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x2
        LDR.N    R1,??DataTable7_11  ;; 0x40023840
        STR      R0,[R1, #+0]
//  578     }
//  579 #endif
//  580 #if STM32_ICU_USE_TIM4
//  581     if (&ICUD4 == icup) {
//  582       nvicDisableVector(STM32_TIM4_NUMBER);
//  583       rccDisableTIM4(FALSE);
//  584     }
//  585 #endif
//  586 #if STM32_ICU_USE_TIM5
//  587     if (&ICUD5 == icup) {
//  588       nvicDisableVector(STM32_TIM5_NUMBER);
//  589       rccDisableTIM5(FALSE);
//  590     }
//  591 #endif
//  592 #if STM32_ICU_USE_TIM8
//  593     if (&ICUD8 == icup) {
//  594       nvicDisableVector(STM32_TIM8_UP_NUMBER);
//  595       nvicDisableVector(STM32_TIM8_CC_NUMBER);
//  596       rccDisableTIM8(FALSE);
//  597     }
//  598 #endif
//  599 #if STM32_ICU_USE_TIM9
//  600     if (&ICUD9 == icup) {
??icu_lld_stop_3:
        LDR.N    R0,??DataTable7_3
        CMP      R0,R4
        BNE.N    ??icu_lld_stop_0
//  601       nvicDisableVector(STM32_TIM9_NUMBER);
        MOVS     R0,#+24
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  602       rccDisableTIM9(FALSE);
        LDR.N    R0,??DataTable7_8  ;; 0x40023844
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x10000
        LDR.N    R1,??DataTable7_8  ;; 0x40023844
        STR      R0,[R1, #+0]
//  603     }
//  604 #endif
//  605   }
//  606 }
??icu_lld_stop_0:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7:
        DC32     ICUD1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_1:
        DC32     ICUD2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_2:
        DC32     ICUD3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_3:
        DC32     ICUD9

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_4:
        DC32     0x40010000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_5:
        DC32     0x40000400

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_6:
        DC32     0x40014000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_7:
        DC32     `?<Constant "icu_lld_start(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_8:
        DC32     0x40023844

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_9:
        DC32     0x40023824

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_10:
        DC32     0xa037a00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_11:
        DC32     0x40023840

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_12:
        DC32     0x40023820

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_13:
        DC32     0x501bd00
//  607 
//  608 /**
//  609  * @brief   Enables the input capture.
//  610  *
//  611  * @param[in] icup      pointer to the @p ICUDriver object
//  612  *
//  613  * @notapi
//  614  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function icu_lld_enable
          CFI NoCalls
        THUMB
//  615 void icu_lld_enable(ICUDriver *icup) {
//  616 
//  617   icup->tim->EGR |= STM32_TIM_EGR_UG;
icu_lld_enable:
        LDR      R1,[R0, #+12]
        LDR      R1,[R1, #+20]
        ORRS     R1,R1,#0x1
        LDR      R2,[R0, #+12]
        STR      R1,[R2, #+20]
//  618   icup->tim->SR = 0;                        /* Clear pending IRQs (if any). */
        LDR      R1,[R0, #+12]
        MOVS     R2,#+0
        STR      R2,[R1, #+16]
//  619   if (icup->config->channel == ICU_CHANNEL_1) {
        LDR      R1,[R0, #+4]
        LDRB     R1,[R1, #+20]
        CMP      R1,#+0
        BNE.N    ??icu_lld_enable_0
//  620     if (icup->config->period_cb != NULL)
        LDR      R1,[R0, #+4]
        LDR      R1,[R1, #+12]
        CMP      R1,#+0
        BEQ.N    ??icu_lld_enable_1
//  621       icup->tim->DIER |= STM32_TIM_DIER_CC1IE;
        LDR      R1,[R0, #+12]
        LDR      R1,[R1, #+12]
        ORRS     R1,R1,#0x2
        LDR      R2,[R0, #+12]
        STR      R1,[R2, #+12]
//  622     if (icup->config->width_cb != NULL)
??icu_lld_enable_1:
        LDR      R1,[R0, #+4]
        LDR      R1,[R1, #+8]
        CMP      R1,#+0
        BEQ.N    ??icu_lld_enable_2
//  623       icup->tim->DIER |= STM32_TIM_DIER_CC2IE;
        LDR      R1,[R0, #+12]
        LDR      R1,[R1, #+12]
        ORRS     R1,R1,#0x4
        LDR      R2,[R0, #+12]
        STR      R1,[R2, #+12]
        B.N      ??icu_lld_enable_2
//  624   } else {
//  625     if (icup->config->width_cb != NULL)
??icu_lld_enable_0:
        LDR      R1,[R0, #+4]
        LDR      R1,[R1, #+8]
        CMP      R1,#+0
        BEQ.N    ??icu_lld_enable_3
//  626       icup->tim->DIER |= STM32_TIM_DIER_CC1IE;
        LDR      R1,[R0, #+12]
        LDR      R1,[R1, #+12]
        ORRS     R1,R1,#0x2
        LDR      R2,[R0, #+12]
        STR      R1,[R2, #+12]
//  627     if (icup->config->period_cb != NULL)
??icu_lld_enable_3:
        LDR      R1,[R0, #+4]
        LDR      R1,[R1, #+12]
        CMP      R1,#+0
        BEQ.N    ??icu_lld_enable_2
//  628       icup->tim->DIER |= STM32_TIM_DIER_CC2IE;
        LDR      R1,[R0, #+12]
        LDR      R1,[R1, #+12]
        ORRS     R1,R1,#0x4
        LDR      R2,[R0, #+12]
        STR      R1,[R2, #+12]
//  629   }
//  630   if (icup->config->overflow_cb != NULL)
??icu_lld_enable_2:
        LDR      R1,[R0, #+4]
        LDR      R1,[R1, #+16]
        CMP      R1,#+0
        BEQ.N    ??icu_lld_enable_4
//  631     icup->tim->DIER |= STM32_TIM_DIER_UIE;
        LDR      R1,[R0, #+12]
        LDR      R1,[R1, #+12]
        ORRS     R1,R1,#0x1
        LDR      R2,[R0, #+12]
        STR      R1,[R2, #+12]
//  632   icup->tim->CR1 = STM32_TIM_CR1_URS | STM32_TIM_CR1_CEN;
??icu_lld_enable_4:
        LDR      R1,[R0, #+12]
        MOVS     R2,#+5
        STR      R2,[R1, #+0]
//  633 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock9
//  634 
//  635 /**
//  636  * @brief   Disables the input capture.
//  637  *
//  638  * @param[in] icup      pointer to the @p ICUDriver object
//  639  *
//  640  * @notapi
//  641  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function icu_lld_disable
          CFI NoCalls
        THUMB
//  642 void icu_lld_disable(ICUDriver *icup) {
//  643 
//  644   icup->tim->CR1   = 0;                     /* Initially stopped.           */
icu_lld_disable:
        LDR      R1,[R0, #+12]
        MOVS     R2,#+0
        STR      R2,[R1, #+0]
//  645   icup->tim->SR    = 0;                     /* Clear pending IRQs (if any). */
        LDR      R1,[R0, #+12]
        MOVS     R2,#+0
        STR      R2,[R1, #+16]
//  646 
//  647   /* All interrupts disabled.*/
//  648   icup->tim->DIER &= ~STM32_TIM_DIER_IRQ_MASK;
        LDR      R1,[R0, #+12]
        LDR      R1,[R1, #+12]
        LSRS     R1,R1,#+8
        LSLS     R1,R1,#+8
        LDR      R2,[R0, #+12]
        STR      R1,[R2, #+12]
//  649 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock10

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "icu_lld_start(), #1">`:
        DATA
        DC8 "icu_lld_start(), #1"

        END
//  650 
//  651 #endif /* HAL_USE_ICU */
//  652 
//  653 /** @} */
// 
//    96 bytes in section .bss
//    20 bytes in section .rodata
// 1 100 bytes in section .text
// 
// 1 100 bytes of CODE  memory
//    20 bytes of CONST memory
//    96 bytes of DATA  memory
//
//Errors: none
//Warnings: none
