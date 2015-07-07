///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:30 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\joysti /
//                       ck.cpp                                               /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\joysti /
//                       ck.cpp -lCN F:\stuff\rusefi_sourceforge\firmware\iar /
//                       \Debug\List\ -lA F:\stuff\rusefi_sourceforge\firmwar /
//                       e\iar\Debug\List\ -o F:\stuff\rusefi_sourceforge\fir /
//                       mware\iar\Debug\Obj\ --no_cse --no_unroll            /
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
//                       joystick.s                                           /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME joystick

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN EXTD1
        EXTERN _Z10hwPortname11brain_pin_e
        EXTERN _Z10onJoystick17joystick_button_e
        EXTERN _Z13mySetPadMode2PKc11brain_pin_ej
        EXTERN _Z8getHwPin11brain_pin_e
        EXTERN _Z9getHwPort11brain_pin_e
        EXTERN addConsoleAction
        EXTERN boardConfiguration
        EXTERN engineConfiguration
        EXTERN extStart
        EXTERN firmwareError
        EXTERN getTimeNowNt
        EXTERN scheduleMsg

        PUBLIC _Z12initJoystickP7Logging
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\hw_layer\joystick.cpp
//    1 /**
//    2  * @file	joystick.cpp
//    3  *
//    4  * @date Jan 2, 2015
//    5  * @author Andrey Belomutskiy, (c) 2012-2015
//    6  */
//    7 
//    8 #include "engine_configuration.h"
//    9 #include "joystick.h"
//   10 #include "engine.h"
//   11 #include "pin_repository.h"
//   12 
//   13 #if HAL_USE_EXT || defined(__DOXYGEN__)
//   14 
//   15 EXTERN_ENGINE
//   16 ;
//   17 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   18 static int joyTotal = 0;
joyTotal:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   19 static int joyCenter;
joyCenter:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   20 static int joyA = 0;
joyA:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   21 static int joyB = 0;
joyB:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   22 static int joyC = 0;
joyC:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   23 static int joyD = 0;
joyD:
        DS8 4
//   24 
//   25 #define NT_EVENT_GAP US2NT(1000)
//   26 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   27 static Logging *sharedLogger;
sharedLogger:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   28 static efitick_t lastEventTime = 0;
lastEventTime:
        DS8 8
//   29 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z11extCallbackP9EXTDriverj
        THUMB
//   30 static void extCallback(EXTDriver *extp, expchannel_t channel) {
_Z11extCallbackP9EXTDriverj:
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
//   31         UNUSED(extp);
//   32 	efitick_t now = getTimeNowNt();
          CFI FunCall getTimeNowNt
        BL       getTimeNowNt
        MOVS     R4,R0
        MOVS     R5,R1
//   33 	if (now - lastEventTime < NT_EVENT_GAP)
        LDR.N    R0,??DataTable4
        LDRD     R2,R3,[R0, #+0]
        SUBS     R0,R4,R2
        SBCS     R1,R5,R3
        LDR.N    R2,??DataTable4_1  ;; 0x29040
        MOVS     R3,#+0
        CMP      R1,R3
        BGT.N    ??extCallback_0
        BLT.N    ??extCallback_1
        CMP      R0,R2
        BCS.N    ??extCallback_0
//   34 		return; // two consecutive events are probably just jitter
??extCallback_1:
        B.N      ??extCallback_2
//   35 	lastEventTime = now;
??extCallback_0:
        LDR.N    R0,??DataTable4
        STRD     R4,R5,[R0, #+0]
//   36 	joyTotal++;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable4_2
        STR      R0,[R1, #+0]
//   37 	joystick_button_e button;
//   38 	// todo: I guess it's time to reduce code duplication and start working with an array
//   39 	if (channel == getHwPin(boardConfiguration->joystickCenterPin)) {
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+764]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        CMP      R7,R0
        BNE.N    ??extCallback_3
//   40 		joyCenter++;
        LDR.N    R0,??DataTable4_4
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable4_4
        STR      R0,[R1, #+0]
//   41 		button = JB_CENTER;
        MOVS     R0,#+0
        MOV      R8,R0
        B.N      ??extCallback_4
//   42 	} else if (channel == getHwPin(boardConfiguration->joystickAPin)) {
??extCallback_3:
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+768]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        CMP      R7,R0
        BNE.N    ??extCallback_5
//   43 		joyA++;
        LDR.N    R0,??DataTable4_5
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable4_5
        STR      R0,[R1, #+0]
//   44 		button = JB_BUTTON_A;
        MOVS     R0,#+1
        MOV      R8,R0
        B.N      ??extCallback_4
//   45 	} else if (channel == getHwPin(boardConfiguration->joystickBPin)) {
??extCallback_5:
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+772]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        CMP      R7,R0
        BNE.N    ??extCallback_6
//   46 		joyB++;
        LDR.N    R0,??DataTable4_6
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable4_6
        STR      R0,[R1, #+0]
//   47 		button = JB_BUTTON_C;
        MOVS     R0,#+3
        MOV      R8,R0
        B.N      ??extCallback_4
//   48 	} else if (channel == getHwPin(boardConfiguration->joystickCPin)) {
??extCallback_6:
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+776]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        CMP      R7,R0
        BNE.N    ??extCallback_7
//   49 		joyC++;
        LDR.N    R0,??DataTable4_7
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable4_7
        STR      R0,[R1, #+0]
//   50 		button = JB_BUTTON_B;
        MOVS     R0,#+2
        MOV      R8,R0
        B.N      ??extCallback_4
//   51 	} else if (channel == getHwPin(boardConfiguration->joystickDPin)) {
??extCallback_7:
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+780]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        CMP      R7,R0
        BNE.N    ??extCallback_8
//   52 		joyD++;
        LDR.N    R0,??DataTable4_8
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable4_8
        STR      R0,[R1, #+0]
//   53 		button = JB_BUTTON_D;
        MOVS     R0,#+4
        MOV      R8,R0
        B.N      ??extCallback_4
//   54 	} else {
//   55 		// unexpected channel
//   56 		return;
??extCallback_8:
        B.N      ??extCallback_2
//   57 	}
//   58 #if EFI_HD44780_LCD || defined(__DOXYGEN__)
//   59 	onJoystick(button);
??extCallback_4:
        MOV      R0,R8
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z10onJoystick17joystick_button_e
        BL       _Z10onJoystick17joystick_button_e
//   60 #endif
//   61 }
??extCallback_2:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock0
//   62 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z12joystickInfov
        THUMB
//   63 static void joystickInfo(void) {
_Z12joystickInfov:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   64 	scheduleMsg(sharedLogger, "total %d center=%d@%s", joyTotal, joyCenter,
//   65 			hwPortname(boardConfiguration->joystickCenterPin));
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+764]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        STR      R0,[SP, #+0]
        LDR.N    R0,??DataTable4_4
        LDR      R3,[R0, #+0]
        LDR.N    R0,??DataTable4_2
        LDR      R2,[R0, #+0]
        LDR.N    R1,??DataTable4_9
        LDR.N    R0,??DataTable4_10
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   66 	scheduleMsg(sharedLogger, "a=%d@%s", joyA, hwPortname(boardConfiguration->joystickAPin));
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+768]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R3,R0
        LDR.N    R0,??DataTable4_5
        LDR      R2,[R0, #+0]
        LDR.N    R1,??DataTable4_11
        LDR.N    R0,??DataTable4_10
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   67 	scheduleMsg(sharedLogger, "b=%d@%s", joyB, hwPortname(boardConfiguration->joystickBPin));
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+772]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R3,R0
        LDR.N    R0,??DataTable4_6
        LDR      R2,[R0, #+0]
        LDR.N    R1,??DataTable4_12
        LDR.N    R0,??DataTable4_10
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   68 	scheduleMsg(sharedLogger, "c=%d@%s", joyC, hwPortname(boardConfiguration->joystickCPin));
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+776]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R3,R0
        LDR.N    R0,??DataTable4_7
        LDR      R2,[R0, #+0]
        LDR.N    R1,??DataTable4_13
        LDR.N    R0,??DataTable4_10
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   69 	scheduleMsg(sharedLogger, "d=%d@%s", joyD, hwPortname(boardConfiguration->joystickDPin));
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+780]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R3,R0
        LDR.N    R0,??DataTable4_8
        LDR      R2,[R0, #+0]
        LDR.N    R1,??DataTable4_14
        LDR.N    R0,??DataTable4_10
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   70 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock1
//   71 
//   72 /**
//   73  * EXTI is a funny thing: you can only use same pin on one port. For example, you can use
//   74  * PA0 PB5 PE2 PD7
//   75  * but you cannot use
//   76  * PA0 PB0 PE2 PD7
//   77  * because pin '0' would be used on two different ports
//   78  */
//   79 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   80 static EXTConfig extcfg = { {
extcfg:
        DS8 184
//   81 /* CH#00 */{ EXT_CH_MODE_DISABLED, NULL },
//   82 /* CH#01 */{ EXT_CH_MODE_DISABLED, NULL },
//   83 /* CH#02 */{ EXT_CH_MODE_DISABLED, NULL },
//   84 /* CH#03 */{ EXT_CH_MODE_DISABLED, NULL },
//   85 /* CH#04 */{ EXT_CH_MODE_DISABLED, NULL },
//   86 /* CH#05 */{ EXT_CH_MODE_DISABLED, NULL },
//   87 /* CH#06 */{ EXT_CH_MODE_DISABLED, NULL },
//   88 /* CH#07 */{ EXT_CH_MODE_DISABLED, NULL },
//   89 /* CH#08 */{ EXT_CH_MODE_DISABLED, NULL },
//   90 /* CH#09 */{ EXT_CH_MODE_DISABLED, NULL },
//   91 /* CH#10 */{ EXT_CH_MODE_DISABLED, NULL },
//   92 /* CH#11 */{ EXT_CH_MODE_DISABLED, NULL },
//   93 /* CH#12 */{ EXT_CH_MODE_DISABLED, NULL },
//   94 /* CH#13 */{ EXT_CH_MODE_DISABLED, NULL },
//   95 /* CH#14 */{ EXT_CH_MODE_DISABLED, NULL },
//   96 /* CH#15 */{ EXT_CH_MODE_DISABLED, NULL },
//   97 /* CH#16 */{ EXT_CH_MODE_DISABLED, NULL },
//   98 /* CH#17 */{ EXT_CH_MODE_DISABLED, NULL },
//   99 /* CH#18 */{ EXT_CH_MODE_DISABLED, NULL },
//  100 /* CH#19 */{ EXT_CH_MODE_DISABLED, NULL },
//  101 /* CH#20 */{ EXT_CH_MODE_DISABLED, NULL },
//  102 /* CH#21 */{ EXT_CH_MODE_DISABLED, NULL },
//  103 /* CH#22 */{ EXT_CH_MODE_DISABLED, NULL } } };
//  104 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z10getExtModeP12GPIO_TypeDef
        THUMB
//  105 static uint32_t getExtMode(GPIO_TypeDef * port) {
_Z10getExtModeP12GPIO_TypeDef:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  106 	if (port == GPIOA) {
        LDR.N    R0,??DataTable4_15  ;; 0x40020000
        CMP      R4,R0
        BNE.N    ??getExtMode_0
//  107 		return EXT_MODE_GPIOA;
        MOVS     R0,#+0
        B.N      ??getExtMode_1
//  108 	} else if (port == GPIOB) {
??getExtMode_0:
        LDR.N    R0,??DataTable4_16  ;; 0x40020400
        CMP      R4,R0
        BNE.N    ??getExtMode_2
//  109 		return EXT_MODE_GPIOB;
        MOVS     R0,#+16
        B.N      ??getExtMode_1
//  110 	} else if (port == GPIOC) {
??getExtMode_2:
        LDR.N    R0,??DataTable4_17  ;; 0x40020800
        CMP      R4,R0
        BNE.N    ??getExtMode_3
//  111 		return EXT_MODE_GPIOC;
        MOVS     R0,#+32
        B.N      ??getExtMode_1
//  112 	} else if (port == GPIOD) {
??getExtMode_3:
        LDR.N    R0,??DataTable4_18  ;; 0x40020c00
        CMP      R4,R0
        BNE.N    ??getExtMode_4
//  113 		return EXT_MODE_GPIOD;
        MOVS     R0,#+48
        B.N      ??getExtMode_1
//  114 	} else if (port == GPIOE) {
??getExtMode_4:
        LDR.N    R0,??DataTable4_19  ;; 0x40021000
        CMP      R4,R0
        BNE.N    ??getExtMode_5
//  115 		return EXT_MODE_GPIOE;
        MOVS     R0,#+64
        B.N      ??getExtMode_1
//  116 	} else if (port == GPIOF) {
??getExtMode_5:
        LDR.N    R0,??DataTable4_20  ;; 0x40021400
        CMP      R4,R0
        BNE.N    ??getExtMode_6
//  117 		return EXT_MODE_GPIOF;
        MOVS     R0,#+80
        B.N      ??getExtMode_1
//  118 	}
//  119 	firmwareError("Unsupported %d", port);
??getExtMode_6:
        MOVS     R1,R4
        LDR.N    R0,??DataTable4_21
          CFI FunCall firmwareError
        BL       firmwareError
//  120 	return 0;
        MOVS     R0,#+0
??getExtMode_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2
//  121 }
//  122 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z8applyPin11brain_pin_e
        THUMB
//  123 static void applyPin(brain_pin_e pin) {
_Z8applyPin11brain_pin_e:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  124 	if (pin == GPIO_UNASSIGNED)
        CMP      R4,#+80
        BEQ.N    ??applyPin_0
//  125 		return;
//  126 
//  127 	int index = getHwPin(pin);
??applyPin_1:
        MOVS     R0,R4
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        MOVS     R5,R0
//  128 	GPIO_TypeDef * port = getHwPort(pin);
        MOVS     R0,R4
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOVS     R6,R0
//  129 
//  130 	extcfg.channels[index].mode = EXT_CH_MODE_RISING_EDGE | EXT_CH_MODE_AUTOSTART | getExtMode(port);
        MOVS     R0,R6
          CFI FunCall _Z10getExtModeP12GPIO_TypeDef
        BL       _Z10getExtModeP12GPIO_TypeDef
        ORRS     R0,R0,#0x5
        LDR.N    R1,??DataTable4_22
        STR      R0,[R1, R5, LSL #+3]
//  131 	extcfg.channels[index].cb = extCallback;
        LDR.N    R0,??DataTable4_22
        ADDS     R0,R0,R5, LSL #+3
        LDR.N    R1,??DataTable4_23
        STR      R1,[R0, #+4]
//  132 }
??applyPin_0:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock3
//  133 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z12initJoystickP7Logging
        THUMB
//  134 void initJoystick(Logging *shared) {
_Z12initJoystickP7Logging:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  135 	if (!engineConfiguration->isJoystickEnabled)
        LDR.N    R0,??DataTable4_24
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+3
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??initJoystick_0
//  136 		return;
//  137 	sharedLogger = shared;
??initJoystick_1:
        LDR.N    R0,??DataTable4_10
        STR      R4,[R0, #+0]
//  138 
//  139 	applyPin(boardConfiguration->joystickCenterPin);
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+764]
          CFI FunCall _Z8applyPin11brain_pin_e
        BL       _Z8applyPin11brain_pin_e
//  140 	applyPin(boardConfiguration->joystickAPin);
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+768]
          CFI FunCall _Z8applyPin11brain_pin_e
        BL       _Z8applyPin11brain_pin_e
//  141 	applyPin(boardConfiguration->joystickBPin);
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+772]
          CFI FunCall _Z8applyPin11brain_pin_e
        BL       _Z8applyPin11brain_pin_e
//  142 	applyPin(boardConfiguration->joystickCPin);
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+776]
          CFI FunCall _Z8applyPin11brain_pin_e
        BL       _Z8applyPin11brain_pin_e
//  143 	applyPin(boardConfiguration->joystickDPin);
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+780]
          CFI FunCall _Z8applyPin11brain_pin_e
        BL       _Z8applyPin11brain_pin_e
//  144 
//  145 	mySetPadMode2("joy center", boardConfiguration->joystickCenterPin, PAL_MODE_INPUT_PULLUP);
        MOVS     R2,#+32
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+764]
        LDR.N    R0,??DataTable4_25
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
//  146 	mySetPadMode2("joy A", boardConfiguration->joystickAPin, PAL_MODE_INPUT_PULLUP);
        MOVS     R2,#+32
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+768]
        LDR.N    R0,??DataTable4_26
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
//  147 	mySetPadMode2("joy B", boardConfiguration->joystickBPin, PAL_MODE_INPUT_PULLUP);
        MOVS     R2,#+32
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+772]
        LDR.N    R0,??DataTable4_27
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
//  148 	mySetPadMode2("joy C", boardConfiguration->joystickCPin, PAL_MODE_INPUT_PULLUP);
        MOVS     R2,#+32
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+776]
        LDR.N    R0,??DataTable4_28
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
//  149 	mySetPadMode2("joy D", boardConfiguration->joystickDPin, PAL_MODE_INPUT_PULLUP);
        MOVS     R2,#+32
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+780]
        LDR.N    R0,??DataTable4_29
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
//  150 
//  151 	addConsoleAction("joystickinfo", joystickInfo);
        LDR.N    R1,??DataTable4_30
        LDR.N    R0,??DataTable4_31
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  152 
//  153 	extStart(&EXTD1, &extcfg);
        LDR.N    R1,??DataTable4_22
        LDR.N    R0,??DataTable4_32
          CFI FunCall extStart
        BL       extStart
//  154 }
??initJoystick_0:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4:
        DC32     lastEventTime

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_1:
        DC32     0x29040

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_2:
        DC32     joyTotal

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_3:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_4:
        DC32     joyCenter

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_5:
        DC32     joyA

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_6:
        DC32     joyB

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_7:
        DC32     joyC

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_8:
        DC32     joyD

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_9:
        DC32     `?<Constant "total %d center=%d@%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_10:
        DC32     sharedLogger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_11:
        DC32     `?<Constant "a=%d@%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_12:
        DC32     `?<Constant "b=%d@%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_13:
        DC32     `?<Constant "c=%d@%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_14:
        DC32     `?<Constant "d=%d@%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_15:
        DC32     0x40020000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_16:
        DC32     0x40020400

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_17:
        DC32     0x40020800

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_18:
        DC32     0x40020c00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_19:
        DC32     0x40021000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_20:
        DC32     0x40021400

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_21:
        DC32     `?<Constant "Unsupported %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_22:
        DC32     extcfg

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_23:
        DC32     _Z11extCallbackP9EXTDriverj

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_24:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_25:
        DC32     `?<Constant "joy center">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_26:
        DC32     `?<Constant "joy A">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_27:
        DC32     `?<Constant "joy B">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_28:
        DC32     `?<Constant "joy C">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_29:
        DC32     `?<Constant "joy D">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_30:
        DC32     _Z12joystickInfov

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_31:
        DC32     `?<Constant "joystickinfo">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_32:
        DC32     EXTD1

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "total %d center=%d@%s">`:
        DATA
        DC8 "total %d center=%d@%s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "a=%d@%s">`:
        DATA
        DC8 "a=%d@%s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "b=%d@%s">`:
        DATA
        DC8 "b=%d@%s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "c=%d@%s">`:
        DATA
        DC8 "c=%d@%s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "d=%d@%s">`:
        DATA
        DC8 "d=%d@%s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Unsupported %d">`:
        DATA
        DC8 "Unsupported %d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "joy center">`:
        DATA
        DC8 "joy center"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "joy A">`:
        DATA
        DC8 "joy A"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "joy B">`:
        DATA
        DC8 "joy B"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "joy C">`:
        DATA
        DC8 "joy C"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "joy D">`:
        DATA
        DC8 "joy D"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "joystickinfo">`:
        DATA
        DC8 "joystickinfo"
        DC8 0, 0, 0

        END
//  155 
//  156 #endif
// 
// 220 bytes in section .bss
// 132 bytes in section .rodata
// 828 bytes in section .text
// 
// 828 bytes of CODE  memory
// 132 bytes of CONST memory
// 220 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
