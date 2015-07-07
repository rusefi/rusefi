///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:51 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       honda_accord.cpp                                     /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       honda_accord.cpp -lCN F:\stuff\rusefi_sourceforge\fi /
//                       rmware\iar\Debug\List\ -lA                           /
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
//                       honda_accord.s                                       /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME honda_accord

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z12setAlgorithm18engine_load_mode_e
        EXTERN _Z14setFuelLoadBinff
        EXTERN _Z18setCommonNTCSensorP14ThermistorConf
        EXTERN _Z24setFrankenso0_1_joystickP22engine_configuration_s
        EXTERN boardConfiguration
        EXTERN engineConfiguration

        PUBLIC _Z19setFrankenso_01_LCDP21board_configuration_s
        PUBLIC _Z30setHondaAccordConfigurationDipv
        PUBLIC _Z31setHondaAccordConfiguration1_24v
        PUBLIC _Z35setHondaAccordConfigurationTwoWiresv
        PUBLIC _Z37setHondaAccordConfigurationThreeWiresv
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\config\engines\honda_accord.cpp
//    1 /**
//    2  * @file	honda_accord.cpp
//    3  *
//    4  * 1993 Honda Prelude 1993
//    5  * http://rusefi.com/wiki/index.php?title=Vehicle:Honda_Prelude_1993
//    6  * http://rusefi.com/forum/viewtopic.php?f=3&t=887
//    7  *
//    8  * 1995 Honda Accord EX
//    9  * http://rusefi.com/wiki/index.php?title=Vehicle:Honda_Accord_1995
//   10  * http://rusefi.com/forum/viewtopic.php?f=3&t=621
//   11  *
//   12  * set_engine_type 6
//   13  * set_engine_type 17
//   14  *
//   15  * @date Jan 12, 2014
//   16  * @author Andrey Belomutskiy, (c) 2012-2015
//   17  */
//   18 
//   19 #include "main.h"
//   20 #include "trigger_decoder.h"
//   21 #include "thermistors.h"
//   22 #include "honda_accord.h"
//   23 #include "engine_math.h"
//   24 #include "settings.h"
//   25 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z19setFrankenso_01_LCDP21board_configuration_s
          CFI NoCalls
        THUMB
//   26 void setFrankenso_01_LCD(board_configuration_s *boardConfiguration) {
//   27 	boardConfiguration->HD44780_rs = GPIOE_7;
_Z19setFrankenso_01_LCDP21board_configuration_s:
        MOVS     R1,#+71
        STR      R1,[R0, #+176]
//   28 	boardConfiguration->HD44780_e = GPIOE_9;
        MOVS     R1,#+73
        STR      R1,[R0, #+180]
//   29 	boardConfiguration->HD44780_db4 = GPIOE_11;
        MOVS     R1,#+75
        STR      R1,[R0, #+184]
//   30 	boardConfiguration->HD44780_db5 = GPIOE_13;
        MOVS     R1,#+77
        STR      R1,[R0, #+188]
//   31 	boardConfiguration->HD44780_db6 = GPIOE_15;
        MOVS     R1,#+79
        STR      R1,[R0, #+192]
//   32 	boardConfiguration->HD44780_db7 = GPIOB_10;
        MOVS     R1,#+26
        STR      R1,[R0, #+196]
//   33 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//   34 
//   35 EXTERN_ENGINE;
//   36 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z33setHondaAccordConfigurationCommonv
        THUMB
//   37 static void setHondaAccordConfigurationCommon(DECLARE_ENGINE_PARAMETER_F) {
_Z33setHondaAccordConfigurationCommonv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   38 	engineConfiguration->map.sensor.type = MT_DENSO183;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+248]
//   39 	boardConfiguration->isFastAdcEnabled = true;
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        ORRS     R0,R0,#0x10
        LDR.N    R1,??DataTable4_3
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+384]
//   40 
//   41 	engineConfiguration->ignitionMode = IM_ONE_COIL;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+444]
//   42 	engineConfiguration->injectionMode = IM_BATCH;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+432]
//   43 
//   44 	engineConfiguration->idleMode = IM_MANUAL;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+1504]
//   45 
//   46 	engineConfiguration->HD44780height = 4;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+4
        STR      R1,[R0, #+520]
//   47 
//   48 	engineConfiguration->specs.cylindersCount = 4;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+4
        STR      R1,[R0, #+404]
//   49 	engineConfiguration->specs.displacement = 2.156;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable4_4  ;; 0x4009fbe7
        STR      R1,[R0, #+400]
//   50 
//   51 	// Keihin 06164-P0A-A00
//   52 	engineConfiguration->injector.flow = 248;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable4_5  ;; 0x43780000
        STR      R1,[R0, #+8]
//   53 
//   54 //	engineConfiguration->algorithm = LM_SPEED_DENSITY;
//   55 	// I want to start with a simple Alpha-N
//   56 	setAlgorithm(LM_ALPHA_N PASS_ENGINE_PARAMETER);
        MOVS     R0,#+1
          CFI FunCall _Z12setAlgorithm18engine_load_mode_e
        BL       _Z12setAlgorithm18engine_load_mode_e
//   57 	setFuelLoadBin(0, 100 PASS_ENGINE_PARAMETER);
        VLDR.W   S1,??DataTable4  ;; 0x42c80000
        VLDR.W   S0,??DataTable4_1  ;; 0x0
          CFI FunCall _Z14setFuelLoadBinff
        BL       _Z14setFuelLoadBinff
//   58 
//   59 	/**
//   60 	 * 18K Ohm @ -20C
//   61 	 * 2.1K Ohm @ 24C
//   62 	 * 100 Ohm @ 120C
//   63 	 */
//   64 	setCommonNTCSensor(&engineConfiguration->clt);
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+256
          CFI FunCall _Z18setCommonNTCSensorP14ThermistorConf
        BL       _Z18setCommonNTCSensorP14ThermistorConf
//   65 	engineConfiguration->clt.config.bias_resistor = 1500; // same as OEM ECU
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable4_6  ;; 0x44bb8000
        STR      R1,[R0, #+280]
//   66 
//   67 	setCommonNTCSensor(&engineConfiguration->iat);
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+288
          CFI FunCall _Z18setCommonNTCSensorP14ThermistorConf
        BL       _Z18setCommonNTCSensorP14ThermistorConf
//   68 	engineConfiguration->iat.config.bias_resistor = 1500; // same as OEM ECU
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable4_6  ;; 0x44bb8000
        STR      R1,[R0, #+312]
//   69 
//   70 	// set_cranking_charge_angle 35
//   71 	engineConfiguration->crankingChargeAngle = 70;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable4_7  ;; 0x428c0000
        STR      R1,[R0, #+108]
//   72 	// set_cranking_timing_angle 0
//   73 	engineConfiguration->crankingTimingAngle = -45;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable4_8  ;; 0xc2340000
        STR      R1,[R0, #+440]
//   74 
//   75 	// set_global_trigger_offset_angle 34
//   76 	engineConfiguration->globalTriggerAngleOffset = 34;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable4_9  ;; 0x42080000
        STR      R1,[R0, #+460]
//   77 
//   78 	// set_rpm_hard_limit 4000
//   79 	engineConfiguration->rpmHardLimit = 4000; // yes, 4k. let's play it safe for now
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        MOV      R1,#+4000
        STR      R1,[R0, #+420]
//   80 	// set_cranking_rpm 2000
//   81 	engineConfiguration->cranking.rpm = 500;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        MOV      R1,#+500
        STRH     R1,[R0, #+96]
//   82 
//   83 
//   84 	// set_ignition_offset 350
//   85 //	engineConfiguration->ignitionOffset = 350;
//   86 	 // set_injection_offset 510
//   87 //	engineConfiguration->injectionOffset = 510;
//   88 
//   89 
//   90 	/**
//   91 	 * ADC inputs:
//   92 	 *
//   93 	 * Inp1/ADC12 PC2: CLT
//   94 	 * Inp2/ADC11 PC1: AIT/IAT
//   95 	 * Inp3/ADC0 PA0: MAP
//   96 	 * Inp4/ADC13 PC3: AFR green wire
//   97 	 * Inp6/ADC1 PA1: TPS
//   98 	 * Inp12/ADC14 PC4: VBatt
//   99 	 */
//  100 
//  101 	/**
//  102 	 * wideband O2 Sensor
//  103 	 */
//  104 	engineConfiguration->afr.hwChannel = EFI_ADC_13;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+13
        STR      R1,[R0, #+584]
//  105 
//  106 	/**
//  107 	 * VBatt
//  108 	 */
//  109 	engineConfiguration->vbattAdcChannel = EFI_ADC_14;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+14
        STR      R1,[R0, #+472]
//  110 	engineConfiguration->vbattDividerCoeff = ((float) (10 + 39)) / 10 * 2;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable4_10  ;; 0x411ccccd
        STR      R1,[R0, #+468]
//  111 
//  112 	//	todo engineConfiguration->afr.hwChannel = 14;
//  113 
//  114 
//  115 	/**
//  116 	 * MAP D17/W5 blue wire
//  117 	 */
//  118 	engineConfiguration->map.sensor.hwChannel = EFI_ADC_0;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+252]
//  119 	/**
//  120 	 * IAT D15/W7 green wire
//  121 	 */
//  122 	engineConfiguration->iat.adcChannel = EFI_ADC_11;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+11
        STR      R1,[R0, #+316]
//  123 	/**
//  124 	 * CLT D13/W9 yellow wire
//  125 	 */
//  126 	engineConfiguration->clt.adcChannel = EFI_ADC_12;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+12
        STR      R1,[R0, #+284]
//  127 	/**
//  128 	 * TPS D11/W11 blue wire
//  129 	 */
//  130 	engineConfiguration->tpsAdcChannel = EFI_ADC_1;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+524]
//  131 
//  132 
//  133 	/**
//  134 	 * Outputs
//  135 	 */
//  136 	// Frankenso low out #1: PE6
//  137 	// Frankenso low out #2: PE5
//  138 	// Frankenso low out #3: PD7
//  139 	// Frankenso low out #4: PC13
//  140 	// Frankenso low out #5: PE3 Fuel Relay
//  141 	// Frankenso low out #6: PE4 radiator fan - blue wire
//  142 	// Frankenso low out #7: PD3 idle air valve solenoid - green wire
//  143 	// Frankenso low out #8: PE2 MIL - white wire
//  144 	// Frankenso low out #9: PB9 Injector #2
//  145 	// Frankenso low out #10: PD5 Injector #3
//  146 	// Frankenso low out #11: PB8 injector #1
//  147 	// Frankenso low out #12: PB7 injector #4
//  148 	boardConfiguration->fuelPumpPin = GPIOE_3;
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        MOVS     R1,#+67
        STR      R1,[R0, #+24]
//  149 	boardConfiguration->fuelPumpPinMode = OM_DEFAULT;
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+28]
//  150 	boardConfiguration->malfunctionIndicatorPin = GPIOE_2;
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        MOVS     R1,#+66
        STR      R1,[R0, #+136]
//  151 	boardConfiguration->malfunctionIndicatorPinMode = OM_DEFAULT;
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+140]
//  152 	boardConfiguration->fanPin = GPIOE_4; // blue wire
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        MOVS     R1,#+68
        STR      R1,[R0, #+144]
//  153 
//  154 	boardConfiguration->idle.solenoidPin = GPIOD_3; // green wire
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        MOVS     R1,#+51
        STR      R1,[R0, #+0]
//  155 
//  156 	boardConfiguration->injectionPins[0] = GPIOB_8;
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        MOVS     R1,#+24
        STR      R1,[R0, #+32]
//  157 	boardConfiguration->injectionPins[1] = GPIOB_9;
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        MOVS     R1,#+25
        STR      R1,[R0, #+36]
//  158 	boardConfiguration->injectionPins[2] = GPIOD_5;
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        MOVS     R1,#+53
        STR      R1,[R0, #+40]
//  159 	boardConfiguration->injectionPins[3] = GPIOB_7;
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        MOVS     R1,#+23
        STR      R1,[R0, #+44]
//  160 
//  161 	boardConfiguration->ignitionPins[0] = GPIOE_12; // white wire
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        MOVS     R1,#+76
        STR      R1,[R0, #+84]
//  162 	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+88]
//  163 	boardConfiguration->ignitionPins[2] = GPIO_UNASSIGNED;
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+92]
//  164 	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+96]
//  165 
//  166 
//  167 	setFrankenso_01_LCD(boardConfiguration);
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
          CFI FunCall _Z19setFrankenso_01_LCDP21board_configuration_s
        BL       _Z19setFrankenso_01_LCDP21board_configuration_s
//  168 	setFrankenso0_1_joystick(engineConfiguration);
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
          CFI FunCall _Z24setFrankenso0_1_joystickP22engine_configuration_s
        BL       _Z24setFrankenso0_1_joystickP22engine_configuration_s
//  169 
//  170 	boardConfiguration->idle.solenoidFrequency = 500;
        LDR.N    R0,??DataTable4_3
        LDR      R0,[R0, #+0]
        MOV      R1,#+500
        STR      R1,[R0, #+8]
//  171 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock1
//  172 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z35setHondaAccordConfigurationTwoWiresv
        THUMB
//  173 void setHondaAccordConfigurationTwoWires(DECLARE_ENGINE_PARAMETER_F) {
_Z35setHondaAccordConfigurationTwoWiresv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  174 	engineConfiguration->engineType = HONDA_ACCORD_CD_TWO_WIRES;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+17
        STR      R1,[R0, #+0]
//  175 	engineConfiguration->trigger.type = TT_HONDA_ACCORD_CD_TWO_WIRES;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+12
        STR      R1,[R0, #+536]
//  176 	setHondaAccordConfigurationCommon(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z33setHondaAccordConfigurationCommonv
        BL       _Z33setHondaAccordConfigurationCommonv
//  177 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock2
//  178 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z37setHondaAccordConfigurationThreeWiresv
        THUMB
//  179 void setHondaAccordConfigurationThreeWires(DECLARE_ENGINE_PARAMETER_F) {
_Z37setHondaAccordConfigurationThreeWiresv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  180 	engineConfiguration->engineType = HONDA_ACCORD_CD;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+6
        STR      R1,[R0, #+0]
//  181 	engineConfiguration->trigger.type = TT_HONDA_ACCORD_CD;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+10
        STR      R1,[R0, #+536]
//  182 	setHondaAccordConfigurationCommon(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z33setHondaAccordConfigurationCommonv
        BL       _Z33setHondaAccordConfigurationCommonv
//  183 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock3
//  184 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z30setHondaAccordConfigurationDipv
        THUMB
//  185 void setHondaAccordConfigurationDip(DECLARE_ENGINE_PARAMETER_F) {
_Z30setHondaAccordConfigurationDipv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  186 	engineConfiguration->engineType = HONDA_ACCORD_CD_DIP;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+18
        STR      R1,[R0, #+0]
//  187 	engineConfiguration->trigger.type = TT_HONDA_ACCORD_CD_DIP;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+13
        STR      R1,[R0, #+536]
//  188 	setHondaAccordConfigurationCommon(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z33setHondaAccordConfigurationCommonv
        BL       _Z33setHondaAccordConfigurationCommonv
//  189 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock4
//  190 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z31setHondaAccordConfiguration1_24v
        THUMB
//  191 void setHondaAccordConfiguration1_24(DECLARE_ENGINE_PARAMETER_F) {
_Z31setHondaAccordConfiguration1_24v:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  192 	engineConfiguration->engineType = HONDA_ACCORD_CD_DIP;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+18
        STR      R1,[R0, #+0]
//  193 	engineConfiguration->trigger.type = TT_HONDA_ACCORD_1_24;
        LDR.N    R0,??DataTable4_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+21
        STR      R1,[R0, #+536]
//  194 	setHondaAccordConfigurationCommon(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z33setHondaAccordConfigurationCommonv
        BL       _Z33setHondaAccordConfigurationCommonv
//  195 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock5

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4:
        DC32     0x42c80000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_1:
        DC32     0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_2:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_3:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_4:
        DC32     0x4009fbe7

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_5:
        DC32     0x43780000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_6:
        DC32     0x44bb8000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_7:
        DC32     0x428c0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_8:
        DC32     0xc2340000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_9:
        DC32     0x42080000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_10:
        DC32     0x411ccccd

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
// 616 bytes in section .text
// 
// 616 bytes of CODE memory
//
//Errors: none
//Warnings: 1
