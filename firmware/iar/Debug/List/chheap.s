///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:41 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chheap.c                                      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chheap.c -lCN F:\stuff\rusefi_sourceforge\fir /
//                       mware\iar\Debug\List\ -lA                            /
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
//                       chheap.s                                             /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME chheap

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN chCoreAlloc
        EXTERN chDbgPanic
        EXTERN chDbgPanic3
        EXTERN chMtxInit
        EXTERN chMtxLock
        EXTERN chMtxUnlock

        PUBLIC _heap_init
        PUBLIC chHeapAlloc
        PUBLIC chHeapFree
        PUBLIC chHeapInit
        PUBLIC chHeapStatus
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\kernel\src\chheap.c
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
//   29  * @file    chheap.c
//   30  * @brief   Heaps code.
//   31  *
//   32  * @addtogroup heaps
//   33  * @details Heap Allocator related APIs.
//   34  *          <h2>Operation mode</h2>
//   35  *          The heap allocator implements a first-fit strategy and its APIs
//   36  *          are functionally equivalent to the usual @p malloc() and @p free()
//   37  *          library functions. The main difference is that the OS heap APIs
//   38  *          are guaranteed to be thread safe.<br>
//   39  *          By enabling the @p CH_USE_MALLOC_HEAP option the heap manager
//   40  *          will use the runtime-provided @p malloc() and @p free() as
//   41  *          back end for the heap APIs instead of the system provided
//   42  *          allocator.
//   43  * @pre     In order to use the heap APIs the @p CH_USE_HEAP option must
//   44  *          be enabled in @p chconf.h.
//   45  * @{
//   46  */
//   47 
//   48 #include "ch.h"
//   49 
//   50 #if CH_USE_HEAP || defined(__DOXYGEN__)
//   51 
//   52 #if !CH_USE_MALLOC_HEAP || defined(__DOXYGEN__)
//   53 
//   54 /*
//   55  * Defaults on the best synchronization mechanism available.
//   56  */
//   57 #if CH_USE_MUTEXES || defined(__DOXYGEN__)
//   58 #define H_LOCK(h)       chMtxLock(&(h)->h_mtx)
//   59 #define H_UNLOCK(h)     chMtxUnlock()
//   60 #else
//   61 #define H_LOCK(h)       chSemWait(&(h)->h_sem)
//   62 #define H_UNLOCK(h)     chSemSignal(&(h)->h_sem)
//   63 #endif
//   64 
//   65 /**
//   66  * @brief   Default heap descriptor.
//   67  */

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   68 static MemoryHeap default_heap;
default_heap:
        DS8 32
//   69 
//   70 /**
//   71  * @brief   Initializes the default heap.
//   72  *
//   73  * @notapi
//   74  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _heap_init
        THUMB
//   75 void _heap_init(void) {
_heap_init:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   76   default_heap.h_provider = chCoreAlloc;
        LDR.N    R0,??DataTable4
        LDR.N    R1,??DataTable4_1
        STR      R1,[R0, #+0]
//   77   default_heap.h_free.h.u.next = (union heap_header *)NULL;
        LDR.N    R0,??DataTable4
        MOVS     R1,#+0
        STR      R1,[R0, #+8]
//   78   default_heap.h_free.h.size = 0;
        LDR.N    R0,??DataTable4
        MOVS     R1,#+0
        STR      R1,[R0, #+12]
//   79 #if CH_USE_MUTEXES || defined(__DOXYGEN__)
//   80   chMtxInit(&default_heap.h_mtx);
        LDR.N    R0,??DataTable4_2
          CFI FunCall chMtxInit
        BL       chMtxInit
//   81 #else
//   82   chSemInit(&default_heap.h_sem, 1);
//   83 #endif
//   84 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0
//   85 
//   86 /**
//   87  * @brief   Initializes a memory heap from a static memory area.
//   88  * @pre     Both the heap buffer base and the heap size must be aligned to
//   89  *          the @p stkalign_t type size.
//   90  * @pre     In order to use this function the option @p CH_USE_MALLOC_HEAP
//   91  *          must be disabled.
//   92  *
//   93  * @param[out] heapp    pointer to the memory heap descriptor to be initialized
//   94  * @param[in] buf       heap buffer base
//   95  * @param[in] size      heap size
//   96  *
//   97  * @init
//   98  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function chHeapInit
        THUMB
//   99 void chHeapInit(MemoryHeap *heapp, void *buf, size_t size) {
chHeapInit:
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
//  100   union heap_header *hp;
//  101 
//  102   chDbgCheck(MEM_IS_ALIGNED(buf) && MEM_IS_ALIGNED(size), "chHeapInit");
        ANDS     R0,R5,#0x7
        CMP      R0,#+0
        BNE.N    ??chHeapInit_0
        TST      R6,#0x7
        BEQ.N    ??chHeapInit_1
??chHeapInit_0:
        MOVS     R2,#+102
        LDR.N    R1,??DataTable4_3
        LDR.N    R0,??DataTable4_4
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  103 
//  104   heapp->h_provider = (memgetfunc_t)NULL;
??chHeapInit_1:
        MOVS     R0,#+0
        STR      R0,[R4, #+0]
//  105   heapp->h_free.h.u.next = hp = buf;
        MOVS     R7,R5
        STR      R5,[R4, #+8]
//  106   heapp->h_free.h.size = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+12]
//  107   hp->h.u.next = NULL;
        MOVS     R0,#+0
        STR      R0,[R7, #+0]
//  108   hp->h.size = size - sizeof(union heap_header);
        SUBS     R0,R6,#+8
        STR      R0,[R7, #+4]
//  109 #if CH_USE_MUTEXES || defined(__DOXYGEN__)
//  110   chMtxInit(&heapp->h_mtx);
        ADDS     R0,R4,#+16
          CFI FunCall chMtxInit
        BL       chMtxInit
//  111 #else
//  112   chSemInit(&heapp->h_sem, 1);
//  113 #endif
//  114 }
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock1
//  115 
//  116 /**
//  117  * @brief   Allocates a block of memory from the heap by using the first-fit
//  118  *          algorithm.
//  119  * @details The allocated block is guaranteed to be properly aligned for a
//  120  *          pointer data type (@p stkalign_t).
//  121  *
//  122  * @param[in] heapp     pointer to a heap descriptor or @p NULL in order to
//  123  *                      access the default heap.
//  124  * @param[in] size      the size of the block to be allocated. Note that the
//  125  *                      allocated block may be a bit bigger than the requested
//  126  *                      size for alignment and fragmentation reasons.
//  127  * @return              A pointer to the allocated block.
//  128  * @retval NULL         if the block cannot be allocated.
//  129  *
//  130  * @api
//  131  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function chHeapAlloc
        THUMB
//  132 void *chHeapAlloc(MemoryHeap *heapp, size_t size) {
chHeapAlloc:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOV      R8,R1
//  133   union heap_header *qp, *hp, *fp;
//  134 
//  135   if (heapp == NULL)
        CMP      R4,#+0
        BNE.N    ??chHeapAlloc_0
//  136     heapp = &default_heap;
        LDR.N    R0,??DataTable4
        MOVS     R4,R0
//  137 
//  138   size = MEM_ALIGN_NEXT(size);
??chHeapAlloc_0:
        ADDS     R0,R8,#+7
        LSRS     R8,R0,#+3
        LSLS     R8,R8,#+3
//  139   qp = &heapp->h_free;
        ADDS     R0,R4,#+8
        MOVS     R5,R0
//  140   H_LOCK(heapp);
        ADDS     R0,R4,#+16
          CFI FunCall chMtxLock
        BL       chMtxLock
//  141 
//  142   while (qp->h.u.next != NULL) {
??chHeapAlloc_1:
        LDR      R0,[R5, #+0]
        CMP      R0,#+0
        BEQ.N    ??chHeapAlloc_2
//  143     hp = qp->h.u.next;
        LDR      R0,[R5, #+0]
        MOVS     R6,R0
//  144     if (hp->h.size >= size) {
        LDR      R0,[R6, #+4]
        CMP      R0,R8
        BCC.N    ??chHeapAlloc_3
//  145       if (hp->h.size < size + sizeof(union heap_header)) {
        LDR      R0,[R6, #+4]
        ADDS     R1,R8,#+8
        CMP      R0,R1
        BCS.N    ??chHeapAlloc_4
//  146         /* Gets the whole block even if it is slightly bigger than the
//  147            requested size because the fragment would be too small to be
//  148            useful.*/
//  149         qp->h.u.next = hp->h.u.next;
        LDR      R0,[R6, #+0]
        STR      R0,[R5, #+0]
        B.N      ??chHeapAlloc_5
//  150       }
//  151       else {
//  152         /* Block bigger enough, must split it.*/
//  153         fp = (void *)((uint8_t *)(hp) + sizeof(union heap_header) + size);
??chHeapAlloc_4:
        ADDS     R0,R8,R6
        ADDS     R0,R0,#+8
        MOVS     R7,R0
//  154         fp->h.u.next = hp->h.u.next;
        LDR      R0,[R6, #+0]
        STR      R0,[R7, #+0]
//  155         fp->h.size = hp->h.size - sizeof(union heap_header) - size;
        LDR      R0,[R6, #+4]
        SUBS     R0,R0,#+8
        SUBS     R0,R0,R8
        STR      R0,[R7, #+4]
//  156         qp->h.u.next = fp;
        STR      R7,[R5, #+0]
//  157         hp->h.size = size;
        STR      R8,[R6, #+4]
//  158       }
//  159       hp->h.u.heap = heapp;
??chHeapAlloc_5:
        STR      R4,[R6, #+0]
//  160 
//  161       H_UNLOCK(heapp);
          CFI FunCall chMtxUnlock
        BL       chMtxUnlock
//  162       return (void *)(hp + 1);
        ADDS     R0,R6,#+8
        B.N      ??chHeapAlloc_6
//  163     }
//  164     qp = hp;
??chHeapAlloc_3:
        MOVS     R5,R6
        B.N      ??chHeapAlloc_1
//  165   }
//  166 
//  167   H_UNLOCK(heapp);
??chHeapAlloc_2:
          CFI FunCall chMtxUnlock
        BL       chMtxUnlock
//  168 
//  169   /* More memory is required, tries to get it from the associated provider
//  170      else fails.*/
//  171   if (heapp->h_provider) {
        LDR      R0,[R4, #+0]
        CMP      R0,#+0
        BEQ.N    ??chHeapAlloc_7
//  172     hp = heapp->h_provider(size + sizeof(union heap_header));
        ADDS     R0,R8,#+8
        LDR      R1,[R4, #+0]
          CFI FunCall
        BLX      R1
        MOVS     R6,R0
//  173     if (hp != NULL) {
        CMP      R6,#+0
        BEQ.N    ??chHeapAlloc_7
//  174       hp->h.u.heap = heapp;
        STR      R4,[R6, #+0]
//  175       hp->h.size = size;
        STR      R8,[R6, #+4]
//  176       hp++;
        ADDS     R6,R6,#+8
//  177       return (void *)hp;
        MOVS     R0,R6
        B.N      ??chHeapAlloc_6
//  178     }
//  179   }
//  180   return NULL;
??chHeapAlloc_7:
        MOVS     R0,#+0
??chHeapAlloc_6:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock2
//  181 }
//  182 
//  183 #define LIMIT(p) (union heap_header *)((uint8_t *)(p) + \ 
//  184                                         sizeof(union heap_header) + \ 
//  185                                         (p)->h.size)
//  186 
//  187 /**
//  188  * @brief   Frees a previously allocated memory block.
//  189  *
//  190  * @param[in] p         pointer to the memory block to be freed
//  191  *
//  192  * @api
//  193  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function chHeapFree
        THUMB
//  194 void chHeapFree(void *p) {
chHeapFree:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
//  195   union heap_header *qp, *hp;
//  196   MemoryHeap *heapp;
//  197 
//  198   chDbgCheck(p != NULL, "chHeapFree");
        CMP      R4,#+0
        BNE.N    ??chHeapFree_0
        MOVS     R2,#+198
        LDR.N    R1,??DataTable4_3
        LDR.N    R0,??DataTable4_5
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  199 
//  200   hp = (union heap_header *)p - 1;
??chHeapFree_0:
        SUBS     R0,R4,#+8
        MOVS     R6,R0
//  201   heapp = hp->h.u.heap;
        LDR      R0,[R6, #+0]
        MOVS     R7,R0
//  202   qp = &heapp->h_free;
        ADDS     R0,R7,#+8
        MOVS     R5,R0
//  203   H_LOCK(heapp);
        ADDS     R0,R7,#+16
          CFI FunCall chMtxLock
        BL       chMtxLock
//  204 
//  205   while (TRUE) {
//  206     chDbgAssert((hp < qp) || (hp >= LIMIT(qp)),
//  207                 "chHeapFree(), #1",
//  208                 "within free block");
??chHeapFree_1:
        CMP      R6,R5
        BCC.N    ??chHeapFree_2
        LDR      R0,[R5, #+4]
        ADDS     R0,R0,R5
        ADDS     R0,R0,#+8
        CMP      R6,R0
        BCS.N    ??chHeapFree_2
        LDR.N    R0,??DataTable4_6
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  209 
//  210     if (((qp == &heapp->h_free) || (hp > qp)) &&
//  211         ((qp->h.u.next == NULL) || (hp < qp->h.u.next))) {
??chHeapFree_2:
        ADDS     R0,R7,#+8
        CMP      R5,R0
        BEQ.N    ??chHeapFree_3
        CMP      R5,R6
        BCS.N    ??chHeapFree_4
??chHeapFree_3:
        LDR      R0,[R5, #+0]
        CMP      R0,#+0
        BEQ.N    ??chHeapFree_5
        LDR      R0,[R5, #+0]
        CMP      R6,R0
        BCS.N    ??chHeapFree_4
//  212       /* Insertion after qp.*/
//  213       hp->h.u.next = qp->h.u.next;
??chHeapFree_5:
        LDR      R0,[R5, #+0]
        STR      R0,[R6, #+0]
//  214       qp->h.u.next = hp;
        STR      R6,[R5, #+0]
//  215       /* Verifies if the newly inserted block should be merged.*/
//  216       if (LIMIT(hp) == hp->h.u.next) {
        LDR      R0,[R6, #+4]
        ADDS     R0,R0,R6
        ADDS     R0,R0,#+8
        LDR      R1,[R6, #+0]
        CMP      R0,R1
        BNE.N    ??chHeapFree_6
//  217         /* Merge with the next block.*/
//  218         hp->h.size += hp->h.u.next->h.size + sizeof(union heap_header);
        LDR      R0,[R6, #+4]
        LDR      R1,[R6, #+0]
        LDR      R1,[R1, #+4]
        ADDS     R1,R1,#+8
        ADDS     R0,R1,R0
        STR      R0,[R6, #+4]
//  219         hp->h.u.next = hp->h.u.next->h.u.next;
        LDR      R0,[R6, #+0]
        LDR      R0,[R0, #+0]
        STR      R0,[R6, #+0]
//  220       }
//  221       if ((LIMIT(qp) == hp)) {
??chHeapFree_6:
        LDR      R0,[R5, #+4]
        ADDS     R0,R0,R5
        ADDS     R0,R0,#+8
        CMP      R0,R6
        BNE.N    ??chHeapFree_7
//  222         /* Merge with the previous block.*/
//  223         qp->h.size += hp->h.size + sizeof(union heap_header);
        LDR      R0,[R5, #+4]
        LDR      R1,[R6, #+4]
        ADDS     R1,R1,#+8
        ADDS     R0,R1,R0
        STR      R0,[R5, #+4]
//  224         qp->h.u.next = hp->h.u.next;
        LDR      R0,[R6, #+0]
        STR      R0,[R5, #+0]
//  225       }
//  226       break;
//  227     }
//  228     qp = qp->h.u.next;
//  229   }
//  230 
//  231   H_UNLOCK(heapp);
??chHeapFree_7:
          CFI FunCall chMtxUnlock
        BL       chMtxUnlock
//  232   return;
        B.N      ??chHeapFree_8
??chHeapFree_4:
        LDR      R5,[R5, #+0]
        B.N      ??chHeapFree_1
??chHeapFree_8:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock3
//  233 }
//  234 
//  235 /**
//  236  * @brief   Reports the heap status.
//  237  * @note    This function is meant to be used in the test suite, it should
//  238  *          not be really useful for the application code.
//  239  * @note    This function is not implemented when the @p CH_USE_MALLOC_HEAP
//  240  *          configuration option is used (it always returns zero).
//  241  *
//  242  * @param[in] heapp     pointer to a heap descriptor or @p NULL in order to
//  243  *                      access the default heap.
//  244  * @param[in] sizep     pointer to a variable that will receive the total
//  245  *                      fragmented free space
//  246  * @return              The number of fragments in the heap.
//  247  *
//  248  * @api
//  249  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function chHeapStatus
        THUMB
//  250 size_t chHeapStatus(MemoryHeap *heapp, size_t *sizep) {
chHeapStatus:
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
//  251   union heap_header *qp;
//  252   size_t n, sz;
//  253 
//  254   if (heapp == NULL)
        CMP      R4,#+0
        BNE.N    ??chHeapStatus_0
//  255     heapp = &default_heap;
        LDR.N    R0,??DataTable4
        MOVS     R4,R0
//  256 
//  257   H_LOCK(heapp);
??chHeapStatus_0:
        ADDS     R0,R4,#+16
          CFI FunCall chMtxLock
        BL       chMtxLock
//  258 
//  259   sz = 0;
        MOVS     R0,#+0
        MOV      R8,R0
//  260   for (n = 0, qp = &heapp->h_free; qp->h.u.next; n++, qp = qp->h.u.next)
        MOVS     R0,#+0
        MOVS     R7,R0
        ADDS     R0,R4,#+8
        MOVS     R6,R0
??chHeapStatus_1:
        LDR      R0,[R6, #+0]
        CMP      R0,#+0
        BEQ.N    ??chHeapStatus_2
//  261     sz += qp->h.u.next->h.size;
        LDR      R0,[R6, #+0]
        LDR      R0,[R0, #+4]
        ADDS     R8,R0,R8
        ADDS     R7,R7,#+1
        LDR      R6,[R6, #+0]
        B.N      ??chHeapStatus_1
//  262   if (sizep)
??chHeapStatus_2:
        CMP      R5,#+0
        BEQ.N    ??chHeapStatus_3
//  263     *sizep = sz;
        STR      R8,[R5, #+0]
//  264 
//  265   H_UNLOCK(heapp);
??chHeapStatus_3:
          CFI FunCall chMtxUnlock
        BL       chMtxUnlock
//  266   return n;
        MOVS     R0,R7
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock4
//  267 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4:
        DC32     default_heap

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_1:
        DC32     chCoreAlloc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_2:
        DC32     default_heap+0x10

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_3:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_4:
        DC32     `?<Constant "\\"chHeapInit\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_5:
        DC32     `?<Constant "\\"chHeapFree\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_6:
        DC32     `?<Constant "chHeapFree(), #1">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chHeapInit\\"()">`:
        DATA
        DC8 "\"chHeapInit\"()"
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
        DC8 6BH, 65H, 72H, 6EH, 65H, 6CH, 5CH, 73H
        DC8 72H, 63H, 5CH, 63H, 68H, 68H, 65H, 61H
        DC8 70H, 2EH, 63H, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chHeapFree\\"()">`:
        DATA
        DC8 "\"chHeapFree\"()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "chHeapFree(), #1">`:
        DATA
        DC8 "chHeapFree(), #1"
        DC8 0, 0, 0

        END
//  268 
//  269 #else /* CH_USE_MALLOC_HEAP */
//  270 
//  271 #include <stdlib.h>
//  272 
//  273 #if CH_USE_MUTEXES
//  274 #define H_LOCK()        chMtxLock(&hmtx)
//  275 #define H_UNLOCK()      chMtxUnlock()
//  276 static Mutex            hmtx;
//  277 #elif CH_USE_SEMAPHORES
//  278 #define H_LOCK()        chSemWait(&hsem)
//  279 #define H_UNLOCK()      chSemSignal(&hsem)
//  280 static Semaphore        hsem;
//  281 #endif
//  282 
//  283 void _heap_init(void) {
//  284 
//  285 #if CH_USE_MUTEXES
//  286   chMtxInit(&hmtx);
//  287 #else
//  288   chSemInit(&hsem, 1);
//  289 #endif
//  290 }
//  291 
//  292 void *chHeapAlloc(MemoryHeap *heapp, size_t size) {
//  293   void *p;
//  294 
//  295   chDbgCheck(heapp == NULL, "chHeapAlloc");
//  296 
//  297   H_LOCK();
//  298   p = malloc(size);
//  299   H_UNLOCK();
//  300   return p;
//  301 }
//  302 
//  303 void chHeapFree(void *p) {
//  304 
//  305   chDbgCheck(p != NULL, "chHeapFree");
//  306 
//  307   H_LOCK();
//  308   free(p);
//  309   H_UNLOCK();
//  310 }
//  311 
//  312 size_t chHeapStatus(MemoryHeap *heapp, size_t *sizep) {
//  313 
//  314   chDbgCheck(heapp == NULL, "chHeapStatus");
//  315 
//  316   if (sizep)
//  317     *sizep = 0;
//  318   return 0;
//  319 }
//  320 
//  321 #endif /* CH_USE_MALLOC_HEAP */
//  322 
//  323 #endif /* CH_USE_HEAP */
//  324 
//  325 /** @} */
// 
//  32 bytes in section .bss
// 120 bytes in section .rodata
// 514 bytes in section .text
// 
// 514 bytes of CODE  memory
// 120 bytes of CONST memory
//  32 bytes of DATA  memory
//
//Errors: none
//Warnings: none
