///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:01 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\console\binary\ /
//                       tunerstudio_io.cpp                                   /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\console\binary\ /
//                       tunerstudio_io.cpp -lCN F:\stuff\rusefi_sourceforge\ /
//                       firmware\iar\Debug\List\ -lA                         /
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
//                       tunerstudio_io.s                                     /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME tunerstudio_io

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN SD3
        EXTERN SDU1
        EXTERN _Z12mySetPadModePKcP12GPIO_TypeDefjj
        EXTERN _Z16isSerialOverUartv
        EXTERN __aeabi_memcpy
        EXTERN boardConfiguration
        EXTERN crc32
        EXTERN firmwareError
        EXTERN getRemainingStack
        EXTERN print
        EXTERN rlist
        EXTERN scheduleMsg
        EXTERN sdStart
        EXTERN tsLogger
        EXTERN usb_serial_start

        PUBLIC _Z11startTsPortv
        PUBLIC _Z14tsSendResponseP12ts_channel_s20ts_response_format_ePKhi
        PUBLIC _Z17getTsSerialDevicev
        PUBLIC _Z20tunerStudioWriteDataP12ts_channel_sPKhi
        PUBLIC _Z25tunerStudioWriteCrcPacketP12ts_channel_shPKvt
        PUBLIC memcpy
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\console\binary\tunerstudio_io.cpp
//    1 /**
//    2  * @file	tunerstudio_io.cpp
//    3  *
//    4  * @date Mar 8, 2015
//    5  * @author Andrey Belomutskiy, (c) 2012-2014
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
//    9 #include "tunerstudio_io.h"
//   10 #include "console_io.h"
//   11 #include "engine.h"
//   12 
//   13 EXTERN_ENGINE;
//   14 
//   15 extern LoggingWithStorage tsLogger;
//   16 
//   17 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//   18 #include "pin_repository.h"
//   19 #include "usbconsole.h"
//   20 #include "map_averaging.h"
//   21 extern SerialUSBDriver SDU1;
//   22 #define CONSOLE_DEVICE &SDU1
//   23 
//   24 #define TS_SERIAL_UART_DEVICE &SD3
//   25 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   26 static SerialConfig tsSerialConfig = { 0, 0, USART_CR2_STOP1_BITS | USART_CR2_LINEN, 0 };
tsSerialConfig:
        DATA
        DC32 0
        DC16 0, 16384, 0
        DC8 0, 0
//   27 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z11startTsPortv
        THUMB
//   28 void startTsPort(void) {
_Z11startTsPortv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   29 	if (isSerialOverUart()) {
          CFI FunCall _Z16isSerialOverUartv
        BL       _Z16isSerialOverUartv
        CMP      R0,#+0
        BEQ.N    ??startTsPort_0
//   30 		print("TunerStudio over USB serial");
        LDR.N    R0,??DataTable2
          CFI FunCall print
        BL       print
//   31 		/**
//   32 		 * This method contains a long delay, that's the reason why this is not done on the main thread
//   33 		 */
//   34 		usb_serial_start();
          CFI FunCall usb_serial_start
        BL       usb_serial_start
        B.N      ??startTsPort_1
//   35 	} else if (boardConfiguration->useSerialPort) {
??startTsPort_0:
        LDR.N    R0,??DataTable2_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+385]
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??startTsPort_1
//   36 
//   37 		print("TunerStudio over USART");
        LDR.N    R0,??DataTable2_2
          CFI FunCall print
        BL       print
//   38 		mySetPadMode("tunerstudio rx", TS_SERIAL_RX_PORT, TS_SERIAL_RX_PIN, PAL_MODE_ALTERNATE(TS_SERIAL_AF));
        MOVW     R3,#+898
        MOVS     R2,#+11
        LDR.N    R1,??DataTable2_3  ;; 0x40020800
        LDR.N    R0,??DataTable2_4
          CFI FunCall _Z12mySetPadModePKcP12GPIO_TypeDefjj
        BL       _Z12mySetPadModePKcP12GPIO_TypeDefjj
//   39 		mySetPadMode("tunerstudio tx", TS_SERIAL_TX_PORT, TS_SERIAL_TX_PIN, PAL_MODE_ALTERNATE(TS_SERIAL_AF));
        MOVW     R3,#+898
        MOVS     R2,#+10
        LDR.N    R1,??DataTable2_3  ;; 0x40020800
        LDR.N    R0,??DataTable2_5
          CFI FunCall _Z12mySetPadModePKcP12GPIO_TypeDefjj
        BL       _Z12mySetPadModePKcP12GPIO_TypeDefjj
//   40 
//   41 		tsSerialConfig.speed = boardConfiguration->tunerStudioSerialSpeed;
        LDR.N    R0,??DataTable2_1
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+332]
        LDR.N    R1,??DataTable2_6
        STR      R0,[R1, #+0]
//   42 
//   43 		sdStart(TS_SERIAL_UART_DEVICE, &tsSerialConfig);
        LDR.N    R1,??DataTable2_6
        LDR.N    R0,??DataTable2_7
          CFI FunCall sdStart
        BL       sdStart
//   44 	}
//   45 }
??startTsPort_1:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock1
//   46 
//   47 #endif /* EFI_PROD_CODE */
//   48 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z17getTsSerialDevicev
        THUMB
//   49 BaseChannel * getTsSerialDevice(void) {
_Z17getTsSerialDevicev:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   50 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//   51 	if (isSerialOverUart()) {
          CFI FunCall _Z16isSerialOverUartv
        BL       _Z16isSerialOverUartv
        CMP      R0,#+0
        BEQ.N    ??getTsSerialDevice_0
//   52 		// if console uses UART then TS uses USB
//   53 		return (BaseChannel *) &SDU1;
        LDR.N    R0,??DataTable2_8
        B.N      ??getTsSerialDevice_1
//   54 	} else {
//   55 		return (BaseChannel *) TS_SERIAL_UART_DEVICE;
??getTsSerialDevice_0:
        LDR.N    R0,??DataTable2_7
??getTsSerialDevice_1:
        POP      {R1,PC}          ;; return
          CFI EndBlock cfiBlock2
//   56 	}
//   57 #else
//   58 	return (BaseChannel *) TS_SIMULATOR_PORT;
//   59 #endif
//   60 }
//   61 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z20tunerStudioWriteDataP12ts_channel_sPKhi
        THUMB
//   62 void tunerStudioWriteData(ts_channel_s *tsChannel, const uint8_t * buffer, int size) {
_Z20tunerStudioWriteDataP12ts_channel_sPKhi:
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
//   63         efiAssertVoid(getRemainingStack(chThdSelf()) > 64, "tunerStudioWriteData");
        LDR.N    R0,??DataTable2_9
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+65
        BGE.N    ??tunerStudioWriteData_0
        LDR.N    R0,??DataTable2_10
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??tunerStudioWriteData_1
//   64 	int transferred = chSequentialStreamWrite(tsChannel->channel, buffer, size);
??tunerStudioWriteData_0:
        MOVS     R2,R6
        MOVS     R1,R5
        LDR      R0,[R4, #+0]
        LDR      R3,[R4, #+0]
        LDR      R3,[R3, #+0]
        LDR      R3,[R3, #+0]
          CFI FunCall
        BLX      R3
        MOVS     R7,R0
//   65 	if (transferred != size) {
        CMP      R7,R6
        BEQ.N    ??tunerStudioWriteData_2
//   66 		scheduleMsg(&tsLogger, "!!! NOT ACCEPTED %d out of %d !!!", transferred, size);
        MOVS     R3,R6
        MOVS     R2,R7
        LDR.N    R1,??DataTable2_11
        LDR.N    R0,??DataTable2_12
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   67 	}
//   68 }
??tunerStudioWriteData_2:
??tunerStudioWriteData_1:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2:
        DC32     `?<Constant "TunerStudio over USB ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_1:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_2:
        DC32     `?<Constant "TunerStudio over USART">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_3:
        DC32     0x40020800

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_4:
        DC32     `?<Constant "tunerstudio rx">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_5:
        DC32     `?<Constant "tunerstudio tx">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_6:
        DC32     tsSerialConfig

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_7:
        DC32     SD3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_8:
        DC32     SDU1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_9:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_10:
        DC32     `?<Constant "tunerStudioWriteData">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_11:
        DC32     `?<Constant "!!! NOT ACCEPTED %d o...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_12:
        DC32     tsLogger
//   69 
//   70 /**
//   71  * Adds size to the beginning of a packet and a crc32 at the end. Then send the packet.
//   72  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z25tunerStudioWriteCrcPacketP12ts_channel_shPKvt
        THUMB
//   73 void tunerStudioWriteCrcPacket(ts_channel_s *tsChannel, const uint8_t command, const void *buf, const uint16_t size) {
_Z25tunerStudioWriteCrcPacketP12ts_channel_shPKvt:
        PUSH     {R3-R9,LR}
          CFI R14 Frame(CFA, -4)
          CFI R9 Frame(CFA, -8)
          CFI R8 Frame(CFA, -12)
          CFI R7 Frame(CFA, -16)
          CFI R6 Frame(CFA, -20)
          CFI R5 Frame(CFA, -24)
          CFI R4 Frame(CFA, -28)
          CFI CFA R13+32
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
//   74 
//   75 	uint8_t *writeBuffer = tsChannel->writeBuffer;
        LDR      R8,[R4, #+4]
//   76 
//   77 	// todo: max size validation
//   78 	*(uint16_t *) writeBuffer = SWAP_UINT16(size + 1);   // packet size including command
        ADDS     R0,R7,#+1
        UXTH     R7,R7            ;; ZeroExt  R7,R7,#+16,#+16
        ADDS     R1,R7,#+1
        ASRS     R1,R1,#+8
        ORRS     R0,R1,R0, LSL #+8
        STRH     R0,[R8, #+0]
//   79 	*(uint8_t *) (writeBuffer + 2) = command;
        STRB     R5,[R8, #+2]
//   80 	if (size != 0)
        UXTH     R7,R7            ;; ZeroExt  R7,R7,#+16,#+16
        CMP      R7,#+0
        BEQ.N    ??tunerStudioWriteCrcPacket_0
//   81 		memcpy(writeBuffer + 3, buf, size);
        UXTH     R7,R7            ;; ZeroExt  R7,R7,#+16,#+16
        MOVS     R2,R7
        MOVS     R1,R6
        ADDS     R0,R8,#+3
          CFI FunCall memcpy
        BL       memcpy
//   82 	// CRC on whole packet
//   83 	uint32_t crc = crc32((void *) (writeBuffer + 2), (uint32_t) (size + 1));
??tunerStudioWriteCrcPacket_0:
        UXTH     R7,R7            ;; ZeroExt  R7,R7,#+16,#+16
        ADDS     R1,R7,#+1
        ADDS     R0,R8,#+2
          CFI FunCall crc32
        BL       crc32
        MOV      R9,R0
//   84 	*(uint32_t *) (writeBuffer + 2 + 1 + size) = SWAP_UINT32(crc);
        LSLS     R0,R9,#+8
        ANDS     R0,R0,#0xFF0000
        ORRS     R0,R0,R9, LSR #+24
        LSRS     R1,R9,#+8
        ANDS     R1,R1,#0xFF00
        ORRS     R0,R1,R0
        ORRS     R0,R0,R9, LSL #+24
        UXTH     R7,R7            ;; ZeroExt  R7,R7,#+16,#+16
        ADDS     R1,R7,R8
        STR      R0,[R1, #+3]
//   85 
//   86 //	scheduleMsg(logger, "TunerStudio: CRC command %x size %d", command, size);
//   87 
//   88 	tunerStudioWriteData(tsChannel, writeBuffer, size + 2 + 1 + 4);      // with size, command and CRC
        UXTH     R7,R7            ;; ZeroExt  R7,R7,#+16,#+16
        ADDS     R2,R7,#+7
        MOV      R1,R8
        MOVS     R0,R4
          CFI FunCall _Z20tunerStudioWriteDataP12ts_channel_sPKhi
        BL       _Z20tunerStudioWriteDataP12ts_channel_sPKhi
//   89 }
        POP      {R0,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock4
//   90 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z14tsSendResponseP12ts_channel_s20ts_response_format_ePKhi
        THUMB
//   91 void tsSendResponse(ts_channel_s *tsChannel, ts_response_format_e mode, const uint8_t * buffer, int size) {
_Z14tsSendResponseP12ts_channel_s20ts_response_format_ePKhi:
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
//   92 	if (mode == TS_CRC) {
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+1
        BNE.N    ??tsSendResponse_0
//   93 		tunerStudioWriteCrcPacket(tsChannel, TS_RESPONSE_OK, buffer, size);
        MOVS     R3,R7
        UXTH     R3,R3            ;; ZeroExt  R3,R3,#+16,#+16
        MOVS     R2,R6
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall _Z25tunerStudioWriteCrcPacketP12ts_channel_shPKvt
        BL       _Z25tunerStudioWriteCrcPacketP12ts_channel_shPKvt
        B.N      ??tsSendResponse_1
//   94 	} else {
//   95 		if (size > 0)
??tsSendResponse_0:
        CMP      R7,#+1
        BLT.N    ??tsSendResponse_1
//   96 			tunerStudioWriteData(tsChannel, buffer, size);
        MOVS     R2,R7
        MOVS     R1,R6
        MOVS     R0,R4
          CFI FunCall _Z20tunerStudioWriteDataP12ts_channel_sPKhi
        BL       _Z20tunerStudioWriteDataP12ts_channel_sPKhi
//   97 	}
//   98 }
??tsSendResponse_1:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock5

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TunerStudio over USB ...">`:
        DATA
        DC8 "TunerStudio over USB serial"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TunerStudio over USART">`:
        DATA
        DC8 "TunerStudio over USART"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "tunerstudio rx">`:
        DATA
        DC8 "tunerstudio rx"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "tunerstudio tx">`:
        DATA
        DC8 "tunerstudio tx"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "tunerStudioWriteData">`:
        DATA
        DC8 "tunerStudioWriteData"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "!!! NOT ACCEPTED %d o...">`:
        DATA
        DC8 "!!! NOT ACCEPTED %d out of %d !!!"
        DC8 0, 0

        END
//   99 
// 
//  12 bytes in section .data
// 144 bytes in section .rodata
// 418 bytes in section .text
// 
// 396 bytes of CODE  memory (+ 22 bytes shared)
// 144 bytes of CONST memory
//  12 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
