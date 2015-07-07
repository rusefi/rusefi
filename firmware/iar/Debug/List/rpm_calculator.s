///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:59 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\tri /
//                       gger\rpm_calculator.cpp                              /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\tri /
//                       gger\rpm_calculator.cpp -lCN                         /
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
//                       rpm_calculator.s                                     /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME rpm_calculator

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z12scheduleTaskPKcP12scheduling_siPFvPvES3_
        EXTERN _Z23addTriggerEventListenerPFv15trigger_event_ejEPKcP6Engine
        EXTERN _Z9scAddDataff
        EXTERN _ZN12scheduling_sC1Ev
        EXTERN _ZN9WaveChart18addWaveChartEvent3EPKcS1_
        EXTERN __aeabi_d2f
        EXTERN __aeabi_ddiv
        EXTERN __aeabi_dmul
        EXTERN __aeabi_i2d
        EXTERN __aeabi_l2f
        EXTERN __aeabi_ldivmod
        EXTERN __aeabi_vec_ctor_nocookie_nodtor
        EXTERN __iar_FDtest
        EXTERN _engine
        EXTERN boardConfiguration
        EXTERN engine
        EXTERN engineConfiguration
        EXTERN firmwareError
        EXTERN getRemainingStack
        EXTERN getTimeNowNt
        EXTERN itoa10
        EXTERN persistentState
        EXTERN rlist
        EXTERN scheduleMsg
        EXTERN waveChart

        PUBLIC _Z10isCrankingv
        PUBLIC _Z11isCrankingEP6Engine
        PUBLIC _Z15scheduleByAngleiP12scheduling_sfPFvPvES1_P13RpmCalculator
        PUBLIC _Z17initRpmCalculatorP7LoggingP6Engine
        PUBLIC _Z20getCrankshaftAngleNtx
        PUBLIC _Z20getRevolutionCounterv
        PUBLIC _Z24rpmShaftPositionCallback15trigger_event_ej
        PUBLIC _ZN13RpmCalculator11setRpmValueEi
        PUBLIC _ZN13RpmCalculator16onNewEngineCycleEv
        PUBLIC _ZN13RpmCalculator18getRpmAccelerationEv
        PUBLIC _ZN13RpmCalculator20getRevolutionCounterEv
        PUBLIC _ZN13RpmCalculator30getRevolutionCounterSinceStartEv
        PUBLIC _ZN13RpmCalculator3rpmEv
        PUBLIC _ZN13RpmCalculator9isRunningEv
        PUBLIC _ZN13RpmCalculatorC1Ev
        PUBLIC _ZN13RpmCalculatorC2Ev
        PUBLIC notRunningPrev
        PUBLIC notRunnintNow
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\trigger\rpm_calculator.cpp
//    1 /**
//    2  * @file    rpm_calculator.cpp
//    3  * @brief   RPM calculator
//    4  *
//    5  * Here we listen to position sensor events in order to figure our if engine is currently running or not.
//    6  * Actual getRpm() is calculated once per crankshaft revolution, based on the amount of time passed
//    7  * since the start of previous shaft revolution.
//    8  *
//    9  * @date Jan 1, 2013
//   10  * @author Andrey Belomutskiy, (c) 2012-2015
//   11  */
//   12 
//   13 #include "main.h"
//   14 #include "rpm_calculator.h"
//   15 
//   16 #if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
//   17 
//   18 #include "trigger_central.h"
//   19 #include "engine_configuration.h"
//   20 #include "engine_math.h"
//   21 
//   22 #if EFI_PROD_CODE
//   23 #include "rfiutil.h"
//   24 #include "engine.h"
//   25 #endif
//   26 
//   27 #if ! EFI_UNIT_TEST
//   28 #include <sensor_chart.h>
//   29 #endif
//   30 
//   31 #include "efilib2.h"
//   32 
//   33 #if EFI_WAVE_CHART
//   34 #include "wave_chart.h"
//   35 extern WaveChart waveChart;
//   36 #endif /* EFI_WAVE_CHART */
//   37 
//   38 EXTERN_ENGINE
//   39 ;
//   40 

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   41 efitime_t notRunnintNow;
notRunnintNow:
        DS8 8

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   42 efitime_t notRunningPrev;
notRunningPrev:
        DS8 8
//   43 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   44 static Logging * logger;
logger:
        DS8 4
//   45 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _ZN13RpmCalculatorC2Ev
        THUMB
// __code __interwork __softfp RpmCalculator::subobject RpmCalculator()
_ZN13RpmCalculatorC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN13RpmCalculatorC1Ev
        BL       _ZN13RpmCalculatorC1Ev
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
        MOVS     R3,#+2
        MOVS     R2,#+24
        LDR.W    R1,??DataTable14_3
        LDR.W    R0,??DataTable14_4
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock1

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _ZN13RpmCalculatorC1Ev
        THUMB
//   46 RpmCalculator::RpmCalculator() {
_ZN13RpmCalculatorC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   47 #if !EFI_PROD_CODE
//   48 	mockRpm = MOCK_UNDEFINED;
//   49 #endif
//   50 	rpmValue = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+0]
//   51 	setRpmValue(0);
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall _ZN13RpmCalculator11setRpmValueEi
        BL       _ZN13RpmCalculator11setRpmValueEi
//   52 
//   53 	// we need this initial to have not_running at first invocation
//   54 	lastRpmEventTimeNt = (efitime_t) -10 * US2NT(US_PER_SECOND_LL);
        LDR.W    R0,??DataTable14_5  ;; 0x9bdd3c00
        MOVS     R1,#-1
        STRD     R0,R1,[R4, #+16]
//   55 	revolutionCounterSinceStart = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+28]
//   56 	revolutionCounterSinceBoot = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+24]
//   57 
//   58 	lastRpmEventTimeNt = 0;
        MOVS     R0,#+0
        MOVS     R1,#+0
        STRD     R0,R1,[R4, #+16]
//   59 	oneDegreeUs = NAN;
        MVNS     R0,#-2147483648
        STR      R0,[R4, #+8]
//   60 }
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2
//   61 
//   62 /**
//   63  * @return true if there was a full shaft revolution within the last second
//   64  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _ZN13RpmCalculator9isRunningEv
        THUMB
//   65 bool RpmCalculator::isRunning(DECLARE_ENGINE_PARAMETER_F) {
_ZN13RpmCalculator9isRunningEv:
        PUSH     {R4,R6,R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//   66 	efitick_t nowNt = getTimeNowNt();
          CFI FunCall getTimeNowNt
        BL       getTimeNowNt
        MOVS     R2,R0
        MOVS     R3,R1
//   67 	if (engine->stopEngineRequestTimeNt != 0) {
        LDR.W    R0,??DataTable14_6
        LDR      R0,[R0, #+0]
        LDRD     R6,R7,[R0, #+48]
        CMP      R7,#+0
        BNE.N    ??isRunning_0
        CMP      R6,#+0
        BEQ.N    ??isRunning_1
//   68 		if (nowNt
//   69 				- engine->stopEngineRequestTimeNt< 3 * US2NT(US_PER_SECOND_LL)) {
??isRunning_0:
        LDR.W    R0,??DataTable14_6
        LDR      R0,[R0, #+0]
        LDRD     R6,R7,[R0, #+48]
        SUBS     R0,R2,R6
        SBCS     R1,R3,R7
        LDR.W    R6,??DataTable14_7  ;; 0x1e0a6e00
        MOVS     R7,#+0
        CMP      R1,R7
        BGT.N    ??isRunning_1
        BLT.N    ??isRunning_2
        CMP      R0,R6
        BCS.N    ??isRunning_1
//   70 			return false;
??isRunning_2:
        MOVS     R0,#+0
        B.N      ??isRunning_3
//   71 		}
//   72 	}
//   73 	/**
//   74 	 * note that the result of this subtraction could be negative, that would happen if
//   75 	 * we have a trigger event between the time we've invoked 'getTimeNow' and here
//   76 	 */
//   77 	bool_t result = nowNt - lastRpmEventTimeNt < US2NT(US_PER_SECOND_LL);
??isRunning_1:
        LDRD     R0,R1,[R4, #+16]
        SUBS     R0,R2,R0
        SBCS     R1,R3,R1
        LDR.W    R6,??DataTable14_8  ;; 0xa037a00
        MOVS     R7,#+0
        CMP      R1,R7
        BGT.N    ??isRunning_4
        BLT.N    ??isRunning_5
        CMP      R0,R6
        BCS.N    ??isRunning_4
??isRunning_5:
        MOVS     R0,#+1
        B.N      ??isRunning_6
??isRunning_4:
        MOVS     R0,#+0
//   78 	if (!result) {
??isRunning_6:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+0
        BNE.N    ??isRunning_7
//   79 		notRunnintNow = nowNt;
        LDR.W    R1,??DataTable14_9
        STRD     R2,R3,[R1, #+0]
//   80 		notRunningPrev = lastRpmEventTimeNt;
        LDRD     R6,R7,[R4, #+16]
        LDR.W    R1,??DataTable14_10
        STRD     R6,R7,[R1, #+0]
//   81 	}
//   82 	return result;
??isRunning_7:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
??isRunning_3:
        POP      {R4,R6,R7,PC}    ;; return
          CFI EndBlock cfiBlock3
//   83 }
//   84 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _ZN13RpmCalculator11setRpmValueEi
          CFI NoCalls
        THUMB
//   85 void RpmCalculator::setRpmValue(int value) {
//   86 	previousRpmValue = rpmValue;
_ZN13RpmCalculator11setRpmValueEi:
        LDR      R2,[R0, #+0]
        STR      R2,[R0, #+4]
//   87 	rpmValue = value;
        STR      R1,[R0, #+0]
//   88 	if (rpmValue <= 0) {
        LDR      R2,[R0, #+0]
        CMP      R2,#+1
        BGE.N    ??setRpmValue_0
//   89 		oneDegreeUs = NAN;
        MVNS     R2,#-2147483648
        STR      R2,[R0, #+8]
        B.N      ??setRpmValue_1
//   90 	} else {
//   91 		oneDegreeUs = getOneDegreeTimeUs(rpmValue);
??setRpmValue_0:
        VLDR     S0,[R0, #0]
        VCVT.F32.S32 S0,S0
        VLDR.W   S1,??DataTable12  ;; 0x4822c2ab
        VDIV.F32 S0,S1,S0
        VSTR     S0,[R0, #+8]
//   92 	}
//   93 }
??setRpmValue_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//   94 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _ZN13RpmCalculator16onNewEngineCycleEv
          CFI NoCalls
        THUMB
//   95 void RpmCalculator::onNewEngineCycle() {
//   96 	revolutionCounterSinceBoot++;
_ZN13RpmCalculator16onNewEngineCycleEv:
        LDR      R1,[R0, #+24]
        ADDS     R1,R1,#+1
        STR      R1,[R0, #+24]
//   97 	revolutionCounterSinceStart++;
        LDR      R1,[R0, #+28]
        ADDS     R1,R1,#+1
        STR      R1,[R0, #+28]
//   98 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock5
//   99 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _ZN13RpmCalculator20getRevolutionCounterEv
          CFI NoCalls
        THUMB
//  100 uint32_t RpmCalculator::getRevolutionCounter(void) {
//  101 	return revolutionCounterSinceBoot;
_ZN13RpmCalculator20getRevolutionCounterEv:
        LDR      R0,[R0, #+24]
        BX       LR               ;; return
          CFI EndBlock cfiBlock6
//  102 }
//  103 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _ZN13RpmCalculator30getRevolutionCounterSinceStartEv
          CFI NoCalls
        THUMB
//  104 uint32_t RpmCalculator::getRevolutionCounterSinceStart(void) {
//  105 	return revolutionCounterSinceStart;
_ZN13RpmCalculator30getRevolutionCounterSinceStartEv:
        LDR      R0,[R0, #+28]
        BX       LR               ;; return
          CFI EndBlock cfiBlock7
//  106 }
//  107 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _ZN13RpmCalculator18getRpmAccelerationEv
        THUMB
//  108 float RpmCalculator::getRpmAcceleration() {
_ZN13RpmCalculator18getRpmAccelerationEv:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R6,R0
//  109 	return 1.0 * previousRpmValue / rpmValue;
        LDR      R0,[R6, #+4]
          CFI FunCall __aeabi_i2d
        BL       __aeabi_i2d
        MOVS     R2,#+0
        LDR.W    R3,??DataTable14_11  ;; 0x3ff00000
          CFI FunCall __aeabi_dmul
        BL       __aeabi_dmul
        MOVS     R4,R0
        MOVS     R5,R1
        LDR      R0,[R6, #+0]
          CFI FunCall __aeabi_i2d
        BL       __aeabi_i2d
        MOVS     R2,R0
        MOVS     R3,R1
        MOVS     R0,R4
        MOVS     R1,R5
          CFI FunCall __aeabi_ddiv
        BL       __aeabi_ddiv
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        VMOV     S0,R0
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock8
//  110 }
//  111 
//  112 /**
//  113  * WARNING: this is a heavy method because 'getRpm()' is relatively heavy
//  114  *
//  115  * @return -1 in case of isNoisySignal(), current RPM otherwise
//  116  */
//  117 // todo: migrate to float return result or add a float version? this would have with calculations
//  118 // todo: add a version which does not check time & saves time? need to profile

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _ZN13RpmCalculator3rpmEv
        THUMB
//  119 int RpmCalculator::rpm(DECLARE_ENGINE_PARAMETER_F) {
_ZN13RpmCalculator3rpmEv:
        PUSH     {R2-R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+16
        MOVS     R4,R0
//  120 #if !EFI_PROD_CODE
//  121 	if (mockRpm != MOCK_UNDEFINED)
//  122 	return mockRpm;
//  123 #endif
//  124 	if (!isRunning(PASS_ENGINE_PARAMETER_F)) {
        MOVS     R0,R4
          CFI FunCall _ZN13RpmCalculator9isRunningEv
        BL       _ZN13RpmCalculator9isRunningEv
        CMP      R0,#+0
        BNE.N    ??rpm_0
//  125 		revolutionCounterSinceStart = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+28]
//  126 		if (rpmValue != 0) {
        LDR      R0,[R4, #+0]
        CMP      R0,#+0
        BEQ.N    ??rpm_0
//  127 			rpmValue = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+0]
//  128 			scheduleMsg(logger,
//  129 					"templog rpm=0 since not running [%x][%x] [%x][%x]",
//  130 					(int) (notRunnintNow >> 32), (int) notRunnintNow,
//  131 					(int) (notRunningPrev >> 32), (int) notRunningPrev);
        LDR.N    R2,??DataTable14_10
        LDRD     R0,R1,[R2, #+0]
        STR      R0,[SP, #+4]
        LDR.N    R2,??DataTable14_10
        LDRD     R0,R1,[R2, #+0]
        STR      R1,[SP, #+0]
        LDR.N    R2,??DataTable14_9
        LDRD     R0,R1,[R2, #+0]
        MOVS     R3,R0
        LDR.N    R2,??DataTable14_9
        LDRD     R0,R1,[R2, #+0]
        MOVS     R2,R1
        LDR.N    R1,??DataTable14_12
        LDR.N    R0,??DataTable14_13
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  132 		}
//  133 	}
//  134 	return rpmValue;
??rpm_0:
        LDR      R0,[R4, #+0]
        POP      {R1,R2,R4,PC}    ;; return
          CFI EndBlock cfiBlock9
//  135 }
//  136 
//  137 #if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z11isCrankingEP6Engine
        THUMB
//  138 bool isCrankingE(Engine *engine) {
_Z11isCrankingEP6Engine:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  139 	int rpm = getRpmE(engine);
        MOVS     R0,R4
          CFI FunCall _ZN13RpmCalculator3rpmEv
        BL       _ZN13RpmCalculator3rpmEv
//  140 	return isCrankingR(rpm);
        CMP      R0,#+1
        BLT.N    ??isCrankingE_0
        LDR.N    R1,??DataTable14_14
        LDRSH    R1,[R1, #+104]
        CMP      R0,R1
        BGE.N    ??isCrankingE_0
        MOVS     R0,#+1
        B.N      ??isCrankingE_1
??isCrankingE_0:
        MOVS     R0,#+0
??isCrankingE_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock10
//  141 }
//  142 
//  143 /**
//  144  * WARNING: this is a heavy method because 'getRpm()' is relatively heavy
//  145  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z10isCrankingv
        THUMB
//  146 bool isCranking(void) {
_Z10isCrankingv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  147 	return isCrankingE(engine);
        LDR.N    R0,??DataTable14_6
        LDR      R0,[R0, #+0]
          CFI FunCall _Z11isCrankingEP6Engine
        BL       _Z11isCrankingEP6Engine
        POP      {R1,PC}          ;; return
          CFI EndBlock cfiBlock11
//  148 }
//  149 #endif
//  150 
//  151 /**
//  152  * @brief Shaft position callback used by RPM calculation logic.
//  153  *
//  154  * This callback should always be the first of trigger callbacks because other callbacks depend of values
//  155  * updated here.
//  156  * This callback is invoked on interrupt thread.
//  157  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _Z24rpmShaftPositionCallback15trigger_event_ej
        THUMB
//  158 void rpmShaftPositionCallback(trigger_event_e ckpSignalType,
//  159 		uint32_t index DECLARE_ENGINE_PARAMETER_S) {
_Z24rpmShaftPositionCallback15trigger_event_ej:
        PUSH     {R4-R11,LR}
          CFI R14 Frame(CFA, -4)
          CFI R11 Frame(CFA, -8)
          CFI R10 Frame(CFA, -12)
          CFI R9 Frame(CFA, -16)
          CFI R8 Frame(CFA, -20)
          CFI R7 Frame(CFA, -24)
          CFI R6 Frame(CFA, -28)
          CFI R5 Frame(CFA, -32)
          CFI R4 Frame(CFA, -36)
          CFI CFA R13+36
        SUB      SP,SP,#+12
          CFI CFA R13+48
        MOV      R11,R0
        MOV      R10,R1
//  160 	RpmCalculator *rpmState = &engine->rpmCalculator;
        LDR.N    R0,??DataTable14_6
        LDR      R6,[R0, #+0]
//  161 	efitick_t nowNt = getTimeNowNt();
          CFI FunCall getTimeNowNt
        BL       getTimeNowNt
        MOVS     R4,R0
        MOVS     R5,R1
//  162 	engine->m.beforeRpmCb = GET_TIMESTAMP();
        MOVW     R0,#+9224
        LDR.N    R1,??DataTable14_6
        LDR      R1,[R1, #+0]
        LDR.N    R2,??DataTable14_15  ;; 0xe0001004
        LDR      R2,[R2, #+0]
        STR      R2,[R0, R1]
//  163 #if EFI_PROD_CODE
//  164 	efiAssertVoid(getRemainingStack(chThdSelf()) > 256, "lowstck#2z");
        LDR.N    R0,??DataTable14_16
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+256
        BGT.N    ??rpmShaftPositionCallback_0
        LDR.N    R0,??DataTable14_17
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??rpmShaftPositionCallback_1
//  165 #endif
//  166 
//  167 	if (index != 0) {
??rpmShaftPositionCallback_0:
        CMP      R10,#+0
        BEQ.N    ??rpmShaftPositionCallback_2
//  168 #if EFI_ANALOG_CHART || defined(__DOXYGEN__)
//  169 		if (boardConfiguration->sensorChartMode == SC_TRIGGER)
        LDR.N    R0,??DataTable14_18
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+784]
        CMP      R0,#+1
        BNE.N    ??rpmShaftPositionCallback_3
//  170 			scAddData(getCrankshaftAngleNt(nowNt PASS_ENGINE_PARAMETER),
//  171 					1000 * ckpSignalType + index);
        MOVS     R0,R4
        MOVS     R1,R5
          CFI FunCall _Z20getCrankshaftAngleNtx
        BL       _Z20getCrankshaftAngleNtx
        UXTB     R11,R11          ;; ZeroExt  R11,R11,#+24,#+24
        MOV      R0,#+1000
        MLA      R0,R0,R11,R10
        VMOV     S1,R0
        VCVT.F32.U32 S1,S1
          CFI FunCall _Z9scAddDataff
        BL       _Z9scAddDataff
//  172 #endif
//  173 		return;
??rpmShaftPositionCallback_3:
        B.N      ??rpmShaftPositionCallback_1
//  174 	}
//  175 
//  176 	bool hadRpmRecently = rpmState->isRunning(PASS_ENGINE_PARAMETER_F);
??rpmShaftPositionCallback_2:
        MOVS     R0,R6
          CFI FunCall _ZN13RpmCalculator9isRunningEv
        BL       _ZN13RpmCalculator9isRunningEv
        MOVS     R7,R0
//  177 
//  178 	if (hadRpmRecently) {
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+0
        BEQ.N    ??rpmShaftPositionCallback_4
//  179 		efitime_t diffNt = nowNt - rpmState->lastRpmEventTimeNt;
        LDRD     R0,R1,[R6, #+16]
        SUBS     R0,R4,R0
        SBCS     R1,R5,R1
        STRD     R0,R1,[SP, #+0]
//  180 		/**
//  181 		 * Four stroke cycle is two crankshaft revolutions
//  182 		 *
//  183 		 * We always do '* 2' because the event signal is already adjusted to 'per engine cycle'
//  184 		 * and each revolution of crankshaft consists of two engine cycles revolutions
//  185 		 *
//  186 		 */
//  187 		if (diffNt == 0) {
        LDRD     R0,R1,[SP, #+0]
        CMP      R1,#+0
        BNE.N    ??rpmShaftPositionCallback_5
        CMP      R0,#+0
        BNE.N    ??rpmShaftPositionCallback_5
//  188 			rpmState->setRpmValue(NOISY_RPM);
        MOVS     R1,#-1
        MOVS     R0,R6
          CFI FunCall _ZN13RpmCalculator11setRpmValueEi
        BL       _ZN13RpmCalculator11setRpmValueEi
        B.N      ??rpmShaftPositionCallback_4
//  189 		} else {
//  190 			int mult = engineConfiguration->engineCycle / 360;
??rpmShaftPositionCallback_5:
        LDR.N    R0,??DataTable14_19
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+416]
        MOV      R1,#+360
        SDIV     R8,R0,R1
//  191 			int rpm = (int) (60 * US2NT(US_PER_SECOND_LL) * mult / diffNt);
        ASRS     R9,R8,#+31
        LDR.N    R2,??DataTable14_20  ;; 0x58d09800
        MOVS     R3,#+2
        UMULL    R0,R1,R2,R8
        MLA      R1,R2,R9,R1
        MLA      R1,R3,R8,R1
        LDRD     R2,R3,[SP, #+0]
          CFI FunCall __aeabi_ldivmod
        BL       __aeabi_ldivmod
        MOV      R9,R0
//  192 			rpmState->setRpmValue(rpm > UNREALISTIC_RPM ? NOISY_RPM : rpm);
        MOVW     R0,#+30001
        CMP      R9,R0
        BLT.N    ??rpmShaftPositionCallback_6
        MOVS     R1,#-1
        B.N      ??rpmShaftPositionCallback_7
??rpmShaftPositionCallback_6:
        MOV      R1,R9
??rpmShaftPositionCallback_7:
        MOVS     R0,R6
          CFI FunCall _ZN13RpmCalculator11setRpmValueEi
        BL       _ZN13RpmCalculator11setRpmValueEi
//  193 		}
//  194 	}
//  195 	rpmState->onNewEngineCycle();
??rpmShaftPositionCallback_4:
        MOVS     R0,R6
          CFI FunCall _ZN13RpmCalculator16onNewEngineCycleEv
        BL       _ZN13RpmCalculator16onNewEngineCycleEv
//  196 	rpmState->lastRpmEventTimeNt = nowNt;
        STRD     R4,R5,[R6, #+16]
//  197 #if EFI_ANALOG_CHART || defined(__DOXYGEN__)
//  198 	if (boardConfiguration->sensorChartMode == SC_TRIGGER)
        LDR.N    R0,??DataTable14_18
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+784]
        CMP      R0,#+1
        BNE.N    ??rpmShaftPositionCallback_8
//  199 		scAddData(getCrankshaftAngleNt(nowNt PASS_ENGINE_PARAMETER), index);
        MOVS     R0,R4
        MOVS     R1,R5
          CFI FunCall _Z20getCrankshaftAngleNtx
        BL       _Z20getCrankshaftAngleNtx
        VMOV     S1,R10
        VCVT.F32.U32 S1,S1
          CFI FunCall _Z9scAddDataff
        BL       _Z9scAddDataff
//  200 #endif
//  201 	engine->m.rpmCbTime = GET_TIMESTAMP() - engine->m.beforeRpmCb;
??rpmShaftPositionCallback_8:
        LDR.N    R0,??DataTable14_15  ;; 0xe0001004
        LDR      R0,[R0, #+0]
        MOVW     R1,#+9224
        LDR.N    R2,??DataTable14_6
        LDR      R2,[R2, #+0]
        LDR      R1,[R1, R2]
        SUBS     R0,R0,R1
        MOVW     R1,#+9228
        LDR.N    R2,??DataTable14_6
        LDR      R2,[R2, #+0]
        STR      R0,[R1, R2]
//  202 }
??rpmShaftPositionCallback_1:
        POP      {R0-R2,R4-R11,PC}  ;; return
          CFI EndBlock cfiBlock12
//  203 

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//  204 static scheduling_s tdcScheduler[2];
tdcScheduler:
        DS8 48
//  205 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  206 static char rpmBuffer[10];
rpmBuffer:
        DS8 12
//  207 
//  208 #if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
//  209 /**
//  210  * This callback has nothing to do with actual engine control, it just sends a Top Dead Center mark to the dev console
//  211  * digital sniffer.
//  212  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z13onTdcCallbackv
        THUMB
//  213 static void onTdcCallback(void) {
_Z13onTdcCallbackv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  214 	itoa10(rpmBuffer, getRpm());
        LDR.N    R0,??DataTable14_6
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN13RpmCalculator3rpmEv
        BL       _ZN13RpmCalculator3rpmEv
        MOVS     R1,R0
        LDR.N    R0,??DataTable14_21
          CFI FunCall itoa10
        BL       itoa10
//  215 	addWaveChartEvent(TOP_DEAD_CENTER_MESSAGE, (char* ) rpmBuffer);
        LDR.N    R2,??DataTable14_21
        ADR.N    R1,??DataTable14  ;; "r"
        LDR.N    R0,??DataTable14_22
          CFI FunCall _ZN9WaveChart18addWaveChartEvent3EPKcS1_
        BL       _ZN9WaveChart18addWaveChartEvent3EPKcS1_
//  216 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock13
//  217 
//  218 /**
//  219  * This trigger callback schedules the actual physical TDC callback in relation to trigger synchronization point.
//  220  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _Z15tdcMarkCallback15trigger_event_ej
        THUMB
//  221 static void tdcMarkCallback(trigger_event_e ckpSignalType,
//  222 		uint32_t index0 DECLARE_ENGINE_PARAMETER_S) {
_Z15tdcMarkCallback15trigger_event_ej:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        SUB      SP,SP,#+8
          CFI CFA R13+32
        MOVS     R4,R0
        MOVS     R5,R1
//  223 	(void) ckpSignalType;
//  224 	bool isTriggerSynchronizationPoint = index0 == 0;
        CMP      R5,#+0
        BNE.N    ??tdcMarkCallback_0
        MOVS     R6,#+1
        B.N      ??tdcMarkCallback_1
??tdcMarkCallback_0:
        MOVS     R6,#+0
//  225 	if (isTriggerSynchronizationPoint
//  226 			&& engineConfiguration->isEngineChartEnabled) {
??tdcMarkCallback_1:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+0
        BEQ.N    ??tdcMarkCallback_2
        LDR.N    R0,??DataTable14_19
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+5
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??tdcMarkCallback_2
//  227 		int revIndex2 = engine->rpmCalculator.getRevolutionCounter() % 2;
        LDR.N    R0,??DataTable14_6
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN13RpmCalculator20getRevolutionCounterEv
        BL       _ZN13RpmCalculator20getRevolutionCounterEv
        ANDS     R7,R0,#0x1
//  228 		int rpm = getRpm();
        LDR.N    R0,??DataTable14_6
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN13RpmCalculator3rpmEv
        BL       _ZN13RpmCalculator3rpmEv
        MOV      R8,R0
//  229 		// todo: use event-based scheduling, not just time-based scheduling
//  230 		if (isValidRpm(rpm)) {
        CMP      R8,#+1
        BLT.N    ??tdcMarkCallback_2
        MOVW     R0,#+30000
        CMP      R8,R0
        BGE.N    ??tdcMarkCallback_2
//  231 			scheduleByAngle(rpm, &tdcScheduler[revIndex2], tdcPosition(),
//  232 					(schfunc_t) onTdcCallback, NULL, &engine->rpmCalculator);
        LDR.N    R0,??DataTable14_6
        LDR      R0,[R0, #+0]
        STR      R0,[SP, #+0]
        MOVS     R3,#+0
        LDR.N    R2,??DataTable14_23
        LDR.N    R0,??DataTable14_24
        VLDR     S0,[R0, #0]
        LDR.N    R0,??DataTable14_14
        VLDR     S1,[R0, #+468]
        VADD.F32 S0,S0,S1
        MOVS     R0,#+24
        LDR.N    R1,??DataTable14_4
        MLA      R1,R0,R7,R1
        MOV      R0,R8
          CFI FunCall _Z15scheduleByAngleiP12scheduling_sfPFvPvES1_P13RpmCalculator
        BL       _Z15scheduleByAngleiP12scheduling_sfPFvPvES1_P13RpmCalculator
//  233 		}
//  234 	}
//  235 }
??tdcMarkCallback_2:
        POP      {R0,R1,R4-R8,PC}  ;; return
          CFI EndBlock cfiBlock14
//  236 #endif
//  237 
//  238 #if EFI_PROD_CODE || EFI_SIMULATOR

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function _Z20getRevolutionCounterv
        THUMB
//  239 int getRevolutionCounter() {
_Z20getRevolutionCounterv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  240 	return engine->rpmCalculator.getRevolutionCounter();
        LDR.N    R0,??DataTable14_6
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN13RpmCalculator20getRevolutionCounterEv
        BL       _ZN13RpmCalculator20getRevolutionCounterEv
        POP      {R1,PC}          ;; return
          CFI EndBlock cfiBlock15
//  241 }
//  242 #endif
//  243 
//  244 /**
//  245  * @return Current crankshaft angle, 0 to 720 for four-stroke
//  246  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function _Z20getCrankshaftAngleNtx
        THUMB
//  247 float getCrankshaftAngleNt(efitime_t timeNt DECLARE_ENGINE_PARAMETER_S) {
_Z20getCrankshaftAngleNtx:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R6,R0
        MOVS     R7,R1
//  248 	efitime_t timeSinceZeroAngleNt = timeNt
//  249 			- engine->rpmCalculator.lastRpmEventTimeNt;
        LDR.N    R0,??DataTable14_6
        LDR      R0,[R0, #+0]
        LDRD     R2,R3,[R0, #+16]
        SUBS     R4,R6,R2
        SBCS     R5,R7,R3
//  250 
//  251 	/**
//  252 	 * even if we use 'getOneDegreeTimeUs' macros here, it looks like the
//  253 	 * compiler is not smart enough to figure out that "A / ( B / C)" could be optimized into
//  254 	 * "A * C / B" in order to replace a slower division with a faster multiplication.
//  255 	 */
//  256 	int rpm = engine->rpmCalculator.rpm(PASS_ENGINE_PARAMETER_F);
        LDR.N    R0,??DataTable14_6
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN13RpmCalculator3rpmEv
        BL       _ZN13RpmCalculator3rpmEv
        MOV      R8,R0
//  257 	return rpm == 0 ? NAN : timeSinceZeroAngleNt / getOneDegreeTimeNt(rpm);
        CMP      R8,#+0
        BNE.N    ??getCrankshaftAngleNt_0
        VLDR.W   S0,??DataTable14_1  ;; 0x7fffffff
        B.N      ??getCrankshaftAngleNt_1
??getCrankshaftAngleNt_0:
        MOVS     R0,R4
        MOVS     R1,R5
          CFI FunCall __aeabi_l2f
        BL       __aeabi_l2f
        VMOV     S0,R0
        VMOV     S1,R8
        VCVT.F32.S32 S1,S1
        VLDR.W   S2,??DataTable14_2  ;; 0x4bd59f80
        VDIV.F32 S1,S2,S1
        VDIV.F32 S0,S0,S1
??getCrankshaftAngleNt_1:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock16
//  258 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12:
        DC32     0x4822c2ab
//  259 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function _Z17initRpmCalculatorP7LoggingP6Engine
        THUMB
//  260 void initRpmCalculator(Logging *sharedLogger, Engine *engine) {
_Z17initRpmCalculatorP7LoggingP6Engine:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  261 	logger = sharedLogger;
        LDR.N    R0,??DataTable14_13
        STR      R4,[R0, #+0]
//  262 #if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
//  263 
//  264 //	tdcScheduler[0].name = "tdc0";
//  265 //	tdcScheduler[1].name = "tdc1";
//  266 	addTriggerEventListener(tdcMarkCallback, "chart TDC mark", engine);
        MOVS     R2,R5
        LDR.N    R1,??DataTable14_25
        LDR.N    R0,??DataTable14_26
          CFI FunCall _Z23addTriggerEventListenerPFv15trigger_event_ejEPKcP6Engine
        BL       _Z23addTriggerEventListenerPFv15trigger_event_ejEPKcP6Engine
//  267 #endif
//  268 
//  269 	addTriggerEventListener(rpmShaftPositionCallback, "rpm reporter", engine);
        MOVS     R2,R5
        LDR.N    R1,??DataTable14_27
        LDR.N    R0,??DataTable14_28
          CFI FunCall _Z23addTriggerEventListenerPFv15trigger_event_ejEPKcP6Engine
        BL       _Z23addTriggerEventListenerPFv15trigger_event_ejEPKcP6Engine
//  270 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock17
//  271 
//  272 #if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
//  273 /**
//  274  * Schedules a callback 'angle' degree of crankshaft from now.
//  275  * The callback would be executed once after the duration of time which
//  276  * it takes the crankshaft to rotate to the specified angle.
//  277  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function _Z15scheduleByAngleiP12scheduling_sfPFvPvES1_P13RpmCalculator
        THUMB
//  278 void scheduleByAngle(int rpm, scheduling_s *timer, angle_t angle,
//  279 		schfunc_t callback, void *param, RpmCalculator *calc) {
_Z15scheduleByAngleiP12scheduling_sfPFvPvES1_P13RpmCalculator:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        VPUSH    {D8}
          CFI D8 Frame(CFA, -32)
          CFI CFA R13+32
        SUB      SP,SP,#+8
          CFI CFA R13+40
        MOVS     R4,R0
        MOVS     R5,R1
        VMOV.F32 S16,S0
        MOVS     R6,R2
        MOVS     R7,R3
        LDR      R8,[SP, #+40]
//  280 	efiAssertVoid(isValidRpm(rpm), "RPM check expected");
        CMP      R4,#+1
        BLT.N    ??scheduleByAngle_0
        MOVW     R0,#+30000
        CMP      R4,R0
        BLT.N    ??scheduleByAngle_1
??scheduleByAngle_0:
        LDR.N    R0,??DataTable14_29
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??scheduleByAngle_2
//  281 	float delayUs = calc->oneDegreeUs * angle;
??scheduleByAngle_1:
        VLDR     S0,[R8, #+8]
        VMUL.F32 S17,S0,S16
//  282 	efiAssertVoid(!cisnan(delayUs), "NaN delay?");
        VMOV.F32 S0,S17
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BNE.N    ??scheduleByAngle_3
        LDR.N    R0,??DataTable14_30
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??scheduleByAngle_2
//  283 	scheduleTask("by angle", timer, (int) delayUs, callback, param);
??scheduleByAngle_3:
        STR      R7,[SP, #+0]
        MOVS     R3,R6
        VCVT.S32.F32 S0,S17
        VMOV     R2,S0
        MOVS     R1,R5
        LDR.N    R0,??DataTable14_31
          CFI FunCall _Z12scheduleTaskPKcP12scheduling_siPFvPvES3_
        BL       _Z12scheduleTaskPKcP12scheduling_siPFvPvES3_
//  284 }
??scheduleByAngle_2:
        ADD      SP,SP,#+8
          CFI CFA R13+32
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+24
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock18

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14:
        DC8      "r",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_1:
        DC32     0x7fffffff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_2:
        DC32     0x4bd59f80

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_3:
        DC32     _ZN12scheduling_sC1Ev

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_4:
        DC32     tdcScheduler

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_5:
        DC32     0x9bdd3c00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_6:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_7:
        DC32     0x1e0a6e00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_8:
        DC32     0xa037a00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_9:
        DC32     notRunnintNow

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_10:
        DC32     notRunningPrev

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_11:
        DC32     0x3ff00000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_12:
        DC32     `?<Constant "templog rpm=0 since n...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_13:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_14:
        DC32     persistentState

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_15:
        DC32     0xe0001004

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_16:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_17:
        DC32     `?<Constant "lowstck#2z">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_18:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_19:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_20:
        DC32     0x58d09800

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_21:
        DC32     rpmBuffer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_22:
        DC32     waveChart

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_23:
        DC32     _Z13onTdcCallbackv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_24:
        DC32     _engine+0xDE4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_25:
        DC32     `?<Constant "chart TDC mark">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_26:
        DC32     _Z15tdcMarkCallback15trigger_event_ej

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_27:
        DC32     `?<Constant "rpm reporter">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_28:
        DC32     _Z24rpmShaftPositionCallback15trigger_event_ej

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_29:
        DC32     `?<Constant "RPM check expected">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_30:
        DC32     `?<Constant "NaN delay?">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_31:
        DC32     `?<Constant "by angle">`

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
`?<Constant "lowstck#2z">`:
        DATA
        DC8 "lowstck#2z"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 "r"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "chart TDC mark">`:
        DATA
        DC8 "chart TDC mark"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "rpm reporter">`:
        DATA
        DC8 "rpm reporter"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "RPM check expected">`:
        DATA
        DC8 "RPM check expected"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "NaN delay?">`:
        DATA
        DC8 "NaN delay?"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "by angle">`:
        DATA
        DC8 "by angle"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "templog rpm=0 since n...">`:
        DATA
        DC8 "templog rpm=0 since not running [%x][%x] [%x][%x]"
        DC8 0, 0

        END
//  285 #endif
//  286 
//  287 #else
//  288 RpmCalculator::RpmCalculator() {
//  289 
//  290 }
//  291 
//  292 #endif /* EFI_SHAFT_POSITION_INPUT */
//  293 
// 
//    80 bytes in section .bss
//     4 bytes in section .init_array
//   142 bytes in section .rodata
// 1 268 bytes in section .text
// 
// 1 272 bytes of CODE  memory
//   142 bytes of CONST memory
//    80 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
