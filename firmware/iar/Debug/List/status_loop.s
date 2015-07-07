///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:53 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\console\status_ /
//                       loop.cpp                                             /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\console\status_ /
//                       loop.cpp -lCN F:\stuff\rusefi_sourceforge\firmware\i /
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
//                       status_loop.s                                        /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME status_loop

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z10getRealMafv
        EXTERN _Z10getTChargeifff
        EXTERN _Z10hwPortname11brain_pin_e
        EXTERN _Z11getBaseFueli
        EXTERN _Z11getEgtValuei
        EXTERN _Z12boolToStringb
        EXTERN _Z12hasMafSensorv
        EXTERN _Z13hasBaroSensorv
        EXTERN _Z13interpolate3dfPfifS_iPS_
        EXTERN _Z13pokeAdcInputsv
        EXTERN _Z14getEngineLoadTv
        EXTERN _Z14getRunningFuelfi
        EXTERN _Z14getTPS10bitAdcv
        EXTERN _Z14isConsoleReadyv
        EXTERN _Z15getBaroPressurev
        EXTERN _Z15getCrankingFuelv
        EXTERN _Z15getVehicleSpeedv
        EXTERN _Z16getBaseTableFuelP22engine_configuration_sif
        EXTERN _Z16getPedalPositionv
        EXTERN _Z16updateHD44780lcdP6Engine
        EXTERN _Z17outputPinRegisterPKcP9OutputPinP12GPIO_TypeDefj
        EXTERN _Z18getinjectionOffseti
        EXTERN _Z20getConfigurationName13engine_type_e
        EXTERN _Z20getCrankEventCounterv
        EXTERN _Z20getInjectorDutyCyclei
        EXTERN _Z21getCoolantTemperaturev
        EXTERN _Z21getEngine_load_mode_e18engine_load_mode_e
        EXTERN _Z21isIgnitionTimingErrorv
        EXTERN _Z21isTriggerDecoderErrorv
        EXTERN _Z23getIntakeAirTemperaturev
        EXTERN _Z25isValidCoolantTemperaturef
        EXTERN _Z27getNeedToWriteConfigurationv
        EXTERN _Z27isValidIntakeAirTemperaturef
        EXTERN _Z6getAfrv
        EXTERN _Z6getMafv
        EXTERN _Z6getMapv
        EXTERN _Z6getTPSv
        EXTERN _Z6sdMathP22engine_configuration_sffff
        EXTERN _Z8getVBattv
        EXTERN _Z8hasVBattv
        EXTERN _Z9getFuelMsi
        EXTERN _Z9printWaveP7Logging
        EXTERN _ZN13RpmCalculator18getRpmAccelerationEv
        EXTERN _ZN13RpmCalculator3rpmEv
        EXTERN _ZN13RpmCalculator9isRunningEv
        EXTERN _ZN16AccelEnrichmemnt16getMapEnrichmentEv
        EXTERN _ZN16AccelEnrichmemnt16getTpsEnrichmentEv
        EXTERN _ZN16AccelEnrichmemnt8getDeltaEv
        EXTERN _ZN18LoggingWithStorageC1EPKc
        EXTERN _ZN7LoggingC1EPKcPci
        EXTERN _ZN9OutputPin13getLogicValueEv
        EXTERN _ZN9OutputPin8setValueEi
        EXTERN _ZN9OutputPinC1Ev
        EXTERN __aeabi_f2d
        EXTERN _engine
        EXTERN addConsoleAction
        EXTERN addConsoleActionFF
        EXTERN addConsoleActionI
        EXTERN append
        EXTERN appendFloat
        EXTERN appendPrintf
        EXTERN appendToLog
        EXTERN boardConfiguration
        EXTERN chThdCreateStatic
        EXTERN chThdSleep
        EXTERN currentTimeMillis
        EXTERN debugFloat
        EXTERN debugInt
        EXTERN engine
        EXTERN engineConfiguration
        EXTERN enginePins
        EXTERN errorMessageBuffer
        EXTERN firmwareError
        EXTERN getRusEfiVersion
        EXTERN getTimeNowNt
        EXTERN getTimeNowSeconds
        EXTERN hasErrorCodes
        EXTERN hasFirmwareErrorFlag
        EXTERN isSdCardAlive
        EXTERN lastDecodingErrorTime
        EXTERN main_loop_started
        EXTERN print
        EXTERN printMsg
        EXTERN printPending
        EXTERN resetLogging
        EXTERN rlist
        EXTERN scheduleLogging
        EXTERN scheduleMsg
        EXTERN triggerCentral
        EXTERN tsOutputChannels
        EXTERN veMap

        PUBLIC _Z10getFullLogv
        PUBLIC _Z10setFullLogi
        PUBLIC _Z12writeLogLinev
        PUBLIC _Z14initStatusLoopP6Engine
        PUBLIC _Z18startStatusThreadsP6Engine
        PUBLIC _Z21updateDevConsoleStateP6Engine
        PUBLIC _Z22updateTunerStudioStateP25TunerStudioOutputChannels
        PUBLIC _Z25prepareTunerStudioOutputsv
        PUBLIC _ZN5Map3DILi16ELi16EE8getValueEff
        PUBLIC _ZZN5Map3DILi16ELi16EE8getValueEffEs
        PUBLIC blinkingPeriod
        PUBLIC checkEnginePin
        PUBLIC needToReportStatus
        PUBLIC runningPin
        PUBLIC warningEnabled
        PUBLIC warningPin
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\console\status_loop.cpp
//    1 /**
//    2  * @file	status_loop.cpp
//    3  * @brief Human-readable protocol status messages
//    4  *
//    5  * http://rusefi.com/forum/viewtopic.php?t=263 Dev console overview
//    6  * http://rusefi.com/forum/viewtopic.php?t=210 Commands overview
//    7  *
//    8  *
//    9  * @date Mar 15, 2013
//   10  * @author Andrey Belomutskiy, (c) 2012-2015
//   11  *
//   12  * This file is part of rusEfi - see http://rusefi.com
//   13  *
//   14  * rusEfi is free software; you can redistribute it and/or modify it under the terms of
//   15  * the GNU General Public License as published by the Free Software Foundation; either
//   16  * version 3 of the License, or (at your option) any later version.
//   17  *
//   18  * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
//   19  * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   20  * GNU General Public License for more details.
//   21  *
//   22  * You should have received a copy of the GNU General Public License along with this program.
//   23  * If not, see <http://www.gnu.org/licenses/>.
//   24  *
//   25  */
//   26 
//   27 #include "main.h"
//   28 #include "status_loop.h"
//   29 
//   30 #include "adc_inputs.h"
//   31 #if EFI_WAVE_ANALYZER || defined(__DOXYGEN__)
//   32 #include "wave_analyzer.h"
//   33 #endif
//   34 
//   35 #include "trigger_central.h"
//   36 #include "engine_state.h"
//   37 #include "io_pins.h"
//   38 #include "efiGpio.h"
//   39 #include "mmc_card.h"
//   40 #include "console_io.h"
//   41 #include "malfunction_central.h"
//   42 #include "speed_density.h"
//   43 
//   44 #include "advance_map.h"
//   45 #include "tunerstudio.h"
//   46 #include "fuel_math.h"
//   47 #include "main_trigger_callback.h"
//   48 #include "engine_math.h"
//   49 #include "idle_thread.h"
//   50 #include "engine_configuration.h"
//   51 #include "rfiutil.h"
//   52 #include "svnversion.h"
//   53 #include "engine.h"
//   54 #include "lcd_controller.h"
//   55 #include "settings.h"
//   56 #include "rusefi_outputs.h"
//   57 
//   58 extern bool_t main_loop_started;
//   59 
//   60 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//   61 // todo: move this logic to algo folder!
//   62 #include "rtc_helper.h"
//   63 #include "lcd_HD44780.h"
//   64 #include "rusefi.h"
//   65 #include "pin_repository.h"
//   66 #include "flash_main.h"
//   67 #include "max31855.h"
//   68 #include "vehicle_speed.h"
//   69 #endif
//   70 
//   71 extern engine_pins_s enginePins;
//   72 extern TriggerCentral triggerCentral;
//   73 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   74 static bool_t subscription[(int) RO_LAST_ELEMENT];
subscription:
        DS8 16
//   75 
//   76 // this 'true' value is needed for simulator

        SECTION `.data`:DATA:REORDER:NOROOT(0)
//   77 static volatile bool fullLog = true;
fullLog:
        DATA
        DC8 1

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   78 int warningEnabled = true;
warningEnabled:
        DATA
        DC32 1
//   79 //int warningEnabled = FALSE;
//   80 
//   81 extern bool hasFirmwareErrorFlag;
//   82 #define FULL_LOGGING_KEY "fl"
//   83 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   84 static char LOGGING_BUFFER[1400];
LOGGING_BUFFER:
        DS8 1400

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   85 static Logging logger("status loop", LOGGING_BUFFER, sizeof(LOGGING_BUFFER));
        MOV      R3,#+1400
        LDR.W    R2,??DataTable17
        LDR.W    R1,??DataTable17_1
        LDR.W    R0,??DataTable18_1
          CFI FunCall _ZN7LoggingC1EPKcPci
        BL       _ZN7LoggingC1EPKcPci
        LDR.W    R1,??DataTable18_2
        LDR.W    R0,??DataTable18_3
          CFI FunCall _ZN18LoggingWithStorageC1EPKc
        BL       _ZN18LoggingWithStorageC1EPKc
        LDR.W    R1,??DataTable18_4
        LDR.W    R0,??DataTable18_5
          CFI FunCall _ZN18LoggingWithStorageC1EPKc
        BL       _ZN18LoggingWithStorageC1EPKc
        LDR.W    R0,??DataTable18_6
          CFI FunCall _ZN9OutputPinC1Ev
        BL       _ZN9OutputPinC1Ev
        LDR.W    R0,??DataTable18_7
          CFI FunCall _ZN9OutputPinC1Ev
        BL       _ZN9OutputPinC1Ev
        LDR.W    R0,??DataTable18_8
          CFI FunCall _ZN9OutputPinC1Ev
        BL       _ZN9OutputPinC1Ev
        LDR.W    R0,??DataTable21
          CFI FunCall _ZN9OutputPinC1Ev
        BL       _ZN9OutputPinC1Ev
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
logger:
        DS8 60
//   86 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z17setWarningEnabledi
          CFI NoCalls
        THUMB
//   87 static void setWarningEnabled(int value) {
//   88 	warningEnabled = value;
_Z17setWarningEnabledi:
        LDR.W    R1,??DataTable22
        STR      R0,[R1, #+0]
//   89 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   90 
//   91 #if EFI_FILE_LOGGING || defined(__DOXYGEN__)

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   92 static LoggingWithStorage fileLogger("file logger");
fileLogger:
        DS8 260
//   93 #endif /* EFI_FILE_LOGGING */
//   94 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   95 static int logFileLineIndex = 0;
logFileLineIndex:
        DS8 4
//   96 #define TAB "\t"
//   97 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z13reportSensorFP7LoggingbPKcS2_fi
        THUMB
//   98 static void reportSensorF(Logging *log, bool fileFormat, const char *caption, const char *units, float value,
//   99 		int precision) {
_Z13reportSensorFP7LoggingbPKcS2_fi:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        VPUSH    {D8}
          CFI D8 Frame(CFA, -32)
          CFI CFA R13+32
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
        VMOV.F32 S16,S0
        LDR      R8,[SP, #+32]
//  100 	if (!fileFormat) {
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BNE.N    ??reportSensorF_0
//  101 #if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
//  102 		debugFloat(log, caption, value, precision);
        MOV      R2,R8
        VMOV.F32 S0,S16
        MOVS     R1,R6
        MOVS     R0,R4
          CFI FunCall debugFloat
        BL       debugFloat
        B.N      ??reportSensorF_1
//  103 #endif /* EFI_PROD_CODE || EFI_SIMULATOR */
//  104 	} else {
//  105 
//  106 #if EFI_FILE_LOGGING || defined(__DOXYGEN__)
//  107 		if (logFileLineIndex == 0) {
??reportSensorF_0:
        LDR.W    R0,??DataTable18_9
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??reportSensorF_2
//  108 			append(log, caption);
        MOVS     R1,R6
        MOVS     R0,R4
          CFI FunCall append
        BL       append
//  109 			append(log, TAB);
        ADR.N    R1,??DataTable3  ;; "\t"
        MOVS     R0,R4
          CFI FunCall append
        BL       append
        B.N      ??reportSensorF_1
//  110 		} else if (logFileLineIndex == 1) {
??reportSensorF_2:
        LDR.W    R0,??DataTable18_9
        LDR      R0,[R0, #+0]
        CMP      R0,#+1
        BNE.N    ??reportSensorF_3
//  111 			append(log, units);
        MOVS     R1,R7
        MOVS     R0,R4
          CFI FunCall append
        BL       append
//  112 			append(log, TAB);
        ADR.N    R1,??DataTable3  ;; "\t"
        MOVS     R0,R4
          CFI FunCall append
        BL       append
        B.N      ??reportSensorF_1
//  113 		} else {
//  114 			appendFloat(log, value, precision);
??reportSensorF_3:
        MOV      R1,R8
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall appendFloat
        BL       appendFloat
//  115 			append(log, TAB);
        ADR.N    R1,??DataTable3  ;; "\t"
        MOVS     R0,R4
          CFI FunCall append
        BL       append
//  116 		}
//  117 #endif /* EFI_FILE_LOGGING */
//  118 	}
//  119 }
??reportSensorF_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+24
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock2
//  120 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z13reportSensorIP7LoggingbPKcS2_i
        THUMB
//  121 static void reportSensorI(Logging *log, bool fileFormat, const char *caption, const char *units, int value) {
_Z13reportSensorIP7LoggingbPKcS2_i:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
        LDR      R8,[SP, #+24]
//  122 	if (!fileFormat) {
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BNE.N    ??reportSensorI_0
//  123 
//  124 #if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
//  125 		debugInt(log, caption, value);
        MOV      R2,R8
        MOVS     R1,R6
        MOVS     R0,R4
          CFI FunCall debugInt
        BL       debugInt
        B.N      ??reportSensorI_1
//  126 #endif /* EFI_PROD_CODE || EFI_SIMULATOR */
//  127 	} else {
//  128 #if EFI_FILE_LOGGING || defined(__DOXYGEN__)
//  129 		if (logFileLineIndex == 0) {
??reportSensorI_0:
        LDR.W    R0,??DataTable18_9
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??reportSensorI_2
//  130 			append(log, caption);
        MOVS     R1,R6
        MOVS     R0,R4
          CFI FunCall append
        BL       append
//  131 			append(log, TAB);
        ADR.N    R1,??DataTable3  ;; "\t"
        MOVS     R0,R4
          CFI FunCall append
        BL       append
        B.N      ??reportSensorI_1
//  132 		} else if (logFileLineIndex == 1) {
??reportSensorI_2:
        LDR.W    R0,??DataTable18_9
        LDR      R0,[R0, #+0]
        CMP      R0,#+1
        BNE.N    ??reportSensorI_3
//  133 			append(log, units);
        MOVS     R1,R7
        MOVS     R0,R4
          CFI FunCall append
        BL       append
//  134 			append(log, TAB);
        ADR.N    R1,??DataTable3  ;; "\t"
        MOVS     R0,R4
          CFI FunCall append
        BL       append
        B.N      ??reportSensorI_1
//  135 		} else {
//  136 			appendPrintf(log, "%d%s", value, TAB);
??reportSensorI_3:
        ADR.N    R3,??DataTable3  ;; "\t"
        MOV      R2,R8
        LDR.W    R1,??DataTable24
        MOVS     R0,R4
          CFI FunCall appendPrintf
        BL       appendPrintf
//  137 		}
//  138 #endif /* EFI_FILE_LOGGING */
//  139 	}
//  140 }
??reportSensorI_1:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3:
        DC8      "\t",0x0,0x0
//  141 
//  142 EXTERN_ENGINE
//  143 ;
//  144 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z12printSensorsP7Loggingb
        THUMB
//  145 static void printSensors(Logging *log, bool fileFormat) {
_Z12printSensorsP7Loggingb:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        VPUSH    {D8}
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        SUB      SP,SP,#+8
          CFI CFA R13+32
        MOVS     R4,R0
        MOVS     R5,R1
//  146 	// current time, in milliseconds
//  147 	int nowMs = currentTimeMillis();
          CFI FunCall currentTimeMillis
        BL       currentTimeMillis
        VMOV     S16,R0
//  148 	float sec = ((float) nowMs) / 1000;
        VCVT.F32.S32 S0,S16
        VLDR.W   S1,??DataTable4  ;; 0x447a0000
        VDIV.F32 S17,S0,S1
//  149 	reportSensorF(log, fileFormat, "time", "", sec, 3);
        MOVS     R0,#+3
        STR      R0,[SP, #+0]
        VMOV.F32 S0,S17
        ADR.N    R3,??DataTable5  ;; ""
        LDR.W    R2,??DataTable18_10
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall _Z13reportSensorFP7LoggingbPKcS2_fi
        BL       _Z13reportSensorFP7LoggingbPKcS2_fi
//  150 
//  151 	int rpm = 0;
        MOVS     R6,#+0
//  152 #if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
//  153 	rpm = getRpmE(engine);
        LDR.W    R0,??DataTable18_11
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN13RpmCalculator3rpmEv
        BL       _ZN13RpmCalculator3rpmEv
        MOVS     R6,R0
//  154 	reportSensorI(log, fileFormat, "rpm", "RPM", rpm);
        STR      R6,[SP, #+0]
        ADR.N    R3,??DataTable5_1  ;; "RPM"
        ADR.N    R2,??DataTable5_2  ;; "rpm"
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall _Z13reportSensorIP7LoggingbPKcS2_i
        BL       _Z13reportSensorIP7LoggingbPKcS2_i
//  155 
//  156 //	reportSensorF(log, fileFormat, "TRG_0_DUTY", "%", getTriggerDutyCycle(0), 2);
//  157 //	reportSensorF(log, fileFormat, "TRG_1_DUTY", "%", getTriggerDutyCycle(1), 2);
//  158 #endif
//  159 
//  160 	if (hasMafSensor()) {
          CFI FunCall _Z12hasMafSensorv
        BL       _Z12hasMafSensorv
        CMP      R0,#+0
        BEQ.N    ??printSensors_0
//  161 		reportSensorF(log, fileFormat, "maf", "V", getMaf(), 2);
          CFI FunCall _Z6getMafv
        BL       _Z6getMafv
        MOVS     R0,#+2
        STR      R0,[SP, #+0]
        ADR.N    R3,??DataTable5_3  ;; "V"
        ADR.N    R2,??DataTable5_4  ;; "maf"
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall _Z13reportSensorFP7LoggingbPKcS2_fi
        BL       _Z13reportSensorFP7LoggingbPKcS2_fi
//  162 		reportSensorF(log, fileFormat, "mafr", "kg/hr", getRealMaf(), 2);
          CFI FunCall _Z10getRealMafv
        BL       _Z10getRealMafv
        MOVS     R0,#+2
        STR      R0,[SP, #+0]
        LDR.W    R3,??DataTable18_12
        LDR.W    R2,??DataTable18_13
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall _Z13reportSensorFP7LoggingbPKcS2_fi
        BL       _Z13reportSensorFP7LoggingbPKcS2_fi
//  163 	}
//  164 
//  165 	reportSensorF(log, fileFormat, "ENGINE_LOAD", "x", getEngineLoadT(), 2);
??printSensors_0:
          CFI FunCall _Z14getEngineLoadTv
        BL       _Z14getEngineLoadTv
        MOVS     R0,#+2
        STR      R0,[SP, #+0]
        ADR.N    R3,??DataTable5_5  ;; "x"
        LDR.W    R2,??DataTable18_14
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall _Z13reportSensorFP7LoggingbPKcS2_fi
        BL       _Z13reportSensorFP7LoggingbPKcS2_fi
//  166 
//  167 
//  168 #if EFI_ANALOG_SENSORS || defined(__DOXYGEN__)
//  169 	if (engineConfiguration->hasMapSensor) {
        LDR.W    R0,??DataTable18_15
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??printSensors_1
//  170 		reportSensorF(log, fileFormat, "MAP", "kPa", getMap(), 2);
          CFI FunCall _Z6getMapv
        BL       _Z6getMapv
        MOVS     R0,#+2
        STR      R0,[SP, #+0]
        ADR.N    R3,??DataTable5_6  ;; "kPa"
        ADR.N    R2,??DataTable5_7  ;; "MAP"
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall _Z13reportSensorFP7LoggingbPKcS2_fi
        BL       _Z13reportSensorFP7LoggingbPKcS2_fi
//  171 //		reportSensorF(log, fileFormat, "map_r", "V", getRawMap(), 2);
//  172 	}
//  173 	if (hasBaroSensor()) {
??printSensors_1:
          CFI FunCall _Z13hasBaroSensorv
        BL       _Z13hasBaroSensorv
        CMP      R0,#+0
        BEQ.N    ??printSensors_2
//  174 		reportSensorF(log, fileFormat, "baro", "kPa", getBaroPressure(), 2);
          CFI FunCall _Z15getBaroPressurev
        BL       _Z15getBaroPressurev
        MOVS     R0,#+2
        STR      R0,[SP, #+0]
        ADR.N    R3,??DataTable5_6  ;; "kPa"
        LDR.W    R2,??DataTable18_16
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall _Z13reportSensorFP7LoggingbPKcS2_fi
        BL       _Z13reportSensorFP7LoggingbPKcS2_fi
//  175 	}
//  176 	if (engineConfiguration->hasAfrSensor) {
??printSensors_2:
        LDR.W    R0,??DataTable18_15
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+3
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??printSensors_3
//  177 		reportSensorF(log, fileFormat, "afr", "AFR", getAfr(), 2);
          CFI FunCall _Z6getAfrv
        BL       _Z6getAfrv
        MOVS     R0,#+2
        STR      R0,[SP, #+0]
        ADR.N    R3,??DataTable5_8  ;; "AFR"
        ADR.N    R2,??DataTable5_9  ;; "afr"
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall _Z13reportSensorFP7LoggingbPKcS2_fi
        BL       _Z13reportSensorFP7LoggingbPKcS2_fi
//  178 	}
//  179 #endif
//  180 
//  181 #if EFI_VEHICLE_SPEED || defined(__DOXYGEN__)
//  182 	if (engineConfiguration->hasVehicleSpeedSensor) {
??printSensors_3:
        LDR.W    R0,??DataTable18_15
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+2
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??printSensors_4
//  183 		reportSensorF(log, fileFormat, "vss", "kph", getVehicleSpeed(), 2);
          CFI FunCall _Z15getVehicleSpeedv
        BL       _Z15getVehicleSpeedv
        MOVS     R0,#+2
        STR      R0,[SP, #+0]
        ADR.N    R3,??DataTable5_10  ;; "kph"
        ADR.N    R2,??DataTable5_11  ;; "vss"
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall _Z13reportSensorFP7LoggingbPKcS2_fi
        BL       _Z13reportSensorFP7LoggingbPKcS2_fi
//  184 	}
//  185 #endif /* EFI_PROD_CODE */
//  186 
//  187 	reportSensorF(log, fileFormat, "ks", "count", engine->knockCount, 0);
??printSensors_4:
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        LDR.W    R0,??DataTable18_11
        LDR      R0,[R0, #+0]
        VLDR     S0,[R0, #+620]
        VCVT.F32.S32 S0,S0
        LDR.W    R3,??DataTable18_17
        ADR.N    R2,??DataTable7  ;; 0x6B, 0x73, 0x00, 0x00
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall _Z13reportSensorFP7LoggingbPKcS2_fi
        BL       _Z13reportSensorFP7LoggingbPKcS2_fi
//  188 
//  189 
//  190 //	reportSensorF(log, fileFormat, "vref", "V", getVRef(engineConfiguration), 2);
//  191 	if (hasVBatt(PASS_ENGINE_PARAMETER_F)) {
          CFI FunCall _Z8hasVBattv
        BL       _Z8hasVBattv
        CMP      R0,#+0
        BEQ.N    ??printSensors_5
//  192 		reportSensorF(log, fileFormat, "vbatt", "V", getVBatt(PASS_ENGINE_PARAMETER_F), 2);
          CFI FunCall _Z8getVBattv
        BL       _Z8getVBattv
        MOVS     R0,#+2
        STR      R0,[SP, #+0]
        ADR.N    R3,??DataTable5_3  ;; "V"
        LDR.W    R2,??DataTable18_18
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall _Z13reportSensorFP7LoggingbPKcS2_fi
        BL       _Z13reportSensorFP7LoggingbPKcS2_fi
//  193 	}
//  194 
//  195 	reportSensorF(log, fileFormat, "TP", "%", getTPS(PASS_ENGINE_PARAMETER_F), 2);
??printSensors_5:
          CFI FunCall _Z6getTPSv
        BL       _Z6getTPSv
        MOVS     R0,#+2
        STR      R0,[SP, #+0]
        ADR.N    R3,??DataTable8  ;; "%"
        ADR.N    R2,??DataTable8_1  ;; 0x54, 0x50, 0x00, 0x00
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall _Z13reportSensorFP7LoggingbPKcS2_fi
        BL       _Z13reportSensorFP7LoggingbPKcS2_fi
//  196 
//  197 	if (fileFormat) {
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BEQ.N    ??printSensors_6
//  198 		reportSensorF(log, fileFormat, "tpsacc", "ms", engine->tpsAccelEnrichment.getTpsEnrichment(PASS_ENGINE_PARAMETER_F), 2);
        LDR.W    R0,??DataTable18_11
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+332
          CFI FunCall _ZN16AccelEnrichmemnt16getTpsEnrichmentEv
        BL       _ZN16AccelEnrichmemnt16getTpsEnrichmentEv
        MOVS     R0,#+2
        STR      R0,[SP, #+0]
        ADR.N    R3,??DataTable8_2  ;; 0x6D, 0x73, 0x00, 0x00
        LDR.W    R2,??DataTable22_2
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall _Z13reportSensorFP7LoggingbPKcS2_fi
        BL       _Z13reportSensorFP7LoggingbPKcS2_fi
//  199 		reportSensorF(log, fileFormat, "advance", "deg", engine->tpsAccelEnrichment.getTpsEnrichment(PASS_ENGINE_PARAMETER_F), 2);
        LDR.W    R0,??DataTable18_11
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+332
          CFI FunCall _ZN16AccelEnrichmemnt16getTpsEnrichmentEv
        BL       _ZN16AccelEnrichmemnt16getTpsEnrichmentEv
        MOVS     R0,#+2
        STR      R0,[SP, #+0]
        ADR.N    R3,??DataTable8_3  ;; "deg"
        LDR.W    R2,??DataTable22_3
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall _Z13reportSensorFP7LoggingbPKcS2_fi
        BL       _Z13reportSensorFP7LoggingbPKcS2_fi
//  200 		reportSensorF(log, fileFormat, "advance", "deg", getFuelMs(rpm PASS_ENGINE_PARAMETER), 2);
        MOVS     R0,R6
          CFI FunCall _Z9getFuelMsi
        BL       _Z9getFuelMsi
        MOVS     R0,#+2
        STR      R0,[SP, #+0]
        ADR.N    R3,??DataTable8_3  ;; "deg"
        LDR.W    R2,??DataTable22_3
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall _Z13reportSensorFP7LoggingbPKcS2_fi
        BL       _Z13reportSensorFP7LoggingbPKcS2_fi
//  201 	}
//  202 
//  203 	if (engineConfiguration->hasCltSensor) {
??printSensors_6:
        LDR.W    R0,??DataTable18_15
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+7
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+0
        BEQ.N    ??printSensors_7
//  204 		reportSensorF(log, fileFormat, "CLT", "C", getCoolantTemperature(PASS_ENGINE_PARAMETER_F), 2);
          CFI FunCall _Z21getCoolantTemperaturev
        BL       _Z21getCoolantTemperaturev
        MOVS     R0,#+2
        STR      R0,[SP, #+0]
        ADR.N    R3,??DataTable8_4  ;; "C"
        ADR.N    R2,??DataTable8_5  ;; "CLT"
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall _Z13reportSensorFP7LoggingbPKcS2_fi
        BL       _Z13reportSensorFP7LoggingbPKcS2_fi
//  205 	}
//  206 
//  207 	reportSensorF(log, fileFormat, "MAT", "C", getIntakeAirTemperature(PASS_ENGINE_PARAMETER_F), 2);
??printSensors_7:
          CFI FunCall _Z23getIntakeAirTemperaturev
        BL       _Z23getIntakeAirTemperaturev
        MOVS     R0,#+2
        STR      R0,[SP, #+0]
        ADR.N    R3,??DataTable8_4  ;; "C"
        ADR.N    R2,??DataTable10  ;; "MAT"
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R4
          CFI FunCall _Z13reportSensorFP7LoggingbPKcS2_fi
        BL       _Z13reportSensorFP7LoggingbPKcS2_fi
//  208 
//  209 //	debugFloat(&logger, "tch", getTCharge1(tps), 2);
//  210 
//  211 }
        ADD      SP,SP,#+8
          CFI CFA R13+24
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4:
        DC32     0x447a0000
//  212 
//  213 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z12writeLogLinev
        THUMB
//  214 void writeLogLine(void) {
_Z12writeLogLinev:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  215 #if EFI_FILE_LOGGING || defined(__DOXYGEN__)
//  216 	if (!main_loop_started)
        LDR.W    R0,??DataTable22_4
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??writeLogLine_0
//  217 		return;
//  218 	resetLogging(&fileLogger);
??writeLogLine_1:
        LDR.W    R0,??DataTable18_3
          CFI FunCall resetLogging
        BL       resetLogging
//  219 	printSensors(&fileLogger, true);
        MOVS     R1,#+1
        LDR.W    R0,??DataTable18_3
          CFI FunCall _Z12printSensorsP7Loggingb
        BL       _Z12printSensorsP7Loggingb
//  220 
//  221 	if (isSdCardAlive()) {
          CFI FunCall isSdCardAlive
        BL       isSdCardAlive
        CMP      R0,#+0
        BEQ.N    ??writeLogLine_2
//  222 		appendPrintf(&fileLogger, "\r\n");
        ADR.N    R1,??DataTable9  ;; 0x0D, 0x0A, 0x00, 0x00
        LDR.W    R0,??DataTable18_3
          CFI FunCall appendPrintf
        BL       appendPrintf
//  223 		appendToLog(fileLogger.buffer);
        LDR.W    R0,??DataTable18_3
        LDR      R0,[R0, #+44]
          CFI FunCall appendToLog
        BL       appendToLog
//  224 		logFileLineIndex++;
        LDR.W    R0,??DataTable18_9
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.W    R1,??DataTable18_9
        STR      R0,[R1, #+0]
//  225 	}
//  226 #endif /* EFI_FILE_LOGGING */
//  227 }
??writeLogLine_2:
??writeLogLine_0:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock5

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5:
        DC8      "",0x0,0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_1:
        DC8      "RPM"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_2:
        DC8      "rpm"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_3:
        DC8      "V",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_4:
        DC8      "maf"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_5:
        DC8      "x",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_6:
        DC8      "kPa"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_7:
        DC8      "MAP"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_8:
        DC8      "AFR"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_9:
        DC8      "afr"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_10:
        DC8      "kph"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_11:
        DC8      "vss"
//  228 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z10printStatev
        THUMB
//  229 static void printState(void) {
_Z10printStatev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
//  230 #if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
//  231 
//  232 	// todo: make SWO work
//  233 //	char *msg = "hello\r\n";
//  234 //	for(int i=0;i<strlen(msg);i++) {
//  235 //		ITM_SendChar(msg[i]);
//  236 //	}
//  237 
//  238 	int rpm = getRpmE(engine);
        LDR.W    R0,??DataTable18_11
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN13RpmCalculator3rpmEv
        BL       _ZN13RpmCalculator3rpmEv
        MOVS     R4,R0
//  239 	if (subscription[(int) RO_TOTAL_REVOLUTION_COUNTER])
        LDR.W    R0,??DataTable24_1
        LDRB     R0,[R0, #+9]
        CMP      R0,#+0
        BEQ.N    ??printState_0
//  240 		debugInt(&logger, "ckp_c", getCrankEventCounter());
          CFI FunCall _Z20getCrankEventCounterv
        BL       _Z20getCrankEventCounterv
        MOVS     R2,R0
        LDR.W    R1,??DataTable24_2
        LDR.W    R0,??DataTable26_3
          CFI FunCall debugInt
        BL       debugInt
//  241 	if (subscription[(int) RO_RUNNING_REVOLUTION_COUNTER])
??printState_0:
        LDR.W    R0,??DataTable24_1
        LDRB     R0,[R0, #+10]
        CMP      R0,#+0
        BEQ.N    ??printState_1
//  242 		debugInt(&logger, "ckp_r", triggerCentral.triggerState.runningRevolutionCounter);
        LDR.W    R0,??DataTable24_3
        LDR      R2,[R0, #+80]
        LDR.W    R1,??DataTable24_4
        LDR.W    R0,??DataTable26_3
          CFI FunCall debugInt
        BL       debugInt
//  243 
//  244 	if (subscription[(int) RO_RUNNING_TRIGGER_ERROR])
??printState_1:
        LDR.W    R0,??DataTable24_1
        LDRB     R0,[R0, #+12]
        CMP      R0,#+0
        BEQ.N    ??printState_2
//  245 		debugInt(&logger, "trg_r_errors", triggerCentral.triggerState.runningTriggerErrorCounter);
        LDR.W    R0,??DataTable24_3
        LDR      R2,[R0, #+64]
        LDR.W    R1,??DataTable24_5
        LDR.W    R0,??DataTable26_3
          CFI FunCall debugInt
        BL       debugInt
//  246 
//  247 	if (subscription[(int) RO_RUNNING_ORDERING_TRIGGER_ERROR])
??printState_2:
        LDR.W    R0,??DataTable24_1
        LDRB     R0,[R0, #+13]
        CMP      R0,#+0
        BEQ.N    ??printState_3
//  248 		debugInt(&logger, "trg_r_order_errors", triggerCentral.triggerState.runningOrderingErrorCounter);
        LDR.W    R0,??DataTable24_3
        LDR      R2,[R0, #+72]
        LDR.W    R1,??DataTable25
        LDR.W    R0,??DataTable26_3
          CFI FunCall debugInt
        BL       debugInt
//  249 
//  250 	if (subscription[(int) RO_WAVE_CHART_CURRENT_SIZE])
??printState_3:
        LDR.W    R0,??DataTable24_1
        LDRB     R0,[R0, #+11]
        CMP      R0,#+0
        BEQ.N    ??printState_4
//  251 		debugInt(&logger, "wave_chart_current", 0);
        MOVS     R2,#+0
        LDR.W    R1,??DataTable26_4
        LDR.W    R0,??DataTable26_3
          CFI FunCall debugInt
        BL       debugInt
//  252 
//  253 //	debugInt(&logger, "idl", getIdleSwitch());
//  254 
//  255 //	debugFloat(&logger, "table_spark", getAdvance(rpm, getMaf()), 2);
//  256 
//  257 	float engineLoad = getEngineLoadT(PASS_ENGINE_PARAMETER_F);
??printState_4:
          CFI FunCall _Z14getEngineLoadTv
        BL       _Z14getEngineLoadTv
        VMOV.F32 S16,S0
//  258 	float baseFuel = getBaseFuel(rpm PASS_ENGINE_PARAMETER);
        MOVS     R0,R4
          CFI FunCall _Z11getBaseFueli
        BL       _Z11getBaseFueli
        VMOV.F32 S17,S0
//  259 	debugFloat(&logger, "fuel_base", baseFuel, 2);
        MOVS     R2,#+2
        VMOV.F32 S0,S17
        LDR.W    R1,??DataTable26_5
        LDR.W    R0,??DataTable26_3
          CFI FunCall debugFloat
        BL       debugFloat
//  260 //	debugFloat(&logger, "fuel_iat", getIatCorrection(getIntakeAirTemperature()), 2);
//  261 //	debugFloat(&logger, "fuel_clt", getCltCorrection(getCoolantTemperature()), 2);
//  262 	debugFloat(&logger, "fuel_lag", engine->injectorLagMs, 2);
        MOVS     R2,#+2
        LDR.W    R0,??DataTable18_11
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+7680
        ADDS     R0,R0,#+240
        VLDR     S0,[R0, #0]
        LDR.W    R1,??DataTable26_6
        LDR.W    R0,??DataTable26_3
          CFI FunCall debugFloat
        BL       debugFloat
//  263 	debugFloat(&logger, "fuel", getFuelMs(rpm PASS_ENGINE_PARAMETER), 2);
        MOVS     R0,R4
          CFI FunCall _Z9getFuelMsi
        BL       _Z9getFuelMsi
        MOVS     R2,#+2
        LDR.W    R1,??DataTable26_7
        LDR.W    R0,??DataTable26_3
          CFI FunCall debugFloat
        BL       debugFloat
//  264 
//  265 	debugFloat(&logger, "timing", engine->engineState.timingAdvance, 2);
        MOVS     R2,#+2
        LDR.W    R0,??DataTable18_11
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+7680
        ADDS     R0,R0,#+184
        VLDR     S0,[R0, #0]
        LDR.W    R1,??DataTable26_8
        LDR.W    R0,??DataTable26_3
          CFI FunCall debugFloat
        BL       debugFloat
//  266 
//  267 //		float map = getMap();
//  268 
//  269 #endif /* EFI_SHAFT_POSITION_INPUT */
//  270 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock6
//  271 
//  272 #define INITIAL_FULL_LOG TRUE
//  273 //#define INITIAL_FULL_LOG FALSE
//  274 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  275 volatile int needToReportStatus = FALSE;
needToReportStatus:
        DS8 4

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  276 static int prevCkpEventCounter = -1;
prevCkpEventCounter:
        DATA
        DC32 -1
//  277 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  278 static LoggingWithStorage logger2("main event handler");
logger2:
        DS8 260
//  279 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z11printStatusv
          CFI NoCalls
        THUMB
//  280 static void printStatus(void) {
//  281 	needToReportStatus = TRUE;
_Z11printStatusv:
        LDR.W    R0,??DataTable26_9
        MOVS     R1,#+1
        STR      R1,[R0, #+0]
//  282 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock7

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7:
        DC8      0x6B, 0x73, 0x00, 0x00
//  283 
//  284 /**
//  285  * Time when the firmware version was reported last time, in seconds
//  286  * TODO: implement a request/response instead of just constantly sending this out
//  287  */

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  288 static systime_t timeOfPreviousPrintVersion = (systime_t) -1;
timeOfPreviousPrintVersion:
        DATA
        DC32 4294967295
//  289 
//  290 #if EFI_PROD_CODE || defined(__DOXYGEN__)

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z11printOutPinPKc11brain_pin_e
        THUMB
//  291 static void printOutPin(const char *pinName, brain_pin_e hwPin) {
_Z11printOutPinPKc11brain_pin_e:
        PUSH     {R4,R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+12
        SUB      SP,SP,#+12
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  292 	appendPrintf(&logger, "outpin%s%s@%s%s", DELIMETER, pinName, hwPortname(hwPin), DELIMETER);
        MOVS     R0,R5
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        ADR.N    R1,??DataTable10_1  ;; ","
        STR      R1,[SP, #+4]
        STR      R0,[SP, #+0]
        MOVS     R3,R4
        ADR.N    R2,??DataTable10_1  ;; ","
        LDR.W    R1,??DataTable26_10
        LDR.W    R0,??DataTable26_3
          CFI FunCall appendPrintf
        BL       appendPrintf
//  293 }
        POP      {R0-R2,R4,R5,PC}  ;; return
          CFI EndBlock cfiBlock8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8:
        DC8      "%",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_1:
        DC8      0x54, 0x50, 0x00, 0x00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_2:
        DC8      0x6D, 0x73, 0x00, 0x00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_3:
        DC8      "deg"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_4:
        DC8      "C",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_5:
        DC8      "CLT"
//  294 #endif /* EFI_PROD_CODE */
//  295 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z9printInfoj
        THUMB
//  296 static void printInfo(systime_t nowSeconds) {
_Z9printInfoj:
        PUSH     {R1-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+24
        MOVS     R4,R0
//  297 	/**
//  298 	 * we report the version every 4 seconds - this way the console does not need to
//  299 	 * request it and we will display it pretty soon
//  300 	 */
//  301 	if (overflowDiff(nowSeconds, timeOfPreviousPrintVersion) < 4) {
        LDR.W    R0,??DataTable26_11
        LDR      R0,[R0, #+0]
        SUBS     R0,R4,R0
        CMP      R0,#+4
        BCC.N    ??printInfo_0
//  302 		return;
//  303 	}
//  304 	timeOfPreviousPrintVersion = nowSeconds;
??printInfo_1:
        LDR.W    R0,??DataTable26_11
        STR      R4,[R0, #+0]
//  305 	appendPrintf(&logger, "rusEfiVersion%s%d@%s %s%s", DELIMETER, getRusEfiVersion(), VCS_VERSION,
//  306 			getConfigurationName(engineConfiguration->engineType),
//  307 			DELIMETER);
        LDR.W    R0,??DataTable18_15
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20getConfigurationName13engine_type_e
        BL       _Z20getConfigurationName13engine_type_e
        MOVS     R5,R0
          CFI FunCall getRusEfiVersion
        BL       getRusEfiVersion
        ADR.N    R1,??DataTable10_1  ;; ","
        STR      R1,[SP, #+8]
        STR      R5,[SP, #+4]
        LDR.W    R1,??DataTable26_12
        STR      R1,[SP, #+0]
        MOVS     R3,R0
        ADR.N    R2,??DataTable10_1  ;; ","
        LDR.W    R1,??DataTable26_13
        LDR.W    R0,??DataTable26_3
          CFI FunCall appendPrintf
        BL       appendPrintf
//  308 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//  309 	printOutPin(CRANK1, boardConfiguration->triggerInputPins[0]);
        LDR.W    R0,??DataTable26_14
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+296]
        ADR.N    R0,??DataTable10_2  ;; 0x74, 0x31, 0x00, 0x00
          CFI FunCall _Z11printOutPinPKc11brain_pin_e
        BL       _Z11printOutPinPKc11brain_pin_e
//  310 	printOutPin(CRANK2, boardConfiguration->triggerInputPins[1]);
        LDR.W    R0,??DataTable26_14
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+300]
        ADR.N    R0,??DataTable10_3  ;; 0x74, 0x32, 0x00, 0x00
          CFI FunCall _Z11printOutPinPKc11brain_pin_e
        BL       _Z11printOutPinPKc11brain_pin_e
//  311 #if EFI_WAVE_ANALYZER || defined(__DOXYGEN__)
//  312 	printOutPin(WA_CHANNEL_1, boardConfiguration->logicAnalyzerPins[0]);
        LDR.W    R0,??DataTable26_14
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+388]
        LDR.W    R0,??DataTable26_15
          CFI FunCall _Z11printOutPinPKc11brain_pin_e
        BL       _Z11printOutPinPKc11brain_pin_e
//  313 	printOutPin(WA_CHANNEL_2, boardConfiguration->logicAnalyzerPins[1]);
        LDR.W    R0,??DataTable26_14
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+392]
        LDR.W    R0,??DataTable26_16
          CFI FunCall _Z11printOutPinPKc11brain_pin_e
        BL       _Z11printOutPinPKc11brain_pin_e
//  314 #endif
//  315 
//  316 	for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
        MOVS     R5,#+0
??printInfo_2:
        LDR.W    R0,??DataTable18_15
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+404]
        CMP      R5,R0
        BGE.N    ??printInfo_3
//  317 		printOutPin(enginePins.coils[i].name, boardConfiguration->ignitionPins[i]);
        LDR.W    R0,??DataTable26_14
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R5, LSL #+2
        LDR      R1,[R0, #+84]
        MOVS     R0,#+20
        LDR.W    R2,??DataTable26_17
        MLA      R0,R0,R5,R2
        LDR      R0,[R0, #+352]
          CFI FunCall _Z11printOutPinPKc11brain_pin_e
        BL       _Z11printOutPinPKc11brain_pin_e
//  318 
//  319 		printOutPin(enginePins.injectors[i].name, boardConfiguration->injectionPins[i]);
        LDR.W    R0,??DataTable26_14
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R5, LSL #+2
        LDR      R1,[R0, #+32]
        MOVS     R0,#+20
        LDR.W    R2,??DataTable26_17
        MLA      R0,R0,R5,R2
        LDR      R0,[R0, #+112]
          CFI FunCall _Z11printOutPinPKc11brain_pin_e
        BL       _Z11printOutPinPKc11brain_pin_e
//  320 	}
        ADDS     R5,R5,#+1
        B.N      ??printInfo_2
//  321 #endif
//  322 
//  323 }
??printInfo_3:
??printInfo_0:
        POP      {R0-R2,R4,R5,PC}  ;; return
          CFI EndBlock cfiBlock9

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9:
        DC8      0x0D, 0x0A, 0x00, 0x00
//  324 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  325 static systime_t timeOfPreviousReport = (systime_t) -1;
timeOfPreviousReport:
        DATA
        DC32 4294967295
//  326 
//  327 extern char errorMessageBuffer[200];
//  328 
//  329 /**
//  330  * @brief Sends all pending data to dev console
//  331  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z21updateDevConsoleStateP6Engine
        THUMB
//  332 void updateDevConsoleState(Engine *engine) {
_Z21updateDevConsoleStateP6Engine:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R5,R0
//  333 	if (!isConsoleReady()) {
          CFI FunCall _Z14isConsoleReadyv
        BL       _Z14isConsoleReadyv
        CMP      R0,#+0
        BEQ.N    ??updateDevConsoleState_0
//  334 		return;
//  335 	}
//  336 // looks like this is not needed anymore
//  337 //	checkIfShouldHalt();
//  338 	printPending();
??updateDevConsoleState_1:
          CFI FunCall printPending
        BL       printPending
//  339 
//  340 	/**
//  341 	 * this should go before the firmware error so that console can detect connection
//  342 	 */
//  343 	printSensors(&logger, false);
        MOVS     R1,#+0
        LDR.W    R0,??DataTable26_3
          CFI FunCall _Z12printSensorsP7Loggingb
        BL       _Z12printSensorsP7Loggingb
//  344 
//  345 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//  346 	// todo: unify with simulator!
//  347 	if (hasFirmwareError()) {
        LDR.W    R0,??DataTable26_18
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??updateDevConsoleState_2
//  348 		scheduleMsg(&logger, "FATAL error: %s", errorMessageBuffer);
        LDR.W    R2,??DataTable26_19
        LDR.W    R1,??DataTable26_20
        LDR.W    R0,??DataTable26_3
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  349 		warningEnabled = false;
        LDR.W    R0,??DataTable22
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//  350 		scheduleLogging(&logger);
        LDR.W    R0,??DataTable26_3
          CFI FunCall scheduleLogging
        BL       scheduleLogging
//  351 		return;
        B.N      ??updateDevConsoleState_0
//  352 	}
//  353 #endif
//  354 
//  355 #if (EFI_PROD_CODE && HAL_USE_ADC) || defined(__DOXYGEN__)
//  356 	pokeAdcInputs();
??updateDevConsoleState_2:
          CFI FunCall _Z13pokeAdcInputsv
        BL       _Z13pokeAdcInputsv
//  357 #endif
//  358 
//  359 	if (!fullLog) {
        LDR.W    R0,??DataTable26_21
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??updateDevConsoleState_0
//  360 		return;
//  361 	}
//  362 
//  363 	systime_t nowSeconds = getTimeNowSeconds();
??updateDevConsoleState_3:
          CFI FunCall getTimeNowSeconds
        BL       getTimeNowSeconds
        MOVS     R6,R0
//  364 	printInfo(nowSeconds);
        MOVS     R0,R6
          CFI FunCall _Z9printInfoj
        BL       _Z9printInfoj
//  365 
//  366 #if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
//  367 	int currentCkpEventCounter = getCrankEventCounter();
          CFI FunCall _Z20getCrankEventCounterv
        BL       _Z20getCrankEventCounterv
        MOVS     R4,R0
//  368 	if (prevCkpEventCounter == currentCkpEventCounter && timeOfPreviousReport == nowSeconds) {
        LDR.W    R0,??DataTable26_22
        LDR      R0,[R0, #+0]
        CMP      R0,R4
        BNE.N    ??updateDevConsoleState_4
        LDR.W    R0,??DataTable26_23
        LDR      R0,[R0, #+0]
        CMP      R0,R6
        BEQ.N    ??updateDevConsoleState_0
//  369 		return;
//  370 	}
//  371 	timeOfPreviousReport = nowSeconds;
??updateDevConsoleState_4:
        LDR.W    R0,??DataTable26_23
        STR      R6,[R0, #+0]
//  372 
//  373 	prevCkpEventCounter = currentCkpEventCounter;
        LDR.W    R0,??DataTable26_22
        STR      R4,[R0, #+0]
//  374 #else
//  375 	chThdSleepMilliseconds(200);
//  376 #endif
//  377 
//  378 	printState();
          CFI FunCall _Z10printStatev
        BL       _Z10printStatev
//  379 
//  380 #if EFI_WAVE_ANALYZER
//  381 	printWave(&logger);
        LDR.W    R0,??DataTable26_3
          CFI FunCall _Z9printWaveP7Logging
        BL       _Z9printWaveP7Logging
//  382 #endif
//  383 
//  384 	scheduleLogging(&logger);
        LDR.W    R0,??DataTable26_3
          CFI FunCall scheduleLogging
        BL       scheduleLogging
//  385 }
??updateDevConsoleState_0:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock10

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10:
        DC8      "MAT"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_1:
        DC8      ",",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_2:
        DC8      0x74, 0x31, 0x00, 0x00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_3:
        DC8      0x74, 0x32, 0x00, 0x00
//  386 
//  387 #if EFI_PROD_CODE
//  388 
//  389 /*
//  390  * command example:
//  391  * sfm 3500 400
//  392  * that would be 'show fuel for rpm 3500 maf 4.0'
//  393  */
//  394 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z13showFuelInfo2ff
        THUMB
//  395 static void showFuelInfo2(float rpm, float engineLoad) {
_Z13showFuelInfo2ff:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8-D11}
          CFI D11 Frame(CFA, -16)
          CFI D10 Frame(CFA, -24)
          CFI D9 Frame(CFA, -32)
          CFI D8 Frame(CFA, -40)
          CFI CFA R13+40
        SUB      SP,SP,#+16
          CFI CFA R13+56
        VMOV.F32 S16,S0
        VMOV.F32 S17,S1
//  396 
//  397 	float baseFuelMs = getBaseTableFuel(engineConfiguration, (int) rpm, engineLoad);
        VMOV.F32 S0,S17
        VCVT.S32.F32 S1,S16
        VMOV     R1,S1
        LDR.W    R0,??DataTable18_15
        LDR      R0,[R0, #+0]
          CFI FunCall _Z16getBaseTableFuelP22engine_configuration_sif
        BL       _Z16getBaseTableFuelP22engine_configuration_sif
        VMOV.F32 S18,S0
//  398 
//  399 	scheduleMsg(&logger, "SD magic fuel %f", sdMath(engineConfiguration, 100, 100, 14.7, convertCelsiusToKelvin(20)));
        VLDR.W   S3,??DataTable15  ;; 0x43929333
        VLDR.W   S2,??DataTable15_1  ;; 0x416b3333
        VLDR.W   S1,??DataTable15_2  ;; 0x42c80000
        VLDR.W   S0,??DataTable15_2  ;; 0x42c80000
        LDR.W    R0,??DataTable18_15
        LDR      R0,[R0, #+0]
          CFI FunCall _Z6sdMathP22engine_configuration_sffff
        BL       _Z6sdMathP22engine_configuration_sffff
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable26_24
        LDR.W    R0,??DataTable18_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  400 	scheduleMsg(&logger, "inj flow %fcc/min displacement %fL", engineConfiguration->injector.flow,
//  401 			engineConfiguration->specs.displacement);
        LDR.W    R0,??DataTable18_15
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+400]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        LDR.W    R0,??DataTable18_15
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+8]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable26_25
        LDR.N    R0,??DataTable18_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  402 
//  403 	scheduleMsg(&logger2, "algo=%s/pump=%s", getEngine_load_mode_e(engineConfiguration->algorithm),
//  404 			boolToString(enginePins.fuelPumpRelay.getLogicValue()));
        LDR.W    R0,??DataTable26_26
          CFI FunCall _ZN9OutputPin13getLogicValueEv
        BL       _ZN9OutputPin13getLogicValueEv
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R4,R0
        LDR.N    R0,??DataTable18_15
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+424]
          CFI FunCall _Z21getEngine_load_mode_e18engine_load_mode_e
        BL       _Z21getEngine_load_mode_e18engine_load_mode_e
        MOVS     R3,R4
        MOVS     R2,R0
        LDR.W    R1,??DataTable26_27
        LDR.N    R0,??DataTable18_5
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  405 
//  406 	scheduleMsg(&logger2, "phase=%f correction=%f", getinjectionOffset(rpm), engineConfiguration->globalFuelCorrection);
        VCVT.S32.F32 S0,S16
        VMOV     R0,S0
          CFI FunCall _Z18getinjectionOffseti
        BL       _Z18getinjectionOffseti
        VMOV.F32 S19,S0
        LDR.N    R0,??DataTable18_15
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+568]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        VMOV     R0,S19
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable26_28
        LDR.N    R0,??DataTable18_5
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  407 
//  408 	scheduleMsg(&logger2, "baro correction=%f", engine->engineState.baroCorrection);
        MOVW     R0,#+7888
        LDR.W    R1,??DataTable26_29
        LDR      R1,[R1, #+0]
        LDR      R0,[R0, R1]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable26_30
        LDR.N    R0,??DataTable18_5
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  409 
//  410 #if EFI_ENGINE_CONTROL
//  411 	scheduleMsg(&logger, "base cranking fuel %f", engineConfiguration->cranking.baseFuel);
        LDR.N    R0,??DataTable18_15
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+92]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable26_31
        LDR.N    R0,??DataTable18_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  412 	scheduleMsg(&logger2, "cranking fuel: %f", getCrankingFuel(PASS_ENGINE_PARAMETER_F));
          CFI FunCall _Z15getCrankingFuelv
        BL       _Z15getCrankingFuelv
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable26_32
        LDR.N    R0,??DataTable18_5
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  413 
//  414 	if (engine->rpmCalculator.isRunning()) {
        LDR.W    R0,??DataTable26_29
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN13RpmCalculator9isRunningEv
        BL       _ZN13RpmCalculator9isRunningEv
        CMP      R0,#+0
        BEQ.N    ??showFuelInfo2_0
//  415 		float iatCorrection = engine->engineState.iatFuelCorrection;
        LDR.W    R0,??DataTable26_29
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+7680
        ADDS     R0,R0,#+196
        VLDR     S19,[R0, #0]
//  416 		float cltCorrection = engine->engineState.cltFuelCorrection;
        LDR.W    R0,??DataTable26_29
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+7680
        ADDS     R0,R0,#+200
        VLDR     S20,[R0, #0]
//  417 		float injectorLag = engine->injectorLagMs;
        LDR.W    R0,??DataTable26_29
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+7680
        ADDS     R0,R0,#+240
        VLDR     S21,[R0, #0]
//  418 		scheduleMsg(&logger2, "rpm=%f engineLoad=%f", rpm, engineLoad);
        VMOV     R0,S17
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable26_33
        LDR.N    R0,??DataTable18_5
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  419 		scheduleMsg(&logger2, "baseFuel=%f", baseFuelMs);
        VMOV     R0,S18
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable26_34
        LDR.N    R0,??DataTable18_5
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  420 
//  421 		scheduleMsg(&logger2, "iatCorrection=%f cltCorrection=%f injectorLag=%f", iatCorrection, cltCorrection,
//  422 				injectorLag);
        VMOV     R0,S21
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+8]
        VMOV     R0,S20
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        VMOV     R0,S19
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable26_35
        LDR.N    R0,??DataTable18_5
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  423 
//  424 		float value = getRunningFuel(baseFuelMs, (int) rpm PASS_ENGINE_PARAMETER);
        VCVT.S32.F32 S0,S16
        VMOV     R0,S0
        VMOV.F32 S0,S18
          CFI FunCall _Z14getRunningFuelfi
        BL       _Z14getRunningFuelfi
        VMOV.F32 S22,S0
//  425 		scheduleMsg(&logger2, "injection pulse width: %f", value);
        VMOV     R0,S22
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable26_36
        LDR.N    R0,??DataTable18_5
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  426 	}
//  427 #endif
//  428 }
??showFuelInfo2_0:
        ADD      SP,SP,#+16
          CFI CFA R13+40
        VPOP     {D8-D11}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI D10 SameValue
          CFI D11 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock11
//  429 
//  430 #if EFI_ENGINE_CONTROL

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _Z12showFuelInfov
        THUMB
//  431 static void showFuelInfo(void) {
_Z12showFuelInfov:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
//  432 	showFuelInfo2((float) getRpmE(engine), getEngineLoadT(PASS_ENGINE_PARAMETER_F));
          CFI FunCall _Z14getEngineLoadTv
        BL       _Z14getEngineLoadTv
        VMOV.F32 S16,S0
        LDR.W    R0,??DataTable26_29
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN13RpmCalculator3rpmEv
        BL       _ZN13RpmCalculator3rpmEv
        VMOV     S0,R0
        VMOV.F32 S1,S16
        VCVT.F32.S32 S0,S0
          CFI FunCall _Z13showFuelInfo2ff
        BL       _Z13showFuelInfo2ff
//  433 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock12
//  434 #endif
//  435 
//  436 #endif /* EFI_PROD_CODE */
//  437 

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//  438 static THD_WORKING_AREA(lcdThreadStack, UTILITY_THREAD_STACK_SIZE);
lcdThreadStack:
        DS8 712
//  439 
//  440 /**
//  441  * blinking thread to show that we are alive
//  442  * that's a trivial task - a smaller stack should work
//  443  */

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//  444 static THD_WORKING_AREA(blinkingStack, 128);
blinkingStack:
        DS8 440
//  445 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  446 static OutputPin communicationPin;
communicationPin:
        DS8 16

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  447 OutputPin checkEnginePin;
checkEnginePin:
        DS8 16

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  448 OutputPin warningPin;
warningPin:
        DS8 16

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  449 OutputPin runningPin;
runningPin:
        DS8 16
//  450 extern engine_pins_s enginePins;
//  451 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  452 static OutputPin *leds[] = { &warningPin, &runningPin, &enginePins.errorLedPin, &communicationPin, &checkEnginePin };
leds:
        DATA
        DC32 warningPin, runningPin, enginePins + 50H, communicationPin
        DC32 checkEnginePin
//  453 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z14initStatisLedsv
        THUMB
//  454 static void initStatisLeds() {
_Z14initStatisLedsv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  455 #if EFI_PROD_CODE
//  456 	outputPinRegister("communication status 1", &communicationPin,
//  457 	LED_COMMUNICATION_PORT, LED_COMMUNICATION_PIN);
        MOVS     R3,#+15
        LDR.W    R2,??DataTable26_37  ;; 0x40020c00
        LDR.N    R1,??DataTable18_6
        LDR.W    R0,??DataTable26_38
          CFI FunCall _Z17outputPinRegisterPKcP9OutputPinP12GPIO_TypeDefj
        BL       _Z17outputPinRegisterPKcP9OutputPinP12GPIO_TypeDefj
//  458 #endif
//  459 
//  460 #if EFI_WARNING_LED
//  461 	outputPinRegister("warning", &warningPin, LED_WARNING_PORT,
//  462 	LED_WARNING_PIN);
        MOVS     R3,#+13
        LDR.W    R2,??DataTable26_37  ;; 0x40020c00
        LDR.N    R1,??DataTable18_8
        LDR.W    R0,??DataTable26_39
          CFI FunCall _Z17outputPinRegisterPKcP9OutputPinP12GPIO_TypeDefj
        BL       _Z17outputPinRegisterPKcP9OutputPinP12GPIO_TypeDefj
//  463 	outputPinRegister("is running status", &runningPin, LED_RUNNING_STATUS_PORT,
//  464 	LED_RUNNING_STATUS_PIN);
        MOVS     R3,#+12
        LDR.W    R2,??DataTable26_37  ;; 0x40020c00
        LDR.W    R1,??DataTable21
        LDR.W    R0,??DataTable26_40
          CFI FunCall _Z17outputPinRegisterPKcP9OutputPinP12GPIO_TypeDefj
        BL       _Z17outputPinRegisterPKcP9OutputPinP12GPIO_TypeDefj
//  465 #endif /* EFI_WARNING_LED */
//  466 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock13
//  467 
//  468 /**
//  469  * This method would blink all the LEDs just to test them
//  470  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _Z16initialLedsBlinkv
        THUMB
//  471 static void initialLedsBlink(void) {
_Z16initialLedsBlinkv:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
//  472 	int size = sizeof(leds) / sizeof(leds[0]);
        MOVS     R4,#+5
//  473 	for (int i = 0; i < size; i++)
        MOVS     R5,#+0
??initialLedsBlink_0:
        CMP      R5,R4
        BGE.N    ??initialLedsBlink_1
//  474 		leds[i]->setValue(1);
        MOVS     R1,#+1
        LDR.W    R0,??DataTable26_41
        LDR      R0,[R0, R5, LSL #+2]
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
        ADDS     R5,R5,#+1
        B.N      ??initialLedsBlink_0
//  475 
//  476 	chThdSleepMilliseconds(100);
??initialLedsBlink_1:
        MOVS     R0,#+100
          CFI FunCall chThdSleep
        BL       chThdSleep
//  477 
//  478 	for (int i = 0; i < size; i++)
        MOVS     R5,#+0
??initialLedsBlink_2:
        CMP      R5,R4
        BGE.N    ??initialLedsBlink_3
//  479 		leds[i]->setValue(0);
        MOVS     R1,#+0
        LDR.W    R0,??DataTable26_41
        LDR      R0,[R0, R5, LSL #+2]
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
        ADDS     R5,R5,#+1
        B.N      ??initialLedsBlink_2
//  480 }
??initialLedsBlink_3:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock14
//  481 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  482 int blinkingPeriod = 33;
blinkingPeriod:
        DATA
        DC32 33
//  483 
//  484 /**
//  485  * this is useful to test connectivity
//  486  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function _Z17setBlinkingPeriodi
          CFI NoCalls
        THUMB
//  487 static void setBlinkingPeriod(int value) {
//  488 	if (value > 0)
_Z17setBlinkingPeriodi:
        CMP      R0,#+1
        BLT.N    ??setBlinkingPeriod_0
//  489 		blinkingPeriod = value;
        LDR.W    R1,??DataTable26_42
        STR      R0,[R1, #+0]
//  490 }
??setBlinkingPeriod_0:
        BX       LR               ;; return
          CFI EndBlock cfiBlock15

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15:
        DC32     0x43929333

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_1:
        DC32     0x416b3333

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_2:
        DC32     0x42c80000
//  491 
//  492 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//  493 
//  494 extern efitick_t lastDecodingErrorTime;
//  495 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function _Z17isTriggerErrorNowv
        THUMB
//  496 static bool_t isTriggerErrorNow() {
_Z17isTriggerErrorNowv:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
//  497 	bool_t justHadError = (getTimeNowNt() - lastDecodingErrorTime) < US2NT(2 * 1000 * 3 * blinkingPeriod);
          CFI FunCall getTimeNowNt
        BL       getTimeNowNt
        LDR.W    R2,??DataTable26_43
        LDRD     R4,R5,[R2, #+0]
        SUBS     R0,R0,R4
        SBCS     R1,R1,R5
        LDR.W    R2,??DataTable26_42
        LDR      R2,[R2, #+0]
        MOVW     R3,#+6000
        MULS     R2,R3,R2
        MOVS     R3,#+168
        SMULL    R2,R3,R3,R2
        CMP      R1,R3
        BGT.N    ??isTriggerErrorNow_0
        BLT.N    ??isTriggerErrorNow_1
        CMP      R0,R2
        BCS.N    ??isTriggerErrorNow_0
??isTriggerErrorNow_1:
        MOVS     R4,#+1
        B.N      ??isTriggerErrorNow_2
??isTriggerErrorNow_0:
        MOVS     R4,#+0
//  498 	return justHadError || isTriggerDecoderError();
??isTriggerErrorNow_2:
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+0
        BNE.N    ??isTriggerErrorNow_3
          CFI FunCall _Z21isTriggerDecoderErrorv
        BL       _Z21isTriggerDecoderErrorv
        B.N      ??isTriggerErrorNow_4
??isTriggerErrorNow_3:
        MOVS     R0,#+1
??isTriggerErrorNow_4:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock16
//  499 }
//  500 
//  501 /**
//  502  * this thread has a lower-then-usual stack size so we cannot afford *print* methods here
//  503  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function _Z14blinkingThreadPv
        THUMB
//  504 static void blinkingThread(void *arg) {
_Z14blinkingThreadPv:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  505 	(void) arg;
//  506 	chRegSetThreadName("communication blinking");
        LDR.W    R0,??DataTable26_44
        LDR      R0,[R0, #+24]
        LDR.W    R1,??DataTable26_45
        STR      R1,[R0, #+24]
//  507 
//  508 	initialLedsBlink();
          CFI FunCall _Z16initialLedsBlinkv
        BL       _Z16initialLedsBlinkv
//  509 
//  510 	while (true) {
//  511 		int delayMs = isConsoleReady() ? 3 * blinkingPeriod : blinkingPeriod;
??blinkingThread_0:
          CFI FunCall _Z14isConsoleReadyv
        BL       _Z14isConsoleReadyv
        CMP      R0,#+0
        BEQ.N    ??blinkingThread_1
        LDR.W    R0,??DataTable26_42
        LDR      R0,[R0, #+0]
        MOVS     R1,#+3
        MUL      R5,R1,R0
        B.N      ??blinkingThread_2
??blinkingThread_1:
        LDR.W    R0,??DataTable26_42
        LDR      R5,[R0, #+0]
//  512 
//  513 #if EFI_INTERNAL_FLASH || defined(__DOXYGEN__)
//  514 		if (getNeedToWriteConfiguration()) {
??blinkingThread_2:
          CFI FunCall _Z27getNeedToWriteConfigurationv
        BL       _Z27getNeedToWriteConfigurationv
        CMP      R0,#+0
        BEQ.N    ??blinkingThread_3
//  515 			delayMs = 2 * delayMs;
        LSLS     R5,R5,#+1
//  516 		}
//  517 #endif
//  518 
//  519 		communicationPin.setValue(0);
??blinkingThread_3:
        MOVS     R1,#+0
        LDR.N    R0,??DataTable18_6
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
//  520 		warningPin.setValue(0);
        MOVS     R1,#+0
        LDR.N    R0,??DataTable18_8
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
//  521 		chThdSleepMilliseconds(delayMs);
        MOV      R0,#+1000
        MUL      R0,R0,R5
        SUBS     R0,R0,#+1
        MOV      R1,#+1000
        UDIV     R0,R0,R1
        ADDS     R0,R0,#+1
          CFI FunCall chThdSleep
        BL       chThdSleep
//  522 
//  523 		communicationPin.setValue(1);
        MOVS     R1,#+1
        LDR.N    R0,??DataTable18_6
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
//  524 #if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
//  525 		if (isTriggerErrorNow() || isIgnitionTimingError())
          CFI FunCall _Z17isTriggerErrorNowv
        BL       _Z17isTriggerErrorNowv
        CMP      R0,#+0
        BNE.N    ??blinkingThread_4
          CFI FunCall _Z21isIgnitionTimingErrorv
        BL       _Z21isIgnitionTimingErrorv
        CMP      R0,#+0
        BEQ.N    ??blinkingThread_5
//  526 			warningPin.setValue(1);
??blinkingThread_4:
        MOVS     R1,#+1
        LDR.N    R0,??DataTable18_8
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
//  527 #endif
//  528 		chThdSleepMilliseconds(delayMs);
??blinkingThread_5:
        MOV      R0,#+1000
        MUL      R0,R0,R5
        SUBS     R0,R0,#+1
        MOV      R1,#+1000
        UDIV     R0,R0,R1
        ADDS     R0,R0,#+1
          CFI FunCall chThdSleep
        BL       chThdSleep
        B.N      ??blinkingThread_0
          CFI EndBlock cfiBlock17
//  529 
//  530 	}
//  531 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17:
        DC32     LOGGING_BUFFER

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_1:
        DC32     `?<Constant "status loop">`
//  532 
//  533 #endif /* EFI_PROD_CODE */
//  534 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function _Z9lcdThreadPv
        THUMB
//  535 static void lcdThread(void *arg) {
_Z9lcdThreadPv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  536 	(void)arg;
//  537 	chRegSetThreadName("lcd");
        LDR.W    R0,??DataTable26_44
        LDR      R0,[R0, #+24]
        ADR.N    R1,??DataTable18  ;; "lcd"
        STR      R1,[R0, #+24]
//  538 	while (true) {
//  539 #if EFI_HD44780_LCD
//  540 		updateHD44780lcd(engine);
??lcdThread_0:
        LDR.N    R0,??DataTable18_11
        LDR      R0,[R0, #+0]
          CFI FunCall _Z16updateHD44780lcdP6Engine
        BL       _Z16updateHD44780lcdP6Engine
//  541 #endif
//  542 		chThdSleepMilliseconds(engineConfiguration->bc.lcdThreadPeriod);
        LDR.W    R0,??DataTable26_46
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+944]
        MOV      R1,#+1000
        MULS     R0,R1,R0
        SUBS     R0,R0,#+1
        MOV      R1,#+1000
        UDIV     R0,R0,R1
        ADDS     R0,R0,#+1
          CFI FunCall chThdSleep
        BL       chThdSleep
        B.N      ??lcdThread_0
          CFI EndBlock cfiBlock18
//  543 	}
//  544 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18:
        DC8      "lcd"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_1:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_2:
        DC32     `?<Constant "file logger">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_3:
        DC32     fileLogger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_4:
        DC32     `?<Constant "main event handler">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_5:
        DC32     logger2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_6:
        DC32     communicationPin

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_7:
        DC32     checkEnginePin

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_8:
        DC32     warningPin

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_9:
        DC32     logFileLineIndex

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_10:
        DC32     `?<Constant "time">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_11:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_12:
        DC32     `?<Constant "kg/hr">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_13:
        DC32     `?<Constant "mafr">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_14:
        DC32     `?<Constant "ENGINE_LOAD">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_15:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_16:
        DC32     `?<Constant "baro">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_17:
        DC32     `?<Constant "count">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable18_18:
        DC32     `?<Constant "vbatt">`
//  545 
//  546 #if EFI_TUNER_STUDIO
//  547 
//  548 extern fuel_Map3D_t veMap;
//  549 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock19 Using cfiCommon0
          CFI Function _Z22updateTunerStudioStateP25TunerStudioOutputChannels
        THUMB
//  550 void updateTunerStudioState(TunerStudioOutputChannels *tsOutputChannels DECLARE_ENGINE_PARAMETER_S) {
_Z22updateTunerStudioStateP25TunerStudioOutputChannels:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        VPUSH    {D8-D11}
          CFI D11 Frame(CFA, -24)
          CFI D10 Frame(CFA, -32)
          CFI D9 Frame(CFA, -40)
          CFI D8 Frame(CFA, -48)
          CFI CFA R13+48
        MOVS     R4,R0
//  551 #if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
//  552 	int rpm = getRpmE(engine);
        LDR.W    R0,??DataTable26_29
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN13RpmCalculator3rpmEv
        BL       _ZN13RpmCalculator3rpmEv
        VMOV     S16,R0
//  553 #else
//  554 	int rpm = 0;
//  555 #endif
//  556 
//  557 	float tps = getTPS(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z6getTPSv
        BL       _Z6getTPSv
        VMOV.F32 S17,S0
//  558 	float coolant = getCoolantTemperature(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z21getCoolantTemperaturev
        BL       _Z21getCoolantTemperaturev
        VMOV.F32 S18,S0
//  559 	float intake = getIntakeAirTemperature(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z23getIntakeAirTemperaturev
        BL       _Z23getIntakeAirTemperaturev
        VMOV.F32 S19,S0
//  560 
//  561 	float engineLoad = getEngineLoadT(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z14getEngineLoadTv
        BL       _Z14getEngineLoadTv
        VMOV.F32 S20,S0
//  562 	float baseFuelMs = getBaseFuel(rpm PASS_ENGINE_PARAMETER);
        VMOV     R0,S16
          CFI FunCall _Z11getBaseFueli
        BL       _Z11getBaseFueli
        VMOV.F32 S21,S0
//  563 
//  564 	// header
//  565 	tsOutputChannels->tsConfigVersion = TS_FILE_VERSION;
        LDR.W    R0,??DataTable26_47  ;; 0x1337961
        STR      R0,[R4, #+84]
//  566 
//  567 	// engine state
//  568 	tsOutputChannels->rpm = rpm;
        VSTR     S16,[R4, #0]
//  569 	tsOutputChannels->coolant_temperature = coolant;
        VSTR     S18,[R4, #+4]
//  570 	tsOutputChannels->intakeAirTemperature = intake;
        VSTR     S19,[R4, #+8]
//  571 	tsOutputChannels->throttlePositon = tps;
        VSTR     S17,[R4, #+12]
//  572 	if (hasMafSensor()) {
          CFI FunCall _Z12hasMafSensorv
        BL       _Z12hasMafSensorv
        CMP      R0,#+0
        BEQ.N    ??updateTunerStudioState_0
//  573 		tsOutputChannels->massAirFlowVoltage = getMaf();
          CFI FunCall _Z6getMafv
        BL       _Z6getMafv
        VSTR     S0,[R4, #+16]
//  574 	}
//  575 	tsOutputChannels->massAirFlowValue = getRealMaf();
??updateTunerStudioState_0:
          CFI FunCall _Z10getRealMafv
        BL       _Z10getRealMafv
        VSTR     S0,[R4, #+108]
//  576 	tsOutputChannels->veValue = veMap.getValue(getMap(), rpm);
          CFI FunCall _Z6getMapv
        BL       _Z6getMapv
        VCVT.F32.S32 S1,S16
        LDR.W    R0,??DataTable26_48
          CFI FunCall _ZN5Map3DILi16ELi16EE8getValueEff
        BL       _ZN5Map3DILi16ELi16EE8getValueEff
        VSTR     S0,[R4, #+112]
//  577 	tsOutputChannels->airFuelRatio = getAfr();
          CFI FunCall _Z6getAfrv
        BL       _Z6getAfrv
        VSTR     S0,[R4, #+20]
//  578 	if (hasVBatt(PASS_ENGINE_PARAMETER_F)) {
          CFI FunCall _Z8hasVBattv
        BL       _Z8hasVBattv
        CMP      R0,#+0
        BEQ.N    ??updateTunerStudioState_1
//  579 		tsOutputChannels->vBatt = getVBatt(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z8getVBattv
        BL       _Z8getVBattv
        VSTR     S0,[R4, #+28]
//  580 	}
//  581 	tsOutputChannels->tpsADC = getTPS10bitAdc(PASS_ENGINE_PARAMETER_F);
??updateTunerStudioState_1:
          CFI FunCall _Z14getTPS10bitAdcv
        BL       _Z14getTPS10bitAdcv
        STRH     R0,[R4, #+32]
//  582 #if EFI_ANALOG_SENSORS || defined(__DOXYGEN__)
//  583 	tsOutputChannels->baroPressure = getBaroPressure();
          CFI FunCall _Z15getBaroPressurev
        BL       _Z15getBaroPressurev
        VSTR     S0,[R4, #+36]
//  584 #endif /* EFI_ANALOG_SENSORS */
//  585 	tsOutputChannels->manifold_air_pressure = getMap();
          CFI FunCall _Z6getMapv
        BL       _Z6getMapv
        VSTR     S0,[R4, #+40]
//  586 	tsOutputChannels->engineLoad = engineLoad;
        VSTR     S20,[R4, #+24]
//  587 	tsOutputChannels->rpmAcceleration = engine->rpmCalculator.getRpmAcceleration();
        LDR.W    R0,??DataTable26_29
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN13RpmCalculator18getRpmAccelerationEv
        BL       _ZN13RpmCalculator18getRpmAccelerationEv
        VSTR     S0,[R4, #+104]
//  588 	tsOutputChannels->currentMapAccelDelta = engine->mapAccelEnrichment.getMapEnrichment(PASS_ENGINE_PARAMETER_F) * 100 / getMap();
        LDR.W    R0,??DataTable26_29
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+56
          CFI FunCall _ZN16AccelEnrichmemnt16getMapEnrichmentEv
        BL       _ZN16AccelEnrichmemnt16getMapEnrichmentEv
        VLDR.W   S1,??DataTable22_1  ;; 0x42c80000
        VMUL.F32 S22,S0,S1
          CFI FunCall _Z6getMapv
        BL       _Z6getMapv
        VDIV.F32 S0,S22,S0
        VSTR     S0,[R4, #+124]
//  589 	tsOutputChannels->tpsAccelFuel = engine->tpsAccelEnrichment.getTpsEnrichment(PASS_ENGINE_PARAMETER_F);
        LDR.W    R0,??DataTable26_29
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+332
          CFI FunCall _ZN16AccelEnrichmemnt16getTpsEnrichmentEv
        BL       _ZN16AccelEnrichmemnt16getTpsEnrichmentEv
        VSTR     S0,[R4, #+128]
//  590 	tsOutputChannels->deltaTps = engine->tpsAccelEnrichment.getDelta();
        LDR.W    R0,??DataTable26_29
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+332
          CFI FunCall _ZN16AccelEnrichmemnt8getDeltaEv
        BL       _ZN16AccelEnrichmemnt8getDeltaEv
        VSTR     S0,[R4, #+116]
//  591 	tsOutputChannels->triggerErrorsCounter = triggerCentral.triggerState.totalTriggerErrorCounter;
        LDR.N    R0,??DataTable24_3
        LDR      R0,[R0, #+60]
        STR      R0,[R4, #+120]
//  592 	tsOutputChannels->baroCorrection = engine->engineState.baroCorrection;
        MOVW     R0,#+7888
        LDR.W    R1,??DataTable26_29
        LDR      R1,[R1, #+0]
        LDR      R0,[R0, R1]
        STR      R0,[R4, #+132]
//  593 	tsOutputChannels->pedalPosition = getPedalPosition(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z16getPedalPositionv
        BL       _Z16getPedalPositionv
        VSTR     S0,[R4, #+136]
//  594 	tsOutputChannels->knockCount = engine->knockCount;
        LDR.W    R0,??DataTable26_29
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+620]
        STR      R0,[R4, #+144]
//  595 	tsOutputChannels->injectorDutyCycle = getInjectorDutyCycle(rpm PASS_ENGINE_PARAMETER);
        VMOV     R0,S16
          CFI FunCall _Z20getInjectorDutyCyclei
        BL       _Z20getInjectorDutyCyclei
        VSTR     S0,[R4, #+140]
//  596 	tsOutputChannels->fuelLevel = engine->engineState.fuelLevel;
        MOVW     R0,#+7768
        LDR.W    R1,??DataTable26_29
        LDR      R1,[R1, #+0]
        LDR      R0,[R0, R1]
        STR      R0,[R4, #+148]
//  597 	tsOutputChannels->hasFatalError = hasFirmwareError();
        LDR.N    R0,??DataTable26_18
        LDRB     R0,[R0, #+0]
        LDR      R1,[R4, #+80]
        BFI      R1,R0,#+6,#+1
        STR      R1,[R4, #+80]
//  598 
//  599 	tsOutputChannels->checkEngine = hasErrorCodes();
          CFI FunCall hasErrorCodes
        BL       hasErrorCodes
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LDR      R1,[R4, #+72]
        BFI      R1,R0,#+8,#+1
        STR      R1,[R4, #+72]
//  600 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//  601 	tsOutputChannels->isTriggerError = isTriggerErrorNow();
          CFI FunCall _Z17isTriggerErrorNowv
        BL       _Z17isTriggerErrorNowv
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LDR      R1,[R4, #+80]
        BFI      R1,R0,#+5,#+1
        STR      R1,[R4, #+80]
//  602 
//  603 #if EFI_MAX_31855 || defined(__DOXYGEN__)
//  604 	for (int i = 0; i < EGT_CHANNEL_COUNT; i++)
        MOVS     R5,#+0
??updateTunerStudioState_2:
        CMP      R5,#+8
        BGE.N    ??updateTunerStudioState_3
//  605 		tsOutputChannels->egtValues.values[i] = getEgtValue(i);
        MOVS     R0,R5
          CFI FunCall _Z11getEgtValuei
        BL       _Z11getEgtValuei
        ADDS     R1,R4,R5, LSL #+1
        STRH     R0,[R1, #+88]
        ADDS     R5,R5,#+1
        B.N      ??updateTunerStudioState_2
//  606 #endif /* EFI_MAX_31855 */
//  607 
//  608 #if EFI_INTERNAL_FLASH || defined(__DOXYGEN__)
//  609 	tsOutputChannels->needBurn = getNeedToWriteConfiguration();
??updateTunerStudioState_3:
          CFI FunCall _Z27getNeedToWriteConfigurationv
        BL       _Z27getNeedToWriteConfigurationv
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LDR      R1,[R4, #+72]
        BFI      R1,R0,#+9,#+1
        STR      R1,[R4, #+72]
//  610 #endif
//  611 #if EFI_FILE_LOGGING || defined(__DOXYGEN__)
//  612 	tsOutputChannels->hasSdCard = isSdCardAlive();
          CFI FunCall isSdCardAlive
        BL       isSdCardAlive
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LDR      R1,[R4, #+72]
        BFI      R1,R0,#+0,#+1
        STR      R1,[R4, #+72]
//  613 #endif
//  614 	tsOutputChannels->isFuelPumpOn = enginePins.fuelPumpRelay.getLogicValue();
        LDR.N    R0,??DataTable26_26
          CFI FunCall _ZN9OutputPin13getLogicValueEv
        BL       _ZN9OutputPin13getLogicValueEv
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LDR      R1,[R4, #+72]
        BFI      R1,R0,#+5,#+1
        STR      R1,[R4, #+72]
//  615 	tsOutputChannels->isFanOn = enginePins.fanRelay.getLogicValue();
        LDR.N    R0,??DataTable26_49
          CFI FunCall _ZN9OutputPin13getLogicValueEv
        BL       _ZN9OutputPin13getLogicValueEv
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LDR      R1,[R4, #+72]
        BFI      R1,R0,#+6,#+1
        STR      R1,[R4, #+72]
//  616 	tsOutputChannels->isO2HeaterOn = enginePins.o2heater.getLogicValue();
        LDR.N    R0,??DataTable26_50
          CFI FunCall _ZN9OutputPin13getLogicValueEv
        BL       _ZN9OutputPin13getLogicValueEv
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LDR      R1,[R4, #+72]
        BFI      R1,R0,#+7,#+1
        STR      R1,[R4, #+72]
//  617 	tsOutputChannels->isIgnitionEnabled = engineConfiguration->isIgnitionEnabled;
        LDR.N    R0,??DataTable26_46
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+1
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LDR      R1,[R4, #+72]
        BFI      R1,R0,#+1,#+1
        STR      R1,[R4, #+72]
//  618 	tsOutputChannels->injection_enabled = engineConfiguration->isInjectionEnabled;
        LDR.N    R0,??DataTable26_46
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        LDR      R1,[R4, #+72]
        BFI      R1,R0,#+2,#+1
        STR      R1,[R4, #+72]
//  619 	tsOutputChannels->cylinder_cleanup_enabled = engineConfiguration->isCylinderCleanupEnabled;
        LDR.N    R0,??DataTable26_46
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+2
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LDR      R1,[R4, #+72]
        BFI      R1,R0,#+3,#+1
        STR      R1,[R4, #+72]
//  620 	tsOutputChannels->cylinder_cleanup_activated = engine->isCylinderCleanupMode;
        MOVW     R0,#+7928
        LDR.N    R1,??DataTable26_29
        LDR      R1,[R1, #+0]
        LDRB     R0,[R0, R1]
        LDR      R1,[R4, #+72]
        BFI      R1,R0,#+4,#+1
        STR      R1,[R4, #+72]
//  621 	tsOutputChannels->secondTriggerChannelEnabled = engineConfiguration->secondTriggerChannelEnabled;
        LDR.N    R0,??DataTable26_46
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+3
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LDR      R1,[R4, #+72]
        BFI      R1,R0,#+10,#+1
        STR      R1,[R4, #+72]
//  622 #if EFI_VEHICLE_SPEED || defined(__DOXYGEN__)
//  623 	tsOutputChannels->vehicleSpeedKph = getVehicleSpeed();
          CFI FunCall _Z15getVehicleSpeedv
        BL       _Z15getVehicleSpeedv
        VSTR     S0,[R4, #+76]
//  624 #endif /* EFI_VEHICLE_SPEED */
//  625 	tsOutputChannels->isCltError = !isValidCoolantTemperature(getCoolantTemperature(PASS_ENGINE_PARAMETER_F));
          CFI FunCall _Z21getCoolantTemperaturev
        BL       _Z21getCoolantTemperaturev
          CFI FunCall _Z25isValidCoolantTemperaturef
        BL       _Z25isValidCoolantTemperaturef
        EORS     R0,R0,#0x1
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LDR      R1,[R4, #+80]
        BFI      R1,R0,#+1,#+1
        STR      R1,[R4, #+80]
//  626 	tsOutputChannels->isIatError = !isValidIntakeAirTemperature(getIntakeAirTemperature(PASS_ENGINE_PARAMETER_F));
          CFI FunCall _Z23getIntakeAirTemperaturev
        BL       _Z23getIntakeAirTemperaturev
          CFI FunCall _Z27isValidIntakeAirTemperaturef
        BL       _Z27isValidIntakeAirTemperaturef
        EORS     R0,R0,#0x1
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LDR      R1,[R4, #+80]
        BFI      R1,R0,#+3,#+1
        STR      R1,[R4, #+80]
//  627 #endif /* EFI_PROD_CODE */
//  628 
//  629 	tsOutputChannels->knockNowIndicator = engine->knockCount > 0;
        LDR.N    R0,??DataTable26_29
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+620]
        CMP      R0,#+1
        BLT.N    ??updateTunerStudioState_4
        MOVS     R0,#+1
        B.N      ??updateTunerStudioState_5
??updateTunerStudioState_4:
        MOVS     R0,#+0
??updateTunerStudioState_5:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LDR      R1,[R4, #+72]
        BFI      R1,R0,#+14,#+1
        STR      R1,[R4, #+72]
//  630 	tsOutputChannels->knockEverIndicator = engine->knockEver;
        LDR.N    R0,??DataTable26_29
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+616]
        LDR      R1,[R4, #+72]
        BFI      R1,R0,#+13,#+1
        STR      R1,[R4, #+72]
//  631 
//  632 	tsOutputChannels->clutchUpState = engine->clutchUpState;
        LDR.N    R0,??DataTable26_29
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+612]
        LDR      R1,[R4, #+72]
        BFI      R1,R0,#+11,#+1
        STR      R1,[R4, #+72]
//  633 	tsOutputChannels->clutchDownState = engine->clutchDownState;
        LDR.N    R0,??DataTable26_29
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+613]
        LDR      R1,[R4, #+72]
        BFI      R1,R0,#+12,#+1
        STR      R1,[R4, #+72]
//  634 	tsOutputChannels->tCharge = getTCharge(rpm, tps, coolant, intake);
        VMOV.F32 S2,S19
        VMOV.F32 S1,S18
        VMOV.F32 S0,S17
        VMOV     R0,S16
          CFI FunCall _Z10getTChargeifff
        BL       _Z10getTChargeifff
        VSTR     S0,[R4, #+52]
//  635 	float timing = engine->engineState.timingAdvance;
        LDR.N    R0,??DataTable26_29
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+7680
        ADDS     R0,R0,#+184
        VLDR     S22,[R0, #0]
//  636 	tsOutputChannels->ignitionAdvance = timing > 360 ? timing - 720 : timing;
        VLDR.W   S0,??DataTable26  ;; 0x43b40001
        VCMP.F32 S22,S0
        FMSTAT   
        BLT.N    ??updateTunerStudioState_6
        VLDR.W   S0,??DataTable26_1  ;; 0xc4340000
        VADD.F32 S0,S22,S0
        B.N      ??updateTunerStudioState_7
??updateTunerStudioState_6:
        VMOV.F32 S0,S22
??updateTunerStudioState_7:
        VSTR     S0,[R4, #+56]
//  637 	tsOutputChannels->sparkDwell = ENGINE(engineState.sparkDwell);
        LDR.N    R0,??DataTable26_51
        LDR      R0,[R0, #+0]
        STR      R0,[R4, #+60]
//  638 	tsOutputChannels->baseFuel = baseFuelMs;
        VSTR     S21,[R4, #+48]
//  639 	tsOutputChannels->pulseWidthMs = getFuelMs(rpm PASS_ENGINE_PARAMETER);
        VMOV     R0,S16
          CFI FunCall _Z9getFuelMsi
        BL       _Z9getFuelMsi
        VSTR     S0,[R4, #+64]
//  640 	tsOutputChannels->crankingFuelMs = getCrankingFuel(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z15getCrankingFuelv
        BL       _Z15getCrankingFuelv
        VSTR     S0,[R4, #+44]
//  641 }
        VPOP     {D8-D11}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI D10 SameValue
          CFI D11 SameValue
          CFI CFA R13+16
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock19
//  642 
//  643 extern TunerStudioOutputChannels tsOutputChannels;
//  644 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock20 Using cfiCommon0
          CFI Function _Z25prepareTunerStudioOutputsv
        THUMB
//  645 void prepareTunerStudioOutputs(void) {
_Z25prepareTunerStudioOutputsv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  646 	// sensor state for EFI Analytics Tuner Studio
//  647 	updateTunerStudioState(&tsOutputChannels PASS_ENGINE_PARAMETER);
        LDR.N    R0,??DataTable26_52
          CFI FunCall _Z22updateTunerStudioStateP25TunerStudioOutputChannels
        BL       _Z22updateTunerStudioStateP25TunerStudioOutputChannels
//  648 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock20
//  649 
//  650 #endif /* EFI_TUNER_STUDIO */
//  651 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock21 Using cfiCommon0
          CFI Function _Z9subscribei
          CFI NoCalls
        THUMB
//  652 static void subscribe(int outputOrdinal) {
//  653 	subscription[outputOrdinal] = true;
_Z9subscribei:
        LDR.N    R1,??DataTable24_1
        MOVS     R2,#+1
        STRB     R2,[R0, R1]
//  654 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock21

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21:
        DC32     runningPin
//  655 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock22 Using cfiCommon0
          CFI Function _Z11unsubscribei
          CFI NoCalls
        THUMB
//  656 static void unsubscribe(int outputOrdinal) {
//  657 	subscription[outputOrdinal] = false;
_Z11unsubscribei:
        LDR.N    R1,??DataTable24_1
        MOVS     R2,#+0
        STRB     R2,[R0, R1]
//  658 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock22

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22:
        DC32     warningEnabled

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22_1:
        DC32     0x42c80000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22_2:
        DC32     `?<Constant "tpsacc">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22_3:
        DC32     `?<Constant "advance">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22_4:
        DC32     main_loop_started
//  659 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock23 Using cfiCommon0
          CFI Function _Z14initStatusLoopP6Engine
        THUMB
//  660 void initStatusLoop(Engine *engine) {
_Z14initStatusLoopP6Engine:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  661 	setFullLog(INITIAL_FULL_LOG);
        MOVS     R0,#+1
          CFI FunCall _Z10setFullLogi
        BL       _Z10setFullLogi
//  662 	addConsoleActionI(FULL_LOGGING_KEY, setFullLog);
        ADR.W    R1,_Z10setFullLogi
        ADR.N    R0,??DataTable26_2  ;; 0x66, 0x6C, 0x00, 0x00
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  663 	addConsoleActionI("warn", setWarningEnabled);
        LDR.N    R1,??DataTable26_53
        LDR.N    R0,??DataTable26_54
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  664 
//  665 #if EFI_PROD_CODE
//  666 
//  667 #if EFI_ENGINE_CONTROL
//  668 	addConsoleActionFF("fuelinfo2", (VoidFloatFloat) showFuelInfo2);
        LDR.N    R1,??DataTable26_55
        LDR.N    R0,??DataTable26_56
          CFI FunCall addConsoleActionFF
        BL       addConsoleActionFF
//  669 	addConsoleAction("fuelinfo", showFuelInfo);
        LDR.N    R1,??DataTable26_57
        LDR.N    R0,??DataTable26_58
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  670 #endif
//  671 
//  672 	subscription[(int) RO_TRG1_DUTY] = true;
        LDR.N    R0,??DataTable24_1
        MOVS     R1,#+1
        STRB     R1,[R0, #+3]
//  673 	subscription[(int) RO_TRG2_DUTY] = true;
        LDR.N    R0,??DataTable24_1
        MOVS     R1,#+1
        STRB     R1,[R0, #+4]
//  674 	subscription[(int) RO_TRG3_DUTY] = false;
        LDR.N    R0,??DataTable24_1
        MOVS     R1,#+0
        STRB     R1,[R0, #+5]
//  675 	subscription[(int) RO_TRG4_DUTY] = false;
        LDR.N    R0,??DataTable24_1
        MOVS     R1,#+0
        STRB     R1,[R0, #+6]
//  676 	subscription[(int) RO_TOTAL_REVOLUTION_COUNTER] = true;
        LDR.N    R0,??DataTable24_1
        MOVS     R1,#+1
        STRB     R1,[R0, #+9]
//  677 	subscription[(int) RO_RUNNING_REVOLUTION_COUNTER] = false;
        LDR.N    R0,??DataTable24_1
        MOVS     R1,#+0
        STRB     R1,[R0, #+10]
//  678 
//  679 	addConsoleActionI("subscribe", subscribe);
        LDR.N    R1,??DataTable26_59
        LDR.N    R0,??DataTable26_60
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  680 	addConsoleActionI("unsubscribe", unsubscribe);
        LDR.N    R1,??DataTable26_61
        LDR.N    R0,??DataTable26_62
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  681 	addConsoleActionI("set_led_blinking_period", setBlinkingPeriod);
        LDR.N    R1,??DataTable26_63
        LDR.N    R0,??DataTable26_64
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  682 
//  683 	addConsoleAction("status", printStatus);
        LDR.N    R1,??DataTable26_65
        LDR.N    R0,??DataTable26_66
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  684 #endif /* EFI_PROD_CODE */
//  685 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock23
//  686 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock24 Using cfiCommon0
          CFI Function _Z18startStatusThreadsP6Engine
        THUMB
//  687 void startStatusThreads(Engine *engine) {
_Z18startStatusThreadsP6Engine:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        SUB      SP,SP,#+8
          CFI CFA R13+16
        MOVS     R4,R0
//  688 	// todo: refactoring needed, this file should probably be split into pieces
//  689 	chThdCreateStatic(lcdThreadStack, sizeof(lcdThreadStack), NORMALPRIO, (tfunc_t) lcdThread, engine);
        STR      R4,[SP, #+0]
        LDR.N    R3,??DataTable26_67
        MOVS     R2,#+64
        MOV      R1,#+712
        LDR.N    R0,??DataTable26_68
          CFI FunCall chThdCreateStatic
        BL       chThdCreateStatic
//  690 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//  691 	initStatisLeds();
          CFI FunCall _Z14initStatisLedsv
        BL       _Z14initStatisLedsv
//  692 	chThdCreateStatic(blinkingStack, sizeof(blinkingStack), NORMALPRIO, (tfunc_t) blinkingThread, NULL);
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        LDR.N    R3,??DataTable26_69
        MOVS     R2,#+64
        MOV      R1,#+440
        LDR.N    R0,??DataTable26_70
          CFI FunCall chThdCreateStatic
        BL       chThdCreateStatic
//  693 #endif /* EFI_PROD_CODE */
//  694 }
        POP      {R0,R1,R4,PC}    ;; return
          CFI EndBlock cfiBlock24

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24:
        DC32     `?<Constant "%d%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_1:
        DC32     subscription

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_2:
        DC32     `?<Constant "ckp_c">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_3:
        DC32     triggerCentral

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_4:
        DC32     `?<Constant "ckp_r">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24_5:
        DC32     `?<Constant "trg_r_errors">`
//  695 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock25 Using cfiCommon0
          CFI Function _Z10setFullLogi
        THUMB
//  696 void setFullLog(int value) {
_Z10setFullLogi:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  697 	print("Setting full logging: %s\r\n", boolToString(value));
        CMP      R4,#+0
        BEQ.N    ??setFullLog_0
        MOVS     R0,#+1
        B.N      ??setFullLog_1
??setFullLog_0:
        MOVS     R0,#+0
??setFullLog_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R1,R0
        LDR.N    R0,??DataTable26_71
          CFI FunCall print
        BL       print
//  698 	printMsg(&logger, "%s%d", FULL_LOGGING_KEY, value);
        MOVS     R3,R4
        ADR.N    R2,??DataTable26_2  ;; 0x66, 0x6C, 0x00, 0x00
        LDR.N    R1,??DataTable26_72
        LDR.N    R0,??DataTable26_3
          CFI FunCall printMsg
        BL       printMsg
//  699 	fullLog = value;
        CMP      R4,#+0
        BEQ.N    ??setFullLog_2
        MOVS     R0,#+1
        B.N      ??setFullLog_3
??setFullLog_2:
        MOVS     R0,#+0
??setFullLog_3:
        LDR.N    R1,??DataTable26_21
        STRB     R0,[R1, #+0]
//  700 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock25

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable25:
        DC32     `?<Constant "trg_r_order_errors">`

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock26 Using cfiCommon0
          CFI Function _Z10getFullLogv
          CFI NoCalls
        THUMB
_Z10getFullLogv:
        LDR.N    R0,??DataTable26_21
        LDRB     R0,[R0, #+0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock26

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26:
        DC32     0x43b40001

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_1:
        DC32     0xc4340000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_2:
        DC8      0x66, 0x6C, 0x00, 0x00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_3:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_4:
        DC32     `?<Constant "wave_chart_current">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_5:
        DC32     `?<Constant "fuel_base">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_6:
        DC32     `?<Constant "fuel_lag">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_7:
        DC32     `?<Constant "fuel">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_8:
        DC32     `?<Constant "timing">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_9:
        DC32     needToReportStatus

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_10:
        DC32     `?<Constant "outpin%s%s@%s%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_11:
        DC32     timeOfPreviousPrintVersion

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_12:
        DC32     `?<Constant "8529">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_13:
        DC32     `?<Constant "rusEfiVersion%s%d@%s ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_14:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_15:
        DC32     `?<Constant "input1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_16:
        DC32     `?<Constant "input2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_17:
        DC32     enginePins

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_18:
        DC32     hasFirmwareErrorFlag

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_19:
        DC32     errorMessageBuffer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_20:
        DC32     `?<Constant "FATAL error: %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_21:
        DC32     fullLog

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_22:
        DC32     prevCkpEventCounter

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_23:
        DC32     timeOfPreviousReport

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_24:
        DC32     `?<Constant "SD magic fuel %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_25:
        DC32     `?<Constant "inj flow %fcc/min dis...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_26:
        DC32     enginePins+0x30

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_27:
        DC32     `?<Constant "algo=%s/pump=%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_28:
        DC32     `?<Constant "phase=%f correction=%f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_29:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_30:
        DC32     `?<Constant "baro correction=%f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_31:
        DC32     `?<Constant "base cranking fuel %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_32:
        DC32     `?<Constant "cranking fuel: %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_33:
        DC32     `?<Constant "rpm=%f engineLoad=%f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_34:
        DC32     `?<Constant "baseFuel=%f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_35:
        DC32     `?<Constant "iatCorrection=%f cltC...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_36:
        DC32     `?<Constant "injection pulse width...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_37:
        DC32     0x40020c00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_38:
        DC32     `?<Constant "communication status 1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_39:
        DC32     `?<Constant "warning">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_40:
        DC32     `?<Constant "is running status">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_41:
        DC32     leds

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_42:
        DC32     blinkingPeriod

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_43:
        DC32     lastDecodingErrorTime

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_44:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_45:
        DC32     `?<Constant "communication blinking">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_46:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_47:
        DC32     0x1337961

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_48:
        DC32     veMap

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_49:
        DC32     enginePins+0x10

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_50:
        DC32     enginePins+0x40

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_51:
        DC32     _engine+0x1EB4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_52:
        DC32     tsOutputChannels

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_53:
        DC32     _Z17setWarningEnabledi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_54:
        DC32     `?<Constant "warn">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_55:
        DC32     _Z13showFuelInfo2ff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_56:
        DC32     `?<Constant "fuelinfo2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_57:
        DC32     _Z12showFuelInfov

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_58:
        DC32     `?<Constant "fuelinfo">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_59:
        DC32     _Z9subscribei

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_60:
        DC32     `?<Constant "subscribe">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_61:
        DC32     _Z11unsubscribei

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_62:
        DC32     `?<Constant "unsubscribe">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_63:
        DC32     _Z17setBlinkingPeriodi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_64:
        DC32     `?<Constant "set_led_blinking_period">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_65:
        DC32     _Z11printStatusv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_66:
        DC32     `?<Constant "status">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_67:
        DC32     _Z9lcdThreadPv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_68:
        DC32     lcdThreadStack

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_69:
        DC32     _Z14blinkingThreadPv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_70:
        DC32     blinkingStack

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_71:
        DC32     `?<Constant "Setting full logging:...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26_72:
        DC32     `?<Constant "%s%d">`

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN5Map3DILi16ELi16EE8getValueEff
          CFI Block cfiBlock27 Using cfiCommon0
          CFI Function _ZN5Map3DILi16ELi16EE8getValueEff
        THUMB
// __interwork __vfp float Map3D<16, 16>::getValue(float, float)
_ZN5Map3DILi16ELi16EE8getValueEff:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+8
          CFI CFA R13+24
        MOVS     R4,R0
        VMOV.F32 S16,S0
        VMOV.F32 S17,S1
        LDR      R0,[R4, #+72]
        LDR.N    R1,??getValue_0+0x4  ;; 0x923f558
        CMP      R0,R1
        BEQ.N    ??getValue_1
        LDR.N    R0,??getValue_0+0x8
          CFI FunCall firmwareError
        BL       firmwareError
        VLDR.W   S0,??getValue_0  ;; 0x7fffffff
        B.N      ??getValue_2
??getValue_1:
        STR      R4,[SP, #+0]
        MOVS     R3,#+16
        LDR      R2,[R4, #+68]
        VMOV.F32 S1,S17
        MOVS     R1,#+16
        LDR      R0,[R4, #+64]
        VMOV.F32 S0,S16
          CFI FunCall _Z13interpolate3dfPfifS_iPS_
        BL       _Z13interpolate3dfPfifS_iPS_
??getValue_2:
        ADD      SP,SP,#+8
          CFI CFA R13+16
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
        DATA
??getValue_0:
        DC32     0x7fffffff
        DC32     0x923f558
        DC32     _ZZN5Map3DILi16ELi16EE8getValueEffEs
          CFI EndBlock cfiBlock27

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
        SECTION_GROUP _ZZN5Map3DILi16ELi16EE8getValueEffEs
// __absolute char const <_ZZN5Map3DILi16ELi16EE8getValueEffEs>[20]
_ZZN5Map3DILi16ELi16EE8getValueEffEs:
        DATA
        DC8 "map not initialized"

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 "\t"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "%d%s">`:
        DATA
        DC8 "%d%s"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "time">`:
        DATA
        DC8 "time"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(0)
        DATA
        DC8 ""

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "rpm"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "RPM"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "maf"

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 "V"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "mafr">`:
        DATA
        DC8 "mafr"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "kg/hr">`:
        DATA
        DC8 "kg/hr"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ENGINE_LOAD">`:
        DATA
        DC8 "ENGINE_LOAD"

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 "x"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "MAP"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "kPa"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "baro">`:
        DATA
        DC8 "baro"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "afr"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "AFR"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "vss"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "kph"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "ks"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "count">`:
        DATA
        DC8 "count"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "vbatt">`:
        DATA
        DC8 "vbatt"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "TP"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 "%"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "tpsacc">`:
        DATA
        DC8 "tpsacc"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "ms"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "advance">`:
        DATA
        DC8 "advance"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "deg"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "CLT"

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 "C"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "MAT"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "\015\012"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ckp_c">`:
        DATA
        DC8 "ckp_c"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ckp_r">`:
        DATA
        DC8 "ckp_r"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "trg_r_errors">`:
        DATA
        DC8 "trg_r_errors"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "trg_r_order_errors">`:
        DATA
        DC8 "trg_r_order_errors"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "wave_chart_current">`:
        DATA
        DC8 "wave_chart_current"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fuel_base">`:
        DATA
        DC8 "fuel_base"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fuel_lag">`:
        DATA
        DC8 "fuel_lag"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fuel">`:
        DATA
        DC8 "fuel"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "timing">`:
        DATA
        DC8 "timing"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "outpin%s%s@%s%s">`:
        DATA
        DC8 "outpin%s%s@%s%s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 ","

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "rusEfiVersion%s%d@%s ...">`:
        DATA
        DC8 "rusEfiVersion%s%d@%s %s%s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "8529">`:
        DATA
        DC8 "8529"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "t1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "t2"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "input1">`:
        DATA
        DC8 "input1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "input2">`:
        DATA
        DC8 "input2"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FATAL error: %s">`:
        DATA
        DC8 "FATAL error: %s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SD magic fuel %f">`:
        DATA
        DC8 "SD magic fuel %f"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "inj flow %fcc/min dis...">`:
        DATA
        DC8 "inj flow %fcc/min displacement %fL"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "algo=%s/pump=%s">`:
        DATA
        DC8 "algo=%s/pump=%s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "phase=%f correction=%f">`:
        DATA
        DC8 "phase=%f correction=%f"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "baro correction=%f">`:
        DATA
        DC8 "baro correction=%f"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "base cranking fuel %f">`:
        DATA
        DC8 "base cranking fuel %f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "cranking fuel: %f">`:
        DATA
        DC8 "cranking fuel: %f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "rpm=%f engineLoad=%f">`:
        DATA
        DC8 "rpm=%f engineLoad=%f"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "baseFuel=%f">`:
        DATA
        DC8 "baseFuel=%f"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "iatCorrection=%f cltC...">`:
        DATA
        DC8 "iatCorrection=%f cltCorrection=%f injectorLag=%f"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "injection pulse width...">`:
        DATA
        DC8 "injection pulse width: %f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "communication status 1">`:
        DATA
        DC8 "communication status 1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "warning">`:
        DATA
        DC8 "warning"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "is running status">`:
        DATA
        DC8 "is running status"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "communication blinking">`:
        DATA
        DC8 "communication blinking"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "lcd"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "fl"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "warn">`:
        DATA
        DC8 "warn"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fuelinfo2">`:
        DATA
        DC8 "fuelinfo2"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fuelinfo">`:
        DATA
        DC8 "fuelinfo"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "subscribe">`:
        DATA
        DC8 "subscribe"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "unsubscribe">`:
        DATA
        DC8 "unsubscribe"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_led_blinking_period">`:
        DATA
        DC8 "set_led_blinking_period"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "status">`:
        DATA
        DC8 "status"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Setting full logging:...">`:
        DATA
        DC8 "Setting full logging: %s\015\012"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "%s%d">`:
        DATA
        DC8 "%s%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "status loop">`:
        DATA
        DC8 "status loop"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "file logger">`:
        DATA
        DC8 "file logger"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "main event handler">`:
        DATA
        DC8 "main event handler"
        DC8 0

        END
//  701 
//  702 bool getFullLog(void) {
//  703 	return fullLog;
//  704 }
// 
// 3 220 bytes in section .bss
//    41 bytes in section .data
//     4 bytes in section .init_array
//   913 bytes in section .rodata
// 4 344 bytes in section .text
// 
// 4 268 bytes of CODE  memory (+ 80 bytes shared)
//   893 bytes of CONST memory (+ 20 bytes shared)
// 3 261 bytes of DATA  memory
//
//Errors: none
//Warnings: 2
