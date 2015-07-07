///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:40 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\can_hw /
//                       .cpp                                                 /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\can_hw /
//                       .cpp -lCN F:\stuff\rusefi_sourceforge\firmware\iar\D /
//                       ebug\List\ -lA F:\stuff\rusefi_sourceforge\firmware\ /
//                       iar\Debug\List\ -o F:\stuff\rusefi_sourceforge\firmw /
//                       are\iar\Debug\Obj\ --no_cse --no_unroll --no_inline  /
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
//                       can_hw.s                                             /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME can_hw

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN CAND1
        EXTERN CAND2
        EXTERN _Z10hwPortname11brain_pin_e
        EXTERN _Z12boolToStringb
        EXTERN _Z13mySetPadMode2PKc11brain_pin_ej
        EXTERN _Z15getVehicleSpeedv
        EXTERN _Z16obdOnCanPacketRxP10CANRxFrame
        EXTERN _ZN13RpmCalculator3rpmEv
        EXTERN _ZN18LoggingWithStorageC1EPKc
        EXTERN __aeabi_d2iz
        EXTERN __aeabi_dadd
        EXTERN __aeabi_ddiv
        EXTERN __aeabi_dmul
        EXTERN __aeabi_f2d
        EXTERN __aeabi_i2d
        EXTERN __aeabi_memset
        EXTERN addConsoleAction
        EXTERN boardConfiguration
        EXTERN canReceive
        EXTERN canStart
        EXTERN canTransmit
        EXTERN chThdCreateStatic
        EXTERN chThdSleep
        EXTERN engine
        EXTERN engineConfiguration
        EXTERN rlist
        EXTERN scheduleMsg
        EXTERN warning

        PUBLIC _Z11sendMessagev
        PUBLIC _Z12commonTxIniti
        PUBLIC _Z12sendMessage2i
        PUBLIC _Z7initCanv
        PUBLIC _Z8setTxBitii
        PUBLIC memset
        PUBLIC txmsg
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\hw_layer\can_hw.cpp
//    1 /**
//    2  * @file	can_hw.cpp
//    3  * @brief	CAN bus low level code
//    4  *
//    5  * todo: this file should be split into two - one for CAN transport level ONLY and
//    6  * another one with actual messages
//    7  *
//    8  * @date Dec 11, 2013
//    9  * @author Andrey Belomutskiy, (c) 2012-2015
//   10  */
//   11 
//   12 #include "main.h"
//   13 #include "can_hw.h"
//   14 #include "string.h"
//   15 #include "obd2.h"
//   16 
//   17 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//   18 
//   19 #include "pin_repository.h"
//   20 #include "engine_state.h"
//   21 #include "can_header.h"
//   22 #include "engine_configuration.h"
//   23 #include "vehicle_speed.h"
//   24 #endif /* EFI_PROD_CODE */
//   25 
//   26 #if EFI_CAN_SUPPORT || defined(__DOXYGEN__)
//   27 
//   28 EXTERN_ENGINE
//   29 ;
//   30 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   31 static int canReadCounter = 0;
canReadCounter:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   32 static int canWriteOk = 0;
canWriteOk:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   33 static int canWriteNotOk = 0;
canWriteNotOk:
        DS8 4

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   34 static LoggingWithStorage logger("CAN driver");
        LDR.W    R1,??DataTable13_1
        LDR.W    R0,??DataTable13_2
          CFI FunCall _ZN18LoggingWithStorageC1EPKc
        BL       _ZN18LoggingWithStorageC1EPKc
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
logger:
        DS8 260

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   35 static THD_WORKING_AREA(canTreadStack, UTILITY_THREAD_STACK_SIZE);
canTreadStack:
        DS8 712

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SID %x/%x %x %x %x %x...">`:
        DATA
        DC8 "SID %x/%x %x %x %x %x %x %x %x %x"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GOT odometerKm %d">`:
        DATA
        DC8 "GOT odometerKm %d"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GOT odometerMi %d">`:
        DATA
        DC8 "GOT odometerMi %d"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GOT time %d">`:
        DATA
        DC8 "GOT time %d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "waiting for CAN">`:
        DATA
        DC8 "waiting for CAN"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "CAN"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "%d too low CAN">`:
        DATA
        DC8 "%d too low CAN"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "CAN TX %s">`:
        DATA
        DC8 "CAN TX %s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "CAN RX %s">`:
        DATA
        DC8 "CAN RX %s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "type=%d canReadEnable...">`:
        DATA
        DC8 74H, 79H, 70H, 65H, 3DH, 25H, 64H, 20H
        DC8 63H, 61H, 6EH, 52H, 65H, 61H, 64H, 45H
        DC8 6EH, 61H, 62H, 6CH, 65H, 64H, 3DH, 25H
        DC8 73H, 20H, 63H, 61H, 6EH, 57H, 72H, 69H
        DC8 74H, 65H, 45H, 6EH, 61H, 62H, 6CH, 65H
        DC8 64H, 3DH, 25H, 73H, 20H, 70H, 65H, 72H
        DC8 69H, 6FH, 64H, 3DH, 25H, 64H, 0
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "CAN rx count %d/tx ok...">`:
        DATA
        DC8 "CAN rx count %d/tx ok %d/tx not ok %d"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "CAN TX">`:
        DATA
        DC8 "CAN TX"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "CAN RX">`:
        DATA
        DC8 "CAN RX"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "caninfo">`:
        DATA
        DC8 "caninfo"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "CAN driver">`:
        DATA
        DC8 "CAN driver"
        DC8 0
//   36 
//   37 /*
//   38  * 500KBaud
//   39  * automatic wakeup
//   40  * automatic recover from abort mode
//   41  * See section 22.7.7 on the STM32 reference manual.
//   42  *
//   43  * speed = 42000000 / (BRP + 1) / (1 + TS1 + 1 + TS2 + 1)
//   44  * 42000000 / 7 / 12 = 500000
//   45  *
//   46  *
//   47  */

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//   48 static const CANConfig canConfig = {
canConfig:
        DATA
        DC32 100, 1572870
//   49 CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
//   50 CAN_BTR_SJW(0) | CAN_BTR_TS2(1) | CAN_BTR_TS1(8) | CAN_BTR_BRP(6) };
//   51 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   52 static CANRxFrame rxBuffer;
rxBuffer:
        DS8 20

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   53 CANTxFrame txmsg;
txmsg:
        DS8 16
//   54 
//   55 // todo: we would need a data structure here

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   56 static int engine_rpm = 0;
engine_rpm:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   57 static float engine_clt = 0;
engine_clt:
        DS8 4
//   58 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   59 static int rand = 1212321311;
rand:
        DATA
        DC32 1212321311
//   60 
//   61 //static CANDriver *getCanDevice() {
//   62 //	if(board)
//   63 //}
//   64 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z11printPacketP10CANRxFrame
        THUMB
//   65 static void printPacket(CANRxFrame *rx) {
_Z11printPacketP10CANRxFrame:
        PUSH     {R4-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+20
        SUB      SP,SP,#+36
          CFI CFA R13+56
        MOVS     R4,R0
//   66 //	scheduleMsg(&logger, "CAN FMI %x", rx->FMI);
//   67 //	scheduleMsg(&logger, "TIME %x", rx->TIME);
//   68 	scheduleMsg(&logger, "SID %x/%x %x %x %x %x %x %x %x %x", rx->SID, rx->DLC, rx->data8[0], rx->data8[1],
//   69 			rx->data8[2], rx->data8[3], rx->data8[4], rx->data8[5], rx->data8[6], rx->data8[7]);
        LDRB     R0,[R4, #+19]
        STR      R0,[SP, #+28]
        LDRB     R0,[R4, #+18]
        STR      R0,[SP, #+24]
        LDRB     R0,[R4, #+17]
        STR      R0,[SP, #+20]
        LDRB     R0,[R4, #+16]
        STR      R0,[SP, #+16]
        LDRB     R0,[R4, #+15]
        STR      R0,[SP, #+12]
        LDRB     R0,[R4, #+14]
        STR      R0,[SP, #+8]
        LDRB     R0,[R4, #+13]
        STR      R0,[SP, #+4]
        LDRB     R0,[R4, #+12]
        STR      R0,[SP, #+0]
        LDRB     R0,[R4, #+4]
        ANDS     R3,R0,#0xF
        LDR      R0,[R4, #+8]
        LSLS     R2,R0,#+21       ;; ZeroExtS R2,R0,#+21,#+21
        LSRS     R2,R2,#+21
        LDR.W    R1,??DataTable13_3
        LDR.W    R0,??DataTable13_2
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   70 
//   71 	if (rx->SID == CAN_BMW_E46_CLUSTER_STATUS) {
        LDR      R0,[R4, #+8]
        LSLS     R0,R0,#+21       ;; ZeroExtS R0,R0,#+21,#+21
        LSRS     R0,R0,#+21
        MOVW     R1,#+1555
        CMP      R0,R1
        BNE.N    ??printPacket_0
//   72 		int odometerKm = 10 * (rx->data8[1] << 8) + rx->data8[0];
        LDRB     R0,[R4, #+13]
        LSLS     R0,R0,#+8
        MOVS     R1,#+10
        LDRB     R2,[R4, #+12]
        MLA      R5,R1,R0,R2
//   73 		int odometerMi = (int) (odometerKm * 0.621371);
        MOVS     R0,R5
          CFI FunCall __aeabi_i2d
        BL       __aeabi_i2d
        LDR.W    R2,??DataTable13_4  ;; 0x6f75d9a1
        LDR.W    R3,??DataTable13_5  ;; 0x3fe3e245
          CFI FunCall __aeabi_dmul
        BL       __aeabi_dmul
          CFI FunCall __aeabi_d2iz
        BL       __aeabi_d2iz
        MOVS     R6,R0
//   74 		scheduleMsg(&logger, "GOT odometerKm %d", odometerKm);
        MOVS     R2,R5
        LDR.W    R1,??DataTable13_6
        LDR.W    R0,??DataTable13_2
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   75 		scheduleMsg(&logger, "GOT odometerMi %d", odometerMi);
        MOVS     R2,R6
        LDR.W    R1,??DataTable13_7
        LDR.W    R0,??DataTable13_2
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   76 		int timeValue = (rx->data8[4] << 8) + rx->data8[3];
        LDRB     R0,[R4, #+16]
        LSLS     R0,R0,#+8
        LDRB     R1,[R4, #+15]
        ADDS     R7,R0,R1
//   77 		scheduleMsg(&logger, "GOT time %d", timeValue);
        MOVS     R2,R7
        LDR.W    R1,??DataTable13_8
        LDR.W    R0,??DataTable13_2
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   78 	}
//   79 }
??printPacket_0:
        ADD      SP,SP,#+36
          CFI CFA R13+20
        POP      {R4-R7,PC}       ;; return
          CFI EndBlock cfiBlock1
//   80 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z13setShortValueP10CANTxFrameii
          CFI NoCalls
        THUMB
//   81 static void setShortValue(CANTxFrame *txmsg, int value, int offset) {
_Z13setShortValueP10CANTxFrameii:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
//   82 	txmsg->data8[offset] = value;
        ADDS     R3,R2,R0
        STRB     R1,[R3, #+8]
//   83 	txmsg->data8[offset + 1] = value >> 8;
        ADDS     R3,R2,R0
        ASRS     R4,R1,#+8
        STRB     R4,[R3, #+9]
//   84 }
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock2
//   85 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z8setTxBitii
          CFI NoCalls
        THUMB
//   86 void setTxBit(int offset, int index) {
//   87 	txmsg.data8[offset] = txmsg.data8[offset] | (1 << index);
_Z8setTxBitii:
        LDR.W    R2,??DataTable13_9
        ADDS     R2,R0,R2
        LDRB     R2,[R2, #+8]
        MOVS     R3,#+1
        LSLS     R3,R3,R1
        ORRS     R2,R3,R2
        LDR.W    R3,??DataTable13_9
        ADDS     R3,R0,R3
        STRB     R2,[R3, #+8]
//   88 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock3
//   89 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z12commonTxIniti
        THUMB
//   90 void commonTxInit(int eid) {
_Z12commonTxIniti:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   91 	memset(&txmsg, 0, sizeof(txmsg));
        MOVS     R2,#+16
        MOVS     R1,#+0
        LDR.W    R0,??DataTable13_9
          CFI FunCall memset
        BL       memset
//   92 	txmsg.IDE = CAN_IDE_STD;
        LDR.W    R0,??DataTable13_9
        LDRB     R0,[R0, #+0]
        ANDS     R0,R0,#0xDF
        LDR.W    R1,??DataTable13_9
        STRB     R0,[R1, #+0]
//   93 	txmsg.EID = eid;
        LDR.W    R0,??DataTable13_9
        LDR      R0,[R0, #+4]
        BFI      R0,R4,#+0,#+29
        LDR.W    R1,??DataTable13_9
        STR      R0,[R1, #+4]
//   94 	txmsg.RTR = CAN_RTR_DATA;
        LDR.W    R0,??DataTable13_9
        LDRB     R0,[R0, #+0]
        ANDS     R0,R0,#0xEF
        LDR.W    R1,??DataTable13_9
        STRB     R0,[R1, #+0]
//   95 	txmsg.DLC = 8;
        MOVS     R0,#+8
        LDR.W    R1,??DataTable13_9
        LDRB     R1,[R1, #+0]
        BFI      R1,R0,#+0,#+4
        LDR.W    R0,??DataTable13_9
        STRB     R1,[R0, #+0]
//   96 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock4
//   97 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z12sendMessage2i
        THUMB
//   98 void sendMessage2(int size) {
_Z12sendMessage2i:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   99 	txmsg.DLC = size;
        MOVS     R0,R4
        LDR.W    R1,??DataTable13_9
        LDRB     R1,[R1, #+0]
        BFI      R1,R0,#+0,#+4
        LDR.W    R0,??DataTable13_9
        STRB     R1,[R0, #+0]
//  100 	msg_t result = canTransmit(&EFI_CAN_DEVICE, CAN_ANY_MAILBOX, &txmsg, TIME_INFINITE);
        MOVS     R3,#-1
        LDR.W    R2,??DataTable13_9
        MOVS     R1,#+0
        LDR.W    R0,??DataTable13_10
          CFI FunCall canTransmit
        BL       canTransmit
//  101 	if (result == RDY_OK) {
        CMP      R0,#+0
        BNE.N    ??sendMessage2_0
//  102 		canWriteOk++;
        LDR.W    R1,??DataTable13_11
        LDR      R1,[R1, #+0]
        ADDS     R1,R1,#+1
        LDR.W    R2,??DataTable13_11
        STR      R1,[R2, #+0]
        B.N      ??sendMessage2_1
//  103 	} else {
//  104 		canWriteNotOk++;
??sendMessage2_0:
        LDR.W    R1,??DataTable13_12
        LDR      R1,[R1, #+0]
        ADDS     R1,R1,#+1
        LDR.W    R2,??DataTable13_12
        STR      R1,[R2, #+0]
//  105 	}
//  106 }
??sendMessage2_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock5
//  107 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z11sendMessagev
        THUMB
//  108 void sendMessage() {
_Z11sendMessagev:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  109 	sendMessage2(8);
        MOVS     R0,#+8
          CFI FunCall _Z12sendMessage2i
        BL       _Z12sendMessage2i
//  110 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock6
//  111 
//  112 #if EFI_PROD_CODE
//  113 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z15canDashboardBMWv
        THUMB
//  114 static void canDashboardBMW(void) {
_Z15canDashboardBMWv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  115 	//BMW Dashboard
//  116 	commonTxInit(CAN_BMW_E46_SPEED);
        MOVW     R0,#+339
          CFI FunCall _Z12commonTxIniti
        BL       _Z12commonTxIniti
//  117 	setShortValue(&txmsg, 10 * 8, 1);
        MOVS     R2,#+1
        MOVS     R1,#+80
        LDR.W    R0,??DataTable13_9
          CFI FunCall _Z13setShortValueP10CANTxFrameii
        BL       _Z13setShortValueP10CANTxFrameii
//  118 	sendMessage();
          CFI FunCall _Z11sendMessagev
        BL       _Z11sendMessagev
//  119 
//  120 	commonTxInit(CAN_BMW_E46_RPM);
        MOVW     R0,#+790
          CFI FunCall _Z12commonTxIniti
        BL       _Z12commonTxIniti
//  121 	setShortValue(&txmsg, (int) (engine_rpm * 6.4), 2);
        MOVS     R2,#+2
        MOVS     R4,R2
        LDR.W    R0,??DataTable13_13
        LDR      R0,[R0, #+0]
          CFI FunCall __aeabi_i2d
        BL       __aeabi_i2d
        LDR.W    R2,??DataTable13_14  ;; 0x9999999a
        LDR.W    R3,??DataTable13_15  ;; 0x40199999
          CFI FunCall __aeabi_dmul
        BL       __aeabi_dmul
          CFI FunCall __aeabi_d2iz
        BL       __aeabi_d2iz
        MOVS     R1,R0
        MOVS     R2,R4
        LDR.W    R0,??DataTable13_9
          CFI FunCall _Z13setShortValueP10CANTxFrameii
        BL       _Z13setShortValueP10CANTxFrameii
//  122 	sendMessage();
          CFI FunCall _Z11sendMessagev
        BL       _Z11sendMessagev
//  123 
//  124 	commonTxInit(CAN_BMW_E46_DME2);
        MOVW     R0,#+809
          CFI FunCall _Z12commonTxIniti
        BL       _Z12commonTxIniti
//  125 	setShortValue(&txmsg, (int) ((engine_clt + 48.373) / 0.75), 1);
        MOVS     R2,#+1
        MOVS     R4,R2
        LDR.W    R0,??DataTable13_16
        LDR      R0,[R0, #+0]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        LDR.W    R2,??DataTable13_17  ;; 0x76c8b439
        LDR.W    R3,??DataTable13_18  ;; 0x40482fbe
          CFI FunCall __aeabi_dadd
        BL       __aeabi_dadd
        MOVS     R2,#+0
        LDR.W    R3,??DataTable13_19  ;; 0x3fe80000
          CFI FunCall __aeabi_ddiv
        BL       __aeabi_ddiv
          CFI FunCall __aeabi_d2iz
        BL       __aeabi_d2iz
        MOVS     R1,R0
        MOVS     R2,R4
        LDR.W    R0,??DataTable13_9
          CFI FunCall _Z13setShortValueP10CANTxFrameii
        BL       _Z13setShortValueP10CANTxFrameii
//  126 	sendMessage();
          CFI FunCall _Z11sendMessagev
        BL       _Z11sendMessagev
//  127 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock7
//  128 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z11canMazdaRX8v
        THUMB
//  129 static void canMazdaRX8(void) {
_Z11canMazdaRX8v:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
//  130 	rand = rand * 17;
        LDR.W    R0,??DataTable13_20
        LDR      R0,[R0, #+0]
        MOVS     R1,#+17
        MULS     R0,R1,R0
        LDR.W    R1,??DataTable13_20
        STR      R0,[R1, #+0]
//  131 
//  132 //	commonTxInit(0x300);
//  133 //	sendMessage2(0);
//  134 
//  135 	commonTxInit(CAN_MAZDA_RX_RPM_SPEED);
        MOVW     R0,#+513
          CFI FunCall _Z12commonTxIniti
        BL       _Z12commonTxIniti
//  136 
//  137 #if EFI_VEHICLE_SPEED || defined(__DOXYGEN__)
//  138 	float kph = getVehicleSpeed();
          CFI FunCall _Z15getVehicleSpeedv
        BL       _Z15getVehicleSpeedv
        VMOV.F32 S16,S0
//  139 
//  140 	setShortValue(&txmsg, SWAP_UINT16(engine_rpm * 4), 0);
        MOVS     R2,#+0
        LDR.W    R0,??DataTable13_13
        LDR      R0,[R0, #+0]
        LSLS     R0,R0,#+2
        LDR.W    R1,??DataTable13_13
        LDR      R1,[R1, #+0]
        LSLS     R1,R1,#+2
        ASRS     R1,R1,#+8
        ORRS     R1,R1,R0, LSL #+8
        LDR.W    R0,??DataTable13_9
          CFI FunCall _Z13setShortValueP10CANTxFrameii
        BL       _Z13setShortValueP10CANTxFrameii
//  141 	setShortValue(&txmsg, 0xFFFF, 2);
        MOVS     R2,#+2
        MOVW     R1,#+65535
        LDR.W    R0,??DataTable13_9
          CFI FunCall _Z13setShortValueP10CANTxFrameii
        BL       _Z13setShortValueP10CANTxFrameii
//  142 	setShortValue(&txmsg, SWAP_UINT16((int )(100 * kph + 10000)), 4);
        MOVS     R2,#+4
        VLDR.W   S0,??DataTable11  ;; 0x42c80000
        VLDR.W   S1,??DataTable11_1  ;; 0x461c4000
        VMLA.F32 S1,S16,S0
        VCVT.S32.F32 S0,S1
        VMOV     R0,S0
        VLDR.W   S0,??DataTable11  ;; 0x42c80000
        VLDR.W   S1,??DataTable11_1  ;; 0x461c4000
        VMLA.F32 S1,S16,S0
        VCVT.S32.F32 S0,S1
        VMOV     R1,S0
        ASRS     R1,R1,#+8
        ORRS     R1,R1,R0, LSL #+8
        LDR.W    R0,??DataTable13_9
          CFI FunCall _Z13setShortValueP10CANTxFrameii
        BL       _Z13setShortValueP10CANTxFrameii
//  143 	setShortValue(&txmsg, 0, 6);
        MOVS     R2,#+6
        MOVS     R1,#+0
        LDR.W    R0,??DataTable13_9
          CFI FunCall _Z13setShortValueP10CANTxFrameii
        BL       _Z13setShortValueP10CANTxFrameii
//  144 	sendMessage();
          CFI FunCall _Z11sendMessagev
        BL       _Z11sendMessagev
//  145 #endif /* EFI_VEHICLE_SPEED */
//  146 
//  147 	commonTxInit(CAN_MAZDA_RX_STATUS_2);
        MOV      R0,#+1056
          CFI FunCall _Z12commonTxIniti
        BL       _Z12commonTxIniti
//  148 	txmsg.data8[0] = 0xFE; //Unknown
        LDR.W    R0,??DataTable13_9
        MOVS     R1,#+254
        STRB     R1,[R0, #+8]
//  149 	txmsg.data8[1] = 0xFE; //Unknown
        LDR.N    R0,??DataTable13_9
        MOVS     R1,#+254
        STRB     R1,[R0, #+9]
//  150 	txmsg.data8[2] = 0xFE; //Unknown
        LDR.N    R0,??DataTable13_9
        MOVS     R1,#+254
        STRB     R1,[R0, #+10]
//  151 	txmsg.data8[3] = 0x34; //DSC OFF in combo with byte 5 Live data only seen 0x34
        LDR.N    R0,??DataTable13_9
        MOVS     R1,#+52
        STRB     R1,[R0, #+11]
//  152 	txmsg.data8[4] = 0x00; // B01000000; // Brake warning B00001000;  //ABS warning
        LDR.N    R0,??DataTable13_9
        MOVS     R1,#+0
        STRB     R1,[R0, #+12]
//  153 	txmsg.data8[5] = 0x40; // TCS in combo with byte 3
        LDR.N    R0,??DataTable13_9
        MOVS     R1,#+64
        STRB     R1,[R0, #+13]
//  154 	txmsg.data8[6] = 0x00; // Unknown
        LDR.N    R0,??DataTable13_9
        MOVS     R1,#+0
        STRB     R1,[R0, #+14]
//  155 	txmsg.data8[7] = 0x00; // Unused
        LDR.N    R0,??DataTable13_9
        MOVS     R1,#+0
        STRB     R1,[R0, #+15]
//  156 
//  157 	commonTxInit(CAN_MAZDA_RX_STATUS_2);
        MOV      R0,#+1056
          CFI FunCall _Z12commonTxIniti
        BL       _Z12commonTxIniti
//  158 	txmsg.data8[0] = 0x98; //temp gauge //~170 is red, ~165 last bar, 152 centre, 90 first bar, 92 second bar
        LDR.N    R0,??DataTable13_9
        MOVS     R1,#+152
        STRB     R1,[R0, #+8]
//  159 	txmsg.data8[1] = 0x00; // something to do with trip meter 0x10, 0x11, 0x17 increments by 0.1 miles
        LDR.N    R0,??DataTable13_9
        MOVS     R1,#+0
        STRB     R1,[R0, #+9]
//  160 	txmsg.data8[2] = 0x00; // unknown
        LDR.N    R0,??DataTable13_9
        MOVS     R1,#+0
        STRB     R1,[R0, #+10]
//  161 	txmsg.data8[3] = 0x00; //unknown
        LDR.N    R0,??DataTable13_9
        MOVS     R1,#+0
        STRB     R1,[R0, #+11]
//  162 	txmsg.data8[4] = 0x01; //Oil Pressure (not really a gauge)
        LDR.N    R0,??DataTable13_9
        MOVS     R1,#+1
        STRB     R1,[R0, #+12]
//  163 	txmsg.data8[5] = 0x00; //check engine light
        LDR.N    R0,??DataTable13_9
        MOVS     R1,#+0
        STRB     R1,[R0, #+13]
//  164 	txmsg.data8[6] = 0x00; //Coolant, oil and battery
        LDR.N    R0,??DataTable13_9
        MOVS     R1,#+0
        STRB     R1,[R0, #+14]
//  165 	txmsg.data8[7] = 0x00; //unused
        LDR.N    R0,??DataTable13_9
        MOVS     R1,#+0
        STRB     R1,[R0, #+15]
//  166 	sendMessage();
          CFI FunCall _Z11sendMessagev
        BL       _Z11sendMessagev
//  167 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock8
//  168 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z16canDashboardFiatv
        THUMB
//  169 static void canDashboardFiat(void) {
_Z16canDashboardFiatv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  170 	//Fiat Dashboard
//  171 	commonTxInit(CAN_FIAT_MOTOR_INFO);
        MOVW     R0,#+1377
          CFI FunCall _Z12commonTxIniti
        BL       _Z12commonTxIniti
//  172 	setShortValue(&txmsg, (int) (engine_clt - 40), 3); //Coolant Temp
        MOVS     R2,#+3
        LDR.N    R0,??DataTable13_16
        VLDR     S0,[R0, #0]
        VLDR.W   S1,??DataTable12  ;; 0xc2200000
        VADD.F32 S0,S0,S1
        VCVT.S32.F32 S0,S0
        VMOV     R1,S0
        LDR.N    R0,??DataTable13_9
          CFI FunCall _Z13setShortValueP10CANTxFrameii
        BL       _Z13setShortValueP10CANTxFrameii
//  173 	setShortValue(&txmsg, engine_rpm / 32, 6); //RPM
        MOVS     R2,#+6
        LDR.N    R0,??DataTable13_13
        LDR      R0,[R0, #+0]
        MOVS     R1,#+32
        SDIV     R1,R0,R1
        LDR.N    R0,??DataTable13_9
          CFI FunCall _Z13setShortValueP10CANTxFrameii
        BL       _Z13setShortValueP10CANTxFrameii
//  174 	sendMessage();
          CFI FunCall _Z11sendMessagev
        BL       _Z11sendMessagev
//  175 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock9
//  176 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z15canDashboardVAGv
        THUMB
//  177 static void canDashboardVAG(void) {
_Z15canDashboardVAGv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  178 	//VAG Dashboard
//  179 	commonTxInit(CAN_VAG_RPM);
        MOV      R0,#+640
          CFI FunCall _Z12commonTxIniti
        BL       _Z12commonTxIniti
//  180 	setShortValue(&txmsg, engine_rpm * 4, 2); //RPM
        MOVS     R2,#+2
        LDR.N    R0,??DataTable13_13
        LDR      R0,[R0, #+0]
        LSLS     R1,R0,#+2
        LDR.N    R0,??DataTable13_9
          CFI FunCall _Z13setShortValueP10CANTxFrameii
        BL       _Z13setShortValueP10CANTxFrameii
//  181 	sendMessage();
          CFI FunCall _Z11sendMessagev
        BL       _Z11sendMessagev
//  182 
//  183 	commonTxInit(CAN_VAG_CLT);
        MOVW     R0,#+649
          CFI FunCall _Z12commonTxIniti
        BL       _Z12commonTxIniti
//  184 	setShortValue(&txmsg, (int) ((engine_clt + 48.373) / 0.75), 1); //Coolant Temp
        MOVS     R2,#+1
        MOVS     R4,R2
        LDR.N    R0,??DataTable13_16
        LDR      R0,[R0, #+0]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        LDR.N    R2,??DataTable13_17  ;; 0x76c8b439
        LDR.N    R3,??DataTable13_18  ;; 0x40482fbe
          CFI FunCall __aeabi_dadd
        BL       __aeabi_dadd
        MOVS     R2,#+0
        LDR.N    R3,??DataTable13_19  ;; 0x3fe80000
          CFI FunCall __aeabi_ddiv
        BL       __aeabi_ddiv
          CFI FunCall __aeabi_d2iz
        BL       __aeabi_d2iz
        MOVS     R1,R0
        MOVS     R2,R4
        LDR.N    R0,??DataTable13_9
          CFI FunCall _Z13setShortValueP10CANTxFrameii
        BL       _Z13setShortValueP10CANTxFrameii
//  185 	sendMessage();
          CFI FunCall _Z11sendMessagev
        BL       _Z11sendMessagev
//  186 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock10
//  187 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z19canInfoNBCBroadcast9can_nbc_e
        THUMB
//  188 static void canInfoNBCBroadcast(can_nbc_e typeOfNBC) {
_Z19canInfoNBCBroadcast9can_nbc_e:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  189 	switch (typeOfNBC) {
        CMP      R4,#+0
        BEQ.N    ??canInfoNBCBroadcast_0
        CMP      R4,#+2
        BEQ.N    ??canInfoNBCBroadcast_1
        BCC.N    ??canInfoNBCBroadcast_2
        CMP      R4,#+3
        BEQ.N    ??canInfoNBCBroadcast_3
        B.N      ??canInfoNBCBroadcast_4
//  190 	case CAN_BUS_NBC_BMW:
//  191 		canDashboardBMW();
??canInfoNBCBroadcast_0:
          CFI FunCall _Z15canDashboardBMWv
        BL       _Z15canDashboardBMWv
//  192 		break;
        B.N      ??canInfoNBCBroadcast_5
//  193 	case CAN_BUS_NBC_FIAT:
//  194 		canDashboardFiat();
??canInfoNBCBroadcast_2:
          CFI FunCall _Z16canDashboardFiatv
        BL       _Z16canDashboardFiatv
//  195 		break;
        B.N      ??canInfoNBCBroadcast_5
//  196 	case CAN_BUS_NBC_VAG:
//  197 		canDashboardVAG();
??canInfoNBCBroadcast_1:
          CFI FunCall _Z15canDashboardVAGv
        BL       _Z15canDashboardVAGv
//  198 		break;
        B.N      ??canInfoNBCBroadcast_5
//  199 	case CAN_BUS_MAZDA_RX8:
//  200 		canMazdaRX8();
??canInfoNBCBroadcast_3:
          CFI FunCall _Z11canMazdaRX8v
        BL       _Z11canMazdaRX8v
//  201 		break;
        B.N      ??canInfoNBCBroadcast_5
//  202 	default:
//  203 		break;
//  204 	}
//  205 }
??canInfoNBCBroadcast_4:
??canInfoNBCBroadcast_5:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock11
//  206 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _Z7canReadv
        THUMB
//  207 static void canRead(void) {
_Z7canReadv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  208 	scheduleMsg(&logger, "waiting for CAN");
        LDR.N    R1,??DataTable13_21
        LDR.N    R0,??DataTable13_2
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  209 	canReceive(&EFI_CAN_DEVICE, CAN_ANY_MAILBOX, &rxBuffer, TIME_INFINITE);
        MOVS     R3,#-1
        LDR.N    R2,??DataTable13_22
        MOVS     R1,#+0
        LDR.N    R0,??DataTable13_10
          CFI FunCall canReceive
        BL       canReceive
//  210 
//  211 	canReadCounter++;
        LDR.N    R0,??DataTable13_23
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable13_23
        STR      R0,[R1, #+0]
//  212 	printPacket(&rxBuffer);
        LDR.N    R0,??DataTable13_22
          CFI FunCall _Z11printPacketP10CANRxFrame
        BL       _Z11printPacketP10CANRxFrame
//  213 	obdOnCanPacketRx(&rxBuffer);
        LDR.N    R0,??DataTable13_22
          CFI FunCall _Z16obdOnCanPacketRxP10CANRxFrame
        BL       _Z16obdOnCanPacketRxP10CANRxFrame
//  214 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock12
//  215 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z15writeStateToCanv
        THUMB
//  216 static void writeStateToCan(void) {
_Z15writeStateToCanv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  217 	engine_rpm = getRpm();
        LDR.N    R0,??DataTable13_24
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN13RpmCalculator3rpmEv
        BL       _ZN13RpmCalculator3rpmEv
        LDR.N    R1,??DataTable13_13
        STR      R0,[R1, #+0]
//  218 	engine_clt = 123; //getCoolantTemperature(engine);
        LDR.N    R0,??DataTable13_16
        LDR.N    R1,??DataTable13_25  ;; 0x42f60000
        STR      R1,[R0, #+0]
//  219 
//  220 	canInfoNBCBroadcast(engineConfiguration->canNbcType);
        LDR.N    R0,??DataTable13_26
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+492]
          CFI FunCall _Z19canInfoNBCBroadcast9can_nbc_e
        BL       _Z19canInfoNBCBroadcast9can_nbc_e
//  221 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock13
//  222 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _Z9canThreadPv
        THUMB
//  223 static msg_t canThread(void *arg) {
_Z9canThreadPv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  224 	chRegSetThreadName("CAN");
        LDR.N    R0,??DataTable13_27
        LDR      R0,[R0, #+24]
        ADR.N    R1,??DataTable13  ;; "CAN"
        STR      R1,[R0, #+24]
//  225 	while (true) {
//  226 		if (engineConfiguration->canWriteEnabled)
??canThread_0:
        LDR.N    R0,??DataTable13_26
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+1
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??canThread_1
//  227 			writeStateToCan();
          CFI FunCall _Z15writeStateToCanv
        BL       _Z15writeStateToCanv
//  228 
//  229 		if (engineConfiguration->canReadEnabled)
??canThread_1:
        LDR.N    R0,??DataTable13_26
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??canThread_2
//  230 			canRead(); // todo: since this is a blocking operation, do we need a separate thread for 'write'?
          CFI FunCall _Z7canReadv
        BL       _Z7canReadv
//  231 
//  232 		if (engineConfiguration->canSleepPeriod < 10) {
??canThread_2:
        LDR.N    R0,??DataTable13_26
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+496]
        CMP      R0,#+10
        BGE.N    ??canThread_3
//  233 			warning(OBD_PCM_Processor_Fault, "%d too low CAN", engineConfiguration->canSleepPeriod);
        LDR.N    R0,??DataTable13_26
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+496]
        LDR.N    R1,??DataTable13_28
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  234 			engineConfiguration->canSleepPeriod = 50;
        LDR.N    R0,??DataTable13_26
        LDR      R0,[R0, #+0]
        MOVS     R1,#+50
        STR      R1,[R0, #+496]
//  235 		}
//  236 
//  237 		chThdSleepMilliseconds(engineConfiguration->canSleepPeriod);
??canThread_3:
        LDR.N    R0,??DataTable13_26
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+496]
        MOV      R1,#+1000
        MULS     R0,R1,R0
        SUBS     R0,R0,#+1
        MOV      R1,#+1000
        UDIV     R0,R0,R1
        ADDS     R0,R0,#+1
          CFI FunCall chThdSleep
        BL       chThdSleep
        B.N      ??canThread_0
          CFI EndBlock cfiBlock14
//  238 	}
//  239 #if defined __GNUC__
//  240 	return -1;
//  241 #endif
//  242 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11:
        DC32     0x42c80000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_1:
        DC32     0x461c4000
//  243 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function _Z7canInfov
        THUMB
//  244 static void canInfo(void) {
_Z7canInfov:
        PUSH     {R2-R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+16
//  245 	scheduleMsg(&logger, "CAN TX %s", hwPortname(boardConfiguration->canTxPin));
        LDR.N    R0,??DataTable13_29
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+344]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R2,R0
        LDR.N    R1,??DataTable13_30
        LDR.N    R0,??DataTable13_2
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  246 	scheduleMsg(&logger, "CAN RX %s", hwPortname(boardConfiguration->canRxPin));
        LDR.N    R0,??DataTable13_29
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+348]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R2,R0
        LDR.N    R1,??DataTable13_31
        LDR.N    R0,??DataTable13_2
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  247 	scheduleMsg(&logger, "type=%d canReadEnabled=%s canWriteEnabled=%s period=%d", engineConfiguration->canNbcType,
//  248 			boolToString(engineConfiguration->canReadEnabled), boolToString(engineConfiguration->canWriteEnabled),
//  249 			engineConfiguration->canSleepPeriod);
        LDR.N    R0,??DataTable13_26
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+1
        ANDS     R0,R0,#0x1
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R4,R0
        LDR.N    R0,??DataTable13_26
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        ANDS     R0,R0,#0x1
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        LDR.N    R1,??DataTable13_26
        LDR      R1,[R1, #+0]
        LDR      R1,[R1, #+496]
        STR      R1,[SP, #+4]
        STR      R4,[SP, #+0]
        MOVS     R3,R0
        LDR.N    R0,??DataTable13_26
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+492]
        LDR.N    R1,??DataTable13_32
        LDR.N    R0,??DataTable13_2
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  250 
//  251 	scheduleMsg(&logger, "CAN rx count %d/tx ok %d/tx not ok %d", canReadCounter, canWriteOk, canWriteNotOk);
        LDR.N    R0,??DataTable13_12
        LDR      R0,[R0, #+0]
        STR      R0,[SP, #+0]
        LDR.N    R0,??DataTable13_11
        LDR      R3,[R0, #+0]
        LDR.N    R0,??DataTable13_23
        LDR      R2,[R0, #+0]
        LDR.N    R1,??DataTable13_33
        LDR.N    R0,??DataTable13_2
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  252 }
        POP      {R0,R1,R4,PC}    ;; return
          CFI EndBlock cfiBlock15

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12:
        DC32     0xc2200000

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function _Z7initCanv
        THUMB
_Z7initCanv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        LDR.N    R0,??DataTable13_26
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+6
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??initCan_0
??initCan_1:
        LDR.N    R1,??DataTable13_34
        LDR.N    R0,??DataTable13_35
          CFI FunCall canStart
        BL       canStart
        LDR.N    R1,??DataTable13_34
        LDR.N    R0,??DataTable13_10
          CFI FunCall canStart
        BL       canStart
        LDR.N    R1,??DataTable13_34
        LDR.N    R0,??DataTable13_10
          CFI FunCall canStart
        BL       canStart
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        LDR.N    R3,??DataTable13_36
        MOVS     R2,#+64
        MOV      R1,#+712
        LDR.N    R0,??DataTable13_37
          CFI FunCall chThdCreateStatic
        BL       chThdCreateStatic
        MOVW     R2,#+1154
        LDR.N    R0,??DataTable13_29
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+344]
        LDR.N    R0,??DataTable13_38
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
        MOVW     R2,#+1154
        LDR.N    R0,??DataTable13_29
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+348]
        LDR.N    R0,??DataTable13_39
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
        LDR.N    R1,??DataTable13_40
        LDR.N    R0,??DataTable13_41
          CFI FunCall addConsoleAction
        BL       addConsoleAction
??initCan_0:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock16

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13:
        DC8      "CAN"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_1:
        DC32     `?<Constant "CAN driver">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_2:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_3:
        DC32     `?<Constant "SID %x/%x %x %x %x %x...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_4:
        DC32     0x6f75d9a1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_5:
        DC32     0x3fe3e245

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_6:
        DC32     `?<Constant "GOT odometerKm %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_7:
        DC32     `?<Constant "GOT odometerMi %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_8:
        DC32     `?<Constant "GOT time %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_9:
        DC32     txmsg

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_10:
        DC32     CAND2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_11:
        DC32     canWriteOk

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_12:
        DC32     canWriteNotOk

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_13:
        DC32     engine_rpm

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_14:
        DC32     0x9999999a

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_15:
        DC32     0x40199999

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_16:
        DC32     engine_clt

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_17:
        DC32     0x76c8b439

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_18:
        DC32     0x40482fbe

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_19:
        DC32     0x3fe80000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_20:
        DC32     rand

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_21:
        DC32     `?<Constant "waiting for CAN">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_22:
        DC32     rxBuffer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_23:
        DC32     canReadCounter

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_24:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_25:
        DC32     0x42f60000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_26:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_27:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_28:
        DC32     `?<Constant "%d too low CAN">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_29:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_30:
        DC32     `?<Constant "CAN TX %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_31:
        DC32     `?<Constant "CAN RX %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_32:
        DC32     `?<Constant "type=%d canReadEnable...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_33:
        DC32     `?<Constant "CAN rx count %d/tx ok...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_34:
        DC32     canConfig

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_35:
        DC32     CAND1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_36:
        DC32     _Z9canThreadPv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_37:
        DC32     canTreadStack

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_38:
        DC32     `?<Constant "CAN TX">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_39:
        DC32     `?<Constant "CAN RX">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_40:
        DC32     _Z7canInfov

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_41:
        DC32     `?<Constant "caninfo">`

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memset
          CFI Block cfiBlock17 Using cfiCommon0
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
          CFI EndBlock cfiBlock17

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

        END
//  253 
//  254 #endif /* EFI_PROD_CODE */
//  255 
//  256 
//  257 void initCan(void) {
//  258 #if EFI_PROD_CODE
//  259 	if (!engineConfiguration->isCanEnabled)
//  260 		return;
//  261 #endif /* EFI_PROD_CODE */
//  262 
//  263 #if STM32_CAN_USE_CAN2
//  264 	// CAN1 is required for CAN2
//  265 	canStart(&CAND1, &canConfig);
//  266 	canStart(&CAND2, &canConfig);
//  267 #else
//  268 	canStart(&CAND1, &canConfig);
//  269 #endif
//  270 
//  271 	canStart(&EFI_CAN_DEVICE, &canConfig);
//  272 #if EFI_PROD_CODE
//  273 
//  274 	chThdCreateStatic(canTreadStack, sizeof(canTreadStack), NORMALPRIO, (tfunc_t) canThread, NULL);
//  275 
//  276 	mySetPadMode2("CAN TX", boardConfiguration->canTxPin, PAL_MODE_ALTERNATE(EFI_CAN_TX_AF));
//  277 	mySetPadMode2("CAN RX", boardConfiguration->canRxPin, PAL_MODE_ALTERNATE(EFI_CAN_RX_AF));
//  278 
//  279 	addConsoleAction("caninfo", canInfo);
//  280 #endif /* EFI_PROD_CODE */
//  281 }
//  282 
//  283 #endif /* EFI_CAN_SUPPORT */
// 
// 1 028 bytes in section .bss
//     4 bytes in section .data
//     4 bytes in section .init_array
//   288 bytes in section .rodata
// 1 672 bytes in section .text
// 
// 1 654 bytes of CODE  memory (+ 22 bytes shared)
//   288 bytes of CONST memory
// 1 032 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
