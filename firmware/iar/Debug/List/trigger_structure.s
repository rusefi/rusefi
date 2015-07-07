///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:58 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\tri /
//                       gger\trigger_structure.cpp                           /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\tri /
//                       gger\trigger_structure.cpp -lCN                      /
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
//                       trigger_structure.s                                  /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME trigger_structure

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z14getEngineCycle16operation_mode_e
        EXTERN _Z17checkSwitchTimes2iPf
        EXTERN _Z25findTriggerZeroEventIndexP12TriggerShapePK16trigger_config_s
        EXTERN _Z28addSkippedToothTriggerEvents15trigger_wheel_eP12TriggerShapeiifffff
        EXTERN _Z37initializeSkippedToothTriggerShapeExtP12TriggerShapeii16operation_mode_e
        EXTERN _ZN12multi_wave_s5resetEv
        EXTERN _ZN12multi_wave_sC1EPfP13single_wave_s
        EXTERN _ZN13single_wave_s4initEPa
        EXTERN _ZN13single_wave_sC1Ev
        EXTERN _ZNK12multi_wave_s13getSwitchTimeEi
        EXTERN __aeabi_f2d
        EXTERN __aeabi_memset
        EXTERN __aeabi_vec_ctor_nocookie_nodtor
        EXTERN engine
        EXTERN engineConfiguration
        EXTERN firmwareError
        EXTERN isSameF
        EXTERN persistentState

        PUBLIC _Z18setVwConfigurationP12TriggerShape
        PUBLIC _Z22configureHondaAccordCDP12TriggerShapebb15trigger_wheel_eS1_f
        PUBLIC _Z25configureHondaAccordCDDipP12TriggerShape
        PUBLIC _Z28setToothedWheelConfigurationP12TriggerShapeii16operation_mode_e
        PUBLIC _ZN12TriggerShape16getOperationModeEv
        PUBLIC _ZN12TriggerShape26calculateTriggerSynchPointEv
        PUBLIC _ZN12TriggerShape28setTriggerSynchronizationGapEf
        PUBLIC _ZN12TriggerShape29setTriggerSynchronizationGap2Eff
        PUBLIC _ZN12TriggerShape30getTriggerShapeSynchPointIndexEv
        PUBLIC _ZN12TriggerShape5clearEv
        PUBLIC _ZN12TriggerShape5resetE16operation_mode_eb
        PUBLIC _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        PUBLIC _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_eff
        PUBLIC _ZN12TriggerShapeC1Ev
        PUBLIC _ZN12TriggerShapeC2Ev
        PUBLIC _ZN12TriggerState15getCurrentIndexEv
        PUBLIC _ZN12TriggerState20getTotalEventCounterEv
        PUBLIC _ZN12TriggerState20resetRunningCountersEv
        PUBLIC _ZN12TriggerState25getStartOfRevolutionIndexEv
        PUBLIC _ZN12TriggerState25getTotalRevolutionCounterEv
        PUBLIC _ZN12TriggerState5clearEv
        PUBLIC _ZN12TriggerStateC1Ev
        PUBLIC _ZN12TriggerStateC2Ev
        PUBLIC _ZN12multi_wave_s13setSwitchTimeEif
        PUBLIC _ZN12multi_wave_s16checkSwitchTimesEi
        PUBLIC _ZN20trigger_shape_helperC1Ev
        PUBLIC _ZN20trigger_shape_helperC2Ev
        PUBLIC _ZNK12TriggerShape14getSwitchAngleEi
        PUBLIC _ZNK12TriggerShape16getCycleDurationEv
        PUBLIC _ZNK12TriggerShape7getSizeEv
        PUBLIC _ZNK12TriggerShape8getAngleEi
        PUBLIC _ZNK12TriggerShape9getLengthEv
        PUBLIC _ZNK12multi_wave_s14findAngleMatchEfi
        PUBLIC _ZNK12multi_wave_s15getChannelStateEii
        PUBLIC _ZNK12multi_wave_s18waveIndertionAngleEfi
        PUBLIC memset
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\trigger\trigger_structure.cpp
//    1 /**
//    2  * @file	trigger_structure.cpp
//    3  *
//    4  * @date Jan 20, 2014
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
//   22 #include "trigger_structure.h"
//   23 #include "error_handling.h"
//   24 #include "trigger_decoder.h"
//   25 #include "engine_math.h"
//   26 
//   27 EXTERN_ENGINE;
//   28 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _ZN20trigger_shape_helperC2Ev
        THUMB
// __code __interwork __softfp trigger_shape_helper::subobject trigger_shape_helper()
_ZN20trigger_shape_helperC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN20trigger_shape_helperC1Ev
        BL       _ZN20trigger_shape_helperC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _ZN12TriggerShapeC2Ev
        THUMB
// __code __interwork __softfp TriggerShape::subobject TriggerShape()
_ZN12TriggerShapeC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShapeC1Ev
        BL       _ZN12TriggerShapeC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _ZN12TriggerStateC2Ev
        THUMB
// __code __interwork __softfp TriggerState::subobject TriggerState()
_ZN12TriggerStateC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerStateC1Ev
        BL       _ZN12TriggerStateC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _ZN20trigger_shape_helperC1Ev
        THUMB
//   29 trigger_shape_helper::trigger_shape_helper() {
_ZN20trigger_shape_helperC1Ev:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R3,#+3
        MOVS     R2,#+4
        LDR.W    R1,??DataTable7_1
        MOVS     R0,R4
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
//   30 	memset(&pinStates, 0, sizeof(pinStates));
        MOV      R2,#+756
        MOVS     R1,#+0
        ADDS     R0,R4,#+12
          CFI FunCall memset
        BL       memset
//   31 	for (int i = 0; i < TRIGGER_CHANNEL_COUNT; i++) {
        MOVS     R5,#+0
??trigger_shape_helper_0:
        CMP      R5,#+3
        BGE.N    ??trigger_shape_helper_1
//   32 		waves[i].init(pinStates[i]);
        MOVS     R0,#+252
        MLA      R0,R0,R5,R4
        ADDS     R1,R0,#+12
        ADDS     R0,R4,R5, LSL #+2
          CFI FunCall _ZN13single_wave_s4initEPa
        BL       _ZN13single_wave_s4initEPa
//   33 	}
        ADDS     R5,R5,#+1
        B.N      ??trigger_shape_helper_0
//   34 }
??trigger_shape_helper_1:
        MOVS     R0,R4
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock3
//   35 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _ZN12TriggerShapeC1Ev
        THUMB
//   36 TriggerShape::TriggerShape() :
_ZN12TriggerShapeC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   37 		wave(switchTimesBuffer, NULL) {
        MOVS     R2,#+0
        ADD      R1,R4,#+5888
        ADDS     R1,R1,#+116
        ADDW     R0,R4,#+2932
          CFI FunCall _ZN12multi_wave_sC1EPfP13single_wave_s
        BL       _ZN12multi_wave_sC1EPfP13single_wave_s
        ADD      R0,R4,#+5120
        ADDS     R0,R0,#+112
          CFI FunCall _ZN20trigger_shape_helperC1Ev
        BL       _ZN20trigger_shape_helperC1Ev
//   38 	reset(OM_NONE, false);
        MOVS     R2,#+0
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape5resetE16operation_mode_eb
        BL       _ZN12TriggerShape5resetE16operation_mode_eb
//   39 	wave.waves = h.waves;
        ADD      R0,R4,#+5120
        ADDS     R0,R0,#+112
        STR      R0,[R4, #+2936]
//   40 	// todo: false here, true in clear() what a mess!
//   41 	useRiseEdge = false;
        MOVS     R0,#+0
        STRB     R0,[R4, #+2916]
//   42 	gapBothDirections = false;
        MOVS     R0,#+0
        STRB     R0,[R4, #+2917]
//   43 	isSynchronizationNeeded = false;
        MOVS     R0,#+0
        STRB     R0,[R4, #+0]
//   44 	// todo: reuse 'clear' method?
//   45 	invertOnAdd = false;
        MOVW     R0,#+5224
        MOVS     R1,#+0
        STRB     R1,[R0, R4]
//   46 	tdcPosition = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+2912]
//   47 	skippedToothCount = totalToothCount = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+4]
        STR      R0,[R4, #+8]
//   48 	syncRatioFrom = syncRatioTo = 0;
        VLDR.W   S0,??DataTable2  ;; 0x0
        ADDW     R0,R4,#+2908
        VSTR     S0,[R0, #0]
        ADDW     R0,R4,#+2904
        VSTR     S0,[R0, #0]
//   49 	memset(eventAngles, 0, sizeof(eventAngles));
        MOV      R2,#+1008
        MOVS     R1,#+0
        ADDW     R0,R4,#+2956
          CFI FunCall memset
        BL       memset
//   50 	memset(frontOnlyIndexes, 0, sizeof(frontOnlyIndexes));
        MOV      R2,#+1008
        MOVS     R1,#+0
        ADD      R0,R4,#+4096
        ADDS     R0,R0,#+120
          CFI FunCall memset
        BL       memset
//   51 	memset(isFrontEvent, 0, sizeof(isFrontEvent));
        MOVS     R2,#+252
        MOVS     R1,#+0
        ADDW     R0,R4,#+3964
          CFI FunCall memset
        BL       memset
//   52 	memset(triggerIndexByAngle, 0, sizeof(triggerIndexByAngle));
        MOV      R2,#+2880
        MOVS     R1,#+0
        ADDS     R0,R4,#+24
          CFI FunCall memset
        BL       memset
//   53 #if EFI_UNIT_TEST
//   54 	memset(&events, 0, sizeof(events));
//   55 #endif
//   56 
//   57 }
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock4
//   58 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _ZNK12TriggerShape7getSizeEv
          CFI NoCalls
        THUMB
//   59 int TriggerShape::getSize() const {
//   60 	return size;
_ZNK12TriggerShape7getSizeEv:
        MOVW     R1,#+5228
        LDR      R0,[R1, R0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock5
//   61 }
//   62 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _ZN12TriggerShape30getTriggerShapeSynchPointIndexEv
          CFI NoCalls
        THUMB
//   63 int TriggerShape::getTriggerShapeSynchPointIndex() {
//   64 	return triggerShapeSynchPointIndex;
_ZN12TriggerShape30getTriggerShapeSynchPointIndexEv:
        MOVW     R1,#+6000
        LDR      R0,[R1, R0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock6
//   65 }
//   66 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _ZN12TriggerShape26calculateTriggerSynchPointEv
        THUMB
//   67 void TriggerShape::calculateTriggerSynchPoint(DECLARE_ENGINE_PARAMETER_F) {
_ZN12TriggerShape26calculateTriggerSynchPointEv:
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
        MOVS     R4,R0
//   68 	trigger_config_s const*triggerConfig = &engineConfiguration->trigger;
        LDR.W    R0,??DataTable7_6
        LDR      R0,[R0, #+0]
        ADDS     R5,R0,#+536
//   69 
//   70 	triggerShapeSynchPointIndex = findTriggerZeroEventIndex(this, triggerConfig PASS_ENGINE_PARAMETER);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z25findTriggerZeroEventIndexP12TriggerShapePK16trigger_config_s
        BL       _Z25findTriggerZeroEventIndexP12TriggerShapePK16trigger_config_s
        MOVW     R1,#+6000
        STR      R0,[R1, R4]
//   71 
//   72 	engine->engineCycleEventCount = getLength();
        MOVS     R0,R4
          CFI FunCall _ZNK12TriggerShape9getLengthEv
        BL       _ZNK12TriggerShape9getLengthEv
        MOVW     R1,#+7932
        LDR.W    R2,??DataTable7_8
        LDR      R2,[R2, #+0]
        STR      R0,[R1, R2]
//   73 
//   74 	float firstAngle = getAngle(triggerShapeSynchPointIndex);
        MOVW     R0,#+6000
        LDR      R1,[R0, R4]
        MOVS     R0,R4
          CFI FunCall _ZNK12TriggerShape8getAngleEi
        BL       _ZNK12TriggerShape8getAngleEi
        VMOV.F32 S16,S0
//   75 
//   76 	int frontOnlyIndex = 0;
        MOVS     R6,#+0
//   77 
//   78 	for (int eventIndex = 0; eventIndex < engine->engineCycleEventCount; eventIndex++) {
        MOVS     R7,#+0
??calculateTriggerSynchPoint_0:
        MOVW     R0,#+7932
        LDR.W    R1,??DataTable7_8
        LDR      R1,[R1, #+0]
        LDR      R0,[R0, R1]
        CMP      R7,R0
        BGE.W    ??calculateTriggerSynchPoint_1
//   79 		if (eventIndex == 0) {
        CMP      R7,#+0
        BNE.N    ??calculateTriggerSynchPoint_2
//   80 			// explicit check for zero to avoid issues where logical zero is not exactly zero due to float nature
//   81 			eventAngles[0] = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+2956]
//   82 			// this value would be used in case of front-only
//   83 			eventAngles[1] = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+2960]
//   84 			frontOnlyIndexes[0] = 0;
        MOVW     R0,#+4216
        MOVS     R1,#+0
        STR      R1,[R0, R4]
        B.N      ??calculateTriggerSynchPoint_3
//   85 		} else {
//   86 			int triggerDefinitionCoordinate = (triggerShapeSynchPointIndex + eventIndex) % engine->engineCycleEventCount;
??calculateTriggerSynchPoint_2:
        MOVW     R0,#+6000
        LDR      R0,[R0, R4]
        ADDS     R0,R7,R0
        MOVW     R1,#+7932
        LDR.W    R2,??DataTable7_8
        LDR      R2,[R2, #+0]
        LDR      R1,[R1, R2]
        SDIV     R2,R0,R1
        MLS      R8,R1,R2,R0
//   87 			int triggerDefinitionIndex = triggerDefinitionCoordinate >= size ? triggerDefinitionCoordinate - size : triggerDefinitionCoordinate;
        MOVW     R0,#+5228
        LDR      R0,[R0, R4]
        CMP      R8,R0
        BLT.N    ??calculateTriggerSynchPoint_4
        MOVW     R0,#+5228
        LDR      R0,[R0, R4]
        SUBS     R9,R8,R0
        B.N      ??calculateTriggerSynchPoint_5
??calculateTriggerSynchPoint_4:
        MOV      R9,R8
//   88 			float angle = getAngle(triggerDefinitionCoordinate) - firstAngle;
??calculateTriggerSynchPoint_5:
        MOV      R1,R8
        MOVS     R0,R4
          CFI FunCall _ZNK12TriggerShape8getAngleEi
        BL       _ZNK12TriggerShape8getAngleEi
        VSUB.F32 S17,S0,S16
//   89 			fixAngle(angle);
        LDR.W    R0,??DataTable7_9
        LDR      R0,[R0, #+424]
        CMP      R0,#+0
        BNE.N    ??calculateTriggerSynchPoint_6
        LDR.W    R0,??DataTable7_11
          CFI FunCall firmwareError
        BL       firmwareError
??calculateTriggerSynchPoint_6:
        VCMP.F32 S17,#0.0
        FMSTAT   
        BPL.N    ??calculateTriggerSynchPoint_7
        LDR.W    R0,??DataTable7_9
        VLDR     S0,[R0, #+424]
        VCVT.F32.S32 S0,S0
        VADD.F32 S17,S0,S17
        B.N      ??calculateTriggerSynchPoint_6
??calculateTriggerSynchPoint_7:
        LDR.W    R0,??DataTable7_9
        VLDR     S0,[R0, #+424]
        VCVT.F32.S32 S0,S0
        VCMP.F32 S17,S0
        FMSTAT   
        BLT.N    ??calculateTriggerSynchPoint_8
        LDR.W    R0,??DataTable7_9
        VLDR     S0,[R0, #+424]
        VCVT.F32.S32 S0,S0
        VSUB.F32 S17,S17,S0
        B.N      ??calculateTriggerSynchPoint_7
//   90 			if (engineConfiguration->useOnlyFrontForTrigger) {
??calculateTriggerSynchPoint_8:
        LDR.W    R0,??DataTable7_6
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+6
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??calculateTriggerSynchPoint_9
//   91 				if (isFrontEvent[triggerDefinitionIndex]) {
        ADDS     R0,R9,R4
        LDRSB    R0,[R0, #+3964]
        CMP      R0,#+0
        BEQ.N    ??calculateTriggerSynchPoint_10
//   92 					frontOnlyIndex += 2;
        ADDS     R6,R6,#+2
//   93 					eventAngles[frontOnlyIndex] = angle;
        ADDS     R0,R4,R6, LSL #+2
        ADDW     R0,R0,#+2956
        VSTR     S17,[R0, #0]
//   94 					eventAngles[frontOnlyIndex + 1] = angle;
        ADDS     R0,R4,R6, LSL #+2
        ADDW     R0,R0,#+2960
        VSTR     S17,[R0, #0]
        B.N      ??calculateTriggerSynchPoint_10
//   95 				}
//   96 			} else {
//   97 				eventAngles[eventIndex] = angle;
??calculateTriggerSynchPoint_9:
        ADDS     R0,R4,R7, LSL #+2
        ADDW     R0,R0,#+2956
        VSTR     S17,[R0, #0]
//   98 			}
//   99 
//  100 			frontOnlyIndexes[eventIndex] = frontOnlyIndex;
??calculateTriggerSynchPoint_10:
        MOVW     R0,#+4216
        ADDS     R1,R4,R7, LSL #+2
        STR      R6,[R0, R1]
//  101 		}
//  102 	}
??calculateTriggerSynchPoint_3:
        ADDS     R7,R7,#+1
        B.N      ??calculateTriggerSynchPoint_0
//  103 }
??calculateTriggerSynchPoint_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+32
        POP      {R0,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock7

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2:
        DC32     0x0
//  104 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _ZN12TriggerShape5clearEv
        THUMB
//  105 void TriggerShape::clear() {
_ZN12TriggerShape5clearEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  106 	tdcPosition = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+2912]
//  107 	setTriggerSynchronizationGap(2);
        VMOV.F32 S0,#2.0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape28setTriggerSynchronizationGapEf
        BL       _ZN12TriggerShape28setTriggerSynchronizationGapEf
//  108 	// todo: true here, false in constructor() what a mess!
//  109 	useRiseEdge = true;
        MOVS     R0,#+1
        STRB     R0,[R4, #+2916]
//  110 
//  111 	invertOnAdd = false;
        MOVW     R0,#+5224
        MOVS     R1,#+0
        STRB     R1,[R0, R4]
//  112 	gapBothDirections = false;
        MOVS     R0,#+0
        STRB     R0,[R4, #+2917]
//  113 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock8
//  114 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _ZN12TriggerShape5resetE16operation_mode_eb
        THUMB
//  115 void TriggerShape::reset(operation_mode_e operationMode, bool needSecondTriggerInput) {
_ZN12TriggerShape5resetE16operation_mode_eb:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  116 	this->operationMode = operationMode;
        MOVW     R0,#+7016
        STR      R5,[R0, R4]
//  117 	size = 0;
        MOVW     R0,#+5228
        MOVS     R1,#+0
        STR      R1,[R0, R4]
//  118 	this->needSecondTriggerInput = needSecondTriggerInput;
        STRB     R6,[R4, #+1]
//  119 	triggerShapeSynchPointIndex = 0;
        MOVW     R0,#+6000
        MOVS     R1,#+0
        STR      R1,[R0, R4]
//  120 	memset(initialState, 0, sizeof(initialState));
        MOVS     R2,#+12
        MOVS     R1,#+0
        ADDS     R0,R4,#+2944
          CFI FunCall memset
        BL       memset
//  121 	memset(switchTimesBuffer, 0, sizeof(switchTimesBuffer));
        MOV      R2,#+1008
        MOVS     R1,#+0
        ADD      R0,R4,#+5888
        ADDS     R0,R0,#+116
          CFI FunCall memset
        BL       memset
//  122 	memset(expectedEventCount, 0, sizeof(expectedEventCount));
        MOVS     R2,#+12
        MOVS     R1,#+0
        ADDW     R0,R4,#+2920
          CFI FunCall memset
        BL       memset
//  123 	wave.reset();
        ADDW     R0,R4,#+2932
          CFI FunCall _ZN12multi_wave_s5resetEv
        BL       _ZN12multi_wave_s5resetEv
//  124 	previousAngle = 0;
        MOVW     R0,#+7012
        MOVS     R1,#+0
        STR      R1,[R0, R4]
//  125 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock9
//  126 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _ZNK12multi_wave_s15getChannelStateEii
          CFI NoCalls
        THUMB
//  127 int multi_wave_s::getChannelState(int channelIndex, int phaseIndex) const {
//  128 	return waves[channelIndex].pinStates[phaseIndex];
_ZNK12multi_wave_s15getChannelStateEii:
        LDR      R0,[R0, #+4]
        LDR      R0,[R0, R1, LSL #+2]
        LDRSB    R0,[R2, R0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock10
//  129 }
//  130 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _ZNK12multi_wave_s18waveIndertionAngleEfi
          CFI NoCalls
        THUMB
//  131 int multi_wave_s::waveIndertionAngle(float angle, int size) const {
_ZNK12multi_wave_s18waveIndertionAngleEfi:
        MOVS     R2,R0
//  132 	for (int i = size - 1; i >= 0; i--) {
        SUBS     R0,R1,#+1
??waveIndertionAngle_0:
        CMP      R0,#+0
        BMI.N    ??waveIndertionAngle_1
//  133 		if (angle > switchTimes[i])
        LDR      R3,[R2, #+8]
        ADDS     R3,R3,R0, LSL #+2
        VLDR     S1,[R3, #0]
        VCMP.F32 S1,S0
        FMSTAT   
        BPL.N    ??waveIndertionAngle_2
//  134 			return i + 1;
        ADDS     R0,R0,#+1
        B.N      ??waveIndertionAngle_3
//  135 	}
??waveIndertionAngle_2:
        SUBS     R0,R0,#+1
        B.N      ??waveIndertionAngle_0
//  136 	return 0;
??waveIndertionAngle_1:
        MOVS     R0,#+0
??waveIndertionAngle_3:
        BX       LR               ;; return
          CFI EndBlock cfiBlock11
//  137 }
//  138 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _ZNK12multi_wave_s14findAngleMatchEfi
        THUMB
//  139 int multi_wave_s::findAngleMatch(float angle, int size) const {
_ZNK12multi_wave_s14findAngleMatchEfi:
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
        VMOV.F32 S16,S0
        MOVS     R5,R1
//  140 	for (int i = 0; i < size; i++) {
        MOVS     R6,#+0
??findAngleMatch_0:
        CMP      R6,R5
        BGE.N    ??findAngleMatch_1
//  141 		if (isSameF(switchTimes[i], angle))
        VMOV.F32 S1,S16
        LDR      R0,[R4, #+8]
        ADDS     R0,R0,R6, LSL #+2
        VLDR     S0,[R0, #0]
          CFI FunCall isSameF
        BL       isSameF
        CMP      R0,#+0
        BEQ.N    ??findAngleMatch_2
//  142 			return i;
        MOVS     R0,R6
        B.N      ??findAngleMatch_3
//  143 	}
??findAngleMatch_2:
        ADDS     R6,R6,#+1
        B.N      ??findAngleMatch_0
//  144 	return EFI_ERROR_CODE;
??findAngleMatch_1:
        MOVS     R0,#-1
??findAngleMatch_3:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock12
//  145 }
//  146 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _ZN12multi_wave_s13setSwitchTimeEif
          CFI NoCalls
        THUMB
//  147 void multi_wave_s::setSwitchTime(int index, float value) {
//  148 	switchTimes[index] = value;
_ZN12multi_wave_s13setSwitchTimeEif:
        LDR      R2,[R0, #+8]
        ADDS     R2,R2,R1, LSL #+2
        VSTR     S0,[R2, #0]
//  149 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock13
//  150 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _ZN12TriggerStateC1Ev
        THUMB
//  151 TriggerState::TriggerState() {
_ZN12TriggerStateC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  152 	cycleCallback = NULL;
        MOVS     R0,#+0
        STR      R0,[R4, #+0]
//  153 	shaft_is_synchronized = false;
        MOVS     R0,#+0
        STRB     R0,[R4, #+4]
//  154 	toothed_previous_time = 0;
        MOVS     R0,#+0
        MOVS     R1,#+0
        STRD     R0,R1,[R4, #+16]
//  155 	toothed_previous_duration = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+8]
//  156 
//  157 	totalRevolutionCounter = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+160]
//  158 	totalTriggerErrorCounter = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+60]
//  159 	orderingErrorCounter = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+68]
//  160 	currentDuration = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+12]
//  161 	curSignal = SHAFT_PRIMARY_DOWN;
        MOVS     R0,#+0
        STRB     R0,[R4, #+96]
//  162 	prevSignal = SHAFT_PRIMARY_DOWN;
        MOVS     R0,#+0
        STRB     R0,[R4, #+97]
//  163 	prevCycleDuration = 0;
        MOVS     R0,#+0
        MOVS     R1,#+0
        STRD     R0,R1,[R4, #+168]
//  164 	startOfCycleNt = 0;
        MOVS     R0,#+0
        MOVS     R1,#+0
        STRD     R0,R1,[R4, #+176]
//  165 
//  166 	resetRunningCounters();
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerState20resetRunningCountersEv
        BL       _ZN12TriggerState20resetRunningCountersEv
//  167 	clear();
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerState5clearEv
        BL       _ZN12TriggerState5clearEv
//  168 	memset(expectedTotalTime, 0, sizeof(expectedTotalTime));
        MOVS     R2,#+12
        MOVS     R1,#+0
        ADDS     R0,R4,#+48
          CFI FunCall memset
        BL       memset
//  169 	totalEventCountBase = 0;
        MOVS     R0,#+0
        MOVS     R1,#+0
        STRD     R0,R1,[R4, #+152]
//  170 	isFirstEvent = true;
        MOVS     R0,#+1
        STRB     R0,[R4, #+164]
//  171 }
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock14
//  172 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function _ZN12TriggerState15getCurrentIndexEv
          CFI NoCalls
        THUMB
//  173 int TriggerState::getCurrentIndex() {
//  174 	return current_index;
_ZN12TriggerState15getCurrentIndexEv:
        LDR      R0,[R0, #+76]
        BX       LR               ;; return
          CFI EndBlock cfiBlock15
//  175 }
//  176 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function _ZN12TriggerState25getStartOfRevolutionIndexEv
          CFI NoCalls
        THUMB
//  177 efitime_t TriggerState::getStartOfRevolutionIndex() {
//  178 	return totalEventCountBase;
_ZN12TriggerState25getStartOfRevolutionIndexEv:
        LDRD     R2,R3,[R0, #+152]
        MOVS     R0,R2
        MOVS     R1,R3
        BX       LR               ;; return
          CFI EndBlock cfiBlock16
//  179 }
//  180 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function _ZN12TriggerState20resetRunningCountersEv
          CFI NoCalls
        THUMB
//  181 void TriggerState::resetRunningCounters() {
//  182 	runningRevolutionCounter = 0;
_ZN12TriggerState20resetRunningCountersEv:
        MOVS     R1,#+0
        STR      R1,[R0, #+80]
//  183 	runningTriggerErrorCounter = 0;
        MOVS     R1,#+0
        STR      R1,[R0, #+64]
//  184 	runningOrderingErrorCounter = 0;
        MOVS     R1,#+0
        STR      R1,[R0, #+72]
//  185 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock17
//  186 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function _ZN12TriggerState20getTotalEventCounterEv
          CFI NoCalls
        THUMB
//  187 efitime_t TriggerState::getTotalEventCounter() {
//  188 	return totalEventCountBase + current_index;
_ZN12TriggerState20getTotalEventCounterEv:
        LDRD     R2,R3,[R0, #+152]
        LDR      R0,[R0, #+76]
        MOVS     R1,#+0
        ADDS     R0,R2,R0
        ADCS     R1,R3,R1
        BX       LR               ;; return
          CFI EndBlock cfiBlock18
//  189 }
//  190 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock19 Using cfiCommon0
          CFI Function _ZN12TriggerState25getTotalRevolutionCounterEv
          CFI NoCalls
        THUMB
//  191 int TriggerState::getTotalRevolutionCounter() {
//  192 	return totalRevolutionCounter;
_ZN12TriggerState25getTotalRevolutionCounterEv:
        LDR      R0,[R0, #+160]
        BX       LR               ;; return
          CFI EndBlock cfiBlock19
//  193 }
//  194 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock20 Using cfiCommon0
          CFI Function _ZN12TriggerState5clearEv
        THUMB
//  195 void TriggerState::clear() {
_ZN12TriggerState5clearEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  196 	memset(eventCount, 0, sizeof(eventCount));
        MOVS     R2,#+12
        MOVS     R1,#+0
        ADDS     R0,R4,#+84
          CFI FunCall memset
        BL       memset
//  197 	memset(eventCountExt, 0, sizeof(eventCountExt));
        MOVS     R2,#+24
        MOVS     R1,#+0
        ADDS     R0,R4,#+100
          CFI FunCall memset
        BL       memset
//  198 	memset(timeOfPreviousEventNt, 0, sizeof(timeOfPreviousEventNt));
        MOVS     R2,#+24
        MOVS     R1,#+0
        ADDS     R0,R4,#+128
          CFI FunCall memset
        BL       memset
//  199 	memset(totalTimeNt, 0, sizeof(totalTimeNt));
        MOVS     R2,#+12
        MOVS     R1,#+0
        ADDS     R0,R4,#+24
          CFI FunCall memset
        BL       memset
//  200 	current_index = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+76]
//  201 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock20
//  202 
//  203 /**
//  204  * Trigger event count equals engine cycle event count if we have a cam sensor.
//  205  * Two trigger cycles make one engine cycle in case of a four stroke engine If we only have a cranksensor.
//  206  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock21 Using cfiCommon0
          CFI Function _ZNK12TriggerShape9getLengthEv
        THUMB
//  207 uint32_t TriggerShape::getLength() const {
_ZNK12TriggerShape9getLengthEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  208 	return operationMode == FOUR_STROKE_CAM_SENSOR ? getSize() : 2 * getSize();
        MOVW     R0,#+7016
        LDR      R0,[R0, R4]
        CMP      R0,#+2
        BNE.N    ??getLength_0
        MOVS     R0,R4
          CFI FunCall _ZNK12TriggerShape7getSizeEv
        BL       _ZNK12TriggerShape7getSizeEv
        B.N      ??getLength_1
??getLength_0:
        MOVS     R0,R4
          CFI FunCall _ZNK12TriggerShape7getSizeEv
        BL       _ZNK12TriggerShape7getSizeEv
        LSLS     R0,R0,#+1
??getLength_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock21
//  209 }
//  210 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock22 Using cfiCommon0
          CFI Function _ZNK12TriggerShape8getAngleEi
        THUMB
//  211 float TriggerShape::getAngle(int index) const {
_ZNK12TriggerShape8getAngleEi:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  212 	// todo: why is this check here? looks like the code below could be used universally
//  213 	if (operationMode == FOUR_STROKE_CAM_SENSOR) {
        MOVW     R0,#+7016
        LDR      R0,[R0, R4]
        CMP      R0,#+2
        BNE.N    ??getAngle_0
//  214 		return getSwitchAngle(index);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _ZNK12TriggerShape14getSwitchAngleEi
        BL       _ZNK12TriggerShape14getSwitchAngleEi
        B.N      ??getAngle_1
//  215 	}
//  216 	/**
//  217 	 * FOUR_STROKE_CRANK_SENSOR magic:
//  218 	 * We have two crank shaft revolutions for each engine cycle
//  219 	 * See also trigger_central.cpp
//  220 	 * See also getEngineCycleEventCount()
//  221 	 */
//  222 	int triggerEventCounter = size;
??getAngle_0:
        MOVW     R0,#+5228
        LDR      R6,[R0, R4]
//  223 
//  224 	if (index < triggerEventCounter) {
        CMP      R5,R6
        BGE.N    ??getAngle_2
//  225 		return getSwitchAngle(index);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _ZNK12TriggerShape14getSwitchAngleEi
        BL       _ZNK12TriggerShape14getSwitchAngleEi
        B.N      ??getAngle_1
//  226 	} else {
//  227 		return 360 + getSwitchAngle(index - triggerEventCounter);
??getAngle_2:
        SUBS     R1,R5,R6
        MOVS     R0,R4
          CFI FunCall _ZNK12TriggerShape14getSwitchAngleEi
        BL       _ZNK12TriggerShape14getSwitchAngleEi
        VLDR.W   S1,??DataTable4  ;; 0x43b40000
        VADD.F32 S0,S0,S1
??getAngle_1:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock22
//  228 	}
//  229 }
//  230 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock23 Using cfiCommon0
          CFI Function _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_eff
        THUMB
//  231 void TriggerShape::addEvent(float angle, trigger_wheel_e const waveIndex, trigger_value_e const stateParam, float filterLeft, float filterRight) {
_ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_eff:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        VPUSH    {D8-D9}
          CFI D9 Frame(CFA, -24)
          CFI D8 Frame(CFA, -32)
          CFI CFA R13+32
        MOVS     R4,R0
        VMOV.F32 S18,S0
        MOVS     R5,R1
        MOVS     R6,R2
        VMOV.F32 S16,S1
        VMOV.F32 S17,S2
//  232 	if (angle > filterLeft && angle < filterRight)
        VCMP.F32 S16,S18
        FMSTAT   
        BPL.N    ??addEvent_0
        VCMP.F32 S18,S17
        FMSTAT   
        BPL.N    ??addEvent_0
//  233 		addEvent(angle, waveIndex, stateParam);
        MOVS     R2,R6
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        VMOV.F32 S0,S18
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  234 }
??addEvent_0:
        VPOP     {D8-D9}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI CFA R13+16
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock23
//  235 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock24 Using cfiCommon0
          CFI Function _ZN12TriggerShape16getOperationModeEv
          CFI NoCalls
        THUMB
//  236 operation_mode_e TriggerShape::getOperationMode() {
//  237 	return operationMode;
_ZN12TriggerShape16getOperationModeEv:
        MOVW     R1,#+7016
        LDR      R0,[R1, R0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock24
//  238 }
//  239 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock25 Using cfiCommon0
          CFI Function _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        THUMB
//  240 void TriggerShape::addEvent(float angle, trigger_wheel_e const waveIndex, trigger_value_e const stateParam) {
_ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e:
        PUSH     {R1,R4-R11,LR}
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
        VPUSH    {D8}
          CFI D8 Frame(CFA, -48)
          CFI CFA R13+48
        SUB      SP,SP,#+8
          CFI CFA R13+56
        MOVS     R4,R0
        VMOV.F32 S16,S0
        MOVS     R5,R2
//  241 	efiAssertVoid(operationMode != OM_NONE, "operationMode not set");
        MOVW     R0,#+7016
        LDR      R0,[R0, R4]
        CMP      R0,#+0
        BNE.N    ??addEvent_1
        LDR.W    R0,??DataTable7_13
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??addEvent_2
//  242 
//  243 	efiAssertVoid(waveIndex!= T_SECONDARY || needSecondTriggerInput, "secondary needed or not?");
??addEvent_1:
        LDRB     R0,[SP, #+16]
        CMP      R0,#+1
        BNE.N    ??addEvent_3
        LDRB     R0,[R4, #+1]
        CMP      R0,#+0
        BNE.N    ??addEvent_3
        LDR.W    R0,??DataTable7_14
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??addEvent_2
//  244 
//  245 	trigger_value_e state;
//  246 	if (invertOnAdd) {
??addEvent_3:
        MOVW     R0,#+5224
        LDRB     R0,[R0, R4]
        CMP      R0,#+0
        BEQ.N    ??addEvent_4
//  247 		state = (stateParam == TV_LOW) ? TV_HIGH : TV_LOW;
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BNE.N    ??addEvent_5
        MOVS     R6,#+1
        B.N      ??addEvent_6
??addEvent_5:
        MOVS     R6,#+0
        B.N      ??addEvent_6
//  248 	} else {
//  249 		state = stateParam;
??addEvent_4:
        MOVS     R6,R5
//  250 	}
//  251 
//  252 #if EFI_UNIT_TEST
//  253 	int signal = waveIndex * 1000 + stateParam;
//  254 	events[size] = signal;
//  255 #endif
//  256 
//  257 	float engineCycle = getEngineCycle(operationMode);
??addEvent_6:
        MOVW     R0,#+7016
        LDR      R0,[R0, R4]
          CFI FunCall _Z14getEngineCycle16operation_mode_e
        BL       _Z14getEngineCycle16operation_mode_e
        VMOV.F32 S17,S0
//  258 
//  259 	/**
//  260 	 * While '720' value works perfectly it has not much sense for crank sensor-only scenario.
//  261 	 * todo: accept angle as a value in the 0..1 range?
//  262 	 */
//  263 	angle /= engineCycle;
        VDIV.F32 S16,S16,S17
//  264 
//  265 #if EFI_PROD_CODE
//  266 // todo: PASS_ENGINE?
//  267 	if (!engineConfiguration->useOnlyFrontForTrigger || stateParam == TV_HIGH) {
        LDR.W    R0,??DataTable7_6
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+6
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??addEvent_7
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+1
        BNE.N    ??addEvent_8
//  268 		expectedEventCount[waveIndex]++;
??addEvent_7:
        LDRB     R0,[SP, #+16]
        ADDS     R0,R4,R0, LSL #+2
        LDR      R0,[R0, #+2920]
        ADDS     R0,R0,#+1
        LDRB     R1,[SP, #+16]
        ADDS     R1,R4,R1, LSL #+2
        STR      R0,[R1, #+2920]
//  269 	}
//  270 #endif
//  271 
//  272 	efiAssertVoid(angle > 0, "angle should be positive");
??addEvent_8:
        VCMP.F32 S16,#0.0
        FMSTAT   
        BGT.N    ??addEvent_9
        LDR.W    R0,??DataTable7_15
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??addEvent_2
//  273 	if (size > 0) {
??addEvent_9:
        MOVW     R0,#+5228
        LDR      R0,[R0, R4]
        CMP      R0,#+1
        BLT.N    ??addEvent_10
//  274 		if (angle <= previousAngle) {
        ADD      R0,R4,#+6912
        ADDS     R0,R0,#+100
        VLDR     S0,[R0, #0]
        VCMP.F32 S0,S16
        FMSTAT   
        BLT.N    ??addEvent_10
//  275 			firmwareError("invalid angle order: %f and %f", angle, previousAngle);
        MOVW     R0,#+7012
        LDR      R0,[R0, R4]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R0,??DataTable7_16
          CFI FunCall firmwareError
        BL       firmwareError
//  276 			return;
        B.N      ??addEvent_2
//  277 		}
//  278 	}
//  279 	previousAngle = angle;
??addEvent_10:
        ADD      R0,R4,#+6912
        ADDS     R0,R0,#+100
        VSTR     S16,[R0, #0]
//  280 	if (size == 0) {
        MOVW     R0,#+5228
        LDR      R0,[R0, R4]
        CMP      R0,#+0
        BNE.N    ??addEvent_11
//  281 		size = 1;
        MOVW     R0,#+5228
        MOVS     R1,#+1
        STR      R1,[R0, R4]
//  282 		for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++) {
        MOVS     R7,#+0
??addEvent_12:
        CMP      R7,#+3
        BGE.N    ??addEvent_13
//  283 			single_wave_s *wave = &this->wave.waves[i];
        LDR      R0,[R4, #+2936]
        ADDS     R8,R0,R7, LSL #+2
//  284 
//  285 			if (wave->pinStates == NULL) {
        LDR      R0,[R8, #+0]
        CMP      R0,#+0
        BNE.N    ??addEvent_14
//  286 				firmwareError("wave pinStates is NULL");
        LDR.W    R0,??DataTable7_17
          CFI FunCall firmwareError
        BL       firmwareError
//  287 				return;
        B.N      ??addEvent_2
//  288 			}
//  289 			wave->pinStates[0] = initialState[i];
??addEvent_14:
        ADDS     R0,R4,R7, LSL #+2
        LDR      R0,[R0, #+2944]
        LDR      R1,[R8, #+0]
        STRB     R0,[R1, #+0]
//  290 		}
        ADDS     R7,R7,#+1
        B.N      ??addEvent_12
//  291 
//  292 		isFrontEvent[0] = TV_HIGH == stateParam;
??addEvent_13:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+1
        BNE.N    ??addEvent_15
        MOVS     R0,#+1
        B.N      ??addEvent_16
??addEvent_15:
        MOVS     R0,#+0
??addEvent_16:
        STRB     R0,[R4, #+3964]
//  293 		wave.setSwitchTime(0, angle);
        VMOV.F32 S0,S16
        MOVS     R1,#+0
        ADDW     R0,R4,#+2932
          CFI FunCall _ZN12multi_wave_s13setSwitchTimeEif
        BL       _ZN12multi_wave_s13setSwitchTimeEif
//  294 		wave.waves[waveIndex].pinStates[0] = state;
        LDRB     R0,[SP, #+16]
        LDR      R1,[R4, #+2936]
        LDR      R0,[R1, R0, LSL #+2]
        STRB     R6,[R0, #+0]
//  295 		return;
        B.N      ??addEvent_2
//  296 	}
//  297 
//  298 	int exactMatch = wave.findAngleMatch(angle, size);
??addEvent_11:
        MOVW     R0,#+5228
        LDR      R1,[R0, R4]
        VMOV.F32 S0,S16
        ADDW     R0,R4,#+2932
          CFI FunCall _ZNK12multi_wave_s14findAngleMatchEfi
        BL       _ZNK12multi_wave_s14findAngleMatchEfi
        MOVS     R7,R0
//  299 	if (exactMatch != EFI_ERROR_CODE) {
        CMN      R7,#+1
        BEQ.N    ??addEvent_17
//  300 		firmwareError("same angle: not supported");
        LDR.W    R0,??DataTable7_18
          CFI FunCall firmwareError
        BL       firmwareError
//  301 		return;
        B.N      ??addEvent_2
//  302 	}
//  303 
//  304 	int index = wave.waveIndertionAngle(angle, size);
??addEvent_17:
        MOVW     R0,#+5228
        LDR      R1,[R0, R4]
        VMOV.F32 S0,S16
        ADDW     R0,R4,#+2932
          CFI FunCall _ZNK12multi_wave_s18waveIndertionAngleEfi
        BL       _ZNK12multi_wave_s18waveIndertionAngleEfi
        MOV      R8,R0
//  305 
//  306 	// shifting existing data
//  307 	// todo: does this logic actually work? I think it does not!
//  308 	for (int i = size - 1; i >= index; i--) {
        MOVW     R0,#+5228
        LDR      R0,[R0, R4]
        SUBS     R9,R0,#+1
??addEvent_18:
        CMP      R9,R8
        BLT.N    ??addEvent_19
//  309 		for (int j = 0; j < PWM_PHASE_MAX_WAVE_PER_PWM; j++) {
        MOVS     R10,#+0
??addEvent_20:
        CMP      R10,#+3
        BGE.N    ??addEvent_21
//  310 			wave.waves[j].pinStates[i + 1] = wave.getChannelState(j, index);
        LDR      R0,[R4, #+2936]
        LDR      R0,[R0, R10, LSL #+2]
        ADDS     R11,R9,R0
        MOV      R2,R8
        MOV      R1,R10
        ADDW     R0,R4,#+2932
          CFI FunCall _ZNK12multi_wave_s15getChannelStateEii
        BL       _ZNK12multi_wave_s15getChannelStateEii
        STRB     R0,[R11, #+1]
//  311 		}
        ADDS     R10,R10,#+1
        B.N      ??addEvent_20
//  312 		wave.setSwitchTime(i + 1, wave.getSwitchTime(i));
??addEvent_21:
        MOV      R1,R9
        ADDW     R0,R4,#+2932
          CFI FunCall _ZNK12multi_wave_s13getSwitchTimeEi
        BL       _ZNK12multi_wave_s13getSwitchTimeEi
        ADDS     R1,R9,#+1
        ADDW     R0,R4,#+2932
          CFI FunCall _ZN12multi_wave_s13setSwitchTimeEif
        BL       _ZN12multi_wave_s13setSwitchTimeEif
//  313 	}
        SUBS     R9,R9,#+1
        B.N      ??addEvent_18
//  314 
//  315 	isFrontEvent[index] = TV_HIGH == stateParam;
??addEvent_19:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+1
        BNE.N    ??addEvent_22
        MOVS     R0,#+1
        B.N      ??addEvent_23
??addEvent_22:
        MOVS     R0,#+0
??addEvent_23:
        ADDS     R1,R8,R4
        STRB     R0,[R1, #+3964]
//  316 
//  317 	if (index != size) {
        MOVW     R0,#+5228
        LDR      R0,[R0, R4]
        CMP      R8,R0
        BEQ.N    ??addEvent_24
//  318 		firmwareError("are we ever here?");
        LDR.W    R0,??DataTable7_19
          CFI FunCall firmwareError
        BL       firmwareError
//  319 	}
//  320 
//  321 //	int index = size;
//  322 	size++;
??addEvent_24:
        MOVW     R0,#+5228
        LDR      R0,[R0, R4]
        ADDS     R0,R0,#+1
        MOVW     R1,#+5228
        STR      R0,[R1, R4]
//  323 
//  324 	for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++) {
        MOVS     R9,#+0
??addEvent_25:
        CMP      R9,#+3
        BGE.N    ??addEvent_26
//  325 		wave.waves[i].pinStates[index] = wave.getChannelState(i, index - 1);
        LDR      R0,[R4, #+2936]
        LDR      R10,[R0, R9, LSL #+2]
        SUBS     R2,R8,#+1
        MOV      R1,R9
        ADDW     R0,R4,#+2932
          CFI FunCall _ZNK12multi_wave_s15getChannelStateEii
        BL       _ZNK12multi_wave_s15getChannelStateEii
        STRB     R0,[R8, R10]
//  326 	}
        ADDS     R9,R9,#+1
        B.N      ??addEvent_25
//  327 	wave.setSwitchTime(index, angle);
??addEvent_26:
        VMOV.F32 S0,S16
        MOV      R1,R8
        ADDW     R0,R4,#+2932
          CFI FunCall _ZN12multi_wave_s13setSwitchTimeEif
        BL       _ZN12multi_wave_s13setSwitchTimeEif
//  328 	wave.waves[waveIndex].pinStates[index] = state;
        LDRB     R0,[SP, #+16]
        LDR      R1,[R4, #+2936]
        LDR      R0,[R1, R0, LSL #+2]
        STRB     R6,[R8, R0]
//  329 }
??addEvent_2:
        ADD      SP,SP,#+8
          CFI CFA R13+48
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+40
        POP      {R0,R4-R11,PC}   ;; return
          CFI EndBlock cfiBlock25

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4:
        DC32     0x43b40000
//  330 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock26 Using cfiCommon0
          CFI Function _ZNK12TriggerShape16getCycleDurationEv
          CFI NoCalls
        THUMB
//  331 int TriggerShape::getCycleDuration() const {
//  332 	return (operationMode == FOUR_STROKE_CAM_SENSOR) ? 720 : 360;
_ZNK12TriggerShape16getCycleDurationEv:
        MOVW     R1,#+7016
        LDR      R0,[R1, R0]
        CMP      R0,#+2
        BNE.N    ??getCycleDuration_0
        MOV      R0,#+720
        B.N      ??getCycleDuration_1
??getCycleDuration_0:
        MOV      R0,#+360
??getCycleDuration_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock26
//  333 }
//  334 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock27 Using cfiCommon0
          CFI Function _ZNK12TriggerShape14getSwitchAngleEi
        THUMB
//  335 float TriggerShape::getSwitchAngle(int index) const {
_ZNK12TriggerShape14getSwitchAngleEi:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        VPUSH    {D8}
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  336 	return getCycleDuration() * wave.getSwitchTime(index);
        MOVS     R0,R4
          CFI FunCall _ZNK12TriggerShape16getCycleDurationEv
        BL       _ZNK12TriggerShape16getCycleDurationEv
        VMOV     S0,R0
        VCVT.F32.S32 S16,S0
        MOVS     R1,R5
        ADDW     R0,R4,#+2932
          CFI FunCall _ZNK12multi_wave_s13getSwitchTimeEi
        BL       _ZNK12multi_wave_s13getSwitchTimeEi
        VMUL.F32 S0,S16,S0
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock27
//  337 }
//  338 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock28 Using cfiCommon0
          CFI Function _ZN12multi_wave_s16checkSwitchTimesEi
        THUMB
//  339 void multi_wave_s::checkSwitchTimes(int size) {
_ZN12multi_wave_s16checkSwitchTimesEi:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  340 	checkSwitchTimes2(size, switchTimes);
        LDR      R1,[R4, #+8]
        MOVS     R0,R5
          CFI FunCall _Z17checkSwitchTimes2iPf
        BL       _Z17checkSwitchTimes2iPf
//  341 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock28
//  342 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock29 Using cfiCommon0
          CFI Function _Z18setVwConfigurationP12TriggerShape
        THUMB
//  343 void setVwConfiguration(TriggerShape *s) {
_Z18setVwConfigurationP12TriggerShape:
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
        MOVS     R4,R0
//  344 	efiAssertVoid(s != NULL, "TriggerShape is NULL");
        CMP      R4,#+0
        BNE.N    ??setVwConfiguration_0
        LDR.W    R0,??DataTable7_20
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??setVwConfiguration_1
//  345 	operation_mode_e operationMode = FOUR_STROKE_CRANK_SENSOR;
??setVwConfiguration_0:
        MOVS     R5,#+1
//  346 
//  347 	s->useRiseEdge = true;
        MOVS     R0,#+1
        STRB     R0,[R4, #+2916]
//  348 
//  349 	initializeSkippedToothTriggerShapeExt(s, 60, 2,
//  350 			operationMode);
        MOVS     R3,R5
        MOVS     R2,#+2
        MOVS     R1,#+60
        MOVS     R0,R4
          CFI FunCall _Z37initializeSkippedToothTriggerShapeExtP12TriggerShapeii16operation_mode_e
        BL       _Z37initializeSkippedToothTriggerShapeExtP12TriggerShapeii16operation_mode_e
//  351 
//  352 	s->isSynchronizationNeeded = true;
        MOVS     R0,#+1
        STRB     R0,[R4, #+0]
//  353 
//  354 	s->reset(operationMode, false);
        MOVS     R2,#+0
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape5resetE16operation_mode_eb
        BL       _ZN12TriggerShape5resetE16operation_mode_eb
//  355 
//  356 	int totalTeethCount = 60;
        MOVS     R6,#+60
//  357 	int skippedCount = 2;
        MOVS     R7,#+2
//  358 
//  359 	float engineCycle = getEngineCycle(operationMode);
        MOVS     R0,R5
          CFI FunCall _Z14getEngineCycle16operation_mode_e
        BL       _Z14getEngineCycle16operation_mode_e
        VMOV.F32 S16,S0
//  360 	float toothWidth = 0.5;
        VMOV.F32 S17,#0.5
//  361 
//  362 	addSkippedToothTriggerEvents(T_PRIMARY, s, 60, 2, toothWidth, 0, engineCycle,
//  363 			NO_LEFT_FILTER, 690);
        VLDR.W   S4,??DataTable5  ;; 0x442c8000
        VMOV.F32 S3,#-1.0
        VMOV.F32 S2,S16
        VLDR.W   S1,??DataTable5_1  ;; 0x0
        VMOV.F32 S0,S17
        MOVS     R3,#+2
        MOVS     R2,#+60
        MOVS     R1,R4
        MOVS     R0,#+0
          CFI FunCall _Z28addSkippedToothTriggerEvents15trigger_wheel_eP12TriggerShapeiifffff
        BL       _Z28addSkippedToothTriggerEvents15trigger_wheel_eP12TriggerShapeiifffff
//  364 
//  365 	float angleDown = engineCycle / totalTeethCount * (totalTeethCount - skippedCount - 1 + (1 - toothWidth) );
        VMOV     S0,R6
        VCVT.F32.S32 S0,S0
        VDIV.F32 S0,S16,S0
        SUBS     R0,R6,R7
        SUBS     R0,R0,#+1
        VMOV     S1,R0
        VCVT.F32.S32 S1,S1
        VMOV.F32 S2,#1.0
        VSUB.F32 S2,S2,S17
        VADD.F32 S1,S1,S2
        VMUL.F32 S18,S0,S1
//  366 	s->addEvent(0 + angleDown + 12, T_PRIMARY, TV_HIGH, NO_LEFT_FILTER, NO_RIGHT_FILTER);
        VLDR.W   S2,??DataTable5_2  ;; 0x447a0000
        VMOV.F32 S1,#-1.0
        MOVS     R2,#+1
        MOVS     R1,#+0
        VMOV.F32 S0,#12.0
        VADD.F32 S0,S18,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_eff
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_eff
//  367 	s->addEvent(0 + engineCycle, T_PRIMARY, TV_LOW, NO_LEFT_FILTER, NO_RIGHT_FILTER);
        VLDR.W   S2,??DataTable5_2  ;; 0x447a0000
        VMOV.F32 S1,#-1.0
        MOVS     R2,#+0
        MOVS     R1,#+0
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_eff
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_eff
//  368 
//  369 	s->setTriggerSynchronizationGap2(1.6, 4);
        VMOV.F32 S1,#4.0
        VLDR.W   S0,??DataTable6  ;; 0x3fcccccd
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape29setTriggerSynchronizationGap2Eff
        BL       _ZN12TriggerShape29setTriggerSynchronizationGap2Eff
//  370 }
??setVwConfiguration_1:
        VPOP     {D8-D9}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI CFA R13+24
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock29

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5:
        DC32     0x442c8000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_1:
        DC32     0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_2:
        DC32     0x447a0000
//  371 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock30 Using cfiCommon0
          CFI Function _Z28setToothedWheelConfigurationP12TriggerShapeii16operation_mode_e
        THUMB
//  372 void setToothedWheelConfiguration(TriggerShape *s, int total, int skipped,
//  373 		operation_mode_e operationMode) {
_Z28setToothedWheelConfigurationP12TriggerShapeii16operation_mode_e:
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
//  374 #if EFI_ENGINE_CONTROL
//  375 
//  376 	s->useRiseEdge = true;
        MOVS     R0,#+1
        STRB     R0,[R4, #+2916]
//  377 
//  378 	initializeSkippedToothTriggerShapeExt(s, total, skipped,
//  379 			operationMode);
        MOVS     R3,R7
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z37initializeSkippedToothTriggerShapeExtP12TriggerShapeii16operation_mode_e
        BL       _Z37initializeSkippedToothTriggerShapeExtP12TriggerShapeii16operation_mode_e
//  380 #endif
//  381 }
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock30
//  382 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock31 Using cfiCommon0
          CFI Function _ZN12TriggerShape29setTriggerSynchronizationGap2Eff
          CFI NoCalls
        THUMB
//  383 void TriggerShape::setTriggerSynchronizationGap2(float syncRatioFrom, float syncRatioTo) {
//  384 	isSynchronizationNeeded = true;
_ZN12TriggerShape29setTriggerSynchronizationGap2Eff:
        MOVS     R1,#+1
        STRB     R1,[R0, #+0]
//  385 	this->syncRatioFrom = syncRatioFrom;
        ADDW     R1,R0,#+2904
        VSTR     S0,[R1, #0]
//  386 	this->syncRatioTo = syncRatioTo;
        ADDW     R1,R0,#+2908
        VSTR     S1,[R1, #0]
//  387 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock31
//  388 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock32 Using cfiCommon0
          CFI Function _ZN12TriggerShape28setTriggerSynchronizationGapEf
        THUMB
//  389 void TriggerShape::setTriggerSynchronizationGap(float synchRatio) {
_ZN12TriggerShape28setTriggerSynchronizationGapEf:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        VMOV.F32 S16,S0
//  390 	setTriggerSynchronizationGap2(synchRatio * 0.75f, synchRatio * 1.25f);
        VMOV.F32 S0,#1.25
        VMUL.F32 S1,S16,S0
        VMOV.F32 S0,#0.75
        VMUL.F32 S0,S16,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape29setTriggerSynchronizationGap2Eff
        BL       _ZN12TriggerShape29setTriggerSynchronizationGap2Eff
//  391 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock32
//  392 
//  393 #define S24 (720.0f / 24 / 2)
//  394 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock33 Using cfiCommon0
          CFI Function _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        THUMB
//  395 static float addAccordPair(TriggerShape *s, float sb, trigger_wheel_e const waveIndex) {
_Z13addAccordPairP12TriggerShapef15trigger_wheel_e:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        VPUSH    {D8}
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        VMOV.F32 S16,S0
        MOVS     R5,R1
//  396 	s->addEvent(sb, waveIndex, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  397 	sb += S24;
        VMOV.F32 S0,#15.0
        VADD.F32 S16,S16,S0
//  398 	s->addEvent(sb, waveIndex, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  399 	sb += S24;
        VMOV.F32 S0,#15.0
        VADD.F32 S16,S16,S0
//  400 
//  401 	return sb;
        VMOV.F32 S0,S16
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock33
//  402 }
//  403 
//  404 #define DIP 7.5f

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock34 Using cfiCommon0
          CFI Function _Z14addAccordPair3P12TriggerShapef
        THUMB
//  405 static float addAccordPair3(TriggerShape *s, float sb) {
_Z14addAccordPair3P12TriggerShapef:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        VMOV.F32 S16,S0
//  406 	sb += DIP;
        VMOV.F32 S0,#7.5
        VADD.F32 S16,S16,S0
//  407 	s->addEvent(sb, T_CHANNEL_3, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+2
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  408 	sb += DIP;
        VMOV.F32 S0,#7.5
        VADD.F32 S16,S16,S0
//  409 	s->addEvent(sb, T_CHANNEL_3, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+2
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  410 	sb += 2 * DIP;
        VMOV.F32 S0,#15.0
        VADD.F32 S16,S16,S0
//  411 	return sb;
        VMOV.F32 S0,S16
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock34
//  412 }
//  413 
//  414 /**
//  415  * Thank you Dip!
//  416  * http://forum.pgmfi.org/viewtopic.php?f=2&t=15570start=210#p139007
//  417  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock35 Using cfiCommon0
          CFI Function _Z25configureHondaAccordCDDipP12TriggerShape
        THUMB
//  418 void configureHondaAccordCDDip(TriggerShape *s) {
_Z25configureHondaAccordCDDipP12TriggerShape:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  419 	s->reset(FOUR_STROKE_CAM_SENSOR, true);
        MOVS     R2,#+1
        MOVS     R1,#+2
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape5resetE16operation_mode_eb
        BL       _ZN12TriggerShape5resetE16operation_mode_eb
//  420 
//  421 	s->initialState[T_SECONDARY] = TV_HIGH;
        MOVS     R0,#+1
        STR      R0,[R4, #+2948]
//  422 	float sb = 0;
        VLDR.W   S16,??DataTable6_1  ;; 0x0
//  423 	sb = addAccordPair3(s, sb);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z14addAccordPair3P12TriggerShapef
        BL       _Z14addAccordPair3P12TriggerShapef
        VMOV.F32 S16,S0
//  424 	sb = addAccordPair3(s, sb);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z14addAccordPair3P12TriggerShapef
        BL       _Z14addAccordPair3P12TriggerShapef
        VMOV.F32 S16,S0
//  425 	sb = addAccordPair3(s, sb);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z14addAccordPair3P12TriggerShapef
        BL       _Z14addAccordPair3P12TriggerShapef
        VMOV.F32 S16,S0
//  426 
//  427 	s->addEvent(90, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        VLDR.W   S0,??DataTable6_2  ;; 0x42b40000
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  428 	sb = 90;
        VLDR.W   S0,??DataTable6_2  ;; 0x42b40000
        VMOV.F32 S16,S0
//  429 	sb = addAccordPair3(s, sb);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z14addAccordPair3P12TriggerShapef
        BL       _Z14addAccordPair3P12TriggerShapef
        VMOV.F32 S16,S0
//  430 	sb = addAccordPair3(s, sb);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z14addAccordPair3P12TriggerShapef
        BL       _Z14addAccordPair3P12TriggerShapef
        VMOV.F32 S16,S0
//  431 	sb = addAccordPair3(s, sb);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z14addAccordPair3P12TriggerShapef
        BL       _Z14addAccordPair3P12TriggerShapef
        VMOV.F32 S16,S0
//  432 
//  433 	s->addEvent(180, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        VLDR.W   S0,??DataTable6_3  ;; 0x43340000
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  434 	sb = 180;
        VLDR.W   S0,??DataTable6_3  ;; 0x43340000
        VMOV.F32 S16,S0
//  435 	sb = addAccordPair3(s, sb);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z14addAccordPair3P12TriggerShapef
        BL       _Z14addAccordPair3P12TriggerShapef
        VMOV.F32 S16,S0
//  436 	sb = addAccordPair3(s, sb);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z14addAccordPair3P12TriggerShapef
        BL       _Z14addAccordPair3P12TriggerShapef
        VMOV.F32 S16,S0
//  437 	sb = addAccordPair3(s, sb);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z14addAccordPair3P12TriggerShapef
        BL       _Z14addAccordPair3P12TriggerShapef
        VMOV.F32 S16,S0
//  438 
//  439 	s->addEvent(270, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        VLDR.W   S0,??DataTable6_4  ;; 0x43870000
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  440 	sb = 270;
        VLDR.W   S0,??DataTable6_4  ;; 0x43870000
        VMOV.F32 S16,S0
//  441 	sb = addAccordPair3(s, sb);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z14addAccordPair3P12TriggerShapef
        BL       _Z14addAccordPair3P12TriggerShapef
        VMOV.F32 S16,S0
//  442 	sb = addAccordPair3(s, sb);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z14addAccordPair3P12TriggerShapef
        BL       _Z14addAccordPair3P12TriggerShapef
        VMOV.F32 S16,S0
//  443 	sb = addAccordPair3(s, sb);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z14addAccordPair3P12TriggerShapef
        BL       _Z14addAccordPair3P12TriggerShapef
        VMOV.F32 S16,S0
//  444 
//  445 
//  446 	s->addEvent(360.0f - DIP, T_PRIMARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable6_5  ;; 0x43b04000
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  447 	s->addEvent(360, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        VLDR.W   S0,??DataTable6_6  ;; 0x43b40000
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  448 	sb = 360;
        VLDR.W   S0,??DataTable6_6  ;; 0x43b40000
        VMOV.F32 S16,S0
//  449 	sb = addAccordPair3(s, sb);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z14addAccordPair3P12TriggerShapef
        BL       _Z14addAccordPair3P12TriggerShapef
        VMOV.F32 S16,S0
//  450 	sb = addAccordPair3(s, sb);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z14addAccordPair3P12TriggerShapef
        BL       _Z14addAccordPair3P12TriggerShapef
        VMOV.F32 S16,S0
//  451 	sb = addAccordPair3(s, sb);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z14addAccordPair3P12TriggerShapef
        BL       _Z14addAccordPair3P12TriggerShapef
        VMOV.F32 S16,S0
//  452 
//  453 	s->addEvent(450, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        VLDR.W   S0,??DataTable7  ;; 0x43e10000
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  454 	sb = 450;
        VLDR.W   S0,??DataTable7  ;; 0x43e10000
        VMOV.F32 S16,S0
//  455 	sb = addAccordPair3(s, sb);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z14addAccordPair3P12TriggerShapef
        BL       _Z14addAccordPair3P12TriggerShapef
        VMOV.F32 S16,S0
//  456 	sb = addAccordPair3(s, sb);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z14addAccordPair3P12TriggerShapef
        BL       _Z14addAccordPair3P12TriggerShapef
        VMOV.F32 S16,S0
//  457 	sb = addAccordPair3(s, sb);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z14addAccordPair3P12TriggerShapef
        BL       _Z14addAccordPair3P12TriggerShapef
        VMOV.F32 S16,S0
//  458 
//  459 	s->addEvent(540, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        VLDR.W   S0,??DataTable7_2  ;; 0x44070000
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  460 	sb = 540;
        VLDR.W   S0,??DataTable7_2  ;; 0x44070000
        VMOV.F32 S16,S0
//  461 	sb = addAccordPair3(s, sb);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z14addAccordPair3P12TriggerShapef
        BL       _Z14addAccordPair3P12TriggerShapef
        VMOV.F32 S16,S0
//  462 	sb = addAccordPair3(s, sb);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z14addAccordPair3P12TriggerShapef
        BL       _Z14addAccordPair3P12TriggerShapef
        VMOV.F32 S16,S0
//  463 	sb = addAccordPair3(s, sb);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z14addAccordPair3P12TriggerShapef
        BL       _Z14addAccordPair3P12TriggerShapef
        VMOV.F32 S16,S0
//  464 
//  465 	s->addEvent(630, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        VLDR.W   S0,??DataTable7_3  ;; 0x441d8000
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  466 	sb = 630;
        VLDR.W   S0,??DataTable7_3  ;; 0x441d8000
        VMOV.F32 S16,S0
//  467 	sb = addAccordPair3(s, sb);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z14addAccordPair3P12TriggerShapef
        BL       _Z14addAccordPair3P12TriggerShapef
        VMOV.F32 S16,S0
//  468 	sb = addAccordPair3(s, sb);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z14addAccordPair3P12TriggerShapef
        BL       _Z14addAccordPair3P12TriggerShapef
        VMOV.F32 S16,S0
//  469 	sb = addAccordPair3(s, sb);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z14addAccordPair3P12TriggerShapef
        BL       _Z14addAccordPair3P12TriggerShapef
        VMOV.F32 S16,S0
//  470 
//  471 	s->addEvent(720.0f - DIP, T_PRIMARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable7_4  ;; 0x44322000
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  472 
//  473 //	s->addEvent(720.0f - 12 * sb, T_SECONDARY, TV_LOW);
//  474 //	s->addEvent(720.0f, T_SECONDARY, TV_LOW);
//  475 
//  476 	s->addEvent(720.0f, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        VLDR.W   S0,??DataTable7_5  ;; 0x44340000
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  477 
//  478 	s->isSynchronizationNeeded = false;
        MOVS     R0,#+0
        STRB     R0,[R4, #+0]
//  479 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock35

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6:
        DC32     0x3fcccccd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_1:
        DC32     0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_2:
        DC32     0x42b40000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_3:
        DC32     0x43340000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_4:
        DC32     0x43870000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_5:
        DC32     0x43b04000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_6:
        DC32     0x43b40000

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock36 Using cfiCommon0
          CFI Function _Z22configureHondaAccordCDP12TriggerShapebb15trigger_wheel_eS1_f
        THUMB
_Z22configureHondaAccordCDP12TriggerShapebb15trigger_wheel_eS1_f:
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
        MOVS     R5,R0
        MOVS     R6,R1
        MOVS     R7,R2
        MOV      R8,R3
        VMOV.F32 S16,S0
        LDR      R4,[SP, #+48]
        MOVS     R2,#+1
        MOVS     R1,#+2
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape5resetE16operation_mode_eb
        BL       _ZN12TriggerShape5resetE16operation_mode_eb
        VMOV.F32 S0,#5.0
        VADD.F32 S17,S16,S0
        VLDR.W   S18,??DataTable7_7  ;; 0x42057cee
        MOVS     R0,#+0
        STRB     R0,[R5, #+0]
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R5
          CFI FunCall _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        BL       _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        VMOV.F32 S17,S0
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+0
        BEQ.N    ??configureHondaAccordCD_0
        MOVS     R2,#+1
        MOV      R1,R8
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        VMOV.F32 S0,#-7.5
        VADD.F32 S0,S17,S0
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
??configureHondaAccordCD_0:
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R5
          CFI FunCall _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        BL       _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        VMOV.F32 S17,S0
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R5
          CFI FunCall _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        BL       _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        VMOV.F32 S17,S0
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+0
        BEQ.N    ??configureHondaAccordCD_1
        MOVS     R2,#+0
        MOV      R1,R8
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        VMOV.F32 S0,#-7.5
        VADD.F32 S0,S17,S0
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
??configureHondaAccordCD_1:
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R5
          CFI FunCall _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        BL       _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        VMOV.F32 S17,S0
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R5
          CFI FunCall _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        BL       _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        VMOV.F32 S17,S0
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+0
        BEQ.N    ??configureHondaAccordCD_2
        MOVS     R2,#+1
        MOVS     R1,R4
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        VLDR.W   S0,??DataTable7_10  ;; 0x43340000
        VADD.F32 S0,S16,S0
        VSUB.F32 S0,S0,S18
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
??configureHondaAccordCD_2:
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R5
          CFI FunCall _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        BL       _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        VMOV.F32 S17,S0
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+0
        BEQ.N    ??configureHondaAccordCD_3
        MOVS     R2,#+0
        MOVS     R1,R4
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        VLDR.W   S0,??DataTable7_10  ;; 0x43340000
        VADD.F32 S0,S16,S0
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
??configureHondaAccordCD_3:
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R5
          CFI FunCall _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        BL       _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        VMOV.F32 S17,S0
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R5
          CFI FunCall _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        BL       _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        VMOV.F32 S17,S0
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R5
          CFI FunCall _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        BL       _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        VMOV.F32 S17,S0
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R5
          CFI FunCall _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        BL       _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        VMOV.F32 S17,S0
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R5
          CFI FunCall _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        BL       _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        VMOV.F32 S17,S0
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+0
        BEQ.N    ??configureHondaAccordCD_4
        MOVS     R2,#+1
        MOVS     R1,R4
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        VLDR.W   S0,??DataTable7_12  ;; 0x43b40000
        VADD.F32 S0,S16,S0
        VSUB.F32 S0,S0,S18
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
??configureHondaAccordCD_4:
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R5
          CFI FunCall _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        BL       _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        VMOV.F32 S17,S0
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+0
        BEQ.N    ??configureHondaAccordCD_5
        MOVS     R2,#+0
        MOVS     R1,R4
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        VLDR.W   S0,??DataTable7_12  ;; 0x43b40000
        VADD.F32 S0,S16,S0
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
??configureHondaAccordCD_5:
        MOVS     R9,#+3
??configureHondaAccordCD_6:
        CMP      R9,#+5
        BGE.N    ??configureHondaAccordCD_7
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R5
          CFI FunCall _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        BL       _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        VMOV.F32 S17,S0
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R5
          CFI FunCall _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        BL       _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        VMOV.F32 S17,S0
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R5
          CFI FunCall _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        BL       _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        VMOV.F32 S17,S0
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R5
          CFI FunCall _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        BL       _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        VMOV.F32 S17,S0
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R5
          CFI FunCall _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        BL       _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        VMOV.F32 S17,S0
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+0
        BEQ.N    ??configureHondaAccordCD_8
        MOVS     R2,#+1
        MOVS     R1,R4
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        VMOV     S0,R9
        VCVT.F32.S32 S0,S0
        VLDR.W   S1,??DataTable7_10  ;; 0x43340000
        VMOV.F32 S2,S16
        VMLA.F32 S2,S0,S1
        VSUB.F32 S0,S2,S18
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
??configureHondaAccordCD_8:
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R5
          CFI FunCall _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        BL       _Z13addAccordPairP12TriggerShapef15trigger_wheel_e
        VMOV.F32 S17,S0
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+0
        BEQ.N    ??configureHondaAccordCD_9
        MOVS     R2,#+0
        MOVS     R1,R4
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        VMOV     S0,R9
        VCVT.F32.S32 S1,S0
        VLDR.W   S2,??DataTable7_10  ;; 0x43340000
        VMOV.F32 S0,S16
        VMLA.F32 S0,S1,S2
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
??configureHondaAccordCD_9:
        ADDS     R9,R9,#+1
        B.N      ??configureHondaAccordCD_6
??configureHondaAccordCD_7:
        VPOP     {D8-D9}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI CFA R13+32
        POP      {R0,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock36

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7:
        DC32     0x43e10000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_1:
        DC32     _ZN13single_wave_sC1Ev

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_2:
        DC32     0x44070000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_3:
        DC32     0x441d8000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_4:
        DC32     0x44322000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_5:
        DC32     0x44340000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_6:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_7:
        DC32     0x42057cee

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_8:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_9:
        DC32     persistentState

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_10:
        DC32     0x43340000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_11:
        DC32     `?<Constant "zero engineCycle">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_12:
        DC32     0x43b40000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_13:
        DC32     `?<Constant "operationMode not set">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_14:
        DC32     `?<Constant "secondary needed or not?">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_15:
        DC32     `?<Constant "angle should be positive">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_16:
        DC32     `?<Constant "invalid angle order: ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_17:
        DC32     `?<Constant "wave pinStates is NULL">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_18:
        DC32     `?<Constant "same angle: not suppo...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_19:
        DC32     `?<Constant "are we ever here?">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_20:
        DC32     `?<Constant "TriggerShape is NULL">`

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memset
          CFI Block cfiBlock37 Using cfiCommon0
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
          CFI EndBlock cfiBlock37

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TriggerShape is NULL">`:
        DATA
        DC8 "TriggerShape is NULL"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "operationMode not set">`:
        DATA
        DC8 "operationMode not set"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "secondary needed or not?">`:
        DATA
        DC8 "secondary needed or not?"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "angle should be positive">`:
        DATA
        DC8 "angle should be positive"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid angle order: ...">`:
        DATA
        DC8 "invalid angle order: %f and %f"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "wave pinStates is NULL">`:
        DATA
        DC8 "wave pinStates is NULL"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "same angle: not suppo...">`:
        DATA
        DC8 "same angle: not supported"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "are we ever here?">`:
        DATA
        DC8 "are we ever here?"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "zero engineCycle">`:
        DATA
        DC8 "zero engineCycle"
        DC8 0, 0, 0

        END
//  480 
//  481 void configureHondaAccordCD(TriggerShape *s, bool withOneEventSignal, bool withFourEventSignal,
//  482 		trigger_wheel_e const oneEventWave,
//  483 		trigger_wheel_e const fourEventWave,
//  484 		float prefix) {
//  485 	s->reset(FOUR_STROKE_CAM_SENSOR, true);
//  486 
//  487 //	trigger_wheel_e const oneEventWave = T_CHANNEL_3;
//  488 //	bool withFourEventSignal = true;
//  489 //	trigger_wheel_e const fourEventWave = T_PRIMARY;
//  490 
//  491 	float sb = 5.0f + prefix;
//  492 
//  493 	float tdcWidth = 0.1854 * 720 / 4;
//  494 
//  495 	s->isSynchronizationNeeded = false;
//  496 
//  497 	sb = addAccordPair(s, sb, T_SECONDARY);
//  498 
//  499 	if (withOneEventSignal)
//  500 		s->addEvent(sb - S24 / 2, oneEventWave, TV_HIGH);
//  501 
//  502 	sb = addAccordPair(s, sb, T_SECONDARY);
//  503 	sb = addAccordPair(s, sb, T_SECONDARY);
//  504 	if (withOneEventSignal)
//  505 		s->addEvent(sb - S24 / 2, oneEventWave, TV_LOW);
//  506 	sb = addAccordPair(s, sb, T_SECONDARY);
//  507 	sb = addAccordPair(s, sb, T_SECONDARY);
//  508 	if (withFourEventSignal) {
//  509 		s->addEvent(1 * 180.0f + prefix - tdcWidth, fourEventWave, TV_HIGH);
//  510 	}
//  511 	sb = addAccordPair(s, sb, T_SECONDARY);
//  512 	if (withFourEventSignal) {
//  513 		s->addEvent(1 * 180.0f + prefix, fourEventWave, TV_LOW);
//  514 	}
//  515 
//  516 	sb = addAccordPair(s, sb, T_SECONDARY);
//  517 	sb = addAccordPair(s, sb,T_SECONDARY);
//  518 	sb = addAccordPair(s, sb, T_SECONDARY);
//  519 	sb = addAccordPair(s, sb, T_SECONDARY);
//  520 	sb = addAccordPair(s, sb, T_SECONDARY);
//  521 
//  522 	if (withFourEventSignal) {
//  523 		s->addEvent(2 * 180.0f + prefix - tdcWidth, fourEventWave, TV_HIGH);
//  524 	}
//  525 	sb = addAccordPair(s, sb, T_SECONDARY);
//  526 	if (withFourEventSignal) {
//  527 		s->addEvent(2 * 180.0f + prefix, fourEventWave, TV_LOW);
//  528 	}
//  529 
//  530 	for (int i = 3; i <= 4; i++) {
//  531 		sb = addAccordPair(s, sb, T_SECONDARY);
//  532 		sb = addAccordPair(s, sb, T_SECONDARY);
//  533 		sb = addAccordPair(s, sb, T_SECONDARY);
//  534 		sb = addAccordPair(s, sb, T_SECONDARY);
//  535 		sb = addAccordPair(s, sb, T_SECONDARY);
//  536 
//  537 		if (withFourEventSignal) {
//  538 			s->addEvent(i * 180.0f + prefix - tdcWidth, fourEventWave, TV_HIGH);
//  539 		}
//  540 		sb = addAccordPair(s, sb, T_SECONDARY);
//  541 		if (withFourEventSignal) {
//  542 			s->addEvent(i * 180.0f + prefix, fourEventWave, TV_LOW);
//  543 		}
//  544 	}
//  545 }
// 
//   228 bytes in section .rodata
// 3 798 bytes in section .text
// 
// 3 776 bytes of CODE  memory (+ 22 bytes shared)
//   228 bytes of CONST memory
//
//Errors: none
//Warnings: 1
