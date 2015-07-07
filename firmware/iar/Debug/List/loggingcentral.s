///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:23 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\util\loggingcen /
//                       tral.cpp                                             /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\util\loggingcen /
//                       tral.cpp -lCN F:\stuff\rusefi_sourceforge\firmware\i /
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
//                       loggingcentral.s                                     /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME loggingcentral

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN __aeabi_memcpy
        EXTERN consoleInBinaryMode
        EXTERN efiStrlen
        EXTERN firmwareError
        EXTERN lockOutputBuffer
        EXTERN printWithLength
        EXTERN resetLogging
        EXTERN unlockOutputBuffer

        PUBLIC _Z17swapOutputBuffersPi
        PUBLIC _Z18initLoggingCentralv
        PUBLIC memcpy
        PUBLIC printPending
        PUBLIC scheduleLogging
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\util\loggingcentral.cpp
//    1 /**
//    2  * @file	loggingcentral.cpp
//    3  *
//    4  * @date Mar 8, 2015
//    5  * @author Andrey Belomutskiy, (c) 2012-2015
//    6  */
//    7 
//    8 #include "main.h"

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memcpy
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function memcpy
        THUMB
// __intrinsic __nounwind __interwork __softfp void *memcpy(void *, void const *, size_t)
memcpy:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall __aeabi_memcpy
        BL       __aeabi_memcpy
        MOVS     R0,R4
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock0
//    9 #include "efilib.h"
//   10 
//   11 #if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
//   12 
//   13 typedef char log_buf_t[DL_OUTPUT_BUFFER];
//   14 
//   15 /**
//   16  * we need to leave a byte for zero terminator, also two bytes for the \r\n in
//   17  * printWithLength, also couple of bytes just in case
//   18  */
//   19 #define MAX_DL_CAPACITY (DL_OUTPUT_BUFFER - 5)
//   20 
//   21 /**
//   22  * This is the buffer into which all the data providers write
//   23  */

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   24 static char *accumulationBuffer;
accumulationBuffer:
        DS8 4
//   25 

        SECTION `.ccm`:DATA:REORDER:NOROOT(2)
//   26 static log_buf_t pendingBuffers0 CCM_OPTIONAL;
pendingBuffers0:
        DS8 8000

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   27 static log_buf_t pendingBuffers1;
pendingBuffers1:
        DS8 8000
//   28 
//   29 /**
//   30  * amount of data accumulated so far
//   31  */

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   32 static uint32_t accumulatedSize;
accumulatedSize:
        DS8 4
//   33 
//   34 /**
//   35  * We copy all the pending data into this buffer once we are ready to push it out
//   36  */

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   37 static char * outputBuffer;
outputBuffer:
        DS8 4
//   38 
//   39 /**
//   40  * This method appends the content of this thread-local logger into the global buffer
//   41  * of logging content
//   42  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function scheduleLogging
        THUMB
//   43 void scheduleLogging(Logging *logging) {
scheduleLogging:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//   44 	// this could be done without locking
//   45 	int newLength = efiStrlen(logging->buffer);
        LDR      R0,[R4, #+44]
          CFI FunCall efiStrlen
        BL       efiStrlen
        MOVS     R5,R0
//   46 
//   47 	bool alreadyLocked = lockOutputBuffer();
          CFI FunCall lockOutputBuffer
        BL       lockOutputBuffer
        MOVS     R6,R0
//   48 	if (accumulatedSize + newLength >= MAX_DL_CAPACITY) {
        LDR.N    R0,??DataTable3
        LDR      R0,[R0, #+0]
        ADDS     R0,R5,R0
        MOVW     R1,#+7995
        CMP      R0,R1
        BCC.N    ??scheduleLogging_0
//   49 		/**
//   50 		 * if no one is consuming the data we have to drop it
//   51 		 * this happens in case of serial-over-USB, todo: find a better solution?
//   52 		 */
//   53 		if (!alreadyLocked) {
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+0
        BNE.N    ??scheduleLogging_1
//   54 			unlockOutputBuffer();
          CFI FunCall unlockOutputBuffer
        BL       unlockOutputBuffer
//   55 		}
//   56 		resetLogging(logging);
??scheduleLogging_1:
        MOVS     R0,R4
          CFI FunCall resetLogging
        BL       resetLogging
//   57 		return;
        B.N      ??scheduleLogging_2
//   58 	}
//   59 	// memcpy is faster then strcpy because it is not looking for line terminator
//   60 	memcpy(accumulationBuffer + accumulatedSize, logging->buffer, newLength + 1);
??scheduleLogging_0:
        ADDS     R2,R5,#+1
        LDR      R1,[R4, #+44]
        LDR.N    R0,??DataTable3
        LDR      R0,[R0, #+0]
        LDR.N    R3,??DataTable3_1
        LDR      R3,[R3, #+0]
        ADDS     R0,R0,R3
          CFI FunCall memcpy
        BL       memcpy
//   61 	accumulatedSize += newLength;
        LDR.N    R0,??DataTable3
        LDR      R0,[R0, #+0]
        ADDS     R0,R5,R0
        LDR.N    R1,??DataTable3
        STR      R0,[R1, #+0]
//   62 	if (!alreadyLocked) {
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+0
        BNE.N    ??scheduleLogging_3
//   63 		unlockOutputBuffer();
          CFI FunCall unlockOutputBuffer
        BL       unlockOutputBuffer
//   64 	}
//   65 	resetLogging(logging);
??scheduleLogging_3:
        MOVS     R0,R4
          CFI FunCall resetLogging
        BL       resetLogging
//   66 }
??scheduleLogging_2:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock1
//   67 
//   68 /**
//   69  * this method should always be invoked from the same thread!
//   70  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z17swapOutputBuffersPi
        THUMB
//   71 char * swapOutputBuffers(int *actualOutputBufferSize) {
_Z17swapOutputBuffersPi:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//   72 	int expectedOutputSize;
//   73 	{ // start of critical section
//   74 		lockOutputBuffer();
          CFI FunCall lockOutputBuffer
        BL       lockOutputBuffer
//   75 		/**
//   76 		 * we cannot output under syslock, we simply rotate which buffer is which
//   77 		 */
//   78 		char *temp = outputBuffer;
        LDR.N    R0,??DataTable3_2
        LDR      R6,[R0, #+0]
//   79 
//   80 #if EFI_ENABLE_ASSERTS
//   81 		expectedOutputSize = accumulatedSize;
        LDR.N    R0,??DataTable3
        LDR      R0,[R0, #+0]
        MOVS     R5,R0
//   82 #endif /* EFI_ENABLE_ASSERTS */
//   83 		outputBuffer = accumulationBuffer;
        LDR.N    R0,??DataTable3_2
        LDR.N    R1,??DataTable3_1
        LDR      R1,[R1, #+0]
        STR      R1,[R0, #+0]
//   84 
//   85 		accumulationBuffer = temp;
        LDR.N    R0,??DataTable3_1
        STR      R6,[R0, #+0]
//   86 		accumulatedSize = 0;
        LDR.N    R0,??DataTable3
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//   87 		accumulationBuffer[0] = 0;
        LDR.N    R0,??DataTable3_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STRB     R1,[R0, #+0]
//   88 
//   89 		unlockOutputBuffer();
          CFI FunCall unlockOutputBuffer
        BL       unlockOutputBuffer
//   90 	} // end of critical section
//   91 
//   92 	*actualOutputBufferSize = efiStrlen(outputBuffer);
        LDR.N    R0,??DataTable3_2
        LDR      R0,[R0, #+0]
          CFI FunCall efiStrlen
        BL       efiStrlen
        STR      R0,[R4, #+0]
//   93 	efiAssert(*actualOutputBufferSize == expectedOutputSize, "out constr", NULL);
        LDR      R0,[R4, #+0]
        CMP      R0,R5
        BEQ.N    ??swapOutputBuffers_0
        LDR.N    R0,??DataTable3_3
          CFI FunCall firmwareError
        BL       firmwareError
        MOVS     R0,#+0
        B.N      ??swapOutputBuffers_1
//   94 	return outputBuffer;
??swapOutputBuffers_0:
        LDR.N    R0,??DataTable3_2
        LDR      R0,[R0, #+0]
??swapOutputBuffers_1:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock2
//   95 }
//   96 
//   97 extern bool_t consoleInBinaryMode;
//   98 
//   99 /**
//  100  * This method actually sends all the pending data to the communication layer.
//  101  * This method is invoked by the main thread - that's the only thread which should be sending
//  102  * actual data to console in order to avoid concurrent access to serial hardware.
//  103  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function printPending
        THUMB
//  104 void printPending(void) {
printPending:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        SUB      SP,SP,#+8
          CFI CFA R13+16
//  105 	if (consoleInBinaryMode)
        LDR.N    R0,??DataTable3_4
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??printPending_0
//  106 		return;
//  107 	int actualOutputBufferSize;
//  108 	char *output = swapOutputBuffers(&actualOutputBufferSize);
??printPending_1:
        ADD      R0,SP,#+0
          CFI FunCall _Z17swapOutputBuffersPi
        BL       _Z17swapOutputBuffersPi
        MOVS     R4,R0
//  109 
//  110 	if (actualOutputBufferSize > 0) {
        LDR      R0,[SP, #+0]
        CMP      R0,#+1
        BLT.N    ??printPending_2
//  111 		printWithLength(output);
        MOVS     R0,R4
          CFI FunCall printWithLength
        BL       printWithLength
//  112 	}
//  113 }
??printPending_2:
??printPending_0:
        POP      {R0,R1,R4,PC}    ;; return
          CFI EndBlock cfiBlock3
//  114 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z18initLoggingCentralv
          CFI NoCalls
        THUMB
//  115 void initLoggingCentral(void) {
//  116 	pendingBuffers0[0] = 0;
_Z18initLoggingCentralv:
        LDR.N    R0,??DataTable3_5
        MOVS     R1,#+0
        STRB     R1,[R0, #+0]
//  117 	pendingBuffers1[0] = 0;
        LDR.N    R0,??DataTable3_6
        MOVS     R1,#+0
        STRB     R1,[R0, #+0]
//  118 	accumulationBuffer = pendingBuffers0;
        LDR.N    R0,??DataTable3_1
        LDR.N    R1,??DataTable3_5
        STR      R1,[R0, #+0]
//  119 	outputBuffer = pendingBuffers1;
        LDR.N    R0,??DataTable3_2
        LDR.N    R1,??DataTable3_6
        STR      R1,[R0, #+0]
//  120 	accumulatedSize = 0;
        LDR.N    R0,??DataTable3
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  121 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3:
        DC32     accumulatedSize

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_1:
        DC32     accumulationBuffer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_2:
        DC32     outputBuffer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_3:
        DC32     `?<Constant "out constr">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_4:
        DC32     consoleInBinaryMode

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_5:
        DC32     pendingBuffers0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_6:
        DC32     pendingBuffers1

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "out constr">`:
        DATA
        DC8 "out constr"
        DC8 0

        END
//  122 
//  123 #endif /* EFI_UNIT_TEST */
// 
// 8 012 bytes in section .bss
// 8 000 bytes in section .ccm
//    12 bytes in section .rodata
//   292 bytes in section .text
// 
//    270 bytes of CODE  memory (+ 22 bytes shared)
//     12 bytes of CONST memory
// 16 012 bytes of DATA  memory
//
//Errors: none
//Warnings: none
