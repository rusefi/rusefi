///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:53 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       mazda_miata.cpp                                      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       mazda_miata.cpp -lCN F:\stuff\rusefi_sourceforge\fir /
//                       mware\iar\Debug\List\ -lA                            /
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
//                       mazda_miata.s                                        /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME mazda_miata

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z12setDensoTODOP19persistent_config_s
        EXTERN _Z12setEgoSensor12ego_sensor_e
        EXTERN _Z13copyFuelTablePA16_KfPA16_f
        EXTERN _Z13setFuelRpmBinff
        EXTERN _Z14setFuelLoadBinff
        EXTERN _Z15copyTimingTablePA16_KfPA16_f
        EXTERN _Z15setWholeFuelMapf
        EXTERN _Z18setCommonNTCSensorP14ThermistorConf
        EXTERN _Z18setMazdaMiataNAMafP19persistent_config_s
        EXTERN _Z18setSingleCoilDwellP22engine_configuration_s
        EXTERN _Z19setFrankenso_01_LCDP21board_configuration_s
        EXTERN _Z19setWholeTimingTablef
        EXTERN _Z24setFrankenso0_1_joystickP22engine_configuration_s
        EXTERN _Z27commonFrankensoAnalogInputsP22engine_configuration_s
        EXTERN boardConfiguration
        EXTERN config
        EXTERN engineConfiguration

        PUBLIC _Z12setMiata1990v
        PUBLIC _Z12setMiata1996v
        PUBLIC _Z14setMiata1994_dv
        PUBLIC _Z14setMiata1994_sv
        PUBLIC _Z15setFordEscortGtv
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\config\engines\mazda_miata.cpp
//    1 /**
//    2  * @file	mazda_miata.cpp
//    3  *
//    4  * FORD_ESCORT_GT = 14
//    5  * set_engine_type 14
//    6  * http://rusefi.com/wiki/index.php?title=Vehicle:Mazda_Protege_1993
//    7  *
//    8  * MIATA_1990 = 19
//    9  * MIATA_1994_DEVIATOR = 20
//   10  * MIATA_1996 = 21
//   11  * MIATA_1994_SPAGS = 24
//   12  * set_engine_type 24
//   13  *
//   14  * @date Apr 11, 2014
//   15  * @author Andrey Belomutskiy, (c) 2012-2015
//   16  */
//   17 
//   18 #include "fsio_impl.h"
//   19 #include "mazda_miata.h"
//   20 #include "engine_math.h"
//   21 #include "advance_map.h"
//   22 #include "allsensors.h"
//   23 #include "honda_accord.h"
//   24 

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//   25 static const fuel_table_t miata_maf_fuel_table = { {/*0 engineLoad=1.2*//*0 800.0*/1.53, /*1 1213.0*/0.92, /*2 1626.0*/
miata_maf_fuel_table:
        DATA
        DC32 3FC3D70AH, 3F6B851FH, 3F3D70A4H, 3F30A3D7H, 3F30A3D7H, 3F2B851FH
        DC32 3F2B851FH, 3F2B851FH, 3F2B851FH, 3F828F5CH, 3F7AE148H, 3F7AE148H
        DC32 3F6B851FH, 3F63D70AH, 3F51EB85H, 3F5EB852H, 403EB852H, 40047AE1H
        DC32 3FDEB852H, 3FC66666H, 3FB70A3DH, 3F970A3DH, 0H, 0H, 0H, 0H, 0H, 0H
        DC32 0H, 0H, 0H, 0H, 409CCCCDH, 405CCCCDH, 4030A3D7H, 40166666H
        DC32 40051EB8H, 3FEB851FH, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H
        DC32 40DE147BH, 4099EB85H, 40728F5CH, 4048F5C3H, 402E147BH, 401AE148H
        DC32 3FF70A3DH, 3FE66666H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 413F0A3DH
        DC32 40D570A4H, 40A4CCCDH, 40875C29H, 406851ECH, 404C28F6H, 40351EB8H
        DC32 4025C28FH, 4018F5C3H, 400F5C29H, 0H, 0H, 0H, 0H, 0H, 0H, 414A147BH
        DC32 412828F6H, 40DD1EB8H, 40B428F6H, 4099999AH, 4086B852H, 40700000H
        DC32 4059999AH, 4045C28FH, 4037AE14H, 40333333H, 402B851FH, 402147AEH
        DC32 401851ECH, 40128F5CH, 0H, 4141C28FH, 41480000H, 4120A3D7H
        DC32 40E7AE14H, 40C3851FH, 40AA3D71H, 40975C29H, 4088F5C3H, 4078F5C3H
        DC32 40670A3DH, 40628F5CH, 40547AE1H, 404851ECH, 403AE148H, 40347AE1H
        DC32 0H, 414170A4H, 41430A3DH, 4146E148H, 412B0A3DH, 40FD70A4H
        DC32 40DBD70AH, 40C2E148H, 40AFAE14H, 409F0A3DH, 4092E148H, 408E147BH
        DC32 4086B852H, 407E147BH, 406CCCCDH, 40633333H, 0H, 414170A4H
        DC32 4142E148H, 414570A4H, 4147AE14H, 4135C28FH, 4117AE14H, 40F5C28FH
        DC32 40DC7AE1H, 40C75C29H, 40B851ECH, 40B4CCCDH, 40AA3D71H, 409F0A3DH
        DC32 40933333H, 408CCCCDH, 0H, 414147AEH, 4142B852H, 414570A4H
        DC32 41468F5CH, 414C28F6H, 4147851FH, 413051ECH, 411AB852H, 4104A3D7H
        DC32 40E75C29H, 40E00000H, 40DA3D71H, 40CE147BH, 40C1EB85H, 40B851ECH
        DC32 0H, 414147AEH, 4142E148H, 414570A4H, 4146E148H, 414B3333H
        DC32 414EE148H, 414C7AE1H, 41466666H, 41366666H, 41228F5CH, 41170A3DH
        DC32 411028F6H, 4105C28FH, 40FA8F5CH, 40EE6666H, 0H, 414170A4H
        DC32 4142B852H, 414570A4H, 4146E148H, 414B3333H, 414E3D71H, 414B851FH
        DC32 4148CCCDH, 414FAE14H, 414F5C29H, 4144CCCDH, 413970A4H, 412E6666H
        DC32 41211EB8H, 411970A4H, 0H, 414147AEH, 4142E148H, 414570A4H
        DC32 4146E148H, 414B3333H, 414EE148H, 414B851FH, 414A147BH, 414DEB85H
        DC32 4158F5C3H, 416C28F6H, 4169EB85H, 41623D71H, 41563D71H, 41430A3DH
        DC32 0H, 414170A4H, 4142B852H, 414570A4H, 4146E148H, 414B5C29H
        DC32 414EB852H, 414BD70AH, 41491EB8H, 414E147BH, 415D1EB8H, 41726666H
        DC32 41791EB8H, 4181C28FH, 41806666H, 417970A4H, 0H, 414147AEH
        DC32 4142E148H, 414570A4H, 41466666H, 414B5C29H, 414EB852H, 414B5C29H
        DC32 41487AE1H, 414F851FH, 4158CCCDH, 416DEB85H, 417AB852H, 418147AEH
        DC32 4181999AH, 417E3D71H, 0H, 414147AEH, 4142B852H, 414570A4H
        DC32 4146E148H, 414BD70AH, 414E8F5CH, 414B5C29H, 4148A3D7H, 414F851FH
        DC32 41587AE1H, 416DEB85H, 4177AE14H, 4180CCCDH, 418170A4H, 417EE148H
        DC32 0H
//   26 		0.74, /*3 2040.0*/0.69, /*4 2453.0*/0.69, /*5 2866.0*/0.67, /*6 3280.0*/0.67, /*7 3693.0*/0.67, /*8 4106.0*/
//   27 		0.67, /*9 4520.0*/1.02, /*10 4933.0*/0.98, /*11 5346.0*/0.98, /*12 5760.0*/0.92, /*13 6173.0*/0.89, /*14 6586.0*/
//   28 		0.82, /*15 7000.0*/0.87 }, {/*1 engineLoad=1.413333*//*0 800.0*/2.98, /*1 1213.0*/2.07, /*2 1626.0*/1.74, /*3 2040.0*/
//   29 		1.55, /*4 2453.0*/1.43, /*5 2866.0*/1.18, /*6 3280.0*/0.0, /*7 3693.0*/0.0, /*8 4106.0*/0.0, /*9 4520.0*/0.0, /*10 4933.0*/
//   30 		0.0, /*11 5346.0*/0.0, /*12 5760.0*/0.0, /*13 6173.0*/0.0, /*14 6586.0*/0.0, /*15 7000.0*/0.0 }, {/*2 engineLoad=1.626666*//*0 800.0*/
//   31 		4.9, /*1 1213.0*/3.45, /*2 1626.0*/2.76, /*3 2040.0*/2.35, /*4 2453.0*/2.08, /*5 2866.0*/1.84, /*6 3280.0*/0.0, /*7 3693.0*/
//   32 		0.0, /*8 4106.0*/0.0, /*9 4520.0*/0.0, /*10 4933.0*/0.0, /*11 5346.0*/0.0, /*12 5760.0*/0.0, /*13 6173.0*/0.0, /*14 6586.0*/
//   33 		0.0, /*15 7000.0*/0.0 }, {/*3 engineLoad=1.839999*//*0 800.0*/6.94, /*1 1213.0*/4.81, /*2 1626.0*/3.79, /*3 2040.0*/
//   34 		3.14, /*4 2453.0*/2.72, /*5 2866.0*/2.42, /*6 3280.0*/1.93, /*7 3693.0*/1.8, /*8 4106.0*/0.0, /*9 4520.0*/0.0, /*10 4933.0*/
//   35 		0.0, /*11 5346.0*/0.0, /*12 5760.0*/0.0, /*13 6173.0*/0.0, /*14 6586.0*/0.0, /*15 7000.0*/0.0 }, {/*4 engineLoad=2.053332*//*0 800.0*/
//   36 		11.94, /*1 1213.0*/6.67, /*2 1626.0*/5.15, /*3 2040.0*/4.23, /*4 2453.0*/3.63, /*5 2866.0*/3.19, /*6 3280.0*/
//   37 		2.83, /*7 3693.0*/2.59, /*8 4106.0*/2.39, /*9 4520.0*/2.24, /*10 4933.0*/0.0, /*11 5346.0*/0.0, /*12 5760.0*/
//   38 		0.0, /*13 6173.0*/0.0, /*14 6586.0*/0.0, /*15 7000.0*/0.0 }, {/*5 engineLoad=2.266665*//*0 800.0*/12.63, /*1 1213.0*/
//   39 		10.51, /*2 1626.0*/6.91, /*3 2040.0*/5.63, /*4 2453.0*/4.8, /*5 2866.0*/4.21, /*6 3280.0*/3.75, /*7 3693.0*/3.4, /*8 4106.0*/
//   40 		3.09, /*9 4520.0*/2.87, /*10 4933.0*/2.8, /*11 5346.0*/2.68, /*12 5760.0*/2.52, /*13 6173.0*/2.38, /*14 6586.0*/
//   41 		2.29, /*15 7000.0*/0.0 }, {/*6 engineLoad=2.479998*//*0 800.0*/12.11, /*1 1213.0*/12.5, /*2 1626.0*/10.04, /*3 2040.0*/
//   42 		7.24, /*4 2453.0*/6.11, /*5 2866.0*/5.32, /*6 3280.0*/4.73, /*7 3693.0*/4.28, /*8 4106.0*/3.89, /*9 4520.0*/
//   43 		3.61, /*10 4933.0*/3.54, /*11 5346.0*/3.32, /*12 5760.0*/3.13, /*13 6173.0*/2.92, /*14 6586.0*/2.82, /*15 7000.0*/
//   44 		0.0 }, {/*7 engineLoad=2.693331*//*0 800.0*/12.09, /*1 1213.0*/12.19, /*2 1626.0*/12.43, /*3 2040.0*/10.69, /*4 2453.0*/
//   45 		7.92, /*5 2866.0*/6.87, /*6 3280.0*/6.09, /*7 3693.0*/5.49, /*8 4106.0*/4.97, /*9 4520.0*/4.59, /*10 4933.0*/
//   46 		4.44, /*11 5346.0*/4.21, /*12 5760.0*/3.97, /*13 6173.0*/3.7, /*14 6586.0*/3.55, /*15 7000.0*/0.0 }, {/*8 engineLoad=2.906664*//*0 800.0*/
//   47 		12.09, /*1 1213.0*/12.18, /*2 1626.0*/12.34, /*3 2040.0*/12.48, /*4 2453.0*/11.36, /*5 2866.0*/9.48, /*6 3280.0*/
//   48 		7.68, /*7 3693.0*/6.89, /*8 4106.0*/6.23, /*9 4520.0*/5.76, /*10 4933.0*/5.65, /*11 5346.0*/5.32, /*12 5760.0*/
//   49 		4.97, /*13 6173.0*/4.6, /*14 6586.0*/4.4, /*15 7000.0*/0.0 }, {/*9 engineLoad=3.119997*//*0 800.0*/12.08, /*1 1213.0*/
//   50 		12.17, /*2 1626.0*/12.34, /*3 2040.0*/12.41, /*4 2453.0*/12.76, /*5 2866.0*/12.47, /*6 3280.0*/11.02, /*7 3693.0*/
//   51 		9.67, /*8 4106.0*/8.29, /*9 4520.0*/7.23, /*10 4933.0*/7.0, /*11 5346.0*/6.82, /*12 5760.0*/6.44, /*13 6173.0*/
//   52 		6.06, /*14 6586.0*/5.76, /*15 7000.0*/0.0 }, {/*10 engineLoad=3.33333*//*0 800.0*/12.08, /*1 1213.0*/12.18, /*2 1626.0*/
//   53 		12.34, /*3 2040.0*/12.43, /*4 2453.0*/12.7, /*5 2866.0*/12.93, /*6 3280.0*/12.78, /*7 3693.0*/12.4, /*8 4106.0*/
//   54 		11.4, /*9 4520.0*/10.16, /*10 4933.0*/9.44, /*11 5346.0*/9.01, /*12 5760.0*/8.36, /*13 6173.0*/7.83, /*14 6586.0*/
//   55 		7.45, /*15 7000.0*/0.0 }, {/*11 engineLoad=3.546663*//*0 800.0*/12.09, /*1 1213.0*/12.17, /*2 1626.0*/12.34, /*3 2040.0*/
//   56 		12.43, /*4 2453.0*/12.7, /*5 2866.0*/12.89, /*6 3280.0*/12.72, /*7 3693.0*/12.55, /*8 4106.0*/12.98, /*9 4520.0*/
//   57 		12.96, /*10 4933.0*/12.3, /*11 5346.0*/11.59, /*12 5760.0*/10.9, /*13 6173.0*/10.07, /*14 6586.0*/9.59, /*15 7000.0*/
//   58 		0.0 }, {/*12 engineLoad=3.759996*//*0 800.0*/12.08, /*1 1213.0*/12.18, /*2 1626.0*/12.34, /*3 2040.0*/12.43, /*4 2453.0*/
//   59 		12.7, /*5 2866.0*/12.93, /*6 3280.0*/12.72, /*7 3693.0*/12.63, /*8 4106.0*/12.87, /*9 4520.0*/13.56, /*10 4933.0*/
//   60 		14.76, /*11 5346.0*/14.62, /*12 5760.0*/14.14, /*13 6173.0*/13.39, /*14 6586.0*/12.19, /*15 7000.0*/0.0 }, {/*13 engineLoad=3.973329*//*0 800.0*/
//   61 		12.09, /*1 1213.0*/12.17, /*2 1626.0*/12.34, /*3 2040.0*/12.43, /*4 2453.0*/12.71, /*5 2866.0*/12.92, /*6 3280.0*/
//   62 		12.74, /*7 3693.0*/12.57, /*8 4106.0*/12.88, /*9 4520.0*/13.82, /*10 4933.0*/15.15, /*11 5346.0*/15.57, /*12 5760.0*/
//   63 		16.22, /*13 6173.0*/16.05, /*14 6586.0*/15.59, /*15 7000.0*/0.0 }, {/*14 engineLoad=4.186662*//*0 800.0*/12.08, /*1 1213.0*/
//   64 		12.18, /*2 1626.0*/12.34, /*3 2040.0*/12.4, /*4 2453.0*/12.71, /*5 2866.0*/12.92, /*6 3280.0*/12.71, /*7 3693.0*/
//   65 		12.53, /*8 4106.0*/12.97, /*9 4520.0*/13.55, /*10 4933.0*/14.87, /*11 5346.0*/15.67, /*12 5760.0*/16.16, /*13 6173.0*/
//   66 		16.2, /*14 6586.0*/15.89, /*15 7000.0*/0.0 }, {/*15 engineLoad=4.399995*//*0 800.0*/12.08, /*1 1213.0*/12.17, /*2 1626.0*/
//   67 		12.34, /*3 2040.0*/12.43, /*4 2453.0*/12.74, /*5 2866.0*/12.91, /*6 3280.0*/12.71, /*7 3693.0*/12.54, /*8 4106.0*/
//   68 		12.97, /*9 4520.0*/13.53, /*10 4933.0*/14.87, /*11 5346.0*/15.48, /*12 5760.0*/16.1, /*13 6173.0*/16.18, /*14 6586.0*/
//   69 		15.93, /*15 7000.0*/0.0 } };
//   70 

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//   71 static const ignition_table_t miata_maf_advance_table = { {/*0  engineLoad=1.200*//*0 800.0*/+4.498, /*1 1213.0*/+11.905, /*2 1626.0*/
miata_maf_advance_table:
        DATA
        DC32 408FEF9EH, 413E7AE1H, 41BB5810H, 41CADB23H, 41CB872BH, 41CBBE77H
        DC32 41EB6666H, 4202DA1DH, 420E3958H, 42166042H, 4210A8F6H, 41F49FBEH
        DC32 41E928F6H, 41E8851FH, 41D89168H, 41E24189H, 409BD70AH, 4142353FH
        DC32 41BB1CACH, 41CC020CH, 41CB872BH, 41CBBE77H, 41E90000H, 42044BC7H
        DC32 4208CFDFH, 42171375H, 420F9893H, 41F426E9H, 41E70A3DH, 41E5EB85H
        DC32 41D98312H, 41DE9BA6H, 409A24DDH, 41443127H, 41BF6666H, 41CC020CH
        DC32 41CC0000H, 41CBBE77H, 41EAE979H, 4205F4BCH, 420B5B23H, 42162E14H
        DC32 420F8000H, 41F2D2F2H, 41E86A7FH, 41E2F5C3H, 41D89375H, 41D69FBEH
        DC32 40912F1BH, 4146BC6AH, 41B9B646H, 41CB26E9H, 41CB4BC7H, 41CBE148H
        DC32 41EB6666H, 4205B53FH, 42085D2FH, 42138C4AH, 42103021H, 41F0A1CBH
        DC32 41E39FBEH, 41E0978DH, 41D98312H, 41D520C5H, 4090B439H, 413C28F6H
        DC32 41BF51ECH, 41CB51ECH, 41CC6873H, 41C91EB8H, 41EAC49CH, 4203AB02H
        DC32 420B428FH, 4214D810H, 420F449CH, 41ED8D50H, 41E6645AH, 41DE8B44H
        DC32 41E0C8B4H, 41DD4BC7H, 4095B22DH, 413E978DH, 41BBE354H, 41CB0831H
        DC32 41CC6873H, 41CC374CH, 41EED917H, 42060B44H, 420B24DDH, 4217DA1DH
        DC32 420FAB02H, 41F89581H, 41E2E354H, 41E3BE77H, 41D98106H, 41D5D4FEH
        DC32 40908B44H, 414076C9H, 41BBD0E5H, 41CBA3D7H, 41CB0E56H, 41CF2F1BH
        DC32 41E92D0EH, 42064FDFH, 42087AE1H, 42111DB2H, 4211BA5EH, 41FAE560H
        DC32 41E0AC08H, 41E3B439H, 41DD872BH, 41DEA3D7H, 40910625H, 41443127H
        DC32 41BF7AE1H, 41CBE979H, 41CCC28FH, 41D6872BH, 41F2020CH, 420621CBH
        DC32 420B2D0EH, 4217872BH, 42106A7FH, 41F0A1CBH, 41E45C29H, 41EA6E98H
        DC32 41D88F5CH, 41E2978DH, 40910625H, 41409375H, 41BB5810H, 41CC1AA0H
        DC32 41CB0E56H, 41CADB23H, 41EF78D5H, 4205DE35H, 420AFDF4H, 421526E9H
        DC32 4211851FH, 41EC20C5H, 41E6645AH, 41E34189H, 41D79BA6H, 41E19FBEH
        DC32 40910625H, 413FA5E3H, 41BDD70AH, 41CC020CH, 41CCFDF4H, 41D29FBEH
        DC32 41F01EB8H, 4204BF7DH, 420B8625H, 42182D0EH, 420F3D71H, 41EEBE77H
        DC32 41EA72B0H, 41E7F9DBH, 41D8DF3BH, 41EAB646H, 4090DD2FH, 41421893H
        DC32 41BBE354H, 41CB70A4H, 41CC6873H, 41D2353FH, 41ED1CACH, 4204051FH
        DC32 42090312H, 421726E9H, 42104ED9H, 41F18106H, 41E8B22DH, 41E3D917H
        DC32 41DAC8B4H, 41DE374CH, 409C72B0H, 4142CCCDH, 41BA2B02H, 41CADB23H
        DC32 41CC6873H, 41CBE148H, 41EF3127H, 4203353FH, 420B24DDH, 421ABE77H
        DC32 420EE354H, 41FAC6A8H, 41E720C5H, 41E2A9FCH, 41D93127H, 41D9E354H
        DC32 40912F1BH, 41412B02H, 41BF2B02H, 41CC3333H, 41CCC28FH, 41CB9BA6H
        DC32 41F36C8BH, 4206DB23H, 420851ECH, 42161AA0H, 420F449CH, 41F5DD2FH
        DC32 41E5BE77H, 41E424DDH, 41DC24DDH, 41D47CEEH, 409B851FH, 413E20C5H
        DC32 41BB6C8BH, 41CBE979H, 41CC49BAH, 41CCE76DH, 41EF28F6H, 4206978DH
        DC32 420B0831H, 4217F3B6H, 42103021H, 41F1CCCDH, 41E59581H, 41E3E560H
        DC32 41D8E354H, 41DDA9FCH, 4095DB23H, 413E5E35H, 41BC0C4AH, 41CC7EFAH
        DC32 41CCE148H, 41CC26E9H, 41EF999AH, 420653F8H, 420A3127H, 4210CDD3H
        DC32 420F26E9H, 41F06C8BH, 41E25604H, 41E2126FH, 41DA26E9H, 41D6E76DH
        DC32 409B3333H, 4143D70AH, 41C0BA5EH, 41CB26E9H, 41CA9581H, 41CC5A1DH
        DC32 41EF51ECH, 42046A7FH, 42101062H, 4217BB64H, 42110419H, 41EDB22DH
        DC32 41E5353FH, 41E05C29H, 41D9D2F2H, 41DE45A2H
//   72 		+23.418, /*3 2040.0*/+25.357, /*4 2453.0*/+25.441, /*5 2866.0*/+25.468, /*6 3280.0*/+29.425, /*7 3693.0*/
//   73 		+32.713, /*8 4106.0*/+35.556, /*9 4520.0*/+37.594, /*10 4933.0*/+36.165, /*11 5346.0*/+30.578, /*12 5760.0*/
//   74 		+29.145, /*13 6173.0*/+29.065, /*14 6586.0*/+27.071, /*15 7000.0*/+28.282 }, {/*1  engineLoad=1.413*//*0 800.0*/
//   75 		+4.87, /*1 1213.0*/+12.138, /*2 1626.0*/+23.389, /*3 2040.0*/+25.501, /*4 2453.0*/+25.441, /*5 2866.0*/+25.468, /*6 3280.0*/
//   76 		+29.125, /*7 3693.0*/+33.074, /*8 4106.0*/+34.203, /*9 4520.0*/+37.769, /*10 4933.0*/+35.899, /*11 5346.0*/
//   77 		+30.519, /*12 5760.0*/+28.88, /*13 6173.0*/+28.74, /*14 6586.0*/+27.189, /*15 7000.0*/+27.826 }, {/*2  engineLoad=1.626*//*0 800.0*/
//   78 		+4.817, /*1 1213.0*/+12.262, /*2 1626.0*/+23.925, /*3 2040.0*/+25.501, /*4 2453.0*/+25.5, /*5 2866.0*/+25.468, /*6 3280.0*/
//   79 		+29.364, /*7 3693.0*/+33.489, /*8 4106.0*/+34.839, /*9 4520.0*/+37.545, /*10 4933.0*/+35.875, /*11 5346.0*/
//   80 		+30.353, /*12 5760.0*/+29.052, /*13 6173.0*/+28.37, /*14 6586.0*/+27.072, /*15 7000.0*/+26.828 }, {/*3  engineLoad=1.839*//*0 800.0*/
//   81 		+4.537, /*1 1213.0*/+12.421, /*2 1626.0*/+23.214, /*3 2040.0*/+25.394, /*4 2453.0*/+25.412, /*5 2866.0*/+25.485, /*6 3280.0*/
//   82 		+29.425, /*7 3693.0*/+33.427, /*8 4106.0*/+34.091, /*9 4520.0*/+36.887, /*10 4933.0*/+36.047, /*11 5346.0*/
//   83 		+30.079, /*12 5760.0*/+28.453, /*13 6173.0*/+28.074, /*14 6586.0*/+27.189, /*15 7000.0*/+26.641 }, {/*4  engineLoad=2.053*//*0 800.0*/
//   84 		+4.522, /*1 1213.0*/+11.76, /*2 1626.0*/+23.915, /*3 2040.0*/+25.415, /*4 2453.0*/+25.551, /*5 2866.0*/+25.14, /*6 3280.0*/
//   85 		+29.346, /*7 3693.0*/+32.917, /*8 4106.0*/+34.815, /*9 4520.0*/+37.211, /*10 4933.0*/+35.817, /*11 5346.0*/
//   86 		+29.694, /*12 5760.0*/+28.799, /*13 6173.0*/+27.818, /*14 6586.0*/+28.098, /*15 7000.0*/+27.662 }, {/*5  engineLoad=2.266*//*0 800.0*/
//   87 		+4.678, /*1 1213.0*/+11.912, /*2 1626.0*/+23.486, /*3 2040.0*/+25.379, /*4 2453.0*/+25.551, /*5 2866.0*/+25.527, /*6 3280.0*/
//   88 		+29.856, /*7 3693.0*/+33.511, /*8 4106.0*/+34.786, /*9 4520.0*/+37.963, /*10 4933.0*/+35.917, /*11 5346.0*/
//   89 		+31.073, /*12 5760.0*/+28.361, /*13 6173.0*/+28.468, /*14 6586.0*/+27.188, /*15 7000.0*/+26.729 }, {/*6  engineLoad=2.479*//*0 800.0*/
//   90 		+4.517, /*1 1213.0*/+12.029, /*2 1626.0*/+23.477, /*3 2040.0*/+25.455, /*4 2453.0*/+25.382, /*5 2866.0*/+25.898, /*6 3280.0*/
//   91 		+29.147, /*7 3693.0*/+33.578, /*8 4106.0*/+34.12, /*9 4520.0*/+36.279, /*10 4933.0*/+36.432, /*11 5346.0*/
//   92 		+31.362, /*12 5760.0*/+28.084, /*13 6173.0*/+28.463, /*14 6586.0*/+27.691, /*15 7000.0*/+27.83 }, {/*7  engineLoad=2.693*//*0 800.0*/
//   93 		+4.532, /*1 1213.0*/+12.262, /*2 1626.0*/+23.935, /*3 2040.0*/+25.489, /*4 2453.0*/+25.595, /*5 2866.0*/+26.816, /*6 3280.0*/
//   94 		+30.251, /*7 3693.0*/+33.533, /*8 4106.0*/+34.794, /*9 4520.0*/+37.882, /*10 4933.0*/+36.104, /*11 5346.0*/
//   95 		+30.079, /*12 5760.0*/+28.545, /*13 6173.0*/+29.304, /*14 6586.0*/+27.07, /*15 7000.0*/+28.324 }, {/*8  engineLoad=2.906*//*0 800.0*/
//   96 		+4.532, /*1 1213.0*/+12.036, /*2 1626.0*/+23.418, /*3 2040.0*/+25.513, /*4 2453.0*/+25.382, /*5 2866.0*/+25.357, /*6 3280.0*/
//   97 		+29.934, /*7 3693.0*/+33.467, /*8 4106.0*/+34.748, /*9 4520.0*/+37.288, /*10 4933.0*/+36.38, /*11 5346.0*/
//   98 		+29.516, /*12 5760.0*/+28.799, /*13 6173.0*/+28.407, /*14 6586.0*/+26.951, /*15 7000.0*/+28.203 }, {/*9  engineLoad=3.119997*//*0 800.0*/
//   99 		+4.532, /*1 1213.0*/+11.978, /*2 1626.0*/+23.73, /*3 2040.0*/+25.501, /*4 2453.0*/+25.624, /*5 2866.0*/+26.328, /*6 3280.0*/
//  100 		+30.015, /*7 3693.0*/+33.187, /*8 4106.0*/+34.881, /*9 4520.0*/+38.044, /*10 4933.0*/+35.81, /*11 5346.0*/
//  101 		+29.843, /*12 5760.0*/+29.306, /*13 6173.0*/+28.997, /*14 6586.0*/+27.109, /*15 7000.0*/+29.339 }, {/*10 engineLoad=3.33333*//*0 800.0*/
//  102 		+4.527, /*1 1213.0*/+12.131, /*2 1626.0*/+23.486, /*3 2040.0*/+25.43, /*4 2453.0*/+25.551, /*5 2866.0*/+26.276, /*6 3280.0*/
//  103 		+29.639, /*7 3693.0*/+33.005, /*8 4106.0*/+34.253, /*9 4520.0*/+37.788, /*10 4933.0*/+36.077, /*11 5346.0*/
//  104 		+30.188, /*12 5760.0*/+29.087, /*13 6173.0*/+28.481, /*14 6586.0*/+27.348, /*15 7000.0*/+27.777 }, {/*11 engineLoad=3.546663*//*0 800.0*/
//  105 		+4.889, /*1 1213.0*/+12.175, /*2 1626.0*/+23.271, /*3 2040.0*/+25.357, /*4 2453.0*/+25.551, /*5 2866.0*/+25.485, /*6 3280.0*/
//  106 		+29.899, /*7 3693.0*/+32.802, /*8 4106.0*/+34.786, /*9 4520.0*/+38.686, /*10 4933.0*/+35.722, /*11 5346.0*/
//  107 		+31.347, /*12 5760.0*/+28.891, /*13 6173.0*/+28.333, /*14 6586.0*/+27.149, /*15 7000.0*/+27.236 }, {/*12 engineLoad=3.759996*//*0 800.0*/
//  108 		+4.537, /*1 1213.0*/+12.073, /*2 1626.0*/+23.896, /*3 2040.0*/+25.525, /*4 2453.0*/+25.595, /*5 2866.0*/+25.451, /*6 3280.0*/
//  109 		+30.428, /*7 3693.0*/+33.714, /*8 4106.0*/+34.08, /*9 4520.0*/+37.526, /*10 4933.0*/+35.817, /*11 5346.0*/
//  110 		+30.733, /*12 5760.0*/+28.718, /*13 6173.0*/+28.518, /*14 6586.0*/+27.518, /*15 7000.0*/+26.561 }, {/*13 engineLoad=3.973329*//*0 800.0*/
//  111 		+4.86, /*1 1213.0*/+11.883, /*2 1626.0*/+23.428, /*3 2040.0*/+25.489, /*4 2453.0*/+25.536, /*5 2866.0*/+25.613, /*6 3280.0*/
//  112 		+29.895, /*7 3693.0*/+33.648, /*8 4106.0*/+34.758, /*9 4520.0*/+37.988, /*10 4933.0*/+36.047, /*11 5346.0*/
//  113 		+30.225, /*12 5760.0*/+28.698, /*13 6173.0*/+28.487, /*14 6586.0*/+27.111, /*15 7000.0*/+27.708 }, {/*14 engineLoad=4.186662*//*0 800.0*/
//  114 		+4.683, /*1 1213.0*/+11.898, /*2 1626.0*/+23.506, /*3 2040.0*/+25.562, /*4 2453.0*/+25.61, /*5 2866.0*/+25.519, /*6 3280.0*/
//  115 		+29.95, /*7 3693.0*/+33.582, /*8 4106.0*/+34.548, /*9 4520.0*/+36.201, /*10 4933.0*/+35.788, /*11 5346.0*/
//  116 		+30.053, /*12 5760.0*/+28.292, /*13 6173.0*/+28.259, /*14 6586.0*/+27.269, /*15 7000.0*/+26.863 }, {/*15 engineLoad=4.399*//*0 800.0*/
//  117 		+4.85, /*1 1213.0*/+12.24, /*2 1626.0*/+24.091, /*3 2040.0*/+25.394, /*4 2453.0*/+25.323, /*5 2866.0*/+25.544, /*6 3280.0*/
//  118 		+29.915, /*7 3693.0*/+33.104, /*8 4106.0*/+36.016, /*9 4520.0*/+37.933, /*10 4933.0*/+36.254, /*11 5346.0*/
//  119 		+29.712, /*12 5760.0*/+28.651, /*13 6173.0*/+28.045, /*14 6586.0*/+27.228, /*15 7000.0*/+27.784 } };
//  120 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z22setDefaultCrankingFuelP22engine_configuration_s
          CFI NoCalls
        THUMB
//  121 static void setDefaultCrankingFuel(engine_configuration_s *engineConfiguration) {
//  122 	// todo: set cranking parameters method based on injectors and displacement?
//  123 
//  124 	// set_cranking_fuel 5
//  125 	engineConfiguration->cranking.baseFuel = 5;
_Z22setDefaultCrankingFuelP22engine_configuration_s:
        LDR.W    R1,??DataTable7  ;; 0x40a00000
        STR      R1,[R0, #+92]
//  126 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//  127 
//  128 EXTERN_ENGINE;
//  129 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z13commonMiataNav
        THUMB
//  130 static void commonMiataNa(DECLARE_ENGINE_PARAMETER_F) {
_Z13commonMiataNav:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  131 	engineConfiguration->trigger.type = TT_MAZDA_MIATA_NA;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+3
        STR      R1,[R0, #+536]
//  132 	engineConfiguration->engineChartSize = 100;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+100
        STR      R1,[R0, #+1512]
//  133 
//  134 	boardConfiguration->triggerInputPins[0] = GPIOC_6; // 2G YEL/BLU
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+38
        STR      R1,[R0, #+296]
//  135 	boardConfiguration->triggerInputPins[1] = GPIOA_5; // 2E White CKP
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+5
        STR      R1,[R0, #+300]
//  136 
//  137 	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+444]
//  138 	setFuelLoadBin(1.2, 4.4 PASS_ENGINE_PARAMETER);
        VLDR.W   S1,??DataTable2  ;; 0x408ccccd
        VLDR.W   S0,??DataTable2_1  ;; 0x3f99999a
          CFI FunCall _Z14setFuelLoadBinff
        BL       _Z14setFuelLoadBinff
//  139 	setFuelRpmBin(800, 7000 PASS_ENGINE_PARAMETER);
        VLDR.W   S1,??DataTable2_2  ;; 0x45dac000
        VLDR.W   S0,??DataTable2_3  ;; 0x44480000
          CFI FunCall _Z13setFuelRpmBinff
        BL       _Z13setFuelRpmBinff
//  140 
//  141 	boardConfiguration->idle.solenoidFrequency = 160;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+160
        STR      R1,[R0, #+8]
//  142 
//  143 	engineConfiguration->globalTriggerAngleOffset = 294;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable7_3  ;; 0x43930000
        STR      R1,[R0, #+460]
//  144 
//  145 	// Frankenstein: high side #1 is PE8
//  146 	// Frankenstein: high side #2 is PE10
//  147 	// Frankenstein: high side #3 is PE12
//  148 	// Frankenstein: high side #4 is PE14
//  149 	// Frankenstein: high side #5 is PC9
//  150 	// Frankenstein: high side #6 is PC7
//  151 
//  152 	boardConfiguration->ignitionPins[0] = GPIOE_12; // Frankenstein: high side #3
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+76
        STR      R1,[R0, #+84]
//  153 	boardConfiguration->ignitionPins[1] = GPIOE_14; // Frankenstein: high side #4
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+78
        STR      R1,[R0, #+88]
//  154 	boardConfiguration->ignitionPins[2] = GPIO_UNASSIGNED;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+92]
//  155 	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+96]
//  156 	boardConfiguration->ignitionPinMode = OM_DEFAULT;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+132]
//  157 
//  158 	setDefaultCrankingFuel(engineConfiguration);
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z22setDefaultCrankingFuelP22engine_configuration_s
        BL       _Z22setDefaultCrankingFuelP22engine_configuration_s
//  159 
//  160 	boardConfiguration->triggerSimulatorPinModes[0] = OM_OPENDRAIN;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+364]
//  161 	boardConfiguration->triggerSimulatorPinModes[1] = OM_OPENDRAIN;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+368]
//  162 
//  163 	setCommonNTCSensor(&engineConfiguration->clt);
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+256
          CFI FunCall _Z18setCommonNTCSensorP14ThermistorConf
        BL       _Z18setCommonNTCSensorP14ThermistorConf
//  164 	engineConfiguration->clt.config.bias_resistor = 2700;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable7_4  ;; 0x4528c000
        STR      R1,[R0, #+280]
//  165 	setCommonNTCSensor(&engineConfiguration->iat);
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+288
          CFI FunCall _Z18setCommonNTCSensorP14ThermistorConf
        BL       _Z18setCommonNTCSensorP14ThermistorConf
//  166 	engineConfiguration->iat.config.bias_resistor = 2700;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable7_4  ;; 0x4528c000
        STR      R1,[R0, #+312]
//  167 
//  168 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock1
//  169 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z17common079721_2351P22engine_configuration_sP21board_configuration_s
          CFI NoCalls
        THUMB
//  170 static void common079721_2351(engine_configuration_s *engineConfiguration, board_configuration_s *boardConfiguration) {
//  171 
//  172 	engineConfiguration->engineChartSize = 150;
_Z17common079721_2351P22engine_configuration_sP21board_configuration_s:
        MOVS     R2,#+150
        STR      R2,[R0, #+1512]
//  173 
//  174 	engineConfiguration->specs.cylindersCount = 4;
        MOVS     R2,#+4
        STR      R2,[R0, #+404]
//  175 	engineConfiguration->specs.firingOrder = FO_1_THEN_3_THEN_4_THEN2;
        MOVS     R2,#+1
        STR      R2,[R0, #+408]
//  176 
//  177 	boardConfiguration->fuelPumpPin = GPIO_UNASSIGNED; // fuel pump is not controlled by ECU on this engine
        MOVS     R2,#+80
        STR      R2,[R1, #+24]
//  178 
//  179 	// set_cranking_injection_mode 0
//  180 	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
        MOVS     R2,#+0
        STR      R2,[R0, #+428]
//  181 	// set_injection_mode 2
//  182 	engineConfiguration->injectionMode = IM_BATCH;
        MOVS     R2,#+2
        STR      R2,[R0, #+432]
//  183 
//  184 	// Frankenstein analog input #1: adc1
//  185 	// Frankenstein analog input #2: adc3
//  186 	// Frankenstein analog input #3: adc13
//  187 	// Frankenstein analog input #4: adc11
//  188 	// todo: see https://docs.google.com/spreadsheet/ccc?key=0Arl1FeMZcfisdEdGdUlHdWh6cVBoSzFIbkxqa1QtZ3c
//  189 	// Frankenstein analog input #5: adc
//  190 	// Frankenstein analog input #6: adc
//  191 	// Frankenstein analog input #7: adc
//  192 	// Frankenstein analog input #8: adc
//  193 	// Frankenstein analog input #9: adc
//  194 	// Frankenstein analog input #10: adc
//  195 	// Frankenstein analog input #11: adc
//  196 	// Frankenstein analog input #12: adc
//  197 	engineConfiguration->mafAdcChannel = EFI_ADC_1;
        MOVS     R2,#+1
        STR      R2,[R0, #+580]
//  198 	engineConfiguration->tpsAdcChannel = EFI_ADC_3;
        MOVS     R2,#+3
        STR      R2,[R0, #+524]
//  199 	engineConfiguration->clt.adcChannel = EFI_ADC_11;
        MOVS     R2,#+11
        STR      R2,[R0, #+284]
//  200 
//  201 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock2
//  202 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z12setMiata1990v
        THUMB
//  203 void setMiata1990(DECLARE_ENGINE_PARAMETER_F) {
_Z12setMiata1990v:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  204 	common079721_2351(engineConfiguration, boardConfiguration);
        LDR.W    R0,??DataTable7_2
        LDR      R1,[R0, #+0]
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z17common079721_2351P22engine_configuration_sP21board_configuration_s
        BL       _Z17common079721_2351P22engine_configuration_sP21board_configuration_s
//  205 
//  206 	commonMiataNa(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z13commonMiataNav
        BL       _Z13commonMiataNav
//  207 
//  208 	// Frankenstein: low side - out #1: PC14
//  209 	// Frankenstein: low side - out #2: PC15
//  210 	// Frankenstein: low side - out #3: PE6
//  211 	// Frankenstein: low side - out #4: PC13
//  212 	// Frankenstein: low side - out #5: PE4
//  213 	// Frankenstein: low side - out #6: PE5
//  214 	// Frankenstein: low side - out #7: PE2
//  215 	// Frankenstein: low side - out #8: PE3
//  216 	// Frankenstein: low side - out #9: PE0
//  217 	// Frankenstein: low side - out #10: PE1
//  218 	// Frankenstein: low side - out #11: PB8
//  219 	// Frankenstein: low side - out #12: PB9
//  220 
//  221 	boardConfiguration->injectionPins[0] = GPIOB_9; // Frankenstein: low side - out #12
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+25
        STR      R1,[R0, #+32]
//  222 	boardConfiguration->injectionPins[1] = GPIOB_8; // Frankenstein: low side - out #11
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+24
        STR      R1,[R0, #+36]
//  223 	boardConfiguration->injectionPins[2] = GPIO_UNASSIGNED;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+40]
//  224 	boardConfiguration->injectionPins[3] = GPIO_UNASSIGNED;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+44]
//  225 	boardConfiguration->injectionPins[4] = GPIO_UNASSIGNED;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+48]
//  226 	boardConfiguration->injectionPins[5] = GPIO_UNASSIGNED;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+52]
//  227 	boardConfiguration->injectionPinMode = OM_DEFAULT;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+80]
//  228 
//  229 // todo: idleValvePin
//  230 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2:
        DC32     0x408ccccd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_1:
        DC32     0x3f99999a

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_2:
        DC32     0x45dac000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_3:
        DC32     0x44480000
//  231 
//  232 
//  233 /**
//  234  * pin 1I/W9 - extra +5v
//  235  * set_engine_type 14
//  236  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z15setFordEscortGtv
        THUMB
//  237 void setFordEscortGt(DECLARE_ENGINE_PARAMETER_F) {
_Z15setFordEscortGtv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  238 	engineConfiguration->trigger.type = TT_MAZDA_DOHC_1_4;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+15
        STR      R1,[R0, #+536]
//  239 
//  240 	common079721_2351(engineConfiguration, boardConfiguration);
        LDR.W    R0,??DataTable7_2
        LDR      R1,[R0, #+0]
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z17common079721_2351P22engine_configuration_sP21board_configuration_s
        BL       _Z17common079721_2351P22engine_configuration_sP21board_configuration_s
//  241 
//  242 	setFrankenso_01_LCD(boardConfiguration);
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
          CFI FunCall _Z19setFrankenso_01_LCDP21board_configuration_s
        BL       _Z19setFrankenso_01_LCDP21board_configuration_s
//  243 	setFrankenso0_1_joystick(engineConfiguration);
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z24setFrankenso0_1_joystickP22engine_configuration_s
        BL       _Z24setFrankenso0_1_joystickP22engine_configuration_s
//  244 
//  245 	setDensoTODO(config);
        LDR.W    R0,??DataTable7_5
        LDR      R0,[R0, #+0]
          CFI FunCall _Z12setDensoTODOP19persistent_config_s
        BL       _Z12setDensoTODOP19persistent_config_s
//  246 
//  247 	engineConfiguration->globalFuelCorrection = 0.75;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1061158912
        STR      R1,[R0, #+568]
//  248 	engineConfiguration->specs.displacement = 1.839;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable7_6  ;; 0x3feb645a
        STR      R1,[R0, #+400]
//  249 //	engineConfiguration->algorithm = LM_PLAIN_MAF;
//  250 	engineConfiguration->algorithm = LM_SPEED_DENSITY;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+3
        STR      R1,[R0, #+424]
//  251 //	engineConfiguration->algorithm = LM_REAL_MAF;
//  252 	boardConfiguration->tunerStudioSerialSpeed = 9600;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOV      R1,#+9600
        STR      R1,[R0, #+332]
//  253 
//  254 	setFuelLoadBin(1.2, 4.4 PASS_ENGINE_PARAMETER);
        VLDR.W   S1,??DataTable3  ;; 0x408ccccd
        VLDR.W   S0,??DataTable3_1  ;; 0x3f99999a
          CFI FunCall _Z14setFuelLoadBinff
        BL       _Z14setFuelLoadBinff
//  255 	setFuelRpmBin(800, 7000 PASS_ENGINE_PARAMETER);
        VLDR.W   S1,??DataTable3_2  ;; 0x45dac000
        VLDR.W   S0,??DataTable3_3  ;; 0x44480000
          CFI FunCall _Z13setFuelRpmBinff
        BL       _Z13setFuelRpmBinff
//  256 
//  257 //	boardConfiguration->triggerInputPins[0] = GPIOC_6; // 2G YEL/BLU
//  258 //	boardConfiguration->triggerInputPins[1] = GPIOA_5; // 2E White CKP
//  259 
//  260 	// in case of SOHC distributor we only have one signal
//  261 //	boardConfiguration->triggerInputPins[0] = GPIOA_5; // 2E White CKP
//  262 //	boardConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
//  263 
//  264 	// in case of SOHC distributor we only have one signal
//  265 	boardConfiguration->triggerInputPins[0] = GPIOC_6;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+38
        STR      R1,[R0, #+296]
//  266 	boardConfiguration->triggerInputPins[1] = GPIOA_5; // 2E White CKP
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+5
        STR      R1,[R0, #+300]
//  267 
//  268 	// Denso 195500-2180
//  269 	engineConfiguration->injector.flow = 265;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable7_7  ;; 0x43848000
        STR      R1,[R0, #+8]
//  270 
//  271 	engineConfiguration->hasBaroSensor = false;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        ANDS     R0,R0,#0xFB
        LDR.W    R1,??DataTable7_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1496]
//  272 
//  273 	engineConfiguration->hasMapSensor = true;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        ORRS     R0,R0,#0x1
        LDR.W    R1,??DataTable7_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1496]
//  274 	boardConfiguration->isFastAdcEnabled = true;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        ORRS     R0,R0,#0x10
        LDR.W    R1,??DataTable7_2
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+384]
//  275 	engineConfiguration->map.sensor.type = MT_DENSO183;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+248]
//  276 	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+4
        STR      R1,[R0, #+252]
//  277 
//  278 	setEgoSensor(ES_BPSX_D1 PASS_ENGINE_PARAMETER);
        MOVS     R0,#+0
          CFI FunCall _Z12setEgoSensor12ego_sensor_e
        BL       _Z12setEgoSensor12ego_sensor_e
//  279 	engineConfiguration->afr.hwChannel = EFI_ADC_2;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+584]
//  280 
//  281 
//  282 	// set_global_trigger_offset_angle -40
//  283 	engineConfiguration->globalTriggerAngleOffset = -40;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable7_8  ;; 0xc2200000
        STR      R1,[R0, #+460]
//  284 	// set_ignition_offset 0
//  285 	engineConfiguration->ignitionOffset = 0;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+448]
//  286 	// set_injection_offset 0
//  287 	engineConfiguration->injectionOffset = 0;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+436]
//  288 
//  289 	// todo: change to 15?
//  290 	// set_cranking_timing_angle 3
//  291 	engineConfiguration->crankingTimingAngle = 3;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable7_9  ;; 0x40400000
        STR      R1,[R0, #+440]
//  292 	engineConfiguration->crankingChargeAngle = 70;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable7_10  ;; 0x428c0000
        STR      R1,[R0, #+108]
//  293 
//  294 	setWholeTimingTable(10 PASS_ENGINE_PARAMETER);
        VMOV.F32 S0,#10.0
          CFI FunCall _Z19setWholeTimingTablef
        BL       _Z19setWholeTimingTablef
//  295 	// set_whole_fuel_map 5
//  296 	setWholeFuelMap(5 PASS_ENGINE_PARAMETER);
        VMOV.F32 S0,#5.0
          CFI FunCall _Z15setWholeFuelMapf
        BL       _Z15setWholeFuelMapf
//  297 
//  298 	setSingleCoilDwell(engineConfiguration);
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z18setSingleCoilDwellP22engine_configuration_s
        BL       _Z18setSingleCoilDwellP22engine_configuration_s
//  299 	engineConfiguration->ignitionMode = IM_ONE_COIL;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+444]
//  300 
//  301 	boardConfiguration->triggerSimulatorPinModes[0] = OM_OPENDRAIN;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+364]
//  302 	boardConfiguration->triggerSimulatorPinModes[1] = OM_OPENDRAIN;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+368]
//  303 
//  304 	boardConfiguration->ignitionPins[0] = GPIOE_14; // Frankenso high side - pin 1G
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+78
        STR      R1,[R0, #+84]
//  305 	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+88]
//  306 	boardConfiguration->ignitionPins[2] = GPIO_UNASSIGNED;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+92]
//  307 	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+96]
//  308 	boardConfiguration->ignitionPinMode = OM_DEFAULT;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+132]
//  309 
//  310 	/**
//  311 	 * Outputs
//  312 	 */
//  313 	// Frankenso low out #1: PE6
//  314 	// Frankenso low out #2: PE5 MIL
//  315 	// Frankenso low out #3:
//  316 	// Frankenso low out #4:
//  317 	// Frankenso low out #5: PE3
//  318 	// Frankenso low out #6: PE4
//  319 	// Frankenso low out #7: PE0<>PD5
//  320 	// Frankenso low out #8: PE2 INJ
//  321 	// Frankenso low out #9: PB9 IDLE
//  322 	// Frankenso low out #10: PE1<>PD3 INJ 1&3
//  323 	// Frankenso low out #11: PB8
//  324 	// Frankenso low out #12: PB7
//  325 
//  326 	boardConfiguration->injectionPins[0] = GPIOD_3;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+51
        STR      R1,[R0, #+32]
//  327 	boardConfiguration->injectionPins[1] = GPIOE_2;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+66
        STR      R1,[R0, #+36]
//  328 
//  329 
//  330 	//setDefaultCrankingFuel(engineConfiguration);
//  331 	engineConfiguration->cranking.baseFuel = 5;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable7  ;; 0x40a00000
        STR      R1,[R0, #+92]
//  332 
//  333 	// 40% idle is good default
//  334 	boardConfiguration->idle.solenoidFrequency = 300;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOV      R1,#+300
        STR      R1,[R0, #+8]
//  335 	boardConfiguration->idle.solenoidPin = GPIOB_9;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+25
        STR      R1,[R0, #+0]
//  336 
//  337 	boardConfiguration->malfunctionIndicatorPin = GPIOE_5;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+69
        STR      R1,[R0, #+136]
//  338 	boardConfiguration->malfunctionIndicatorPinMode = OM_DEFAULT;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+140]
//  339 
//  340 	boardConfiguration->tunerStudioSerialSpeed = 9600;
        LDR.W    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOV      R1,#+9600
        STR      R1,[R0, #+332]
//  341 
//  342 	commonFrankensoAnalogInputs(engineConfiguration);
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z27commonFrankensoAnalogInputsP22engine_configuration_s
        BL       _Z27commonFrankensoAnalogInputsP22engine_configuration_s
//  343 	setCommonNTCSensor(&engineConfiguration->clt);
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+256
          CFI FunCall _Z18setCommonNTCSensorP14ThermistorConf
        BL       _Z18setCommonNTCSensorP14ThermistorConf
//  344 	engineConfiguration->clt.config.bias_resistor = 2700;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable7_4  ;; 0x4528c000
        STR      R1,[R0, #+280]
//  345 	setCommonNTCSensor(&engineConfiguration->iat);
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+288
          CFI FunCall _Z18setCommonNTCSensorP14ThermistorConf
        BL       _Z18setCommonNTCSensorP14ThermistorConf
//  346 	engineConfiguration->iat.config.bias_resistor = 2700;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable7_4  ;; 0x4528c000
        STR      R1,[R0, #+312]
//  347 
//  348 	engineConfiguration->hasTpsSensor = false;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        ANDS     R0,R0,#0x7F
        LDR.W    R1,??DataTable7_1
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1497]
//  349 	engineConfiguration->tpsAdcChannel = EFI_ADC_NONE;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+16
        STR      R1,[R0, #+524]
//  350 //	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;
//  351 	engineConfiguration->mafAdcChannel = EFI_ADC_0;
        LDR.W    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+580]
//  352 	engineConfiguration->clt.adcChannel = EFI_ADC_12;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+12
        STR      R1,[R0, #+284]
//  353 	engineConfiguration->iat.adcChannel = EFI_ADC_11;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+11
        STR      R1,[R0, #+316]
//  354 
//  355 	// todo: 8.2 or 10k?
//  356 	engineConfiguration->vbattDividerCoeff = ((float) (10 + 33)) / 10 * 2;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable7_11  ;; 0x4109999a
        STR      R1,[R0, #+468]
//  357 
//  358 	// end of Ford Escort GT config
//  359 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3:
        DC32     0x408ccccd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_1:
        DC32     0x3f99999a

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_2:
        DC32     0x45dac000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_3:
        DC32     0x44480000
//  360 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z19setMiata1994_commonv
        THUMB
//  361 static void setMiata1994_common(DECLARE_ENGINE_PARAMETER_F) {
_Z19setMiata1994_commonv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  362 	commonMiataNa(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z13commonMiataNav
        BL       _Z13commonMiataNav
//  363 	engineConfiguration->specs.displacement = 1.839;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable7_6  ;; 0x3feb645a
        STR      R1,[R0, #+400]
//  364 
//  365 	// set_cranking_timing_angle 0
//  366 	engineConfiguration->crankingTimingAngle = 0;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+440]
//  367 
//  368 	engineConfiguration->crankingChargeAngle = 70;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable7_10  ;; 0x428c0000
        STR      R1,[R0, #+108]
//  369 
//  370 	copyFuelTable(miata_maf_fuel_table, config->fuelTable);
        LDR.N    R0,??DataTable7_5
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+10496
        ADDS     R1,R1,#+184
        LDR.N    R0,??DataTable7_12
          CFI FunCall _Z13copyFuelTablePA16_KfPA16_f
        BL       _Z13copyFuelTablePA16_KfPA16_f
//  371 
//  372 	copyTimingTable(miata_maf_advance_table, config->ignitionTable);
        LDR.N    R0,??DataTable7_5
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+11776
        ADDS     R1,R1,#+56
        LDR.N    R0,??DataTable7_13
          CFI FunCall _Z15copyTimingTablePA16_KfPA16_f
        BL       _Z15copyTimingTablePA16_KfPA16_f
//  373 
//  374 //	boardConfiguration->triggerSimulatorPins[0] = GPIOD_2; // 2G - YEL/BLU
//  375 //	boardConfiguration->triggerSimulatorPins[1] = GPIOB_3; // 2E - WHT - four times
//  376 //	boardConfiguration->triggerSimulatorPinModes[0] = OM_OPENDRAIN;
//  377 //	boardConfiguration->triggerSimulatorPinModes[1] = OM_OPENDRAIN;
//  378 //
//  379 //	boardConfiguration->triggerInputPins[0] = GPIO_UNASSIGNED;
//  380 //	boardConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
//  381 //
//  382 //	boardConfiguration->is_enabled_spi_1 = false;
//  383 //	boardConfiguration->is_enabled_spi_2 = false;
//  384 //	boardConfiguration->is_enabled_spi_3 = false;
//  385 
//  386 	/**
//  387 	 * Outputs
//  388 	 */
//  389 	// Frankenso low out #: PE6
//  390 	// Frankenso low out #: PE5
//  391 	// Frankenso low out #:
//  392 	// Frankenso low out #:
//  393 	// Frankenso low out #5: PE3
//  394 	// Frankenso low out #6: PE4
//  395 	// Frankenso low out #7: PE1 (do not use with discovery!)
//  396 	// Frankenso low out #8:
//  397 	// Frankenso low out #9: PB9
//  398 	// Frankenso low out #10: PE0 (do not use with discovery!)
//  399 	// Frankenso low out #11: PB8
//  400 	// Frankenso low out #12: PB7
//  401 	boardConfiguration->fanPin = GPIOE_6;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+70
        STR      R1,[R0, #+144]
//  402 
//  403 	boardConfiguration->o2heaterPin = GPIO_UNASSIGNED;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+376]
//  404 
//  405 	boardConfiguration->fuelPumpPin = GPIOE_4;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+68
        STR      R1,[R0, #+24]
//  406 
//  407 	boardConfiguration->injectionPins[4] = GPIO_UNASSIGNED;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+48]
//  408 	boardConfiguration->injectionPins[5] = GPIO_UNASSIGNED;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+52]
//  409 	boardConfiguration->injectionPinMode = OM_DEFAULT;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+80]
//  410 
//  411 	boardConfiguration->idle.solenoidPin = GPIOB_9;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+25
        STR      R1,[R0, #+0]
//  412 
//  413 	boardConfiguration->ignitionPins[0] = GPIOE_14; // Frankenso high side - pin 1G
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+78
        STR      R1,[R0, #+84]
//  414 	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+88]
//  415 	boardConfiguration->ignitionPins[2] = GPIOC_7; // Frankenso high side - pin 1H
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+39
        STR      R1,[R0, #+92]
//  416 	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+96]
//  417 	boardConfiguration->ignitionPinMode = OM_DEFAULT;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+132]
//  418 
//  419 	setFrankenso_01_LCD(boardConfiguration);
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
          CFI FunCall _Z19setFrankenso_01_LCDP21board_configuration_s
        BL       _Z19setFrankenso_01_LCDP21board_configuration_s
//  420 
//  421 	commonFrankensoAnalogInputs(engineConfiguration);
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z27commonFrankensoAnalogInputsP22engine_configuration_s
        BL       _Z27commonFrankensoAnalogInputsP22engine_configuration_s
//  422 
//  423 	engineConfiguration->tpsAdcChannel = EFI_ADC_2;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+524]
//  424 	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+4
        STR      R1,[R0, #+252]
//  425 	engineConfiguration->mafAdcChannel = EFI_ADC_0;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+580]
//  426 	engineConfiguration->clt.adcChannel = EFI_ADC_12;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+12
        STR      R1,[R0, #+284]
//  427 	engineConfiguration->iat.adcChannel = EFI_ADC_11;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+11
        STR      R1,[R0, #+316]
//  428 // end of 1994 commond
//  429 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock5
//  430 
//  431 /**
//  432  * Frankenso board
//  433  * set_engine_type 20
//  434  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z14setMiata1994_dv
        THUMB
//  435 void setMiata1994_d(DECLARE_ENGINE_PARAMETER_F) {
_Z14setMiata1994_dv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  436 	setMiata1994_common(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z19setMiata1994_commonv
        BL       _Z19setMiata1994_commonv
//  437 	engineConfiguration->vbattDividerCoeff = ((float) (8.2 + 33)) / 8.2 * 2;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable7_14  ;; 0x4120c7ce
        STR      R1,[R0, #+468]
//  438 	/**
//  439 	 * This board was avoiding PE0 & PE1 mosfets altogether
//  440 	 */
//  441 	boardConfiguration->injectionPins[0] = GPIOD_7; // avoiding PE1
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+55
        STR      R1,[R0, #+32]
//  442 	boardConfiguration->injectionPins[1] = GPIOE_2;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+66
        STR      R1,[R0, #+36]
//  443 	boardConfiguration->injectionPins[2] = GPIOB_8;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+24
        STR      R1,[R0, #+40]
//  444 	boardConfiguration->injectionPins[3] = GPIOB_7;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+23
        STR      R1,[R0, #+44]
//  445 
//  446 	// todo: add the diode? change idle valve logic?
//  447 	boardConfiguration->idle.solenoidPin = GPIO_UNASSIGNED;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+0]
//  448 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock6
//  449 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z14setMiata1994_sv
        THUMB
//  450 void setMiata1994_s(DECLARE_ENGINE_PARAMETER_F) {
_Z14setMiata1994_sv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  451 	setMiata1994_common(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z19setMiata1994_commonv
        BL       _Z19setMiata1994_commonv
//  452 	engineConfiguration->vbattDividerCoeff = ((float) (10.0 + 33)) / 10 * 2;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable7_11  ;; 0x4109999a
        STR      R1,[R0, #+468]
//  453 
//  454 	boardConfiguration->triggerSimulatorPins[2] = GPIO_UNASSIGNED;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+360]
//  455 
//  456 	engineConfiguration->acSwitchAdc = EFI_ADC_1; // PA1, W50 on Frankenso
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+1520]
//  457 
//  458 	engineConfiguration->afr.hwChannel = EFI_ADC_3;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+3
        STR      R1,[R0, #+584]
//  459 	setEgoSensor(ES_Innovate_MTX_L PASS_ENGINE_PARAMETER);
        MOVS     R0,#+1
          CFI FunCall _Z12setEgoSensor12ego_sensor_e
        BL       _Z12setEgoSensor12ego_sensor_e
//  460 
//  461 	/**
//  462 	 * This board has PE0<>PD5 & PE1<>PD3 rewired in order to avoid Discovery issue
//  463 	 */
//  464 	boardConfiguration->injectionPins[0] = GPIOD_3; // avoiding PE1
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+51
        STR      R1,[R0, #+32]
//  465 	boardConfiguration->injectionPins[1] = GPIOE_2; // injector #2
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+66
        STR      R1,[R0, #+36]
//  466 	boardConfiguration->injectionPins[2] = GPIOB_8; // injector #3
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+24
        STR      R1,[R0, #+40]
//  467 	boardConfiguration->injectionPins[3] = GPIOB_7; // injector #4
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+23
        STR      R1,[R0, #+44]
//  468 
//  469 	//	setFsio(engineConfiguration, 0, GPIOD_11, "coolant 80 >");
//  470 	boardConfiguration->idle.solenoidFrequency = 500;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOV      R1,#+500
        STR      R1,[R0, #+8]
//  471 
//  472 	engineConfiguration->acCutoffLowRpm = 400;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOV      R1,#+400
        STRH     R1,[R0, #+1528]
//  473 	engineConfiguration->acCutoffHighRpm = 4500;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVW     R1,#+4500
        STRH     R1,[R0, #+1530]
//  474 	engineConfiguration->acIdleRpmBump = 200;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+200
        STRH     R1,[R0, #+1532]
//  475 
//  476 	//engineConfiguration->idleMode != IM_AUTO;
//  477 	engineConfiguration->targetIdleRpm = 800;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOV      R1,#+800
        STR      R1,[R0, #+1516]
//  478 
//  479 	engineConfiguration->fanOffTemperature = 90;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable7_15  ;; 0x42b40000
        STR      R1,[R0, #+480]
//  480 	engineConfiguration->fanOnTemperature = 95;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable7_16  ;; 0x42be0000
        STR      R1,[R0, #+476]
//  481 
//  482 	engineConfiguration->tpsMin = 86;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+86
        STRH     R1,[R0, #+82]
//  483 	engineConfiguration->tpsMax = 596;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOV      R1,#+596
        STRH     R1,[R0, #+84]
//  484 
//  485 	boardConfiguration->malfunctionIndicatorPin = GPIOE_5;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+69
        STR      R1,[R0, #+136]
//  486 	boardConfiguration->malfunctionIndicatorPinMode = OM_DEFAULT;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+140]
//  487 
//  488 	engineConfiguration->algorithm = LM_REAL_MAF;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+4
        STR      R1,[R0, #+424]
//  489 	setMazdaMiataNAMaf(config);
        LDR.N    R0,??DataTable7_5
        LDR      R0,[R0, #+0]
          CFI FunCall _Z18setMazdaMiataNAMafP19persistent_config_s
        BL       _Z18setMazdaMiataNAMafP19persistent_config_s
//  490 	engineConfiguration->injector.flow = 230;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable7_17  ;; 0x43660000
        STR      R1,[R0, #+8]
//  491 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock7
//  492 
//  493 /**
//  494  * Tom tomiata, Frankenstein board
//  495  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z12setMiata1996v
        THUMB
//  496 void setMiata1996(DECLARE_ENGINE_PARAMETER_F) {
_Z12setMiata1996v:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  497 	commonMiataNa(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z13commonMiataNav
        BL       _Z13commonMiataNav
//  498 	engineConfiguration->specs.displacement = 1.839;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable7_6  ;; 0x3feb645a
        STR      R1,[R0, #+400]
//  499 
//  500 	copyFuelTable(miata_maf_fuel_table, config->fuelTable);
        LDR.N    R0,??DataTable7_5
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+10496
        ADDS     R1,R1,#+184
        LDR.N    R0,??DataTable7_12
          CFI FunCall _Z13copyFuelTablePA16_KfPA16_f
        BL       _Z13copyFuelTablePA16_KfPA16_f
//  501 	copyTimingTable(miata_maf_advance_table, config->ignitionTable);
        LDR.N    R0,??DataTable7_5
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+11776
        ADDS     R1,R1,#+56
        LDR.N    R0,??DataTable7_13
          CFI FunCall _Z15copyTimingTablePA16_KfPA16_f
        BL       _Z15copyTimingTablePA16_KfPA16_f
//  502 
//  503 	// upside down
//  504 	boardConfiguration->triggerInputPins[0] = GPIOA_5;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+5
        STR      R1,[R0, #+296]
//  505 	boardConfiguration->triggerInputPins[1] = GPIOC_6;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+38
        STR      R1,[R0, #+300]
//  506 
//  507 	boardConfiguration->fuelPumpPin = GPIOE_4;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+68
        STR      R1,[R0, #+24]
//  508 	boardConfiguration->idle.solenoidPin = GPIOE_5;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+69
        STR      R1,[R0, #+0]
//  509 
//  510 	engineConfiguration->mafAdcChannel = EFI_ADC_1;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+580]
//  511 	engineConfiguration->clt.adcChannel = EFI_ADC_11;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+11
        STR      R1,[R0, #+284]
//  512 	engineConfiguration->tpsAdcChannel = EFI_ADC_13;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+13
        STR      R1,[R0, #+524]
//  513 
//  514 	boardConfiguration->ignitionPins[0] = GPIOE_12; // Frankenstein: high side #3
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+76
        STR      R1,[R0, #+84]
//  515 	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+88]
//  516 	boardConfiguration->ignitionPins[2] = GPIOE_14; // Frankenstein: high side #4
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+78
        STR      R1,[R0, #+92]
//  517 	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+96]
//  518 	boardConfiguration->ignitionPinMode = OM_DEFAULT;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+132]
//  519 
//  520 	// harness is sequential but we have a limited board
//  521 	engineConfiguration->crankingInjectionMode = IM_BATCH;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+428]
//  522 	engineConfiguration->injectionMode = IM_BATCH;
        LDR.N    R0,??DataTable7_1
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+432]
//  523 
//  524 	boardConfiguration->injectionPins[0] = GPIOB_9; // Frankenstein: low side - out #12
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+25
        STR      R1,[R0, #+32]
//  525 	boardConfiguration->injectionPins[1] = GPIOB_8; // Frankenstein: low side - out #11
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+24
        STR      R1,[R0, #+36]
//  526 	boardConfiguration->injectionPins[2] = GPIO_UNASSIGNED;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+40]
//  527 	boardConfiguration->injectionPins[3] = GPIO_UNASSIGNED;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+44]
//  528 	boardConfiguration->injectionPins[4] = GPIO_UNASSIGNED;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+48]
//  529 	boardConfiguration->injectionPins[5] = GPIO_UNASSIGNED;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+52]
//  530 	boardConfiguration->injectionPinMode = OM_DEFAULT;
        LDR.N    R0,??DataTable7_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+80]
//  531 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7:
        DC32     0x40a00000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_1:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_2:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_3:
        DC32     0x43930000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_4:
        DC32     0x4528c000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_5:
        DC32     config

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_6:
        DC32     0x3feb645a

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_7:
        DC32     0x43848000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_8:
        DC32     0xc2200000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_9:
        DC32     0x40400000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_10:
        DC32     0x428c0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_11:
        DC32     0x4109999a

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_12:
        DC32     miata_maf_fuel_table

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_13:
        DC32     miata_maf_advance_table

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_14:
        DC32     0x4120c7ce

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_15:
        DC32     0x42b40000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_16:
        DC32     0x42be0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_17:
        DC32     0x43660000

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
// 1 978 bytes in section .text
// 
// 1 978 bytes of CODE  memory
// 2 048 bytes of CONST memory
//
//Errors: none
//Warnings: 1
