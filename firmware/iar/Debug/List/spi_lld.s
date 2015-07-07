///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:23 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32\SPIv1\spi_lld.c                      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32\SPIv1\spi_lld.c -lCN                 /
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
//                       spi_lld.s                                            /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME spi_lld

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _stm32_dma_streams
        EXTERN chDbgPanic
        EXTERN chSchReadyI
        EXTERN dbg_check_lock_from_isr
        EXTERN dbg_check_unlock_from_isr
        EXTERN dmaStreamAllocate
        EXTERN dmaStreamRelease
        EXTERN spiObjectInit

        PUBLIC SPID1
        PUBLIC SPID2
        PUBLIC SPID3
        PUBLIC spi_lld_exchange
        PUBLIC spi_lld_ignore
        PUBLIC spi_lld_init
        PUBLIC spi_lld_polled_exchange
        PUBLIC spi_lld_receive
        PUBLIC spi_lld_select
        PUBLIC spi_lld_send
        PUBLIC spi_lld_start
        PUBLIC spi_lld_stop
        PUBLIC spi_lld_unselect
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\platforms\STM32\SPIv1\spi_lld.c
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
//   18  * @file    STM32/SPIv1/spi_lld.c
//   19  * @brief   STM32 SPI subsystem low level driver source.
//   20  *
//   21  * @addtogroup SPI
//   22  * @{
//   23  */
//   24 
//   25 #include "ch.h"
//   26 #include "hal.h"
//   27 
//   28 #if HAL_USE_SPI || defined(__DOXYGEN__)
//   29 
//   30 /*===========================================================================*/
//   31 /* Driver local definitions.                                                 */
//   32 /*===========================================================================*/
//   33 
//   34 #define SPI1_RX_DMA_CHANNEL                                                 \ 
//   35   STM32_DMA_GETCHANNEL(STM32_SPI_SPI1_RX_DMA_STREAM,                        \ 
//   36                        STM32_SPI1_RX_DMA_CHN)
//   37 
//   38 #define SPI1_TX_DMA_CHANNEL                                                 \ 
//   39   STM32_DMA_GETCHANNEL(STM32_SPI_SPI1_TX_DMA_STREAM,                        \ 
//   40                        STM32_SPI1_TX_DMA_CHN)
//   41 
//   42 #define SPI2_RX_DMA_CHANNEL                                                 \ 
//   43   STM32_DMA_GETCHANNEL(STM32_SPI_SPI2_RX_DMA_STREAM,                        \ 
//   44                        STM32_SPI2_RX_DMA_CHN)
//   45 
//   46 #define SPI2_TX_DMA_CHANNEL                                                 \ 
//   47   STM32_DMA_GETCHANNEL(STM32_SPI_SPI2_TX_DMA_STREAM,                        \ 
//   48                        STM32_SPI2_TX_DMA_CHN)
//   49 
//   50 #define SPI3_RX_DMA_CHANNEL                                                 \ 
//   51   STM32_DMA_GETCHANNEL(STM32_SPI_SPI3_RX_DMA_STREAM,                        \ 
//   52                        STM32_SPI3_RX_DMA_CHN)
//   53 
//   54 #define SPI3_TX_DMA_CHANNEL                                                 \ 
//   55   STM32_DMA_GETCHANNEL(STM32_SPI_SPI3_TX_DMA_STREAM,                        \ 
//   56                        STM32_SPI3_TX_DMA_CHN)
//   57 
//   58 #define SPI4_RX_DMA_CHANNEL                                                 \ 
//   59   STM32_DMA_GETCHANNEL(STM32_SPI_SPI4_RX_DMA_STREAM,                        \ 
//   60                        STM32_SPI4_RX_DMA_CHN)
//   61 
//   62 #define SPI4_TX_DMA_CHANNEL                                                 \ 
//   63   STM32_DMA_GETCHANNEL(STM32_SPI_SPI4_TX_DMA_STREAM,                        \ 
//   64                        STM32_SPI4_TX_DMA_CHN)
//   65 
//   66 #define SPI5_RX_DMA_CHANNEL                                                 \ 
//   67   STM32_DMA_GETCHANNEL(STM32_SPI_SPI5_RX_DMA_STREAM,                        \ 
//   68                        STM32_SPI5_RX_DMA_CHN)
//   69 
//   70 #define SPI5_TX_DMA_CHANNEL                                                 \ 
//   71   STM32_DMA_GETCHANNEL(STM32_SPI_SPI5_TX_DMA_STREAM,                        \ 
//   72                        STM32_SPI5_TX_DMA_CHN)
//   73 
//   74 #define SPI6_RX_DMA_CHANNEL                                                 \ 
//   75   STM32_DMA_GETCHANNEL(STM32_SPI_SPI6_RX_DMA_STREAM,                        \ 
//   76                        STM32_SPI6_RX_DMA_CHN)
//   77 
//   78 #define SPI6_TX_DMA_CHANNEL                                                 \ 
//   79   STM32_DMA_GETCHANNEL(STM32_SPI_SPI6_TX_DMA_STREAM,                        \ 
//   80                        STM32_SPI6_TX_DMA_CHN)
//   81 
//   82 /*===========================================================================*/
//   83 /* Driver exported variables.                                                */
//   84 /*===========================================================================*/
//   85 
//   86 /** @brief SPI1 driver identifier.*/
//   87 #if STM32_SPI_USE_SPI1 || defined(__DOXYGEN__)

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   88 SPIDriver SPID1;
SPID1:
        DS8 48
//   89 #endif
//   90 
//   91 /** @brief SPI2 driver identifier.*/
//   92 #if STM32_SPI_USE_SPI2 || defined(__DOXYGEN__)

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   93 SPIDriver SPID2;
SPID2:
        DS8 48
//   94 #endif
//   95 
//   96 /** @brief SPI3 driver identifier.*/
//   97 #if STM32_SPI_USE_SPI3 || defined(__DOXYGEN__)

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   98 SPIDriver SPID3;
SPID3:
        DS8 48
//   99 #endif
//  100 
//  101 /** @brief SPI4 driver identifier.*/
//  102 #if STM32_SPI_USE_SPI4 || defined(__DOXYGEN__)
//  103 SPIDriver SPID4;
//  104 #endif
//  105 
//  106 /** @brief SPI5 driver identifier.*/
//  107 #if STM32_SPI_USE_SPI5 || defined(__DOXYGEN__)
//  108 SPIDriver SPID5;
//  109 #endif
//  110 
//  111 /** @brief SPI6 driver identifier.*/
//  112 #if STM32_SPI_USE_SPI6 || defined(__DOXYGEN__)
//  113 SPIDriver SPID6;
//  114 #endif
//  115 
//  116 /*===========================================================================*/
//  117 /* Driver local variables and types.                                         */
//  118 /*===========================================================================*/
//  119 

        SECTION `.bss`:DATA:REORDER:NOROOT(1)
//  120 static uint16_t dummytx;
dummytx:
        DS8 2

        SECTION `.bss`:DATA:REORDER:NOROOT(1)
//  121 static uint16_t dummyrx;
dummyrx:
        DS8 2
//  122 
//  123 /*===========================================================================*/
//  124 /* Driver local functions.                                                   */
//  125 /*===========================================================================*/
//  126 
//  127 /**
//  128  * @brief   Shared end-of-rx service routine.
//  129  *
//  130  * @param[in] spip      pointer to the @p SPIDriver object
//  131  * @param[in] flags     pre-shifted content of the ISR register
//  132  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function spi_lld_serve_rx_interrupt
        THUMB
//  133 static void spi_lld_serve_rx_interrupt(SPIDriver *spip, uint32_t flags) {
spi_lld_serve_rx_interrupt:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  134 
//  135   /* DMA errors handling.*/
//  136 #if defined(STM32_SPI_DMA_ERROR_HOOK)
//  137   if ((flags & (STM32_DMA_ISR_TEIF | STM32_DMA_ISR_DMEIF)) != 0) {
        TST      R5,#0xC
//  138     STM32_SPI_DMA_ERROR_HOOK(spip);
//  139   }
//  140 #else
//  141   (void)flags;
//  142 #endif
//  143 
//  144   /* Stop everything.*/
//  145   dmaStreamDisable(spip->dmatx);
??spi_lld_serve_rx_interrupt_0:
        LDR      R0,[R4, #+36]
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+0]
        LSRS     R0,R0,#+5
        LSLS     R0,R0,#+5
        LDR      R1,[R4, #+36]
        LDR      R1,[R1, #+0]
        STR      R0,[R1, #+0]
??spi_lld_serve_rx_interrupt_1:
        LDR      R0,[R4, #+36]
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+0]
        LSLS     R0,R0,#+31
        BMI.N    ??spi_lld_serve_rx_interrupt_1
        MOVS     R0,#+61
        LDR      R1,[R4, #+36]
        LDRSB    R1,[R1, #+8]
        LSLS     R0,R0,R1
        LDR      R1,[R4, #+36]
        LDR      R1,[R1, #+4]
        STR      R0,[R1, #+0]
//  146   dmaStreamDisable(spip->dmarx);
        LDR      R0,[R4, #+32]
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+0]
        LSRS     R0,R0,#+5
        LSLS     R0,R0,#+5
        LDR      R1,[R4, #+32]
        LDR      R1,[R1, #+0]
        STR      R0,[R1, #+0]
??spi_lld_serve_rx_interrupt_2:
        LDR      R0,[R4, #+32]
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+0]
        LSLS     R0,R0,#+31
        BMI.N    ??spi_lld_serve_rx_interrupt_2
        MOVS     R0,#+61
        LDR      R1,[R4, #+32]
        LDRSB    R1,[R1, #+8]
        LSLS     R0,R0,R1
        LDR      R1,[R4, #+32]
        LDR      R1,[R1, #+4]
        STR      R0,[R1, #+0]
//  147 
//  148   /* Portable SPI ISR code defined in the high level driver, note, it is
//  149      a macro.*/
//  150   _spi_isr_code(spip);
        LDR      R0,[R4, #+4]
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??spi_lld_serve_rx_interrupt_3
        MOVS     R0,#+4
        STRB     R0,[R4, #+0]
        MOVS     R0,R4
        LDR      R1,[R4, #+4]
        LDR      R1,[R1, #+0]
          CFI FunCall
        BLX      R1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+4
        BNE.N    ??spi_lld_serve_rx_interrupt_4
        MOVS     R0,#+2
        STRB     R0,[R4, #+0]
        B.N      ??spi_lld_serve_rx_interrupt_4
??spi_lld_serve_rx_interrupt_3:
        MOVS     R0,#+2
        STRB     R0,[R4, #+0]
??spi_lld_serve_rx_interrupt_4:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BEQ.N    ??spi_lld_serve_rx_interrupt_5
        LDR      R0,[R4, #+8]
        MOVS     R6,R0
        MOVS     R0,#+0
        STR      R0,[R4, #+8]
        MOVS     R0,#+0
        STR      R0,[R6, #+40]
        MOVS     R0,R6
          CFI FunCall chSchReadyI
        BL       chSchReadyI
??spi_lld_serve_rx_interrupt_5:
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  151 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock0
//  152 
//  153 /**
//  154  * @brief   Shared end-of-tx service routine.
//  155  *
//  156  * @param[in] spip      pointer to the @p SPIDriver object
//  157  * @param[in] flags     pre-shifted content of the ISR register
//  158  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function spi_lld_serve_tx_interrupt
          CFI NoCalls
        THUMB
//  159 static void spi_lld_serve_tx_interrupt(SPIDriver *spip, uint32_t flags) {
//  160 
//  161   /* DMA errors handling.*/
//  162 #if defined(STM32_SPI_DMA_ERROR_HOOK)
//  163   (void)spip;
//  164   if ((flags & (STM32_DMA_ISR_TEIF | STM32_DMA_ISR_DMEIF)) != 0) {
spi_lld_serve_tx_interrupt:
        TST      R1,#0xC
//  165     STM32_SPI_DMA_ERROR_HOOK(spip);
//  166   }
//  167 #else
//  168   (void)spip;
//  169   (void)flags;
//  170 #endif
//  171 }
??spi_lld_serve_tx_interrupt_0:
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//  172 
//  173 /*===========================================================================*/
//  174 /* Driver interrupt handlers.                                                */
//  175 /*===========================================================================*/
//  176 
//  177 /*===========================================================================*/
//  178 /* Driver exported functions.                                                */
//  179 /*===========================================================================*/
//  180 
//  181 /**
//  182  * @brief   Low level SPI driver initialization.
//  183  *
//  184  * @notapi
//  185  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function spi_lld_init
        THUMB
//  186 void spi_lld_init(void) {
spi_lld_init:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  187 
//  188   dummytx = 0xFFFF;
        LDR.W    R0,??DataTable5
        MOVW     R1,#+65535
        STRH     R1,[R0, #+0]
//  189 
//  190 #if STM32_SPI_USE_SPI1
//  191   spiObjectInit(&SPID1);
        LDR.W    R0,??DataTable5_1
          CFI FunCall spiObjectInit
        BL       spiObjectInit
//  192   SPID1.spi       = SPI1;
        LDR.W    R0,??DataTable5_1
        LDR.W    R1,??DataTable5_2  ;; 0x40013000
        STR      R1,[R0, #+28]
//  193   SPID1.dmarx     = STM32_DMA_STREAM(STM32_SPI_SPI1_RX_DMA_STREAM);
        LDR.W    R0,??DataTable5_1
        LDR.W    R1,??DataTable5_3
        STR      R1,[R0, #+32]
//  194   SPID1.dmatx     = STM32_DMA_STREAM(STM32_SPI_SPI1_TX_DMA_STREAM);
        LDR.W    R0,??DataTable5_1
        LDR.W    R1,??DataTable5_4
        STR      R1,[R0, #+36]
//  195   SPID1.rxdmamode = STM32_DMA_CR_CHSEL(SPI1_RX_DMA_CHANNEL) |
//  196                     STM32_DMA_CR_PL(STM32_SPI_SPI1_DMA_PRIORITY) |
//  197                     STM32_DMA_CR_DIR_P2M |
//  198                     STM32_DMA_CR_TCIE |
//  199                     STM32_DMA_CR_DMEIE |
//  200                     STM32_DMA_CR_TEIE;
        LDR.W    R0,??DataTable5_1
        LDR.W    R1,??DataTable5_5  ;; 0x6010016
        STR      R1,[R0, #+40]
//  201   SPID1.txdmamode = STM32_DMA_CR_CHSEL(SPI1_TX_DMA_CHANNEL) |
//  202                     STM32_DMA_CR_PL(STM32_SPI_SPI1_DMA_PRIORITY) |
//  203                     STM32_DMA_CR_DIR_M2P |
//  204                     STM32_DMA_CR_DMEIE |
//  205                     STM32_DMA_CR_TEIE;
        LDR.W    R0,??DataTable5_1
        LDR.W    R1,??DataTable5_6  ;; 0x6010046
        STR      R1,[R0, #+44]
//  206 #endif
//  207 
//  208 #if STM32_SPI_USE_SPI2
//  209   spiObjectInit(&SPID2);
        LDR.W    R0,??DataTable5_7
          CFI FunCall spiObjectInit
        BL       spiObjectInit
//  210   SPID2.spi       = SPI2;
        LDR.W    R0,??DataTable5_7
        LDR.W    R1,??DataTable5_8  ;; 0x40003800
        STR      R1,[R0, #+28]
//  211   SPID2.dmarx     = STM32_DMA_STREAM(STM32_SPI_SPI2_RX_DMA_STREAM);
        LDR.W    R0,??DataTable5_7
        LDR.W    R1,??DataTable5_9
        STR      R1,[R0, #+32]
//  212   SPID2.dmatx     = STM32_DMA_STREAM(STM32_SPI_SPI2_TX_DMA_STREAM);
        LDR.W    R0,??DataTable5_7
        LDR.W    R1,??DataTable5_10
        STR      R1,[R0, #+36]
//  213   SPID2.rxdmamode = STM32_DMA_CR_CHSEL(SPI2_RX_DMA_CHANNEL) |
//  214                     STM32_DMA_CR_PL(STM32_SPI_SPI2_DMA_PRIORITY) |
//  215                     STM32_DMA_CR_DIR_P2M |
//  216                     STM32_DMA_CR_TCIE |
//  217                     STM32_DMA_CR_DMEIE |
//  218                     STM32_DMA_CR_TEIE;
        LDR.N    R0,??DataTable5_7
        LDR.W    R1,??DataTable5_11  ;; 0x10016
        STR      R1,[R0, #+40]
//  219   SPID2.txdmamode = STM32_DMA_CR_CHSEL(SPI2_TX_DMA_CHANNEL) |
//  220                     STM32_DMA_CR_PL(STM32_SPI_SPI2_DMA_PRIORITY) |
//  221                     STM32_DMA_CR_DIR_M2P |
//  222                     STM32_DMA_CR_DMEIE |
//  223                     STM32_DMA_CR_TEIE;
        LDR.N    R0,??DataTable5_7
        LDR.N    R1,??DataTable5_12  ;; 0x10046
        STR      R1,[R0, #+44]
//  224 #endif
//  225 
//  226 #if STM32_SPI_USE_SPI3
//  227   spiObjectInit(&SPID3);
        LDR.N    R0,??DataTable5_13
          CFI FunCall spiObjectInit
        BL       spiObjectInit
//  228   SPID3.spi       = SPI3;
        LDR.N    R0,??DataTable5_13
        LDR.N    R1,??DataTable5_14  ;; 0x40003c00
        STR      R1,[R0, #+28]
//  229   SPID3.dmarx     = STM32_DMA_STREAM(STM32_SPI_SPI3_RX_DMA_STREAM);
        LDR.N    R0,??DataTable5_13
        LDR.N    R1,??DataTable5_15
        STR      R1,[R0, #+32]
//  230   SPID3.dmatx     = STM32_DMA_STREAM(STM32_SPI_SPI3_TX_DMA_STREAM);
        LDR.N    R0,??DataTable5_13
        LDR.N    R1,??DataTable5_16
        STR      R1,[R0, #+36]
//  231   SPID3.rxdmamode = STM32_DMA_CR_CHSEL(SPI3_RX_DMA_CHANNEL) |
//  232                     STM32_DMA_CR_PL(STM32_SPI_SPI3_DMA_PRIORITY) |
//  233                     STM32_DMA_CR_DIR_P2M |
//  234                     STM32_DMA_CR_TCIE |
//  235                     STM32_DMA_CR_DMEIE |
//  236                     STM32_DMA_CR_TEIE;
        LDR.N    R0,??DataTable5_13
        LDR.N    R1,??DataTable5_11  ;; 0x10016
        STR      R1,[R0, #+40]
//  237   SPID3.txdmamode = STM32_DMA_CR_CHSEL(SPI3_TX_DMA_CHANNEL) |
//  238                     STM32_DMA_CR_PL(STM32_SPI_SPI3_DMA_PRIORITY) |
//  239                     STM32_DMA_CR_DIR_M2P |
//  240                     STM32_DMA_CR_DMEIE |
//  241                     STM32_DMA_CR_TEIE;
        LDR.N    R0,??DataTable5_13
        LDR.N    R1,??DataTable5_12  ;; 0x10046
        STR      R1,[R0, #+44]
//  242 #endif
//  243 
//  244 #if STM32_SPI_USE_SPI4
//  245   spiObjectInit(&SPID4);
//  246   SPID4.spi       = SPI4;
//  247   SPID4.dmarx     = STM32_DMA_STREAM(STM32_SPI_SPI4_RX_DMA_STREAM);
//  248   SPID4.dmatx     = STM32_DMA_STREAM(STM32_SPI_SPI4_TX_DMA_STREAM);
//  249   SPID4.rxdmamode = STM32_DMA_CR_CHSEL(SPI4_RX_DMA_CHANNEL) |
//  250                     STM32_DMA_CR_PL(STM32_SPI_SPI4_DMA_PRIORITY) |
//  251                     STM32_DMA_CR_DIR_P2M |
//  252                     STM32_DMA_CR_TCIE |
//  253                     STM32_DMA_CR_DMEIE |
//  254                     STM32_DMA_CR_TEIE;
//  255   SPID4.txdmamode = STM32_DMA_CR_CHSEL(SPI4_TX_DMA_CHANNEL) |
//  256                     STM32_DMA_CR_PL(STM32_SPI_SPI4_DMA_PRIORITY) |
//  257                     STM32_DMA_CR_DIR_M2P |
//  258                     STM32_DMA_CR_DMEIE |
//  259                     STM32_DMA_CR_TEIE;
//  260 #endif
//  261 
//  262 #if STM32_SPI_USE_SPI5
//  263   spiObjectInit(&SPID5);
//  264   SPID5.spi       = SPI5;
//  265   SPID5.dmarx     = STM32_DMA_STREAM(STM32_SPI_SPI5_RX_DMA_STREAM);
//  266   SPID5.dmatx     = STM32_DMA_STREAM(STM32_SPI_SPI5_TX_DMA_STREAM);
//  267   SPID5.rxdmamode = STM32_DMA_CR_CHSEL(SPI5_RX_DMA_CHANNEL) |
//  268                     STM32_DMA_CR_PL(STM32_SPI_SPI5_DMA_PRIORITY) |
//  269                     STM32_DMA_CR_DIR_P2M |
//  270                     STM32_DMA_CR_TCIE |
//  271                     STM32_DMA_CR_DMEIE |
//  272                     STM32_DMA_CR_TEIE;
//  273   SPID5.txdmamode = STM32_DMA_CR_CHSEL(SPI5_TX_DMA_CHANNEL) |
//  274                     STM32_DMA_CR_PL(STM32_SPI_SPI5_DMA_PRIORITY) |
//  275                     STM32_DMA_CR_DIR_M2P |
//  276                     STM32_DMA_CR_DMEIE |
//  277                     STM32_DMA_CR_TEIE;
//  278 #endif
//  279 
//  280 #if STM32_SPI_USE_SPI6
//  281   spiObjectInit(&SPID6);
//  282   SPID6.spi       = SPI6;
//  283   SPID6.dmarx     = STM32_DMA_STREAM(STM32_SPI_SPI6_RX_DMA_STREAM);
//  284   SPID6.dmatx     = STM32_DMA_STREAM(STM32_SPI_SPI6_TX_DMA_STREAM);
//  285   SPID6.rxdmamode = STM32_DMA_CR_CHSEL(SPI6_RX_DMA_CHANNEL) |
//  286                     STM32_DMA_CR_PL(STM32_SPI_SPI6_DMA_PRIORITY) |
//  287                     STM32_DMA_CR_DIR_P2M |
//  288                     STM32_DMA_CR_TCIE |
//  289                     STM32_DMA_CR_DMEIE |
//  290                     STM32_DMA_CR_TEIE;
//  291   SPID6.txdmamode = STM32_DMA_CR_CHSEL(SPI6_TX_DMA_CHANNEL) |
//  292                     STM32_DMA_CR_PL(STM32_SPI_SPI6_DMA_PRIORITY) |
//  293                     STM32_DMA_CR_DIR_M2P |
//  294                     STM32_DMA_CR_DMEIE |
//  295                     STM32_DMA_CR_TEIE;
//  296 #endif
//  297 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock2
//  298 
//  299 /**
//  300  * @brief   Configures and activates the SPI peripheral.
//  301  *
//  302  * @param[in] spip      pointer to the @p SPIDriver object
//  303  *
//  304  * @notapi
//  305  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function spi_lld_start
        THUMB
//  306 void spi_lld_start(SPIDriver *spip) {
spi_lld_start:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  307 
//  308   /* If in stopped state then enables the SPI and DMA clocks.*/
//  309   if (spip->state == SPI_STOP) {
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BNE.N    ??spi_lld_start_0
//  310 #if STM32_SPI_USE_SPI1
//  311     if (&SPID1 == spip) {
        LDR.N    R0,??DataTable5_1
        CMP      R0,R4
        BNE.N    ??spi_lld_start_1
//  312       bool_t b;
//  313       b = dmaStreamAllocate(spip->dmarx,
//  314                             STM32_SPI_SPI1_IRQ_PRIORITY,
//  315                             (stm32_dmaisr_t)spi_lld_serve_rx_interrupt,
//  316                             (void *)spip);
        MOVS     R3,R4
        LDR.N    R2,??DataTable5_17
        MOVS     R1,#+10
        LDR      R0,[R4, #+32]
          CFI FunCall dmaStreamAllocate
        BL       dmaStreamAllocate
        MOVS     R5,R0
//  317       chDbgAssert(!b, "spi_lld_start(), #1", "stream already allocated");
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BEQ.N    ??spi_lld_start_2
        LDR.N    R0,??DataTable5_18
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  318       b = dmaStreamAllocate(spip->dmatx,
//  319                             STM32_SPI_SPI1_IRQ_PRIORITY,
//  320                             (stm32_dmaisr_t)spi_lld_serve_tx_interrupt,
//  321                             (void *)spip);
??spi_lld_start_2:
        MOVS     R3,R4
        LDR.N    R2,??DataTable5_19
        MOVS     R1,#+10
        LDR      R0,[R4, #+36]
          CFI FunCall dmaStreamAllocate
        BL       dmaStreamAllocate
        MOVS     R5,R0
//  322       chDbgAssert(!b, "spi_lld_start(), #2", "stream already allocated");
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BEQ.N    ??spi_lld_start_3
        LDR.N    R0,??DataTable5_20
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  323       rccEnableSPI1(FALSE);
??spi_lld_start_3:
        LDR.N    R0,??DataTable5_21  ;; 0x40023844
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x1000
        LDR.N    R1,??DataTable5_21  ;; 0x40023844
        STR      R0,[R1, #+0]
//  324     }
//  325 #endif
//  326 #if STM32_SPI_USE_SPI2
//  327     if (&SPID2 == spip) {
??spi_lld_start_1:
        LDR.N    R0,??DataTable5_7
        CMP      R0,R4
        BNE.N    ??spi_lld_start_4
//  328       bool_t b;
//  329       b = dmaStreamAllocate(spip->dmarx,
//  330                             STM32_SPI_SPI2_IRQ_PRIORITY,
//  331                             (stm32_dmaisr_t)spi_lld_serve_rx_interrupt,
//  332                             (void *)spip);
        MOVS     R3,R4
        LDR.N    R2,??DataTable5_17
        MOVS     R1,#+10
        LDR      R0,[R4, #+32]
          CFI FunCall dmaStreamAllocate
        BL       dmaStreamAllocate
        MOVS     R5,R0
//  333       chDbgAssert(!b, "spi_lld_start(), #3", "stream already allocated");
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BEQ.N    ??spi_lld_start_5
        LDR.N    R0,??DataTable5_22
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  334       b = dmaStreamAllocate(spip->dmatx,
//  335                             STM32_SPI_SPI2_IRQ_PRIORITY,
//  336                             (stm32_dmaisr_t)spi_lld_serve_tx_interrupt,
//  337                             (void *)spip);
??spi_lld_start_5:
        MOVS     R3,R4
        LDR.N    R2,??DataTable5_19
        MOVS     R1,#+10
        LDR      R0,[R4, #+36]
          CFI FunCall dmaStreamAllocate
        BL       dmaStreamAllocate
        MOVS     R5,R0
//  338       chDbgAssert(!b, "spi_lld_start(), #4", "stream already allocated");
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BEQ.N    ??spi_lld_start_6
        LDR.N    R0,??DataTable5_23
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  339       rccEnableSPI2(FALSE);
??spi_lld_start_6:
        LDR.N    R0,??DataTable5_24  ;; 0x40023840
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x4000
        LDR.N    R1,??DataTable5_24  ;; 0x40023840
        STR      R0,[R1, #+0]
//  340     }
//  341 #endif
//  342 #if STM32_SPI_USE_SPI3
//  343     if (&SPID3 == spip) {
??spi_lld_start_4:
        LDR.N    R0,??DataTable5_13
        CMP      R0,R4
        BNE.N    ??spi_lld_start_7
//  344       bool_t b;
//  345       b = dmaStreamAllocate(spip->dmarx,
//  346                             STM32_SPI_SPI3_IRQ_PRIORITY,
//  347                             (stm32_dmaisr_t)spi_lld_serve_rx_interrupt,
//  348                             (void *)spip);
        MOVS     R3,R4
        LDR.N    R2,??DataTable5_17
        MOVS     R1,#+10
        LDR      R0,[R4, #+32]
          CFI FunCall dmaStreamAllocate
        BL       dmaStreamAllocate
        MOVS     R5,R0
//  349       chDbgAssert(!b, "spi_lld_start(), #5", "stream already allocated");
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BEQ.N    ??spi_lld_start_8
        LDR.N    R0,??DataTable5_25
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  350       b = dmaStreamAllocate(spip->dmatx,
//  351                             STM32_SPI_SPI3_IRQ_PRIORITY,
//  352                             (stm32_dmaisr_t)spi_lld_serve_tx_interrupt,
//  353                             (void *)spip);
??spi_lld_start_8:
        MOVS     R3,R4
        LDR.N    R2,??DataTable5_19
        MOVS     R1,#+10
        LDR      R0,[R4, #+36]
          CFI FunCall dmaStreamAllocate
        BL       dmaStreamAllocate
        MOVS     R5,R0
//  354       chDbgAssert(!b, "spi_lld_start(), #6", "stream already allocated");
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BEQ.N    ??spi_lld_start_9
        LDR.N    R0,??DataTable5_26
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  355       rccEnableSPI3(FALSE);
??spi_lld_start_9:
        LDR.N    R0,??DataTable5_24  ;; 0x40023840
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x8000
        LDR.N    R1,??DataTable5_24  ;; 0x40023840
        STR      R0,[R1, #+0]
//  356     }
//  357 #endif
//  358 #if STM32_SPI_USE_SPI4
//  359     if (&SPID4 == spip) {
//  360       bool_t b;
//  361       b = dmaStreamAllocate(spip->dmarx,
//  362                             STM32_SPI_SPI4_IRQ_PRIORITY,
//  363                             (stm32_dmaisr_t)spi_lld_serve_rx_interrupt,
//  364                             (void *)spip);
//  365       chDbgAssert(!b, "spi_lld_start(), #7", "stream already allocated");
//  366       b = dmaStreamAllocate(spip->dmatx,
//  367                             STM32_SPI_SPI4_IRQ_PRIORITY,
//  368                             (stm32_dmaisr_t)spi_lld_serve_tx_interrupt,
//  369                             (void *)spip);
//  370       chDbgAssert(!b, "spi_lld_start(), #8", "stream already allocated");
//  371       rccEnableSPI4(FALSE);
//  372     }
//  373 #endif
//  374 #if STM32_SPI_USE_SPI5
//  375     if (&SPID5 == spip) {
//  376       bool_t b;
//  377       b = dmaStreamAllocate(spip->dmarx,
//  378                             STM32_SPI_SPI5_IRQ_PRIORITY,
//  379                             (stm32_dmaisr_t)spi_lld_serve_rx_interrupt,
//  380                             (void *)spip);
//  381       chDbgAssert(!b, "spi_lld_start(), #9", "stream already allocated");
//  382       b = dmaStreamAllocate(spip->dmatx,
//  383                             STM32_SPI_SPI5_IRQ_PRIORITY,
//  384                             (stm32_dmaisr_t)spi_lld_serve_tx_interrupt,
//  385                             (void *)spip);
//  386       chDbgAssert(!b, "spi_lld_start(), #10", "stream already allocated");
//  387       rccEnableSPI5(FALSE);
//  388     }
//  389 #endif
//  390 #if STM32_SPI_USE_SPI6
//  391     if (&SPID6 == spip) {
//  392       bool_t b;
//  393       b = dmaStreamAllocate(spip->dmarx,
//  394                             STM32_SPI_SPI6_IRQ_PRIORITY,
//  395                             (stm32_dmaisr_t)spi_lld_serve_rx_interrupt,
//  396                             (void *)spip);
//  397       chDbgAssert(!b, "spi_lld_start(), #11", "stream already allocated");
//  398       b = dmaStreamAllocate(spip->dmatx,
//  399                             STM32_SPI_SPI6_IRQ_PRIORITY,
//  400                             (stm32_dmaisr_t)spi_lld_serve_tx_interrupt,
//  401                             (void *)spip);
//  402       chDbgAssert(!b, "spi_lld_start(), #12", "stream already allocated");
//  403       rccEnableSPI6(FALSE);
//  404     }
//  405 #endif
//  406 
//  407     /* DMA setup.*/
//  408     dmaStreamSetPeripheral(spip->dmarx, &spip->spi->DR);
??spi_lld_start_7:
        LDR      R0,[R4, #+32]
        LDR      R0,[R0, #+0]
        LDR      R1,[R4, #+28]
        ADDS     R1,R1,#+12
        STR      R1,[R0, #+8]
//  409     dmaStreamSetPeripheral(spip->dmatx, &spip->spi->DR);
        LDR      R0,[R4, #+36]
        LDR      R0,[R0, #+0]
        LDR      R1,[R4, #+28]
        ADDS     R1,R1,#+12
        STR      R1,[R0, #+8]
//  410   }
//  411 
//  412   /* Configuration-specific DMA setup.*/
//  413   if ((spip->config->cr1 & SPI_CR1_DFF) == 0) {
??spi_lld_start_0:
        LDR      R0,[R4, #+4]
        LDRH     R0,[R0, #+10]
        LSLS     R0,R0,#+20
        BMI.N    ??spi_lld_start_10
//  414     /* Frame width is 8 bits or smaller.*/
//  415     spip->rxdmamode = (spip->rxdmamode & ~STM32_DMA_CR_SIZE_MASK) |
//  416                       STM32_DMA_CR_PSIZE_BYTE | STM32_DMA_CR_MSIZE_BYTE;
        LDR      R0,[R4, #+40]
        BICS     R0,R0,#0x7800
        STR      R0,[R4, #+40]
//  417     spip->txdmamode = (spip->txdmamode & ~STM32_DMA_CR_SIZE_MASK) |
//  418                       STM32_DMA_CR_PSIZE_BYTE | STM32_DMA_CR_MSIZE_BYTE;
        LDR      R0,[R4, #+44]
        BICS     R0,R0,#0x7800
        STR      R0,[R4, #+44]
        B.N      ??spi_lld_start_11
//  419   }
//  420   else {
//  421     /* Frame width is larger than 8 bits.*/
//  422     spip->rxdmamode = (spip->rxdmamode & ~STM32_DMA_CR_SIZE_MASK) |
//  423                       STM32_DMA_CR_PSIZE_HWORD | STM32_DMA_CR_MSIZE_HWORD;
??spi_lld_start_10:
        MOVS     R0,#+5
        LDR      R1,[R4, #+40]
        BFI      R1,R0,#+11,#+4
        STR      R1,[R4, #+40]
//  424     spip->txdmamode = (spip->txdmamode & ~STM32_DMA_CR_SIZE_MASK) |
//  425                       STM32_DMA_CR_PSIZE_HWORD | STM32_DMA_CR_MSIZE_HWORD;
        MOVS     R0,#+5
        LDR      R1,[R4, #+44]
        BFI      R1,R0,#+11,#+4
        STR      R1,[R4, #+44]
//  426   }
//  427   /* SPI setup and enable.*/
//  428   spip->spi->CR1  = 0;
??spi_lld_start_11:
        LDR      R0,[R4, #+28]
        MOVS     R1,#+0
        STRH     R1,[R0, #+0]
//  429   spip->spi->CR1  = spip->config->cr1 | SPI_CR1_MSTR | SPI_CR1_SSM |
//  430                     SPI_CR1_SSI;
        LDR      R0,[R4, #+4]
        LDRH     R0,[R0, #+10]
        ORRS     R0,R0,#0x304
        LDR      R1,[R4, #+28]
        STRH     R0,[R1, #+0]
//  431   spip->spi->CR2  = SPI_CR2_SSOE | SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN;
        LDR      R0,[R4, #+28]
        MOVS     R1,#+7
        STRH     R1,[R0, #+4]
//  432   spip->spi->CR1 |= SPI_CR1_SPE;
        LDR      R0,[R4, #+28]
        LDRH     R0,[R0, #+0]
        ORRS     R0,R0,#0x40
        LDR      R1,[R4, #+28]
        STRH     R0,[R1, #+0]
//  433 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock3
//  434 
//  435 /**
//  436  * @brief   Deactivates the SPI peripheral.
//  437  *
//  438  * @param[in] spip      pointer to the @p SPIDriver object
//  439  *
//  440  * @notapi
//  441  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function spi_lld_stop
        THUMB
//  442 void spi_lld_stop(SPIDriver *spip) {
spi_lld_stop:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  443 
//  444   /* If in ready state then disables the SPI clock.*/
//  445   if (spip->state == SPI_READY) {
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BNE.N    ??spi_lld_stop_0
//  446 
//  447     /* SPI disable.*/
//  448     spip->spi->CR1 = 0;
        LDR      R0,[R4, #+28]
        MOVS     R1,#+0
        STRH     R1,[R0, #+0]
//  449     spip->spi->CR2 = 0;
        LDR      R0,[R4, #+28]
        MOVS     R1,#+0
        STRH     R1,[R0, #+4]
//  450     dmaStreamRelease(spip->dmarx);
        LDR      R0,[R4, #+32]
          CFI FunCall dmaStreamRelease
        BL       dmaStreamRelease
//  451     dmaStreamRelease(spip->dmatx);
        LDR      R0,[R4, #+36]
          CFI FunCall dmaStreamRelease
        BL       dmaStreamRelease
//  452 
//  453 #if STM32_SPI_USE_SPI1
//  454     if (&SPID1 == spip)
        LDR.N    R0,??DataTable5_1
        CMP      R0,R4
        BNE.N    ??spi_lld_stop_1
//  455       rccDisableSPI1(FALSE);
        LDR.N    R0,??DataTable5_21  ;; 0x40023844
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x1000
        LDR.N    R1,??DataTable5_21  ;; 0x40023844
        STR      R0,[R1, #+0]
//  456 #endif
//  457 #if STM32_SPI_USE_SPI2
//  458     if (&SPID2 == spip)
??spi_lld_stop_1:
        LDR.N    R0,??DataTable5_7
        CMP      R0,R4
        BNE.N    ??spi_lld_stop_2
//  459       rccDisableSPI2(FALSE);
        LDR.N    R0,??DataTable5_24  ;; 0x40023840
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x4000
        LDR.N    R1,??DataTable5_24  ;; 0x40023840
        STR      R0,[R1, #+0]
//  460 #endif
//  461 #if STM32_SPI_USE_SPI3
//  462     if (&SPID3 == spip)
??spi_lld_stop_2:
        LDR.N    R0,??DataTable5_13
        CMP      R0,R4
        BNE.N    ??spi_lld_stop_0
//  463       rccDisableSPI3(FALSE);
        LDR.N    R0,??DataTable5_24  ;; 0x40023840
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x8000
        LDR.N    R1,??DataTable5_24  ;; 0x40023840
        STR      R0,[R1, #+0]
//  464 #endif
//  465 #if STM32_SPI_USE_SPI4
//  466     if (&SPID4 == spip)
//  467       rccDisableSPI4(FALSE);
//  468 #endif
//  469 #if STM32_SPI_USE_SPI5
//  470     if (&SPID5 == spip)
//  471       rccDisableSPI5(FALSE);
//  472 #endif
//  473 #if STM32_SPI_USE_SPI6
//  474     if (&SPID6 == spip)
//  475       rccDisableSPI6(FALSE);
//  476 #endif
//  477   }
//  478 }
??spi_lld_stop_0:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock4
//  479 
//  480 /**
//  481  * @brief   Asserts the slave select signal and prepares for transfers.
//  482  *
//  483  * @param[in] spip      pointer to the @p SPIDriver object
//  484  *
//  485  * @notapi
//  486  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function spi_lld_select
          CFI NoCalls
        THUMB
//  487 void spi_lld_select(SPIDriver *spip) {
//  488 
//  489   palClearPad(spip->config->ssport, spip->config->sspad);
spi_lld_select:
        MOVS     R1,#+1
        LDR      R2,[R0, #+4]
        LDRH     R2,[R2, #+8]
        LSLS     R1,R1,R2
        LDR      R2,[R0, #+4]
        LDR      R2,[R2, #+4]
        STRH     R1,[R2, #+26]
//  490 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock5
//  491 
//  492 /**
//  493  * @brief   Deasserts the slave select signal.
//  494  * @details The previously selected peripheral is unselected.
//  495  *
//  496  * @param[in] spip      pointer to the @p SPIDriver object
//  497  *
//  498  * @notapi
//  499  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function spi_lld_unselect
          CFI NoCalls
        THUMB
//  500 void spi_lld_unselect(SPIDriver *spip) {
//  501 
//  502   palSetPad(spip->config->ssport, spip->config->sspad);
spi_lld_unselect:
        MOVS     R1,#+1
        LDR      R2,[R0, #+4]
        LDRH     R2,[R2, #+8]
        LSLS     R1,R1,R2
        LDR      R2,[R0, #+4]
        LDR      R2,[R2, #+4]
        STRH     R1,[R2, #+24]
//  503 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock6
//  504 
//  505 /**
//  506  * @brief   Ignores data on the SPI bus.
//  507  * @details This asynchronous function starts the transmission of a series of
//  508  *          idle words on the SPI bus and ignores the received data.
//  509  * @post    At the end of the operation the configured callback is invoked.
//  510  *
//  511  * @param[in] spip      pointer to the @p SPIDriver object
//  512  * @param[in] n         number of words to be ignored
//  513  *
//  514  * @notapi
//  515  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function spi_lld_ignore
          CFI NoCalls
        THUMB
//  516 void spi_lld_ignore(SPIDriver *spip, size_t n) {
//  517 
//  518   dmaStreamSetMemory0(spip->dmarx, &dummyrx);
spi_lld_ignore:
        LDR      R2,[R0, #+32]
        LDR      R2,[R2, #+0]
        LDR.N    R3,??DataTable5_27
        STR      R3,[R2, #+12]
//  519   dmaStreamSetTransactionSize(spip->dmarx, n);
        LDR      R2,[R0, #+32]
        LDR      R2,[R2, #+0]
        STR      R1,[R2, #+4]
//  520   dmaStreamSetMode(spip->dmarx, spip->rxdmamode);
        LDR      R2,[R0, #+32]
        LDR      R2,[R2, #+0]
        LDR      R3,[R0, #+40]
        STR      R3,[R2, #+0]
//  521 
//  522   dmaStreamSetMemory0(spip->dmatx, &dummytx);
        LDR      R2,[R0, #+36]
        LDR      R2,[R2, #+0]
        LDR.N    R3,??DataTable5
        STR      R3,[R2, #+12]
//  523   dmaStreamSetTransactionSize(spip->dmatx, n);
        LDR      R2,[R0, #+36]
        LDR      R2,[R2, #+0]
        STR      R1,[R2, #+4]
//  524   dmaStreamSetMode(spip->dmatx, spip->txdmamode);
        LDR      R2,[R0, #+36]
        LDR      R2,[R2, #+0]
        LDR      R3,[R0, #+44]
        STR      R3,[R2, #+0]
//  525 
//  526   dmaStreamEnable(spip->dmarx);
        LDR      R2,[R0, #+32]
        LDR      R2,[R2, #+0]
        LDR      R2,[R2, #+0]
        ORRS     R2,R2,#0x1
        LDR      R3,[R0, #+32]
        LDR      R3,[R3, #+0]
        STR      R2,[R3, #+0]
//  527   dmaStreamEnable(spip->dmatx);
        LDR      R2,[R0, #+36]
        LDR      R2,[R2, #+0]
        LDR      R2,[R2, #+0]
        ORRS     R2,R2,#0x1
        LDR      R3,[R0, #+36]
        LDR      R3,[R3, #+0]
        STR      R2,[R3, #+0]
//  528 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock7
//  529 
//  530 /**
//  531  * @brief   Exchanges data on the SPI bus.
//  532  * @details This asynchronous function starts a simultaneous transmit/receive
//  533  *          operation.
//  534  * @post    At the end of the operation the configured callback is invoked.
//  535  * @note    The buffers are organized as uint8_t arrays for data sizes below or
//  536  *          equal to 8 bits else it is organized as uint16_t arrays.
//  537  *
//  538  * @param[in] spip      pointer to the @p SPIDriver object
//  539  * @param[in] n         number of words to be exchanged
//  540  * @param[in] txbuf     the pointer to the transmit buffer
//  541  * @param[out] rxbuf    the pointer to the receive buffer
//  542  *
//  543  * @notapi
//  544  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function spi_lld_exchange
          CFI NoCalls
        THUMB
//  545 void spi_lld_exchange(SPIDriver *spip, size_t n,
//  546                       const void *txbuf, void *rxbuf) {
spi_lld_exchange:
        PUSH     {R4,R5}
          CFI R5 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  547 
//  548   dmaStreamSetMemory0(spip->dmarx, rxbuf);
        LDR      R4,[R0, #+32]
        LDR      R4,[R4, #+0]
        STR      R3,[R4, #+12]
//  549   dmaStreamSetTransactionSize(spip->dmarx, n);
        LDR      R4,[R0, #+32]
        LDR      R4,[R4, #+0]
        STR      R1,[R4, #+4]
//  550   dmaStreamSetMode(spip->dmarx, spip->rxdmamode| STM32_DMA_CR_MINC);
        LDR      R4,[R0, #+40]
        ORRS     R4,R4,#0x400
        LDR      R5,[R0, #+32]
        LDR      R5,[R5, #+0]
        STR      R4,[R5, #+0]
//  551 
//  552   dmaStreamSetMemory0(spip->dmatx, txbuf);
        LDR      R4,[R0, #+36]
        LDR      R4,[R4, #+0]
        STR      R2,[R4, #+12]
//  553   dmaStreamSetTransactionSize(spip->dmatx, n);
        LDR      R4,[R0, #+36]
        LDR      R4,[R4, #+0]
        STR      R1,[R4, #+4]
//  554   dmaStreamSetMode(spip->dmatx, spip->txdmamode | STM32_DMA_CR_MINC);
        LDR      R4,[R0, #+44]
        ORRS     R4,R4,#0x400
        LDR      R5,[R0, #+36]
        LDR      R5,[R5, #+0]
        STR      R4,[R5, #+0]
//  555 
//  556   dmaStreamEnable(spip->dmarx);
        LDR      R4,[R0, #+32]
        LDR      R4,[R4, #+0]
        LDR      R4,[R4, #+0]
        ORRS     R4,R4,#0x1
        LDR      R5,[R0, #+32]
        LDR      R5,[R5, #+0]
        STR      R4,[R5, #+0]
//  557   dmaStreamEnable(spip->dmatx);
        LDR      R4,[R0, #+36]
        LDR      R4,[R4, #+0]
        LDR      R4,[R4, #+0]
        ORRS     R4,R4,#0x1
        LDR      R5,[R0, #+36]
        LDR      R5,[R5, #+0]
        STR      R4,[R5, #+0]
//  558 }
        POP      {R4,R5}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock8
//  559 
//  560 /**
//  561  * @brief   Sends data over the SPI bus.
//  562  * @details This asynchronous function starts a transmit operation.
//  563  * @post    At the end of the operation the configured callback is invoked.
//  564  * @note    The buffers are organized as uint8_t arrays for data sizes below or
//  565  *          equal to 8 bits else it is organized as uint16_t arrays.
//  566  *
//  567  * @param[in] spip      pointer to the @p SPIDriver object
//  568  * @param[in] n         number of words to send
//  569  * @param[in] txbuf     the pointer to the transmit buffer
//  570  *
//  571  * @notapi
//  572  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function spi_lld_send
          CFI NoCalls
        THUMB
//  573 void spi_lld_send(SPIDriver *spip, size_t n, const void *txbuf) {
spi_lld_send:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
//  574 
//  575   dmaStreamSetMemory0(spip->dmarx, &dummyrx);
        LDR      R3,[R0, #+32]
        LDR      R3,[R3, #+0]
        LDR.N    R4,??DataTable5_27
        STR      R4,[R3, #+12]
//  576   dmaStreamSetTransactionSize(spip->dmarx, n);
        LDR      R3,[R0, #+32]
        LDR      R3,[R3, #+0]
        STR      R1,[R3, #+4]
//  577   dmaStreamSetMode(spip->dmarx, spip->rxdmamode);
        LDR      R3,[R0, #+32]
        LDR      R3,[R3, #+0]
        LDR      R4,[R0, #+40]
        STR      R4,[R3, #+0]
//  578 
//  579   dmaStreamSetMemory0(spip->dmatx, txbuf);
        LDR      R3,[R0, #+36]
        LDR      R3,[R3, #+0]
        STR      R2,[R3, #+12]
//  580   dmaStreamSetTransactionSize(spip->dmatx, n);
        LDR      R3,[R0, #+36]
        LDR      R3,[R3, #+0]
        STR      R1,[R3, #+4]
//  581   dmaStreamSetMode(spip->dmatx, spip->txdmamode | STM32_DMA_CR_MINC);
        LDR      R3,[R0, #+44]
        ORRS     R3,R3,#0x400
        LDR      R4,[R0, #+36]
        LDR      R4,[R4, #+0]
        STR      R3,[R4, #+0]
//  582 
//  583   dmaStreamEnable(spip->dmarx);
        LDR      R3,[R0, #+32]
        LDR      R3,[R3, #+0]
        LDR      R3,[R3, #+0]
        ORRS     R3,R3,#0x1
        LDR      R4,[R0, #+32]
        LDR      R4,[R4, #+0]
        STR      R3,[R4, #+0]
//  584   dmaStreamEnable(spip->dmatx);
        LDR      R3,[R0, #+36]
        LDR      R3,[R3, #+0]
        LDR      R3,[R3, #+0]
        ORRS     R3,R3,#0x1
        LDR      R4,[R0, #+36]
        LDR      R4,[R4, #+0]
        STR      R3,[R4, #+0]
//  585 }
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock9
//  586 
//  587 /**
//  588  * @brief   Receives data from the SPI bus.
//  589  * @details This asynchronous function starts a receive operation.
//  590  * @post    At the end of the operation the configured callback is invoked.
//  591  * @note    The buffers are organized as uint8_t arrays for data sizes below or
//  592  *          equal to 8 bits else it is organized as uint16_t arrays.
//  593  *
//  594  * @param[in] spip      pointer to the @p SPIDriver object
//  595  * @param[in] n         number of words to receive
//  596  * @param[out] rxbuf    the pointer to the receive buffer
//  597  *
//  598  * @notapi
//  599  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function spi_lld_receive
          CFI NoCalls
        THUMB
//  600 void spi_lld_receive(SPIDriver *spip, size_t n, void *rxbuf) {
spi_lld_receive:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
//  601 
//  602   dmaStreamSetMemory0(spip->dmarx, rxbuf);
        LDR      R3,[R0, #+32]
        LDR      R3,[R3, #+0]
        STR      R2,[R3, #+12]
//  603   dmaStreamSetTransactionSize(spip->dmarx, n);
        LDR      R3,[R0, #+32]
        LDR      R3,[R3, #+0]
        STR      R1,[R3, #+4]
//  604   dmaStreamSetMode(spip->dmarx, spip->rxdmamode | STM32_DMA_CR_MINC);
        LDR      R3,[R0, #+40]
        ORRS     R3,R3,#0x400
        LDR      R4,[R0, #+32]
        LDR      R4,[R4, #+0]
        STR      R3,[R4, #+0]
//  605 
//  606   dmaStreamSetMemory0(spip->dmatx, &dummytx);
        LDR      R3,[R0, #+36]
        LDR      R3,[R3, #+0]
        LDR.N    R4,??DataTable5
        STR      R4,[R3, #+12]
//  607   dmaStreamSetTransactionSize(spip->dmatx, n);
        LDR      R3,[R0, #+36]
        LDR      R3,[R3, #+0]
        STR      R1,[R3, #+4]
//  608   dmaStreamSetMode(spip->dmatx, spip->txdmamode);
        LDR      R3,[R0, #+36]
        LDR      R3,[R3, #+0]
        LDR      R4,[R0, #+44]
        STR      R4,[R3, #+0]
//  609 
//  610   dmaStreamEnable(spip->dmarx);
        LDR      R3,[R0, #+32]
        LDR      R3,[R3, #+0]
        LDR      R3,[R3, #+0]
        ORRS     R3,R3,#0x1
        LDR      R4,[R0, #+32]
        LDR      R4,[R4, #+0]
        STR      R3,[R4, #+0]
//  611   dmaStreamEnable(spip->dmatx);
        LDR      R3,[R0, #+36]
        LDR      R3,[R3, #+0]
        LDR      R3,[R3, #+0]
        ORRS     R3,R3,#0x1
        LDR      R4,[R0, #+36]
        LDR      R4,[R4, #+0]
        STR      R3,[R4, #+0]
//  612 }
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock10

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5:
        DC32     dummytx

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_1:
        DC32     SPID1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_2:
        DC32     0x40013000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_3:
        DC32     _stm32_dma_streams+0x60

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_4:
        DC32     _stm32_dma_streams+0x84

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_5:
        DC32     0x6010016

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_6:
        DC32     0x6010046

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_7:
        DC32     SPID2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_8:
        DC32     0x40003800

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_9:
        DC32     _stm32_dma_streams+0x24

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_10:
        DC32     _stm32_dma_streams+0x30

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_11:
        DC32     0x10016

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_12:
        DC32     0x10046

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_13:
        DC32     SPID3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_14:
        DC32     0x40003c00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_15:
        DC32     _stm32_dma_streams

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_16:
        DC32     _stm32_dma_streams+0x54

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_17:
        DC32     spi_lld_serve_rx_interrupt

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_18:
        DC32     `?<Constant "spi_lld_start(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_19:
        DC32     spi_lld_serve_tx_interrupt

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_20:
        DC32     `?<Constant "spi_lld_start(), #2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_21:
        DC32     0x40023844

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_22:
        DC32     `?<Constant "spi_lld_start(), #3">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_23:
        DC32     `?<Constant "spi_lld_start(), #4">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_24:
        DC32     0x40023840

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_25:
        DC32     `?<Constant "spi_lld_start(), #5">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_26:
        DC32     `?<Constant "spi_lld_start(), #6">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_27:
        DC32     dummyrx
//  613 
//  614 /**
//  615  * @brief   Exchanges one frame using a polled wait.
//  616  * @details This synchronous function exchanges one frame using a polled
//  617  *          synchronization method. This function is useful when exchanging
//  618  *          small amount of data on high speed channels, usually in this
//  619  *          situation is much more efficient just wait for completion using
//  620  *          polling than suspending the thread waiting for an interrupt.
//  621  *
//  622  * @param[in] spip      pointer to the @p SPIDriver object
//  623  * @param[in] frame     the data frame to send over the SPI bus
//  624  * @return              The received data frame from the SPI bus.
//  625  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function spi_lld_polled_exchange
          CFI NoCalls
        THUMB
//  626 uint16_t spi_lld_polled_exchange(SPIDriver *spip, uint16_t frame) {
//  627 
//  628   spip->spi->DR = frame;
spi_lld_polled_exchange:
        LDR      R2,[R0, #+28]
        STRH     R1,[R2, #+12]
//  629   while ((spip->spi->SR & SPI_SR_RXNE) == 0)
??spi_lld_polled_exchange_0:
        LDR      R2,[R0, #+28]
        LDRH     R2,[R2, #+8]
        LSLS     R2,R2,#+31
        BPL.N    ??spi_lld_polled_exchange_0
//  630     ;
//  631   return spip->spi->DR;
        LDR      R0,[R0, #+28]
        LDRH     R0,[R0, #+12]
        BX       LR               ;; return
          CFI EndBlock cfiBlock11
//  632 }

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spi_lld_start(), #1">`:
        DATA
        DC8 "spi_lld_start(), #1"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spi_lld_start(), #2">`:
        DATA
        DC8 "spi_lld_start(), #2"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spi_lld_start(), #3">`:
        DATA
        DC8 "spi_lld_start(), #3"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spi_lld_start(), #4">`:
        DATA
        DC8 "spi_lld_start(), #4"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spi_lld_start(), #5">`:
        DATA
        DC8 "spi_lld_start(), #5"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spi_lld_start(), #6">`:
        DATA
        DC8 "spi_lld_start(), #6"

        END
//  633 
//  634 #endif /* HAL_USE_SPI */
//  635 
//  636 /** @} */
// 
//   148 bytes in section .bss
//   120 bytes in section .rodata
// 1 250 bytes in section .text
// 
// 1 250 bytes of CODE  memory
//   120 bytes of CONST memory
//   148 bytes of DATA  memory
//
//Errors: none
//Warnings: none
