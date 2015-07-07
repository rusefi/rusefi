///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:42 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chmemcore.c                                   /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chmemcore.c -lCN F:\stuff\rusefi_sourceforge\ /
//                       firmware\iar\Debug\List\ -lA                         /
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
//                       chmemcore.s                                          /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME chmemcore

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN chDbgCheckClassI
        EXTERN dbg_check_lock
        EXTERN dbg_check_unlock

        PUBLIC _core_init
        PUBLIC chCoreAlloc
        PUBLIC chCoreAllocI
        PUBLIC chCoreStatus
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\kernel\src\chmemcore.c
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
//   29  * @file    chmemcore.c
//   30  * @brief   Core memory manager code.
//   31  *
//   32  * @addtogroup memcore
//   33  * @details Core Memory Manager related APIs and services.
//   34  *          <h2>Operation mode</h2>
//   35  *          The core memory manager is a simplified allocator that only
//   36  *          allows to allocate memory blocks without the possibility to
//   37  *          free them.<br>
//   38  *          This allocator is meant as a memory blocks provider for the
//   39  *          other allocators such as:
//   40  *          - C-Runtime allocator (through a compiler specific adapter module).
//   41  *          - Heap allocator (see @ref heaps).
//   42  *          - Memory pools allocator (see @ref pools).
//   43  *          .
//   44  *          By having a centralized memory provider the various allocators
//   45  *          can coexist and share the main memory.<br>
//   46  *          This allocator, alone, is also useful for very simple
//   47  *          applications that just require a simple way to get memory
//   48  *          blocks.
//   49  * @pre     In order to use the core memory manager APIs the @p CH_USE_MEMCORE
//   50  *          option must be enabled in @p chconf.h.
//   51  * @{
//   52  */
//   53 
//   54 #include "ch.h"
//   55 
//   56 #if CH_USE_MEMCORE || defined(__DOXYGEN__)
//   57 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   58 static uint8_t *nextmem;
nextmem:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   59 static uint8_t *endmem;
endmem:
        DS8 4
//   60 
//   61 /**
//   62  * @brief   Low level memory manager initialization.
//   63  *
//   64  * @notapi
//   65  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _core_init
          CFI NoCalls
        THUMB
//   66 void _core_init(void) {
//   67 #if CH_MEMCORE_SIZE == 0
//   68   extern uint8_t __heap_base__[];
//   69   extern uint8_t __heap_end__[];
//   70   nextmem = (uint8_t *)MEM_ALIGN_NEXT(__heap_base__);
//   71   endmem = (uint8_t *)MEM_ALIGN_PREV(__heap_end__);
//   72 #else
//   73   static stkalign_t buffer[MEM_ALIGN_NEXT(CH_MEMCORE_SIZE)/MEM_ALIGN_SIZE];
//   74   nextmem = (uint8_t *)&buffer[0];
_core_init:
        LDR.N    R0,??DataTable2
        LDR.N    R1,??DataTable2_1
        STR      R1,[R0, #+0]
//   75   endmem = (uint8_t *)&buffer[MEM_ALIGN_NEXT(CH_MEMCORE_SIZE)/MEM_ALIGN_SIZE];
        LDR.N    R0,??DataTable2_2
        LDR.N    R1,??DataTable2_3
        STR      R1,[R0, #+0]
//   76 #endif
//   77 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
??buffer:
        DS8 2048
//   78 
//   79 /**
//   80  * @brief   Allocates a memory block.
//   81  * @details The size of the returned block is aligned to the alignment
//   82  *          type so it is not possible to allocate less
//   83  *          than <code>MEM_ALIGN_SIZE</code>.
//   84  *
//   85  * @param[in] size      the size of the block to be allocated
//   86  * @return              A pointer to the allocated memory block.
//   87  * @retval NULL         allocation failed, core memory exhausted.
//   88  *
//   89  * @api
//   90  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function chCoreAlloc
        THUMB
//   91 void *chCoreAlloc(size_t size) {
chCoreAlloc:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//   92   void *p;
//   93 
//   94   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//   95   p = chCoreAllocI(size);
        MOVS     R0,R4
          CFI FunCall chCoreAllocI
        BL       chCoreAllocI
        MOVS     R5,R0
//   96   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//   97   return p;
        MOVS     R0,R5
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock1
//   98 }
//   99 
//  100 /**
//  101  * @brief   Allocates a memory block.
//  102  * @details The size of the returned block is aligned to the alignment
//  103  *          type so it is not possible to allocate less than
//  104  *          <code>MEM_ALIGN_SIZE</code>.
//  105  *
//  106  * @param[in] size      the size of the block to be allocated.
//  107  * @return              A pointer to the allocated memory block.
//  108  * @retval NULL         allocation failed, core memory exhausted.
//  109  *
//  110  * @iclass
//  111  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function chCoreAllocI
        THUMB
//  112 void *chCoreAllocI(size_t size) {
chCoreAllocI:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R5,R0
//  113   void *p;
//  114 
//  115   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  116 
//  117   size = MEM_ALIGN_NEXT(size);
        ADDS     R0,R5,#+7
        LSRS     R5,R0,#+3
        LSLS     R5,R5,#+3
//  118   if ((size_t)(endmem - nextmem) < size)
        LDR.N    R0,??DataTable2_2
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable2
        LDR      R1,[R1, #+0]
        SUBS     R0,R0,R1
        CMP      R0,R5
        BCS.N    ??chCoreAllocI_0
//  119     return NULL;
        MOVS     R0,#+0
        B.N      ??chCoreAllocI_1
//  120   p = nextmem;
??chCoreAllocI_0:
        LDR.N    R0,??DataTable2
        LDR      R0,[R0, #+0]
        MOVS     R4,R0
//  121   nextmem += size;
        LDR.N    R0,??DataTable2
        LDR      R0,[R0, #+0]
        ADDS     R0,R5,R0
        LDR.N    R1,??DataTable2
        STR      R0,[R1, #+0]
//  122   return p;
        MOVS     R0,R4
??chCoreAllocI_1:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2
//  123 }
//  124 
//  125 /**
//  126  * @brief   Core memory status.
//  127  *
//  128  * @return              The size, in bytes, of the free core memory.
//  129  *
//  130  * @api
//  131  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function chCoreStatus
          CFI NoCalls
        THUMB
//  132 size_t chCoreStatus(void) {
//  133 
//  134   return (size_t)(endmem - nextmem);
chCoreStatus:
        LDR.N    R0,??DataTable2_2
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable2
        LDR      R1,[R1, #+0]
        SUBS     R0,R0,R1
        BX       LR               ;; return
          CFI EndBlock cfiBlock3
//  135 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2:
        DC32     nextmem

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_1:
        DC32     ??buffer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_2:
        DC32     endmem

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_3:
        DC32     ??buffer+0x800

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
//  136 #endif /* CH_USE_MEMCORE */
//  137 
//  138 /** @} */
// 
// 2 056 bytes in section .bss
//   130 bytes in section .text
// 
//   130 bytes of CODE memory
// 2 056 bytes of DATA memory
//
//Errors: none
//Warnings: none
