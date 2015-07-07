///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:34 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       ford_1995_inline_6.cpp                               /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       ford_1995_inline_6.cpp -lCN                          /
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
//                       ford_1995_inline_6.s                                 /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME ford_1995_inline_6

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z16setConstantDwellf
        EXTERN _Z16setOperationModeP22engine_configuration_s16operation_mode_e
        EXTERN _Z24convertVoltageTo10bitADCf
        EXTERN _Z26setThermistorConfigurationP14ThermistorConfffffff
        EXTERN boardConfiguration
        EXTERN engineConfiguration

        PUBLIC _Z14setFordInline6v
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\config\engines\ford_1995_inline_6.cpp
//    1 /**
//    2  * @file ford_1995_inline_6.cpp
//    3  * @brief Default engine configuration for a 1995 Ford inline 6 engine
//    4  *
//    5  * http://rusefi.com/forum/viewtopic.php?f=3&t=469
//    6  *
//    7  * This config overrides some values of the default configuration which is set by setDefaultConfiguration() method
//    8  *
//    9  * FORD_INLINE_6_1995 = 7
//   10  * set_engine_type 7
//   11  *
//   12  * @date Feb 12, 2014
//   13  * @author Andrey Belomutskiy, (c) 2012-2015
//   14  */
//   15 
//   16 #include "main.h"
//   17 #include "ford_1995_inline_6.h"
//   18 #include "engine_math.h"
//   19 #include "allsensors.h"
//   20 
//   21 #if EFI_SUPPORT_1995_FORD_INLINE_6 || defined(__DOXYGEN__)
//   22 
//   23 EXTERN_ENGINE;
//   24 
//   25 /**
//   26  * @brief Default values for persistent properties
//   27  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z14setFordInline6v
        THUMB
//   28 void setFordInline6(DECLARE_ENGINE_PARAMETER_F) {
_Z14setFordInline6v:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   29 	engineConfiguration->specs.cylindersCount = 6;
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+6
        STR      R1,[R0, #+404]
//   30 
//   31 	setOperationMode(engineConfiguration, FOUR_STROKE_CAM_SENSOR);
        MOVS     R1,#+2
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
          CFI FunCall _Z16setOperationModeP22engine_configuration_s16operation_mode_e
        BL       _Z16setOperationModeP22engine_configuration_s16operation_mode_e
//   32 
//   33 	engineConfiguration->ignitionMode = IM_ONE_COIL;
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+444]
//   34 	engineConfiguration->specs.firingOrder = FO_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4;
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+4
        STR      R1,[R0, #+408]
//   35 	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+428]
//   36 	engineConfiguration->injectionMode = IM_BATCH;
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+432]
//   37 	engineConfiguration->twoWireBatch = true;
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        ORRS     R0,R0,#0x20
        LDR.N    R1,??setFordInline6_0+0x18
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1509]
//   38 
//   39 
//   40 	/**
//   41 	 * 0.5ms dweel time just to be sure it would fit within camshaft revolution, dwell is not controlled by us anyway
//   42 	 */
//   43 	setConstantDwell(0.5 PASS_ENGINE_PARAMETER);
        VMOV.F32 S0,#0.5
          CFI FunCall _Z16setConstantDwellf
        BL       _Z16setConstantDwellf
//   44 
//   45 	/**
//   46 	 * We treat the trigger as 6/0 toothed wheel
//   47 	 */
//   48 	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL;
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+536]
//   49 	engineConfiguration->trigger.customTotalToothCount = 6;
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+6
        STR      R1,[R0, #+544]
//   50 	engineConfiguration->trigger.customSkippedToothCount = 0;
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+548]
//   51 
//   52 	engineConfiguration->globalTriggerAngleOffset = 0;
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+460]
//   53 	engineConfiguration->ignitionOffset = 13;
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setFordInline6_0+0x1C  ;; 0x41500000
        STR      R1,[R0, #+448]
//   54 	engineConfiguration->injectionOffset = 207.269999;
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setFordInline6_0+0x20  ;; 0x434f451f
        STR      R1,[R0, #+436]
//   55 
//   56 	setThermistorConfiguration(&engineConfiguration->clt, -10.0, 160310.0, 60.0, 7700.0, 120.0, 1180.0);
        VLDR.W   S5,??setFordInline6_0  ;; 0x44938000
        VLDR.W   S4,??setFordInline6_0+0x4  ;; 0x42f00000
        VLDR.W   S3,??setFordInline6_0+0x8  ;; 0x45f0a000
        VLDR.W   S2,??setFordInline6_0+0xC  ;; 0x42700000
        VLDR.W   S1,??setFordInline6_0+0x10  ;; 0x481c8d80
        VMOV.F32 S0,#-10.0
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+256
          CFI FunCall _Z26setThermistorConfigurationP14ThermistorConfffffff
        BL       _Z26setThermistorConfigurationP14ThermistorConfffffff
//   57 	engineConfiguration->clt.config.bias_resistor = 2700;
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setFordInline6_0+0x24  ;; 0x4528c000
        STR      R1,[R0, #+280]
//   58 
//   59 	setThermistorConfiguration(&engineConfiguration->iat, -10.0, 160310.0, 60.0, 7700.0, 120.0, 1180.0);
        VLDR.W   S5,??setFordInline6_0  ;; 0x44938000
        VLDR.W   S4,??setFordInline6_0+0x4  ;; 0x42f00000
        VLDR.W   S3,??setFordInline6_0+0x8  ;; 0x45f0a000
        VLDR.W   S2,??setFordInline6_0+0xC  ;; 0x42700000
        VLDR.W   S1,??setFordInline6_0+0x10  ;; 0x481c8d80
        VMOV.F32 S0,#-10.0
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+288
          CFI FunCall _Z26setThermistorConfigurationP14ThermistorConfffffff
        BL       _Z26setThermistorConfigurationP14ThermistorConfffffff
//   60 	engineConfiguration->iat.config.bias_resistor = 2700;
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setFordInline6_0+0x24  ;; 0x4528c000
        STR      R1,[R0, #+312]
//   61 
//   62 	// 12ch analog board pinout:
//   63 	// input channel 3 is PA7, that's ADC7
//   64 	// input channel 5 is PA4, that's ADC4
//   65 	// input channel 6 is PA3, that's ADC3
//   66 	// input channel 7 is PA2, that's ADC2
//   67 	// input channel 8 is PA1, that's ADC1
//   68 	// input channel 9 is PA0, that's ADC0
//   69 	// input channel 10 is PC3, that's ADC13
//   70 	// input channel 12 is PC1, that's ADC11
//   71 
//   72 	engineConfiguration->tpsAdcChannel = EFI_ADC_4;
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+4
        STR      R1,[R0, #+524]
//   73 	engineConfiguration->iat.adcChannel = EFI_ADC_2;
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+316]
//   74 	engineConfiguration->clt.adcChannel = EFI_ADC_1;
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+284]
//   75 	engineConfiguration->afr.hwChannel = EFI_ADC_11;
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+11
        STR      R1,[R0, #+584]
//   76 
//   77 	engineConfiguration->map.sensor.type = MT_MPX4250;
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+248]
//   78 	engineConfiguration->map.sensor.hwChannel = EFI_ADC_15;
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+15
        STR      R1,[R0, #+252]
//   79 	engineConfiguration->baroSensor.type = MT_MPX4250;
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+616]
//   80 	engineConfiguration->baroSensor.hwChannel = EFI_ADC_7;
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+7
        STR      R1,[R0, #+620]
//   81 
//   82 	// 6 channel output board
//   83 	// output 1 is PB9
//   84 	// output 3 is PE3
//   85 	// output 5 is PC13
//   86 	// output 6 is PC15
//   87 
//   88 	boardConfiguration->fuelPumpPin = GPIOC_13;
        LDR.N    R0,??setFordInline6_0+0x28
        LDR      R0,[R0, #+0]
        MOVS     R1,#+45
        STR      R1,[R0, #+24]
//   89 	boardConfiguration->injectionPins[0] = GPIOB_9;
        LDR.N    R0,??setFordInline6_0+0x28
        LDR      R0,[R0, #+0]
        MOVS     R1,#+25
        STR      R1,[R0, #+32]
//   90 	boardConfiguration->injectionPins[1] = GPIOE_3;
        LDR.N    R0,??setFordInline6_0+0x28
        LDR      R0,[R0, #+0]
        MOVS     R1,#+67
        STR      R1,[R0, #+36]
//   91 	boardConfiguration->ignitionPins[0] = GPIOC_15;
        LDR.N    R0,??setFordInline6_0+0x28
        LDR      R0,[R0, #+0]
        MOVS     R1,#+47
        STR      R1,[R0, #+84]
//   92 
//   93 	boardConfiguration->injectionPins[2] = GPIO_UNASSIGNED;
        LDR.N    R0,??setFordInline6_0+0x28
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+40]
//   94 	boardConfiguration->fanPin = GPIO_UNASSIGNED;
        LDR.N    R0,??setFordInline6_0+0x28
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+144]
//   95 
//   96 	engineConfiguration->tpsMin = convertVoltageTo10bitADC(1.250);
        VMOV.F32 S0,#1.25
          CFI FunCall _Z24convertVoltageTo10bitADCf
        BL       _Z24convertVoltageTo10bitADCf
        LDR.N    R1,??setFordInline6_0+0x18
        LDR      R1,[R1, #+0]
        STRH     R0,[R1, #+82]
//   97 	engineConfiguration->tpsMax = convertVoltageTo10bitADC(4.538);
        VLDR.W   S0,??setFordInline6_0+0x14  ;; 0x4091374c
          CFI FunCall _Z24convertVoltageTo10bitADCf
        BL       _Z24convertVoltageTo10bitADCf
        LDR.N    R1,??setFordInline6_0+0x18
        LDR      R1,[R1, #+0]
        STRH     R0,[R1, #+84]
//   98 
//   99 	//	engineConfiguration->vbattAdcChannel = 0; //
//  100 	engineConfiguration->mafAdcChannel = EFI_ADC_1;
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+580]
//  101 	engineConfiguration->hasMafSensor = true;
        LDR.N    R0,??setFordInline6_0+0x18
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        ORRS     R0,R0,#0x40
        LDR.N    R1,??setFordInline6_0+0x18
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1497]
//  102 
//  103 	boardConfiguration->triggerInputPins[0] = GPIOA_8;
        LDR.N    R0,??setFordInline6_0+0x28
        LDR      R0,[R0, #+0]
        MOVS     R1,#+8
        STR      R1,[R0, #+296]
//  104 	boardConfiguration->triggerInputPins[1] = GPIOA_5;
        LDR.N    R0,??setFordInline6_0+0x28
        LDR      R0,[R0, #+0]
        MOVS     R1,#+5
        STR      R1,[R0, #+300]
//  105 	boardConfiguration->logicAnalyzerPins[0] = GPIOC_6;
        LDR.N    R0,??setFordInline6_0+0x28
        LDR      R0,[R0, #+0]
        MOVS     R1,#+38
        STR      R1,[R0, #+388]
//  106 	boardConfiguration->logicAnalyzerPins[1] = GPIOE_5;
        LDR.N    R0,??setFordInline6_0+0x28
        LDR      R0,[R0, #+0]
        MOVS     R1,#+69
        STR      R1,[R0, #+392]
//  107 }
        POP      {R0,PC}          ;; return
        DATA
??setFordInline6_0:
        DC32     0x44938000
        DC32     0x42f00000
        DC32     0x45f0a000
        DC32     0x42700000
        DC32     0x481c8d80
        DC32     0x4091374c
        DC32     engineConfiguration
        DC32     0x41500000
        DC32     0x434f451f
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
//  108 
//  109 #endif /* EFI_SUPPORT_1995_FORD_INLINE_6 */
// 
// 520 bytes in section .text
// 
// 520 bytes of CODE memory
//
//Errors: none
//Warnings: 1
