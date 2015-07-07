///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:10:07 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       vw.cpp                                               /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       vw.cpp -lCN F:\stuff\rusefi_sourceforge\firmware\iar /
//                       \Debug\List\ -lA F:\stuff\rusefi_sourceforge\firmwar /
//                       e\iar\Debug\List\ -o F:\stuff\rusefi_sourceforge\fir /
//                       mware\iar\Debug\Obj\ --no_cse --no_unroll            /
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
//                       vw.s                                                 /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME vw

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z12setEgoSensor12ego_sensor_e
        EXTERN _Z15setWholeFuelMapf
        EXTERN _Z16setOperationModeP22engine_configuration_s16operation_mode_e
        EXTERN _Z19setWholeTimingTablef
        EXTERN _Z28setCustomEngineConfigurationv
        EXTERN boardConfiguration
        EXTERN engineConfiguration

        PUBLIC _Z8setVwAbav
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\config\engines\vw.cpp
//    1 /*
//    2  * @file vw.cpp
//    3  *
//    4  * set_engine_type 32
//    5  *
//    6  * @date May 24, 2015
//    7  * @author Andrey Belomutskiy, (c) 2012-2015
//    8  */
//    9 
//   10 #include "main.h"
//   11 #include "vw.h"
//   12 #include "custom_engine.h"
//   13 #include "ego.h"
//   14 
//   15 EXTERN_ENGINE;
//   16 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z8setVwAbav
        THUMB
//   17 void setVwAba(DECLARE_ENGINE_PARAMETER_F) {
_Z8setVwAbav:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   18 	setCustomEngineConfiguration(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z28setCustomEngineConfigurationv
        BL       _Z28setCustomEngineConfigurationv
//   19 
//   20 	setWholeTimingTable(20 PASS_ENGINE_PARAMETER);
        VMOV.F32 S0,#20.0
          CFI FunCall _Z19setWholeTimingTablef
        BL       _Z19setWholeTimingTablef
//   21 	engineConfiguration->cranking.baseFuel = 12;
        LDR.N    R0,??setVwAba_0
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setVwAba_0+0x4  ;; 0x41400000
        STR      R1,[R0, #+92]
//   22 	// set_cranking_timing_angle 0
//   23 	engineConfiguration->crankingTimingAngle = 20;
        LDR.N    R0,??setVwAba_0
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setVwAba_0+0x8  ;; 0x41a00000
        STR      R1,[R0, #+440]
//   24 
//   25 	// set_whole_fuel_map 12
//   26 	setWholeFuelMap(12 PASS_ENGINE_PARAMETER);
        VMOV.F32 S0,#12.0
          CFI FunCall _Z15setWholeFuelMapf
        BL       _Z15setWholeFuelMapf
//   27 
//   28 	// set_global_trigger_offset_angle 90
//   29 	engineConfiguration->globalTriggerAngleOffset = 90;
        LDR.N    R0,??setVwAba_0
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setVwAba_0+0xC  ;; 0x42b40000
        STR      R1,[R0, #+460]
//   30 
//   31 
//   32 	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
        MOVS     R1,#+1
        LDR.N    R0,??setVwAba_0
        LDR      R0,[R0, #+0]
          CFI FunCall _Z16setOperationModeP22engine_configuration_s16operation_mode_e
        BL       _Z16setOperationModeP22engine_configuration_s16operation_mode_e
//   33 //	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
//   34 	engineConfiguration->trigger.type = TT_60_2_VW;
        LDR.N    R0,??setVwAba_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+20
        STR      R1,[R0, #+536]
//   35 
//   36 	engineConfiguration->mafAdcChannel = EFI_ADC_1;
        LDR.N    R0,??setVwAba_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+580]
//   37 
//   38 
//   39 	//Base engine setting
//   40 	engineConfiguration->specs.cylindersCount = 4;
        LDR.N    R0,??setVwAba_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+4
        STR      R1,[R0, #+404]
//   41 	engineConfiguration->specs.displacement = 2.5;
        LDR.N    R0,??setVwAba_0
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setVwAba_0+0x10  ;; 0x40200000
        STR      R1,[R0, #+400]
//   42 	engineConfiguration->injector.flow = 320; // 30lb/h
        LDR.N    R0,??setVwAba_0
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setVwAba_0+0x14  ;; 0x43a00000
        STR      R1,[R0, #+8]
//   43 	// set_algorithm 3
//   44 	engineConfiguration->algorithm = LM_SPEED_DENSITY;
        LDR.N    R0,??setVwAba_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+3
        STR      R1,[R0, #+424]
//   45 	engineConfiguration->map.sensor.type = MT_GM_3_BAR;
        LDR.N    R0,??setVwAba_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+6
        STR      R1,[R0, #+248]
//   46 
//   47 	engineConfiguration->ignitionMode = IM_ONE_COIL;
        LDR.N    R0,??setVwAba_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+444]
//   48 
//   49 	boardConfiguration->ignitionPins[0] = GPIOE_14; // Frankenso high side - pin 1G
        LDR.N    R0,??setVwAba_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+78
        STR      R1,[R0, #+84]
//   50 	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
        LDR.N    R0,??setVwAba_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+88]
//   51 	boardConfiguration->ignitionPins[2] = GPIO_UNASSIGNED;
        LDR.N    R0,??setVwAba_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+92]
//   52 	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
        LDR.N    R0,??setVwAba_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+96]
//   53 	boardConfiguration->ignitionPinMode = OM_DEFAULT;
        LDR.N    R0,??setVwAba_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+132]
//   54 
//   55 	setEgoSensor(ES_PLX PASS_ENGINE_PARAMETER);
        MOVS     R0,#+4
          CFI FunCall _Z12setEgoSensor12ego_sensor_e
        BL       _Z12setEgoSensor12ego_sensor_e
//   56 }
        POP      {R0,PC}          ;; return
        DATA
??setVwAba_0:
        DC32     engineConfiguration
        DC32     0x41400000
        DC32     0x41a00000
        DC32     0x42b40000
        DC32     0x40200000
        DC32     0x43a00000
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
//   57 
//   58 
// 
// 216 bytes in section .text
// 
// 216 bytes of CODE memory
//
//Errors: none
//Warnings: 1
