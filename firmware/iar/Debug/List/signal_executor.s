///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:10:03 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\alg /
//                       o\signal_executor.cpp                                /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\alg /
//                       o\signal_executor.cpp -lCN                           /
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
//                       signal_executor.s                                    /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME signal_executor

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z14scheduleByTimePKcP12scheduling_sxPFvPvES3_
        EXTERN _Z20getRevolutionCounterv
        EXTERN _Z22initSignalExecutorImplv
        EXTERN _ZN9WaveChart18addWaveChartEvent3EPKcS1_
        EXTERN __aeabi_f2d
        EXTERN __iar_FDtest
        EXTERN enginePins
        EXTERN firmwareError
        EXTERN persistentState
        EXTERN warning
        EXTERN waveChart

        PUBLIC _Z10turnPinLowP14NamedOutputPin
        PUBLIC _Z11turnPinHighP14NamedOutputPin
        PUBLIC _Z14scheduleOutputP19OutputSignal_structxff
        PUBLIC _Z18initSignalExecutorv
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\algo\signal_executor.cpp
//    1 /**
//    2  * @file	signal_executor.cpp
//    3  *
//    4  * todo: we should split this file into two:
//    5  * one for pure scheduling and another one for signal output which would
//    6  * use the scheduling
//    7  *
//    8  * @date Dec 4, 2013
//    9  * @author Andrey Belomutskiy, (c) 2012-2015
//   10  *
//   11  * This file is part of rusEfi - see http://rusefi.com
//   12  *
//   13  * rusEfi is free software; you can redistribute it and/or modify it under the terms of
//   14  * the GNU General Public License as published by the Free Software Foundation; either
//   15  * version 3 of the License, or (at your option) any later version.
//   16  *
//   17  * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
//   18  * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   19  * GNU General Public License for more details.
//   20  *
//   21  * You should have received a copy of the GNU General Public License along with this program.
//   22  * If not, see <http://www.gnu.org/licenses/>.
//   23  */
//   24 
//   25 #include "main.h"
//   26 #include "signal_executor.h"
//   27 #include "efiGpio.h"
//   28 #include "engine.h"
//   29 
//   30 /**
//   31  * Signal executors feed digital events right into WaveChart used by Sniffer tab of Dev Console
//   32  */
//   33 #include "rpm_calculator.h"
//   34 
//   35 EXTERN_ENGINE;
//   36 
//   37 #if EFI_WAVE_CHART
//   38 #include "wave_chart.h"
//   39 extern WaveChart waveChart;
//   40 #endif
//   41 
//   42 #include "efiGpio.h"
//   43 
//   44 extern engine_pins_s enginePins;
//   45 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   46 static const char *sparkNames[IGNITION_PIN_COUNT] = { "c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8",
sparkNames:
        DATA
        DC32 `?<Constant "c1">`, `?<Constant "c2">`, `?<Constant "c3">`
        DC32 `?<Constant "c4">`, `?<Constant "c5">`, `?<Constant "c6">`
        DC32 `?<Constant "c7">`, `?<Constant "c8">`, `?<Constant "c9">`
        DC32 `?<Constant "cA">`, `?<Constant "cB">`, `?<Constant "cD">`
//   47 		"c9", "cA", "cB", "cD"};
//   48 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   49 static const char *injectorNames[INJECTION_PIN_COUNT] = { "i1", "i2", "i3", "i4", "i5", "i6", "i7", "i8",
injectorNames:
        DATA
        DC32 `?<Constant "i1">`, `?<Constant "i2">`, `?<Constant "i3">`
        DC32 `?<Constant "i4">`, `?<Constant "i5">`, `?<Constant "i6">`
        DC32 `?<Constant "i7">`, `?<Constant "i8">`, `?<Constant "j9">`
        DC32 `?<Constant "iA">`, `?<Constant "iB">`, `?<Constant "iC">`
//   50 		"j9", "iA", "iB", "iC"};
//   51 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z18initSignalExecutorv
        THUMB
//   52 void initSignalExecutor(void) {
_Z18initSignalExecutorv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   53 	initSignalExecutorImpl();
          CFI FunCall _Z22initSignalExecutorImplv
        BL       _Z22initSignalExecutorImplv
//   54 	for (int i = 0; i < IGNITION_PIN_COUNT;i++) {
        MOVS     R0,#+0
??initSignalExecutor_0:
        CMP      R0,#+12
        BGE.N    ??initSignalExecutor_1
//   55 		enginePins.coils[i].name = sparkNames[i];
        MOVS     R1,#+20
        LDR.N    R2,??DataTable3_2
        MLA      R1,R1,R0,R2
        LDR.N    R2,??DataTable3_3
        LDR      R2,[R2, R0, LSL #+2]
        STR      R2,[R1, #+352]
//   56 	}
        ADDS     R0,R0,#+1
        B.N      ??initSignalExecutor_0
//   57 	for (int i = 0; i < INJECTION_PIN_COUNT;i++) {
??initSignalExecutor_1:
        MOVS     R0,#+0
??initSignalExecutor_2:
        CMP      R0,#+12
        BGE.N    ??initSignalExecutor_3
//   58 		enginePins.injectors[i].name = injectorNames[i];
        MOVS     R1,#+20
        LDR.N    R2,??DataTable3_2
        MLA      R1,R1,R0,R2
        LDR.N    R2,??DataTable3_4
        LDR      R2,[R2, R0, LSL #+2]
        STR      R2,[R1, #+112]
//   59 	}
        ADDS     R0,R0,#+1
        B.N      ??initSignalExecutor_2
//   60 }
??initSignalExecutor_3:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0
//   61 
//   62 //uint32_t dbgStart;
//   63 //uint32_t dbgDurr;
//   64 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z11turnPinHighP14NamedOutputPin
        THUMB
//   65 void turnPinHigh(NamedOutputPin *output) {
_Z11turnPinHighP14NamedOutputPin:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//   66 	efiAssertVoid(output!=NULL, "NULL @ turnPinHigh");
        CMP      R4,#+0
        BNE.N    ??turnPinHigh_0
        LDR.N    R0,??DataTable3_5
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??turnPinHigh_1
//   67 #if EFI_DEFAILED_LOGGING
//   68 //	signal->hi_time = hTimeNow();
//   69 #endif /* EFI_DEFAILED_LOGGING */
//   70 
//   71 #if EFI_GPIO
//   72 	// turn the output level ACTIVE
//   73 	// todo: this XOR should go inside the setOutputPinValue method
//   74 	doSetOutputPinValue2(output, true);
??turnPinHigh_0:
        LDR      R0,[R4, #+0]
        CMP      R0,#+0
        BEQ.N    ??turnPinHigh_2
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BNE.N    ??turnPinHigh_3
        LDR.N    R0,??DataTable3_6
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??turnPinHigh_1
??turnPinHigh_3:
        LDR      R0,[R4, #+8]
        LDR      R0,[R0, #+0]
        MOVS     R5,R0
        CMP      R5,#+4
        BLT.N    ??turnPinHigh_4
        LDR.N    R0,??DataTable3_7
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??turnPinHigh_1
??turnPinHigh_4:
        CMP      R5,#+0
        BEQ.N    ??turnPinHigh_5
        CMP      R5,#+2
        BNE.N    ??turnPinHigh_6
??turnPinHigh_5:
        MOVS     R6,#+1
        B.N      ??turnPinHigh_7
??turnPinHigh_6:
        MOVS     R6,#+0
??turnPinHigh_7:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        LDR      R0,[R4, #+12]
        CMP      R0,#+1
        BEQ.N    ??turnPinHigh_2
        MVNS     R0,R6
        ANDS     R0,R0,#0x1
        LDR      R1,[R4, #+4]
        ADDS     R1,R1,#+16
        LSLS     R0,R0,R1
        ANDS     R1,R6,#0x1
        LDR      R2,[R4, #+4]
        LSLS     R1,R1,R2
        ORRS     R0,R1,R0
        LDR      R1,[R4, #+0]
        STR      R0,[R1, #+24]
        MOVS     R0,#+1
        STR      R0,[R4, #+12]
//   75 	// sleep for the needed duration
//   76 #endif
//   77 #if EFI_WAVE_CHART
//   78 	// explicit check here is a performance optimization to speed up no-chart mode
//   79 	if (CONFIG(isEngineChartEnabled)) {
??turnPinHigh_2:
        LDR.N    R0,??DataTable3_8
        LDRB     R0,[R0, #+1504]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+5
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??turnPinHigh_8
//   80 		// this is a performance optimization - array index is cheaper then invoking a method with 'switch'
//   81 		const char *pinName = output->name;
        LDR      R5,[R4, #+16]
//   82 //	dbgDurr = hal_lld_get_counter_value() - dbgStart;
//   83 
//   84 		addWaveChartEvent(pinName, WC_UP);
        ADR.N    R2,??DataTable3  ;; "u"
        MOVS     R1,R5
        LDR.N    R0,??DataTable3_9
          CFI FunCall _ZN9WaveChart18addWaveChartEvent3EPKcS1_
        BL       _ZN9WaveChart18addWaveChartEvent3EPKcS1_
//   85 	}
//   86 #endif /* EFI_WAVE_ANALYZER */
//   87 //	dbgDurr = hal_lld_get_counter_value() - dbgStart;
//   88 }
??turnPinHigh_8:
??turnPinHigh_1:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock1
//   89 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z10turnPinLowP14NamedOutputPin
        THUMB
//   90 void turnPinLow(NamedOutputPin *output) {
_Z10turnPinLowP14NamedOutputPin:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//   91 	efiAssertVoid(output!=NULL, "NULL turnPinLow");
        CMP      R4,#+0
        BNE.N    ??turnPinLow_0
        LDR.N    R0,??DataTable3_10
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??turnPinLow_1
//   92 #if EFI_GPIO
//   93 	// turn off the output
//   94 	doSetOutputPinValue2(output, false);
??turnPinLow_0:
        LDR      R0,[R4, #+0]
        CMP      R0,#+0
        BEQ.N    ??turnPinLow_2
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BNE.N    ??turnPinLow_3
        LDR.N    R0,??DataTable3_6
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??turnPinLow_1
??turnPinLow_3:
        LDR      R0,[R4, #+8]
        LDR      R0,[R0, #+0]
        MOVS     R5,R0
        CMP      R5,#+4
        BLT.N    ??turnPinLow_4
        LDR.N    R0,??DataTable3_7
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??turnPinLow_1
??turnPinLow_4:
        CMP      R5,#+1
        BEQ.N    ??turnPinLow_5
        CMP      R5,#+3
        BNE.N    ??turnPinLow_6
??turnPinLow_5:
        MOVS     R6,#+1
        B.N      ??turnPinLow_7
??turnPinLow_6:
        MOVS     R6,#+0
??turnPinLow_7:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        LDR      R0,[R4, #+12]
        CMP      R0,#+0
        BEQ.N    ??turnPinLow_2
        MVNS     R0,R6
        ANDS     R0,R0,#0x1
        LDR      R1,[R4, #+4]
        ADDS     R1,R1,#+16
        LSLS     R0,R0,R1
        ANDS     R1,R6,#0x1
        LDR      R2,[R4, #+4]
        LSLS     R1,R1,R2
        ORRS     R0,R1,R0
        LDR      R1,[R4, #+0]
        STR      R0,[R1, #+24]
        MOVS     R0,#+0
        STR      R0,[R4, #+12]
//   95 #endif
//   96 
//   97 #if EFI_DEFAILED_LOGGING
//   98 	systime_t after = hTimeNow();
//   99 	debugInt(&signal->logging, "a_time", after - signal->hi_time);
//  100 	scheduleLogging(&signal->logging);
//  101 #endif /* EFI_DEFAILED_LOGGING */
//  102 
//  103 #if EFI_WAVE_CHART
//  104 	if (CONFIG(isEngineChartEnabled)) {
??turnPinLow_2:
        LDR.N    R0,??DataTable3_8
        LDRB     R0,[R0, #+1504]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+5
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??turnPinLow_8
//  105 		// this is a performance optimization - array index is cheaper then invoking a method with 'switch'
//  106 		const char *pinName = output->name;
        LDR      R5,[R4, #+16]
//  107 
//  108 		addWaveChartEvent(pinName, WC_DOWN);
        ADR.N    R2,??DataTable3_1  ;; "d"
        MOVS     R1,R5
        LDR.N    R0,??DataTable3_9
          CFI FunCall _ZN9WaveChart18addWaveChartEvent3EPKcS1_
        BL       _ZN9WaveChart18addWaveChartEvent3EPKcS1_
//  109 	}
//  110 #endif /* EFI_WAVE_ANALYZER */
//  111 }
??turnPinLow_8:
??turnPinLow_1:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock2
//  112 
//  113 int getRevolutionCounter(void);
//  114 
//  115 /**
//  116  *
//  117  * @param	delay	the number of ticks before the output signal
//  118  * 					immediate output if delay is zero
//  119  * @param	dwell	the number of ticks of output duration
//  120  *
//  121  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z14scheduleOutputP19OutputSignal_structxff
        THUMB
//  122 void scheduleOutput(OutputSignal *signal, efitimeus_t nowUs, float delayUs, float durationUs) {
_Z14scheduleOutputP19OutputSignal_structxff:
        PUSH     {R3-R9,LR}
          CFI R14 Frame(CFA, -4)
          CFI R9 Frame(CFA, -8)
          CFI R8 Frame(CFA, -12)
          CFI R7 Frame(CFA, -16)
          CFI R6 Frame(CFA, -20)
          CFI R5 Frame(CFA, -24)
          CFI R4 Frame(CFA, -28)
          CFI CFA R13+32
        VPUSH    {D8}
          CFI D8 Frame(CFA, -40)
          CFI CFA R13+40
        SUB      SP,SP,#+8
          CFI CFA R13+48
        MOVS     R6,R0
        MOVS     R4,R2
        MOVS     R5,R3
        VMOV.F32 S17,S0
        VMOV.F32 S16,S1
//  123 #if EFI_GPIO
//  124 	if (durationUs < 0) {
        VCMP.F32 S16,#0.0
        FMSTAT   
        BPL.N    ??scheduleOutput_0
//  125 		warning(OBD_PCM_Processor_Fault, "duration cannot be negative: %d", durationUs);
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable3_11
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  126 		return;
        B.N      ??scheduleOutput_1
//  127 	}
//  128 	if (cisnan(durationUs)) {
??scheduleOutput_0:
        VMOV.F32 S0,S16
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BNE.N    ??scheduleOutput_2
//  129 		warning(OBD_PCM_Processor_Fault, "NaN in scheduleOutput", durationUs);
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable3_12
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  130 		return;
        B.N      ??scheduleOutput_1
//  131 	}
//  132 
//  133 	efiAssertVoid(signal!=NULL, "signal is NULL");
??scheduleOutput_2:
        CMP      R6,#+0
        BNE.N    ??scheduleOutput_3
        LDR.N    R0,??DataTable3_13
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??scheduleOutput_1
//  134 	int index = getRevolutionCounter() % 2;
??scheduleOutput_3:
          CFI FunCall _Z20getRevolutionCounterv
        BL       _Z20getRevolutionCounterv
        MOVS     R1,#+2
        SDIV     R7,R0,R1
        MLS      R7,R7,R1,R0
//  135 	scheduling_s * sUp = &signal->signalTimerUp[index];
        MOVS     R0,#+24
        MLA      R0,R0,R7,R6
        ADDS     R8,R0,#+8
//  136 	scheduling_s * sDown = &signal->signalTimerDown[index];
        MOVS     R0,#+24
        MLA      R0,R0,R7,R6
        ADDS     R9,R0,#+56
//  137 
//  138 	scheduleByTime("out up", sUp, nowUs + (int) delayUs, (schfunc_t) &turnPinHigh, signal->output);
        LDR      R0,[R6, #+0]
        STR      R0,[SP, #+4]
        LDR.N    R0,??DataTable3_14
        STR      R0,[SP, #+0]
        VCVT.S32.F32 S0,S17
        VMOV     R0,S0
        ASRS     R1,R0,#+31
        ADDS     R2,R4,R0
        ADCS     R3,R5,R1
        MOV      R1,R8
        LDR.N    R0,??DataTable3_15
          CFI FunCall _Z14scheduleByTimePKcP12scheduling_sxPFvPvES3_
        BL       _Z14scheduleByTimePKcP12scheduling_sxPFvPvES3_
//  139 	scheduleByTime("out down", sDown, nowUs + (int) (delayUs + durationUs), (schfunc_t) &turnPinLow, signal->output);
        LDR      R0,[R6, #+0]
        STR      R0,[SP, #+4]
        LDR.N    R0,??DataTable3_16
        STR      R0,[SP, #+0]
        VADD.F32 S0,S17,S16
        VCVT.S32.F32 S0,S0
        VMOV     R0,S0
        ASRS     R1,R0,#+31
        ADDS     R2,R4,R0
        ADCS     R3,R5,R1
        MOV      R1,R9
        LDR.N    R0,??DataTable3_17
          CFI FunCall _Z14scheduleByTimePKcP12scheduling_sxPFvPvES3_
        BL       _Z14scheduleByTimePKcP12scheduling_sxPFvPvES3_
//  140 #endif
//  141 }
??scheduleOutput_1:
        ADD      SP,SP,#+8
          CFI CFA R13+40
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+32
        POP      {R0,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3:
        DC8      "u",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_1:
        DC8      "d",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_2:
        DC32     enginePins

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_3:
        DC32     sparkNames

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_4:
        DC32     injectorNames

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_5:
        DC32     `?<Constant "NULL @ turnPinHigh">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_6:
        DC32     `?<Constant "pin mode not initialized">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_7:
        DC32     `?<Constant "invalid pin_output_mo...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_8:
        DC32     persistentState

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_9:
        DC32     waveChart

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_10:
        DC32     `?<Constant "NULL turnPinLow">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_11:
        DC32     `?<Constant "duration cannot be ne...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_12:
        DC32     `?<Constant "NaN in scheduleOutput">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_13:
        DC32     `?<Constant "signal is NULL">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_14:
        DC32     _Z11turnPinHighP14NamedOutputPin

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_15:
        DC32     `?<Constant "out up">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_16:
        DC32     _Z10turnPinLowP14NamedOutputPin

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_17:
        DC32     `?<Constant "out down">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "c1">`:
        DATA
        DC8 "c1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "c2">`:
        DATA
        DC8 "c2"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "c3">`:
        DATA
        DC8 "c3"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "c4">`:
        DATA
        DC8 "c4"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "c5">`:
        DATA
        DC8 "c5"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "c6">`:
        DATA
        DC8 "c6"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "c7">`:
        DATA
        DC8 "c7"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "c8">`:
        DATA
        DC8 "c8"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "c9">`:
        DATA
        DC8 "c9"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "cA">`:
        DATA
        DC8 "cA"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "cB">`:
        DATA
        DC8 "cB"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "cD">`:
        DATA
        DC8 "cD"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "i1">`:
        DATA
        DC8 "i1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "i2">`:
        DATA
        DC8 "i2"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "i3">`:
        DATA
        DC8 "i3"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "i4">`:
        DATA
        DC8 "i4"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "i5">`:
        DATA
        DC8 "i5"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "i6">`:
        DATA
        DC8 "i6"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "i7">`:
        DATA
        DC8 "i7"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "i8">`:
        DATA
        DC8 "i8"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "j9">`:
        DATA
        DC8 "j9"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "iA">`:
        DATA
        DC8 "iA"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "iB">`:
        DATA
        DC8 "iB"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "iC">`:
        DATA
        DC8 "iC"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "NULL @ turnPinHigh">`:
        DATA
        DC8 "NULL @ turnPinHigh"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "pin mode not initialized">`:
        DATA
        DC8 "pin mode not initialized"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid pin_output_mo...">`:
        DATA
        DC8 "invalid pin_output_mode_e"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 "u"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "NULL turnPinLow">`:
        DATA
        DC8 "NULL turnPinLow"

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 "d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "duration cannot be ne...">`:
        DATA
        DC8 "duration cannot be negative: %d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "NaN in scheduleOutput">`:
        DATA
        DC8 "NaN in scheduleOutput"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "signal is NULL">`:
        DATA
        DC8 "signal is NULL"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "out up">`:
        DATA
        DC8 "out up"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "out down">`:
        DATA
        DC8 "out down"
        DC8 0, 0, 0

        END
// 
//  96 bytes in section .data
// 284 bytes in section .rodata
// 628 bytes in section .text
// 
// 628 bytes of CODE  memory
// 284 bytes of CONST memory
//  96 bytes of DATA  memory
//
//Errors: none
//Warnings: 3
