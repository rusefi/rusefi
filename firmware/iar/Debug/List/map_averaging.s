///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:10 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\map /
//                       _averaging.cpp                                       /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\map /
//                       _averaging.cpp -lCN F:\stuff\rusefi_sourceforge\firm /
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
//                       map_averaging.s                                      /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME map_averaging

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z10turnPinLowP14NamedOutputPin
        EXTERN _Z11turnPinHighP14NamedOutputPin
        EXTERN _Z15getMapByVoltagef
        EXTERN _Z15scheduleByAngleiP12scheduling_sfPFvPvES1_P13RpmCalculator
        EXTERN _Z20getCrankshaftAngleNtx
        EXTERN _Z20getRevolutionCounterv
        EXTERN _Z23addTriggerEventListenerPFv15trigger_event_ejEPKcP6Engine
        EXTERN _Z9getRawMapv
        EXTERN _Z9scAddDataff
        EXTERN _ZN12scheduling_sC1Ev
        EXTERN _ZN14NamedOutputPinC1EPKc
        EXTERN __aeabi_vec_ctor_nocookie_nodtor
        EXTERN __iar_FDtest
        EXTERN _engine
        EXTERN addConsoleAction
        EXTERN boardConfiguration
        EXTERN dbg_check_lock_from_isr
        EXTERN dbg_check_unlock_from_isr
        EXTERN engine
        EXTERN engineConfiguration
        EXTERN firmwareError
        EXTERN getRemainingStack
        EXTERN getTimeNowNt
        EXTERN lockAnyContext
        EXTERN persistentState
        EXTERN rlist
        EXTERN scheduleMsg

        PUBLIC _Z13getMapVoltagev
        PUBLIC _Z16initMapAveragingP7LoggingP6Engine
        PUBLIC _Z20mapAveragingCallbackt
        PUBLIC _Z6getMapv
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\map_averaging.cpp
//    1 /**
//    2  * @file	map_averaging.cpp
//    3  *
//    4  * @date Dec 11, 2013
//    5  * @author Andrey Belomutskiy, (c) 2012-2015
//    6  *
//    7  * This file is part of rusEfi - see http://rusefi.com
//    8  *
//    9  * rusEfi is free software; you can redistribute it and/or modify it under the terms of
//   10  * the GNU General Public License as published by the Free Software Foundation; either
//   11  * version 3 of the License, or (at your option) any later version.
//   12  *
//   13  * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
//   14  * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   15  * GNU General Public License for more details.
//   16  *
//   17  * You should have received a copy of the GNU General Public License along with this program.
//   18  * If not, see <http://www.gnu.org/licenses/>.
//   19  */
//   20 
//   21 #include "main.h"
//   22 #include "efilib2.h"
//   23 #include "map.h"
//   24 
//   25 #if EFI_MAP_AVERAGING || defined(__DOXYGEN__)
//   26 
//   27 #include "map_averaging.h"
//   28 #include "trigger_central.h"
//   29 #include "adc_inputs.h"
//   30 #include "engine_state.h"
//   31 #include "engine_configuration.h"
//   32 #include "interpolation.h"
//   33 #include "signal_executor.h"
//   34 #include "engine.h"
//   35 #include "engine_math.h"
//   36 
//   37 #if EFI_ANALOG_CHART
//   38 #include <sensor_chart.h>
//   39 #endif /* EFI_ANALOG_CHART */
//   40 
//   41 #define FAST_MAP_CHART_SKIP_FACTOR 16
//   42 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   43 static Logging *logger;
logger:
        DS8 4

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   44 static NamedOutputPin mapAveragingPin("map");
        ADR.N    R1,??DataTable3  ;; "map"
        LDR.W    R0,??DataTable8
          CFI FunCall _ZN14NamedOutputPinC1EPKc
        BL       _ZN14NamedOutputPinC1EPKc
//   45 
//   46 /**
//   47  * Running counter of measurements per revolution
//   48  */
//   49 static volatile int perRevolutionCounter = 0;
//   50 /**
//   51  * Number of measurements in previous shaft revolution
//   52  */
//   53 static volatile int perRevolution = 0;
//   54 
//   55 /**
//   56  * In this lock-free imlementation 'readIndex' is always pointing
//   57  * to the consistent copy of accumulator and counter pair
//   58  */
//   59 static int readIndex = 0;
//   60 static float accumulators[2];
//   61 static int counters[2];
//   62 
//   63 
//   64 /**
//   65  * Running MAP accumulator
//   66  */
//   67 static volatile float mapAccumulator = 0;
//   68 /**
//   69  * Running counter of measurements to consider for averaging
//   70  */
//   71 static volatile int mapMeasurementsCounter = 0;
//   72 
//   73 /**
//   74  * v_ for Voltage
//   75  */
//   76 static float v_averagedMapValue;
//   77 
//   78 EXTERN_ENGINE;
//   79 
//   80 static scheduling_s startTimer[2];
        MOVS     R3,#+2
        MOVS     R2,#+24
        LDR.W    R1,??DataTable8_1
        LDR.W    R0,??DataTable8_2
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
//   81 static scheduling_s endTimer[2];
        MOVS     R3,#+2
        MOVS     R2,#+24
        LDR.W    R1,??DataTable8_1
        LDR.W    R0,??DataTable8_3
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
mapAveragingPin:
        DS8 20

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
perRevolutionCounter:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
perRevolution:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
readIndex:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
accumulators:
        DS8 8

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
counters:
        DS8 8

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
mapAccumulator:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
mapMeasurementsCounter:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
v_averagedMapValue:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
startTimer:
        DS8 48

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
endTimer:
        DS8 48
//   82 
//   83 /**
//   84  * that's a performance optimization: let's not bother averaging
//   85  * if we are outside of of the window
//   86  */

        SECTION `.bss`:DATA:REORDER:NOROOT(0)
//   87 static bool_t isAveraging = false;
isAveraging:
        DS8 1
//   88 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z14startAveragingPv
        THUMB
//   89 static void startAveraging(void *arg) {
_Z14startAveragingPv:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//   90 	(void) arg;
//   91 	efiAssertVoid(getRemainingStack(chThdSelf()) > 128, "lowstck#9");
        LDR.W    R0,??DataTable8_4
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+129
        BGE.N    ??startAveraging_0
        LDR.W    R0,??DataTable8_5
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??startAveraging_1
//   92 	bool wasLocked = lockAnyContext();
??startAveraging_0:
          CFI FunCall lockAnyContext
        BL       lockAnyContext
        MOVS     R5,R0
//   93 	;
//   94 	// with locking we would have a consistent state
//   95 	mapAccumulator = 0;
        LDR.W    R0,??DataTable8_6
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//   96 	mapMeasurementsCounter = 0;
        LDR.W    R0,??DataTable8_7
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//   97 	isAveraging = true;
        LDR.W    R0,??DataTable8_8
        MOVS     R1,#+1
        STRB     R1,[R0, #+0]
//   98 	if (!wasLocked)
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BNE.N    ??startAveraging_2
//   99 		chSysUnlockFromIsr()
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  100 	;
//  101 	turnPinHigh(&mapAveragingPin);
??startAveraging_2:
        LDR.W    R0,??DataTable8
          CFI FunCall _Z11turnPinHighP14NamedOutputPin
        BL       _Z11turnPinHighP14NamedOutputPin
//  102 }
??startAveraging_1:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock1
//  103 
//  104 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//  105 /**
//  106  * This method is invoked from ADC callback.
//  107  * @note This method is invoked OFTEN, this method is a potential bottle-next - the implementation should be
//  108  * as fast as possible
//  109  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z20mapAveragingCallbackt
        THUMB
//  110 void mapAveragingCallback(adcsample_t adcValue) {
_Z20mapAveragingCallbackt:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        VPUSH    {D8}
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
//  111 	if(!isAveraging && boardConfiguration->sensorChartMode != SC_MAP) {
        LDR.W    R0,??DataTable8_8
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??mapAveragingCallback_0
        LDR.W    R0,??DataTable8_9
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+784]
        CMP      R0,#+2
        BNE.N    ??mapAveragingCallback_1
//  112 		return;
//  113 	}
//  114 
//  115 	/* Calculates the average values from the ADC samples.*/
//  116 	perRevolutionCounter++;
??mapAveragingCallback_0:
        LDR.W    R0,??DataTable8_10
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.W    R1,??DataTable8_10
        STR      R0,[R1, #+0]
//  117 	efiAssertVoid(getRemainingStack(chThdSelf()) > 128, "lowstck#9a");
        LDR.W    R0,??DataTable8_4
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+129
        BGE.N    ??mapAveragingCallback_2
        LDR.W    R0,??DataTable8_11
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??mapAveragingCallback_1
//  118 
//  119 
//  120 #if (EFI_ANALOG_CHART && EFI_ANALOG_SENSORS) || defined(__DOXYGEN__)
//  121 	if (boardConfiguration->sensorChartMode == SC_MAP)
??mapAveragingCallback_2:
        LDR.W    R0,??DataTable8_9
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+784]
        CMP      R0,#+2
        BNE.N    ??mapAveragingCallback_3
//  122 		if (perRevolutionCounter % FAST_MAP_CHART_SKIP_FACTOR == 0) {
        LDR.W    R0,??DataTable8_10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+16
        SDIV     R2,R0,R1
        MLS      R2,R2,R1,R0
        CMP      R2,#+0
        BNE.N    ??mapAveragingCallback_3
//  123 			float voltage = adcToVoltsDivided(adcValue);
        UXTH     R4,R4            ;; ZeroExt  R4,R4,#+16,#+16
        VMOV     S0,R4
        VCVT.F32.U32 S0,S0
        VLDR.W   S1,??DataTable3_1  ;; 0x3a400c01
        VMUL.F32 S0,S0,S1
        LDR.N    R0,??DataTable8_12
        LDR      R0,[R0, #+0]
        VLDR     S1,[R0, #+464]
        VMUL.F32 S16,S0,S1
//  124 			float currentPressure = getMapByVoltage(voltage);
        VMOV.F32 S0,S16
          CFI FunCall _Z15getMapByVoltagef
        BL       _Z15getMapByVoltagef
        VMOV.F32 S17,S0
//  125 			scAddData(getCrankshaftAngleNt(getTimeNowNt() PASS_ENGINE_PARAMETER), currentPressure);
          CFI FunCall getTimeNowNt
        BL       getTimeNowNt
          CFI FunCall _Z20getCrankshaftAngleNtx
        BL       _Z20getCrankshaftAngleNtx
        VMOV.F32 S1,S17
          CFI FunCall _Z9scAddDataff
        BL       _Z9scAddDataff
//  126 		}
//  127 #endif /* EFI_ANALOG_CHART */
//  128 
//  129 	/**
//  130 	 * Local copy is now safe, but it's an overkill: we only
//  131 	 * have one writing thread anyway
//  132 	 */
//  133 	int readIndexLocal = readIndex;
??mapAveragingCallback_3:
        LDR.N    R0,??DataTable8_13
        LDR      R5,[R0, #+0]
//  134 	int writeIndex = readIndexLocal ^ 1;
        EORS     R6,R5,#0x1
//  135 	accumulators[writeIndex] = accumulators[readIndexLocal] + adcValue;
        UXTH     R4,R4            ;; ZeroExt  R4,R4,#+16,#+16
        VMOV     S0,R4
        VCVT.F32.U32 S0,S0
        LDR.N    R0,??DataTable8_14
        ADDS     R0,R0,R5, LSL #+2
        VLDR     S1,[R0, #0]
        VADD.F32 S0,S0,S1
        LDR.N    R0,??DataTable8_14
        ADDS     R0,R0,R6, LSL #+2
        VSTR     S0,[R0, #0]
//  136 	counters[writeIndex] = counters[readIndexLocal] + 1;
        LDR.N    R0,??DataTable8_15
        LDR      R0,[R0, R5, LSL #+2]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable8_15
        STR      R0,[R1, R6, LSL #+2]
//  137 	// this would commit the new pair of values
//  138 	readIndex = writeIndex;
        LDR.N    R0,??DataTable8_13
        STR      R6,[R0, #+0]
//  139 
//  140 	// todo: migrate to the lock-free implementation
//  141 	chSysLockFromIsr()
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  142 	;
//  143 	// with locking we would have a consistent state
//  144 
//  145 	mapAccumulator += adcValue;
        UXTH     R4,R4            ;; ZeroExt  R4,R4,#+16,#+16
        VMOV     S0,R4
        VCVT.F32.U32 S0,S0
        LDR.N    R0,??DataTable8_6
        VLDR     S1,[R0, #0]
        VADD.F32 S0,S0,S1
        LDR.N    R0,??DataTable8_6
        VSTR     S0,[R0, #0]
//  146 	mapMeasurementsCounter++;
        LDR.N    R0,??DataTable8_7
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable8_7
        STR      R0,[R1, #+0]
//  147 	chSysUnlockFromIsr()
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  148 	;
//  149 }
??mapAveragingCallback_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock2
//  150 #endif
//  151 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z12endAveragingPv
        THUMB
//  152 static void endAveraging(void *arg) {
_Z12endAveragingPv:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  153 	(void) arg;
//  154 	bool wasLocked = lockAnyContext();
          CFI FunCall lockAnyContext
        BL       lockAnyContext
        MOVS     R5,R0
//  155 	isAveraging = false;
        LDR.N    R0,??DataTable8_8
        MOVS     R1,#+0
        STRB     R1,[R0, #+0]
//  156 	// with locking we would have a consistent state
//  157 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//  158 	v_averagedMapValue = adcToVoltsDivided(mapAccumulator / mapMeasurementsCounter);
        LDR.N    R0,??DataTable8_7
        VLDR     S0,[R0, #0]
        VCVT.F32.S32 S0,S0
        LDR.N    R0,??DataTable8_6
        VLDR     S1,[R0, #0]
        VDIV.F32 S0,S1,S0
        VLDR.W   S1,??DataTable3_1  ;; 0x3a400c01
        VMUL.F32 S0,S0,S1
        LDR.N    R0,??DataTable8_12
        LDR      R0,[R0, #+0]
        VLDR     S1,[R0, #+464]
        VMUL.F32 S0,S0,S1
        LDR.N    R0,??DataTable8_16
        VSTR     S0,[R0, #0]
//  159 #endif
//  160 	if (!wasLocked)
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BNE.N    ??endAveraging_0
//  161 		chSysUnlockFromIsr()
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  162 	;
//  163 	turnPinLow(&mapAveragingPin);
??endAveraging_0:
        LDR.N    R0,??DataTable8
          CFI FunCall _Z10turnPinLowP14NamedOutputPin
        BL       _Z10turnPinLowP14NamedOutputPin
//  164 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3:
        DC8      "map"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_1:
        DC32     0x3a400c01
//  165 
//  166 /**
//  167  * Shaft Position callback used to schedule start and end of MAP averaging
//  168  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z20mapAveragingCallback15trigger_event_ej
        THUMB
//  169 static void mapAveragingCallback(trigger_event_e ckpEventType, uint32_t index DECLARE_ENGINE_PARAMETER_S) {
_Z20mapAveragingCallback15trigger_event_ej:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        VPUSH    {D8-D9}
          CFI D9 Frame(CFA, -32)
          CFI D8 Frame(CFA, -40)
          CFI CFA R13+40
        SUB      SP,SP,#+8
          CFI CFA R13+48
        MOVS     R4,R0
        MOVS     R5,R1
//  170 	// this callback is invoked on interrupt thread
//  171         UNUSED(ckpEventType);
//  172 	engine->m.beforeMapAveragingCb = GET_TIMESTAMP();
        MOVW     R0,#+9208
        LDR.N    R1,??DataTable8_17
        LDR      R1,[R1, #+0]
        LDR.N    R2,??DataTable8_18  ;; 0xe0001004
        LDR      R2,[R2, #+0]
        STR      R2,[R0, R1]
//  173 	if (index != CONFIG(mapAveragingSchedulingAtIndex))
        LDR.N    R0,??DataTable8_19
        LDR      R0,[R0, #+1656]
        CMP      R5,R0
        BNE.W    ??mapAveragingCallback_4
//  174 		return;
//  175 
//  176 	int rpm = ENGINE(rpmCalculator.rpmValue);
??mapAveragingCallback_5:
        LDR.N    R0,??DataTable8_20
        LDR      R6,[R0, #+0]
//  177 	if (!isValidRpm(rpm))
        CMP      R6,#+1
        BLT.N    ??mapAveragingCallback_6
        MOVW     R0,#+30000
        CMP      R6,R0
        BLT.N    ??mapAveragingCallback_7
//  178 		return;
??mapAveragingCallback_6:
        B.N      ??mapAveragingCallback_4
//  179 
//  180 	perRevolution = perRevolutionCounter;
??mapAveragingCallback_7:
        LDR.N    R0,??DataTable8_21
        LDR.N    R1,??DataTable8_10
        LDR      R1,[R1, #+0]
        STR      R1,[R0, #+0]
//  181 	perRevolutionCounter = 0;
        LDR.N    R0,??DataTable8_10
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  182 
//  183 	angle_t currentAngle = TRIGGER_SHAPE(eventAngles[index]);
        LDR.N    R0,??DataTable8_20
        ADDS     R0,R0,R5, LSL #+2
        LDR      R0,[R0, #+3600]
        VMOV     S16,R0
//  184 
//  185 	angle_t samplingStart = ENGINE(engineState.mapAveragingStart) - currentAngle;
        LDR.N    R0,??DataTable8_22
        VLDR     S0,[R0, #0]
        VSUB.F32 S17,S0,S16
//  186 	fixAngle(samplingStart);
        LDR.N    R0,??DataTable8_19
        LDR      R0,[R0, #+424]
        CMP      R0,#+0
        BNE.N    ??mapAveragingCallback_8
        LDR.N    R0,??DataTable8_23
          CFI FunCall firmwareError
        BL       firmwareError
??mapAveragingCallback_8:
        VCMP.F32 S17,#0.0
        FMSTAT   
        BPL.N    ??mapAveragingCallback_9
        LDR.N    R0,??DataTable8_19
        VLDR     S0,[R0, #+424]
        VCVT.F32.S32 S0,S0
        VADD.F32 S17,S0,S17
        B.N      ??mapAveragingCallback_8
??mapAveragingCallback_9:
        LDR.N    R0,??DataTable8_19
        VLDR     S0,[R0, #+424]
        VCVT.F32.S32 S0,S0
        VCMP.F32 S17,S0
        FMSTAT   
        BLT.N    ??mapAveragingCallback_10
        LDR.N    R0,??DataTable8_19
        VLDR     S0,[R0, #+424]
        VCVT.F32.S32 S0,S0
        VSUB.F32 S17,S17,S0
        B.N      ??mapAveragingCallback_9
//  187 
//  188 	angle_t samplingDuration = ENGINE(engineState.mapAveragingDuration);
??mapAveragingCallback_10:
        LDR.N    R0,??DataTable8_24
        VLDR     S18,[R0, #0]
//  189 	if (samplingDuration <= 0) {
        VCMP.F32 S18,#0.0
        FMSTAT   
        BHI.N    ??mapAveragingCallback_11
//  190 		firmwareError("map sampling angle should be positive");
        LDR.N    R0,??DataTable8_25
          CFI FunCall firmwareError
        BL       firmwareError
//  191 		return;
        B.N      ??mapAveragingCallback_4
//  192 	}
//  193 
//  194 	angle_t samplingEnd = samplingStart + samplingDuration;
??mapAveragingCallback_11:
        VADD.F32 S19,S17,S18
//  195 	fixAngle(samplingEnd);
        LDR.N    R0,??DataTable8_19
        LDR      R0,[R0, #+424]
        CMP      R0,#+0
        BNE.N    ??mapAveragingCallback_12
        LDR.N    R0,??DataTable8_23
          CFI FunCall firmwareError
        BL       firmwareError
??mapAveragingCallback_12:
        VCMP.F32 S19,#0.0
        FMSTAT   
        BPL.N    ??mapAveragingCallback_13
        LDR.N    R0,??DataTable8_19
        VLDR     S0,[R0, #+424]
        VCVT.F32.S32 S0,S0
        VADD.F32 S19,S0,S19
        B.N      ??mapAveragingCallback_12
??mapAveragingCallback_13:
        LDR.N    R0,??DataTable8_19
        VLDR     S0,[R0, #+424]
        VCVT.F32.S32 S0,S0
        VCMP.F32 S19,S0
        FMSTAT   
        BLT.N    ??mapAveragingCallback_14
        LDR.N    R0,??DataTable8_19
        VLDR     S0,[R0, #+424]
        VCVT.F32.S32 S0,S0
        VSUB.F32 S19,S19,S0
        B.N      ??mapAveragingCallback_13
//  196 	if (cisnan(samplingEnd)) {
??mapAveragingCallback_14:
        VMOV.F32 S0,S19
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BEQ.N    ??mapAveragingCallback_4
//  197 		// value is not yet prepared
//  198 		return;
//  199 	}
//  200 
//  201 	int structIndex = getRevolutionCounter() % 2;
??mapAveragingCallback_15:
          CFI FunCall _Z20getRevolutionCounterv
        BL       _Z20getRevolutionCounterv
        MOVS     R1,#+2
        SDIV     R7,R0,R1
        MLS      R7,R7,R1,R0
//  202 	// todo: schedule this based on closest trigger event, same as ignition works
//  203 	scheduleByAngle(rpm, &startTimer[structIndex], samplingStart, startAveraging, NULL, &engine->rpmCalculator);
        LDR.N    R0,??DataTable8_17
        LDR      R0,[R0, #+0]
        STR      R0,[SP, #+0]
        MOVS     R3,#+0
        LDR.N    R2,??DataTable8_26
        VMOV.F32 S0,S17
        MOVS     R0,#+24
        LDR.N    R1,??DataTable8_2
        MLA      R1,R0,R7,R1
        MOVS     R0,R6
          CFI FunCall _Z15scheduleByAngleiP12scheduling_sfPFvPvES1_P13RpmCalculator
        BL       _Z15scheduleByAngleiP12scheduling_sfPFvPvES1_P13RpmCalculator
//  204 	scheduleByAngle(rpm, &endTimer[structIndex], samplingEnd, endAveraging, NULL, &engine->rpmCalculator);
        LDR.N    R0,??DataTable8_17
        LDR      R0,[R0, #+0]
        STR      R0,[SP, #+0]
        MOVS     R3,#+0
        LDR.N    R2,??DataTable8_27
        VMOV.F32 S0,S19
        MOVS     R0,#+24
        LDR.N    R1,??DataTable8_3
        MLA      R1,R0,R7,R1
        MOVS     R0,R6
          CFI FunCall _Z15scheduleByAngleiP12scheduling_sfPFvPvES1_P13RpmCalculator
        BL       _Z15scheduleByAngleiP12scheduling_sfPFvPvES1_P13RpmCalculator
//  205 	engine->m.mapAveragingCbTime = GET_TIMESTAMP() - engine->m.beforeMapAveragingCb;
        LDR.N    R0,??DataTable8_18  ;; 0xe0001004
        LDR      R0,[R0, #+0]
        MOVW     R1,#+9208
        LDR.N    R2,??DataTable8_17
        LDR      R2,[R2, #+0]
        LDR      R1,[R1, R2]
        SUBS     R0,R0,R1
        MOVW     R1,#+9212
        LDR.N    R2,??DataTable8_17
        LDR      R2,[R2, #+0]
        STR      R0,[R1, R2]
//  206 }
??mapAveragingCallback_4:
        ADD      SP,SP,#+8
          CFI CFA R13+40
        VPOP     {D8-D9}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI CFA R13+24
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock4
//  207 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z12showMapStatsv
        THUMB
//  208 static void showMapStats(void) {
_Z12showMapStatsv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  209 	scheduleMsg(logger, "per revolution %d", perRevolution);
        LDR.N    R0,??DataTable8_21
        LDR      R2,[R0, #+0]
        LDR.N    R1,??DataTable8_28
        LDR.N    R0,??DataTable8_29
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  210 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock5
//  211 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z13getMapVoltagev
          CFI NoCalls
        THUMB
//  212 float getMapVoltage(void) {
//  213 	return v_averagedMapValue;
_Z13getMapVoltagev:
        LDR.N    R0,??DataTable8_16
        VLDR     S0,[R0, #0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock6
//  214 }
//  215 
//  216 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//  217 
//  218 /**
//  219  * Because of MAP window averaging, MAP is only available while engine is spinning
//  220  * @return Manifold Absolute Pressure, in kPa
//  221  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z6getMapv
        THUMB
//  222 float getMap(void) {
_Z6getMapv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  223 	if (engineConfiguration->hasFrequencyReportingMapSensor) {
        LDR.N    R0,??DataTable8_12
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1498]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+2
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??getMap_0
//  224 		return getRawMap();
          CFI FunCall _Z9getRawMapv
        BL       _Z9getRawMapv
        B.N      ??getMap_1
//  225 	}
//  226 
//  227 #if EFI_ANALOG_SENSORS || defined(__DOXYGEN__)
//  228 	if (!isValidRpm(engine->rpmCalculator.rpmValue))
??getMap_0:
        LDR.N    R0,??DataTable8_17
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+0]
        CMP      R0,#+1
        BLT.N    ??getMap_2
        LDR.N    R0,??DataTable8_17
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+0]
        MOVW     R1,#+30000
        CMP      R0,R1
        BLT.N    ??getMap_3
//  229 		return getRawMap(); // maybe return NaN in case of stopped engine?
??getMap_2:
          CFI FunCall _Z9getRawMapv
        BL       _Z9getRawMapv
        B.N      ??getMap_1
//  230 	return getMapByVoltage(v_averagedMapValue);
??getMap_3:
        LDR.N    R0,??DataTable8_16
        VLDR     S0,[R0, #0]
          CFI FunCall _Z15getMapByVoltagef
        BL       _Z15getMapByVoltagef
??getMap_1:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock7
//  231 #else
//  232 	return 100;
//  233 #endif
//  234 }
//  235 #endif /* EFI_PROD_CODE */
//  236 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z16initMapAveragingP7LoggingP6Engine
        THUMB
//  237 void initMapAveraging(Logging *sharedLogger, Engine *engine) {
_Z16initMapAveragingP7LoggingP6Engine:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  238 	logger = sharedLogger;
        LDR.N    R0,??DataTable8_29
        STR      R4,[R0, #+0]
//  239 
//  240 //	startTimer[0].name = "map start0";
//  241 //	startTimer[1].name = "map start1";
//  242 //	endTimer[0].name = "map end0";
//  243 //	endTimer[1].name = "map end1";
//  244 
//  245 	addTriggerEventListener(&mapAveragingCallback, "MAP averaging", engine);
        MOVS     R2,R5
        LDR.N    R1,??DataTable8_30
        LDR.N    R0,??DataTable8_31
          CFI FunCall _Z23addTriggerEventListenerPFv15trigger_event_ejEPKcP6Engine
        BL       _Z23addTriggerEventListenerPFv15trigger_event_ejEPKcP6Engine
//  246 	addConsoleAction("faststat", showMapStats);
        LDR.N    R1,??DataTable8_32
        LDR.N    R0,??DataTable8_33
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  247 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8:
        DC32     mapAveragingPin

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_1:
        DC32     _ZN12scheduling_sC1Ev

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_2:
        DC32     startTimer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_3:
        DC32     endTimer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_4:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_5:
        DC32     `?<Constant "lowstck#9">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_6:
        DC32     mapAccumulator

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_7:
        DC32     mapMeasurementsCounter

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_8:
        DC32     isAveraging

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_9:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_10:
        DC32     perRevolutionCounter

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_11:
        DC32     `?<Constant "lowstck#9a">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_12:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_13:
        DC32     readIndex

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_14:
        DC32     accumulators

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_15:
        DC32     counters

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_16:
        DC32     v_averagedMapValue

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_17:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_18:
        DC32     0xe0001004

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_19:
        DC32     persistentState

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_20:
        DC32     _engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_21:
        DC32     perRevolution

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_22:
        DC32     _engine+0x1EAC

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_23:
        DC32     `?<Constant "zero engineCycle">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_24:
        DC32     _engine+0x1EB0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_25:
        DC32     `?<Constant "map sampling angle sh...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_26:
        DC32     _Z14startAveragingPv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_27:
        DC32     _Z12endAveragingPv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_28:
        DC32     `?<Constant "per revolution %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_29:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_30:
        DC32     `?<Constant "MAP averaging">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_31:
        DC32     _Z20mapAveragingCallback15trigger_event_ej

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_32:
        DC32     _Z12showMapStatsv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_33:
        DC32     `?<Constant "faststat">`

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
`?<Constant "lowstck#9">`:
        DATA
        DC8 "lowstck#9"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lowstck#9a">`:
        DATA
        DC8 "lowstck#9a"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "zero engineCycle">`:
        DATA
        DC8 "zero engineCycle"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "map sampling angle sh...">`:
        DATA
        DC8 "map sampling angle should be positive"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "per revolution %d">`:
        DATA
        DC8 "per revolution %d"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MAP averaging">`:
        DATA
        DC8 "MAP averaging"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "faststat">`:
        DATA
        DC8 "faststat"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "map"

        END
//  248 
//  249 #else
//  250 
//  251 #if EFI_PROD_CODE
//  252 
//  253 float getMap(void) {
//  254 #if EFI_ANALOG_SENSORS || defined(__DOXYGEN__)
//  255 	return getRawMap();
//  256 #else
//  257 	return NAN;
//  258 #endif /* EFI_ANALOG_SENSORS */
//  259 }
//  260 #endif /* EFI_PROD_CODE */
//  261 
//  262 #endif /* EFI_MAP_AVERAGING */
// 
//   161 bytes in section .bss
//     4 bytes in section .init_array
//   136 bytes in section .rodata
// 1 164 bytes in section .text
// 
// 1 168 bytes of CODE  memory
//   136 bytes of CONST memory
//   161 bytes of DATA  memory
//
//Errors: none
//Warnings: 2
