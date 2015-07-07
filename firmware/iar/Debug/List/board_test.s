///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:08 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\board_ /
//                       test.cpp                                             /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\board_ /
//                       test.cpp -lCN F:\stuff\rusefi_sourceforge\firmware\i /
//                       ar\Debug\List\ -lA F:\stuff\rusefi_sourceforge\firmw /
//                       are\iar\Debug\List\ -o F:\stuff\rusefi_sourceforge\f /
//                       irmware\iar\Debug\Obj\ --no_cse --no_unroll          /
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
//                       board_test.s                                         /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME board_test

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z10hwPortname11brain_pin_e
        EXTERN _Z13mySetPadMode2PKc11brain_pin_ej
        EXTERN _Z16getAdcChannelPin13adc_channel_e
        EXTERN _Z17getAdcChannelPort13adc_channel_e
        EXTERN _Z8getHwPin11brain_pin_e
        EXTERN _Z8portnameP12GPIO_TypeDef
        EXTERN _Z9getHwPort11brain_pin_e
        EXTERN _ZN9AdcDevice18getAdcValueByIndexEi
        EXTERN _ZN9AdcDevice34getAdcHardwareIndexByInternalIndexEi
        EXTERN _ZN9AdcDevice4sizeEv
        EXTERN __aeabi_f2d
        EXTERN addConsoleAction
        EXTERN addConsoleActionI
        EXTERN chThdCreateStatic
        EXTERN chThdSleep
        EXTERN fastAdc
        EXTERN print
        EXTERN rlist
        EXTERN slowAdc

        PUBLIC _Z13initBoardTestv
        PUBLIC isBoardTestMode
        PUBLIC pinsCount
        PUBLIC printBoardTestState
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\hw_layer\board_test.cpp
//    1 /**
//    2  * @file	board_test.cpp
//    3  * @brief	This is a simple board testing utility
//    4  *
//    5  * By default this is enabled by grounding PB0
//    6  *
//    7  * @date Mar 12, 2014
//    8  * @author Andrey Belomutskiy, (c) 2012-2015
//    9  *
//   10  * This file is part of rusEfi - see http://rusefi.com
//   11  *
//   12  * rusEfi is free software; you can redistribute it and/or modify it under the terms of
//   13  * the GNU General Public License as published by the Free Software Foundation; either
//   14  * version 3 of the License, or (at your option) any later version.
//   15  *
//   16  * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
//   17  * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   18  * GNU General Public License for more details.
//   19  *
//   20  * You should have received a copy of the GNU General Public License along with this program.
//   21  * If not, see <http://www.gnu.org/licenses/>.
//   22  */
//   23 
//   24 #include "main.h"
//   25 #include "board_test.h"
//   26 #include "rusefi_enums.h"
//   27 #include "pin_repository.h"
//   28 #include "gpio_helper.h"
//   29 #include "adc_inputs.h"
//   30 #include "AdcConfiguration.h"
//   31 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   32 static volatile int stepCoutner = 0;
stepCoutner:
        DS8 4

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   33 static volatile brain_pin_e currentPin = GPIO_UNASSIGNED;
currentPin:
        DATA
        DC32 80
//   34 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z17isTimeForNextStepi
          CFI NoCalls
        THUMB
//   35 static bool isTimeForNextStep(int copy) {
//   36 	return copy != stepCoutner;
_Z17isTimeForNextStepi:
        LDR.N    R1,??DataTable11_2
        LDR      R1,[R1, #+0]
        CMP      R0,R1
        BEQ.N    ??isTimeForNextStep_0
        MOVS     R0,#+1
        B.N      ??isTimeForNextStep_1
??isTimeForNextStep_0:
        MOVS     R0,#+0
??isTimeForNextStep_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//   37 }
//   38 
//   39 #if HAL_USE_ADC || defined(__DOXYGEN__)
//   40 extern AdcDevice slowAdc;
//   41 extern AdcDevice fastAdc;
//   42 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z13processAdcPinP9AdcDevicei
        THUMB
//   43 static void processAdcPin(AdcDevice *adc, int index) {
_Z13processAdcPinP9AdcDevicei:
        PUSH     {R4-R10,LR}
          CFI R14 Frame(CFA, -4)
          CFI R10 Frame(CFA, -8)
          CFI R9 Frame(CFA, -12)
          CFI R8 Frame(CFA, -16)
          CFI R7 Frame(CFA, -20)
          CFI R6 Frame(CFA, -24)
          CFI R5 Frame(CFA, -28)
          CFI R4 Frame(CFA, -32)
          CFI CFA R13+32
        VPUSH    {D8}
          CFI D8 Frame(CFA, -40)
          CFI CFA R13+40
        SUB      SP,SP,#+8
          CFI CFA R13+48
        MOV      R9,R0
        MOV      R8,R1
//   44 	adc_channel_e hwIndex = adc->getAdcHardwareIndexByInternalIndex(index);
        MOV      R1,R8
        MOV      R0,R9
          CFI FunCall _ZN9AdcDevice34getAdcHardwareIndexByInternalIndexEi
        BL       _ZN9AdcDevice34getAdcHardwareIndexByInternalIndexEi
        MOVS     R4,R0
//   45 	GPIO_TypeDef* port = getAdcChannelPort(hwIndex);
        MOVS     R0,R4
          CFI FunCall _Z17getAdcChannelPort13adc_channel_e
        BL       _Z17getAdcChannelPort13adc_channel_e
        MOVS     R5,R0
//   46 	int pin = getAdcChannelPin(hwIndex);
        MOVS     R0,R4
          CFI FunCall _Z16getAdcChannelPin13adc_channel_e
        BL       _Z16getAdcChannelPin13adc_channel_e
        MOVS     R6,R0
//   47 
//   48 	int copy = stepCoutner;
        LDR.N    R0,??DataTable11_2
        LDR      R10,[R0, #+0]
//   49 
//   50 	int c = 0;
        MOVS     R7,#+0
//   51 
//   52 	while (!isTimeForNextStep(copy)) {
??processAdcPin_0:
        MOV      R0,R10
          CFI FunCall _Z17isTimeForNextStepi
        BL       _Z17isTimeForNextStepi
        CMP      R0,#+0
        BNE.N    ??processAdcPin_1
//   53 		print("ch%d hwIndex=%d %s%d\r\n", index, hwIndex, portname(port), pin);
        MOVS     R0,R5
          CFI FunCall _Z8portnameP12GPIO_TypeDef
        BL       _Z8portnameP12GPIO_TypeDef
        STR      R6,[SP, #+0]
        MOVS     R3,R0
        MOVS     R2,R4
        MOV      R1,R8
        LDR.N    R0,??DataTable11_3
          CFI FunCall print
        BL       print
//   54 		int adcValue = adc->getAdcValueByIndex(index);
        MOV      R1,R8
        MOV      R0,R9
          CFI FunCall _ZN9AdcDevice18getAdcValueByIndexEi
        BL       _ZN9AdcDevice18getAdcValueByIndexEi
        VMOV     S16,R0
//   55 
//   56 //		print("ADC%d val= %d%s", hwIndex, value, DELIMETER);
//   57 		float volts = adcToVolts(adcValue) * 2;
        VCVT.F32.S32 S0,S16
        VLDR.W   S1,??DataTable10  ;; 0x3a400c01
        VMUL.F32 S0,S0,S1
        VMOV.F32 S1,#2.0
        VMUL.F32 S17,S0,S1
//   58 		print("v=%f  adc=%d  c=%d (hit 'n'<ENTER> for next step\r\n", volts, adcValue, c++);
        STR      R7,[SP, #+4]
        VSTR     S16,[SP, #+0]
        VMOV     R0,S17
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R0,??DataTable11_4
          CFI FunCall print
        BL       print
        ADDS     R7,R7,#+1
//   59 
//   60 		chThdSleepMilliseconds(300);
        MOV      R0,#+300
          CFI FunCall chThdSleep
        BL       chThdSleep
        B.N      ??processAdcPin_0
//   61 
//   62 	}
//   63 }
??processAdcPin_1:
        ADD      SP,SP,#+8
          CFI CFA R13+40
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+32
        POP      {R4-R10,PC}      ;; return
          CFI EndBlock cfiBlock1
//   64 #endif

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   65 static volatile int currentIndex = 0;
currentIndex:
        DS8 4
//   66 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z10waitForKeyv
        THUMB
//   67 static void waitForKey(void) {
_Z10waitForKeyv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//   68 	print("Please hit N<ENTER> to continue\r\n");
        LDR.N    R0,??DataTable11_5
          CFI FunCall print
        BL       print
//   69 	int copy = stepCoutner;
        LDR.N    R0,??DataTable11_2
        LDR      R4,[R0, #+0]
//   70 	while (!isTimeForNextStep(copy)) {
??waitForKey_0:
        MOVS     R0,R4
          CFI FunCall _Z17isTimeForNextStepi
        BL       _Z17isTimeForNextStepi
        CMP      R0,#+0
        BNE.N    ??waitForKey_1
//   71 		chThdSleepMilliseconds(200);
        MOVS     R0,#+200
          CFI FunCall chThdSleep
        BL       chThdSleep
        B.N      ??waitForKey_0
//   72 	}
//   73 }
??waitForKey_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2
//   74 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z8nextStepv
          CFI NoCalls
        THUMB
//   75 static void nextStep(void) {
//   76 	stepCoutner++;
_Z8nextStepv:
        LDR.N    R0,??DataTable11_2
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable11_2
        STR      R0,[R1, #+0]
//   77 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock3
//   78 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z8setIndexi
        THUMB
//   79 static void setIndex(int index) {
_Z8setIndexi:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   80 	currentIndex = index;
        LDR.N    R0,??DataTable11_6
        STR      R4,[R0, #+0]
//   81 	nextStep();
          CFI FunCall _Z8nextStepv
        BL       _Z8nextStepv
//   82 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock4
//   83 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   84 static brain_pin_e BLINK_PINS[] = { GPIOE_8, // HIGH DRIVER 1
BLINK_PINS:
        DATA
        DC32 72, 74, 76, 78, 41, 39, 46, 47, 70, 45, 68, 69, 66, 67, 64, 65, 24
        DC32 25, 51, 53, 55, 23
//   85 		GPIOE_10, // HIGH DRIVER 2
//   86 		GPIOE_12, // HIGH DRIVER 3
//   87 		GPIOE_14, // HIGH DRIVER 4
//   88 		GPIOC_9, // HIGH DRIVER 5
//   89 		GPIOC_7, // HIGH DRIVER 6
//   90 		// index = 6
//   91 		GPIOC_14, // OUT 1
//   92 		GPIOC_15, // OUT2
//   93 		GPIOE_6,  // OUT3
//   94 		GPIOC_13, // OUT4
//   95 		GPIOE_4, // OUT5
//   96 		GPIOE_5, // OUT6
//   97 		GPIOE_2, // OUT7
//   98 		GPIOE_3, // OUT8
//   99 		GPIOE_0, // OUT9
//  100 		GPIOE_1, // OUT10
//  101 		GPIOB_8, // OUT11
//  102 		GPIOB_9, // OUT12
//  103 		GPIOD_3, // Frankenso PE1 reroute
//  104 		GPIOD_5, // Frankenso PE0 reroute
//  105 		GPIOD_7, // OUT3 Frankenso
//  106 		GPIOB_7, // OUT12 Frankenso
//  107 		};
//  108 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  109 int pinsCount = sizeof(BLINK_PINS) / sizeof(brain_pin_e);
pinsCount:
        DATA
        DC32 22
//  110 

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//  111 static THD_WORKING_AREA(btThreadStack, UTILITY_THREAD_STACK_SIZE);
btThreadStack:
        DS8 712
//  112 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z18setCurrentPinValueb
        THUMB
//  113 static void setCurrentPinValue(bool value) {
_Z18setCurrentPinValueb:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  114 	GPIO_TypeDef *hwPort = getHwPort(currentPin);
        LDR.N    R0,??DataTable11_7
        LDR      R0,[R0, #+0]
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOVS     R5,R0
//  115 	uint32_t hwPin = getHwPin(currentPin);
        LDR.N    R0,??DataTable11_7
        LDR      R0,[R0, #+0]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
//  116 	palWritePad(hwPort, hwPin, value);
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        MVNS     R1,R4
        ANDS     R1,R1,#0x1
        ADDS     R2,R0,#+16
        LSLS     R1,R1,R2
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        LSLS     R2,R4,R0
        ORRS     R1,R2,R1
        STR      R1,[R5, #+24]
//  117 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock5
//  118 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z8ivThreadi
        THUMB
//  119 static msg_t ivThread(int param) {
_Z8ivThreadi:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  120 	(void) param;
//  121 	chRegSetThreadName("board test blinking");
        LDR.N    R0,??DataTable11_8
        LDR      R0,[R0, #+24]
        LDR.N    R1,??DataTable11_9
        STR      R1,[R0, #+24]
//  122 
//  123 	int value = 0;
        MOVS     R5,#+0
//  124 
//  125 	while (TRUE) {
//  126 		chThdSleepMilliseconds(250);
??ivThread_0:
        MOVS     R0,#+250
          CFI FunCall chThdSleep
        BL       chThdSleep
//  127 		value = 1 - value;
        RSBS     R5,R5,#+1
//  128 		setCurrentPinValue(value);
        CMP      R5,#+0
        BEQ.N    ??ivThread_1
        MOVS     R0,#+1
        B.N      ??ivThread_2
??ivThread_1:
        MOVS     R0,#+0
??ivThread_2:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z18setCurrentPinValueb
        BL       _Z18setCurrentPinValueb
        B.N      ??ivThread_0
          CFI EndBlock cfiBlock6
//  129 	}
//  130 #if defined __GNUC__
//  131 	return 0;
//  132 #endif        
//  133 }
//  134 

        SECTION `.bss`:DATA:REORDER:NOROOT(0)
//  135 static bool is_board_test_mode = false;
is_board_test_mode:
        DS8 1
//  136 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function isBoardTestMode
          CFI NoCalls
        THUMB
//  137 bool isBoardTestMode(void) {
//  138 	return is_board_test_mode;
isBoardTestMode:
        LDR.N    R0,??DataTable11_10
        LDRB     R0,[R0, #+0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock7
//  139 }
//  140 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function printBoardTestState
        THUMB
//  141 void printBoardTestState(void) {
printBoardTestState:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  142 	print("Current index=%d\r\n", currentIndex);
        LDR.N    R0,??DataTable11_6
        LDR      R1,[R0, #+0]
        LDR.N    R0,??DataTable11_11
          CFI FunCall print
        BL       print
//  143 	print("'n' for next step and 'set X' to return to step X\r\n");
        LDR.N    R0,??DataTable11_12
          CFI FunCall print
        BL       print
//  144 #if HAL_USE_ADC || defined(__DOXYGEN__)
//  145 	print("ADC count: slow %d/fast %d\r\n", slowAdc.size(), fastAdc.size());
        LDR.N    R0,??DataTable11_13
          CFI FunCall _ZN9AdcDevice4sizeEv
        BL       _ZN9AdcDevice4sizeEv
        MOVS     R4,R0
        LDR.N    R0,??DataTable11_14
          CFI FunCall _ZN9AdcDevice4sizeEv
        BL       _ZN9AdcDevice4sizeEv
        MOVS     R1,R0
        MOVS     R2,R4
        LDR.N    R0,??DataTable11_15
          CFI FunCall print
        BL       print
//  146 #endif
//  147 
//  148 	if (currentPin != GPIO_UNASSIGNED) {
        LDR.N    R0,??DataTable11_7
        LDR      R0,[R0, #+0]
        CMP      R0,#+80
        BEQ.N    ??printBoardTestState_0
//  149 		print("Blinking %s\r\n", hwPortname(currentPin));
        LDR.N    R0,??DataTable11_7
        LDR      R0,[R0, #+0]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R1,R0
        LDR.N    R0,??DataTable11_16
          CFI FunCall print
        BL       print
//  150 	}
//  151 }
??printBoardTestState_0:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock8
//  152 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z16btInitOutputPinsv
        THUMB
//  153 static void btInitOutputPins() {
_Z16btInitOutputPinsv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  154 	for (int i = 0; i < pinsCount; i++) {
        MOVS     R4,#+0
??btInitOutputPins_0:
        LDR.N    R0,??DataTable11_17
        LDR      R0,[R0, #+0]
        CMP      R4,R0
        BGE.N    ??btInitOutputPins_1
//  155 		currentPin = BLINK_PINS[i];
        LDR.N    R0,??DataTable11_18
        LDR      R0,[R0, R4, LSL #+2]
        LDR.N    R1,??DataTable11_7
        STR      R0,[R1, #+0]
//  156 		mySetPadMode2("test", currentPin, PAL_STM32_MODE_OUTPUT);
        MOVS     R2,#+1
        LDR.N    R0,??DataTable11_7
        LDR      R1,[R0, #+0]
        LDR.N    R0,??DataTable11_19
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
//  157 	}
        ADDS     R4,R4,#+1
        B.N      ??btInitOutputPins_0
//  158 }
??btInitOutputPins_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock9
//  159 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z18blinkAllOutputPinsv
        THUMB
//  160 static void blinkAllOutputPins() {
_Z18blinkAllOutputPinsv:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
//  161 	int msgCounter = 0;
        MOVS     R4,#+0
//  162 	for (int k = 0; k < 6; k++) {
        MOVS     R5,#+0
??blinkAllOutputPins_0:
        CMP      R5,#+6
        BGE.N    ??blinkAllOutputPins_1
//  163 		for (int i = 0; i < pinsCount; i++) {
        MOVS     R6,#+0
??blinkAllOutputPins_2:
        LDR.N    R0,??DataTable11_17
        LDR      R0,[R0, #+0]
        CMP      R6,R0
        BGE.N    ??blinkAllOutputPins_3
//  164 			currentPin = BLINK_PINS[i];
        LDR.N    R0,??DataTable11_18
        LDR      R0,[R0, R6, LSL #+2]
        LDR.N    R1,??DataTable11_7
        STR      R0,[R1, #+0]
//  165 			setCurrentPinValue(k % 2);
        MOVS     R0,#+2
        SDIV     R1,R5,R0
        MLS      R1,R1,R0,R5
        CMP      R1,#+0
        BEQ.N    ??blinkAllOutputPins_4
        MOVS     R0,#+1
        B.N      ??blinkAllOutputPins_5
??blinkAllOutputPins_4:
        MOVS     R0,#+0
??blinkAllOutputPins_5:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z18setCurrentPinValueb
        BL       _Z18setCurrentPinValueb
//  166 		}
        ADDS     R6,R6,#+1
        B.N      ??blinkAllOutputPins_2
//  167 		print("blinking %d\r\n", msgCounter++);
??blinkAllOutputPins_3:
        MOVS     R1,R4
        LDR.N    R0,??DataTable11_20
          CFI FunCall print
        BL       print
        ADDS     R4,R4,#+1
//  168 		chThdSleepMilliseconds(250);
        MOVS     R0,#+250
          CFI FunCall chThdSleep
        BL       chThdSleep
//  169 	}
        ADDS     R5,R5,#+1
        B.N      ??blinkAllOutputPins_0
//  170 	currentPin = GPIO_UNASSIGNED;
??blinkAllOutputPins_1:
        LDR.N    R0,??DataTable11_7
        MOVS     R1,#+80
        STR      R1,[R0, #+0]
//  171 	/**
//  172 	 * Now let's blink all pins one by one
//  173 	 */
//  174 	for (int k = 0; k < 2; k++) {
        MOVS     R5,#+0
??blinkAllOutputPins_6:
        CMP      R5,#+2
        BGE.N    ??blinkAllOutputPins_7
//  175 		for (int i = 0; i < pinsCount; i++) {
        MOVS     R6,#+0
??blinkAllOutputPins_8:
        LDR.N    R0,??DataTable11_17
        LDR      R0,[R0, #+0]
        CMP      R6,R0
        BGE.N    ??blinkAllOutputPins_9
//  176 			if (currentPin != GPIO_UNASSIGNED)
        LDR.N    R0,??DataTable11_7
        LDR      R0,[R0, #+0]
        CMP      R0,#+80
        BEQ.N    ??blinkAllOutputPins_10
//  177 				setCurrentPinValue(false); // turn off previous pin
        MOVS     R0,#+0
          CFI FunCall _Z18setCurrentPinValueb
        BL       _Z18setCurrentPinValueb
//  178 
//  179 			currentPin = BLINK_PINS[i];
??blinkAllOutputPins_10:
        LDR.N    R0,??DataTable11_18
        LDR      R0,[R0, R6, LSL #+2]
        LDR.N    R1,??DataTable11_7
        STR      R0,[R1, #+0]
//  180 			setCurrentPinValue(true);
        MOVS     R0,#+1
          CFI FunCall _Z18setCurrentPinValueb
        BL       _Z18setCurrentPinValueb
//  181 			print("blinking %d!\r\n", msgCounter++);
        MOVS     R1,R4
        LDR.N    R0,??DataTable11_21
          CFI FunCall print
        BL       print
        ADDS     R4,R4,#+1
//  182 			chThdSleepMilliseconds(250);
        MOVS     R0,#+250
          CFI FunCall chThdSleep
        BL       chThdSleep
//  183 		}
        ADDS     R6,R6,#+1
        B.N      ??blinkAllOutputPins_8
//  184 	}
??blinkAllOutputPins_9:
        ADDS     R5,R5,#+1
        B.N      ??blinkAllOutputPins_6
//  185 	setCurrentPinValue(false);
??blinkAllOutputPins_7:
        MOVS     R0,#+0
          CFI FunCall _Z18setCurrentPinValueb
        BL       _Z18setCurrentPinValueb
//  186 	currentPin = GPIO_UNASSIGNED;
        LDR.N    R0,??DataTable11_7
        MOVS     R1,#+80
        STR      R1,[R0, #+0]
//  187 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock10

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10:
        DC32     0x3a400c01
//  188 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z13initBoardTestv
        THUMB
//  189 void initBoardTest(void) {
_Z13initBoardTestv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        SUB      SP,SP,#+8
          CFI CFA R13+16
//  190 	is_board_test_mode = true;
        LDR.N    R0,??DataTable11_10
        MOVS     R1,#+1
        STRB     R1,[R0, #+0]
//  191 	addConsoleAction("n", nextStep);
        LDR.N    R1,??DataTable11_22
        ADR.N    R0,??DataTable11  ;; "n"
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  192 	addConsoleActionI("set", setIndex);
        LDR.N    R1,??DataTable11_23
        ADR.N    R0,??DataTable11_1  ;; "set"
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  193 
//  194 	btInitOutputPins();
          CFI FunCall _Z16btInitOutputPinsv
        BL       _Z16btInitOutputPinsv
//  195 	blinkAllOutputPins();
          CFI FunCall _Z18blinkAllOutputPinsv
        BL       _Z18blinkAllOutputPinsv
//  196 
//  197 	chThdCreateStatic(btThreadStack, sizeof(btThreadStack), NORMALPRIO, (tfunc_t) ivThread, NULL);
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        LDR.N    R3,??DataTable11_24
        MOVS     R2,#+64
        MOV      R1,#+712
        LDR.N    R0,??DataTable11_25
          CFI FunCall chThdCreateStatic
        BL       chThdCreateStatic
//  198 	// this code is ugly as hell, I had no time to think. Todo: refactor
//  199 
//  200 #if HAL_USE_ADC || defined(__DOXYGEN__)
//  201 	/**
//  202 	 * in board test mode all currently enabled ADC channels are running in slow mode
//  203 	 */
//  204 	while (currentIndex < slowAdc.size()) {
??initBoardTest_0:
        LDR.N    R0,??DataTable11_6
        LDR      R4,[R0, #+0]
        LDR.N    R0,??DataTable11_14
          CFI FunCall _ZN9AdcDevice4sizeEv
        BL       _ZN9AdcDevice4sizeEv
        CMP      R4,R0
        BGE.N    ??initBoardTest_1
//  205 		processAdcPin(&slowAdc, currentIndex);
        LDR.N    R0,??DataTable11_6
        LDR      R1,[R0, #+0]
        LDR.N    R0,??DataTable11_14
          CFI FunCall _Z13processAdcPinP9AdcDevicei
        BL       _Z13processAdcPinP9AdcDevicei
//  206 		currentIndex++;
        LDR.N    R0,??DataTable11_6
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable11_6
        STR      R0,[R1, #+0]
        B.N      ??initBoardTest_0
//  207 	}
//  208 #endif
//  209 
//  210 	currentIndex = 0;
??initBoardTest_1:
        LDR.N    R0,??DataTable11_6
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  211 
//  212 	while (currentIndex < pinsCount) {
??initBoardTest_2:
        LDR.N    R0,??DataTable11_6
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable11_17
        LDR      R1,[R1, #+0]
        CMP      R0,R1
        BGE.N    ??initBoardTest_3
//  213 		currentPin = BLINK_PINS[currentIndex];
        LDR.N    R0,??DataTable11_6
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable11_18
        LDR      R0,[R1, R0, LSL #+2]
        LDR.N    R1,??DataTable11_7
        STR      R0,[R1, #+0]
//  214 
//  215 		printBoardTestState();
          CFI FunCall printBoardTestState
        BL       printBoardTestState
//  216 
//  217 		currentIndex++;
        LDR.N    R0,??DataTable11_6
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable11_6
        STR      R0,[R1, #+0]
//  218 		waitForKey();
          CFI FunCall _Z10waitForKeyv
        BL       _Z10waitForKeyv
        B.N      ??initBoardTest_2
//  219 	}
//  220 	// no buffered logger still, just plain old stdout
//  221 	while (1) {
//  222 		print("Board test done, thank you! Time to remove that jumper and reboot\r\n");
??initBoardTest_3:
        LDR.N    R0,??DataTable11_26
          CFI FunCall print
        BL       print
//  223 		print("Bye!\r\n");
        LDR.N    R0,??DataTable11_27
          CFI FunCall print
        BL       print
//  224 		chThdSleepSeconds(1);
        MOV      R0,#+1000
          CFI FunCall chThdSleep
        BL       chThdSleep
        B.N      ??initBoardTest_3
          CFI EndBlock cfiBlock11
//  225 	}
//  226 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11:
        DC8      "n",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_1:
        DC8      "set"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_2:
        DC32     stepCoutner

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_3:
        DC32     `?<Constant "ch%d hwIndex=%d %s%d\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_4:
        DC32     `?<Constant "v=%f  adc=%d  c=%d (h...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_5:
        DC32     `?<Constant "Please hit N<ENTER> t...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_6:
        DC32     currentIndex

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_7:
        DC32     currentPin

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_8:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_9:
        DC32     `?<Constant "board test blinking">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_10:
        DC32     is_board_test_mode

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_11:
        DC32     `?<Constant "Current index=%d\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_12:
        DC32     `?<Constant "\\'n\\' for next step and...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_13:
        DC32     fastAdc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_14:
        DC32     slowAdc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_15:
        DC32     `?<Constant "ADC count: slow %d/fa...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_16:
        DC32     `?<Constant "Blinking %s\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_17:
        DC32     pinsCount

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_18:
        DC32     BLINK_PINS

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_19:
        DC32     `?<Constant "test">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_20:
        DC32     `?<Constant "blinking %d\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_21:
        DC32     `?<Constant "blinking %d!\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_22:
        DC32     _Z8nextStepv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_23:
        DC32     _Z8setIndexi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_24:
        DC32     _Z8ivThreadi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_25:
        DC32     btThreadStack

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_26:
        DC32     `?<Constant "Board test done, than...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_27:
        DC32     `?<Constant "Bye!\\r\\n">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ch%d hwIndex=%d %s%d\\r\\n">`:
        DATA
        DC8 "ch%d hwIndex=%d %s%d\015\012"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "v=%f  adc=%d  c=%d (h...">`:
        DATA
        DC8 76H, 3DH, 25H, 66H, 20H, 20H, 61H, 64H
        DC8 63H, 3DH, 25H, 64H, 20H, 20H, 63H, 3DH
        DC8 25H, 64H, 20H, 28H, 68H, 69H, 74H, 20H
        DC8 27H, 6EH, 27H, 3CH, 45H, 4EH, 54H, 45H
        DC8 52H, 3EH, 20H, 66H, 6FH, 72H, 20H, 6EH
        DC8 65H, 78H, 74H, 20H, 73H, 74H, 65H, 70H
        DC8 0DH, 0AH, 0
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Please hit N<ENTER> t...">`:
        DATA
        DC8 "Please hit N<ENTER> to continue\015\012"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "board test blinking">`:
        DATA
        DC8 "board test blinking"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Current index=%d\\r\\n">`:
        DATA
        DC8 "Current index=%d\015\012"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "\\'n\\' for next step and...">`:
        DATA
        DC8 27H, 6EH, 27H, 20H, 66H, 6FH, 72H, 20H
        DC8 6EH, 65H, 78H, 74H, 20H, 73H, 74H, 65H
        DC8 70H, 20H, 61H, 6EH, 64H, 20H, 27H, 73H
        DC8 65H, 74H, 20H, 58H, 27H, 20H, 74H, 6FH
        DC8 20H, 72H, 65H, 74H, 75H, 72H, 6EH, 20H
        DC8 74H, 6FH, 20H, 73H, 74H, 65H, 70H, 20H
        DC8 58H, 0DH, 0AH, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ADC count: slow %d/fa...">`:
        DATA
        DC8 "ADC count: slow %d/fast %d\015\012"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Blinking %s\\r\\n">`:
        DATA
        DC8 "Blinking %s\015\012"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "test">`:
        DATA
        DC8 "test"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "blinking %d\\r\\n">`:
        DATA
        DC8 "blinking %d\015\012"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "blinking %d!\\r\\n">`:
        DATA
        DC8 "blinking %d!\015\012"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 "n"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "set"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Board test done, than...">`:
        DATA
        DC8 42H, 6FH, 61H, 72H, 64H, 20H, 74H, 65H
        DC8 73H, 74H, 20H, 64H, 6FH, 6EH, 65H, 2CH
        DC8 20H, 74H, 68H, 61H, 6EH, 6BH, 20H, 79H
        DC8 6FH, 75H, 21H, 20H, 54H, 69H, 6DH, 65H
        DC8 20H, 74H, 6FH, 20H, 72H, 65H, 6DH, 6FH
        DC8 76H, 65H, 20H, 74H, 68H, 61H, 74H, 20H
        DC8 6AH, 75H, 6DH, 70H, 65H, 72H, 20H, 61H
        DC8 6EH, 64H, 20H, 72H, 65H, 62H, 6FH, 6FH
        DC8 74H, 0DH, 0AH, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Bye!\\r\\n">`:
        DATA
        DC8 "Bye!\015\012"
        DC8 0

        END
// 
// 721 bytes in section .bss
//  96 bytes in section .data
// 374 bytes in section .rodata
// 884 bytes in section .text
// 
// 884 bytes of CODE  memory
// 374 bytes of CONST memory
// 817 bytes of DATA  memory
//
//Errors: none
//Warnings: none
