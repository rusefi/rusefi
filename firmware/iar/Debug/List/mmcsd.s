///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:07 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\mmcsd.c                                          /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\mmcsd.c -lCN F:\stuff\rusefi_sourceforge\firmwar /
//                       e\iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge\fi /
//                       rmware\iar\Debug\List\ -o                            /
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
//                       mmcsd.s                                              /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME mmcsd

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN chDbgPanic3

        PUBLIC mmcsdGetCapacity
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\src\mmcsd.c
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
//   29  * @file    mmcsd.c
//   30  * @brief   MMC/SD cards common code.
//   31  *
//   32  * @addtogroup MMCSD
//   33  * @{
//   34  */
//   35 
//   36 #include "ch.h"
//   37 #include "hal.h"
//   38 
//   39 #if HAL_USE_MMC_SPI || HAL_USE_SDC || defined(__DOXYGEN__)
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
//   57 /**
//   58  * @brief   Gets a bit field from a words array.
//   59  * @note    The bit zero is the LSb of the first word.
//   60  *
//   61  * @param[in] data      pointer to the words array
//   62  * @param[in] end       bit offset of the last bit of the field, inclusive
//   63  * @param[in] start     bit offset of the first bit of the field, inclusive
//   64  *
//   65  * @return              The bits field value, left aligned.
//   66  *
//   67  * @notapi
//   68  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function mmcsd_get_slice
        THUMB
//   69 static uint32_t mmcsd_get_slice(uint32_t *data, uint32_t end, uint32_t start) {
mmcsd_get_slice:
        PUSH     {R4-R10,LR}
          CFI R14 Frame(CFA, -4)
          CFI R10 Frame(CFA, -8)
          CFI R9 Frame(CFA, -12)
          CFI R8 Frame(CFA, -16)
          CFI R7 Frame(CFA, -20)
          CFI R6 Frame(CFA, -24)
          CFI R5 Frame(CFA, -28)
          CFI R4 Frame(CFA, -32)
          CFI CFA R13+32
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   70   unsigned startidx, endidx, startoff;
//   71   uint32_t endmask;
//   72 
//   73   chDbgCheck((end >= start) && ((end - start) < 32), "mmcsd_get_slice");
        CMP      R5,R6
        BCC.N    ??mmcsd_get_slice_0
        SUBS     R0,R5,R6
        CMP      R0,#+32
        BCC.N    ??mmcsd_get_slice_1
??mmcsd_get_slice_0:
        MOVS     R2,#+73
        LDR.N    R1,??DataTable0
        LDR.N    R0,??DataTable0_1
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//   74 
//   75   startidx = start / 32;
??mmcsd_get_slice_1:
        LSRS     R0,R6,#+5
        MOVS     R7,R0
//   76   startoff = start % 32;
        MOVS     R0,#+32
        UDIV     R1,R6,R0
        MLS      R1,R1,R0,R6
        MOV      R9,R1
//   77   endidx   = end / 32;
        LSRS     R0,R5,#+5
        MOV      R8,R0
//   78   endmask  = (1 << ((end % 32) + 1)) - 1;
        MOVS     R0,#+1
        MOVS     R1,#+32
        UDIV     R2,R5,R1
        MLS      R2,R2,R1,R5
        ADDS     R1,R2,#+1
        LSLS     R0,R0,R1
        SUBS     R0,R0,#+1
        MOV      R10,R0
//   79 
//   80   /* One or two pieces?*/
//   81   if (startidx < endidx)
        CMP      R7,R8
        BCS.N    ??mmcsd_get_slice_2
//   82     return (data[startidx] >> startoff) |               /* Two pieces case. */
//   83            ((data[endidx] & endmask) << (32 - startoff));
        LDR      R0,[R4, R7, LSL #+2]
        LSRS     R0,R0,R9
        LDR      R1,[R4, R8, LSL #+2]
        ANDS     R1,R10,R1
        RSBS     R2,R9,#+32
        LSLS     R1,R1,R2
        ORRS     R0,R1,R0
        B.N      ??mmcsd_get_slice_3
//   84   return (data[startidx] & endmask) >> startoff;        /* One piece case.  */
??mmcsd_get_slice_2:
        LDR      R0,[R4, R7, LSL #+2]
        ANDS     R0,R10,R0
        LSRS     R0,R0,R9
??mmcsd_get_slice_3:
        POP      {R4-R10,PC}      ;; return
          CFI EndBlock cfiBlock0
//   85 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_1:
        DC32     `?<Constant "\\"mmcsd_get_slice\\"()">`
//   86 
//   87 /*===========================================================================*/
//   88 /* Driver exported functions.                                                */
//   89 /*===========================================================================*/
//   90 
//   91 /**
//   92  * @brief   Extract card capacity from a CSD.
//   93  * @details The capacity is returned as number of available blocks.
//   94  *
//   95  * @param[in] csd       the CSD record
//   96  *
//   97  * @return              The card capacity.
//   98  * @retval 0            CSD format error
//   99  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function mmcsdGetCapacity
        THUMB
//  100 uint32_t mmcsdGetCapacity(uint32_t csd[4]) {
mmcsdGetCapacity:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R5,R0
//  101 
//  102   switch (csd[3] >> 30) {
        LDR      R0,[R5, #+12]
        LSRS     R0,R0,#+30
        CMP      R0,#+0
        BEQ.N    ??mmcsdGetCapacity_0
        CMP      R0,#+1
        BEQ.N    ??mmcsdGetCapacity_1
        B.N      ??mmcsdGetCapacity_2
//  103   uint32_t a, b, c;
//  104   case 0:
//  105     /* CSD version 1.0 */
//  106     a = mmcsd_get_slice(csd, MMCSD_CSD_10_C_SIZE_SLICE);
??mmcsdGetCapacity_0:
        MOVS     R2,#+62
        MOVS     R1,#+73
        MOVS     R0,R5
          CFI FunCall mmcsd_get_slice
        BL       mmcsd_get_slice
        MOVS     R7,R0
//  107     b = mmcsd_get_slice(csd, MMCSD_CSD_10_C_SIZE_MULT_SLICE);
        MOVS     R2,#+47
        MOVS     R1,#+49
        MOVS     R0,R5
          CFI FunCall mmcsd_get_slice
        BL       mmcsd_get_slice
        MOVS     R6,R0
//  108     c = mmcsd_get_slice(csd, MMCSD_CSD_10_READ_BL_LEN_SLICE);
        MOVS     R2,#+80
        MOVS     R1,#+83
        MOVS     R0,R5
          CFI FunCall mmcsd_get_slice
        BL       mmcsd_get_slice
        MOVS     R4,R0
//  109     return (a + 1) << (b + 2) << (c - 9);       /* 2^9 == MMCSD_BLOCK_SIZE. */
        ADDS     R0,R7,#+1
        ADDS     R1,R6,#+2
        LSLS     R0,R0,R1
        SUBS     R1,R4,#+9
        LSLS     R0,R0,R1
        B.N      ??mmcsdGetCapacity_3
//  110   case 1:
//  111     /* CSD version 2.0.*/
//  112     return 1024 * (mmcsd_get_slice(csd, MMCSD_CSD_20_C_SIZE_SLICE) + 1);
??mmcsdGetCapacity_1:
        MOVS     R2,#+48
        MOVS     R1,#+69
        MOVS     R0,R5
          CFI FunCall mmcsd_get_slice
        BL       mmcsd_get_slice
        ADDS     R0,R0,#+1
        MOV      R1,#+1024
        MULS     R0,R1,R0
        B.N      ??mmcsdGetCapacity_3
//  113   default:
//  114     /* Reserved value detected.*/
//  115     return 0;
??mmcsdGetCapacity_2:
        MOVS     R0,#+0
??mmcsdGetCapacity_3:
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock1
//  116   }
//  117 }

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"mmcsd_get_slice\\"()">`:
        DATA
        DC8 "\"mmcsd_get_slice\"()"

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
        DC8 6DH, 6DH, 63H, 73H, 64H, 2EH, 63H, 0

        END
//  118 
//  119 #endif /* HAL_USE_MMC_SPI || HAL_USE_SDC */
//  120 
//  121 /** @} */
// 
//  84 bytes in section .rodata
// 216 bytes in section .text
// 
// 216 bytes of CODE  memory
//  84 bytes of CONST memory
//
//Errors: none
//Warnings: none
