///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:04 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\steppe /
//                       r.cpp                                                /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\steppe /
//                       r.cpp -lCN F:\stuff\rusefi_sourceforge\firmware\iar\ /
//                       Debug\List\ -lA F:\stuff\rusefi_sourceforge\firmware /
//                       \iar\Debug\List\ -o F:\stuff\rusefi_sourceforge\firm /
//                       ware\iar\Debug\Obj\ --no_cse --no_unroll             /
//                       --no_inline --no_code_motion --no_tbaa               /
//                       --no_clustering --no_scheduling --debug              /
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
//                       stepper.s                                            /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME stepper

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z13mySetPadMode2PKc11brain_pin_ej
        EXTERN _Z8getHwPin11brain_pin_e
        EXTERN _Z9getHwPort11brain_pin_e
        EXTERN chThdCreateStatic
        EXTERN chThdSleep
        EXTERN maxF
        EXTERN maxI
        EXTERN rlist

        PUBLIC _ZN12StepperMotor10initializeE11brain_pin_eS0_fiS0_
        PUBLIC _ZN12StepperMotor17getTargetPositionEv
        PUBLIC _ZN12StepperMotor17setTargetPositionEi
        PUBLIC _ZN12StepperMotor5pulseEv
        PUBLIC _ZN12StepperMotorC1Ev
        PUBLIC _ZN12StepperMotorC2Ev
        
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
          CFI Function _ZN12StepperMotorC2Ev
        THUMB
// __code __interwork __softfp StepperMotor::subobject StepperMotor()
_ZN12StepperMotorC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN12StepperMotorC1Ev
        BL       _ZN12StepperMotorC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0
// F:\stuff\rusefi_sourceforge\firmware\hw_layer\stepper.cpp
//    1 /**
//    2  * @file	stepper.cpp
//    3  *
//    4  * http://rusefi.com/wiki/index.php?title=Hardware:Stepper_motor
//    5  *
//    6  * @date Dec 24, 2014
//    7  * @author Andrey Belomutskiy, (c) 2012-2015
//    8  */
//    9 
//   10 #include "stepper.h"
//   11 #include "pin_repository.h"
//   12 
//   13 #define ST_DELAY_MS 10
//   14 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z8stThreadP12StepperMotor
        THUMB
//   15 static msg_t stThread(StepperMotor *motor) {
_Z8stThreadP12StepperMotor:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
//   16 	chRegSetThreadName("stepper");
        LDR.N    R0,??DataTable1
        LDR      R0,[R0, #+24]
        LDR.N    R1,??DataTable1_1
        STR      R1,[R0, #+24]
//   17 
//   18 	palWritePad(motor->directionPort, motor->directionPin, false);
        MOVS     R0,#+1
        LDR      R1,[R4, #+4]
        ADDS     R1,R1,#+16
        LSLS     R0,R0,R1
        MOVS     R1,#+0
        LDR      R2,[R4, #+4]
        LSLS     R1,R1,R2
        ORRS     R0,R1,R0
        LDR      R1,[R4, #+0]
        STR      R0,[R1, #+24]
//   19 
//   20 	// let's park the motor in a known position to begin with
//   21 	for (int i = 0; i < motor->totalSteps; i++) {
        MOVS     R5,#+0
??stThread_0:
        LDR      R0,[R4, #+16]
        CMP      R5,R0
        BGE.N    ??stThread_1
//   22 		motor->pulse();
        MOVS     R0,R4
          CFI FunCall _ZN12StepperMotor5pulseEv
        BL       _ZN12StepperMotor5pulseEv
//   23 	}
        ADDS     R5,R5,#+1
        B.N      ??stThread_0
//   24 
//   25 	while (true) {
//   26 		int targetPosition = motor->getTargetPosition();
??stThread_1:
        MOVS     R0,R4
          CFI FunCall _ZN12StepperMotor17getTargetPositionEv
        BL       _ZN12StepperMotor17getTargetPositionEv
        MOVS     R5,R0
//   27 		int currentPosition = motor->currentPosition;
        LDR      R6,[R4, #+8]
//   28 
//   29 		if (targetPosition == currentPosition) {
        CMP      R5,R6
        BNE.N    ??stThread_2
//   30 			chThdSleepMilliseconds(ST_DELAY_MS);
        MOVS     R0,#+10
          CFI FunCall chThdSleep
        BL       chThdSleep
//   31 			continue;
        B.N      ??stThread_1
//   32 		}
//   33 		bool_t isIncrementing = targetPosition > currentPosition;
??stThread_2:
        CMP      R6,R5
        BGE.N    ??stThread_3
        MOVS     R7,#+1
        B.N      ??stThread_4
??stThread_3:
        MOVS     R7,#+0
//   34 		palWritePad(motor->directionPort, motor->directionPin, isIncrementing);
??stThread_4:
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        MVNS     R0,R7
        ANDS     R0,R0,#0x1
        LDR      R1,[R4, #+4]
        ADDS     R1,R1,#+16
        LSLS     R0,R0,R1
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        LDR      R1,[R4, #+4]
        LSLS     R1,R7,R1
        ORRS     R0,R1,R0
        LDR      R1,[R4, #+0]
        STR      R0,[R1, #+24]
//   35 		if (isIncrementing) {
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+0
        BEQ.N    ??stThread_5
//   36 			motor->currentPosition++;
        LDR      R0,[R4, #+8]
        ADDS     R0,R0,#+1
        STR      R0,[R4, #+8]
        B.N      ??stThread_6
//   37 		} else {
//   38 			motor->currentPosition--;
??stThread_5:
        LDR      R0,[R4, #+8]
        SUBS     R0,R0,#+1
        STR      R0,[R4, #+8]
//   39 		}
//   40 		motor->pulse();
??stThread_6:
        MOVS     R0,R4
          CFI FunCall _ZN12StepperMotor5pulseEv
        BL       _ZN12StepperMotor5pulseEv
        B.N      ??stThread_1
          CFI EndBlock cfiBlock1
//   41 	}
//   42 
//   43 	// let's part the motor in a known position to begin with
//   44 //	for (int i = 0; i < ST_COUNT / 2; i++) {
//   45 //		motor->pulse();
//   46 //	}
//   47 
//   48 	return 0;
//   49 }
//   50 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _ZN12StepperMotorC1Ev
          CFI NoCalls
        THUMB
//   51 StepperMotor::StepperMotor() {
//   52 	currentPosition = 0;
_ZN12StepperMotorC1Ev:
        MOVS     R1,#+0
        STR      R1,[R0, #+8]
//   53 	targetPosition = 0;
        MOVS     R1,#+0
        STR      R1,[R0, #+20]
//   54 	directionPort = NULL;
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//   55 	directionPin = 0;
        MOVS     R1,#+0
        STR      R1,[R0, #+4]
//   56 	enablePort = NULL;
        MOVS     R1,#+0
        STR      R1,[R0, #+32]
//   57 	enablePin = 0;
        MOVS     R1,#+0
        STR      R1,[R0, #+36]
//   58 	stepPort = NULL;
        MOVS     R1,#+0
        STR      R1,[R0, #+24]
//   59 	stepPin = 0;
        MOVS     R1,#+0
        STR      R1,[R0, #+28]
//   60 	reactionTime = 0;
        MOVS     R1,#+0
        STR      R1,[R0, #+12]
//   61 	totalSteps = 0;
        MOVS     R1,#+0
        STR      R1,[R0, #+16]
//   62 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock2
//   63 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _ZN12StepperMotor17getTargetPositionEv
          CFI NoCalls
        THUMB
//   64 int StepperMotor::getTargetPosition() {
//   65 	return targetPosition;
_ZN12StepperMotor17getTargetPositionEv:
        LDR      R0,[R0, #+20]
        BX       LR               ;; return
          CFI EndBlock cfiBlock3
//   66 }
//   67 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _ZN12StepperMotor17setTargetPositionEi
          CFI NoCalls
        THUMB
//   68 void StepperMotor::setTargetPosition(int targetPosition) {
//   69 	this->targetPosition = targetPosition;
_ZN12StepperMotor17setTargetPositionEi:
        STR      R1,[R0, #+20]
//   70 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//   71 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _ZN12StepperMotor5pulseEv
        THUMB
//   72 void StepperMotor::pulse() {
_ZN12StepperMotor5pulseEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   73 	palWritePad(enablePort, enablePin, false); // ebable stepper
        MOVS     R0,#+1
        LDR      R1,[R4, #+36]
        ADDS     R1,R1,#+16
        LSLS     R0,R0,R1
        MOVS     R1,#+0
        LDR      R2,[R4, #+36]
        LSLS     R1,R1,R2
        ORRS     R0,R1,R0
        LDR      R1,[R4, #+32]
        STR      R0,[R1, #+24]
//   74 
//   75 	palWritePad(stepPort, stepPin, true);
        MOVS     R0,#+0
        LDR      R1,[R4, #+28]
        ADDS     R1,R1,#+16
        LSLS     R0,R0,R1
        MOVS     R1,#+1
        LDR      R2,[R4, #+28]
        LSLS     R1,R1,R2
        ORRS     R0,R1,R0
        LDR      R1,[R4, #+24]
        STR      R0,[R1, #+24]
//   76 	chThdSleepMilliseconds(ST_DELAY_MS);
        MOVS     R0,#+10
          CFI FunCall chThdSleep
        BL       chThdSleep
//   77 	palWritePad(stepPort, stepPin, false);
        MOVS     R0,#+1
        LDR      R1,[R4, #+28]
        ADDS     R1,R1,#+16
        LSLS     R0,R0,R1
        MOVS     R1,#+0
        LDR      R2,[R4, #+28]
        LSLS     R1,R1,R2
        ORRS     R0,R1,R0
        LDR      R1,[R4, #+24]
        STR      R0,[R1, #+24]
//   78 	chThdSleepMilliseconds(ST_DELAY_MS);
        MOVS     R0,#+10
          CFI FunCall chThdSleep
        BL       chThdSleep
//   79 
//   80 	palWritePad(enablePort, enablePin, true); // disable stepper
        MOVS     R0,#+0
        LDR      R1,[R4, #+36]
        ADDS     R1,R1,#+16
        LSLS     R0,R0,R1
        MOVS     R1,#+1
        LDR      R2,[R4, #+36]
        LSLS     R1,R1,R2
        ORRS     R0,R1,R0
        LDR      R1,[R4, #+32]
        STR      R0,[R1, #+24]
//   81 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock5
//   82 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _ZN12StepperMotor10initializeE11brain_pin_eS0_fiS0_
        THUMB
//   83 void StepperMotor::initialize(brain_pin_e stepPin, brain_pin_e directionPin, float reactionTime, int totalSteps,
//   84 		brain_pin_e enablePin) {
_ZN12StepperMotor10initializeE11brain_pin_eS0_fiS0_:
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
        MOVS     R6,R2
        VMOV.F32 S16,S0
        MOVS     R7,R3
        LDR      R8,[SP, #+40]
//   85 	this->reactionTime = maxF(1, reactionTime);
        VMOV.F32 S1,S16
        VMOV.F32 S0,#1.0
          CFI FunCall maxF
        BL       maxF
        VSTR     S0,[R4, #+12]
//   86 	this->totalSteps = maxI(3, totalSteps);
        MOVS     R1,R7
        MOVS     R0,#+3
          CFI FunCall maxI
        BL       maxI
        STR      R0,[R4, #+16]
//   87 	if (stepPin == GPIO_UNASSIGNED || directionPin == GPIO_UNASSIGNED) {
        CMP      R5,#+80
        BEQ.N    ??initialize_0
        CMP      R6,#+80
        BNE.N    ??initialize_1
//   88 		return;
??initialize_0:
        B.N      ??initialize_2
//   89 	}
//   90 
//   91 	stepPort = getHwPort(stepPin);
??initialize_1:
        MOVS     R0,R5
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        STR      R0,[R4, #+24]
//   92 	this->stepPin = getHwPin(stepPin);
        MOVS     R0,R5
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        STR      R0,[R4, #+28]
//   93 
//   94 	directionPort = getHwPort(directionPin);
        MOVS     R0,R6
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        STR      R0,[R4, #+0]
//   95 	this->directionPin = getHwPin(directionPin);
        MOVS     R0,R6
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        STR      R0,[R4, #+4]
//   96 
//   97 	enablePort = getHwPort(enablePin);
        MOV      R0,R8
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        STR      R0,[R4, #+32]
//   98 	this->enablePin = getHwPin(enablePin);
        MOV      R0,R8
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        STR      R0,[R4, #+36]
//   99 
//  100 	mySetPadMode2("stepper step", stepPin, PAL_MODE_OUTPUT_PUSHPULL);
        MOVS     R2,#+1
        MOVS     R1,R5
        LDR.N    R0,??DataTable1_2
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
//  101 	mySetPadMode2("stepper dir", directionPin, PAL_MODE_OUTPUT_PUSHPULL);
        MOVS     R2,#+1
        MOVS     R1,R6
        LDR.N    R0,??DataTable1_3
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
//  102 	mySetPadMode2("stepper enable", enablePin, PAL_MODE_OUTPUT_PUSHPULL);
        MOVS     R2,#+1
        MOV      R1,R8
        LDR.N    R0,??DataTable1_4
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
//  103 	palWritePad(this->enablePort, enablePin, true); // disable stepper
        MOVS     R0,#+0
        ADDS     R1,R8,#+16
        LSLS     R0,R0,R1
        MOVS     R1,#+1
        LSLS     R1,R1,R8
        ORRS     R0,R1,R0
        LDR      R1,[R4, #+32]
        STR      R0,[R1, #+24]
//  104 
//  105 	chThdCreateStatic(stThreadStack, sizeof(stThreadStack), NORMALPRIO, (tfunc_t) stThread, this);
        STR      R4,[SP, #+0]
        LDR.N    R3,??DataTable1_5
        MOVS     R2,#+64
        MOV      R1,#+712
        ADDS     R0,R4,#+40
          CFI FunCall chThdCreateStatic
        BL       chThdCreateStatic
//  106 }
??initialize_2:
        ADD      SP,SP,#+8
          CFI CFA R13+32
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+24
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock6

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_1:
        DC32     `?<Constant "stepper">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_2:
        DC32     `?<Constant "stepper step">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_3:
        DC32     `?<Constant "stepper dir">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_4:
        DC32     `?<Constant "stepper enable">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_5:
        DC32     _Z8stThreadP12StepperMotor

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "stepper">`:
        DATA
        DC8 "stepper"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "stepper step">`:
        DATA
        DC8 "stepper step"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "stepper dir">`:
        DATA
        DC8 "stepper dir"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "stepper enable">`:
        DATA
        DC8 "stepper enable"
        DC8 0

        END
//  107 
// 
//  52 bytes in section .rodata
// 510 bytes in section .text
// 
// 510 bytes of CODE  memory
//  52 bytes of CONST memory
//
//Errors: none
//Warnings: 1
