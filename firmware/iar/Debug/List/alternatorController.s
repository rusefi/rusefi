///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:14 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\alt /
//                       ernatorController.cpp                                /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\alt /
//                       ernatorController.cpp -lCN                           /
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
//                       alternatorController.s                               /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME alternatorController

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z10hwPortname11brain_pin_e
        EXTERN _Z12boolToStringb
        EXTERN _Z17startSimplePwmExtP9PwmConfigPKc11brain_pin_eP9OutputPinffPFvS0_iE
        EXTERN _Z8getVBattv
        EXTERN _ZN3Pid13updateFactorsEfff
        EXTERN _ZN3Pid14getIntegrationEv
        EXTERN _ZN3Pid4getDEv
        EXTERN _ZN3Pid4getIEv
        EXTERN _ZN3Pid4getPEv
        EXTERN _ZN3Pid8getValueEfff
        EXTERN _ZN3PidC1Efffff
        EXTERN _ZN9OutputPin8setValueEi
        EXTERN _ZN9OutputPinC1Ev
        EXTERN _ZN9SimplePwm21setSimplePwmDutyCycleEf
        EXTERN _ZN9SimplePwmC1Ev
        EXTERN __aeabi_f2d
        EXTERN addConsoleAction
        EXTERN addConsoleActionF
        EXTERN boardConfiguration
        EXTERN chThdCreateStatic
        EXTERN chThdSleep
        EXTERN engineConfiguration
        EXTERN firmwareError
        EXTERN rlist
        EXTERN scheduleMsg

        PUBLIC _Z11showAltInfov
        PUBLIC _Z13setAltPFactorf
        PUBLIC _Z18initAlternatorCtrlP7Logging
        PUBLIC _Z30setDefaultAlternatorParametersv
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\alternatorController.cpp
//    1 /**
//    2  * @file    alternatorController.cpp
//    3  * @brief   alternator controller - turn alternator off if you do not really need it
//    4  *
//    5  * @date Apr 6, 2014
//    6  * @author Dmitry Sidin
//    7  * @author Andrey Belomutskiy, (c) 2012-2015
//    8  */
//    9 
//   10 #include "main.h"
//   11 #include "engine.h"
//   12 #include "rpm_calculator.h"
//   13 #include "pwm_generator.h"
//   14 #include "alternatorController.h"
//   15 #include "pin_repository.h"
//   16 #include "voltage.h"
//   17 #include "pid.h"
//   18 
//   19 EXTERN_ENGINE
//   20 ;
//   21 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   22 static Logging *logger;
logger:
        DS8 4
//   23 
//   24 #define ALTERNATOR_VALVE_PWM_FREQUENCY 300
//   25 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   26 static SimplePwm alternatorControl;
        LDR.N    R0,??DataTable7_2
          CFI FunCall _ZN9SimplePwmC1Ev
        BL       _ZN9SimplePwmC1Ev
//   27 static OutputPin alternatorPin;
        LDR.N    R0,??DataTable7_3
          CFI FunCall _ZN9OutputPinC1Ev
        BL       _ZN9OutputPinC1Ev
//   28 static Pid altPid(10, 0, 0, 1, 90);
        VLDR.W   S4,??DataTable4  ;; 0x42b40000
        VMOV.F32 S3,#1.0
        VLDR.W   S2,??DataTable5  ;; 0x0
        VLDR.W   S1,??DataTable5  ;; 0x0
        VMOV.F32 S0,#10.0
        LDR.N    R0,??DataTable7_4
          CFI FunCall _ZN3PidC1Efffff
        BL       _ZN3PidC1Efffff
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
alternatorControl:
        DS8 112

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
alternatorPin:
        DS8 16

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
altPid:
        DS8 28
//   29 

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   30 static THD_WORKING_AREA(alternatorControlThreadStack, UTILITY_THREAD_STACK_SIZE);
alternatorControlThreadStack:
        DS8 712
//   31 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   32 static float currentAltDuty;
currentAltDuty:
        DS8 4
//   33 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z13AltCtrlThreadi
        THUMB
//   34 static msg_t AltCtrlThread(int param) {
_Z13AltCtrlThreadi:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8-D10}
          CFI D10 Frame(CFA, -16)
          CFI D9 Frame(CFA, -24)
          CFI D8 Frame(CFA, -32)
          CFI CFA R13+32
        SUB      SP,SP,#+40
          CFI CFA R13+72
        MOVS     R4,R0
//   35         UNUSED(param);
//   36 	chRegSetThreadName("AlternatorController");
        LDR.N    R0,??DataTable7_5
        LDR      R0,[R0, #+24]
        LDR.N    R1,??DataTable7_6
        STR      R1,[R0, #+24]
//   37 	while (true) {
//   38 		chThdSleepMilliseconds(engineConfiguration->alternatorDT);
??AltCtrlThread_0:
        LDR.N    R0,??DataTable7_7
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+2164]
        MOV      R1,#+1000
        MULS     R0,R1,R0
        SUBS     R0,R0,#+1
        MOV      R1,#+1000
        UDIV     R0,R0,R1
        ADDS     R0,R0,#+1
          CFI FunCall chThdSleep
        BL       chThdSleep
//   39 
//   40 		currentAltDuty = engineConfiguration->alternatorOffset + altPid.getValue(engineConfiguration->targetVBatt, getVBatt(PASS_ENGINE_PARAMETER_F), 1);
          CFI FunCall _Z8getVBattv
        BL       _Z8getVBattv
        VMOV.F32 S1,S0
        VMOV.F32 S2,#1.0
        LDR.N    R0,??DataTable7_7
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+2156]
        VMOV     S0,R0
        LDR.N    R0,??DataTable7_4
          CFI FunCall _ZN3Pid8getValueEfff
        BL       _ZN3Pid8getValueEfff
        LDR.N    R0,??DataTable7_7
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+2168]
        VMOV     S1,R0
        VADD.F32 S0,S1,S0
        LDR.N    R0,??DataTable7_8
        VSTR     S0,[R0, #0]
//   41 		if (boardConfiguration->isVerboseAlternator) {
        LDR.N    R0,??DataTable7_9
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+7
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+0
        BEQ.N    ??AltCtrlThread_1
//   42 			scheduleMsg(logger, "alt duty: %f/vbatt=%f/p=%f/i=%f/d=%f int=%f", currentAltDuty, getVBatt(PASS_ENGINE_PARAMETER_F),
//   43 					altPid.getP(), altPid.getI(), altPid.getD(), altPid.getIntegration());
        LDR.N    R0,??DataTable7_4
          CFI FunCall _ZN3Pid14getIntegrationEv
        BL       _ZN3Pid14getIntegrationEv
        VMOV.F32 S16,S0
        LDR.N    R0,??DataTable7_4
          CFI FunCall _ZN3Pid4getDEv
        BL       _ZN3Pid4getDEv
        VMOV.F32 S17,S0
        LDR.N    R0,??DataTable7_4
          CFI FunCall _ZN3Pid4getIEv
        BL       _ZN3Pid4getIEv
        VMOV.F32 S18,S0
        LDR.N    R0,??DataTable7_4
          CFI FunCall _ZN3Pid4getPEv
        BL       _ZN3Pid4getPEv
        VMOV.F32 S19,S0
          CFI FunCall _Z8getVBattv
        BL       _Z8getVBattv
        VMOV.F32 S20,S0
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+32]
        VMOV     R0,S17
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+24]
        VMOV     R0,S18
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+16]
        VMOV     R0,S19
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+8]
        VMOV     R0,S20
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        LDR.N    R0,??DataTable7_8
        LDR      R0,[R0, #+0]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable7_10
        LDR.N    R0,??DataTable7_11
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   44 		}
//   45 
//   46 		alternatorControl.setSimplePwmDutyCycle(currentAltDuty / 100);
??AltCtrlThread_1:
        LDR.N    R0,??DataTable7_8
        VLDR     S0,[R0, #0]
        VLDR.W   S1,??DataTable6  ;; 0x42c80000
        VDIV.F32 S0,S0,S1
        LDR.N    R0,??DataTable7_2
          CFI FunCall _ZN9SimplePwm21setSimplePwmDutyCycleEf
        BL       _ZN9SimplePwm21setSimplePwmDutyCycleEf
        B.N      ??AltCtrlThread_0
          CFI EndBlock cfiBlock1
//   47 	}
//   48 #if defined __GNUC__
//   49 	return -1;
//   50 #endif
//   51 }
//   52 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z13applySettingsv
        THUMB
//   53 static void applySettings(void) {
_Z13applySettingsv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   54 	altPid.updateFactors(engineConfiguration->alternatorControlPFactor, 0, 0);
        VLDR.W   S2,??DataTable5  ;; 0x0
        VLDR.W   S1,??DataTable5  ;; 0x0
        LDR.N    R0,??DataTable7_7
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+2172]
        VMOV     S0,R0
        LDR.N    R0,??DataTable7_4
          CFI FunCall _ZN3Pid13updateFactorsEfff
        BL       _ZN3Pid13updateFactorsEfff
//   55 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock2
//   56 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z11showAltInfov
        THUMB
//   57 void showAltInfo(void) {
_Z11showAltInfov:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+16
          CFI CFA R13+32
//   58 	scheduleMsg(logger, "alt=%s @%s t=%dms", boolToString(engineConfiguration->isAlternatorControlEnabled),
//   59 			hwPortname(boardConfiguration->alternatorControlPin),
//   60 			engineConfiguration->alternatorDT);
        LDR.N    R0,??DataTable7_9
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+168]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R4,R0
        LDR.N    R0,??DataTable7_7
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+5
        ANDS     R0,R0,#0x1
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        LDR.N    R1,??DataTable7_7
        LDR      R1,[R1, #+0]
        LDR      R1,[R1, #+2164]
        STR      R1,[SP, #+0]
        MOVS     R3,R4
        MOVS     R2,R0
        LDR.N    R1,??DataTable7_12
        LDR.N    R0,??DataTable7_11
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   61 	scheduleMsg(logger, "p=%f/i=%f/d=%f offset=%f", engineConfiguration->alternatorControlPFactor,
//   62 			0, 0, engineConfiguration->alternatorOffset); // todo: i & d
        LDR.N    R0,??DataTable7_7
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+2168]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+8]
        MOVS     R0,#+0
        STR      R0,[SP, #+4]
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        LDR.N    R0,??DataTable7_7
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+2172]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable7_13
        LDR.N    R0,??DataTable7_11
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   63 	scheduleMsg(logger, "vbatt=%f/duty=%f/target=%f", getVBatt(PASS_ENGINE_PARAMETER_F), currentAltDuty,
//   64 			engineConfiguration->targetVBatt);
          CFI FunCall _Z8getVBattv
        BL       _Z8getVBattv
        VMOV.F32 S16,S0
        LDR.N    R0,??DataTable7_7
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+2156]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+8]
        LDR.N    R0,??DataTable7_8
        LDR      R0,[R0, #+0]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable7_14
        LDR.N    R0,??DataTable7_11
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   65 }
        ADD      SP,SP,#+16
          CFI CFA R13+16
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//   66 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z13setAltPFactorf
        THUMB
//   67 void setAltPFactor(float p) {
_Z13setAltPFactorf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//   68 	engineConfiguration->alternatorControlPFactor = p;
        LDR.N    R0,??DataTable7_7
        LDR      R0,[R0, #+0]
        ADDW     R0,R0,#+2172
        VSTR     S16,[R0, #0]
//   69 	scheduleMsg(logger, "setAltPid: %f", p);
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable7_15
        LDR.N    R0,??DataTable7_11
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   70 	applySettings();
          CFI FunCall _Z13applySettingsv
        BL       _Z13applySettingsv
//   71 	showAltInfo();
          CFI FunCall _Z11showAltInfov
        BL       _Z11showAltInfov
//   72 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4:
        DC32     0x42b40000
//   73 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z23applyAlternatorPinStateP9PwmConfigi
        THUMB
//   74 static void applyAlternatorPinState(PwmConfig *state, int stateIndex) {
_Z23applyAlternatorPinStateP9PwmConfigi:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//   75 	efiAssertVoid(stateIndex < PWM_PHASE_MAX_COUNT, "invalid stateIndex");
        CMP      R5,#+252
        BLT.N    ??applyAlternatorPinState_0
        LDR.N    R0,??DataTable7_16
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??applyAlternatorPinState_1
//   76 	efiAssertVoid(state->multiWave.waveCount == 1, "invalid idle waveCount");
??applyAlternatorPinState_0:
        LDR      R0,[R4, #+12]
        CMP      R0,#+1
        BEQ.N    ??applyAlternatorPinState_2
        LDR.N    R0,??DataTable7_17
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??applyAlternatorPinState_1
//   77 	OutputPin *output = state->outputPins[0];
??applyAlternatorPinState_2:
        LDR      R6,[R4, #+0]
//   78 	int value = state->multiWave.waves[0].pinStates[stateIndex];
        LDR      R0,[R4, #+16]
        LDR      R0,[R0, #+0]
        LDRSB    R7,[R5, R0]
//   79 	if (!value || engineConfiguration->isAlternatorControlEnabled)
        CMP      R7,#+0
        BEQ.N    ??applyAlternatorPinState_3
        LDR.N    R0,??DataTable7_7
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+5
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??applyAlternatorPinState_4
//   80 		output->setValue(value);
??applyAlternatorPinState_3:
        MOVS     R1,R7
        MOVS     R0,R6
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
//   81 }
??applyAlternatorPinState_4:
??applyAlternatorPinState_1:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock5

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5:
        DC32     0x0
//   82 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z30setDefaultAlternatorParametersv
          CFI NoCalls
        THUMB
//   83 void setDefaultAlternatorParameters(void) {
//   84 	engineConfiguration->alternatorOffAboveTps = 120;
_Z30setDefaultAlternatorParametersv:
        LDR.N    R0,??DataTable7_7
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable7_18  ;; 0x42f00000
        STR      R1,[R0, #+2160]
//   85 
//   86 	boardConfiguration->alternatorControlPin = GPIO_UNASSIGNED;
        LDR.N    R0,??DataTable7_9
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+168]
//   87 	boardConfiguration->alternatorControlPinMode = OM_DEFAULT;
        LDR.N    R0,??DataTable7_9
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+172]
//   88 	engineConfiguration->targetVBatt = 14;
        LDR.N    R0,??DataTable7_7
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable7_19  ;; 0x41600000
        STR      R1,[R0, #+2156]
//   89 
//   90 	engineConfiguration->alternatorOffset = 0;
        LDR.N    R0,??DataTable7_7
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+2168]
//   91 	engineConfiguration->alternatorControlPFactor = 30;
        LDR.N    R0,??DataTable7_7
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable7_20  ;; 0x41f00000
        STR      R1,[R0, #+2172]
//   92 	engineConfiguration->alternatorDT = 100;
        LDR.N    R0,??DataTable7_7
        LDR      R0,[R0, #+0]
        MOVS     R1,#+100
        STR      R1,[R0, #+2164]
//   93 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock6

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6:
        DC32     0x42c80000
//   94 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z18initAlternatorCtrlP7Logging
        THUMB
//   95 void initAlternatorCtrl(Logging *sharedLogger) {
_Z18initAlternatorCtrlP7Logging:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        SUB      SP,SP,#+8
          CFI CFA R13+16
        MOVS     R4,R0
//   96 	logger = sharedLogger;
        LDR.N    R0,??DataTable7_11
        STR      R4,[R0, #+0]
//   97 	addConsoleAction("altinfo", showAltInfo);
        LDR.N    R1,??DataTable7_21
        LDR.N    R0,??DataTable7_22
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//   98 	if (boardConfiguration->alternatorControlPin == GPIO_UNASSIGNED)
        LDR.N    R0,??DataTable7_9
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+168]
        CMP      R0,#+80
        BEQ.N    ??initAlternatorCtrl_0
//   99 		return;
//  100 
//  101 	startSimplePwmExt(&alternatorControl, "Alternator control", boardConfiguration->alternatorControlPin,
//  102 			&alternatorPin,
//  103 			ALTERNATOR_VALVE_PWM_FREQUENCY, 0.1, applyAlternatorPinState);
??initAlternatorCtrl_1:
        LDR.N    R0,??DataTable7_23
        STR      R0,[SP, #+0]
        VLDR.W   S1,??DataTable7  ;; 0x3dcccccd
        VLDR.W   S0,??DataTable7_1  ;; 0x43960000
        LDR.N    R3,??DataTable7_3
        LDR.N    R0,??DataTable7_9
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+168]
        LDR.N    R1,??DataTable7_24
        LDR.N    R0,??DataTable7_2
          CFI FunCall _Z17startSimplePwmExtP9PwmConfigPKc11brain_pin_eP9OutputPinffPFvS0_iE
        BL       _Z17startSimplePwmExtP9PwmConfigPKc11brain_pin_eP9OutputPinffPFvS0_iE
//  104 	chThdCreateStatic(alternatorControlThreadStack, sizeof(alternatorControlThreadStack), LOWPRIO,
//  105 			(tfunc_t) AltCtrlThread, NULL);
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        LDR.N    R3,??DataTable7_25
        MOVS     R2,#+2
        MOV      R1,#+712
        LDR.N    R0,??DataTable7_26
          CFI FunCall chThdCreateStatic
        BL       chThdCreateStatic
//  106 
//  107 	addConsoleActionF("set_alt_p", setAltPFactor);
        LDR.N    R1,??DataTable7_27
        LDR.N    R0,??DataTable7_28
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
//  108 
//  109 	applySettings();
          CFI FunCall _Z13applySettingsv
        BL       _Z13applySettingsv
//  110 }
??initAlternatorCtrl_0:
        POP      {R0,R1,R4,PC}    ;; return
          CFI EndBlock cfiBlock7

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7:
        DC32     0x3dcccccd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_1:
        DC32     0x43960000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_2:
        DC32     alternatorControl

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_3:
        DC32     alternatorPin

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_4:
        DC32     altPid

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_5:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_6:
        DC32     `?<Constant "AlternatorController">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_7:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_8:
        DC32     currentAltDuty

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_9:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_10:
        DC32     `?<Constant "alt duty: %f/vbatt=%f...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_11:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_12:
        DC32     `?<Constant "alt=%s @%s t=%dms">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_13:
        DC32     `?<Constant "p=%f/i=%f/d=%f offset=%f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_14:
        DC32     `?<Constant "vbatt=%f/duty=%f/targ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_15:
        DC32     `?<Constant "setAltPid: %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_16:
        DC32     `?<Constant "invalid stateIndex">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_17:
        DC32     `?<Constant "invalid idle waveCount">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_18:
        DC32     0x42f00000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_19:
        DC32     0x41600000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_20:
        DC32     0x41f00000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_21:
        DC32     _Z11showAltInfov

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_22:
        DC32     `?<Constant "altinfo">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_23:
        DC32     _Z23applyAlternatorPinStateP9PwmConfigi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_24:
        DC32     `?<Constant "Alternator control">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_25:
        DC32     _Z13AltCtrlThreadi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_26:
        DC32     alternatorControlThreadStack

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_27:
        DC32     _Z13setAltPFactorf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_28:
        DC32     `?<Constant "set_alt_p">`

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
`?<Constant "AlternatorController">`:
        DATA
        DC8 "AlternatorController"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "alt duty: %f/vbatt=%f...">`:
        DATA
        DC8 "alt duty: %f/vbatt=%f/p=%f/i=%f/d=%f int=%f"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "alt=%s @%s t=%dms">`:
        DATA
        DC8 "alt=%s @%s t=%dms"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "p=%f/i=%f/d=%f offset=%f">`:
        DATA
        DC8 "p=%f/i=%f/d=%f offset=%f"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "vbatt=%f/duty=%f/targ...">`:
        DATA
        DC8 "vbatt=%f/duty=%f/target=%f"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "setAltPid: %f">`:
        DATA
        DC8 "setAltPid: %f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid stateIndex">`:
        DATA
        DC8 "invalid stateIndex"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid idle waveCount">`:
        DATA
        DC8 "invalid idle waveCount"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "altinfo">`:
        DATA
        DC8 "altinfo"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Alternator control">`:
        DATA
        DC8 "Alternator control"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_alt_p">`:
        DATA
        DC8 "set_alt_p"
        DC8 0, 0

        END
// 
// 876 bytes in section .bss
//   4 bytes in section .init_array
// 244 bytes in section .rodata
// 948 bytes in section .text
// 
// 952 bytes of CODE  memory
// 244 bytes of CONST memory
// 876 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
