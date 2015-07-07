///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:07 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\serial.c                                         /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\serial.c -lCN F:\stuff\rusefi_sourceforge\firmwa /
//                       re\iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge\f /
//                       irmware\iar\Debug\List\ -o                           /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\Debug\Obj\  /
//                       --no_cse --no_unroll --no_inline --no_code_motion    /
//                       --no_tbaa --no_clustering --no_scheduling --debug    /
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
//                       serial.s                                             /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME serial

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN chDbgCheckClassI
        EXTERN chDbgPanic
        EXTERN chDbgPanic3
        EXTERN chEvtBroadcastFlagsI
        EXTERN chIQGetTimeout
        EXTERN chIQInit
        EXTERN chIQPutI
        EXTERN chIQReadTimeout
        EXTERN chIQResetI
        EXTERN chOQGetI
        EXTERN chOQInit
        EXTERN chOQPutTimeout
        EXTERN chOQResetI
        EXTERN chOQWriteTimeout
        EXTERN chSchRescheduleS
        EXTERN dbg_check_lock
        EXTERN dbg_check_unlock
        EXTERN sd_lld_init
        EXTERN sd_lld_start
        EXTERN sd_lld_stop

        PUBLIC sdIncomingDataI
        PUBLIC sdInit
        PUBLIC sdObjectInit
        PUBLIC sdRequestDataI
        PUBLIC sdStart
        PUBLIC sdStop
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\src\serial.c
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
//   29  * @file    serial.c
//   30  * @brief   Serial Driver code.
//   31  *
//   32  * @addtogroup SERIAL
//   33  * @{
//   34  */
//   35 
//   36 #include "ch.h"
//   37 #include "hal.h"
//   38 
//   39 #if HAL_USE_SERIAL || defined(__DOXYGEN__)
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
//   57 /*
//   58  * Interface implementation, the following functions just invoke the equivalent
//   59  * queue-level function or macro.
//   60  */
//   61 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function write
        THUMB
//   62 static size_t write(void *ip, const uint8_t *bp, size_t n) {
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
//   63 
//   64   return chOQWriteTimeout(&((SerialDriver *)ip)->oqueue, bp,
//   65                           n, TIME_INFINITE);
        MOVS     R3,#-1
        MOVS     R2,R6
        MOVS     R1,R5
        ADDS     R0,R4,#+48
          CFI FunCall chOQWriteTimeout
        BL       chOQWriteTimeout
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock0
//   66 }
//   67 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function read
        THUMB
//   68 static size_t read(void *ip, uint8_t *bp, size_t n) {
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
//   69 
//   70   return chIQReadTimeout(&((SerialDriver *)ip)->iqueue, bp,
//   71                          n, TIME_INFINITE);
        MOVS     R3,#-1
        MOVS     R2,R6
        MOVS     R1,R5
        ADDS     R0,R4,#+12
          CFI FunCall chIQReadTimeout
        BL       chIQReadTimeout
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock1
//   72 }
//   73 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function put
        THUMB
//   74 static msg_t put(void *ip, uint8_t b) {
put:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   75 
//   76   return chOQPutTimeout(&((SerialDriver *)ip)->oqueue, b, TIME_INFINITE);
        MOVS     R2,#-1
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R0,R4,#+48
          CFI FunCall chOQPutTimeout
        BL       chOQPutTimeout
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2
//   77 }
//   78 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function get
        THUMB
//   79 static msg_t get(void *ip) {
get:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   80 
//   81   return chIQGetTimeout(&((SerialDriver *)ip)->iqueue, TIME_INFINITE);
        MOVS     R1,#-1
        ADDS     R0,R4,#+12
          CFI FunCall chIQGetTimeout
        BL       chIQGetTimeout
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//   82 }
//   83 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function putt
        THUMB
//   84 static msg_t putt(void *ip, uint8_t b, systime_t timeout) {
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
//   85 
//   86   return chOQPutTimeout(&((SerialDriver *)ip)->oqueue, b, timeout);
        MOVS     R2,R6
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R0,R4,#+48
          CFI FunCall chOQPutTimeout
        BL       chOQPutTimeout
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock4
//   87 }
//   88 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function gett
        THUMB
//   89 static msg_t gett(void *ip, systime_t timeout) {
gett:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   90 
//   91   return chIQGetTimeout(&((SerialDriver *)ip)->iqueue, timeout);
        MOVS     R1,R5
        ADDS     R0,R4,#+12
          CFI FunCall chIQGetTimeout
        BL       chIQGetTimeout
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock5
//   92 }
//   93 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function writet
        THUMB
//   94 static size_t writet(void *ip, const uint8_t *bp, size_t n, systime_t time) {
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
//   95 
//   96   return chOQWriteTimeout(&((SerialDriver *)ip)->oqueue, bp, n, time);
        MOVS     R3,R7
        MOVS     R2,R6
        MOVS     R1,R5
        ADDS     R0,R4,#+48
          CFI FunCall chOQWriteTimeout
        BL       chOQWriteTimeout
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock6
//   97 }
//   98 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function readt
        THUMB
//   99 static size_t readt(void *ip, uint8_t *bp, size_t n, systime_t time) {
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
//  100 
//  101   return chIQReadTimeout(&((SerialDriver *)ip)->iqueue, bp, n, time);
        MOVS     R3,R7
        MOVS     R2,R6
        MOVS     R1,R5
        ADDS     R0,R4,#+12
          CFI FunCall chIQReadTimeout
        BL       chIQReadTimeout
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock7
//  102 }

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"sdStart\\"()">`:
        DATA
        DC8 "\"sdStart\"()"

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
        DC8 73H, 65H, 72H, 69H, 61H, 6CH, 2EH, 63H
        DC8 0
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "sdStart(), #1">`:
        DATA
        DC8 "sdStart(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"sdStop\\"()">`:
        DATA
        DC8 "\"sdStop\"()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "sdStop(), #1">`:
        DATA
        DC8 "sdStop(), #1"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"sdIncomingDataI\\"()">`:
        DATA
        DC8 "\"sdIncomingDataI\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"sdRequestDataI\\"()">`:
        DATA
        DC8 "\"sdRequestDataI\"()"
        DC8 0
//  103 

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//  104 static const struct SerialDriverVMT vmt = {
vmt:
        DATA
        DC32 write, read, put, get, putt, gett, writet, readt
//  105   write, read, put, get,
//  106   putt, gett, writet, readt
//  107 };
//  108 
//  109 /*===========================================================================*/
//  110 /* Driver exported functions.                                                */
//  111 /*===========================================================================*/
//  112 
//  113 /**
//  114  * @brief   Serial Driver initialization.
//  115  * @note    This function is implicitly invoked by @p halInit(), there is
//  116  *          no need to explicitly initialize the driver.
//  117  *
//  118  * @init
//  119  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function sdInit
        THUMB
//  120 void sdInit(void) {
sdInit:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  121 
//  122   sd_lld_init();
          CFI FunCall sd_lld_init
        BL       sd_lld_init
//  123 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock8
//  124 
//  125 /**
//  126  * @brief   Initializes a generic full duplex driver object.
//  127  * @details The HW dependent part of the initialization has to be performed
//  128  *          outside, usually in the hardware initialization code.
//  129  *
//  130  * @param[out] sdp      pointer to a @p SerialDriver structure
//  131  * @param[in] inotify   pointer to a callback function that is invoked when
//  132  *                      some data is read from the Queue. The value can be
//  133  *                      @p NULL.
//  134  * @param[in] onotify   pointer to a callback function that is invoked when
//  135  *                      some data is written in the Queue. The value can be
//  136  *                      @p NULL.
//  137  *
//  138  * @init
//  139  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function sdObjectInit
        THUMB
//  140 void sdObjectInit(SerialDriver *sdp, qnotify_t inotify, qnotify_t onotify) {
sdObjectInit:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+8
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  141 
//  142   sdp->vmt = &vmt;
        LDR.N    R0,??DataTable4
        STR      R0,[R4, #+0]
//  143   chEvtInit(&sdp->event);
        ADDS     R0,R4,#+4
        STR      R0,[R4, #+4]
//  144   sdp->state = SD_STOP;
        MOVS     R0,#+1
        STRB     R0,[R4, #+8]
//  145   chIQInit(&sdp->iqueue, sdp->ib, SERIAL_BUFFERS_SIZE, inotify, sdp);
        STR      R4,[SP, #+0]
        MOVS     R3,R5
        MOVS     R2,#+16
        ADDS     R1,R4,#+84
        ADDS     R0,R4,#+12
          CFI FunCall chIQInit
        BL       chIQInit
//  146   chOQInit(&sdp->oqueue, sdp->ob, SERIAL_BUFFERS_SIZE, onotify, sdp);
        STR      R4,[SP, #+0]
        MOVS     R3,R6
        MOVS     R2,#+16
        ADDS     R1,R4,#+100
        ADDS     R0,R4,#+48
          CFI FunCall chOQInit
        BL       chOQInit
//  147 }
        POP      {R0,R1,R4-R6,PC}  ;; return
          CFI EndBlock cfiBlock9
//  148 
//  149 /**
//  150  * @brief   Configures and starts the driver.
//  151  *
//  152  * @param[in] sdp       pointer to a @p SerialDriver object
//  153  * @param[in] config    the architecture-dependent serial driver configuration.
//  154  *                      If this parameter is set to @p NULL then a default
//  155  *                      configuration is used.
//  156  *
//  157  * @api
//  158  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function sdStart
        THUMB
//  159 void sdStart(SerialDriver *sdp, const SerialConfig *config) {
sdStart:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  160 
//  161   chDbgCheck(sdp != NULL, "sdStart");
        CMP      R4,#+0
        BNE.N    ??sdStart_0
        MOVS     R2,#+161
        LDR.N    R1,??DataTable4_1
        LDR.N    R0,??DataTable4_2
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  162 
//  163   chSysLock();
??sdStart_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  164   chDbgAssert((sdp->state == SD_STOP) || (sdp->state == SD_READY),
//  165               "sdStart(), #1",
//  166               "invalid state");
        LDRB     R0,[R4, #+8]
        CMP      R0,#+1
        BEQ.N    ??sdStart_1
        LDRB     R0,[R4, #+8]
        CMP      R0,#+2
        BEQ.N    ??sdStart_1
        LDR.N    R0,??DataTable4_3
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  167   sd_lld_start(sdp, config);
??sdStart_1:
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall sd_lld_start
        BL       sd_lld_start
//  168   sdp->state = SD_READY;
        MOVS     R0,#+2
        STRB     R0,[R4, #+8]
//  169   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  170 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock10
//  171 
//  172 /**
//  173  * @brief   Stops the driver.
//  174  * @details Any thread waiting on the driver's queues will be awakened with
//  175  *          the message @p Q_RESET.
//  176  *
//  177  * @param[in] sdp       pointer to a @p SerialDriver object
//  178  *
//  179  * @api
//  180  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function sdStop
        THUMB
//  181 void sdStop(SerialDriver *sdp) {
sdStop:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  182 
//  183   chDbgCheck(sdp != NULL, "sdStop");
        CMP      R4,#+0
        BNE.N    ??sdStop_0
        MOVS     R2,#+183
        LDR.N    R1,??DataTable4_1
        LDR.N    R0,??DataTable4_4
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  184 
//  185   chSysLock();
??sdStop_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  186   chDbgAssert((sdp->state == SD_STOP) || (sdp->state == SD_READY),
//  187               "sdStop(), #1",
//  188               "invalid state");
        LDRB     R0,[R4, #+8]
        CMP      R0,#+1
        BEQ.N    ??sdStop_1
        LDRB     R0,[R4, #+8]
        CMP      R0,#+2
        BEQ.N    ??sdStop_1
        LDR.N    R0,??DataTable4_5
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  189   sd_lld_stop(sdp);
??sdStop_1:
        MOVS     R0,R4
          CFI FunCall sd_lld_stop
        BL       sd_lld_stop
//  190   sdp->state = SD_STOP;
        MOVS     R0,#+1
        STRB     R0,[R4, #+8]
//  191   chOQResetI(&sdp->oqueue);
        ADDS     R0,R4,#+48
          CFI FunCall chOQResetI
        BL       chOQResetI
//  192   chIQResetI(&sdp->iqueue);
        ADDS     R0,R4,#+12
          CFI FunCall chIQResetI
        BL       chIQResetI
//  193   chSchRescheduleS();
          CFI FunCall chSchRescheduleS
        BL       chSchRescheduleS
//  194   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  195 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock11
//  196 
//  197 /**
//  198  * @brief   Handles incoming data.
//  199  * @details This function must be called from the input interrupt service
//  200  *          routine in order to enqueue incoming data and generate the
//  201  *          related events.
//  202  * @note    The incoming data event is only generated when the input queue
//  203  *          becomes non-empty.
//  204  * @note    In order to gain some performance it is suggested to not use
//  205  *          this function directly but copy this code directly into the
//  206  *          interrupt service routine.
//  207  *
//  208  * @param[in] sdp       pointer to a @p SerialDriver structure
//  209  * @param[in] b         the byte to be written in the driver's Input Queue
//  210  *
//  211  * @iclass
//  212  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function sdIncomingDataI
        THUMB
//  213 void sdIncomingDataI(SerialDriver *sdp, uint8_t b) {
sdIncomingDataI:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  214 
//  215   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  216   chDbgCheck(sdp != NULL, "sdIncomingDataI");
        CMP      R4,#+0
        BNE.N    ??sdIncomingDataI_0
        MOVS     R2,#+216
        LDR.N    R1,??DataTable4_1
        LDR.N    R0,??DataTable4_6
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  217 
//  218   if (chIQIsEmptyI(&sdp->iqueue))
??sdIncomingDataI_0:
        LDR      R0,[R4, #+20]
        CMP      R0,#+0
        BNE.N    ??sdIncomingDataI_1
//  219     chnAddFlagsI(sdp, CHN_INPUT_AVAILABLE);
        MOVS     R1,#+4
        ADDS     R0,R4,#+4
          CFI FunCall chEvtBroadcastFlagsI
        BL       chEvtBroadcastFlagsI
//  220   if (chIQPutI(&sdp->iqueue, b) < Q_OK)
??sdIncomingDataI_1:
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R0,R4,#+12
          CFI FunCall chIQPutI
        BL       chIQPutI
        CMP      R0,#+0
        BPL.N    ??sdIncomingDataI_2
//  221     chnAddFlagsI(sdp, SD_OVERRUN_ERROR);
        MOVS     R1,#+128
        ADDS     R0,R4,#+4
          CFI FunCall chEvtBroadcastFlagsI
        BL       chEvtBroadcastFlagsI
//  222 }
??sdIncomingDataI_2:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock12
//  223 
//  224 /**
//  225  * @brief   Handles outgoing data.
//  226  * @details Must be called from the output interrupt service routine in order
//  227  *          to get the next byte to be transmitted.
//  228  * @note    In order to gain some performance it is suggested to not use
//  229  *          this function directly but copy this code directly into the
//  230  *          interrupt service routine.
//  231  *
//  232  * @param[in] sdp       pointer to a @p SerialDriver structure
//  233  * @return              The byte value read from the driver's output queue.
//  234  * @retval Q_EMPTY      if the queue is empty (the lower driver usually
//  235  *                      disables the interrupt source when this happens).
//  236  *
//  237  * @iclass
//  238  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function sdRequestDataI
        THUMB
//  239 msg_t sdRequestDataI(SerialDriver *sdp) {
sdRequestDataI:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  240   msg_t  b;
//  241 
//  242   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  243   chDbgCheck(sdp != NULL, "sdRequestDataI");
        CMP      R4,#+0
        BNE.N    ??sdRequestDataI_0
        MOVS     R2,#+243
        LDR.N    R1,??DataTable4_1
        LDR.N    R0,??DataTable4_7
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  244 
//  245   b = chOQGetI(&sdp->oqueue);
??sdRequestDataI_0:
        ADDS     R0,R4,#+48
          CFI FunCall chOQGetI
        BL       chOQGetI
        MOVS     R5,R0
//  246   if (b < Q_OK)
        CMP      R5,#+0
        BPL.N    ??sdRequestDataI_1
//  247     chnAddFlagsI(sdp, CHN_OUTPUT_EMPTY);
        MOVS     R1,#+8
        ADDS     R0,R4,#+4
          CFI FunCall chEvtBroadcastFlagsI
        BL       chEvtBroadcastFlagsI
//  248   return b;
??sdRequestDataI_1:
        MOVS     R0,R5
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock13
//  249 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4:
        DC32     vmt

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_1:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_2:
        DC32     `?<Constant "\\"sdStart\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_3:
        DC32     `?<Constant "sdStart(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_4:
        DC32     `?<Constant "\\"sdStop\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_5:
        DC32     `?<Constant "sdStop(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_6:
        DC32     `?<Constant "\\"sdIncomingDataI\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_7:
        DC32     `?<Constant "\\"sdRequestDataI\\"()">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
//  250 
//  251 #endif /* HAL_USE_SERIAL */
//  252 
//  253 /** @} */
// 
// 196 bytes in section .rodata
// 560 bytes in section .text
// 
// 560 bytes of CODE  memory
// 196 bytes of CONST memory
//
//Errors: none
//Warnings: none
