///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:31 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\inj /
//                       ector_central.cpp                                    /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\inj /
//                       ector_central.cpp -lCN F:\stuff\rusefi_sourceforge\f /
//                       irmware\iar\Debug\List\ -lA                          /
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
//                       injector_central.s                                   /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME injector_central

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z10hwPortname11brain_pin_e
        EXTERN _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
        EXTERN _Z9unmarkPin11brain_pin_e
        EXTERN _ZN18LoggingWithStorageC1EPKc
        EXTERN _ZN9OutputPin10unregisterEv
        EXTERN _ZN9OutputPin8setValueEi
        EXTERN __aeabi_f2d
        EXTERN activeConfiguration
        EXTERN addConsoleAction
        EXTERN addConsoleActionII
        EXTERN addConsoleActionSSS
        EXTERN addConsoleActionSSSSS
        EXTERN atoff
        EXTERN atoi
        EXTERN boardConfiguration
        EXTERN chThdCreateStatic
        EXTERN chThdSleep
        EXTERN checkEnginePin
        EXTERN engineConfiguration
        EXTERN enginePins
        EXTERN firmwareError
        EXTERN print
        EXTERN rlist
        EXTERN scheduleMsg

        PUBLIC _Z13fuelPumpBenchv
        PUBLIC _Z16assertCylinderIdiPKc
        PUBLIC _Z16stopIgnitionPinsv
        PUBLIC _Z17isInjectorEnabledi
        PUBLIC _Z17startIgnitionPinsv
        PUBLIC _Z17stopInjectionPinsv
        PUBLIC _Z18isRunningBenchTestv
        PUBLIC _Z18startInjectionPinsv
        PUBLIC _Z19initInjectorCentralv
        PUBLIC _Z8fanBenchv
        PUBLIC _Z8milBenchv
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\injector_central.cpp
//    1 /**
//    2  * @file    injector_central.cpp
//    3  * @brief	Utility methods related to fuel injection.
//    4  *
//    5  *
//    6  * @date Sep 8, 2013
//    7  * @author Andrey Belomutskiy, (c) 2012-2015
//    8  *
//    9  * This file is part of rusEfi - see http://rusefi.com
//   10  *
//   11  * rusEfi is free software; you can redistribute it and/or modify it under the terms of
//   12  * the GNU General Public License as published by the Free Software Foundation; either
//   13  * version 3 of the License, or (at your option) any later version.
//   14  *
//   15  * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
//   16  * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   17  * GNU General Public License for more details.
//   18  *
//   19  * You should have received a copy of the GNU General Public License along with this program.
//   20  * If not, see <http://www.gnu.org/licenses/>.
//   21  */
//   22 
//   23 // todo: rename this file
//   24 #include "main.h"
//   25 
//   26 #if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
//   27 
//   28 #include "injector_central.h"
//   29 #include "io_pins.h"
//   30 #include "signal_executor.h"
//   31 #include "main_trigger_callback.h"
//   32 #include "engine_configuration.h"
//   33 #include "pin_repository.h"
//   34 #include "efiGpio.h"
//   35 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   36 static LoggingWithStorage logger("InjectorCentral");
        LDR.W    R1,??DataTable21
        LDR.W    R0,??DataTable21_1
          CFI FunCall _ZN18LoggingWithStorageC1EPKc
        BL       _ZN18LoggingWithStorageC1EPKc
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
logger:
        DS8 260
//   37 EXTERN_ENGINE
//   38 ;
//   39 

        SECTION `.bss`:DATA:REORDER:NOROOT(0)
//   40 static bool_t isRunningBench = false;
isRunningBench:
        DS8 1
//   41 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   42 static int is_injector_enabled[MAX_INJECTOR_COUNT];
is_injector_enabled:
        DS8 48
//   43 
//   44 extern engine_pins_s enginePins;
//   45 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z18isRunningBenchTestv
          CFI NoCalls
        THUMB
//   46 bool_t isRunningBenchTest(void) {
//   47 	return isRunningBench;
_Z18isRunningBenchTestv:
        LDR.W    R0,??DataTable21_2
        LDRB     R0,[R0, #+0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   48 }
//   49 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z16assertCylinderIdiPKc
        THUMB
//   50 void assertCylinderId(int cylinderId, const char *msg) {
_Z16assertCylinderIdiPKc:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   51 	int isValid = cylinderId >= 1 && cylinderId <= engineConfiguration->specs.cylindersCount;
        CMP      R4,#+1
        BLT.N    ??assertCylinderId_0
        LDR.W    R0,??DataTable21_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+404]
        CMP      R0,R4
        BLT.N    ??assertCylinderId_0
        MOVS     R6,#+1
        B.N      ??assertCylinderId_1
??assertCylinderId_0:
        MOVS     R6,#+0
??assertCylinderId_1:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
//   52 	if (!isValid) {
        CMP      R6,#+0
        BNE.N    ??assertCylinderId_2
//   53 		// we are here only in case of a fatal issue - at this point it is fine to make some blocking i-o
//   54 		//scheduleSimpleMsg(&logger, "cid=", cylinderId);
//   55 		print("ERROR [%s] cid=%d\r\n", msg, cylinderId);
        MOVS     R2,R4
        MOVS     R1,R5
        LDR.W    R0,??DataTable21_4
          CFI FunCall print
        BL       print
//   56 		efiAssertVoid(false, "Cylinder ID");
        LDR.W    R0,??DataTable21_5
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??assertCylinderId_3
//   57 	}
//   58 }
??assertCylinderId_2:
??assertCylinderId_3:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock2
//   59 
//   60 /**
//   61  * @param cylinderId - from 1 to NUMBER_OF_CYLINDERS
//   62  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z17isInjectorEnabledi
        THUMB
//   63 int isInjectorEnabled(int cylinderId) {
_Z17isInjectorEnabledi:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   64 	assertCylinderId(cylinderId, "isInjectorEnabled");
        LDR.W    R1,??DataTable21_6
        MOVS     R0,R4
          CFI FunCall _Z16assertCylinderIdiPKc
        BL       _Z16assertCylinderIdiPKc
//   65 	return is_injector_enabled[cylinderId - 1];
        LDR.W    R0,??DataTable21_7
        ADDS     R0,R0,R4, LSL #+2
        LDR      R0,[R0, #-4]
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//   66 }
//   67 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z11printStatusv
        THUMB
//   68 static void printStatus(void) {
_Z11printStatusv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//   69 	for (int id = 1; id <= engineConfiguration->specs.cylindersCount; id++) {
        MOVS     R4,#+1
??printStatus_0:
        LDR.W    R0,??DataTable21_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+404]
        CMP      R0,R4
        BLT.N    ??printStatus_1
//   70 		scheduleMsg(&logger, "injector_%d_%d", isInjectorEnabled(id));
        MOVS     R0,R4
          CFI FunCall _Z17isInjectorEnabledi
        BL       _Z17isInjectorEnabledi
        MOVS     R2,R0
        LDR.W    R1,??DataTable21_8
        LDR.W    R0,??DataTable21_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   71 	}
        ADDS     R4,R4,#+1
        B.N      ??printStatus_0
//   72 }
??printStatus_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock4
//   73 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z18setInjectorEnabledii
        THUMB
//   74 static void setInjectorEnabled(int id, int value) {
_Z18setInjectorEnabledii:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   75 	efiAssertVoid(id >= 0 && id < engineConfiguration->specs.cylindersCount, "injector id");
        CMP      R4,#+0
        BMI.N    ??setInjectorEnabled_0
        LDR.W    R0,??DataTable21_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+404]
        CMP      R4,R0
        BLT.N    ??setInjectorEnabled_1
??setInjectorEnabled_0:
        LDR.W    R0,??DataTable21_9
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??setInjectorEnabled_2
//   76 	is_injector_enabled[id] = value;
??setInjectorEnabled_1:
        LDR.W    R0,??DataTable21_7
        STR      R5,[R0, R4, LSL #+2]
//   77 	printStatus();
          CFI FunCall _Z11printStatusv
        BL       _Z11printStatusv
//   78 }
??setInjectorEnabled_2:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock5
//   79 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z8runBench11brain_pin_eP9OutputPinfffi
        THUMB
//   80 static void runBench(brain_pin_e brainPin, OutputPin *output, float delayMs, float onTimeMs, float offTimeMs,
//   81 		int count) {
_Z8runBench11brain_pin_eP9OutputPinfffi:
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
        VPUSH    {D8-D9}
          CFI D9 Frame(CFA, -40)
          CFI D8 Frame(CFA, -48)
          CFI CFA R13+48
        SUB      SP,SP,#+16
          CFI CFA R13+64
        MOV      R9,R0
        MOVS     R4,R1
        VMOV.F32 S16,S0
        VMOV.F32 S17,S1
        VMOV.F32 S18,S2
        MOV      R8,R2
//   82 	int delaySt = (int) (delayMs * CH_FREQUENCY / 1000);
        VLDR.W   S0,??DataTable10  ;; 0x447a0000
        VMUL.F32 S0,S16,S0
        VLDR.W   S1,??DataTable10  ;; 0x447a0000
        VDIV.F32 S0,S0,S1
        VCVT.S32.F32 S0,S0
        VMOV     R5,S0
//   83 	int onTimeSt = (int) (onTimeMs * CH_FREQUENCY / 1000);
        VLDR.W   S0,??DataTable10  ;; 0x447a0000
        VMUL.F32 S0,S17,S0
        VLDR.W   S1,??DataTable10  ;; 0x447a0000
        VDIV.F32 S0,S0,S1
        VCVT.S32.F32 S0,S0
        VMOV     R6,S0
//   84 	int offTimeSt = (int) (offTimeMs * CH_FREQUENCY / 1000);
        VLDR.W   S0,??DataTable10  ;; 0x447a0000
        VMUL.F32 S0,S18,S0
        VLDR.W   S1,??DataTable10  ;; 0x447a0000
        VDIV.F32 S0,S0,S1
        VCVT.S32.F32 S0,S0
        VMOV     R7,S0
//   85 	if (delaySt < 0) {
        CMP      R5,#+0
        BPL.N    ??runBench_0
//   86 		scheduleMsg(&logger, "Invalid delay %f", delayMs);
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable21_10
        LDR.W    R0,??DataTable21_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   87 		return;
        B.N      ??runBench_1
//   88 	}
//   89 	if (onTimeSt <= 0) {
??runBench_0:
        CMP      R6,#+1
        BGE.N    ??runBench_2
//   90 		scheduleMsg(&logger, "Invalid onTime %f", onTimeMs);
        VMOV     R0,S17
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable21_11
        LDR.W    R0,??DataTable21_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   91 		return;
        B.N      ??runBench_1
//   92 	}
//   93 	if (offTimeSt <= 0) {
??runBench_2:
        CMP      R7,#+1
        BGE.N    ??runBench_3
//   94 		scheduleMsg(&logger, "Invalid offTime %f", offTimeMs);
        VMOV     R0,S18
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable21_12
        LDR.W    R0,??DataTable21_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   95 		return;
        B.N      ??runBench_1
//   96 	}
//   97 	scheduleMsg(&logger, "Running bench: ON_TIME=%f ms OFF_TIME=%fms Counter=%d", onTimeMs, offTimeMs, count);
??runBench_3:
        STR      R8,[SP, #+8]
        VMOV     R0,S18
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        VMOV     R0,S17
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable21_13
        LDR.W    R0,??DataTable21_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   98 	scheduleMsg(&logger, "output on %s", hwPortname(brainPin));
        MOV      R0,R9
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R2,R0
        LDR.W    R1,??DataTable21_14
        LDR.W    R0,??DataTable21_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   99 
//  100 	if (delaySt != 0) {
        CMP      R5,#+0
        BEQ.N    ??runBench_4
//  101 		chThdSleep(delaySt);
        MOVS     R0,R5
          CFI FunCall chThdSleep
        BL       chThdSleep
//  102 	}
//  103 
//  104 	isRunningBench = true;
??runBench_4:
        LDR.W    R0,??DataTable21_2
        MOVS     R1,#+1
        STRB     R1,[R0, #+0]
//  105 	for (int i = 0; i < count; i++) {
        MOVS     R10,#+0
??runBench_5:
        CMP      R10,R8
        BGE.N    ??runBench_6
//  106 		output->setValue(true);
        MOVS     R1,#+1
        MOVS     R0,R4
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
//  107 		chThdSleep(onTimeSt);
        MOVS     R0,R6
          CFI FunCall chThdSleep
        BL       chThdSleep
//  108 		output->setValue(false);
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
//  109 		chThdSleep(offTimeSt);
        MOVS     R0,R7
          CFI FunCall chThdSleep
        BL       chThdSleep
//  110 	}
        ADDS     R10,R10,#+1
        B.N      ??runBench_5
//  111 	scheduleMsg(&logger, "Done!");
??runBench_6:
        LDR.W    R1,??DataTable21_15
        LDR.W    R0,??DataTable21_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  112 	isRunningBench = false;
        LDR.W    R0,??DataTable21_2
        MOVS     R1,#+0
        STRB     R1,[R0, #+0]
//  113 }
??runBench_1:
        ADD      SP,SP,#+16
          CFI CFA R13+48
        VPOP     {D8-D9}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI CFA R13+32
        POP      {R4-R10,PC}      ;; return
          CFI EndBlock cfiBlock6
//  114 

        SECTION `.bss`:DATA:REORDER:NOROOT(0)
//  115 static volatile bool_t needToRunBench = false;
needToRunBench:
        DS8 1

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  116 static float onTime;
onTime:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  117 static float offTime;
offTime:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  118 static float delayMs;
delayMs:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  119 static int count;
count:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  120 static brain_pin_e brainPin;
brainPin:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  121 static OutputPin* pinX;
pinX:
        DS8 4
//  122 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z8pinbenchPKcS0_S0_S0_P9OutputPin11brain_pin_e
        THUMB
//  123 static void pinbench(const char *delayStr, const char *onTimeStr, const char *offTimeStr, const char *countStr,
//  124 		OutputPin* pinParam, brain_pin_e brainPinParam) {
_Z8pinbenchPKcS0_S0_S0_P9OutputPin11brain_pin_e:
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
        LDR      R8,[SP, #+32]
        LDR      R9,[SP, #+36]
//  125 	delayMs = atoff(delayStr);
        MOVS     R0,R4
          CFI FunCall atoff
        BL       atoff
        LDR.W    R0,??DataTable21_16
        VSTR     S0,[R0, #0]
//  126 	onTime = atoff(onTimeStr);
        MOVS     R0,R5
          CFI FunCall atoff
        BL       atoff
        LDR.W    R0,??DataTable21_17
        VSTR     S0,[R0, #0]
//  127 	offTime = atoff(offTimeStr);
        MOVS     R0,R6
          CFI FunCall atoff
        BL       atoff
        LDR.W    R0,??DataTable21_18
        VSTR     S0,[R0, #0]
//  128 	count = atoi(countStr);
        MOVS     R0,R7
          CFI FunCall atoi
        BL       atoi
        LDR.W    R1,??DataTable21_19
        STR      R0,[R1, #+0]
//  129 
//  130 	brainPin = brainPinParam;
        LDR.W    R0,??DataTable21_20
        STR      R9,[R0, #+0]
//  131 	pinX = pinParam;
        LDR.W    R0,??DataTable21_21
        STR      R8,[R0, #+0]
//  132 	needToRunBench = true;
        LDR.W    R0,??DataTable21_22
        MOVS     R1,#+1
        STRB     R1,[R0, #+0]
//  133 }
        POP      {R0,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock7
//  134 
//  135 /**
//  136  * delay 100, cylinder #2, 5ms ON, 1000ms OFF, repeat 2 times
//  137  * fuelbench2 100 2 5 1000 2
//  138  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z10fuelbench2PKcS0_S0_S0_S0_
        THUMB
//  139 static void fuelbench2(const char *delayStr, const char *indexStr, const char * onTimeStr, const char *offTimeStr,
//  140 		const char *countStr) {
_Z10fuelbench2PKcS0_S0_S0_S0_:
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
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
        LDR      R8,[SP, #+40]
//  141 	int index = atoi(indexStr);
        MOVS     R0,R5
          CFI FunCall atoi
        BL       atoi
        MOV      R9,R0
//  142 	if (index < 1 || index > engineConfiguration->specs.cylindersCount) {
        CMP      R9,#+1
        BLT.N    ??fuelbench2_0
        LDR.W    R0,??DataTable21_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+404]
        CMP      R0,R9
        BGE.N    ??fuelbench2_1
//  143 		scheduleMsg(&logger, "Invalid index: %d", index);
??fuelbench2_0:
        MOV      R2,R9
        LDR.W    R1,??DataTable21_23
        LDR.W    R0,??DataTable21_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  144 		return;
        B.N      ??fuelbench2_2
//  145 	}
//  146 	brain_pin_e b = boardConfiguration->injectionPins[index - 1];
??fuelbench2_1:
        LDR.W    R0,??DataTable21_24
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R9, LSL #+2
        LDR      R10,[R0, #+28]
//  147 	pinbench(delayStr, onTimeStr, offTimeStr, countStr, &enginePins.injectors[index - 1], b);
        STR      R10,[SP, #+4]
        MOVS     R0,#+20
        LDR.W    R1,??DataTable21_25
        MLA      R0,R0,R9,R1
        ADDS     R0,R0,#+76
        STR      R0,[SP, #+0]
        MOV      R3,R8
        MOVS     R2,R7
        MOVS     R1,R6
        MOVS     R0,R4
          CFI FunCall _Z8pinbenchPKcS0_S0_S0_P9OutputPin11brain_pin_e
        BL       _Z8pinbenchPKcS0_S0_S0_P9OutputPin11brain_pin_e
//  148 }
??fuelbench2_2:
        POP      {R0,R1,R4-R10,PC}  ;; return
          CFI EndBlock cfiBlock8
//  149 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z8fanBenchv
        THUMB
//  150 void fanBench(void) {
_Z8fanBenchv:
        PUSH     {LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+4
        SUB      SP,SP,#+12
          CFI CFA R13+16
//  151 	pinbench("0", "3000", "100", "1", &enginePins.fanRelay, boardConfiguration->fanPin);
        LDR.W    R0,??DataTable21_24
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+144]
        STR      R0,[SP, #+4]
        LDR.W    R0,??DataTable21_26
        STR      R0,[SP, #+0]
        ADR.N    R3,??DataTable18  ;; "1"
        ADR.N    R2,??DataTable18_1  ;; "100"
        LDR.W    R1,??DataTable21_27
        ADR.N    R0,??DataTable20  ;; "0"
          CFI FunCall _Z8pinbenchPKcS0_S0_S0_P9OutputPin11brain_pin_e
        BL       _Z8pinbenchPKcS0_S0_S0_P9OutputPin11brain_pin_e
//  152 }
        POP      {R0-R2,PC}       ;; return
          CFI EndBlock cfiBlock9
//  153 
//  154 extern OutputPin checkEnginePin;
//  155 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z8milBenchv
        THUMB
//  156 void milBench(void) {
_Z8milBenchv:
        PUSH     {LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+4
        SUB      SP,SP,#+12
          CFI CFA R13+16
//  157 	pinbench("0", "3000", "100", "1", &checkEnginePin, boardConfiguration->malfunctionIndicatorPin);
        LDR.W    R0,??DataTable21_24
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+136]
        STR      R0,[SP, #+4]
        LDR.W    R0,??DataTable21_28
        STR      R0,[SP, #+0]
        ADR.N    R3,??DataTable18  ;; "1"
        ADR.N    R2,??DataTable18_1  ;; "100"
        LDR.W    R1,??DataTable21_27
        ADR.N    R0,??DataTable20  ;; "0"
          CFI FunCall _Z8pinbenchPKcS0_S0_S0_P9OutputPin11brain_pin_e
        BL       _Z8pinbenchPKcS0_S0_S0_P9OutputPin11brain_pin_e
//  158 }
        POP      {R0-R2,PC}       ;; return
          CFI EndBlock cfiBlock10

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10:
        DC32     0x447a0000
//  159 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z13fuelPumpBenchv
        THUMB
//  160 void fuelPumpBench(void) {
_Z13fuelPumpBenchv:
        PUSH     {LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+4
        SUB      SP,SP,#+12
          CFI CFA R13+16
//  161 	pinbench("0", "3000", "100", "1", &enginePins.fuelPumpRelay, boardConfiguration->fuelPumpPin);
        LDR.N    R0,??DataTable21_24
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+24]
        STR      R0,[SP, #+4]
        LDR.N    R0,??DataTable21_29
        STR      R0,[SP, #+0]
        ADR.N    R3,??DataTable18  ;; "1"
        ADR.N    R2,??DataTable18_1  ;; "100"
        LDR.N    R1,??DataTable21_27
        ADR.N    R0,??DataTable20  ;; "0"
          CFI FunCall _Z8pinbenchPKcS0_S0_S0_P9OutputPin11brain_pin_e
        BL       _Z8pinbenchPKcS0_S0_S0_P9OutputPin11brain_pin_e
//  162 }
        POP      {R0-R2,PC}       ;; return
          CFI EndBlock cfiBlock11
//  163 
//  164 // fuelbench 5 1000 2

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _Z9fuelbenchPKcS0_S0_
        THUMB
//  165 static void fuelbench(const char * onTimeStr, const char *offTimeStr, const char *countStr) {
_Z9fuelbenchPKcS0_S0_:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+8
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  166 	fuelbench2("0", "1", onTimeStr, offTimeStr, countStr);
        STR      R6,[SP, #+0]
        MOVS     R3,R5
        MOVS     R2,R4
        ADR.N    R1,??DataTable18  ;; "1"
        ADR.N    R0,??DataTable20  ;; "0"
          CFI FunCall _Z10fuelbench2PKcS0_S0_S0_S0_
        BL       _Z10fuelbench2PKcS0_S0_S0_S0_
//  167 }
        POP      {R0,R1,R4-R6,PC}  ;; return
          CFI EndBlock cfiBlock12
//  168 
//  169 /**
//  170  * sparkbench2 0 1 5 1000 2
//  171  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z11sparkbench2PKcS0_S0_S0_S0_
        THUMB
//  172 static void sparkbench2(const char *delayStr, const char *indexStr, const char * onTimeStr, const char *offTimeStr,
//  173 		const char *countStr) {
_Z11sparkbench2PKcS0_S0_S0_S0_:
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
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
        LDR      R8,[SP, #+40]
//  174 	int index = atoi(indexStr);
        MOVS     R0,R5
          CFI FunCall atoi
        BL       atoi
        MOV      R9,R0
//  175 	if (index < 1 || index > engineConfiguration->specs.cylindersCount) {
        CMP      R9,#+1
        BLT.N    ??sparkbench2_0
        LDR.N    R0,??DataTable21_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+404]
        CMP      R0,R9
        BGE.N    ??sparkbench2_1
//  176 		scheduleMsg(&logger, "Invalid index: %d", index);
??sparkbench2_0:
        MOV      R2,R9
        LDR.N    R1,??DataTable21_23
        LDR.N    R0,??DataTable21_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  177 		return;
        B.N      ??sparkbench2_2
//  178 	}
//  179 	brain_pin_e b = boardConfiguration->ignitionPins[index - 1];
??sparkbench2_1:
        LDR.N    R0,??DataTable21_24
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R9, LSL #+2
        LDR      R10,[R0, #+80]
//  180 	pinbench(delayStr, onTimeStr, offTimeStr, countStr, &enginePins.coils[index - 1], b);
        STR      R10,[SP, #+4]
        MOVS     R0,#+20
        LDR.N    R1,??DataTable21_25
        MLA      R0,R0,R9,R1
        ADDS     R0,R0,#+316
        STR      R0,[SP, #+0]
        MOV      R3,R8
        MOVS     R2,R7
        MOVS     R1,R6
        MOVS     R0,R4
          CFI FunCall _Z8pinbenchPKcS0_S0_S0_P9OutputPin11brain_pin_e
        BL       _Z8pinbenchPKcS0_S0_S0_P9OutputPin11brain_pin_e
//  181 }
??sparkbench2_2:
        POP      {R0,R1,R4-R10,PC}  ;; return
          CFI EndBlock cfiBlock13
//  182 
//  183 /**
//  184  * sparkbench 5 400 2
//  185  * 5 ms ON, 400 ms OFF, two times
//  186  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _Z10sparkbenchPKcS0_S0_
        THUMB
//  187 static void sparkbench(const char * onTimeStr, const char *offTimeStr, const char *countStr) {
_Z10sparkbenchPKcS0_S0_:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+8
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  188 	sparkbench2("0", "1", onTimeStr, offTimeStr, countStr);
        STR      R6,[SP, #+0]
        MOVS     R3,R5
        MOVS     R2,R4
        ADR.N    R1,??DataTable18  ;; "1"
        ADR.N    R0,??DataTable20  ;; "0"
          CFI FunCall _Z11sparkbench2PKcS0_S0_S0_S0_
        BL       _Z11sparkbench2PKcS0_S0_S0_S0_
//  189 }
        POP      {R0,R1,R4-R6,PC}  ;; return
          CFI EndBlock cfiBlock14
//  190 

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//  191 static THD_WORKING_AREA(benchThreadStack, UTILITY_THREAD_STACK_SIZE);
benchThreadStack:
        DS8 712
//  192 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function _Z11benchThreadi
        THUMB
//  193 static msg_t benchThread(int param) {
_Z11benchThreadi:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  194 	(void) param;
//  195 	chRegSetThreadName("BenchThread");
        LDR.N    R0,??DataTable21_30
        LDR      R0,[R0, #+24]
        LDR.N    R1,??DataTable21_31
        STR      R1,[R0, #+24]
//  196 
//  197 	while (true) {
//  198 		while (!needToRunBench) {
??benchThread_0:
??benchThread_1:
        LDR.N    R0,??DataTable21_22
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??benchThread_2
//  199 			chThdSleepMilliseconds(200);
        MOVS     R0,#+200
          CFI FunCall chThdSleep
        BL       chThdSleep
        B.N      ??benchThread_1
//  200 		}
//  201 		needToRunBench = false;
??benchThread_2:
        LDR.N    R0,??DataTable21_22
        MOVS     R1,#+0
        STRB     R1,[R0, #+0]
//  202 		runBench(brainPin, pinX, delayMs, onTime, offTime, count);
        LDR.N    R0,??DataTable21_19
        LDR      R2,[R0, #+0]
        LDR.N    R0,??DataTable21_18
        VLDR     S2,[R0, #0]
        LDR.N    R0,??DataTable21_17
        VLDR     S1,[R0, #0]
        LDR.N    R0,??DataTable21_16
        VLDR     S0,[R0, #0]
        LDR.N    R0,??DataTable21_21
        LDR      R1,[R0, #+0]
        LDR.N    R0,??DataTable21_20
        LDR      R0,[R0, #+0]
          CFI FunCall _Z8runBench11brain_pin_eP9OutputPinfffi
        BL       _Z8runBench11brain_pin_eP9OutputPinfffi
        B.N      ??benchThread_0
          CFI EndBlock cfiBlock15
//  203 	}
//  204 #if defined __GNUC__
//  205 	return 0;
//  206 #endif
//  207 }
//  208 
//  209 extern engine_configuration_s activeConfiguration;
//  210 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function _Z10unregister11brain_pin_eP9OutputPin
        THUMB
//  211 static void unregister(brain_pin_e currentPin, OutputPin *output) {
_Z10unregister11brain_pin_eP9OutputPin:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  212 	if (currentPin == GPIO_UNASSIGNED)
        CMP      R4,#+80
        BEQ.N    ??unregister_0
//  213 		return;
//  214 	scheduleMsg(&logger, "unregistering %s", hwPortname(currentPin));
??unregister_1:
        MOVS     R0,R4
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R2,R0
        LDR.N    R1,??DataTable21_32
        LDR.N    R0,??DataTable21_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  215 	unmarkPin(currentPin);
        MOVS     R0,R4
          CFI FunCall _Z9unmarkPin11brain_pin_e
        BL       _Z9unmarkPin11brain_pin_e
//  216 	output->unregister();
        MOVS     R0,R5
          CFI FunCall _ZN9OutputPin10unregisterEv
        BL       _ZN9OutputPin10unregisterEv
//  217 }
??unregister_0:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock16
//  218 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function _Z16stopIgnitionPinsv
        THUMB
//  219 void stopIgnitionPins(void) {
_Z16stopIgnitionPinsv:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
//  220 	for (int i = 0; i < IGNITION_PIN_COUNT; i++) {
        MOVS     R4,#+0
??stopIgnitionPins_0:
        CMP      R4,#+12
        BGE.N    ??stopIgnitionPins_1
//  221 		NamedOutputPin *output = &enginePins.injectors[i];
        MOVS     R0,#+20
        LDR.N    R1,??DataTable21_25
        MLA      R0,R0,R4,R1
        ADDS     R5,R0,#+96
//  222 		brain_pin_e currentPin = activeConfiguration.bc.ignitionPins[i];
        LDR.N    R0,??DataTable21_33
        ADDS     R0,R0,R4, LSL #+2
        LDR      R6,[R0, #+708]
//  223 		if (engineConfiguration->bc.ignitionPins[i] != currentPin) {
        LDR.N    R0,??DataTable21_3
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R4, LSL #+2
        LDR      R0,[R0, #+708]
        CMP      R0,R6
        BEQ.N    ??stopIgnitionPins_2
//  224 			unregister(currentPin, output);
        MOVS     R1,R5
        MOVS     R0,R6
          CFI FunCall _Z10unregister11brain_pin_eP9OutputPin
        BL       _Z10unregister11brain_pin_eP9OutputPin
//  225 		}
//  226 	}
??stopIgnitionPins_2:
        ADDS     R4,R4,#+1
        B.N      ??stopIgnitionPins_0
//  227 }
??stopIgnitionPins_1:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock17
//  228 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function _Z17stopInjectionPinsv
        THUMB
//  229 void stopInjectionPins(void) {
_Z17stopInjectionPinsv:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
//  230 	for (int i = 0; i < INJECTION_PIN_COUNT; i++) {
        MOVS     R4,#+0
??stopInjectionPins_0:
        CMP      R4,#+12
        BGE.N    ??stopInjectionPins_1
//  231 		NamedOutputPin *output = &enginePins.injectors[i];
        MOVS     R0,#+20
        LDR.N    R1,??DataTable21_25
        MLA      R0,R0,R4,R1
        ADDS     R5,R0,#+96
//  232 		brain_pin_e currentPin = activeConfiguration.bc.injectionPins[i];
        LDR.N    R0,??DataTable21_33
        ADDS     R0,R0,R4, LSL #+2
        LDR      R6,[R0, #+656]
//  233 		if (engineConfiguration->bc.injectionPins[i] != currentPin) {
        LDR.N    R0,??DataTable21_3
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R4, LSL #+2
        LDR      R0,[R0, #+656]
        CMP      R0,R6
        BEQ.N    ??stopInjectionPins_2
//  234 			unregister(currentPin, output);
        MOVS     R1,R5
        MOVS     R0,R6
          CFI FunCall _Z10unregister11brain_pin_eP9OutputPin
        BL       _Z10unregister11brain_pin_eP9OutputPin
//  235 		}
//  236 	}
??stopInjectionPins_2:
        ADDS     R4,R4,#+1
        B.N      ??stopInjectionPins_0
//  237 }
??stopInjectionPins_1:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock18

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18:
        DC8      "1",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_1:
        DC8      "100"
//  238 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock19 Using cfiCommon0
          CFI Function _Z17startIgnitionPinsv
        THUMB
//  239 void startIgnitionPins(void) {
_Z17startIgnitionPinsv:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
//  240 	for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
        MOVS     R4,#+0
??startIgnitionPins_0:
        LDR.N    R0,??DataTable21_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+404]
        CMP      R4,R0
        BGE.N    ??startIgnitionPins_1
//  241 		NamedOutputPin *output = &enginePins.coils[i];
        MOVS     R0,#+20
        LDR.N    R1,??DataTable21_25
        MLA      R0,R0,R4,R1
        ADDS     R5,R0,#+336
//  242 		if (boardConfiguration->ignitionPins[i] != activeConfiguration.bc.ignitionPins[i]) {
        LDR.N    R0,??DataTable21_24
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R4, LSL #+2
        LDR      R0,[R0, #+84]
        LDR.N    R1,??DataTable21_33
        ADDS     R1,R1,R4, LSL #+2
        LDR      R1,[R1, #+708]
        CMP      R0,R1
        BEQ.N    ??startIgnitionPins_2
//  243 			outputPinRegisterExt2(output->name, output, boardConfiguration->ignitionPins[i],
//  244 				&boardConfiguration->ignitionPinMode);
        LDR.N    R0,??DataTable21_24
        LDR      R0,[R0, #+0]
        ADDS     R3,R0,#+132
        LDR.N    R0,??DataTable21_24
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R4, LSL #+2
        LDR      R2,[R0, #+84]
        MOVS     R1,R5
        LDR      R0,[R5, #+16]
          CFI FunCall _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
        BL       _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
//  245 		}
//  246 	}
??startIgnitionPins_2:
        ADDS     R4,R4,#+1
        B.N      ??startIgnitionPins_0
//  247 }
??startIgnitionPins_1:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock19
//  248 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock20 Using cfiCommon0
          CFI Function _Z18startInjectionPinsv
        THUMB
//  249 void startInjectionPins(void) {
_Z18startInjectionPinsv:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
//  250 	// todo: should we move this code closer to the injection logic?
//  251 	for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
        MOVS     R4,#+0
??startInjectionPins_0:
        LDR.N    R0,??DataTable21_3
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+404]
        CMP      R4,R0
        BGE.N    ??startInjectionPins_1
//  252 		NamedOutputPin *output = &enginePins.injectors[i];
        MOVS     R0,#+20
        LDR.N    R1,??DataTable21_25
        MLA      R0,R0,R4,R1
        ADDS     R5,R0,#+96
//  253 		if (engineConfiguration->bc.injectionPins[i] != activeConfiguration.bc.injectionPins[i]) {
        LDR.N    R0,??DataTable21_3
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R4, LSL #+2
        LDR      R0,[R0, #+656]
        LDR.N    R1,??DataTable21_33
        ADDS     R1,R1,R4, LSL #+2
        LDR      R1,[R1, #+656]
        CMP      R0,R1
        BEQ.N    ??startInjectionPins_2
//  254 
//  255 			outputPinRegisterExt2(output->name, output, boardConfiguration->injectionPins[i],
//  256 					&boardConfiguration->injectionPinMode);
        LDR.N    R0,??DataTable21_24
        LDR      R0,[R0, #+0]
        ADDS     R3,R0,#+80
        LDR.N    R0,??DataTable21_24
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R4, LSL #+2
        LDR      R2,[R0, #+32]
        MOVS     R1,R5
        LDR      R0,[R5, #+16]
          CFI FunCall _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
        BL       _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
//  257 		}
//  258 	}
??startInjectionPins_2:
        ADDS     R4,R4,#+1
        B.N      ??startInjectionPins_0
//  259 }
??startInjectionPins_1:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock20

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable20:
        DC8      "0",0x0,0x0
//  260 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock21 Using cfiCommon0
          CFI Function _Z19initInjectorCentralv
        THUMB
//  261 void initInjectorCentral(void) {
_Z19initInjectorCentralv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  262 	chThdCreateStatic(benchThreadStack, sizeof(benchThreadStack), NORMALPRIO, (tfunc_t) benchThread, NULL);
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        LDR.N    R3,??DataTable21_34
        MOVS     R2,#+64
        MOV      R1,#+712
        LDR.N    R0,??DataTable21_35
          CFI FunCall chThdCreateStatic
        BL       chThdCreateStatic
//  263 
//  264 	for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
        MOVS     R0,#+0
??initInjectorCentral_0:
        LDR.N    R1,??DataTable21_3
        LDR      R1,[R1, #+0]
        LDR      R1,[R1, #+404]
        CMP      R0,R1
        BGE.N    ??initInjectorCentral_1
//  265 		is_injector_enabled[i] = true;
        LDR.N    R1,??DataTable21_7
        MOVS     R2,#+1
        STR      R2,[R1, R0, LSL #+2]
//  266 	}
        ADDS     R0,R0,#+1
        B.N      ??initInjectorCentral_0
//  267 
//  268 	startInjectionPins();
??initInjectorCentral_1:
          CFI FunCall _Z18startInjectionPinsv
        BL       _Z18startInjectionPinsv
//  269 	startIgnitionPins();
          CFI FunCall _Z17startIgnitionPinsv
        BL       _Z17startIgnitionPinsv
//  270 
//  271 	printStatus();
          CFI FunCall _Z11printStatusv
        BL       _Z11printStatusv
//  272 	addConsoleActionII("injector", setInjectorEnabled);
        LDR.N    R1,??DataTable21_36
        LDR.N    R0,??DataTable21_37
          CFI FunCall addConsoleActionII
        BL       addConsoleActionII
//  273 
//  274 	addConsoleAction("fuelpumpbench", fuelPumpBench);
        LDR.N    R1,??DataTable21_38
        LDR.N    R0,??DataTable21_39
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  275 	addConsoleAction("fanbench", fanBench);
        LDR.N    R1,??DataTable21_40
        LDR.N    R0,??DataTable21_41
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  276 
//  277 	addConsoleAction("milbench", milBench);
        LDR.N    R1,??DataTable21_42
        LDR.N    R0,??DataTable21_43
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  278 	addConsoleActionSSS("fuelbench", fuelbench);
        LDR.N    R1,??DataTable21_44
        LDR.N    R0,??DataTable21_45
          CFI FunCall addConsoleActionSSS
        BL       addConsoleActionSSS
//  279 	addConsoleActionSSS("sparkbench", sparkbench);
        LDR.N    R1,??DataTable21_46
        LDR.N    R0,??DataTable21_47
          CFI FunCall addConsoleActionSSS
        BL       addConsoleActionSSS
//  280 
//  281 	addConsoleActionSSSSS("fuelbench2", fuelbench2);
        LDR.N    R1,??DataTable21_48
        LDR.N    R0,??DataTable21_49
          CFI FunCall addConsoleActionSSSSS
        BL       addConsoleActionSSSSS
//  282 	addConsoleActionSSSSS("sparkbench2", sparkbench2);
        LDR.N    R1,??DataTable21_50
        LDR.N    R0,??DataTable21_51
          CFI FunCall addConsoleActionSSSSS
        BL       addConsoleActionSSSSS
//  283 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock21

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21:
        DC32     `?<Constant "InjectorCentral">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_1:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_2:
        DC32     isRunningBench

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_3:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_4:
        DC32     `?<Constant "ERROR [%s] cid=%d\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_5:
        DC32     `?<Constant "Cylinder ID">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_6:
        DC32     `?<Constant "isInjectorEnabled">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_7:
        DC32     is_injector_enabled

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_8:
        DC32     `?<Constant "injector_%d_%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_9:
        DC32     `?<Constant "injector id">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_10:
        DC32     `?<Constant "Invalid delay %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_11:
        DC32     `?<Constant "Invalid onTime %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_12:
        DC32     `?<Constant "Invalid offTime %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_13:
        DC32     `?<Constant "Running bench: ON_TIM...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_14:
        DC32     `?<Constant "output on %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_15:
        DC32     `?<Constant "Done!">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_16:
        DC32     delayMs

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_17:
        DC32     onTime

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_18:
        DC32     offTime

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_19:
        DC32     count

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_20:
        DC32     brainPin

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_21:
        DC32     pinX

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_22:
        DC32     needToRunBench

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_23:
        DC32     `?<Constant "Invalid index: %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_24:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_25:
        DC32     enginePins

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_26:
        DC32     enginePins+0x10

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_27:
        DC32     `?<Constant "3000">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_28:
        DC32     checkEnginePin

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_29:
        DC32     enginePins+0x30

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_30:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_31:
        DC32     `?<Constant "BenchThread">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_32:
        DC32     `?<Constant "unregistering %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_33:
        DC32     activeConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_34:
        DC32     _Z11benchThreadi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_35:
        DC32     benchThreadStack

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_36:
        DC32     _Z18setInjectorEnabledii

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_37:
        DC32     `?<Constant "injector">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_38:
        DC32     _Z13fuelPumpBenchv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_39:
        DC32     `?<Constant "fuelpumpbench">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_40:
        DC32     _Z8fanBenchv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_41:
        DC32     `?<Constant "fanbench">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_42:
        DC32     _Z8milBenchv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_43:
        DC32     `?<Constant "milbench">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_44:
        DC32     _Z9fuelbenchPKcS0_S0_

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_45:
        DC32     `?<Constant "fuelbench">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_46:
        DC32     _Z10sparkbenchPKcS0_S0_

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_47:
        DC32     `?<Constant "sparkbench">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_48:
        DC32     _Z10fuelbench2PKcS0_S0_S0_S0_

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_49:
        DC32     `?<Constant "fuelbench2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_50:
        DC32     _Z11sparkbench2PKcS0_S0_S0_S0_

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_51:
        DC32     `?<Constant "sparkbench2">`

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
`?<Constant "ERROR [%s] cid=%d\\r\\n">`:
        DATA
        DC8 "ERROR [%s] cid=%d\015\012"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Cylinder ID">`:
        DATA
        DC8 "Cylinder ID"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "isInjectorEnabled">`:
        DATA
        DC8 "isInjectorEnabled"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "injector_%d_%d">`:
        DATA
        DC8 "injector_%d_%d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "injector id">`:
        DATA
        DC8 "injector id"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Invalid delay %f">`:
        DATA
        DC8 "Invalid delay %f"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Invalid onTime %f">`:
        DATA
        DC8 "Invalid onTime %f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Invalid offTime %f">`:
        DATA
        DC8 "Invalid offTime %f"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Running bench: ON_TIM...">`:
        DATA
        DC8 52H, 75H, 6EH, 6EH, 69H, 6EH, 67H, 20H
        DC8 62H, 65H, 6EH, 63H, 68H, 3AH, 20H, 4FH
        DC8 4EH, 5FH, 54H, 49H, 4DH, 45H, 3DH, 25H
        DC8 66H, 20H, 6DH, 73H, 20H, 4FH, 46H, 46H
        DC8 5FH, 54H, 49H, 4DH, 45H, 3DH, 25H, 66H
        DC8 6DH, 73H, 20H, 43H, 6FH, 75H, 6EH, 74H
        DC8 65H, 72H, 3DH, 25H, 64H, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "output on %s">`:
        DATA
        DC8 "output on %s"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Done!">`:
        DATA
        DC8 "Done!"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Invalid index: %d">`:
        DATA
        DC8 "Invalid index: %d"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 "0"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "3000">`:
        DATA
        DC8 "3000"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "100"

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 "1"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "BenchThread">`:
        DATA
        DC8 "BenchThread"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "unregistering %s">`:
        DATA
        DC8 "unregistering %s"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "injector">`:
        DATA
        DC8 "injector"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fuelpumpbench">`:
        DATA
        DC8 "fuelpumpbench"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fanbench">`:
        DATA
        DC8 "fanbench"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "milbench">`:
        DATA
        DC8 "milbench"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fuelbench">`:
        DATA
        DC8 "fuelbench"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "sparkbench">`:
        DATA
        DC8 "sparkbench"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fuelbench2">`:
        DATA
        DC8 "fuelbench2"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "sparkbench2">`:
        DATA
        DC8 "sparkbench2"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "InjectorCentral">`:
        DATA
        DC8 "InjectorCentral"

        END
//  284 
//  285 #endif
// 
// 1 046 bytes in section .bss
//     4 bytes in section .init_array
//   404 bytes in section .rodata
// 1 760 bytes in section .text
// 
// 1 764 bytes of CODE  memory
//   404 bytes of CONST memory
// 1 046 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
