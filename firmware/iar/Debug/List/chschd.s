///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:43 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chschd.c                                      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chschd.c -lCN F:\stuff\rusefi_sourceforge\fir /
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
//                       chschd.s                                             /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME chschd

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _port_switch
        EXTERN chDbgCheckClassI
        EXTERN chDbgCheckClassS
        EXTERN chDbgPanic
        EXTERN chVTResetI
        EXTERN chVTSetI
        EXTERN dbg_check_lock_from_isr
        EXTERN dbg_check_unlock_from_isr
        EXTERN dbg_trace
        EXTERN getRemainingStack

        PUBLIC _scheduler_init
        PUBLIC chSchDoReschedule
        PUBLIC chSchDoRescheduleAhead
        PUBLIC chSchDoRescheduleBehind
        PUBLIC chSchGoSleepS
        PUBLIC chSchGoSleepTimeoutS
        PUBLIC chSchReadyI
        PUBLIC chSchRescheduleS
        PUBLIC chSchWakeupS
        PUBLIC rlist
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\kernel\src\chschd.c
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
//   29  * @file    chschd.c
//   30  * @brief   Scheduler code.
//   31  *
//   32  * @addtogroup scheduler
//   33  * @details This module provides the default portable scheduler code,
//   34  *          scheduler functions can be individually captured by the port
//   35  *          layer in order to provide architecture optimized equivalents.
//   36  *          When a function is captured its default code is not built into
//   37  *          the OS image, the optimized version is included instead.
//   38  * @{
//   39  */
//   40 
//   41 #include "ch.h"

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function fifo_remove
          CFI NoCalls
        THUMB
// static __interwork __softfp struct Thread *fifo_remove(ThreadsQueue *)
fifo_remove:
        MOVS     R1,R0
        LDR      R0,[R1, #+0]
        LDR      R2,[R0, #+0]
        STR      R2,[R1, #+0]
        STR      R1,[R2, #+4]
        BX       LR               ;; return
          CFI EndBlock cfiBlock0

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function dequeue
          CFI NoCalls
        THUMB
// static __interwork __softfp struct Thread *dequeue(struct Thread *)
dequeue:
        LDR      R1,[R0, #+4]
        LDR      R2,[R0, #+0]
        STR      R2,[R1, #+0]
        LDR      R1,[R0, #+0]
        LDR      R2,[R0, #+4]
        STR      R2,[R1, #+4]
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   42 
//   43 /**
//   44  * @brief   Ready list header.
//   45  */
//   46 #if !defined(PORT_OPTIMIZED_RLIST_VAR) || defined(__DOXYGEN__)

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   47 ReadyList rlist;
rlist:
        DS8 28
//   48 #endif /* !defined(PORT_OPTIMIZED_RLIST_VAR) */
//   49 
//   50 /**
//   51  * @brief   Scheduler initialization.
//   52  *
//   53  * @notapi
//   54  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _scheduler_init
          CFI NoCalls
        THUMB
//   55 void _scheduler_init(void) {
//   56 
//   57   queue_init(&rlist.r_queue);
_scheduler_init:
        LDR.N    R0,??DataTable8
        LDR.N    R1,??DataTable8
        STR      R0,[R1, #+4]
        LDR.N    R1,??DataTable8
        STR      R0,[R1, #+0]
//   58   rlist.r_prio = NOPRIO;
        LDR.N    R0,??DataTable8
        MOVS     R1,#+0
        STR      R1,[R0, #+8]
//   59 #if CH_USE_REGISTRY
//   60   rlist.r_newer = rlist.r_older = (Thread *)&rlist;
        LDR.N    R0,??DataTable8
        LDR.N    R1,??DataTable8
        STR      R0,[R1, #+20]
        LDR.N    R1,??DataTable8
        STR      R0,[R1, #+16]
//   61 #endif
//   62 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock2
//   63 
//   64 /**
//   65  * @brief   Inserts a thread in the Ready List.
//   66  * @details The thread is positioned behind all threads with higher or equal
//   67  *          priority.
//   68  * @pre     The thread must not be already inserted in any list through its
//   69  *          @p p_next and @p p_prev or list corruption would occur.
//   70  * @post    This function does not reschedule so a call to a rescheduling
//   71  *          function must be performed before unlocking the kernel. Note that
//   72  *          interrupt handlers always reschedule on exit so an explicit
//   73  *          reschedule must not be performed in ISRs.
//   74  *
//   75  * @param[in] tp        the thread to be made ready
//   76  * @return              The thread pointer.
//   77  *
//   78  * @iclass
//   79  */
//   80 #if !defined(PORT_OPTIMIZED_READYI) || defined(__DOXYGEN__)

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function chSchReadyI
        THUMB
//   81 Thread *chSchReadyI(Thread *tp) {
chSchReadyI:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//   82   Thread *cp;
//   83 
//   84   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//   85 
//   86   /* Integrity checks.*/
//   87   chDbgAssert((tp->p_state != THD_STATE_READY) &&
//   88               (tp->p_state != THD_STATE_FINAL),
//   89               "chSchReadyI(), #1",
//   90               "invalid state");
        LDRB     R0,[R4, #+32]
        CMP      R0,#+0
        BEQ.N    ??chSchReadyI_0
        LDRB     R0,[R4, #+32]
        CMP      R0,#+14
        BNE.N    ??chSchReadyI_1
??chSchReadyI_0:
        LDR.N    R0,??DataTable8_1
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//   91 
//   92   tp->p_state = THD_STATE_READY;
??chSchReadyI_1:
        MOVS     R0,#+0
        STRB     R0,[R4, #+32]
//   93   cp = (Thread *)&rlist.r_queue;
        LDR.N    R0,??DataTable8
        MOVS     R5,R0
//   94   do {
//   95     cp = cp->p_next;
??chSchReadyI_2:
        LDR      R5,[R5, #+0]
//   96   } while (cp->p_prio >= tp->p_prio);
        LDR      R0,[R5, #+8]
        LDR      R1,[R4, #+8]
        CMP      R0,R1
        BCS.N    ??chSchReadyI_2
//   97   /* Insertion on p_prev.*/
//   98   tp->p_next = cp;
        STR      R5,[R4, #+0]
//   99   tp->p_prev = cp->p_prev;
        LDR      R0,[R5, #+4]
        STR      R0,[R4, #+4]
//  100   tp->p_prev->p_next = cp->p_prev = tp;
        STR      R4,[R5, #+4]
        LDR      R0,[R4, #+4]
        STR      R4,[R0, #+0]
//  101   return tp;
        MOVS     R0,R4
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock3
//  102 }
//  103 #endif /* !defined(PORT_OPTIMIZED_READYI) */
//  104 
//  105 /**
//  106  * @brief   Puts the current thread to sleep into the specified state.
//  107  * @details The thread goes into a sleeping state. The possible
//  108  *          @ref thread_states are defined into @p threads.h.
//  109  *
//  110  * @param[in] newstate  the new thread state
//  111  *
//  112  * @sclass
//  113  */
//  114 #if !defined(PORT_OPTIMIZED_GOSLEEPS) || defined(__DOXYGEN__)

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function chSchGoSleepS
        THUMB
//  115 void chSchGoSleepS(tstate_t newstate) {
chSchGoSleepS:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  116   Thread *otp;
//  117 
//  118   chDbgCheckClassS();
          CFI FunCall chDbgCheckClassS
        BL       chDbgCheckClassS
//  119 
//  120   (otp = currp)->p_state = newstate;
        LDR.N    R0,??DataTable8
        LDR      R0,[R0, #+24]
        MOVS     R5,R0
        STRB     R4,[R0, #+32]
//  121 #if CH_TIME_QUANTUM > 0
//  122   /* The thread is renouncing its remaining time slices so it will have a new
//  123      time quantum when it will wakeup.*/
//  124   otp->p_preempt = CH_TIME_QUANTUM;
        MOVS     R0,#+20
        STRB     R0,[R5, #+34]
//  125 #endif
//  126   setcurrp(fifo_remove(&rlist.r_queue));
        LDR.N    R0,??DataTable8
          CFI FunCall fifo_remove
        BL       fifo_remove
        LDR.N    R1,??DataTable8
        STR      R0,[R1, #+24]
//  127   currp->p_state = THD_STATE_CURRENT;
        LDR.N    R0,??DataTable8
        LDR      R0,[R0, #+24]
        MOVS     R1,#+1
        STRB     R1,[R0, #+32]
//  128   chSysSwitch(currp, otp);
        MOVS     R0,R5
          CFI FunCall dbg_trace
        BL       dbg_trace
        MOVS     R0,R5
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+0
        BPL.N    ??chSchGoSleepS_0
        LDR.N    R0,??DataTable8_2
          CFI FunCall chDbgPanic
        BL       chDbgPanic
??chSchGoSleepS_0:
        MOVS     R1,R5
        LDR.N    R0,??DataTable8
        LDR      R0,[R0, #+24]
          CFI FunCall _port_switch
        BL       _port_switch
//  129 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock4
//  130 #endif /* !defined(PORT_OPTIMIZED_GOSLEEPS) */
//  131 
//  132 #if !defined(PORT_OPTIMIZED_GOSLEEPTIMEOUTS) || defined(__DOXYGEN__)
//  133 /*
//  134  * Timeout wakeup callback.
//  135  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function wakeup
        THUMB
//  136 static void wakeup(void *p) {
wakeup:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  137   Thread *tp = (Thread *)p;
        MOVS     R5,R4
//  138 
//  139   chSysLockFromIsr();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  140   switch (tp->p_state) {
        LDRB     R0,[R5, #+32]
        CMP      R0,#+0
        BEQ.N    ??wakeup_0
        CMP      R0,#+3
        BEQ.N    ??wakeup_1
        CMP      R0,#+13
        BEQ.N    ??wakeup_2
        B.N      ??wakeup_3
//  141   case THD_STATE_READY:
//  142     /* Handling the special case where the thread has been made ready by
//  143        another thread with higher priority.*/
//  144     chSysUnlockFromIsr();
??wakeup_0:
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  145     return;
        B.N      ??wakeup_4
//  146 #if CH_USE_SEMAPHORES || CH_USE_QUEUES ||                                   \ 
//  147     (CH_USE_CONDVARS && CH_USE_CONDVARS_TIMEOUT)
//  148 #if CH_USE_SEMAPHORES
//  149   case THD_STATE_WTSEM:
//  150     chSemFastSignalI((Semaphore *)tp->p_u.wtobjp);
??wakeup_1:
        LDR      R0,[R5, #+40]
        LDR      R0,[R0, #+8]
        ADDS     R0,R0,#+1
        LDR      R1,[R5, #+40]
        STR      R0,[R1, #+8]
//  151     /* Falls into, intentional. */
//  152 #endif
//  153 #if CH_USE_QUEUES
//  154   case THD_STATE_WTQUEUE:
//  155 #endif
//  156 #if CH_USE_CONDVARS && CH_USE_CONDVARS_TIMEOUT
//  157   case THD_STATE_WTCOND:
//  158 #endif
//  159     /* States requiring dequeuing.*/
//  160     dequeue(tp);
??wakeup_2:
        MOVS     R0,R5
          CFI FunCall dequeue
        BL       dequeue
//  161 #endif
//  162   }
//  163   tp->p_u.rdymsg = RDY_TIMEOUT;
??wakeup_3:
        MOVS     R0,#-1
        STR      R0,[R5, #+40]
//  164   chSchReadyI(tp);
        MOVS     R0,R5
          CFI FunCall chSchReadyI
        BL       chSchReadyI
//  165   chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  166 }
??wakeup_4:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock5
//  167 
//  168 /**
//  169  * @brief   Puts the current thread to sleep into the specified state with
//  170  *          timeout specification.
//  171  * @details The thread goes into a sleeping state, if it is not awakened
//  172  *          explicitly within the specified timeout then it is forcibly
//  173  *          awakened with a @p RDY_TIMEOUT low level message. The possible
//  174  *          @ref thread_states are defined into @p threads.h.
//  175  *
//  176  * @param[in] newstate  the new thread state
//  177  * @param[in] time      the number of ticks before the operation timeouts, the
//  178  *                      special values are handled as follow:
//  179  *                      - @a TIME_INFINITE the thread enters an infinite sleep
//  180  *                        state, this is equivalent to invoking
//  181  *                        @p chSchGoSleepS() but, of course, less efficient.
//  182  *                      - @a TIME_IMMEDIATE this value is not allowed.
//  183  *                      .
//  184  * @return              The wakeup message.
//  185  * @retval RDY_TIMEOUT if a timeout occurs.
//  186  *
//  187  * @sclass
//  188  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function chSchGoSleepTimeoutS
        THUMB
//  189 msg_t chSchGoSleepTimeoutS(tstate_t newstate, systime_t time) {
chSchGoSleepTimeoutS:
        PUSH     {R4,R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+12
        SUB      SP,SP,#+20
          CFI CFA R13+32
        MOVS     R4,R0
        MOVS     R5,R1
//  190 
//  191   chDbgCheckClassS();
          CFI FunCall chDbgCheckClassS
        BL       chDbgCheckClassS
//  192 
//  193   if (TIME_INFINITE != time) {
        CMN      R5,#+1
        BEQ.N    ??chSchGoSleepTimeoutS_0
//  194     VirtualTimer vt;
//  195 
//  196     chVTSetI(&vt, time, wakeup, currp);
        LDR.N    R0,??DataTable8
        LDR      R3,[R0, #+24]
        LDR.N    R2,??DataTable8_3
        MOVS     R1,R5
        ADD      R0,SP,#+0
          CFI FunCall chVTSetI
        BL       chVTSetI
//  197     chSchGoSleepS(newstate);
        MOVS     R0,R4
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall chSchGoSleepS
        BL       chSchGoSleepS
//  198     if (chVTIsArmedI(&vt))
        LDR      R0,[SP, #+12]
        CMP      R0,#+0
        BEQ.N    ??chSchGoSleepTimeoutS_1
//  199       chVTResetI(&vt);
        ADD      R0,SP,#+0
          CFI FunCall chVTResetI
        BL       chVTResetI
        B.N      ??chSchGoSleepTimeoutS_1
//  200   }
//  201   else
//  202     chSchGoSleepS(newstate);
??chSchGoSleepTimeoutS_0:
        MOVS     R0,R4
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall chSchGoSleepS
        BL       chSchGoSleepS
//  203   return currp->p_u.rdymsg;
??chSchGoSleepTimeoutS_1:
        LDR.N    R0,??DataTable8
        LDR      R0,[R0, #+24]
        LDR      R0,[R0, #+40]
        ADD      SP,SP,#+20
          CFI CFA R13+12
        POP      {R4,R5,PC}       ;; return
          CFI EndBlock cfiBlock6
//  204 }
//  205 #endif /* !defined(PORT_OPTIMIZED_GOSLEEPTIMEOUTS) */
//  206 
//  207 /**
//  208  * @brief   Wakes up a thread.
//  209  * @details The thread is inserted into the ready list or immediately made
//  210  *          running depending on its relative priority compared to the current
//  211  *          thread.
//  212  * @pre     The thread must not be already inserted in any list through its
//  213  *          @p p_next and @p p_prev or list corruption would occur.
//  214  * @note    It is equivalent to a @p chSchReadyI() followed by a
//  215  *          @p chSchRescheduleS() but much more efficient.
//  216  * @note    The function assumes that the current thread has the highest
//  217  *          priority.
//  218  *
//  219  * @param[in] ntp       the Thread to be made ready
//  220  * @param[in] msg       message to the awakened thread
//  221  *
//  222  * @sclass
//  223  */
//  224 #if !defined(PORT_OPTIMIZED_WAKEUPS) || defined(__DOXYGEN__)

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function chSchWakeupS
        THUMB
//  225 void chSchWakeupS(Thread *ntp, msg_t msg) {
chSchWakeupS:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  226 
//  227   chDbgCheckClassS();
          CFI FunCall chDbgCheckClassS
        BL       chDbgCheckClassS
//  228 
//  229   ntp->p_u.rdymsg = msg;
        STR      R5,[R4, #+40]
//  230   /* If the waken thread has a not-greater priority than the current
//  231      one then it is just inserted in the ready list else it made
//  232      running immediately and the invoking thread goes in the ready
//  233      list instead.*/
//  234   if (ntp->p_prio <= currp->p_prio)
        LDR.N    R0,??DataTable8
        LDR      R0,[R0, #+24]
        LDR      R0,[R0, #+8]
        LDR      R1,[R4, #+8]
        CMP      R0,R1
        BCC.N    ??chSchWakeupS_0
//  235     chSchReadyI(ntp);
        MOVS     R0,R4
          CFI FunCall chSchReadyI
        BL       chSchReadyI
        B.N      ??chSchWakeupS_1
//  236   else {
//  237     Thread *otp = chSchReadyI(currp);
??chSchWakeupS_0:
        LDR.N    R0,??DataTable8
        LDR      R0,[R0, #+24]
          CFI FunCall chSchReadyI
        BL       chSchReadyI
        MOVS     R6,R0
//  238     setcurrp(ntp);
        LDR.N    R0,??DataTable8
        STR      R4,[R0, #+24]
//  239     ntp->p_state = THD_STATE_CURRENT;
        MOVS     R0,#+1
        STRB     R0,[R4, #+32]
//  240     chSysSwitch(ntp, otp);
        MOVS     R0,R6
          CFI FunCall dbg_trace
        BL       dbg_trace
        MOVS     R0,R6
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+0
        BPL.N    ??chSchWakeupS_2
        LDR.N    R0,??DataTable8_2
          CFI FunCall chDbgPanic
        BL       chDbgPanic
??chSchWakeupS_2:
        MOVS     R1,R6
        MOVS     R0,R4
          CFI FunCall _port_switch
        BL       _port_switch
//  241   }
//  242 }
??chSchWakeupS_1:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock7
//  243 #endif /* !defined(PORT_OPTIMIZED_WAKEUPS) */
//  244 
//  245 /**
//  246  * @brief   Performs a reschedule if a higher priority thread is runnable.
//  247  * @details If a thread with a higher priority than the current thread is in
//  248  *          the ready list then make the higher priority thread running.
//  249  *
//  250  * @sclass
//  251  */
//  252 #if !defined(PORT_OPTIMIZED_RESCHEDULES) || defined(__DOXYGEN__)

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function chSchRescheduleS
        THUMB
//  253 void chSchRescheduleS(void) {
chSchRescheduleS:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  254 
//  255   chDbgCheckClassS();
          CFI FunCall chDbgCheckClassS
        BL       chDbgCheckClassS
//  256 
//  257   if (chSchIsRescRequiredI())
        LDR.N    R0,??DataTable8
        LDR      R0,[R0, #+24]
        LDR      R0,[R0, #+8]
        LDR.N    R1,??DataTable8
        LDR      R1,[R1, #+0]
        LDR      R1,[R1, #+8]
        CMP      R0,R1
        BCS.N    ??chSchRescheduleS_0
//  258     chSchDoRescheduleAhead();
          CFI FunCall chSchDoRescheduleAhead
        BL       chSchDoRescheduleAhead
//  259 }
??chSchRescheduleS_0:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock8
//  260 #endif /* !defined(PORT_OPTIMIZED_RESCHEDULES) */
//  261 
//  262 /**
//  263  * @brief   Evaluates if preemption is required.
//  264  * @details The decision is taken by comparing the relative priorities and
//  265  *          depending on the state of the round robin timeout counter.
//  266  * @note    Not a user function, it is meant to be invoked by the scheduler
//  267  *          itself or from within the port layer.
//  268  *
//  269  * @retval TRUE         if there is a thread that must go in running state
//  270  *                      immediately.
//  271  * @retval FALSE        if preemption is not required.
//  272  *
//  273  * @special
//  274  */
//  275 #if !defined(PORT_OPTIMIZED_ISPREEMPTIONREQUIRED) || defined(__DOXYGEN__)
//  276 bool_t chSchIsPreemptionRequired(void) {
//  277   tprio_t p1 = firstprio(&rlist.r_queue);
//  278   tprio_t p2 = currp->p_prio;
//  279 #if CH_TIME_QUANTUM > 0
//  280   /* If the running thread has not reached its time quantum, reschedule only
//  281      if the first thread on the ready queue has a higher priority.
//  282      Otherwise, if the running thread has used up its time quantum, reschedule
//  283      if the first thread on the ready queue has equal or higher priority.*/
//  284   return currp->p_preempt ? p1 > p2 : p1 >= p2;
//  285 #else
//  286   /* If the round robin preemption feature is not enabled then performs a
//  287      simpler comparison.*/
//  288   return p1 > p2;
//  289 #endif
//  290 }
//  291 #endif /* !defined(PORT_OPTIMIZED_ISPREEMPTIONREQUIRED) */
//  292 
//  293 /**
//  294  * @brief   Switches to the first thread on the runnable queue.
//  295  * @details The current thread is positioned in the ready list behind all
//  296  *          threads having the same priority. The thread regains its time
//  297  *          quantum.
//  298  * @note    Not a user function, it is meant to be invoked by the scheduler
//  299  *          itself or from within the port layer.
//  300  *
//  301  * @special
//  302  */
//  303 #if !defined(PORT_OPTIMIZED_DORESCHEDULEBEHIND) || defined(__DOXYGEN__)

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function chSchDoRescheduleBehind
        THUMB
//  304 void chSchDoRescheduleBehind(void) {
chSchDoRescheduleBehind:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  305   Thread *otp;
//  306 
//  307   otp = currp;
        LDR.N    R0,??DataTable8
        LDR      R0,[R0, #+24]
        MOVS     R4,R0
//  308   /* Picks the first thread from the ready queue and makes it current.*/
//  309   setcurrp(fifo_remove(&rlist.r_queue));
        LDR.N    R0,??DataTable8
          CFI FunCall fifo_remove
        BL       fifo_remove
        LDR.N    R1,??DataTable8
        STR      R0,[R1, #+24]
//  310   currp->p_state = THD_STATE_CURRENT;
        LDR.N    R0,??DataTable8
        LDR      R0,[R0, #+24]
        MOVS     R1,#+1
        STRB     R1,[R0, #+32]
//  311 #if CH_TIME_QUANTUM > 0
//  312   otp->p_preempt = CH_TIME_QUANTUM;
        MOVS     R0,#+20
        STRB     R0,[R4, #+34]
//  313 #endif
//  314   chSchReadyI(otp);
        MOVS     R0,R4
          CFI FunCall chSchReadyI
        BL       chSchReadyI
//  315   chSysSwitch(currp, otp);
        MOVS     R0,R4
          CFI FunCall dbg_trace
        BL       dbg_trace
        MOVS     R0,R4
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+0
        BPL.N    ??chSchDoRescheduleBehind_0
        LDR.N    R0,??DataTable8_2
          CFI FunCall chDbgPanic
        BL       chDbgPanic
??chSchDoRescheduleBehind_0:
        MOVS     R1,R4
        LDR.N    R0,??DataTable8
        LDR      R0,[R0, #+24]
          CFI FunCall _port_switch
        BL       _port_switch
//  316 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock9
//  317 #endif /* !defined(PORT_OPTIMIZED_DORESCHEDULEBEHIND) */
//  318 
//  319 /**
//  320  * @brief   Switches to the first thread on the runnable queue.
//  321  * @details The current thread is positioned in the ready list ahead of all
//  322  *          threads having the same priority.
//  323  * @note    Not a user function, it is meant to be invoked by the scheduler
//  324  *          itself or from within the port layer.
//  325  *
//  326  * @special
//  327  */
//  328 #if !defined(PORT_OPTIMIZED_DORESCHEDULEAHEAD) || defined(__DOXYGEN__)

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function chSchDoRescheduleAhead
        THUMB
//  329 void chSchDoRescheduleAhead(void) {
chSchDoRescheduleAhead:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
//  330   Thread *otp, *cp;
//  331 
//  332   otp = currp;
        LDR.N    R0,??DataTable8
        LDR      R0,[R0, #+24]
        MOVS     R4,R0
//  333   /* Picks the first thread from the ready queue and makes it current.*/
//  334   setcurrp(fifo_remove(&rlist.r_queue));
        LDR.N    R0,??DataTable8
          CFI FunCall fifo_remove
        BL       fifo_remove
        LDR.N    R1,??DataTable8
        STR      R0,[R1, #+24]
//  335   currp->p_state = THD_STATE_CURRENT;
        LDR.N    R0,??DataTable8
        LDR      R0,[R0, #+24]
        MOVS     R1,#+1
        STRB     R1,[R0, #+32]
//  336 
//  337   otp->p_state = THD_STATE_READY;
        MOVS     R0,#+0
        STRB     R0,[R4, #+32]
//  338   cp = (Thread *)&rlist.r_queue;
        LDR.N    R0,??DataTable8
        MOVS     R5,R0
//  339   do {
//  340     cp = cp->p_next;
??chSchDoRescheduleAhead_0:
        LDR      R5,[R5, #+0]
//  341   } while (cp->p_prio > otp->p_prio);
        LDR      R0,[R4, #+8]
        LDR      R1,[R5, #+8]
        CMP      R0,R1
        BCC.N    ??chSchDoRescheduleAhead_0
//  342   /* Insertion on p_prev.*/
//  343   otp->p_next = cp;
        STR      R5,[R4, #+0]
//  344   otp->p_prev = cp->p_prev;
        LDR      R0,[R5, #+4]
        STR      R0,[R4, #+4]
//  345   otp->p_prev->p_next = cp->p_prev = otp;
        STR      R4,[R5, #+4]
        LDR      R0,[R4, #+4]
        STR      R4,[R0, #+0]
//  346 
//  347   chSysSwitch(currp, otp);
        MOVS     R0,R4
          CFI FunCall dbg_trace
        BL       dbg_trace
        MOVS     R0,R4
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+0
        BPL.N    ??chSchDoRescheduleAhead_1
        LDR.N    R0,??DataTable8_2
          CFI FunCall chDbgPanic
        BL       chDbgPanic
??chSchDoRescheduleAhead_1:
        MOVS     R1,R4
        LDR.N    R0,??DataTable8
        LDR      R0,[R0, #+24]
          CFI FunCall _port_switch
        BL       _port_switch
//  348 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock10
//  349 #endif /* !defined(PORT_OPTIMIZED_DORESCHEDULEAHEAD) */
//  350 
//  351 /**
//  352  * @brief   Switches to the first thread on the runnable queue.
//  353  * @details The current thread is positioned in the ready list behind or
//  354  *          ahead of all threads having the same priority depending on
//  355  *          if it used its whole time slice.
//  356  * @note    Not a user function, it is meant to be invoked by the scheduler
//  357  *          itself or from within the port layer.
//  358  *
//  359  * @special
//  360  */
//  361 #if !defined(PORT_OPTIMIZED_DORESCHEDULE) || defined(__DOXYGEN__)

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function chSchDoReschedule
        THUMB
//  362 void chSchDoReschedule(void) {
chSchDoReschedule:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  363 
//  364 #if CH_TIME_QUANTUM > 0
//  365   /* If CH_TIME_QUANTUM is enabled then there are two different scenarios to
//  366      handle on preemption: time quantum elapsed or not.*/
//  367   if (currp->p_preempt == 0) {
        LDR.N    R0,??DataTable8
        LDR      R0,[R0, #+24]
        LDRB     R0,[R0, #+34]
        CMP      R0,#+0
        BNE.N    ??chSchDoReschedule_0
//  368     /* The thread consumed its time quantum so it is enqueued behind threads
//  369        with same priority level, however, it acquires a new time quantum.*/
//  370     chSchDoRescheduleBehind();
          CFI FunCall chSchDoRescheduleBehind
        BL       chSchDoRescheduleBehind
        B.N      ??chSchDoReschedule_1
//  371   }
//  372   else {
//  373     /* The thread didn't consume all its time quantum so it is put ahead of
//  374        threads with equal priority and does not acquire a new time quantum.*/
//  375     chSchDoRescheduleAhead();
??chSchDoReschedule_0:
          CFI FunCall chSchDoRescheduleAhead
        BL       chSchDoRescheduleAhead
//  376   }
//  377 #else /* !(CH_TIME_QUANTUM > 0) */
//  378   /* If the round-robin mechanism is disabled then the thread goes always
//  379      ahead of its peers.*/
//  380   chSchDoRescheduleAhead();
//  381 #endif /* !(CH_TIME_QUANTUM > 0) */
//  382 }
??chSchDoReschedule_1:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock11

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_1:
        DC32     `?<Constant "chSchReadyI(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_2:
        DC32     `?<Constant "stack overflow">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_3:
        DC32     wakeup

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "chSchReadyI(), #1">`:
        DATA
        DC8 "chSchReadyI(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "stack overflow">`:
        DATA
        DC8 "stack overflow"
        DC8 0

        END
//  383 #endif /* !defined(PORT_OPTIMIZED_DORESCHEDULE) */
//  384 
//  385 /** @} */
// 
//  28 bytes in section .bss
//  36 bytes in section .rodata
// 678 bytes in section .text
// 
// 678 bytes of CODE  memory
//  36 bytes of CONST memory
//  28 bytes of DATA  memory
//
//Errors: none
//Warnings: none
