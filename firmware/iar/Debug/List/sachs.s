///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:10:01 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       sachs.cpp                                            /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       sachs.cpp -lCN F:\stuff\rusefi_sourceforge\firmware\ /
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
//                       sachs.s                                              /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME sachs

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z12setTableBin2Pfifff
        EXTERN _Z13setFuelRpmBinff
        EXTERN _Z15setTimingRpmBinff
        EXTERN _Z16setOperationModeP22engine_configuration_s16operation_mode_e
        EXTERN _Z24convertVoltageTo10bitADCf
        EXTERN boardConfiguration
        EXTERN config
        EXTERN engineConfiguration

        PUBLIC _Z8setSachsv
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\config\engines\sachs.cpp
//    1 /**
//    2  * @file	sachs.cpp
//    3  *
//    4  * set_engine_type 29
//    5  * http://rusefi.com/forum/viewtopic.php?f=3&t=396
//    6  *
//    7  * @date Jan 26, 2015
//    8  * @author Andrey Belomutskiy, (c) 2012-2015
//    9  */
//   10 
//   11 #include "sachs.h"
//   12 #include "allsensors.h"
//   13 #include "engine_math.h"
//   14 
//   15 EXTERN_ENGINE;
//   16 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z8setSachsv
        THUMB
//   17 void setSachs(DECLARE_ENGINE_PARAMETER_F) {
_Z8setSachsv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   18 	engineConfiguration->specs.displacement = 0.1; // 100cc
        LDR.N    R0,??setSachs_0+0x10
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setSachs_0+0x14  ;; 0x3dcccccd
        STR      R1,[R0, #+400]
//   19 	engineConfiguration->specs.cylindersCount = 1;
        LDR.N    R0,??setSachs_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+404]
//   20 	engineConfiguration->engineCycle = 360;
        LDR.N    R0,??setSachs_0+0x10
        LDR      R0,[R0, #+0]
        MOV      R1,#+360
        STR      R1,[R0, #+416]
//   21 
//   22 	setOperationMode(engineConfiguration, TWO_STROKE);
        MOVS     R1,#+3
        LDR.N    R0,??setSachs_0+0x10
        LDR      R0,[R0, #+0]
          CFI FunCall _Z16setOperationModeP22engine_configuration_s16operation_mode_e
        BL       _Z16setOperationModeP22engine_configuration_s16operation_mode_e
//   23 	engineConfiguration->specs.firingOrder = FO_ONE_CYLINDER;
        LDR.N    R0,??setSachs_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+408]
//   24 	engineConfiguration->engineChartSize = 400;
        LDR.N    R0,??setSachs_0+0x10
        LDR      R0,[R0, #+0]
        MOV      R1,#+400
        STR      R1,[R0, #+1512]
//   25 
//   26 	 // set_injection_offset 0
//   27 	engineConfiguration->injectionOffset = 0;
        LDR.N    R0,??setSachs_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+436]
//   28 
//   29 
//   30 	/**
//   31 	 * 50/2 trigger
//   32 	 */
//   33 	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL;
        LDR.N    R0,??setSachs_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+536]
//   34 	engineConfiguration->trigger.customTotalToothCount = 50;
        LDR.N    R0,??setSachs_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+50
        STR      R1,[R0, #+544]
//   35 	engineConfiguration->trigger.customSkippedToothCount = 2;
        LDR.N    R0,??setSachs_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+548]
//   36 
//   37 	engineConfiguration->hasIatSensor = false;
        LDR.N    R0,??setSachs_0+0x10
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        ANDS     R0,R0,#0xFD
        LDR.N    R1,??setSachs_0+0x10
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1496]
//   38 	engineConfiguration->hasMapSensor = false;
        LDR.N    R0,??setSachs_0+0x10
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        ANDS     R0,R0,#0xFE
        LDR.N    R1,??setSachs_0+0x10
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1496]
//   39 	engineConfiguration->hasBaroSensor = false;
        LDR.N    R0,??setSachs_0+0x10
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        ANDS     R0,R0,#0xFB
        LDR.N    R1,??setSachs_0+0x10
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1496]
//   40 	engineConfiguration->hasAfrSensor = false;
        LDR.N    R0,??setSachs_0+0x10
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        ANDS     R0,R0,#0xF7
        LDR.N    R1,??setSachs_0+0x10
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1496]
//   41 	engineConfiguration->hasCltSensor = false;
        LDR.N    R0,??setSachs_0+0x10
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        ANDS     R0,R0,#0x7F
        LDR.N    R1,??setSachs_0+0x10
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1496]
//   42 	boardConfiguration->useSerialPort = false;
        LDR.N    R0,??setSachs_0+0x18
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+385]
        ANDS     R0,R0,#0xFE
        LDR.N    R1,??setSachs_0+0x18
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+385]
//   43 
//   44 	// Frankenstein analog input #1: PA1 adc1 MAP
//   45 	// Frankenstein analog input #2: PA3 adc3 TPS
//   46 	// Frankenstein analog input #3: PC3 adc13 IAT
//   47 	// Frankenstein analog input #4: PC1 adc11 CLT
//   48 	// Frankenstein analog input #5: PA0 adc0 O2
//   49 	// Frankenstein analog input #6: PC2 adc12
//   50 	// Frankenstein analog input #7: PA4 adc4
//   51 	// Frankenstein analog input #8: PA2 adc2
//   52 	// Frankenstein analog input #9: PA6 adc6
//   53 	// Frankenstein analog input #10: PA7 adc7
//   54 	// Frankenstein analog input #11: PC4 adc14
//   55 	// Frankenstein analog input #12: PC5 adc15
//   56 
//   57 	engineConfiguration->tpsAdcChannel = EFI_ADC_3;
        LDR.N    R0,??setSachs_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+3
        STR      R1,[R0, #+524]
//   58 	engineConfiguration->vbattAdcChannel = EFI_ADC_NONE;
        LDR.N    R0,??setSachs_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+16
        STR      R1,[R0, #+472]
//   59 
//   60 	/**
//   61 	 * TPS 0% 0.9v
//   62 	 * TPS 100% 2.34v
//   63 	 */
//   64 	engineConfiguration->tpsMin = convertVoltageTo10bitADC(1.250);
        VMOV.F32 S0,#1.25
          CFI FunCall _Z24convertVoltageTo10bitADCf
        BL       _Z24convertVoltageTo10bitADCf
        LDR.N    R1,??setSachs_0+0x10
        LDR      R1,[R1, #+0]
        STRH     R0,[R1, #+82]
//   65 	engineConfiguration->tpsMax = convertVoltageTo10bitADC(4.538);
        VLDR.W   S0,??setSachs_0  ;; 0x4091374c
          CFI FunCall _Z24convertVoltageTo10bitADCf
        BL       _Z24convertVoltageTo10bitADCf
        LDR.N    R1,??setSachs_0+0x10
        LDR      R1,[R1, #+0]
        STRH     R0,[R1, #+84]
//   66 
//   67 
//   68 	// Frankenstein: low side - out #1: PC14
//   69 	// Frankenstein: low side - out #2: PC15
//   70 	// Frankenstein: low side - out #3: PE6
//   71 	// Frankenstein: low side - out #4: PC13
//   72 	// Frankenstein: low side - out #5: PE4
//   73 	// Frankenstein: low side - out #6: PE5
//   74 	// Frankenstein: low side - out #7: PE2
//   75 	// Frankenstein: low side - out #8: PE3
//   76 	// Frankenstein: low side - out #9: PE0
//   77 	// Frankenstein: low side - out #10: PE1
//   78 	// Frankenstein: low side - out #11: PB8
//   79 	// Frankenstein: low side - out #12: PB9
//   80 
//   81 	boardConfiguration->triggerInputPins[0] = GPIOA_5;
        LDR.N    R0,??setSachs_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+5
        STR      R1,[R0, #+296]
//   82 	boardConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
        LDR.N    R0,??setSachs_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+300]
//   83 
//   84 	boardConfiguration->injectionPins[0] = GPIOC_15;
        LDR.N    R0,??setSachs_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+47
        STR      R1,[R0, #+32]
//   85 
//   86 	boardConfiguration->fuelPumpPin = GPIOE_6;
        LDR.N    R0,??setSachs_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+70
        STR      R1,[R0, #+24]
//   87 
//   88 	// todo: extract a method? figure out something smarter
//   89 	setFuelRpmBin(800, 15000 PASS_ENGINE_PARAMETER);
        VLDR.W   S1,??setSachs_0+0x4  ;; 0x466a6000
        VLDR.W   S0,??setSachs_0+0x8  ;; 0x44480000
          CFI FunCall _Z13setFuelRpmBinff
        BL       _Z13setFuelRpmBinff
//   90 	setTimingRpmBin(800, 15000 PASS_ENGINE_PARAMETER);
        VLDR.W   S1,??setSachs_0+0x4  ;; 0x466a6000
        VLDR.W   S0,??setSachs_0+0x8  ;; 0x44480000
          CFI FunCall _Z15setTimingRpmBinff
        BL       _Z15setTimingRpmBinff
//   91 	setTableBin2(config->veRpmBins, FUEL_RPM_COUNT, 15000, 7000, 1);
        VMOV.F32 S2,#1.0
        VLDR.W   S1,??setSachs_0+0xC  ;; 0x45dac000
        VLDR.W   S0,??setSachs_0+0x4  ;; 0x466a6000
        MOVS     R1,#+16
        LDR.N    R0,??setSachs_0+0x1C
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+13824
        ADDS     R0,R0,#+248
          CFI FunCall _Z12setTableBin2Pfifff
        BL       _Z12setTableBin2Pfifff
//   92 	setTableBin2(config->afrRpmBins, FUEL_RPM_COUNT, 15000, 7000, 1);
        VMOV.F32 S2,#1.0
        VLDR.W   S1,??setSachs_0+0xC  ;; 0x45dac000
        VLDR.W   S0,??setSachs_0+0x4  ;; 0x466a6000
        MOVS     R1,#+16
        LDR.N    R0,??setSachs_0+0x1C
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+15104
        ADDS     R0,R0,#+120
          CFI FunCall _Z12setTableBin2Pfifff
        BL       _Z12setTableBin2Pfifff
//   93 
//   94 	engineConfiguration->hasFrequencyReportingMapSensor = true;
        LDR.N    R0,??setSachs_0+0x10
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1498]
        ORRS     R0,R0,#0x4
        LDR.N    R1,??setSachs_0+0x10
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1498]
//   95 	boardConfiguration->frequencyReportingMapInputPin = GPIOC_6;
        LDR.N    R0,??setSachs_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+38
        STR      R1,[R0, #+868]
//   96 	boardConfiguration->mapFrequency100Kpa = 159;
        LDR.N    R0,??setSachs_0+0x18
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setSachs_0+0x20  ;; 0x431f0000
        STR      R1,[R0, #+156]
//   97 	boardConfiguration->mapFrequency0Kpa = 80;
        LDR.N    R0,??setSachs_0+0x18
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setSachs_0+0x24  ;; 0x42a00000
        STR      R1,[R0, #+152]
//   98 }
        POP      {R0,PC}          ;; return
        Nop      
        DATA
??setSachs_0:
        DC32     0x4091374c
        DC32     0x466a6000
        DC32     0x44480000
        DC32     0x45dac000
        DC32     engineConfiguration
        DC32     0x3dcccccd
        DC32     boardConfiguration
        DC32     config
        DC32     0x431f0000
        DC32     0x42a00000
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
// 488 bytes in section .text
// 
// 488 bytes of CODE memory
//
//Errors: none
//Warnings: 1
