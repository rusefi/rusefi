///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:44 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\console\console /
//                       _io.cpp                                              /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\console\console /
//                       _io.cpp -lCN F:\stuff\rusefi_sourceforge\firmware\ia /
//                       r\Debug\List\ -lA F:\stuff\rusefi_sourceforge\firmwa /
//                       re\iar\Debug\List\ -o F:\stuff\rusefi_sourceforge\fi /
//                       rmware\iar\Debug\Obj\ --no_cse --no_unroll           /
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
//                       console_io.s                                         /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME console_io

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN SD3
        EXTERN SDU1
        EXTERN _Z21runBinaryProtocolLoopP12ts_channel_sb
        EXTERN _pal_lld_setgroupmode
        EXTERN addConsoleAction
        EXTERN chEvtGetAndClearFlagsI
        EXTERN chEvtRegisterMask
        EXTERN chThdCreateStatic
        EXTERN chThdSleep
        EXTERN dbg_check_lock
        EXTERN dbg_check_lock_from_isr
        EXTERN dbg_check_unlock
        EXTERN dbg_check_unlock_from_isr
        EXTERN dbg_isr_cnt
        EXTERN dbg_lock_cnt
        EXTERN efiTrim
        EXTERN firmwareError
        EXTERN getRemainingStack
        EXTERN is_usb_serial_ready
        EXTERN rlist
        EXTERN scheduleMsg
        EXTERN sdStart
        EXTERN usb_serial_start

        PUBLIC _Z12startConsoleP7LoggingPFvPcE
        PUBLIC _Z14consolePutChari
        PUBLIC _Z14isConsoleReadyv
        PUBLIC _Z16isSerialOverUartv
        PUBLIC _Z17getConsoleChannelv
        PUBLIC _Z19consoleOutputBufferPKhi
        PUBLIC binaryConsole
        PUBLIC buffer
        PUBLIC consoleInBinaryMode
        PUBLIC console_line_callback
        PUBLIC lastWriteActual
        PUBLIC lastWriteSize
        PUBLIC lockAnyContext
        PUBLIC lockOutputBuffer
        PUBLIC unlockAnyContext
        PUBLIC unlockOutputBuffer
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\console\console_io.cpp
//    1 /**
//    2  * @file	console_io.c
//    3  *
//    4  * @date Dec 29, 2012
//    5  * @author Andrey Belomutskiy, (c) 2012-2015
//    6  *
//    7  * This file is part of rusEfi - see http://rusefi.com
//    8  *
//    9  * rusEfi is free software; you can redistribute it and/or modify it under the terms of
//   10  * the GNU General Public License as published by the Free Software Foundation; either
//   11  * version 3 of the License, or (at your option) any later version.
//   12  *
//   13  * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
//   14  * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   15  * GNU General Public License for more details.
//   16  *
//   17  * You should have received a copy of the GNU General Public License along with this program.
//   18  * If not, see <http://www.gnu.org/licenses/>.
//   19  */
//   20 
//   21 #include "main.h"
//   22 #include "console_io.h"
//   23 #include "rfiutil.h"
//   24 #include "tunerstudio.h"
//   25 
//   26 #if HAL_USE_SERIAL_USB || defined(__DOXYGEN__)
//   27 #include "usbcfg.h"
//   28 #include "usbconsole.h"
//   29 extern SerialUSBDriver SDU1;
//   30 #endif
//   31 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   32 int lastWriteSize;
lastWriteSize:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   33 int lastWriteActual;
lastWriteActual:
        DS8 4
//   34 

        SECTION `.bss`:DATA:REORDER:NOROOT(0)
//   35 static bool isSerialConsoleStarted = false;
isSerialConsoleStarted:
        DS8 1
//   36 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   37 static EventListener consoleEventListener;
consoleEventListener:
        DS8 16
//   38 
//   39 /**
//   40  * @brief   Reads a whole line from the input channel.
//   41  *
//   42  * @param[in] chp       pointer to a @p BaseChannel object
//   43  * @param[in] line      pointer to the line buffer
//   44  * @param[in] size      buffer maximum length
//   45  * @return              The operation status.
//   46  * @retval TRUE         the channel was reset or CTRL-D pressed.
//   47  * @retval FALSE        operation successful.
//   48  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z14getConsoleLineP20BaseSequentialStreamPcj
        THUMB
//   49 static bool getConsoleLine(BaseSequentialStream *chp, char *line, unsigned size) {
_Z14getConsoleLineP20BaseSequentialStreamPcj:
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
//   50 	char *p = line;
        MOVS     R7,R5
//   51 
//   52 	while (true) {
//   53 		if (!isConsoleReady()) {
??getConsoleLine_0:
          CFI FunCall _Z14isConsoleReadyv
        BL       _Z14isConsoleReadyv
        CMP      R0,#+0
        BNE.N    ??getConsoleLine_1
//   54 			// we better do not read from USB serial before it is ready
//   55 			chThdSleepMilliseconds(10);
        MOVS     R0,#+10
          CFI FunCall chThdSleep
        BL       chThdSleep
//   56 			continue;
        B.N      ??getConsoleLine_0
//   57 		}
//   58 
//   59 		short c = (short) chSequentialStreamGet(chp);
??getConsoleLine_1:
        MOVS     R0,R4
        LDR      R1,[R4, #+0]
        LDR      R1,[R1, #+12]
          CFI FunCall
        BLX      R1
        MOV      R8,R0
//   60 
//   61 		if (isSerialOverUart()) {
          CFI FunCall _Z16isSerialOverUartv
        BL       _Z16isSerialOverUartv
        CMP      R0,#+0
        BEQ.N    ??getConsoleLine_2
//   62 			uint32_t flags;
//   63 			chSysLock()
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//   64 			;
//   65 
//   66 			flags = chEvtGetAndClearFlagsI(&consoleEventListener);
        LDR.N    R0,??DataTable9_1
          CFI FunCall chEvtGetAndClearFlagsI
        BL       chEvtGetAndClearFlagsI
        MOV      R9,R0
//   67 			chSysUnlock()
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//   68 			;
//   69 
//   70 			if (flags & SD_OVERRUN_ERROR) {
//   71 //				firmwareError("serial overrun");
//   72 			}
//   73 
//   74 		}
//   75 
//   76 #if EFI_UART_ECHO_TEST_MODE
//   77 		/**
//   78 		 * That's test code - let's test connectivity
//   79 		 */
//   80 		consolePutChar((uint8_t) c);
//   81 		continue;
//   82 #endif
//   83 
//   84 		if (c < 0 || c == 4) {
??getConsoleLine_2:
        SXTH     R8,R8            ;; SignExt  R8,R8,#+16,#+16
        CMP      R8,#+0
        BMI.N    ??getConsoleLine_3
        SXTH     R8,R8            ;; SignExt  R8,R8,#+16,#+16
        CMP      R8,#+4
        BNE.N    ??getConsoleLine_4
//   85 			return true;
??getConsoleLine_3:
        MOVS     R0,#+1
        B.N      ??getConsoleLine_5
//   86 		}
//   87 		if (c == 8) {
??getConsoleLine_4:
        SXTH     R8,R8            ;; SignExt  R8,R8,#+16,#+16
        CMP      R8,#+8
        BNE.N    ??getConsoleLine_6
//   88 			if (p != line) {
        CMP      R7,R5
        BEQ.N    ??getConsoleLine_7
//   89 				// backspace
//   90 				consolePutChar((uint8_t) c);
        MOV      R0,R8
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z14consolePutChari
        BL       _Z14consolePutChari
//   91 				consolePutChar(0x20);
        MOVS     R0,#+32
          CFI FunCall _Z14consolePutChari
        BL       _Z14consolePutChari
//   92 				consolePutChar((uint8_t) c);
        MOV      R0,R8
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z14consolePutChari
        BL       _Z14consolePutChari
//   93 				p--;
        SUBS     R7,R7,#+1
//   94 			}
//   95 			continue;
??getConsoleLine_7:
        B.N      ??getConsoleLine_0
//   96 		}
//   97 		if (c == '\r') {
??getConsoleLine_6:
        SXTH     R8,R8            ;; SignExt  R8,R8,#+16,#+16
        CMP      R8,#+13
        BNE.N    ??getConsoleLine_8
//   98 			consolePutChar('\r');
        MOVS     R0,#+13
          CFI FunCall _Z14consolePutChari
        BL       _Z14consolePutChari
//   99 			consolePutChar('\n');
        MOVS     R0,#+10
          CFI FunCall _Z14consolePutChari
        BL       _Z14consolePutChari
//  100 			*p = 0;
        MOVS     R0,#+0
        STRB     R0,[R7, #+0]
//  101 			return false;
        MOVS     R0,#+0
        B.N      ??getConsoleLine_5
//  102 		}
//  103 		if (c == '\n') {
??getConsoleLine_8:
        SXTH     R8,R8            ;; SignExt  R8,R8,#+16,#+16
        CMP      R8,#+10
        BNE.N    ??getConsoleLine_9
//  104 			consolePutChar('\n');
        MOVS     R0,#+10
          CFI FunCall _Z14consolePutChari
        BL       _Z14consolePutChari
//  105 			*p = 0;
        MOVS     R0,#+0
        STRB     R0,[R7, #+0]
//  106 			return false;
        MOVS     R0,#+0
        B.N      ??getConsoleLine_5
//  107 		}
//  108 		if (c < 0x20) {
??getConsoleLine_9:
        SXTH     R8,R8            ;; SignExt  R8,R8,#+16,#+16
        CMP      R8,#+32
        BLT.N    ??getConsoleLine_0
//  109 			continue;
//  110 		}
//  111 		if (p < line + size - 1) {
??getConsoleLine_10:
        ADDS     R0,R6,R5
        SUBS     R0,R0,#+1
        CMP      R7,R0
        BCS.N    ??getConsoleLine_0
//  112 			consolePutChar((uint8_t) c);
        MOV      R0,R8
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z14consolePutChari
        BL       _Z14consolePutChari
//  113 			*p++ = (char) c;
        STRB     R8,[R7, #+0]
        ADDS     R7,R7,#+1
        B.N      ??getConsoleLine_0
//  114 		}
//  115 	}
??getConsoleLine_5:
        POP      {R1,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock0
//  116 }
//  117 
//  118 // todo: this is ugly as hell!

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  119 static char consoleInput[] = "                                                                              ";
consoleInput:
        DATA
        DC8 20H, 20H, 20H, 20H, 20H, 20H, 20H, 20H
        DC8 20H, 20H, 20H, 20H, 20H, 20H, 20H, 20H
        DC8 20H, 20H, 20H, 20H, 20H, 20H, 20H, 20H
        DC8 20H, 20H, 20H, 20H, 20H, 20H, 20H, 20H
        DC8 20H, 20H, 20H, 20H, 20H, 20H, 20H, 20H
        DC8 20H, 20H, 20H, 20H, 20H, 20H, 20H, 20H
        DC8 20H, 20H, 20H, 20H, 20H, 20H, 20H, 20H
        DC8 20H, 20H, 20H, 20H, 20H, 20H, 20H, 20H
        DC8 20H, 20H, 20H, 20H, 20H, 20H, 20H, 20H
        DC8 20H, 20H, 20H, 20H, 20H, 20H, 0
        DC8 0
//  120 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  121 CommandHandler console_line_callback;
console_line_callback:
        DS8 4
//  122 

        SECTION `.bss`:DATA:REORDER:NOROOT(0)
//  123 static bool is_serial_over_uart;
is_serial_over_uart:
        DS8 1
//  124 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z16isSerialOverUartv
          CFI NoCalls
        THUMB
//  125 bool isSerialOverUart(void) {
//  126 	return is_serial_over_uart;
_Z16isSerialOverUartv:
        LDR.N    R0,??DataTable9_2
        LDRB     R0,[R0, #+0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//  127 }
//  128 
//  129 #if (defined(EFI_CONSOLE_UART_DEVICE) && ! EFI_SIMULATOR ) || defined(__DOXYGEN__)

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  130 static SerialConfig serialConfig = { SERIAL_SPEED, 0, USART_CR2_STOP1_BITS | USART_CR2_LINEN, 0 };
serialConfig:
        DATA
        DC32 115200
        DC16 0, 16384, 0
        DC8 0, 0
//  131 #endif
//  132 
//  133 #if EFI_PROD_CODE || EFI_EGT || defined(__DOXYGEN__)
//  134 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z17getConsoleChannelv
        THUMB
//  135 SerialDriver * getConsoleChannel(void) {
_Z17getConsoleChannelv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  136 #if defined(EFI_CONSOLE_UART_DEVICE) || defined(__DOXYGEN__)
//  137 	if (isSerialOverUart()) {
          CFI FunCall _Z16isSerialOverUartv
        BL       _Z16isSerialOverUartv
        CMP      R0,#+0
        BEQ.N    ??getConsoleChannel_0
//  138 		return (SerialDriver *) EFI_CONSOLE_UART_DEVICE;
        LDR.N    R0,??DataTable9_3
        B.N      ??getConsoleChannel_1
//  139 	}
//  140 #endif /* EFI_CONSOLE_UART_DEVICE */
//  141 
//  142 #if HAL_USE_SERIAL_USB || defined(__DOXYGEN__)
//  143 	return (SerialDriver *) &SDU1;
??getConsoleChannel_0:
        LDR.N    R0,??DataTable9_4
??getConsoleChannel_1:
        POP      {R1,PC}          ;; return
          CFI EndBlock cfiBlock2
//  144 #else
//  145 	return NULL;
//  146 #endif
//  147 }
//  148 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z14isConsoleReadyv
        THUMB
//  149 bool isConsoleReady(void) {
_Z14isConsoleReadyv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  150 	if (isSerialOverUart()) {
          CFI FunCall _Z16isSerialOverUartv
        BL       _Z16isSerialOverUartv
        CMP      R0,#+0
        BEQ.N    ??isConsoleReady_0
//  151 		return isSerialConsoleStarted;
        LDR.N    R0,??DataTable9_5
        LDRB     R0,[R0, #+0]
        B.N      ??isConsoleReady_1
//  152 	} else {
//  153 		return is_usb_serial_ready();
??isConsoleReady_0:
          CFI FunCall is_usb_serial_ready
        BL       is_usb_serial_ready
??isConsoleReady_1:
        POP      {R1,PC}          ;; return
          CFI EndBlock cfiBlock3
//  154 	}
//  155 }
//  156 #endif /* EFI_PROD_CODE || EFI_EGT */
//  157 

        SECTION `.bss`:DATA:REORDER:NOROOT(0)
//  158 bool_t consoleInBinaryMode = false;
consoleInBinaryMode:
        DS8 1
//  159 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  160 ts_channel_s binaryConsole;
binaryConsole:
        DS8 308
//  161 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  162 uint8_t buffer[DL_OUTPUT_BUFFER];
buffer:
        DS8 8000
//  163 

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//  164 static THD_WORKING_AREA(consoleThreadStack, 2 * UTILITY_THREAD_STACK_SIZE);
consoleThreadStack:
        DS8 1112

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z29consoleThreadThreadEntryPointPv
        THUMB
//  165 static msg_t consoleThreadThreadEntryPoint(void *arg) {
_Z29consoleThreadThreadEntryPointPv:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  166 	(void) arg;
//  167 	chRegSetThreadName("console thread");
        LDR.N    R0,??DataTable9_6
        LDR      R0,[R0, #+24]
        LDR.N    R1,??DataTable9_7
        STR      R1,[R0, #+24]
//  168 
//  169 #if EFI_PROD_CODE
//  170 	if (!isSerialOverUart()) {
          CFI FunCall _Z16isSerialOverUartv
        BL       _Z16isSerialOverUartv
        CMP      R0,#+0
        BNE.N    ??consoleThreadThreadEntryPoint_0
//  171 		/**
//  172 		 * This method contains a long delay, that's the reason why this is not done on the main thread
//  173 		 */
//  174 		usb_serial_start();
          CFI FunCall usb_serial_start
        BL       usb_serial_start
//  175 	}
//  176 #endif /* EFI_PROD_CODE */
//  177 
//  178 	binaryConsole.channel = (BaseChannel *) getConsoleChannel();
??consoleThreadThreadEntryPoint_0:
          CFI FunCall _Z17getConsoleChannelv
        BL       _Z17getConsoleChannelv
        LDR.N    R1,??DataTable9_8
        STR      R0,[R1, #+0]
//  179 	// todo: clean this spot!
//  180 	binaryConsole.writeBuffer = buffer;
        LDR.N    R0,??DataTable9_8
        LDR.N    R1,??DataTable9_9
        STR      R1,[R0, #+4]
//  181 
//  182 	while (true) {
//  183 		efiAssert(getRemainingStack(chThdSelf()) > 256, "lowstck#9e", 0);
??consoleThreadThreadEntryPoint_1:
        LDR.N    R0,??DataTable9_6
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+256
        BGT.N    ??consoleThreadThreadEntryPoint_2
        LDR.N    R0,??DataTable9_10
          CFI FunCall firmwareError
        BL       firmwareError
        MOVS     R0,#+0
        B.N      ??consoleThreadThreadEntryPoint_3
//  184 		bool end = getConsoleLine((BaseSequentialStream*) getConsoleChannel(), consoleInput, sizeof(consoleInput));
??consoleThreadThreadEntryPoint_2:
          CFI FunCall _Z17getConsoleChannelv
        BL       _Z17getConsoleChannelv
        MOVS     R2,#+79
        LDR.N    R1,??DataTable9_11
          CFI FunCall _Z14getConsoleLineP20BaseSequentialStreamPcj
        BL       _Z14getConsoleLineP20BaseSequentialStreamPcj
        MOVS     R5,R0
//  185 		if (end) {
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BNE.N    ??consoleThreadThreadEntryPoint_1
//  186 			// firmware simulator is the only case when this happens
//  187 			continue;
//  188 		}
//  189 
//  190 		char *trimmed = efiTrim(consoleInput);
??consoleThreadThreadEntryPoint_4:
        LDR.N    R0,??DataTable9_11
          CFI FunCall efiTrim
        BL       efiTrim
        MOVS     R6,R0
//  191 
//  192 		(console_line_callback)(trimmed);
        MOVS     R0,R6
        LDR.N    R1,??DataTable9_12
        LDR      R1,[R1, #+0]
          CFI FunCall
        BLX      R1
//  193 
//  194 		if (consoleInBinaryMode) {
        LDR.N    R0,??DataTable9_13
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??consoleThreadThreadEntryPoint_1
//  195 			// switch to binary protocol
//  196 			runBinaryProtocolLoop(&binaryConsole, true);
        MOVS     R1,#+1
        LDR.N    R0,??DataTable9_8
          CFI FunCall _Z21runBinaryProtocolLoopP12ts_channel_sb
        BL       _Z21runBinaryProtocolLoopP12ts_channel_sb
        B.N      ??consoleThreadThreadEntryPoint_1
//  197 		}
//  198 	}
??consoleThreadThreadEntryPoint_3:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock4
//  199 #if defined __GNUC__
//  200 	return false;
//  201 #endif        
//  202 }
//  203 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z14consolePutChari
        THUMB
//  204 void consolePutChar(int x) {
_Z14consolePutChari:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  205 	chSequentialStreamPut(getConsoleChannel(), (uint8_t )(x));
          CFI FunCall _Z17getConsoleChannelv
        BL       _Z17getConsoleChannelv
        MOVS     R5,R0
          CFI FunCall _Z17getConsoleChannelv
        BL       _Z17getConsoleChannelv
        MOVS     R2,R0
        MOVS     R1,R4
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R5
        LDR      R2,[R2, #+0]
        LDR      R2,[R2, #+8]
          CFI FunCall
        BLX      R2
//  206 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock5
//  207 
//  208 // 10 seconds
//  209 #define CONSOLE_WRITE_TIMEOUT 10000
//  210 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z19consoleOutputBufferPKhi
        THUMB
//  211 void consoleOutputBuffer(const uint8_t *buf, int size) {
_Z19consoleOutputBufferPKhi:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  212 	lastWriteSize = size;
        LDR.N    R0,??DataTable9_14
        STR      R5,[R0, #+0]
//  213 #if !EFI_UART_ECHO_TEST_MODE
//  214 	lastWriteActual = chnWriteTimeout(getConsoleChannel(), buf, size, CONSOLE_WRITE_TIMEOUT);
          CFI FunCall _Z17getConsoleChannelv
        BL       _Z17getConsoleChannelv
        MOVS     R6,R0
          CFI FunCall _Z17getConsoleChannelv
        BL       _Z17getConsoleChannelv
        MOVS     R7,R0
        MOVW     R3,#+10000
        MOVS     R2,R5
        MOVS     R1,R4
        MOVS     R0,R6
        LDR      R6,[R7, #+0]
        LDR      R6,[R6, #+24]
          CFI FunCall
        BLX      R6
        LDR.N    R1,??DataTable9_15
        STR      R0,[R1, #+0]
//  215 //	if (r != size)
//  216 //		firmwareError("Partial console write");
//  217 #endif /* EFI_UART_ECHO_TEST_MODE */
//  218 }
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock6
//  219 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  220 static Logging *logger;
logger:
        DS8 4
//  221 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z22switchToBinaryProtocolv
        THUMB
//  222 static void switchToBinaryProtocol(void) {
_Z22switchToBinaryProtocolv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  223 	scheduleMsg(logger, "switching to binary protocol");
        LDR.N    R1,??DataTable9_16
        LDR.N    R0,??DataTable9_17
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  224 	consoleInBinaryMode = true;
        LDR.N    R0,??DataTable9_13
        MOVS     R1,#+1
        STRB     R1,[R0, #+0]
//  225 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock7
//  226 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z12startConsoleP7LoggingPFvPcE
        THUMB
//  227 void startConsole(Logging *sharedLogger, CommandHandler console_line_callback_p) {
_Z12startConsoleP7LoggingPFvPcE:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  228 	logger = sharedLogger;
        LDR.N    R0,??DataTable9_17
        STR      R4,[R0, #+0]
//  229 	console_line_callback = console_line_callback_p;
        LDR.N    R0,??DataTable9_12
        STR      R5,[R0, #+0]
//  230 
//  231 #if (defined(EFI_CONSOLE_UART_DEVICE) && ! EFI_SIMULATOR) || defined(__DOXYGEN__)
//  232 
//  233 	palSetPadMode(CONSOLE_MODE_SWITCH_PORT, CONSOLE_MODE_SWITCH_PIN, PAL_MODE_INPUT_PULLUP);
        MOVS     R2,#+32
        MOVS     R1,#+2
        LDR.N    R0,??DataTable9_18  ;; 0x40020400
          CFI FunCall _pal_lld_setgroupmode
        BL       _pal_lld_setgroupmode
//  234 
//  235 	is_serial_over_uart = GET_CONSOLE_MODE_VALUE() == EFI_USE_UART_FOR_CONSOLE;
        LDR.N    R0,??DataTable9_19  ;; 0x40020410
        LDR      R0,[R0, #+0]
        UBFX     R0,R0,#+1,#+1
        ANDS     R0,R0,#0x1
        EORS     R0,R0,#0x1
        LDR.N    R1,??DataTable9_2
        STRB     R0,[R1, #+0]
//  236 
//  237 	if (isSerialOverUart()) {
          CFI FunCall _Z16isSerialOverUartv
        BL       _Z16isSerialOverUartv
        CMP      R0,#+0
        BEQ.N    ??startConsole_0
//  238 		/*
//  239 		 * Activates the serial using the driver default configuration (that's 38400)
//  240 		 * it is important to set 'NONE' as flow control! in terminal application on the PC
//  241 		 */
//  242 		sdStart(EFI_CONSOLE_UART_DEVICE, &serialConfig);
        LDR.N    R1,??DataTable9_20
        LDR.N    R0,??DataTable9_3
          CFI FunCall sdStart
        BL       sdStart
//  243 
//  244 // cannot use pin repository here because pin repository prints to console
//  245 		palSetPadMode(EFI_CONSOLE_RX_PORT, EFI_CONSOLE_RX_PIN, PAL_MODE_ALTERNATE(EFI_CONSOLE_AF));
        MOVW     R2,#+898
        MOV      R1,#+2048
        LDR.N    R0,??DataTable9_21  ;; 0x40020800
          CFI FunCall _pal_lld_setgroupmode
        BL       _pal_lld_setgroupmode
//  246 		palSetPadMode(EFI_CONSOLE_TX_PORT, EFI_CONSOLE_TX_PIN, PAL_MODE_ALTERNATE(EFI_CONSOLE_AF));
        MOVW     R2,#+898
        MOV      R1,#+1024
        LDR.N    R0,??DataTable9_21  ;; 0x40020800
          CFI FunCall _pal_lld_setgroupmode
        BL       _pal_lld_setgroupmode
//  247 
//  248 		isSerialConsoleStarted = true;
        LDR.N    R0,??DataTable9_5
        MOVS     R1,#+1
        STRB     R1,[R0, #+0]
//  249 
//  250 		chEvtRegisterMask((EventSource *) chnGetEventSource(EFI_CONSOLE_UART_DEVICE), &consoleEventListener, 1);
        MOVS     R2,#+1
        LDR.N    R1,??DataTable9_1
        LDR.N    R0,??DataTable9_22
          CFI FunCall chEvtRegisterMask
        BL       chEvtRegisterMask
//  251 	}
//  252 #else
//  253 	is_serial_over_uart = false;
//  254 #endif /* EFI_PROD_CODE */
//  255 
//  256 	chThdCreateStatic(consoleThreadStack, sizeof(consoleThreadStack), NORMALPRIO, consoleThreadThreadEntryPoint, NULL);
??startConsole_0:
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        LDR.N    R3,??DataTable9_23
        MOVS     R2,#+64
        MOV      R1,#+1112
        LDR.N    R0,??DataTable9_24
          CFI FunCall chThdCreateStatic
        BL       chThdCreateStatic
//  257 	addConsoleAction(SWITCH_TO_BINARY_COMMAND, switchToBinaryProtocol);
        LDR.N    R1,??DataTable9_25
        ADR.N    R0,??DataTable9  ;; "~"
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  258 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock8
//  259 
//  260 /**
//  261  * @return TRUE if already in locked context
//  262  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function lockAnyContext
        THUMB
//  263 bool lockAnyContext(void) {
lockAnyContext:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  264 	int alreadyLocked = isLocked();
        LDR.N    R0,??DataTable9_26
        LDR      R0,[R0, #+0]
        CMP      R0,#+1
        BLT.N    ??lockAnyContext_0
        MOVS     R4,#+1
        B.N      ??lockAnyContext_1
??lockAnyContext_0:
        MOVS     R4,#+0
??lockAnyContext_1:
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
//  265 	if (alreadyLocked)
        CMP      R4,#+0
        BEQ.N    ??lockAnyContext_2
//  266 		return true;
        MOVS     R0,#+1
        B.N      ??lockAnyContext_3
//  267 	if (isIsrContext()) {
??lockAnyContext_2:
        LDR.N    R0,??DataTable9_27
        LDR      R0,[R0, #+0]
        CMP      R0,#+1
        BLT.N    ??lockAnyContext_4
//  268 		chSysLockFromIsr()
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
        B.N      ??lockAnyContext_5
//  269 		;
//  270 	} else {
//  271 		chSysLock()
??lockAnyContext_4:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock
        BL       dbg_check_lock
//  272 		;
//  273 	}
//  274 	return false;
??lockAnyContext_5:
        MOVS     R0,#+0
??lockAnyContext_3:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock9
//  275 }
//  276 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function lockOutputBuffer
        THUMB
//  277 bool lockOutputBuffer(void) {
lockOutputBuffer:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  278 	return lockAnyContext();
          CFI FunCall lockAnyContext
        BL       lockAnyContext
        POP      {R1,PC}          ;; return
          CFI EndBlock cfiBlock10
//  279 }
//  280 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function unlockAnyContext
        THUMB
//  281 void unlockAnyContext(void) {
unlockAnyContext:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  282 	if (isIsrContext()) {
        LDR.N    R0,??DataTable9_27
        LDR      R0,[R0, #+0]
        CMP      R0,#+1
        BLT.N    ??unlockAnyContext_0
//  283 		chSysUnlockFromIsr()
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
        B.N      ??unlockAnyContext_1
//  284 		;
//  285 	} else {
//  286 		chSysUnlock()
??unlockAnyContext_0:
          CFI FunCall dbg_check_unlock
        BL       dbg_check_unlock
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  287 		;
//  288 	}
//  289 }
??unlockAnyContext_1:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock11

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9:
        DC8      "~",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_1:
        DC32     consoleEventListener

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_2:
        DC32     is_serial_over_uart

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_3:
        DC32     SD3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_4:
        DC32     SDU1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_5:
        DC32     isSerialConsoleStarted

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_6:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_7:
        DC32     `?<Constant "console thread">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_8:
        DC32     binaryConsole

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_9:
        DC32     buffer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_10:
        DC32     `?<Constant "lowstck#9e">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_11:
        DC32     consoleInput

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_12:
        DC32     console_line_callback

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_13:
        DC32     consoleInBinaryMode

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_14:
        DC32     lastWriteSize

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_15:
        DC32     lastWriteActual

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_16:
        DC32     `?<Constant "switching to binary p...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_17:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_18:
        DC32     0x40020400

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_19:
        DC32     0x40020410

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_20:
        DC32     serialConfig

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_21:
        DC32     0x40020800

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_22:
        DC32     SD3+0x4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_23:
        DC32     _Z29consoleThreadThreadEntryPointPv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_24:
        DC32     consoleThreadStack

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_25:
        DC32     _Z22switchToBinaryProtocolv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_26:
        DC32     dbg_lock_cnt

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_27:
        DC32     dbg_isr_cnt
//  290 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function unlockOutputBuffer
        THUMB
//  291 void unlockOutputBuffer(void) {
unlockOutputBuffer:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  292 	unlockAnyContext();
          CFI FunCall unlockAnyContext
        BL       unlockAnyContext
//  293 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock12

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "console thread">`:
        DATA
        DC8 "console thread"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lowstck#9e">`:
        DATA
        DC8 "lowstck#9e"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "switching to binary p...">`:
        DATA
        DC8 "switching to binary protocol"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 "~"

        END
// 
// 9 455 bytes in section .bss
//    92 bytes in section .data
//    62 bytes in section .rodata
//   842 bytes in section .text
// 
//   842 bytes of CODE  memory
//    62 bytes of CONST memory
// 9 547 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
