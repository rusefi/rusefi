///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:10:01 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\serial_usb.c                                     /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\serial_usb.c -lCN F:\stuff\rusefi_sourceforge\fi /
//                       rmware\iar\Debug\List\ -lA                           /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                        -o F:\stuff\rusefi_sourceforge\firmware\iar\Debug\O /
//                       bj\ --no_cse --no_unroll --no_inline                 /
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
//                       serial_usb.s                                         /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME serial_usb

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN chDbgPanic
        EXTERN chDbgPanic3
        EXTERN chEvtBroadcastFlagsI
        EXTERN chIQGetTimeout
        EXTERN chIQInit
        EXTERN chIQReadTimeout
        EXTERN chIQResetI
        EXTERN chOQInit
        EXTERN chOQPutTimeout
        EXTERN chOQResetI
        EXTERN chOQWriteTimeout
        EXTERN chSchRescheduleS
        EXTERN dbg_check_lock
        EXTERN dbg_check_lock_from_isr
        EXTERN dbg_check_unlock
        EXTERN dbg_check_unlock_from_isr
        EXTERN usbPrepareQueuedReceive
        EXTERN usbPrepareQueuedTransmit
        EXTERN usbStartReceiveI
        EXTERN usbStartTransmitI

        PUBLIC sduConfigureHookI
        PUBLIC sduDataReceived
        PUBLIC sduDataTransmitted
        PUBLIC sduInit
        PUBLIC sduInterruptTransmitted
        PUBLIC sduObjectInit
        PUBLIC sduRequestsHook
        PUBLIC sduStart
        PUBLIC sduStop
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\src\serial_usb.c
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
//   29  * @file    serial_usb.c
//   30  * @brief   Serial over USB Driver code.
//   31  *
//   32  * @addtogroup SERIAL_USB
//   33  * @{
//   34  */
//   35 
//   36 #include "ch.h"
//   37 #include "hal.h"
//   38 
//   39 #if HAL_USE_SERIAL_USB || defined(__DOXYGEN__)
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
//   53 /*
//   54  * Current Line Coding.
//   55  */

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   56 static cdc_linecoding_t linecoding = {
linecoding:
        DATA
        DC8 0, 150, 0, 0, 0, 0, 8, 0
//   57   {0x00, 0x96, 0x00, 0x00},             /* 38400.                           */
//   58   LC_STOP_1, LC_PARITY_NONE, 8
//   59 };
//   60 
//   61 /*===========================================================================*/
//   62 /* Driver local functions.                                                   */
//   63 /*===========================================================================*/
//   64 
//   65 /*
//   66  * Interface implementation.
//   67  */
//   68 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function write
        THUMB
//   69 static size_t write(void *ip, const uint8_t *bp, size_t n) {
write:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   70 
//   71   return chOQWriteTimeout(&((SerialUSBDriver *)ip)->oqueue, bp,
//   72                           n, TIME_INFINITE);
        MOVS     R3,#-1
        MOVS     R2,R6
        MOVS     R1,R5
        ADDS     R0,R4,#+48
          CFI FunCall chOQWriteTimeout
        BL       chOQWriteTimeout
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock0
//   73 }
//   74 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function read
        THUMB
//   75 static size_t read(void *ip, uint8_t *bp, size_t n) {
read:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   76 
//   77   return chIQReadTimeout(&((SerialUSBDriver *)ip)->iqueue, bp,
//   78                          n, TIME_INFINITE);
        MOVS     R3,#-1
        MOVS     R2,R6
        MOVS     R1,R5
        ADDS     R0,R4,#+12
          CFI FunCall chIQReadTimeout
        BL       chIQReadTimeout
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock1
//   79 }
//   80 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function put
        THUMB
//   81 static msg_t put(void *ip, uint8_t b) {
put:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   82 
//   83   return chOQPutTimeout(&((SerialUSBDriver *)ip)->oqueue, b, TIME_INFINITE);
        MOVS     R2,#-1
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R0,R4,#+48
          CFI FunCall chOQPutTimeout
        BL       chOQPutTimeout
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2
//   84 }
//   85 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function get
        THUMB
//   86 static msg_t get(void *ip) {
get:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   87 
//   88   return chIQGetTimeout(&((SerialUSBDriver *)ip)->iqueue, TIME_INFINITE);
        MOVS     R1,#-1
        ADDS     R0,R4,#+12
          CFI FunCall chIQGetTimeout
        BL       chIQGetTimeout
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//   89 }
//   90 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function putt
        THUMB
//   91 static msg_t putt(void *ip, uint8_t b, systime_t timeout) {
putt:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   92 
//   93   return chOQPutTimeout(&((SerialUSBDriver *)ip)->oqueue, b, timeout);
        MOVS     R2,R6
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R0,R4,#+48
          CFI FunCall chOQPutTimeout
        BL       chOQPutTimeout
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock4
//   94 }
//   95 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function gett
        THUMB
//   96 static msg_t gett(void *ip, systime_t timeout) {
gett:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   97 
//   98   return chIQGetTimeout(&((SerialUSBDriver *)ip)->iqueue, timeout);
        MOVS     R1,R5
        ADDS     R0,R4,#+12
          CFI FunCall chIQGetTimeout
        BL       chIQGetTimeout
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock5
//   99 }
//  100 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function writet
        THUMB
//  101 static size_t writet(void *ip, const uint8_t *bp, size_t n, systime_t time) {
writet:
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
//  102 
//  103   return chOQWriteTimeout(&((SerialUSBDriver *)ip)->oqueue, bp, n, time);
        MOVS     R3,R7
        MOVS     R2,R6
        MOVS     R1,R5
        ADDS     R0,R4,#+48
          CFI FunCall chOQWriteTimeout
        BL       chOQWriteTimeout
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock6
//  104 }
//  105 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function readt
        THUMB
//  106 static size_t readt(void *ip, uint8_t *bp, size_t n, systime_t time) {
readt:
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
//  107 
//  108   return chIQReadTimeout(&((SerialUSBDriver *)ip)->iqueue, bp, n, time);
        MOVS     R3,R7
        MOVS     R2,R6
        MOVS     R1,R5
        ADDS     R0,R4,#+12
          CFI FunCall chIQReadTimeout
        BL       chIQReadTimeout
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock7
//  109 }

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"sduStart\\"()">`:
        DATA
        DC8 "\"sduStart\"()"
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
        DC8 73H, 65H, 72H, 69H, 61H, 6CH, 5FH, 75H
        DC8 73H, 62H, 2EH, 63H, 0
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "sduStart(), #1">`:
        DATA
        DC8 "sduStart(), #1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"sdStop\\"()">`:
        DATA
        DC8 "\"sdStop\"()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "sduStop(), #1">`:
        DATA
        DC8 "sduStop(), #1"
        DC8 0, 0
//  110 

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//  111 static const struct SerialUSBDriverVMT vmt = {
vmt:
        DATA
        DC32 write, read, put, get, putt, gett, writet, readt
//  112   write, read, put, get,
//  113   putt, gett, writet, readt
//  114 };
//  115 
//  116 /**
//  117  * @brief   Notification of data removed from the input queue.
//  118  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function inotify
        THUMB
//  119 static void inotify(GenericQueue *qp) {
inotify:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
//  120   size_t n, maxsize;
//  121   SerialUSBDriver *sdup = chQGetLink(qp);
        LDR      R7,[R4, #+32]
//  122 
//  123   /* If the USB driver is not in the appropriate state then transactions
//  124      must not be started.*/
//  125   if ((usbGetDriverStateI(sdup->config->usbp) != USB_ACTIVE) ||
//  126       (sdup->state != SDU_READY))
        LDR      R0,[R7, #+596]
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+0]
        CMP      R0,#+4
        BNE.N    ??inotify_0
        LDRB     R0,[R7, #+8]
        CMP      R0,#+2
        BEQ.N    ??inotify_1
//  127     return;
??inotify_0:
        B.N      ??inotify_2
//  128 
//  129   /* If there is in the queue enough space to hold at least one packet and
//  130      a transaction is not yet started then a new transaction is started for
//  131      the available space.*/
//  132   maxsize = sdup->config->usbp->epc[sdup->config->bulk_out]->out_maxsize;
??inotify_1:
        LDR      R0,[R7, #+596]
        LDRB     R0,[R0, #+5]
        LDR      R1,[R7, #+596]
        LDR      R1,[R1, #+0]
        ADDS     R0,R1,R0, LSL #+2
        LDR      R0,[R0, #+12]
        LDRH     R0,[R0, #+18]
        MOVS     R6,R0
//  133   if (!usbGetReceiveStatusI(sdup->config->usbp, sdup->config->bulk_out) &&
//  134       ((n = chIQGetEmptyI(&sdup->iqueue)) >= maxsize)) {
        LDR      R0,[R7, #+596]
        LDR      R0,[R0, #+0]
        LDRH     R0,[R0, #+10]
        MOVS     R1,#+1
        LDR      R2,[R7, #+596]
        LDRSB    R2,[R2, #+5]
        LSLS     R1,R1,R2
        TST      R0,R1
        BNE.N    ??inotify_3
        LDR      R0,[R7, #+28]
        LDR      R1,[R7, #+24]
        SUBS     R0,R0,R1
        LDR      R1,[R7, #+20]
        SUBS     R5,R0,R1
        CMP      R5,R6
        BCC.N    ??inotify_3
//  135     chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  136 
//  137     n = (n / maxsize) * maxsize;
        UDIV     R0,R5,R6
        MUL      R5,R6,R0
//  138     usbPrepareQueuedReceive(sdup->config->usbp,
//  139                             sdup->config->bulk_out,
//  140                             &sdup->iqueue, n);
        MOVS     R3,R5
        ADDS     R2,R7,#+12
        LDR      R0,[R7, #+596]
        LDRB     R1,[R0, #+5]
        LDR      R0,[R7, #+596]
        LDR      R0,[R0, #+0]
          CFI FunCall usbPrepareQueuedReceive
        BL       usbPrepareQueuedReceive
//  141 
//  142     chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  143     usbStartReceiveI(sdup->config->usbp, sdup->config->bulk_out);
        LDR      R0,[R7, #+596]
        LDRB     R1,[R0, #+5]
        LDR      R0,[R7, #+596]
        LDR      R0,[R0, #+0]
          CFI FunCall usbStartReceiveI
        BL       usbStartReceiveI
//  144   }
//  145 }
??inotify_3:
??inotify_2:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock8
//  146 
//  147 /**
//  148  * @brief   Notification of data inserted into the output queue.
//  149  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function onotify
        THUMB
//  150 static void onotify(GenericQueue *qp) {
onotify:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  151   size_t n;
//  152   SerialUSBDriver *sdup = chQGetLink(qp);
        LDR      R6,[R4, #+32]
//  153 
//  154   /* If the USB driver is not in the appropriate state then transactions
//  155      must not be started.*/
//  156   if ((usbGetDriverStateI(sdup->config->usbp) != USB_ACTIVE) ||
//  157       (sdup->state != SDU_READY))
        LDR      R0,[R6, #+596]
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+0]
        CMP      R0,#+4
        BNE.N    ??onotify_0
        LDRB     R0,[R6, #+8]
        CMP      R0,#+2
        BEQ.N    ??onotify_1
//  158     return;
??onotify_0:
        B.N      ??onotify_2
//  159 
//  160   /* If there is not an ongoing transaction and the output queue contains
//  161      data then a new transaction is started.*/
//  162   if (!usbGetTransmitStatusI(sdup->config->usbp, sdup->config->bulk_in) &&
//  163       ((n = chOQGetFullI(&sdup->oqueue)) > 0)) {
??onotify_1:
        LDR      R0,[R6, #+596]
        LDR      R0,[R0, #+0]
        LDRH     R0,[R0, #+8]
        MOVS     R1,#+1
        LDR      R2,[R6, #+596]
        LDRSB    R2,[R2, #+4]
        LSLS     R1,R1,R2
        TST      R0,R1
        BNE.N    ??onotify_3
        LDR      R0,[R6, #+64]
        LDR      R1,[R6, #+60]
        SUBS     R0,R0,R1
        LDR      R1,[R6, #+56]
        SUBS     R5,R0,R1
        CMP      R5,#+0
        BEQ.N    ??onotify_3
//  164     chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  165 
//  166     usbPrepareQueuedTransmit(sdup->config->usbp,
//  167                              sdup->config->bulk_in,
//  168                              &sdup->oqueue, n);
        MOVS     R3,R5
        ADDS     R2,R6,#+48
        LDR      R0,[R6, #+596]
        LDRB     R1,[R0, #+4]
        LDR      R0,[R6, #+596]
        LDR      R0,[R0, #+0]
          CFI FunCall usbPrepareQueuedTransmit
        BL       usbPrepareQueuedTransmit
//  169 
//  170     chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  171     usbStartTransmitI(sdup->config->usbp, sdup->config->bulk_in);
        LDR      R0,[R6, #+596]
        LDRB     R1,[R0, #+4]
        LDR      R0,[R6, #+596]
        LDR      R0,[R0, #+0]
          CFI FunCall usbStartTransmitI
        BL       usbStartTransmitI
//  172   }
//  173 }
??onotify_3:
??onotify_2:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock9
//  174 
//  175 /*===========================================================================*/
//  176 /* Driver exported functions.                                                */
//  177 /*===========================================================================*/
//  178 
//  179 /**
//  180  * @brief   Serial Driver initialization.
//  181  * @note    This function is implicitly invoked by @p halInit(), there is
//  182  *          no need to explicitly initialize the driver.
//  183  *
//  184  * @init
//  185  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function sduInit
          CFI NoCalls
        THUMB
//  186 void sduInit(void) {
//  187 }
sduInit:
        BX       LR               ;; return
          CFI EndBlock cfiBlock10
//  188 
//  189 /**
//  190  * @brief   Initializes a generic full duplex driver object.
//  191  * @details The HW dependent part of the initialization has to be performed
//  192  *          outside, usually in the hardware initialization code.
//  193  *
//  194  * @param[out] sdup     pointer to a @p SerialUSBDriver structure
//  195  *
//  196  * @init
//  197  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function sduObjectInit
        THUMB
//  198 void sduObjectInit(SerialUSBDriver *sdup) {
sduObjectInit:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        SUB      SP,SP,#+8
          CFI CFA R13+16
        MOVS     R4,R0
//  199 
//  200   sdup->vmt = &vmt;
        LDR.N    R0,??DataTable3
        STR      R0,[R4, #+0]
//  201   chEvtInit(&sdup->event);
        ADDS     R0,R4,#+4
        STR      R0,[R4, #+4]
//  202   sdup->state = SDU_STOP;
        MOVS     R0,#+1
        STRB     R0,[R4, #+8]
//  203   chIQInit(&sdup->iqueue, sdup->ib, SERIAL_USB_BUFFERS_SIZE, inotify, sdup);
        STR      R4,[SP, #+0]
        LDR.N    R3,??DataTable3_1
        MOV      R2,#+256
        ADDS     R1,R4,#+84
        ADDS     R0,R4,#+12
          CFI FunCall chIQInit
        BL       chIQInit
//  204   chOQInit(&sdup->oqueue, sdup->ob, SERIAL_USB_BUFFERS_SIZE, onotify, sdup);
        STR      R4,[SP, #+0]
        LDR.N    R3,??DataTable3_2
        MOV      R2,#+256
        ADDS     R1,R4,#+340
        ADDS     R0,R4,#+48
          CFI FunCall chOQInit
        BL       chOQInit
//  205 }
        POP      {R0,R1,R4,PC}    ;; return
          CFI EndBlock cfiBlock11
//  206 
//  207 /**
//  208  * @brief   Configures and starts the driver.
//  209  *
//  210  * @param[in] sdup      pointer to a @p SerialUSBDriver object
//  211  * @param[in] config    the serial over USB driver configuration
//  212  *
//  213  * @api
//  214  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function sduStart
        THUMB
//  215 void sduStart(SerialUSBDriver *sdup, const SerialUSBConfig *config) {
sduStart:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  216   USBDriver *usbp = config->usbp;
        LDR      R6,[R5, #+0]
//  217 
//  218   chDbgCheck(sdup != NULL, "sduStart");
        CMP      R4,#+0
        BNE.N    ??sduStart_0
        MOVS     R2,#+218
        LDR.N    R1,??DataTable3_3
        LDR.N    R0,??DataTable3_4
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  219 
//  220   chSysLock();
??sduStart_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  221   chDbgAssert((sdup->state == SDU_STOP) || (sdup->state == SDU_READY),
//  222               "sduStart(), #1",
//  223               "invalid state");
        LDRB     R0,[R4, #+8]
        CMP      R0,#+1
        BEQ.N    ??sduStart_1
        LDRB     R0,[R4, #+8]
        CMP      R0,#+2
        BEQ.N    ??sduStart_1
        LDR.N    R0,??DataTable3_5
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  224   usbp->in_params[config->bulk_in - 1]   = sdup;
??sduStart_1:
        LDRB     R0,[R5, #+4]
        ADDS     R0,R6,R0, LSL #+2
        STR      R4,[R0, #+24]
//  225   usbp->out_params[config->bulk_out - 1] = sdup;
        LDRB     R0,[R5, #+5]
        ADDS     R0,R6,R0, LSL #+2
        STR      R4,[R0, #+36]
//  226   usbp->in_params[config->int_in - 1]    = sdup;
        LDRB     R0,[R5, #+6]
        ADDS     R0,R6,R0, LSL #+2
        STR      R4,[R0, #+24]
//  227   sdup->config = config;
        STR      R5,[R4, #+596]
//  228   sdup->state = SDU_READY;
        MOVS     R0,#+2
        STRB     R0,[R4, #+8]
//  229   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  230 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock12
//  231 
//  232 /**
//  233  * @brief   Stops the driver.
//  234  * @details Any thread waiting on the driver's queues will be awakened with
//  235  *          the message @p Q_RESET.
//  236  *
//  237  * @param[in] sdup      pointer to a @p SerialUSBDriver object
//  238  *
//  239  * @api
//  240  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function sduStop
        THUMB
//  241 void sduStop(SerialUSBDriver *sdup) {
sduStop:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  242   USBDriver *usbp = sdup->config->usbp;
        LDR      R0,[R4, #+596]
        LDR      R5,[R0, #+0]
//  243 
//  244   chDbgCheck(sdup != NULL, "sdStop");
        CMP      R4,#+0
        BNE.N    ??sduStop_0
        MOVS     R2,#+244
        LDR.N    R1,??DataTable3_3
        LDR.N    R0,??DataTable3_6
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  245 
//  246   chSysLock();
??sduStop_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  247 
//  248   chDbgAssert((sdup->state == SDU_STOP) || (sdup->state == SDU_READY),
//  249               "sduStop(), #1",
//  250               "invalid state");
        LDRB     R0,[R4, #+8]
        CMP      R0,#+1
        BEQ.N    ??sduStop_1
        LDRB     R0,[R4, #+8]
        CMP      R0,#+2
        BEQ.N    ??sduStop_1
        LDR.N    R0,??DataTable3_7
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  251 
//  252   /* Driver in stopped state.*/
//  253   usbp->in_params[sdup->config->bulk_in - 1]   = NULL;
??sduStop_1:
        LDR      R0,[R4, #+596]
        LDRB     R0,[R0, #+4]
        ADDS     R0,R5,R0, LSL #+2
        MOVS     R1,#+0
        STR      R1,[R0, #+24]
//  254   usbp->out_params[sdup->config->bulk_out - 1] = NULL;
        LDR      R0,[R4, #+596]
        LDRB     R0,[R0, #+5]
        ADDS     R0,R5,R0, LSL #+2
        MOVS     R1,#+0
        STR      R1,[R0, #+36]
//  255   usbp->in_params[sdup->config->int_in - 1]    = NULL;
        LDR      R0,[R4, #+596]
        LDRB     R0,[R0, #+6]
        ADDS     R0,R5,R0, LSL #+2
        MOVS     R1,#+0
        STR      R1,[R0, #+24]
//  256   sdup->state = SDU_STOP;
        MOVS     R0,#+1
        STRB     R0,[R4, #+8]
//  257 
//  258   /* Queues reset in order to signal the driver stop to the application.*/
//  259   chnAddFlagsI(sdup, CHN_DISCONNECTED);
        MOVS     R1,#+2
        ADDS     R0,R4,#+4
          CFI FunCall chEvtBroadcastFlagsI
        BL       chEvtBroadcastFlagsI
//  260   chIQResetI(&sdup->iqueue);
        ADDS     R0,R4,#+12
          CFI FunCall chIQResetI
        BL       chIQResetI
//  261   chOQResetI(&sdup->oqueue);
        ADDS     R0,R4,#+48
          CFI FunCall chOQResetI
        BL       chOQResetI
//  262   chSchRescheduleS();
          CFI FunCall chSchRescheduleS
        BL       chSchRescheduleS
//  263 
//  264   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  265 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock13
//  266 
//  267 /**
//  268  * @brief   USB device configured handler.
//  269  *
//  270  * @param[in] sdup      pointer to a @p SerialUSBDriver object
//  271  *
//  272  * @iclass
//  273  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function sduConfigureHookI
        THUMB
//  274 void sduConfigureHookI(SerialUSBDriver *sdup) {
sduConfigureHookI:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  275   USBDriver *usbp = sdup->config->usbp;
        LDR      R0,[R4, #+596]
        LDR      R5,[R0, #+0]
//  276 
//  277   chIQResetI(&sdup->iqueue);
        ADDS     R0,R4,#+12
          CFI FunCall chIQResetI
        BL       chIQResetI
//  278   chOQResetI(&sdup->oqueue);
        ADDS     R0,R4,#+48
          CFI FunCall chOQResetI
        BL       chOQResetI
//  279   chnAddFlagsI(sdup, CHN_CONNECTED);
        MOVS     R1,#+1
        ADDS     R0,R4,#+4
          CFI FunCall chEvtBroadcastFlagsI
        BL       chEvtBroadcastFlagsI
//  280 
//  281   /* Starts the first OUT transaction immediately.*/
//  282   usbPrepareQueuedReceive(usbp, sdup->config->bulk_out, &sdup->iqueue,
//  283                           usbp->epc[sdup->config->bulk_out]->out_maxsize);
        LDR      R0,[R4, #+596]
        LDRB     R0,[R0, #+5]
        ADDS     R0,R5,R0, LSL #+2
        LDR      R0,[R0, #+12]
        LDRH     R3,[R0, #+18]
        ADDS     R2,R4,#+12
        LDR      R0,[R4, #+596]
        LDRB     R1,[R0, #+5]
        MOVS     R0,R5
          CFI FunCall usbPrepareQueuedReceive
        BL       usbPrepareQueuedReceive
//  284   usbStartReceiveI(usbp, sdup->config->bulk_out);
        LDR      R0,[R4, #+596]
        LDRB     R1,[R0, #+5]
        MOVS     R0,R5
          CFI FunCall usbStartReceiveI
        BL       usbStartReceiveI
//  285 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock14
//  286 
//  287 /**
//  288  * @brief   Default requests hook.
//  289  * @details Applications wanting to use the Serial over USB driver can use
//  290  *          this function as requests hook in the USB configuration.
//  291  *          The following requests are emulated:
//  292  *          - CDC_GET_LINE_CODING.
//  293  *          - CDC_SET_LINE_CODING.
//  294  *          - CDC_SET_CONTROL_LINE_STATE.
//  295  *          .
//  296  *
//  297  * @param[in] usbp      pointer to the @p USBDriver object
//  298  * @return              The hook status.
//  299  * @retval TRUE         Message handled internally.
//  300  * @retval FALSE        Message not handled.
//  301  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function sduRequestsHook
          CFI NoCalls
        THUMB
//  302 bool_t sduRequestsHook(USBDriver *usbp) {
sduRequestsHook:
        MOVS     R1,R0
//  303 
//  304   if ((usbp->setup[0] & USB_RTYPE_TYPE_MASK) == USB_RTYPE_TYPE_CLASS) {
        LDRB     R0,[R1, #+68]
        ANDS     R0,R0,#0x60
        CMP      R0,#+32
        BNE.N    ??sduRequestsHook_0
//  305     switch (usbp->setup[1]) {
        LDRB     R0,[R1, #+69]
        CMP      R0,#+32
        BEQ.N    ??sduRequestsHook_1
        BCC.N    ??sduRequestsHook_2
        CMP      R0,#+34
        BEQ.N    ??sduRequestsHook_3
        BCS.N    ??sduRequestsHook_2
//  306     case CDC_GET_LINE_CODING:
//  307       usbSetupTransfer(usbp, (uint8_t *)&linecoding, sizeof(linecoding), NULL);
??sduRequestsHook_4:
        LDR.N    R0,??DataTable3_8
        STR      R0,[R1, #+56]
        MOVS     R0,#+7
        STR      R0,[R1, #+60]
        MOVS     R0,#+0
        STR      R0,[R1, #+64]
//  308       return TRUE;
        MOVS     R0,#+1
        B.N      ??sduRequestsHook_5
//  309     case CDC_SET_LINE_CODING:
//  310       usbSetupTransfer(usbp, (uint8_t *)&linecoding, sizeof(linecoding), NULL);
??sduRequestsHook_1:
        LDR.N    R0,??DataTable3_8
        STR      R0,[R1, #+56]
        MOVS     R0,#+7
        STR      R0,[R1, #+60]
        MOVS     R0,#+0
        STR      R0,[R1, #+64]
//  311       return TRUE;
        MOVS     R0,#+1
        B.N      ??sduRequestsHook_5
//  312     case CDC_SET_CONTROL_LINE_STATE:
//  313       /* Nothing to do, there are no control lines.*/
//  314       usbSetupTransfer(usbp, NULL, 0, NULL);
??sduRequestsHook_3:
        MOVS     R0,#+0
        STR      R0,[R1, #+56]
        MOVS     R0,#+0
        STR      R0,[R1, #+60]
        MOVS     R0,#+0
        STR      R0,[R1, #+64]
//  315       return TRUE;
        MOVS     R0,#+1
        B.N      ??sduRequestsHook_5
//  316     default:
//  317       return FALSE;
??sduRequestsHook_2:
        MOVS     R0,#+0
        B.N      ??sduRequestsHook_5
//  318     }
//  319   }
//  320   return FALSE;
??sduRequestsHook_0:
        MOVS     R0,#+0
??sduRequestsHook_5:
        BX       LR               ;; return
          CFI EndBlock cfiBlock15
//  321 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3:
        DC32     vmt

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_1:
        DC32     inotify

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_2:
        DC32     onotify

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_3:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_4:
        DC32     `?<Constant "\\"sduStart\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_5:
        DC32     `?<Constant "sduStart(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_6:
        DC32     `?<Constant "\\"sdStop\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_7:
        DC32     `?<Constant "sduStop(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_8:
        DC32     linecoding
//  322 
//  323 /**
//  324  * @brief   Default data transmitted callback.
//  325  * @details The application must use this function as callback for the IN
//  326  *          data endpoint.
//  327  *
//  328  * @param[in] usbp      pointer to the @p USBDriver object
//  329  * @param[in] ep        endpoint number
//  330  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function sduDataTransmitted
        THUMB
//  331 void sduDataTransmitted(USBDriver *usbp, usbep_t ep) {
sduDataTransmitted:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  332   size_t n;
//  333   SerialUSBDriver *sdup = usbp->in_params[ep - 1];
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R7,[R0, #+24]
//  334 
//  335   if (sdup == NULL)
        CMP      R7,#+0
        BEQ.N    ??sduDataTransmitted_0
//  336     return;
//  337 
//  338   chSysLockFromIsr();
??sduDataTransmitted_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  339   chnAddFlagsI(sdup, CHN_OUTPUT_EMPTY);
        MOVS     R1,#+8
        ADDS     R0,R7,#+4
          CFI FunCall chEvtBroadcastFlagsI
        BL       chEvtBroadcastFlagsI
//  340 
//  341   if ((n = chOQGetFullI(&sdup->oqueue)) > 0) {
        LDR      R0,[R7, #+64]
        LDR      R1,[R7, #+60]
        SUBS     R0,R0,R1
        LDR      R1,[R7, #+56]
        SUBS     R0,R0,R1
        MOVS     R6,R0
        CMP      R0,#+0
        BEQ.N    ??sduDataTransmitted_2
//  342     /* The endpoint cannot be busy, we are in the context of the callback,
//  343        so it is safe to transmit without a check.*/
//  344     chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  345 
//  346     usbPrepareQueuedTransmit(usbp, ep, &sdup->oqueue, n);
        MOVS     R3,R6
        ADDS     R2,R7,#+48
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall usbPrepareQueuedTransmit
        BL       usbPrepareQueuedTransmit
//  347 
//  348     chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  349     usbStartTransmitI(usbp, ep);
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall usbStartTransmitI
        BL       usbStartTransmitI
        B.N      ??sduDataTransmitted_3
//  350   }
//  351   else if ((usbp->epc[ep]->in_state->txsize > 0) &&
//  352            !(usbp->epc[ep]->in_state->txsize &
//  353              (usbp->epc[ep]->in_maxsize - 1))) {
??sduDataTransmitted_2:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R0,[R0, #+20]
        LDR      R0,[R0, #+4]
        CMP      R0,#+0
        BEQ.N    ??sduDataTransmitted_3
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDR      R0,[R0, #+20]
        LDR      R0,[R0, #+4]
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R1,R4,R5, LSL #+2
        LDR      R1,[R1, #+12]
        LDRH     R1,[R1, #+16]
        SUBS     R1,R1,#+1
        TST      R0,R1
        BNE.N    ??sduDataTransmitted_3
//  354     /* Transmit zero sized packet in case the last one has maximum allowed
//  355        size. Otherwise the recipient may expect more data coming soon and
//  356        not return buffered data to app. See section 5.8.3 Bulk Transfer
//  357        Packet Size Constraints of the USB Specification document.*/
//  358     chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  359 
//  360     usbPrepareQueuedTransmit(usbp, ep, &sdup->oqueue, 0);
        MOVS     R3,#+0
        ADDS     R2,R7,#+48
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall usbPrepareQueuedTransmit
        BL       usbPrepareQueuedTransmit
//  361 
//  362     chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  363     usbStartTransmitI(usbp, ep);
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall usbStartTransmitI
        BL       usbStartTransmitI
//  364   }
//  365 
//  366   chSysUnlockFromIsr();
??sduDataTransmitted_3:
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  367 }
??sduDataTransmitted_0:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock16
//  368 
//  369 /**
//  370  * @brief   Default data received callback.
//  371  * @details The application must use this function as callback for the OUT
//  372  *          data endpoint.
//  373  *
//  374  * @param[in] usbp      pointer to the @p USBDriver object
//  375  * @param[in] ep        endpoint number
//  376  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function sduDataReceived
        THUMB
//  377 void sduDataReceived(USBDriver *usbp, usbep_t ep) {
sduDataReceived:
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
//  378   size_t n, maxsize;
//  379   SerialUSBDriver *sdup = usbp->out_params[ep - 1];
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R8,[R0, #+36]
//  380 
//  381   if (sdup == NULL)
        CMP      R8,#+0
        BEQ.N    ??sduDataReceived_0
//  382     return;
//  383 
//  384   chSysLockFromIsr();
??sduDataReceived_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  385   chnAddFlagsI(sdup, CHN_INPUT_AVAILABLE);
        MOVS     R1,#+4
        ADDS     R0,R8,#+4
          CFI FunCall chEvtBroadcastFlagsI
        BL       chEvtBroadcastFlagsI
//  386 
//  387   /* Writes to the input queue can only happen when there is enough space
//  388      to hold at least one packet.*/
//  389   maxsize = usbp->epc[ep]->out_maxsize;
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+12]
        LDRH     R0,[R0, #+18]
        MOVS     R7,R0
//  390   if ((n = chIQGetEmptyI(&sdup->iqueue)) >= maxsize) {
        LDR      R0,[R8, #+28]
        LDR      R1,[R8, #+24]
        SUBS     R0,R0,R1
        LDR      R1,[R8, #+20]
        SUBS     R0,R0,R1
        MOVS     R6,R0
        CMP      R0,R7
        BCC.N    ??sduDataReceived_2
//  391     /* The endpoint cannot be busy, we are in the context of the callback,
//  392        so a packet is in the buffer for sure.*/
//  393     chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  394 
//  395     n = (n / maxsize) * maxsize;
        UDIV     R0,R6,R7
        MUL      R6,R7,R0
//  396     usbPrepareQueuedReceive(usbp, ep, &sdup->iqueue, n);
        MOVS     R3,R6
        ADDS     R2,R8,#+12
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall usbPrepareQueuedReceive
        BL       usbPrepareQueuedReceive
//  397 
//  398     chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  399     usbStartReceiveI(usbp, ep);
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall usbStartReceiveI
        BL       usbStartReceiveI
//  400   }
//  401 
//  402   chSysUnlockFromIsr();
??sduDataReceived_2:
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  403 }
??sduDataReceived_0:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock17
//  404 
//  405 /**
//  406  * @brief   Default data received callback.
//  407  * @details The application must use this function as callback for the IN
//  408  *          interrupt endpoint.
//  409  *
//  410  * @param[in] usbp      pointer to the @p USBDriver object
//  411  * @param[in] ep        endpoint number
//  412  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function sduInterruptTransmitted
          CFI NoCalls
        THUMB
//  413 void sduInterruptTransmitted(USBDriver *usbp, usbep_t ep) {
//  414 
//  415   (void)usbp;
//  416   (void)ep;
//  417 }
sduInterruptTransmitted:
        BX       LR               ;; return
          CFI EndBlock cfiBlock18

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
//  418 
//  419 #endif /* HAL_USE_SERIAL */
//  420 
//  421 /** @} */
// 
//     8 bytes in section .data
//   164 bytes in section .rodata
// 1 310 bytes in section .text
// 
// 1 310 bytes of CODE  memory
//   164 bytes of CONST memory
//     8 bytes of DATA  memory
//
//Errors: none
//Warnings: none
