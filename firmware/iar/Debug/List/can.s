///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:40 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\can.c                                            /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\can.c -lCN F:\stuff\rusefi_sourceforge\firmware\ /
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
//                       can.s                                                /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME can

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN can_lld_init
        EXTERN can_lld_is_rx_nonempty
        EXTERN can_lld_is_tx_empty
        EXTERN can_lld_receive
        EXTERN can_lld_sleep
        EXTERN can_lld_start
        EXTERN can_lld_stop
        EXTERN can_lld_transmit
        EXTERN can_lld_wakeup
        EXTERN chDbgPanic
        EXTERN chDbgPanic3
        EXTERN chEvtBroadcastFlagsI
        EXTERN chSchGoSleepTimeoutS
        EXTERN chSchRescheduleS
        EXTERN chSemInit
        EXTERN chSemResetI
        EXTERN chSemWaitTimeoutS
        EXTERN dbg_check_lock
        EXTERN dbg_check_unlock

        PUBLIC canInit
        PUBLIC canObjectInit
        PUBLIC canReceive
        PUBLIC canSleep
        PUBLIC canStart
        PUBLIC canStop
        PUBLIC canTransmit
        PUBLIC canWakeup
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\src\can.c
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
//   29  * @file    can.c
//   30  * @brief   CAN Driver code.
//   31  *
//   32  * @addtogroup CAN
//   33  * @{
//   34  */
//   35 
//   36 #include "ch.h"
//   37 #include "hal.h"
//   38 
//   39 #if HAL_USE_CAN || defined(__DOXYGEN__)
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
//   62  * @brief   CAN Driver initialization.
//   63  * @note    This function is implicitly invoked by @p halInit(), there is
//   64  *          no need to explicitly initialize the driver.
//   65  *
//   66  * @init
//   67  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function canInit
        THUMB
//   68 void canInit(void) {
canInit:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   69 
//   70   can_lld_init();
          CFI FunCall can_lld_init
        BL       can_lld_init
//   71 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0
//   72 
//   73 /**
//   74  * @brief   Initializes the standard part of a @p CANDriver structure.
//   75  *
//   76  * @param[out] canp     pointer to the @p CANDriver object
//   77  *
//   78  * @init
//   79  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function canObjectInit
        THUMB
//   80 void canObjectInit(CANDriver *canp) {
canObjectInit:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   81 
//   82   canp->state    = CAN_STOP;
        MOVS     R0,#+1
        STRB     R0,[R4, #+0]
//   83   canp->config   = NULL;
        MOVS     R0,#+0
        STR      R0,[R4, #+4]
//   84   chSemInit(&canp->txsem, 0);
        MOVS     R1,#+0
        ADDS     R0,R4,#+8
          CFI FunCall chSemInit
        BL       chSemInit
//   85   chSemInit(&canp->rxsem, 0);
        MOVS     R1,#+0
        ADDS     R0,R4,#+20
          CFI FunCall chSemInit
        BL       chSemInit
//   86   chEvtInit(&canp->rxfull_event);
        ADDS     R0,R4,#+32
        STR      R0,[R4, #+32]
//   87   chEvtInit(&canp->txempty_event);
        ADDS     R0,R4,#+36
        STR      R0,[R4, #+36]
//   88   chEvtInit(&canp->error_event);
        ADDS     R0,R4,#+40
        STR      R0,[R4, #+40]
//   89 #if CAN_USE_SLEEP_MODE
//   90   chEvtInit(&canp->sleep_event);
        ADDS     R0,R4,#+44
        STR      R0,[R4, #+44]
//   91   chEvtInit(&canp->wakeup_event);
        ADDS     R0,R4,#+48
        STR      R0,[R4, #+48]
//   92 #endif /* CAN_USE_SLEEP_MODE */
//   93 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1
//   94 
//   95 /**
//   96  * @brief   Configures and activates the CAN peripheral.
//   97  * @note    Activating the CAN bus can be a slow operation this this function
//   98  *          is not atomic, it waits internally for the initialization to
//   99  *          complete.
//  100  *
//  101  * @param[in] canp      pointer to the @p CANDriver object
//  102  * @param[in] config    pointer to the @p CANConfig object. Depending on
//  103  *                      the implementation the value can be @p NULL.
//  104  *
//  105  * @api
//  106  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function canStart
        THUMB
//  107 void canStart(CANDriver *canp, const CANConfig *config) {
canStart:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  108 
//  109   chDbgCheck(canp != NULL, "canStart");
        CMP      R4,#+0
        BNE.N    ??canStart_0
        MOVS     R2,#+109
        LDR.N    R1,??DataTable5
        LDR.N    R0,??DataTable5_1
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  110 
//  111   chSysLock();
??canStart_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  112   chDbgAssert((canp->state == CAN_STOP) ||
//  113               (canp->state == CAN_STARTING) ||
//  114               (canp->state == CAN_READY),
//  115               "canStart(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BEQ.N    ??canStart_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??canStart_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+3
        BEQ.N    ??canStart_1
        LDR.N    R0,??DataTable5_2
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  116   while (canp->state == CAN_STARTING)
??canStart_1:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BNE.N    ??canStart_2
//  117     chThdSleepS(1);
        MOVS     R1,#+1
        MOVS     R0,#+6
          CFI FunCall chSchGoSleepTimeoutS
        BL       chSchGoSleepTimeoutS
        B.N      ??canStart_1
//  118   if (canp->state == CAN_STOP) {
??canStart_2:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BNE.N    ??canStart_3
//  119     canp->config = config;
        STR      R5,[R4, #+4]
//  120     can_lld_start(canp);
        MOVS     R0,R4
          CFI FunCall can_lld_start
        BL       can_lld_start
//  121     canp->state = CAN_READY;
        MOVS     R0,#+3
        STRB     R0,[R4, #+0]
//  122   }
//  123   chSysUnlock();
??canStart_3:
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  124 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2
//  125 
//  126 /**
//  127  * @brief   Deactivates the CAN peripheral.
//  128  *
//  129  * @param[in] canp      pointer to the @p CANDriver object
//  130  *
//  131  * @api
//  132  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function canStop
        THUMB
//  133 void canStop(CANDriver *canp) {
canStop:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  134 
//  135   chDbgCheck(canp != NULL, "canStop");
        CMP      R4,#+0
        BNE.N    ??canStop_0
        MOVS     R2,#+135
        LDR.N    R1,??DataTable5
        LDR.N    R0,??DataTable5_3
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  136 
//  137   chSysLock();
??canStop_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  138   chDbgAssert((canp->state == CAN_STOP) || (canp->state == CAN_READY),
//  139               "canStop(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BEQ.N    ??canStop_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+3
        BEQ.N    ??canStop_1
        LDR.N    R0,??DataTable5_4
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  140   can_lld_stop(canp);
??canStop_1:
        MOVS     R0,R4
          CFI FunCall can_lld_stop
        BL       can_lld_stop
//  141   canp->state  = CAN_STOP;
        MOVS     R0,#+1
        STRB     R0,[R4, #+0]
//  142   chSemResetI(&canp->rxsem, 0);
        MOVS     R1,#+0
        ADDS     R0,R4,#+20
          CFI FunCall chSemResetI
        BL       chSemResetI
//  143   chSemResetI(&canp->txsem, 0);
        MOVS     R1,#+0
        ADDS     R0,R4,#+8
          CFI FunCall chSemResetI
        BL       chSemResetI
//  144   chSchRescheduleS();
          CFI FunCall chSchRescheduleS
        BL       chSchRescheduleS
//  145   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  146 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//  147 
//  148 /**
//  149  * @brief   Can frame transmission.
//  150  * @details The specified frame is queued for transmission, if the hardware
//  151  *          queue is full then the invoking thread is queued.
//  152  * @note    Trying to transmit while in sleep mode simply enqueues the thread.
//  153  *
//  154  * @param[in] canp      pointer to the @p CANDriver object
//  155  * @param[in] mailbox   mailbox number, @p CAN_ANY_MAILBOX for any mailbox
//  156  * @param[in] ctfp      pointer to the CAN frame to be transmitted
//  157  * @param[in] timeout   the number of ticks before the operation timeouts,
//  158  *                      the following special values are allowed:
//  159  *                      - @a TIME_IMMEDIATE immediate timeout.
//  160  *                      - @a TIME_INFINITE no timeout.
//  161  *                      .
//  162  * @return              The operation result.
//  163  * @retval RDY_OK       the frame has been queued for transmission.
//  164  * @retval RDY_TIMEOUT  The operation has timed out.
//  165  * @retval RDY_RESET    The driver has been stopped while waiting.
//  166  *
//  167  * @api
//  168  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function canTransmit
        THUMB
//  169 msg_t canTransmit(CANDriver *canp,
//  170                   canmbx_t mailbox,
//  171                   const CANTxFrame *ctfp,
//  172                   systime_t timeout) {
canTransmit:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
//  173 
//  174   chDbgCheck((canp != NULL) && (ctfp != NULL) && (mailbox <= CAN_TX_MAILBOXES),
//  175              "canTransmit");
        CMP      R4,#+0
        BEQ.N    ??canTransmit_0
        CMP      R6,#+0
        BEQ.N    ??canTransmit_0
        CMP      R5,#+4
        BCC.N    ??canTransmit_1
??canTransmit_0:
        MOVS     R2,#+175
        LDR.N    R1,??DataTable5
        LDR.N    R0,??DataTable5_5
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  176 
//  177   chSysLock();
??canTransmit_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  178   chDbgAssert((canp->state == CAN_READY) || (canp->state == CAN_SLEEP),
//  179               "canTransmit(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+3
        BEQ.N    ??canTransmit_2
        LDRB     R0,[R4, #+0]
        CMP      R0,#+4
        BEQ.N    ??canTransmit_2
        LDR.N    R0,??DataTable5_6
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  180   while ((canp->state == CAN_SLEEP) || !can_lld_is_tx_empty(canp, mailbox)) {
??canTransmit_2:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+4
        BEQ.N    ??canTransmit_3
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall can_lld_is_tx_empty
        BL       can_lld_is_tx_empty
        CMP      R0,#+0
        BNE.N    ??canTransmit_4
//  181     msg_t msg = chSemWaitTimeoutS(&canp->txsem, timeout);
??canTransmit_3:
        MOVS     R1,R7
        ADDS     R0,R4,#+8
          CFI FunCall chSemWaitTimeoutS
        BL       chSemWaitTimeoutS
        MOV      R8,R0
//  182     if (msg != RDY_OK) {
        CMP      R8,#+0
        BEQ.N    ??canTransmit_2
//  183       chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  184       return msg;
        MOV      R0,R8
        B.N      ??canTransmit_5
//  185     }
//  186   }
//  187   can_lld_transmit(canp, mailbox, ctfp);
??canTransmit_4:
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall can_lld_transmit
        BL       can_lld_transmit
//  188   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  189   return RDY_OK;
        MOVS     R0,#+0
??canTransmit_5:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock4
//  190 }
//  191 
//  192 /**
//  193  * @brief   Can frame receive.
//  194  * @details The function waits until a frame is received.
//  195  * @note    Trying to receive while in sleep mode simply enqueues the thread.
//  196  *
//  197  * @param[in] canp      pointer to the @p CANDriver object
//  198  * @param[in] mailbox   mailbox number, @p CAN_ANY_MAILBOX for any mailbox
//  199  * @param[out] crfp     pointer to the buffer where the CAN frame is copied
//  200  * @param[in] timeout   the number of ticks before the operation timeouts,
//  201  *                      the following special values are allowed:
//  202  *                      - @a TIME_IMMEDIATE immediate timeout (useful in an
//  203  *                        event driven scenario where a thread never blocks
//  204  *                        for I/O).
//  205  *                      - @a TIME_INFINITE no timeout.
//  206  *                      .
//  207  * @return              The operation result.
//  208  * @retval RDY_OK       a frame has been received and placed in the buffer.
//  209  * @retval RDY_TIMEOUT  The operation has timed out.
//  210  * @retval RDY_RESET    The driver has been stopped while waiting.
//  211  *
//  212  * @api
//  213  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function canReceive
        THUMB
//  214 msg_t canReceive(CANDriver *canp,
//  215                  canmbx_t mailbox,
//  216                  CANRxFrame *crfp,
//  217                  systime_t timeout) {
canReceive:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
//  218 
//  219   chDbgCheck((canp != NULL) && (crfp != NULL) && (mailbox < CAN_RX_MAILBOXES),
//  220              "canReceive");
        CMP      R4,#+0
        BEQ.N    ??canReceive_0
        CMP      R6,#+0
        BEQ.N    ??canReceive_0
        CMP      R5,#+2
        BCC.N    ??canReceive_1
??canReceive_0:
        MOVS     R2,#+220
        LDR.N    R1,??DataTable5
        LDR.N    R0,??DataTable5_7
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  221 
//  222   chSysLock();
??canReceive_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  223   chDbgAssert((canp->state == CAN_READY) || (canp->state == CAN_SLEEP),
//  224               "canReceive(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+3
        BEQ.N    ??canReceive_2
        LDRB     R0,[R4, #+0]
        CMP      R0,#+4
        BEQ.N    ??canReceive_2
        LDR.N    R0,??DataTable5_8
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  225   while ((canp->state == CAN_SLEEP) || !can_lld_is_rx_nonempty(canp, mailbox)) {
??canReceive_2:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+4
        BEQ.N    ??canReceive_3
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall can_lld_is_rx_nonempty
        BL       can_lld_is_rx_nonempty
        CMP      R0,#+0
        BNE.N    ??canReceive_4
//  226     msg_t msg = chSemWaitTimeoutS(&canp->rxsem, timeout);
??canReceive_3:
        MOVS     R1,R7
        ADDS     R0,R4,#+20
          CFI FunCall chSemWaitTimeoutS
        BL       chSemWaitTimeoutS
        MOV      R8,R0
//  227     if (msg != RDY_OK) {
        CMP      R8,#+0
        BEQ.N    ??canReceive_2
//  228       chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  229       return msg;
        MOV      R0,R8
        B.N      ??canReceive_5
//  230     }
//  231   }
//  232   can_lld_receive(canp, mailbox, crfp);
??canReceive_4:
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall can_lld_receive
        BL       can_lld_receive
//  233   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  234   return RDY_OK;
        MOVS     R0,#+0
??canReceive_5:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock5
//  235 }
//  236 
//  237 #if CAN_USE_SLEEP_MODE || defined(__DOXYGEN__)
//  238 /**
//  239  * @brief   Enters the sleep mode.
//  240  * @details This function puts the CAN driver in sleep mode and broadcasts
//  241  *          the @p sleep_event event source.
//  242  * @pre     In order to use this function the option @p CAN_USE_SLEEP_MODE must
//  243  *          be enabled and the @p CAN_SUPPORTS_SLEEP mode must be supported
//  244  *          by the low level driver.
//  245  *
//  246  * @param[in] canp      pointer to the @p CANDriver object
//  247  *
//  248  * @api
//  249  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function canSleep
        THUMB
//  250 void canSleep(CANDriver *canp) {
canSleep:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  251 
//  252   chDbgCheck(canp != NULL, "canSleep");
        CMP      R4,#+0
        BNE.N    ??canSleep_0
        MOVS     R2,#+252
        LDR.N    R1,??DataTable5
        LDR.N    R0,??DataTable5_9
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  253 
//  254   chSysLock();
??canSleep_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  255   chDbgAssert((canp->state == CAN_READY) || (canp->state == CAN_SLEEP),
//  256               "canSleep(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+3
        BEQ.N    ??canSleep_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+4
        BEQ.N    ??canSleep_1
        LDR.N    R0,??DataTable5_10
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  257   if (canp->state == CAN_READY) {
??canSleep_1:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+3
        BNE.N    ??canSleep_2
//  258     can_lld_sleep(canp);
        MOVS     R0,R4
          CFI FunCall can_lld_sleep
        BL       can_lld_sleep
//  259     canp->state = CAN_SLEEP;
        MOVS     R0,#+4
        STRB     R0,[R4, #+0]
//  260     chEvtBroadcastI(&canp->sleep_event);
        MOVS     R1,#+0
        ADDS     R0,R4,#+44
          CFI FunCall chEvtBroadcastFlagsI
        BL       chEvtBroadcastFlagsI
//  261     chSchRescheduleS();
          CFI FunCall chSchRescheduleS
        BL       chSchRescheduleS
//  262   }
//  263   chSysUnlock();
??canSleep_2:
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  264 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock6
//  265 
//  266 /**
//  267  * @brief   Enforces leaving the sleep mode.
//  268  * @note    The sleep mode is supposed to be usually exited automatically by
//  269  *          an hardware event.
//  270  *
//  271  * @param[in] canp      pointer to the @p CANDriver object
//  272  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function canWakeup
        THUMB
//  273 void canWakeup(CANDriver *canp) {
canWakeup:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  274 
//  275   chDbgCheck(canp != NULL, "canWakeup");
        CMP      R4,#+0
        BNE.N    ??canWakeup_0
        MOVW     R2,#+275
        LDR.N    R1,??DataTable5
        LDR.N    R0,??DataTable5_11
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  276 
//  277   chSysLock();
??canWakeup_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  278   chDbgAssert((canp->state == CAN_READY) || (canp->state == CAN_SLEEP),
//  279               "canWakeup(), #1", "invalid state");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+3
        BEQ.N    ??canWakeup_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+4
        BEQ.N    ??canWakeup_1
        LDR.N    R0,??DataTable5_12
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  280   if (canp->state == CAN_SLEEP) {
??canWakeup_1:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+4
        BNE.N    ??canWakeup_2
//  281     can_lld_wakeup(canp);
        MOVS     R0,R4
          CFI FunCall can_lld_wakeup
        BL       can_lld_wakeup
//  282     canp->state = CAN_READY;
        MOVS     R0,#+3
        STRB     R0,[R4, #+0]
//  283     chEvtBroadcastI(&canp->wakeup_event);
        MOVS     R1,#+0
        ADDS     R0,R4,#+48
          CFI FunCall chEvtBroadcastFlagsI
        BL       chEvtBroadcastFlagsI
//  284     chSchRescheduleS();
          CFI FunCall chSchRescheduleS
        BL       chSchRescheduleS
//  285   }
//  286   chSysUnlock();
??canWakeup_2:
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  287 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock7

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_1:
        DC32     `?<Constant "\\"canStart\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_2:
        DC32     `?<Constant "canStart(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_3:
        DC32     `?<Constant "\\"canStop\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_4:
        DC32     `?<Constant "canStop(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_5:
        DC32     `?<Constant "\\"canTransmit\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_6:
        DC32     `?<Constant "canTransmit(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_7:
        DC32     `?<Constant "\\"canReceive\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_8:
        DC32     `?<Constant "canReceive(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_9:
        DC32     `?<Constant "\\"canSleep\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_10:
        DC32     `?<Constant "canSleep(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_11:
        DC32     `?<Constant "\\"canWakeup\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_12:
        DC32     `?<Constant "canWakeup(), #1">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"canStart\\"()">`:
        DATA
        DC8 "\"canStart\"()"
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
        DC8 63H, 61H, 6EH, 2EH, 63H, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "canStart(), #1">`:
        DATA
        DC8 "canStart(), #1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"canStop\\"()">`:
        DATA
        DC8 "\"canStop\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "canStop(), #1">`:
        DATA
        DC8 "canStop(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"canTransmit\\"()">`:
        DATA
        DC8 "\"canTransmit\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "canTransmit(), #1">`:
        DATA
        DC8 "canTransmit(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"canReceive\\"()">`:
        DATA
        DC8 "\"canReceive\"()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "canReceive(), #1">`:
        DATA
        DC8 "canReceive(), #1"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"canSleep\\"()">`:
        DATA
        DC8 "\"canSleep\"()"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "canSleep(), #1">`:
        DATA
        DC8 "canSleep(), #1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"canWakeup\\"()">`:
        DATA
        DC8 "\"canWakeup\"()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "canWakeup(), #1">`:
        DATA
        DC8 "canWakeup(), #1"

        END
//  288 #endif /* CAN_USE_SLEEP_MODE */
//  289 
//  290 #endif /* HAL_USE_CAN */
//  291 
//  292 /** @} */
// 
// 260 bytes in section .rodata
// 770 bytes in section .text
// 
// 770 bytes of CODE  memory
// 260 bytes of CONST memory
//
//Errors: none
//Warnings: none
