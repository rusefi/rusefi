///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:54 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       mazda_miata_nb.cpp                                   /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       mazda_miata_nb.cpp -lCN F:\stuff\rusefi_sourceforge\ /
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
//                       mazda_miata_nb.s                                     /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME mazda_miata_nb

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z15setWholeFuelMapf
        EXTERN _Z26setThermistorConfigurationP14ThermistorConfffffff
        EXTERN boardConfiguration
        EXTERN engineConfiguration

        PUBLIC _Z34setMazdaMiataNbEngineConfigurationv
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\config\engines\mazda_miata_nb.cpp
//    1 /**
//    2  * @file	mazda_miata_nb.cpp
//    3  *
//    4  * MAZDA_MIATA_NB = 9
//    5  *
//    6  * @date Feb 18, 2014
//    7  * @author Andrey Belomutskiy, (c) 2012-2015
//    8  */
//    9 
//   10 #include "mazda_miata_nb.h"
//   11 #include "thermistors.h"
//   12 
//   13 EXTERN_ENGINE;
//   14 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z34setMazdaMiataNbEngineConfigurationv
        THUMB
//   15 void setMazdaMiataNbEngineConfiguration(DECLARE_ENGINE_PARAMETER_F) {
_Z34setMazdaMiataNbEngineConfigurationv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   16 	// set_rpm_hard_limit 3000
//   17 	engineConfiguration->rpmHardLimit = 3000; // yes, 3k. let's play it safe for now
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x28
        LDR      R0,[R0, #+0]
        MOVW     R1,#+3000
        STR      R1,[R0, #+420]
//   18 
//   19 	engineConfiguration->trigger.type = TT_MAZDA_MIATA_NB;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x28
        LDR      R0,[R0, #+0]
        MOVS     R1,#+4
        STR      R1,[R0, #+536]
//   20 
//   21 	engineConfiguration->globalTriggerAngleOffset = 276;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x28
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setMazdaMiataNbEngineConfiguration_0+0x2C  ;; 0x438a0000
        STR      R1,[R0, #+460]
//   22 
//   23 	// set_cranking_injection_mode 0
//   24 	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x28
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+428]
//   25 	// set_injection_mode 1
//   26 	engineConfiguration->injectionMode = IM_SEQUENTIAL;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x28
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+432]
//   27 	// set_ignition_mode 2
//   28 	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x28
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+444]
//   29 	// set_firing_order 2
//   30 	engineConfiguration->specs.firingOrder = FO_1_THEN_3_THEN_4_THEN2;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x28
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+408]
//   31 
//   32 	setThermistorConfiguration(&engineConfiguration->clt, 0, 32500, 30, 7550, 100, 700);
        VLDR.W   S5,??setMazdaMiataNbEngineConfiguration_0  ;; 0x442f0000
        VLDR.W   S4,??setMazdaMiataNbEngineConfiguration_0+0x4  ;; 0x42c80000
        VLDR.W   S3,??setMazdaMiataNbEngineConfiguration_0+0x8  ;; 0x45ebf000
        VMOV.F32 S2,#30.0
        VLDR.W   S1,??setMazdaMiataNbEngineConfiguration_0+0xC  ;; 0x46fde800
        VLDR.W   S0,??setMazdaMiataNbEngineConfiguration_0+0x10  ;; 0x0
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x28
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+256
          CFI FunCall _Z26setThermistorConfigurationP14ThermistorConfffffff
        BL       _Z26setThermistorConfigurationP14ThermistorConfffffff
//   33 	engineConfiguration->clt.config.bias_resistor = 2700;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x28
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setMazdaMiataNbEngineConfiguration_0+0x30  ;; 0x4528c000
        STR      R1,[R0, #+280]
//   34 
//   35 	setThermistorConfiguration(&engineConfiguration->iat, -10, 160310, 60, 7700, 120.00, 1180);
        VLDR.W   S5,??setMazdaMiataNbEngineConfiguration_0+0x14  ;; 0x44938000
        VLDR.W   S4,??setMazdaMiataNbEngineConfiguration_0+0x18  ;; 0x42f00000
        VLDR.W   S3,??setMazdaMiataNbEngineConfiguration_0+0x1C  ;; 0x45f0a000
        VLDR.W   S2,??setMazdaMiataNbEngineConfiguration_0+0x20  ;; 0x42700000
        VLDR.W   S1,??setMazdaMiataNbEngineConfiguration_0+0x24  ;; 0x481c8d80
        VMOV.F32 S0,#-10.0
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x28
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+288
          CFI FunCall _Z26setThermistorConfigurationP14ThermistorConfffffff
        BL       _Z26setThermistorConfigurationP14ThermistorConfffffff
//   36 	engineConfiguration->iat.config.bias_resistor = 2700;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x28
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setMazdaMiataNbEngineConfiguration_0+0x30  ;; 0x4528c000
        STR      R1,[R0, #+312]
//   37 
//   38 	engineConfiguration->tpsAdcChannel = EFI_ADC_3; // 15 is the old value
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x28
        LDR      R0,[R0, #+0]
        MOVS     R1,#+3
        STR      R1,[R0, #+524]
//   39 	engineConfiguration->vbattAdcChannel = EFI_ADC_0; // 1 is the old value
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x28
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+472]
//   40 //  engineConfiguration->map.channel = 1;
//   41 	engineConfiguration->mafAdcChannel = EFI_ADC_1;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x28
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+580]
//   42 	engineConfiguration->clt.adcChannel = EFI_ADC_11;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x28
        LDR      R0,[R0, #+0]
        MOVS     R1,#+11
        STR      R1,[R0, #+284]
//   43 	engineConfiguration->iat.adcChannel = EFI_ADC_13;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x28
        LDR      R0,[R0, #+0]
        MOVS     R1,#+13
        STR      R1,[R0, #+316]
//   44 	engineConfiguration->afr.hwChannel = EFI_ADC_2;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x28
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+584]
//   45 
//   46 	boardConfiguration->idle.solenoidPin = GPIOE_0;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x34
        LDR      R0,[R0, #+0]
        MOVS     R1,#+64
        STR      R1,[R0, #+0]
//   47 	boardConfiguration->idle.solenoidPinMode = OM_DEFAULT;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x34
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+4]
//   48 
//   49 	boardConfiguration->fuelPumpPin = GPIOC_14; // Frankenstein: low side - out #4
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x34
        LDR      R0,[R0, #+0]
        MOVS     R1,#+46
        STR      R1,[R0, #+24]
//   50 	boardConfiguration->fuelPumpPinMode = OM_DEFAULT;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x34
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+28]
//   51 
//   52 	boardConfiguration->injectionPins[0] = GPIOB_9; // Frankenstein: low side - out #12
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x34
        LDR      R0,[R0, #+0]
        MOVS     R1,#+25
        STR      R1,[R0, #+32]
//   53 	boardConfiguration->injectionPins[1] = GPIOB_8; // Frankenstein: low side - out #11
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x34
        LDR      R0,[R0, #+0]
        MOVS     R1,#+24
        STR      R1,[R0, #+36]
//   54 	boardConfiguration->injectionPins[2] = GPIOE_3; // Frankenstein: low side - out #8
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x34
        LDR      R0,[R0, #+0]
        MOVS     R1,#+67
        STR      R1,[R0, #+40]
//   55 	boardConfiguration->injectionPins[3] = GPIOE_5; // Frankenstein: low side - out #6
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x34
        LDR      R0,[R0, #+0]
        MOVS     R1,#+69
        STR      R1,[R0, #+44]
//   56 	boardConfiguration->injectionPins[4] = GPIO_UNASSIGNED;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x34
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+48]
//   57 	boardConfiguration->injectionPins[5] = GPIO_UNASSIGNED;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x34
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+52]
//   58 	boardConfiguration->injectionPinMode = OM_DEFAULT;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x34
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+80]
//   59 
//   60 	boardConfiguration->ignitionPins[0] = GPIOE_10; // Frankenstein: high side #1
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x34
        LDR      R0,[R0, #+0]
        MOVS     R1,#+74
        STR      R1,[R0, #+84]
//   61 	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x34
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+88]
//   62 	boardConfiguration->ignitionPins[2] = GPIOC_9; // // Frankenstein: high side #2
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x34
        LDR      R0,[R0, #+0]
        MOVS     R1,#+41
        STR      R1,[R0, #+92]
//   63 	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x34
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+96]
//   64 	boardConfiguration->ignitionPinMode = OM_INVERTED;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x34
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+132]
//   65 
//   66 	boardConfiguration->malfunctionIndicatorPin = GPIOE_1;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x34
        LDR      R0,[R0, #+0]
        MOVS     R1,#+65
        STR      R1,[R0, #+136]
//   67 	boardConfiguration->malfunctionIndicatorPinMode = OM_DEFAULT;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x34
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+140]
//   68 
//   69 	boardConfiguration->fanPin = GPIOE_6;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x34
        LDR      R0,[R0, #+0]
        MOVS     R1,#+70
        STR      R1,[R0, #+144]
//   70 	boardConfiguration->fanPinMode = OM_DEFAULT;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x34
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+148]
//   71 
//   72 	boardConfiguration->clutchDownPin = GPIO_UNASSIGNED;
        LDR.N    R0,??setMazdaMiataNbEngineConfiguration_0+0x34
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+160]
//   73 
//   74 	// set_whole_fuel_map 3
//   75 	setWholeFuelMap(3 PASS_ENGINE_PARAMETER);
        VMOV.F32 S0,#3.0
          CFI FunCall _Z15setWholeFuelMapf
        BL       _Z15setWholeFuelMapf
//   76 
//   77 
//   78 // 10 deg before TDC is default timing
//   79 }
        POP      {R0,PC}          ;; return
        DATA
??setMazdaMiataNbEngineConfiguration_0:
        DC32     0x442f0000
        DC32     0x42c80000
        DC32     0x45ebf000
        DC32     0x46fde800
        DC32     0x0
        DC32     0x44938000
        DC32     0x42f00000
        DC32     0x45f0a000
        DC32     0x42700000
        DC32     0x481c8d80
        DC32     engineConfiguration
        DC32     0x438a0000
        DC32     0x4528c000
        DC32     boardConfiguration
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
// 472 bytes in section .text
// 
// 472 bytes of CODE memory
//
//Errors: none
//Warnings: 1
