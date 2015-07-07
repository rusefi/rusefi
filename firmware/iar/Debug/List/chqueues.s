///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:29 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chqueues.c                                    /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chqueues.c -lCN F:\stuff\rusefi_sourceforge\f /
//                       irmware\iar\Debug\List\ -lA                          /
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
//                       chqueues.s                                           /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME chqueues

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN chDbgCheckClassI
        EXTERN chDbgPanic3
        EXTERN chSchGoSleepTimeoutS
        EXTERN chSchReadyI
        EXTERN dbg_check_lock
        EXTERN dbg_check_unlock
        EXTERN rlist

        PUBLIC chIQGetTimeout
        PUBLIC chIQInit
        PUBLIC chIQPutI
        PUBLIC chIQReadTimeout
        PUBLIC chIQResetI
        PUBLIC chOQGetI
        PUBLIC chOQInit
        PUBLIC chOQPutTimeout
        PUBLIC chOQResetI
        PUBLIC chOQWriteTimeout
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\kernel\src\chqueues.c
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
//   29  * @file    chqueues.c
//   30  * @brief   I/O Queues code.
//   31  *
//   32  * @addtogroup io_queues
//   33  * @details ChibiOS/RT queues are mostly used in serial-like device drivers.
//   34  *          The device drivers are usually designed to have a lower side
//   35  *          (lower driver, it is usually an interrupt service routine) and an
//   36  *          upper side (upper driver, accessed by the application threads).<br>
//   37  *          There are several kind of queues:<br>
//   38  *          - <b>Input queue</b>, unidirectional queue where the writer is the
//   39  *            lower side and the reader is the upper side.
//   40  *          - <b>Output queue</b>, unidirectional queue where the writer is the
//   41  *            upper side and the reader is the lower side.
//   42  *          - <b>Full duplex queue</b>, bidirectional queue. Full duplex queues
//   43  *            are implemented by pairing an input queue and an output queue
//   44  *            together.
//   45  *          .
//   46  * @pre     In order to use the I/O queues the @p CH_USE_QUEUES option must
//   47  *          be enabled in @p chconf.h.
//   48  * @{
//   49  */
//   50 
//   51 #include "ch.h"

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
//   52 
//   53 #if CH_USE_QUEUES || defined(__DOXYGEN__)
//   54 
//   55 /**
//   56  * @brief   Puts the invoking thread into the queue's threads queue.
//   57  *
//   58  * @param[out] qp       pointer to an @p GenericQueue structure
//   59  * @param[in] time      the number of ticks before the operation timeouts,
//   60  *                      the following special values are allowed:
//   61  *                      - @a TIME_IMMEDIATE immediate timeout.
//   62  *                      - @a TIME_INFINITE no timeout.
//   63  *                      .
//   64  * @return              A message specifying how the invoking thread has been
//   65  *                      released from threads queue.
//   66  * @retval Q_OK         is the normal exit, thread signaled.
//   67  * @retval Q_RESET      if the queue has been reset.
//   68  * @retval Q_TIMEOUT    if the queue operation timed out.
//   69  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function qwait
        THUMB
//   70 static msg_t qwait(GenericQueue *qp, systime_t time) {
qwait:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   71 
//   72   if (TIME_IMMEDIATE == time)
        CMP      R5,#+0
        BNE.N    ??qwait_0
//   73     return Q_TIMEOUT;
        MOVS     R0,#-1
        B.N      ??qwait_1
//   74   currp->p_u.wtobjp = qp;
??qwait_0:
        LDR.N    R0,??DataTable2
        LDR      R0,[R0, #+24]
        STR      R4,[R0, #+40]
//   75   queue_insert(currp, &qp->q_waiting);
        MOVS     R1,R4
        LDR.N    R0,??DataTable2
        LDR      R0,[R0, #+24]
          CFI FunCall queue_insert
        BL       queue_insert
//   76   return chSchGoSleepTimeoutS(THD_STATE_WTQUEUE, time);
        MOVS     R1,R5
        MOVS     R0,#+13
          CFI FunCall chSchGoSleepTimeoutS
        BL       chSchGoSleepTimeoutS
??qwait_1:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2
//   77 }
//   78 
//   79 /**
//   80  * @brief   Initializes an input queue.
//   81  * @details A Semaphore is internally initialized and works as a counter of
//   82  *          the bytes contained in the queue.
//   83  * @note    The callback is invoked from within the S-Locked system state,
//   84  *          see @ref system_states.
//   85  *
//   86  * @param[out] iqp      pointer to an @p InputQueue structure
//   87  * @param[in] bp        pointer to a memory area allocated as queue buffer
//   88  * @param[in] size      size of the queue buffer
//   89  * @param[in] infy      pointer to a callback function that is invoked when
//   90  *                      data is read from the queue. The value can be @p NULL.
//   91  * @param[in] link      application defined pointer
//   92  *
//   93  * @init
//   94  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function chIQInit
          CFI NoCalls
        THUMB
//   95 void chIQInit(InputQueue *iqp, uint8_t *bp, size_t size, qnotify_t infy,
//   96               void *link) {
chIQInit:
        PUSH     {R4,R5}
          CFI R5 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        LDR      R4,[SP, #+8]
//   97 
//   98   queue_init(&iqp->q_waiting);
        STR      R0,[R0, #+4]
        STR      R0,[R0, #+0]
//   99   iqp->q_counter = 0;
        MOVS     R5,#+0
        STR      R5,[R0, #+8]
//  100   iqp->q_buffer = iqp->q_rdptr = iqp->q_wrptr = bp;
        STR      R1,[R0, #+20]
        STR      R1,[R0, #+24]
        STR      R1,[R0, #+12]
//  101   iqp->q_top = bp + size;
        ADDS     R5,R2,R1
        STR      R5,[R0, #+16]
//  102   iqp->q_notify = infy;
        STR      R3,[R0, #+28]
//  103   iqp->q_link = link;
        STR      R4,[R0, #+32]
//  104 }
        POP      {R4,R5}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock3
//  105 
//  106 /**
//  107  * @brief   Resets an input queue.
//  108  * @details All the data in the input queue is erased and lost, any waiting
//  109  *          thread is resumed with status @p Q_RESET.
//  110  * @note    A reset operation can be used by a low level driver in order to
//  111  *          obtain immediate attention from the high level layers.
//  112  *
//  113  * @param[in] iqp       pointer to an @p InputQueue structure
//  114  *
//  115  * @iclass
//  116  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function chIQResetI
        THUMB
//  117 void chIQResetI(InputQueue *iqp) {
chIQResetI:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  118 
//  119   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  120 
//  121   iqp->q_rdptr = iqp->q_wrptr = iqp->q_buffer;
        LDR      R0,[R4, #+12]
        STR      R0,[R4, #+20]
        STR      R0,[R4, #+24]
//  122   iqp->q_counter = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+8]
//  123   while (notempty(&iqp->q_waiting))
??chIQResetI_0:
        LDR      R0,[R4, #+0]
        CMP      R0,R4
        BEQ.N    ??chIQResetI_1
//  124     chSchReadyI(fifo_remove(&iqp->q_waiting))->p_u.rdymsg = Q_RESET;
        MOVS     R0,R4
          CFI FunCall fifo_remove
        BL       fifo_remove
          CFI FunCall chSchReadyI
        BL       chSchReadyI
        MVNS     R1,#+1
        STR      R1,[R0, #+40]
        B.N      ??chIQResetI_0
//  125 }
??chIQResetI_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock4
//  126 
//  127 /**
//  128  * @brief   Input queue write.
//  129  * @details A byte value is written into the low end of an input queue.
//  130  *
//  131  * @param[in] iqp       pointer to an @p InputQueue structure
//  132  * @param[in] b         the byte value to be written in the queue
//  133  * @return              The operation status.
//  134  * @retval Q_OK         if the operation has been completed with success.
//  135  * @retval Q_FULL       if the queue is full and the operation cannot be
//  136  *                      completed.
//  137  *
//  138  * @iclass
//  139  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function chIQPutI
        THUMB
//  140 msg_t chIQPutI(InputQueue *iqp, uint8_t b) {
chIQPutI:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  141 
//  142   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  143 
//  144   if (chIQIsFullI(iqp))
        LDR      R0,[R4, #+20]
        LDR      R1,[R4, #+24]
        CMP      R0,R1
        BNE.N    ??chIQPutI_0
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BEQ.N    ??chIQPutI_0
        MOVS     R0,#+1
        B.N      ??chIQPutI_1
??chIQPutI_0:
        MOVS     R0,#+0
??chIQPutI_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+0
        BEQ.N    ??chIQPutI_2
//  145     return Q_FULL;
        MVNS     R0,#+3
        B.N      ??chIQPutI_3
//  146 
//  147   iqp->q_counter++;
??chIQPutI_2:
        LDR      R0,[R4, #+8]
        ADDS     R0,R0,#+1
        STR      R0,[R4, #+8]
//  148   *iqp->q_wrptr++ = b;
        LDR      R0,[R4, #+20]
        ADDS     R1,R0,#+1
        STR      R1,[R4, #+20]
        STRB     R5,[R0, #+0]
//  149   if (iqp->q_wrptr >= iqp->q_top)
        LDR      R0,[R4, #+20]
        LDR      R1,[R4, #+16]
        CMP      R0,R1
        BCC.N    ??chIQPutI_4
//  150     iqp->q_wrptr = iqp->q_buffer;
        LDR      R0,[R4, #+12]
        STR      R0,[R4, #+20]
//  151 
//  152   if (notempty(&iqp->q_waiting))
??chIQPutI_4:
        LDR      R0,[R4, #+0]
        CMP      R0,R4
        BEQ.N    ??chIQPutI_5
//  153     chSchReadyI(fifo_remove(&iqp->q_waiting))->p_u.rdymsg = Q_OK;
        MOVS     R0,R4
          CFI FunCall fifo_remove
        BL       fifo_remove
          CFI FunCall chSchReadyI
        BL       chSchReadyI
        MOVS     R1,#+0
        STR      R1,[R0, #+40]
//  154 
//  155   return Q_OK;
??chIQPutI_5:
        MOVS     R0,#+0
??chIQPutI_3:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock5
//  156 }
//  157 
//  158 /**
//  159  * @brief   Input queue read with timeout.
//  160  * @details This function reads a byte value from an input queue. If the queue
//  161  *          is empty then the calling thread is suspended until a byte arrives
//  162  *          in the queue or a timeout occurs.
//  163  * @note    The callback is invoked before reading the character from the
//  164  *          buffer or before entering the state @p THD_STATE_WTQUEUE.
//  165  *
//  166  * @param[in] iqp       pointer to an @p InputQueue structure
//  167  * @param[in] time      the number of ticks before the operation timeouts,
//  168  *                      the following special values are allowed:
//  169  *                      - @a TIME_IMMEDIATE immediate timeout.
//  170  *                      - @a TIME_INFINITE no timeout.
//  171  *                      .
//  172  * @return              A byte value from the queue.
//  173  * @retval Q_TIMEOUT    if the specified time expired.
//  174  * @retval Q_RESET      if the queue has been reset.
//  175  *
//  176  * @api
//  177  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function chIQGetTimeout
        THUMB
//  178 msg_t chIQGetTimeout(InputQueue *iqp, systime_t time) {
chIQGetTimeout:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  179   uint8_t b;
//  180 
//  181   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  182   if (iqp->q_notify)
        LDR      R0,[R4, #+28]
        CMP      R0,#+0
        BEQ.N    ??chIQGetTimeout_0
//  183     iqp->q_notify(iqp);
        MOVS     R0,R4
        LDR      R1,[R4, #+28]
          CFI FunCall
        BLX      R1
//  184 
//  185   while (chIQIsEmptyI(iqp)) {
??chIQGetTimeout_0:
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BNE.N    ??chIQGetTimeout_1
//  186     msg_t msg;
//  187     if ((msg = qwait((GenericQueue *)iqp, time)) < Q_OK) {
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall qwait
        BL       qwait
        MOVS     R7,R0
        CMP      R0,#+0
        BPL.N    ??chIQGetTimeout_0
//  188       chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  189       return msg;
        MOVS     R0,R7
        B.N      ??chIQGetTimeout_2
//  190     }
//  191   }
//  192 
//  193   iqp->q_counter--;
??chIQGetTimeout_1:
        LDR      R0,[R4, #+8]
        SUBS     R0,R0,#+1
        STR      R0,[R4, #+8]
//  194   b = *iqp->q_rdptr++;
        LDR      R0,[R4, #+24]
        ADDS     R1,R0,#+1
        STR      R1,[R4, #+24]
        LDRB     R0,[R0, #+0]
        MOVS     R6,R0
//  195   if (iqp->q_rdptr >= iqp->q_top)
        LDR      R0,[R4, #+24]
        LDR      R1,[R4, #+16]
        CMP      R0,R1
        BCC.N    ??chIQGetTimeout_3
//  196     iqp->q_rdptr = iqp->q_buffer;
        LDR      R0,[R4, #+12]
        STR      R0,[R4, #+24]
//  197 
//  198   chSysUnlock();
??chIQGetTimeout_3:
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  199   return b;
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        MOVS     R0,R6
??chIQGetTimeout_2:
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock6
//  200 }
//  201 
//  202 /**
//  203  * @brief   Input queue read with timeout.
//  204  * @details The function reads data from an input queue into a buffer. The
//  205  *          operation completes when the specified amount of data has been
//  206  *          transferred or after the specified timeout or if the queue has
//  207  *          been reset.
//  208  * @note    The function is not atomic, if you need atomicity it is suggested
//  209  *          to use a semaphore or a mutex for mutual exclusion.
//  210  * @note    The callback is invoked before reading each character from the
//  211  *          buffer or before entering the state @p THD_STATE_WTQUEUE.
//  212  *
//  213  * @param[in] iqp       pointer to an @p InputQueue structure
//  214  * @param[out] bp       pointer to the data buffer
//  215  * @param[in] n         the maximum amount of data to be transferred, the
//  216  *                      value 0 is reserved
//  217  * @param[in] time      the number of ticks before the operation timeouts,
//  218  *                      the following special values are allowed:
//  219  *                      - @a TIME_IMMEDIATE immediate timeout.
//  220  *                      - @a TIME_INFINITE no timeout.
//  221  *                      .
//  222  * @return              The number of bytes effectively transferred.
//  223  *
//  224  * @api
//  225  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function chIQReadTimeout
        THUMB
//  226 size_t chIQReadTimeout(InputQueue *iqp, uint8_t *bp,
//  227                        size_t n, systime_t time) {
chIQReadTimeout:
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
//  228   qnotify_t nfy = iqp->q_notify;
        LDR      R9,[R4, #+28]
//  229   size_t r = 0;
        MOVS     R8,#+0
//  230 
//  231   chDbgCheck(n > 0, "chIQReadTimeout");
        CMP      R6,#+0
        BNE.N    ??chIQReadTimeout_0
        MOVS     R2,#+231
        LDR.N    R1,??DataTable2_1
        LDR.N    R0,??DataTable2_2
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  232 
//  233   chSysLock();
??chIQReadTimeout_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  234   while (TRUE) {
//  235     if (nfy)
??chIQReadTimeout_1:
        MOV      R0,R9
        CMP      R0,#+0
        BEQ.N    ??chIQReadTimeout_2
//  236       nfy(iqp);
        MOVS     R0,R4
          CFI FunCall
        BLX      R9
//  237 
//  238     while (chIQIsEmptyI(iqp)) {
??chIQReadTimeout_2:
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BNE.N    ??chIQReadTimeout_3
//  239       if (qwait((GenericQueue *)iqp, time) != Q_OK) {
        MOVS     R1,R7
        MOVS     R0,R4
          CFI FunCall qwait
        BL       qwait
        CMP      R0,#+0
        BEQ.N    ??chIQReadTimeout_2
//  240         chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  241         return r;
        MOV      R0,R8
        B.N      ??chIQReadTimeout_4
//  242       }
//  243     }
//  244 
//  245     iqp->q_counter--;
??chIQReadTimeout_3:
        LDR      R0,[R4, #+8]
        SUBS     R0,R0,#+1
        STR      R0,[R4, #+8]
//  246     *bp++ = *iqp->q_rdptr++;
        LDR      R0,[R4, #+24]
        ADDS     R1,R0,#+1
        STR      R1,[R4, #+24]
        LDRB     R0,[R0, #+0]
        STRB     R0,[R5, #+0]
        ADDS     R5,R5,#+1
//  247     if (iqp->q_rdptr >= iqp->q_top)
        LDR      R0,[R4, #+24]
        LDR      R1,[R4, #+16]
        CMP      R0,R1
        BCC.N    ??chIQReadTimeout_5
//  248       iqp->q_rdptr = iqp->q_buffer;
        LDR      R0,[R4, #+12]
        STR      R0,[R4, #+24]
//  249 
//  250     chSysUnlock(); /* Gives a preemption chance in a controlled point.*/
??chIQReadTimeout_5:
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  251     r++;
        ADDS     R8,R8,#+1
//  252     if (--n == 0)
        SUBS     R6,R6,#+1
        CMP      R6,#+0
        BNE.N    ??chIQReadTimeout_6
//  253       return r;
        MOV      R0,R8
        B.N      ??chIQReadTimeout_4
//  254 
//  255     chSysLock();
??chIQReadTimeout_6:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
        B.N      ??chIQReadTimeout_1
//  256   }
??chIQReadTimeout_4:
        POP      {R1,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock7
//  257 }
//  258 
//  259 /**
//  260  * @brief   Initializes an output queue.
//  261  * @details A Semaphore is internally initialized and works as a counter of
//  262  *          the free bytes in the queue.
//  263  * @note    The callback is invoked from within the S-Locked system state,
//  264  *          see @ref system_states.
//  265  *
//  266  * @param[out] oqp      pointer to an @p OutputQueue structure
//  267  * @param[in] bp        pointer to a memory area allocated as queue buffer
//  268  * @param[in] size      size of the queue buffer
//  269  * @param[in] onfy      pointer to a callback function that is invoked when
//  270  *                      data is written to the queue. The value can be @p NULL.
//  271  * @param[in] link      application defined pointer
//  272  *
//  273  * @init
//  274  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function chOQInit
          CFI NoCalls
        THUMB
//  275 void chOQInit(OutputQueue *oqp, uint8_t *bp, size_t size, qnotify_t onfy,
//  276               void *link) {
chOQInit:
        PUSH     {R4,R5}
          CFI R5 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        LDR      R4,[SP, #+8]
//  277 
//  278   queue_init(&oqp->q_waiting);
        STR      R0,[R0, #+4]
        STR      R0,[R0, #+0]
//  279   oqp->q_counter = size;
        STR      R2,[R0, #+8]
//  280   oqp->q_buffer = oqp->q_rdptr = oqp->q_wrptr = bp;
        STR      R1,[R0, #+20]
        STR      R1,[R0, #+24]
        STR      R1,[R0, #+12]
//  281   oqp->q_top = bp + size;
        ADDS     R5,R2,R1
        STR      R5,[R0, #+16]
//  282   oqp->q_notify = onfy;
        STR      R3,[R0, #+28]
//  283   oqp->q_link = link;
        STR      R4,[R0, #+32]
//  284 }
        POP      {R4,R5}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock8
//  285 
//  286 /**
//  287  * @brief   Resets an output queue.
//  288  * @details All the data in the output queue is erased and lost, any waiting
//  289  *          thread is resumed with status @p Q_RESET.
//  290  * @note    A reset operation can be used by a low level driver in order to
//  291  *          obtain immediate attention from the high level layers.
//  292  *
//  293  * @param[in] oqp       pointer to an @p OutputQueue structure
//  294  *
//  295  * @iclass
//  296  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function chOQResetI
        THUMB
//  297 void chOQResetI(OutputQueue *oqp) {
chOQResetI:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  298 
//  299   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  300 
//  301   oqp->q_rdptr = oqp->q_wrptr = oqp->q_buffer;
        LDR      R0,[R4, #+12]
        STR      R0,[R4, #+20]
        STR      R0,[R4, #+24]
//  302   oqp->q_counter = chQSizeI(oqp);
        LDR      R0,[R4, #+16]
        LDR      R1,[R4, #+12]
        SUBS     R0,R0,R1
        STR      R0,[R4, #+8]
//  303   while (notempty(&oqp->q_waiting))
??chOQResetI_0:
        LDR      R0,[R4, #+0]
        CMP      R0,R4
        BEQ.N    ??chOQResetI_1
//  304     chSchReadyI(fifo_remove(&oqp->q_waiting))->p_u.rdymsg = Q_RESET;
        MOVS     R0,R4
          CFI FunCall fifo_remove
        BL       fifo_remove
          CFI FunCall chSchReadyI
        BL       chSchReadyI
        MVNS     R1,#+1
        STR      R1,[R0, #+40]
        B.N      ??chOQResetI_0
//  305 }
??chOQResetI_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock9
//  306 
//  307 /**
//  308  * @brief   Output queue write with timeout.
//  309  * @details This function writes a byte value to an output queue. If the queue
//  310  *          is full then the calling thread is suspended until there is space
//  311  *          in the queue or a timeout occurs.
//  312  * @note    The callback is invoked after writing the character into the
//  313  *          buffer.
//  314  *
//  315  * @param[in] oqp       pointer to an @p OutputQueue structure
//  316  * @param[in] b         the byte value to be written in the queue
//  317  * @param[in] time      the number of ticks before the operation timeouts,
//  318  *                      the following special values are allowed:
//  319  *                      - @a TIME_IMMEDIATE immediate timeout.
//  320  *                      - @a TIME_INFINITE no timeout.
//  321  *                      .
//  322  * @return              The operation status.
//  323  * @retval Q_OK         if the operation succeeded.
//  324  * @retval Q_TIMEOUT    if the specified time expired.
//  325  * @retval Q_RESET      if the queue has been reset.
//  326  *
//  327  * @api
//  328  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function chOQPutTimeout
        THUMB
//  329 msg_t chOQPutTimeout(OutputQueue *oqp, uint8_t b, systime_t time) {
chOQPutTimeout:
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
//  330 
//  331   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  332   while (chOQIsFullI(oqp)) {
??chOQPutTimeout_0:
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BNE.N    ??chOQPutTimeout_1
//  333     msg_t msg;
//  334 
//  335     if ((msg = qwait((GenericQueue *)oqp, time)) < Q_OK) {
        MOVS     R1,R6
        MOVS     R0,R4
          CFI FunCall qwait
        BL       qwait
        MOVS     R7,R0
        CMP      R0,#+0
        BPL.N    ??chOQPutTimeout_0
//  336       chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  337       return msg;
        MOVS     R0,R7
        B.N      ??chOQPutTimeout_2
//  338     }
//  339   }
//  340 
//  341   oqp->q_counter--;
??chOQPutTimeout_1:
        LDR      R0,[R4, #+8]
        SUBS     R0,R0,#+1
        STR      R0,[R4, #+8]
//  342   *oqp->q_wrptr++ = b;
        LDR      R0,[R4, #+20]
        ADDS     R1,R0,#+1
        STR      R1,[R4, #+20]
        STRB     R5,[R0, #+0]
//  343   if (oqp->q_wrptr >= oqp->q_top)
        LDR      R0,[R4, #+20]
        LDR      R1,[R4, #+16]
        CMP      R0,R1
        BCC.N    ??chOQPutTimeout_3
//  344     oqp->q_wrptr = oqp->q_buffer;
        LDR      R0,[R4, #+12]
        STR      R0,[R4, #+20]
//  345 
//  346   if (oqp->q_notify)
??chOQPutTimeout_3:
        LDR      R0,[R4, #+28]
        CMP      R0,#+0
        BEQ.N    ??chOQPutTimeout_4
//  347     oqp->q_notify(oqp);
        MOVS     R0,R4
        LDR      R1,[R4, #+28]
          CFI FunCall
        BLX      R1
//  348 
//  349   chSysUnlock();
??chOQPutTimeout_4:
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  350   return Q_OK;
        MOVS     R0,#+0
??chOQPutTimeout_2:
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock10
//  351 }
//  352 
//  353 /**
//  354  * @brief   Output queue read.
//  355  * @details A byte value is read from the low end of an output queue.
//  356  *
//  357  * @param[in] oqp       pointer to an @p OutputQueue structure
//  358  * @return              The byte value from the queue.
//  359  * @retval Q_EMPTY      if the queue is empty.
//  360  *
//  361  * @iclass
//  362  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function chOQGetI
        THUMB
//  363 msg_t chOQGetI(OutputQueue *oqp) {
chOQGetI:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  364   uint8_t b;
//  365 
//  366   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  367 
//  368   if (chOQIsEmptyI(oqp))
        LDR      R0,[R4, #+20]
        LDR      R1,[R4, #+24]
        CMP      R0,R1
        BNE.N    ??chOQGetI_0
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BEQ.N    ??chOQGetI_0
        MOVS     R0,#+1
        B.N      ??chOQGetI_1
??chOQGetI_0:
        MOVS     R0,#+0
??chOQGetI_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+0
        BEQ.N    ??chOQGetI_2
//  369     return Q_EMPTY;
        MVNS     R0,#+2
        B.N      ??chOQGetI_3
//  370 
//  371   oqp->q_counter++;
??chOQGetI_2:
        LDR      R0,[R4, #+8]
        ADDS     R0,R0,#+1
        STR      R0,[R4, #+8]
//  372   b = *oqp->q_rdptr++;
        LDR      R0,[R4, #+24]
        ADDS     R1,R0,#+1
        STR      R1,[R4, #+24]
        LDRB     R0,[R0, #+0]
        MOVS     R5,R0
//  373   if (oqp->q_rdptr >= oqp->q_top)
        LDR      R0,[R4, #+24]
        LDR      R1,[R4, #+16]
        CMP      R0,R1
        BCC.N    ??chOQGetI_4
//  374     oqp->q_rdptr = oqp->q_buffer;
        LDR      R0,[R4, #+12]
        STR      R0,[R4, #+24]
//  375 
//  376   if (notempty(&oqp->q_waiting))
??chOQGetI_4:
        LDR      R0,[R4, #+0]
        CMP      R0,R4
        BEQ.N    ??chOQGetI_5
//  377     chSchReadyI(fifo_remove(&oqp->q_waiting))->p_u.rdymsg = Q_OK;
        MOVS     R0,R4
          CFI FunCall fifo_remove
        BL       fifo_remove
          CFI FunCall chSchReadyI
        BL       chSchReadyI
        MOVS     R1,#+0
        STR      R1,[R0, #+40]
//  378 
//  379   return b;
??chOQGetI_5:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        MOVS     R0,R5
??chOQGetI_3:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock11
//  380 }
//  381 
//  382 /**
//  383  * @brief   Output queue write with timeout.
//  384  * @details The function writes data from a buffer to an output queue. The
//  385  *          operation completes when the specified amount of data has been
//  386  *          transferred or after the specified timeout or if the queue has
//  387  *          been reset.
//  388  * @note    The function is not atomic, if you need atomicity it is suggested
//  389  *          to use a semaphore or a mutex for mutual exclusion.
//  390  * @note    The callback is invoked after writing each character into the
//  391  *          buffer.
//  392  *
//  393  * @param[in] oqp       pointer to an @p OutputQueue structure
//  394  * @param[in] bp        pointer to the data buffer
//  395  * @param[in] n         the maximum amount of data to be transferred, the
//  396  *                      value 0 is reserved
//  397  * @param[in] time      the number of ticks before the operation timeouts,
//  398  *                      the following special values are allowed:
//  399  *                      - @a TIME_IMMEDIATE immediate timeout.
//  400  *                      - @a TIME_INFINITE no timeout.
//  401  *                      .
//  402  * @return              The number of bytes effectively transferred.
//  403  *
//  404  * @api
//  405  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function chOQWriteTimeout
        THUMB
//  406 size_t chOQWriteTimeout(OutputQueue *oqp, const uint8_t *bp,
//  407                         size_t n, systime_t time) {
chOQWriteTimeout:
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
//  408   qnotify_t nfy = oqp->q_notify;
        LDR      R8,[R4, #+28]
//  409   size_t w = 0;
        MOVS     R9,#+0
//  410 
//  411   chDbgCheck(n > 0, "chOQWriteTimeout");
        CMP      R6,#+0
        BNE.N    ??chOQWriteTimeout_0
        MOVW     R2,#+411
        LDR.N    R1,??DataTable2_1
        LDR.N    R0,??DataTable2_3
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  412 
//  413   chSysLock();
??chOQWriteTimeout_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  414   while (TRUE) {
//  415     while (chOQIsFullI(oqp)) {
??chOQWriteTimeout_1:
??chOQWriteTimeout_2:
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BNE.N    ??chOQWriteTimeout_3
//  416       if (qwait((GenericQueue *)oqp, time) != Q_OK) {
        MOVS     R1,R7
        MOVS     R0,R4
          CFI FunCall qwait
        BL       qwait
        CMP      R0,#+0
        BEQ.N    ??chOQWriteTimeout_2
//  417         chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  418         return w;
        MOV      R0,R9
        B.N      ??chOQWriteTimeout_4
//  419       }
//  420     }
//  421     oqp->q_counter--;
??chOQWriteTimeout_3:
        LDR      R0,[R4, #+8]
        SUBS     R0,R0,#+1
        STR      R0,[R4, #+8]
//  422     *oqp->q_wrptr++ = *bp++;
        LDR      R0,[R4, #+20]
        ADDS     R1,R0,#+1
        STR      R1,[R4, #+20]
        LDRB     R1,[R5, #+0]
        STRB     R1,[R0, #+0]
        ADDS     R5,R5,#+1
//  423     if (oqp->q_wrptr >= oqp->q_top)
        LDR      R0,[R4, #+20]
        LDR      R1,[R4, #+16]
        CMP      R0,R1
        BCC.N    ??chOQWriteTimeout_5
//  424       oqp->q_wrptr = oqp->q_buffer;
        LDR      R0,[R4, #+12]
        STR      R0,[R4, #+20]
//  425 
//  426     if (nfy)
??chOQWriteTimeout_5:
        MOV      R0,R8
        CMP      R0,#+0
        BEQ.N    ??chOQWriteTimeout_6
//  427       nfy(oqp);
        MOVS     R0,R4
          CFI FunCall
        BLX      R8
//  428 
//  429     chSysUnlock(); /* Gives a preemption chance in a controlled point.*/
??chOQWriteTimeout_6:
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  430     w++;
        ADDS     R9,R9,#+1
//  431     if (--n == 0)
        SUBS     R6,R6,#+1
        CMP      R6,#+0
        BNE.N    ??chOQWriteTimeout_7
//  432       return w;
        MOV      R0,R9
        B.N      ??chOQWriteTimeout_4
//  433     chSysLock();
??chOQWriteTimeout_7:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
        B.N      ??chOQWriteTimeout_1
//  434   }
??chOQWriteTimeout_4:
        POP      {R1,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock12
//  435 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_1:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_2:
        DC32     `?<Constant "\\"chIQReadTimeout\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_3:
        DC32     `?<Constant "\\"chOQWriteTimeout\\"()">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chIQReadTimeout\\"()">`:
        DATA
        DC8 "\"chIQReadTimeout\"()"

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
        DC8 72H, 63H, 5CH, 63H, 68H, 71H, 75H, 65H
        DC8 75H, 65H, 73H, 2EH, 63H, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chOQWriteTimeout\\"()">`:
        DATA
        DC8 "\"chOQWriteTimeout\"()"
        DC8 0, 0, 0

        END
//  436 #endif  /* CH_USE_QUEUES */
//  437 
//  438 /** @} */
// 
// 116 bytes in section .rodata
// 944 bytes in section .text
// 
// 944 bytes of CODE  memory
// 116 bytes of CONST memory
//
//Errors: none
//Warnings: none
