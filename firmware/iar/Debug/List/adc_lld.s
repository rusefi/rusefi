///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:38 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32F4xx\adc_lld.c                        /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32F4xx\adc_lld.c -lCN                   /
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
//                       adc_lld.s                                            /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME adc_lld

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _port_irq_epilogue
        EXTERN _stm32_dma_streams
        EXTERN adcObjectInit
        EXTERN chDbgPanic
        EXTERN chSchReadyI
        EXTERN dbg_check_enter_isr
        EXTERN dbg_check_leave_isr
        EXTERN dbg_check_lock_from_isr
        EXTERN dbg_check_unlock_from_isr
        EXTERN dmaStreamAllocate
        EXTERN dmaStreamRelease
        EXTERN firmwareError
        EXTERN getRemainingStack
        EXTERN nvicEnableVector
        EXTERN rlist

        PUBLIC ADCD1
        PUBLIC ADCD2
        PUBLIC Vector88
        PUBLIC adcSTM32DisableTSVREFE
        PUBLIC adcSTM32DisableVBATE
        PUBLIC adcSTM32EnableTSVREFE
        PUBLIC adcSTM32EnableVBATE
        PUBLIC adc_lld_init
        PUBLIC adc_lld_start
        PUBLIC adc_lld_start_conversion
        PUBLIC adc_lld_stop
        PUBLIC adc_lld_stop_conversion
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\platforms\STM32F4xx\adc_lld.c
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
//   18  * @file    STM32F4xx/adc_lld.c
//   19  * @brief   STM32F4xx/STM32F2xx ADC subsystem low level driver source.
//   20  *
//   21  * @addtogroup ADC
//   22  * @{
//   23  */
//   24 
//   25 #include "ch.h"
//   26 #include "hal.h"
//   27 
//   28 #if HAL_USE_ADC || defined(__DOXYGEN__)
//   29 
//   30 /*===========================================================================*/
//   31 /* Driver local definitions.                                                 */
//   32 /*===========================================================================*/
//   33 
//   34 #define ADC1_DMA_CHANNEL                                                    \ 
//   35   STM32_DMA_GETCHANNEL(STM32_ADC_ADC1_DMA_STREAM, STM32_ADC1_DMA_CHN)
//   36 
//   37 #define ADC2_DMA_CHANNEL                                                    \ 
//   38   STM32_DMA_GETCHANNEL(STM32_ADC_ADC2_DMA_STREAM, STM32_ADC2_DMA_CHN)
//   39 
//   40 #define ADC3_DMA_CHANNEL                                                    \ 
//   41   STM32_DMA_GETCHANNEL(STM32_ADC_ADC3_DMA_STREAM, STM32_ADC3_DMA_CHN)
//   42 
//   43 /*===========================================================================*/
//   44 /* Driver exported variables.                                                */
//   45 /*===========================================================================*/
//   46 
//   47 /** @brief ADC1 driver identifier.*/
//   48 #if STM32_ADC_USE_ADC1 || defined(__DOXYGEN__)

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   49 ADCDriver ADCD1;
ADCD1:
        DS8 52
//   50 #endif
//   51 
//   52 /** @brief ADC2 driver identifier.*/
//   53 #if STM32_ADC_USE_ADC2 || defined(__DOXYGEN__)

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   54 ADCDriver ADCD2;
ADCD2:
        DS8 52
//   55 #endif
//   56 
//   57 /** @brief ADC3 driver identifier.*/
//   58 #if STM32_ADC_USE_ADC3 || defined(__DOXYGEN__)
//   59 ADCDriver ADCD3;
//   60 #endif
//   61 
//   62 /*===========================================================================*/
//   63 /* Driver local variables and types.                                         */
//   64 /*===========================================================================*/
//   65 
//   66 /*===========================================================================*/
//   67 /* Driver local functions.                                                   */
//   68 /*===========================================================================*/
//   69 
//   70 #include "error_handling.h"
//   71 
//   72 /**
//   73  * @brief   ADC DMA ISR service routine.
//   74  *
//   75  * @param[in] adcp      pointer to the @p ADCDriver object
//   76  * @param[in] flags     pre-shifted content of the ISR register
//   77  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function adc_lld_serve_rx_interrupt
        THUMB
//   78 static void adc_lld_serve_rx_interrupt(ADCDriver *adcp, uint32_t flags) {
adc_lld_serve_rx_interrupt:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//   79 
//   80 	efiAssertVoid(getRemainingStack(chThdSelf()) > 64, "sys_adc");
        LDR.W    R0,??DataTable8
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+65
        BGE.N    ??adc_lld_serve_rx_interrupt_0
        LDR.W    R0,??DataTable8_1
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??adc_lld_serve_rx_interrupt_1
//   81 
//   82   /* DMA errors handling.*/
//   83   if ((flags & (STM32_DMA_ISR_TEIF | STM32_DMA_ISR_DMEIF)) != 0) {
??adc_lld_serve_rx_interrupt_0:
        TST      R5,#0xC
        BEQ.N    ??adc_lld_serve_rx_interrupt_2
//   84     /* DMA, this could help only if the DMA tries to access an unmapped
//   85        address space or violates alignment rules.*/
//   86     _adc_isr_error_code(adcp, ADC_ERR_DMAFAILURE);
        MOVS     R0,R4
          CFI FunCall adc_lld_stop_conversion
        BL       adc_lld_stop_conversion
        LDR      R0,[R4, #+16]
        LDR      R0,[R0, #+8]
        CMP      R0,#+0
        BEQ.N    ??adc_lld_serve_rx_interrupt_3
        MOVS     R0,#+5
        STRB     R0,[R4, #+0]
        MOVS     R1,#+0
        MOVS     R0,R4
        LDR      R2,[R4, #+16]
        LDR      R2,[R2, #+8]
          CFI FunCall
        BLX      R2
        LDRB     R0,[R4, #+0]
        CMP      R0,#+5
        BNE.N    ??adc_lld_serve_rx_interrupt_3
        MOVS     R0,#+2
        STRB     R0,[R4, #+0]
??adc_lld_serve_rx_interrupt_3:
        MOVS     R0,#+0
        STR      R0,[R4, #+16]
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
        LDR      R0,[R4, #+20]
        CMP      R0,#+0
        BEQ.N    ??adc_lld_serve_rx_interrupt_4
        LDR      R0,[R4, #+20]
        MOVS     R6,R0
        MOVS     R0,#+0
        STR      R0,[R4, #+20]
        MOVS     R0,#-1
        STR      R0,[R6, #+40]
        MOVS     R0,R6
          CFI FunCall chSchReadyI
        BL       chSchReadyI
??adc_lld_serve_rx_interrupt_4:
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
        B.N      ??adc_lld_serve_rx_interrupt_5
//   87   }
//   88   else {
//   89     /* It is possible that the conversion group has already be reset by the
//   90        ADC error handler, in this case this interrupt is spurious.*/
//   91     if (adcp->grpp != NULL) {
??adc_lld_serve_rx_interrupt_2:
        LDR      R0,[R4, #+16]
        CMP      R0,#+0
        BEQ.N    ??adc_lld_serve_rx_interrupt_5
//   92       if ((flags & STM32_DMA_ISR_TCIF) != 0) {
        LSLS     R0,R5,#+26
        BPL.N    ??adc_lld_serve_rx_interrupt_6
//   93         /* Transfer complete processing.*/
//   94         _adc_isr_full_code(adcp);
        LDR      R0,[R4, #+16]
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??adc_lld_serve_rx_interrupt_7
        LDR      R0,[R4, #+16]
        LDR      R0,[R0, #+4]
        CMP      R0,#+0
        BEQ.N    ??adc_lld_serve_rx_interrupt_5
        LDR      R0,[R4, #+12]
        CMP      R0,#+2
        BCC.N    ??adc_lld_serve_rx_interrupt_8
        LDR      R0,[R4, #+12]
        LSRS     R0,R0,#+1
        MOVS     R7,R0
        LDR      R0,[R4, #+16]
        LDRH     R0,[R0, #+2]
        MUL      R0,R0,R7
        MOVS     R6,R0
        MOVS     R2,R7
        LDR      R0,[R4, #+8]
        ADDS     R1,R0,R6, LSL #+1
        MOVS     R0,R4
        LDR      R3,[R4, #+16]
        LDR      R3,[R3, #+4]
          CFI FunCall
        BLX      R3
        B.N      ??adc_lld_serve_rx_interrupt_5
??adc_lld_serve_rx_interrupt_8:
        LDR      R2,[R4, #+12]
        LDR      R1,[R4, #+8]
        MOVS     R0,R4
        LDR      R3,[R4, #+16]
        LDR      R3,[R3, #+4]
          CFI FunCall
        BLX      R3
        B.N      ??adc_lld_serve_rx_interrupt_5
??adc_lld_serve_rx_interrupt_7:
        MOVS     R0,R4
          CFI FunCall adc_lld_stop_conversion
        BL       adc_lld_stop_conversion
        LDR      R0,[R4, #+16]
        LDR      R0,[R0, #+4]
        CMP      R0,#+0
        BEQ.N    ??adc_lld_serve_rx_interrupt_9
        MOVS     R0,#+4
        STRB     R0,[R4, #+0]
        LDR      R2,[R4, #+12]
        LDR      R1,[R4, #+8]
        MOVS     R0,R4
        LDR      R3,[R4, #+16]
        LDR      R3,[R3, #+4]
          CFI FunCall
        BLX      R3
        LDRB     R0,[R4, #+0]
        CMP      R0,#+4
        BNE.N    ??adc_lld_serve_rx_interrupt_10
        MOVS     R0,#+2
        STRB     R0,[R4, #+0]
        MOVS     R0,#+0
        STR      R0,[R4, #+16]
        B.N      ??adc_lld_serve_rx_interrupt_10
??adc_lld_serve_rx_interrupt_9:
        MOVS     R0,#+2
        STRB     R0,[R4, #+0]
        MOVS     R0,#+0
        STR      R0,[R4, #+16]
??adc_lld_serve_rx_interrupt_10:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
        LDR      R0,[R4, #+20]
        CMP      R0,#+0
        BEQ.N    ??adc_lld_serve_rx_interrupt_11
        LDR      R0,[R4, #+20]
        MOVS     R6,R0
        MOVS     R0,#+0
        STR      R0,[R4, #+20]
        MOVS     R0,#+0
        STR      R0,[R6, #+40]
        MOVS     R0,R6
          CFI FunCall chSchReadyI
        BL       chSchReadyI
??adc_lld_serve_rx_interrupt_11:
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
        B.N      ??adc_lld_serve_rx_interrupt_5
//   95       }
//   96       else if ((flags & STM32_DMA_ISR_HTIF) != 0) {
??adc_lld_serve_rx_interrupt_6:
        LSLS     R0,R5,#+27
        BPL.N    ??adc_lld_serve_rx_interrupt_5
//   97         /* Half transfer processing.*/
//   98         _adc_isr_half_code(adcp);
        LDR      R0,[R4, #+16]
        LDR      R0,[R0, #+4]
        CMP      R0,#+0
        BEQ.N    ??adc_lld_serve_rx_interrupt_5
        LDR      R0,[R4, #+12]
        LSRS     R2,R0,#+1
        LDR      R1,[R4, #+8]
        MOVS     R0,R4
        LDR      R3,[R4, #+16]
        LDR      R3,[R3, #+4]
          CFI FunCall
        BLX      R3
//   99       }
//  100     }
//  101   }
//  102 }
??adc_lld_serve_rx_interrupt_5:
??adc_lld_serve_rx_interrupt_1:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock0
//  103 
//  104 /*===========================================================================*/
//  105 /* Driver interrupt handlers.                                                */
//  106 /*===========================================================================*/
//  107 
//  108 #if STM32_ADC_USE_ADC1 || STM32_ADC_USE_ADC2 || STM32_ADC_USE_ADC3 ||       \ 
//  109     defined(__DOXYGEN__)
//  110 /**
//  111  * @brief   ADC interrupt handler.
//  112  *
//  113  * @isr
//  114  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function Vector88
        THUMB
//  115 CH_IRQ_HANDLER(ADC1_2_3_IRQHandler) {
Vector88:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
//  116   uint32_t sr;
//  117 
//  118   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  119 
//  120 #if STM32_ADC_USE_ADC1
//  121   sr = ADC1->SR;
        LDR.W    R0,??DataTable8_2  ;; 0x40012000
        LDR      R0,[R0, #+0]
        MOVS     R4,R0
//  122   ADC1->SR = 0;
        LDR.W    R0,??DataTable8_2  ;; 0x40012000
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  123   /* Note, an overflow may occur after the conversion ended before the driver
//  124      is able to stop the ADC, this is why the DMA channel is checked too.*/
//  125   if ((sr & ADC_SR_OVR) && (dmaStreamGetTransactionSize(ADCD1.dmastp) > 0)) {
        LSLS     R0,R4,#+26
        BPL.N    ??Vector88_0
        LDR.W    R0,??DataTable8_3
        LDR      R0,[R0, #+44]
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+4]
        CMP      R0,#+0
        BEQ.N    ??Vector88_0
//  126     /* ADC overflow condition, this could happen only if the DMA is unable
//  127        to read data fast enough.*/
//  128     if (ADCD1.grpp != NULL)
        LDR.W    R0,??DataTable8_3
        LDR      R0,[R0, #+16]
        CMP      R0,#+0
        BEQ.N    ??Vector88_0
//  129       _adc_isr_error_code(&ADCD1, ADC_ERR_OVERFLOW);
        LDR.W    R0,??DataTable8_3
          CFI FunCall adc_lld_stop_conversion
        BL       adc_lld_stop_conversion
        LDR.W    R0,??DataTable8_3
        LDR      R0,[R0, #+16]
        LDR      R0,[R0, #+8]
        CMP      R0,#+0
        BEQ.N    ??Vector88_1
        LDR.N    R0,??DataTable8_3
        MOVS     R1,#+5
        STRB     R1,[R0, #+0]
        MOVS     R1,#+1
        LDR.N    R0,??DataTable8_3
        LDR.N    R2,??DataTable8_3
        LDR      R2,[R2, #+16]
        LDR      R2,[R2, #+8]
          CFI FunCall
        BLX      R2
        LDR.N    R0,??DataTable8_3
        LDRB     R0,[R0, #+0]
        CMP      R0,#+5
        BNE.N    ??Vector88_1
        LDR.N    R0,??DataTable8_3
        MOVS     R1,#+2
        STRB     R1,[R0, #+0]
??Vector88_1:
        LDR.N    R0,??DataTable8_3
        MOVS     R1,#+0
        STR      R1,[R0, #+16]
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
        LDR.N    R0,??DataTable8_3
        LDR      R0,[R0, #+20]
        CMP      R0,#+0
        BEQ.N    ??Vector88_2
        LDR.N    R0,??DataTable8_3
        LDR      R0,[R0, #+20]
        MOVS     R5,R0
        LDR.N    R0,??DataTable8_3
        MOVS     R1,#+0
        STR      R1,[R0, #+20]
        MOVS     R0,#-1
        STR      R0,[R5, #+40]
        MOVS     R0,R5
          CFI FunCall chSchReadyI
        BL       chSchReadyI
??Vector88_2:
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  130   }
//  131   /* TODO: Add here analog watchdog handling.*/
//  132 #endif /* STM32_ADC_USE_ADC1 */
//  133 
//  134 #if STM32_ADC_USE_ADC2
//  135   sr = ADC2->SR;
??Vector88_0:
        LDR.N    R0,??DataTable8_4  ;; 0x40012100
        LDR      R0,[R0, #+0]
        MOVS     R4,R0
//  136   ADC2->SR = 0;
        LDR.N    R0,??DataTable8_4  ;; 0x40012100
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  137   /* Note, an overflow may occur after the conversion ended before the driver
//  138      is able to stop the ADC, this is why the DMA channel is checked too.*/
//  139   if ((sr & ADC_SR_OVR) && (dmaStreamGetTransactionSize(ADCD2.dmastp) > 0)) {
        LSLS     R0,R4,#+26
        BPL.N    ??Vector88_3
        LDR.N    R0,??DataTable8_5
        LDR      R0,[R0, #+44]
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+4]
        CMP      R0,#+0
        BEQ.N    ??Vector88_3
//  140     /* ADC overflow condition, this could happen only if the DMA is unable
//  141        to read data fast enough.*/
//  142     if (ADCD2.grpp != NULL)
        LDR.N    R0,??DataTable8_5
        LDR      R0,[R0, #+16]
        CMP      R0,#+0
        BEQ.N    ??Vector88_3
//  143       _adc_isr_error_code(&ADCD2, ADC_ERR_OVERFLOW);
        LDR.N    R0,??DataTable8_5
          CFI FunCall adc_lld_stop_conversion
        BL       adc_lld_stop_conversion
        LDR.N    R0,??DataTable8_5
        LDR      R0,[R0, #+16]
        LDR      R0,[R0, #+8]
        CMP      R0,#+0
        BEQ.N    ??Vector88_4
        LDR.N    R0,??DataTable8_5
        MOVS     R1,#+5
        STRB     R1,[R0, #+0]
        MOVS     R1,#+1
        LDR.N    R0,??DataTable8_5
        LDR.N    R2,??DataTable8_5
        LDR      R2,[R2, #+16]
        LDR      R2,[R2, #+8]
          CFI FunCall
        BLX      R2
        LDR.N    R0,??DataTable8_5
        LDRB     R0,[R0, #+0]
        CMP      R0,#+5
        BNE.N    ??Vector88_4
        LDR.N    R0,??DataTable8_5
        MOVS     R1,#+2
        STRB     R1,[R0, #+0]
??Vector88_4:
        LDR.N    R0,??DataTable8_5
        MOVS     R1,#+0
        STR      R1,[R0, #+16]
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
        LDR.N    R0,??DataTable8_5
        LDR      R0,[R0, #+20]
        CMP      R0,#+0
        BEQ.N    ??Vector88_5
        LDR.N    R0,??DataTable8_5
        LDR      R0,[R0, #+20]
        MOVS     R5,R0
        LDR.N    R0,??DataTable8_5
        MOVS     R1,#+0
        STR      R1,[R0, #+20]
        MOVS     R0,#-1
        STR      R0,[R5, #+40]
        MOVS     R0,R5
          CFI FunCall chSchReadyI
        BL       chSchReadyI
??Vector88_5:
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  144   }
//  145   /* TODO: Add here analog watchdog handling.*/
//  146 #endif /* STM32_ADC_USE_ADC2 */
//  147 
//  148 #if STM32_ADC_USE_ADC3
//  149   sr = ADC3->SR;
//  150   ADC3->SR = 0;
//  151   /* Note, an overflow may occur after the conversion ended before the driver
//  152      is able to stop the ADC, this is why the DMA channel is checked too.*/
//  153   if ((sr & ADC_SR_OVR) && (dmaStreamGetTransactionSize(ADCD3.dmastp) > 0)) {
//  154     /* ADC overflow condition, this could happen only if the DMA is unable
//  155        to read data fast enough.*/
//  156     if (ADCD3.grpp != NULL)
//  157       _adc_isr_error_code(&ADCD3, ADC_ERR_OVERFLOW);
//  158   }
//  159   /* TODO: Add here analog watchdog handling.*/
//  160 #endif /* STM32_ADC_USE_ADC3 */
//  161 
//  162   CH_IRQ_EPILOGUE();
??Vector88_3:
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  163 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock1
//  164 #endif
//  165 
//  166 /*===========================================================================*/
//  167 /* Driver exported functions.                                                */
//  168 /*===========================================================================*/
//  169 
//  170 /**
//  171  * @brief   Low level ADC driver initialization.
//  172  *
//  173  * @notapi
//  174  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function adc_lld_init
        THUMB
//  175 void adc_lld_init(void) {
adc_lld_init:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  176 
//  177 #if STM32_ADC_USE_ADC1
//  178   /* Driver initialization.*/
//  179   adcObjectInit(&ADCD1);
        LDR.N    R0,??DataTable8_3
          CFI FunCall adcObjectInit
        BL       adcObjectInit
//  180   ADCD1.adc = ADC1;
        LDR.N    R0,??DataTable8_3
        LDR.N    R1,??DataTable8_2  ;; 0x40012000
        STR      R1,[R0, #+40]
//  181   ADCD1.dmastp  = STM32_DMA_STREAM(STM32_ADC_ADC1_DMA_STREAM);
        LDR.N    R0,??DataTable8_3
        LDR.N    R1,??DataTable8_6
        STR      R1,[R0, #+44]
//  182   ADCD1.dmamode = STM32_DMA_CR_CHSEL(ADC1_DMA_CHANNEL) |
//  183                   STM32_DMA_CR_PL(STM32_ADC_ADC1_DMA_PRIORITY) |
//  184                   STM32_DMA_CR_DIR_P2M |
//  185                   STM32_DMA_CR_MSIZE_HWORD | STM32_DMA_CR_PSIZE_HWORD |
//  186                   STM32_DMA_CR_MINC        | STM32_DMA_CR_TCIE        |
//  187                   STM32_DMA_CR_DMEIE       | STM32_DMA_CR_TEIE;
        LDR.N    R0,??DataTable8_3
        LDR.N    R1,??DataTable8_7  ;; 0x22c16
        STR      R1,[R0, #+48]
//  188 #endif
//  189 
//  190 #if STM32_ADC_USE_ADC2
//  191   /* Driver initialization.*/
//  192   adcObjectInit(&ADCD2);
        LDR.N    R0,??DataTable8_5
          CFI FunCall adcObjectInit
        BL       adcObjectInit
//  193   ADCD2.adc = ADC2;
        LDR.N    R0,??DataTable8_5
        LDR.N    R1,??DataTable8_4  ;; 0x40012100
        STR      R1,[R0, #+40]
//  194   ADCD2.dmastp  = STM32_DMA_STREAM(STM32_ADC_ADC2_DMA_STREAM);
        LDR.N    R0,??DataTable8_5
        LDR.N    R1,??DataTable8_8
        STR      R1,[R0, #+44]
//  195   ADCD2.dmamode = STM32_DMA_CR_CHSEL(ADC2_DMA_CHANNEL) |
//  196                   STM32_DMA_CR_PL(STM32_ADC_ADC2_DMA_PRIORITY) |
//  197                   STM32_DMA_CR_DIR_P2M |
//  198                   STM32_DMA_CR_MSIZE_HWORD | STM32_DMA_CR_PSIZE_HWORD |
//  199                   STM32_DMA_CR_MINC        | STM32_DMA_CR_TCIE        |
//  200                   STM32_DMA_CR_DMEIE       | STM32_DMA_CR_TEIE;
        LDR.N    R0,??DataTable8_5
        LDR.N    R1,??DataTable8_9  ;; 0x2022c16
        STR      R1,[R0, #+48]
//  201 #endif
//  202 
//  203 #if STM32_ADC_USE_ADC3
//  204   /* Driver initialization.*/
//  205   adcObjectInit(&ADCD3);
//  206   ADCD3.adc = ADC3;
//  207   ADCD3.dmastp  = STM32_DMA_STREAM(STM32_ADC_ADC3_DMA_STREAM);
//  208   ADCD3.dmamode = STM32_DMA_CR_CHSEL(ADC3_DMA_CHANNEL) |
//  209                   STM32_DMA_CR_PL(STM32_ADC_ADC3_DMA_PRIORITY) |
//  210                   STM32_DMA_CR_DIR_P2M |
//  211                   STM32_DMA_CR_MSIZE_HWORD | STM32_DMA_CR_PSIZE_HWORD |
//  212                   STM32_DMA_CR_MINC        | STM32_DMA_CR_TCIE        |
//  213                   STM32_DMA_CR_DMEIE       | STM32_DMA_CR_TEIE;
//  214 #endif
//  215 
//  216   /* The shared vector is initialized on driver initialization and never
//  217      disabled.*/
//  218   nvicEnableVector(ADC_IRQn, CORTEX_PRIORITY_MASK(STM32_ADC_IRQ_PRIORITY));
        MOVS     R1,#+96
        MOVS     R0,#+18
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  219 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock2
//  220 
//  221 /**
//  222  * @brief   Configures and activates the ADC peripheral.
//  223  *
//  224  * @param[in] adcp      pointer to the @p ADCDriver object
//  225  *
//  226  * @notapi
//  227  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function adc_lld_start
        THUMB
//  228 void adc_lld_start(ADCDriver *adcp) {
adc_lld_start:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  229 
//  230   /* If in stopped state then enables the ADC and DMA clocks.*/
//  231   if (adcp->state == ADC_STOP) {
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BNE.N    ??adc_lld_start_0
//  232 #if STM32_ADC_USE_ADC1
//  233     if (&ADCD1 == adcp) {
        LDR.N    R0,??DataTable8_3
        CMP      R0,R4
        BNE.N    ??adc_lld_start_1
//  234       bool_t b;
//  235       b = dmaStreamAllocate(adcp->dmastp,
//  236                             STM32_ADC_ADC1_DMA_IRQ_PRIORITY,
//  237                             (stm32_dmaisr_t)adc_lld_serve_rx_interrupt,
//  238                             (void *)adcp);
        MOVS     R3,R4
        LDR.N    R2,??DataTable8_10
        MOVS     R1,#+6
        LDR      R0,[R4, #+44]
          CFI FunCall dmaStreamAllocate
        BL       dmaStreamAllocate
        MOVS     R5,R0
//  239       chDbgAssert(!b, "adc_lld_start(), #1", "stream already allocated");
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BEQ.N    ??adc_lld_start_2
        LDR.N    R0,??DataTable8_11
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  240       dmaStreamSetPeripheral(adcp->dmastp, &ADC1->DR);
??adc_lld_start_2:
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable8_12  ;; 0x4001204c
        STR      R1,[R0, #+8]
//  241       rccEnableADC1(FALSE);
        LDR.N    R0,??DataTable8_13  ;; 0x40023844
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x100
        LDR.N    R1,??DataTable8_13  ;; 0x40023844
        STR      R0,[R1, #+0]
//  242     }
//  243 #endif /* STM32_ADC_USE_ADC1 */
//  244 
//  245 #if STM32_ADC_USE_ADC2
//  246     if (&ADCD2 == adcp) {
??adc_lld_start_1:
        LDR.N    R0,??DataTable8_5
        CMP      R0,R4
        BNE.N    ??adc_lld_start_3
//  247       bool_t b;
//  248       b = dmaStreamAllocate(adcp->dmastp,
//  249                             STM32_ADC_ADC2_DMA_IRQ_PRIORITY,
//  250                             (stm32_dmaisr_t)adc_lld_serve_rx_interrupt,
//  251                             (void *)adcp);
        MOVS     R3,R4
        LDR.N    R2,??DataTable8_10
        MOVS     R1,#+6
        LDR      R0,[R4, #+44]
          CFI FunCall dmaStreamAllocate
        BL       dmaStreamAllocate
        MOVS     R5,R0
//  252       chDbgAssert(!b, "adc_lld_start(), #2", "stream already allocated");
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BEQ.N    ??adc_lld_start_4
        LDR.N    R0,??DataTable8_14
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  253       dmaStreamSetPeripheral(adcp->dmastp, &ADC2->DR);
??adc_lld_start_4:
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable8_15  ;; 0x4001214c
        STR      R1,[R0, #+8]
//  254       rccEnableADC2(FALSE);
        LDR.N    R0,??DataTable8_13  ;; 0x40023844
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x200
        LDR.N    R1,??DataTable8_13  ;; 0x40023844
        STR      R0,[R1, #+0]
//  255     }
//  256 #endif /* STM32_ADC_USE_ADC2 */
//  257 
//  258 #if STM32_ADC_USE_ADC3
//  259     if (&ADCD3 == adcp) {
//  260       bool_t b;
//  261       b = dmaStreamAllocate(adcp->dmastp,
//  262                             STM32_ADC_ADC3_DMA_IRQ_PRIORITY,
//  263                             (stm32_dmaisr_t)adc_lld_serve_rx_interrupt,
//  264                             (void *)adcp);
//  265       chDbgAssert(!b, "adc_lld_start(), #3", "stream already allocated");
//  266       dmaStreamSetPeripheral(adcp->dmastp, &ADC3->DR);
//  267       rccEnableADC3(FALSE);
//  268     }
//  269 #endif /* STM32_ADC_USE_ADC3 */
//  270 
//  271     /* This is a common register but apparently it requires that at least one
//  272        of the ADCs is clocked in order to allow writing, see bug 3575297.*/
//  273     ADC->CCR = (ADC->CCR & (ADC_CCR_TSVREFE | ADC_CCR_VBATE)) |
//  274                (STM32_ADC_ADCPRE << 16);
??adc_lld_start_3:
        LDR.N    R0,??DataTable8_16  ;; 0x40012304
        LDR      R0,[R0, #+0]
        ANDS     R0,R0,#0xC00000
        ORRS     R0,R0,#0x10000
        LDR.N    R1,??DataTable8_16  ;; 0x40012304
        STR      R0,[R1, #+0]
//  275 
//  276     /* ADC initial setup, starting the analog part here in order to reduce
//  277        the latency when starting a conversion.*/
//  278     adcp->adc->CR1 = 0;
        LDR      R0,[R4, #+40]
        MOVS     R1,#+0
        STR      R1,[R0, #+4]
//  279     adcp->adc->CR2 = 0;
        LDR      R0,[R4, #+40]
        MOVS     R1,#+0
        STR      R1,[R0, #+8]
//  280     adcp->adc->CR2 = ADC_CR2_ADON;
        LDR      R0,[R4, #+40]
        MOVS     R1,#+1
        STR      R1,[R0, #+8]
//  281   }
//  282 }
??adc_lld_start_0:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock3
//  283 
//  284 /**
//  285  * @brief   Deactivates the ADC peripheral.
//  286  *
//  287  * @param[in] adcp      pointer to the @p ADCDriver object
//  288  *
//  289  * @notapi
//  290  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function adc_lld_stop
        THUMB
//  291 void adc_lld_stop(ADCDriver *adcp) {
adc_lld_stop:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  292 
//  293   /* If in ready state then disables the ADC clock.*/
//  294   if (adcp->state == ADC_READY) {
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BNE.N    ??adc_lld_stop_0
//  295     dmaStreamRelease(adcp->dmastp);
        LDR      R0,[R4, #+44]
          CFI FunCall dmaStreamRelease
        BL       dmaStreamRelease
//  296     adcp->adc->CR1 = 0;
        LDR      R0,[R4, #+40]
        MOVS     R1,#+0
        STR      R1,[R0, #+4]
//  297     adcp->adc->CR2 = 0;
        LDR      R0,[R4, #+40]
        MOVS     R1,#+0
        STR      R1,[R0, #+8]
//  298 
//  299 #if STM32_ADC_USE_ADC1
//  300     if (&ADCD1 == adcp)
        LDR.N    R0,??DataTable8_3
        CMP      R0,R4
        BNE.N    ??adc_lld_stop_1
//  301       rccDisableADC1(FALSE);
        LDR.N    R0,??DataTable8_13  ;; 0x40023844
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x100
        LDR.N    R1,??DataTable8_13  ;; 0x40023844
        STR      R0,[R1, #+0]
//  302 #endif
//  303 
//  304 #if STM32_ADC_USE_ADC2
//  305     if (&ADCD2 == adcp)
??adc_lld_stop_1:
        LDR.N    R0,??DataTable8_5
        CMP      R0,R4
        BNE.N    ??adc_lld_stop_0
//  306       rccDisableADC2(FALSE);
        LDR.N    R0,??DataTable8_13  ;; 0x40023844
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x200
        LDR.N    R1,??DataTable8_13  ;; 0x40023844
        STR      R0,[R1, #+0]
//  307 #endif
//  308 
//  309 #if STM32_ADC_USE_ADC3
//  310     if (&ADCD3 == adcp)
//  311       rccDisableADC3(FALSE);
//  312 #endif
//  313   }
//  314 }
??adc_lld_stop_0:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock4
//  315 
//  316 /**
//  317  * @brief   Starts an ADC conversion.
//  318  *
//  319  * @param[in] adcp      pointer to the @p ADCDriver object
//  320  *
//  321  * @notapi
//  322  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function adc_lld_start_conversion
          CFI NoCalls
        THUMB
//  323 void adc_lld_start_conversion(ADCDriver *adcp) {
adc_lld_start_conversion:
        PUSH     {R4,R5}
          CFI R5 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  324   uint32_t mode;
//  325   uint32_t cr2;
//  326   const ADCConversionGroup *grpp = adcp->grpp;
        LDR      R2,[R0, #+16]
//  327 
//  328   /* DMA setup.*/
//  329   mode = adcp->dmamode;
        LDR      R4,[R0, #+48]
        MOVS     R3,R4
//  330   if (grpp->circular) {
        LDRB     R4,[R2, #+0]
        CMP      R4,#+0
        BEQ.N    ??adc_lld_start_conversion_0
//  331     mode |= STM32_DMA_CR_CIRC;
        ORRS     R3,R3,#0x100
//  332     if (adcp->depth > 1) {
        LDR      R4,[R0, #+12]
        CMP      R4,#+2
        BCC.N    ??adc_lld_start_conversion_0
//  333       /* If circular buffer depth > 1, then the half transfer interrupt
//  334          is enabled in order to allow streaming processing.*/
//  335       mode |= STM32_DMA_CR_HTIE;
        ORRS     R3,R3,#0x8
//  336     }
//  337   }
//  338   dmaStreamSetMemory0(adcp->dmastp, adcp->samples);
??adc_lld_start_conversion_0:
        LDR      R4,[R0, #+44]
        LDR      R4,[R4, #+0]
        LDR      R5,[R0, #+8]
        STR      R5,[R4, #+12]
//  339   dmaStreamSetTransactionSize(adcp->dmastp, (uint32_t)grpp->num_channels *
//  340                                             (uint32_t)adcp->depth);
        LDRH     R4,[R2, #+2]
        LDR      R5,[R0, #+12]
        MULS     R4,R5,R4
        LDR      R5,[R0, #+44]
        LDR      R5,[R5, #+0]
        STR      R4,[R5, #+4]
//  341   dmaStreamSetMode(adcp->dmastp, mode);
        LDR      R4,[R0, #+44]
        LDR      R4,[R4, #+0]
        STR      R3,[R4, #+0]
//  342   dmaStreamEnable(adcp->dmastp);
        LDR      R4,[R0, #+44]
        LDR      R4,[R4, #+0]
        LDR      R4,[R4, #+0]
        ORRS     R4,R4,#0x1
        LDR      R5,[R0, #+44]
        LDR      R5,[R5, #+0]
        STR      R4,[R5, #+0]
//  343 
//  344   /* ADC setup.*/
//  345   adcp->adc->SR    = 0;
        LDR      R4,[R0, #+40]
        MOVS     R5,#+0
        STR      R5,[R4, #+0]
//  346   adcp->adc->SMPR1 = grpp->smpr1;
        LDR      R4,[R0, #+40]
        LDR      R5,[R2, #+20]
        STR      R5,[R4, #+12]
//  347   adcp->adc->SMPR2 = grpp->smpr2;
        LDR      R4,[R0, #+40]
        LDR      R5,[R2, #+24]
        STR      R5,[R4, #+16]
//  348   adcp->adc->SQR1  = grpp->sqr1;
        LDR      R4,[R0, #+40]
        LDR      R5,[R2, #+28]
        STR      R5,[R4, #+44]
//  349   adcp->adc->SQR2  = grpp->sqr2;
        LDR      R4,[R0, #+40]
        LDR      R5,[R2, #+32]
        STR      R5,[R4, #+48]
//  350   adcp->adc->SQR3  = grpp->sqr3;
        LDR      R4,[R0, #+40]
        LDR      R5,[R2, #+36]
        STR      R5,[R4, #+52]
//  351 
//  352   /* ADC configuration and start.*/
//  353   adcp->adc->CR1   = grpp->cr1 | ADC_CR1_OVRIE | ADC_CR1_SCAN;
        LDR      R4,[R0, #+40]
        LDR      R5,[R2, #+12]
        ORR      R5,R5,#0x4000000
        ORRS     R5,R5,#0x100
        STR      R5,[R4, #+4]
//  354 
//  355   /* Enforcing the mandatory bits in CR2.*/
//  356   cr2 = grpp->cr2 | ADC_CR2_DMA | ADC_CR2_DDS | ADC_CR2_ADON;
        LDR      R4,[R2, #+16]
        MOVW     R5,#+769
        ORRS     R4,R5,R4
        MOVS     R1,R4
//  357 
//  358   /* The start method is different dependign if HW or SW triggered, the
//  359      start is performed using the method specified in the CR2 configuration.*/
//  360   if ((cr2 & ADC_CR2_SWSTART) != 0) {
        LSLS     R4,R1,#+1
        BPL.N    ??adc_lld_start_conversion_1
//  361     /* Initializing CR2 while keeping ADC_CR2_SWSTART at zero.*/
//  362     adcp->adc->CR2 = (cr2 | ADC_CR2_CONT) & ~ADC_CR2_SWSTART;
        LDR      R4,[R0, #+40]
        BICS     R5,R1,#0x40000000
        ORRS     R5,R5,#0x2
        STR      R5,[R4, #+8]
//  363 
//  364     /* Finally enabling ADC_CR2_SWSTART.*/
//  365     adcp->adc->CR2 = (cr2 | ADC_CR2_CONT);
        LDR      R4,[R0, #+40]
        ORRS     R5,R1,#0x2
        STR      R5,[R4, #+8]
        B.N      ??adc_lld_start_conversion_2
//  366   }
//  367   else
//  368     adcp->adc->CR2 = cr2;
??adc_lld_start_conversion_1:
        LDR      R4,[R0, #+40]
        STR      R1,[R4, #+8]
//  369 }
??adc_lld_start_conversion_2:
        POP      {R4,R5}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock5
//  370 
//  371 /**
//  372  * @brief   Stops an ongoing conversion.
//  373  *
//  374  * @param[in] adcp      pointer to the @p ADCDriver object
//  375  *
//  376  * @notapi
//  377  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function adc_lld_stop_conversion
          CFI NoCalls
        THUMB
//  378 void adc_lld_stop_conversion(ADCDriver *adcp) {
//  379 
//  380   dmaStreamDisable(adcp->dmastp);
adc_lld_stop_conversion:
        LDR      R1,[R0, #+44]
        LDR      R1,[R1, #+0]
        LDR      R1,[R1, #+0]
        LSRS     R1,R1,#+5
        LSLS     R1,R1,#+5
        LDR      R2,[R0, #+44]
        LDR      R2,[R2, #+0]
        STR      R1,[R2, #+0]
??adc_lld_stop_conversion_0:
        LDR      R1,[R0, #+44]
        LDR      R1,[R1, #+0]
        LDR      R1,[R1, #+0]
        LSLS     R1,R1,#+31
        BMI.N    ??adc_lld_stop_conversion_0
        MOVS     R1,#+61
        LDR      R2,[R0, #+44]
        LDRSB    R2,[R2, #+8]
        LSLS     R1,R1,R2
        LDR      R2,[R0, #+44]
        LDR      R2,[R2, #+4]
        STR      R1,[R2, #+0]
//  381   adcp->adc->CR1 = 0;
        LDR      R1,[R0, #+40]
        MOVS     R2,#+0
        STR      R2,[R1, #+4]
//  382   adcp->adc->CR2 = 0;
        LDR      R1,[R0, #+40]
        MOVS     R2,#+0
        STR      R2,[R1, #+8]
//  383   adcp->adc->CR2 = ADC_CR2_ADON;
        LDR      R1,[R0, #+40]
        MOVS     R2,#+1
        STR      R2,[R1, #+8]
//  384 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock6
//  385 
//  386 /**
//  387  * @brief   Enables the TSVREFE bit.
//  388  * @details The TSVREFE bit is required in order to sample the internal
//  389  *          temperature sensor and internal reference voltage.
//  390  * @note    This is an STM32-only functionality.
//  391  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function adcSTM32EnableTSVREFE
          CFI NoCalls
        THUMB
//  392 void adcSTM32EnableTSVREFE(void) {
//  393 
//  394   ADC->CCR |= ADC_CCR_TSVREFE;
adcSTM32EnableTSVREFE:
        LDR.N    R0,??DataTable8_16  ;; 0x40012304
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x800000
        LDR.N    R1,??DataTable8_16  ;; 0x40012304
        STR      R0,[R1, #+0]
//  395 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock7
//  396 
//  397 /**
//  398  * @brief   Disables the TSVREFE bit.
//  399  * @details The TSVREFE bit is required in order to sample the internal
//  400  *          temperature sensor and internal reference voltage.
//  401  * @note    This is an STM32-only functionality.
//  402  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function adcSTM32DisableTSVREFE
          CFI NoCalls
        THUMB
//  403 void adcSTM32DisableTSVREFE(void) {
//  404 
//  405   ADC->CCR &= ~ADC_CCR_TSVREFE;
adcSTM32DisableTSVREFE:
        LDR.N    R0,??DataTable8_16  ;; 0x40012304
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x800000
        LDR.N    R1,??DataTable8_16  ;; 0x40012304
        STR      R0,[R1, #+0]
//  406 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock8
//  407 
//  408 /**
//  409  * @brief   Enables the VBATE bit.
//  410  * @details The VBATE bit is required in order to sample the VBAT channel.
//  411  * @note    This is an STM32-only functionality.
//  412  * @note    This function is meant to be called after @p adcStart().
//  413  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function adcSTM32EnableVBATE
          CFI NoCalls
        THUMB
//  414 void adcSTM32EnableVBATE(void) {
//  415 
//  416   ADC->CCR |= ADC_CCR_VBATE;
adcSTM32EnableVBATE:
        LDR.N    R0,??DataTable8_16  ;; 0x40012304
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x400000
        LDR.N    R1,??DataTable8_16  ;; 0x40012304
        STR      R0,[R1, #+0]
//  417 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock9
//  418 
//  419 /**
//  420  * @brief   Disables the VBATE bit.
//  421  * @details The VBATE bit is required in order to sample the VBAT channel.
//  422  * @note    This is an STM32-only functionality.
//  423  * @note    This function is meant to be called after @p adcStart().
//  424  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function adcSTM32DisableVBATE
          CFI NoCalls
        THUMB
//  425 void adcSTM32DisableVBATE(void) {
//  426 
//  427   ADC->CCR &= ~ADC_CCR_VBATE;
adcSTM32DisableVBATE:
        LDR.N    R0,??DataTable8_16  ;; 0x40012304
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x400000
        LDR.N    R1,??DataTable8_16  ;; 0x40012304
        STR      R0,[R1, #+0]
//  428 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock10

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_1:
        DC32     `?<Constant "sys_adc">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_2:
        DC32     0x40012000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_3:
        DC32     ADCD1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_4:
        DC32     0x40012100

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_5:
        DC32     ADCD2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_6:
        DC32     _stm32_dma_streams+0x90

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_7:
        DC32     0x22c16

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_8:
        DC32     _stm32_dma_streams+0x78

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_9:
        DC32     0x2022c16

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_10:
        DC32     adc_lld_serve_rx_interrupt

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_11:
        DC32     `?<Constant "adc_lld_start(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_12:
        DC32     0x4001204c

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_13:
        DC32     0x40023844

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_14:
        DC32     `?<Constant "adc_lld_start(), #2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_15:
        DC32     0x4001214c

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_16:
        DC32     0x40012304

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "sys_adc">`:
        DATA
        DC8 "sys_adc"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "adc_lld_start(), #1">`:
        DATA
        DC8 "adc_lld_start(), #1"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "adc_lld_start(), #2">`:
        DATA
        DC8 "adc_lld_start(), #2"

        END
//  429 
//  430 #endif /* HAL_USE_ADC */
//  431 
//  432 /** @} */
// 
//   104 bytes in section .bss
//    48 bytes in section .rodata
// 1 272 bytes in section .text
// 
// 1 272 bytes of CODE  memory
//    48 bytes of CONST memory
//   104 bytes of DATA  memory
//
//Errors: none
//Warnings: none
