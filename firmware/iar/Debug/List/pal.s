///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:41 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\pal.c                                            /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\pal.c -lCN F:\stuff\rusefi_sourceforge\firmware\ /
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
//                       pal.s                                                /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME pal

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _pal_lld_setgroupmode
        EXTERN chDbgPanic3

        PUBLIC palReadBus
        PUBLIC palSetBusMode
        PUBLIC palWriteBus
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\src\pal.c
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
//   29  * @file    pal.c
//   30  * @brief   I/O Ports Abstraction Layer code.
//   31  *
//   32  * @addtogroup PAL
//   33  * @{
//   34  */
//   35 
//   36 #include "ch.h"
//   37 #include "hal.h"
//   38 
//   39 #if HAL_USE_PAL || defined(__DOXYGEN__)
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
//   62  * @brief   Read from an I/O bus.
//   63  * @note    The operation is not guaranteed to be atomic on all the
//   64  *          architectures, for atomicity and/or portability reasons you may
//   65  *          need to enclose port I/O operations between @p chSysLock() and
//   66  *          @p chSysUnlock().
//   67  * @note    The function internally uses the @p palReadGroup() macro. The use
//   68  *          of this function is preferred when you value code size, readability
//   69  *          and error checking over speed.
//   70  * @note    The function can be called from any context.
//   71  *
//   72  * @param[in] bus       the I/O bus, pointer to a @p IOBus structure
//   73  * @return              The bus logical states.
//   74  *
//   75  * @special
//   76  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function palReadBus
        THUMB
//   77 ioportmask_t palReadBus(IOBus *bus) {
palReadBus:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   78 
//   79   chDbgCheck((bus != NULL) && (bus->offset < PAL_IOPORTS_WIDTH),
//   80              "palReadBus");
        CMP      R4,#+0
        BEQ.N    ??palReadBus_0
        LDR      R0,[R4, #+8]
        CMP      R0,#+16
        BCC.N    ??palReadBus_1
??palReadBus_0:
        MOVS     R2,#+80
        LDR.N    R1,??DataTable2
        LDR.N    R0,??DataTable2_1
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//   81 
//   82   return palReadGroup(bus->portid, bus->mask, bus->offset);
??palReadBus_1:
        LDR      R0,[R4, #+0]
        LDR      R0,[R0, #+16]
        LDR      R1,[R4, #+8]
        LSRS     R0,R0,R1
        LDR      R1,[R4, #+4]
        ANDS     R0,R1,R0
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0
//   83 }
//   84 
//   85 /**
//   86  * @brief   Write to an I/O bus.
//   87  * @note    The operation is not guaranteed to be atomic on all the
//   88  *          architectures, for atomicity and/or portability reasons you may
//   89  *          need to enclose port I/O operations between @p chSysLock() and
//   90  *          @p chSysUnlock().
//   91  * @note    The default implementation is non atomic and not necessarily
//   92  *          optimal. Low level drivers may  optimize the function by using
//   93  *          specific hardware or coding.
//   94  * @note    The function can be called from any context.
//   95  *
//   96  * @param[in] bus       the I/O bus, pointer to a @p IOBus structure
//   97  * @param[in] bits      the bits to be written on the I/O bus. Values exceeding
//   98  *                      the bus width are masked so most significant bits are
//   99  *                      lost.
//  100  *
//  101  * @special
//  102  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function palWriteBus
        THUMB
//  103 void palWriteBus(IOBus *bus, ioportmask_t bits) {
palWriteBus:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  104 
//  105   chDbgCheck((bus != NULL) && (bus->offset < PAL_IOPORTS_WIDTH),
//  106              "palWriteBus");
        CMP      R4,#+0
        BEQ.N    ??palWriteBus_0
        LDR      R0,[R4, #+8]
        CMP      R0,#+16
        BCC.N    ??palWriteBus_1
??palWriteBus_0:
        MOVS     R2,#+106
        LDR.N    R1,??DataTable2
        LDR.N    R0,??DataTable2_2
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  107 
//  108   palWriteGroup(bus->portid, bus->mask, bus->offset, bits);
??palWriteBus_1:
        LDR      R0,[R4, #+4]
        BICS     R0,R0,R5
        LDR      R1,[R4, #+8]
        ADDS     R1,R1,#+16
        LSLS     R0,R0,R1
        LDR      R1,[R4, #+4]
        ANDS     R1,R1,R5
        LDR      R2,[R4, #+8]
        LSLS     R1,R1,R2
        ORRS     R0,R1,R0
        LDR      R1,[R4, #+0]
        STR      R0,[R1, #+24]
//  109 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock1
//  110 
//  111 /**
//  112  * @brief   Programs a bus with the specified mode.
//  113  * @note    The operation is not guaranteed to be atomic on all the
//  114  *          architectures, for atomicity and/or portability reasons you may
//  115  *          need to enclose port I/O operations between @p chSysLock() and
//  116  *          @p chSysUnlock().
//  117  * @note    The default implementation is non atomic and not necessarily
//  118  *          optimal. Low level drivers may  optimize the function by using
//  119  *          specific hardware or coding.
//  120  * @note    The function can be called from any context.
//  121  *
//  122  * @param[in] bus       the I/O bus, pointer to a @p IOBus structure
//  123  * @param[in] mode      the mode
//  124  *
//  125  * @special
//  126  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function palSetBusMode
        THUMB
//  127 void palSetBusMode(IOBus *bus, iomode_t mode) {
palSetBusMode:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  128 
//  129   chDbgCheck((bus != NULL) && (bus->offset < PAL_IOPORTS_WIDTH),
//  130              "palSetBusMode");
        CMP      R4,#+0
        BEQ.N    ??palSetBusMode_0
        LDR      R0,[R4, #+8]
        CMP      R0,#+16
        BCC.N    ??palSetBusMode_1
??palSetBusMode_0:
        MOVS     R2,#+130
        LDR.N    R1,??DataTable2
        LDR.N    R0,??DataTable2_3
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  131 
//  132   palSetGroupMode(bus->portid, bus->mask, bus->offset, mode);
??palSetBusMode_1:
        MOVS     R2,R5
        LDR      R0,[R4, #+4]
        LDR      R1,[R4, #+8]
        LSLS     R1,R0,R1
        LDR      R0,[R4, #+0]
          CFI FunCall _pal_lld_setgroupmode
        BL       _pal_lld_setgroupmode
//  133 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_1:
        DC32     `?<Constant "\\"palReadBus\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_2:
        DC32     `?<Constant "\\"palWriteBus\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_3:
        DC32     `?<Constant "\\"palSetBusMode\\"()">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"palReadBus\\"()">`:
        DATA
        DC8 "\"palReadBus\"()"
        DC8 0

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
        DC8 70H, 61H, 6CH, 2EH, 63H, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"palWriteBus\\"()">`:
        DATA
        DC8 "\"palWriteBus\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"palSetBusMode\\"()">`:
        DATA
        DC8 "\"palSetBusMode\"()"
        DC8 0, 0

        END
//  134 
//  135 #endif /* HAL_USE_PAL */
//  136 
//  137 /** @} */
// 
// 116 bytes in section .rodata
// 150 bytes in section .text
// 
// 150 bytes of CODE  memory
// 116 bytes of CONST memory
//
//Errors: none
//Warnings: none
