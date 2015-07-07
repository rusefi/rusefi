///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:46 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32\I2Cv1\i2c_lld.c                      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       platforms\STM32\I2Cv1\i2c_lld.c -lCN                 /
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
//                       i2c_lld.s                                            /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME i2c_lld

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _port_irq_epilogue
        EXTERN _stm32_dma_streams
        EXTERN chDbgPanic
        EXTERN chDbgPanic3
        EXTERN chSchGoSleepS
        EXTERN chSchReadyI
        EXTERN chVTResetI
        EXTERN chVTSetI
        EXTERN dbg_check_enter_isr
        EXTERN dbg_check_leave_isr
        EXTERN dbg_check_lock
        EXTERN dbg_check_lock_from_isr
        EXTERN dbg_check_unlock
        EXTERN dbg_check_unlock_from_isr
        EXTERN dmaStreamAllocate
        EXTERN dmaStreamRelease
        EXTERN i2cObjectInit
        EXTERN nvicDisableVector
        EXTERN nvicEnableVector
        EXTERN port_halt
        EXTERN rlist

        PUBLIC I2CD1
        PUBLIC VectorBC
        PUBLIC VectorC0
        PUBLIC i2c_lld_init
        PUBLIC i2c_lld_master_receive_timeout
        PUBLIC i2c_lld_master_transmit_timeout
        PUBLIC i2c_lld_start
        PUBLIC i2c_lld_stop
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\platforms\STM32\I2Cv1\i2c_lld.c
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
//   17    Concepts and parts of this file have been contributed by Uladzimir Pylinsky
//   18    aka barthess.
//   19  */
//   20 
//   21 /**
//   22  * @file    STM32/I2Cv1/i2c_lld.c
//   23  * @brief   STM32 I2C subsystem low level driver source.
//   24  *
//   25  * @addtogroup I2C
//   26  * @{
//   27  */
//   28 
//   29 #include "ch.h"
//   30 #include "hal.h"
//   31 
//   32 #if HAL_USE_I2C || defined(__DOXYGEN__)
//   33 
//   34 /*===========================================================================*/
//   35 /* Driver local definitions.                                                 */
//   36 /*===========================================================================*/
//   37 
//   38 #define I2C1_RX_DMA_CHANNEL                                                 \ 
//   39   STM32_DMA_GETCHANNEL(STM32_I2C_I2C1_RX_DMA_STREAM,                        \ 
//   40                        STM32_I2C1_RX_DMA_CHN)
//   41 
//   42 #define I2C1_TX_DMA_CHANNEL                                                 \ 
//   43   STM32_DMA_GETCHANNEL(STM32_I2C_I2C1_TX_DMA_STREAM,                        \ 
//   44                        STM32_I2C1_TX_DMA_CHN)
//   45 
//   46 #define I2C2_RX_DMA_CHANNEL                                                 \ 
//   47   STM32_DMA_GETCHANNEL(STM32_I2C_I2C2_RX_DMA_STREAM,                        \ 
//   48                        STM32_I2C2_RX_DMA_CHN)
//   49 
//   50 #define I2C2_TX_DMA_CHANNEL                                                 \ 
//   51   STM32_DMA_GETCHANNEL(STM32_I2C_I2C2_TX_DMA_STREAM,                        \ 
//   52                        STM32_I2C2_TX_DMA_CHN)
//   53 
//   54 #define I2C3_RX_DMA_CHANNEL                                                 \ 
//   55   STM32_DMA_GETCHANNEL(STM32_I2C_I2C3_RX_DMA_STREAM,                        \ 
//   56                        STM32_I2C3_RX_DMA_CHN)
//   57 
//   58 #define I2C3_TX_DMA_CHANNEL                                                 \ 
//   59   STM32_DMA_GETCHANNEL(STM32_I2C_I2C3_TX_DMA_STREAM,                        \ 
//   60                        STM32_I2C3_TX_DMA_CHN)
//   61 
//   62 /*===========================================================================*/
//   63 /* Driver constants.                                                         */
//   64 /*===========================================================================*/
//   65 
//   66 #define I2C_EV5_MASTER_MODE_SELECT                                          \ 
//   67   ((uint32_t)(((I2C_SR2_MSL | I2C_SR2_BUSY) << 16) | I2C_SR1_SB))
//   68 
//   69 #define I2C_EV6_MASTER_TRA_MODE_SELECTED                                    \ 
//   70   ((uint32_t)(((I2C_SR2_MSL | I2C_SR2_BUSY | I2C_SR2_TRA) << 16) |          \ 
//   71               I2C_SR1_ADDR | I2C_SR1_TXE))
//   72 
//   73 #define I2C_EV6_MASTER_REC_MODE_SELECTED                                    \ 
//   74   ((uint32_t)(((I2C_SR2_MSL | I2C_SR2_BUSY)<< 16) | I2C_SR1_ADDR))
//   75 
//   76 #define I2C_EV8_2_MASTER_BYTE_TRANSMITTED                                   \ 
//   77   ((uint32_t)(((I2C_SR2_MSL | I2C_SR2_BUSY | I2C_SR2_TRA) << 16) |          \ 
//   78               I2C_SR1_BTF | I2C_SR1_TXE))
//   79 
//   80 #define I2C_EV9_MASTER_ADD10                                                \ 
//   81   ((uint32_t)(((I2C_SR2_MSL | I2C_SR2_BUSY) << 16) | I2C_SR1_ADD10))
//   82 
//   83 #define I2C_EV_MASK 0x00FFFFFF
//   84 
//   85 #define I2C_ERROR_MASK                                                      \ 
//   86   ((uint16_t)(I2C_SR1_BERR | I2C_SR1_ARLO | I2C_SR1_AF | I2C_SR1_OVR |      \ 
//   87               I2C_SR1_PECERR | I2C_SR1_TIMEOUT | I2C_SR1_SMBALERT))
//   88 
//   89 /*===========================================================================*/
//   90 /* Driver exported variables.                                                */
//   91 /*===========================================================================*/
//   92 
//   93 /** @brief I2C1 driver identifier.*/
//   94 #if STM32_I2C_USE_I2C1 || defined(__DOXYGEN__)

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   95 I2CDriver I2CD1;
I2CD1:
        DS8 56
//   96 #endif
//   97 
//   98 /** @brief I2C2 driver identifier.*/
//   99 #if STM32_I2C_USE_I2C2 || defined(__DOXYGEN__)
//  100 I2CDriver I2CD2;
//  101 #endif
//  102 
//  103 /** @brief I2C3 driver identifier.*/
//  104 #if STM32_I2C_USE_I2C3 || defined(__DOXYGEN__)
//  105 I2CDriver I2CD3;
//  106 #endif
//  107 
//  108 /*===========================================================================*/
//  109 /* Driver local variables and types.                                         */
//  110 /*===========================================================================*/
//  111 
//  112 /*===========================================================================*/
//  113 /* Driver local functions.                                                   */
//  114 /*===========================================================================*/
//  115 
//  116 /**
//  117  * @brief   Wakes up the waiting thread.
//  118  *
//  119  * @param[in] i2cp      pointer to the @p I2CDriver object
//  120  * @param[in] msg       wakeup message
//  121  *
//  122  * @notapi
//  123  */
//  124 #define wakeup_isr(i2cp, msg) {                                             \ 
//  125   chSysLockFromIsr();                                                       \ 
//  126   if ((i2cp)->thread != NULL) {                                             \ 
//  127     Thread *tp = (i2cp)->thread;                                            \ 
//  128     (i2cp)->thread = NULL;                                                  \ 
//  129     tp->p_u.rdymsg = (msg);                                                 \ 
//  130     chSchReadyI(tp);                                                        \ 
//  131   }                                                                         \ 
//  132   chSysUnlockFromIsr();                                                     \ 
//  133 }
//  134 
//  135 /**
//  136  * @brief   Aborts an I2C transaction.
//  137  *
//  138  * @param[in] i2cp      pointer to the @p I2CDriver object
//  139  *
//  140  * @notapi
//  141  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function i2c_lld_abort_operation
          CFI NoCalls
        THUMB
//  142 static void i2c_lld_abort_operation(I2CDriver *i2cp) {
//  143   I2C_TypeDef *dp = i2cp->i2c;
i2c_lld_abort_operation:
        LDR      R1,[R0, #+52]
//  144 
//  145   /* Stops the I2C peripheral.*/
//  146   dp->CR1 = I2C_CR1_SWRST;
        MOV      R2,#+32768
        STRH     R2,[R1, #+0]
//  147   dp->CR1 = 0;
        MOVS     R2,#+0
        STRH     R2,[R1, #+0]
//  148   dp->CR2 = 0;
        MOVS     R2,#+0
        STRH     R2,[R1, #+4]
//  149   dp->SR1 = 0;
        MOVS     R2,#+0
        STRH     R2,[R1, #+20]
//  150 
//  151   /* Stops the associated DMA streams.*/
//  152   dmaStreamDisable(i2cp->dmatx);
        LDR      R2,[R0, #+48]
        LDR      R2,[R2, #+0]
        LDR      R2,[R2, #+0]
        LSRS     R2,R2,#+5
        LSLS     R2,R2,#+5
        LDR      R3,[R0, #+48]
        LDR      R3,[R3, #+0]
        STR      R2,[R3, #+0]
??i2c_lld_abort_operation_0:
        LDR      R2,[R0, #+48]
        LDR      R2,[R2, #+0]
        LDR      R2,[R2, #+0]
        LSLS     R2,R2,#+31
        BMI.N    ??i2c_lld_abort_operation_0
        MOVS     R2,#+61
        LDR      R3,[R0, #+48]
        LDRSB    R3,[R3, #+8]
        LSLS     R2,R2,R3
        LDR      R3,[R0, #+48]
        LDR      R3,[R3, #+4]
        STR      R2,[R3, #+0]
//  153   dmaStreamDisable(i2cp->dmarx);
        LDR      R2,[R0, #+44]
        LDR      R2,[R2, #+0]
        LDR      R2,[R2, #+0]
        LSRS     R2,R2,#+5
        LSLS     R2,R2,#+5
        LDR      R3,[R0, #+44]
        LDR      R3,[R3, #+0]
        STR      R2,[R3, #+0]
??i2c_lld_abort_operation_1:
        LDR      R2,[R0, #+44]
        LDR      R2,[R2, #+0]
        LDR      R2,[R2, #+0]
        LSLS     R2,R2,#+31
        BMI.N    ??i2c_lld_abort_operation_1
        MOVS     R2,#+61
        LDR      R3,[R0, #+44]
        LDRSB    R3,[R3, #+8]
        LSLS     R2,R2,R3
        LDR      R3,[R0, #+44]
        LDR      R3,[R3, #+4]
        STR      R2,[R3, #+0]
//  154 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//  155 
//  156 /**
//  157  * @brief   Handling of stalled I2C transactions.
//  158  *
//  159  * @param[in] i2cp      pointer to the @p I2CDriver object
//  160  *
//  161  * @notapi
//  162  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function i2c_lld_safety_timeout
        THUMB
//  163 static void i2c_lld_safety_timeout(void *p) {
i2c_lld_safety_timeout:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  164   I2CDriver *i2cp = (I2CDriver *)p;
        MOVS     R5,R4
//  165 
//  166   chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  167   if (i2cp->thread) {
        LDR      R0,[R5, #+28]
        CMP      R0,#+0
        BEQ.N    ??i2c_lld_safety_timeout_0
//  168     Thread *tp = i2cp->thread;
        LDR      R6,[R5, #+28]
//  169     i2c_lld_abort_operation(i2cp);
        MOVS     R0,R5
          CFI FunCall i2c_lld_abort_operation
        BL       i2c_lld_abort_operation
//  170     i2cp->thread = NULL;
        MOVS     R0,#+0
        STR      R0,[R5, #+28]
//  171     tp->p_u.rdymsg = RDY_TIMEOUT;
        MOVS     R0,#-1
        STR      R0,[R6, #+40]
//  172     chSchReadyI(tp);
        MOVS     R0,R6
          CFI FunCall chSchReadyI
        BL       chSchReadyI
//  173   }
//  174   chSysUnlockFromIsr();
??i2c_lld_safety_timeout_0:
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  175 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock1
//  176 
//  177 /**
//  178  * @brief   Set clock speed.
//  179  *
//  180  * @param[in] i2cp      pointer to the @p I2CDriver object
//  181  *
//  182  * @notapi
//  183  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function i2c_lld_set_clock
        THUMB
//  184 static void i2c_lld_set_clock(I2CDriver *i2cp) {
i2c_lld_set_clock:
        PUSH     {R3-R9,LR}
          CFI R14 Frame(CFA, -4)
          CFI R9 Frame(CFA, -8)
          CFI R8 Frame(CFA, -12)
          CFI R7 Frame(CFA, -16)
          CFI R6 Frame(CFA, -20)
          CFI R5 Frame(CFA, -24)
          CFI R4 Frame(CFA, -28)
          CFI CFA R13+32
        MOVS     R4,R0
//  185   I2C_TypeDef *dp = i2cp->i2c;
        LDR      R5,[R4, #+52]
//  186   uint16_t regCCR, clock_div;
//  187   int32_t clock_speed = i2cp->config->clock_speed;
        LDR      R0,[R4, #+4]
        LDR      R7,[R0, #+4]
//  188   i2cdutycycle_t duty = i2cp->config->duty_cycle;
        LDR      R0,[R4, #+4]
        LDRB     R8,[R0, #+8]
//  189 
//  190   chDbgCheck((i2cp != NULL) && (clock_speed > 0) && (clock_speed <= 4000000),
//  191              "i2c_lld_set_clock");
        CMP      R4,#+0
        BEQ.N    ??i2c_lld_set_clock_0
        CMP      R7,#+1
        BLT.N    ??i2c_lld_set_clock_0
        LDR.W    R0,??DataTable8  ;; 0x3d0901
        CMP      R7,R0
        BLT.N    ??i2c_lld_set_clock_1
??i2c_lld_set_clock_0:
        MOVS     R2,#+191
        LDR.W    R1,??DataTable8_1
        LDR.W    R0,??DataTable8_2
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  192 
//  193   /* CR2 Configuration.*/
//  194   dp->CR2 &= (uint16_t)~I2C_CR2_FREQ;
??i2c_lld_set_clock_1:
        LDRH     R0,[R5, #+4]
        MOVW     R1,#+65472
        ANDS     R0,R1,R0
        STRH     R0,[R5, #+4]
//  195   dp->CR2 |= (uint16_t)I2C_CLK_FREQ;
        LDRH     R0,[R5, #+4]
        ORRS     R0,R0,#0x2A
        STRH     R0,[R5, #+4]
//  196 
//  197   /* CCR Configuration.*/
//  198   regCCR = 0;
        MOVS     R0,#+0
        MOV      R9,R0
//  199   clock_div = I2C_CCR_CCR;
        MOVW     R0,#+4095
        MOVS     R6,R0
//  200 
//  201   if (clock_speed <= 100000) {
        LDR.W    R0,??DataTable8_3  ;; 0x186a1
        CMP      R7,R0
        BGE.N    ??i2c_lld_set_clock_2
//  202     /* Configure clock_div in standard mode.*/
//  203     chDbgAssert(duty == STD_DUTY_CYCLE,
//  204                 "i2c_lld_set_clock(), #1",
//  205                 "Invalid standard mode duty cycle");
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        CMP      R8,#+1
        BEQ.N    ??i2c_lld_set_clock_3
        LDR.W    R0,??DataTable8_4
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  206 
//  207     /* Standard mode clock_div calculate: Tlow/Thigh = 1/1.*/
//  208     chDbgAssert((STM32_PCLK1 % (clock_speed * 2)) == 0,
//  209                 "i2c_lld_set_clock(), #2",
//  210                 "PCLK1 must be divided without remainder");
??i2c_lld_set_clock_3:
        LDR.W    R0,??DataTable8_5  ;; 0x280de80
        LSLS     R1,R7,#+1
        SDIV     R2,R0,R1
        MLS      R0,R1,R2,R0
        CMP      R0,#+0
        BEQ.N    ??i2c_lld_set_clock_4
        LDR.W    R0,??DataTable8_6
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  211     clock_div = (uint16_t)(STM32_PCLK1 / (clock_speed * 2));
??i2c_lld_set_clock_4:
        LDR.W    R0,??DataTable8_5  ;; 0x280de80
        LSLS     R1,R7,#+1
        SDIV     R0,R0,R1
        MOVS     R6,R0
//  212 
//  213     chDbgAssert(clock_div >= 0x04,
//  214                 "i2c_lld_set_clock(), #3",
//  215                 "Clock divider less then 0x04 not allowed");
        UXTH     R6,R6            ;; ZeroExt  R6,R6,#+16,#+16
        CMP      R6,#+4
        BGE.N    ??i2c_lld_set_clock_5
        LDR.W    R0,??DataTable8_7
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  216     regCCR |= (clock_div & I2C_CCR_CCR);
??i2c_lld_set_clock_5:
        LSLS     R0,R6,#+20       ;; ZeroExtS R0,R6,#+20,#+20
        LSRS     R0,R0,#+20
        ORRS     R9,R0,R9
//  217 
//  218     /* Sets the Maximum Rise Time for standard mode.*/
//  219     dp->TRISE = I2C_CLK_FREQ + 1;
        MOVS     R0,#+43
        STRH     R0,[R5, #+32]
        B.N      ??i2c_lld_set_clock_6
//  220   }
//  221   else if (clock_speed <= 400000) {
??i2c_lld_set_clock_2:
        LDR.W    R0,??DataTable8_8  ;; 0x61a81
        CMP      R7,R0
        BGE.N    ??i2c_lld_set_clock_6
//  222     /* Configure clock_div in fast mode.*/
//  223     chDbgAssert((duty == FAST_DUTY_CYCLE_2) || (duty == FAST_DUTY_CYCLE_16_9),
//  224                 "i2c_lld_set_clock(), #4",
//  225                 "Invalid fast mode duty cycle");
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        CMP      R8,#+2
        BEQ.N    ??i2c_lld_set_clock_7
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        CMP      R8,#+3
        BEQ.N    ??i2c_lld_set_clock_7
        LDR.W    R0,??DataTable8_9
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  226 
//  227     if (duty == FAST_DUTY_CYCLE_2) {
??i2c_lld_set_clock_7:
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        CMP      R8,#+2
        BNE.N    ??i2c_lld_set_clock_8
//  228       /* Fast mode clock_div calculate: Tlow/Thigh = 2/1.*/
//  229       chDbgAssert((STM32_PCLK1 % (clock_speed * 3)) == 0,
//  230                   "i2c_lld_set_clock(), #5",
//  231                   "PCLK1 must be divided without remainder");
        LDR.W    R0,??DataTable8_5  ;; 0x280de80
        MOVS     R1,#+3
        MUL      R1,R1,R7
        SDIV     R2,R0,R1
        MLS      R0,R1,R2,R0
        CMP      R0,#+0
        BEQ.N    ??i2c_lld_set_clock_9
        LDR.W    R0,??DataTable8_10
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  232       clock_div = (uint16_t)(STM32_PCLK1 / (clock_speed * 3));
??i2c_lld_set_clock_9:
        LDR.W    R0,??DataTable8_5  ;; 0x280de80
        MOVS     R1,#+3
        MUL      R1,R1,R7
        SDIV     R0,R0,R1
        MOVS     R6,R0
        B.N      ??i2c_lld_set_clock_10
//  233     }
//  234     else if (duty == FAST_DUTY_CYCLE_16_9) {
??i2c_lld_set_clock_8:
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        CMP      R8,#+3
        BNE.N    ??i2c_lld_set_clock_10
//  235       /* Fast mode clock_div calculate: Tlow/Thigh = 16/9.*/
//  236       chDbgAssert((STM32_PCLK1 % (clock_speed * 25)) == 0,
//  237                   "i2c_lld_set_clock(), #6",
//  238                   "PCLK1 must be divided without remainder");
        LDR.W    R0,??DataTable8_5  ;; 0x280de80
        MOVS     R1,#+25
        MUL      R1,R1,R7
        SDIV     R2,R0,R1
        MLS      R0,R1,R2,R0
        CMP      R0,#+0
        BEQ.N    ??i2c_lld_set_clock_11
        LDR.W    R0,??DataTable8_11
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  239       clock_div = (uint16_t)(STM32_PCLK1 / (clock_speed * 25));
??i2c_lld_set_clock_11:
        LDR.W    R0,??DataTable8_5  ;; 0x280de80
        MOVS     R1,#+25
        MUL      R1,R1,R7
        SDIV     R0,R0,R1
        MOVS     R6,R0
//  240       regCCR |= I2C_CCR_DUTY;
        ORRS     R9,R9,#0x4000
//  241     }
//  242 
//  243     chDbgAssert(clock_div >= 0x01,
//  244                     "i2c_lld_set_clock(), #7",
//  245                     "Clock divider less then 0x04 not allowed");
??i2c_lld_set_clock_10:
        UXTH     R6,R6            ;; ZeroExt  R6,R6,#+16,#+16
        CMP      R6,#+0
        BNE.N    ??i2c_lld_set_clock_12
        LDR.W    R0,??DataTable8_12
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  246     regCCR |= (I2C_CCR_FS | (clock_div & I2C_CCR_CCR));
??i2c_lld_set_clock_12:
        LSLS     R0,R6,#+20       ;; ZeroExtS R0,R6,#+20,#+20
        LSRS     R0,R0,#+20
        ORRS     R0,R0,#0x8000
        ORRS     R9,R0,R9
//  247 
//  248     /* Sets the Maximum Rise Time for fast mode.*/
//  249     dp->TRISE = (I2C_CLK_FREQ * 300 / 1000) + 1;
        MOVS     R0,#+13
        STRH     R0,[R5, #+32]
//  250   }
//  251 
//  252   chDbgAssert((clock_div <= I2C_CCR_CCR),
//  253               "i2c_lld_set_clock(), #8", "the selected clock is too low");
??i2c_lld_set_clock_6:
        UXTH     R6,R6            ;; ZeroExt  R6,R6,#+16,#+16
        CMP      R6,#+4096
        BLT.N    ??i2c_lld_set_clock_13
        LDR.W    R0,??DataTable8_13
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  254 
//  255   dp->CCR = regCCR;
??i2c_lld_set_clock_13:
        STRH     R9,[R5, #+28]
//  256 }
        POP      {R0,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock2
//  257 
//  258 /**
//  259  * @brief   Set operation mode of I2C hardware.
//  260  *
//  261  * @param[in] i2cp      pointer to the @p I2CDriver object
//  262  *
//  263  * @notapi
//  264  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function i2c_lld_set_opmode
          CFI NoCalls
        THUMB
//  265 static void i2c_lld_set_opmode(I2CDriver *i2cp) {
i2c_lld_set_opmode:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
//  266   I2C_TypeDef *dp = i2cp->i2c;
        LDR      R1,[R0, #+52]
//  267   i2copmode_t opmode = i2cp->config->op_mode;
        LDR      R2,[R0, #+4]
        LDRB     R2,[R2, #+0]
//  268   uint16_t regCR1;
//  269 
//  270   regCR1 = dp->CR1;
        LDRH     R4,[R1, #+0]
        MOVS     R3,R4
//  271   switch (opmode) {
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        CMP      R2,#+1
        BEQ.N    ??i2c_lld_set_opmode_0
        BCC.N    ??i2c_lld_set_opmode_1
        CMP      R2,#+3
        BEQ.N    ??i2c_lld_set_opmode_2
        BCC.N    ??i2c_lld_set_opmode_3
        B.N      ??i2c_lld_set_opmode_1
//  272   case OPMODE_I2C:
//  273     regCR1 &= (uint16_t)~(I2C_CR1_SMBUS|I2C_CR1_SMBTYPE);
??i2c_lld_set_opmode_0:
        MOVW     R4,#+65525
        ANDS     R3,R4,R3
//  274     break;
        B.N      ??i2c_lld_set_opmode_1
//  275   case OPMODE_SMBUS_DEVICE:
//  276     regCR1 |= I2C_CR1_SMBUS;
??i2c_lld_set_opmode_3:
        ORRS     R3,R3,#0x2
//  277     regCR1 &= (uint16_t)~(I2C_CR1_SMBTYPE);
        MOVW     R4,#+65527
        ANDS     R3,R4,R3
//  278     break;
        B.N      ??i2c_lld_set_opmode_1
//  279   case OPMODE_SMBUS_HOST:
//  280     regCR1 |= (I2C_CR1_SMBUS|I2C_CR1_SMBTYPE);
??i2c_lld_set_opmode_2:
        ORRS     R3,R3,#0xA
//  281     break;
//  282   }
//  283   dp->CR1 = regCR1;
??i2c_lld_set_opmode_1:
        STRH     R3,[R1, #+0]
//  284 }
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock3
//  285 
//  286 /**
//  287  * @brief   I2C shared ISR code.
//  288  *
//  289  * @param[in] i2cp      pointer to the @p I2CDriver object
//  290  *
//  291  * @notapi
//  292  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function i2c_lld_serve_event_interrupt
        THUMB
//  293 static void i2c_lld_serve_event_interrupt(I2CDriver *i2cp) {
i2c_lld_serve_event_interrupt:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
//  294   I2C_TypeDef *dp = i2cp->i2c;
        LDR      R5,[R4, #+52]
//  295   uint32_t regSR2 = dp->SR2;
        LDRH     R6,[R5, #+24]
//  296   uint32_t event = dp->SR1;
        LDRH     R7,[R5, #+20]
//  297 
//  298   /* Interrupts are disabled just before dmaStreamEnable() because there
//  299      is no need of interrupts until next transaction begin. All the work is
//  300      done by the DMA.*/
//  301   switch (I2C_EV_MASK & (event | (regSR2 << 16))) {
        ORRS     R0,R7,R6, LSL #+16
        LSLS     R0,R0,#+8        ;; ZeroExtS R0,R0,#+8,#+8
        LSRS     R0,R0,#+8
        LDR.W    R1,??DataTable8_14  ;; 0x30001
        SUBS     R0,R0,R1
        BEQ.N    ??i2c_lld_serve_event_interrupt_0
        SUBS     R0,R0,#+1
        BEQ.N    ??i2c_lld_serve_event_interrupt_1
        SUBS     R0,R0,#+6
        BEQ.N    ??i2c_lld_serve_event_interrupt_2
        LDR.W    R1,??DataTable8_15  ;; 0x4007a
        SUBS     R0,R0,R1
        BEQ.N    ??i2c_lld_serve_event_interrupt_3
        SUBS     R0,R0,#+2
        BEQ.N    ??i2c_lld_serve_event_interrupt_4
        B.N      ??i2c_lld_serve_event_interrupt_5
//  302   case I2C_EV5_MASTER_MODE_SELECT:
//  303     if ((i2cp->addr >> 8) > 0) { 
??i2c_lld_serve_event_interrupt_0:
        LDRH     R0,[R4, #+32]
        UXTH     R0,R0            ;; ZeroExt  R0,R0,#+16,#+16
        LSRS     R0,R0,#+8
        UXTH     R0,R0            ;; ZeroExt  R0,R0,#+16,#+16
        CMP      R0,#+1
        BLT.N    ??i2c_lld_serve_event_interrupt_6
//  304       /* 10-bit address: 1 1 1 1 0 X X R/W */
//  305       dp->DR = 0xF0 | (0x6 & (i2cp->addr >> 8)) | (0x1 & i2cp->addr);
        LDRH     R0,[R4, #+32]
        UXTH     R0,R0            ;; ZeroExt  R0,R0,#+16,#+16
        LSRS     R0,R0,#+8
        ANDS     R0,R0,#0x6
        LDRB     R1,[R4, #+32]
        ANDS     R1,R1,#0x1
        ORRS     R0,R1,R0
        ORRS     R0,R0,#0xF0
        STRH     R0,[R5, #+16]
        B.N      ??i2c_lld_serve_event_interrupt_7
//  306     } else {
//  307       dp->DR = i2cp->addr;
??i2c_lld_serve_event_interrupt_6:
        LDRH     R0,[R4, #+32]
        STRH     R0,[R5, #+16]
//  308     }
//  309     break;
??i2c_lld_serve_event_interrupt_7:
        B.N      ??i2c_lld_serve_event_interrupt_8
//  310   case I2C_EV9_MASTER_ADD10:
//  311     /* Set second addr byte (10-bit addressing)*/
//  312     dp->DR = (0xFF & (i2cp->addr >> 1));
??i2c_lld_serve_event_interrupt_2:
        LDRH     R0,[R4, #+32]
        UBFX     R0,R0,#+1,#+8
        STRH     R0,[R5, #+16]
//  313     break;
        B.N      ??i2c_lld_serve_event_interrupt_8
//  314   case I2C_EV6_MASTER_REC_MODE_SELECTED:
//  315     dp->CR2 &= ~I2C_CR2_ITEVTEN;
??i2c_lld_serve_event_interrupt_1:
        LDRH     R0,[R5, #+4]
        MOVW     R1,#+65023
        ANDS     R0,R1,R0
        STRH     R0,[R5, #+4]
//  316     dmaStreamEnable(i2cp->dmarx);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x1
        LDR      R1,[R4, #+44]
        LDR      R1,[R1, #+0]
        STR      R0,[R1, #+0]
//  317     dp->CR2 |= I2C_CR2_LAST;                 /* Needed in receiver mode. */
        LDRH     R0,[R5, #+4]
        ORRS     R0,R0,#0x1000
        STRH     R0,[R5, #+4]
//  318     if (dmaStreamGetTransactionSize(i2cp->dmarx) < 2)
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+4]
        CMP      R0,#+2
        BCS.N    ??i2c_lld_serve_event_interrupt_9
//  319       dp->CR1 &= ~I2C_CR1_ACK;
        LDRH     R0,[R5, #+0]
        MOVW     R1,#+64511
        ANDS     R0,R1,R0
        STRH     R0,[R5, #+0]
//  320     break;
??i2c_lld_serve_event_interrupt_9:
        B.N      ??i2c_lld_serve_event_interrupt_8
//  321   case I2C_EV6_MASTER_TRA_MODE_SELECTED:
//  322     dp->CR2 &= ~I2C_CR2_ITEVTEN;
??i2c_lld_serve_event_interrupt_3:
        LDRH     R0,[R5, #+4]
        MOVW     R1,#+65023
        ANDS     R0,R1,R0
        STRH     R0,[R5, #+4]
//  323     dmaStreamEnable(i2cp->dmatx);
        LDR      R0,[R4, #+48]
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x1
        LDR      R1,[R4, #+48]
        LDR      R1,[R1, #+0]
        STR      R0,[R1, #+0]
//  324     break;
        B.N      ??i2c_lld_serve_event_interrupt_8
//  325   case I2C_EV8_2_MASTER_BYTE_TRANSMITTED:
//  326     /* Catches BTF event after the end of transmission.*/
//  327     if (dmaStreamGetTransactionSize(i2cp->dmarx) > 0) {
??i2c_lld_serve_event_interrupt_4:
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+4]
        CMP      R0,#+0
        BEQ.N    ??i2c_lld_serve_event_interrupt_10
//  328       /* Starts "read after write" operation, LSB = 1 -> receive.*/
//  329       i2cp->addr |= 0x01;
        LDRH     R0,[R4, #+32]
        ORRS     R0,R0,#0x1
        STRH     R0,[R4, #+32]
//  330       dp->CR1 |= I2C_CR1_START | I2C_CR1_ACK;
        LDRH     R0,[R5, #+0]
        ORRS     R0,R0,#0x500
        STRH     R0,[R5, #+0]
//  331       return;
        B.N      ??i2c_lld_serve_event_interrupt_11
//  332     }
//  333     dp->CR2 &= ~I2C_CR2_ITEVTEN;
??i2c_lld_serve_event_interrupt_10:
        LDRH     R0,[R5, #+4]
        MOVW     R1,#+65023
        ANDS     R0,R1,R0
        STRH     R0,[R5, #+4]
//  334     dp->CR1 |= I2C_CR1_STOP;
        LDRH     R0,[R5, #+0]
        ORRS     R0,R0,#0x200
        STRH     R0,[R5, #+0]
//  335     wakeup_isr(i2cp, RDY_OK);
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
        LDR      R0,[R4, #+28]
        CMP      R0,#+0
        BEQ.N    ??i2c_lld_serve_event_interrupt_12
        LDR      R0,[R4, #+28]
        MOV      R8,R0
        MOVS     R0,#+0
        STR      R0,[R4, #+28]
        MOVS     R0,#+0
        STR      R0,[R8, #+40]
        MOV      R0,R8
          CFI FunCall chSchReadyI
        BL       chSchReadyI
??i2c_lld_serve_event_interrupt_12:
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  336     break;
        B.N      ??i2c_lld_serve_event_interrupt_8
//  337   default:
//  338     break;
//  339   }
//  340   /* Clear ADDR flag. */
//  341   if (event & (I2C_SR1_ADDR | I2C_SR1_ADD10))
??i2c_lld_serve_event_interrupt_5:
??i2c_lld_serve_event_interrupt_8:
        TST      R7,#0xA
        BEQ.N    ??i2c_lld_serve_event_interrupt_13
//  342     (void)dp->SR2;
        LDRH     R0,[R5, #+24]
//  343 }
??i2c_lld_serve_event_interrupt_13:
??i2c_lld_serve_event_interrupt_11:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock4
//  344 
//  345 /**
//  346  * @brief   DMA RX end IRQ handler.
//  347  *
//  348  * @param[in] i2cp      pointer to the @p I2CDriver object
//  349  * @param[in] flags     pre-shifted content of the ISR register
//  350  *
//  351  * @notapi
//  352  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function i2c_lld_serve_rx_end_irq
        THUMB
//  353 static void i2c_lld_serve_rx_end_irq(I2CDriver *i2cp, uint32_t flags) {
i2c_lld_serve_rx_end_irq:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  354   I2C_TypeDef *dp = i2cp->i2c;
        LDR      R6,[R4, #+52]
//  355 
//  356   /* DMA errors handling.*/
//  357 #if defined(STM32_I2C_DMA_ERROR_HOOK)
//  358   if ((flags & (STM32_DMA_ISR_TEIF | STM32_DMA_ISR_DMEIF)) != 0) {
        TST      R5,#0xC
        BEQ.N    ??i2c_lld_serve_rx_end_irq_0
//  359     STM32_I2C_DMA_ERROR_HOOK(i2cp);
          CFI FunCall port_halt
        BL       port_halt
//  360   }
//  361 #else
//  362   (void)flags;
//  363 #endif
//  364 
//  365   dmaStreamDisable(i2cp->dmarx);
??i2c_lld_serve_rx_end_irq_0:
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+0]
        LSRS     R0,R0,#+5
        LSLS     R0,R0,#+5
        LDR      R1,[R4, #+44]
        LDR      R1,[R1, #+0]
        STR      R0,[R1, #+0]
??i2c_lld_serve_rx_end_irq_1:
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+0]
        LSLS     R0,R0,#+31
        BMI.N    ??i2c_lld_serve_rx_end_irq_1
        MOVS     R0,#+61
        LDR      R1,[R4, #+44]
        LDRSB    R1,[R1, #+8]
        LSLS     R0,R0,R1
        LDR      R1,[R4, #+44]
        LDR      R1,[R1, #+4]
        STR      R0,[R1, #+0]
//  366 
//  367   dp->CR2 &= ~I2C_CR2_LAST;
        LDRH     R0,[R6, #+4]
        MOVW     R1,#+61439
        ANDS     R0,R1,R0
        STRH     R0,[R6, #+4]
//  368   dp->CR1 &= ~I2C_CR1_ACK;
        LDRH     R0,[R6, #+0]
        MOVW     R1,#+64511
        ANDS     R0,R1,R0
        STRH     R0,[R6, #+0]
//  369   dp->CR1 |= I2C_CR1_STOP;
        LDRH     R0,[R6, #+0]
        ORRS     R0,R0,#0x200
        STRH     R0,[R6, #+0]
//  370   wakeup_isr(i2cp, RDY_OK);
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
        LDR      R0,[R4, #+28]
        CMP      R0,#+0
        BEQ.N    ??i2c_lld_serve_rx_end_irq_2
        LDR      R0,[R4, #+28]
        MOVS     R7,R0
        MOVS     R0,#+0
        STR      R0,[R4, #+28]
        MOVS     R0,#+0
        STR      R0,[R7, #+40]
        MOVS     R0,R7
          CFI FunCall chSchReadyI
        BL       chSchReadyI
??i2c_lld_serve_rx_end_irq_2:
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  371 }
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock5
//  372 
//  373 /**
//  374  * @brief    DMA TX end IRQ handler.
//  375  *
//  376  * @param[in] i2cp      pointer to the @p I2CDriver object
//  377  *
//  378  * @notapi
//  379  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function i2c_lld_serve_tx_end_irq
        THUMB
//  380 static void i2c_lld_serve_tx_end_irq(I2CDriver *i2cp, uint32_t flags) {
i2c_lld_serve_tx_end_irq:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  381   I2C_TypeDef *dp = i2cp->i2c;
        LDR      R6,[R4, #+52]
//  382 
//  383   /* DMA errors handling.*/
//  384 #if defined(STM32_I2C_DMA_ERROR_HOOK)
//  385   if ((flags & (STM32_DMA_ISR_TEIF | STM32_DMA_ISR_DMEIF)) != 0) {
        TST      R5,#0xC
        BEQ.N    ??i2c_lld_serve_tx_end_irq_0
//  386     STM32_I2C_DMA_ERROR_HOOK(i2cp);
          CFI FunCall port_halt
        BL       port_halt
//  387   }
//  388 #else
//  389   (void)flags;
//  390 #endif
//  391 
//  392   dmaStreamDisable(i2cp->dmatx);
??i2c_lld_serve_tx_end_irq_0:
        LDR      R0,[R4, #+48]
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+0]
        LSRS     R0,R0,#+5
        LSLS     R0,R0,#+5
        LDR      R1,[R4, #+48]
        LDR      R1,[R1, #+0]
        STR      R0,[R1, #+0]
??i2c_lld_serve_tx_end_irq_1:
        LDR      R0,[R4, #+48]
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+0]
        LSLS     R0,R0,#+31
        BMI.N    ??i2c_lld_serve_tx_end_irq_1
        MOVS     R0,#+61
        LDR      R1,[R4, #+48]
        LDRSB    R1,[R1, #+8]
        LSLS     R0,R0,R1
        LDR      R1,[R4, #+48]
        LDR      R1,[R1, #+4]
        STR      R0,[R1, #+0]
//  393   /* Enables interrupts to catch BTF event meaning transmission part complete.
//  394      Interrupt handler will decide to generate STOP or to begin receiving part
//  395      of R/W transaction itself.*/
//  396   dp->CR2 |= I2C_CR2_ITEVTEN;
        LDRH     R0,[R6, #+4]
        ORRS     R0,R0,#0x200
        STRH     R0,[R6, #+4]
//  397 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock6
//  398 
//  399 /**
//  400  * @brief   I2C error handler.
//  401  *
//  402  * @param[in] i2cp      pointer to the @p I2CDriver object
//  403  * @param[in]  sr       content of the SR1 register to be decoded
//  404  *
//  405  * @notapi
//  406  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function i2c_lld_serve_error_interrupt
        THUMB
//  407 static void i2c_lld_serve_error_interrupt(I2CDriver *i2cp, uint16_t sr) {
i2c_lld_serve_error_interrupt:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  408 
//  409   /* Clears interrupt flags just to be safe.*/
//  410   dmaStreamDisable(i2cp->dmatx);
        LDR      R0,[R4, #+48]
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+0]
        LSRS     R0,R0,#+5
        LSLS     R0,R0,#+5
        LDR      R1,[R4, #+48]
        LDR      R1,[R1, #+0]
        STR      R0,[R1, #+0]
??i2c_lld_serve_error_interrupt_0:
        LDR      R0,[R4, #+48]
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+0]
        LSLS     R0,R0,#+31
        BMI.N    ??i2c_lld_serve_error_interrupt_0
        MOVS     R0,#+61
        LDR      R1,[R4, #+48]
        LDRSB    R1,[R1, #+8]
        LSLS     R0,R0,R1
        LDR      R1,[R4, #+48]
        LDR      R1,[R1, #+4]
        STR      R0,[R1, #+0]
//  411   dmaStreamDisable(i2cp->dmarx);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+0]
        LSRS     R0,R0,#+5
        LSLS     R0,R0,#+5
        LDR      R1,[R4, #+44]
        LDR      R1,[R1, #+0]
        STR      R0,[R1, #+0]
??i2c_lld_serve_error_interrupt_1:
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+0]
        LSLS     R0,R0,#+31
        BMI.N    ??i2c_lld_serve_error_interrupt_1
        MOVS     R0,#+61
        LDR      R1,[R4, #+44]
        LDRSB    R1,[R1, #+8]
        LSLS     R0,R0,R1
        LDR      R1,[R4, #+44]
        LDR      R1,[R1, #+4]
        STR      R0,[R1, #+0]
//  412 
//  413   i2cp->errors = I2CD_NO_ERROR;
        MOVS     R0,#+0
        STR      R0,[R4, #+8]
//  414 
//  415   if (sr & I2C_SR1_BERR)                            /* Bus error.           */
        LSLS     R0,R5,#+23
        BPL.N    ??i2c_lld_serve_error_interrupt_2
//  416     i2cp->errors |= I2CD_BUS_ERROR;
        LDR      R0,[R4, #+8]
        ORRS     R0,R0,#0x1
        STR      R0,[R4, #+8]
//  417 
//  418   if (sr & I2C_SR1_ARLO)                            /* Arbitration lost.    */
??i2c_lld_serve_error_interrupt_2:
        LSLS     R0,R5,#+22
        BPL.N    ??i2c_lld_serve_error_interrupt_3
//  419     i2cp->errors |= I2CD_ARBITRATION_LOST;
        LDR      R0,[R4, #+8]
        ORRS     R0,R0,#0x2
        STR      R0,[R4, #+8]
//  420 
//  421   if (sr & I2C_SR1_AF) {                            /* Acknowledge fail.    */
??i2c_lld_serve_error_interrupt_3:
        LSLS     R0,R5,#+21
        BPL.N    ??i2c_lld_serve_error_interrupt_4
//  422     i2cp->i2c->CR2 &= ~I2C_CR2_ITEVTEN;
        LDR      R0,[R4, #+52]
        LDRH     R0,[R0, #+4]
        MOVW     R1,#+65023
        ANDS     R0,R1,R0
        LDR      R1,[R4, #+52]
        STRH     R0,[R1, #+4]
//  423     i2cp->i2c->CR1 |= I2C_CR1_STOP;                 /* Setting stop bit.    */
        LDR      R0,[R4, #+52]
        LDRH     R0,[R0, #+0]
        ORRS     R0,R0,#0x200
        LDR      R1,[R4, #+52]
        STRH     R0,[R1, #+0]
//  424     i2cp->errors |= I2CD_ACK_FAILURE;
        LDR      R0,[R4, #+8]
        ORRS     R0,R0,#0x4
        STR      R0,[R4, #+8]
//  425   }
//  426 
//  427   if (sr & I2C_SR1_OVR)                             /* Overrun.             */
??i2c_lld_serve_error_interrupt_4:
        LSLS     R0,R5,#+20
        BPL.N    ??i2c_lld_serve_error_interrupt_5
//  428     i2cp->errors |= I2CD_OVERRUN;
        LDR      R0,[R4, #+8]
        ORRS     R0,R0,#0x8
        STR      R0,[R4, #+8]
//  429 
//  430   if (sr & I2C_SR1_TIMEOUT)                         /* SMBus Timeout.       */
??i2c_lld_serve_error_interrupt_5:
        LSLS     R0,R5,#+17
        BPL.N    ??i2c_lld_serve_error_interrupt_6
//  431     i2cp->errors |= I2CD_TIMEOUT;
        LDR      R0,[R4, #+8]
        ORRS     R0,R0,#0x20
        STR      R0,[R4, #+8]
//  432 
//  433   if (sr & I2C_SR1_PECERR)                          /* PEC error.           */
??i2c_lld_serve_error_interrupt_6:
        LSLS     R0,R5,#+19
        BPL.N    ??i2c_lld_serve_error_interrupt_7
//  434     i2cp->errors |= I2CD_PEC_ERROR;
        LDR      R0,[R4, #+8]
        ORRS     R0,R0,#0x10
        STR      R0,[R4, #+8]
//  435 
//  436   if (sr & I2C_SR1_SMBALERT)                        /* SMBus alert.         */
??i2c_lld_serve_error_interrupt_7:
        LSLS     R0,R5,#+16
        BPL.N    ??i2c_lld_serve_error_interrupt_8
//  437     i2cp->errors |= I2CD_SMB_ALERT;
        LDR      R0,[R4, #+8]
        ORRS     R0,R0,#0x40
        STR      R0,[R4, #+8]
//  438 
//  439   /* If some error has been identified then sends wakes the waiting thread.*/
//  440   if (i2cp->errors != I2CD_NO_ERROR)
??i2c_lld_serve_error_interrupt_8:
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BEQ.N    ??i2c_lld_serve_error_interrupt_9
//  441     wakeup_isr(i2cp, RDY_RESET);
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
        LDR      R0,[R4, #+28]
        CMP      R0,#+0
        BEQ.N    ??i2c_lld_serve_error_interrupt_10
        LDR      R0,[R4, #+28]
        MOVS     R6,R0
        MOVS     R0,#+0
        STR      R0,[R4, #+28]
        MVNS     R0,#+1
        STR      R0,[R6, #+40]
        MOVS     R0,R6
          CFI FunCall chSchReadyI
        BL       chSchReadyI
??i2c_lld_serve_error_interrupt_10:
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  442 }
??i2c_lld_serve_error_interrupt_9:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock7
//  443 
//  444 /*===========================================================================*/
//  445 /* Driver interrupt handlers.                                                */
//  446 /*===========================================================================*/
//  447 
//  448 #if STM32_I2C_USE_I2C1 || defined(__DOXYGEN__)
//  449 /**
//  450  * @brief   I2C1 event interrupt handler.
//  451  *
//  452  * @notapi
//  453  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function VectorBC
        THUMB
//  454 CH_IRQ_HANDLER(I2C1_EV_IRQHandler) {
VectorBC:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  455 
//  456   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  457 
//  458   i2c_lld_serve_event_interrupt(&I2CD1);
        LDR.W    R0,??DataTable8_16
          CFI FunCall i2c_lld_serve_event_interrupt
        BL       i2c_lld_serve_event_interrupt
//  459 
//  460   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  461 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock8
//  462 
//  463 /**
//  464  * @brief   I2C1 error interrupt handler.
//  465  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function VectorC0
        THUMB
//  466 CH_IRQ_HANDLER(I2C1_ER_IRQHandler) {
VectorC0:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  467   uint16_t sr = I2CD1.i2c->SR1;
        LDR.W    R0,??DataTable8_16
        LDR      R0,[R0, #+52]
        LDRH     R4,[R0, #+20]
//  468 
//  469   CH_IRQ_PROLOGUE();
          CFI FunCall dbg_check_enter_isr
        BL       dbg_check_enter_isr
//  470 
//  471   I2CD1.i2c->SR1 = ~(sr & I2C_ERROR_MASK);
        LDR.W    R0,??DataTable8_16
        LDR      R0,[R0, #+52]
        UXTH     R4,R4            ;; ZeroExt  R4,R4,#+16,#+16
        ANDS     R1,R4,#0xDF00
        MVNS     R1,R1
        STRH     R1,[R0, #+20]
//  472   i2c_lld_serve_error_interrupt(&I2CD1, sr);
        MOVS     R1,R4
        UXTH     R1,R1            ;; ZeroExt  R1,R1,#+16,#+16
        LDR.N    R0,??DataTable8_16
          CFI FunCall i2c_lld_serve_error_interrupt
        BL       i2c_lld_serve_error_interrupt
//  473 
//  474   CH_IRQ_EPILOGUE();
          CFI FunCall dbg_check_leave_isr
        BL       dbg_check_leave_isr
          CFI FunCall _port_irq_epilogue
        BL       _port_irq_epilogue
//  475 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock9
//  476 #endif /* STM32_I2C_USE_I2C1 */
//  477 
//  478 #if STM32_I2C_USE_I2C2 || defined(__DOXYGEN__)
//  479 /**
//  480  * @brief   I2C2 event interrupt handler.
//  481  *
//  482  * @notapi
//  483  */
//  484 CH_IRQ_HANDLER(I2C2_EV_IRQHandler) {
//  485 
//  486   CH_IRQ_PROLOGUE();
//  487 
//  488   i2c_lld_serve_event_interrupt(&I2CD2);
//  489 
//  490   CH_IRQ_EPILOGUE();
//  491 }
//  492 
//  493 /**
//  494  * @brief   I2C2 error interrupt handler.
//  495  *
//  496  * @notapi
//  497  */
//  498 CH_IRQ_HANDLER(I2C2_ER_IRQHandler) {
//  499   uint16_t sr = I2CD2.i2c->SR1;
//  500 
//  501   CH_IRQ_PROLOGUE();
//  502 
//  503   I2CD2.i2c->SR1 = ~(sr & I2C_ERROR_MASK);
//  504   i2c_lld_serve_error_interrupt(&I2CD2, sr);
//  505 
//  506   CH_IRQ_EPILOGUE();
//  507 }
//  508 #endif /* STM32_I2C_USE_I2C2 */
//  509 
//  510 #if STM32_I2C_USE_I2C3 || defined(__DOXYGEN__)
//  511 /**
//  512  * @brief   I2C3 event interrupt handler.
//  513  *
//  514  * @notapi
//  515  */
//  516 CH_IRQ_HANDLER(I2C3_EV_IRQHandler) {
//  517 
//  518   CH_IRQ_PROLOGUE();
//  519 
//  520   i2c_lld_serve_event_interrupt(&I2CD3);
//  521 
//  522   CH_IRQ_EPILOGUE();
//  523 }
//  524 
//  525 /**
//  526  * @brief   I2C3 error interrupt handler.
//  527  *
//  528  * @notapi
//  529  */
//  530 CH_IRQ_HANDLER(I2C3_ER_IRQHandler) {
//  531   uint16_t sr = I2CD3.i2c->SR1;
//  532 
//  533   CH_IRQ_PROLOGUE();
//  534 
//  535   I2CD3.i2c->SR1 = ~(sr & I2C_ERROR_MASK);
//  536   i2c_lld_serve_error_interrupt(&I2CD3, sr);
//  537 
//  538   CH_IRQ_EPILOGUE();
//  539 }
//  540 #endif /* STM32_I2C_USE_I2C3 */
//  541 
//  542 /*===========================================================================*/
//  543 /* Driver exported functions.                                                */
//  544 /*===========================================================================*/
//  545 
//  546 /**
//  547  * @brief   Low level I2C driver initialization.
//  548  *
//  549  * @notapi
//  550  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function i2c_lld_init
        THUMB
//  551 void i2c_lld_init(void) {
i2c_lld_init:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  552 
//  553 #if STM32_I2C_USE_I2C1
//  554   i2cObjectInit(&I2CD1);
        LDR.N    R0,??DataTable8_16
          CFI FunCall i2cObjectInit
        BL       i2cObjectInit
//  555   I2CD1.thread = NULL;
        LDR.N    R0,??DataTable8_16
        MOVS     R1,#+0
        STR      R1,[R0, #+28]
//  556   I2CD1.i2c    = I2C1;
        LDR.N    R0,??DataTable8_16
        LDR.N    R1,??DataTable8_17  ;; 0x40005400
        STR      R1,[R0, #+52]
//  557   I2CD1.dmarx  = STM32_DMA_STREAM(STM32_I2C_I2C1_RX_DMA_STREAM);
        LDR.N    R0,??DataTable8_16
        LDR.N    R1,??DataTable8_18
        STR      R1,[R0, #+44]
//  558   I2CD1.dmatx  = STM32_DMA_STREAM(STM32_I2C_I2C1_TX_DMA_STREAM);
        LDR.N    R0,??DataTable8_16
        LDR.N    R1,??DataTable8_19
        STR      R1,[R0, #+48]
//  559 #endif /* STM32_I2C_USE_I2C1 */
//  560 
//  561 #if STM32_I2C_USE_I2C2
//  562   i2cObjectInit(&I2CD2);
//  563   I2CD2.thread = NULL;
//  564   I2CD2.i2c    = I2C2;
//  565   I2CD2.dmarx  = STM32_DMA_STREAM(STM32_I2C_I2C2_RX_DMA_STREAM);
//  566   I2CD2.dmatx  = STM32_DMA_STREAM(STM32_I2C_I2C2_TX_DMA_STREAM);
//  567 #endif /* STM32_I2C_USE_I2C2 */
//  568 
//  569 #if STM32_I2C_USE_I2C3
//  570   i2cObjectInit(&I2CD3);
//  571   I2CD3.thread = NULL;
//  572   I2CD3.i2c    = I2C3;
//  573   I2CD3.dmarx  = STM32_DMA_STREAM(STM32_I2C_I2C3_RX_DMA_STREAM);
//  574   I2CD3.dmatx  = STM32_DMA_STREAM(STM32_I2C_I2C3_TX_DMA_STREAM);
//  575 #endif /* STM32_I2C_USE_I2C3 */
//  576 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock10
//  577 
//  578 /**
//  579  * @brief   Configures and activates the I2C peripheral.
//  580  *
//  581  * @param[in] i2cp      pointer to the @p I2CDriver object
//  582  *
//  583  * @notapi
//  584  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function i2c_lld_start
        THUMB
//  585 void i2c_lld_start(I2CDriver *i2cp) {
i2c_lld_start:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  586   I2C_TypeDef *dp = i2cp->i2c;
        LDR      R5,[R4, #+52]
//  587 
//  588   i2cp->txdmamode = STM32_DMA_CR_PSIZE_BYTE | STM32_DMA_CR_MSIZE_BYTE |
//  589                     STM32_DMA_CR_MINC       | STM32_DMA_CR_DMEIE |
//  590                     STM32_DMA_CR_TEIE       | STM32_DMA_CR_TCIE |
//  591                     STM32_DMA_CR_DIR_M2P;
        MOVW     R0,#+1110
        STR      R0,[R4, #+40]
//  592   i2cp->rxdmamode = STM32_DMA_CR_PSIZE_BYTE | STM32_DMA_CR_MSIZE_BYTE |
//  593                     STM32_DMA_CR_MINC       | STM32_DMA_CR_DMEIE |
//  594                     STM32_DMA_CR_TEIE       | STM32_DMA_CR_TCIE |
//  595                     STM32_DMA_CR_DIR_P2M;
        MOVW     R0,#+1046
        STR      R0,[R4, #+36]
//  596 
//  597   /* If in stopped state then enables the I2C and DMA clocks.*/
//  598   if (i2cp->state == I2C_STOP) {
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BNE.N    ??i2c_lld_start_0
//  599 
//  600 #if STM32_I2C_USE_I2C1
//  601     if (&I2CD1 == i2cp) {
        LDR.N    R0,??DataTable8_16
        CMP      R0,R4
        BNE.N    ??i2c_lld_start_0
//  602       bool_t b;
//  603 
//  604       rccResetI2C1();
        LDR.N    R0,??DataTable8_20  ;; 0x40023820
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x200000
        LDR.N    R1,??DataTable8_20  ;; 0x40023820
        STR      R0,[R1, #+0]
        LDR.N    R0,??DataTable8_20  ;; 0x40023820
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  605       b = dmaStreamAllocate(i2cp->dmarx,
//  606                             STM32_I2C_I2C1_IRQ_PRIORITY,
//  607                             (stm32_dmaisr_t)i2c_lld_serve_rx_end_irq,
//  608                             (void *)i2cp);
        MOVS     R3,R4
        LDR.N    R2,??DataTable8_21
        MOVS     R1,#+5
        LDR      R0,[R4, #+44]
          CFI FunCall dmaStreamAllocate
        BL       dmaStreamAllocate
        MOVS     R6,R0
//  609       chDbgAssert(!b, "i2c_lld_start(), #1", "stream already allocated");
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+0
        BEQ.N    ??i2c_lld_start_1
        LDR.N    R0,??DataTable8_22
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  610       b = dmaStreamAllocate(i2cp->dmatx,
//  611                             STM32_I2C_I2C1_IRQ_PRIORITY,
//  612                             (stm32_dmaisr_t)i2c_lld_serve_tx_end_irq,
//  613                             (void *)i2cp);
??i2c_lld_start_1:
        MOVS     R3,R4
        LDR.N    R2,??DataTable8_23
        MOVS     R1,#+5
        LDR      R0,[R4, #+48]
          CFI FunCall dmaStreamAllocate
        BL       dmaStreamAllocate
        MOVS     R6,R0
//  614       chDbgAssert(!b, "i2c_lld_start(), #2", "stream already allocated");
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+0
        BEQ.N    ??i2c_lld_start_2
        LDR.N    R0,??DataTable8_24
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  615       rccEnableI2C1(FALSE);
??i2c_lld_start_2:
        LDR.N    R0,??DataTable8_25  ;; 0x40023840
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x200000
        LDR.N    R1,??DataTable8_25  ;; 0x40023840
        STR      R0,[R1, #+0]
//  616       nvicEnableVector(I2C1_EV_IRQn,
//  617           CORTEX_PRIORITY_MASK(STM32_I2C_I2C1_IRQ_PRIORITY));
        MOVS     R1,#+80
        MOVS     R0,#+31
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  618       nvicEnableVector(I2C1_ER_IRQn,
//  619           CORTEX_PRIORITY_MASK(STM32_I2C_I2C1_IRQ_PRIORITY));
        MOVS     R1,#+80
        MOVS     R0,#+32
          CFI FunCall nvicEnableVector
        BL       nvicEnableVector
//  620 
//  621       i2cp->rxdmamode |= STM32_DMA_CR_CHSEL(I2C1_RX_DMA_CHANNEL) |
//  622                        STM32_DMA_CR_PL(STM32_I2C_I2C1_DMA_PRIORITY);
        LDR      R0,[R4, #+36]
        LDR.N    R1,??DataTable8_26  ;; 0x2030000
        ORRS     R0,R1,R0
        STR      R0,[R4, #+36]
//  623       i2cp->txdmamode |= STM32_DMA_CR_CHSEL(I2C1_TX_DMA_CHANNEL) |
//  624                        STM32_DMA_CR_PL(STM32_I2C_I2C1_DMA_PRIORITY);
        LDR      R0,[R4, #+40]
        LDR.N    R1,??DataTable8_26  ;; 0x2030000
        ORRS     R0,R1,R0
        STR      R0,[R4, #+40]
//  625     }
//  626 #endif /* STM32_I2C_USE_I2C1 */
//  627 
//  628 #if STM32_I2C_USE_I2C2
//  629     if (&I2CD2 == i2cp) {
//  630       bool_t b;
//  631 
//  632       rccResetI2C2();
//  633       b = dmaStreamAllocate(i2cp->dmarx,
//  634                             STM32_I2C_I2C2_IRQ_PRIORITY,
//  635                             (stm32_dmaisr_t)i2c_lld_serve_rx_end_irq,
//  636                             (void *)i2cp);
//  637       chDbgAssert(!b, "i2c_lld_start(), #3", "stream already allocated");
//  638       b = dmaStreamAllocate(i2cp->dmatx,
//  639                             STM32_I2C_I2C2_IRQ_PRIORITY,
//  640                             (stm32_dmaisr_t)i2c_lld_serve_tx_end_irq,
//  641                             (void *)i2cp);
//  642       chDbgAssert(!b, "i2c_lld_start(), #4", "stream already allocated");
//  643       rccEnableI2C2(FALSE);
//  644       nvicEnableVector(I2C2_EV_IRQn,
//  645           CORTEX_PRIORITY_MASK(STM32_I2C_I2C2_IRQ_PRIORITY));
//  646       nvicEnableVector(I2C2_ER_IRQn,
//  647           CORTEX_PRIORITY_MASK(STM32_I2C_I2C2_IRQ_PRIORITY));
//  648 
//  649       i2cp->rxdmamode |= STM32_DMA_CR_CHSEL(I2C2_RX_DMA_CHANNEL) |
//  650                        STM32_DMA_CR_PL(STM32_I2C_I2C2_DMA_PRIORITY);
//  651       i2cp->txdmamode |= STM32_DMA_CR_CHSEL(I2C2_TX_DMA_CHANNEL) |
//  652                        STM32_DMA_CR_PL(STM32_I2C_I2C2_DMA_PRIORITY);
//  653     }
//  654 #endif /* STM32_I2C_USE_I2C2 */
//  655 
//  656 #if STM32_I2C_USE_I2C3
//  657     if (&I2CD3 == i2cp) {
//  658       bool_t b;
//  659 
//  660       rccResetI2C3();
//  661       b = dmaStreamAllocate(i2cp->dmarx,
//  662                             STM32_I2C_I2C3_IRQ_PRIORITY,
//  663                             (stm32_dmaisr_t)i2c_lld_serve_rx_end_irq,
//  664                             (void *)i2cp);
//  665       chDbgAssert(!b, "i2c_lld_start(), #5", "stream already allocated");
//  666       b = dmaStreamAllocate(i2cp->dmatx,
//  667                             STM32_I2C_I2C3_IRQ_PRIORITY,
//  668                             (stm32_dmaisr_t)i2c_lld_serve_tx_end_irq,
//  669                             (void *)i2cp);
//  670       chDbgAssert(!b, "i2c_lld_start(), #6", "stream already allocated");
//  671       rccEnableI2C3(FALSE);
//  672       nvicEnableVector(I2C3_EV_IRQn,
//  673           CORTEX_PRIORITY_MASK(STM32_I2C_I2C3_IRQ_PRIORITY));
//  674       nvicEnableVector(I2C3_ER_IRQn,
//  675           CORTEX_PRIORITY_MASK(STM32_I2C_I2C3_IRQ_PRIORITY));
//  676 
//  677       i2cp->rxdmamode |= STM32_DMA_CR_CHSEL(I2C3_RX_DMA_CHANNEL) |
//  678                        STM32_DMA_CR_PL(STM32_I2C_I2C3_DMA_PRIORITY);
//  679       i2cp->txdmamode |= STM32_DMA_CR_CHSEL(I2C3_TX_DMA_CHANNEL) |
//  680                        STM32_DMA_CR_PL(STM32_I2C_I2C3_DMA_PRIORITY);
//  681     }
//  682 #endif /* STM32_I2C_USE_I2C3 */
//  683   }
//  684 
//  685   /* I2C registers pointed by the DMA.*/
//  686   dmaStreamSetPeripheral(i2cp->dmarx, &dp->DR);
??i2c_lld_start_0:
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
        ADDS     R1,R5,#+16
        STR      R1,[R0, #+8]
//  687   dmaStreamSetPeripheral(i2cp->dmatx, &dp->DR);
        LDR      R0,[R4, #+48]
        LDR      R0,[R0, #+0]
        ADDS     R1,R5,#+16
        STR      R1,[R0, #+8]
//  688 
//  689   /* Reset i2c peripheral.*/
//  690   dp->CR1 = I2C_CR1_SWRST;
        MOV      R0,#+32768
        STRH     R0,[R5, #+0]
//  691   dp->CR1 = 0;
        MOVS     R0,#+0
        STRH     R0,[R5, #+0]
//  692   dp->CR2 = I2C_CR2_ITERREN | I2C_CR2_DMAEN;
        MOV      R0,#+2304
        STRH     R0,[R5, #+4]
//  693 
//  694   /* Setup I2C parameters.*/
//  695   i2c_lld_set_clock(i2cp);
        MOVS     R0,R4
          CFI FunCall i2c_lld_set_clock
        BL       i2c_lld_set_clock
//  696   i2c_lld_set_opmode(i2cp);
        MOVS     R0,R4
          CFI FunCall i2c_lld_set_opmode
        BL       i2c_lld_set_opmode
//  697 
//  698   /* Ready to go.*/
//  699   dp->CR1 |= I2C_CR1_PE;
        LDRH     R0,[R5, #+0]
        ORRS     R0,R0,#0x1
        STRH     R0,[R5, #+0]
//  700 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock11
//  701 
//  702 /**
//  703  * @brief   Deactivates the I2C peripheral.
//  704  *
//  705  * @param[in] i2cp      pointer to the @p I2CDriver object
//  706  *
//  707  * @notapi
//  708  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function i2c_lld_stop
        THUMB
//  709 void i2c_lld_stop(I2CDriver *i2cp) {
i2c_lld_stop:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  710 
//  711   /* If not in stopped state then disables the I2C clock.*/
//  712   if (i2cp->state != I2C_STOP) {
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BEQ.N    ??i2c_lld_stop_0
//  713 
//  714     /* I2C disable.*/
//  715     i2c_lld_abort_operation(i2cp);
        MOVS     R0,R4
          CFI FunCall i2c_lld_abort_operation
        BL       i2c_lld_abort_operation
//  716     dmaStreamRelease(i2cp->dmatx);
        LDR      R0,[R4, #+48]
          CFI FunCall dmaStreamRelease
        BL       dmaStreamRelease
//  717     dmaStreamRelease(i2cp->dmarx);
        LDR      R0,[R4, #+44]
          CFI FunCall dmaStreamRelease
        BL       dmaStreamRelease
//  718 
//  719 #if STM32_I2C_USE_I2C1
//  720     if (&I2CD1 == i2cp) {
        LDR.N    R0,??DataTable8_16
        CMP      R0,R4
        BNE.N    ??i2c_lld_stop_0
//  721       nvicDisableVector(I2C1_EV_IRQn);
        MOVS     R0,#+31
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  722       nvicDisableVector(I2C1_ER_IRQn);
        MOVS     R0,#+32
          CFI FunCall nvicDisableVector
        BL       nvicDisableVector
//  723       rccDisableI2C1(FALSE);
        LDR.N    R0,??DataTable8_25  ;; 0x40023840
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x200000
        LDR.N    R1,??DataTable8_25  ;; 0x40023840
        STR      R0,[R1, #+0]
//  724     }
//  725 #endif
//  726 
//  727 #if STM32_I2C_USE_I2C2
//  728     if (&I2CD2 == i2cp) {
//  729       nvicDisableVector(I2C2_EV_IRQn);
//  730       nvicDisableVector(I2C2_ER_IRQn);
//  731       rccDisableI2C2(FALSE);
//  732     }
//  733 #endif
//  734 
//  735 #if STM32_I2C_USE_I2C3
//  736     if (&I2CD3 == i2cp) {
//  737       nvicDisableVector(I2C3_EV_IRQn);
//  738       nvicDisableVector(I2C3_ER_IRQn);
//  739       rccDisableI2C3(FALSE);
//  740     }
//  741 #endif
//  742   }
//  743 }
??i2c_lld_stop_0:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock12
//  744 
//  745 /**
//  746  * @brief   Receives data via the I2C bus as master.
//  747  * @details Number of receiving bytes must be more than 1 on STM32F1x. This is
//  748  *          hardware restriction.
//  749  *
//  750  * @param[in] i2cp      pointer to the @p I2CDriver object
//  751  * @param[in] addr      slave device address
//  752  * @param[out] rxbuf    pointer to the receive buffer
//  753  * @param[in] rxbytes   number of bytes to be received
//  754  * @param[in] timeout   the number of ticks before the operation timeouts,
//  755  *                      the following special values are allowed:
//  756  *                      - @a TIME_INFINITE no timeout.
//  757  *                      .
//  758  * @return              The operation status.
//  759  * @retval RDY_OK       if the function succeeded.
//  760  * @retval RDY_RESET    if one or more I2C errors occurred, the errors can
//  761  *                      be retrieved using @p i2cGetErrors().
//  762  * @retval RDY_TIMEOUT  if a timeout occurred before operation end. <b>After a
//  763  *                      timeout the driver must be stopped and restarted
//  764  *                      because the bus is in an uncertain state</b>.
//  765  *
//  766  * @notapi
//  767  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function i2c_lld_master_receive_timeout
        THUMB
//  768 msg_t i2c_lld_master_receive_timeout(I2CDriver *i2cp, i2caddr_t addr,
//  769                                      uint8_t *rxbuf, size_t rxbytes,
//  770                                      systime_t timeout) {
i2c_lld_master_receive_timeout:
        PUSH     {R4-R9,LR}
          CFI R14 Frame(CFA, -4)
          CFI R9 Frame(CFA, -8)
          CFI R8 Frame(CFA, -12)
          CFI R7 Frame(CFA, -16)
          CFI R6 Frame(CFA, -20)
          CFI R5 Frame(CFA, -24)
          CFI R4 Frame(CFA, -28)
          CFI CFA R13+28
        SUB      SP,SP,#+20
          CFI CFA R13+48
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
        LDR      R8,[SP, #+48]
//  771   I2C_TypeDef *dp = i2cp->i2c;
        LDR      R9,[R4, #+52]
//  772   VirtualTimer vt;
//  773 
//  774 #if defined(STM32F1XX_I2C)
//  775   chDbgCheck((rxbytes > 1), "i2c_lld_master_receive_timeout");
//  776 #endif
//  777 
//  778   /* Global timeout for the whole operation.*/
//  779   if (timeout != TIME_INFINITE)
        CMN      R8,#+1
        BEQ.N    ??i2c_lld_master_receive_timeout_0
//  780     chVTSetI(&vt, timeout, i2c_lld_safety_timeout, (void *)i2cp);
        MOVS     R3,R4
        LDR.N    R2,??DataTable8_27
        MOV      R1,R8
        ADD      R0,SP,#+0
          CFI FunCall chVTSetI
        BL       chVTSetI
//  781 
//  782   /* Releases the lock from high level driver.*/
//  783   chSysUnlock();
??i2c_lld_master_receive_timeout_0:
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  784 
//  785   /* Initializes driver fields, LSB = 1 -> receive.*/
//  786   i2cp->addr = (addr << 1) | 0x01;
        LSLS     R0,R5,#+1
        ORRS     R0,R0,#0x1
        STRH     R0,[R4, #+32]
//  787   i2cp->errors = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+8]
//  788 
//  789   /* RX DMA setup.*/
//  790   dmaStreamSetMode(i2cp->dmarx, i2cp->rxdmamode);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
        LDR      R1,[R4, #+36]
        STR      R1,[R0, #+0]
//  791   dmaStreamSetMemory0(i2cp->dmarx, rxbuf);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
        STR      R6,[R0, #+12]
//  792   dmaStreamSetTransactionSize(i2cp->dmarx, rxbytes);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
        STR      R7,[R0, #+4]
//  793 
//  794   /* Waits until BUSY flag is reset and the STOP from the previous operation
//  795      is completed, alternatively for a timeout condition.*/
//  796   while ((dp->SR2 & I2C_SR2_BUSY) || (dp->CR1 & I2C_CR1_STOP)) {
??i2c_lld_master_receive_timeout_1:
        LDRH     R0,[R9, #+24]
        LSLS     R0,R0,#+30
        BMI.N    ??i2c_lld_master_receive_timeout_2
        LDRH     R0,[R9, #+0]
        LSLS     R0,R0,#+22
        BPL.N    ??i2c_lld_master_receive_timeout_3
//  797     chSysLock();
??i2c_lld_master_receive_timeout_2:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  798     if ((timeout != TIME_INFINITE) && !chVTIsArmedI(&vt))
        CMN      R8,#+1
        BEQ.N    ??i2c_lld_master_receive_timeout_4
        LDR      R0,[SP, #+12]
        CMP      R0,#+0
        BNE.N    ??i2c_lld_master_receive_timeout_4
//  799       return RDY_TIMEOUT;
        MOVS     R0,#-1
        B.N      ??i2c_lld_master_receive_timeout_5
//  800     chSysUnlock();
??i2c_lld_master_receive_timeout_4:
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
        B.N      ??i2c_lld_master_receive_timeout_1
//  801   }
//  802 
//  803   /* This lock will be released in high level driver.*/
//  804   chSysLock();
??i2c_lld_master_receive_timeout_3:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  805 
//  806   /* Atomic check on the timer in order to make sure that a timeout didn't
//  807      happen outside the critical zone.*/
//  808   if ((timeout != TIME_INFINITE) && !chVTIsArmedI(&vt))
        CMN      R8,#+1
        BEQ.N    ??i2c_lld_master_receive_timeout_6
        LDR      R0,[SP, #+12]
        CMP      R0,#+0
        BNE.N    ??i2c_lld_master_receive_timeout_6
//  809     return RDY_TIMEOUT;
        MOVS     R0,#-1
        B.N      ??i2c_lld_master_receive_timeout_5
//  810 
//  811   /* Starts the operation.*/
//  812   dp->CR2 |= I2C_CR2_ITEVTEN;
??i2c_lld_master_receive_timeout_6:
        LDRH     R0,[R9, #+4]
        ORRS     R0,R0,#0x200
        STRH     R0,[R9, #+4]
//  813   dp->CR1 |= I2C_CR1_START | I2C_CR1_ACK;
        LDRH     R0,[R9, #+0]
        ORRS     R0,R0,#0x500
        STRH     R0,[R9, #+0]
//  814 
//  815   /* Waits for the operation completion or a timeout.*/
//  816   i2cp->thread = chThdSelf();
        LDR.N    R0,??DataTable8_28
        LDR      R0,[R0, #+24]
        STR      R0,[R4, #+28]
//  817   chSchGoSleepS(THD_STATE_SUSPENDED);
        MOVS     R0,#+2
          CFI FunCall chSchGoSleepS
        BL       chSchGoSleepS
//  818   if ((timeout != TIME_INFINITE) && chVTIsArmedI(&vt))
        CMN      R8,#+1
        BEQ.N    ??i2c_lld_master_receive_timeout_7
        LDR      R0,[SP, #+12]
        CMP      R0,#+0
        BEQ.N    ??i2c_lld_master_receive_timeout_7
//  819     chVTResetI(&vt);
        ADD      R0,SP,#+0
          CFI FunCall chVTResetI
        BL       chVTResetI
//  820 
//  821   return chThdSelf()->p_u.rdymsg;
??i2c_lld_master_receive_timeout_7:
        LDR.N    R0,??DataTable8_28
        LDR      R0,[R0, #+24]
        LDR      R0,[R0, #+40]
??i2c_lld_master_receive_timeout_5:
        ADD      SP,SP,#+20
          CFI CFA R13+28
        POP      {R4-R9,PC}       ;; return
          CFI EndBlock cfiBlock13
//  822 }
//  823 
//  824 /**
//  825  * @brief   Transmits data via the I2C bus as master.
//  826  * @details Number of receiving bytes must be 0 or more than 1 on STM32F1x.
//  827  *          This is hardware restriction.
//  828  *
//  829  * @param[in] i2cp      pointer to the @p I2CDriver object
//  830  * @param[in] addr      slave device address
//  831  * @param[in] txbuf     pointer to the transmit buffer
//  832  * @param[in] txbytes   number of bytes to be transmitted
//  833  * @param[out] rxbuf    pointer to the receive buffer
//  834  * @param[in] rxbytes   number of bytes to be received
//  835  * @param[in] timeout   the number of ticks before the operation timeouts,
//  836  *                      the following special values are allowed:
//  837  *                      - @a TIME_INFINITE no timeout.
//  838  *                      .
//  839  * @return              The operation status.
//  840  * @retval RDY_OK       if the function succeeded.
//  841  * @retval RDY_RESET    if one or more I2C errors occurred, the errors can
//  842  *                      be retrieved using @p i2cGetErrors().
//  843  * @retval RDY_TIMEOUT  if a timeout occurred before operation end. <b>After a
//  844  *                      timeout the driver must be stopped and restarted
//  845  *                      because the bus is in an uncertain state</b>.
//  846  *
//  847  * @notapi
//  848  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function i2c_lld_master_transmit_timeout
        THUMB
//  849 msg_t i2c_lld_master_transmit_timeout(I2CDriver *i2cp, i2caddr_t addr,
//  850                                       const uint8_t *txbuf, size_t txbytes,
//  851                                       uint8_t *rxbuf, size_t rxbytes,
//  852                                       systime_t timeout) {
i2c_lld_master_transmit_timeout:
        PUSH     {R4-R11,LR}
          CFI R14 Frame(CFA, -4)
          CFI R11 Frame(CFA, -8)
          CFI R10 Frame(CFA, -12)
          CFI R9 Frame(CFA, -16)
          CFI R8 Frame(CFA, -20)
          CFI R7 Frame(CFA, -24)
          CFI R6 Frame(CFA, -28)
          CFI R5 Frame(CFA, -32)
          CFI R4 Frame(CFA, -36)
          CFI CFA R13+36
        SUB      SP,SP,#+20
          CFI CFA R13+56
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
        LDR      R8,[SP, #+56]
        LDR      R9,[SP, #+60]
        LDR      R10,[SP, #+64]
//  853   I2C_TypeDef *dp = i2cp->i2c;
        LDR      R11,[R4, #+52]
//  854   VirtualTimer vt;
//  855 
//  856 #if defined(STM32F1XX_I2C)
//  857   chDbgCheck(((rxbytes == 0) || ((rxbytes > 1) && (rxbuf != NULL))),
//  858              "i2c_lld_master_transmit_timeout");
//  859 #endif
//  860 
//  861   /* Global timeout for the whole operation.*/
//  862   if (timeout != TIME_INFINITE)
        CMN      R10,#+1
        BEQ.N    ??i2c_lld_master_transmit_timeout_0
//  863     chVTSetI(&vt, timeout, i2c_lld_safety_timeout, (void *)i2cp);
        MOVS     R3,R4
        LDR.N    R2,??DataTable8_27
        MOV      R1,R10
        ADD      R0,SP,#+0
          CFI FunCall chVTSetI
        BL       chVTSetI
//  864 
//  865   /* Releases the lock from high level driver.*/
//  866   chSysUnlock();
??i2c_lld_master_transmit_timeout_0:
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  867 
//  868   /* Initializes driver fields, LSB = 0 -> write.*/
//  869   i2cp->addr = addr << 1;
        LSLS     R0,R5,#+1
        STRH     R0,[R4, #+32]
//  870   i2cp->errors = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+8]
//  871 
//  872   /* TX DMA setup.*/
//  873   dmaStreamSetMode(i2cp->dmatx, i2cp->txdmamode);
        LDR      R0,[R4, #+48]
        LDR      R0,[R0, #+0]
        LDR      R1,[R4, #+40]
        STR      R1,[R0, #+0]
//  874   dmaStreamSetMemory0(i2cp->dmatx, txbuf);
        LDR      R0,[R4, #+48]
        LDR      R0,[R0, #+0]
        STR      R6,[R0, #+12]
//  875   dmaStreamSetTransactionSize(i2cp->dmatx, txbytes);
        LDR      R0,[R4, #+48]
        LDR      R0,[R0, #+0]
        STR      R7,[R0, #+4]
//  876 
//  877   /* RX DMA setup.*/
//  878   dmaStreamSetMode(i2cp->dmarx, i2cp->rxdmamode);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
        LDR      R1,[R4, #+36]
        STR      R1,[R0, #+0]
//  879   dmaStreamSetMemory0(i2cp->dmarx, rxbuf);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
        STR      R8,[R0, #+12]
//  880   dmaStreamSetTransactionSize(i2cp->dmarx, rxbytes);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
        STR      R9,[R0, #+4]
//  881 
//  882   /* Waits until BUSY flag is reset and the STOP from the previous operation
//  883      is completed, alternatively for a timeout condition.*/
//  884   while ((dp->SR2 & I2C_SR2_BUSY) || (dp->CR1 & I2C_CR1_STOP)) {
??i2c_lld_master_transmit_timeout_1:
        LDRH     R0,[R11, #+24]
        LSLS     R0,R0,#+30
        BMI.N    ??i2c_lld_master_transmit_timeout_2
        LDRH     R0,[R11, #+0]
        LSLS     R0,R0,#+22
        BPL.N    ??i2c_lld_master_transmit_timeout_3
//  885     chSysLock();
??i2c_lld_master_transmit_timeout_2:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  886     if ((timeout != TIME_INFINITE) && !chVTIsArmedI(&vt))
        CMN      R10,#+1
        BEQ.N    ??i2c_lld_master_transmit_timeout_4
        LDR      R0,[SP, #+12]
        CMP      R0,#+0
        BNE.N    ??i2c_lld_master_transmit_timeout_4
//  887       return RDY_TIMEOUT;
        MOVS     R0,#-1
        B.N      ??i2c_lld_master_transmit_timeout_5
//  888     chSysUnlock();
??i2c_lld_master_transmit_timeout_4:
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
        B.N      ??i2c_lld_master_transmit_timeout_1
//  889   }
//  890 
//  891   /* This lock will be released in high level driver.*/
//  892   chSysLock();
??i2c_lld_master_transmit_timeout_3:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  893 
//  894   /* Atomic check on the timer in order to make sure that a timeout didn't
//  895      happen outside the critical zone.*/
//  896   if ((timeout != TIME_INFINITE) && !chVTIsArmedI(&vt))
        CMN      R10,#+1
        BEQ.N    ??i2c_lld_master_transmit_timeout_6
        LDR      R0,[SP, #+12]
        CMP      R0,#+0
        BNE.N    ??i2c_lld_master_transmit_timeout_6
//  897     return RDY_TIMEOUT;
        MOVS     R0,#-1
        B.N      ??i2c_lld_master_transmit_timeout_5
//  898 
//  899   /* Starts the operation.*/
//  900   dp->CR2 |= I2C_CR2_ITEVTEN;
??i2c_lld_master_transmit_timeout_6:
        LDRH     R0,[R11, #+4]
        ORRS     R0,R0,#0x200
        STRH     R0,[R11, #+4]
//  901   dp->CR1 |= I2C_CR1_START;
        LDRH     R0,[R11, #+0]
        ORRS     R0,R0,#0x100
        STRH     R0,[R11, #+0]
//  902 
//  903   /* Waits for the operation completion or a timeout.*/
//  904   i2cp->thread = chThdSelf();
        LDR.N    R0,??DataTable8_28
        LDR      R0,[R0, #+24]
        STR      R0,[R4, #+28]
//  905   chSchGoSleepS(THD_STATE_SUSPENDED);
        MOVS     R0,#+2
          CFI FunCall chSchGoSleepS
        BL       chSchGoSleepS
//  906   if ((timeout != TIME_INFINITE) && chVTIsArmedI(&vt))
        CMN      R10,#+1
        BEQ.N    ??i2c_lld_master_transmit_timeout_7
        LDR      R0,[SP, #+12]
        CMP      R0,#+0
        BEQ.N    ??i2c_lld_master_transmit_timeout_7
//  907     chVTResetI(&vt);
        ADD      R0,SP,#+0
          CFI FunCall chVTResetI
        BL       chVTResetI
//  908 
//  909   return chThdSelf()->p_u.rdymsg;
??i2c_lld_master_transmit_timeout_7:
        LDR.N    R0,??DataTable8_28
        LDR      R0,[R0, #+24]
        LDR      R0,[R0, #+40]
??i2c_lld_master_transmit_timeout_5:
        ADD      SP,SP,#+20
          CFI CFA R13+36
        POP      {R4-R11,PC}      ;; return
          CFI EndBlock cfiBlock14
//  910 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8:
        DC32     0x3d0901

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_1:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_2:
        DC32     `?<Constant "\\"i2c_lld_set_clock\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_3:
        DC32     0x186a1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_4:
        DC32     `?<Constant "i2c_lld_set_clock(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_5:
        DC32     0x280de80

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_6:
        DC32     `?<Constant "i2c_lld_set_clock(), #2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_7:
        DC32     `?<Constant "i2c_lld_set_clock(), #3">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_8:
        DC32     0x61a81

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_9:
        DC32     `?<Constant "i2c_lld_set_clock(), #4">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_10:
        DC32     `?<Constant "i2c_lld_set_clock(), #5">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_11:
        DC32     `?<Constant "i2c_lld_set_clock(), #6">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_12:
        DC32     `?<Constant "i2c_lld_set_clock(), #7">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_13:
        DC32     `?<Constant "i2c_lld_set_clock(), #8">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_14:
        DC32     0x30001

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_15:
        DC32     0x4007a

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_16:
        DC32     I2CD1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_17:
        DC32     0x40005400

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_18:
        DC32     _stm32_dma_streams+0x3C

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_19:
        DC32     _stm32_dma_streams+0x48

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_20:
        DC32     0x40023820

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_21:
        DC32     i2c_lld_serve_rx_end_irq

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_22:
        DC32     `?<Constant "i2c_lld_start(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_23:
        DC32     i2c_lld_serve_tx_end_irq

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_24:
        DC32     `?<Constant "i2c_lld_start(), #2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_25:
        DC32     0x40023840

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_26:
        DC32     0x2030000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_27:
        DC32     i2c_lld_safety_timeout

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_28:
        DC32     rlist

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"i2c_lld_set_clock\\"()">`:
        DATA
        DC8 "\"i2c_lld_set_clock\"()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`:
        DATA
        DC8 46H, 3AH, 5CH, 73H, 74H, 75H, 66H, 66H
        DC8 5CH, 72H, 75H, 73H, 65H, 66H, 69H, 5FH
        DC8 73H, 6FH, 75H, 72H, 63H, 65H, 66H, 6FH
        DC8 72H, 67H, 65H, 5CH, 66H, 69H, 72H, 6DH
        DC8 77H, 61H, 72H, 65H, 5CH, 63H, 68H, 69H
        DC8 62H, 69H, 6FH, 73H, 5CH, 6FH, 73H, 5CH
        DC8 68H, 61H, 6CH, 5CH, 70H, 6CH, 61H, 74H
        DC8 66H, 6FH, 72H, 6DH, 73H, 5CH, 53H, 54H
        DC8 4DH, 33H, 32H, 5CH, 49H, 32H, 43H, 76H
        DC8 31H, 5CH, 69H, 32H, 63H, 5FH, 6CH, 6CH
        DC8 64H, 2EH, 63H, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "i2c_lld_set_clock(), #1">`:
        DATA
        DC8 "i2c_lld_set_clock(), #1"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "i2c_lld_set_clock(), #2">`:
        DATA
        DC8 "i2c_lld_set_clock(), #2"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "i2c_lld_set_clock(), #3">`:
        DATA
        DC8 "i2c_lld_set_clock(), #3"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "i2c_lld_set_clock(), #4">`:
        DATA
        DC8 "i2c_lld_set_clock(), #4"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "i2c_lld_set_clock(), #5">`:
        DATA
        DC8 "i2c_lld_set_clock(), #5"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "i2c_lld_set_clock(), #6">`:
        DATA
        DC8 "i2c_lld_set_clock(), #6"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "i2c_lld_set_clock(), #7">`:
        DATA
        DC8 "i2c_lld_set_clock(), #7"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "i2c_lld_set_clock(), #8">`:
        DATA
        DC8 "i2c_lld_set_clock(), #8"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "i2c_lld_start(), #1">`:
        DATA
        DC8 "i2c_lld_start(), #1"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "i2c_lld_start(), #2">`:
        DATA
        DC8 "i2c_lld_start(), #2"

        END
//  911 
//  912 #endif /* HAL_USE_I2C */
//  913 
//  914 /** @} */
// 
//    56 bytes in section .bss
//   340 bytes in section .rodata
// 2 334 bytes in section .text
// 
// 2 334 bytes of CODE  memory
//   340 bytes of CONST memory
//    56 bytes of DATA  memory
//
//Errors: none
//Warnings: none
