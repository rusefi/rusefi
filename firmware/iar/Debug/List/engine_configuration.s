///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:46 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\alg /
//                       o\engine_configuration.cpp                           /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\alg /
//                       o\engine_configuration.cpp -lCN                      /
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
//                       engine_configuration.s                               /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME engine_configuration

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z10setRoverv8v
        EXTERN _Z11setAcuraRSXP22engine_configuration_s
        EXTERN _Z12setEgoSensor12ego_sensor_e
        EXTERN _Z12setMiata1990v
        EXTERN _Z12setMiata1996v
        EXTERN _Z12setTableBin2Pfifff
        EXTERN _Z13setFuelRpmBinff
        EXTERN _Z13setTableValuePfS_iff
        EXTERN _Z14setFordInline6v
        EXTERN _Z14setFuelLoadBinff
        EXTERN _Z14setMiata1994_dv
        EXTERN _Z14setMiata1994_sv
        EXTERN _Z14setRpmTableBinPfi
        EXTERN _Z15setDodgeRam1996v
        EXTERN _Z15setFordEscortGtv
        EXTERN _Z15setTimingRpmBinff
        EXTERN _Z16setMiniCooperR50P22engine_configuration_s
        EXTERN _Z16setSubaru2003Wrxv
        EXTERN _Z16setTimingLoadBinff
        EXTERN _Z17setDefaultVETablev
        EXTERN _Z18setBosch0280218037P19persistent_config_s
        EXTERN _Z19syncTunerStudioCopyv
        EXTERN _Z20prepareOutputSignalsv
        EXTERN _Z22applyFsioConfigurationv
        EXTERN _Z22setFrankenstein_01_LCDP21board_configuration_s
        EXTERN _Z23setDefaultEtbParametersv
        EXTERN _Z24convertVoltageTo10bitADCf
        EXTERN _Z25initializeIgnitionActionsffP9ArrayListI13IgnitionEventLi80EE
        EXTERN _Z26setMitsubishiConfigurationv
        EXTERN _Z26setTestEngineConfigurationv
        EXTERN _Z26setThermistorConfigurationP14ThermistorConfffffff
        EXTERN _Z28setCustomEngineConfigurationv
        EXTERN _Z29setDefaultIatTimingCorrectionv
        EXTERN _Z30setDefaultAlternatorParametersv
        EXTERN _Z30setHondaAccordConfigurationDipv
        EXTERN _Z30setMazda323EngineConfigurationP22engine_configuration_s
        EXTERN _Z30setMazda626EngineConfigurationv
        EXTERN _Z31setHondaAccordConfiguration1_24v
        EXTERN _Z32setFordAspireEngineConfigurationv
        EXTERN _Z34setDodgeNeonNGCEngineConfigurationv
        EXTERN _Z34setMazdaMiataNbEngineConfigurationv
        EXTERN _Z35setDodgeNeon1995EngineConfigurationv
        EXTERN _Z35setNissanPrimeraEngineConfigurationP22engine_configuration_s
        EXTERN _Z36setCitroenBerlingoTU3JPConfigurationv
        EXTERN _Z37setHondaAccordConfigurationThreeWiresv
        EXTERN _Z38setGy6139qmbDefaultEngineConfigurationv
        EXTERN _Z39setFordFiestaDefaultEngineConfigurationv
        EXTERN _Z8setGm2_2v
        EXTERN _Z8setSachsv
        EXTERN _Z8setVwAbav
        EXTERN _Z9setBmwE34v
        EXTERN _ZN12FuelScheduleC1Ev
        EXTERN _ZN12TriggerShape16getOperationModeEv
        EXTERN _ZN12TriggerShape22initializeTriggerShapeEP7Logging
        EXTERN _ZN13IgnitionEventC1Ev
        EXTERN _ZNK12TriggerShape7getSizeEv
        EXTERN _ZNK12TriggerShape9getLengthEv
        EXTERN __aeabi_memset
        EXTERN __aeabi_vec_ctor_nocookie_nodtor
        EXTERN boardConfiguration
        EXTERN config
        EXTERN engine
        EXTERN engineConfiguration
        EXTERN firmwareError
        EXTERN persistentState
        EXTERN scheduleMsg
        EXTERN setSaturnIonEngineConfiguration
        EXTERN strcpy
        EXTERN warning

        PUBLIC _Z12setTimingMapPA16_ff
        PUBLIC _Z13copyFuelTablePA16_KfPA16_f
        PUBLIC _Z13prepareShapesv
        PUBLIC _Z15copyTimingTablePA16_KfPA16_f
        PUBLIC _Z15setWholeFuelMapf
        PUBLIC _Z16getOperationModePK22engine_configuration_s
        PUBLIC _Z16getRpmMultiplier16operation_mode_e
        PUBLIC _Z16setConstantDwellf
        PUBLIC _Z16setOperationModeP22engine_configuration_s16operation_mode_e
        PUBLIC _Z19setWholeTimingTablef
        PUBLIC _Z20setFuelTablesLoadBinff
        PUBLIC _Z21resetConfigurationExtP7Logging13engine_type_e
        PUBLIC _Z23setDefaultConfigurationv
        PUBLIC _Z24prepareVoidConfigurationP22engine_configuration_s
        PUBLIC _Z24setFrankenso0_1_joystickP22engine_configuration_s
        PUBLIC _Z26setWholeIatCorrTimingTablef
        PUBLIC _Z27commonFrankensoAnalogInputsP22engine_configuration_s
        PUBLIC _Z29getGlobalConfigurationVersionv
        PUBLIC _Z31applyNonPersistentConfigurationP7Logging
        PUBLIC _Z35incrementGlobalConfigurationVersionv
        PUBLIC _Z6setMapPA16_ff
        PUBLIC _ZN23engine_configuration2_sC1Ev
        PUBLIC _ZN23engine_configuration2_sC2Ev
        PUBLIC _ZN9ArrayListI13IgnitionEventLi80EE5resetEv
        PUBLIC _ZN9ArrayListI13IgnitionEventLi80EEC1Ev
        PUBLIC _ZN9ArrayListI13IgnitionEventLi80EEC2Ev
        PUBLIC memset
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\algo\engine_configuration.cpp
//    1 /**
//    2  * @file	engine_configuration.cpp
//    3  * @brief	Utility method related to the engine configuration data structure.
//    4  *
//    5  * @date Nov 22, 2013
//    6  * @author Andrey Belomutskiy, (c) 2012-2015
//    7  *
//    8  * This file is part of rusEfi - see http://rusefi.com
//    9  *
//   10  * rusEfi is free software; you can redistribute it and/or modify it under the terms of
//   11  * the GNU General Public License as published by the Free Software Foundation; either
//   12  * version 3 of the License, or (at your option) any later version.
//   13  *
//   14  * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
//   15  * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   16  * GNU General Public License for more details.
//   17  *
//   18  * You should have received a copy of the GNU General Public License along with this program.
//   19  * If not, see <http://www.gnu.org/licenses/>.
//   20  *
//   21  */
//   22 
//   23 #include "main.h"
//   24 #include "engine_configuration.h"
//   25 #include "fsio_impl.h"
//   26 #include "allsensors.h"
//   27 #include "interpolation.h"
//   28 #include "trigger_decoder.h"
//   29 #include "engine_math.h"
//   30 #include "speed_density.h"
//   31 #include "advance_map.h"
//   32 #if EFI_PROD_CODE
//   33 #include "electronic_throttle.h"
//   34 #include "alternatorController.h"
//   35 #endif
//   36 
//   37 #include "custom_engine.h"
//   38 #include "acura_rsx.h"
//   39 #include "audi_aan.h"
//   40 #include "bmw_e34.h"
//   41 #include "dodge_neon.h"
//   42 #include "ford_aspire.h"
//   43 #include "ford_fiesta.h"
//   44 #include "ford_1995_inline_6.h"
//   45 #include "snow_blower.h"
//   46 #include "nissan_primera.h"
//   47 #include "honda_accord.h"
//   48 #include "GY6_139QMB.h"
//   49 #include "mazda_miata_nb.h"
//   50 #include "mazda_323.h"
//   51 #include "mazda_626.h"
//   52 #include "saturn_ion.h"
//   53 #include "MiniCooperR50.h"
//   54 #include "mazda_miata.h"
//   55 #include "citroenBerlingoTU3JP.h"
//   56 #include "rover_v8.h"
//   57 #include "mitsubishi.h"
//   58 #include "subaru.h"
//   59 #include "test_engine.h"
//   60 #include "sachs.h"
//   61 #include "gm_2_2.h"
//   62 #include "dodge_ram.h"
//   63 #include "vw.h"
//   64 
//   65 EXTERN_ENGINE;
//   66 
//   67 #if EFI_TUNER_STUDIO
//   68 #include "tunerstudio.h"
//   69 #endif
//   70 
//   71 //#define TS_DEFAULT_SPEED 115200
//   72 #define TS_DEFAULT_SPEED 38400
//   73 
//   74 #define xxxxx 0
//   75 
//   76 static fuel_table_t alphaNfuel = {
//   77 		{/*0  engineLoad=0.00*/   /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
//   78 		{/*1  engineLoad=6.66*/   /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
//   79 		{/*2  engineLoad=13.33*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
//   80 		{/*3  engineLoad=20.00*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
//   81 		{/*4  engineLoad=26.66*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
//   82 		{/*5  engineLoad=33.33*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
//   83 		{/*6  engineLoad=40.00*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
//   84 		{/*7  engineLoad=46.66*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
//   85 		{/*8  engineLoad=53.33*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
//   86 		{/*9  engineLoad=60.00*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
//   87 		{/*10 engineLoad=66.66*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
//   88 		{/*11 engineLoad=73.33*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
//   89 		{/*12 engineLoad=80.00*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
//   90 		{/*13 engineLoad=86.66*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
//   91 		{/*14 engineLoad=93.33*/  /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx},
//   92 		{/*15 engineLoad=100.00*/ /*0 800.0*/xxxxx, /*1 1213.0*/xxxxx, /*2 1626.0*/xxxxx, /*3 2040.0*/xxxxx, /*4 2453.0*/xxxxx, /*5 2866.0*/xxxxx, /*6 3280.0*/xxxxx, /*7 3693.0*/xxxxx, /*8 4106.0*/xxxxx, /*9 4520.0*/xxxxx, /*10 4933.0*/xxxxx, /*11 5346.0*/xxxxx, /*12 5760.0*/xxxxx, /*13 6173.0*/xxxxx, /*14 6586.0*/xxxxx, /*15 7000.0*/xxxxx}
//   93 		};
//   94 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   95 static volatile int globalConfigurationVersion = 0;
globalConfigurationVersion:
        DS8 4

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _ZN23engine_configuration2_sC2Ev
        THUMB
// __code __interwork __softfp engine_configuration2_s::subobject engine_configuration2_s()
_ZN23engine_configuration2_sC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN23engine_configuration2_sC1Ev
        BL       _ZN23engine_configuration2_sC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0
//   96 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z29getGlobalConfigurationVersionv
          CFI NoCalls
        THUMB
//   97 int getGlobalConfigurationVersion(void) {
//   98 	return globalConfigurationVersion;
_Z29getGlobalConfigurationVersionv:
        LDR.N    R0,??DataTable6
        LDR      R0,[R0, #+0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   99 }
//  100 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z35incrementGlobalConfigurationVersionv
          CFI NoCalls
        THUMB
//  101 void incrementGlobalConfigurationVersion(void) {
//  102 	globalConfigurationVersion++;
_Z35incrementGlobalConfigurationVersionv:
        LDR.N    R0,??DataTable6
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??DataTable6
        STR      R0,[R1, #+0]
//  103 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock2
//  104 
//  105 /**
//  106  * @brief Sets the same dwell time across the whole getRpm() range
//  107  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z16setConstantDwellf
          CFI NoCalls
        THUMB
//  108 void setConstantDwell(floatms_t dwellMs DECLARE_ENGINE_PARAMETER_S) {
//  109 	for (int i = 0; i < DWELL_CURVE_SIZE; i++) {
_Z16setConstantDwellf:
        MOVS     R0,#+0
??setConstantDwell_0:
        CMP      R0,#+8
        BGE.N    ??setConstantDwell_1
//  110 		engineConfiguration->sparkDwellBins[i] = 1000 * i;
        MOV      R1,#+1000
        MUL      R1,R1,R0
        VMOV     S1,R1
        VCVT.F32.S32 S1,S1
        LDR.N    R1,??DataTable6_1
        LDR      R1,[R1, #+0]
        ADDS     R1,R1,R0, LSL #+2
        VSTR     S1,[R1, #+336]
//  111 		engineConfiguration->sparkDwell[i] = dwellMs;
        LDR.N    R1,??DataTable6_1
        LDR      R1,[R1, #+0]
        ADDS     R1,R1,R0, LSL #+2
        VSTR     S0,[R1, #+368]
//  112 	}
        ADDS     R0,R0,#+1
        B.N      ??setConstantDwell_0
//  113 }
??setConstantDwell_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock3
//  114 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z6setMapPA16_ff
          CFI NoCalls
        THUMB
//  115 void setMap(fuel_table_t table, float value) {
//  116 	for (int l = 0; l < FUEL_LOAD_COUNT; l++) {
_Z6setMapPA16_ff:
        MOVS     R1,#+0
??setMap_0:
        CMP      R1,#+16
        BGE.N    ??setMap_1
//  117 		for (int r = 0; r < FUEL_RPM_COUNT; r++) {
        MOVS     R2,#+0
??setMap_2:
        CMP      R2,#+16
        BGE.N    ??setMap_3
//  118 			table[l][r] = value;
        MOVS     R3,#+64
        MLA      R3,R3,R1,R0
        ADDS     R3,R3,R2, LSL #+2
        VSTR     S0,[R3, #0]
//  119 		}
        ADDS     R2,R2,#+1
        B.N      ??setMap_2
//  120 	}
??setMap_3:
        ADDS     R1,R1,#+1
        B.N      ??setMap_0
//  121 }
??setMap_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//  122 
//  123 static void setWholeVEMap(float value DECLARE_ENGINE_PARAMETER_S) {
//  124 	setMap(config->veTable, value);
//  125 }
//  126 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z15setWholeFuelMapf
        THUMB
//  127 void setWholeFuelMap(float value DECLARE_ENGINE_PARAMETER_S) {
_Z15setWholeFuelMapf:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  128 	setMap(config->fuelTable, value);
        VMOV.F32 S0,S16
        LDR.N    R0,??DataTable6_2
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+10496
        ADDS     R0,R0,#+184
          CFI FunCall _Z6setMapPA16_ff
        BL       _Z6setMapPA16_ff
//  129 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock5
//  130 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z20setFuelTablesLoadBinff
        THUMB
//  131 void setFuelTablesLoadBin(float minValue, float maxValue DECLARE_ENGINE_PARAMETER_S) {
_Z20setFuelTablesLoadBinff:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
        VMOV.F32 S17,S1
//  132 	setTableBin2(config->injPhaseLoadBins, FUEL_LOAD_COUNT, minValue, maxValue, 1);
        VMOV.F32 S2,#1.0
        VMOV.F32 S1,S17
        VMOV.F32 S0,S16
        MOVS     R1,#+16
        LDR.N    R0,??DataTable6_2
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+10496
        ADDS     R0,R0,#+56
          CFI FunCall _Z12setTableBin2Pfifff
        BL       _Z12setTableBin2Pfifff
//  133 	setTableBin2(config->veLoadBins, FUEL_LOAD_COUNT, minValue, maxValue, 1);
        VMOV.F32 S2,#1.0
        VMOV.F32 S1,S17
        VMOV.F32 S0,S16
        MOVS     R1,#+16
        LDR.N    R0,??DataTable6_2
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+13824
        ADDS     R0,R0,#+184
          CFI FunCall _Z12setTableBin2Pfifff
        BL       _Z12setTableBin2Pfifff
//  134 	setTableBin2(config->afrLoadBins, FUEL_LOAD_COUNT, minValue, maxValue, 1);
        VMOV.F32 S2,#1.0
        VMOV.F32 S1,S17
        VMOV.F32 S0,S16
        MOVS     R1,#+16
        LDR.N    R0,??DataTable6_2
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+15104
        ADDS     R0,R0,#+56
          CFI FunCall _Z12setTableBin2Pfifff
        BL       _Z12setTableBin2Pfifff
//  135 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock6
//  136 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z12setTimingMapPA16_ff
          CFI NoCalls
        THUMB
//  137 void setTimingMap(ignition_table_t map, float value) {
//  138 	for (int l = 0; l < IGN_LOAD_COUNT; l++) {
_Z12setTimingMapPA16_ff:
        MOVS     R1,#+0
??setTimingMap_0:
        CMP      R1,#+16
        BGE.N    ??setTimingMap_1
//  139 		for (int r = 0; r < IGN_RPM_COUNT; r++) {
        MOVS     R2,#+0
??setTimingMap_2:
        CMP      R2,#+16
        BGE.N    ??setTimingMap_3
//  140 			map[l][r] = value;
        MOVS     R3,#+64
        MLA      R3,R3,R1,R0
        ADDS     R3,R3,R2, LSL #+2
        VSTR     S0,[R3, #0]
//  141 		}
        ADDS     R2,R2,#+1
        B.N      ??setTimingMap_2
//  142 	}
??setTimingMap_3:
        ADDS     R1,R1,#+1
        B.N      ??setTimingMap_0
//  143 }
??setTimingMap_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock7
//  144 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z26setWholeIatCorrTimingTablef
        THUMB
//  145 void setWholeIatCorrTimingTable(float value DECLARE_ENGINE_PARAMETER_S) {
_Z26setWholeIatCorrTimingTablef:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  146 	setTimingMap(config->ignitionIatCorrTable, value);
        VMOV.F32 S0,S16
        LDR.N    R0,??DataTable6_2
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+8192
        ADDS     R0,R0,#+184
          CFI FunCall _Z12setTimingMapPA16_ff
        BL       _Z12setTimingMapPA16_ff
//  147 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock8
//  148 
//  149 /**
//  150  * See also crankingTimingAngle
//  151  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z19setWholeTimingTablef
        THUMB
//  152 void setWholeTimingTable(angle_t value DECLARE_ENGINE_PARAMETER_S) {
_Z19setWholeTimingTablef:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
//  153 	setTimingMap(config->ignitionTable, value);
        VMOV.F32 S0,S16
        LDR.N    R0,??DataTable6_2
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+11776
        ADDS     R0,R0,#+56
          CFI FunCall _Z12setTimingMapPA16_ff
        BL       _Z12setTimingMapPA16_ff
//  154 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock9

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6:
        DC32     globalConfigurationVersion

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_1:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6_2:
        DC32     config
//  155 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z20initTemperatureCurveiPfS_
          CFI NoCalls
        THUMB
//  156 static void initTemperatureCurve(int size, float *bins, float *values) {
_Z20initTemperatureCurveiPfS_:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
//  157 	for (int i = 0; i < size; i++) {
        MOVS     R3,#+0
??initTemperatureCurve_0:
        CMP      R3,R0
        BGE.N    ??initTemperatureCurve_1
//  158 		bins[i] = -40 + i * 10;
        MOVS     R4,#+10
        MUL      R4,R4,R3
        SUBS     R4,R4,#+40
        VMOV     S0,R4
        VCVT.F32.S32 S0,S0
        ADDS     R4,R1,R3, LSL #+2
        VSTR     S0,[R4, #0]
//  159 		values[i] = 1; // this correction is a multiplier
        MOVS     R4,#+1065353216
        STR      R4,[R2, R3, LSL #+2]
//  160 	}
        ADDS     R3,R3,#+1
        B.N      ??initTemperatureCurve_0
//  161 }
??initTemperatureCurve_1:
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock10
//  162 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z24prepareVoidConfigurationP22engine_configuration_s
        THUMB
//  163 void prepareVoidConfiguration(engine_configuration_s *activeConfiguration) {
_Z24prepareVoidConfigurationP22engine_configuration_s:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  164 	memset(activeConfiguration, 0, sizeof(engine_configuration_s));
        MOVW     R2,#+2216
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall memset
        BL       memset
//  165 	board_configuration_s *boardConfiguration = &activeConfiguration->bc;
        ADDS     R5,R4,#+624
//  166 
//  167 	boardConfiguration->triggerInputPins[0] = GPIO_UNASSIGNED;
        MOVS     R0,#+80
        STR      R0,[R5, #+296]
//  168 	boardConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
        MOVS     R0,#+80
        STR      R0,[R5, #+300]
//  169 	boardConfiguration->triggerInputPins[2] = GPIO_UNASSIGNED;
        MOVS     R0,#+80
        STR      R0,[R5, #+304]
//  170 
//  171 	for (int i = 0; i < JOYSTICK_PIN_COUNT; i++) {
        MOVS     R0,#+0
??prepareVoidConfiguration_0:
        CMP      R0,#+5
        BGE.N    ??prepareVoidConfiguration_1
//  172 		boardConfiguration->joystickPins[i] = GPIO_UNASSIGNED;
        ADDS     R1,R5,R0, LSL #+2
        MOVS     R2,#+80
        STR      R2,[R1, #+580]
//  173 	}
        ADDS     R0,R0,#+1
        B.N      ??prepareVoidConfiguration_0
//  174 
//  175 	boardConfiguration->acRelayPin = GPIO_UNASSIGNED;
??prepareVoidConfiguration_1:
        MOVS     R0,#+80
        STR      R0,[R5, #+608]
//  176 	boardConfiguration->acRelayPinMode = OM_DEFAULT;
        MOVS     R0,#+0
        STR      R0,[R5, #+612]
//  177 
//  178 #if EFI_PROD_CODE
//  179 	setDefaultAlternatorParameters();
          CFI FunCall _Z30setDefaultAlternatorParametersv
        BL       _Z30setDefaultAlternatorParametersv
//  180 	setDefaultEtbParameters();
          CFI FunCall _Z23setDefaultEtbParametersv
        BL       _Z23setDefaultEtbParametersv
//  181 #endif
//  182 
//  183 	boardConfiguration->wboHeaterPin = GPIO_UNASSIGNED;
        MOVS     R0,#+80
        STR      R0,[R5, #+260]
//  184 	boardConfiguration->wboPumpPin = GPIO_UNASSIGNED;
        MOVS     R0,#+80
        STR      R0,[R5, #+264]
//  185 
//  186 	boardConfiguration->mainRelayPin = GPIO_UNASSIGNED;
        MOVS     R0,#+80
        STR      R0,[R5, #+308]
//  187 	boardConfiguration->mainRelayPinMode = OM_DEFAULT;
        MOVS     R0,#+0
        STR      R0,[R5, #+412]
//  188 	boardConfiguration->idle.solenoidPin = GPIO_UNASSIGNED;
        MOVS     R0,#+80
        STR      R0,[R5, #+0]
//  189 	boardConfiguration->idle.solenoidPinMode = OM_DEFAULT;
        MOVS     R0,#+0
        STR      R0,[R5, #+4]
//  190 	boardConfiguration->fuelPumpPin = GPIO_UNASSIGNED;
        MOVS     R0,#+80
        STR      R0,[R5, #+24]
//  191 	boardConfiguration->fuelPumpPinMode = OM_DEFAULT;
        MOVS     R0,#+0
        STR      R0,[R5, #+28]
//  192 	boardConfiguration->etbControlPin1 = GPIO_UNASSIGNED;
        MOVS     R0,#+80
        STR      R0,[R5, #+240]
//  193 	boardConfiguration->etbControlPin2 = GPIO_UNASSIGNED;
        MOVS     R0,#+80
        STR      R0,[R5, #+244]
//  194 	boardConfiguration->etbDirectionPin1 = GPIO_UNASSIGNED;
        MOVS     R0,#+80
        STR      R0,[R5, #+232]
//  195 	boardConfiguration->etbDirectionPin2 = GPIO_UNASSIGNED;
        MOVS     R0,#+80
        STR      R0,[R5, #+236]
//  196 	boardConfiguration->o2heaterPin = GPIO_UNASSIGNED;
        MOVS     R0,#+80
        STR      R0,[R5, #+376]
//  197 
//  198 	boardConfiguration->malfunctionIndicatorPin = GPIO_UNASSIGNED;
        MOVS     R0,#+80
        STR      R0,[R5, #+136]
//  199 	boardConfiguration->malfunctionIndicatorPinMode = OM_DEFAULT;
        MOVS     R0,#+0
        STR      R0,[R5, #+140]
//  200 
//  201 	boardConfiguration->fanPin = GPIO_UNASSIGNED;
        MOVS     R0,#+80
        STR      R0,[R5, #+144]
//  202 	boardConfiguration->fanPinMode = OM_DEFAULT;
        MOVS     R0,#+0
        STR      R0,[R5, #+148]
//  203 
//  204 	boardConfiguration->clutchDownPin = GPIO_UNASSIGNED;
        MOVS     R0,#+80
        STR      R0,[R5, #+160]
//  205 	boardConfiguration->clutchDownPinMode = PI_PULLUP;
        MOVS     R0,#+1
        STR      R0,[R5, #+164]
//  206 	boardConfiguration->clutchUpPin = GPIO_UNASSIGNED;
        MOVS     R0,#+80
        STR      R0,[R5, #+796]
//  207 	boardConfiguration->clutchUpPinMode = PI_PULLUP;
        MOVS     R0,#+1
        STR      R0,[R5, #+800]
//  208 
//  209 	for (int i = 0;i < INJECTION_PIN_COUNT;i++) {
        MOVS     R0,#+0
??prepareVoidConfiguration_2:
        CMP      R0,#+12
        BGE.N    ??prepareVoidConfiguration_3
//  210 		boardConfiguration->injectionPins[i] = GPIO_UNASSIGNED;
        ADDS     R1,R5,R0, LSL #+2
        MOVS     R2,#+80
        STR      R2,[R1, #+32]
//  211 	}
        ADDS     R0,R0,#+1
        B.N      ??prepareVoidConfiguration_2
//  212 	boardConfiguration->injectionPinMode = OM_DEFAULT;
??prepareVoidConfiguration_3:
        MOVS     R0,#+0
        STR      R0,[R5, #+80]
//  213 
//  214 	for (int i = 0;i < IGNITION_PIN_COUNT;i++) {
        MOVS     R0,#+0
??prepareVoidConfiguration_4:
        CMP      R0,#+12
        BGE.N    ??prepareVoidConfiguration_5
//  215 		boardConfiguration->ignitionPins[i] = GPIO_UNASSIGNED;
        ADDS     R1,R5,R0, LSL #+2
        MOVS     R2,#+80
        STR      R2,[R1, #+84]
//  216 	}
        ADDS     R0,R0,#+1
        B.N      ??prepareVoidConfiguration_4
//  217 	boardConfiguration->ignitionPinMode = OM_DEFAULT;
??prepareVoidConfiguration_5:
        MOVS     R0,#+0
        STR      R0,[R5, #+132]
//  218 
//  219 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock11
//  220 
//  221 /**
//  222  * @brief	Global default engine configuration
//  223  * This method sets the default global engine configuration. These values are later overridden by engine-specific defaults
//  224  * and the settings saves in flash memory.
//  225  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _Z23setDefaultConfigurationv
        THUMB
//  226 void setDefaultConfiguration(DECLARE_ENGINE_PARAMETER_F) {
_Z23setDefaultConfigurationv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  227 #if ! EFI_UNIT_TEST
//  228 	memset(&persistentState.persistentConfiguration, 0, sizeof(persistentState.persistentConfiguration));
        MOVW     R2,#+15288
        MOVS     R1,#+0
        LDR.W    R0,??setDefaultConfiguration_0
          CFI FunCall memset
        BL       memset
//  229 #endif
//  230 	prepareVoidConfiguration(engineConfiguration);
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
          CFI FunCall _Z24prepareVoidConfigurationP22engine_configuration_s
        BL       _Z24prepareVoidConfigurationP22engine_configuration_s
//  231 
//  232 	boardConfiguration->mafSensorType = Bosch0280218037;
        LDR.W    R0,??setDefaultConfiguration_0+0x8
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+788]
//  233 	setBosch0280218037(config);
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
          CFI FunCall _Z18setBosch0280218037P19persistent_config_s
        BL       _Z18setBosch0280218037P19persistent_config_s
//  234 
//  235 	engineConfiguration->injector.lag = 1.0;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1065353216
        STR      R1,[R0, #+12]
//  236 
//  237 	engineConfiguration->acCutoffLowRpm = 700;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOV      R1,#+700
        STRH     R1,[R0, #+1528]
//  238 	engineConfiguration->acCutoffHighRpm = 5000;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVW     R1,#+5000
        STRH     R1,[R0, #+1530]
//  239 
//  240 	initTemperatureCurve(IAT_CURVE_SIZE, config->iatFuelCorrBins, config->iatFuelCorr);
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R2,R0,#+5888
        ADDS     R2,R2,#+120
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+5888
        ADDS     R1,R1,#+56
        MOVS     R0,#+16
          CFI FunCall _Z20initTemperatureCurveiPfS_
        BL       _Z20initTemperatureCurveiPfS_
//  241 	initTemperatureCurve(CLT_CURVE_SIZE, config->cltFuelCorrBins, config->cltFuelCorr);
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R2,R0,#+5632
        ADDS     R2,R2,#+248
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+5632
        ADDS     R1,R1,#+184
        MOVS     R0,#+16
          CFI FunCall _Z20initTemperatureCurveiPfS_
        BL       _Z20initTemperatureCurveiPfS_
//  242 
//  243 	initTemperatureCurve(CLT_CURVE_SIZE, config->cltIdleCorrBins, config->cltIdleCorr);
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R2,R0,#+6144
        ADDS     R2,R2,#+120
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+6144
        ADDS     R1,R1,#+56
        MOVS     R0,#+16
          CFI FunCall _Z20initTemperatureCurveiPfS_
        BL       _Z20initTemperatureCurveiPfS_
//  244 
//  245 
//  246 	setTableValue(config->cltIdleCorrBins, config->cltIdleCorr, CLT_CURVE_SIZE, -40, 1.5);
        VMOV.F32 S1,#1.5
        VLDR.W   S0,??setDefaultConfiguration_1  ;; 0xc2200000
        MOVS     R2,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+6144
        ADDS     R1,R1,#+120
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+6144
        ADDS     R0,R0,#+56
          CFI FunCall _Z13setTableValuePfS_iff
        BL       _Z13setTableValuePfS_iff
//  247 	setTableValue(config->cltIdleCorrBins, config->cltIdleCorr, CLT_CURVE_SIZE, -30, 1.5);
        VMOV.F32 S1,#1.5
        VMOV.F32 S0,#-30.0
        MOVS     R2,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+6144
        ADDS     R1,R1,#+120
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+6144
        ADDS     R0,R0,#+56
          CFI FunCall _Z13setTableValuePfS_iff
        BL       _Z13setTableValuePfS_iff
//  248 	setTableValue(config->cltIdleCorrBins, config->cltIdleCorr, CLT_CURVE_SIZE, -20, 1.42);
        VLDR.W   S1,??setDefaultConfiguration_2  ;; 0x3fb5c28f
        VMOV.F32 S0,#-20.0
        MOVS     R2,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+6144
        ADDS     R1,R1,#+120
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+6144
        ADDS     R0,R0,#+56
          CFI FunCall _Z13setTableValuePfS_iff
        BL       _Z13setTableValuePfS_iff
//  249 	setTableValue(config->cltIdleCorrBins, config->cltIdleCorr, CLT_CURVE_SIZE, -10, 1.36);
        VLDR.W   S1,??setDefaultConfiguration_2+0x4  ;; 0x3fae147b
        VMOV.F32 S0,#-10.0
        MOVS     R2,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+6144
        ADDS     R1,R1,#+120
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+6144
        ADDS     R0,R0,#+56
          CFI FunCall _Z13setTableValuePfS_iff
        BL       _Z13setTableValuePfS_iff
//  250 	setTableValue(config->cltIdleCorrBins, config->cltIdleCorr, CLT_CURVE_SIZE, 0, 1.28);
        VLDR.W   S1,??setDefaultConfiguration_2+0x8  ;; 0x3fa3d70a
        VLDR.W   S0,??setDefaultConfiguration_2+0xC  ;; 0x0
        MOVS     R2,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+6144
        ADDS     R1,R1,#+120
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+6144
        ADDS     R0,R0,#+56
          CFI FunCall _Z13setTableValuePfS_iff
        BL       _Z13setTableValuePfS_iff
//  251 	setTableValue(config->cltIdleCorrBins, config->cltIdleCorr, CLT_CURVE_SIZE, 10, 1.19);
        VLDR.W   S1,??setDefaultConfiguration_2+0x10  ;; 0x3f9851ec
        VMOV.F32 S0,#10.0
        MOVS     R2,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+6144
        ADDS     R1,R1,#+120
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+6144
        ADDS     R0,R0,#+56
          CFI FunCall _Z13setTableValuePfS_iff
        BL       _Z13setTableValuePfS_iff
//  252 	setTableValue(config->cltIdleCorrBins, config->cltIdleCorr, CLT_CURVE_SIZE, 20, 1.12);
        VLDR.W   S1,??setDefaultConfiguration_2+0x14  ;; 0x3f8f5c29
        VMOV.F32 S0,#20.0
        MOVS     R2,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+6144
        ADDS     R1,R1,#+120
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+6144
        ADDS     R0,R0,#+56
          CFI FunCall _Z13setTableValuePfS_iff
        BL       _Z13setTableValuePfS_iff
//  253 	setTableValue(config->cltIdleCorrBins, config->cltIdleCorr, CLT_CURVE_SIZE, 30, 1.10);
        VLDR.W   S1,??setDefaultConfiguration_2+0x18  ;; 0x3f8ccccd
        VMOV.F32 S0,#30.0
        MOVS     R2,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+6144
        ADDS     R1,R1,#+120
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+6144
        ADDS     R0,R0,#+56
          CFI FunCall _Z13setTableValuePfS_iff
        BL       _Z13setTableValuePfS_iff
//  254 	setTableValue(config->cltIdleCorrBins, config->cltIdleCorr, CLT_CURVE_SIZE, 40, 1.06);
        VLDR.W   S1,??setDefaultConfiguration_2+0x1C  ;; 0x3f87ae14
        VLDR.W   S0,??setDefaultConfiguration_2+0x20  ;; 0x42200000
        MOVS     R2,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+6144
        ADDS     R1,R1,#+120
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+6144
        ADDS     R0,R0,#+56
          CFI FunCall _Z13setTableValuePfS_iff
        BL       _Z13setTableValuePfS_iff
//  255 	setTableValue(config->cltIdleCorrBins, config->cltIdleCorr, CLT_CURVE_SIZE, 50, 1.06);
        VLDR.W   S1,??setDefaultConfiguration_2+0x1C  ;; 0x3f87ae14
        VLDR.W   S0,??setDefaultConfiguration_3  ;; 0x42480000
        MOVS     R2,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+6144
        ADDS     R1,R1,#+120
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+6144
        ADDS     R0,R0,#+56
          CFI FunCall _Z13setTableValuePfS_iff
        BL       _Z13setTableValuePfS_iff
//  256 	setTableValue(config->cltIdleCorrBins, config->cltIdleCorr, CLT_CURVE_SIZE, 60, 1.03);
        VLDR.W   S1,??setDefaultConfiguration_2+0x24  ;; 0x3f83d70a
        VLDR.W   S0,??setDefaultConfiguration_2+0x28  ;; 0x42700000
        MOVS     R2,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+6144
        ADDS     R1,R1,#+120
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+6144
        ADDS     R0,R0,#+56
          CFI FunCall _Z13setTableValuePfS_iff
        BL       _Z13setTableValuePfS_iff
//  257 	setTableValue(config->cltIdleCorrBins, config->cltIdleCorr, CLT_CURVE_SIZE, 70, 1.01);
        VLDR.W   S1,??setDefaultConfiguration_2+0x2C  ;; 0x3f8147ae
        VLDR.W   S0,??setDefaultConfiguration_2+0x30  ;; 0x428c0000
        MOVS     R2,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+6144
        ADDS     R1,R1,#+120
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+6144
        ADDS     R0,R0,#+56
          CFI FunCall _Z13setTableValuePfS_iff
        BL       _Z13setTableValuePfS_iff
//  258 
//  259 
//  260 	setTableValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, -40, 1.5);
        VMOV.F32 S1,#1.5
        VLDR.W   S0,??setDefaultConfiguration_1  ;; 0xc2200000
        MOVS     R2,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+5632
        ADDS     R1,R1,#+248
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+5632
        ADDS     R0,R0,#+184
          CFI FunCall _Z13setTableValuePfS_iff
        BL       _Z13setTableValuePfS_iff
//  261 	setTableValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, -30, 1.5);
        VMOV.F32 S1,#1.5
        VMOV.F32 S0,#-30.0
        MOVS     R2,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+5632
        ADDS     R1,R1,#+248
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+5632
        ADDS     R0,R0,#+184
          CFI FunCall _Z13setTableValuePfS_iff
        BL       _Z13setTableValuePfS_iff
//  262 	setTableValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, -20, 1.42);
        VLDR.W   S1,??setDefaultConfiguration_2  ;; 0x3fb5c28f
        VMOV.F32 S0,#-20.0
        MOVS     R2,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+5632
        ADDS     R1,R1,#+248
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+5632
        ADDS     R0,R0,#+184
          CFI FunCall _Z13setTableValuePfS_iff
        BL       _Z13setTableValuePfS_iff
//  263 	setTableValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, -10, 1.36);
        VLDR.W   S1,??setDefaultConfiguration_2+0x4  ;; 0x3fae147b
        VMOV.F32 S0,#-10.0
        MOVS     R2,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+5632
        ADDS     R1,R1,#+248
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+5632
        ADDS     R0,R0,#+184
          CFI FunCall _Z13setTableValuePfS_iff
        BL       _Z13setTableValuePfS_iff
//  264 	setTableValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, 0, 1.28);
        VLDR.W   S1,??setDefaultConfiguration_2+0x8  ;; 0x3fa3d70a
        VLDR.W   S0,??setDefaultConfiguration_2+0xC  ;; 0x0
        MOVS     R2,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+5632
        ADDS     R1,R1,#+248
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+5632
        ADDS     R0,R0,#+184
          CFI FunCall _Z13setTableValuePfS_iff
        BL       _Z13setTableValuePfS_iff
//  265 	setTableValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, 10, 1.19);
        VLDR.W   S1,??setDefaultConfiguration_2+0x10  ;; 0x3f9851ec
        VMOV.F32 S0,#10.0
        MOVS     R2,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+5632
        ADDS     R1,R1,#+248
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+5632
        ADDS     R0,R0,#+184
          CFI FunCall _Z13setTableValuePfS_iff
        BL       _Z13setTableValuePfS_iff
//  266 	setTableValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, 20, 1.12);
        VLDR.W   S1,??setDefaultConfiguration_2+0x14  ;; 0x3f8f5c29
        VMOV.F32 S0,#20.0
        MOVS     R2,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+5632
        ADDS     R1,R1,#+248
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+5632
        ADDS     R0,R0,#+184
          CFI FunCall _Z13setTableValuePfS_iff
        BL       _Z13setTableValuePfS_iff
//  267 	setTableValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, 30, 1.10);
        VLDR.W   S1,??setDefaultConfiguration_2+0x18  ;; 0x3f8ccccd
        VMOV.F32 S0,#30.0
        MOVS     R2,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+5632
        ADDS     R1,R1,#+248
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+5632
        ADDS     R0,R0,#+184
          CFI FunCall _Z13setTableValuePfS_iff
        BL       _Z13setTableValuePfS_iff
//  268 	setTableValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, 40, 1.06);
        VLDR.W   S1,??setDefaultConfiguration_2+0x1C  ;; 0x3f87ae14
        VLDR.W   S0,??setDefaultConfiguration_2+0x20  ;; 0x42200000
        MOVS     R2,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+5632
        ADDS     R1,R1,#+248
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+5632
        ADDS     R0,R0,#+184
          CFI FunCall _Z13setTableValuePfS_iff
        BL       _Z13setTableValuePfS_iff
//  269 	setTableValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, 50, 1.06);
        VLDR.W   S1,??setDefaultConfiguration_2+0x1C  ;; 0x3f87ae14
        VLDR.W   S0,??setDefaultConfiguration_3  ;; 0x42480000
        MOVS     R2,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+5632
        ADDS     R1,R1,#+248
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+5632
        ADDS     R0,R0,#+184
          CFI FunCall _Z13setTableValuePfS_iff
        BL       _Z13setTableValuePfS_iff
//  270 	setTableValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, 60, 1.03);
        VLDR.W   S1,??setDefaultConfiguration_2+0x24  ;; 0x3f83d70a
        VLDR.W   S0,??setDefaultConfiguration_2+0x28  ;; 0x42700000
        MOVS     R2,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+5632
        ADDS     R1,R1,#+248
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+5632
        ADDS     R0,R0,#+184
          CFI FunCall _Z13setTableValuePfS_iff
        BL       _Z13setTableValuePfS_iff
//  271 	setTableValue(config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE, 70, 1.01);
        VLDR.W   S1,??setDefaultConfiguration_2+0x2C  ;; 0x3f8147ae
        VLDR.W   S0,??setDefaultConfiguration_2+0x30  ;; 0x428c0000
        B.N      ??setDefaultConfiguration_4
        Nop      
        DATA
??setDefaultConfiguration_1:
        DC32     0xc2200000
        THUMB
??setDefaultConfiguration_4:
        MOVS     R2,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+5632
        ADDS     R1,R1,#+248
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+5632
        ADDS     R0,R0,#+184
          CFI FunCall _Z13setTableValuePfS_iff
        BL       _Z13setTableValuePfS_iff
//  272 
//  273 	for (int i = 0; i < VBAT_INJECTOR_CURVE_SIZE; i++) {
        MOVS     R0,#+0
??setDefaultConfiguration_5:
        CMP      R0,#+8
        BGE.N    ??setDefaultConfiguration_6
//  274 		engineConfiguration->injector.battLagCorrBins[i] = 12 - VBAT_INJECTOR_CURVE_SIZE / 2 + i;
        ADDS     R1,R0,#+8
        VMOV     S0,R1
        VCVT.F32.S32 S0,S0
        LDR.W    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        ADDS     R1,R1,R0, LSL #+2
        VSTR     S0,[R1, #+16]
//  275 		engineConfiguration->injector.battLagCorr[i] = 0; // zero extra time by default
        LDR.W    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        ADDS     R1,R1,R0, LSL #+2
        MOVS     R2,#+0
        STR      R2,[R1, #+48]
//  276 	}
        ADDS     R0,R0,#+1
        B.N      ??setDefaultConfiguration_5
        DATA
??setDefaultConfiguration_2:
        DC32     0x3fb5c28f
        DC32     0x3fae147b
        DC32     0x3fa3d70a
        DC32     0x0
        DC32     0x3f9851ec
        DC32     0x3f8f5c29
        DC32     0x3f8ccccd
        DC32     0x3f87ae14
        DC32     0x42200000
        DC32     0x3f83d70a
        DC32     0x42700000
        DC32     0x3f8147ae
        DC32     0x428c0000
        THUMB
//  277 
//  278 	setConstantDwell(4 PASS_ENGINE_PARAMETER); // 4ms is global default dwell
??setDefaultConfiguration_6:
        VMOV.F32 S0,#4.0
          CFI FunCall _Z16setConstantDwellf
        BL       _Z16setConstantDwellf
//  279 	engineConfiguration->useConstantDwellDuringCranking = false;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        ANDS     R0,R0,#0xEF
        LDR.W    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1496]
//  280 
//  281 	setFuelLoadBin(1.2, 4.4 PASS_ENGINE_PARAMETER);
        VLDR.W   S1,??setDefaultConfiguration_7  ;; 0x408ccccd
        VLDR.W   S0,??setDefaultConfiguration_7+0x4  ;; 0x3f99999a
          CFI FunCall _Z14setFuelLoadBinff
        BL       _Z14setFuelLoadBinff
//  282 	setFuelRpmBin(800, 7000 PASS_ENGINE_PARAMETER);
        VLDR.W   S1,??setDefaultConfiguration_7+0x8  ;; 0x45dac000
        VLDR.W   S0,??setDefaultConfiguration_7+0xC  ;; 0x44480000
          CFI FunCall _Z13setFuelRpmBinff
        BL       _Z13setFuelRpmBinff
//  283 	setTimingLoadBin(1.2, 4.4 PASS_ENGINE_PARAMETER);
        VLDR.W   S1,??setDefaultConfiguration_7  ;; 0x408ccccd
        VLDR.W   S0,??setDefaultConfiguration_7+0x4  ;; 0x3f99999a
          CFI FunCall _Z16setTimingLoadBinff
        BL       _Z16setTimingLoadBinff
//  284 	setTimingRpmBin(800, 7000 PASS_ENGINE_PARAMETER);
        VLDR.W   S1,??setDefaultConfiguration_7+0x8  ;; 0x45dac000
        VLDR.W   S0,??setDefaultConfiguration_7+0xC  ;; 0x44480000
          CFI FunCall _Z15setTimingRpmBinff
        BL       _Z15setTimingRpmBinff
//  285 
//  286 	setTableBin2(engineConfiguration->map.samplingAngleBins, MAP_ANGLE_SIZE, 800, 7000, 1);
        VMOV.F32 S2,#1.0
        VLDR.W   S1,??setDefaultConfiguration_7+0x8  ;; 0x45dac000
        VLDR.W   S0,??setDefaultConfiguration_7+0xC  ;; 0x44480000
        MOVS     R1,#+8
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+112
          CFI FunCall _Z12setTableBin2Pfifff
        BL       _Z12setTableBin2Pfifff
//  287 	setTableBin2(engineConfiguration->map.samplingAngle, MAP_ANGLE_SIZE, 100, 130, 1);
        VMOV.F32 S2,#1.0
        VLDR.W   S1,??setDefaultConfiguration_8  ;; 0x43020000
        VLDR.W   S0,??setDefaultConfiguration_8+0x4  ;; 0x42c80000
        MOVS     R1,#+8
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+144
          CFI FunCall _Z12setTableBin2Pfifff
        BL       _Z12setTableBin2Pfifff
//  288 	setTableBin2(engineConfiguration->map.samplingWindowBins, MAP_ANGLE_SIZE, 800, 7000, 1);
        VMOV.F32 S2,#1.0
        VLDR.W   S1,??setDefaultConfiguration_7+0x8  ;; 0x45dac000
        VLDR.W   S0,??setDefaultConfiguration_7+0xC  ;; 0x44480000
        MOVS     R1,#+8
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+176
          CFI FunCall _Z12setTableBin2Pfifff
        BL       _Z12setTableBin2Pfifff
//  289 	setTableBin2(engineConfiguration->map.samplingWindow, MAP_ANGLE_SIZE, 50, 50, 1);
        VMOV.F32 S2,#1.0
        VLDR.W   S1,??setDefaultConfiguration_3  ;; 0x42480000
        VLDR.W   S0,??setDefaultConfiguration_3  ;; 0x42480000
        MOVS     R1,#+8
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+208
          CFI FunCall _Z12setTableBin2Pfifff
        BL       _Z12setTableBin2Pfifff
//  290 
//  291 	// set_whole_timing_map 3
//  292 	setWholeFuelMap(3 PASS_ENGINE_PARAMETER);
        VMOV.F32 S0,#3.0
          CFI FunCall _Z15setWholeFuelMapf
        BL       _Z15setWholeFuelMapf
//  293 	setMap(config->afrTable, 14.7);
        VLDR.W   S0,??setDefaultConfiguration_8+0x8  ;; 0x416b3333
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+14080
        ADDS     R0,R0,#+56
          CFI FunCall _Z6setMapPA16_ff
        BL       _Z6setMapPA16_ff
//  294 
//  295 	setDefaultVETable(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z17setDefaultVETablev
        BL       _Z17setDefaultVETablev
//  296 
//  297 	setMap(config->injectionPhase, -180);
        VLDR.W   S0,??setDefaultConfiguration_8+0xC  ;; 0xc3340000
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+9472
        ADDS     R0,R0,#+56
          CFI FunCall _Z6setMapPA16_ff
        BL       _Z6setMapPA16_ff
//  298 	setRpmTableBin(config->injPhaseRpmBins, FUEL_RPM_COUNT);
        MOVS     R1,#+16
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+10496
        ADDS     R0,R0,#+120
          CFI FunCall _Z14setRpmTableBinPfi
        BL       _Z14setRpmTableBinPfi
//  299 	setFuelTablesLoadBin(10, 160 PASS_ENGINE_PARAMETER);
        VLDR.W   S1,??setDefaultConfiguration_8+0x10  ;; 0x43200000
        B.N      ??setDefaultConfiguration_9
        DATA
??setDefaultConfiguration_3:
        DC32     0x42480000
        THUMB
??setDefaultConfiguration_9:
        VMOV.F32 S0,#10.0
          CFI FunCall _Z20setFuelTablesLoadBinff
        BL       _Z20setFuelTablesLoadBinff
//  300 	setDefaultIatTimingCorrection(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z29setDefaultIatTimingCorrectionv
        BL       _Z29setDefaultIatTimingCorrectionv
//  301 
//  302 	setThermistorConfiguration(&engineConfiguration->clt, 0, 9500, 23.8889, 2100, 48.8889, 1000);
        VLDR.W   S5,??setDefaultConfiguration_8+0x14  ;; 0x447a0000
        VLDR.W   S4,??setDefaultConfiguration_8+0x18  ;; 0x42438e3c
        VLDR.W   S3,??setDefaultConfiguration_8+0x1C  ;; 0x45034000
        VLDR.W   S2,??setDefaultConfiguration_8+0x20  ;; 0x41bf1c78
        VLDR.W   S1,??setDefaultConfiguration_8+0x24  ;; 0x46147000
        VLDR.W   S0,??setDefaultConfiguration_8+0x28  ;; 0x0
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+256
          CFI FunCall _Z26setThermistorConfigurationP14ThermistorConfffffff
        BL       _Z26setThermistorConfigurationP14ThermistorConfffffff
//  303 	engineConfiguration->clt.config.bias_resistor = 1500;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable7_2  ;; 0x44bb8000
        STR      R1,[R0, #+280]
//  304 
//  305 	setThermistorConfiguration(&engineConfiguration->iat, 32, 9500, 75, 2100, 120, 1000);
        VLDR.W   S5,??setDefaultConfiguration_8+0x14  ;; 0x447a0000
        VLDR.W   S4,??setDefaultConfiguration_8+0x2C  ;; 0x42f00000
        VLDR.W   S3,??setDefaultConfiguration_8+0x1C  ;; 0x45034000
        VLDR.W   S2,??setDefaultConfiguration_8+0x30  ;; 0x42960000
        VLDR.W   S1,??setDefaultConfiguration_8+0x24  ;; 0x46147000
        VLDR.W   S0,??setDefaultConfiguration_8+0x34  ;; 0x42000000
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+288
          CFI FunCall _Z26setThermistorConfigurationP14ThermistorConfffffff
        BL       _Z26setThermistorConfigurationP14ThermistorConfffffff
//  306 // todo: this value is way off! I am pretty sure temp coeffs are off also
//  307 	engineConfiguration->iat.config.bias_resistor = 2700;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable7_3  ;; 0x4528c000
        STR      R1,[R0, #+312]
//  308 
//  309 
//  310 	engineConfiguration->step1rpm = 3000;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVW     R1,#+3000
        STR      R1,[R0, #+320]
//  311 	engineConfiguration->step1timing = 10;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+10
        STR      R1,[R0, #+324]
//  312 
//  313 	engineConfiguration->rpmHardLimit = 7000;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVW     R1,#+7000
        STR      R1,[R0, #+420]
//  314 	engineConfiguration->cranking.rpm = 550;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVW     R1,#+550
        STRH     R1,[R0, #+96]
//  315 	engineConfiguration->cutFuelOnHardLimit = true;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1498]
        ORRS     R0,R0,#0x1
        LDR.W    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1498]
//  316 	engineConfiguration->cutSparkOnHardLimit = true;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1498]
        ORRS     R0,R0,#0x2
        LDR.W    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1498]
//  317 
//  318 	engineConfiguration->noAccelAfterHardLimitPeriodSecs = 3;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable7_4  ;; 0x40400000
        STR      R1,[R0, #+1644]
//  319 
//  320 	config->crankingFuelCoef[0] = 5; // base cranking fuel adjustment coefficient
        MOVW     R0,#+6072
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        LDR.W    R2,??DataTable9  ;; 0x40a00000
        STR      R2,[R0, R1]
//  321 	config->crankingFuelBins[0] = -20; // temperature in C
        MOVW     R0,#+6104
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        LDR.W    R2,??DataTable7_5  ;; 0xc1a00000
        STR      R2,[R0, R1]
//  322 	config->crankingFuelCoef[1] = 3.7;
        MOVW     R0,#+6076
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        LDR.W    R2,??DataTable7_6  ;; 0x406ccccd
        STR      R2,[R0, R1]
//  323 	config->crankingFuelBins[1] = -10;
        MOVW     R0,#+6108
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        LDR.W    R2,??DataTable7_7  ;; 0xc1200000
        STR      R2,[R0, R1]
//  324 	config->crankingFuelCoef[2] = 2.6;
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+6080
        LDR.W    R1,??DataTable7_8  ;; 0x40266666
        STR      R1,[R0, #+0]
//  325 	config->crankingFuelBins[2] = 5;
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+6112
        LDR.W    R1,??DataTable9  ;; 0x40a00000
        STR      R1,[R0, #+0]
//  326 	config->crankingFuelCoef[3] = 2.4;
        MOVW     R0,#+6084
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        LDR.W    R2,??DataTable8  ;; 0x4019999a
        STR      R2,[R0, R1]
//  327 	config->crankingFuelBins[3] = 20;
        MOVW     R0,#+6116
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        LDR.W    R2,??DataTable8_1  ;; 0x41a00000
        STR      R2,[R0, R1]
//  328 	config->crankingFuelCoef[4] = 2.1;
        MOVW     R0,#+6088
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        LDR.W    R2,??DataTable9_1  ;; 0x40066666
        STR      R2,[R0, R1]
//  329 	config->crankingFuelBins[4] = 35;
        MOVW     R0,#+6120
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        LDR.W    R2,??DataTable10  ;; 0x420c0000
        STR      R2,[R0, R1]
//  330 	config->crankingFuelCoef[5] = 1.8;
        MOVW     R0,#+6092
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        LDR.W    R2,??DataTable9_2  ;; 0x3fe66666
        STR      R2,[R0, R1]
//  331 	config->crankingFuelBins[5] = 50;
        MOVW     R0,#+6124
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        LDR.W    R2,??DataTable10_1  ;; 0x42480000
        STR      R2,[R0, R1]
//  332 	config->crankingFuelCoef[6] = 1.5;
        MOVW     R0,#+6096
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        MOVS     R2,#+1069547520
        STR      R2,[R0, R1]
//  333 	config->crankingFuelBins[6] = 65;
        MOVW     R0,#+6128
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        LDR.W    R2,??DataTable9_3  ;; 0x42820000
        STR      R2,[R0, R1]
//  334 	config->crankingFuelCoef[7] = 1;
        MOVW     R0,#+6100
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        MOVS     R2,#+1065353216
        STR      R2,[R0, R1]
//  335 	config->crankingFuelBins[7] = 90;
        MOVW     R0,#+6132
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        LDR.W    R2,??DataTable9_4  ;; 0x42b40000
        STR      R2,[R0, R1]
//  336 
//  337 	config->crankingCycleCoef[0] = 2;
        MOVW     R0,#+6136
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        MOVS     R2,#+1073741824
        STR      R2,[R0, R1]
//  338 	config->crankingCycleBins[0] = 10;
        MOVW     R0,#+6168
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        LDR.W    R2,??DataTable11  ;; 0x41200000
        STR      R2,[R0, R1]
//  339 	config->crankingCycleCoef[1] = 1;
        MOVW     R0,#+6140
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        MOVS     R2,#+1065353216
        STR      R2,[R0, R1]
//  340 	config->crankingCycleBins[1] = 31;
        MOVW     R0,#+6172
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        LDR.W    R2,??DataTable10_2  ;; 0x41f80000
        STR      R2,[R0, R1]
//  341 	config->crankingCycleCoef[2] = 1;
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+6144
        MOVS     R1,#+1065353216
        STR      R1,[R0, #+0]
//  342 	config->crankingCycleBins[2] = 52;
        LDR.W    R0,??setDefaultConfiguration_0+0xC
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+6176
        LDR.W    R1,??DataTable11_1  ;; 0x42500000
        STR      R1,[R0, #+0]
//  343 	config->crankingCycleCoef[3] = 0.5;
        MOVW     R0,#+6148
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        MOVS     R2,#+1056964608
        STR      R2,[R0, R1]
//  344 	config->crankingCycleBins[3] = 73;
        MOVW     R0,#+6180
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        LDR.W    R2,??DataTable11_2  ;; 0x42920000
        STR      R2,[R0, R1]
//  345 	config->crankingCycleCoef[4] = 0.5;
        MOVW     R0,#+6152
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        MOVS     R2,#+1056964608
        STR      R2,[R0, R1]
//  346 	config->crankingCycleBins[4] = 74;
        MOVW     R0,#+6184
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        LDR.W    R2,??DataTable11_3  ;; 0x42940000
        STR      R2,[R0, R1]
//  347 	config->crankingCycleCoef[5] = 0.5;
        MOVW     R0,#+6156
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        MOVS     R2,#+1056964608
        STR      R2,[R0, R1]
//  348 	config->crankingCycleBins[5] = 75;
        MOVW     R0,#+6188
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        LDR.N    R2,??setDefaultConfiguration_8+0x30  ;; 0x42960000
        STR      R2,[R0, R1]
//  349 	config->crankingCycleCoef[6] = 0.5;
        MOVW     R0,#+6160
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        MOVS     R2,#+1056964608
        STR      R2,[R0, R1]
//  350 	config->crankingCycleBins[6] = 76;
        MOVW     R0,#+6192
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        LDR.W    R2,??DataTable11_4  ;; 0x42980000
        B.N      ??setDefaultConfiguration_10
        Nop      
        DATA
??setDefaultConfiguration_7:
        DC32     0x408ccccd
        DC32     0x3f99999a
        DC32     0x45dac000
        DC32     0x44480000
        THUMB
??setDefaultConfiguration_10:
        STR      R2,[R0, R1]
//  351 	config->crankingCycleCoef[7] = 0.5;
        MOVW     R0,#+6164
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        MOVS     R2,#+1056964608
        STR      R2,[R0, R1]
//  352 	config->crankingCycleBins[7] = 77;
        MOVW     R0,#+6196
        LDR.W    R1,??setDefaultConfiguration_0+0xC
        LDR      R1,[R1, #+0]
        LDR.W    R2,??DataTable11_5  ;; 0x429a0000
        B.N      ??setDefaultConfiguration_11
        Nop      
        DATA
??setDefaultConfiguration_8:
        DC32     0x43020000
        DC32     0x42c80000
        DC32     0x416b3333
        DC32     0xc3340000
        DC32     0x43200000
        DC32     0x447a0000
        DC32     0x42438e3c
        DC32     0x45034000
        DC32     0x41bf1c78
        DC32     0x46147000
        DC32     0x0
        DC32     0x42f00000
        DC32     0x42960000
        DC32     0x42000000
        THUMB
??setDefaultConfiguration_11:
        STR      R2,[R0, R1]
//  353 
//  354 	engineConfiguration->cranking.baseFuel = 5;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable9  ;; 0x40a00000
        STR      R1,[R0, #+92]
//  355 
//  356 	engineConfiguration->analogInputDividerCoefficient = 2;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1073741824
        STR      R1,[R0, #+464]
//  357 
//  358 	engineConfiguration->crankingChargeAngle = 70;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable11_6  ;; 0x428c0000
        STR      R1,[R0, #+108]
//  359 	engineConfiguration->timingMode = TM_DYNAMIC;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+452]
//  360 	engineConfiguration->fixedModeTiming = 50;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable10_1  ;; 0x42480000
        STR      R1,[R0, #+456]
//  361 
//  362 	// performance optimization
//  363 	boardConfiguration->sensorChartMode = SC_OFF;
        LDR.W    R0,??setDefaultConfiguration_0+0x8
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+784]
//  364 
//  365 	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+4
        STR      R1,[R0, #+252]
//  366 	engineConfiguration->baroSensor.hwChannel = EFI_ADC_NONE;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+16
        STR      R1,[R0, #+620]
//  367 
//  368 	engineConfiguration->pedalPositionChannel = EFI_ADC_NONE;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+16
        STR      R1,[R0, #+604]
//  369 
//  370 	engineConfiguration->specs.firingOrder = FO_1_THEN_3_THEN_4_THEN2;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+408]
//  371 	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+428]
//  372 	engineConfiguration->injectionMode = IM_SEQUENTIAL;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+432]
//  373 
//  374 	engineConfiguration->ignitionMode = IM_ONE_COIL;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+444]
//  375 	engineConfiguration->globalTriggerAngleOffset = 0;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+460]
//  376 	engineConfiguration->injectionOffset = 0;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+436]
//  377 	engineConfiguration->ignitionOffset = 0;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+448]
//  378 	engineConfiguration->overrideCrankingIgnition = true;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+528]
//  379 	engineConfiguration->sensorChartFrequency = 20;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+20
        STR      R1,[R0, #+532]
//  380 
//  381 	engineConfiguration->algorithm = LM_PLAIN_MAF;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+424]
//  382 
//  383 	engineConfiguration->vbattDividerCoeff = ((float) (15 + 65)) / 15;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable11_7  ;; 0x40aaaaab
        STR      R1,[R0, #+468]
//  384 
//  385 	engineConfiguration->fanOnTemperature = 75;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable11_8  ;; 0x42960000
        STR      R1,[R0, #+476]
//  386 	engineConfiguration->fanOffTemperature = 70;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable11_6  ;; 0x428c0000
        STR      R1,[R0, #+480]
//  387 
//  388 	engineConfiguration->tpsMin = convertVoltageTo10bitADC(1.250);
        VMOV.F32 S0,#1.25
          CFI FunCall _Z24convertVoltageTo10bitADCf
        BL       _Z24convertVoltageTo10bitADCf
        LDR.W    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRH     R0,[R1, #+82]
//  389 	engineConfiguration->tpsMax = convertVoltageTo10bitADC(4.538);
        VLDR.W   S0,??setDefaultConfiguration_12  ;; 0x4091374c
          CFI FunCall _Z24convertVoltageTo10bitADCf
        BL       _Z24convertVoltageTo10bitADCf
        LDR.W    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRH     R0,[R1, #+84]
//  390 	engineConfiguration->tpsErrorLowValue = convertVoltageTo10bitADC(0.2);
        VLDR.W   S0,??setDefaultConfiguration_12+0x4  ;; 0x3e4ccccd
          CFI FunCall _Z24convertVoltageTo10bitADCf
        BL       _Z24convertVoltageTo10bitADCf
        LDR.W    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRH     R0,[R1, #+86]
//  391 	engineConfiguration->tpsErrorHighValue = convertVoltageTo10bitADC(6);
        VMOV.F32 S0,#6.0
          CFI FunCall _Z24convertVoltageTo10bitADCf
        BL       _Z24convertVoltageTo10bitADCf
        LDR.W    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRH     R0,[R1, #+88]
//  392 
//  393 	engineConfiguration->canNbcType = CAN_BUS_NBC_BMW;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+492]
//  394 	engineConfiguration->canSleepPeriod = 50;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+50
        STR      R1,[R0, #+496]
//  395 	engineConfiguration->canReadEnabled = true;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        ORRS     R0,R0,#0x1
        LDR.W    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1497]
//  396 	engineConfiguration->canWriteEnabled = false;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        ANDS     R0,R0,#0xFD
        LDR.W    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1497]
//  397 
//  398 	setOperationMode(engineConfiguration, FOUR_STROKE_CAM_SENSOR);
        MOVS     R1,#+2
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
          CFI FunCall _Z16setOperationModeP22engine_configuration_s16operation_mode_e
        BL       _Z16setOperationModeP22engine_configuration_s16operation_mode_e
//  399 	engineConfiguration->specs.cylindersCount = 4;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+4
        STR      R1,[R0, #+404]
//  400 	engineConfiguration->specs.displacement = 2;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1073741824
        STR      R1,[R0, #+400]
//  401 	/**
//  402 	 * By the way http://users.erols.com/srweiss/tableifc.htm has a LOT of data
//  403 	 */
//  404 	engineConfiguration->injector.flow = 200;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable11_10  ;; 0x43480000
        STR      R1,[R0, #+8]
//  405 
//  406 	engineConfiguration->displayMode = DM_HD44780;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+504]
//  407 
//  408 	engineConfiguration->logFormat = LF_NATIVE;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+508]
//  409 	engineConfiguration->directSelfStimulation = false;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+80]
        ANDS     R0,R0,#0xFE
        LDR.W    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+80]
//  410 
//  411 	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+8
        STR      R1,[R0, #+536]
//  412 
//  413 	engineConfiguration->HD44780width = 20;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+20
        STR      R1,[R0, #+516]
//  414 	engineConfiguration->HD44780height = 4;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+4
        STR      R1,[R0, #+520]
//  415 
//  416 	engineConfiguration->vRefAdcChannel = EFI_ADC_NONE;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+16
        STR      R1,[R0, #+1536]
//  417 	engineConfiguration->vbattAdcChannel = EFI_ADC_NONE;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+16
        STR      R1,[R0, #+472]
//  418 	engineConfiguration->clt.adcChannel = EFI_ADC_6;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+6
        STR      R1,[R0, #+284]
//  419 	engineConfiguration->iat.adcChannel = EFI_ADC_7;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+7
        STR      R1,[R0, #+316]
//  420 	engineConfiguration->mafAdcChannel = EFI_ADC_NONE;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+16
        STR      R1,[R0, #+580]
//  421 	engineConfiguration->afr.hwChannel = EFI_ADC_14;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+14
        STR      R1,[R0, #+584]
//  422 
//  423 	engineConfiguration->cylinderBore = 87.5;
        LDR.W    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable11_11  ;; 0x42af0000
        STR      R1,[R0, #+412]
//  424 
//  425 	setEgoSensor(ES_14Point7_Free PASS_ENGINE_PARAMETER);
        MOVS     R0,#+2
          CFI FunCall _Z12setEgoSensor12ego_sensor_e
        BL       _Z12setEgoSensor12ego_sensor_e
//  426 
//  427 	engineConfiguration->globalFuelCorrection = 1;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1065353216
        STR      R1,[R0, #+568]
//  428 
//  429 	engineConfiguration->map.sensor.type = MT_MPX4250;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+248]
//  430 
//  431 	engineConfiguration->baroSensor.type = MT_CUSTOM;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+616]
//  432 	engineConfiguration->baroSensor.valueAt0 = 0;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+608]
//  433 	engineConfiguration->baroSensor.valueAt5 = 500;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable11_12  ;; 0x43fa0000
        STR      R1,[R0, #+612]
//  434 
//  435 	engineConfiguration->hasMapSensor = true;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        ORRS     R0,R0,#0x1
        LDR.N    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1496]
//  436 	engineConfiguration->hasMafSensor = true;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        ORRS     R0,R0,#0x40
        LDR.N    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1497]
//  437 	engineConfiguration->hasTpsSensor = true;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        ORRS     R0,R0,#0x80
        LDR.N    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1497]
//  438 	engineConfiguration->hasAfrSensor = true;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        ORRS     R0,R0,#0x8
        LDR.N    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1496]
//  439 	engineConfiguration->hasIatSensor = true;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        ORRS     R0,R0,#0x2
        LDR.N    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1496]
//  440 	engineConfiguration->hasCltSensor = true;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        ORRS     R0,R0,#0x80
        LDR.N    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1496]
//  441 	engineConfiguration->hasBaroSensor = false;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        ANDS     R0,R0,#0xFB
        LDR.N    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1496]
//  442 	engineConfiguration->isEngineChartEnabled = true;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        ORRS     R0,R0,#0x20
        LDR.N    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1496]
//  443 
//  444 	engineConfiguration->useOnlyFrontForTrigger = false;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        ANDS     R0,R0,#0xBF
        LDR.N    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1509]
//  445 
//  446 	boardConfiguration->idle.solenoidFrequency = 200;
        LDR.N    R0,??setDefaultConfiguration_0+0x8
        LDR      R0,[R0, #+0]
        MOVS     R1,#+200
        STR      R1,[R0, #+8]
//  447 	// set_idle_position 50
//  448 	boardConfiguration->idlePosition = 50;
        LDR.N    R0,??setDefaultConfiguration_0+0x8
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable10_1  ;; 0x42480000
        STR      R1,[R0, #+20]
//  449 	engineConfiguration->targetIdleRpm = 1200;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOV      R1,#+1200
        STR      R1,[R0, #+1516]
//  450 //	engineConfiguration->idleMode = IM_AUTO;
//  451 	engineConfiguration->idleMode = IM_MANUAL;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+1504]
//  452 
//  453 	engineConfiguration->acSwitchAdc = EFI_ADC_NONE;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+16
        STR      R1,[R0, #+1520]
//  454 
//  455 	engineConfiguration->externalKnockSenseAdc = EFI_ADC_NONE;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+16
        STR      R1,[R0, #+1620]
//  456 
//  457 	boardConfiguration->useSerialPort = true;
        LDR.N    R0,??setDefaultConfiguration_0+0x8
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+385]
        ORRS     R0,R0,#0x1
        LDR.N    R1,??setDefaultConfiguration_0+0x8
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+385]
//  458 	boardConfiguration->useStepperIdle = false;
        LDR.N    R0,??setDefaultConfiguration_0+0x8
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+385]
        ANDS     R0,R0,#0xFD
        LDR.N    R1,??setDefaultConfiguration_0+0x8
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+385]
//  459 
//  460 	boardConfiguration->idle.stepperDirectionPin = GPIOE_10;
        LDR.N    R0,??setDefaultConfiguration_0+0x8
        LDR      R0,[R0, #+0]
        MOVS     R1,#+74
        STR      R1,[R0, #+12]
//  461 	boardConfiguration->idle.stepperStepPin = GPIOE_12;
        LDR.N    R0,??setDefaultConfiguration_0+0x8
        LDR      R0,[R0, #+0]
        MOVS     R1,#+76
        STR      R1,[R0, #+16]
//  462 	engineConfiguration->stepperEnablePin = GPIOE_14;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+78
        STR      R1,[R0, #+1640]
//  463 	engineConfiguration->idleStepperReactionTime = 10;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable11  ;; 0x41200000
        B.N      ??setDefaultConfiguration_13
        DATA
??setDefaultConfiguration_12:
        DC32     0x4091374c
        DC32     0x3e4ccccd
        THUMB
??setDefaultConfiguration_13:
        STR      R1,[R0, #+1548]
//  464 	engineConfiguration->idleStepperTotalSteps = 150;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+150
        STR      R1,[R0, #+1636]
//  465 
//  466 #if EFI_PROD_CODE
//  467 	engineConfiguration->engineChartSize = 300;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOV      R1,#+300
        STR      R1,[R0, #+1512]
//  468 #else
//  469 	// need more events for automated test
//  470 	engineConfiguration->engineChartSize = 400;
//  471 #endif
//  472 
//  473 	engineConfiguration->engineCycle = 720;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOV      R1,#+720
        STR      R1,[R0, #+416]
//  474 
//  475 	engineConfiguration->primingSquirtDurationMs = 5;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable9  ;; 0x40a00000
        STR      R1,[R0, #+100]
//  476 
//  477 	engineConfiguration->isInjectionEnabled = true;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        ORRS     R0,R0,#0x1
        LDR.N    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1508]
//  478 	engineConfiguration->isIgnitionEnabled = true;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        ORRS     R0,R0,#0x2
        LDR.N    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1508]
//  479 	engineConfiguration->isCylinderCleanupEnabled = true;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        ORRS     R0,R0,#0x4
        LDR.N    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1508]
//  480 	engineConfiguration->secondTriggerChannelEnabled = true;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        ORRS     R0,R0,#0x8
        LDR.N    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1508]
//  481 
//  482 	engineConfiguration->isMapAveragingEnabled = true;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        ORRS     R0,R0,#0x20
        LDR.N    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1508]
//  483 	engineConfiguration->isMilEnabled = true;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        ORRS     R0,R0,#0x40
        LDR.N    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1508]
//  484 	engineConfiguration->isFuelPumpEnabled = true;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        ORRS     R0,R0,#0x80
        LDR.N    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1508]
//  485 	engineConfiguration->isTunerStudioEnabled = true;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        ORRS     R0,R0,#0x1
        LDR.N    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1509]
//  486 	engineConfiguration->isWaveAnalyzerEnabled = true;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        ORRS     R0,R0,#0x2
        LDR.N    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1509]
//  487 	engineConfiguration->isIdleThreadEnabled = true;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        ORRS     R0,R0,#0x4
        LDR.N    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1509]
//  488 
//  489 	engineConfiguration->acIdleRpmBump = 200;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+200
        STRH     R1,[R0, #+1532]
//  490 	engineConfiguration->knockDetectionWindowStart = 35;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable10  ;; 0x420c0000
        STR      R1,[R0, #+1540]
//  491 	engineConfiguration->knockDetectionWindowEnd = 135;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable11_13  ;; 0x43070000
        STR      R1,[R0, #+1544]
//  492 
//  493 	engineConfiguration->fuelLevelSensor = EFI_ADC_NONE;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+16
        STR      R1,[R0, #+484]
//  494 	boardConfiguration->fuelLevelEmptyTankVoltage = 0;
        LDR.N    R0,??setDefaultConfiguration_0+0x8
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+268]
//  495 	boardConfiguration->fuelLevelFullTankVoltage = 5;
        LDR.N    R0,??setDefaultConfiguration_0+0x8
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable9  ;; 0x40a00000
        STR      R1,[R0, #+272]
//  496 
//  497 	engineConfiguration->hipOutputChannel = EFI_ADC_NONE;
        LDR.N    R0,??setDefaultConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+16
        STR      R1,[R0, #+1500]
//  498 
//  499 	/**
//  500 	 * this is RPM. 10000 rpm is only 166Hz, 800 rpm is 13Hz
//  501 	 */
//  502 	boardConfiguration->triggerSimulatorFrequency = 1200;
        LDR.N    R0,??setDefaultConfiguration_0+0x8
        LDR      R0,[R0, #+0]
        MOV      R1,#+1200
        STR      R1,[R0, #+208]
//  503 
//  504 	boardConfiguration->triggerErrorPin = GPIO_UNASSIGNED;
        LDR.N    R0,??setDefaultConfiguration_0+0x8
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+600]
//  505 
//  506 	boardConfiguration->max31855spiDevice = SPI_NONE;
        LDR.N    R0,??setDefaultConfiguration_0+0x8
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+448]
//  507 	for (int i = 0; i < EGT_CHANNEL_COUNT; i++) {
        MOVS     R0,#+0
??setDefaultConfiguration_14:
        CMP      R0,#+8
        BGE.N    ??setDefaultConfiguration_15
//  508 		boardConfiguration->max31855_cs[i] = GPIO_UNASSIGNED;
        LDR.N    R1,??setDefaultConfiguration_0+0x8
        LDR      R1,[R1, #+0]
        ADDS     R1,R1,R0, LSL #+2
        MOVS     R2,#+80
        STR      R2,[R1, #+416]
//  509 	}
        ADDS     R0,R0,#+1
        B.N      ??setDefaultConfiguration_14
//  510 	for (int i = 0; i < LE_COMMAND_COUNT; i++) {
??setDefaultConfiguration_15:
        MOVS     R0,#+0
??setDefaultConfiguration_16:
        CMP      R0,#+16
        BGE.N    ??setDefaultConfiguration_17
//  511 		boardConfiguration->fsioPins[i] = GPIO_UNASSIGNED;
        LDR.N    R1,??setDefaultConfiguration_0+0x8
        LDR      R1,[R1, #+0]
        ADDS     R1,R1,R0, LSL #+2
        MOVS     R2,#+80
        STR      R2,[R1, #+452]
//  512 		config->le_formulas[i][0] = 0;
        MOVS     R1,#+200
        LDR.N    R2,??setDefaultConfiguration_0+0xC
        LDR      R2,[R2, #+0]
        MLA      R1,R1,R0,R2
        MOVS     R2,#+0
        STRB     R2,[R1, #+2216]
//  513 		boardConfiguration->fsioDigitalInputs[i] = GPIO_UNASSIGNED;
        LDR.N    R1,??setDefaultConfiguration_0+0x8
        LDR      R1,[R1, #+0]
        ADDS     R1,R1,R0, LSL #+2
        MOVS     R2,#+80
        STR      R2,[R1, #+804]
//  514 		engineConfiguration->fsioInputModes[i] = PI_DEFAULT;
        LDR.N    R1,??setDefaultConfiguration_0+0x4
        LDR      R1,[R1, #+0]
        ADDS     R1,R1,R0, LSL #+2
        MOVS     R2,#+0
        STR      R2,[R1, #+1556]
//  515 	}
        ADDS     R0,R0,#+1
        B.N      ??setDefaultConfiguration_16
        DATA
??setDefaultConfiguration_0:
        DC32     persistentState+0x8
        DC32     engineConfiguration
        DC32     boardConfiguration
        DC32     config
        THUMB
//  516 
//  517 	boardConfiguration->triggerSimulatorPins[0] = GPIOD_1;
??setDefaultConfiguration_17:
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+49
        STR      R1,[R0, #+352]
//  518 	boardConfiguration->triggerSimulatorPins[1] = GPIOD_2;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+50
        STR      R1,[R0, #+356]
//  519 	boardConfiguration->triggerSimulatorPins[2] = GPIO_UNASSIGNED;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+360]
//  520 
//  521 	boardConfiguration->triggerSimulatorPinModes[0] = OM_DEFAULT;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+364]
//  522 	boardConfiguration->triggerSimulatorPinModes[1] = OM_DEFAULT;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+368]
//  523 	boardConfiguration->triggerSimulatorPinModes[2] = OM_DEFAULT;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+372]
//  524 
//  525 	setFrankenstein_01_LCD(boardConfiguration);
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
          CFI FunCall _Z22setFrankenstein_01_LCDP21board_configuration_s
        BL       _Z22setFrankenstein_01_LCDP21board_configuration_s
//  526 
//  527 	strcpy(config->timingMultiplier, "1");
        ADR.N    R1,??DataTable7  ;; "1"
        LDR.W    R0,??DataTable11_15
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+5376
        ADDS     R0,R0,#+40
          CFI FunCall strcpy
        BL       strcpy
//  528 	strcpy(config->timingAdditive, "0");
        ADR.N    R1,??DataTable7_1  ;; "0"
        LDR.W    R0,??DataTable11_15
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+5376
        ADDS     R0,R0,#+240
          CFI FunCall strcpy
        BL       strcpy
//  529 
//  530 	engineConfiguration->hip9011SpiDevice = SPI_DEVICE_2;
        LDR.N    R0,??DataTable7_9
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+564]
//  531 
//  532 	engineConfiguration->isAlternatorControlEnabled = true;
        LDR.N    R0,??DataTable7_9
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        ORRS     R0,R0,#0x20
        LDR.N    R1,??DataTable7_9
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1497]
//  533 
//  534 	engineConfiguration->isGpsEnabled = false;
        LDR.N    R0,??DataTable7_9
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        ANDS     R0,R0,#0xEF
        LDR.N    R1,??DataTable7_9
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1497]
//  535 	boardConfiguration->gps_rx_pin = GPIOB_7;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+23
        STR      R1,[R0, #+200]
//  536 	boardConfiguration->gps_tx_pin = GPIOB_6;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+22
        STR      R1,[R0, #+204]
//  537 
//  538 	boardConfiguration->triggerInputPins[0] = GPIOC_6;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+38
        STR      R1,[R0, #+296]
//  539 	boardConfiguration->triggerInputPins[1] = GPIOA_5;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+5
        STR      R1,[R0, #+300]
//  540 	boardConfiguration->logicAnalyzerPins[0] = GPIO_UNASSIGNED;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+388]
//  541 	boardConfiguration->logicAnalyzerPins[1] = GPIO_UNASSIGNED; // GPIOE_5 is a popular option (if available)
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+392]
//  542 	boardConfiguration->logicAnalyzerPins[2] = GPIO_UNASSIGNED;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+396]
//  543 	boardConfiguration->logicAnalyzerPins[3] = GPIO_UNASSIGNED;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+400]
//  544 	boardConfiguration->vehicleSpeedSensorInputPin = GPIO_UNASSIGNED;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+792]
//  545 	engineConfiguration->vehicleSpeedCoef = 1.0f;
        LDR.N    R0,??DataTable7_9
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1065353216
        STR      R1,[R0, #+488]
//  546 
//  547 	boardConfiguration->logicAnalyzerMode[0] = false;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STRB     R1,[R0, #+404]
//  548 	boardConfiguration->logicAnalyzerMode[1] = false;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STRB     R1,[R0, #+405]
//  549 
//  550 	boardConfiguration->idleThreadPeriod = 100;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+100
        STR      R1,[R0, #+312]
//  551 	boardConfiguration->consoleLoopPeriod = 200;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+200
        STR      R1,[R0, #+316]
//  552 	boardConfiguration->lcdThreadPeriod = 300;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOV      R1,#+300
        STR      R1,[R0, #+320]
//  553 	boardConfiguration->generalPeriodicThreadPeriod = 50;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+50
        STR      R1,[R0, #+328]
//  554 
//  555 	boardConfiguration->boardTestModeJumperPin = GPIOB_0;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+16
        STR      R1,[R0, #+336]
//  556 
//  557 	boardConfiguration->canDeviceMode = CD_USE_CAN2;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+340]
//  558 	boardConfiguration->canTxPin = GPIOB_6;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+22
        STR      R1,[R0, #+344]
//  559 	boardConfiguration->canRxPin = GPIOB_12;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+28
        STR      R1,[R0, #+348]
//  560 
//  561 	// set this to SPI_DEVICE_3 to enable stimulation
//  562 	boardConfiguration->digitalPotentiometerSpiDevice = SPI_NONE;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+212]
//  563 	boardConfiguration->digitalPotentiometerChipSelect[0] = GPIOD_7;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+55
        STR      R1,[R0, #+216]
//  564 	boardConfiguration->digitalPotentiometerChipSelect[1] = GPIO_UNASSIGNED;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+220]
//  565 	boardConfiguration->digitalPotentiometerChipSelect[2] = GPIOD_5;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+53
        STR      R1,[R0, #+224]
//  566 	boardConfiguration->digitalPotentiometerChipSelect[3] = GPIO_UNASSIGNED;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+228]
//  567 
//  568 	boardConfiguration->is_enabled_spi_1 = false;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        ANDS     R0,R0,#0xFE
        LDR.W    R1,??DataTable11_14
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+384]
//  569 	boardConfiguration->is_enabled_spi_2 = false;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        ANDS     R0,R0,#0xFD
        LDR.W    R1,??DataTable11_14
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+384]
//  570 	boardConfiguration->is_enabled_spi_3 = true;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        ORRS     R0,R0,#0x4
        LDR.W    R1,??DataTable11_14
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+384]
//  571 
//  572 	boardConfiguration->spi1mosiPin = GPIOB_5;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+21
        STR      R1,[R0, #+724]
//  573 	boardConfiguration->spi1misoPin = GPIOB_4;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+20
        STR      R1,[R0, #+728]
//  574 	boardConfiguration->spi1sckPin = GPIOB_3;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+19
        STR      R1,[R0, #+732]
//  575 
//  576 	boardConfiguration->spi2mosiPin = GPIOB_15;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+31
        STR      R1,[R0, #+736]
//  577 	boardConfiguration->spi2misoPin = GPIOB_14;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+30
        STR      R1,[R0, #+740]
//  578 	boardConfiguration->spi2sckPin = GPIOB_13;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+29
        STR      R1,[R0, #+744]
//  579 
//  580 	boardConfiguration->spi3mosiPin = GPIOB_5;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+21
        STR      R1,[R0, #+748]
//  581 	boardConfiguration->spi3misoPin = GPIOB_4;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+20
        STR      R1,[R0, #+752]
//  582 	boardConfiguration->spi3sckPin = GPIOB_3;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+19
        STR      R1,[R0, #+756]
//  583 
//  584 	boardConfiguration->hip9011Gain = 1;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1065353216
        STR      R1,[R0, #+760]
//  585 
//  586 	boardConfiguration->sdCardCsPin = GPIOD_4;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOVS     R1,#+52
        STR      R1,[R0, #+324]
//  587 	boardConfiguration->isSdCardEnabled = true;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        ORRS     R0,R0,#0x8
        LDR.W    R1,??DataTable11_14
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+384]
//  588 	boardConfiguration->isFastAdcEnabled = true;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        ORRS     R0,R0,#0x10
        LDR.W    R1,??DataTable11_14
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+384]
//  589 	boardConfiguration->isEngineControlEnabled = true;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        ORRS     R0,R0,#0x20
        LDR.W    R1,??DataTable11_14
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+384]
//  590 
//  591 	boardConfiguration->isVerboseAlternator = false;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        ANDS     R0,R0,#0x7F
        LDR.W    R1,??DataTable11_14
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+384]
//  592 
//  593 	boardConfiguration->tunerStudioSerialSpeed = TS_DEFAULT_SPEED;
        LDR.W    R0,??DataTable11_14
        LDR      R0,[R0, #+0]
        MOV      R1,#+38400
        STR      R1,[R0, #+332]
//  594 
//  595 	engineConfiguration->mapAccelLength = 6;
        LDR.N    R0,??DataTable7_9
        LDR      R0,[R0, #+0]
        MOVS     R1,#+6
        STR      R1,[R0, #+2196]
//  596 	engineConfiguration->mapAccelEnrichmentThreshold = 5; // kPa
        LDR.N    R0,??DataTable7_9
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable9  ;; 0x40a00000
        STR      R1,[R0, #+2208]
//  597 	engineConfiguration->mapAccelEnrichmentMultiplier = 2;
        LDR.N    R0,??DataTable7_9
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1073741824
        STR      R1,[R0, #+2212]
//  598 
//  599 	engineConfiguration->tpsAccelLength = 12;
        LDR.N    R0,??DataTable7_9
        LDR      R0,[R0, #+0]
        MOVS     R1,#+12
        STR      R1,[R0, #+2184]
//  600 	engineConfiguration->tpsAccelEnrichmentThreshold = 40; // TPS % change, per engine cycle
        LDR.N    R0,??DataTable7_9
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable11_16  ;; 0x42200000
        STR      R1,[R0, #+2188]
//  601 	engineConfiguration->tpsAccelEnrichmentMultiplier = 0.05;
        LDR.N    R0,??DataTable7_9
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable11_17  ;; 0x3d4ccccd
        STR      R1,[R0, #+2192]
//  602 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock12

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7:
        DC8      "1",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_1:
        DC8      "0",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_2:
        DC32     0x44bb8000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_3:
        DC32     0x4528c000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_4:
        DC32     0x40400000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_5:
        DC32     0xc1a00000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_6:
        DC32     0x406ccccd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_7:
        DC32     0xc1200000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_8:
        DC32     0x40266666

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_9:
        DC32     engineConfiguration
//  603 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z21resetConfigurationExtP7Logging13engine_type_e
        THUMB
//  604 void resetConfigurationExt(Logging * logger, engine_type_e engineType DECLARE_ENGINE_PARAMETER_S) {
_Z21resetConfigurationExtP7Logging13engine_type_e:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  605 	/**
//  606 	 * Let's apply global defaults first
//  607 	 */
//  608 	setDefaultConfiguration(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z23setDefaultConfigurationv
        BL       _Z23setDefaultConfigurationv
//  609 #if EFI_SIMULATOR
//  610 	engineConfiguration->directSelfStimulation = true;
//  611 #endif /* */
//  612 	engineConfiguration->engineType = engineType;
        LDR.N    R0,??DataTable11_18
        LDR      R0,[R0, #+0]
        STR      R5,[R0, #+0]
//  613 	engineConfiguration->headerMagicValue = HEADER_MAGIC_NUMBER;
        LDR.N    R0,??DataTable11_18
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable11_19  ;; 0x1221239
        STR      R1,[R0, #+4]
//  614 
//  615 	/**
//  616 	 * And override them with engine-specific defaults
//  617 	 */
//  618 	switch (engineType) {
        MOVS     R0,R5
        CMP      R0,#+0
        BEQ.N    ??resetConfigurationExt_0
        CMP      R0,#+2
        BEQ.N    ??resetConfigurationExt_1
        CMP      R0,#+3
        BEQ.N    ??resetConfigurationExt_2
        CMP      R0,#+4
        BEQ.N    ??resetConfigurationExt_3
        CMP      R0,#+5
        BEQ.N    ??resetConfigurationExt_4
        CMP      R0,#+6
        BEQ.N    ??resetConfigurationExt_5
        CMP      R0,#+7
        BEQ.N    ??resetConfigurationExt_6
        CMP      R0,#+8
        BEQ.N    ??resetConfigurationExt_7
        CMP      R0,#+9
        BEQ.N    ??resetConfigurationExt_8
        CMP      R0,#+10
        BEQ.W    ??resetConfigurationExt_9
        CMP      R0,#+11
        BEQ.N    ??resetConfigurationExt_10
        CMP      R0,#+12
        BEQ.N    ??resetConfigurationExt_11
        CMP      R0,#+13
        BEQ.N    ??resetConfigurationExt_12
        CMP      R0,#+14
        BEQ.N    ??resetConfigurationExt_13
        CMP      R0,#+15
        BEQ.N    ??resetConfigurationExt_14
        CMP      R0,#+16
        BEQ.N    ??resetConfigurationExt_15
        CMP      R0,#+17
        BEQ.N    ??resetConfigurationExt_16
        CMP      R0,#+18
        BEQ.N    ??resetConfigurationExt_17
        CMP      R0,#+19
        BEQ.N    ??resetConfigurationExt_18
        CMP      R0,#+20
        BEQ.N    ??resetConfigurationExt_19
        CMP      R0,#+21
        BEQ.N    ??resetConfigurationExt_20
        CMP      R0,#+22
        BEQ.N    ??resetConfigurationExt_21
        CMP      R0,#+23
        BEQ.N    ??resetConfigurationExt_22
        CMP      R0,#+24
        BEQ.N    ??resetConfigurationExt_23
        CMP      R0,#+25
        BEQ.N    ??resetConfigurationExt_24
        CMP      R0,#+26
        BEQ.N    ??resetConfigurationExt_25
        CMP      R0,#+27
        BEQ.N    ??resetConfigurationExt_26
        CMP      R0,#+28
        BEQ.N    ??resetConfigurationExt_27
        CMP      R0,#+29
        BEQ.N    ??resetConfigurationExt_28
        CMP      R0,#+30
        BEQ.N    ??resetConfigurationExt_29
        CMP      R0,#+31
        BEQ.N    ??resetConfigurationExt_30
        CMP      R0,#+32
        BEQ.N    ??resetConfigurationExt_31
        B.N      ??resetConfigurationExt_32
//  619 	case CUSTOM_ENGINE:
//  620 		setCustomEngineConfiguration(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_0:
          CFI FunCall _Z28setCustomEngineConfigurationv
        BL       _Z28setCustomEngineConfigurationv
//  621 		break;
        B.N      ??resetConfigurationExt_33
//  622 	case ACURA_RSX:
//  623 		setAcuraRSX(engineConfiguration);
??resetConfigurationExt_26:
        LDR.N    R0,??DataTable11_18
        LDR      R0,[R0, #+0]
          CFI FunCall _Z11setAcuraRSXP22engine_configuration_s
        BL       _Z11setAcuraRSXP22engine_configuration_s
//  624 		break;
        B.N      ??resetConfigurationExt_33
//  625 #if EFI_SUPPORT_DODGE_NEON || defined(__DOXYGEN__)
//  626 	case DODGE_NEON_1995:
//  627 		setDodgeNeon1995EngineConfiguration(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_1:
          CFI FunCall _Z35setDodgeNeon1995EngineConfigurationv
        BL       _Z35setDodgeNeon1995EngineConfigurationv
//  628 		break;
        B.N      ??resetConfigurationExt_33
//  629 	case DODGE_NEON_2003:
//  630 		setDodgeNeonNGCEngineConfiguration(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_22:
          CFI FunCall _Z34setDodgeNeonNGCEngineConfigurationv
        BL       _Z34setDodgeNeonNGCEngineConfigurationv
//  631 		break;
        B.N      ??resetConfigurationExt_33
//  632 #endif /* EFI_SUPPORT_DODGE_NEON */
//  633 #if EFI_SUPPORT_FORD_ASPIRE || defined(__DOXYGEN__)
//  634 	case FORD_ASPIRE_1996:
//  635 		setFordAspireEngineConfiguration(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_2:
          CFI FunCall _Z32setFordAspireEngineConfigurationv
        BL       _Z32setFordAspireEngineConfigurationv
//  636 		break;
        B.N      ??resetConfigurationExt_33
//  637 #endif /* EFI_SUPPORT_FORD_ASPIRE */
//  638 #if EFI_SUPPORT_FORD_FIESTA || defined(__DOXYGEN__)
//  639 	case FORD_FIESTA:
//  640 		setFordFiestaDefaultEngineConfiguration(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_3:
          CFI FunCall _Z39setFordFiestaDefaultEngineConfigurationv
        BL       _Z39setFordFiestaDefaultEngineConfigurationv
//  641 		break;
        B.N      ??resetConfigurationExt_33
//  642 #endif /* EFI_SUPPORT_FORD_FIESTA */
//  643 #if EFI_SUPPORT_NISSAN_PRIMERA || defined(__DOXYGEN__)
//  644 	case NISSAN_PRIMERA:
//  645 		setNissanPrimeraEngineConfiguration(engineConfiguration);
??resetConfigurationExt_4:
        LDR.N    R0,??DataTable11_18
        LDR      R0,[R0, #+0]
          CFI FunCall _Z35setNissanPrimeraEngineConfigurationP22engine_configuration_s
        BL       _Z35setNissanPrimeraEngineConfigurationP22engine_configuration_s
//  646 		break;
        B.N      ??resetConfigurationExt_33
//  647 #endif
//  648 	case HONDA_ACCORD_CD:
//  649 		setHondaAccordConfigurationThreeWires(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_5:
          CFI FunCall _Z37setHondaAccordConfigurationThreeWiresv
        BL       _Z37setHondaAccordConfigurationThreeWiresv
//  650 		break;
        B.N      ??resetConfigurationExt_33
//  651 	case HONDA_ACCORD_CD_TWO_WIRES:
//  652 		setHondaAccordConfiguration1_24(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_16:
          CFI FunCall _Z31setHondaAccordConfiguration1_24v
        BL       _Z31setHondaAccordConfiguration1_24v
//  653 		break;
        B.N      ??resetConfigurationExt_33
//  654 	case HONDA_ACCORD_CD_DIP:
//  655 		setHondaAccordConfigurationDip(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_17:
          CFI FunCall _Z30setHondaAccordConfigurationDipv
        BL       _Z30setHondaAccordConfigurationDipv
//  656 		break;
        B.N      ??resetConfigurationExt_33
//  657 	case MITSU_4G93:
//  658 		setMitsubishiConfiguration(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_15:
          CFI FunCall _Z26setMitsubishiConfigurationv
        BL       _Z26setMitsubishiConfigurationv
//  659 		break;
        B.N      ??resetConfigurationExt_33
//  660 #if EFI_SUPPORT_1995_FORD_INLINE_6 || defined(__DOXYGEN__)
//  661 	case FORD_INLINE_6_1995:
//  662 		setFordInline6(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_6:
          CFI FunCall _Z14setFordInline6v
        BL       _Z14setFordInline6v
//  663 		break;
        B.N      ??resetConfigurationExt_33
//  664 #endif /* EFI_SUPPORT_1995_FORD_INLINE_6 */
//  665 	case GY6_139QMB:
//  666 		setGy6139qmbDefaultEngineConfiguration(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_7:
          CFI FunCall _Z38setGy6139qmbDefaultEngineConfigurationv
        BL       _Z38setGy6139qmbDefaultEngineConfigurationv
//  667 		break;
        B.N      ??resetConfigurationExt_33
//  668 	case MAZDA_MIATA_NB:
//  669 		setMazdaMiataNbEngineConfiguration(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_8:
          CFI FunCall _Z34setMazdaMiataNbEngineConfigurationv
        BL       _Z34setMazdaMiataNbEngineConfigurationv
//  670 		break;
        B.N      ??resetConfigurationExt_33
//  671 	case MAZDA_323:
//  672 		setMazda323EngineConfiguration(engineConfiguration);
??resetConfigurationExt_10:
        LDR.N    R0,??DataTable11_18
        LDR      R0,[R0, #+0]
          CFI FunCall _Z30setMazda323EngineConfigurationP22engine_configuration_s
        BL       _Z30setMazda323EngineConfigurationP22engine_configuration_s
//  673 		break;
        B.N      ??resetConfigurationExt_33
//  674 	case MAZDA_626:
//  675 		setMazda626EngineConfiguration(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_27:
          CFI FunCall _Z30setMazda626EngineConfigurationv
        BL       _Z30setMazda626EngineConfigurationv
//  676 		break;
        B.N      ??resetConfigurationExt_33
//  677 	case SATURN_ION_2004:
//  678 		setSaturnIonEngineConfiguration(engineConfiguration);
??resetConfigurationExt_11:
        LDR.N    R0,??DataTable11_18
        LDR      R0,[R0, #+0]
          CFI FunCall setSaturnIonEngineConfiguration
        BL       setSaturnIonEngineConfiguration
//  679 		break;
        B.N      ??resetConfigurationExt_33
//  680 	case MINI_COOPER_R50:
//  681 		setMiniCooperR50(engineConfiguration);
??resetConfigurationExt_12:
        LDR.N    R0,??DataTable11_18
        LDR      R0,[R0, #+0]
          CFI FunCall _Z16setMiniCooperR50P22engine_configuration_s
        BL       _Z16setMiniCooperR50P22engine_configuration_s
//  682 		break;
        B.N      ??resetConfigurationExt_33
//  683 	case FORD_ESCORT_GT:
//  684 		setFordEscortGt(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_13:
          CFI FunCall _Z15setFordEscortGtv
        BL       _Z15setFordEscortGtv
//  685 		break;
        B.N      ??resetConfigurationExt_33
//  686 	case MIATA_1990:
//  687 		setMiata1990(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_18:
          CFI FunCall _Z12setMiata1990v
        BL       _Z12setMiata1990v
//  688 		break;
        B.N      ??resetConfigurationExt_33
//  689 	case MIATA_1994_DEVIATOR:
//  690 		setMiata1994_d(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_19:
          CFI FunCall _Z14setMiata1994_dv
        BL       _Z14setMiata1994_dv
//  691 		break;
        B.N      ??resetConfigurationExt_33
//  692 	case MIATA_1994_SPAGS:
//  693 		setMiata1994_s(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_23:
          CFI FunCall _Z14setMiata1994_sv
        BL       _Z14setMiata1994_sv
//  694 		break;
        B.N      ??resetConfigurationExt_33
//  695 	case MIATA_1996:
//  696 		setMiata1996(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_20:
          CFI FunCall _Z12setMiata1996v
        BL       _Z12setMiata1996v
//  697 		break;
        B.N      ??resetConfigurationExt_33
//  698 	case CITROEN_TU3JP:
//  699 		setCitroenBerlingoTU3JPConfiguration(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_14:
          CFI FunCall _Z36setCitroenBerlingoTU3JPConfigurationv
        BL       _Z36setCitroenBerlingoTU3JPConfigurationv
//  700 		break;
        B.N      ??resetConfigurationExt_33
//  701 	case ROVER_V8:
//  702 		setRoverv8(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_9:
          CFI FunCall _Z10setRoverv8v
        BL       _Z10setRoverv8v
//  703 		break;
        B.N      ??resetConfigurationExt_33
//  704 	case SUBARU_2003_WRX:
//  705 		setSubaru2003Wrx(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_21:
          CFI FunCall _Z16setSubaru2003Wrxv
        BL       _Z16setSubaru2003Wrxv
//  706 		break;
        B.N      ??resetConfigurationExt_33
//  707 	case BMW_E34:
//  708 		setBmwE34(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_24:
          CFI FunCall _Z9setBmwE34v
        BL       _Z9setBmwE34v
//  709 		break;
        B.N      ??resetConfigurationExt_33
//  710 	case GM_2_2:
//  711 		setGm2_2(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_29:
          CFI FunCall _Z8setGm2_2v
        BL       _Z8setGm2_2v
//  712 		break;
        B.N      ??resetConfigurationExt_33
//  713 	case DODGE_RAM:
//  714 		setDodgeRam1996(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_30:
          CFI FunCall _Z15setDodgeRam1996v
        BL       _Z15setDodgeRam1996v
//  715 		break;
        B.N      ??resetConfigurationExt_33
//  716 	case VW_ABA:
//  717 		setVwAba(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_31:
          CFI FunCall _Z8setVwAbav
        BL       _Z8setVwAbav
//  718 		break;
        B.N      ??resetConfigurationExt_33
//  719 	case TEST_ENGINE:
//  720 		setTestEngineConfiguration(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_25:
          CFI FunCall _Z26setTestEngineConfigurationv
        BL       _Z26setTestEngineConfigurationv
//  721 		break;
        B.N      ??resetConfigurationExt_33
//  722 	case SACHS:
//  723 		setSachs(PASS_ENGINE_PARAMETER_F);
??resetConfigurationExt_28:
          CFI FunCall _Z8setSachsv
        BL       _Z8setSachsv
//  724 		break;
        B.N      ??resetConfigurationExt_33
//  725 	default:
//  726 		warning(OBD_PCM_Processor_Fault, "Unexpected engine type: %d", engineType);
??resetConfigurationExt_32:
        MOVS     R2,R5
        LDR.N    R1,??DataTable11_20
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  727 	}
//  728 	applyNonPersistentConfiguration(logger PASS_ENGINE_PARAMETER);
??resetConfigurationExt_33:
        MOVS     R0,R4
          CFI FunCall _Z31applyNonPersistentConfigurationP7Logging
        BL       _Z31applyNonPersistentConfigurationP7Logging
//  729 	// todo: eliminate triggerShape.operationMode?
//  730 	if (engineConfiguration->operationMode != engine->triggerShape.getOperationMode())
        LDR.N    R0,??DataTable11_21
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+644
          CFI FunCall _ZN12TriggerShape16getOperationModeEv
        BL       _ZN12TriggerShape16getOperationModeEv
        LDR.N    R1,??DataTable11_18
        LDR      R1,[R1, #+0]
        LDR      R1,[R1, #+500]
        CMP      R1,R0
        BEQ.N    ??resetConfigurationExt_34
//  731 		firmwareError("operationMode mismatch");
        LDR.N    R0,??DataTable11_22
          CFI FunCall firmwareError
        BL       firmwareError
//  732 
//  733 #if EFI_TUNER_STUDIO
//  734 	syncTunerStudioCopy();
??resetConfigurationExt_34:
          CFI FunCall _Z19syncTunerStudioCopyv
        BL       _Z19syncTunerStudioCopyv
//  735 #endif
//  736 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock13

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8:
        DC32     0x4019999a

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_1:
        DC32     0x41a00000
//  737 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _ZN23engine_configuration2_sC1Ev
        THUMB
//  738 engine_configuration2_s::engine_configuration2_s() {
_ZN23engine_configuration2_sC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN12FuelScheduleC1Ev
        BL       _ZN12FuelScheduleC1Ev
        ADD      R0,R4,#+5888
        ADDS     R0,R0,#+136
          CFI FunCall _ZN12FuelScheduleC1Ev
        BL       _ZN12FuelScheduleC1Ev
        MOVS     R3,#+2
        MOVW     R2,#+7688
        LDR.N    R1,??DataTable11_23
        ADD      R0,R4,#+12032
        ADDS     R0,R0,#+80
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
//  739 }
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock14

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9:
        DC32     0x40a00000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_1:
        DC32     0x40066666

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_2:
        DC32     0x3fe66666

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_3:
        DC32     0x42820000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_4:
        DC32     0x42b40000
//  740 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function _Z31applyNonPersistentConfigurationP7Logging
        THUMB
//  741 void applyNonPersistentConfiguration(Logging * logger DECLARE_ENGINE_PARAMETER_S) {
_Z31applyNonPersistentConfigurationP7Logging:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  742 
//  743 // todo: this would require 'initThermistors() to re-establish a reference, todo: fix
//  744 //	memset(engineConfiguration2, 0, sizeof(engine_configuration2_s));
//  745 #if EFI_PROD_CODE
//  746 	scheduleMsg(logger, "applyNonPersistentConfiguration()");
        LDR.N    R1,??DataTable11_24
        MOVS     R0,R4
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  747 #endif
//  748 #if EFI_ENGINE_CONTROL
//  749 	engine->triggerShape.initializeTriggerShape(logger PASS_ENGINE_PARAMETER);
        MOVS     R1,R4
        LDR.N    R0,??DataTable11_21
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+644
          CFI FunCall _ZN12TriggerShape22initializeTriggerShapeEP7Logging
        BL       _ZN12TriggerShape22initializeTriggerShapeEP7Logging
//  750 #endif
//  751 	if (engine->triggerShape.getSize() == 0) {
        LDR.N    R0,??DataTable11_21
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+644
          CFI FunCall _ZNK12TriggerShape7getSizeEv
        BL       _ZNK12TriggerShape7getSizeEv
        CMP      R0,#+0
        BNE.N    ??applyNonPersistentConfiguration_0
//  752 		firmwareError("triggerShape size is zero");
        LDR.N    R0,??DataTable11_25
          CFI FunCall firmwareError
        BL       firmwareError
//  753 		return;
        B.N      ??applyNonPersistentConfiguration_1
//  754 	}
//  755 	if (engine->triggerShape.getSize() == 0) {
??applyNonPersistentConfiguration_0:
        LDR.N    R0,??DataTable11_21
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+644
          CFI FunCall _ZNK12TriggerShape7getSizeEv
        BL       _ZNK12TriggerShape7getSizeEv
        CMP      R0,#+0
        BNE.N    ??applyNonPersistentConfiguration_2
//  756 		firmwareError("shaftPositionEventCount is zero");
        LDR.N    R0,??DataTable11_26
          CFI FunCall firmwareError
        BL       firmwareError
//  757 		return;
        B.N      ??applyNonPersistentConfiguration_1
//  758 	}
//  759 	engine->engineCycleEventCount = engine->triggerShape.getLength();
??applyNonPersistentConfiguration_2:
        LDR.N    R0,??DataTable11_21
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+644
          CFI FunCall _ZNK12TriggerShape9getLengthEv
        BL       _ZNK12TriggerShape9getLengthEv
        MOVW     R1,#+7932
        LDR.N    R2,??DataTable11_21
        LDR      R2,[R2, #+0]
        STR      R0,[R1, R2]
//  760 #if EFI_FSIO || defined(__DOXYGEN__)
//  761 	applyFsioConfiguration(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z22applyFsioConfigurationv
        BL       _Z22applyFsioConfigurationv
//  762 #endif
//  763 }
??applyNonPersistentConfiguration_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock15

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10:
        DC32     0x420c0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_1:
        DC32     0x42480000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_2:
        DC32     0x41f80000
//  764 
//  765 #if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
//  766 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function _Z13prepareShapesv
        THUMB
//  767 void prepareShapes(DECLARE_ENGINE_PARAMETER_F) {
_Z13prepareShapesv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  768 	engine_configuration2_s *engineConfiguration2 = engine->engineConfiguration2;
        LDR.N    R0,??DataTable11_21
        LDR      R0,[R0, #+0]
        LDR      R4,[R0, #+40]
//  769 	prepareOutputSignals(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z20prepareOutputSignalsv
        BL       _Z20prepareOutputSignalsv
//  770 
//  771 	// todo: looks like this is here only for unit tests. todo: remove
//  772 	initializeIgnitionActions(0, 0, &engineConfiguration2->ignitionEvents[0] PASS_ENGINE_PARAMETER);
        ADD      R0,R4,#+12032
        ADDS     R0,R0,#+80
        VLDR.W   S1,??DataTable11_9  ;; 0x0
        VLDR.W   S0,??DataTable11_9  ;; 0x0
          CFI FunCall _Z25initializeIgnitionActionsffP9ArrayListI13IgnitionEventLi80EE
        BL       _Z25initializeIgnitionActionsffP9ArrayListI13IgnitionEventLi80EE
//  773 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock16

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11:
        DC32     0x41200000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_1:
        DC32     0x42500000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_2:
        DC32     0x42920000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_3:
        DC32     0x42940000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_4:
        DC32     0x42980000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_5:
        DC32     0x429a0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_6:
        DC32     0x428c0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_7:
        DC32     0x40aaaaab

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_8:
        DC32     0x42960000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_9:
        DC32     0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_10:
        DC32     0x43480000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_11:
        DC32     0x42af0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_12:
        DC32     0x43fa0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_13:
        DC32     0x43070000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_14:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_15:
        DC32     config

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_16:
        DC32     0x42200000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_17:
        DC32     0x3d4ccccd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_18:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_19:
        DC32     0x1221239

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_20:
        DC32     `?<Constant "Unexpected engine typ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_21:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_22:
        DC32     `?<Constant "operationMode mismatch">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_23:
        DC32     _ZN9ArrayListI13IgnitionEventLi80EEC1Ev

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_24:
        DC32     `?<Constant "applyNonPersistentCon...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_25:
        DC32     `?<Constant "triggerShape size is ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11_26:
        DC32     `?<Constant "shaftPositionEventCou...">`
//  774 
//  775 #endif
//  776 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function _Z16getRpmMultiplier16operation_mode_e
          CFI NoCalls
        THUMB
//  777 float getRpmMultiplier(operation_mode_e mode) {
//  778 	if (mode == FOUR_STROKE_CAM_SENSOR) {
_Z16getRpmMultiplier16operation_mode_e:
        CMP      R0,#+2
        BNE.N    ??getRpmMultiplier_0
//  779 		return 0.5;
        VMOV.F32 S0,#0.5
        B.N      ??getRpmMultiplier_1
//  780 	} else if (mode == FOUR_STROKE_CRANK_SENSOR) {
??getRpmMultiplier_0:
        CMP      R0,#+1
        BNE.N    ??getRpmMultiplier_2
//  781 		return 1;
        VMOV.F32 S0,#1.0
        B.N      ??getRpmMultiplier_1
//  782 	}
//  783 	return 1;
??getRpmMultiplier_2:
        VMOV.F32 S0,#1.0
??getRpmMultiplier_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock17
//  784 }
//  785 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function _Z16setOperationModeP22engine_configuration_s16operation_mode_e
          CFI NoCalls
        THUMB
//  786 void setOperationMode(engine_configuration_s *engineConfiguration, operation_mode_e mode) {
//  787 	engineConfiguration->operationMode = mode;
_Z16setOperationModeP22engine_configuration_s16operation_mode_e:
        STR      R1,[R0, #+500]
//  788 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock18
//  789 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock19 Using cfiCommon0
          CFI Function _Z16getOperationModePK22engine_configuration_s
          CFI NoCalls
        THUMB
//  790 operation_mode_e getOperationMode(engine_configuration_s const *engineConfiguration) {
//  791 	return engineConfiguration->operationMode;
_Z16getOperationModePK22engine_configuration_s:
        LDR      R0,[R0, #+500]
        BX       LR               ;; return
          CFI EndBlock cfiBlock19
//  792 }
//  793 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock20 Using cfiCommon0
          CFI Function _Z27commonFrankensoAnalogInputsP22engine_configuration_s
          CFI NoCalls
        THUMB
//  794 void commonFrankensoAnalogInputs(engine_configuration_s *engineConfiguration) {
//  795 	/**
//  796 	 * VBatt
//  797 	 */
//  798 	engineConfiguration->vbattAdcChannel = EFI_ADC_14;
_Z27commonFrankensoAnalogInputsP22engine_configuration_s:
        MOVS     R1,#+14
        STR      R1,[R0, #+472]
//  799 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock20
//  800 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock21 Using cfiCommon0
          CFI Function _Z24setFrankenso0_1_joystickP22engine_configuration_s
          CFI NoCalls
        THUMB
//  801 void setFrankenso0_1_joystick(engine_configuration_s *engineConfiguration) {
//  802 	engineConfiguration->isJoystickEnabled = true;
_Z24setFrankenso0_1_joystickP22engine_configuration_s:
        LDRB     R1,[R0, #+1497]
        ORRS     R1,R1,#0x8
        STRB     R1,[R0, #+1497]
//  803 	board_configuration_s *boardConfiguration = &engineConfiguration->bc;
        ADDS     R1,R0,#+624
//  804 	boardConfiguration->joystickCenterPin = GPIOC_8;
        MOVS     R2,#+40
        STR      R2,[R1, #+764]
//  805 	boardConfiguration->joystickAPin = GPIOD_10;
        MOVS     R2,#+58
        STR      R2,[R1, #+768]
//  806 	boardConfiguration->joystickBPin = GPIO_UNASSIGNED;
        MOVS     R2,#+80
        STR      R2,[R1, #+772]
//  807 	boardConfiguration->joystickCPin = GPIO_UNASSIGNED;
        MOVS     R2,#+80
        STR      R2,[R1, #+776]
//  808 	boardConfiguration->joystickDPin = GPIOD_11;
        MOVS     R2,#+59
        STR      R2,[R1, #+780]
//  809 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock21
//  810 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock22 Using cfiCommon0
          CFI Function _Z13copyFuelTablePA16_KfPA16_f
          CFI NoCalls
        THUMB
//  811 void copyFuelTable(fuel_table_t const source, fuel_table_t destination) {
_Z13copyFuelTablePA16_KfPA16_f:
        PUSH     {R4,R5}
          CFI R5 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  812 	for (int k = 0; k < FUEL_LOAD_COUNT; k++) {
        MOVS     R2,#+0
??copyFuelTable_0:
        CMP      R2,#+16
        BGE.N    ??copyFuelTable_1
//  813 		for (int r = 0; r < FUEL_RPM_COUNT; r++) {
        MOVS     R3,#+0
??copyFuelTable_2:
        CMP      R3,#+16
        BGE.N    ??copyFuelTable_3
//  814 			destination[k][r] = source[k][r];
        MOVS     R4,#+64
        MLA      R4,R4,R2,R1
        MOVS     R5,#+64
        MLA      R5,R5,R2,R0
        LDR      R5,[R5, R3, LSL #+2]
        STR      R5,[R4, R3, LSL #+2]
//  815 		}
        ADDS     R3,R3,#+1
        B.N      ??copyFuelTable_2
//  816 	}
??copyFuelTable_3:
        ADDS     R2,R2,#+1
        B.N      ??copyFuelTable_0
//  817 }
??copyFuelTable_1:
        POP      {R4,R5}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock22

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock23 Using cfiCommon0
          CFI Function _Z15copyTimingTablePA16_KfPA16_f
          CFI NoCalls
        THUMB
_Z15copyTimingTablePA16_KfPA16_f:
        PUSH     {R4,R5}
          CFI R5 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R2,#+0
??copyTimingTable_0:
        CMP      R2,#+16
        BGE.N    ??copyTimingTable_1
        MOVS     R3,#+0
??copyTimingTable_2:
        CMP      R3,#+16
        BGE.N    ??copyTimingTable_3
        MOVS     R4,#+64
        MLA      R4,R4,R2,R1
        MOVS     R5,#+64
        MLA      R5,R5,R2,R0
        LDR      R5,[R5, R3, LSL #+2]
        STR      R5,[R4, R3, LSL #+2]
        ADDS     R3,R3,#+1
        B.N      ??copyTimingTable_2
??copyTimingTable_3:
        ADDS     R2,R2,#+1
        B.N      ??copyTimingTable_0
??copyTimingTable_1:
        POP      {R4,R5}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock23

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memset
          CFI Block cfiBlock24 Using cfiCommon0
          CFI Function memset
        THUMB
// __intrinsic __nounwind __interwork __softfp void *memset(void *, int, size_t)
memset:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R2,R5
        MOVS     R1,R6
        MOVS     R0,R4
          CFI FunCall __aeabi_memset
        BL       __aeabi_memset
        MOVS     R0,R4
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock24

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN9ArrayListI13IgnitionEventLi80EEC1Ev
          CFI Block cfiBlock25 Using cfiCommon0
          CFI Function _ZN9ArrayListI13IgnitionEventLi80EEC1Ev
        THUMB
// __code __interwork __softfp ArrayList<IgnitionEvent, 80>::ArrayList()
_ZN9ArrayListI13IgnitionEventLi80EEC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R3,#+80
        MOVS     R2,#+96
        LDR.N    R1,??ArrayList_0
        ADDS     R0,R4,#+8
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
        MOV      R2,#+7680
        MOVS     R1,#+0
        ADDS     R0,R4,#+8
          CFI FunCall memset
        BL       memset
        MOVS     R0,R4
          CFI FunCall _ZN9ArrayListI13IgnitionEventLi80EE5resetEv
        BL       _ZN9ArrayListI13IgnitionEventLi80EE5resetEv
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
        Nop      
        DATA
??ArrayList_0:
        DC32     _ZN13IgnitionEventC1Ev
          CFI EndBlock cfiBlock25

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN9ArrayListI13IgnitionEventLi80EE5resetEv
          CFI Block cfiBlock26 Using cfiCommon0
          CFI Function _ZN9ArrayListI13IgnitionEventLi80EE5resetEv
          CFI NoCalls
        THUMB
// __interwork __softfp void ArrayList<IgnitionEvent, 80>::reset()
_ZN9ArrayListI13IgnitionEventLi80EE5resetEv:
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock26

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

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 "0"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Unexpected engine typ...">`:
        DATA
        DC8 "Unexpected engine type: %d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "operationMode mismatch">`:
        DATA
        DC8 "operationMode mismatch"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "applyNonPersistentCon...">`:
        DATA
        DC8 "applyNonPersistentConfiguration()"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "triggerShape size is ...">`:
        DATA
        DC8 "triggerShape size is zero"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "shaftPositionEventCou...">`:
        DATA
        DC8 "shaftPositionEventCount is zero"

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN9ArrayListI13IgnitionEventLi80EEC2Ev
          CFI Block cfiBlock27 Using cfiCommon0
          CFI Function _ZN9ArrayListI13IgnitionEventLi80EEC2Ev
        THUMB
// __code __interwork __softfp ArrayList<IgnitionEvent, 80>::subobject ArrayList()
_ZN9ArrayListI13IgnitionEventLi80EEC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN9ArrayListI13IgnitionEventLi80EEC1Ev
        BL       _ZN9ArrayListI13IgnitionEventLi80EEC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock27

        END
//  818 
//  819 void copyTimingTable(ignition_table_t const source, ignition_table_t destination) {
//  820 	for (int k = 0; k < IGN_LOAD_COUNT; k++) {
//  821 		for (int r = 0; r < IGN_RPM_COUNT; r++) {
//  822 			destination[k][r] = source[k][r];
//  823 		}
//  824 	}
//  825 }
//  826 
// 
//     4 bytes in section .bss
//   152 bytes in section .rodata
// 6 348 bytes in section .text
// 
// 6 260 bytes of CODE  memory (+ 88 bytes shared)
//   152 bytes of CONST memory
//     4 bytes of DATA  memory
//
//Errors: none
//Warnings: 3
