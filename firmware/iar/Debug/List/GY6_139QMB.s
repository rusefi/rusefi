///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:37 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       GY6_139QMB.cpp                                       /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       GY6_139QMB.cpp -lCN F:\stuff\rusefi_sourceforge\firm /
//                       ware\iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge /
//                       \firmware\iar\Debug\List\ -o                         /
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
//                       GY6_139QMB.s                                         /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME GY6_139QMB

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z12setEgoSensor12ego_sensor_e
        EXTERN _Z13copyFuelTablePA16_KfPA16_f
        EXTERN _Z13setFuelRpmBinff
        EXTERN _Z14setFuelLoadBinff
        EXTERN _Z15setTimingRpmBinff
        EXTERN _Z16setOperationModeP22engine_configuration_s16operation_mode_e
        EXTERN _Z16setTimingLoadBinff
        EXTERN boardConfiguration
        EXTERN config
        EXTERN engineConfiguration

        PUBLIC _Z38setGy6139qmbDefaultEngineConfigurationv
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\config\engines\GY6_139QMB.cpp
//    1 /**
//    2  * @file	GY6_139QMB.cpp
//    3  * @brief	139qmb default engine configuration
//    4  *
//    5  * @date Feb 13, 2014
//    6  * @author rus084, (c) 2015
//    7  * @author Andrey Belomutskiy, (c) 2012-2015
//    8  */
//    9 
//   10 #include "main.h"
//   11 #include "GY6_139QMB.h"
//   12 #include "engine_math.h"
//   13 #include "engine_configuration.h"
//   14 #include "allsensors.h"
//   15 #include "advance_map.h"
//   16 
//   17 EXTERN_ENGINE
//   18 ;
//   19 

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//   20 static const fuel_table_t default_139qmb_fuel_table = {
default_139qmb_fuel_table:
        DATA
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H, 40C00000H
        DC32 40C00000H, 40C00000H, 40C00000H, 40C00000H
//   21 
//   22 { 6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   23 		6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   24 		6.000000, 6.000000 }, { 6.000000, 6.000000, 6.000000, 6.000000,
//   25 		6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   26 		6.000000, 6.000000, 6.000000, 6.000000, 6.000000 }, { 6.000000,
//   27 		6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   28 		6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   29 		6.000000 }, { 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   30 		6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   31 		6.000000, 6.000000, 6.000000, 6.000000 }, { 6.000000, 6.000000,
//   32 		6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   33 		6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000 },
//   34 		{ 6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   35 				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   36 				6.000000, 6.000000, 6.000000 }, { 6.000000, 6.000000, 6.000000,
//   37 				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   38 				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   39 				6.000000 }, { 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   40 				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   41 				6.000000, 6.000000, 6.000000, 6.000000, 6.000000 }, { 6.000000,
//   42 				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   43 				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   44 				6.000000, 6.000000, 6.000000 }, { 6.000000, 6.000000, 6.000000,
//   45 				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   46 				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   47 				6.000000 }, { 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   48 				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   49 				6.000000, 6.000000, 6.000000, 6.000000, 6.000000 }, { 6.000000,
//   50 				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   51 				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   52 				6.000000, 6.000000, 6.000000 }, { 6.000000, 6.000000, 6.000000,
//   53 				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   54 				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   55 				6.000000 }, { 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   56 				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   57 				6.000000, 6.000000, 6.000000, 6.000000, 6.000000 }, { 6.000000,
//   58 				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   59 				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   60 				6.000000, 6.000000, 6.000000 }, { 6.000000, 6.000000, 6.000000,
//   61 				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   62 				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
//   63 				6.000000 }
//   64 
//   65 };
//   66 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z20setDefault139qmbMapsv
        THUMB
//   67 static void setDefault139qmbMaps(DECLARE_ENGINE_PARAMETER_F) {
_Z20setDefault139qmbMapsv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   68 
//   69 	setFuelLoadBin(1.2, 4.4 PASS_ENGINE_PARAMETER);
        VLDR.W   S1,??DataTable1  ;; 0x408ccccd
        VLDR.W   S0,??DataTable1_1  ;; 0x3f99999a
          CFI FunCall _Z14setFuelLoadBinff
        BL       _Z14setFuelLoadBinff
//   70 	setFuelRpmBin(1000, 11000 PASS_ENGINE_PARAMETER);
        VLDR.W   S1,??DataTable1_2  ;; 0x462be000
        VLDR.W   S0,??DataTable1_3  ;; 0x447a0000
          CFI FunCall _Z13setFuelRpmBinff
        BL       _Z13setFuelRpmBinff
//   71 	setTimingLoadBin(1.2, 4.4 PASS_ENGINE_PARAMETER);
        VLDR.W   S1,??DataTable1  ;; 0x408ccccd
        VLDR.W   S0,??DataTable1_1  ;; 0x3f99999a
          CFI FunCall _Z16setTimingLoadBinff
        BL       _Z16setTimingLoadBinff
//   72 	setTimingRpmBin(1000, 11000 PASS_ENGINE_PARAMETER);
        VLDR.W   S1,??DataTable1_2  ;; 0x462be000
        VLDR.W   S0,??DataTable1_3  ;; 0x447a0000
          CFI FunCall _Z15setTimingRpmBinff
        BL       _Z15setTimingRpmBinff
//   73 
//   74 	copyFuelTable(default_139qmb_fuel_table, config->fuelTable);
        LDR.N    R0,??DataTable1_4
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+10496
        ADDS     R1,R1,#+184
        LDR.N    R0,??DataTable1_5
          CFI FunCall _Z13copyFuelTablePA16_KfPA16_f
        BL       _Z13copyFuelTablePA16_KfPA16_f
//   75 	//copyTimingTable(default_139qmb_timing_table, config->ignitionTable);
//   76 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0
//   77 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z38setGy6139qmbDefaultEngineConfigurationv
        THUMB
//   78 void setGy6139qmbDefaultEngineConfiguration(DECLARE_ENGINE_PARAMETER_F) {
_Z38setGy6139qmbDefaultEngineConfigurationv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   79 	setDefault139qmbMaps(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z20setDefault139qmbMapsv
        BL       _Z20setDefault139qmbMapsv
//   80 //        engineConfiguration->map.sensor.type = MT_3V_SENSOR;
//   81 	setEgoSensor(ES_NarrowBand PASS_ENGINE_PARAMETER);
        MOVS     R0,#+3
          CFI FunCall _Z12setEgoSensor12ego_sensor_e
        BL       _Z12setEgoSensor12ego_sensor_e
//   82 
//   83 	engineConfiguration->rpmHardLimit = 9000;
        LDR.N    R0,??DataTable1_6
        LDR      R0,[R0, #+0]
        MOVW     R1,#+9000
        STR      R1,[R0, #+420]
//   84 	engineConfiguration->cranking.rpm = 1100;
        LDR.N    R0,??DataTable1_6
        LDR      R0,[R0, #+0]
        MOVW     R1,#+1100
        STRH     R1,[R0, #+96]
//   85 	engineConfiguration->targetIdleRpm = 2000;
        LDR.N    R0,??DataTable1_6
        LDR      R0,[R0, #+0]
        MOV      R1,#+2000
        STR      R1,[R0, #+1516]
//   86 	engineConfiguration->analogInputDividerCoefficient = 1;
        LDR.N    R0,??DataTable1_6
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1065353216
        STR      R1,[R0, #+464]
//   87 	engineConfiguration->algorithm = LM_MAP;
        LDR.N    R0,??DataTable1_6
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+424]
//   88 	engineConfiguration->globalTriggerAngleOffset = 45;
        LDR.N    R0,??DataTable1_6
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable1_7  ;; 0x42340000
        STR      R1,[R0, #+460]
//   89 	engineConfiguration->bc.sensorChartMode = SC_MAP;
        LDR.N    R0,??DataTable1_6
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+1408]
//   90 	engineConfiguration->specs.displacement = 0.072; // 72cc
        LDR.N    R0,??DataTable1_6
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable1_8  ;; 0x3d9374bc
        STR      R1,[R0, #+400]
//   91 	engineConfiguration->specs.cylindersCount = 1;
        LDR.N    R0,??DataTable1_6
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+404]
//   92 	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
        MOVS     R1,#+1
        LDR.N    R0,??DataTable1_6
        LDR      R0,[R0, #+0]
          CFI FunCall _Z16setOperationModeP22engine_configuration_s16operation_mode_e
        BL       _Z16setOperationModeP22engine_configuration_s16operation_mode_e
//   93 
//   94 	engineConfiguration->specs.firingOrder = FO_ONE_CYLINDER;
        LDR.N    R0,??DataTable1_6
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+408]
//   95 
//   96 	/**
//   97 	 * We treat the trigger as 8-1 toothed wheel
//   98 	 */
//   99 	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL;
        LDR.N    R0,??DataTable1_6
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+536]
//  100 	engineConfiguration->trigger.customTotalToothCount = 8;
        LDR.N    R0,??DataTable1_6
        LDR      R0,[R0, #+0]
        MOVS     R1,#+8
        STR      R1,[R0, #+544]
//  101 	engineConfiguration->trigger.customSkippedToothCount = 1;
        LDR.N    R0,??DataTable1_6
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+548]
//  102 
//  103 	boardConfiguration->injectionPins[0] = GPIOC_9;
        LDR.N    R0,??DataTable1_9
        LDR      R0,[R0, #+0]
        MOVS     R1,#+41
        STR      R1,[R0, #+32]
//  104 	boardConfiguration->ignitionPins[0] = GPIOC_8;
        LDR.N    R0,??DataTable1_9
        LDR      R0,[R0, #+0]
        MOVS     R1,#+40
        STR      R1,[R0, #+84]
//  105 
//  106 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1:
        DC32     0x408ccccd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_1:
        DC32     0x3f99999a

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_2:
        DC32     0x462be000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_3:
        DC32     0x447a0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_4:
        DC32     config

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_5:
        DC32     default_139qmb_fuel_table

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_6:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_7:
        DC32     0x42340000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_8:
        DC32     0x3d9374bc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_9:
        DC32     boardConfiguration

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
// 1 024 bytes in section .rodata
//   286 bytes in section .text
// 
//   286 bytes of CODE  memory
// 1 024 bytes of CONST memory
//
//Errors: none
//Warnings: 1
