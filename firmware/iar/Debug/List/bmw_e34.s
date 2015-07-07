///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:39 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       bmw_e34.cpp                                          /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       bmw_e34.cpp -lCN F:\stuff\rusefi_sourceforge\firmwar /
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
//                       bmw_e34.s                                            /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME bmw_e34

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z12setAlgorithm18engine_load_mode_e
        EXTERN _Z16setConstantDwellf
        EXTERN _Z16setOperationModeP22engine_configuration_s16operation_mode_e
        EXTERN _Z19setWholeTimingTablef
        EXTERN _Z26setThermistorConfigurationP14ThermistorConfffffff
        EXTERN boardConfiguration
        EXTERN engineConfiguration

        PUBLIC _Z9setBmwE34v
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\config\engines\bmw_e34.cpp
//    1 /**
//    2  * @file bmw_e34.cpp
//    3  *
//    4  * set_engine_type 25
//    5  *
//    6  * Coil pairs: 1-6, 2-5, 3-4
//    7  *
//    8  * Injector groups: 1,2,3 & 4,5,6
//    9  *
//   10  * @date Nov 5, 2014
//   11  * @author Andrey Belomutskiy, (c) 2012-2015
//   12  */
//   13 
//   14 #include "global.h"
//   15 #include "bmw_e34.h"
//   16 #include "thermistors.h"
//   17 #include "engine_math.h"
//   18 
//   19 EXTERN_ENGINE
//   20 ;
//   21 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z9setBmwE34v
        THUMB
//   22 void setBmwE34(DECLARE_ENGINE_PARAMETER_F) {
_Z9setBmwE34v:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//   23 	// chartsize 450
//   24 	engineConfiguration->engineChartSize = 450;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        MOV      R1,#+450
        STR      R1,[R0, #+1512]
//   25 
//   26 //	setOperationMode(engineConfiguration, FOUR_STROKE_CAM_SENSOR);
//   27 //	engineConfiguration->trigger.type = TT_ONE_PLUS_TOOTHED_WHEEL_60_2;
//   28 //	engineConfiguration->injectionMode = IM_SEQUENTIAL;
//   29 //	boardConfiguration->triggerInputPins[0] = GPIOC_6;
//   30 //	boardConfiguration->triggerInputPins[1] = GPIOA_5;
//   31 
//   32 //Base engine setting
//   33 	engineConfiguration->specs.cylindersCount = 6;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+6
        STR      R1,[R0, #+404]
//   34 	engineConfiguration->specs.displacement = 2.91;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setBmwE34_0+0x14  ;; 0x403a3d71
        STR      R1,[R0, #+400]
//   35 	engineConfiguration->specs.firingOrder = FO_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+4
        STR      R1,[R0, #+408]
//   36 	engineConfiguration->injectionMode = IM_BATCH;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+432]
//   37 	engineConfiguration->twoWireBatch = true;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        ORRS     R0,R0,#0x20
        LDR.N    R1,??setBmwE34_0+0x10
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1509]
//   38 	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+444]
//   39 	engineConfiguration->useOnlyFrontForTrigger = true;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        ORRS     R0,R0,#0x40
        LDR.N    R1,??setBmwE34_0+0x10
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1509]
//   40 
//   41 	// Trigger configuration
//   42 //	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
//   43 	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
        MOVS     R1,#+1
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
          CFI FunCall _Z16setOperationModeP22engine_configuration_s16operation_mode_e
        BL       _Z16setOperationModeP22engine_configuration_s16operation_mode_e
//   44 	engineConfiguration->globalTriggerAngleOffset = 84;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setBmwE34_0+0x18  ;; 0x42a80000
        STR      R1,[R0, #+460]
//   45 
//   46 	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+536]
//   47 	engineConfiguration->trigger.customTotalToothCount = 30;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+30
        STR      R1,[R0, #+544]
//   48 	engineConfiguration->trigger.customSkippedToothCount = 1;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+548]
//   49 
//   50 
//   51 	// Injection settings
//   52 	engineConfiguration->injector.lag = 1.15;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setBmwE34_0+0x1C  ;; 0x3f933333
        STR      R1,[R0, #+12]
//   53 	engineConfiguration->injector.flow = 750;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setBmwE34_0+0x20  ;; 0x443b8000
        STR      R1,[R0, #+8]
//   54 
//   55 	// General settings
//   56 	boardConfiguration->tunerStudioSerialSpeed = 57600;
        LDR.N    R0,??setBmwE34_0+0x24
        LDR      R0,[R0, #+0]
        MOV      R1,#+57600
        STR      R1,[R0, #+332]
//   57 	engineConfiguration->rpmHardLimit = 7000;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        MOVW     R1,#+7000
        STR      R1,[R0, #+420]
//   58 	setAlgorithm(LM_SPEED_DENSITY PASS_ENGINE_PARAMETER);
        MOVS     R0,#+3
          CFI FunCall _Z12setAlgorithm18engine_load_mode_e
        BL       _Z12setAlgorithm18engine_load_mode_e
//   59 
//   60 	// for best performance at high RPM, we need to turn off 'Engine Sniffer' and 'Sensor Sniffer'
//   61 	boardConfiguration->sensorChartMode = SC_OFF;
        LDR.N    R0,??setBmwE34_0+0x24
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+784]
//   62 	engineConfiguration->isEngineChartEnabled = false;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        ANDS     R0,R0,#0xDF
        LDR.N    R1,??setBmwE34_0+0x10
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1496]
//   63 
//   64 	engineConfiguration->isCylinderCleanupEnabled = false;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        ANDS     R0,R0,#0xFB
        LDR.N    R1,??setBmwE34_0+0x10
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1508]
//   65 	engineConfiguration->isInjectionEnabled = true;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        ORRS     R0,R0,#0x1
        LDR.N    R1,??setBmwE34_0+0x10
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1508]
//   66 	engineConfiguration->isIgnitionEnabled = true;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        ORRS     R0,R0,#0x2
        LDR.N    R1,??setBmwE34_0+0x10
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1508]
//   67 
//   68 	setConstantDwell(3 PASS_ENGINE_PARAMETER); // a bit shorter dwell
        VMOV.F32 S0,#3.0
          CFI FunCall _Z16setConstantDwellf
        BL       _Z16setConstantDwellf
//   69 	engineConfiguration->ignMathCalculateAtIndex = 14;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+14
        STR      R1,[R0, #+1524]
//   70 
//   71 	engineConfiguration->mapAveragingSchedulingAtIndex = 6;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+6
        STR      R1,[R0, #+1648]
//   72 
//   73 	// Cranking
//   74 	engineConfiguration->cranking.rpm = 600;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        MOV      R1,#+600
        STRH     R1,[R0, #+96]
//   75 	engineConfiguration->crankingInjectionMode = IM_BATCH;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+428]
//   76 	engineConfiguration->cranking.baseFuel = 7;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setBmwE34_0+0x28  ;; 0x40e00000
        STR      R1,[R0, #+92]
//   77 	engineConfiguration->useConstantDwellDuringCranking = true;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        ORRS     R0,R0,#0x10
        LDR.N    R1,??setBmwE34_0+0x10
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1496]
//   78 	engineConfiguration->ignitionDwellForCrankingMs = 6;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setBmwE34_0+0x2C  ;; 0x40c00000
        STR      R1,[R0, #+104]
//   79 
//   80 	// Inputs configuration
//   81 	engineConfiguration->analogInputDividerCoefficient = 1.52;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setBmwE34_0+0x30  ;; 0x3fc28f5c
        STR      R1,[R0, #+464]
//   82 	engineConfiguration->vbattDividerCoeff = 5.33;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setBmwE34_0+0x34  ;; 0x40aa8f5c
        STR      R1,[R0, #+468]
//   83 	engineConfiguration->vbattAdcChannel = EFI_ADC_15; // PC5
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+15
        STR      R1,[R0, #+472]
//   84 
//   85 	engineConfiguration->tpsAdcChannel = EFI_ADC_3;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+3
        STR      R1,[R0, #+524]
//   86 
//   87 
//   88 	boardConfiguration->triggerInputPins[0] = GPIOA_5;
        LDR.N    R0,??setBmwE34_0+0x24
        LDR      R0,[R0, #+0]
        MOVS     R1,#+5
        STR      R1,[R0, #+296]
//   89 	boardConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
        LDR.N    R0,??setBmwE34_0+0x24
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+300]
//   90 
//   91 	setWholeTimingTable(25 PASS_ENGINE_PARAMETER);
        VMOV.F32 S0,#25.0
          CFI FunCall _Z19setWholeTimingTablef
        BL       _Z19setWholeTimingTablef
//   92 
//   93 	board_configuration_s *bc = &engineConfiguration->bc;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        ADDS     R4,R0,#+624
//   94 	bc->malfunctionIndicatorPin = GPIO_UNASSIGNED;
        MOVS     R0,#+80
        STR      R0,[R4, #+136]
//   95 
//   96 //	bc->isFastAdcEnabled = true;
//   97 
//   98 	bc->injectionPinMode = OM_INVERTED;
        MOVS     R0,#+1
        STR      R0,[R4, #+80]
//   99 	bc->injectionPins[0] = GPIOB_8; // #1
        MOVS     R0,#+24
        STR      R0,[R4, #+32]
//  100 	bc->injectionPins[1] = GPIOE_2; // #2
        MOVS     R0,#+66
        STR      R0,[R4, #+36]
//  101 	bc->injectionPins[2] = GPIOE_3; // #3
        MOVS     R0,#+67
        STR      R0,[R4, #+40]
//  102 	bc->injectionPins[3] = GPIOE_4; // #4
        MOVS     R0,#+68
        STR      R0,[R4, #+44]
//  103 	bc->injectionPins[4] = GPIOE_5; // #5
        MOVS     R0,#+69
        STR      R0,[R4, #+48]
//  104 	bc->injectionPins[5] = GPIOE_6; // #6
        MOVS     R0,#+70
        STR      R0,[R4, #+52]
//  105 
//  106 	bc->ignitionPinMode = OM_INVERTED;
        MOVS     R0,#+1
        STR      R0,[R4, #+132]
//  107 
//  108 	bc->ignitionPins[0] = GPIOB_5; // #1
        MOVS     R0,#+21
        STR      R0,[R4, #+84]
//  109 	bc->ignitionPins[2] = GPIOB_6; // #3
        MOVS     R0,#+22
        STR      R0,[R4, #+92]
//  110 	bc->ignitionPins[4] = GPIOB_7; // #5
        MOVS     R0,#+23
        STR      R0,[R4, #+100]
//  111 
//  112 	bc->triggerErrorPin = GPIO_UNASSIGNED;
        MOVS     R0,#+80
        STR      R0,[R4, #+600]
//  113 
//  114 	// clutch up
//  115 	boardConfiguration->clutchUpPin = GPIOD_3;
        LDR.N    R0,??setBmwE34_0+0x24
        LDR      R0,[R0, #+0]
        MOVS     R1,#+51
        STR      R1,[R0, #+796]
//  116 	boardConfiguration->clutchUpPinMode = PI_PULLUP;
        LDR.N    R0,??setBmwE34_0+0x24
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+800]
//  117 
//  118 	// fuel pump
//  119 	boardConfiguration->fuelPumpPin = GPIOD_4;
        LDR.N    R0,??setBmwE34_0+0x24
        LDR      R0,[R0, #+0]
        MOVS     R1,#+52
        STR      R1,[R0, #+24]
//  120 
//  121 	// idle
//  122 	boardConfiguration->idle.solenoidPin = GPIOC_14;
        LDR.N    R0,??setBmwE34_0+0x24
        LDR      R0,[R0, #+0]
        MOVS     R1,#+46
        STR      R1,[R0, #+0]
//  123 	boardConfiguration->idle.solenoidPinMode = OM_INVERTED;
        LDR.N    R0,??setBmwE34_0+0x24
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+4]
//  124 	boardConfiguration->idle.solenoidFrequency = 300;
        LDR.N    R0,??setBmwE34_0+0x24
        LDR      R0,[R0, #+0]
        MOV      R1,#+300
        STR      R1,[R0, #+8]
//  125 	boardConfiguration->idlePosition = 50; // set_idle_pwm 50
        LDR.N    R0,??setBmwE34_0+0x24
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setBmwE34_0+0x38  ;; 0x42480000
        STR      R1,[R0, #+20]
//  126 
//  127 	// disable sd_card
//  128 	boardConfiguration->sdCardCsPin = GPIO_UNASSIGNED;
        LDR.N    R0,??setBmwE34_0+0x24
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+324]
//  129 	boardConfiguration->is_enabled_spi_2 = false;
        LDR.N    R0,??setBmwE34_0+0x24
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        ANDS     R0,R0,#0xFD
        LDR.N    R1,??setBmwE34_0+0x24
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+384]
//  130 	boardConfiguration->is_enabled_spi_3 = false;
        LDR.N    R0,??setBmwE34_0+0x24
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        ANDS     R0,R0,#0xFB
        LDR.N    R1,??setBmwE34_0+0x24
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+384]
//  131 	boardConfiguration->max31855spiDevice = SPI_NONE;
        LDR.N    R0,??setBmwE34_0+0x24
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+448]
//  132 
//  133 	// turbocharger boost control solenoid: TODO output: GPIOE_6
//  134 	// water injection #1 TODO GPIOD_7
//  135 	// water injection #2 TODO GPIOE_2
//  136 
//  137 	/**
//  138 	 * emulating the 60-0 trigger takes some resources, let's keep it slow by default
//  139 	 * rpm 200
//  140 	 */
//  141 	bc->triggerSimulatorFrequency = 200;
        MOVS     R0,#+200
        STR      R0,[R4, #+208]
//  142 
//  143 	// Configurating sensors:
//  144 
//  145 	// map
//  146 	engineConfiguration->map.sensor.type = MT_MPX4250;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+248]
//  147 
//  148 	// thermistors
//  149 	engineConfiguration->hasCltSensor = true;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        ORRS     R0,R0,#0x80
        LDR.N    R1,??setBmwE34_0+0x10
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1496]
//  150 	engineConfiguration->hasIatSensor = true;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        ORRS     R0,R0,#0x2
        LDR.N    R1,??setBmwE34_0+0x10
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1496]
//  151 	setThermistorConfiguration(&engineConfiguration->clt, -10, 9300, 20, 2500, 80, 335);
        VLDR.W   S5,??setBmwE34_0  ;; 0x43a78000
        VLDR.W   S4,??setBmwE34_0+0x4  ;; 0x42a00000
        VLDR.W   S3,??setBmwE34_0+0x8  ;; 0x451c4000
        VMOV.F32 S2,#20.0
        VLDR.W   S1,??setBmwE34_0+0xC  ;; 0x46115000
        VMOV.F32 S0,#-10.0
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+256
          CFI FunCall _Z26setThermistorConfigurationP14ThermistorConfffffff
        BL       _Z26setThermistorConfigurationP14ThermistorConfffffff
//  152 	engineConfiguration->iat.config.bias_resistor = 2200;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setBmwE34_0+0x3C  ;; 0x45098000
        STR      R1,[R0, #+312]
//  153 	setThermistorConfiguration(&engineConfiguration->iat, -10, 9300, 20, 2500, 80, 335);
        VLDR.W   S5,??setBmwE34_0  ;; 0x43a78000
        VLDR.W   S4,??setBmwE34_0+0x4  ;; 0x42a00000
        VLDR.W   S3,??setBmwE34_0+0x8  ;; 0x451c4000
        VMOV.F32 S2,#20.0
        VLDR.W   S1,??setBmwE34_0+0xC  ;; 0x46115000
        VMOV.F32 S0,#-10.0
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+288
          CFI FunCall _Z26setThermistorConfigurationP14ThermistorConfffffff
        BL       _Z26setThermistorConfigurationP14ThermistorConfffffff
//  154 	engineConfiguration->clt.config.bias_resistor = 2200;
        LDR.N    R0,??setBmwE34_0+0x10
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setBmwE34_0+0x3C  ;; 0x45098000
        STR      R1,[R0, #+280]
//  155 
//  156 //	/**
//  157 //	 * This saves a couple of ticks in trigger emulation methods
//  158 //	 * TODO: add some smart logic to detect unneeded trigger simulation pins?
//  159 //	 * TODO: but probably not worth it
//  160 //	 */
//  161 //	bc->triggerSimulatorPins[1] = GPIO_UNASSIGNED;
//  162 
//  163 	boardConfiguration->triggerSimulatorPins[0] = GPIOD_1;
        LDR.N    R0,??setBmwE34_0+0x24
        LDR      R0,[R0, #+0]
        MOVS     R1,#+49
        STR      R1,[R0, #+352]
//  164 	boardConfiguration->triggerSimulatorPins[1] = GPIOD_2;
        LDR.N    R0,??setBmwE34_0+0x24
        LDR      R0,[R0, #+0]
        MOVS     R1,#+50
        STR      R1,[R0, #+356]
//  165 	bc->triggerSimulatorPins[2] = GPIO_UNASSIGNED;
        MOVS     R0,#+80
        STR      R0,[R4, #+360]
//  166 }
        POP      {R4,PC}          ;; return
        DATA
??setBmwE34_0:
        DC32     0x43a78000
        DC32     0x42a00000
        DC32     0x451c4000
        DC32     0x46115000
        DC32     engineConfiguration
        DC32     0x403a3d71
        DC32     0x42a80000
        DC32     0x3f933333
        DC32     0x443b8000
        DC32     boardConfiguration
        DC32     0x40e00000
        DC32     0x40c00000
        DC32     0x3fc28f5c
        DC32     0x40aa8f5c
        DC32     0x42480000
        DC32     0x45098000
          CFI EndBlock cfiBlock0

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
// 
// 872 bytes in section .text
// 
// 872 bytes of CODE memory
//
//Errors: none
//Warnings: 1
