///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:25 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32\TIMv1\pwm_lld.c                      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32\TIMv1\pwm_lld.c -lCN                 /
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
//                       pwm_lld.s                                            /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME pwm_lld

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _port_irq_epilogue
        EXTERN chDbgPanic
        EXTERN dbg_check_enter_isr
        EXTERN dbg_check_leave_isr
        EXTERN nvicDisableVector
        EXTERN nvicEnableVector
        EXTERN pwmObjectInit

        PUBLIC PWMD4
        PUBLIC PWMD8
        PUBLIC VectorB8
        PUBLIC VectorF0
        PUBLIC VectorF8
        PUBLIC pwm_lld_disable_channel
        PUBLIC pwm_lld_enable_channel
        PUBLIC pwm_lld_init
        PUBLIC pwm_lld_start
        PUBLIC pwm_lld_stop
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\platforms\STM32\TIMv1\pwm_lld.c
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
//   18  * @file    STM32/pwm_lld.c
//   19  * @brief   STM32 PWM subsystem low level driver header.
//   20  *
//   21  * @addtogroup PWM
//   22  * @{
//   23  */
//   24 
//   25 #include "ch.h"
//   26 #include "hal.h"
//   27 
//   28 #if HAL_USE_PWM || defined(__DOXYGEN__)
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
//   39  * @brief   PWMD1 driver identifier.
//   40  * @note    The driver PWMD1 allocates the complex timer TIM1 when enabled.
//   41  */
//   42 #if STM32_PWM_USE_TIM1 || defined(__DOXYGEN__)
//   43 PWMDriver PWMD1;
//   44 #endif
//   45 
//   46 /**
//   47  * @brief   PWMD2 driver identifier.
//   48  * @note    The driver PWMD2 allocates the timer TIM2 when enabled.
//   49  */
//   50 #if STM32_PWM_USE_TIM2 || defined(__DOXYGEN__)
//   51 PWMDriver PWMD2;
//   52 #endif
//   53 
//   54 /**
//   55  * @brief   PWMD3 driver identifier.
//   56  * @note    The driver PWMD3 allocates the timer TIM3 when enabled.
//   57  */
//   58 #if STM32_PWM_USE_TIM3 || defined(__DOXYGEN__)
//   59 PWMDriver PWMD3;
//   60 #endif
//   61 
//   62 /**
//   63  * @brief   PWMD4 driver identifier.
//   64  * @note    The driver PWMD4 allocates the timer TIM4 when enabled.
//   65  */
//   66 #if STM32_PWM_USE_TIM4 || defined(__DOXYGEN__)

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   67 PWMDriver PWMD4;
PWMD4:
        DS8 20
//   68 #endif
//   69 
//   70 /**
//   71  * @brief   PWMD5 driver identifier.
//   72  * @note    The driver PWMD5 allocates the timer TIM5 when enabled.
//   73  */
//   74 #if STM32_PWM_USE_TIM5 || defined(__DOXYGEN__)
//   75 PWMDriver PWMD5;
//   76 #endif
//   77 
//   78 /**
//   79  * @brief   PWMD8 driver identifier.
//   80  * @note    The driver PWMD8 allocates the timer TIM8 when enabled.
//   81  */
//   82 #if STM32_PWM_USE_TIM8 || defined(__DOXYGEN__)

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   83 PWMDriver PWMD8;
PWMD8:
        DS8 20
//   84 #endif
//   85 
//   86 /**
//   87  * @brief   PWMD9 driver identifier.
//   88  * @note    The driver PWMD9 allocates the timer TIM9 when enabled.
//   89  */
//   90 #if STM32_PWM_USE_TIM9 || defined(__DOXYGEN__)
//   91 PWMDriver PWMD9;
//   92 #endif
//   93 
//   94 /*===========================================================================*/
//   95 /* Driver local variables and types.                                         */
//   96 /*===========================================================================*/
//   97 
//   98 /*===========================================================================*/
//   99 /* Driver local functions.                                                   */
//  100 /*===========================================================================*/
//  101 
//  102 #if STM32_PWM_USE_TIM2 || STM32_PWM_USE_TIM3 || STM32_PWM_USE_TIM4 ||       \ 
//  103     STM32_PWM_USE_TIM5 || STM32_PWM_USE_TIM9 || defined(__DOXYGEN__)
//  104 /**
//  105  * @brief   Common TIM2...TIM5,TIM9 IRQ handler.
//  106  * @note    It is assumed that the various sources are only activated if the
//  107  *          associated callback pointer is not equal to @p NULL in order to not
//  108  *          perform an extra check in a potentially critical interrupt handler.
//  109  *
//  110  * @param[in] pwmp      pointer to a @p PWMDriver object
//  111  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function pwm_lld_serve_interrupt
        THUMB
//  112 static void pwm_lld_serve_interrupt(PWMDriver *pwmp) {
pwm_lld_serve_interrupt:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  113   uint16_t sr;
//  114 
//  115   sr  = pwmp->tim->SR;
        LDR      R0,[R4, #+16]
        LDR      R0,[R0, #+16]
        MOVS     R5,R0
//  116   sr &= pwmp->tim->DIER & STM32_TIM_DIER_IRQ_MASK;
        LDR      R0,[R4, #+16]
        LDR      R0,[R0, #+12]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        ANDS     R5,R0,R5
//  117   pwmp->tim->SR = ~sr;
        LDR      R0,[R4, #+16]
        UXTH     R5,R5            ;; ZeroExt  R5,R5,#+16,#+16
        MVNS     R1,R5
        STR      R1,[R0, #+16]
//  118   if ((sr & STM32_TIM_SR_CC1IF) != 0)
        LSLS     R0,R5,#+30
        BPL.N    ??pwm_lld_serve_interrupt_0
//  119     pwmp->config->channels[0].callback(pwmp);
        MOVS     R0,R4
        LDR      R1,[R4, #+4]
        LDR      R1,[R1, #+16]
          CFI FunCall
        BLX      R1
//  120   if ((sr & STM32_TIM_SR_CC2IF) != 0)
??pwm_lld_serve_interrupt_0:
        LSLS     R0,R5,#+29
        BPL.N    ??pwm_lld_serve_interrupt_1
//  121     pwmp->config->channels[1].callback(pwmp);
        MOVS     R0,R4
        LDR      R1,[R4, #+4]
        LDR      R1,[R1, #+24]
          CFI FunCall
        BLX      R1
//  122   if ((sr & STM32_TIM_SR_CC3IF) != 0)
??pwm_lld_serve_interrupt_1:
        LSLS     R0,R5,#+28
        BPL.N    ??pwm_lld_serve_interrupt_2
//  123     pwmp->config->channels[2].callback(pwmp);
        MOVS     R0,R4
        LDR      R1,[R4, #+4]
        LDR      R1,[R1, #+32]
          CFI FunCall
        BLX      R1
//  124   if ((sr & STM32_TIM_SR_CC4IF) != 0)
??pwm_lld_serve_interrupt_2:
        LSLS     R0,R5,#+27
        BPL.N    ??pwm_lld_serve_interrupt_3
//  125     pwmp->config->channels[3].callback(pwmp);
        MOVS     R0,R4
        LDR      R1,[R4, #+4]
        LDR      R1,[R1, #+40]
          CFI FunCall
        BLX      R1
//  126   if ((sr & STM32_TIM_SR_UIF) != 0)
??pwm_lld_serve_interrupt_3:
        LSLS     R0,R5,#+31
        BPL.N    ??pwm_lld_serve_interrupt_4
//  127     pwmp->config->callback(pwmp);
        MOVS     R0,R4
        LDR      R1,[R4, #+4]
        LDR      R1,[R1, #+8]
          CFI FunCall
        BLX      R1
//  128 }
??pwm_lld_serve_interrupt_4:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock0
//  129 #endif /* STM32_PWM_USE_TIM2 || ... || STM32_PWM_USE_TIM5 */
//  130 
//  131 /*===========================================================================*/
//  132 /* Driver interrupt handlers.                                                */
//  133 /*===========================================================================*/
//  134 
//  135 #if STM32_PWM_USE_TIM1
//  136 #if !defined(STM32_TIM1_UP_HANDLER)
//  137 #error "STM32_TIM1_UP_HANDLER not defined"
//  138 #endif
//  139 /**
//  140  * @brief   TIM1 update interrupt handler.
//  141  * @note    It is assumed that this interrupt is only activated if the callback
//  142  *          pointer is not equal to @p NULL in order to not perform an extra
//  143  *          check in a potentially critical interrupt handler.
//  144  *
//  145  * @isr
//  146  */
//  147 CH_IRQ_HANDLER(STM32_TIM1_UP_HANDLER) {
//  148 
//  149   CH_IRQ_PROLOGUE();
//  150 
//  151   STM32_TIM1->SR = ~STM32_TIM_SR_UIF;
//  152   PWMD1.config->callback(&PWMD1);
//  153 
//  154   CH_IRQ_EPILOGUE();
//  155 }
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
//  168 CH_IRQ_HANDLER(STM32_TIM1_CC_HANDLER) {
//  169   uint16_t sr;
//  170 
//  171   CH_IRQ_PROLOGUE();
//  172 
//  173   sr = STM32_TIM1->SR & STM32_TIM1->DIER & STM32_TIM_DIER_IRQ_MASK;
//  174   STM32_TIM1->SR = ~sr;
//  175   if ((sr & STM32_TIM_SR_CC1IF) != 0)
//  176     PWMD1.config->channels[0].callback(&PWMD1);
//  177   if ((sr & STM32_TIM_SR_CC2IF) != 0)
//  178     PWMD1.config->channels[1].callback(&PWMD1);
//  179   if ((sr & STM32_TIM_SR_CC3IF) != 0)
//  180     PWMD1.config->channels[2].callback(&PWMD1);
//  181   if ((sr & STM32_TIM_SR_CC4IF) != 0)
//  182     PWMD1.config->channels[3].callback(&PWMD1);
//  183 
//  184   CH_IRQ_EPILOGUE();
//  185 }
//  186 #endif /* STM32_PWM_USE_TIM1 */
//  187 
//  188 #if STM32_PWM_USE_TIM2
//  189 #if !defined(STM32_TIM2_HANDLER)
//  190 #error "STM32_TIM2_HANDLER not defined"
//  191 #endif
//  192 /**
//  193  * @brief   TIM2 interrupt handler.
//  194  *
//  195  * @isr
//  196  */
//  197 CH_IRQ_HANDLER(STM32_TIM2_HANDLER) {
//  198 
//  199   CH_IRQ_PROLOGUE();
//  200 
//  201   pwm_lld_serve_interrupt(&PWMD2);
//  202 
//  203   CH_IRQ_EPILOGUE();
//  204 }
//  205 #endif /* STM32_PWM_USE_TIM2 */
//  206 
//  207 #if STM32_PWM_USE_TIM3
//  208 #if !defined(STM32_TIM3_HANDLER)
//  209 #error "STM32_TIM3_HANDLER not defined"
//  210 #endif
//  211 /**
//  212  * @brief   TIM3 interrupt handler.
//  213  *
//  214  * @isr
//  215  */
//  216 CH_IRQ_HANDLER(STM32_TIM3_HANDLER) {
//  217 
//  218   CH_IRQ_PROLOGUE();
//  219 
//  220   pwm_lld_serve_interrupt(&PWMD3);
//  221 
//  222   CH_IRQ_EPILOGUE();
//  223 }
//  224 #endif /* STM32_PWM_USE_TIM3 */
//  225 
//  226 #if STM32_PWM_USE_TIM4
//  227 #if !defined(STM32_TIM4_HANDLER)
//  228 #error "STM32_TIM4_HANDLER not defined"
//  229 #endif
//  230 /**
//  231  * @brief   TIM4 interrupt handler.
//  232  *
//  233  * @isr
//  234  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function VectorB8
        THUMB
//  235 CH_IRQ_HANDLER(STM32_TIM4_HANDLER) {
VectorB8:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  236 
//  237   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  238 
//  239   pwm_lld_serve_interrupt(&PWMD4);
        LDR.N    R0,??DataTable5
          CFI FunCall pwm_lld_serve_interrupt
        BL       pwm_lld_serve_interrupt
//  240 
//  241   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  242 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock1
//  243 #endif /* STM32_PWM_USE_TIM4 */
//  244 
//  245 #if STM32_PWM_USE_TIM5
//  246 #if !defined(STM32_TIM5_HANDLER)
//  247 #error "STM32_TIM5_HANDLER not defined"
//  248 #endif
//  249 /**
//  250  * @brief   TIM5 interrupt handler.
//  251  *
//  252  * @isr
//  253  */
//  254 CH_IRQ_HANDLER(STM32_TIM5_HANDLER) {
//  255 
//  256   CH_IRQ_PROLOGUE();
//  257 
//  258   pwm_lld_serve_interrupt(&PWMD5);
//  259 
//  260   CH_IRQ_EPILOGUE();
//  261 }
//  262 #endif /* STM32_PWM_USE_TIM5 */
//  263 
//  264 #if STM32_PWM_USE_TIM8
//  265 #if !defined(STM32_TIM8_UP_HANDLER)
//  266 #error "STM32_TIM8_UP_HANDLER not defined"
//  267 #endif
//  268 /**
//  269  * @brief   TIM8 update interrupt handler.
//  270  * @note    It is assumed that this interrupt is only activated if the callback
//  271  *          pointer is not equal to @p NULL in order to not perform an extra
//  272  *          check in a potentially critical interrupt handler.
//  273  *
//  274  * @isr
//  275  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function VectorF0
        THUMB
//  276 CH_IRQ_HANDLER(STM32_TIM8_UP_HANDLER) {
VectorF0:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  277 
//  278   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  279 
//  280   STM32_TIM8->SR = ~TIM_SR_UIF;
        LDR.N    R0,??DataTable5_1  ;; 0x40010410
        MVNS     R1,#+1
        STR      R1,[R0, #+0]
//  281   PWMD8.config->callback(&PWMD8);
        LDR.N    R0,??DataTable5_2
        LDR.N    R1,??DataTable5_2
        LDR      R1,[R1, #+4]
        LDR      R1,[R1, #+8]
          CFI FunCall
        BLX      R1
//  282 
//  283   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  284 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock2
//  285 
//  286 #if !defined(STM32_TIM8_CC_HANDLER)
//  287 #error "STM32_TIM8_CC_HANDLER not defined"
//  288 #endif
//  289 /**
//  290  * @brief   TIM8 compare interrupt handler.
//  291  * @note    It is assumed that the various sources are only activated if the
//  292  *          associated callback pointer is not equal to @p NULL in order to not
//  293  *          perform an extra check in a potentially critical interrupt handler.
//  294  *
//  295  * @isr
//  296  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function VectorF8
        THUMB
//  297 CH_IRQ_HANDLER(STM32_TIM8_CC_HANDLER) {
VectorF8:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  298   uint16_t sr;
//  299 
//  300   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  301 
//  302   sr = STM32_TIM8->SR & STM32_TIM8->DIER & STM32_TIM_DIER_IRQ_MASK;
        LDR.N    R0,??DataTable5_1  ;; 0x40010410
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable5_3  ;; 0x4001040c
        LDR      R1,[R1, #+0]
        ANDS     R0,R1,R0
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        MOVS     R4,R0
//  303   STM32_TIM8->SR = ~sr;
        LDR.N    R0,??DataTable5_1  ;; 0x40010410
        UXTH     R4,R4            ;; ZeroExt  R4,R4,#+16,#+16
        MVNS     R1,R4
        STR      R1,[R0, #+0]
//  304   if ((sr & STM32_TIM_SR_CC1IF) != 0)
        LSLS     R0,R4,#+30
        BPL.N    ??VectorF8_0
//  305     PWMD8.config->channels[0].callback(&PWMD8);
        LDR.N    R0,??DataTable5_2
        LDR.N    R1,??DataTable5_2
        LDR      R1,[R1, #+4]
        LDR      R1,[R1, #+16]
          CFI FunCall
        BLX      R1
//  306   if ((sr & STM32_TIM_SR_CC2IF) != 0)
??VectorF8_0:
        LSLS     R0,R4,#+29
        BPL.N    ??VectorF8_1
//  307     PWMD8.config->channels[1].callback(&PWMD8);
        LDR.N    R0,??DataTable5_2
        LDR.N    R1,??DataTable5_2
        LDR      R1,[R1, #+4]
        LDR      R1,[R1, #+24]
          CFI FunCall
        BLX      R1
//  308   if ((sr & STM32_TIM_SR_CC3IF) != 0)
??VectorF8_1:
        LSLS     R0,R4,#+28
        BPL.N    ??VectorF8_2
//  309     PWMD8.config->channels[2].callback(&PWMD8);
        LDR.N    R0,??DataTable5_2
        LDR.N    R1,??DataTable5_2
        LDR      R1,[R1, #+4]
        LDR      R1,[R1, #+32]
          CFI FunCall
        BLX      R1
//  310   if ((sr & STM32_TIM_SR_CC4IF) != 0)
??VectorF8_2:
        LSLS     R0,R4,#+27
        BPL.N    ??VectorF8_3
//  311     PWMD8.config->channels[3].callback(&PWMD8);
        LDR.N    R0,??DataTable5_2
        LDR.N    R1,??DataTable5_2
        LDR      R1,[R1, #+4]
        LDR      R1,[R1, #+40]
          CFI FunCall
        BLX      R1
//  312 
//  313   CH_IRQ_EPILOGUE();
??VectorF8_3:
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  314 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//  315 #endif /* STM32_PWM_USE_TIM8 */
//  316 
//  317 #if STM32_PWM_USE_TIM9
//  318 #if !defined(STM32_TIM9_HANDLER)
//  319 #error "STM32_TIM9_HANDLER not defined"
//  320 #endif
//  321 /**
//  322  * @brief   TIM9 interrupt handler.
//  323  *
//  324  * @isr
//  325  */
//  326 CH_IRQ_HANDLER(STM32_TIM9_HANDLER) {
//  327 
//  328   CH_IRQ_PROLOGUE();
//  329 
//  330   pwm_lld_serve_interrupt(&PWMD9);
//  331 
//  332   CH_IRQ_EPILOGUE();
//  333 }
//  334 #endif /* STM32_PWM_USE_TIM9 */
//  335 
//  336 /*===========================================================================*/
//  337 /* Driver exported functions.                                                */
//  338 /*===========================================================================*/
//  339 
//  340 /**
//  341  * @brief   Low level PWM driver initialization.
//  342  *
//  343  * @notapi
//  344  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function pwm_lld_init
        THUMB
//  345 void pwm_lld_init(void) {
pwm_lld_init:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  346 
//  347 #if STM32_PWM_USE_TIM1
//  348   /* Driver initialization.*/
//  349   pwmObjectInit(&PWMD1);
//  350   PWMD1.tim = STM32_TIM1;
//  351 #endif
//  352 
//  353 #if STM32_PWM_USE_TIM2
//  354   /* Driver initialization.*/
//  355   pwmObjectInit(&PWMD2);
//  356   PWMD2.tim = STM32_TIM2;
//  357 #endif
//  358 
//  359 #if STM32_PWM_USE_TIM3
//  360   /* Driver initialization.*/
//  361   pwmObjectInit(&PWMD3);
//  362   PWMD3.tim = STM32_TIM3;
//  363 #endif
//  364 
//  365 #if STM32_PWM_USE_TIM4
//  366   /* Driver initialization.*/
//  367   pwmObjectInit(&PWMD4);
        LDR.N    R0,??DataTable5
          CFI FunCall pwmObjectInit
        BL       pwmObjectInit
//  368   PWMD4.tim = STM32_TIM4;
        LDR.N    R0,??DataTable5
        LDR.N    R1,??DataTable5_4  ;; 0x40000800
        STR      R1,[R0, #+16]
//  369 #endif
//  370 
//  371 #if STM32_PWM_USE_TIM5
//  372   /* Driver initialization.*/
//  373   pwmObjectInit(&PWMD5);
//  374   PWMD5.tim = STM32_TIM5;
//  375 #endif
//  376 
//  377 #if STM32_PWM_USE_TIM8
//  378   /* Driver initialization.*/
//  379   pwmObjectInit(&PWMD8);
        LDR.N    R0,??DataTable5_2
          CFI FunCall pwmObjectInit
        BL       pwmObjectInit
//  380   PWMD8.tim = STM32_TIM8;
        LDR.N    R0,??DataTable5_2
        LDR.N    R1,??DataTable5_5  ;; 0x40010400
        STR      R1,[R0, #+16]
//  381 #endif
//  382 
//  383 #if STM32_PWM_USE_TIM9
//  384   /* Driver initialization.*/
//  385   pwmObjectInit(&PWMD9);
//  386   PWMD9.tim = STM32_TIM9;
//  387 #endif
//  388 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock4
//  389 
//  390 /**
//  391  * @brief   Configures and activates the PWM peripheral.
//  392  * @note    Starting a driver that is already in the @p PWM_READY state
//  393  *          disables all the active channels.
//  394  *
//  395  * @param[in] pwmp      pointer to a @p PWMDriver object
//  396  *
//  397  * @notapi
//  398  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function pwm_lld_start
        THUMB
//  399 void pwm_lld_start(PWMDriver *pwmp) {
pwm_lld_start:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  400   uint32_t psc;
//  401   uint32_t ccer;
//  402 
//  403   if (pwmp->state == PWM_STOP) {
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BNE.N    ??pwm_lld_start_0
//  404     /* Clock activation and timer reset.*/
//  405 #if STM32_PWM_USE_TIM1
//  406     if (&PWMD1 == pwmp) {
//  407       rccEnableTIM1(FALSE);
//  408       rccResetTIM1();
//  409       nvicEnableVector(STM32_TIM1_UP_NUMBER,
//  410                        CORTEX_PRIORITY_MASK(STM32_PWM_TIM1_IRQ_PRIORITY));
//  411       nvicEnableVector(STM32_TIM1_CC_NUMBER,
//  412                        CORTEX_PRIORITY_MASK(STM32_PWM_TIM1_IRQ_PRIORITY));
//  413 #if defined(STM32_TIM1CLK)
//  414       pwmp->clock = STM32_TIM1CLK;
//  415 #else
//  416       pwmp->clock = STM32_TIMCLK2;
//  417 #endif
//  418     }
//  419 #endif
//  420 #if STM32_PWM_USE_TIM2
//  421     if (&PWMD2 == pwmp) {
//  422       rccEnableTIM2(FALSE);
//  423       rccResetTIM2();
//  424       nvicEnableVector(STM32_TIM2_NUMBER,
//  425                        CORTEX_PRIORITY_MASK(STM32_PWM_TIM2_IRQ_PRIORITY));
//  426       pwmp->clock = STM32_TIMCLK1;
//  427     }
//  428 #endif
//  429 #if STM32_PWM_USE_TIM3
//  430     if (&PWMD3 == pwmp) {
//  431       rccEnableTIM3(FALSE);
//  432       rccResetTIM3();
//  433       nvicEnableVector(STM32_TIM3_NUMBER,
//  434                        CORTEX_PRIORITY_MASK(STM32_PWM_TIM3_IRQ_PRIORITY));
//  435       pwmp->clock = STM32_TIMCLK1;
//  436     }
//  437 #endif
//  438 #if STM32_PWM_USE_TIM4
//  439     if (&PWMD4 == pwmp) {
        LDR.N    R0,??DataTable5
        CMP      R0,R4
        BNE.N    ??pwm_lld_start_1
//  440       rccEnableTIM4(FALSE);
        LDR.N    R0,??DataTable5_6  ;; 0x40023840
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x4
        LDR.N    R1,??DataTable5_6  ;; 0x40023840
        STR      R0,[R1, #+0]
//  441       rccResetTIM4();
        LDR.N    R0,??DataTable5_7  ;; 0x40023820
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x4
        LDR.N    R1,??DataTable5_7  ;; 0x40023820
        STR      R0,[R1, #+0]
        LDR.N    R0,??DataTable5_7  ;; 0x40023820
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  442       nvicEnableVector(STM32_TIM4_NUMBER,
//  443                        CORTEX_PRIORITY_MASK(STM32_PWM_TIM4_IRQ_PRIORITY));
        MOVS     R1,#+112
        MOVS     R0,#+30
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  444       pwmp->clock = STM32_TIMCLK1;
        LDR.N    R0,??DataTable5_8  ;; 0x501bd00
        STR      R0,[R4, #+12]
//  445     }
//  446 #endif
//  447 
//  448 #if STM32_PWM_USE_TIM5
//  449     if (&PWMD5 == pwmp) {
//  450       rccEnableTIM5(FALSE);
//  451       rccResetTIM5();
//  452       nvicEnableVector(STM32_TIM5_NUMBER,
//  453                        CORTEX_PRIORITY_MASK(STM32_PWM_TIM5_IRQ_PRIORITY));
//  454       pwmp->clock = STM32_TIMCLK1;
//  455     }
//  456 #endif
//  457 #if STM32_PWM_USE_TIM8
//  458     if (&PWMD8 == pwmp) {
??pwm_lld_start_1:
        LDR.N    R0,??DataTable5_2
        CMP      R0,R4
        BNE.N    ??pwm_lld_start_2
//  459       rccEnableTIM8(FALSE);
        LDR.N    R0,??DataTable5_9  ;; 0x40023844
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x2
        LDR.N    R1,??DataTable5_9  ;; 0x40023844
        STR      R0,[R1, #+0]
//  460       rccResetTIM8();
        LDR.N    R0,??DataTable5_10  ;; 0x40023824
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x2
        LDR.N    R1,??DataTable5_10  ;; 0x40023824
        STR      R0,[R1, #+0]
        LDR.N    R0,??DataTable5_10  ;; 0x40023824
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  461       nvicEnableVector(STM32_TIM8_UP_NUMBER,
//  462                        CORTEX_PRIORITY_MASK(STM32_PWM_TIM8_IRQ_PRIORITY));
        MOVS     R1,#+112
        MOVS     R0,#+44
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  463       nvicEnableVector(STM32_TIM8_CC_NUMBER,
//  464                        CORTEX_PRIORITY_MASK(STM32_PWM_TIM8_IRQ_PRIORITY));
        MOVS     R1,#+112
        MOVS     R0,#+46
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  465 #if defined(STM32_TIM8CLK)
//  466       pwmp->clock = STM32_TIM8CLK;
//  467 #else
//  468       pwmp->clock = STM32_TIMCLK2;
        LDR.N    R0,??DataTable5_11  ;; 0xa037a00
        STR      R0,[R4, #+12]
//  469 #endif
//  470     }
//  471 #endif
//  472 #if STM32_PWM_USE_TIM9
//  473     if (&PWMD9 == pwmp) {
//  474       rccEnableTIM9(FALSE);
//  475       rccResetTIM9();
//  476       nvicEnableVector(STM32_TIM9_NUMBER,
//  477                        CORTEX_PRIORITY_MASK(STM32_PWM_TIM9_IRQ_PRIORITY));
//  478       pwmp->clock = STM32_TIMCLK2;
//  479     }
//  480 #endif
//  481 
//  482     /* All channels configured in PWM1 mode with preload enabled and will
//  483        stay that way until the driver is stopped.*/
//  484     pwmp->tim->CCMR1 = STM32_TIM_CCMR1_OC1M(6) | STM32_TIM_CCMR1_OC1PE |
//  485                        STM32_TIM_CCMR1_OC2M(6) | STM32_TIM_CCMR1_OC2PE;
??pwm_lld_start_2:
        LDR      R0,[R4, #+16]
        MOVW     R1,#+26728
        STR      R1,[R0, #+24]
//  486     pwmp->tim->CCMR2 = STM32_TIM_CCMR2_OC3M(6) | STM32_TIM_CCMR2_OC3PE |
//  487                        STM32_TIM_CCMR2_OC4M(6) | STM32_TIM_CCMR2_OC4PE;
        LDR      R0,[R4, #+16]
        MOVW     R1,#+26728
        STR      R1,[R0, #+28]
        B.N      ??pwm_lld_start_3
//  488   }
//  489   else {
//  490     /* Driver re-configuration scenario, it must be stopped first.*/
//  491     pwmp->tim->CR1    = 0;                  /* Timer disabled.              */
??pwm_lld_start_0:
        LDR      R0,[R4, #+16]
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  492     pwmp->tim->CCR[0] = 0;                  /* Comparator 1 disabled.       */
        LDR      R0,[R4, #+16]
        MOVS     R1,#+0
        STR      R1,[R0, #+52]
//  493     pwmp->tim->CCR[1] = 0;                  /* Comparator 2 disabled.       */
        LDR      R0,[R4, #+16]
        MOVS     R1,#+0
        STR      R1,[R0, #+56]
//  494     pwmp->tim->CCR[2] = 0;                  /* Comparator 3 disabled.       */
        LDR      R0,[R4, #+16]
        MOVS     R1,#+0
        STR      R1,[R0, #+60]
//  495     pwmp->tim->CCR[3] = 0;                  /* Comparator 4 disabled.       */
        LDR      R0,[R4, #+16]
        MOVS     R1,#+0
        STR      R1,[R0, #+64]
//  496     pwmp->tim->CNT  = 0;                    /* Counter reset to zero.       */
        LDR      R0,[R4, #+16]
        MOVS     R1,#+0
        STR      R1,[R0, #+36]
//  497   }
//  498 
//  499   /* Timer configuration.*/
//  500   psc = (pwmp->clock / pwmp->config->frequency) - 1;
??pwm_lld_start_3:
        LDR      R0,[R4, #+12]
        LDR      R1,[R4, #+4]
        LDR      R1,[R1, #+0]
        UDIV     R0,R0,R1
        SUBS     R0,R0,#+1
        MOVS     R5,R0
//  501   chDbgAssert((psc <= 0xFFFF) &&
//  502               ((psc + 1) * pwmp->config->frequency) == pwmp->clock,
//  503               "pwm_lld_start(), #1", "invalid frequency");
        CMP      R5,#+65536
        BCS.N    ??pwm_lld_start_4
        ADDS     R0,R5,#+1
        LDR      R1,[R4, #+4]
        LDR      R1,[R1, #+0]
        MULS     R0,R1,R0
        LDR      R1,[R4, #+12]
        CMP      R0,R1
        BEQ.N    ??pwm_lld_start_5
??pwm_lld_start_4:
        LDR.N    R0,??DataTable5_12
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  504   pwmp->tim->PSC  = (uint16_t)psc;
??pwm_lld_start_5:
        LDR      R0,[R4, #+16]
        MOVS     R1,R5
        UXTH     R1,R1            ;; ZeroExt  R1,R1,#+16,#+16
        STR      R1,[R0, #+40]
//  505   pwmp->tim->ARR  = (uint16_t)(pwmp->period - 1);
        LDR      R0,[R4, #+16]
        LDRH     R1,[R4, #+8]
        SUBS     R1,R1,#+1
        UXTH     R1,R1            ;; ZeroExt  R1,R1,#+16,#+16
        STR      R1,[R0, #+44]
//  506   pwmp->tim->CR2  = pwmp->config->cr2;
        LDR      R0,[R4, #+16]
        LDR      R1,[R4, #+4]
        LDR      R1,[R1, #+44]
        STR      R1,[R0, #+4]
//  507 
//  508   /* Output enables and polarities setup.*/
//  509   ccer = 0;
        MOVS     R0,#+0
        MOVS     R6,R0
//  510   switch (pwmp->config->channels[0].mode & PWM_OUTPUT_MASK) {
        LDR      R0,[R4, #+4]
        LDRB     R0,[R0, #+12]
        ANDS     R0,R0,#0xF
        CMP      R0,#+1
        BEQ.N    ??pwm_lld_start_6
        CMP      R0,#+2
        BNE.N    ??pwm_lld_start_7
//  511   case PWM_OUTPUT_ACTIVE_LOW:
//  512     ccer |= STM32_TIM_CCER_CC1P;
??pwm_lld_start_8:
        ORRS     R6,R6,#0x2
//  513   case PWM_OUTPUT_ACTIVE_HIGH:
//  514     ccer |= STM32_TIM_CCER_CC1E;
??pwm_lld_start_6:
        ORRS     R6,R6,#0x1
//  515   default:
//  516     ;
//  517   }
//  518   switch (pwmp->config->channels[1].mode & PWM_OUTPUT_MASK) {
??pwm_lld_start_7:
        LDR      R0,[R4, #+4]
        LDRB     R0,[R0, #+20]
        ANDS     R0,R0,#0xF
        CMP      R0,#+1
        BEQ.N    ??pwm_lld_start_9
        CMP      R0,#+2
        BNE.N    ??pwm_lld_start_10
//  519   case PWM_OUTPUT_ACTIVE_LOW:
//  520     ccer |= STM32_TIM_CCER_CC2P;
??pwm_lld_start_11:
        ORRS     R6,R6,#0x20
//  521   case PWM_OUTPUT_ACTIVE_HIGH:
//  522     ccer |= STM32_TIM_CCER_CC2E;
??pwm_lld_start_9:
        ORRS     R6,R6,#0x10
//  523   default:
//  524     ;
//  525   }
//  526   switch (pwmp->config->channels[2].mode & PWM_OUTPUT_MASK) {
??pwm_lld_start_10:
        LDR      R0,[R4, #+4]
        LDRB     R0,[R0, #+28]
        ANDS     R0,R0,#0xF
        CMP      R0,#+1
        BEQ.N    ??pwm_lld_start_12
        CMP      R0,#+2
        BNE.N    ??pwm_lld_start_13
//  527   case PWM_OUTPUT_ACTIVE_LOW:
//  528     ccer |= STM32_TIM_CCER_CC3P;
??pwm_lld_start_14:
        ORRS     R6,R6,#0x200
//  529   case PWM_OUTPUT_ACTIVE_HIGH:
//  530     ccer |= STM32_TIM_CCER_CC3E;
??pwm_lld_start_12:
        ORRS     R6,R6,#0x100
//  531   default:
//  532     ;
//  533   }
//  534   switch (pwmp->config->channels[3].mode & PWM_OUTPUT_MASK) {
??pwm_lld_start_13:
        LDR      R0,[R4, #+4]
        LDRB     R0,[R0, #+36]
        ANDS     R0,R0,#0xF
        CMP      R0,#+1
        BEQ.N    ??pwm_lld_start_15
        CMP      R0,#+2
        BNE.N    ??pwm_lld_start_16
//  535   case PWM_OUTPUT_ACTIVE_LOW:
//  536     ccer |= STM32_TIM_CCER_CC4P;
??pwm_lld_start_17:
        ORRS     R6,R6,#0x2000
//  537   case PWM_OUTPUT_ACTIVE_HIGH:
//  538     ccer |= STM32_TIM_CCER_CC4E;
??pwm_lld_start_15:
        ORRS     R6,R6,#0x1000
//  539   default:
//  540     ;
//  541   }
//  542 #if STM32_PWM_USE_ADVANCED
//  543 #if STM32_PWM_USE_TIM1 && !STM32_PWM_USE_TIM8
//  544   if (&PWMD1 == pwmp) {
//  545 #endif
//  546 #if !STM32_PWM_USE_TIM1 && STM32_PWM_USE_TIM8
//  547   if (&PWMD8 == pwmp) {
//  548 #endif
//  549 #if STM32_PWM_USE_TIM1 && STM32_PWM_USE_TIM8
//  550   if ((&PWMD1 == pwmp) || (&PWMD8 == pwmp)) {
//  551 #endif
//  552     switch (pwmp->config->channels[0].mode & PWM_COMPLEMENTARY_OUTPUT_MASK) {
//  553     case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_LOW:
//  554       ccer |= STM32_TIM_CCER_CC1NP;
//  555     case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH:
//  556       ccer |= STM32_TIM_CCER_CC1NE;
//  557     default:
//  558       ;
//  559     }
//  560     switch (pwmp->config->channels[1].mode & PWM_COMPLEMENTARY_OUTPUT_MASK) {
//  561     case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_LOW:
//  562       ccer |= STM32_TIM_CCER_CC2NP;
//  563     case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH:
//  564       ccer |= STM32_TIM_CCER_CC2NE;
//  565     default:
//  566       ;
//  567     }
//  568     switch (pwmp->config->channels[2].mode & PWM_COMPLEMENTARY_OUTPUT_MASK) {
//  569     case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_LOW:
//  570       ccer |= STM32_TIM_CCER_CC3NP;
//  571     case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH:
//  572       ccer |= STM32_TIM_CCER_CC3NE;
//  573     default:
//  574       ;
//  575     }
//  576   }
//  577 #endif /* STM32_PWM_USE_ADVANCED*/
//  578 
//  579   pwmp->tim->CCER  = ccer;
??pwm_lld_start_16:
        LDR      R0,[R4, #+16]
        STR      R6,[R0, #+32]
//  580   pwmp->tim->EGR   = STM32_TIM_EGR_UG;      /* Update event.                */
        LDR      R0,[R4, #+16]
        MOVS     R1,#+1
        STR      R1,[R0, #+20]
//  581   pwmp->tim->SR    = 0;                     /* Clear pending IRQs.          */
        LDR      R0,[R4, #+16]
        MOVS     R1,#+0
        STR      R1,[R0, #+16]
//  582   pwmp->tim->DIER  = (pwmp->config->callback == NULL ? 0 : STM32_TIM_DIER_UIE) |
//  583                      (pwmp->config->dier & ~STM32_TIM_DIER_IRQ_MASK);
        LDR      R0,[R4, #+4]
        LDR      R0,[R0, #+8]
        CMP      R0,#+0
        BNE.N    ??pwm_lld_start_18
        MOVS     R0,#+0
        B.N      ??pwm_lld_start_19
??pwm_lld_start_18:
        MOVS     R0,#+1
??pwm_lld_start_19:
        LDR      R1,[R4, #+4]
        LDR      R1,[R1, #+48]
        LSRS     R1,R1,#+8
        LSLS     R1,R1,#+8
        ORRS     R0,R1,R0
        LDR      R1,[R4, #+16]
        STR      R0,[R1, #+12]
//  584 #if STM32_PWM_USE_TIM1 || STM32_PWM_USE_TIM8
//  585 #if STM32_PWM_USE_ADVANCED
//  586   pwmp->tim->BDTR  = pwmp->config->bdtr | STM32_TIM_BDTR_MOE;
//  587 #else
//  588   pwmp->tim->BDTR  = STM32_TIM_BDTR_MOE;
        LDR      R0,[R4, #+16]
        MOV      R1,#+32768
        STR      R1,[R0, #+68]
//  589 #endif
//  590 #endif
//  591   /* Timer configured and started.*/
//  592   pwmp->tim->CR1   = STM32_TIM_CR1_ARPE | STM32_TIM_CR1_URS |
//  593                      STM32_TIM_CR1_CEN;
        LDR      R0,[R4, #+16]
        MOVS     R1,#+133
        STR      R1,[R0, #+0]
//  594 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock5
//  595 
//  596 /**
//  597  * @brief   Deactivates the PWM peripheral.
//  598  *
//  599  * @param[in] pwmp      pointer to a @p PWMDriver object
//  600  *
//  601  * @notapi
//  602  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function pwm_lld_stop
        THUMB
//  603 void pwm_lld_stop(PWMDriver *pwmp) {
pwm_lld_stop:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  604 
//  605   /* If in ready state then disables the PWM clock.*/
//  606   if (pwmp->state == PWM_READY) {
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BNE.N    ??pwm_lld_stop_0
//  607     pwmp->tim->CR1  = 0;                    /* Timer disabled.              */
        LDR      R0,[R4, #+16]
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  608     pwmp->tim->DIER = 0;                    /* All IRQs disabled.           */
        LDR      R0,[R4, #+16]
        MOVS     R1,#+0
        STR      R1,[R0, #+12]
//  609     pwmp->tim->SR   = 0;                    /* Clear eventual pending IRQs. */
        LDR      R0,[R4, #+16]
        MOVS     R1,#+0
        STR      R1,[R0, #+16]
//  610 #if STM32_PWM_USE_TIM1 || STM32_PWM_USE_TIM8
//  611     pwmp->tim->BDTR  = 0;
        LDR      R0,[R4, #+16]
        MOVS     R1,#+0
        STR      R1,[R0, #+68]
//  612 #endif
//  613 
//  614 #if STM32_PWM_USE_TIM1
//  615     if (&PWMD1 == pwmp) {
//  616       nvicDisableVector(STM32_TIM1_UP_NUMBER);
//  617       nvicDisableVector(STM32_TIM1_CC_NUMBER);
//  618       rccDisableTIM1(FALSE);
//  619     }
//  620 #endif
//  621 #if STM32_PWM_USE_TIM2
//  622     if (&PWMD2 == pwmp) {
//  623       nvicDisableVector(STM32_TIM2_NUMBER);
//  624       rccDisableTIM2(FALSE);
//  625     }
//  626 #endif
//  627 #if STM32_PWM_USE_TIM3
//  628     if (&PWMD3 == pwmp) {
//  629       nvicDisableVector(STM32_TIM3_NUMBER);
//  630       rccDisableTIM3(FALSE);
//  631     }
//  632 #endif
//  633 #if STM32_PWM_USE_TIM4
//  634     if (&PWMD4 == pwmp) {
        LDR.N    R0,??DataTable5
        CMP      R0,R4
        BNE.N    ??pwm_lld_stop_1
//  635       nvicDisableVector(STM32_TIM4_NUMBER);
        MOVS     R0,#+30
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  636       rccDisableTIM4(FALSE);
        LDR.N    R0,??DataTable5_6  ;; 0x40023840
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x4
        LDR.N    R1,??DataTable5_6  ;; 0x40023840
        STR      R0,[R1, #+0]
//  637     }
//  638 #endif
//  639 #if STM32_PWM_USE_TIM5
//  640     if (&PWMD5 == pwmp) {
//  641       nvicDisableVector(STM32_TIM5_NUMBER);
//  642       rccDisableTIM5(FALSE);
//  643     }
//  644 #endif
//  645 #if STM32_PWM_USE_TIM8
//  646     if (&PWMD8 == pwmp) {
??pwm_lld_stop_1:
        LDR.N    R0,??DataTable5_2
        CMP      R0,R4
        BNE.N    ??pwm_lld_stop_0
//  647       nvicDisableVector(STM32_TIM8_UP_NUMBER);
        MOVS     R0,#+44
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  648       nvicDisableVector(STM32_TIM8_CC_NUMBER);
        MOVS     R0,#+46
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  649       rccDisableTIM8(FALSE);
        LDR.N    R0,??DataTable5_9  ;; 0x40023844
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x2
        LDR.N    R1,??DataTable5_9  ;; 0x40023844
        STR      R0,[R1, #+0]
//  650     }
//  651 #endif
//  652 #if STM32_PWM_USE_TIM9
//  653     if (&PWMD9 == pwmp) {
//  654       nvicDisableVector(STM32_TIM9_NUMBER);
//  655       rccDisableTIM9(FALSE);
//  656     }
//  657 #endif
//  658   }
//  659 }
??pwm_lld_stop_0:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock6

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5:
        DC32     PWMD4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_1:
        DC32     0x40010410

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_2:
        DC32     PWMD8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_3:
        DC32     0x4001040c

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_4:
        DC32     0x40000800

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_5:
        DC32     0x40010400

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_6:
        DC32     0x40023840

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_7:
        DC32     0x40023820

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_8:
        DC32     0x501bd00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_9:
        DC32     0x40023844

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_10:
        DC32     0x40023824

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_11:
        DC32     0xa037a00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_12:
        DC32     `?<Constant "pwm_lld_start(), #1">`
//  660 
//  661 /**
//  662  * @brief   Enables a PWM channel.
//  663  * @pre     The PWM unit must have been activated using @p pwmStart().
//  664  * @post    The channel is active using the specified configuration.
//  665  * @note    The function has effect at the next cycle start.
//  666  *
//  667  * @param[in] pwmp      pointer to a @p PWMDriver object
//  668  * @param[in] channel   PWM channel identifier (0...PWM_CHANNELS-1)
//  669  * @param[in] width     PWM pulse width as clock pulses number
//  670  *
//  671  * @notapi
//  672  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function pwm_lld_enable_channel
          CFI NoCalls
        THUMB
//  673 void pwm_lld_enable_channel(PWMDriver *pwmp,
//  674                             pwmchannel_t channel,
//  675                             pwmcnt_t width) {
pwm_lld_enable_channel:
        PUSH     {R4,R5}
          CFI R5 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  676 
//  677   pwmp->tim->CCR[channel] = width;                  /* New duty cycle.      */
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR      R3,[R0, #+16]
        ADDS     R3,R3,R1, LSL #+2
        UXTH     R2,R2            ;; ZeroExt  R2,R2,#+16,#+16
        STR      R2,[R3, #+52]
//  678   /* If there is a callback defined for the channel then the associated
//  679      interrupt must be enabled.*/
//  680   if (pwmp->config->channels[channel].callback != NULL) {
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR      R3,[R0, #+4]
        ADDS     R3,R3,R1, LSL #+3
        LDR      R3,[R3, #+16]
        CMP      R3,#+0
        BEQ.N    ??pwm_lld_enable_channel_0
//  681     uint32_t dier = pwmp->tim->DIER;
        LDR      R3,[R0, #+16]
        LDR      R3,[R3, #+12]
//  682     /* If the IRQ is not already enabled care must be taken to clear it,
//  683        it is probably already pending because the timer is running.*/
//  684     if ((dier & (2 << channel)) == 0) {
        MOVS     R4,#+2
        LSLS     R4,R4,R1
        TST      R3,R4
        BNE.N    ??pwm_lld_enable_channel_0
//  685       pwmp->tim->DIER = dier | (2 << channel);
        MOVS     R4,#+2
        LSLS     R4,R4,R1
        ORRS     R4,R4,R3
        LDR      R5,[R0, #+16]
        STR      R4,[R5, #+12]
//  686       pwmp->tim->SR   = ~(2 << channel);
        LDR      R4,[R0, #+16]
        MOVS     R5,#+2
        LSLS     R5,R5,R1
        MVNS     R5,R5
        STR      R5,[R4, #+16]
//  687     }
//  688   }
//  689 }
??pwm_lld_enable_channel_0:
        POP      {R4,R5}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock7
//  690 
//  691 /**
//  692  * @brief   Disables a PWM channel.
//  693  * @pre     The PWM unit must have been activated using @p pwmStart().
//  694  * @post    The channel is disabled and its output line returned to the
//  695  *          idle state.
//  696  * @note    The function has effect at the next cycle start.
//  697  *
//  698  * @param[in] pwmp      pointer to a @p PWMDriver object
//  699  * @param[in] channel   PWM channel identifier (0...PWM_CHANNELS-1)
//  700  *
//  701  * @notapi
//  702  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function pwm_lld_disable_channel
          CFI NoCalls
        THUMB
//  703 void pwm_lld_disable_channel(PWMDriver *pwmp, pwmchannel_t channel) {
//  704 
//  705   pwmp->tim->CCR[channel] = 0;
pwm_lld_disable_channel:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR      R2,[R0, #+16]
        ADDS     R2,R2,R1, LSL #+2
        MOVS     R3,#+0
        STR      R3,[R2, #+52]
//  706   pwmp->tim->DIER &= ~(2 << channel);
        LDR      R2,[R0, #+16]
        LDR      R2,[R2, #+12]
        MOVS     R3,#+2
        LSLS     R3,R3,R1
        BICS     R2,R2,R3
        LDR      R3,[R0, #+16]
        STR      R2,[R3, #+12]
//  707 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock8

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "pwm_lld_start(), #1">`:
        DATA
        DC8 "pwm_lld_start(), #1"

        END
//  708 
//  709 #endif /* HAL_USE_PWM */
//  710 
//  711 /** @} */
// 
//  40 bytes in section .bss
//  20 bytes in section .rodata
// 896 bytes in section .text
// 
// 896 bytes of CODE  memory
//  20 bytes of CONST memory
//  40 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
