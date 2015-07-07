///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:49 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\development\wav /
//                       e_chart.cpp                                          /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\development\wav /
//                       e_chart.cpp -lCN F:\stuff\rusefi_sourceforge\firmwar /
//                       e\iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge\fi /
//                       rmware\iar\Debug\List\ -o                            /
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
//                       wave_chart.s                                         /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME wave_chart

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z10getFullLogv
        EXTERN _Z12boolToStringb
        EXTERN _ZN13RpmCalculator20getRevolutionCounterEv
        EXTERN _ZN18LoggingWithStorageC1EPKc
        EXTERN _ZN7LoggingC1Ev
        EXTERN __aeabi_ldivmod
        EXTERN addConsoleActionI
        EXTERN appendFast
        EXTERN appendPrintf
        EXTERN engine
        EXTERN engineConfiguration
        EXTERN firmwareError
        EXTERN getRemainingStack
        EXTERN getTimeNowNt
        EXTERN initLoggingExt
        EXTERN itoa10
        EXTERN lockOutputBuffer
        EXTERN maxLockTime
        EXTERN printMsg
        EXTERN remainingSize
        EXTERN resetLogging
        EXTERN rlist
        EXTERN scheduleLogging
        EXTERN scheduleMsg
        EXTERN unlockOutputBuffer

        PUBLIC _Z12setChartSizei
        PUBLIC _Z13initWaveChartP9WaveChart
        PUBLIC _Z22showWaveChartHistogramv
        PUBLIC _ZN9WaveChart12publishChartEv
        PUBLIC _ZN9WaveChart14resetWaveChartEv
        PUBLIC _ZN9WaveChart15isWaveChartFullEv
        PUBLIC _ZN9WaveChart18addWaveChartEvent3EPKcS1_
        PUBLIC _ZN9WaveChart18publishChartIfFullEv
        PUBLIC _ZN9WaveChart19isStartedTooLongAgoEv
        PUBLIC _ZN9WaveChart4initEv
        PUBLIC _ZN9WaveChartC1Ev
        PUBLIC _ZN9WaveChartC2Ev
        PUBLIC skipUntilEngineCycle
        PUBLIC waveChartUsedSize
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\development\wave_chart.cpp
//    1 /**
//    2  * @file	wave_chart.cpp
//    3  * @brief	Dev console wave sniffer logic
//    4  *
//    5  * Here we have our own build-in logic analyzer. The data we aggregate here is sent to the
//    6  * java UI Dev Console so that it can be displayed nicely in the Sniffer tab.
//    7  *
//    8  * Both external events (see wave_analyzer.c) and internal (see signal executors) are supported
//    9  *
//   10  * @date Jun 23, 2013
//   11  * @author Andrey Belomutskiy, (c) 2012-2015
//   12  *
//   13  * This file is part of rusEfi - see http://rusefi.com
//   14  *
//   15  * rusEfi is free software; you can redistribute it and/or modify it under the terms of
//   16  * the GNU General Public License as published by the Free Software Foundation; either
//   17  * version 3 of the License, or (at your option) any later version.
//   18  *
//   19  * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
//   20  * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   21  * GNU General Public License for more details.
//   22  *
//   23  * You should have received a copy of the GNU General Public License along with this program.
//   24  * If not, see <http://www.gnu.org/licenses/>.
//   25  */
//   26 
//   27 #include "main.h"
//   28 #include "wave_chart.h"
//   29 
//   30 #if EFI_WAVE_CHART || defined(__DOXYGEN__)
//   31 
//   32 #include "engine_configuration.h"
//   33 #include "eficonsole.h"
//   34 #include "status_loop.h"
//   35 
//   36 #define CHART_DELIMETER	'!'
//   37 
//   38 #if EFI_HISTOGRAMS || defined(__DOXYGEN__)
//   39 #include "rfiutil.h"
//   40 #include "histogram.h"
//   41 static histogram_s waveChartHisto;
//   42 #endif
//   43 
//   44 EXTERN_ENGINE
//   45 ;
//   46 extern uint32_t maxLockTime;
//   47 
//   48 /**
//   49  * This is the number of events in the digital chart which would be displayed
//   50  * on the 'digital sniffer' pane
//   51  */
//   52 #if EFI_PROD_CODE
//   53 #define WAVE_LOGGING_SIZE 5000
//   54 #else
//   55 #define WAVE_LOGGING_SIZE 35000
//   56 #endif
//   57 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   58 int waveChartUsedSize;
waveChartUsedSize:
        DS8 4
//   59 
//   60 //#define DEBUG_WAVE 1
//   61 
//   62 #if DEBUG_WAVE
//   63 static Logging debugLogging;
//   64 #endif /* DEBUG_WAVE */
//   65 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   66 static LoggingWithStorage logger("wave info");
logger:
        DS8 260
//   67 
//   68 /**
//   69  * We want to skip some engine cycles to skip what was scheduled before parameters were changed
//   70  */

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   71 uint32_t skipUntilEngineCycle = 0;
skipUntilEngineCycle:
        DS8 4

        SECTION `.ccm`:DATA:REORDER:NOROOT(2)
WAVE_LOGGING_BUFFER:
        DS8 5000

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _ZN9WaveChartC2Ev
        THUMB
// __code __interwork __softfp WaveChart::subobject WaveChart()
_ZN9WaveChartC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN9WaveChartC1Ev
        BL       _ZN9WaveChartC1Ev
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
        LDR.N    R1,??DataTable10
        LDR.N    R0,??DataTable10_1
          CFI FunCall _ZN18LoggingWithStorageC1EPKc
        BL       _ZN18LoggingWithStorageC1EPKc
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock1
//   72 
//   73 #if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
//   74 extern WaveChart waveChart;
//   75 //static void resetWaveChartNow(void) {
//   76 //	skipUntilEngineCycle = engine->rpmCalculator.getRevolutionCounter() + 3;
//   77 //	waveChart.resetWaveChart();
//   78 //}
//   79 #endif
//   80 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _ZN9WaveChart14resetWaveChartEv
        THUMB
//   81 void WaveChart::resetWaveChart() {
_ZN9WaveChart14resetWaveChartEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   82 #if DEBUG_WAVE
//   83 	scheduleSimpleMsg(&debugLogging, "reset while at ", counter);
//   84 #endif /* DEBUG_WAVE */
//   85 	resetLogging(&logging);
        MOVS     R0,R4
          CFI FunCall resetLogging
        BL       resetLogging
//   86 	counter = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+60]
//   87 	startTimeNt = 0;
        MOVS     R0,#+0
        MOVS     R1,#+0
        STRD     R0,R1,[R4, #+64]
//   88 	appendPrintf(&logging, "wave_chart%s", DELIMETER);
        ADR.N    R2,??DataTable9  ;; ","
        LDR.N    R1,??DataTable10_2
        MOVS     R0,R4
          CFI FunCall appendPrintf
        BL       appendPrintf
//   89 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2
//   90 
//   91 static char WAVE_LOGGING_BUFFER[WAVE_LOGGING_SIZE] CCM_OPTIONAL;
//   92 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _ZN9WaveChart19isStartedTooLongAgoEv
        THUMB
//   93 bool_t WaveChart::isStartedTooLongAgo() {
_ZN9WaveChart19isStartedTooLongAgoEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   94 	/**
//   95 	 * Say at 300rpm we should get at least four events per revolution.
//   96 	 * That's 300/60*4=20 events per second
//   97 	 * engineChartSize/20 is the longest meaningful chart.
//   98 	 *
//   99 	 */
//  100 	efitime_t chartDurationNt = getTimeNowNt() - startTimeNt;
          CFI FunCall getTimeNowNt
        BL       getTimeNowNt
        LDRD     R2,R3,[R4, #+64]
        SUBS     R0,R0,R2
        SBCS     R1,R1,R3
//  101 	return startTimeNt != 0 && NT2US(chartDurationNt) > engineConfiguration->engineChartSize * 1000000 / 20;
        LDRD     R2,R3,[R4, #+64]
        CMP      R3,#+0
        BNE.N    ??isStartedTooLongAgo_0
        CMP      R2,#+0
        BEQ.N    ??isStartedTooLongAgo_1
??isStartedTooLongAgo_0:
        MOVS     R2,#+168
        MOVS     R3,#+0
          CFI FunCall __aeabi_ldivmod
        BL       __aeabi_ldivmod
        LDR.N    R2,??DataTable10_3
        LDR      R2,[R2, #+0]
        LDR      R2,[R2, #+1512]
        LDR.N    R3,??DataTable10_4  ;; 0xf4240
        MULS     R2,R3,R2
        MOVS     R3,#+20
        UDIV     R2,R2,R3
        MOVS     R3,#+0
        CMP      R3,R1
        BGT.N    ??isStartedTooLongAgo_1
        BLT.N    ??isStartedTooLongAgo_2
        CMP      R2,R0
        BCS.N    ??isStartedTooLongAgo_1
??isStartedTooLongAgo_2:
        MOVS     R0,#+1
        B.N      ??isStartedTooLongAgo_3
??isStartedTooLongAgo_1:
        MOVS     R0,#+0
??isStartedTooLongAgo_3:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//  102 }
//  103 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _ZN9WaveChart15isWaveChartFullEv
          CFI NoCalls
        THUMB
//  104 bool_t WaveChart::isWaveChartFull() {
//  105 	return counter >= engineConfiguration->engineChartSize;
_ZN9WaveChart15isWaveChartFullEv:
        LDR      R0,[R0, #+60]
        LDR.N    R1,??DataTable10_3
        LDR      R1,[R1, #+0]
        LDR      R1,[R1, #+1512]
        CMP      R0,R1
        BCC.N    ??isWaveChartFull_0
        MOVS     R0,#+1
        B.N      ??isWaveChartFull_1
??isWaveChartFull_0:
        MOVS     R0,#+0
??isWaveChartFull_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//  106 }
//  107 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z11printStatusv
        THUMB
//  108 static void printStatus(void) {
_Z11printStatusv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  109 	scheduleMsg(&logger, "engine chart: %s", boolToString(engineConfiguration->isEngineChartEnabled));
        LDR.N    R0,??DataTable10_3
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+5
        ANDS     R0,R0,#0x1
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R2,R0
        LDR.N    R1,??DataTable10_5
        LDR.N    R0,??DataTable10_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  110 	scheduleMsg(&logger, "engine chart size=%d", engineConfiguration->engineChartSize);
        LDR.N    R0,??DataTable10_3
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+1512]
        LDR.N    R1,??DataTable10_6
        LDR.N    R0,??DataTable10_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  111 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock5
//  112 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z14setChartActivei
        THUMB
//  113 static void setChartActive(int value) {
_Z14setChartActivei:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  114 	engineConfiguration->isEngineChartEnabled = value;
        CMP      R4,#+0
        BEQ.N    ??setChartActive_0
        MOVS     R0,#+1
        B.N      ??setChartActive_1
??setChartActive_0:
        MOVS     R0,#+0
??setChartActive_1:
        LDR.N    R1,??DataTable10_3
        LDR      R1,[R1, #+0]
        LDRB     R1,[R1, #+1496]
        BFI      R1,R0,#+5,#+1
        LDR.N    R0,??DataTable10_3
        LDR      R0,[R0, #+0]
        STRB     R1,[R0, #+1496]
//  115 	printStatus();
          CFI FunCall _Z11printStatusv
        BL       _Z11printStatusv
//  116 	maxLockTime = 0;
        LDR.N    R0,??DataTable10_7
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  117 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock6
//  118 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z12setChartSizei
        THUMB
//  119 void setChartSize(int newSize) {
_Z12setChartSizei:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  120 	if (newSize < 5) {
        CMP      R4,#+5
        BLT.N    ??setChartSize_0
//  121 		return;
//  122 	}
//  123 	engineConfiguration->engineChartSize = newSize;
??setChartSize_1:
        LDR.N    R0,??DataTable10_3
        LDR      R0,[R0, #+0]
        STR      R4,[R0, #+1512]
//  124 	printStatus();
          CFI FunCall _Z11printStatusv
        BL       _Z11printStatusv
//  125 }
??setChartSize_0:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock7
//  126 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _ZN9WaveChart18publishChartIfFullEv
        THUMB
//  127 void WaveChart::publishChartIfFull() {
_ZN9WaveChart18publishChartIfFullEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  128 	if (isWaveChartFull() || isStartedTooLongAgo()) {
        MOVS     R0,R4
          CFI FunCall _ZN9WaveChart15isWaveChartFullEv
        BL       _ZN9WaveChart15isWaveChartFullEv
        CMP      R0,#+0
        BNE.N    ??publishChartIfFull_0
        MOVS     R0,R4
          CFI FunCall _ZN9WaveChart19isStartedTooLongAgoEv
        BL       _ZN9WaveChart19isStartedTooLongAgoEv
        CMP      R0,#+0
        BEQ.N    ??publishChartIfFull_1
//  129 		publishChart();
??publishChartIfFull_0:
        MOVS     R0,R4
          CFI FunCall _ZN9WaveChart12publishChartEv
        BL       _ZN9WaveChart12publishChartEv
//  130 		resetWaveChart();
        MOVS     R0,R4
          CFI FunCall _ZN9WaveChart14resetWaveChartEv
        BL       _ZN9WaveChart14resetWaveChartEv
//  131 	}
//  132 }
??publishChartIfFull_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock8
//  133 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _ZN9WaveChartC1Ev
        THUMB
//  134 WaveChart::WaveChart() {
_ZN9WaveChartC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN7LoggingC1Ev
        BL       _ZN7LoggingC1Ev
//  135 	isInitialized = false;
        MOVS     R0,#+0
        STR      R0,[R4, #+72]
//  136 	startTimeNt = 0;
        MOVS     R0,#+0
        MOVS     R1,#+0
        STRD     R0,R1,[R4, #+64]
//  137 	counter = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+60]
//  138 }
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock9
//  139 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _ZN9WaveChart4initEv
        THUMB
//  140 void WaveChart::init() {
_ZN9WaveChart4initEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  141 	initLoggingExt(&logging, "wave chart", WAVE_LOGGING_BUFFER, sizeof(WAVE_LOGGING_BUFFER));
        MOVW     R3,#+5000
        LDR.N    R2,??DataTable10_8
        LDR.N    R1,??DataTable10_9
        MOVS     R0,R4
          CFI FunCall initLoggingExt
        BL       initLoggingExt
//  142 	isInitialized = true;
        MOVS     R0,#+1
        STR      R0,[R4, #+72]
//  143 	resetWaveChart();
        MOVS     R0,R4
          CFI FunCall _ZN9WaveChart14resetWaveChartEv
        BL       _ZN9WaveChart14resetWaveChartEv
//  144 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock10
//  145 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _ZN9WaveChart12publishChartEv
        THUMB
//  146 void WaveChart::publishChart() {
_ZN9WaveChart12publishChartEv:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  147 	appendPrintf(&logging, DELIMETER);
        ADR.N    R1,??DataTable9  ;; ","
        MOVS     R0,R4
          CFI FunCall appendPrintf
        BL       appendPrintf
//  148 	waveChartUsedSize = loggingSize(&logging);
        LDR      R0,[R4, #+48]
        LDR      R1,[R4, #+44]
        SUBS     R0,R0,R1
        LDR.N    R1,??DataTable10_10
        STR      R0,[R1, #+0]
//  149 #if DEBUG_WAVE
//  150 	Logging *l = &chart->logging;
//  151 	scheduleSimpleMsg(&debugLogging, "IT'S TIME", strlen(l->buffer));
//  152 #endif
//  153 	bool isFullLog = getFullLog();
          CFI FunCall _Z10getFullLogv
        BL       _Z10getFullLogv
        MOVS     R5,R0
//  154 	if (engineConfiguration->isEngineChartEnabled && isFullLog) {
        LDR.N    R0,??DataTable10_3
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        LSLS     R0,R0,#+26
        BPL.N    ??publishChart_0
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BEQ.N    ??publishChart_0
//  155 		scheduleLogging(&logging);
        MOVS     R0,R4
          CFI FunCall scheduleLogging
        BL       scheduleLogging
//  156 	}
//  157 }
??publishChart_0:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock11
//  158 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  159 static char timeBuffer[10];
timeBuffer:
        DS8 12
//  160 
//  161 /**
//  162  * @brief	Register an event for digital sniffer
//  163  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _ZN9WaveChart18addWaveChartEvent3EPKcS1_
        THUMB
//  164 void WaveChart::addWaveChartEvent3(const char *name, const char * msg) {
_ZN9WaveChart18addWaveChartEvent3EPKcS1_:
        PUSH     {R3-R11,LR}
          CFI R14 Frame(CFA, -4)
          CFI R11 Frame(CFA, -8)
          CFI R10 Frame(CFA, -12)
          CFI R9 Frame(CFA, -16)
          CFI R8 Frame(CFA, -20)
          CFI R7 Frame(CFA, -24)
          CFI R6 Frame(CFA, -28)
          CFI R5 Frame(CFA, -32)
          CFI R4 Frame(CFA, -36)
          CFI CFA R13+40
        MOVS     R6,R0
        MOVS     R7,R1
        MOV      R8,R2
//  165 	if (engine->rpmCalculator.getRevolutionCounter() < skipUntilEngineCycle)
        LDR.N    R0,??DataTable10_11
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN13RpmCalculator20getRevolutionCounterEv
        BL       _ZN13RpmCalculator20getRevolutionCounterEv
        LDR.N    R1,??DataTable10_12
        LDR      R1,[R1, #+0]
        CMP      R0,R1
        BCC.N    ??addWaveChartEvent3_0
//  166 		return;
//  167 	efiAssertVoid(name!=NULL, "WC: NULL name");
??addWaveChartEvent3_1:
        CMP      R7,#+0
        BNE.N    ??addWaveChartEvent3_2
        LDR.N    R0,??DataTable10_13
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??addWaveChartEvent3_0
//  168 	if (!engineConfiguration->isEngineChartEnabled) {
??addWaveChartEvent3_2:
        LDR.N    R0,??DataTable10_3
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+5
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??addWaveChartEvent3_0
//  169 		return;
//  170 	}
//  171 
//  172 #if EFI_PROD_CODE
//  173 	efiAssertVoid(getRemainingStack(chThdSelf()) > 32, "lowstck#2c");
??addWaveChartEvent3_3:
        LDR.N    R0,??DataTable10_14
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+33
        BGE.N    ??addWaveChartEvent3_4
        LDR.N    R0,??DataTable10_15
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??addWaveChartEvent3_0
//  174 #endif
//  175 
//  176 	efiAssertVoid(isInitialized, "chart not initialized");
??addWaveChartEvent3_4:
        LDR      R0,[R6, #+72]
        CMP      R0,#+0
        BNE.N    ??addWaveChartEvent3_5
        LDR.N    R0,??DataTable10_16
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??addWaveChartEvent3_0
//  177 #if DEBUG_WAVE
//  178 	scheduleSimpleMsg(&debugLogging, "current", chart->counter);
//  179 #endif
//  180 	if (isWaveChartFull()) {
??addWaveChartEvent3_5:
        MOVS     R0,R6
          CFI FunCall _ZN9WaveChart15isWaveChartFullEv
        BL       _ZN9WaveChart15isWaveChartFullEv
        CMP      R0,#+0
        BNE.N    ??addWaveChartEvent3_0
//  181 		return;
//  182 	}
//  183 
//  184 #if EFI_HISTOGRAMS && EFI_PROD_CODE
//  185 	int beforeCallback = hal_lld_get_counter_value();
//  186 #endif
//  187 
//  188 	efitick_t nowNt = getTimeNowNt();
??addWaveChartEvent3_6:
          CFI FunCall getTimeNowNt
        BL       getTimeNowNt
        MOVS     R4,R0
        MOVS     R5,R1
//  189 
//  190 	bool alreadyLocked = lockOutputBuffer(); // we have multiple threads writing to the same output buffer
          CFI FunCall lockOutputBuffer
        BL       lockOutputBuffer
        MOV      R9,R0
//  191 
//  192 	if (counter == 0) {
        LDR      R0,[R6, #+60]
        CMP      R0,#+0
        BNE.N    ??addWaveChartEvent3_7
//  193 		startTimeNt = nowNt;
        STRD     R4,R5,[R6, #+64]
//  194 	}
//  195 	counter++;
??addWaveChartEvent3_7:
        LDR      R0,[R6, #+60]
        ADDS     R0,R0,#+1
        STR      R0,[R6, #+60]
//  196 
//  197 	/**
//  198 	 * We want smaller times within a chart in order to reduce packet size.
//  199 	 */
//  200 	/**
//  201 	 * todo: migrate to binary fractions in order to eliminate
//  202 	 * this division? I do not like division
//  203 	 *
//  204 	 * at least that's 32 bit division now
//  205 	 */
//  206 	uint32_t diffNt = nowNt - startTimeNt;
        LDRD     R0,R1,[R6, #+64]
        SUBS     R10,R4,R0
//  207 	uint32_t time100 = NT2US(diffNt / 10);
        MOVS     R0,#+10
        UDIV     R0,R10,R0
        MOVS     R1,#+168
        UDIV     R11,R0,R1
//  208 
//  209 	if (remainingSize(&logging) > 35) {
        MOVS     R0,R6
          CFI FunCall remainingSize
        BL       remainingSize
        CMP      R0,#+36
        BCC.N    ??addWaveChartEvent3_8
//  210 		/**
//  211 		 * printf is a heavy method, append is used here as a performance optimization
//  212 		 */
//  213 		appendFast(&logging, name);
        MOVS     R1,R7
        MOVS     R0,R6
          CFI FunCall appendFast
        BL       appendFast
//  214 		appendChar(&logging, CHART_DELIMETER);
        LDR      R0,[R6, #+48]
        MOVS     R1,#+33
        STRB     R1,[R0, #+0]
        LDR      R0,[R6, #+48]
        ADDS     R0,R0,#+1
        STR      R0,[R6, #+48]
//  215 		appendFast(&logging, msg);
        MOV      R1,R8
        MOVS     R0,R6
          CFI FunCall appendFast
        BL       appendFast
//  216 		appendChar(&logging, CHART_DELIMETER);
        LDR      R0,[R6, #+48]
        MOVS     R1,#+33
        STRB     R1,[R0, #+0]
        LDR      R0,[R6, #+48]
        ADDS     R0,R0,#+1
        STR      R0,[R6, #+48]
//  217 //		time100 -= startTime100;
//  218 
//  219 		itoa10(timeBuffer, time100);
        MOV      R1,R11
        LDR.N    R0,??DataTable10_17
          CFI FunCall itoa10
        BL       itoa10
//  220 		appendFast(&logging, timeBuffer);
        LDR.N    R1,??DataTable10_17
        MOVS     R0,R6
          CFI FunCall appendFast
        BL       appendFast
//  221 		appendChar(&logging, CHART_DELIMETER);
        LDR      R0,[R6, #+48]
        MOVS     R1,#+33
        STRB     R1,[R0, #+0]
        LDR      R0,[R6, #+48]
        ADDS     R0,R0,#+1
        STR      R0,[R6, #+48]
//  222 		logging.linePointer[0] = 0;
        LDR      R0,[R6, #+48]
        MOVS     R1,#+0
        STRB     R1,[R0, #+0]
//  223 	}
//  224 	if (!alreadyLocked) {
??addWaveChartEvent3_8:
        UXTB     R9,R9            ;; ZeroExt  R9,R9,#+24,#+24
        CMP      R9,#+0
        BNE.N    ??addWaveChartEvent3_9
//  225 		unlockOutputBuffer();
          CFI FunCall unlockOutputBuffer
        BL       unlockOutputBuffer
//  226 	}
//  227 
//  228 #if EFI_HISTOGRAMS && EFI_PROD_CODE
//  229 	int64_t diff = hal_lld_get_counter_value() - beforeCallback;
//  230 	if (diff > 0) {
//  231 		hsAdd(&waveChartHisto, diff);
//  232 	}
//  233 #endif /* EFI_HISTOGRAMS */
//  234 
//  235 }
??addWaveChartEvent3_9:
??addWaveChartEvent3_0:
        POP      {R0,R4-R11,PC}   ;; return
          CFI EndBlock cfiBlock12

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9:
        DC8      ",",0x0,0x0
//  236 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z22showWaveChartHistogramv
          CFI NoCalls
        THUMB
//  237 void showWaveChartHistogram(void) {
//  238 #if (EFI_HISTOGRAMS && EFI_PROD_CODE) || defined(__DOXYGEN__)
//  239 	printHistogram(&logger, &waveChartHisto);
//  240 #endif
//  241 }
_Z22showWaveChartHistogramv:
        BX       LR               ;; return
          CFI EndBlock cfiBlock13
//  242 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _Z13initWaveChartP9WaveChart
        THUMB
//  243 void initWaveChart(WaveChart *chart) {
_Z13initWaveChartP9WaveChart:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  244 
//  245 	if (!engineConfiguration->isEngineChartEnabled) {
        LDR.N    R0,??DataTable10_3
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+5
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BNE.N    ??initWaveChart_0
//  246 		printMsg(&logger, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! chart disabled");
        LDR.N    R1,??DataTable10_18
        LDR.N    R0,??DataTable10_1
          CFI FunCall printMsg
        BL       printMsg
//  247 	}
//  248 
//  249 	/**
//  250 	 * constructor does not work because we need specific initialization order
//  251 	 */
//  252 	chart->init();
??initWaveChart_0:
        MOVS     R0,R4
          CFI FunCall _ZN9WaveChart4initEv
        BL       _ZN9WaveChart4initEv
//  253 
//  254 	printStatus();
          CFI FunCall _Z11printStatusv
        BL       _Z11printStatusv
//  255 
//  256 #if DEBUG_WAVE || defined(__DOXYGEN__)
//  257 	initLoggingExt(&debugLogging, "wave chart debug", &debugLogging.DEFAULT_BUFFER, sizeof(debugLogging.DEFAULT_BUFFER));
//  258 #endif
//  259 
//  260 #if EFI_HISTOGRAMS || defined(__DOXYGEN__)
//  261 	initHistogram(&waveChartHisto, "wave chart");
//  262 #endif /* EFI_HISTOGRAMS */
//  263 
//  264 	addConsoleActionI("chartsize", setChartSize);
        LDR.N    R1,??DataTable10_19
        LDR.N    R0,??DataTable10_20
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  265 	addConsoleActionI("chart", setChartActive);
        LDR.N    R1,??DataTable10_21
        LDR.N    R0,??DataTable10_22
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  266 //#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
//  267 //	addConsoleAction("reset_engine_chart", resetWaveChartNow);
//  268 //#endif
//  269 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock14

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10:
        DC32     `?<Constant "wave info">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_1:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_2:
        DC32     `?<Constant "wave_chart%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_3:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_4:
        DC32     0xf4240

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_5:
        DC32     `?<Constant "engine chart: %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_6:
        DC32     `?<Constant "engine chart size=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_7:
        DC32     maxLockTime

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_8:
        DC32     WAVE_LOGGING_BUFFER

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_9:
        DC32     `?<Constant "wave chart">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_10:
        DC32     waveChartUsedSize

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_11:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_12:
        DC32     skipUntilEngineCycle

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_13:
        DC32     `?<Constant "WC: NULL name">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_14:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_15:
        DC32     `?<Constant "lowstck#2c">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_16:
        DC32     `?<Constant "chart not initialized">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_17:
        DC32     timeBuffer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_18:
        DC32     `?<Constant "!!!!!!!!!!!!!!!!!!!!!...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_19:
        DC32     _Z12setChartSizei

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_20:
        DC32     `?<Constant "chartsize">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_21:
        DC32     _Z14setChartActivei

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_22:
        DC32     `?<Constant "chart">`

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
`?<Constant "engine chart: %s">`:
        DATA
        DC8 "engine chart: %s"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "engine chart size=%d">`:
        DATA
        DC8 "engine chart size=%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "!!!!!!!!!!!!!!!!!!!!!...">`:
        DATA
        DC8 21H, 21H, 21H, 21H, 21H, 21H, 21H, 21H
        DC8 21H, 21H, 21H, 21H, 21H, 21H, 21H, 21H
        DC8 21H, 21H, 21H, 21H, 21H, 21H, 21H, 21H
        DC8 21H, 21H, 21H, 21H, 21H, 21H, 21H, 21H
        DC8 21H, 21H, 21H, 21H, 21H, 21H, 21H, 21H
        DC8 21H, 21H, 21H, 21H, 21H, 21H, 21H, 21H
        DC8 21H, 21H, 21H, 21H, 21H, 21H, 21H, 21H
        DC8 21H, 21H, 21H, 21H, 21H, 21H, 21H, 21H
        DC8 21H, 21H, 21H, 21H, 21H, 21H, 20H, 63H
        DC8 68H, 61H, 72H, 74H, 20H, 64H, 69H, 73H
        DC8 61H, 62H, 6CH, 65H, 64H, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "chartsize">`:
        DATA
        DC8 "chartsize"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "chart">`:
        DATA
        DC8 "chart"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "wave chart">`:
        DATA
        DC8 "wave chart"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 ","

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "wave_chart%s">`:
        DATA
        DC8 "wave_chart%s"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "WC: NULL name">`:
        DATA
        DC8 "WC: NULL name"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lowstck#2c">`:
        DATA
        DC8 "lowstck#2c"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "chart not initialized">`:
        DATA
        DC8 "chart not initialized"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "wave info">`:
        DATA
        DC8 "wave info"
        DC8 0, 0

        END
//  270 
//  271 #endif /* EFI_WAVE_CHART */
// 
//   280 bytes in section .bss
// 5 000 bytes in section .ccm
//     4 bytes in section .init_array
//   246 bytes in section .rodata
//   840 bytes in section .text
// 
//   844 bytes of CODE  memory
//   246 bytes of CONST memory
// 5 280 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
