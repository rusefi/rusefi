///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:10 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\vari /
//                       ous\memstreams.c                                     /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\vari /
//                       ous\memstreams.c -lCN F:\stuff\rusefi_sourceforge\fi /
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
//                       memstreams.s                                         /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME memstreams

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN __aeabi_memcpy

        PUBLIC memcpy
        PUBLIC msObjectInit
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\various\memstreams.c
//    1 /*
//    2     ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
//    3 
//    4     Licensed under the Apache License, Version 2.0 (the "License");
//    5     you may not use this file except in compliance with the License.
//    6     You may obtain a copy of the License at
//    7 
//    8         http://www.apache.org/licenses/LICENSE-2.0
//    9 
//   10     Unless required by applicable law or agreed to in writing, software
//   11     distributed under the License is distributed on an "AS IS" BASIS,
//   12     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   13     See the License for the specific language governing permissions and
//   14     limitations under the License.
//   15 */
//   16 
//   17 /**
//   18  * @file    memstreams.c
//   19  * @brief   Memory streams code.
//   20  *
//   21  * @addtogroup memory_streams
//   22  * @{
//   23  */
//   24 
//   25 #include <string.h>

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memcpy
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function memcpy
        THUMB
// __intrinsic __nounwind __interwork __softfp void *memcpy(void *, void const *, size_t)
memcpy:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall __aeabi_memcpy
        BL       __aeabi_memcpy
        MOVS     R0,R4
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock0
//   26 
//   27 #include "ch.h"
//   28 #include "memstreams.h"
//   29 
//   30 /*===========================================================================*/
//   31 /* Driver local definitions.                                                 */
//   32 /*===========================================================================*/
//   33 
//   34 /*===========================================================================*/
//   35 /* Driver exported variables.                                                */
//   36 /*===========================================================================*/
//   37 
//   38 /*===========================================================================*/
//   39 /* Driver local variables.                                                   */
//   40 /*===========================================================================*/
//   41 
//   42 /*===========================================================================*/
//   43 /* Driver local functions.                                                   */
//   44 /*===========================================================================*/
//   45 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function writes
        THUMB
//   46 static size_t writes(void *ip, const uint8_t *bp, size_t n) {
writes:
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
//   47   MemoryStream *msp = ip;
        MOVS     R7,R4
//   48 
//   49   if (msp->size - msp->eos < n)
        LDR      R0,[R7, #+8]
        LDR      R1,[R7, #+12]
        SUBS     R0,R0,R1
        CMP      R0,R6
        BCS.N    ??writes_0
//   50     n = msp->size - msp->eos;
        LDR      R0,[R7, #+8]
        LDR      R1,[R7, #+12]
        SUBS     R0,R0,R1
        MOVS     R6,R0
//   51   memcpy(msp->buffer + msp->eos, bp, n);
??writes_0:
        MOVS     R2,R6
        MOVS     R1,R5
        LDR      R0,[R7, #+12]
        LDR      R3,[R7, #+4]
        ADDS     R0,R0,R3
          CFI FunCall memcpy
        BL       memcpy
//   52   msp->eos += n;
        LDR      R0,[R7, #+12]
        ADDS     R0,R6,R0
        STR      R0,[R7, #+12]
//   53   return n;
        MOVS     R0,R6
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock1
//   54 }
//   55 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function reads
        THUMB
//   56 static size_t reads(void *ip, uint8_t *bp, size_t n) {
reads:
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
//   57   MemoryStream *msp = ip;
        MOVS     R7,R4
//   58 
//   59   if (msp->eos - msp->offset < n)
        LDR      R0,[R7, #+12]
        LDR      R1,[R7, #+16]
        SUBS     R0,R0,R1
        CMP      R0,R6
        BCS.N    ??reads_0
//   60     n = msp->eos - msp->offset;
        LDR      R0,[R7, #+12]
        LDR      R1,[R7, #+16]
        SUBS     R0,R0,R1
        MOVS     R6,R0
//   61   memcpy(bp, msp->buffer + msp->offset, n);
??reads_0:
        MOVS     R2,R6
        LDR      R0,[R7, #+16]
        LDR      R1,[R7, #+4]
        ADDS     R1,R0,R1
        MOVS     R0,R5
          CFI FunCall memcpy
        BL       memcpy
//   62   msp->offset += n;
        LDR      R0,[R7, #+16]
        ADDS     R0,R6,R0
        STR      R0,[R7, #+16]
//   63   return n;
        MOVS     R0,R6
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock2
//   64 }
//   65 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function put
          CFI NoCalls
        THUMB
//   66 static msg_t put(void *ip, uint8_t b) {
put:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
        MOVS     R2,R0
//   67   MemoryStream *msp = ip;
        MOVS     R3,R2
//   68 
//   69   if (msp->size - msp->eos <= 0)
        LDR      R0,[R3, #+8]
        LDR      R4,[R3, #+12]
        SUBS     R0,R0,R4
        CMP      R0,#+0
        BNE.N    ??put_0
//   70     return RDY_RESET;
        MVNS     R0,#+1
        B.N      ??put_1
//   71   *(msp->buffer + msp->eos) = b;
??put_0:
        LDR      R0,[R3, #+12]
        LDR      R4,[R3, #+4]
        STRB     R1,[R0, R4]
//   72   msp->eos += 1;
        LDR      R0,[R3, #+12]
        ADDS     R0,R0,#+1
        STR      R0,[R3, #+12]
//   73   return RDY_OK;
        MOVS     R0,#+0
??put_1:
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock3
//   74 }
//   75 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function get
          CFI NoCalls
        THUMB
//   76 static msg_t get(void *ip) {
get:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
        MOVS     R1,R0
//   77   uint8_t b;
//   78   MemoryStream *msp = ip;
        MOVS     R3,R1
//   79 
//   80   if (msp->eos - msp->offset <= 0)
        LDR      R0,[R3, #+12]
        LDR      R4,[R3, #+16]
        SUBS     R0,R0,R4
        CMP      R0,#+0
        BNE.N    ??get_0
//   81     return RDY_RESET;
        MVNS     R0,#+1
        B.N      ??get_1
//   82   b = *(msp->buffer + msp->offset);
??get_0:
        LDR      R0,[R3, #+16]
        LDR      R4,[R3, #+4]
        LDRB     R0,[R0, R4]
        MOVS     R2,R0
//   83   msp->offset += 1;
        LDR      R0,[R3, #+16]
        ADDS     R0,R0,#+1
        STR      R0,[R3, #+16]
//   84   return b;
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        MOVS     R0,R2
??get_1:
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//   85 }
//   86 

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//   87 static const struct MemStreamVMT vmt = {writes, reads, put, get};
vmt:
        DATA
        DC32 writes, reads, put, get
//   88 
//   89 /*===========================================================================*/
//   90 /* Driver exported functions.                                                */
//   91 /*===========================================================================*/
//   92 
//   93 /**
//   94  * @brief   Memory stream object initialization.
//   95  *
//   96  * @param[out] msp      pointer to the @p MemoryStream object to be initialized
//   97  * @param[in] buffer    pointer to the memory buffer for the memory stream
//   98  * @param[in] size      total size of the memory stream buffer
//   99  * @param[in] eos       initial End Of Stream offset. Normally you need to
//  100  *                      put this to zero for RAM buffers or equal to @p size
//  101  *                      for ROM streams.
//  102  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function msObjectInit
          CFI NoCalls
        THUMB
//  103 void msObjectInit(MemoryStream *msp, uint8_t *buffer,
//  104                   size_t size, size_t eos) {
msObjectInit:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
//  105 
//  106   msp->vmt    = &vmt;
        LDR.N    R4,??DataTable0
        STR      R4,[R0, #+0]
//  107   msp->buffer = buffer;
        STR      R1,[R0, #+4]
//  108   msp->size   = size;
        STR      R2,[R0, #+8]
//  109   msp->eos    = eos;
        STR      R3,[R0, #+12]
//  110   msp->offset = 0;
        MOVS     R4,#+0
        STR      R4,[R0, #+16]
//  111 }
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock5

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0:
        DC32     vmt

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
//  112 
//  113 /** @} */
// 
//  16 bytes in section .rodata
// 234 bytes in section .text
// 
// 212 bytes of CODE  memory (+ 22 bytes shared)
//  16 bytes of CONST memory
//
//Errors: none
//Warnings: none
