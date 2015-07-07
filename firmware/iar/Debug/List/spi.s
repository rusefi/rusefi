///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:06 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\spi.c                                            /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\spi.c -lCN F:\stuff\rusefi_sourceforge\firmware\ /
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
//                       spi.s                                                /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME spi

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN chDbgPanic
        EXTERN chDbgPanic3
        EXTERN chMtxInit
        EXTERN chMtxLock
        EXTERN chMtxUnlock
        EXTERN chSchGoSleepS
        EXTERN dbg_check_lock
        EXTERN dbg_check_unlock
        EXTERN rlist
        EXTERN spi_lld_exchange
        EXTERN spi_lld_ignore
        EXTERN spi_lld_init
        EXTERN spi_lld_receive
        EXTERN spi_lld_select
        EXTERN spi_lld_send
        EXTERN spi_lld_start
        EXTERN spi_lld_stop
        EXTERN spi_lld_unselect

        PUBLIC spiAcquireBus
        PUBLIC spiExchange
        PUBLIC spiIgnore
        PUBLIC spiInit
        PUBLIC spiObjectInit
        PUBLIC spiReceive
        PUBLIC spiReleaseBus
        PUBLIC spiSelect
        PUBLIC spiSend
        PUBLIC spiStart
        PUBLIC spiStartExchange
        PUBLIC spiStartIgnore
        PUBLIC spiStartReceive
        PUBLIC spiStartSend
        PUBLIC spiStop
        PUBLIC spiUnselect
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\src\spi.c
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
//   29  * @file    spi.c
//   30  * @brief   SPI Driver code.
//   31  *
//   32  * @addtogroup SPI
//   33  * @{
//   34  */
//   35 
//   36 #include "ch.h"
//   37 #include "hal.h"
//   38 
//   39 #if HAL_USE_SPI || defined(__DOXYGEN__)
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
//   62  * @brief   SPI Driver initialization.
//   63  * @note    This function is implicitly invoked by @p halInit(), there is
//   64  *          no need to explicitly initialize the driver.
//   65  *
//   66  * @init
//   67  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function spiInit
        THUMB
//   68 void spiInit(void) {
spiInit:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   69 
//   70   spi_lld_init();
          CFI FunCall spi_lld_init
        BL       spi_lld_init
//   71 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0
//   72 
//   73 /**
//   74  * @brief   Initializes the standard part of a @p SPIDriver structure.
//   75  *
//   76  * @param[out] spip     pointer to the @p SPIDriver object
//   77  *
//   78  * @init
//   79  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function spiObjectInit
        THUMB
//   80 void spiObjectInit(SPIDriver *spip) {
spiObjectInit:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   81 
//   82   spip->state = SPI_STOP;
        MOVS     R0,#+1
        STRB     R0,[R4, #+0]
//   83   spip->config = NULL;
        MOVS     R0,#+0
        STR      R0,[R4, #+4]
//   84 #if SPI_USE_WAIT
//   85   spip->thread = NULL;
        MOVS     R0,#+0
        STR      R0,[R4, #+8]
//   86 #endif /* SPI_USE_WAIT */
//   87 #if SPI_USE_MUTUAL_EXCLUSION
//   88 #if CH_USE_MUTEXES
//   89   chMtxInit(&spip->mutex);
        ADDS     R0,R4,#+12
          CFI FunCall chMtxInit
        BL       chMtxInit
//   90 #else
//   91   chSemInit(&spip->semaphore, 1);
//   92 #endif
//   93 #endif /* SPI_USE_MUTUAL_EXCLUSION */
//   94 #if defined(SPI_DRIVER_EXT_INIT_HOOK)
//   95   SPI_DRIVER_EXT_INIT_HOOK(spip);
//   96 #endif
//   97 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1
//   98 
//   99 /**
//  100  * @brief   Configures and activates the SPI peripheral.
//  101  *
//  102  * @param[in] spip      pointer to the @p SPIDriver object
//  103  * @param[in] config    pointer to the @p SPIConfig object
//  104  *
//  105  * @api
//  106  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function spiStart
        THUMB
//  107 void spiStart(SPIDriver *spip, const SPIConfig *config) {
spiStart:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  108 
//  109   chDbgCheck((spip != NULL) && (config != NULL), "spiStart");
        CMP      R4,#+0
        BEQ.N    ??spiStart_0
        CMP      R5,#+0
        BNE.N    ??spiStart_1
??spiStart_0:
        MOVS     R2,#+109
        LDR.W    R1,??DataTable13
        LDR.W    R0,??DataTable13_1
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  110 
//  111   chSysLock();
??spiStart_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  112   chDbgAssert((spip->state == SPI_STOP) || (spip->state == SPI_READY),
//  113               "spiStart(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BEQ.N    ??spiStart_2
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??spiStart_2
        LDR.W    R0,??DataTable13_2
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  114   spip->config = config;
??spiStart_2:
        STR      R5,[R4, #+4]
//  115   spi_lld_start(spip);
        MOVS     R0,R4
          CFI FunCall spi_lld_start
        BL       spi_lld_start
//  116   spip->state = SPI_READY;
        MOVS     R0,#+2
        STRB     R0,[R4, #+0]
//  117   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  118 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2
//  119 
//  120 /**
//  121  * @brief Deactivates the SPI peripheral.
//  122  * @note  Deactivating the peripheral also enforces a release of the slave
//  123  *        select line.
//  124  *
//  125  * @param[in] spip      pointer to the @p SPIDriver object
//  126  *
//  127  * @api
//  128  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function spiStop
        THUMB
//  129 void spiStop(SPIDriver *spip) {
spiStop:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  130 
//  131   chDbgCheck(spip != NULL, "spiStop");
        CMP      R4,#+0
        BNE.N    ??spiStop_0
        MOVS     R2,#+131
        LDR.W    R1,??DataTable13
        LDR.W    R0,??DataTable13_3
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  132 
//  133   chSysLock();
??spiStop_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  134   chDbgAssert((spip->state == SPI_STOP) || (spip->state == SPI_READY),
//  135               "spiStop(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BEQ.N    ??spiStop_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??spiStop_1
        LDR.W    R0,??DataTable13_4
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  136   spi_lld_unselect(spip);
??spiStop_1:
        MOVS     R0,R4
          CFI FunCall spi_lld_unselect
        BL       spi_lld_unselect
//  137   spi_lld_stop(spip);
        MOVS     R0,R4
          CFI FunCall spi_lld_stop
        BL       spi_lld_stop
//  138   spip->state = SPI_STOP;
        MOVS     R0,#+1
        STRB     R0,[R4, #+0]
//  139   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  140 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//  141 
//  142 /**
//  143  * @brief   Asserts the slave select signal and prepares for transfers.
//  144  *
//  145  * @param[in] spip      pointer to the @p SPIDriver object
//  146  *
//  147  * @api
//  148  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function spiSelect
        THUMB
//  149 void spiSelect(SPIDriver *spip) {
spiSelect:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  150 
//  151   chDbgCheck(spip != NULL, "spiSelect");
        CMP      R4,#+0
        BNE.N    ??spiSelect_0
        MOVS     R2,#+151
        LDR.W    R1,??DataTable13
        LDR.W    R0,??DataTable13_5
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  152 
//  153   chSysLock();
??spiSelect_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  154   chDbgAssert(spip->state == SPI_READY, "spiSelect(), #1", "not ready");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??spiSelect_1
        LDR.W    R0,??DataTable13_6
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  155   spiSelectI(spip);
??spiSelect_1:
        MOVS     R0,R4
          CFI FunCall spi_lld_select
        BL       spi_lld_select
//  156   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  157 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock4
//  158 
//  159 /**
//  160  * @brief   Deasserts the slave select signal.
//  161  * @details The previously selected peripheral is unselected.
//  162  *
//  163  * @param[in] spip      pointer to the @p SPIDriver object
//  164  *
//  165  * @api
//  166  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function spiUnselect
        THUMB
//  167 void spiUnselect(SPIDriver *spip) {
spiUnselect:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  168 
//  169   chDbgCheck(spip != NULL, "spiUnselect");
        CMP      R4,#+0
        BNE.N    ??spiUnselect_0
        MOVS     R2,#+169
        LDR.W    R1,??DataTable13
        LDR.W    R0,??DataTable13_7
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  170 
//  171   chSysLock();
??spiUnselect_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  172   chDbgAssert(spip->state == SPI_READY, "spiUnselect(), #1", "not ready");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??spiUnselect_1
        LDR.W    R0,??DataTable13_8
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  173   spiUnselectI(spip);
??spiUnselect_1:
        MOVS     R0,R4
          CFI FunCall spi_lld_unselect
        BL       spi_lld_unselect
//  174   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  175 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock5
//  176 
//  177 /**
//  178  * @brief   Ignores data on the SPI bus.
//  179  * @details This asynchronous function starts the transmission of a series of
//  180  *          idle words on the SPI bus and ignores the received data.
//  181  * @pre     A slave must have been selected using @p spiSelect() or
//  182  *          @p spiSelectI().
//  183  * @post    At the end of the operation the configured callback is invoked.
//  184  *
//  185  * @param[in] spip      pointer to the @p SPIDriver object
//  186  * @param[in] n         number of words to be ignored
//  187  *
//  188  * @api
//  189  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function spiStartIgnore
        THUMB
//  190 void spiStartIgnore(SPIDriver *spip, size_t n) {
spiStartIgnore:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  191 
//  192   chDbgCheck((spip != NULL) && (n > 0), "spiStartIgnore");
        CMP      R4,#+0
        BEQ.N    ??spiStartIgnore_0
        CMP      R5,#+0
        BNE.N    ??spiStartIgnore_1
??spiStartIgnore_0:
        MOVS     R2,#+192
        LDR.N    R1,??DataTable13
        LDR.W    R0,??DataTable13_9
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  193 
//  194   chSysLock();
??spiStartIgnore_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  195   chDbgAssert(spip->state == SPI_READY, "spiStartIgnore(), #1", "not ready");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??spiStartIgnore_2
        LDR.W    R0,??DataTable13_10
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  196   spiStartIgnoreI(spip, n);
??spiStartIgnore_2:
        MOVS     R0,#+3
        STRB     R0,[R4, #+0]
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall spi_lld_ignore
        BL       spi_lld_ignore
//  197   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  198 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock6
//  199 
//  200 /**
//  201  * @brief   Exchanges data on the SPI bus.
//  202  * @details This asynchronous function starts a simultaneous transmit/receive
//  203  *          operation.
//  204  * @pre     A slave must have been selected using @p spiSelect() or
//  205  *          @p spiSelectI().
//  206  * @post    At the end of the operation the configured callback is invoked.
//  207  * @note    The buffers are organized as uint8_t arrays for data sizes below
//  208  *          or equal to 8 bits else it is organized as uint16_t arrays.
//  209  *
//  210  * @param[in] spip      pointer to the @p SPIDriver object
//  211  * @param[in] n         number of words to be exchanged
//  212  * @param[in] txbuf     the pointer to the transmit buffer
//  213  * @param[out] rxbuf    the pointer to the receive buffer
//  214  *
//  215  * @api
//  216  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function spiStartExchange
        THUMB
//  217 void spiStartExchange(SPIDriver *spip, size_t n,
//  218                       const void *txbuf, void *rxbuf) {
spiStartExchange:
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
//  219 
//  220   chDbgCheck((spip != NULL) && (n > 0) && (rxbuf != NULL) && (txbuf != NULL),
//  221              "spiStartExchange");
        CMP      R4,#+0
        BEQ.N    ??spiStartExchange_0
        CMP      R5,#+0
        BEQ.N    ??spiStartExchange_0
        CMP      R7,#+0
        BEQ.N    ??spiStartExchange_0
        CMP      R6,#+0
        BNE.N    ??spiStartExchange_1
??spiStartExchange_0:
        MOVS     R2,#+221
        LDR.N    R1,??DataTable13
        LDR.N    R0,??DataTable13_11
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  222 
//  223   chSysLock();
??spiStartExchange_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  224   chDbgAssert(spip->state == SPI_READY, "spiStartExchange(), #1", "not ready");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??spiStartExchange_2
        LDR.N    R0,??DataTable13_12
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  225   spiStartExchangeI(spip, n, txbuf, rxbuf);
??spiStartExchange_2:
        MOVS     R0,#+3
        STRB     R0,[R4, #+0]
        MOVS     R3,R7
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall spi_lld_exchange
        BL       spi_lld_exchange
//  226   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  227 }
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock7
//  228 
//  229 /**
//  230  * @brief   Sends data over the SPI bus.
//  231  * @details This asynchronous function starts a transmit operation.
//  232  * @pre     A slave must have been selected using @p spiSelect() or
//  233  *          @p spiSelectI().
//  234  * @post    At the end of the operation the configured callback is invoked.
//  235  * @note    The buffers are organized as uint8_t arrays for data sizes below
//  236  *          or equal to 8 bits else it is organized as uint16_t arrays.
//  237  *
//  238  * @param[in] spip      pointer to the @p SPIDriver object
//  239  * @param[in] n         number of words to send
//  240  * @param[in] txbuf     the pointer to the transmit buffer
//  241  *
//  242  * @api
//  243  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function spiStartSend
        THUMB
//  244 void spiStartSend(SPIDriver *spip, size_t n, const void *txbuf) {
spiStartSend:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  245 
//  246   chDbgCheck((spip != NULL) && (n > 0) && (txbuf != NULL),
//  247              "spiStartSend");
        CMP      R4,#+0
        BEQ.N    ??spiStartSend_0
        CMP      R5,#+0
        BEQ.N    ??spiStartSend_0
        CMP      R6,#+0
        BNE.N    ??spiStartSend_1
??spiStartSend_0:
        MOVS     R2,#+247
        LDR.N    R1,??DataTable13
        LDR.N    R0,??DataTable13_13
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  248 
//  249   chSysLock();
??spiStartSend_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  250   chDbgAssert(spip->state == SPI_READY, "spiStartSend(), #1", "not ready");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??spiStartSend_2
        LDR.N    R0,??DataTable13_14
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  251   spiStartSendI(spip, n, txbuf);
??spiStartSend_2:
        MOVS     R0,#+3
        STRB     R0,[R4, #+0]
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall spi_lld_send
        BL       spi_lld_send
//  252   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  253 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock8
//  254 
//  255 /**
//  256  * @brief   Receives data from the SPI bus.
//  257  * @details This asynchronous function starts a receive operation.
//  258  * @pre     A slave must have been selected using @p spiSelect() or
//  259  *          @p spiSelectI().
//  260  * @post    At the end of the operation the configured callback is invoked.
//  261  * @note    The buffers are organized as uint8_t arrays for data sizes below
//  262  *          or equal to 8 bits else it is organized as uint16_t arrays.
//  263  *
//  264  * @param[in] spip      pointer to the @p SPIDriver object
//  265  * @param[in] n         number of words to receive
//  266  * @param[out] rxbuf    the pointer to the receive buffer
//  267  *
//  268  * @api
//  269  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function spiStartReceive
        THUMB
//  270 void spiStartReceive(SPIDriver *spip, size_t n, void *rxbuf) {
spiStartReceive:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  271 
//  272   chDbgCheck((spip != NULL) && (n > 0) && (rxbuf != NULL),
//  273              "spiStartReceive");
        CMP      R4,#+0
        BEQ.N    ??spiStartReceive_0
        CMP      R5,#+0
        BEQ.N    ??spiStartReceive_0
        CMP      R6,#+0
        BNE.N    ??spiStartReceive_1
??spiStartReceive_0:
        MOVW     R2,#+273
        LDR.N    R1,??DataTable13
        LDR.N    R0,??DataTable13_15
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  274 
//  275   chSysLock();
??spiStartReceive_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  276   chDbgAssert(spip->state == SPI_READY, "spiStartReceive(), #1", "not ready");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??spiStartReceive_2
        LDR.N    R0,??DataTable13_16
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  277   spiStartReceiveI(spip, n, rxbuf);
??spiStartReceive_2:
        MOVS     R0,#+3
        STRB     R0,[R4, #+0]
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall spi_lld_receive
        BL       spi_lld_receive
//  278   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  279 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock9
//  280 
//  281 #if SPI_USE_WAIT || defined(__DOXYGEN__)
//  282 /**
//  283  * @brief   Ignores data on the SPI bus.
//  284  * @details This synchronous function performs the transmission of a series of
//  285  *          idle words on the SPI bus and ignores the received data.
//  286  * @pre     In order to use this function the option @p SPI_USE_WAIT must be
//  287  *          enabled.
//  288  * @pre     In order to use this function the driver must have been configured
//  289  *          without callbacks (@p end_cb = @p NULL).
//  290  *
//  291  * @param[in] spip      pointer to the @p SPIDriver object
//  292  * @param[in] n         number of words to be ignored
//  293  *
//  294  * @api
//  295  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function spiIgnore
        THUMB
//  296 void spiIgnore(SPIDriver *spip, size_t n) {
spiIgnore:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  297 
//  298   chDbgCheck((spip != NULL) && (n > 0), "spiIgnoreWait");
        CMP      R4,#+0
        BEQ.N    ??spiIgnore_0
        CMP      R5,#+0
        BNE.N    ??spiIgnore_1
??spiIgnore_0:
        MOV      R2,#+298
        LDR.N    R1,??DataTable13
        LDR.N    R0,??DataTable13_17
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  299 
//  300   chSysLock();
??spiIgnore_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  301   chDbgAssert(spip->state == SPI_READY, "spiIgnore(), #1", "not ready");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??spiIgnore_2
        LDR.N    R0,??DataTable13_18
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  302   chDbgAssert(spip->config->end_cb == NULL, "spiIgnore(), #2", "has callback");
??spiIgnore_2:
        LDR      R0,[R4, #+4]
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??spiIgnore_3
        LDR.N    R0,??DataTable13_19
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  303   spiStartIgnoreI(spip, n);
??spiIgnore_3:
        MOVS     R0,#+3
        STRB     R0,[R4, #+0]
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall spi_lld_ignore
        BL       spi_lld_ignore
//  304   _spi_wait_s(spip);
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BEQ.N    ??spiIgnore_4
        LDR.N    R0,??DataTable13_20
          CFI FunCall chDbgPanic
        BL       chDbgPanic
??spiIgnore_4:
        LDR.N    R0,??DataTable13_21
        LDR      R0,[R0, #+24]
        STR      R0,[R4, #+8]
        MOVS     R0,#+2
          CFI FunCall chSchGoSleepS
        BL       chSchGoSleepS
//  305   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  306 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock10
//  307 
//  308 /**
//  309  * @brief   Exchanges data on the SPI bus.
//  310  * @details This synchronous function performs a simultaneous transmit/receive
//  311  *          operation.
//  312  * @pre     In order to use this function the option @p SPI_USE_WAIT must be
//  313  *          enabled.
//  314  * @pre     In order to use this function the driver must have been configured
//  315  *          without callbacks (@p end_cb = @p NULL).
//  316  * @note    The buffers are organized as uint8_t arrays for data sizes below
//  317  *          or equal to 8 bits else it is organized as uint16_t arrays.
//  318  *
//  319  * @param[in] spip      pointer to the @p SPIDriver object
//  320  * @param[in] n         number of words to be exchanged
//  321  * @param[in] txbuf     the pointer to the transmit buffer
//  322  * @param[out] rxbuf    the pointer to the receive buffer
//  323  *
//  324  * @api
//  325  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function spiExchange
        THUMB
//  326 void spiExchange(SPIDriver *spip, size_t n,
//  327                  const void *txbuf, void *rxbuf) {
spiExchange:
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
//  328 
//  329   chDbgCheck((spip != NULL) && (n > 0) && (rxbuf != NULL) && (txbuf != NULL),
//  330              "spiExchange");
        CMP      R4,#+0
        BEQ.N    ??spiExchange_0
        CMP      R5,#+0
        BEQ.N    ??spiExchange_0
        CMP      R7,#+0
        BEQ.N    ??spiExchange_0
        CMP      R6,#+0
        BNE.N    ??spiExchange_1
??spiExchange_0:
        MOV      R2,#+330
        LDR.N    R1,??DataTable13
        LDR.N    R0,??DataTable13_22
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  331 
//  332   chSysLock();
??spiExchange_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  333   chDbgAssert(spip->state == SPI_READY, "spiExchange(), #1", "not ready");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??spiExchange_2
        LDR.N    R0,??DataTable13_23
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  334   chDbgAssert(spip->config->end_cb == NULL,
//  335               "spiExchange(), #2", "has callback");
??spiExchange_2:
        LDR      R0,[R4, #+4]
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??spiExchange_3
        LDR.N    R0,??DataTable13_24
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  336   spiStartExchangeI(spip, n, txbuf, rxbuf);
??spiExchange_3:
        MOVS     R0,#+3
        STRB     R0,[R4, #+0]
        MOVS     R3,R7
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall spi_lld_exchange
        BL       spi_lld_exchange
//  337   _spi_wait_s(spip);
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BEQ.N    ??spiExchange_4
        LDR.N    R0,??DataTable13_20
          CFI FunCall chDbgPanic
        BL       chDbgPanic
??spiExchange_4:
        LDR.N    R0,??DataTable13_21
        LDR      R0,[R0, #+24]
        STR      R0,[R4, #+8]
        MOVS     R0,#+2
          CFI FunCall chSchGoSleepS
        BL       chSchGoSleepS
//  338   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  339 }
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock11
//  340 
//  341 /**
//  342  * @brief   Sends data over the SPI bus.
//  343  * @details This synchronous function performs a transmit operation.
//  344  * @pre     In order to use this function the option @p SPI_USE_WAIT must be
//  345  *          enabled.
//  346  * @pre     In order to use this function the driver must have been configured
//  347  *          without callbacks (@p end_cb = @p NULL).
//  348  * @note    The buffers are organized as uint8_t arrays for data sizes below
//  349  *          or equal to 8 bits else it is organized as uint16_t arrays.
//  350  *
//  351  * @param[in] spip      pointer to the @p SPIDriver object
//  352  * @param[in] n         number of words to send
//  353  * @param[in] txbuf     the pointer to the transmit buffer
//  354  *
//  355  * @api
//  356  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function spiSend
        THUMB
//  357 void spiSend(SPIDriver *spip, size_t n, const void *txbuf) {
spiSend:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  358 
//  359   chDbgCheck((spip != NULL) && (n > 0) && (txbuf != NULL), "spiSend");
        CMP      R4,#+0
        BEQ.N    ??spiSend_0
        CMP      R5,#+0
        BEQ.N    ??spiSend_0
        CMP      R6,#+0
        BNE.N    ??spiSend_1
??spiSend_0:
        MOVW     R2,#+359
        LDR.N    R1,??DataTable13
        LDR.N    R0,??DataTable13_25
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  360 
//  361   chSysLock();
??spiSend_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  362   chDbgAssert(spip->state == SPI_READY, "spiSend(), #1", "not ready");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??spiSend_2
        LDR.N    R0,??DataTable13_26
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  363   chDbgAssert(spip->config->end_cb == NULL, "spiSend(), #2", "has callback");
??spiSend_2:
        LDR      R0,[R4, #+4]
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??spiSend_3
        LDR.N    R0,??DataTable13_27
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  364   spiStartSendI(spip, n, txbuf);
??spiSend_3:
        MOVS     R0,#+3
        STRB     R0,[R4, #+0]
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall spi_lld_send
        BL       spi_lld_send
//  365   _spi_wait_s(spip);
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BEQ.N    ??spiSend_4
        LDR.N    R0,??DataTable13_20
          CFI FunCall chDbgPanic
        BL       chDbgPanic
??spiSend_4:
        LDR.N    R0,??DataTable13_21
        LDR      R0,[R0, #+24]
        STR      R0,[R4, #+8]
        MOVS     R0,#+2
          CFI FunCall chSchGoSleepS
        BL       chSchGoSleepS
//  366   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  367 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock12
//  368 
//  369 /**
//  370  * @brief   Receives data from the SPI bus.
//  371  * @details This synchronous function performs a receive operation.
//  372  * @pre     In order to use this function the option @p SPI_USE_WAIT must be
//  373  *          enabled.
//  374  * @pre     In order to use this function the driver must have been configured
//  375  *          without callbacks (@p end_cb = @p NULL).
//  376  * @note    The buffers are organized as uint8_t arrays for data sizes below
//  377  *          or equal to 8 bits else it is organized as uint16_t arrays.
//  378  *
//  379  * @param[in] spip      pointer to the @p SPIDriver object
//  380  * @param[in] n         number of words to receive
//  381  * @param[out] rxbuf    the pointer to the receive buffer
//  382  *
//  383  * @api
//  384  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function spiReceive
        THUMB
//  385 void spiReceive(SPIDriver *spip, size_t n, void *rxbuf) {
spiReceive:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  386 
//  387   chDbgCheck((spip != NULL) && (n > 0) && (rxbuf != NULL),
//  388              "spiReceive");
        CMP      R4,#+0
        BEQ.N    ??spiReceive_0
        CMP      R5,#+0
        BEQ.N    ??spiReceive_0
        CMP      R6,#+0
        BNE.N    ??spiReceive_1
??spiReceive_0:
        MOV      R2,#+388
        LDR.N    R1,??DataTable13
        LDR.N    R0,??DataTable13_28
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  389 
//  390   chSysLock();
??spiReceive_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  391   chDbgAssert(spip->state == SPI_READY, "spiReceive(), #1", "not ready");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??spiReceive_2
        LDR.N    R0,??DataTable13_29
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  392   chDbgAssert(spip->config->end_cb == NULL,
//  393               "spiReceive(), #2", "has callback");
??spiReceive_2:
        LDR      R0,[R4, #+4]
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??spiReceive_3
        LDR.N    R0,??DataTable13_30
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  394   spiStartReceiveI(spip, n, rxbuf);
??spiReceive_3:
        MOVS     R0,#+3
        STRB     R0,[R4, #+0]
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall spi_lld_receive
        BL       spi_lld_receive
//  395   _spi_wait_s(spip);
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BEQ.N    ??spiReceive_4
        LDR.N    R0,??DataTable13_20
          CFI FunCall chDbgPanic
        BL       chDbgPanic
??spiReceive_4:
        LDR.N    R0,??DataTable13_21
        LDR      R0,[R0, #+24]
        STR      R0,[R4, #+8]
        MOVS     R0,#+2
          CFI FunCall chSchGoSleepS
        BL       chSchGoSleepS
//  396   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  397 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock13
//  398 #endif /* SPI_USE_WAIT */
//  399 
//  400 #if SPI_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
//  401 /**
//  402  * @brief   Gains exclusive access to the SPI bus.
//  403  * @details This function tries to gain ownership to the SPI bus, if the bus
//  404  *          is already being used then the invoking thread is queued.
//  405  * @pre     In order to use this function the option @p SPI_USE_MUTUAL_EXCLUSION
//  406  *          must be enabled.
//  407  *
//  408  * @param[in] spip      pointer to the @p SPIDriver object
//  409  *
//  410  * @api
//  411  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function spiAcquireBus
        THUMB
//  412 void spiAcquireBus(SPIDriver *spip) {
spiAcquireBus:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  413 
//  414   chDbgCheck(spip != NULL, "spiAcquireBus");
        CMP      R4,#+0
        BNE.N    ??spiAcquireBus_0
        MOV      R2,#+414
        LDR.N    R1,??DataTable13
        LDR.N    R0,??DataTable13_31
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  415 
//  416 #if CH_USE_MUTEXES
//  417   chMtxLock(&spip->mutex);
??spiAcquireBus_0:
        ADDS     R0,R4,#+12
          CFI FunCall chMtxLock
        BL       chMtxLock
//  418 #elif CH_USE_SEMAPHORES
//  419   chSemWait(&spip->semaphore);
//  420 #endif
//  421 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock14
//  422 
//  423 /**
//  424  * @brief   Releases exclusive access to the SPI bus.
//  425  * @pre     In order to use this function the option @p SPI_USE_MUTUAL_EXCLUSION
//  426  *          must be enabled.
//  427  *
//  428  * @param[in] spip      pointer to the @p SPIDriver object
//  429  *
//  430  * @api
//  431  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function spiReleaseBus
        THUMB
//  432 void spiReleaseBus(SPIDriver *spip) {
spiReleaseBus:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  433 
//  434   chDbgCheck(spip != NULL, "spiReleaseBus");
        CMP      R4,#+0
        BNE.N    ??spiReleaseBus_0
        MOV      R2,#+434
        LDR.N    R1,??DataTable13
        LDR.N    R0,??DataTable13_32
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  435 
//  436 #if CH_USE_MUTEXES
//  437   (void)spip;
//  438   chMtxUnlock();
??spiReleaseBus_0:
          CFI FunCall chMtxUnlock
        BL       chMtxUnlock
//  439 #elif CH_USE_SEMAPHORES
//  440   chSemSignal(&spip->semaphore);
//  441 #endif
//  442 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock15

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_1:
        DC32     `?<Constant "\\"spiStart\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_2:
        DC32     `?<Constant "spiStart(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_3:
        DC32     `?<Constant "\\"spiStop\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_4:
        DC32     `?<Constant "spiStop(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_5:
        DC32     `?<Constant "\\"spiSelect\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_6:
        DC32     `?<Constant "spiSelect(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_7:
        DC32     `?<Constant "\\"spiUnselect\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_8:
        DC32     `?<Constant "spiUnselect(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_9:
        DC32     `?<Constant "\\"spiStartIgnore\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_10:
        DC32     `?<Constant "spiStartIgnore(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_11:
        DC32     `?<Constant "\\"spiStartExchange\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_12:
        DC32     `?<Constant "spiStartExchange(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_13:
        DC32     `?<Constant "\\"spiStartSend\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_14:
        DC32     `?<Constant "spiStartSend(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_15:
        DC32     `?<Constant "\\"spiStartReceive\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_16:
        DC32     `?<Constant "spiStartReceive(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_17:
        DC32     `?<Constant "\\"spiIgnoreWait\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_18:
        DC32     `?<Constant "spiIgnore(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_19:
        DC32     `?<Constant "spiIgnore(), #2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_20:
        DC32     `?<Constant "_spi_wait(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_21:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_22:
        DC32     `?<Constant "\\"spiExchange\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_23:
        DC32     `?<Constant "spiExchange(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_24:
        DC32     `?<Constant "spiExchange(), #2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_25:
        DC32     `?<Constant "\\"spiSend\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_26:
        DC32     `?<Constant "spiSend(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_27:
        DC32     `?<Constant "spiSend(), #2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_28:
        DC32     `?<Constant "\\"spiReceive\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_29:
        DC32     `?<Constant "spiReceive(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_30:
        DC32     `?<Constant "spiReceive(), #2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_31:
        DC32     `?<Constant "\\"spiAcquireBus\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_32:
        DC32     `?<Constant "\\"spiReleaseBus\\"()">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"spiStart\\"()">`:
        DATA
        DC8 "\"spiStart\"()"
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
        DC8 73H, 70H, 69H, 2EH, 63H, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spiStart(), #1">`:
        DATA
        DC8 "spiStart(), #1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"spiStop\\"()">`:
        DATA
        DC8 "\"spiStop\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spiStop(), #1">`:
        DATA
        DC8 "spiStop(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"spiSelect\\"()">`:
        DATA
        DC8 "\"spiSelect\"()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spiSelect(), #1">`:
        DATA
        DC8 "spiSelect(), #1"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"spiUnselect\\"()">`:
        DATA
        DC8 "\"spiUnselect\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spiUnselect(), #1">`:
        DATA
        DC8 "spiUnselect(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"spiStartIgnore\\"()">`:
        DATA
        DC8 "\"spiStartIgnore\"()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spiStartIgnore(), #1">`:
        DATA
        DC8 "spiStartIgnore(), #1"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"spiStartExchange\\"()">`:
        DATA
        DC8 "\"spiStartExchange\"()"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spiStartExchange(), #1">`:
        DATA
        DC8 "spiStartExchange(), #1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"spiStartSend\\"()">`:
        DATA
        DC8 "\"spiStartSend\"()"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spiStartSend(), #1">`:
        DATA
        DC8 "spiStartSend(), #1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"spiStartReceive\\"()">`:
        DATA
        DC8 "\"spiStartReceive\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spiStartReceive(), #1">`:
        DATA
        DC8 "spiStartReceive(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"spiIgnoreWait\\"()">`:
        DATA
        DC8 "\"spiIgnoreWait\"()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spiIgnore(), #1">`:
        DATA
        DC8 "spiIgnore(), #1"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spiIgnore(), #2">`:
        DATA
        DC8 "spiIgnore(), #2"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "_spi_wait(), #1">`:
        DATA
        DC8 "_spi_wait(), #1"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"spiExchange\\"()">`:
        DATA
        DC8 "\"spiExchange\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spiExchange(), #1">`:
        DATA
        DC8 "spiExchange(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spiExchange(), #2">`:
        DATA
        DC8 "spiExchange(), #2"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"spiSend\\"()">`:
        DATA
        DC8 "\"spiSend\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spiSend(), #1">`:
        DATA
        DC8 "spiSend(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spiSend(), #2">`:
        DATA
        DC8 "spiSend(), #2"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"spiReceive\\"()">`:
        DATA
        DC8 "\"spiReceive\"()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spiReceive(), #1">`:
        DATA
        DC8 "spiReceive(), #1"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spiReceive(), #2">`:
        DATA
        DC8 "spiReceive(), #2"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"spiAcquireBus\\"()">`:
        DATA
        DC8 "\"spiAcquireBus\"()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"spiReleaseBus\\"()">`:
        DATA
        DC8 "\"spiReleaseBus\"()"
        DC8 0, 0

        END
//  443 #endif /* SPI_USE_MUTUAL_EXCLUSION */
//  444 
//  445 #endif /* HAL_USE_SPI */
//  446 
//  447 /** @} */
// 
//   632 bytes in section .rodata
// 1 302 bytes in section .text
// 
// 1 302 bytes of CODE  memory
//   632 bytes of CONST memory
//
//Errors: none
//Warnings: none
