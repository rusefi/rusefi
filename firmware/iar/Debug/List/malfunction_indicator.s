///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:24 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\mal /
//                       function_indicator.cpp                               /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\mal /
//                       function_indicator.cpp -lCN                          /
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
//                       malfunction_indicator.s                              /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME malfunction_indicator

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _ZN9OutputPin8setValueEi
        EXTERN addConsoleAction
        EXTERN addError
        EXTERN chThdCreateStatic
        EXTERN chThdSleep
        EXTERN checkEnginePin
        EXTERN efiPow10
        EXTERN getErrorCodes
        EXTERN rlist

        PUBLIC _Z24initMalfunctionIndicatorv
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\malfunction_indicator.cpp
//    1 /**
//    2  * @file malfunction_indicator.cpp
//    3  * @brief We can blink out OBD-II error codes using Malfunction Indicator Light (MIL)
//    4  *
//    5  *
//    6  * @date Dec 20, 2013
//    7  * @author Konstantin Nikonenko
//    8  * @author Andrey Belomutskiy, (c) 2012-2015
//    9  * we show 4 digit error code - 1,5sec * (4xxx+1) digit + 0,4sec * (x3xxx+1) + ....
//   10  * ATTENTION!!! 0 = 1 blink, 1 = 2 blinks, ...., 9 = 10 blinks
//   11  * sequence is the constant!!!
//   12  *
//   13  *
//   14  * This file is part of rusEfi - see http://rusefi.com
//   15  *
//   16  * rusEfi is free software; you can redistribute it and/or modify it under the terms of
//   17  * the GNU General Public License as published by the Free Software Foundation; either
//   18  * version 3 of the License, or (at your option) any later version.
//   19  *
//   20  * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
//   21  * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   22  * GNU General Public License for more details.
//   23  *
//   24  * You should have received a copy of the GNU General Public License along with this program.
//   25  * If not, see <http://www.gnu.org/licenses/>.
//   26  */
//   27 
//   28 #include "main.h"
//   29 #include "io_pins.h"
//   30 #include "malfunction_central.h"
//   31 #include "malfunction_indicator.h"
//   32 #include "efiGpio.h"
//   33 
//   34 #if EFI_MALFUNCTION_INDICATOR || defined(__DOXYGEN__)
//   35 
//   36 #define MFI_LONG_BLINK	1500
//   37 #define MFI_SHORT_BLINK	400
//   38 #define MFI_BLINK_SEPARATOR 400
//   39 #define MFI_CHECKENGINE_LIGHT 10000
//   40 

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   41 static THD_WORKING_AREA(mfiThreadStack, UTILITY_THREAD_STACK_SIZE);	// declare thread
mfiThreadStack:
        DS8 712
//   42 
//   43 extern OutputPin checkEnginePin;
//   44 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z12blink_digitsii
        THUMB
//   45 static void blink_digits(int digit, int duration) {
_Z12blink_digitsii:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   46 	for (int iter = 0; iter < digit; iter++) {
        MOVS     R6,#+0
??blink_digits_0:
        CMP      R6,R4
        BGE.N    ??blink_digits_1
//   47 		checkEnginePin.setValue(0);
        MOVS     R1,#+0
        LDR.N    R0,??DataTable2
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
//   48 		chThdSleepMilliseconds(duration);
        MOV      R0,#+1000
        MUL      R0,R0,R5
        SUBS     R0,R0,#+1
        MOV      R1,#+1000
        UDIV     R0,R0,R1
        ADDS     R0,R0,#+1
          CFI FunCall chThdSleep
        BL       chThdSleep
//   49 		checkEnginePin.setValue(1);
        MOVS     R1,#+1
        LDR.N    R0,??DataTable2
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
//   50 		chThdSleepMilliseconds(MFI_BLINK_SEPARATOR);
        MOV      R0,#+400
          CFI FunCall chThdSleep
        BL       chThdSleep
//   51 	}
        ADDS     R6,R6,#+1
        B.N      ??blink_digits_0
//   52 }
??blink_digits_1:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock0
//   53 
//   54 // calculate how many digits our code have

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z11DigitLengthi
          CFI NoCalls
        THUMB
//   55 static int DigitLength(int digit) {
_Z11DigitLengthi:
        MOVS     R1,R0
//   56 	int i = 0;
        MOVS     R0,#+0
//   57 	while (digit > 0) {
??DigitLength_0:
        CMP      R1,#+1
        BLT.N    ??DigitLength_1
//   58 		digit = digit / 10;
        MOVS     R2,#+10
        SDIV     R1,R1,R2
//   59 		++i;
        ADDS     R0,R0,#+1
        B.N      ??DigitLength_0
//   60 	}
//   61 	return i;
??DigitLength_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   62 }
//   63 
//   64 // display code

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z16DisplayErrorCodeii
        THUMB
//   65 static void DisplayErrorCode(int length, int code) {
_Z16DisplayErrorCodeii:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//   66 	// todo: I suggest we use 'itoa' method to simplify this logic
//   67 	for (int iter = length - 1; iter >= 0; iter--) {
        SUBS     R6,R4,#+1
??DisplayErrorCode_0:
        CMP      R6,#+0
        BMI.N    ??DisplayErrorCode_1
//   68 		int ourDigit = (int) efiPow10(iter);		// 10^0 = 1, 10^1 = 10, 10^2=100, 10^3 = 1000, ....
        MOVS     R0,R6
          CFI FunCall efiPow10
        BL       efiPow10
        MOVS     R7,R0
//   69 		int digit = 1;						// as we remember "0" we show as one blink
        MOVS     R8,#+1
//   70 		while (code >= ourDigit) {
??DisplayErrorCode_2:
        CMP      R5,R7
        BLT.N    ??DisplayErrorCode_3
//   71 			code = code - ourDigit;
        SUBS     R5,R5,R7
//   72 			digit++;
        ADDS     R8,R8,#+1
        B.N      ??DisplayErrorCode_2
//   73 		}
//   74 		if (iter % 2 == 0)
??DisplayErrorCode_3:
        MOVS     R0,#+2
        SDIV     R1,R6,R0
        MLS      R1,R1,R0,R6
        CMP      R1,#+0
        BNE.N    ??DisplayErrorCode_4
//   75 			blink_digits(digit, MFI_SHORT_BLINK);		// even 2,0 - long blink
        MOV      R1,#+400
        MOV      R0,R8
          CFI FunCall _Z12blink_digitsii
        BL       _Z12blink_digitsii
        B.N      ??DisplayErrorCode_5
//   76 		else
//   77 			blink_digits(digit, MFI_LONG_BLINK); 		// odd  3,1 - short blink
??DisplayErrorCode_4:
        MOVW     R1,#+1500
        MOV      R0,R8
          CFI FunCall _Z12blink_digitsii
        BL       _Z12blink_digitsii
//   78 	}
??DisplayErrorCode_5:
        SUBS     R6,R6,#+1
        B.N      ??DisplayErrorCode_0
//   79 }
??DisplayErrorCode_1:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock2
//   80 
//   81 //  our main thread for show check engine error
//   82 #if defined __GNUC__
//   83 __attribute__((noreturn))    static msg_t mfiThread(void)
//   84 #else

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z9mfiThreadv
        THUMB
//   85 		static msg_t mfiThread(void)
//   86 #endif
//   87 		{
_Z9mfiThreadv:
        PUSH     {R4,R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+12
        SUB      SP,SP,#+44
          CFI CFA R13+56
//   88 	chRegSetThreadName("MFIndicator");
        LDR.N    R0,??DataTable2_1
        LDR      R0,[R0, #+24]
        LDR.N    R1,??DataTable2_2
        STR      R1,[R0, #+24]
//   89 	error_codes_set_s localErrorCopy;
//   90 
//   91 	while (TRUE) {
//   92 		chThdSleepSeconds(10);
??mfiThread_0:
        MOVW     R0,#+10000
          CFI FunCall chThdSleep
        BL       chThdSleep
//   93 
//   94 		getErrorCodes(&localErrorCopy);
        ADD      R0,SP,#+0
          CFI FunCall getErrorCodes
        BL       getErrorCodes
//   95 		for (int p = 0; p < localErrorCopy.count; p++) {
        MOVS     R4,#+0
??mfiThread_1:
        LDR      R0,[SP, #+0]
        CMP      R4,R0
        BGE.N    ??mfiThread_0
//   96 			// Calculate how many digits in this integer and display error code from start to end
//   97 			int code = localErrorCopy.error_codes[p];
        ADD      R0,SP,#+0
        ADDS     R0,R0,R4, LSL #+2
        LDR      R5,[R0, #+4]
//   98 			DisplayErrorCode(DigitLength(code), code);
        MOVS     R0,R5
          CFI FunCall _Z11DigitLengthi
        BL       _Z11DigitLengthi
        MOVS     R1,R5
          CFI FunCall _Z16DisplayErrorCodeii
        BL       _Z16DisplayErrorCodeii
//   99 		}
        ADDS     R4,R4,#+1
        B.N      ??mfiThread_1
          CFI EndBlock cfiBlock3
//  100 	}
//  101 }
//  102 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z7testMilv
        THUMB
//  103 static void testMil(void) {
_Z7testMilv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  104 	addError(OBD_Engine_Coolant_Temperature_Circuit_Malfunction);
        MOVS     R0,#+115
          CFI FunCall addError
        BL       addError
//  105 	addError(OBD_Intake_Air_Temperature_Circuit_Malfunction);
        MOVS     R0,#+110
          CFI FunCall addError
        BL       addError
//  106 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock4
//  107 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z24initMalfunctionIndicatorv
        THUMB
//  108 void initMalfunctionIndicator(void) {
_Z24initMalfunctionIndicatorv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  109 	// create static thread
//  110 	chThdCreateStatic(mfiThreadStack, sizeof(mfiThreadStack), LOWPRIO, (tfunc_t) mfiThread, NULL);
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        LDR.N    R3,??DataTable2_3
        MOVS     R2,#+2
        MOV      R1,#+712
        LDR.N    R0,??DataTable2_4
          CFI FunCall chThdCreateStatic
        BL       chThdCreateStatic
//  111 
//  112 	addConsoleAction("testmil", testMil);
        LDR.N    R1,??DataTable2_5
        LDR.N    R0,??DataTable2_6
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  113 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock5

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2:
        DC32     checkEnginePin

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_1:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_2:
        DC32     `?<Constant "MFIndicator">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_3:
        DC32     _Z9mfiThreadv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_4:
        DC32     mfiThreadStack

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_5:
        DC32     _Z7testMilv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_6:
        DC32     `?<Constant "testmil">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MFIndicator">`:
        DATA
        DC8 "MFIndicator"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "testmil">`:
        DATA
        DC8 "testmil"

        END
//  114 
//  115 #endif /* EFI_MALFUNCTION_INDICATOR */
// 
// 712 bytes in section .bss
//  20 bytes in section .rodata
// 300 bytes in section .text
// 
// 300 bytes of CODE  memory
//  20 bytes of CONST memory
// 712 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
