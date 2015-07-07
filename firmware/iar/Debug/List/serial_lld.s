///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:44 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32\USARTv1\serial_lld.c                 /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32\USARTv1\serial_lld.c -lCN            /
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
//                       serial_lld.s                                         /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME serial_lld

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _port_irq_epilogue
        EXTERN chEvtBroadcastFlagsI
        EXTERN chOQGetI
        EXTERN dbg_check_enter_isr
        EXTERN dbg_check_leave_isr
        EXTERN dbg_check_lock_from_isr
        EXTERN dbg_check_unlock_from_isr
        EXTERN nvicDisableVector
        EXTERN nvicEnableVector
        EXTERN sdIncomingDataI
        EXTERN sdObjectInit

        PUBLIC SD1
        PUBLIC SD2
        PUBLIC SD3
        PUBLIC VectorD4
        PUBLIC VectorD8
        PUBLIC VectorDC
        PUBLIC sd_lld_init
        PUBLIC sd_lld_start
        PUBLIC sd_lld_stop
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\platforms\STM32\USARTv1\serial_lld.c
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
//   18  * @file    STM32/USARTv1/serial_lld.c
//   19  * @brief   STM32 low level serial driver code.
//   20  *
//   21  * @addtogroup SERIAL
//   22  * @{
//   23  */
//   24 
//   25 #include "ch.h"
//   26 #include "hal.h"
//   27 
//   28 #if HAL_USE_SERIAL || defined(__DOXYGEN__)
//   29 
//   30 /*===========================================================================*/
//   31 /* Driver local definitions.                                                 */
//   32 /*===========================================================================*/
//   33 
//   34 /*===========================================================================*/
//   35 /* Driver exported variables.                                                */
//   36 /*===========================================================================*/
//   37 
//   38 /** @brief USART1 serial driver identifier.*/
//   39 #if STM32_SERIAL_USE_USART1 || defined(__DOXYGEN__)

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   40 SerialDriver SD1;
SD1:
        DS8 120
//   41 #endif
//   42 
//   43 /** @brief USART2 serial driver identifier.*/
//   44 #if STM32_SERIAL_USE_USART2 || defined(__DOXYGEN__)

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   45 SerialDriver SD2;
SD2:
        DS8 120
//   46 #endif
//   47 
//   48 /** @brief USART3 serial driver identifier.*/
//   49 #if STM32_SERIAL_USE_USART3 || defined(__DOXYGEN__)

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   50 SerialDriver SD3;
SD3:
        DS8 120
//   51 #endif
//   52 
//   53 /** @brief UART4 serial driver identifier.*/
//   54 #if STM32_SERIAL_USE_UART4 || defined(__DOXYGEN__)
//   55 SerialDriver SD4;
//   56 #endif
//   57 
//   58 /** @brief UART5 serial driver identifier.*/
//   59 #if STM32_SERIAL_USE_UART5 || defined(__DOXYGEN__)
//   60 SerialDriver SD5;
//   61 #endif
//   62 
//   63 /** @brief USART6 serial driver identifier.*/
//   64 #if STM32_SERIAL_USE_USART6 || defined(__DOXYGEN__)
//   65 SerialDriver SD6;
//   66 #endif
//   67 
//   68 /*===========================================================================*/
//   69 /* Driver local variables and types.                                         */
//   70 /*===========================================================================*/
//   71 
//   72 /** @brief Driver default configuration.*/

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//   73 static const SerialConfig default_config =
default_config:
        DATA
        DC32 38400
        DC16 0, 16384, 0
        DC8 0, 0
//   74 {
//   75   SERIAL_DEFAULT_BITRATE,
//   76   0,
//   77   USART_CR2_STOP1_BITS | USART_CR2_LINEN,
//   78   0
//   79 };
//   80 
//   81 /*===========================================================================*/
//   82 /* Driver local functions.                                                   */
//   83 /*===========================================================================*/
//   84 
//   85 /**
//   86  * @brief   USART initialization.
//   87  * @details This function must be invoked with interrupts disabled.
//   88  *
//   89  * @param[in] sdp       pointer to a @p SerialDriver object
//   90  * @param[in] config    the architecture-dependent serial driver configuration
//   91  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function usart_init
          CFI NoCalls
        THUMB
//   92 static void usart_init(SerialDriver *sdp, const SerialConfig *config) {
usart_init:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
//   93   USART_TypeDef *u = sdp->usart;
        LDR      R2,[R0, #+116]
//   94 
//   95   /* Baud rate setting.*/
//   96 #if STM32_HAS_USART6
//   97   if ((sdp->usart == USART1) || (sdp->usart == USART6))
        LDR      R3,[R0, #+116]
        LDR.N    R4,??DataTable9  ;; 0x40011000
        CMP      R3,R4
        BEQ.N    ??usart_init_0
        LDR      R3,[R0, #+116]
        LDR.N    R4,??DataTable9_1  ;; 0x40011400
        CMP      R3,R4
        BNE.N    ??usart_init_1
//   98 #else
//   99   if (sdp->usart == USART1)
//  100 #endif
//  101     u->BRR = STM32_PCLK2 / config->speed;
??usart_init_0:
        LDR.N    R3,??DataTable9_2  ;; 0x501bd00
        LDR      R4,[R1, #+0]
        UDIV     R3,R3,R4
        STRH     R3,[R2, #+8]
        B.N      ??usart_init_2
//  102   else
//  103     u->BRR = STM32_PCLK1 / config->speed;
??usart_init_1:
        LDR.N    R3,??DataTable9_3  ;; 0x280de80
        LDR      R4,[R1, #+0]
        UDIV     R3,R3,R4
        STRH     R3,[R2, #+8]
//  104 
//  105   /* Note that some bits are enforced.*/
//  106   u->CR2 = config->cr2 | USART_CR2_LBDIE;
??usart_init_2:
        LDRH     R3,[R1, #+6]
        ORRS     R3,R3,#0x40
        STRH     R3,[R2, #+16]
//  107   u->CR3 = config->cr3 | USART_CR3_EIE;
        LDRH     R3,[R1, #+8]
        ORRS     R3,R3,#0x1
        STRH     R3,[R2, #+20]
//  108   u->CR1 = config->cr1 | USART_CR1_UE | USART_CR1_PEIE |
//  109                          USART_CR1_RXNEIE | USART_CR1_TE |
//  110                          USART_CR1_RE;
        LDRH     R3,[R1, #+4]
        ORR      R3,R3,#0x2100
        ORRS     R3,R3,#0x2C
        STRH     R3,[R2, #+12]
//  111   u->SR = 0;
        MOVS     R3,#+0
        STRH     R3,[R2, #+0]
//  112   (void)u->SR;  /* SR reset step 1.*/
        LDRH     R3,[R2, #+0]
//  113   (void)u->DR;  /* SR reset step 2.*/
        LDRH     R4,[R2, #+4]
//  114 }
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//  115 
//  116 /**
//  117  * @brief   USART de-initialization.
//  118  * @details This function must be invoked with interrupts disabled.
//  119  *
//  120  * @param[in] u         pointer to an USART I/O block
//  121  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function usart_deinit
          CFI NoCalls
        THUMB
//  122 static void usart_deinit(USART_TypeDef *u) {
//  123 
//  124   u->CR1 = 0;
usart_deinit:
        MOVS     R1,#+0
        STRH     R1,[R0, #+12]
//  125   u->CR2 = 0;
        MOVS     R1,#+0
        STRH     R1,[R0, #+16]
//  126   u->CR3 = 0;
        MOVS     R1,#+0
        STRH     R1,[R0, #+20]
//  127 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//  128 
//  129 /**
//  130  * @brief   Error handling routine.
//  131  *
//  132  * @param[in] sdp       pointer to a @p SerialDriver object
//  133  * @param[in] sr        USART SR register value
//  134  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function set_error
        THUMB
//  135 static void set_error(SerialDriver *sdp, uint16_t sr) {
set_error:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  136   flagsmask_t sts = 0;
        MOVS     R6,#+0
//  137 
//  138   if (sr & USART_SR_ORE)
        LSLS     R0,R5,#+28
        BPL.N    ??set_error_0
//  139     sts |= SD_OVERRUN_ERROR;
        ORRS     R6,R6,#0x80
//  140   if (sr & USART_SR_PE)
??set_error_0:
        LSLS     R0,R5,#+31
        BPL.N    ??set_error_1
//  141     sts |= SD_PARITY_ERROR;
        ORRS     R6,R6,#0x20
//  142   if (sr & USART_SR_FE)
??set_error_1:
        LSLS     R0,R5,#+30
        BPL.N    ??set_error_2
//  143     sts |= SD_FRAMING_ERROR;
        ORRS     R6,R6,#0x40
//  144   if (sr & USART_SR_NE)
??set_error_2:
        LSLS     R0,R5,#+29
        BPL.N    ??set_error_3
//  145     sts |= SD_NOISE_ERROR;
        ORRS     R6,R6,#0x100
//  146   chnAddFlagsI(sdp, sts);
??set_error_3:
        MOVS     R1,R6
        ADDS     R0,R4,#+4
          CFI FunCall chEvtBroadcastFlagsI
        BL       chEvtBroadcastFlagsI
//  147 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock2
//  148 
//  149 /**
//  150  * @brief   Common IRQ handler.
//  151  *
//  152  * @param[in] sdp       communication channel associated to the USART
//  153  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function serve_interrupt
        THUMB
//  154 static void serve_interrupt(SerialDriver *sdp) {
serve_interrupt:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
//  155   USART_TypeDef *u = sdp->usart;
        LDR      R5,[R4, #+116]
//  156   uint16_t cr1 = u->CR1;
        LDRH     R6,[R5, #+12]
//  157   uint16_t sr = u->SR;
        LDRH     R7,[R5, #+0]
//  158 
//  159   /* Special case, LIN break detection.*/
//  160   if (sr & USART_SR_LBD) {
        LSLS     R0,R7,#+23
        BPL.N    ??serve_interrupt_0
//  161     chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  162     chnAddFlagsI(sdp, SD_BREAK_DETECTED);
        MOV      R1,#+512
        ADDS     R0,R4,#+4
          CFI FunCall chEvtBroadcastFlagsI
        BL       chEvtBroadcastFlagsI
//  163     chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  164     u->SR = ~USART_SR_LBD;
        MOVW     R0,#+65279
        STRH     R0,[R5, #+0]
//  165   }
//  166 
//  167   /* Data available.*/
//  168   chSysLockFromIsr();
??serve_interrupt_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  169   while (sr & (USART_SR_RXNE | USART_SR_ORE | USART_SR_NE | USART_SR_FE |
//  170                USART_SR_PE)) {
??serve_interrupt_1:
        MOVS     R0,#+47
        TST      R7,R0
        BEQ.N    ??serve_interrupt_2
//  171     uint8_t b;
//  172 
//  173     /* Error condition detection.*/
//  174     if (sr & (USART_SR_ORE | USART_SR_NE | USART_SR_FE  | USART_SR_PE))
        MOVS     R0,#+15
        TST      R7,R0
        BEQ.N    ??serve_interrupt_3
//  175       set_error(sdp, sr);
        MOVS     R1,R7
        UXTH     R1,R1            ;; ZeroExt  R1,R1,#+16,#+16
        MOVS     R0,R4
          CFI FunCall set_error
        BL       set_error
//  176     b = u->DR;
??serve_interrupt_3:
        LDRH     R0,[R5, #+4]
        MOV      R8,R0
//  177     if (sr & USART_SR_RXNE)
        LSLS     R0,R7,#+26
        BPL.N    ??serve_interrupt_4
//  178       sdIncomingDataI(sdp, b);
        MOV      R1,R8
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall sdIncomingDataI
        BL       sdIncomingDataI
//  179     sr = u->SR;
??serve_interrupt_4:
        LDRH     R0,[R5, #+0]
        MOVS     R7,R0
        B.N      ??serve_interrupt_1
//  180   }
//  181   chSysUnlockFromIsr();
??serve_interrupt_2:
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  182 
//  183   /* Transmission buffer empty.*/
//  184   if ((cr1 & USART_CR1_TXEIE) && (sr & USART_SR_TXE)) {
        LSLS     R0,R6,#+24
        BPL.N    ??serve_interrupt_5
        LSLS     R0,R7,#+24
        BPL.N    ??serve_interrupt_5
//  185     msg_t b;
//  186     chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  187     b = chOQGetI(&sdp->oqueue);
        ADDS     R0,R4,#+48
          CFI FunCall chOQGetI
        BL       chOQGetI
        MOV      R8,R0
//  188     if (b < Q_OK) {
        CMP      R8,#+0
        BPL.N    ??serve_interrupt_6
//  189       chnAddFlagsI(sdp, CHN_OUTPUT_EMPTY);
        MOVS     R1,#+8
        ADDS     R0,R4,#+4
          CFI FunCall chEvtBroadcastFlagsI
        BL       chEvtBroadcastFlagsI
//  190       u->CR1 = (cr1 & ~USART_CR1_TXEIE) | USART_CR1_TCIE;
        MOVW     R0,#+65407
        ANDS     R0,R0,R6
        ORRS     R0,R0,#0x40
        STRH     R0,[R5, #+12]
        B.N      ??serve_interrupt_7
//  191     }
//  192     else
//  193       u->DR = b;
??serve_interrupt_6:
        STRH     R8,[R5, #+4]
//  194     chSysUnlockFromIsr();
??serve_interrupt_7:
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  195   }
//  196 
//  197   /* Physical transmission end.*/
//  198   if (sr & USART_SR_TC) {
??serve_interrupt_5:
        LSLS     R0,R7,#+25
        BPL.N    ??serve_interrupt_8
//  199     chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  200     if (chOQIsEmptyI(&sdp->oqueue))
        LDR      R0,[R4, #+68]
        LDR      R1,[R4, #+72]
        CMP      R0,R1
        BNE.N    ??serve_interrupt_9
        LDR      R0,[R4, #+56]
        CMP      R0,#+0
        BEQ.N    ??serve_interrupt_9
        MOVS     R0,#+1
        B.N      ??serve_interrupt_10
??serve_interrupt_9:
        MOVS     R0,#+0
??serve_interrupt_10:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+0
        BEQ.N    ??serve_interrupt_11
//  201       chnAddFlagsI(sdp, CHN_TRANSMISSION_END);
        MOVS     R1,#+16
        ADDS     R0,R4,#+4
          CFI FunCall chEvtBroadcastFlagsI
        BL       chEvtBroadcastFlagsI
//  202     u->CR1 = cr1 & ~USART_CR1_TCIE;
??serve_interrupt_11:
        MOVW     R0,#+65471
        ANDS     R0,R0,R6
        STRH     R0,[R5, #+12]
//  203     u->SR = ~USART_SR_TC;
        MOVW     R0,#+65471
        STRH     R0,[R5, #+0]
//  204     chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  205   }
//  206 }
??serve_interrupt_8:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock3
//  207 
//  208 #if STM32_SERIAL_USE_USART1 || defined(__DOXYGEN__)

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function notify1
          CFI NoCalls
        THUMB
//  209 static void notify1(GenericQueue *qp) {
//  210 
//  211   (void)qp;
//  212   USART1->CR1 |= USART_CR1_TXEIE;
notify1:
        LDR.N    R1,??DataTable9_4  ;; 0x4001100c
        LDRH     R1,[R1, #+0]
        ORRS     R1,R1,#0x80
        LDR.N    R2,??DataTable9_4  ;; 0x4001100c
        STRH     R1,[R2, #+0]
//  213 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//  214 #endif
//  215 
//  216 #if STM32_SERIAL_USE_USART2 || defined(__DOXYGEN__)

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function notify2
          CFI NoCalls
        THUMB
//  217 static void notify2(GenericQueue *qp) {
//  218 
//  219   (void)qp;
//  220   USART2->CR1 |= USART_CR1_TXEIE;
notify2:
        LDR.N    R1,??DataTable9_5  ;; 0x4000440c
        LDRH     R1,[R1, #+0]
        ORRS     R1,R1,#0x80
        LDR.N    R2,??DataTable9_5  ;; 0x4000440c
        STRH     R1,[R2, #+0]
//  221 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock5
//  222 #endif
//  223 
//  224 #if STM32_SERIAL_USE_USART3 || defined(__DOXYGEN__)

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function notify3
          CFI NoCalls
        THUMB
//  225 static void notify3(GenericQueue *qp) {
//  226 
//  227   (void)qp;
//  228   USART3->CR1 |= USART_CR1_TXEIE;
notify3:
        LDR.N    R1,??DataTable9_6  ;; 0x4000480c
        LDRH     R1,[R1, #+0]
        ORRS     R1,R1,#0x80
        LDR.N    R2,??DataTable9_6  ;; 0x4000480c
        STRH     R1,[R2, #+0]
//  229 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock6
//  230 #endif
//  231 
//  232 #if STM32_SERIAL_USE_UART4 || defined(__DOXYGEN__)
//  233 static void notify4(GenericQueue *qp) {
//  234 
//  235   (void)qp;
//  236   UART4->CR1 |= USART_CR1_TXEIE;
//  237 }
//  238 #endif
//  239 
//  240 #if STM32_SERIAL_USE_UART5 || defined(__DOXYGEN__)
//  241 static void notify5(GenericQueue *qp) {
//  242 
//  243   (void)qp;
//  244   UART5->CR1 |= USART_CR1_TXEIE;
//  245 }
//  246 #endif
//  247 
//  248 #if STM32_SERIAL_USE_USART6 || defined(__DOXYGEN__)
//  249 static void notify6(GenericQueue *qp) {
//  250 
//  251   (void)qp;
//  252   USART6->CR1 |= USART_CR1_TXEIE;
//  253 }
//  254 #endif
//  255 
//  256 /*===========================================================================*/
//  257 /* Driver interrupt handlers.                                                */
//  258 /*===========================================================================*/
//  259 
//  260 #if STM32_SERIAL_USE_USART1 || defined(__DOXYGEN__)
//  261 #if !defined(STM32_USART1_HANDLER)
//  262 #error "STM32_USART1_HANDLER not defined"
//  263 #endif
//  264 /**
//  265  * @brief   USART1 interrupt handler.
//  266  *
//  267  * @isr
//  268  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function VectorD4
        THUMB
//  269 CH_IRQ_HANDLER(STM32_USART1_HANDLER) {
VectorD4:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  270 
//  271   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  272 
//  273   serve_interrupt(&SD1);
        LDR.N    R0,??DataTable9_7
          CFI FunCall serve_interrupt
        BL       serve_interrupt
//  274 
//  275   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  276 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock7
//  277 #endif
//  278 
//  279 #if STM32_SERIAL_USE_USART2 || defined(__DOXYGEN__)
//  280 #if !defined(STM32_USART2_HANDLER)
//  281 #error "STM32_USART2_HANDLER not defined"
//  282 #endif
//  283 /**
//  284  * @brief   USART2 interrupt handler.
//  285  *
//  286  * @isr
//  287  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function VectorD8
        THUMB
//  288 CH_IRQ_HANDLER(STM32_USART2_HANDLER) {
VectorD8:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  289 
//  290   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  291 
//  292   serve_interrupt(&SD2);
        LDR.N    R0,??DataTable9_8
          CFI FunCall serve_interrupt
        BL       serve_interrupt
//  293 
//  294   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  295 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock8
//  296 #endif
//  297 
//  298 #if STM32_SERIAL_USE_USART3 || defined(__DOXYGEN__)
//  299 #if !defined(STM32_USART3_HANDLER)
//  300 #error "STM32_USART3_HANDLER not defined"
//  301 #endif
//  302 /**
//  303  * @brief   USART3 interrupt handler.
//  304  *
//  305  * @isr
//  306  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function VectorDC
        THUMB
//  307 CH_IRQ_HANDLER(STM32_USART3_HANDLER) {
VectorDC:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  308 
//  309   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  310 
//  311   serve_interrupt(&SD3);
        LDR.N    R0,??DataTable9_9
          CFI FunCall serve_interrupt
        BL       serve_interrupt
//  312 
//  313   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  314 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock9
//  315 #endif
//  316 
//  317 #if STM32_SERIAL_USE_UART4 || defined(__DOXYGEN__)
//  318 #if !defined(STM32_UART4_HANDLER)
//  319 #error "STM32_UART4_HANDLER not defined"
//  320 #endif
//  321 /**
//  322  * @brief   UART4 interrupt handler.
//  323  *
//  324  * @isr
//  325  */
//  326 CH_IRQ_HANDLER(STM32_UART4_HANDLER) {
//  327 
//  328   CH_IRQ_PROLOGUE();
//  329 
//  330   serve_interrupt(&SD4);
//  331 
//  332   CH_IRQ_EPILOGUE();
//  333 }
//  334 #endif
//  335 
//  336 #if STM32_SERIAL_USE_UART5 || defined(__DOXYGEN__)
//  337 #if !defined(STM32_UART5_HANDLER)
//  338 #error "STM32_UART5_HANDLER not defined"
//  339 #endif
//  340 /**
//  341  * @brief   UART5 interrupt handler.
//  342  *
//  343  * @isr
//  344  */
//  345 CH_IRQ_HANDLER(STM32_UART5_HANDLER) {
//  346 
//  347   CH_IRQ_PROLOGUE();
//  348 
//  349   serve_interrupt(&SD5);
//  350 
//  351   CH_IRQ_EPILOGUE();
//  352 }
//  353 #endif
//  354 
//  355 #if STM32_SERIAL_USE_USART6 || defined(__DOXYGEN__)
//  356 #if !defined(STM32_USART6_HANDLER)
//  357 #error "STM32_USART6_HANDLER not defined"
//  358 #endif
//  359 /**
//  360  * @brief   USART1 interrupt handler.
//  361  *
//  362  * @isr
//  363  */
//  364 CH_IRQ_HANDLER(STM32_USART6_HANDLER) {
//  365 
//  366   CH_IRQ_PROLOGUE();
//  367 
//  368   serve_interrupt(&SD6);
//  369 
//  370   CH_IRQ_EPILOGUE();
//  371 }
//  372 #endif
//  373 
//  374 /*===========================================================================*/
//  375 /* Driver exported functions.                                                */
//  376 /*===========================================================================*/
//  377 
//  378 /**
//  379  * @brief   Low level serial driver initialization.
//  380  *
//  381  * @notapi
//  382  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function sd_lld_init
        THUMB
//  383 void sd_lld_init(void) {
sd_lld_init:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  384 
//  385 #if STM32_SERIAL_USE_USART1
//  386   sdObjectInit(&SD1, NULL, notify1);
        LDR.N    R2,??DataTable9_10
        MOVS     R1,#+0
        LDR.N    R0,??DataTable9_7
          CFI FunCall sdObjectInit
        BL       sdObjectInit
//  387   SD1.usart = USART1;
        LDR.N    R0,??DataTable9_7
        LDR.N    R1,??DataTable9  ;; 0x40011000
        STR      R1,[R0, #+116]
//  388 #endif
//  389 
//  390 #if STM32_SERIAL_USE_USART2
//  391   sdObjectInit(&SD2, NULL, notify2);
        LDR.N    R2,??DataTable9_11
        MOVS     R1,#+0
        LDR.N    R0,??DataTable9_8
          CFI FunCall sdObjectInit
        BL       sdObjectInit
//  392   SD2.usart = USART2;
        LDR.N    R0,??DataTable9_8
        LDR.N    R1,??DataTable9_12  ;; 0x40004400
        STR      R1,[R0, #+116]
//  393 #endif
//  394 
//  395 #if STM32_SERIAL_USE_USART3
//  396   sdObjectInit(&SD3, NULL, notify3);
        LDR.N    R2,??DataTable9_13
        MOVS     R1,#+0
        LDR.N    R0,??DataTable9_9
          CFI FunCall sdObjectInit
        BL       sdObjectInit
//  397   SD3.usart = USART3;
        LDR.N    R0,??DataTable9_9
        LDR.N    R1,??DataTable9_14  ;; 0x40004800
        STR      R1,[R0, #+116]
//  398 #endif
//  399 
//  400 #if STM32_SERIAL_USE_UART4
//  401   sdObjectInit(&SD4, NULL, notify4);
//  402   SD4.usart = UART4;
//  403 #endif
//  404 
//  405 #if STM32_SERIAL_USE_UART5
//  406   sdObjectInit(&SD5, NULL, notify5);
//  407   SD5.usart = UART5;
//  408 #endif
//  409 
//  410 #if STM32_SERIAL_USE_USART6
//  411   sdObjectInit(&SD6, NULL, notify6);
//  412   SD6.usart = USART6;
//  413 #endif
//  414 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock10
//  415 
//  416 /**
//  417  * @brief   Low level serial driver configuration and (re)start.
//  418  *
//  419  * @param[in] sdp       pointer to a @p SerialDriver object
//  420  * @param[in] config    the architecture-dependent serial driver configuration.
//  421  *                      If this parameter is set to @p NULL then a default
//  422  *                      configuration is used.
//  423  *
//  424  * @notapi
//  425  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function sd_lld_start
        THUMB
//  426 void sd_lld_start(SerialDriver *sdp, const SerialConfig *config) {
sd_lld_start:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  427 
//  428   if (config == NULL)
        CMP      R5,#+0
        BNE.N    ??sd_lld_start_0
//  429     config = &default_config;
        LDR.N    R0,??DataTable9_15
        MOVS     R5,R0
//  430 
//  431   if (sdp->state == SD_STOP) {
??sd_lld_start_0:
        LDRB     R0,[R4, #+8]
        CMP      R0,#+1
        BNE.N    ??sd_lld_start_1
//  432 #if STM32_SERIAL_USE_USART1
//  433     if (&SD1 == sdp) {
        LDR.N    R0,??DataTable9_7
        CMP      R0,R4
        BNE.N    ??sd_lld_start_2
//  434       rccEnableUSART1(FALSE);
        LDR.N    R0,??DataTable9_16  ;; 0x40023844
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x10
        LDR.N    R1,??DataTable9_16  ;; 0x40023844
        STR      R0,[R1, #+0]
//  435       nvicEnableVector(STM32_USART1_NUMBER,
//  436                        CORTEX_PRIORITY_MASK(STM32_SERIAL_USART1_PRIORITY));
        MOVS     R1,#+192
        MOVS     R0,#+37
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  437     }
//  438 #endif
//  439 #if STM32_SERIAL_USE_USART2
//  440     if (&SD2 == sdp) {
??sd_lld_start_2:
        LDR.N    R0,??DataTable9_8
        CMP      R0,R4
        BNE.N    ??sd_lld_start_3
//  441       rccEnableUSART2(FALSE);
        LDR.N    R0,??DataTable9_17  ;; 0x40023840
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x20000
        LDR.N    R1,??DataTable9_17  ;; 0x40023840
        STR      R0,[R1, #+0]
//  442       nvicEnableVector(STM32_USART2_NUMBER,
//  443                        CORTEX_PRIORITY_MASK(STM32_SERIAL_USART2_PRIORITY));
        MOVS     R1,#+192
        MOVS     R0,#+38
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  444     }
//  445 #endif
//  446 #if STM32_SERIAL_USE_USART3
//  447     if (&SD3 == sdp) {
??sd_lld_start_3:
        LDR.N    R0,??DataTable9_9
        CMP      R0,R4
        BNE.N    ??sd_lld_start_1
//  448       rccEnableUSART3(FALSE);
        LDR.N    R0,??DataTable9_17  ;; 0x40023840
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x40000
        LDR.N    R1,??DataTable9_17  ;; 0x40023840
        STR      R0,[R1, #+0]
//  449       nvicEnableVector(STM32_USART3_NUMBER,
//  450                        CORTEX_PRIORITY_MASK(STM32_SERIAL_USART3_PRIORITY));
        MOVS     R1,#+192
        MOVS     R0,#+39
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  451     }
//  452 #endif
//  453 #if STM32_SERIAL_USE_UART4
//  454     if (&SD4 == sdp) {
//  455       rccEnableUART4(FALSE);
//  456       nvicEnableVector(STM32_UART4_NUMBER,
//  457                        CORTEX_PRIORITY_MASK(STM32_SERIAL_UART4_PRIORITY));
//  458     }
//  459 #endif
//  460 #if STM32_SERIAL_USE_UART5
//  461     if (&SD5 == sdp) {
//  462       rccEnableUART5(FALSE);
//  463       nvicEnableVector(STM32_UART5_NUMBER,
//  464                        CORTEX_PRIORITY_MASK(STM32_SERIAL_UART5_PRIORITY));
//  465     }
//  466 #endif
//  467 #if STM32_SERIAL_USE_USART6
//  468     if (&SD6 == sdp) {
//  469       rccEnableUSART6(FALSE);
//  470       nvicEnableVector(STM32_USART6_NUMBER,
//  471                        CORTEX_PRIORITY_MASK(STM32_SERIAL_USART6_PRIORITY));
//  472     }
//  473 #endif
//  474   }
//  475   usart_init(sdp, config);
??sd_lld_start_1:
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall usart_init
        BL       usart_init
//  476 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock11
//  477 
//  478 /**
//  479  * @brief   Low level serial driver stop.
//  480  * @details De-initializes the USART, stops the associated clock, resets the
//  481  *          interrupt vector.
//  482  *
//  483  * @param[in] sdp       pointer to a @p SerialDriver object
//  484  *
//  485  * @notapi
//  486  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function sd_lld_stop
        THUMB
//  487 void sd_lld_stop(SerialDriver *sdp) {
sd_lld_stop:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  488 
//  489   if (sdp->state == SD_READY) {
        LDRB     R0,[R4, #+8]
        CMP      R0,#+2
        BNE.N    ??sd_lld_stop_0
//  490     usart_deinit(sdp->usart);
        LDR      R0,[R4, #+116]
          CFI FunCall usart_deinit
        BL       usart_deinit
//  491 #if STM32_SERIAL_USE_USART1
//  492     if (&SD1 == sdp) {
        LDR.N    R0,??DataTable9_7
        CMP      R0,R4
        BNE.N    ??sd_lld_stop_1
//  493       rccDisableUSART1(FALSE);
        LDR.N    R0,??DataTable9_16  ;; 0x40023844
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x10
        LDR.N    R1,??DataTable9_16  ;; 0x40023844
        STR      R0,[R1, #+0]
//  494       nvicDisableVector(STM32_USART1_NUMBER);
        MOVS     R0,#+37
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  495       return;
        B.N      ??sd_lld_stop_2
//  496     }
//  497 #endif
//  498 #if STM32_SERIAL_USE_USART2
//  499     if (&SD2 == sdp) {
??sd_lld_stop_1:
        LDR.N    R0,??DataTable9_8
        CMP      R0,R4
        BNE.N    ??sd_lld_stop_3
//  500       rccDisableUSART2(FALSE);
        LDR.N    R0,??DataTable9_17  ;; 0x40023840
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x20000
        LDR.N    R1,??DataTable9_17  ;; 0x40023840
        STR      R0,[R1, #+0]
//  501       nvicDisableVector(STM32_USART2_NUMBER);
        MOVS     R0,#+38
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  502       return;
        B.N      ??sd_lld_stop_2
//  503     }
//  504 #endif
//  505 #if STM32_SERIAL_USE_USART3
//  506     if (&SD3 == sdp) {
??sd_lld_stop_3:
        LDR.N    R0,??DataTable9_9
        CMP      R0,R4
        BNE.N    ??sd_lld_stop_0
//  507       rccDisableUSART3(FALSE);
        LDR.N    R0,??DataTable9_17  ;; 0x40023840
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x40000
        LDR.N    R1,??DataTable9_17  ;; 0x40023840
        STR      R0,[R1, #+0]
//  508       nvicDisableVector(STM32_USART3_NUMBER);
        MOVS     R0,#+39
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  509       return;
        B.N      ??sd_lld_stop_2
//  510     }
//  511 #endif
//  512 #if STM32_SERIAL_USE_UART4
//  513     if (&SD4 == sdp) {
//  514       rccDisableUART4(FALSE);
//  515       nvicDisableVector(STM32_UART4_NUMBER);
//  516       return;
//  517     }
//  518 #endif
//  519 #if STM32_SERIAL_USE_UART5
//  520     if (&SD5 == sdp) {
//  521       rccDisableUART5(FALSE);
//  522       nvicDisableVector(STM32_UART5_NUMBER);
//  523       return;
//  524     }
//  525 #endif
//  526 #if STM32_SERIAL_USE_USART6
//  527     if (&SD6 == sdp) {
//  528       rccDisableUSART6(FALSE);
//  529       nvicDisableVector(STM32_USART6_NUMBER);
//  530       return;
//  531     }
//  532 #endif
//  533   }
//  534 }
??sd_lld_stop_0:
??sd_lld_stop_2:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock12

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9:
        DC32     0x40011000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_1:
        DC32     0x40011400

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_2:
        DC32     0x501bd00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_3:
        DC32     0x280de80

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_4:
        DC32     0x4001100c

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_5:
        DC32     0x4000440c

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_6:
        DC32     0x4000480c

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_7:
        DC32     SD1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_8:
        DC32     SD2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_9:
        DC32     SD3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_10:
        DC32     notify1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_11:
        DC32     notify2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_12:
        DC32     0x40004400

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_13:
        DC32     notify3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_14:
        DC32     0x40004800

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_15:
        DC32     default_config

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_16:
        DC32     0x40023844

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_17:
        DC32     0x40023840

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
//  535 
//  536 #endif /* HAL_USE_SERIAL */
//  537 
//  538 /** @} */
// 
// 360 bytes in section .bss
//  12 bytes in section .rodata
// 846 bytes in section .text
// 
// 846 bytes of CODE  memory
//  12 bytes of CONST memory
// 360 bytes of DATA  memory
//
//Errors: none
//Warnings: none
