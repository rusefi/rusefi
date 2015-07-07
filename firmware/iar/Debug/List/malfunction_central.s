///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:27 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\alg /
//                       o\malfunction_central.c                              /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\alg /
//                       o\malfunction_central.c -lCN                         /
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
//                       malfunction_central.s                                /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME malfunction_central

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        PUBLIC addError
        PUBLIC getErrorCodes
        PUBLIC hasErrorCodes
        PUBLIC initMalfunctionCentral
        PUBLIC removeError
        PUBLIC setError
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\algo\malfunction_central.c
//    1 /**
//    2  * @file malfunction_central.c
//    3  * @brief This data structure holds current malfunction codes
//    4  *
//    5  * @date Dec 20, 2013
//    6  * @author Andrey Belomutskiy, (c) 2012-2015
//    7  */
//    8 
//    9 #include "malfunction_central.h"
//   10 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   11 static error_codes_set_s error_codes_set;
error_codes_set:
        DS8 44
//   12 
//   13 /**
//   14  * @return -1 if code not found
//   15  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function find_position
          CFI NoCalls
        THUMB
//   16 static int find_position(obd_code_e e_code)							// Search if code is present 
//   17 {
find_position:
        MOVS     R1,R0
//   18 	// cycle for searching element equal seaching code
//   19 	for (int t = 0; t < error_codes_set.count; t++)
        MOVS     R0,#+0
??find_position_0:
        LDR.N    R2,??DataTable5
        LDR      R2,[R2, #+0]
        CMP      R0,R2
        BGE.N    ??find_position_1
//   20 		if (error_codes_set.error_codes[t] == e_code)
        LDR.N    R2,??DataTable5
        ADDS     R2,R2,R0, LSL #+2
        LDR      R2,[R2, #+4]
        CMP      R2,R1
        BEQ.N    ??find_position_2
//   21 			return t;			// we found position where this code is present
??find_position_3:
        ADDS     R0,R0,#+1
        B.N      ??find_position_0
//   22 	return -1;														// -1 if code not found
??find_position_1:
        MOVS     R0,#-1
??find_position_2:
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//   23 }
//   24 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function initMalfunctionCentral
          CFI NoCalls
        THUMB
//   25 void initMalfunctionCentral(void) {
//   26 	error_codes_set.count = 0;
initMalfunctionCentral:
        LDR.N    R0,??DataTable5
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//   27 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   28 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function addError
        THUMB
//   29 void addError(obd_code_e errorCode) {
addError:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   30 	if (error_codes_set.count < MAX_ERROR_CODES_COUNT && find_position(errorCode) == -1) {
        LDR.N    R0,??DataTable5
        LDR      R0,[R0, #+0]
        CMP      R0,#+10
        BGE.N    ??addError_0
        MOVS     R0,R4
          CFI FunCall find_position
        BL       find_position
        CMN      R0,#+1
        BNE.N    ??addError_0
//   31 		error_codes_set.error_codes[error_codes_set.count] = errorCode;
        LDR.N    R0,??DataTable5
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable5
        ADDS     R0,R1,R0, LSL #+2
        STR      R4,[R0, #+4]
//   32 		error_codes_set.count++;
        LDR.N    R0,??DataTable5
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable5
        STR      R0,[R1, #+0]
//   33 	}
//   34 }
??addError_0:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2
//   35 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function removeError
        THUMB
//   36 void removeError(obd_code_e errorCode) {
removeError:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   37 	int pos = find_position(errorCode);
        MOVS     R0,R4
          CFI FunCall find_position
        BL       find_position
//   38 	if (pos >= 0) {
        CMP      R0,#+0
        BMI.N    ??removeError_0
//   39 		for (int t = pos; t < error_codes_set.count; t++) 					// shift all right elements to one pos left
        MOVS     R1,R0
??removeError_1:
        LDR.N    R2,??DataTable5
        LDR      R2,[R2, #+0]
        CMP      R1,R2
        BGE.N    ??removeError_2
//   40 			error_codes_set.error_codes[t] = error_codes_set.error_codes[t + 1];
        LDR.N    R2,??DataTable5
        ADDS     R2,R2,R1, LSL #+2
        LDR.N    R3,??DataTable5
        ADDS     R3,R3,R1, LSL #+2
        LDR      R3,[R3, #+8]
        STR      R3,[R2, #+4]
        ADDS     R1,R1,#+1
        B.N      ??removeError_1
//   41 	error_codes_set.error_codes[--error_codes_set.count] = (obd_code_e)0;				// place 0
??removeError_2:
        LDR.N    R1,??DataTable5
        LDR      R1,[R1, #+0]
        SUBS     R1,R1,#+1
        LDR.N    R2,??DataTable5
        STR      R1,[R2, #+0]
        LDR.N    R2,??DataTable5
        ADDS     R1,R2,R1, LSL #+2
        MOVS     R2,#+0
        STR      R2,[R1, #+4]
//   42 	}
//   43 }
??removeError_0:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//   44 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function setError
        THUMB
//   45 void setError(bool_t isError, obd_code_e errorCode) {
setError:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   46 	if (isError)
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+0
        BEQ.N    ??setError_0
//   47 		addError(errorCode);
        MOVS     R0,R5
          CFI FunCall addError
        BL       addError
        B.N      ??setError_1
//   48 	else
//   49 		removeError(errorCode);
??setError_0:
        MOVS     R0,R5
          CFI FunCall removeError
        BL       removeError
//   50 }
??setError_1:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock4
//   51 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function getErrorCodes
          CFI NoCalls
        THUMB
//   52 void getErrorCodes(error_codes_set_s * copy) {
//   53 	copy->count = error_codes_set.count;
getErrorCodes:
        LDR.N    R1,??DataTable5
        LDR      R1,[R1, #+0]
        STR      R1,[R0, #+0]
//   54 	for (int i = 0; i < copy->count; i++)
        MOVS     R1,#+0
??getErrorCodes_0:
        LDR      R2,[R0, #+0]
        CMP      R1,R2
        BGE.N    ??getErrorCodes_1
//   55 		copy->error_codes[i] = error_codes_set.error_codes[i];
        ADDS     R2,R0,R1, LSL #+2
        LDR.N    R3,??DataTable5
        ADDS     R3,R3,R1, LSL #+2
        LDR      R3,[R3, #+4]
        STR      R3,[R2, #+4]
        ADDS     R1,R1,#+1
        B.N      ??getErrorCodes_0
//   56 }
??getErrorCodes_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock5
//   57 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function hasErrorCodes
          CFI NoCalls
        THUMB
//   58 bool hasErrorCodes(void) {
//   59 	return error_codes_set.count > 0;
hasErrorCodes:
        LDR.N    R0,??DataTable5
        LDR      R0,[R0, #+0]
        CMP      R0,#+1
        BLT.N    ??hasErrorCodes_0
        MOVS     R0,#+1
        B.N      ??hasErrorCodes_1
??hasErrorCodes_0:
        MOVS     R0,#+0
??hasErrorCodes_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return
          CFI EndBlock cfiBlock6
//   60 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5:
        DC32     error_codes_set

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
//  44 bytes in section .bss
// 240 bytes in section .text
// 
// 240 bytes of CODE memory
//  44 bytes of DATA memory
//
//Errors: none
//Warnings: none
