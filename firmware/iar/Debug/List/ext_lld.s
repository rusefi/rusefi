///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:49 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32\ext_lld.c                            /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32\ext_lld.c -lCN                       /
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
//                       ext_lld.s                                            /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME ext_lld

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN extObjectInit
        EXTERN ext_lld_exti_irq_disable
        EXTERN ext_lld_exti_irq_enable

        PUBLIC EXTD1
        PUBLIC ext_lld_channel_disable
        PUBLIC ext_lld_channel_enable
        PUBLIC ext_lld_init
        PUBLIC ext_lld_start
        PUBLIC ext_lld_stop
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\platforms\STM32\ext_lld.c
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
//   18  * @file    STM32/ext_lld.c
//   19  * @brief   STM32 EXT subsystem low level driver source.
//   20  *
//   21  * @addtogroup EXT
//   22  * @{
//   23  */
//   24 
//   25 #include "ch.h"
//   26 #include "hal.h"
//   27 
//   28 #if HAL_USE_EXT || defined(__DOXYGEN__)
//   29 
//   30 #include "ext_lld_isr.h"
//   31 
//   32 /*===========================================================================*/
//   33 /* Driver local definitions.                                                 */
//   34 /*===========================================================================*/
//   35 
//   36 /*===========================================================================*/
//   37 /* Driver exported variables.                                                */
//   38 /*===========================================================================*/
//   39 
//   40 /**
//   41  * @brief   EXTD1 driver identifier.
//   42  */

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   43 EXTDriver EXTD1;
EXTD1:
        DS8 8
//   44 
//   45 /*===========================================================================*/
//   46 /* Driver local variables and types.                                         */
//   47 /*===========================================================================*/
//   48 
//   49 /*===========================================================================*/
//   50 /* Driver local functions.                                                   */
//   51 /*===========================================================================*/
//   52 
//   53 /*===========================================================================*/
//   54 /* Driver interrupt handlers.                                                */
//   55 /*===========================================================================*/
//   56 
//   57 /*===========================================================================*/
//   58 /* Driver exported functions.                                                */
//   59 /*===========================================================================*/
//   60 
//   61 /**
//   62  * @brief   Low level EXT driver initialization.
//   63  *
//   64  * @notapi
//   65  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function ext_lld_init
        THUMB
//   66 void ext_lld_init(void) {
ext_lld_init:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   67 
//   68   /* Driver initialization.*/
//   69   extObjectInit(&EXTD1);
        LDR.N    R0,??DataTable3
          CFI FunCall extObjectInit
        BL       extObjectInit
//   70 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0
//   71 
//   72 /**
//   73  * @brief   Configures and activates the EXT peripheral.
//   74  *
//   75  * @param[in] extp      pointer to the @p EXTDriver object
//   76  *
//   77  * @notapi
//   78  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function ext_lld_start
        THUMB
//   79 void ext_lld_start(EXTDriver *extp) {
ext_lld_start:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//   80   unsigned i;
//   81 
//   82   if (extp->state == EXT_STOP)
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BNE.N    ??ext_lld_start_0
//   83     ext_lld_exti_irq_enable();
          CFI FunCall ext_lld_exti_irq_enable
        BL       ext_lld_exti_irq_enable
//   84 
//   85   /* Configuration of automatic channels.*/
//   86   for (i = 0; i < EXT_MAX_CHANNELS; i++)
??ext_lld_start_0:
        MOVS     R0,#+0
        MOVS     R5,R0
??ext_lld_start_1:
        CMP      R5,#+23
        BCS.N    ??ext_lld_start_2
//   87     if (extp->config->channels[i].mode & EXT_CH_MODE_AUTOSTART)
        LDR      R0,[R4, #+4]
        LDRB     R0,[R0, R5, LSL #+3]
        LSLS     R0,R0,#+29
        BPL.N    ??ext_lld_start_3
//   88       ext_lld_channel_enable(extp, i);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall ext_lld_channel_enable
        BL       ext_lld_channel_enable
        B.N      ??ext_lld_start_4
//   89     else
//   90       ext_lld_channel_disable(extp, i);
??ext_lld_start_3:
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall ext_lld_channel_disable
        BL       ext_lld_channel_disable
??ext_lld_start_4:
        ADDS     R5,R5,#+1
        B.N      ??ext_lld_start_1
//   91 }
??ext_lld_start_2:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock1
//   92 
//   93 /**
//   94  * @brief   Deactivates the EXT peripheral.
//   95  *
//   96  * @param[in] extp      pointer to the @p EXTDriver object
//   97  *
//   98  * @notapi
//   99  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function ext_lld_stop
        THUMB
//  100 void ext_lld_stop(EXTDriver *extp) {
ext_lld_stop:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  101 
//  102   if (extp->state == EXT_ACTIVE)
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BNE.N    ??ext_lld_stop_0
//  103     ext_lld_exti_irq_disable();
          CFI FunCall ext_lld_exti_irq_disable
        BL       ext_lld_exti_irq_disable
//  104 
//  105   EXTI->EMR = 0;
??ext_lld_stop_0:
        LDR.N    R0,??DataTable3_1  ;; 0x40013c04
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  106   EXTI->IMR = 0;
        LDR.N    R0,??DataTable3_2  ;; 0x40013c00
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  107   EXTI->PR  = 0xFFFFFFFF;
        LDR.N    R0,??DataTable3_3  ;; 0x40013c14
        MOVS     R1,#-1
        STR      R1,[R0, #+0]
//  108 #if STM32_EXTI_NUM_CHANNELS > 32
//  109   EXTI->PR2 = 0xFFFFFFFF;
//  110 #endif
//  111 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2
//  112 
//  113 /**
//  114  * @brief   Enables an EXT channel.
//  115  *
//  116  * @param[in] extp      pointer to the @p EXTDriver object
//  117  * @param[in] channel   channel to be enabled
//  118  *
//  119  * @notapi
//  120  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function ext_lld_channel_enable
          CFI NoCalls
        THUMB
//  121 void ext_lld_channel_enable(EXTDriver *extp, expchannel_t channel) {
ext_lld_channel_enable:
        PUSH     {R4,R5}
          CFI R5 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  122 
//  123   /* Setting the associated GPIO for external channels.*/
//  124   if (channel < 16) {
        CMP      R1,#+16
        BCS.N    ??ext_lld_channel_enable_0
//  125     uint32_t n = channel >> 2;
        LSRS     R2,R1,#+2
//  126     uint32_t mask = ~(0xF << ((channel & 3) * 4));
        MOVS     R3,#+15
        ANDS     R4,R1,#0x3
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        LSLS     R4,R4,#+2
        LSLS     R3,R3,R4
        MVNS     R3,R3
//  127     uint32_t port = ((extp->config->channels[channel].mode &
//  128                       EXT_MODE_GPIO_MASK) >>
//  129                      EXT_MODE_GPIO_OFF) << ((channel & 3) * 4);
        LDR      R4,[R0, #+4]
        LDR      R4,[R4, R1, LSL #+3]
        UBFX     R4,R4,#+4,#+4
        ANDS     R5,R1,#0x3
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        LSLS     R5,R5,#+2
        LSLS     R4,R4,R5
//  130 
//  131 #if defined(STM32F10X_LD_VL) || defined(STM32F10X_MD_VL) ||                 \ 
//  132     defined(STM32F10X_HD_VL) || defined(STM32F10X_LD)    ||                 \ 
//  133     defined(STM32F10X_MD)    || defined(STM32F10X_HD)    ||                 \ 
//  134     defined(STM32F10X_XL)    || defined(STM32F10X_CL)
//  135     AFIO->EXTICR[n] = (AFIO->EXTICR[n] & mask) | port;
//  136 #else /* !defined(STM32F1XX) */
//  137     SYSCFG->EXTICR[n] = (SYSCFG->EXTICR[n] & mask) | port;
        LDR.N    R5,??DataTable3_4  ;; 0x40013808
        LDR      R5,[R5, R2, LSL #+2]
        ANDS     R3,R3,R5
        ORRS     R3,R4,R3
        LDR.N    R4,??DataTable3_4  ;; 0x40013808
        STR      R3,[R4, R2, LSL #+2]
//  138 #endif /* !defined(STM32F1XX) */
//  139   }
//  140 
//  141 #if STM32_EXTI_NUM_CHANNELS > 32
//  142   if (channel < 32) {
//  143 #endif
//  144     /* Programming edge registers.*/
//  145     if (extp->config->channels[channel].mode & EXT_CH_MODE_RISING_EDGE)
??ext_lld_channel_enable_0:
        LDR      R2,[R0, #+4]
        LDRB     R2,[R2, R1, LSL #+3]
        LSLS     R2,R2,#+31
        BPL.N    ??ext_lld_channel_enable_1
//  146       EXTI->RTSR |= (1 << channel);
        LDR.N    R2,??DataTable3_5  ;; 0x40013c08
        LDR      R2,[R2, #+0]
        MOVS     R3,#+1
        LSLS     R3,R3,R1
        ORRS     R2,R3,R2
        LDR.N    R3,??DataTable3_5  ;; 0x40013c08
        STR      R2,[R3, #+0]
        B.N      ??ext_lld_channel_enable_2
//  147     else
//  148       EXTI->RTSR &= ~(1 << channel);
??ext_lld_channel_enable_1:
        LDR.N    R2,??DataTable3_5  ;; 0x40013c08
        LDR      R2,[R2, #+0]
        MOVS     R3,#+1
        LSLS     R3,R3,R1
        BICS     R2,R2,R3
        LDR.N    R3,??DataTable3_5  ;; 0x40013c08
        STR      R2,[R3, #+0]
//  149     if (extp->config->channels[channel].mode & EXT_CH_MODE_FALLING_EDGE)
??ext_lld_channel_enable_2:
        LDR      R2,[R0, #+4]
        LDRB     R2,[R2, R1, LSL #+3]
        LSLS     R2,R2,#+30
        BPL.N    ??ext_lld_channel_enable_3
//  150       EXTI->FTSR |= (1 << channel);
        LDR.N    R2,??DataTable3_6  ;; 0x40013c0c
        LDR      R2,[R2, #+0]
        MOVS     R3,#+1
        LSLS     R3,R3,R1
        ORRS     R2,R3,R2
        LDR.N    R3,??DataTable3_6  ;; 0x40013c0c
        STR      R2,[R3, #+0]
        B.N      ??ext_lld_channel_enable_4
//  151     else
//  152       EXTI->FTSR &= ~(1 << channel);
??ext_lld_channel_enable_3:
        LDR.N    R2,??DataTable3_6  ;; 0x40013c0c
        LDR      R2,[R2, #+0]
        MOVS     R3,#+1
        LSLS     R3,R3,R1
        BICS     R2,R2,R3
        LDR.N    R3,??DataTable3_6  ;; 0x40013c0c
        STR      R2,[R3, #+0]
//  153 
//  154     /* Programming interrupt and event registers.*/
//  155     if (extp->config->channels[channel].cb != NULL) {
??ext_lld_channel_enable_4:
        LDR      R2,[R0, #+4]
        ADDS     R2,R2,R1, LSL #+3
        LDR      R2,[R2, #+4]
        CMP      R2,#+0
        BEQ.N    ??ext_lld_channel_enable_5
//  156       EXTI->IMR |= (1 << channel);
        LDR.N    R2,??DataTable3_2  ;; 0x40013c00
        LDR      R2,[R2, #+0]
        MOVS     R3,#+1
        LSLS     R3,R3,R1
        ORRS     R2,R3,R2
        LDR.N    R3,??DataTable3_2  ;; 0x40013c00
        STR      R2,[R3, #+0]
//  157       EXTI->EMR &= ~(1 << channel);
        LDR.N    R2,??DataTable3_1  ;; 0x40013c04
        LDR      R2,[R2, #+0]
        MOVS     R3,#+1
        LSLS     R3,R3,R1
        BICS     R2,R2,R3
        LDR.N    R3,??DataTable3_1  ;; 0x40013c04
        STR      R2,[R3, #+0]
        B.N      ??ext_lld_channel_enable_6
//  158     }
//  159     else {
//  160       EXTI->EMR |= (1 << channel);
??ext_lld_channel_enable_5:
        LDR.N    R2,??DataTable3_1  ;; 0x40013c04
        LDR      R2,[R2, #+0]
        MOVS     R3,#+1
        LSLS     R3,R3,R1
        ORRS     R2,R3,R2
        LDR.N    R3,??DataTable3_1  ;; 0x40013c04
        STR      R2,[R3, #+0]
//  161       EXTI->IMR &= ~(1 << channel);
        LDR.N    R2,??DataTable3_2  ;; 0x40013c00
        LDR      R2,[R2, #+0]
        MOVS     R3,#+1
        LSLS     R3,R3,R1
        BICS     R2,R2,R3
        LDR.N    R3,??DataTable3_2  ;; 0x40013c00
        STR      R2,[R3, #+0]
//  162     }
//  163 #if STM32_EXTI_NUM_CHANNELS > 32
//  164   }
//  165   else {
//  166     /* Programming edge registers.*/
//  167     if (extp->config->channels[channel].mode & EXT_CH_MODE_RISING_EDGE)
//  168       EXTI->RTSR2 |= (1 << (32 - channel));
//  169     else
//  170       EXTI->RTSR2 &= ~(1 << (32 - channel));
//  171     if (extp->config->channels[channel].mode & EXT_CH_MODE_FALLING_EDGE)
//  172       EXTI->FTSR2 |= (1 << (32 - channel));
//  173     else
//  174       EXTI->FTSR2 &= ~(1 << (32 - channel));
//  175 
//  176     /* Programming interrupt and event registers.*/
//  177     if (extp->config->channels[channel].cb != NULL) {
//  178       EXTI->IMR2 |= (1 << (32 - channel));
//  179       EXTI->EMR2 &= ~(1 << (32 - channel));
//  180     }
//  181     else {
//  182       EXTI->EMR2 |= (1 << (32 - channel));
//  183       EXTI->IMR2 &= ~(1 << (32 - channel));
//  184     }
//  185   }
//  186 #endif
//  187 }
??ext_lld_channel_enable_6:
        POP      {R4,R5}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock3
//  188 
//  189 /**
//  190  * @brief   Disables an EXT channel.
//  191  *
//  192  * @param[in] extp      pointer to the @p EXTDriver object
//  193  * @param[in] channel   channel to be disabled
//  194  *
//  195  * @notapi
//  196  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function ext_lld_channel_disable
          CFI NoCalls
        THUMB
//  197 void ext_lld_channel_disable(EXTDriver *extp, expchannel_t channel) {
//  198 
//  199   (void)extp;
//  200 
//  201 #if STM32_EXTI_NUM_CHANNELS > 32
//  202   if (channel < 32) {
//  203 #endif
//  204     EXTI->IMR   &= ~(1 << channel);
ext_lld_channel_disable:
        LDR.N    R2,??DataTable3_2  ;; 0x40013c00
        LDR      R2,[R2, #+0]
        MOVS     R3,#+1
        LSLS     R3,R3,R1
        BICS     R2,R2,R3
        LDR.N    R3,??DataTable3_2  ;; 0x40013c00
        STR      R2,[R3, #+0]
//  205     EXTI->EMR   &= ~(1 << channel);
        LDR.N    R2,??DataTable3_1  ;; 0x40013c04
        LDR      R2,[R2, #+0]
        MOVS     R3,#+1
        LSLS     R3,R3,R1
        BICS     R2,R2,R3
        LDR.N    R3,??DataTable3_1  ;; 0x40013c04
        STR      R2,[R3, #+0]
//  206     EXTI->RTSR  &= ~(1 << channel);
        LDR.N    R2,??DataTable3_5  ;; 0x40013c08
        LDR      R2,[R2, #+0]
        MOVS     R3,#+1
        LSLS     R3,R3,R1
        BICS     R2,R2,R3
        LDR.N    R3,??DataTable3_5  ;; 0x40013c08
        STR      R2,[R3, #+0]
//  207     EXTI->FTSR  &= ~(1 << channel);
        LDR.N    R2,??DataTable3_6  ;; 0x40013c0c
        LDR      R2,[R2, #+0]
        MOVS     R3,#+1
        LSLS     R3,R3,R1
        BICS     R2,R2,R3
        LDR.N    R3,??DataTable3_6  ;; 0x40013c0c
        STR      R2,[R3, #+0]
//  208     EXTI->PR     =  (1 << channel);
        MOVS     R2,#+1
        LSLS     R2,R2,R1
        LDR.N    R3,??DataTable3_3  ;; 0x40013c14
        STR      R2,[R3, #+0]
//  209 #if STM32_EXTI_NUM_CHANNELS > 32
//  210   }
//  211   else {
//  212     EXTI->IMR2  &= ~(1 << (32 - channel));
//  213     EXTI->EMR2  &= ~(1 << (32 - channel));
//  214     EXTI->RTSR2 &= ~(1 << (32 - channel));
//  215     EXTI->FTSR2 &= ~(1 << (32 - channel));
//  216     EXTI->PR2    =  (1 << (32 - channel));
//  217   }
//  218 #endif
//  219 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3:
        DC32     EXTD1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_1:
        DC32     0x40013c04

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_2:
        DC32     0x40013c00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_3:
        DC32     0x40013c14

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_4:
        DC32     0x40013808

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_5:
        DC32     0x40013c08

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_6:
        DC32     0x40013c0c

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
//  220 
//  221 #endif /* HAL_USE_EXT */
//  222 
//  223 /** @} */
// 
//   8 bytes in section .bss
// 408 bytes in section .text
// 
// 408 bytes of CODE memory
//   8 bytes of DATA memory
//
//Errors: none
//Warnings: none
