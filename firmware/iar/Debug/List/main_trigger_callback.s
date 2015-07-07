///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:32 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\tri /
//                       gger\main_trigger_callback.cpp                       /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\tri /
//                       gger\main_trigger_callback.cpp -lCN                  /
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
//                       main_trigger_callback.s                              /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME main_trigger_callback

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z10turnPinLowP14NamedOutputPin
        EXTERN _Z11turnPinHighP14NamedOutputPin
        EXTERN _Z12scheduleTaskPKcP12scheduling_siPFvPvES3_
        EXTERN _Z14getEngineLoadTv
        EXTERN _Z14printHistogramP7LoggingP11histogram_s
        EXTERN _Z14scheduleOutputP19OutputSignal_structxff
        EXTERN _Z18isInjectionEnabledP22engine_configuration_s
        EXTERN _Z19findTriggerPositionP24event_trigger_position_sf
        EXTERN _Z19getInternalAdcValuePKc13adc_channel_e
        EXTERN _Z20getRevolutionCounterv
        EXTERN _Z20prepareOutputSignalsv
        EXTERN _Z22showWaveChartHistogramv
        EXTERN _Z23addTriggerEventListenerPFv15trigger_event_ejEPKcP6Engine
        EXTERN _Z25initializeIgnitionActionsffP9ArrayListI13IgnitionEventLi80EE
        EXTERN _Z26printAllCallbacksHistogramv
        EXTERN _Z9getFuelMsi
        EXTERN _ZN12FuelSchedule13addFuelEventsE16injection_mode_e
        EXTERN _ZN13RpmCalculator20getRevolutionCounterEv
        EXTERN _ZN13RpmCalculator3rpmEv
        EXTERN _ZN16AccelEnrichmemnt13onEngineCycleEv
        EXTERN _ZN16AccelEnrichmemnt16onEngineCycleTpsEv
        EXTERN _ZN18LocalVersionHolder5isOldEv
        EXTERN _ZN18LocalVersionHolderC1Ev
        EXTERN _ZN6Engine10knockLogicEf
        EXTERN _ZNK12TriggerShape7getSizeEv
        EXTERN _ZNK12TriggerShape9getLengthEv
        EXTERN __aeabi_atexit
        EXTERN __aeabi_d2f
        EXTERN __aeabi_ddiv
        EXTERN __aeabi_f2d
        EXTERN __aeabi_i2d
        EXTERN __aeabi_memset
        EXTERN __dso_handle
        EXTERN __iar_FDtest
        EXTERN _engine
        EXTERN addConsoleAction
        EXTERN addConsoleActionP
        EXTERN currentTimeMillis
        EXTERN engine
        EXTERN engineConfiguration
        EXTERN enginePins
        EXTERN firmwareError
        EXTERN getRemainingStack
        EXTERN getTimeNowUs
        EXTERN hasFirmwareErrorFlag
        EXTERN persistentState
        EXTERN printMsg
        EXTERN rlist
        EXTERN scheduleMsg
        EXTERN warning

        PUBLIC _Z15assertNotInListI13IgnitionEventEbPT_S2_
        PUBLIC _Z19mainTriggerCallback15trigger_event_ej
        PUBLIC _Z21initMainEventListenerP7LoggingP6Engine
        PUBLIC _Z21isIgnitionTimingErrorv
        PUBLIC _ZN13cyclic_bufferIiE3addEi
        PUBLIC _ZN13cyclic_bufferIiE3sumEi
        PUBLIC _ZN13cyclic_bufferIiE5baseCEi
        PUBLIC _ZN13cyclic_bufferIiE5clearEv
        PUBLIC _ZN13cyclic_bufferIiE7setSizeEi
        PUBLIC _ZN13cyclic_bufferIiEC1Ev
        PUBLIC _ZN13cyclic_bufferIiEC2Ev
        PUBLIC _ZN13cyclic_bufferIiED1Ev
        PUBLIC _ZN13cyclic_bufferIiED2Ev
        PUBLIC _ZN19MainTriggerCallback4initEP6Engine
        PUBLIC _ZZ15assertNotInListI13IgnitionEventEbPT_S2_Es
        PUBLIC _ZZ15assertNotInListI13IgnitionEventEbPT_S2_Es_0
        PUBLIC cyccnt
        PUBLIC memset
        PUBLIC showMainHistogram
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\trigger\main_trigger_callback.cpp
//    1 /**
//    2  * @file    main_trigger_callback.cpp
//    3  * @brief   Main logic is here!
//    4  *
//    5  * See http://rusefi.com/docs/html/
//    6  *
//    7  * @date Feb 7, 2013
//    8  * @author Andrey Belomutskiy, (c) 2012-2015
//    9  *
//   10  * This file is part of rusEfi - see http://rusefi.com
//   11  *
//   12  * rusEfi is free software; you can redistribute it and/or modify it under the terms of
//   13  * the GNU General Public License as published by the Free Software Foundation; either
//   14  * version 3 of the License, or (at your option) any later version.
//   15  *
//   16  * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
//   17  * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   18  * GNU General Public License for more details.
//   19  *
//   20  * You should have received a copy of the GNU General Public License along with this program.
//   21  * If not, see <http://www.gnu.org/licenses/>.
//   22  */
//   23 
//   24 #include "main.h"
//   25 #if EFI_PROD_CODE
//   26 #include <nvic.h>
//   27 #endif
//   28 
//   29 #if !EFI_PROD_CODE && !EFI_SIMULATOR
//   30 
//   31 #define chThdSelf() 0
//   32 #define getRemainingStack(x) (999999)
//   33 
//   34 #endif
//   35 
//   36 #if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
//   37 
//   38 #include "main_trigger_callback.h"
//   39 #include "efiGpio.h"
//   40 #include "engine_math.h"
//   41 #include "trigger_central.h"
//   42 #include "rpm_calculator.h"
//   43 #include "signal_executor.h"
//   44 #include "engine_configuration.h"
//   45 #include "interpolation.h"
//   46 #include "advance_map.h"
//   47 #include "allsensors.h"
//   48 #include "cyclic_buffer.h"
//   49 #include "histogram.h"
//   50 #include "fuel_math.h"
//   51 #include "histogram.h"
//   52 #if EFI_PROD_CODE
//   53 #include "rfiutil.h"
//   54 #endif /* EFI_HISTOGRAMS */
//   55 #include "LocalVersionHolder.h"
//   56 #include "event_queue.h"
//   57 #include "engine.h"
//   58 #include "efilib2.h"
//   59 
//   60 EXTERN_ENGINE
//   61 ;
//   62 extern bool hasFirmwareErrorFlag;
//   63 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   64 static LocalVersionHolder triggerVersion;
        LDR.W    R0,??DataTable13
          CFI FunCall _ZN18LocalVersionHolderC1Ev
        BL       _ZN18LocalVersionHolderC1Ev
//   65 
//   66 extern engine_pins_s enginePins;
//   67 static MainTriggerCallback mainTriggerCallbackInstance;
//   68 
//   69 /**
//   70  * That's the list of pending spark firing events
//   71  */
//   72 static IgnitionEvent *iHead = NULL;
//   73 
//   74 /**
//   75  * In order to archive higher event precision, we are using a hybrid approach
//   76  * where we are scheduling events based on the closest trigger event with a time offset.
//   77  *
//   78  * This queue is using global trigger event index as 'time'
//   79  */
//   80 //static EventQueue triggerEventsQueue;
//   81 static cyclic_buffer<int> ignitionErrorDetection;
        LDR.W    R0,??DataTable13_1
          CFI FunCall _ZN13cyclic_bufferIiEC1Ev
        BL       _ZN13cyclic_bufferIiEC1Ev
        LDR.W    R2,??DataTable13_2
        LDR.W    R1,??DataTable13_3
          CFI FunCall __aeabi_atexit
        BL       __aeabi_atexit
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
triggerVersion:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
mainTriggerCallbackInstance:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
iHead:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
ignitionErrorDetection:
        DS8 268
//   82 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   83 static Logging *logger;
logger:
        DS8 4
//   84 
//   85 // todo: figure out if this even helps?
//   86 //#if defined __GNUC__
//   87 //#define RAM_METHOD_PREFIX __attribute__((section(".ram")))
//   88 //#else
//   89 //#define RAM_METHOD_PREFIX
//   90 //#endif
//   91 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z26startSimultaniousInjectionP6Engine
        THUMB
//   92 static void startSimultaniousInjection(Engine *engine) {
_Z26startSimultaniousInjectionP6Engine:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//   93 	for (int i = 0; i < engine->engineConfiguration->specs.cylindersCount; i++) {
        MOVS     R5,#+0
??startSimultaniousInjection_0:
        LDR      R0,[R4, #+36]
        LDR      R0,[R0, #+404]
        CMP      R5,R0
        BGE.N    ??startSimultaniousInjection_1
//   94 		turnPinHigh(&enginePins.injectors[i]);
        MOVS     R0,#+20
        LDR.W    R1,??DataTable13_4
        MLA      R0,R0,R5,R1
        ADDS     R0,R0,#+96
          CFI FunCall _Z11turnPinHighP14NamedOutputPin
        BL       _Z11turnPinHighP14NamedOutputPin
//   95 	}
        ADDS     R5,R5,#+1
        B.N      ??startSimultaniousInjection_0
//   96 }
??startSimultaniousInjection_1:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock1
//   97 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z24endSimultaniousInjectionP6Engine
        THUMB
//   98 static void endSimultaniousInjection(Engine *engine) {
_Z24endSimultaniousInjectionP6Engine:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//   99 	for (int i = 0; i < engine->engineConfiguration->specs.cylindersCount; i++) {
        MOVS     R5,#+0
??endSimultaniousInjection_0:
        LDR      R0,[R4, #+36]
        LDR      R0,[R0, #+404]
        CMP      R5,R0
        BGE.N    ??endSimultaniousInjection_1
//  100 		turnPinLow(&enginePins.injectors[i]);
        MOVS     R0,#+20
        LDR.W    R1,??DataTable13_4
        MLA      R0,R0,R5,R1
        ADDS     R0,R0,#+96
          CFI FunCall _Z10turnPinLowP14NamedOutputPin
        BL       _Z10turnPinLowP14NamedOutputPin
//  101 	}
        ADDS     R5,R5,#+1
        B.N      ??endSimultaniousInjection_0
//  102 }
??endSimultaniousInjection_1:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2
//  103 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z24handleFuelInjectionEventP14InjectionEventi
        THUMB
//  104 static ALWAYS_INLINE void handleFuelInjectionEvent(InjectionEvent *event, int rpm DECLARE_ENGINE_PARAMETER_S) {
_Z24handleFuelInjectionEventP14InjectionEventi:
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
        MOVS     R4,R0
        MOVS     R5,R1
//  105 	floatms_t injectionDuration = ENGINE(fuelMs);
        LDR.W    R0,??DataTable13_5
        VLDR     S16,[R0, #+608]
//  106 	if (cisnan(injectionDuration)) {
        VMOV.F32 S0,S16
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BNE.N    ??handleFuelInjectionEvent_0
//  107 		warning(OBD_PCM_Processor_Fault, "NaN injection pulse");
        LDR.W    R1,??DataTable13_6
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  108 		return;
        B.N      ??handleFuelInjectionEvent_1
//  109 	}
//  110 	if (injectionDuration < 0) {
??handleFuelInjectionEvent_0:
        VCMP.F32 S16,#0.0
        FMSTAT   
        BPL.N    ??handleFuelInjectionEvent_2
//  111 		warning(OBD_PCM_Processor_Fault, "Negative injection pulse %f", injectionDuration);
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable13_7
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  112 		return;
        B.N      ??handleFuelInjectionEvent_1
//  113 	}
//  114 
//  115 	if (engine->isCylinderCleanupMode)
??handleFuelInjectionEvent_2:
        MOVW     R0,#+7928
        LDR.W    R1,??DataTable13_8
        LDR      R1,[R1, #+0]
        LDRB     R0,[R0, R1]
        CMP      R0,#+0
        BNE.N    ??handleFuelInjectionEvent_1
//  116 		return;
//  117 
//  118 	floatus_t injectionStartDelayUs = ENGINE(rpmCalculator.oneDegreeUs) * event->injectionStart.angleOffset;
??handleFuelInjectionEvent_3:
        LDR.W    R0,??DataTable13_5
        VLDR     S0,[R0, #+8]
        VLDR     S1,[R4, #+8]
        VMUL.F32 S17,S0,S1
//  119 
//  120 	if (event->isSimultanious) {
        LDRB     R0,[R4, #+120]
        CMP      R0,#+0
        BEQ.N    ??handleFuelInjectionEvent_4
//  121 		if (injectionDuration < 0) {
        VCMP.F32 S16,#0.0
        FMSTAT   
        BPL.N    ??handleFuelInjectionEvent_5
//  122 			firmwareError("duration cannot be negative: %d", injectionDuration);
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R0,??DataTable13_9
          CFI FunCall firmwareError
        BL       firmwareError
//  123 			return;
        B.N      ??handleFuelInjectionEvent_1
//  124 		}
//  125 		if (cisnan(injectionDuration)) {
??handleFuelInjectionEvent_5:
        VMOV.F32 S0,S16
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BNE.N    ??handleFuelInjectionEvent_6
//  126 			firmwareError("NaN in scheduleOutput", injectionDuration);
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R0,??DataTable13_10
          CFI FunCall firmwareError
        BL       firmwareError
//  127 			return;
        B.N      ??handleFuelInjectionEvent_1
//  128 		}
//  129 		/**
//  130 		 * this is pretty much copy-paste of 'scheduleOutput'
//  131 		 * 'scheduleOutput' is currently only used for injection, so maybe it should be
//  132 		 * changed into 'scheduleInjection' and unified? todo: think about it.
//  133 		 */
//  134 		OutputSignal *signal = &event->actuator;
??handleFuelInjectionEvent_6:
        ADDS     R6,R4,#+16
//  135 		efiAssertVoid(signal!=NULL, "signal is NULL");
        CMP      R6,#+0
        BNE.N    ??handleFuelInjectionEvent_7
        LDR.W    R0,??DataTable13_11
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??handleFuelInjectionEvent_1
//  136 		int index = getRevolutionCounter() % 2;
??handleFuelInjectionEvent_7:
          CFI FunCall _Z20getRevolutionCounterv
        BL       _Z20getRevolutionCounterv
        MOVS     R1,#+2
        SDIV     R7,R0,R1
        MLS      R7,R7,R1,R0
//  137 		scheduling_s * sUp = &signal->signalTimerUp[index];
        MOVS     R0,#+24
        MLA      R0,R0,R7,R6
        ADDS     R8,R0,#+8
//  138 		scheduling_s * sDown = &signal->signalTimerDown[index];
        MOVS     R0,#+24
        MLA      R0,R0,R7,R6
        ADDS     R9,R0,#+56
//  139 
//  140 		scheduleTask("out up", sUp, (int) injectionStartDelayUs, (schfunc_t) &startSimultaniousInjection, engine);
        LDR.W    R0,??DataTable13_8
        LDR      R0,[R0, #+0]
        STR      R0,[SP, #+0]
        LDR.W    R3,??DataTable13_12
        VCVT.S32.F32 S0,S17
        VMOV     R2,S0
        MOV      R1,R8
        LDR.W    R0,??DataTable13_13
          CFI FunCall _Z12scheduleTaskPKcP12scheduling_siPFvPvES3_
        BL       _Z12scheduleTaskPKcP12scheduling_siPFvPvES3_
//  141 		scheduleTask("out down", sDown, (int) injectionStartDelayUs + MS2US(injectionDuration), (schfunc_t) &endSimultaniousInjection, engine);
        LDR.W    R0,??DataTable13_8
        LDR      R0,[R0, #+0]
        STR      R0,[SP, #+0]
        LDR.W    R3,??DataTable13_14
        VCVT.S32.F32 S0,S17
        VCVT.F32.S32 S0,S0
        VLDR.W   S1,??DataTable5  ;; 0x447a0000
        VMLA.F32 S0,S16,S1
        VCVT.S32.F32 S0,S0
        VMOV     R2,S0
        MOV      R1,R9
        LDR.W    R0,??DataTable13_15
          CFI FunCall _Z12scheduleTaskPKcP12scheduling_siPFvPvES3_
        BL       _Z12scheduleTaskPKcP12scheduling_siPFvPvES3_
        B.N      ??handleFuelInjectionEvent_8
//  142 
//  143 	} else {
//  144 		scheduleOutput(&event->actuator, getTimeNowUs(), injectionStartDelayUs, MS2US(injectionDuration));
??handleFuelInjectionEvent_4:
          CFI FunCall getTimeNowUs
        BL       getTimeNowUs
        VLDR.W   S0,??DataTable5  ;; 0x447a0000
        VMUL.F32 S1,S16,S0
        VMOV.F32 S0,S17
        MOVS     R2,R0
        MOVS     R3,R1
        ADDS     R0,R4,#+16
          CFI FunCall _Z14scheduleOutputP19OutputSignal_structxff
        BL       _Z14scheduleOutputP19OutputSignal_structxff
//  145 	}
//  146 }
??handleFuelInjectionEvent_8:
??handleFuelInjectionEvent_1:
        ADD      SP,SP,#+8
          CFI CFA R13+40
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+32
        POP      {R0,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock3
//  147 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z10handleFuelji
        THUMB
//  148 static ALWAYS_INLINE void handleFuel(uint32_t eventIndex, int rpm DECLARE_ENGINE_PARAMETER_S) {
_Z10handleFuelji:
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
//  149 	if (!isInjectionEnabled(engine->engineConfiguration))
        LDR.W    R0,??DataTable13_8
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+36]
          CFI FunCall _Z18isInjectionEnabledP22engine_configuration_s
        BL       _Z18isInjectionEnabledP22engine_configuration_s
        CMP      R0,#+0
        BEQ.N    ??handleFuel_0
//  150 		return;
//  151 	efiAssertVoid(getRemainingStack(chThdSelf()) > 128, "lowstck#3");
??handleFuel_1:
        LDR.W    R0,??DataTable13_16
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+129
        BGE.N    ??handleFuel_2
        LDR.W    R0,??DataTable13_17
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??handleFuel_0
//  152 	efiAssertVoid(eventIndex < engine->triggerShape.getLength(), "handleFuel/event index");
??handleFuel_2:
        LDR.W    R0,??DataTable13_8
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+644
          CFI FunCall _ZNK12TriggerShape9getLengthEv
        BL       _ZNK12TriggerShape9getLengthEv
        CMP      R4,R0
        BCC.N    ??handleFuel_3
        LDR.W    R0,??DataTable13_18
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??handleFuel_0
//  153 
//  154 	/**
//  155 	 * Ignition events are defined by addFuelEvents() according to selected
//  156 	 * fueling strategy
//  157 	 */
//  158 	FuelSchedule *fs =
//  159 	isCrankingR(rpm) ?
//  160 			&ENGINE(engineConfiguration2)->crankingInjectionEvents : &engine->engineConfiguration2->injectionEvents;
??handleFuel_3:
        CMP      R5,#+1
        BLT.N    ??handleFuel_4
        LDR.W    R0,??DataTable13_19
        LDRSH    R0,[R0, #+104]
        CMP      R5,R0
        BGE.N    ??handleFuel_4
        LDR.W    R0,??DataTable13_5
        LDR      R6,[R0, #+40]
        B.N      ??handleFuel_5
??handleFuel_4:
        LDR.W    R0,??DataTable13_8
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+40]
        ADD      R6,R0,#+5888
        ADDS     R6,R6,#+136
//  161 
//  162 	InjectionEventList *source = &fs->events;
??handleFuel_5:
        MOVS     R7,R6
//  163 
//  164 	if (!fs->hasEvents[eventIndex])
        MOVW     R0,#+5768
        ADDS     R1,R4,R6
        LDRB     R0,[R0, R1]
        CMP      R0,#+0
        BEQ.N    ??handleFuel_0
//  165 		return;
//  166 
//  167 	engine->tpsAccelEnrichment.onEngineCycleTps(PASS_ENGINE_PARAMETER_F);
??handleFuel_6:
        LDR.W    R0,??DataTable13_8
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+332
          CFI FunCall _ZN16AccelEnrichmemnt16onEngineCycleTpsEv
        BL       _ZN16AccelEnrichmemnt16onEngineCycleTpsEv
//  168 
//  169 	engine->mapAccelEnrichment.onEngineCycle(PASS_ENGINE_PARAMETER_F);
        LDR.W    R0,??DataTable13_8
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+56
          CFI FunCall _ZN16AccelEnrichmemnt13onEngineCycleEv
        BL       _ZN16AccelEnrichmemnt13onEngineCycleEv
//  170 	ENGINE(fuelMs) = getFuelMs(rpm PASS_ENGINE_PARAMETER) * engineConfiguration->globalFuelCorrection;
        MOVS     R0,R5
          CFI FunCall _Z9getFuelMsi
        BL       _Z9getFuelMsi
        LDR.W    R0,??DataTable13_20
        LDR      R0,[R0, #+0]
        VLDR     S1,[R0, #+568]
        VMUL.F32 S0,S0,S1
        LDR.W    R0,??DataTable13_5
        VSTR     S0,[R0, #+608]
//  171 
//  172 	for (int i = 0; i < source->size; i++) {
        MOVS     R8,#+0
??handleFuel_7:
        LDR      R0,[R7, #+0]
        CMP      R8,R0
        BGE.N    ??handleFuel_8
//  173 		InjectionEvent *event = &source->elements[i];
        MOVS     R0,#+128
        MLA      R0,R0,R8,R7
        ADDS     R9,R0,#+8
//  174 		if (event->injectionStart.eventIndex != eventIndex)
        LDR      R0,[R9, #+0]
        CMP      R0,R4
        BNE.N    ??handleFuel_9
//  175 			continue;
//  176 		handleFuelInjectionEvent(event, rpm PASS_ENGINE_PARAMETER);
??handleFuel_10:
        MOVS     R1,R5
        MOV      R0,R9
          CFI FunCall _Z24handleFuelInjectionEventP14InjectionEventi
        BL       _Z24handleFuelInjectionEventP14InjectionEventi
//  177 	}
??handleFuel_9:
        ADDS     R8,R8,#+1
        B.N      ??handleFuel_7
//  178 }
??handleFuel_8:
??handleFuel_0:
        POP      {R0,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock4
//  179 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z16handleSparkEventjP13IgnitionEventi
        THUMB
//  180 static ALWAYS_INLINE void handleSparkEvent(uint32_t eventIndex, IgnitionEvent *iEvent,
//  181 		int rpm DECLARE_ENGINE_PARAMETER_S) {
_Z16handleSparkEventjP13IgnitionEventi:
        PUSH     {R3-R9,LR}
          CFI R14 Frame(CFA, -4)
          CFI R9 Frame(CFA, -8)
          CFI R8 Frame(CFA, -12)
          CFI R7 Frame(CFA, -16)
          CFI R6 Frame(CFA, -20)
          CFI R5 Frame(CFA, -24)
          CFI R4 Frame(CFA, -28)
          CFI CFA R13+32
        VPUSH    {D8-D9}
          CFI D9 Frame(CFA, -40)
          CFI D8 Frame(CFA, -48)
          CFI CFA R13+48
        SUB      SP,SP,#+8
          CFI CFA R13+56
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  182 
//  183 	float dwellMs = engine->engineState.sparkDwell;
        LDR.W    R0,??DataTable13_8
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+7680
        ADDS     R0,R0,#+180
        VLDR     S16,[R0, #0]
//  184 	if (cisnan(dwellMs) || dwellMs < 0) {
        VMOV.F32 S0,S16
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BEQ.N    ??handleSparkEvent_0
        VCMP.F32 S16,#0.0
        FMSTAT   
        BPL.N    ??handleSparkEvent_1
//  185 		firmwareError("invalid dwell: %f at %d", dwellMs, rpm);
??handleSparkEvent_0:
        STR      R6,[SP, #+0]
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R0,??DataTable13_21
          CFI FunCall firmwareError
        BL       firmwareError
//  186 		return;
        B.N      ??handleSparkEvent_2
//  187 	}
//  188 
//  189 	floatus_t chargeDelayUs = engine->rpmCalculator.oneDegreeUs * iEvent->dwellPosition.angleOffset;
??handleSparkEvent_1:
        LDR.W    R0,??DataTable13_8
        LDR      R0,[R0, #+0]
        VLDR     S0,[R0, #+8]
        VLDR     S1,[R5, #+68]
        VMUL.F32 S17,S0,S1
//  190 	int isIgnitionError = chargeDelayUs < 0;
        VCMP.F32 S17,#0.0
        FMSTAT   
        BPL.N    ??handleSparkEvent_3
        MOVS     R7,#+1
        B.N      ??handleSparkEvent_4
??handleSparkEvent_3:
        MOVS     R7,#+0
??handleSparkEvent_4:
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
//  191 	ignitionErrorDetection.add(isIgnitionError);
        MOVS     R1,R7
        LDR.W    R0,??DataTable13_1
          CFI FunCall _ZN13cyclic_bufferIiE3addEi
        BL       _ZN13cyclic_bufferIiE3addEi
//  192 	if (isIgnitionError) {
        CMP      R7,#+0
        BEQ.N    ??handleSparkEvent_5
//  193 #if EFI_PROD_CODE
//  194 		scheduleMsg(logger, "Negative spark delay=%f", chargeDelayUs);
        VMOV     R0,S17
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable13_22
        LDR.W    R0,??DataTable13_23
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  195 #endif
//  196 		chargeDelayUs = 0;
        VLDR.W   S0,??DataTable8  ;; 0x0
        VMOV.F32 S17,S0
//  197 		return;
        B.N      ??handleSparkEvent_2
//  198 	}
//  199 
//  200 	if (cisnan(dwellMs)) {
??handleSparkEvent_5:
        VMOV.F32 S0,S16
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BNE.N    ??handleSparkEvent_6
//  201 		firmwareError("NaN in scheduleOutput", dwellMs);
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R0,??DataTable13_10
          CFI FunCall firmwareError
        BL       firmwareError
//  202 		return;
        B.N      ??handleSparkEvent_2
//  203 	}
//  204 
//  205 	/**
//  206 	 * We are alternating two event lists in order to avoid a potential issue around revolution boundary
//  207 	 * when an event is scheduled within the next revolution.
//  208 	 */
//  209 	scheduling_s * sUp = &iEvent->signalTimerUp;
??handleSparkEvent_6:
        ADDS     R8,R5,#+8
//  210 	scheduling_s * sDown = &iEvent->signalTimerDown;
        ADDS     R9,R5,#+32
//  211 
//  212 	/**
//  213 	 * The start of charge is always within the current trigger event range, so just plain time-based scheduling
//  214 	 */
//  215 	scheduleTask("spark up", sUp, chargeDelayUs, (schfunc_t) &turnPinHigh, iEvent->output);
        LDR      R0,[R5, #+0]
        STR      R0,[SP, #+0]
        LDR.W    R3,??DataTable13_24
        VCVT.S32.F32 S0,S17
        VMOV     R2,S0
        MOV      R1,R8
        LDR.W    R0,??DataTable13_25
          CFI FunCall _Z12scheduleTaskPKcP12scheduling_siPFvPvES3_
        BL       _Z12scheduleTaskPKcP12scheduling_siPFvPvES3_
//  216 	/**
//  217 	 * Spark event is often happening during a later trigger event timeframe
//  218 	 * TODO: improve precision
//  219 	 */
//  220 
//  221 	findTriggerPosition(&iEvent->sparkPosition, iEvent->advance PASS_ENGINE_PARAMETER);
        VLDR     S0,[R5, #+56]
        ADDS     R0,R5,#+72
          CFI FunCall _Z19findTriggerPositionP24event_trigger_position_sf
        BL       _Z19findTriggerPositionP24event_trigger_position_sf
//  222 
//  223 	if (iEvent->sparkPosition.eventIndex == eventIndex) {
        LDR      R0,[R5, #+72]
        CMP      R0,R4
        BNE.N    ??handleSparkEvent_7
//  224 		/**
//  225 		 * Spark should be fired before the next trigger event - time-based delay is best precision possible
//  226 		 */
//  227 		float timeTillIgnitionUs = engine->rpmCalculator.oneDegreeUs * iEvent->sparkPosition.angleOffset;
        LDR.W    R0,??DataTable13_8
        LDR      R0,[R0, #+0]
        VLDR     S0,[R0, #+8]
        VLDR     S1,[R5, #+80]
        VMUL.F32 S18,S0,S1
//  228 
//  229 		scheduleTask("spark 1down", sDown, (int) timeTillIgnitionUs, (schfunc_t) &turnPinLow, iEvent->output);
        LDR      R0,[R5, #+0]
        STR      R0,[SP, #+0]
        LDR.W    R3,??DataTable13_26
        VCVT.S32.F32 S0,S18
        VMOV     R2,S0
        MOV      R1,R9
        LDR.W    R0,??DataTable13_27
          CFI FunCall _Z12scheduleTaskPKcP12scheduling_siPFvPvES3_
        BL       _Z12scheduleTaskPKcP12scheduling_siPFvPvES3_
        B.N      ??handleSparkEvent_8
//  230 	} else {
//  231 		/**
//  232 		 * Spark should be scheduled in relation to some future trigger event, this way we get better firing precision
//  233 		 */
//  234 		bool isPending = assertNotInList<IgnitionEvent>(iHead, iEvent);
??handleSparkEvent_7:
        MOVS     R1,R5
        LDR.W    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
          CFI FunCall _Z15assertNotInListI13IgnitionEventEbPT_S2_
        BL       _Z15assertNotInListI13IgnitionEventEbPT_S2_
//  235 		if (isPending)
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+0
        BNE.N    ??handleSparkEvent_2
//  236 			return;
//  237 
//  238 		LL_APPEND(iHead, iEvent);
??handleSparkEvent_9:
        LDR.W    R1,??DataTable13_28
        LDR      R1,[R1, #+0]
        CMP      R1,#+0
        BEQ.N    ??handleSparkEvent_10
        LDR.W    R1,??DataTable13_28
        LDR      R1,[R1, #+0]
        STR      R1,[R5, #+84]
??handleSparkEvent_11:
        LDR      R1,[R5, #+84]
        LDR      R1,[R1, #+84]
        CMP      R1,#+0
        BEQ.N    ??handleSparkEvent_12
        LDR      R1,[R5, #+84]
        LDR      R1,[R1, #+84]
        STR      R1,[R5, #+84]
        B.N      ??handleSparkEvent_11
??handleSparkEvent_12:
        LDR      R1,[R5, #+84]
        STR      R5,[R1, #+84]
        B.N      ??handleSparkEvent_13
??handleSparkEvent_10:
        LDR.W    R1,??DataTable13_28
        STR      R5,[R1, #+0]
??handleSparkEvent_13:
        MOVS     R1,#+0
        STR      R1,[R5, #+84]
//  239 	}
//  240 }
??handleSparkEvent_8:
??handleSparkEvent_2:
        ADD      SP,SP,#+8
          CFI CFA R13+48
        VPOP     {D8-D9}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI CFA R13+32
        POP      {R0,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock5

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5:
        DC32     0x447a0000
//  241 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z11handleSparkjiP9ArrayListI13IgnitionEventLi80EE
        THUMB
//  242 static ALWAYS_INLINE void handleSpark(uint32_t eventIndex, int rpm,
//  243 		IgnitionEventList *list DECLARE_ENGINE_PARAMETER_S) {
_Z11handleSparkjiP9ArrayListI13IgnitionEventLi80EE:
        PUSH     {R4-R10,LR}
          CFI R14 Frame(CFA, -4)
          CFI R10 Frame(CFA, -8)
          CFI R9 Frame(CFA, -12)
          CFI R8 Frame(CFA, -16)
          CFI R7 Frame(CFA, -20)
          CFI R6 Frame(CFA, -24)
          CFI R5 Frame(CFA, -28)
          CFI R4 Frame(CFA, -32)
          CFI CFA R13+32
        VPUSH    {D8}
          CFI D8 Frame(CFA, -40)
          CFI CFA R13+40
        SUB      SP,SP,#+8
          CFI CFA R13+48
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  244 	if (!isValidRpm(rpm) || !engineConfiguration->isIgnitionEnabled)
        CMP      R5,#+1
        BLT.N    ??handleSpark_0
        MOVW     R0,#+30000
        CMP      R5,R0
        BGE.N    ??handleSpark_0
        LDR.W    R0,??DataTable13_20
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+1
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BNE.N    ??handleSpark_1
//  245 		return; // this might happen for instance in case of a single trigger event after a pause
??handleSpark_0:
        B.N      ??handleSpark_2
//  246 
//  247 	/**
//  248 	 * Ignition schedule is defined once per revolution
//  249 	 * See initializeIgnitionActions()
//  250 	 */
//  251 
//  252 	IgnitionEvent *current, *tmp;
//  253 
//  254 	LL_FOREACH_SAFE(iHead, current, tmp)
??handleSpark_1:
        LDR.W    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
        MOVS     R7,R0
??handleSpark_3:
        CMP      R7,#+0
        BEQ.N    ??handleSpark_4
        LDR      R8,[R7, #+84]
        MOVS     R0,#+0
        CMP      R0,#+0
        BNE.N    ??handleSpark_4
//  255 	{
//  256 		if (current->sparkPosition.eventIndex == eventIndex) {
        LDR      R0,[R7, #+72]
        CMP      R0,R4
        BNE.N    ??handleSpark_5
//  257 			// time to fire a spark which was scheduled previously
//  258 			LL_DELETE(iHead, current);
        LDR.W    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
        CMP      R0,R7
        BNE.N    ??handleSpark_6
        LDR.W    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+84]
        LDR.W    R1,??DataTable13_28
        STR      R0,[R1, #+0]
        B.N      ??handleSpark_7
??handleSpark_6:
        LDR.W    R1,??DataTable13_28
        LDR      R1,[R1, #+0]
        MOVS     R0,R1
??handleSpark_8:
        LDR.W    R1,??DataTable13_28
        LDR      R1,[R1, #+0]
        LDR      R1,[R1, #+84]
        CMP      R1,#+0
        BEQ.N    ??handleSpark_9
        LDR.W    R1,??DataTable13_28
        LDR      R1,[R1, #+0]
        LDR      R1,[R1, #+84]
        CMP      R1,R7
        BEQ.N    ??handleSpark_9
        LDR.W    R1,??DataTable13_28
        LDR      R1,[R1, #+0]
        LDR      R1,[R1, #+84]
        LDR.W    R2,??DataTable13_28
        STR      R1,[R2, #+0]
        B.N      ??handleSpark_8
??handleSpark_9:
        LDR.W    R1,??DataTable13_28
        LDR      R1,[R1, #+0]
        LDR      R1,[R1, #+84]
        CMP      R1,#+0
        BEQ.N    ??handleSpark_10
        LDR.W    R1,??DataTable13_28
        LDR      R1,[R1, #+0]
        LDR      R2,[R7, #+84]
        STR      R2,[R1, #+84]
??handleSpark_10:
        LDR.W    R2,??DataTable13_28
        MOVS     R1,R2
        STR      R0,[R1, #+0]
//  259 
//  260 			scheduling_s * sDown = &current->signalTimerDown;
??handleSpark_7:
        ADDS     R9,R7,#+32
//  261 
//  262 			float timeTillIgnitionUs = ENGINE(rpmCalculator.oneDegreeUs) * current->sparkPosition.angleOffset;
        LDR.W    R0,??DataTable13_5
        VLDR     S0,[R0, #+8]
        VLDR     S1,[R7, #+80]
        VMUL.F32 S16,S0,S1
//  263 			scheduleTask("spark 2down", sDown, (int) timeTillIgnitionUs, (schfunc_t) &turnPinLow, current->output);
        LDR      R0,[R7, #+0]
        STR      R0,[SP, #+0]
        LDR.W    R3,??DataTable13_26
        VCVT.S32.F32 S0,S16
        VMOV     R2,S0
        MOV      R1,R9
        LDR.W    R0,??DataTable13_29
          CFI FunCall _Z12scheduleTaskPKcP12scheduling_siPFvPvES3_
        BL       _Z12scheduleTaskPKcP12scheduling_siPFvPvES3_
//  264 		}
//  265 	}
??handleSpark_5:
        MOV      R7,R8
        B.N      ??handleSpark_3
//  266 
//  267 //	scheduleSimpleMsg(&logger, "eventId spark ", eventIndex);
//  268 	for (int i = 0; i < list->size; i++) {
??handleSpark_4:
        MOVS     R9,#+0
??handleSpark_11:
        LDR      R0,[R6, #+0]
        CMP      R9,R0
        BGE.N    ??handleSpark_12
//  269 		IgnitionEvent *event = &list->elements[i];
        MOVS     R0,#+96
        MLA      R0,R0,R9,R6
        ADDS     R10,R0,#+8
//  270 		if (event->dwellPosition.eventIndex != eventIndex)
        LDR      R0,[R10, #+60]
        CMP      R0,R4
        BNE.N    ??handleSpark_13
//  271 			continue;
//  272 		handleSparkEvent(eventIndex, event, rpm PASS_ENGINE_PARAMETER);
??handleSpark_14:
        MOVS     R2,R5
        MOV      R1,R10
        MOVS     R0,R4
          CFI FunCall _Z16handleSparkEventjP13IgnitionEventi
        BL       _Z16handleSparkEventjP13IgnitionEventi
//  273 	}
??handleSpark_13:
        ADDS     R9,R9,#+1
        B.N      ??handleSpark_11
//  274 }
??handleSpark_12:
??handleSpark_2:
        ADD      SP,SP,#+8
          CFI CFA R13+40
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+32
        POP      {R4-R10,PC}      ;; return
          CFI EndBlock cfiBlock6
//  275 

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//  276 static histogram_s mainLoopHisto;
mainLoopHisto:
        DS8 3616
//  277 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function showMainHistogram
        THUMB
//  278 void showMainHistogram(void) {
showMainHistogram:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  279 #if EFI_PROD_CODE
//  280 	printHistogram(logger, &mainLoopHisto);
        LDR.W    R1,??DataTable13_30
        LDR.W    R0,??DataTable13_23
        LDR      R0,[R0, #+0]
          CFI FunCall _Z14printHistogramP7LoggingP11histogram_s
        BL       _Z14printHistogramP7LoggingP11histogram_s
//  281 #endif
//  282 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock7
//  283 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z12ignitionCalci
        THUMB
//  284 static void ignitionCalc(int rpm DECLARE_ENGINE_PARAMETER_S) {
_Z12ignitionCalci:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+8
          CFI CFA R13+24
        MOVS     R4,R0
//  285 	/**
//  286 	 * Within one engine cycle all cylinders are fired with same timing advance.
//  287 	 * todo: one day we can control cylinders individually?
//  288 	 */
//  289 	float dwellMs = ENGINE(engineState.sparkDwell);
        LDR.W    R0,??DataTable13_31
        VLDR     S16,[R0, #0]
//  290 
//  291 	if (cisnan(dwellMs) || dwellMs < 0) {
        VMOV.F32 S0,S16
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BEQ.N    ??ignitionCalc_0
        VCMP.F32 S16,#0.0
        FMSTAT   
        BPL.N    ??ignitionCalc_1
//  292 		firmwareError("invalid dwell: %f at %d", dwellMs, rpm);
??ignitionCalc_0:
        STR      R4,[SP, #+0]
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R0,??DataTable13_21
          CFI FunCall firmwareError
        BL       firmwareError
//  293 		return;
        B.N      ??ignitionCalc_2
//  294 	}
//  295 	// todo: eliminate this field
//  296 	engine->engineState.advance = -ENGINE(engineState.timingAdvance);
??ignitionCalc_1:
        LDR.W    R0,??DataTable13_8
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+7872
        LDR.W    R1,??DataTable13_32
        VLDR     S0,[R1, #0]
        VNEG.F32 S0,S0
        VSTR     S0,[R0, #0]
//  297 }
??ignitionCalc_2:
        ADD      SP,SP,#+8
          CFI CFA R13+16
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8:
        DC32     0x0
//  298 
//  299 #if EFI_PROD_CODE
//  300 /**
//  301  * this field is used as an Expression in IAR debugger
//  302  */

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  303 uint32_t *cyccnt = (uint32_t*)&DWT_CYCCNT;
cyccnt:
        DATA
        DC32 0E0001004H
//  304 #endif
//  305 
//  306 /**
//  307  * This is the main trigger event handler.
//  308  * Both injection and ignition are controlled from this method.
//  309  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z19mainTriggerCallback15trigger_event_ej
        THUMB
//  310 void mainTriggerCallback(trigger_event_e ckpSignalType, uint32_t eventIndex DECLARE_ENGINE_PARAMETER_S) {
_Z19mainTriggerCallback15trigger_event_ej:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        VPUSH    {D8}
          CFI D8 Frame(CFA, -32)
          CFI CFA R13+32
        MOVS     R4,R0
        MOVS     R5,R1
//  311 	engine->m.beforeMainTrigger = GET_TIMESTAMP();
        LDR.W    R0,??DataTable13_8
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+9152
        LDR.W    R1,??DataTable13_33  ;; 0xe0001004
        LDR      R1,[R1, #+0]
        STR      R1,[R0, #+0]
//  312 	if (hasFirmwareError()) {
        LDR.W    R0,??DataTable13_34
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BNE.W    ??mainTriggerCallback_0
//  313 		/**
//  314 		 * In case on a major error we should not process any more events.
//  315 		 * TODO: add 'pin shutdown' invocation somewhere
//  316 		 */
//  317 		return;
//  318 	}
//  319 
//  320 	(void) ckpSignalType;
//  321 	efiAssertVoid(eventIndex < 2 * engine->triggerShape.getSize(), "trigger/event index");
??mainTriggerCallback_1:
        LDR.W    R0,??DataTable13_8
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+644
          CFI FunCall _ZNK12TriggerShape7getSizeEv
        BL       _ZNK12TriggerShape7getSizeEv
        CMP      R5,R0, LSL #+1
        BCC.N    ??mainTriggerCallback_2
        LDR.W    R0,??DataTable13_35
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??mainTriggerCallback_0
//  322 	efiAssertVoid(getRemainingStack(chThdSelf()) > 128, "lowstck#2");
??mainTriggerCallback_2:
        LDR.W    R0,??DataTable13_16
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+129
        BGE.N    ??mainTriggerCallback_3
        LDR.W    R0,??DataTable13_36
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??mainTriggerCallback_0
//  323 
//  324 	int rpm = getRpmE(engine);
??mainTriggerCallback_3:
        LDR.W    R0,??DataTable13_8
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN13RpmCalculator3rpmEv
        BL       _ZN13RpmCalculator3rpmEv
        MOVS     R6,R0
//  325 	if (rpm == 0) {
        CMP      R6,#+0
        BEQ.W    ??mainTriggerCallback_0
//  326 		// this happens while we just start cranking
//  327 		// todo: check for 'trigger->is_synchnonized?'
//  328 		return;
//  329 	}
//  330 	if (rpm == NOISY_RPM) {
??mainTriggerCallback_4:
        CMN      R6,#+1
        BNE.N    ??mainTriggerCallback_5
//  331 		warning(OBD_Camshaft_Position_Sensor_Circuit_Range_Performance, "noisy trigger");
        LDR.W    R1,??DataTable13_37
        MOVW     R0,#+341
          CFI FunCall warning
        BL       warning
//  332 		return;
        B.N      ??mainTriggerCallback_0
//  333 	}
//  334 	if (rpm > engineConfiguration->rpmHardLimit) {
??mainTriggerCallback_5:
        LDR.W    R0,??DataTable13_20
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+420]
        CMP      R0,R6
        BGE.N    ??mainTriggerCallback_6
//  335 		warning(OBD_PCM_Processor_Fault, "skipping stroke due to rpm=%d", rpm);
        MOVS     R2,R6
        LDR.W    R1,??DataTable13_38
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  336 		return;
        B.N      ??mainTriggerCallback_0
//  337 	}
//  338 
//  339 #if EFI_HISTOGRAMS && EFI_PROD_CODE
//  340 	int beforeCallback = hal_lld_get_counter_value();
//  341 #endif
//  342 
//  343 	int revolutionIndex = engine->rpmCalculator.getRevolutionCounter() % 2;
??mainTriggerCallback_6:
        LDR.W    R0,??DataTable13_8
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN13RpmCalculator20getRevolutionCounterEv
        BL       _ZN13RpmCalculator20getRevolutionCounterEv
        ANDS     R7,R0,#0x1
//  344 
//  345 	if (eventIndex == 0) {
        CMP      R5,#+0
        BNE.N    ??mainTriggerCallback_7
//  346 		if (triggerVersion.isOld())
        LDR.W    R0,??DataTable13
          CFI FunCall _ZN18LocalVersionHolder5isOldEv
        BL       _ZN18LocalVersionHolder5isOldEv
        CMP      R0,#+0
        BEQ.N    ??mainTriggerCallback_7
//  347 			prepareOutputSignals(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z20prepareOutputSignalsv
        BL       _Z20prepareOutputSignalsv
//  348 	}
//  349 
//  350 	if (engineConfiguration->useOnlyFrontForTrigger && engineConfiguration->ignMathCalculateAtIndex % 2 != 0) {
??mainTriggerCallback_7:
        LDR.W    R0,??DataTable13_20
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+6
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??mainTriggerCallback_8
        LDR.W    R0,??DataTable13_20
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+1524]
        MOVS     R1,#+2
        SDIV     R2,R0,R1
        MLS      R2,R2,R1,R0
        CMP      R2,#+0
        BEQ.N    ??mainTriggerCallback_8
//  351 		firmwareError("invalid ignMathCalculateAtIndex %d", engineConfiguration->ignMathCalculateAtIndex);
        LDR.W    R0,??DataTable13_20
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+1524]
        LDR.W    R0,??DataTable13_39
          CFI FunCall firmwareError
        BL       firmwareError
//  352 	}
//  353 
//  354 	if (eventIndex == engineConfiguration->ignMathCalculateAtIndex) {
??mainTriggerCallback_8:
        LDR.W    R0,??DataTable13_20
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+1524]
        CMP      R5,R0
        BNE.N    ??mainTriggerCallback_9
//  355 		if (engineConfiguration->externalKnockSenseAdc != EFI_ADC_NONE) {
        LDR.W    R0,??DataTable13_20
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+1620]
        CMP      R0,#+16
        BEQ.N    ??mainTriggerCallback_10
//  356 			float externalKnockValue = getVoltageDivided("knock", engineConfiguration->externalKnockSenseAdc);
        LDR.W    R0,??DataTable13_20
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+1620]
        LDR.W    R0,??DataTable13_40
          CFI FunCall _Z19getInternalAdcValuePKc13adc_channel_e
        BL       _Z19getInternalAdcValuePKc13adc_channel_e
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        VLDR.W   S1,??DataTable11  ;; 0x3a400c01
        VMUL.F32 S0,S0,S1
        LDR.W    R0,??DataTable13_20
        LDR      R0,[R0, #+0]
        VLDR     S1,[R0, #+464]
        VMUL.F32 S16,S0,S1
//  357 			engine->knockLogic(externalKnockValue);
        VMOV.F32 S0,S16
        LDR.W    R0,??DataTable13_8
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN6Engine10knockLogicEf
        BL       _ZN6Engine10knockLogicEf
//  358 		}
//  359 
//  360 		engine->m.beforeIgnitionMath = GET_TIMESTAMP();
??mainTriggerCallback_10:
        MOVW     R0,#+9160
        LDR.W    R1,??DataTable13_8
        LDR      R1,[R1, #+0]
        LDR.W    R2,??DataTable13_33  ;; 0xe0001004
        LDR      R2,[R2, #+0]
        STR      R2,[R0, R1]
//  361 		ignitionCalc(rpm PASS_ENGINE_PARAMETER);
        MOVS     R0,R6
          CFI FunCall _Z12ignitionCalci
        BL       _Z12ignitionCalci
//  362 		engine->m.ignitionMathTime = GET_TIMESTAMP() - engine->m.beforeIgnitionMath;
        LDR.W    R0,??DataTable13_33  ;; 0xe0001004
        LDR      R0,[R0, #+0]
        MOVW     R1,#+9160
        LDR.N    R2,??DataTable13_8
        LDR      R2,[R2, #+0]
        LDR      R1,[R1, R2]
        SUBS     R0,R0,R1
        MOVW     R1,#+9164
        LDR.N    R2,??DataTable13_8
        LDR      R2,[R2, #+0]
        STR      R0,[R1, R2]
//  363 	}
//  364 
//  365 	if (eventIndex == 0) {
??mainTriggerCallback_9:
        CMP      R5,#+0
        BNE.W    ??mainTriggerCallback_11
//  366 		engine->m.beforeFuelCalc = GET_TIMESTAMP();
        MOVW     R0,#+9200
        LDR.N    R1,??DataTable13_8
        LDR      R1,[R1, #+0]
        LDR.W    R2,??DataTable13_33  ;; 0xe0001004
        LDR      R2,[R2, #+0]
        STR      R2,[R0, R1]
//  367 		ENGINE(fuelMs) = getFuelMs(rpm PASS_ENGINE_PARAMETER) * engineConfiguration->globalFuelCorrection;
        MOVS     R0,R6
          CFI FunCall _Z9getFuelMsi
        BL       _Z9getFuelMsi
        LDR.N    R0,??DataTable13_20
        LDR      R0,[R0, #+0]
        VLDR     S1,[R0, #+568]
        VMUL.F32 S0,S0,S1
        LDR.N    R0,??DataTable13_5
        VSTR     S0,[R0, #+608]
//  368 		engine->m.fuelCalcTime = GET_TIMESTAMP() - engine->m.beforeFuelCalc;
        LDR.W    R0,??DataTable13_33  ;; 0xe0001004
        LDR      R0,[R0, #+0]
        MOVW     R1,#+9200
        LDR.N    R2,??DataTable13_8
        LDR      R2,[R2, #+0]
        LDR      R1,[R1, R2]
        SUBS     R0,R0,R1
        MOVW     R1,#+9204
        LDR.N    R2,??DataTable13_8
        LDR      R2,[R2, #+0]
        STR      R0,[R1, R2]
//  369 
//  370 		engine->m.beforeIgnitionSch = GET_TIMESTAMP();
        MOVW     R0,#+9168
        LDR.N    R1,??DataTable13_8
        LDR      R1,[R1, #+0]
        LDR.N    R2,??DataTable13_33  ;; 0xe0001004
        LDR      R2,[R2, #+0]
        STR      R2,[R0, R1]
//  371 		/**
//  372 		 * TODO: warning. there is a bit of a hack here, todo: improve.
//  373 		 * currently output signals/times signalTimerUp from the previous revolutions could be
//  374 		 * still used because they have crossed the revolution boundary
//  375 		 * but we are already repurposing the output signals, but everything works because we
//  376 		 * are not affecting that space in memory. todo: use two instances of 'ignitionSignals'
//  377 		 */
//  378 		float maxAllowedDwellAngle = (int) (engineConfiguration->engineCycle / 2); // the cast is about making Coverity happy
        LDR.N    R0,??DataTable13_20
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+416]
        MOVS     R1,#+2
        SDIV     R0,R0,R1
        VMOV     S0,R0
        VCVT.F32.S32 S16,S0
//  379 
//  380 		if (engineConfiguration->ignitionMode == IM_ONE_COIL) {
        LDR.N    R0,??DataTable13_20
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+444]
        CMP      R0,#+0
        BNE.N    ??mainTriggerCallback_12
//  381 			maxAllowedDwellAngle = engineConfiguration->engineCycle / engineConfiguration->specs.cylindersCount / 1.1;
        LDR.N    R0,??DataTable13_20
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+416]
        LDR.N    R1,??DataTable13_20
        LDR      R1,[R1, #+0]
        LDR      R1,[R1, #+404]
        SDIV     R0,R0,R1
          CFI FunCall __aeabi_i2d
        BL       __aeabi_i2d
        LDR.N    R2,??DataTable13_41  ;; 0x9999999a
        LDR.N    R3,??DataTable13_42  ;; 0x3ff19999
          CFI FunCall __aeabi_ddiv
        BL       __aeabi_ddiv
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        VMOV     S16,R0
//  382 		}
//  383 
//  384 		if (engine->engineState.dwellAngle == 0) {
??mainTriggerCallback_12:
        LDR.N    R0,??DataTable13_8
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+7680
        ADDS     R0,R0,#+188
        VLDR     S0,[R0, #0]
        VCMP.F32 S0,#0.0
        FMSTAT   
        BNE.N    ??mainTriggerCallback_13
//  385 			warning(OBD_PCM_Processor_Fault, "dwell is zero?");
        LDR.N    R1,??DataTable13_43
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  386 		}
//  387 		if (engine->engineState.dwellAngle > maxAllowedDwellAngle) {
??mainTriggerCallback_13:
        LDR.N    R0,??DataTable13_8
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+7680
        ADDS     R0,R0,#+188
        VLDR     S0,[R0, #0]
        VCMP.F32 S16,S0
        FMSTAT   
        BPL.N    ??mainTriggerCallback_14
//  388 			warning(OBD_PCM_Processor_Fault, "dwell angle too long: %f", engine->engineState.dwellAngle);
        MOVW     R0,#+7868
        LDR.N    R1,??DataTable13_8
        LDR      R1,[R1, #+0]
        LDR      R0,[R0, R1]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable13_44
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  389 		}
//  390 
//  391 		// todo: add some check for dwell overflow? like 4 times 6 ms while engine cycle is less then that
//  392 
//  393 		if (cisnan(engine->engineState.advance)) {
??mainTriggerCallback_14:
        LDR.N    R0,??DataTable13_8
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+7872
        VLDR     S0,[R0, #0]
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BEQ.N    ??mainTriggerCallback_0
//  394 			// error should already be reported
//  395 			return;
//  396 		}
//  397 		initializeIgnitionActions(engine->engineState.advance, engine->engineState.dwellAngle,
//  398 				&engine->engineConfiguration2->ignitionEvents[revolutionIndex] PASS_ENGINE_PARAMETER);
??mainTriggerCallback_15:
        MOVW     R0,#+7688
        LDR.N    R1,??DataTable13_8
        LDR      R1,[R1, #+0]
        LDR      R1,[R1, #+40]
        MLA      R0,R0,R7,R1
        ADD      R0,R0,#+12032
        ADDS     R0,R0,#+80
        LDR.N    R1,??DataTable13_8
        LDR      R1,[R1, #+0]
        ADD      R1,R1,#+7680
        ADDS     R1,R1,#+188
        VLDR     S1,[R1, #0]
        LDR.N    R1,??DataTable13_8
        LDR      R1,[R1, #+0]
        ADDS     R1,R1,#+7872
        VLDR     S0,[R1, #0]
          CFI FunCall _Z25initializeIgnitionActionsffP9ArrayListI13IgnitionEventLi80EE
        BL       _Z25initializeIgnitionActionsffP9ArrayListI13IgnitionEventLi80EE
//  399 		engine->m.ignitionSchTime = GET_TIMESTAMP() - engine->m.beforeIgnitionSch;
        LDR.N    R0,??DataTable13_33  ;; 0xe0001004
        LDR      R0,[R0, #+0]
        MOVW     R1,#+9168
        LDR.N    R2,??DataTable13_8
        LDR      R2,[R2, #+0]
        LDR      R1,[R1, R2]
        SUBS     R0,R0,R1
        MOVW     R1,#+9172
        LDR.N    R2,??DataTable13_8
        LDR      R2,[R2, #+0]
        STR      R0,[R1, R2]
//  400 
//  401 		engine->m.beforeInjectonSch = GET_TIMESTAMP();
        MOVW     R0,#+9176
        LDR.N    R1,??DataTable13_8
        LDR      R1,[R1, #+0]
        LDR.N    R2,??DataTable13_33  ;; 0xe0001004
        LDR      R2,[R2, #+0]
        STR      R2,[R0, R1]
//  402 
//  403 		if (isCrankingR(rpm)) {
        CMP      R6,#+1
        BLT.N    ??mainTriggerCallback_16
        LDR.N    R0,??DataTable13_19
        LDRSH    R0,[R0, #+104]
        CMP      R6,R0
        BGE.N    ??mainTriggerCallback_16
//  404 			ENGINE(engineConfiguration2)->crankingInjectionEvents.addFuelEvents(
//  405 					engineConfiguration->crankingInjectionMode PASS_ENGINE_PARAMETER);
        LDR.N    R0,??DataTable13_20
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+428]
        LDR.N    R0,??DataTable13_5
        LDR      R0,[R0, #+40]
          CFI FunCall _ZN12FuelSchedule13addFuelEventsE16injection_mode_e
        BL       _ZN12FuelSchedule13addFuelEventsE16injection_mode_e
        B.N      ??mainTriggerCallback_17
//  406 		} else {
//  407 			ENGINE(engineConfiguration2)->injectionEvents.addFuelEvents(
//  408 					engineConfiguration->injectionMode PASS_ENGINE_PARAMETER);
??mainTriggerCallback_16:
        LDR.N    R0,??DataTable13_20
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+432]
        LDR.N    R0,??DataTable13_5
        LDR      R0,[R0, #+40]
        ADD      R0,R0,#+5888
        ADDS     R0,R0,#+136
          CFI FunCall _ZN12FuelSchedule13addFuelEventsE16injection_mode_e
        BL       _ZN12FuelSchedule13addFuelEventsE16injection_mode_e
//  409 		}
//  410 		engine->m.injectonSchTime = GET_TIMESTAMP() - engine->m.beforeInjectonSch;
??mainTriggerCallback_17:
        LDR.N    R0,??DataTable13_33  ;; 0xe0001004
        LDR      R0,[R0, #+0]
        MOVW     R1,#+9176
        LDR.N    R2,??DataTable13_8
        LDR      R2,[R2, #+0]
        LDR      R1,[R1, R2]
        SUBS     R0,R0,R1
        MOVW     R1,#+9180
        LDR.N    R2,??DataTable13_8
        LDR      R2,[R2, #+0]
        STR      R0,[R1, R2]
//  411 	}
//  412 
//  413 //	triggerEventsQueue.executeAll(getCrankEventCounter());
//  414 
//  415 	handleFuel(eventIndex, rpm PASS_ENGINE_PARAMETER);
??mainTriggerCallback_11:
        MOVS     R1,R6
        MOVS     R0,R5
          CFI FunCall _Z10handleFuelji
        BL       _Z10handleFuelji
//  416 	handleSpark(eventIndex, rpm, &engine->engineConfiguration2->ignitionEvents[revolutionIndex] PASS_ENGINE_PARAMETER);
        MOVW     R0,#+7688
        LDR.N    R1,??DataTable13_8
        LDR      R1,[R1, #+0]
        LDR      R1,[R1, #+40]
        MLA      R0,R0,R7,R1
        ADD      R2,R0,#+12032
        ADDS     R2,R2,#+80
        MOVS     R1,R6
        MOVS     R0,R5
          CFI FunCall _Z11handleSparkjiP9ArrayListI13IgnitionEventLi80EE
        BL       _Z11handleSparkjiP9ArrayListI13IgnitionEventLi80EE
//  417 #if EFI_HISTOGRAMS && EFI_PROD_CODE
//  418 	int diff = hal_lld_get_counter_value() - beforeCallback;
//  419 	if (diff > 0)
//  420 	hsAdd(&mainLoopHisto, diff);
//  421 #endif /* EFI_HISTOGRAMS */
//  422 
//  423 	if (eventIndex == 0) {
        CMP      R5,#+0
        BNE.N    ??mainTriggerCallback_18
//  424 		engine->m.mainTriggerCallbackTime = GET_TIMESTAMP() - engine->m.beforeMainTrigger;
        LDR.N    R0,??DataTable13_33  ;; 0xe0001004
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable13_8
        LDR      R1,[R1, #+0]
        ADDS     R1,R1,#+9152
        LDR      R1,[R1, #+0]
        SUBS     R0,R0,R1
        MOVW     R1,#+9156
        LDR.N    R2,??DataTable13_8
        LDR      R2,[R2, #+0]
        STR      R0,[R1, R2]
//  425 	}
//  426 }
??mainTriggerCallback_18:
??mainTriggerCallback_0:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+24
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock9
//  427 
//  428 #if EFI_WAVE_CHART || defined(__DOXYGEN__)
//  429 #include "wave_chart.h"
//  430 #endif
//  431 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z20showTriggerHistogramv
        THUMB
//  432 static void showTriggerHistogram(void) {
_Z20showTriggerHistogramv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  433 	printAllCallbacksHistogram();
          CFI FunCall _Z26printAllCallbacksHistogramv
        BL       _Z26printAllCallbacksHistogramv
//  434 	showMainHistogram();
          CFI FunCall showMainHistogram
        BL       showMainHistogram
//  435 #if EFI_WAVE_CHART || defined(__DOXYGEN__)
//  436 	showWaveChartHistogram();
          CFI FunCall _Z22showWaveChartHistogramv
        BL       _Z22showWaveChartHistogramv
//  437 #endif
//  438 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock10
//  439 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _ZN19MainTriggerCallback4initEP6Engine
        THUMB
//  440 void MainTriggerCallback::init(Engine *engine) {
_ZN19MainTriggerCallback4initEP6Engine:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  441 	efiAssertVoid(engine!=NULL, "engine NULL");
        CMP      R5,#+0
        BNE.N    ??init_0
        LDR.N    R0,??DataTable13_45
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??init_1
//  442 	this->engine = engine;
??init_0:
        STR      R5,[R4, #+0]
//  443 }
??init_1:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock11
//  444 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _Z12showMainInfoP6Engine
        THUMB
//  445 static void showMainInfo(Engine *engine) {
_Z12showMainInfoP6Engine:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        VPUSH    {D8}
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        SUB      SP,SP,#+8
          CFI CFA R13+32
        MOVS     R4,R0
//  446 #if EFI_PROD_CODE
//  447 	int rpm = engine->rpmCalculator.rpm(PASS_ENGINE_PARAMETER_F);
        MOVS     R0,R4
          CFI FunCall _ZN13RpmCalculator3rpmEv
        BL       _ZN13RpmCalculator3rpmEv
        MOVS     R5,R0
//  448 	float el = getEngineLoadT(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z14getEngineLoadTv
        BL       _Z14getEngineLoadTv
        VMOV.F32 S16,S0
//  449 	scheduleMsg(logger, "rpm %d engine_load %f", rpm, el);
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        MOVS     R2,R5
        LDR.N    R1,??DataTable13_46
        LDR.N    R0,??DataTable13_23
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  450 	scheduleMsg(logger, "fuel %fms timing %f", getFuelMs(rpm PASS_ENGINE_PARAMETER),
//  451 			engine->engineState.timingAdvance);
        MOVS     R0,R5
          CFI FunCall _Z9getFuelMsi
        BL       _Z9getFuelMsi
        VMOV.F32 S17,S0
        MOVW     R0,#+7864
        LDR      R0,[R0, R4]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        VMOV     R0,S17
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable13_47
        LDR.N    R0,??DataTable13_23
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  452 #endif
//  453 }
        ADD      SP,SP,#+8
          CFI CFA R13+24
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock12

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11:
        DC32     0x3a400c01
//  454 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z21initMainEventListenerP7LoggingP6Engine
        THUMB
//  455 void initMainEventListener(Logging *sharedLogger, Engine *engine) {
_Z21initMainEventListenerP7LoggingP6Engine:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  456 	logger = sharedLogger;
        LDR.N    R0,??DataTable13_23
        STR      R4,[R0, #+0]
//  457 	efiAssertVoid(engine!=NULL, "null engine");
        CMP      R5,#+0
        BNE.N    ??initMainEventListener_0
        LDR.N    R0,??DataTable13_48
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??initMainEventListener_1
//  458 
//  459 	mainTriggerCallbackInstance.init(engine);
??initMainEventListener_0:
        MOVS     R1,R5
        LDR.N    R0,??DataTable13_49
          CFI FunCall _ZN19MainTriggerCallback4initEP6Engine
        BL       _ZN19MainTriggerCallback4initEP6Engine
//  460 
//  461 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//  462 	addConsoleAction("performanceinfo", showTriggerHistogram);
        LDR.N    R1,??DataTable13_50
        LDR.N    R0,??DataTable13_51
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  463 	addConsoleActionP("maininfo", (VoidPtr) showMainInfo, engine);
        MOVS     R2,R5
        LDR.N    R1,??DataTable13_52
        LDR.N    R0,??DataTable13_53
          CFI FunCall addConsoleActionP
        BL       addConsoleActionP
//  464 
//  465 	printMsg(logger, "initMainLoop: %d", currentTimeMillis());
          CFI FunCall currentTimeMillis
        BL       currentTimeMillis
        MOVS     R2,R0
        LDR.N    R1,??DataTable13_54
        LDR.N    R0,??DataTable13_23
        LDR      R0,[R0, #+0]
          CFI FunCall printMsg
        BL       printMsg
//  466 	if (!isInjectionEnabled(mainTriggerCallbackInstance.engine->engineConfiguration))
        LDR.N    R0,??DataTable13_49
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+36]
          CFI FunCall _Z18isInjectionEnabledP22engine_configuration_s
        BL       _Z18isInjectionEnabledP22engine_configuration_s
        CMP      R0,#+0
        BNE.N    ??initMainEventListener_2
//  467 		printMsg(logger, "!!!!!!!!!!!!!!!!!!! injection disabled");
        LDR.N    R1,??DataTable13_55
        LDR.N    R0,??DataTable13_23
        LDR      R0,[R0, #+0]
          CFI FunCall printMsg
        BL       printMsg
//  468 #endif
//  469 
//  470 #if EFI_HISTOGRAMS
//  471 	initHistogram(&mainLoopHisto, "main callback");
//  472 #endif /* EFI_HISTOGRAMS */
//  473 
//  474 	addTriggerEventListener(mainTriggerCallback, "main loop", engine);
??initMainEventListener_2:
        MOVS     R2,R5
        LDR.N    R1,??DataTable13_56
        LDR.N    R0,??DataTable13_57
          CFI FunCall _Z23addTriggerEventListenerPFv15trigger_event_ejEPKcP6Engine
        BL       _Z23addTriggerEventListenerPFv15trigger_event_ejEPKcP6Engine
//  475 }
??initMainEventListener_1:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock13

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _Z21isIgnitionTimingErrorv
        THUMB
_Z21isIgnitionTimingErrorv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        MOVS     R1,#+6
        LDR.N    R0,??DataTable13_1
          CFI FunCall _ZN13cyclic_bufferIiE3sumEi
        BL       _ZN13cyclic_bufferIiE3sumEi
        CMP      R0,#+5
        BLT.N    ??isIgnitionTimingError_0
        MOVS     R0,#+1
        B.N      ??isIgnitionTimingError_1
??isIgnitionTimingError_0:
        MOVS     R0,#+0
??isIgnitionTimingError_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R1,PC}          ;; return
          CFI EndBlock cfiBlock14

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13:
        DC32     triggerVersion

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_1:
        DC32     ignitionErrorDetection

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_2:
        DC32     __dso_handle

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_3:
        DC32     _ZN13cyclic_bufferIiED1Ev

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_4:
        DC32     enginePins

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_5:
        DC32     _engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_6:
        DC32     `?<Constant "NaN injection pulse">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_7:
        DC32     `?<Constant "Negative injection pu...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_8:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_9:
        DC32     `?<Constant "duration cannot be ne...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_10:
        DC32     `?<Constant "NaN in scheduleOutput">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_11:
        DC32     `?<Constant "signal is NULL">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_12:
        DC32     _Z26startSimultaniousInjectionP6Engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_13:
        DC32     `?<Constant "out up">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_14:
        DC32     _Z24endSimultaniousInjectionP6Engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_15:
        DC32     `?<Constant "out down">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_16:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_17:
        DC32     `?<Constant "lowstck#3">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_18:
        DC32     `?<Constant "handleFuel/event index">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_19:
        DC32     persistentState

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_20:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_21:
        DC32     `?<Constant "invalid dwell: %f at %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_22:
        DC32     `?<Constant "Negative spark delay=%f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_23:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_24:
        DC32     _Z11turnPinHighP14NamedOutputPin

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_25:
        DC32     `?<Constant "spark up">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_26:
        DC32     _Z10turnPinLowP14NamedOutputPin

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_27:
        DC32     `?<Constant "spark 1down">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_28:
        DC32     iHead

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_29:
        DC32     `?<Constant "spark 2down">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_30:
        DC32     mainLoopHisto

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_31:
        DC32     _engine+0x1EB4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_32:
        DC32     _engine+0x1EB8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_33:
        DC32     0xe0001004

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_34:
        DC32     hasFirmwareErrorFlag

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_35:
        DC32     `?<Constant "trigger/event index">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_36:
        DC32     `?<Constant "lowstck#2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_37:
        DC32     `?<Constant "noisy trigger">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_38:
        DC32     `?<Constant "skipping stroke due t...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_39:
        DC32     `?<Constant "invalid ignMathCalcul...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_40:
        DC32     `?<Constant "knock">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_41:
        DC32     0x9999999a

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_42:
        DC32     0x3ff19999

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_43:
        DC32     `?<Constant "dwell is zero?">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_44:
        DC32     `?<Constant "dwell angle too long: %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_45:
        DC32     `?<Constant "engine NULL">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_46:
        DC32     `?<Constant "rpm %d engine_load %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_47:
        DC32     `?<Constant "fuel %fms timing %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_48:
        DC32     `?<Constant "null engine">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_49:
        DC32     mainTriggerCallbackInstance

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_50:
        DC32     _Z20showTriggerHistogramv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_51:
        DC32     `?<Constant "performanceinfo">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_52:
        DC32     _Z12showMainInfoP6Engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_53:
        DC32     `?<Constant "maininfo">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_54:
        DC32     `?<Constant "initMainLoop: %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_55:
        DC32     `?<Constant "!!!!!!!!!!!!!!!!!!! i...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_56:
        DC32     `?<Constant "main loop">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_57:
        DC32     _Z19mainTriggerCallback15trigger_event_ej

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memset
          CFI Block cfiBlock15 Using cfiCommon0
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
          CFI EndBlock cfiBlock15

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIiEC1Ev
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIiEC1Ev
        THUMB
// __code __interwork __softfp cyclic_buffer<int>::cyclic_buffer()
_ZN13cyclic_bufferIiEC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R1,#+64
        MOVS     R0,R4
          CFI FunCall _ZN13cyclic_bufferIiE5baseCEi
        BL       _ZN13cyclic_bufferIiE5baseCEi
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock16

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIiE5baseCEi
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIiE5baseCEi
        THUMB
// __interwork __softfp void cyclic_buffer<int>::baseC(int)
_ZN13cyclic_bufferIiE5baseCEi:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R0,#+0
        STR      R0,[R4, #+256]
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _ZN13cyclic_bufferIiE7setSizeEi
        BL       _ZN13cyclic_bufferIiE7setSizeEi
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock17

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIiED1Ev
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIiED1Ev
          CFI NoCalls
        THUMB
// __code __interwork __softfp cyclic_buffer<int>::~cyclic_buffer()
_ZN13cyclic_bufferIiED1Ev:
        BX       LR               ;; return
          CFI EndBlock cfiBlock18

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIiE3addEi
          CFI Block cfiBlock19 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIiE3addEi
          CFI NoCalls
        THUMB
// __interwork __softfp void cyclic_buffer<int>::add(int)
_ZN13cyclic_bufferIiE3addEi:
        LDR      R2,[R0, #+256]
        ADDS     R2,R2,#+1
        STR      R2,[R0, #+256]
        LDR      R2,[R0, #+256]
        LDR      R3,[R0, #+264]
        CMP      R2,R3
        BNE.N    ??add_0
        MOVS     R2,#+0
        STR      R2,[R0, #+256]
??add_0:
        LDR      R2,[R0, #+256]
        STR      R1,[R0, R2, LSL #+2]
        LDR      R2,[R0, #+260]
        ADDS     R2,R2,#+1
        STR      R2,[R0, #+260]
        BX       LR               ;; return
          CFI EndBlock cfiBlock19

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIiE7setSizeEi
          CFI Block cfiBlock20 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIiE7setSizeEi
        THUMB
// __interwork __softfp void cyclic_buffer<int>::setSize(int)
_ZN13cyclic_bufferIiE7setSizeEi:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R0,R4
          CFI FunCall _ZN13cyclic_bufferIiE5clearEv
        BL       _ZN13cyclic_bufferIiE5clearEv
        CMP      R5,#+64
        BGE.N    ??setSize_0
        MOVS     R0,R5
        B.N      ??setSize_1
??setSize_0:
        MOVS     R0,#+64
??setSize_1:
        STR      R0,[R4, #+264]
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock20

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIiE3sumEi
          CFI Block cfiBlock21 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIiE3sumEi
          CFI NoCalls
        THUMB
// __interwork __softfp int cyclic_buffer<int>::sum(int)
_ZN13cyclic_bufferIiE3sumEi:
        PUSH     {R4-R6}
          CFI R6 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+12
        MOVS     R2,R0
        LDR      R0,[R2, #+260]
        CMP      R0,R1
        BGE.N    ??sum_0
        LDR      R0,[R2, #+260]
        MOVS     R1,R0
??sum_0:
        LDR      R3,[R2, #+256]
        MOVS     R0,#+0
        MOVS     R4,#+0
??sum_1:
        CMP      R4,R1
        BGE.N    ??sum_2
        SUBS     R5,R3,R4
??sum_3:
        CMP      R5,#+0
        BPL.N    ??sum_4
        LDR      R6,[R2, #+264]
        ADDS     R5,R6,R5
        B.N      ??sum_3
??sum_4:
        LDR      R5,[R2, R5, LSL #+2]
        ADDS     R0,R5,R0
        ADDS     R4,R4,#+1
        B.N      ??sum_1
??sum_2:
        POP      {R4-R6}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R6 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock21

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIiE5clearEv
          CFI Block cfiBlock22 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIiE5clearEv
        THUMB
// __interwork __softfp void cyclic_buffer<int>::clear()
_ZN13cyclic_bufferIiE5clearEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOV      R2,#+256
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall memset
        BL       memset
        MOVS     R0,#+0
        STR      R0,[R4, #+260]
        MOVS     R0,#+0
        STR      R0,[R4, #+260]
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock22

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _Z15assertNotInListI13IgnitionEventEbPT_S2_
          CFI Block cfiBlock23 Using cfiCommon0
          CFI Function _Z15assertNotInListI13IgnitionEventEbPT_S2_
        THUMB
// __interwork __softfp bool assertNotInList<IgnitionEvent>(IgnitionEvent *, IgnitionEvent *)
_Z15assertNotInListI13IgnitionEventEbPT_S2_:
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
        LDR      R6,[R6, #+84]
        B.N      ??assertNotInList_0
??assertNotInList_1:
        MOVS     R0,#+0
??assertNotInList_4:
        POP      {R1,R4-R7,PC}    ;; return
        Nop      
        DATA
??assertNotInList_3:
        DC32     _ZZ15assertNotInListI13IgnitionEventEbPT_S2_Es
        DC32     _ZZ15assertNotInListI13IgnitionEventEbPT_S2_Es_0
          CFI EndBlock cfiBlock23

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
        SECTION_GROUP _ZZ15assertNotInListI13IgnitionEventEbPT_S2_Es
// __absolute char const <_ZZ15assertNotInListI13IgnitionEventEbPT_S2_Es>[14]
_ZZ15assertNotInListI13IgnitionEventEbPT_S2_Es:
        DATA
        DC8 "Looped queue?"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        SECTION_GROUP _ZZ15assertNotInListI13IgnitionEventEbPT_S2_Es_0
// __absolute char const <_ZZ15assertNotInListI13IgnitionEventEbPT_S2_Es_0>[35]
_ZZ15assertNotInListI13IgnitionEventEbPT_S2_Es_0:
        DATA
        DC8 "re-adding element into event_queue"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "NaN injection pulse">`:
        DATA
        DC8 "NaN injection pulse"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Negative injection pu...">`:
        DATA
        DC8 "Negative injection pulse %f"

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

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lowstck#3">`:
        DATA
        DC8 "lowstck#3"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "handleFuel/event index">`:
        DATA
        DC8 "handleFuel/event index"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid dwell: %f at %d">`:
        DATA
        DC8 "invalid dwell: %f at %d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Negative spark delay=%f">`:
        DATA
        DC8 "Negative spark delay=%f"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spark up">`:
        DATA
        DC8 "spark up"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spark 1down">`:
        DATA
        DC8 "spark 1down"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spark 2down">`:
        DATA
        DC8 "spark 2down"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "trigger/event index">`:
        DATA
        DC8 "trigger/event index"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lowstck#2">`:
        DATA
        DC8 "lowstck#2"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "noisy trigger">`:
        DATA
        DC8 "noisy trigger"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "skipping stroke due t...">`:
        DATA
        DC8 "skipping stroke due to rpm=%d"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid ignMathCalcul...">`:
        DATA
        DC8 "invalid ignMathCalculateAtIndex %d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "knock">`:
        DATA
        DC8 "knock"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "dwell is zero?">`:
        DATA
        DC8 "dwell is zero?"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "dwell angle too long: %f">`:
        DATA
        DC8 "dwell angle too long: %f"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "rpm %d engine_load %f">`:
        DATA
        DC8 "rpm %d engine_load %f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fuel %fms timing %f">`:
        DATA
        DC8 "fuel %fms timing %f"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "null engine">`:
        DATA
        DC8 "null engine"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "performanceinfo">`:
        DATA
        DC8 "performanceinfo"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "maininfo">`:
        DATA
        DC8 "maininfo"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "initMainLoop: %d">`:
        DATA
        DC8 "initMainLoop: %d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "!!!!!!!!!!!!!!!!!!! i...">`:
        DATA
        DC8 "!!!!!!!!!!!!!!!!!!! injection disabled"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "main loop">`:
        DATA
        DC8 "main loop"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "engine NULL">`:
        DATA
        DC8 "engine NULL"

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIiEC2Ev
          CFI Block cfiBlock24 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIiEC2Ev
        THUMB
// __code __interwork __softfp cyclic_buffer<int>::subobject cyclic_buffer()
_ZN13cyclic_bufferIiEC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN13cyclic_bufferIiEC1Ev
        BL       _ZN13cyclic_bufferIiEC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock24

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIiED2Ev
          CFI Block cfiBlock25 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIiED2Ev
        THUMB
// __code __interwork __softfp cyclic_buffer<int>::subobject ~cyclic_buffer()
_ZN13cyclic_bufferIiED2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN13cyclic_bufferIiED1Ev
        BL       _ZN13cyclic_bufferIiED1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock25

        END
//  476 
//  477 int isIgnitionTimingError(void) {
//  478 	return ignitionErrorDetection.sum(6) > 4;
//  479 }
//  480 
//  481 #endif /* EFI_ENGINE_CONTROL */
// 
// 3 900 bytes in section .bss
//     4 bytes in section .data
//     4 bytes in section .init_array
//   648 bytes in section .rodata
// 3 212 bytes in section .text
// 
// 2 898 bytes of CODE  memory (+ 318 bytes shared)
//   596 bytes of CONST memory (+  52 bytes shared)
// 3 904 bytes of DATA  memory
//
//Errors: none
//Warnings: 3
