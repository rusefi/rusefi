///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:51 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\console\binary\ /
//                       tunerstudio.cpp                                      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\console\binary\ /
//                       tunerstudio.cpp -lCN F:\stuff\rusefi_sourceforge\fir /
//                       mware\iar\Debug\List\ -lA                            /
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
//                       tunerstudio.s                                        /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME tunerstudio

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z11startTsPortv
        EXTERN _Z14tsSendResponseP12ts_channel_s20ts_response_format_ePKhi
        EXTERN _Z16isSerialOverUartv
        EXTERN _Z17getTsSerialDevicev
        EXTERN _Z17swapOutputBuffersPi
        EXTERN _Z20tunerStudioWriteDataP12ts_channel_sPKhi
        EXTERN _Z25prepareTunerStudioOutputsv
        EXTERN _Z25tunerStudioWriteCrcPacketP12ts_channel_shPKvt
        EXTERN _Z27setNeedToWriteConfigurationv
        EXTERN _Z35incrementGlobalConfigurationVersionv
        EXTERN _Z8portnameP12GPIO_TypeDef
        EXTERN _ZN18LoggingWithStorageC1EPKc
        EXTERN __aeabi_memcpy
        EXTERN __aeabi_memset
        EXTERN addConsoleAction
        EXTERN addConsoleActionI
        EXTERN boardConfiguration
        EXTERN chThdCreateStatic
        EXTERN chThdSleep
        EXTERN console_line_callback
        EXTERN crc32
        EXTERN currentTimeMillis
        EXTERN efiTrim
        EXTERN engineConfiguration
        EXTERN firmwareError
        EXTERN is_usb_serial_ready
        EXTERN persistentState
        EXTERN print
        EXTERN rlist
        EXTERN scheduleMsg

        PUBLIC _Z11requestBurnv
        PUBLIC _Z11yellowMagiciii
        PUBLIC _Z12printTsStatsv
        PUBLIC _Z16handleCrc32CheckP12ts_channel_s20ts_response_format_ettt
        PUBLIC _Z16tunerStudioDebugPKc
        PUBLIC _Z16tunerStudioErrorPKc
        PUBLIC _Z17handleBurnCommandP12ts_channel_s20ts_response_format_et
        PUBLIC _Z17handleTestCommandP12ts_channel_s
        PUBLIC _Z18getWorkingPageAddri
        PUBLIC _Z18handlePlainCommandP12ts_channel_sh
        PUBLIC _Z18handleQueryCommandP12ts_channel_s20ts_response_format_e
        PUBLIC _Z19syncTunerStudioCopyv
        PUBLIC _Z21handlePageReadCommandP12ts_channel_s20ts_response_format_ettt
        PUBLIC _Z21runBinaryProtocolLoopP12ts_channel_sb
        PUBLIC _Z22getTunerStudioPageSizei
        PUBLIC _Z23handlePageSelectCommandP12ts_channel_s20ts_response_format_et
        PUBLIC _Z23handleWriteChunkCommandP12ts_channel_s20ts_response_format_essPv
        PUBLIC _Z23handleWriteValueCommandP12ts_channel_s20ts_response_format_etth
        PUBLIC _Z27handleOutputChannelsCommandP12ts_channel_s20ts_response_format_e
        PUBLIC _Z27tunerStudioHandleCrcCommandP12ts_channel_sPci
        PUBLIC _Z28startTunerStudioConnectivityv
        PUBLIC configWorkingCopy
        PUBLIC currentPageId
        PUBLIC memcpy
        PUBLIC memset
        PUBLIC tsChannel
        PUBLIC tsLogger
        PUBLIC tsOutputChannels
        PUBLIC tsState
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\console\binary\tunerstudio.cpp
//    1 /**
//    2  * @file	tunerstudio.cpp
//    3  * @brief	Binary protocol implementation
//    4  *
//    5  * This implementation would not happen without the documentation
//    6  * provided by Jon Zeeff (jon@zeeff.com)
//    7  *
//    8  *
//    9  * @brief Integration with EFI Analytics Tuner Studio software
//   10  *
//   11  * Tuner Studio has a really simple protocol, a minimal implementation
//   12  * capable of displaying current engine state on the gauges would
//   13  * require only two commands: queryCommand and ochGetCommand
//   14  *
//   15  * queryCommand:
//   16  * 		Communication initialization command. TunerStudio sends a single byte H
//   17  * 		ECU response:
//   18  * 			One of the known ECU id strings. We are using "MShift v0.01" id string.
//   19  *
//   20  * ochGetCommand:
//   21  * 		Request for output channels state.TunerStudio sends a single byte O
//   22  * 		ECU response:
//   23  * 			A snapshot of output channels as described in [OutputChannels] section of the .ini file
//   24  * 			The length of this block is 'ochBlockSize' property of the .ini file
//   25  *
//   26  * These two commands are enough to get working gauges. In order to start configuring the ECU using
//   27  * tuner studio, three more commands should be implemented:
//   28  *
//   29  * todo: merge this file with tunerstudio.c?
//   30  *
//   31  *
//   32  * @date Oct 22, 2013
//   33  * @author Andrey Belomutskiy, (c) 2012-2015
//   34  *
//   35  * This file is part of rusEfi - see http://rusefi.com
//   36  *
//   37  * rusEfi is free software; you can redistribute it and/or modify it under the terms of
//   38  * the GNU General Public License as published by the Free Software Foundation; either
//   39  * version 3 of the License, or (at your option) any later version.
//   40  *
//   41  * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
//   42  * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   43  * GNU General Public License for more details.
//   44  *
//   45  * You should have received a copy of the GNU General Public License along with this program.
//   46  * If not, see <http://www.gnu.org/licenses/>.
//   47  *
//   48  * @author Andrey Belomutskiy, (c) 2012-2015
//   49  *
//   50  * This file is part of rusEfi - see http://rusefi.com
//   51  *
//   52  * rusEfi is free software; you can redistribute it and/or modify it under the terms of
//   53  * the GNU General Public License as published by the Free Software Foundation; either
//   54  * version 3 of the License, or (at your option) any later version.
//   55  *
//   56  * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
//   57  * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   58  * GNU General Public License for more details.
//   59  *
//   60  * You should have received a copy of the GNU General Public License along with this program.
//   61  * If not, see <http://www.gnu.org/licenses/>.
//   62  *
//   63  */
//   64 
//   65 #include "main.h"
//   66 
//   67 #include "engine_state.h"
//   68 #include "tunerstudio.h"
//   69 
//   70 #include "main_trigger_callback.h"
//   71 #include "flash_main.h"
//   72 
//   73 #include "tunerstudio_io.h"
//   74 #include "tunerstudio_configuration.h"
//   75 #include "malfunction_central.h"
//   76 #include "console_io.h"
//   77 #include "crc.h"
//   78 
//   79 #include <string.h>
//   80 #include "engine_configuration.h"
//   81 #include "svnversion.h"
//   82 #include "loggingcentral.h"
//   83 #include "status_loop.h"
//   84 
//   85 #if EFI_TUNER_STUDIO || defined(__DOXYGEN__)
//   86 
//   87 EXTERN_ENGINE
//   88 ;
//   89 
//   90 extern short currentPageId;
//   91 
//   92 // in MS, that's 3 seconds
//   93 #define TS_READ_TIMEOUT MS2ST(3000)
//   94 
//   95 /**
//   96  * note the use-case where text console port is switched into
//   97  * binary port
//   98  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   99 LoggingWithStorage tsLogger("binary");
        LDR.W    R1,??DataTable22
        LDR.W    R0,??DataTable23
          CFI FunCall _ZN18LoggingWithStorageC1EPKc
        BL       _ZN18LoggingWithStorageC1EPKc
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
tsLogger:
        DS8 260
//  100 
//  101 extern persistent_config_s configWorkingCopy;
//  102 extern persistent_config_container_s persistentState;
//  103 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  104 static efitimems_t previousWriteReportMs = 0;
previousWriteReportMs:
        DS8 4
//  105 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  106 ts_channel_s tsChannel;
tsChannel:
        DS8 308
//  107 
//  108 extern uint8_t crcWriteBuffer[300];
//  109 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z15ts_serial_readyb
        THUMB
//  110 static int ts_serial_ready(bool_t isConsoleRedirect) {
_Z15ts_serial_readyb:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  111 #if EFI_PROD_CODE
//  112 	if (isSerialOverUart() ^ isConsoleRedirect) {
          CFI FunCall _Z16isSerialOverUartv
        BL       _Z16isSerialOverUartv
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        TEQ      R4,R0
        BEQ.N    ??ts_serial_ready_0
//  113 		// TS uses USB when console uses serial
//  114 		return is_usb_serial_ready();
          CFI FunCall is_usb_serial_ready
        BL       is_usb_serial_ready
        B.N      ??ts_serial_ready_1
//  115 	} else {
//  116 		// TS uses serial when console uses USB
//  117 		return true;
??ts_serial_ready_0:
        MOVS     R0,#+1
??ts_serial_ready_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1
//  118 	}
//  119 #else
//  120 	return true;
//  121 #endif
//  122 }
//  123 

        SECTION `.data`:DATA:REORDER:NOROOT(1)
//  124 static uint16_t BINARY_RESPONSE = (uint16_t) SWAP_UINT16(BINARY_SWITCH_TAG);
BINARY_RESPONSE:
        DATA
        DC16 2686
//  125 
//  126 // this thread wants a bit extra stack

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//  127 static THD_WORKING_AREA(tsThreadStack, 3 * UTILITY_THREAD_STACK_SIZE);
tsThreadStack:
        DS8 1512
//  128 
//  129 extern TunerStudioOutputChannels tsOutputChannels;
//  130 
//  131 extern tunerstudio_counters_s tsState;
//  132 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z7resetTsv
        THUMB
//  133 static void resetTs(void) {
_Z7resetTsv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  134 	memset(&tsState, 0, sizeof(tsState));
        MOVS     R2,#+40
        MOVS     R1,#+0
        LDR.W    R0,??DataTable22_1
          CFI FunCall memset
        BL       memset
//  135 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock2
//  136 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z18printErrorCountersv
        THUMB
//  137 static void printErrorCounters(void) {
_Z18printErrorCountersv:
        PUSH     {LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+4
        SUB      SP,SP,#+20
          CFI CFA R13+24
//  138 	scheduleMsg(&tsLogger, "TunerStudio size=%d / total=%d / errors=%d / H=%d / O=%d / P=%d / B=%d",
//  139 			sizeof(tsOutputChannels), tsState.tsCounter, tsState.errorCounter, tsState.queryCommandCounter,
//  140 			tsState.outputChannelsCommandCounter, tsState.readPageCommandsCounter, tsState.burnCommandCounter);
        LDR.W    R0,??DataTable22_1
        LDR      R0,[R0, #+12]
        STR      R0,[SP, #+16]
        LDR.W    R0,??DataTable22_1
        LDR      R0,[R0, #+8]
        STR      R0,[SP, #+12]
        LDR.W    R0,??DataTable22_1
        LDR      R0,[R0, #+4]
        STR      R0,[SP, #+8]
        LDR.W    R0,??DataTable22_1
        LDR      R0,[R0, #+0]
        STR      R0,[SP, #+4]
        LDR.W    R0,??DataTable22_1
        LDR      R0,[R0, #+32]
        STR      R0,[SP, #+0]
        LDR.W    R0,??DataTable22_1
        LDR      R3,[R0, #+36]
        MOVS     R2,#+196
        LDR.W    R1,??DataTable22_2
        LDR.W    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  141 	scheduleMsg(&tsLogger, "TunerStudio W=%d / C=%d / P=%d / page=%d", tsState.writeValueCommandCounter,
//  142 			tsState.writeChunkCommandCounter, tsState.pageCommandCounter, currentPageId);
        LDR.W    R0,??DataTable22_3
        LDRSH    R0,[R0, #+0]
        STR      R0,[SP, #+4]
        LDR.W    R0,??DataTable22_1
        LDR      R0,[R0, #+16]
        STR      R0,[SP, #+0]
        LDR.W    R0,??DataTable22_1
        LDR      R3,[R0, #+28]
        LDR.W    R0,??DataTable22_1
        LDR      R2,[R0, #+20]
        LDR.W    R1,??DataTable22_4
        LDR.W    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  143 //	scheduleMsg(&tsLogger, "page size=%d", getTunerStudioPageSize(currentPageId));
//  144 }
        ADD      SP,SP,#+20
          CFI CFA R13+4
        POP      {PC}             ;; return
          CFI EndBlock cfiBlock3
//  145 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z12printTsStatsv
        THUMB
//  146 void printTsStats(void) {
_Z12printTsStatsv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        SUB      SP,SP,#+16
          CFI CFA R13+24
//  147 #if EFI_PROD_CODE
//  148 	if (!isSerialOverUart()) {
          CFI FunCall _Z16isSerialOverUartv
        BL       _Z16isSerialOverUartv
        CMP      R0,#+0
        BNE.N    ??printTsStats_0
//  149 		scheduleMsg(&tsLogger, "TS RX on %s%d/TX on %s%d @%d", portname(TS_SERIAL_RX_PORT), TS_SERIAL_RX_PIN,
//  150 				portname(TS_SERIAL_TX_PORT), TS_SERIAL_TX_PIN, boardConfiguration->tunerStudioSerialSpeed);
        LDR.W    R0,??DataTable22_5  ;; 0x40020800
          CFI FunCall _Z8portnameP12GPIO_TypeDef
        BL       _Z8portnameP12GPIO_TypeDef
        MOVS     R4,R0
        LDR.W    R0,??DataTable22_5  ;; 0x40020800
          CFI FunCall _Z8portnameP12GPIO_TypeDef
        BL       _Z8portnameP12GPIO_TypeDef
        LDR.W    R1,??DataTable22_6
        LDR      R1,[R1, #+0]
        LDR      R1,[R1, #+332]
        STR      R1,[SP, #+8]
        MOVS     R1,#+10
        STR      R1,[SP, #+4]
        STR      R4,[SP, #+0]
        MOVS     R3,#+11
        MOVS     R2,R0
        LDR.W    R1,??DataTable22_7
        LDR.W    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  151 	}
//  152 #endif /* EFI_PROD_CODE */
//  153 
//  154 	printErrorCounters();
??printTsStats_0:
          CFI FunCall _Z18printErrorCountersv
        BL       _Z18printErrorCountersv
//  155 
//  156 //	scheduleMsg(logger, "analogChartFrequency %d",
//  157 //			(int) (&engineConfiguration->analogChartFrequency) - (int) engineConfiguration);
//  158 //
//  159 //	int fuelMapOffset = (int) (&engineConfiguration->fuelTable) - (int) engineConfiguration;
//  160 //	scheduleMsg(logger, "fuelTable %d", fuelMapOffset);
//  161 //
//  162 //	int offset = (int) (&boardConfiguration->hip9011Gain) - (int) engineConfiguration;
//  163 //	scheduleMsg(&tsLogger, "hip9011Gain %d", offset);
//  164 //
//  165 //	offset = (int) (&engineConfiguration->crankingCycleBins) - (int) engineConfiguration;
//  166 //	scheduleMsg(&tsLogger, "crankingCycleBins %d", offset);
//  167 //
//  168 //	offset = (int) (&engineConfiguration->engineCycle) - (int) engineConfiguration;
//  169 //	scheduleMsg(&tsLogger, "engineCycle %d", offset);
//  170 }
        POP      {R0-R4,PC}       ;; return
          CFI EndBlock cfiBlock4
//  171 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z10setTsSpeedi
        THUMB
//  172 static void setTsSpeed(int value) {
_Z10setTsSpeedi:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  173 	boardConfiguration->tunerStudioSerialSpeed = value;
        LDR.W    R0,??DataTable22_6
        LDR      R0,[R0, #+0]
        STR      R4,[R0, #+332]
//  174 	printTsStats();
          CFI FunCall _Z12printTsStatsv
        BL       _Z12printTsStatsv
//  175 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock5
//  176 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z16tunerStudioDebugPKc
        THUMB
//  177 void tunerStudioDebug(const char *msg) {
_Z16tunerStudioDebugPKc:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  178 #if EFI_TUNER_STUDIO_VERBOSE
//  179 	scheduleMsg(&tsLogger, "%s", msg);
        MOVS     R2,R4
        ADR.N    R1,??DataTable10  ;; 0x25, 0x73, 0x00, 0x00
        LDR.W    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  180 #endif
//  181 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock6
//  182 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z18getWorkingPageAddri
          CFI NoCalls
        THUMB
//  183 char *getWorkingPageAddr(int pageIndex) {
_Z18getWorkingPageAddri:
        MOVS     R1,R0
//  184 	switch (pageIndex) {
        CMP      R1,#+0
        BNE.N    ??getWorkingPageAddr_0
//  185 	case 0:
//  186 		return (char*) &configWorkingCopy.engineConfiguration;
        LDR.W    R0,??DataTable23_1
        B.N      ??getWorkingPageAddr_1
//  187 //	case 1:
//  188 //		return (char*) &configWorkingCopy.ve2Table;
//  189 //	case 2:
//  190 //		return (char*) &configWorkingCopy.fuelTable;
//  191 //	case 3:
//  192 //		return (char*) &configWorkingCopy.ignitionTable;
//  193 //	case 4:
//  194 //		return (char*) &configWorkingCopy.veTable;
//  195 //	case 5:
//  196 //		return (char*) &configWorkingCopy.afrTable;
//  197 //	case 6:
//  198 //		return (char*) &configWorkingCopy.injectionPhase;
//  199 //	case 2: // fuelTable
//  200 //	case 3: // ignitionTable
//  201 //	case 4: // veTable
//  202 //	case 5: // afrTable
//  203 //		return (char*) &configWorkingCopy.engineConfiguration + PAGE_0_SIZE + (pageIndex - 2) * 1024;
//  204 	}
//  205 	return NULL;
??getWorkingPageAddr_0:
        MOVS     R0,#+0
??getWorkingPageAddr_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock7
//  206 }
//  207 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z22getTunerStudioPageSizei
          CFI NoCalls
        THUMB
//  208 int getTunerStudioPageSize(int pageIndex) {
_Z22getTunerStudioPageSizei:
        MOVS     R1,R0
//  209 	switch (pageIndex) {
        CMP      R1,#+0
        BNE.N    ??getTunerStudioPageSize_0
//  210 	case 0:
//  211 		return TOTAL_CONFIG_SIZE;
        MOVW     R0,#+15288
        B.N      ??getTunerStudioPageSize_1
//  212 //	case 1:
//  213 //	case 2:
//  214 //	case 3:
//  215 //	case 4:
//  216 //	case 5:
//  217 //	case 6:
//  218 //		return PAGE_1_SIZE;
//  219 //	case 2:
//  220 //	case 3:
//  221 //	case 4:
//  222 //		return 1024;
//  223 	}
//  224 	return 0;
??getTunerStudioPageSize_0:
        MOVS     R0,#+0
??getTunerStudioPageSize_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock8
//  225 }
//  226 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z23handlePageSelectCommandP12ts_channel_s20ts_response_format_et
        THUMB
//  227 void handlePageSelectCommand(ts_channel_s *tsChannel, ts_response_format_e mode, uint16_t pageId) {
_Z23handlePageSelectCommandP12ts_channel_s20ts_response_format_et:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  228 	tsState.pageCommandCounter++;
        LDR.W    R0,??DataTable22_1
        LDR      R0,[R0, #+16]
        ADDS     R0,R0,#+1
        LDR.W    R1,??DataTable22_1
        STR      R0,[R1, #+16]
//  229 
//  230 	currentPageId = pageId;
        LDR.W    R0,??DataTable22_3
        STRH     R6,[R0, #+0]
//  231 	scheduleMsg(&tsLogger, "PAGE %d", currentPageId);
        LDR.W    R0,??DataTable22_3
        LDRSH    R2,[R0, #+0]
        LDR.W    R1,??DataTable23_2
        LDR.W    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  232 	tsSendResponse(tsChannel, mode, NULL, 0);
        MOVS     R3,#+0
        MOVS     R2,#+0
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall _Z14tsSendResponseP12ts_channel_s20ts_response_format_ePKhi
        BL       _Z14tsSendResponseP12ts_channel_s20ts_response_format_ePKhi
//  233 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock9
//  234 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z11yellowMagiciii
        THUMB
//  235 void yellowMagic(int currentPageId, int offset, int count) {
_Z11yellowMagiciii:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  236 	if (offset > 6200) {
        MOVW     R0,#+6201
        CMP      R5,R0
        BLT.N    ??yellowMagic_0
//  237 		scheduleMsg(&tsLogger, "applying soft change from %d length %d", offset, count);
        MOVS     R3,R6
        MOVS     R2,R5
        LDR.W    R1,??DataTable23_3
        LDR.W    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  238 		memcpy(((char*) engineConfiguration) + offset, ((char*) &configWorkingCopy.engineConfiguration) + offset,
//  239 				count);
        MOVS     R2,R6
        LDR.W    R0,??DataTable23_1
        ADDS     R1,R5,R0
        LDR.W    R0,??DataTable24_1
        LDR      R0,[R0, #+0]
        ADDS     R0,R5,R0
          CFI FunCall memcpy
        BL       memcpy
//  240 	}
//  241 }
??yellowMagic_0:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock10
//  242 
//  243 /**
//  244  * This command is needed to make the whole transfer a bit faster
//  245  * @note See also handleWriteValueCommand
//  246  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z23handleWriteChunkCommandP12ts_channel_s20ts_response_format_essPv
        THUMB
//  247 void handleWriteChunkCommand(ts_channel_s *tsChannel, ts_response_format_e mode, short offset, short count,
//  248 		void *content) {
_Z23handleWriteChunkCommandP12ts_channel_s20ts_response_format_essPv:
        PUSH     {R4-R9,LR}
          CFI R14 Frame(CFA, -4)
          CFI R9 Frame(CFA, -8)
          CFI R8 Frame(CFA, -12)
          CFI R7 Frame(CFA, -16)
          CFI R6 Frame(CFA, -20)
          CFI R5 Frame(CFA, -24)
          CFI R4 Frame(CFA, -28)
          CFI CFA R13+28
        SUB      SP,SP,#+12
          CFI CFA R13+40
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
        LDR      R8,[SP, #+40]
//  249 	tsState.writeChunkCommandCounter++;
        LDR.W    R0,??DataTable24_2
        LDR      R0,[R0, #+28]
        ADDS     R0,R0,#+1
        LDR.W    R1,??DataTable24_2
        STR      R0,[R1, #+28]
//  250 
//  251 	scheduleMsg(&tsLogger, "WRITE CHUNK mode=%d p=%d o=%d s=%d", mode, currentPageId, offset, count);
        SXTH     R7,R7            ;; SignExt  R7,R7,#+16,#+16
        STR      R7,[SP, #+4]
        SXTH     R6,R6            ;; SignExt  R6,R6,#+16,#+16
        STR      R6,[SP, #+0]
        LDR.W    R0,??DataTable24_3
        LDRSH    R3,[R0, #+0]
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        MOVS     R2,R5
        LDR.W    R1,??DataTable23_4
        LDR.W    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  252 
//  253 	if (offset > getTunerStudioPageSize(currentPageId)) {
        LDR.W    R0,??DataTable24_3
        LDRSH    R0,[R0, #+0]
          CFI FunCall _Z22getTunerStudioPageSizei
        BL       _Z22getTunerStudioPageSizei
        SXTH     R6,R6            ;; SignExt  R6,R6,#+16,#+16
        CMP      R0,R6
        BGE.N    ??handleWriteChunkCommand_0
//  254 		scheduleMsg(&tsLogger, "ERROR invalid offset %d", offset);
        SXTH     R6,R6            ;; SignExt  R6,R6,#+16,#+16
        MOVS     R2,R6
        LDR.W    R1,??DataTable24_4
        LDR.W    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  255 		tunerStudioError("ERROR: out of range");
        LDR.W    R0,??DataTable24_5
          CFI FunCall _Z16tunerStudioErrorPKc
        BL       _Z16tunerStudioErrorPKc
//  256 		offset = 0;
        MOVS     R0,#+0
        MOVS     R6,R0
//  257 	}
//  258 
//  259 	if (count > getTunerStudioPageSize(currentPageId)) {
??handleWriteChunkCommand_0:
        LDR.W    R0,??DataTable24_3
        LDRSH    R0,[R0, #+0]
          CFI FunCall _Z22getTunerStudioPageSizei
        BL       _Z22getTunerStudioPageSizei
        SXTH     R7,R7            ;; SignExt  R7,R7,#+16,#+16
        CMP      R0,R7
        BGE.N    ??handleWriteChunkCommand_1
//  260 		tunerStudioError("ERROR: unexpected count");
        LDR.W    R0,??DataTable24_6
          CFI FunCall _Z16tunerStudioErrorPKc
        BL       _Z16tunerStudioErrorPKc
//  261 		scheduleMsg(&tsLogger, "ERROR unexpected count %d", count);
        SXTH     R7,R7            ;; SignExt  R7,R7,#+16,#+16
        MOVS     R2,R7
        LDR.W    R1,??DataTable24_7
        LDR.W    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  262 		count = 0;
        MOVS     R0,#+0
        MOVS     R7,R0
//  263 	}
//  264 
//  265 	uint8_t * addr = (uint8_t *) (getWorkingPageAddr(currentPageId) + offset);
??handleWriteChunkCommand_1:
        SXTH     R6,R6            ;; SignExt  R6,R6,#+16,#+16
        LDR.W    R0,??DataTable24_3
        LDRSH    R0,[R0, #+0]
          CFI FunCall _Z18getWorkingPageAddri
        BL       _Z18getWorkingPageAddri
        ADDS     R9,R6,R0
//  266 	memcpy(addr, content, count);
        SXTH     R7,R7            ;; SignExt  R7,R7,#+16,#+16
        MOVS     R2,R7
        MOV      R1,R8
        MOV      R0,R9
          CFI FunCall memcpy
        BL       memcpy
//  267 	yellowMagic(currentPageId, offset, count);
        SXTH     R7,R7            ;; SignExt  R7,R7,#+16,#+16
        MOVS     R2,R7
        SXTH     R6,R6            ;; SignExt  R6,R6,#+16,#+16
        MOVS     R1,R6
        LDR.W    R0,??DataTable24_3
        LDRSH    R0,[R0, #+0]
          CFI FunCall _Z11yellowMagiciii
        BL       _Z11yellowMagiciii
//  268 
//  269 	tsSendResponse(tsChannel, mode, NULL, 0);
        MOVS     R3,#+0
        MOVS     R2,#+0
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall _Z14tsSendResponseP12ts_channel_s20ts_response_format_ePKhi
        BL       _Z14tsSendResponseP12ts_channel_s20ts_response_format_ePKhi
//  270 }
        POP      {R0-R2,R4-R9,PC}  ;; return
          CFI EndBlock cfiBlock11
//  271 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _Z16handleCrc32CheckP12ts_channel_s20ts_response_format_ettt
        THUMB
//  272 void handleCrc32Check(ts_channel_s *tsChannel, ts_response_format_e mode, uint16_t pageId, uint16_t offset,
//  273 		uint16_t count) {
_Z16handleCrc32CheckP12ts_channel_s20ts_response_format_ettt:
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
        MOVS     R5,R0
        MOVS     R6,R1
        MOVS     R7,R2
        MOV      R8,R3
        LDR      R4,[SP, #+40]
//  274 	tsState.crc32CheckCommandCounter++;
        LDR.W    R0,??DataTable22_1
        LDR      R0,[R0, #+24]
        ADDS     R0,R0,#+1
        LDR.W    R1,??DataTable22_1
        STR      R0,[R1, #+24]
//  275 
//  276 	count = SWAP_UINT16(count);
        UXTH     R4,R4            ;; ZeroExt  R4,R4,#+16,#+16
        LSRS     R0,R4,#+8
        ORRS     R4,R0,R4, LSL #+8
//  277 
//  278 	count = getTunerStudioPageSize(pageId);
        UXTH     R7,R7            ;; ZeroExt  R7,R7,#+16,#+16
        MOVS     R0,R7
          CFI FunCall _Z22getTunerStudioPageSizei
        BL       _Z22getTunerStudioPageSizei
        MOVS     R4,R0
//  279 
//  280 	scheduleMsg(&tsLogger, "CRC32 request: pageId %d offset %d size %d", pageId, offset, count);
        UXTH     R4,R4            ;; ZeroExt  R4,R4,#+16,#+16
        STR      R4,[SP, #+0]
        UXTH     R8,R8            ;; ZeroExt  R8,R8,#+16,#+16
        MOV      R3,R8
        UXTH     R7,R7            ;; ZeroExt  R7,R7,#+16,#+16
        MOVS     R2,R7
        LDR.W    R1,??DataTable24_8
        LDR.W    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  281 
//  282 	uint32_t crc = SWAP_UINT32(crc32((void * ) getWorkingPageAddr(0), count));
        MOVS     R0,#+0
          CFI FunCall _Z18getWorkingPageAddri
        BL       _Z18getWorkingPageAddri
        UXTH     R4,R4            ;; ZeroExt  R4,R4,#+16,#+16
        MOVS     R1,R4
          CFI FunCall crc32
        BL       crc32
        MOV      R9,R0
        MOVS     R0,#+0
          CFI FunCall _Z18getWorkingPageAddri
        BL       _Z18getWorkingPageAddri
        UXTH     R4,R4            ;; ZeroExt  R4,R4,#+16,#+16
        MOVS     R1,R4
          CFI FunCall crc32
        BL       crc32
        MOV      R10,R0
        MOVS     R0,#+0
          CFI FunCall _Z18getWorkingPageAddri
        BL       _Z18getWorkingPageAddri
        UXTH     R4,R4            ;; ZeroExt  R4,R4,#+16,#+16
        MOVS     R1,R4
          CFI FunCall crc32
        BL       crc32
        MOV      R11,R0
        MOVS     R0,#+0
          CFI FunCall _Z18getWorkingPageAddri
        BL       _Z18getWorkingPageAddri
        LSLS     R1,R10,#+8
        ANDS     R1,R1,#0xFF0000
        ORRS     R1,R1,R9, LSR #+24
        LSRS     R2,R11,#+8
        ANDS     R2,R2,#0xFF00
        ORRS     R9,R2,R1
        UXTH     R4,R4            ;; ZeroExt  R4,R4,#+16,#+16
        MOVS     R1,R4
          CFI FunCall crc32
        BL       crc32
        ORRS     R0,R9,R0, LSL #+24
        STR      R0,[SP, #+0]
//  283 
//  284 	scheduleMsg(&tsLogger, "CRC32 response: %x", crc);
        LDR      R2,[SP, #+0]
        LDR.W    R1,??DataTable24_9
        LDR.W    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  285 
//  286 	tsSendResponse(tsChannel, mode, (const uint8_t *) &crc, 4);
        MOVS     R3,#+4
        ADD      R2,SP,#+0
        MOVS     R1,R6
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R5
          CFI FunCall _Z14tsSendResponseP12ts_channel_s20ts_response_format_ePKhi
        BL       _Z14tsSendResponseP12ts_channel_s20ts_response_format_ePKhi
//  287 }
        POP      {R0,R4-R11,PC}   ;; return
          CFI EndBlock cfiBlock12

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10:
        DC8      0x25, 0x73, 0x00, 0x00
//  288 
//  289 /**
//  290  * 'Write' command receives a single value at a given offset
//  291  * @note Writing values one by one is pretty slow
//  292  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z23handleWriteValueCommandP12ts_channel_s20ts_response_format_etth
        THUMB
//  293 void handleWriteValueCommand(ts_channel_s *tsChannel, ts_response_format_e mode, uint16_t page, uint16_t offset,
//  294 		uint8_t value) {
_Z23handleWriteValueCommandP12ts_channel_s20ts_response_format_etth:
        PUSH     {R3-R9,LR}
          CFI R14 Frame(CFA, -4)
          CFI R9 Frame(CFA, -8)
          CFI R8 Frame(CFA, -12)
          CFI R7 Frame(CFA, -16)
          CFI R6 Frame(CFA, -20)
          CFI R5 Frame(CFA, -24)
          CFI R4 Frame(CFA, -28)
          CFI CFA R13+32
        MOVS     R5,R0
        MOVS     R6,R1
        MOVS     R7,R2
        MOV      R8,R3
        LDR      R4,[SP, #+32]
//  295 	tsState.writeValueCommandCounter++;
        LDR.W    R0,??DataTable24_2
        LDR      R0,[R0, #+20]
        ADDS     R0,R0,#+1
        LDR.W    R1,??DataTable24_2
        STR      R0,[R1, #+20]
//  296 
//  297 	currentPageId = page;
        LDR.W    R0,??DataTable24_3
        STRH     R7,[R0, #+0]
//  298 
//  299 	tunerStudioDebug("got W (Write)"); // we can get a lot of these
        LDR.W    R0,??DataTable24_10
          CFI FunCall _Z16tunerStudioDebugPKc
        BL       _Z16tunerStudioDebugPKc
//  300 
//  301 #if EFI_TUNER_STUDIO_VERBOSE
//  302 //	scheduleMsg(logger, "Page number %d\r\n", pageId); // we can get a lot of these
//  303 #endif
//  304 
//  305 //	int size = sizeof(TunerStudioWriteValueRequest);
//  306 //	scheduleMsg(logger, "Reading %d\r\n", size);
//  307 
//  308 	if (offset > getTunerStudioPageSize(currentPageId)) {
        LDR.W    R0,??DataTable24_3
        LDRSH    R0,[R0, #+0]
          CFI FunCall _Z22getTunerStudioPageSizei
        BL       _Z22getTunerStudioPageSizei
        UXTH     R8,R8            ;; ZeroExt  R8,R8,#+16,#+16
        CMP      R0,R8
        BGE.N    ??handleWriteValueCommand_0
//  309 		tunerStudioError("ERROR: out of range2");
        LDR.W    R0,??DataTable24_11
          CFI FunCall _Z16tunerStudioErrorPKc
        BL       _Z16tunerStudioErrorPKc
//  310 		scheduleMsg(&tsLogger, "ERROR offset %d", offset);
        UXTH     R8,R8            ;; ZeroExt  R8,R8,#+16,#+16
        MOV      R2,R8
        LDR.W    R1,??DataTable24_12
        LDR.W    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  311 		offset = 0;
        MOVS     R0,#+0
        MOV      R8,R0
//  312 		return;
        B.N      ??handleWriteValueCommand_1
//  313 	}
//  314 
//  315 	efitimems_t nowMs = currentTimeMillis();
??handleWriteValueCommand_0:
          CFI FunCall currentTimeMillis
        BL       currentTimeMillis
        MOV      R9,R0
//  316 	if (nowMs - previousWriteReportMs > 5) {
        LDR.W    R0,??DataTable24_13
        LDR      R0,[R0, #+0]
        SUBS     R0,R9,R0
        CMP      R0,#+6
        BCC.N    ??handleWriteValueCommand_2
//  317 		previousWriteReportMs = nowMs;
        LDR.W    R0,??DataTable24_13
        STR      R9,[R0, #+0]
//  318 		scheduleMsg(&tsLogger, "page %d offset %d: value=%d", currentPageId, offset, value);
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        STR      R4,[SP, #+0]
        UXTH     R8,R8            ;; ZeroExt  R8,R8,#+16,#+16
        MOV      R3,R8
        LDR.W    R0,??DataTable24_3
        LDRSH    R2,[R0, #+0]
        LDR.W    R1,??DataTable24_14
        LDR.W    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  319 	}
//  320 
//  321 	getWorkingPageAddr(currentPageId)[offset] = value;
??handleWriteValueCommand_2:
        UXTH     R8,R8            ;; ZeroExt  R8,R8,#+16,#+16
        LDR.W    R0,??DataTable24_3
        LDRSH    R0,[R0, #+0]
          CFI FunCall _Z18getWorkingPageAddri
        BL       _Z18getWorkingPageAddri
        STRB     R4,[R8, R0]
//  322 
//  323 	yellowMagic(currentPageId, offset, 1);
        MOVS     R2,#+1
        UXTH     R8,R8            ;; ZeroExt  R8,R8,#+16,#+16
        MOV      R1,R8
        LDR.W    R0,??DataTable24_3
        LDRSH    R0,[R0, #+0]
          CFI FunCall _Z11yellowMagiciii
        BL       _Z11yellowMagiciii
//  324 
//  325 //	scheduleMsg(logger, "va=%d", configWorkingCopy.boardConfiguration.idleValvePin);
//  326 }
??handleWriteValueCommand_1:
        POP      {R0,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock13
//  327 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _Z13sendErrorCodeP12ts_channel_s
        THUMB
//  328 static void sendErrorCode(ts_channel_s *tsChannel) {
_Z13sendErrorCodeP12ts_channel_s:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  329 	tunerStudioWriteCrcPacket(tsChannel, TS_RESPONSE_CRC_FAILURE, NULL, 0);
        MOVS     R3,#+0
        MOVS     R2,#+0
        MOVS     R1,#+130
        MOVS     R0,R4
          CFI FunCall _Z25tunerStudioWriteCrcPacketP12ts_channel_shPKvt
        BL       _Z25tunerStudioWriteCrcPacketP12ts_channel_shPKvt
//  330 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock14
//  331 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function _Z21handlePageReadCommandP12ts_channel_s20ts_response_format_ettt
        THUMB
//  332 void handlePageReadCommand(ts_channel_s *tsChannel, ts_response_format_e mode, uint16_t pageId, uint16_t offset,
//  333 		uint16_t count) {
_Z21handlePageReadCommandP12ts_channel_s20ts_response_format_ettt:
        PUSH     {R2-R10,LR}
          CFI R14 Frame(CFA, -4)
          CFI R10 Frame(CFA, -8)
          CFI R9 Frame(CFA, -12)
          CFI R8 Frame(CFA, -16)
          CFI R7 Frame(CFA, -20)
          CFI R6 Frame(CFA, -24)
          CFI R5 Frame(CFA, -28)
          CFI R4 Frame(CFA, -32)
          CFI CFA R13+40
        MOVS     R5,R0
        MOVS     R6,R1
        MOVS     R7,R2
        MOV      R8,R3
        LDR      R4,[SP, #+40]
//  334 	tsState.readPageCommandsCounter++;
        LDR.W    R0,??DataTable24_2
        LDR      R0,[R0, #+8]
        ADDS     R0,R0,#+1
        LDR.W    R1,??DataTable24_2
        STR      R0,[R1, #+8]
//  335 	currentPageId = pageId;
        LDR.W    R0,??DataTable24_3
        STRH     R7,[R0, #+0]
//  336 
//  337 #if EFI_TUNER_STUDIO_VERBOSE
//  338 	scheduleMsg(&tsLogger, "READ mode=%d page=%d offset=%d size=%d", mode, (int) currentPageId, offset, count);
        UXTH     R4,R4            ;; ZeroExt  R4,R4,#+16,#+16
        STR      R4,[SP, #+4]
        UXTH     R8,R8            ;; ZeroExt  R8,R8,#+16,#+16
        STR      R8,[SP, #+0]
        LDR.W    R0,??DataTable24_3
        LDRSH    R3,[R0, #+0]
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        MOVS     R2,R6
        LDR.W    R1,??DataTable24_15
        LDR.W    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  339 #endif
//  340 
//  341 	if (currentPageId >= PAGE_COUNT) {
        LDR.W    R0,??DataTable24_3
        LDRSH    R0,[R0, #+0]
        CMP      R0,#+1
        BLT.N    ??handlePageReadCommand_0
//  342 		// something is not right here
//  343 		currentPageId = 0;
        LDR.W    R0,??DataTable24_3
        MOVS     R1,#+0
        STRH     R1,[R0, #+0]
//  344 		tunerStudioError("ERROR: invalid page number");
        LDR.W    R0,??DataTable24_16
          CFI FunCall _Z16tunerStudioErrorPKc
        BL       _Z16tunerStudioErrorPKc
//  345 		return;
        B.N      ??handlePageReadCommand_1
//  346 	}
//  347 
//  348 	int size = getTunerStudioPageSize(currentPageId);
??handlePageReadCommand_0:
        LDR.W    R0,??DataTable24_3
        LDRSH    R0,[R0, #+0]
          CFI FunCall _Z22getTunerStudioPageSizei
        BL       _Z22getTunerStudioPageSizei
        MOV      R9,R0
//  349 
//  350 	if (size < offset + count) {
        UXTH     R4,R4            ;; ZeroExt  R4,R4,#+16,#+16
        UXTAH    R0,R4,R8
        CMP      R9,R0
        BGE.N    ??handlePageReadCommand_2
//  351 		scheduleMsg(&tsLogger, "invalid offset/count %d/%d", offset, count);
        UXTH     R4,R4            ;; ZeroExt  R4,R4,#+16,#+16
        MOVS     R3,R4
        UXTH     R8,R8            ;; ZeroExt  R8,R8,#+16,#+16
        MOV      R2,R8
        LDR.W    R1,??DataTable24_17
        LDR.W    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  352 		sendErrorCode(tsChannel);
        MOVS     R0,R5
          CFI FunCall _Z13sendErrorCodeP12ts_channel_s
        BL       _Z13sendErrorCodeP12ts_channel_s
//  353 		return;
        B.N      ??handlePageReadCommand_1
//  354 	}
//  355 
//  356 	const uint8_t *addr = (const uint8_t *) (getWorkingPageAddr(currentPageId) + offset);
??handlePageReadCommand_2:
        UXTH     R8,R8            ;; ZeroExt  R8,R8,#+16,#+16
        LDR.W    R0,??DataTable24_3
        LDRSH    R0,[R0, #+0]
          CFI FunCall _Z18getWorkingPageAddri
        BL       _Z18getWorkingPageAddri
        ADDS     R10,R8,R0
//  357 	tsSendResponse(tsChannel, mode, addr, count);
        UXTH     R4,R4            ;; ZeroExt  R4,R4,#+16,#+16
        MOVS     R3,R4
        MOV      R2,R10
        MOVS     R1,R6
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R5
          CFI FunCall _Z14tsSendResponseP12ts_channel_s20ts_response_format_ePKhi
        BL       _Z14tsSendResponseP12ts_channel_s20ts_response_format_ePKhi
//  358 #if EFI_TUNER_STUDIO_VERBOSE
//  359 //	scheduleMsg(&tsLogger, "Sending %d done", count);
//  360 #endif
//  361 }
??handlePageReadCommand_1:
        POP      {R0,R1,R4-R10,PC}  ;; return
          CFI EndBlock cfiBlock15
//  362 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function _Z11requestBurnv
        THUMB
//  363 void requestBurn(void) {
_Z11requestBurnv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  364 #if EFI_INTERNAL_FLASH
//  365 	setNeedToWriteConfiguration();
          CFI FunCall _Z27setNeedToWriteConfigurationv
        BL       _Z27setNeedToWriteConfigurationv
//  366 #endif
//  367 	incrementGlobalConfigurationVersion();
          CFI FunCall _Z35incrementGlobalConfigurationVersionv
        BL       _Z35incrementGlobalConfigurationVersionv
//  368 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock16
//  369 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function _Z16sendResponseCode20ts_response_format_eP12ts_channel_sh
        THUMB
//  370 static void sendResponseCode(ts_response_format_e mode, ts_channel_s *tsChannel, const uint8_t responseCode) {
_Z16sendResponseCode20ts_response_format_eP12ts_channel_sh:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  371 	if (mode == TS_CRC) {
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+1
        BNE.N    ??sendResponseCode_0
//  372 		tunerStudioWriteCrcPacket(tsChannel, responseCode, NULL, 0);
        MOVS     R3,#+0
        MOVS     R2,#+0
        MOVS     R1,R6
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R5
          CFI FunCall _Z25tunerStudioWriteCrcPacketP12ts_channel_shPKvt
        BL       _Z25tunerStudioWriteCrcPacketP12ts_channel_shPKvt
//  373 	}
//  374 }
??sendResponseCode_0:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock17
//  375 
//  376 /**
//  377  * 'Burn' command is a command to commit the changes
//  378  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function _Z17handleBurnCommandP12ts_channel_s20ts_response_format_et
        THUMB
//  379 void handleBurnCommand(ts_channel_s *tsChannel, ts_response_format_e mode, uint16_t page) {
_Z17handleBurnCommandP12ts_channel_s20ts_response_format_et:
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
//  380 	efitimems_t nowMs = currentTimeMillis();
          CFI FunCall currentTimeMillis
        BL       currentTimeMillis
        MOVS     R7,R0
//  381 	tsState.burnCommandCounter++;
        LDR.W    R0,??DataTable22_1
        LDR      R0,[R0, #+12]
        ADDS     R0,R0,#+1
        LDR.W    R1,??DataTable22_1
        STR      R0,[R1, #+12]
//  382 
//  383 	scheduleMsg(&tsLogger, "got B (Burn) %s", mode == TS_PLAIN ? "plain" : "CRC");
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BNE.N    ??handleBurnCommand_0
        LDR.W    R2,??DataTable24_18
        B.N      ??handleBurnCommand_1
??handleBurnCommand_0:
        ADR.N    R2,??DataTable14  ;; "CRC"
??handleBurnCommand_1:
        LDR.W    R1,??DataTable24_19
        LDR.W    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  384 
//  385 	currentPageId = page;
        LDR.W    R0,??DataTable22_3
        STRH     R6,[R0, #+0]
//  386 
//  387 #if EFI_TUNER_STUDIO_VERBOSE
//  388 	// pointless since we only have one page now
//  389 //	scheduleMsg(logger, "Page number %d", currentPageId);
//  390 #endif
//  391 
//  392 // todo: how about some multi-threading?
//  393 	memcpy(&persistentState.persistentConfiguration, &configWorkingCopy, sizeof(persistent_config_s));
        MOVW     R2,#+15288
        LDR.W    R1,??DataTable23_1
        LDR.W    R0,??DataTable24_20
          CFI FunCall memcpy
        BL       memcpy
//  394 
//  395 	requestBurn();
          CFI FunCall _Z11requestBurnv
        BL       _Z11requestBurnv
//  396 	sendResponseCode(mode, tsChannel, TS_RESPONSE_BURN_OK);
        MOVS     R2,#+4
        MOVS     R1,R4
        MOVS     R0,R5
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z16sendResponseCode20ts_response_format_eP12ts_channel_sh
        BL       _Z16sendResponseCode20ts_response_format_eP12ts_channel_sh
//  397 	scheduleMsg(&tsLogger, "BURN in %dms", currentTimeMillis() - nowMs);
          CFI FunCall currentTimeMillis
        BL       currentTimeMillis
        SUBS     R2,R0,R7
        LDR.W    R1,??DataTable24_21
        LDR.W    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  398 }
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock18
//  399 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  400 static TunerStudioReadRequest readRequest;
readRequest:
        DS8 8

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  401 static TunerStudioWriteChunkRequest writeChunkRequest;
writeChunkRequest:
        DS8 8

        SECTION `.bss`:DATA:REORDER:NOROOT(1)
//  402 static short int pageIn;
pageIn:
        DS8 2
//  403 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock19 Using cfiCommon0
          CFI Function _Z14isKnownCommandc
          CFI NoCalls
        THUMB
//  404 static bool isKnownCommand(char command) {
//  405 	return command == TS_HELLO_COMMAND || command == TS_READ_COMMAND || command == TS_OUTPUT_COMMAND
//  406 			|| command == TS_PAGE_COMMAND || command == TS_BURN_COMMAND || command == TS_SINGLE_WRITE_COMMAND
//  407 			|| command == TS_LEGACY_HELLO_COMMAND || command == TS_CHUNK_WRITE_COMMAND || command == TS_EXECUTE
//  408 			|| command == TS_GET_TEXT || command == TS_CRC_CHECK_COMMAND;
_Z14isKnownCommandc:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+83
        BEQ.N    ??isKnownCommand_0
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+82
        BEQ.N    ??isKnownCommand_0
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+79
        BEQ.N    ??isKnownCommand_0
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+80
        BEQ.N    ??isKnownCommand_0
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+66
        BEQ.N    ??isKnownCommand_0
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+87
        BEQ.N    ??isKnownCommand_0
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+81
        BEQ.N    ??isKnownCommand_0
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+67
        BEQ.N    ??isKnownCommand_0
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+69
        BEQ.N    ??isKnownCommand_0
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+71
        BEQ.N    ??isKnownCommand_0
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+107
        BNE.N    ??isKnownCommand_1
??isKnownCommand_0:
        MOVS     R0,#+1
        B.N      ??isKnownCommand_2
??isKnownCommand_1:
        MOVS     R0,#+0
??isKnownCommand_2:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return
          CFI EndBlock cfiBlock19
//  409 }
//  410 

        SECTION `.bss`:DATA:REORDER:NOROOT(0)
//  411 static uint8_t firstByte;
firstByte:
        DS8 1

        SECTION `.bss`:DATA:REORDER:NOROOT(0)
//  412 static uint8_t secondByte;
secondByte:
        DS8 1
//  413 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock20 Using cfiCommon0
          CFI Function _Z21runBinaryProtocolLoopP12ts_channel_sb
        THUMB
//  414 void runBinaryProtocolLoop(ts_channel_s *tsChannel, bool_t isConsoleRedirect) {
_Z21runBinaryProtocolLoopP12ts_channel_sb:
        PUSH     {R1,R4-R11,LR}
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
        SUB      SP,SP,#+16
          CFI CFA R13+56
        MOVS     R4,R0
//  415 	int wasReady = false;
        MOVS     R0,#+0
        STR      R0,[SP, #+8]
//  416 	while (true) {
//  417 		int isReady = ts_serial_ready(isConsoleRedirect);
??runBinaryProtocolLoop_0:
        LDRB     R0,[SP, #+16]
          CFI FunCall _Z15ts_serial_readyb
        BL       _Z15ts_serial_readyb
        MOVS     R5,R0
//  418 		if (!isReady) {
        CMP      R5,#+0
        BNE.N    ??runBinaryProtocolLoop_1
//  419 			chThdSleepMilliseconds(10);
        MOVS     R0,#+10
          CFI FunCall chThdSleep
        BL       chThdSleep
//  420 			wasReady = false;
        MOVS     R0,#+0
        STR      R0,[SP, #+8]
//  421 			continue;
        B.N      ??runBinaryProtocolLoop_0
//  422 		}
//  423 
//  424 		if (!wasReady) {
??runBinaryProtocolLoop_1:
        LDR      R0,[SP, #+8]
        CMP      R0,#+0
        BNE.N    ??runBinaryProtocolLoop_2
//  425 			wasReady = true;
        MOVS     R0,#+1
        STR      R0,[SP, #+8]
//  426 //			scheduleSimpleMsg(&logger, "ts channel is now ready ", hTimeNow());
//  427 		}
//  428 
//  429 		tsState.tsCounter++;
??runBinaryProtocolLoop_2:
        LDR.W    R0,??DataTable24_2
        LDR      R0,[R0, #+36]
        ADDS     R0,R0,#+1
        LDR.W    R1,??DataTable24_2
        STR      R0,[R1, #+36]
//  430 
//  431 		int recieved = chnReadTimeout(tsChannel->channel, &firstByte, 1, TS_READ_TIMEOUT);
        MOVW     R3,#+3000
        MOVS     R2,#+1
        LDR.W    R1,??DataTable24_22
        LDR      R0,[R4, #+0]
        LDR      R6,[R4, #+0]
        LDR      R6,[R6, #+0]
        LDR      R6,[R6, #+28]
          CFI FunCall
        BLX      R6
        MOVS     R6,R0
//  432 		if (recieved != 1) {
        CMP      R6,#+1
        BNE.N    ??runBinaryProtocolLoop_0
//  433 //			tunerStudioError("ERROR: no command");
//  434 			continue;
//  435 		}
//  436 //		scheduleMsg(logger, "Got first=%x=[%c]", firstByte, firstByte);
//  437 		if (handlePlainCommand(tsChannel, firstByte))
??runBinaryProtocolLoop_3:
        LDR.W    R0,??DataTable24_22
        LDRB     R1,[R0, #+0]
        MOVS     R0,R4
          CFI FunCall _Z18handlePlainCommandP12ts_channel_sh
        BL       _Z18handlePlainCommandP12ts_channel_sh
        CMP      R0,#+0
        BNE.N    ??runBinaryProtocolLoop_0
//  438 			continue;
//  439 
//  440 		recieved = chnReadTimeout(tsChannel->channel, &secondByte, 1, TS_READ_TIMEOUT);
??runBinaryProtocolLoop_4:
        MOVW     R3,#+3000
        MOVS     R2,#+1
        LDR.W    R1,??DataTable24_23
        LDR      R0,[R4, #+0]
        LDR      R7,[R4, #+0]
        LDR      R7,[R7, #+0]
        LDR      R7,[R7, #+28]
          CFI FunCall
        BLX      R7
        MOVS     R6,R0
//  441 		if (recieved != 1) {
        CMP      R6,#+1
        BEQ.N    ??runBinaryProtocolLoop_5
//  442 			tunerStudioError("ERROR: no second");
        LDR.W    R0,??DataTable24_24
          CFI FunCall _Z16tunerStudioErrorPKc
        BL       _Z16tunerStudioErrorPKc
//  443 			continue;
        B.N      ??runBinaryProtocolLoop_0
//  444 		}
//  445 //		scheduleMsg(logger, "Got secondByte=%x=[%c]", secondByte, secondByte);
//  446 
//  447 		uint32_t incomingPacketSize = firstByte * 256 + secondByte;
??runBinaryProtocolLoop_5:
        LDR.W    R0,??DataTable24_22
        LDRB     R0,[R0, #+0]
        MOV      R1,#+256
        LDR.W    R2,??DataTable24_23
        LDRB     R2,[R2, #+0]
        MLA      R7,R1,R0,R2
//  448 
//  449 		if (incomingPacketSize == BINARY_SWITCH_TAG) {
        MOVW     R0,#+32266
        CMP      R7,R0
        BNE.N    ??runBinaryProtocolLoop_6
//  450 			// we are here if we get a binary switch request while already in binary mode. We will just ignore it.
//  451 			tunerStudioWriteData(tsChannel, (const uint8_t *) &BINARY_RESPONSE, 2);
        MOVS     R2,#+2
        LDR.W    R1,??DataTable24_25
        MOVS     R0,R4
          CFI FunCall _Z20tunerStudioWriteDataP12ts_channel_sPKhi
        BL       _Z20tunerStudioWriteDataP12ts_channel_sPKhi
//  452 			continue;
        B.N      ??runBinaryProtocolLoop_0
//  453 		}
//  454 
//  455 		if (incomingPacketSize == 0 || incomingPacketSize > (sizeof(tsChannel->crcReadBuffer) - CRC_WRAPPING_SIZE)) {
??runBinaryProtocolLoop_6:
        CMP      R7,#+0
        BEQ.N    ??runBinaryProtocolLoop_7
        CMP      R7,#+294
        BCC.N    ??runBinaryProtocolLoop_8
//  456 			scheduleMsg(&tsLogger, "TunerStudio: invalid size: %d", incomingPacketSize);
??runBinaryProtocolLoop_7:
        MOVS     R2,R7
        LDR.W    R1,??DataTable24_26
        LDR.W    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  457 			tunerStudioError("ERROR: CRC header size");
        LDR.W    R0,??DataTable24_27
          CFI FunCall _Z16tunerStudioErrorPKc
        BL       _Z16tunerStudioErrorPKc
//  458 			sendErrorCode(tsChannel);
        MOVS     R0,R4
          CFI FunCall _Z13sendErrorCodeP12ts_channel_s
        BL       _Z13sendErrorCodeP12ts_channel_s
//  459 			continue;
        B.N      ??runBinaryProtocolLoop_0
//  460 		}
//  461 
//  462 		recieved = chnReadTimeout(tsChannel->channel, (uint8_t* )tsChannel->crcReadBuffer, 1, TS_READ_TIMEOUT);
??runBinaryProtocolLoop_8:
        MOVW     R3,#+3000
        MOVS     R2,#+1
        ADDS     R1,R4,#+8
        LDR      R0,[R4, #+0]
        LDR      R12,[R4, #+0]
        LDR      R12,[R12, #+0]
        LDR      R12,[R12, #+28]
          CFI FunCall
        BLX      R12
        MOVS     R6,R0
//  463 		if (recieved != 1) {
        CMP      R6,#+1
        BEQ.N    ??runBinaryProtocolLoop_9
//  464 			tunerStudioError("ERROR: did not receive command");
        LDR.W    R0,??DataTable24_28
          CFI FunCall _Z16tunerStudioErrorPKc
        BL       _Z16tunerStudioErrorPKc
//  465 			continue;
        B.N      ??runBinaryProtocolLoop_0
//  466 		}
//  467 
//  468 		char command = tsChannel->crcReadBuffer[0];
??runBinaryProtocolLoop_9:
        LDRB     R8,[R4, #+8]
//  469 		if (!isKnownCommand(command)) {
        MOV      R0,R8
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z14isKnownCommandc
        BL       _Z14isKnownCommandc
        CMP      R0,#+0
        BNE.N    ??runBinaryProtocolLoop_10
//  470 			scheduleMsg(&tsLogger, "unexpected command %x", command);
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        MOV      R2,R8
        LDR.W    R1,??DataTable24_29
        LDR.W    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  471 			sendErrorCode(tsChannel);
        MOVS     R0,R4
          CFI FunCall _Z13sendErrorCodeP12ts_channel_s
        BL       _Z13sendErrorCodeP12ts_channel_s
//  472 			continue;
        B.N      ??runBinaryProtocolLoop_0
//  473 		}
//  474 
//  475 //		scheduleMsg(logger, "TunerStudio: reading %d+4 bytes(s)", incomingPacketSize);
//  476 
//  477 		recieved = chnReadTimeout(tsChannel->channel, (uint8_t * ) (tsChannel->crcReadBuffer + 1),
//  478 				incomingPacketSize + CRC_VALUE_SIZE - 1, TS_READ_TIMEOUT);
??runBinaryProtocolLoop_10:
        MOVW     R3,#+3000
        ADDS     R2,R7,#+3
        ADDS     R1,R4,#+9
        LDR      R0,[R4, #+0]
        LDR      R12,[R4, #+0]
        LDR      R12,[R12, #+0]
        LDR      R12,[R12, #+28]
          CFI FunCall
        BLX      R12
        MOVS     R6,R0
//  479 		int expectedSize = incomingPacketSize + CRC_VALUE_SIZE - 1;
        ADDS     R0,R7,#+3
        STR      R0,[SP, #+12]
//  480 		if (recieved != expectedSize) {
        LDR      R0,[SP, #+12]
        CMP      R6,R0
        BEQ.N    ??runBinaryProtocolLoop_11
//  481 			scheduleMsg(&tsLogger, "Got only %d bytes while expecting %d for command %c", recieved,
//  482 					expectedSize, command);
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        STR      R8,[SP, #+0]
        LDR      R3,[SP, #+12]
        MOVS     R2,R6
        LDR.W    R1,??DataTable24_30
        LDR.W    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  483 			tunerStudioError("ERROR: not enough bytes in stream");
        LDR.W    R0,??DataTable24_31
          CFI FunCall _Z16tunerStudioErrorPKc
        BL       _Z16tunerStudioErrorPKc
//  484 			sendResponseCode(TS_CRC, tsChannel, TS_RESPONSE_UNDERRUN);
        MOVS     R2,#+128
        MOVS     R1,R4
        MOVS     R0,#+1
          CFI FunCall _Z16sendResponseCode20ts_response_format_eP12ts_channel_sh
        BL       _Z16sendResponseCode20ts_response_format_eP12ts_channel_sh
//  485 			continue;
        B.N      ??runBinaryProtocolLoop_0
//  486 		}
//  487 
//  488 		uint32_t expectedCrc = *(uint32_t*) (tsChannel->crcReadBuffer + incomingPacketSize);
??runBinaryProtocolLoop_11:
        ADDS     R0,R7,R4
        LDR      R9,[R0, #+8]
//  489 
//  490 		expectedCrc = SWAP_UINT32(expectedCrc);
        LSLS     R0,R9,#+8
        ANDS     R0,R0,#0xFF0000
        ORRS     R0,R0,R9, LSR #+24
        LSRS     R1,R9,#+8
        ANDS     R1,R1,#0xFF00
        ORRS     R0,R1,R0
        ORRS     R9,R0,R9, LSL #+24
//  491 
//  492 		uint32_t actualCrc = crc32(tsChannel->crcReadBuffer, incomingPacketSize);
        MOVS     R1,R7
        ADDS     R0,R4,#+8
          CFI FunCall crc32
        BL       crc32
        MOV      R10,R0
//  493 		if (actualCrc != expectedCrc) {
        CMP      R10,R9
        BEQ.N    ??runBinaryProtocolLoop_12
//  494 			scheduleMsg(&tsLogger, "TunerStudio: CRC %x %x %x %x", tsChannel->crcReadBuffer[incomingPacketSize + 0],
//  495 					tsChannel->crcReadBuffer[incomingPacketSize + 1], tsChannel->crcReadBuffer[incomingPacketSize + 2],
//  496 					tsChannel->crcReadBuffer[incomingPacketSize + 3]);
        ADDS     R0,R7,R4
        LDRB     R0,[R0, #+11]
        STR      R0,[SP, #+4]
        ADDS     R0,R7,R4
        LDRB     R0,[R0, #+10]
        STR      R0,[SP, #+0]
        ADDS     R0,R7,R4
        LDRB     R3,[R0, #+9]
        ADDS     R0,R7,R4
        LDRB     R2,[R0, #+8]
        LDR.W    R1,??DataTable24_32
        LDR.W    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  497 
//  498 			scheduleMsg(&tsLogger, "TunerStudio: command %c actual CRC %x/expected %x", tsChannel->crcReadBuffer[0],
//  499 					actualCrc, expectedCrc);
        STR      R9,[SP, #+0]
        MOV      R3,R10
        LDRB     R2,[R4, #+8]
        LDR.W    R1,??DataTable24_33
        LDR.W    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  500 			tunerStudioError("ERROR: CRC issue");
        LDR.W    R0,??DataTable24_34
          CFI FunCall _Z16tunerStudioErrorPKc
        BL       _Z16tunerStudioErrorPKc
//  501 			continue;
        B.N      ??runBinaryProtocolLoop_0
//  502 		}
//  503 
//  504 //		scheduleMsg(logger, "TunerStudio: P00-07 %x %x %x %x %x %x %x %x", crcIoBuffer[0], crcIoBuffer[1],
//  505 //				crcIoBuffer[2], crcIoBuffer[3], crcIoBuffer[4], crcIoBuffer[5], crcIoBuffer[6], crcIoBuffer[7]);
//  506 
//  507 		int success = tunerStudioHandleCrcCommand(tsChannel, tsChannel->crcReadBuffer, incomingPacketSize);
??runBinaryProtocolLoop_12:
        MOVS     R2,R7
        ADDS     R1,R4,#+8
        MOVS     R0,R4
          CFI FunCall _Z27tunerStudioHandleCrcCommandP12ts_channel_sPci
        BL       _Z27tunerStudioHandleCrcCommandP12ts_channel_sPci
        MOV      R11,R0
//  508 		if (!success)
        CMP      R11,#+0
        BNE.W    ??runBinaryProtocolLoop_0
//  509 			print("got unexpected TunerStudio command %x:%c\r\n", command, command);
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        MOV      R2,R8
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        MOV      R1,R8
        LDR.W    R0,??DataTable24_35
          CFI FunCall print
        BL       print
        B.N      ??runBinaryProtocolLoop_0
          CFI EndBlock cfiBlock20
//  510 
//  511 	}
//  512 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14:
        DC8      "CRC"
//  513 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock21 Using cfiCommon0
          CFI Function _Z18tsThreadEntryPointPv
        THUMB
//  514 static msg_t tsThreadEntryPoint(void *arg) {
_Z18tsThreadEntryPointPv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  515 	(void) arg;
//  516 	chRegSetThreadName("tunerstudio thread");
        LDR.W    R0,??DataTable24_36
        LDR      R0,[R0, #+24]
        LDR.W    R1,??DataTable24_37
        STR      R1,[R0, #+24]
//  517 
//  518 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//  519 	startTsPort();
          CFI FunCall _Z11startTsPortv
        BL       _Z11startTsPortv
//  520 #endif
//  521 
//  522 	runBinaryProtocolLoop(&tsChannel, false);
        MOVS     R1,#+0
        LDR.W    R0,??DataTable24_38
          CFI FunCall _Z21runBinaryProtocolLoopP12ts_channel_sb
        BL       _Z21runBinaryProtocolLoopP12ts_channel_sb
//  523 
//  524 #if defined __GNUC__
//  525 	return 0;
//  526 #endif
//  527 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock21
//  528 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock22 Using cfiCommon0
          CFI Function _Z19syncTunerStudioCopyv
        THUMB
//  529 void syncTunerStudioCopy(void) {
_Z19syncTunerStudioCopyv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  530 	memcpy(&configWorkingCopy, &persistentState.persistentConfiguration, sizeof(persistent_config_s));
        MOVW     R2,#+15288
        LDR.W    R1,??DataTable24_20
        LDR.W    R0,??DataTable23_1
          CFI FunCall memcpy
        BL       memcpy
//  531 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock22
//  532 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  533 tunerstudio_counters_s tsState;
tsState:
        DS8 40

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  534 TunerStudioOutputChannels tsOutputChannels;
tsOutputChannels:
        DS8 196
//  535 /**
//  536  * this is a local copy of the configuration. Any changes to this copy
//  537  * have no effect until this copy is explicitly propagated to the main working copy
//  538  */

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  539 persistent_config_s configWorkingCopy;
configWorkingCopy:
        DS8 15288
//  540 

        SECTION `.bss`:DATA:REORDER:NOROOT(1)
//  541 short currentPageId;
currentPageId:
        DS8 2
//  542 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock23 Using cfiCommon0
          CFI Function _Z16tunerStudioErrorPKc
        THUMB
//  543 void tunerStudioError(const char *msg) {
_Z16tunerStudioErrorPKc:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  544 	tunerStudioDebug(msg);
        MOVS     R0,R4
          CFI FunCall _Z16tunerStudioDebugPKc
        BL       _Z16tunerStudioDebugPKc
//  545 	printErrorCounters();
          CFI FunCall _Z18printErrorCountersv
        BL       _Z18printErrorCountersv
//  546 	tsState.errorCounter++;
        LDR.W    R0,??DataTable24_2
        LDR      R0,[R0, #+32]
        ADDS     R0,R0,#+1
        LDR.W    R1,??DataTable24_2
        STR      R0,[R1, #+32]
//  547 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock23
//  548 
//  549 /**
//  550  * Query with CRC takes place while re-establishing connection
//  551  * Query without CRC takes place on TunerStudio startup
//  552  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock24 Using cfiCommon0
          CFI Function _Z18handleQueryCommandP12ts_channel_s20ts_response_format_e
        THUMB
//  553 void handleQueryCommand(ts_channel_s *tsChannel, ts_response_format_e mode) {
_Z18handleQueryCommandP12ts_channel_s20ts_response_format_e:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  554 	tsState.queryCommandCounter++;
        LDR.N    R0,??DataTable22_1
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable22_1
        STR      R0,[R1, #+0]
//  555 #if EFI_TUNER_STUDIO_VERBOSE
//  556 	scheduleMsg(&tsLogger, "got S/H (queryCommand) mode=%d", mode);
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        MOVS     R2,R5
        LDR.W    R1,??DataTable24_39
        LDR.W    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  557 	printTsStats();
          CFI FunCall _Z12printTsStatsv
        BL       _Z12printTsStatsv
//  558 #endif
//  559 	tsSendResponse(tsChannel, mode, (const uint8_t *) TS_SIGNATURE, strlen(TS_SIGNATURE) + 1);
        MOVS     R0,#+1
        ADDS     R3,R0,#+12
        LDR.W    R2,??DataTable24_40
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall _Z14tsSendResponseP12ts_channel_s20ts_response_format_ePKhi
        BL       _Z14tsSendResponseP12ts_channel_s20ts_response_format_ePKhi
//  560 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock24
//  561 
//  562 /**
//  563  * @brief 'Output' command sends out a snapshot of current values
//  564  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock25 Using cfiCommon0
          CFI Function _Z27handleOutputChannelsCommandP12ts_channel_s20ts_response_format_e
        THUMB
//  565 void handleOutputChannelsCommand(ts_channel_s *tsChannel, ts_response_format_e mode) {
_Z27handleOutputChannelsCommandP12ts_channel_s20ts_response_format_e:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  566 	tsState.outputChannelsCommandCounter++;
        LDR.N    R0,??DataTable22_1
        LDR      R0,[R0, #+4]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable22_1
        STR      R0,[R1, #+4]
//  567 	prepareTunerStudioOutputs();
          CFI FunCall _Z25prepareTunerStudioOutputsv
        BL       _Z25prepareTunerStudioOutputsv
//  568 	// this method is invoked too often to print any debug information
//  569 	tsSendResponse(tsChannel, mode, (const uint8_t *) &tsOutputChannels, sizeof(TunerStudioOutputChannels));
        MOVS     R3,#+196
        LDR.W    R2,??DataTable24_41
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall _Z14tsSendResponseP12ts_channel_s20ts_response_format_ePKhi
        BL       _Z14tsSendResponseP12ts_channel_s20ts_response_format_ePKhi
//  570 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock25
//  571 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock26 Using cfiCommon0
          CFI Function _Z17handleTestCommandP12ts_channel_s
        THUMB
//  572 void handleTestCommand(ts_channel_s *tsChannel) {
_Z17handleTestCommandP12ts_channel_s:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  573 	/**
//  574 	 * this is NOT a standard TunerStudio command, this is my own
//  575 	 * extension of the protocol to simplify troubleshooting
//  576 	 */
//  577 	tunerStudioDebug("got T (Test)");
        LDR.W    R0,??DataTable24_42
          CFI FunCall _Z16tunerStudioDebugPKc
        BL       _Z16tunerStudioDebugPKc
//  578 	tunerStudioWriteData(tsChannel, (const uint8_t *) VCS_VERSION, sizeof(VCS_VERSION));
        MOVS     R2,#+5
        LDR.W    R1,??DataTable24_43
        MOVS     R0,R4
          CFI FunCall _Z20tunerStudioWriteDataP12ts_channel_sPKhi
        BL       _Z20tunerStudioWriteDataP12ts_channel_sPKhi
//  579 	/**
//  580 	 * Please note that this response is a magic constant used by dev console for protocol detection
//  581 	 * @see EngineState#TS_PROTOCOL_TAG
//  582 	 */
//  583 	tunerStudioWriteData(tsChannel, (const uint8_t *) " ts_p_alive\r\n", 8);
        MOVS     R2,#+8
        LDR.W    R1,??DataTable24_44
        MOVS     R0,R4
          CFI FunCall _Z20tunerStudioWriteDataP12ts_channel_sPKhi
        BL       _Z20tunerStudioWriteDataP12ts_channel_sPKhi
//  584 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock26
//  585 
//  586 extern CommandHandler console_line_callback;
//  587 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock27 Using cfiCommon0
          CFI Function _Z13handleGetTextP12ts_channel_s
        THUMB
//  588 static void handleGetText(ts_channel_s *tsChannel) {
_Z13handleGetTextP12ts_channel_s:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  589 	int outputSize;
//  590 	char *output = swapOutputBuffers(&outputSize);
        ADD      R0,SP,#+0
          CFI FunCall _Z17swapOutputBuffersPi
        BL       _Z17swapOutputBuffersPi
        MOVS     R5,R0
//  591 
//  592 	tunerStudioWriteCrcPacket(tsChannel, TS_RESPONSE_COMMAND_OK, output, outputSize);
        LDR      R3,[SP, #+0]
        UXTH     R3,R3            ;; ZeroExt  R3,R3,#+16,#+16
        MOVS     R2,R5
        MOVS     R1,#+7
        MOVS     R0,R4
          CFI FunCall _Z25tunerStudioWriteCrcPacketP12ts_channel_shPKvt
        BL       _Z25tunerStudioWriteCrcPacketP12ts_channel_shPKvt
//  593 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock27
//  594 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock28 Using cfiCommon0
          CFI Function _Z20handleExecuteCommandP12ts_channel_sPci
        THUMB
//  595 static void handleExecuteCommand(ts_channel_s *tsChannel, char *data, int incomingPacketSize) {
_Z20handleExecuteCommandP12ts_channel_sPci:
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
//  596 	tunerStudioWriteCrcPacket(tsChannel, TS_RESPONSE_COMMAND_OK, NULL, 0);
        MOVS     R3,#+0
        MOVS     R2,#+0
        MOVS     R1,#+7
        MOVS     R0,R4
          CFI FunCall _Z25tunerStudioWriteCrcPacketP12ts_channel_shPKvt
        BL       _Z25tunerStudioWriteCrcPacketP12ts_channel_shPKvt
//  597 	data[incomingPacketSize] = 0;
        MOVS     R0,#+0
        STRB     R0,[R6, R5]
//  598 	char *trimmed = efiTrim(data);
        MOVS     R0,R5
          CFI FunCall efiTrim
        BL       efiTrim
        MOVS     R7,R0
//  599 	(console_line_callback)(trimmed);
        MOVS     R0,R7
        LDR.W    R1,??DataTable24_45
        LDR      R1,[R1, #+0]
          CFI FunCall
        BLX      R1
//  600 }
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock28
//  601 
//  602 /**
//  603  * @return true if legacy command was processed, false otherwise
//  604  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock29 Using cfiCommon0
          CFI Function _Z18handlePlainCommandP12ts_channel_sh
        THUMB
//  605 bool handlePlainCommand(ts_channel_s *tsChannel, uint8_t command) {
_Z18handlePlainCommandP12ts_channel_sh:
        PUSH     {R4-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+20
        SUB      SP,SP,#+12
          CFI CFA R13+32
        MOVS     R4,R0
        MOVS     R5,R1
//  606 	if (command == TS_HELLO_COMMAND || command == TS_HELLO_COMMAND_DEPRECATED) {
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+83
        BEQ.N    ??handlePlainCommand_0
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+72
        BNE.N    ??handlePlainCommand_1
//  607 		scheduleMsg(&tsLogger, "Got naked Query command");
??handlePlainCommand_0:
        LDR.W    R1,??DataTable24_46
        LDR.W    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  608 		handleQueryCommand(tsChannel, TS_PLAIN);
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall _Z18handleQueryCommandP12ts_channel_s20ts_response_format_e
        BL       _Z18handleQueryCommandP12ts_channel_s20ts_response_format_e
//  609 		return true;
        MOVS     R0,#+1
        B.N      ??handlePlainCommand_2
//  610 	} else if (command == 't' || command == 'T') {
??handlePlainCommand_1:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+116
        BEQ.N    ??handlePlainCommand_3
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+84
        BNE.N    ??handlePlainCommand_4
//  611 		handleTestCommand(tsChannel);
??handlePlainCommand_3:
        MOVS     R0,R4
          CFI FunCall _Z17handleTestCommandP12ts_channel_s
        BL       _Z17handleTestCommandP12ts_channel_s
//  612 		return true;
        MOVS     R0,#+1
        B.N      ??handlePlainCommand_2
//  613 	} else if (command == TS_PAGE_COMMAND) {
??handlePlainCommand_4:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+80
        BNE.N    ??handlePlainCommand_5
//  614 		int recieved = chnReadTimeout(tsChannel->channel, (uint8_t * )&pageIn, sizeof(pageIn), TS_READ_TIMEOUT);
        MOVW     R3,#+3000
        MOVS     R2,#+2
        LDR.W    R1,??DataTable24_47
        LDR      R0,[R4, #+0]
        LDR      R6,[R4, #+0]
        LDR      R6,[R6, #+0]
        LDR      R6,[R6, #+28]
          CFI FunCall
        BLX      R6
        MOVS     R6,R0
//  615 		if (recieved != sizeof(pageIn)) {
        CMP      R6,#+2
        BEQ.N    ??handlePlainCommand_6
//  616 			tunerStudioError("ERROR: not enough for PAGE");
        LDR.W    R0,??DataTable24_48
          CFI FunCall _Z16tunerStudioErrorPKc
        BL       _Z16tunerStudioErrorPKc
//  617 			return true;
        MOVS     R0,#+1
        B.N      ??handlePlainCommand_2
//  618 		}
//  619 		handlePageSelectCommand(tsChannel, TS_PLAIN, pageIn);
??handlePlainCommand_6:
        LDR.W    R0,??DataTable24_47
        LDRH     R2,[R0, #+0]
        UXTH     R2,R2            ;; ZeroExt  R2,R2,#+16,#+16
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall _Z23handlePageSelectCommandP12ts_channel_s20ts_response_format_et
        BL       _Z23handlePageSelectCommandP12ts_channel_s20ts_response_format_et
//  620 		return true;
        MOVS     R0,#+1
        B.N      ??handlePlainCommand_2
//  621 	} else if (command == TS_BURN_COMMAND) {
??handlePlainCommand_5:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+66
        BNE.N    ??handlePlainCommand_7
//  622 		scheduleMsg(&tsLogger, "Got naked BURN");
        LDR.W    R1,??DataTable24_49
        LDR.N    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  623 		uint16_t page;
//  624 		int recieved = chnReadTimeout(tsChannel->channel, (uint8_t * )&page, sizeof(page), TS_READ_TIMEOUT);
        MOVW     R3,#+3000
        MOVS     R2,#+2
        ADD      R1,SP,#+4
        LDR      R0,[R4, #+0]
        LDR      R6,[R4, #+0]
        LDR      R6,[R6, #+0]
        LDR      R6,[R6, #+28]
          CFI FunCall
        BLX      R6
        MOVS     R6,R0
//  625 		if (recieved != sizeof(page)) {
        CMP      R6,#+2
        BEQ.N    ??handlePlainCommand_8
//  626 			tunerStudioError("ERROR: Not enough for plain burn");
        LDR.W    R0,??DataTable24_50
          CFI FunCall _Z16tunerStudioErrorPKc
        BL       _Z16tunerStudioErrorPKc
//  627 			return true;
        MOVS     R0,#+1
        B.N      ??handlePlainCommand_2
//  628 		}
//  629 		handleBurnCommand(tsChannel, TS_PLAIN, page);
??handlePlainCommand_8:
        LDRH     R2,[SP, #+4]
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall _Z17handleBurnCommandP12ts_channel_s20ts_response_format_et
        BL       _Z17handleBurnCommandP12ts_channel_s20ts_response_format_et
//  630 		return true;
        MOVS     R0,#+1
        B.N      ??handlePlainCommand_2
//  631 	} else if (command == TS_CHUNK_WRITE_COMMAND) {
??handlePlainCommand_7:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+67
        BNE.N    ??handlePlainCommand_9
//  632 		scheduleMsg(&tsLogger, "Got naked CHUNK_WRITE");
        LDR.W    R1,??DataTable24_51
        LDR.N    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  633 		int recieved = chnReadTimeout(tsChannel->channel, (uint8_t * )&writeChunkRequest, sizeof(writeChunkRequest),
//  634 				TS_READ_TIMEOUT);
        MOVW     R3,#+3000
        MOVS     R2,#+6
        LDR.W    R1,??DataTable24_52
        LDR      R0,[R4, #+0]
        LDR      R6,[R4, #+0]
        LDR      R6,[R6, #+0]
        LDR      R6,[R6, #+28]
          CFI FunCall
        BLX      R6
        MOVS     R6,R0
//  635 		if (recieved != sizeof(writeChunkRequest)) {
        CMP      R6,#+6
        BEQ.N    ??handlePlainCommand_10
//  636 			scheduleMsg(&tsLogger, "ERROR: Not enough for plain chunk write header: %d", recieved);
        MOVS     R2,R6
        LDR.W    R1,??DataTable24_53
        LDR.N    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  637 			tsState.errorCounter++;
        LDR.N    R0,??DataTable22_1
        LDR      R0,[R0, #+32]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable22_1
        STR      R0,[R1, #+32]
//  638 			return true;
        MOVS     R0,#+1
        B.N      ??handlePlainCommand_2
//  639 		}
//  640 		recieved = chnReadTimeout(tsChannel->channel, (uint8_t * )&tsChannel->crcReadBuffer, writeChunkRequest.count,
//  641 				TS_READ_TIMEOUT);
??handlePlainCommand_10:
        MOVW     R3,#+3000
        LDR.W    R0,??DataTable24_52
        LDRSH    R2,[R0, #+4]
        ADDS     R1,R4,#+8
        LDR      R0,[R4, #+0]
        LDR      R7,[R4, #+0]
        LDR      R7,[R7, #+0]
        LDR      R7,[R7, #+28]
          CFI FunCall
        BLX      R7
        MOVS     R6,R0
//  642 		if (recieved != writeChunkRequest.count) {
        LDR.W    R0,??DataTable24_52
        LDRSH    R0,[R0, #+4]
        CMP      R6,R0
        BEQ.N    ??handlePlainCommand_11
//  643 			scheduleMsg(&tsLogger, "ERROR: Not enough for plain chunk write content: %d while expecting %d", recieved,
//  644 					writeChunkRequest.count);
        LDR.W    R0,??DataTable24_52
        LDRSH    R3,[R0, #+4]
        MOVS     R2,R6
        LDR.W    R1,??DataTable24_54
        LDR.N    R0,??DataTable23
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  645 			tsState.errorCounter++;
        LDR.N    R0,??DataTable22_1
        LDR      R0,[R0, #+32]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable22_1
        STR      R0,[R1, #+32]
//  646 			return true;
        MOVS     R0,#+1
        B.N      ??handlePlainCommand_2
//  647 		}
//  648 		currentPageId = writeChunkRequest.page;
??handlePlainCommand_11:
        LDR.N    R0,??DataTable22_3
        LDR.W    R1,??DataTable24_52
        LDRH     R1,[R1, #+0]
        STRH     R1,[R0, #+0]
//  649 
//  650 		handleWriteChunkCommand(tsChannel, TS_PLAIN, writeChunkRequest.offset, writeChunkRequest.count,
//  651 				(uint8_t *) &tsChannel->crcReadBuffer);
        ADDS     R0,R4,#+8
        STR      R0,[SP, #+0]
        LDR.W    R0,??DataTable24_52
        LDRSH    R3,[R0, #+4]
        LDR.W    R0,??DataTable24_52
        LDRSH    R2,[R0, #+2]
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall _Z23handleWriteChunkCommandP12ts_channel_s20ts_response_format_essPv
        BL       _Z23handleWriteChunkCommandP12ts_channel_s20ts_response_format_essPv
//  652 		return true;
        MOVS     R0,#+1
        B.N      ??handlePlainCommand_2
//  653 	} else if (command == TS_READ_COMMAND) {
??handlePlainCommand_9:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+82
        BNE.N    ??handlePlainCommand_12
//  654 		//scheduleMsg(logger, "Got naked READ PAGE???");
//  655 		int recieved = chnReadTimeout(tsChannel->channel, (uint8_t * )&readRequest, sizeof(readRequest),
//  656 				TS_READ_TIMEOUT);
        MOVW     R3,#+3000
        MOVS     R2,#+6
        LDR.W    R1,??DataTable24_55
        LDR      R0,[R4, #+0]
        LDR      R6,[R4, #+0]
        LDR      R6,[R6, #+0]
        LDR      R6,[R6, #+28]
          CFI FunCall
        BLX      R6
        MOVS     R6,R0
//  657 		if (recieved != sizeof(readRequest)) {
        CMP      R6,#+6
        BEQ.N    ??handlePlainCommand_13
//  658 			tunerStudioError("Not enough for plain read header");
        LDR.N    R0,??DataTable24_56
          CFI FunCall _Z16tunerStudioErrorPKc
        BL       _Z16tunerStudioErrorPKc
//  659 			return true;
        MOVS     R0,#+1
        B.N      ??handlePlainCommand_2
//  660 		}
//  661 		handlePageReadCommand(tsChannel, TS_PLAIN, readRequest.page, readRequest.offset, readRequest.count);
??handlePlainCommand_13:
        LDR.N    R0,??DataTable24_55
        LDRH     R0,[R0, #+4]
        UXTH     R0,R0            ;; ZeroExt  R0,R0,#+16,#+16
        STR      R0,[SP, #+0]
        LDR.N    R0,??DataTable24_55
        LDRH     R3,[R0, #+2]
        UXTH     R3,R3            ;; ZeroExt  R3,R3,#+16,#+16
        LDR.N    R0,??DataTable24_55
        LDRH     R2,[R0, #+0]
        UXTH     R2,R2            ;; ZeroExt  R2,R2,#+16,#+16
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall _Z21handlePageReadCommandP12ts_channel_s20ts_response_format_ettt
        BL       _Z21handlePageReadCommandP12ts_channel_s20ts_response_format_ettt
//  662 		return true;
        MOVS     R0,#+1
        B.N      ??handlePlainCommand_2
//  663 	} else if (command == TS_OUTPUT_COMMAND) {
??handlePlainCommand_12:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+79
        BNE.N    ??handlePlainCommand_14
//  664 		//scheduleMsg(logger, "Got naked Channels???");
//  665 		handleOutputChannelsCommand(tsChannel, TS_PLAIN);
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall _Z27handleOutputChannelsCommandP12ts_channel_s20ts_response_format_e
        BL       _Z27handleOutputChannelsCommandP12ts_channel_s20ts_response_format_e
//  666 		return true;
        MOVS     R0,#+1
        B.N      ??handlePlainCommand_2
//  667 	} else if (command == TS_LEGACY_HELLO_COMMAND) {
??handlePlainCommand_14:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+81
        BNE.N    ??handlePlainCommand_15
//  668 		tunerStudioDebug("ignoring LEGACY_HELLO_COMMAND");
        LDR.N    R0,??DataTable24_57
          CFI FunCall _Z16tunerStudioDebugPKc
        BL       _Z16tunerStudioDebugPKc
//  669 		return true;
        MOVS     R0,#+1
        B.N      ??handlePlainCommand_2
//  670 	} else if (command == TS_COMMAND_F) {
??handlePlainCommand_15:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+70
        BNE.N    ??handlePlainCommand_16
//  671 		tunerStudioDebug("not ignoring F");
        LDR.N    R0,??DataTable24_58
          CFI FunCall _Z16tunerStudioDebugPKc
        BL       _Z16tunerStudioDebugPKc
//  672 		tunerStudioWriteData(tsChannel, (const uint8_t *) PROTOCOL, strlen(PROTOCOL));
        MOVS     R2,#+3
        ADR.N    R1,??DataTable24  ;; "001"
        MOVS     R0,R4
          CFI FunCall _Z20tunerStudioWriteDataP12ts_channel_sPKhi
        BL       _Z20tunerStudioWriteDataP12ts_channel_sPKhi
//  673 		return true;
        MOVS     R0,#+1
        B.N      ??handlePlainCommand_2
//  674 	} else {
//  675 		return false;
??handlePlainCommand_16:
        MOVS     R0,#+0
??handlePlainCommand_2:
        POP      {R1-R7,PC}       ;; return
          CFI EndBlock cfiBlock29
//  676 	}
//  677 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22:
        DC32     `?<Constant "binary">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22_1:
        DC32     tsState

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22_2:
        DC32     `?<Constant "TunerStudio size=%d /...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22_3:
        DC32     currentPageId

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22_4:
        DC32     `?<Constant "TunerStudio W=%d / C=...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22_5:
        DC32     0x40020800

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22_6:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22_7:
        DC32     `?<Constant "TS RX on %s%d/TX on %...">`
//  678 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock30 Using cfiCommon0
          CFI Function _Z27tunerStudioHandleCrcCommandP12ts_channel_sPci
        THUMB
//  679 int tunerStudioHandleCrcCommand(ts_channel_s *tsChannel, char *data, int incomingPacketSize) {
_Z27tunerStudioHandleCrcCommandP12ts_channel_sPci:
        PUSH     {R4-R10,LR}
          CFI R14 Frame(CFA, -4)
          CFI R10 Frame(CFA, -8)
          CFI R9 Frame(CFA, -12)
          CFI R8 Frame(CFA, -16)
          CFI R7 Frame(CFA, -20)
          CFI R6 Frame(CFA, -24)
          CFI R5 Frame(CFA, -28)
          CFI R4 Frame(CFA, -32)
          CFI CFA R13+32
        SUB      SP,SP,#+8
          CFI CFA R13+40
        MOVS     R4,R0
        MOVS     R7,R1
        MOVS     R5,R2
//  680 	char command = data[0];
        LDRB     R6,[R7, #+0]
//  681 	data++;
        ADDS     R7,R7,#+1
//  682 	if (command == TS_HELLO_COMMAND || command == TS_HELLO_COMMAND_DEPRECATED) {
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+83
        BEQ.N    ??tunerStudioHandleCrcCommand_0
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+72
        BNE.N    ??tunerStudioHandleCrcCommand_1
//  683 		tunerStudioDebug("got Query command");
??tunerStudioHandleCrcCommand_0:
        LDR.N    R0,??DataTable24_59
          CFI FunCall _Z16tunerStudioDebugPKc
        BL       _Z16tunerStudioDebugPKc
//  684 		handleQueryCommand(tsChannel, TS_CRC);
        MOVS     R1,#+1
        MOVS     R0,R4
          CFI FunCall _Z18handleQueryCommandP12ts_channel_s20ts_response_format_e
        BL       _Z18handleQueryCommandP12ts_channel_s20ts_response_format_e
        B.N      ??tunerStudioHandleCrcCommand_2
//  685 	} else if (command == TS_GET_TEXT) {
??tunerStudioHandleCrcCommand_1:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+71
        BNE.N    ??tunerStudioHandleCrcCommand_3
//  686 		handleGetText(tsChannel);
        MOVS     R0,R4
          CFI FunCall _Z13handleGetTextP12ts_channel_s
        BL       _Z13handleGetTextP12ts_channel_s
        B.N      ??tunerStudioHandleCrcCommand_2
//  687 	} else if (command == TS_EXECUTE) {
??tunerStudioHandleCrcCommand_3:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+69
        BNE.N    ??tunerStudioHandleCrcCommand_4
//  688 		handleExecuteCommand(tsChannel, data, incomingPacketSize - 1);
        SUBS     R2,R5,#+1
        MOVS     R1,R7
        MOVS     R0,R4
          CFI FunCall _Z20handleExecuteCommandP12ts_channel_sPci
        BL       _Z20handleExecuteCommandP12ts_channel_sPci
        B.N      ??tunerStudioHandleCrcCommand_2
//  689 	} else if (command == TS_OUTPUT_COMMAND) {
??tunerStudioHandleCrcCommand_4:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+79
        BNE.N    ??tunerStudioHandleCrcCommand_5
//  690 		handleOutputChannelsCommand(tsChannel, TS_CRC);
        MOVS     R1,#+1
        MOVS     R0,R4
          CFI FunCall _Z27handleOutputChannelsCommandP12ts_channel_s20ts_response_format_e
        BL       _Z27handleOutputChannelsCommandP12ts_channel_s20ts_response_format_e
        B.N      ??tunerStudioHandleCrcCommand_2
//  691 	} else if (command == TS_PAGE_COMMAND) {
??tunerStudioHandleCrcCommand_5:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+80
        BNE.N    ??tunerStudioHandleCrcCommand_6
//  692 		uint16_t page = *(uint16_t *) data;
        LDRH     R8,[R7, #+0]
//  693 		handlePageSelectCommand(tsChannel, TS_CRC, page);
        MOV      R2,R8
        UXTH     R2,R2            ;; ZeroExt  R2,R2,#+16,#+16
        MOVS     R1,#+1
        MOVS     R0,R4
          CFI FunCall _Z23handlePageSelectCommandP12ts_channel_s20ts_response_format_et
        BL       _Z23handlePageSelectCommandP12ts_channel_s20ts_response_format_et
        B.N      ??tunerStudioHandleCrcCommand_2
//  694 	} else if (command == TS_CHUNK_WRITE_COMMAND) {
??tunerStudioHandleCrcCommand_6:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+67
        BNE.N    ??tunerStudioHandleCrcCommand_7
//  695 		currentPageId = *(uint16_t *) data;
        LDR.N    R0,??DataTable24_3
        LDRSH    R1,[R7, #+0]
        STRH     R1,[R0, #+0]
//  696 		uint16_t offset = *(uint16_t *) (data + 2);
        LDRH     R8,[R7, #+2]
//  697 		uint16_t count = *(uint16_t *) (data + 4);
        LDRH     R9,[R7, #+4]
//  698 		handleWriteChunkCommand(tsChannel, TS_CRC, offset, count, data + sizeof(TunerStudioWriteChunkRequest));
        ADDS     R0,R7,#+6
        STR      R0,[SP, #+0]
        MOV      R3,R9
        SXTH     R3,R3            ;; SignExt  R3,R3,#+16,#+16
        MOV      R2,R8
        SXTH     R2,R2            ;; SignExt  R2,R2,#+16,#+16
        MOVS     R1,#+1
        MOVS     R0,R4
          CFI FunCall _Z23handleWriteChunkCommandP12ts_channel_s20ts_response_format_essPv
        BL       _Z23handleWriteChunkCommandP12ts_channel_s20ts_response_format_essPv
        B.N      ??tunerStudioHandleCrcCommand_2
//  699 	} else if (command == TS_SINGLE_WRITE_COMMAND) {
??tunerStudioHandleCrcCommand_7:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+87
        BNE.N    ??tunerStudioHandleCrcCommand_8
//  700 		uint16_t page = *(uint16_t *) data;
        LDRH     R8,[R7, #+0]
//  701 		uint16_t offset = *(uint16_t *) (data + 2);
        LDRH     R9,[R7, #+2]
//  702 		uint8_t value = data[4];
        LDRB     R10,[R7, #+4]
//  703 		handleWriteValueCommand(tsChannel, TS_CRC, page, offset, value);
        UXTB     R10,R10          ;; ZeroExt  R10,R10,#+24,#+24
        STR      R10,[SP, #+0]
        MOV      R3,R9
        UXTH     R3,R3            ;; ZeroExt  R3,R3,#+16,#+16
        MOV      R2,R8
        UXTH     R2,R2            ;; ZeroExt  R2,R2,#+16,#+16
        MOVS     R1,#+1
        MOVS     R0,R4
          CFI FunCall _Z23handleWriteValueCommandP12ts_channel_s20ts_response_format_etth
        BL       _Z23handleWriteValueCommandP12ts_channel_s20ts_response_format_etth
        B.N      ??tunerStudioHandleCrcCommand_2
//  704 	} else if (command == TS_CRC_CHECK_COMMAND) {
??tunerStudioHandleCrcCommand_8:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+107
        BNE.N    ??tunerStudioHandleCrcCommand_9
//  705 		uint16_t page = *(uint16_t *) data;
        LDRH     R8,[R7, #+0]
//  706 		uint16_t offset = *(uint16_t *) (data + 2);
        LDRH     R9,[R7, #+2]
//  707 		uint16_t count = *(uint16_t *) (data + 4);
        LDRH     R10,[R7, #+4]
//  708 		handleCrc32Check(tsChannel, TS_CRC, page, offset, count);
        UXTH     R10,R10          ;; ZeroExt  R10,R10,#+16,#+16
        STR      R10,[SP, #+0]
        MOV      R3,R9
        UXTH     R3,R3            ;; ZeroExt  R3,R3,#+16,#+16
        MOV      R2,R8
        UXTH     R2,R2            ;; ZeroExt  R2,R2,#+16,#+16
        MOVS     R1,#+1
        MOVS     R0,R4
          CFI FunCall _Z16handleCrc32CheckP12ts_channel_s20ts_response_format_ettt
        BL       _Z16handleCrc32CheckP12ts_channel_s20ts_response_format_ettt
        B.N      ??tunerStudioHandleCrcCommand_2
//  709 	} else if (command == TS_BURN_COMMAND) {
??tunerStudioHandleCrcCommand_9:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+66
        BNE.N    ??tunerStudioHandleCrcCommand_10
//  710 		uint16_t page = *(uint16_t *) data;
        LDRH     R8,[R7, #+0]
//  711 		handleBurnCommand(tsChannel, TS_CRC, page);
        MOV      R2,R8
        UXTH     R2,R2            ;; ZeroExt  R2,R2,#+16,#+16
        MOVS     R1,#+1
        MOVS     R0,R4
          CFI FunCall _Z17handleBurnCommandP12ts_channel_s20ts_response_format_et
        BL       _Z17handleBurnCommandP12ts_channel_s20ts_response_format_et
        B.N      ??tunerStudioHandleCrcCommand_2
//  712 	} else if (command == TS_READ_COMMAND) {
??tunerStudioHandleCrcCommand_10:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+82
        BNE.N    ??tunerStudioHandleCrcCommand_11
//  713 		uint16_t page = *(uint16_t *) data;
        LDRH     R8,[R7, #+0]
//  714 		uint16_t offset = *(uint16_t *) (data + 2);
        LDRH     R9,[R7, #+2]
//  715 		uint16_t count = *(uint16_t *) (data + 4);
        LDRH     R10,[R7, #+4]
//  716 		handlePageReadCommand(tsChannel, TS_CRC, page, offset, count);
        UXTH     R10,R10          ;; ZeroExt  R10,R10,#+16,#+16
        STR      R10,[SP, #+0]
        MOV      R3,R9
        UXTH     R3,R3            ;; ZeroExt  R3,R3,#+16,#+16
        MOV      R2,R8
        UXTH     R2,R2            ;; ZeroExt  R2,R2,#+16,#+16
        MOVS     R1,#+1
        MOVS     R0,R4
          CFI FunCall _Z21handlePageReadCommandP12ts_channel_s20ts_response_format_ettt
        BL       _Z21handlePageReadCommandP12ts_channel_s20ts_response_format_ettt
        B.N      ??tunerStudioHandleCrcCommand_2
//  717 	} else if (command == 't' || command == 'T') {
??tunerStudioHandleCrcCommand_11:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+116
        BEQ.N    ??tunerStudioHandleCrcCommand_12
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+84
        BNE.N    ??tunerStudioHandleCrcCommand_13
//  718 		handleTestCommand(tsChannel);
??tunerStudioHandleCrcCommand_12:
        MOVS     R0,R4
          CFI FunCall _Z17handleTestCommandP12ts_channel_s
        BL       _Z17handleTestCommandP12ts_channel_s
        B.N      ??tunerStudioHandleCrcCommand_2
//  719 	} else if (command == TS_LEGACY_HELLO_COMMAND) {
??tunerStudioHandleCrcCommand_13:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+81
        BNE.N    ??tunerStudioHandleCrcCommand_14
//  720 		/**
//  721 		 * 'Q' is the query command used for compatibility with older ECUs
//  722 		 */
//  723 		tunerStudioDebug("ignoring Q");
        LDR.N    R0,??DataTable24_60
          CFI FunCall _Z16tunerStudioDebugPKc
        BL       _Z16tunerStudioDebugPKc
        B.N      ??tunerStudioHandleCrcCommand_2
//  724 	} else if (command == TS_COMMAND_F) {
??tunerStudioHandleCrcCommand_14:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+70
        BNE.N    ??tunerStudioHandleCrcCommand_15
//  725 		tunerStudioDebug("ignoring F");
        LDR.N    R0,??DataTable24_61
          CFI FunCall _Z16tunerStudioDebugPKc
        BL       _Z16tunerStudioDebugPKc
        B.N      ??tunerStudioHandleCrcCommand_2
//  726 		/**
//  727 		 * http://www.msextra.com/forums/viewtopic.php?f=122&t=48327
//  728 		 * Response from TS support: This is an optional command		 *
//  729 		 * "The F command is used to find what ini. file needs to be loaded in TunerStudio to match the controller.
//  730 		 * If you are able to just make your firmware ignore the command that would work.
//  731 		 * Currently on some firmware versions the F command is not used and is just ignored by the firmware as a unknown command."
//  732 		 */
//  733 	} else {
//  734 		tunerStudioError("ERROR: ignoring unexpected command");
??tunerStudioHandleCrcCommand_15:
        LDR.N    R0,??DataTable24_62
          CFI FunCall _Z16tunerStudioErrorPKc
        BL       _Z16tunerStudioErrorPKc
//  735 		return false;
        MOVS     R0,#+0
        B.N      ??tunerStudioHandleCrcCommand_16
//  736 	}
//  737 	return true;
??tunerStudioHandleCrcCommand_2:
        MOVS     R0,#+1
??tunerStudioHandleCrcCommand_16:
        POP      {R1,R2,R4-R10,PC}  ;; return
          CFI EndBlock cfiBlock30
//  738 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23:
        DC32     tsLogger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_1:
        DC32     configWorkingCopy

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_2:
        DC32     `?<Constant "PAGE %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_3:
        DC32     `?<Constant "applying soft change ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_4:
        DC32     `?<Constant "WRITE CHUNK mode=%d p...">`
//  739 
//  740 /**
//  741  * we use 'blockingFactor = 256' in rusefi.ini
//  742  * todo: should we just do (256 + CRC_WRAPPING_SIZE) ?
//  743  */
//  744 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  745 static uint8_t tsCrcWriteBuffer[300];
tsCrcWriteBuffer:
        DS8 300

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock31 Using cfiCommon0
          CFI Function _Z28startTunerStudioConnectivityv
        THUMB
_Z28startTunerStudioConnectivityv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        MOVS     R0,#+0
          CFI FunCall _Z22getTunerStudioPageSizei
        BL       _Z22getTunerStudioPageSizei
        MOVW     R1,#+15288
        CMP      R0,R1
        BEQ.N    ??startTunerStudioConnectivity_0
        MOVS     R0,#+0
          CFI FunCall _Z22getTunerStudioPageSizei
        BL       _Z22getTunerStudioPageSizei
        MOVS     R2,R0
        MOVW     R1,#+15288
        LDR.N    R0,??DataTable24_63
          CFI FunCall firmwareError
        BL       firmwareError
??startTunerStudioConnectivity_0:
        MOVS     R2,#+40
        MOVS     R1,#+0
        LDR.N    R0,??DataTable24_2
          CFI FunCall memset
        BL       memset
          CFI FunCall _Z19syncTunerStudioCopyv
        BL       _Z19syncTunerStudioCopyv
        LDR.N    R1,??DataTable24_64
        LDR.N    R0,??DataTable24_65
          CFI FunCall addConsoleAction
        BL       addConsoleAction
        LDR.N    R1,??DataTable24_66
        LDR.N    R0,??DataTable24_67
          CFI FunCall addConsoleAction
        BL       addConsoleAction
        LDR.N    R1,??DataTable24_68
        LDR.N    R0,??DataTable24_69
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
          CFI FunCall _Z17getTsSerialDevicev
        BL       _Z17getTsSerialDevicev
        LDR.N    R1,??DataTable24_38
        STR      R0,[R1, #+0]
        LDR.N    R0,??DataTable24_38
        LDR.N    R1,??DataTable24_70
        STR      R1,[R0, #+4]
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        LDR.N    R3,??DataTable24_71
        MOVS     R2,#+64
        MOV      R1,#+1512
        LDR.N    R0,??DataTable24_72
          CFI FunCall chThdCreateStatic
        BL       chThdCreateStatic
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock31

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24:
        DC8      "001"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_1:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_2:
        DC32     tsState

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_3:
        DC32     currentPageId

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_4:
        DC32     `?<Constant "ERROR invalid offset %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_5:
        DC32     `?<Constant "ERROR: out of range">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_6:
        DC32     `?<Constant "ERROR: unexpected count">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_7:
        DC32     `?<Constant "ERROR unexpected coun...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_8:
        DC32     `?<Constant "CRC32 request: pageId...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_9:
        DC32     `?<Constant "CRC32 response: %x">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_10:
        DC32     `?<Constant "got W (Write)">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_11:
        DC32     `?<Constant "ERROR: out of range2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_12:
        DC32     `?<Constant "ERROR offset %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_13:
        DC32     previousWriteReportMs

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_14:
        DC32     `?<Constant "page %d offset %d: va...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_15:
        DC32     `?<Constant "READ mode=%d page=%d ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_16:
        DC32     `?<Constant "ERROR: invalid page n...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_17:
        DC32     `?<Constant "invalid offset/count ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_18:
        DC32     `?<Constant "plain">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_19:
        DC32     `?<Constant "got B (Burn) %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_20:
        DC32     persistentState+0x8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_21:
        DC32     `?<Constant "BURN in %dms">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_22:
        DC32     firstByte

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_23:
        DC32     secondByte

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_24:
        DC32     `?<Constant "ERROR: no second">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_25:
        DC32     BINARY_RESPONSE

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_26:
        DC32     `?<Constant "TunerStudio: invalid ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_27:
        DC32     `?<Constant "ERROR: CRC header size">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_28:
        DC32     `?<Constant "ERROR: did not receiv...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_29:
        DC32     `?<Constant "unexpected command %x">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_30:
        DC32     `?<Constant "Got only %d bytes whi...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_31:
        DC32     `?<Constant "ERROR: not enough byt...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_32:
        DC32     `?<Constant "TunerStudio: CRC %x %...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_33:
        DC32     `?<Constant "TunerStudio: command ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_34:
        DC32     `?<Constant "ERROR: CRC issue">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_35:
        DC32     `?<Constant "got unexpected TunerS...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_36:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_37:
        DC32     `?<Constant "tunerstudio thread">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_38:
        DC32     tsChannel

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_39:
        DC32     `?<Constant "got S/H (queryCommand...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_40:
        DC32     `?<Constant "MShift v0.01">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_41:
        DC32     tsOutputChannels

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_42:
        DC32     `?<Constant "got T (Test)">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_43:
        DC32     `?<Constant "8529">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_44:
        DC32     `?<Constant " ts_p_alive\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_45:
        DC32     console_line_callback

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_46:
        DC32     `?<Constant "Got naked Query command">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_47:
        DC32     pageIn

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_48:
        DC32     `?<Constant "ERROR: not enough for...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_49:
        DC32     `?<Constant "Got naked BURN">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_50:
        DC32     `?<Constant "ERROR: Not enough for...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_51:
        DC32     `?<Constant "Got naked CHUNK_WRITE">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_52:
        DC32     writeChunkRequest

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_53:
        DC32     `?<Constant "ERROR: Not enough for...">_1`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_54:
        DC32     `?<Constant "ERROR: Not enough for...">_2`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_55:
        DC32     readRequest

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_56:
        DC32     `?<Constant "Not enough for plain ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_57:
        DC32     `?<Constant "ignoring LEGACY_HELLO...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_58:
        DC32     `?<Constant "not ignoring F">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_59:
        DC32     `?<Constant "got Query command">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_60:
        DC32     `?<Constant "ignoring Q">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_61:
        DC32     `?<Constant "ignoring F">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_62:
        DC32     `?<Constant "ERROR: ignoring unexp...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_63:
        DC32     `?<Constant "TS page size mismatch...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_64:
        DC32     _Z12printTsStatsv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_65:
        DC32     `?<Constant "tsinfo">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_66:
        DC32     _Z7resetTsv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_67:
        DC32     `?<Constant "reset_ts">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_68:
        DC32     _Z10setTsSpeedi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_69:
        DC32     `?<Constant "set_ts_speed">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_70:
        DC32     tsCrcWriteBuffer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_71:
        DC32     _Z18tsThreadEntryPointPv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_72:
        DC32     tsThreadStack

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memcpy
          CFI Block cfiBlock32 Using cfiCommon0
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
          CFI EndBlock cfiBlock32

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memset
          CFI Block cfiBlock33 Using cfiCommon0
          CFI Function memset
        THUMB
// __intrinsic __nounwind __interwork __softfp void *memset(void *, int, size_t)
memset:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R2,R5
        MOVS     R1,R6
        MOVS     R0,R4
          CFI FunCall __aeabi_memset
        BL       __aeabi_memset
        MOVS     R0,R4
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock33

        SECTION `.init_array`:CODE:ROOT(2)
        SECTION_TYPE SHT_INIT_ARRAY, 0
        DATA
        DC32    RELOC_ARM_TARGET1 __sti__routine

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TunerStudio size=%d /...">`:
        DATA
        DC8 54H, 75H, 6EH, 65H, 72H, 53H, 74H, 75H
        DC8 64H, 69H, 6FH, 20H, 73H, 69H, 7AH, 65H
        DC8 3DH, 25H, 64H, 20H, 2FH, 20H, 74H, 6FH
        DC8 74H, 61H, 6CH, 3DH, 25H, 64H, 20H, 2FH
        DC8 20H, 65H, 72H, 72H, 6FH, 72H, 73H, 3DH
        DC8 25H, 64H, 20H, 2FH, 20H, 48H, 3DH, 25H
        DC8 64H, 20H, 2FH, 20H, 4FH, 3DH, 25H, 64H
        DC8 20H, 2FH, 20H, 50H, 3DH, 25H, 64H, 20H
        DC8 2FH, 20H, 42H, 3DH, 25H, 64H, 0
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TunerStudio W=%d / C=...">`:
        DATA
        DC8 "TunerStudio W=%d / C=%d / P=%d / page=%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TS RX on %s%d/TX on %...">`:
        DATA
        DC8 "TS RX on %s%d/TX on %s%d @%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "%s"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "PAGE %d">`:
        DATA
        DC8 "PAGE %d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "applying soft change ...">`:
        DATA
        DC8 "applying soft change from %d length %d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "WRITE CHUNK mode=%d p...">`:
        DATA
        DC8 "WRITE CHUNK mode=%d p=%d o=%d s=%d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ERROR invalid offset %d">`:
        DATA
        DC8 "ERROR invalid offset %d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ERROR: out of range">`:
        DATA
        DC8 "ERROR: out of range"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ERROR: unexpected count">`:
        DATA
        DC8 "ERROR: unexpected count"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ERROR unexpected coun...">`:
        DATA
        DC8 "ERROR unexpected count %d"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "CRC32 request: pageId...">`:
        DATA
        DC8 "CRC32 request: pageId %d offset %d size %d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "CRC32 response: %x">`:
        DATA
        DC8 "CRC32 response: %x"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "got W (Write)">`:
        DATA
        DC8 "got W (Write)"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ERROR: out of range2">`:
        DATA
        DC8 "ERROR: out of range2"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ERROR offset %d">`:
        DATA
        DC8 "ERROR offset %d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "page %d offset %d: va...">`:
        DATA
        DC8 "page %d offset %d: value=%d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "READ mode=%d page=%d ...">`:
        DATA
        DC8 "READ mode=%d page=%d offset=%d size=%d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ERROR: invalid page n...">`:
        DATA
        DC8 "ERROR: invalid page number"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid offset/count ...">`:
        DATA
        DC8 "invalid offset/count %d/%d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "got B (Burn) %s">`:
        DATA
        DC8 "got B (Burn) %s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "plain">`:
        DATA
        DC8 "plain"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "CRC"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "BURN in %dms">`:
        DATA
        DC8 "BURN in %dms"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ERROR: no second">`:
        DATA
        DC8 "ERROR: no second"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TunerStudio: invalid ...">`:
        DATA
        DC8 "TunerStudio: invalid size: %d"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ERROR: CRC header size">`:
        DATA
        DC8 "ERROR: CRC header size"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ERROR: did not receiv...">`:
        DATA
        DC8 "ERROR: did not receive command"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "unexpected command %x">`:
        DATA
        DC8 "unexpected command %x"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Got only %d bytes whi...">`:
        DATA
        DC8 47H, 6FH, 74H, 20H, 6FH, 6EH, 6CH, 79H
        DC8 20H, 25H, 64H, 20H, 62H, 79H, 74H, 65H
        DC8 73H, 20H, 77H, 68H, 69H, 6CH, 65H, 20H
        DC8 65H, 78H, 70H, 65H, 63H, 74H, 69H, 6EH
        DC8 67H, 20H, 25H, 64H, 20H, 66H, 6FH, 72H
        DC8 20H, 63H, 6FH, 6DH, 6DH, 61H, 6EH, 64H
        DC8 20H, 25H, 63H, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ERROR: not enough byt...">`:
        DATA
        DC8 "ERROR: not enough bytes in stream"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TunerStudio: CRC %x %...">`:
        DATA
        DC8 "TunerStudio: CRC %x %x %x %x"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TunerStudio: command ...">`:
        DATA
        DC8 "TunerStudio: command %c actual CRC %x/expected %x"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ERROR: CRC issue">`:
        DATA
        DC8 "ERROR: CRC issue"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "got unexpected TunerS...">`:
        DATA
        DC8 "got unexpected TunerStudio command %x:%c\015\012"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "tunerstudio thread">`:
        DATA
        DC8 "tunerstudio thread"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "got S/H (queryCommand...">`:
        DATA
        DC8 "got S/H (queryCommand) mode=%d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MShift v0.01">`:
        DATA
        DC8 "MShift v0.01"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "got T (Test)">`:
        DATA
        DC8 "got T (Test)"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "8529">`:
        DATA
        DC8 "8529"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant " ts_p_alive\\r\\n">`:
        DATA
        DC8 " ts_p_alive\015\012"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Got naked Query command">`:
        DATA
        DC8 "Got naked Query command"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ERROR: not enough for...">`:
        DATA
        DC8 "ERROR: not enough for PAGE"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Got naked BURN">`:
        DATA
        DC8 "Got naked BURN"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ERROR: Not enough for...">`:
        DATA
        DC8 "ERROR: Not enough for plain burn"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Got naked CHUNK_WRITE">`:
        DATA
        DC8 "Got naked CHUNK_WRITE"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ERROR: Not enough for...">_1`:
        DATA
        DC8 45H, 52H, 52H, 4FH, 52H, 3AH, 20H, 4EH
        DC8 6FH, 74H, 20H, 65H, 6EH, 6FH, 75H, 67H
        DC8 68H, 20H, 66H, 6FH, 72H, 20H, 70H, 6CH
        DC8 61H, 69H, 6EH, 20H, 63H, 68H, 75H, 6EH
        DC8 6BH, 20H, 77H, 72H, 69H, 74H, 65H, 20H
        DC8 68H, 65H, 61H, 64H, 65H, 72H, 3AH, 20H
        DC8 25H, 64H, 0
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ERROR: Not enough for...">_2`:
        DATA
        DC8 45H, 52H, 52H, 4FH, 52H, 3AH, 20H, 4EH
        DC8 6FH, 74H, 20H, 65H, 6EH, 6FH, 75H, 67H
        DC8 68H, 20H, 66H, 6FH, 72H, 20H, 70H, 6CH
        DC8 61H, 69H, 6EH, 20H, 63H, 68H, 75H, 6EH
        DC8 6BH, 20H, 77H, 72H, 69H, 74H, 65H, 20H
        DC8 63H, 6FH, 6EH, 74H, 65H, 6EH, 74H, 3AH
        DC8 20H, 25H, 64H, 20H, 77H, 68H, 69H, 6CH
        DC8 65H, 20H, 65H, 78H, 70H, 65H, 63H, 74H
        DC8 69H, 6EH, 67H, 20H, 25H, 64H, 0
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Not enough for plain ...">`:
        DATA
        DC8 "Not enough for plain read header"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ignoring LEGACY_HELLO...">`:
        DATA
        DC8 "ignoring LEGACY_HELLO_COMMAND"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "not ignoring F">`:
        DATA
        DC8 "not ignoring F"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "001"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "got Query command">`:
        DATA
        DC8 "got Query command"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ignoring Q">`:
        DATA
        DC8 "ignoring Q"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ignoring F">`:
        DATA
        DC8 "ignoring F"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ERROR: ignoring unexp...">`:
        DATA
        DC8 "ERROR: ignoring unexpected command"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TS page size mismatch...">`:
        DATA
        DC8 "TS page size mismatch: %d/%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "tsinfo">`:
        DATA
        DC8 "tsinfo"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "reset_ts">`:
        DATA
        DC8 "reset_ts"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_ts_speed">`:
        DATA
        DC8 "set_ts_speed"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "binary">`:
        DATA
        DC8 "binary"
        DC8 0

        END
//  746 
//  747 void startTunerStudioConnectivity(void) {
//  748 	if (sizeof(persistent_config_s) != getTunerStudioPageSize(0))
//  749 		firmwareError("TS page size mismatch: %d/%d", sizeof(persistent_config_s), getTunerStudioPageSize(0));
//  750 
//  751 	if (sizeof(TunerStudioOutputChannels) != TS_OUTPUT_SIZE)
//  752 		firmwareError("TS outputs size mismatch: %d/%d", sizeof(TunerStudioOutputChannels), TS_OUTPUT_SIZE);
//  753 
//  754 	memset(&tsState, 0, sizeof(tsState));
//  755 	syncTunerStudioCopy();
//  756 
//  757 	addConsoleAction("tsinfo", printTsStats);
//  758 	addConsoleAction("reset_ts", resetTs);
//  759 	addConsoleActionI("set_ts_speed", setTsSpeed);
//  760 
//  761 	tsChannel.channel = getTsSerialDevice();
//  762 	tsChannel.writeBuffer = tsCrcWriteBuffer;
//  763 
//  764 	chThdCreateStatic(tsThreadStack, sizeof(tsThreadStack), NORMALPRIO, tsThreadEntryPoint, NULL);
//  765 }
//  766 
//  767 #endif
// 
// 17 930 bytes in section .bss
//      2 bytes in section .data
//      4 bytes in section .init_array
//  1 592 bytes in section .rodata
//  3 718 bytes in section .text
// 
//  3 678 bytes of CODE  memory (+ 44 bytes shared)
//  1 592 bytes of CONST memory
// 17 932 bytes of DATA  memory
//
//Errors: none
//Warnings: 2
