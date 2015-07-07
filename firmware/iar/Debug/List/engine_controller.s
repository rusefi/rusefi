///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:18 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\eng /
//                       ine_controller.cpp                                   /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\eng /
//                       ine_controller.cpp -lCN F:\stuff\rusefi_sourceforge\ /
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
//                       engine_controller.s                                  /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME engine_controller

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z10getAdcMode13adc_channel_e
        EXTERN _Z11initSensorsP7Logging
        EXTERN _Z11isCrankingEP6Engine
        EXTERN _Z12boolToStringb
        EXTERN _Z12hasMafSensorv
        EXTERN _Z12initFsioImplP7LoggingP6Engine
        EXTERN _Z12initSettingsP22engine_configuration_s
        EXTERN _Z12updateAndSetP7State64j
        EXTERN _Z13hasBaroSensorv
        EXTERN _Z13interpolate2dfPfS_i
        EXTERN _Z13prepareShapesv
        EXTERN _Z15initSensorChartv
        EXTERN _Z15startIdleThreadP7LoggingP6Engine
        EXTERN _Z16getAdcChannelPin13adc_channel_e
        EXTERN _Z16initMapAveragingP7LoggingP6Engine
        EXTERN _Z16initPwmGeneratorv
        EXTERN _Z16initWaveAnalyzerP7Logging
        EXTERN _Z17getAdcChannelPort13adc_channel_e
        EXTERN _Z17initLcdControllerv
        EXTERN _Z17initRpmCalculatorP7LoggingP6Engine
        EXTERN _Z18initAlternatorCtrlP7Logging
        EXTERN _Z19getInternalAdcValuePKc13adc_channel_e
        EXTERN _Z19initAccelEnrichmentP7Logging
        EXTERN _Z19initInjectorCentralv
        EXTERN _Z21getCoolantTemperaturev
        EXTERN _Z21initMainEventListenerP7LoggingP6Engine
        EXTERN _Z21writeToFlashIfPendingv
        EXTERN _Z22initElectronicThrottlev
        EXTERN _Z23getIntakeAirTemperaturev
        EXTERN _Z24initMalfunctionIndicatorv
        EXTERN _Z25isValidCoolantTemperaturef
        EXTERN _Z27isValidIntakeAirTemperaturef
        EXTERN _Z28startTunerStudioConnectivityv
        EXTERN _Z6getTPSv
        EXTERN _Z7runFsiov
        EXTERN _Z8initAlgoP7LoggingP22engine_configuration_s
        EXTERN _Z8portnameP12GPIO_TypeDef
        EXTERN _ZN13RpmCalculator3rpmEv
        EXTERN _ZN13RpmCalculator9isRunningEv
        EXTERN _ZN16AccelEnrichmemnt5resetEv
        EXTERN _ZN16IntListenerArray22invokeJustArgCallbacksEv
        EXTERN _ZN17Overflow64Counter3getEv
        EXTERN _ZN17Overflow64CounterC1Ev
        EXTERN _ZN18LocalVersionHolder5isOldEv
        EXTERN _ZN18LocalVersionHolderC1Ev
        EXTERN _ZN18LoggingWithStorageC1EPKc
        EXTERN _ZN23engine_configuration2_sC1Ev
        EXTERN _ZN6Engine15printKnockStateEv
        EXTERN _ZN6Engine17updateSlowSensorsEv
        EXTERN _ZN6Engine20periodicFastCallbackEv
        EXTERN _ZN6Engine8watchdogEv
        EXTERN _ZN6EngineC1EP19persistent_config_s
        EXTERN _ZN9AdcDevice8isHwUsedE13adc_channel_e
        EXTERN _ZN9OutputPin8setValueEi
        EXTERN __aeabi_atexit
        EXTERN __aeabi_f2d
        EXTERN __aeabi_ldivmod
        EXTERN __dso_handle
        EXTERN __iar_FDtest
        EXTERN absI
        EXTERN addConsoleAction
        EXTERN addConsoleActionI
        EXTERN addConsoleActionII
        EXTERN addConsoleActionSS
        EXTERN addConsoleActionSSS
        EXTERN applyNewConfiguration
        EXTERN atoff
        EXTERN atoi
        EXTERN chThdCreateStatic
        EXTERN chThdSleep
        EXTERN chVTSetAny
        EXTERN fastAdc
        EXTERN firmwareError
        EXTERN getRemainingStack
        EXTERN hasFirmwareErrorFlag
        EXTERN initMalfunctionCentral
        EXTERN itoa10
        EXTERN lockAnyContext
        EXTERN rlist
        EXTERN runningPin
        EXTERN scheduleMsg
        EXTERN setError
        EXTERN strcpy
        EXTERN unlockAnyContext
        EXTERN vtlist

        PUBLIC _Z17initConfigActionsv
        PUBLIC _Z18initPeriodicEventsv
        PUBLIC _Z19initEngineContollerP7Logging
        PUBLIC _Z22getPinNameByAdcChannel13adc_channel_ePc
        PUBLIC _ZN13cyclic_bufferIfED1Ev
        PUBLIC _ZN13cyclic_bufferIfED2Ev
        PUBLIC _ZN16AccelEnrichmemntD1Ev
        PUBLIC _ZN6EngineD1Ev
        PUBLIC _engine
        PUBLIC boardConfiguration
        PUBLIC config
        PUBLIC currentTimeMillis
        PUBLIC engine
        PUBLIC engineConfiguration
        PUBLIC engineConfiguration2
        PUBLIC getTimeNowNt
        PUBLIC getTimeNowSeconds
        PUBLIC getTimeNowUs
        PUBLIC halTime
        PUBLIC persistentState
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\engine_controller.cpp
//    1 /**
//    2  * @file    engine_controller.cpp
//    3  * @brief   Controllers package entry point code
//    4  *
//    5  *
//    6  *
//    7  * @date Feb 7, 2013
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
//   24 #include <sensor_chart.h>
//   25 #include "main.h"
//   26 #include "engine_configuration.h"
//   27 #include "trigger_central.h"
//   28 #include "engine_controller.h"
//   29 #include "fsio_core.h"
//   30 #include "fsio_impl.h"
//   31 #include "idle_thread.h"
//   32 #include "rpm_calculator.h"
//   33 #include "signal_executor.h"
//   34 #include "main_trigger_callback.h"
//   35 #include "io_pins.h"
//   36 #include "flash_main.h"
//   37 #include "tunerstudio.h"
//   38 #include "injector_central.h"
//   39 #include "rfiutil.h"
//   40 #include "engine_math.h"
//   41 #include "wave_analyzer.h"
//   42 #include "allsensors.h"
//   43 #include "electronic_throttle.h"
//   44 #include "map_averaging.h"
//   45 #include "malfunction_central.h"
//   46 #include "malfunction_indicator.h"
//   47 #include "engine.h"
//   48 #include "algo.h"
//   49 #include "LocalVersionHolder.h"
//   50 #include "alternatorController.h"
//   51 #include "fuel_math.h"
//   52 #include "settings.h"
//   53 
//   54 #if HAL_USE_ADC || defined(__DOXYGEN__)
//   55 #include "AdcConfiguration.h"
//   56 #endif
//   57 
//   58 #if EFI_PROD_CODE
//   59 #include "pwm_generator.h"
//   60 #include "adc_inputs.h"
//   61 #include "efilib2.h"
//   62 #include "PwmTester.h"
//   63 #include "pwm_generator.h"
//   64 #include "lcd_controller.h"
//   65 #include "pin_repository.h"
//   66 #endif
//   67 
//   68 extern bool hasFirmwareErrorFlag;
//   69 

        SECTION `.ccm`:DATA:REORDER:NOROOT(2)
//   70 persistent_config_container_s persistentState CCM_OPTIONAL;
persistentState:
        DS8 15300
//   71 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   72 persistent_config_s *config = &persistentState.persistentConfiguration;
config:
        DATA
        DC32 persistentState + 8H
//   73 
//   74 /**
//   75  * todo: it really looks like these fields should become 'static', i.e. private
//   76  * the whole 'extern ...' pattern is less then perfect, I guess the 'God object' Engine
//   77  * would be a smaller evil. Whatever is needed should be passed into methods/modules/files as an explicit parameter.
//   78  */

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   79 engine_configuration_s *engineConfiguration = &persistentState.persistentConfiguration.engineConfiguration;
engineConfiguration:
        DATA
        DC32 persistentState + 8H

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   80 board_configuration_s *boardConfiguration = &persistentState.persistentConfiguration.engineConfiguration.bc;
boardConfiguration:
        DATA
        DC32 persistentState + 278H
//   81 
//   82 /**
//   83  * CH_FREQUENCY is the number of system ticks in a second
//   84  */
//   85 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   86 static VirtualTimer periodicSlowTimer;
periodicSlowTimer:
        DS8 20

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   87 static VirtualTimer periodicFastTimer;
periodicFastTimer:
        DS8 20
//   88 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   89 static LoggingWithStorage logger("Engine Controller");
        LDR.W    R1,??DataTable23
        LDR.W    R0,??DataTable23_1
          CFI FunCall _ZN18LoggingWithStorageC1EPKc
        BL       _ZN18LoggingWithStorageC1EPKc
//   90 
//   91 #if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
//   92 static engine_configuration2_s ec2 CCM_OPTIONAL;
        LDR.W    R0,??DataTable23_2
          CFI FunCall _ZN23engine_configuration2_sC1Ev
        BL       _ZN23engine_configuration2_sC1Ev
//   93 engine_configuration2_s * engineConfiguration2 = &ec2;
//   94 #endif
//   95 
//   96 #if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
//   97 
//   98 /**
//   99  * todo: this should probably become 'static', i.e. private, and propagated around explicitly?
//  100  */
//  101 Engine _engine(&persistentState.persistentConfiguration);
        LDR.W    R1,??DataTable23_3
        LDR.W    R0,??DataTable23_4
          CFI FunCall _ZN6EngineC1EP19persistent_config_s
        BL       _ZN6EngineC1EP19persistent_config_s
        LDR.W    R2,??DataTable23_5
        LDR.W    R1,??DataTable23_6
          CFI FunCall __aeabi_atexit
        BL       __aeabi_atexit
        LDR.W    R0,??DataTable23_7
          CFI FunCall _ZN17Overflow64CounterC1Ev
        BL       _ZN17Overflow64CounterC1Ev
        LDR.W    R0,??DataTable23_8
          CFI FunCall _ZN18LocalVersionHolderC1Ev
        BL       _ZN18LocalVersionHolderC1Ev
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
logger:
        DS8 260

        SECTION `.ccm`:DATA:REORDER:NOROOT(3)
ec2:
        DS8 27488

        SECTION `.data`:DATA:REORDER:NOROOT(2)
engineConfiguration2:
        DATA
        DC32 ec2

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
_engine:
        DS8 9240

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  102 Engine * engine = &_engine;
engine:
        DATA
        DC32 _engine
//  103 #endif
//  104 
//  105 /**
//  106  * I am not sure if this needs to be configurable.
//  107  *
//  108  * Also technically the whole feature might be implemented as cranking fuel coefficient curve by TPS.
//  109  */
//  110 #define CLEANUP_MODE_TPS 95
//  111 
//  112 extern OutputPin runningPin;
//  113 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z8csThreadv
        THUMB
//  114 static msg_t csThread(void) {
_Z8csThreadv:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
//  115 	chRegSetThreadName("status");
        LDR.W    R0,??DataTable23_9
        LDR      R0,[R0, #+24]
        LDR.W    R1,??DataTable23_10
        STR      R1,[R0, #+24]
//  116 #if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
//  117 	while (true) {
//  118 		int rpm = getRpm();
??csThread_0:
        LDR.W    R0,??DataTable23_11
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN13RpmCalculator3rpmEv
        BL       _ZN13RpmCalculator3rpmEv
        MOVS     R4,R0
//  119 		int is_cranking = isCrankingR(rpm);
        CMP      R4,#+1
        BLT.N    ??csThread_1
        LDR.W    R0,??DataTable23_12
        LDRSH    R0,[R0, #+104]
        CMP      R4,R0
        BGE.N    ??csThread_1
        MOVS     R5,#+1
        B.N      ??csThread_2
??csThread_1:
        MOVS     R5,#+0
??csThread_2:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
//  120 		int is_running = rpm > 0 && !is_cranking;
        CMP      R4,#+1
        BLT.N    ??csThread_3
        CMP      R5,#+0
        BNE.N    ??csThread_3
        MOVS     R6,#+1
        B.N      ??csThread_4
??csThread_3:
        MOVS     R6,#+0
??csThread_4:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
//  121 		if (is_running) {
        CMP      R6,#+0
        BEQ.N    ??csThread_5
//  122 			// blinking while running
//  123 			runningPin.setValue(0);
        MOVS     R1,#+0
        LDR.W    R0,??DataTable23_13
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
//  124 			chThdSleepMilliseconds(50);
        MOVS     R0,#+50
          CFI FunCall chThdSleep
        BL       chThdSleep
//  125 			runningPin.setValue(1);
        MOVS     R1,#+1
        LDR.W    R0,??DataTable23_13
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
//  126 			chThdSleepMilliseconds(50);
        MOVS     R0,#+50
          CFI FunCall chThdSleep
        BL       chThdSleep
        B.N      ??csThread_0
//  127 		} else {
//  128 			// constant on while cranking and off if engine is stopped
//  129 			runningPin.setValue(is_cranking);
??csThread_5:
        MOVS     R1,R5
        LDR.W    R0,??DataTable23_13
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
//  130 			chThdSleepMilliseconds(100);
        MOVS     R0,#+100
          CFI FunCall chThdSleep
        BL       chThdSleep
        B.N      ??csThread_0
          CFI EndBlock cfiBlock1
//  131 		}
//  132 	}
//  133 #endif /* EFI_SHAFT_POSITION_INPUT */
//  134 	return -1;
//  135 }
//  136 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z16updateErrorCodesv
        THUMB
//  137 static void updateErrorCodes(void) {
_Z16updateErrorCodesv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  138 	/**
//  139 	 * technically we can set error codes right inside the getMethods, but I a bit on a fence about it
//  140 	 */
//  141 	setError(!isValidIntakeAirTemperature(getIntakeAirTemperature(PASS_ENGINE_PARAMETER_F)),
//  142 			OBD_Intake_Air_Temperature_Circuit_Malfunction);
          CFI FunCall _Z23getIntakeAirTemperaturev
        BL       _Z23getIntakeAirTemperaturev
          CFI FunCall _Z27isValidIntakeAirTemperaturef
        BL       _Z27isValidIntakeAirTemperaturef
        MOVS     R1,#+110
        EORS     R0,R0,#0x1
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall setError
        BL       setError
//  143 	setError(!isValidCoolantTemperature(getCoolantTemperature(PASS_ENGINE_PARAMETER_F)),
//  144 			OBD_Engine_Coolant_Temperature_Circuit_Malfunction);
          CFI FunCall _Z21getCoolantTemperaturev
        BL       _Z21getCoolantTemperaturev
          CFI FunCall _Z25isValidCoolantTemperaturef
        BL       _Z25isValidCoolantTemperaturef
        MOVS     R1,#+115
        EORS     R0,R0,#0x1
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall setError
        BL       setError
//  145 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock2
//  146 
//  147 #if EFI_PROD_CODE || defined(__DOXYGEN__)

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//  148 Overflow64Counter halTime;
halTime:
        DS8 16
//  149 
//  150 //todo: macro to save method invocation

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function getTimeNowUs
        THUMB
//  151 efitimeus_t getTimeNowUs(void) {
getTimeNowUs:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  152 	return getTimeNowNt() / (CORE_CLOCK / 1000000);
          CFI FunCall getTimeNowNt
        BL       getTimeNowNt
        MOVS     R2,#+168
        MOVS     R3,#+0
          CFI FunCall __aeabi_ldivmod
        BL       __aeabi_ldivmod
        POP      {R2,PC}          ;; return
          CFI EndBlock cfiBlock3
//  153 }
//  154 
//  155 //todo: macro to save method invocation

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function getTimeNowNt
        THUMB
//  156 efitick_t getTimeNowNt(void) {
getTimeNowNt:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  157 	return halTime.get();
        LDR.W    R0,??DataTable23_7
          CFI FunCall _ZN17Overflow64Counter3getEv
        BL       _ZN17Overflow64Counter3getEv
        POP      {R2,PC}          ;; return
          CFI EndBlock cfiBlock4
//  158 }
//  159 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function currentTimeMillis
          CFI NoCalls
        THUMB
//  160 efitimems_t currentTimeMillis(void) {
//  161 	// todo: migrate to getTimeNowUs? or not?
//  162 	return chTimeNow() / TICKS_IN_MS;
currentTimeMillis:
        LDR.W    R0,??DataTable23_14
        LDR      R0,[R0, #+12]
        MOVS     R1,#+1
        UDIV     R0,R0,R1
        BX       LR               ;; return
          CFI EndBlock cfiBlock5
//  163 }
//  164 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function getTimeNowSeconds
          CFI NoCalls
        THUMB
//  165 int getTimeNowSeconds(void) {
//  166 	return chTimeNow() / CH_FREQUENCY;
getTimeNowSeconds:
        LDR.W    R0,??DataTable23_14
        LDR      R0,[R0, #+12]
        MOV      R1,#+1000
        UDIV     R0,R0,R1
        BX       LR               ;; return
          CFI EndBlock cfiBlock6
//  167 }
//  168 
//  169 #endif /* EFI_PROD_CODE */
//  170 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z22cylinderCleanupControlP6Engine
        THUMB
//  171 static void cylinderCleanupControl(Engine *engine) {
_Z22cylinderCleanupControlP6Engine:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  172 #if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
//  173 	bool newValue;
//  174 	if (engineConfiguration->isCylinderCleanupEnabled) {
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+2
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??cylinderCleanupControl_0
//  175 		newValue = isCrankingE(engine) && getTPS(PASS_ENGINE_PARAMETER_F) > CLEANUP_MODE_TPS;
        MOVS     R0,R4
          CFI FunCall _Z11isCrankingEP6Engine
        BL       _Z11isCrankingEP6Engine
        CMP      R0,#+0
        BEQ.N    ??cylinderCleanupControl_1
          CFI FunCall _Z6getTPSv
        BL       _Z6getTPSv
        VLDR.W   S1,??DataTable10  ;; 0x42be0001
        VCMP.F32 S0,S1
        FMSTAT   
        BLT.N    ??cylinderCleanupControl_1
        MOVS     R5,#+1
        B.N      ??cylinderCleanupControl_2
??cylinderCleanupControl_1:
        MOVS     R5,#+0
??cylinderCleanupControl_2:
        B.N      ??cylinderCleanupControl_3
//  176 	} else {
//  177 		newValue = false;
??cylinderCleanupControl_0:
        MOVS     R0,#+0
        MOVS     R5,R0
//  178 	}
//  179 	if (newValue != engine->isCylinderCleanupMode) {
??cylinderCleanupControl_3:
        MOVW     R0,#+7928
        LDRB     R0,[R0, R4]
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,R0
        BEQ.N    ??cylinderCleanupControl_4
//  180 		engine->isCylinderCleanupMode = newValue;
        MOVW     R0,#+7928
        STRB     R5,[R0, R4]
//  181 		scheduleMsg(&logger, "isCylinderCleanupMode %s", boolToString(newValue));
        MOVS     R0,R5
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R2,R0
        LDR.W    R1,??DataTable23_16
        LDR.W    R0,??DataTable23_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  182 	}
//  183 #endif
//  184 }
??cylinderCleanupControl_4:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock7
//  185 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  186 static LocalVersionHolder versionForConfigurationListeners;
versionForConfigurationListeners:
        DS8 4
//  187 
//  188 static void periodicSlowCallback(Engine *engine);
//  189 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z26scheduleNextSlowInvocationv
        THUMB
//  190 static void scheduleNextSlowInvocation(void) {
_Z26scheduleNextSlowInvocationv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  191 	// schedule next invocation
//  192 	int period = boardConfiguration->generalPeriodicThreadPeriod;
        LDR.W    R0,??DataTable23_17
        LDR      R0,[R0, #+0]
        LDR      R4,[R0, #+328]
//  193 	if (period == 0)
        CMP      R4,#+0
        BNE.N    ??scheduleNextSlowInvocation_0
//  194 		period = 50; // this might happen while resetting config
        MOVS     R0,#+50
        MOVS     R4,R0
//  195 	chVTSetAny(&periodicSlowTimer, period * TICKS_IN_MS, (vtfunc_t) &periodicSlowCallback, engine);
??scheduleNextSlowInvocation_0:
        LDR.W    R0,??DataTable23_11
        LDR      R3,[R0, #+0]
        ADR.W    R2,_Z20periodicSlowCallbackP6Engine
        MOVS     R1,R4
        LDR.W    R0,??DataTable23_18
          CFI FunCall chVTSetAny
        BL       chVTSetAny
//  196 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock8
//  197 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z20periodicFastCallbackv
        THUMB
//  198 static void periodicFastCallback(DECLARE_ENGINE_PARAMETER_F) {
_Z20periodicFastCallbackv:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
//  199 	int rpm = engine->rpmCalculator.rpmValue;
        LDR.W    R0,??DataTable23_11
        LDR      R0,[R0, #+0]
        LDR      R4,[R0, #+0]
//  200 
//  201 	if (isValidRpm(rpm)) {
        CMP      R4,#+1
        BLT.N    ??periodicFastCallback_0
        MOVW     R0,#+30000
        CMP      R4,R0
        BGE.N    ??periodicFastCallback_0
//  202 		MAP_sensor_config_s * c = &engineConfiguration->map;
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        ADDS     R5,R0,#+112
//  203 		engine->engineState.mapAveragingStart = interpolate2d(rpm, c->samplingAngleBins, c->samplingAngle, MAP_ANGLE_SIZE);
        MOVS     R2,#+8
        ADDS     R1,R5,#+32
        MOVS     R0,R5
        VMOV     S0,R4
        VCVT.F32.S32 S0,S0
          CFI FunCall _Z13interpolate2dfPfS_i
        BL       _Z13interpolate2dfPfS_i
        LDR.W    R0,??DataTable23_11
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+7680
        ADDS     R0,R0,#+172
        VSTR     S0,[R0, #0]
//  204 		engine->engineState.mapAveragingDuration = interpolate2d(rpm, c->samplingWindowBins, c->samplingWindow, MAP_WINDOW_SIZE);
        MOVS     R2,#+8
        ADDS     R1,R5,#+96
        ADDS     R0,R5,#+64
        VMOV     S0,R4
        VCVT.F32.S32 S0,S0
          CFI FunCall _Z13interpolate2dfPfS_i
        BL       _Z13interpolate2dfPfS_i
        LDR.W    R0,??DataTable23_11
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+7680
        ADDS     R0,R0,#+176
        VSTR     S0,[R0, #0]
        B.N      ??periodicFastCallback_1
//  205 	} else {
//  206 		engine->engineState.mapAveragingStart = NAN;
??periodicFastCallback_0:
        MOVW     R0,#+7852
        LDR.W    R1,??DataTable23_11
        LDR      R1,[R1, #+0]
        MVNS     R2,#-2147483648
        STR      R2,[R0, R1]
//  207 		engine->engineState.mapAveragingDuration = NAN;
        MOVW     R0,#+7856
        LDR.W    R1,??DataTable23_11
        LDR      R1,[R1, #+0]
        MVNS     R2,#-2147483648
        STR      R2,[R0, R1]
//  208 	}
//  209 
//  210 	engine->periodicFastCallback();
??periodicFastCallback_1:
        LDR.W    R0,??DataTable23_11
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN6Engine20periodicFastCallbackEv
        BL       _ZN6Engine20periodicFastCallbackEv
//  211 
//  212 	chVTSetAny(&periodicFastTimer, 20 * TICKS_IN_MS, (vtfunc_t) &periodicFastCallback, engine);
        LDR.W    R0,??DataTable23_11
        LDR      R3,[R0, #+0]
        LDR.W    R2,??DataTable23_19
        MOVS     R1,#+20
        LDR.W    R0,??DataTable23_20
          CFI FunCall chVTSetAny
        BL       chVTSetAny
//  213 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock9
//  214 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z20periodicSlowCallbackP6Engine
        THUMB
//  215 static void periodicSlowCallback(Engine *engine) {
_Z20periodicSlowCallbackP6Engine:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  216 	efiAssertVoid(getRemainingStack(chThdSelf()) > 64, "lowStckOnEv");
        LDR.W    R0,??DataTable23_9
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+65
        BGE.N    ??periodicSlowCallback_0
        LDR.W    R0,??DataTable23_21
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??periodicSlowCallback_1
//  217 #if EFI_PROD_CODE
//  218 	/**
//  219 	 * We need to push current value into the 64 bit counter often enough so that we do not miss an overflow
//  220 	 */
//  221 	bool alreadyLocked = lockAnyContext();
??periodicSlowCallback_0:
          CFI FunCall lockAnyContext
        BL       lockAnyContext
        MOVS     R5,R0
//  222 	updateAndSet(&halTime.state, hal_lld_get_counter_value());
        LDR.W    R0,??DataTable23_22  ;; 0xe0001004
        LDR      R1,[R0, #+0]
        LDR.W    R0,??DataTable23_7
          CFI FunCall _Z12updateAndSetP7State64j
        BL       _Z12updateAndSetP7State64j
//  223 	if (!alreadyLocked) {
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BNE.N    ??periodicSlowCallback_2
//  224 		unlockAnyContext();
          CFI FunCall unlockAnyContext
        BL       unlockAnyContext
//  225 	}
//  226 #endif
//  227 
//  228 #if (EFI_PROD_CODE && EFI_ENGINE_CONTROL && EFI_INTERNAL_FLASH) || defined(__DOXYGEN__)
//  229 	if (!engine->rpmCalculator.isRunning()) {
??periodicSlowCallback_2:
        MOVS     R0,R4
          CFI FunCall _ZN13RpmCalculator9isRunningEv
        BL       _ZN13RpmCalculator9isRunningEv
        CMP      R0,#+0
        BNE.N    ??periodicSlowCallback_3
//  230 		writeToFlashIfPending();
          CFI FunCall _Z21writeToFlashIfPendingv
        BL       _Z21writeToFlashIfPendingv
//  231 	}
//  232 #endif
//  233 
//  234 	if (versionForConfigurationListeners.isOld()) {
??periodicSlowCallback_3:
        LDR.W    R0,??DataTable23_8
          CFI FunCall _ZN18LocalVersionHolder5isOldEv
        BL       _ZN18LocalVersionHolder5isOldEv
        CMP      R0,#+0
        BEQ.N    ??periodicSlowCallback_4
//  235 		/**
//  236 		 * version change could happen for multiple reason and on different threads
//  237 		 * in order to be sure which thread (and which stack) invokes the potentially heavy
//  238 		 * listeners we invoke them from here.
//  239 		 */
//  240 		engine->configurationListeners.invokeJustArgCallbacks();
        ADD      R0,R4,#+8960
        ADDS     R0,R0,#+68
          CFI FunCall _ZN16IntListenerArray22invokeJustArgCallbacksEv
        BL       _ZN16IntListenerArray22invokeJustArgCallbacksEv
//  241 	}
//  242 
//  243 	engine->watchdog();
??periodicSlowCallback_4:
        MOVS     R0,R4
          CFI FunCall _ZN6Engine8watchdogEv
        BL       _ZN6Engine8watchdogEv
//  244 	engine->updateSlowSensors();
        MOVS     R0,R4
          CFI FunCall _ZN6Engine17updateSlowSensorsEv
        BL       _ZN6Engine17updateSlowSensorsEv
//  245 
//  246 #if (EFI_PROD_CODE && EFI_FSIO) || defined(__DOXYGEN__)
//  247 	runFsio();
          CFI FunCall _Z7runFsiov
        BL       _Z7runFsiov
//  248 #endif
//  249 
//  250 	updateErrorCodes();
          CFI FunCall _Z16updateErrorCodesv
        BL       _Z16updateErrorCodesv
//  251 
//  252 	cylinderCleanupControl(engine);
        MOVS     R0,R4
          CFI FunCall _Z22cylinderCleanupControlP6Engine
        BL       _Z22cylinderCleanupControlP6Engine
//  253 
//  254 	scheduleNextSlowInvocation();
          CFI FunCall _Z26scheduleNextSlowInvocationv
        BL       _Z26scheduleNextSlowInvocationv
//  255 }
??periodicSlowCallback_1:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock10
//  256 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z18initPeriodicEventsv
        THUMB
//  257 void initPeriodicEvents(DECLARE_ENGINE_PARAMETER_F) {
_Z18initPeriodicEventsv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  258 	scheduleNextSlowInvocation();
          CFI FunCall _Z26scheduleNextSlowInvocationv
        BL       _Z26scheduleNextSlowInvocationv
//  259 	periodicFastCallback(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z20periodicFastCallbackv
        BL       _Z20periodicFastCallbackv
//  260 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock11
//  261 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _Z22getPinNameByAdcChannel13adc_channel_ePc
        THUMB
//  262 char * getPinNameByAdcChannel(adc_channel_e hwChannel, char *buffer) {
_Z22getPinNameByAdcChannel13adc_channel_ePc:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  263 #if HAL_USE_ADC || defined(__DOXYGEN__)
//  264 	strcpy((char*) buffer, portname(getAdcChannelPort(hwChannel)));
        MOVS     R0,R4
          CFI FunCall _Z17getAdcChannelPort13adc_channel_e
        BL       _Z17getAdcChannelPort13adc_channel_e
          CFI FunCall _Z8portnameP12GPIO_TypeDef
        BL       _Z8portnameP12GPIO_TypeDef
        MOVS     R1,R0
        MOVS     R0,R5
          CFI FunCall strcpy
        BL       strcpy
//  265 	itoa10(&buffer[2], getAdcChannelPin(hwChannel));
        MOVS     R0,R4
          CFI FunCall _Z16getAdcChannelPin13adc_channel_e
        BL       _Z16getAdcChannelPin13adc_channel_e
        MOVS     R1,R0
        ADDS     R0,R5,#+2
          CFI FunCall itoa10
        BL       itoa10
//  266 #else
//  267 	strcpy(buffer, "NONE");
//  268 #endif
//  269 	return (char*) buffer;
        MOVS     R0,R5
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock12
//  270 }
//  271 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  272 static char pinNameBuffer[16];
pinNameBuffer:
        DS8 16
//  273 
//  274 #if HAL_USE_ADC || defined(__DOXYGEN__)
//  275 extern AdcDevice fastAdc;
//  276 #endif
//  277 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z25printAnalogChannelInfoExtPKc13adc_channel_eff
        THUMB
//  278 static void printAnalogChannelInfoExt(const char *name, adc_channel_e hwChannel, float adcVoltage,
//  279 		float dividerCoeff) {
_Z25printAnalogChannelInfoExtPKc13adc_channel_eff:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        VPUSH    {D8-D9}
          CFI D9 Frame(CFA, -32)
          CFI D8 Frame(CFA, -40)
          CFI CFA R13+40
        SUB      SP,SP,#+32
          CFI CFA R13+72
        MOVS     R4,R0
        MOVS     R5,R1
        VMOV.F32 S16,S0
        VMOV.F32 S17,S1
//  280 #if HAL_USE_ADC || defined(__DOXYGEN__)
//  281 	if (hwChannel == EFI_ADC_NONE) {
        CMP      R5,#+16
        BNE.N    ??printAnalogChannelInfoExt_0
//  282 		scheduleMsg(&logger, "ADC is not assigned for %s", name);
        MOVS     R2,R4
        LDR.W    R1,??DataTable23_23
        LDR.W    R0,??DataTable23_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  283 		return;
        B.N      ??printAnalogChannelInfoExt_1
//  284 	}
//  285 
//  286 	if (fastAdc.isHwUsed(hwChannel)) {
??printAnalogChannelInfoExt_0:
        MOVS     R1,R5
        LDR.W    R0,??DataTable23_24
          CFI FunCall _ZN9AdcDevice8isHwUsedE13adc_channel_e
        BL       _ZN9AdcDevice8isHwUsedE13adc_channel_e
        CMP      R0,#+0
        BEQ.N    ??printAnalogChannelInfoExt_2
//  287 		scheduleMsg(&logger, "fast enabled=%s", boolToString(boardConfiguration->isFastAdcEnabled));
        LDR.W    R0,??DataTable23_17
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+4
        ANDS     R0,R0,#0x1
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R2,R0
        LDR.W    R1,??DataTable23_25
        LDR.W    R0,??DataTable23_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  288 	}
//  289 
//  290 	float voltage = adcVoltage * dividerCoeff;
??printAnalogChannelInfoExt_2:
        VMUL.F32 S18,S16,S17
//  291 	scheduleMsg(&logger, "%s ADC%d %s %s rawValue=%f/divided=%fv/divider=%f", name, hwChannel, getAdcMode(hwChannel),
//  292 			getPinNameByAdcChannel(hwChannel, pinNameBuffer), adcVoltage, voltage, dividerCoeff);
        LDR.W    R1,??DataTable23_26
        MOVS     R0,R5
          CFI FunCall _Z22getPinNameByAdcChannel13adc_channel_ePc
        BL       _Z22getPinNameByAdcChannel13adc_channel_ePc
        MOVS     R6,R0
        MOVS     R0,R5
          CFI FunCall _Z10getAdcMode13adc_channel_e
        BL       _Z10getAdcMode13adc_channel_e
        MOVS     R7,R0
        VMOV     R0,S17
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+24]
        VMOV     R0,S18
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+16]
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+8]
        STR      R6,[SP, #+4]
        STR      R7,[SP, #+0]
        MOVS     R3,R5
        MOVS     R2,R4
        LDR.W    R1,??DataTable23_27
        LDR.W    R0,??DataTable23_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  293 #endif
//  294 }
??printAnalogChannelInfoExt_1:
        ADD      SP,SP,#+32
          CFI CFA R13+40
        VPOP     {D8-D9}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI CFA R13+24
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock13
//  295 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _Z22printAnalogChannelInfoPKc13adc_channel_e
        THUMB
//  296 static void printAnalogChannelInfo(const char *name, adc_channel_e hwChannel) {
_Z22printAnalogChannelInfoPKc13adc_channel_e:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  297 #if HAL_USE_ADC || defined(__DOXYGEN__)
//  298 	printAnalogChannelInfoExt(name, hwChannel, getVoltage("print", hwChannel), engineConfiguration->analogInputDividerCoefficient);
        MOVS     R1,R5
        LDR.W    R0,??DataTable23_28
          CFI FunCall _Z19getInternalAdcValuePKc13adc_channel_e
        BL       _Z19getInternalAdcValuePKc13adc_channel_e
        VMOV     S0,R0
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        VLDR     S1,[R0, #+464]
        VCVT.F32.S32 S0,S0
        VLDR.W   S2,??DataTable16  ;; 0x3a400c01
        VMUL.F32 S0,S0,S2
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z25printAnalogChannelInfoExtPKc13adc_channel_eff
        BL       _Z25printAnalogChannelInfoExtPKc13adc_channel_eff
//  299 #endif
//  300 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock14

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10:
        DC32     0x42be0001
//  301 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function _Z15printAnalogInfov
        THUMB
//  302 static void printAnalogInfo(void) {
_Z15printAnalogInfov:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  303 	scheduleMsg(&logger, "analogInputDividerCoefficient: %f", engineConfiguration->analogInputDividerCoefficient);
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+464]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable23_29
        LDR.W    R0,??DataTable23_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  304 
//  305 	printAnalogChannelInfo("hip9011", engineConfiguration->hipOutputChannel);
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+1500]
        LDR.W    R0,??DataTable23_30
          CFI FunCall _Z22printAnalogChannelInfoPKc13adc_channel_e
        BL       _Z22printAnalogChannelInfoPKc13adc_channel_e
//  306 	printAnalogChannelInfo("fuel gauge", engineConfiguration->fuelLevelSensor);
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+484]
        LDR.W    R0,??DataTable23_31
          CFI FunCall _Z22printAnalogChannelInfoPKc13adc_channel_e
        BL       _Z22printAnalogChannelInfoPKc13adc_channel_e
//  307 	printAnalogChannelInfo("TPS", engineConfiguration->tpsAdcChannel);
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+524]
        ADR.N    R0,??DataTable15  ;; "TPS"
          CFI FunCall _Z22printAnalogChannelInfoPKc13adc_channel_e
        BL       _Z22printAnalogChannelInfoPKc13adc_channel_e
//  308 	printAnalogChannelInfo("pPS", engineConfiguration->pedalPositionChannel);
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+604]
        ADR.N    R0,??DataTable15_1  ;; "pPS"
          CFI FunCall _Z22printAnalogChannelInfoPKc13adc_channel_e
        BL       _Z22printAnalogChannelInfoPKc13adc_channel_e
//  309 	printAnalogChannelInfo("CLT", engineConfiguration->clt.adcChannel);
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+284]
        ADR.N    R0,??DataTable15_2  ;; "CLT"
          CFI FunCall _Z22printAnalogChannelInfoPKc13adc_channel_e
        BL       _Z22printAnalogChannelInfoPKc13adc_channel_e
//  310 	if (engineConfiguration->hasIatSensor) {
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+1
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??printAnalogInfo_0
//  311 		printAnalogChannelInfo("IAT", engineConfiguration->iat.adcChannel);
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+316]
        ADR.N    R0,??DataTable16_1  ;; "IAT"
          CFI FunCall _Z22printAnalogChannelInfoPKc13adc_channel_e
        BL       _Z22printAnalogChannelInfoPKc13adc_channel_e
//  312 	}
//  313 	if (hasMafSensor()) {
??printAnalogInfo_0:
          CFI FunCall _Z12hasMafSensorv
        BL       _Z12hasMafSensorv
        CMP      R0,#+0
        BEQ.N    ??printAnalogInfo_1
//  314 		printAnalogChannelInfo("MAF", engineConfiguration->mafAdcChannel);
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+580]
        ADR.N    R0,??DataTable17  ;; "MAF"
          CFI FunCall _Z22printAnalogChannelInfoPKc13adc_channel_e
        BL       _Z22printAnalogChannelInfoPKc13adc_channel_e
//  315 	}
//  316 	printAnalogChannelInfo("AFR", engineConfiguration->afr.hwChannel);
??printAnalogInfo_1:
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+584]
        ADR.N    R0,??DataTable17_1  ;; "AFR"
          CFI FunCall _Z22printAnalogChannelInfoPKc13adc_channel_e
        BL       _Z22printAnalogChannelInfoPKc13adc_channel_e
//  317 	if (engineConfiguration->hasMapSensor) {
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??printAnalogInfo_2
//  318 		printAnalogChannelInfo("MAP", engineConfiguration->map.sensor.hwChannel);
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+252]
        ADR.N    R0,??DataTable17_2  ;; "MAP"
          CFI FunCall _Z22printAnalogChannelInfoPKc13adc_channel_e
        BL       _Z22printAnalogChannelInfoPKc13adc_channel_e
//  319 	}
//  320 	if (hasBaroSensor()) {
??printAnalogInfo_2:
          CFI FunCall _Z13hasBaroSensorv
        BL       _Z13hasBaroSensorv
        CMP      R0,#+0
        BEQ.N    ??printAnalogInfo_3
//  321 		printAnalogChannelInfo("BARO", engineConfiguration->baroSensor.hwChannel);
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+620]
        LDR.W    R0,??DataTable23_32
          CFI FunCall _Z22printAnalogChannelInfoPKc13adc_channel_e
        BL       _Z22printAnalogChannelInfoPKc13adc_channel_e
//  322 	}
//  323 	if (engineConfiguration->externalKnockSenseAdc != EFI_ADC_NONE) {
??printAnalogInfo_3:
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+1620]
        CMP      R0,#+16
        BEQ.N    ??printAnalogInfo_4
//  324 		printAnalogChannelInfo("extKno", engineConfiguration->externalKnockSenseAdc);
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+1620]
        LDR.W    R0,??DataTable23_33
          CFI FunCall _Z22printAnalogChannelInfoPKc13adc_channel_e
        BL       _Z22printAnalogChannelInfoPKc13adc_channel_e
//  325 	}
//  326 
//  327 	printAnalogChannelInfo("A/C sw", engineConfiguration->acSwitchAdc);
??printAnalogInfo_4:
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+1520]
        LDR.W    R0,??DataTable23_34
          CFI FunCall _Z22printAnalogChannelInfoPKc13adc_channel_e
        BL       _Z22printAnalogChannelInfoPKc13adc_channel_e
//  328 	printAnalogChannelInfo("HIP9011", engineConfiguration->hipOutputChannel);
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+1500]
        LDR.W    R0,??DataTable23_35
          CFI FunCall _Z22printAnalogChannelInfoPKc13adc_channel_e
        BL       _Z22printAnalogChannelInfoPKc13adc_channel_e
//  329 	printAnalogChannelInfoExt("Vbatt", engineConfiguration->vbattAdcChannel, getVoltage("vbatt", engineConfiguration->vbattAdcChannel),
//  330 			engineConfiguration->vbattDividerCoeff);
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+472]
        LDR.W    R0,??DataTable23_36
          CFI FunCall _Z19getInternalAdcValuePKc13adc_channel_e
        BL       _Z19getInternalAdcValuePKc13adc_channel_e
        VMOV     S0,R0
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        VLDR     S1,[R0, #+468]
        VCVT.F32.S32 S0,S0
        VLDR.W   S2,??DataTable16  ;; 0x3a400c01
        VMUL.F32 S0,S0,S2
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+472]
        LDR.W    R0,??DataTable23_37
          CFI FunCall _Z25printAnalogChannelInfoExtPKc13adc_channel_eff
        BL       _Z25printAnalogChannelInfoExtPKc13adc_channel_eff
//  331 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock15
//  332 

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//  333 static THD_WORKING_AREA(csThreadStack, UTILITY_THREAD_STACK_SIZE);	// declare thread stack
csThreadStack:
        DS8 712
//  334 
//  335 #define isOutOfBounds(offset) ((offset<0) || (offset) >= (int) sizeof(engine_configuration_s))
//  336 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function _Z8getShorti
        THUMB
//  337 static void getShort(int offset) {
_Z8getShorti:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  338 	if (isOutOfBounds(offset))
        CMP      R4,#+0
        BMI.N    ??getShort_0
        MOVW     R0,#+2216
        CMP      R4,R0
        BLT.N    ??getShort_1
//  339 		return;
??getShort_0:
        B.N      ??getShort_2
//  340 	uint16_t *ptr = (uint16_t *) (&((char *) engineConfiguration)[offset]);
??getShort_1:
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        ADDS     R5,R4,R0
//  341 	uint16_t value = *ptr;
        LDRH     R6,[R5, #+0]
//  342 	/**
//  343 	 * this response is part of dev console API
//  344 	 */
//  345 	scheduleMsg(&logger, "short @%d is %d", offset, value);
        UXTH     R6,R6            ;; ZeroExt  R6,R6,#+16,#+16
        MOVS     R3,R6
        MOVS     R2,R4
        LDR.W    R1,??DataTable23_38
        LDR.W    R0,??DataTable23_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  346 }
??getShort_2:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock16
//  347 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function _Z6setBitPKcS0_S0_
        THUMB
//  348 static void setBit(const char *offsetStr, const char *bitStr, const char *valueStr) {
_Z6setBitPKcS0_S0_:
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
        MOVS     R5,R1
        MOVS     R6,R2
//  349 	int offset = atoi(offsetStr);
        MOVS     R0,R4
          CFI FunCall atoi
        BL       atoi
        MOVS     R7,R0
//  350 	if (absI(offset) == absI(ERROR_CODE)) {
        MOVS     R0,R7
          CFI FunCall absI
        BL       absI
        MOV      R8,R0
        LDR.W    R0,??DataTable23_39  ;; 0x128ce430
          CFI FunCall absI
        BL       absI
        CMP      R8,R0
        BNE.N    ??setBit_0
//  351 		scheduleMsg(&logger, "invalid offset [%s]", offsetStr);
        MOVS     R2,R4
        LDR.W    R1,??DataTable23_40
        LDR.W    R0,??DataTable23_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  352 		return;
        B.N      ??setBit_1
//  353 	}
//  354 	if (isOutOfBounds(offset)) {
??setBit_0:
        CMP      R7,#+0
        BMI.N    ??setBit_2
        MOVW     R0,#+2216
        CMP      R7,R0
        BLT.N    ??setBit_3
//  355 		return;
??setBit_2:
        B.N      ??setBit_1
//  356 	}
//  357 	int bit = atoi(bitStr);
??setBit_3:
        MOVS     R0,R5
          CFI FunCall atoi
        BL       atoi
        MOV      R8,R0
//  358 	if (absI(bit) == absI(ERROR_CODE)) {
        MOV      R0,R8
          CFI FunCall absI
        BL       absI
        MOV      R9,R0
        LDR.W    R0,??DataTable23_39  ;; 0x128ce430
          CFI FunCall absI
        BL       absI
        CMP      R9,R0
        BNE.N    ??setBit_4
//  359 		scheduleMsg(&logger, "invalid bit [%s]", bitStr);
        MOVS     R2,R5
        LDR.W    R1,??DataTable23_41
        LDR.W    R0,??DataTable23_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  360 		return;
        B.N      ??setBit_1
//  361 	}
//  362 	int value = atoi(valueStr);
??setBit_4:
        MOVS     R0,R6
          CFI FunCall atoi
        BL       atoi
        MOV      R9,R0
//  363 	if (absI(value) == absI(ERROR_CODE)) {
        MOV      R0,R9
          CFI FunCall absI
        BL       absI
        MOV      R10,R0
        LDR.W    R0,??DataTable23_39  ;; 0x128ce430
          CFI FunCall absI
        BL       absI
        CMP      R10,R0
        BNE.N    ??setBit_5
//  364 		scheduleMsg(&logger, "invalid value [%s]", valueStr);
        MOVS     R2,R6
        LDR.W    R1,??DataTable23_42
        LDR.W    R0,??DataTable23_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  365 		return;
        B.N      ??setBit_1
//  366 	}
//  367 	int *ptr = (int *) (&((char *) engineConfiguration)[offset]);
??setBit_5:
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        ADDS     R10,R7,R0
//  368 	*ptr ^= (-value ^ *ptr) & (1 << bit);
        LDR      R0,[R10, #+0]
        RSBS     R1,R9,#+0
        LDR      R2,[R10, #+0]
        EORS     R1,R2,R1
        MOVS     R2,#+1
        LSLS     R2,R2,R8
        ANDS     R1,R2,R1
        EORS     R0,R1,R0
        STR      R0,[R10, #+0]
//  369 	/**
//  370 	 * this response is part of dev console API
//  371 	 */
//  372 	scheduleMsg(&logger, "bit @%d/%d is %d", offset, bit, value);
        STR      R9,[SP, #+0]
        MOV      R3,R8
        MOVS     R2,R7
        LDR.W    R1,??DataTable23_43
        LDR.W    R0,??DataTable23_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  373 	applyNewConfiguration();
          CFI FunCall applyNewConfiguration
        BL       applyNewConfiguration
//  374 }
??setBit_1:
        POP      {R0,R1,R4-R10,PC}  ;; return
          CFI EndBlock cfiBlock17
//  375 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function _Z8setShortii
        THUMB
//  376 static void setShort(const int offset, const int value) {
_Z8setShortii:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  377 	if (isOutOfBounds(offset))
        CMP      R4,#+0
        BMI.N    ??setShort_0
        MOVW     R0,#+2216
        CMP      R4,R0
        BLT.N    ??setShort_1
//  378 		return;
??setShort_0:
        B.N      ??setShort_2
//  379 	uint16_t *ptr = (uint16_t *) (&((char *) engineConfiguration)[offset]);
??setShort_1:
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        ADDS     R6,R4,R0
//  380 	*ptr = (uint16_t) value;
        STRH     R5,[R6, #+0]
//  381 	getShort(offset);
        MOVS     R0,R4
          CFI FunCall _Z8getShorti
        BL       _Z8getShorti
//  382 	applyNewConfiguration();
          CFI FunCall applyNewConfiguration
        BL       applyNewConfiguration
//  383 }
??setShort_2:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock18
//  384 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock19 Using cfiCommon0
          CFI Function _Z6getBitii
        THUMB
//  385 static void getBit(int offset, int bit) {
_Z6getBitii:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  386 	if (isOutOfBounds(offset))
        CMP      R4,#+0
        BMI.N    ??getBit_0
        MOVW     R0,#+2216
        CMP      R4,R0
        BLT.N    ??getBit_1
//  387 		return;
??getBit_0:
        B.N      ??getBit_2
//  388 	int *ptr = (int *) (&((char *) engineConfiguration)[offset]);
??getBit_1:
        LDR.W    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        ADDS     R6,R4,R0
//  389 	int value = (*ptr >> bit) & 1;
        LDR      R0,[R6, #+0]
        ASRS     R0,R0,R5
        ANDS     R7,R0,#0x1
//  390 	/**
//  391 	 * this response is part of dev console API
//  392 	 */
//  393 	scheduleMsg(&logger, "bit @%d/%d is %d", offset, bit, value);
        STR      R7,[SP, #+0]
        MOVS     R3,R5
        MOVS     R2,R4
        LDR.W    R1,??DataTable23_43
        LDR.N    R0,??DataTable23_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  394 }
??getBit_2:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock19

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15:
        DC8      "TPS"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_1:
        DC8      "pPS"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_2:
        DC8      "CLT"
//  395 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock20 Using cfiCommon0
          CFI Function _Z6getInti
        THUMB
//  396 static void getInt(int offset) {
_Z6getInti:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  397 	if (isOutOfBounds(offset))
        CMP      R4,#+0
        BMI.N    ??getInt_0
        MOVW     R0,#+2216
        CMP      R4,R0
        BLT.N    ??getInt_1
//  398 		return;
??getInt_0:
        B.N      ??getInt_2
//  399 	int *ptr = (int *) (&((char *) engineConfiguration)[offset]);
??getInt_1:
        LDR.N    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        ADDS     R5,R4,R0
//  400 	int value = *ptr;
        LDR      R6,[R5, #+0]
//  401 	/**
//  402 	 * this response is part of dev console API
//  403 	 */
//  404 	scheduleMsg(&logger, "int @%d is %d", offset, value);
        MOVS     R3,R6
        MOVS     R2,R4
        LDR.W    R1,??DataTable23_44
        LDR.N    R0,??DataTable23_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  405 }
??getInt_2:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock20

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16:
        DC32     0x3a400c01

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_1:
        DC8      "IAT"
//  406 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock21 Using cfiCommon0
          CFI Function _Z6setIntii
        THUMB
//  407 static void setInt(const int offset, const int value) {
_Z6setIntii:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  408 	if (isOutOfBounds(offset))
        CMP      R4,#+0
        BMI.N    ??setInt_0
        MOVW     R0,#+2216
        CMP      R4,R0
        BLT.N    ??setInt_1
//  409 		return;
??setInt_0:
        B.N      ??setInt_2
//  410 	int *ptr = (int *) (&((char *) engineConfiguration)[offset]);
??setInt_1:
        LDR.N    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        ADDS     R6,R4,R0
//  411 	*ptr = value;
        STR      R5,[R6, #+0]
//  412 	getInt(offset);
        MOVS     R0,R4
          CFI FunCall _Z6getInti
        BL       _Z6getInti
//  413 	applyNewConfiguration();
          CFI FunCall applyNewConfiguration
        BL       applyNewConfiguration
//  414 }
??setInt_2:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock21

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17:
        DC8      "MAF"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_1:
        DC8      "AFR"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_2:
        DC8      "MAP"
//  415 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock22 Using cfiCommon0
          CFI Function _Z8getFloati
        THUMB
//  416 static void getFloat(int offset) {
_Z8getFloati:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        VPUSH    {D8}
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        SUB      SP,SP,#+8
          CFI CFA R13+32
        MOVS     R4,R0
//  417 	if (isOutOfBounds(offset))
        CMP      R4,#+0
        BMI.N    ??getFloat_0
        MOVW     R0,#+2216
        CMP      R4,R0
        BLT.N    ??getFloat_1
//  418 		return;
??getFloat_0:
        B.N      ??getFloat_2
//  419 	float *ptr = (float *) (&((char *) engineConfiguration)[offset]);
??getFloat_1:
        LDR.N    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        ADDS     R5,R4,R0
//  420 	float value = *ptr;
        VLDR     S16,[R5, #0]
//  421 	/**
//  422 	 * this response is part of dev console API
//  423 	 */
//  424 	scheduleMsg(&logger, "float @%d is %..100000f", offset, value);
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        MOVS     R2,R4
        LDR.N    R1,??DataTable23_45
        LDR.N    R0,??DataTable23_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  425 }
??getFloat_2:
        ADD      SP,SP,#+8
          CFI CFA R13+24
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock22
//  426 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock23 Using cfiCommon0
          CFI Function _Z8setFloatPKcS0_
        THUMB
//  427 static void setFloat(const char *offsetStr, const char *valueStr) {
_Z8setFloatPKcS0_:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        VPUSH    {D8}
          CFI D8 Frame(CFA, -32)
          CFI CFA R13+32
        MOVS     R4,R0
        MOVS     R5,R1
//  428 	int offset = atoi(offsetStr);
        MOVS     R0,R4
          CFI FunCall atoi
        BL       atoi
        MOVS     R6,R0
//  429 	if (absI(offset) == absI(ERROR_CODE)) {
        MOVS     R0,R6
          CFI FunCall absI
        BL       absI
        MOVS     R7,R0
        LDR.N    R0,??DataTable23_39  ;; 0x128ce430
          CFI FunCall absI
        BL       absI
        CMP      R7,R0
        BNE.N    ??setFloat_0
//  430 		scheduleMsg(&logger, "invalid offset [%s]", offsetStr);
        MOVS     R2,R4
        LDR.N    R1,??DataTable23_40
        LDR.N    R0,??DataTable23_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  431 		return;
        B.N      ??setFloat_1
//  432 	}
//  433 	if (isOutOfBounds(offset))
??setFloat_0:
        CMP      R6,#+0
        BMI.N    ??setFloat_2
        MOVW     R0,#+2216
        CMP      R6,R0
        BLT.N    ??setFloat_3
//  434 		return;
??setFloat_2:
        B.N      ??setFloat_1
//  435 	float value = atoff(valueStr);
??setFloat_3:
        MOVS     R0,R5
          CFI FunCall atoff
        BL       atoff
        VMOV.F32 S16,S0
//  436 	if (cisnan(value)) {
        VMOV.F32 S0,S16
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BNE.N    ??setFloat_4
//  437 		scheduleMsg(&logger, "invalid value [%s]", valueStr);
        MOVS     R2,R5
        LDR.N    R1,??DataTable23_42
        LDR.N    R0,??DataTable23_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  438 		return;
        B.N      ??setFloat_1
//  439 	}
//  440 	float *ptr = (float *) (&((char *) engine->engineConfiguration)[offset]);
??setFloat_4:
        LDR.N    R0,??DataTable23_11
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+36]
        ADDS     R7,R6,R0
//  441 	*ptr = value;
        VSTR     S16,[R7, #0]
//  442 	getFloat(offset);
        MOVS     R0,R6
          CFI FunCall _Z8getFloati
        BL       _Z8getFloati
//  443 }
??setFloat_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+24
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock23
//  444 
//  445 #if EFI_PROD_CODE || defined(__DOXYGEN__)

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock24 Using cfiCommon0
          CFI Function _Z10resetAccelv
        THUMB
//  446 static void resetAccel(void) {
_Z10resetAccelv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  447 	engine->mapAccelEnrichment.reset();
        LDR.N    R0,??DataTable23_11
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+56
          CFI FunCall _ZN16AccelEnrichmemnt5resetEv
        BL       _ZN16AccelEnrichmemnt5resetEv
//  448 	engine->tpsAccelEnrichment.reset();
        LDR.N    R0,??DataTable23_11
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+332
          CFI FunCall _ZN16AccelEnrichmemnt5resetEv
        BL       _ZN16AccelEnrichmemnt5resetEv
//  449 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock24
//  450 #endif
//  451 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock25 Using cfiCommon0
          CFI Function _Z17initConfigActionsv
        THUMB
//  452 void initConfigActions(void) {
_Z17initConfigActionsv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  453 	addConsoleActionSS("set_float", (VoidCharPtrCharPtr) setFloat);
        LDR.N    R1,??DataTable23_46
        LDR.N    R0,??DataTable23_47
          CFI FunCall addConsoleActionSS
        BL       addConsoleActionSS
//  454 	addConsoleActionII("set_int", (VoidIntInt) setInt);
        LDR.N    R1,??DataTable23_48
        LDR.N    R0,??DataTable23_49
          CFI FunCall addConsoleActionII
        BL       addConsoleActionII
//  455 	addConsoleActionII("set_short", (VoidIntInt) setShort);
        LDR.N    R1,??DataTable23_50
        LDR.N    R0,??DataTable23_51
          CFI FunCall addConsoleActionII
        BL       addConsoleActionII
//  456 	addConsoleActionSSS("set_bit", setBit);
        LDR.N    R1,??DataTable23_52
        LDR.N    R0,??DataTable23_53
          CFI FunCall addConsoleActionSSS
        BL       addConsoleActionSSS
//  457 	addConsoleActionI("get_float", getFloat);
        LDR.N    R1,??DataTable23_54
        LDR.N    R0,??DataTable23_55
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  458 	addConsoleActionI("get_int", getInt);
        LDR.N    R1,??DataTable23_56
        LDR.N    R0,??DataTable23_57
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  459 	addConsoleActionI("get_short", getShort);
        LDR.N    R1,??DataTable23_58
        LDR.N    R0,??DataTable23_59
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  460 	addConsoleActionII("get_bit", getBit);
        LDR.N    R1,??DataTable23_60
        LDR.N    R0,??DataTable23_61
          CFI FunCall addConsoleActionII
        BL       addConsoleActionII
//  461 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock25
//  462 
//  463 // todo: move this logic somewhere else?

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock26 Using cfiCommon0
          CFI Function _Z12getKnockInfov
        THUMB
//  464 static void getKnockInfo(void) {
_Z12getKnockInfov:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  465 	adc_channel_e hwChannel = engineConfiguration->externalKnockSenseAdc;
        LDR.N    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDR      R4,[R0, #+1620]
//  466 	scheduleMsg(&logger, "externalKnockSenseAdc on ADC", getPinNameByAdcChannel(hwChannel, pinNameBuffer));
        LDR.N    R1,??DataTable23_26
        MOVS     R0,R4
          CFI FunCall _Z22getPinNameByAdcChannel13adc_channel_ePc
        BL       _Z22getPinNameByAdcChannel13adc_channel_ePc
        MOVS     R2,R0
        LDR.N    R1,??DataTable23_62
        LDR.N    R0,??DataTable23_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  467 
//  468 	engine->printKnockState();
        LDR.N    R0,??DataTable23_11
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN6Engine15printKnockStateEv
        BL       _ZN6Engine15printKnockStateEv
//  469 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock26

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock27 Using cfiCommon0
          CFI Function _Z19initEngineContollerP7Logging
        THUMB
_Z19initEngineContollerP7Logging:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        SUB      SP,SP,#+8
          CFI CFA R13+16
        MOVS     R4,R0
        LDR.N    R1,??DataTable23_63
        LDR.N    R0,??DataTable23_64
          CFI FunCall addConsoleAction
        BL       addConsoleAction
          CFI FunCall _Z17initConfigActionsv
        BL       _Z17initConfigActionsv
        LDR.N    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
          CFI FunCall _Z12initSettingsP22engine_configuration_s
        BL       _Z12initSettingsP22engine_configuration_s
        LDR.N    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??initEngineContoller_0
          CFI FunCall _Z28startTunerStudioConnectivityv
        BL       _Z28startTunerStudioConnectivityv
??initEngineContoller_0:
        LDR.N    R0,??DataTable23_65
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BNE.W    ??initEngineContoller_1
??initEngineContoller_2:
        MOVS     R0,R4
          CFI FunCall _Z11initSensorsP7Logging
        BL       _Z11initSensorsP7Logging
          CFI FunCall _Z16initPwmGeneratorv
        BL       _Z16initPwmGeneratorv
          CFI FunCall _Z15initSensorChartv
        BL       _Z15initSensorChartv
        LDR.N    R0,??DataTable23_15
        LDR      R1,[R0, #+0]
        MOVS     R0,R4
          CFI FunCall _Z8initAlgoP7LoggingP22engine_configuration_s
        BL       _Z8initAlgoP7LoggingP22engine_configuration_s
        LDR.N    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+1
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??initEngineContoller_3
        MOVS     R0,R4
          CFI FunCall _Z16initWaveAnalyzerP7Logging
        BL       _Z16initWaveAnalyzerP7Logging
??initEngineContoller_3:
        LDR.N    R0,??DataTable23_11
        LDR      R1,[R0, #+0]
        MOVS     R0,R4
          CFI FunCall _Z17initRpmCalculatorP7LoggingP6Engine
        BL       _Z17initRpmCalculatorP7LoggingP6Engine
          CFI FunCall _Z18initPeriodicEventsv
        BL       _Z18initPeriodicEventsv
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        LDR.N    R3,??DataTable23_66
        MOVS     R2,#+2
        MOV      R1,#+712
        LDR.N    R0,??DataTable23_67
          CFI FunCall chThdCreateStatic
        BL       chThdCreateStatic
          CFI FunCall _Z19initInjectorCentralv
        BL       _Z19initInjectorCentralv
          CFI FunCall _Z13prepareShapesv
        BL       _Z13prepareShapesv
          CFI FunCall initMalfunctionCentral
        BL       initMalfunctionCentral
        MOVS     R0,R4
          CFI FunCall _Z18initAlternatorCtrlP7Logging
        BL       _Z18initAlternatorCtrlP7Logging
          CFI FunCall _Z22initElectronicThrottlev
        BL       _Z22initElectronicThrottlev
        LDR.N    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+6
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??initEngineContoller_4
          CFI FunCall _Z24initMalfunctionIndicatorv
        BL       _Z24initMalfunctionIndicatorv
??initEngineContoller_4:
        LDR.N    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+5
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??initEngineContoller_5
        LDR.N    R0,??DataTable23_11
        LDR      R1,[R0, #+0]
        MOVS     R0,R4
          CFI FunCall _Z16initMapAveragingP7LoggingP6Engine
        BL       _Z16initMapAveragingP7LoggingP6Engine
??initEngineContoller_5:
        LDR.N    R0,??DataTable23_17
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+5
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??initEngineContoller_6
        LDR.N    R0,??DataTable23_11
        LDR      R1,[R0, #+0]
        MOVS     R0,R4
          CFI FunCall _Z21initMainEventListenerP7LoggingP6Engine
        BL       _Z21initMainEventListenerP7LoggingP6Engine
??initEngineContoller_6:
        LDR.N    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+2
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??initEngineContoller_7
        LDR.N    R0,??DataTable23_11
        LDR      R1,[R0, #+0]
        MOVS     R0,R4
          CFI FunCall _Z15startIdleThreadP7LoggingP6Engine
        BL       _Z15startIdleThreadP7LoggingP6Engine
??initEngineContoller_7:
        LDR.N    R0,??DataTable23_15
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+1620]
        CMP      R0,#+16
        BEQ.N    ??initEngineContoller_8
        LDR.N    R1,??DataTable23_68
        LDR.N    R0,??DataTable23_69
          CFI FunCall addConsoleAction
        BL       addConsoleAction
??initEngineContoller_8:
        MOVS     R0,R4
          CFI FunCall _Z19initAccelEnrichmentP7Logging
        BL       _Z19initAccelEnrichmentP7Logging
          CFI FunCall _Z17initConfigActionsv
        BL       _Z17initConfigActionsv
        LDR.N    R1,??DataTable23_70
        LDR.N    R0,??DataTable23_71
          CFI FunCall addConsoleAction
        BL       addConsoleAction
        LDR.N    R0,??DataTable23_11
        LDR      R1,[R0, #+0]
        MOVS     R0,R4
          CFI FunCall _Z12initFsioImplP7LoggingP6Engine
        BL       _Z12initFsioImplP7LoggingP6Engine
          CFI FunCall _Z17initLcdControllerv
        BL       _Z17initLcdControllerv
??initEngineContoller_1:
        POP      {R0,R1,R4,PC}    ;; return
          CFI EndBlock cfiBlock27

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23:
        DC32     `?<Constant "Engine Controller">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_1:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_2:
        DC32     ec2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_3:
        DC32     persistentState+0x8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_4:
        DC32     _engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_5:
        DC32     __dso_handle

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_6:
        DC32     _ZN6EngineD1Ev

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_7:
        DC32     halTime

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_8:
        DC32     versionForConfigurationListeners

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_9:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_10:
        DC32     `?<Constant "status">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_11:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_12:
        DC32     persistentState

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_13:
        DC32     runningPin

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_14:
        DC32     vtlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_15:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_16:
        DC32     `?<Constant "isCylinderCleanupMode %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_17:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_18:
        DC32     periodicSlowTimer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_19:
        DC32     _Z20periodicFastCallbackv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_20:
        DC32     periodicFastTimer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_21:
        DC32     `?<Constant "lowStckOnEv">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_22:
        DC32     0xe0001004

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_23:
        DC32     `?<Constant "ADC is not assigned f...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_24:
        DC32     fastAdc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_25:
        DC32     `?<Constant "fast enabled=%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_26:
        DC32     pinNameBuffer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_27:
        DC32     `?<Constant "%s ADC%d %s %s rawVal...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_28:
        DC32     `?<Constant "print">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_29:
        DC32     `?<Constant "analogInputDividerCoe...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_30:
        DC32     `?<Constant "hip9011">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_31:
        DC32     `?<Constant "fuel gauge">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_32:
        DC32     `?<Constant "BARO">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_33:
        DC32     `?<Constant "extKno">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_34:
        DC32     `?<Constant "A/C sw">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_35:
        DC32     `?<Constant "HIP9011">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_36:
        DC32     `?<Constant "vbatt">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_37:
        DC32     `?<Constant "Vbatt">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_38:
        DC32     `?<Constant "short @%d is %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_39:
        DC32     0x128ce430

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_40:
        DC32     `?<Constant "invalid offset [%s]">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_41:
        DC32     `?<Constant "invalid bit [%s]">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_42:
        DC32     `?<Constant "invalid value [%s]">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_43:
        DC32     `?<Constant "bit @%d/%d is %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_44:
        DC32     `?<Constant "int @%d is %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_45:
        DC32     `?<Constant "float @%d is %..100000f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_46:
        DC32     _Z8setFloatPKcS0_

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_47:
        DC32     `?<Constant "set_float">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_48:
        DC32     _Z6setIntii

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_49:
        DC32     `?<Constant "set_int">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_50:
        DC32     _Z8setShortii

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_51:
        DC32     `?<Constant "set_short">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_52:
        DC32     _Z6setBitPKcS0_S0_

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_53:
        DC32     `?<Constant "set_bit">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_54:
        DC32     _Z8getFloati

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_55:
        DC32     `?<Constant "get_float">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_56:
        DC32     _Z6getInti

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_57:
        DC32     `?<Constant "get_int">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_58:
        DC32     _Z8getShorti

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_59:
        DC32     `?<Constant "get_short">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_60:
        DC32     _Z6getBitii

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_61:
        DC32     `?<Constant "get_bit">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_62:
        DC32     `?<Constant "externalKnockSenseAdc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_63:
        DC32     _Z15printAnalogInfov

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_64:
        DC32     `?<Constant "analoginfo">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_65:
        DC32     hasFirmwareErrorFlag

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_66:
        DC32     _Z8csThreadv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_67:
        DC32     csThreadStack

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_68:
        DC32     _Z12getKnockInfov

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_69:
        DC32     `?<Constant "knockinfo">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_70:
        DC32     _Z10resetAccelv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_71:
        DC32     `?<Constant "reset_accel">`

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIfED1Ev
          CFI Block cfiBlock28 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIfED1Ev
          CFI NoCalls
        THUMB
// __code __interwork __softfp cyclic_buffer<float>::~cyclic_buffer()
_ZN13cyclic_bufferIfED1Ev:
        BX       LR               ;; return
          CFI EndBlock cfiBlock28

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN16AccelEnrichmemntD1Ev
          CFI Block cfiBlock29 Using cfiCommon0
          CFI Function _ZN16AccelEnrichmemntD1Ev
        THUMB
// __code __interwork __softfp AccelEnrichmemnt::~AccelEnrichmemnt()
_ZN16AccelEnrichmemntD1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        ADDS     R0,R4,#+4
          CFI FunCall _ZN13cyclic_bufferIfED1Ev
        BL       _ZN13cyclic_bufferIfED1Ev
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock29

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN6EngineD1Ev
          CFI Block cfiBlock30 Using cfiCommon0
          CFI Function _ZN6EngineD1Ev
        THUMB
// __code __interwork __softfp Engine::~Engine()
_ZN6EngineD1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        ADDS     R0,R4,#+332
          CFI FunCall _ZN16AccelEnrichmemntD1Ev
        BL       _ZN16AccelEnrichmemntD1Ev
        ADDS     R0,R4,#+56
          CFI FunCall _ZN16AccelEnrichmemntD1Ev
        BL       _ZN16AccelEnrichmemntD1Ev
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock30

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
`?<Constant "status">`:
        DATA
        DC8 "status"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "isCylinderCleanupMode %s">`:
        DATA
        DC8 "isCylinderCleanupMode %s"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lowStckOnEv">`:
        DATA
        DC8 "lowStckOnEv"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ADC is not assigned f...">`:
        DATA
        DC8 "ADC is not assigned for %s"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fast enabled=%s">`:
        DATA
        DC8 "fast enabled=%s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "%s ADC%d %s %s rawVal...">`:
        DATA
        DC8 "%s ADC%d %s %s rawValue=%f/divided=%fv/divider=%f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "print">`:
        DATA
        DC8 "print"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "analogInputDividerCoe...">`:
        DATA
        DC8 "analogInputDividerCoefficient: %f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "hip9011">`:
        DATA
        DC8 "hip9011"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fuel gauge">`:
        DATA
        DC8 "fuel gauge"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "TPS"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "pPS"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "CLT"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "IAT"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "MAF"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "AFR"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "MAP"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "BARO">`:
        DATA
        DC8 "BARO"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "extKno">`:
        DATA
        DC8 "extKno"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "A/C sw">`:
        DATA
        DC8 "A/C sw"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "HIP9011">`:
        DATA
        DC8 "HIP9011"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Vbatt">`:
        DATA
        DC8 "Vbatt"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "vbatt">`:
        DATA
        DC8 "vbatt"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "short @%d is %d">`:
        DATA
        DC8 "short @%d is %d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid offset [%s]">`:
        DATA
        DC8 "invalid offset [%s]"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid bit [%s]">`:
        DATA
        DC8 "invalid bit [%s]"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid value [%s]">`:
        DATA
        DC8 "invalid value [%s]"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "bit @%d/%d is %d">`:
        DATA
        DC8 "bit @%d/%d is %d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "int @%d is %d">`:
        DATA
        DC8 "int @%d is %d"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "float @%d is %..100000f">`:
        DATA
        DC8 "float @%d is %..100000f"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_float">`:
        DATA
        DC8 "set_float"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_int">`:
        DATA
        DC8 "set_int"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_short">`:
        DATA
        DC8 "set_short"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_bit">`:
        DATA
        DC8 "set_bit"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "get_float">`:
        DATA
        DC8 "get_float"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "get_int">`:
        DATA
        DC8 "get_int"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "get_short">`:
        DATA
        DC8 "get_short"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "get_bit">`:
        DATA
        DC8 "get_bit"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "externalKnockSenseAdc...">`:
        DATA
        DC8 "externalKnockSenseAdc on ADC"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "analoginfo">`:
        DATA
        DC8 "analoginfo"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "knockinfo">`:
        DATA
        DC8 "knockinfo"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "reset_accel">`:
        DATA
        DC8 "reset_accel"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Engine Controller">`:
        DATA
        DC8 "Engine Controller"
        DC8 0, 0

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIfED2Ev
          CFI Block cfiBlock31 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIfED2Ev
        THUMB
// __code __interwork __softfp cyclic_buffer<float>::subobject ~cyclic_buffer()
_ZN13cyclic_bufferIfED2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN13cyclic_bufferIfED1Ev
        BL       _ZN13cyclic_bufferIfED1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock31

        END
//  470 
//  471 void initEngineContoller(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_S) {
//  472 	addConsoleAction("analoginfo", printAnalogInfo);
//  473 	initConfigActions();
//  474 #if EFI_PROD_CODE
//  475 	// todo: this is a mess, remove code duplication with simulator
//  476 	initSettings(engineConfiguration);
//  477 #endif
//  478 
//  479 #if EFI_TUNER_STUDIO || defined(__DOXYGEN__)
//  480 	if (engineConfiguration->isTunerStudioEnabled) {
//  481 		startTunerStudioConnectivity();
//  482 	}
//  483 #endif
//  484 
//  485 	if (hasFirmwareError()) {
//  486 		return;
//  487 	}
//  488 
//  489 	initSensors(sharedLogger PASS_ENGINE_PARAMETER_F);
//  490 
//  491 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//  492 	initPwmGenerator();
//  493 #endif
//  494 
//  495 #if EFI_ANALOG_CHART || defined(__DOXYGEN__)
//  496 	initSensorChart();
//  497 #endif /* EFI_ANALOG_CHART */
//  498 
//  499 	initAlgo(sharedLogger, engineConfiguration);
//  500 
//  501 #if EFI_WAVE_ANALYZER || defined(__DOXYGEN__)
//  502 	if (engineConfiguration->isWaveAnalyzerEnabled) {
//  503 		initWaveAnalyzer(sharedLogger);
//  504 	}
//  505 #endif /* EFI_WAVE_ANALYZER */
//  506 
//  507 #if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
//  508 	/**
//  509 	 * there is an implicit dependency on the fact that 'tachometer' listener is the 1st listener - this case
//  510 	 * other listeners can access current RPM value
//  511 	 */
//  512 	initRpmCalculator(sharedLogger, engine);
//  513 #endif /* EFI_SHAFT_POSITION_INPUT */
//  514 
//  515 // multiple issues with this	initMapAdjusterThread();
//  516 	initPeriodicEvents(PASS_ENGINE_PARAMETER_F);
//  517 
//  518 	chThdCreateStatic(csThreadStack, sizeof(csThreadStack), LOWPRIO, (tfunc_t) csThread, NULL);
//  519 
//  520 #if (EFI_PROD_CODE && EFI_ENGINE_CONTROL) || defined(__DOXYGEN__)
//  521 	initInjectorCentral();
//  522 	/**
//  523 	 * This has to go after 'initInjectorCentral' and 'initInjectorCentral' in order to
//  524 	 * properly detect un-assigned output pins
//  525 	 */
//  526 	prepareShapes(PASS_ENGINE_PARAMETER_F);
//  527 #endif
//  528 
//  529 #if EFI_PWM_TESTER || defined(__DOXYGEN__)
//  530 	initPwmTester();
//  531 #endif
//  532 
//  533 	initMalfunctionCentral();
//  534 
//  535 #if EFI_ALTERNATOR_CONTROL || defined(__DOXYGEN__)
//  536 	initAlternatorCtrl(sharedLogger);
//  537 #endif
//  538 
//  539 #if EFI_ELECTRONIC_THROTTLE_BODY || defined(__DOXYGEN__)
//  540 	initElectronicThrottle();
//  541 #endif /* EFI_ELECTRONIC_THROTTLE_BODY */
//  542 
//  543 #if EFI_MALFUNCTION_INDICATOR || defined(__DOXYGEN__)
//  544 	if (engineConfiguration->isMilEnabled) {
//  545 		initMalfunctionIndicator();
//  546 	}
//  547 #endif /* EFI_MALFUNCTION_INDICATOR */
//  548 
//  549 #if EFI_MAP_AVERAGING || defined(__DOXYGEN__)
//  550 	if (engineConfiguration->isMapAveragingEnabled) {
//  551 		initMapAveraging(sharedLogger, engine);
//  552 	}
//  553 #endif /* EFI_MAP_AVERAGING */
//  554 
//  555 #if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
//  556 	if (boardConfiguration->isEngineControlEnabled) {
//  557 		/**
//  558 		 * This method initialized the main listener which actually runs injectors & ignition
//  559 		 */
//  560 		initMainEventListener(sharedLogger, engine);
//  561 	}
//  562 #endif /* EFI_ENGINE_CONTROL */
//  563 
//  564 #if EFI_IDLE_CONTROL || defined(__DOXYGEN__)
//  565 	if (engineConfiguration->isIdleThreadEnabled) {
//  566 		startIdleThread(sharedLogger, engine);
//  567 	}
//  568 #endif
//  569 
//  570 	if (engineConfiguration->externalKnockSenseAdc != EFI_ADC_NONE) {
//  571 		addConsoleAction("knockinfo", getKnockInfo);
//  572 	}
//  573 
//  574 	initAccelEnrichment(sharedLogger);
//  575 
//  576 	initConfigActions();
//  577 #if EFI_PROD_CODE
//  578 	addConsoleAction("reset_accel", resetAccel);
//  579 #endif
//  580 
//  581 #if (EFI_PROD_CODE && EFI_FSIO) || defined(__DOXYGEN__)
//  582 	initFsioImpl(sharedLogger, engine);
//  583 #endif
//  584 
//  585 #if EFI_HD44780_LCD || defined(__DOXYGEN__)
//  586 	initLcdController();
//  587 #endif
//  588 }
// 
// 10 288 bytes in section .bss
// 42 788 bytes in section .ccm
//     20 bytes in section .data
//      4 bytes in section .init_array
//    588 bytes in section .rodata
//  2 876 bytes in section .text
// 
//  2 828 bytes of CODE  memory (+ 52 bytes shared)
//    588 bytes of CONST memory
// 53 096 bytes of DATA  memory
//
//Errors: none
//Warnings: 3
