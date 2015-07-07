///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:44 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\util\efilib.cpp /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\util\efilib.cpp /
//                        -lCN F:\stuff\rusefi_sourceforge\firmware\iar\Debug /
//                       \List\ -lA F:\stuff\rusefi_sourceforge\firmware\iar\ /
//                       Debug\List\ -o F:\stuff\rusefi_sourceforge\firmware\ /
//                       iar\Debug\Obj\ --no_cse --no_unroll --no_inline      /
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
//                       efilib.s                                             /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME efilib

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN __aeabi_d2f
        EXTERN __aeabi_dmul
        EXTERN __aeabi_f2d
        EXTERN strcpy
        EXTERN strlen

        PUBLIC _Z12boolToStringb
        PUBLIC _Z14printHistogramP7LoggingP11histogram_s
        PUBLIC absF
        PUBLIC absI
        PUBLIC atoff
        PUBLIC atoi
        PUBLIC efiPow10
        PUBLIC efiRound
        PUBLIC efiStrlen
        PUBLIC efiTrim
        PUBLIC indexOf
        PUBLIC isSameF
        PUBLIC itoa10
        PUBLIC maxF
        PUBLIC maxI
        PUBLIC minF
        PUBLIC minI
        PUBLIC startsWith
        PUBLIC strEqual
        PUBLIC strEqualCaseInsensitive
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\util\efilib.cpp
//    1 /**
//    2  * @file	efilib.cpp
//    3  *
//    4  * We cannot use stdlib because we do not have malloc - so, we have to implement these functions
//    5  *
//    6  * @date Feb 21, 2014
//    7  * @author Andrey Belomutskiy, (c) 2012-2015
//    8  */
//    9 
//   10 #include <string.h>
//   11 #include <math.h>
//   12 #include "efilib.h"
//   13 #include "datalogging.h"
//   14 #include "histogram.h"
//   15 
//   16 #define _MAX_FILLER 11
//   17 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z12boolToStringb
          CFI NoCalls
        THUMB
//   18 const char * boolToString(bool value) {
//   19 	return value ? "Yes" : "No";
_Z12boolToStringb:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+0
        BEQ.N    ??boolToString_0
        ADR.N    R0,??DataTable3  ;; "Yes"
        B.N      ??boolToString_1
??boolToString_0:
        ADR.N    R0,??DataTable3_1  ;; 0x4E, 0x6F, 0x00, 0x00
??boolToString_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//   20 }
//   21 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function minI
          CFI NoCalls
        THUMB
//   22 int minI(int i1, int i2) {
//   23 	return i1 < i2 ? i1 : i2;
minI:
        CMP      R0,R1
        BLT.N    ??minI_0
??minI_1:
        MOVS     R0,R1
??minI_0:
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   24 }
//   25 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function efiRound
          CFI NoCalls
        THUMB
//   26 float efiRound(float value, float precision) {
efiRound:
        VMOV.F32 S2,S0
//   27 	int a = (int) (value / precision);
        VDIV.F32 S0,S2,S1
        VCVT.S32.F32 S0,S0
//   28 	return a * precision;
        VCVT.F32.S32 S0,S0
        VMUL.F32 S0,S0,S1
        BX       LR               ;; return
          CFI EndBlock cfiBlock2
//   29 }
//   30 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function absF
          CFI NoCalls
        THUMB
//   31 float absF(float value) {
//   32 	return value > 0 ? value : -value;
absF:
        VCMP.F32 S0,#0.0
        FMSTAT   
        BGT.N    ??absF_0
??absF_1:
        VNEG.F32 S0,S0
??absF_0:
        BX       LR               ;; return
          CFI EndBlock cfiBlock3
//   33 }
//   34 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function absI
          CFI NoCalls
        THUMB
//   35 int absI(int32_t value) {
//   36 	return value >= 0 ? value : -value;
absI:
        CMP      R0,#+0
        BPL.N    ??absI_0
??absI_1:
        RSBS     R0,R0,#+0
??absI_0:
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//   37 }
//   38 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function maxI
          CFI NoCalls
        THUMB
//   39 int maxI(int i1, int i2) {
//   40 	return i1 > i2 ? i1 : i2;
maxI:
        CMP      R1,R0
        BLT.N    ??maxI_0
??maxI_1:
        MOVS     R0,R1
??maxI_0:
        BX       LR               ;; return
          CFI EndBlock cfiBlock5
//   41 }
//   42 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function maxF
          CFI NoCalls
        THUMB
//   43 float maxF(float i1, float i2) {
//   44 	return i1 > i2 ? i1 : i2;
maxF:
        VCMP.F32 S1,S0
        FMSTAT   
        BMI.N    ??maxF_0
??maxF_1:
        VMOV.F32 S0,S1
??maxF_0:
        BX       LR               ;; return
          CFI EndBlock cfiBlock6
//   45 }
//   46 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function minF
          CFI NoCalls
        THUMB
//   47 float minF(float i1, float i2) {
//   48 	return i1 < i2 ? i1 : i2;
minF:
        VCMP.F32 S0,S1
        FMSTAT   
        BMI.N    ??minF_0
??minF_1:
        VMOV.F32 S0,S1
??minF_0:
        BX       LR               ;; return
          CFI EndBlock cfiBlock7
//   49 }
//   50 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function efiStrlen
          CFI NoCalls
        THUMB
//   51 uint32_t efiStrlen(const char *param) {
//   52 	register const char *s;
//   53 	for (s = param; *s; ++s)
efiStrlen:
        MOVS     R1,R0
??efiStrlen_0:
        LDRB     R2,[R1, #+0]
        CMP      R2,#+0
        BEQ.N    ??efiStrlen_1
        ADDS     R1,R1,#+1
        B.N      ??efiStrlen_0
//   54 		;
//   55 	return (s - param);
??efiStrlen_1:
        SUBS     R0,R1,R0
        BX       LR               ;; return
          CFI EndBlock cfiBlock8
//   56 }
//   57 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function efiTrim
        THUMB
//   58 char * efiTrim(char *param) {
efiTrim:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   59 	while (param[0] == ' ') {
??efiTrim_0:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+32
        BNE.N    ??efiTrim_1
//   60 		param++; // that would skip leading spaces
        ADDS     R4,R4,#+1
        B.N      ??efiTrim_0
//   61 	}
//   62 	int len = efiStrlen(param);
??efiTrim_1:
        MOVS     R0,R4
          CFI FunCall efiStrlen
        BL       efiStrlen
        MOVS     R1,R0
//   63 	while (len > 0 && param[len - 1] == ' ') {
??efiTrim_2:
        CMP      R1,#+1
        BLT.N    ??efiTrim_3
        ADDS     R0,R1,R4
        LDRB     R0,[R0, #-1]
        CMP      R0,#+32
        BNE.N    ??efiTrim_3
//   64 		param[len - 1] = 0;
        ADDS     R0,R1,R4
        MOVS     R2,#+0
        STRB     R2,[R0, #-1]
//   65 		len--;
        SUBS     R1,R1,#+1
        B.N      ??efiTrim_2
//   66 	}
//   67 	return param;
??efiTrim_3:
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock9
//   68 }
//   69 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function startsWith
        THUMB
//   70 bool startsWith(const char *line, const char *prefix) {
startsWith:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   71 	uint32_t len = efiStrlen(prefix);
        MOVS     R0,R5
          CFI FunCall efiStrlen
        BL       efiStrlen
        MOVS     R6,R0
//   72 	if (efiStrlen(line) < len) {
        MOVS     R0,R4
          CFI FunCall efiStrlen
        BL       efiStrlen
        CMP      R0,R6
        BCS.N    ??startsWith_0
//   73 		return false;
        MOVS     R0,#+0
        B.N      ??startsWith_1
//   74 	}
//   75 	for (uint32_t i = 0; i < len; i++) {
??startsWith_0:
        MOVS     R1,#+0
??startsWith_2:
        CMP      R1,R6
        BCS.N    ??startsWith_3
//   76 		if (line[i] != prefix[i]) {
        LDRB     R0,[R1, R4]
        LDRB     R2,[R1, R5]
        CMP      R0,R2
        BEQ.N    ??startsWith_4
//   77 			return false;
        MOVS     R0,#+0
        B.N      ??startsWith_1
//   78 		}
//   79 	}
??startsWith_4:
        ADDS     R1,R1,#+1
        B.N      ??startsWith_2
//   80 	return true;
??startsWith_3:
        MOVS     R0,#+1
??startsWith_1:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock10
//   81 }
//   82 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function indexOf
        THUMB
//   83 int indexOf(const char *string, char ch) {
indexOf:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   84 	// todo: there should be a standard function for this
//   85 	// todo: on the other hand MISRA wants us not to use standard headers
//   86 	int len = efiStrlen(string);
        MOVS     R0,R4
          CFI FunCall efiStrlen
        BL       efiStrlen
        MOVS     R1,R0
//   87 	for (int i = 0; i < len; i++) {
        MOVS     R0,#+0
??indexOf_0:
        CMP      R0,R1
        BGE.N    ??indexOf_1
//   88 		if (string[i] == ch) {
        LDRB     R2,[R0, R4]
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R2,R5
        BEQ.N    ??indexOf_2
//   89 			return i;
//   90 		}
//   91 	}
??indexOf_3:
        ADDS     R0,R0,#+1
        B.N      ??indexOf_0
//   92 	return -1;
??indexOf_1:
        MOVS     R0,#-1
??indexOf_2:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock11
//   93 }
//   94 
//   95 // string to integer

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function atoi
        THUMB
//   96 int atoi(const char *string) {
atoi:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//   97 	// todo: use stdlib '#include <stdlib.h> '
//   98 	int len = strlen(string);
        MOVS     R0,R4
          CFI FunCall strlen
        BL       strlen
        MOVS     R5,R0
//   99 	if (len == 0) {
        CMP      R5,#+0
        BNE.N    ??atoi_0
//  100 		return -ERROR_CODE;
        LDR.N    R0,??DataTable4_2  ;; 0xed731bd0
        B.N      ??atoi_1
//  101 	}
//  102 	if (string[0] == '-') {
??atoi_0:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+45
        BNE.N    ??atoi_2
//  103 		return -atoi(string + 1);
        ADDS     R0,R4,#+1
          CFI FunCall atoi
        BL       atoi
        RSBS     R0,R0,#+0
        B.N      ??atoi_1
//  104 	}
//  105 	int result = 0;
??atoi_2:
        MOVS     R0,#+0
//  106 
//  107 	for (int i = 0; i < len; i++) {
        MOVS     R1,#+0
??atoi_3:
        CMP      R1,R5
        BGE.N    ??atoi_4
//  108 		char ch = string[i];
        LDRB     R2,[R1, R4]
//  109 		if (ch < '0' || ch > '9') {
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        CMP      R2,#+48
        BLT.N    ??atoi_5
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        CMP      R2,#+58
        BLT.N    ??atoi_6
//  110 			return ERROR_CODE;
??atoi_5:
        LDR.N    R0,??DataTable4_3  ;; 0x128ce430
        B.N      ??atoi_1
//  111 		}
//  112 		int c = ch - '0';
??atoi_6:
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        SUBS     R3,R2,#+48
//  113 		result = result * 10 + c;
        MOVS     R6,#+10
        MLA      R0,R6,R0,R3
//  114 	}
        ADDS     R1,R1,#+1
        B.N      ??atoi_3
//  115 
//  116 	return result;
??atoi_4:
??atoi_1:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock12
//  117 }
//  118 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  119 static char todofixthismesswithcopy[100];
todofixthismesswithcopy:
        DS8 100
//  120 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z13ltoa_internalPclj
          CFI NoCalls
        THUMB
//  121 static char *ltoa_internal(char *p, long num, unsigned radix) {
_Z13ltoa_internalPclj:
        PUSH     {R4,R5}
          CFI R5 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  122 	int i;
//  123 	char *q;
//  124 
//  125 	q = p + _MAX_FILLER;
        ADDW     R5,R0,#+11
        MOVS     R4,R5
//  126 	do {
//  127 		i = (int) (num % radix);
??ltoa_internal_0:
        UDIV     R5,R1,R2
        MLS      R5,R2,R5,R1
        MOVS     R3,R5
//  128 		i += '0';
        ADDS     R3,R3,#+48
//  129 		if (i > '9')
        CMP      R3,#+58
        BLT.N    ??ltoa_internal_1
//  130 			i += 'A' - '0' - 10;
        ADDS     R3,R3,#+7
//  131 		*--q = i;
??ltoa_internal_1:
        SUBS     R4,R4,#+1
        STRB     R3,[R4, #+0]
//  132 	} while ((num /= radix) != 0);
        UDIV     R1,R1,R2
        CMP      R1,#+0
        BNE.N    ??ltoa_internal_0
//  133 
//  134 	i = (int) (p + _MAX_FILLER - q);
        ADDS     R5,R0,#+11
        SUBS     R5,R5,R4
        MOVS     R3,R5
//  135 	do {
//  136 		*p++ = *q++;
??ltoa_internal_2:
        LDRB     R5,[R4, #+0]
        STRB     R5,[R0, #+0]
        ADDS     R4,R4,#+1
        ADDS     R0,R0,#+1
//  137 	} while (--i);
        SUBS     R3,R3,#+1
        CMP      R3,#+0
        BNE.N    ??ltoa_internal_2
//  138 
//  139 	return p;
        POP      {R4,R5}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock13
//  140 }
//  141 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _Z11itoa_signedPcij
        THUMB
//  142 static char* itoa_signed(char *p, int num, unsigned radix) {
_Z11itoa_signedPcij:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  143 	if (num < 0) {
        CMP      R5,#+0
        BPL.N    ??itoa_signed_0
//  144 		*p++ = '-';
        MOVS     R0,#+45
        STRB     R0,[R4, #+0]
        ADDS     R4,R4,#+1
//  145 		char *end = ltoa_internal(p, -num, radix);
        MOVS     R2,R6
        RSBS     R1,R5,#+0
        MOVS     R0,R4
          CFI FunCall _Z13ltoa_internalPclj
        BL       _Z13ltoa_internalPclj
//  146 		*end = 0;
        MOVS     R1,#+0
        STRB     R1,[R0, #+0]
//  147 		return end;
        B.N      ??itoa_signed_1
//  148 	}
//  149 	char *end = ltoa_internal(p, num, radix);
??itoa_signed_0:
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z13ltoa_internalPclj
        BL       _Z13ltoa_internalPclj
//  150 	*end = 0;
        MOVS     R1,#+0
        STRB     R1,[R0, #+0]
//  151 	return end;
??itoa_signed_1:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock14
//  152 }
//  153 
//  154 /**
//  155  * Integer to string
//  156  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function itoa10
        THUMB
//  157 char* itoa10(char *p, int num) {
itoa10:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  158 // todo: unit test
//  159 	return itoa_signed(p, num, 10);
        MOVS     R2,#+10
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z11itoa_signedPcij
        BL       _Z11itoa_signedPcij
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock15
//  160 }
//  161 
//  162 #define EPS 0.0001
//  163 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function isSameF
        THUMB
//  164 bool isSameF(float v1, float v2) {
isSameF:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S17,S0
        VMOV.F32 S16,S1
//  165 	return absF(v1 - v2) < EPS;
        VSUB.F32 S0,S17,S16
          CFI FunCall absF
        BL       absF
        VLDR.W   S1,??DataTable4  ;; 0x38d1b718
        VCMP.F32 S0,S1
        FMSTAT   
        BPL.N    ??isSameF_0
        MOVS     R0,#+1
        B.N      ??isSameF_1
??isSameF_0:
        MOVS     R0,#+0
??isSameF_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R1,PC}          ;; return
          CFI EndBlock cfiBlock16
//  166 }
//  167 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function efiPow10
        THUMB
//  168 int efiPow10(int param) {
efiPow10:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  169 	switch (param) {
        CMP      R4,#+0
        BEQ.N    ??efiPow10_0
        CMP      R4,#+2
        BEQ.N    ??efiPow10_1
        BCC.N    ??efiPow10_2
        CMP      R4,#+4
        BEQ.N    ??efiPow10_3
        BCC.N    ??efiPow10_4
        CMP      R4,#+6
        BEQ.N    ??efiPow10_5
        BCC.N    ??efiPow10_6
        CMP      R4,#+8
        BEQ.N    ??efiPow10_7
        BCC.N    ??efiPow10_8
        B.N      ??efiPow10_9
//  170 	case 0:
//  171 		return 1;
??efiPow10_0:
        MOVS     R0,#+1
        B.N      ??efiPow10_10
//  172 	case 1:
//  173 		return 10;
??efiPow10_2:
        MOVS     R0,#+10
        B.N      ??efiPow10_10
//  174 	case 2:
//  175 		return 100;
??efiPow10_1:
        MOVS     R0,#+100
        B.N      ??efiPow10_10
//  176 	case 3:
//  177 		return 1000;
??efiPow10_4:
        MOV      R0,#+1000
        B.N      ??efiPow10_10
//  178 	case 4:
//  179 		return 10000;
??efiPow10_3:
        MOVW     R0,#+10000
        B.N      ??efiPow10_10
//  180 	case 5:
//  181 		return 100000;
??efiPow10_6:
        LDR.N    R0,??DataTable4_4  ;; 0x186a0
        B.N      ??efiPow10_10
//  182 	case 6:
//  183 		return 1000000;
??efiPow10_5:
        LDR.N    R0,??DataTable4_5  ;; 0xf4240
        B.N      ??efiPow10_10
//  184 	case 7:
//  185 		return 10000000;
??efiPow10_8:
        LDR.N    R0,??DataTable4_6  ;; 0x989680
        B.N      ??efiPow10_10
//  186 	case 8:
//  187 		return 100000000;
??efiPow10_7:
        LDR.N    R0,??DataTable4_7  ;; 0x5f5e100
        B.N      ??efiPow10_10
//  188 	}
//  189 	return 10 * efiPow10(10 - 1);
??efiPow10_9:
        MOVS     R0,#+9
          CFI FunCall efiPow10
        BL       efiPow10
        MOVS     R1,#+10
        MULS     R0,R1,R0
??efiPow10_10:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock17
//  190 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3:
        DC8      "Yes"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_1:
        DC8      0x4E, 0x6F, 0x00, 0x00
//  191 
//  192 /**
//  193  * string to float
//  194  * @return NAN in case of invalid string
//  195  * todo: explicit value for error code?
//  196  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function atoff
        THUMB
//  197 float atoff(const char *param) {
atoff:
        PUSH     {R3-R9,LR}
          CFI R14 Frame(CFA, -4)
          CFI R9 Frame(CFA, -8)
          CFI R8 Frame(CFA, -12)
          CFI R7 Frame(CFA, -16)
          CFI R6 Frame(CFA, -20)
          CFI R5 Frame(CFA, -24)
          CFI R4 Frame(CFA, -28)
          CFI CFA R13+32
        VPUSH    {D8}
          CFI D8 Frame(CFA, -40)
          CFI CFA R13+40
        MOV      R9,R0
//  198 	uint32_t totallen = strlen(param);
        MOV      R0,R9
          CFI FunCall strlen
        BL       strlen
        MOV      R8,R0
//  199 	if (totallen > sizeof(todofixthismesswithcopy) - 1)
        CMP      R8,#+100
        BCC.N    ??atoff_0
//  200 		return (float) NAN;
        VLDR.W   S0,??DataTable4_1  ;; 0x7fffffff
        B.N      ??atoff_1
//  201 	strcpy(todofixthismesswithcopy, param);
??atoff_0:
        MOV      R1,R9
        LDR.N    R0,??DataTable4_8
          CFI FunCall strcpy
        BL       strcpy
//  202 	char *string = todofixthismesswithcopy;
        LDR.N    R5,??DataTable4_8
//  203 
//  204 	// todo: is there a standard function?
//  205 	// todo: create a unit test
//  206 	int dotIndex = indexOf(string, '.');
        MOVS     R1,#+46
        MOVS     R0,R5
          CFI FunCall indexOf
        BL       indexOf
        MOVS     R4,R0
//  207 	if (dotIndex == -1) {
        CMN      R4,#+1
        BNE.N    ??atoff_2
//  208 		// just an integer
//  209 		int result = atoi(string);
        MOVS     R0,R5
          CFI FunCall atoi
        BL       atoi
        VMOV     S16,R0
//  210 		if (absI(result) == ERROR_CODE)
        VMOV     R0,S16
          CFI FunCall absI
        BL       absI
        LDR.N    R1,??DataTable4_3  ;; 0x128ce430
        CMP      R0,R1
        BNE.N    ??atoff_3
//  211 			return (float) NAN;
        VLDR.W   S0,??DataTable4_1  ;; 0x7fffffff
        B.N      ??atoff_1
//  212 		return (float) result;
??atoff_3:
        VCVT.F32.S32 S0,S16
        B.N      ??atoff_1
//  213 	}
//  214 	// todo: this needs to be fixed
//  215 	string[dotIndex] = 0;
??atoff_2:
        MOVS     R0,#+0
        STRB     R0,[R4, R5]
//  216 	int integerPart = atoi(string);
        MOVS     R0,R5
          CFI FunCall atoi
        BL       atoi
        VMOV     S16,R0
//  217 	if (absI(integerPart) == ERROR_CODE)
        VMOV     R0,S16
          CFI FunCall absI
        BL       absI
        LDR.N    R1,??DataTable4_3  ;; 0x128ce430
        CMP      R0,R1
        BNE.N    ??atoff_4
//  218 		return (float) NAN;
        VLDR.W   S0,??DataTable4_1  ;; 0x7fffffff
        B.N      ??atoff_1
//  219 	string += (dotIndex + 1);
??atoff_4:
        ADDS     R0,R4,R5
        ADDS     R5,R0,#+1
//  220 	int decimalLen = strlen(string);
        MOVS     R0,R5
          CFI FunCall strlen
        BL       strlen
        MOVS     R6,R0
//  221 	int decimal = atoi(string);
        MOVS     R0,R5
          CFI FunCall atoi
        BL       atoi
        VMOV     S17,R0
//  222 	if (absI(decimal) == ERROR_CODE)
        VMOV     R0,S17
          CFI FunCall absI
        BL       absI
        LDR.N    R1,??DataTable4_3  ;; 0x128ce430
        CMP      R0,R1
        BNE.N    ??atoff_5
//  223 		return (float) NAN;
        VLDR.W   S0,??DataTable4_1  ;; 0x7fffffff
        B.N      ??atoff_1
//  224 	float divider = 1.0;
??atoff_5:
        VMOV.F32 S0,#1.0
//  225 	// todo: reuse 'pow10' function which we have anyway
//  226 	for (int i = 0; i < decimalLen; i++) {
        MOVS     R7,#+0
??atoff_6:
        CMP      R7,R6
        BGE.N    ??atoff_7
//  227 		divider = divider * 10.0;
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,#+0
        LDR.N    R3,??DataTable4_9  ;; 0x40240000
          CFI FunCall __aeabi_dmul
        BL       __aeabi_dmul
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        VMOV     S0,R0
//  228 	}
        ADDS     R7,R7,#+1
        B.N      ??atoff_6
//  229 	return integerPart + decimal / divider;
??atoff_7:
        VCVT.F32.S32 S1,S16
        VCVT.F32.S32 S2,S17
        VDIV.F32 S0,S2,S0
        VADD.F32 S0,S1,S0
??atoff_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+32
        POP      {R0,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock18
//  230 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4:
        DC32     0x38d1b718

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_1:
        DC32     0x7fffffff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_2:
        DC32     0xed731bd0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_3:
        DC32     0x128ce430

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_4:
        DC32     0x186a0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_5:
        DC32     0xf4240

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_6:
        DC32     0x989680

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_7:
        DC32     0x5f5e100

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_8:
        DC32     todofixthismesswithcopy

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_9:
        DC32     0x40240000
//  231 
//  232 #define TO_LOWER(x) (((x)>='A' && (x)<='Z') ? (x) - 'A' + 'a' : (x))
//  233 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock19 Using cfiCommon0
          CFI Function strEqualCaseInsensitive
        THUMB
//  234 bool strEqualCaseInsensitive(const char *str1, const char *str2) {
strEqualCaseInsensitive:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  235 	int len1 = strlen(str1);
        MOVS     R0,R4
          CFI FunCall strlen
        BL       strlen
        MOVS     R6,R0
//  236 	int len2 = strlen(str2);
        MOVS     R0,R5
          CFI FunCall strlen
        BL       strlen
        MOVS     R1,R0
//  237 	if (len1 != len2) {
        CMP      R6,R1
        BEQ.N    ??strEqualCaseInsensitive_0
//  238 		return false;
        MOVS     R0,#+0
        B.N      ??strEqualCaseInsensitive_1
//  239 	}
//  240 	for (int i = 0; i < len1; i++)
??strEqualCaseInsensitive_0:
        MOVS     R2,#+0
??strEqualCaseInsensitive_2:
        CMP      R2,R6
        BGE.N    ??strEqualCaseInsensitive_3
//  241 		if (TO_LOWER(str1[i]) != TO_LOWER(str2[i]))
        LDRB     R0,[R2, R4]
        CMP      R0,#+65
        BLT.N    ??strEqualCaseInsensitive_4
        LDRB     R0,[R2, R4]
        CMP      R0,#+91
        BGE.N    ??strEqualCaseInsensitive_4
        LDRB     R0,[R2, R4]
        ADDS     R0,R0,#+32
        B.N      ??strEqualCaseInsensitive_5
??strEqualCaseInsensitive_4:
        LDRB     R0,[R2, R4]
??strEqualCaseInsensitive_5:
        LDRB     R3,[R2, R5]
        CMP      R3,#+65
        BLT.N    ??strEqualCaseInsensitive_6
        LDRB     R3,[R2, R5]
        CMP      R3,#+91
        BGE.N    ??strEqualCaseInsensitive_6
        LDRB     R3,[R2, R5]
        ADDS     R3,R3,#+32
        B.N      ??strEqualCaseInsensitive_7
??strEqualCaseInsensitive_6:
        LDRB     R3,[R2, R5]
??strEqualCaseInsensitive_7:
        CMP      R0,R3
        BEQ.N    ??strEqualCaseInsensitive_8
//  242 			return false;
        MOVS     R0,#+0
        B.N      ??strEqualCaseInsensitive_1
??strEqualCaseInsensitive_8:
        ADDS     R2,R2,#+1
        B.N      ??strEqualCaseInsensitive_2
//  243 	return true;
??strEqualCaseInsensitive_3:
        MOVS     R0,#+1
??strEqualCaseInsensitive_1:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock19
//  244 }
//  245 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock20 Using cfiCommon0
          CFI Function strEqual
        THUMB
//  246 bool strEqual(const char *str1, const char *str2) {
strEqual:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  247 	// todo: there must be a standard function?!
//  248 	int len1 = strlen(str1);
        MOVS     R0,R4
          CFI FunCall strlen
        BL       strlen
        MOVS     R6,R0
//  249 	int len2 = strlen(str2);
        MOVS     R0,R5
          CFI FunCall strlen
        BL       strlen
        MOVS     R1,R0
//  250 	if (len1 != len2) {
        CMP      R6,R1
        BEQ.N    ??strEqual_0
//  251 		return false;
        MOVS     R0,#+0
        B.N      ??strEqual_1
//  252 	}
//  253 	for (int i = 0; i < len1; i++)
??strEqual_0:
        MOVS     R2,#+0
??strEqual_2:
        CMP      R2,R6
        BGE.N    ??strEqual_3
//  254 		if (str1[i] != str2[i])
        LDRB     R0,[R2, R4]
        LDRB     R3,[R2, R5]
        CMP      R0,R3
        BEQ.N    ??strEqual_4
//  255 			return false;
        MOVS     R0,#+0
        B.N      ??strEqual_1
??strEqual_4:
        ADDS     R2,R2,#+1
        B.N      ??strEqual_2
//  256 	return true;
??strEqual_3:
        MOVS     R0,#+1
??strEqual_1:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock20
//  257 }
//  258 
//  259 /**
//  260  * @brief This function knows how to print a histogram_s summary
//  261  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock21 Using cfiCommon0
          CFI Function _Z14printHistogramP7LoggingP11histogram_s
          CFI NoCalls
        THUMB
//  262 void printHistogram(Logging *logging, histogram_s *histogram) {
//  263 #if EFI_HISTOGRAMS && ! EFI_UNIT_TEST
//  264 	int report[5];
//  265 	int len = hsReport(histogram, report);
//  266 
//  267 	resetLogging(logging);
//  268 	appendMsgPrefix(logging);
//  269 	appendPrintf(logging, "histogram %s *", histogram->name);
//  270 	for (int i = 0; i < len; i++)
//  271 	appendPrintf(logging, "%d ", report[i]);
//  272 	appendPrintf(logging, "*");
//  273 	appendMsgPostfix(logging);
//  274 	scheduleLogging(logging);
//  275 #else
//  276 	UNUSED(logging);
//  277 	UNUSED(histogram);
//  278 	
//  279 #endif /* EFI_HISTOGRAMS */
//  280 }
_Z14printHistogramP7LoggingP11histogram_s:
        BX       LR               ;; return
          CFI EndBlock cfiBlock21

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "Yes"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "No"
        DC8 0

        END
//  281 
// 
//   100 bytes in section .bss
//     8 bytes in section .rodata
// 1 044 bytes in section .text
// 
// 1 044 bytes of CODE  memory
//     8 bytes of CONST memory
//   100 bytes of DATA  memory
//
//Errors: none
//Warnings: none
