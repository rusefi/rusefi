///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:49 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32F4xx\ext_lld_isr.c                    /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32F4xx\ext_lld_isr.c -lCN               /
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
//                       ext_lld_isr.s                                        /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME ext_lld_isr

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN EXTD1
        EXTERN _port_irq_epilogue
        EXTERN dbg_check_enter_isr
        EXTERN dbg_check_leave_isr
        EXTERN nvicDisableVector
        EXTERN nvicEnableVector

        PUBLIC TAMPER_STAMP_IRQHandler
        PUBLIC Vector138
        PUBLIC Vector170
        PUBLIC Vector44
        PUBLIC Vector4C
        PUBLIC Vector58
        PUBLIC Vector5C
        PUBLIC Vector60
        PUBLIC Vector64
        PUBLIC Vector68
        PUBLIC Vector9C
        PUBLIC VectorE0
        PUBLIC VectorE4
        PUBLIC VectorE8
        PUBLIC ext_lld_exti_irq_disable
        PUBLIC ext_lld_exti_irq_enable
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\platforms\STM32F4xx\ext_lld_isr.c
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
//   18  * @file    STM32F4xx/ext_lld_isr.c
//   19  * @brief   STM32F4xx/STM32F2xx EXT subsystem low level driver ISR code.
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
//   40 /*===========================================================================*/
//   41 /* Driver local variables.                                                   */
//   42 /*===========================================================================*/
//   43 
//   44 /*===========================================================================*/
//   45 /* Driver local functions.                                                   */
//   46 /*===========================================================================*/
//   47 
//   48 /*===========================================================================*/
//   49 /* Driver interrupt handlers.                                                */
//   50 /*===========================================================================*/
//   51 
//   52 /**
//   53  * @brief   EXTI[0] interrupt handler.
//   54  *
//   55  * @isr
//   56  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function Vector58
        THUMB
//   57 CH_IRQ_HANDLER(EXTI0_IRQHandler) {
Vector58:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   58 
//   59   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//   60 
//   61   EXTI->PR = (1 << 0);
        LDR.N    R0,??DataTable13  ;; 0x40013c14
        MOVS     R1,#+1
        STR      R1,[R0, #+0]
//   62   EXTD1.config->channels[0].cb(&EXTD1, 0);
        MOVS     R1,#+0
        LDR.N    R0,??DataTable13_1
        LDR.N    R2,??DataTable13_1
        LDR      R2,[R2, #+4]
        LDR      R2,[R2, #+4]
          CFI FunCall
        BLX      R2
//   63 
//   64   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//   65 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0
//   66 
//   67 /**
//   68  * @brief   EXTI[1] interrupt handler.
//   69  *
//   70  * @isr
//   71  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function Vector5C
        THUMB
//   72 CH_IRQ_HANDLER(EXTI1_IRQHandler) {
Vector5C:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   73 
//   74   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//   75 
//   76   EXTI->PR = (1 << 1);
        LDR.N    R0,??DataTable13  ;; 0x40013c14
        MOVS     R1,#+2
        STR      R1,[R0, #+0]
//   77   EXTD1.config->channels[1].cb(&EXTD1, 1);
        MOVS     R1,#+1
        LDR.N    R0,??DataTable13_1
        LDR.N    R2,??DataTable13_1
        LDR      R2,[R2, #+4]
        LDR      R2,[R2, #+12]
          CFI FunCall
        BLX      R2
//   78 
//   79   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//   80 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock1
//   81 
//   82 /**
//   83  * @brief   EXTI[2] interrupt handler.
//   84  *
//   85  * @isr
//   86  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function Vector60
        THUMB
//   87 CH_IRQ_HANDLER(EXTI2_IRQHandler) {
Vector60:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   88 
//   89   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//   90 
//   91   EXTI->PR = (1 << 2);
        LDR.N    R0,??DataTable13  ;; 0x40013c14
        MOVS     R1,#+4
        STR      R1,[R0, #+0]
//   92   EXTD1.config->channels[2].cb(&EXTD1, 2);
        MOVS     R1,#+2
        LDR.N    R0,??DataTable13_1
        LDR.N    R2,??DataTable13_1
        LDR      R2,[R2, #+4]
        LDR      R2,[R2, #+20]
          CFI FunCall
        BLX      R2
//   93 
//   94   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//   95 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock2
//   96 
//   97 /**
//   98  * @brief   EXTI[3] interrupt handler.
//   99  *
//  100  * @isr
//  101  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function Vector64
        THUMB
//  102 CH_IRQ_HANDLER(EXTI3_IRQHandler) {
Vector64:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  103 
//  104   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  105 
//  106   EXTI->PR = (1 << 3);
        LDR.N    R0,??DataTable13  ;; 0x40013c14
        MOVS     R1,#+8
        STR      R1,[R0, #+0]
//  107   EXTD1.config->channels[3].cb(&EXTD1, 3);
        MOVS     R1,#+3
        LDR.N    R0,??DataTable13_1
        LDR.N    R2,??DataTable13_1
        LDR      R2,[R2, #+4]
        LDR      R2,[R2, #+28]
          CFI FunCall
        BLX      R2
//  108 
//  109   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  110 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock3
//  111 
//  112 /**
//  113  * @brief   EXTI[4] interrupt handler.
//  114  *
//  115  * @isr
//  116  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function Vector68
        THUMB
//  117 CH_IRQ_HANDLER(EXTI4_IRQHandler) {
Vector68:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  118 
//  119   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  120 
//  121   EXTI->PR = (1 << 4);
        LDR.N    R0,??DataTable13  ;; 0x40013c14
        MOVS     R1,#+16
        STR      R1,[R0, #+0]
//  122   EXTD1.config->channels[4].cb(&EXTD1, 4);
        MOVS     R1,#+4
        LDR.N    R0,??DataTable13_1
        LDR.N    R2,??DataTable13_1
        LDR      R2,[R2, #+4]
        LDR      R2,[R2, #+36]
          CFI FunCall
        BLX      R2
//  123 
//  124   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  125 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock4
//  126 
//  127 /**
//  128  * @brief   EXTI[5]...EXTI[9] interrupt handler.
//  129  *
//  130  * @isr
//  131  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function Vector9C
        THUMB
//  132 CH_IRQ_HANDLER(EXTI9_5_IRQHandler) {
Vector9C:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  133   uint32_t pr;
//  134 
//  135   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  136 
//  137   pr = EXTI->PR & ((1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9));
        LDR.N    R0,??DataTable13  ;; 0x40013c14
        LDR      R0,[R0, #+0]
        ANDS     R0,R0,#0x3E0
        MOVS     R4,R0
//  138   EXTI->PR = pr;
        LDR.N    R0,??DataTable13  ;; 0x40013c14
        STR      R4,[R0, #+0]
//  139   if (pr & (1 << 5))
        LSLS     R0,R4,#+26
        BPL.N    ??Vector9C_0
//  140     EXTD1.config->channels[5].cb(&EXTD1, 5);
        MOVS     R1,#+5
        LDR.N    R0,??DataTable13_1
        LDR.N    R2,??DataTable13_1
        LDR      R2,[R2, #+4]
        LDR      R2,[R2, #+44]
          CFI FunCall
        BLX      R2
//  141   if (pr & (1 << 6))
??Vector9C_0:
        LSLS     R0,R4,#+25
        BPL.N    ??Vector9C_1
//  142     EXTD1.config->channels[6].cb(&EXTD1, 6);
        MOVS     R1,#+6
        LDR.N    R0,??DataTable13_1
        LDR.N    R2,??DataTable13_1
        LDR      R2,[R2, #+4]
        LDR      R2,[R2, #+52]
          CFI FunCall
        BLX      R2
//  143   if (pr & (1 << 7))
??Vector9C_1:
        LSLS     R0,R4,#+24
        BPL.N    ??Vector9C_2
//  144     EXTD1.config->channels[7].cb(&EXTD1, 7);
        MOVS     R1,#+7
        LDR.N    R0,??DataTable13_1
        LDR.N    R2,??DataTable13_1
        LDR      R2,[R2, #+4]
        LDR      R2,[R2, #+60]
          CFI FunCall
        BLX      R2
//  145   if (pr & (1 << 8))
??Vector9C_2:
        LSLS     R0,R4,#+23
        BPL.N    ??Vector9C_3
//  146     EXTD1.config->channels[8].cb(&EXTD1, 8);
        MOVS     R1,#+8
        LDR.N    R0,??DataTable13_1
        LDR.N    R2,??DataTable13_1
        LDR      R2,[R2, #+4]
        LDR      R2,[R2, #+68]
          CFI FunCall
        BLX      R2
//  147   if (pr & (1 << 9))
??Vector9C_3:
        LSLS     R0,R4,#+22
        BPL.N    ??Vector9C_4
//  148     EXTD1.config->channels[9].cb(&EXTD1, 9);
        MOVS     R1,#+9
        LDR.N    R0,??DataTable13_1
        LDR.N    R2,??DataTable13_1
        LDR      R2,[R2, #+4]
        LDR      R2,[R2, #+76]
          CFI FunCall
        BLX      R2
//  149 
//  150   CH_IRQ_EPILOGUE();
??Vector9C_4:
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  151 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock5
//  152 
//  153 /**
//  154  * @brief   EXTI[10]...EXTI[15] interrupt handler.
//  155  *
//  156  * @isr
//  157  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function VectorE0
        THUMB
//  158 CH_IRQ_HANDLER(EXTI15_10_IRQHandler) {
VectorE0:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  159   uint32_t pr;
//  160 
//  161   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  162 
//  163   pr = EXTI->PR & ((1 << 10) | (1 << 11) | (1 << 12) | (1 << 13) | (1 << 14) |
//  164                    (1 << 15));
        LDR.N    R0,??DataTable13  ;; 0x40013c14
        LDR      R0,[R0, #+0]
        ANDS     R0,R0,#0xFC00
        MOVS     R4,R0
//  165   EXTI->PR = pr;
        LDR.N    R0,??DataTable13  ;; 0x40013c14
        STR      R4,[R0, #+0]
//  166   if (pr & (1 << 10))
        LSLS     R0,R4,#+21
        BPL.N    ??VectorE0_0
//  167     EXTD1.config->channels[10].cb(&EXTD1, 10);
        MOVS     R1,#+10
        LDR.N    R0,??DataTable13_1
        LDR.N    R2,??DataTable13_1
        LDR      R2,[R2, #+4]
        LDR      R2,[R2, #+84]
          CFI FunCall
        BLX      R2
//  168   if (pr & (1 << 11))
??VectorE0_0:
        LSLS     R0,R4,#+20
        BPL.N    ??VectorE0_1
//  169     EXTD1.config->channels[11].cb(&EXTD1, 11);
        MOVS     R1,#+11
        LDR.N    R0,??DataTable13_1
        LDR.N    R2,??DataTable13_1
        LDR      R2,[R2, #+4]
        LDR      R2,[R2, #+92]
          CFI FunCall
        BLX      R2
//  170   if (pr & (1 << 12))
??VectorE0_1:
        LSLS     R0,R4,#+19
        BPL.N    ??VectorE0_2
//  171     EXTD1.config->channels[12].cb(&EXTD1, 12);
        MOVS     R1,#+12
        LDR.N    R0,??DataTable13_1
        LDR.N    R2,??DataTable13_1
        LDR      R2,[R2, #+4]
        LDR      R2,[R2, #+100]
          CFI FunCall
        BLX      R2
//  172   if (pr & (1 << 13))
??VectorE0_2:
        LSLS     R0,R4,#+18
        BPL.N    ??VectorE0_3
//  173     EXTD1.config->channels[13].cb(&EXTD1, 13);
        MOVS     R1,#+13
        LDR.N    R0,??DataTable13_1
        LDR.N    R2,??DataTable13_1
        LDR      R2,[R2, #+4]
        LDR      R2,[R2, #+108]
          CFI FunCall
        BLX      R2
//  174   if (pr & (1 << 14))
??VectorE0_3:
        LSLS     R0,R4,#+17
        BPL.N    ??VectorE0_4
//  175     EXTD1.config->channels[14].cb(&EXTD1, 14);
        MOVS     R1,#+14
        LDR.N    R0,??DataTable13_1
        LDR.N    R2,??DataTable13_1
        LDR      R2,[R2, #+4]
        LDR      R2,[R2, #+116]
          CFI FunCall
        BLX      R2
//  176   if (pr & (1 << 15))
??VectorE0_4:
        LSLS     R0,R4,#+16
        BPL.N    ??VectorE0_5
//  177     EXTD1.config->channels[15].cb(&EXTD1, 15);
        MOVS     R1,#+15
        LDR.N    R0,??DataTable13_1
        LDR.N    R2,??DataTable13_1
        LDR      R2,[R2, #+4]
        LDR      R2,[R2, #+124]
          CFI FunCall
        BLX      R2
//  178 
//  179   CH_IRQ_EPILOGUE();
??VectorE0_5:
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  180 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock6
//  181 
//  182 /**
//  183  * @brief   EXTI[16] interrupt handler (PVD).
//  184  *
//  185  * @isr
//  186  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function Vector44
        THUMB
//  187 CH_IRQ_HANDLER(PVD_IRQHandler) {
Vector44:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  188 
//  189   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  190 
//  191   EXTI->PR = (1 << 16);
        LDR.N    R0,??DataTable13  ;; 0x40013c14
        MOVS     R1,#+65536
        STR      R1,[R0, #+0]
//  192   EXTD1.config->channels[16].cb(&EXTD1, 16);
        MOVS     R1,#+16
        LDR.N    R0,??DataTable13_1
        LDR.N    R2,??DataTable13_1
        LDR      R2,[R2, #+4]
        LDR      R2,[R2, #+132]
          CFI FunCall
        BLX      R2
//  193 
//  194   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  195 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock7
//  196 
//  197 /**
//  198  * @brief   EXTI[17] interrupt handler (RTC).
//  199  *
//  200  * @isr
//  201  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function VectorE4
        THUMB
//  202 CH_IRQ_HANDLER(RTC_Alarm_IRQHandler) {
VectorE4:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  203 
//  204   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  205 
//  206   EXTI->PR = (1 << 17);
        LDR.N    R0,??DataTable13  ;; 0x40013c14
        MOVS     R1,#+131072
        STR      R1,[R0, #+0]
//  207   EXTD1.config->channels[17].cb(&EXTD1, 17);
        MOVS     R1,#+17
        LDR.N    R0,??DataTable13_1
        LDR.N    R2,??DataTable13_1
        LDR      R2,[R2, #+4]
        LDR      R2,[R2, #+140]
          CFI FunCall
        BLX      R2
//  208 
//  209   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  210 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock8
//  211 
//  212 /**
//  213  * @brief   EXTI[18] interrupt handler (OTG_FS_WKUP).
//  214  *
//  215  * @isr
//  216  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function VectorE8
        THUMB
//  217 CH_IRQ_HANDLER(OTG_FS_WKUP_IRQHandler) {
VectorE8:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  218 
//  219   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  220 
//  221   EXTI->PR = (1 << 18);
        LDR.N    R0,??DataTable13  ;; 0x40013c14
        MOVS     R1,#+262144
        STR      R1,[R0, #+0]
//  222   EXTD1.config->channels[18].cb(&EXTD1, 18);
        MOVS     R1,#+18
        LDR.N    R0,??DataTable13_1
        LDR.N    R2,??DataTable13_1
        LDR      R2,[R2, #+4]
        LDR      R2,[R2, #+148]
          CFI FunCall
        BLX      R2
//  223 
//  224   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  225 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock9
//  226 
//  227 #if !defined(STM32F401xx)
//  228 /**
//  229  * @brief   EXTI[19] interrupt handler (ETH_WKUP).
//  230  *
//  231  * @isr
//  232  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function Vector138
        THUMB
//  233 CH_IRQ_HANDLER(ETH_WKUP_IRQHandler) {
Vector138:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  234 
//  235   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  236 
//  237   EXTI->PR = (1 << 19);
        LDR.N    R0,??DataTable13  ;; 0x40013c14
        MOVS     R1,#+524288
        STR      R1,[R0, #+0]
//  238   EXTD1.config->channels[19].cb(&EXTD1, 19);
        MOVS     R1,#+19
        LDR.N    R0,??DataTable13_1
        LDR.N    R2,??DataTable13_1
        LDR      R2,[R2, #+4]
        LDR      R2,[R2, #+156]
          CFI FunCall
        BLX      R2
//  239 
//  240   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  241 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock10
//  242 
//  243 /**
//  244  * @brief   EXTI[20] interrupt handler (OTG_HS_WKUP).
//  245  *
//  246  * @isr
//  247  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function Vector170
        THUMB
//  248 CH_IRQ_HANDLER(OTG_HS_WKUP_IRQHandler) {
Vector170:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  249 
//  250   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  251 
//  252   EXTI->PR = (1 << 20);
        LDR.N    R0,??DataTable13  ;; 0x40013c14
        MOVS     R1,#+1048576
        STR      R1,[R0, #+0]
//  253   EXTD1.config->channels[20].cb(&EXTD1, 20);
        MOVS     R1,#+20
        LDR.N    R0,??DataTable13_1
        LDR.N    R2,??DataTable13_1
        LDR      R2,[R2, #+4]
        LDR      R2,[R2, #+164]
          CFI FunCall
        BLX      R2
//  254 
//  255   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  256 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock11
//  257 
//  258 /**
//  259  * @brief   EXTI[21] interrupt handler (TAMPER_STAMP).
//  260  *
//  261  * @isr
//  262  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function TAMPER_STAMP_IRQHandler
        THUMB
//  263 CH_IRQ_HANDLER(TAMPER_STAMP_IRQHandler) {
TAMPER_STAMP_IRQHandler:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  264 
//  265   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  266 
//  267   EXTI->PR = (1 << 21);
        LDR.N    R0,??DataTable13  ;; 0x40013c14
        MOVS     R1,#+2097152
        STR      R1,[R0, #+0]
//  268   EXTD1.config->channels[21].cb(&EXTD1, 21);
        MOVS     R1,#+21
        LDR.N    R0,??DataTable13_1
        LDR.N    R2,??DataTable13_1
        LDR      R2,[R2, #+4]
        LDR      R2,[R2, #+172]
          CFI FunCall
        BLX      R2
//  269 
//  270   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  271 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock12
//  272 #endif /* !defined(STM32F401xx) */
//  273 
//  274 /**
//  275  * @brief   EXTI[22] interrupt handler (RTC_WKUP).
//  276  *
//  277  * @isr
//  278  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function Vector4C
        THUMB
//  279 CH_IRQ_HANDLER(RTC_WKUP_IRQHandler) {
Vector4C:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  280 
//  281   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  282 
//  283   EXTI->PR = (1 << 22);
        LDR.N    R0,??DataTable13  ;; 0x40013c14
        MOVS     R1,#+4194304
        STR      R1,[R0, #+0]
//  284   EXTD1.config->channels[22].cb(&EXTD1, 22);
        MOVS     R1,#+22
        LDR.N    R0,??DataTable13_1
        LDR.N    R2,??DataTable13_1
        LDR      R2,[R2, #+4]
        LDR      R2,[R2, #+180]
          CFI FunCall
        BLX      R2
//  285 
//  286   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  287 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock13

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13:
        DC32     0x40013c14

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_1:
        DC32     EXTD1
//  288 
//  289 /*===========================================================================*/
//  290 /* Driver exported functions.                                                */
//  291 /*===========================================================================*/
//  292 
//  293 /**
//  294  * @brief   Enables EXTI IRQ sources.
//  295  *
//  296  * @notapi
//  297  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function ext_lld_exti_irq_enable
        THUMB
//  298 void ext_lld_exti_irq_enable(void) {
ext_lld_exti_irq_enable:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  299 
//  300   nvicEnableVector(EXTI0_IRQn,
//  301                    CORTEX_PRIORITY_MASK(STM32_EXT_EXTI0_IRQ_PRIORITY));
        MOVS     R1,#+96
        MOVS     R0,#+6
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  302   nvicEnableVector(EXTI1_IRQn,
//  303                    CORTEX_PRIORITY_MASK(STM32_EXT_EXTI1_IRQ_PRIORITY));
        MOVS     R1,#+96
        MOVS     R0,#+7
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  304   nvicEnableVector(EXTI2_IRQn,
//  305                    CORTEX_PRIORITY_MASK(STM32_EXT_EXTI2_IRQ_PRIORITY));
        MOVS     R1,#+96
        MOVS     R0,#+8
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  306   nvicEnableVector(EXTI3_IRQn,
//  307                    CORTEX_PRIORITY_MASK(STM32_EXT_EXTI3_IRQ_PRIORITY));
        MOVS     R1,#+96
        MOVS     R0,#+9
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  308   nvicEnableVector(EXTI4_IRQn,
//  309                    CORTEX_PRIORITY_MASK(STM32_EXT_EXTI4_IRQ_PRIORITY));
        MOVS     R1,#+96
        MOVS     R0,#+10
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  310   nvicEnableVector(EXTI9_5_IRQn,
//  311                    CORTEX_PRIORITY_MASK(STM32_EXT_EXTI5_9_IRQ_PRIORITY));
        MOVS     R1,#+96
        MOVS     R0,#+23
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  312   nvicEnableVector(EXTI15_10_IRQn,
//  313                    CORTEX_PRIORITY_MASK(STM32_EXT_EXTI10_15_IRQ_PRIORITY));
        MOVS     R1,#+96
        MOVS     R0,#+40
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  314   nvicEnableVector(PVD_IRQn,
//  315                    CORTEX_PRIORITY_MASK(STM32_EXT_EXTI16_IRQ_PRIORITY));
        MOVS     R1,#+96
        MOVS     R0,#+1
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  316   nvicEnableVector(RTC_Alarm_IRQn,
//  317                    CORTEX_PRIORITY_MASK(STM32_EXT_EXTI17_IRQ_PRIORITY));
        MOVS     R1,#+240
        MOVS     R0,#+41
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  318   nvicEnableVector(OTG_FS_WKUP_IRQn,
//  319                    CORTEX_PRIORITY_MASK(STM32_EXT_EXTI18_IRQ_PRIORITY));
        MOVS     R1,#+96
        MOVS     R0,#+42
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  320 #if !defined(STM32F401xx)
//  321   nvicEnableVector(ETH_WKUP_IRQn,
//  322                    CORTEX_PRIORITY_MASK(STM32_EXT_EXTI19_IRQ_PRIORITY));
        MOVS     R1,#+96
        MOVS     R0,#+62
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  323   nvicEnableVector(OTG_HS_WKUP_IRQn,
//  324                    CORTEX_PRIORITY_MASK(STM32_EXT_EXTI20_IRQ_PRIORITY));
        MOVS     R1,#+96
        MOVS     R0,#+76
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  325   nvicEnableVector(TAMP_STAMP_IRQn,
//  326                    CORTEX_PRIORITY_MASK(STM32_EXT_EXTI21_IRQ_PRIORITY));
        MOVS     R1,#+240
        MOVS     R0,#+2
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  327 #endif /* !defined(STM32F401xx) */
//  328   nvicEnableVector(RTC_WKUP_IRQn,
//  329                    CORTEX_PRIORITY_MASK(STM32_EXT_EXTI22_IRQ_PRIORITY));
        MOVS     R1,#+240
        MOVS     R0,#+3
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  330 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock14
//  331 
//  332 /**
//  333  * @brief   Disables EXTI IRQ sources.
//  334  *
//  335  * @notapi
//  336  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function ext_lld_exti_irq_disable
        THUMB
//  337 void ext_lld_exti_irq_disable(void) {
ext_lld_exti_irq_disable:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  338 
//  339   nvicDisableVector(EXTI0_IRQn);
        MOVS     R0,#+6
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  340   nvicDisableVector(EXTI1_IRQn);
        MOVS     R0,#+7
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  341   nvicDisableVector(EXTI2_IRQn);
        MOVS     R0,#+8
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  342   nvicDisableVector(EXTI3_IRQn);
        MOVS     R0,#+9
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  343   nvicDisableVector(EXTI4_IRQn);
        MOVS     R0,#+10
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  344   nvicDisableVector(EXTI9_5_IRQn);
        MOVS     R0,#+23
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  345   nvicDisableVector(EXTI15_10_IRQn);
        MOVS     R0,#+40
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  346   nvicDisableVector(PVD_IRQn);
        MOVS     R0,#+1
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  347   nvicDisableVector(RTC_Alarm_IRQn);
        MOVS     R0,#+41
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  348   nvicDisableVector(OTG_FS_WKUP_IRQn);
        MOVS     R0,#+42
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  349 #if !defined(STM32F401xx)
//  350   nvicDisableVector(ETH_WKUP_IRQn);
        MOVS     R0,#+62
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  351   nvicDisableVector(OTG_HS_WKUP_IRQn);
        MOVS     R0,#+76
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  352   nvicDisableVector(TAMP_STAMP_IRQn);
        MOVS     R0,#+2
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  353 #endif /* !defined(STM32F401xx) */
//  354   nvicDisableVector(RTC_WKUP_IRQn);
        MOVS     R0,#+3
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  355 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock15

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
//  356 
//  357 #endif /* HAL_USE_EXT */
//  358 
//  359 /** @} */
// 
// 884 bytes in section .text
// 
// 884 bytes of CODE memory
//
//Errors: none
//Warnings: none
