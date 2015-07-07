///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:51 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\alg /
//                       o\idle_controller.cpp                                /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\alg /
//                       o\idle_controller.cpp -lCN                           /
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
//                       idle_controller.s                                    /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME idle_controller

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z10isCrankingv
        EXTERN _Z6getTPSv
        EXTERN _Z9idleDebugPKcf
        EXTERN __aeabi_cdcmple
        EXTERN __aeabi_d2iz
        EXTERN __aeabi_dmul
        EXTERN __aeabi_i2d
        EXTERN engineConfiguration
        EXTERN maxF
        EXTERN minF

        PUBLIC _ZN14IdleValveState12setTargetRpmEi
        PUBLIC _ZN14IdleValveState4initEv
        PUBLIC _ZN14IdleValveState7getIdleEij
        PUBLIC _ZN14IdleValveStateC1Ev
        PUBLIC _ZN14IdleValveStateC2Ev
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\algo\idle_controller.cpp
//    1 /**
//    2  * @file	idle_controller.cpp
//    3  * @brief	Simple Idle Air Valve control algorithm
//    4  *
//    5  *			This algorithm is trying to get current RPM to the desired 'target' value
//    6  *			by changing Idle Air Valve solenoid duty cycle. Depending on how far current RPM
//    7  *			is from the target RPM, the incremental change would be smaller or bigger.
//    8  *
//    9  *
//   10  * todo: DEFAULT_IDLE_DUTY should be a field on the IdleValveState, not a constant
//   11  *
//   12  * @date May 22, 2013
//   13  * @author Andrey Belomutskiy, (c) 2012-2015
//   14  */
//   15 
//   16 #include "main.h"
//   17 #include "idle_controller.h"
//   18 #include "efilib.h"
//   19 #include "rpm_calculator.h"
//   20 #include "tps.h"
//   21 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   22 static int lastGoodValue = DEFAULT_IDLE_DUTY;
lastGoodValue:
        DATA
        DC32 60
//   23 
//   24 EXTERN_ENGINE
//   25 ;
//   26 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _ZN14IdleValveStateC2Ev
        THUMB
// __code __interwork __softfp IdleValveState::subobject IdleValveState()
_ZN14IdleValveStateC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN14IdleValveStateC1Ev
        BL       _ZN14IdleValveStateC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _ZN14IdleValveStateC1Ev
          CFI NoCalls
        THUMB
//   27 IdleValveState::IdleValveState() {
//   28 	value = DEFAULT_IDLE_DUTY;
_ZN14IdleValveStateC1Ev:
        LDR.N    R1,??DataTable5_2  ;; 0x42700000
        STR      R1,[R0, #+4]
//   29 	timeOfLastIdleChange = 0;
        MOVS     R1,#+0
        STR      R1,[R0, #+8]
//   30 	time = 0;
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//   31 	targetRpmRangeLeft = targetRpmRangeRight = 0;
        MOVS     R1,#+0
        STR      R1,[R0, #+16]
        STR      R1,[R0, #+12]
//   32 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   33 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _ZN14IdleValveState4initEv
        THUMB
//   34 void IdleValveState::init(DECLARE_ENGINE_PARAMETER_F) {
_ZN14IdleValveState4initEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   35 	setTargetRpm(engineConfiguration->targetIdleRpm);
        LDR.N    R0,??DataTable5_5
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+1516]
        MOVS     R0,R4
          CFI FunCall _ZN14IdleValveState12setTargetRpmEi
        BL       _ZN14IdleValveState12setTargetRpmEi
//   36 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2
//   37 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _ZN14IdleValveState12setTargetRpmEi
        THUMB
//   38 void IdleValveState::setTargetRpm(int targetRpm) {
_ZN14IdleValveState12setTargetRpmEi:
        PUSH     {R4,R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+12
        MOVS     R5,R0
        MOVS     R4,R1
//   39 	targetRpmRangeLeft = (int) (targetRpm * 0.93);
        MOVS     R0,R4
          CFI FunCall __aeabi_i2d
        BL       __aeabi_i2d
        LDR.N    R2,??DataTable5_6  ;; 0x5c28f5c3
        LDR.N    R3,??DataTable5_7  ;; 0x3fedc28f
          CFI FunCall __aeabi_dmul
        BL       __aeabi_dmul
          CFI FunCall __aeabi_d2iz
        BL       __aeabi_d2iz
        STR      R0,[R5, #+12]
//   40 	targetRpmRangeRight = (int) (targetRpm * 1.07);
        MOVS     R0,R4
          CFI FunCall __aeabi_i2d
        BL       __aeabi_i2d
        LDR.N    R2,??DataTable5_8  ;; 0x51eb851f
        LDR.N    R3,??DataTable5_9  ;; 0x3ff11eb8
          CFI FunCall __aeabi_dmul
        BL       __aeabi_dmul
          CFI FunCall __aeabi_d2iz
        BL       __aeabi_d2iz
        STR      R0,[R5, #+16]
//   41 }
        POP      {R4,R5,PC}       ;; return
          CFI EndBlock cfiBlock3
//   42 
//   43 /**
//   44  * @brief	sets new idle valve duty cycle: checks the bounds and reports new value
//   45  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z11setNewValueP14IdleValveStateijPKcf
        THUMB
//   46 static percent_t setNewValue(IdleValveState *idle, int currentRpm, efitimems_t now, const char * msg, percent_t newValue) {
_Z11setNewValueP14IdleValveStateijPKcf:
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
        VMOV     S16,R1
        MOVS     R5,R2
        MOVS     R6,R3
        VMOV.F32 S17,S0
//   47 	newValue = maxF(newValue, MIN_IDLE);
        VMOV.F32 S1,#10.0
        VMOV.F32 S0,S17
          CFI FunCall maxF
        BL       maxF
        VMOV.F32 S17,S0
//   48 	newValue = minF(newValue, MAX_IDLE);
        VLDR.W   S1,??DataTable5  ;; 0x42b40000
        VMOV.F32 S0,S17
          CFI FunCall minF
        BL       minF
        VMOV.F32 S17,S0
//   49 
//   50 	if (idle->value != newValue) {
        VLDR     S0,[R4, #+4]
        VCMP.F32 S0,S17
        FMSTAT   
        BEQ.N    ??setNewValue_0
//   51 		idleDebug(msg, currentRpm);
        VCVT.F32.S32 S0,S16
        MOVS     R0,R6
          CFI FunCall _Z9idleDebugPKcf
        BL       _Z9idleDebugPKcf
//   52 		idle->timeOfLastIdleChange = now;
        STR      R5,[R4, #+8]
//   53 	}
//   54 
//   55 	idle->value = newValue;
??setNewValue_0:
        VSTR     S17,[R4, #+4]
//   56 	return newValue;
        VMOV.F32 S0,S17
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock4
//   57 }
//   58 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z11changeValueP14IdleValveStateiiPKcf
        THUMB
//   59 static percent_t changeValue(IdleValveState *idle, int currentRpm, int now, const char * msg, percent_t delta DECLARE_ENGINE_PARAMETER_S) {
_Z11changeValueP14IdleValveStateiiPKcf:
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
        MOVS     R6,R2
        MOVS     R7,R3
        VMOV.F32 S16,S0
//   60 	if (getTPS(PASS_ENGINE_PARAMETER_F) > 5.0) {
          CFI FunCall _Z6getTPSv
        BL       _Z6getTPSv
        VLDR.W   S1,??DataTable5_1  ;; 0x40a00001
        VCMP.F32 S0,S1
        FMSTAT   
        BLT.N    ??changeValue_0
//   61 		// We are not supposed to be in idle mode. Don't touch anything
//   62 		idleDebug("TPS Lockout, TPS=", getTPS(PASS_ENGINE_PARAMETER_F));
          CFI FunCall _Z6getTPSv
        BL       _Z6getTPSv
        LDR.N    R0,??DataTable5_10
          CFI FunCall _Z9idleDebugPKcf
        BL       _Z9idleDebugPKcf
//   63 		return idle->value;
        VLDR     S0,[R4, #+4]
        B.N      ??changeValue_1
//   64 	}
//   65 	percent_t newValue = idle->value + delta;
??changeValue_0:
        VLDR     S0,[R4, #+4]
        VADD.F32 S17,S0,S16
//   66 	return setNewValue(idle, currentRpm, now, msg, newValue);
        VMOV.F32 S0,S17
        MOVS     R3,R7
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11setNewValueP14IdleValveStateijPKcf
        BL       _Z11setNewValueP14IdleValveStateijPKcf
??changeValue_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+24
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock5
//   67 }
//   68 
//   69 /**
//   70  * now - current time in milliseconds
//   71  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _ZN14IdleValveState7getIdleEij
        THUMB
//   72 percent_t IdleValveState::getIdle(int currentRpm, efitimems_t now DECLARE_ENGINE_PARAMETER_S) {
_ZN14IdleValveState7getIdleEij:
        PUSH     {R4-R6,R8,R9,LR}
          CFI R14 Frame(CFA, -4)
          CFI R9 Frame(CFA, -8)
          CFI R8 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   73 	if (currentRpm == 0 || isCranking()) {
        CMP      R5,#+0
        BEQ.N    ??getIdle_0
          CFI FunCall _Z10isCrankingv
        BL       _Z10isCrankingv
        CMP      R0,#+0
        BEQ.N    ??getIdle_1
//   74 		return setNewValue(this, currentRpm, now, "cranking value: ", DEFAULT_IDLE_DUTY);
??getIdle_0:
        VLDR.W   S0,??DataTable5_2  ;; 0x42700000
        LDR.N    R3,??DataTable5_11
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11setNewValueP14IdleValveStateijPKcf
        BL       _Z11setNewValueP14IdleValveStateijPKcf
        B.N      ??getIdle_2
//   75 	}
//   76 
//   77 	if (currentRpm < 0.7 * targetRpmRangeLeft) {
??getIdle_1:
        LDR      R0,[R4, #+12]
          CFI FunCall __aeabi_i2d
        BL       __aeabi_i2d
        MOVS     R2,#+1717986918
        LDR.N    R3,??DataTable5_12  ;; 0x3fe66666
          CFI FunCall __aeabi_dmul
        BL       __aeabi_dmul
        MOVS     R2,R0
        MOVS     R3,R1
        MOV      R8,R2
        MOV      R9,R3
        MOVS     R0,R5
          CFI FunCall __aeabi_i2d
        BL       __aeabi_i2d
        MOV      R2,R8
        MOV      R3,R9
          CFI FunCall __aeabi_cdcmple
        BL       __aeabi_cdcmple
        BCS.N    ??getIdle_3
//   78 		return setNewValue(this, currentRpm, now, "RPMs are seriously low: ", lastGoodValue);
        LDR.N    R0,??DataTable5_13
        VLDR     S0,[R0, #0]
        VCVT.F32.S32 S0,S0
        LDR.N    R3,??DataTable5_14
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11setNewValueP14IdleValveStateijPKcf
        BL       _Z11setNewValueP14IdleValveStateijPKcf
        B.N      ??getIdle_2
//   79 	}
//   80 
//   81 	if (now - timeOfLastIdleChange < IDLE_PERIOD) {
??getIdle_3:
        LDR      R0,[R4, #+8]
        SUBS     R0,R6,R0
        CMP      R0,#+1000
        BCS.N    ??getIdle_4
//   82 		// too soon to adjust anything - exiting
//   83 		return value;
        VLDR     S0,[R4, #+4]
        B.N      ??getIdle_2
//   84 	}
//   85 
//   86 	if (currentRpm > targetRpmRangeLeft && currentRpm < targetRpmRangeRight) {
??getIdle_4:
        LDR      R0,[R4, #+12]
        CMP      R0,R5
        BGE.N    ??getIdle_5
        LDR      R0,[R4, #+16]
        CMP      R5,R0
        BGE.N    ??getIdle_5
//   87 		// current RPM is good enough
//   88 		// todo: need idle signal input
//   89 		//lastGoodValue = idle->value;
//   90 		return value;
        VLDR     S0,[R4, #+4]
        B.N      ??getIdle_2
//   91 	}
//   92 
//   93 	if (currentRpm >= targetRpmRangeRight + 100)
??getIdle_5:
        LDR      R0,[R4, #+16]
        ADDS     R0,R0,#+100
        CMP      R5,R0
        BLT.N    ??getIdle_6
//   94 		return changeValue(this, currentRpm, now, "idle control: rpm is too high: ", -IDLE_DECREASE_STEP PASS_ENGINE_PARAMETER);
        VMOV.F32 S0,#-0.5
        LDR.N    R3,??DataTable5_15
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11changeValueP14IdleValveStateiiPKcf
        BL       _Z11changeValueP14IdleValveStateiiPKcf
        B.N      ??getIdle_2
//   95 
//   96 	if (currentRpm >= targetRpmRangeRight)
??getIdle_6:
        LDR      R0,[R4, #+16]
        CMP      R5,R0
        BLT.N    ??getIdle_7
//   97 		return changeValue(this, currentRpm, now, "idle control: rpm is a bit too high: ", -IDLE_DECREASE_SMALL_STEP PASS_ENGINE_PARAMETER);
        VLDR.W   S0,??DataTable5_3  ;; 0xbdcccccd
        LDR.N    R3,??DataTable5_16
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11changeValueP14IdleValveStateiiPKcf
        BL       _Z11changeValueP14IdleValveStateiiPKcf
        B.N      ??getIdle_2
//   98 
//   99 	// we are here if RPM is low, let's see how low
//  100 	if (currentRpm < targetRpmRangeLeft - 100) {
??getIdle_7:
        LDR      R0,[R4, #+12]
        SUBS     R0,R0,#+100
        CMP      R5,R0
        BGE.N    ??getIdle_8
//  101 		return changeValue(this, currentRpm, now, "idle control: RPMs are low: ", IDLE_INCREASE_STEP PASS_ENGINE_PARAMETER);
        VMOV.F32 S0,#0.5
        LDR.N    R3,??DataTable5_17
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11changeValueP14IdleValveStateiiPKcf
        BL       _Z11changeValueP14IdleValveStateiiPKcf
        B.N      ??getIdle_2
//  102 	}
//  103 	return changeValue(this, currentRpm, now, "idle control: RPMs are a bit low: ", IDLE_INCREASE_SMALL_STEP PASS_ENGINE_PARAMETER);
??getIdle_8:
        VLDR.W   S0,??DataTable5_4  ;; 0x3dcccccd
        LDR.N    R3,??DataTable5_18
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11changeValueP14IdleValveStateiiPKcf
        BL       _Z11changeValueP14IdleValveStateiiPKcf
??getIdle_2:
        POP      {R4-R6,R8,R9,PC}  ;; return
          CFI EndBlock cfiBlock6
//  104 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5:
        DC32     0x42b40000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_1:
        DC32     0x40a00001

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_2:
        DC32     0x42700000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_3:
        DC32     0xbdcccccd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_4:
        DC32     0x3dcccccd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_5:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_6:
        DC32     0x5c28f5c3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_7:
        DC32     0x3fedc28f

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_8:
        DC32     0x51eb851f

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_9:
        DC32     0x3ff11eb8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_10:
        DC32     `?<Constant "TPS Lockout, TPS=">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_11:
        DC32     `?<Constant "cranking value: ">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_12:
        DC32     0x3fe66666

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_13:
        DC32     lastGoodValue

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_14:
        DC32     `?<Constant "RPMs are seriously low: ">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_15:
        DC32     `?<Constant "idle control: rpm is ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_16:
        DC32     `?<Constant "idle control: rpm is ...">_1`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_17:
        DC32     `?<Constant "idle control: RPMs ar...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_18:
        DC32     `?<Constant "idle control: RPMs ar...">_1`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TPS Lockout, TPS=">`:
        DATA
        DC8 "TPS Lockout, TPS="
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "cranking value: ">`:
        DATA
        DC8 "cranking value: "
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "RPMs are seriously low: ">`:
        DATA
        DC8 "RPMs are seriously low: "
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "idle control: rpm is ...">`:
        DATA
        DC8 "idle control: rpm is too high: "

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "idle control: rpm is ...">_1`:
        DATA
        DC8 "idle control: rpm is a bit too high: "
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "idle control: RPMs ar...">`:
        DATA
        DC8 "idle control: RPMs are low: "
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "idle control: RPMs ar...">_1`:
        DATA
        DC8 "idle control: RPMs are a bit low: "
        DC8 0

        END
// 
//   4 bytes in section .data
// 208 bytes in section .rodata
// 584 bytes in section .text
// 
// 584 bytes of CODE  memory
// 208 bytes of CONST memory
//   4 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
