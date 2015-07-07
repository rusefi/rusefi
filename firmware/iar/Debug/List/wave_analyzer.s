///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:59 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\development\wav /
//                       e_analyzer.cpp                                       /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\development\wav /
//                       e_analyzer.cpp -lCN F:\stuff\rusefi_sourceforge\firm /
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
//                       -On --eec++                                          /
//    MISRA C version =  2004                                                 /
//      Enabled       =  1.2-1.4 2.3 3.4 4.2 6.5 7 8.2 8.3 9.3 12.3 13.6      /
//                       13.7 15.5 17.3 19.17 20.7-20.9 20.11                 /
//      Checked       =  1.4 2.3 4.2 6.5 7 8.2 8.3 9.3 12.3 13.7 15.5         /
//                       20.7-20.9 20.11                                      /
//      Not checked   =  1.1-1.3 1.5-2.2 2.4-4.1 5-6.4 8.1 8.4-9.2 10-12.2    /
//                       12.4-13.6 14-15.4 16-20.6 20.10 20.12 21             /
//    List file       =  F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                       wave_analyzer.s                                      /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME wave_analyzer

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z10hwPortname11brain_pin_e
        EXTERN _Z16startInputDriverP15digital_input_sb
        EXTERN _Z20getRevolutionCounterv
        EXTERN _Z22initWaveAnalyzerDriverPKc11brain_pin_e
        EXTERN _ZN13RpmCalculator3rpmEv
        EXTERN _ZN16IntListenerArray16registerCallbackEPFviEPv
        EXTERN _ZN9WaveChart18addWaveChartEvent3EPKcS1_
        EXTERN _ZN9WaveChart18publishChartIfFullEv
        EXTERN __aeabi_l2f
        EXTERN __aeabi_vec_ctor_nocookie_nodtor
        EXTERN _engine
        EXTERN addConsoleActionII
        EXTERN appendFloat
        EXTERN appendPrintf
        EXTERN boardConfiguration
        EXTERN chThdCreateStatic
        EXTERN chThdSleep
        EXTERN engine
        EXTERN firmwareError
        EXTERN getTimeNowUs
        EXTERN persistentState
        EXTERN print
        EXTERN rlist
        EXTERN waveChart

        PUBLIC _Z16initWaveAnalyzerP7Logging
        PUBLIC _Z9printWaveP7Logging
        PUBLIC _ZN10WaveReader11onFallEventEv
        PUBLIC _ZN10WaveReaderC1Ev
        PUBLIC _ZN10WaveReaderC2Ev
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\development\wave_analyzer.cpp
//    1 /**
//    2  * @file	wave_analyzer.cpp
//    3  * @brief	Initialization of Input Capture pins used for dev console sniffer
//    4  *
//    5  * This file is responsible for sniffing of external digital signals and registering
//    6  * these digital events in WaveChart used by the Sniffer tab of Dev Console.
//    7  *
//    8  *
//    9  * @date Jan 7, 2013
//   10  * @author Andrey Belomutskiy, (c) 2012-2015
//   11  */
//   12 
//   13 #include "main.h"
//   14 #include "wave_analyzer.h"
//   15 #include "eficonsole.h"
//   16 #include "data_buffer.h"
//   17 #include "pin_repository.h"
//   18 #include "engine_state.h"
//   19 #include "signal_executor.h"
//   20 #include "engine_configuration.h"
//   21 #include "trigger_central.h"
//   22 #include "rfiutil.h"
//   23 #include "engine_math.h"
//   24 #include "engine.h"
//   25 #include "rpm_calculator.h"
//   26 #include "wave_chart.h"
//   27 
//   28 #if EFI_WAVE_ANALYZER || defined(__DOXYGEN__)
//   29 
//   30 EXTERN_ENGINE;
//   31 
//   32 #define CHART_RESET_DELAY 1
//   33 #define MAX_ICU_COUNT 5
//   34 
//   35 #if EFI_WAVE_CHART || defined(__DOXYGEN__)
//   36 extern WaveChart waveChart;
//   37 #endif
//   38 extern bool hasFirmwareErrorFlag;
//   39 
//   40 /**
//   41  * Difference between current 1st trigger event and previous 1st trigger event.
//   42  */
//   43 static volatile uint32_t engineCycleDurationUs;

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   44 static volatile efitime_t previousEngineCycleTimeUs = 0;
previousEngineCycleTimeUs:
        DS8 8
//   45 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   46 static int waveReaderCount = 0;
waveReaderCount:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   47 static WaveReader readers[MAX_ICU_COUNT];
readers:
        DS8 440
//   48 

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   49 static THD_WORKING_AREA(waThreadStack, UTILITY_THREAD_STACK_SIZE);
waThreadStack:
        DS8 712

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   50 static Logging * logger;
logger:
        DS8 4

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _ZN10WaveReaderC2Ev
        THUMB
// __code __interwork __softfp WaveReader::subobject WaveReader()
_ZN10WaveReaderC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN10WaveReaderC1Ev
        BL       _ZN10WaveReaderC1Ev
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
        MOVS     R3,#+5
        MOVS     R2,#+88
        ADR.W    R1,_ZN10WaveReaderC1Ev
        LDR.W    R0,??DataTable11_7
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock1
//   51 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z17ensureInitializedP10WaveReader
        THUMB
//   52 static void ensureInitialized(WaveReader *reader) {
_Z17ensureInitializedP10WaveReader:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   53 	efiAssertVoid(reader->hw->started, "wave analyzer NOT INITIALIZED");
        LDR      R0,[R4, #+0]
        LDRB     R0,[R0, #+5]
        CMP      R0,#+0
        BNE.N    ??ensureInitialized_0
        LDR.W    R0,??DataTable11_8
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??ensureInitialized_1
//   54 }
??ensureInitialized_0:
??ensureInitialized_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2
//   55 
//   56 #if EFI_WAVE_ANALYZER || defined(__DOXYGEN__)
//   57 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z18waAnaWidthCallbackP10WaveReader
        THUMB
//   58 static void waAnaWidthCallback(WaveReader *reader) {
_Z18waAnaWidthCallbackP10WaveReader:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R6,R0
//   59 	efitick_t nowUs = getTimeNowUs();
          CFI FunCall getTimeNowUs
        BL       getTimeNowUs
        MOVS     R4,R0
        MOVS     R5,R1
//   60 	reader->eventCounter++;
        LDR      R0,[R6, #+8]
        ADDS     R0,R0,#+1
        STR      R0,[R6, #+8]
//   61 	reader->lastActivityTimeUs = nowUs;
        STRD     R4,R5,[R6, #+32]
//   62 	addWaveChartEvent(reader->name, WC_UP);
        ADR.N    R2,??DataTable9  ;; "u"
        LDR      R1,[R6, #+4]
        LDR.W    R0,??DataTable11_9
          CFI FunCall _ZN9WaveChart18addWaveChartEvent3EPKcS1_
        BL       _ZN9WaveChart18addWaveChartEvent3EPKcS1_
//   63 
//   64 	uint32_t width = nowUs - reader->periodEventTimeUs;
        LDRD     R0,R1,[R6, #+40]
        SUBS     R0,R4,R0
//   65 	reader->last_wave_low_widthUs = width;
        MOVS     R1,#+0
        STRD     R0,R1,[R6, #+72]
//   66 
//   67 	reader->signalPeriodUs = nowUs - reader->widthEventTimeUs;
        LDRD     R2,R3,[R6, #+48]
        SUBS     R2,R4,R2
        SBCS     R3,R5,R3
        STRD     R2,R3,[R6, #+56]
//   68 	reader->widthEventTimeUs = nowUs;
        STRD     R4,R5,[R6, #+48]
//   69 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock3
//   70 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _ZN10WaveReader11onFallEventEv
        THUMB
//   71 void WaveReader::onFallEvent() {
_ZN10WaveReader11onFallEventEv:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOV      R8,R0
//   72 	efitick_t nowUs = getTimeNowUs();
          CFI FunCall getTimeNowUs
        BL       getTimeNowUs
        MOVS     R4,R0
        MOVS     R5,R1
//   73 	eventCounter++;
        LDR      R0,[R8, #+8]
        ADDS     R0,R0,#+1
        STR      R0,[R8, #+8]
//   74 	lastActivityTimeUs = nowUs;
        STRD     R4,R5,[R8, #+32]
//   75 	addWaveChartEvent(name, WC_DOWN);
        ADR.N    R2,??DataTable9_1  ;; "d"
        LDR      R1,[R8, #+4]
        LDR.W    R0,??DataTable11_9
          CFI FunCall _ZN9WaveChart18addWaveChartEvent3EPKcS1_
        BL       _ZN9WaveChart18addWaveChartEvent3EPKcS1_
//   76 
//   77 	efitick_t width = nowUs - widthEventTimeUs;
        LDRD     R0,R1,[R8, #+48]
        SUBS     R6,R4,R0
        SBCS     R7,R5,R1
//   78 	last_wave_high_widthUs = width;
        STRD     R6,R7,[R8, #+80]
//   79 
//   80 	int revolutionCounter = getRevolutionCounter();
          CFI FunCall _Z20getRevolutionCounterv
        BL       _Z20getRevolutionCounterv
//   81 
//   82 	totalOnTimeAccumulatorUs += width;
        LDRD     R2,R3,[R8, #+24]
        ADDS     R2,R2,R6
        ADCS     R3,R3,R7
        STRD     R2,R3,[R8, #+24]
//   83 	if (currentRevolutionCounter != revolutionCounter) {
        LDR      R1,[R8, #+12]
        CMP      R1,R0
        BEQ.N    ??onFallEvent_0
//   84 		/**
//   85 		 * We are here in case of a new engine cycle
//   86 		 */
//   87 		currentRevolutionCounter = revolutionCounter;
        STR      R0,[R8, #+12]
//   88 		prevTotalOnTimeUs = totalOnTimeAccumulatorUs;
        LDRD     R2,R3,[R8, #+24]
        STRD     R2,R3,[R8, #+16]
//   89 		totalOnTimeAccumulatorUs = 0;
        MOVS     R2,#+0
        MOVS     R3,#+0
        STRD     R2,R3,[R8, #+24]
//   90 
//   91 		waveOffsetUs = nowUs - previousEngineCycleTimeUs;
        LDR.W    R1,??DataTable11_10
        LDRD     R2,R3,[R1, #+0]
        SUBS     R2,R4,R2
        SBCS     R3,R5,R3
        STRD     R2,R3,[R8, #+64]
//   92 
//   93 	}
//   94 
//   95 	periodEventTimeUs = nowUs;
??onFallEvent_0:
        STRD     R4,R5,[R8, #+40]
//   96 
//   97 //	uint32_t period = engineCycleDurationUs;  // local copy of volatile variable
//   98 }
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock4
//   99 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z19waIcuPeriodCallbackP10WaveReader
        THUMB
//  100 static void waIcuPeriodCallback(WaveReader *reader) {
_Z19waIcuPeriodCallbackP10WaveReader:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  101 	reader->onFallEvent();
        MOVS     R0,R4
          CFI FunCall _ZN10WaveReader11onFallEventEv
        BL       _ZN10WaveReader11onFallEventEv
//  102 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock5
//  103 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z17setWaveModeSilentii
        THUMB
//  104 static void setWaveModeSilent(int index, int mode) {
_Z17setWaveModeSilentii:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  105 	WaveReader *reader = &readers[index];
        MOVS     R0,#+88
        LDR.N    R1,??DataTable11_7
        MLA      R6,R0,R4,R1
//  106 
//  107 	startInputDriver(reader->hw, mode);
        CMP      R5,#+0
        BEQ.N    ??setWaveModeSilent_0
        MOVS     R1,#+1
        B.N      ??setWaveModeSilent_1
??setWaveModeSilent_0:
        MOVS     R1,#+0
??setWaveModeSilent_1:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR      R0,[R6, #+0]
          CFI FunCall _Z16startInputDriverP15digital_input_sb
        BL       _Z16startInputDriverP15digital_input_sb
//  108 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock6
//  109 
//  110 //static int getEventCounter(int index) {
//  111 //	WaveReader *reader = &readers[index];
//  112 //	ensureInitialized(reader);
//  113 //	return reader->eventCounter;
//  114 //}
//  115 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z8initWavePKci
        THUMB
//  116 static void initWave(const char *name, int index) {
_Z8initWavePKci:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  117 	brain_pin_e brainPin = boardConfiguration->logicAnalyzerPins[index];
        LDR.N    R0,??DataTable11_11
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R5, LSL #+2
        LDR      R6,[R0, #+388]
//  118 
//  119 	bool mode = boardConfiguration->logicAnalyzerMode[index];
        LDR.N    R0,??DataTable11_11
        LDR      R0,[R0, #+0]
        ADDS     R0,R5,R0
        LDRB     R0,[R0, #+404]
        CMP      R0,#+0
        BEQ.N    ??initWave_0
        MOVS     R7,#+1
        B.N      ??initWave_1
??initWave_0:
        MOVS     R7,#+0
//  120 
//  121 	waveReaderCount++;
??initWave_1:
        LDR.N    R0,??DataTable11_12
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable11_12
        STR      R0,[R1, #+0]
//  122 	efiAssertVoid(index < MAX_ICU_COUNT, "too many ICUs");
        CMP      R5,#+5
        BLT.N    ??initWave_2
        LDR.N    R0,??DataTable11_13
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??initWave_3
//  123 	WaveReader *reader = &readers[index];
??initWave_2:
        MOVS     R0,#+88
        LDR.N    R1,??DataTable11_7
        MLA      R8,R0,R5,R1
//  124 	reader->name = name;
        STR      R4,[R8, #+4]
//  125 
//  126 	reader->hw = initWaveAnalyzerDriver("wave input", brainPin);
        MOVS     R1,R6
        LDR.N    R0,??DataTable11_14
          CFI FunCall _Z22initWaveAnalyzerDriverPKc11brain_pin_e
        BL       _Z22initWaveAnalyzerDriverPKc11brain_pin_e
        STR      R0,[R8, #+0]
//  127 
//  128 
//  129 	reader->hw->widthListeners.registerCallback((VoidInt) waAnaWidthCallback, (void*) reader);
        MOV      R2,R8
        LDR.N    R1,??DataTable11_15
        LDR      R0,[R8, #+0]
        ADDS     R0,R0,#+8
          CFI FunCall _ZN16IntListenerArray16registerCallbackEPFviEPv
        BL       _ZN16IntListenerArray16registerCallbackEPFviEPv
//  130 
//  131 	reader->hw->periodListeners.registerCallback((VoidInt) waIcuPeriodCallback, (void*) reader);
        MOV      R2,R8
        LDR.N    R1,??DataTable11_16
        LDR      R0,[R8, #+0]
        ADDS     R0,R0,#+132
          CFI FunCall _ZN16IntListenerArray16registerCallbackEPFviEPv
        BL       _ZN16IntListenerArray16registerCallbackEPFviEPv
//  132 
//  133 
//  134 	print("wave%d input on %s\r\n", index, hwPortname(brainPin));
        MOVS     R0,R6
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R2,R0
        MOVS     R1,R5
        LDR.N    R0,??DataTable11_17
          CFI FunCall print
        BL       print
//  135 	startInputDriver(reader->hw, mode);
        MOVS     R1,R7
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR      R0,[R8, #+0]
          CFI FunCall _Z16startInputDriverP15digital_input_sb
        BL       _Z16startInputDriverP15digital_input_sb
//  136 }
??initWave_3:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock7
//  137 #endif
//  138 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _ZN10WaveReaderC1Ev
          CFI NoCalls
        THUMB
//  139 WaveReader::WaveReader() {
//  140 	hw = NULL;
_ZN10WaveReaderC1Ev:
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  141 	last_wave_high_widthUs = 0;
        MOVS     R2,#+0
        MOVS     R3,#+0
        STRD     R2,R3,[R0, #+80]
//  142 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock8
//  143 
//  144 static void waTriggerEventListener(trigger_event_e ckpSignalType, uint32_t index DECLARE_ENGINE_PARAMETER_S) {
//  145 	(void)ckpSignalType;
//  146 	if (index != 0) {
//  147 		return;
//  148 	}
//  149 	efitick_t nowUs = getTimeNowUs();
//  150 	engineCycleDurationUs = nowUs - previousEngineCycleTimeUs;
//  151 	previousEngineCycleTimeUs = nowUs;
//  152 }
//  153 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z8waThreadPv
        THUMB
//  154 static msg_t waThread(void *arg) {
_Z8waThreadPv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  155 	(void)arg;
//  156 	chRegSetThreadName("Wave Analyzer");
        LDR.N    R0,??DataTable11_18
        LDR      R0,[R0, #+24]
        LDR.N    R1,??DataTable11_19
        STR      R1,[R0, #+24]
//  157 #if EFI_WAVE_CHART
//  158 	while (TRUE) {
//  159 		chThdSleepSeconds(CHART_RESET_DELAY);
??waThread_0:
        MOV      R0,#+1000
          CFI FunCall chThdSleep
        BL       chThdSleep
//  160 
//  161 		waveChart.publishChartIfFull();
        LDR.N    R0,??DataTable11_9
          CFI FunCall _ZN9WaveChart18publishChartIfFullEv
        BL       _ZN9WaveChart18publishChartIfFullEv
        B.N      ??waThread_0
          CFI EndBlock cfiBlock9
//  162 	}
//  163 #endif /* EFI_WAVE_CHART */
//  164 #if defined __GNUC__
//  165 	return -1;
//  166 #endif
//  167 }
//  168 
//  169 static uint32_t getWaveLowWidth(int index) {
//  170 	WaveReader *reader = &readers[index];
//  171 	ensureInitialized(reader);
//  172 	return reader->last_wave_low_widthUs;
//  173 }
//  174 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z15getSignalOnTimei
        THUMB
//  175 static float getSignalOnTime(int index) {
_Z15getSignalOnTimei:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  176 	WaveReader *reader = &readers[index];
        MOVS     R0,#+88
        LDR.N    R1,??DataTable11_7
        MLA      R5,R0,R4,R1
//  177 	ensureInitialized(reader);
        MOVS     R0,R5
          CFI FunCall _Z17ensureInitializedP10WaveReader
        BL       _Z17ensureInitializedP10WaveReader
//  178 	if (getTimeNowUs() - reader->lastActivityTimeUs > 4 * US_PER_SECOND) {
          CFI FunCall getTimeNowUs
        BL       getTimeNowUs
        LDRD     R2,R3,[R5, #+32]
        SUBS     R0,R0,R2
        SBCS     R1,R1,R3
        LDR.N    R2,??DataTable11_20  ;; 0x3d0901
        MOVS     R3,#+0
        CMP      R1,R3
        BLT.N    ??getSignalOnTime_0
        BGT.N    ??getSignalOnTime_1
        CMP      R0,R2
        BCC.N    ??getSignalOnTime_0
//  179 		return 0.0f; // dwell time has expired
??getSignalOnTime_1:
        VLDR.W   S0,??DataTable11  ;; 0x0
        B.N      ??getSignalOnTime_2
//  180 	}
//  181 	return reader->last_wave_high_widthUs / 1000.0f;
??getSignalOnTime_0:
        LDRD     R0,R1,[R5, #+80]
          CFI FunCall __aeabi_l2f
        BL       __aeabi_l2f
        VMOV     S0,R0
        VLDR.W   S1,??DataTable11_1  ;; 0x447a0000
        VDIV.F32 S0,S0,S1
??getSignalOnTime_2:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock10
//  182 }
//  183 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z13getWaveOffseti
        THUMB
//  184 static efitime_t getWaveOffset(int index) {
_Z13getWaveOffseti:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  185 	WaveReader *reader = &readers[index];
        MOVS     R0,#+88
        LDR.N    R1,??DataTable11_7
        MLA      R5,R0,R4,R1
//  186 	ensureInitialized(reader);
        MOVS     R0,R5
          CFI FunCall _Z17ensureInitializedP10WaveReader
        BL       _Z17ensureInitializedP10WaveReader
//  187 	return reader->waveOffsetUs;
        LDRD     R0,R1,[R5, #+64]
        POP      {R2,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock11
//  188 }
//  189 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _Z17getSignalPeriodMsi
        THUMB
//  190 static float getSignalPeriodMs(int index) {
_Z17getSignalPeriodMsi:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  191 	WaveReader *reader = &readers[index];
        MOVS     R0,#+88
        LDR.N    R1,??DataTable11_7
        MLA      R5,R0,R4,R1
//  192 	ensureInitialized(reader);
        MOVS     R0,R5
          CFI FunCall _Z17ensureInitializedP10WaveReader
        BL       _Z17ensureInitializedP10WaveReader
//  193 	return reader->signalPeriodUs / 1000.0f;
        LDRD     R0,R1,[R5, #+56]
          CFI FunCall __aeabi_l2f
        BL       __aeabi_l2f
        VMOV     S0,R0
        VLDR.W   S1,??DataTable11_1  ;; 0x447a0000
        VDIV.F32 S0,S0,S1
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock12
//  194 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9:
        DC8      "u",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_1:
        DC8      "d",0x0,0x0
//  195 
//  196 //static efitime_t getWidthEventTime(int index) {
//  197 //	WaveReader *reader = &readers[index];
//  198 //	ensureInitialized(reader);
//  199 //	return reader->widthEventTimeUs;
//  200 //}
//  201 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z10reportWaveP7Loggingi
        THUMB
//  202 static void reportWave(Logging *logging, int index) {
_Z10reportWaveP7Loggingi:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        VPUSH    {D8-D10}
          CFI D10 Frame(CFA, -24)
          CFI D9 Frame(CFA, -32)
          CFI D8 Frame(CFA, -40)
          CFI CFA R13+40
        MOVS     R5,R0
        MOVS     R6,R1
//  203 	if (readers[index].hw->started) {
        MOVS     R0,#+88
        LDR.N    R1,??DataTable11_7
        MLA      R0,R0,R6,R1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+5]
        CMP      R0,#+0
        BEQ.W    ??reportWave_0
//  204 //	int counter = getEventCounter(index);
//  205 //	debugInt2(logging, "ev", index, counter);
//  206 
//  207 		float dwellMs = getSignalOnTime(index);
        MOVS     R0,R6
          CFI FunCall _Z15getSignalOnTimei
        BL       _Z15getSignalOnTimei
        VMOV.F32 S16,S0
//  208 		float periodMs = getSignalPeriodMs(index);
        MOVS     R0,R6
          CFI FunCall _Z17getSignalPeriodMsi
        BL       _Z17getSignalPeriodMsi
        VMOV.F32 S17,S0
//  209 
//  210 		appendPrintf(logging, "duty%d%s", index, DELIMETER);
        ADR.N    R3,??DataTable11_2  ;; ","
        MOVS     R2,R6
        LDR.N    R1,??DataTable11_21
        MOVS     R0,R5
          CFI FunCall appendPrintf
        BL       appendPrintf
//  211 		appendFloat(logging, 100.0f * dwellMs / periodMs, 2);
        MOVS     R1,#+2
        VLDR.W   S0,??DataTable11_3  ;; 0x42c80000
        VMUL.F32 S0,S16,S0
        VDIV.F32 S0,S0,S17
        MOVS     R0,R5
          CFI FunCall appendFloat
        BL       appendFloat
//  212 		appendPrintf(logging, "%s", DELIMETER);
        ADR.N    R2,??DataTable11_2  ;; ","
        ADR.N    R1,??DataTable11_4  ;; 0x25, 0x73, 0x00, 0x00
        MOVS     R0,R5
          CFI FunCall appendPrintf
        BL       appendPrintf
//  213 
//  214 		/**
//  215 		 * that's the ON time of the LAST signal
//  216 		 */
//  217 		appendPrintf(logging, "dwell%d%s", index, DELIMETER);
        ADR.N    R3,??DataTable11_2  ;; ","
        MOVS     R2,R6
        LDR.N    R1,??DataTable11_22
        MOVS     R0,R5
          CFI FunCall appendPrintf
        BL       appendPrintf
//  218 		appendFloat(logging, dwellMs, 2);
        MOVS     R1,#+2
        VMOV.F32 S0,S16
        MOVS     R0,R5
          CFI FunCall appendFloat
        BL       appendFloat
//  219 		appendPrintf(logging, "%s", DELIMETER);
        ADR.N    R2,??DataTable11_2  ;; ","
        ADR.N    R1,??DataTable11_4  ;; 0x25, 0x73, 0x00, 0x00
        MOVS     R0,R5
          CFI FunCall appendPrintf
        BL       appendPrintf
//  220 
//  221 		/**
//  222 		 * that's the total ON time during the previous engine cycle
//  223 		 */
//  224 		appendPrintf(logging, "total_dwell%d%s", index, DELIMETER);
        ADR.N    R3,??DataTable11_2  ;; ","
        MOVS     R2,R6
        LDR.N    R1,??DataTable11_23
        MOVS     R0,R5
          CFI FunCall appendPrintf
        BL       appendPrintf
//  225 		appendFloat(logging, readers[index].prevTotalOnTimeUs / 1000.0f, 2);
        MOVS     R1,#+2
        MOVS     R4,R1
        MOVS     R0,#+88
        LDR.N    R1,??DataTable11_7
        MLA      R2,R0,R6,R1
        LDRD     R0,R1,[R2, #+16]
          CFI FunCall __aeabi_l2f
        BL       __aeabi_l2f
        VMOV     S0,R0
        VLDR.W   S1,??DataTable11_1  ;; 0x447a0000
        VDIV.F32 S0,S0,S1
        MOVS     R1,R4
        MOVS     R0,R5
          CFI FunCall appendFloat
        BL       appendFloat
//  226 		appendPrintf(logging, "%s", DELIMETER);
        ADR.N    R2,??DataTable11_2  ;; ","
        ADR.N    R1,??DataTable11_4  ;; 0x25, 0x73, 0x00, 0x00
        MOVS     R0,R5
          CFI FunCall appendPrintf
        BL       appendPrintf
//  227 
//  228 		appendPrintf(logging, "period%d%s", index, DELIMETER);
        ADR.N    R3,??DataTable11_2  ;; ","
        MOVS     R2,R6
        LDR.N    R1,??DataTable11_24
        MOVS     R0,R5
          CFI FunCall appendPrintf
        BL       appendPrintf
//  229 		appendFloat(logging, periodMs, 2);
        MOVS     R1,#+2
        VMOV.F32 S0,S17
        MOVS     R0,R5
          CFI FunCall appendFloat
        BL       appendFloat
//  230 		appendPrintf(logging, "%s", DELIMETER);
        ADR.N    R2,??DataTable11_2  ;; ","
        ADR.N    R1,??DataTable11_4  ;; 0x25, 0x73, 0x00, 0x00
        MOVS     R0,R5
          CFI FunCall appendPrintf
        BL       appendPrintf
//  231 
//  232 		uint32_t offsetUs = getWaveOffset(index);
        MOVS     R0,R6
          CFI FunCall _Z13getWaveOffseti
        BL       _Z13getWaveOffseti
        VMOV     S18,R0
//  233 		int rpm = getRpm();
        LDR.N    R0,??DataTable11_25
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN13RpmCalculator3rpmEv
        BL       _ZN13RpmCalculator3rpmEv
        MOVS     R4,R0
//  234 		float oneDegreeUs = rpm == 0 ? NAN : getOneDegreeTimeUs(rpm);
        CMP      R4,#+0
        BNE.N    ??reportWave_1
        VLDR.W   S19,??DataTable11_5  ;; 0x7fffffff
        B.N      ??reportWave_2
??reportWave_1:
        VMOV     S0,R4
        VCVT.F32.S32 S0,S0
        VLDR.W   S1,??DataTable11_6  ;; 0x4822c2ab
        VDIV.F32 S19,S1,S0
//  235 
//  236 		appendPrintf(logging, "advance%d%s", index, DELIMETER);
??reportWave_2:
        ADR.N    R3,??DataTable11_2  ;; ","
        MOVS     R2,R6
        LDR.N    R1,??DataTable11_26
        MOVS     R0,R5
          CFI FunCall appendPrintf
        BL       appendPrintf
//  237 		float angle = (offsetUs / oneDegreeUs) - tdcPosition();
        VCVT.F32.U32 S0,S18
        VDIV.F32 S0,S0,S19
        LDR.N    R0,??DataTable11_27
        VLDR     S1,[R0, #0]
        LDR.N    R0,??DataTable11_28
        VLDR     S2,[R0, #+468]
        VADD.F32 S1,S1,S2
        VSUB.F32 S20,S0,S1
//  238 		fixAngle(angle);
        LDR.N    R0,??DataTable11_28
        LDR      R0,[R0, #+424]
        CMP      R0,#+0
        BNE.N    ??reportWave_3
        LDR.N    R0,??DataTable11_29
          CFI FunCall firmwareError
        BL       firmwareError
??reportWave_3:
        VCMP.F32 S20,#0.0
        FMSTAT   
        BPL.N    ??reportWave_4
        LDR.N    R0,??DataTable11_28
        VLDR     S0,[R0, #+424]
        VCVT.F32.S32 S0,S0
        VADD.F32 S20,S0,S20
        B.N      ??reportWave_3
??reportWave_4:
        LDR.N    R0,??DataTable11_28
        VLDR     S0,[R0, #+424]
        VCVT.F32.S32 S0,S0
        VCMP.F32 S20,S0
        FMSTAT   
        BLT.N    ??reportWave_5
        LDR.N    R0,??DataTable11_28
        VLDR     S0,[R0, #+424]
        VCVT.F32.S32 S0,S0
        VSUB.F32 S20,S20,S0
        B.N      ??reportWave_4
//  239 		appendFloat(logging, angle, 3);
??reportWave_5:
        MOVS     R1,#+3
        VMOV.F32 S0,S20
        MOVS     R0,R5
          CFI FunCall appendFloat
        BL       appendFloat
//  240 		appendPrintf(logging, "%s", DELIMETER);
        ADR.N    R2,??DataTable11_2  ;; ","
        ADR.N    R1,??DataTable11_4  ;; 0x25, 0x73, 0x00, 0x00
        MOVS     R0,R5
          CFI FunCall appendPrintf
        BL       appendPrintf
//  241 	}
//  242 }
??reportWave_0:
        VPOP     {D8-D10}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI D10 SameValue
          CFI CFA R13+16
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock13
//  243 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _Z9printWaveP7Logging
        THUMB
//  244 void printWave(Logging *logging) {
_Z9printWaveP7Logging:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  245 	reportWave(logging, 0);
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall _Z10reportWaveP7Loggingi
        BL       _Z10reportWaveP7Loggingi
//  246 	reportWave(logging, 1);
        MOVS     R1,#+1
        MOVS     R0,R4
          CFI FunCall _Z10reportWaveP7Loggingi
        BL       _Z10reportWaveP7Loggingi
//  247 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock14
//  248 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function _Z16initWaveAnalyzerP7Logging
        THUMB
//  249 void initWaveAnalyzer(Logging *sharedLogger) {
_Z16initWaveAnalyzerP7Logging:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        SUB      SP,SP,#+8
          CFI CFA R13+16
        MOVS     R4,R0
//  250 	logger = sharedLogger;
        LDR.N    R0,??DataTable11_30
        STR      R4,[R0, #+0]
//  251 #if EFI_WAVE_ANALYZER || defined(__DOXYGEN__)
//  252 	initWave(WA_CHANNEL_1, 0);
        MOVS     R1,#+0
        LDR.N    R0,??DataTable11_31
          CFI FunCall _Z8initWavePKci
        BL       _Z8initWavePKci
//  253 	initWave(WA_CHANNEL_2, 1);
        MOVS     R1,#+1
        LDR.N    R0,??DataTable11_32
          CFI FunCall _Z8initWavePKci
        BL       _Z8initWavePKci
//  254 
//  255 //	addTriggerEventListener(waTriggerEventListener, "wave analyzer", NULL);
//  256 
//  257 	addConsoleActionII("set_logic_input_mode", setWaveModeSilent);
        LDR.N    R1,??DataTable11_33
        LDR.N    R0,??DataTable11_34
          CFI FunCall addConsoleActionII
        BL       addConsoleActionII
//  258 
//  259 	chThdCreateStatic(waThreadStack, sizeof(waThreadStack), NORMALPRIO, waThread, (void*) NULL);
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        LDR.N    R3,??DataTable11_35
        MOVS     R2,#+64
        MOV      R1,#+712
        LDR.N    R0,??DataTable11_36
          CFI FunCall chThdCreateStatic
        BL       chThdCreateStatic
//  260 
//  261 #else
//  262 	print("wave disabled\r\n");
//  263 #endif
//  264 }
        POP      {R0,R1,R4,PC}    ;; return
          CFI EndBlock cfiBlock15

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11:
        DC32     0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_1:
        DC32     0x447a0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_2:
        DC8      ",",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_3:
        DC32     0x42c80000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_4:
        DC8      0x25, 0x73, 0x00, 0x00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_5:
        DC32     0x7fffffff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_6:
        DC32     0x4822c2ab

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_7:
        DC32     readers

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_8:
        DC32     `?<Constant "wave analyzer NOT INI...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_9:
        DC32     waveChart

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_10:
        DC32     previousEngineCycleTimeUs

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_11:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_12:
        DC32     waveReaderCount

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_13:
        DC32     `?<Constant "too many ICUs">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_14:
        DC32     `?<Constant "wave input">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_15:
        DC32     _Z18waAnaWidthCallbackP10WaveReader

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_16:
        DC32     _Z19waIcuPeriodCallbackP10WaveReader

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_17:
        DC32     `?<Constant "wave%d input on %s\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_18:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_19:
        DC32     `?<Constant "Wave Analyzer">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_20:
        DC32     0x3d0901

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_21:
        DC32     `?<Constant "duty%d%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_22:
        DC32     `?<Constant "dwell%d%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_23:
        DC32     `?<Constant "total_dwell%d%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_24:
        DC32     `?<Constant "period%d%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_25:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_26:
        DC32     `?<Constant "advance%d%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_27:
        DC32     _engine+0xDE4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_28:
        DC32     persistentState

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_29:
        DC32     `?<Constant "zero engineCycle">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_30:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_31:
        DC32     `?<Constant "input1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_32:
        DC32     `?<Constant "input2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_33:
        DC32     _Z17setWaveModeSilentii

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_34:
        DC32     `?<Constant "set_logic_input_mode">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_35:
        DC32     _Z8waThreadPv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_36:
        DC32     waThreadStack

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
`?<Constant "wave analyzer NOT INI...">`:
        DATA
        DC8 "wave analyzer NOT INITIALIZED"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 "u"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "too many ICUs">`:
        DATA
        DC8 "too many ICUs"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "wave input">`:
        DATA
        DC8 "wave input"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "wave%d input on %s\\r\\n">`:
        DATA
        DC8 "wave%d input on %s\015\012"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Wave Analyzer">`:
        DATA
        DC8 "Wave Analyzer"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "duty%d%s">`:
        DATA
        DC8 "duty%d%s"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 ","

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "%s"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "dwell%d%s">`:
        DATA
        DC8 "dwell%d%s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "total_dwell%d%s">`:
        DATA
        DC8 "total_dwell%d%s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "period%d%s">`:
        DATA
        DC8 "period%d%s"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "advance%d%s">`:
        DATA
        DC8 "advance%d%s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "zero engineCycle">`:
        DATA
        DC8 "zero engineCycle"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "input1">`:
        DATA
        DC8 "input1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "input2">`:
        DATA
        DC8 "input2"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_logic_input_mode">`:
        DATA
        DC8 "set_logic_input_mode"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 "d"

        END
//  265 
//  266 #endif
// 
// 1 168 bytes in section .bss
//     4 bytes in section .init_array
//   234 bytes in section .rodata
// 1 262 bytes in section .text
// 
// 1 266 bytes of CODE  memory
//   234 bytes of CONST memory
// 1 168 bytes of DATA  memory
//
//Errors: none
//Warnings: 4
