///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:00 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chthreads.c                                   /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chthreads.c -lCN F:\stuff\rusefi_sourceforge\ /
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
//                       chthreads.s                                          /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME chthreads

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _port_thread_start
        EXTERN chDbgCheckClassI
        EXTERN chDbgPanic
        EXTERN chDbgPanic3
        EXTERN chSchDoRescheduleBehind
        EXTERN chSchGoSleepS
        EXTERN chSchGoSleepTimeoutS
        EXTERN chSchRescheduleS
        EXTERN chSchWakeupS
        EXTERN dbg_check_lock
        EXTERN dbg_check_unlock
        EXTERN rlist
        EXTERN vtlist

        PUBLIC _thread_init
        PUBLIC _thread_memfill
        PUBLIC chThdCreateI
        PUBLIC chThdCreateStatic
        PUBLIC chThdExit
        PUBLIC chThdExitS
        PUBLIC chThdResume
        PUBLIC chThdSetPriority
        PUBLIC chThdSleep
        PUBLIC chThdSleepUntil
        PUBLIC chThdTerminate
        PUBLIC chThdYield
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\kernel\src\chthreads.c
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
//   29  * @file    chthreads.c
//   30  * @brief   Threads code.
//   31  *
//   32  * @addtogroup threads
//   33  * @details Threads related APIs and services.
//   34  *
//   35  *          <h2>Operation mode</h2>
//   36  *          A thread is an abstraction of an independent instructions flow.
//   37  *          In ChibiOS/RT a thread is represented by a "C" function owning
//   38  *          a processor context, state informations and a dedicated stack
//   39  *          area. In this scenario static variables are shared among all
//   40  *          threads while automatic variables are local to the thread.<br>
//   41  *          Operations defined for threads:
//   42  *          - <b>Create</b>, a thread is started on the specified thread
//   43  *            function. This operation is available in multiple variants,
//   44  *            both static and dynamic.
//   45  *          - <b>Exit</b>, a thread terminates by returning from its top
//   46  *            level function or invoking a specific API, the thread can
//   47  *            return a value that can be retrieved by other threads.
//   48  *          - <b>Wait</b>, a thread waits for the termination of another
//   49  *            thread and retrieves its return value.
//   50  *          - <b>Resume</b>, a thread created in suspended state is started.
//   51  *          - <b>Sleep</b>, the execution of a thread is suspended for the
//   52  *            specified amount of time or the specified future absolute time
//   53  *            is reached.
//   54  *          - <b>SetPriority</b>, a thread changes its own priority level.
//   55  *          - <b>Yield</b>, a thread voluntarily renounces to its time slot.
//   56  *          .
//   57  *          The threads subsystem is implicitly included in kernel however
//   58  *          some of its part may be excluded by disabling them in @p chconf.h,
//   59  *          see the @p CH_USE_WAITEXIT and @p CH_USE_DYNAMIC configuration
//   60  *          options.
//   61  * @{
//   62  */
//   63 
//   64 #include "ch.h"
//   65 
//   66 /**
//   67  * @brief   Initializes a thread structure.
//   68  * @note    This is an internal functions, do not use it in application code.
//   69  *
//   70  * @param[in] tp        pointer to the thread
//   71  * @param[in] prio      the priority level for the new thread
//   72  * @return              The same thread pointer passed as parameter.
//   73  *
//   74  * @notapi
//   75  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _thread_init
          CFI NoCalls
        THUMB
//   76 Thread *_thread_init(Thread *tp, tprio_t prio) {
//   77 
//   78   tp->p_prio = prio;
_thread_init:
        STR      R1,[R0, #+8]
//   79   tp->p_state = THD_STATE_SUSPENDED;
        MOVS     R2,#+2
        STRB     R2,[R0, #+32]
//   80   tp->p_flags = THD_MEM_MODE_STATIC;
        MOVS     R2,#+0
        STRB     R2,[R0, #+33]
//   81 #if CH_TIME_QUANTUM > 0
//   82   tp->p_preempt = CH_TIME_QUANTUM;
        MOVS     R2,#+20
        STRB     R2,[R0, #+34]
//   83 #endif
//   84 #if CH_USE_MUTEXES
//   85   tp->p_realprio = prio;
        STR      R1,[R0, #+64]
//   86   tp->p_mtxlist = NULL;
        MOVS     R2,#+0
        STR      R2,[R0, #+60]
//   87 #endif
//   88 #if CH_USE_EVENTS
//   89   tp->p_epending = 0;
        MOVS     R2,#+0
        STR      R2,[R0, #+56]
//   90 #endif
//   91 #if CH_DBG_THREADS_PROFILING
//   92   tp->p_time = 0;
        MOVS     R2,#+0
        STR      R2,[R0, #+36]
//   93 #endif
//   94 #if CH_USE_DYNAMIC
//   95   tp->p_refs = 1;
//   96 #endif
//   97 #if CH_USE_REGISTRY
//   98   tp->p_name = NULL;
        MOVS     R2,#+0
        STR      R2,[R0, #+24]
//   99   REG_INSERT(tp);
        LDR.N    R2,??DataTable7
        STR      R2,[R0, #+16]
        LDR.N    R2,??DataTable7
        LDR      R2,[R2, #+20]
        STR      R2,[R0, #+20]
        LDR.N    R2,??DataTable7
        STR      R0,[R2, #+20]
        LDR      R2,[R0, #+20]
        STR      R0,[R2, #+16]
//  100 #endif
//  101 #if CH_USE_WAITEXIT
//  102   list_init(&tp->p_waiting);
//  103 #endif
//  104 #if CH_USE_MESSAGES
//  105   queue_init(&tp->p_msgqueue);
        ADDS     R2,R0,#+44
        STR      R2,[R0, #+48]
        STR      R2,[R0, #+44]
//  106 #endif
//  107 #if CH_DBG_ENABLE_STACK_CHECK
//  108   tp->p_stklimit = (stkalign_t *)(tp + 1);
        ADDS     R2,R0,#+76
        STR      R2,[R0, #+28]
//  109 #endif
//  110 #if defined(THREAD_EXT_INIT_HOOK)
//  111   THREAD_EXT_INIT_HOOK(tp);
//  112 #endif
//  113   return tp;
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//  114 }
//  115 
//  116 #if CH_DBG_FILL_THREADS || defined(__DOXYGEN__)
//  117 /**
//  118  * @brief   Memory fill utility.
//  119  *
//  120  * @param[in] startp    first address to fill
//  121  * @param[in] endp      last address to fill +1
//  122  * @param[in] v         filler value
//  123  *
//  124  * @notapi
//  125  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _thread_memfill
          CFI NoCalls
        THUMB
//  126 void _thread_memfill(uint8_t *startp, uint8_t *endp, uint8_t v) {
//  127 
//  128   while (startp < endp)
_thread_memfill:
??_thread_memfill_0:
        CMP      R0,R1
        BCS.N    ??_thread_memfill_1
//  129     *startp++ = v;
        STRB     R2,[R0, #+0]
        ADDS     R0,R0,#+1
        B.N      ??_thread_memfill_0
//  130 }
??_thread_memfill_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//  131 #endif /* CH_DBG_FILL_THREADS */
//  132 
//  133 /**
//  134  * @brief   Creates a new thread into a static memory area.
//  135  * @details The new thread is initialized but not inserted in the ready list,
//  136  *          the initial state is @p THD_STATE_SUSPENDED.
//  137  * @post    The initialized thread can be subsequently started by invoking
//  138  *          @p chThdResume(), @p chThdResumeI() or @p chSchWakeupS()
//  139  *          depending on the execution context.
//  140  * @note    A thread can terminate by calling @p chThdExit() or by simply
//  141  *          returning from its main function.
//  142  * @note    Threads created using this function do not obey to the
//  143  *          @p CH_DBG_FILL_THREADS debug option because it would keep
//  144  *          the kernel locked for too much time.
//  145  *
//  146  * @param[out] wsp      pointer to a working area dedicated to the thread stack
//  147  * @param[in] size      size of the working area
//  148  * @param[in] prio      the priority level for the new thread
//  149  * @param[in] pf        the thread function
//  150  * @param[in] arg       an argument passed to the thread function. It can be
//  151  *                      @p NULL.
//  152  * @return              The pointer to the @p Thread structure allocated for
//  153  *                      the thread into the working space area.
//  154  *
//  155  * @iclass
//  156  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function chThdCreateI
        THUMB
//  157 Thread *chThdCreateI(void *wsp, size_t size,
//  158                      tprio_t prio, tfunc_t pf, void *arg) {
chThdCreateI:
        PUSH     {R3-R9,LR}
          CFI R14 Frame(CFA, -4)
          CFI R9 Frame(CFA, -8)
          CFI R8 Frame(CFA, -12)
          CFI R7 Frame(CFA, -16)
          CFI R6 Frame(CFA, -20)
          CFI R5 Frame(CFA, -24)
          CFI R4 Frame(CFA, -28)
          CFI CFA R13+32
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
        LDR      R8,[SP, #+32]
//  159   /* Thread structure is laid out in the lower part of the thread workspace.*/
//  160   Thread *tp = wsp;
        MOV      R9,R4
//  161 
//  162   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  163 
//  164   chDbgCheck((wsp != NULL) && (size >= THD_WA_SIZE(0)) &&
//  165              (prio <= HIGHPRIO) && (pf != NULL),
//  166              "chThdCreateI");
        CMP      R4,#+0
        BEQ.N    ??chThdCreateI_0
        CMP      R5,#+312
        BCC.N    ??chThdCreateI_0
        CMP      R6,#+128
        BCS.N    ??chThdCreateI_0
        MOVS     R0,R7
        CMP      R0,#+0
        BNE.N    ??chThdCreateI_1
??chThdCreateI_0:
        MOVS     R2,#+166
        LDR.N    R1,??DataTable7_1
        LDR.N    R0,??DataTable7_2
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  167   SETUP_CONTEXT(wsp, size, pf, arg);
??chThdCreateI_1:
        ADDS     R0,R5,R4
        SUBS     R0,R0,#+100
        STR      R0,[R9, #+12]
        LDR      R0,[R9, #+12]
        STR      R7,[R0, #+64]
        LDR      R0,[R9, #+12]
        STR      R8,[R0, #+68]
        LDR      R0,[R9, #+12]
        LDR.N    R1,??DataTable7_3
        STR      R1,[R0, #+96]
//  168   return _thread_init(tp, prio);
        MOVS     R1,R6
        MOV      R0,R9
          CFI FunCall _thread_init
        BL       _thread_init
        POP      {R1,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock2
//  169 }
//  170 
//  171 /**
//  172  * @brief   Creates a new thread into a static memory area.
//  173  * @note    A thread can terminate by calling @p chThdExit() or by simply
//  174  *          returning from its main function.
//  175  *
//  176  * @param[out] wsp      pointer to a working area dedicated to the thread stack
//  177  * @param[in] size      size of the working area
//  178  * @param[in] prio      the priority level for the new thread
//  179  * @param[in] pf        the thread function
//  180  * @param[in] arg       an argument passed to the thread function. It can be
//  181  *                      @p NULL.
//  182  * @return              The pointer to the @p Thread structure allocated for
//  183  *                      the thread into the working space area.
//  184  *
//  185  * @api
//  186  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function chThdCreateStatic
        THUMB
//  187 Thread *chThdCreateStatic(void *wsp, size_t size,
//  188                           tprio_t prio, tfunc_t pf, void *arg) {
chThdCreateStatic:
        PUSH     {R3-R9,LR}
          CFI R14 Frame(CFA, -4)
          CFI R9 Frame(CFA, -8)
          CFI R8 Frame(CFA, -12)
          CFI R7 Frame(CFA, -16)
          CFI R6 Frame(CFA, -20)
          CFI R5 Frame(CFA, -24)
          CFI R4 Frame(CFA, -28)
          CFI CFA R13+32
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
        LDR      R8,[SP, #+32]
//  189   Thread *tp;
//  190   
//  191 #if CH_DBG_FILL_THREADS
//  192   _thread_memfill((uint8_t *)wsp,
//  193                   (uint8_t *)wsp + sizeof(Thread),
//  194                   CH_THREAD_FILL_VALUE);
        MOVS     R2,#+255
        ADDS     R1,R4,#+76
        MOVS     R0,R4
          CFI FunCall _thread_memfill
        BL       _thread_memfill
//  195   _thread_memfill((uint8_t *)wsp + sizeof(Thread),
//  196                   (uint8_t *)wsp + size,
//  197                   CH_STACK_FILL_VALUE);
        MOVS     R2,#+85
        ADDS     R1,R5,R4
        ADDS     R0,R4,#+76
          CFI FunCall _thread_memfill
        BL       _thread_memfill
//  198 #endif
//  199   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  200   chSchWakeupS(tp = chThdCreateI(wsp, size, prio, pf, arg), RDY_OK);
        STR      R8,[SP, #+0]
        MOVS     R3,R7
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall chThdCreateI
        BL       chThdCreateI
        MOV      R9,R0
        MOVS     R1,#+0
          CFI FunCall chSchWakeupS
        BL       chSchWakeupS
//  201   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  202   return tp;
        MOV      R0,R9
        POP      {R1,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock3
//  203 }
//  204 
//  205 /**
//  206  * @brief   Changes the running thread priority level then reschedules if
//  207  *          necessary.
//  208  * @note    The function returns the real thread priority regardless of the
//  209  *          current priority that could be higher than the real priority
//  210  *          because the priority inheritance mechanism.
//  211  *
//  212  * @param[in] newprio   the new priority level of the running thread
//  213  * @return              The old priority level.
//  214  *
//  215  * @api
//  216  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function chThdSetPriority
        THUMB
//  217 tprio_t chThdSetPriority(tprio_t newprio) {
chThdSetPriority:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  218   tprio_t oldprio;
//  219 
//  220   chDbgCheck(newprio <= HIGHPRIO, "chThdSetPriority");
        CMP      R4,#+128
        BCC.N    ??chThdSetPriority_0
        MOVS     R2,#+220
        LDR.N    R1,??DataTable7_1
        LDR.N    R0,??DataTable7_4
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  221 
//  222   chSysLock();
??chThdSetPriority_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  223 #if CH_USE_MUTEXES
//  224   oldprio = currp->p_realprio;
        LDR.N    R0,??DataTable7
        LDR      R0,[R0, #+24]
        LDR      R0,[R0, #+64]
        MOVS     R5,R0
//  225   if ((currp->p_prio == currp->p_realprio) || (newprio > currp->p_prio))
        LDR.N    R0,??DataTable7
        LDR      R0,[R0, #+24]
        LDR      R0,[R0, #+8]
        LDR.N    R1,??DataTable7
        LDR      R1,[R1, #+24]
        LDR      R1,[R1, #+64]
        CMP      R0,R1
        BEQ.N    ??chThdSetPriority_1
        LDR.N    R0,??DataTable7
        LDR      R0,[R0, #+24]
        LDR      R0,[R0, #+8]
        CMP      R0,R4
        BCS.N    ??chThdSetPriority_2
//  226     currp->p_prio = newprio;
??chThdSetPriority_1:
        LDR.N    R0,??DataTable7
        LDR      R0,[R0, #+24]
        STR      R4,[R0, #+8]
//  227   currp->p_realprio = newprio;
??chThdSetPriority_2:
        LDR.N    R0,??DataTable7
        LDR      R0,[R0, #+24]
        STR      R4,[R0, #+64]
//  228 #else
//  229   oldprio = currp->p_prio;
//  230   currp->p_prio = newprio;
//  231 #endif
//  232   chSchRescheduleS();
          CFI FunCall chSchRescheduleS
        BL       chSchRescheduleS
//  233   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  234   return oldprio;
        MOVS     R0,R5
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock4
//  235 }
//  236 
//  237 /**
//  238  * @brief   Resumes a suspended thread.
//  239  * @pre     The specified thread pointer must refer to an initialized thread
//  240  *          in the @p THD_STATE_SUSPENDED state.
//  241  * @post    The specified thread is immediately started or put in the ready
//  242  *          list depending on the relative priority levels.
//  243  * @note    Use this function to start threads created with @p chThdCreateI().
//  244  *
//  245  * @param[in] tp        pointer to the thread
//  246  * @return              The pointer to the thread.
//  247  *
//  248  * @api
//  249  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function chThdResume
        THUMB
//  250 Thread *chThdResume(Thread *tp) {
chThdResume:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  251 
//  252   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  253   chDbgAssert(tp->p_state == THD_STATE_SUSPENDED,
//  254               "chThdResume(), #1",
//  255               "thread not in THD_STATE_SUSPENDED state");
        LDRB     R0,[R4, #+32]
        CMP      R0,#+2
        BEQ.N    ??chThdResume_0
        LDR.N    R0,??DataTable7_5
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  256   chSchWakeupS(tp, RDY_OK);
??chThdResume_0:
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall chSchWakeupS
        BL       chSchWakeupS
//  257   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  258   return tp;
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock5
//  259 }
//  260 
//  261 /**
//  262  * @brief   Requests a thread termination.
//  263  * @pre     The target thread must be written to invoke periodically
//  264  *          @p chThdShouldTerminate() and terminate cleanly if it returns
//  265  *          @p TRUE.
//  266  * @post    The specified thread will terminate after detecting the termination
//  267  *          condition.
//  268  *
//  269  * @param[in] tp        pointer to the thread
//  270  *
//  271  * @api
//  272  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function chThdTerminate
        THUMB
//  273 void chThdTerminate(Thread *tp) {
chThdTerminate:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  274 
//  275   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  276   tp->p_flags |= THD_TERMINATE;
        LDRB     R0,[R4, #+33]
        ORRS     R0,R0,#0x4
        STRB     R0,[R4, #+33]
//  277   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  278 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock6
//  279 
//  280 /**
//  281  * @brief   Suspends the invoking thread for the specified time.
//  282  *
//  283  * @param[in] time      the delay in system ticks, the special values are
//  284  *                      handled as follow:
//  285  *                      - @a TIME_INFINITE the thread enters an infinite sleep
//  286  *                        state.
//  287  *                      - @a TIME_IMMEDIATE this value is not allowed.
//  288  *                      .
//  289  *
//  290  * @api
//  291  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function chThdSleep
        THUMB
//  292 void chThdSleep(systime_t time) {
chThdSleep:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  293 
//  294   chDbgCheck(time != TIME_IMMEDIATE, "chThdSleep");
        CMP      R4,#+0
        BNE.N    ??chThdSleep_0
        MOV      R2,#+294
        LDR.N    R1,??DataTable7_1
        LDR.N    R0,??DataTable7_6
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  295 
//  296   chSysLock();
??chThdSleep_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  297   chThdSleepS(time);
        MOVS     R1,R4
        MOVS     R0,#+6
          CFI FunCall chSchGoSleepTimeoutS
        BL       chSchGoSleepTimeoutS
//  298   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  299 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock7
//  300 
//  301 /**
//  302  * @brief   Suspends the invoking thread until the system time arrives to the
//  303  *          specified value.
//  304  *
//  305  * @param[in] time      absolute system time
//  306  *
//  307  * @api
//  308  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function chThdSleepUntil
        THUMB
//  309 void chThdSleepUntil(systime_t time) {
chThdSleepUntil:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  310 
//  311   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  312   if ((time -= chTimeNow()) > 0)
        LDR.N    R0,??DataTable7_7
        LDR      R0,[R0, #+12]
        SUBS     R4,R4,R0
        CMP      R4,#+0
        BEQ.N    ??chThdSleepUntil_0
//  313     chThdSleepS(time);
        MOVS     R1,R4
        MOVS     R0,#+6
          CFI FunCall chSchGoSleepTimeoutS
        BL       chSchGoSleepTimeoutS
//  314   chSysUnlock();
??chThdSleepUntil_0:
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  315 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock8
//  316 
//  317 /**
//  318  * @brief   Yields the time slot.
//  319  * @details Yields the CPU control to the next thread in the ready list with
//  320  *          equal priority, if any.
//  321  *
//  322  * @api
//  323  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function chThdYield
        THUMB
//  324 void chThdYield(void) {
chThdYield:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  325 
//  326   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  327   chSchDoYieldS();
        LDR.N    R0,??DataTable7
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+8]
        LDR.N    R1,??DataTable7
        LDR      R1,[R1, #+24]
        LDR      R1,[R1, #+8]
        CMP      R0,R1
        BCC.N    ??chThdYield_0
          CFI FunCall chSchDoRescheduleBehind
        BL       chSchDoRescheduleBehind
//  328   chSysUnlock();
??chThdYield_0:
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  329 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock9
//  330 
//  331 /**
//  332  * @brief   Terminates the current thread.
//  333  * @details The thread goes in the @p THD_STATE_FINAL state holding the
//  334  *          specified exit status code, other threads can retrieve the
//  335  *          exit status code by invoking the function @p chThdWait().
//  336  * @post    Eventual code after this function will never be executed,
//  337  *          this function never returns. The compiler has no way to
//  338  *          know this so do not assume that the compiler would remove
//  339  *          the dead code.
//  340  *
//  341  * @param[in] msg       thread exit code
//  342  *
//  343  * @api
//  344  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function chThdExit
        THUMB
//  345 void chThdExit(msg_t msg) {
chThdExit:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  346 
//  347   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  348   chThdExitS(msg);
        MOVS     R0,R4
          CFI FunCall chThdExitS
        BL       chThdExitS
//  349   /* The thread never returns here.*/
//  350 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock10
//  351 
//  352 /**
//  353  * @brief   Terminates the current thread.
//  354  * @details The thread goes in the @p THD_STATE_FINAL state holding the
//  355  *          specified exit status code, other threads can retrieve the
//  356  *          exit status code by invoking the function @p chThdWait().
//  357  * @post    Eventual code after this function will never be executed,
//  358  *          this function never returns. The compiler has no way to
//  359  *          know this so do not assume that the compiler would remove
//  360  *          the dead code.
//  361  *
//  362  * @param[in] msg       thread exit code
//  363  *
//  364  * @sclass
//  365  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function chThdExitS
        THUMB
//  366 void chThdExitS(msg_t msg) {
chThdExitS:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  367   Thread *tp = currp;
        LDR.N    R0,??DataTable7
        LDR      R5,[R0, #+24]
//  368 
//  369   tp->p_u.exitcode = msg;
        STR      R4,[R5, #+40]
//  370 #if defined(THREAD_EXT_EXIT_HOOK)
//  371   THREAD_EXT_EXIT_HOOK(tp);
//  372 #endif
//  373 #if CH_USE_WAITEXIT
//  374   while (notempty(&tp->p_waiting))
//  375     chSchReadyI(list_remove(&tp->p_waiting));
//  376 #endif
//  377 #if CH_USE_REGISTRY
//  378   /* Static threads are immediately removed from the registry because
//  379      there is no memory to recover.*/
//  380   if ((tp->p_flags & THD_MEM_MODE_MASK) == THD_MEM_MODE_STATIC)
        LDRB     R0,[R5, #+33]
        MOVS     R1,#+3
        TST      R0,R1
        BNE.N    ??chThdExitS_0
//  381     REG_REMOVE(tp);
        LDR      R0,[R5, #+20]
        LDR      R1,[R5, #+16]
        STR      R1,[R0, #+16]
        LDR      R0,[R5, #+16]
        LDR      R1,[R5, #+20]
        STR      R1,[R0, #+20]
//  382 #endif
//  383   chSchGoSleepS(THD_STATE_FINAL);
??chThdExitS_0:
        MOVS     R0,#+14
          CFI FunCall chSchGoSleepS
        BL       chSchGoSleepS
//  384   /* The thread never returns here.*/
//  385   chDbgAssert(FALSE, "chThdExitS(), #1", "zombies apocalypse");
        LDR.N    R0,??DataTable7_8
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  386 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock11

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_1:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_2:
        DC32     `?<Constant "\\"chThdCreateI\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_3:
        DC32     _port_thread_start

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_4:
        DC32     `?<Constant "\\"chThdSetPriority\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_5:
        DC32     `?<Constant "chThdResume(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_6:
        DC32     `?<Constant "\\"chThdSleep\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_7:
        DC32     vtlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_8:
        DC32     `?<Constant "chThdExitS(), #1">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chThdCreateI\\"()">`:
        DATA
        DC8 "\"chThdCreateI\"()"
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
        DC8 72H, 63H, 5CH, 63H, 68H, 74H, 68H, 72H
        DC8 65H, 61H, 64H, 73H, 2EH, 63H, 0
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chThdSetPriority\\"()">`:
        DATA
        DC8 "\"chThdSetPriority\"()"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "chThdResume(), #1">`:
        DATA
        DC8 "chThdResume(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chThdSleep\\"()">`:
        DATA
        DC8 "\"chThdSleep\"()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "chThdExitS(), #1">`:
        DATA
        DC8 "chThdExitS(), #1"
        DC8 0, 0, 0

        END
//  387 
//  388 #if CH_USE_WAITEXIT || defined(__DOXYGEN__)
//  389 /**
//  390  * @brief   Blocks the execution of the invoking thread until the specified
//  391  *          thread terminates then the exit code is returned.
//  392  * @details This function waits for the specified thread to terminate then
//  393  *          decrements its reference counter, if the counter reaches zero then
//  394  *          the thread working area is returned to the proper allocator.<br>
//  395  *          The memory used by the exited thread is handled in different ways
//  396  *          depending on the API that spawned the thread:
//  397  *          - If the thread was spawned by @p chThdCreateStatic() or by
//  398  *            @p chThdCreateI() then nothing happens and the thread working
//  399  *            area is not released or modified in any way. This is the
//  400  *            default, totally static, behavior.
//  401  *          - If the thread was spawned by @p chThdCreateFromHeap() then
//  402  *            the working area is returned to the system heap.
//  403  *          - If the thread was spawned by @p chThdCreateFromMemoryPool()
//  404  *            then the working area is returned to the owning memory pool.
//  405  *          .
//  406  * @pre     The configuration option @p CH_USE_WAITEXIT must be enabled in
//  407  *          order to use this function.
//  408  * @post    Enabling @p chThdWait() requires 2-4 (depending on the
//  409  *          architecture) extra bytes in the @p Thread structure.
//  410  * @post    After invoking @p chThdWait() the thread pointer becomes invalid
//  411  *          and must not be used as parameter for further system calls.
//  412  * @note    If @p CH_USE_DYNAMIC is not specified this function just waits for
//  413  *          the thread termination, no memory allocators are involved.
//  414  *
//  415  * @param[in] tp        pointer to the thread
//  416  * @return              The exit code from the terminated thread.
//  417  *
//  418  * @api
//  419  */
//  420 msg_t chThdWait(Thread *tp) {
//  421   msg_t msg;
//  422 
//  423   chDbgCheck(tp != NULL, "chThdWait");
//  424 
//  425   chSysLock();
//  426   chDbgAssert(tp != currp, "chThdWait(), #1", "waiting self");
//  427 #if CH_USE_DYNAMIC
//  428   chDbgAssert(tp->p_refs > 0, "chThdWait(), #2", "not referenced");
//  429 #endif
//  430   if (tp->p_state != THD_STATE_FINAL) {
//  431     list_insert(currp, &tp->p_waiting);
//  432     chSchGoSleepS(THD_STATE_WTEXIT);
//  433   }
//  434   msg = tp->p_u.exitcode;
//  435   chSysUnlock();
//  436 #if CH_USE_DYNAMIC
//  437   chThdRelease(tp);
//  438 #endif
//  439   return msg;
//  440 }
//  441 #endif /* CH_USE_WAITEXIT */
//  442 
//  443 /** @} */
// 
// 172 bytes in section .rodata
// 690 bytes in section .text
// 
// 690 bytes of CODE  memory
// 172 bytes of CONST memory
//
//Errors: none
//Warnings: none
