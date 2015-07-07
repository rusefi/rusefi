///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:49 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\tri /
//                       gger\trigger_central.cpp                             /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\tri /
//                       gger\trigger_central.cpp -lCN                        /
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
//                       trigger_central.s                                    /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME trigger_central

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z10hwPortname11brain_pin_e
        EXTERN _Z12boolToStringb
        EXTERN _Z13initWaveChartP9WaveChart
        EXTERN _Z14clearCallbacksP16IntListenerArray
        EXTERN _Z14printHistogramP7LoggingP11histogram_s
        EXTERN _Z17getTrigger_type_e14trigger_type_e
        EXTERN _Z20getConfigurationName13engine_type_e
        EXTERN _Z20getPin_output_mode_e17pin_output_mode_e
        EXTERN _Z21isTriggerDecoderErrorv
        EXTERN _ZN12TriggerState15getCurrentIndexEv
        EXTERN _ZN12TriggerState18decodeTriggerEventE15trigger_event_ex
        EXTERN _ZN12TriggerState19getTriggerDutyCycleEi
        EXTERN _ZN12TriggerState20getTotalEventCounterEv
        EXTERN _ZN12TriggerState20resetRunningCountersEv
        EXTERN _ZN12TriggerState25getStartOfRevolutionIndexEv
        EXTERN _ZN12TriggerState25getTotalRevolutionCounterEv
        EXTERN _ZN12TriggerStateC1Ev
        EXTERN _ZN16IntListenerArray16registerCallbackEPFviEPv
        EXTERN _ZN16IntListenerArrayC1Ev
        EXTERN _ZN6Engine14onTriggerEventEx
        EXTERN _ZN9WaveChart18addWaveChartEvent3EPKcS1_
        EXTERN _ZN9WaveChartC1Ev
        EXTERN _ZNK12TriggerShape7getSizeEv
        EXTERN __aeabi_f2d
        EXTERN __aeabi_memset
        EXTERN _engine
        EXTERN addConsoleAction
        EXTERN boardConfiguration
        EXTERN cyccnt
        EXTERN engine
        EXTERN engineConfiguration
        EXTERN firmwareError
        EXTERN getRemainingStack
        EXTERN getTimeNowNt
        EXTERN hipLastExecutionCount
        EXTERN hwSetTimerTime
        EXTERN isInsideTriggerHandler
        EXTERN itoa10
        EXTERN maxEventQueueTime
        EXTERN maxHowFarOff
        EXTERN maxLockTime
        EXTERN print
        EXTERN rlist
        EXTERN scheduleMsg
        EXTERN strcpy
        EXTERN triggerSignal
        EXTERN warning

        PUBLIC _Z11triggerInfov
        PUBLIC _Z18initTriggerCentralP7LoggingP6Engine
        PUBLIC _Z19getTriggerDutyCyclei
        PUBLIC _Z19hwHandleShaftSignal15trigger_event_e
        PUBLIC _Z20getCrankEventCounterv
        PUBLIC _Z23addTriggerEventListenerPFv15trigger_event_ejEPKcP6Engine
        PUBLIC _Z25getStartOfRevolutionIndexv
        PUBLIC _Z26printAllCallbacksHistogramv
        PUBLIC _ZN14TriggerCentral13resetCountersEv
        PUBLIC _ZN14TriggerCentral16addEventListenerEPFv15trigger_event_ejEPKcP6Engine
        PUBLIC _ZN14TriggerCentral17getHwEventCounterEi
        PUBLIC _ZN14TriggerCentral17handleShaftSignalE15trigger_event_e
        PUBLIC _ZN14TriggerCentralC1Ev
        PUBLIC _ZN14TriggerCentralC2Ev
        PUBLIC maxTriggerReentraint
        PUBLIC memset
        PUBLIC triggerCentral
        PUBLIC triggerDuration
        PUBLIC triggerHanlderEntryTime
        PUBLIC triggerMaxDuration
        PUBLIC triggerReentraint
        PUBLIC waveChart
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\trigger\trigger_central.cpp
//    1 /*
//    2  * @file	trigger_central.cpp
//    3  *
//    4  * @date Feb 23, 2014
//    5  * @author Andrey Belomutskiy, (c) 2012-2015
//    6  */
//    7 
//    8 #include "main.h"
//    9 
//   10 #if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
//   11 
//   12 #include "trigger_central.h"
//   13 #include "trigger_decoder.h"
//   14 #include "main_trigger_callback.h"
//   15 #include "engine_configuration.h"
//   16 #include "listener_array.h"
//   17 #include "data_buffer.h"
//   18 #include "histogram.h"
//   19 #include "pwm_generator_logic.h"
//   20 #include "efilib2.h"
//   21 #include "settings.h"
//   22 #include "auto_generated_enums.h"
//   23 
//   24 #include "rpm_calculator.h"
//   25 #if EFI_PROD_CODE
//   26 #include "rfiutil.h"
//   27 #include "pin_repository.h"
//   28 #endif
//   29 
//   30 #if EFI_WAVE_CHART
//   31 #include "wave_chart.h"

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   32 WaveChart waveChart;
waveChart:
        DS8 80
//   33 #endif /* EFI_WAVE_CHART */
//   34 

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   35 static histogram_s triggerCallback;
triggerCallback:
        DS8 3616
//   36 
//   37 // we need this initial to have not_running at first invocation

        SECTION `.data`:DATA:REORDER:NOROOT(3)
//   38 static volatile efitime_t previousShaftEventTimeNt = (efitimems_t) -10 * US2NT(US_PER_SECOND_LL);
previousShaftEventTimeNt:
        DATA
        DC32 2614967296, 167999999
//   39 

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   40 TriggerCentral triggerCentral;
triggerCentral:
        DS8 344
//   41 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   42 static Logging *logger;
logger:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
triggerHanlderEntryTime:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
triggerReentraint:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
maxTriggerReentraint:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
triggerDuration:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
triggerMaxDuration:
        DS8 4

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _ZN14TriggerCentralC2Ev
        THUMB
// __code __interwork __softfp TriggerCentral::subobject TriggerCentral()
_ZN14TriggerCentralC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN14TriggerCentralC1Ev
        BL       _ZN14TriggerCentralC1Ev
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
        LDR.W    R0,??DataTable13_1
          CFI FunCall _ZN9WaveChartC1Ev
        BL       _ZN9WaveChartC1Ev
        LDR.W    R0,??DataTable13_2
          CFI FunCall _ZN14TriggerCentralC1Ev
        BL       _ZN14TriggerCentralC1Ev
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock1
//   43 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z20getCrankEventCounterv
        THUMB
//   44 efitime_t getCrankEventCounter() {
_Z20getCrankEventCounterv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   45 	return triggerCentral.triggerState.getTotalEventCounter();
        LDR.W    R0,??DataTable13_2
          CFI FunCall _ZN12TriggerState20getTotalEventCounterEv
        BL       _ZN12TriggerState20getTotalEventCounterEv
        POP      {R2,PC}          ;; return
          CFI EndBlock cfiBlock2
//   46 }
//   47 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z25getStartOfRevolutionIndexv
        THUMB
//   48 efitime_t getStartOfRevolutionIndex() {
_Z25getStartOfRevolutionIndexv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   49 	return triggerCentral.triggerState.getStartOfRevolutionIndex();
        LDR.W    R0,??DataTable13_2
          CFI FunCall _ZN12TriggerState25getStartOfRevolutionIndexEv
        BL       _ZN12TriggerState25getStartOfRevolutionIndexEv
        POP      {R2,PC}          ;; return
          CFI EndBlock cfiBlock3
//   50 }
//   51 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _ZN14TriggerCentral16addEventListenerEPFv15trigger_event_ejEPKcP6Engine
        THUMB
//   52 void TriggerCentral::addEventListener(ShaftPositionListener listener, const char *name, Engine *engine) {
_ZN14TriggerCentral16addEventListenerEPFv15trigger_event_ejEPKcP6Engine:
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
        MOVS     R7,R3
//   53 	print("registerCkpListener: %s\r\n", name);
        MOVS     R1,R6
        LDR.W    R0,??DataTable13_3
          CFI FunCall print
        BL       print
//   54 	triggerListeneres.registerCallback((VoidInt) listener, engine);
        MOVS     R2,R7
        MOVS     R1,R5
        ADDS     R0,R4,#+192
          CFI FunCall _ZN16IntListenerArray16registerCallbackEPFviEPv
        BL       _ZN16IntListenerArray16registerCallbackEPFviEPv
//   55 }
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock4
//   56 
//   57 /**
//   58  * @brief Adds a trigger event listener
//   59  *
//   60  * Trigger event listener would be invoked on each trigger event. For example, for a 60/2 wheel
//   61  * that would be 116 events: 58 SHAFT_PRIMARY_UP and 58 SHAFT_PRIMARY_DOWN events.
//   62  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z23addTriggerEventListenerPFv15trigger_event_ejEPKcP6Engine
        THUMB
//   63 void addTriggerEventListener(ShaftPositionListener listener, const char *name, Engine *engine) {
_Z23addTriggerEventListenerPFv15trigger_event_ejEPKcP6Engine:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   64 	triggerCentral.addEventListener(listener, name, engine);
        MOVS     R3,R6
        MOVS     R2,R5
        MOVS     R1,R4
        LDR.W    R0,??DataTable13_2
          CFI FunCall _ZN14TriggerCentral16addEventListenerEPFv15trigger_event_ejEPKcP6Engine
        BL       _ZN14TriggerCentral16addEventListenerEPFv15trigger_event_ejEPKcP6Engine
//   65 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock5
//   66 
//   67 uint32_t triggerHanlderEntryTime;
//   68 
//   69 #if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
//   70 EXTERN_ENGINE
//   71 ;
//   72 
//   73 int triggerReentraint = 0;
//   74 int maxTriggerReentraint = 0;
//   75 uint32_t triggerDuration;
//   76 uint32_t triggerMaxDuration = 0;
//   77 
//   78 extern bool isInsideTriggerHandler;
//   79 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z19hwHandleShaftSignal15trigger_event_e
        THUMB
//   80 void hwHandleShaftSignal(trigger_event_e signal) {
_Z19hwHandleShaftSignal15trigger_event_e:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   81 	triggerHanlderEntryTime = GET_TIMESTAMP();
        LDR.W    R0,??DataTable13_4
        LDR.W    R1,??DataTable13_5  ;; 0xe0001004
        LDR      R1,[R1, #+0]
        STR      R1,[R0, #+0]
//   82 	isInsideTriggerHandler = true;
        LDR.W    R0,??DataTable13_6
        MOVS     R1,#+1
        STRB     R1,[R0, #+0]
//   83 	if (triggerReentraint > maxTriggerReentraint)
        LDR.W    R0,??DataTable13_7
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable13_8
        LDR      R1,[R1, #+0]
        CMP      R0,R1
        BGE.N    ??hwHandleShaftSignal_0
//   84 		maxTriggerReentraint = triggerReentraint;
        LDR.W    R0,??DataTable13_7
        LDR.W    R1,??DataTable13_8
        LDR      R1,[R1, #+0]
        STR      R1,[R0, #+0]
//   85 	triggerReentraint++;
??hwHandleShaftSignal_0:
        LDR.W    R0,??DataTable13_8
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.W    R1,??DataTable13_8
        STR      R0,[R1, #+0]
//   86 	efiAssertVoid(getRemainingStack(chThdSelf()) > 128, "lowstck#8");
        LDR.W    R0,??DataTable13_9
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+129
        BGE.N    ??hwHandleShaftSignal_1
        LDR.W    R0,??DataTable13_10
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??hwHandleShaftSignal_2
//   87 	triggerCentral.handleShaftSignal(signal PASS_ENGINE_PARAMETER);
??hwHandleShaftSignal_1:
        MOVS     R1,R4
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR.W    R0,??DataTable13_2
          CFI FunCall _ZN14TriggerCentral17handleShaftSignalE15trigger_event_e
        BL       _ZN14TriggerCentral17handleShaftSignalE15trigger_event_e
//   88 	triggerReentraint--;
        LDR.W    R0,??DataTable13_8
        LDR      R0,[R0, #+0]
        SUBS     R0,R0,#+1
        LDR.W    R1,??DataTable13_8
        STR      R0,[R1, #+0]
//   89 	triggerDuration = GET_TIMESTAMP() - triggerHanlderEntryTime;
        LDR.W    R0,??DataTable13_5  ;; 0xe0001004
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable13_4
        LDR      R1,[R1, #+0]
        SUBS     R0,R0,R1
        LDR.W    R1,??DataTable13_11
        STR      R0,[R1, #+0]
//   90 	isInsideTriggerHandler = false;
        LDR.W    R0,??DataTable13_6
        MOVS     R1,#+0
        STRB     R1,[R0, #+0]
//   91 	if (triggerDuration > triggerMaxDuration)
        LDR.W    R0,??DataTable13_12
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable13_11
        LDR      R1,[R1, #+0]
        CMP      R0,R1
        BCS.N    ??hwHandleShaftSignal_3
//   92 		triggerMaxDuration = triggerDuration;
        LDR.W    R0,??DataTable13_12
        LDR.W    R1,??DataTable13_11
        LDR      R1,[R1, #+0]
        STR      R1,[R0, #+0]
//   93 }
??hwHandleShaftSignal_3:
??hwHandleShaftSignal_2:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock6
//   94 #endif /* EFI_PROD_CODE */
//   95 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _ZN14TriggerCentralC1Ev
        THUMB
//   96 TriggerCentral::TriggerCentral() {
_ZN14TriggerCentralC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerStateC1Ev
        BL       _ZN12TriggerStateC1Ev
        ADDS     R0,R4,#+192
          CFI FunCall _ZN16IntListenerArrayC1Ev
        BL       _ZN16IntListenerArrayC1Ev
//   97 	nowNt = 0;
        MOVS     R0,#+0
        MOVS     R1,#+0
        STRD     R0,R1,[R4, #+184]
//   98 	memset(hwEventCounters, 0, sizeof(hwEventCounters));
        MOVS     R2,#+24
        MOVS     R1,#+0
        ADDS     R0,R4,#+316
          CFI FunCall memset
        BL       memset
//   99 	clearCallbacks(&triggerListeneres);
        ADDS     R0,R4,#+192
          CFI FunCall _Z14clearCallbacksP16IntListenerArray
        BL       _Z14clearCallbacksP16IntListenerArray
//  100 }
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock7
//  101 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _ZN14TriggerCentral17getHwEventCounterEi
          CFI NoCalls
        THUMB
//  102 int TriggerCentral::getHwEventCounter(int index) {
//  103 	return hwEventCounters[index];
_ZN14TriggerCentral17getHwEventCounterEi:
        ADDS     R0,R0,R1, LSL #+2
        LDR      R0,[R0, #+316]
        BX       LR               ;; return
          CFI EndBlock cfiBlock8
//  104 }
//  105 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _ZN14TriggerCentral13resetCountersEv
        THUMB
//  106 void TriggerCentral::resetCounters() {
_ZN14TriggerCentral13resetCountersEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  107 	memset(hwEventCounters, 0, sizeof(hwEventCounters));
        MOVS     R2,#+24
        MOVS     R1,#+0
        ADDS     R0,R4,#+316
          CFI FunCall memset
        BL       memset
//  108 	triggerState.resetRunningCounters();
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerState20resetRunningCountersEv
        BL       _ZN12TriggerState20resetRunningCountersEv
//  109 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock9
//  110 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  111 static char shaft_signal_msg_index[15];
shaft_signal_msg_index:
        DS8 16
//  112 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  113 static bool_t isUpEvent[6] = { false, true, false, true, false, true };
isUpEvent:
        DATA
        DC8 0, 1, 0, 1, 0, 1, 0, 0

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  114 static const char *eventId[6] = { CRANK1, CRANK1, CRANK2, CRANK2, CRANK3, CRANK3 };
eventId:
        DATA
        DC32 `?<Constant "t1">`, `?<Constant "t1">`, `?<Constant "t2">`
        DC32 `?<Constant "t2">`, `?<Constant "t3">`, `?<Constant "t3">`
//  115 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z22reportEventToWaveChart15trigger_event_ei
        THUMB
//  116 static ALWAYS_INLINE void reportEventToWaveChart(trigger_event_e ckpSignalType, int index DECLARE_ENGINE_PARAMETER_S) {
_Z22reportEventToWaveChart15trigger_event_ei:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  117 	itoa10(&shaft_signal_msg_index[2], index);
        MOVS     R1,R5
        LDR.W    R0,??DataTable13_13
          CFI FunCall itoa10
        BL       itoa10
//  118 	bool_t isUp = isUpEvent[(int) ckpSignalType];
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        LDR.W    R0,??DataTable13_14
        LDRB     R6,[R4, R0]
//  119 	shaft_signal_msg_index[0] = isUp ? 'u' : 'd';
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+0
        BEQ.N    ??reportEventToWaveChart_0
        MOVS     R0,#+117
        B.N      ??reportEventToWaveChart_1
??reportEventToWaveChart_0:
        MOVS     R0,#+100
??reportEventToWaveChart_1:
        LDR.W    R1,??DataTable13_15
        STRB     R0,[R1, #+0]
//  120 
//  121 	addWaveChartEvent(eventId[(int )ckpSignalType], (char* ) shaft_signal_msg_index);
        LDR.W    R2,??DataTable13_15
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        LDR.W    R0,??DataTable13_16
        LDR      R1,[R0, R4, LSL #+2]
        LDR.W    R0,??DataTable13_1
          CFI FunCall _ZN9WaveChart18addWaveChartEvent3EPKcS1_
        BL       _ZN9WaveChart18addWaveChartEvent3EPKcS1_
//  122 	if (engineConfiguration->useOnlyFrontForTrigger) {
        LDR.W    R0,??DataTable13_17
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+6
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??reportEventToWaveChart_2
//  123 		// let's add the opposite event right away
//  124 		shaft_signal_msg_index[0] = isUp ? 'd' : 'u';
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+0
        BEQ.N    ??reportEventToWaveChart_3
        MOVS     R0,#+100
        B.N      ??reportEventToWaveChart_4
??reportEventToWaveChart_3:
        MOVS     R0,#+117
??reportEventToWaveChart_4:
        LDR.W    R1,??DataTable13_15
        STRB     R0,[R1, #+0]
//  125 		addWaveChartEvent(eventId[(int )ckpSignalType], (char* ) shaft_signal_msg_index);
        LDR.W    R2,??DataTable13_15
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        LDR.W    R0,??DataTable13_16
        LDR      R1,[R0, R4, LSL #+2]
        LDR.W    R0,??DataTable13_1
          CFI FunCall _ZN9WaveChart18addWaveChartEvent3EPKcS1_
        BL       _ZN9WaveChart18addWaveChartEvent3EPKcS1_
//  126 	}
//  127 }
??reportEventToWaveChart_2:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock10
//  128 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _ZN14TriggerCentral17handleShaftSignalE15trigger_event_e
        THUMB
//  129 void TriggerCentral::handleShaftSignal(trigger_event_e signal DECLARE_ENGINE_PARAMETER_S) {
_ZN14TriggerCentral17handleShaftSignalE15trigger_event_e:
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
//  130 	efiAssertVoid(engine!=NULL, "configuration");
        LDR.W    R0,??DataTable13_18
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??handleShaftSignal_0
        LDR.W    R0,??DataTable13_19
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??handleShaftSignal_1
//  131 
//  132 	nowNt = getTimeNowNt();
??handleShaftSignal_0:
          CFI FunCall getTimeNowNt
        BL       getTimeNowNt
        STRD     R0,R1,[R4, #+184]
//  133 
//  134 	efiAssertVoid(engine->engineConfiguration!=NULL, "engineConfiguration");
        LDR.W    R0,??DataTable13_18
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+36]
        CMP      R0,#+0
        BNE.N    ??handleShaftSignal_2
        LDR.W    R0,??DataTable13_20
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??handleShaftSignal_1
//  135 	efiAssertVoid(engine->engineConfiguration2!=NULL, "engineConfiguration2");
??handleShaftSignal_2:
        LDR.W    R0,??DataTable13_18
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+40]
        CMP      R0,#+0
        BNE.N    ??handleShaftSignal_3
        LDR.W    R0,??DataTable13_21
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??handleShaftSignal_1
//  136 
//  137 	engine->onTriggerEvent(nowNt);
??handleShaftSignal_3:
        LDRD     R2,R3,[R4, #+184]
        LDR.W    R0,??DataTable13_18
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN6Engine14onTriggerEventEx
        BL       _ZN6Engine14onTriggerEventEx
//  138 
//  139 #if EFI_HISTOGRAMS && EFI_PROD_CODE
//  140 	int beforeCallback = hal_lld_get_counter_value();
//  141 #endif
//  142 	int eventIndex = (int) signal;
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        MOVS     R6,R5
//  143 	efiAssertVoid(eventIndex >= 0 && eventIndex < HW_EVENT_TYPES, "signal type");
        CMP      R6,#+0
        BMI.N    ??handleShaftSignal_4
        CMP      R6,#+6
        BLT.N    ??handleShaftSignal_5
??handleShaftSignal_4:
        LDR.W    R0,??DataTable13_22
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??handleShaftSignal_1
//  144 	hwEventCounters[eventIndex]++;
??handleShaftSignal_5:
        ADDS     R0,R4,R6, LSL #+2
        LDR      R0,[R0, #+316]
        ADDS     R0,R0,#+1
        ADDS     R1,R4,R6, LSL #+2
        STR      R0,[R1, #+316]
//  145 
//  146 	if (nowNt - previousShaftEventTimeNt > US2NT(US_PER_SECOND_LL)) {
        LDRD     R0,R1,[R4, #+184]
        LDR.W    R2,??DataTable13_23
        LDRD     R8,R9,[R2, #+0]
        SUBS     R0,R0,R8
        SBCS     R1,R1,R9
        LDR.W    R2,??DataTable13_24  ;; 0xa037a01
        MOVS     R3,#+0
        CMP      R1,R3
        BLT.N    ??handleShaftSignal_6
        BGT.N    ??handleShaftSignal_7
        CMP      R0,R2
        BCC.N    ??handleShaftSignal_6
//  147 		/**
//  148 		 * We are here if there is a time gap between now and previous shaft event - that means the engine is not runnig.
//  149 		 * That means we have lost synchronization since the engine is not running :)
//  150 		 */
//  151 		triggerState.shaft_is_synchronized = false;
??handleShaftSignal_7:
        MOVS     R0,#+0
        STRB     R0,[R4, #+4]
//  152 	}
//  153 	previousShaftEventTimeNt = nowNt;
??handleShaftSignal_6:
        LDRD     R0,R1,[R4, #+184]
        LDR.W    R2,??DataTable13_23
        STRD     R0,R1,[R2, #+0]
//  154 
//  155 	/**
//  156 	 * This invocation changes the state of triggerState
//  157 	 */
//  158 	triggerState.decodeTriggerEvent(signal, nowNt PASS_ENGINE_PARAMETER);
        LDRD     R2,R3,[R4, #+184]
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerState18decodeTriggerEventE15trigger_event_ex
        BL       _ZN12TriggerState18decodeTriggerEventE15trigger_event_ex
//  159 
//  160 	if (!triggerState.shaft_is_synchronized) {
        LDRB     R0,[R4, #+4]
        CMP      R0,#+0
        BEQ.N    ??handleShaftSignal_1
//  161 		// we should not propagate event if we do not know where we are
//  162 		return;
//  163 	}
//  164 
//  165 	/**
//  166 	 * If we only have a crank position sensor with four stroke, here we are extending crank revolutions with a 360 degree
//  167 	 * cycle into a four stroke, 720 degrees cycle.
//  168 	 */
//  169 	int triggerIndexForListeners;
//  170 	if (engineConfiguration->operationMode != FOUR_STROKE_CRANK_SENSOR) {
??handleShaftSignal_8:
        LDR.W    R0,??DataTable13_17
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+500]
        CMP      R0,#+1
        BEQ.N    ??handleShaftSignal_9
//  171 		// That's easy - trigger cycle matches engine cycle
//  172 		triggerIndexForListeners = triggerState.getCurrentIndex();
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerState15getCurrentIndexEv
        BL       _ZN12TriggerState15getCurrentIndexEv
        MOVS     R7,R0
        B.N      ??handleShaftSignal_10
//  173 	} else {
//  174 		bool isEven = triggerState.getTotalRevolutionCounter() & 1;
??handleShaftSignal_9:
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerState25getTotalRevolutionCounterEv
        BL       _ZN12TriggerState25getTotalRevolutionCounterEv
        ANDS     R8,R0,#0x1
//  175 
//  176 		triggerIndexForListeners = triggerState.getCurrentIndex() + (isEven ? 0 : TRIGGER_SHAPE(size));
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerState15getCurrentIndexEv
        BL       _ZN12TriggerState15getCurrentIndexEv
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        CMP      R8,#+0
        BEQ.N    ??handleShaftSignal_11
        MOVS     R1,#+0
        B.N      ??handleShaftSignal_12
??handleShaftSignal_11:
        LDR.W    R1,??DataTable13_25
        LDR      R1,[R1, #+0]
??handleShaftSignal_12:
        ADDS     R7,R1,R0
//  177 	}
//  178 	reportEventToWaveChart(signal, triggerIndexForListeners PASS_ENGINE_PARAMETER);
??handleShaftSignal_10:
        MOVS     R1,R7
        MOVS     R0,R5
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z22reportEventToWaveChart15trigger_event_ei
        BL       _Z22reportEventToWaveChart15trigger_event_ei
//  179 
//  180 	if (triggerState.current_index >= TRIGGER_SHAPE(size)) {
        LDR      R0,[R4, #+76]
        LDR.W    R1,??DataTable13_25
        LDR      R1,[R1, #+0]
        CMP      R0,R1
        BCC.N    ??handleShaftSignal_13
//  181 		warning(OBD_PCM_Processor_Fault, "unexpected eventIndex=%d", triggerState.current_index);
        LDR      R2,[R4, #+76]
        LDR.W    R1,??DataTable13_26
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
        B.N      ??handleShaftSignal_14
//  182 	} else {
//  183 
//  184 		/**
//  185 		 * Here we invoke all the listeners - the main engine control logic is inside these listeners
//  186 		 */
//  187 		for (int i = 0; i < triggerListeneres.currentListenersCount; i++) {
??handleShaftSignal_13:
        MOVS     R8,#+0
??handleShaftSignal_15:
        LDR      R0,[R4, #+192]
        CMP      R8,R0
        BGE.N    ??handleShaftSignal_14
//  188 			ShaftPositionListener listener = (ShaftPositionListener) triggerListeneres.callbacks[i];
        ADDS     R0,R4,R8, LSL #+2
        LDR      R9,[R0, #+196]
//  189 			(listener)(signal, triggerIndexForListeners PASS_ENGINE_PARAMETER);
        MOVS     R1,R7
        MOVS     R0,R5
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall
        BLX      R9
//  190 		}
        ADDS     R8,R8,#+1
        B.N      ??handleShaftSignal_15
//  191 
//  192 	}
//  193 #if EFI_HISTOGRAMS && EFI_PROD_CODE
//  194 	int afterCallback = hal_lld_get_counter_value();
//  195 	int diff = afterCallback - beforeCallback;
//  196 	// this counter is only 32 bits so it overflows every minute, let's ignore the value in case of the overflow for simplicity
//  197 	if (diff > 0) {
//  198 		hsAdd(&triggerCallback, diff);
//  199 	}
//  200 #endif /* EFI_HISTOGRAMS */
//  201 }
??handleShaftSignal_14:
??handleShaftSignal_1:
        POP      {R0,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock11
//  202 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _Z26printAllCallbacksHistogramv
        THUMB
//  203 void printAllCallbacksHistogram(void) {
_Z26printAllCallbacksHistogramv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  204 #if EFI_PROD_CODE
//  205 	printHistogram(logger, &triggerCallback);
        LDR.W    R1,??DataTable13_27
        LDR.W    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall _Z14printHistogramP7LoggingP11histogram_s
        BL       _Z14printHistogramP7LoggingP11histogram_s
//  206 #endif
//  207 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock12
//  208 
//  209 EXTERN_ENGINE
//  210 ;
//  211 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z16triggerShapeInfov
        THUMB
//  212 static void triggerShapeInfo(void) {
_Z16triggerShapeInfov:
        PUSH     {R4,R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+12
        SUB      SP,SP,#+12
          CFI CFA R13+24
//  213 #if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
//  214 	TriggerShape *s = &engine->triggerShape;
        LDR.W    R0,??DataTable13_18
        LDR      R0,[R0, #+0]
        ADDS     R4,R0,#+644
//  215 	scheduleMsg(logger, "useRise=%s", boolToString(s->useRiseEdge));
        LDRB     R0,[R4, #+2916]
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R2,R0
        LDR.W    R1,??DataTable13_29
        LDR.W    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  216 	scheduleMsg(logger, "gap from %f to %f", s->syncRatioFrom, s->syncRatioTo);
        LDR      R0,[R4, #+2908]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        LDR      R0,[R4, #+2904]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable13_30
        LDR.W    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  217 
//  218 	for (int i = 0; i < s->getSize(); i++) {
        MOVS     R5,#+0
??triggerShapeInfo_0:
        MOVS     R0,R4
          CFI FunCall _ZNK12TriggerShape7getSizeEv
        BL       _ZNK12TriggerShape7getSizeEv
        CMP      R5,R0
        BGE.N    ??triggerShapeInfo_1
//  219 		scheduleMsg(logger, "event %d %f", i, s->eventAngles[i]);
        ADDS     R0,R4,R5, LSL #+2
        LDR      R0,[R0, #+2956]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        MOVS     R2,R5
        LDR.W    R1,??DataTable13_31
        LDR.W    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  220 	}
        ADDS     R5,R5,#+1
        B.N      ??triggerShapeInfo_0
//  221 #endif
//  222 }
??triggerShapeInfo_1:
        POP      {R0-R2,R4,R5,PC}  ;; return
          CFI EndBlock cfiBlock13
//  223 
//  224 #if EFI_UNIT_TEST || defined(__DOXYGEN__)
//  225 #include <stdio.h>
//  226 #include <stdlib.h>
//  227 
//  228 
//  229 /**
//  230  * This is used to generate trigger info which is later used by TODO to generate images for documentation
//  231  */
//  232 void printAllTriggers() {
//  233 
//  234 	FILE * fp = fopen ("triggers.txt", "w+");
//  235 
//  236 	for (int triggerId = 1; triggerId < TT_UNUSED; triggerId++) {
//  237 		trigger_type_e tt = (trigger_type_e) triggerId;
//  238 
//  239 		printf("Exporting %s\r\n", getTrigger_type_e(tt));
//  240 
//  241 		persistent_config_s pc;
//  242 		Engine e(&pc);
//  243 		Engine *engine = &e;
//  244 		persistent_config_s *config = &pc;
//  245 		engine_configuration_s *engineConfiguration = &pc.engineConfiguration;
//  246 		board_configuration_s *boardConfiguration = &engineConfiguration->bc;
//  247 
//  248 		engineConfiguration->engineCycle = 720;
//  249 		engineConfiguration->trigger.type = tt;
//  250 		engineConfiguration->operationMode = FOUR_STROKE_CAM_SENSOR;
//  251 
//  252 		TriggerShape *s = &engine->triggerShape;
//  253 		s->initializeTriggerShape(NULL PASS_ENGINE_PARAMETER);
//  254 
//  255 		fprintf(fp, "TRIGGERTYPE %d %d %s\r\n", triggerId, s->getSize(), getTrigger_type_e(tt));
//  256 		for (int i = 0; i < s->getSize(); i++) {
//  257 
//  258 			int triggerDefinitionCoordinate = (s->getTriggerShapeSynchPointIndex() + i) % s->getLength();
//  259 
//  260 
//  261 			fprintf(fp, "event %d %d %f\r\n", i, s->events[triggerDefinitionCoordinate], s->eventAngles[i]);
//  262 		}
//  263 
//  264 	}
//  265 	fclose(fp);
//  266 	printf("All triggers exported\r\n");
//  267 }
//  268 
//  269 #endif
//  270 
//  271 
//  272 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//  273 extern PwmConfig triggerSignal;
//  274 #endif /* #if EFI_PROD_CODE */
//  275 
//  276 extern uint32_t maxLockTime;
//  277 extern uint32_t maxEventQueueTime;
//  278 extern uint32_t hipLastExecutionCount;
//  279 extern uint32_t hwSetTimerTime;
//  280 
//  281 extern int maxHowFarOff;
//  282 extern uint32_t *cyccnt;
//  283 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _Z11triggerInfov
        THUMB
//  284 void triggerInfo(void) {
_Z11triggerInfov:
        PUSH     {R4-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+20
        SUB      SP,SP,#+20
          CFI CFA R13+40
//  285 #if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
//  286 
//  287 	TriggerShape *ts = &engine->triggerShape;
        LDR.W    R0,??DataTable13_18
        LDR      R0,[R0, #+0]
        ADDS     R4,R0,#+644
//  288 
//  289 	scheduleMsg(logger, "Template %s (%d) trigger %s (%d) useRiseEdge=%s onlyFront=%s",
//  290 			getConfigurationName(engineConfiguration->engineType), engineConfiguration->engineType,
//  291 			getTrigger_type_e(engineConfiguration->trigger.type), engineConfiguration->trigger.type,
//  292 			boolToString(TRIGGER_SHAPE(useRiseEdge)), boolToString(engineConfiguration->useOnlyFrontForTrigger));
        LDR.W    R0,??DataTable13_17
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+6
        ANDS     R0,R0,#0x1
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R5,R0
        LDR.W    R0,??DataTable13_32
        LDRB     R0,[R0, #+3560]
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R7,R0
        LDR.W    R0,??DataTable13_17
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+536]
          CFI FunCall _Z17getTrigger_type_e14trigger_type_e
        BL       _Z17getTrigger_type_e14trigger_type_e
        MOVS     R6,R0
        LDR.W    R0,??DataTable13_17
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20getConfigurationName13engine_type_e
        BL       _Z20getConfigurationName13engine_type_e
        STR      R5,[SP, #+12]
        STR      R7,[SP, #+8]
        LDR.W    R1,??DataTable13_17
        LDR      R1,[R1, #+0]
        LDR      R1,[R1, #+536]
        STR      R1,[SP, #+4]
        STR      R6,[SP, #+0]
        LDR.W    R1,??DataTable13_17
        LDR      R1,[R1, #+0]
        LDR      R3,[R1, #+0]
        MOVS     R2,R0
        LDR.W    R1,??DataTable13_33
        LDR.W    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  293 
//  294 	if (engineConfiguration->trigger.type == TT_TOOTHED_WHEEL) {
        LDR.W    R0,??DataTable13_17
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+536]
        CMP      R0,#+0
        BNE.N    ??triggerInfo_0
//  295 		scheduleMsg(logger, "total %d/skipped %d", engineConfiguration->trigger.customTotalToothCount,
//  296 				engineConfiguration->trigger.customSkippedToothCount);
        LDR.W    R0,??DataTable13_17
        LDR      R0,[R0, #+0]
        LDR      R3,[R0, #+548]
        LDR.W    R0,??DataTable13_17
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+544]
        LDR.W    R1,??DataTable13_34
        LDR.W    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  297 	}
//  298 
//  299 	scheduleMsg(logger, "trigger#1 event counters up=%d/down=%d", triggerCentral.getHwEventCounter(0),
//  300 			triggerCentral.getHwEventCounter(1));
??triggerInfo_0:
        MOVS     R1,#+1
        LDR.W    R0,??DataTable13_2
          CFI FunCall _ZN14TriggerCentral17getHwEventCounterEi
        BL       _ZN14TriggerCentral17getHwEventCounterEi
        MOVS     R5,R0
        MOVS     R1,#+0
        LDR.W    R0,??DataTable13_2
          CFI FunCall _ZN14TriggerCentral17getHwEventCounterEi
        BL       _ZN14TriggerCentral17getHwEventCounterEi
        MOVS     R2,R0
        MOVS     R3,R5
        LDR.W    R1,??DataTable13_35
        LDR.W    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  301 
//  302 	if (engine->triggerShape.needSecondTriggerInput) {
        LDR.W    R0,??DataTable13_18
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+645]
        CMP      R0,#+0
        BEQ.N    ??triggerInfo_1
//  303 		scheduleMsg(logger, "trigger#2 event counters up=%d/down=%d", triggerCentral.getHwEventCounter(2),
//  304 				triggerCentral.getHwEventCounter(3));
        MOVS     R1,#+3
        LDR.W    R0,??DataTable13_2
          CFI FunCall _ZN14TriggerCentral17getHwEventCounterEi
        BL       _ZN14TriggerCentral17getHwEventCounterEi
        MOVS     R5,R0
        MOVS     R1,#+2
        LDR.W    R0,??DataTable13_2
          CFI FunCall _ZN14TriggerCentral17getHwEventCounterEi
        BL       _ZN14TriggerCentral17getHwEventCounterEi
        MOVS     R3,R5
        MOVS     R2,R0
        LDR.W    R1,??DataTable13_36
        LDR.W    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  305 	}
//  306 	scheduleMsg(logger, "expected cycle events %d/%d/%d", ts->expectedEventCount[0],
//  307 			engine->triggerShape.expectedEventCount[1], ts->expectedEventCount[2]);
??triggerInfo_1:
        LDR      R0,[R4, #+2928]
        STR      R0,[SP, #+0]
        LDR.W    R0,??DataTable13_18
        LDR      R0,[R0, #+0]
        LDR      R3,[R0, #+3568]
        LDR      R2,[R4, #+2920]
        LDR.W    R1,??DataTable13_37
        LDR.W    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  308 
//  309 	scheduleMsg(logger, "trigger type=%d/need2ndChannel=%s", engineConfiguration->trigger.type,
//  310 			boolToString(engine->triggerShape.needSecondTriggerInput));
        LDR.W    R0,??DataTable13_18
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+645]
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R3,R0
        LDR.N    R0,??DataTable13_17
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+536]
        LDR.W    R1,??DataTable13_38
        LDR.W    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  311 	scheduleMsg(logger, "expected duty #0=%f/#1=%f", ts->dutyCycle[0], ts->dutyCycle[1]);
        LDR      R0,[R4, #+16]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        LDR      R0,[R4, #+12]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable13_39
        LDR.N    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  312 
//  313 	scheduleMsg(logger, "synchronizationNeeded=%s/isError=%s/total errors=%d ord_err=%d/total revolutions=%d/self=%s",
//  314 			boolToString(ts->isSynchronizationNeeded),
//  315 			boolToString(isTriggerDecoderError()), triggerCentral.triggerState.totalTriggerErrorCounter,
//  316 			triggerCentral.triggerState.orderingErrorCounter, triggerCentral.triggerState.getTotalRevolutionCounter(),
//  317 			boolToString(engineConfiguration->directSelfStimulation));
        LDR.N    R0,??DataTable13_17
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+80]
        ANDS     R0,R0,#0x1
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R6,R0
        LDR.N    R0,??DataTable13_2
          CFI FunCall _ZN12TriggerState25getTotalRevolutionCounterEv
        BL       _ZN12TriggerState25getTotalRevolutionCounterEv
        MOVS     R7,R0
          CFI FunCall _Z21isTriggerDecoderErrorv
        BL       _Z21isTriggerDecoderErrorv
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R5,R0
        LDRB     R0,[R4, #+0]
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        STR      R6,[SP, #+12]
        STR      R7,[SP, #+8]
        LDR.N    R1,??DataTable13_2
        LDR      R1,[R1, #+68]
        STR      R1,[SP, #+4]
        LDR.N    R1,??DataTable13_2
        LDR      R1,[R1, #+60]
        STR      R1,[SP, #+0]
        MOVS     R3,R5
        MOVS     R2,R0
        LDR.N    R1,??DataTable13_40
        LDR.N    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  318 
//  319 	if (ts->isSynchronizationNeeded) {
        LDRB     R0,[R4, #+0]
        CMP      R0,#+0
        BEQ.N    ??triggerInfo_2
//  320 		scheduleMsg(logger, "gap from %f to %f", ts->syncRatioFrom, ts->syncRatioTo);
        LDR      R0,[R4, #+2908]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        LDR      R0,[R4, #+2904]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable13_30
        LDR.N    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  321 	}
//  322 
//  323 #endif
//  324 
//  325 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//  326 
//  327 	scheduleMsg(logger, "primary trigger input: %s", hwPortname(boardConfiguration->triggerInputPins[0]));
??triggerInfo_2:
        LDR.N    R0,??DataTable13_41
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+296]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R2,R0
        LDR.N    R1,??DataTable13_42
        LDR.N    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  328 	scheduleMsg(logger, "primary trigger simulator: %s %s freq=%d",
//  329 			hwPortname(boardConfiguration->triggerSimulatorPins[0]),
//  330 			getPin_output_mode_e(boardConfiguration->triggerSimulatorPinModes[0]),
//  331 			boardConfiguration->triggerSimulatorFrequency);
        LDR.N    R0,??DataTable13_41
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+364]
          CFI FunCall _Z20getPin_output_mode_e17pin_output_mode_e
        BL       _Z20getPin_output_mode_e17pin_output_mode_e
        MOVS     R5,R0
        LDR.N    R0,??DataTable13_41
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+352]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        LDR.N    R1,??DataTable13_41
        LDR      R1,[R1, #+0]
        LDR      R1,[R1, #+208]
        STR      R1,[SP, #+0]
        MOVS     R3,R5
        MOVS     R2,R0
        LDR.N    R1,??DataTable13_43
        LDR.N    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  332 
//  333 	if (engine->triggerShape.needSecondTriggerInput) {
        LDR.N    R0,??DataTable13_18
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+645]
        CMP      R0,#+0
        BEQ.N    ??triggerInfo_3
//  334 		scheduleMsg(logger, "secondary trigger input: %s", hwPortname(boardConfiguration->triggerInputPins[1]));
        LDR.N    R0,??DataTable13_41
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+300]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R2,R0
        LDR.N    R1,??DataTable13_44
        LDR.N    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  335 #if EFI_EMULATE_POSITION_SENSORS || defined(__DOXYGEN__)
//  336 		scheduleMsg(logger, "secondary trigger simulator: %s %s phase=%d",
//  337 				hwPortname(boardConfiguration->triggerSimulatorPins[1]),
//  338 				getPin_output_mode_e(boardConfiguration->triggerSimulatorPinModes[1]), triggerSignal.safe.phaseIndex);
        LDR.N    R0,??DataTable13_41
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+368]
          CFI FunCall _Z20getPin_output_mode_e17pin_output_mode_e
        BL       _Z20getPin_output_mode_e17pin_output_mode_e
        MOVS     R5,R0
        LDR.N    R0,??DataTable13_41
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+356]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        LDR.N    R1,??DataTable13_45
        LDR      R1,[R1, #+72]
        STR      R1,[SP, #+0]
        MOVS     R3,R5
        MOVS     R2,R0
        LDR.N    R1,??DataTable13_46
        LDR.N    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  339 #endif /* EFI_EMULATE_POSITION_SENSORS */
//  340 	}
//  341 //	scheduleMsg(logger, "3rd trigger simulator: %s %s", hwPortname(boardConfiguration->triggerSimulatorPins[2]),
//  342 //			getPin_output_mode_e(boardConfiguration->triggerSimulatorPinModes[2]));
//  343 
//  344 	scheduleMsg(logger, "trigger error extra LED: %s %s", hwPortname(boardConfiguration->triggerErrorPin),
//  345 			getPin_output_mode_e(boardConfiguration->triggerErrorPinMode));
??triggerInfo_3:
        LDR.N    R0,??DataTable13_41
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+604]
          CFI FunCall _Z20getPin_output_mode_e17pin_output_mode_e
        BL       _Z20getPin_output_mode_e17pin_output_mode_e
        MOVS     R5,R0
        LDR.N    R0,??DataTable13_41
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+600]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R3,R5
        MOVS     R2,R0
        LDR.N    R1,??DataTable13_47
        LDR.N    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  346 	scheduleMsg(logger, "primary logic input: %s", hwPortname(boardConfiguration->logicAnalyzerPins[0]));
        LDR.N    R0,??DataTable13_41
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+388]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R2,R0
        LDR.N    R1,??DataTable13_48
        LDR.N    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  347 	scheduleMsg(logger, "secondary logic input: %s", hwPortname(boardConfiguration->logicAnalyzerPins[1]));
        LDR.N    R0,??DataTable13_41
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+392]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R2,R0
        LDR.N    R1,??DataTable13_49
        LDR.N    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  348 
//  349 	scheduleMsg(logger, "zeroTestTime=%d maxHowFarOff=%d", engine->m.zeroTestTime, maxHowFarOff);
        LDR.N    R0,??DataTable13_50
        LDR      R3,[R0, #+0]
        MOVW     R0,#+9188
        LDR.N    R1,??DataTable13_18
        LDR      R1,[R1, #+0]
        LDR      R2,[R0, R1]
        LDR.N    R1,??DataTable13_51
        LDR.N    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  350 	maxHowFarOff = 0;
        LDR.N    R0,??DataTable13_50
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  351 
//  352 	scheduleMsg(logger, "advanceLookupTime=%d now=%d fuelCalcTime=%d",
//  353 			engine->m.advanceLookupTime, *cyccnt,
//  354 			engine->m.fuelCalcTime);
        MOVW     R0,#+9204
        LDR.N    R1,??DataTable13_18
        LDR      R1,[R1, #+0]
        LDR      R0,[R0, R1]
        STR      R0,[SP, #+0]
        LDR.N    R0,??DataTable13_52
        LDR      R0,[R0, #+0]
        LDR      R3,[R0, #+0]
        MOVW     R0,#+9196
        LDR.N    R1,??DataTable13_18
        LDR      R1,[R1, #+0]
        LDR      R2,[R0, R1]
        LDR.N    R1,??DataTable13_53
        LDR.N    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  355 
//  356 	scheduleMsg(logger,
//  357 			"ignitionMathTime=%d ignitionSchTime=%d injectonSchTime=%d",
//  358 			engine->m.ignitionMathTime, engine->m.ignitionSchTime,
//  359 			engine->m.injectonSchTime);
        MOVW     R0,#+9180
        LDR.N    R1,??DataTable13_18
        LDR      R1,[R1, #+0]
        LDR      R0,[R0, R1]
        STR      R0,[SP, #+0]
        MOVW     R0,#+9172
        LDR.N    R1,??DataTable13_18
        LDR      R1,[R1, #+0]
        LDR      R3,[R0, R1]
        MOVW     R0,#+9164
        LDR.N    R1,??DataTable13_18
        LDR      R1,[R1, #+0]
        LDR      R2,[R0, R1]
        LDR.N    R1,??DataTable13_54
        LDR.N    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  360 
//  361 	scheduleMsg(logger, "mapTime=%d/hipTime=%d/rpmTime=%d/mainTriggerCallbackTime=%d",
//  362 			engine->m.mapAveragingCbTime,
//  363 			engine->m.hipCbTime,
//  364 			engine->m.rpmCbTime,
//  365 			engine->m.mainTriggerCallbackTime);
        MOVW     R0,#+9156
        LDR.N    R1,??DataTable13_18
        LDR      R1,[R1, #+0]
        LDR      R0,[R0, R1]
        STR      R0,[SP, #+4]
        MOVW     R0,#+9228
        LDR.N    R1,??DataTable13_18
        LDR      R1,[R1, #+0]
        LDR      R0,[R0, R1]
        STR      R0,[SP, #+0]
        MOVW     R0,#+9220
        LDR.N    R1,??DataTable13_18
        LDR      R1,[R1, #+0]
        LDR      R3,[R0, R1]
        MOVW     R0,#+9212
        LDR.N    R1,??DataTable13_18
        LDR      R1,[R1, #+0]
        LDR      R2,[R0, R1]
        LDR.N    R1,??DataTable13_55
        LDR.N    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  366 
//  367 	scheduleMsg(logger, "maxLockTime=%d / maxTriggerReentraint=%d", maxLockTime, maxTriggerReentraint);
        LDR.N    R0,??DataTable13_7
        LDR      R3,[R0, #+0]
        LDR.N    R0,??DataTable13_56
        LDR      R2,[R0, #+0]
        LDR.N    R1,??DataTable13_57
        LDR.N    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  368 	maxLockTime = 0;
        LDR.N    R0,??DataTable13_56
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  369 	scheduleMsg(logger, "maxEventQueueTime=%d", maxEventQueueTime);
        LDR.N    R0,??DataTable13_58
        LDR      R2,[R0, #+0]
        LDR.N    R1,??DataTable13_59
        LDR.N    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  370 
//  371 	scheduleMsg(logger, "hipLastExecutionCount=%d", hipLastExecutionCount);
        LDR.N    R0,??DataTable13_60
        LDR      R2,[R0, #+0]
        LDR.N    R1,??DataTable13_61
        LDR.N    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  372 	scheduleMsg(logger, "hwSetTimerTime %d", hwSetTimerTime);
        LDR.N    R0,??DataTable13_62
        LDR      R2,[R0, #+0]
        LDR.N    R1,??DataTable13_63
        LDR.N    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  373 
//  374 	scheduleMsg(logger, "totalTriggerHandlerMaxTime=%d", triggerMaxDuration);
        LDR.N    R0,??DataTable13_12
        LDR      R2,[R0, #+0]
        LDR.N    R1,??DataTable13_64
        LDR.N    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  375 	triggerMaxDuration = 0;
        LDR.N    R0,??DataTable13_12
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  376 
//  377 #endif /* EFI_PROD_CODE */
//  378 }
        ADD      SP,SP,#+20
          CFI CFA R13+20
        POP      {R4-R7,PC}       ;; return
          CFI EndBlock cfiBlock14
//  379 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function _Z19getTriggerDutyCyclei
        THUMB
//  380 float getTriggerDutyCycle(int index) {
_Z19getTriggerDutyCyclei:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  381 	return triggerCentral.triggerState.getTriggerDutyCycle(index);
        MOVS     R1,R4
        LDR.N    R0,??DataTable13_2
          CFI FunCall _ZN12TriggerState19getTriggerDutyCycleEi
        BL       _ZN12TriggerState19getTriggerDutyCycleEi
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock15
//  382 }
//  383 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function _Z27resetRunningTriggerCountersv
        THUMB
//  384 static void resetRunningTriggerCounters() {
_Z27resetRunningTriggerCountersv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  385 	triggerCentral.resetCounters();
        LDR.N    R0,??DataTable13_2
          CFI FunCall _ZN14TriggerCentral13resetCountersEv
        BL       _ZN14TriggerCentral13resetCountersEv
//  386 #if EFI_PROD_CODE
//  387 	triggerInfo();
          CFI FunCall _Z11triggerInfov
        BL       _Z11triggerInfov
//  388 #endif
//  389 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock16

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function _Z18initTriggerCentralP7LoggingP6Engine
        THUMB
_Z18initTriggerCentralP7LoggingP6Engine:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        LDR.N    R0,??DataTable13_28
        STR      R4,[R0, #+0]
        ADR.N    R1,??DataTable13  ;; 0x78, 0x5F, 0x00, 0x00
        LDR.N    R0,??DataTable13_15
          CFI FunCall strcpy
        BL       strcpy
        LDR.N    R0,??DataTable13_1
          CFI FunCall _Z13initWaveChartP9WaveChart
        BL       _Z13initWaveChartP9WaveChart
        LDR.N    R1,??DataTable13_65
        LDR.N    R0,??DataTable13_66
          CFI FunCall addConsoleAction
        BL       addConsoleAction
        LDR.N    R1,??DataTable13_67
        LDR.N    R0,??DataTable13_68
          CFI FunCall addConsoleAction
        BL       addConsoleAction
        LDR.N    R1,??DataTable13_69
        LDR.N    R0,??DataTable13_70
          CFI FunCall addConsoleAction
        BL       addConsoleAction
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock17

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13:
        DC8      0x78, 0x5F, 0x00, 0x00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_1:
        DC32     waveChart

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_2:
        DC32     triggerCentral

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_3:
        DC32     `?<Constant "registerCkpListener: ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_4:
        DC32     triggerHanlderEntryTime

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_5:
        DC32     0xe0001004

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_6:
        DC32     isInsideTriggerHandler

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_7:
        DC32     maxTriggerReentraint

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_8:
        DC32     triggerReentraint

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_9:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_10:
        DC32     `?<Constant "lowstck#8">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_11:
        DC32     triggerDuration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_12:
        DC32     triggerMaxDuration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_13:
        DC32     shaft_signal_msg_index+0x2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_14:
        DC32     isUpEvent

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_15:
        DC32     shaft_signal_msg_index

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_16:
        DC32     eventId

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_17:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_18:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_19:
        DC32     `?<Constant "configuration">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_20:
        DC32     `?<Constant "engineConfiguration">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_21:
        DC32     `?<Constant "engineConfiguration2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_22:
        DC32     `?<Constant "signal type">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_23:
        DC32     previousShaftEventTimeNt

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_24:
        DC32     0xa037a01

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_25:
        DC32     _engine+0x16F0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_26:
        DC32     `?<Constant "unexpected eventIndex=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_27:
        DC32     triggerCallback

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_28:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_29:
        DC32     `?<Constant "useRise=%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_30:
        DC32     `?<Constant "gap from %f to %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_31:
        DC32     `?<Constant "event %d %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_32:
        DC32     _engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_33:
        DC32     `?<Constant "Template %s (%d) trig...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_34:
        DC32     `?<Constant "total %d/skipped %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_35:
        DC32     `?<Constant "trigger#1 event count...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_36:
        DC32     `?<Constant "trigger#2 event count...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_37:
        DC32     `?<Constant "expected cycle events...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_38:
        DC32     `?<Constant "trigger type=%d/need2...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_39:
        DC32     `?<Constant "expected duty #0=%f/#...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_40:
        DC32     `?<Constant "synchronizationNeeded...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_41:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_42:
        DC32     `?<Constant "primary trigger input...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_43:
        DC32     `?<Constant "primary trigger simul...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_44:
        DC32     `?<Constant "secondary trigger inp...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_45:
        DC32     triggerSignal

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_46:
        DC32     `?<Constant "secondary trigger sim...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_47:
        DC32     `?<Constant "trigger error extra L...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_48:
        DC32     `?<Constant "primary logic input: %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_49:
        DC32     `?<Constant "secondary logic input...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_50:
        DC32     maxHowFarOff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_51:
        DC32     `?<Constant "zeroTestTime=%d maxHo...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_52:
        DC32     cyccnt

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_53:
        DC32     `?<Constant "advanceLookupTime=%d ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_54:
        DC32     `?<Constant "ignitionMathTime=%d i...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_55:
        DC32     `?<Constant "mapTime=%d/hipTime=%d...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_56:
        DC32     maxLockTime

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_57:
        DC32     `?<Constant "maxLockTime=%d / maxT...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_58:
        DC32     maxEventQueueTime

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_59:
        DC32     `?<Constant "maxEventQueueTime=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_60:
        DC32     hipLastExecutionCount

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_61:
        DC32     `?<Constant "hipLastExecutionCount=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_62:
        DC32     hwSetTimerTime

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_63:
        DC32     `?<Constant "hwSetTimerTime %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_64:
        DC32     `?<Constant "totalTriggerHandlerMa...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_65:
        DC32     _Z11triggerInfov

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_66:
        DC32     `?<Constant "triggerinfo">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_67:
        DC32     _Z16triggerShapeInfov

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_68:
        DC32     `?<Constant "trigger_shape_info">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_69:
        DC32     _Z27resetRunningTriggerCountersv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_70:
        DC32     `?<Constant "reset_trigger">`

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memset
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function memset
        THUMB
// __intrinsic __nounwind __interwork __softfp void *memset(void *, int, size_t)
memset:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R2,R5
        MOVS     R1,R6
        MOVS     R0,R4
          CFI FunCall __aeabi_memset
        BL       __aeabi_memset
        MOVS     R0,R4
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock18

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
`?<Constant "t1">`:
        DATA
        DC8 "t1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "t2">`:
        DATA
        DC8 "t2"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "t3">`:
        DATA
        DC8 "t3"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lowstck#8">`:
        DATA
        DC8 "lowstck#8"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "useRise=%s">`:
        DATA
        DC8 "useRise=%s"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "gap from %f to %f">`:
        DATA
        DC8 "gap from %f to %f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "event %d %f">`:
        DATA
        DC8 "event %d %f"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Template %s (%d) trig...">`:
        DATA
        DC8 54H, 65H, 6DH, 70H, 6CH, 61H, 74H, 65H
        DC8 20H, 25H, 73H, 20H, 28H, 25H, 64H, 29H
        DC8 20H, 74H, 72H, 69H, 67H, 67H, 65H, 72H
        DC8 20H, 25H, 73H, 20H, 28H, 25H, 64H, 29H
        DC8 20H, 75H, 73H, 65H, 52H, 69H, 73H, 65H
        DC8 45H, 64H, 67H, 65H, 3DH, 25H, 73H, 20H
        DC8 6FH, 6EH, 6CH, 79H, 46H, 72H, 6FH, 6EH
        DC8 74H, 3DH, 25H, 73H, 0
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "total %d/skipped %d">`:
        DATA
        DC8 "total %d/skipped %d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "trigger#1 event count...">`:
        DATA
        DC8 "trigger#1 event counters up=%d/down=%d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "trigger#2 event count...">`:
        DATA
        DC8 "trigger#2 event counters up=%d/down=%d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "expected cycle events...">`:
        DATA
        DC8 "expected cycle events %d/%d/%d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "trigger type=%d/need2...">`:
        DATA
        DC8 "trigger type=%d/need2ndChannel=%s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "expected duty #0=%f/#...">`:
        DATA
        DC8 "expected duty #0=%f/#1=%f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "synchronizationNeeded...">`:
        DATA
        DC8 73H, 79H, 6EH, 63H, 68H, 72H, 6FH, 6EH
        DC8 69H, 7AH, 61H, 74H, 69H, 6FH, 6EH, 4EH
        DC8 65H, 65H, 64H, 65H, 64H, 3DH, 25H, 73H
        DC8 2FH, 69H, 73H, 45H, 72H, 72H, 6FH, 72H
        DC8 3DH, 25H, 73H, 2FH, 74H, 6FH, 74H, 61H
        DC8 6CH, 20H, 65H, 72H, 72H, 6FH, 72H, 73H
        DC8 3DH, 25H, 64H, 20H, 6FH, 72H, 64H, 5FH
        DC8 65H, 72H, 72H, 3DH, 25H, 64H, 2FH, 74H
        DC8 6FH, 74H, 61H, 6CH, 20H, 72H, 65H, 76H
        DC8 6FH, 6CH, 75H, 74H, 69H, 6FH, 6EH, 73H
        DC8 3DH, 25H, 64H, 2FH, 73H, 65H, 6CH, 66H
        DC8 3DH, 25H, 73H, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "primary trigger input...">`:
        DATA
        DC8 "primary trigger input: %s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "primary trigger simul...">`:
        DATA
        DC8 "primary trigger simulator: %s %s freq=%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "secondary trigger inp...">`:
        DATA
        DC8 "secondary trigger input: %s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "secondary trigger sim...">`:
        DATA
        DC8 "secondary trigger simulator: %s %s phase=%d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "trigger error extra L...">`:
        DATA
        DC8 "trigger error extra LED: %s %s"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "primary logic input: %s">`:
        DATA
        DC8 "primary logic input: %s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "secondary logic input...">`:
        DATA
        DC8 "secondary logic input: %s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "zeroTestTime=%d maxHo...">`:
        DATA
        DC8 "zeroTestTime=%d maxHowFarOff=%d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "advanceLookupTime=%d ...">`:
        DATA
        DC8 "advanceLookupTime=%d now=%d fuelCalcTime=%d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ignitionMathTime=%d i...">`:
        DATA
        DC8 69H, 67H, 6EH, 69H, 74H, 69H, 6FH, 6EH
        DC8 4DH, 61H, 74H, 68H, 54H, 69H, 6DH, 65H
        DC8 3DH, 25H, 64H, 20H, 69H, 67H, 6EH, 69H
        DC8 74H, 69H, 6FH, 6EH, 53H, 63H, 68H, 54H
        DC8 69H, 6DH, 65H, 3DH, 25H, 64H, 20H, 69H
        DC8 6EH, 6AH, 65H, 63H, 74H, 6FH, 6EH, 53H
        DC8 63H, 68H, 54H, 69H, 6DH, 65H, 3DH, 25H
        DC8 64H, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "mapTime=%d/hipTime=%d...">`:
        DATA
        DC8 6DH, 61H, 70H, 54H, 69H, 6DH, 65H, 3DH
        DC8 25H, 64H, 2FH, 68H, 69H, 70H, 54H, 69H
        DC8 6DH, 65H, 3DH, 25H, 64H, 2FH, 72H, 70H
        DC8 6DH, 54H, 69H, 6DH, 65H, 3DH, 25H, 64H
        DC8 2FH, 6DH, 61H, 69H, 6EH, 54H, 72H, 69H
        DC8 67H, 67H, 65H, 72H, 43H, 61H, 6CH, 6CH
        DC8 62H, 61H, 63H, 6BH, 54H, 69H, 6DH, 65H
        DC8 3DH, 25H, 64H, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "maxLockTime=%d / maxT...">`:
        DATA
        DC8 "maxLockTime=%d / maxTriggerReentraint=%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "maxEventQueueTime=%d">`:
        DATA
        DC8 "maxEventQueueTime=%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "hipLastExecutionCount=%d">`:
        DATA
        DC8 "hipLastExecutionCount=%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "hwSetTimerTime %d">`:
        DATA
        DC8 "hwSetTimerTime %d"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "totalTriggerHandlerMa...">`:
        DATA
        DC8 "totalTriggerHandlerMaxTime=%d"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "x_"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "triggerinfo">`:
        DATA
        DC8 "triggerinfo"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "trigger_shape_info">`:
        DATA
        DC8 "trigger_shape_info"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "reset_trigger">`:
        DATA
        DC8 "reset_trigger"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "registerCkpListener: ...">`:
        DATA
        DC8 "registerCkpListener: %s\015\012"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "configuration">`:
        DATA
        DC8 "configuration"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "engineConfiguration">`:
        DATA
        DC8 "engineConfiguration"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "engineConfiguration2">`:
        DATA
        DC8 "engineConfiguration2"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "signal type">`:
        DATA
        DC8 "signal type"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "unexpected eventIndex=%d">`:
        DATA
        DC8 "unexpected eventIndex=%d"
        DC8 0, 0, 0

        END
//  390 
//  391 void initTriggerCentral(Logging *sharedLogger, Engine *engine) {
//  392 	logger = sharedLogger;
//  393 	strcpy((char*) shaft_signal_msg_index, "x_");
//  394 
//  395 #if EFI_WAVE_CHART
//  396 	initWaveChart(&waveChart);
//  397 #endif /* EFI_WAVE_CHART */
//  398 
//  399 #if EFI_PROD_CODE || EFI_SIMULATOR
//  400 	addConsoleAction("triggerinfo", triggerInfo);
//  401 	addConsoleAction("trigger_shape_info", triggerShapeInfo);
//  402 	addConsoleAction("reset_trigger", resetRunningTriggerCounters);
//  403 #endif
//  404 
//  405 #if EFI_HISTOGRAMS
//  406 	initHistogram(&triggerCallback, "all callbacks");
//  407 #endif /* EFI_HISTOGRAMS */
//  408 }
//  409 
//  410 #endif
// 
// 4 080 bytes in section .bss
//    40 bytes in section .data
//     4 bytes in section .init_array
// 1 172 bytes in section .rodata
// 2 352 bytes in section .text
// 
// 2 334 bytes of CODE  memory (+ 22 bytes shared)
// 1 172 bytes of CONST memory
// 4 120 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
