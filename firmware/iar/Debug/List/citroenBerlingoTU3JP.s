///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:43 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       citroenBerlingoTU3JP.cpp                             /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       citroenBerlingoTU3JP.cpp -lCN                        /
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
//                       citroenBerlingoTU3JP.s                               /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME citroenBerlingoTU3JP

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z12setAlgorithm18engine_load_mode_e
        EXTERN _Z13copyFuelTablePA16_KfPA16_f
        EXTERN _Z13setFuelRpmBinff
        EXTERN _Z14setFuelLoadBinff
        EXTERN _Z15copyTimingTablePA16_KfPA16_f
        EXTERN _Z15setTimingRpmBinff
        EXTERN _Z16setOperationModeP22engine_configuration_s16operation_mode_e
        EXTERN _Z26setThermistorConfigurationP14ThermistorConfffffff
        EXTERN boardConfiguration
        EXTERN config
        EXTERN engineConfiguration

        PUBLIC _Z36setCitroenBerlingoTU3JPConfigurationv
        PUBLIC _Z6setLCDP21board_configuration_s
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\config\engines\citroenBerlingoTU3JP.cpp
//    1 /**
//    2  * @file	citroenBerlingoTU3JP.cpp
//    3  *
//    4  * CITROEN_TU3JP
//    5  * set_engine_type 15
//    6  * Board pin out for Frankenstein
//    7  *
//    8  * This config overrides some values of the default configuration which is set by setDefaultConfiguration() method
//    9  *
//   10  *
//   11  * @date Apr 15, 2014
//   12  * @author Andrey Belomutskiy, (c) 2012-2015
//   13  */
//   14 
//   15 #include "engine_math.h"
//   16 #include "thermistors.h"
//   17 #include "citroenBerlingoTU3JP.h"
//   18 #include "settings.h"
//   19 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z6setLCDP21board_configuration_s
          CFI NoCalls
        THUMB
//   20 void setLCD(board_configuration_s *boardConfiguration) {
//   21 	boardConfiguration->HD44780_rs = GPIOE_7;
_Z6setLCDP21board_configuration_s:
        MOVS     R1,#+71
        STR      R1,[R0, #+176]
//   22 	boardConfiguration->HD44780_e = GPIOE_9;
        MOVS     R1,#+73
        STR      R1,[R0, #+180]
//   23 	boardConfiguration->HD44780_db4 = GPIOE_11;
        MOVS     R1,#+75
        STR      R1,[R0, #+184]
//   24 	boardConfiguration->HD44780_db5 = GPIOE_13;
        MOVS     R1,#+77
        STR      R1,[R0, #+188]
//   25 	boardConfiguration->HD44780_db6 = GPIOE_15;
        MOVS     R1,#+79
        STR      R1,[R0, #+192]
//   26 	boardConfiguration->HD44780_db7 = GPIOB_10;
        MOVS     R1,#+26
        STR      R1,[R0, #+196]
//   27 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//   28 

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//   29 static const fuel_table_t tps_fuel_table = {
tps_fuel_table:
        DATA
        DC32 40800000H, 40800000H, 40800000H, 40800000H, 40800000H, 40800000H
        DC32 40800000H, 40800000H, 40800000H, 40800000H, 40800000H, 40800000H
        DC32 40800000H, 40800000H, 40800000H, 40800000H, 40A23D71H, 40A23D71H
        DC32 40A23D71H, 40A23D71H, 40A23D71H, 40A23D71H, 40A23D71H, 40A23D71H
        DC32 40A23D71H, 40A23D71H, 40A23D71H, 40A23D71H, 40A23D71H, 40A23D71H
        DC32 40A23D71H, 40A23D71H, 40C428F6H, 40C428F6H, 40C428F6H, 40C428F6H
        DC32 40C428F6H, 40C428F6H, 40C428F6H, 40C428F6H, 40C428F6H, 40C428F6H
        DC32 40C428F6H, 40C428F6H, 40C428F6H, 40C428F6H, 40C428F6H, 40C428F6H
        DC32 40E66666H, 40E66666H, 40E66666H, 40E66666H, 40E66666H, 40E66666H
        DC32 40E66666H, 40E66666H, 40E66666H, 40E66666H, 40E66666H, 40E66666H
        DC32 40E66666H, 40E66666H, 40E66666H, 40E66666H, 410451ECH, 410451ECH
        DC32 410451ECH, 410451ECH, 410451ECH, 410451ECH, 410451ECH, 410451ECH
        DC32 410451ECH, 410451ECH, 410451ECH, 410451ECH, 410451ECH, 410451ECH
        DC32 410451ECH, 410451ECH, 411547AEH, 411547AEH, 411547AEH, 411547AEH
        DC32 411547AEH, 411547AEH, 411547AEH, 411547AEH, 411547AEH, 411547AEH
        DC32 411547AEH, 411547AEH, 411547AEH, 411547AEH, 411547AEH, 411547AEH
        DC32 41266666H, 41266666H, 41266666H, 41266666H, 41266666H, 41266666H
        DC32 41266666H, 41266666H, 41266666H, 41266666H, 41266666H, 41266666H
        DC32 41266666H, 41266666H, 41266666H, 41266666H, 4137851FH, 4137851FH
        DC32 4137851FH, 4137851FH, 4137851FH, 4137851FH, 4137851FH, 4137851FH
        DC32 4137851FH, 4137851FH, 4137851FH, 4137851FH, 4137851FH, 4137851FH
        DC32 4137851FH, 4137851FH, 41487AE1H, 41487AE1H, 41487AE1H, 41487AE1H
        DC32 41487AE1H, 41487AE1H, 41487AE1H, 41487AE1H, 41487AE1H, 41487AE1H
        DC32 41487AE1H, 41487AE1H, 41487AE1H, 41487AE1H, 41487AE1H, 41487AE1H
        DC32 4149999AH, 4159999AH, 4159999AH, 4159999AH, 4159999AH, 4159999AH
        DC32 4159999AH, 4159999AH, 4159999AH, 4159999AH, 4159999AH, 4159999AH
        DC32 4159999AH, 4159999AH, 4159999AH, 4159999AH, 416AB852H, 416AB852H
        DC32 416AB852H, 416AB852H, 416AB852H, 416AB852H, 416AB852H, 416AB852H
        DC32 416AB852H, 416AB852H, 416AB852H, 416AB852H, 416AB852H, 416AB852H
        DC32 416AB852H, 416AB852H, 417BAE14H, 417BAE14H, 417BAE14H, 417BAE14H
        DC32 417BAE14H, 417BAE14H, 417BAE14H, 417BAE14H, 417BAE14H, 417BAE14H
        DC32 417BAE14H, 417BAE14H, 417BAE14H, 417BAE14H, 417BAE14H, 417BAE14H
        DC32 41866666H, 41866666H, 41866666H, 41866666H, 41866666H, 41866666H
        DC32 41866666H, 41866666H, 41866666H, 41866666H, 41866666H, 41866666H
        DC32 41866666H, 41866666H, 41866666H, 41866666H, 418EF5C3H, 418EF5C3H
        DC32 418EF5C3H, 418EF5C3H, 418EF5C3H, 418EF5C3H, 418EF5C3H, 418EF5C3H
        DC32 418EF5C3H, 418EF5C3H, 418EF5C3H, 418EF5C3H, 418EF5C3H, 418EF5C3H
        DC32 418EF5C3H, 418EF5C3H, 419770A4H, 419770A4H, 419770A4H, 419770A4H
        DC32 419770A4H, 419770A4H, 419770A4H, 419770A4H, 419770A4H, 419770A4H
        DC32 419770A4H, 419770A4H, 419770A4H, 419770A4H, 419770A4H, 419770A4H
        DC32 41A00000H, 41A00000H, 41A00000H, 41A00000H, 41A00000H, 41A00000H
        DC32 41A00000H, 41A00000H, 41A00000H, 41A00000H, 41A00000H, 41A00000H
        DC32 41A00000H, 41A00000H, 41A00000H, 41A00000H
//   30 {/*0  engineLoad=0.00*/   /*0 800.0*/4.00,  /*1 1213.0*/4.00,  /*2 1626.0*/4.00,  /*3 2040.0*/4.00,  /*4 2453.0*/4.00,  /*5 2866.0*/4.00,  /*6 3280.0*/4.00,  /*7 3693.0*/4.00,  /*8 4106.0*/4.00,  /*9 4520.0*/4.00,  /*10 4933.0*/4.00,  /*11 5346.0*/4.00,  /*12 5760.0*/4.00,  /*13 6173.0*/4.00,  /*14 6586.0*/4.00,  /*15 7000.0*/4.00},
//   31 {/*1  engineLoad=6.66*/   /*0 800.0*/5.07,  /*1 1213.0*/5.07,  /*2 1626.0*/5.07,  /*3 2040.0*/5.07,  /*4 2453.0*/5.07,  /*5 2866.0*/5.07,  /*6 3280.0*/5.07,  /*7 3693.0*/5.07,  /*8 4106.0*/5.07,  /*9 4520.0*/5.07,  /*10 4933.0*/5.07,  /*11 5346.0*/5.07,  /*12 5760.0*/5.07,  /*13 6173.0*/5.07,  /*14 6586.0*/5.07,  /*15 7000.0*/5.07},
//   32 {/*2  engineLoad=13.33*/  /*0 800.0*/6.13,  /*1 1213.0*/6.13,  /*2 1626.0*/6.13,  /*3 2040.0*/6.13,  /*4 2453.0*/6.13,  /*5 2866.0*/6.13,  /*6 3280.0*/6.13,  /*7 3693.0*/6.13,  /*8 4106.0*/6.13,  /*9 4520.0*/6.13,  /*10 4933.0*/6.13,  /*11 5346.0*/6.13,  /*12 5760.0*/6.13,  /*13 6173.0*/6.13,  /*14 6586.0*/6.13,  /*15 7000.0*/6.13},
//   33 {/*3  engineLoad=20.00*/  /*0 800.0*/7.20,  /*1 1213.0*/7.20,  /*2 1626.0*/7.20,  /*3 2040.0*/7.20,  /*4 2453.0*/7.20,  /*5 2866.0*/7.20,  /*6 3280.0*/7.20,  /*7 3693.0*/7.20,  /*8 4106.0*/7.20,  /*9 4520.0*/7.20,  /*10 4933.0*/7.20,  /*11 5346.0*/7.20,  /*12 5760.0*/7.20,  /*13 6173.0*/7.20,  /*14 6586.0*/7.20,  /*15 7000.0*/7.20},
//   34 {/*4  engineLoad=26.66*/  /*0 800.0*/8.27,  /*1 1213.0*/8.27,  /*2 1626.0*/8.27,  /*3 2040.0*/8.27,  /*4 2453.0*/8.27,  /*5 2866.0*/8.27,  /*6 3280.0*/8.27,  /*7 3693.0*/8.27,  /*8 4106.0*/8.27,  /*9 4520.0*/8.27,  /*10 4933.0*/8.27,  /*11 5346.0*/8.27,  /*12 5760.0*/8.27,  /*13 6173.0*/8.27,  /*14 6586.0*/8.27,  /*15 7000.0*/8.27},
//   35 {/*5  engineLoad=33.33*/  /*0 800.0*/9.33,  /*1 1213.0*/9.33,  /*2 1626.0*/9.33,  /*3 2040.0*/9.33,  /*4 2453.0*/9.33,  /*5 2866.0*/9.33,  /*6 3280.0*/9.33,  /*7 3693.0*/9.33,  /*8 4106.0*/9.33,  /*9 4520.0*/9.33,  /*10 4933.0*/9.33,  /*11 5346.0*/9.33,  /*12 5760.0*/9.33,  /*13 6173.0*/9.33,  /*14 6586.0*/9.33,  /*15 7000.0*/9.33},
//   36 {/*6  engineLoad=40.00*/  /*0 800.0*/10.40, /*1 1213.0*/10.40, /*2 1626.0*/10.40, /*3 2040.0*/10.40, /*4 2453.0*/10.40, /*5 2866.0*/10.40, /*6 3280.0*/10.40, /*7 3693.0*/10.40, /*8 4106.0*/10.40, /*9 4520.0*/10.40, /*10 4933.0*/10.40, /*11 5346.0*/10.40, /*12 5760.0*/10.40, /*13 6173.0*/10.40, /*14 6586.0*/10.40, /*15 7000.0*/10.40},
//   37 {/*7  engineLoad=46.66*/  /*0 800.0*/11.47, /*1 1213.0*/11.47, /*2 1626.0*/11.47, /*3 2040.0*/11.47, /*4 2453.0*/11.47, /*5 2866.0*/11.47, /*6 3280.0*/11.47, /*7 3693.0*/11.47, /*8 4106.0*/11.47, /*9 4520.0*/11.47, /*10 4933.0*/11.47, /*11 5346.0*/11.47, /*12 5760.0*/11.47, /*13 6173.0*/11.47, /*14 6586.0*/11.47, /*15 7000.0*/11.47},
//   38 {/*8  engineLoad=53.33*/  /*0 800.0*/12.53, /*1 1213.0*/12.53, /*2 1626.0*/12.53, /*3 2040.0*/12.53, /*4 2453.0*/12.53, /*5 2866.0*/12.53, /*6 3280.0*/12.53, /*7 3693.0*/12.53, /*8 4106.0*/12.53, /*9 4520.0*/12.53, /*10 4933.0*/12.53, /*11 5346.0*/12.53, /*12 5760.0*/12.53, /*13 6173.0*/12.53, /*14 6586.0*/12.53, /*15 7000.0*/12.53},
//   39 {/*9  engineLoad=60.00*/  /*0 800.0*/12.60, /*1 1213.0*/13.60, /*2 1626.0*/13.60, /*3 2040.0*/13.60, /*4 2453.0*/13.60, /*5 2866.0*/13.60, /*6 3280.0*/13.60, /*7 3693.0*/13.60, /*8 4106.0*/13.60, /*9 4520.0*/13.60, /*10 4933.0*/13.60, /*11 5346.0*/13.60, /*12 5760.0*/13.60, /*13 6173.0*/13.60, /*14 6586.0*/13.60, /*15 7000.0*/13.60},
//   40 {/*10 engineLoad=66.66*/  /*0 800.0*/14.67, /*1 1213.0*/14.67, /*2 1626.0*/14.67, /*3 2040.0*/14.67, /*4 2453.0*/14.67, /*5 2866.0*/14.67, /*6 3280.0*/14.67, /*7 3693.0*/14.67, /*8 4106.0*/14.67, /*9 4520.0*/14.67, /*10 4933.0*/14.67, /*11 5346.0*/14.67, /*12 5760.0*/14.67, /*13 6173.0*/14.67, /*14 6586.0*/14.67, /*15 7000.0*/14.67},
//   41 {/*11 engineLoad=73.33*/  /*0 800.0*/15.73, /*1 1213.0*/15.73, /*2 1626.0*/15.73, /*3 2040.0*/15.73, /*4 2453.0*/15.73, /*5 2866.0*/15.73, /*6 3280.0*/15.73, /*7 3693.0*/15.73, /*8 4106.0*/15.73, /*9 4520.0*/15.73, /*10 4933.0*/15.73, /*11 5346.0*/15.73, /*12 5760.0*/15.73, /*13 6173.0*/15.73, /*14 6586.0*/15.73, /*15 7000.0*/15.73},
//   42 {/*12 engineLoad=80.00*/  /*0 800.0*/16.80, /*1 1213.0*/16.80, /*2 1626.0*/16.80, /*3 2040.0*/16.80, /*4 2453.0*/16.80, /*5 2866.0*/16.80, /*6 3280.0*/16.80, /*7 3693.0*/16.80, /*8 4106.0*/16.80, /*9 4520.0*/16.80, /*10 4933.0*/16.80, /*11 5346.0*/16.80, /*12 5760.0*/16.80, /*13 6173.0*/16.80, /*14 6586.0*/16.80, /*15 7000.0*/16.80},
//   43 {/*13 engineLoad=86.66*/  /*0 800.0*/17.87, /*1 1213.0*/17.87, /*2 1626.0*/17.87, /*3 2040.0*/17.87, /*4 2453.0*/17.87, /*5 2866.0*/17.87, /*6 3280.0*/17.87, /*7 3693.0*/17.87, /*8 4106.0*/17.87, /*9 4520.0*/17.87, /*10 4933.0*/17.87, /*11 5346.0*/17.87, /*12 5760.0*/17.87, /*13 6173.0*/17.87, /*14 6586.0*/17.87, /*15 7000.0*/17.87},
//   44 {/*14 engineLoad=93.33*/  /*0 800.0*/18.93, /*1 1213.0*/18.93, /*2 1626.0*/18.93, /*3 2040.0*/18.93, /*4 2453.0*/18.93, /*5 2866.0*/18.93, /*6 3280.0*/18.93, /*7 3693.0*/18.93, /*8 4106.0*/18.93, /*9 4520.0*/18.93, /*10 4933.0*/18.93, /*11 5346.0*/18.93, /*12 5760.0*/18.93, /*13 6173.0*/18.93, /*14 6586.0*/18.93, /*15 7000.0*/18.93},
//   45 {/*15 engineLoad=100.00*/ /*0 800.0*/20.00, /*1 1213.0*/20.00, /*2 1626.0*/20.00, /*3 2040.0*/20.00, /*4 2453.0*/20.00, /*5 2866.0*/20.00, /*6 3280.0*/20.00, /*7 3693.0*/20.00, /*8 4106.0*/20.00, /*9 4520.0*/20.00, /*10 4933.0*/20.00, /*11 5346.0*/20.00, /*12 5760.0*/20.00, /*13 6173.0*/20.00, /*14 6586.0*/20.00, /*15 7000.0*/20.00}
//   46 };
//   47 

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//   48 static const ignition_table_t tps_advance_table = {
tps_advance_table:
        DATA
        DC32 41700000H, 4182A3D7H, 418D5C29H, 41980000H, 41A2A3D7H, 41AD5C29H
        DC32 41B80000H, 41C2A3D7H, 41CD5C29H, 41D80000H, 41E2A3D7H, 41ED5C29H
        DC32 41F80000H, 420151ECH, 4206AE14H, 420C0000H, 416CCCCDH, 4180F5C3H
        DC32 418B999AH, 419628F6H, 41A0CCCDH, 41AB5C29H, 41B60000H, 41C08F5CH
        DC32 41CB3333H, 41D5C28FH, 41E051ECH, 41EAF5C3H, 41F5851FH, 4200147BH
        DC32 42055C29H, 420AAE14H, 4169999AH, 417EB852H, 4189D70AH, 41946666H
        DC32 419EE148H, 41A970A4H, 41B3EB85H, 41BE7AE1H, 41C8F5C3H, 41D3851FH
        DC32 41DE147BH, 41E88F5CH, 41F31EB8H, 41FD999AH, 4204147BH, 420951ECH
        DC32 41666666H, 417B5C29H, 4188147BH, 41928F5CH, 419D0A3DH, 41A770A4H
        DC32 41B1EB85H, 41BC6666H, 41C6CCCDH, 41D147AEH, 41DBC28FH, 41E628F6H
        DC32 41F0A3D7H, 41FB1EB8H, 4202C28FH, 42080000H, 41633333H, 41780000H
        DC32 41866666H, 4190B852H, 419B1EB8H, 41A5851FH, 41AFEB85H, 41BA3D71H
        DC32 41C4A3D7H, 41CF0A3DH, 41D970A4H, 41E3D70AH, 41EE28F6H, 41F88F5CH
        DC32 42017AE1H, 4206AE14H, 41600000H, 4174A3D7H, 4184A3D7H, 418EF5C3H
        DC32 419947AEH, 41A3851FH, 41ADD70AH, 41B828F6H, 41C27AE1H, 41CCCCCDH
        DC32 41D71EB8H, 41E170A4H, 41EBC28FH, 41F6147BH, 420028F6H, 420551ECH
        DC32 415CCCCDH, 417147AEH, 4182E148H, 418D1EB8H, 41975C29H, 41A1999AH
        DC32 41ABD70AH, 41B6147BH, 41C051ECH, 41CA8F5CH, 41D4CCCDH, 41DF0A3DH
        DC32 41E947AEH, 41F3851FH, 41FDC28FH, 42040000H, 4159999AH, 416DEB85H
        DC32 41811EB8H, 418B47AEH, 419570A4H, 419FAE14H, 41A9D70AH, 41B40000H
        DC32 41BE28F6H, 41C851ECH, 41D27AE1H, 41DCA3D7H, 41E6CCCDH, 41F0F5C3H
        DC32 41FB3333H, 4202AE14H, 41566666H, 416A8F5CH, 417EB852H, 4189851FH
        DC32 4193999AH, 419DAE14H, 41A7C28FH, 41B1EB85H, 41BC0000H, 41C6147BH
        DC32 41D028F6H, 41DA3D71H, 41E46666H, 41EE7AE1H, 41F88F5CH, 420151ECH
        DC32 41533333H, 41673333H, 417B5C29H, 4187AE14H, 4191AE14H, 419BC28FH
        DC32 41A5C28FH, 41AFC28FH, 41B9D70AH, 41C3D70AH, 41CDD70AH, 41D7EB85H
        DC32 41E1EB85H, 41EBEB85H, 41F60000H, 42000000H, 41500000H, 4163D70AH
        DC32 4177D70AH, 4185D70AH, 418FD70AH, 4199C28FH, 41A3C28FH, 41ADAE14H
        DC32 41B7AE14H, 41C1999AH, 41CB851FH, 41D5851FH, 41DF70A4H, 41E970A4H
        DC32 41F35C29H, 41FD5C29H, 414CCCCDH, 4160A3D7H, 417451ECH, 4184147BH
        DC32 418DEB85H, 4197D70AH, 41A1AE14H, 41AB999AH, 41B570A4H, 41BF5C29H
        DC32 41C947AEH, 41D31EB8H, 41DD0A3DH, 41E6E148H, 41F0CCCDH, 41FAA3D7H
        DC32 4149999AH, 415D47AEH, 4170CCCDH, 41823D71H, 418C147BH, 4195D70AH
        DC32 419FAE14H, 41A9851FH, 41B347AEH, 41BD1EB8H, 41C6F5C3H, 41D0B852H
        DC32 41DA8F5CH, 41E46666H, 41EE28F6H, 41F80000H, 41466666H, 4159EB85H
        DC32 416D70A4H, 41806666H, 418A28F6H, 4193EB85H, 419DAE14H, 41A75C29H
        DC32 41B11EB8H, 41BAE148H, 41C4A3D7H, 41CE6666H, 41D8147BH, 41E1D70AH
        DC32 41EB999AH, 41F55C29H, 41433333H, 41568F5CH, 4169EB85H, 417D47AEH
        DC32 418851ECH, 4191EB85H, 419B999AH, 41A547AEH, 41AEF5C3H, 41B8A3D7H
        DC32 41C251ECH, 41CC0000H, 41D5AE14H, 41DF5C29H, 41E8F5C3H, 41F2A3D7H
        DC32 41400000H, 41533333H, 41666666H, 4179999AH, 41866666H, 41900000H
        DC32 4199999AH, 41A33333H, 41ACCCCDH, 41B66666H, 41C00000H, 41C9999AH
        DC32 41D33333H, 41DCCCCDH, 41E66666H, 41F00000H
//   49 {/*0  engineLoad=0.00*/   /*0 800.0*/+15.00, /*1 1213.0*/+16.33, /*2 1626.0*/+17.67, /*3 2040.0*/+19.00, /*4 2453.0*/+20.33, /*5 2866.0*/+21.67, /*6 3280.0*/+23.00, /*7 3693.0*/+24.33, /*8 4106.0*/+25.67, /*9 4520.0*/+27.00, /*10 4933.0*/+28.33, /*11 5346.0*/+29.67, /*12 5760.0*/+31.00, /*13 6173.0*/+32.33, /*14 6586.0*/+33.67, /*15 7000.0*/+35.00},
//   50 {/*1  engineLoad=6.66*/	  /*0 800.0*/+14.80, /*1 1213.0*/+16.12, /*2 1626.0*/+17.45, /*3 2040.0*/+18.77, /*4 2453.0*/+20.10, /*5 2866.0*/+21.42, /*6 3280.0*/+22.75, /*7 3693.0*/+24.07, /*8 4106.0*/+25.40, /*9 4520.0*/+26.72, /*10 4933.0*/+28.04, /*11 5346.0*/+29.37, /*12 5760.0*/+30.69, /*13 6173.0*/+32.02, /*14 6586.0*/+33.34, /*15 7000.0*/+34.67},
//   51 {/*2  engineLoad=13.33*/  /*0 800.0*/+14.60, /*1 1213.0*/+15.92, /*2 1626.0*/+17.23, /*3 2040.0*/+18.55, /*4 2453.0*/+19.86, /*5 2866.0*/+21.18, /*6 3280.0*/+22.49, /*7 3693.0*/+23.81, /*8 4106.0*/+25.12, /*9 4520.0*/+26.44, /*10 4933.0*/+27.76, /*11 5346.0*/+29.07, /*12 5760.0*/+30.39, /*13 6173.0*/+31.70, /*14 6586.0*/+33.02, /*15 7000.0*/+34.33},
//   52 {/*3  engineLoad=20.00*/  /*0 800.0*/+14.40, /*1 1213.0*/+15.71, /*2 1626.0*/+17.01, /*3 2040.0*/+18.32, /*4 2453.0*/+19.63, /*5 2866.0*/+20.93, /*6 3280.0*/+22.24, /*7 3693.0*/+23.55, /*8 4106.0*/+24.85, /*9 4520.0*/+26.16, /*10 4933.0*/+27.47, /*11 5346.0*/+28.77, /*12 5760.0*/+30.08, /*13 6173.0*/+31.39, /*14 6586.0*/+32.69, /*15 7000.0*/+34.00},
//   53 {/*4  engineLoad=26.66*/  /*0 800.0*/+14.20, /*1 1213.0*/+15.50, /*2 1626.0*/+16.80, /*3 2040.0*/+18.09, /*4 2453.0*/+19.39, /*5 2866.0*/+20.69, /*6 3280.0*/+21.99, /*7 3693.0*/+23.28, /*8 4106.0*/+24.58, /*9 4520.0*/+25.88, /*10 4933.0*/+27.18, /*11 5346.0*/+28.48, /*12 5760.0*/+29.77, /*13 6173.0*/+31.07, /*14 6586.0*/+32.37, /*15 7000.0*/+33.67},
//   54 {/*5  engineLoad=33.33*/  /*0 800.0*/+14.00, /*1 1213.0*/+15.29, /*2 1626.0*/+16.58, /*3 2040.0*/+17.87, /*4 2453.0*/+19.16, /*5 2866.0*/+20.44, /*6 3280.0*/+21.73, /*7 3693.0*/+23.02, /*8 4106.0*/+24.31, /*9 4520.0*/+25.60, /*10 4933.0*/+26.89, /*11 5346.0*/+28.18, /*12 5760.0*/+29.47, /*13 6173.0*/+30.76, /*14 6586.0*/+32.04, /*15 7000.0*/+33.33},
//   55 {/*6  engineLoad=40.00*/  /*0 800.0*/+13.80, /*1 1213.0*/+15.08, /*2 1626.0*/+16.36, /*3 2040.0*/+17.64, /*4 2453.0*/+18.92, /*5 2866.0*/+20.20, /*6 3280.0*/+21.48, /*7 3693.0*/+22.76, /*8 4106.0*/+24.04, /*9 4520.0*/+25.32, /*10 4933.0*/+26.60, /*11 5346.0*/+27.88, /*12 5760.0*/+29.16, /*13 6173.0*/+30.44, /*14 6586.0*/+31.72, /*15 7000.0*/+33.00},
//   56 {/*7  engineLoad=46.66*/  /*0 800.0*/+13.60, /*1 1213.0*/+14.87, /*2 1626.0*/+16.14, /*3 2040.0*/+17.41, /*4 2453.0*/+18.68, /*5 2866.0*/+19.96, /*6 3280.0*/+21.23, /*7 3693.0*/+22.50, /*8 4106.0*/+23.77, /*9 4520.0*/+25.04, /*10 4933.0*/+26.31, /*11 5346.0*/+27.58, /*12 5760.0*/+28.85, /*13 6173.0*/+30.12, /*14 6586.0*/+31.40, /*15 7000.0*/+32.67},
//   57 {/*8  engineLoad=53.33*/  /*0 800.0*/+13.40, /*1 1213.0*/+14.66, /*2 1626.0*/+15.92, /*3 2040.0*/+17.19, /*4 2453.0*/+18.45, /*5 2866.0*/+19.71, /*6 3280.0*/+20.97, /*7 3693.0*/+22.24, /*8 4106.0*/+23.50, /*9 4520.0*/+24.76, /*10 4933.0*/+26.02, /*11 5346.0*/+27.28, /*12 5760.0*/+28.55, /*13 6173.0*/+29.81, /*14 6586.0*/+31.07, /*15 7000.0*/+32.33},
//   58 {/*9  engineLoad=60.00*/  /*0 800.0*/+13.20, /*1 1213.0*/+14.45, /*2 1626.0*/+15.71, /*3 2040.0*/+16.96, /*4 2453.0*/+18.21, /*5 2866.0*/+19.47, /*6 3280.0*/+20.72, /*7 3693.0*/+21.97, /*8 4106.0*/+23.23, /*9 4520.0*/+24.48, /*10 4933.0*/+25.73, /*11 5346.0*/+26.99, /*12 5760.0*/+28.24, /*13 6173.0*/+29.49, /*14 6586.0*/+30.75, /*15 7000.0*/+32.00},
//   59 {/*10 engineLoad=66.66*/  /*0 800.0*/+13.00, /*1 1213.0*/+14.24, /*2 1626.0*/+15.49, /*3 2040.0*/+16.73, /*4 2453.0*/+17.98, /*5 2866.0*/+19.22, /*6 3280.0*/+20.47, /*7 3693.0*/+21.71, /*8 4106.0*/+22.96, /*9 4520.0*/+24.20, /*10 4933.0*/+25.44, /*11 5346.0*/+26.69, /*12 5760.0*/+27.93, /*13 6173.0*/+29.18, /*14 6586.0*/+30.42, /*15 7000.0*/+31.67},
//   60 {/*11 engineLoad=73.33*/  /*0 800.0*/+12.80, /*1 1213.0*/+14.04, /*2 1626.0*/+15.27, /*3 2040.0*/+16.51, /*4 2453.0*/+17.74, /*5 2866.0*/+18.98, /*6 3280.0*/+20.21, /*7 3693.0*/+21.45, /*8 4106.0*/+22.68, /*9 4520.0*/+23.92, /*10 4933.0*/+25.16, /*11 5346.0*/+26.39, /*12 5760.0*/+27.63, /*13 6173.0*/+28.86, /*14 6586.0*/+30.10, /*15 7000.0*/+31.33},
//   61 {/*12 engineLoad=80.00*/  /*0 800.0*/+12.60, /*1 1213.0*/+13.83, /*2 1626.0*/+15.05, /*3 2040.0*/+16.28, /*4 2453.0*/+17.51, /*5 2866.0*/+18.73, /*6 3280.0*/+19.96, /*7 3693.0*/+21.19, /*8 4106.0*/+22.41, /*9 4520.0*/+23.64, /*10 4933.0*/+24.87, /*11 5346.0*/+26.09, /*12 5760.0*/+27.32, /*13 6173.0*/+28.55, /*14 6586.0*/+29.77, /*15 7000.0*/+31.00},
//   62 {/*13 engineLoad=86.66*/  /*0 800.0*/+12.40, /*1 1213.0*/+13.62, /*2 1626.0*/+14.84, /*3 2040.0*/+16.05, /*4 2453.0*/+17.27, /*5 2866.0*/+18.49, /*6 3280.0*/+19.71, /*7 3693.0*/+20.92, /*8 4106.0*/+22.14, /*9 4520.0*/+23.36, /*10 4933.0*/+24.58, /*11 5346.0*/+25.80, /*12 5760.0*/+27.01, /*13 6173.0*/+28.23, /*14 6586.0*/+29.45, /*15 7000.0*/+30.67},
//   63 {/*14 engineLoad=93.33*/  /*0 800.0*/+12.20, /*1 1213.0*/+13.41, /*2 1626.0*/+14.62, /*3 2040.0*/+15.83, /*4 2453.0*/+17.04, /*5 2866.0*/+18.24, /*6 3280.0*/+19.45, /*7 3693.0*/+20.66, /*8 4106.0*/+21.87, /*9 4520.0*/+23.08, /*10 4933.0*/+24.29, /*11 5346.0*/+25.50, /*12 5760.0*/+26.71, /*13 6173.0*/+27.92, /*14 6586.0*/+29.12, /*15 7000.0*/+30.33},
//   64 {/*15 engineLoad=100.00*/ /*0 800.0*/+12.00, /*1 1213.0*/+13.20, /*2 1626.0*/+14.40, /*3 2040.0*/+15.60, /*4 2453.0*/+16.80, /*5 2866.0*/+18.00, /*6 3280.0*/+19.20, /*7 3693.0*/+20.40, /*8 4106.0*/+21.60, /*9 4520.0*/+22.80, /*10 4933.0*/+24.00, /*11 5346.0*/+25.20, /*12 5760.0*/+26.40, /*13 6173.0*/+27.60, /*14 6586.0*/+28.80, /*15 7000.0*/+30.00}
//   65 };
//   66 
//   67 //static const float rpmSteps[16] = {400, 800, 1200, 1600, 2000, 2400, 2800, 3200, 3600, 4000, 4400, 4800, 5200, 5600, 6000, 6400};
//   68 //static const float mapSteps[16] = {25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100};
//   69 
//   70 EXTERN_ENGINE;
//   71 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z36setCitroenBerlingoTU3JPConfigurationv
        THUMB
//   72 void setCitroenBerlingoTU3JPConfiguration(DECLARE_ENGINE_PARAMETER_F) {
_Z36setCitroenBerlingoTU3JPConfigurationv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   73 	engineConfiguration->engineType = CITROEN_TU3JP;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        MOVS     R1,#+15
        STR      R1,[R0, #+0]
//   74 
//   75 	/**
//   76 	 * Base engine setting
//   77 	 */
//   78 	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
        MOVS     R1,#+1
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
          CFI FunCall _Z16setOperationModeP22engine_configuration_s16operation_mode_e
        BL       _Z16setOperationModeP22engine_configuration_s16operation_mode_e
//   79 	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        MOVS     R1,#+8
        STR      R1,[R0, #+536]
//   80 	engineConfiguration->globalTriggerAngleOffset = 114;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable0_12  ;; 0x42e40000
        STR      R1,[R0, #+460]
//   81 	engineConfiguration->specs.cylindersCount = 4;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        MOVS     R1,#+4
        STR      R1,[R0, #+404]
//   82 	engineConfiguration->specs.displacement = 1.360;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable0_13  ;; 0x3fae147b
        STR      R1,[R0, #+400]
//   83 	engineConfiguration->specs.firingOrder = FO_1_THEN_3_THEN_4_THEN2;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+408]
//   84 	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+444]
//   85 	engineConfiguration->injectionMode = IM_BATCH;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+432]
//   86 	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+428]
//   87 	engineConfiguration->rpmHardLimit = 5000;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        MOVW     R1,#+5000
        STR      R1,[R0, #+420]
//   88 	engineConfiguration->cranking.rpm = 600;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        MOV      R1,#+600
        STRH     R1,[R0, #+96]
//   89 
//   90 //	memcpy(config->ve2RpmBins, rpmSteps, sizeof(rpmSteps));
//   91 
//   92 	/**
//   93 	* Cranking fuel setting
//   94 	* TODO: they recomend using something like -40C for low point and +80C for high point
//   95 	*/
//   96 	engineConfiguration->cranking.baseFuel = 15;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable0_14  ;; 0x41700000
        STR      R1,[R0, #+92]
//   97 
//   98 	/**
//   99 	 * Algorithm Alpha-N setting
//  100 	 */
//  101 	setAlgorithm(LM_ALPHA_N PASS_ENGINE_PARAMETER);
        MOVS     R0,#+1
          CFI FunCall _Z12setAlgorithm18engine_load_mode_e
        BL       _Z12setAlgorithm18engine_load_mode_e
//  102 	setFuelLoadBin(0, 100 PASS_ENGINE_PARAMETER);
        VLDR.W   S1,??DataTable0  ;; 0x42c80000
        VLDR.W   S0,??DataTable0_1  ;; 0x0
          CFI FunCall _Z14setFuelLoadBinff
        BL       _Z14setFuelLoadBinff
//  103 	setFuelRpmBin(800, 7000 PASS_ENGINE_PARAMETER);
        VLDR.W   S1,??DataTable0_2  ;; 0x45dac000
        VLDR.W   S0,??DataTable0_3  ;; 0x44480000
          CFI FunCall _Z13setFuelRpmBinff
        BL       _Z13setFuelRpmBinff
//  104 	setTimingRpmBin(800, 7000 PASS_ENGINE_PARAMETER);
        VLDR.W   S1,??DataTable0_2  ;; 0x45dac000
        VLDR.W   S0,??DataTable0_3  ;; 0x44480000
          CFI FunCall _Z15setTimingRpmBinff
        BL       _Z15setTimingRpmBinff
//  105 
//  106 	/**
//  107 	 * Outputs
//  108 	 */
//  109 
//  110 	// Frankenstein lo-side output #1: PC14 Igniter 1-4
//  111 	// Frankenstein lo-side output #2: PC15 Igniter 2-3
//  112 	// Frankenstein lo-side output #3: PE6  Injector 1-4
//  113 	// Frankenstein lo-side output #4: PC13 Injector 2-3
//  114 	// Frankenstein lo-side output #5: PE4
//  115 	// Frankenstein lo-side output #6: PE5
//  116 	// Frankenstein lo-side output #7: PE2
//  117 	// Frankenstein lo-side output #8: PE3
//  118 	// Frankenstein lo-side output #9: PE0	Fan
//  119 	// Frankenstein lo-side output #10: PE1	MIL
//  120 	// Frankenstein lo-side output #11: PB8	Main relay
//  121 	// Frankenstein lo-side output #12: PB9	Fuel pump
//  122 
//  123 	boardConfiguration->ignitionPins[0] = GPIOC_14;
        LDR.N    R0,??DataTable0_15
        LDR      R0,[R0, #+0]
        MOVS     R1,#+46
        STR      R1,[R0, #+84]
//  124 	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
        LDR.N    R0,??DataTable0_15
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+88]
//  125 	boardConfiguration->ignitionPins[2] = GPIOC_15;
        LDR.N    R0,??DataTable0_15
        LDR      R0,[R0, #+0]
        MOVS     R1,#+47
        STR      R1,[R0, #+92]
//  126 	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
        LDR.N    R0,??DataTable0_15
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+96]
//  127 
//  128 	engineConfiguration->injector.flow = 137; //SIEMENS DEKA VAZ20734
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable0_16  ;; 0x43090000
        STR      R1,[R0, #+8]
//  129 	boardConfiguration->injectionPins[0] = GPIOE_6;
        LDR.N    R0,??DataTable0_15
        LDR      R0,[R0, #+0]
        MOVS     R1,#+70
        STR      R1,[R0, #+32]
//  130 	boardConfiguration->injectionPins[1] = GPIOC_13;
        LDR.N    R0,??DataTable0_15
        LDR      R0,[R0, #+0]
        MOVS     R1,#+45
        STR      R1,[R0, #+36]
//  131 	boardConfiguration->injectionPins[2] = GPIO_UNASSIGNED;
        LDR.N    R0,??DataTable0_15
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+40]
//  132 	boardConfiguration->injectionPins[3] = GPIO_UNASSIGNED;
        LDR.N    R0,??DataTable0_15
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+44]
//  133 
//  134 	boardConfiguration->fanPin = GPIOE_0;
        LDR.N    R0,??DataTable0_15
        LDR      R0,[R0, #+0]
        MOVS     R1,#+64
        STR      R1,[R0, #+144]
//  135 	boardConfiguration->fanPinMode = OM_DEFAULT;
        LDR.N    R0,??DataTable0_15
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+148]
//  136 
//  137 	boardConfiguration->malfunctionIndicatorPin = GPIOE_1;
        LDR.N    R0,??DataTable0_15
        LDR      R0,[R0, #+0]
        MOVS     R1,#+65
        STR      R1,[R0, #+136]
//  138 	boardConfiguration->malfunctionIndicatorPinMode = OM_DEFAULT;
        LDR.N    R0,??DataTable0_15
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+140]
//  139 
//  140 	boardConfiguration->mainRelayPin = GPIOB_8;
        LDR.N    R0,??DataTable0_15
        LDR      R0,[R0, #+0]
        MOVS     R1,#+24
        STR      R1,[R0, #+308]
//  141 
//  142 	boardConfiguration->fuelPumpPin = GPIOB_9;
        LDR.N    R0,??DataTable0_15
        LDR      R0,[R0, #+0]
        MOVS     R1,#+25
        STR      R1,[R0, #+24]
//  143 	boardConfiguration->fuelPumpPinMode = OM_DEFAULT;
        LDR.N    R0,??DataTable0_15
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+28]
//  144 
//  145 	setLCD(boardConfiguration);
        LDR.N    R0,??DataTable0_15
        LDR      R0,[R0, #+0]
          CFI FunCall _Z6setLCDP21board_configuration_s
        BL       _Z6setLCDP21board_configuration_s
//  146 
//  147 
//  148 //	boardConfiguration->o2heaterPin = GPIOC_13;
//  149 //	boardConfiguration->logicAnalyzerPins[1] = GPIO_UNASSIGNED;
//  150 
//  151 	/**
//  152 	 * Inputs
//  153 	 */
//  154 
//  155 	// See https://docs.google.com/spreadsheet/ccc?key=0Arl1FeMZcfisdEdGdUlHdWh6cVBoSzFIbkxqa1QtZ3c
//  156 	// Frankenstein analog input #1: PA1		adc1	MAP
//  157 	// Frankenstein analog input #2: PA3		adc3	TPS
//  158 	// Frankenstein analog input #3: PC3		adc13	IAT
//  159 	// Frankenstein analog input #4: PC1		adc11	CLT
//  160 	// Frankenstein analog input #5: PA0		adc0	vBatt
//  161 	// Frankenstein analog input #6: PC2		adc12	WBO
//  162 	// Frankenstein analog input #7: PA4		adc4
//  163 	// Frankenstein analog input #8: PA2		adc2
//  164 	// Frankenstein analog input #9: PA6		adc6
//  165 	// Frankenstein analog input #10: PA7		adc7
//  166 	// Frankenstein analog input #11: PC4		adc14
//  167 	// Frankenstein analog input #12: PC5|PA8	adc15	Speed Sensor
//  168 
//  169 	/**
//  170 	 * MAP <BOSCH 0 261 230 057>
//  171 	 */
//  172 	engineConfiguration->map.sensor.hwChannel = EFI_ADC_1;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+252]
//  173 	engineConfiguration->map.sensor.type = MT_CUSTOM;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+248]
//  174 	engineConfiguration->map.sensor.valueAt0 = 10;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable0_17  ;; 0x41200000
        STR      R1,[R0, #+240]
//  175 	engineConfiguration->map.sensor.valueAt5 = 110;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable0_18  ;; 0x42dc0000
        STR      R1,[R0, #+244]
//  176 	/**
//  177 	 * TPS <MAGNETI MARELLI>
//  178 	 */
//  179 	engineConfiguration->tpsAdcChannel = EFI_ADC_3;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        MOVS     R1,#+3
        STR      R1,[R0, #+524]
//  180 	engineConfiguration->tpsMin = 108; // convert 12to10 bit (ADC/4)
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        MOVS     R1,#+108
        STRH     R1,[R0, #+82]
//  181 	engineConfiguration->tpsMax = 812; // convert 12to10 bit (ADC/4)
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        MOV      R1,#+812
        STRH     R1,[R0, #+84]
//  182 	/**
//  183 	 * IAT <OEM ECU>
//  184 	 */
//  185 	engineConfiguration->iat.adcChannel = EFI_ADC_13;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        MOVS     R1,#+13
        STR      R1,[R0, #+316]
//  186 	setThermistorConfiguration(&engineConfiguration->iat, -20.0, 15600.0, 23.0, 2250.0, 92.0, 240.0);
        VLDR.W   S5,??DataTable0_4  ;; 0x43700000
        VLDR.W   S4,??DataTable0_5  ;; 0x42b80000
        VLDR.W   S3,??DataTable0_6  ;; 0x450ca000
        VMOV.F32 S2,#23.0
        VLDR.W   S1,??DataTable0_7  ;; 0x4673c000
        VMOV.F32 S0,#-20.0
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+288
          CFI FunCall _Z26setThermistorConfigurationP14ThermistorConfffffff
        BL       _Z26setThermistorConfigurationP14ThermistorConfffffff
//  187 	engineConfiguration->iat.config.bias_resistor = 2660;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable0_19  ;; 0x45264000
        STR      R1,[R0, #+312]
//  188 	/**
//  189 	* CLT <LADA Samara>
//  190 	*/
//  191 	engineConfiguration->clt.adcChannel = EFI_ADC_11;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        MOVS     R1,#+11
        STR      R1,[R0, #+284]
//  192 	setThermistorConfiguration(&engineConfiguration->clt, -20.0, 28680.0, 25.0, 2796.0, 100.0, 177.0);
        VLDR.W   S5,??DataTable0_8  ;; 0x43310000
        VLDR.W   S4,??DataTable0  ;; 0x42c80000
        VLDR.W   S3,??DataTable0_9  ;; 0x452ec000
        VMOV.F32 S2,#25.0
        VLDR.W   S1,??DataTable0_10  ;; 0x46e01000
        VMOV.F32 S0,#-20.0
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+256
          CFI FunCall _Z26setThermistorConfigurationP14ThermistorConfffffff
        BL       _Z26setThermistorConfigurationP14ThermistorConfffffff
//  193 	engineConfiguration->iat.config.bias_resistor = 2660;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable0_19  ;; 0x45264000
        STR      R1,[R0, #+312]
//  194 	/**
//  195 	 * vBatt
//  196 	 */
//  197 	engineConfiguration->vbattAdcChannel = EFI_ADC_0;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+472]
//  198 	engineConfiguration->vbattDividerCoeff = ((float) (2.6 + 10.1)) / 2.6 * 2;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable0_20  ;; 0x411c4ec5
        STR      R1,[R0, #+468]
//  199 	/**
//  200 	* WBO Innovate LC-1
//  201 	*/
//  202 	engineConfiguration->afr.hwChannel = EFI_ADC_12;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        MOVS     R1,#+12
        STR      R1,[R0, #+584]
//  203 	/**
//  204 	* Speed Sensor
//  205 	*/
//  206 	boardConfiguration->vehicleSpeedSensorInputPin = GPIOA_8;
        LDR.N    R0,??DataTable0_15
        LDR      R0,[R0, #+0]
        MOVS     R1,#+8
        STR      R1,[R0, #+792]
//  207 	engineConfiguration->hasVehicleSpeedSensor = true;
        LDR.N    R0,??DataTable0_11
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        ORRS     R0,R0,#0x4
        LDR.N    R1,??DataTable0_11
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1497]
//  208 	/**
//  209 	* Other
//  210 	*/
//  211 //	engineConfiguration->mafAdcChannel = GPIO_UNASSIGNED;
//  212 
//  213 	copyFuelTable(tps_fuel_table, config->fuelTable);
        LDR.N    R0,??DataTable0_21
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+10496
        ADDS     R1,R1,#+184
        LDR.N    R0,??DataTable0_22
          CFI FunCall _Z13copyFuelTablePA16_KfPA16_f
        BL       _Z13copyFuelTablePA16_KfPA16_f
//  214 	copyTimingTable(tps_advance_table, config->ignitionTable);
        LDR.N    R0,??DataTable0_21
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+11776
        ADDS     R1,R1,#+56
        LDR.N    R0,??DataTable0_23
          CFI FunCall _Z15copyTimingTablePA16_KfPA16_f
        BL       _Z15copyTimingTablePA16_KfPA16_f
//  215 
//  216 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0:
        DC32     0x42c80000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_1:
        DC32     0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_2:
        DC32     0x45dac000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_3:
        DC32     0x44480000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_4:
        DC32     0x43700000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_5:
        DC32     0x42b80000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_6:
        DC32     0x450ca000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_7:
        DC32     0x4673c000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_8:
        DC32     0x43310000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_9:
        DC32     0x452ec000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_10:
        DC32     0x46e01000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_11:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_12:
        DC32     0x42e40000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_13:
        DC32     0x3fae147b

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_14:
        DC32     0x41700000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_15:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_16:
        DC32     0x43090000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_17:
        DC32     0x41200000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_18:
        DC32     0x42dc0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_19:
        DC32     0x45264000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_20:
        DC32     0x411c4ec5

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_21:
        DC32     config

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_22:
        DC32     tps_fuel_table

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_23:
        DC32     tps_advance_table

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
// 2 048 bytes in section .rodata
//   732 bytes in section .text
// 
//   732 bytes of CODE  memory
// 2 048 bytes of CONST memory
//
//Errors: none
//Warnings: 1
