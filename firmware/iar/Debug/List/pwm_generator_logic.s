///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:59 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\sys /
//                       tem\pwm_generator_logic.cpp                          /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\sys /
//                       tem\pwm_generator_logic.cpp -lCN                     /
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
//                       pwm_generator_logic.s                                /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME pwm_generator_logic

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z14scheduleByTimePKcP12scheduling_sxPFvPvES3_
        EXTERN _Z17checkSwitchTimes2iPf
        EXTERN _ZN12multi_wave_s13setSwitchTimeEif
        EXTERN _ZN12multi_wave_s4initEPfP13single_wave_s
        EXTERN _ZN12multi_wave_sC1Ev
        EXTERN _ZN12scheduling_sC1Ev
        EXTERN _ZN13single_wave_s4initEPa
        EXTERN _ZN13single_wave_sC1Ev
        EXTERN _ZNK12multi_wave_s13getSwitchTimeEi
        EXTERN __aeabi_f2lz
        EXTERN __aeabi_ldivmod
        EXTERN __aeabi_memset
        EXTERN __aeabi_vec_ctor_nocookie_nodtor
        EXTERN __iar_FDtest
        EXTERN firmwareError
        EXTERN getTimeNowNt
        EXTERN getTimeNowUs

        PUBLIC _Z17copyPwmParametersP9PwmConfigiPfiPPa
        PUBLIC _ZN9PwmConfig13weComplexInitEPKciPfiPPaPFvPS_EPFvS5_iE
        PUBLIC _ZN9PwmConfig15baseConstructorEv
        PUBLIC _ZN9PwmConfig16handleCycleStartEv
        PUBLIC _ZN9PwmConfig4initEPfP13single_wave_s
        PUBLIC _ZN9PwmConfigC1EPfP13single_wave_s
        PUBLIC _ZN9PwmConfigC1Ev
        PUBLIC _ZN9PwmConfigC2EPfP13single_wave_s
        PUBLIC _ZN9PwmConfigC2Ev
        PUBLIC _ZN9SimplePwm21setSimplePwmDutyCycleEf
        PUBLIC _ZN9SimplePwmC1Ev
        PUBLIC _ZN9SimplePwmC2Ev
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
        

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _ZN9PwmConfigC2Ev
        THUMB
// __code __interwork __softfp PwmConfig::subobject PwmConfig()
_ZN9PwmConfigC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN9PwmConfigC1Ev
        BL       _ZN9PwmConfigC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _ZN9PwmConfigC2EPfP13single_wave_s
        THUMB
// __code __interwork __softfp PwmConfig::subobject PwmConfig(float *, single_wave_s *)
_ZN9PwmConfigC2EPfP13single_wave_s:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN9PwmConfigC1EPfP13single_wave_s
        BL       _ZN9PwmConfigC1EPfP13single_wave_s
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1
// F:\stuff\rusefi_sourceforge\firmware\controllers\system\pwm_generator_logic.cpp
//    1 /**
//    2  * @file    pwm_generator_logic.cpp
//    3  *
//    4  * This PWM implementation keep track of when it would be the next time to toggle the signal.
//    5  * It constantly sets timer to that next toggle time, then sets the timer again from the callback, and so on.
//    6  *
//    7  * @date Mar 2, 2014
//    8  * @author Andrey Belomutskiy, (c) 2012-2015
//    9  */
//   10 
//   11 #include "main.h"
//   12 #include "pwm_generator_logic.h"
//   13 
//   14 /**
//   15  * We need to limit the number of iterations in order to avoid precision loss while calculating
//   16  * next toggle time
//   17  */
//   18 #define ITERATION_LIMIT 1000
//   19 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _ZN9SimplePwmC2Ev
        THUMB
// __code __interwork __softfp SimplePwm::subobject SimplePwm()
_ZN9SimplePwmC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN9SimplePwmC1Ev
        BL       _ZN9SimplePwmC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _ZN9SimplePwmC1Ev
        THUMB
//   20 SimplePwm::SimplePwm() {
_ZN9SimplePwmC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN9PwmConfigC2Ev
        BL       _ZN9PwmConfigC2Ev
        MOVS     R3,#+1
        MOVS     R2,#+4
        LDR.N    R1,??DataTable5_1
        ADDS     R0,R4,#+96
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
        ADDS     R0,R4,#+108
          CFI FunCall _ZN13single_wave_sC1Ev
        BL       _ZN13single_wave_sC1Ev
//   21 	waveInstance.init(pinStates);
        ADDS     R1,R4,#+92
        ADDS     R0,R4,#+108
          CFI FunCall _ZN13single_wave_s4initEPa
        BL       _ZN13single_wave_s4initEPa
//   22 	sr[0] = waveInstance;
        LDR      R0,[R4, #+108]
        STR      R0,[R4, #+96]
//   23 	init(_switchTimes, sr);
        ADDS     R2,R4,#+96
        ADDS     R1,R4,#+100
        MOVS     R0,R4
          CFI FunCall _ZN9PwmConfig4initEPfP13single_wave_s
        BL       _ZN9PwmConfig4initEPfP13single_wave_s
//   24 }
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _ZN9PwmConfig15baseConstructorEv
        THUMB
//   25 void PwmConfig::baseConstructor() {
_ZN9PwmConfig15baseConstructorEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   26 	memset(&scheduling, 0, sizeof(scheduling));
        MOVS     R2,#+24
        MOVS     R1,#+0
        ADDS     R0,R4,#+32
          CFI FunCall memset
        BL       memset
//   27 	memset(&safe, 0, sizeof(safe));
        MOVS     R2,#+24
        MOVS     R1,#+0
        ADDS     R0,R4,#+56
          CFI FunCall memset
        BL       memset
//   28 	dbgNestingLevel = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+28]
//   29 	periodNt = NAN;
        MVNS     R0,#-2147483648
        STR      R0,[R4, #+24]
//   30 	memset(&outputPins, 0, sizeof(outputPins));
        MOVS     R2,#+12
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall memset
        BL       memset
//   31 	phaseCount = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+80]
//   32 	cycleCallback = NULL;
        MOVS     R0,#+0
        STR      R0,[R4, #+84]
//   33 	stateChangeCallback = NULL;
        MOVS     R0,#+0
        STR      R0,[R4, #+88]
//   34 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock4
//   35 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _ZN9PwmConfigC1Ev
        THUMB
//   36 PwmConfig::PwmConfig() {
_ZN9PwmConfigC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        ADDS     R0,R4,#+12
          CFI FunCall _ZN12multi_wave_sC1Ev
        BL       _ZN12multi_wave_sC1Ev
        ADDS     R0,R4,#+32
          CFI FunCall _ZN12scheduling_sC1Ev
        BL       _ZN12scheduling_sC1Ev
//   37 	baseConstructor();
        MOVS     R0,R4
          CFI FunCall _ZN9PwmConfig15baseConstructorEv
        BL       _ZN9PwmConfig15baseConstructorEv
//   38 }
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock5
//   39 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _ZN9PwmConfigC1EPfP13single_wave_s
        THUMB
//   40 PwmConfig::PwmConfig(float *st, single_wave_s *waves) {
_ZN9PwmConfigC1EPfP13single_wave_s:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        ADDS     R0,R4,#+12
          CFI FunCall _ZN12multi_wave_sC1Ev
        BL       _ZN12multi_wave_sC1Ev
        ADDS     R0,R4,#+32
          CFI FunCall _ZN12scheduling_sC1Ev
        BL       _ZN12scheduling_sC1Ev
//   41 	baseConstructor();
        MOVS     R0,R4
          CFI FunCall _ZN9PwmConfig15baseConstructorEv
        BL       _ZN9PwmConfig15baseConstructorEv
//   42 	multiWave.init(st, waves);
        MOVS     R2,R6
        MOVS     R1,R5
        ADDS     R0,R4,#+12
          CFI FunCall _ZN12multi_wave_s4initEPfP13single_wave_s
        BL       _ZN12multi_wave_s4initEPfP13single_wave_s
//   43 }
        MOVS     R0,R4
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock6
//   44 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _ZN9PwmConfig4initEPfP13single_wave_s
        THUMB
//   45 void PwmConfig::init(float *st, single_wave_s *waves) {
_ZN9PwmConfig4initEPfP13single_wave_s:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   46 	multiWave.init(st, waves);
        MOVS     R2,R6
        MOVS     R1,R5
        ADDS     R0,R4,#+12
          CFI FunCall _ZN12multi_wave_s4initEPfP13single_wave_s
        BL       _ZN12multi_wave_s4initEPfP13single_wave_s
//   47 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock7
//   48 
//   49 /**
//   50  * @param dutyCycle value between 0 and 1
//   51  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _ZN9SimplePwm21setSimplePwmDutyCycleEf
        THUMB
//   52 void SimplePwm::setSimplePwmDutyCycle(float dutyCycle) {
_ZN9SimplePwm21setSimplePwmDutyCycleEf:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        VMOV.F32 S16,S0
//   53 	multiWave.setSwitchTime(0, dutyCycle);
        VMOV.F32 S0,S16
        MOVS     R1,#+0
        ADDS     R0,R4,#+12
          CFI FunCall _ZN12multi_wave_s13setSwitchTimeEif
        BL       _ZN12multi_wave_s13setSwitchTimeEif
//   54 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock8
//   55 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z19getNextSwitchTimeUsP9PwmConfig
        THUMB
//   56 static efitimeus_t getNextSwitchTimeUs(PwmConfig *state) {
_Z19getNextSwitchTimeUsP9PwmConfig:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8-D9}
          CFI D9 Frame(CFA, -16)
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
//   57 	efiAssert(state->safe.phaseIndex < PWM_PHASE_MAX_COUNT, "phaseIndex range", 0);
        LDR      R0,[R4, #+72]
        CMP      R0,#+252
        BLT.N    ??getNextSwitchTimeUs_0
        LDR.N    R0,??DataTable5_2
          CFI FunCall firmwareError
        BL       firmwareError
        MOVS     R0,#+0
        MOVS     R1,#+0
        B.N      ??getNextSwitchTimeUs_1
//   58 	int iteration = state->safe.iteration;
??getNextSwitchTimeUs_0:
        VLDR     S16,[R4, #+60]
//   59 	float switchTime = state->multiWave.getSwitchTime(state->safe.phaseIndex);
        LDR      R1,[R4, #+72]
        ADDS     R0,R4,#+12
          CFI FunCall _ZNK12multi_wave_s13getSwitchTimeEi
        BL       _ZNK12multi_wave_s13getSwitchTimeEi
        VMOV.F32 S17,S0
//   60 	float periodNt = state->safe.periodNt;
        VLDR     S18,[R4, #+56]
//   61 #if DEBUG_PWM
//   62 	scheduleMsg(&logger, "iteration=%d switchTime=%f period=%f", iteration, switchTime, period);
//   63 #endif
//   64 
//   65 	/**
//   66 	 * Once 'iteration' gets relatively high, we might lose calculation precision here.
//   67 	 * This is addressed by ITERATION_LIMIT
//   68 	 */
//   69 	efitime_t timeToSwitchNt = (efitime_t) ((iteration + switchTime) * periodNt);
        VCVT.F32.S32 S0,S16
        VADD.F32 S0,S0,S17
        VMUL.F32 S0,S0,S18
        VMOV     R0,S0
          CFI FunCall __aeabi_f2lz
        BL       __aeabi_f2lz
//   70 
//   71 #if DEBUG_PWM
//   72 	scheduleMsg(&logger, "start=%d timeToSwitch=%d", state->safe.start, timeToSwitch);
//   73 #endif
//   74 	return NT2US(state->safe.startNt + timeToSwitchNt);
        LDRD     R2,R3,[R4, #+64]
        ADDS     R0,R2,R0
        ADCS     R1,R3,R1
        MOVS     R2,#+168
        MOVS     R3,#+0
          CFI FunCall __aeabi_ldivmod
        BL       __aeabi_ldivmod
??getNextSwitchTimeUs_1:
        VPOP     {D8-D9}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock9
//   75 }
//   76 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _ZN9PwmConfig16handleCycleStartEv
        THUMB
//   77 void PwmConfig::handleCycleStart() {
_ZN9PwmConfig16handleCycleStartEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   78 	if (safe.phaseIndex == 0) {
        LDR      R0,[R4, #+72]
        CMP      R0,#+0
        BNE.N    ??handleCycleStart_0
//   79 		if (cycleCallback != NULL) {
        LDR      R0,[R4, #+84]
        CMP      R0,#+0
        BEQ.N    ??handleCycleStart_1
//   80 			cycleCallback(this);
        MOVS     R0,R4
        LDR      R1,[R4, #+84]
          CFI FunCall
        BLX      R1
//   81 		}
//   82 		efiAssertVoid(periodNt != 0, "period not initialized");
??handleCycleStart_1:
        VLDR     S0,[R4, #+24]
        VCMP.F32 S0,#0.0
        FMSTAT   
        BNE.N    ??handleCycleStart_2
        LDR.N    R0,??DataTable5_3
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??handleCycleStart_3
//   83 		if (safe.periodNt != periodNt || safe.iteration == ITERATION_LIMIT) {
??handleCycleStart_2:
        VLDR     S0,[R4, #+56]
        VLDR     S1,[R4, #+24]
        VCMP.F32 S0,S1
        FMSTAT   
        BNE.N    ??handleCycleStart_4
        LDR      R0,[R4, #+60]
        CMP      R0,#+1000
        BNE.N    ??handleCycleStart_0
//   84 			/**
//   85 			 * period length has changed - we need to reset internal state
//   86 			 */
//   87 			safe.startNt = getTimeNowNt();
??handleCycleStart_4:
          CFI FunCall getTimeNowNt
        BL       getTimeNowNt
        STRD     R0,R1,[R4, #+64]
//   88 			safe.iteration = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+60]
//   89 			safe.periodNt = periodNt;
        LDR      R0,[R4, #+24]
        STR      R0,[R4, #+56]
//   90 #if DEBUG_PWM
//   91 			scheduleMsg(&logger, "state reset start=%d iteration=%d", state->safe.start, state->safe.iteration);
//   92 #endif
//   93 		}
//   94 	}
//   95 }
??handleCycleStart_0:
??handleCycleStart_3:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock10
//   96 
//   97 /**
//   98  * @return Next time for signal toggle
//   99  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z14togglePwmStateP9PwmConfig
        THUMB
//  100 static efitimeus_t togglePwmState(PwmConfig *state) {
_Z14togglePwmStateP9PwmConfig:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  101 #if DEBUG_PWM
//  102 	scheduleMsg(&logger, "togglePwmState phaseIndex=%d iteration=%d", state->safe.phaseIndex, state->safe.iteration);
//  103 	scheduleMsg(&logger, "state->period=%f state->safe.period=%f", state->period, state->safe.period);
//  104 #endif
//  105 
//  106 	if (cisnan(state->periodNt)) {
        VLDR     S0,[R4, #+24]
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BNE.N    ??togglePwmState_0
//  107 		/**
//  108 		 * NaN period means PWM is paused
//  109 		 */
//  110 		return getTimeNowUs() + MS2US(100);
          CFI FunCall getTimeNowUs
        BL       getTimeNowUs
        LDR.N    R2,??DataTable5_4  ;; 0x186a0
        MOVS     R3,#+0
        ADDS     R0,R0,R2
        ADCS     R1,R1,R3
        B.N      ??togglePwmState_1
//  111 	}
//  112 
//  113 	state->handleCycleStart();
??togglePwmState_0:
        MOVS     R0,R4
          CFI FunCall _ZN9PwmConfig16handleCycleStartEv
        BL       _ZN9PwmConfig16handleCycleStartEv
//  114 
//  115 	/**
//  116 	 * Here is where the 'business logic' - the actual pin state change is happening
//  117 	 */
//  118 	// callback state index is offset by one. todo: why? can we simplify this?
//  119 	int cbStateIndex = state->safe.phaseIndex == 0 ? state->phaseCount - 1 : state->safe.phaseIndex - 1;
        LDR      R0,[R4, #+72]
        CMP      R0,#+0
        BNE.N    ??togglePwmState_2
        LDR      R0,[R4, #+80]
        SUBS     R5,R0,#+1
        B.N      ??togglePwmState_3
??togglePwmState_2:
        LDR      R0,[R4, #+72]
        SUBS     R5,R0,#+1
//  120 	state->stateChangeCallback(state, cbStateIndex);
??togglePwmState_3:
        MOVS     R1,R5
        MOVS     R0,R4
        LDR      R2,[R4, #+88]
          CFI FunCall
        BLX      R2
//  121 
//  122 	efitimeus_t nextSwitchTimeUs = getNextSwitchTimeUs(state);
        MOVS     R0,R4
          CFI FunCall _Z19getNextSwitchTimeUsP9PwmConfig
        BL       _Z19getNextSwitchTimeUsP9PwmConfig
//  123 #if DEBUG_PWM
//  124 	scheduleMsg(&logger, "%s: nextSwitchTime %d", state->name, nextSwitchTime);
//  125 #endif
//  126 	// signed value is needed here
//  127 //	int64_t timeToSwitch = nextSwitchTimeUs - getTimeNowUs();
//  128 //	if (timeToSwitch < 1) {
//  129 //		/**
//  130 //		 * We are here if we are late for a state transition.
//  131 //		 * At 12000RPM=200Hz with a 60 toothed wheel we need to change state every
//  132 //		 * 1000000 / 200 / 120 = ~41 uS. We are kind of OK.
//  133 //		 *
//  134 //		 * We are also here after a flash write. Flash write freezes the whole chip for a couple of seconds,
//  135 //		 * so PWM generation and trigger simulation generation would have to recover from this time lag.
//  136 //		 */
//  137 //		//todo: introduce error and test this error handling		warning(OBD_PCM_Processor_Fault, "PWM: negative switch time");
//  138 //		timeToSwitch = 10;
//  139 //	}
//  140 
//  141 	state->safe.phaseIndex++;
        LDR      R2,[R4, #+72]
        ADDS     R2,R2,#+1
        STR      R2,[R4, #+72]
//  142 	if (state->safe.phaseIndex == state->phaseCount) {
        LDR      R2,[R4, #+72]
        LDR      R3,[R4, #+80]
        CMP      R2,R3
        BNE.N    ??togglePwmState_4
//  143 		state->safe.phaseIndex = 0; // restart
        MOVS     R2,#+0
        STR      R2,[R4, #+72]
//  144 		state->safe.iteration++;
        LDR      R2,[R4, #+60]
        ADDS     R2,R2,#+1
        STR      R2,[R4, #+60]
//  145 	}
//  146 	return nextSwitchTimeUs;
??togglePwmState_4:
??togglePwmState_1:
        POP      {R2,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock11
//  147 }
//  148 
//  149 /**
//  150  * Main PWM loop: toggle pin & schedule next invocation
//  151  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _Z13timerCallbackP9PwmConfig
        THUMB
//  152 static void timerCallback(PwmConfig *state) {
_Z13timerCallbackP9PwmConfig:
        PUSH     {R2-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+24
        MOVS     R6,R0
//  153 	state->dbgNestingLevel++;
        LDR      R0,[R6, #+28]
        ADDS     R0,R0,#+1
        STR      R0,[R6, #+28]
//  154 	efiAssertVoid(state->dbgNestingLevel < 25, "PWM nesting issue");
        LDR      R0,[R6, #+28]
        CMP      R0,#+25
        BLT.N    ??timerCallback_0
        LDR.N    R0,??DataTable5_5
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??timerCallback_1
//  155 
//  156 	efitimeus_t switchTimeUs = togglePwmState(state);
??timerCallback_0:
        MOVS     R0,R6
          CFI FunCall _Z14togglePwmStateP9PwmConfig
        BL       _Z14togglePwmStateP9PwmConfig
        MOVS     R4,R0
        MOVS     R5,R1
//  157 	scheduleByTime("pwm", &state->scheduling, switchTimeUs, (schfunc_t) timerCallback, state);
        STR      R6,[SP, #+4]
        LDR.N    R0,??DataTable5_6
        STR      R0,[SP, #+0]
        MOVS     R2,R4
        MOVS     R3,R5
        ADDS     R1,R6,#+32
        ADR.N    R0,??DataTable5  ;; "pwm"
          CFI FunCall _Z14scheduleByTimePKcP12scheduling_sxPFvPvES3_
        BL       _Z14scheduleByTimePKcP12scheduling_sxPFvPvES3_
//  158 	state->dbgNestingLevel--;
        LDR      R0,[R6, #+28]
        SUBS     R0,R0,#+1
        STR      R0,[R6, #+28]
//  159 }
??timerCallback_1:
        POP      {R0,R1,R4-R6,PC}  ;; return
          CFI EndBlock cfiBlock12
//  160 
//  161 /**
//  162  * Incoming parameters are potentially just values on current stack, so we have to copy
//  163  * into our own permanent storage, right?
//  164  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z17copyPwmParametersP9PwmConfigiPfiPPa
        THUMB
//  165 void copyPwmParameters(PwmConfig *state, int phaseCount, float *switchTimes, int waveCount, pin_state_t **pinStates) {
_Z17copyPwmParametersP9PwmConfigiPfiPPa:
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
        MOVS     R6,R2
        MOVS     R7,R3
        LDR      R8,[SP, #+32]
//  166 	state->phaseCount = phaseCount;
        STR      R5,[R4, #+80]
//  167 
//  168 	for (int phaseIndex = 0; phaseIndex < phaseCount; phaseIndex++) {
        MOVS     R9,#+0
??copyPwmParameters_0:
        CMP      R9,R5
        BGE.N    ??copyPwmParameters_1
//  169 		state->multiWave.setSwitchTime(phaseIndex, switchTimes[phaseIndex]);
        ADDS     R0,R6,R9, LSL #+2
        VLDR     S0,[R0, #0]
        MOV      R1,R9
        ADDS     R0,R4,#+12
          CFI FunCall _ZN12multi_wave_s13setSwitchTimeEif
        BL       _ZN12multi_wave_s13setSwitchTimeEif
//  170 
//  171 		for (int waveIndex = 0; waveIndex < waveCount; waveIndex++) {
        MOVS     R0,#+0
??copyPwmParameters_2:
        CMP      R0,R7
        BGE.N    ??copyPwmParameters_3
//  172 //			print("output switch time index (%d/%d) at %f to %d\r\n", phaseIndex,waveIndex,
//  173 //					switchTimes[phaseIndex], pinStates[waveIndex][phaseIndex]);
//  174 			state->multiWave.waves[waveIndex].pinStates[phaseIndex] = pinStates[waveIndex][phaseIndex];
        LDR      R1,[R4, #+16]
        LDR      R1,[R1, R0, LSL #+2]
        LDR      R2,[R8, R0, LSL #+2]
        LDRB     R2,[R9, R2]
        STRB     R2,[R9, R1]
//  175 		}
        ADDS     R0,R0,#+1
        B.N      ??copyPwmParameters_2
//  176 	}
??copyPwmParameters_3:
        ADDS     R9,R9,#+1
        B.N      ??copyPwmParameters_0
//  177 }
??copyPwmParameters_1:
        POP      {R0,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock13

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _ZN9PwmConfig13weComplexInitEPKciPfiPPaPFvPS_EPFvS5_iE
        THUMB
_ZN9PwmConfig13weComplexInitEPKciPfiPPaPFvPS_EPFvS5_iE:
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
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
        LDR      R8,[SP, #+40]
        LDR      R9,[SP, #+44]
        LDR      R10,[SP, #+48]
        LDR      R11,[SP, #+52]
        VLDR     S0,[R4, #+24]
        VCMP.F32 S0,#0.0
        FMSTAT   
        BNE.N    ??weComplexInit_0
        LDR.N    R0,??DataTable5_7
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??weComplexInit_1
??weComplexInit_0:
        CMP      R6,#+0
        BNE.N    ??weComplexInit_2
        LDR.N    R0,??DataTable5_8
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??weComplexInit_1
??weComplexInit_2:
        CMP      R6,#+253
        BLT.N    ??weComplexInit_3
        LDR.N    R0,??DataTable5_9
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??weComplexInit_1
??weComplexInit_3:
        CMP      R8,#+1
        BGE.N    ??weComplexInit_4
        LDR.N    R0,??DataTable5_10
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??weComplexInit_1
??weComplexInit_4:
        MOVS     R1,R7
        MOVS     R0,R6
          CFI FunCall _Z17checkSwitchTimes2iPf
        BL       _Z17checkSwitchTimes2iPf
        STR      R10,[R4, #+84]
        STR      R11,[R4, #+88]
        STR      R8,[R4, #+12]
        STR      R9,[SP, #+0]
        MOV      R3,R8
        MOVS     R2,R7
        MOVS     R1,R6
        MOVS     R0,R4
          CFI FunCall _Z17copyPwmParametersP9PwmConfigiPfiPPa
        BL       _Z17copyPwmParametersP9PwmConfigiPfiPPa
        MOVS     R0,#+0
        STR      R0,[R4, #+72]
        LDR.N    R0,??DataTable5_11  ;; 0xbf800000
        STR      R0,[R4, #+56]
        MOVS     R0,#-1
        STR      R0,[R4, #+60]
        MOVS     R0,R4
          CFI FunCall _Z13timerCallbackP9PwmConfig
        BL       _Z13timerCallbackP9PwmConfig
??weComplexInit_1:
        POP      {R0,R4-R11,PC}   ;; return
          CFI EndBlock cfiBlock14

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5:
        DC8      "pwm"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_1:
        DC32     _ZN13single_wave_sC1Ev

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_2:
        DC32     `?<Constant "phaseIndex range">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_3:
        DC32     `?<Constant "period not initialized">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_4:
        DC32     0x186a0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_5:
        DC32     `?<Constant "PWM nesting issue">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_6:
        DC32     _Z13timerCallbackP9PwmConfig

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_7:
        DC32     `?<Constant "period is not initial...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_8:
        DC32     `?<Constant "signal length cannot ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_9:
        DC32     `?<Constant "too many phases in PWM">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_10:
        DC32     `?<Constant "waveCount should be p...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_11:
        DC32     0xbf800000

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

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "phaseIndex range">`:
        DATA
        DC8 "phaseIndex range"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "PWM nesting issue">`:
        DATA
        DC8 "PWM nesting issue"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "pwm"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "period is not initial...">`:
        DATA
        DC8 "period is not initialized"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "signal length cannot ...">`:
        DATA
        DC8 "signal length cannot be zero"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "too many phases in PWM">`:
        DATA
        DC8 "too many phases in PWM"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "waveCount should be p...">`:
        DATA
        DC8 "waveCount should be positive"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "period not initialized">`:
        DATA
        DC8 "period not initialized"
        DC8 0

        END
//  178 
//  179 void PwmConfig::weComplexInit(const char *msg, int phaseCount, float *switchTimes, int waveCount,
//  180 		pin_state_t **pinStates, pwm_cycle_callback *cycleCallback, pwm_gen_callback *stateChangeCallback) {
//  181 
//  182 	efiAssertVoid(periodNt != 0, "period is not initialized");
//  183 	if (phaseCount == 0) {
//  184 		firmwareError("signal length cannot be zero");
//  185 		return;
//  186 	}
//  187 	if (phaseCount > PWM_PHASE_MAX_COUNT) {
//  188 		firmwareError("too many phases in PWM");
//  189 		return;
//  190 	}
//  191 	efiAssertVoid(waveCount > 0, "waveCount should be positive");
//  192 	checkSwitchTimes2(phaseCount, switchTimes);
//  193 
//  194 	this->cycleCallback = cycleCallback;
//  195 	this->stateChangeCallback = stateChangeCallback;
//  196 
//  197 	multiWave.waveCount = waveCount;
//  198 
//  199 	copyPwmParameters(this, phaseCount, switchTimes, waveCount, pinStates);
//  200 
//  201 	safe.phaseIndex = 0;
//  202 	safe.periodNt = -1;
//  203 	safe.iteration = -1;
//  204 
//  205 	// let's start the indefinite callback loop of PWM generation
//  206 	timerCallback(this);
//  207 }
// 
// 184 bytes in section .rodata
// 926 bytes in section .text
// 
// 904 bytes of CODE  memory (+ 22 bytes shared)
// 184 bytes of CONST memory
//
//Errors: none
//Warnings: 1
