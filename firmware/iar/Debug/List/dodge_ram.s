///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:22 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       dodge_ram.cpp                                        /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       dodge_ram.cpp -lCN F:\stuff\rusefi_sourceforge\firmw /
//                       are\iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge\ /
//                       firmware\iar\Debug\List\ -o                          /
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
//                       dodge_ram.s                                          /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME dodge_ram

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z14setDodgeSensorP14ThermistorConf
        EXTERN _Z15setWholeFuelMapf
        EXTERN _Z16setOperationModeP22engine_configuration_s16operation_mode_e
        EXTERN _Z19setWholeTimingTablef
        EXTERN _Z28setCustomEngineConfigurationv
        EXTERN boardConfiguration
        EXTERN engineConfiguration

        PUBLIC _Z15setDodgeRam1996v
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\config\engines\dodge_ram.cpp
//    1 /*
//    2  * @file dodge_ram.cpp
//    3  *
//    4  * set_engine_type 31
//    5  *
//    6  * @date Apr 22, 2015
//    7  * @author Andrey Belomutskiy, (c) 2012-2015
//    8  */
//    9 
//   10 #include "main.h"
//   11 #include "dodge_ram.h"
//   12 #include "custom_engine.h"
//   13 #include "allsensors.h"
//   14 
//   15 EXTERN_ENGINE
//   16 ;
//   17 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z15setDodgeRam1996v
        THUMB
//   18 void setDodgeRam1996(DECLARE_ENGINE_PARAMETER_F) {
_Z15setDodgeRam1996v:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   19 	setCustomEngineConfiguration(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z28setCustomEngineConfigurationv
        BL       _Z28setCustomEngineConfigurationv
//   20 
//   21 	setOperationMode(engineConfiguration, FOUR_STROKE_CAM_SENSOR);
        MOVS     R1,#+2
        LDR.N    R0,??setDodgeRam1996_0
        LDR      R0,[R0, #+0]
          CFI FunCall _Z16setOperationModeP22engine_configuration_s16operation_mode_e
        BL       _Z16setOperationModeP22engine_configuration_s16operation_mode_e
//   22 	engineConfiguration->trigger.type = TT_DODGE_RAM;
        LDR.N    R0,??setDodgeRam1996_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+19
        STR      R1,[R0, #+536]
//   23 	engineConfiguration->injector.flow = 243.6; // 23.2lb/h
        LDR.N    R0,??setDodgeRam1996_0
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setDodgeRam1996_0+0x4  ;; 0x4373999a
        STR      R1,[R0, #+8]
//   24 	engineConfiguration->map.sensor.type = MT_DODGE_NEON_2003;
        LDR.N    R0,??setDodgeRam1996_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+4
        STR      R1,[R0, #+248]
//   25 	engineConfiguration->algorithm = LM_SPEED_DENSITY;
        LDR.N    R0,??setDodgeRam1996_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+3
        STR      R1,[R0, #+424]
//   26 
//   27 	// set_cranking_charge_angle 30
//   28 	engineConfiguration->crankingChargeAngle = 30;
        LDR.N    R0,??setDodgeRam1996_0
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setDodgeRam1996_0+0x8  ;; 0x41f00000
        STR      R1,[R0, #+108]
//   29 
//   30 	// set_whole_fuel_map 6
//   31 	setWholeFuelMap(6 PASS_ENGINE_PARAMETER);
        VMOV.F32 S0,#6.0
          CFI FunCall _Z15setWholeFuelMapf
        BL       _Z15setWholeFuelMapf
//   32 
//   33 	//Base engine setting
//   34 	engineConfiguration->specs.cylindersCount = 8;
        LDR.N    R0,??setDodgeRam1996_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+8
        STR      R1,[R0, #+404]
//   35 	engineConfiguration->specs.firingOrder = FO_1_8_4_3_6_5_7_2;
        LDR.N    R0,??setDodgeRam1996_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+5
        STR      R1,[R0, #+408]
//   36 	engineConfiguration->specs.displacement = 5.2;
        LDR.N    R0,??setDodgeRam1996_0
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setDodgeRam1996_0+0xC  ;; 0x40a66666
        STR      R1,[R0, #+400]
//   37 
//   38 	boardConfiguration->triggerInputPins[0] = GPIOC_6;
        LDR.N    R0,??setDodgeRam1996_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+38
        STR      R1,[R0, #+296]
//   39 	boardConfiguration->triggerInputPins[1] = GPIOA_5;
        LDR.N    R0,??setDodgeRam1996_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+5
        STR      R1,[R0, #+300]
//   40 
//   41 	boardConfiguration->injectionPins[0] = GPIOE_2;
        LDR.N    R0,??setDodgeRam1996_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+66
        STR      R1,[R0, #+32]
//   42 	boardConfiguration->injectionPins[1] = GPIOB_9;
        LDR.N    R0,??setDodgeRam1996_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+25
        STR      R1,[R0, #+36]
//   43 	boardConfiguration->injectionPins[2] = GPIOD_5;
        LDR.N    R0,??setDodgeRam1996_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+53
        STR      R1,[R0, #+40]
//   44 	boardConfiguration->injectionPins[3] = GPIOB_8;
        LDR.N    R0,??setDodgeRam1996_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+24
        STR      R1,[R0, #+44]
//   45 
//   46 	boardConfiguration->injectionPins[4] = GPIOB_7;
        LDR.N    R0,??setDodgeRam1996_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+23
        STR      R1,[R0, #+48]
//   47 	boardConfiguration->injectionPins[5] = GPIOE_3;
        LDR.N    R0,??setDodgeRam1996_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+67
        STR      R1,[R0, #+52]
//   48 	boardConfiguration->injectionPins[6] = GPIOE_4;
        LDR.N    R0,??setDodgeRam1996_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+68
        STR      R1,[R0, #+56]
//   49 	boardConfiguration->injectionPins[7] = GPIOD_3;
        LDR.N    R0,??setDodgeRam1996_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+51
        STR      R1,[R0, #+60]
//   50 
//   51 	boardConfiguration->ignitionPins[0] = GPIOC_9;
        LDR.N    R0,??setDodgeRam1996_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+41
        STR      R1,[R0, #+84]
//   52 	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
        LDR.N    R0,??setDodgeRam1996_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+88]
//   53 	boardConfiguration->ignitionPins[2] = GPIO_UNASSIGNED;
        LDR.N    R0,??setDodgeRam1996_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+92]
//   54 	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
        LDR.N    R0,??setDodgeRam1996_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+96]
//   55 
//   56 	boardConfiguration->mainRelayPin = GPIOE_5;
        LDR.N    R0,??setDodgeRam1996_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+69
        STR      R1,[R0, #+308]
//   57 	boardConfiguration->fuelPumpPin = GPIOE_6;
        LDR.N    R0,??setDodgeRam1996_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+70
        STR      R1,[R0, #+24]
//   58 	boardConfiguration->fanPin = GPIO_UNASSIGNED;
        LDR.N    R0,??setDodgeRam1996_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+144]
//   59 
//   60 	engineConfiguration->engineChartSize = 450;
        LDR.N    R0,??setDodgeRam1996_0
        LDR      R0,[R0, #+0]
        MOV      R1,#+450
        STR      R1,[R0, #+1512]
//   61 
//   62 	// set_cranking_fuel 7
//   63 	engineConfiguration->cranking.baseFuel = 7;
        LDR.N    R0,??setDodgeRam1996_0
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setDodgeRam1996_0+0x14  ;; 0x40e00000
        STR      R1,[R0, #+92]
//   64 
//   65 	// set_global_trigger_offset_angle -50
//   66 	engineConfiguration->globalTriggerAngleOffset = -50;
        LDR.N    R0,??setDodgeRam1996_0
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setDodgeRam1996_0+0x18  ;; 0xc2480000
        STR      R1,[R0, #+460]
//   67 
//   68 	// set_global_fuel_correction 1
//   69 
//   70 	// set_whole_timing_map 10
//   71 	setWholeTimingTable(10 PASS_ENGINE_PARAMETER);
        VMOV.F32 S0,#10.0
          CFI FunCall _Z19setWholeTimingTablef
        BL       _Z19setWholeTimingTablef
//   72 
//   73 
//   74 	boardConfiguration->alternatorControlPin = GPIOD_7;
        LDR.N    R0,??setDodgeRam1996_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+55
        STR      R1,[R0, #+168]
//   75 	engineConfiguration->alternatorControlPFactor = 22;
        LDR.N    R0,??setDodgeRam1996_0
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setDodgeRam1996_0+0x1C  ;; 0x41b00000
        STR      R1,[R0, #+2172]
//   76 
//   77 	boardConfiguration->idle.solenoidPin = GPIOC_13;
        LDR.N    R0,??setDodgeRam1996_0+0x10
        LDR      R0,[R0, #+0]
        MOVS     R1,#+45
        STR      R1,[R0, #+0]
//   78 	boardConfiguration->idle.solenoidFrequency = 300;
        LDR.N    R0,??setDodgeRam1996_0+0x10
        LDR      R0,[R0, #+0]
        MOV      R1,#+300
        STR      R1,[R0, #+8]
//   79 
//   80 	engineConfiguration->vbattAdcChannel = EFI_ADC_14;
        LDR.N    R0,??setDodgeRam1996_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+14
        STR      R1,[R0, #+472]
//   81 //	engineConfiguration->vbattDividerCoeff = ((float) (8.93 + 41.27)) / 8.93 * 2;
//   82 	engineConfiguration->vbattDividerCoeff = 9.6;
        LDR.N    R0,??setDodgeRam1996_0
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setDodgeRam1996_0+0x20  ;; 0x4119999a
        STR      R1,[R0, #+468]
//   83 
//   84 	setDodgeSensor(&engineConfiguration->clt);
        LDR.N    R0,??setDodgeRam1996_0
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+256
          CFI FunCall _Z14setDodgeSensorP14ThermistorConf
        BL       _Z14setDodgeSensorP14ThermistorConf
//   85 	engineConfiguration->clt.config.bias_resistor = 2700;
        LDR.N    R0,??setDodgeRam1996_0
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setDodgeRam1996_0+0x24  ;; 0x4528c000
        STR      R1,[R0, #+280]
//   86 
//   87 	setDodgeSensor(&engineConfiguration->iat);
        LDR.N    R0,??setDodgeRam1996_0
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+288
          CFI FunCall _Z14setDodgeSensorP14ThermistorConf
        BL       _Z14setDodgeSensorP14ThermistorConf
//   88 	engineConfiguration->iat.config.bias_resistor = 2700;
        LDR.N    R0,??setDodgeRam1996_0
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setDodgeRam1996_0+0x24  ;; 0x4528c000
        STR      R1,[R0, #+312]
//   89 
//   90 	boardConfiguration->useStepperIdle = true;
        LDR.N    R0,??setDodgeRam1996_0+0x10
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+385]
        ORRS     R0,R0,#0x2
        LDR.N    R1,??setDodgeRam1996_0+0x10
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+385]
//   91 }
        POP      {R0,PC}          ;; return
        Nop      
        DATA
??setDodgeRam1996_0:
        DC32     engineConfiguration
        DC32     0x4373999a
        DC32     0x41f00000
        DC32     0x40a66666
        DC32     boardConfiguration
        DC32     0x40e00000
        DC32     0xc2480000
        DC32     0x41b00000
        DC32     0x4119999a
        DC32     0x4528c000
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
// 448 bytes in section .text
// 
// 448 bytes of CODE memory
//
//Errors: none
//Warnings: 1
