///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:13 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\cor /
//                       e\avg_values.cpp                                     /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\cor /
//                       e\avg_values.cpp -lCN F:\stuff\rusefi_sourceforge\fi /
//                       rmware\iar\Debug\List\ -lA                           /
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
//                       avg_values.s                                         /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME avg_values

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        PUBLIC _Z11avgAddValueP8AvgTableiff
        PUBLIC _Z11avgGetValueP8AvgTableif
        PUBLIC _Z17avgGetValuesCountP8AvgTableif
        PUBLIC _Z20avgGetValueByIndexesP8AvgTableii
        PUBLIC _Z7avgFillP8AvgTableif
        PUBLIC _Z8avgResetP8AvgTable
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\core\avg_values.cpp
//    1 /*
//    2  * avg_values.c
//    3  *
//    4  * @date Jul 23, 2013
//    5  * @author Andrey Belomutskiy, (c) 2012-2015
//    6  */
//    7 
//    8 #include "avg_values.h"
//    9 #include <math.h>
//   10 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z7avgFillP8AvgTableif
          CFI NoCalls
        THUMB
//   11 void avgFill(AvgTable *table, int count, float value) {
_Z7avgFillP8AvgTableif:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
//   12 	for (int i = 0; i < AVG_TAB_SIZE; i++) {
        MOVS     R2,#+0
??avgFill_0:
        CMP      R2,#+48
        BGE.N    ??avgFill_1
//   13 		for (int j = 0; j < AVG_TAB_SIZE; j++) {
        MOVS     R3,#+0
??avgFill_2:
        CMP      R3,#+48
        BGE.N    ??avgFill_3
//   14 			table->counts[i][j] = count;
        MOVS     R4,#+192
        MLA      R4,R4,R2,R0
        ADDS     R4,R4,R3, LSL #+2
        ADDS     R4,R4,#+9216
        STR      R1,[R4, #+0]
//   15 			table->values[i][j] = value;
        MOVS     R4,#+192
        MLA      R4,R4,R2,R0
        ADDS     R4,R4,R3, LSL #+2
        VSTR     S0,[R4, #0]
//   16 		}
        ADDS     R3,R3,#+1
        B.N      ??avgFill_2
//   17 	}
??avgFill_3:
        ADDS     R2,R2,#+1
        B.N      ??avgFill_0
//   18 }
??avgFill_1:
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//   19 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z8avgResetP8AvgTable
        THUMB
//   20 void avgReset(AvgTable *table) {
_Z8avgResetP8AvgTable:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   21 	avgFill(table, 0, 0);
        VLDR.W   S0,??DataTable4  ;; 0x0
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall _Z7avgFillP8AvgTableif
        BL       _Z7avgFillP8AvgTableif
//   22 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1
//   23 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z11avgAddValueP8AvgTableiff
          CFI NoCalls
        THUMB
//   24 void avgAddValue(AvgTable *table, int rpm, float key, float value) {
_Z11avgAddValueP8AvgTableiff:
        PUSH     {R4,R5}
          CFI R5 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//   25   if (rpm >= MAX_RPM || key >= MAX_KEY) {
        CMP      R1,#+8000
        BGE.N    ??avgAddValue_0
        VMOV.F32 S2,#5.0
        VCMP.F32 S0,S2
        FMSTAT   
        BLT.N    ??avgAddValue_1
//   26 		return;
??avgAddValue_0:
        B.N      ??avgAddValue_2
//   27         }
//   28 	int i = (int)(AVG_TAB_SIZE * rpm / MAX_RPM);
??avgAddValue_1:
        MOVS     R2,#+48
        MUL      R2,R2,R1
        MOV      R3,#+8000
        SDIV     R2,R2,R3
//   29 	int j = (int)(AVG_TAB_SIZE * key / MAX_KEY);
        VLDR.W   S2,??DataTable4_1  ;; 0x42400000
        VMUL.F32 S2,S0,S2
        VMOV.F32 S3,#5.0
        VDIV.F32 S2,S2,S3
        VCVT.S32.F32 S2,S2
        VMOV     R3,S2
//   30 
//   31 	table->values[i][j] += value;
        MOVS     R4,#+192
        MLA      R4,R4,R2,R0
        ADDS     R4,R4,R3, LSL #+2
        MOVS     R5,#+192
        MLA      R5,R5,R2,R0
        ADDS     R5,R5,R3, LSL #+2
        VLDR     S2,[R5, #0]
        VADD.F32 S2,S2,S1
        VSTR     S2,[R4, #0]
//   32 	table->counts[i][j]++;
        MOVS     R4,#+192
        MLA      R4,R4,R2,R0
        ADDS     R4,R4,R3, LSL #+2
        ADDS     R4,R4,#+9216
        MOVS     R5,#+192
        MLA      R5,R5,R2,R0
        ADDS     R5,R5,R3, LSL #+2
        ADDS     R5,R5,#+9216
        LDR      R5,[R5, #+0]
        ADDS     R5,R5,#+1
        STR      R5,[R4, #+0]
//   33 }
??avgAddValue_2:
        POP      {R4,R5}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock2
//   34 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z20avgGetValueByIndexesP8AvgTableii
          CFI NoCalls
        THUMB
//   35 float avgGetValueByIndexes(AvgTable *table, int i, int j) {
//   36 	int count = table->counts[i][j];
_Z20avgGetValueByIndexesP8AvgTableii:
        MOVS     R3,#+192
        MLA      R3,R3,R1,R0
        ADDS     R3,R3,R2, LSL #+2
        ADDS     R3,R3,#+9216
        LDR      R3,[R3, #+0]
//   37 	if (count == 0) {
        CMP      R3,#+0
        BNE.N    ??avgGetValueByIndexes_0
//   38 		return NAN;
        VLDR.W   S0,??DataTable4_2  ;; 0x7fffffff
        B.N      ??avgGetValueByIndexes_1
//   39         }
//   40 	return table->values[i][j] / count;
??avgGetValueByIndexes_0:
        VMOV     S0,R3
        VCVT.F32.S32 S0,S0
        MOVS     R3,#+192
        MLA      R0,R3,R1,R0
        ADDS     R0,R0,R2, LSL #+2
        VLDR     S1,[R0, #0]
        VDIV.F32 S0,S1,S0
??avgGetValueByIndexes_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock3
//   41 }
//   42 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z11avgGetValueP8AvgTableif
        THUMB
//   43 float avgGetValue(AvgTable *table, int rpm, float key) {
_Z11avgGetValueP8AvgTableif:
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
//   44   if (rpm >= MAX_RPM || key >= MAX_KEY) {
        CMP      R5,#+8000
        BGE.N    ??avgGetValue_0
        VMOV.F32 S0,#5.0
        VCMP.F32 S16,S0
        FMSTAT   
        BLT.N    ??avgGetValue_1
//   45 		return NAN;
??avgGetValue_0:
        VLDR.W   S0,??DataTable4_2  ;; 0x7fffffff
        B.N      ??avgGetValue_2
//   46   }
//   47 	int i = (int)(AVG_TAB_SIZE * rpm / MAX_RPM);
??avgGetValue_1:
        MOVS     R0,#+48
        MUL      R0,R0,R5
        MOV      R1,#+8000
        SDIV     R6,R0,R1
//   48 	int j = (int)(AVG_TAB_SIZE * key / MAX_KEY);
        VLDR.W   S0,??DataTable4_1  ;; 0x42400000
        VMUL.F32 S0,S16,S0
        VMOV.F32 S1,#5.0
        VDIV.F32 S0,S0,S1
        VCVT.S32.F32 S17,S0
//   49 	return avgGetValueByIndexes(table, i, j);
        VMOV     R2,S17
        MOVS     R1,R6
        MOVS     R0,R4
          CFI FunCall _Z20avgGetValueByIndexesP8AvgTableii
        BL       _Z20avgGetValueByIndexesP8AvgTableii
??avgGetValue_2:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock4
//   50 }
//   51 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z17avgGetValuesCountP8AvgTableif
          CFI NoCalls
        THUMB
//   52 int avgGetValuesCount(AvgTable *table, int rpm, float key) {
_Z17avgGetValuesCountP8AvgTableif:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
        MOVS     R2,R0
//   53   if (rpm >= MAX_RPM || key >= MAX_KEY) {
        CMP      R1,#+8000
        BGE.N    ??avgGetValuesCount_0
        VMOV.F32 S1,#5.0
        VCMP.F32 S0,S1
        FMSTAT   
        BLT.N    ??avgGetValuesCount_1
//   54 		return 0;
??avgGetValuesCount_0:
        MOVS     R0,#+0
        B.N      ??avgGetValuesCount_2
//   55   }
//   56 	int i = (int)(AVG_TAB_SIZE * rpm / MAX_RPM);
??avgGetValuesCount_1:
        MOVS     R0,#+48
        MUL      R0,R0,R1
        MOV      R3,#+8000
        SDIV     R0,R0,R3
//   57 	int j = (int)(AVG_TAB_SIZE * key / MAX_KEY);
        VLDR.W   S1,??DataTable4_1  ;; 0x42400000
        VMUL.F32 S1,S0,S1
        VMOV.F32 S2,#5.0
        VDIV.F32 S1,S1,S2
        VCVT.S32.F32 S1,S1
        VMOV     R3,S1
//   58 
//   59 	return table->counts[i][j];
        MOVS     R4,#+192
        MLA      R0,R4,R0,R2
        ADDS     R0,R0,R3, LSL #+2
        ADDS     R0,R0,#+9216
        LDR      R0,[R0, #+0]
??avgGetValuesCount_2:
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock5
//   60 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4:
        DC32     0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_1:
        DC32     0x42400000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_2:
        DC32     0x7fffffff

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
// 
// 450 bytes in section .text
// 
// 450 bytes of CODE memory
//
//Errors: none
//Warnings: none
