///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:56 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\obd /
//                       2.cpp                                                /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\obd /
//                       2.cpp -lCN F:\stuff\rusefi_sourceforge\firmware\iar\ /
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
//                       obd2.s                                               /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME obd2

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z11sendMessagev
        EXTERN _Z12commonTxIniti
        EXTERN _Z8setTxBitii
        EXTERN _ZN18LoggingWithStorageC1EPKc
        EXTERN scheduleMsg
        EXTERN txmsg

        PUBLIC _Z16obdOnCanPacketRxP10CANRxFrame
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\obd2.cpp
//    1 /*
//    2  * @file obd2.cpp
//    3  *
//    4  * ISO 15765-4
//    5  * http://en.wikipedia.org/wiki/OBD-II_PIDs
//    6  *
//    7  * @date Jun 9, 2015
//    8  * @author Andrey Belomutskiy, (c) 2012-2015
//    9  *
//   10  * This file is part of rusEfi - see http://rusefi.com
//   11  *
//   12  * rusEfi is free software; you can redistribute it and/or modify it under the terms of
//   13  * the GNU General Public License as published by the Free Software Foundation; either
//   14  * version 3 of the License, or (at your option) any later version.
//   15  *
//   16  * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
//   17  * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   18  * GNU General Public License for more details.
//   19  *
//   20  * You should have received a copy of the GNU General Public License along with this program.
//   21  * If not, see <http://www.gnu.org/licenses/>.
//   22  */
//   23 
//   24 #include "main.h"
//   25 #include "obd2.h"
//   26 #include "can_hw.h"
//   27 
//   28 extern CANTxFrame txmsg;
//   29 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   30 static LoggingWithStorage logger("obd2");
        LDR.N    R1,??DataTable2
        LDR.N    R0,??DataTable2_1
          CFI FunCall _ZN18LoggingWithStorageC1EPKc
        BL       _ZN18LoggingWithStorageC1EPKc
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
logger:
        DS8 260
//   31 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z20handleGetDataRequestP10CANRxFrame
        THUMB
//   32 static void handleGetDataRequest(CANRxFrame *rx) {
_Z20handleGetDataRequestP10CANRxFrame:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//   33 	if (rx->data8[2] == PID_SUPPORTED_PIDS_REQUEST) {
        LDRB     R0,[R4, #+14]
        CMP      R0,#+0
        BNE.N    ??handleGetDataRequest_0
//   34 		scheduleMsg(&logger, "Got lookup request");
        LDR.N    R1,??DataTable2_2
        LDR.N    R0,??DataTable2_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   35 
//   36 		commonTxInit(OBD_TEST_RESPONSE);
        MOV      R0,#+2024
          CFI FunCall _Z12commonTxIniti
        BL       _Z12commonTxIniti
//   37 		txmsg.data8[0] = 6; // 6 bytes
        LDR.N    R0,??DataTable2_3
        MOVS     R1,#+6
        STRB     R1,[R0, #+8]
//   38 		txmsg.data8[1] = 0x41; // mode 1
        LDR.N    R0,??DataTable2_3
        MOVS     R1,#+65
        STRB     R1,[R0, #+9]
//   39 		txmsg.data8[2] = PID_SUPPORTED_PIDS_REQUEST;
        LDR.N    R0,??DataTable2_3
        MOVS     R1,#+0
        STRB     R1,[R0, #+10]
//   40 
//   41 		setTxBit(3, 8 - PID_ENGINE_LOAD);
        MOVS     R1,#+4
        MOVS     R0,#+3
          CFI FunCall _Z8setTxBitii
        BL       _Z8setTxBitii
//   42 		setTxBit(3, 8 - PID_COOLANT_TEMP);
        MOVS     R1,#+3
        MOVS     R0,#+3
          CFI FunCall _Z8setTxBitii
        BL       _Z8setTxBitii
//   43 
//   44 		setTxBit(4, 16 - PID_RPM);
        MOVS     R1,#+4
        MOVS     R0,#+4
          CFI FunCall _Z8setTxBitii
        BL       _Z8setTxBitii
//   45 		setTxBit(4, 16 - PID_TIMING_ADVANCE);
        MOVS     R1,#+2
        MOVS     R0,#+4
          CFI FunCall _Z8setTxBitii
        BL       _Z8setTxBitii
//   46 		setTxBit(4, 16 - PID_INTAKE_TEMP);
        MOVS     R1,#+1
        MOVS     R0,#+4
          CFI FunCall _Z8setTxBitii
        BL       _Z8setTxBitii
//   47 
//   48 		setTxBit(5, 24 - PID_THROTTLE);
        MOVS     R1,#+7
        MOVS     R0,#+5
          CFI FunCall _Z8setTxBitii
        BL       _Z8setTxBitii
//   49 
//   50 		sendMessage();
          CFI FunCall _Z11sendMessagev
        BL       _Z11sendMessagev
        B.N      ??handleGetDataRequest_1
//   51 	} else if (rx->data8[2] == PID_COOLANT_TEMP) {
??handleGetDataRequest_0:
        LDRB     R0,[R4, #+14]
        CMP      R0,#+5
        BNE.N    ??handleGetDataRequest_2
//   52 		scheduleMsg(&logger, "Got CLT request");
        LDR.N    R1,??DataTable2_4
        LDR.N    R0,??DataTable2_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   53 
//   54 		int clt = 67; // todo: use real value
        MOVS     R5,#+67
//   55 
//   56 		commonTxInit(OBD_TEST_RESPONSE);
        MOV      R0,#+2024
          CFI FunCall _Z12commonTxIniti
        BL       _Z12commonTxIniti
//   57 		txmsg.data8[0] = 3; // 3 bytes
        LDR.N    R0,??DataTable2_3
        MOVS     R1,#+3
        STRB     R1,[R0, #+8]
//   58 		txmsg.data8[1] = 0x41; // mode 1
        LDR.N    R0,??DataTable2_3
        MOVS     R1,#+65
        STRB     R1,[R0, #+9]
//   59 		txmsg.data8[2] = PID_COOLANT_TEMP;
        LDR.N    R0,??DataTable2_3
        MOVS     R1,#+5
        STRB     R1,[R0, #+10]
//   60 		txmsg.data8[3] = clt + 40;
        ADDS     R0,R5,#+40
        LDR.N    R1,??DataTable2_3
        STRB     R0,[R1, #+11]
//   61 		sendMessage();
          CFI FunCall _Z11sendMessagev
        BL       _Z11sendMessagev
        B.N      ??handleGetDataRequest_1
//   62 	} else if (rx->data8[2] == PID_RPM) {
??handleGetDataRequest_2:
        LDRB     R0,[R4, #+14]
        CMP      R0,#+12
        BNE.N    ??handleGetDataRequest_3
//   63 		scheduleMsg(&logger, "Got RPM request");
        LDR.N    R1,??DataTable2_5
        LDR.N    R0,??DataTable2_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   64 		unsigned int rpm = 2000*4; //	rotation/min.	((A*256)+B)/4 todo: use real value
        MOV      R5,#+8000
//   65 		commonTxInit(OBD_TEST_RESPONSE);
        MOV      R0,#+2024
          CFI FunCall _Z12commonTxIniti
        BL       _Z12commonTxIniti
//   66 		txmsg.data8[0] = 4; // 4 bytes
        LDR.N    R0,??DataTable2_3
        MOVS     R1,#+4
        STRB     R1,[R0, #+8]
//   67 		txmsg.data8[1] = 0x41; // mode 1
        LDR.N    R0,??DataTable2_3
        MOVS     R1,#+65
        STRB     R1,[R0, #+9]
//   68 		txmsg.data8[2] = PID_RPM;
        LDR.N    R0,??DataTable2_3
        MOVS     R1,#+12
        STRB     R1,[R0, #+10]
//   69 		txmsg.data8[3] = ((rpm>>8) & 0xFF);
        LSRS     R0,R5,#+8
        LDR.N    R1,??DataTable2_3
        STRB     R0,[R1, #+11]
//   70 		txmsg.data8[4] = ((rpm) & 0xFF);
        LDR.N    R0,??DataTable2_3
        STRB     R5,[R0, #+12]
//   71 		sendMessage();
          CFI FunCall _Z11sendMessagev
        BL       _Z11sendMessagev
        B.N      ??handleGetDataRequest_1
//   72 		
//   73 	} else if (rx->data8[2] == PID_TIMING_ADVANCE) {
??handleGetDataRequest_3:
        LDRB     R0,[R4, #+14]
        CMP      R0,#+14
        BNE.N    ??handleGetDataRequest_4
//   74 		scheduleMsg(&logger, "Got timing request");
        LDR.N    R1,??DataTable2_6
        LDR.N    R0,??DataTable2_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
        B.N      ??handleGetDataRequest_1
//   75 	} else if (rx->data8[2] == PID_SPEED) {
??handleGetDataRequest_4:
        LDRB     R0,[R4, #+14]
        CMP      R0,#+13
        BNE.N    ??handleGetDataRequest_5
//   76 		scheduleMsg(&logger, "Got speed request");
        LDR.N    R1,??DataTable2_7
        LDR.N    R0,??DataTable2_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   77 		commonTxInit(OBD_TEST_RESPONSE);
        MOV      R0,#+2024
          CFI FunCall _Z12commonTxIniti
        BL       _Z12commonTxIniti
//   78 		txmsg.data8[0] = 3; // 3 bytes
        LDR.N    R0,??DataTable2_3
        MOVS     R1,#+3
        STRB     R1,[R0, #+8]
//   79 		txmsg.data8[1] = 0x41; // mode 1
        LDR.N    R0,??DataTable2_3
        MOVS     R1,#+65
        STRB     R1,[R0, #+9]
//   80 		txmsg.data8[2] = PID_INTAKE_MAP;
        LDR.N    R0,??DataTable2_3
        MOVS     R1,#+11
        STRB     R1,[R0, #+10]
//   81 		txmsg.data8[3] = 123; // todo: real speed
        LDR.N    R0,??DataTable2_3
        MOVS     R1,#+123
        STRB     R1,[R0, #+11]
//   82 		sendMessage();
          CFI FunCall _Z11sendMessagev
        BL       _Z11sendMessagev
        B.N      ??handleGetDataRequest_1
//   83 	} else if (rx->data8[2] == PID_FUEL_PRESSURE) {
??handleGetDataRequest_5:
        LDRB     R0,[R4, #+14]
        CMP      R0,#+10
        BNE.N    ??handleGetDataRequest_6
//   84 		scheduleMsg(&logger, "Got fuel pressure request");
        LDR.N    R1,??DataTable2_8
        LDR.N    R0,??DataTable2_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
        B.N      ??handleGetDataRequest_1
//   85 	} else if (rx->data8[2] == PID_INTAKE_MAP) {
??handleGetDataRequest_6:
        LDRB     R0,[R4, #+14]
        CMP      R0,#+11
        BNE.N    ??handleGetDataRequest_7
//   86 		scheduleMsg(&logger, "Got MAP request");
        LDR.N    R1,??DataTable2_9
        LDR.N    R0,??DataTable2_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   87 		commonTxInit(OBD_TEST_RESPONSE);
        MOV      R0,#+2024
          CFI FunCall _Z12commonTxIniti
        BL       _Z12commonTxIniti
//   88 		txmsg.data8[0] = 3; // 3 bytes
        LDR.N    R0,??DataTable2_3
        MOVS     R1,#+3
        STRB     R1,[R0, #+8]
//   89 		txmsg.data8[1] = 0x41; // mode 1
        LDR.N    R0,??DataTable2_3
        MOVS     R1,#+65
        STRB     R1,[R0, #+9]
//   90 		txmsg.data8[2] = PID_INTAKE_MAP;
        LDR.N    R0,??DataTable2_3
        MOVS     R1,#+11
        STRB     R1,[R0, #+10]
//   91 		txmsg.data8[3] = 46; // todo: real MAP
        LDR.N    R0,??DataTable2_3
        MOVS     R1,#+46
        STRB     R1,[R0, #+11]
//   92 		sendMessage();
          CFI FunCall _Z11sendMessagev
        BL       _Z11sendMessagev
        B.N      ??handleGetDataRequest_1
//   93 	} else {
//   94 		scheduleMsg(&logger, "Got unhandled request");
??handleGetDataRequest_7:
        LDR.N    R1,??DataTable2_10
        LDR.N    R0,??DataTable2_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   95 	}
//   96 
//   97 }
??handleGetDataRequest_1:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock1
//   98 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z16obdOnCanPacketRxP10CANRxFrame
        THUMB
//   99 void obdOnCanPacketRx(CANRxFrame *rx) {
_Z16obdOnCanPacketRxP10CANRxFrame:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  100 	if (rx->SID != OBD_TEST_REQUEST) {
        LDR      R0,[R4, #+8]
        LSLS     R0,R0,#+21       ;; ZeroExtS R0,R0,#+21,#+21
        LSRS     R0,R0,#+21
        MOVW     R1,#+2015
        CMP      R0,R1
        BNE.N    ??obdOnCanPacketRx_0
//  101 		return;
//  102 	}
//  103 	if (rx->data8[0] == 2 && rx->data8[1] == OBD_CURRENT_DATA) {
??obdOnCanPacketRx_1:
        LDRB     R0,[R4, #+12]
        CMP      R0,#+2
        BNE.N    ??obdOnCanPacketRx_2
        LDRB     R0,[R4, #+13]
        CMP      R0,#+1
        BNE.N    ??obdOnCanPacketRx_2
//  104 		handleGetDataRequest(rx);
        MOVS     R0,R4
          CFI FunCall _Z20handleGetDataRequestP10CANRxFrame
        BL       _Z20handleGetDataRequestP10CANRxFrame
        B.N      ??obdOnCanPacketRx_3
//  105 	} else if (rx->data8[0] == 1 && rx->data8[1] == OBD_STORED_DIAGNOSTIC_TROUBLE_CODES) {
??obdOnCanPacketRx_2:
        LDRB     R0,[R4, #+12]
        CMP      R0,#+1
        BNE.N    ??obdOnCanPacketRx_4
        LDRB     R0,[R4, #+13]
        CMP      R0,#+3
        BNE.N    ??obdOnCanPacketRx_4
//  106 		scheduleMsg(&logger, "Got stored DTC request");
        LDR.N    R1,??DataTable2_11
        LDR.N    R0,??DataTable2_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
        B.N      ??obdOnCanPacketRx_3
//  107 	} else {
//  108 		scheduleMsg(&logger, "Got unhandled OBD message");
??obdOnCanPacketRx_4:
        LDR.N    R1,??DataTable2_12
        LDR.N    R0,??DataTable2_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  109 	}
//  110 }
??obdOnCanPacketRx_3:
??obdOnCanPacketRx_0:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2:
        DC32     `?<Constant "obd2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_1:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_2:
        DC32     `?<Constant "Got lookup request">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_3:
        DC32     txmsg

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_4:
        DC32     `?<Constant "Got CLT request">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_5:
        DC32     `?<Constant "Got RPM request">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_6:
        DC32     `?<Constant "Got timing request">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_7:
        DC32     `?<Constant "Got speed request">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_8:
        DC32     `?<Constant "Got fuel pressure req...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_9:
        DC32     `?<Constant "Got MAP request">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_10:
        DC32     `?<Constant "Got unhandled request">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_11:
        DC32     `?<Constant "Got stored DTC request">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_12:
        DC32     `?<Constant "Got unhandled OBD mes...">`

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
`?<Constant "Got lookup request">`:
        DATA
        DC8 "Got lookup request"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Got CLT request">`:
        DATA
        DC8 "Got CLT request"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Got RPM request">`:
        DATA
        DC8 "Got RPM request"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Got timing request">`:
        DATA
        DC8 "Got timing request"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Got speed request">`:
        DATA
        DC8 "Got speed request"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Got fuel pressure req...">`:
        DATA
        DC8 "Got fuel pressure request"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Got MAP request">`:
        DATA
        DC8 "Got MAP request"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Got unhandled request">`:
        DATA
        DC8 "Got unhandled request"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Got stored DTC request">`:
        DATA
        DC8 "Got stored DTC request"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Got unhandled OBD mes...">`:
        DATA
        DC8 "Got unhandled OBD message"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "obd2">`:
        DATA
        DC8 "obd2"
        DC8 0, 0, 0

        END
//  111 
// 
// 260 bytes in section .bss
//   4 bytes in section .init_array
// 220 bytes in section .rodata
// 494 bytes in section .text
// 
// 498 bytes of CODE  memory
// 220 bytes of CONST memory
// 260 bytes of DATA  memory
//
//Errors: none
//Warnings: none
