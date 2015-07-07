///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:31 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chvt.c                                        /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chvt.c -lCN F:\stuff\rusefi_sourceforge\firmw /
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
//                       chvt.s                                               /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME chvt

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN chDbgCheckClassI
        EXTERN chDbgPanic
        EXTERN chDbgPanic3

        PUBLIC _vt_init
        PUBLIC chVTResetI
        PUBLIC chVTSetI
        PUBLIC vtlist
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\kernel\src\chvt.c
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
//   29  * @file    chvt.c
//   30  * @brief   Time and Virtual Timers related code.
//   31  *
//   32  * @addtogroup time
//   33  * @details Time and Virtual Timers related APIs and services.
//   34  * @{
//   35  */
//   36 
//   37 #include "ch.h"
//   38 
//   39 /**
//   40  * @brief   Virtual timers delta list header.
//   41  */

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   42 VTList vtlist;
vtlist:
        DS8 16
//   43 
//   44 /**
//   45  * @brief   Virtual Timers initialization.
//   46  * @note    Internal use only.
//   47  *
//   48  * @notapi
//   49  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _vt_init
          CFI NoCalls
        THUMB
//   50 void _vt_init(void) {
//   51 
//   52   vtlist.vt_next = vtlist.vt_prev = (void *)&vtlist;
_vt_init:
        LDR.N    R0,??DataTable2
        LDR.N    R1,??DataTable2
        STR      R0,[R1, #+4]
        LDR.N    R1,??DataTable2
        STR      R0,[R1, #+0]
//   53   vtlist.vt_time = (systime_t)-1;
        LDR.N    R0,??DataTable2
        MOVS     R1,#-1
        STR      R1,[R0, #+8]
//   54   vtlist.vt_systime = 0;
        LDR.N    R0,??DataTable2
        MOVS     R1,#+0
        STR      R1,[R0, #+12]
//   55 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//   56 
//   57 /**
//   58  * @brief   Enables a virtual timer.
//   59  * @note    The associated function is invoked from interrupt context.
//   60  *
//   61  * @param[out] vtp      the @p VirtualTimer structure pointer
//   62  * @param[in] time      the number of ticks before the operation timeouts, the
//   63  *                      special values are handled as follow:
//   64  *                      - @a TIME_INFINITE is allowed but interpreted as a
//   65  *                        normal time specification.
//   66  *                      - @a TIME_IMMEDIATE this value is not allowed.
//   67  *                      .
//   68  * @param[in] vtfunc    the timer callback function. After invoking the
//   69  *                      callback the timer is disabled and the structure can
//   70  *                      be disposed or reused.
//   71  * @param[in] par       a parameter that will be passed to the callback
//   72  *                      function
//   73  *
//   74  * @iclass
//   75  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function chVTSetI
        THUMB
//   76 void chVTSetI(VirtualTimer *vtp, systime_t time, vtfunc_t vtfunc, void *par) {
chVTSetI:
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
//   77   VirtualTimer *p;
//   78 
//   79   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//   80   chDbgCheck((vtp != NULL) && (vtfunc != NULL) && (time != TIME_IMMEDIATE),
//   81              "chVTSetI");
        CMP      R4,#+0
        BEQ.N    ??chVTSetI_0
        MOVS     R0,R6
        CMP      R0,#+0
        BEQ.N    ??chVTSetI_0
        CMP      R5,#+0
        BNE.N    ??chVTSetI_1
??chVTSetI_0:
        MOVS     R2,#+81
        LDR.N    R1,??DataTable2_1
        LDR.N    R0,??DataTable2_2
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//   82 
//   83   vtp->vt_par = par;
??chVTSetI_1:
        STR      R7,[R4, #+16]
//   84   vtp->vt_func = vtfunc;
        STR      R6,[R4, #+12]
//   85   p = vtlist.vt_next;
        LDR.N    R0,??DataTable2
        LDR      R0,[R0, #+0]
        MOV      R8,R0
//   86   while (p->vt_time < time) {
??chVTSetI_2:
        LDR      R0,[R8, #+8]
        CMP      R0,R5
        BCS.N    ??chVTSetI_3
//   87     time -= p->vt_time;
        LDR      R0,[R8, #+8]
        SUBS     R5,R5,R0
//   88     p = p->vt_next;
        LDR      R8,[R8, #+0]
        B.N      ??chVTSetI_2
//   89   }
//   90 
//   91   vtp->vt_prev = (vtp->vt_next = p)->vt_prev;
??chVTSetI_3:
        STR      R8,[R4, #+0]
        LDR      R0,[R8, #+4]
        STR      R0,[R4, #+4]
//   92   vtp->vt_prev->vt_next = p->vt_prev = vtp;
        STR      R4,[R8, #+4]
        LDR      R0,[R4, #+4]
        STR      R4,[R0, #+0]
//   93   vtp->vt_time = time;
        STR      R5,[R4, #+8]
//   94   if (p != (void *)&vtlist)
        LDR.N    R0,??DataTable2
        CMP      R8,R0
        BEQ.N    ??chVTSetI_4
//   95     p->vt_time -= time;
        LDR      R0,[R8, #+8]
        SUBS     R0,R0,R5
        STR      R0,[R8, #+8]
//   96 }
??chVTSetI_4:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock1
//   97 
//   98 /**
//   99  * @brief   Disables a Virtual Timer.
//  100  * @note    The timer MUST be active when this function is invoked.
//  101  *
//  102  * @param[in] vtp       the @p VirtualTimer structure pointer
//  103  *
//  104  * @iclass
//  105  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function chVTResetI
        THUMB
//  106 void chVTResetI(VirtualTimer *vtp) {
chVTResetI:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  107 
//  108   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  109   chDbgCheck(vtp != NULL, "chVTResetI");
        CMP      R4,#+0
        BNE.N    ??chVTResetI_0
        MOVS     R2,#+109
        LDR.N    R1,??DataTable2_1
        LDR.N    R0,??DataTable2_3
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  110   chDbgAssert(vtp->vt_func != NULL,
//  111               "chVTResetI(), #1",
//  112               "timer not set or already triggered");
??chVTResetI_0:
        LDR      R0,[R4, #+12]
        CMP      R0,#+0
        BNE.N    ??chVTResetI_1
        LDR.N    R0,??DataTable2_4
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  113 
//  114   if (vtp->vt_next != (void *)&vtlist)
??chVTResetI_1:
        LDR      R0,[R4, #+0]
        LDR.N    R1,??DataTable2
        CMP      R0,R1
        BEQ.N    ??chVTResetI_2
//  115     vtp->vt_next->vt_time += vtp->vt_time;
        LDR      R0,[R4, #+0]
        LDR      R0,[R0, #+8]
        LDR      R1,[R4, #+8]
        ADDS     R0,R1,R0
        LDR      R1,[R4, #+0]
        STR      R0,[R1, #+8]
//  116   vtp->vt_prev->vt_next = vtp->vt_next;
??chVTResetI_2:
        LDR      R0,[R4, #+4]
        LDR      R1,[R4, #+0]
        STR      R1,[R0, #+0]
//  117   vtp->vt_next->vt_prev = vtp->vt_prev;
        LDR      R0,[R4, #+0]
        LDR      R1,[R4, #+4]
        STR      R1,[R0, #+4]
//  118   vtp->vt_func = (vtfunc_t)NULL;
        MOVS     R0,#+0
        STR      R0,[R4, #+12]
//  119 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2:
        DC32     vtlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_1:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_2:
        DC32     `?<Constant "\\"chVTSetI\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_3:
        DC32     `?<Constant "\\"chVTResetI\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_4:
        DC32     `?<Constant "chVTResetI(), #1">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chVTSetI\\"()">`:
        DATA
        DC8 "\"chVTSetI\"()"
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
        DC8 6BH, 65H, 72H, 6EH, 65H, 6CH, 5CH, 73H
        DC8 72H, 63H, 5CH, 63H, 68H, 76H, 74H, 2EH
        DC8 63H, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chVTResetI\\"()">`:
        DATA
        DC8 "\"chVTResetI\"()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "chVTResetI(), #1">`:
        DATA
        DC8 "chVTResetI(), #1"
        DC8 0, 0, 0

        END
//  120 
//  121 /** @} */
// 
//  16 bytes in section .bss
// 120 bytes in section .rodata
// 228 bytes in section .text
// 
// 228 bytes of CODE  memory
// 120 bytes of CONST memory
//  16 bytes of DATA  memory
//
//Errors: none
//Warnings: none
