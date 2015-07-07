///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:17 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chevents.c                                    /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\kern /
//                       el\src\chevents.c -lCN F:\stuff\rusefi_sourceforge\f /
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
//                       chevents.s                                           /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME chevents

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN chDbgCheckClassI
        EXTERN chDbgPanic
        EXTERN chDbgPanic3
        EXTERN chSchGoSleepS
        EXTERN chSchGoSleepTimeoutS
        EXTERN chSchReadyI
        EXTERN chSchRescheduleS
        EXTERN dbg_check_lock
        EXTERN dbg_check_unlock
        EXTERN rlist

        PUBLIC chEvtAddEvents
        PUBLIC chEvtBroadcastFlags
        PUBLIC chEvtBroadcastFlagsI
        PUBLIC chEvtDispatch
        PUBLIC chEvtGetAndClearEvents
        PUBLIC chEvtGetAndClearFlags
        PUBLIC chEvtGetAndClearFlagsI
        PUBLIC chEvtRegisterMask
        PUBLIC chEvtSignal
        PUBLIC chEvtSignalI
        PUBLIC chEvtUnregister
        PUBLIC chEvtWaitAll
        PUBLIC chEvtWaitAllTimeout
        PUBLIC chEvtWaitAny
        PUBLIC chEvtWaitAnyTimeout
        PUBLIC chEvtWaitOne
        PUBLIC chEvtWaitOneTimeout
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\kernel\src\chevents.c
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
//   27 /*
//   28    Concepts and parts of this file have been contributed by Scott (skute).
//   29  */
//   30 
//   31 /**
//   32  * @file    chevents.c
//   33  * @brief   Events code.
//   34  *
//   35  * @addtogroup events
//   36  * @details Event Flags, Event Sources and Event Listeners.
//   37  *          <h2>Operation mode</h2>
//   38  *          Each thread has a mask of pending event flags inside its @p Thread
//   39  *          structure.
//   40  *          Operations defined for event flags:
//   41  *          - <b>Wait</b>, the invoking thread goes to sleep until a certain
//   42  *            AND/OR combination of event flags becomes pending.
//   43  *          - <b>Clear</b>, a mask of event flags is cleared from the pending
//   44  *            events mask, the cleared event flags mask is returned (only the
//   45  *            flags that were actually pending and then cleared).
//   46  *          - <b>Signal</b>, an event mask is directly ORed to the mask of the
//   47  *            signaled thread.
//   48  *          - <b>Broadcast</b>, each thread registered on an Event Source is
//   49  *            signaled with the event flags specified in its Event Listener.
//   50  *          - <b>Dispatch</b>, an events mask is scanned and for each bit set
//   51  *            to one an associated handler function is invoked. Bit masks are
//   52  *            scanned from bit zero upward.
//   53  *          .
//   54  *          An Event Source is a special object that can be "broadcasted" by
//   55  *          a thread or an interrupt service routine. Broadcasting an Event
//   56  *          Source has the effect that all the threads registered on the
//   57  *          Event Source will be signaled with an events mask.<br>
//   58  *          An unlimited number of Event Sources can exists in a system and
//   59  *          each thread can be listening on an unlimited number of
//   60  *          them.
//   61  * @pre     In order to use the Events APIs the @p CH_USE_EVENTS option must be
//   62  *          enabled in @p chconf.h.
//   63  * @post    Enabling events requires 1-4 (depending on the architecture)
//   64  *          extra bytes in the @p Thread structure.
//   65  * @{
//   66  */
//   67 
//   68 #include "ch.h"
//   69 
//   70 #if CH_USE_EVENTS || defined(__DOXYGEN__)
//   71 /**
//   72  * @brief   Registers an Event Listener on an Event Source.
//   73  * @details Once a thread has registered as listener on an event source it
//   74  *          will be notified of all events broadcasted there.
//   75  * @note    Multiple Event Listeners can specify the same bits to be ORed to
//   76  *          different threads.
//   77  *
//   78  * @param[in] esp       pointer to the  @p EventSource structure
//   79  * @param[out] elp      pointer to the @p EventListener structure
//   80  * @param[in] mask      the mask of event flags to be ORed to the thread when
//   81  *                      the event source is broadcasted
//   82  *
//   83  * @api
//   84  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function chEvtRegisterMask
        THUMB
//   85 void chEvtRegisterMask(EventSource *esp, EventListener *elp, eventmask_t mask) {
chEvtRegisterMask:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   86 
//   87   chDbgCheck((esp != NULL) && (elp != NULL), "chEvtRegisterMask");
        CMP      R4,#+0
        BEQ.N    ??chEvtRegisterMask_0
        CMP      R5,#+0
        BNE.N    ??chEvtRegisterMask_1
??chEvtRegisterMask_0:
        MOVS     R2,#+87
        LDR.W    R1,??DataTable13
        LDR.W    R0,??DataTable13_1
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//   88 
//   89   chSysLock();
??chEvtRegisterMask_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//   90   elp->el_next     = esp->es_next;
        LDR      R0,[R4, #+0]
        STR      R0,[R5, #+0]
//   91   esp->es_next     = elp;
        STR      R5,[R4, #+0]
//   92   elp->el_listener = currp;
        LDR.W    R0,??DataTable13_2
        LDR      R0,[R0, #+24]
        STR      R0,[R5, #+4]
//   93   elp->el_mask     = mask;
        STR      R6,[R5, #+8]
//   94   elp->el_flags    = 0;
        MOVS     R0,#+0
        STR      R0,[R5, #+12]
//   95   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//   96 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock0
//   97 
//   98 /**
//   99  * @brief   Unregisters an Event Listener from its Event Source.
//  100  * @note    If the event listener is not registered on the specified event
//  101  *          source then the function does nothing.
//  102  * @note    For optimal performance it is better to perform the unregister
//  103  *          operations in inverse order of the register operations (elements
//  104  *          are found on top of the list).
//  105  *
//  106  * @param[in] esp       pointer to the  @p EventSource structure
//  107  * @param[in] elp       pointer to the @p EventListener structure
//  108  *
//  109  * @api
//  110  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function chEvtUnregister
        THUMB
//  111 void chEvtUnregister(EventSource *esp, EventListener *elp) {
chEvtUnregister:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  112   EventListener *p;
//  113 
//  114   chDbgCheck((esp != NULL) && (elp != NULL), "chEvtUnregister");
        CMP      R4,#+0
        BEQ.N    ??chEvtUnregister_0
        CMP      R5,#+0
        BNE.N    ??chEvtUnregister_1
??chEvtUnregister_0:
        MOVS     R2,#+114
        LDR.W    R1,??DataTable13
        LDR.W    R0,??DataTable13_3
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  115 
//  116   p = (EventListener *)esp;
??chEvtUnregister_1:
        MOVS     R6,R4
//  117   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  118   while (p->el_next != (EventListener *)esp) {
??chEvtUnregister_2:
        LDR      R0,[R6, #+0]
        CMP      R0,R4
        BEQ.N    ??chEvtUnregister_3
//  119     if (p->el_next == elp) {
        LDR      R0,[R6, #+0]
        CMP      R0,R5
        BNE.N    ??chEvtUnregister_4
//  120       p->el_next = elp->el_next;
        LDR      R0,[R5, #+0]
        STR      R0,[R6, #+0]
//  121       break;
        B.N      ??chEvtUnregister_3
//  122     }
//  123     p = p->el_next;
??chEvtUnregister_4:
        LDR      R6,[R6, #+0]
        B.N      ??chEvtUnregister_2
//  124   }
//  125   chSysUnlock();
??chEvtUnregister_3:
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  126 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock1
//  127 
//  128 /**
//  129  * @brief   Clears the pending events specified in the mask.
//  130  *
//  131  * @param[in] mask      the events to be cleared
//  132  * @return              The pending events that were cleared.
//  133  *
//  134  * @api
//  135  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function chEvtGetAndClearEvents
        THUMB
//  136 eventmask_t chEvtGetAndClearEvents(eventmask_t mask) {
chEvtGetAndClearEvents:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  137   eventmask_t m;
//  138 
//  139   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  140 
//  141   m = currp->p_epending & mask;
        LDR.W    R0,??DataTable13_2
        LDR      R0,[R0, #+24]
        LDR      R0,[R0, #+56]
        ANDS     R0,R4,R0
        MOVS     R5,R0
//  142   currp->p_epending &= ~mask;
        LDR.W    R0,??DataTable13_2
        LDR      R0,[R0, #+24]
        LDR      R0,[R0, #+56]
        BICS     R0,R0,R4
        LDR.W    R1,??DataTable13_2
        LDR      R1,[R1, #+24]
        STR      R0,[R1, #+56]
//  143 
//  144   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  145   return m;
        MOVS     R0,R5
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2
//  146 }
//  147 
//  148 /**
//  149  * @brief   Adds (OR) a set of event flags on the current thread, this is
//  150  *          @b much faster than using @p chEvtBroadcast() or @p chEvtSignal().
//  151  *
//  152  * @param[in] mask      the event flags to be added
//  153  * @return              The current pending events mask.
//  154  *
//  155  * @api
//  156  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function chEvtAddEvents
        THUMB
//  157 eventmask_t chEvtAddEvents(eventmask_t mask) {
chEvtAddEvents:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  158 
//  159   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  160 
//  161   mask = (currp->p_epending |= mask);
        LDR.N    R0,??DataTable13_2
        LDR      R0,[R0, #+24]
        LDR      R0,[R0, #+56]
        ORRS     R4,R4,R0
        LDR.N    R0,??DataTable13_2
        LDR      R0,[R0, #+24]
        STR      R4,[R0, #+56]
//  162 
//  163   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  164   return mask;
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//  165 }
//  166 
//  167 /**
//  168  * @brief   Signals all the Event Listeners registered on the specified Event
//  169  *          Source.
//  170  * @details This function variants ORs the specified event flags to all the
//  171  *          threads registered on the @p EventSource in addition to the event
//  172  *          flags specified by the threads themselves in the
//  173  *          @p EventListener objects.
//  174  * @post    This function does not reschedule so a call to a rescheduling
//  175  *          function must be performed before unlocking the kernel. Note that
//  176  *          interrupt handlers always reschedule on exit so an explicit
//  177  *          reschedule must not be performed in ISRs.
//  178  *
//  179  * @param[in] esp       pointer to the @p EventSource structure
//  180  * @param[in] flags     the flags set to be added to the listener flags mask
//  181  *
//  182  * @iclass
//  183  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function chEvtBroadcastFlagsI
        THUMB
//  184 void chEvtBroadcastFlagsI(EventSource *esp, flagsmask_t flags) {
chEvtBroadcastFlagsI:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  185   EventListener *elp;
//  186 
//  187   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  188   chDbgCheck(esp != NULL, "chEvtBroadcastMaskI");
        CMP      R4,#+0
        BNE.N    ??chEvtBroadcastFlagsI_0
        MOVS     R2,#+188
        LDR.N    R1,??DataTable13
        LDR.N    R0,??DataTable13_4
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  189 
//  190   elp = esp->es_next;
??chEvtBroadcastFlagsI_0:
        LDR      R0,[R4, #+0]
        MOVS     R6,R0
//  191   while (elp != (EventListener *)esp) {
??chEvtBroadcastFlagsI_1:
        CMP      R6,R4
        BEQ.N    ??chEvtBroadcastFlagsI_2
//  192     elp->el_flags |= flags;
        LDR      R0,[R6, #+12]
        ORRS     R0,R5,R0
        STR      R0,[R6, #+12]
//  193     chEvtSignalI(elp->el_listener, elp->el_mask);
        LDR      R1,[R6, #+8]
        LDR      R0,[R6, #+4]
          CFI FunCall chEvtSignalI
        BL       chEvtSignalI
//  194     elp = elp->el_next;
        LDR      R6,[R6, #+0]
        B.N      ??chEvtBroadcastFlagsI_1
//  195   }
//  196 }
??chEvtBroadcastFlagsI_2:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock4
//  197 
//  198 /**
//  199  * @brief   Returns the flags associated to an @p EventListener.
//  200  * @details The flags are returned and the @p EventListener flags mask is
//  201  *          cleared.
//  202  *
//  203  * @param[in] elp       pointer to the @p EventListener structure
//  204  * @return              The flags added to the listener by the associated
//  205  *                      event source.
//  206  *
//  207  * @api
//  208  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function chEvtGetAndClearFlags
        THUMB
//  209 flagsmask_t chEvtGetAndClearFlags(EventListener *elp) {
chEvtGetAndClearFlags:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  210   flagsmask_t flags;
//  211 
//  212   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  213 
//  214   flags = elp->el_flags;
        LDR      R0,[R4, #+12]
        MOVS     R5,R0
//  215   elp->el_flags = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+12]
//  216 
//  217   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  218   return flags;
        MOVS     R0,R5
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock5
//  219 }
//  220 
//  221 /**
//  222  * @brief   Adds a set of event flags directly to specified @p Thread.
//  223  *
//  224  * @param[in] tp        the thread to be signaled
//  225  * @param[in] mask      the event flags set to be ORed
//  226  *
//  227  * @api
//  228  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function chEvtSignal
        THUMB
//  229 void chEvtSignal(Thread *tp, eventmask_t mask) {
chEvtSignal:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  230 
//  231   chDbgCheck(tp != NULL, "chEvtSignal");
        CMP      R4,#+0
        BNE.N    ??chEvtSignal_0
        MOVS     R2,#+231
        LDR.N    R1,??DataTable13
        LDR.N    R0,??DataTable13_5
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  232 
//  233   chSysLock();
??chEvtSignal_0:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  234   chEvtSignalI(tp, mask);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall chEvtSignalI
        BL       chEvtSignalI
//  235   chSchRescheduleS();
          CFI FunCall chSchRescheduleS
        BL       chSchRescheduleS
//  236   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  237 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock6
//  238 
//  239 /**
//  240  * @brief   Adds a set of event flags directly to specified @p Thread.
//  241  * @post    This function does not reschedule so a call to a rescheduling
//  242  *          function must be performed before unlocking the kernel. Note that
//  243  *          interrupt handlers always reschedule on exit so an explicit
//  244  *          reschedule must not be performed in ISRs.
//  245  *
//  246  * @param[in] tp        the thread to be signaled
//  247  * @param[in] mask      the event flags set to be ORed
//  248  *
//  249  * @iclass
//  250  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function chEvtSignalI
        THUMB
//  251 void chEvtSignalI(Thread *tp, eventmask_t mask) {
chEvtSignalI:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  252 
//  253   chDbgCheckClassI();
          CFI FunCall chDbgCheckClassI
        BL       chDbgCheckClassI
//  254   chDbgCheck(tp != NULL, "chEvtSignalI");
        CMP      R4,#+0
        BNE.N    ??chEvtSignalI_0
        MOVS     R2,#+254
        LDR.N    R1,??DataTable13
        LDR.N    R0,??DataTable13_6
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  255 
//  256   tp->p_epending |= mask;
??chEvtSignalI_0:
        LDR      R0,[R4, #+56]
        ORRS     R0,R5,R0
        STR      R0,[R4, #+56]
//  257   /* Test on the AND/OR conditions wait states.*/
//  258   if (((tp->p_state == THD_STATE_WTOREVT) &&
//  259        ((tp->p_epending & tp->p_u.ewmask) != 0)) ||
//  260       ((tp->p_state == THD_STATE_WTANDEVT) &&
//  261        ((tp->p_epending & tp->p_u.ewmask) == tp->p_u.ewmask)))
        LDRB     R0,[R4, #+32]
        CMP      R0,#+8
        BNE.N    ??chEvtSignalI_1
        LDR      R0,[R4, #+56]
        LDR      R1,[R4, #+40]
        TST      R0,R1
        BNE.N    ??chEvtSignalI_2
??chEvtSignalI_1:
        LDRB     R0,[R4, #+32]
        CMP      R0,#+9
        BNE.N    ??chEvtSignalI_3
        LDR      R0,[R4, #+56]
        LDR      R1,[R4, #+40]
        ANDS     R0,R1,R0
        LDR      R1,[R4, #+40]
        CMP      R0,R1
        BNE.N    ??chEvtSignalI_3
//  262     chSchReadyI(tp)->p_u.rdymsg = RDY_OK;
??chEvtSignalI_2:
        MOVS     R0,R4
          CFI FunCall chSchReadyI
        BL       chSchReadyI
        MOVS     R1,#+0
        STR      R1,[R0, #+40]
//  263 }
??chEvtSignalI_3:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock7
//  264 
//  265 /**
//  266  * @brief   Signals all the Event Listeners registered on the specified Event
//  267  *          Source.
//  268  * @details This function variants ORs the specified event flags to all the
//  269  *          threads registered on the @p EventSource in addition to the event
//  270  *          flags specified by the threads themselves in the
//  271  *          @p EventListener objects.
//  272  *
//  273  * @param[in] esp       pointer to the @p EventSource structure
//  274  * @param[in] flags     the flags set to be added to the listener flags mask
//  275  *
//  276  * @api
//  277  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function chEvtBroadcastFlags
        THUMB
//  278 void chEvtBroadcastFlags(EventSource *esp, flagsmask_t flags) {
chEvtBroadcastFlags:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  279 
//  280   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  281   chEvtBroadcastFlagsI(esp, flags);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall chEvtBroadcastFlagsI
        BL       chEvtBroadcastFlagsI
//  282   chSchRescheduleS();
          CFI FunCall chSchRescheduleS
        BL       chSchRescheduleS
//  283   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  284 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock8
//  285 
//  286 /**
//  287  * @brief   Returns the flags associated to an @p EventListener.
//  288  * @details The flags are returned and the @p EventListener flags mask is
//  289  *          cleared.
//  290  *
//  291  * @param[in] elp       pointer to the @p EventListener structure
//  292  * @return              The flags added to the listener by the associated
//  293  *                      event source.
//  294  *
//  295  * @iclass
//  296  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function chEvtGetAndClearFlagsI
          CFI NoCalls
        THUMB
//  297 flagsmask_t chEvtGetAndClearFlagsI(EventListener *elp) {
chEvtGetAndClearFlagsI:
        MOVS     R1,R0
//  298   flagsmask_t flags;
//  299 
//  300   flags = elp->el_flags;
        LDR      R2,[R1, #+12]
        MOVS     R0,R2
//  301   elp->el_flags = 0;
        MOVS     R2,#+0
        STR      R2,[R1, #+12]
//  302 
//  303   return flags;
        BX       LR               ;; return
          CFI EndBlock cfiBlock9
//  304 }
//  305 
//  306 /**
//  307  * @brief   Invokes the event handlers associated to an event flags mask.
//  308  *
//  309  * @param[in] mask      mask of the event flags to be dispatched
//  310  * @param[in] handlers  an array of @p evhandler_t. The array must have size
//  311  *                      equal to the number of bits in eventmask_t.
//  312  *
//  313  * @api
//  314  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function chEvtDispatch
        THUMB
//  315 void chEvtDispatch(const evhandler_t *handlers, eventmask_t mask) {
chEvtDispatch:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R6,R1
//  316   eventid_t eid;
//  317 
//  318   chDbgCheck(handlers != NULL, "chEvtDispatch");
        CMP      R4,#+0
        BNE.N    ??chEvtDispatch_0
        MOV      R2,#+318
        LDR.N    R1,??DataTable13
        LDR.N    R0,??DataTable13_7
          CFI FunCall chDbgPanic3
        BL       chDbgPanic3
//  319 
//  320   eid = 0;
??chEvtDispatch_0:
        MOVS     R0,#+0
        MOVS     R5,R0
//  321   while (mask) {
??chEvtDispatch_1:
        CMP      R6,#+0
        BEQ.N    ??chEvtDispatch_2
//  322     if (mask & EVENT_MASK(eid)) {
        LSRS     R0,R6,R5
        LSLS     R0,R0,#+31
        BPL.N    ??chEvtDispatch_3
//  323       chDbgAssert(handlers[eid] != NULL,
//  324                   "chEvtDispatch(), #1",
//  325                   "null handler");
        LDR      R0,[R4, R5, LSL #+2]
        CMP      R0,#+0
        BNE.N    ??chEvtDispatch_4
        LDR.N    R0,??DataTable13_8
          CFI FunCall chDbgPanic
        BL       chDbgPanic
//  326       mask &= ~EVENT_MASK(eid);
??chEvtDispatch_4:
        MOVS     R0,#+1
        LSLS     R0,R0,R5
        BICS     R6,R6,R0
//  327       handlers[eid](eid);
        MOVS     R0,R5
        LDR      R1,[R4, R5, LSL #+2]
          CFI FunCall
        BLX      R1
//  328     }
//  329     eid++;
??chEvtDispatch_3:
        ADDS     R5,R5,#+1
        B.N      ??chEvtDispatch_1
//  330   }
//  331 }
??chEvtDispatch_2:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock10
//  332 
//  333 #if CH_OPTIMIZE_SPEED || !CH_USE_EVENTS_TIMEOUT || defined(__DOXYGEN__)
//  334 /**
//  335  * @brief   Waits for exactly one of the specified events.
//  336  * @details The function waits for one event among those specified in
//  337  *          @p mask to become pending then the event is cleared and returned.
//  338  * @note    One and only one event is served in the function, the one with the
//  339  *          lowest event id. The function is meant to be invoked into a loop in
//  340  *          order to serve all the pending events.<br>
//  341  *          This means that Event Listeners with a lower event identifier have
//  342  *          an higher priority.
//  343  *
//  344  * @param[in] mask      mask of the event flags that the function should wait
//  345  *                      for, @p ALL_EVENTS enables all the events
//  346  * @return              The mask of the lowest id served and cleared event.
//  347  *
//  348  * @api
//  349  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function chEvtWaitOne
        THUMB
//  350 eventmask_t chEvtWaitOne(eventmask_t mask) {
chEvtWaitOne:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  351   Thread *ctp = currp;
        LDR.N    R0,??DataTable13_2
        LDR      R5,[R0, #+24]
//  352   eventmask_t m;
//  353 
//  354   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  355 
//  356   if ((m = (ctp->p_epending & mask)) == 0) {
        LDR      R0,[R5, #+56]
        ANDS     R0,R4,R0
        MOVS     R6,R0
        CMP      R0,#+0
        BNE.N    ??chEvtWaitOne_0
//  357     ctp->p_u.ewmask = mask;
        STR      R4,[R5, #+40]
//  358     chSchGoSleepS(THD_STATE_WTOREVT);
        MOVS     R0,#+8
          CFI FunCall chSchGoSleepS
        BL       chSchGoSleepS
//  359     m = ctp->p_epending & mask;
        LDR      R0,[R5, #+56]
        ANDS     R0,R4,R0
        MOVS     R6,R0
//  360   }
//  361   m ^= m & (m - 1);
??chEvtWaitOne_0:
        SUBS     R0,R6,#+1
        ANDS     R0,R0,R6
        EORS     R6,R0,R6
//  362   ctp->p_epending &= ~m;
        LDR      R0,[R5, #+56]
        BICS     R0,R0,R6
        STR      R0,[R5, #+56]
//  363 
//  364   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  365   return m;
        MOVS     R0,R6
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock11
//  366 }
//  367 
//  368 /**
//  369  * @brief   Waits for any of the specified events.
//  370  * @details The function waits for any event among those specified in
//  371  *          @p mask to become pending then the events are cleared and returned.
//  372  *
//  373  * @param[in] mask      mask of the event flags that the function should wait
//  374  *                      for, @p ALL_EVENTS enables all the events
//  375  * @return              The mask of the served and cleared events.
//  376  *
//  377  * @api
//  378  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function chEvtWaitAny
        THUMB
//  379 eventmask_t chEvtWaitAny(eventmask_t mask) {
chEvtWaitAny:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  380   Thread *ctp = currp;
        LDR.N    R0,??DataTable13_2
        LDR      R5,[R0, #+24]
//  381   eventmask_t m;
//  382 
//  383   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  384 
//  385   if ((m = (ctp->p_epending & mask)) == 0) {
        LDR      R0,[R5, #+56]
        ANDS     R0,R4,R0
        MOVS     R6,R0
        CMP      R0,#+0
        BNE.N    ??chEvtWaitAny_0
//  386     ctp->p_u.ewmask = mask;
        STR      R4,[R5, #+40]
//  387     chSchGoSleepS(THD_STATE_WTOREVT);
        MOVS     R0,#+8
          CFI FunCall chSchGoSleepS
        BL       chSchGoSleepS
//  388     m = ctp->p_epending & mask;
        LDR      R0,[R5, #+56]
        ANDS     R0,R4,R0
        MOVS     R6,R0
//  389   }
//  390   ctp->p_epending &= ~m;
??chEvtWaitAny_0:
        LDR      R0,[R5, #+56]
        BICS     R0,R0,R6
        STR      R0,[R5, #+56]
//  391 
//  392   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  393   return m;
        MOVS     R0,R6
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock12
//  394 }
//  395 
//  396 /**
//  397  * @brief   Waits for all the specified events.
//  398  * @details The function waits for all the events specified in @p mask to
//  399  *          become pending then the events are cleared and returned.
//  400  *
//  401  * @param[in] mask      mask of the event flags that the function should wait
//  402  *                      for, @p ALL_EVENTS requires all the events
//  403  * @return              The mask of the served and cleared events.
//  404  *
//  405  * @api
//  406  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function chEvtWaitAll
        THUMB
//  407 eventmask_t chEvtWaitAll(eventmask_t mask) {
chEvtWaitAll:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  408   Thread *ctp = currp;
        LDR.N    R0,??DataTable13_2
        LDR      R5,[R0, #+24]
//  409 
//  410   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  411 
//  412   if ((ctp->p_epending & mask) != mask) {
        LDR      R0,[R5, #+56]
        ANDS     R0,R4,R0
        CMP      R0,R4
        BEQ.N    ??chEvtWaitAll_0
//  413     ctp->p_u.ewmask = mask;
        STR      R4,[R5, #+40]
//  414     chSchGoSleepS(THD_STATE_WTANDEVT);
        MOVS     R0,#+9
          CFI FunCall chSchGoSleepS
        BL       chSchGoSleepS
//  415   }
//  416   ctp->p_epending &= ~mask;
??chEvtWaitAll_0:
        LDR      R0,[R5, #+56]
        BICS     R0,R0,R4
        STR      R0,[R5, #+56]
//  417 
//  418   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  419   return mask;
        MOVS     R0,R4
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock13
//  420 }
//  421 #endif /* CH_OPTIMIZE_SPEED || !CH_USE_EVENTS_TIMEOUT */
//  422 
//  423 #if CH_USE_EVENTS_TIMEOUT || defined(__DOXYGEN__)
//  424 /**
//  425  * @brief   Waits for exactly one of the specified events.
//  426  * @details The function waits for one event among those specified in
//  427  *          @p mask to become pending then the event is cleared and returned.
//  428  * @note    One and only one event is served in the function, the one with the
//  429  *          lowest event id. The function is meant to be invoked into a loop in
//  430  *          order to serve all the pending events.<br>
//  431  *          This means that Event Listeners with a lower event identifier have
//  432  *          an higher priority.
//  433  *
//  434  * @param[in] mask      mask of the event flags that the function should wait
//  435  *                      for, @p ALL_EVENTS enables all the events
//  436  * @param[in] time      the number of ticks before the operation timeouts,
//  437  *                      the following special values are allowed:
//  438  *                      - @a TIME_IMMEDIATE immediate timeout.
//  439  *                      - @a TIME_INFINITE no timeout.
//  440  *                      .
//  441  * @return              The mask of the lowest id served and cleared event.
//  442  * @retval 0            if the operation has timed out.
//  443  *
//  444  * @api
//  445  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function chEvtWaitOneTimeout
        THUMB
//  446 eventmask_t chEvtWaitOneTimeout(eventmask_t mask, systime_t time) {
chEvtWaitOneTimeout:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  447   Thread *ctp = currp;
        LDR.N    R0,??DataTable13_2
        LDR      R6,[R0, #+24]
//  448   eventmask_t m;
//  449 
//  450   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  451 
//  452   if ((m = (ctp->p_epending & mask)) == 0) {
        LDR      R0,[R6, #+56]
        ANDS     R0,R4,R0
        MOVS     R7,R0
        CMP      R0,#+0
        BNE.N    ??chEvtWaitOneTimeout_0
//  453     if (TIME_IMMEDIATE == time) {
        CMP      R5,#+0
        BNE.N    ??chEvtWaitOneTimeout_1
//  454       chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  455       return (eventmask_t)0;
        MOVS     R0,#+0
        B.N      ??chEvtWaitOneTimeout_2
//  456     }
//  457     ctp->p_u.ewmask = mask;
??chEvtWaitOneTimeout_1:
        STR      R4,[R6, #+40]
//  458     if (chSchGoSleepTimeoutS(THD_STATE_WTOREVT, time) < RDY_OK) {
        MOVS     R1,R5
        MOVS     R0,#+8
          CFI FunCall chSchGoSleepTimeoutS
        BL       chSchGoSleepTimeoutS
        CMP      R0,#+0
        BPL.N    ??chEvtWaitOneTimeout_3
//  459       chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  460       return (eventmask_t)0;
        MOVS     R0,#+0
        B.N      ??chEvtWaitOneTimeout_2
//  461     }
//  462     m = ctp->p_epending & mask;
??chEvtWaitOneTimeout_3:
        LDR      R0,[R6, #+56]
        ANDS     R0,R4,R0
        MOVS     R7,R0
//  463   }
//  464   m ^= m & (m - 1);
??chEvtWaitOneTimeout_0:
        SUBS     R0,R7,#+1
        ANDS     R0,R0,R7
        EORS     R7,R0,R7
//  465   ctp->p_epending &= ~m;
        LDR      R0,[R6, #+56]
        BICS     R0,R0,R7
        STR      R0,[R6, #+56]
//  466 
//  467   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  468   return m;
        MOVS     R0,R7
??chEvtWaitOneTimeout_2:
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock14
//  469 }
//  470 
//  471 /**
//  472  * @brief   Waits for any of the specified events.
//  473  * @details The function waits for any event among those specified in
//  474  *          @p mask to become pending then the events are cleared and
//  475  *          returned.
//  476  *
//  477  * @param[in] mask      mask of the event flags that the function should wait
//  478  *                      for, @p ALL_EVENTS enables all the events
//  479  * @param[in] time      the number of ticks before the operation timeouts,
//  480  *                      the following special values are allowed:
//  481  *                      - @a TIME_IMMEDIATE immediate timeout.
//  482  *                      - @a TIME_INFINITE no timeout.
//  483  *                      .
//  484  * @return              The mask of the served and cleared events.
//  485  * @retval 0            if the operation has timed out.
//  486  *
//  487  * @api
//  488  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function chEvtWaitAnyTimeout
        THUMB
//  489 eventmask_t chEvtWaitAnyTimeout(eventmask_t mask, systime_t time) {
chEvtWaitAnyTimeout:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  490   Thread *ctp = currp;
        LDR.N    R0,??DataTable13_2
        LDR      R6,[R0, #+24]
//  491   eventmask_t m;
//  492 
//  493   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  494 
//  495   if ((m = (ctp->p_epending & mask)) == 0) {
        LDR      R0,[R6, #+56]
        ANDS     R0,R4,R0
        MOVS     R7,R0
        CMP      R0,#+0
        BNE.N    ??chEvtWaitAnyTimeout_0
//  496     if (TIME_IMMEDIATE == time) {
        CMP      R5,#+0
        BNE.N    ??chEvtWaitAnyTimeout_1
//  497       chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  498       return (eventmask_t)0;
        MOVS     R0,#+0
        B.N      ??chEvtWaitAnyTimeout_2
//  499     }
//  500     ctp->p_u.ewmask = mask;
??chEvtWaitAnyTimeout_1:
        STR      R4,[R6, #+40]
//  501     if (chSchGoSleepTimeoutS(THD_STATE_WTOREVT, time) < RDY_OK) {
        MOVS     R1,R5
        MOVS     R0,#+8
          CFI FunCall chSchGoSleepTimeoutS
        BL       chSchGoSleepTimeoutS
        CMP      R0,#+0
        BPL.N    ??chEvtWaitAnyTimeout_3
//  502       chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  503       return (eventmask_t)0;
        MOVS     R0,#+0
        B.N      ??chEvtWaitAnyTimeout_2
//  504     }
//  505     m = ctp->p_epending & mask;
??chEvtWaitAnyTimeout_3:
        LDR      R0,[R6, #+56]
        ANDS     R0,R4,R0
        MOVS     R7,R0
//  506   }
//  507   ctp->p_epending &= ~m;
??chEvtWaitAnyTimeout_0:
        LDR      R0,[R6, #+56]
        BICS     R0,R0,R7
        STR      R0,[R6, #+56]
//  508 
//  509   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  510   return m;
        MOVS     R0,R7
??chEvtWaitAnyTimeout_2:
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock15
//  511 }
//  512 
//  513 /**
//  514  * @brief   Waits for all the specified events.
//  515  * @details The function waits for all the events specified in @p mask to
//  516  *          become pending then the events are cleared and returned.
//  517  *
//  518  * @param[in] mask      mask of the event flags that the function should wait
//  519  *                      for, @p ALL_EVENTS requires all the events
//  520  * @param[in] time      the number of ticks before the operation timeouts,
//  521  *                      the following special values are allowed:
//  522  *                      - @a TIME_IMMEDIATE immediate timeout.
//  523  *                      - @a TIME_INFINITE no timeout.
//  524  *                      .
//  525  * @return              The mask of the served and cleared events.
//  526  * @retval 0            if the operation has timed out.
//  527  *
//  528  * @api
//  529  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function chEvtWaitAllTimeout
        THUMB
//  530 eventmask_t chEvtWaitAllTimeout(eventmask_t mask, systime_t time) {
chEvtWaitAllTimeout:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  531   Thread *ctp = currp;
        LDR.N    R0,??DataTable13_2
        LDR      R6,[R0, #+24]
//  532 
//  533   chSysLock();
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  534 
//  535   if ((ctp->p_epending & mask) != mask) {
        LDR      R0,[R6, #+56]
        ANDS     R0,R4,R0
        CMP      R0,R4
        BEQ.N    ??chEvtWaitAllTimeout_0
//  536     if (TIME_IMMEDIATE == time) {
        CMP      R5,#+0
        BNE.N    ??chEvtWaitAllTimeout_1
//  537       chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  538       return (eventmask_t)0;
        MOVS     R0,#+0
        B.N      ??chEvtWaitAllTimeout_2
//  539     }
//  540     ctp->p_u.ewmask = mask;
??chEvtWaitAllTimeout_1:
        STR      R4,[R6, #+40]
//  541     if (chSchGoSleepTimeoutS(THD_STATE_WTANDEVT, time) < RDY_OK) {
        MOVS     R1,R5
        MOVS     R0,#+9
          CFI FunCall chSchGoSleepTimeoutS
        BL       chSchGoSleepTimeoutS
        CMP      R0,#+0
        BPL.N    ??chEvtWaitAllTimeout_0
//  542       chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  543       return (eventmask_t)0;
        MOVS     R0,#+0
        B.N      ??chEvtWaitAllTimeout_2
//  544     }
//  545   }
//  546   ctp->p_epending &= ~mask;
??chEvtWaitAllTimeout_0:
        LDR      R0,[R6, #+56]
        BICS     R0,R0,R4
        STR      R0,[R6, #+56]
//  547 
//  548   chSysUnlock();
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  549   return mask;
        MOVS     R0,R4
??chEvtWaitAllTimeout_2:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock16
//  550 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13:
        DC32     `?<Constant "F:\\\\stuff\\\\rusefi_sourc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_1:
        DC32     `?<Constant "\\"chEvtRegisterMask\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_2:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_3:
        DC32     `?<Constant "\\"chEvtUnregister\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_4:
        DC32     `?<Constant "\\"chEvtBroadcastMaskI\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_5:
        DC32     `?<Constant "\\"chEvtSignal\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_6:
        DC32     `?<Constant "\\"chEvtSignalI\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_7:
        DC32     `?<Constant "\\"chEvtDispatch\\"()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_8:
        DC32     `?<Constant "chEvtDispatch(), #1">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chEvtRegisterMask\\"()">`:
        DATA
        DC8 "\"chEvtRegisterMask\"()"
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
        DC8 72H, 63H, 5CH, 63H, 68H, 65H, 76H, 65H
        DC8 6EH, 74H, 73H, 2EH, 63H, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chEvtUnregister\\"()">`:
        DATA
        DC8 "\"chEvtUnregister\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chEvtBroadcastMaskI\\"()">`:
        DATA
        DC8 "\"chEvtBroadcastMaskI\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chEvtSignal\\"()">`:
        DATA
        DC8 "\"chEvtSignal\"()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chEvtSignalI\\"()">`:
        DATA
        DC8 "\"chEvtSignalI\"()"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\"chEvtDispatch\\"()">`:
        DATA
        DC8 "\"chEvtDispatch\"()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "chEvtDispatch(), #1">`:
        DATA
        DC8 "chEvtDispatch(), #1"

        END
//  551 #endif /* CH_USE_EVENTS_TIMEOUT */
//  552 
//  553 #endif /* CH_USE_EVENTS */
//  554 
//  555 /** @} */
// 
//   216 bytes in section .rodata
// 1 114 bytes in section .text
// 
// 1 114 bytes of CODE  memory
//   216 bytes of CONST memory
//
//Errors: none
//Warnings: none
