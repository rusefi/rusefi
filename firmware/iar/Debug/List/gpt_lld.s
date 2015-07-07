///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:04 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32\TIMv1\gpt_lld.c                      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32\TIMv1\gpt_lld.c -lCN                 /
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
//                       gpt_lld.s                                            /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME gpt_lld

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _port_irq_epilogue
        EXTERN chDbgPanic
        EXTERN dbg_check_enter_isr
        EXTERN dbg_check_leave_isr
        EXTERN gptObjectInit
        EXTERN nvicDisableVector
        EXTERN nvicEnableVector

        PUBLIC GPTD5
        PUBLIC Vector108
        PUBLIC gpt_lld_init
        PUBLIC gpt_lld_polled_delay
        PUBLIC gpt_lld_start
        PUBLIC gpt_lld_start_timer
        PUBLIC gpt_lld_stop
        PUBLIC gpt_lld_stop_timer
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\platforms\STM32\TIMv1\gpt_lld.c
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
//   16 
//   17 /**
//   18  * @file    STM32/gpt_lld.c
//   19  * @brief   STM32 GPT subsystem low level driver source.
//   20  *
//   21  * @addtogroup GPT
//   22  * @{
//   23  */
//   24 
//   25 #include "ch.h"
//   26 #include "hal.h"
//   27 
//   28 #if HAL_USE_GPT || defined(__DOXYGEN__)
//   29 
//   30 /*===========================================================================*/
//   31 /* Driver local definitions.                                                 */
//   32 /*===========================================================================*/
//   33 
//   34 /*===========================================================================*/
//   35 /* Driver exported variables.                                                */
//   36 /*===========================================================================*/
//   37 
//   38 /**
//   39  * @brief   GPTD1 driver identifier.
//   40  * @note    The driver GPTD1 allocates the complex timer TIM1 when enabled.
//   41  */
//   42 #if STM32_GPT_USE_TIM1 || defined(__DOXYGEN__)
//   43 GPTDriver GPTD1;
//   44 #endif
//   45 
//   46 /**
//   47  * @brief   GPTD2 driver identifier.
//   48  * @note    The driver GPTD2 allocates the timer TIM2 when enabled.
//   49  */
//   50 #if STM32_GPT_USE_TIM2 || defined(__DOXYGEN__)
//   51 GPTDriver GPTD2;
//   52 #endif
//   53 
//   54 /**
//   55  * @brief   GPTD3 driver identifier.
//   56  * @note    The driver GPTD3 allocates the timer TIM3 when enabled.
//   57  */
//   58 #if STM32_GPT_USE_TIM3 || defined(__DOXYGEN__)
//   59 GPTDriver GPTD3;
//   60 #endif
//   61 
//   62 /**
//   63  * @brief   GPTD4 driver identifier.
//   64  * @note    The driver GPTD4 allocates the timer TIM4 when enabled.
//   65  */
//   66 #if STM32_GPT_USE_TIM4 || defined(__DOXYGEN__)
//   67 GPTDriver GPTD4;
//   68 #endif
//   69 
//   70 /**
//   71  * @brief   GPTD5 driver identifier.
//   72  * @note    The driver GPTD5 allocates the timer TIM5 when enabled.
//   73  */
//   74 #if STM32_GPT_USE_TIM5 || defined(__DOXYGEN__)

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   75 GPTDriver GPTD5;
GPTD5:
        DS8 16
//   76 #endif
//   77 
//   78 /**
//   79  * @brief   GPTD6 driver identifier.
//   80  * @note    The driver GPTD6 allocates the timer TIM6 when enabled.
//   81  */
//   82 #if STM32_GPT_USE_TIM6 || defined(__DOXYGEN__)
//   83 GPTDriver GPTD6;
//   84 #endif
//   85 
//   86 /**
//   87  * @brief   GPTD7 driver identifier.
//   88  * @note    The driver GPTD7 allocates the timer TIM7 when enabled.
//   89  */
//   90 #if STM32_GPT_USE_TIM7 || defined(__DOXYGEN__)
//   91 GPTDriver GPTD7;
//   92 #endif
//   93 
//   94 /**
//   95  * @brief   GPTD8 driver identifier.
//   96  * @note    The driver GPTD8 allocates the timer TIM8 when enabled.
//   97  */
//   98 #if STM32_GPT_USE_TIM8 || defined(__DOXYGEN__)
//   99 GPTDriver GPTD8;
//  100 #endif
//  101 
//  102 /**
//  103  * @brief   GPTD9 driver identifier.
//  104  * @note    The driver GPTD9 allocates the timer TIM9 when enabled.
//  105  */
//  106 #if STM32_GPT_USE_TIM9 || defined(__DOXYGEN__)
//  107 GPTDriver GPTD9;
//  108 #endif
//  109 
//  110 /**
//  111  * @brief   GPTD11 driver identifier.
//  112  * @note    The driver GPTD11 allocates the timer TIM11 when enabled.
//  113  */
//  114 #if STM32_GPT_USE_TIM11 || defined(__DOXYGEN__)
//  115 GPTDriver GPTD11;
//  116 #endif
//  117 
//  118 /**
//  119  * @brief   GPTD12 driver identifier.
//  120  * @note    The driver GPTD12 allocates the timer TIM12 when enabled.
//  121  */
//  122 #if STM32_GPT_USE_TIM12 || defined(__DOXYGEN__)
//  123 GPTDriver GPTD12;
//  124 #endif
//  125 
//  126 /**
//  127  * @brief   GPTD14 driver identifier.
//  128  * @note    The driver GPTD14 allocates the timer TIM14 when enabled.
//  129  */
//  130 #if STM32_GPT_USE_TIM14 || defined(__DOXYGEN__)
//  131 GPTDriver GPTD14;
//  132 #endif
//  133 
//  134 /*===========================================================================*/
//  135 /* Driver local variables and types.                                         */
//  136 /*===========================================================================*/
//  137 
//  138 /*===========================================================================*/
//  139 /* Driver local functions.                                                   */
//  140 /*===========================================================================*/
//  141 
//  142 /**
//  143  * @brief   Shared IRQ handler.
//  144  *
//  145  * @param[in] gptp      pointer to a @p GPTDriver object
//  146  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function gpt_lld_serve_interrupt
        THUMB
//  147 static void gpt_lld_serve_interrupt(GPTDriver *gptp) {
gpt_lld_serve_interrupt:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  148 
//  149   gptp->tim->SR = 0;
        LDR      R0,[R4, #+12]
        MOVS     R1,#+0
        STR      R1,[R0, #+16]
//  150   if (gptp->state == GPT_ONESHOT) {
        LDRB     R0,[R4, #+0]
        CMP      R0,#+4
        BNE.N    ??gpt_lld_serve_interrupt_0
//  151     gptp->state = GPT_READY;                /* Back in GPT_READY state.     */
        MOVS     R0,#+2
        STRB     R0,[R4, #+0]
//  152     gpt_lld_stop_timer(gptp);               /* Timer automatically stopped. */
        MOVS     R0,R4
          CFI FunCall gpt_lld_stop_timer
        BL       gpt_lld_stop_timer
//  153   }
//  154   gptp->config->callback(gptp);
??gpt_lld_serve_interrupt_0:
        MOVS     R0,R4
        LDR      R1,[R4, #+4]
        LDR      R1,[R1, #+4]
          CFI FunCall
        BLX      R1
//  155 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0
//  156 
//  157 /*===========================================================================*/
//  158 /* Driver interrupt handlers.                                                */
//  159 /*===========================================================================*/
//  160 
//  161 #if STM32_GPT_USE_TIM1
//  162 #if !defined(STM32_TIM1_UP_HANDLER)
//  163 #error "STM32_TIM1_UP_HANDLER not defined"
//  164 #endif
//  165 /**
//  166  * @brief   TIM2 interrupt handler.
//  167  *
//  168  * @isr
//  169  */
//  170 CH_IRQ_HANDLER(STM32_TIM1_UP_HANDLER) {
//  171 
//  172   CH_IRQ_PROLOGUE();
//  173 
//  174   gpt_lld_serve_interrupt(&GPTD1);
//  175 
//  176   CH_IRQ_EPILOGUE();
//  177 }
//  178 #endif /* STM32_GPT_USE_TIM1 */
//  179 
//  180 #if STM32_GPT_USE_TIM2
//  181 #if !defined(STM32_TIM2_HANDLER)
//  182 #error "STM32_TIM2_HANDLER not defined"
//  183 #endif
//  184 /**
//  185  * @brief   TIM2 interrupt handler.
//  186  *
//  187  * @isr
//  188  */
//  189 CH_IRQ_HANDLER(STM32_TIM2_HANDLER) {
//  190 
//  191   CH_IRQ_PROLOGUE();
//  192 
//  193   gpt_lld_serve_interrupt(&GPTD2);
//  194 
//  195   CH_IRQ_EPILOGUE();
//  196 }
//  197 #endif /* STM32_GPT_USE_TIM2 */
//  198 
//  199 #if STM32_GPT_USE_TIM3
//  200 #if !defined(STM32_TIM3_HANDLER)
//  201 #error "STM32_TIM3_HANDLER not defined"
//  202 #endif
//  203 /**
//  204  * @brief   TIM3 interrupt handler.
//  205  *
//  206  * @isr
//  207  */
//  208 CH_IRQ_HANDLER(STM32_TIM3_HANDLER) {
//  209 
//  210   CH_IRQ_PROLOGUE();
//  211 
//  212   gpt_lld_serve_interrupt(&GPTD3);
//  213 
//  214   CH_IRQ_EPILOGUE();
//  215 }
//  216 #endif /* STM32_GPT_USE_TIM3 */
//  217 
//  218 #if STM32_GPT_USE_TIM4
//  219 #if !defined(STM32_TIM4_HANDLER)
//  220 #error "STM32_TIM4_HANDLER not defined"
//  221 #endif
//  222 /**
//  223  * @brief   TIM4 interrupt handler.
//  224  *
//  225  * @isr
//  226  */
//  227 CH_IRQ_HANDLER(STM32_TIM4_HANDLER) {
//  228 
//  229   CH_IRQ_PROLOGUE();
//  230 
//  231   gpt_lld_serve_interrupt(&GPTD4);
//  232 
//  233   CH_IRQ_EPILOGUE();
//  234 }
//  235 #endif /* STM32_GPT_USE_TIM4 */
//  236 
//  237 #if STM32_GPT_USE_TIM5
//  238 #if !defined(STM32_TIM5_HANDLER)
//  239 #error "STM32_TIM5_HANDLER not defined"
//  240 #endif
//  241 /**
//  242  * @brief   TIM5 interrupt handler.
//  243  *
//  244  * @isr
//  245  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function Vector108
        THUMB
//  246 CH_IRQ_HANDLER(STM32_TIM5_HANDLER) {
Vector108:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  247 
//  248   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  249 
//  250   gpt_lld_serve_interrupt(&GPTD5);
        LDR.N    R0,??DataTable3
          CFI FunCall gpt_lld_serve_interrupt
        BL       gpt_lld_serve_interrupt
//  251 
//  252   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  253 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock1
//  254 #endif /* STM32_GPT_USE_TIM5 */
//  255 
//  256 #if STM32_GPT_USE_TIM6
//  257 #if !defined(STM32_TIM6_HANDLER)
//  258 #error "STM32_TIM6_HANDLER not defined"
//  259 #endif
//  260 /**
//  261  * @brief   TIM6 interrupt handler.
//  262  *
//  263  * @isr
//  264  */
//  265 CH_IRQ_HANDLER(STM32_TIM6_HANDLER) {
//  266 
//  267   CH_IRQ_PROLOGUE();
//  268 
//  269   gpt_lld_serve_interrupt(&GPTD6);
//  270 
//  271   CH_IRQ_EPILOGUE();
//  272 }
//  273 #endif /* STM32_GPT_USE_TIM6 */
//  274 
//  275 #if STM32_GPT_USE_TIM7
//  276 #if !defined(STM32_TIM7_HANDLER)
//  277 #error "STM32_TIM7_HANDLER not defined"
//  278 #endif
//  279 /**
//  280  * @brief   TIM7 interrupt handler.
//  281  *
//  282  * @isr
//  283  */
//  284 CH_IRQ_HANDLER(STM32_TIM7_HANDLER) {
//  285 
//  286   CH_IRQ_PROLOGUE();
//  287 
//  288   gpt_lld_serve_interrupt(&GPTD7);
//  289 
//  290   CH_IRQ_EPILOGUE();
//  291 }
//  292 #endif /* STM32_GPT_USE_TIM7 */
//  293 
//  294 #if STM32_GPT_USE_TIM8
//  295 #if !defined(STM32_TIM8_UP_HANDLER)
//  296 #error "STM32_TIM8_UP_HANDLER not defined"
//  297 #endif
//  298 /**
//  299  * @brief   TIM8 interrupt handler.
//  300  *
//  301  * @isr
//  302  */
//  303 CH_IRQ_HANDLER(STM32_TIM8_UP_HANDLER) {
//  304 
//  305   CH_IRQ_PROLOGUE();
//  306 
//  307   gpt_lld_serve_interrupt(&GPTD8);
//  308 
//  309   CH_IRQ_EPILOGUE();
//  310 }
//  311 #endif /* STM32_GPT_USE_TIM8 */
//  312 
//  313 #if STM32_GPT_USE_TIM9
//  314 #if !defined(STM32_TIM9_HANDLER)
//  315 #error "STM32_TIM9_HANDLER not defined"
//  316 #endif
//  317 /**
//  318  * @brief   TIM9 interrupt handler.
//  319  *
//  320  * @isr
//  321  */
//  322 CH_IRQ_HANDLER(STM32_TIM9_HANDLER) {
//  323 
//  324   CH_IRQ_PROLOGUE();
//  325 
//  326   gpt_lld_serve_interrupt(&GPTD9);
//  327 
//  328   CH_IRQ_EPILOGUE();
//  329 }
//  330 #endif /* STM32_GPT_USE_TIM9 */
//  331 
//  332 #if STM32_GPT_USE_TIM11
//  333 #if !defined(STM32_TIM11_HANDLER)
//  334 #error "STM32_TIM11_HANDLER not defined"
//  335 #endif
//  336 /**
//  337  * @brief   TIM11 interrupt handler.
//  338  *
//  339  * @isr
//  340  */
//  341 CH_IRQ_HANDLER(STM32_TIM11_HANDLER) {
//  342 
//  343   CH_IRQ_PROLOGUE();
//  344 
//  345   gpt_lld_serve_interrupt(&GPTD11);
//  346 
//  347   CH_IRQ_EPILOGUE();
//  348 }
//  349 #endif /* STM32_GPT_USE_TIM11 */
//  350 
//  351 #if STM32_GPT_USE_TIM12
//  352 #if !defined(STM32_TIM12_HANDLER)
//  353 #error "STM32_TIM12_HANDLER not defined"
//  354 #endif
//  355 /**
//  356  * @brief   TIM12 interrupt handler.
//  357  *
//  358  * @isr
//  359  */
//  360 CH_IRQ_HANDLER(STM32_TIM12_HANDLER) {
//  361 
//  362   CH_IRQ_PROLOGUE();
//  363 
//  364   gpt_lld_serve_interrupt(&GPTD12);
//  365 
//  366   CH_IRQ_EPILOGUE();
//  367 }
//  368 #endif /* STM32_GPT_USE_TIM12 */
//  369 
//  370 #if STM32_GPT_USE_TIM14
//  371 #if !defined(STM32_TIM14_HANDLER)
//  372 #error "STM32_TIM14_HANDLER not defined"
//  373 #endif
//  374 /**
//  375  * @brief   TIM14 interrupt handler.
//  376  *
//  377  * @isr
//  378  */
//  379 CH_IRQ_HANDLER(STM32_TIM14_HANDLER) {
//  380 
//  381   CH_IRQ_PROLOGUE();
//  382 
//  383   gpt_lld_serve_interrupt(&GPTD14);
//  384 
//  385   CH_IRQ_EPILOGUE();
//  386 }
//  387 #endif /* STM32_GPT_USE_TIM14 */
//  388 
//  389 /*===========================================================================*/
//  390 /* Driver exported functions.                                                */
//  391 /*===========================================================================*/
//  392 
//  393 /**
//  394  * @brief   Low level GPT driver initialization.
//  395  *
//  396  * @notapi
//  397  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function gpt_lld_init
        THUMB
//  398 void gpt_lld_init(void) {
gpt_lld_init:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  399 
//  400 #if STM32_GPT_USE_TIM1
//  401   /* Driver initialization.*/
//  402   GPTD1.tim = STM32_TIM1;
//  403   gptObjectInit(&GPTD1);
//  404 #endif
//  405 
//  406 #if STM32_GPT_USE_TIM2
//  407   /* Driver initialization.*/
//  408   GPTD2.tim = STM32_TIM2;
//  409   gptObjectInit(&GPTD2);
//  410 #endif
//  411 
//  412 #if STM32_GPT_USE_TIM3
//  413   /* Driver initialization.*/
//  414   GPTD3.tim = STM32_TIM3;
//  415   gptObjectInit(&GPTD3);
//  416 #endif
//  417 
//  418 #if STM32_GPT_USE_TIM4
//  419   /* Driver initialization.*/
//  420   GPTD4.tim = STM32_TIM4;
//  421   gptObjectInit(&GPTD4);
//  422 #endif
//  423 
//  424 #if STM32_GPT_USE_TIM5
//  425   /* Driver initialization.*/
//  426   GPTD5.tim = STM32_TIM5;
        LDR.N    R0,??DataTable3
        LDR.N    R1,??DataTable3_1  ;; 0x40000c00
        STR      R1,[R0, #+12]
//  427   gptObjectInit(&GPTD5);
        LDR.N    R0,??DataTable3
          CFI FunCall gptObjectInit
        BL       gptObjectInit
//  428 #endif
//  429 
//  430 #if STM32_GPT_USE_TIM6
//  431   /* Driver initialization.*/
//  432   GPTD6.tim = STM32_TIM6;
//  433   gptObjectInit(&GPTD6);
//  434 #endif
//  435 
//  436 #if STM32_GPT_USE_TIM7
//  437   /* Driver initialization.*/
//  438   GPTD7.tim = STM32_TIM7;
//  439   gptObjectInit(&GPTD7);
//  440 #endif
//  441 
//  442 #if STM32_GPT_USE_TIM8
//  443   /* Driver initialization.*/
//  444   GPTD8.tim = STM32_TIM8;
//  445   gptObjectInit(&GPTD8);
//  446 #endif
//  447 
//  448 #if STM32_GPT_USE_TIM9
//  449   /* Driver initialization.*/
//  450   GPTD9.tim = STM32_TIM9;
//  451   gptObjectInit(&GPTD9);
//  452 #endif
//  453 
//  454 #if STM32_GPT_USE_TIM11
//  455   /* Driver initialization.*/
//  456   GPTD11.tim = STM32_TIM11;
//  457   gptObjectInit(&GPTD11);
//  458 #endif
//  459 
//  460 #if STM32_GPT_USE_TIM12
//  461   /* Driver initialization.*/
//  462   GPTD12.tim = STM32_TIM12;
//  463   gptObjectInit(&GPTD12);
//  464 #endif
//  465 
//  466 #if STM32_GPT_USE_TIM14
//  467   /* Driver initialization.*/
//  468   GPTD14.tim = STM32_TIM14;
//  469   gptObjectInit(&GPTD14);
//  470 #endif
//  471 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock2
//  472 
//  473 /**
//  474  * @brief   Configures and activates the GPT peripheral.
//  475  *
//  476  * @param[in] gptp      pointer to the @p GPTDriver object
//  477  *
//  478  * @notapi
//  479  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function gpt_lld_start
        THUMB
//  480 void gpt_lld_start(GPTDriver *gptp) {
gpt_lld_start:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  481   uint16_t psc;
//  482 
//  483   if (gptp->state == GPT_STOP) {
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BNE.N    ??gpt_lld_start_0
//  484     /* Clock activation.*/
//  485 #if STM32_GPT_USE_TIM1
//  486     if (&GPTD1 == gptp) {
//  487       rccEnableTIM1(FALSE);
//  488       rccResetTIM1();
//  489       nvicEnableVector(STM32_TIM1_UP_NUMBER,
//  490                        CORTEX_PRIORITY_MASK(STM32_GPT_TIM1_IRQ_PRIORITY));
//  491 #if defined(STM32_TIM1CLK)
//  492       gptp->clock = STM32_TIM1CLK;
//  493 #else
//  494       gptp->clock = STM32_TIMCLK2;
//  495 #endif
//  496     }
//  497 #endif
//  498 #if STM32_GPT_USE_TIM2
//  499     if (&GPTD2 == gptp) {
//  500       rccEnableTIM2(FALSE);
//  501       rccResetTIM2();
//  502       nvicEnableVector(STM32_TIM2_NUMBER,
//  503                        CORTEX_PRIORITY_MASK(STM32_GPT_TIM2_IRQ_PRIORITY));
//  504       gptp->clock = STM32_TIMCLK1;
//  505     }
//  506 #endif
//  507 #if STM32_GPT_USE_TIM3
//  508     if (&GPTD3 == gptp) {
//  509       rccEnableTIM3(FALSE);
//  510       rccResetTIM3();
//  511       nvicEnableVector(STM32_TIM3_NUMBER,
//  512                        CORTEX_PRIORITY_MASK(STM32_GPT_TIM3_IRQ_PRIORITY));
//  513       gptp->clock = STM32_TIMCLK1;
//  514     }
//  515 #endif
//  516 #if STM32_GPT_USE_TIM4
//  517     if (&GPTD4 == gptp) {
//  518       rccEnableTIM4(FALSE);
//  519       rccResetTIM4();
//  520       nvicEnableVector(STM32_TIM4_NUMBER,
//  521                        CORTEX_PRIORITY_MASK(STM32_GPT_TIM4_IRQ_PRIORITY));
//  522       gptp->clock = STM32_TIMCLK1;
//  523     }
//  524 #endif
//  525 
//  526 #if STM32_GPT_USE_TIM5
//  527     if (&GPTD5 == gptp) {
        LDR.N    R0,??DataTable3
        CMP      R0,R4
        BNE.N    ??gpt_lld_start_0
//  528       rccEnableTIM5(FALSE);
        LDR.N    R0,??DataTable3_2  ;; 0x40023840
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x8
        LDR.N    R1,??DataTable3_2  ;; 0x40023840
        STR      R0,[R1, #+0]
//  529       rccResetTIM5();
        LDR.N    R0,??DataTable3_3  ;; 0x40023820
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x8
        LDR.N    R1,??DataTable3_3  ;; 0x40023820
        STR      R0,[R1, #+0]
        LDR.N    R0,??DataTable3_3  ;; 0x40023820
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  530       nvicEnableVector(STM32_TIM5_NUMBER,
//  531                        CORTEX_PRIORITY_MASK(STM32_GPT_TIM5_IRQ_PRIORITY));
        MOVS     R1,#+64
        MOVS     R0,#+50
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  532       gptp->clock = STM32_TIMCLK1;
        LDR.N    R0,??DataTable3_4  ;; 0x501bd00
        STR      R0,[R4, #+8]
//  533     }
//  534 #endif
//  535 
//  536 #if STM32_GPT_USE_TIM6
//  537     if (&GPTD6 == gptp) {
//  538       rccEnableTIM6(FALSE);
//  539       rccResetTIM6();
//  540       nvicEnableVector(STM32_TIM6_NUMBER,
//  541                        CORTEX_PRIORITY_MASK(STM32_GPT_TIM6_IRQ_PRIORITY));
//  542       gptp->clock = STM32_TIMCLK1;
//  543     }
//  544 #endif
//  545 
//  546 #if STM32_GPT_USE_TIM7
//  547     if (&GPTD7 == gptp) {
//  548       rccEnableTIM7(FALSE);
//  549       rccResetTIM7();
//  550       nvicEnableVector(STM32_TIM7_NUMBER,
//  551                        CORTEX_PRIORITY_MASK(STM32_GPT_TIM7_IRQ_PRIORITY));
//  552       gptp->clock = STM32_TIMCLK1;
//  553     }
//  554 #endif
//  555 
//  556 #if STM32_GPT_USE_TIM8
//  557     if (&GPTD8 == gptp) {
//  558       rccEnableTIM8(FALSE);
//  559       rccResetTIM8();
//  560       nvicEnableVector(STM32_TIM8_UP_NUMBER,
//  561                        CORTEX_PRIORITY_MASK(STM32_GPT_TIM8_IRQ_PRIORITY));
//  562 #if defined(STM32_TIM8CLK)
//  563       gptp->clock = STM32_TIM8CLK;
//  564 #else
//  565       gptp->clock = STM32_TIMCLK2;
//  566 #endif
//  567     }
//  568 #endif
//  569 
//  570 #if STM32_GPT_USE_TIM9
//  571     if (&GPTD9 == gptp) {
//  572       rccEnableTIM9(FALSE);
//  573       rccResetTIM9();
//  574       nvicEnableVector(STM32_TIM9_NUMBER,
//  575                        CORTEX_PRIORITY_MASK(STM32_GPT_TIM9_IRQ_PRIORITY));
//  576       gptp->clock = STM32_TIMCLK2;
//  577     }
//  578 #endif
//  579 
//  580 #if STM32_GPT_USE_TIM11
//  581     if (&GPTD11 == gptp) {
//  582       rccEnableTIM11(FALSE);
//  583       rccResetTIM11();
//  584       nvicEnableVector(STM32_TIM11_NUMBER,
//  585                        CORTEX_PRIORITY_MASK(STM32_GPT_TIM11_IRQ_PRIORITY));
//  586       gptp->clock = STM32_TIMCLK2;
//  587     }
//  588 #endif
//  589 
//  590 #if STM32_GPT_USE_TIM12
//  591     if (&GPTD12 == gptp) {
//  592       rccEnableTIM12(FALSE);
//  593       rccResetTIM12();
//  594       nvicEnableVector(STM32_TIM12_NUMBER,
//  595                        CORTEX_PRIORITY_MASK(STM32_GPT_TIM12_IRQ_PRIORITY));
//  596       gptp->clock = STM32_TIMCLK1;
//  597     }
//  598 #endif
//  599 
//  600 #if STM32_GPT_USE_TIM14
//  601     if (&GPTD14 == gptp) {
//  602       rccEnableTIM14(FALSE);
//  603       rccResetTIM14();
//  604       nvicEnableVector(STM32_TIM14_NUMBER,
//  605                        CORTEX_PRIORITY_MASK(STM32_GPT_TIM14_IRQ_PRIORITY));
//  606       gptp->clock = STM32_TIMCLK1;
//  607     }
//  608 #endif
//  609   }
//  610 
//  611   /* Prescaler value calculation.*/
//  612   psc = (uint16_t)((gptp->clock / gptp->config->frequency) - 1);
??gpt_lld_start_0:
        LDR      R0,[R4, #+8]
        LDR      R1,[R4, #+4]
        LDR      R1,[R1, #+0]
        UDIV     R0,R0,R1
        SUBS     R0,R0,#+1
        MOVS     R5,R0
//  613   chDbgAssert(((uint32_t)(psc + 1) * gptp->config->frequency) == gptp->clock,
//  614               "gpt_lld_start(), #1", "invalid frequency");
        UXTH     R5,R5            ;; ZeroExt  R5,R5,#+16,#+16
        ADDS     R0,R5,#+1
        LDR      R1,[R4, #+4]
        LDR      R1,[R1, #+0]
        MULS     R0,R1,R0
        LDR      R1,[R4, #+8]
        CMP      R0,R1
        BEQ.N    ??gpt_lld_start_1
        LDR.N    R0,??DataTable3_5
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  615 
//  616   /* Timer configuration.*/
//  617   gptp->tim->CR1  = 0;                          /* Initially stopped.       */
??gpt_lld_start_1:
        LDR      R0,[R4, #+12]
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  618   gptp->tim->CR2  = STM32_TIM_CR2_CCDS;         /* DMA on UE (if any).      */
        LDR      R0,[R4, #+12]
        MOVS     R1,#+8
        STR      R1,[R0, #+4]
//  619   gptp->tim->PSC  = psc;                        /* Prescaler value.         */
        LDR      R0,[R4, #+12]
        UXTH     R5,R5            ;; ZeroExt  R5,R5,#+16,#+16
        STR      R5,[R0, #+40]
//  620   gptp->tim->DIER = gptp->config->dier &        /* DMA-related DIER bits.   */
//  621                     STM32_TIM_DIER_IRQ_MASK;
        LDR      R0,[R4, #+4]
        LDRB     R0,[R0, #+8]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LDR      R1,[R4, #+12]
        STR      R0,[R1, #+12]
//  622   gptp->tim->SR   = 0;                          /* Clear pending IRQs.      */
        LDR      R0,[R4, #+12]
        MOVS     R1,#+0
        STR      R1,[R0, #+16]
//  623 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock3
//  624 
//  625 /**
//  626  * @brief   Deactivates the GPT peripheral.
//  627  *
//  628  * @param[in] gptp      pointer to the @p GPTDriver object
//  629  *
//  630  * @notapi
//  631  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function gpt_lld_stop
        THUMB
//  632 void gpt_lld_stop(GPTDriver *gptp) {
gpt_lld_stop:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  633 
//  634   if (gptp->state == GPT_READY) {
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BNE.N    ??gpt_lld_stop_0
//  635     gptp->tim->CR1  = 0;                        /* Timer disabled.          */
        LDR      R0,[R4, #+12]
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  636     gptp->tim->DIER = 0;                        /* All IRQs disabled.       */
        LDR      R0,[R4, #+12]
        MOVS     R1,#+0
        STR      R1,[R0, #+12]
//  637     gptp->tim->SR   = 0;                        /* Clear pending IRQs.      */
        LDR      R0,[R4, #+12]
        MOVS     R1,#+0
        STR      R1,[R0, #+16]
//  638 
//  639 #if STM32_GPT_USE_TIM1
//  640     if (&GPTD1 == gptp) {
//  641       nvicDisableVector(STM32_TIM1_UP_NUMBER);
//  642       rccDisableTIM1(FALSE);
//  643     }
//  644 #endif
//  645 #if STM32_GPT_USE_TIM2
//  646     if (&GPTD2 == gptp) {
//  647       nvicDisableVector(STM32_TIM2_NUMBER);
//  648       rccDisableTIM2(FALSE);
//  649     }
//  650 #endif
//  651 #if STM32_GPT_USE_TIM3
//  652     if (&GPTD3 == gptp) {
//  653       nvicDisableVector(STM32_TIM3_NUMBER);
//  654       rccDisableTIM3(FALSE);
//  655     }
//  656 #endif
//  657 #if STM32_GPT_USE_TIM4
//  658     if (&GPTD4 == gptp) {
//  659       nvicDisableVector(STM32_TIM4_NUMBER);
//  660       rccDisableTIM4(FALSE);
//  661     }
//  662 #endif
//  663 #if STM32_GPT_USE_TIM5
//  664     if (&GPTD5 == gptp) {
        LDR.N    R0,??DataTable3
        CMP      R0,R4
        BNE.N    ??gpt_lld_stop_0
//  665       nvicDisableVector(STM32_TIM5_NUMBER);
        MOVS     R0,#+50
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  666       rccDisableTIM5(FALSE);
        LDR.N    R0,??DataTable3_2  ;; 0x40023840
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x8
        LDR.N    R1,??DataTable3_2  ;; 0x40023840
        STR      R0,[R1, #+0]
//  667     }
//  668 #endif
//  669 #if STM32_GPT_USE_TIM6
//  670     if (&GPTD6 == gptp) {
//  671       nvicDisableVector(STM32_TIM6_NUMBER);
//  672       rccDisableTIM6(FALSE);
//  673     }
//  674 #endif
//  675 #if STM32_GPT_USE_TIM7
//  676     if (&GPTD7 == gptp) {
//  677       nvicDisableVector(STM32_TIM7_NUMBER);
//  678       rccDisableTIM7(FALSE);
//  679     }
//  680 #endif
//  681 #if STM32_GPT_USE_TIM8
//  682     if (&GPTD8 == gptp) {
//  683       nvicDisableVector(STM32_TIM8_UP_NUMBER);
//  684       rccDisableTIM8(FALSE);
//  685     }
//  686 #endif
//  687 #if STM32_GPT_USE_TIM9
//  688     if (&GPTD9 == gptp) {
//  689       nvicDisableVector(STM32_TIM9_NUMBER);
//  690       rccDisableTIM9(FALSE);
//  691     }
//  692 #endif
//  693 #if STM32_GPT_USE_TIM11
//  694     if (&GPTD11 == gptp) {
//  695       nvicDisableVector(STM32_TIM11_NUMBER);
//  696       rccDisableTIM11(FALSE);
//  697     }
//  698 #endif
//  699 #if STM32_GPT_USE_TIM12
//  700     if (&GPTD12 == gptp) {
//  701       nvicDisableVector(STM32_TIM12_NUMBER);
//  702       rccDisableTIM12(FALSE);
//  703     }
//  704 #endif
//  705 #if STM32_GPT_USE_TIM14
//  706     if (&GPTD14 == gptp) {
//  707       nvicDisableVector(STM32_TIM14_NUMBER);
//  708       rccDisableTIM14(FALSE);
//  709     }
//  710 #endif
//  711   }
//  712 }
??gpt_lld_stop_0:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3:
        DC32     GPTD5

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_1:
        DC32     0x40000c00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_2:
        DC32     0x40023840

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_3:
        DC32     0x40023820

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_4:
        DC32     0x501bd00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_5:
        DC32     `?<Constant "gpt_lld_start(), #1">`
//  713 
//  714 /**
//  715  * @brief   Starts the timer in continuous mode.
//  716  *
//  717  * @param[in] gptp      pointer to the @p GPTDriver object
//  718  * @param[in] interval  period in ticks
//  719  *
//  720  * @notapi
//  721  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function gpt_lld_start_timer
          CFI NoCalls
        THUMB
//  722 void gpt_lld_start_timer(GPTDriver *gptp, gptcnt_t interval) {
//  723 
//  724   gptp->tim->ARR   = (uint32_t)(interval - 1);  /* Time constant.           */
gpt_lld_start_timer:
        LDR      R2,[R0, #+12]
        SUBS     R3,R1,#+1
        STR      R3,[R2, #+44]
//  725   gptp->tim->EGR   = STM32_TIM_EGR_UG;          /* Update event.            */
        LDR      R2,[R0, #+12]
        MOVS     R3,#+1
        STR      R3,[R2, #+20]
//  726   gptp->tim->CNT   = 0;                         /* Reset counter.           */
        LDR      R2,[R0, #+12]
        MOVS     R3,#+0
        STR      R3,[R2, #+36]
//  727 
//  728   /* NOTE: After generating the UG event it takes several clock cycles before
//  729      SR bit 0 goes to 1. This is because the clearing of CNT has been inserted
//  730      before the clearing of SR, to give it some time.*/
//  731   gptp->tim->SR    = 0;                         /* Clear pending IRQs.      */
        LDR      R2,[R0, #+12]
        MOVS     R3,#+0
        STR      R3,[R2, #+16]
//  732   gptp->tim->DIER |= STM32_TIM_DIER_UIE;        /* Update Event IRQ enabled.*/
        LDR      R2,[R0, #+12]
        LDR      R2,[R2, #+12]
        ORRS     R2,R2,#0x1
        LDR      R3,[R0, #+12]
        STR      R2,[R3, #+12]
//  733   gptp->tim->CR1   = STM32_TIM_CR1_URS | STM32_TIM_CR1_CEN;
        LDR      R2,[R0, #+12]
        MOVS     R3,#+5
        STR      R3,[R2, #+0]
//  734 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock5
//  735 
//  736 /**
//  737  * @brief   Stops the timer.
//  738  *
//  739  * @param[in] gptp      pointer to the @p GPTDriver object
//  740  *
//  741  * @notapi
//  742  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function gpt_lld_stop_timer
          CFI NoCalls
        THUMB
//  743 void gpt_lld_stop_timer(GPTDriver *gptp) {
//  744 
//  745   gptp->tim->CR1   = 0;                         /* Initially stopped.       */
gpt_lld_stop_timer:
        LDR      R1,[R0, #+12]
        MOVS     R2,#+0
        STR      R2,[R1, #+0]
//  746   gptp->tim->SR    = 0;                         /* Clear pending IRQs.      */
        LDR      R1,[R0, #+12]
        MOVS     R2,#+0
        STR      R2,[R1, #+16]
//  747 
//  748   /* All interrupts disabled.*/
//  749   gptp->tim->DIER &= ~STM32_TIM_DIER_IRQ_MASK;
        LDR      R1,[R0, #+12]
        LDR      R1,[R1, #+12]
        LSRS     R1,R1,#+8
        LSLS     R1,R1,#+8
        LDR      R2,[R0, #+12]
        STR      R1,[R2, #+12]
//  750 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock6
//  751 
//  752 /**
//  753  * @brief   Starts the timer in one shot mode and waits for completion.
//  754  * @details This function specifically polls the timer waiting for completion
//  755  *          in order to not have extra delays caused by interrupt servicing,
//  756  *          this function is only recommended for short delays.
//  757  *
//  758  * @param[in] gptp      pointer to the @p GPTDriver object
//  759  * @param[in] interval  time interval in ticks
//  760  *
//  761  * @notapi
//  762  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function gpt_lld_polled_delay
          CFI NoCalls
        THUMB
//  763 void gpt_lld_polled_delay(GPTDriver *gptp, gptcnt_t interval) {
//  764 
//  765   gptp->tim->ARR  = (uint32_t)(interval - 1);   /* Time constant.           */
gpt_lld_polled_delay:
        LDR      R2,[R0, #+12]
        SUBS     R3,R1,#+1
        STR      R3,[R2, #+44]
//  766   gptp->tim->EGR  = STM32_TIM_EGR_UG;           /* Update event.            */
        LDR      R2,[R0, #+12]
        MOVS     R3,#+1
        STR      R3,[R2, #+20]
//  767   gptp->tim->SR   = 0;                          /* Clear pending IRQs.      */
        LDR      R2,[R0, #+12]
        MOVS     R3,#+0
        STR      R3,[R2, #+16]
//  768   gptp->tim->CR1  = STM32_TIM_CR1_OPM | STM32_TIM_CR1_URS | STM32_TIM_CR1_CEN;
        LDR      R2,[R0, #+12]
        MOVS     R3,#+13
        STR      R3,[R2, #+0]
//  769   while (!(gptp->tim->SR & STM32_TIM_SR_UIF))
??gpt_lld_polled_delay_0:
        LDR      R2,[R0, #+12]
        LDR      R2,[R2, #+16]
        LSLS     R2,R2,#+31
        BPL.N    ??gpt_lld_polled_delay_0
//  770     ;
//  771 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock7

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "gpt_lld_start(), #1">`:
        DATA
        DC8 "gpt_lld_start(), #1"

        END
//  772 
//  773 #endif /* HAL_USE_GPT */
//  774 
//  775 /** @} */
// 
//  16 bytes in section .bss
//  20 bytes in section .rodata
// 386 bytes in section .text
// 
// 386 bytes of CODE  memory
//  20 bytes of CONST memory
//  16 bytes of DATA  memory
//
//Errors: none
//Warnings: none
