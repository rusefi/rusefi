///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:52 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       ford_aspire.cpp                                      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       ford_aspire.cpp -lCN F:\stuff\rusefi_sourceforge\fir /
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
//                       ford_aspire.s                                        /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME ford_aspire

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z13copyFuelTablePA16_KfPA16_f
        EXTERN _Z13setFuelRpmBinff
        EXTERN _Z14setFuelLoadBinff
        EXTERN _Z15copyTimingTablePA16_KfPA16_f
        EXTERN _Z15setTimingRpmBinff
        EXTERN _Z16setOperationModeP22engine_configuration_s16operation_mode_e
        EXTERN _Z16setTimingLoadBinff
        EXTERN _Z18setSingleCoilDwellP22engine_configuration_s
        EXTERN _Z26setThermistorConfigurationP14ThermistorConfffffff
        EXTERN boardConfiguration
        EXTERN config
        EXTERN engineConfiguration

        PUBLIC _Z32setFordAspireEngineConfigurationv
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\config\engines\ford_aspire.cpp
//    1 /**
//    2  * @file    ford_aspire.cpp
//    3  * @brief	1996 Ford Aspire default engine configuration
//    4  *
//    5  * FORD_ASPIRE_1996 = 3
//    6  * set_engine_type 3
//    7  *
//    8  * @date Sep 9, 2013
//    9  * @author Andrey Belomutskiy, (c) 2012-2015
//   10  */
//   11 
//   12 #include "main.h"
//   13 
//   14 #include "ford_aspire.h"
//   15 
//   16 #include "allsensors.h"
//   17 #include "engine_math.h"
//   18 #include "advance_map.h"
//   19 #include "engine_configuration.h"
//   20 
//   21 #if EFI_SUPPORT_FORD_ASPIRE || defined(__DOXYGEN__)
//   22 
//   23 /**
//   24  * This is just the default map which is stored into flash memory in case flash is empty
//   25  * The convenient way to override these default would be to tune this map using TunerStudio software
//   26  * with which rusEfi is integrated
//   27  */

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//   28 static const fuel_table_t default_aspire_fuel_table = {
default_aspire_fuel_table:
        DATA
        DC32 3FC56042H, 3FC617C2H, 3FC6A394H, 3FC8F5C3H, 3FC66666H, 3F689E56H
        DC32 3F4B7803H, 3F467382H, 3F4AAF04H, 3F4CCCCDH, 3F4C7571H, 3F4E4485H
        DC32 3F4F5C29H, 3F4F5C29H, 3F4F5C29H, 3F4F5C29H, 3FC41CEFH, 3FC85C92H
        DC32 3FC68F2AH, 3FC6C144H, 3FC5E3EBH, 3F4D55B4H, 3F4F5C29H, 3F49DB98H
        DC32 3F4F13F0H, 3F3EB778H, 3F338D82H, 3F361E93H, 3F3EA27FH, 3F4F5C29H
        DC32 3F4F5C29H, 3F4F5C29H, 3FC594AFH, 3FC5D556H, 3FC72991H, 3FC5E271H
        DC32 3FB529A2H, 3DEC1C1DH, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H
        DC32 3FC4F766H, 3FDEF65AH, 3FC77A81H, 3FCBB44EH, 3F9D486FH, 0H, 0H, 0H
        DC32 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 4014AAEFH, 405C27C8H, 403462AEH
        DC32 400F8149H, 40080DBFH, 3F98D656H, 3EFA58D6H, 0H, 0H, 0H, 0H, 0H, 0H
        DC32 0H, 0H, 0H, 40F64B19H, 40BEA3ECH, 40966E69H, 407844D0H, 40465FD9H
        DC32 4019CC92H, 401D5AC9H, 3FE53ECDH, 400C31B1H, 4004C758H, 3FAF37B0H
        DC32 3FA4CEA3H, 0H, 0H, 0H, 0H, 416547AFH, 410B5F54H, 40D1DC3CH
        DC32 40AFBA88H, 40A3C283H, 4089E1ACH, 404BDF93H, 4055BAB2H, 40388659H
        DC32 4035C919H, 40292C62H, 4020536DH, 4012FC26H, 0H, 0H, 0H, 41AAD500H
        DC32 4176A7DCH, 412F4102H, 410C78EAH, 40BBBA36H, 40A13C06H, 40A39FC7H
        DC32 408CA878H, 409130BEH, 4063CCFBH, 40750825H, 40496F11H, 40582C56H
        DC32 4031EB85H, 4031EB85H, 4031EB85H, 41A029C8H, 41827D57H, 4189AE06H
        DC32 41799B93H, 412F33F3H, 40F8C03AH, 40C7C8B8H, 40ECD1D2H, 40D58162H
        DC32 40C44949H, 40B0F5C0H, 40989F77H, 4090E679H, 408051ECH, 408051ECH
        DC32 408051ECH, 41921CACH, 41776D18H, 4179860DH, 41765508H, 414FCAEDH
        DC32 41556D5CH, 410E5CE4H, 410ED65BH, 410086ADH, 40D89BFAH, 40BA58C3H
        DC32 40CD387EH, 40C2DB64H, 40B75C29H, 40B75C29H, 40B75C29H, 418B513DH
        DC32 4175DCC7H, 4172FF80H, 41756A17H, 4174E7EEH, 417630B3H, 41678866H
        DC32 413F1157H, 412FC4D7H, 411F4D6AH, 41104E47H, 4107109CH, 40F07C83H
        DC32 40DCCCCDH, 40DCCCCDH, 40DCCCCDH, 41880AECH, 4175F4F3H, 417302B4H
        DC32 417473C9H, 417536CAH, 4178FC7FH, 417A38F5H, 4176A5A5H, 415A1617H
        DC32 412EB34CH, 4138220EH, 412A8F67H, 4118D43DH, 4113AE14H, 4113AE14H
        DC32 4113AE14H, 4187D495H, 4175C3FAH, 4172BAB9H, 4174D3AEH, 41754DE8H
        DC32 417AF34EH, 417CF7BAH, 417B3D40H, 417D3FEDH, 4171D6E4H, 416AA7F1H
        DC32 413E8AA3H, 4140BF61H, 4113AE14H, 4113AE14H, 4113AE14H, 4187CE07H
        DC32 41757D46H, 4174E435H, 41749842H, 41749EEEH, 4177BED4H, 417EDC96H
        DC32 417C7C61H, 417AB464H, 417D6318H, 4180FC97H, 4170DFF2H, 416F8FB7H
        DC32 4169C28FH, 4169C28FH, 4169C28FH, 4187B1E8H, 4175E4EEH, 41735D96H
        DC32 4174A677H, 41743825H, 4177C5BFH, 417D3CC2H, 417CD6C6H, 417AD89EH
        DC32 417D3BE2H, 417FF459H, 41803841H, 4177A7DBH, 41730A3DH, 41730A3DH
        DC32 41730A3DH, 4188147BH, 41740000H, 417AE148H, 41770A3DH, 417451ECH
        DC32 4177851FH, 417CCCCDH, 417BAE14H, 4179999AH, 417CA3D7H, 4180F5C3H
        DC32 4180E148H, 417A147BH, 41726666H, 41726666H, 41726666H
//   29 /* RPM					800.000000	1213.333374	1626.666748	2040.000000	2453.333496	2866.666748	3280.000000	3693.333496	4106.666992	4520.000000	4933.333496	5346.666992	5760.000000	6173.333496	6586.666992	7000.000000*/
//   30 /* Load 1.200000 */{	1.542000,	1.547600,	1.551867,	1.570000,	1.550000,	0.908666,	0.794800,	0.775200,	0.791733,	0.800000,	0.798667,	0.805733,	0.810000,	0.810000,	0.810000,	0.810000},
//   31 /* Load 1.413333 */{	1.532133,	1.565325,	1.551244,	1.552773,	1.546018,	0.802089,	0.810000,	0.788507,	0.808898,	0.744987,	0.701378,	0.711404,	0.744667,	0.810000,	0.810000,	0.810000},
//   32 /* Load 1.626667 */{	1.543600,	1.545573,	1.555956,	1.545973,	1.415333,	0.115288,	0.000000,	0.000000,	0.000000,	0.000000,	0.000000,	0.000000,	0.000000,	0.000000,	0.000000,	0.000000},
//   33 /* Load 1.840000 */{	1.538800,	1.741893,	1.558426,	1.591440,	1.228773,	0.000000,	0.000000,	0.000000,	0.000000,	0.000000,	0.000000,	0.000000,	0.000000,	0.000000,	0.000000,	0.000000},
//   34 /* Load 2.053333 */{	2.322933,	3.439928,	2.818523,	2.242266,	2.125839,	1.194041,	0.488959,	0.000000,	0.000000,	0.000000,	0.000000,	0.000000,	0.000000,	0.000000,	0.000000,	0.000000},
//   35 /* Load 2.266667 */{	7.696667,	5.957510,	4.700978,	3.879200,	3.099600,	2.403111,	2.458666,	1.790979,	2.190533,	2.074667,	1.368887,	1.287556,	0.000000,	0.000000,	0.000000,	0.000000},
//   36 /* Load 2.480000 */{	14.330001,	8.710773,	6.558134,	5.491520,	5.117494,	4.308798,	3.185521,	3.339520,	2.883200,	2.840399,	2.643334,	2.505092,	2.296640,	0.000000,	0.000000,	0.000000},
//   37 /* Load 2.693333 */{	21.354004,	15.415981,	10.953371,	8.779520,	5.866481,	5.038577,	5.113254,	4.395565,	4.537200,	3.559386,	3.828622,	3.147404,	3.377706,	2.780000,	2.780000,	2.780000},
//   38 /* Load 2.906667 */{	20.020401,	16.311201,	17.209972,	15.600482,	10.950183,	7.773465,	6.243252,	7.400613,	6.672044,	6.133946,	5.529999,	4.769466,	4.528134,	4.010000,	4.010000,	4.010000},
//   39 /* Load 3.120000 */{	18.264000,	15.464134,	15.595227,	15.395760,	12.987042,	13.339199,	8.897678,	8.927333,	8.032880,	6.769040,	5.823335,	6.413146,	6.089281,	5.730000,	5.730000,	5.730000},
//   40 /* Load 3.333333 */{	17.414667,	15.366401,	15.187378,	15.338401,	15.306623,	15.386889,	14.470800,	11.941733,	10.985557,	9.956400,	9.019111,	8.441555,	7.515199,	6.900000,	6.900000,	6.900000},
//   41 /* Load 3.546667 */{	17.005333,	15.372302,	15.188160,	15.278268,	15.325876,	15.561645,	15.638906,	15.415441,	13.630393,	10.918774,	11.508314,	10.660010,	9.551816,	9.230000,	9.230000,	9.230000},
//   42 /* Load 3.760000 */{	16.978800,	15.360346,	15.170587,	15.301680,	15.331520,	15.684401,	15.810480,	15.702454,	15.828107,	15.114964,	14.666001,	11.908847,	12.046723,	9.230000,	9.230000,	9.230000},
//   43 /* Load 3.973333 */{	16.975599,	15.343084,	15.305715,	15.287172,	15.288801,	15.484089,	15.928854,	15.780366,	15.669041,	15.836693,	16.123335,	15.054674,	14.972587,	14.610000,	14.610000,	14.610000},
//   44 /* Load 4.186667 */{	16.961868,	15.368391,	15.210348,	15.290641,	15.263707,	15.485778,	15.827333,	15.802435,	15.677885,	15.827120,	15.997155,	16.027468,	15.478480,	15.190000,	15.190000,	15.190000},
//   45 /* Load 4.400000 */{	17.010000,	15.250000,	15.680000,	15.440000,	15.270000,	15.470000,	15.800000,	15.730000,	15.600000,	15.790000,	16.120001,	16.110001,	15.630000,	15.150000,	15.150000,	15.150000}
//   46 };
//   47 

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//   48 static const ignition_table_t default_aspire_timing_table = {
default_aspire_timing_table:
        DATA
        DC32 0BF2978D5H, 40F75C29H, 4185C6A8H, 41B91EB8H, 41EB2F1BH, 41FA24DDH
        DC32 42006E98H, 41F37CEEH, 41F72B02H, 41D53F7DH, 41C5A1CBH, 41C8DD2FH
        DC32 41C90E56H, 41CBAE14H, 41CBAE14H, 41CBAE14H, 0BF0BC6A8H, 40F52F1BH
        DC32 41870E56H, 41BBDB23H, 41EC28F6H, 41FA978DH, 42006E98H, 41F53F7DH
        DC32 41F3BE77H, 41D70A3DH, 41C5F7CFH, 41C5EF9EH, 41E84189H, 41CC7EFAH
        DC32 41CC7EFAH, 41CC7EFAH, 0BF158106H, 40FBD70AH, 4185B646H, 41B8353FH
        DC32 41EC5604H, 41F953F8H, 4200B439H, 41F451ECH, 41F224DDH, 41D353F8H
        DC32 41C1126FH, 41C81062H, 41C59581H, 41D15810H, 41D15810H, 41D15810H
        DC32 0BF158106H, 40F50E56H, 4185B646H, 41BA0831H, 41EAD0E5H, 41F7D2F2H
        DC32 42009168H, 41F6FDF4H, 41F72B02H, 41D41062H, 41C476C9H, 41C31EB8H
        DC32 41CE1893H, 420DF7CFH, 420DF7CFH, 420DF7CFH, 0BF158106H, 40FB9581H
        DC32 41844DD3H, 41BA0831H, 41E9DB23H, 41FA5E35H, 4202147BH, 41F122D1H
        DC32 41F31AA0H, 41CF3B64H, 41C2F5C3H, 41C0A7F0H, 41C65604H, 41C2D0E5H
        DC32 41C2D0E5H, 41C2D0E5H, 3FAE978DH, 40F73B64H, 418672B0H, 41BA0831H
        DC32 41ED1EB8H, 41F80C4AH, 42013127H, 41F74BC7H, 41EF0E56H, 41D322D1H
        DC32 41C54FDFH, 41D9DF3BH, 41CAFDF4H, 41CB5810H, 41CB5810H, 41CB5810H
        DC32 0BFAE978DH, 4127D70AH, 4185A5E3H, 41B38937H, 41E0D0E5H, 41F1E76DH
        DC32 420174BCH, 41F5C28FH, 41F72B02H, 41D4DD2FH, 41C54FDFH, 41C374BCH
        DC32 41C40000H, 41CC147BH, 41CC147BH, 41CC147BH, 0C11DD2F2H, 4126A7F0H
        DC32 413AE148H, 41993333H, 41CE0831H, 41DF7CEEH, 4202374CH, 41F53F7DH
        DC32 41F13B64H, 41D978D5H, 41CA0419H, 41B67EFAH, 41C39DB2H, 41C9C28FH
        DC32 41C9C28FH, 41C9C28FH, 0C11DDB23H, 0C0AE76C9H, 0C036A7F0H
        DC32 4189B22DH, 418C6A7FH, 41A58106H, 41CD47AEH, 41DE7AE1H, 41DD8937H
        DC32 41D9CAC1H, 41CF0E56H, 41CAE148H, 41D0CCCDH, 41DFEF9EH, 41DFEF9EH
        DC32 41DFEF9EH, 0C11DD2F2H, 0C0AE76C9H, 0C036A7F0H, 3EAF1AA0H
        DC32 41486A7FH, 4181BE77H, 41AAE979H, 41DCB852H, 41CE3D71H, 41C15C29H
        DC32 41C54FDFH, 41CCAC08H, 41D3EB85H, 41FFBE77H, 41FFBE77H, 41FFBE77H
        DC32 0C11DD2F2H, 0C0B08312H, 0C036A7F0H, 3E676C8BH, 402F3B64H
        DC32 40743958H, 413EC8B4H, 419676C9H, 41A84DD3H, 41AC4DD3H, 41A9AE14H
        DC32 41B1D2F2H, 41C85E35H, 41C93F7DH, 41C93F7DH, 41C93F7DH, 0C11DDB23H
        DC32 0C0B09375H, 0C036A7F0H, 80000000H, 40416873H, 40743958H, 40CDB22DH
        DC32 40F9374CH, 419B6873H, 41A6E148H, 419FBA5EH, 41A83D71H, 41AB2B02H
        DC32 41AC8F5CH, 41AC8F5CH, 41AC8F5CH, 0C11DD2F2H, 0C0B08312H
        DC32 0C0316873H, 3E676C8BH, 402ED917H, 40600000H, 40D98937H, 4101A1CBH
        DC32 410A8F5CH, 41180000H, 413C9BA6H, 41A10E56H, 41A09375H, 41A4147BH
        DC32 41A4147BH, 41A4147BH, 0C11DD2F2H, 0C0B09375H, 0C036A7F0H
        DC32 80000000H, 403851ECH, 40743958H, 40CD70A4H, 41051EB8H, 4106D0E5H
        DC32 41088312H, 4137851FH, 4137126FH, 4159C28FH, 41405A1DH, 41405A1DH
        DC32 41405A1DH, 0C11C0000H, 0C0B09375H, 0C026A7F0H, 80000000H
        DC32 403851ECH, 4069DB23H, 40C1999AH, 40DC6A7FH, 4105F3B6H, 4115D2F2H
        DC32 413C3958H, 413BB646H, 413DD2F2H, 41460419H, 41460419H, 41460419H
        DC32 0BEB33333H, 0C0B2E148H, 0BF008312H, 0BF68F5C3H, 0BF5D2F1BH
        DC32 0BF743958H, 0BFA978D5H, 40EDF3B6H, 0BF95C28FH, 0BF86E979H
        DC32 0C003B646H, 0C00645A2H, 0C028B439H, 4145A1CBH, 4145A1CBH
        DC32 4145A1CBH
//   49 /* RPM					800.000000	1213.333374	1626.666748	2040.000000	2453.333496	2866.666748	3280.000000	3693.333496	4106.666992	4520.000000	4933.333496	5346.666992	5760.000000	6173.333496	6586.666992	7000.000000*/
//   50 /* Load 1.200000 */{	-0.662000,	+7.730000,	+16.722000,	+23.139999,	+29.398001,	+31.268000,	+32.108002,	+30.436001,	+30.896000,	+26.656000,	+24.704000,	+25.108000,	+25.132000,	+25.459999,	+25.459999,	+25.459999},
//   51 /* Load 1.413333 */{	-0.546000,	+7.662000,	+16.882000,	+23.482000,	+29.520000,	+31.323999,	+32.108002,	+30.656000,	+30.468000,	+26.879999,	+24.746000,	+24.742001,	+29.032000,	+25.562000,	+25.562000,	+25.562000},
//   52 /* Load 1.626667 */{	-0.584000,	+7.870000,	+16.714001,	+23.025999,	+29.542000,	+31.166000,	+32.175999,	+30.540001,	+30.268000,	+26.416000,	+24.134001,	+25.007999,	+24.698000,	+26.167999,	+26.167999,	+26.167999},
//   53 /* Load 1.840000 */{	-0.584000,	+7.658000,	+16.714001,	+23.254000,	+29.351999,	+30.978001,	+32.141998,	+30.874001,	+30.896000,	+26.507999,	+24.558001,	+24.389999,	+25.761999,	+35.492001,	+35.492001,	+35.492001},
//   54 /* Load 2.053333 */{	-0.584000,	+7.862000,	+16.538000,	+23.254000,	+29.232000,	+31.296000,	+32.520000,	+30.142000,	+30.388000,	+25.903999,	+24.370001,	+24.082001,	+24.792000,	+24.351999,	+24.351999,	+24.351999},
//   55 /* Load 2.266667 */{	+1.364000,	+7.726000,	+16.806000,	+23.254000,	+29.639999,	+31.006001,	+32.298000,	+30.912001,	+29.882000,	+26.392000,	+24.664000,	+27.233999,	+25.374001,	+25.417999,	+25.417999,	+25.417999},
//   56 /* Load 2.480000 */{	-1.364000,	+10.490000,	+16.705999,	+22.441999,	+28.101999,	+30.238001,	+32.363998,	+30.719999,	+30.896000,	+26.608000,	+24.664000,	+24.431999,	+24.500000,	+25.510000,	+25.510000,	+25.510000},
//   57 /* Load 2.693333 */{	-9.864000,	+10.416000,	+11.680000,	+19.150000,	+25.754000,	+27.936001,	+32.554001,	+30.656000,	+30.153999,	+27.184000,	+25.252001,	+22.812000,	+24.452000,	+25.219999,	+25.219999,	+25.219999},
//   58 /* Load 2.906667 */{	-9.866000,	-5.452000,	-2.854000,	+17.212000,	+17.552000,	+20.688000,	+25.660000,	+27.809999,	+27.691999,	+27.224001,	+25.882000,	+25.360001,	+26.100000,	+27.992001,	+27.992001,	+27.992001},
//   59 /* Load 3.120000 */{	-9.864000,	-5.452000,	-2.854000,	+0.342000,	+12.526000,	+16.218000,	+21.364000,	+27.590000,	+25.780001,	+24.170000,	+24.664000,	+25.584000,	+26.490000,	+31.968000,	+31.968000,	+31.968000},
//   60 /* Load 3.333333 */{	-9.864000,	-5.516000,	-2.854000,	+0.226000,	+2.738000,	+3.816000,	+11.924000,	+18.808001,	+21.038000,	+21.538000,	+21.209999,	+22.228001,	+25.046000,	+25.156000,	+25.156000,	+25.156000},
//   61 /* Load 3.546667 */{	-9.866000,	-5.518000,	-2.854000,	-0.000000,	+3.022000,	+3.816000,	+6.428000,	+7.788000,	+19.426001,	+20.860001,	+19.966000,	+21.030001,	+21.396000,	+21.570000,	+21.570000,	+21.570000},
//   62 /* Load 3.760000 */{	-9.864000,	-5.516000,	-2.772000,	+0.226000,	+2.732000,	+3.500000,	+6.798000,	+8.102000,	+8.660000,	+9.500000,	+11.788000,	+20.132000,	+20.072001,	+20.510000,	+20.510000,	+20.510000},
//   63 /* Load 3.973333 */{	-9.864000,	-5.518000,	-2.854000,	-0.000000,	+2.880000,	+3.816000,	+6.420000,	+8.320000,	+8.426000,	+8.532000,	+11.470000,	+11.442000,	+13.610000,	+12.022000,	+12.022000,	+12.022000},
//   64 /* Load 4.186667 */{	-9.750000,	-5.518000,	-2.604000,	-0.000000,	+2.880000,	+3.654000,	+6.050000,	+6.888000,	+8.372000,	+9.364000,	+11.764000,	+11.732000,	+11.864000,	+12.376000,	+12.376000,	+12.376000},
//   65 /* Load 4.400000 */{	-0.350000,	-5.590000,	-0.502000,	-0.910000,	-0.864000,	-0.954000,	-1.324000,	+7.436000,	-1.170000,	-1.054000,	-2.058000,	-2.098000,	-2.636000,	+12.352000,	+12.352000,	+12.352000}
//   66 };
//   67 
//   68 EXTERN_ENGINE;
//   69 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z20setDefaultAspireMapsv
        THUMB
//   70 static void setDefaultAspireMaps(DECLARE_ENGINE_PARAMETER_F) {
_Z20setDefaultAspireMapsv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   71 
//   72 	setFuelLoadBin(1.2, 4.4 PASS_ENGINE_PARAMETER);
        VLDR.W   S1,??DataTable1  ;; 0x408ccccd
        VLDR.W   S0,??DataTable1_1  ;; 0x3f99999a
          CFI FunCall _Z14setFuelLoadBinff
        BL       _Z14setFuelLoadBinff
//   73 	setFuelRpmBin(800, 7000 PASS_ENGINE_PARAMETER);
        VLDR.W   S1,??DataTable1_2  ;; 0x45dac000
        VLDR.W   S0,??DataTable1_3  ;; 0x44480000
          CFI FunCall _Z13setFuelRpmBinff
        BL       _Z13setFuelRpmBinff
//   74 	setTimingLoadBin(1.2, 4.4 PASS_ENGINE_PARAMETER);
        VLDR.W   S1,??DataTable1  ;; 0x408ccccd
        VLDR.W   S0,??DataTable1_1  ;; 0x3f99999a
          CFI FunCall _Z16setTimingLoadBinff
        BL       _Z16setTimingLoadBinff
//   75 	setTimingRpmBin(800, 7000 PASS_ENGINE_PARAMETER);
        VLDR.W   S1,??DataTable1_2  ;; 0x45dac000
        VLDR.W   S0,??DataTable1_3  ;; 0x44480000
          CFI FunCall _Z15setTimingRpmBinff
        BL       _Z15setTimingRpmBinff
//   76 
//   77 	copyFuelTable(default_aspire_fuel_table, config->fuelTable);
        LDR.N    R0,??DataTable1_9
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+10496
        ADDS     R1,R1,#+184
        LDR.N    R0,??DataTable1_10
          CFI FunCall _Z13copyFuelTablePA16_KfPA16_f
        BL       _Z13copyFuelTablePA16_KfPA16_f
//   78 	copyTimingTable(default_aspire_timing_table, config->ignitionTable);
        LDR.N    R0,??DataTable1_9
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+11776
        ADDS     R1,R1,#+56
        LDR.N    R0,??DataTable1_11
          CFI FunCall _Z15copyTimingTablePA16_KfPA16_f
        BL       _Z15copyTimingTablePA16_KfPA16_f
//   79 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0
//   80 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z32setFordAspireEngineConfigurationv
        THUMB
//   81 void setFordAspireEngineConfiguration(DECLARE_ENGINE_PARAMETER_F) {
_Z32setFordAspireEngineConfigurationv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   82 	engineConfiguration->tpsMin = 100;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        MOVS     R1,#+100
        STRH     R1,[R0, #+82]
//   83 	engineConfiguration->tpsMax = 750;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        MOVW     R1,#+750
        STRH     R1,[R0, #+84]
//   84 
//   85 	engineConfiguration->rpmHardLimit = 7000;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        MOVW     R1,#+7000
        STR      R1,[R0, #+420]
//   86 
//   87 	/**
//   88 	 * 18K Ohm @ -20C
//   89 	 * 2.1K Ohm @ 24C
//   90 	 * 1K Ohm @ 49C
//   91 	 */
//   92 	setThermistorConfiguration(&engineConfiguration->clt, -20, 18000, 23.8889, 2100, 48.8889, 1000);
        VLDR.W   S5,??DataTable1_4  ;; 0x447a0000
        VLDR.W   S4,??DataTable1_5  ;; 0x42438e3c
        VLDR.W   S3,??DataTable1_6  ;; 0x45034000
        VLDR.W   S2,??DataTable1_7  ;; 0x41bf1c78
        VLDR.W   S1,??DataTable1_8  ;; 0x468ca000
        VMOV.F32 S0,#-20.0
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+256
          CFI FunCall _Z26setThermistorConfigurationP14ThermistorConfffffff
        BL       _Z26setThermistorConfigurationP14ThermistorConfffffff
//   93 	engineConfiguration->clt.config.bias_resistor = 3300; // that's my custom resistor value!
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable1_13  ;; 0x454e4000
        STR      R1,[R0, #+280]
//   94 
//   95 	engineConfiguration->cranking.baseFuel = 3;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable1_14  ;; 0x40400000
        STR      R1,[R0, #+92]
//   96 
//   97 //	engineConfiguration->ignitionPinMode = OM_INVERTED;
//   98 
//   99 	engineConfiguration->specs.cylindersCount = 4;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        MOVS     R1,#+4
        STR      R1,[R0, #+404]
//  100 	engineConfiguration->specs.displacement = 1.3;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable1_15  ;; 0x3fa66666
        STR      R1,[R0, #+400]
//  101 	// Denso 195500-2110
//  102 	engineConfiguration->injector.flow = 119.8;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable1_16  ;; 0x42ef999a
        STR      R1,[R0, #+8]
//  103 
//  104 	engineConfiguration->specs.firingOrder = FO_1_THEN_3_THEN_4_THEN2;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+408]
//  105 	engineConfiguration->globalTriggerAngleOffset = 175;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable1_17  ;; 0x432f0000
        STR      R1,[R0, #+460]
//  106 	engineConfiguration->ignitionOffset = 87;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable1_18  ;; 0x42ae0000
        STR      R1,[R0, #+448]
//  107 	engineConfiguration->injectionOffset = 54 + 360;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable1_19  ;; 0x43cf0000
        STR      R1,[R0, #+436]
//  108 
//  109 	setDefaultAspireMaps(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z20setDefaultAspireMapsv
        BL       _Z20setDefaultAspireMapsv
//  110 	// set_cranking_rpm 550
//  111 	engineConfiguration->cranking.rpm = 550;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        MOVW     R1,#+550
        STRH     R1,[R0, #+96]
//  112 	// set_cranking_charge_angle 70
//  113 	engineConfiguration->crankingChargeAngle = 70;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable1_20  ;; 0x428c0000
        STR      R1,[R0, #+108]
//  114 	// set_cranking_timing_angle 37
//  115 	engineConfiguration->crankingTimingAngle = -37;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable1_21  ;; 0xc2140000
        STR      R1,[R0, #+440]
//  116 
//  117 	setSingleCoilDwell(engineConfiguration);
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
          CFI FunCall _Z18setSingleCoilDwellP22engine_configuration_s
        BL       _Z18setSingleCoilDwellP22engine_configuration_s
//  118 	engineConfiguration->ignitionMode = IM_ONE_COIL;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+444]
//  119 	setOperationMode(engineConfiguration, FOUR_STROKE_CAM_SENSOR);
        MOVS     R1,#+2
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
          CFI FunCall _Z16setOperationModeP22engine_configuration_s16operation_mode_e
        BL       _Z16setOperationModeP22engine_configuration_s16operation_mode_e
//  120 	engineConfiguration->useOnlyFrontForTrigger = true;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        ORRS     R0,R0,#0x40
        LDR.N    R1,??DataTable1_12
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1509]
//  121 	engineConfiguration->trigger.type = TT_FORD_ASPIRE;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+536]
//  122 
//  123 	boardConfiguration->injectionPins[4] = GPIO_UNASSIGNED;
        LDR.N    R0,??DataTable1_22
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+48]
//  124 	boardConfiguration->injectionPins[5] = GPIO_UNASSIGNED;
        LDR.N    R0,??DataTable1_22
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+52]
//  125 
//  126 	engineConfiguration->HD44780width = 20;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        MOVS     R1,#+20
        STR      R1,[R0, #+516]
//  127 	engineConfiguration->HD44780height = 4;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        MOVS     R1,#+4
        STR      R1,[R0, #+520]
//  128 
//  129 	// Frankenstein analog input #1: adc1
//  130 	// Frankenstein analog input #2: adc3
//  131 	// Frankenstein analog input #3: adc13
//  132 	// Frankenstein analog input #4: adc11
//  133 	// Frankenstein analog input #5: adc
//  134 	// Frankenstein analog input #6: adc
//  135 	// Frankenstein analog input #7: adc
//  136 	// Frankenstein analog input #8: adc
//  137 	// Frankenstein analog input #9: adc
//  138 	// Frankenstein analog input #10: adc
//  139 	// Frankenstein analog input #11: adc
//  140 	// Frankenstein analog input #12: adc
//  141 
//  142 
//  143 	engineConfiguration->tpsAdcChannel = EFI_ADC_3;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        MOVS     R1,#+3
        STR      R1,[R0, #+524]
//  144 	engineConfiguration->vbattAdcChannel = EFI_ADC_0;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+472]
//  145 	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        MOVS     R1,#+4
        STR      R1,[R0, #+252]
//  146 	engineConfiguration->mafAdcChannel = EFI_ADC_1;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+580]
//  147 	engineConfiguration->clt.adcChannel = EFI_ADC_11;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        MOVS     R1,#+11
        STR      R1,[R0, #+284]
//  148 //	engineConfiguration->iat.adcChannel =
//  149 
//  150 	engineConfiguration->map.sensor.type = MT_DENSO183;
        LDR.N    R0,??DataTable1_12
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+248]
//  151 }
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
        DC32     0x45dac000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_3:
        DC32     0x44480000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_4:
        DC32     0x447a0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_5:
        DC32     0x42438e3c

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_6:
        DC32     0x45034000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_7:
        DC32     0x41bf1c78

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_8:
        DC32     0x468ca000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_9:
        DC32     config

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_10:
        DC32     default_aspire_fuel_table

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_11:
        DC32     default_aspire_timing_table

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_12:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_13:
        DC32     0x454e4000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_14:
        DC32     0x40400000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_15:
        DC32     0x3fa66666

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_16:
        DC32     0x42ef999a

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_17:
        DC32     0x432f0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_18:
        DC32     0x42ae0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_19:
        DC32     0x43cf0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_20:
        DC32     0x428c0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_21:
        DC32     0xc2140000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_22:
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
//  152 
//  153 #endif /* EFI_SUPPORT_FORD_ASPIRE */
// 
// 2 048 bytes in section .rodata
//   524 bytes in section .text
// 
//   524 bytes of CODE  memory
// 2 048 bytes of CONST memory
//
//Errors: none
//Warnings: 1
