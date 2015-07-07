///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:17 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chmboxes.c                                    /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chmboxes.c -lCN F:\stuff\rusefi_sourceforge\f /
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
//                       chmboxes.s                                           /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME chmboxes

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN chDbgCheckClassI
        EXTERN chDbgCheckClassS
        EXTERN chDbgPanic3
        EXTERN chSchRescheduleS
        EXTERN chSemInit
        EXTERN chSemResetI
        EXTERN chSemSignalI
        EXTERN chSemWaitTimeoutS
        EXTERN dbg_check_lock
        EXTERN dbg_check_unlock

        PUBLIC chMBFetch
        PUBLIC chMBFetchI
        PUBLIC chMBFetchS
        PUBLIC chMBInit
        PUBLIC chMBPost
        PUBLIC chMBPostAhead
        PUBLIC chMBPostAheadI
        PUBLIC chMBPostAheadS
        PUBLIC chMBPostI
        PUBLIC chMBPostS
        PUBLIC chMBReset
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\kernel\src\chmboxes.c
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
//   29  * @file    chmboxes.c
//   30  * @brief   Mailboxes code.
//   31  *
//   32  * @addtogroup mailboxes
//   33  * @details Asynchronous messages.
//   34  *          <h2>Operation mode</h2>
//   35  *          A mailbox is an asynchronous communication mechanism.<br>
//   36  *          Operations defined for mailboxes:
//   37  *          - <b>Post</b>: Posts a message on the mailbox in FIFO order.
//   38  *          - <b>Post Ahead</b>: Posts a message on the mailbox with urgent
//   39  *            priority.
//   40  *          - <b>Fetch</b>: A message is fetched from the mailbox and removed
//   41  *            from the queue.
//   42  *          - <b>Reset</b>: The mailbox is emptied and all the stored messages
//   43  *            are lost.
//   44  *          .
//   45  *          A message is a variable of type msg_t that is guaranteed to have
//   46  *          the same size of and be compatible with (data) pointers (anyway an
//   47  *          explicit cast is needed).
//   48  *          If larger messages need to be exchanged then a pointer to a
//   49  *          structure can be posted in the mailbox but the posting side has
//   50  *          no predefined way to know when the message has been processed. A
//   51  *          possible approach is to allocate memory (from a memory pool for
//   52  *          example) from the posting side and free it on the fetching side.
//   53  *          Another approach is to set a "done" flag into the structure pointed
//   54  *          by the message.
//   55  * @pre     In order to use the mailboxes APIs the @p CH_USE_MAILBOXES option
//   56  *          must be enabled in @p chconf.h.
//   57  * @{
//   58  */
//   59 
//   60 #include "ch.h"
//   61 
//   62 #if CH_USE_MAILBOXES || defined(__DOXYGEN__)
//   63 /**
//   64  * @brief   Initializes a Mailbox object.
//   65  *
//   66  * @param[out] mbp      the pointer to the Mailbox structure to be initialized
//   67  * @param[in] buf       pointer to the messages buffer as an array of @p msg_t
//   68  * @param[in] n         number of elements in the buffer array
//   69  *
//   70  * @init
//   71  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function chMBInit
        THUMB
//   72 void chMBInit(Mailbox *mbp, msg_t *buf, cnt_t n) {
chMBInit:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   73 
//   74   chDbgCheck((mbp != NULL) && (buf != NULL) && (n > 0), "chMBInit");
        CMP      R4,#+0
        BEQ.N    ??chMBInit_0
        CMP      R5,#+0
        BEQ.N    ??chMBInit_0
        CMP      R6,#+1
        BGE.N    ??chMBInit_1
??chMBInit_0:
        MOVS     R2,#+74
        LDR.N    R1,??DataTable7
        LDR.N    R0,??DataTable7_1
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//   75 
//   76   mbp->mb_buffer = mbp->mb_wrptr = mbp->mb_rdptr = buf;
??chMBInit_1:
        STR      R5,[R4, #+12]
        STR      R5,[R4, #+8]
        STR      R5,[R4, #+0]
//   77   mbp->mb_top = &buf[n];
        ADDS     R0,R5,R6, LSL #+2
        STR      R0,[R4, #+4]
//   78   chSemInit(&mbp->mb_emptysem, n);
        MOVS     R1,R6
        ADDS     R0,R4,#+28
          CFI FunCall chSemInit
        BL       chSemInit
//   79   chSemInit(&mbp->mb_fullsem, 0);
        MOVS     R1,#+0
        ADDS     R0,R4,#+16
          CFI FunCall chSemInit
        BL       chSemInit
//   80 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock0
//   81 
//   82 /**
//   83  * @brief   Resets a Mailbox object.
//   84  * @details All the waiting threads are resumed with status @p RDY_RESET and
//   85  *          the queued messages are lost.
//   86  *
//   87  * @param[in] mbp       the pointer to an initialized Mailbox object
//   88  *
//   89  * @api
//   90  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function chMBReset
        THUMB
//   91 void chMBReset(Mailbox *mbp) {
chMBReset:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   92 
//   93   chDbgCheck(mbp != NULL, "chMBReset");
        CMP      R4,#+0
        BNE.N    ??chMBReset_0
        MOVS     R2,#+93
        LDR.N    R1,??DataTable7
        LDR.N    R0,??DataTable7_2
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//   94 
//   95   chSysLock();
??chMBReset_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//   96   mbp->mb_wrptr = mbp->mb_rdptr = mbp->mb_buffer;
        LDR      R0,[R4, #+0]
        STR      R0,[R4, #+12]
        STR      R0,[R4, #+8]
//   97   chSemResetI(&mbp->mb_emptysem, mbp->mb_top - mbp->mb_buffer);
        LDR      R0,[R4, #+4]
        LDR      R1,[R4, #+0]
        SUBS     R0,R0,R1
        ASRS     R1,R0,#+2
        ADDS     R0,R4,#+28
          CFI FunCall chSemResetI
        BL       chSemResetI
//   98   chSemResetI(&mbp->mb_fullsem, 0);
        MOVS     R1,#+0
        ADDS     R0,R4,#+16
          CFI FunCall chSemResetI
        BL       chSemResetI
//   99   chSchRescheduleS();
          CFI FunCall chSchRescheduleS
        BL       chSchRescheduleS
//  100   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  101 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1
//  102 
//  103 /**
//  104  * @brief   Posts a message into a mailbox.
//  105  * @details The invoking thread waits until a empty slot in the mailbox becomes
//  106  *          available or the specified time runs out.
//  107  *
//  108  * @param[in] mbp       the pointer to an initialized Mailbox object
//  109  * @param[in] msg       the message to be posted on the mailbox
//  110  * @param[in] time      the number of ticks before the operation timeouts,
//  111  *                      the following special values are allowed:
//  112  *                      - @a TIME_IMMEDIATE immediate timeout.
//  113  *                      - @a TIME_INFINITE no timeout.
//  114  *                      .
//  115  * @return              The operation status.
//  116  * @retval RDY_OK       if a message has been correctly posted.
//  117  * @retval RDY_RESET    if the mailbox has been reset while waiting.
//  118  * @retval RDY_TIMEOUT  if the operation has timed out.
//  119  *
//  120  * @api
//  121  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function chMBPost
        THUMB
//  122 msg_t chMBPost(Mailbox *mbp, msg_t msg, systime_t time) {
chMBPost:
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
//  123   msg_t rdymsg;
//  124 
//  125   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  126   rdymsg = chMBPostS(mbp, msg, time);
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall chMBPostS
        BL       chMBPostS
        MOVS     R7,R0
//  127   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  128   return rdymsg;
        MOVS     R0,R7
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock2
//  129 }
//  130 
//  131 /**
//  132  * @brief   Posts a message into a mailbox.
//  133  * @details The invoking thread waits until a empty slot in the mailbox becomes
//  134  *          available or the specified time runs out.
//  135  *
//  136  * @param[in] mbp       the pointer to an initialized Mailbox object
//  137  * @param[in] msg       the message to be posted on the mailbox
//  138  * @param[in] time      the number of ticks before the operation timeouts,
//  139  *                      the following special values are allowed:
//  140  *                      - @a TIME_IMMEDIATE immediate timeout.
//  141  *                      - @a TIME_INFINITE no timeout.
//  142  *                      .
//  143  * @return              The operation status.
//  144  * @retval RDY_OK       if a message has been correctly posted.
//  145  * @retval RDY_RESET    if the mailbox has been reset while waiting.
//  146  * @retval RDY_TIMEOUT  if the operation has timed out.
//  147  *
//  148  * @sclass
//  149  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function chMBPostS
        THUMB
//  150 msg_t chMBPostS(Mailbox *mbp, msg_t msg, systime_t time) {
chMBPostS:
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
//  151   msg_t rdymsg;
//  152 
//  153   chDbgCheckClassS();
          CFI FunCall chDbgCheckClassS
        BL       chDbgCheckClassS
//  154   chDbgCheck(mbp != NULL, "chMBPostS");
        CMP      R4,#+0
        BNE.N    ??chMBPostS_0
        MOVS     R2,#+154
        LDR.N    R1,??DataTable7
        LDR.N    R0,??DataTable7_3
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  155 
//  156   rdymsg = chSemWaitTimeoutS(&mbp->mb_emptysem, time);
??chMBPostS_0:
        MOVS     R1,R6
        ADDS     R0,R4,#+28
          CFI FunCall chSemWaitTimeoutS
        BL       chSemWaitTimeoutS
        MOVS     R7,R0
//  157   if (rdymsg == RDY_OK) {
        CMP      R7,#+0
        BNE.N    ??chMBPostS_1
//  158     *mbp->mb_wrptr++ = msg;
        LDR      R0,[R4, #+8]
        ADDS     R1,R0,#+4
        STR      R1,[R4, #+8]
        STR      R5,[R0, #+0]
//  159     if (mbp->mb_wrptr >= mbp->mb_top)
        LDR      R0,[R4, #+8]
        LDR      R1,[R4, #+4]
        CMP      R0,R1
        BCC.N    ??chMBPostS_2
//  160       mbp->mb_wrptr = mbp->mb_buffer;
        LDR      R0,[R4, #+0]
        STR      R0,[R4, #+8]
//  161     chSemSignalI(&mbp->mb_fullsem);
??chMBPostS_2:
        ADDS     R0,R4,#+16
          CFI FunCall chSemSignalI
        BL       chSemSignalI
//  162     chSchRescheduleS();
          CFI FunCall chSchRescheduleS
        BL       chSchRescheduleS
//  163   }
//  164   return rdymsg;
??chMBPostS_1:
        MOVS     R0,R7
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock3
//  165 }
//  166 
//  167 /**
//  168  * @brief   Posts a message into a mailbox.
//  169  * @details This variant is non-blocking, the function returns a timeout
//  170  *          condition if the queue is full.
//  171  *
//  172  * @param[in] mbp       the pointer to an initialized Mailbox object
//  173  * @param[in] msg       the message to be posted on the mailbox
//  174  * @return              The operation status.
//  175  * @retval RDY_OK       if a message has been correctly posted.
//  176  * @retval RDY_TIMEOUT  if the mailbox is full and the message cannot be
//  177  *                      posted.
//  178  *
//  179  * @iclass
//  180  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function chMBPostI
        THUMB
//  181 msg_t chMBPostI(Mailbox *mbp, msg_t msg) {
chMBPostI:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  182 
//  183   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  184   chDbgCheck(mbp != NULL, "chMBPostI");
        CMP      R4,#+0
        BNE.N    ??chMBPostI_0
        MOVS     R2,#+184
        LDR.N    R1,??DataTable7
        LDR.N    R0,??DataTable7_4
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  185 
//  186   if (chSemGetCounterI(&mbp->mb_emptysem) <= 0)
??chMBPostI_0:
        LDR      R0,[R4, #+36]
        CMP      R0,#+1
        BGE.N    ??chMBPostI_1
//  187     return RDY_TIMEOUT;
        MOVS     R0,#-1
        B.N      ??chMBPostI_2
//  188   chSemFastWaitI(&mbp->mb_emptysem);
??chMBPostI_1:
        LDR      R0,[R4, #+36]
        SUBS     R0,R0,#+1
        STR      R0,[R4, #+36]
//  189   *mbp->mb_wrptr++ = msg;
        LDR      R0,[R4, #+8]
        ADDS     R1,R0,#+4
        STR      R1,[R4, #+8]
        STR      R5,[R0, #+0]
//  190   if (mbp->mb_wrptr >= mbp->mb_top)
        LDR      R0,[R4, #+8]
        LDR      R1,[R4, #+4]
        CMP      R0,R1
        BCC.N    ??chMBPostI_3
//  191     mbp->mb_wrptr = mbp->mb_buffer;
        LDR      R0,[R4, #+0]
        STR      R0,[R4, #+8]
//  192   chSemSignalI(&mbp->mb_fullsem);
??chMBPostI_3:
        ADDS     R0,R4,#+16
          CFI FunCall chSemSignalI
        BL       chSemSignalI
//  193   return RDY_OK;
        MOVS     R0,#+0
??chMBPostI_2:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock4
//  194 }
//  195 
//  196 /**
//  197  * @brief   Posts an high priority message into a mailbox.
//  198  * @details The invoking thread waits until a empty slot in the mailbox becomes
//  199  *          available or the specified time runs out.
//  200  *
//  201  * @param[in] mbp       the pointer to an initialized Mailbox object
//  202  * @param[in] msg       the message to be posted on the mailbox
//  203  * @param[in] time      the number of ticks before the operation timeouts,
//  204  *                      the following special values are allowed:
//  205  *                      - @a TIME_IMMEDIATE immediate timeout.
//  206  *                      - @a TIME_INFINITE no timeout.
//  207  *                      .
//  208  * @return              The operation status.
//  209  * @retval RDY_OK       if a message has been correctly posted.
//  210  * @retval RDY_RESET    if the mailbox has been reset while waiting.
//  211  * @retval RDY_TIMEOUT  if the operation has timed out.
//  212  *
//  213  * @api
//  214  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function chMBPostAhead
        THUMB
//  215 msg_t chMBPostAhead(Mailbox *mbp, msg_t msg, systime_t time) {
chMBPostAhead:
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
//  216   msg_t rdymsg;
//  217 
//  218   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  219   rdymsg = chMBPostAheadS(mbp, msg, time);
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall chMBPostAheadS
        BL       chMBPostAheadS
        MOVS     R7,R0
//  220   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  221   return rdymsg;
        MOVS     R0,R7
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock5
//  222 }
//  223 
//  224 /**
//  225  * @brief   Posts an high priority message into a mailbox.
//  226  * @details The invoking thread waits until a empty slot in the mailbox becomes
//  227  *          available or the specified time runs out.
//  228  *
//  229  * @param[in] mbp       the pointer to an initialized Mailbox object
//  230  * @param[in] msg       the message to be posted on the mailbox
//  231  * @param[in] time      the number of ticks before the operation timeouts,
//  232  *                      the following special values are allowed:
//  233  *                      - @a TIME_IMMEDIATE immediate timeout.
//  234  *                      - @a TIME_INFINITE no timeout.
//  235  *                      .
//  236  * @return              The operation status.
//  237  * @retval RDY_OK       if a message has been correctly posted.
//  238  * @retval RDY_RESET    if the mailbox has been reset while waiting.
//  239  * @retval RDY_TIMEOUT  if the operation has timed out.
//  240  *
//  241  * @sclass
//  242  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function chMBPostAheadS
        THUMB
//  243 msg_t chMBPostAheadS(Mailbox *mbp, msg_t msg, systime_t time) {
chMBPostAheadS:
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
//  244   msg_t rdymsg;
//  245 
//  246   chDbgCheckClassS();
          CFI FunCall chDbgCheckClassS
        BL       chDbgCheckClassS
//  247   chDbgCheck(mbp != NULL, "chMBPostAheadS");
        CMP      R4,#+0
        BNE.N    ??chMBPostAheadS_0
        MOVS     R2,#+247
        LDR.N    R1,??DataTable7
        LDR.N    R0,??DataTable7_5
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  248 
//  249   rdymsg = chSemWaitTimeoutS(&mbp->mb_emptysem, time);
??chMBPostAheadS_0:
        MOVS     R1,R6
        ADDS     R0,R4,#+28
          CFI FunCall chSemWaitTimeoutS
        BL       chSemWaitTimeoutS
        MOVS     R7,R0
//  250   if (rdymsg == RDY_OK) {
        CMP      R7,#+0
        BNE.N    ??chMBPostAheadS_1
//  251     if (--mbp->mb_rdptr < mbp->mb_buffer)
        LDR      R0,[R4, #+12]
        SUBS     R0,R0,#+4
        STR      R0,[R4, #+12]
        LDR      R1,[R4, #+0]
        CMP      R0,R1
        BCS.N    ??chMBPostAheadS_2
//  252       mbp->mb_rdptr = mbp->mb_top - 1;
        LDR      R0,[R4, #+4]
        SUBS     R0,R0,#+4
        STR      R0,[R4, #+12]
//  253     *mbp->mb_rdptr = msg;
??chMBPostAheadS_2:
        LDR      R0,[R4, #+12]
        STR      R5,[R0, #+0]
//  254     chSemSignalI(&mbp->mb_fullsem);
        ADDS     R0,R4,#+16
          CFI FunCall chSemSignalI
        BL       chSemSignalI
//  255     chSchRescheduleS();
          CFI FunCall chSchRescheduleS
        BL       chSchRescheduleS
//  256   }
//  257   return rdymsg;
??chMBPostAheadS_1:
        MOVS     R0,R7
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock6
//  258 }
//  259 
//  260 /**
//  261  * @brief   Posts an high priority message into a mailbox.
//  262  * @details This variant is non-blocking, the function returns a timeout
//  263  *          condition if the queue is full.
//  264  *
//  265  * @param[in] mbp       the pointer to an initialized Mailbox object
//  266  * @param[in] msg       the message to be posted on the mailbox
//  267  * @return              The operation status.
//  268  * @retval RDY_OK       if a message has been correctly posted.
//  269  * @retval RDY_TIMEOUT  if the mailbox is full and the message cannot be
//  270  *                      posted.
//  271  *
//  272  * @iclass
//  273  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function chMBPostAheadI
        THUMB
//  274 msg_t chMBPostAheadI(Mailbox *mbp, msg_t msg) {
chMBPostAheadI:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  275 
//  276   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  277   chDbgCheck(mbp != NULL, "chMBPostAheadI");
        CMP      R4,#+0
        BNE.N    ??chMBPostAheadI_0
        MOVW     R2,#+277
        LDR.N    R1,??DataTable7
        LDR.N    R0,??DataTable7_6
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  278 
//  279   if (chSemGetCounterI(&mbp->mb_emptysem) <= 0)
??chMBPostAheadI_0:
        LDR      R0,[R4, #+36]
        CMP      R0,#+1
        BGE.N    ??chMBPostAheadI_1
//  280     return RDY_TIMEOUT;
        MOVS     R0,#-1
        B.N      ??chMBPostAheadI_2
//  281   chSemFastWaitI(&mbp->mb_emptysem);
??chMBPostAheadI_1:
        LDR      R0,[R4, #+36]
        SUBS     R0,R0,#+1
        STR      R0,[R4, #+36]
//  282   if (--mbp->mb_rdptr < mbp->mb_buffer)
        LDR      R0,[R4, #+12]
        SUBS     R0,R0,#+4
        STR      R0,[R4, #+12]
        LDR      R1,[R4, #+0]
        CMP      R0,R1
        BCS.N    ??chMBPostAheadI_3
//  283     mbp->mb_rdptr = mbp->mb_top - 1;
        LDR      R0,[R4, #+4]
        SUBS     R0,R0,#+4
        STR      R0,[R4, #+12]
//  284   *mbp->mb_rdptr = msg;
??chMBPostAheadI_3:
        LDR      R0,[R4, #+12]
        STR      R5,[R0, #+0]
//  285   chSemSignalI(&mbp->mb_fullsem);
        ADDS     R0,R4,#+16
          CFI FunCall chSemSignalI
        BL       chSemSignalI
//  286   return RDY_OK;
        MOVS     R0,#+0
??chMBPostAheadI_2:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock7
//  287 }
//  288 
//  289 /**
//  290  * @brief   Retrieves a message from a mailbox.
//  291  * @details The invoking thread waits until a message is posted in the mailbox
//  292  *          or the specified time runs out.
//  293  *
//  294  * @param[in] mbp       the pointer to an initialized Mailbox object
//  295  * @param[out] msgp     pointer to a message variable for the received message
//  296  * @param[in] time      the number of ticks before the operation timeouts,
//  297  *                      the following special values are allowed:
//  298  *                      - @a TIME_IMMEDIATE immediate timeout.
//  299  *                      - @a TIME_INFINITE no timeout.
//  300  *                      .
//  301  * @return              The operation status.
//  302  * @retval RDY_OK       if a message has been correctly fetched.
//  303  * @retval RDY_RESET    if the mailbox has been reset while waiting.
//  304  * @retval RDY_TIMEOUT  if the operation has timed out.
//  305  *
//  306  * @api
//  307  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function chMBFetch
        THUMB
//  308 msg_t chMBFetch(Mailbox *mbp, msg_t *msgp, systime_t time) {
chMBFetch:
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
//  309   msg_t rdymsg;
//  310 
//  311   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  312   rdymsg = chMBFetchS(mbp, msgp, time);
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall chMBFetchS
        BL       chMBFetchS
        MOVS     R7,R0
//  313   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  314   return rdymsg;
        MOVS     R0,R7
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock8
//  315 }
//  316 
//  317 /**
//  318  * @brief   Retrieves a message from a mailbox.
//  319  * @details The invoking thread waits until a message is posted in the mailbox
//  320  *          or the specified time runs out.
//  321  *
//  322  * @param[in] mbp       the pointer to an initialized Mailbox object
//  323  * @param[out] msgp     pointer to a message variable for the received message
//  324  * @param[in] time      the number of ticks before the operation timeouts,
//  325  *                      the following special values are allowed:
//  326  *                      - @a TIME_IMMEDIATE immediate timeout.
//  327  *                      - @a TIME_INFINITE no timeout.
//  328  *                      .
//  329  * @return              The operation status.
//  330  * @retval RDY_OK       if a message has been correctly fetched.
//  331  * @retval RDY_RESET    if the mailbox has been reset while waiting.
//  332  * @retval RDY_TIMEOUT  if the operation has timed out.
//  333  *
//  334  * @sclass
//  335  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function chMBFetchS
        THUMB
//  336 msg_t chMBFetchS(Mailbox *mbp, msg_t *msgp, systime_t time) {
chMBFetchS:
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
//  337   msg_t rdymsg;
//  338 
//  339   chDbgCheckClassS();
          CFI FunCall chDbgCheckClassS
        BL       chDbgCheckClassS
//  340   chDbgCheck((mbp != NULL) && (msgp != NULL), "chMBFetchS");
        CMP      R4,#+0
        BEQ.N    ??chMBFetchS_0
        CMP      R5,#+0
        BNE.N    ??chMBFetchS_1
??chMBFetchS_0:
        MOV      R2,#+340
        LDR.N    R1,??DataTable7
        LDR.N    R0,??DataTable7_7
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  341 
//  342   rdymsg = chSemWaitTimeoutS(&mbp->mb_fullsem, time);
??chMBFetchS_1:
        MOVS     R1,R6
        ADDS     R0,R4,#+16
          CFI FunCall chSemWaitTimeoutS
        BL       chSemWaitTimeoutS
        MOVS     R7,R0
//  343   if (rdymsg == RDY_OK) {
        CMP      R7,#+0
        BNE.N    ??chMBFetchS_2
//  344     *msgp = *mbp->mb_rdptr++;
        LDR      R0,[R4, #+12]
        ADDS     R1,R0,#+4
        STR      R1,[R4, #+12]
        LDR      R0,[R0, #+0]
        STR      R0,[R5, #+0]
//  345     if (mbp->mb_rdptr >= mbp->mb_top)
        LDR      R0,[R4, #+12]
        LDR      R1,[R4, #+4]
        CMP      R0,R1
        BCC.N    ??chMBFetchS_3
//  346       mbp->mb_rdptr = mbp->mb_buffer;
        LDR      R0,[R4, #+0]
        STR      R0,[R4, #+12]
//  347     chSemSignalI(&mbp->mb_emptysem);
??chMBFetchS_3:
        ADDS     R0,R4,#+28
          CFI FunCall chSemSignalI
        BL       chSemSignalI
//  348     chSchRescheduleS();
          CFI FunCall chSchRescheduleS
        BL       chSchRescheduleS
//  349   }
//  350   return rdymsg;
??chMBFetchS_2:
        MOVS     R0,R7
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock9
//  351 }
//  352 
//  353 /**
//  354  * @brief   Retrieves a message from a mailbox.
//  355  * @details This variant is non-blocking, the function returns a timeout
//  356  *          condition if the queue is empty.
//  357  *
//  358  * @param[in] mbp       the pointer to an initialized Mailbox object
//  359  * @param[out] msgp     pointer to a message variable for the received message
//  360  * @return              The operation status.
//  361  * @retval RDY_OK       if a message has been correctly fetched.
//  362  * @retval RDY_TIMEOUT  if the mailbox is empty and a message cannot be
//  363  *                      fetched.
//  364  *
//  365  * @iclass
//  366  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function chMBFetchI
        THUMB
//  367 msg_t chMBFetchI(Mailbox *mbp, msg_t *msgp) {
chMBFetchI:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  368 
//  369   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  370   chDbgCheck((mbp != NULL) && (msgp != NULL), "chMBFetchI");
        CMP      R4,#+0
        BEQ.N    ??chMBFetchI_0
        CMP      R5,#+0
        BNE.N    ??chMBFetchI_1
??chMBFetchI_0:
        MOV      R2,#+370
        LDR.N    R1,??DataTable7
        LDR.N    R0,??DataTable7_8
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  371 
//  372   if (chSemGetCounterI(&mbp->mb_fullsem) <= 0)
??chMBFetchI_1:
        LDR      R0,[R4, #+24]
        CMP      R0,#+1
        BGE.N    ??chMBFetchI_2
//  373     return RDY_TIMEOUT;
        MOVS     R0,#-1
        B.N      ??chMBFetchI_3
//  374   chSemFastWaitI(&mbp->mb_fullsem);
??chMBFetchI_2:
        LDR      R0,[R4, #+24]
        SUBS     R0,R0,#+1
        STR      R0,[R4, #+24]
//  375   *msgp = *mbp->mb_rdptr++;
        LDR      R0,[R4, #+12]
        ADDS     R1,R0,#+4
        STR      R1,[R4, #+12]
        LDR      R0,[R0, #+0]
        STR      R0,[R5, #+0]
//  376   if (mbp->mb_rdptr >= mbp->mb_top)
        LDR      R0,[R4, #+12]
        LDR      R1,[R4, #+4]
        CMP      R0,R1
        BCC.N    ??chMBFetchI_4
//  377     mbp->mb_rdptr = mbp->mb_buffer;
        LDR      R0,[R4, #+0]
        STR      R0,[R4, #+12]
//  378   chSemSignalI(&mbp->mb_emptysem);
??chMBFetchI_4:
        ADDS     R0,R4,#+28
          CFI FunCall chSemSignalI
        BL       chSemSignalI
//  379   return RDY_OK;
        MOVS     R0,#+0
??chMBFetchI_3:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock10
//  380 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_1:
        DC32     `?<Constant "\\"chMBInit\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_2:
        DC32     `?<Constant "\\"chMBReset\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_3:
        DC32     `?<Constant "\\"chMBPostS\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_4:
        DC32     `?<Constant "\\"chMBPostI\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_5:
        DC32     `?<Constant "\\"chMBPostAheadS\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_6:
        DC32     `?<Constant "\\"chMBPostAheadI\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_7:
        DC32     `?<Constant "\\"chMBFetchS\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_8:
        DC32     `?<Constant "\\"chMBFetchI\\"()">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chMBInit\\"()">`:
        DATA
        DC8 "\"chMBInit\"()"
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
        DC8 72H, 63H, 5CH, 63H, 68H, 6DH, 62H, 6FH
        DC8 78H, 65H, 73H, 2EH, 63H, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chMBReset\\"()">`:
        DATA
        DC8 "\"chMBReset\"()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chMBPostS\\"()">`:
        DATA
        DC8 "\"chMBPostS\"()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chMBPostI\\"()">`:
        DATA
        DC8 "\"chMBPostI\"()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chMBPostAheadS\\"()">`:
        DATA
        DC8 "\"chMBPostAheadS\"()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chMBPostAheadI\\"()">`:
        DATA
        DC8 "\"chMBPostAheadI\"()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chMBFetchS\\"()">`:
        DATA
        DC8 "\"chMBFetchS\"()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chMBFetchI\\"()">`:
        DATA
        DC8 "\"chMBFetchI\"()"
        DC8 0

        END
//  381 #endif /* CH_USE_MAILBOXES */
//  382 
//  383 /** @} */
// 
// 208 bytes in section .rodata
// 786 bytes in section .text
// 
// 786 bytes of CODE  memory
// 208 bytes of CONST memory
//
//Errors: none
//Warnings: none
