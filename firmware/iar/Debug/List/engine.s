///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:56 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\alg /
//                       o\engine.cpp                                         /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\alg /
//                       o\engine.cpp -lCN F:\stuff\rusefi_sourceforge\firmwa /
//                       re\iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge\f /
//                       irmware\iar\Debug\List\ -o                           /
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
//                       engine.s                                             /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME engine

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z10getAdvanceif
        EXTERN _Z10getTChargeifff
        EXTERN _Z11interpolatefffff
        EXTERN _Z11triggerInfov
        EXTERN _Z12boolToStringb
        EXTERN _Z13interpolate2dfPfS_i
        EXTERN _Z13interpolate3dfPfifS_iPS_
        EXTERN _Z14getEngineLoadTv
        EXTERN _Z14getInjectorLagf
        EXTERN _Z16getBaseTableFuelP22engine_configuration_sif
        EXTERN _Z16getCltCorrectionf
        EXTERN _Z16getIatCorrectionf
        EXTERN _Z16getSparkDwellMsTi
        EXTERN _Z17getBaroCorrectionv
        EXTERN _Z18getinjectionOffseti
        EXTERN _Z18isRunningBenchTestv
        EXTERN _Z19getInternalAdcValuePKc13adc_channel_e
        EXTERN _Z21getCoolantTemperaturev
        EXTERN _Z23getIntakeAirTemperaturev
        EXTERN _Z6getMapv
        EXTERN _Z6getTPSv
        EXTERN _Z8getVBattv
        EXTERN _Z8hasVBattv
        EXTERN _ZN12TriggerShapeC1Ev
        EXTERN _ZN13RpmCalculator3rpmEv
        EXTERN _ZN13RpmCalculatorC1Ev
        EXTERN _ZN14ThermistorMathC1Ev
        EXTERN _ZN16AccelEnrichmemntC1Ev
        EXTERN _ZN16IntListenerArray16registerCallbackEPFviEPv
        EXTERN _ZN16IntListenerArrayC1Ev
        EXTERN _ZN18LoggingWithStorageC1EPKc
        EXTERN _ZN9OutputPin13getLogicValueEv
        EXTERN _ZN9OutputPin13isInitializedEv
        EXTERN _ZN9OutputPin8setValueEi
        EXTERN __aeabi_d2f
        EXTERN __aeabi_ddiv
        EXTERN __aeabi_dmul
        EXTERN __aeabi_f2d
        EXTERN __aeabi_i2d
        EXTERN __aeabi_memset
        EXTERN _engine
        EXTERN afrMap
        EXTERN engine
        EXTERN engineConfiguration
        EXTERN enginePins
        EXTERN firmwareError
        EXTERN getTimeNowNt
        EXTERN getTimeNowUs
        EXTERN scheduleMsg
        EXTERN veMap
        EXTERN warning

        PUBLIC _ZN11EngineStateC1Ev
        PUBLIC _ZN11EngineStateC2Ev
        PUBLIC _ZN18StartupFuelPumping15setPumpsCounterEP22engine_configuration_si
        PUBLIC _ZN18StartupFuelPumping6updateEv
        PUBLIC _ZN18StartupFuelPumpingC1Ev
        PUBLIC _ZN18StartupFuelPumpingC2Ev
        PUBLIC _ZN5Map3DILi16ELi16EE8getValueEff
        PUBLIC _ZN6Engine10knockLogicEf
        PUBLIC _ZN6Engine12preCalculateEv
        PUBLIC _ZN6Engine14onTriggerEventEx
        PUBLIC _ZN6Engine15printKnockStateEv
        PUBLIC _ZN6Engine17updateSlowSensorsEv
        PUBLIC _ZN6Engine20periodicFastCallbackEv
        PUBLIC _ZN6Engine24addConfigurationListenerEPFvPS_E
        PUBLIC _ZN6Engine4initEv
        PUBLIC _ZN6Engine8stopPinsEv
        PUBLIC _ZN6Engine8watchdogEv
        PUBLIC _ZN6EngineC1EP19persistent_config_s
        PUBLIC _ZN6EngineC2EP19persistent_config_s
        PUBLIC _ZN7Table2DILi8EE7preCalcEPfS1_
        PUBLIC _ZN7Table2DILi8EEC1Ev
        PUBLIC _ZN7Table2DILi8EEC2Ev
        PUBLIC _ZZN5Map3DILi16ELi16EE8getValueEffEs
        PUBLIC _ZZN7Table2DILi8EE7preCalcEPfS1_Es
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\algo\engine.cpp
//    1 /**
//    2  * @file	engine.cpp
//    3  *
//    4  *
//    5  * This might be a http://en.wikipedia.org/wiki/God_object but that's best way I can
//    6  * express myself in C/C++. I am open for suggestions :)
//    7  *
//    8  * @date May 21, 2014
//    9  * @author Andrey Belomutskiy, (c) 2012-2015
//   10  */
//   11 
//   12 #include "main.h"

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memset
          CFI Block cfiBlock0 Using cfiCommon0
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
          CFI EndBlock cfiBlock0
//   13 #include "engine.h"

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN7Table2DILi8EEC1Ev
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _ZN7Table2DILi8EEC1Ev
          CFI NoCalls
        THUMB
// __code __interwork __softfp Table2D<8>::Table2D()
_ZN7Table2DILi8EEC1Ev:
        MOVS     R1,#+0
        STR      R1,[R0, #+64]
        BX       LR               ;; return
          CFI EndBlock cfiBlock1

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN7Table2DILi8EE7preCalcEPfS1_
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _ZN7Table2DILi8EE7preCalcEPfS1_
        THUMB
// __interwork __softfp void Table2D<8>::preCalc(float *, float *)
_ZN7Table2DILi8EE7preCalcEPfS1_:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        VPUSH    {D8}
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        SUB      SP,SP,#+8
          CFI CFA R13+32
        MOVS     R4,R1
        MOVS     R5,R2
        STR      R4,[R0, #+64]
        MOVS     R6,#+0
??preCalc_0:
        CMP      R6,#+7
        BGE.N    ??preCalc_1
        LDR      R1,[R4, R6, LSL #+2]
        VMOV     S16,R1
        ADDS     R1,R4,R6, LSL #+2
        VLDR     S17,[R1, #+4]
        VCMP.F32 S16,S17
        FMSTAT   
        BNE.N    ??preCalc_2
        VMOV     R0,S17
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??preCalc_3
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
        B.N      ??preCalc_4
??preCalc_2:
        LDR      R1,[R5, R6, LSL #+2]
        VMOV     S0,R1
        ADDS     R1,R5,R6, LSL #+2
        VLDR     S1,[R1, #+4]
        VSUB.F32 S2,S0,S1
        VSUB.F32 S3,S16,S17
        VDIV.F32 S2,S2,S3
        ADDS     R1,R0,R6, LSL #+2
        VSTR     S2,[R1, #0]
        ADDS     R1,R0,R6, LSL #+2
        VLDR     S2,[R1, #0]
        VMLS.F32 S0,S2,S16
        ADDS     R1,R0,R6, LSL #+2
        VSTR     S0,[R1, #+32]
        ADDS     R6,R6,#+1
        B.N      ??preCalc_0
??preCalc_1:
??preCalc_4:
        ADD      SP,SP,#+8
          CFI CFA R13+24
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R4-R6,PC}       ;; return
        Nop      
        DATA
??preCalc_3:
        DC32     _ZZN7Table2DILi8EE7preCalcEPfS1_Es
          CFI EndBlock cfiBlock2

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN5Map3DILi16ELi16EE8getValueEff
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _ZN5Map3DILi16ELi16EE8getValueEff
        THUMB
// __interwork __vfp float Map3D<16, 16>::getValue(float, float)
_ZN5Map3DILi16ELi16EE8getValueEff:
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
        VMOV.F32 S16,S0
        VMOV.F32 S17,S1
        LDR      R0,[R4, #+72]
        LDR.N    R1,??getValue_0+0x4  ;; 0x923f558
        CMP      R0,R1
        BEQ.N    ??getValue_1
        LDR.N    R0,??getValue_0+0x8
          CFI FunCall firmwareError
        BL       firmwareError
        VLDR.W   S0,??getValue_0  ;; 0x7fffffff
        B.N      ??getValue_2
??getValue_1:
        STR      R4,[SP, #+0]
        MOVS     R3,#+16
        LDR      R2,[R4, #+68]
        VMOV.F32 S1,S17
        MOVS     R1,#+16
        LDR      R0,[R4, #+64]
        VMOV.F32 S0,S16
          CFI FunCall _Z13interpolate3dfPfifS_iPS_
        BL       _Z13interpolate3dfPfifS_iPS_
??getValue_2:
        ADD      SP,SP,#+8
          CFI CFA R13+16
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
        DATA
??getValue_0:
        DC32     0x7fffffff
        DC32     0x923f558
        DC32     _ZZN5Map3DILi16ELi16EE8getValueEffEs
          CFI EndBlock cfiBlock3
//   14 #include "engine_state.h"
//   15 #include "efiGpio.h"
//   16 #include "trigger_central.h"
//   17 #include "fuel_math.h"
//   18 #include "engine_math.h"
//   19 #include "advance_map.h"
//   20 #include "speed_density.h"
//   21 
//   22 #if EFI_PROD_CODE
//   23 #include "injector_central.h"
//   24 #else
//   25 #define isRunningBenchTest() true
//   26 #endif
//   27 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   28 static LoggingWithStorage logger("engine");
logger:
        DS8 260

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _ZN11EngineStateC2Ev
        THUMB
// __code __interwork __softfp EngineState::subobject EngineState()
_ZN11EngineStateC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN11EngineStateC1Ev
        BL       _ZN11EngineStateC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock4

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _ZN6EngineC2EP19persistent_config_s
        THUMB
// __code __interwork __softfp Engine::subobject Engine(persistent_config_s *)
_ZN6EngineC2EP19persistent_config_s:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN6EngineC1EP19persistent_config_s
        BL       _ZN6EngineC1EP19persistent_config_s
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock5

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _ZN18StartupFuelPumpingC2Ev
        THUMB
// __code __interwork __softfp StartupFuelPumping::subobject StartupFuelPumping()
_ZN18StartupFuelPumpingC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN18StartupFuelPumpingC1Ev
        BL       _ZN18StartupFuelPumpingC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock6

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        LDR.W    R1,??DataTable12_3
        LDR.W    R0,??DataTable12_4
          CFI FunCall _ZN18LoggingWithStorageC1EPKc
        BL       _ZN18LoggingWithStorageC1EPKc
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock7
//   29 
//   30 extern engine_pins_s enginePins;
//   31 
//   32 EXTERN_ENGINE
//   33 ;
//   34 
//   35 /**
//   36  * We are executing these heavy (logarithm) methods from outside the trigger callbacks for performance reasons.
//   37  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _ZN6Engine17updateSlowSensorsEv
        THUMB
//   38 void Engine::updateSlowSensors() {
_ZN6Engine17updateSlowSensorsEv:
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
//   39 	Engine *engine = this;
        MOVS     R5,R4
//   40 	board_configuration_s * boardConfiguration = &engineConfiguration->bc;
        LDR      R0,[R4, #+36]
        ADDS     R6,R0,#+624
//   41 	engineState.iat = getIntakeAirTemperature(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z23getIntakeAirTemperaturev
        BL       _Z23getIntakeAirTemperaturev
        ADD      R0,R4,#+7680
        ADDS     R0,R0,#+80
        VSTR     S0,[R0, #0]
//   42 	engineState.clt = getCoolantTemperature(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z21getCoolantTemperaturev
        BL       _Z21getCoolantTemperaturev
        ADD      R0,R4,#+7680
        ADDS     R0,R0,#+84
        VSTR     S0,[R0, #0]
//   43 
//   44 	if (engineConfiguration->fuelLevelSensor != EFI_ADC_NONE) {
        LDR      R0,[R4, #+36]
        LDR      R0,[R0, #+484]
        CMP      R0,#+16
        BEQ.N    ??updateSlowSensors_0
//   45 		float fuelLevelVoltage = getVoltageDivided("fuel", engineConfiguration->fuelLevelSensor);
        LDR      R0,[R4, #+36]
        LDR      R1,[R0, #+484]
        LDR.W    R0,??DataTable12_5
          CFI FunCall _Z19getInternalAdcValuePKc13adc_channel_e
        BL       _Z19getInternalAdcValuePKc13adc_channel_e
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        VLDR.W   S1,??DataTable8  ;; 0x3a400c01
        VMUL.F32 S0,S0,S1
        LDR      R0,[R4, #+36]
        VLDR     S1,[R0, #+464]
        VMUL.F32 S16,S0,S1
//   46 		engineState.fuelLevel = interpolate(boardConfiguration->fuelLevelEmptyTankVoltage, 0,
//   47 				boardConfiguration->fuelLevelFullTankVoltage, 100,
//   48 				fuelLevelVoltage);
        VMOV.F32 S4,S16
        VLDR.W   S3,??DataTable8_1  ;; 0x42c80000
        VLDR     S2,[R6, #+272]
        VLDR.W   S1,??DataTable8_2  ;; 0x0
        VLDR     S0,[R6, #+268]
          CFI FunCall _Z11interpolatefffff
        BL       _Z11interpolatefffff
        ADD      R0,R4,#+7680
        ADDS     R0,R0,#+88
        VSTR     S0,[R0, #0]
//   49 	}
//   50 	float vBatt = hasVBatt(PASS_ENGINE_PARAMETER_F) ? getVBatt(PASS_ENGINE_PARAMETER_F) : 12;
??updateSlowSensors_0:
          CFI FunCall _Z8hasVBattv
        BL       _Z8hasVBattv
        CMP      R0,#+0
        BEQ.N    ??updateSlowSensors_1
          CFI FunCall _Z8getVBattv
        BL       _Z8getVBattv
        VMOV.F32 S16,S0
        B.N      ??updateSlowSensors_2
??updateSlowSensors_1:
        VMOV.F32 S16,#12.0
//   51 
//   52 	injectorLagMs = getInjectorLag(vBatt PASS_ENGINE_PARAMETER);
??updateSlowSensors_2:
        VMOV.F32 S0,S16
          CFI FunCall _Z14getInjectorLagf
        BL       _Z14getInjectorLagf
        ADD      R0,R4,#+7680
        ADDS     R0,R0,#+240
        VSTR     S0,[R0, #0]
//   53 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock8
//   54 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _ZN6Engine14onTriggerEventEx
          CFI NoCalls
        THUMB
//   55 void Engine::onTriggerEvent(efitick_t nowNt) {
_ZN6Engine14onTriggerEventEx:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
//   56 	isSpinning = true;
        MOVW     R1,#+9232
        MOVS     R4,#+1
        STRB     R4,[R1, R0]
//   57 	lastTriggerEventTimeNt = nowNt;
        ADD      R1,R0,#+7680
        ADDS     R1,R1,#+232
        STRD     R2,R3,[R1, #+0]
//   58 }
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock9
//   59 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z24invokeEnginePreCalculateP6Engine
        THUMB
//   60 static void invokeEnginePreCalculate(Engine *engine) {
_Z24invokeEnginePreCalculateP6Engine:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   61 	engine->preCalculate();
        MOVS     R0,R4
          CFI FunCall _ZN6Engine12preCalculateEv
        BL       _ZN6Engine12preCalculateEv
//   62 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock10
//   63 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _ZN6Engine24addConfigurationListenerEPFvPS_E
        THUMB
//   64 void Engine::addConfigurationListener(configuration_callback_t callback) {
_ZN6Engine24addConfigurationListenerEPFvPS_E:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   65 	configurationListeners.registerCallback((VoidInt)invokeEnginePreCalculate, this);
        MOVS     R2,R4
        LDR.W    R1,??DataTable12_6
        ADD      R0,R4,#+8960
        ADDS     R0,R0,#+68
          CFI FunCall _ZN16IntListenerArray16registerCallbackEPFviEPv
        BL       _ZN16IntListenerArray16registerCallbackEPFviEPv
//   66 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock11
//   67 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _ZN6EngineC1EP19persistent_config_s
        THUMB
//   68 Engine::Engine(persistent_config_s *config) {
_ZN6EngineC1EP19persistent_config_s:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R0,R4
          CFI FunCall _ZN13RpmCalculatorC1Ev
        BL       _ZN13RpmCalculatorC1Ev
        ADDS     R0,R4,#+56
          CFI FunCall _ZN16AccelEnrichmemntC1Ev
        BL       _ZN16AccelEnrichmemntC1Ev
        ADDS     R0,R4,#+332
          CFI FunCall _ZN16AccelEnrichmemntC1Ev
        BL       _ZN16AccelEnrichmemntC1Ev
        ADDS     R0,R4,#+644
          CFI FunCall _ZN12TriggerShapeC1Ev
        BL       _ZN12TriggerShapeC1Ev
        ADD      R0,R4,#+7680
        ADDS     R0,R0,#+80
          CFI FunCall _ZN11EngineStateC1Ev
        BL       _ZN11EngineStateC1Ev
        ADDS     R0,R4,#+7936
          CFI FunCall _ZN7Table2DILi8EEC1Ev
        BL       _ZN7Table2DILi8EEC1Ev
        ADD      R0,R4,#+8960
        ADDS     R0,R0,#+68
          CFI FunCall _ZN16IntListenerArrayC1Ev
        BL       _ZN16IntListenerArrayC1Ev
//   69 	lastTriggerEventTimeNt = 0;
        ADD      R0,R4,#+7680
        ADDS     R0,R0,#+232
        MOVS     R2,#+0
        MOVS     R3,#+0
        STRD     R2,R3,[R0, #+0]
//   70 	isCylinderCleanupMode = false;
        MOVW     R0,#+7928
        MOVS     R1,#+0
        STRB     R1,[R0, R4]
//   71 	engineCycleEventCount = 0;
        MOVW     R0,#+7932
        MOVS     R1,#+0
        STR      R1,[R0, R4]
//   72 	stopEngineRequestTimeNt = 0;
        MOVS     R0,#+0
        MOVS     R1,#+0
        STRD     R0,R1,[R4, #+48]
//   73 	isRunningPwmTest = false;
        MOVS     R0,#+0
        STRB     R0,[R4, #+614]
//   74 	isTestMode = false;
        MOVS     R0,#+0
        STRB     R0,[R4, #+640]
//   75 	isSpinning = false;
        MOVW     R0,#+9232
        MOVS     R1,#+0
        STRB     R1,[R0, R4]
//   76 	adcToVoltageInputDividerCoefficient = NAN;
        MOVW     R0,#+7924
        MVNS     R1,#-2147483648
        STR      R1,[R0, R4]
//   77 	this->config = config;
        STR      R5,[R4, #+32]
//   78 	engineConfiguration = &config->engineConfiguration;
        STR      R5,[R4, #+36]
//   79 	engineConfiguration2 = NULL;
        MOVS     R0,#+0
        STR      R0,[R4, #+40]
//   80 	engineState.iat = engineState.clt = NAN;
        VLDR.W   S0,??DataTable9  ;; 0x7fffffff
        ADD      R0,R4,#+7680
        ADDS     R0,R0,#+84
        VSTR     S0,[R0, #0]
        ADD      R0,R4,#+7680
        ADDS     R0,R0,#+80
        VSTR     S0,[R0, #0]
//   81 	memset(&ignitionPin, 0, sizeof(ignitionPin));
        MOVS     R2,#+48
        MOVS     R1,#+0
        ADDS     R0,R4,#+7712
          CFI FunCall memset
        BL       memset
//   82 	memset(config, 0, sizeof(persistent_config_s));
        MOVW     R2,#+15288
        MOVS     R1,#+0
        MOVS     R0,R5
          CFI FunCall memset
        BL       memset
//   83 
//   84 	knockNow = false;
        MOVS     R0,#+0
        STRB     R0,[R4, #+615]
//   85 	knockEver = false;
        MOVS     R0,#+0
        STRB     R0,[R4, #+616]
//   86 	knockCount = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+620]
//   87 	knockDebug = false;
        MOVS     R0,#+0
        STRB     R0,[R4, #+624]
//   88 	timeOfLastKnockEvent = 0;
        MOVS     R0,#+0
        MOVS     R1,#+0
        STRD     R0,R1,[R4, #+632]
//   89 	injectorLagMs = fuelMs = 0;
        VLDR.W   S0,??DataTable8_2  ;; 0x0
        VSTR     S0,[R4, #+608]
        ADD      R0,R4,#+7680
        ADDS     R0,R0,#+240
        VSTR     S0,[R0, #0]
//   90 	clutchDownState = clutchUpState = false;
        MOVS     R0,#+0
        STRB     R0,[R4, #+612]
        STRB     R0,[R4, #+613]
//   91 	memset(&m, 0, sizeof(m));
        MOVS     R2,#+80
        MOVS     R1,#+0
        ADDS     R0,R4,#+9152
          CFI FunCall memset
        BL       memset
//   92 
//   93 	addConfigurationListener(invokeEnginePreCalculate);
        LDR.W    R1,??DataTable12_6
        MOVS     R0,R4
          CFI FunCall _ZN6Engine24addConfigurationListenerEPFvPS_E
        BL       _ZN6Engine24addConfigurationListenerEPFvPS_E
//   94 }
        MOVS     R0,R4
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock12
//   95 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _ZN11EngineStateC1Ev
        THUMB
//   96 EngineState::EngineState() {
_ZN11EngineStateC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        ADDS     R0,R4,#+12
          CFI FunCall _ZN14ThermistorMathC1Ev
        BL       _ZN14ThermistorMathC1Ev
        ADDS     R0,R4,#+52
          CFI FunCall _ZN14ThermistorMathC1Ev
        BL       _ZN14ThermistorMathC1Ev
//   97 	advance = dwellAngle = 0;
        VLDR.W   S0,??DataTable8_2  ;; 0x0
        VSTR     S0,[R4, #+108]
        VSTR     S0,[R4, #+112]
//   98 }
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock13
//   99 
//  100 /**
//  101  * Here we have a bunch of stuff which should invoked after configuration change
//  102  * so that we can prepare some helper structures
//  103  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _ZN6Engine12preCalculateEv
        THUMB
//  104 void Engine::preCalculate() {
_ZN6Engine12preCalculateEv:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        VPUSH    {D8}
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
//  105 	sparkTable.preCalc(engineConfiguration->sparkDwellBins,
//  106 			engineConfiguration->sparkDwell);
        LDR      R0,[R4, #+36]
        ADDS     R2,R0,#+368
        LDR      R0,[R4, #+36]
        ADDS     R1,R0,#+336
        ADDS     R0,R4,#+7936
          CFI FunCall _ZN7Table2DILi8EE7preCalcEPfS1_
        BL       _ZN7Table2DILi8EE7preCalcEPfS1_
//  107 
//  108 	/**
//  109 	 * Here we prepare a fast, index-based MAF lookup from a slower curve description
//  110 	 */
//  111 	for (int i = 0; i < MAF_DECODING_CACHE_SIZE; i++) {
        MOVS     R5,#+0
??preCalculate_0:
        CMP      R5,#+255
        BGT.N    ??preCalculate_1
//  112 		float volts = i / MAF_DECODING_CACHE_MULT;
        MOVS     R0,R5
          CFI FunCall __aeabi_i2d
        BL       __aeabi_i2d
        LDR.W    R2,??DataTable12_7  ;; 0x9999999a
        LDR.W    R3,??DataTable12_8  ;; 0x40499999
          CFI FunCall __aeabi_ddiv
        BL       __aeabi_ddiv
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        VMOV     S16,R0
//  113 		float maf = interpolate2d(volts, config->mafDecodingBins,
//  114 				config->mafDecoding, MAF_DECODING_COUNT);
        MOV      R2,#+256
        LDR      R0,[R4, #+32]
        ADD      R1,R0,#+6144
        ADDS     R1,R1,#+184
        LDR      R0,[R4, #+32]
        ADD      R0,R0,#+7168
        ADDS     R0,R0,#+184
        VMOV.F32 S0,S16
          CFI FunCall _Z13interpolate2dfPfS_i
        BL       _Z13interpolate2dfPfS_i
//  115 		mafDecodingLookup[i] = maf;
        ADDS     R0,R4,R5, LSL #+2
        ADD      R0,R0,#+7936
        ADDS     R0,R0,#+68
        VSTR     S0,[R0, #0]
//  116 	}
        ADDS     R5,R5,#+1
        B.N      ??preCalculate_0
//  117 }
??preCalculate_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock14
//  118 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function _ZN6Engine4initEv
          CFI NoCalls
        THUMB
//  119 void Engine::init() {
//  120 }
_ZN6Engine4initEv:
        BX       LR               ;; return
          CFI EndBlock cfiBlock15
//  121 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function _Z7stopPinP14NamedOutputPin
        THUMB
//  122 static bool stopPin(NamedOutputPin *output) {
_Z7stopPinP14NamedOutputPin:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  123 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//  124 	if (output->isInitialized() && output->getLogicValue()) {
        MOVS     R0,R4
          CFI FunCall _ZN9OutputPin13isInitializedEv
        BL       _ZN9OutputPin13isInitializedEv
        CMP      R0,#+0
        BEQ.N    ??stopPin_0
        MOVS     R0,R4
          CFI FunCall _ZN9OutputPin13getLogicValueEv
        BL       _ZN9OutputPin13getLogicValueEv
        CMP      R0,#+0
        BEQ.N    ??stopPin_0
//  125 		output->setValue(false);
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
//  126 		scheduleMsg(&logger, "turning off %s", output->name);
        LDR      R2,[R4, #+16]
        LDR.W    R1,??DataTable12_9
        LDR.W    R0,??DataTable12_4
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  127 		return true;
        MOVS     R0,#+1
        B.N      ??stopPin_1
//  128 	}
//  129 #endif
//  130 	return false;
??stopPin_0:
        MOVS     R0,#+0
??stopPin_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock16
//  131 }
//  132 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function _ZN6Engine8stopPinsEv
        THUMB
//  133 bool_t Engine::stopPins() {
_ZN6Engine8stopPinsEv:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  134 	bool result = false;
        MOVS     R6,#+0
//  135 	for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
        MOVS     R5,#+0
??stopPins_0:
        LDR      R0,[R4, #+36]
        LDR      R0,[R0, #+404]
        CMP      R5,R0
        BGE.N    ??stopPins_1
//  136 		result |= stopPin(&enginePins.coils[i]);
        MOVS     R0,#+20
        LDR.W    R1,??DataTable12_10
        MLA      R0,R0,R5,R1
        ADDS     R0,R0,#+336
          CFI FunCall _Z7stopPinP14NamedOutputPin
        BL       _Z7stopPinP14NamedOutputPin
        ORRS     R6,R0,R6
//  137 		result |= stopPin(&enginePins.injectors[i]);
        MOVS     R0,#+20
        LDR.W    R1,??DataTable12_10
        MLA      R0,R0,R5,R1
        ADDS     R0,R0,#+96
          CFI FunCall _Z7stopPinP14NamedOutputPin
        BL       _Z7stopPinP14NamedOutputPin
        ORRS     R6,R0,R6
//  138 	}
        ADDS     R5,R5,#+1
        B.N      ??stopPins_0
//  139 	return result;
??stopPins_1:
        MOVS     R0,R6
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock17
//  140 }
//  141 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function _ZN6Engine15printKnockStateEv
        THUMB
//  142 void Engine::printKnockState(void) {
_ZN6Engine15printKnockStateEv:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  143 	scheduleMsg(&logger, "knock now=%s/ever=%s", boolToString(knockNow), boolToString(knockEver));
        LDRB     R0,[R4, #+616]
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R5,R0
        LDRB     R0,[R4, #+615]
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R3,R5
        MOVS     R2,R0
        LDR.W    R1,??DataTable12_11
        LDR.W    R0,??DataTable12_4
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  144 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock18

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8:
        DC32     0x3a400c01

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_1:
        DC32     0x42c80000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_2:
        DC32     0x0
//  145 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock19 Using cfiCommon0
          CFI Function _ZN6Engine10knockLogicEf
        THUMB
//  146 void Engine::knockLogic(float knockVolts) {
_ZN6Engine10knockLogicEf:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        VMOV.F32 S16,S0
//  147     knockNow = knockVolts > engineConfiguration->knockVThreshold;
        LDR      R0,[R4, #+36]
        LDR      R0,[R0, #+1552]
        VMOV     S0,R0
        VCMP.F32 S0,S16
        FMSTAT   
        BPL.N    ??knockLogic_0
        MOVS     R0,#+1
        B.N      ??knockLogic_1
??knockLogic_0:
        MOVS     R0,#+0
??knockLogic_1:
        STRB     R0,[R4, #+615]
//  148     /**
//  149      * KnockCount is directly proportional to the degrees of ignition
//  150      * advance removed
//  151      * ex: degrees to subtract = knockCount;
//  152      */
//  153 
//  154     /**
//  155      * TODO use knockLevel as a factor for amount of ignition advance
//  156      * to remove
//  157      * Perhaps allow the user to set a multiplier
//  158      * ex: degrees to subtract = knockCount + (knockLevel * X)
//  159      * X = user configurable multiplier
//  160      */
//  161     if (knockNow) {
        LDRB     R0,[R4, #+615]
        CMP      R0,#+0
        BEQ.N    ??knockLogic_2
//  162         knockEver = true;
        MOVS     R0,#+1
        STRB     R0,[R4, #+616]
//  163         timeOfLastKnockEvent = getTimeNowUs();
          CFI FunCall getTimeNowUs
        BL       getTimeNowUs
        STRD     R0,R1,[R4, #+632]
//  164         if (knockCount < engineConfiguration->maxKnockSubDeg)
        VLDR     S0,[R4, #+620]
        VCVT.F32.S32 S0,S0
        LDR      R0,[R4, #+36]
        VLDR     S1,[R0, #+576]
        VCMP.F32 S0,S1
        FMSTAT   
        BPL.N    ??knockLogic_3
//  165             knockCount++;
        LDR      R0,[R4, #+620]
        ADDS     R0,R0,#+1
        STR      R0,[R4, #+620]
        B.N      ??knockLogic_3
//  166     } else if (knockCount >= 1) {
??knockLogic_2:
        LDR      R0,[R4, #+620]
        CMP      R0,#+1
        BLT.N    ??knockLogic_4
//  167         knockCount--;
        LDR      R0,[R4, #+620]
        SUBS     R0,R0,#+1
        STR      R0,[R4, #+620]
        B.N      ??knockLogic_3
//  168 	} else {
//  169         knockCount = 0;
??knockLogic_4:
        MOVS     R0,#+0
        STR      R0,[R4, #+620]
//  170     }
//  171 }
??knockLogic_3:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock19
//  172 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock20 Using cfiCommon0
          CFI Function _ZN6Engine8watchdogEv
        THUMB
//  173 void Engine::watchdog() {
_ZN6Engine8watchdogEv:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        SUB      SP,SP,#+16
          CFI CFA R13+40
        MOV      R8,R0
//  174 #if EFI_ENGINE_CONTROL
//  175 	if (isRunningPwmTest)
        LDRB     R0,[R8, #+614]
        CMP      R0,#+0
        BNE.N    ??watchdog_0
//  176 		return;
//  177 	if (!isSpinning) {
??watchdog_1:
        MOVW     R0,#+9232
        LDRB     R0,[R0, R8]
        CMP      R0,#+0
        BNE.N    ??watchdog_2
//  178 		if (!isRunningBenchTest() && stopPins()) {
          CFI FunCall _Z18isRunningBenchTestv
        BL       _Z18isRunningBenchTestv
        CMP      R0,#+0
        BNE.N    ??watchdog_3
        MOV      R0,R8
          CFI FunCall _ZN6Engine8stopPinsEv
        BL       _ZN6Engine8stopPinsEv
        CMP      R0,#+0
        BEQ.N    ??watchdog_3
//  179 			firmwareError("Some pins were turned off by 2nd pass watchdog");
        LDR.N    R0,??DataTable12_12
          CFI FunCall firmwareError
        BL       firmwareError
//  180 		}
//  181 		return;
??watchdog_3:
        B.N      ??watchdog_0
//  182 	}
//  183 	efitick_t nowNt = getTimeNowNt();
??watchdog_2:
          CFI FunCall getTimeNowNt
        BL       getTimeNowNt
        MOVS     R4,R0
        MOVS     R5,R1
//  184 	/**
//  185 	 * Lowest possible cranking is about 240 RPM, that's 4 revolutions per second.
//  186 	 * 0.25 second is 250000 uS
//  187 	 *
//  188 	 * todo: better watch dog implementation should be implemented - see
//  189 	 * http://sourceforge.net/p/rusefi/tickets/96/
//  190 	 *
//  191 	 * note that the result of this subtraction could be negative, that would happen if
//  192 	 * we have a trigger event between the time we've invoked 'getTimeNow' and here
//  193 	 */
//  194 	efitick_t timeSinceLastTriggerEvent = nowNt - lastTriggerEventTimeNt;
        ADD      R0,R8,#+7680
        ADDS     R0,R0,#+232
        LDRD     R2,R3,[R0, #+0]
        SUBS     R6,R4,R2
        SBCS     R7,R5,R3
//  195 	if (timeSinceLastTriggerEvent < US2NT(250000LL)) {
        LDR.N    R0,??DataTable12_13  ;; 0x280de80
        MOVS     R1,#+0
        CMP      R7,R1
        BGT.N    ??watchdog_4
        BLT.N    ??watchdog_5
        CMP      R6,R0
        BCS.N    ??watchdog_4
//  196 		return;
??watchdog_5:
        B.N      ??watchdog_0
//  197 	}
//  198 	isSpinning = false;
??watchdog_4:
        MOVW     R0,#+9232
        MOVS     R1,#+0
        STRB     R1,[R0, R8]
//  199 #if EFI_PROD_CODE || EFI_SIMULATOR
//  200 	scheduleMsg(&logger, "engine has STOPPED");
        LDR.N    R1,??DataTable12_14
        LDR.N    R0,??DataTable12_4
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  201 	scheduleMsg(&logger, "templog engine has STOPPED [%x][%x] [%x][%x] %d",
//  202 			(int)(nowNt >> 32), (int)nowNt,
//  203 			(int)(lastTriggerEventTimeNt >> 32), (int)lastTriggerEventTimeNt,
//  204 			(int)timeSinceLastTriggerEvent
//  205 			);
        STR      R6,[SP, #+8]
        ADD      R2,R8,#+7680
        ADDS     R2,R2,#+232
        LDRD     R0,R1,[R2, #+0]
        STR      R0,[SP, #+4]
        ADD      R2,R8,#+7680
        ADDS     R2,R2,#+232
        LDRD     R0,R1,[R2, #+0]
        STR      R1,[SP, #+0]
        MOVS     R3,R4
        MOVS     R2,R5
        LDR.N    R1,??DataTable12_15
        LDR.N    R0,??DataTable12_4
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  206 	triggerInfo();
          CFI FunCall _Z11triggerInfov
        BL       _Z11triggerInfov
//  207 #endif
//  208 
//  209 	stopPins();
        MOV      R0,R8
          CFI FunCall _ZN6Engine8stopPinsEv
        BL       _ZN6Engine8stopPinsEv
//  210 #endif
//  211 }
??watchdog_0:
        POP      {R0-R8,PC}       ;; return
          CFI EndBlock cfiBlock20

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9:
        DC32     0x7fffffff
//  212 
//  213 extern fuel_Map3D_t veMap;
//  214 extern fuel_Map3D_t afrMap;
//  215 
//  216 /**
//  217  * The idea of this method is to execute all heavy calculations in a lower-priority thread,
//  218  * so that trigger event handler/IO scheduler tasks are faster. Th
//  219  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock21 Using cfiCommon0
          CFI Function _ZN6Engine20periodicFastCallbackEv
        THUMB
//  220 void Engine::periodicFastCallback(DECLARE_ENGINE_PARAMETER_F) {
_ZN6Engine20periodicFastCallbackEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8-D10}
          CFI D10 Frame(CFA, -16)
          CFI D9 Frame(CFA, -24)
          CFI D8 Frame(CFA, -32)
          CFI CFA R13+32
        MOVS     R4,R0
//  221 	int rpm = rpmCalculator.rpmValue;
        VLDR     S16,[R4, #0]
//  222 	float engineLoad = getEngineLoadT(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z14getEngineLoadTv
        BL       _Z14getEngineLoadTv
        VMOV.F32 S17,S0
//  223 
//  224 	engineState.sparkDwell = getSparkDwellMsT(rpm PASS_ENGINE_PARAMETER);
        VMOV     R0,S16
          CFI FunCall _Z16getSparkDwellMsTi
        BL       _Z16getSparkDwellMsTi
        ADD      R0,R4,#+7680
        ADDS     R0,R0,#+180
        VSTR     S0,[R0, #0]
//  225 	// todo: move this field to engineState
//  226 	engine->engineState.dwellAngle = engineState.sparkDwell / getOneDegreeTimeMs(rpm);
        VCVT.F32.S32 S0,S16
        VLDR.W   S1,??DataTable12  ;; 0x4326aaab
        VDIV.F32 S0,S1,S0
        ADD      R0,R4,#+7680
        ADDS     R0,R0,#+180
        VLDR     S1,[R0, #0]
        VDIV.F32 S0,S1,S0
        LDR.N    R0,??DataTable12_16
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+7680
        ADDS     R0,R0,#+188
        VSTR     S0,[R0, #0]
//  227 
//  228 	engine->engineState.iatFuelCorrection = getIatCorrection(engine->engineState.iat PASS_ENGINE_PARAMETER);
        LDR.N    R0,??DataTable12_16
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+7680
        ADDS     R0,R0,#+80
        VLDR     S0,[R0, #0]
          CFI FunCall _Z16getIatCorrectionf
        BL       _Z16getIatCorrectionf
        LDR.N    R0,??DataTable12_16
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+7680
        ADDS     R0,R0,#+196
        VSTR     S0,[R0, #0]
//  229 	engine->engineState.cltFuelCorrection = getCltCorrection(engine->engineState.clt PASS_ENGINE_PARAMETER);
        LDR.N    R0,??DataTable12_16
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+7680
        ADDS     R0,R0,#+84
        VLDR     S0,[R0, #0]
          CFI FunCall _Z16getCltCorrectionf
        BL       _Z16getCltCorrectionf
        LDR.N    R0,??DataTable12_16
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+7680
        ADDS     R0,R0,#+200
        VSTR     S0,[R0, #0]
//  230 
//  231 	engine->engineState.baroCorrection = getBaroCorrection(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z17getBaroCorrectionv
        BL       _Z17getBaroCorrectionv
        LDR.N    R0,??DataTable12_16
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+7680
        ADDS     R0,R0,#+208
        VSTR     S0,[R0, #0]
//  232 
//  233 	engine->engineState.injectionOffset = getinjectionOffset(rpm PASS_ENGINE_PARAMETER);
        VMOV     R0,S16
          CFI FunCall _Z18getinjectionOffseti
        BL       _Z18getinjectionOffseti
        LDR.N    R0,??DataTable12_16
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+7680
        ADDS     R0,R0,#+228
        VSTR     S0,[R0, #0]
//  234 	engine->engineState.timingAdvance = getAdvance(rpm, engineLoad PASS_ENGINE_PARAMETER);
        VMOV.F32 S0,S17
        VMOV     R0,S16
          CFI FunCall _Z10getAdvanceif
        BL       _Z10getAdvanceif
        LDR.N    R0,??DataTable12_16
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+7680
        ADDS     R0,R0,#+184
        VSTR     S0,[R0, #0]
//  235 
//  236 	if (engineConfiguration->algorithm == LM_SPEED_DENSITY) {
        LDR      R0,[R4, #+36]
        LDR      R0,[R0, #+424]
        CMP      R0,#+3
        BNE.N    ??periodicFastCallback_0
//  237 		float coolantC = ENGINE(engineState.clt);
        LDR.N    R0,??DataTable12_17
        VLDR     S18,[R0, #0]
//  238 		float intakeC = ENGINE(engineState.iat);
        LDR.N    R0,??DataTable12_18
        VLDR     S19,[R0, #0]
//  239 		float tps = getTPS(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z6getTPSv
        BL       _Z6getTPSv
        VMOV.F32 S20,S0
//  240 		engine->engineState.tChargeK = convertCelsiusToKelvin(getTCharge(rpm, tps, coolantC, intakeC));
        VMOV.F32 S2,S19
        VMOV.F32 S1,S18
        VMOV.F32 S0,S20
        VMOV     R0,S16
          CFI FunCall _Z10getTChargeifff
        BL       _Z10getTChargeifff
        VLDR.W   S1,??DataTable12_1  ;; 0x43889333
        VADD.F32 S0,S0,S1
        LDR.N    R0,??DataTable12_16
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+7680
        ADDS     R0,R0,#+212
        VSTR     S0,[R0, #0]
//  241 		float map = getMap();
          CFI FunCall _Z6getMapv
        BL       _Z6getMapv
        VMOV.F32 S21,S0
//  242 
//  243 		/**
//  244 		 * *0.01 because of https://sourceforge.net/p/rusefi/tickets/153/
//  245 		 */
//  246 		engine->engineState.currentVE = engine->engineState.baroCorrection * veMap.getValue(map, rpm) * 0.01;
        VCVT.F32.S32 S1,S16
        VMOV.F32 S0,S21
        LDR.N    R0,??DataTable12_19
          CFI FunCall _ZN5Map3DILi16ELi16EE8getValueEff
        BL       _ZN5Map3DILi16ELi16EE8getValueEff
        LDR.N    R0,??DataTable12_16
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+7680
        ADDS     R0,R0,#+208
        VLDR     S1,[R0, #0]
        VMUL.F32 S0,S1,S0
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        LDR.N    R2,??DataTable12_20  ;; 0x47ae147b
        LDR.N    R3,??DataTable12_21  ;; 0x3f847ae1
          CFI FunCall __aeabi_dmul
        BL       __aeabi_dmul
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        MOVW     R1,#+7896
        LDR.N    R2,??DataTable12_16
        LDR      R2,[R2, #+0]
        STR      R0,[R1, R2]
//  247 		engine->engineState.targerAFR = afrMap.getValue(map, rpm);
        VCVT.F32.S32 S1,S16
        VMOV.F32 S0,S21
        LDR.N    R0,??DataTable12_22
          CFI FunCall _ZN5Map3DILi16ELi16EE8getValueEff
        BL       _ZN5Map3DILi16ELi16EE8getValueEff
        LDR.N    R0,??DataTable12_16
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+7680
        ADDS     R0,R0,#+220
        VSTR     S0,[R0, #0]
        B.N      ??periodicFastCallback_1
//  248 	} else {
//  249 		float engineLoad = getEngineLoadT(PASS_ENGINE_PARAMETER_F);
??periodicFastCallback_0:
          CFI FunCall _Z14getEngineLoadTv
        BL       _Z14getEngineLoadTv
        VMOV.F32 S18,S0
//  250 		engine->engineState.baseTableFuel = getBaseTableFuel(engineConfiguration, rpm, engineLoad);
        VMOV.F32 S0,S18
        VMOV     R1,S16
        LDR      R0,[R4, #+36]
          CFI FunCall _Z16getBaseTableFuelP22engine_configuration_sif
        BL       _Z16getBaseTableFuelP22engine_configuration_sif
        LDR.N    R0,??DataTable12_16
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+7904
        VSTR     S0,[R0, #0]
//  251 	}
//  252 }
??periodicFastCallback_1:
        VPOP     {D8-D10}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI D10 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock21
//  253 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock22 Using cfiCommon0
          CFI Function _ZN18StartupFuelPumpingC1Ev
          CFI NoCalls
        THUMB
//  254 StartupFuelPumping::StartupFuelPumping() {
//  255 	isTpsAbove50 = false;
_ZN18StartupFuelPumpingC1Ev:
        MOVS     R1,#+0
        STRB     R1,[R0, #+0]
//  256 	pumpsCounter = 0;
        MOVS     R1,#+0
        STR      R1,[R0, #+4]
//  257 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock22
//  258 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock23 Using cfiCommon0
          CFI Function _ZN18StartupFuelPumping15setPumpsCounterEP22engine_configuration_si
        THUMB
//  259 void StartupFuelPumping::setPumpsCounter(
//  260 		engine_configuration_s *engineConfiguration, int newValue) {
_ZN18StartupFuelPumping15setPumpsCounterEP22engine_configuration_si:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  261 	if (pumpsCounter != newValue) {
        LDR      R0,[R4, #+4]
        CMP      R0,R6
        BEQ.N    ??setPumpsCounter_0
//  262 		pumpsCounter = newValue;
        STR      R6,[R4, #+4]
//  263 
//  264 		if (pumpsCounter == PUMPS_TO_PRIME) {
        LDR      R0,[R4, #+4]
        CMP      R0,#+6
        BNE.N    ??setPumpsCounter_1
//  265 			scheduleMsg(&logger, "let's squirt prime pulse %f", pumpsCounter);
        LDR      R2,[R4, #+4]
        LDR.N    R1,??DataTable12_23
        LDR.N    R0,??DataTable12_4
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  266 			pumpsCounter = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+4]
        B.N      ??setPumpsCounter_0
//  267 		} else {
//  268 			scheduleMsg(&logger, "setPumpsCounter %d", pumpsCounter);
??setPumpsCounter_1:
        LDR      R2,[R4, #+4]
        LDR.N    R1,??DataTable12_24
        LDR.N    R0,??DataTable12_4
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  269 		}
//  270 	}
//  271 }
??setPumpsCounter_0:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock23
//  272 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock24 Using cfiCommon0
          CFI Function _ZN18StartupFuelPumping6updateEv
        THUMB
//  273 void StartupFuelPumping::update(DECLARE_ENGINE_PARAMETER_F) {
_ZN18StartupFuelPumping6updateEv:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  274 	if (engine->rpmCalculator.rpm(PASS_ENGINE_PARAMETER_F) == 0) {
        LDR.N    R0,??DataTable12_16
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN13RpmCalculator3rpmEv
        BL       _ZN13RpmCalculator3rpmEv
        CMP      R0,#+0
        BNE.N    ??update_0
//  275 		bool isTpsAbove50 = getTPS(PASS_ENGINE_PARAMETER_F) >= 50;
          CFI FunCall _Z6getTPSv
        BL       _Z6getTPSv
        VLDR.W   S1,??DataTable12_2  ;; 0x42480000
        VCMP.F32 S0,S1
        FMSTAT   
        BLT.N    ??update_1
        MOVS     R5,#+1
        B.N      ??update_2
??update_1:
        MOVS     R5,#+0
//  276 
//  277 		if (this->isTpsAbove50 != isTpsAbove50) {
??update_2:
        LDRB     R0,[R4, #+0]
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R0,R5
        BEQ.N    ??update_3
//  278 			setPumpsCounter(engineConfiguration, pumpsCounter + 1);
        LDR      R0,[R4, #+4]
        ADDS     R2,R0,#+1
        LDR.N    R0,??DataTable12_25
        LDR      R1,[R0, #+0]
        MOVS     R0,R4
          CFI FunCall _ZN18StartupFuelPumping15setPumpsCounterEP22engine_configuration_si
        BL       _ZN18StartupFuelPumping15setPumpsCounterEP22engine_configuration_si
        B.N      ??update_3
//  279 		}
//  280 
//  281 	} else {
//  282 		/**
//  283 		 * Engine is not stopped - not priming pumping mode
//  284 		 */
//  285 		setPumpsCounter(engineConfiguration, 0);
??update_0:
        MOVS     R2,#+0
        LDR.N    R0,??DataTable12_25
        LDR      R1,[R0, #+0]
        MOVS     R0,R4
          CFI FunCall _ZN18StartupFuelPumping15setPumpsCounterEP22engine_configuration_si
        BL       _ZN18StartupFuelPumping15setPumpsCounterEP22engine_configuration_si
//  286 		isTpsAbove50 = false;
        MOVS     R0,#+0
        STRB     R0,[R4, #+0]
//  287 	}
//  288 }
??update_3:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock24

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12:
        DC32     0x4326aaab

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_1:
        DC32     0x43889333

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_2:
        DC32     0x42480000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_3:
        DC32     `?<Constant "engine">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_4:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_5:
        DC32     `?<Constant "fuel">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_6:
        DC32     _Z24invokeEnginePreCalculateP6Engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_7:
        DC32     0x9999999a

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_8:
        DC32     0x40499999

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_9:
        DC32     `?<Constant "turning off %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_10:
        DC32     enginePins

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_11:
        DC32     `?<Constant "knock now=%s/ever=%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_12:
        DC32     `?<Constant "Some pins were turned...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_13:
        DC32     0x280de80

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_14:
        DC32     `?<Constant "engine has STOPPED">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_15:
        DC32     `?<Constant "templog engine has ST...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_16:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_17:
        DC32     _engine+0x1E54

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_18:
        DC32     _engine+0x1E50

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_19:
        DC32     veMap

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_20:
        DC32     0x47ae147b

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_21:
        DC32     0x3f847ae1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_22:
        DC32     afrMap

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_23:
        DC32     `?<Constant "let\\'s squirt prime pu...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_24:
        DC32     `?<Constant "setPumpsCounter %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_25:
        DC32     engineConfiguration

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN7Table2DILi8EEC2Ev
          CFI Block cfiBlock25 Using cfiCommon0
          CFI Function _ZN7Table2DILi8EEC2Ev
        THUMB
// __code __interwork __softfp Table2D<8>::subobject Table2D()
_ZN7Table2DILi8EEC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN7Table2DILi8EEC1Ev
        BL       _ZN7Table2DILi8EEC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock25

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
        SECTION_GROUP _ZZN7Table2DILi8EE7preCalcEPfS1_Es
// __absolute char const <_ZZN7Table2DILi8EE7preCalcEPfS1_Es>[46]
_ZZN7Table2DILi8EE7preCalcEPfS1_Es:
        DATA
        DC8 "preCalc: Same x1 and x2 in interpolate: %f/%f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        SECTION_GROUP _ZZN5Map3DILi16ELi16EE8getValueEffEs
// __absolute char const <_ZZN5Map3DILi16ELi16EE8getValueEffEs>[20]
_ZZN5Map3DILi16ELi16EE8getValueEffEs:
        DATA
        DC8 "map not initialized"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "turning off %s">`:
        DATA
        DC8 "turning off %s"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fuel">`:
        DATA
        DC8 "fuel"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Some pins were turned...">`:
        DATA
        DC8 "Some pins were turned off by 2nd pass watchdog"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "engine has STOPPED">`:
        DATA
        DC8 "engine has STOPPED"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "templog engine has ST...">`:
        DATA
        DC8 "templog engine has STOPPED [%x][%x] [%x][%x] %d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "knock now=%s/ever=%s">`:
        DATA
        DC8 "knock now=%s/ever=%s"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "let\\'s squirt prime pu...">`:
        DATA
        DC8 "let's squirt prime pulse %f"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "setPumpsCounter %d">`:
        DATA
        DC8 "setPumpsCounter %d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "engine">`:
        DATA
        DC8 "engine"
        DC8 0

        END
//  289 
// 
//   260 bytes in section .bss
//     4 bytes in section .init_array
//   288 bytes in section .rodata
// 2 124 bytes in section .text
// 
// 1 852 bytes of CODE  memory (+ 276 bytes shared)
//   220 bytes of CONST memory (+  68 bytes shared)
//   260 bytes of DATA  memory
//
//Errors: none
//Warnings: 2
