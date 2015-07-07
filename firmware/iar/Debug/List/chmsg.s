///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:42 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chmsg.c                                       /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chmsg.c -lCN F:\stuff\rusefi_sourceforge\firm /
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
//                       chmsg.s                                              /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME chmsg

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN chDbgPanic
        EXTERN chDbgPanic3
        EXTERN chSchGoSleepS
        EXTERN chSchReadyI
        EXTERN chSchWakeupS
        EXTERN dbg_check_lock
        EXTERN dbg_check_unlock
        EXTERN rlist

        PUBLIC chMsgRelease
        PUBLIC chMsgSend
        PUBLIC chMsgWait
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\kernel\src\chmsg.c
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
//   29  * @file    chmsg.c
//   30  * @brief   Messages code.
//   31  *
//   32  * @addtogroup messages
//   33  * @details Synchronous inter-thread messages APIs and services.
//   34  *          <h2>Operation Mode</h2>
//   35  *          Synchronous messages are an easy to use and fast IPC mechanism,
//   36  *          threads can both act as message servers and/or message clients,
//   37  *          the mechanism allows data to be carried in both directions. Note
//   38  *          that messages are not copied between the client and server threads
//   39  *          but just a pointer passed so the exchange is very time
//   40  *          efficient.<br>
//   41  *          Messages are scalar data types of type @p msg_t that are guaranteed
//   42  *          to be size compatible with data pointers. Note that on some
//   43  *          architectures function pointers can be larger that @p msg_t.<br>
//   44  *          Messages are usually processed in FIFO order but it is possible to
//   45  *          process them in priority order by enabling the
//   46  *          @p CH_USE_MESSAGES_PRIORITY option in @p chconf.h.<br>
//   47  * @pre     In order to use the message APIs the @p CH_USE_MESSAGES option
//   48  *          must be enabled in @p chconf.h.
//   49  * @post    Enabling messages requires 6-12 (depending on the architecture)
//   50  *          extra bytes in the @p Thread structure.
//   51  * @{
//   52  */
//   53 
//   54 #include "ch.h"

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
//   55 
//   56 #if CH_USE_MESSAGES || defined(__DOXYGEN__)
//   57 
//   58 #if CH_USE_MESSAGES_PRIORITY
//   59 #define msg_insert(tp, qp) prio_insert(tp, qp)
//   60 #else
//   61 #define msg_insert(tp, qp) queue_insert(tp, qp)
//   62 #endif
//   63 
//   64 /**
//   65  * @brief   Sends a message to the specified thread.
//   66  * @details The sender is stopped until the receiver executes a
//   67  *          @p chMsgRelease()after receiving the message.
//   68  *
//   69  * @param[in] tp        the pointer to the thread
//   70  * @param[in] msg       the message
//   71  * @return              The answer message from @p chMsgRelease().
//   72  *
//   73  * @api
//   74  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function chMsgSend
        THUMB
//   75 msg_t chMsgSend(Thread *tp, msg_t msg) {
chMsgSend:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   76   Thread *ctp = currp;
        LDR.N    R0,??DataTable2
        LDR      R6,[R0, #+24]
//   77 
//   78   chDbgCheck(tp != NULL, "chMsgSend");
        CMP      R4,#+0
        BNE.N    ??chMsgSend_0
        MOVS     R2,#+78
        LDR.N    R1,??DataTable2_1
        LDR.N    R0,??DataTable2_2
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//   79 
//   80   chSysLock();
??chMsgSend_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//   81   ctp->p_msg = msg;
        STR      R5,[R6, #+52]
//   82   ctp->p_u.wtobjp = &tp->p_msgqueue;
        ADDS     R0,R4,#+44
        STR      R0,[R6, #+40]
//   83   msg_insert(ctp, &tp->p_msgqueue);
        ADDS     R1,R4,#+44
        MOVS     R0,R6
          CFI FunCall queue_insert
        BL       queue_insert
//   84   if (tp->p_state == THD_STATE_WTMSG)
        LDRB     R0,[R4, #+32]
        CMP      R0,#+12
        BNE.N    ??chMsgSend_1
//   85     chSchReadyI(tp);
        MOVS     R0,R4
          CFI FunCall chSchReadyI
        BL       chSchReadyI
//   86   chSchGoSleepS(THD_STATE_SNDMSGQ);
??chMsgSend_1:
        MOVS     R0,#+10
          CFI FunCall chSchGoSleepS
        BL       chSchGoSleepS
//   87   msg = ctp->p_u.rdymsg;
        LDR      R0,[R6, #+40]
        MOVS     R5,R0
//   88   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//   89   return msg;
        MOVS     R0,R5
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock2
//   90 }
//   91 
//   92 /**
//   93  * @brief   Suspends the thread and waits for an incoming message.
//   94  * @post    After receiving a message the function @p chMsgGet() must be
//   95  *          called in order to retrieve the message and then @p chMsgRelease()
//   96  *          must be invoked in order to acknowledge the reception and send
//   97  *          the answer.
//   98  * @note    If the message is a pointer then you can assume that the data
//   99  *          pointed by the message is stable until you invoke @p chMsgRelease()
//  100  *          because the sending thread is suspended until then.
//  101  *
//  102  * @return              A reference to the thread carrying the message.
//  103  *
//  104  * @api
//  105  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function chMsgWait
        THUMB
//  106 Thread *chMsgWait(void) {
chMsgWait:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  107   Thread *tp;
//  108 
//  109   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  110   if (!chMsgIsPendingI(currp))
        LDR.N    R0,??DataTable2
        LDR      R0,[R0, #+24]
        LDR      R0,[R0, #+44]
        LDR.N    R1,??DataTable2
        LDR      R1,[R1, #+24]
        ADDS     R1,R1,#+44
        CMP      R0,R1
        BNE.N    ??chMsgWait_0
//  111     chSchGoSleepS(THD_STATE_WTMSG);
        MOVS     R0,#+12
          CFI FunCall chSchGoSleepS
        BL       chSchGoSleepS
//  112   tp = fifo_remove(&currp->p_msgqueue);
??chMsgWait_0:
        LDR.N    R0,??DataTable2
        LDR      R0,[R0, #+24]
        ADDS     R0,R0,#+44
          CFI FunCall fifo_remove
        BL       fifo_remove
        MOVS     R4,R0
//  113   tp->p_state = THD_STATE_SNDMSG;
        MOVS     R0,#+11
        STRB     R0,[R4, #+32]
//  114   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  115   return tp;
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//  116 }
//  117 
//  118 /**
//  119  * @brief   Releases a sender thread specifying a response message.
//  120  * @pre     Invoke this function only after a message has been received
//  121  *          using @p chMsgWait().
//  122  *
//  123  * @param[in] tp        pointer to the thread
//  124  * @param[in] msg       message to be returned to the sender
//  125  *
//  126  * @api
//  127  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function chMsgRelease
        THUMB
//  128 void chMsgRelease(Thread *tp, msg_t msg) {
chMsgRelease:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  129 
//  130   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  131   chDbgAssert(tp->p_state == THD_STATE_SNDMSG,
//  132               "chMsgRelease(), #1", "invalid state");
        LDRB     R0,[R4, #+32]
        CMP      R0,#+11
        BEQ.N    ??chMsgRelease_0
        LDR.N    R0,??DataTable2_3
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  133   chMsgReleaseS(tp, msg);
??chMsgRelease_0:
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall chSchWakeupS
        BL       chSchWakeupS
//  134   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  135 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock4

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
        DC32     `?<Constant "\\"chMsgSend\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_3:
        DC32     `?<Constant "chMsgRelease(), #1">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chMsgSend\\"()">`:
        DATA
        DC8 "\"chMsgSend\"()"
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
        DC8 72H, 63H, 5CH, 63H, 68H, 6DH, 73H, 67H
        DC8 2EH, 63H, 0
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "chMsgRelease(), #1">`:
        DATA
        DC8 "chMsgRelease(), #1"
        DC8 0

        END
//  136 
//  137 #endif /* CH_USE_MESSAGES */
//  138 
//  139 /** @} */
// 
// 104 bytes in section .rodata
// 248 bytes in section .text
// 
// 248 bytes of CODE  memory
// 104 bytes of CONST memory
//
//Errors: none
//Warnings: none
