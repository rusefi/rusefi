///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:48 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\sys /
//                       tem\event_queue.cpp                                  /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\sys /
//                       tem\event_queue.cpp -lCN F:\stuff\rusefi_sourceforge /
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
//                       -On --eec++                                          /
//    MISRA C version =  2004                                                 /
//      Enabled       =  1.2-1.4 2.3 3.4 4.2 6.5 7 8.2 8.3 9.3 12.3 13.6      /
//                       13.7 15.5 17.3 19.17 20.7-20.9 20.11                 /
//      Checked       =  1.4 2.3 4.2 6.5 7 8.2 8.3 9.3 12.3 13.7 15.5         /
//                       20.7-20.9 20.11                                      /
//      Not checked   =  1.1-1.3 1.5-2.2 2.4-4.1 5-6.4 8.1 8.4-9.2 10-12.2    /
//                       12.4-13.6 14-15.4 16-20.6 20.10 20.12 21             /
//    List file       =  F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                       event_queue.s                                        /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME event_queue

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN firmwareError
        EXTERN maxI
        EXTERN warning

        PUBLIC _Z15assertNotInListI12scheduling_sEbPT_S2_
        PUBLIC _ZN10EventQueue10executeAllEx
        PUBLIC _ZN10EventQueue10insertTaskEP12scheduling_sxPFvPvES2_
        PUBLIC _ZN10EventQueue12setLateDelayEi
        PUBLIC _ZN10EventQueue14checkIfPendingEP12scheduling_s
        PUBLIC _ZN10EventQueue14getForUnitTextEi
        PUBLIC _ZN10EventQueue16getNextEventTimeEx
        PUBLIC _ZN10EventQueue4sizeEv
        PUBLIC _ZN10EventQueue5clearEv
        PUBLIC _ZN10EventQueue7getHeadEv
        PUBLIC _ZN10EventQueueC1Ev
        PUBLIC _ZN10EventQueueC2Ev
        PUBLIC _ZN12scheduling_sC1Ev
        PUBLIC _ZN12scheduling_sC2Ev
        PUBLIC _ZZ15assertNotInListI12scheduling_sEbPT_S2_Es
        PUBLIC _ZZ15assertNotInListI12scheduling_sEbPT_S2_Es_0
        PUBLIC lastEventQueueTime
        PUBLIC maxEventQueueTime
        PUBLIC maxHowFarOff
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\system\event_queue.cpp
//    1 /**
//    2  * @file event_queue.cpp
//    3  * This is a data structure which keeps track of all pending events
//    4  * Implemented as a linked list, which is fine since the number of
//    5  * pending events is pretty low
//    6  * todo: MAYBE migrate to a better data structure, but that's low priority
//    7  *
//    8  * this data structure is NOT thread safe
//    9  *
//   10  * @date Apr 17, 2014
//   11  * @author Andrey Belomutskiy, (c) 2012-2015
//   12  */
//   13 
//   14 #include "main.h"
//   15 #include "event_queue.h"
//   16 #include "efitime.h"
//   17 #include "efilib2.h"
//   18 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   19 int maxHowFarOff = 0;
maxHowFarOff:
        DS8 4
//   20 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _ZN12scheduling_sC2Ev
        THUMB
// __code __interwork __softfp scheduling_s::subobject scheduling_s()
_ZN12scheduling_sC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN12scheduling_sC1Ev
        BL       _ZN12scheduling_sC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _ZN10EventQueueC2Ev
        THUMB
// __code __interwork __softfp EventQueue::subobject EventQueue()
_ZN10EventQueueC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN10EventQueueC1Ev
        BL       _ZN10EventQueueC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _ZN12scheduling_sC1Ev
          CFI NoCalls
        THUMB
//   21 scheduling_s::scheduling_s() {
//   22 	callback = NULL;
_ZN12scheduling_sC1Ev:
        MOVS     R1,#+0
        STR      R1,[R0, #+8]
//   23 	next = NULL;
        MOVS     R1,#+0
        STR      R1,[R0, #+16]
//   24 	param = NULL;
        MOVS     R1,#+0
        STR      R1,[R0, #+12]
//   25 	isScheduled = false;
        MOVS     R1,#+0
        STRB     R1,[R0, #+20]
//   26 	momentX = 0;
        MOVS     R2,#+0
        MOVS     R3,#+0
        STRD     R2,R3,[R0, #+0]
//   27 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock2
//   28 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _ZN10EventQueueC1Ev
        THUMB
//   29 EventQueue::EventQueue() {
_ZN10EventQueueC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   30 	head = NULL;
        MOVS     R0,#+0
        STR      R0,[R4, #+0]
//   31 	setLateDelay(100);
        MOVS     R1,#+100
        MOVS     R0,R4
          CFI FunCall _ZN10EventQueue12setLateDelayEi
        BL       _ZN10EventQueue12setLateDelayEi
//   32 }
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//   33 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _ZN10EventQueue14checkIfPendingEP12scheduling_s
        THUMB
//   34 bool EventQueue::checkIfPending(scheduling_s *scheduling) {
_ZN10EventQueue14checkIfPendingEP12scheduling_s:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   35 	return assertNotInList<scheduling_s>(head, scheduling);
        MOVS     R1,R5
        LDR      R0,[R4, #+0]
          CFI FunCall _Z15assertNotInListI12scheduling_sEbPT_S2_
        BL       _Z15assertNotInListI12scheduling_sEbPT_S2_
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock4
//   36 }
//   37 
//   38 /**
//   39  * @return true if inserted into the head of the list
//   40  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _ZN10EventQueue10insertTaskEP12scheduling_sxPFvPvES2_
        THUMB
//   41 bool_t EventQueue::insertTask(scheduling_s *scheduling, efitime_t timeX, schfunc_t callback, void *param) {
_ZN10EventQueue10insertTaskEP12scheduling_sxPFvPvES2_:
        PUSH     {R3-R9,LR}
          CFI R14 Frame(CFA, -4)
          CFI R9 Frame(CFA, -8)
          CFI R8 Frame(CFA, -12)
          CFI R7 Frame(CFA, -16)
          CFI R6 Frame(CFA, -20)
          CFI R5 Frame(CFA, -24)
          CFI R4 Frame(CFA, -28)
          CFI CFA R13+32
        MOVS     R6,R0
        MOVS     R7,R1
        MOVS     R4,R2
        MOVS     R5,R3
        LDR      R8,[SP, #+32]
        LDR      R9,[SP, #+36]
//   42 #if EFI_UNIT_TEST
//   43 	assertListIsSorted();
//   44 #endif
//   45 	efiAssert(callback != NULL, "NULL callback", false);
        MOV      R0,R8
        CMP      R0,#+0
        BNE.N    ??insertTask_0
        LDR.N    R0,??DataTable1
          CFI FunCall firmwareError
        BL       firmwareError
        MOVS     R0,#+0
        B.N      ??insertTask_1
//   46 
//   47 	if (scheduling->isScheduled)
??insertTask_0:
        LDRB     R0,[R7, #+20]
        CMP      R0,#+0
        BEQ.N    ??insertTask_2
//   48 		return false;
        MOVS     R0,#+0
        B.N      ??insertTask_1
//   49 
//   50 	scheduling->momentX = timeX;
??insertTask_2:
        STRD     R4,R5,[R7, #+0]
//   51 	scheduling->callback = callback;
        STR      R8,[R7, #+8]
//   52 	scheduling->param = param;
        STR      R9,[R7, #+12]
//   53 	scheduling->isScheduled = true;
        MOVS     R0,#+1
        STRB     R0,[R7, #+20]
//   54 
//   55 	if (head == NULL || timeX < head->momentX) {
        LDR      R0,[R6, #+0]
        CMP      R0,#+0
        BEQ.N    ??insertTask_3
        LDR      R2,[R6, #+0]
        LDRD     R0,R1,[R2, #+0]
        CMP      R5,R1
        BGT.N    ??insertTask_4
        BLT.N    ??insertTask_5
        CMP      R4,R0
        BCS.N    ??insertTask_4
//   56 		LL_PREPEND(head, scheduling);
??insertTask_5:
??insertTask_3:
        LDR      R0,[R6, #+0]
        STR      R0,[R7, #+16]
        STR      R7,[R6, #+0]
//   57 #if EFI_UNIT_TEST
//   58 		assertListIsSorted();
//   59 #endif
//   60 		return true;
        MOVS     R0,#+1
        B.N      ??insertTask_1
//   61 	} else {
//   62 		scheduling_s *insertPosition = head;
??insertTask_4:
        LDR      R2,[R6, #+0]
//   63 		while (insertPosition->next != NULL && insertPosition->next->momentX < timeX) {
??insertTask_6:
        LDR      R0,[R2, #+16]
        CMP      R0,#+0
        BEQ.N    ??insertTask_7
        LDR      R3,[R2, #+16]
        LDRD     R0,R1,[R3, #+0]
        CMP      R1,R5
        BGT.N    ??insertTask_7
        BLT.N    ??insertTask_8
        CMP      R0,R4
        BCS.N    ??insertTask_7
//   64 			insertPosition = insertPosition->next;
??insertTask_8:
        LDR      R2,[R2, #+16]
        B.N      ??insertTask_6
//   65 		}
//   66 
//   67 		scheduling->next = insertPosition->next;
??insertTask_7:
        LDR      R0,[R2, #+16]
        STR      R0,[R7, #+16]
//   68 		insertPosition->next = scheduling;
        STR      R7,[R2, #+16]
//   69 #if EFI_UNIT_TEST
//   70 		assertListIsSorted();
//   71 #endif
//   72 		return false;
        MOVS     R0,#+0
??insertTask_1:
        POP      {R1,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock5
//   73 	}
//   74 }
//   75 
//   76 /**
//   77  * On this layer it does not matter which units are used - us, ms ot nt.
//   78  *
//   79  * This method is always invoked under a lock
//   80  * @return Get the timestamp of the soonest pending action, skipping all the actions in the past
//   81  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _ZN10EventQueue16getNextEventTimeEx
          CFI NoCalls
        THUMB
//   82 efitime_t EventQueue::getNextEventTime(efitime_t nowX) {
_ZN10EventQueue16getNextEventTimeEx:
        PUSH     {R4,R6-R9}
          CFI R9 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+20
        MOVS     R4,R0
//   83 	efitime_t nextTimeUs = EMPTY_QUEUE;
        MOVS     R6,#-1
        MVNS     R7,#-268435456
//   84 
//   85 	if (head != NULL) {
        LDR      R0,[R4, #+0]
        CMP      R0,#+0
        BEQ.N    ??getNextEventTime_0
//   86 		if (head->momentX <= nowX) {
        LDR      R0,[R4, #+0]
        LDRD     R8,R9,[R0, #+0]
        CMP      R3,R9
        BLT.N    ??getNextEventTime_1
        BGT.N    ??getNextEventTime_2
        CMP      R2,R8
        BCC.N    ??getNextEventTime_1
//   87 			/**
//   88 			 * We are here if action timestamp is in the past
//   89 			 *
//   90 			 * looks like we end up here after 'writeconfig' (which freezes the firmware) - we are late
//   91 			 * for the next scheduled event
//   92 			 */
//   93 			efitime_t aBitInTheFuture = nowX + lateDelay;
??getNextEventTime_2:
        LDRD     R0,R1,[R4, #+8]
        ADDS     R0,R2,R0
        ADCS     R1,R3,R1
//   94 			return aBitInTheFuture;
        B.N      ??getNextEventTime_3
//   95 		} else {
//   96 			return head->momentX;
??getNextEventTime_1:
        LDR      R4,[R4, #+0]
        LDRD     R0,R1,[R4, #+0]
        B.N      ??getNextEventTime_3
//   97 		}
//   98 	}
//   99 	return EMPTY_QUEUE;
??getNextEventTime_0:
        MOVS     R0,#-1
        MVNS     R1,#-268435456
??getNextEventTime_3:
        POP      {R4,R6-R9}
          CFI R4 SameValue
          CFI R6 SameValue
          CFI R7 SameValue
          CFI R8 SameValue
          CFI R9 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock6
//  100 }
//  101 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  102 static scheduling_s * longScheduling;
longScheduling:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  103 uint32_t maxEventQueueTime = 0;
maxEventQueueTime:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  104 uint32_t lastEventQueueTime;
lastEventQueueTime:
        DS8 4
//  105 
//  106 /**
//  107  * Invoke all pending actions prior to specified timestamp
//  108  * @return number of executed actions
//  109  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _ZN10EventQueue10executeAllEx
        THUMB
//  110 int EventQueue::executeAll(efitime_t now) {
_ZN10EventQueue10executeAllEx:
        PUSH     {R4-R11,LR}
          CFI R14 Frame(CFA, -4)
          CFI R11 Frame(CFA, -8)
          CFI R10 Frame(CFA, -12)
          CFI R9 Frame(CFA, -16)
          CFI R8 Frame(CFA, -20)
          CFI R7 Frame(CFA, -24)
          CFI R6 Frame(CFA, -28)
          CFI R5 Frame(CFA, -32)
          CFI R4 Frame(CFA, -36)
          CFI CFA R13+36
        SUB      SP,SP,#+12
          CFI CFA R13+48
        MOV      R10,R0
        MOVS     R4,R2
        MOVS     R5,R3
//  111 	scheduling_s * current, *tmp;
//  112 
//  113 	scheduling_s * executionList = NULL;
        MOVS     R11,#+0
//  114 
//  115 	int listIterationCounter = 0;
        MOVS     R7,#+0
//  116 	int executionCounter = 0;
        MOVS     R8,#+0
//  117 	// we need safe iteration because we are removing elements inside the loop
//  118 	LL_FOREACH_SAFE(head, current, tmp)
        LDR      R0,[R10, #+0]
        MOVS     R6,R0
??executeAll_0:
        CMP      R6,#+0
        BEQ.N    ??executeAll_1
        LDR      R9,[R6, #+16]
        MOVS     R0,#+0
        CMP      R0,#+0
        BNE.N    ??executeAll_1
//  119 	{
//  120 		if (++listIterationCounter > QUEUE_LENGTH_LIMIT) {
        ADDS     R7,R7,#+1
        CMP      R7,#+1000
        BLE.N    ??executeAll_2
//  121 			firmwareError("Is this list looped?");
        LDR.N    R0,??DataTable1_1
          CFI FunCall firmwareError
        BL       firmwareError
//  122 			return false;
        MOVS     R0,#+0
        B.N      ??executeAll_3
//  123 		}
//  124 		if (current->momentX <= now) {
??executeAll_2:
        LDRD     R0,R1,[R6, #+0]
        CMP      R5,R1
        BLT.N    ??executeAll_4
        BGT.N    ??executeAll_5
        CMP      R4,R0
        BCC.N    ??executeAll_4
//  125 			executionCounter++;
??executeAll_5:
        ADDS     R8,R8,#+1
//  126 			efiAssert(head == current, "removing from head", -1);
        LDR      R0,[R10, #+0]
        CMP      R0,R6
        BEQ.N    ??executeAll_6
        LDR.N    R0,??DataTable1_2
          CFI FunCall firmwareError
        BL       firmwareError
        MOVS     R0,#-1
        B.N      ??executeAll_3
//  127 			//LL_DELETE(head, current);
//  128 			head = head->next;
??executeAll_6:
        LDR      R0,[R10, #+0]
        LDR      R0,[R0, #+16]
        STR      R0,[R10, #+0]
//  129 			LL_PREPEND(executionList, current);
        STR      R11,[R6, #+16]
        MOV      R11,R6
//  130 		} else {
        MOV      R6,R9
        B.N      ??executeAll_0
//  131 			/**
//  132 			 * The list is sorted. Once we find one action in the future, all the remaining ones
//  133 			 * are also in the future.
//  134 			 */
//  135 			break;
//  136 		}
//  137 	}
//  138 #if EFI_UNIT_TEST
//  139 	assertListIsSorted();
//  140 #endif
//  141 
//  142 	/*
//  143 	 * we need safe iteration here because 'callback' might change change 'current->next'
//  144 	 * while re-inserting it into the queue from within the callback
//  145 	 */
//  146 	LL_FOREACH_SAFE(executionList, current, tmp)
??executeAll_4:
??executeAll_1:
        MOV      R6,R11
??executeAll_7:
        CMP      R6,#+0
        BEQ.N    ??executeAll_8
        LDR      R9,[R6, #+16]
        MOVS     R0,#+0
        CMP      R0,#+0
        BNE.N    ??executeAll_8
//  147 	{
//  148 		uint32_t before = GET_TIMESTAMP();
        LDR.N    R0,??DataTable1_3  ;; 0xe0001004
        LDR      R0,[R0, #+0]
        STR      R0,[SP, #+4]
//  149 		current->isScheduled = false;
        MOVS     R0,#+0
        STRB     R0,[R6, #+20]
//  150 		int howFarOff = now - current->momentX;
        LDRD     R0,R1,[R6, #+0]
        SUBS     R0,R4,R0
        STR      R0,[SP, #+0]
//  151 		maxHowFarOff = maxI(maxHowFarOff, howFarOff);
        LDR      R1,[SP, #+0]
        LDR.N    R0,??DataTable1_4
        LDR      R0,[R0, #+0]
          CFI FunCall maxI
        BL       maxI
        LDR.N    R1,??DataTable1_4
        STR      R0,[R1, #+0]
//  152 		current->callback(current->param);
        LDR      R0,[R6, #+12]
        LDR      R1,[R6, #+8]
          CFI FunCall
        BLX      R1
//  153 		// even with overflow it's safe to subtract here
//  154 		lastEventQueueTime = GET_TIMESTAMP() - before;
        LDR.N    R0,??DataTable1_3  ;; 0xe0001004
        LDR      R0,[R0, #+0]
        LDR      R1,[SP, #+4]
        SUBS     R0,R0,R1
        LDR.N    R1,??DataTable1_5
        STR      R0,[R1, #+0]
//  155 		if (lastEventQueueTime > maxEventQueueTime)
        LDR.N    R0,??DataTable1_6
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable1_5
        LDR      R1,[R1, #+0]
        CMP      R0,R1
        BCS.N    ??executeAll_9
//  156 			maxEventQueueTime = lastEventQueueTime;
        LDR.N    R0,??DataTable1_6
        LDR.N    R1,??DataTable1_5
        LDR      R1,[R1, #+0]
        STR      R1,[R0, #+0]
//  157 		if (lastEventQueueTime > 2000) {
??executeAll_9:
        LDR.N    R0,??DataTable1_5
        LDR      R0,[R0, #+0]
        CMP      R0,#+2000
        BLS.N    ??executeAll_10
//  158 			longScheduling = current;
        LDR.N    R0,??DataTable1_7
        STR      R6,[R0, #+0]
//  159 			lastEventQueueTime++;
        LDR.N    R0,??DataTable1_5
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable1_5
        STR      R0,[R1, #+0]
//  160 		}
//  161 	}
??executeAll_10:
        MOV      R6,R9
        B.N      ??executeAll_7
//  162 	return executionCounter;
??executeAll_8:
        MOV      R0,R8
??executeAll_3:
        POP      {R1-R11,PC}      ;; return
          CFI EndBlock cfiBlock7
//  163 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1:
        DC32     `?<Constant "NULL callback">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_1:
        DC32     `?<Constant "Is this list looped?">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_2:
        DC32     `?<Constant "removing from head">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_3:
        DC32     0xe0001004

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_4:
        DC32     maxHowFarOff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_5:
        DC32     lastEventQueueTime

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_6:
        DC32     maxEventQueueTime

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_7:
        DC32     longScheduling
//  164 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _ZN10EventQueue4sizeEv
          CFI NoCalls
        THUMB
//  165 int EventQueue::size(void) {
_ZN10EventQueue4sizeEv:
        MOVS     R1,R0
//  166 	scheduling_s *tmp;
//  167 	int result;
//  168 	LL_COUNT(head, tmp, result);
        MOVS     R3,#+0
        MOVS     R0,R3
        LDR      R3,[R1, #+0]
        MOVS     R2,R3
??size_0:
        CMP      R2,#+0
        BEQ.N    ??size_1
        ADDS     R0,R0,#+1
        LDR      R2,[R2, #+16]
        B.N      ??size_0
//  169 	return result;
??size_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock8
//  170 }
//  171 
//  172 #if EFI_UNIT_TEST
//  173 void EventQueue::assertListIsSorted() {
//  174 	scheduling_s *current = head;
//  175 	while (current != NULL && current->next != NULL) {
//  176 		efiAssertVoid(current->momentX <= current->next->momentX, "list order");
//  177 		current = current->next;
//  178 	}
//  179 }
//  180 #endif
//  181 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _ZN10EventQueue12setLateDelayEi
          CFI NoCalls
        THUMB
//  182 void EventQueue::setLateDelay(int value) {
//  183 	lateDelay = value;
_ZN10EventQueue12setLateDelayEi:
        MOVS     R2,R1
        ASRS     R3,R1,#+31
        STRD     R2,R3,[R0, #+8]
//  184 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock9
//  185 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _ZN10EventQueue7getHeadEv
          CFI NoCalls
        THUMB
//  186 scheduling_s * EventQueue::getHead() {
//  187 	return head;
_ZN10EventQueue7getHeadEv:
        LDR      R0,[R0, #+0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock10
//  188 }
//  189 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _ZN10EventQueue14getForUnitTextEi
          CFI NoCalls
        THUMB
//  190 scheduling_s *EventQueue::getForUnitText(int index) {
_ZN10EventQueue14getForUnitTextEi:
        MOVS     R2,R0
//  191 	scheduling_s * current;
//  192 
//  193 	LL_FOREACH(head, current)
        LDR      R0,[R2, #+0]
        MOVS     R3,R0
??getForUnitText_0:
        CMP      R3,#+0
        BEQ.N    ??getForUnitText_1
//  194 	{
//  195 		if (index == 0)
        CMP      R1,#+0
        BNE.N    ??getForUnitText_2
//  196 			return current;
        MOVS     R0,R3
        B.N      ??getForUnitText_3
//  197 		index--;
??getForUnitText_2:
        SUBS     R1,R1,#+1
//  198 	}
        LDR      R3,[R3, #+16]
        B.N      ??getForUnitText_0
//  199 	return NULL;
??getForUnitText_1:
        MOVS     R0,#+0
??getForUnitText_3:
        BX       LR               ;; return
          CFI EndBlock cfiBlock11
//  200 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _ZN10EventQueue5clearEv
          CFI NoCalls
        THUMB
_ZN10EventQueue5clearEv:
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock12

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _Z15assertNotInListI12scheduling_sEbPT_S2_
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z15assertNotInListI12scheduling_sEbPT_S2_
        THUMB
// __interwork __softfp bool assertNotInList<scheduling_s>(scheduling_s *, scheduling_s *)
_Z15assertNotInListI12scheduling_sEbPT_S2_:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R7,#+0
        MOVS     R6,R4
??assertNotInList_0:
        CMP      R6,#+0
        BEQ.N    ??assertNotInList_1
        ADDS     R7,R7,#+1
        CMP      R7,#+1000
        BLE.N    ??assertNotInList_2
        LDR.N    R0,??assertNotInList_3
          CFI FunCall firmwareError
        BL       firmwareError
        MOVS     R0,#+0
        B.N      ??assertNotInList_4
??assertNotInList_2:
        CMP      R6,R5
        BNE.N    ??assertNotInList_5
        LDR.N    R1,??assertNotInList_3+0x4
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
        MOVS     R0,#+1
        B.N      ??assertNotInList_4
??assertNotInList_5:
        LDR      R6,[R6, #+16]
        B.N      ??assertNotInList_0
??assertNotInList_1:
        MOVS     R0,#+0
??assertNotInList_4:
        POP      {R1,R4-R7,PC}    ;; return
        Nop      
        DATA
??assertNotInList_3:
        DC32     _ZZ15assertNotInListI12scheduling_sEbPT_S2_Es
        DC32     _ZZ15assertNotInListI12scheduling_sEbPT_S2_Es_0
          CFI EndBlock cfiBlock13

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        SECTION_GROUP _ZZ15assertNotInListI12scheduling_sEbPT_S2_Es
// __absolute char const <_ZZ15assertNotInListI12scheduling_sEbPT_S2_Es>[14]
_ZZ15assertNotInListI12scheduling_sEbPT_S2_Es:
        DATA
        DC8 "Looped queue?"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        SECTION_GROUP _ZZ15assertNotInListI12scheduling_sEbPT_S2_Es_0
// __absolute char const <_ZZ15assertNotInListI12scheduling_sEbPT_S2_Es_0>[35]
_ZZ15assertNotInListI12scheduling_sEbPT_S2_Es_0:
        DATA
        DC8 "re-adding element into event_queue"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "NULL callback">`:
        DATA
        DC8 "NULL callback"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Is this list looped?">`:
        DATA
        DC8 "Is this list looped?"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "removing from head">`:
        DATA
        DC8 "removing from head"
        DC8 0

        END
//  201 
//  202 void EventQueue::clear(void) {
//  203 	head = NULL;
//  204 }
// 
//  16 bytes in section .bss
// 112 bytes in section .rodata
// 696 bytes in section .text
// 
// 628 bytes of CODE  memory (+ 68 bytes shared)
//  60 bytes of CONST memory (+ 52 bytes shared)
//  16 bytes of DATA  memory
//
//Errors: none
//Warnings: 3
