///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:42 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chmtx.c                                       /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chmtx.c -lCN F:\stuff\rusefi_sourceforge\firm /
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
//                       chmtx.s                                              /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME chmtx

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN chDbgCheckClassS
        EXTERN chDbgPanic
        EXTERN chDbgPanic3
        EXTERN chSchGoSleepS
        EXTERN chSchReadyI
        EXTERN chSchRescheduleS
        EXTERN chSchWakeupS
        EXTERN dbg_check_lock
        EXTERN dbg_check_unlock
        EXTERN rlist

        PUBLIC chMtxInit
        PUBLIC chMtxLock
        PUBLIC chMtxLockS
        PUBLIC chMtxTryLock
        PUBLIC chMtxTryLockS
        PUBLIC chMtxUnlock
        PUBLIC chMtxUnlockAll
        PUBLIC chMtxUnlockS
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\kernel\src\chmtx.c
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
//   29  * @file    chmtx.c
//   30  * @brief   Mutexes code.
//   31  *
//   32  * @addtogroup mutexes
//   33  * @details Mutexes related APIs and services.
//   34  *
//   35  *          <h2>Operation mode</h2>
//   36  *          A mutex is a threads synchronization object that can be in two
//   37  *          distinct states:
//   38  *          - Not owned (unlocked).
//   39  *          - Owned by a thread (locked).
//   40  *          .
//   41  *          Operations defined for mutexes:
//   42  *          - <b>Lock</b>: The mutex is checked, if the mutex is not owned by
//   43  *            some other thread then it is associated to the locking thread
//   44  *            else the thread is queued on the mutex in a list ordered by
//   45  *            priority.
//   46  *          - <b>Unlock</b>: The mutex is released by the owner and the highest
//   47  *            priority thread waiting in the queue, if any, is resumed and made
//   48  *            owner of the mutex.
//   49  *          .
//   50  *          <h2>Constraints</h2>
//   51  *          In ChibiOS/RT the Unlock operations are always performed in
//   52  *          lock-reverse order. The unlock API does not even have a parameter,
//   53  *          the mutex to unlock is selected from an internal, per-thread, stack
//   54  *          of owned mutexes. This both improves the performance and is
//   55  *          required for an efficient implementation of the priority
//   56  *          inheritance mechanism.
//   57  *
//   58  *          <h2>The priority inversion problem</h2>
//   59  *          The mutexes in ChibiOS/RT implements the <b>full</b> priority
//   60  *          inheritance mechanism in order handle the priority inversion
//   61  *          problem.<br>
//   62  *          When a thread is queued on a mutex, any thread, directly or
//   63  *          indirectly, holding the mutex gains the same priority of the
//   64  *          waiting thread (if their priority was not already equal or higher).
//   65  *          The mechanism works with any number of nested mutexes and any
//   66  *          number of involved threads. The algorithm complexity (worst case)
//   67  *          is N with N equal to the number of nested mutexes.
//   68  * @pre     In order to use the mutex APIs the @p CH_USE_MUTEXES option
//   69  *          must be enabled in @p chconf.h.
//   70  * @post    Enabling mutexes requires 5-12 (depending on the architecture)
//   71  *          extra bytes in the @p Thread structure.
//   72  * @{
//   73  */
//   74 
//   75 #include "ch.h"

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function prio_insert
          CFI NoCalls
        THUMB
// static __interwork __softfp void prio_insert(struct Thread *, ThreadsQueue *)
prio_insert:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
        MOVS     R2,R1
??prio_insert_0:
        LDR      R2,[R2, #+0]
        CMP      R2,R1
        BEQ.N    ??prio_insert_1
        LDR      R3,[R2, #+8]
        LDR      R4,[R0, #+8]
        CMP      R3,R4
        BCS.N    ??prio_insert_0
??prio_insert_1:
        STR      R2,[R0, #+0]
        LDR      R3,[R2, #+4]
        STR      R3,[R0, #+4]
        STR      R0,[R2, #+4]
        LDR      R3,[R0, #+4]
        STR      R0,[R3, #+0]
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock0

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
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
          CFI EndBlock cfiBlock1

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
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
          CFI EndBlock cfiBlock2
//   76 
//   77 #if CH_USE_MUTEXES || defined(__DOXYGEN__)
//   78 
//   79 /**
//   80  * @brief   Initializes s @p Mutex structure.
//   81  *
//   82  * @param[out] mp       pointer to a @p Mutex structure
//   83  *
//   84  * @init
//   85  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function chMtxInit
        THUMB
//   86 void chMtxInit(Mutex *mp) {
chMtxInit:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   87 
//   88   chDbgCheck(mp != NULL, "chMtxInit");
        CMP      R4,#+0
        BNE.N    ??chMtxInit_0
        MOVS     R2,#+88
        LDR.N    R1,??DataTable5
        LDR.N    R0,??DataTable5_1
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//   89 
//   90   queue_init(&mp->m_queue);
??chMtxInit_0:
        STR      R4,[R4, #+4]
        STR      R4,[R4, #+0]
//   91   mp->m_owner = NULL;
        MOVS     R0,#+0
        STR      R0,[R4, #+8]
//   92 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//   93 
//   94 /**
//   95  * @brief   Locks the specified mutex.
//   96  * @post    The mutex is locked and inserted in the per-thread stack of owned
//   97  *          mutexes.
//   98  *
//   99  * @param[in] mp        pointer to the @p Mutex structure
//  100  *
//  101  * @api
//  102  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function chMtxLock
        THUMB
//  103 void chMtxLock(Mutex *mp) {
chMtxLock:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  104 
//  105   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  106 
//  107   chMtxLockS(mp);
        MOVS     R0,R4
          CFI FunCall chMtxLockS
        BL       chMtxLockS
//  108 
//  109   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  110 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock4
//  111 
//  112 /**
//  113  * @brief   Locks the specified mutex.
//  114  * @post    The mutex is locked and inserted in the per-thread stack of owned
//  115  *          mutexes.
//  116  *
//  117  * @param[in] mp        pointer to the @p Mutex structure
//  118  *
//  119  * @sclass
//  120  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function chMtxLockS
        THUMB
//  121 void chMtxLockS(Mutex *mp) {
chMtxLockS:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  122   Thread *ctp = currp;
        LDR.N    R0,??DataTable5_2
        LDR      R5,[R0, #+24]
//  123 
//  124   chDbgCheckClassS();
          CFI FunCall chDbgCheckClassS
        BL       chDbgCheckClassS
//  125   chDbgCheck(mp != NULL, "chMtxLockS");
        CMP      R4,#+0
        BNE.N    ??chMtxLockS_0
        MOVS     R2,#+125
        LDR.N    R1,??DataTable5
        LDR.N    R0,??DataTable5_3
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  126 
//  127   /* Is the mutex already locked? */
//  128   if (mp->m_owner != NULL) {
??chMtxLockS_0:
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BEQ.N    ??chMtxLockS_1
//  129     /* Priority inheritance protocol; explores the thread-mutex dependencies
//  130        boosting the priority of all the affected threads to equal the priority
//  131        of the running thread requesting the mutex.*/
//  132     Thread *tp = mp->m_owner;
        LDR      R6,[R4, #+8]
//  133     /* Does the running thread have higher priority than the mutex
//  134        owning thread? */
//  135     while (tp->p_prio < ctp->p_prio) {
??chMtxLockS_2:
        LDR      R0,[R6, #+8]
        LDR      R1,[R5, #+8]
        CMP      R0,R1
        BCS.N    ??chMtxLockS_3
//  136       /* Make priority of thread tp match the running thread's priority.*/
//  137       tp->p_prio = ctp->p_prio;
        LDR      R0,[R5, #+8]
        STR      R0,[R6, #+8]
//  138       /* The following states need priority queues reordering.*/
//  139       switch (tp->p_state) {
        LDRB     R0,[R6, #+32]
        CMP      R0,#+0
        BEQ.N    ??chMtxLockS_4
        CMP      R0,#+4
        BNE.N    ??chMtxLockS_5
//  140       case THD_STATE_WTMTX:
//  141         /* Re-enqueues the mutex owner with its new priority.*/
//  142         prio_insert(dequeue(tp), (ThreadsQueue *)tp->p_u.wtobjp);
??chMtxLockS_6:
        MOVS     R0,R6
          CFI FunCall dequeue
        BL       dequeue
        LDR      R1,[R6, #+40]
          CFI FunCall prio_insert
        BL       prio_insert
//  143         tp = ((Mutex *)tp->p_u.wtobjp)->m_owner;
        LDR      R0,[R6, #+40]
        LDR      R6,[R0, #+8]
//  144         continue;
        B.N      ??chMtxLockS_2
//  145 #if CH_USE_CONDVARS ||                                                      \ 
//  146     (CH_USE_SEMAPHORES && CH_USE_SEMAPHORES_PRIORITY) ||                    \ 
//  147     (CH_USE_MESSAGES && CH_USE_MESSAGES_PRIORITY)
//  148 #if CH_USE_CONDVARS
//  149       case THD_STATE_WTCOND:
//  150 #endif
//  151 #if CH_USE_SEMAPHORES && CH_USE_SEMAPHORES_PRIORITY
//  152       case THD_STATE_WTSEM:
//  153 #endif
//  154 #if CH_USE_MESSAGES && CH_USE_MESSAGES_PRIORITY
//  155       case THD_STATE_SNDMSGQ:
//  156 #endif
//  157         /* Re-enqueues tp with its new priority on the queue.*/
//  158         prio_insert(dequeue(tp), (ThreadsQueue *)tp->p_u.wtobjp);
//  159         break;
//  160 #endif
//  161       case THD_STATE_READY:
//  162 #if CH_DBG_ENABLE_ASSERTS
//  163         /* Prevents an assertion in chSchReadyI().*/
//  164         tp->p_state = THD_STATE_CURRENT;
??chMtxLockS_4:
        MOVS     R0,#+1
        STRB     R0,[R6, #+32]
//  165 #endif
//  166         /* Re-enqueues tp with its new priority on the ready list.*/
//  167         chSchReadyI(dequeue(tp));
        MOVS     R0,R6
          CFI FunCall dequeue
        BL       dequeue
          CFI FunCall chSchReadyI
        BL       chSchReadyI
//  168         break;
//  169       }
//  170       break;
//  171     }
//  172     /* Sleep on the mutex.*/
//  173     prio_insert(ctp, &mp->m_queue);
??chMtxLockS_5:
??chMtxLockS_3:
        MOVS     R1,R4
        MOVS     R0,R5
          CFI FunCall prio_insert
        BL       prio_insert
//  174     ctp->p_u.wtobjp = mp;
        STR      R4,[R5, #+40]
//  175     chSchGoSleepS(THD_STATE_WTMTX);
        MOVS     R0,#+4
          CFI FunCall chSchGoSleepS
        BL       chSchGoSleepS
//  176     /* It is assumed that the thread performing the unlock operation assigns
//  177        the mutex to this thread.*/
//  178     chDbgAssert(mp->m_owner == ctp, "chMtxLockS(), #1", "not owner");
        LDR      R0,[R4, #+8]
        CMP      R0,R5
        BEQ.N    ??chMtxLockS_7
        LDR.N    R0,??DataTable5_4
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  179     chDbgAssert(ctp->p_mtxlist == mp, "chMtxLockS(), #2", "not owned");
??chMtxLockS_7:
        LDR      R0,[R5, #+60]
        CMP      R0,R4
        BEQ.N    ??chMtxLockS_8
        LDR.N    R0,??DataTable5_5
          CFI FunCall chDbgPanic
        BL       chDbgPanic
        B.N      ??chMtxLockS_8
//  180   }
//  181   else {
//  182     /* It was not owned, inserted in the owned mutexes list.*/
//  183     mp->m_owner = ctp;
??chMtxLockS_1:
        STR      R5,[R4, #+8]
//  184     mp->m_next = ctp->p_mtxlist;
        LDR      R0,[R5, #+60]
        STR      R0,[R4, #+12]
//  185     ctp->p_mtxlist = mp;
        STR      R4,[R5, #+60]
//  186   }
//  187 }
??chMtxLockS_8:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock5
//  188 
//  189 /**
//  190  * @brief   Tries to lock a mutex.
//  191  * @details This function attempts to lock a mutex, if the mutex is already
//  192  *          locked by another thread then the function exits without waiting.
//  193  * @post    The mutex is locked and inserted in the per-thread stack of owned
//  194  *          mutexes.
//  195  * @note    This function does not have any overhead related to the
//  196  *          priority inheritance mechanism because it does not try to
//  197  *          enter a sleep state.
//  198  *
//  199  * @param[in] mp        pointer to the @p Mutex structure
//  200  * @return              The operation status.
//  201  * @retval TRUE         if the mutex has been successfully acquired
//  202  * @retval FALSE        if the lock attempt failed.
//  203  *
//  204  * @api
//  205  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function chMtxTryLock
        THUMB
//  206 bool_t chMtxTryLock(Mutex *mp) {
chMtxTryLock:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  207   bool_t b;
//  208 
//  209   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  210 
//  211   b = chMtxTryLockS(mp);
        MOVS     R0,R4
          CFI FunCall chMtxTryLockS
        BL       chMtxTryLockS
        MOVS     R5,R0
//  212 
//  213   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  214   return b;
        MOVS     R0,R5
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock6
//  215 }
//  216 
//  217 /**
//  218  * @brief   Tries to lock a mutex.
//  219  * @details This function attempts to lock a mutex, if the mutex is already
//  220  *          taken by another thread then the function exits without waiting.
//  221  * @post    The mutex is locked and inserted in the per-thread stack of owned
//  222  *          mutexes.
//  223  * @note    This function does not have any overhead related to the
//  224  *          priority inheritance mechanism because it does not try to
//  225  *          enter a sleep state.
//  226  *
//  227  * @param[in] mp        pointer to the @p Mutex structure
//  228  * @return              The operation status.
//  229  * @retval TRUE         if the mutex has been successfully acquired
//  230  * @retval FALSE        if the lock attempt failed.
//  231  *
//  232  * @sclass
//  233  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function chMtxTryLockS
        THUMB
//  234 bool_t chMtxTryLockS(Mutex *mp) {
chMtxTryLockS:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  235 
//  236   chDbgCheckClassS();
          CFI FunCall chDbgCheckClassS
        BL       chDbgCheckClassS
//  237   chDbgCheck(mp != NULL, "chMtxTryLockS");
        CMP      R4,#+0
        BNE.N    ??chMtxTryLockS_0
        MOVS     R2,#+237
        LDR.N    R1,??DataTable5
        LDR.N    R0,??DataTable5_6
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  238 
//  239   if (mp->m_owner != NULL)
??chMtxTryLockS_0:
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BEQ.N    ??chMtxTryLockS_1
//  240     return FALSE;
        MOVS     R0,#+0
        B.N      ??chMtxTryLockS_2
//  241   mp->m_owner = currp;
??chMtxTryLockS_1:
        LDR.N    R0,??DataTable5_2
        LDR      R0,[R0, #+24]
        STR      R0,[R4, #+8]
//  242   mp->m_next = currp->p_mtxlist;
        LDR.N    R0,??DataTable5_2
        LDR      R0,[R0, #+24]
        LDR      R0,[R0, #+60]
        STR      R0,[R4, #+12]
//  243   currp->p_mtxlist = mp;
        LDR.N    R0,??DataTable5_2
        LDR      R0,[R0, #+24]
        STR      R4,[R0, #+60]
//  244   return TRUE;
        MOVS     R0,#+1
??chMtxTryLockS_2:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock7
//  245 }
//  246 
//  247 /**
//  248  * @brief   Unlocks the next owned mutex in reverse lock order.
//  249  * @pre     The invoking thread <b>must</b> have at least one owned mutex.
//  250  * @post    The mutex is unlocked and removed from the per-thread stack of
//  251  *          owned mutexes.
//  252  *
//  253  * @return              A pointer to the unlocked mutex.
//  254  *
//  255  * @api
//  256  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function chMtxUnlock
        THUMB
//  257 Mutex *chMtxUnlock(void) {
chMtxUnlock:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
//  258   Thread *ctp = currp;
        LDR.N    R0,??DataTable5_2
        LDR      R4,[R0, #+24]
//  259   Mutex *ump, *mp;
//  260 
//  261   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  262   chDbgAssert(ctp->p_mtxlist != NULL,
//  263               "chMtxUnlock(), #1",
//  264               "owned mutexes list empty");
        LDR      R0,[R4, #+60]
        CMP      R0,#+0
        BNE.N    ??chMtxUnlock_0
        LDR.N    R0,??DataTable5_7
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  265   chDbgAssert(ctp->p_mtxlist->m_owner == ctp,
//  266               "chMtxUnlock(), #2",
//  267               "ownership failure");
??chMtxUnlock_0:
        LDR      R0,[R4, #+60]
        LDR      R0,[R0, #+8]
        CMP      R0,R4
        BEQ.N    ??chMtxUnlock_1
        LDR.N    R0,??DataTable5_8
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  268   /* Removes the top Mutex from the Thread's owned mutexes list and marks it
//  269      as not owned.*/
//  270   ump = ctp->p_mtxlist;
??chMtxUnlock_1:
        LDR      R0,[R4, #+60]
        MOVS     R5,R0
//  271   ctp->p_mtxlist = ump->m_next;
        LDR      R0,[R5, #+12]
        STR      R0,[R4, #+60]
//  272   /* If a thread is waiting on the mutex then the fun part begins.*/
//  273   if (chMtxQueueNotEmptyS(ump)) {
        LDR      R0,[R5, #+0]
        CMP      R0,R5
        BEQ.N    ??chMtxUnlock_2
//  274     Thread *tp;
//  275 
//  276     /* Recalculates the optimal thread priority by scanning the owned
//  277        mutexes list.*/
//  278     tprio_t newprio = ctp->p_realprio;
        LDR      R8,[R4, #+64]
//  279     mp = ctp->p_mtxlist;
        LDR      R0,[R4, #+60]
        MOVS     R6,R0
//  280     while (mp != NULL) {
??chMtxUnlock_3:
        CMP      R6,#+0
        BEQ.N    ??chMtxUnlock_4
//  281       /* If the highest priority thread waiting in the mutexes list has a
//  282          greater priority than the current thread base priority then the final
//  283          priority will have at least that priority.*/
//  284       if (chMtxQueueNotEmptyS(mp) && (mp->m_queue.p_next->p_prio > newprio))
        LDR      R0,[R6, #+0]
        CMP      R0,R6
        BEQ.N    ??chMtxUnlock_5
        LDR      R0,[R6, #+0]
        LDR      R0,[R0, #+8]
        CMP      R8,R0
        BCS.N    ??chMtxUnlock_5
//  285         newprio = mp->m_queue.p_next->p_prio;
        LDR      R0,[R6, #+0]
        LDR      R0,[R0, #+8]
        MOV      R8,R0
//  286       mp = mp->m_next;
??chMtxUnlock_5:
        LDR      R6,[R6, #+12]
        B.N      ??chMtxUnlock_3
//  287     }
//  288     /* Assigns to the current thread the highest priority among all the
//  289        waiting threads.*/
//  290     ctp->p_prio = newprio;
??chMtxUnlock_4:
        STR      R8,[R4, #+8]
//  291     /* Awakens the highest priority thread waiting for the unlocked mutex and
//  292        assigns the mutex to it.*/
//  293     tp = fifo_remove(&ump->m_queue);
        MOVS     R0,R5
          CFI FunCall fifo_remove
        BL       fifo_remove
        MOVS     R7,R0
//  294     ump->m_owner = tp;
        STR      R7,[R5, #+8]
//  295     ump->m_next = tp->p_mtxlist;
        LDR      R0,[R7, #+60]
        STR      R0,[R5, #+12]
//  296     tp->p_mtxlist = ump;
        STR      R5,[R7, #+60]
//  297     chSchWakeupS(tp, RDY_OK);
        MOVS     R1,#+0
        MOVS     R0,R7
          CFI FunCall chSchWakeupS
        BL       chSchWakeupS
        B.N      ??chMtxUnlock_6
//  298   }
//  299   else
//  300     ump->m_owner = NULL;
??chMtxUnlock_2:
        MOVS     R0,#+0
        STR      R0,[R5, #+8]
//  301   chSysUnlock();
??chMtxUnlock_6:
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  302   return ump;
        MOVS     R0,R5
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock8
//  303 }
//  304 
//  305 /**
//  306  * @brief   Unlocks the next owned mutex in reverse lock order.
//  307  * @pre     The invoking thread <b>must</b> have at least one owned mutex.
//  308  * @post    The mutex is unlocked and removed from the per-thread stack of
//  309  *          owned mutexes.
//  310  * @post    This function does not reschedule so a call to a rescheduling
//  311  *          function must be performed before unlocking the kernel.
//  312  *
//  313  * @return              A pointer to the unlocked mutex.
//  314  *
//  315  * @sclass
//  316  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function chMtxUnlockS
        THUMB
//  317 Mutex *chMtxUnlockS(void) {
chMtxUnlockS:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
//  318   Thread *ctp = currp;
        LDR.N    R0,??DataTable5_2
        LDR      R4,[R0, #+24]
//  319   Mutex *ump, *mp;
//  320 
//  321   chDbgCheckClassS();
          CFI FunCall chDbgCheckClassS
        BL       chDbgCheckClassS
//  322   chDbgAssert(ctp->p_mtxlist != NULL,
//  323               "chMtxUnlockS(), #1",
//  324               "owned mutexes list empty");
        LDR      R0,[R4, #+60]
        CMP      R0,#+0
        BNE.N    ??chMtxUnlockS_0
        LDR.N    R0,??DataTable5_9
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  325   chDbgAssert(ctp->p_mtxlist->m_owner == ctp,
//  326               "chMtxUnlockS(), #2",
//  327               "ownership failure");
??chMtxUnlockS_0:
        LDR      R0,[R4, #+60]
        LDR      R0,[R0, #+8]
        CMP      R0,R4
        BEQ.N    ??chMtxUnlockS_1
        LDR.N    R0,??DataTable5_10
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  328 
//  329   /* Removes the top Mutex from the owned mutexes list and marks it as not
//  330      owned.*/
//  331   ump = ctp->p_mtxlist;
??chMtxUnlockS_1:
        LDR      R0,[R4, #+60]
        MOVS     R5,R0
//  332   ctp->p_mtxlist = ump->m_next;
        LDR      R0,[R5, #+12]
        STR      R0,[R4, #+60]
//  333   /* If a thread is waiting on the mutex then the fun part begins.*/
//  334   if (chMtxQueueNotEmptyS(ump)) {
        LDR      R0,[R5, #+0]
        CMP      R0,R5
        BEQ.N    ??chMtxUnlockS_2
//  335     Thread *tp;
//  336 
//  337     /* Recalculates the optimal thread priority by scanning the owned
//  338        mutexes list.*/
//  339     tprio_t newprio = ctp->p_realprio;
        LDR      R8,[R4, #+64]
//  340     mp = ctp->p_mtxlist;
        LDR      R0,[R4, #+60]
        MOVS     R6,R0
//  341     while (mp != NULL) {
??chMtxUnlockS_3:
        CMP      R6,#+0
        BEQ.N    ??chMtxUnlockS_4
//  342       /* If the highest priority thread waiting in the mutexes list has a
//  343          greater priority than the current thread base priority then the final
//  344          priority will have at least that priority.*/
//  345       if (chMtxQueueNotEmptyS(mp) && (mp->m_queue.p_next->p_prio > newprio))
        LDR      R0,[R6, #+0]
        CMP      R0,R6
        BEQ.N    ??chMtxUnlockS_5
        LDR      R0,[R6, #+0]
        LDR      R0,[R0, #+8]
        CMP      R8,R0
        BCS.N    ??chMtxUnlockS_5
//  346         newprio = mp->m_queue.p_next->p_prio;
        LDR      R0,[R6, #+0]
        LDR      R0,[R0, #+8]
        MOV      R8,R0
//  347       mp = mp->m_next;
??chMtxUnlockS_5:
        LDR      R6,[R6, #+12]
        B.N      ??chMtxUnlockS_3
//  348     }
//  349     ctp->p_prio = newprio;
??chMtxUnlockS_4:
        STR      R8,[R4, #+8]
//  350     /* Awakens the highest priority thread waiting for the unlocked mutex and
//  351        assigns the mutex to it.*/
//  352     tp = fifo_remove(&ump->m_queue);
        MOVS     R0,R5
          CFI FunCall fifo_remove
        BL       fifo_remove
        MOVS     R7,R0
//  353     ump->m_owner = tp;
        STR      R7,[R5, #+8]
//  354     ump->m_next = tp->p_mtxlist;
        LDR      R0,[R7, #+60]
        STR      R0,[R5, #+12]
//  355     tp->p_mtxlist = ump;
        STR      R5,[R7, #+60]
//  356     chSchReadyI(tp);
        MOVS     R0,R7
          CFI FunCall chSchReadyI
        BL       chSchReadyI
        B.N      ??chMtxUnlockS_6
//  357   }
//  358   else
//  359     ump->m_owner = NULL;
??chMtxUnlockS_2:
        MOVS     R0,#+0
        STR      R0,[R5, #+8]
//  360   return ump;
??chMtxUnlockS_6:
        MOVS     R0,R5
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock9
//  361 }
//  362 
//  363 /**
//  364  * @brief   Unlocks all the mutexes owned by the invoking thread.
//  365  * @post    The stack of owned mutexes is emptied and all the found
//  366  *          mutexes are unlocked.
//  367  * @note    This function is <b>MUCH MORE</b> efficient than releasing the
//  368  *          mutexes one by one and not just because the call overhead,
//  369  *          this function does not have any overhead related to the priority
//  370  *          inheritance mechanism.
//  371  *
//  372  * @api
//  373  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function chMtxUnlockAll
        THUMB
//  374 void chMtxUnlockAll(void) {
chMtxUnlockAll:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
//  375   Thread *ctp = currp;
        LDR.N    R0,??DataTable5_2
        LDR      R4,[R0, #+24]
//  376 
//  377   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  378   if (ctp->p_mtxlist != NULL) {
        LDR      R0,[R4, #+60]
        CMP      R0,#+0
        BEQ.N    ??chMtxUnlockAll_0
//  379     do {
//  380       Mutex *ump = ctp->p_mtxlist;
??chMtxUnlockAll_1:
        LDR      R5,[R4, #+60]
//  381       ctp->p_mtxlist = ump->m_next;
        LDR      R0,[R5, #+12]
        STR      R0,[R4, #+60]
//  382       if (chMtxQueueNotEmptyS(ump)) {
        LDR      R0,[R5, #+0]
        CMP      R0,R5
        BEQ.N    ??chMtxUnlockAll_2
//  383         Thread *tp = fifo_remove(&ump->m_queue);
        MOVS     R0,R5
          CFI FunCall fifo_remove
        BL       fifo_remove
        MOVS     R6,R0
//  384         ump->m_owner = tp;
        STR      R6,[R5, #+8]
//  385         ump->m_next = tp->p_mtxlist;
        LDR      R0,[R6, #+60]
        STR      R0,[R5, #+12]
//  386         tp->p_mtxlist = ump;
        STR      R5,[R6, #+60]
//  387         chSchReadyI(tp);
        MOVS     R0,R6
          CFI FunCall chSchReadyI
        BL       chSchReadyI
        B.N      ??chMtxUnlockAll_3
//  388       }
//  389       else
//  390         ump->m_owner = NULL;
??chMtxUnlockAll_2:
        MOVS     R0,#+0
        STR      R0,[R5, #+8]
//  391     } while (ctp->p_mtxlist != NULL);
??chMtxUnlockAll_3:
        LDR      R0,[R4, #+60]
        CMP      R0,#+0
        BNE.N    ??chMtxUnlockAll_1
//  392     ctp->p_prio = ctp->p_realprio;
        LDR      R0,[R4, #+64]
        STR      R0,[R4, #+8]
//  393     chSchRescheduleS();
          CFI FunCall chSchRescheduleS
        BL       chSchRescheduleS
//  394   }
//  395   chSysUnlock();
??chMtxUnlockAll_0:
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  396 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock10

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_1:
        DC32     `?<Constant "\\"chMtxInit\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_2:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_3:
        DC32     `?<Constant "\\"chMtxLockS\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_4:
        DC32     `?<Constant "chMtxLockS(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_5:
        DC32     `?<Constant "chMtxLockS(), #2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_6:
        DC32     `?<Constant "\\"chMtxTryLockS\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_7:
        DC32     `?<Constant "chMtxUnlock(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_8:
        DC32     `?<Constant "chMtxUnlock(), #2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_9:
        DC32     `?<Constant "chMtxUnlockS(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_10:
        DC32     `?<Constant "chMtxUnlockS(), #2">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chMtxInit\\"()">`:
        DATA
        DC8 "\"chMtxInit\"()"
        DC8 0, 0

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
        DC8 72H, 63H, 5CH, 63H, 68H, 6DH, 74H, 78H
        DC8 2EH, 63H, 0
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chMtxLockS\\"()">`:
        DATA
        DC8 "\"chMtxLockS\"()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "chMtxLockS(), #1">`:
        DATA
        DC8 "chMtxLockS(), #1"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "chMtxLockS(), #2">`:
        DATA
        DC8 "chMtxLockS(), #2"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chMtxTryLockS\\"()">`:
        DATA
        DC8 "\"chMtxTryLockS\"()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "chMtxUnlock(), #1">`:
        DATA
        DC8 "chMtxUnlock(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "chMtxUnlock(), #2">`:
        DATA
        DC8 "chMtxUnlock(), #2"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "chMtxUnlockS(), #1">`:
        DATA
        DC8 "chMtxUnlockS(), #1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "chMtxUnlockS(), #2">`:
        DATA
        DC8 "chMtxUnlockS(), #2"
        DC8 0

        END
//  397 
//  398 #endif /* CH_USE_MUTEXES */
//  399 
//  400 /** @} */
// 
// 240 bytes in section .rodata
// 760 bytes in section .text
// 
// 760 bytes of CODE  memory
// 240 bytes of CONST memory
//
//Errors: none
//Warnings: none
