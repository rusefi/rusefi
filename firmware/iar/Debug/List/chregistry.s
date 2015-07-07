///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:45 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chregistry.c                                  /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chregistry.c -lCN F:\stuff\rusefi_sourceforge /
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
//                       chregistry.s                                         /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME chregistry

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN dbg_check_lock
        EXTERN dbg_check_unlock
        EXTERN rlist

        PUBLIC chRegFirstThread
        PUBLIC chRegNextThread
        PUBLIC ch_debug
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\kernel\src\chregistry.c
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
//   29  * @file    chregistry.c
//   30  * @brief   Threads registry code.
//   31  *
//   32  * @addtogroup registry
//   33  * @details Threads Registry related APIs and services.
//   34  *
//   35  *          <h2>Operation mode</h2>
//   36  *          The Threads Registry is a double linked list that holds all the
//   37  *          active threads in the system.<br>
//   38  *          Operations defined for the registry:
//   39  *          - <b>First</b>, returns the first, in creation order, active thread
//   40  *            in the system.
//   41  *          - <b>Next</b>, returns the next, in creation order, active thread
//   42  *            in the system.
//   43  *          .
//   44  *          The registry is meant to be mainly a debug feature, for example,
//   45  *          using the registry a debugger can enumerate the active threads
//   46  *          in any given moment or the shell can print the active threads
//   47  *          and their state.<br>
//   48  *          Another possible use is for centralized threads memory management,
//   49  *          terminating threads can pulse an event source and an event handler
//   50  *          can perform a scansion of the registry in order to recover the
//   51  *          memory.
//   52  * @pre     In order to use the threads registry the @p CH_USE_REGISTRY option
//   53  *          must be enabled in @p chconf.h.
//   54  * @{
//   55  */
//   56 #include "ch.h"
//   57 
//   58 #if CH_USE_REGISTRY || defined(__DOXYGEN__)
//   59 
//   60 #define _offsetof(st, m)                                                     \ 
//   61   ((size_t)((char *)&((st *)0)->m - (char *)0))
//   62 
//   63 /*
//   64  * OS signature in ROM plus debug-related information.
//   65  */

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//   66 ROMCONST chdebug_t ch_debug = {
ch_debug:
        DATA
        DC8 6DH, 61H, 69H, 6EH
        DC8 0, 22
        DC16 4488
        DC8 4, 4, 76, 8, 12, 16, 20, 24, 28, 32, 33, 0, 34, 36, 0, 0
//   67   "main",
//   68   (uint8_t)0,
//   69   (uint8_t)sizeof (chdebug_t),
//   70   (uint16_t)((CH_KERNEL_MAJOR << 11) |
//   71              (CH_KERNEL_MINOR << 6) |
//   72              (CH_KERNEL_PATCH << 0)),
//   73   (uint8_t)sizeof (void *),
//   74   (uint8_t)sizeof (systime_t),
//   75   (uint8_t)sizeof (Thread),
//   76   (uint8_t)_offsetof(Thread, p_prio),
//   77   (uint8_t)_offsetof(Thread, p_ctx),
//   78   (uint8_t)_offsetof(Thread, p_newer),
//   79   (uint8_t)_offsetof(Thread, p_older),
//   80   (uint8_t)_offsetof(Thread, p_name),
//   81 #if CH_DBG_ENABLE_STACK_CHECK
//   82   (uint8_t)_offsetof(Thread, p_stklimit),
//   83 #else
//   84   (uint8_t)0,
//   85 #endif
//   86   (uint8_t)_offsetof(Thread, p_state),
//   87   (uint8_t)_offsetof(Thread, p_flags),
//   88 #if CH_USE_DYNAMIC
//   89   (uint8_t)_offsetof(Thread, p_refs),
//   90 #else
//   91   (uint8_t)0,
//   92 #endif
//   93 #if CH_TIME_QUANTUM > 0
//   94   (uint8_t)_offsetof(Thread, p_preempt),
//   95 #else
//   96   (uint8_t)0,
//   97 #endif
//   98 #if CH_DBG_THREADS_PROFILING
//   99   (uint8_t)_offsetof(Thread, p_time)
//  100 #else
//  101   (uint8_t)0
//  102 #endif
//  103 };
//  104 
//  105 /**
//  106  * @brief   Returns the first thread in the system.
//  107  * @details Returns the most ancient thread in the system, usually this is
//  108  *          the main thread unless it terminated. A reference is added to the
//  109  *          returned thread in order to make sure its status is not lost.
//  110  * @note    This function cannot return @p NULL because there is always at
//  111  *          least one thread in the system.
//  112  *
//  113  * @return              A reference to the most ancient thread.
//  114  *
//  115  * @api
//  116  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function chRegFirstThread
        THUMB
//  117 Thread *chRegFirstThread(void) {
chRegFirstThread:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  118   Thread *tp;
//  119 
//  120   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  121   tp = rlist.r_newer;
        LDR.N    R0,??DataTable1
        LDR      R0,[R0, #+16]
        MOVS     R4,R0
//  122 #if CH_USE_DYNAMIC
//  123   tp->p_refs++;
//  124 #endif
//  125   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  126   return tp;
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0
//  127 }
//  128 
//  129 /**
//  130  * @brief   Returns the thread next to the specified one.
//  131  * @details The reference counter of the specified thread is decremented and
//  132  *          the reference counter of the returned thread is incremented.
//  133  *
//  134  * @param[in] tp        pointer to the thread
//  135  * @return              A reference to the next thread.
//  136  * @retval NULL         if there is no next thread.
//  137  *
//  138  * @api
//  139  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function chRegNextThread
        THUMB
//  140 Thread *chRegNextThread(Thread *tp) {
chRegNextThread:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  141   Thread *ntp;
//  142 
//  143   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  144   ntp = tp->p_newer;
        LDR      R0,[R4, #+16]
        MOVS     R5,R0
//  145   if (ntp == (Thread *)&rlist)
        LDR.N    R0,??DataTable1
        CMP      R5,R0
        BNE.N    ??chRegNextThread_0
//  146     ntp = NULL;
        MOVS     R0,#+0
        MOVS     R5,R0
//  147 #if CH_USE_DYNAMIC
//  148   else {
//  149     chDbgAssert(ntp->p_refs < 255, "chRegNextThread(), #1",
//  150                 "too many references");
//  151     ntp->p_refs++;
//  152   }
//  153 #endif
//  154   chSysUnlock();
??chRegNextThread_0:
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  155 #if CH_USE_DYNAMIC
//  156   chThdRelease(tp);
//  157 #endif
//  158   return ntp;
        MOVS     R0,R5
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock1
//  159 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1:
        DC32     rlist

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
//  160 
//  161 #endif /* CH_USE_REGISTRY */
//  162 
//  163 /** @} */
// 
// 24 bytes in section .rodata
// 78 bytes in section .text
// 
// 78 bytes of CODE  memory
// 24 bytes of CONST memory
//
//Errors: none
//Warnings: none
