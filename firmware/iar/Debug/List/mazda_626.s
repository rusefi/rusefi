///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:19 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       mazda_626.cpp                                        /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       mazda_626.cpp -lCN F:\stuff\rusefi_sourceforge\firmw /
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
//                       mazda_626.s                                          /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME mazda_626

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z12setAlgorithm18engine_load_mode_e
        EXTERN _Z12setEgoSensor12ego_sensor_e
        EXTERN _Z14setFuelLoadBinff
        EXTERN _Z15setWholeFuelMapf
        EXTERN _Z18setCommonNTCSensorP14ThermistorConf
        EXTERN _Z19setWholeTimingTablef
        EXTERN _Z26setThermistorConfigurationP14ThermistorConfffffff
        EXTERN _Z27commonFrankensoAnalogInputsP22engine_configuration_s
        EXTERN _Z28setCustomEngineConfigurationv
        EXTERN _Z7setFsioi11brain_pin_ePKc
        EXTERN boardConfiguration
        EXTERN engineConfiguration

        PUBLIC _Z30setMazda626EngineConfigurationv
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\config\engines\mazda_626.cpp
//    1 /**
//    2  * @file	mazda_626.cpp
//    3  * MAZDA_626
//    4  * set_engine_type 28
//    5  *
//    6  * @date Jan 16, 2015
//    7  * @author Andrey Belomutskiy, (c) 2012-2015
//    8  */
//    9 
//   10 #include "main.h"
//   11 #include "mazda_626.h"
//   12 #include "engine_math.h"
//   13 #include "honda_accord.h"
//   14 #include "custom_engine.h"
//   15 #include "allsensors.h"
//   16 #include "fsio_impl.h"
//   17 #include "settings.h"
//   18 
//   19 EXTERN_ENGINE;
//   20 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z30setMazda626EngineConfigurationv
        THUMB
//   21 void setMazda626EngineConfiguration(DECLARE_ENGINE_PARAMETER_F) {
_Z30setMazda626EngineConfigurationv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   22 	setCustomEngineConfiguration(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z28setCustomEngineConfigurationv
        BL       _Z28setCustomEngineConfigurationv
//   23 
//   24 	engineConfiguration->trigger.type = TT_MAZDA_DOHC_1_4;
        LDR.N    R0,??setMazda626EngineConfiguration_0+0x20
        LDR      R0,[R0, #+0]
        MOVS     R1,#+15
        STR      R1,[R0, #+536]
//   25 
//   26 	engineConfiguration->engineChartSize = 150;
        LDR.N    R0,??setMazda626EngineConfiguration_0+0x20
        LDR      R0,[R0, #+0]
        MOVS     R1,#+150
        STR      R1,[R0, #+1512]
//   27 
//   28 	boardConfiguration->sensorChartMode = SC_TRIGGER;
        LDR.N    R0,??setMazda626EngineConfiguration_0+0x24
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+784]
//   29 	engineConfiguration->sensorChartFrequency = 2;
        LDR.N    R0,??setMazda626EngineConfiguration_0+0x20
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+532]
//   30 
//   31 	engineConfiguration->injector.flow = 330;
        LDR.N    R0,??setMazda626EngineConfiguration_0+0x20
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setMazda626EngineConfiguration_0+0x28  ;; 0x43a50000
        STR      R1,[R0, #+8]
//   32 	engineConfiguration->specs.displacement = 2.0;
        LDR.N    R0,??setMazda626EngineConfiguration_0+0x20
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1073741824
        STR      R1,[R0, #+400]
//   33 
//   34 	// set_global_trigger_offset_angle -42
//   35 	engineConfiguration->globalTriggerAngleOffset = -42;
        LDR.N    R0,??setMazda626EngineConfiguration_0+0x20
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setMazda626EngineConfiguration_0+0x2C  ;; 0xc2280000
        STR      R1,[R0, #+460]
//   36 
//   37 	// set_cranking_timing_angle 15
//   38 	engineConfiguration->crankingTimingAngle = 8;
        LDR.N    R0,??setMazda626EngineConfiguration_0+0x20
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1090519040
        STR      R1,[R0, #+440]
//   39 	engineConfiguration->crankingChargeAngle = 70;
        LDR.N    R0,??setMazda626EngineConfiguration_0+0x20
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setMazda626EngineConfiguration_0+0x30  ;; 0x428c0000
        STR      R1,[R0, #+108]
//   40 
//   41 	// set_cranking_fuel 9
//   42 	engineConfiguration->cranking.baseFuel = 9;
        LDR.N    R0,??setMazda626EngineConfiguration_0+0x20
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setMazda626EngineConfiguration_0+0x34  ;; 0x41100000
        STR      R1,[R0, #+92]
//   43 
//   44 	engineConfiguration->rpmHardLimit = 6000;
        LDR.N    R0,??setMazda626EngineConfiguration_0+0x20
        LDR      R0,[R0, #+0]
        MOVW     R1,#+6000
        STR      R1,[R0, #+420]
//   45 
//   46 	engineConfiguration->tpsMin = 80;
        LDR.N    R0,??setMazda626EngineConfiguration_0+0x20
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STRH     R1,[R0, #+82]
//   47 	engineConfiguration->tpsMax = 764;
        LDR.N    R0,??setMazda626EngineConfiguration_0+0x20
        LDR      R0,[R0, #+0]
        MOV      R1,#+764
        STRH     R1,[R0, #+84]
//   48 
//   49 	setAlgorithm(LM_SPEED_DENSITY PASS_ENGINE_PARAMETER);
        MOVS     R0,#+3
          CFI FunCall _Z12setAlgorithm18engine_load_mode_e
        BL       _Z12setAlgorithm18engine_load_mode_e
//   50 	setFuelLoadBin(0, 100 PASS_ENGINE_PARAMETER);
        VLDR.W   S1,??setMazda626EngineConfiguration_0  ;; 0x42c80000
        VLDR.W   S0,??setMazda626EngineConfiguration_0+0x4  ;; 0x0
          CFI FunCall _Z14setFuelLoadBinff
        BL       _Z14setFuelLoadBinff
//   51 
//   52 	// set_whole_fuel_map 9
//   53 	setWholeFuelMap(9 PASS_ENGINE_PARAMETER);
        VMOV.F32 S0,#9.0
          CFI FunCall _Z15setWholeFuelMapf
        BL       _Z15setWholeFuelMapf
//   54 
//   55 // set_whole_timing_map 10
//   56 	setWholeTimingTable(10 PASS_ENGINE_PARAMETER);
        VMOV.F32 S0,#10.0
          CFI FunCall _Z19setWholeTimingTablef
        BL       _Z19setWholeTimingTablef
//   57 
//   58 	// http://i.imgur.com/fclVzvu.jpg
//   59 	setCommonNTCSensor(&engineConfiguration->clt);
        LDR.N    R0,??setMazda626EngineConfiguration_0+0x20
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+256
          CFI FunCall _Z18setCommonNTCSensorP14ThermistorConf
        BL       _Z18setCommonNTCSensorP14ThermistorConf
//   60 	// http://i.imgur.com/2hI67yW.jpg
//   61 	setThermistorConfiguration(&engineConfiguration->iat, 13, 56000, 50, 9000, 80.0, 2700);
        VLDR.W   S5,??setMazda626EngineConfiguration_0+0x8  ;; 0x4528c000
        VLDR.W   S4,??setMazda626EngineConfiguration_0+0xC  ;; 0x42a00000
        VLDR.W   S3,??setMazda626EngineConfiguration_0+0x10  ;; 0x460ca000
        VLDR.W   S2,??setMazda626EngineConfiguration_0+0x14  ;; 0x42480000
        VLDR.W   S1,??setMazda626EngineConfiguration_0+0x18  ;; 0x475ac000
        VMOV.F32 S0,#13.0
        LDR.N    R0,??setMazda626EngineConfiguration_0+0x20
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+288
          CFI FunCall _Z26setThermistorConfigurationP14ThermistorConfffffff
        BL       _Z26setThermistorConfigurationP14ThermistorConfffffff
//   62 	engineConfiguration->iat.config.bias_resistor = 18000;
        LDR.N    R0,??setMazda626EngineConfiguration_0+0x20
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setMazda626EngineConfiguration_0+0x38  ;; 0x468ca000
        STR      R1,[R0, #+312]
//   63 
//   64 	commonFrankensoAnalogInputs(engineConfiguration);
        LDR.N    R0,??setMazda626EngineConfiguration_0+0x20
        LDR      R0,[R0, #+0]
          CFI FunCall _Z27commonFrankensoAnalogInputsP22engine_configuration_s
        BL       _Z27commonFrankensoAnalogInputsP22engine_configuration_s
//   65 //	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;
//   66 	engineConfiguration->mafAdcChannel = EFI_ADC_0;
        LDR.N    R0,??setMazda626EngineConfiguration_0+0x20
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+580]
//   67 
//   68 	// todo: 8.2 or 10k?
//   69 	engineConfiguration->vbattDividerCoeff = ((float) (10 + 33)) / 10 * 2;
        LDR.N    R0,??setMazda626EngineConfiguration_0+0x20
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setMazda626EngineConfiguration_0+0x3C  ;; 0x4109999a
        STR      R1,[R0, #+468]
//   70 
//   71 	engineConfiguration->map.sensor.type = MT_SUBY_DENSO;
        LDR.N    R0,??setMazda626EngineConfiguration_0+0x20
        LDR      R0,[R0, #+0]
        MOVS     R1,#+5
        STR      R1,[R0, #+248]
//   72 
//   73 
//   74 // todo	engineConfiguration->afr.hwChannel = EFI_ADC_3;
//   75 	setEgoSensor(ES_Innovate_MTX_L PASS_ENGINE_PARAMETER);
        MOVS     R0,#+1
          CFI FunCall _Z12setEgoSensor12ego_sensor_e
        BL       _Z12setEgoSensor12ego_sensor_e
//   76 
//   77 
//   78 #if EFI_FSIO || defined(__DOXYGEN__)
//   79 	// backup main relay pin
//   80 	setFsio(0, GPIOE_6, "1" PASS_ENGINE_PARAMETER);
        ADR.N    R2,??setMazda626EngineConfiguration_0+0x1C  ;; "1"
        MOVS     R1,#+70
        MOVS     R0,#+0
          CFI FunCall _Z7setFsioi11brain_pin_ePKc
        BL       _Z7setFsioi11brain_pin_ePKc
//   81 #endif
//   82 
//   83 	engineConfiguration->externalKnockSenseAdc = EFI_ADC_4;
        LDR.N    R0,??setMazda626EngineConfiguration_0+0x20
        LDR      R0,[R0, #+0]
        MOVS     R1,#+4
        STR      R1,[R0, #+1620]
//   84 
//   85 
//   86 }
        POP      {R0,PC}          ;; return
        DATA
??setMazda626EngineConfiguration_0:
        DC32     0x42c80000
        DC32     0x0
        DC32     0x4528c000
        DC32     0x42a00000
        DC32     0x460ca000
        DC32     0x42480000
        DC32     0x475ac000
        DC8      "1",0x0,0x0
        DC32     engineConfiguration
        DC32     boardConfiguration
        DC32     0x43a50000
        DC32     0xc2280000
        DC32     0x428c0000
        DC32     0x41100000
        DC32     0x468ca000
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

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 "1"

        END
// 
//   2 bytes in section .rodata
// 360 bytes in section .text
// 
// 360 bytes of CODE  memory
//   2 bytes of CONST memory
//
//Errors: none
//Warnings: 1
