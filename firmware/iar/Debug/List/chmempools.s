///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:42 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chmempools.c                                  /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chmempools.c -lCN F:\stuff\rusefi_sourceforge /
//                       \firmware\iar\Debug\List\ -lA                        /
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
//                       chmempools.s                                         /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME chmempools

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN chDbgCheckClassI
        EXTERN chDbgPanic3
        EXTERN dbg_check_lock
        EXTERN dbg_check_unlock

        PUBLIC chPoolAlloc
        PUBLIC chPoolAllocI
        PUBLIC chPoolFree
        PUBLIC chPoolFreeI
        PUBLIC chPoolInit
        PUBLIC chPoolLoadArray
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\kernel\src\chmempools.c
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
//   29  * @file    chmempools.c
//   30  * @brief   Memory Pools code.
//   31  *
//   32  * @addtogroup pools
//   33  * @details Memory Pools related APIs and services.
//   34  *          <h2>Operation mode</h2>
//   35  *          The Memory Pools APIs allow to allocate/free fixed size objects in
//   36  *          <b>constant time</b> and reliably without memory fragmentation
//   37  *          problems.<br>
//   38  *          Memory Pools do not enforce any alignment constraint on the
//   39  *          contained object however the objects must be properly aligned
//   40  *          to contain a pointer to void.
//   41  * @pre     In order to use the memory pools APIs the @p CH_USE_MEMPOOLS option
//   42  *          must be enabled in @p chconf.h.
//   43  * @{
//   44  */
//   45 
//   46 #include "ch.h"
//   47 
//   48 #if CH_USE_MEMPOOLS || defined(__DOXYGEN__)
//   49 /**
//   50  * @brief   Initializes an empty memory pool.
//   51  *
//   52  * @param[out] mp       pointer to a @p MemoryPool structure
//   53  * @param[in] size      the size of the objects contained in this memory pool,
//   54  *                      the minimum accepted size is the size of a pointer to
//   55  *                      void.
//   56  * @param[in] provider  memory provider function for the memory pool or
//   57  *                      @p NULL if the pool is not allowed to grow
//   58  *                      automatically
//   59  *
//   60  * @init
//   61  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function chPoolInit
        THUMB
//   62 void chPoolInit(MemoryPool *mp, size_t size, memgetfunc_t provider) {
chPoolInit:
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
//   64   chDbgCheck((mp != NULL) && (size >= sizeof(void *)), "chPoolInit");
        CMP      R4,#+0
        BEQ.N    ??chPoolInit_0
        CMP      R5,#+4
        BCS.N    ??chPoolInit_1
??chPoolInit_0:
        MOVS     R2,#+64
        LDR.N    R1,??DataTable3
        LDR.N    R0,??DataTable3_1
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//   65 
//   66   mp->mp_next = NULL;
??chPoolInit_1:
        MOVS     R0,#+0
        STR      R0,[R4, #+0]
//   67   mp->mp_object_size = size;
        STR      R5,[R4, #+4]
//   68   mp->mp_provider = provider;
        STR      R6,[R4, #+8]
//   69 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock0
//   70 
//   71 /**
//   72  * @brief   Loads a memory pool with an array of static objects.
//   73  * @pre     The memory pool must be already been initialized.
//   74  * @pre     The array elements must be of the right size for the specified
//   75  *          memory pool.
//   76  * @post    The memory pool contains the elements of the input array.
//   77  *
//   78  * @param[in] mp        pointer to a @p MemoryPool structure
//   79  * @param[in] p         pointer to the array first element
//   80  * @param[in] n         number of elements in the array
//   81  *
//   82  * @api
//   83  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function chPoolLoadArray
        THUMB
//   84 void chPoolLoadArray(MemoryPool *mp, void *p, size_t n) {
chPoolLoadArray:
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
//   86   chDbgCheck((mp != NULL) && (n != 0), "chPoolLoadArray");
        CMP      R4,#+0
        BEQ.N    ??chPoolLoadArray_0
        CMP      R6,#+0
        BNE.N    ??chPoolLoadArray_1
??chPoolLoadArray_0:
        MOVS     R2,#+86
        LDR.N    R1,??DataTable3
        LDR.N    R0,??DataTable3_2
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//   87 
//   88   while (n) {
??chPoolLoadArray_1:
        CMP      R6,#+0
        BEQ.N    ??chPoolLoadArray_2
//   89     chPoolAdd(mp, p);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall chPoolFree
        BL       chPoolFree
//   90     p = (void *)(((uint8_t *)p) + mp->mp_object_size);
        LDR      R0,[R4, #+4]
        ADDS     R5,R0,R5
//   91     n--;
        SUBS     R6,R6,#+1
        B.N      ??chPoolLoadArray_1
//   92   }
//   93 }
??chPoolLoadArray_2:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock1
//   94 
//   95 /**
//   96  * @brief   Allocates an object from a memory pool.
//   97  * @pre     The memory pool must be already been initialized.
//   98  *
//   99  * @param[in] mp        pointer to a @p MemoryPool structure
//  100  * @return              The pointer to the allocated object.
//  101  * @retval NULL         if pool is empty.
//  102  *
//  103  * @iclass
//  104  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function chPoolAllocI
        THUMB
//  105 void *chPoolAllocI(MemoryPool *mp) {
chPoolAllocI:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  106   void *objp;
//  107 
//  108   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  109   chDbgCheck(mp != NULL, "chPoolAllocI");
        CMP      R4,#+0
        BNE.N    ??chPoolAllocI_0
        MOVS     R2,#+109
        LDR.N    R1,??DataTable3
        LDR.N    R0,??DataTable3_3
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  110 
//  111   if ((objp = mp->mp_next) != NULL)
??chPoolAllocI_0:
        LDR      R0,[R4, #+0]
        MOVS     R5,R0
        CMP      R0,#+0
        BEQ.N    ??chPoolAllocI_1
//  112     mp->mp_next = mp->mp_next->ph_next;
        LDR      R0,[R4, #+0]
        LDR      R0,[R0, #+0]
        STR      R0,[R4, #+0]
        B.N      ??chPoolAllocI_2
//  113   else if (mp->mp_provider != NULL)
??chPoolAllocI_1:
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BEQ.N    ??chPoolAllocI_2
//  114     objp = mp->mp_provider(mp->mp_object_size);
        LDR      R0,[R4, #+4]
        LDR      R1,[R4, #+8]
          CFI FunCall
        BLX      R1
        MOVS     R5,R0
//  115   return objp;
??chPoolAllocI_2:
        MOVS     R0,R5
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2
//  116 }
//  117 
//  118 /**
//  119  * @brief   Allocates an object from a memory pool.
//  120  * @pre     The memory pool must be already been initialized.
//  121  *
//  122  * @param[in] mp        pointer to a @p MemoryPool structure
//  123  * @return              The pointer to the allocated object.
//  124  * @retval NULL         if pool is empty.
//  125  *
//  126  * @api
//  127  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function chPoolAlloc
        THUMB
//  128 void *chPoolAlloc(MemoryPool *mp) {
chPoolAlloc:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  129   void *objp;
//  130 
//  131   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  132   objp = chPoolAllocI(mp);
        MOVS     R0,R4
          CFI FunCall chPoolAllocI
        BL       chPoolAllocI
        MOVS     R5,R0
//  133   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  134   return objp;
        MOVS     R0,R5
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock3
//  135 }
//  136 
//  137 /**
//  138  * @brief   Releases an object into a memory pool.
//  139  * @pre     The memory pool must be already been initialized.
//  140  * @pre     The freed object must be of the right size for the specified
//  141  *          memory pool.
//  142  * @pre     The object must be properly aligned to contain a pointer to void.
//  143  *
//  144  * @param[in] mp        pointer to a @p MemoryPool structure
//  145  * @param[in] objp      the pointer to the object to be released
//  146  *
//  147  * @iclass
//  148  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function chPoolFreeI
        THUMB
//  149 void chPoolFreeI(MemoryPool *mp, void *objp) {
chPoolFreeI:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  150   struct pool_header *php = objp;
        MOVS     R6,R5
//  151 
//  152   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  153   chDbgCheck((mp != NULL) && (objp != NULL), "chPoolFreeI");
        CMP      R4,#+0
        BEQ.N    ??chPoolFreeI_0
        CMP      R5,#+0
        BNE.N    ??chPoolFreeI_1
??chPoolFreeI_0:
        MOVS     R2,#+153
        LDR.N    R1,??DataTable3
        LDR.N    R0,??DataTable3_4
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  154 
//  155   php->ph_next = mp->mp_next;
??chPoolFreeI_1:
        LDR      R0,[R4, #+0]
        STR      R0,[R6, #+0]
//  156   mp->mp_next = php;
        STR      R6,[R4, #+0]
//  157 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_1:
        DC32     `?<Constant "\\"chPoolInit\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_2:
        DC32     `?<Constant "\\"chPoolLoadArray\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_3:
        DC32     `?<Constant "\\"chPoolAllocI\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_4:
        DC32     `?<Constant "\\"chPoolFreeI\\"()">`
//  158 
//  159 /**
//  160  * @brief   Releases an object into a memory pool.
//  161  * @pre     The memory pool must be already been initialized.
//  162  * @pre     The freed object must be of the right size for the specified
//  163  *          memory pool.
//  164  * @pre     The object must be properly aligned to contain a pointer to void.
//  165  *
//  166  * @param[in] mp        pointer to a @p MemoryPool structure
//  167  * @param[in] objp      the pointer to the object to be released
//  168  *
//  169  * @api
//  170  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function chPoolFree
        THUMB
//  171 void chPoolFree(MemoryPool *mp, void *objp) {
chPoolFree:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  172 
//  173   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  174   chPoolFreeI(mp, objp);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall chPoolFreeI
        BL       chPoolFreeI
//  175   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  176 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock5

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chPoolInit\\"()">`:
        DATA
        DC8 "\"chPoolInit\"()"
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
        DC8 72H, 63H, 5CH, 63H, 68H, 6DH, 65H, 6DH
        DC8 70H, 6FH, 6FH, 6CH, 73H, 2EH, 63H, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chPoolLoadArray\\"()">`:
        DATA
        DC8 "\"chPoolLoadArray\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chPoolAllocI\\"()">`:
        DATA
        DC8 "\"chPoolAllocI\"()"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chPoolFreeI\\"()">`:
        DATA
        DC8 "\"chPoolFreeI\"()"

        END
//  177 
//  178 #endif /* CH_USE_MEMPOOLS */
//  179 
//  180 /** @} */
// 
// 144 bytes in section .rodata
// 270 bytes in section .text
// 
// 270 bytes of CODE  memory
// 144 bytes of CONST memory
//
//Errors: none
//Warnings: none
