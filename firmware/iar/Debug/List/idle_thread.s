///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:34 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\idl /
//                       e_thread.cpp                                         /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\idl /
//                       e_thread.cpp -lCN F:\stuff\rusefi_sourceforge\firmwa /
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
//                       idle_thread.s                                        /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME idle_thread

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z10hwPortname11brain_pin_e
        EXTERN _Z12boolToStringb
        EXTERN _Z12getInputMode16pin_input_mode_e
        EXTERN _Z13interpolate2dfPfS_i
        EXTERN _Z13mySetPadMode2PKc11brain_pin_ej
        EXTERN _Z14getIdle_mode_e11idle_mode_e
        EXTERN _Z17startSimplePwmExtP9PwmConfigPKc11brain_pin_eP9OutputPinffPFvS0_iE
        EXTERN _Z8getHwPin11brain_pin_e
        EXTERN _Z9getHwPort11brain_pin_e
        EXTERN _ZN12StepperMotor10initializeE11brain_pin_eS0_fiS0_
        EXTERN _ZN12StepperMotor17setTargetPositionEi
        EXTERN _ZN12StepperMotorC1Ev
        EXTERN _ZN13RpmCalculator3rpmEv
        EXTERN _ZN14IdleValveState12setTargetRpmEi
        EXTERN _ZN14IdleValveState4initEv
        EXTERN _ZN14IdleValveState7getIdleEij
        EXTERN _ZN14IdleValveStateC1Ev
        EXTERN _ZN9OutputPin8setValueEi
        EXTERN _ZN9OutputPinC1Ev
        EXTERN _ZN9SimplePwm21setSimplePwmDutyCycleEf
        EXTERN _ZN9SimplePwmC1Ev
        EXTERN __aeabi_f2d
        EXTERN absF
        EXTERN addConsoleAction
        EXTERN addConsoleActionI
        EXTERN boardConfiguration
        EXTERN chThdCreateStatic
        EXTERN chThdSleep
        EXTERN config
        EXTERN currentTimeMillis
        EXTERN engine
        EXTERN engineConfiguration
        EXTERN firmwareError
        EXTERN maxF
        EXTERN minF
        EXTERN rlist
        EXTERN scheduleMsg

        PUBLIC _Z15startIdleThreadP7LoggingP6Engine
        PUBLIC _Z9idleDebugPKcf
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\idle_thread.cpp
//    1 /**
//    2  * @file    idle_thread.cpp
//    3  * @brief   Idle Air Control valve thread.
//    4  *
//    5  * This thread looks at current RPM and decides if it should increase or decrease IAC duty cycle.
//    6  * This file is has the hardware & scheduling logic, desired idle level lives separately
//    7  *
//    8  *
//    9  * @date May 23, 2013
//   10  * @author Andrey Belomutskiy, (c) 2012-2015
//   11  *
//   12  * This file is part of rusEfi - see http://rusefi.com
//   13  *
//   14  * rusEfi is free software; you can redistribute it and/or modify it under the terms of
//   15  * the GNU General Public License as published by the Free Software Foundation; either
//   16  * version 3 of the License, or (at your option) any later version.
//   17  *
//   18  * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
//   19  * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   20  * GNU General Public License for more details.
//   21  *
//   22  * You should have received a copy of the GNU General Public License along with this program.
//   23  * If not, see <http://www.gnu.org/licenses/>.
//   24  */
//   25 
//   26 #include "main.h"
//   27 #include "idle_controller.h"
//   28 #include "rpm_calculator.h"
//   29 #include "pwm_generator.h"
//   30 #include "idle_thread.h"
//   31 #include "pin_repository.h"
//   32 #include "engine_configuration.h"
//   33 #include "engine.h"
//   34 #include "stepper.h"
//   35 
//   36 #if EFI_IDLE_CONTROL || defined(__DOXYGEN__)
//   37 

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   38 static THD_WORKING_AREA(ivThreadStack, UTILITY_THREAD_STACK_SIZE);
ivThreadStack:
        DS8 712
//   39 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   40 static Logging *logger;
logger:
        DS8 4
//   41 EXTERN_ENGINE
//   42 ;
//   43 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   44 static OutputPin idleSolenoidPin;
        LDR.W    R0,??DataTable10_1
          CFI FunCall _ZN9OutputPinC1Ev
        BL       _ZN9OutputPinC1Ev
//   45 static SimplePwm idleSolenoid;
        LDR.W    R0,??DataTable10_2
          CFI FunCall _ZN9SimplePwmC1Ev
        BL       _ZN9SimplePwmC1Ev
//   46 
//   47 static StepperMotor iacMotor;
        LDR.W    R0,??DataTable10_3
          CFI FunCall _ZN12StepperMotorC1Ev
        BL       _ZN12StepperMotorC1Ev
//   48 
//   49 /**
//   50  * that's the position with CLT and IAT corrections
//   51  */
//   52 static float actualIdlePosition = -100.0f;
//   53 
//   54 /**
//   55  * Idle level calculation algorithm lives in idle_controller.cpp
//   56  */
//   57 static IdleValveState idleMath;
        LDR.W    R0,??DataTable10_4
          CFI FunCall _ZN14IdleValveStateC1Ev
        BL       _ZN14IdleValveStateC1Ev
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
idleSolenoidPin:
        DS8 16

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
idleSolenoid:
        DS8 112

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
iacMotor:
        DS8 752

        SECTION `.data`:DATA:REORDER:NOROOT(2)
actualIdlePosition:
        DATA
        DC32 0C2C80000H

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
idleMath:
        DS8 20
//   58 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z9idleDebugPKcf
        THUMB
//   59 void idleDebug(const char *msg, percent_t value) {
_Z9idleDebugPKcf:
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
//   60 	scheduleMsg(logger, "%s%f", msg, value);
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        MOVS     R2,R4
        LDR.W    R1,??DataTable10_5
        LDR.W    R0,??DataTable10_6
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   61 }
        ADD      SP,SP,#+8
          CFI CFA R13+16
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1
//   62 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z12showIdleInfov
        THUMB
//   63 static void showIdleInfo(void) {
_Z12showIdleInfov:
        PUSH     {R1-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+24
//   64 	scheduleMsg(logger, "idleMode=%s position=%f isStepper=%s", getIdle_mode_e(engineConfiguration->idleMode),
//   65 			boardConfiguration->idlePosition, boolToString(boardConfiguration->useStepperIdle));
        LDR.W    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+385]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+1
        ANDS     R0,R0,#0x1
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R4,R0
        LDR.W    R0,??DataTable10_8
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+1504]
          CFI FunCall _Z14getIdle_mode_e11idle_mode_e
        BL       _Z14getIdle_mode_e11idle_mode_e
        MOVS     R5,R0
        STR      R4,[SP, #+8]
        LDR.W    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+20]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        MOVS     R2,R5
        LDR.W    R1,??DataTable10_9
        LDR.W    R0,??DataTable10_6
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   66 	if (boardConfiguration->useStepperIdle) {
        LDR.W    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+385]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+1
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??showIdleInfo_0
//   67 		scheduleMsg(logger, "direction=%s reactionTime=%f", hwPortname(boardConfiguration->idle.stepperDirectionPin),
//   68 				engineConfiguration->idleStepperReactionTime);
        LDR.W    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+12]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R4,R0
        LDR.W    R0,??DataTable10_8
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+1548]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        MOVS     R2,R4
        LDR.W    R1,??DataTable10_10
        LDR.W    R0,??DataTable10_6
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   69 		scheduleMsg(logger, "step=%s steps=%d", hwPortname(boardConfiguration->idle.stepperStepPin),
//   70 				engineConfiguration->idleStepperTotalSteps);
        LDR.W    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+16]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        LDR.W    R1,??DataTable10_8
        LDR      R1,[R1, #+0]
        LDR      R3,[R1, #+1636]
        MOVS     R2,R0
        LDR.W    R1,??DataTable10_11
        LDR.W    R0,??DataTable10_6
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   71 		scheduleMsg(logger, "enable=%s", hwPortname(engineConfiguration->stepperEnablePin));
        LDR.W    R0,??DataTable10_8
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+1640]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R2,R0
        LDR.W    R1,??DataTable10_12
        LDR.W    R0,??DataTable10_6
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
        B.N      ??showIdleInfo_1
//   72 	} else {
//   73 		scheduleMsg(logger, "idle valve freq=%d on %s", boardConfiguration->idle.solenoidFrequency,
//   74 				hwPortname(boardConfiguration->idle.solenoidPin));
??showIdleInfo_0:
        LDR.W    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+0]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R3,R0
        LDR.W    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+8]
        LDR.W    R1,??DataTable10_13
        LDR.W    R0,??DataTable10_6
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   75 	}
//   76 }
??showIdleInfo_1:
        POP      {R0-R2,R4,R5,PC}  ;; return
          CFI EndBlock cfiBlock2
//   77 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z21setIdleControlEnabledi
        THUMB
//   78 static void setIdleControlEnabled(int value) {
_Z21setIdleControlEnabledi:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   79 	engineConfiguration->idleMode = value ? IM_AUTO : IM_MANUAL;
        CMP      R4,#+0
        BEQ.N    ??setIdleControlEnabled_0
        MOVS     R0,#+0
        B.N      ??setIdleControlEnabled_1
??setIdleControlEnabled_0:
        MOVS     R0,#+1
??setIdleControlEnabled_1:
        LDR.W    R1,??DataTable10_8
        LDR      R1,[R1, #+0]
        STR      R0,[R1, #+1504]
//   80 	showIdleInfo();
          CFI FunCall _Z12showIdleInfov
        BL       _Z12showIdleInfov
//   81 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//   82 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z15setIdleValvePwmf
        THUMB
//   83 static void setIdleValvePwm(percent_t value) {
_Z15setIdleValvePwmf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//   84 	/**
//   85 	 * currently idle level is an percent value (0-100 range), and PWM takes a float in the 0..1 range
//   86 	 * todo: unify?
//   87 	 */
//   88 	idleSolenoid.setSimplePwmDutyCycle(value / 100);
        VLDR.W   S0,??DataTable9  ;; 0x42c80000
        VDIV.F32 S0,S16,S0
        LDR.W    R0,??DataTable10_2
          CFI FunCall _ZN9SimplePwm21setSimplePwmDutyCycleEf
        BL       _ZN9SimplePwm21setSimplePwmDutyCycleEf
//   89 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock4
//   90 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z22doSetIdleValvePositioni
        THUMB
//   91 static void doSetIdleValvePosition(int positionPercent) {
_Z22doSetIdleValvePositioni:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV     S16,R0
//   92 	boardConfiguration->idlePosition = positionPercent;
        VCVT.F32.S32 S0,S16
        LDR.W    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        VSTR     S0,[R0, #+20]
//   93 
//   94 	percent_t cltCorrectedPosition = interpolate2d(engine->engineState.clt, config->cltIdleCorrBins, config->cltIdleCorr,
//   95 	CLT_CURVE_SIZE) * positionPercent;
        VCVT.F32.S32 S17,S16
        MOVS     R2,#+16
        LDR.W    R0,??DataTable10_14
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+6144
        ADDS     R1,R1,#+120
        LDR.W    R0,??DataTable10_14
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+6144
        ADDS     R0,R0,#+56
        LDR.W    R3,??DataTable10_15
        LDR      R3,[R3, #+0]
        ADD      R3,R3,#+7680
        ADDS     R3,R3,#+84
        VLDR     S0,[R3, #0]
          CFI FunCall _Z13interpolate2dfPfS_i
        BL       _Z13interpolate2dfPfS_i
        VMUL.F32 S17,S17,S0
//   96 
//   97 	// let's put the value into the right range
//   98 	cltCorrectedPosition = maxF(cltCorrectedPosition, 0.01);
        VLDR.W   S1,??DataTable9_1  ;; 0x3c23d70a
        VMOV.F32 S0,S17
          CFI FunCall maxF
        BL       maxF
        VMOV.F32 S17,S0
//   99 	cltCorrectedPosition = minF(cltCorrectedPosition, 99.9);
        VLDR.W   S1,??DataTable9_2  ;; 0x42c7cccd
        VMOV.F32 S0,S17
          CFI FunCall minF
        BL       minF
        VMOV.F32 S17,S0
//  100 
//  101 	if (absF(cltCorrectedPosition - actualIdlePosition) < 1) {
        LDR.W    R0,??DataTable10_16
        VLDR     S0,[R0, #0]
        VSUB.F32 S0,S17,S0
          CFI FunCall absF
        BL       absF
        VMOV.F32 S1,#1.0
        VCMP.F32 S0,S1
        FMSTAT   
        BMI.N    ??doSetIdleValvePosition_0
//  102 		return; // value is pretty close, let's leave the poor valve alone
//  103 	}
//  104 	actualIdlePosition = cltCorrectedPosition;
??doSetIdleValvePosition_1:
        LDR.N    R0,??DataTable10_16
        VSTR     S17,[R0, #0]
//  105 
//  106 	if (boardConfiguration->useStepperIdle) {
        LDR.N    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+385]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+1
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??doSetIdleValvePosition_2
//  107 		iacMotor.setTargetPosition(cltCorrectedPosition / 100 * engineConfiguration->idleStepperTotalSteps);
        VLDR.W   S0,??DataTable9  ;; 0x42c80000
        VDIV.F32 S0,S17,S0
        LDR.N    R0,??DataTable10_8
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+1636]
        VMOV     S1,R0
        VCVT.F32.S32 S1,S1
        VMUL.F32 S0,S0,S1
        VCVT.S32.F32 S0,S0
        VMOV     R1,S0
        LDR.N    R0,??DataTable10_3
          CFI FunCall _ZN12StepperMotor17setTargetPositionEi
        BL       _ZN12StepperMotor17setTargetPositionEi
        B.N      ??doSetIdleValvePosition_3
//  108 	} else {
//  109 		setIdleValvePwm(cltCorrectedPosition);
??doSetIdleValvePosition_2:
        VMOV.F32 S0,S17
          CFI FunCall _Z15setIdleValvePwmf
        BL       _Z15setIdleValvePwmf
//  110 	}
//  111 }
??doSetIdleValvePosition_3:
??doSetIdleValvePosition_0:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock5
//  112 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z20setIdleValvePositioni
        THUMB
//  113 static void setIdleValvePosition(int positionPercent) {
_Z20setIdleValvePositioni:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  114 	if (positionPercent < 1 || positionPercent > 99)
        CMP      R4,#+1
        BLT.N    ??setIdleValvePosition_0
        CMP      R4,#+100
        BLT.N    ??setIdleValvePosition_1
//  115 		return;
??setIdleValvePosition_0:
        B.N      ??setIdleValvePosition_2
//  116 	scheduleMsg(logger, "setting idle valve position %d", positionPercent);
??setIdleValvePosition_1:
        MOVS     R2,R4
        LDR.N    R1,??DataTable10_17
        LDR.N    R0,??DataTable10_6
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  117 	showIdleInfo();
          CFI FunCall _Z12showIdleInfov
        BL       _Z12showIdleInfov
//  118 	doSetIdleValvePosition(positionPercent);
        MOVS     R0,R4
          CFI FunCall _Z22doSetIdleValvePositioni
        BL       _Z22doSetIdleValvePositioni
//  119 }
??setIdleValvePosition_2:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock6
//  120 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z8ivThreadi
        THUMB
//  121 static msg_t ivThread(int param) {
_Z8ivThreadi:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        VPUSH    {D8}
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
//  122 	(void) param;
//  123 	chRegSetThreadName("IdleValve");
        LDR.N    R0,??DataTable10_18
        LDR      R0,[R0, #+24]
        LDR.N    R1,??DataTable10_19
        STR      R1,[R0, #+24]
//  124 
//  125 	percent_t currentIdleValve = -1;
        VMOV.F32 S16,#-1.0
//  126 	while (true) {
//  127 		chThdSleepMilliseconds(boardConfiguration->idleThreadPeriod);
??ivThread_0:
        LDR.N    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+312]
        MOV      R1,#+1000
        MULS     R0,R1,R0
        SUBS     R0,R0,#+1
        MOV      R1,#+1000
        UDIV     R0,R0,R1
        ADDS     R0,R0,#+1
          CFI FunCall chThdSleep
        BL       chThdSleep
//  128 
//  129 		// this value is not used yet
//  130 		if (boardConfiguration->clutchDownPin != GPIO_UNASSIGNED) {
        LDR.N    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+160]
        CMP      R0,#+80
        BEQ.N    ??ivThread_1
//  131 			engine->clutchDownState = palReadPad(getHwPort(boardConfiguration->clutchDownPin),
//  132 					getHwPin(boardConfiguration->clutchDownPin));
        LDR.N    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+160]
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        LDR      R5,[R0, #+16]
        LDR.N    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+160]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        LSRS     R0,R5,R0
        ANDS     R0,R0,#0x1
        LDR.N    R1,??DataTable10_15
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+613]
//  133 		}
//  134 		if (boardConfiguration->clutchUpPin != GPIO_UNASSIGNED) {
??ivThread_1:
        LDR.N    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+796]
        CMP      R0,#+80
        BEQ.N    ??ivThread_2
//  135 			engine->clutchUpState = palReadPad(getHwPort(boardConfiguration->clutchUpPin),
//  136 					getHwPin(boardConfiguration->clutchUpPin));
        LDR.N    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+796]
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        LDR      R5,[R0, #+16]
        LDR.N    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+796]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        LSRS     R0,R5,R0
        ANDS     R0,R0,#0x1
        LDR.N    R1,??DataTable10_15
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+612]
//  137 		}
//  138 
//  139 		if (engineConfiguration->idleMode != IM_AUTO) {
??ivThread_2:
        LDR.N    R0,??DataTable10_8
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+1504]
        CMP      R0,#+0
        BEQ.N    ??ivThread_3
//  140 			// let's re-apply CLT correction
//  141 			doSetIdleValvePosition(boardConfiguration->idlePosition);
        LDR.N    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        VLDR     S0,[R0, #+20]
        VCVT.S32.F32 S0,S0
        VMOV     R0,S0
          CFI FunCall _Z22doSetIdleValvePositioni
        BL       _Z22doSetIdleValvePositioni
//  142 			continue;
        B.N      ??ivThread_0
//  143 		}
//  144 
//  145 		efitimems_t now = currentTimeMillis();
??ivThread_3:
          CFI FunCall currentTimeMillis
        BL       currentTimeMillis
        MOVS     R5,R0
//  146 
//  147 		percent_t newValue = idleMath.getIdle(getRpm(), now PASS_ENGINE_PARAMETER);
        LDR.N    R0,??DataTable10_15
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN13RpmCalculator3rpmEv
        BL       _ZN13RpmCalculator3rpmEv
        MOVS     R1,R0
        MOVS     R2,R5
        LDR.N    R0,??DataTable10_4
          CFI FunCall _ZN14IdleValveState7getIdleEij
        BL       _ZN14IdleValveState7getIdleEij
        VMOV.F32 S17,S0
//  148 
//  149 		if (currentIdleValve != newValue) {
        VCMP.F32 S16,S17
        FMSTAT   
        BEQ.N    ??ivThread_0
//  150 			currentIdleValve = newValue;
        VMOV.F32 S16,S17
//  151 
//  152 			setIdleValvePwm(newValue);
        VMOV.F32 S0,S17
          CFI FunCall _Z15setIdleValvePwmf
        BL       _Z15setIdleValvePwmf
        B.N      ??ivThread_0
          CFI EndBlock cfiBlock7
//  153 		}
//  154 	}
//  155 #if defined __GNUC__
//  156 	return -1;
//  157 #endif
//  158 }
//  159 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z16setIdleRpmActioni
        THUMB
//  160 static void setIdleRpmAction(int value) {
_Z16setIdleRpmActioni:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  161 	idleMath.setTargetRpm(value);
        MOVS     R1,R4
        LDR.N    R0,??DataTable10_4
          CFI FunCall _ZN14IdleValveState12setTargetRpmEi
        BL       _ZN14IdleValveState12setTargetRpmEi
//  162 	scheduleMsg(logger, "target idle RPM %d", value);
        MOVS     R2,R4
        LDR.N    R1,??DataTable10_20
        LDR.N    R0,??DataTable10_6
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  163 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock8
//  164 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z25applyIdleSolenoidPinStateP9PwmConfigi
        THUMB
//  165 static void applyIdleSolenoidPinState(PwmConfig *state, int stateIndex) {
_Z25applyIdleSolenoidPinStateP9PwmConfigi:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  166 	efiAssertVoid(stateIndex < PWM_PHASE_MAX_COUNT, "invalid stateIndex");
        CMP      R5,#+252
        BLT.N    ??applyIdleSolenoidPinState_0
        LDR.N    R0,??DataTable10_21
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??applyIdleSolenoidPinState_1
//  167 	efiAssertVoid(state->multiWave.waveCount == 1, "invalid idle waveCount");
??applyIdleSolenoidPinState_0:
        LDR      R0,[R4, #+12]
        CMP      R0,#+1
        BEQ.N    ??applyIdleSolenoidPinState_2
        LDR.N    R0,??DataTable10_22
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??applyIdleSolenoidPinState_1
//  168 	OutputPin *output = state->outputPins[0];
??applyIdleSolenoidPinState_2:
        LDR      R6,[R4, #+0]
//  169 	int value = state->multiWave.waves[0].pinStates[stateIndex];
        LDR      R0,[R4, #+16]
        LDR      R0,[R0, #+0]
        LDRSB    R7,[R5, R0]
//  170 	if (!value || engine->rpmCalculator.rpmValue != 0)
        CMP      R7,#+0
        BEQ.N    ??applyIdleSolenoidPinState_3
        LDR.N    R0,??DataTable10_15
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??applyIdleSolenoidPinState_4
//  171 		output->setValue(value);
??applyIdleSolenoidPinState_3:
        MOVS     R1,R7
        MOVS     R0,R6
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
//  172 }
??applyIdleSolenoidPinState_4:
??applyIdleSolenoidPinState_1:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock9

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9:
        DC32     0x42c80000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_1:
        DC32     0x3c23d70a

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_2:
        DC32     0x42c7cccd
//  173 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z15startIdleThreadP7LoggingP6Engine
        THUMB
//  174 void startIdleThread(Logging*sharedLogger, Engine *engine) {
_Z15startIdleThreadP7LoggingP6Engine:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  175 	logger = sharedLogger;
        LDR.N    R0,??DataTable10_6
        STR      R4,[R0, #+0]
//  176 
//  177 	if (boardConfiguration->useStepperIdle) {
        LDR.N    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+385]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+1
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??startIdleThread_0
//  178 		iacMotor.initialize(boardConfiguration->idle.stepperStepPin, boardConfiguration->idle.stepperDirectionPin,
//  179 				engineConfiguration->idleStepperReactionTime, engineConfiguration->idleStepperTotalSteps,
//  180 				engineConfiguration->stepperEnablePin);
        LDR.N    R0,??DataTable10_8
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+1640]
        STR      R0,[SP, #+0]
        LDR.N    R0,??DataTable10_8
        LDR      R0,[R0, #+0]
        LDR      R3,[R0, #+1636]
        LDR.N    R0,??DataTable10_8
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+1548]
        VMOV     S0,R0
        LDR.N    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+12]
        LDR.N    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+16]
        LDR.N    R0,??DataTable10_3
          CFI FunCall _ZN12StepperMotor10initializeE11brain_pin_eS0_fiS0_
        BL       _ZN12StepperMotor10initializeE11brain_pin_eS0_fiS0_
        B.N      ??startIdleThread_1
//  181 	} else {
//  182 		/**
//  183 		 * Start PWM for idleValvePin
//  184 		 */
//  185 		startSimplePwmExt(&idleSolenoid, "Idle Valve", boardConfiguration->idle.solenoidPin, &idleSolenoidPin,
//  186 				boardConfiguration->idle.solenoidFrequency, boardConfiguration->idlePosition / 100,
//  187 				applyIdleSolenoidPinState);
??startIdleThread_0:
        LDR.N    R0,??DataTable10_23
        STR      R0,[SP, #+0]
        LDR.N    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        VLDR     S0,[R0, #+20]
        VLDR.W   S1,??DataTable10  ;; 0x42c80000
        VDIV.F32 S1,S0,S1
        LDR.N    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        VLDR     S0,[R0, #+8]
        VCVT.F32.S32 S0,S0
        LDR.N    R3,??DataTable10_1
        LDR.N    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+0]
        LDR.N    R1,??DataTable10_24
        LDR.N    R0,??DataTable10_2
          CFI FunCall _Z17startSimplePwmExtP9PwmConfigPKc11brain_pin_eP9OutputPinffPFvS0_iE
        BL       _Z17startSimplePwmExtP9PwmConfigPKc11brain_pin_eP9OutputPinffPFvS0_iE
//  188 	}
//  189 
//  190 	idleMath.init();
??startIdleThread_1:
        LDR.N    R0,??DataTable10_4
          CFI FunCall _ZN14IdleValveState4initEv
        BL       _ZN14IdleValveState4initEv
//  191 	scheduleMsg(logger, "initial idle %d", idleMath.value);
        LDR.N    R0,??DataTable10_4
        LDR      R0,[R0, #+4]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable10_25
        LDR.N    R0,??DataTable10_6
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  192 
//  193 	chThdCreateStatic(ivThreadStack, sizeof(ivThreadStack), NORMALPRIO, (tfunc_t) ivThread, NULL);
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        LDR.N    R3,??DataTable10_26
        MOVS     R2,#+64
        MOV      R1,#+712
        LDR.N    R0,??DataTable10_27
          CFI FunCall chThdCreateStatic
        BL       chThdCreateStatic
//  194 
//  195 	// this is idle switch INPUT - sometimes there is a switch on the throttle pedal
//  196 	// this switch is not used yet
//  197 	if (boardConfiguration->clutchDownPin != GPIO_UNASSIGNED)
        LDR.N    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+160]
        CMP      R0,#+80
        BEQ.N    ??startIdleThread_2
//  198 		mySetPadMode2("clutch down switch", boardConfiguration->clutchDownPin,
//  199 				getInputMode(boardConfiguration->clutchDownPinMode));
        LDR.N    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+164]
          CFI FunCall _Z12getInputMode16pin_input_mode_e
        BL       _Z12getInputMode16pin_input_mode_e
        MOVS     R2,R0
        LDR.N    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+160]
        LDR.N    R0,??DataTable10_28
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
//  200 
//  201 	if (boardConfiguration->clutchUpPin != GPIO_UNASSIGNED)
??startIdleThread_2:
        LDR.N    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+796]
        CMP      R0,#+80
        BEQ.N    ??startIdleThread_3
//  202 		mySetPadMode2("clutch up switch", boardConfiguration->clutchUpPin,
//  203 				getInputMode(boardConfiguration->clutchUpPinMode));
        LDR.N    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+800]
          CFI FunCall _Z12getInputMode16pin_input_mode_e
        BL       _Z12getInputMode16pin_input_mode_e
        MOVS     R2,R0
        LDR.N    R0,??DataTable10_7
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+796]
        LDR.N    R0,??DataTable10_29
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
//  204 
//  205 	addConsoleAction("idleinfo", showIdleInfo);
??startIdleThread_3:
        LDR.N    R1,??DataTable10_30
        LDR.N    R0,??DataTable10_31
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  206 	addConsoleActionI("set_idle_rpm", setIdleRpmAction);
        LDR.N    R1,??DataTable10_32
        LDR.N    R0,??DataTable10_33
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  207 
//  208 	addConsoleActionI("set_idle_position", setIdleValvePosition);
        LDR.N    R1,??DataTable10_34
        LDR.N    R0,??DataTable10_35
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  209 
//  210 	addConsoleActionI("set_idle_enabled", (VoidInt) setIdleControlEnabled);
        LDR.N    R1,??DataTable10_36
        LDR.N    R0,??DataTable10_37
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  211 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock10

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10:
        DC32     0x42c80000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_1:
        DC32     idleSolenoidPin

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_2:
        DC32     idleSolenoid

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_3:
        DC32     iacMotor

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_4:
        DC32     idleMath

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_5:
        DC32     `?<Constant "%s%f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_6:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_7:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_8:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_9:
        DC32     `?<Constant "idleMode=%s position=...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_10:
        DC32     `?<Constant "direction=%s reaction...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_11:
        DC32     `?<Constant "step=%s steps=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_12:
        DC32     `?<Constant "enable=%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_13:
        DC32     `?<Constant "idle valve freq=%d on %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_14:
        DC32     config

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_15:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_16:
        DC32     actualIdlePosition

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_17:
        DC32     `?<Constant "setting idle valve po...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_18:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_19:
        DC32     `?<Constant "IdleValve">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_20:
        DC32     `?<Constant "target idle RPM %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_21:
        DC32     `?<Constant "invalid stateIndex">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_22:
        DC32     `?<Constant "invalid idle waveCount">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_23:
        DC32     _Z25applyIdleSolenoidPinStateP9PwmConfigi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_24:
        DC32     `?<Constant "Idle Valve">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_25:
        DC32     `?<Constant "initial idle %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_26:
        DC32     _Z8ivThreadi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_27:
        DC32     ivThreadStack

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_28:
        DC32     `?<Constant "clutch down switch">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_29:
        DC32     `?<Constant "clutch up switch">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_30:
        DC32     _Z12showIdleInfov

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_31:
        DC32     `?<Constant "idleinfo">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_32:
        DC32     _Z16setIdleRpmActioni

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_33:
        DC32     `?<Constant "set_idle_rpm">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_34:
        DC32     _Z20setIdleValvePositioni

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_35:
        DC32     `?<Constant "set_idle_position">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_36:
        DC32     _Z21setIdleControlEnabledi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_37:
        DC32     `?<Constant "set_idle_enabled">`

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
`?<Constant "%s%f">`:
        DATA
        DC8 "%s%f"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "idleMode=%s position=...">`:
        DATA
        DC8 "idleMode=%s position=%f isStepper=%s"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "direction=%s reaction...">`:
        DATA
        DC8 "direction=%s reactionTime=%f"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "step=%s steps=%d">`:
        DATA
        DC8 "step=%s steps=%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "enable=%s">`:
        DATA
        DC8 "enable=%s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "idle valve freq=%d on %s">`:
        DATA
        DC8 "idle valve freq=%d on %s"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "setting idle valve po...">`:
        DATA
        DC8 "setting idle valve position %d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "IdleValve">`:
        DATA
        DC8 "IdleValve"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "target idle RPM %d">`:
        DATA
        DC8 "target idle RPM %d"
        DC8 0

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
`?<Constant "Idle Valve">`:
        DATA
        DC8 "Idle Valve"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "initial idle %d">`:
        DATA
        DC8 "initial idle %d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "clutch down switch">`:
        DATA
        DC8 "clutch down switch"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "clutch up switch">`:
        DATA
        DC8 "clutch up switch"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "idleinfo">`:
        DATA
        DC8 "idleinfo"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_idle_rpm">`:
        DATA
        DC8 "set_idle_rpm"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_idle_position">`:
        DATA
        DC8 "set_idle_position"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_idle_enabled">`:
        DATA
        DC8 "set_idle_enabled"
        DC8 0, 0, 0

        END
//  212 
//  213 #endif
// 
// 1 616 bytes in section .bss
//     4 bytes in section .data
//     4 bytes in section .init_array
//   384 bytes in section .rodata
// 1 450 bytes in section .text
// 
// 1 454 bytes of CODE  memory
//   384 bytes of CONST memory
// 1 620 bytes of DATA  memory
//
//Errors: none
//Warnings: 3
