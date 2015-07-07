///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:12 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\alg /
//                       o\accel_enrichment.cpp                               /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\alg /
//                       o\accel_enrichment.cpp -lCN                          /
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
//                       accel_enrichment.s                                   /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME accel_enrichment

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z14getEngineLoadTv
        EXTERN _Z6getTPSv
        EXTERN __aeabi_f2d
        EXTERN __aeabi_memset
        EXTERN __iar_FDtest
        EXTERN addConsoleAction
        EXTERN addConsoleActionF
        EXTERN addConsoleActionI
        EXTERN engine
        EXTERN engineConfiguration
        EXTERN scheduleMsg

        PUBLIC _Z19initAccelEnrichmentP7Logging
        PUBLIC _ZN13cyclic_bufferIfE3addEf
        PUBLIC _ZN13cyclic_bufferIfE5baseCEi
        PUBLIC _ZN13cyclic_bufferIfE5clearEv
        PUBLIC _ZN13cyclic_bufferIfE7getSizeEv
        PUBLIC _ZN13cyclic_bufferIfE7setSizeEi
        PUBLIC _ZN13cyclic_bufferIfE8maxValueEi
        PUBLIC _ZN13cyclic_bufferIfEC1Ev
        PUBLIC _ZN13cyclic_bufferIfEC2Ev
        PUBLIC _ZN16AccelEnrichmemnt10onNewValueEf
        PUBLIC _ZN16AccelEnrichmemnt13onEngineCycleEv
        PUBLIC _ZN16AccelEnrichmemnt16getMapEnrichmentEv
        PUBLIC _ZN16AccelEnrichmemnt16getTpsEnrichmentEv
        PUBLIC _ZN16AccelEnrichmemnt16onEngineCycleTpsEv
        PUBLIC _ZN16AccelEnrichmemnt5resetEv
        PUBLIC _ZN16AccelEnrichmemnt8getDeltaEv
        PUBLIC _ZN16AccelEnrichmemntC1Ev
        PUBLIC _ZN16AccelEnrichmemntC2Ev
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\algo\accel_enrichment.cpp
//    1 /**
//    2  * @file    accel_enrichment.cpp
//    3  * @brief   Acceleration enrichment calculator
//    4  *
//    5  * @date Apr 21, 2014
//    6  * @author Dmitry Sidin
//    7  * @author Andrey Belomutskiy, (c) 2012-2015
//    8  */
//    9 
//   10 #include "main.h"
//   11 #include "trigger_central.h"
//   12 #include "accel_enrichment.h"
//   13 #include "engine_state.h"
//   14 #include "engine_math.h"
//   15 #include "signal_executor.h"
//   16 
//   17 EXTERN_ENGINE
//   18 ;
//   19 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   20 static Logging *logger;
logger:
        DS8 4

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _ZN16AccelEnrichmemntC2Ev
        THUMB
// __code __interwork __softfp AccelEnrichmemnt::subobject AccelEnrichmemnt()
_ZN16AccelEnrichmemntC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN16AccelEnrichmemntC1Ev
        BL       _ZN16AccelEnrichmemntC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0
//   21 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _ZN16AccelEnrichmemnt8getDeltaEv
        THUMB
//   22 float AccelEnrichmemnt::getDelta() {
_ZN16AccelEnrichmemnt8getDeltaEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   23 	return cb.maxValue(cb.getSize());
        ADDS     R0,R4,#+4
          CFI FunCall _ZN13cyclic_bufferIfE7getSizeEv
        BL       _ZN13cyclic_bufferIfE7getSizeEv
        MOVS     R1,R0
        ADDS     R0,R4,#+4
          CFI FunCall _ZN13cyclic_bufferIfE8maxValueEi
        BL       _ZN13cyclic_bufferIfE8maxValueEi
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1
//   24 }
//   25 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _ZN16AccelEnrichmemnt16getTpsEnrichmentEv
        THUMB
//   26 floatms_t AccelEnrichmemnt::getTpsEnrichment(DECLARE_ENGINE_PARAMETER_F) {
_ZN16AccelEnrichmemnt16getTpsEnrichmentEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   27 	float d = getDelta();
        MOVS     R0,R4
          CFI FunCall _ZN16AccelEnrichmemnt8getDeltaEv
        BL       _ZN16AccelEnrichmemnt8getDeltaEv
        VMOV.F32 S1,S0
//   28 	if (d > engineConfiguration->tpsAccelEnrichmentThreshold) {
        LDR.N    R0,??DataTable12
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+2188]
        VMOV     S0,R0
        VCMP.F32 S0,S1
        FMSTAT   
        BPL.N    ??getTpsEnrichment_0
//   29 		return d * engineConfiguration->tpsAccelEnrichmentMultiplier;
        LDR.N    R0,??DataTable12
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+2192]
        VMOV     S0,R0
        VMUL.F32 S0,S1,S0
        B.N      ??getTpsEnrichment_1
//   30 	}
//   31 	return 0;
??getTpsEnrichment_0:
        VLDR.W   S0,??DataTable11  ;; 0x0
??getTpsEnrichment_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2
//   32 }
//   33 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _ZN16AccelEnrichmemnt16getMapEnrichmentEv
        THUMB
//   34 float AccelEnrichmemnt::getMapEnrichment(DECLARE_ENGINE_PARAMETER_F) {
_ZN16AccelEnrichmemnt16getMapEnrichmentEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   35 	float d = getDelta();
        MOVS     R0,R4
          CFI FunCall _ZN16AccelEnrichmemnt8getDeltaEv
        BL       _ZN16AccelEnrichmemnt8getDeltaEv
        VMOV.F32 S1,S0
//   36 	if (d > engineConfiguration->mapAccelEnrichmentThreshold) {
        LDR.N    R0,??DataTable12
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+2208]
        VMOV     S0,R0
        VCMP.F32 S0,S1
        FMSTAT   
        BPL.N    ??getMapEnrichment_0
//   37 		return d * engineConfiguration->mapAccelEnrichmentMultiplier;
        LDR.N    R0,??DataTable12
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+2212]
        VMOV     S0,R0
        VMUL.F32 S0,S1,S0
        B.N      ??getMapEnrichment_1
//   38 	}
//   39 //	if (d < engineConfiguration->deaccelEnrichmentThreshold) {
//   40 //		return d * engineConfiguration->deaccelEnrichmentMultiplier;
//   41 //	}
//   42 	return 0;
??getMapEnrichment_0:
        VLDR.W   S0,??DataTable11  ;; 0x0
??getMapEnrichment_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//   43 }
//   44 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _ZN16AccelEnrichmemnt5resetEv
          CFI NoCalls
        THUMB
//   45 void AccelEnrichmemnt::reset() {
//   46 	delta = 0;
_ZN16AccelEnrichmemnt5resetEv:
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//   47 	currentValue = NAN;
        MVNS     R1,#-2147483648
        STR      R1,[R0, #+272]
//   48 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//   49 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _ZN16AccelEnrichmemnt10onNewValueEf
        THUMB
//   50 void AccelEnrichmemnt::onNewValue(float currentValue DECLARE_ENGINE_PARAMETER_S) {
_ZN16AccelEnrichmemnt10onNewValueEf:
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
//   51 	if (!cisnan(this->currentValue)) {
        VLDR     S0,[R4, #+272]
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BEQ.N    ??onNewValue_0
//   52 		delta = currentValue - this->currentValue;
        VLDR     S0,[R4, #+272]
        VSUB.F32 S0,S16,S0
        VSTR     S0,[R4, #0]
//   53 		FuelSchedule *fs = &engine->engineConfiguration2->injectionEvents;
        LDR.N    R0,??DataTable12_1
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+40]
        ADD      R5,R0,#+5888
        ADDS     R5,R5,#+136
//   54 		cb.add(delta * fs->eventsCount);
        ADD      R0,R5,#+5888
        ADDS     R0,R0,#+132
        VLDR     S0,[R0, #0]
        VCVT.F32.S32 S0,S0
        VLDR     S1,[R4, #0]
        VMUL.F32 S0,S0,S1
        ADDS     R0,R4,#+4
          CFI FunCall _ZN13cyclic_bufferIfE3addEf
        BL       _ZN13cyclic_bufferIfE3addEf
//   55 	}
//   56 
//   57 	this->currentValue = currentValue;
??onNewValue_0:
        VSTR     S16,[R4, #+272]
//   58 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock5
//   59 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _ZN16AccelEnrichmemnt16onEngineCycleTpsEv
        THUMB
//   60 void AccelEnrichmemnt::onEngineCycleTps(DECLARE_ENGINE_PARAMETER_F) {
_ZN16AccelEnrichmemnt16onEngineCycleTpsEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   61 	onNewValue(getTPS(PASS_ENGINE_PARAMETER_F) PASS_ENGINE_PARAMETER);
          CFI FunCall _Z6getTPSv
        BL       _Z6getTPSv
        MOVS     R0,R4
          CFI FunCall _ZN16AccelEnrichmemnt10onNewValueEf
        BL       _ZN16AccelEnrichmemnt10onNewValueEf
//   62 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock6
//   63 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _ZN16AccelEnrichmemnt13onEngineCycleEv
        THUMB
//   64 void AccelEnrichmemnt::onEngineCycle(DECLARE_ENGINE_PARAMETER_F) {
_ZN16AccelEnrichmemnt13onEngineCycleEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   65 	onNewValue(getEngineLoadT(PASS_ENGINE_PARAMETER_F) PASS_ENGINE_PARAMETER);
          CFI FunCall _Z14getEngineLoadTv
        BL       _Z14getEngineLoadTv
        MOVS     R0,R4
          CFI FunCall _ZN16AccelEnrichmemnt10onNewValueEf
        BL       _ZN16AccelEnrichmemnt10onNewValueEf
//   66 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock7
//   67 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _ZN16AccelEnrichmemntC1Ev
        THUMB
//   68 AccelEnrichmemnt::AccelEnrichmemnt() {
_ZN16AccelEnrichmemntC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        ADDS     R0,R4,#+4
          CFI FunCall _ZN13cyclic_bufferIfEC1Ev
        BL       _ZN13cyclic_bufferIfEC1Ev
//   69 	reset();
        MOVS     R0,R4
          CFI FunCall _ZN16AccelEnrichmemnt5resetEv
        BL       _ZN16AccelEnrichmemnt5resetEv
//   70 	cb.setSize(4);
        MOVS     R1,#+4
        ADDS     R0,R4,#+4
          CFI FunCall _ZN13cyclic_bufferIfE7setSizeEi
        BL       _ZN13cyclic_bufferIfE7setSizeEi
//   71 }
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock8
//   72 
//   73 #if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
//   74 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z9accelInfov
        THUMB
//   75 static void accelInfo() {
_Z9accelInfov:
        PUSH     {LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+4
        SUB      SP,SP,#+12
          CFI CFA R13+16
//   76 //	scheduleMsg(logger, "MAP accel length=%d", mapInstance.cb.getSize());
//   77 	scheduleMsg(logger, "MAP accel th=%f/mult=%f", engineConfiguration->mapAccelEnrichmentThreshold, engineConfiguration->mapAccelEnrichmentMultiplier);
        LDR.N    R0,??DataTable12
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+2212]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        LDR.N    R0,??DataTable12
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+2208]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable12_2
        LDR.N    R0,??DataTable12_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   78 	scheduleMsg(logger, "MAP decel th=%f/mult=%f", engineConfiguration->decelEnrichmentThreshold, engineConfiguration->decelEnrichmentMultiplier);
        LDR.N    R0,??DataTable12
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+2204]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        LDR.N    R0,??DataTable12
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+2200]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable12_4
        LDR.N    R0,??DataTable12_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   79 
//   80 //	scheduleMsg(logger, "TPS accel length=%d", tpsInstance.cb.getSize());
//   81 	scheduleMsg(logger, "TPS accel th=%f/mult=%f", engineConfiguration->tpsAccelEnrichmentThreshold, engineConfiguration->tpsAccelEnrichmentMultiplier);
        LDR.N    R0,??DataTable12
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+2192]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        LDR.N    R0,??DataTable12
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+2188]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable12_5
        LDR.N    R0,??DataTable12_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   82 }
        POP      {R0-R2,PC}       ;; return
          CFI EndBlock cfiBlock9
//   83 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z14setMapAccelThrf
        THUMB
//   84 static void setMapAccelThr(float value) {
_Z14setMapAccelThrf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//   85 	engineConfiguration->mapAccelEnrichmentThreshold = value;
        LDR.N    R0,??DataTable12
        LDR      R0,[R0, #+0]
        ADDW     R0,R0,#+2208
        VSTR     S16,[R0, #0]
//   86 	accelInfo();
          CFI FunCall _Z9accelInfov
        BL       _Z9accelInfov
//   87 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock10
//   88 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z15setMapAccelMultf
        THUMB
//   89 static void setMapAccelMult(float value) {
_Z15setMapAccelMultf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//   90 	engineConfiguration->mapAccelEnrichmentMultiplier = value;
        LDR.N    R0,??DataTable12
        LDR      R0,[R0, #+0]
        ADDW     R0,R0,#+2212
        VSTR     S16,[R0, #0]
//   91 	accelInfo();
          CFI FunCall _Z9accelInfov
        BL       _Z9accelInfov
//   92 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock11
//   93 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _Z14setTpsAccelThrf
        THUMB
//   94 static void setTpsAccelThr(float value) {
_Z14setTpsAccelThrf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//   95 	engineConfiguration->tpsAccelEnrichmentThreshold = value;
        LDR.N    R0,??DataTable12
        LDR      R0,[R0, #+0]
        ADDW     R0,R0,#+2188
        VSTR     S16,[R0, #0]
//   96 	accelInfo();
          CFI FunCall _Z9accelInfov
        BL       _Z9accelInfov
//   97 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock12
//   98 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z15setTpsAccelMultf
        THUMB
//   99 static void setTpsAccelMult(float value) {
_Z15setTpsAccelMultf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  100 	engineConfiguration->tpsAccelEnrichmentMultiplier = value;
        LDR.N    R0,??DataTable12
        LDR      R0,[R0, #+0]
        ADDW     R0,R0,#+2192
        VSTR     S16,[R0, #0]
//  101 	accelInfo();
          CFI FunCall _Z9accelInfov
        BL       _Z9accelInfov
//  102 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock13
//  103 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _Z11setDecelThrf
        THUMB
//  104 static void setDecelThr(float value) {
_Z11setDecelThrf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  105 	engineConfiguration->decelEnrichmentThreshold = value;
        LDR.N    R0,??DataTable12
        LDR      R0,[R0, #+0]
        ADDW     R0,R0,#+2200
        VSTR     S16,[R0, #0]
//  106 	accelInfo();
          CFI FunCall _Z9accelInfov
        BL       _Z9accelInfov
//  107 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock14
//  108 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function _Z12setDecelMultf
        THUMB
//  109 static void setDecelMult(float value) {
_Z12setDecelMultf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  110 	engineConfiguration->decelEnrichmentMultiplier = value;
        LDR.N    R0,??DataTable12
        LDR      R0,[R0, #+0]
        ADDW     R0,R0,#+2204
        VSTR     S16,[R0, #0]
//  111 	accelInfo();
          CFI FunCall _Z9accelInfov
        BL       _Z9accelInfov
//  112 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock15
//  113 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function _Z14setTpsAccelLeni
        THUMB
//  114 static void setTpsAccelLen(int len) {
_Z14setTpsAccelLeni:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  115 	if (len < 1) {
        CMP      R4,#+1
        BGE.N    ??setTpsAccelLen_0
//  116 		scheduleMsg(logger, "Length shoold be positive");
        LDR.N    R1,??DataTable12_6
        LDR.N    R0,??DataTable12_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  117 		return;
        B.N      ??setTpsAccelLen_1
//  118 	}
//  119 	engine->tpsAccelEnrichment.cb.setSize(len);
??setTpsAccelLen_0:
        MOVS     R1,R4
        LDR.N    R0,??DataTable12_1
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+336
          CFI FunCall _ZN13cyclic_bufferIfE7setSizeEi
        BL       _ZN13cyclic_bufferIfE7setSizeEi
//  120 	accelInfo();
          CFI FunCall _Z9accelInfov
        BL       _Z9accelInfov
//  121 }
??setTpsAccelLen_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock16
//  122 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function _Z14setMapAccelLeni
        THUMB
//  123 static void setMapAccelLen(int len) {
_Z14setMapAccelLeni:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  124 	if (len < 1) {
        CMP      R4,#+1
        BGE.N    ??setMapAccelLen_0
//  125 		scheduleMsg(logger, "Length shoold be positive");
        LDR.N    R1,??DataTable12_6
        LDR.N    R0,??DataTable12_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  126 		return;
        B.N      ??setMapAccelLen_1
//  127 	}
//  128 	engine->mapAccelEnrichment.cb.setSize(len);
??setMapAccelLen_0:
        MOVS     R1,R4
        LDR.N    R0,??DataTable12_1
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+60
          CFI FunCall _ZN13cyclic_bufferIfE7setSizeEi
        BL       _ZN13cyclic_bufferIfE7setSizeEi
//  129 	accelInfo();
          CFI FunCall _Z9accelInfov
        BL       _Z9accelInfov
//  130 }
??setMapAccelLen_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock17

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11:
        DC32     0x0

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function _Z19initAccelEnrichmentP7Logging
        THUMB
_Z19initAccelEnrichmentP7Logging:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        LDR.N    R0,??DataTable12_3
        STR      R4,[R0, #+0]
        LDR.N    R1,??DataTable12_7
        LDR.N    R0,??DataTable12_8
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
        LDR.N    R1,??DataTable12_9
        LDR.N    R0,??DataTable12_10
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
        LDR.N    R1,??DataTable12_11
        LDR.N    R0,??DataTable12_12
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
        LDR.N    R1,??DataTable12_13
        LDR.N    R0,??DataTable12_14
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
        LDR.N    R1,??DataTable12_15
        LDR.N    R0,??DataTable12_16
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
        LDR.N    R1,??DataTable12_17
        LDR.N    R0,??DataTable12_18
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
        LDR.N    R1,??DataTable12_19
        LDR.N    R0,??DataTable12_20
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
        LDR.N    R1,??DataTable12_21
        LDR.N    R0,??DataTable12_22
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
        LDR.N    R1,??DataTable12_23
        LDR.N    R0,??DataTable12_24
          CFI FunCall addConsoleAction
        BL       addConsoleAction
        LDR.N    R0,??DataTable12
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+2196]
          CFI FunCall _Z14setMapAccelLeni
        BL       _Z14setMapAccelLeni
        LDR.N    R0,??DataTable12
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+2184]
          CFI FunCall _Z14setTpsAccelLeni
        BL       _Z14setTpsAccelLeni
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock18

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_1:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_2:
        DC32     `?<Constant "MAP accel th=%f/mult=%f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_3:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_4:
        DC32     `?<Constant "MAP decel th=%f/mult=%f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_5:
        DC32     `?<Constant "TPS accel th=%f/mult=%f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_6:
        DC32     `?<Constant "Length shoold be posi...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_7:
        DC32     _Z14setTpsAccelLeni

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_8:
        DC32     `?<Constant "set_tps_accel_len">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_9:
        DC32     _Z14setTpsAccelThrf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_10:
        DC32     `?<Constant "set_tps_accel_threshold">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_11:
        DC32     _Z15setTpsAccelMultf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_12:
        DC32     `?<Constant "set_tps_accel_multiplier">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_13:
        DC32     _Z14setMapAccelLeni

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_14:
        DC32     `?<Constant "set_map_accel_len">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_15:
        DC32     _Z14setMapAccelThrf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_16:
        DC32     `?<Constant "set_map_accel_threshold">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_17:
        DC32     _Z15setMapAccelMultf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_18:
        DC32     `?<Constant "set_map_accel_multiplier">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_19:
        DC32     _Z11setDecelThrf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_20:
        DC32     `?<Constant "set_decel_threshold">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_21:
        DC32     _Z12setDecelMultf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_22:
        DC32     `?<Constant "set_decel_multiplier">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_23:
        DC32     _Z9accelInfov

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_24:
        DC32     `?<Constant "accelinfo">`

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memset
          CFI Block cfiBlock19 Using cfiCommon0
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
          CFI EndBlock cfiBlock19

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIfEC1Ev
          CFI Block cfiBlock20 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIfEC1Ev
        THUMB
// __code __interwork __softfp cyclic_buffer<float>::cyclic_buffer()
_ZN13cyclic_bufferIfEC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R1,#+64
        MOVS     R0,R4
          CFI FunCall _ZN13cyclic_bufferIfE5baseCEi
        BL       _ZN13cyclic_bufferIfE5baseCEi
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock20

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIfE5baseCEi
          CFI Block cfiBlock21 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIfE5baseCEi
        THUMB
// __interwork __softfp void cyclic_buffer<float>::baseC(int)
_ZN13cyclic_bufferIfE5baseCEi:
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
          CFI FunCall _ZN13cyclic_bufferIfE7setSizeEi
        BL       _ZN13cyclic_bufferIfE7setSizeEi
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock21

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIfE3addEf
          CFI Block cfiBlock22 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIfE3addEf
          CFI NoCalls
        THUMB
// __interwork __vfp void cyclic_buffer<float>::add(float)
_ZN13cyclic_bufferIfE3addEf:
        LDR      R1,[R0, #+256]
        ADDS     R1,R1,#+1
        STR      R1,[R0, #+256]
        LDR      R1,[R0, #+256]
        LDR      R2,[R0, #+264]
        CMP      R1,R2
        BNE.N    ??add_0
        MOVS     R1,#+0
        STR      R1,[R0, #+256]
??add_0:
        LDR      R1,[R0, #+256]
        ADDS     R1,R0,R1, LSL #+2
        VSTR     S0,[R1, #0]
        LDR      R1,[R0, #+260]
        ADDS     R1,R1,#+1
        STR      R1,[R0, #+260]
        BX       LR               ;; return
          CFI EndBlock cfiBlock22

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIfE7setSizeEi
          CFI Block cfiBlock23 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIfE7setSizeEi
        THUMB
// __interwork __softfp void cyclic_buffer<float>::setSize(int)
_ZN13cyclic_bufferIfE7setSizeEi:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R0,R4
          CFI FunCall _ZN13cyclic_bufferIfE5clearEv
        BL       _ZN13cyclic_bufferIfE5clearEv
        CMP      R5,#+64
        BGE.N    ??setSize_0
        MOVS     R0,R5
        B.N      ??setSize_1
??setSize_0:
        MOVS     R0,#+64
??setSize_1:
        STR      R0,[R4, #+264]
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock23

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIfE7getSizeEv
          CFI Block cfiBlock24 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIfE7getSizeEv
          CFI NoCalls
        THUMB
// __interwork __softfp int cyclic_buffer<float>::getSize()
_ZN13cyclic_bufferIfE7getSizeEv:
        LDR      R0,[R0, #+264]
        BX       LR               ;; return
          CFI EndBlock cfiBlock24

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN13cyclic_bufferIfE8maxValueEi
          CFI Block cfiBlock25 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIfE8maxValueEi
          CFI NoCalls
        THUMB
// __interwork __vfp float cyclic_buffer<float>::maxValue(int)
_ZN13cyclic_bufferIfE8maxValueEi:
        PUSH     {R4,R5}
          CFI R5 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        LDR      R2,[R0, #+260]
        CMP      R2,R1
        BGE.N    ??maxValue_0
        LDR      R2,[R0, #+260]
        MOVS     R1,R2
??maxValue_0:
        LDR      R2,[R0, #+256]
        VLDR.W   S0,??maxValue_1  ;; 0xc9f42400
        MOVS     R3,#+0
??maxValue_2:
        CMP      R3,R1
        BGE.N    ??maxValue_3
        SUBS     R4,R2,R3
??maxValue_4:
        CMP      R4,#+0
        BPL.N    ??maxValue_5
        LDR      R5,[R0, #+264]
        ADDS     R4,R5,R4
        B.N      ??maxValue_4
??maxValue_5:
        ADDS     R5,R0,R4, LSL #+2
        VLDR     S1,[R5, #0]
        VCMP.F32 S0,S1
        FMSTAT   
        BPL.N    ??maxValue_6
        LDR      R4,[R0, R4, LSL #+2]
        VMOV     S0,R4
??maxValue_6:
        ADDS     R3,R3,#+1
        B.N      ??maxValue_2
??maxValue_3:
        POP      {R4,R5}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
        Nop      
        DATA
??maxValue_1:
        DC32     0xc9f42400
          CFI EndBlock cfiBlock25

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIfE5clearEv
          CFI Block cfiBlock26 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIfE5clearEv
        THUMB
// __interwork __softfp void cyclic_buffer<float>::clear()
_ZN13cyclic_bufferIfE5clearEv:
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
          CFI EndBlock cfiBlock26

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MAP accel th=%f/mult=%f">`:
        DATA
        DC8 "MAP accel th=%f/mult=%f"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MAP decel th=%f/mult=%f">`:
        DATA
        DC8 "MAP decel th=%f/mult=%f"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TPS accel th=%f/mult=%f">`:
        DATA
        DC8 "TPS accel th=%f/mult=%f"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Length shoold be posi...">`:
        DATA
        DC8 "Length shoold be positive"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_tps_accel_len">`:
        DATA
        DC8 "set_tps_accel_len"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_tps_accel_threshold">`:
        DATA
        DC8 "set_tps_accel_threshold"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_tps_accel_multiplier">`:
        DATA
        DC8 "set_tps_accel_multiplier"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_map_accel_len">`:
        DATA
        DC8 "set_map_accel_len"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_map_accel_threshold">`:
        DATA
        DC8 "set_map_accel_threshold"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_map_accel_multiplier">`:
        DATA
        DC8 "set_map_accel_multiplier"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_decel_threshold">`:
        DATA
        DC8 "set_decel_threshold"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_decel_multiplier">`:
        DATA
        DC8 "set_decel_multiplier"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "accelinfo">`:
        DATA
        DC8 "accelinfo"
        DC8 0, 0

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIfEC2Ev
          CFI Block cfiBlock27 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIfEC2Ev
        THUMB
// __code __interwork __softfp cyclic_buffer<float>::subobject cyclic_buffer()
_ZN13cyclic_bufferIfEC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN13cyclic_bufferIfEC1Ev
        BL       _ZN13cyclic_bufferIfEC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock27

        END
//  131 
//  132 void initAccelEnrichment(Logging *sharedLogger) {
//  133 	logger = sharedLogger;
//  134 	addConsoleActionI("set_tps_accel_len", setTpsAccelLen);
//  135 	addConsoleActionF("set_tps_accel_threshold", setTpsAccelThr);
//  136 	addConsoleActionF("set_tps_accel_multiplier", setTpsAccelMult);
//  137 
//  138 	addConsoleActionI("set_map_accel_len", setMapAccelLen);
//  139 	addConsoleActionF("set_map_accel_threshold", setMapAccelThr);
//  140 	addConsoleActionF("set_map_accel_multiplier", setMapAccelMult);
//  141 	addConsoleActionF("set_decel_threshold", setDecelThr);
//  142 	addConsoleActionF("set_decel_multiplier", setDecelMult);
//  143 	addConsoleAction("accelinfo", accelInfo);
//  144 
//  145 	setMapAccelLen(engineConfiguration->mapAccelLength);
//  146 	setTpsAccelLen(engineConfiguration->tpsAccelLength);
//  147 }
//  148 #endif /* ! EFI_UNIT_TEST */
// 
//     4 bytes in section .bss
//   300 bytes in section .rodata
// 1 196 bytes in section .text
// 
// 924 bytes of CODE  memory (+ 272 bytes shared)
// 300 bytes of CONST memory
//   4 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
