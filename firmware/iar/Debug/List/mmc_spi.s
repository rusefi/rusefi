///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:56 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\mmc_spi.c                                        /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\mmc_spi.c -lCN F:\stuff\rusefi_sourceforge\firmw /
//                       are\iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge\ /
//                       firmware\iar\Debug\List\ -o                          /
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
//                       mmc_spi.s                                            /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME mmc_spi

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN chDbgPanic
        EXTERN chDbgPanic3
        EXTERN chThdSleep
        EXTERN dbg_check_lock
        EXTERN dbg_check_unlock
        EXTERN mmc_lld_is_card_inserted
        EXTERN mmc_lld_is_write_protected
        EXTERN mmcsdGetCapacity
        EXTERN spiIgnore
        EXTERN spiReceive
        EXTERN spiSelect
        EXTERN spiSend
        EXTERN spiStart
        EXTERN spiStop
        EXTERN spiUnselect

        PUBLIC mmcConnect
        PUBLIC mmcDisconnect
        PUBLIC mmcErase
        PUBLIC mmcGetInfo
        PUBLIC mmcInit
        PUBLIC mmcObjectInit
        PUBLIC mmcSequentialRead
        PUBLIC mmcSequentialWrite
        PUBLIC mmcStart
        PUBLIC mmcStartSequentialRead
        PUBLIC mmcStartSequentialWrite
        PUBLIC mmcStop
        PUBLIC mmcStopSequentialRead
        PUBLIC mmcStopSequentialWrite
        PUBLIC mmcSync
        
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
        

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"mmcStart\\"()">`:
        DATA
        DC8 "\"mmcStart\"()"
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
        DC8 6DH, 6DH, 63H, 5FH, 73H, 70H, 69H, 2EH
        DC8 63H, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "mmcStart(), #1">`:
        DATA
        DC8 "mmcStart(), #1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"mmcStop\\"()">`:
        DATA
        DC8 "\"mmcStop\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "mmcStop(), #1">`:
        DATA
        DC8 "mmcStop(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"mmcConnect\\"()">`:
        DATA
        DC8 "\"mmcConnect\"()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "mmcConnect(), #1">`:
        DATA
        DC8 "mmcConnect(), #1"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"mmcDisconnect\\"()">`:
        DATA
        DC8 "\"mmcDisconnect\"()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "mmcDisconnect(), #1">`:
        DATA
        DC8 "mmcDisconnect(), #1"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"mmcStartSequentialRe...">`:
        DATA
        DC8 "\"mmcStartSequentialRead\"()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "mmcStartSequentialRea...">`:
        DATA
        DC8 "mmcStartSequentialRead(), #1"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"mmcSequentialRead\\"()">`:
        DATA
        DC8 "\"mmcSequentialRead\"()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"mmcStopSequentialRea...">`:
        DATA
        DC8 "\"mmcStopSequentialRead\"()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"mmcStartSequentialWr...">`:
        DATA
        DC8 "\"mmcStartSequentialWrite\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "mmcStartSequentialWri...">`:
        DATA
        DC8 "mmcStartSequentialWrite(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"mmcSequentialWrite\\"()">`:
        DATA
        DC8 "\"mmcSequentialWrite\"()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"mmcStopSequentialWri...">`:
        DATA
        DC8 "\"mmcStopSequentialWrite\"()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"mmcSync\\"()">`:
        DATA
        DC8 "\"mmcSync\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"mmcGetInfo\\"()">`:
        DATA
        DC8 "\"mmcGetInfo\"()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"mmcErase\\"()">`:
        DATA
        DC8 "\"mmcErase\"()"
        DC8 0, 0, 0
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\src\mmc_spi.c
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
//   27 /*
//   28    Parts of this file have been contributed by Matthias Blaicher.
//   29  */
//   30 
//   31 /**
//   32  * @file    mmc_spi.c
//   33  * @brief   MMC over SPI driver code.
//   34  *
//   35  * @addtogroup MMC_SPI
//   36  * @{
//   37  */
//   38 
//   39 #include <string.h>
//   40 
//   41 #include "ch.h"
//   42 #include "hal.h"
//   43 
//   44 #if HAL_USE_MMC_SPI || defined(__DOXYGEN__)
//   45 
//   46 /*===========================================================================*/
//   47 /* Driver local definitions.                                                 */
//   48 /*===========================================================================*/
//   49 
//   50 /*===========================================================================*/
//   51 /* Driver exported variables.                                                */
//   52 /*===========================================================================*/
//   53 
//   54 /*===========================================================================*/
//   55 /* Driver local variables and types.                                         */
//   56 /*===========================================================================*/
//   57 
//   58 /* Forward declarations required by mmc_vmt.*/
//   59 static bool_t mmc_read(void *instance, uint32_t startblk,
//   60                        uint8_t *buffer, uint32_t n);
//   61 static bool_t mmc_write(void *instance, uint32_t startblk,
//   62                         const uint8_t *buffer, uint32_t n);
//   63 
//   64 /**
//   65  * @brief   Virtual methods table.
//   66  */

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//   67 static const struct MMCDriverVMT mmc_vmt = {
mmc_vmt:
        DATA
        DC32 mmc_lld_is_card_inserted, mmc_lld_is_write_protected, mmcConnect
        DC32 mmcDisconnect, mmc_read, mmc_write, mmcSync, mmcGetInfo
//   68   (bool_t (*)(void *))mmc_lld_is_card_inserted,
//   69   (bool_t (*)(void *))mmc_lld_is_write_protected,
//   70   (bool_t (*)(void *))mmcConnect,
//   71   (bool_t (*)(void *))mmcDisconnect,
//   72   mmc_read,
//   73   mmc_write,
//   74   (bool_t (*)(void *))mmcSync,
//   75   (bool_t (*)(void *, BlockDeviceInfo *))mmcGetInfo
//   76 };
//   77 
//   78 /**
//   79  * @brief   Lookup table for CRC-7 ( based on polynomial x^7 + x^3 + 1).
//   80  */

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//   81 static const uint8_t crc7_lookup_table[256] = {
crc7_lookup_table:
        DATA
        DC8 0, 9, 18, 27, 36, 45, 54, 63, 72, 65, 90, 83, 108, 101, 126, 119
        DC8 25, 16, 11, 2, 61, 52, 47, 38, 81, 88, 67, 74, 117, 124, 103, 110
        DC8 50, 59, 32, 41, 22, 31, 4, 13, 122, 115, 104, 97, 94, 87, 76, 69
        DC8 43, 34, 57, 48, 15, 6, 29, 20, 99, 106, 113, 120, 71, 78, 85, 92
        DC8 100, 109, 118, 127, 64, 73, 82, 91, 44, 37, 62, 55, 8, 1, 26, 19
        DC8 125, 116, 111, 102, 89, 80, 75, 66, 53, 60, 39, 46, 17, 24, 3, 10
        DC8 86, 95, 68, 77, 114, 123, 96, 105, 30, 23, 12, 5, 58, 51, 40, 33
        DC8 79, 70, 93, 84, 107, 98, 121, 112, 7, 14, 21, 28, 35, 42, 49, 56
        DC8 65, 72, 83, 90, 101, 108, 119, 126, 9, 0, 27, 18, 45, 36, 63, 54
        DC8 88, 81, 74, 67, 124, 117, 110, 103, 16, 25, 2, 11, 52, 61, 38, 47
        DC8 115, 122, 97, 104, 87, 94, 69, 76, 59, 50, 41, 32, 31, 22, 13, 4
        DC8 106, 99, 120, 113, 78, 71, 92, 85, 34, 43, 48, 57, 6, 15, 20, 29
        DC8 37, 44, 55, 62, 1, 8, 19, 26, 109, 100, 127, 118, 73, 64, 91, 82
        DC8 60, 53, 46, 39, 24, 17, 10, 3, 116, 125, 102, 111, 80, 89, 66, 75
        DC8 23, 30, 5, 12, 51, 58, 33, 40, 95, 86, 77, 68, 123, 114, 105, 96
        DC8 14, 7, 28, 21, 42, 35, 56, 49, 70, 79, 84, 93, 98, 107, 112, 121
//   82   0x00, 0x09, 0x12, 0x1b, 0x24, 0x2d, 0x36, 0x3f, 0x48, 0x41, 0x5a, 0x53,
//   83   0x6c, 0x65, 0x7e, 0x77, 0x19, 0x10, 0x0b, 0x02, 0x3d, 0x34, 0x2f, 0x26,
//   84   0x51, 0x58, 0x43, 0x4a, 0x75, 0x7c, 0x67, 0x6e, 0x32, 0x3b, 0x20, 0x29,
//   85   0x16, 0x1f, 0x04, 0x0d, 0x7a, 0x73, 0x68, 0x61, 0x5e, 0x57, 0x4c, 0x45,
//   86   0x2b, 0x22, 0x39, 0x30, 0x0f, 0x06, 0x1d, 0x14, 0x63, 0x6a, 0x71, 0x78,
//   87   0x47, 0x4e, 0x55, 0x5c, 0x64, 0x6d, 0x76, 0x7f, 0x40, 0x49, 0x52, 0x5b,
//   88   0x2c, 0x25, 0x3e, 0x37, 0x08, 0x01, 0x1a, 0x13, 0x7d, 0x74, 0x6f, 0x66,
//   89   0x59, 0x50, 0x4b, 0x42, 0x35, 0x3c, 0x27, 0x2e, 0x11, 0x18, 0x03, 0x0a,
//   90   0x56, 0x5f, 0x44, 0x4d, 0x72, 0x7b, 0x60, 0x69, 0x1e, 0x17, 0x0c, 0x05,
//   91   0x3a, 0x33, 0x28, 0x21, 0x4f, 0x46, 0x5d, 0x54, 0x6b, 0x62, 0x79, 0x70,
//   92   0x07, 0x0e, 0x15, 0x1c, 0x23, 0x2a, 0x31, 0x38, 0x41, 0x48, 0x53, 0x5a,
//   93   0x65, 0x6c, 0x77, 0x7e, 0x09, 0x00, 0x1b, 0x12, 0x2d, 0x24, 0x3f, 0x36,
//   94   0x58, 0x51, 0x4a, 0x43, 0x7c, 0x75, 0x6e, 0x67, 0x10, 0x19, 0x02, 0x0b,
//   95   0x34, 0x3d, 0x26, 0x2f, 0x73, 0x7a, 0x61, 0x68, 0x57, 0x5e, 0x45, 0x4c,
//   96   0x3b, 0x32, 0x29, 0x20, 0x1f, 0x16, 0x0d, 0x04, 0x6a, 0x63, 0x78, 0x71,
//   97   0x4e, 0x47, 0x5c, 0x55, 0x22, 0x2b, 0x30, 0x39, 0x06, 0x0f, 0x14, 0x1d,
//   98   0x25, 0x2c, 0x37, 0x3e, 0x01, 0x08, 0x13, 0x1a, 0x6d, 0x64, 0x7f, 0x76,
//   99   0x49, 0x40, 0x5b, 0x52, 0x3c, 0x35, 0x2e, 0x27, 0x18, 0x11, 0x0a, 0x03,
//  100   0x74, 0x7d, 0x66, 0x6f, 0x50, 0x59, 0x42, 0x4b, 0x17, 0x1e, 0x05, 0x0c,
//  101   0x33, 0x3a, 0x21, 0x28, 0x5f, 0x56, 0x4d, 0x44, 0x7b, 0x72, 0x69, 0x60,
//  102   0x0e, 0x07, 0x1c, 0x15, 0x2a, 0x23, 0x38, 0x31, 0x46, 0x4f, 0x54, 0x5d,
//  103   0x62, 0x6b, 0x70, 0x79
//  104 };
//  105 
//  106 /*===========================================================================*/
//  107 /* Driver local functions.                                                   */
//  108 /*===========================================================================*/
//  109 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function mmc_read
        THUMB
//  110 static bool_t mmc_read(void *instance, uint32_t startblk,
//  111                 uint8_t *buffer, uint32_t n) {
mmc_read:
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
//  112 
//  113   if (mmcStartSequentialRead((MMCDriver *)instance, startblk))
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall mmcStartSequentialRead
        BL       mmcStartSequentialRead
        CMP      R0,#+0
        BEQ.N    ??mmc_read_0
//  114     return CH_FAILED;
        MOVS     R0,#+1
        B.N      ??mmc_read_1
//  115   while (n > 0) {
??mmc_read_0:
        CMP      R7,#+0
        BEQ.N    ??mmc_read_2
//  116     if (mmcSequentialRead((MMCDriver *)instance, buffer))
        MOVS     R1,R6
        MOVS     R0,R4
          CFI FunCall mmcSequentialRead
        BL       mmcSequentialRead
        CMP      R0,#+0
        BEQ.N    ??mmc_read_3
//  117       return CH_FAILED;
        MOVS     R0,#+1
        B.N      ??mmc_read_1
//  118     buffer += MMCSD_BLOCK_SIZE;
??mmc_read_3:
        ADDS     R6,R6,#+512
//  119     n--;
        SUBS     R7,R7,#+1
        B.N      ??mmc_read_0
//  120   }
//  121   if (mmcStopSequentialRead((MMCDriver *)instance))
??mmc_read_2:
        MOVS     R0,R4
          CFI FunCall mmcStopSequentialRead
        BL       mmcStopSequentialRead
        CMP      R0,#+0
        BEQ.N    ??mmc_read_4
//  122     return CH_FAILED;
        MOVS     R0,#+1
        B.N      ??mmc_read_1
//  123   return CH_SUCCESS;
??mmc_read_4:
        MOVS     R0,#+0
??mmc_read_1:
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock0
//  124 }
//  125 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function mmc_write
        THUMB
//  126 static bool_t mmc_write(void *instance, uint32_t startblk,
//  127                  const uint8_t *buffer, uint32_t n) {
mmc_write:
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
//  128 
//  129   if (mmcStartSequentialWrite((MMCDriver *)instance, startblk))
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall mmcStartSequentialWrite
        BL       mmcStartSequentialWrite
        CMP      R0,#+0
        BEQ.N    ??mmc_write_0
//  130     return CH_FAILED;
        MOVS     R0,#+1
        B.N      ??mmc_write_1
//  131   while (n > 0) {
??mmc_write_0:
        CMP      R7,#+0
        BEQ.N    ??mmc_write_2
//  132     if (mmcSequentialWrite((MMCDriver *)instance, buffer))
        MOVS     R1,R6
        MOVS     R0,R4
          CFI FunCall mmcSequentialWrite
        BL       mmcSequentialWrite
        CMP      R0,#+0
        BEQ.N    ??mmc_write_3
//  133         return CH_FAILED;
        MOVS     R0,#+1
        B.N      ??mmc_write_1
//  134     buffer += MMCSD_BLOCK_SIZE;
??mmc_write_3:
        ADDS     R6,R6,#+512
//  135     n--;
        SUBS     R7,R7,#+1
        B.N      ??mmc_write_0
//  136   }
//  137   if (mmcStopSequentialWrite((MMCDriver *)instance))
??mmc_write_2:
        MOVS     R0,R4
          CFI FunCall mmcStopSequentialWrite
        BL       mmcStopSequentialWrite
        CMP      R0,#+0
        BEQ.N    ??mmc_write_4
//  138     return CH_FAILED;
        MOVS     R0,#+1
        B.N      ??mmc_write_1
//  139   return CH_SUCCESS;
??mmc_write_4:
        MOVS     R0,#+0
??mmc_write_1:
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock1
//  140 }
//  141 
//  142 /**
//  143  * @brief Calculate the MMC standard CRC-7 based on a lookup table.
//  144  *
//  145  * @param[in] crc       start value for CRC
//  146  * @param[in] buffer    pointer to data buffer
//  147  * @param[in] len       length of data
//  148  * @return              Calculated CRC
//  149  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function crc7
          CFI NoCalls
        THUMB
//  150 static uint8_t crc7(uint8_t crc, const uint8_t *buffer, size_t len) {
//  151 
//  152   while (len--)
crc7:
??crc7_0:
        MOVS     R3,R2
        SUBS     R2,R3,#+1
        CMP      R3,#+0
        BEQ.N    ??crc7_1
//  153     crc = crc7_lookup_table[(crc << 1) ^ (*buffer++)];
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LDRB     R3,[R1, #+0]
        EORS     R0,R3,R0, LSL #+1
        LDR.W    R3,??DataTable14
        LDRB     R0,[R0, R3]
        ADDS     R1,R1,#+1
        B.N      ??crc7_0
//  154   return crc;
??crc7_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return
          CFI EndBlock cfiBlock2
//  155 }
//  156 
//  157 /**
//  158  * @brief   Waits an idle condition.
//  159  *
//  160  * @param[in] mmcp      pointer to the @p MMCDriver object
//  161  *
//  162  * @notapi
//  163  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function wait
        THUMB
//  164 static void wait(MMCDriver *mmcp) {
wait:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  165   int i;
//  166   uint8_t buf[4];
//  167 
//  168   for (i = 0; i < 16; i++) {
        MOVS     R0,#+0
        MOVS     R5,R0
??wait_0:
        CMP      R5,#+16
        BGE.N    ??wait_1
//  169     spiReceive(mmcp->config->spip, 1, buf);
        ADD      R2,SP,#+0
        MOVS     R1,#+1
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiReceive
        BL       spiReceive
//  170     if (buf[0] == 0xFF)
        LDRB     R0,[SP, #+0]
        CMP      R0,#+255
        BEQ.N    ??wait_2
//  171       return;
//  172   }
??wait_3:
        ADDS     R5,R5,#+1
        B.N      ??wait_0
//  173   /* Looks like it is a long wait.*/
//  174   while (TRUE) {
//  175     spiReceive(mmcp->config->spip, 1, buf);
??wait_1:
        ADD      R2,SP,#+0
        MOVS     R1,#+1
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiReceive
        BL       spiReceive
//  176     if (buf[0] == 0xFF)
        LDRB     R0,[SP, #+0]
        CMP      R0,#+255
        BEQ.N    ??wait_2
//  177       break;
//  178 #if MMC_NICE_WAITING
//  179     /* Trying to be nice with the other threads.*/
//  180     chThdSleep(1);
//  181 #endif
//  182   }
//  183 }
??wait_4:
        MOVS     R0,#+1
          CFI FunCall chThdSleep
        BL       chThdSleep
        B.N      ??wait_1
??wait_2:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock3
//  184 
//  185 /**
//  186  * @brief   Sends a command header.
//  187  *
//  188  * @param[in] mmcp      pointer to the @p MMCDriver object
//  189  * @param[in] cmd       the command id
//  190  * @param[in] arg       the command argument
//  191  *
//  192  * @notapi
//  193  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function send_hdr
        THUMB
//  194 static void send_hdr(MMCDriver *mmcp, uint8_t cmd, uint32_t arg) {
send_hdr:
        PUSH     {R2-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  195   uint8_t buf[6];
//  196 
//  197   /* Wait for the bus to become idle if a write operation was in progress.*/
//  198   wait(mmcp);
        MOVS     R0,R4
          CFI FunCall wait
        BL       wait
//  199 
//  200   buf[0] = 0x40 | cmd;
        ORRS     R0,R5,#0x40
        STRB     R0,[SP, #+0]
//  201   buf[1] = arg >> 24;
        LSRS     R0,R6,#+24
        STRB     R0,[SP, #+1]
//  202   buf[2] = arg >> 16;
        LSRS     R0,R6,#+16
        STRB     R0,[SP, #+2]
//  203   buf[3] = arg >> 8;
        LSRS     R0,R6,#+8
        STRB     R0,[SP, #+3]
//  204   buf[4] = arg;
        MOVS     R0,R6
        STRB     R0,[SP, #+4]
//  205   /* Calculate CRC for command header, shift to right position, add stop bit.*/
//  206   buf[5] = ((crc7(0, buf, 5) & 0x7F) << 1) | 0x01;
        MOVS     R2,#+5
        ADD      R1,SP,#+0
        MOVS     R0,#+0
          CFI FunCall crc7
        BL       crc7
        LSLS     R0,R0,#+1
        ORRS     R0,R0,#0x1
        STRB     R0,[SP, #+5]
//  207 
//  208   spiSend(mmcp->config->spip, 6, buf);
        ADD      R2,SP,#+0
        MOVS     R1,#+6
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiSend
        BL       spiSend
//  209 }
        POP      {R0,R1,R4-R6,PC}  ;; return
          CFI EndBlock cfiBlock4
//  210 
//  211 /**
//  212  * @brief   Receives a single byte response.
//  213  *
//  214  * @param[in] mmcp      pointer to the @p MMCDriver object
//  215  * @return              The response as an @p uint8_t value.
//  216  * @retval 0xFF         timed out.
//  217  *
//  218  * @notapi
//  219  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function recvr1
        THUMB
//  220 static uint8_t recvr1(MMCDriver *mmcp) {
recvr1:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  221   int i;
//  222   uint8_t r1[1];
//  223 
//  224   for (i = 0; i < 9; i++) {
        MOVS     R0,#+0
        MOVS     R5,R0
??recvr1_0:
        CMP      R5,#+9
        BGE.N    ??recvr1_1
//  225     spiReceive(mmcp->config->spip, 1, r1);
        ADD      R2,SP,#+0
        MOVS     R1,#+1
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiReceive
        BL       spiReceive
//  226     if (r1[0] != 0xFF)
        LDRB     R0,[SP, #+0]
        CMP      R0,#+255
        BEQ.N    ??recvr1_2
//  227       return r1[0];
        LDRB     R0,[SP, #+0]
        B.N      ??recvr1_3
//  228   }
??recvr1_2:
        ADDS     R5,R5,#+1
        B.N      ??recvr1_0
//  229   return 0xFF;
??recvr1_1:
        MOVS     R0,#+255
??recvr1_3:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock5
//  230 }
//  231 
//  232 /**
//  233  * @brief   Receives a three byte response.
//  234  *
//  235  * @param[in] mmcp      pointer to the @p MMCDriver object
//  236  * @param[out] buffer   pointer to four bytes wide buffer
//  237  * @return              First response byte as an @p uint8_t value.
//  238  * @retval 0xFF         timed out.
//  239  *
//  240  * @notapi
//  241  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function recvr3
        THUMB
//  242 static uint8_t recvr3(MMCDriver *mmcp, uint8_t* buffer) {
recvr3:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  243   uint8_t r1;
//  244 
//  245   r1 = recvr1(mmcp);
        MOVS     R0,R4
          CFI FunCall recvr1
        BL       recvr1
        MOVS     R6,R0
//  246   spiReceive(mmcp->config->spip, 4, buffer);
        MOVS     R2,R5
        MOVS     R1,#+4
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiReceive
        BL       spiReceive
//  247 
//  248   return r1;
        MOVS     R0,R6
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock6
//  249 }
//  250 
//  251 /**
//  252  * @brief   Sends a command an returns a single byte response.
//  253  *
//  254  * @param[in] mmcp      pointer to the @p MMCDriver object
//  255  * @param[in] cmd       the command id
//  256  * @param[in] arg       the command argument
//  257  * @return              The response as an @p uint8_t value.
//  258  * @retval 0xFF         timed out.
//  259  *
//  260  * @notapi
//  261  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function send_command_R1
        THUMB
//  262 static uint8_t send_command_R1(MMCDriver *mmcp, uint8_t cmd, uint32_t arg) {
send_command_R1:
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
//  263   uint8_t r1;
//  264 
//  265   spiSelect(mmcp->config->spip);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiSelect
        BL       spiSelect
//  266   send_hdr(mmcp, cmd, arg);
        MOVS     R2,R6
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall send_hdr
        BL       send_hdr
//  267   r1 = recvr1(mmcp);
        MOVS     R0,R4
          CFI FunCall recvr1
        BL       recvr1
        MOVS     R7,R0
//  268   spiUnselect(mmcp->config->spip);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiUnselect
        BL       spiUnselect
//  269   return r1;
        MOVS     R0,R7
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock7
//  270 }
//  271 
//  272 /**
//  273  * @brief   Sends a command which returns a five bytes response (R3).
//  274  *
//  275  * @param[in] mmcp      pointer to the @p MMCDriver object
//  276  * @param[in] cmd       the command id
//  277  * @param[in] arg       the command argument
//  278  * @param[out] response pointer to four bytes wide uint8_t buffer
//  279  * @return              The first byte of the response (R1) as an @p
//  280  *                      uint8_t value.
//  281  * @retval 0xFF         timed out.
//  282  *
//  283  * @notapi
//  284  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function send_command_R3
        THUMB
//  285 static uint8_t send_command_R3(MMCDriver *mmcp, uint8_t cmd, uint32_t arg,
//  286                                uint8_t *response) {
send_command_R3:
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
//  287   uint8_t r1;
//  288 
//  289   spiSelect(mmcp->config->spip);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiSelect
        BL       spiSelect
//  290   send_hdr(mmcp, cmd, arg);
        MOVS     R2,R6
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall send_hdr
        BL       send_hdr
//  291   r1 = recvr3(mmcp, response);
        MOVS     R1,R7
        MOVS     R0,R4
          CFI FunCall recvr3
        BL       recvr3
        MOV      R8,R0
//  292   spiUnselect(mmcp->config->spip);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiUnselect
        BL       spiUnselect
//  293   return r1;
        MOV      R0,R8
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock8
//  294 }
//  295 
//  296 /**
//  297  * @brief   Reads the CSD.
//  298  *
//  299  * @param[in] mmcp      pointer to the @p MMCDriver object
//  300  * @param[out] csd       pointer to the CSD buffer
//  301  *
//  302  * @return              The operation status.
//  303  * @retval CH_SUCCESS   the operation succeeded.
//  304  * @retval CH_FAILED    the operation failed.
//  305  *
//  306  * @notapi
//  307  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function read_CxD
        THUMB
//  308 static bool_t read_CxD(MMCDriver *mmcp, uint8_t cmd, uint32_t cxd[4]) {
read_CxD:
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
//  309   unsigned i;
//  310   uint8_t *bp, buf[16];
//  311 
//  312   spiSelect(mmcp->config->spip);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiSelect
        BL       spiSelect
//  313   send_hdr(mmcp, cmd, 0);
        MOVS     R2,#+0
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall send_hdr
        BL       send_hdr
//  314   if (recvr1(mmcp) != 0x00) {
        MOVS     R0,R4
          CFI FunCall recvr1
        BL       recvr1
        CMP      R0,#+0
        BEQ.N    ??read_CxD_0
//  315     spiUnselect(mmcp->config->spip);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiUnselect
        BL       spiUnselect
//  316     return CH_FAILED;
        MOVS     R0,#+1
        B.N      ??read_CxD_1
//  317   }
//  318 
//  319   /* Wait for data availability.*/
//  320   for (i = 0; i < MMC_WAIT_DATA; i++) {
??read_CxD_0:
        MOVS     R0,#+0
        MOVS     R7,R0
??read_CxD_2:
        MOVW     R0,#+10000
        CMP      R7,R0
        BCS.N    ??read_CxD_3
//  321     spiReceive(mmcp->config->spip, 1, buf);
        ADD      R2,SP,#+0
        MOVS     R1,#+1
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiReceive
        BL       spiReceive
//  322     if (buf[0] == 0xFE) {
        LDRB     R0,[SP, #+0]
        CMP      R0,#+254
        BNE.N    ??read_CxD_4
//  323       uint32_t *wp;
//  324 
//  325       spiReceive(mmcp->config->spip, 16, buf);
        ADD      R2,SP,#+0
        MOVS     R1,#+16
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiReceive
        BL       spiReceive
//  326       bp = buf;
        ADD      R0,SP,#+0
        MOV      R8,R0
//  327       for (wp = &cxd[3]; wp >= cxd; wp--) {
        ADDS     R0,R6,#+12
        MOV      R9,R0
??read_CxD_5:
        CMP      R9,R6
        BCC.N    ??read_CxD_6
//  328         *wp = ((uint32_t)bp[0] << 24) | ((uint32_t)bp[1] << 16) |
//  329               ((uint32_t)bp[2] << 8)  | (uint32_t)bp[3];
        LDRB     R0,[R8, #+0]
        LDRB     R1,[R8, #+1]
        LSLS     R1,R1,#+16
        ORRS     R0,R1,R0, LSL #+24
        LDRB     R1,[R8, #+2]
        ORRS     R0,R0,R1, LSL #+8
        LDRB     R1,[R8, #+3]
        ORRS     R0,R1,R0
        STR      R0,[R9, #+0]
//  330         bp += 4;
        ADDS     R8,R8,#+4
//  331       }
        SUBS     R9,R9,#+4
        B.N      ??read_CxD_5
//  332 
//  333       /* CRC ignored then end of transaction. */
//  334       spiIgnore(mmcp->config->spip, 2);
??read_CxD_6:
        MOVS     R1,#+2
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiIgnore
        BL       spiIgnore
//  335       spiUnselect(mmcp->config->spip);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiUnselect
        BL       spiUnselect
//  336 
//  337       return CH_SUCCESS;
        MOVS     R0,#+0
        B.N      ??read_CxD_1
//  338     }
//  339   }
??read_CxD_4:
        ADDS     R7,R7,#+1
        B.N      ??read_CxD_2
//  340   return CH_FAILED;
??read_CxD_3:
        MOVS     R0,#+1
??read_CxD_1:
        ADD      SP,SP,#+20
          CFI CFA R13+28
        POP      {R4-R9,PC}       ;; return
          CFI EndBlock cfiBlock9
//  341 }
//  342 
//  343 /**
//  344  * @brief   Waits that the card reaches an idle state.
//  345  *
//  346  * @param[in] mmcp      pointer to the @p MMCDriver object
//  347  *
//  348  * @notapi
//  349  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function sync
        THUMB
//  350 static void sync(MMCDriver *mmcp) {
sync:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        SUB      SP,SP,#+8
          CFI CFA R13+16
        MOVS     R4,R0
//  351   uint8_t buf[1];
//  352 
//  353   spiSelect(mmcp->config->spip);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiSelect
        BL       spiSelect
//  354   while (TRUE) {
//  355     spiReceive(mmcp->config->spip, 1, buf);
??sync_0:
        ADD      R2,SP,#+0
        MOVS     R1,#+1
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiReceive
        BL       spiReceive
//  356     if (buf[0] == 0xFF)
        LDRB     R0,[SP, #+0]
        CMP      R0,#+255
        BNE.N    ??sync_1
//  357       break;
//  358 #if MMC_NICE_WAITING
//  359     chThdSleep(1);      /* Trying to be nice with the other threads.*/
//  360 #endif
//  361   }
//  362   spiUnselect(mmcp->config->spip);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiUnselect
        BL       spiUnselect
//  363 }
        B.N      ??sync_2
??sync_1:
        MOVS     R0,#+1
          CFI FunCall chThdSleep
        BL       chThdSleep
        B.N      ??sync_0
??sync_2:
        POP      {R0,R1,R4,PC}    ;; return
          CFI EndBlock cfiBlock10
//  364 
//  365 /*===========================================================================*/
//  366 /* Driver exported functions.                                                */
//  367 /*===========================================================================*/
//  368 
//  369 /**
//  370  * @brief   MMC over SPI driver initialization.
//  371  * @note    This function is implicitly invoked by @p halInit(), there is
//  372  *          no need to explicitly initialize the driver.
//  373  *
//  374  * @init
//  375  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function mmcInit
          CFI NoCalls
        THUMB
//  376 void mmcInit(void) {
//  377 
//  378 }
mmcInit:
        BX       LR               ;; return
          CFI EndBlock cfiBlock11
//  379 
//  380 /**
//  381  * @brief   Initializes an instance.
//  382  *
//  383  * @param[out] mmcp         pointer to the @p MMCDriver object
//  384  *
//  385  * @init
//  386  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function mmcObjectInit
          CFI NoCalls
        THUMB
//  387 void mmcObjectInit(MMCDriver *mmcp) {
//  388 
//  389   mmcp->vmt = &mmc_vmt;
mmcObjectInit:
        LDR.W    R1,??DataTable14_1
        STR      R1,[R0, #+0]
//  390   mmcp->state = BLK_STOP;
        MOVS     R1,#+1
        STRB     R1,[R0, #+4]
//  391   mmcp->config = NULL;
        MOVS     R1,#+0
        STR      R1,[R0, #+44]
//  392   mmcp->block_addresses = FALSE;
        MOVS     R1,#+0
        STRB     R1,[R0, #+48]
//  393 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock12
//  394 
//  395 /**
//  396  * @brief   Configures and activates the MMC peripheral.
//  397  *
//  398  * @param[in] mmcp      pointer to the @p MMCDriver object
//  399  * @param[in] config    pointer to the @p MMCConfig object.
//  400  *
//  401  * @api
//  402  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function mmcStart
        THUMB
//  403 void mmcStart(MMCDriver *mmcp, const MMCConfig *config) {
mmcStart:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  404 
//  405   chDbgCheck((mmcp != NULL) && (config != NULL), "mmcStart");
        CMP      R4,#+0
        BEQ.N    ??mmcStart_0
        CMP      R5,#+0
        BNE.N    ??mmcStart_1
??mmcStart_0:
        MOVW     R2,#+405
        LDR.W    R1,??DataTable14_2
        LDR.W    R0,??DataTable14_3
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  406   chDbgAssert((mmcp->state == BLK_STOP) || (mmcp->state == BLK_ACTIVE),
//  407               "mmcStart(), #1", "invalid state");
??mmcStart_1:
        LDRB     R0,[R4, #+4]
        CMP      R0,#+1
        BEQ.N    ??mmcStart_2
        LDRB     R0,[R4, #+4]
        CMP      R0,#+2
        BEQ.N    ??mmcStart_2
        LDR.W    R0,??DataTable14_4
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  408 
//  409   mmcp->config = config;
??mmcStart_2:
        STR      R5,[R4, #+44]
//  410   mmcp->state = BLK_ACTIVE;
        MOVS     R0,#+2
        STRB     R0,[R4, #+4]
//  411 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock13
//  412 
//  413 /**
//  414  * @brief   Disables the MMC peripheral.
//  415  *
//  416  * @param[in] mmcp      pointer to the @p MMCDriver object
//  417  *
//  418  * @api
//  419  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function mmcStop
        THUMB
//  420 void mmcStop(MMCDriver *mmcp) {
mmcStop:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  421 
//  422   chDbgCheck(mmcp != NULL, "mmcStop");
        CMP      R4,#+0
        BNE.N    ??mmcStop_0
        MOV      R2,#+422
        LDR.W    R1,??DataTable14_2
        LDR.W    R0,??DataTable14_5
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  423   chDbgAssert((mmcp->state == BLK_STOP) || (mmcp->state == BLK_ACTIVE),
//  424               "mmcStop(), #1", "invalid state");
??mmcStop_0:
        LDRB     R0,[R4, #+4]
        CMP      R0,#+1
        BEQ.N    ??mmcStop_1
        LDRB     R0,[R4, #+4]
        CMP      R0,#+2
        BEQ.N    ??mmcStop_1
        LDR.W    R0,??DataTable14_6
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  425 
//  426   spiStop(mmcp->config->spip);
??mmcStop_1:
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiStop
        BL       spiStop
//  427   mmcp->state = BLK_STOP;
        MOVS     R0,#+1
        STRB     R0,[R4, #+4]
//  428 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock14
//  429 
//  430 /**
//  431  * @brief   Performs the initialization procedure on the inserted card.
//  432  * @details This function should be invoked when a card is inserted and
//  433  *          brings the driver in the @p MMC_READY state where it is possible
//  434  *          to perform read and write operations.
//  435  * @note    It is possible to invoke this function from the insertion event
//  436  *          handler.
//  437  *
//  438  * @param[in] mmcp      pointer to the @p MMCDriver object
//  439  *
//  440  * @return              The operation status.
//  441  * @retval CH_SUCCESS   the operation succeeded and the driver is now
//  442  *                      in the @p MMC_READY state.
//  443  * @retval CH_FAILED    the operation failed.
//  444  *
//  445  * @api
//  446  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function mmcConnect
        THUMB
//  447 bool_t mmcConnect(MMCDriver *mmcp) {
mmcConnect:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+8
          CFI CFA R13+24
        MOVS     R4,R0
//  448   unsigned i;
//  449   uint8_t r3[4];
//  450 
//  451   chDbgCheck(mmcp != NULL, "mmcConnect");
        CMP      R4,#+0
        BNE.N    ??mmcConnect_0
        MOVW     R2,#+451
        LDR.W    R1,??DataTable14_2
        LDR.W    R0,??DataTable14_7
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  452 
//  453   chDbgAssert((mmcp->state == BLK_ACTIVE) || (mmcp->state == BLK_READY),
//  454               "mmcConnect(), #1", "invalid state");
??mmcConnect_0:
        LDRB     R0,[R4, #+4]
        CMP      R0,#+2
        BEQ.N    ??mmcConnect_1
        LDRB     R0,[R4, #+4]
        CMP      R0,#+5
        BEQ.N    ??mmcConnect_1
        LDR.W    R0,??DataTable14_8
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  455 
//  456   /* Connection procedure in progress.*/
//  457   mmcp->state = BLK_CONNECTING;
??mmcConnect_1:
        MOVS     R0,#+3
        STRB     R0,[R4, #+4]
//  458   mmcp->block_addresses = FALSE;
        MOVS     R0,#+0
        STRB     R0,[R4, #+48]
//  459 
//  460   /* Slow clock mode and 128 clock pulses.*/
//  461   spiStart(mmcp->config->spip, mmcp->config->lscfg);
        LDR      R0,[R4, #+44]
        LDR      R1,[R0, #+4]
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiStart
        BL       spiStart
//  462   spiIgnore(mmcp->config->spip, 16);
        MOVS     R1,#+16
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiIgnore
        BL       spiIgnore
//  463 
//  464   /* SPI mode selection.*/
//  465   i = 0;
        MOVS     R0,#+0
        MOVS     R5,R0
//  466   while (TRUE) {
//  467     if (send_command_R1(mmcp, MMCSD_CMD_GO_IDLE_STATE, 0) == 0x01)
??mmcConnect_2:
        MOVS     R2,#+0
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall send_command_R1
        BL       send_command_R1
        CMP      R0,#+1
        BNE.N    ??mmcConnect_3
//  468       break;
//  469     if (++i >= MMC_CMD0_RETRY)
//  470       goto failed;
//  471     chThdSleepMilliseconds(10);
//  472   }
//  473 
//  474   /* Try to detect if this is a high capacity card and switch to block
//  475      addresses if possible.
//  476      This method is based on "How to support SDC Ver2 and high capacity cards"
//  477      by ElmChan.*/
//  478   if (send_command_R3(mmcp, MMCSD_CMD_SEND_IF_COND,
//  479                       MMCSD_CMD8_PATTERN, r3) != 0x05) {
        ADD      R3,SP,#+0
        MOV      R2,#+426
        MOVS     R1,#+8
        MOVS     R0,R4
          CFI FunCall send_command_R3
        BL       send_command_R3
        CMP      R0,#+5
        BNE.N    ??mmcConnect_4
        B.N      ??mmcConnect_5
??mmcConnect_3:
        ADDS     R5,R5,#+1
        CMP      R5,#+10
        BCS.N    ??mmcConnect_6
??mmcConnect_7:
        MOVS     R0,#+10
          CFI FunCall chThdSleep
        BL       chThdSleep
        B.N      ??mmcConnect_2
//  480 
//  481     /* Switch to SDHC mode.*/
//  482     i = 0;
??mmcConnect_4:
        MOVS     R0,#+0
        MOVS     R5,R0
//  483     while (TRUE) {
//  484       if ((send_command_R1(mmcp, MMCSD_CMD_APP_CMD, 0) == 0x01) &&
//  485           (send_command_R3(mmcp, MMCSD_CMD_APP_OP_COND,
//  486                            0x400001aa, r3) == 0x00))
??mmcConnect_8:
        MOVS     R2,#+0
        MOVS     R1,#+55
        MOVS     R0,R4
          CFI FunCall send_command_R1
        BL       send_command_R1
        CMP      R0,#+1
        BNE.N    ??mmcConnect_9
        ADD      R3,SP,#+0
        LDR.W    R2,??DataTable14_9  ;; 0x400001aa
        MOVS     R1,#+41
        MOVS     R0,R4
          CFI FunCall send_command_R3
        BL       send_command_R3
        CMP      R0,#+0
        BNE.N    ??mmcConnect_9
//  487         break;
//  488 
//  489       if (++i >= MMC_ACMD41_RETRY)
//  490         goto failed;
//  491       chThdSleepMilliseconds(10);
//  492     }
//  493 
//  494     /* Execute dedicated read on OCR register */
//  495     send_command_R3(mmcp, MMCSD_CMD_READ_OCR, 0, r3);
        ADD      R3,SP,#+0
        MOVS     R2,#+0
        MOVS     R1,#+58
        MOVS     R0,R4
          CFI FunCall send_command_R3
        BL       send_command_R3
//  496 
//  497     /* Check if CCS is set in response. Card operates in block mode if set.*/
//  498     if (r3[0] & 0x40)
        LDRB     R0,[SP, #+0]
        LSLS     R0,R0,#+25
        BMI.N    ??mmcConnect_10
        B.N      ??mmcConnect_5
??mmcConnect_9:
        ADDS     R5,R5,#+1
        CMP      R5,#+100
        BCS.N    ??mmcConnect_6
??mmcConnect_11:
        MOVS     R0,#+10
          CFI FunCall chThdSleep
        BL       chThdSleep
        B.N      ??mmcConnect_8
//  499       mmcp->block_addresses = TRUE;
??mmcConnect_10:
        MOVS     R0,#+1
        STRB     R0,[R4, #+48]
//  500   }
//  501 
//  502   /* Initialization.*/
//  503   i = 0;
??mmcConnect_5:
        MOVS     R0,#+0
        MOVS     R5,R0
//  504   while (TRUE) {
//  505     uint8_t b = send_command_R1(mmcp, MMCSD_CMD_INIT, 0);
??mmcConnect_12:
        MOVS     R2,#+0
        MOVS     R1,#+1
        MOVS     R0,R4
          CFI FunCall send_command_R1
        BL       send_command_R1
        MOVS     R6,R0
//  506     if (b == 0x00)
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+0
        BNE.N    ??mmcConnect_13
//  507       break;
//  508     if (b != 0x01)
//  509       goto failed;
//  510     if (++i >= MMC_CMD1_RETRY)
//  511       goto failed;
//  512     chThdSleepMilliseconds(10);
//  513   }
//  514 
//  515   /* Initialization complete, full speed.*/
//  516   spiStart(mmcp->config->spip, mmcp->config->hscfg);
        LDR      R0,[R4, #+44]
        LDR      R1,[R0, #+8]
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiStart
        BL       spiStart
//  517 
//  518   /* Setting block size.*/
//  519   if (send_command_R1(mmcp, MMCSD_CMD_SET_BLOCKLEN,
//  520                       MMCSD_BLOCK_SIZE) != 0x00)
        MOV      R2,#+512
        MOVS     R1,#+16
        MOVS     R0,R4
          CFI FunCall send_command_R1
        BL       send_command_R1
        CMP      R0,#+0
        BNE.N    ??mmcConnect_14
        B.N      ??mmcConnect_15
??mmcConnect_13:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+1
        BNE.N    ??mmcConnect_6
??mmcConnect_16:
        ADDS     R5,R5,#+1
        CMP      R5,#+100
        BCS.N    ??mmcConnect_6
??mmcConnect_17:
        MOVS     R0,#+10
          CFI FunCall chThdSleep
        BL       chThdSleep
        B.N      ??mmcConnect_12
//  521     goto failed;
??mmcConnect_14:
        B.N      ??mmcConnect_6
//  522 
//  523   /* Determine capacity.*/
//  524   if (read_CxD(mmcp, MMCSD_CMD_SEND_CSD, mmcp->csd))
??mmcConnect_15:
        ADDS     R2,R4,#+24
        MOVS     R1,#+9
        MOVS     R0,R4
          CFI FunCall read_CxD
        BL       read_CxD
        CMP      R0,#+0
        BNE.N    ??mmcConnect_6
//  525     goto failed;
//  526   mmcp->capacity = mmcsdGetCapacity(mmcp->csd);
??mmcConnect_18:
        ADDS     R0,R4,#+24
          CFI FunCall mmcsdGetCapacity
        BL       mmcsdGetCapacity
        STR      R0,[R4, #+40]
//  527   if (mmcp->capacity == 0)
        LDR      R0,[R4, #+40]
        CMP      R0,#+0
        BEQ.N    ??mmcConnect_6
//  528     goto failed;
//  529 
//  530   if (read_CxD(mmcp, MMCSD_CMD_SEND_CID, mmcp->cid))
??mmcConnect_19:
        ADDS     R2,R4,#+8
        MOVS     R1,#+10
        MOVS     R0,R4
          CFI FunCall read_CxD
        BL       read_CxD
        CMP      R0,#+0
        BNE.N    ??mmcConnect_6
//  531     goto failed;
//  532 
//  533   mmcp->state = BLK_READY;
??mmcConnect_20:
        MOVS     R0,#+5
        STRB     R0,[R4, #+4]
//  534   return CH_SUCCESS;
        MOVS     R0,#+0
        B.N      ??mmcConnect_21
//  535 
//  536   /* Connection failed, state reset to BLK_ACTIVE.*/
//  537 failed:
//  538   spiStop(mmcp->config->spip);
??mmcConnect_6:
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiStop
        BL       spiStop
//  539   mmcp->state = BLK_ACTIVE;
        MOVS     R0,#+2
        STRB     R0,[R4, #+4]
//  540   return CH_FAILED;
        MOVS     R0,#+1
??mmcConnect_21:
        POP      {R1,R2,R4-R6,PC}  ;; return
          CFI EndBlock cfiBlock15
//  541 }
//  542 
//  543 /**
//  544  * @brief   Brings the driver in a state safe for card removal.
//  545  *
//  546  * @param[in] mmcp      pointer to the @p MMCDriver object
//  547  * @return              The operation status.
//  548  *
//  549  * @retval CH_SUCCESS   the operation succeeded and the driver is now
//  550  *                      in the @p MMC_INSERTED state.
//  551  * @retval CH_FAILED    the operation failed.
//  552  *
//  553  * @api
//  554  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function mmcDisconnect
        THUMB
//  555 bool_t mmcDisconnect(MMCDriver *mmcp) {
mmcDisconnect:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  556 
//  557   chDbgCheck(mmcp != NULL, "mmcDisconnect");
        CMP      R4,#+0
        BNE.N    ??mmcDisconnect_0
        MOVW     R2,#+557
        LDR.W    R1,??DataTable14_2
        LDR.W    R0,??DataTable14_10
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  558 
//  559   chSysLock();
??mmcDisconnect_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  560   chDbgAssert((mmcp->state == BLK_ACTIVE) || (mmcp->state == BLK_READY),
//  561               "mmcDisconnect(), #1", "invalid state");
        LDRB     R0,[R4, #+4]
        CMP      R0,#+2
        BEQ.N    ??mmcDisconnect_1
        LDRB     R0,[R4, #+4]
        CMP      R0,#+5
        BEQ.N    ??mmcDisconnect_1
        LDR.W    R0,??DataTable14_11
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  562   if (mmcp->state == BLK_ACTIVE) {
??mmcDisconnect_1:
        LDRB     R0,[R4, #+4]
        CMP      R0,#+2
        BNE.N    ??mmcDisconnect_2
//  563     chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  564     return CH_SUCCESS;
        MOVS     R0,#+0
        B.N      ??mmcDisconnect_3
//  565   }
//  566   mmcp->state = BLK_DISCONNECTING;
??mmcDisconnect_2:
        MOVS     R0,#+4
        STRB     R0,[R4, #+4]
//  567   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  568 
//  569   /* Wait for the pending write operations to complete.*/
//  570   spiStart(mmcp->config->spip, mmcp->config->hscfg);
        LDR      R0,[R4, #+44]
        LDR      R1,[R0, #+8]
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiStart
        BL       spiStart
//  571   sync(mmcp);
        MOVS     R0,R4
          CFI FunCall sync
        BL       sync
//  572 
//  573   spiStop(mmcp->config->spip);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiStop
        BL       spiStop
//  574   mmcp->state = BLK_ACTIVE;
        MOVS     R0,#+2
        STRB     R0,[R4, #+4]
//  575   return CH_SUCCESS;
        MOVS     R0,#+0
??mmcDisconnect_3:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock16
//  576 }
//  577 
//  578 /**
//  579  * @brief   Starts a sequential read.
//  580  *
//  581  * @param[in] mmcp      pointer to the @p MMCDriver object
//  582  * @param[in] startblk  first block to read
//  583  *
//  584  * @return              The operation status.
//  585  * @retval CH_SUCCESS   the operation succeeded.
//  586  * @retval CH_FAILED    the operation failed.
//  587  *
//  588  * @api
//  589  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function mmcStartSequentialRead
        THUMB
//  590 bool_t mmcStartSequentialRead(MMCDriver *mmcp, uint32_t startblk) {
mmcStartSequentialRead:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  591 
//  592   chDbgCheck(mmcp != NULL, "mmcStartSequentialRead");
        CMP      R4,#+0
        BNE.N    ??mmcStartSequentialRead_0
        MOV      R2,#+592
        LDR.N    R1,??DataTable14_2
        LDR.W    R0,??DataTable14_12
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  593   chDbgAssert(mmcp->state == BLK_READY,
//  594               "mmcStartSequentialRead(), #1", "invalid state");
??mmcStartSequentialRead_0:
        LDRB     R0,[R4, #+4]
        CMP      R0,#+5
        BEQ.N    ??mmcStartSequentialRead_1
        LDR.W    R0,??DataTable14_13
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  595 
//  596   /* Read operation in progress.*/
//  597   mmcp->state = BLK_READING;
??mmcStartSequentialRead_1:
        MOVS     R0,#+6
        STRB     R0,[R4, #+4]
//  598 
//  599   /* (Re)starting the SPI in case it has been reprogrammed externally, it can
//  600      happen if the SPI bus is shared among multiple peripherals.*/
//  601   spiStart(mmcp->config->spip, mmcp->config->hscfg);
        LDR      R0,[R4, #+44]
        LDR      R1,[R0, #+8]
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiStart
        BL       spiStart
//  602   spiSelect(mmcp->config->spip);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiSelect
        BL       spiSelect
//  603 
//  604   if (mmcp->block_addresses)
        LDRB     R0,[R4, #+48]
        CMP      R0,#+0
        BEQ.N    ??mmcStartSequentialRead_2
//  605     send_hdr(mmcp, MMCSD_CMD_READ_MULTIPLE_BLOCK, startblk);
        MOVS     R2,R5
        MOVS     R1,#+18
        MOVS     R0,R4
          CFI FunCall send_hdr
        BL       send_hdr
        B.N      ??mmcStartSequentialRead_3
//  606   else
//  607     send_hdr(mmcp, MMCSD_CMD_READ_MULTIPLE_BLOCK, startblk * MMCSD_BLOCK_SIZE);
??mmcStartSequentialRead_2:
        MOV      R0,#+512
        MUL      R2,R0,R5
        MOVS     R1,#+18
        MOVS     R0,R4
          CFI FunCall send_hdr
        BL       send_hdr
//  608 
//  609   if (recvr1(mmcp) != 0x00) {
??mmcStartSequentialRead_3:
        MOVS     R0,R4
          CFI FunCall recvr1
        BL       recvr1
        CMP      R0,#+0
        BEQ.N    ??mmcStartSequentialRead_4
//  610     spiStop(mmcp->config->spip);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiStop
        BL       spiStop
//  611     mmcp->state = BLK_READY;
        MOVS     R0,#+5
        STRB     R0,[R4, #+4]
//  612     return CH_FAILED;
        MOVS     R0,#+1
        B.N      ??mmcStartSequentialRead_5
//  613   }
//  614   return CH_SUCCESS;
??mmcStartSequentialRead_4:
        MOVS     R0,#+0
??mmcStartSequentialRead_5:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock17
//  615 }
//  616 
//  617 /**
//  618  * @brief   Reads a block within a sequential read operation.
//  619  *
//  620  * @param[in] mmcp      pointer to the @p MMCDriver object
//  621  * @param[out] buffer   pointer to the read buffer
//  622  *
//  623  * @return              The operation status.
//  624  * @retval CH_SUCCESS   the operation succeeded.
//  625  * @retval CH_FAILED    the operation failed.
//  626  *
//  627  * @api
//  628  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function mmcSequentialRead
        THUMB
//  629 bool_t mmcSequentialRead(MMCDriver *mmcp, uint8_t *buffer) {
mmcSequentialRead:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  630   int i;
//  631 
//  632   chDbgCheck((mmcp != NULL) && (buffer != NULL), "mmcSequentialRead");
        CMP      R4,#+0
        BEQ.N    ??mmcSequentialRead_0
        CMP      R5,#+0
        BNE.N    ??mmcSequentialRead_1
??mmcSequentialRead_0:
        MOV      R2,#+632
        LDR.N    R1,??DataTable14_2
        LDR.N    R0,??DataTable14_14
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  633 
//  634   if (mmcp->state != BLK_READING)
??mmcSequentialRead_1:
        LDRB     R0,[R4, #+4]
        CMP      R0,#+6
        BEQ.N    ??mmcSequentialRead_2
//  635     return CH_FAILED;
        MOVS     R0,#+1
        B.N      ??mmcSequentialRead_3
//  636 
//  637   for (i = 0; i < MMC_WAIT_DATA; i++) {
??mmcSequentialRead_2:
        MOVS     R0,#+0
        MOVS     R6,R0
??mmcSequentialRead_4:
        MOVW     R0,#+10000
        CMP      R6,R0
        BGE.N    ??mmcSequentialRead_5
//  638     spiReceive(mmcp->config->spip, 1, buffer);
        MOVS     R2,R5
        MOVS     R1,#+1
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiReceive
        BL       spiReceive
//  639     if (buffer[0] == 0xFE) {
        LDRB     R0,[R5, #+0]
        CMP      R0,#+254
        BNE.N    ??mmcSequentialRead_6
//  640       spiReceive(mmcp->config->spip, MMCSD_BLOCK_SIZE, buffer);
        MOVS     R2,R5
        MOV      R1,#+512
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiReceive
        BL       spiReceive
//  641       /* CRC ignored. */
//  642       spiIgnore(mmcp->config->spip, 2);
        MOVS     R1,#+2
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiIgnore
        BL       spiIgnore
//  643       return CH_SUCCESS;
        MOVS     R0,#+0
        B.N      ??mmcSequentialRead_3
//  644     }
//  645   }
??mmcSequentialRead_6:
        ADDS     R6,R6,#+1
        B.N      ??mmcSequentialRead_4
//  646   /* Timeout.*/
//  647   spiUnselect(mmcp->config->spip);
??mmcSequentialRead_5:
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiUnselect
        BL       spiUnselect
//  648   spiStop(mmcp->config->spip);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiStop
        BL       spiStop
//  649   mmcp->state = BLK_READY;
        MOVS     R0,#+5
        STRB     R0,[R4, #+4]
//  650   return CH_FAILED;
        MOVS     R0,#+1
??mmcSequentialRead_3:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock18
//  651 }
//  652 
//  653 /**
//  654  * @brief   Stops a sequential read gracefully.
//  655  *
//  656  * @param[in] mmcp      pointer to the @p MMCDriver object
//  657  *
//  658  * @return              The operation status.
//  659  * @retval CH_SUCCESS   the operation succeeded.
//  660  * @retval CH_FAILED    the operation failed.
//  661  *
//  662  * @api
//  663  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock19 Using cfiCommon0
          CFI Function mmcStopSequentialRead
        THUMB
//  664 bool_t mmcStopSequentialRead(MMCDriver *mmcp) {
mmcStopSequentialRead:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  665   static const uint8_t stopcmd[] = {0x40 | MMCSD_CMD_STOP_TRANSMISSION,
//  666                                     0, 0, 0, 0, 1, 0xFF};
//  667 
//  668   chDbgCheck(mmcp != NULL, "mmcStopSequentialRead");
        CMP      R4,#+0
        BNE.N    ??mmcStopSequentialRead_0
        MOV      R2,#+668
        LDR.N    R1,??DataTable14_2
        LDR.N    R0,??DataTable14_15
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  669 
//  670   if (mmcp->state != BLK_READING)
??mmcStopSequentialRead_0:
        LDRB     R0,[R4, #+4]
        CMP      R0,#+6
        BEQ.N    ??mmcStopSequentialRead_1
//  671     return CH_FAILED;
        MOVS     R0,#+1
        B.N      ??mmcStopSequentialRead_2
//  672 
//  673   spiSend(mmcp->config->spip, sizeof(stopcmd), stopcmd);
??mmcStopSequentialRead_1:
        LDR.N    R2,??DataTable14_16
        MOVS     R1,#+7
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiSend
        BL       spiSend
//  674 /*  result = recvr1(mmcp) != 0x00;*/
//  675   /* Note, ignored r1 response, it can be not zero, unknown issue.*/
//  676   (void) recvr1(mmcp);
        MOVS     R0,R4
          CFI FunCall recvr1
        BL       recvr1
        MOVS     R5,R0
//  677 
//  678   /* Read operation finished.*/
//  679   spiUnselect(mmcp->config->spip);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiUnselect
        BL       spiUnselect
//  680   mmcp->state = BLK_READY;
        MOVS     R0,#+5
        STRB     R0,[R4, #+4]
//  681   return CH_SUCCESS;
        MOVS     R0,#+0
??mmcStopSequentialRead_2:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock19
//  682 }

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
??stopcmd:
        DATA
        DC8 76, 0, 0, 0, 0, 1, 255, 0
//  683 
//  684 /**
//  685  * @brief   Starts a sequential write.
//  686  *
//  687  * @param[in] mmcp      pointer to the @p MMCDriver object
//  688  * @param[in] startblk  first block to write
//  689  *
//  690  * @return              The operation status.
//  691  * @retval CH_SUCCESS   the operation succeeded.
//  692  * @retval CH_FAILED    the operation failed.
//  693  *
//  694  * @api
//  695  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock20 Using cfiCommon0
          CFI Function mmcStartSequentialWrite
        THUMB
//  696 bool_t mmcStartSequentialWrite(MMCDriver *mmcp, uint32_t startblk) {
mmcStartSequentialWrite:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  697 
//  698   chDbgCheck(mmcp != NULL, "mmcStartSequentialWrite");
        CMP      R4,#+0
        BNE.N    ??mmcStartSequentialWrite_0
        MOVW     R2,#+698
        LDR.N    R1,??DataTable14_2
        LDR.N    R0,??DataTable14_17
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  699   chDbgAssert(mmcp->state == BLK_READY,
//  700               "mmcStartSequentialWrite(), #1", "invalid state");
??mmcStartSequentialWrite_0:
        LDRB     R0,[R4, #+4]
        CMP      R0,#+5
        BEQ.N    ??mmcStartSequentialWrite_1
        LDR.N    R0,??DataTable14_18
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  701 
//  702   /* Write operation in progress.*/
//  703   mmcp->state = BLK_WRITING;
??mmcStartSequentialWrite_1:
        MOVS     R0,#+7
        STRB     R0,[R4, #+4]
//  704 
//  705   spiStart(mmcp->config->spip, mmcp->config->hscfg);
        LDR      R0,[R4, #+44]
        LDR      R1,[R0, #+8]
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiStart
        BL       spiStart
//  706   spiSelect(mmcp->config->spip);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiSelect
        BL       spiSelect
//  707   if (mmcp->block_addresses)
        LDRB     R0,[R4, #+48]
        CMP      R0,#+0
        BEQ.N    ??mmcStartSequentialWrite_2
//  708     send_hdr(mmcp, MMCSD_CMD_WRITE_MULTIPLE_BLOCK, startblk);
        MOVS     R2,R5
        MOVS     R1,#+25
        MOVS     R0,R4
          CFI FunCall send_hdr
        BL       send_hdr
        B.N      ??mmcStartSequentialWrite_3
//  709   else
//  710     send_hdr(mmcp, MMCSD_CMD_WRITE_MULTIPLE_BLOCK,
//  711              startblk * MMCSD_BLOCK_SIZE);
??mmcStartSequentialWrite_2:
        MOV      R0,#+512
        MUL      R2,R0,R5
        MOVS     R1,#+25
        MOVS     R0,R4
          CFI FunCall send_hdr
        BL       send_hdr
//  712 
//  713   if (recvr1(mmcp) != 0x00) {
??mmcStartSequentialWrite_3:
        MOVS     R0,R4
          CFI FunCall recvr1
        BL       recvr1
        CMP      R0,#+0
        BEQ.N    ??mmcStartSequentialWrite_4
//  714     spiStop(mmcp->config->spip);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiStop
        BL       spiStop
//  715     mmcp->state = BLK_READY;
        MOVS     R0,#+5
        STRB     R0,[R4, #+4]
//  716     return CH_FAILED;
        MOVS     R0,#+1
        B.N      ??mmcStartSequentialWrite_5
//  717   }
//  718   return CH_SUCCESS;
??mmcStartSequentialWrite_4:
        MOVS     R0,#+0
??mmcStartSequentialWrite_5:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock20
//  719 }
//  720 
//  721 /**
//  722  * @brief   Writes a block within a sequential write operation.
//  723  *
//  724  * @param[in] mmcp      pointer to the @p MMCDriver object
//  725  * @param[out] buffer   pointer to the write buffer
//  726  *
//  727  * @return              The operation status.
//  728  * @retval CH_SUCCESS   the operation succeeded.
//  729  * @retval CH_FAILED    the operation failed.
//  730  *
//  731  * @api
//  732  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock21 Using cfiCommon0
          CFI Function mmcSequentialWrite
        THUMB
//  733 bool_t mmcSequentialWrite(MMCDriver *mmcp, const uint8_t *buffer) {
mmcSequentialWrite:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  734   static const uint8_t start[] = {0xFF, 0xFC};
//  735   uint8_t b[1];
//  736 
//  737   chDbgCheck((mmcp != NULL) && (buffer != NULL), "mmcSequentialWrite");
        CMP      R4,#+0
        BEQ.N    ??mmcSequentialWrite_0
        CMP      R5,#+0
        BNE.N    ??mmcSequentialWrite_1
??mmcSequentialWrite_0:
        MOVW     R2,#+737
        LDR.N    R1,??DataTable14_2
        LDR.N    R0,??DataTable14_19
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  738 
//  739   if (mmcp->state != BLK_WRITING)
??mmcSequentialWrite_1:
        LDRB     R0,[R4, #+4]
        CMP      R0,#+7
        BEQ.N    ??mmcSequentialWrite_2
//  740     return CH_FAILED;
        MOVS     R0,#+1
        B.N      ??mmcSequentialWrite_3
//  741 
//  742   spiSend(mmcp->config->spip, sizeof(start), start);    /* Data prologue.   */
??mmcSequentialWrite_2:
        LDR.N    R2,??DataTable14_20
        MOVS     R1,#+2
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiSend
        BL       spiSend
//  743   spiSend(mmcp->config->spip, MMCSD_BLOCK_SIZE, buffer);/* Data.            */
        MOVS     R2,R5
        MOV      R1,#+512
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiSend
        BL       spiSend
//  744   spiIgnore(mmcp->config->spip, 2);                     /* CRC ignored.     */
        MOVS     R1,#+2
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiIgnore
        BL       spiIgnore
//  745   spiReceive(mmcp->config->spip, 1, b);
        ADD      R2,SP,#+0
        MOVS     R1,#+1
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiReceive
        BL       spiReceive
//  746   if ((b[0] & 0x1F) == 0x05) {
        LDRB     R0,[SP, #+0]
        ANDS     R0,R0,#0x1F
        CMP      R0,#+5
        BNE.N    ??mmcSequentialWrite_4
//  747     wait(mmcp);
        MOVS     R0,R4
          CFI FunCall wait
        BL       wait
//  748     return CH_SUCCESS;
        MOVS     R0,#+0
        B.N      ??mmcSequentialWrite_3
//  749   }
//  750 
//  751   /* Error.*/
//  752   spiUnselect(mmcp->config->spip);
??mmcSequentialWrite_4:
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiUnselect
        BL       spiUnselect
//  753   spiStop(mmcp->config->spip);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiStop
        BL       spiStop
//  754   mmcp->state = BLK_READY;
        MOVS     R0,#+5
        STRB     R0,[R4, #+4]
//  755   return CH_FAILED;
        MOVS     R0,#+1
??mmcSequentialWrite_3:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock21
//  756 }

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
??start:
        DATA
        DC8 255, 252
//  757 
//  758 /**
//  759  * @brief   Stops a sequential write gracefully.
//  760  *
//  761  * @param[in] mmcp      pointer to the @p MMCDriver object
//  762  *
//  763  * @return              The operation status.
//  764  * @retval CH_SUCCESS   the operation succeeded.
//  765  * @retval CH_FAILED    the operation failed.
//  766  *
//  767  * @api
//  768  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock22 Using cfiCommon0
          CFI Function mmcStopSequentialWrite
        THUMB
//  769 bool_t mmcStopSequentialWrite(MMCDriver *mmcp) {
mmcStopSequentialWrite:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  770   static const uint8_t stop[] = {0xFD, 0xFF};
//  771 
//  772   chDbgCheck(mmcp != NULL, "mmcStopSequentialWrite");
        CMP      R4,#+0
        BNE.N    ??mmcStopSequentialWrite_0
        MOV      R2,#+772
        LDR.N    R1,??DataTable14_2
        LDR.N    R0,??DataTable14_21
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  773 
//  774   if (mmcp->state != BLK_WRITING)
??mmcStopSequentialWrite_0:
        LDRB     R0,[R4, #+4]
        CMP      R0,#+7
        BEQ.N    ??mmcStopSequentialWrite_1
//  775     return CH_FAILED;
        MOVS     R0,#+1
        B.N      ??mmcStopSequentialWrite_2
//  776 
//  777   spiSend(mmcp->config->spip, sizeof(stop), stop);
??mmcStopSequentialWrite_1:
        LDR.N    R2,??DataTable14_22
        MOVS     R1,#+2
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiSend
        BL       spiSend
//  778   spiUnselect(mmcp->config->spip);
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiUnselect
        BL       spiUnselect
//  779 
//  780   /* Write operation finished.*/
//  781   mmcp->state = BLK_READY;
        MOVS     R0,#+5
        STRB     R0,[R4, #+4]
//  782   return CH_SUCCESS;
        MOVS     R0,#+0
??mmcStopSequentialWrite_2:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock22
//  783 }

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
??stop:
        DATA
        DC8 253, 255
//  784 
//  785 /**
//  786  * @brief   Waits for card idle condition.
//  787  *
//  788  * @param[in] mmcp      pointer to the @p MMCDriver object
//  789  *
//  790  * @return              The operation status.
//  791  * @retval CH_SUCCESS   the operation succeeded.
//  792  * @retval CH_FAILED    the operation failed.
//  793  *
//  794  * @api
//  795  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock23 Using cfiCommon0
          CFI Function mmcSync
        THUMB
//  796 bool_t mmcSync(MMCDriver *mmcp) {
mmcSync:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  797 
//  798   chDbgCheck(mmcp != NULL, "mmcSync");
        CMP      R4,#+0
        BNE.N    ??mmcSync_0
        MOVW     R2,#+798
        LDR.N    R1,??DataTable14_2
        LDR.N    R0,??DataTable14_23
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  799 
//  800   if (mmcp->state != BLK_READY)
??mmcSync_0:
        LDRB     R0,[R4, #+4]
        CMP      R0,#+5
        BEQ.N    ??mmcSync_1
//  801     return CH_FAILED;
        MOVS     R0,#+1
        B.N      ??mmcSync_2
//  802 
//  803   /* Synchronization operation in progress.*/
//  804   mmcp->state = BLK_SYNCING;
??mmcSync_1:
        MOVS     R0,#+8
        STRB     R0,[R4, #+4]
//  805 
//  806   spiStart(mmcp->config->spip, mmcp->config->hscfg);
        LDR      R0,[R4, #+44]
        LDR      R1,[R0, #+8]
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiStart
        BL       spiStart
//  807   sync(mmcp);
        MOVS     R0,R4
          CFI FunCall sync
        BL       sync
//  808 
//  809   /* Synchronization operation finished.*/
//  810   mmcp->state = BLK_READY;
        MOVS     R0,#+5
        STRB     R0,[R4, #+4]
//  811   return CH_SUCCESS;
        MOVS     R0,#+0
??mmcSync_2:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock23
//  812 }
//  813 
//  814 /**
//  815  * @brief   Returns the media info.
//  816  *
//  817  * @param[in] mmcp      pointer to the @p MMCDriver object
//  818  * @param[out] bdip     pointer to a @p BlockDeviceInfo structure
//  819  *
//  820  * @return              The operation status.
//  821  * @retval CH_SUCCESS   the operation succeeded.
//  822  * @retval CH_FAILED    the operation failed.
//  823  *
//  824  * @api
//  825  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock24 Using cfiCommon0
          CFI Function mmcGetInfo
        THUMB
//  826 bool_t mmcGetInfo(MMCDriver *mmcp, BlockDeviceInfo *bdip) {
mmcGetInfo:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  827 
//  828   chDbgCheck((mmcp != NULL) && (bdip != NULL), "mmcGetInfo");
        CMP      R4,#+0
        BEQ.N    ??mmcGetInfo_0
        CMP      R5,#+0
        BNE.N    ??mmcGetInfo_1
??mmcGetInfo_0:
        MOV      R2,#+828
        LDR.N    R1,??DataTable14_2
        LDR.N    R0,??DataTable14_24
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  829 
//  830   if (mmcp->state != BLK_READY)
??mmcGetInfo_1:
        LDRB     R0,[R4, #+4]
        CMP      R0,#+5
        BEQ.N    ??mmcGetInfo_2
//  831     return CH_FAILED;
        MOVS     R0,#+1
        B.N      ??mmcGetInfo_3
//  832 
//  833   bdip->blk_num  = mmcp->capacity;
??mmcGetInfo_2:
        LDR      R0,[R4, #+40]
        STR      R0,[R5, #+4]
//  834   bdip->blk_size = MMCSD_BLOCK_SIZE;
        MOV      R0,#+512
        STR      R0,[R5, #+0]
//  835 
//  836   return CH_SUCCESS;
        MOVS     R0,#+0
??mmcGetInfo_3:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock24
//  837 }
//  838 
//  839 /**
//  840  * @brief   Erases blocks.
//  841  *
//  842  * @param[in] mmcp      pointer to the @p MMCDriver object
//  843  * @param[in] startblk  starting block number
//  844  * @param[in] endblk    ending block number
//  845  *
//  846  * @return              The operation status.
//  847  * @retval CH_SUCCESS   the operation succeeded.
//  848  * @retval CH_FAILED    the operation failed.
//  849  *
//  850  * @api
//  851  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock25 Using cfiCommon0
          CFI Function mmcErase
        THUMB
//  852 bool_t mmcErase(MMCDriver *mmcp, uint32_t startblk, uint32_t endblk) {
mmcErase:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  853 
//  854   chDbgCheck((mmcp != NULL), "mmcErase");
        CMP      R4,#+0
        BNE.N    ??mmcErase_0
        MOVW     R2,#+854
        LDR.N    R1,??DataTable14_2
        LDR.N    R0,??DataTable14_25
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  855 
//  856   /* Erase operation in progress.*/
//  857   mmcp->state = BLK_WRITING;
??mmcErase_0:
        MOVS     R0,#+7
        STRB     R0,[R4, #+4]
//  858 
//  859   /* Handling command differences between HC and normal cards.*/
//  860   if (!mmcp->block_addresses) {
        LDRB     R0,[R4, #+48]
        CMP      R0,#+0
        BNE.N    ??mmcErase_1
//  861     startblk *= MMCSD_BLOCK_SIZE;
        MOV      R0,#+512
        MULS     R5,R0,R5
//  862     endblk *= MMCSD_BLOCK_SIZE;
        MOV      R0,#+512
        MULS     R6,R0,R6
//  863   }
//  864 
//  865   if (send_command_R1(mmcp, MMCSD_CMD_ERASE_RW_BLK_START, startblk))
??mmcErase_1:
        MOVS     R2,R5
        MOVS     R1,#+32
        MOVS     R0,R4
          CFI FunCall send_command_R1
        BL       send_command_R1
        CMP      R0,#+0
        BNE.N    ??mmcErase_2
//  866     goto failed;
//  867 
//  868   if (send_command_R1(mmcp, MMCSD_CMD_ERASE_RW_BLK_END, endblk))
??mmcErase_3:
        MOVS     R2,R6
        MOVS     R1,#+33
        MOVS     R0,R4
          CFI FunCall send_command_R1
        BL       send_command_R1
        CMP      R0,#+0
        BNE.N    ??mmcErase_2
//  869     goto failed;
//  870 
//  871   if (send_command_R1(mmcp, MMCSD_CMD_ERASE, 0))
??mmcErase_4:
        MOVS     R2,#+0
        MOVS     R1,#+38
        MOVS     R0,R4
          CFI FunCall send_command_R1
        BL       send_command_R1
        CMP      R0,#+0
        BNE.N    ??mmcErase_2
//  872     goto failed;
//  873 
//  874   mmcp->state = BLK_READY;
??mmcErase_5:
        MOVS     R0,#+5
        STRB     R0,[R4, #+4]
//  875   return CH_SUCCESS;
        MOVS     R0,#+0
        B.N      ??mmcErase_6
//  876 
//  877   /* Command failed, state reset to BLK_ACTIVE.*/
//  878 failed:
//  879   spiStop(mmcp->config->spip);
??mmcErase_2:
        LDR      R0,[R4, #+44]
        LDR      R0,[R0, #+0]
          CFI FunCall spiStop
        BL       spiStop
//  880   mmcp->state = BLK_READY;
        MOVS     R0,#+5
        STRB     R0,[R4, #+4]
//  881   return CH_FAILED;
        MOVS     R0,#+1
??mmcErase_6:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock25
//  882 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14:
        DC32     crc7_lookup_table

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_1:
        DC32     mmc_vmt

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_2:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_3:
        DC32     `?<Constant "\\"mmcStart\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_4:
        DC32     `?<Constant "mmcStart(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_5:
        DC32     `?<Constant "\\"mmcStop\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_6:
        DC32     `?<Constant "mmcStop(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_7:
        DC32     `?<Constant "\\"mmcConnect\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_8:
        DC32     `?<Constant "mmcConnect(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_9:
        DC32     0x400001aa

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_10:
        DC32     `?<Constant "\\"mmcDisconnect\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_11:
        DC32     `?<Constant "mmcDisconnect(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_12:
        DC32     `?<Constant "\\"mmcStartSequentialRe...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_13:
        DC32     `?<Constant "mmcStartSequentialRea...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_14:
        DC32     `?<Constant "\\"mmcSequentialRead\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_15:
        DC32     `?<Constant "\\"mmcStopSequentialRea...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_16:
        DC32     ??stopcmd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_17:
        DC32     `?<Constant "\\"mmcStartSequentialWr...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_18:
        DC32     `?<Constant "mmcStartSequentialWri...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_19:
        DC32     `?<Constant "\\"mmcSequentialWrite\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_20:
        DC32     ??start

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_21:
        DC32     `?<Constant "\\"mmcStopSequentialWri...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_22:
        DC32     ??stop

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_23:
        DC32     `?<Constant "\\"mmcSync\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_24:
        DC32     `?<Constant "\\"mmcGetInfo\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_25:
        DC32     `?<Constant "\\"mmcErase\\"()">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
//  883 
//  884 #endif /* HAL_USE_MMC_SPI */
//  885 
//  886 /** @} */
// 
//   772 bytes in section .rodata
// 2 320 bytes in section .text
// 
// 2 320 bytes of CODE  memory
//   772 bytes of CONST memory
//
//Errors: none
//Warnings: none
