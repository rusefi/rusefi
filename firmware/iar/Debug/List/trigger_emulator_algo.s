///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:54 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\tri /
//                       gger\trigger_emulator_algo.cpp                       /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\tri /
//                       gger\trigger_emulator_algo.cpp -lCN                  /
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
//                       trigger_emulator_algo.s                              /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME trigger_emulator_algo

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z13applyPinStateP9PwmConfigi
        EXTERN _Z16getRpmMultiplier16operation_mode_e
        EXTERN _Z17copyPwmParametersP9PwmConfigiPfiPPa
        EXTERN _Z19hwHandleShaftSignal15trigger_event_e
        EXTERN _Z29getGlobalConfigurationVersionv
        EXTERN _Z31applyNonPersistentConfigurationP7Logging
        EXTERN _ZN13single_wave_sC1EPa
        EXTERN _ZN18LocalVersionHolder10getVersionEv
        EXTERN _ZN18LocalVersionHolder5isOldEv
        EXTERN _ZN18LocalVersionHolderC1Ev
        EXTERN _ZN9PwmConfig13weComplexInitEPKciPfiPPaPFvPS_EPFvS5_iE
        EXTERN _ZN9PwmConfigC1EPfP13single_wave_s
        EXTERN _ZN9WaveChart14resetWaveChartEv
        EXTERN _ZNK12TriggerShape7getSizeEv
        EXTERN __aeabi_d2f
        EXTERN __aeabi_ddiv
        EXTERN __aeabi_f2d
        EXTERN __aeabi_f2lz
        EXTERN __aeabi_l2f
        EXTERN addConsoleActionIP
        EXTERN addConsoleActionP
        EXTERN currentTimeMillis
        EXTERN engine
        EXTERN engineConfiguration
        EXTERN scheduleMsg
        EXTERN waveChart

        PUBLIC _Z21setTriggerEmulatorRPMiP6Engine
        PUBLIC _Z24initTriggerEmulatorLogicP7LoggingP6Engine
        PUBLIC _ZN21TriggerEmulatorHelper22handleEmulatorCallbackEP9PwmConfigi
        PUBLIC _ZN21TriggerEmulatorHelperC1Ev
        PUBLIC _ZN21TriggerEmulatorHelperC2Ev
        PUBLIC triggerSignal
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\trigger\trigger_emulator_algo.cpp
//    1 /**
//    2  * @file trigger_emulator_algo.cpp
//    3  *
//    4  * @date Mar 3, 2014
//    5  * @author Andrey Belomutskiy, (c) 2012-2015
//    6  */
//    7 #include "main.h"
//    8 
//    9 #if EFI_EMULATE_POSITION_SENSORS || defined(__DOXYGEN__)
//   10 
//   11 #include "trigger_emulator_algo.h"
//   12 #include "engine_configuration.h"
//   13 #include "LocalVersionHolder.h"
//   14 #include "trigger_central.h"
//   15 
//   16 #if EFI_PROD_CODE
//   17 #include "pwm_generator.h"
//   18 #endif
//   19 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _ZN21TriggerEmulatorHelperC2Ev
        THUMB
// __code __interwork __softfp TriggerEmulatorHelper::subobject TriggerEmulatorHelper()
_ZN21TriggerEmulatorHelperC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN21TriggerEmulatorHelperC1Ev
        BL       _ZN21TriggerEmulatorHelperC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.data`:DATA:REORDER:NOROOT(2)
isUpEvent:
        DATA
        DC8 0, 1, 0, 1, 0, 1, 0, 0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
pinStates1:
        DS8 252

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
pinStates2:
        DS8 252

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
pinStates3:
        DS8 252

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        LDR.W    R1,??DataTable7_2
        LDR.W    R0,??DataTable7_3
          CFI FunCall _ZN13single_wave_sC1EPa
        BL       _ZN13single_wave_sC1EPa
        LDR.W    R1,??DataTable7_4
        LDR.W    R0,??DataTable7_5
          CFI FunCall _ZN13single_wave_sC1EPa
        BL       _ZN13single_wave_sC1EPa
        LDR.W    R1,??DataTable7_6
        LDR.W    R0,??DataTable7_7
          CFI FunCall _ZN13single_wave_sC1EPa
        BL       _ZN13single_wave_sC1EPa
        LDR.W    R0,??DataTable7_8
        LDR.N    R1,??DataTable7_3
        LDR      R1,[R1, #+0]
        STR      R1,[R0, #+0]
        LDR.W    R0,??DataTable7_8
        LDR.N    R1,??DataTable7_3
        LDR      R1,[R1, #+4]
        STR      R1,[R0, #+4]
        LDR.N    R0,??DataTable7_8
        LDR.N    R1,??DataTable7_3
        LDR      R1,[R1, #+8]
        STR      R1,[R0, #+8]
        LDR.N    R2,??DataTable7_8
        LDR.N    R1,??DataTable7_9
        LDR.N    R0,??DataTable7_10
          CFI FunCall _ZN9PwmConfigC1EPfP13single_wave_s
        BL       _ZN9PwmConfigC1EPfP13single_wave_s
        LDR.N    R0,??DataTable7_11
          CFI FunCall _ZN18LocalVersionHolderC1Ev
        BL       _ZN18LocalVersionHolderC1Ev
        LDR.N    R0,??DataTable7_12
          CFI FunCall _ZN21TriggerEmulatorHelperC1Ev
        BL       _ZN21TriggerEmulatorHelperC1Ev
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock1

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _ZN21TriggerEmulatorHelperC1Ev
          CFI NoCalls
        THUMB
//   20 TriggerEmulatorHelper::TriggerEmulatorHelper() {
//   21 	primaryWheelState = false;
_ZN21TriggerEmulatorHelperC1Ev:
        MOVS     R1,#+0
        STRB     R1,[R0, #+0]
//   22 	secondaryWheelState = false;
        MOVS     R1,#+0
        STRB     R1,[R0, #+1]
//   23 	thirdWheelState = false;
        MOVS     R1,#+0
        STRB     R1,[R0, #+2]
//   24 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock2
//   25 
//   26 // this is not the only place where we have 'isUpEvent'. todo: reuse
//   27 static bool_t isUpEvent[6] = { false, true, false, true, false, true };
//   28 
//   29 EXTERN_ENGINE
//   30 ;
//   31 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z15fireShaftSignal15trigger_event_e
        THUMB
//   32 static void fireShaftSignal(trigger_event_e signal) {
_Z15fireShaftSignal15trigger_event_e:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   33 	if (!engineConfiguration->useOnlyFrontForTrigger || isUpEvent[(int) signal])
        LDR.N    R0,??DataTable7_13
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+6
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??fireShaftSignal_0
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        LDR.N    R0,??DataTable7_14
        LDRB     R0,[R4, R0]
        CMP      R0,#+0
        BEQ.N    ??fireShaftSignal_1
//   34 		hwHandleShaftSignal(signal);
??fireShaftSignal_0:
        MOVS     R0,R4
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z19hwHandleShaftSignal15trigger_event_e
        BL       _Z19hwHandleShaftSignal15trigger_event_e
//   35 }
??fireShaftSignal_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//   36 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _ZN21TriggerEmulatorHelper22handleEmulatorCallbackEP9PwmConfigi
        THUMB
//   37 void TriggerEmulatorHelper::handleEmulatorCallback(PwmConfig *state, int stateIndex) {
_ZN21TriggerEmulatorHelper22handleEmulatorCallbackEP9PwmConfigi:
        PUSH     {R0,R4-R11,LR}
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
        SUB      SP,SP,#+8
          CFI CFA R13+48
        MOVS     R4,R1
        MOVS     R5,R2
//   38 	int prevIndex = (stateIndex + state->phaseCount - 1) % state->phaseCount;
        LDR      R0,[R4, #+80]
        ADDS     R0,R0,R5
        SUBS     R0,R0,#+1
        LDR      R1,[R4, #+80]
        SDIV     R2,R0,R1
        MLS      R6,R1,R2,R0
//   39 
//   40 	bool_t primaryWheelState = state->multiWave.waves[0].pinStates[prevIndex];
        LDR      R0,[R4, #+16]
        LDR      R0,[R0, #+0]
        LDRSB    R0,[R6, R0]
        CMP      R0,#+0
        BEQ.N    ??handleEmulatorCallback_0
        MOVS     R9,#+1
        B.N      ??handleEmulatorCallback_1
??handleEmulatorCallback_0:
        MOVS     R9,#+0
//   41 	int newPrimaryWheelState = state->multiWave.waves[0].pinStates[stateIndex];
??handleEmulatorCallback_1:
        LDR      R0,[R4, #+16]
        LDR      R0,[R0, #+0]
        LDRSB    R0,[R5, R0]
        STR      R0,[SP, #+0]
//   42 
//   43 	bool_t secondaryWheelState = state->multiWave.waves[1].pinStates[prevIndex];
        LDR      R0,[R4, #+16]
        LDR      R0,[R0, #+4]
        LDRSB    R0,[R6, R0]
        CMP      R0,#+0
        BEQ.N    ??handleEmulatorCallback_2
        MOVS     R10,#+1
        B.N      ??handleEmulatorCallback_3
??handleEmulatorCallback_2:
        MOVS     R10,#+0
//   44 	int newSecondaryWheelState = state->multiWave.waves[1].pinStates[stateIndex];
??handleEmulatorCallback_3:
        LDR      R0,[R4, #+16]
        LDR      R0,[R0, #+4]
        LDRSB    R7,[R5, R0]
//   45 
//   46 	bool_t thirdWheelState = state->multiWave.waves[2].pinStates[prevIndex];
        LDR      R0,[R4, #+16]
        LDR      R0,[R0, #+8]
        LDRSB    R0,[R6, R0]
        CMP      R0,#+0
        BEQ.N    ??handleEmulatorCallback_4
        MOVS     R11,#+1
        B.N      ??handleEmulatorCallback_5
??handleEmulatorCallback_4:
        MOVS     R11,#+0
//   47 	int new3rdWheelState = state->multiWave.waves[2].pinStates[stateIndex];
??handleEmulatorCallback_5:
        LDR      R0,[R4, #+16]
        LDR      R0,[R0, #+8]
        LDRSB    R8,[R5, R0]
//   48 
//   49 	if (primaryWheelState != newPrimaryWheelState) {
        UXTB     R9,R9            ;; ZeroExt  R9,R9,#+24,#+24
        LDR      R0,[SP, #+0]
        CMP      R9,R0
        BEQ.N    ??handleEmulatorCallback_6
//   50 		primaryWheelState = newPrimaryWheelState;
        LDR      R0,[SP, #+0]
        CMP      R0,#+0
        BEQ.N    ??handleEmulatorCallback_7
        MOVS     R9,#+1
        B.N      ??handleEmulatorCallback_8
??handleEmulatorCallback_7:
        MOVS     R9,#+0
//   51 		fireShaftSignal(primaryWheelState ? SHAFT_PRIMARY_UP : SHAFT_PRIMARY_DOWN);
??handleEmulatorCallback_8:
        MOV      R0,R9
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z15fireShaftSignal15trigger_event_e
        BL       _Z15fireShaftSignal15trigger_event_e
//   52 	}
//   53 
//   54 	if (secondaryWheelState != newSecondaryWheelState) {
??handleEmulatorCallback_6:
        UXTB     R10,R10          ;; ZeroExt  R10,R10,#+24,#+24
        CMP      R10,R7
        BEQ.N    ??handleEmulatorCallback_9
//   55 		secondaryWheelState = newSecondaryWheelState;
        CMP      R7,#+0
        BEQ.N    ??handleEmulatorCallback_10
        MOVS     R10,#+1
        B.N      ??handleEmulatorCallback_11
??handleEmulatorCallback_10:
        MOVS     R10,#+0
//   56 		fireShaftSignal(secondaryWheelState ? SHAFT_SECONDARY_UP : SHAFT_SECONDARY_DOWN);
??handleEmulatorCallback_11:
        UXTB     R10,R10          ;; ZeroExt  R10,R10,#+24,#+24
        CMP      R10,#+0
        BEQ.N    ??handleEmulatorCallback_12
        MOVS     R0,#+3
        B.N      ??handleEmulatorCallback_13
??handleEmulatorCallback_12:
        MOVS     R0,#+2
??handleEmulatorCallback_13:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z15fireShaftSignal15trigger_event_e
        BL       _Z15fireShaftSignal15trigger_event_e
//   57 	}
//   58 
//   59 	if (thirdWheelState != new3rdWheelState) {
??handleEmulatorCallback_9:
        UXTB     R11,R11          ;; ZeroExt  R11,R11,#+24,#+24
        CMP      R11,R8
        BEQ.N    ??handleEmulatorCallback_14
//   60 		thirdWheelState = new3rdWheelState;
        CMP      R8,#+0
        BEQ.N    ??handleEmulatorCallback_15
        MOVS     R11,#+1
        B.N      ??handleEmulatorCallback_16
??handleEmulatorCallback_15:
        MOVS     R11,#+0
//   61 		fireShaftSignal(thirdWheelState ? SHAFT_3RD_UP : SHAFT_3RD_DOWN);
??handleEmulatorCallback_16:
        UXTB     R11,R11          ;; ZeroExt  R11,R11,#+24,#+24
        CMP      R11,#+0
        BEQ.N    ??handleEmulatorCallback_17
        MOVS     R0,#+5
        B.N      ??handleEmulatorCallback_18
??handleEmulatorCallback_17:
        MOVS     R0,#+4
??handleEmulatorCallback_18:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z15fireShaftSignal15trigger_event_e
        BL       _Z15fireShaftSignal15trigger_event_e
//   62 	}
//   63 
//   64 	//	print("hello %d\r\n", chTimeNow());
//   65 }
??handleEmulatorCallback_14:
        POP      {R0-R2,R4-R11,PC}  ;; return
          CFI EndBlock cfiBlock4
//   66 
//   67 /*
//   68  * todo: should we simply re-use instances used by trigger_decoder?
//   69  * todo: since we are emulating same shape we are decoding
//   70  */
//   71 static pin_state_t pinStates1[PWM_PHASE_MAX_COUNT];
//   72 static pin_state_t pinStates2[PWM_PHASE_MAX_COUNT];
//   73 static pin_state_t pinStates3[PWM_PHASE_MAX_COUNT];

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   74 static single_wave_s waves[PWM_PHASE_MAX_WAVE_PER_PWM] = { single_wave_s(pinStates1), single_wave_s(pinStates2),
waves:
        DS8 12
//   75 		single_wave_s(pinStates3) };

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   76 static single_wave_s sr[PWM_PHASE_MAX_WAVE_PER_PWM] = { waves[0], waves[1], waves[2] };
sr:
        DS8 12
//   77 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   78 static float swtchTms[PWM_PHASE_MAX_COUNT];
swtchTms:
        DS8 1008
//   79 

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   80 PwmConfig triggerSignal(swtchTms, sr);
triggerSignal:
        DS8 96
//   81 
//   82 #define DO_NOT_STOP 999999999
//   83 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   84 static int stopEmulationAtIndex = DO_NOT_STOP;
stopEmulationAtIndex:
        DATA
        DC32 999999999

        SECTION `.data`:DATA:REORDER:NOROOT(0)
//   85 static bool isEmulating = true;
isEmulating:
        DATA
        DC8 1
//   86 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   87 static Logging *logger;
logger:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   88 static LocalVersionHolder emulatorConfigVersion;
emulatorConfigVersion:
        DS8 4
//   89 
//   90 #if EFI_WAVE_CHART
//   91 #include "wave_chart.h"
//   92 extern WaveChart waveChart;
//   93 #endif /* EFI_WAVE_CHART */
//   94 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z21setTriggerEmulatorRPMiP6Engine
        THUMB
//   95 void setTriggerEmulatorRPM(int rpm, Engine *engine) {
_Z21setTriggerEmulatorRPMiP6Engine:
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
//   96 	engineConfiguration->bc.triggerSimulatorFrequency = rpm;
        LDR.N    R0,??DataTable7_13
        LDR      R0,[R0, #+0]
        STR      R4,[R0, #+832]
//   97 	/**
//   98 	 * All we need to do here is to change the periodMs
//   99 	 * togglePwmState() would see that the periodMs has changed and act accordingly
//  100 	 */
//  101 	if (rpm == 0) {
        CMP      R4,#+0
        BNE.N    ??setTriggerEmulatorRPM_0
//  102 		triggerSignal.periodNt = NAN;
        LDR.N    R0,??DataTable7_10
        MVNS     R1,#-2147483648
        STR      R1,[R0, #+24]
        B.N      ??setTriggerEmulatorRPM_1
//  103 	} else {
//  104 		float rpmM = getRpmMultiplier(engineConfiguration->operationMode);
??setTriggerEmulatorRPM_0:
        LDR.N    R0,??DataTable7_13
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+500]
          CFI FunCall _Z16getRpmMultiplier16operation_mode_e
        BL       _Z16getRpmMultiplier16operation_mode_e
        VMOV.F32 S16,S0
//  105 		float gRpm = rpm * rpmM / 60.0; // per minute converted to per second
        VMOV     S0,R4
        VCVT.F32.S32 S0,S0
        VMUL.F32 S0,S0,S16
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,#+0
        LDR.N    R3,??DataTable7_15  ;; 0x404e0000
          CFI FunCall __aeabi_ddiv
        BL       __aeabi_ddiv
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        VMOV     S0,R0
//  106 		triggerSignal.periodNt = US2NT(frequency2periodUs(gRpm));
        VLDR.W   S1,??DataTable7  ;; 0x49742400
        VDIV.F32 S0,S1,S0
        VMOV     R0,S0
          CFI FunCall __aeabi_f2lz
        BL       __aeabi_f2lz
        MOVS     R6,#+168
        MOVS     R7,#+0
        UMULL    R2,R3,R6,R0
        MLA      R3,R6,R1,R3
        MLA      R3,R7,R0,R3
        MOVS     R0,R2
        MOVS     R1,R3
          CFI FunCall __aeabi_l2f
        BL       __aeabi_l2f
        LDR.N    R1,??DataTable7_10
        STR      R0,[R1, #+24]
//  107 	}
//  108 #if EFI_WAVE_CHART
//  109 	if (engine->isTestMode)
??setTriggerEmulatorRPM_1:
        LDRB     R0,[R5, #+640]
        CMP      R0,#+0
        BEQ.N    ??setTriggerEmulatorRPM_2
//  110 		waveChart.resetWaveChart();
        LDR.N    R0,??DataTable7_16
          CFI FunCall _ZN9WaveChart14resetWaveChartEv
        BL       _ZN9WaveChart14resetWaveChartEv
//  111 #endif /* EFI_WAVE_CHART */
//  112 
//  113 	scheduleMsg(logger, "Emulating position sensor(s). RPM=%d", rpm);
??setTriggerEmulatorRPM_2:
        MOVS     R2,R4
        LDR.N    R1,??DataTable7_17
        LDR.N    R0,??DataTable7_18
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  114 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+24
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock5
//  115 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z26updateTriggerShapeIfNeededP9PwmConfig
        THUMB
//  116 static void updateTriggerShapeIfNeeded(PwmConfig *state) {
_Z26updateTriggerShapeIfNeededP9PwmConfig:
        PUSH     {R0-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+32
        MOVS     R4,R0
//  117 	if (emulatorConfigVersion.isOld()) {
        LDR.N    R0,??DataTable7_11
          CFI FunCall _ZN18LocalVersionHolder5isOldEv
        BL       _ZN18LocalVersionHolder5isOldEv
        CMP      R0,#+0
        BEQ.N    ??updateTriggerShapeIfNeeded_0
//  118 		scheduleMsg(logger, "Stimulator: updating trigger shape: %d/%d %d", emulatorConfigVersion.getVersion(),
//  119 				getGlobalConfigurationVersion(), currentTimeMillis());
          CFI FunCall currentTimeMillis
        BL       currentTimeMillis
        MOVS     R5,R0
          CFI FunCall _Z29getGlobalConfigurationVersionv
        BL       _Z29getGlobalConfigurationVersionv
        MOVS     R6,R0
        LDR.N    R0,??DataTable7_11
          CFI FunCall _ZN18LocalVersionHolder10getVersionEv
        BL       _ZN18LocalVersionHolder10getVersionEv
        STR      R5,[SP, #+0]
        MOVS     R3,R6
        MOVS     R2,R0
        LDR.N    R1,??DataTable7_19
        LDR.N    R0,??DataTable7_18
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  120 
//  121 		applyNonPersistentConfiguration(logger PASS_ENGINE_PARAMETER);
        LDR.N    R0,??DataTable7_18
        LDR      R0,[R0, #+0]
          CFI FunCall _Z31applyNonPersistentConfigurationP7Logging
        BL       _Z31applyNonPersistentConfigurationP7Logging
//  122 
//  123 		TriggerShape *s = &engine->triggerShape;
        LDR.N    R0,??DataTable7_20
        LDR      R0,[R0, #+0]
        ADDS     R5,R0,#+644
//  124 		pin_state_t *pinStates[PWM_PHASE_MAX_WAVE_PER_PWM] = { s->wave.waves[0].pinStates, s->wave.waves[1].pinStates,
        LDR      R0,[R5, #+2936]
        LDR      R0,[R0, #+0]
        STR      R0,[SP, #+4]
        LDR      R0,[R5, #+2936]
        LDR      R0,[R0, #+4]
        STR      R0,[SP, #+8]
        LDR      R0,[R5, #+2936]
        LDR      R0,[R0, #+8]
        STR      R0,[SP, #+12]
//  125 				s->wave.waves[2].pinStates };
//  126 		copyPwmParameters(state, s->getSize(), s->wave.switchTimes, PWM_PHASE_MAX_WAVE_PER_PWM, pinStates);
        MOVS     R0,R5
          CFI FunCall _ZNK12TriggerShape7getSizeEv
        BL       _ZNK12TriggerShape7getSizeEv
        MOVS     R1,R0
        ADD      R0,SP,#+4
        STR      R0,[SP, #+0]
        MOVS     R3,#+3
        LDR      R2,[R5, #+2940]
        MOVS     R0,R4
          CFI FunCall _Z17copyPwmParametersP9PwmConfigiPfiPPa
        BL       _Z17copyPwmParametersP9PwmConfigiPfiPPa
//  127 		state->safe.periodNt = -1; // this would cause loop re-initialization
        LDR.N    R0,??DataTable7_21  ;; 0xbf800000
        STR      R0,[R4, #+56]
//  128 	}
//  129 }
??updateTriggerShapeIfNeeded_0:
        POP      {R0-R6,PC}       ;; return
          CFI EndBlock cfiBlock6
//  130 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  131 static TriggerEmulatorHelper helper;
helper:
        DS8 4
//  132 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z21emulatorApplyPinStateP9PwmConfigi
        THUMB
//  133 static void emulatorApplyPinState(PwmConfig *state, int stateIndex) {
_Z21emulatorApplyPinStateP9PwmConfigi:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  134 	if (stopEmulationAtIndex == stateIndex) {
        LDR.N    R0,??DataTable7_22
        LDR      R0,[R0, #+0]
        CMP      R0,R5
        BNE.N    ??emulatorApplyPinState_0
//  135 		isEmulating = false;
        LDR.N    R0,??DataTable7_23
        MOVS     R1,#+0
        STRB     R1,[R0, #+0]
//  136 	}
//  137 	if (!isEmulating) {
??emulatorApplyPinState_0:
        LDR.N    R0,??DataTable7_23
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??emulatorApplyPinState_1
//  138 		return;
//  139 	}
//  140 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//  141 	applyPinState(state, stateIndex);
??emulatorApplyPinState_2:
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z13applyPinStateP9PwmConfigi
        BL       _Z13applyPinStateP9PwmConfigi
//  142 #endif /* EFI_PROD_CODE */
//  143 	if (engineConfiguration->directSelfStimulation) {
        LDR.N    R0,??DataTable7_13
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+80]
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??emulatorApplyPinState_3
//  144 		/**
//  145 		 * this callback would invoke the input signal handlers directly
//  146 		 */
//  147 		helper.handleEmulatorCallback(state, stateIndex);
        MOVS     R2,R5
        MOVS     R1,R4
        LDR.N    R0,??DataTable7_12
          CFI FunCall _ZN21TriggerEmulatorHelper22handleEmulatorCallbackEP9PwmConfigi
        BL       _ZN21TriggerEmulatorHelper22handleEmulatorCallbackEP9PwmConfigi
//  148 	}
//  149 }
??emulatorApplyPinState_3:
??emulatorApplyPinState_1:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock7
//  150 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z18setEmulatorAtIndexiP6Engine
          CFI NoCalls
        THUMB
//  151 static void setEmulatorAtIndex(int index, Engine *engine) {
//  152 	stopEmulationAtIndex = index;
_Z18setEmulatorAtIndexiP6Engine:
        LDR.N    R2,??DataTable7_22
        STR      R0,[R2, #+0]
//  153 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock8
//  154 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z16resumeStimulatorP6Engine
          CFI NoCalls
        THUMB
//  155 static void resumeStimulator(Engine *engine) {
//  156 	isEmulating = true;
_Z16resumeStimulatorP6Engine:
        LDR.N    R1,??DataTable7_23
        MOVS     R2,#+1
        STRB     R2,[R1, #+0]
//  157 	stopEmulationAtIndex = DO_NOT_STOP;
        LDR.N    R1,??DataTable7_22
        LDR.N    R2,??DataTable7_24  ;; 0x3b9ac9ff
        STR      R2,[R1, #+0]
//  158 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock9
//  159 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z24initTriggerEmulatorLogicP7LoggingP6Engine
        THUMB
//  160 void initTriggerEmulatorLogic(Logging *sharedLogger, Engine *engine) {
_Z24initTriggerEmulatorLogicP7LoggingP6Engine:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+32
          CFI CFA R13+48
        MOVS     R4,R0
        MOVS     R5,R1
//  161 	logger = sharedLogger;
        LDR.N    R0,??DataTable7_18
        STR      R4,[R0, #+0]
//  162 
//  163 	TriggerShape *s = &engine->triggerShape;
        ADDS     R6,R5,#+644
//  164 	setTriggerEmulatorRPM(engineConfiguration->bc.triggerSimulatorFrequency, engine);
        MOVS     R1,R5
        LDR.N    R0,??DataTable7_13
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+832]
          CFI FunCall _Z21setTriggerEmulatorRPMiP6Engine
        BL       _Z21setTriggerEmulatorRPMiP6Engine
//  165 	pin_state_t *pinStates[PWM_PHASE_MAX_WAVE_PER_PWM] = { s->wave.waves[0].pinStates, s->wave.waves[1].pinStates,
        LDR      R0,[R6, #+2936]
        LDR      R0,[R0, #+0]
        STR      R0,[SP, #+16]
        LDR      R0,[R6, #+2936]
        LDR      R0,[R0, #+4]
        STR      R0,[SP, #+20]
        LDR      R0,[R6, #+2936]
        LDR      R0,[R0, #+8]
        STR      R0,[SP, #+24]
//  166 			s->wave.waves[2].pinStates };
//  167 	triggerSignal.weComplexInit("position sensor", s->getSize(), s->wave.switchTimes, PWM_PHASE_MAX_WAVE_PER_PWM,
//  168 			pinStates, updateTriggerShapeIfNeeded, emulatorApplyPinState);
        MOVS     R0,R6
          CFI FunCall _ZNK12TriggerShape7getSizeEv
        BL       _ZNK12TriggerShape7getSizeEv
        LDR.N    R1,??DataTable7_25
        STR      R1,[SP, #+12]
        LDR.N    R1,??DataTable7_26
        STR      R1,[SP, #+8]
        ADD      R1,SP,#+16
        STR      R1,[SP, #+4]
        MOVS     R1,#+3
        STR      R1,[SP, #+0]
        LDR      R3,[R6, #+2940]
        MOVS     R2,R0
        LDR.N    R1,??DataTable7_27
        LDR.N    R0,??DataTable7_10
          CFI FunCall _ZN9PwmConfig13weComplexInitEPKciPfiPPaPFvPS_EPFvS5_iE
        BL       _ZN9PwmConfig13weComplexInitEPKciPfiPPaPFvPS_EPFvS5_iE
//  169 
//  170 	addConsoleActionIP("rpm", (VoidIntVoidPtr) setTriggerEmulatorRPM, engine);
        MOVS     R2,R5
        LDR.N    R1,??DataTable7_28
        ADR.N    R0,??DataTable7_1  ;; "rpm"
          CFI FunCall addConsoleActionIP
        BL       addConsoleActionIP
//  171 	addConsoleActionIP("stop_stimulator_at_index", (VoidIntVoidPtr) setEmulatorAtIndex, engine);
        MOVS     R2,R5
        LDR.N    R1,??DataTable7_29
        LDR.N    R0,??DataTable7_30
          CFI FunCall addConsoleActionIP
        BL       addConsoleActionIP
//  172 	addConsoleActionP("resume_stimulator", (VoidPtr) resumeStimulator, engine);
        MOVS     R2,R5
        LDR.N    R1,??DataTable7_31
        LDR.N    R0,??DataTable7_32
          CFI FunCall addConsoleActionP
        BL       addConsoleActionP
//  173 }
        ADD      SP,SP,#+32
          CFI CFA R13+16
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock10

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7:
        DC32     0x49742400

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_1:
        DC8      "rpm"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_2:
        DC32     pinStates1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_3:
        DC32     waves

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_4:
        DC32     pinStates2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_5:
        DC32     waves+0x4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_6:
        DC32     pinStates3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_7:
        DC32     waves+0x8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_8:
        DC32     sr

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_9:
        DC32     swtchTms

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_10:
        DC32     triggerSignal

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_11:
        DC32     emulatorConfigVersion

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_12:
        DC32     helper

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_13:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_14:
        DC32     isUpEvent

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_15:
        DC32     0x404e0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_16:
        DC32     waveChart

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_17:
        DC32     `?<Constant "Emulating position se...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_18:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_19:
        DC32     `?<Constant "Stimulator: updating ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_20:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_21:
        DC32     0xbf800000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_22:
        DC32     stopEmulationAtIndex

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_23:
        DC32     isEmulating

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_24:
        DC32     0x3b9ac9ff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_25:
        DC32     _Z21emulatorApplyPinStateP9PwmConfigi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_26:
        DC32     _Z26updateTriggerShapeIfNeededP9PwmConfig

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_27:
        DC32     `?<Constant "position sensor">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_28:
        DC32     _Z21setTriggerEmulatorRPMiP6Engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_29:
        DC32     _Z18setEmulatorAtIndexiP6Engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_30:
        DC32     `?<Constant "stop_stimulator_at_index">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_31:
        DC32     _Z16resumeStimulatorP6Engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_32:
        DC32     `?<Constant "resume_stimulator">`

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
`?<Constant "Emulating position se...">`:
        DATA
        DC8 "Emulating position sensor(s). RPM=%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Stimulator: updating ...">`:
        DATA
        DC8 "Stimulator: updating trigger shape: %d/%d %d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "position sensor">`:
        DATA
        DC8 "position sensor"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "rpm"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "stop_stimulator_at_index">`:
        DATA
        DC8 "stop_stimulator_at_index"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "resume_stimulator">`:
        DATA
        DC8 "resume_stimulator"
        DC8 0, 0

        END
//  174 #endif
// 
// 1 896 bytes in section .bss
//    13 bytes in section .data
//     4 bytes in section .init_array
//   156 bytes in section .rodata
// 1 014 bytes in section .text
// 
// 1 018 bytes of CODE  memory
//   156 bytes of CONST memory
// 1 909 bytes of DATA  memory
//
//Errors: none
//Warnings: 4
