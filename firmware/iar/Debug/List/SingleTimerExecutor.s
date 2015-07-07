///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:12 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\sys /
//                       tem\SingleTimerExecutor.cpp                          /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\sys /
//                       tem\SingleTimerExecutor.cpp -lCN                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                        -lA F:\stuff\rusefi_sourceforge\firmware\iar\Debug\ /
//                       List\ -o F:\stuff\rusefi_sourceforge\firmware\iar\De /
//                       bug\Obj\ --no_cse --no_unroll --no_inline            /
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
//                       SingleTimerExecutor.s                                /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME SingleTimerExecutor

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _ZN10EventQueue10executeAllEx
        EXTERN _ZN10EventQueue10insertTaskEP12scheduling_sxPFvPvES2_
        EXTERN _ZN10EventQueue12setLateDelayEi
        EXTERN _ZN10EventQueue16getNextEventTimeEx
        EXTERN _ZN10EventQueueC1Ev
        EXTERN dbg_lock_cnt
        EXTERN firmwareError
        EXTERN getRemainingStack
        EXTERN getTimeNowNt
        EXTERN getTimeNowUs
        EXTERN globalTimerCallback
        EXTERN initMicrosecondTimer
        EXTERN lockAnyContext
        EXTERN rlist
        EXTERN setHardwareUsTimer
        EXTERN unlockAnyContext

        PUBLIC _Z12scheduleTaskPKcP12scheduling_siPFvPvES3_
        PUBLIC _Z14scheduleByTimePKcP12scheduling_sxPFvPvES3_
        PUBLIC _Z22initSignalExecutorImplv
        PUBLIC _ZN8Executor14scheduleByTimeEP12scheduling_sxPFvPvES2_
        PUBLIC _ZN8Executor15onTimerCallbackEv
        PUBLIC _ZN8Executor21scheduleTimerCallbackEv
        PUBLIC _ZN8Executor8scheduleEP12scheduling_sxiPFvPvES2_
        PUBLIC _ZN8Executor9doExecuteEv
        PUBLIC _ZN8ExecutorC1Ev
        PUBLIC _ZN8ExecutorC2Ev
        PUBLIC beforeHwSetTimer
        PUBLIC hwSetTimerTime
        PUBLIC lastExecutionCount
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\system\SingleTimerExecutor.cpp
//    1 /**
//    2  * @file SingleTimerExecutor.cpp
//    3  *
//    4  * This class combines the powers of a 1MHz hardware timer from microsecond_timer.c
//    5  * and pending events queue event_queue.cpp
//    6  *
//    7  * As of version 2.6.x, ChibiOS tick-based kernel is not capable of scheduling events
//    8  * with the level of precision we need, and realistically it should not.
//    9  *
//   10  * http://sourceforge.net/p/rusefi/tickets/24/
//   11  *
//   12  * @date: Apr 18, 2014
//   13  * @author Andrey Belomutskiy, (c) 2012-2015
//   14  */
//   15 
//   16 #include "SingleTimerExecutor.h"
//   17 #include "efitime.h"
//   18 #include "efilib2.h"
//   19 
//   20 #if EFI_PROD_CODE
//   21 #include "microsecond_timer.h"
//   22 #endif
//   23 
//   24 #if (EFI_SIGNAL_EXECUTOR_ONE_TIMER && EFI_PROD_CODE )|| defined(__DOXYGEN__)
//   25 #include "rfiutil.h"
//   26 

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   27 static Executor instance;
instance:
        DS8 24
//   28 
//   29 extern schfunc_t globalTimerCallback;
//   30 
//   31 //static int timerIsLate = 0;
//   32 //static efitime_t callbackTime = 0;
//   33 /**
//   34  * these fields are global in order to facilitate debugging
//   35  */

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   36 static efitime_t nextEventTimeNt = 0;
nextEventTimeNt:
        DS8 8
//   37 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   38 uint32_t beforeHwSetTimer;
beforeHwSetTimer:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   39 uint32_t hwSetTimerTime;
hwSetTimerTime:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   40 uint32_t lastExecutionCount;
lastExecutionCount:
        DS8 4

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _ZN8ExecutorC2Ev
        THUMB
// __code __interwork __softfp Executor::subobject Executor()
_ZN8ExecutorC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN8ExecutorC1Ev
        BL       _ZN8ExecutorC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        LDR.N    R0,??DataTable6
          CFI FunCall _ZN8ExecutorC1Ev
        BL       _ZN8ExecutorC1Ev
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock1
//   41 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z16executorCallbackPv
        THUMB
//   42 static void executorCallback(void *arg) {
_Z16executorCallbackPv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   43 	(void)arg;
//   44 	efiAssertVoid(getRemainingStack(chThdSelf()) > 256, "lowstck#2y");
        LDR.N    R0,??DataTable6_1
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+256
        BGT.N    ??executorCallback_0
        LDR.N    R0,??DataTable6_2
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??executorCallback_1
//   45 
//   46 //	callbackTime = getTimeNowNt();
//   47 //	if((callbackTime > nextEventTimeNt) && (callbackTime - nextEventTimeNt > US2NT(5000))) {
//   48 //		timerIsLate++;
//   49 //	}
//   50 
//   51 	instance.onTimerCallback();
??executorCallback_0:
        LDR.N    R0,??DataTable6
          CFI FunCall _ZN8Executor15onTimerCallbackEv
        BL       _ZN8Executor15onTimerCallbackEv
//   52 }
??executorCallback_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2
//   53 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _ZN8ExecutorC1Ev
        THUMB
//   54 Executor::Executor() {
_ZN8ExecutorC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN10EventQueueC1Ev
        BL       _ZN10EventQueueC1Ev
//   55 	reentrantFlag = false;
        MOVS     R0,#+0
        STRB     R0,[R4, #+16]
//   56 	/**
//   57 	 * todo: a good comment
//   58 	 */
//   59 	queue.setLateDelay(US2NT(100));
        MOVW     R1,#+16800
        MOVS     R0,R4
          CFI FunCall _ZN10EventQueue12setLateDelayEi
        BL       _ZN10EventQueue12setLateDelayEi
//   60 }
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//   61 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _ZN8Executor14scheduleByTimeEP12scheduling_sxPFvPvES2_
        THUMB
//   62 void Executor::scheduleByTime(scheduling_s *scheduling, efitimeus_t timeUs, schfunc_t callback,
//   63 		void *param) {
_ZN8Executor14scheduleByTimeEP12scheduling_sxPFvPvES2_:
        PUSH     {R2-R10,LR}
          CFI R14 Frame(CFA, -4)
          CFI R10 Frame(CFA, -8)
          CFI R9 Frame(CFA, -12)
          CFI R8 Frame(CFA, -16)
          CFI R7 Frame(CFA, -20)
          CFI R6 Frame(CFA, -24)
          CFI R5 Frame(CFA, -28)
          CFI R4 Frame(CFA, -32)
          CFI CFA R13+40
        MOVS     R6,R0
        MOV      R9,R1
        MOVS     R4,R2
        MOVS     R5,R3
        LDR      R7,[SP, #+40]
        LDR      R8,[SP, #+44]
//   64 //	if (delayUs < 0) {
//   65 //		firmwareError("Negative delayUs %s: %d", prefix, delayUs);
//   66 //		return;
//   67 //	}
//   68 //	if (delayUs == 0) {
//   69 //		callback(param);
//   70 //		return;
//   71 //	}
//   72 	if (!reentrantFlag) {
        LDRB     R0,[R6, #+16]
        CMP      R0,#+0
        BNE.N    ??scheduleByTime_0
//   73 		// this would guard the queue and disable interrupts
//   74 		lockAnyContext();
          CFI FunCall lockAnyContext
        BL       lockAnyContext
//   75 	}
//   76 	bool_t needToResetTimer = queue.insertTask(scheduling, US2NT(timeUs), callback, param);
??scheduleByTime_0:
        STR      R8,[SP, #+4]
        STR      R7,[SP, #+0]
        MOVS     R0,#+168
        MOVS     R1,#+0
        UMULL    R2,R3,R0,R4
        MLA      R3,R0,R5,R3
        MLA      R3,R1,R4,R3
        MOV      R1,R9
        MOVS     R0,R6
          CFI FunCall _ZN10EventQueue10insertTaskEP12scheduling_sxPFvPvES2_
        BL       _ZN10EventQueue10insertTaskEP12scheduling_sxPFvPvES2_
        MOV      R10,R0
//   77 	if (!reentrantFlag) {
        LDRB     R0,[R6, #+16]
        CMP      R0,#+0
        BNE.N    ??scheduleByTime_1
//   78 		doExecute();
        MOVS     R0,R6
          CFI FunCall _ZN8Executor9doExecuteEv
        BL       _ZN8Executor9doExecuteEv
//   79 		if (needToResetTimer) {
        UXTB     R10,R10          ;; ZeroExt  R10,R10,#+24,#+24
        CMP      R10,#+0
        BEQ.N    ??scheduleByTime_2
//   80 			scheduleTimerCallback();
        MOVS     R0,R6
          CFI FunCall _ZN8Executor21scheduleTimerCallbackEv
        BL       _ZN8Executor21scheduleTimerCallbackEv
//   81 		}
//   82 		unlockAnyContext();
??scheduleByTime_2:
          CFI FunCall unlockAnyContext
        BL       unlockAnyContext
//   83 	}
//   84 }
??scheduleByTime_1:
        POP      {R0,R1,R4-R10,PC}  ;; return
          CFI EndBlock cfiBlock4
//   85 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _ZN8Executor8scheduleEP12scheduling_sxiPFvPvES2_
        THUMB
//   86 void Executor::schedule(scheduling_s *scheduling, efitime_t nowUs, int delayUs, schfunc_t callback,
//   87 		void *param) {
_ZN8Executor8scheduleEP12scheduling_sxiPFvPvES2_:
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
        MOVS     R4,R0
        MOVS     R5,R1
        MOV      R10,R2
        MOV      R11,R3
        LDR      R8,[SP, #+48]
        LDR      R6,[SP, #+52]
        LDR      R7,[SP, #+56]
//   88 	scheduleByTime(scheduling, nowUs + delayUs, callback, param);
        STR      R7,[SP, #+4]
        STR      R6,[SP, #+0]
        ASRS     R9,R8,#+31
        ADDS     R2,R10,R8
        ADCS     R3,R11,R9
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _ZN8Executor14scheduleByTimeEP12scheduling_sxPFvPvES2_
        BL       _ZN8Executor14scheduleByTimeEP12scheduling_sxPFvPvES2_
//   89 }
        POP      {R0-R2,R4-R11,PC}  ;; return
          CFI EndBlock cfiBlock5
//   90 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _ZN8Executor15onTimerCallbackEv
        THUMB
//   91 void Executor::onTimerCallback() {
_ZN8Executor15onTimerCallbackEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   92 	lockAnyContext();
          CFI FunCall lockAnyContext
        BL       lockAnyContext
//   93 	doExecute();
        MOVS     R0,R4
          CFI FunCall _ZN8Executor9doExecuteEv
        BL       _ZN8Executor9doExecuteEv
//   94 	scheduleTimerCallback();
        MOVS     R0,R4
          CFI FunCall _ZN8Executor21scheduleTimerCallbackEv
        BL       _ZN8Executor21scheduleTimerCallbackEv
//   95 	unlockAnyContext();
          CFI FunCall unlockAnyContext
        BL       unlockAnyContext
//   96 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock6
//   97 
//   98 /*
//   99  * this private method is executed under lock
//  100  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _ZN8Executor9doExecuteEv
        THUMB
//  101 void Executor::doExecute() {
_ZN8Executor9doExecuteEv:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOV      R8,R0
//  102 	/**
//  103 	 * Let's execute actions we should execute at this point.
//  104 	 * reentrantFlag takes care of the use case where the actions we are executing are scheduling
//  105 	 * further invocations
//  106 	 */
//  107 	reentrantFlag = true;
        MOVS     R0,#+1
        STRB     R0,[R8, #+16]
//  108 	int shouldExecute = 1;
        MOVS     R6,#+1
//  109 	/**
//  110 	 * in real life it could be that while we executing listeners time passes and it's already time to execute
//  111 	 * next listeners.
//  112 	 * TODO: add a counter & figure out a limit of iterations?
//  113 	 */
//  114 	int totalExecuted = 0;
        MOVS     R7,#+0
//  115 	while (shouldExecute > 0) {
??doExecute_0:
        CMP      R6,#+1
        BLT.N    ??doExecute_1
//  116 		/**
//  117 		 * It's worth noting that that the actions might be adding new actions into the queue
//  118 		 */
//  119 		efitick_t nowNt = getTimeNowNt();
          CFI FunCall getTimeNowNt
        BL       getTimeNowNt
        MOVS     R4,R0
        MOVS     R5,R1
//  120 		shouldExecute = queue.executeAll(nowNt);
        MOVS     R2,R4
        MOVS     R3,R5
        MOV      R0,R8
          CFI FunCall _ZN10EventQueue10executeAllEx
        BL       _ZN10EventQueue10executeAllEx
        MOVS     R6,R0
//  121 		totalExecuted += shouldExecute;
        ADDS     R7,R6,R7
        B.N      ??doExecute_0
//  122 	}
//  123 	lastExecutionCount = totalExecuted;
??doExecute_1:
        LDR.N    R0,??DataTable6_3
        STR      R7,[R0, #+0]
//  124 	if (!isLocked()) {
        LDR.N    R0,??DataTable6_4
        LDR      R0,[R0, #+0]
        CMP      R0,#+1
        BGE.N    ??doExecute_2
//  125 		firmwareError("Someone has stolen my lock");
        LDR.N    R0,??DataTable6_5
          CFI FunCall firmwareError
        BL       firmwareError
//  126 		return;
        B.N      ??doExecute_3
//  127 	}
//  128 	reentrantFlag = false;
??doExecute_2:
        MOVS     R0,#+0
        STRB     R0,[R8, #+16]
//  129 }
??doExecute_3:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock7
//  130 
//  131 /**
//  132  * This method is always invoked under a lock
//  133  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _ZN8Executor21scheduleTimerCallbackEv
        THUMB
//  134 void Executor::scheduleTimerCallback() {
_ZN8Executor21scheduleTimerCallbackEv:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R6,R0
//  135 	/**
//  136 	 * Let's grab fresh time value
//  137 	 */
//  138 	efitick_t nowNt = getTimeNowNt();
          CFI FunCall getTimeNowNt
        BL       getTimeNowNt
        MOVS     R4,R0
        MOVS     R5,R1
//  139 	nextEventTimeNt = queue.getNextEventTime(nowNt);
        MOVS     R2,R4
        MOVS     R3,R5
        MOVS     R0,R6
          CFI FunCall _ZN10EventQueue16getNextEventTimeEx
        BL       _ZN10EventQueue16getNextEventTimeEx
        LDR.N    R2,??DataTable6_6
        STRD     R0,R1,[R2, #+0]
//  140 	efiAssertVoid(nextEventTimeNt > nowNt, "setTimer constraint");
        LDR.N    R0,??DataTable6_6
        LDRD     R2,R3,[R0, #+0]
        CMP      R5,R3
        BLT.N    ??scheduleTimerCallback_0
        BGT.N    ??scheduleTimerCallback_1
        CMP      R4,R2
        BCC.N    ??scheduleTimerCallback_0
??scheduleTimerCallback_1:
        LDR.N    R0,??DataTable6_7
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??scheduleTimerCallback_2
//  141 	if (nextEventTimeNt == EMPTY_QUEUE)
??scheduleTimerCallback_0:
        LDR.N    R0,??DataTable6_6
        LDRD     R2,R3,[R0, #+0]
        MOVS     R0,#-1
        MVNS     R1,#-268435456
        CMP      R3,R1
        BNE.N    ??scheduleTimerCallback_3
        CMP      R2,R0
        BEQ.N    ??scheduleTimerCallback_2
//  142 		return; // no pending events in the queue
//  143 	int32_t hwAlarmTime = NT2US((int32_t)nextEventTimeNt - (int32_t)nowNt);
??scheduleTimerCallback_3:
        LDR.N    R2,??DataTable6_6
        LDRD     R0,R1,[R2, #+0]
        SUBS     R0,R0,R4
        MOVS     R1,#+168
        SDIV     R7,R0,R1
//  144 	beforeHwSetTimer = GET_TIMESTAMP();
        LDR.N    R0,??DataTable6_8
        LDR.N    R1,??DataTable6_9  ;; 0xe0001004
        LDR      R1,[R1, #+0]
        STR      R1,[R0, #+0]
//  145 	setHardwareUsTimer(hwAlarmTime == 0 ? 1 : hwAlarmTime);
        CMP      R7,#+0
        BNE.N    ??scheduleTimerCallback_4
        MOVS     R0,#+1
        B.N      ??scheduleTimerCallback_5
??scheduleTimerCallback_4:
        MOVS     R0,R7
??scheduleTimerCallback_5:
          CFI FunCall setHardwareUsTimer
        BL       setHardwareUsTimer
//  146 	hwSetTimerTime = GET_TIMESTAMP() - beforeHwSetTimer;
        LDR.N    R0,??DataTable6_9  ;; 0xe0001004
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable6_8
        LDR      R1,[R1, #+0]
        SUBS     R0,R0,R1
        LDR.N    R1,??DataTable6_10
        STR      R0,[R1, #+0]
//  147 }
??scheduleTimerCallback_2:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock8
//  148 
//  149 /**
//  150  * @brief Schedule an event
//  151  *
//  152  * Invokes event callback after the specified amount of time.
//  153  *
//  154  * @param [in, out] scheduling Data structure to keep this event in the collection.
//  155  * @param [in] delayUs the number of microseconds before the output signal immediate output if delay is zero.
//  156  * @param [in] dwell the number of ticks of output duration.
//  157  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z12scheduleTaskPKcP12scheduling_siPFvPvES3_
        THUMB
//  158 void scheduleTask(const char *prefix, scheduling_s *scheduling, int delayUs, schfunc_t callback, void *param) {
_Z12scheduleTaskPKcP12scheduling_siPFvPvES3_:
        PUSH     {R2-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+32
        MOV      R8,R0
        MOVS     R4,R1
        MOVS     R6,R2
        MOVS     R5,R3
        LDR      R7,[SP, #+32]
//  159 //	scheduling->name = prefix;
//  160 	instance.scheduleByTime(scheduling, getTimeNowUs() + delayUs, callback, param);
          CFI FunCall getTimeNowUs
        BL       getTimeNowUs
        STR      R7,[SP, #+4]
        STR      R5,[SP, #+0]
        MOVS     R2,R6
        ASRS     R3,R6,#+31
        ADDS     R2,R0,R2
        ADCS     R3,R1,R3
        MOVS     R1,R4
        LDR.N    R0,??DataTable6
          CFI FunCall _ZN8Executor14scheduleByTimeEP12scheduling_sxPFvPvES2_
        BL       _ZN8Executor14scheduleByTimeEP12scheduling_sxPFvPvES2_
//  161 }
        POP      {R0,R1,R4-R8,PC}  ;; return
          CFI EndBlock cfiBlock9
//  162 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z14scheduleByTimePKcP12scheduling_sxPFvPvES3_
        THUMB
//  163 void scheduleByTime(const char *prefix, scheduling_s *scheduling, efitimeus_t time, schfunc_t callback, void *param) {
_Z14scheduleByTimePKcP12scheduling_sxPFvPvES3_:
        PUSH     {R4-R9,LR}
          CFI R14 Frame(CFA, -4)
          CFI R9 Frame(CFA, -8)
          CFI R8 Frame(CFA, -12)
          CFI R7 Frame(CFA, -16)
          CFI R6 Frame(CFA, -20)
          CFI R5 Frame(CFA, -24)
          CFI R4 Frame(CFA, -28)
          CFI CFA R13+28
        SUB      SP,SP,#+12
          CFI CFA R13+40
        MOVS     R6,R0
        MOVS     R7,R1
        MOVS     R4,R2
        MOVS     R5,R3
        LDR      R8,[SP, #+40]
        LDR      R9,[SP, #+44]
//  164 //	scheduling->name = prefix;
//  165 	instance.scheduleByTime(scheduling, time, callback, param);
        STR      R9,[SP, #+4]
        STR      R8,[SP, #+0]
        MOVS     R2,R4
        MOVS     R3,R5
        MOVS     R1,R7
        LDR.N    R0,??DataTable6
          CFI FunCall _ZN8Executor14scheduleByTimeEP12scheduling_sxPFvPvES2_
        BL       _ZN8Executor14scheduleByTimeEP12scheduling_sxPFvPvES2_
//  166 }
        POP      {R0-R2,R4-R9,PC}  ;; return
          CFI EndBlock cfiBlock10
//  167 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z22initSignalExecutorImplv
        THUMB
//  168 void initSignalExecutorImpl(void) {
_Z22initSignalExecutorImplv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  169 	globalTimerCallback = executorCallback;
        LDR.N    R0,??DataTable6_11
        LDR.N    R1,??DataTable6_12
        STR      R1,[R0, #+0]
//  170 #if EFI_PROD_CODE
//  171 	initMicrosecondTimer();
          CFI FunCall initMicrosecondTimer
        BL       initMicrosecondTimer
//  172 #endif /* EFI_PROD_CODE */
//  173 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock11

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6:
        DC32     instance

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_1:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_2:
        DC32     `?<Constant "lowstck#2y">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_3:
        DC32     lastExecutionCount

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_4:
        DC32     dbg_lock_cnt

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_5:
        DC32     `?<Constant "Someone has stolen my...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_6:
        DC32     nextEventTimeNt

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_7:
        DC32     `?<Constant "setTimer constraint">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_8:
        DC32     beforeHwSetTimer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_9:
        DC32     0xe0001004

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_10:
        DC32     hwSetTimerTime

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_11:
        DC32     globalTimerCallback

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_12:
        DC32     _Z16executorCallbackPv

        SECTION `.init_array`:CODE:ROOT(2)
        SECTION_TYPE SHT_INIT_ARRAY, 0
        DATA
        DC32    RELOC_ARM_TARGET1 __sti__routine

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lowstck#2y">`:
        DATA
        DC8 "lowstck#2y"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Someone has stolen my...">`:
        DATA
        DC8 "Someone has stolen my lock"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "setTimer constraint">`:
        DATA
        DC8 "setTimer constraint"

        END
//  174 
//  175 #endif
//  176 
// 
//  44 bytes in section .bss
//   4 bytes in section .init_array
//  60 bytes in section .rodata
// 610 bytes in section .text
// 
// 614 bytes of CODE  memory
//  60 bytes of CONST memory
//  44 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
