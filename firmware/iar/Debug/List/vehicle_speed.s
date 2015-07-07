///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:57 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\vehicl /
//                       e_speed.cpp                                          /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\vehicl /
//                       e_speed.cpp -lCN F:\stuff\rusefi_sourceforge\firmwar /
//                       e\iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge\fi /
//                       rmware\iar\Debug\List\ -o                            /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\Debug\Obj\  /
//                       --no_cse --no_unroll --no_inline --no_code_motion    /
//                       --no_tbaa --no_clustering --no_scheduling --debug    /
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
//                       vehicle_speed.s                                      /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME vehicle_speed

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z10hwPortname11brain_pin_e
        EXTERN _Z16startInputDriverP15digital_input_sb
        EXTERN _Z22initWaveAnalyzerDriverPKc11brain_pin_e
        EXTERN _ZN16IntListenerArray16registerCallbackEPFviEPv
        EXTERN __aeabi_f2d
        EXTERN __aeabi_l2f
        EXTERN addConsoleAction
        EXTERN boardConfiguration
        EXTERN engineConfiguration
        EXTERN getTimeNowNt
        EXTERN scheduleMsg

        PUBLIC _Z15getVehicleSpeedv
        PUBLIC _Z16initVehicleSpeedP7Logging
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\hw_layer\vehicle_speed.cpp
//    1 /**
//    2  * @file vehicle_speed.cpp
//    3  *
//    4  * @date Dec 26, 2014
//    5  * @author Andrey Belomutskiy, (c) 2012-2015
//    6  */
//    7 
//    8 #include "vehicle_speed.h"
//    9 
//   10 #if EFI_VEHICLE_SPEED || defined(__DOXYGEN__)
//   11 
//   12 #include "engine.h"
//   13 #include "digital_input_hw.h"
//   14 #include "pin_repository.h"
//   15 
//   16 EXTERN_ENGINE
//   17 ;
//   18 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   19 static Logging *logger;
logger:
        DS8 4
//   20 

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   21 static efitick_t lastSignalTimeNt = 0;
lastSignalTimeNt:
        DS8 8

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   22 static efitick_t vssDiff = 0;
vssDiff:
        DS8 8

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   23 static int vssCounter = 0;
vssCounter:
        DS8 4
//   24 
//   25 /**
//   26  * @return vehicle speed, in kilometers per hour
//   27  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z15getVehicleSpeedv
        THUMB
//   28 float getVehicleSpeed(void) {
_Z15getVehicleSpeedv:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        VPUSH    {D8}
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
//   29 	efitick_t nowNt = getTimeNowNt();
          CFI FunCall getTimeNowNt
        BL       getTimeNowNt
        MOVS     R4,R0
        MOVS     R5,R1
//   30 	if (nowNt - lastSignalTimeNt > US2NT(US_PER_SECOND_LL))
        LDR.N    R0,??DataTable3_3
        LDRD     R2,R3,[R0, #+0]
        SUBS     R0,R4,R2
        SBCS     R1,R5,R3
        LDR.N    R2,??DataTable3_4  ;; 0xa037a01
        MOVS     R3,#+0
        CMP      R1,R3
        BLT.N    ??getVehicleSpeed_0
        BGT.N    ??getVehicleSpeed_1
        CMP      R0,R2
        BCC.N    ??getVehicleSpeed_0
//   31 		return 0; // previous signal time is too long ago - we are stopped
??getVehicleSpeed_1:
        VLDR.W   S0,??DataTable3  ;; 0x0
        B.N      ??getVehicleSpeed_2
//   32 
//   33 	return engineConfiguration->vehicleSpeedCoef * US2NT(US_PER_SECOND_LL) / vssDiff;
??getVehicleSpeed_0:
        LDR.N    R0,??DataTable3_5
        LDR      R0,[R0, #+0]
        VLDR     S0,[R0, #+488]
        VLDR.W   S1,??DataTable3_1  ;; 0x4d2037a0
        VMUL.F32 S16,S0,S1
        LDR.N    R2,??DataTable3_6
        LDRD     R0,R1,[R2, #+0]
          CFI FunCall __aeabi_l2f
        BL       __aeabi_l2f
        VMOV     S0,R0
        VDIV.F32 S0,S16,S0
??getVehicleSpeed_2:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock0
//   34 }
//   35 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z18vsAnaWidthCallbackv
        THUMB
//   36 static void vsAnaWidthCallback(void) {
_Z18vsAnaWidthCallbackv:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
//   37 	vssCounter++;
        LDR.N    R0,??DataTable3_7
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable3_7
        STR      R0,[R1, #+0]
//   38 	efitick_t nowNt = getTimeNowNt();
          CFI FunCall getTimeNowNt
        BL       getTimeNowNt
//   39 	vssDiff = nowNt - lastSignalTimeNt;
        LDR.N    R2,??DataTable3_3
        LDRD     R4,R5,[R2, #+0]
        SUBS     R2,R0,R4
        SBCS     R3,R1,R5
        LDR.N    R4,??DataTable3_6
        STRD     R2,R3,[R4, #+0]
//   40 	lastSignalTimeNt = nowNt;
        LDR.N    R2,??DataTable3_3
        STRD     R0,R1,[R2, #+0]
//   41 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock1
//   42 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z9speedInfov
        THUMB
//   43 static void speedInfo(void) {
_Z9speedInfov:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+24
          CFI CFA R13+40
//   44 	scheduleMsg(logger, "VSS@%s c=%f eventCounter=%d speed=%f",
//   45 			hwPortname(boardConfiguration->vehicleSpeedSensorInputPin),
//   46 			engineConfiguration->vehicleSpeedCoef,
//   47 			vssCounter,
//   48 			getVehicleSpeed());
          CFI FunCall _Z15getVehicleSpeedv
        BL       _Z15getVehicleSpeedv
        VMOV.F32 S16,S0
        LDR.N    R0,??DataTable3_8
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+792]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R4,R0
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+16]
        LDR.N    R0,??DataTable3_7
        LDR      R0,[R0, #+0]
        STR      R0,[SP, #+8]
        LDR.N    R0,??DataTable3_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+488]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        MOVS     R2,R4
        LDR.N    R1,??DataTable3_9
        LDR.N    R0,??DataTable3_10
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   49 	scheduleMsg(logger, "vss diff %d", vssDiff);
        LDR.N    R0,??DataTable3_6
        LDRD     R2,R3,[R0, #+0]
        LDR.N    R1,??DataTable3_11
        LDR.N    R0,??DataTable3_10
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   50 
//   51 }
        ADD      SP,SP,#+24
          CFI CFA R13+16
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2
//   52 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z16initVehicleSpeedP7Logging
        THUMB
//   53 void initVehicleSpeed(Logging *l) {
_Z16initVehicleSpeedP7Logging:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//   54 	logger = l;
        LDR.N    R0,??DataTable3_10
        STR      R4,[R0, #+0]
//   55 	if (boardConfiguration->vehicleSpeedSensorInputPin == GPIO_UNASSIGNED)
        LDR.N    R0,??DataTable3_8
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+792]
        CMP      R0,#+80
        BEQ.N    ??initVehicleSpeed_0
//   56 		return;
//   57 	digital_input_s* vehicleSpeedInput = initWaveAnalyzerDriver("VSS", boardConfiguration->vehicleSpeedSensorInputPin);
??initVehicleSpeed_1:
        LDR.N    R0,??DataTable3_8
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+792]
        ADR.N    R0,??DataTable3_2  ;; "VSS"
          CFI FunCall _Z22initWaveAnalyzerDriverPKc11brain_pin_e
        BL       _Z22initWaveAnalyzerDriverPKc11brain_pin_e
        MOVS     R5,R0
//   58 	startInputDriver(vehicleSpeedInput, true);
        MOVS     R1,#+1
        MOVS     R0,R5
          CFI FunCall _Z16startInputDriverP15digital_input_sb
        BL       _Z16startInputDriverP15digital_input_sb
//   59 
//   60 	vehicleSpeedInput->widthListeners.registerCallback((VoidInt) vsAnaWidthCallback, NULL);
        MOVS     R2,#+0
        LDR.N    R1,??DataTable3_12
        ADDS     R0,R5,#+8
          CFI FunCall _ZN16IntListenerArray16registerCallbackEPFviEPv
        BL       _ZN16IntListenerArray16registerCallbackEPFviEPv
//   61 	addConsoleAction("speedinfo", speedInfo);
        LDR.N    R1,??DataTable3_13
        LDR.N    R0,??DataTable3_14
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//   62 }
??initVehicleSpeed_0:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3:
        DC32     0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_1:
        DC32     0x4d2037a0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_2:
        DC8      "VSS"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_3:
        DC32     lastSignalTimeNt

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_4:
        DC32     0xa037a01

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_5:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_6:
        DC32     vssDiff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_7:
        DC32     vssCounter

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_8:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_9:
        DC32     `?<Constant "VSS@%s c=%f eventCoun...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_10:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_11:
        DC32     `?<Constant "vss diff %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_12:
        DC32     _Z18vsAnaWidthCallbackv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_13:
        DC32     _Z9speedInfov

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_14:
        DC32     `?<Constant "speedinfo">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "VSS@%s c=%f eventCoun...">`:
        DATA
        DC8 "VSS@%s c=%f eventCounter=%d speed=%f"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "vss diff %d">`:
        DATA
        DC8 "vss diff %d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "VSS"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "speedinfo">`:
        DATA
        DC8 "speedinfo"
        DC8 0, 0

        END
//   63 
//   64 #endif /* EFI_VEHICLE_SPEED */
// 
//  24 bytes in section .bss
//  68 bytes in section .rodata
// 354 bytes in section .text
// 
// 354 bytes of CODE  memory
//  68 bytes of CONST memory
//  24 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
