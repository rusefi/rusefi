///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:30 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chsys.c                                       /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chsys.c -lCN F:\stuff\rusefi_sourceforge\firm /
//                       ware\iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge /
//                       \firmware\iar\Debug\List\ -o                         /
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
//                       chsys.s                                              /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME chsys

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN __main_thread_stack_base__
        EXTERN _core_init
        EXTERN _heap_init
        EXTERN _port_init
        EXTERN _scheduler_init
        EXTERN _thread_init
        EXTERN _trace_init
        EXTERN _vt_init
        EXTERN chDbgCheckClassI
        EXTERN chThdCreateStatic
        EXTERN ch_debug
        EXTERN dbg_check_enable
        EXTERN dbg_check_lock_from_isr
        EXTERN dbg_check_unlock_from_isr
        EXTERN rlist
        EXTERN vtlist

        PUBLIC _idle_thread
        PUBLIC _idle_thread_wa
        PUBLIC chSysInit
        PUBLIC chSysTimerHandlerI
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\kernel\src\chsys.c
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
//   29  * @file    chsys.c
//   30  * @brief   System related code.
//   31  *
//   32  * @addtogroup system
//   33  * @details System related APIs and services:
//   34  *          - Initialization.
//   35  *          - Locks.
//   36  *          - Interrupt Handling.
//   37  *          - Power Management.
//   38  *          - Abnormal Termination.
//   39  *          .
//   40  * @{
//   41  */
//   42 
//   43 #include "ch.h"
//   44 
//   45 #if !CH_NO_IDLE_THREAD || defined(__DOXYGEN__)
//   46 /**
//   47  * @brief   Idle thread working area.
//   48  */

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   49 WORKING_AREA(_idle_thread_wa, PORT_IDLE_THREAD_STACK_SIZE);
_idle_thread_wa:
        DS8 1336
//   50 
//   51 /**
//   52  * @brief   This function implements the idle thread infinite loop.
//   53  * @details The function puts the processor in the lowest power mode capable
//   54  *          to serve interrupts.<br>
//   55  *          The priority is internally set to the minimum system value so
//   56  *          that this thread is executed only if there are no other ready
//   57  *          threads in the system.
//   58  *
//   59  * @param[in] p the thread parameter, unused in this scenario
//   60  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _idle_thread
          CFI NoCalls
        THUMB
//   61 void _idle_thread(void *p) {
//   62 
//   63   (void)p;
//   64   chRegSetThreadName("idle");
_idle_thread:
        LDR.N    R1,??DataTable2
        LDR      R1,[R1, #+24]
        LDR.N    R2,??DataTable2_1
        STR      R2,[R1, #+24]
//   65   while (TRUE) {
??_idle_thread_0:
        B.N      ??_idle_thread_0
          CFI EndBlock cfiBlock0
//   66     port_wait_for_interrupt();
//   67     IDLE_LOOP_HOOK();
//   68   }
//   69 }
//   70 #endif /* CH_NO_IDLE_THREAD */
//   71 
//   72 /**
//   73  * @brief   ChibiOS/RT initialization.
//   74  * @details After executing this function the current instructions stream
//   75  *          becomes the main thread.
//   76  * @pre     Interrupts must be still disabled when @p chSysInit() is invoked
//   77  *          and are internally enabled.
//   78  * @post    The main thread is created with priority @p NORMALPRIO.
//   79  * @note    This function has special, architecture-dependent, requirements,
//   80  *          see the notes into the various port reference manuals.
//   81  *
//   82  * @special
//   83  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function chSysInit
        THUMB
//   84 void chSysInit(void) {
chSysInit:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   85   static Thread mainthread;
//   86 #if CH_DBG_ENABLE_STACK_CHECK
//   87   extern stkalign_t __main_thread_stack_base__;
//   88 #endif
//   89 
//   90   port_init();
          CFI FunCall _port_init
        BL       _port_init
//   91   _scheduler_init();
          CFI FunCall _scheduler_init
        BL       _scheduler_init
//   92   _vt_init();
          CFI FunCall _vt_init
        BL       _vt_init
//   93 #if CH_USE_MEMCORE
//   94   _core_init();
          CFI FunCall _core_init
        BL       _core_init
//   95 #endif
//   96 #if CH_USE_HEAP
//   97   _heap_init();
          CFI FunCall _heap_init
        BL       _heap_init
//   98 #endif
//   99 #if CH_DBG_ENABLE_TRACE
//  100   _trace_init();
          CFI FunCall _trace_init
        BL       _trace_init
//  101 #endif
//  102 
//  103   /* Now this instructions flow becomes the main thread.*/
//  104   setcurrp(_thread_init(&mainthread, NORMALPRIO));
        MOVS     R1,#+64
        LDR.N    R0,??DataTable2_2
          CFI FunCall _thread_init
        BL       _thread_init
        LDR.N    R1,??DataTable2
        STR      R0,[R1, #+24]
//  105   currp->p_state = THD_STATE_CURRENT;
        LDR.N    R0,??DataTable2
        LDR      R0,[R0, #+24]
        MOVS     R1,#+1
        STRB     R1,[R0, #+32]
//  106 #if CH_DBG_ENABLE_STACK_CHECK
//  107   /* This is a special case because the main thread Thread structure is not
//  108      adjacent to its stack area.*/
//  109   currp->p_stklimit = &__main_thread_stack_base__;
        LDR.N    R0,??DataTable2
        LDR      R0,[R0, #+24]
        LDR.N    R1,??DataTable2_3
        STR      R1,[R0, #+28]
//  110 #endif
//  111   chSysEnable();
          CFI FunCall dbg_check_enable
        BL       dbg_check_enable
        MOVS     R0,#+0
        MSR      BASEPRI,R0
        CPSIE    I
//  112 
//  113   /* Note, &ch_debug points to the string "main" if the registry is
//  114      active, else the parameter is ignored.*/
//  115   chRegSetThreadName((const char *)&ch_debug);
        LDR.N    R0,??DataTable2
        LDR      R0,[R0, #+24]
        LDR.N    R1,??DataTable2_4
        STR      R1,[R0, #+24]
//  116 
//  117 #if !CH_NO_IDLE_THREAD
//  118   /* This thread has the lowest priority in the system, its role is just to
//  119      serve interrupts in its context while keeping the lowest energy saving
//  120      mode compatible with the system status.*/
//  121   chThdCreateStatic(_idle_thread_wa, sizeof(_idle_thread_wa), IDLEPRIO,
//  122                     (tfunc_t)_idle_thread, NULL);
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        LDR.N    R3,??DataTable2_5
        MOVS     R2,#+1
        MOV      R1,#+1336
        LDR.N    R0,??DataTable2_6
          CFI FunCall chThdCreateStatic
        BL       chThdCreateStatic
//  123 #endif
//  124 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock1

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
??mainthread:
        DS8 76
//  125 
//  126 /**
//  127  * @brief   Handles time ticks for round robin preemption and timer increments.
//  128  * @details Decrements the remaining time quantum of the running thread
//  129  *          and preempts it when the quantum is used up. Increments system
//  130  *          time and manages the timers.
//  131  * @note    The frequency of the timer determines the system tick granularity
//  132  *          and, together with the @p CH_TIME_QUANTUM macro, the round robin
//  133  *          interval.
//  134  *
//  135  * @iclass
//  136  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function chSysTimerHandlerI
        THUMB
//  137 void chSysTimerHandlerI(void) {
chSysTimerHandlerI:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
//  138 
//  139   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  140 
//  141 #if CH_TIME_QUANTUM > 0
//  142   /* Running thread has not used up quantum yet? */
//  143   if (currp->p_preempt > 0)
        LDR.N    R0,??DataTable2
        LDR      R0,[R0, #+24]
        LDRB     R0,[R0, #+34]
        CMP      R0,#+0
        BEQ.N    ??chSysTimerHandlerI_0
//  144     /* Decrement remaining quantum.*/
//  145     currp->p_preempt--;
        LDR.N    R0,??DataTable2
        LDR      R0,[R0, #+24]
        LDRB     R0,[R0, #+34]
        SUBS     R0,R0,#+1
        LDR.N    R1,??DataTable2
        LDR      R1,[R1, #+24]
        STRB     R0,[R1, #+34]
//  146 #endif
//  147 #if CH_DBG_THREADS_PROFILING
//  148   currp->p_time++;
??chSysTimerHandlerI_0:
        LDR.N    R0,??DataTable2
        LDR      R0,[R0, #+24]
        LDR      R0,[R0, #+36]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable2
        LDR      R1,[R1, #+24]
        STR      R0,[R1, #+36]
//  149 #endif
//  150   chVTDoTickI();
        LDR.N    R0,??DataTable2_7
        LDR      R0,[R0, #+12]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable2_7
        STR      R0,[R1, #+12]
        LDR.N    R0,??DataTable2_7
        LDR.N    R1,??DataTable2_7
        LDR      R1,[R1, #+0]
        CMP      R0,R1
        BEQ.N    ??chSysTimerHandlerI_1
        LDR.N    R0,??DataTable2_7
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+8]
        SUBS     R0,R0,#+1
        LDR.N    R1,??DataTable2_7
        LDR      R1,[R1, #+0]
        STR      R0,[R1, #+8]
??chSysTimerHandlerI_2:
        LDR.N    R0,??DataTable2_7
        LDR      R0,[R0, #+0]
        MOVS     R4,R0
        LDR      R0,[R0, #+8]
        CMP      R0,#+0
        BNE.N    ??chSysTimerHandlerI_1
        LDR      R0,[R4, #+12]
        MOVS     R5,R0
        MOVS     R0,#+0
        STR      R0,[R4, #+12]
        LDR      R0,[R4, #+0]
        LDR.N    R1,??DataTable2_7
        STR      R1,[R0, #+4]
        LDR.N    R0,??DataTable2_7
        LDR      R1,[R4, #+0]
        STR      R1,[R0, #+0]
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
        LDR      R0,[R4, #+16]
          CFI FunCall
        BLX      R5
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
        B.N      ??chSysTimerHandlerI_2
//  151 #if defined(SYSTEM_TICK_EVENT_HOOK)
//  152   SYSTEM_TICK_EVENT_HOOK();
//  153 #endif
//  154 }
??chSysTimerHandlerI_1:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_1:
        DC32     `?<Constant "idle">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_2:
        DC32     ??mainthread

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_3:
        DC32     __main_thread_stack_base__

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_4:
        DC32     ch_debug

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_5:
        DC32     _idle_thread

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_6:
        DC32     _idle_thread_wa

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_7:
        DC32     vtlist

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "idle">`:
        DATA
        DC8 "idle"
        DC8 0, 0, 0

        END
//  155 
//  156 /** @} */
// 
// 1 412 bytes in section .bss
//     8 bytes in section .rodata
//   282 bytes in section .text
// 
//   282 bytes of CODE  memory
//     8 bytes of CONST memory
// 1 412 bytes of DATA  memory
//
//Errors: none
//Warnings: none
