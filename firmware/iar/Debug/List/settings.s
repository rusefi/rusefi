///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:10:02 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\set /
//                       tings.cpp                                            /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\set /
//                       tings.cpp -lCN F:\stuff\rusefi_sourceforge\firmware\ /
//                       iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge\firm /
//                       ware\iar\Debug\List\ -o F:\stuff\rusefi_sourceforge\ /
//                       firmware\iar\Debug\Obj\ --no_cse --no_unroll         /
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
//                       settings.s                                           /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME settings

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z10hwPortname11brain_pin_e
        EXTERN _Z11setDateTimePKc
        EXTERN _Z11showAltInfov
        EXTERN _Z12boolToStringb
        EXTERN _Z12setAlgorithm18engine_load_mode_e
        EXTERN _Z13getAdcChannel11brain_pin_e
        EXTERN _Z13getResistanceP14ThermistorConf
        EXTERN _Z13getTPSVoltagev
        EXTERN _Z13parseBrainPinPKc
        EXTERN _Z13printDateTimev
        EXTERN _Z13setAltPFactorf
        EXTERN _Z14getPinFunction11brain_pin_e
        EXTERN _Z14getTPS10bitAdcv
        EXTERN _Z15getTemperatureCP14ThermistorConfP14ThermistorMath
        EXTERN _Z15setWholeFuelMapf
        EXTERN _Z15writeToFlashNowv
        EXTERN _Z16getAdcChannelPin13adc_channel_e
        EXTERN _Z16getTiming_mode_e13timing_mode_e
        EXTERN _Z17getAdcChannelPort13adc_channel_e
        EXTERN _Z17getTrigger_type_e14trigger_type_e
        EXTERN _Z18getIgnition_mode_e15ignition_mode_e
        EXTERN _Z18getTpsRateOfChangev
        EXTERN _Z19getInjection_mode_e16injection_mode_e
        EXTERN _Z19getInternalAdcValuePKc13adc_channel_e
        EXTERN _Z20getPin_output_mode_e17pin_output_mode_e
        EXTERN _Z21getCoolantTemperaturev
        EXTERN _Z21getEngine_load_mode_e18engine_load_mode_e
        EXTERN _Z21resetConfigurationExtP7Logging13engine_type_e
        EXTERN _Z22getPinNameByAdcChannel13adc_channel_ePc
        EXTERN _Z23getIntakeAirTemperaturev
        EXTERN _Z25isValidCoolantTemperaturef
        EXTERN _Z27isValidIntakeAirTemperaturef
        EXTERN _Z28setToothedWheelConfigurationP12TriggerShapeii16operation_mode_e
        EXTERN _Z29getGlobalConfigurationVersionv
        EXTERN _Z35incrementGlobalConfigurationVersionv
        EXTERN _Z6getTPSv
        EXTERN _Z6setMapPA16_ff
        EXTERN _Z8getHwPin11brain_pin_e
        EXTERN _Z8portnameP12GPIO_TypeDef
        EXTERN _Z9findIndexPKfif
        EXTERN _Z9getHwPort11brain_pin_e
        EXTERN _ZN7LoggingC1EPKcPci
        EXTERN _ZN9OutputPin13getLogicValueEv
        EXTERN _ZN9WaveChart14resetWaveChartEv
        EXTERN __aeabi_f2d
        EXTERN absI
        EXTERN addConsoleAction
        EXTERN addConsoleActionF
        EXTERN addConsoleActionFF
        EXTERN addConsoleActionI
        EXTERN addConsoleActionII
        EXTERN addConsoleActionP
        EXTERN addConsoleActionS
        EXTERN addConsoleActionSS
        EXTERN addConsoleActionSSP
        EXTERN addConsoleActionSSS
        EXTERN appendMsgPostfix
        EXTERN appendMsgPrefix
        EXTERN appendPrintf
        EXTERN applyNewConfiguration
        EXTERN atoff
        EXTERN atoi
        EXTERN boardConfiguration
        EXTERN config
        EXTERN engine
        EXTERN engineConfiguration
        EXTERN enginePins
        EXTERN getTimeNowNt
        EXTERN maxNesting
        EXTERN scheduleLogging
        EXTERN scheduleMsg
        EXTERN strEqual
        EXTERN strEqualCaseInsensitive
        EXTERN waveChart
        EXTERN waveChartUsedSize

        PUBLIC _Z10stopEnginev
        PUBLIC _Z12initSettingsP22engine_configuration_s
        PUBLIC _Z13printSpiStateP7LoggingP21board_configuration_s
        PUBLIC _Z13setEngineTypei
        PUBLIC _Z15printFloatArrayPKcPfi
        PUBLIC _Z18printConfigurationP22engine_configuration_s
        PUBLIC _Z20getConfigurationName13engine_type_e
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\settings.cpp
//    1 /**
//    2  * @file settings.cpp
//    3  * @brief This file is about configuring engine via the human-readable protocol
//    4  *
//    5  * @date Dec 30, 2012
//    6  * @author Andrey Belomutskiy, (c) 2012-2015
//    7  */
//    8 
//    9 #include "main.h"
//   10 #include "settings.h"
//   11 #include "eficonsole.h"
//   12 #include "engine_configuration.h"
//   13 #include "adc_inputs.h"
//   14 #include "engine_controller.h"
//   15 #include "thermistors.h"
//   16 #include "adc_inputs.h"
//   17 #include "interpolation.h"
//   18 #include "tps.h"
//   19 #include "map.h"
//   20 #include "trigger_decoder.h"
//   21 #include "console_io.h"
//   22 #include "engine.h"
//   23 #include "efiGpio.h"
//   24 #include "engine_math.h"
//   25 #include "alternatorController.h"
//   26 
//   27 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//   28 #include "rtc_helper.h"
//   29 #include "rusefi.h"
//   30 #include "pin_repository.h"
//   31 #include "hardware.h"
//   32 #endif /* EFI_PROD_CODE */
//   33 
//   34 #if EFI_INTERNAL_FLASH || defined(__DOXYGEN__)
//   35 #include "flash_main.h"
//   36 #endif /* EFI_INTERNAL_FLASH */
//   37 
//   38 #if EFI_WAVE_CHART || defined(__DOXYGEN__)
//   39 #include "wave_chart.h"
//   40 extern int waveChartUsedSize;
//   41 extern WaveChart waveChart;
//   42 #endif /* EFI_WAVE_CHART */
//   43 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   44 static char LOGGING_BUFFER[1000];
LOGGING_BUFFER:
        DS8 1000

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   45 static Logging logger("settings control", LOGGING_BUFFER, sizeof(LOGGING_BUFFER));
        MOV      R3,#+1000
        LDR.W    R2,??DataTable21
        LDR.W    R1,??DataTable21_1
        LDR.W    R0,??DataTable35
          CFI FunCall _ZN7LoggingC1EPKcPci
        BL       _ZN7LoggingC1EPKcPci
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
logger:
        DS8 60
//   46 
//   47 extern int maxNesting;
//   48 extern engine_pins_s enginePins;
//   49 
//   50 /*
//   51  static void printIntArray(int array[], int size) {
//   52  for (int j = 0; j < size; j++) {
//   53  print("%d ", array[j]);
//   54  }
//   55  print("\r\n");
//   56  }
//   57  */
//   58 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z15printFloatArrayPKcPfi
        THUMB
//   59 void printFloatArray(const char *prefix, float array[], int size) {
_Z15printFloatArrayPKcPfi:
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
//   60 	appendMsgPrefix(&logger);
        LDR.W    R0,??DataTable35
          CFI FunCall appendMsgPrefix
        BL       appendMsgPrefix
//   61 	appendPrintf(&logger, prefix);
        MOVS     R1,R4
        LDR.W    R0,??DataTable35
          CFI FunCall appendPrintf
        BL       appendPrintf
//   62 	for (int j = 0; j < size; j++) {
        MOVS     R7,#+0
??printFloatArray_0:
        CMP      R7,R6
        BGE.N    ??printFloatArray_1
//   63 		appendPrintf(&logger, "%f ", array[j]);
        LDR      R0,[R5, R7, LSL #+2]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        ADR.N    R1,??DataTable3  ;; "%f "
        LDR.W    R0,??DataTable35
          CFI FunCall appendPrintf
        BL       appendPrintf
//   64 	}
        ADDS     R7,R7,#+1
        B.N      ??printFloatArray_0
//   65 	appendMsgPostfix(&logger);
??printFloatArray_1:
        LDR.W    R0,??DataTable35
          CFI FunCall appendMsgPostfix
        BL       appendMsgPostfix
//   66 	scheduleLogging(&logger);
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleLogging
        BL       scheduleLogging
//   67 }
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock1
//   68 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z13printSpiStateP7LoggingP21board_configuration_s
        THUMB
//   69 void printSpiState(Logging *logger, board_configuration_s *boardConfiguration) {
_Z13printSpiStateP7LoggingP21board_configuration_s:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//   70 	scheduleMsg(logger, "spi 1=%s/2=%s/3=%s", boolToString(boardConfiguration->is_enabled_spi_1),
//   71 			boolToString(boardConfiguration->is_enabled_spi_2), boolToString(boardConfiguration->is_enabled_spi_3));
        LDRB     R0,[R5, #+384]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+2
        ANDS     R0,R0,#0x1
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R6,R0
        LDRB     R0,[R5, #+384]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+1
        ANDS     R0,R0,#0x1
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R7,R0
        LDRB     R0,[R5, #+384]
        ANDS     R0,R0,#0x1
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        STR      R6,[SP, #+0]
        MOVS     R3,R7
        MOVS     R2,R0
        LDR.W    R1,??DataTable37
        MOVS     R0,R4
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   72 }
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock2
//   73 
//   74 extern board_configuration_s *boardConfiguration;
//   75 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z12printOutputsP22engine_configuration_s
        THUMB
//   76 static void printOutputs(engine_configuration_s *engineConfiguration) {
_Z12printOutputsP22engine_configuration_s:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+8
          CFI CFA R13+24
        MOVS     R4,R0
//   77 	// engine_configuration2_s *engineConfiguration2
//   78 	scheduleMsg(&logger, "injectionPins: mode %s", getPin_output_mode_e(boardConfiguration->injectionPinMode));
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+80]
          CFI FunCall _Z20getPin_output_mode_e17pin_output_mode_e
        BL       _Z20getPin_output_mode_e17pin_output_mode_e
        MOVS     R2,R0
        LDR.W    R1,??DataTable23_1
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   79 	for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
        MOVS     R5,#+0
??printOutputs_0:
        LDR      R0,[R4, #+404]
        CMP      R5,R0
        BGE.N    ??printOutputs_1
//   80 		brain_pin_e brainPin = boardConfiguration->injectionPins[i];
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R5, LSL #+2
        LDR      R6,[R0, #+32]
//   81 		scheduleMsg(&logger, "injection #%d @ %s", (1 + i), hwPortname(brainPin));
        MOVS     R0,R6
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R3,R0
        ADDS     R2,R5,#+1
        LDR.W    R1,??DataTable24
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   82 	}
        ADDS     R5,R5,#+1
        B.N      ??printOutputs_0
//   83 
//   84 	scheduleMsg(&logger, "ignitionPins: mode %s", getPin_output_mode_e(boardConfiguration->ignitionPinMode));
??printOutputs_1:
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+132]
          CFI FunCall _Z20getPin_output_mode_e17pin_output_mode_e
        BL       _Z20getPin_output_mode_e17pin_output_mode_e
        MOVS     R2,R0
        LDR.W    R1,??DataTable25
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   85 	for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
        MOVS     R5,#+0
??printOutputs_2:
        LDR      R0,[R4, #+404]
        CMP      R5,R0
        BGE.N    ??printOutputs_3
//   86 		brain_pin_e brainPin = boardConfiguration->ignitionPins[i];
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R5, LSL #+2
        LDR      R6,[R0, #+84]
//   87 		scheduleMsg(&logger, "ignition #%d @ %s", (1 + i), hwPortname(brainPin));
        MOVS     R0,R6
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R3,R0
        ADDS     R2,R5,#+1
        LDR.W    R1,??DataTable27
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   88 	}
        ADDS     R5,R5,#+1
        B.N      ??printOutputs_2
//   89 
//   90 	scheduleMsg(&logger, "idlePin: mode %s @ %s freq=%d", getPin_output_mode_e(boardConfiguration->idle.solenoidPinMode),
//   91 			hwPortname(boardConfiguration->idle.solenoidPin), boardConfiguration->idle.solenoidFrequency);
??printOutputs_3:
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+0]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R5,R0
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+4]
          CFI FunCall _Z20getPin_output_mode_e17pin_output_mode_e
        BL       _Z20getPin_output_mode_e17pin_output_mode_e
        LDR.W    R1,??DataTable32
        LDR      R1,[R1, #+0]
        LDR      R1,[R1, #+8]
        STR      R1,[SP, #+0]
        MOVS     R3,R5
        MOVS     R2,R0
        LDR.W    R1,??DataTable28
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   92 	scheduleMsg(&logger, "malfunctionIndicatorn: %s mode=%s", hwPortname(boardConfiguration->malfunctionIndicatorPin),
//   93 			getPin_output_mode_e(boardConfiguration->malfunctionIndicatorPinMode));
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+140]
          CFI FunCall _Z20getPin_output_mode_e17pin_output_mode_e
        BL       _Z20getPin_output_mode_e17pin_output_mode_e
        MOVS     R5,R0
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+136]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R3,R5
        MOVS     R2,R0
        LDR.W    R1,??DataTable29_1
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   94 
//   95 	scheduleMsg(&logger, "fuelPumpPin: mode %s @ %s", getPin_output_mode_e(boardConfiguration->fuelPumpPinMode),
//   96 			hwPortname(boardConfiguration->fuelPumpPin));
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+24]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R5,R0
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+28]
          CFI FunCall _Z20getPin_output_mode_e17pin_output_mode_e
        BL       _Z20getPin_output_mode_e17pin_output_mode_e
        MOVS     R3,R5
        MOVS     R2,R0
        LDR.W    R1,??DataTable30
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   97 
//   98 	scheduleMsg(&logger, "fanPin: mode %s @ %s", getPin_output_mode_e(boardConfiguration->fanPinMode),
//   99 			hwPortname(boardConfiguration->fanPin));
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+144]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R5,R0
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+148]
          CFI FunCall _Z20getPin_output_mode_e17pin_output_mode_e
        BL       _Z20getPin_output_mode_e17pin_output_mode_e
        MOVS     R3,R5
        MOVS     R2,R0
        LDR.W    R1,??DataTable31
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  100 
//  101 	scheduleMsg(&logger, "mainRelay: mode %s @ %s", getPin_output_mode_e(boardConfiguration->mainRelayPinMode),
//  102 			hwPortname(boardConfiguration->mainRelayPin));
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+308]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R5,R0
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+412]
          CFI FunCall _Z20getPin_output_mode_e17pin_output_mode_e
        BL       _Z20getPin_output_mode_e17pin_output_mode_e
        MOVS     R3,R5
        MOVS     R2,R0
        LDR.W    R1,??DataTable32_1
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  103 
//  104 	scheduleMsg(&logger, "alternator field: mode %s @ %s",
//  105 			getPin_output_mode_e(boardConfiguration->alternatorControlPinMode),
//  106 			hwPortname(boardConfiguration->alternatorControlPin));
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+168]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R5,R0
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+172]
          CFI FunCall _Z20getPin_output_mode_e17pin_output_mode_e
        BL       _Z20getPin_output_mode_e17pin_output_mode_e
        MOVS     R3,R5
        MOVS     R2,R0
        LDR.W    R1,??DataTable33
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  107 }
        POP      {R0,R1,R4-R6,PC}  ;; return
          CFI EndBlock cfiBlock3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3:
        DC8      "%f "
//  108 
//  109 EXTERN_ENGINE
//  110 ;
//  111 
//  112 /**
//  113  * These should be not very long because these are displayed on the LCD as is
//  114  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z20getConfigurationName13engine_type_e
          CFI NoCalls
        THUMB
//  115 const char* getConfigurationName(engine_type_e engineType) {
_Z20getConfigurationName13engine_type_e:
        MOVS     R1,R0
//  116 	switch (engineType) {
        MOVS     R0,R1
        CMP      R0,#+0
        BEQ.N    ??getConfigurationName_0
        CMP      R0,#+2
        BEQ.N    ??getConfigurationName_1
        CMP      R0,#+3
        BEQ.N    ??getConfigurationName_2
        CMP      R0,#+4
        BEQ.N    ??getConfigurationName_3
        CMP      R0,#+5
        BEQ.N    ??getConfigurationName_4
        CMP      R0,#+6
        BEQ.N    ??getConfigurationName_5
        CMP      R0,#+7
        BEQ.N    ??getConfigurationName_6
        CMP      R0,#+8
        BEQ.N    ??getConfigurationName_7
        CMP      R0,#+9
        BEQ.N    ??getConfigurationName_8
        CMP      R0,#+10
        BEQ.N    ??getConfigurationName_9
        CMP      R0,#+11
        BEQ.N    ??getConfigurationName_10
        CMP      R0,#+12
        BEQ.N    ??getConfigurationName_11
        CMP      R0,#+13
        BEQ.N    ??getConfigurationName_12
        CMP      R0,#+14
        BEQ.N    ??getConfigurationName_13
        CMP      R0,#+15
        BEQ.N    ??getConfigurationName_14
        CMP      R0,#+16
        BEQ.N    ??getConfigurationName_15
        CMP      R0,#+17
        BEQ.N    ??getConfigurationName_16
        CMP      R0,#+18
        BEQ.N    ??getConfigurationName_17
        CMP      R0,#+19
        BEQ.N    ??getConfigurationName_18
        CMP      R0,#+20
        BEQ.N    ??getConfigurationName_19
        CMP      R0,#+21
        BEQ.N    ??getConfigurationName_20
        CMP      R0,#+23
        BEQ.N    ??getConfigurationName_21
        CMP      R0,#+24
        BEQ.N    ??getConfigurationName_22
        CMP      R0,#+25
        BEQ.N    ??getConfigurationName_23
        CMP      R0,#+26
        BEQ.N    ??getConfigurationName_24
        CMP      R0,#+28
        BEQ.N    ??getConfigurationName_25
        CMP      R0,#+29
        BEQ.N    ??getConfigurationName_26
        CMP      R0,#+30
        BEQ.N    ??getConfigurationName_27
        CMP      R0,#+31
        BEQ.N    ??getConfigurationName_28
        CMP      R0,#+32
        BEQ.N    ??getConfigurationName_29
        B.N      ??getConfigurationName_30
//  117 	case CUSTOM_ENGINE:
//  118 		return "CUSTOM";
??getConfigurationName_0:
        LDR.W    R0,??DataTable35_2
        B.N      ??getConfigurationName_31
//  119 #if EFI_SUPPORT_DODGE_NEON
//  120 	case DODGE_NEON_1995:
//  121 		return "Neon95";
??getConfigurationName_1:
        LDR.W    R0,??DataTable35_3
        B.N      ??getConfigurationName_31
//  122 	case DODGE_NEON_2003:
//  123 		return "Neon03";
??getConfigurationName_21:
        LDR.W    R0,??DataTable35_4
        B.N      ??getConfigurationName_31
//  124 #endif /* EFI_SUPPORT_DODGE_NEON */
//  125 #if EFI_SUPPORT_FORD_ASPIRE
//  126 	case FORD_ASPIRE_1996:
//  127 		return "Aspire";
??getConfigurationName_2:
        LDR.W    R0,??DataTable35_5
        B.N      ??getConfigurationName_31
//  128 #endif /* EFI_SUPPORT_FORD_ASPIRE */
//  129 #if EFI_SUPPORT_FORD_FIESTA
//  130 	case FORD_FIESTA:
//  131 		return "Fiesta";
??getConfigurationName_3:
        LDR.W    R0,??DataTable35_6
        B.N      ??getConfigurationName_31
//  132 #endif /* EFI_SUPPORT_FORD_FIESTA */
//  133 #if EFI_SUPPORT_NISSAN_PRIMERA
//  134 	case NISSAN_PRIMERA:
//  135 		return "Primera";
??getConfigurationName_4:
        LDR.W    R0,??DataTable36
        B.N      ??getConfigurationName_31
//  136 #endif /* EFI_SUPPORT_NISSAN_PRIMERA */
//  137 	case HONDA_ACCORD_CD:
//  138 		return "Accord3";
??getConfigurationName_5:
        LDR.W    R0,??DataTable36_1
        B.N      ??getConfigurationName_31
//  139 	case HONDA_ACCORD_CD_TWO_WIRES:
//  140 		return "Accord2";
??getConfigurationName_16:
        LDR.W    R0,??DataTable37_1
        B.N      ??getConfigurationName_31
//  141 	case HONDA_ACCORD_CD_DIP:
//  142 		return "HondaD";
??getConfigurationName_17:
        LDR.W    R0,??DataTable37_2
        B.N      ??getConfigurationName_31
//  143 	case FORD_INLINE_6_1995:
//  144 		return "Fordi6";
??getConfigurationName_6:
        LDR.W    R0,??DataTable37_3
        B.N      ??getConfigurationName_31
//  145 	case GY6_139QMB:
//  146 		return "Gy6139";
??getConfigurationName_7:
        LDR.W    R0,??DataTable37_4
        B.N      ??getConfigurationName_31
//  147 	case MAZDA_MIATA_NB:
//  148 		return "MiataNB";
??getConfigurationName_8:
        LDR.W    R0,??DataTable37_5
        B.N      ??getConfigurationName_31
//  149 	case MAZDA_323:
//  150 		return "M323";
??getConfigurationName_10:
        LDR.W    R0,??DataTable37_6
        B.N      ??getConfigurationName_31
//  151 	case SATURN_ION_2004:
//  152 		return "Saturn Ion";
??getConfigurationName_11:
        LDR.W    R0,??DataTable37_7
        B.N      ??getConfigurationName_31
//  153 	case MINI_COOPER_R50:
//  154 		return "CoopR50";
??getConfigurationName_12:
        LDR.W    R0,??DataTable37_8
        B.N      ??getConfigurationName_31
//  155 	case FORD_ESCORT_GT:
//  156 		return "EscrtGT";
??getConfigurationName_13:
        LDR.W    R0,??DataTable37_9
        B.N      ??getConfigurationName_31
//  157 	case CITROEN_TU3JP:
//  158 		return "TU3JP";
??getConfigurationName_14:
        LDR.W    R0,??DataTable37_10
        B.N      ??getConfigurationName_31
//  159 	case ROVER_V8:
//  160 		return "Rvrv8";
??getConfigurationName_9:
        LDR.W    R0,??DataTable37_11
        B.N      ??getConfigurationName_31
//  161 	case MITSU_4G93:
//  162 		return "Mi4G93";
??getConfigurationName_15:
        LDR.W    R0,??DataTable37_12
        B.N      ??getConfigurationName_31
//  163 	case MIATA_1990:
//  164 		return "MX590";
??getConfigurationName_18:
        LDR.W    R0,??DataTable38
        B.N      ??getConfigurationName_31
//  165 	case MIATA_1994_DEVIATOR:
//  166 		return "MX594d";
??getConfigurationName_19:
        LDR.W    R0,??DataTable38_1
        B.N      ??getConfigurationName_31
//  167 	case MIATA_1994_SPAGS:
//  168 		return "MX594s";
??getConfigurationName_22:
        LDR.W    R0,??DataTable39
        B.N      ??getConfigurationName_31
//  169 	case MIATA_1996:
//  170 		return "MX596";
??getConfigurationName_20:
        LDR.W    R0,??DataTable39_1
        B.N      ??getConfigurationName_31
//  171 	case BMW_E34:
//  172 		return "BMWe34";
??getConfigurationName_23:
        LDR.W    R0,??DataTable40
        B.N      ??getConfigurationName_31
//  173 	case TEST_ENGINE:
//  174 		return "Test";
??getConfigurationName_24:
        LDR.W    R0,??DataTable40_1
        B.N      ??getConfigurationName_31
//  175 	case VW_ABA:
//  176 		return "VW_ABA";
??getConfigurationName_29:
        LDR.W    R0,??DataTable40_2
        B.N      ??getConfigurationName_31
//  177 	case SACHS:
//  178 		return "SACHS";
??getConfigurationName_26:
        LDR.W    R0,??DataTable40_3
        B.N      ??getConfigurationName_31
//  179 	case GM_2_2:
//  180 	  return "GM_2_2";
??getConfigurationName_27:
        LDR.W    R0,??DataTable40_4
        B.N      ??getConfigurationName_31
//  181 	case DODGE_RAM:
//  182 		return "DODGE_RAM";
??getConfigurationName_28:
        LDR.W    R0,??DataTable40_5
        B.N      ??getConfigurationName_31
//  183 	case MAZDA_626:
//  184 		return "Mazda626";
??getConfigurationName_25:
        LDR.W    R0,??DataTable40_6
        B.N      ??getConfigurationName_31
//  185 	default:
//  186 		return "UnknownType";
??getConfigurationName_30:
        LDR.W    R0,??DataTable52
??getConfigurationName_31:
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//  187 	}
//  188 }
//  189 
//  190 /**
//  191  * @brief	Prints current engine configuration to human-readable console.
//  192  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z18printConfigurationP22engine_configuration_s
        THUMB
//  193 void printConfiguration(engine_configuration_s *engineConfiguration) {
_Z18printConfigurationP22engine_configuration_s:
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
        SUB      SP,SP,#+24
          CFI CFA R13+56
        MOVS     R4,R0
//  194 
//  195 	scheduleMsg(&logger, "Template %s/%d trigger %s/%s/%d", getConfigurationName(engineConfiguration->engineType),
//  196 			engineConfiguration->engineType, getTrigger_type_e(engineConfiguration->trigger.type),
//  197 			getEngine_load_mode_e(engineConfiguration->algorithm), engineConfiguration->algorithm);
        LDR      R0,[R4, #+424]
          CFI FunCall _Z21getEngine_load_mode_e18engine_load_mode_e
        BL       _Z21getEngine_load_mode_e18engine_load_mode_e
        MOVS     R5,R0
        LDR      R0,[R4, #+536]
          CFI FunCall _Z17getTrigger_type_e14trigger_type_e
        BL       _Z17getTrigger_type_e14trigger_type_e
        MOVS     R6,R0
        LDR      R0,[R4, #+0]
          CFI FunCall _Z20getConfigurationName13engine_type_e
        BL       _Z20getConfigurationName13engine_type_e
        MOVS     R2,R0
        LDR      R0,[R4, #+424]
        STR      R0,[SP, #+8]
        STR      R5,[SP, #+4]
        STR      R6,[SP, #+0]
        LDR      R3,[R4, #+0]
        LDR.W    R1,??DataTable41
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  198 
//  199 
//  200 	scheduleMsg(&logger, "configurationVersion=%d", getGlobalConfigurationVersion());
          CFI FunCall _Z29getGlobalConfigurationVersionv
        BL       _Z29getGlobalConfigurationVersionv
        MOVS     R2,R0
        LDR.W    R1,??DataTable41_1
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  201 
//  202 	for (int k = 0; k < FUEL_LOAD_COUNT; k++) {
        MOVS     R0,#+0
??printConfiguration_0:
        CMP      R0,#+16
        BGE.N    ??printConfiguration_1
        ADDS     R0,R0,#+1
        B.N      ??printConfiguration_0
//  203 //		print("line %d (%f): ", k, engineConfiguration->fuelKeyBins[k]);
//  204 //		for (int r = 0; r < FUEL_RPM_COUNT; r++) {
//  205 //			print("%f ", engineConfiguration->fuelTable[k][r]);
//  206 //		}
//  207 //		print("\r\n");
//  208 	}
//  209 
//  210 //	printFloatArray("RPM bin: ", config->fuelRpmBins, FUEL_RPM_COUNT);
//  211 //
//  212 //	printFloatArray("Y bin: ", config->fuelLoadBins, FUEL_LOAD_COUNT);
//  213 //
//  214 //	printFloatArray("CLT: ", config->cltFuelCorr, CLT_CURVE_SIZE);
//  215 //	printFloatArray("CLT bins: ", config->cltFuelCorrBins, CLT_CURVE_SIZE);
//  216 //
//  217 //	printFloatArray("IAT: ", config->iatFuelCorr, IAT_CURVE_SIZE);
//  218 //	printFloatArray("IAT bins: ", config->iatFuelCorrBins, IAT_CURVE_SIZE);
//  219 //
//  220 //	printFloatArray("vBatt: ", engineConfiguration->injector.battLagCorr, VBAT_INJECTOR_CURVE_SIZE);
//  221 //	printFloatArray("vBatt bins: ", engineConfiguration->injector.battLagCorrBins, VBAT_INJECTOR_CURVE_SIZE);
//  222 
//  223 	scheduleMsg(&logger, "rpmHardLimit: %d/operationMode=%d", engineConfiguration->rpmHardLimit,
//  224 			engineConfiguration->operationMode);
??printConfiguration_1:
        LDR      R3,[R4, #+500]
        LDR      R2,[R4, #+420]
        LDR.W    R1,??DataTable41_2
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  225 
//  226 	scheduleMsg(&logger, "globalTriggerAngleOffset=%f", engineConfiguration->globalTriggerAngleOffset);
        LDR      R0,[R4, #+460]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable42
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  227 
//  228 	scheduleMsg(&logger, "=== cranking ===");
        LDR.W    R1,??DataTable42_1
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  229 	scheduleMsg(&logger, "crankingRpm: %d", engineConfiguration->cranking.rpm);
        LDRSH    R2,[R4, #+96]
        LDR.W    R1,??DataTable43
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  230 	scheduleMsg(&logger, "cranking injection %s", getInjection_mode_e(engineConfiguration->crankingInjectionMode));
        LDR      R0,[R4, #+428]
          CFI FunCall _Z19getInjection_mode_e16injection_mode_e
        BL       _Z19getInjection_mode_e16injection_mode_e
        MOVS     R2,R0
        LDR.W    R1,??DataTable43_1
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  231 
//  232 	if (engineConfiguration->useConstantDwellDuringCranking) {
        LDRB     R0,[R4, #+1496]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+4
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??printConfiguration_2
//  233 		scheduleMsg(&logger, "ignitionDwellForCrankingMs=%f", engineConfiguration->ignitionDwellForCrankingMs);
        LDR      R0,[R4, #+104]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable43_2
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
        B.N      ??printConfiguration_3
//  234 	} else {
//  235 		scheduleMsg(&logger, "cranking charge charge angle=%f fire at %f", engineConfiguration->crankingChargeAngle,
//  236 				engineConfiguration->crankingTimingAngle);
??printConfiguration_2:
        LDR      R0,[R4, #+440]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        LDR      R0,[R4, #+108]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable43_3
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  237 	}
//  238 
//  239 	scheduleMsg(&logger, "=== ignition ===");
??printConfiguration_3:
        LDR.W    R1,??DataTable44
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  240 
//  241 	scheduleMsg(&logger, "ignitionMode: %s/enabled=%s", getIgnition_mode_e(engineConfiguration->ignitionMode),
//  242 			boolToString(engineConfiguration->isIgnitionEnabled));
        LDRB     R0,[R4, #+1508]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+1
        ANDS     R0,R0,#0x1
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R5,R0
        LDR      R0,[R4, #+444]
          CFI FunCall _Z18getIgnition_mode_e15ignition_mode_e
        BL       _Z18getIgnition_mode_e15ignition_mode_e
        MOVS     R3,R5
        MOVS     R2,R0
        LDR.W    R1,??DataTable44_1
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  243 	scheduleMsg(&logger, "timingMode: %s", getTiming_mode_e(engineConfiguration->timingMode));
        LDR      R0,[R4, #+452]
          CFI FunCall _Z16getTiming_mode_e13timing_mode_e
        BL       _Z16getTiming_mode_e13timing_mode_e
        MOVS     R2,R0
        LDR.W    R1,??DataTable44_2
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  244 	if (engineConfiguration->timingMode == TM_FIXED) {
        LDR      R0,[R4, #+452]
        CMP      R0,#+1
        BNE.N    ??printConfiguration_4
//  245 		scheduleMsg(&logger, "fixedModeTiming: %d", (int) engineConfiguration->fixedModeTiming);
        VLDR     S0,[R4, #+456]
        VCVT.S32.F32 S0,S0
        VMOV     R2,S0
        LDR.W    R1,??DataTable45
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  246 	}
//  247 	scheduleMsg(&logger, "ignitionOffset=%f", engineConfiguration->ignitionOffset);
??printConfiguration_4:
        LDR      R0,[R4, #+448]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable46_1
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  248 
//  249 	scheduleMsg(&logger, "=== injection ===");
        LDR.W    R1,??DataTable47_1
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  250 	scheduleMsg(&logger, "injection %s offset=%f/enabled=%s", getInjection_mode_e(engineConfiguration->injectionMode),
//  251 			(double) engineConfiguration->injectionOffset, boolToString(engineConfiguration->isInjectionEnabled));
        LDRB     R0,[R4, #+1508]
        ANDS     R0,R0,#0x1
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R5,R0
        LDR      R0,[R4, #+432]
          CFI FunCall _Z19getInjection_mode_e16injection_mode_e
        BL       _Z19getInjection_mode_e16injection_mode_e
        MOVS     R6,R0
        STR      R5,[SP, #+8]
        LDR      R0,[R4, #+436]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        MOVS     R2,R6
        LDR.W    R1,??DataTable47_2
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  252 
//  253 	printOutputs(engineConfiguration);
        MOVS     R0,R4
          CFI FunCall _Z12printOutputsP22engine_configuration_s
        BL       _Z12printOutputsP22engine_configuration_s
//  254 
//  255 	scheduleMsg(&logger, "map_avg=%s/mil=%s/fp=%s/ts=%s/wa=%s/it=%s/fastAdc=%s",
//  256 			boolToString(engineConfiguration->isMapAveragingEnabled), boolToString(engineConfiguration->isMilEnabled),
//  257 			boolToString(engineConfiguration->isFuelPumpEnabled),
//  258 			boolToString(engineConfiguration->isTunerStudioEnabled),
//  259 			boolToString(engineConfiguration->isWaveAnalyzerEnabled),
//  260 			boolToString(engineConfiguration->isIdleThreadEnabled), boolToString(boardConfiguration->isFastAdcEnabled));
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+4
        ANDS     R0,R0,#0x1
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R5,R0
        LDRB     R0,[R4, #+1509]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+2
        ANDS     R0,R0,#0x1
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R6,R0
        LDRB     R0,[R4, #+1509]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+1
        ANDS     R0,R0,#0x1
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R7,R0
        LDRB     R0,[R4, #+1509]
        ANDS     R0,R0,#0x1
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOV      R8,R0
        LDRB     R0,[R4, #+1508]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+7
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOV      R9,R0
        LDRB     R0,[R4, #+1508]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+6
        ANDS     R0,R0,#0x1
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOV      R10,R0
        LDRB     R0,[R4, #+1508]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+5
        ANDS     R0,R0,#0x1
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        STR      R5,[SP, #+16]
        STR      R6,[SP, #+12]
        STR      R7,[SP, #+8]
        STR      R8,[SP, #+4]
        STR      R9,[SP, #+0]
        MOV      R3,R10
        MOVS     R2,R0
        LDR.W    R1,??DataTable48
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  261 
//  262 	scheduleMsg(&logger, "isManualSpinningMode=%s/isCylinderCleanupEnabled=%s",
//  263 			boolToString(engineConfiguration->isManualSpinningMode),
//  264 			boolToString(engineConfiguration->isCylinderCleanupEnabled));
        LDRB     R0,[R4, #+1508]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+2
        ANDS     R0,R0,#0x1
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R5,R0
        LDRB     R0,[R4, #+1509]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+4
        ANDS     R0,R0,#0x1
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R3,R5
        MOVS     R2,R0
        LDR.W    R1,??DataTable50
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  265 
//  266 	scheduleMsg(&logger, "clutchUp@%s: %s", hwPortname(boardConfiguration->clutchUpPin),
//  267 			boolToString(engine->clutchUpState));
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+612]
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R5,R0
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+796]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R3,R5
        MOVS     R2,R0
        LDR.W    R1,??DataTable51
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  268 	scheduleMsg(&logger, "clutchDown@%s: %s", hwPortname(boardConfiguration->clutchDownPin),
//  269 			boolToString(engine->clutchDownState));
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+613]
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R5,R0
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+160]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R3,R5
        MOVS     R2,R0
        LDR.W    R1,??DataTable52_1
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  270 
//  271 	scheduleMsg(&logger, "boardTestModeJumperPin: %s/nesting=%d",
//  272 			hwPortname(boardConfiguration->boardTestModeJumperPin), maxNesting);
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+336]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        LDR.W    R1,??DataTable53
        LDR      R3,[R1, #+0]
        MOVS     R2,R0
        LDR.W    R1,??DataTable53_1
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  273 
//  274 	scheduleMsg(&logger, "digitalPotentiometerSpiDevice %d", boardConfiguration->digitalPotentiometerSpiDevice);
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+212]
        LDR.W    R1,??DataTable54
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  275 
//  276 	for (int i = 0; i < DIGIPOT_COUNT; i++) {
        MOVS     R5,#+0
??printConfiguration_5:
        CMP      R5,#+4
        BGE.N    ??printConfiguration_6
//  277 		scheduleMsg(&logger, "digitalPotentiometer CS%d %s", i,
//  278 				hwPortname(boardConfiguration->digitalPotentiometerChipSelect[i]));
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R5, LSL #+2
        LDR      R0,[R0, #+216]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R3,R0
        MOVS     R2,R5
        LDR.W    R1,??DataTable54_1
        LDR.W    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  279 	}
        ADDS     R5,R5,#+1
        B.N      ??printConfiguration_5
//  280 #if EFI_PROD_CODE
//  281 
//  282 	printSpiState(&logger, boardConfiguration);
??printConfiguration_6:
        LDR.W    R0,??DataTable32
        LDR      R1,[R0, #+0]
        LDR.W    R0,??DataTable35
          CFI FunCall _Z13printSpiStateP7LoggingP21board_configuration_s
        BL       _Z13printSpiStateP7LoggingP21board_configuration_s
//  283 
//  284 #endif /* EFI_PROD_CODE */
//  285 }
        ADD      SP,SP,#+24
          CFI CFA R13+32
        POP      {R4-R10,PC}      ;; return
          CFI EndBlock cfiBlock5
//  286 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z20doPrintConfigurationP6Engine
        THUMB
//  287 static void doPrintConfiguration(Engine *engine) {
_Z20doPrintConfigurationP6Engine:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  288 	printConfiguration(engineConfiguration);
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
          CFI FunCall _Z18printConfigurationP22engine_configuration_s
        BL       _Z18printConfigurationP22engine_configuration_s
//  289 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock6
//  290 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z18setFixedModeTimingi
        THUMB
//  291 static void setFixedModeTiming(int value) {
_Z18setFixedModeTimingi:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV     S16,R0
//  292 	engineConfiguration->fixedModeTiming = value;
        VCVT.F32.S32 S0,S16
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        VSTR     S0,[R0, #+456]
//  293 	doPrintConfiguration(engine);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20doPrintConfigurationP6Engine
        BL       _Z20doPrintConfigurationP6Engine
//  294 	incrementGlobalConfigurationVersion();
          CFI FunCall _Z35incrementGlobalConfigurationVersionv
        BL       _Z35incrementGlobalConfigurationVersionv
//  295 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock7
//  296 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z13setTimingModei
        THUMB
//  297 static void setTimingMode(int value) {
_Z13setTimingModei:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  298 	engineConfiguration->timingMode = (timing_mode_e) value;
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        STR      R4,[R0, #+452]
//  299 	doPrintConfiguration(engine);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20doPrintConfigurationP6Engine
        BL       _Z20doPrintConfigurationP6Engine
//  300 	incrementGlobalConfigurationVersion();
          CFI FunCall _Z35incrementGlobalConfigurationVersionv
        BL       _Z35incrementGlobalConfigurationVersionv
//  301 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock8
//  302 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z13setEngineTypei
        THUMB
//  303 void setEngineType(int value) {
_Z13setEngineTypei:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  304 	engineConfiguration->engineType = (engine_type_e) value;
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        STR      R4,[R0, #+0]
//  305 	resetConfigurationExt(&logger, (engine_type_e) value PASS_ENGINE_PARAMETER);
        MOVS     R1,R4
        LDR.W    R0,??DataTable35
          CFI FunCall _Z21resetConfigurationExtP7Logging13engine_type_e
        BL       _Z21resetConfigurationExtP7Logging13engine_type_e
//  306 #if EFI_WAVE_CHART || defined(__DOXYGEN__)
//  307 	if (engine->isTestMode)
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+640]
        CMP      R0,#+0
        BEQ.N    ??setEngineType_0
//  308 		waveChart.resetWaveChart();
        LDR.W    R0,??DataTable56
          CFI FunCall _ZN9WaveChart14resetWaveChartEv
        BL       _ZN9WaveChart14resetWaveChartEv
//  309 #endif
//  310 
//  311 #if EFI_INTERNAL_FLASH
//  312 	writeToFlashNow();
??setEngineType_0:
          CFI FunCall _Z15writeToFlashNowv
        BL       _Z15writeToFlashNowv
//  313 //	scheduleReset();
//  314 #endif /* EFI_PROD_CODE */
//  315 	incrementGlobalConfigurationVersion();
          CFI FunCall _Z35incrementGlobalConfigurationVersionv
        BL       _Z35incrementGlobalConfigurationVersionv
//  316 	doPrintConfiguration(engine);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20doPrintConfigurationP6Engine
        BL       _Z20doPrintConfigurationP6Engine
//  317 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock9
//  318 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z24setIdleSolenoidFrequencyi
          CFI NoCalls
        THUMB
//  319 static void setIdleSolenoidFrequency(int value) {
//  320 	boardConfiguration->idle.solenoidFrequency = value;
_Z24setIdleSolenoidFrequencyi:
        LDR.W    R1,??DataTable32
        LDR      R1,[R1, #+0]
        STR      R0,[R1, #+8]
//  321 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock10
//  322 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z19setInjectionPinModei
        THUMB
//  323 static void setInjectionPinMode(int value) {
_Z19setInjectionPinModei:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  324 	boardConfiguration->injectionPinMode = (pin_output_mode_e) value;
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        STR      R4,[R0, #+80]
//  325 	doPrintConfiguration(engine);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20doPrintConfigurationP6Engine
        BL       _Z20doPrintConfigurationP6Engine
//  326 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock11
//  327 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _Z18setIgnitionPinModei
        THUMB
//  328 static void setIgnitionPinMode(int value) {
_Z18setIgnitionPinModei:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  329 	boardConfiguration->ignitionPinMode = (pin_output_mode_e) value;
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        STR      R4,[R0, #+132]
//  330 	doPrintConfiguration(engine);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20doPrintConfigurationP6Engine
        BL       _Z20doPrintConfigurationP6Engine
//  331 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock12
//  332 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z14setIdlePinModei
        THUMB
//  333 static void setIdlePinMode(int value) {
_Z14setIdlePinModei:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  334 	boardConfiguration->idle.solenoidPinMode = (pin_output_mode_e) value;
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        STR      R4,[R0, #+4]
//  335 	doPrintConfiguration(engine);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20doPrintConfigurationP6Engine
        BL       _Z20doPrintConfigurationP6Engine
//  336 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock13
//  337 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _Z18setInjectionOffsetf
        THUMB
//  338 static void setInjectionOffset(float value) {
_Z18setInjectionOffsetf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  339 	engineConfiguration->injectionOffset = value;
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        VSTR     S16,[R0, #+436]
//  340 	doPrintConfiguration(engine);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20doPrintConfigurationP6Engine
        BL       _Z20doPrintConfigurationP6Engine
//  341 	incrementGlobalConfigurationVersion();
          CFI FunCall _Z35incrementGlobalConfigurationVersionv
        BL       _Z35incrementGlobalConfigurationVersionv
//  342 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock14
//  343 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function _Z17setIgnitionOffsetf
        THUMB
//  344 static void setIgnitionOffset(float value) {
_Z17setIgnitionOffsetf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  345 	engineConfiguration->ignitionOffset = value;
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        VSTR     S16,[R0, #+448]
//  346 	doPrintConfiguration(engine);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20doPrintConfigurationP6Engine
        BL       _Z20doPrintConfigurationP6Engine
//  347 	incrementGlobalConfigurationVersion();
          CFI FunCall _Z35incrementGlobalConfigurationVersionv
        BL       _Z35incrementGlobalConfigurationVersionv
//  348 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock15
//  349 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function _Z18setFuelPumpPinModei
        THUMB
//  350 static void setFuelPumpPinMode(int value) {
_Z18setFuelPumpPinModei:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  351 	boardConfiguration->fuelPumpPinMode = (pin_output_mode_e) value;
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        STR      R4,[R0, #+28]
//  352 	doPrintConfiguration(engine);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20doPrintConfigurationP6Engine
        BL       _Z20doPrintConfigurationP6Engine
//  353 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock16
//  354 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function _Z30setMalfunctionIndicatorPinModei
        THUMB
//  355 static void setMalfunctionIndicatorPinMode(int value) {
_Z30setMalfunctionIndicatorPinModei:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  356 	boardConfiguration->malfunctionIndicatorPinMode = (pin_output_mode_e) value;
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        STR      R4,[R0, #+140]
//  357 	doPrintConfiguration(engine);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20doPrintConfigurationP6Engine
        BL       _Z20doPrintConfigurationP6Engine
//  358 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock17
//  359 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function _Z18setSensorChartModei
        THUMB
//  360 static void setSensorChartMode(int value) {
_Z18setSensorChartModei:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  361 	boardConfiguration->sensorChartMode = (sensor_chart_e) value;
        LDR.W    R0,??DataTable32
        LDR      R0,[R0, #+0]
        STR      R4,[R0, #+784]
//  362 	doPrintConfiguration(engine);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20doPrintConfigurationP6Engine
        BL       _Z20doPrintConfigurationP6Engine
//  363 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock18
//  364 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock19 Using cfiCommon0
          CFI Function _Z5setOMi
        THUMB
//  365 static void setOM(int value) {
_Z5setOMi:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  366 	engineConfiguration->operationMode = (operation_mode_e)value;
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        STR      R4,[R0, #+500]
//  367 	doPrintConfiguration(engine);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20doPrintConfigurationP6Engine
        BL       _Z20doPrintConfigurationP6Engine
//  368 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock19
//  369 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  370 static char pinNameBuffer[16];
pinNameBuffer:
        DS8 16
//  371 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock20 Using cfiCommon0
          CFI Function _Z15printThermistorPKcP14ThermistorConfP14ThermistorMath
        THUMB
//  372 static void printThermistor(const char *msg, ThermistorConf *config, ThermistorMath *tm) {
_Z15printThermistorPKcP14ThermistorConfP14ThermistorMath:
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
        SUB      SP,SP,#+40
          CFI CFA R13+80
        MOVS     R7,R0
        MOVS     R6,R1
        MOVS     R4,R2
//  373 	thermistor_curve_s * curve = &tm->curve;
//  374 	adc_channel_e adcChannel = config->adcChannel;
        LDR      R5,[R6, #+28]
//  375 	float voltage = getVoltageDivided("term", adcChannel);
        MOVS     R1,R5
        LDR.W    R0,??DataTable59_1
          CFI FunCall _Z19getInternalAdcValuePKc13adc_channel_e
        BL       _Z19getInternalAdcValuePKc13adc_channel_e
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        VLDR.W   S1,??DataTable23  ;; 0x3a400c01
        VMUL.F32 S0,S0,S1
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        VLDR     S1,[R0, #+464]
        VMUL.F32 S16,S0,S1
//  376 	float r = getResistance(config);
        MOVS     R0,R6
          CFI FunCall _Z13getResistanceP14ThermistorConf
        BL       _Z13getResistanceP14ThermistorConf
        VMOV.F32 S17,S0
//  377 
//  378 	float t = getTemperatureC(config, tm);
        MOVS     R1,R4
        MOVS     R0,R6
          CFI FunCall _Z15getTemperatureCP14ThermistorConfP14ThermistorMath
        BL       _Z15getTemperatureCP14ThermistorConfP14ThermistorMath
        VMOV.F32 S18,S0
//  379 
//  380 	thermistor_conf_s *tc = &config->config;
//  381 
//  382 	scheduleMsg(&logger, "%s volts=%f Celsius=%f sensorR=%f on channel %d", msg, voltage, t, r, adcChannel);
        STR      R5,[SP, #+24]
        VMOV     R0,S17
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+16]
        VMOV     R0,S18
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+8]
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        MOVS     R2,R7
        LDR.W    R1,??DataTable59_2
        LDR.W    R0,??DataTable55_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  383 	scheduleMsg(&logger, "@%s", getPinNameByAdcChannel(adcChannel, pinNameBuffer));
        LDR.W    R1,??DataTable59_3
        MOVS     R0,R5
          CFI FunCall _Z22getPinNameByAdcChannel13adc_channel_ePc
        BL       _Z22getPinNameByAdcChannel13adc_channel_ePc
        MOVS     R2,R0
        ADR.N    R1,??DataTable26  ;; "@%s"
        LDR.W    R0,??DataTable55_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  384 	scheduleMsg(&logger, "C=%f/R=%f C=%f/R=%f C=%f/R=%f",
//  385 			tc->tempC_1, tc->resistance_1,
//  386 			tc->tempC_2, tc->resistance_2,
//  387 			tc->tempC_3, tc->resistance_3);
        LDR      R0,[R6, #+20]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+32]
        LDR      R0,[R6, #+8]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+24]
        LDR      R0,[R6, #+16]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+16]
        LDR      R0,[R6, #+4]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+8]
        LDR      R0,[R6, #+12]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        LDR      R0,[R6, #+0]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable59_4
        LDR.W    R0,??DataTable55_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  388 
//  389 	scheduleMsg(&logger, "bias resistor=%fK A=%..100000f B=%..100000f C=%..100000f", tc->bias_resistor / 1000,
//  390 			curve->s_h_a, curve->s_h_b, curve->s_h_c);
        LDR      R0,[R4, #+8]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+16]
        LDR      R0,[R4, #+4]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+8]
        LDR      R0,[R4, #+0]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        VLDR     S0,[R6, #+24]
        VLDR.W   S1,??DataTable29  ;; 0x447a0000
        VDIV.F32 S0,S0,S1
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable59_5
        LDR.W    R0,??DataTable55_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  391 	scheduleMsg(&logger, "==============================");
        LDR.W    R1,??DataTable60
        LDR.W    R0,??DataTable55_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  392 }
        ADD      SP,SP,#+40
          CFI CFA R13+40
        VPOP     {D8-D9}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI CFA R13+24
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock20
//  393 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock21 Using cfiCommon0
          CFI Function _Z12printTPSInfov
        THUMB
//  394 static void printTPSInfo(void) {
_Z12printTPSInfov:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        VPUSH    {D8}
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        SUB      SP,SP,#+16
          CFI CFA R13+40
//  395 #if (EFI_PROD_CODE && HAL_USE_ADC) || defined(__DOXYGEN__)
//  396 	if (!engineConfiguration->hasTpsSensor) {
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+7
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+0
        BNE.N    ??printTPSInfo_0
//  397 		scheduleMsg(&logger, "NO TPS SENSOR");
        LDR.W    R1,??DataTable60_1
        LDR.W    R0,??DataTable55_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  398 		return;
        B.N      ??printTPSInfo_1
//  399 	}
//  400 
//  401 	GPIO_TypeDef* port = getAdcChannelPort(engineConfiguration->tpsAdcChannel);
??printTPSInfo_0:
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+524]
          CFI FunCall _Z17getAdcChannelPort13adc_channel_e
        BL       _Z17getAdcChannelPort13adc_channel_e
        MOVS     R4,R0
//  402 	int pin = getAdcChannelPin(engineConfiguration->tpsAdcChannel);
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+524]
          CFI FunCall _Z16getAdcChannelPin13adc_channel_e
        BL       _Z16getAdcChannelPin13adc_channel_e
        MOVS     R5,R0
//  403 
//  404 	scheduleMsg(&logger, "tps min %d/max %d v=%f @%s%d", engineConfiguration->tpsMin, engineConfiguration->tpsMax,
//  405 			getTPSVoltage(PASS_ENGINE_PARAMETER_F), portname(port), pin);
        MOVS     R0,R4
          CFI FunCall _Z8portnameP12GPIO_TypeDef
        BL       _Z8portnameP12GPIO_TypeDef
        MOVS     R6,R0
          CFI FunCall _Z13getTPSVoltagev
        BL       _Z13getTPSVoltagev
        STR      R5,[SP, #+12]
        STR      R6,[SP, #+8]
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        LDRSH    R3,[R0, #+84]
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        LDRSH    R2,[R0, #+82]
        LDR.W    R1,??DataTable61
        LDR.W    R0,??DataTable55_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  406 #endif
//  407 	scheduleMsg(&logger, "current 10bit=%d value=%f rate=%f", getTPS10bitAdc(), getTPS(PASS_ENGINE_PARAMETER_F),
//  408 			getTpsRateOfChange());
          CFI FunCall _Z18getTpsRateOfChangev
        BL       _Z18getTpsRateOfChangev
        VMOV.F32 S16,S0
          CFI FunCall _Z6getTPSv
        BL       _Z6getTPSv
        VMOV.F32 S17,S0
          CFI FunCall _Z14getTPS10bitAdcv
        BL       _Z14getTPS10bitAdcv
        MOVS     R6,R0
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+8]
        VMOV     R0,S17
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        MOVS     R2,R6
        LDR.W    R1,??DataTable61_1
        LDR.W    R0,??DataTable55_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  409 }
??printTPSInfo_1:
        ADD      SP,SP,#+16
          CFI CFA R13+24
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock21

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21:
        DC32     LOGGING_BUFFER

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_1:
        DC32     `?<Constant "settings control">`
//  410 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock22 Using cfiCommon0
          CFI Function _Z20printTemperatureInfov
        THUMB
//  411 static void printTemperatureInfo(void) {
_Z20printTemperatureInfov:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  412 #if EFI_ANALOG_SENSORS || defined(__DOXYGEN__)
//  413 	printThermistor("CLT", &engineConfiguration->clt, &engine->engineState.cltCurve);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
        ADD      R2,R0,#+7680
        ADDS     R2,R2,#+132
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        ADDS     R1,R0,#+256
        ADR.N    R0,??DataTable34  ;; "CLT"
          CFI FunCall _Z15printThermistorPKcP14ThermistorConfP14ThermistorMath
        BL       _Z15printThermistorPKcP14ThermistorConfP14ThermistorMath
//  414 	if (!isValidCoolantTemperature(getCoolantTemperature(PASS_ENGINE_PARAMETER_F))) {
          CFI FunCall _Z21getCoolantTemperaturev
        BL       _Z21getCoolantTemperaturev
          CFI FunCall _Z25isValidCoolantTemperaturef
        BL       _Z25isValidCoolantTemperaturef
        CMP      R0,#+0
        BNE.N    ??printTemperatureInfo_0
//  415 		scheduleMsg(&logger, "CLT sensing error");
        LDR.W    R1,??DataTable62
        LDR.W    R0,??DataTable55_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  416 	}
//  417 	printThermistor("IAT", &engineConfiguration->iat, &engine->engineState.iatCurve);
??printTemperatureInfo_0:
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
        ADD      R2,R0,#+7680
        ADDS     R2,R2,#+92
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        ADDS     R1,R0,#+288
        ADR.N    R0,??DataTable35_1  ;; "IAT"
          CFI FunCall _Z15printThermistorPKcP14ThermistorConfP14ThermistorMath
        BL       _Z15printThermistorPKcP14ThermistorConfP14ThermistorMath
//  418 	if (!isValidIntakeAirTemperature(getIntakeAirTemperature(PASS_ENGINE_PARAMETER_F))) {
          CFI FunCall _Z23getIntakeAirTemperaturev
        BL       _Z23getIntakeAirTemperaturev
          CFI FunCall _Z27isValidIntakeAirTemperaturef
        BL       _Z27isValidIntakeAirTemperaturef
        CMP      R0,#+0
        BNE.N    ??printTemperatureInfo_1
//  419 		scheduleMsg(&logger, "IAT sensing error");
        LDR.W    R1,??DataTable62_1
        LDR.W    R0,??DataTable55_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  420 	}
//  421 
//  422 	scheduleMsg(&logger, "fan=%s @ %s", boolToString(enginePins.fanRelay.getLogicValue()),
//  423 			hwPortname(boardConfiguration->fanPin));
??printTemperatureInfo_1:
        LDR.N    R0,??DataTable32
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+144]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R4,R0
        LDR.W    R0,??DataTable62_2
          CFI FunCall _ZN9OutputPin13getLogicValueEv
        BL       _ZN9OutputPin13getLogicValueEv
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R2,R0
        MOVS     R3,R4
        LDR.W    R1,??DataTable63
        LDR.W    R0,??DataTable55_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  424 
//  425 	scheduleMsg(&logger, "A/C relay=%s @ %s", boolToString(enginePins.acRelay.getLogicValue()),
//  426 			hwPortname(boardConfiguration->acRelayPin));
        LDR.N    R0,??DataTable32
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+608]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R4,R0
        LDR.W    R0,??DataTable63_1
          CFI FunCall _ZN9OutputPin13getLogicValueEv
        BL       _ZN9OutputPin13getLogicValueEv
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R2,R0
        MOVS     R3,R4
        LDR.W    R1,??DataTable63_2
        LDR.W    R0,??DataTable55_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  427 
//  428 #endif
//  429 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock22
//  430 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock23 Using cfiCommon0
          CFI Function _Z14setCrankingRpmi
        THUMB
//  431 static void setCrankingRpm(int value) {
_Z14setCrankingRpmi:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  432 	engineConfiguration->cranking.rpm = value;
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        STRH     R4,[R0, #+96]
//  433 	doPrintConfiguration(engine);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20doPrintConfigurationP6Engine
        BL       _Z20doPrintConfigurationP6Engine
//  434 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock23

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23:
        DC32     0x3a400c01

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_1:
        DC32     `?<Constant "injectionPins: mode %s">`
//  435 
//  436 /**
//  437  * this method is used in console - it also prints current configuration
//  438  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock24 Using cfiCommon0
          CFI Function _Z15setAlgorithmInti
        THUMB
//  439 static void setAlgorithmInt(int value) {
_Z15setAlgorithmInti:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  440 	setAlgorithm((engine_load_mode_e) value);
        MOVS     R0,R4
          CFI FunCall _Z12setAlgorithm18engine_load_mode_e
        BL       _Z12setAlgorithm18engine_load_mode_e
//  441 	doPrintConfiguration(engine);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20doPrintConfigurationP6Engine
        BL       _Z20doPrintConfigurationP6Engine
//  442 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock24

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable24:
        DC32     `?<Constant "injection #%d @ %s">`
//  443 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock25 Using cfiCommon0
          CFI Function _Z14setFiringOrderi
        THUMB
//  444 static void setFiringOrder(int value) {
_Z14setFiringOrderi:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  445 	engineConfiguration->specs.firingOrder = (firing_order_e) value;
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        STR      R4,[R0, #+408]
//  446 	doPrintConfiguration(engine);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20doPrintConfigurationP6Engine
        BL       _Z20doPrintConfigurationP6Engine
//  447 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock25

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable25:
        DC32     `?<Constant "ignitionPins: mode %s">`
//  448 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock26 Using cfiCommon0
          CFI Function _Z15setRpmHardLimiti
        THUMB
//  449 static void setRpmHardLimit(int value) {
_Z15setRpmHardLimiti:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  450 	engineConfiguration->rpmHardLimit = value;
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        STR      R4,[R0, #+420]
//  451 	doPrintConfiguration(engine);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20doPrintConfigurationP6Engine
        BL       _Z20doPrintConfigurationP6Engine
//  452 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock26

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable26:
        DC8      "@%s"
//  453 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock27 Using cfiCommon0
          CFI Function _Z15setCrankingFuelf
        THUMB
//  454 static void setCrankingFuel(float timeMs) {
_Z15setCrankingFuelf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  455 	engineConfiguration->cranking.baseFuel = timeMs;
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        VSTR     S16,[R0, #+92]
//  456 	printTemperatureInfo();
          CFI FunCall _Z20printTemperatureInfov
        BL       _Z20printTemperatureInfov
//  457 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock27

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable27:
        DC32     `?<Constant "ignition #%d @ %s">`
//  458 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock28 Using cfiCommon0
          CFI Function _Z27setGlobalTriggerAngleOffsetf
        THUMB
//  459 static void setGlobalTriggerAngleOffset(float value) {
_Z27setGlobalTriggerAngleOffsetf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  460 	engineConfiguration->globalTriggerAngleOffset = value;
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        VSTR     S16,[R0, #+460]
//  461 	incrementGlobalConfigurationVersion();
          CFI FunCall _Z35incrementGlobalConfigurationVersionv
        BL       _Z35incrementGlobalConfigurationVersionv
//  462 	doPrintConfiguration(engine);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20doPrintConfigurationP6Engine
        BL       _Z20doPrintConfigurationP6Engine
//  463 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock28

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable28:
        DC32     `?<Constant "idlePin: mode %s @ %s...">`
//  464 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock29 Using cfiCommon0
          CFI Function _Z22setCrankingTimingAnglef
        THUMB
//  465 static void setCrankingTimingAngle(float value) {
_Z22setCrankingTimingAnglef:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  466 	engineConfiguration->crankingTimingAngle = value;
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        VSTR     S16,[R0, #+440]
//  467 	incrementGlobalConfigurationVersion();
          CFI FunCall _Z35incrementGlobalConfigurationVersionv
        BL       _Z35incrementGlobalConfigurationVersionv
//  468 	doPrintConfiguration(engine);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20doPrintConfigurationP6Engine
        BL       _Z20doPrintConfigurationP6Engine
//  469 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock29

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable29:
        DC32     0x447a0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable29_1:
        DC32     `?<Constant "malfunctionIndicatorn...">`
//  470 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock30 Using cfiCommon0
          CFI Function _Z24setCrankingInjectionModei
        THUMB
//  471 static void setCrankingInjectionMode(int value) {
_Z24setCrankingInjectionModei:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  472 	engineConfiguration->crankingInjectionMode = (injection_mode_e) value;
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        STR      R4,[R0, #+428]
//  473 	incrementGlobalConfigurationVersion();
          CFI FunCall _Z35incrementGlobalConfigurationVersionv
        BL       _Z35incrementGlobalConfigurationVersionv
//  474 	doPrintConfiguration(engine);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20doPrintConfigurationP6Engine
        BL       _Z20doPrintConfigurationP6Engine
//  475 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock30

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable30:
        DC32     `?<Constant "fuelPumpPin: mode %s ...">`
//  476 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock31 Using cfiCommon0
          CFI Function _Z16setInjectionModei
        THUMB
//  477 static void setInjectionMode(int value) {
_Z16setInjectionModei:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  478 	engineConfiguration->injectionMode = (injection_mode_e) value;
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        STR      R4,[R0, #+432]
//  479 	incrementGlobalConfigurationVersion();
          CFI FunCall _Z35incrementGlobalConfigurationVersionv
        BL       _Z35incrementGlobalConfigurationVersionv
//  480 	doPrintConfiguration(engine);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20doPrintConfigurationP6Engine
        BL       _Z20doPrintConfigurationP6Engine
//  481 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock31

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable31:
        DC32     `?<Constant "fanPin: mode %s @ %s">`
//  482 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock32 Using cfiCommon0
          CFI Function _Z15setIgnitionModei
        THUMB
//  483 static void setIgnitionMode(int value) {
_Z15setIgnitionModei:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  484 	engineConfiguration->ignitionMode = (ignition_mode_e) value;
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        STR      R4,[R0, #+444]
//  485 	incrementGlobalConfigurationVersion();
          CFI FunCall _Z35incrementGlobalConfigurationVersionv
        BL       _Z35incrementGlobalConfigurationVersionv
//  486 	doPrintConfiguration(engine);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20doPrintConfigurationP6Engine
        BL       _Z20doPrintConfigurationP6Engine
//  487 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock32

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable32:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable32_1:
        DC32     `?<Constant "mainRelay: mode %s @ %s">`
//  488 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock33 Using cfiCommon0
          CFI Function _Z14setTriggerTypei
        THUMB
//  489 static void setTriggerType(int value) {
_Z14setTriggerTypei:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  490 	engineConfiguration->trigger.type = (trigger_type_e) value;
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        STR      R4,[R0, #+536]
//  491 	incrementGlobalConfigurationVersion();
          CFI FunCall _Z35incrementGlobalConfigurationVersionv
        BL       _Z35incrementGlobalConfigurationVersionv
//  492 	doPrintConfiguration(engine);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20doPrintConfigurationP6Engine
        BL       _Z20doPrintConfigurationP6Engine
//  493 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock33

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable33:
        DC32     `?<Constant "alternator field: mod...">`
//  494 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock34 Using cfiCommon0
          CFI Function _Z15setToothedWheelii
        THUMB
//  495 static void setToothedWheel(int total, int skipped) {
_Z15setToothedWheelii:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  496 	scheduleMsg(&logger, "toothed: total=%d/skipped=%d", total, skipped);
        MOVS     R3,R5
        MOVS     R2,R4
        LDR.W    R1,??DataTable65
        LDR.N    R0,??DataTable35
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  497 	setToothedWheelConfiguration(&engine->triggerShape, total, skipped, engineConfiguration->operationMode);
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        LDR      R3,[R0, #+500]
        MOVS     R2,R5
        MOVS     R1,R4
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+644
          CFI FunCall _Z28setToothedWheelConfigurationP12TriggerShapeii16operation_mode_e
        BL       _Z28setToothedWheelConfigurationP12TriggerShapeii16operation_mode_e
//  498 //	initializeTriggerShape(&logger, engineConfiguration, engineConfiguration2);
//  499 	incrementGlobalConfigurationVersion();
          CFI FunCall _Z35incrementGlobalConfigurationVersionv
        BL       _Z35incrementGlobalConfigurationVersionv
//  500 	doPrintConfiguration(engine);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20doPrintConfigurationP6Engine
        BL       _Z20doPrintConfigurationP6Engine
//  501 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock34

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable34:
        DC8      "CLT"
//  502 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock35 Using cfiCommon0
          CFI Function _Z22setCrankingChargeAnglef
        THUMB
//  503 static void setCrankingChargeAngle(float value) {
_Z22setCrankingChargeAnglef:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  504 	engineConfiguration->crankingChargeAngle = value;
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        VSTR     S16,[R0, #+108]
//  505 	incrementGlobalConfigurationVersion();
          CFI FunCall _Z35incrementGlobalConfigurationVersionv
        BL       _Z35incrementGlobalConfigurationVersionv
//  506 	doPrintConfiguration(engine);
        LDR.W    R0,??DataTable50_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20doPrintConfigurationP6Engine
        BL       _Z20doPrintConfigurationP6Engine
//  507 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock35

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable35:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable35_1:
        DC8      "IAT"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable35_2:
        DC32     `?<Constant "CUSTOM">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable35_3:
        DC32     `?<Constant "Neon95">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable35_4:
        DC32     `?<Constant "Neon03">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable35_5:
        DC32     `?<Constant "Aspire">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable35_6:
        DC32     `?<Constant "Fiesta">`
//  508 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock36 Using cfiCommon0
          CFI Function _Z23setGlobalFuelCorrectionf
        THUMB
//  509 static void setGlobalFuelCorrection(float value) {
_Z23setGlobalFuelCorrectionf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  510 	if (value < 0.01 || value > 50)
        VLDR.W   S0,??DataTable46  ;; 0x3c23d70b
        VCMP.F32 S16,S0
        FMSTAT   
        BMI.N    ??setGlobalFuelCorrection_0
        VLDR.W   S0,??DataTable47  ;; 0x42480001
        VCMP.F32 S16,S0
        FMSTAT   
        BLT.N    ??setGlobalFuelCorrection_1
//  511 		return;
??setGlobalFuelCorrection_0:
        B.N      ??setGlobalFuelCorrection_2
//  512 	scheduleMsg(&logger, "setting fuel mult=%f", value);
??setGlobalFuelCorrection_1:
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable65_1
        LDR.W    R0,??DataTable55_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  513 	engineConfiguration->globalFuelCorrection = value;
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        VSTR     S16,[R0, #+568]
//  514 }
??setGlobalFuelCorrection_2:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock36

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable36:
        DC32     `?<Constant "Primera">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable36_1:
        DC32     `?<Constant "Accord3">`
//  515 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock37 Using cfiCommon0
          CFI Function _Z10setCltBiasf
          CFI NoCalls
        THUMB
//  516 static void setCltBias(float value) {
//  517 	engineConfiguration->clt.config.bias_resistor = value;
_Z10setCltBiasf:
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        VSTR     S0,[R0, #+280]
//  518 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock37

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable37:
        DC32     `?<Constant "spi 1=%s/2=%s/3=%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable37_1:
        DC32     `?<Constant "Accord2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable37_2:
        DC32     `?<Constant "HondaD">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable37_3:
        DC32     `?<Constant "Fordi6">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable37_4:
        DC32     `?<Constant "Gy6139">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable37_5:
        DC32     `?<Constant "MiataNB">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable37_6:
        DC32     `?<Constant "M323">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable37_7:
        DC32     `?<Constant "Saturn Ion">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable37_8:
        DC32     `?<Constant "CoopR50">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable37_9:
        DC32     `?<Constant "EscrtGT">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable37_10:
        DC32     `?<Constant "TU3JP">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable37_11:
        DC32     `?<Constant "Rvrv8">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable37_12:
        DC32     `?<Constant "Mi4G93">`
//  519 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock38 Using cfiCommon0
          CFI Function _Z13setFanSettingff
        THUMB
//  520 static void setFanSetting(float onTempC, float offTempC) {
_Z13setFanSettingff:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+8
          CFI CFA R13+24
        VMOV.F32 S16,S0
        VMOV.F32 S17,S1
//  521 	if (onTempC <= offTempC) {
        VCMP.F32 S17,S16
        FMSTAT   
        BLT.N    ??setFanSetting_0
//  522 		scheduleMsg(&logger, "ON temp [%f] should be above OFF temp [%f]", onTempC, offTempC);
        VMOV     R0,S17
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable65_2
        LDR.W    R0,??DataTable55_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  523 		return;
        B.N      ??setFanSetting_1
//  524 	}
//  525 	engineConfiguration->fanOnTemperature = onTempC;
??setFanSetting_0:
        LDR.W    R0,??DataTable55
        LDR      R0,[R0, #+0]
        VSTR     S16,[R0, #+476]
//  526 	engineConfiguration->fanOffTemperature = offTempC;
        LDR.N    R0,??DataTable55
        LDR      R0,[R0, #+0]
        VSTR     S17,[R0, #+480]
//  527 }
??setFanSetting_1:
        ADD      SP,SP,#+8
          CFI CFA R13+16
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock38

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable38:
        DC32     `?<Constant "MX590">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable38_1:
        DC32     `?<Constant "MX594d">`
//  528 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock39 Using cfiCommon0
          CFI Function _Z10setIatBiasf
          CFI NoCalls
        THUMB
//  529 static void setIatBias(float value) {
//  530 	engineConfiguration->iat.config.bias_resistor = value;
_Z10setIatBiasf:
        LDR.N    R0,??DataTable55
        LDR      R0,[R0, #+0]
        VSTR     S0,[R0, #+312]
//  531 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock39

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable39:
        DC32     `?<Constant "MX594s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable39_1:
        DC32     `?<Constant "MX596">`
//  532 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock40 Using cfiCommon0
          CFI Function _Z15setVBattDividerf
          CFI NoCalls
        THUMB
//  533 static void setVBattDivider(float value) {
//  534 	engineConfiguration->vbattDividerCoeff = value;
_Z15setVBattDividerf:
        LDR.N    R0,??DataTable55
        LDR      R0,[R0, #+0]
        VSTR     S0,[R0, #+468]
//  535 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock40

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable40:
        DC32     `?<Constant "BMWe34">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable40_1:
        DC32     `?<Constant "Test">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable40_2:
        DC32     `?<Constant "VW_ABA">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable40_3:
        DC32     `?<Constant "SACHS">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable40_4:
        DC32     `?<Constant "GM_2_2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable40_5:
        DC32     `?<Constant "DODGE_RAM">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable40_6:
        DC32     `?<Constant "Mazda626">`
//  536 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock41 Using cfiCommon0
          CFI Function _Z17setWholeTimingMapf
        THUMB
//  537 static void setWholeTimingMap(float value) {
_Z17setWholeTimingMapf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  538 	// todo: table helper?
//  539 	scheduleMsg(&logger, "Setting whole timing map to %f", value);
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable65_3
        LDR.N    R0,??DataTable55_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  540 	for (int l = 0; l < IGN_LOAD_COUNT; l++) {
        MOVS     R0,#+0
??setWholeTimingMap_0:
        CMP      R0,#+16
        BGE.N    ??setWholeTimingMap_1
//  541 		for (int r = 0; r < IGN_RPM_COUNT; r++) {
        MOVS     R1,#+0
??setWholeTimingMap_2:
        CMP      R1,#+16
        BGE.N    ??setWholeTimingMap_3
//  542 			config->ignitionTable[l][r] = value;
        MOVS     R2,#+64
        LDR.W    R3,??DataTable66
        LDR      R3,[R3, #+0]
        MLA      R2,R2,R0,R3
        ADDS     R2,R2,R1, LSL #+2
        ADD      R2,R2,#+11776
        ADDS     R2,R2,#+56
        VSTR     S16,[R2, #0]
//  543 		}
        ADDS     R1,R1,#+1
        B.N      ??setWholeTimingMap_2
//  544 	}
??setWholeTimingMap_3:
        ADDS     R0,R0,#+1
        B.N      ??setWholeTimingMap_0
//  545 }
??setWholeTimingMap_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock41

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable41:
        DC32     `?<Constant "Template %s/%d trigge...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable41_1:
        DC32     `?<Constant "configurationVersion=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable41_2:
        DC32     `?<Constant "rpmHardLimit: %d/oper...">`
//  546 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock42 Using cfiCommon0
          CFI Function _Z19setWholePhaseMapCmdf
        THUMB
//  547 static void setWholePhaseMapCmd(float value) {
_Z19setWholePhaseMapCmdf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  548 	scheduleMsg(&logger, "Setting whole injection phase map to %f", value);
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable65_4
        LDR.N    R0,??DataTable55_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  549 	setMap(config->injectionPhase, value);
        VMOV.F32 S0,S16
        LDR.W    R0,??DataTable66
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+9472
        ADDS     R0,R0,#+56
          CFI FunCall _Z6setMapPA16_ff
        BL       _Z6setMapPA16_ff
//  550 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock42

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable42:
        DC32     `?<Constant "globalTriggerAngleOff...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable42_1:
        DC32     `?<Constant "=== cranking ===">`
//  551 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock43 Using cfiCommon0
          CFI Function _Z20setWholeTimingMapCmdf
        THUMB
//  552 static void setWholeTimingMapCmd(float value) {
_Z20setWholeTimingMapCmdf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  553 	scheduleMsg(&logger, "Setting whole timing advance map to %f", value);
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable65_5
        LDR.N    R0,??DataTable55_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  554 	setWholeTimingMap(value);
        VMOV.F32 S0,S16
          CFI FunCall _Z17setWholeTimingMapf
        BL       _Z17setWholeTimingMapf
//  555 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock43

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable43:
        DC32     `?<Constant "crankingRpm: %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable43_1:
        DC32     `?<Constant "cranking injection %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable43_2:
        DC32     `?<Constant "ignitionDwellForCrank...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable43_3:
        DC32     `?<Constant "cranking charge charg...">`
//  556 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock44 Using cfiCommon0
          CFI Function _Z13setWholeVeCmdf
        THUMB
//  557 static void setWholeVeCmd(float value) {
_Z13setWholeVeCmdf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  558 	scheduleMsg(&logger, "Setting whole VE map to %f", value);
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable65_6
        LDR.N    R0,??DataTable55_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  559 	if (engineConfiguration->algorithm != LM_SPEED_DENSITY) {
        LDR.N    R0,??DataTable55
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+424]
        CMP      R0,#+3
        BEQ.N    ??setWholeVeCmd_0
//  560 		scheduleMsg(&logger, "WARNING: setting VE map not in SD mode is pointless");
        LDR.W    R1,??DataTable65_7
        LDR.N    R0,??DataTable55_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  561 	}
//  562 	setMap(config->veTable, value);
??setWholeVeCmd_0:
        VMOV.F32 S0,S16
        LDR.W    R0,??DataTable66
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+12800
        ADDS     R0,R0,#+184
          CFI FunCall _Z6setMapPA16_ff
        BL       _Z6setMapPA16_ff
//  563 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock44

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable44:
        DC32     `?<Constant "=== ignition ===">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable44_1:
        DC32     `?<Constant "ignitionMode: %s/enab...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable44_2:
        DC32     `?<Constant "timingMode: %s">`
//  564 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock45 Using cfiCommon0
          CFI Function _Z18setWholeFuelMapCmdf
        THUMB
//  565 static void setWholeFuelMapCmd(float value) {
_Z18setWholeFuelMapCmdf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  566 	scheduleMsg(&logger, "Setting whole fuel map to %f", value);
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable65_8
        LDR.W    R0,??DataTable71
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  567 	if (engineConfiguration->algorithm == LM_SPEED_DENSITY) {
        LDR.N    R0,??DataTable55
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+424]
        CMP      R0,#+3
        BNE.N    ??setWholeFuelMapCmd_0
//  568 		scheduleMsg(&logger, "WARNING: setting fuel map in SD mode is pointless");
        LDR.W    R1,??DataTable65_9
        LDR.W    R0,??DataTable71
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  569 	}
//  570 	setWholeFuelMap(value PASS_ENGINE_PARAMETER);
??setWholeFuelMapCmd_0:
        VMOV.F32 S0,S16
          CFI FunCall _Z15setWholeFuelMapf
        BL       _Z15setWholeFuelMapf
//  571 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock45

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable45:
        DC32     `?<Constant "fixedModeTiming: %d">`
//  572 
//  573 #if EFI_PROD_CODE
//  574 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock46 Using cfiCommon0
          CFI Function _Z9setEgtSpii
          CFI NoCalls
        THUMB
//  575 static void setEgtSpi(int spi) {
//  576 	boardConfiguration->max31855spiDevice = (spi_device_e) spi;
_Z9setEgtSpii:
        LDR.W    R1,??DataTable71_1
        LDR      R1,[R1, #+0]
        STR      R0,[R1, #+448]
//  577 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock46

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable46:
        DC32     0x3c23d70b

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable46_1:
        DC32     `?<Constant "ignitionOffset=%f">`
//  578 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock47 Using cfiCommon0
          CFI Function _Z9setPotSpii
          CFI NoCalls
        THUMB
//  579 static void setPotSpi(int spi) {
//  580 	boardConfiguration->digitalPotentiometerSpiDevice = (spi_device_e) spi;
_Z9setPotSpii:
        LDR.W    R1,??DataTable71_1
        LDR      R1,[R1, #+0]
        STR      R0,[R1, #+212]
//  581 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock47

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable47:
        DC32     0x42480001

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable47_1:
        DC32     `?<Constant "=== injection ===">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable47_2:
        DC32     `?<Constant "injection %s offset=%...">`
//  582 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock48 Using cfiCommon0
          CFI Function _Z14setIgnitionPinPKcS0_
        THUMB
//  583 static void setIgnitionPin(const char *indexStr, const char *pinName) {
_Z14setIgnitionPinPKcS0_:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  584 	int index = atoi(indexStr) - 1; // convert from human index into software index
        MOVS     R0,R4
          CFI FunCall atoi
        BL       atoi
        SUBS     R6,R0,#+1
//  585 	if (index < 0 || index >= IGNITION_PIN_COUNT)
        CMP      R6,#+0
        BMI.N    ??setIgnitionPin_0
        CMP      R6,#+12
        BLT.N    ??setIgnitionPin_1
//  586 		return;
??setIgnitionPin_0:
        B.N      ??setIgnitionPin_2
//  587 	brain_pin_e pin = parseBrainPin(pinName);
??setIgnitionPin_1:
        MOVS     R0,R5
          CFI FunCall _Z13parseBrainPinPKc
        BL       _Z13parseBrainPinPKc
        MOVS     R7,R0
//  588 	// todo: extract method - code duplication with other 'set_xxx_pin' methods?
//  589 	if (pin == GPIO_INVALID) {
        CMP      R7,#+81
        BNE.N    ??setIgnitionPin_3
//  590 		scheduleMsg(&logger, "invalid pin name [%s]", pinName);
        MOVS     R2,R5
        LDR.W    R1,??DataTable65_10
        LDR.N    R0,??DataTable55_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  591 		return;
        B.N      ??setIgnitionPin_2
//  592 	}
//  593 	scheduleMsg(&logger, "setting ignition pin[%d] to %s please save&restart", index, hwPortname(pin));
??setIgnitionPin_3:
        MOVS     R0,R7
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R3,R0
        MOVS     R2,R6
        LDR.W    R1,??DataTable65_11
        LDR.N    R0,??DataTable55_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  594 	boardConfiguration->ignitionPins[index] = pin;
        LDR.W    R0,??DataTable71_1
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R6, LSL #+2
        STR      R7,[R0, #+84]
//  595 	applyNewConfiguration();
          CFI FunCall applyNewConfiguration
        BL       applyNewConfiguration
//  596 }
??setIgnitionPin_2:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock48

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable48:
        DC32     `?<Constant "map_avg=%s/mil=%s/fp=...">`
//  597 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock49 Using cfiCommon0
          CFI Function _Z16setIndividualPinPKcP11brain_pin_eS0_
        THUMB
//  598 static void setIndividualPin(const char *pinName, brain_pin_e *targetPin, const char *name) {
_Z16setIndividualPinPKcP11brain_pin_eS0_:
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
//  599 	brain_pin_e pin = parseBrainPin(pinName);
        MOVS     R0,R4
          CFI FunCall _Z13parseBrainPinPKc
        BL       _Z13parseBrainPinPKc
        MOVS     R7,R0
//  600 	if (pin == GPIO_INVALID) {
        CMP      R7,#+81
        BNE.N    ??setIndividualPin_0
//  601 		scheduleMsg(&logger, "invalid pin name [%s]", pinName);
        MOVS     R2,R4
        LDR.W    R1,??DataTable65_10
        LDR.W    R0,??DataTable71
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  602 		return;
        B.N      ??setIndividualPin_1
//  603 	}
//  604 	scheduleMsg(&logger, "setting %s pin to %s please save&restart", name, hwPortname(pin));
??setIndividualPin_0:
        MOVS     R0,R7
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R3,R0
        MOVS     R2,R6
        LDR.W    R1,??DataTable65_12
        LDR.W    R0,??DataTable71
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  605 	*targetPin = pin;
        STR      R7,[R5, #+0]
//  606 	applyNewConfiguration();
          CFI FunCall applyNewConfiguration
        BL       applyNewConfiguration
//  607 }
??setIndividualPin_1:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock49
//  608 
//  609 // set_idle_pin none

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock50 Using cfiCommon0
          CFI Function _Z10setIdlePinPKc
        THUMB
//  610 static void setIdlePin(const char *pinName) {
_Z10setIdlePinPKc:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  611 	setIndividualPin(pinName, &boardConfiguration->idle.solenoidPin, "idle");
        LDR.W    R2,??DataTable65_13
        LDR.W    R0,??DataTable71_1
        LDR      R1,[R0, #+0]
        MOVS     R0,R4
          CFI FunCall _Z16setIndividualPinPKcP11brain_pin_eS0_
        BL       _Z16setIndividualPinPKcP11brain_pin_eS0_
//  612 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock50

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable50:
        DC32     `?<Constant "isManualSpinningMode=...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable50_1:
        DC32     engine
//  613 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock51 Using cfiCommon0
          CFI Function _Z15setMainRelayPinPKc
        THUMB
//  614 static void setMainRelayPin(const char *pinName) {
_Z15setMainRelayPinPKc:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  615 	setIndividualPin(pinName, &boardConfiguration->mainRelayPin, "main relay");
        LDR.W    R2,??DataTable65_14
        LDR.W    R0,??DataTable71_1
        LDR      R0,[R0, #+0]
        ADDS     R1,R0,#+308
        MOVS     R0,R4
          CFI FunCall _Z16setIndividualPinPKcP11brain_pin_eS0_
        BL       _Z16setIndividualPinPKcP11brain_pin_eS0_
//  616 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock51

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable51:
        DC32     `?<Constant "clutchUp@%s: %s">`
//  617 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock52 Using cfiCommon0
          CFI Function _Z16setAlternatorPinPKc
        THUMB
//  618 static void setAlternatorPin(const char *pinName) {
_Z16setAlternatorPinPKc:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  619 	setIndividualPin(pinName, &boardConfiguration->alternatorControlPin, "alternator");
        LDR.W    R2,??DataTable66_2
        LDR.W    R0,??DataTable71_1
        LDR      R0,[R0, #+0]
        ADDS     R1,R0,#+168
        MOVS     R0,R4
          CFI FunCall _Z16setIndividualPinPKcP11brain_pin_eS0_
        BL       _Z16setIndividualPinPKcP11brain_pin_eS0_
//  620 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock52

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable52:
        DC32     `?<Constant "UnknownType">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable52_1:
        DC32     `?<Constant "clutchDown@%s: %s">`
//  621 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock53 Using cfiCommon0
          CFI Function _Z8setACPinPKc
        THUMB
//  622 static void setACPin(const char *pinName) {
_Z8setACPinPKc:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  623 	setIndividualPin(pinName, &boardConfiguration->acRelayPin, "A/C");
        ADR.N    R2,??DataTable59  ;; "A/C"
        LDR.W    R0,??DataTable71_1
        LDR      R0,[R0, #+0]
        ADDS     R1,R0,#+608
        MOVS     R0,R4
          CFI FunCall _Z16setIndividualPinPKcP11brain_pin_eS0_
        BL       _Z16setIndividualPinPKcP11brain_pin_eS0_
//  624 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock53

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable53:
        DC32     maxNesting

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable53_1:
        DC32     `?<Constant "boardTestModeJumperPi...">`
//  625 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock54 Using cfiCommon0
          CFI Function _Z14setFuelPumpPinPKc
        THUMB
//  626 static void setFuelPumpPin(const char *pinName) {
_Z14setFuelPumpPinPKc:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  627 	setIndividualPin(pinName, &boardConfiguration->fuelPumpPin, "fuelPump");
        LDR.W    R2,??DataTable66_3
        LDR.W    R0,??DataTable71_1
        LDR      R0,[R0, #+0]
        ADDS     R1,R0,#+24
        MOVS     R0,R4
          CFI FunCall _Z16setIndividualPinPKcP11brain_pin_eS0_
        BL       _Z16setIndividualPinPKcP11brain_pin_eS0_
//  628 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock54

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable54:
        DC32     `?<Constant "digitalPotentiometerS...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable54_1:
        DC32     `?<Constant "digitalPotentiometer ...">`
//  629 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock55 Using cfiCommon0
          CFI Function _Z15setInjectionPinPKcS0_
        THUMB
//  630 static void setInjectionPin(const char *indexStr, const char *pinName) {
_Z15setInjectionPinPKcS0_:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  631 	int index = atoi(indexStr) - 1; // convert from human index into software index
        MOVS     R0,R4
          CFI FunCall atoi
        BL       atoi
        SUBS     R6,R0,#+1
//  632 	if (index < 0 || index >= INJECTION_PIN_COUNT)
        CMP      R6,#+0
        BMI.N    ??setInjectionPin_0
        CMP      R6,#+12
        BLT.N    ??setInjectionPin_1
//  633 		return;
??setInjectionPin_0:
        B.N      ??setInjectionPin_2
//  634 	brain_pin_e pin = parseBrainPin(pinName);
??setInjectionPin_1:
        MOVS     R0,R5
          CFI FunCall _Z13parseBrainPinPKc
        BL       _Z13parseBrainPinPKc
        MOVS     R7,R0
//  635 	// todo: extract method - code duplication with other 'set_xxx_pin' methods?
//  636 	if (pin == GPIO_INVALID) {
        CMP      R7,#+81
        BNE.N    ??setInjectionPin_3
//  637 		scheduleMsg(&logger, "invalid pin name [%s]", pinName);
        MOVS     R2,R5
        LDR.W    R1,??DataTable65_10
        LDR.N    R0,??DataTable55_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  638 		return;
        B.N      ??setInjectionPin_2
//  639 	}
//  640 	scheduleMsg(&logger, "setting injection pin[%d] to %s please save&restart", index, hwPortname(pin));
??setInjectionPin_3:
        MOVS     R0,R7
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R3,R0
        MOVS     R2,R6
        LDR.W    R1,??DataTable68
        LDR.N    R0,??DataTable55_1
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  641 	boardConfiguration->injectionPins[index] = pin;
        LDR.W    R0,??DataTable75
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R6, LSL #+2
        STR      R7,[R0, #+32]
//  642 	applyNewConfiguration();
          CFI FunCall applyNewConfiguration
        BL       applyNewConfiguration
//  643 }
??setInjectionPin_2:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock55

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable55:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable55_1:
        DC32     logger
//  644 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock56 Using cfiCommon0
          CFI Function _Z18setTriggerInputPinPKcS0_
        THUMB
//  645 static void setTriggerInputPin(const char *indexStr, const char *pinName) {
_Z18setTriggerInputPinPKcS0_:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  646 	int index = atoi(indexStr);
        MOVS     R0,R4
          CFI FunCall atoi
        BL       atoi
        MOVS     R6,R0
//  647 	if (index < 0 || index > 2)
        CMP      R6,#+0
        BMI.N    ??setTriggerInputPin_0
        CMP      R6,#+3
        BLT.N    ??setTriggerInputPin_1
//  648 		return;
??setTriggerInputPin_0:
        B.N      ??setTriggerInputPin_2
//  649 	brain_pin_e pin = parseBrainPin(pinName);
??setTriggerInputPin_1:
        MOVS     R0,R5
          CFI FunCall _Z13parseBrainPinPKc
        BL       _Z13parseBrainPinPKc
        MOVS     R7,R0
//  650 	// todo: extract method - code duplication with other 'set_xxx_pin' methods?
//  651 	if (pin == GPIO_INVALID) {
        CMP      R7,#+81
        BNE.N    ??setTriggerInputPin_3
//  652 		scheduleMsg(&logger, "invalid pin name [%s]", pinName);
        MOVS     R2,R5
        LDR.W    R1,??DataTable65_10
        LDR.W    R0,??DataTable71
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  653 		return;
        B.N      ??setTriggerInputPin_2
//  654 	}
//  655 	scheduleMsg(&logger, "setting trigger pin[%d] to %s please save&restart", index, hwPortname(pin));
??setTriggerInputPin_3:
        MOVS     R0,R7
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R3,R0
        MOVS     R2,R6
        LDR.W    R1,??DataTable71_3
        LDR.W    R0,??DataTable71
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  656 	boardConfiguration->triggerInputPins[index] = pin;
        LDR.W    R0,??DataTable75
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R6, LSL #+2
        STR      R7,[R0, #+296]
//  657 	applyNewConfiguration();
          CFI FunCall applyNewConfiguration
        BL       applyNewConfiguration
//  658 }
??setTriggerInputPin_2:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock56

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable56:
        DC32     waveChart
//  659 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock57 Using cfiCommon0
          CFI Function _Z23setTriggerSimulatorModePKcS0_
        THUMB
//  660 static void setTriggerSimulatorMode(const char *indexStr, const char *modeCode) {
_Z23setTriggerSimulatorModePKcS0_:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  661 	int index = atoi(indexStr);
        MOVS     R0,R4
          CFI FunCall atoi
        BL       atoi
        MOVS     R6,R0
//  662 	if (index < 0 || index >= TRIGGER_SIMULATOR_PIN_COUNT || absI(index) == ERROR_CODE) {
        CMP      R6,#+0
        BMI.N    ??setTriggerSimulatorMode_0
        CMP      R6,#+3
        BGE.N    ??setTriggerSimulatorMode_0
        MOVS     R0,R6
          CFI FunCall absI
        BL       absI
        LDR.W    R1,??DataTable71_4  ;; 0x128ce430
        CMP      R0,R1
        BNE.N    ??setTriggerSimulatorMode_1
//  663 		return;
??setTriggerSimulatorMode_0:
        B.N      ??setTriggerSimulatorMode_2
//  664 	}
//  665 	int mode = atoi(modeCode);
??setTriggerSimulatorMode_1:
        MOVS     R0,R5
          CFI FunCall atoi
        BL       atoi
        MOVS     R7,R0
//  666 	if (absI(mode) == ERROR_CODE) {
        MOVS     R0,R7
          CFI FunCall absI
        BL       absI
        LDR.W    R1,??DataTable71_4  ;; 0x128ce430
        CMP      R0,R1
        BEQ.N    ??setTriggerSimulatorMode_2
//  667 		return;
//  668 	}
//  669 	boardConfiguration->triggerSimulatorPinModes[index] = (pin_output_mode_e) mode;
??setTriggerSimulatorMode_3:
        LDR.W    R0,??DataTable75
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R6, LSL #+2
        STR      R7,[R0, #+364]
//  670 }
??setTriggerSimulatorMode_2:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock57
//  671 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock58 Using cfiCommon0
          CFI Function _Z11setEgtCSPinPKcS0_P21board_configuration_s
        THUMB
//  672 static void setEgtCSPin(const char *indexStr, const char *pinName, board_configuration_s * board_configuration_s) {
_Z11setEgtCSPinPKcS0_P21board_configuration_s:
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
//  673 	int index = atoi(indexStr);
        MOVS     R0,R4
          CFI FunCall atoi
        BL       atoi
        MOVS     R7,R0
//  674 	if (index < 0 || index >= EGT_CHANNEL_COUNT || absI(index) == ERROR_CODE)
        CMP      R7,#+0
        BMI.N    ??setEgtCSPin_0
        CMP      R7,#+8
        BGE.N    ??setEgtCSPin_0
        MOVS     R0,R7
          CFI FunCall absI
        BL       absI
        LDR.W    R1,??DataTable71_4  ;; 0x128ce430
        CMP      R0,R1
        BNE.N    ??setEgtCSPin_1
//  675 		return;
??setEgtCSPin_0:
        B.N      ??setEgtCSPin_2
//  676 	brain_pin_e pin = parseBrainPin(pinName);
??setEgtCSPin_1:
        MOVS     R0,R5
          CFI FunCall _Z13parseBrainPinPKc
        BL       _Z13parseBrainPinPKc
        MOV      R8,R0
//  677 	if (pin == GPIO_INVALID) {
        CMP      R8,#+81
        BNE.N    ??setEgtCSPin_3
//  678 		scheduleMsg(&logger, "invalid pin name [%s]", pinName);
        MOVS     R2,R5
        LDR.W    R1,??DataTable65_10
        LDR.W    R0,??DataTable71
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  679 		return;
        B.N      ??setEgtCSPin_2
//  680 	}
//  681 	scheduleMsg(&logger, "setting EGT CS pin[%d] to %s please save&restart", index, hwPortname(pin));
??setEgtCSPin_3:
        MOV      R0,R8
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R3,R0
        MOVS     R2,R7
        LDR.W    R1,??DataTable74
        LDR.W    R0,??DataTable71
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  682 	boardConfiguration->max31855_cs[index] = pin;
        LDR.W    R0,??DataTable75
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R7, LSL #+2
        STR      R8,[R0, #+416]
//  683 	applyNewConfiguration();
          CFI FunCall applyNewConfiguration
        BL       applyNewConfiguration
//  684 }
??setEgtCSPin_2:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock58
//  685 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock59 Using cfiCommon0
          CFI Function _Z22setTriggerSimulatorPinPKcS0_
        THUMB
//  686 static void setTriggerSimulatorPin(const char *indexStr, const char *pinName) {
_Z22setTriggerSimulatorPinPKcS0_:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  687 	int index = atoi(indexStr);
        MOVS     R0,R4
          CFI FunCall atoi
        BL       atoi
        MOVS     R6,R0
//  688 	if (index < 0 || index >= TRIGGER_SIMULATOR_PIN_COUNT || absI(index) == ERROR_CODE)
        CMP      R6,#+0
        BMI.N    ??setTriggerSimulatorPin_0
        CMP      R6,#+3
        BGE.N    ??setTriggerSimulatorPin_0
        MOVS     R0,R6
          CFI FunCall absI
        BL       absI
        LDR.W    R1,??DataTable71_4  ;; 0x128ce430
        CMP      R0,R1
        BNE.N    ??setTriggerSimulatorPin_1
//  689 		return;
??setTriggerSimulatorPin_0:
        B.N      ??setTriggerSimulatorPin_2
//  690 	brain_pin_e pin = parseBrainPin(pinName);
??setTriggerSimulatorPin_1:
        MOVS     R0,R5
          CFI FunCall _Z13parseBrainPinPKc
        BL       _Z13parseBrainPinPKc
        MOVS     R7,R0
//  691 	if (pin == GPIO_INVALID) {
        CMP      R7,#+81
        BNE.N    ??setTriggerSimulatorPin_3
//  692 		scheduleMsg(&logger, "invalid pin name [%s]", pinName);
        MOVS     R2,R5
        LDR.W    R1,??DataTable65_10
        LDR.W    R0,??DataTable71
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  693 		return;
        B.N      ??setTriggerSimulatorPin_2
//  694 	}
//  695 	scheduleMsg(&logger, "setting trigger simulator pin[%d] to %s please save&restart", index, hwPortname(pin));
??setTriggerSimulatorPin_3:
        MOVS     R0,R7
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R3,R0
        MOVS     R2,R6
        LDR.W    R1,??DataTable74_1
        LDR.W    R0,??DataTable71
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  696 	boardConfiguration->triggerSimulatorPins[index] = pin;
        LDR.W    R0,??DataTable75
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R6, LSL #+2
        STR      R7,[R0, #+352]
//  697 	applyNewConfiguration();
          CFI FunCall applyNewConfiguration
        BL       applyNewConfiguration
//  698 }
??setTriggerSimulatorPin_2:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock59

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable59:
        DC8      "A/C"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable59_1:
        DC32     `?<Constant "term">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable59_2:
        DC32     `?<Constant "%s volts=%f Celsius=%...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable59_3:
        DC32     pinNameBuffer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable59_4:
        DC32     `?<Constant "C=%f/R=%f C=%f/R=%f C...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable59_5:
        DC32     `?<Constant "bias resistor=%fK A=%...">`
//  699 
//  700 #if HAL_USE_ADC || defined(__DOXYGEN__)

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock60 Using cfiCommon0
          CFI Function _Z17setAnalogInputPinPKcS0_
        THUMB
//  701 static void setAnalogInputPin(const char *sensorStr, const char *pinName) {
_Z17setAnalogInputPinPKcS0_:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  702 	brain_pin_e pin = parseBrainPin(pinName);
        MOVS     R0,R5
          CFI FunCall _Z13parseBrainPinPKc
        BL       _Z13parseBrainPinPKc
        MOVS     R6,R0
//  703 	if (pin == GPIO_INVALID) {
        CMP      R6,#+81
        BNE.N    ??setAnalogInputPin_0
//  704 		scheduleMsg(&logger, "invalid pin name [%s]", pinName);
        MOVS     R2,R5
        LDR.W    R1,??DataTable65_10
        LDR.W    R0,??DataTable71
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  705 		return;
        B.N      ??setAnalogInputPin_1
//  706 	}
//  707 	adc_channel_e channel = getAdcChannel(pin);
??setAnalogInputPin_0:
        MOVS     R0,R6
          CFI FunCall _Z13getAdcChannel11brain_pin_e
        BL       _Z13getAdcChannel11brain_pin_e
        MOVS     R7,R0
//  708 	if (channel == EFI_ADC_ERROR) {
        MOVW     R0,#+999
        CMP      R7,R0
        BNE.N    ??setAnalogInputPin_2
//  709 		scheduleMsg(&logger, "Error with [%s]", pinName);
        MOVS     R2,R5
        LDR.W    R1,??DataTable74_2
        LDR.W    R0,??DataTable71
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  710 		return;
        B.N      ??setAnalogInputPin_1
//  711 	}
//  712 	if (strEqual("map", sensorStr)) {
??setAnalogInputPin_2:
        MOVS     R1,R4
        ADR.N    R0,??DataTable64  ;; "map"
          CFI FunCall strEqual
        BL       strEqual
        CMP      R0,#+0
        BEQ.N    ??setAnalogInputPin_3
//  713 		engineConfiguration->map.sensor.hwChannel = channel;
        LDR.W    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        STR      R7,[R0, #+252]
//  714 		scheduleMsg(&logger, "setting MAP to %s/%d", pinName, channel);
        MOVS     R3,R7
        MOVS     R2,R5
        LDR.W    R1,??DataTable75_2
        LDR.W    R0,??DataTable71
          CFI FunCall scheduleMsg
        BL       scheduleMsg
        B.N      ??setAnalogInputPin_4
//  715 	} else if (strEqual("clt", sensorStr)) {
??setAnalogInputPin_3:
        MOVS     R1,R4
        ADR.N    R0,??DataTable64_1  ;; "clt"
          CFI FunCall strEqual
        BL       strEqual
        CMP      R0,#+0
        BEQ.N    ??setAnalogInputPin_5
//  716 		engineConfiguration->clt.adcChannel = channel;
        LDR.W    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        STR      R7,[R0, #+284]
//  717 		scheduleMsg(&logger, "setting CLT to %s/%d", pinName, channel);
        MOVS     R3,R7
        MOVS     R2,R5
        LDR.W    R1,??DataTable75_3
        LDR.W    R0,??DataTable71
          CFI FunCall scheduleMsg
        BL       scheduleMsg
        B.N      ??setAnalogInputPin_4
//  718 	} else if (strEqual("iat", sensorStr)) {
??setAnalogInputPin_5:
        MOVS     R1,R4
        ADR.N    R0,??DataTable64_2  ;; "iat"
          CFI FunCall strEqual
        BL       strEqual
        CMP      R0,#+0
        BEQ.N    ??setAnalogInputPin_6
//  719 		engineConfiguration->iat.adcChannel = channel;
        LDR.W    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        STR      R7,[R0, #+316]
//  720 		scheduleMsg(&logger, "setting IAT to %s/%d", pinName, channel);
        MOVS     R3,R7
        MOVS     R2,R5
        LDR.W    R1,??DataTable75_4
        LDR.W    R0,??DataTable71
          CFI FunCall scheduleMsg
        BL       scheduleMsg
        B.N      ??setAnalogInputPin_4
//  721 	} else if (strEqual("tps", sensorStr)) {
??setAnalogInputPin_6:
        MOVS     R1,R4
        ADR.N    R0,??DataTable64_3  ;; "tps"
          CFI FunCall strEqual
        BL       strEqual
        CMP      R0,#+0
        BEQ.N    ??setAnalogInputPin_4
//  722 		engineConfiguration->tpsAdcChannel = channel;
        LDR.W    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        STR      R7,[R0, #+524]
//  723 		scheduleMsg(&logger, "setting TPS to %s/%d", pinName, channel);
        MOVS     R3,R7
        MOVS     R2,R5
        LDR.W    R1,??DataTable75_5
        LDR.W    R0,??DataTable71
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  724 	}
//  725 	applyNewConfiguration();
??setAnalogInputPin_4:
          CFI FunCall applyNewConfiguration
        BL       applyNewConfiguration
//  726 }
??setAnalogInputPin_1:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock60

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable60:
        DC32     `?<Constant "=====================...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable60_1:
        DC32     `?<Constant "NO TPS SENSOR">`
//  727 #endif
//  728 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock61 Using cfiCommon0
          CFI Function _Z16setLogicInputPinPKcS0_
        THUMB
//  729 static void setLogicInputPin(const char *indexStr, const char *pinName) {
_Z16setLogicInputPinPKcS0_:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  730 	int index = atoi(indexStr);
        MOVS     R0,R4
          CFI FunCall atoi
        BL       atoi
        MOVS     R6,R0
//  731 	if (index < 0 || index > 2) {
        CMP      R6,#+0
        BMI.N    ??setLogicInputPin_0
        CMP      R6,#+3
        BLT.N    ??setLogicInputPin_1
//  732 		return;
??setLogicInputPin_0:
        B.N      ??setLogicInputPin_2
//  733 	}
//  734 	brain_pin_e pin = parseBrainPin(pinName);
??setLogicInputPin_1:
        MOVS     R0,R5
          CFI FunCall _Z13parseBrainPinPKc
        BL       _Z13parseBrainPinPKc
        MOVS     R7,R0
//  735 	if (pin == GPIO_INVALID) {
        CMP      R7,#+81
        BNE.N    ??setLogicInputPin_3
//  736 		scheduleMsg(&logger, "invalid pin name [%s]", pinName);
        MOVS     R2,R5
        LDR.N    R1,??DataTable65_10
        LDR.W    R0,??DataTable71
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  737 		return;
        B.N      ??setLogicInputPin_2
//  738 	}
//  739 	scheduleMsg(&logger, "setting logic input pin[%d] to %s please save&restart", index, hwPortname(pin));
??setLogicInputPin_3:
        MOVS     R0,R7
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R3,R0
        MOVS     R2,R6
        LDR.W    R1,??DataTable75_6
        LDR.W    R0,??DataTable71
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  740 	boardConfiguration->logicAnalyzerPins[index] = pin;
        LDR.W    R0,??DataTable75
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R6, LSL #+2
        STR      R7,[R0, #+388]
//  741 }
??setLogicInputPin_2:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock61

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable61:
        DC32     `?<Constant "tps min %d/max %d v=%...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable61_1:
        DC32     `?<Constant "current 10bit=%d valu...">`
//  742 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock62 Using cfiCommon0
          CFI Function _Z15showPinFunctionPKc
        THUMB
//  743 static void showPinFunction(const char *pinName) {
_Z15showPinFunctionPKc:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  744 	brain_pin_e pin = parseBrainPin(pinName);
        MOVS     R0,R4
          CFI FunCall _Z13parseBrainPinPKc
        BL       _Z13parseBrainPinPKc
        MOVS     R5,R0
//  745 	if (pin == GPIO_INVALID) {
        CMP      R5,#+81
        BNE.N    ??showPinFunction_0
//  746 		scheduleMsg(&logger, "invalid pin name [%s]", pinName);
        MOVS     R2,R4
        LDR.N    R1,??DataTable65_10
        LDR.W    R0,??DataTable71
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  747 		return;
        B.N      ??showPinFunction_1
//  748 	}
//  749 	scheduleMsg(&logger, "Pin %s: [%s]", pinName, getPinFunction(pin));
??showPinFunction_0:
        MOVS     R0,R5
          CFI FunCall _Z14getPinFunction11brain_pin_e
        BL       _Z14getPinFunction11brain_pin_e
        MOVS     R3,R0
        MOVS     R2,R4
        LDR.W    R1,??DataTable75_7
        LDR.W    R0,??DataTable71
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  750 }
??showPinFunction_1:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock62

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable62:
        DC32     `?<Constant "CLT sensing error">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable62_1:
        DC32     `?<Constant "IAT sensing error">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable62_2:
        DC32     enginePins+0x10
//  751 
//  752 #endif /* EFI_PROD_CODE */
//  753 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock63 Using cfiCommon0
          CFI Function _Z12setTimingMapPKcS0_S0_
        THUMB
//  754 static void setTimingMap(const char * rpmStr, const char *loadStr, const char *valueStr) {
_Z12setTimingMapPKcS0_S0_:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        VPUSH    {D8-D9}
          CFI D9 Frame(CFA, -32)
          CFI D8 Frame(CFA, -40)
          CFI CFA R13+40
        SUB      SP,SP,#+8
          CFI CFA R13+48
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  755 	float rpm = atoff(rpmStr);
        MOVS     R0,R4
          CFI FunCall atoff
        BL       atoff
        VMOV.F32 S16,S0
//  756 	float engineLoad = atoff(loadStr);
        MOVS     R0,R5
          CFI FunCall atoff
        BL       atoff
        VMOV.F32 S17,S0
//  757 	float value = atoff(valueStr);
        MOVS     R0,R6
          CFI FunCall atoff
        BL       atoff
        VMOV.F32 S18,S0
//  758 
//  759 	int rpmIndex = findIndex(config->ignitionRpmBins, IGN_RPM_COUNT, rpm);
        VMOV.F32 S0,S16
        MOVS     R1,#+16
        LDR.W    R0,??DataTable66
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+12800
        ADDS     R0,R0,#+120
          CFI FunCall _Z9findIndexPKfif
        BL       _Z9findIndexPKfif
        MOVS     R7,R0
//  760 	rpmIndex = rpmIndex < 0 ? 0 : rpmIndex;
        CMP      R7,#+0
        BPL.N    ??setTimingMap_0
        MOVS     R7,#+0
        B.N      ??setTimingMap_1
//  761 	int loadIndex = findIndex(config->ignitionLoadBins, IGN_LOAD_COUNT, engineLoad);
??setTimingMap_0:
??setTimingMap_1:
        VMOV.F32 S0,S17
        MOVS     R1,#+16
        LDR.W    R0,??DataTable66
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+12800
        ADDS     R0,R0,#+56
          CFI FunCall _Z9findIndexPKfif
        BL       _Z9findIndexPKfif
        MOV      R8,R0
//  762 	loadIndex = loadIndex < 0 ? 0 : loadIndex;
        CMP      R8,#+0
        BPL.N    ??setTimingMap_2
        MOVS     R8,#+0
        B.N      ??setTimingMap_3
//  763 
//  764 	config->ignitionTable[loadIndex][rpmIndex] = value;
??setTimingMap_2:
??setTimingMap_3:
        MOVS     R0,#+64
        LDR.W    R1,??DataTable66
        LDR      R1,[R1, #+0]
        MLA      R0,R0,R8,R1
        ADDS     R0,R0,R7, LSL #+2
        ADD      R0,R0,#+11776
        ADDS     R0,R0,#+56
        VSTR     S18,[R0, #0]
//  765 	scheduleMsg(&logger, "Setting timing map entry %d:%d to %f", rpmIndex, loadIndex, value);
        VMOV     R0,S18
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        MOV      R3,R8
        MOVS     R2,R7
        LDR.W    R1,??DataTable75_8
        LDR.W    R0,??DataTable76
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  766 }
        ADD      SP,SP,#+8
          CFI CFA R13+40
        VPOP     {D8-D9}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI CFA R13+24
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock63

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable63:
        DC32     `?<Constant "fan=%s @ %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable63_1:
        DC32     enginePins+0x20

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable63_2:
        DC32     `?<Constant "A/C relay=%s @ %s">`
//  767 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock64 Using cfiCommon0
          CFI Function _Z10setFuelMapPKcS0_S0_
        THUMB
//  768 static void setFuelMap(const char * rpmStr, const char *loadStr, const char *valueStr) {
_Z10setFuelMapPKcS0_S0_:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        VPUSH    {D8-D9}
          CFI D9 Frame(CFA, -32)
          CFI D8 Frame(CFA, -40)
          CFI CFA R13+40
        SUB      SP,SP,#+8
          CFI CFA R13+48
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  769 	float rpm = atoff(rpmStr);
        MOVS     R0,R4
          CFI FunCall atoff
        BL       atoff
        VMOV.F32 S16,S0
//  770 	float engineLoad = atoff(loadStr);
        MOVS     R0,R5
          CFI FunCall atoff
        BL       atoff
        VMOV.F32 S17,S0
//  771 	float value = atoff(valueStr);
        MOVS     R0,R6
          CFI FunCall atoff
        BL       atoff
        VMOV.F32 S18,S0
//  772 
//  773 	int rpmIndex = findIndex(config->fuelRpmBins, FUEL_RPM_COUNT, rpm);
        VMOV.F32 S0,S16
        MOVS     R1,#+16
        LDR.W    R0,??DataTable66
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+11520
        ADDS     R0,R0,#+248
          CFI FunCall _Z9findIndexPKfif
        BL       _Z9findIndexPKfif
        MOVS     R7,R0
//  774 	rpmIndex = rpmIndex < 0 ? 0 : rpmIndex;
        CMP      R7,#+0
        BPL.N    ??setFuelMap_0
        MOVS     R7,#+0
        B.N      ??setFuelMap_1
//  775 	int loadIndex = findIndex(config->fuelLoadBins, FUEL_LOAD_COUNT, engineLoad);
??setFuelMap_0:
??setFuelMap_1:
        VMOV.F32 S0,S17
        MOVS     R1,#+16
        LDR.W    R0,??DataTable66
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+11520
        ADDS     R0,R0,#+184
          CFI FunCall _Z9findIndexPKfif
        BL       _Z9findIndexPKfif
        MOV      R8,R0
//  776 	loadIndex = loadIndex < 0 ? 0 : loadIndex;
        CMP      R8,#+0
        BPL.N    ??setFuelMap_2
        MOVS     R8,#+0
        B.N      ??setFuelMap_3
//  777 
//  778 	config->fuelTable[loadIndex][rpmIndex] = value;
??setFuelMap_2:
??setFuelMap_3:
        MOVS     R0,#+64
        LDR.W    R1,??DataTable66
        LDR      R1,[R1, #+0]
        MLA      R0,R0,R8,R1
        ADDS     R0,R0,R7, LSL #+2
        ADD      R0,R0,#+10496
        ADDS     R0,R0,#+184
        VSTR     S18,[R0, #0]
//  779 	scheduleMsg(&logger, "Setting fuel map entry %d:%d to %f", rpmIndex, loadIndex, value);
        VMOV     R0,S18
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        MOV      R3,R8
        MOVS     R2,R7
        LDR.W    R1,??DataTable75_9
        LDR.W    R0,??DataTable76
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  780 }
        ADD      SP,SP,#+8
          CFI CFA R13+40
        VPOP     {D8-D9}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI CFA R13+24
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock64

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable64:
        DC8      "map"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable64_1:
        DC8      "clt"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable64_2:
        DC8      "iat"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable64_3:
        DC8      "tps"
//  781 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock65 Using cfiCommon0
          CFI Function _Z10setSpiModeib
        THUMB
//  782 static void setSpiMode(int index, bool mode) {
_Z10setSpiModeib:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  783 	switch (index) {
        CMP      R4,#+1
        BEQ.N    ??setSpiMode_0
        BCC.N    ??setSpiMode_1
        CMP      R4,#+3
        BEQ.N    ??setSpiMode_2
        BCC.N    ??setSpiMode_3
        B.N      ??setSpiMode_1
//  784 	case 1:
//  785 		boardConfiguration->is_enabled_spi_1 = mode;
??setSpiMode_0:
        LDR.W    R0,??DataTable71_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        BFI      R0,R5,#+0,#+1
        LDR.W    R1,??DataTable71_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+384]
//  786 		break;
        B.N      ??setSpiMode_4
//  787 	case 2:
//  788 		boardConfiguration->is_enabled_spi_2 = mode;
??setSpiMode_3:
        LDR.W    R0,??DataTable71_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        BFI      R0,R5,#+1,#+1
        LDR.W    R1,??DataTable71_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+384]
//  789 		break;
        B.N      ??setSpiMode_4
//  790 	case 3:
//  791 		boardConfiguration->is_enabled_spi_3 = mode;
??setSpiMode_2:
        LDR.W    R0,??DataTable71_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        BFI      R0,R5,#+2,#+1
        LDR.W    R1,??DataTable71_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+384]
//  792 		break;
        B.N      ??setSpiMode_4
//  793 	default:
//  794 		scheduleMsg(&logger, "invalid spi index %d", index);
??setSpiMode_1:
        MOVS     R2,R4
        LDR.W    R1,??DataTable75_10
        LDR.W    R0,??DataTable76
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  795 		return;
        B.N      ??setSpiMode_5
//  796 	}
//  797 	printSpiState(&logger, boardConfiguration);
??setSpiMode_4:
        LDR.W    R0,??DataTable71_1
        LDR      R1,[R0, #+0]
        LDR.W    R0,??DataTable76
          CFI FunCall _Z13printSpiStateP7LoggingP21board_configuration_s
        BL       _Z13printSpiStateP7LoggingP21board_configuration_s
//  798 }
??setSpiMode_5:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock65

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable65:
        DC32     `?<Constant "toothed: total=%d/ski...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable65_1:
        DC32     `?<Constant "setting fuel mult=%f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable65_2:
        DC32     `?<Constant "ON temp [%f] should b...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable65_3:
        DC32     `?<Constant "Setting whole timing ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable65_4:
        DC32     `?<Constant "Setting whole injecti...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable65_5:
        DC32     `?<Constant "Setting whole timing ...">_1`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable65_6:
        DC32     `?<Constant "Setting whole VE map ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable65_7:
        DC32     `?<Constant "WARNING: setting VE m...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable65_8:
        DC32     `?<Constant "Setting whole fuel ma...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable65_9:
        DC32     `?<Constant "WARNING: setting fuel...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable65_10:
        DC32     `?<Constant "invalid pin name [%s]">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable65_11:
        DC32     `?<Constant "setting ignition pin[...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable65_12:
        DC32     `?<Constant "setting %s pin to %s ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable65_13:
        DC32     `?<Constant "idle">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable65_14:
        DC32     `?<Constant "main relay">`
//  799 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock66 Using cfiCommon0
          CFI Function _Z15enableOrDisablePKcb
        THUMB
//  800 static void enableOrDisable(const char *param, bool isEnabled) {
_Z15enableOrDisablePKcb:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  801 	if (strEqualCaseInsensitive(param, "fastadc")) {
        LDR.W    R1,??DataTable75_11
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_0
//  802 		boardConfiguration->isFastAdcEnabled = isEnabled;
        LDR.W    R0,??DataTable71_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        BFI      R0,R5,#+4,#+1
        LDR.W    R1,??DataTable71_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+384]
        B.N      ??enableOrDisable_1
//  803 	} else if (strEqualCaseInsensitive(param, "step1limimter")) {
??enableOrDisable_0:
        LDR.W    R1,??DataTable75_12
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_2
//  804 		boardConfiguration->enabledStep1Limiter = isEnabled;
        LDR.W    R0,??DataTable71_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+385]
        BFI      R0,R5,#+2,#+1
        LDR.W    R1,??DataTable71_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+385]
        B.N      ??enableOrDisable_1
//  805 	} else if (strEqualCaseInsensitive(param, "serial")) {
??enableOrDisable_2:
        LDR.W    R1,??DataTable75_13
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_3
//  806 		boardConfiguration->useSerialPort = isEnabled;
        LDR.W    R0,??DataTable71_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+385]
        BFI      R0,R5,#+0,#+1
        LDR.W    R1,??DataTable71_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+385]
        B.N      ??enableOrDisable_1
//  807 	} else if (strEqualCaseInsensitive(param, "stepperidle")) {
??enableOrDisable_3:
        LDR.W    R1,??DataTable75_14
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_4
//  808 		boardConfiguration->useStepperIdle = isEnabled;
        LDR.W    R0,??DataTable71_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+385]
        BFI      R0,R5,#+1,#+1
        LDR.W    R1,??DataTable71_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+385]
        B.N      ??enableOrDisable_1
//  809 	} else if (strEqualCaseInsensitive(param, "trigger_only_front")) {
??enableOrDisable_4:
        LDR.W    R1,??DataTable75_15
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_5
//  810 		engineConfiguration->useOnlyFrontForTrigger = isEnabled;
        LDR.W    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        BFI      R0,R5,#+6,#+1
        LDR.W    R1,??DataTable75_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1509]
//  811 		incrementGlobalConfigurationVersion();
          CFI FunCall _Z35incrementGlobalConfigurationVersionv
        BL       _Z35incrementGlobalConfigurationVersionv
        B.N      ??enableOrDisable_1
//  812 	} else if (strEqualCaseInsensitive(param, "two_wire_batch")) {
??enableOrDisable_5:
        LDR.W    R1,??DataTable75_16
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_6
//  813 		engineConfiguration->twoWireBatch = isEnabled;
        LDR.W    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        BFI      R0,R5,#+5,#+1
        LDR.W    R1,??DataTable75_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1509]
//  814 		incrementGlobalConfigurationVersion();
          CFI FunCall _Z35incrementGlobalConfigurationVersionv
        BL       _Z35incrementGlobalConfigurationVersionv
        B.N      ??enableOrDisable_1
//  815 	} else if (strEqualCaseInsensitive(param, "joystick")) {
??enableOrDisable_6:
        LDR.W    R1,??DataTable75_17
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_7
//  816 		engineConfiguration->isJoystickEnabled = isEnabled;
        LDR.W    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        BFI      R0,R5,#+3,#+1
        LDR.W    R1,??DataTable75_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1497]
        B.N      ??enableOrDisable_1
//  817 	} else if (strEqualCaseInsensitive(param, "HIP9011")) {
??enableOrDisable_7:
        LDR.W    R1,??DataTable75_18
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_8
//  818 		boardConfiguration->isHip9011Enabled = isEnabled;
        LDR.W    R0,??DataTable71_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        BFI      R0,R5,#+6,#+1
        LDR.W    R1,??DataTable71_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+384]
        B.N      ??enableOrDisable_1
//  819 	} else if (strEqualCaseInsensitive(param, "altdebug")) {
??enableOrDisable_8:
        LDR.W    R1,??DataTable75_19
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_9
//  820 		boardConfiguration->isVerboseAlternator = isEnabled;
        LDR.W    R0,??DataTable71_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        ANDS     R0,R0,#0x7F
        ORRS     R0,R0,R5, LSL #+7
        LDR.W    R1,??DataTable71_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+384]
        B.N      ??enableOrDisable_1
//  821 	} else if (strEqualCaseInsensitive(param, "tpic_advanced_mode")) {
??enableOrDisable_9:
        LDR.W    R1,??DataTable75_20
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_10
//  822 		boardConfiguration->useTpicAdvancedMode = isEnabled;
        LDR.W    R0,??DataTable71_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+385]
        BFI      R0,R5,#+3,#+1
        LDR.W    R1,??DataTable71_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+385]
        B.N      ??enableOrDisable_1
//  823 	} else if (strEqualCaseInsensitive(param, "knockdebug")) {
??enableOrDisable_10:
        LDR.W    R1,??DataTable75_21
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_11
//  824 		engine->knockDebug = isEnabled;
        LDR.W    R0,??DataTable75_22
        LDR      R0,[R0, #+0]
        STRB     R5,[R0, #+624]
        B.N      ??enableOrDisable_1
//  825 	} else if (strEqualCaseInsensitive(param, "altcontrol")) {
??enableOrDisable_11:
        LDR.W    R1,??DataTable75_23
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_12
//  826 		engineConfiguration->isAlternatorControlEnabled = isEnabled;
        LDR.W    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        BFI      R0,R5,#+5,#+1
        LDR.W    R1,??DataTable75_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1497]
        B.N      ??enableOrDisable_1
//  827 	} else if (strEqualCaseInsensitive(param, "sd")) {
??enableOrDisable_12:
        ADR.N    R1,??DataTable66_1  ;; 0x73, 0x64, 0x00, 0x00
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_13
//  828 		boardConfiguration->isSdCardEnabled = isEnabled;
        LDR.W    R0,??DataTable71_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        BFI      R0,R5,#+3,#+1
        LDR.W    R1,??DataTable71_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+384]
        B.N      ??enableOrDisable_1
//  829 	} else if (strEqualCaseInsensitive(param, "can")) {
??enableOrDisable_13:
        ADR.N    R1,??DataTable67  ;; "can"
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_14
//  830 		engineConfiguration->isCanEnabled = isEnabled;
        LDR.W    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        BFI      R0,R5,#+6,#+1
        LDR.W    R1,??DataTable75_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1496]
        B.N      ??enableOrDisable_1
//  831 	} else if (strEqualCaseInsensitive(param, "test_mode")) {
??enableOrDisable_14:
        LDR.W    R1,??DataTable75_24
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_15
//  832 		engine->isTestMode = isEnabled;
        LDR.W    R0,??DataTable75_22
        LDR      R0,[R0, #+0]
        STRB     R5,[R0, #+640]
        B.N      ??enableOrDisable_1
//  833 	} else if (strEqualCaseInsensitive(param, "can_read")) {
??enableOrDisable_15:
        LDR.W    R1,??DataTable75_25
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_16
//  834 		engineConfiguration->canReadEnabled = isEnabled;
        LDR.W    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        BFI      R0,R5,#+0,#+1
        LDR.W    R1,??DataTable75_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1497]
        B.N      ??enableOrDisable_1
//  835 	} else if (strEqualCaseInsensitive(param, "can_write")) {
??enableOrDisable_16:
        LDR.W    R1,??DataTable75_26
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_17
//  836 		engineConfiguration->canWriteEnabled = isEnabled;
        LDR.W    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        BFI      R0,R5,#+1,#+1
        LDR.W    R1,??DataTable75_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1497]
        B.N      ??enableOrDisable_1
//  837 	} else if (strEqualCaseInsensitive(param, "injection")) {
??enableOrDisable_17:
        LDR.W    R1,??DataTable75_27
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_18
//  838 		engineConfiguration->isInjectionEnabled = isEnabled;
        LDR.W    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        BFI      R0,R5,#+0,#+1
        LDR.W    R1,??DataTable75_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1508]
        B.N      ??enableOrDisable_1
//  839 	} else if (strEqualCaseInsensitive(param, "trigger_details")) {
??enableOrDisable_18:
        LDR.W    R1,??DataTable75_28
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_19
//  840 		engineConfiguration->isPrintTriggerSynchDetails = isEnabled;
        LDR.W    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        BFI      R0,R5,#+3,#+1
        LDR.W    R1,??DataTable75_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1509]
        B.N      ??enableOrDisable_1
//  841 	} else if (strEqualCaseInsensitive(param, "ignition")) {
??enableOrDisable_19:
        LDR.W    R1,??DataTable75_29
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_20
//  842 		engineConfiguration->isIgnitionEnabled = isEnabled;
        LDR.W    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        BFI      R0,R5,#+1,#+1
        LDR.W    R1,??DataTable75_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1508]
        B.N      ??enableOrDisable_1
//  843 	} else if (strEqualCaseInsensitive(param, "self_stimulation")) {
??enableOrDisable_20:
        LDR.W    R1,??DataTable75_30
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_21
//  844 		engineConfiguration->directSelfStimulation = isEnabled;
        LDR.W    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+80]
        BFI      R0,R5,#+0,#+1
        LDR.W    R1,??DataTable75_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+80]
        B.N      ??enableOrDisable_1
//  845 	} else if (strEqualCaseInsensitive(param, "engine_control")) {
??enableOrDisable_21:
        LDR.W    R1,??DataTable75_31
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_22
//  846 		boardConfiguration->isEngineControlEnabled = isEnabled;
        LDR.N    R0,??DataTable71_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        BFI      R0,R5,#+5,#+1
        LDR.N    R1,??DataTable71_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+384]
        B.N      ??enableOrDisable_1
//  847 	} else if (strEqualCaseInsensitive(param, "map_avg")) {
??enableOrDisable_22:
        LDR.W    R1,??DataTable75_32
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_23
//  848 		engineConfiguration->isMapAveragingEnabled = isEnabled;
        LDR.W    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        BFI      R0,R5,#+5,#+1
        LDR.W    R1,??DataTable75_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1508]
        B.N      ??enableOrDisable_1
//  849 	} else if (strEqualCaseInsensitive(param, "mil")) {
??enableOrDisable_23:
        ADR.N    R1,??DataTable71_2  ;; "mil"
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_24
//  850 		engineConfiguration->isMilEnabled = isEnabled;
        LDR.W    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        BFI      R0,R5,#+6,#+1
        LDR.W    R1,??DataTable75_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1508]
        B.N      ??enableOrDisable_1
//  851 	} else if (strEqualCaseInsensitive(param, "fuel_pump")) {
??enableOrDisable_24:
        LDR.W    R1,??DataTable76_3
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_25
//  852 		engineConfiguration->isFuelPumpEnabled = isEnabled;
        LDR.W    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        ANDS     R0,R0,#0x7F
        ORRS     R0,R0,R5, LSL #+7
        LDR.W    R1,??DataTable75_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1508]
        B.N      ??enableOrDisable_1
//  853 	} else if (strEqualCaseInsensitive(param, "tunerstudio")) {
??enableOrDisable_25:
        LDR.W    R1,??DataTable76_4
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_26
//  854 		engineConfiguration->isTunerStudioEnabled = isEnabled;
        LDR.W    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        BFI      R0,R5,#+0,#+1
        LDR.W    R1,??DataTable75_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1509]
        B.N      ??enableOrDisable_1
//  855 	} else if (strEqualCaseInsensitive(param, "wave_analyzer")) {
??enableOrDisable_26:
        LDR.W    R1,??DataTable76_5
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_27
//  856 		engineConfiguration->isWaveAnalyzerEnabled = isEnabled;
        LDR.W    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        BFI      R0,R5,#+1,#+1
        LDR.N    R1,??DataTable75_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1509]
        B.N      ??enableOrDisable_1
//  857 	} else if (strEqualCaseInsensitive(param, "idle_thread")) {
??enableOrDisable_27:
        LDR.W    R1,??DataTable76_6
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_28
//  858 		engineConfiguration->isIdleThreadEnabled = isEnabled;
        LDR.N    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        BFI      R0,R5,#+2,#+1
        LDR.N    R1,??DataTable75_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1509]
        B.N      ??enableOrDisable_1
//  859 	} else if (strEqualCaseInsensitive(param, "manual_spinning")) {
??enableOrDisable_28:
        LDR.W    R1,??DataTable76_7
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_29
//  860 		engineConfiguration->isManualSpinningMode = isEnabled;
        LDR.N    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        BFI      R0,R5,#+4,#+1
        LDR.N    R1,??DataTable75_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1509]
        B.N      ??enableOrDisable_1
//  861 	} else if (strEqualCaseInsensitive(param, "cylinder_cleanup")) {
??enableOrDisable_29:
        LDR.W    R1,??DataTable76_8
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??enableOrDisable_30
//  862 		engineConfiguration->isCylinderCleanupEnabled = isEnabled;
        LDR.N    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        BFI      R0,R5,#+2,#+1
        LDR.N    R1,??DataTable75_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1508]
        B.N      ??enableOrDisable_1
//  863 	} else {
//  864 		scheduleMsg(&logger, "unexpected [%s]", param);
??enableOrDisable_30:
        MOVS     R2,R4
        LDR.W    R1,??DataTable76_9
        LDR.W    R0,??DataTable76
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  865 		return; // well, MISRA would not like this 'return' here :(
        B.N      ??enableOrDisable_31
//  866 	}
//  867 	scheduleMsg(&logger, "[%s] %s", param, isEnabled ? "enabled" : "disabled");
??enableOrDisable_1:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BEQ.N    ??enableOrDisable_32
        LDR.W    R3,??DataTable76_10
        B.N      ??enableOrDisable_33
??enableOrDisable_32:
        LDR.W    R3,??DataTable76_11
??enableOrDisable_33:
        MOVS     R2,R4
        LDR.W    R1,??DataTable76_12
        LDR.W    R0,??DataTable76
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  868 }
??enableOrDisable_31:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock66

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable66:
        DC32     config

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable66_1:
        DC8      0x73, 0x64, 0x00, 0x00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable66_2:
        DC32     `?<Constant "alternator">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable66_3:
        DC32     `?<Constant "fuelPump">`
//  869 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock67 Using cfiCommon0
          CFI Function _Z6enablePKc
        THUMB
//  870 static void enable(const char *param) {
_Z6enablePKc:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  871 	enableOrDisable(param, true);
        MOVS     R1,#+1
        MOVS     R0,R4
          CFI FunCall _Z15enableOrDisablePKcb
        BL       _Z15enableOrDisablePKcb
//  872 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock67
//  873 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock68 Using cfiCommon0
          CFI Function _Z7disablePKc
        THUMB
//  874 static void disable(const char *param) {
_Z7disablePKc:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  875 	enableOrDisable(param, false);
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall _Z15enableOrDisablePKcb
        BL       _Z15enableOrDisablePKcb
//  876 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock68
//  877 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock69 Using cfiCommon0
          CFI Function _Z9enableSpii
        THUMB
//  878 static void enableSpi(int index) {
_Z9enableSpii:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  879 	setSpiMode(index, true);
        MOVS     R1,#+1
        MOVS     R0,R4
          CFI FunCall _Z10setSpiModeib
        BL       _Z10setSpiModeib
//  880 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock69
//  881 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock70 Using cfiCommon0
          CFI Function _Z10disableSpii
        THUMB
//  882 static void disableSpi(int index) {
_Z10disableSpii:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  883 	setSpiMode(index, false);
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall _Z10setSpiModeib
        BL       _Z10setSpiModeib
//  884 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock70
//  885 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock71 Using cfiCommon0
          CFI Function _Z15enableInjectionv
        THUMB
//  886 static void enableInjection(void) {
_Z15enableInjectionv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  887 	engineConfiguration->isInjectionEnabled = true;
        LDR.N    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        ORRS     R0,R0,#0x1
        LDR.N    R1,??DataTable75_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1508]
//  888 	scheduleMsg(&logger, "injection enabled");
        LDR.W    R1,??DataTable76_13
        LDR.W    R0,??DataTable76
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  889 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock71

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable67:
        DC8      "can"
//  890 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock72 Using cfiCommon0
          CFI Function _Z16disableInjectionv
        THUMB
//  891 static void disableInjection(void) {
_Z16disableInjectionv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  892 	engineConfiguration->isInjectionEnabled = false;
        LDR.N    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        ANDS     R0,R0,#0xFE
        LDR.N    R1,??DataTable75_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1508]
//  893 	scheduleMsg(&logger, "injection disabled");
        LDR.W    R1,??DataTable76_14
        LDR.W    R0,??DataTable76
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  894 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock72

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable68:
        DC32     `?<Constant "setting injection pin...">`
//  895 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock73 Using cfiCommon0
          CFI Function _Z14enableIgnitionv
        THUMB
//  896 static void enableIgnition(void) {
_Z14enableIgnitionv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  897 	engineConfiguration->isIgnitionEnabled = true;
        LDR.N    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        ORRS     R0,R0,#0x2
        LDR.N    R1,??DataTable75_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1508]
//  898 	scheduleMsg(&logger, "ignition enabled");
        LDR.W    R1,??DataTable76_15
        LDR.W    R0,??DataTable76
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  899 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock73
//  900 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock74 Using cfiCommon0
          CFI Function _Z15disableIgnitionv
        THUMB
//  901 static void disableIgnition(void) {
_Z15disableIgnitionv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  902 	engineConfiguration->isIgnitionEnabled = false;
        LDR.N    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        ANDS     R0,R0,#0xFD
        LDR.N    R1,??DataTable75_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1508]
//  903 	scheduleMsg(&logger, "ignition disabled");
        LDR.W    R1,??DataTable76_16
        LDR.W    R0,??DataTable76
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  904 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock74
//  905 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock75 Using cfiCommon0
          CFI Function _Z10stopEnginev
        THUMB
//  906 void stopEngine(void) {
_Z10stopEnginev:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  907 	engine->stopEngineRequestTimeNt = getTimeNowNt();
          CFI FunCall getTimeNowNt
        BL       getTimeNowNt
        LDR.W    R2,??DataTable76_17
        LDR      R2,[R2, #+0]
        STRD     R0,R1,[R2, #+48]
//  908 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock75

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable71:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable71_1:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable71_2:
        DC8      "mil"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable71_3:
        DC32     `?<Constant "setting trigger pin[%...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable71_4:
        DC32     0x128ce430
//  909 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock76 Using cfiCommon0
          CFI Function _Z12printAllInfov
        THUMB
//  910 static void printAllInfo(void) {
_Z12printAllInfov:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  911 	printTemperatureInfo();
          CFI FunCall _Z20printTemperatureInfov
        BL       _Z20printTemperatureInfov
//  912 	printTPSInfo();
          CFI FunCall _Z12printTPSInfov
        BL       _Z12printTPSInfov
//  913 #if EFI_WAVE_CHART
//  914 	scheduleMsg(&logger, "waveChartUsedSize=%d", waveChartUsedSize);
        LDR.W    R0,??DataTable76_18
        LDR      R2,[R0, #+0]
        LDR.W    R1,??DataTable76_19
        LDR.W    R0,??DataTable76
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  915 #endif
//  916 #if EFI_PROD_CODE
//  917 	scheduleMsg(&logger, "console mode jumper: %s", boolToString(!GET_CONSOLE_MODE_VALUE()));
        LDR.W    R0,??DataTable76_20  ;; 0x40020410
        LDR      R0,[R0, #+0]
        UBFX     R0,R0,#+1,#+1
        ANDS     R0,R0,#0x1
        EORS     R0,R0,#0x1
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R2,R0
        LDR.W    R1,??DataTable76_21
        LDR.W    R0,??DataTable76
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  918 	scheduleMsg(&logger, "board test mode jumper: %s", boolToString(GET_BOARD_TEST_MODE_VALUE()));
        LDR.N    R0,??DataTable75
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+336]
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        LDR      R4,[R0, #+16]
        LDR.N    R0,??DataTable75
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+336]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        LSRS     R0,R4,R0
        ANDS     R0,R0,#0x1
        EORS     R0,R0,#0x1
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        MOVS     R2,R0
        LDR.W    R1,??DataTable76_22
        LDR.W    R0,??DataTable76
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  919 #endif
//  920 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock76
//  921 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock77 Using cfiCommon0
          CFI Function _Z14setInjectorLagf
          CFI NoCalls
        THUMB
//  922 static void setInjectorLag(float value) {
//  923 	engineConfiguration->injector.lag = value;
_Z14setInjectorLagf:
        LDR.N    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        VSTR     S0,[R0, #+12]
//  924 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock77
//  925 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock78 Using cfiCommon0
          CFI Function _Z8getValuePKc
        THUMB
//  926 static void getValue(const char *paramStr) {
_Z8getValuePKc:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  927 	if (strEqualCaseInsensitive(paramStr, "todo")) {
        LDR.W    R1,??DataTable76_23
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??getValue_0
//  928 		scheduleMsg(&logger, "something");
        LDR.W    R1,??DataTable76_24
        LDR.W    R0,??DataTable76
          CFI FunCall scheduleMsg
        BL       scheduleMsg
        B.N      ??getValue_1
//  929 	}
//  930 
//  931 #if EFI_RTC || defined(__DOXYGEN__)
//  932 	else if (strEqualCaseInsensitive(paramStr, "date")) {
??getValue_0:
        LDR.W    R1,??DataTable76_25
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??getValue_1
//  933 		printDateTime();
          CFI FunCall _Z13printDateTimev
        BL       _Z13printDateTimev
//  934 	}
//  935 #endif
//  936 }
??getValue_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock78

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable74:
        DC32     `?<Constant "setting EGT CS pin[%d...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable74_1:
        DC32     `?<Constant "setting trigger simul...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable74_2:
        DC32     `?<Constant "Error with [%s]">`
//  937 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock79 Using cfiCommon0
          CFI Function _Z8setValuePKcS0_
        THUMB
//  938 static void setValue(const char *paramStr, const char *valueStr) {
_Z8setValuePKcS0_:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        VPUSH    {D8}
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  939 	float valueF = atoff(valueStr);
        MOVS     R0,R5
          CFI FunCall atoff
        BL       atoff
        VMOV.F32 S16,S0
//  940 	int valueI = atoi(valueStr);
        MOVS     R0,R5
          CFI FunCall atoi
        BL       atoi
        MOVS     R6,R0
//  941 
//  942 	if (strEqualCaseInsensitive(paramStr, "vsscoeff")) {
        LDR.W    R1,??DataTable76_26
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??setValue_0
//  943 		engineConfiguration->vehicleSpeedCoef = valueF;
        LDR.N    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        VSTR     S16,[R0, #+488]
        B.N      ??setValue_1
//  944 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//  945 	} else if (strEqualCaseInsensitive(paramStr, "alt_t")) {
??setValue_0:
        LDR.W    R1,??DataTable76_27
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??setValue_2
//  946 		if (valueI > 10) {
        CMP      R6,#+11
        BLT.N    ??setValue_3
//  947 			engineConfiguration->alternatorDT = valueI;
        LDR.N    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        STR      R6,[R0, #+2164]
//  948 		}
//  949 		showAltInfo();
??setValue_3:
          CFI FunCall _Z11showAltInfov
        BL       _Z11showAltInfov
        B.N      ??setValue_1
//  950 	} else if (strEqualCaseInsensitive(paramStr, "alt_offset")) {
??setValue_2:
        LDR.W    R1,??DataTable76_28
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??setValue_4
//  951 		engineConfiguration->alternatorOffset = valueI;
        VMOV     S0,R6
        VCVT.F32.S32 S0,S0
        LDR.N    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        ADDW     R0,R0,#+2168
        VSTR     S0,[R0, #0]
        B.N      ??setValue_1
//  952 	} else if (strEqualCaseInsensitive(paramStr, "alt_p")) {
??setValue_4:
        LDR.W    R1,??DataTable76_29
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??setValue_5
//  953 		setAltPFactor(valueF);
        VMOV.F32 S0,S16
          CFI FunCall _Z13setAltPFactorf
        BL       _Z13setAltPFactorf
        B.N      ??setValue_1
//  954 #endif
//  955 	} else if (strEqualCaseInsensitive(paramStr, "step1rpm")) {
??setValue_5:
        LDR.W    R1,??DataTable76_30
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??setValue_6
//  956 		engineConfiguration->step1rpm = valueI;
        LDR.N    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        STR      R6,[R0, #+320]
        B.N      ??setValue_1
//  957 	} else if (strEqualCaseInsensitive(paramStr, "step1timing")) {
??setValue_6:
        LDR.W    R1,??DataTable76_31
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??setValue_7
//  958 		engineConfiguration->step1timing = valueI;
        LDR.N    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        STR      R6,[R0, #+324]
        B.N      ??setValue_1
//  959 	} else if (strEqualCaseInsensitive(paramStr, "targetvbatt")) {
??setValue_7:
        LDR.W    R1,??DataTable76_32
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??setValue_8
//  960 		engineConfiguration->targetVBatt = valueF;
        LDR.N    R0,??DataTable75_1
        LDR      R0,[R0, #+0]
        ADDW     R0,R0,#+2156
        VSTR     S16,[R0, #0]
        B.N      ??setValue_1
//  961 #if EFI_RTC || defined(__DOXYGEN__)
//  962 	} else if (strEqualCaseInsensitive(paramStr, "date")) {
??setValue_8:
        LDR.W    R1,??DataTable76_25
        MOVS     R0,R4
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??setValue_1
//  963 		setDateTime(valueStr);
        MOVS     R0,R5
          CFI FunCall _Z11setDateTimePKc
        BL       _Z11setDateTimePKc
//  964 #endif
//  965 	}
//  966 }
??setValue_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock79

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_1:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_2:
        DC32     `?<Constant "setting MAP to %s/%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_3:
        DC32     `?<Constant "setting CLT to %s/%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_4:
        DC32     `?<Constant "setting IAT to %s/%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_5:
        DC32     `?<Constant "setting TPS to %s/%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_6:
        DC32     `?<Constant "setting logic input p...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_7:
        DC32     `?<Constant "Pin %s: [%s]">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_8:
        DC32     `?<Constant "Setting timing map en...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_9:
        DC32     `?<Constant "Setting fuel map entr...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_10:
        DC32     `?<Constant "invalid spi index %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_11:
        DC32     `?<Constant "fastadc">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_12:
        DC32     `?<Constant "step1limimter">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_13:
        DC32     `?<Constant "serial">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_14:
        DC32     `?<Constant "stepperidle">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_15:
        DC32     `?<Constant "trigger_only_front">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_16:
        DC32     `?<Constant "two_wire_batch">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_17:
        DC32     `?<Constant "joystick">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_18:
        DC32     `?<Constant "HIP9011">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_19:
        DC32     `?<Constant "altdebug">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_20:
        DC32     `?<Constant "tpic_advanced_mode">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_21:
        DC32     `?<Constant "knockdebug">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_22:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_23:
        DC32     `?<Constant "altcontrol">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_24:
        DC32     `?<Constant "test_mode">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_25:
        DC32     `?<Constant "can_read">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_26:
        DC32     `?<Constant "can_write">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_27:
        DC32     `?<Constant "injection">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_28:
        DC32     `?<Constant "trigger_details">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_29:
        DC32     `?<Constant "ignition">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_30:
        DC32     `?<Constant "self_stimulation">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_31:
        DC32     `?<Constant "engine_control">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable75_32:
        DC32     `?<Constant "map_avg">`
//  967 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock80 Using cfiCommon0
          CFI Function _Z12initSettingsP22engine_configuration_s
        THUMB
//  968 void initSettings(engine_configuration_s *engineConfiguration) {
_Z12initSettingsP22engine_configuration_s:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  969 	addConsoleActionP("showconfig", (VoidPtr) doPrintConfiguration, &engine);
        LDR.N    R2,??DataTable76_17
        LDR.N    R1,??DataTable76_33
        LDR.N    R0,??DataTable76_34
          CFI FunCall addConsoleActionP
        BL       addConsoleActionP
//  970 	addConsoleAction("tempinfo", printTemperatureInfo);
        LDR.N    R1,??DataTable76_35
        LDR.N    R0,??DataTable76_36
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  971 	addConsoleAction("tpsinfo", printTPSInfo);
        LDR.N    R1,??DataTable76_37
        LDR.N    R0,??DataTable76_38
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  972 	addConsoleAction("info", printAllInfo);
        LDR.N    R1,??DataTable76_39
        LDR.N    R0,??DataTable76_40
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  973 
//  974 	addConsoleActionF("set_ignition_offset", setIgnitionOffset);
        LDR.N    R1,??DataTable76_41
        LDR.N    R0,??DataTable76_42
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
//  975 	addConsoleActionF("set_injection_offset", setInjectionOffset);
        LDR.N    R1,??DataTable76_43
        LDR.N    R0,??DataTable76_44
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
//  976 	addConsoleActionF("set_global_trigger_offset_angle", setGlobalTriggerAngleOffset);
        LDR.N    R1,??DataTable76_45
        LDR.N    R0,??DataTable76_46
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
//  977 	addConsoleActionI("set_sensor_chart_mode", setSensorChartMode);
        LDR.N    R1,??DataTable76_47
        LDR.N    R0,??DataTable76_48
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  978 	addConsoleActionI("set_fixed_mode_timing", setFixedModeTiming);
        LDR.N    R1,??DataTable76_49
        LDR.N    R0,??DataTable76_50
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  979 	addConsoleActionI("set_timing_mode", setTimingMode);
        LDR.N    R1,??DataTable76_51
        LDR.N    R0,??DataTable76_52
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  980 	addConsoleActionI("set_engine_type", setEngineType);
        LDR.N    R1,??DataTable76_53
        LDR.N    R0,??DataTable76_54
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  981 
//  982 	addConsoleActionI("set_injection_pin_mode", setInjectionPinMode);
        LDR.N    R1,??DataTable76_55
        LDR.N    R0,??DataTable76_56
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  983 	addConsoleActionI("set_ignition_pin_mode", setIgnitionPinMode);
        LDR.N    R1,??DataTable76_57
        LDR.N    R0,??DataTable76_58
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  984 	addConsoleActionI("set_idle_pin_mode", setIdlePinMode);
        LDR.N    R1,??DataTable76_59
        LDR.N    R0,??DataTable76_60
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  985 	addConsoleActionI("set_fuel_pump_pin_mode", setFuelPumpPinMode);
        LDR.N    R1,??DataTable76_61
        LDR.N    R0,??DataTable76_62
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  986 	addConsoleActionI("set_malfunction_indicator_pin_mode", setMalfunctionIndicatorPinMode);
        LDR.N    R1,??DataTable76_63
        LDR.N    R0,??DataTable76_64
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  987 	addConsoleActionI("set_operation_mode", setOM);
        LDR.N    R1,??DataTable76_65
        LDR.N    R0,??DataTable76_66
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  988 	// todo: start saving values into flash right away?
//  989 
//  990 	addConsoleActionF("set_global_fuel_correction", setGlobalFuelCorrection);
        LDR.N    R1,??DataTable76_67
        LDR.N    R0,??DataTable76_68
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
//  991 
//  992 	addConsoleActionF("set_cranking_fuel", setCrankingFuel);
        LDR.N    R1,??DataTable76_69
        LDR.N    R0,??DataTable76_70
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
//  993 	addConsoleActionI("set_cranking_rpm", setCrankingRpm);
        LDR.N    R1,??DataTable76_71
        LDR.N    R0,??DataTable76_72
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  994 	addConsoleActionF("set_cranking_timing_angle", setCrankingTimingAngle);
        LDR.N    R1,??DataTable76_73
        LDR.N    R0,??DataTable76_74
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
//  995 	addConsoleActionF("set_cranking_charge_angle", setCrankingChargeAngle);
        LDR.N    R1,??DataTable76_75
        LDR.N    R0,??DataTable76_76
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
//  996 	addConsoleActionI("set_ignition_mode", setIgnitionMode);
        LDR.N    R1,??DataTable76_77
        LDR.N    R0,??DataTable76_78
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  997 	addConsoleActionI("set_cranking_injection_mode", setCrankingInjectionMode);
        LDR.N    R1,??DataTable76_79
        LDR.N    R0,??DataTable76_80
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  998 	addConsoleActionI("set_injection_mode", setInjectionMode);
        LDR.N    R1,??DataTable76_81
        LDR.N    R0,??DataTable76_82
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  999 
// 1000 	addConsoleActionF("set_whole_phase_map", setWholePhaseMapCmd);
        LDR.N    R1,??DataTable76_83
        LDR.N    R0,??DataTable76_84
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
// 1001 	addConsoleActionF("set_whole_timing_map", setWholeTimingMapCmd);
        LDR.N    R1,??DataTable76_85
        LDR.N    R0,??DataTable76_86
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
// 1002 	addConsoleActionF("set_whole_fuel_map", setWholeFuelMapCmd);
        LDR.N    R1,??DataTable76_87
        LDR.N    R0,??DataTable76_88
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
// 1003 	addConsoleActionF("set_whole_ve_map", setWholeVeCmd);
        LDR.N    R1,??DataTable76_89
        LDR.N    R0,??DataTable76_90
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
// 1004 	addConsoleActionSSS("set_fuel_map", setFuelMap);
        LDR.N    R1,??DataTable76_91
        LDR.N    R0,??DataTable76_92
          CFI FunCall addConsoleActionSSS
        BL       addConsoleActionSSS
// 1005 
// 1006 	addConsoleActionF("set_whole_timing_map", setWholeTimingMap);
        LDR.N    R1,??DataTable76_93
        LDR.N    R0,??DataTable76_86
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
// 1007 	addConsoleActionSSS("set_timing_map", setTimingMap);
        LDR.N    R1,??DataTable76_94
        LDR.N    R0,??DataTable76_95
          CFI FunCall addConsoleActionSSS
        BL       addConsoleActionSSS
// 1008 
// 1009 	addConsoleActionI("set_rpm_hard_limit", setRpmHardLimit);
        LDR.N    R1,??DataTable76_96
        LDR.N    R0,??DataTable76_97
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
// 1010 	addConsoleActionI("set_firing_order", setFiringOrder);
        LDR.N    R1,??DataTable76_98
        LDR.N    R0,??DataTable76_99
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
// 1011 	addConsoleActionI("set_algorithm", setAlgorithmInt);
        LDR.N    R1,??DataTable76_100
        LDR.N    R0,??DataTable76_101
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
// 1012 	addConsoleAction("stopengine", (Void) stopEngine);
        LDR.N    R1,??DataTable76_102
        LDR.N    R0,??DataTable76_103
          CFI FunCall addConsoleAction
        BL       addConsoleAction
// 1013 
// 1014 	// todo: refactor this - looks like all boolean flags should be controlled with less code duplication
// 1015 	addConsoleAction("enable_injection", enableInjection);
        LDR.N    R1,??DataTable76_104
        LDR.N    R0,??DataTable76_105
          CFI FunCall addConsoleAction
        BL       addConsoleAction
// 1016 	addConsoleAction("disable_injection", disableInjection);
        LDR.N    R1,??DataTable76_106
        LDR.N    R0,??DataTable76_107
          CFI FunCall addConsoleAction
        BL       addConsoleAction
// 1017 	addConsoleAction("enable_ignition", enableIgnition);
        LDR.N    R1,??DataTable76_108
        LDR.N    R0,??DataTable76_109
          CFI FunCall addConsoleAction
        BL       addConsoleAction
// 1018 	addConsoleAction("disable_ignition", disableIgnition);
        LDR.N    R1,??DataTable76_110
        LDR.N    R0,??DataTable76_111
          CFI FunCall addConsoleAction
        BL       addConsoleAction
// 1019 
// 1020 	addConsoleActionI("enable_spi", enableSpi);
        LDR.N    R1,??DataTable76_112
        LDR.N    R0,??DataTable76_113
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
// 1021 	addConsoleActionI("disable_spi", disableSpi);
        LDR.N    R1,??DataTable76_114
        LDR.N    R0,??DataTable76_115
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
// 1022 
// 1023 	addConsoleActionS("enable", enable);
        LDR.N    R1,??DataTable76_116
        LDR.N    R0,??DataTable76_117
          CFI FunCall addConsoleActionS
        BL       addConsoleActionS
// 1024 	addConsoleActionS("disable", disable);
        LDR.N    R1,??DataTable76_118
        LDR.N    R0,??DataTable76_119
          CFI FunCall addConsoleActionS
        BL       addConsoleActionS
// 1025 
// 1026 	addConsoleActionII("set_toothed_wheel", setToothedWheel);
        LDR.N    R1,??DataTable76_120
        LDR.N    R0,??DataTable76_121
          CFI FunCall addConsoleActionII
        BL       addConsoleActionII
// 1027 	addConsoleActionI("set_trigger_type", setTriggerType);
        LDR.N    R1,??DataTable76_122
        LDR.N    R0,??DataTable76_123
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
// 1028 
// 1029 	addConsoleActionF("set_vbatt_divider", setVBattDivider);
        LDR.N    R1,??DataTable76_124
        LDR.N    R0,??DataTable76_125
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
// 1030 
// 1031 	addConsoleActionF("set_injector_lag", setInjectorLag);
        LDR.N    R1,??DataTable76_126
        LDR.N    R0,??DataTable76_127
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
// 1032 
// 1033 	addConsoleActionF("set_clt_bias", setCltBias);
        LDR.N    R1,??DataTable76_128
        LDR.N    R0,??DataTable76_129
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
// 1034 	addConsoleActionF("set_iat_bias", setIatBias);
        LDR.N    R1,??DataTable76_130
        LDR.N    R0,??DataTable76_131
          CFI FunCall addConsoleActionF
        BL       addConsoleActionF
// 1035 	addConsoleActionI("set_idle_solenoid_freq", setIdleSolenoidFrequency);
        LDR.N    R1,??DataTable76_132
        LDR.N    R0,??DataTable76_133
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
// 1036 
// 1037 	addConsoleActionFF("set_fan", setFanSetting);
        LDR.N    R1,??DataTable76_134
        LDR.N    R0,??DataTable76_135
          CFI FunCall addConsoleActionFF
        BL       addConsoleActionFF
// 1038 
// 1039 #if EFI_PROD_CODE
// 1040 	addConsoleActionS("showpin", showPinFunction);
        LDR.N    R1,??DataTable76_136
        LDR.N    R0,??DataTable76_137
          CFI FunCall addConsoleActionS
        BL       addConsoleActionS
// 1041 	addConsoleActionSS("set_injection_pin", setInjectionPin);
        LDR.N    R1,??DataTable76_138
        LDR.N    R0,??DataTable76_139
          CFI FunCall addConsoleActionSS
        BL       addConsoleActionSS
// 1042 	addConsoleActionSS("set_ignition_pin", setIgnitionPin);
        LDR.N    R1,??DataTable76_140
        LDR.N    R0,??DataTable76_141
          CFI FunCall addConsoleActionSS
        BL       addConsoleActionSS
// 1043 	addConsoleActionSS("set_trigger_input_pin", setTriggerInputPin);
        LDR.N    R1,??DataTable76_142
        LDR.N    R0,??DataTable76_143
          CFI FunCall addConsoleActionSS
        BL       addConsoleActionSS
// 1044 	addConsoleActionSS("set_trigger_simulator_pin", setTriggerSimulatorPin);
        LDR.N    R1,??DataTable76_144
        LDR.N    R0,??DataTable76_145
          CFI FunCall addConsoleActionSS
        BL       addConsoleActionSS
// 1045 
// 1046 	addConsoleActionSSP("set_egt_cs_pin", (VoidCharPtrCharPtrVoidPtr) setEgtCSPin, boardConfiguration);
        LDR.N    R0,??DataTable76_146
        LDR      R2,[R0, #+0]
        LDR.N    R1,??DataTable76_147
        LDR.N    R0,??DataTable76_148
          CFI FunCall addConsoleActionSSP
        BL       addConsoleActionSSP
// 1047 	addConsoleActionI("set_egt_spi", setEgtSpi);
        LDR.N    R1,??DataTable76_149
        LDR.N    R0,??DataTable76_150
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
// 1048 
// 1049 	addConsoleActionSS("set_trigger_simulator_mode", setTriggerSimulatorMode);
        LDR.N    R1,??DataTable76_151
        LDR.N    R0,??DataTable76_152
          CFI FunCall addConsoleActionSS
        BL       addConsoleActionSS
// 1050 	addConsoleActionS("set_fuel_pump_pin", setFuelPumpPin);
        LDR.N    R1,??DataTable76_153
        LDR.N    R0,??DataTable76_154
          CFI FunCall addConsoleActionS
        BL       addConsoleActionS
// 1051 	addConsoleActionS("set_ac_pin", setACPin);
        LDR.N    R1,??DataTable76_155
        LDR.N    R0,??DataTable76_156
          CFI FunCall addConsoleActionS
        BL       addConsoleActionS
// 1052 	addConsoleActionS("set_alternator_pin", setAlternatorPin);
        LDR.N    R1,??DataTable76_157
        LDR.N    R0,??DataTable76_158
          CFI FunCall addConsoleActionS
        BL       addConsoleActionS
// 1053 	addConsoleActionS("set_idle_pin", setIdlePin);
        LDR.N    R1,??DataTable76_159
        LDR.N    R0,??DataTable76_160
          CFI FunCall addConsoleActionS
        BL       addConsoleActionS
// 1054 	addConsoleActionS("set_main_relay_pin", setMainRelayPin);
        LDR.N    R1,??DataTable76_161
        LDR.N    R0,??DataTable76_162
          CFI FunCall addConsoleActionS
        BL       addConsoleActionS
// 1055 
// 1056 	addConsoleActionSS("set", setValue);
        LDR.N    R1,??DataTable76_163
        ADR.N    R0,??DataTable76_1  ;; "set"
          CFI FunCall addConsoleActionSS
        BL       addConsoleActionSS
// 1057 	addConsoleActionS("get", getValue);
        LDR.N    R1,??DataTable76_164
        ADR.N    R0,??DataTable76_2  ;; "get"
          CFI FunCall addConsoleActionS
        BL       addConsoleActionS
// 1058 
// 1059 #if HAL_USE_ADC || defined(__DOXYGEN__)
// 1060 	addConsoleActionSS("set_analog_input_pin", setAnalogInputPin);
        LDR.N    R1,??DataTable76_165
        LDR.N    R0,??DataTable76_166
          CFI FunCall addConsoleActionSS
        BL       addConsoleActionSS
// 1061 #endif
// 1062 	addConsoleActionSS("set_logic_input_pin", setLogicInputPin);
        LDR.N    R1,??DataTable76_167
        LDR.N    R0,??DataTable76_168
          CFI FunCall addConsoleActionSS
        BL       addConsoleActionSS
// 1063 	addConsoleActionI("set_pot_spi", setPotSpi);
        LDR.N    R1,??DataTable76_169
        LDR.N    R0,??DataTable76_170
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
// 1064 #endif /* EFI_PROD_CODE */
// 1065 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock80

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_1:
        DC8      "set"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_2:
        DC8      "get"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_3:
        DC32     `?<Constant "fuel_pump">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_4:
        DC32     `?<Constant "tunerstudio">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_5:
        DC32     `?<Constant "wave_analyzer">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_6:
        DC32     `?<Constant "idle_thread">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_7:
        DC32     `?<Constant "manual_spinning">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_8:
        DC32     `?<Constant "cylinder_cleanup">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_9:
        DC32     `?<Constant "unexpected [%s]">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_10:
        DC32     `?<Constant "enabled">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_11:
        DC32     `?<Constant "disabled">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_12:
        DC32     `?<Constant "[%s] %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_13:
        DC32     `?<Constant "injection enabled">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_14:
        DC32     `?<Constant "injection disabled">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_15:
        DC32     `?<Constant "ignition enabled">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_16:
        DC32     `?<Constant "ignition disabled">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_17:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_18:
        DC32     waveChartUsedSize

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_19:
        DC32     `?<Constant "waveChartUsedSize=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_20:
        DC32     0x40020410

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_21:
        DC32     `?<Constant "console mode jumper: %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_22:
        DC32     `?<Constant "board test mode jumpe...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_23:
        DC32     `?<Constant "todo">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_24:
        DC32     `?<Constant "something">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_25:
        DC32     `?<Constant "date">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_26:
        DC32     `?<Constant "vsscoeff">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_27:
        DC32     `?<Constant "alt_t">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_28:
        DC32     `?<Constant "alt_offset">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_29:
        DC32     `?<Constant "alt_p">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_30:
        DC32     `?<Constant "step1rpm">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_31:
        DC32     `?<Constant "step1timing">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_32:
        DC32     `?<Constant "targetvbatt">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_33:
        DC32     _Z20doPrintConfigurationP6Engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_34:
        DC32     `?<Constant "showconfig">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_35:
        DC32     _Z20printTemperatureInfov

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_36:
        DC32     `?<Constant "tempinfo">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_37:
        DC32     _Z12printTPSInfov

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_38:
        DC32     `?<Constant "tpsinfo">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_39:
        DC32     _Z12printAllInfov

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_40:
        DC32     `?<Constant "info">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_41:
        DC32     _Z17setIgnitionOffsetf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_42:
        DC32     `?<Constant "set_ignition_offset">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_43:
        DC32     _Z18setInjectionOffsetf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_44:
        DC32     `?<Constant "set_injection_offset">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_45:
        DC32     _Z27setGlobalTriggerAngleOffsetf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_46:
        DC32     `?<Constant "set_global_trigger_of...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_47:
        DC32     _Z18setSensorChartModei

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_48:
        DC32     `?<Constant "set_sensor_chart_mode">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_49:
        DC32     _Z18setFixedModeTimingi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_50:
        DC32     `?<Constant "set_fixed_mode_timing">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_51:
        DC32     _Z13setTimingModei

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_52:
        DC32     `?<Constant "set_timing_mode">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_53:
        DC32     _Z13setEngineTypei

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_54:
        DC32     `?<Constant "set_engine_type">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_55:
        DC32     _Z19setInjectionPinModei

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_56:
        DC32     `?<Constant "set_injection_pin_mode">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_57:
        DC32     _Z18setIgnitionPinModei

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_58:
        DC32     `?<Constant "set_ignition_pin_mode">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_59:
        DC32     _Z14setIdlePinModei

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_60:
        DC32     `?<Constant "set_idle_pin_mode">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_61:
        DC32     _Z18setFuelPumpPinModei

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_62:
        DC32     `?<Constant "set_fuel_pump_pin_mode">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_63:
        DC32     _Z30setMalfunctionIndicatorPinModei

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_64:
        DC32     `?<Constant "set_malfunction_indic...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_65:
        DC32     _Z5setOMi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_66:
        DC32     `?<Constant "set_operation_mode">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_67:
        DC32     _Z23setGlobalFuelCorrectionf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_68:
        DC32     `?<Constant "set_global_fuel_corre...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_69:
        DC32     _Z15setCrankingFuelf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_70:
        DC32     `?<Constant "set_cranking_fuel">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_71:
        DC32     _Z14setCrankingRpmi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_72:
        DC32     `?<Constant "set_cranking_rpm">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_73:
        DC32     _Z22setCrankingTimingAnglef

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_74:
        DC32     `?<Constant "set_cranking_timing_a...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_75:
        DC32     _Z22setCrankingChargeAnglef

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_76:
        DC32     `?<Constant "set_cranking_charge_a...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_77:
        DC32     _Z15setIgnitionModei

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_78:
        DC32     `?<Constant "set_ignition_mode">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_79:
        DC32     _Z24setCrankingInjectionModei

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_80:
        DC32     `?<Constant "set_cranking_injectio...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_81:
        DC32     _Z16setInjectionModei

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_82:
        DC32     `?<Constant "set_injection_mode">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_83:
        DC32     _Z19setWholePhaseMapCmdf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_84:
        DC32     `?<Constant "set_whole_phase_map">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_85:
        DC32     _Z20setWholeTimingMapCmdf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_86:
        DC32     `?<Constant "set_whole_timing_map">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_87:
        DC32     _Z18setWholeFuelMapCmdf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_88:
        DC32     `?<Constant "set_whole_fuel_map">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_89:
        DC32     _Z13setWholeVeCmdf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_90:
        DC32     `?<Constant "set_whole_ve_map">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_91:
        DC32     _Z10setFuelMapPKcS0_S0_

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_92:
        DC32     `?<Constant "set_fuel_map">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_93:
        DC32     _Z17setWholeTimingMapf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_94:
        DC32     _Z12setTimingMapPKcS0_S0_

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_95:
        DC32     `?<Constant "set_timing_map">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_96:
        DC32     _Z15setRpmHardLimiti

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_97:
        DC32     `?<Constant "set_rpm_hard_limit">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_98:
        DC32     _Z14setFiringOrderi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_99:
        DC32     `?<Constant "set_firing_order">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_100:
        DC32     _Z15setAlgorithmInti

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_101:
        DC32     `?<Constant "set_algorithm">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_102:
        DC32     _Z10stopEnginev

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_103:
        DC32     `?<Constant "stopengine">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_104:
        DC32     _Z15enableInjectionv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_105:
        DC32     `?<Constant "enable_injection">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_106:
        DC32     _Z16disableInjectionv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_107:
        DC32     `?<Constant "disable_injection">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_108:
        DC32     _Z14enableIgnitionv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_109:
        DC32     `?<Constant "enable_ignition">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_110:
        DC32     _Z15disableIgnitionv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_111:
        DC32     `?<Constant "disable_ignition">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_112:
        DC32     _Z9enableSpii

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_113:
        DC32     `?<Constant "enable_spi">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_114:
        DC32     _Z10disableSpii

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_115:
        DC32     `?<Constant "disable_spi">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_116:
        DC32     _Z6enablePKc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_117:
        DC32     `?<Constant "enable">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_118:
        DC32     _Z7disablePKc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_119:
        DC32     `?<Constant "disable">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_120:
        DC32     _Z15setToothedWheelii

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_121:
        DC32     `?<Constant "set_toothed_wheel">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_122:
        DC32     _Z14setTriggerTypei

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_123:
        DC32     `?<Constant "set_trigger_type">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_124:
        DC32     _Z15setVBattDividerf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_125:
        DC32     `?<Constant "set_vbatt_divider">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_126:
        DC32     _Z14setInjectorLagf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_127:
        DC32     `?<Constant "set_injector_lag">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_128:
        DC32     _Z10setCltBiasf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_129:
        DC32     `?<Constant "set_clt_bias">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_130:
        DC32     _Z10setIatBiasf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_131:
        DC32     `?<Constant "set_iat_bias">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_132:
        DC32     _Z24setIdleSolenoidFrequencyi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_133:
        DC32     `?<Constant "set_idle_solenoid_freq">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_134:
        DC32     _Z13setFanSettingff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_135:
        DC32     `?<Constant "set_fan">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_136:
        DC32     _Z15showPinFunctionPKc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_137:
        DC32     `?<Constant "showpin">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_138:
        DC32     _Z15setInjectionPinPKcS0_

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_139:
        DC32     `?<Constant "set_injection_pin">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_140:
        DC32     _Z14setIgnitionPinPKcS0_

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_141:
        DC32     `?<Constant "set_ignition_pin">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_142:
        DC32     _Z18setTriggerInputPinPKcS0_

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_143:
        DC32     `?<Constant "set_trigger_input_pin">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_144:
        DC32     _Z22setTriggerSimulatorPinPKcS0_

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_145:
        DC32     `?<Constant "set_trigger_simulator...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_146:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_147:
        DC32     _Z11setEgtCSPinPKcS0_P21board_configuration_s

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_148:
        DC32     `?<Constant "set_egt_cs_pin">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_149:
        DC32     _Z9setEgtSpii

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_150:
        DC32     `?<Constant "set_egt_spi">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_151:
        DC32     _Z23setTriggerSimulatorModePKcS0_

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_152:
        DC32     `?<Constant "set_trigger_simulator...">_1`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_153:
        DC32     _Z14setFuelPumpPinPKc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_154:
        DC32     `?<Constant "set_fuel_pump_pin">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_155:
        DC32     _Z8setACPinPKc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_156:
        DC32     `?<Constant "set_ac_pin">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_157:
        DC32     _Z16setAlternatorPinPKc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_158:
        DC32     `?<Constant "set_alternator_pin">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_159:
        DC32     _Z10setIdlePinPKc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_160:
        DC32     `?<Constant "set_idle_pin">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_161:
        DC32     _Z15setMainRelayPinPKc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_162:
        DC32     `?<Constant "set_main_relay_pin">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_163:
        DC32     _Z8setValuePKcS0_

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_164:
        DC32     _Z8getValuePKc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_165:
        DC32     _Z17setAnalogInputPinPKcS0_

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_166:
        DC32     `?<Constant "set_analog_input_pin">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_167:
        DC32     _Z16setLogicInputPinPKcS0_

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_168:
        DC32     `?<Constant "set_logic_input_pin">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_169:
        DC32     _Z9setPotSpii

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable76_170:
        DC32     `?<Constant "set_pot_spi">`

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
        DATA
        DC8 "%f "

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spi 1=%s/2=%s/3=%s">`:
        DATA
        DC8 "spi 1=%s/2=%s/3=%s"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "injectionPins: mode %s">`:
        DATA
        DC8 "injectionPins: mode %s"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "injection #%d @ %s">`:
        DATA
        DC8 "injection #%d @ %s"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ignitionPins: mode %s">`:
        DATA
        DC8 "ignitionPins: mode %s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ignition #%d @ %s">`:
        DATA
        DC8 "ignition #%d @ %s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "idlePin: mode %s @ %s...">`:
        DATA
        DC8 "idlePin: mode %s @ %s freq=%d"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "malfunctionIndicatorn...">`:
        DATA
        DC8 "malfunctionIndicatorn: %s mode=%s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fuelPumpPin: mode %s ...">`:
        DATA
        DC8 "fuelPumpPin: mode %s @ %s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fanPin: mode %s @ %s">`:
        DATA
        DC8 "fanPin: mode %s @ %s"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "mainRelay: mode %s @ %s">`:
        DATA
        DC8 "mainRelay: mode %s @ %s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "alternator field: mod...">`:
        DATA
        DC8 "alternator field: mode %s @ %s"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "CUSTOM">`:
        DATA
        DC8 "CUSTOM"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Neon95">`:
        DATA
        DC8 "Neon95"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Neon03">`:
        DATA
        DC8 "Neon03"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Aspire">`:
        DATA
        DC8 "Aspire"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Fiesta">`:
        DATA
        DC8 "Fiesta"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Primera">`:
        DATA
        DC8 "Primera"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Accord3">`:
        DATA
        DC8 "Accord3"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Accord2">`:
        DATA
        DC8 "Accord2"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "HondaD">`:
        DATA
        DC8 "HondaD"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Fordi6">`:
        DATA
        DC8 "Fordi6"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Gy6139">`:
        DATA
        DC8 "Gy6139"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MiataNB">`:
        DATA
        DC8 "MiataNB"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "M323">`:
        DATA
        DC8 "M323"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Saturn Ion">`:
        DATA
        DC8 "Saturn Ion"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "CoopR50">`:
        DATA
        DC8 "CoopR50"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EscrtGT">`:
        DATA
        DC8 "EscrtGT"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TU3JP">`:
        DATA
        DC8 "TU3JP"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Rvrv8">`:
        DATA
        DC8 "Rvrv8"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Mi4G93">`:
        DATA
        DC8 "Mi4G93"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MX590">`:
        DATA
        DC8 "MX590"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MX594d">`:
        DATA
        DC8 "MX594d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MX594s">`:
        DATA
        DC8 "MX594s"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MX596">`:
        DATA
        DC8 "MX596"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "BMWe34">`:
        DATA
        DC8 "BMWe34"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Test">`:
        DATA
        DC8 "Test"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "VW_ABA">`:
        DATA
        DC8 "VW_ABA"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SACHS">`:
        DATA
        DC8 "SACHS"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GM_2_2">`:
        DATA
        DC8 "GM_2_2"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "DODGE_RAM">`:
        DATA
        DC8 "DODGE_RAM"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Mazda626">`:
        DATA
        DC8 "Mazda626"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "UnknownType">`:
        DATA
        DC8 "UnknownType"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Template %s/%d trigge...">`:
        DATA
        DC8 "Template %s/%d trigger %s/%s/%d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "configurationVersion=%d">`:
        DATA
        DC8 "configurationVersion=%d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "rpmHardLimit: %d/oper...">`:
        DATA
        DC8 "rpmHardLimit: %d/operationMode=%d"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "globalTriggerAngleOff...">`:
        DATA
        DC8 "globalTriggerAngleOffset=%f"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "=== cranking ===">`:
        DATA
        DC8 "=== cranking ==="
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "crankingRpm: %d">`:
        DATA
        DC8 "crankingRpm: %d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "cranking injection %s">`:
        DATA
        DC8 "cranking injection %s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ignitionDwellForCrank...">`:
        DATA
        DC8 "ignitionDwellForCrankingMs=%f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "cranking charge charg...">`:
        DATA
        DC8 "cranking charge charge angle=%f fire at %f"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "=== ignition ===">`:
        DATA
        DC8 "=== ignition ==="
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ignitionMode: %s/enab...">`:
        DATA
        DC8 "ignitionMode: %s/enabled=%s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "timingMode: %s">`:
        DATA
        DC8 "timingMode: %s"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fixedModeTiming: %d">`:
        DATA
        DC8 "fixedModeTiming: %d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ignitionOffset=%f">`:
        DATA
        DC8 "ignitionOffset=%f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "=== injection ===">`:
        DATA
        DC8 "=== injection ==="
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "injection %s offset=%...">`:
        DATA
        DC8 "injection %s offset=%f/enabled=%s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "map_avg=%s/mil=%s/fp=...">`:
        DATA
        DC8 6DH, 61H, 70H, 5FH, 61H, 76H, 67H, 3DH
        DC8 25H, 73H, 2FH, 6DH, 69H, 6CH, 3DH, 25H
        DC8 73H, 2FH, 66H, 70H, 3DH, 25H, 73H, 2FH
        DC8 74H, 73H, 3DH, 25H, 73H, 2FH, 77H, 61H
        DC8 3DH, 25H, 73H, 2FH, 69H, 74H, 3DH, 25H
        DC8 73H, 2FH, 66H, 61H, 73H, 74H, 41H, 64H
        DC8 63H, 3DH, 25H, 73H, 0
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "isManualSpinningMode=...">`:
        DATA
        DC8 69H, 73H, 4DH, 61H, 6EH, 75H, 61H, 6CH
        DC8 53H, 70H, 69H, 6EH, 6EH, 69H, 6EH, 67H
        DC8 4DH, 6FH, 64H, 65H, 3DH, 25H, 73H, 2FH
        DC8 69H, 73H, 43H, 79H, 6CH, 69H, 6EH, 64H
        DC8 65H, 72H, 43H, 6CH, 65H, 61H, 6EH, 75H
        DC8 70H, 45H, 6EH, 61H, 62H, 6CH, 65H, 64H
        DC8 3DH, 25H, 73H, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "clutchUp@%s: %s">`:
        DATA
        DC8 "clutchUp@%s: %s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "clutchDown@%s: %s">`:
        DATA
        DC8 "clutchDown@%s: %s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "boardTestModeJumperPi...">`:
        DATA
        DC8 "boardTestModeJumperPin: %s/nesting=%d"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "digitalPotentiometerS...">`:
        DATA
        DC8 "digitalPotentiometerSpiDevice %d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "digitalPotentiometer ...">`:
        DATA
        DC8 "digitalPotentiometer CS%d %s"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "term">`:
        DATA
        DC8 "term"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "%s volts=%f Celsius=%...">`:
        DATA
        DC8 "%s volts=%f Celsius=%f sensorR=%f on channel %d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "@%s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "C=%f/R=%f C=%f/R=%f C...">`:
        DATA
        DC8 "C=%f/R=%f C=%f/R=%f C=%f/R=%f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "bias resistor=%fK A=%...">`:
        DATA
        DC8 62H, 69H, 61H, 73H, 20H, 72H, 65H, 73H
        DC8 69H, 73H, 74H, 6FH, 72H, 3DH, 25H, 66H
        DC8 4BH, 20H, 41H, 3DH, 25H, 2EH, 2EH, 31H
        DC8 30H, 30H, 30H, 30H, 30H, 66H, 20H, 42H
        DC8 3DH, 25H, 2EH, 2EH, 31H, 30H, 30H, 30H
        DC8 30H, 30H, 66H, 20H, 43H, 3DH, 25H, 2EH
        DC8 2EH, 31H, 30H, 30H, 30H, 30H, 30H, 66H
        DC8 0
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "=====================...">`:
        DATA
        DC8 "=============================="
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "NO TPS SENSOR">`:
        DATA
        DC8 "NO TPS SENSOR"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "tps min %d/max %d v=%...">`:
        DATA
        DC8 "tps min %d/max %d v=%f @%s%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "current 10bit=%d valu...">`:
        DATA
        DC8 "current 10bit=%d value=%f rate=%f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "CLT"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "CLT sensing error">`:
        DATA
        DC8 "CLT sensing error"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "IAT"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "IAT sensing error">`:
        DATA
        DC8 "IAT sensing error"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fan=%s @ %s">`:
        DATA
        DC8 "fan=%s @ %s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "A/C relay=%s @ %s">`:
        DATA
        DC8 "A/C relay=%s @ %s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "toothed: total=%d/ski...">`:
        DATA
        DC8 "toothed: total=%d/skipped=%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "setting fuel mult=%f">`:
        DATA
        DC8 "setting fuel mult=%f"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ON temp [%f] should b...">`:
        DATA
        DC8 "ON temp [%f] should be above OFF temp [%f]"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Setting whole timing ...">`:
        DATA
        DC8 "Setting whole timing map to %f"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Setting whole injecti...">`:
        DATA
        DC8 "Setting whole injection phase map to %f"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Setting whole timing ...">_1`:
        DATA
        DC8 "Setting whole timing advance map to %f"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Setting whole VE map ...">`:
        DATA
        DC8 "Setting whole VE map to %f"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "WARNING: setting VE m...">`:
        DATA
        DC8 57H, 41H, 52H, 4EH, 49H, 4EH, 47H, 3AH
        DC8 20H, 73H, 65H, 74H, 74H, 69H, 6EH, 67H
        DC8 20H, 56H, 45H, 20H, 6DH, 61H, 70H, 20H
        DC8 6EH, 6FH, 74H, 20H, 69H, 6EH, 20H, 53H
        DC8 44H, 20H, 6DH, 6FH, 64H, 65H, 20H, 69H
        DC8 73H, 20H, 70H, 6FH, 69H, 6EH, 74H, 6CH
        DC8 65H, 73H, 73H, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Setting whole fuel ma...">`:
        DATA
        DC8 "Setting whole fuel map to %f"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "WARNING: setting fuel...">`:
        DATA
        DC8 "WARNING: setting fuel map in SD mode is pointless"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid pin name [%s]">`:
        DATA
        DC8 "invalid pin name [%s]"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "setting ignition pin[...">`:
        DATA
        DC8 73H, 65H, 74H, 74H, 69H, 6EH, 67H, 20H
        DC8 69H, 67H, 6EH, 69H, 74H, 69H, 6FH, 6EH
        DC8 20H, 70H, 69H, 6EH, 5BH, 25H, 64H, 5DH
        DC8 20H, 74H, 6FH, 20H, 25H, 73H, 20H, 70H
        DC8 6CH, 65H, 61H, 73H, 65H, 20H, 73H, 61H
        DC8 76H, 65H, 26H, 72H, 65H, 73H, 74H, 61H
        DC8 72H, 74H, 0
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "setting %s pin to %s ...">`:
        DATA
        DC8 "setting %s pin to %s please save&restart"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "idle">`:
        DATA
        DC8 "idle"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "main relay">`:
        DATA
        DC8 "main relay"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "alternator">`:
        DATA
        DC8 "alternator"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "A/C"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fuelPump">`:
        DATA
        DC8 "fuelPump"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "setting injection pin...">`:
        DATA
        DC8 73H, 65H, 74H, 74H, 69H, 6EH, 67H, 20H
        DC8 69H, 6EH, 6AH, 65H, 63H, 74H, 69H, 6FH
        DC8 6EH, 20H, 70H, 69H, 6EH, 5BH, 25H, 64H
        DC8 5DH, 20H, 74H, 6FH, 20H, 25H, 73H, 20H
        DC8 70H, 6CH, 65H, 61H, 73H, 65H, 20H, 73H
        DC8 61H, 76H, 65H, 26H, 72H, 65H, 73H, 74H
        DC8 61H, 72H, 74H, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "setting trigger pin[%...">`:
        DATA
        DC8 "setting trigger pin[%d] to %s please save&restart"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "setting EGT CS pin[%d...">`:
        DATA
        DC8 "setting EGT CS pin[%d] to %s please save&restart"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "setting trigger simul...">`:
        DATA
        DC8 73H, 65H, 74H, 74H, 69H, 6EH, 67H, 20H
        DC8 74H, 72H, 69H, 67H, 67H, 65H, 72H, 20H
        DC8 73H, 69H, 6DH, 75H, 6CH, 61H, 74H, 6FH
        DC8 72H, 20H, 70H, 69H, 6EH, 5BH, 25H, 64H
        DC8 5DH, 20H, 74H, 6FH, 20H, 25H, 73H, 20H
        DC8 70H, 6CH, 65H, 61H, 73H, 65H, 20H, 73H
        DC8 61H, 76H, 65H, 26H, 72H, 65H, 73H, 74H
        DC8 61H, 72H, 74H, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Error with [%s]">`:
        DATA
        DC8 "Error with [%s]"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "map"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "setting MAP to %s/%d">`:
        DATA
        DC8 "setting MAP to %s/%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "clt"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "setting CLT to %s/%d">`:
        DATA
        DC8 "setting CLT to %s/%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "iat"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "setting IAT to %s/%d">`:
        DATA
        DC8 "setting IAT to %s/%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "tps"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "setting TPS to %s/%d">`:
        DATA
        DC8 "setting TPS to %s/%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "setting logic input p...">`:
        DATA
        DC8 73H, 65H, 74H, 74H, 69H, 6EH, 67H, 20H
        DC8 6CH, 6FH, 67H, 69H, 63H, 20H, 69H, 6EH
        DC8 70H, 75H, 74H, 20H, 70H, 69H, 6EH, 5BH
        DC8 25H, 64H, 5DH, 20H, 74H, 6FH, 20H, 25H
        DC8 73H, 20H, 70H, 6CH, 65H, 61H, 73H, 65H
        DC8 20H, 73H, 61H, 76H, 65H, 26H, 72H, 65H
        DC8 73H, 74H, 61H, 72H, 74H, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Pin %s: [%s]">`:
        DATA
        DC8 "Pin %s: [%s]"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Setting timing map en...">`:
        DATA
        DC8 "Setting timing map entry %d:%d to %f"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Setting fuel map entr...">`:
        DATA
        DC8 "Setting fuel map entry %d:%d to %f"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid spi index %d">`:
        DATA
        DC8 "invalid spi index %d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fastadc">`:
        DATA
        DC8 "fastadc"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "step1limimter">`:
        DATA
        DC8 "step1limimter"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "serial">`:
        DATA
        DC8 "serial"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "stepperidle">`:
        DATA
        DC8 "stepperidle"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "trigger_only_front">`:
        DATA
        DC8 "trigger_only_front"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "two_wire_batch">`:
        DATA
        DC8 "two_wire_batch"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "joystick">`:
        DATA
        DC8 "joystick"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "HIP9011">`:
        DATA
        DC8 "HIP9011"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "altdebug">`:
        DATA
        DC8 "altdebug"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "tpic_advanced_mode">`:
        DATA
        DC8 "tpic_advanced_mode"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "knockdebug">`:
        DATA
        DC8 "knockdebug"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "altcontrol">`:
        DATA
        DC8 "altcontrol"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "sd"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "can"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "test_mode">`:
        DATA
        DC8 "test_mode"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "can_read">`:
        DATA
        DC8 "can_read"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "can_write">`:
        DATA
        DC8 "can_write"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "injection">`:
        DATA
        DC8 "injection"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "trigger_details">`:
        DATA
        DC8 "trigger_details"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ignition">`:
        DATA
        DC8 "ignition"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "self_stimulation">`:
        DATA
        DC8 "self_stimulation"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "engine_control">`:
        DATA
        DC8 "engine_control"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "map_avg">`:
        DATA
        DC8 "map_avg"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "mil"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fuel_pump">`:
        DATA
        DC8 "fuel_pump"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "tunerstudio">`:
        DATA
        DC8 "tunerstudio"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "wave_analyzer">`:
        DATA
        DC8 "wave_analyzer"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "idle_thread">`:
        DATA
        DC8 "idle_thread"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "manual_spinning">`:
        DATA
        DC8 "manual_spinning"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "cylinder_cleanup">`:
        DATA
        DC8 "cylinder_cleanup"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "unexpected [%s]">`:
        DATA
        DC8 "unexpected [%s]"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "[%s] %s">`:
        DATA
        DC8 "[%s] %s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "enabled">`:
        DATA
        DC8 "enabled"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "disabled">`:
        DATA
        DC8 "disabled"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "injection enabled">`:
        DATA
        DC8 "injection enabled"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "injection disabled">`:
        DATA
        DC8 "injection disabled"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ignition enabled">`:
        DATA
        DC8 "ignition enabled"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ignition disabled">`:
        DATA
        DC8 "ignition disabled"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "waveChartUsedSize=%d">`:
        DATA
        DC8 "waveChartUsedSize=%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "console mode jumper: %s">`:
        DATA
        DC8 "console mode jumper: %s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "board test mode jumpe...">`:
        DATA
        DC8 "board test mode jumper: %s"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "todo">`:
        DATA
        DC8 "todo"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "something">`:
        DATA
        DC8 "something"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "date">`:
        DATA
        DC8 "date"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "vsscoeff">`:
        DATA
        DC8 "vsscoeff"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "alt_t">`:
        DATA
        DC8 "alt_t"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "alt_offset">`:
        DATA
        DC8 "alt_offset"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "alt_p">`:
        DATA
        DC8 "alt_p"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "step1rpm">`:
        DATA
        DC8 "step1rpm"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "step1timing">`:
        DATA
        DC8 "step1timing"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "targetvbatt">`:
        DATA
        DC8 "targetvbatt"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "showconfig">`:
        DATA
        DC8 "showconfig"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "tempinfo">`:
        DATA
        DC8 "tempinfo"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "tpsinfo">`:
        DATA
        DC8 "tpsinfo"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "info">`:
        DATA
        DC8 "info"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_ignition_offset">`:
        DATA
        DC8 "set_ignition_offset"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_injection_offset">`:
        DATA
        DC8 "set_injection_offset"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_global_trigger_of...">`:
        DATA
        DC8 "set_global_trigger_offset_angle"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_sensor_chart_mode">`:
        DATA
        DC8 "set_sensor_chart_mode"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_fixed_mode_timing">`:
        DATA
        DC8 "set_fixed_mode_timing"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_timing_mode">`:
        DATA
        DC8 "set_timing_mode"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_engine_type">`:
        DATA
        DC8 "set_engine_type"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_injection_pin_mode">`:
        DATA
        DC8 "set_injection_pin_mode"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_ignition_pin_mode">`:
        DATA
        DC8 "set_ignition_pin_mode"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_idle_pin_mode">`:
        DATA
        DC8 "set_idle_pin_mode"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_fuel_pump_pin_mode">`:
        DATA
        DC8 "set_fuel_pump_pin_mode"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_malfunction_indic...">`:
        DATA
        DC8 "set_malfunction_indicator_pin_mode"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_operation_mode">`:
        DATA
        DC8 "set_operation_mode"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_global_fuel_corre...">`:
        DATA
        DC8 "set_global_fuel_correction"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_cranking_fuel">`:
        DATA
        DC8 "set_cranking_fuel"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_cranking_rpm">`:
        DATA
        DC8 "set_cranking_rpm"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_cranking_timing_a...">`:
        DATA
        DC8 "set_cranking_timing_angle"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_cranking_charge_a...">`:
        DATA
        DC8 "set_cranking_charge_angle"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_ignition_mode">`:
        DATA
        DC8 "set_ignition_mode"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_cranking_injectio...">`:
        DATA
        DC8 "set_cranking_injection_mode"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_injection_mode">`:
        DATA
        DC8 "set_injection_mode"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_whole_phase_map">`:
        DATA
        DC8 "set_whole_phase_map"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_whole_timing_map">`:
        DATA
        DC8 "set_whole_timing_map"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_whole_fuel_map">`:
        DATA
        DC8 "set_whole_fuel_map"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_whole_ve_map">`:
        DATA
        DC8 "set_whole_ve_map"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_fuel_map">`:
        DATA
        DC8 "set_fuel_map"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_timing_map">`:
        DATA
        DC8 "set_timing_map"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_rpm_hard_limit">`:
        DATA
        DC8 "set_rpm_hard_limit"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_firing_order">`:
        DATA
        DC8 "set_firing_order"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_algorithm">`:
        DATA
        DC8 "set_algorithm"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "stopengine">`:
        DATA
        DC8 "stopengine"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "enable_injection">`:
        DATA
        DC8 "enable_injection"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "disable_injection">`:
        DATA
        DC8 "disable_injection"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "enable_ignition">`:
        DATA
        DC8 "enable_ignition"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "disable_ignition">`:
        DATA
        DC8 "disable_ignition"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "enable_spi">`:
        DATA
        DC8 "enable_spi"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "disable_spi">`:
        DATA
        DC8 "disable_spi"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "enable">`:
        DATA
        DC8 "enable"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "disable">`:
        DATA
        DC8 "disable"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_toothed_wheel">`:
        DATA
        DC8 "set_toothed_wheel"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_trigger_type">`:
        DATA
        DC8 "set_trigger_type"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_vbatt_divider">`:
        DATA
        DC8 "set_vbatt_divider"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_injector_lag">`:
        DATA
        DC8 "set_injector_lag"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_clt_bias">`:
        DATA
        DC8 "set_clt_bias"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_iat_bias">`:
        DATA
        DC8 "set_iat_bias"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_idle_solenoid_freq">`:
        DATA
        DC8 "set_idle_solenoid_freq"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_fan">`:
        DATA
        DC8 "set_fan"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "showpin">`:
        DATA
        DC8 "showpin"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_injection_pin">`:
        DATA
        DC8 "set_injection_pin"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_ignition_pin">`:
        DATA
        DC8 "set_ignition_pin"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_trigger_input_pin">`:
        DATA
        DC8 "set_trigger_input_pin"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_trigger_simulator...">`:
        DATA
        DC8 "set_trigger_simulator_pin"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_egt_cs_pin">`:
        DATA
        DC8 "set_egt_cs_pin"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_egt_spi">`:
        DATA
        DC8 "set_egt_spi"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_trigger_simulator...">_1`:
        DATA
        DC8 "set_trigger_simulator_mode"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_fuel_pump_pin">`:
        DATA
        DC8 "set_fuel_pump_pin"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_ac_pin">`:
        DATA
        DC8 "set_ac_pin"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_alternator_pin">`:
        DATA
        DC8 "set_alternator_pin"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_idle_pin">`:
        DATA
        DC8 "set_idle_pin"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_main_relay_pin">`:
        DATA
        DC8 "set_main_relay_pin"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "set"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "get"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_analog_input_pin">`:
        DATA
        DC8 "set_analog_input_pin"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_logic_input_pin">`:
        DATA
        DC8 "set_logic_input_pin"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_pot_spi">`:
        DATA
        DC8 "set_pot_spi"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "settings control">`:
        DATA
        DC8 "settings control"
        DC8 0, 0, 0

        END
// 1066 
// 
// 1 076 bytes in section .bss
//     4 bytes in section .init_array
// 4 616 bytes in section .rodata
// 9 320 bytes in section .text
// 
// 9 324 bytes of CODE  memory
// 4 616 bytes of CONST memory
// 1 076 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
