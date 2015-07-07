///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:25:45 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\ele /
//                       ctronic_throttle.cpp                                 /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\ele /
//                       ctronic_throttle.cpp -lCN                            /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                        -lA F:\stuff\rusefi_sourceforge\firmware\iar\Debug\ /
//                       List\ -o F:\stuff\rusefi_sourceforge\firmware\iar\De /
//                       bug\Obj\ --no_cse --no_unroll --no_inline            /
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
//                       electronic_throttle.s                                /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME electronic_throttle

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z10hwPortname11brain_pin_e
        EXTERN _Z13applyPinStateP9PwmConfigi
        EXTERN _Z16getPedalPositionv
        EXTERN _Z17startSimplePwmExtP9PwmConfigPKc11brain_pin_eP9OutputPinffPFvS0_iE
        EXTERN _Z22getPinNameByAdcChannel13adc_channel_ePc
        EXTERN _Z6getTPSv
        EXTERN _ZN18LoggingWithStorageC1EPKc
        EXTERN _ZN3Pid13updateFactorsEfff
        EXTERN _ZN3Pid8getValueEfff
        EXTERN _ZN3PidC1Efffff
        EXTERN _ZN9OutputPinC1Ev
        EXTERN _ZN9SimplePwm21setSimplePwmDutyCycleEf
        EXTERN _ZN9SimplePwmC1Ev
        EXTERN __aeabi_d2f
        EXTERN __aeabi_dadd
        EXTERN __aeabi_ddiv
        EXTERN __aeabi_f2d
        EXTERN __aeabi_i2d
        EXTERN addConsoleAction
        EXTERN addConsoleActionF
        EXTERN addConsoleActionI
        EXTERN boardConfiguration
        EXTERN chThdCreateStatic
        EXTERN chThdSleep
        EXTERN engineConfiguration
        EXTERN minI
        EXTERN print
        EXTERN scheduleMsg

        PUBLIC _Z14applyPinState2P9PwmConfigi
        PUBLIC _Z22initElectronicThrottlev
        PUBLIC _Z23setDefaultEtbParametersv
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\electronic_throttle.cpp
//    1 /**
//    2  * @file	electronic_throttle.cpp
//    3  * @brief	Electronic Throttle Module driver L298N
//    4  *
//    5  * todo: make this more universal if/when we get other hardware options
//    6  *
//    7  * @date Dec 7, 2013
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
//   25 #include "electronic_throttle.h"
//   26 #include "pin_repository.h"
//   27 #include "idle_controller.h"
//   28 #include "tps.h"
//   29 #include "io_pins.h"
//   30 #include "engine_configuration.h"
//   31 #include "pwm_generator.h"
//   32 #include "pwm_generator_logic.h"
//   33 #include "pid.h"
//   34 #include "engine_controller.h"
//   35 
//   36 #if EFI_ELECTRONIC_THROTTLE_BODY || defined(__DOXYGEN__)
//   37 
//   38 #define ETB_FREQ 400
//   39 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   40 static LoggingWithStorage logger("ETB");
        ADR.N    R1,??DataTable6  ;; "ETB"
        LDR.N    R0,??DataTable8_3
          CFI FunCall _ZN18LoggingWithStorageC1EPKc
        BL       _ZN18LoggingWithStorageC1EPKc
//   41 /**
//   42  * @brief Control Thread stack
//   43  */
//   44 static THD_WORKING_AREA(etbTreadStack, UTILITY_THREAD_STACK_SIZE);
//   45 /**
//   46  * @brief Pulse-Width Modulation state
//   47  */
//   48 static SimplePwm etbPwmUp;
        LDR.N    R0,??DataTable8_4
          CFI FunCall _ZN9SimplePwmC1Ev
        BL       _ZN9SimplePwmC1Ev
//   49 static OutputPin output1;
        LDR.N    R0,??DataTable8_5
          CFI FunCall _ZN9OutputPinC1Ev
        BL       _ZN9OutputPinC1Ev
//   50 static SimplePwm etbPwmDown;
        LDR.N    R0,??DataTable8_6
          CFI FunCall _ZN9SimplePwmC1Ev
        BL       _ZN9SimplePwmC1Ev
//   51 static OutputPin output2;
        LDR.N    R0,??DataTable8_7
          CFI FunCall _ZN9OutputPinC1Ev
        BL       _ZN9OutputPinC1Ev
//   52 
//   53 static Pid pid(10, 0, 0, 1, 90);
        VLDR.W   S4,??DataTable7  ;; 0x42b40000
        VMOV.F32 S3,#1.0
        VLDR.W   S2,??DataTable7_1  ;; 0x0
        VLDR.W   S1,??DataTable7_1  ;; 0x0
        VMOV.F32 S0,#10.0
        LDR.N    R0,??DataTable8_8
          CFI FunCall _ZN3PidC1Efffff
        BL       _ZN3PidC1Efffff
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
logger:
        DS8 260

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
etbTreadStack:
        DS8 712

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
etbPwmUp:
        DS8 112

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
output1:
        DS8 16

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
etbPwmDown:
        DS8 112

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
output2:
        DS8 16

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
pid:
        DS8 28
//   54 
//   55 static float prevTps;
//   56 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   57 static float currentEtbDuty;
currentEtbDuty:
        DS8 4
//   58 
//   59 EXTERN_ENGINE;
//   60 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z9etbThreadPv
        THUMB
//   61 static msg_t etbThread(void *arg) {
_Z9etbThreadPv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//   62         UNUSED(arg);
//   63 	while (true) {
//   64 		float pedal = getPedalPosition(PASS_ENGINE_PARAMETER_F);
??etbThread_0:
          CFI FunCall _Z16getPedalPositionv
        BL       _Z16getPedalPositionv
        VMOV.F32 S16,S0
//   65 		float tps = getTPS();
          CFI FunCall _Z6getTPSv
        BL       _Z6getTPSv
        VMOV.F32 S17,S0
//   66 
//   67 		currentEtbDuty = pid.getValue(pedal, getTPS(), 1);
          CFI FunCall _Z6getTPSv
        BL       _Z6getTPSv
        VMOV.F32 S1,S0
        VMOV.F32 S2,#1.0
        VMOV.F32 S0,S16
        LDR.N    R0,??DataTable8_8
          CFI FunCall _ZN3Pid8getValueEfff
        BL       _ZN3Pid8getValueEfff
        LDR.N    R0,??DataTable8_9
        VSTR     S0,[R0, #0]
//   68 
//   69 		etbPwmUp.setSimplePwmDutyCycle(currentEtbDuty / 100);
        LDR.N    R0,??DataTable8_9
        VLDR     S0,[R0, #0]
        VLDR.W   S1,??DataTable7_2  ;; 0x42c80000
        VDIV.F32 S0,S0,S1
        LDR.N    R0,??DataTable8_4
          CFI FunCall _ZN9SimplePwm21setSimplePwmDutyCycleEf
        BL       _ZN9SimplePwm21setSimplePwmDutyCycleEf
//   70 
//   71 
//   72 //		if (tps != prevTps) {
//   73 //			prevTps = tps;
//   74 //			scheduleMsg(&logger, "tps=%d", (int) tps);
//   75 //		}
//   76 
//   77 		// this thread is activated 10 times per second
//   78 		chThdSleepMilliseconds(boardConfiguration->etbDT);
        LDR.N    R0,??DataTable8_10
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+280]
        MOV      R1,#+1000
        MULS     R0,R1,R0
        SUBS     R0,R0,#+1
        MOV      R1,#+1000
        UDIV     R0,R0,R1
        ADDS     R0,R0,#+1
          CFI FunCall chThdSleep
        BL       chThdSleep
        B.N      ??etbThread_0
          CFI EndBlock cfiBlock1
//   79 	}
//   80 #if defined __GNUC__
//   81 	return -1;
//   82 #endif
//   83 }
//   84 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z18setThrottleConsolei
        THUMB
//   85 static void setThrottleConsole(int level) {
_Z18setThrottleConsolei:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//   86 	scheduleMsg(&logger, "setting throttle=%d", level);
        MOVS     R2,R4
        LDR.N    R1,??DataTable8_11
        LDR.N    R0,??DataTable8_3
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   87 
//   88 	float dc = 0.01 + (minI(level, 98)) / 100.0;
        MOVS     R1,#+98
        MOVS     R0,R4
          CFI FunCall minI
        BL       minI
          CFI FunCall __aeabi_i2d
        BL       __aeabi_i2d
        MOVS     R2,#+0
        LDR.N    R3,??DataTable8_12  ;; 0x40590000
          CFI FunCall __aeabi_ddiv
        BL       __aeabi_ddiv
        LDR.N    R2,??DataTable8_13  ;; 0x47ae147b
        LDR.N    R3,??DataTable8_14  ;; 0x3f847ae1
          CFI FunCall __aeabi_dadd
        BL       __aeabi_dadd
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        VMOV     S16,R0
//   89 	etbPwmUp.setSimplePwmDutyCycle(dc);
        VMOV.F32 S0,S16
        LDR.N    R0,??DataTable8_4
          CFI FunCall _ZN9SimplePwm21setSimplePwmDutyCycleEf
        BL       _ZN9SimplePwm21setSimplePwmDutyCycleEf
//   90 	print("st = %f\r\n", dc);
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R0,??DataTable8_15
          CFI FunCall print
        BL       print
//   91 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2
//   92 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z11showEthInfov
        THUMB
//   93 static void showEthInfo(void) {
_Z11showEthInfov:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        SUB      SP,SP,#+24
          CFI CFA R13+32
//   94 	static char pinNameBuffer[16];
//   95 
//   96 	scheduleMsg(&logger, "pedal=%f %d/%d @", getPedalPosition(), engineConfiguration->pedalPositionMin, engineConfiguration->pedalPositionMax,
//   97 			getPinNameByAdcChannel(engineConfiguration->pedalPositionChannel, pinNameBuffer));
        LDR.N    R1,??DataTable8_16
        LDR.N    R0,??DataTable8_17
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+604]
          CFI FunCall _Z22getPinNameByAdcChannel13adc_channel_ePc
        BL       _Z22getPinNameByAdcChannel13adc_channel_ePc
        MOVS     R4,R0
          CFI FunCall _Z16getPedalPositionv
        BL       _Z16getPedalPositionv
        STR      R4,[SP, #+8]
        LDR.N    R0,??DataTable8_17
        LDR      R0,[R0, #+0]
        LDRSH    R0,[R0, #+574]
        STR      R0,[SP, #+4]
        LDR.N    R0,??DataTable8_17
        LDR      R0,[R0, #+0]
        LDRSH    R0,[R0, #+572]
        STR      R0,[SP, #+0]
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable8_18
        LDR.N    R0,??DataTable8_3
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   98 
//   99 	scheduleMsg(&logger, "tsp=%f", getTPS());
          CFI FunCall _Z6getTPSv
        BL       _Z6getTPSv
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable8_19
        LDR.N    R0,??DataTable8_3
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  100 
//  101 	scheduleMsg(&logger, "etbControlPin1=%s duty=%f", hwPortname(boardConfiguration->etbControlPin1),
//  102 			currentEtbDuty);
        LDR.N    R0,??DataTable8_10
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+240]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R4,R0
        LDR.N    R0,??DataTable8_9
        LDR      R0,[R0, #+0]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        MOVS     R2,R4
        LDR.N    R1,??DataTable8_20
        LDR.N    R0,??DataTable8_3
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  103 	scheduleMsg(&logger, "etb P=%f I=%f D=%f dT=%d", boardConfiguration->etbPFactor,
//  104 			boardConfiguration->etbIFactor,
//  105 			0.0,
//  106 			boardConfiguration->etbDT);
        LDR.N    R0,??DataTable8_10
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+280]
        STR      R0,[SP, #+16]
        MOVS     R0,#+0
        MOVS     R1,#+0
        STRD     R0,R1,[SP, #+8]
        LDR.N    R0,??DataTable8_10
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+256]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        LDR.N    R0,??DataTable8_10
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+252]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable8_21
        LDR.N    R0,??DataTable8_3
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  107 }
        ADD      SP,SP,#+24
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
??pinNameBuffer:
        DS8 16
//  108 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z5applyv
        THUMB
//  109 static void apply(void) {
_Z5applyv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  110 	pid.updateFactors(boardConfiguration->etbPFactor, boardConfiguration->etbIFactor, 0);
        VLDR.W   S2,??DataTable7_1  ;; 0x0
        LDR.N    R0,??DataTable8_10
        LDR      R0,[R0, #+0]
        VLDR     S1,[R0, #+256]
        LDR.N    R0,??DataTable8_10
        LDR      R0,[R0, #+0]
        VLDR     S0,[R0, #+252]
        LDR.N    R0,??DataTable8_8
          CFI FunCall _ZN3Pid13updateFactorsEfff
        BL       _ZN3Pid13updateFactorsEfff
//  111 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock4
//  112 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z13setEtbPFactorf
        THUMB
//  113 static void setEtbPFactor(float value) {
_Z13setEtbPFactorf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  114 	boardConfiguration->etbPFactor = value;
        LDR.N    R0,??DataTable8_10
        LDR      R0,[R0, #+0]
        VSTR     S16,[R0, #+252]
//  115 	apply();
          CFI FunCall _Z5applyv
        BL       _Z5applyv
//  116 	showEthInfo();
          CFI FunCall _Z11showEthInfov
        BL       _Z11showEthInfov
//  117 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock5
//  118 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z13setEtbIFactorf
        THUMB
//  119 static void setEtbIFactor(float value) {
_Z13setEtbIFactorf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  120 	boardConfiguration->etbIFactor = value;
        LDR.N    R0,??DataTable8_10
        LDR      R0,[R0, #+0]
        VSTR     S16,[R0, #+256]
//  121 	apply();
          CFI FunCall _Z5applyv
        BL       _Z5applyv
//  122 	showEthInfo();
          CFI FunCall _Z11showEthInfov
        BL       _Z11showEthInfov
//  123 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock6

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6:
        DC8      "ETB"
//  124 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z23setDefaultEtbParametersv
          CFI NoCalls
        THUMB
//  125 void setDefaultEtbParameters(void) {
//  126 	engineConfiguration->pedalPositionMax = 6;
_Z23setDefaultEtbParametersv:
        LDR.N    R0,??DataTable8_17
        LDR      R0,[R0, #+0]
        MOVS     R1,#+6
        STRH     R1,[R0, #+574]
//  127 	boardConfiguration->etbPFactor = 10;
        LDR.N    R0,??DataTable8_10
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable8_22  ;; 0x41200000
        STR      R1,[R0, #+252]
//  128 	boardConfiguration->etbIFactor = 0;
        LDR.N    R0,??DataTable8_10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+256]
//  129 	boardConfiguration->etbDT = 100;
        LDR.N    R0,??DataTable8_10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+100
        STR      R1,[R0, #+280]
//  130 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock7

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7:
        DC32     0x42b40000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_1:
        DC32     0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_2:
        DC32     0x42c80000
//  131 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z14applyPinState2P9PwmConfigi
        THUMB
//  132 void applyPinState2(PwmConfig *state, int stateIndex) {
_Z14applyPinState2P9PwmConfigi:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  133 	applyPinState(state, stateIndex);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z13applyPinStateP9PwmConfigi
        BL       _Z13applyPinStateP9PwmConfigi
//  134 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock8
//  135 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z22initElectronicThrottlev
        THUMB
//  136 void initElectronicThrottle(void) {
_Z22initElectronicThrottlev:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  137 	// these two lines are controlling direction
//  138 //	outputPinRegister("etb1", ELECTRONIC_THROTTLE_CONTROL_1, ETB_CONTROL_LINE_1_PORT, ETB_CONTROL_LINE_1_PIN);
//  139 //	outputPinRegister("etb2", ELECTRONIC_THROTTLE_CONTROL_2, ETB_CONTROL_LINE_2_PORT, ETB_CONTROL_LINE_2_PIN);
//  140 
//  141 	// this line used for PWM
//  142 	startSimplePwmExt(&etbPwmUp, "etb1",
//  143 			boardConfiguration->etbControlPin1,
//  144 			&output1,
//  145 			ETB_FREQ,
//  146 			0.80,
//  147 			applyPinState2);
        LDR.N    R0,??DataTable8_23
        STR      R0,[SP, #+0]
        VLDR.W   S1,??DataTable8  ;; 0x3f4ccccd
        VLDR.W   S0,??DataTable8_1  ;; 0x43c80000
        LDR.N    R3,??DataTable8_5
        LDR.N    R0,??DataTable8_10
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+240]
        LDR.N    R1,??DataTable8_24
        LDR.N    R0,??DataTable8_4
          CFI FunCall _Z17startSimplePwmExtP9PwmConfigPKc11brain_pin_eP9OutputPinffPFvS0_iE
        BL       _Z17startSimplePwmExtP9PwmConfigPKc11brain_pin_eP9OutputPinffPFvS0_iE
//  148 	startSimplePwmExt(&etbPwmUp, "etb2",
//  149 			boardConfiguration->etbControlPin2,
//  150 			&output2,
//  151 			ETB_FREQ,
//  152 			0.80,
//  153 			applyPinState);
        LDR.N    R0,??DataTable8_25
        STR      R0,[SP, #+0]
        VLDR.W   S1,??DataTable8  ;; 0x3f4ccccd
        VLDR.W   S0,??DataTable8_1  ;; 0x43c80000
        LDR.N    R3,??DataTable8_7
        LDR.N    R0,??DataTable8_10
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+244]
        LDR.N    R1,??DataTable8_26
        LDR.N    R0,??DataTable8_4
          CFI FunCall _Z17startSimplePwmExtP9PwmConfigPKc11brain_pin_eP9OutputPinffPFvS0_iE
        BL       _Z17startSimplePwmExtP9PwmConfigPKc11brain_pin_eP9OutputPinffPFvS0_iE
//  154 
//  155 	addConsoleActionI("e", setThrottleConsole);
        LDR.N    R1,??DataTable8_27
        ADR.N    R0,??DataTable8_2  ;; "e"
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  156 
//  157 	addConsoleAction("ethinfo", showEthInfo);
        LDR.N    R1,??DataTable8_28
        LDR.N    R0,??DataTable8_29
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  158 
//  159 	addConsoleActionF("set_etb_p", setEtbPFactor);
        LDR.N    R1,??DataTable8_30
        LDR.N    R0,??DataTable8_31
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
//  160 	addConsoleActionF("set_etb_i", setEtbIFactor);
        LDR.N    R1,??DataTable8_32
        LDR.N    R0,??DataTable8_33
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
//  161 
//  162 	apply();
          CFI FunCall _Z5applyv
        BL       _Z5applyv
//  163 
//  164 	chThdCreateStatic(etbTreadStack, sizeof(etbTreadStack), NORMALPRIO, (tfunc_t) etbThread, NULL);
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        LDR.N    R3,??DataTable8_34
        MOVS     R2,#+64
        MOV      R1,#+712
        LDR.N    R0,??DataTable8_35
          CFI FunCall chThdCreateStatic
        BL       chThdCreateStatic
//  165 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock9

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8:
        DC32     0x3f4ccccd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_1:
        DC32     0x43c80000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_2:
        DC8      "e",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_3:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_4:
        DC32     etbPwmUp

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_5:
        DC32     output1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_6:
        DC32     etbPwmDown

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_7:
        DC32     output2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_8:
        DC32     pid

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_9:
        DC32     currentEtbDuty

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_10:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_11:
        DC32     `?<Constant "setting throttle=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_12:
        DC32     0x40590000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_13:
        DC32     0x47ae147b

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_14:
        DC32     0x3f847ae1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_15:
        DC32     `?<Constant "st = %f\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_16:
        DC32     ??pinNameBuffer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_17:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_18:
        DC32     `?<Constant "pedal=%f %d/%d @">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_19:
        DC32     `?<Constant "tsp=%f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_20:
        DC32     `?<Constant "etbControlPin1=%s dut...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_21:
        DC32     `?<Constant "etb P=%f I=%f D=%f dT=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_22:
        DC32     0x41200000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_23:
        DC32     _Z14applyPinState2P9PwmConfigi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_24:
        DC32     `?<Constant "etb1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_25:
        DC32     _Z13applyPinStateP9PwmConfigi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_26:
        DC32     `?<Constant "etb2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_27:
        DC32     _Z18setThrottleConsolei

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_28:
        DC32     _Z11showEthInfov

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_29:
        DC32     `?<Constant "ethinfo">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_30:
        DC32     _Z13setEtbPFactorf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_31:
        DC32     `?<Constant "set_etb_p">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_32:
        DC32     _Z13setEtbIFactorf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_33:
        DC32     `?<Constant "set_etb_i">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_34:
        DC32     _Z9etbThreadPv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_35:
        DC32     etbTreadStack

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
`?<Constant "setting throttle=%d">`:
        DATA
        DC8 "setting throttle=%d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "st = %f\\r\\n">`:
        DATA
        DC8 "st = %f\015\012"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "pedal=%f %d/%d @">`:
        DATA
        DC8 "pedal=%f %d/%d @"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "tsp=%f">`:
        DATA
        DC8 "tsp=%f"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "etbControlPin1=%s dut...">`:
        DATA
        DC8 "etbControlPin1=%s duty=%f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "etb P=%f I=%f D=%f dT=%d">`:
        DATA
        DC8 "etb P=%f I=%f D=%f dT=%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "etb1">`:
        DATA
        DC8 "etb1"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "etb2">`:
        DATA
        DC8 "etb2"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 "e"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ethinfo">`:
        DATA
        DC8 "ethinfo"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_etb_p">`:
        DATA
        DC8 "set_etb_p"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_etb_i">`:
        DATA
        DC8 "set_etb_i"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "ETB"

        END
//  166 
//  167 #endif /* EFI_ELECTRONIC_THROTTLE_BODY */
//  168 
// 
// 1 276 bytes in section .bss
//     4 bytes in section .init_array
//   170 bytes in section .rodata
//   872 bytes in section .text
// 
//   876 bytes of CODE  memory
//   170 bytes of CONST memory
// 1 276 bytes of DATA  memory
//
//Errors: none
//Warnings: 2
