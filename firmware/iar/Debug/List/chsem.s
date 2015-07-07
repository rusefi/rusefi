///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:43 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chsem.c                                       /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chsem.c -lCN F:\stuff\rusefi_sourceforge\firm /
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
//                       chsem.s                                              /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME chsem

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN chDbgCheckClassI
        EXTERN chDbgCheckClassS
        EXTERN chDbgPanic
        EXTERN chDbgPanic3
        EXTERN chSchGoSleepS
        EXTERN chSchGoSleepTimeoutS
        EXTERN chSchReadyI
        EXTERN chSchRescheduleS
        EXTERN chSchWakeupS
        EXTERN dbg_check_lock
        EXTERN dbg_check_unlock
        EXTERN rlist

        PUBLIC chSemAddCounterI
        PUBLIC chSemInit
        PUBLIC chSemReset
        PUBLIC chSemResetI
        PUBLIC chSemSignal
        PUBLIC chSemSignalI
        PUBLIC chSemWait
        PUBLIC chSemWaitS
        PUBLIC chSemWaitTimeout
        PUBLIC chSemWaitTimeoutS
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\kernel\src\chsem.c
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
//   29  * @file    chsem.c
//   30  * @brief   Semaphores code.
//   31  *
//   32  * @addtogroup semaphores
//   33  * @details Semaphores related APIs and services.
//   34  *
//   35  *          <h2>Operation mode</h2>
//   36  *          Semaphores are a flexible synchronization primitive, ChibiOS/RT
//   37  *          implements semaphores in their "counting semaphores" variant as
//   38  *          defined by Edsger Dijkstra plus several enhancements like:
//   39  *          - Wait operation with timeout.
//   40  *          - Reset operation.
//   41  *          - Atomic wait+signal operation.
//   42  *          - Return message from the wait operation (OK, RESET, TIMEOUT).
//   43  *          .
//   44  *          The binary semaphores variant can be easily implemented using
//   45  *          counting semaphores.<br>
//   46  *          Operations defined for semaphores:
//   47  *          - <b>Signal</b>: The semaphore counter is increased and if the
//   48  *            result is non-positive then a waiting thread is removed from
//   49  *            the semaphore queue and made ready for execution.
//   50  *          - <b>Wait</b>: The semaphore counter is decreased and if the result
//   51  *            becomes negative the thread is queued in the semaphore and
//   52  *            suspended.
//   53  *          - <b>Reset</b>: The semaphore counter is reset to a non-negative
//   54  *            value and all the threads in the queue are released.
//   55  *          .
//   56  *          Semaphores can be used as guards for mutual exclusion zones
//   57  *          (note that mutexes are recommended for this kind of use) but
//   58  *          also have other uses, queues guards and counters for example.<br>
//   59  *          Semaphores usually use a FIFO queuing strategy but it is possible
//   60  *          to make them order threads by priority by enabling
//   61  *          @p CH_USE_SEMAPHORES_PRIORITY in @p chconf.h.
//   62  * @pre     In order to use the semaphore APIs the @p CH_USE_SEMAPHORES
//   63  *          option must be enabled in @p chconf.h.
//   64  * @{
//   65  */
//   66 
//   67 #include "ch.h"

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function queue_insert
          CFI NoCalls
        THUMB
// static __interwork __softfp void queue_insert(struct Thread *, ThreadsQueue *)
queue_insert:
        STR      R1,[R0, #+0]
        LDR      R2,[R1, #+4]
        STR      R2,[R0, #+4]
        STR      R0,[R1, #+4]
        LDR      R2,[R0, #+4]
        STR      R0,[R2, #+0]
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
          CFI Function lifo_remove
          CFI NoCalls
        THUMB
// static __interwork __softfp struct Thread *lifo_remove(ThreadsQueue *)
lifo_remove:
        MOVS     R1,R0
        LDR      R0,[R1, #+4]
        LDR      R2,[R0, #+4]
        STR      R2,[R1, #+4]
        STR      R1,[R2, #+0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock2
//   68 
//   69 #if CH_USE_SEMAPHORES || defined(__DOXYGEN__)
//   70 
//   71 #if CH_USE_SEMAPHORES_PRIORITY
//   72 #define sem_insert(tp, qp) prio_insert(tp, qp)
//   73 #else
//   74 #define sem_insert(tp, qp) queue_insert(tp, qp)
//   75 #endif
//   76 
//   77 /**
//   78  * @brief   Initializes a semaphore with the specified counter value.
//   79  *
//   80  * @param[out] sp       pointer to a @p Semaphore structure
//   81  * @param[in] n         initial value of the semaphore counter. Must be
//   82  *                      non-negative.
//   83  *
//   84  * @init
//   85  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function chSemInit
        THUMB
//   86 void chSemInit(Semaphore *sp, cnt_t n) {
chSemInit:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   87 
//   88   chDbgCheck((sp != NULL) && (n >= 0), "chSemInit");
        CMP      R4,#+0
        BEQ.N    ??chSemInit_0
        CMP      R5,#+0
        BPL.N    ??chSemInit_1
??chSemInit_0:
        MOVS     R2,#+88
        LDR.N    R1,??DataTable6
        LDR.N    R0,??DataTable6_1
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//   89 
//   90   queue_init(&sp->s_queue);
??chSemInit_1:
        STR      R4,[R4, #+4]
        STR      R4,[R4, #+0]
//   91   sp->s_cnt = n;
        STR      R5,[R4, #+8]
//   92 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock3
//   93 
//   94 /**
//   95  * @brief   Performs a reset operation on the semaphore.
//   96  * @post    After invoking this function all the threads waiting on the
//   97  *          semaphore, if any, are released and the semaphore counter is set
//   98  *          to the specified, non negative, value.
//   99  * @note    The released threads can recognize they were waked up by a reset
//  100  *          rather than a signal because the @p chSemWait() will return
//  101  *          @p RDY_RESET instead of @p RDY_OK.
//  102  *
//  103  * @param[in] sp        pointer to a @p Semaphore structure
//  104  * @param[in] n         the new value of the semaphore counter. The value must
//  105  *                      be non-negative.
//  106  *
//  107  * @api
//  108  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function chSemReset
        THUMB
//  109 void chSemReset(Semaphore *sp, cnt_t n) {
chSemReset:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  110 
//  111   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  112   chSemResetI(sp, n);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall chSemResetI
        BL       chSemResetI
//  113   chSchRescheduleS();
          CFI FunCall chSchRescheduleS
        BL       chSchRescheduleS
//  114   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  115 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock4
//  116 
//  117 /**
//  118  * @brief   Performs a reset operation on the semaphore.
//  119  * @post    After invoking this function all the threads waiting on the
//  120  *          semaphore, if any, are released and the semaphore counter is set
//  121  *          to the specified, non negative, value.
//  122  * @post    This function does not reschedule so a call to a rescheduling
//  123  *          function must be performed before unlocking the kernel. Note that
//  124  *          interrupt handlers always reschedule on exit so an explicit
//  125  *          reschedule must not be performed in ISRs.
//  126  * @note    The released threads can recognize they were waked up by a reset
//  127  *          rather than a signal because the @p chSemWait() will return
//  128  *          @p RDY_RESET instead of @p RDY_OK.
//  129  *
//  130  * @param[in] sp        pointer to a @p Semaphore structure
//  131  * @param[in] n         the new value of the semaphore counter. The value must
//  132  *                      be non-negative.
//  133  *
//  134  * @iclass
//  135  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function chSemResetI
        THUMB
//  136 void chSemResetI(Semaphore *sp, cnt_t n) {
chSemResetI:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  137   cnt_t cnt;
//  138 
//  139   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  140   chDbgCheck((sp != NULL) && (n >= 0), "chSemResetI");
        CMP      R4,#+0
        BEQ.N    ??chSemResetI_0
        CMP      R5,#+0
        BPL.N    ??chSemResetI_1
??chSemResetI_0:
        MOVS     R2,#+140
        LDR.N    R1,??DataTable6
        LDR.N    R0,??DataTable6_2
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  141   chDbgAssert(((sp->s_cnt >= 0) && isempty(&sp->s_queue)) ||
//  142               ((sp->s_cnt < 0) && notempty(&sp->s_queue)),
//  143               "chSemResetI(), #1",
//  144               "inconsistent semaphore");
??chSemResetI_1:
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BMI.N    ??chSemResetI_2
        LDR      R0,[R4, #+0]
        CMP      R0,R4
        BEQ.N    ??chSemResetI_3
??chSemResetI_2:
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BPL.N    ??chSemResetI_4
        LDR      R0,[R4, #+0]
        CMP      R0,R4
        BNE.N    ??chSemResetI_3
??chSemResetI_4:
        LDR.N    R0,??DataTable6_3
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  145 
//  146   cnt = sp->s_cnt;
??chSemResetI_3:
        LDR      R0,[R4, #+8]
        MOVS     R6,R0
//  147   sp->s_cnt = n;
        STR      R5,[R4, #+8]
//  148   while (++cnt <= 0)
??chSemResetI_5:
        ADDS     R6,R6,#+1
        CMP      R6,#+1
        BGE.N    ??chSemResetI_6
//  149     chSchReadyI(lifo_remove(&sp->s_queue))->p_u.rdymsg = RDY_RESET;
        MOVS     R0,R4
          CFI FunCall lifo_remove
        BL       lifo_remove
          CFI FunCall chSchReadyI
        BL       chSchReadyI
        MVNS     R1,#+1
        STR      R1,[R0, #+40]
        B.N      ??chSemResetI_5
//  150 }
??chSemResetI_6:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock5
//  151 
//  152 /**
//  153  * @brief   Performs a wait operation on a semaphore.
//  154  *
//  155  * @param[in] sp        pointer to a @p Semaphore structure
//  156  * @return              A message specifying how the invoking thread has been
//  157  *                      released from the semaphore.
//  158  * @retval RDY_OK       if the thread has not stopped on the semaphore or the
//  159  *                      semaphore has been signaled.
//  160  * @retval RDY_RESET    if the semaphore has been reset using @p chSemReset().
//  161  *
//  162  * @api
//  163  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function chSemWait
        THUMB
//  164 msg_t chSemWait(Semaphore *sp) {
chSemWait:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  165   msg_t msg;
//  166 
//  167   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  168   msg = chSemWaitS(sp);
        MOVS     R0,R4
          CFI FunCall chSemWaitS
        BL       chSemWaitS
        MOVS     R5,R0
//  169   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  170   return msg;
        MOVS     R0,R5
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock6
//  171 }
//  172 
//  173 /**
//  174  * @brief   Performs a wait operation on a semaphore.
//  175  *
//  176  * @param[in] sp        pointer to a @p Semaphore structure
//  177  * @return              A message specifying how the invoking thread has been
//  178  *                      released from the semaphore.
//  179  * @retval RDY_OK       if the thread has not stopped on the semaphore or the
//  180  *                      semaphore has been signaled.
//  181  * @retval RDY_RESET    if the semaphore has been reset using @p chSemReset().
//  182  *
//  183  * @sclass
//  184  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function chSemWaitS
        THUMB
//  185 msg_t chSemWaitS(Semaphore *sp) {
chSemWaitS:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  186 
//  187   chDbgCheckClassS();
          CFI FunCall chDbgCheckClassS
        BL       chDbgCheckClassS
//  188   chDbgCheck(sp != NULL, "chSemWaitS");
        CMP      R4,#+0
        BNE.N    ??chSemWaitS_0
        MOVS     R2,#+188
        LDR.N    R1,??DataTable6
        LDR.N    R0,??DataTable6_4
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  189   chDbgAssert(((sp->s_cnt >= 0) && isempty(&sp->s_queue)) ||
//  190               ((sp->s_cnt < 0) && notempty(&sp->s_queue)),
//  191               "chSemWaitS(), #1",
//  192               "inconsistent semaphore");
??chSemWaitS_0:
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BMI.N    ??chSemWaitS_1
        LDR      R0,[R4, #+0]
        CMP      R0,R4
        BEQ.N    ??chSemWaitS_2
??chSemWaitS_1:
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BPL.N    ??chSemWaitS_3
        LDR      R0,[R4, #+0]
        CMP      R0,R4
        BNE.N    ??chSemWaitS_2
??chSemWaitS_3:
        LDR.N    R0,??DataTable6_5
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  193 
//  194   if (--sp->s_cnt < 0) {
??chSemWaitS_2:
        LDR      R0,[R4, #+8]
        SUBS     R0,R0,#+1
        STR      R0,[R4, #+8]
        CMP      R0,#+0
        BPL.N    ??chSemWaitS_4
//  195     currp->p_u.wtobjp = sp;
        LDR.N    R0,??DataTable6_6
        LDR      R0,[R0, #+24]
        STR      R4,[R0, #+40]
//  196     sem_insert(currp, &sp->s_queue);
        MOVS     R1,R4
        LDR.N    R0,??DataTable6_6
        LDR      R0,[R0, #+24]
          CFI FunCall queue_insert
        BL       queue_insert
//  197     chSchGoSleepS(THD_STATE_WTSEM);
        MOVS     R0,#+3
          CFI FunCall chSchGoSleepS
        BL       chSchGoSleepS
//  198     return currp->p_u.rdymsg;
        LDR.N    R0,??DataTable6_6
        LDR      R0,[R0, #+24]
        LDR      R0,[R0, #+40]
        B.N      ??chSemWaitS_5
//  199   }
//  200   return RDY_OK;
??chSemWaitS_4:
        MOVS     R0,#+0
??chSemWaitS_5:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock7
//  201 }
//  202 
//  203 /**
//  204  * @brief   Performs a wait operation on a semaphore with timeout specification.
//  205  *
//  206  * @param[in] sp        pointer to a @p Semaphore structure
//  207  * @param[in] time      the number of ticks before the operation timeouts,
//  208  *                      the following special values are allowed:
//  209  *                      - @a TIME_IMMEDIATE immediate timeout.
//  210  *                      - @a TIME_INFINITE no timeout.
//  211  *                      .
//  212  * @return              A message specifying how the invoking thread has been
//  213  *                      released from the semaphore.
//  214  * @retval RDY_OK       if the thread has not stopped on the semaphore or the
//  215  *                      semaphore has been signaled.
//  216  * @retval RDY_RESET    if the semaphore has been reset using @p chSemReset().
//  217  * @retval RDY_TIMEOUT  if the semaphore has not been signaled or reset within
//  218  *                      the specified timeout.
//  219  *
//  220  * @api
//  221  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function chSemWaitTimeout
        THUMB
//  222 msg_t chSemWaitTimeout(Semaphore *sp, systime_t time) {
chSemWaitTimeout:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  223   msg_t msg;
//  224 
//  225   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  226   msg = chSemWaitTimeoutS(sp, time);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall chSemWaitTimeoutS
        BL       chSemWaitTimeoutS
        MOVS     R6,R0
//  227   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  228   return msg;
        MOVS     R0,R6
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock8
//  229 }
//  230 
//  231 /**
//  232  * @brief   Performs a wait operation on a semaphore with timeout specification.
//  233  *
//  234  * @param[in] sp        pointer to a @p Semaphore structure
//  235  * @param[in] time      the number of ticks before the operation timeouts,
//  236  *                      the following special values are allowed:
//  237  *                      - @a TIME_IMMEDIATE immediate timeout.
//  238  *                      - @a TIME_INFINITE no timeout.
//  239  *                      .
//  240  * @return              A message specifying how the invoking thread has been
//  241  *                      released from the semaphore.
//  242  * @retval RDY_OK       if the thread has not stopped on the semaphore or the
//  243  *                      semaphore has been signaled.
//  244  * @retval RDY_RESET    if the semaphore has been reset using @p chSemReset().
//  245  * @retval RDY_TIMEOUT  if the semaphore has not been signaled or reset within
//  246  *                      the specified timeout.
//  247  *
//  248  * @sclass
//  249  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function chSemWaitTimeoutS
        THUMB
//  250 msg_t chSemWaitTimeoutS(Semaphore *sp, systime_t time) {
chSemWaitTimeoutS:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  251 
//  252   chDbgCheckClassS();
          CFI FunCall chDbgCheckClassS
        BL       chDbgCheckClassS
//  253   chDbgCheck(sp != NULL, "chSemWaitTimeoutS");
        CMP      R4,#+0
        BNE.N    ??chSemWaitTimeoutS_0
        MOVS     R2,#+253
        LDR.N    R1,??DataTable6
        LDR.N    R0,??DataTable6_7
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  254   chDbgAssert(((sp->s_cnt >= 0) && isempty(&sp->s_queue)) ||
//  255               ((sp->s_cnt < 0) && notempty(&sp->s_queue)),
//  256               "chSemWaitTimeoutS(), #1",
//  257               "inconsistent semaphore");
??chSemWaitTimeoutS_0:
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BMI.N    ??chSemWaitTimeoutS_1
        LDR      R0,[R4, #+0]
        CMP      R0,R4
        BEQ.N    ??chSemWaitTimeoutS_2
??chSemWaitTimeoutS_1:
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BPL.N    ??chSemWaitTimeoutS_3
        LDR      R0,[R4, #+0]
        CMP      R0,R4
        BNE.N    ??chSemWaitTimeoutS_2
??chSemWaitTimeoutS_3:
        LDR.N    R0,??DataTable6_8
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  258 
//  259   if (--sp->s_cnt < 0) {
??chSemWaitTimeoutS_2:
        LDR      R0,[R4, #+8]
        SUBS     R0,R0,#+1
        STR      R0,[R4, #+8]
        CMP      R0,#+0
        BPL.N    ??chSemWaitTimeoutS_4
//  260     if (TIME_IMMEDIATE == time) {
        CMP      R5,#+0
        BNE.N    ??chSemWaitTimeoutS_5
//  261       sp->s_cnt++;
        LDR      R0,[R4, #+8]
        ADDS     R0,R0,#+1
        STR      R0,[R4, #+8]
//  262       return RDY_TIMEOUT;
        MOVS     R0,#-1
        B.N      ??chSemWaitTimeoutS_6
//  263     }
//  264     currp->p_u.wtobjp = sp;
??chSemWaitTimeoutS_5:
        LDR.N    R0,??DataTable6_6
        LDR      R0,[R0, #+24]
        STR      R4,[R0, #+40]
//  265     sem_insert(currp, &sp->s_queue);
        MOVS     R1,R4
        LDR.N    R0,??DataTable6_6
        LDR      R0,[R0, #+24]
          CFI FunCall queue_insert
        BL       queue_insert
//  266     return chSchGoSleepTimeoutS(THD_STATE_WTSEM, time);
        MOVS     R1,R5
        MOVS     R0,#+3
          CFI FunCall chSchGoSleepTimeoutS
        BL       chSchGoSleepTimeoutS
        B.N      ??chSemWaitTimeoutS_6
//  267   }
//  268   return RDY_OK;
??chSemWaitTimeoutS_4:
        MOVS     R0,#+0
??chSemWaitTimeoutS_6:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock9
//  269 }
//  270 
//  271 /**
//  272  * @brief   Performs a signal operation on a semaphore.
//  273  *
//  274  * @param[in] sp        pointer to a @p Semaphore structure
//  275  *
//  276  * @api
//  277  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function chSemSignal
        THUMB
//  278 void chSemSignal(Semaphore *sp) {
chSemSignal:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  279 
//  280   chDbgCheck(sp != NULL, "chSemSignal");
        CMP      R4,#+0
        BNE.N    ??chSemSignal_0
        MOV      R2,#+280
        LDR.N    R1,??DataTable6
        LDR.N    R0,??DataTable6_9
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  281   chDbgAssert(((sp->s_cnt >= 0) && isempty(&sp->s_queue)) ||
//  282               ((sp->s_cnt < 0) && notempty(&sp->s_queue)),
//  283               "chSemSignal(), #1",
//  284               "inconsistent semaphore");
??chSemSignal_0:
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BMI.N    ??chSemSignal_1
        LDR      R0,[R4, #+0]
        CMP      R0,R4
        BEQ.N    ??chSemSignal_2
??chSemSignal_1:
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BPL.N    ??chSemSignal_3
        LDR      R0,[R4, #+0]
        CMP      R0,R4
        BNE.N    ??chSemSignal_2
??chSemSignal_3:
        LDR.N    R0,??DataTable6_10
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  285 
//  286   chSysLock();
??chSemSignal_2:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  287   if (++sp->s_cnt <= 0)
        LDR      R0,[R4, #+8]
        ADDS     R0,R0,#+1
        STR      R0,[R4, #+8]
        CMP      R0,#+1
        BGE.N    ??chSemSignal_4
//  288     chSchWakeupS(fifo_remove(&sp->s_queue), RDY_OK);
        MOVS     R0,R4
          CFI FunCall fifo_remove
        BL       fifo_remove
        MOVS     R1,#+0
          CFI FunCall chSchWakeupS
        BL       chSchWakeupS
//  289   chSysUnlock();
??chSemSignal_4:
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  290 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock10
//  291 
//  292 /**
//  293  * @brief   Performs a signal operation on a semaphore.
//  294  * @post    This function does not reschedule so a call to a rescheduling
//  295  *          function must be performed before unlocking the kernel. Note that
//  296  *          interrupt handlers always reschedule on exit so an explicit
//  297  *          reschedule must not be performed in ISRs.
//  298  *
//  299  * @param[in] sp    pointer to a @p Semaphore structure
//  300  *
//  301  * @iclass
//  302  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function chSemSignalI
        THUMB
//  303 void chSemSignalI(Semaphore *sp) {
chSemSignalI:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  304 
//  305   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  306   chDbgCheck(sp != NULL, "chSemSignalI");
        CMP      R4,#+0
        BNE.N    ??chSemSignalI_0
        MOV      R2,#+306
        LDR.N    R1,??DataTable6
        LDR.N    R0,??DataTable6_11
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  307   chDbgAssert(((sp->s_cnt >= 0) && isempty(&sp->s_queue)) ||
//  308               ((sp->s_cnt < 0) && notempty(&sp->s_queue)),
//  309               "chSemSignalI(), #1",
//  310               "inconsistent semaphore");
??chSemSignalI_0:
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BMI.N    ??chSemSignalI_1
        LDR      R0,[R4, #+0]
        CMP      R0,R4
        BEQ.N    ??chSemSignalI_2
??chSemSignalI_1:
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BPL.N    ??chSemSignalI_3
        LDR      R0,[R4, #+0]
        CMP      R0,R4
        BNE.N    ??chSemSignalI_2
??chSemSignalI_3:
        LDR.N    R0,??DataTable6_12
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  311 
//  312   if (++sp->s_cnt <= 0) {
??chSemSignalI_2:
        LDR      R0,[R4, #+8]
        ADDS     R0,R0,#+1
        STR      R0,[R4, #+8]
        CMP      R0,#+1
        BGE.N    ??chSemSignalI_4
//  313     /* Note, it is done this way in order to allow a tail call on
//  314              chSchReadyI().*/
//  315     Thread *tp = fifo_remove(&sp->s_queue);
        MOVS     R0,R4
          CFI FunCall fifo_remove
        BL       fifo_remove
        MOVS     R5,R0
//  316     tp->p_u.rdymsg = RDY_OK;
        MOVS     R0,#+0
        STR      R0,[R5, #+40]
//  317     chSchReadyI(tp);
        MOVS     R0,R5
          CFI FunCall chSchReadyI
        BL       chSchReadyI
//  318   }
//  319 }
??chSemSignalI_4:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock11
//  320 
//  321 /**
//  322  * @brief   Adds the specified value to the semaphore counter.
//  323  * @post    This function does not reschedule so a call to a rescheduling
//  324  *          function must be performed before unlocking the kernel. Note that
//  325  *          interrupt handlers always reschedule on exit so an explicit
//  326  *          reschedule must not be performed in ISRs.
//  327  *
//  328  * @param[in] sp        pointer to a @p Semaphore structure
//  329  * @param[in] n         value to be added to the semaphore counter. The value
//  330  *                      must be positive.
//  331  *
//  332  * @iclass
//  333  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function chSemAddCounterI
        THUMB
//  334 void chSemAddCounterI(Semaphore *sp, cnt_t n) {
chSemAddCounterI:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  335 
//  336   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  337   chDbgCheck((sp != NULL) && (n > 0), "chSemAddCounterI");
        CMP      R4,#+0
        BEQ.N    ??chSemAddCounterI_0
        CMP      R5,#+1
        BGE.N    ??chSemAddCounterI_1
??chSemAddCounterI_0:
        MOVW     R2,#+337
        LDR.N    R1,??DataTable6
        LDR.N    R0,??DataTable6_13
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  338   chDbgAssert(((sp->s_cnt >= 0) && isempty(&sp->s_queue)) ||
//  339               ((sp->s_cnt < 0) && notempty(&sp->s_queue)),
//  340               "chSemAddCounterI(), #1",
//  341               "inconsistent semaphore");
??chSemAddCounterI_1:
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BMI.N    ??chSemAddCounterI_2
        LDR      R0,[R4, #+0]
        CMP      R0,R4
        BEQ.N    ??chSemAddCounterI_3
??chSemAddCounterI_2:
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BPL.N    ??chSemAddCounterI_4
        LDR      R0,[R4, #+0]
        CMP      R0,R4
        BNE.N    ??chSemAddCounterI_3
??chSemAddCounterI_4:
        LDR.N    R0,??DataTable6_14
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  342 
//  343   while (n > 0) {
??chSemAddCounterI_3:
        CMP      R5,#+1
        BLT.N    ??chSemAddCounterI_5
//  344     if (++sp->s_cnt <= 0)
        LDR      R0,[R4, #+8]
        ADDS     R0,R0,#+1
        STR      R0,[R4, #+8]
        CMP      R0,#+1
        BGE.N    ??chSemAddCounterI_6
//  345       chSchReadyI(fifo_remove(&sp->s_queue))->p_u.rdymsg = RDY_OK;
        MOVS     R0,R4
          CFI FunCall fifo_remove
        BL       fifo_remove
          CFI FunCall chSchReadyI
        BL       chSchReadyI
        MOVS     R1,#+0
        STR      R1,[R0, #+40]
//  346     n--;
??chSemAddCounterI_6:
        SUBS     R5,R5,#+1
        B.N      ??chSemAddCounterI_3
//  347   }
//  348 }
??chSemAddCounterI_5:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock12

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_1:
        DC32     `?<Constant "\\"chSemInit\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_2:
        DC32     `?<Constant "\\"chSemResetI\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_3:
        DC32     `?<Constant "chSemResetI(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_4:
        DC32     `?<Constant "\\"chSemWaitS\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_5:
        DC32     `?<Constant "chSemWaitS(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_6:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_7:
        DC32     `?<Constant "\\"chSemWaitTimeoutS\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_8:
        DC32     `?<Constant "chSemWaitTimeoutS(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_9:
        DC32     `?<Constant "\\"chSemSignal\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_10:
        DC32     `?<Constant "chSemSignal(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_11:
        DC32     `?<Constant "\\"chSemSignalI\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_12:
        DC32     `?<Constant "chSemSignalI(), #1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_13:
        DC32     `?<Constant "\\"chSemAddCounterI\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_14:
        DC32     `?<Constant "chSemAddCounterI(), #1">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chSemInit\\"()">`:
        DATA
        DC8 "\"chSemInit\"()"
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
        DC8 72H, 63H, 5CH, 63H, 68H, 73H, 65H, 6DH
        DC8 2EH, 63H, 0
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chSemResetI\\"()">`:
        DATA
        DC8 "\"chSemResetI\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "chSemResetI(), #1">`:
        DATA
        DC8 "chSemResetI(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chSemWaitS\\"()">`:
        DATA
        DC8 "\"chSemWaitS\"()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "chSemWaitS(), #1">`:
        DATA
        DC8 "chSemWaitS(), #1"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chSemWaitTimeoutS\\"()">`:
        DATA
        DC8 "\"chSemWaitTimeoutS\"()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "chSemWaitTimeoutS(), #1">`:
        DATA
        DC8 "chSemWaitTimeoutS(), #1"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chSemSignal\\"()">`:
        DATA
        DC8 "\"chSemSignal\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "chSemSignal(), #1">`:
        DATA
        DC8 "chSemSignal(), #1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chSemSignalI\\"()">`:
        DATA
        DC8 "\"chSemSignalI\"()"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "chSemSignalI(), #1">`:
        DATA
        DC8 "chSemSignalI(), #1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chSemAddCounterI\\"()">`:
        DATA
        DC8 "\"chSemAddCounterI\"()"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "chSemAddCounterI(), #1">`:
        DATA
        DC8 "chSemAddCounterI(), #1"
        DC8 0

        END
//  349 
//  350 #if CH_USE_SEMSW || defined(__DOXYGEN__)
//  351 /**
//  352  * @brief   Performs atomic signal and wait operations on two semaphores.
//  353  * @pre     The configuration option @p CH_USE_SEMSW must be enabled in order
//  354  *          to use this function.
//  355  *
//  356  * @param[in] sps       pointer to a @p Semaphore structure to be signaled
//  357  * @param[in] spw       pointer to a @p Semaphore structure to wait on
//  358  * @return              A message specifying how the invoking thread has been
//  359  *                      released from the semaphore.
//  360  * @retval RDY_OK       if the thread has not stopped on the semaphore or the
//  361  *                      semaphore has been signaled.
//  362  * @retval RDY_RESET    if the semaphore has been reset using @p chSemReset().
//  363  *
//  364  * @api
//  365  */
//  366 msg_t chSemSignalWait(Semaphore *sps, Semaphore *spw) {
//  367   msg_t msg;
//  368 
//  369   chDbgCheck((sps != NULL) && (spw != NULL), "chSemSignalWait");
//  370   chDbgAssert(((sps->s_cnt >= 0) && isempty(&sps->s_queue)) ||
//  371               ((sps->s_cnt < 0) && notempty(&sps->s_queue)),
//  372               "chSemSignalWait(), #1",
//  373               "inconsistent semaphore");
//  374   chDbgAssert(((spw->s_cnt >= 0) && isempty(&spw->s_queue)) ||
//  375               ((spw->s_cnt < 0) && notempty(&spw->s_queue)),
//  376               "chSemSignalWait(), #2",
//  377               "inconsistent semaphore");
//  378 
//  379   chSysLock();
//  380   if (++sps->s_cnt <= 0)
//  381     chSchReadyI(fifo_remove(&sps->s_queue))->p_u.rdymsg = RDY_OK;
//  382   if (--spw->s_cnt < 0) {
//  383     Thread *ctp = currp;
//  384     sem_insert(ctp, &spw->s_queue);
//  385     ctp->p_u.wtobjp = spw;
//  386     chSchGoSleepS(THD_STATE_WTSEM);
//  387     msg = ctp->p_u.rdymsg;
//  388   }
//  389   else {
//  390     chSchRescheduleS();
//  391     msg = RDY_OK;
//  392   }
//  393   chSysUnlock();
//  394   return msg;
//  395 }
//  396 #endif /* CH_USE_SEMSW */
//  397 
//  398 #endif /* CH_USE_SEMAPHORES */
//  399 
//  400 /** @} */
// 
// 328 bytes in section .rodata
// 814 bytes in section .text
// 
// 814 bytes of CODE  memory
// 328 bytes of CONST memory
//
//Errors: none
//Warnings: none
