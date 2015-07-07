///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:10:03 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\cor /
//                       e\signal_filtering.c                                 /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\cor /
//                       e\signal_filtering.c -lCN                            /
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
//                       -On                                                  /
//    MISRA C version =  2004                                                 /
//      Enabled       =  1.2-1.4 2.3 3.4 4.2 6.5 7 8.2 8.3 9.3 12.3 13.6      /
//                       13.7 15.5 17.3 19.17 20.7-20.9 20.11                 /
//      Checked       =  1.4 2.3 4.2 6.5 7 8.2 8.3 9.3 12.3 13.7 15.5         /
//                       20.7-20.9 20.11                                      /
//      Not checked   =  1.1-1.3 1.5-2.2 2.4-4.1 5-6.4 8.1 8.4-9.2 10-12.2    /
//                       12.4-13.6 14-15.4 16-20.6 20.10 20.12 21             /
//    List file       =  F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                       signal_filtering.s                                   /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME signal_filtering

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        PUBLIC sfAddValue
        PUBLIC sfAddValue2
        PUBLIC sfGetValue
        PUBLIC sfInit
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\core\signal_filtering.c
//    1 /*
//    2  * signal_filtering.c
//    3  *
//    4  * @date Aug 5, 2013
//    5  * @author pc
//    6  */
//    7 
//    8 #ifndef SIGNAL_FILTERING_C_
//    9 #define SIGNAL_FILTERING_C_
//   10 
//   11 #include "signal_filtering.h"
//   12 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function sfInit
          CFI NoCalls
        THUMB
//   13 void sfInit(SignalFiltering *fs, float K, float initialValue) {
//   14 	fs->pointer = 0;
sfInit:
        MOVS     R1,#+0
        STR      R1,[R0, #+40]
//   15 	fs->K = K;
        VSTR     S0,[R0, #+44]
//   16 	fs->Vout = initialValue;
        VSTR     S1,[R0, #+52]
//   17 	fs->Vacc = initialValue * K;
        VMUL.F32 S2,S1,S0
        VSTR     S2,[R0, #+48]
//   18 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//   19 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function addCopyAndSort
          CFI NoCalls
        THUMB
//   20 static void addCopyAndSort(SignalFiltering *fs, float value) {
addCopyAndSort:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
//   21 	fs->values[fs->pointer] = value;
        LDR      R1,[R0, #+40]
        ADDS     R1,R0,R1, LSL #+2
        VSTR     S0,[R1, #0]
//   22 	fs->pointer = ++fs->pointer == FILTER_SIZE ? 0 : fs->pointer;
        LDR      R1,[R0, #+40]
        ADDS     R1,R1,#+1
        STR      R1,[R0, #+40]
        CMP      R1,#+5
        BNE.N    ??addCopyAndSort_0
        MOVS     R1,#+0
        B.N      ??addCopyAndSort_1
??addCopyAndSort_0:
        LDR      R1,[R0, #+40]
??addCopyAndSort_1:
        STR      R1,[R0, #+40]
//   23 
//   24 	for (int i = 0; i < FILTER_SIZE; i++)
        MOVS     R1,#+0
??addCopyAndSort_2:
        CMP      R1,#+5
        BGE.N    ??addCopyAndSort_3
//   25 		fs->sorted[i] = fs->values[i];
        ADDS     R2,R0,R1, LSL #+2
        LDR      R3,[R0, R1, LSL #+2]
        STR      R3,[R2, #+20]
        ADDS     R1,R1,#+1
        B.N      ??addCopyAndSort_2
//   26 
//   27 	for (int i = 0; i < FILTER_SIZE; i++)
??addCopyAndSort_3:
        MOVS     R1,#+0
??addCopyAndSort_4:
        CMP      R1,#+5
        BGE.N    ??addCopyAndSort_5
//   28 		for (int j = i + 1; j < FILTER_SIZE; j++)
        ADDS     R2,R1,#+1
??addCopyAndSort_6:
        CMP      R2,#+5
        BGE.N    ??addCopyAndSort_7
//   29 			if (fs->sorted[i] < fs->sorted[j]) {
        ADDS     R3,R0,R1, LSL #+2
        VLDR     S1,[R3, #+20]
        ADDS     R3,R0,R2, LSL #+2
        VLDR     S2,[R3, #+20]
        VCMP.F32 S1,S2
        FMSTAT   
        BPL.N    ??addCopyAndSort_8
//   30 				float temp = fs->sorted[i];
        ADDS     R3,R0,R1, LSL #+2
        VLDR     S1,[R3, #+20]
//   31 				fs->sorted[i] = fs->sorted[j];
        ADDS     R3,R0,R1, LSL #+2
        ADDS     R4,R0,R2, LSL #+2
        LDR      R4,[R4, #+20]
        STR      R4,[R3, #+20]
//   32 				fs->sorted[j] = temp;
        ADDS     R3,R0,R2, LSL #+2
        VSTR     S1,[R3, #+20]
//   33 			}
??addCopyAndSort_8:
        ADDS     R2,R2,#+1
        B.N      ??addCopyAndSort_6
??addCopyAndSort_7:
        ADDS     R1,R1,#+1
        B.N      ??addCopyAndSort_4
//   34 }
??addCopyAndSort_5:
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   35 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function sfAddValue
        THUMB
//   36 void sfAddValue(SignalFiltering *fs, float value) {
sfAddValue:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        VMOV.F32 S16,S0
//   37 	addCopyAndSort(fs, value);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall addCopyAndSort
        BL       addCopyAndSort
//   38 	float Vin = fs->sorted[FILTER_SIZE / 2];
        VLDR     S0,[R4, #+28]
//   39 
//   40 	fs->Vacc += Vin - fs->Vout;
        VLDR     S1,[R4, #+52]
        VSUB.F32 S1,S0,S1
        VLDR     S2,[R4, #+48]
        VADD.F32 S1,S1,S2
        VSTR     S1,[R4, #+48]
//   41 	fs->Vout = fs->Vacc / fs->K;
        VLDR     S1,[R4, #+48]
        VLDR     S2,[R4, #+44]
        VDIV.F32 S1,S1,S2
        VSTR     S1,[R4, #+52]
//   42 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2
//   43 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function sfAddValue2
        THUMB
//   44 void sfAddValue2(SignalFiltering *fs, float value) {
sfAddValue2:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        VMOV.F32 S16,S0
//   45 	addCopyAndSort(fs, value);
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall addCopyAndSort
        BL       addCopyAndSort
//   46 
//   47 	int fromIndex = FILTER_SIZE / 4;
        MOVS     R0,#+1
//   48 	int toIndex = FILTER_SIZE / 4 + FILTER_SIZE / 2;
        MOVS     R1,#+3
//   49 
//   50 	/**
//   51 	 * this implementation takes the average of the middle hald of the sorted values
//   52 	 */
//   53 	float result = 0;
        VLDR.W   S0,??DataTable0  ;; 0x0
//   54 	for (int i = fromIndex; i < toIndex; i++)
        MOVS     R2,R0
??sfAddValue2_0:
        CMP      R2,R1
        BGE.N    ??sfAddValue2_1
//   55 		result += fs->sorted[i];
        ADDS     R3,R4,R2, LSL #+2
        VLDR     S1,[R3, #+20]
        VADD.F32 S0,S0,S1
        ADDS     R2,R2,#+1
        B.N      ??sfAddValue2_0
//   56 
//   57 	fs->Vout = result / (FILTER_SIZE / 2);
??sfAddValue2_1:
        VMOV.F32 S1,#2.0
        VDIV.F32 S1,S0,S1
        VSTR     S1,[R4, #+52]
//   58 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0:
        DC32     0x0
//   59 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function sfGetValue
          CFI NoCalls
        THUMB
//   60 float sfGetValue(SignalFiltering *fs) {
//   61 	return fs->Vout;
sfGetValue:
        VLDR     S0,[R0, #+52]
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//   62 }

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        END
//   63 
//   64 #endif /* SIGNAL_FILTERING_C_ */
// 
// 298 bytes in section .text
// 
// 298 bytes of CODE memory
//
//Errors: none
//Warnings: none
