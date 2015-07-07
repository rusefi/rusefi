///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:29 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\util\dataloggin /
//                       g.cpp                                                /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\util\dataloggin /
//                       g.cpp -lCN F:\stuff\rusefi_sourceforge\firmware\iar\ /
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
//                       datalogging.s                                        /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME datalogging

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z14isConsoleReadyv
        EXTERN _Z18initLoggingCentralv
        EXTERN _Z19consoleOutputBufferPKhi
        EXTERN __aeabi_f2d
        EXTERN chvprintf
        EXTERN efiStrlen
        EXTERN firmwareError
        EXTERN getRemainingStack
        EXTERN itoa10
        EXTERN lockAnyContext
        EXTERN msObjectInit
        EXTERN rlist
        EXTERN scheduleLogging
        EXTERN strcpy
        EXTERN strlen
        EXTERN unlockAnyContext
        EXTERN warning

        PUBLIC _ZN18LoggingWithStorageC1EPKc
        PUBLIC _ZN18LoggingWithStorageC2EPKc
        PUBLIC _ZN7Logging15baseConstructorEv
        PUBLIC _ZN7LoggingC1EPKcPci
        PUBLIC _ZN7LoggingC1Ev
        PUBLIC _ZN7LoggingC2EPKcPci
        PUBLIC _ZN7LoggingC2Ev
        PUBLIC append
        PUBLIC appendFast
        PUBLIC appendFloat
        PUBLIC appendMsgPostfix
        PUBLIC appendMsgPrefix
        PUBLIC appendPrintf
        PUBLIC debugFloat
        PUBLIC debugInt
        PUBLIC initIntermediateLoggingBuffer
        PUBLIC initLoggingExt
        PUBLIC isInitialized
        PUBLIC printLine
        PUBLIC printMsg
        PUBLIC printWithLength
        PUBLIC remainingSize
        PUBLIC resetLogging
        PUBLIC scheduleMsg
        PUBLIC vappendPrintf
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\util\datalogging.cpp
//    1 /**
//    2  * @file    datalogging.cpp
//    3  * @brief   Buffered console output stream code
//    4  *
//    5  * Here we have a memory buffer and method related to
//    6  * printing messages into this buffer. The purpose of the
//    7  * buffer is to allow fast, non-blocking, thread-safe logging.
//    8  *
//    9  * The idea is that each interrupt handler would have it's own logging buffer. You can add
//   10  * stuff into this buffer without any locking since it's you own buffer, and once you get
//   11  * the whole message you invoke the scheduleLogging() method which appends your local content
//   12  * into the global logging buffer, from which it is later dispatched to the console by our
//   13  * main console thread.
//   14  *
//   15  * @date Feb 25, 2013
//   16  * @author Andrey Belomutskiy, (c) 2012-2015
//   17  *
//   18  * This file is part of rusEfi - see http://rusefi.com
//   19  *
//   20  * rusEfi is free software; you can redistribute it and/or modify it under the terms of
//   21  * the GNU General Public License as published by the Free Software Foundation; either
//   22  * version 3 of the License, or (at your option) any later version.
//   23  *
//   24  * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
//   25  * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   26  * GNU General Public License for more details.
//   27  *
//   28  * You should have received a copy of the GNU General Public License along with this program.
//   29  * If not, see <http://www.gnu.org/licenses/>.
//   30  *
//   31  */
//   32 
//   33 #include <stdbool.h>
//   34 #include "main.h"
//   35 
//   36 #if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
//   37 #include "chprintf.h"
//   38 #include "chmtx.h"
//   39 #include "memstreams.h"
//   40 #include "console_io.h"
//   41 #include "rfiutil.h"
//   42 #include "loggingcentral.h"
//   43 
//   44 // we use this magic constant to make sure it's not just a random non-zero int in memory
//   45 #define MAGIC_LOGGING_FLAG 45234441
//   46 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   47 static MemoryStream intermediateLoggingBuffer;
intermediateLoggingBuffer:
        DS8 20

        SECTION `.ccm`:DATA:REORDER:NOROOT(2)
//   48 static uint8_t intermediateLoggingBufferData[INTERMEDIATE_LOGGING_BUFFER_SIZE] CCM_OPTIONAL;
intermediateLoggingBufferData:
        DS8 2000
//   49 //todo define max-printf-buffer

        SECTION `.bss`:DATA:REORDER:NOROOT(0)
//   50 static bool_t intermediateLoggingBufferInited = false;
intermediateLoggingBufferInited:
        DS8 1

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
header:
        DS8 16

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _ZN7LoggingC2Ev
        THUMB
// __code __interwork __softfp Logging::subobject Logging()
_ZN7LoggingC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN7LoggingC1Ev
        BL       _ZN7LoggingC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _ZN7LoggingC2EPKcPci
        THUMB
// __code __interwork __softfp Logging::subobject Logging(char const *, char *, int)
_ZN7LoggingC2EPKcPci:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN7LoggingC1EPKcPci
        BL       _ZN7LoggingC1EPKcPci
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _ZN18LoggingWithStorageC2EPKc
        THUMB
// __code __interwork __softfp LoggingWithStorage::subobject LoggingWithStorage(char const *)
_ZN18LoggingWithStorageC2EPKc:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN18LoggingWithStorageC1EPKc
        BL       _ZN18LoggingWithStorageC1EPKc
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2
//   51 
//   52 /**
//   53  * @returns true if data does not fit into this buffer
//   54  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z14validateBufferP7Loggingj
        THUMB
//   55 static INLINE bool_t validateBuffer(Logging *logging, uint32_t extraLen) {
_Z14validateBufferP7Loggingj:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   56 	if (logging->buffer == NULL) {
        LDR      R0,[R4, #+44]
        CMP      R0,#+0
        BNE.N    ??validateBuffer_0
//   57 		firmwareError("Logging not initialized: %s", logging->name);
        LDR      R1,[R4, #+0]
        LDR.W    R0,??DataTable16_2
          CFI FunCall firmwareError
        BL       firmwareError
//   58 		return true;
        MOVS     R0,#+1
        B.N      ??validateBuffer_1
//   59 	}
//   60 
//   61 	if (remainingSize(logging) < extraLen + 1) {
??validateBuffer_0:
        MOVS     R0,R4
          CFI FunCall remainingSize
        BL       remainingSize
        ADDS     R1,R5,#+1
        CMP      R0,R1
        BCS.N    ??validateBuffer_2
//   62 #if EFI_PROD_CODE
//   63 		warning(OBD_PCM_Processor_Fault, "output overflow %s", logging->name);
        LDR      R2,[R4, #+0]
        LDR.W    R1,??DataTable16_3
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//   64 #endif
//   65 		return true;
        MOVS     R0,#+1
        B.N      ??validateBuffer_1
//   66 	}
//   67 	return false;
??validateBuffer_2:
        MOVS     R0,#+0
??validateBuffer_1:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock3
//   68 }
//   69 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function append
        THUMB
//   70 void append(Logging *logging, const char *text) {
append:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//   71 	efiAssertVoid(text != NULL, "append NULL");
        CMP      R5,#+0
        BNE.N    ??append_0
        LDR.W    R0,??DataTable16_4
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??append_1
//   72 	uint32_t extraLen = efiStrlen(text);
??append_0:
        MOVS     R0,R5
          CFI FunCall efiStrlen
        BL       efiStrlen
        MOVS     R6,R0
//   73 	bool_t isError = validateBuffer(logging, extraLen);
        MOVS     R1,R6
        MOVS     R0,R4
          CFI FunCall _Z14validateBufferP7Loggingj
        BL       _Z14validateBufferP7Loggingj
        MOVS     R7,R0
//   74 	if (isError) {
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+0
        BNE.N    ??append_1
//   75 		return;
//   76 	}
//   77 	strcpy(logging->linePointer, text);
??append_2:
        MOVS     R1,R5
        LDR      R0,[R4, #+48]
          CFI FunCall strcpy
        BL       strcpy
//   78 	/**
//   79 	 * And now we are pointing at the zero char at the end of the buffer again
//   80 	 */
//   81 	logging->linePointer += extraLen;
        LDR      R0,[R4, #+48]
        ADDS     R0,R6,R0
        STR      R0,[R4, #+48]
//   82 }
??append_1:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock4
//   83 
//   84 /**
//   85  * @note This method if fast because it does not validate much, be sure what you are doing
//   86  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function appendFast
          CFI NoCalls
        THUMB
//   87 void appendFast(Logging *logging, const char *text) {
appendFast:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
//   88 //  todo: fix this implementation? this would be a one-pass implementation instead of a two-pass
//   89 //	char c;
//   90 //	char *s = (char *) text;
//   91 //	do {
//   92 //		c = *s++;
//   93 //		*logging->linePointer++ = c;
//   94 //	} while (c != '\0');
//   95 	register char *s;
//   96 	for (s = (char *) text; *s; ++s)
        MOVS     R2,R1
??appendFast_0:
        LDRB     R3,[R2, #+0]
        CMP      R3,#+0
        BEQ.N    ??appendFast_1
        ADDS     R2,R2,#+1
        B.N      ??appendFast_0
//   97 		;
//   98 	int extraLen = (s - text);
??appendFast_1:
        SUBS     R3,R2,R1
//   99 
//  100 	s = logging->linePointer;
        LDR      R4,[R0, #+48]
        MOVS     R2,R4
//  101 	while ((*s++ = *text++) != 0)
??appendFast_2:
        LDRB     R4,[R1, #+0]
        ADDS     R1,R1,#+1
        STRB     R4,[R2, #+0]
        ADDS     R2,R2,#+1
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+0
        BNE.N    ??appendFast_2
//  102 		;
//  103 
//  104 //	strcpy(logging->linePointer, text);
//  105 	logging->linePointer += extraLen;
        LDR      R4,[R0, #+48]
        ADDS     R4,R3,R4
        STR      R4,[R0, #+48]
//  106 }
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock5
//  107 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z14vappendPrintfIP7LoggingPKc8_VA_LIST
        THUMB
//  108 static void vappendPrintfI(Logging *logging, const char *fmt, va_list arg) {
_Z14vappendPrintfIP7LoggingPKc8_VA_LIST:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  109 	intermediateLoggingBuffer.eos = 0; // reset
        LDR.W    R0,??DataTable16_5
        MOVS     R1,#+0
        STR      R1,[R0, #+12]
//  110 	efiAssertVoid(getRemainingStack(chThdSelf()) > 128, "lowstck#1b");
        LDR.W    R0,??DataTable16_6
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+129
        BGE.N    ??vappendPrintfI_0
        LDR.W    R0,??DataTable16_7
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??vappendPrintfI_1
//  111 	chvprintf((BaseSequentialStream *) &intermediateLoggingBuffer, fmt, arg);
??vappendPrintfI_0:
        MOVS     R2,R6
        MOVS     R1,R5
        LDR.N    R0,??DataTable16_5
          CFI FunCall chvprintf
        BL       chvprintf
//  112 	intermediateLoggingBuffer.buffer[intermediateLoggingBuffer.eos] = 0; // need to terminate explicitly
        LDR.N    R0,??DataTable16_5
        LDR      R0,[R0, #+12]
        LDR.N    R1,??DataTable16_5
        LDR      R1,[R1, #+4]
        MOVS     R2,#+0
        STRB     R2,[R0, R1]
//  113 	append(logging, (char *) intermediateLoggingBufferData);
        LDR.N    R1,??DataTable16_8
        MOVS     R0,R4
          CFI FunCall append
        BL       append
//  114 }
??vappendPrintfI_1:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock6
//  115 
//  116 /**
//  117  * this method acquires system lock to guard the shared intermediateLoggingBuffer memory stream
//  118  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function vappendPrintf
        THUMB
//  119 void vappendPrintf(Logging *logging, const char *fmt, va_list arg) {
vappendPrintf:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  120 	efiAssertVoid(getRemainingStack(chThdSelf()) > 128, "lowstck#5b");
        LDR.N    R0,??DataTable16_6
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+129
        BGE.N    ??vappendPrintf_0
        LDR.N    R0,??DataTable16_9
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??vappendPrintf_1
//  121 	if (!intermediateLoggingBufferInited) {
??vappendPrintf_0:
        LDR.N    R0,??DataTable16_10
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??vappendPrintf_2
//  122 		firmwareError("intermediateLoggingBufferInited not inited!");
        LDR.N    R0,??DataTable16_11
          CFI FunCall firmwareError
        BL       firmwareError
//  123 		return;
        B.N      ??vappendPrintf_1
//  124 	}
//  125 	int wasLocked = lockAnyContext();
??vappendPrintf_2:
          CFI FunCall lockAnyContext
        BL       lockAnyContext
        MOVS     R7,R0
//  126 	vappendPrintfI(logging, fmt, arg);
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z14vappendPrintfIP7LoggingPKc8_VA_LIST
        BL       _Z14vappendPrintfIP7LoggingPKc8_VA_LIST
//  127 	if (!wasLocked) {
        CMP      R7,#+0
        BNE.N    ??vappendPrintf_3
//  128 		unlockAnyContext();
          CFI FunCall unlockAnyContext
        BL       unlockAnyContext
//  129 	}
//  130 }
??vappendPrintf_3:
??vappendPrintf_1:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock7
//  131 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function appendPrintf
        THUMB
//  132 void appendPrintf(Logging *logging, const char *fmt, ...) {
appendPrintf:
        PUSH     {R2,R3}
          CFI CFA R13+8
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  133 	efiAssertVoid(getRemainingStack(chThdSelf()) > 128, "lowstck#4");
        LDR.N    R0,??DataTable16_6
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+129
        BGE.N    ??appendPrintf_0
        LDR.N    R0,??DataTable16_12
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??appendPrintf_1
//  134 	va_list ap;
//  135 	va_start(ap, fmt);
??appendPrintf_0:
        ADD      R0,SP,#+16
        MOVS     R6,R0
//  136 	vappendPrintf(logging, fmt, ap);
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall vappendPrintf
        BL       vappendPrintf
//  137 	va_end(ap);
//  138 }
??appendPrintf_1:
        POP      {R4-R6}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R6 SameValue
          CFI CFA R13+12
        LDR      PC,[SP], #+12    ;; return
          CFI EndBlock cfiBlock8
//  139 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function initLoggingExt
        THUMB
//  140 void initLoggingExt(Logging *logging, const char *name, char *buffer, int bufferSize) {
initLoggingExt:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
//  141 	logging->name = name;
        STR      R5,[R4, #+0]
//  142 	logging->buffer = buffer;
        STR      R6,[R4, #+44]
//  143 	logging->bufferSize = bufferSize;
        STR      R7,[R4, #+52]
//  144 	resetLogging(logging);
        MOVS     R0,R4
          CFI FunCall resetLogging
        BL       resetLogging
//  145 	logging->isInitialized = MAGIC_LOGGING_FLAG;
        LDR.N    R0,??DataTable16_13  ;; 0x2b23909
        STR      R0,[R4, #+56]
//  146 }
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock9
//  147 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function isInitialized
          CFI NoCalls
        THUMB
//  148 int isInitialized(Logging *logging) {
//  149 	return logging->isInitialized == MAGIC_LOGGING_FLAG;
isInitialized:
        LDR      R0,[R0, #+56]
        LDR.N    R1,??DataTable16_13  ;; 0x2b23909
        CMP      R0,R1
        BNE.N    ??isInitialized_0
        MOVS     R0,#+1
        B.N      ??isInitialized_1
??isInitialized_0:
        MOVS     R0,#+0
??isInitialized_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return
          CFI EndBlock cfiBlock10
//  150 }
//  151 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function debugInt
        THUMB
//  152 void debugInt(Logging *logging, const char *caption, int value) {
debugInt:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  153 	append(logging, caption);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall append
        BL       append
//  154 	append(logging, DELIMETER);
        ADR.N    R1,??DataTable16  ;; ","
        MOVS     R0,R4
          CFI FunCall append
        BL       append
//  155 	appendPrintf(logging, "%d%s", value, DELIMETER);
        ADR.N    R3,??DataTable16  ;; ","
        MOVS     R2,R6
        LDR.N    R1,??DataTable16_14
        MOVS     R0,R4
          CFI FunCall appendPrintf
        BL       appendPrintf
//  156 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock11
//  157 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function appendFloat
        THUMB
//  158 void appendFloat(Logging *logging, float value, int precision) {
appendFloat:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        VPUSH    {D8}
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        VMOV.F32 S16,S0
        MOVS     R5,R1
//  159 	/**
//  160 	 * todo: #1 this implementation is less than perfect
//  161 	 * todo: #2 The only way to avoid double promotion would probably be using *float instead of float
//  162 	 * See also http://stackoverflow.com/questions/5522051/printing-a-float-in-c-while-avoiding-variadic-parameter-promotion-to-double
//  163 	 */
//  164 	switch (precision) {
        CMP      R5,#+1
        BEQ.N    ??appendFloat_0
        BCC.N    ??appendFloat_1
        CMP      R5,#+3
        BEQ.N    ??appendFloat_2
        BCC.N    ??appendFloat_3
        CMP      R5,#+5
        BEQ.N    ??appendFloat_4
        BCC.N    ??appendFloat_5
        CMP      R5,#+6
        BEQ.N    ??appendFloat_6
        B.N      ??appendFloat_1
//  165 	case 1:
//  166 		appendPrintf(logging, "%..10f", value);
??appendFloat_0:
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable16_15
        MOVS     R0,R4
          CFI FunCall appendPrintf
        BL       appendPrintf
//  167 		break;
        B.N      ??appendFloat_7
//  168 	case 2:
//  169 		appendPrintf(logging, "%..100f", value);
??appendFloat_3:
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable16_16
        MOVS     R0,R4
          CFI FunCall appendPrintf
        BL       appendPrintf
//  170 		break;
        B.N      ??appendFloat_7
//  171 	case 3:
//  172 		appendPrintf(logging, "%..1000f", value);
??appendFloat_2:
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable16_17
        MOVS     R0,R4
          CFI FunCall appendPrintf
        BL       appendPrintf
//  173 		break;
        B.N      ??appendFloat_7
//  174 	case 4:
//  175 		appendPrintf(logging, "%..10000f", value);
??appendFloat_5:
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable16_18
        MOVS     R0,R4
          CFI FunCall appendPrintf
        BL       appendPrintf
//  176 		break;
        B.N      ??appendFloat_7
//  177 	case 5:
//  178 		appendPrintf(logging, "%..100000f", value);
??appendFloat_4:
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable16_19
        MOVS     R0,R4
          CFI FunCall appendPrintf
        BL       appendPrintf
//  179 		break;
        B.N      ??appendFloat_7
//  180 	case 6:
//  181 		appendPrintf(logging, "%..1000000f", value);
??appendFloat_6:
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable16_20
        MOVS     R0,R4
          CFI FunCall appendPrintf
        BL       appendPrintf
//  182 		break;
        B.N      ??appendFloat_7
//  183 
//  184 	default:
//  185 		appendPrintf(logging, "%f", value);
??appendFloat_1:
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        ADR.N    R1,??DataTable16_1  ;; 0x25, 0x66, 0x00, 0x00
        MOVS     R0,R4
          CFI FunCall appendPrintf
        BL       appendPrintf
//  186 	}
//  187 }
??appendFloat_7:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock12
//  188 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function debugFloat
        THUMB
//  189 void debugFloat(Logging *logging, const char *caption, float value, int precision) {
debugFloat:
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
        MOVS     R6,R2
//  190 	append(logging, caption);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall append
        BL       append
//  191 	append(logging, DELIMETER);
        ADR.N    R1,??DataTable16  ;; ","
        MOVS     R0,R4
          CFI FunCall append
        BL       append
//  192 
//  193 	appendFloat(logging, value, precision);
        MOVS     R1,R6
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall appendFloat
        BL       appendFloat
//  194 	append(logging, DELIMETER);
        ADR.N    R1,??DataTable16  ;; ","
        MOVS     R0,R4
          CFI FunCall append
        BL       append
//  195 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock13
//  196 
//  197 static char header[16];
//  198 
//  199 /**
//  200  * this method should invoked on the main thread only
//  201  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function printWithLength
        THUMB
//  202 void printWithLength(char *line) {
printWithLength:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  203 	/**
//  204 	 * this is my way to detect serial port transmission errors
//  205 	 * following code is functionally identical to
//  206 	 *   print("line:%d:%s\r\n", len, line);
//  207 	 * but it is faster because it outputs the whole buffer, not single characters
//  208 	 * We need this optimization because when we output larger chunks of data like the wave_chart:
//  209 	 * When we work with actual hardware, it is faster to invoke 'chSequentialStreamWrite' for the
//  210 	 * whole buffer then to invoke 'chSequentialStreamPut' once per character.
//  211 	 */
//  212 	int len = efiStrlen(line);
        MOVS     R0,R4
          CFI FunCall efiStrlen
        BL       efiStrlen
        MOVS     R5,R0
//  213 	strcpy(header, "line:");
        LDR.N    R1,??DataTable16_21
        LDR.N    R0,??DataTable16_22
          CFI FunCall strcpy
        BL       strcpy
//  214 	char *p = header + efiStrlen(header);
          CFI FunCall efiStrlen
        BL       efiStrlen
        LDR.N    R1,??DataTable16_22
        ADDS     R6,R0,R1
//  215 	p = itoa10(p, len);
        MOVS     R1,R5
        MOVS     R0,R6
          CFI FunCall itoa10
        BL       itoa10
        MOVS     R6,R0
//  216 	*p++ = ':';
        MOVS     R0,#+58
        STRB     R0,[R6, #+0]
        ADDS     R6,R6,#+1
//  217 	*p++ = '\0';
        MOVS     R0,#+0
        STRB     R0,[R6, #+0]
        ADDS     R6,R6,#+1
//  218 
//  219 	p = line;
        MOVS     R6,R4
//  220 	p += len;
        ADDS     R6,R5,R6
//  221 	*p++ = '\r';
        MOVS     R0,#+13
        STRB     R0,[R6, #+0]
        ADDS     R6,R6,#+1
//  222 	*p++ = '\n';
        MOVS     R0,#+10
        STRB     R0,[R6, #+0]
        ADDS     R6,R6,#+1
//  223 
//  224 	if (!isConsoleReady())
          CFI FunCall _Z14isConsoleReadyv
        BL       _Z14isConsoleReadyv
        CMP      R0,#+0
        BEQ.N    ??printWithLength_0
//  225 		return;
//  226 	consoleOutputBuffer((const uint8_t *) header, strlen(header));
??printWithLength_1:
        LDR.N    R0,??DataTable16_22
          CFI FunCall strlen
        BL       strlen
        MOVS     R1,R0
        LDR.N    R0,??DataTable16_22
          CFI FunCall _Z19consoleOutputBufferPKhi
        BL       _Z19consoleOutputBufferPKhi
//  227 	consoleOutputBuffer((const uint8_t *) line, p - line);
        SUBS     R1,R6,R4
        MOVS     R0,R4
          CFI FunCall _Z19consoleOutputBufferPKhi
        BL       _Z19consoleOutputBufferPKhi
//  228 }
??printWithLength_0:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock14
//  229 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function printLine
        THUMB
//  230 void printLine(Logging *logging) {
printLine:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  231 	printWithLength(logging->buffer);
        LDR      R0,[R4, #+44]
          CFI FunCall printWithLength
        BL       printWithLength
//  232 	resetLogging(logging);
        MOVS     R0,R4
          CFI FunCall resetLogging
        BL       resetLogging
//  233 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock15
//  234 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function appendMsgPrefix
        THUMB
//  235 void appendMsgPrefix(Logging *logging) {
appendMsgPrefix:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  236 	append(logging, "msg" DELIMETER);
        LDR.N    R1,??DataTable16_23
        MOVS     R0,R4
          CFI FunCall append
        BL       append
//  237 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock16
//  238 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function appendMsgPostfix
        THUMB
//  239 void appendMsgPostfix(Logging *logging) {
appendMsgPostfix:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  240 	append(logging, DELIMETER);
        ADR.N    R1,??DataTable16  ;; ","
        MOVS     R0,R4
          CFI FunCall append
        BL       append
//  241 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock17
//  242 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function resetLogging
        THUMB
//  243 void resetLogging(Logging *logging) {
resetLogging:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  244 	char *buffer = logging->buffer;
        LDR      R5,[R4, #+44]
//  245 	if (buffer == NULL) {
        CMP      R5,#+0
        BNE.N    ??resetLogging_0
//  246 		firmwareError("Null buffer: %s", logging->name);
        LDR      R1,[R4, #+0]
        LDR.N    R0,??DataTable16_24
          CFI FunCall firmwareError
        BL       firmwareError
//  247 		return;
        B.N      ??resetLogging_1
//  248 	}
//  249 	logging->linePointer = buffer;
??resetLogging_0:
        STR      R5,[R4, #+48]
//  250 	logging->linePointer[0] = 0;
        LDR      R0,[R4, #+48]
        MOVS     R1,#+0
        STRB     R1,[R0, #+0]
//  251 }
??resetLogging_1:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock18
//  252 
//  253 /**
//  254  * This method would output a simple console message immediately.
//  255  * This method should only be invoked on main thread because only the main thread can write to the console
//  256  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock19 Using cfiCommon0
          CFI Function printMsg
        THUMB
//  257 void printMsg(Logging *logger, const char *fmt, ...) {
printMsg:
        PUSH     {R2,R3}
          CFI CFA R13+8
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  258 	efiAssertVoid(getRemainingStack(chThdSelf()) > 128, "lowstck#5o");
        LDR.N    R0,??DataTable16_6
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+129
        BGE.N    ??printMsg_0
        LDR.N    R0,??DataTable16_25
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??printMsg_1
//  259 //	resetLogging(logging); // I guess 'reset' is not needed here?
//  260 	appendMsgPrefix(logger);
??printMsg_0:
        MOVS     R0,R4
          CFI FunCall appendMsgPrefix
        BL       appendMsgPrefix
//  261 
//  262 	va_list ap;
//  263 	va_start(ap, fmt);
        ADD      R0,SP,#+16
        MOVS     R6,R0
//  264 	vappendPrintf(logger, fmt, ap);
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall vappendPrintf
        BL       vappendPrintf
//  265 	va_end(ap);
//  266 
//  267 	append(logger, DELIMETER);
        ADR.N    R1,??DataTable16  ;; ","
        MOVS     R0,R4
          CFI FunCall append
        BL       append
//  268 	printLine(logger);
        MOVS     R0,R4
          CFI FunCall printLine
        BL       printLine
//  269 }
??printMsg_1:
        POP      {R4-R6}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R6 SameValue
          CFI CFA R13+12
        LDR      PC,[SP], #+12    ;; return
          CFI EndBlock cfiBlock19
//  270 
//  271 /**
//  272  * this whole method is executed under syslock so that we can have multiple threads use the same shared buffer
//  273  * in order to reduce memory usage
//  274  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock20 Using cfiCommon0
          CFI Function scheduleMsg
        THUMB
//  275 void scheduleMsg(Logging *logging, const char *fmt, ...) {
scheduleMsg:
        PUSH     {R2,R3}
          CFI CFA R13+8
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -12)
          CFI R7 Frame(CFA, -16)
          CFI R6 Frame(CFA, -20)
          CFI R5 Frame(CFA, -24)
          CFI R4 Frame(CFA, -28)
          CFI CFA R13+32
        MOVS     R4,R0
        MOVS     R5,R1
//  276 	efiAssertVoid(logging != NULL, "logging NULL");
        CMP      R4,#+0
        BNE.N    ??scheduleMsg_0
        LDR.N    R0,??DataTable16_26
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??scheduleMsg_1
//  277 	int wasLocked = lockAnyContext();
??scheduleMsg_0:
          CFI FunCall lockAnyContext
        BL       lockAnyContext
        MOVS     R6,R0
//  278 	resetLogging(logging); // todo: is 'reset' really needed here?
        MOVS     R0,R4
          CFI FunCall resetLogging
        BL       resetLogging
//  279 	appendMsgPrefix(logging);
        MOVS     R0,R4
          CFI FunCall appendMsgPrefix
        BL       appendMsgPrefix
//  280 
//  281 	va_list ap;
//  282 	va_start(ap, fmt);
        ADD      R0,SP,#+24
        MOVS     R7,R0
//  283 	vappendPrintf(logging, fmt, ap);
        MOVS     R2,R7
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall vappendPrintf
        BL       vappendPrintf
//  284 	va_end(ap);
//  285 
//  286 	appendMsgPostfix(logging);
        MOVS     R0,R4
          CFI FunCall appendMsgPostfix
        BL       appendMsgPostfix
//  287 	scheduleLogging(logging);
        MOVS     R0,R4
          CFI FunCall scheduleLogging
        BL       scheduleLogging
//  288 	if (!wasLocked) {
        CMP      R6,#+0
        BNE.N    ??scheduleMsg_2
//  289 		unlockAnyContext();
          CFI FunCall unlockAnyContext
        BL       unlockAnyContext
//  290 	}
//  291 }
??scheduleMsg_2:
??scheduleMsg_1:
        POP      {R0,R4-R7}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R6 SameValue
          CFI R7 SameValue
          CFI CFA R13+12
        LDR      PC,[SP], #+12    ;; return
          CFI EndBlock cfiBlock20
//  292 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock21 Using cfiCommon0
          CFI Function remainingSize
          CFI NoCalls
        THUMB
//  293 uint32_t remainingSize(Logging *logging) {
//  294 	return logging->bufferSize - loggingSize(logging);
remainingSize:
        LDR      R1,[R0, #+52]
        LDR      R2,[R0, #+48]
        SUBS     R1,R1,R2
        LDR      R0,[R0, #+44]
        ADDS     R0,R0,R1
        BX       LR               ;; return
          CFI EndBlock cfiBlock21
//  295 }
//  296 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock22 Using cfiCommon0
          CFI Function initIntermediateLoggingBuffer
        THUMB
//  297 void initIntermediateLoggingBuffer(void) {
initIntermediateLoggingBuffer:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  298 	initLoggingCentral();
          CFI FunCall _Z18initLoggingCentralv
        BL       _Z18initLoggingCentralv
//  299 
//  300 	msObjectInit(&intermediateLoggingBuffer, intermediateLoggingBufferData, INTERMEDIATE_LOGGING_BUFFER_SIZE, 0);
        MOVS     R3,#+0
        MOV      R2,#+2000
        LDR.N    R1,??DataTable16_8
        LDR.N    R0,??DataTable16_5
          CFI FunCall msObjectInit
        BL       msObjectInit
//  301 	intermediateLoggingBufferInited = TRUE;
        LDR.N    R0,??DataTable16_10
        MOVS     R1,#+1
        STRB     R1,[R0, #+0]
//  302 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock22

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16:
        DC8      ",",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_1:
        DC8      0x25, 0x66, 0x00, 0x00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_2:
        DC32     `?<Constant "Logging not initializ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_3:
        DC32     `?<Constant "output overflow %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_4:
        DC32     `?<Constant "append NULL">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_5:
        DC32     intermediateLoggingBuffer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_6:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_7:
        DC32     `?<Constant "lowstck#1b">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_8:
        DC32     intermediateLoggingBufferData

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_9:
        DC32     `?<Constant "lowstck#5b">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_10:
        DC32     intermediateLoggingBufferInited

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_11:
        DC32     `?<Constant "intermediateLoggingBu...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_12:
        DC32     `?<Constant "lowstck#4">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_13:
        DC32     0x2b23909

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_14:
        DC32     `?<Constant "%d%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_15:
        DC32     `?<Constant "%..10f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_16:
        DC32     `?<Constant "%..100f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_17:
        DC32     `?<Constant "%..1000f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_18:
        DC32     `?<Constant "%..10000f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_19:
        DC32     `?<Constant "%..100000f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_20:
        DC32     `?<Constant "%..1000000f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_21:
        DC32     `?<Constant "line:">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_22:
        DC32     header

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_23:
        DC32     `?<Constant "msg,">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_24:
        DC32     `?<Constant "Null buffer: %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_25:
        DC32     `?<Constant "lowstck#5o">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_26:
        DC32     `?<Constant "logging NULL">`
//  303 
//  304 #endif /* ! EFI_UNIT_TEST */
//  305 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock23 Using cfiCommon0
          CFI Function _ZN7Logging15baseConstructorEv
          CFI NoCalls
        THUMB
//  306 void Logging::baseConstructor() {
//  307 	name = NULL;
_ZN7Logging15baseConstructorEv:
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  308 	buffer = NULL;
        MOVS     R1,#+0
        STR      R1,[R0, #+44]
//  309 	linePointer = NULL;
        MOVS     R1,#+0
        STR      R1,[R0, #+48]
//  310 	bufferSize = 0;
        MOVS     R1,#+0
        STR      R1,[R0, #+52]
//  311 	isInitialized = false;
        MOVS     R1,#+0
        STR      R1,[R0, #+56]
//  312 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock23
//  313 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock24 Using cfiCommon0
          CFI Function _ZN7LoggingC1Ev
        THUMB
//  314 Logging::Logging() {
_ZN7LoggingC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  315 	baseConstructor();
        MOVS     R0,R4
          CFI FunCall _ZN7Logging15baseConstructorEv
        BL       _ZN7Logging15baseConstructorEv
//  316 }
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock24
//  317 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock25 Using cfiCommon0
          CFI Function _ZN7LoggingC1EPKcPci
        THUMB
//  318 Logging::Logging(char const *name, char *buffer, int bufferSize) {
_ZN7LoggingC1EPKcPci:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
//  319 	baseConstructor();
        MOVS     R0,R4
          CFI FunCall _ZN7Logging15baseConstructorEv
        BL       _ZN7Logging15baseConstructorEv
//  320 #if ! EFI_UNIT_TEST
//  321 	initLoggingExt(this, name, buffer, bufferSize);
        MOVS     R3,R7
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall initLoggingExt
        BL       initLoggingExt
//  322 #endif /* ! EFI_UNIT_TEST */
//  323 }
        MOVS     R0,R4
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock25
//  324 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock26 Using cfiCommon0
          CFI Function _ZN18LoggingWithStorageC1EPKc
        THUMB
//  325 LoggingWithStorage::LoggingWithStorage(const char *name) : Logging(name, DEFAULT_BUFFER, sizeof(DEFAULT_BUFFER))   {
_ZN18LoggingWithStorageC1EPKc:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R3,#+200
        ADDS     R2,R4,#+60
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _ZN7LoggingC2EPKcPci
        BL       _ZN7LoggingC2EPKcPci
//  326 }
        MOVS     R0,R4
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock26

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Logging not initializ...">`:
        DATA
        DC8 "Logging not initialized: %s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "output overflow %s">`:
        DATA
        DC8 "output overflow %s"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "append NULL">`:
        DATA
        DC8 "append NULL"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lowstck#1b">`:
        DATA
        DC8 "lowstck#1b"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lowstck#5b">`:
        DATA
        DC8 "lowstck#5b"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "intermediateLoggingBu...">`:
        DATA
        DC8 "intermediateLoggingBufferInited not inited!"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lowstck#4">`:
        DATA
        DC8 "lowstck#4"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 ","

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "%d%s">`:
        DATA
        DC8 "%d%s"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "%..10f">`:
        DATA
        DC8 "%..10f"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "%..100f">`:
        DATA
        DC8 "%..100f"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "%..1000f">`:
        DATA
        DC8 "%..1000f"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "%..10000f">`:
        DATA
        DC8 "%..10000f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "%..100000f">`:
        DATA
        DC8 "%..100000f"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "%..1000000f">`:
        DATA
        DC8 "%..1000000f"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "%f"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "line:">`:
        DATA
        DC8 "line:"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "msg,">`:
        DATA
        DC8 "msg,"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Null buffer: %s">`:
        DATA
        DC8 "Null buffer: %s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lowstck#5o">`:
        DATA
        DC8 "lowstck#5o"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "logging NULL">`:
        DATA
        DC8 "logging NULL"
        DC8 0, 0, 0

        END
// 
//    37 bytes in section .bss
// 2 000 bytes in section .ccm
//   278 bytes in section .rodata
// 1 288 bytes in section .text
// 
// 1 288 bytes of CODE  memory
//   278 bytes of CONST memory
// 2 037 bytes of DATA  memory
//
//Errors: none
//Warnings: none
