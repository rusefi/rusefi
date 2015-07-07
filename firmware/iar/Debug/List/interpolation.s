///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:30 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\cor /
//                       e\interpolation.cpp                                  /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\cor /
//                       e\interpolation.cpp -lCN F:\stuff\rusefi_sourceforge /
//                       \firmware\iar\Debug\List\ -lA                        /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                        -o F:\stuff\rusefi_sourceforge\firmware\iar\Debug\O /
//                       bj\ --no_cse --no_unroll --no_inline                 /
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
//                       interpolation.s                                      /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME interpolation

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN __aeabi_f2d
        EXTERN __iar_FDtest
        EXTERN addConsoleAction
        EXTERN firmwareError
        EXTERN scheduleMsg
        EXTERN warning

        PUBLIC _Z10findIndex2PKfjf
        PUBLIC _Z11interpolatefffff
        PUBLIC _Z13interpolate2dfPfS_i
        PUBLIC _Z13interpolate3dfPfifS_iPS_
        PUBLIC _Z13setTableValuePfS_iff
        PUBLIC _Z17initInterpolationP7Logging
        PUBLIC _Z9findIndexPKfif
        PUBLIC _ZN17FastInterpolation4initEffff
        PUBLIC _ZN17FastInterpolation8getValueEf
        PUBLIC _ZN17FastInterpolationC1Effff
        PUBLIC _ZN17FastInterpolationC1Ev
        PUBLIC _ZN17FastInterpolationC2Effff
        PUBLIC _ZN17FastInterpolationC2Ev
        PUBLIC array16
        PUBLIC logger
        PUBLIC needInterpolationLogging
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\core\interpolation.cpp
//    1 /**
//    2  * @file    interpolation.cpp
//    3  * @brief	Linear interpolation algorithms
//    4  *
//    5  * @date Oct 17, 2013
//    6  * @author Andrey Belomutskiy, (c) 2012-2015
//    7  * @author Dmitry Sidin, (c) 2015
//    8  */
//    9 
//   10 #if DEBUG_FUEL
//   11 #include <stdio.h>
//   12 #endif
//   13 
//   14 #include <math.h>
//   15 
//   16 #include "main.h"
//   17 #include "efilib2.h"
//   18 #include "interpolation.h"
//   19 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   20 int needInterpolationLogging = true;
needInterpolationLogging:
        DATA
        DC32 1
//   21 
//   22 #define BINARY_PERF true
//   23 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   24 Logging * logger;
logger:
        DS8 4
//   25 
//   26 #if BINARY_PERF
//   27 
//   28 #define COUNT 10000
//   29 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   30 float array16[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
array16:
        DATA
        DC32 3F800000H, 40000000H, 40400000H, 40800000H, 40A00000H, 40C00000H
        DC32 40E00000H, 41000000H, 41100000H, 41200000H, 41300000H, 41400000H
        DC32 41500000H, 41600000H, 41700000H, 41800000H

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _ZN17FastInterpolationC2Ev
        THUMB
// __code __interwork __softfp FastInterpolation::subobject FastInterpolation()
_ZN17FastInterpolationC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN17FastInterpolationC1Ev
        BL       _ZN17FastInterpolationC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _ZN17FastInterpolationC2Effff
        THUMB
// __code __interwork __vfp FastInterpolation::subobject FastInterpolation(float, float, float, float)
_ZN17FastInterpolationC2Effff:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN17FastInterpolationC1Effff
        BL       _ZN17FastInterpolationC1Effff
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1
//   31 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z10testBinaryv
        THUMB
//   32 static void testBinary(void) {
_Z10testBinaryv:
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
//   33 	const int size16 = 16;
        MOVS     R0,#+16
        STR      R0,[SP, #+0]
//   34 
//   35 	uint32_t totalOld = 0;
        MOVS     R4,#+0
//   36 	uint32_t totalNew = 0;
        MOVS     R5,#+0
//   37 
//   38 	for (int v = 0; v <= 16; v++) {
        MOVS     R6,#+0
??testBinary_0:
        CMP      R6,#+17
        BGE.N    ??testBinary_1
//   39 		uint32_t timeOld;
//   40 		{
//   41 			uint32_t start = GET_TIMESTAMP();
        LDR.W    R0,??DataTable7_1  ;; 0xe0001004
        LDR      R9,[R0, #+0]
//   42 			int temp = 0;
        MOVS     R0,#+0
//   43 			for (int i = 0; i < COUNT; i++) {
        MOVS     R10,#+0
??testBinary_2:
        MOVW     R1,#+10000
        CMP      R10,R1
        BGE.N    ??testBinary_3
//   44 				temp += findIndex(array16, size16, v);
        MOV      R8,R0
        VMOV     S0,R6
        VCVT.F32.S32 S0,S0
        MOVS     R1,#+16
        LDR.W    R0,??DataTable7_2
          CFI FunCall _Z9findIndexPKfif
        BL       _Z9findIndexPKfif
        ADDS     R0,R0,R8
//   45 			}
        ADDS     R10,R10,#+1
        B.N      ??testBinary_2
//   46 			timeOld = GET_TIMESTAMP() - start;
??testBinary_3:
        LDR.W    R1,??DataTable7_1  ;; 0xe0001004
        LDR      R1,[R1, #+0]
        SUBS     R1,R1,R9
        MOVS     R7,R1
//   47 		}
//   48 		uint32_t timeNew;
//   49 		{
//   50 			uint32_t start = GET_TIMESTAMP();
        LDR.W    R1,??DataTable7_1  ;; 0xe0001004
        LDR      R9,[R1, #+0]
//   51 			int temp = 0;
        MOVS     R10,#+0
//   52 			for (int i = 0; i < COUNT; i++) {
        MOVS     R11,#+0
??testBinary_4:
        MOVW     R0,#+10000
        CMP      R11,R0
        BGE.N    ??testBinary_5
//   53 				temp += findIndex2(array16, size16, v);
        VMOV     S0,R6
        VCVT.F32.S32 S0,S0
        MOVS     R1,#+16
        LDR.W    R0,??DataTable7_2
          CFI FunCall _Z10findIndex2PKfjf
        BL       _Z10findIndex2PKfjf
        ADDS     R10,R0,R10
//   54 			}
        ADDS     R11,R11,#+1
        B.N      ??testBinary_4
//   55 			timeNew = GET_TIMESTAMP() - start;
??testBinary_5:
        LDR.W    R0,??DataTable7_1  ;; 0xe0001004
        LDR      R0,[R0, #+0]
        SUBS     R0,R0,R9
        MOV      R8,R0
//   56 		}
//   57 		scheduleMsg(logger, "for v=%d old=%d ticks", v, timeOld);
        MOVS     R3,R7
        MOVS     R2,R6
        LDR.W    R1,??DataTable7_3
        LDR.W    R0,??DataTable7_4
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   58 		scheduleMsg(logger, "for v=%d new=%d ticks", v, timeNew);
        MOV      R3,R8
        MOVS     R2,R6
        LDR.W    R1,??DataTable7_5
        LDR.W    R0,??DataTable7_4
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   59 
//   60 		totalOld += timeOld;
        ADDS     R4,R7,R4
//   61 		totalNew += timeNew;
        ADDS     R5,R8,R5
//   62 	}
        ADDS     R6,R6,#+1
        B.N      ??testBinary_0
//   63 	scheduleMsg(logger, "totalOld=%d ticks", totalOld);
??testBinary_1:
        MOVS     R2,R4
        LDR.W    R1,??DataTable7_6
        LDR.W    R0,??DataTable7_4
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   64 	scheduleMsg(logger, "totalNew=%d ticks", totalNew);
        MOVS     R2,R5
        LDR.W    R1,??DataTable7_7
        LDR.W    R0,??DataTable7_4
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   65 
//   66 }
        POP      {R0,R4-R11,PC}   ;; return
          CFI EndBlock cfiBlock2
//   67 
//   68 #endif
//   69 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _ZN17FastInterpolationC1Ev
        THUMB
//   70 FastInterpolation::FastInterpolation() {
_ZN17FastInterpolationC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   71 	init(0, 0, 1, 1);
        VMOV.F32 S3,#1.0
        VMOV.F32 S2,#1.0
        VLDR.W   S1,??DataTable5  ;; 0x0
        VLDR.W   S0,??DataTable5  ;; 0x0
        MOVS     R0,R4
          CFI FunCall _ZN17FastInterpolation4initEffff
        BL       _ZN17FastInterpolation4initEffff
//   72 }
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//   73 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _ZN17FastInterpolationC1Effff
        THUMB
//   74 FastInterpolation::FastInterpolation(float x1, float y1, float x2, float y2) {
_ZN17FastInterpolationC1Effff:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8-D9}
          CFI D9 Frame(CFA, -16)
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        VMOV.F32 S16,S0
        VMOV.F32 S17,S1
        VMOV.F32 S18,S2
        VMOV.F32 S19,S3
//   75 	init(x1, y1, x2, y2);
        VMOV.F32 S3,S19
        VMOV.F32 S2,S18
        VMOV.F32 S1,S17
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _ZN17FastInterpolation4initEffff
        BL       _ZN17FastInterpolation4initEffff
//   76 }
        MOVS     R0,R4
        VPOP     {D8-D9}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock4
//   77 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _ZN17FastInterpolation4initEffff
        THUMB
//   78 void FastInterpolation::init(float x1, float y1, float x2, float y2) {
_ZN17FastInterpolation4initEffff:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8-D9}
          CFI D9 Frame(CFA, -16)
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        SUB      SP,SP,#+8
          CFI CFA R13+32
        MOVS     R4,R0
        VMOV.F32 S16,S0
        VMOV.F32 S17,S1
        VMOV.F32 S18,S2
        VMOV.F32 S19,S3
//   79 	if (x1 == x2) {
        VCMP.F32 S16,S18
        FMSTAT   
        BNE.N    ??init_0
//   80 		firmwareError("init: Same x1 and x2 in interpolate: %f/%f", x1, x2);
        VMOV     R0,S18
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R0,??DataTable7_8
          CFI FunCall firmwareError
        BL       firmwareError
//   81 		return;
        B.N      ??init_1
//   82 	}
//   83 	a = INTERPOLATION_A(x1, y1, x2, y2);
??init_0:
        VSUB.F32 S0,S17,S19
        VSUB.F32 S1,S16,S18
        VDIV.F32 S0,S0,S1
        VSTR     S0,[R4, #0]
//   84 	b = y1 - a * x1;
        VLDR     S0,[R4, #0]
        VMOV.F32 S1,S17
        VMLS.F32 S1,S0,S16
        VSTR     S1,[R4, #+4]
//   85 }
??init_1:
        ADD      SP,SP,#+8
          CFI CFA R13+24
        VPOP     {D8-D9}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock5
//   86 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _ZN17FastInterpolation8getValueEf
          CFI NoCalls
        THUMB
//   87 float FastInterpolation::getValue(float x) {
//   88 	return a * x + b;
_ZN17FastInterpolation8getValueEf:
        VLDR     S2,[R0, #0]
        VLDR     S1,[R0, #+4]
        VMLA.F32 S1,S2,S0
        VMOV.F32 S0,S1
        BX       LR               ;; return
          CFI EndBlock cfiBlock6
//   89 }
//   90 
//   91 /** @brief	Linear interpolation by two points
//   92  *
//   93  * @param	x1 key of the first point
//   94  * @param	y1 value of the first point
//   95  * @param	x2 key of the second point
//   96  * @param	y2 value of the second point
//   97  * @param	X key to be interpolated
//   98  *
//   99  * @note	For example, "interpolate(engineConfiguration.tpsMin, 0, engineConfiguration.tpsMax, 100, adc);"
//  100  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z11interpolatefffff
        THUMB
//  101 float interpolate(float x1, float y1, float x2, float y2, float x) {
_Z11interpolatefffff:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8-D10}
          CFI D10 Frame(CFA, -16)
          CFI D9 Frame(CFA, -24)
          CFI D8 Frame(CFA, -32)
          CFI CFA R13+32
        SUB      SP,SP,#+8
          CFI CFA R13+40
        VMOV.F32 S20,S0
        VMOV.F32 S16,S1
        VMOV.F32 S17,S2
        VMOV.F32 S18,S3
        VMOV.F32 S19,S4
//  102 	// todo: double comparison using EPS
//  103 	if (x1 == x2) {
        VCMP.F32 S20,S17
        FMSTAT   
        BNE.N    ??interpolate_0
//  104 		/**
//  105 		 * we could end up here for example while resetting bins while changing engine type
//  106 		 */
//  107 		warning(OBD_PCM_Processor_Fault, "interpolate: Same x1 and x2 in interpolate: %f/%f", x1, x2);
        VMOV     R0,S17
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        VMOV     R0,S20
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable7_9
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  108 		return NAN;
        VLDR.W   S0,??DataTable5_1  ;; 0x7fffffff
        B.N      ??interpolate_1
//  109 	}
//  110 
//  111 	// a*x1 + b = y1
//  112 	// a*x2 + b = y2
//  113 //	efiAssertVoid(x1 != x2, "no way we can interpolate");
//  114 	float a = INTERPOLATION_A(x1, y1, x2, y2);
??interpolate_0:
        VSUB.F32 S0,S16,S18
        VSUB.F32 S1,S20,S17
        VDIV.F32 S1,S0,S1
//  115 	float b = y1 - a * x1;
        VMOV.F32 S2,S16
        VMLS.F32 S2,S1,S20
//  116 	float result = a * x + b;
        VMOV.F32 S0,S2
        VMLA.F32 S0,S1,S19
//  117 #if	DEBUG_FUEL
//  118 	printf("x1=%f y1=%f x2=%f y2=%f\r\n", x1, y1, x2, y2);
//  119 	printf("a=%f b=%f result=%f\r\n", a, b, result);
//  120 #endif
//  121 	return result;
??interpolate_1:
        ADD      SP,SP,#+8
          CFI CFA R13+32
        VPOP     {D8-D10}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI D10 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock7
//  122 }
//  123 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z10findIndex2PKfjf
        THUMB
//  124 int findIndex2(const float array[], unsigned size, float value) {
_Z10findIndex2PKfjf:
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
        VMOV.F32 S16,S0
//  125 	efiAssert(!cisnan(value), "NaN in findIndex", 0);
        VMOV.F32 S0,S16
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BNE.N    ??findIndex2_0
        LDR.W    R0,??DataTable7_10
          CFI FunCall firmwareError
        BL       firmwareError
        MOVS     R0,#+0
        B.N      ??findIndex2_1
//  126 	efiAssert(size > 1, "NaN in findIndex", 0);
??findIndex2_0:
        CMP      R5,#+2
        BCS.N    ??findIndex2_2
        LDR.W    R0,??DataTable7_10
          CFI FunCall firmwareError
        BL       firmwareError
        MOVS     R0,#+0
        B.N      ??findIndex2_1
//  127 //	if (size <= 1)
//  128 //		return size && *array <= value ? 0 : -1;
//  129 
//  130 	signed i = 0;
??findIndex2_2:
        MOVS     R6,#+0
//  131 	//unsigned b = 1 << int(log(float(size) - 1) / 0.69314718055994530942);
//  132 	unsigned b = size >> 1; // in our case size is always a power of 2
        LSRS     R7,R5,#+1
//  133 	efiAssert(b + b == size, "Size not power of 2", -1);
        ADDS     R0,R7,R7
        CMP      R0,R5
        BEQ.N    ??findIndex2_3
        LDR.W    R0,??DataTable7_11
          CFI FunCall firmwareError
        BL       firmwareError
        MOVS     R0,#-1
        B.N      ??findIndex2_1
//  134 	for (; b; b >>= 1) {
??findIndex2_3:
        CMP      R7,#+0
        BEQ.N    ??findIndex2_4
//  135 		unsigned j = i | b;
        ORRS     R0,R7,R6
//  136 		/**
//  137 		 * it should be
//  138 		 * "if (j < size && array[j] <= value)"
//  139 		 * but in our case size is always power of 2 thus size is always more then j
//  140 		 */
//  141 		// efiAssert(j < size, "size", 0);
//  142 		if (array[j] <= value)
        ADDS     R1,R4,R0, LSL #+2
        VLDR     S0,[R1, #0]
        VCMP.F32 S16,S0
        FMSTAT   
        BLT.N    ??findIndex2_5
//  143 			i = j;
        MOVS     R6,R0
//  144 	}
??findIndex2_5:
        LSRS     R7,R7,#+1
        B.N      ??findIndex2_3
//  145 	return i || *array <= value ? i : -1;
??findIndex2_4:
        CMP      R6,#+0
        BNE.N    ??findIndex2_6
        VLDR     S0,[R4, #0]
        VCMP.F32 S16,S0
        FMSTAT   
        BLT.N    ??findIndex2_7
??findIndex2_6:
        B.N      ??findIndex2_8
??findIndex2_7:
        MOVS     R6,#-1
??findIndex2_8:
        MOVS     R0,R6
??findIndex2_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+24
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock8
//  146 }
//  147 
//  148 /** @brief	Binary search
//  149  * @returns	the highest index within sorted array such that array[i] is greater than or equal to the parameter
//  150  * @note If the parameter is smaller than the first element of the array, -1 is returned.
//  151  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z9findIndexPKfif
        THUMB
//  152 int findIndex(const float array[], int size, float value) {
_Z9findIndexPKfif:
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
        MOVS     R5,R1
        VMOV.F32 S16,S0
//  153 	efiAssert(!cisnan(value), "NaN in findIndex", 0);
        VMOV.F32 S0,S16
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BNE.N    ??findIndex_0
        LDR.N    R0,??DataTable7_10
          CFI FunCall firmwareError
        BL       firmwareError
        MOVS     R0,#+0
        B.N      ??findIndex_1
//  154 
//  155 	if (value < array[0])
??findIndex_0:
        VLDR     S0,[R4, #0]
        VCMP.F32 S16,S0
        FMSTAT   
        BPL.N    ??findIndex_2
//  156 		return -1;
        MOVS     R0,#-1
        B.N      ??findIndex_1
//  157 	int middle;
//  158 
//  159 	int left = 0;
??findIndex_2:
        MOVS     R1,#+0
//  160 	int right = size;
        MOVS     R2,R5
//  161 
//  162 	// todo: extract binary search as template method?
//  163 	while (true) {
//  164 #if 0
//  165 		// that's an assertion to make sure we do not loop here
//  166 		size--;
//  167 		efiAssert(size > 0, "Unexpected state in binary search", 0);
//  168 #endif
//  169 
//  170 		// todo: compare current implementation with
//  171 		// http://eigenjoy.com/2011/01/21/worlds-fastest-binary-search/
//  172 		// ?
//  173 		middle = (left + right) / 2;
??findIndex_3:
        ADDS     R3,R2,R1
        MOVS     R6,#+2
        SDIV     R3,R3,R6
        MOVS     R0,R3
//  174 
//  175 //		print("left=%d middle=%d right=%d: %f\r\n", left, middle, right, array[middle]);
//  176 
//  177 		if (middle == left)
        CMP      R0,R1
        BEQ.N    ??findIndex_4
//  178 			break;
//  179 
//  180 		if (value < array[middle]) {
??findIndex_5:
        ADDS     R3,R4,R0, LSL #+2
        VLDR     S0,[R3, #0]
        VCMP.F32 S16,S0
        FMSTAT   
        BPL.N    ??findIndex_6
//  181 			right = middle;
        MOVS     R2,R0
        B.N      ??findIndex_3
//  182 		} else if (value > array[middle]) {
??findIndex_6:
        ADDS     R3,R4,R0, LSL #+2
        VLDR     S0,[R3, #0]
        VCMP.F32 S0,S16
        FMSTAT   
        BPL.N    ??findIndex_7
//  183 			left = middle;
        MOVS     R1,R0
        B.N      ??findIndex_3
//  184 		} else {
//  185 			break;
//  186 		}
//  187 	}
//  188 
//  189 	return middle;
??findIndex_7:
??findIndex_4:
??findIndex_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock9
//  190 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5:
        DC32     0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_1:
        DC32     0x7fffffff
//  191 
//  192 /**
//  193  * @brief	One-dimensional table lookup with linear interpolation
//  194  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z13interpolate2dfPfS_i
        THUMB
//  195 float interpolate2d(float value, float bin[], float values[], int size) {
_Z13interpolate2dfPfS_i:
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
        VMOV.F32 S16,S0
        MOVS     R4,R0
        MOVS     R6,R1
        MOVS     R7,R2
//  196 	int index = findIndex(bin, size, value);
        VMOV.F32 S0,S16
        MOVS     R1,R7
        MOVS     R0,R4
          CFI FunCall _Z9findIndexPKfif
        BL       _Z9findIndexPKfif
        MOVS     R5,R0
//  197 
//  198 	if (index == -1)
        CMN      R5,#+1
        BNE.N    ??interpolate2d_0
//  199 		return values[0];
        VLDR     S0,[R6, #0]
        B.N      ??interpolate2d_1
//  200 	if (index == size - 1)
??interpolate2d_0:
        SUBS     R0,R7,#+1
        CMP      R5,R0
        BNE.N    ??interpolate2d_2
//  201 		return values[size - 1];
        ADDS     R0,R6,R7, LSL #+2
        SUBS     R0,R0,#+4
        VLDR     S0,[R0, #0]
        B.N      ??interpolate2d_1
//  202 
//  203 	return interpolate(bin[index], values[index], bin[index + 1], values[index + 1], value);
??interpolate2d_2:
        VMOV.F32 S4,S16
        ADDS     R0,R6,R5, LSL #+2
        VLDR     S3,[R0, #+4]
        ADDS     R0,R4,R5, LSL #+2
        VLDR     S2,[R0, #+4]
        ADDS     R0,R6,R5, LSL #+2
        VLDR     S1,[R0, #0]
        ADDS     R0,R4,R5, LSL #+2
        VLDR     S0,[R0, #0]
          CFI FunCall _Z11interpolatefffff
        BL       _Z11interpolatefffff
??interpolate2d_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+24
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock10
//  204 }
//  205 
//  206 /**
//  207  * @brief	Two-dimensional table lookup with linear interpolation
//  208  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z13interpolate3dfPfifS_iPS_
        THUMB
//  209 float interpolate3d(float x, float xBin[], int xBinSize, float y, float yBin[], int yBinSize, float* map[]) {
_Z13interpolate3dfPfifS_iPS_:
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
        VPUSH    {D8-D13}
          CFI D13 Frame(CFA, -48)
          CFI D12 Frame(CFA, -56)
          CFI D11 Frame(CFA, -64)
          CFI D10 Frame(CFA, -72)
          CFI D9 Frame(CFA, -80)
          CFI D8 Frame(CFA, -88)
          CFI CFA R13+88
        SUB      SP,SP,#+8
          CFI CFA R13+96
        VMOV.F32 S16,S0
        MOV      R8,R0
        MOVS     R4,R1
        VMOV.F32 S17,S1
        MOVS     R5,R2
        MOVS     R6,R3
        LDR      R7,[SP, #+96]
//  210 	if (cisnan(x)) {
        VMOV.F32 S0,S16
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BNE.N    ??interpolate3d_0
//  211 		warning(OBD_PCM_Processor_Fault, "%f: x is NaN in interpolate3d", x);
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable7_12
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  212 		return NAN;
        VLDR.W   S0,??DataTable7  ;; 0x7fffffff
        B.N      ??interpolate3d_1
//  213 	}
//  214 	if (cisnan(y)) {
??interpolate3d_0:
        VMOV.F32 S0,S17
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BNE.N    ??interpolate3d_2
//  215 		warning(OBD_PCM_Processor_Fault, "%f: y is NaN in interpolate3d", y);
        VMOV     R0,S17
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable7_13
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  216 		return NAN;
        VLDR.W   S0,??DataTable7  ;; 0x7fffffff
        B.N      ??interpolate3d_1
//  217 	}
//  218 
//  219 	int xIndex = findIndex(xBin, xBinSize, x);
??interpolate3d_2:
        VMOV.F32 S0,S16
        MOVS     R1,R4
        MOV      R0,R8
          CFI FunCall _Z9findIndexPKfif
        BL       _Z9findIndexPKfif
        MOV      R10,R0
//  220 #if	DEBUG_INTERPOLATION
//  221 	if (needInterpolationLogging)
//  222 		printf("X index=%d\r\n", xIndex);
//  223 #endif
//  224 	int yIndex = findIndex(yBin, yBinSize, y);
        VMOV.F32 S0,S17
        MOVS     R1,R6
        MOVS     R0,R5
          CFI FunCall _Z9findIndexPKfif
        BL       _Z9findIndexPKfif
        MOV      R9,R0
//  225 	if (xIndex < 0 && yIndex < 0) {
        CMP      R10,#+0
        BPL.N    ??interpolate3d_3
        CMP      R9,#+0
        BPL.N    ??interpolate3d_3
//  226 #if	DEBUG_INTERPOLATION
//  227 		if (needInterpolationLogging)
//  228 			printf("X and Y are smaller than smallest cell in table: %d\r\n", xIndex);
//  229 #endif
//  230 		return map[0][0];
        LDR      R0,[R7, #+0]
        VLDR     S0,[R0, #0]
        B.N      ??interpolate3d_1
//  231 	}
//  232 
//  233 	if (xIndex < 0) {
??interpolate3d_3:
        CMP      R10,#+0
        BPL.N    ??interpolate3d_4
//  234 #if	DEBUG_INTERPOLATION
//  235 		if (needInterpolationLogging)
//  236 			printf("X is smaller than smallest cell in table: %dr\n", xIndex);
//  237 #endif
//  238 		if (yIndex == yBinSize - 1)
        SUBS     R0,R6,#+1
        CMP      R9,R0
        BNE.N    ??interpolate3d_5
//  239 			return map[0][yIndex];
        LDR      R0,[R7, #+0]
        ADDS     R0,R0,R9, LSL #+2
        VLDR     S0,[R0, #0]
        B.N      ??interpolate3d_1
//  240 		float keyMin = yBin[yIndex];
??interpolate3d_5:
        LDR      R0,[R5, R9, LSL #+2]
        VMOV     S18,R0
//  241 		float keyMax = yBin[yIndex + 1];
        ADDS     R0,R5,R9, LSL #+2
        VLDR     S19,[R0, #+4]
//  242 		float rpmMinValue = map[0][yIndex];
        LDR      R0,[R7, #+0]
        LDR      R0,[R0, R9, LSL #+2]
        VMOV     S20,R0
//  243 		float rpmMaxValue = map[0][yIndex + 1];
        LDR      R0,[R7, #+0]
        ADDS     R0,R0,R9, LSL #+2
        VLDR     S21,[R0, #+4]
//  244 
//  245 		return interpolate(keyMin, rpmMinValue, keyMax, rpmMaxValue, y);
        VMOV.F32 S4,S17
        VMOV.F32 S3,S21
        VMOV.F32 S2,S19
        VMOV.F32 S1,S20
        VMOV.F32 S0,S18
          CFI FunCall _Z11interpolatefffff
        BL       _Z11interpolatefffff
        B.N      ??interpolate3d_1
//  246 	}
//  247 
//  248 	if (yIndex < 0) {
??interpolate3d_4:
        CMP      R9,#+0
        BPL.N    ??interpolate3d_6
//  249 #if	DEBUG_INTERPOLATION
//  250 		if (needInterpolationLogging)
//  251 			printf("Y is smaller than smallest cell in table: %d\r\n", yIndex);
//  252 #endif
//  253 		// no interpolation should be fine here.
//  254 		return map[xIndex][0];
        LDR      R0,[R7, R10, LSL #+2]
        VLDR     S0,[R0, #0]
        B.N      ??interpolate3d_1
//  255 	}
//  256 
//  257 	if (xIndex == xBinSize - 1 && yIndex == yBinSize - 1) {
??interpolate3d_6:
        SUBS     R0,R4,#+1
        CMP      R10,R0
        BNE.N    ??interpolate3d_7
        SUBS     R0,R6,#+1
        CMP      R9,R0
        BNE.N    ??interpolate3d_7
//  258 #if	DEBUG_INTERPOLATION
//  259 		if (needInterpolationLogging)
//  260 			printf("X and Y are larger than largest cell in table: %d %d\r\n", xIndex, yIndex);
//  261 #endif
//  262 		return map[xBinSize - 1][yBinSize - 1];
        ADDS     R0,R7,R4, LSL #+2
        LDR      R0,[R0, #-4]
        ADDS     R0,R0,R6, LSL #+2
        SUBS     R0,R0,#+4
        VLDR     S0,[R0, #0]
        B.N      ??interpolate3d_1
//  263 	}
//  264 
//  265 	if (xIndex == xBinSize - 1) {
??interpolate3d_7:
        SUBS     R0,R4,#+1
        CMP      R10,R0
        BNE.N    ??interpolate3d_8
//  266 #if	DEBUG_INTERPOLATION
//  267 		if (needInterpolationLogging)
//  268 			printf("TODO BETTER LOGGING x overflow %d\r\n", yIndex);
//  269 #endif
//  270 		// todo: implement better handling - y interpolation
//  271 		return map[xBinSize - 1][yIndex];
        ADDS     R0,R7,R4, LSL #+2
        LDR      R0,[R0, #-4]
        ADDS     R0,R0,R9, LSL #+2
        VLDR     S0,[R0, #0]
        B.N      ??interpolate3d_1
//  272 	}
//  273 
//  274 	if (yIndex == yBinSize - 1) {
??interpolate3d_8:
        SUBS     R0,R6,#+1
        CMP      R9,R0
        BNE.N    ??interpolate3d_9
//  275 #if	DEBUG_INTERPOLATION
//  276 		if (needInterpolationLogging)
//  277 			printf("Y is larger than largest cell in table: %d\r\n", yIndex);
//  278 #endif
//  279 		// todo: implement better handling - x interpolation
//  280 		return map[xIndex][yBinSize - 1];
        LDR      R0,[R7, R10, LSL #+2]
        ADDS     R0,R0,R6, LSL #+2
        SUBS     R0,R0,#+4
        VLDR     S0,[R0, #0]
        B.N      ??interpolate3d_1
//  281 	}
//  282 
//  283 	/*
//  284 	 * first we find the interpolated value for this RPM
//  285 	 */
//  286 	int rpmMaxIndex = xIndex + 1;
??interpolate3d_9:
        ADDS     R0,R10,#+1
        STR      R0,[SP, #+0]
//  287 
//  288 	float xMin = xBin[xIndex];
        LDR      R0,[R8, R10, LSL #+2]
        VMOV     S18,R0
//  289 	float xMax = xBin[xIndex + 1];
        ADDS     R0,R8,R10, LSL #+2
        VLDR     S19,[R0, #+4]
//  290 	float rpmMinKeyMinValue = map[xIndex][yIndex];
        LDR      R0,[R7, R10, LSL #+2]
        LDR      R0,[R0, R9, LSL #+2]
        VMOV     S27,R0
//  291 	float rpmMaxKeyMinValue = map[xIndex + 1][yIndex];
        ADDS     R0,R7,R10, LSL #+2
        LDR      R0,[R0, #+4]
        LDR      R0,[R0, R9, LSL #+2]
        VMOV     S20,R0
//  292 
//  293 	float keyMinValue = interpolate(xMin, rpmMinKeyMinValue, xMax, rpmMaxKeyMinValue, x);
        VMOV.F32 S4,S16
        VMOV.F32 S3,S20
        VMOV.F32 S2,S19
        VMOV.F32 S1,S27
        VMOV.F32 S0,S18
          CFI FunCall _Z11interpolatefffff
        BL       _Z11interpolatefffff
        VMOV.F32 S21,S0
//  294 
//  295 #if	DEBUG_INTERPOLATION
//  296 	if (needInterpolationLogging) {
//  297 		printf("X=%f:\r\nrange %f - %f\r\n", x, xMin, xMax);
//  298 		printf("X interpolation range %f   %f result %f\r\n", rpmMinKeyMinValue, rpmMaxKeyMinValue, keyMinValue);
//  299 	}
//  300 #endif
//  301 
//  302 	int keyMaxIndex = yIndex + 1;
        ADDS     R11,R9,#+1
//  303 	float keyMin = yBin[yIndex];
        LDR      R0,[R5, R9, LSL #+2]
        VMOV     S22,R0
//  304 	float keyMax = yBin[keyMaxIndex];
        LDR      R0,[R5, R11, LSL #+2]
        VMOV     S23,R0
//  305 	float rpmMinKeyMaxValue = map[xIndex][keyMaxIndex];
        LDR      R0,[R7, R10, LSL #+2]
        LDR      R0,[R0, R11, LSL #+2]
        VMOV     S24,R0
//  306 	float rpmMaxKeyMaxValue = map[rpmMaxIndex][keyMaxIndex];
        LDR      R0,[SP, #+0]
        LDR      R0,[R7, R0, LSL #+2]
        LDR      R0,[R0, R11, LSL #+2]
        VMOV     S25,R0
//  307 
//  308 	float keyMaxValue = interpolate(xMin, rpmMinKeyMaxValue, xMax, rpmMaxKeyMaxValue, x);
        VMOV.F32 S4,S16
        VMOV.F32 S3,S25
        VMOV.F32 S2,S19
        VMOV.F32 S1,S24
        VMOV.F32 S0,S18
          CFI FunCall _Z11interpolatefffff
        BL       _Z11interpolatefffff
        VMOV.F32 S26,S0
//  309 
//  310 #if	DEBUG_INTERPOLATION
//  311 	if (needInterpolationLogging) {
//  312 		printf("key=%f:\r\nrange %f - %f\r\n", y, keyMin, keyMax);
//  313 		printf("key interpolation range %f   %f result %f\r\n", rpmMinKeyMaxValue, rpmMaxKeyMaxValue, keyMaxValue);
//  314 	}
//  315 #endif
//  316 
//  317 	float result = interpolate(keyMin, keyMinValue, keyMax, keyMaxValue, y);
        VMOV.F32 S4,S17
        VMOV.F32 S3,S26
        VMOV.F32 S2,S23
        VMOV.F32 S1,S21
        VMOV.F32 S0,S22
          CFI FunCall _Z11interpolatefffff
        BL       _Z11interpolatefffff
//  318 	return result;
??interpolate3d_1:
        ADD      SP,SP,#+8
          CFI CFA R13+88
        VPOP     {D8-D13}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI D10 SameValue
          CFI D11 SameValue
          CFI D12 SameValue
          CFI D13 SameValue
          CFI CFA R13+40
        POP      {R0,R4-R11,PC}   ;; return
          CFI EndBlock cfiBlock11
//  319 }
//  320 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _Z13setTableValuePfS_iff
        THUMB
//  321 void setTableValue(float bins[], float values[], int size, float key, float value) {
_Z13setTableValuePfS_iff:
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
        MOVS     R5,R1
        MOVS     R6,R2
        VMOV.F32 S17,S0
        VMOV.F32 S16,S1
//  322 	int index = findIndex(bins, size, key);
        VMOV.F32 S0,S17
        MOVS     R1,R6
        MOVS     R0,R4
          CFI FunCall _Z9findIndexPKfif
        BL       _Z9findIndexPKfif
//  323 	if (index == -1)
        CMN      R0,#+1
        BNE.N    ??setTableValue_0
//  324 		index = 0;
        MOVS     R1,#+0
        MOVS     R0,R1
//  325 	values[index] = value;
??setTableValue_0:
        ADDS     R1,R5,R0, LSL #+2
        VSTR     S16,[R1, #0]
//  326 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock12
//  327 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z17initInterpolationP7Logging
        THUMB
//  328 void initInterpolation(Logging *sharedLogger) {
_Z17initInterpolationP7Logging:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  329 	logger = sharedLogger;
        LDR.N    R0,??DataTable7_4
        STR      R4,[R0, #+0]
//  330 #if BINARY_PERF && ! EFI_UNIT_TEST
//  331 	addConsoleAction("binarytest", testBinary);
        LDR.N    R1,??DataTable7_14
        LDR.N    R0,??DataTable7_15
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  332 #endif
//  333 
//  334 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock13

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7:
        DC32     0x7fffffff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_1:
        DC32     0xe0001004

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_2:
        DC32     array16

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_3:
        DC32     `?<Constant "for v=%d old=%d ticks">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_4:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_5:
        DC32     `?<Constant "for v=%d new=%d ticks">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_6:
        DC32     `?<Constant "totalOld=%d ticks">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_7:
        DC32     `?<Constant "totalNew=%d ticks">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_8:
        DC32     `?<Constant "init: Same x1 and x2 ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_9:
        DC32     `?<Constant "interpolate: Same x1 ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_10:
        DC32     `?<Constant "NaN in findIndex">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_11:
        DC32     `?<Constant "Size not power of 2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_12:
        DC32     `?<Constant "%f: x is NaN in inter...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_13:
        DC32     `?<Constant "%f: y is NaN in inter...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_14:
        DC32     _Z10testBinaryv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_15:
        DC32     `?<Constant "binarytest">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "for v=%d old=%d ticks">`:
        DATA
        DC8 "for v=%d old=%d ticks"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "for v=%d new=%d ticks">`:
        DATA
        DC8 "for v=%d new=%d ticks"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "totalOld=%d ticks">`:
        DATA
        DC8 "totalOld=%d ticks"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "totalNew=%d ticks">`:
        DATA
        DC8 "totalNew=%d ticks"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "interpolate: Same x1 ...">`:
        DATA
        DC8 "interpolate: Same x1 and x2 in interpolate: %f/%f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "NaN in findIndex">`:
        DATA
        DC8 "NaN in findIndex"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Size not power of 2">`:
        DATA
        DC8 "Size not power of 2"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "%f: x is NaN in inter...">`:
        DATA
        DC8 "%f: x is NaN in interpolate3d"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "%f: y is NaN in inter...">`:
        DATA
        DC8 "%f: y is NaN in interpolate3d"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "binarytest">`:
        DATA
        DC8 "binarytest"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "init: Same x1 and x2 ...">`:
        DATA
        DC8 "init: Same x1 and x2 in interpolate: %f/%f"
        DC8 0

        END
// 
//     4 bytes in section .bss
//    68 bytes in section .data
//   300 bytes in section .rodata
// 1 618 bytes in section .text
// 
// 1 618 bytes of CODE  memory
//   300 bytes of CONST memory
//    72 bytes of DATA  memory
//
//Errors: none
//Warnings: none
