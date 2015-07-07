///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:08 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\util\data_buffe /
//                       r.c                                                  /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\util\data_buffe /
//                       r.c -lCN F:\stuff\rusefi_sourceforge\firmware\iar\De /
//                       bug\List\ -lA F:\stuff\rusefi_sourceforge\firmware\i /
//                       ar\Debug\List\ -o F:\stuff\rusefi_sourceforge\firmwa /
//                       re\iar\Debug\Obj\ --no_cse --no_unroll --no_inline   /
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
//                       data_buffer.s                                        /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME data_buffer

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        PUBLIC dbAdd
        PUBLIC dbClear
        PUBLIC dbCopy
        PUBLIC dbIsFull
        PUBLIC modp
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\util\data_buffer.c
//    1 /*@
//    2  * @file	data_buffer.c
//    3  *
//    4  * @date Dec 8, 2012
//    5  * @author Andrey Belomutskiy, (c) 2012-2015
//    6  */
//    7 
//    8 #include "data_buffer.h"
//    9 #include "main.h"
//   10 #if EFI_PROD_CODE
//   11 //#include "eficonsole.h"
//   12 #endif /* EFI_PROD_CODE */
//   13 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function dbIsFull
          CFI NoCalls
        THUMB
//   14 int dbIsFull(data_buffer_s *db) {
//   15 	return db->size == DB_MAX_SIZE;
dbIsFull:
        ADDS     R0,R0,#+4096
        LDR      R0,[R0, #+0]
        CMP      R0,#+1024
        BNE.N    ??dbIsFull_0
        MOVS     R0,#+1
        B.N      ??dbIsFull_1
??dbIsFull_0:
        MOVS     R0,#+0
??dbIsFull_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//   16 }
//   17 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function dbClear
          CFI NoCalls
        THUMB
//   18 void dbClear(data_buffer_s *db) {
//   19 	db->size = 0;
dbClear:
        ADDS     R1,R0,#+4096
        MOVS     R2,#+0
        STR      R2,[R1, #+0]
//   20 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   21 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function dbCopy
          CFI NoCalls
        THUMB
//   22 void dbCopy(data_buffer_s *source, data_buffer_s *target) {
dbCopy:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
//   23 	int s = source->size;
        ADDS     R2,R0,#+4096
        LDR      R2,[R2, #+0]
//   24 	target->size = s;
        ADDS     R3,R1,#+4096
        STR      R2,[R3, #+0]
//   25 	for (int i = 0; i < s; i++)
        MOVS     R3,#+0
??dbCopy_0:
        CMP      R3,R2
        BGE.N    ??dbCopy_1
//   26 		target->elements[i] = source->elements[i];
        LDR      R4,[R0, R3, LSL #+2]
        STR      R4,[R1, R3, LSL #+2]
        ADDS     R3,R3,#+1
        B.N      ??dbCopy_0
//   27 }
??dbCopy_1:
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock2
//   28 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function dbAdd
          CFI NoCalls
        THUMB
//   29 void dbAdd(data_buffer_s *db, int value) {
dbAdd:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
//   30 	if (db->size == DB_MAX_SIZE)
        ADDS     R2,R0,#+4096
        LDR      R2,[R2, #+0]
        CMP      R2,#+1024
        BEQ.N    ??dbAdd_0
//   31 		return;
//   32 	int s = db->size;
??dbAdd_1:
        ADDS     R2,R0,#+4096
        LDR      R2,[R2, #+0]
//   33 	db->elements[s] = value;
        STR      R1,[R0, R2, LSL #+2]
//   34 	db->size = s + 1;
        ADDS     R3,R0,#+4096
        ADDS     R4,R2,#+1
        STR      R4,[R3, #+0]
//   35 }
??dbAdd_0:
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock3
//   36 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function modp
          CFI NoCalls
        THUMB
//   37 int modp(int param) {
//   38 	return param > EF_PERIOD ? param - EF_PERIOD : param;
modp:
        LDR.N    R1,??DataTable0  ;; 0x5f5e101
        CMP      R0,R1
        BLT.N    ??modp_0
        LDR.N    R1,??DataTable0_1  ;; 0xfa0a1f00
        ADDS     R0,R1,R0
        B.N      ??modp_1
??modp_0:
??modp_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//   39 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0:
        DC32     0x5f5e101

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_1:
        DC32     0xfa0a1f00

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
//   40 
//   41 //void dbPrint(data_buffer_s *db, char *message, int withDiff) {
//   42 //	int s = db->size;
//   43 //	print("buffer [%s] size=%d\r\n", message, s);
//   44 //	int range = db->elements[s - 1] - db->elements[0];
//   45 //	print("range %d\r\n", range);
//   46 //
//   47 //	for (int i = 0; i < s; i++) {
//   48 //		print("%d: %d", i, db->elements[i]);
//   49 //		if (withDiff && i > 0) {
//   50 //			int diff = modp(db->elements[i]) - modp(db->elements[i - 1]);
//   51 //			print(" t=%d", diff);
//   52 //		}
//   53 //		print("\r\n");
//   54 //	}
//   55 //}
//   56 
//   57 //void dbPrintTable(data_buffer_s *table[], char *caption[], int columns) {
//   58 //	for (int c = 0; c < columns; c++)
//   59 //		print("%7s", caption[c]);
//   60 //	print("\r\n");
//   61 //
//   62 //	for (int r = 0; r < DB_MAX_SIZE; r++) {
//   63 //		for (int c = 0; c < columns; c++) {
//   64 //			data_buffer_s *buf = table[c];
//   65 //			print("%7d", buf->elements[r]);
//   66 //		}
//   67 //		print("\r\n");
//   68 //	}
//   69 //}
//   70 
// 
// 126 bytes in section .text
// 
// 126 bytes of CODE memory
//
//Errors: none
//Warnings: none
