///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:02 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       custom_engine.cpp                                    /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       custom_engine.cpp -lCN F:\stuff\rusefi_sourceforge\f /
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
//                       custom_engine.s                                      /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME custom_engine

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z18setCommonNTCSensorP14ThermistorConf
        EXTERN _Z19setFrankenso_01_LCDP21board_configuration_s
        EXTERN _Z24setFrankenso0_1_joystickP22engine_configuration_s
        EXTERN boardConfiguration
        EXTERN engineConfiguration

        PUBLIC _Z28setCustomEngineConfigurationv
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\config\engines\custom_engine.cpp
//    1 /**
//    2  * @file	custom_engine.cpp
//    3  *
//    4  * @date Jan 18, 2015
//    5  * @author Andrey Belomutskiy, (c) 2012-2015
//    6  */
//    7 #ifndef CONFIG_ENGINES_CUSTOM_ENGINE_CPP_
//    8 #define CONFIG_ENGINES_CUSTOM_ENGINE_CPP_
//    9 
//   10 #include "custom_engine.h"
//   11 #include "honda_accord.h"
//   12 #include "allsensors.h"
//   13 
//   14 EXTERN_ENGINE;
//   15 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z28setCustomEngineConfigurationv
        THUMB
//   16 void setCustomEngineConfiguration(DECLARE_ENGINE_PARAMETER_F) {
_Z28setCustomEngineConfigurationv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   17 	engineConfiguration->trigger.type = TT_ONE_PLUS_ONE;
        LDR.N    R0,??setCustomEngineConfiguration_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+16
        STR      R1,[R0, #+536]
//   18 
//   19 	setFrankenso_01_LCD(boardConfiguration);
        LDR.N    R0,??setCustomEngineConfiguration_0+0x4
        LDR      R0,[R0, #+0]
          CFI FunCall _Z19setFrankenso_01_LCDP21board_configuration_s
        BL       _Z19setFrankenso_01_LCDP21board_configuration_s
//   20 	setFrankenso0_1_joystick(engineConfiguration);
        LDR.N    R0,??setCustomEngineConfiguration_0
        LDR      R0,[R0, #+0]
          CFI FunCall _Z24setFrankenso0_1_joystickP22engine_configuration_s
        BL       _Z24setFrankenso0_1_joystickP22engine_configuration_s
//   21 
//   22 	/**
//   23 	 * Frankenso analog #1 PC2 ADC12 CLT
//   24 	 * Frankenso analog #2 PC1 ADC11 IAT
//   25 	 * Frankenso analog #3 PA0 ADC0 MAP
//   26 	 * Frankenso analog #4 PC3 ADC13 WBO / O2
//   27 	 * Frankenso analog #5 PA2 ADC2 TPS
//   28 	 * Frankenso analog #6 PA1 ADC1
//   29 	 * Frankenso analog #7 PA4 ADC4
//   30 	 * Frankenso analog #8 PA3 ADC3
//   31 	 * Frankenso analog #9 PA7 ADC7
//   32 	 * Frankenso analog #10 PA6 ADC6
//   33 	 * Frankenso analog #11 PC5 ADC15
//   34 	 * Frankenso analog #12 PC4 ADC14 VBatt
//   35 	 */
//   36 	engineConfiguration->tpsAdcChannel = EFI_ADC_2;
        LDR.N    R0,??setCustomEngineConfiguration_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+524]
//   37 
//   38 	engineConfiguration->map.sensor.hwChannel = EFI_ADC_0;
        LDR.N    R0,??setCustomEngineConfiguration_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+252]
//   39 
//   40 	engineConfiguration->clt.adcChannel = EFI_ADC_12;
        LDR.N    R0,??setCustomEngineConfiguration_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+12
        STR      R1,[R0, #+284]
//   41 	engineConfiguration->iat.adcChannel = EFI_ADC_11;
        LDR.N    R0,??setCustomEngineConfiguration_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+11
        STR      R1,[R0, #+316]
//   42 	engineConfiguration->afr.hwChannel = EFI_ADC_13;
        LDR.N    R0,??setCustomEngineConfiguration_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+13
        STR      R1,[R0, #+584]
//   43 
//   44 	setCommonNTCSensor(&engineConfiguration->clt);
        LDR.N    R0,??setCustomEngineConfiguration_0
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+256
          CFI FunCall _Z18setCommonNTCSensorP14ThermistorConf
        BL       _Z18setCommonNTCSensorP14ThermistorConf
//   45 	engineConfiguration->clt.config.bias_resistor = 2700;
        LDR.N    R0,??setCustomEngineConfiguration_0
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setCustomEngineConfiguration_0+0x8  ;; 0x4528c000
        STR      R1,[R0, #+280]
//   46 	setCommonNTCSensor(&engineConfiguration->iat);
        LDR.N    R0,??setCustomEngineConfiguration_0
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+288
          CFI FunCall _Z18setCommonNTCSensorP14ThermistorConf
        BL       _Z18setCommonNTCSensorP14ThermistorConf
//   47 	engineConfiguration->iat.config.bias_resistor = 2700;
        LDR.N    R0,??setCustomEngineConfiguration_0
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setCustomEngineConfiguration_0+0x8  ;; 0x4528c000
        STR      R1,[R0, #+312]
//   48 
//   49 
//   50 	/**
//   51 	 * http://rusefi.com/wiki/index.php?title=Manual:Hardware_Frankenso_board
//   52 	 */
//   53 	// Frankenso low out #1: PE6
//   54 	// Frankenso low out #2: PE5
//   55 	// Frankenso low out #3: PD7 Main Relay
//   56 	// Frankenso low out #4: PC13 Idle valve solenoid
//   57 	// Frankenso low out #5: PE3
//   58 	// Frankenso low out #6: PE4 fuel pump relay
//   59 	// Frankenso low out #7: PE1 (do not use with discovery!)
//   60 	// Frankenso low out #8: PE2 injector #2
//   61 	// Frankenso low out #9: PB9 injector #1
//   62 	// Frankenso low out #10: PE0 (do not use with discovery!)
//   63 	// Frankenso low out #11: PB8 injector #3
//   64 	// Frankenso low out #12: PB7 injector #4
//   65 
//   66 	boardConfiguration->fuelPumpPin = GPIOE_4;
        LDR.N    R0,??setCustomEngineConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+68
        STR      R1,[R0, #+24]
//   67 	boardConfiguration->mainRelayPin = GPIOD_7;
        LDR.N    R0,??setCustomEngineConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+55
        STR      R1,[R0, #+308]
//   68 	boardConfiguration->idle.solenoidPin = GPIOC_13;
        LDR.N    R0,??setCustomEngineConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+45
        STR      R1,[R0, #+0]
//   69 
//   70 	boardConfiguration->fanPin = GPIOE_5;
        LDR.N    R0,??setCustomEngineConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+69
        STR      R1,[R0, #+144]
//   71 
//   72 	boardConfiguration->injectionPins[0] = GPIOB_9; // #1
        LDR.N    R0,??setCustomEngineConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+25
        STR      R1,[R0, #+32]
//   73 	boardConfiguration->injectionPins[1] = GPIOE_2; // #2
        LDR.N    R0,??setCustomEngineConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+66
        STR      R1,[R0, #+36]
//   74 	boardConfiguration->injectionPins[2] = GPIOB_8; // #3
        LDR.N    R0,??setCustomEngineConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+24
        STR      R1,[R0, #+40]
//   75 	boardConfiguration->injectionPins[3] = GPIOB_7; // #4
        LDR.N    R0,??setCustomEngineConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+23
        STR      R1,[R0, #+44]
//   76 
//   77 #if EFI_PWM_TESTER
//   78 	boardConfiguration->injectionPins[4] = GPIOC_8; // #5
//   79 	boardConfiguration->injectionPins[5] = GPIOD_10; // #6
//   80 	boardConfiguration->injectionPins[6] = GPIOD_9;
//   81 	boardConfiguration->injectionPins[7] = GPIOD_11;
//   82 	boardConfiguration->injectionPins[8] = GPIOD_0;
//   83 	boardConfiguration->injectionPins[9] = GPIOB_11;
//   84 	boardConfiguration->injectionPins[10] = GPIOC_7;
//   85 	boardConfiguration->injectionPins[11] = GPIOE_4;
//   86 
//   87 	/**
//   88 	 * We want to initialize all outputs for test
//   89 	 */
//   90 	engineConfiguration->specs.cylindersCount = 12;
//   91 
//   92 	engineConfiguration->displayMode = DM_NONE;
//   93 #else
//   94 	boardConfiguration->injectionPins[4] = GPIO_UNASSIGNED;
        LDR.N    R0,??setCustomEngineConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+48]
//   95 	boardConfiguration->injectionPins[5] = GPIO_UNASSIGNED;
        LDR.N    R0,??setCustomEngineConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+52]
//   96 	boardConfiguration->injectionPins[6] = GPIO_UNASSIGNED;
        LDR.N    R0,??setCustomEngineConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+56]
//   97 	boardConfiguration->injectionPins[7] = GPIO_UNASSIGNED;
        LDR.N    R0,??setCustomEngineConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+60]
//   98 	boardConfiguration->injectionPins[8] = GPIO_UNASSIGNED;
        LDR.N    R0,??setCustomEngineConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+64]
//   99 	boardConfiguration->injectionPins[9] = GPIO_UNASSIGNED;
        LDR.N    R0,??setCustomEngineConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+68]
//  100 	boardConfiguration->injectionPins[10] = GPIO_UNASSIGNED;
        LDR.N    R0,??setCustomEngineConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+72]
//  101 	boardConfiguration->injectionPins[11] = GPIO_UNASSIGNED;
        LDR.N    R0,??setCustomEngineConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+76]
//  102 
//  103 	boardConfiguration->ignitionPins[0] = GPIOE_14;
        LDR.N    R0,??setCustomEngineConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+78
        STR      R1,[R0, #+84]
//  104 	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED; // todo: update this value
        LDR.N    R0,??setCustomEngineConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+88]
//  105 	boardConfiguration->ignitionPins[2] = GPIOC_7;
        LDR.N    R0,??setCustomEngineConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+39
        STR      R1,[R0, #+92]
//  106 	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED; // todo: update this value
        LDR.N    R0,??setCustomEngineConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+96]
//  107 #endif
//  108 
//  109 	// todo: 8.2 or 10k?
//  110 	engineConfiguration->vbattDividerCoeff = ((float) (10 + 33)) / 10 * 2;
        LDR.N    R0,??setCustomEngineConfiguration_0
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setCustomEngineConfiguration_0+0xC  ;; 0x4109999a
        STR      R1,[R0, #+468]
//  111 
//  112 }
        POP      {R0,PC}          ;; return
        Nop      
        DATA
??setCustomEngineConfiguration_0:
        DC32     engineConfiguration
        DC32     boardConfiguration
        DC32     0x4528c000
        DC32     0x4109999a
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
//  113 
//  114 #endif /* CONFIG_ENGINES_CUSTOM_ENGINE_CPP_ */
// 
// 316 bytes in section .text
// 
// 316 bytes of CODE memory
//
//Errors: none
//Warnings: 1
