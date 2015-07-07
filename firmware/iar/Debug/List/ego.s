///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:45 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\sen /
//                       sors\ego.cpp                                         /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\sen /
//                       sors\ego.cpp -lCN F:\stuff\rusefi_sourceforge\firmwa /
//                       re\iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge\f /
//                       irmware\iar\Debug\List\ -o                           /
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
//                       ego.s                                                /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME ego

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z11interpolatefffff
        EXTERN _Z19getInternalAdcValuePKc13adc_channel_e
        EXTERN boardConfiguration
        EXTERN engineConfiguration
        EXTERN firmwareError

        PUBLIC _Z12setEgoSensor12ego_sensor_e
        PUBLIC _Z6getAfrv
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\sensors\ego.cpp
//    1 #include "main.h"
//    2 #include "engine_configuration.h"
//    3 #include "interpolation.h"
//    4 #include "adc_inputs.h"
//    5 #include "engine.h"
//    6 
//    7 EXTERN_ENGINE;
//    8 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z6getAfrv
        THUMB
//    9 float getAfr(DECLARE_ENGINE_PARAMETER_F) {
_Z6getAfrv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
//   10 	afr_sensor_s * sensor = &engineConfiguration->afr;
        LDR.N    R0,??DataTable2_2
        LDR      R0,[R0, #+0]
        ADDS     R4,R0,#+584
//   11 
//   12 	float volts = getVoltageDivided("ego", sensor->hwChannel);
        LDR      R1,[R4, #+0]
        ADR.N    R0,??DataTable2  ;; "ego"
          CFI FunCall _Z19getInternalAdcValuePKc13adc_channel_e
        BL       _Z19getInternalAdcValuePKc13adc_channel_e
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        VLDR.W   S1,??DataTable2_1  ;; 0x3a400c01
        VMUL.F32 S0,S0,S1
        LDR.N    R0,??DataTable2_2
        LDR      R0,[R0, #+0]
        VLDR     S1,[R0, #+464]
        VMUL.F32 S16,S0,S1
//   13 
//   14 	return interpolate(sensor->v1, sensor->value1, sensor->v2, sensor->value2, volts);
        VMOV.F32 S4,S16
        VLDR     S3,[R4, #+16]
        VLDR     S2,[R4, #+12]
        VLDR     S1,[R4, #+8]
        VLDR     S0,[R4, #+4]
          CFI FunCall _Z11interpolatefffff
        BL       _Z11interpolatefffff
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0
//   15 }
//   16 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z13initEgoSensorP12afr_sensor_s12ego_sensor_e
        THUMB
//   17 static void initEgoSensor(afr_sensor_s *sensor, ego_sensor_e type) {
_Z13initEgoSensorP12afr_sensor_s12ego_sensor_e:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   18 
//   19 	switch (type) {
        CMP      R5,#+0
        BEQ.N    ??initEgoSensor_0
        CMP      R5,#+2
        BEQ.N    ??initEgoSensor_1
        BCC.N    ??initEgoSensor_2
        CMP      R5,#+4
        BEQ.N    ??initEgoSensor_3
        BCC.N    ??initEgoSensor_4
        B.N      ??initEgoSensor_5
//   20 	case ES_BPSX_D1:
//   21 		/**
//   22 		 * This decodes BPSX D1 Wideband Controller analog signal
//   23 		 */
//   24 		sensor->v1 = 0;
??initEgoSensor_0:
        MOVS     R0,#+0
        STR      R0,[R4, #+4]
//   25 		sensor->value1 = 9;
        LDR.N    R0,??DataTable2_3  ;; 0x41100000
        STR      R0,[R4, #+8]
//   26 		sensor->v2 = 5;
        LDR.N    R0,??DataTable2_4  ;; 0x40a00000
        STR      R0,[R4, #+12]
//   27 		sensor->value2 = 19;
        LDR.N    R0,??DataTable2_5  ;; 0x41980000
        STR      R0,[R4, #+16]
//   28 		break;
        B.N      ??initEgoSensor_6
//   29 
//   30 	case ES_Innovate_MTX_L:
//   31 		sensor->v1 = 0;
??initEgoSensor_2:
        MOVS     R0,#+0
        STR      R0,[R4, #+4]
//   32 		sensor->value1 = 7.35;
        LDR.N    R0,??DataTable2_6  ;; 0x40eb3333
        STR      R0,[R4, #+8]
//   33 		sensor->v2 = 5;
        LDR.N    R0,??DataTable2_4  ;; 0x40a00000
        STR      R0,[R4, #+12]
//   34 		sensor->value2 = 22.39;
        LDR.N    R0,??DataTable2_7  ;; 0x41b31eb8
        STR      R0,[R4, #+16]
//   35 		break;
        B.N      ??initEgoSensor_6
//   36 	case ES_14Point7_Free:
//   37 		sensor->v1 = 0;
??initEgoSensor_1:
        MOVS     R0,#+0
        STR      R0,[R4, #+4]
//   38 		sensor->value1 = 9.996;
        LDR.N    R0,??DataTable2_8  ;; 0x411fef9e
        STR      R0,[R4, #+8]
//   39 		sensor->v2 = 5;
        LDR.N    R0,??DataTable2_4  ;; 0x40a00000
        STR      R0,[R4, #+12]
//   40 		sensor->value2 = 19.992;
        LDR.N    R0,??DataTable2_9  ;; 0x419fef9e
        STR      R0,[R4, #+16]
//   41 		break;
        B.N      ??initEgoSensor_6
//   42 		// technically 14Point7 and PLX use the same scale
//   43 	case ES_PLX:
//   44 		sensor->v1 = 0;
??initEgoSensor_3:
        MOVS     R0,#+0
        STR      R0,[R4, #+4]
//   45 		sensor->value1 = 10;
        LDR.N    R0,??DataTable2_10  ;; 0x41200000
        STR      R0,[R4, #+8]
//   46 		sensor->v2 = 5;
        LDR.N    R0,??DataTable2_4  ;; 0x40a00000
        STR      R0,[R4, #+12]
//   47 		sensor->value2 = 20;
        LDR.N    R0,??DataTable2_11  ;; 0x41a00000
        STR      R0,[R4, #+16]
//   48 		break;
        B.N      ??initEgoSensor_6
//   49 	case ES_NarrowBand:
//   50 		sensor->v1 = 0.1;
??initEgoSensor_4:
        LDR.N    R0,??DataTable2_12  ;; 0x3dcccccd
        STR      R0,[R4, #+4]
//   51 		sensor->value1 = 15;
        LDR.N    R0,??DataTable2_13  ;; 0x41700000
        STR      R0,[R4, #+8]
//   52 		sensor->v2 = 0.9;
        LDR.N    R0,??DataTable2_14  ;; 0x3f666666
        STR      R0,[R4, #+12]
//   53 		sensor->value2 = 14;
        LDR.N    R0,??DataTable2_15  ;; 0x41600000
        STR      R0,[R4, #+16]
//   54 		break;
        B.N      ??initEgoSensor_6
//   55 	default:
//   56 		firmwareError("Unexpected EGO %d", type);
??initEgoSensor_5:
        MOVS     R1,R5
        LDR.N    R0,??DataTable2_16
          CFI FunCall firmwareError
        BL       firmwareError
//   57 		break;
//   58 	}
//   59 }
??initEgoSensor_6:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock1
//   60 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z12setEgoSensor12ego_sensor_e
        THUMB
//   61 void setEgoSensor(ego_sensor_e type DECLARE_ENGINE_PARAMETER_S) {
_Z12setEgoSensor12ego_sensor_e:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   62 	boardConfiguration->afr_type = type;
        LDR.N    R0,??DataTable2_17
        LDR      R0,[R0, #+0]
        STR      R4,[R0, #+276]
//   63 	initEgoSensor(&engineConfiguration->afr, type);
        MOVS     R1,R4
        LDR.N    R0,??DataTable2_2
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+584
          CFI FunCall _Z13initEgoSensorP12afr_sensor_s12ego_sensor_e
        BL       _Z13initEgoSensorP12afr_sensor_s12ego_sensor_e
//   64 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2:
        DC8      "ego"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_1:
        DC32     0x3a400c01

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_2:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_3:
        DC32     0x41100000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_4:
        DC32     0x40a00000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_5:
        DC32     0x41980000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_6:
        DC32     0x40eb3333

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_7:
        DC32     0x41b31eb8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_8:
        DC32     0x411fef9e

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_9:
        DC32     0x419fef9e

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_10:
        DC32     0x41200000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_11:
        DC32     0x41a00000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_12:
        DC32     0x3dcccccd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_13:
        DC32     0x41700000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_14:
        DC32     0x3f666666

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_15:
        DC32     0x41600000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_16:
        DC32     `?<Constant "Unexpected EGO %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_17:
        DC32     boardConfiguration

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "ego"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Unexpected EGO %d">`:
        DATA
        DC8 "Unexpected EGO %d"
        DC8 0, 0

        END
// 
//  24 bytes in section .rodata
// 304 bytes in section .text
// 
// 304 bytes of CODE  memory
//  24 bytes of CONST memory
//
//Errors: none
//Warnings: 1
