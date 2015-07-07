///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:14 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\alg /
//                       o\auto_generated_enums.cpp                           /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\alg /
//                       o\auto_generated_enums.cpp -lCN                      /
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
//                       auto_generated_enums.s                               /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME auto_generated_enums

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        PUBLIC _Z14getBrain_pin_e11brain_pin_e
        PUBLIC _Z14getGpio_mode_e11gpio_mode_e
        PUBLIC _Z14getIdle_mode_e11idle_mode_e
        PUBLIC _Z15getEgo_sensor_e12ego_sensor_e
        PUBLIC _Z15getLog_format_e12log_format_e
        PUBLIC _Z15getSpi_device_e12spi_device_e
        PUBLIC _Z16getAdc_channel_e13adc_channel_e
        PUBLIC _Z16getEngine_type_e13engine_type_e
        PUBLIC _Z16getTiming_mode_e13timing_mode_e
        PUBLIC _Z17getDisplay_mode_e14display_mode_e
        PUBLIC _Z17getFiring_order_e14firing_order_e
        PUBLIC _Z17getTrigger_type_e14trigger_type_e
        PUBLIC _Z18getIgnition_mode_e15ignition_mode_e
        PUBLIC _Z18getTrigger_event_e15trigger_event_e
        PUBLIC _Z18getTrigger_value_e15trigger_value_e
        PUBLIC _Z18getTrigger_wheel_e15trigger_wheel_e
        PUBLIC _Z19getInjection_mode_e16injection_mode_e
        PUBLIC _Z19getOperation_mode_e16operation_mode_e
        PUBLIC _Z19getPin_input_mode_e16pin_input_mode_e
        PUBLIC _Z20getPin_output_mode_e17pin_output_mode_e
        PUBLIC _Z21getAdc_channel_mode_e18adc_channel_mode_e
        PUBLIC _Z21getEngine_load_mode_e18engine_load_mode_e
        PUBLIC _Z27getCranking_ignition_mode_e24cranking_ignition_mode_e
        PUBLIC _Z29getAir_pressure_sensor_type_e26air_pressure_sensor_type_e
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\algo\auto_generated_enums.cpp
//    1 // auto-generated from../../firmware/\controllers/algo/io_pins.h
//    2 // by enum2string.jar tool
//    3 
//    4 #include "main.h"
//    5 #include "io_pins.h"
//    6 // auto-generated from../../firmware/\controllers/algo/rusefi_enums.h
//    7 // by enum2string.jar tool
//    8 
//    9 #include "main.h"
//   10 #include "rusefi_enums.h"

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z16getTiming_mode_e13timing_mode_e
          CFI NoCalls
        THUMB
//   11 const char *getTiming_mode_e(timing_mode_e value) {
_Z16getTiming_mode_e13timing_mode_e:
        MOVS     R1,R0
//   12 	switch(value) {
        MOVS     R0,R1
        CMP      R0,#+0
        BEQ.N    ??getTiming_mode_e_0
        CMP      R0,#+1
        BEQ.N    ??getTiming_mode_e_1
        LDR.W    R2,??DataTable21  ;; 0x77359400
        CMP      R0,R2
        BNE.N    ??getTiming_mode_e_2
//   13 	case Internal_ForceMyEnumIntSize_timing_mode:
//   14 	  return "Internal_ForceMyEnumIntSize_timing_mode";
??getTiming_mode_e_3:
        LDR.W    R0,??DataTable21_1
        B.N      ??getTiming_mode_e_4
//   15 	case TM_DYNAMIC:
//   16 		return "TM_DYNAMIC";
??getTiming_mode_e_0:
        LDR.W    R0,??DataTable21_2
        B.N      ??getTiming_mode_e_4
//   17 	case TM_FIXED:
//   18 		return "TM_FIXED";
??getTiming_mode_e_1:
        LDR.W    R0,??DataTable21_3
        B.N      ??getTiming_mode_e_4
//   19 	}
//   20 	return NULL;
??getTiming_mode_e_2:
        MOVS     R0,#+0
??getTiming_mode_e_4:
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//   21 }
//   22 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z16getEngine_type_e13engine_type_e
          CFI NoCalls
        THUMB
//   23 const char *getEngine_type_e(engine_type_e value){
_Z16getEngine_type_e13engine_type_e:
        MOVS     R1,R0
//   24 switch(value) {
        MOVS     R0,R1
        CMP      R0,#+0
        BEQ.N    ??getEngine_type_e_0
        CMP      R0,#+1
        BEQ.N    ??getEngine_type_e_1
        CMP      R0,#+2
        BEQ.N    ??getEngine_type_e_2
        CMP      R0,#+3
        BEQ.N    ??getEngine_type_e_3
        CMP      R0,#+4
        BEQ.N    ??getEngine_type_e_4
        CMP      R0,#+5
        BEQ.W    ??getEngine_type_e_5
        CMP      R0,#+6
        BEQ.N    ??getEngine_type_e_6
        CMP      R0,#+7
        BEQ.N    ??getEngine_type_e_7
        CMP      R0,#+8
        BEQ.N    ??getEngine_type_e_8
        CMP      R0,#+9
        BEQ.N    ??getEngine_type_e_9
        CMP      R0,#+10
        BEQ.W    ??getEngine_type_e_10
        CMP      R0,#+11
        BEQ.N    ??getEngine_type_e_11
        CMP      R0,#+12
        BEQ.W    ??getEngine_type_e_12
        CMP      R0,#+13
        BEQ.N    ??getEngine_type_e_13
        CMP      R0,#+14
        BEQ.N    ??getEngine_type_e_14
        CMP      R0,#+15
        BEQ.N    ??getEngine_type_e_15
        CMP      R0,#+16
        BEQ.N    ??getEngine_type_e_16
        CMP      R0,#+17
        BEQ.N    ??getEngine_type_e_17
        CMP      R0,#+18
        BEQ.N    ??getEngine_type_e_18
        CMP      R0,#+19
        BEQ.N    ??getEngine_type_e_19
        CMP      R0,#+20
        BEQ.N    ??getEngine_type_e_20
        CMP      R0,#+21
        BEQ.N    ??getEngine_type_e_21
        CMP      R0,#+22
        BEQ.N    ??getEngine_type_e_22
        CMP      R0,#+23
        BEQ.N    ??getEngine_type_e_23
        CMP      R0,#+24
        BEQ.N    ??getEngine_type_e_24
        CMP      R0,#+25
        BEQ.N    ??getEngine_type_e_25
        CMP      R0,#+26
        BEQ.N    ??getEngine_type_e_26
        CMP      R0,#+27
        BEQ.N    ??getEngine_type_e_27
        CMP      R0,#+28
        BEQ.N    ??getEngine_type_e_28
        CMP      R0,#+29
        BEQ.N    ??getEngine_type_e_29
        CMP      R0,#+30
        BEQ.N    ??getEngine_type_e_30
        CMP      R0,#+31
        BEQ.N    ??getEngine_type_e_31
        CMP      R0,#+32
        BEQ.N    ??getEngine_type_e_32
        CMP      R0,#+33
        BEQ.N    ??getEngine_type_e_33
        LDR.W    R2,??DataTable21  ;; 0x77359400
        CMP      R0,R2
        BEQ.N    ??getEngine_type_e_34
        B.N      ??getEngine_type_e_35
//   25 case ACURA_RSX:
//   26 	return "ACURA_RSX";
??getEngine_type_e_27:
        LDR.W    R0,??DataTable21_4
        B.N      ??getEngine_type_e_36
//   27 case AUDI_AAN:
//   28   return "AUDI_AAN";
??getEngine_type_e_1:
        LDR.W    R0,??DataTable21_5
        B.N      ??getEngine_type_e_36
//   29 case SACHS:
//   30   return "SACHS";
??getEngine_type_e_29:
        LDR.W    R0,??DataTable21_6
        B.N      ??getEngine_type_e_36
//   31 case GM_2_2:
//   32   return "GM_2_2";
??getEngine_type_e_30:
        LDR.W    R0,??DataTable21_7
        B.N      ??getEngine_type_e_36
//   33 case DODGE_RAM:
//   34   return "DODGE_RAM";
??getEngine_type_e_31:
        LDR.W    R0,??DataTable21_8
        B.N      ??getEngine_type_e_36
//   35 case BMW_E34:
//   36   return "BMW_E34";
??getEngine_type_e_25:
        LDR.W    R0,??DataTable21_9
        B.N      ??getEngine_type_e_36
//   37 case CITROEN_TU3JP:
//   38   return "CITROEN_TU3JP";
??getEngine_type_e_15:
        LDR.W    R0,??DataTable21_10
        B.N      ??getEngine_type_e_36
//   39 case CUSTOM_ENGINE:
//   40   return "CUSTOM_ENGINE";
??getEngine_type_e_0:
        LDR.W    R0,??DataTable21_11
        B.N      ??getEngine_type_e_36
//   41 case DODGE_NEON_1995:
//   42   return "DODGE_NEON_1995";
??getEngine_type_e_2:
        LDR.W    R0,??DataTable21_12
        B.N      ??getEngine_type_e_36
//   43 case DODGE_NEON_2003:
//   44   return "DODGE_NEON_2003";
??getEngine_type_e_23:
        LDR.W    R0,??DataTable21_13
        B.N      ??getEngine_type_e_36
//   45 case ET_UNUSED:
//   46   return "ET_UNUSED";
??getEngine_type_e_33:
        LDR.W    R0,??DataTable21_14
        B.N      ??getEngine_type_e_36
//   47 case FORD_ASPIRE_1996:
//   48   return "FORD_ASPIRE_1996";
??getEngine_type_e_3:
        LDR.W    R0,??DataTable21_15
        B.N      ??getEngine_type_e_36
//   49 case FORD_ESCORT_GT:
//   50   return "FORD_ESCORT_GT";
??getEngine_type_e_14:
        LDR.W    R0,??DataTable21_16
        B.N      ??getEngine_type_e_36
//   51 case FORD_FIESTA:
//   52   return "FORD_FIESTA";
??getEngine_type_e_4:
        LDR.W    R0,??DataTable21_17
        B.N      ??getEngine_type_e_36
//   53 case FORD_INLINE_6_1995:
//   54   return "FORD_INLINE_6_1995";
??getEngine_type_e_7:
        LDR.W    R0,??DataTable21_18
        B.N      ??getEngine_type_e_36
//   55 case Force_4b_engine_type:
//   56   return "Force_4b_engine_type";
??getEngine_type_e_34:
        LDR.W    R0,??DataTable21_19
        B.N      ??getEngine_type_e_36
//   57 case GY6_139QMB:
//   58   return "GY6_139QMB";
??getEngine_type_e_8:
        LDR.W    R0,??DataTable21_20
        B.N      ??getEngine_type_e_36
//   59 case HONDA_ACCORD_CD:
//   60   return "HONDA_ACCORD_CD";
??getEngine_type_e_6:
        LDR.W    R0,??DataTable21_21
        B.N      ??getEngine_type_e_36
//   61 case HONDA_ACCORD_CD_DIP:
//   62   return "HONDA_ACCORD_CD_DIP";
??getEngine_type_e_18:
        LDR.W    R0,??DataTable21_22
        B.N      ??getEngine_type_e_36
//   63 case HONDA_ACCORD_CD_TWO_WIRES:
//   64   return "HONDA_ACCORD_CD_TWO_WIRES";
??getEngine_type_e_17:
        LDR.W    R0,??DataTable21_23
        B.N      ??getEngine_type_e_36
//   65 case MAZDA_323:
//   66   return "MAZDA_323";
??getEngine_type_e_11:
        LDR.W    R0,??DataTable21_24
        B.N      ??getEngine_type_e_36
//   67 case MAZDA_626:
//   68   return "MAZDA_626";
??getEngine_type_e_28:
        LDR.W    R0,??DataTable21_25
        B.N      ??getEngine_type_e_36
//   69 case MAZDA_MIATA_NB:
//   70   return "MAZDA_MIATA_NB";
??getEngine_type_e_9:
        LDR.W    R0,??DataTable21_26
        B.N      ??getEngine_type_e_36
//   71 case MIATA_1990:
//   72   return "MIATA_1990";
??getEngine_type_e_19:
        LDR.W    R0,??DataTable21_27
        B.N      ??getEngine_type_e_36
//   73 case MIATA_1994_DEVIATOR:
//   74   return "MIATA_1994_DEVIATOR";
??getEngine_type_e_20:
        LDR.W    R0,??DataTable21_28
        B.N      ??getEngine_type_e_36
//   75 case MIATA_1994_SPAGS:
//   76   return "MIATA_1994_SPAGS";
??getEngine_type_e_24:
        LDR.W    R0,??DataTable21_29
        B.N      ??getEngine_type_e_36
//   77 case MIATA_1996:
//   78   return "MIATA_1996";
??getEngine_type_e_21:
        LDR.W    R0,??DataTable21_30
        B.N      ??getEngine_type_e_36
//   79 case MINI_COOPER_R50:
//   80   return "MINI_COOPER_R50";
??getEngine_type_e_13:
        LDR.W    R0,??DataTable21_31
        B.N      ??getEngine_type_e_36
//   81 case MITSU_4G93:
//   82   return "MITSU_4G93";
??getEngine_type_e_16:
        LDR.W    R0,??DataTable21_32
        B.N      ??getEngine_type_e_36
//   83 case NISSAN_PRIMERA:
//   84   return "NISSAN_PRIMERA";
??getEngine_type_e_5:
        LDR.W    R0,??DataTable21_33
        B.N      ??getEngine_type_e_36
//   85 case ROVER_V8:
//   86   return "ROVER_V8";
??getEngine_type_e_10:
        LDR.W    R0,??DataTable21_34
        B.N      ??getEngine_type_e_36
//   87 case SATURN_ION_2004:
//   88   return "SATURN_ION_2004";
??getEngine_type_e_12:
        LDR.W    R0,??DataTable21_35
        B.N      ??getEngine_type_e_36
//   89 case SUBARU_2003_WRX:
//   90   return "SUBARU_2003_WRX";
??getEngine_type_e_22:
        LDR.W    R0,??DataTable21_36
        B.N      ??getEngine_type_e_36
//   91 case TEST_ENGINE:
//   92   return "TEST_ENGINE";
??getEngine_type_e_26:
        LDR.W    R0,??DataTable21_37
        B.N      ??getEngine_type_e_36
//   93 case VW_ABA:
//   94   return "VW_ABA";
??getEngine_type_e_32:
        LDR.W    R0,??DataTable21_38
        B.N      ??getEngine_type_e_36
//   95   }
//   96  return NULL;
??getEngine_type_e_35:
        MOVS     R0,#+0
??getEngine_type_e_36:
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   97 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z17getTrigger_type_e14trigger_type_e
          CFI NoCalls
        THUMB
//   98 const char *getTrigger_type_e(trigger_type_e value){
_Z17getTrigger_type_e14trigger_type_e:
        MOVS     R1,R0
//   99 switch(value) {
        MOVS     R0,R1
        CMP      R0,#+0
        BEQ.N    ??getTrigger_type_e_0
        CMP      R0,#+1
        BEQ.N    ??getTrigger_type_e_1
        CMP      R0,#+2
        BEQ.N    ??getTrigger_type_e_2
        CMP      R0,#+3
        BEQ.N    ??getTrigger_type_e_3
        CMP      R0,#+4
        BEQ.N    ??getTrigger_type_e_4
        CMP      R0,#+5
        BEQ.N    ??getTrigger_type_e_5
        CMP      R0,#+6
        BEQ.N    ??getTrigger_type_e_6
        CMP      R0,#+7
        BEQ.N    ??getTrigger_type_e_7
        CMP      R0,#+8
        BEQ.N    ??getTrigger_type_e_8
        CMP      R0,#+9
        BEQ.N    ??getTrigger_type_e_9
        CMP      R0,#+10
        BEQ.N    ??getTrigger_type_e_10
        CMP      R0,#+11
        BEQ.N    ??getTrigger_type_e_11
        CMP      R0,#+12
        BEQ.N    ??getTrigger_type_e_12
        CMP      R0,#+13
        BEQ.N    ??getTrigger_type_e_13
        CMP      R0,#+14
        BEQ.N    ??getTrigger_type_e_14
        CMP      R0,#+15
        BEQ.N    ??getTrigger_type_e_15
        CMP      R0,#+16
        BEQ.N    ??getTrigger_type_e_16
        CMP      R0,#+17
        BEQ.N    ??getTrigger_type_e_17
        CMP      R0,#+18
        BEQ.N    ??getTrigger_type_e_18
        CMP      R0,#+19
        BEQ.N    ??getTrigger_type_e_19
        CMP      R0,#+20
        BEQ.N    ??getTrigger_type_e_20
        CMP      R0,#+21
        BEQ.N    ??getTrigger_type_e_21
        CMP      R0,#+22
        BEQ.N    ??getTrigger_type_e_22
        LDR.W    R2,??DataTable21  ;; 0x77359400
        CMP      R0,R2
        BNE.N    ??getTrigger_type_e_23
//  100 case Force_4b_trigger_type:
//  101   return "Force_4b_trigger_type";
??getTrigger_type_e_24:
        LDR.W    R0,??DataTable21_39
        B.N      ??getTrigger_type_e_25
//  102 case TT_DODGE_NEON_1995:
//  103   return "TT_DODGE_NEON_1995";
??getTrigger_type_e_2:
        LDR.W    R0,??DataTable21_40
        B.N      ??getTrigger_type_e_25
//  104 case TT_DODGE_NEON_2003:
//  105   return "TT_DODGE_NEON_2003";
??getTrigger_type_e_14:
        LDR.W    R0,??DataTable21_41
        B.N      ??getTrigger_type_e_25
//  106 case TT_FORD_ASPIRE:
//  107   return "TT_FORD_ASPIRE";
??getTrigger_type_e_1:
        LDR.W    R0,??DataTable21_42
        B.N      ??getTrigger_type_e_25
//  108 case TT_MAZDA_SOHC_4:
//  109   return "TT_MAZDA_SOHC_4";
??getTrigger_type_e_7:
        LDR.W    R0,??DataTable21_43
        B.N      ??getTrigger_type_e_25
//  110 case TT_MAZDA_DOHC_1_4:
//  111   return "TT_MAZDA_DOHC_1_4";
??getTrigger_type_e_15:
        LDR.W    R0,??DataTable21_44
        B.N      ??getTrigger_type_e_25
//  112 case TT_ONE_PLUS_ONE:
//  113   return "TT_ONE_PLUS_ONE";
??getTrigger_type_e_16:
        LDR.W    R0,??DataTable21_45
        B.N      ??getTrigger_type_e_25
//  114 case TT_GM_7X:
//  115   return "TT_GM_7X";
??getTrigger_type_e_5:
        LDR.W    R0,??DataTable21_46
        B.N      ??getTrigger_type_e_25
//  116 case TT_HONDA_ACCORD_CD:
//  117   return "TT_HONDA_ACCORD_CD";
??getTrigger_type_e_10:
        LDR.W    R0,??DataTable21_47
        B.N      ??getTrigger_type_e_25
//  118 case TT_HONDA_ACCORD_1_24:
//  119 	return "TT_HONDA_ACCORD_1_24";
??getTrigger_type_e_21:
        LDR.W    R0,??DataTable21_48
        B.N      ??getTrigger_type_e_25
//  120 case TT_HONDA_ACCORD_CD_DIP:
//  121   return "TT_HONDA_ACCORD_CD_DIP";
??getTrigger_type_e_13:
        LDR.W    R0,??DataTable21_49
        B.N      ??getTrigger_type_e_25
//  122 case TT_HONDA_ACCORD_CD_TWO_WIRES:
//  123   return "TT_HONDA_ACCORD_CD_TWO_WIRES";
??getTrigger_type_e_12:
        LDR.W    R0,??DataTable21_50
        B.N      ??getTrigger_type_e_25
//  124 case TT_MAZDA_MIATA_NA:
//  125   return "TT_MAZDA_MIATA_NA";
??getTrigger_type_e_3:
        LDR.W    R0,??DataTable21_51
        B.N      ??getTrigger_type_e_25
//  126 case TT_MAZDA_MIATA_NB:
//  127   return "TT_MAZDA_MIATA_NB";
??getTrigger_type_e_4:
        LDR.W    R0,??DataTable21_52
        B.N      ??getTrigger_type_e_25
//  128 case TT_MINI_COOPER_R50:
//  129   return "TT_MINI_COOPER_R50";
??getTrigger_type_e_6:
        LDR.W    R0,??DataTable21_53
        B.N      ??getTrigger_type_e_25
//  130 case TT_MITSU:
//  131   return "TT_MITSU";
??getTrigger_type_e_11:
        LDR.W    R0,??DataTable21_54
        B.N      ??getTrigger_type_e_25
//  132 case TT_TOOTHED_WHEEL:
//  133   return "TT_TOOTHED_WHEEL";
??getTrigger_type_e_0:
        LDR.W    R0,??DataTable21_55
        B.N      ??getTrigger_type_e_25
//  134 case TT_TOOTHED_WHEEL_36_1:
//  135   return "TT_TOOTHED_WHEEL_36_1";
??getTrigger_type_e_9:
        LDR.W    R0,??DataTable21_56
        B.N      ??getTrigger_type_e_25
//  136 case TT_TOOTHED_WHEEL_60_2:
//  137   return "TT_TOOTHED_WHEEL_60_2";
??getTrigger_type_e_8:
        LDR.W    R0,??DataTable21_57
        B.N      ??getTrigger_type_e_25
//  138 case TT_ONE_PLUS_TOOTHED_WHEEL_60_2:
//  139 	return "TT_ONE_PLUS_TOOTHED_WHEEL_60_2";
??getTrigger_type_e_17:
        LDR.W    R0,??DataTable21_58
        B.N      ??getTrigger_type_e_25
//  140 case TT_ONE:
//  141 	return "TT_ONE";
??getTrigger_type_e_18:
        LDR.W    R0,??DataTable21_59
        B.N      ??getTrigger_type_e_25
//  142 case TT_60_2_VW:
//  143 	return "TT_60_2_VW";
??getTrigger_type_e_20:
        LDR.W    R0,??DataTable21_60
        B.N      ??getTrigger_type_e_25
//  144 case TT_DODGE_RAM:
//  145 	return "TT_DODGE_RAM";
??getTrigger_type_e_19:
        LDR.W    R0,??DataTable21_61
        B.N      ??getTrigger_type_e_25
//  146 case TT_UNUSED:
//  147 	return "TT_UNUSED";
??getTrigger_type_e_22:
        LDR.W    R0,??DataTable22
        B.N      ??getTrigger_type_e_25
//  148   }
//  149  return NULL;
??getTrigger_type_e_23:
        MOVS     R0,#+0
??getTrigger_type_e_25:
        BX       LR               ;; return
          CFI EndBlock cfiBlock2
//  150 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z21getAdc_channel_mode_e18adc_channel_mode_e
          CFI NoCalls
        THUMB
//  151 const char *getAdc_channel_mode_e(adc_channel_mode_e value){
_Z21getAdc_channel_mode_e18adc_channel_mode_e:
        MOVS     R1,R0
//  152 switch(value) {
        MOVS     R0,R1
        CMP      R0,#+0
        BEQ.N    ??getAdc_channel_mode_e_0
        CMP      R0,#+1
        BEQ.N    ??getAdc_channel_mode_e_1
        CMP      R0,#+2
        BEQ.N    ??getAdc_channel_mode_e_2
        LDR.W    R2,??DataTable21  ;; 0x77359400
        CMP      R0,R2
        BEQ.N    ??getAdc_channel_mode_e_3
        B.N      ??getAdc_channel_mode_e_4
//  153 case ADC_FAST:
//  154   return "ADC_FAST";
??getAdc_channel_mode_e_2:
        LDR.W    R0,??DataTable21_62
        B.N      ??getAdc_channel_mode_e_5
//  155 case ADC_OFF:
//  156   return "ADC_OFF";
??getAdc_channel_mode_e_0:
        LDR.W    R0,??DataTable21_63
        B.N      ??getAdc_channel_mode_e_5
//  157 case ADC_SLOW:
//  158   return "ADC_SLOW";
??getAdc_channel_mode_e_1:
        LDR.W    R0,??DataTable21_64
        B.N      ??getAdc_channel_mode_e_5
//  159 case Force_4b_adc_channel_mode:
//  160   return "Force_4b_adc_channel_mode";
??getAdc_channel_mode_e_3:
        LDR.W    R0,??DataTable22_1
        B.N      ??getAdc_channel_mode_e_5
//  161   }
//  162  return NULL;
??getAdc_channel_mode_e_4:
        MOVS     R0,#+0
??getAdc_channel_mode_e_5:
        BX       LR               ;; return
          CFI EndBlock cfiBlock3
//  163 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z18getTrigger_value_e15trigger_value_e
          CFI NoCalls
        THUMB
//  164 const char *getTrigger_value_e(trigger_value_e value){
_Z18getTrigger_value_e15trigger_value_e:
        MOVS     R1,R0
//  165 switch(value) {
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+0
        BEQ.N    ??getTrigger_value_e_0
        CMP      R1,#+1
        BNE.N    ??getTrigger_value_e_1
//  166 case TV_HIGH:
//  167   return "TV_HIGH";
??getTrigger_value_e_2:
        LDR.W    R0,??DataTable21_65
        B.N      ??getTrigger_value_e_3
//  168 case TV_LOW:
//  169   return "TV_LOW";
??getTrigger_value_e_0:
        LDR.W    R0,??DataTable22_2
        B.N      ??getTrigger_value_e_3
//  170   }
//  171  return NULL;
??getTrigger_value_e_1:
        MOVS     R0,#+0
??getTrigger_value_e_3:
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//  172 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z18getTrigger_wheel_e15trigger_wheel_e
          CFI NoCalls
        THUMB
//  173 const char *getTrigger_wheel_e(trigger_wheel_e value){
_Z18getTrigger_wheel_e15trigger_wheel_e:
        MOVS     R1,R0
//  174 switch(value) {
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+0
        BEQ.N    ??getTrigger_wheel_e_0
        CMP      R1,#+2
        BEQ.N    ??getTrigger_wheel_e_1
        BCC.N    ??getTrigger_wheel_e_2
        B.N      ??getTrigger_wheel_e_3
//  175 case T_CHANNEL_3:
//  176   return "T_CHANNEL_3";
??getTrigger_wheel_e_1:
        LDR.W    R0,??DataTable21_66
        B.N      ??getTrigger_wheel_e_4
//  177 case T_PRIMARY:
//  178   return "T_PRIMARY";
??getTrigger_wheel_e_0:
        LDR.W    R0,??DataTable21_67
        B.N      ??getTrigger_wheel_e_4
//  179 case T_SECONDARY:
//  180   return "T_SECONDARY";
??getTrigger_wheel_e_2:
        LDR.W    R0,??DataTable22_3
        B.N      ??getTrigger_wheel_e_4
//  181   }
//  182  return NULL;
??getTrigger_wheel_e_3:
        MOVS     R0,#+0
??getTrigger_wheel_e_4:
        BX       LR               ;; return
          CFI EndBlock cfiBlock5
//  183 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z18getTrigger_event_e15trigger_event_e
          CFI NoCalls
        THUMB
//  184 const char *getTrigger_event_e(trigger_event_e value){
_Z18getTrigger_event_e15trigger_event_e:
        MOVS     R1,R0
//  185 switch(value) {
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+0
        BEQ.N    ??getTrigger_event_e_0
        CMP      R1,#+2
        BEQ.N    ??getTrigger_event_e_1
        BCC.N    ??getTrigger_event_e_2
        CMP      R1,#+4
        BEQ.N    ??getTrigger_event_e_3
        BCC.N    ??getTrigger_event_e_4
        CMP      R1,#+5
        BEQ.N    ??getTrigger_event_e_5
        B.N      ??getTrigger_event_e_6
//  186 case SHAFT_3RD_DOWN:
//  187   return "SHAFT_3RD_DOWN";
??getTrigger_event_e_3:
        LDR.W    R0,??DataTable21_68
        B.N      ??getTrigger_event_e_7
//  188 case SHAFT_3RD_UP:
//  189   return "SHAFT_3RD_UP";
??getTrigger_event_e_5:
        LDR.W    R0,??DataTable21_69
        B.N      ??getTrigger_event_e_7
//  190 case SHAFT_PRIMARY_DOWN:
//  191   return "SHAFT_PRIMARY_DOWN";
??getTrigger_event_e_0:
        LDR.W    R0,??DataTable21_70
        B.N      ??getTrigger_event_e_7
//  192 case SHAFT_PRIMARY_UP:
//  193   return "SHAFT_PRIMARY_UP";
??getTrigger_event_e_2:
        LDR.W    R0,??DataTable21_71
        B.N      ??getTrigger_event_e_7
//  194 case SHAFT_SECONDARY_DOWN:
//  195   return "SHAFT_SECONDARY_DOWN";
??getTrigger_event_e_1:
        LDR.W    R0,??DataTable21_72
        B.N      ??getTrigger_event_e_7
//  196 case SHAFT_SECONDARY_UP:
//  197   return "SHAFT_SECONDARY_UP";
??getTrigger_event_e_4:
        LDR.W    R0,??DataTable22_4
        B.N      ??getTrigger_event_e_7
//  198   }
//  199  return NULL;
??getTrigger_event_e_6:
        MOVS     R0,#+0
??getTrigger_event_e_7:
        BX       LR               ;; return
          CFI EndBlock cfiBlock6
//  200 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z21getEngine_load_mode_e18engine_load_mode_e
          CFI NoCalls
        THUMB
//  201 const char *getEngine_load_mode_e(engine_load_mode_e value){
_Z21getEngine_load_mode_e18engine_load_mode_e:
        MOVS     R1,R0
//  202 switch(value) {
        MOVS     R0,R1
        CMP      R0,#+0
        BEQ.N    ??getEngine_load_mode_e_0
        CMP      R0,#+1
        BEQ.N    ??getEngine_load_mode_e_1
        CMP      R0,#+2
        BEQ.N    ??getEngine_load_mode_e_2
        CMP      R0,#+3
        BEQ.N    ??getEngine_load_mode_e_3
        CMP      R0,#+4
        BEQ.N    ??getEngine_load_mode_e_4
        LDR.W    R2,??DataTable21  ;; 0x77359400
        CMP      R0,R2
        BNE.N    ??getEngine_load_mode_e_5
//  203 case Force_4b_engine_load_mode:
//  204   return "Force_4b_engine_load_mode";
??getEngine_load_mode_e_6:
        LDR.W    R0,??DataTable22_5
        B.N      ??getEngine_load_mode_e_7
//  205 case LM_ALPHA_N:
//  206   return "LM_ALPHA_N";
??getEngine_load_mode_e_1:
        LDR.W    R0,??DataTable22_6
        B.N      ??getEngine_load_mode_e_7
//  207 case LM_PLAIN_MAF:
//  208   return "LM_PLAIN_MAF";
??getEngine_load_mode_e_0:
        LDR.W    R0,??DataTable22_7
        B.N      ??getEngine_load_mode_e_7
//  209 case LM_REAL_MAF:
//  210   return "LM_REAL_MAF";
??getEngine_load_mode_e_4:
        LDR.W    R0,??DataTable22_8
        B.N      ??getEngine_load_mode_e_7
//  211 case LM_MAP:
//  212   return "LM_MAP";
??getEngine_load_mode_e_2:
        LDR.W    R0,??DataTable22_9
        B.N      ??getEngine_load_mode_e_7
//  213 case LM_SPEED_DENSITY:
//  214   return "LM_SPEED_DENSITY";
??getEngine_load_mode_e_3:
        LDR.W    R0,??DataTable23
        B.N      ??getEngine_load_mode_e_7
//  215   }
//  216  return NULL;
??getEngine_load_mode_e_5:
        MOVS     R0,#+0
??getEngine_load_mode_e_7:
        BX       LR               ;; return
          CFI EndBlock cfiBlock7
//  217 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z17getDisplay_mode_e14display_mode_e
          CFI NoCalls
        THUMB
//  218 const char *getDisplay_mode_e(display_mode_e value){
_Z17getDisplay_mode_e14display_mode_e:
        MOVS     R1,R0
//  219 switch(value) {
        MOVS     R0,R1
        CMP      R0,#+0
        BEQ.N    ??getDisplay_mode_e_0
        CMP      R0,#+1
        BEQ.N    ??getDisplay_mode_e_1
        CMP      R0,#+2
        BEQ.N    ??getDisplay_mode_e_2
        LDR.W    R2,??DataTable21  ;; 0x77359400
        CMP      R0,R2
        BEQ.N    ??getDisplay_mode_e_3
        B.N      ??getDisplay_mode_e_4
//  220 case DM_HD44780:
//  221   return "DM_HD44780";
??getDisplay_mode_e_1:
        LDR.W    R0,??DataTable22_10
        B.N      ??getDisplay_mode_e_5
//  222 case DM_HD44780_OVER_PCF8574:
//  223   return "DM_HD44780_OVER_PCF8574";
??getDisplay_mode_e_2:
        LDR.W    R0,??DataTable22_11
        B.N      ??getDisplay_mode_e_5
//  224 case DM_NONE:
//  225   return "DM_NONE";
??getDisplay_mode_e_0:
        LDR.W    R0,??DataTable23_1
        B.N      ??getDisplay_mode_e_5
//  226 case Force_4b_display_mode:
//  227   return "Force_4b_display_mode";
??getDisplay_mode_e_3:
        LDR.W    R0,??DataTable23_2
        B.N      ??getDisplay_mode_e_5
//  228   }
//  229  return NULL;
??getDisplay_mode_e_4:
        MOVS     R0,#+0
??getDisplay_mode_e_5:
        BX       LR               ;; return
          CFI EndBlock cfiBlock8
//  230 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z15getLog_format_e12log_format_e
          CFI NoCalls
        THUMB
//  231 const char *getLog_format_e(log_format_e value){
_Z15getLog_format_e12log_format_e:
        MOVS     R1,R0
//  232 switch(value) {
        MOVS     R0,R1
        CMP      R0,#+0
        BEQ.N    ??getLog_format_e_0
        CMP      R0,#+1
        BEQ.N    ??getLog_format_e_1
        LDR.W    R2,??DataTable21  ;; 0x77359400
        CMP      R0,R2
        BNE.N    ??getLog_format_e_2
//  233 case Force_4b_log_format:
//  234   return "Force_4b_log_format";
??getLog_format_e_3:
        LDR.W    R0,??DataTable23_3
        B.N      ??getLog_format_e_4
//  235 case LF_NATIVE:
//  236   return "LF_NATIVE";
??getLog_format_e_0:
        LDR.W    R0,??DataTable23_4
        B.N      ??getLog_format_e_4
//  237 case LM_MLV:
//  238   return "LM_MLV";
??getLog_format_e_1:
        LDR.W    R0,??DataTable23_5
        B.N      ??getLog_format_e_4
//  239   }
//  240  return NULL;
??getLog_format_e_2:
        MOVS     R0,#+0
??getLog_format_e_4:
        BX       LR               ;; return
          CFI EndBlock cfiBlock9
//  241 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z14getIdle_mode_e11idle_mode_e
          CFI NoCalls
        THUMB
//  242 const char *getIdle_mode_e(idle_mode_e value){
_Z14getIdle_mode_e11idle_mode_e:
        MOVS     R1,R0
//  243 switch(value) {
        MOVS     R0,R1
        CMP      R0,#+0
        BEQ.N    ??getIdle_mode_e_0
        CMP      R0,#+1
        BEQ.N    ??getIdle_mode_e_1
        LDR.W    R2,??DataTable21  ;; 0x77359400
        CMP      R0,R2
        BNE.N    ??getIdle_mode_e_2
//  244 case Force_4b_idle_mode:
//  245   return "Force_4b_idle_mode";
??getIdle_mode_e_3:
        LDR.W    R0,??DataTable23_6
        B.N      ??getIdle_mode_e_4
//  246 case IM_AUTO:
//  247   return "IM_AUTO";
??getIdle_mode_e_0:
        LDR.W    R0,??DataTable23_7
        B.N      ??getIdle_mode_e_4
//  248 case IM_MANUAL:
//  249   return "IM_MANUAL";
??getIdle_mode_e_1:
        LDR.W    R0,??DataTable23_8
        B.N      ??getIdle_mode_e_4
//  250   }
//  251  return NULL;
??getIdle_mode_e_2:
        MOVS     R0,#+0
??getIdle_mode_e_4:
        BX       LR               ;; return
          CFI EndBlock cfiBlock10
//  252 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z20getPin_output_mode_e17pin_output_mode_e
          CFI NoCalls
        THUMB
//  253 const char *getPin_output_mode_e(pin_output_mode_e value){
_Z20getPin_output_mode_e17pin_output_mode_e:
        MOVS     R1,R0
//  254 switch(value) {
        MOVS     R0,R1
        CMP      R0,#+0
        BEQ.N    ??getPin_output_mode_e_0
        CMP      R0,#+1
        BEQ.N    ??getPin_output_mode_e_1
        CMP      R0,#+2
        BEQ.N    ??getPin_output_mode_e_2
        CMP      R0,#+3
        BEQ.N    ??getPin_output_mode_e_3
        LDR.W    R2,??DataTable21  ;; 0x77359400
        CMP      R0,R2
        BNE.N    ??getPin_output_mode_e_4
//  255 case Force_4b_pin_output_mode:
//  256   return "Force_4b_pin_output_mode";
??getPin_output_mode_e_5:
        LDR.W    R0,??DataTable23_9
        B.N      ??getPin_output_mode_e_6
//  257 case OM_DEFAULT:
//  258   return "OM_DEFAULT";
??getPin_output_mode_e_0:
        LDR.W    R0,??DataTable23_10
        B.N      ??getPin_output_mode_e_6
//  259 case OM_INVERTED:
//  260   return "OM_INVERTED";
??getPin_output_mode_e_1:
        LDR.W    R0,??DataTable23_11
        B.N      ??getPin_output_mode_e_6
//  261 case OM_OPENDRAIN:
//  262   return "OM_OPENDRAIN";
??getPin_output_mode_e_2:
        LDR.W    R0,??DataTable23_12
        B.N      ??getPin_output_mode_e_6
//  263 case OM_OPENDRAIN_INVERTED:
//  264   return "OM_OPENDRAIN_INVERTED";
??getPin_output_mode_e_3:
        LDR.W    R0,??DataTable23_13
        B.N      ??getPin_output_mode_e_6
//  265   }
//  266  return NULL;
??getPin_output_mode_e_4:
        MOVS     R0,#+0
??getPin_output_mode_e_6:
        BX       LR               ;; return
          CFI EndBlock cfiBlock11
//  267 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _Z14getGpio_mode_e11gpio_mode_e
          CFI NoCalls
        THUMB
//  268 const char *getGpio_mode_e(gpio_mode_e value){
_Z14getGpio_mode_e11gpio_mode_e:
        MOVS     R1,R0
//  269 switch(value) {
        LDR.W    R0,??DataTable21  ;; 0x77359400
        CMP      R1,R0
        BNE.N    ??getGpio_mode_e_0
//  270 case Force_4b_gpio_mode:
//  271   return "Force_4b_gpio_mode";
        LDR.W    R0,??DataTable23_14
        B.N      ??getGpio_mode_e_1
//  272   }
//  273  return NULL;
??getGpio_mode_e_0:
        MOVS     R0,#+0
??getGpio_mode_e_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock12
//  274 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z19getPin_input_mode_e16pin_input_mode_e
          CFI NoCalls
        THUMB
//  275 const char *getPin_input_mode_e(pin_input_mode_e value){
_Z19getPin_input_mode_e16pin_input_mode_e:
        MOVS     R1,R0
//  276 switch(value) {
        MOVS     R0,R1
        CMP      R0,#+0
        BEQ.N    ??getPin_input_mode_e_0
        CMP      R0,#+1
        BEQ.N    ??getPin_input_mode_e_1
        CMP      R0,#+2
        BEQ.N    ??getPin_input_mode_e_2
        LDR.N    R2,??DataTable21  ;; 0x77359400
        CMP      R0,R2
        BNE.N    ??getPin_input_mode_e_3
//  277 case Force_4b_pin_input_mode:
//  278   return "Force_4b_pin_input_mode";
??getPin_input_mode_e_4:
        LDR.W    R0,??DataTable23_15
        B.N      ??getPin_input_mode_e_5
//  279 case PI_DEFAULT:
//  280   return "PI_DEFAULT";
??getPin_input_mode_e_0:
        LDR.W    R0,??DataTable23_16
        B.N      ??getPin_input_mode_e_5
//  281 case PI_PULLDOWN:
//  282   return "PI_PULLDOWN";
??getPin_input_mode_e_2:
        LDR.W    R0,??DataTable23_17
        B.N      ??getPin_input_mode_e_5
//  283 case PI_PULLUP:
//  284   return "PI_PULLUP";
??getPin_input_mode_e_1:
        LDR.W    R0,??DataTable23_18
        B.N      ??getPin_input_mode_e_5
//  285   }
//  286  return NULL;
??getPin_input_mode_e_3:
        MOVS     R0,#+0
??getPin_input_mode_e_5:
        BX       LR               ;; return
          CFI EndBlock cfiBlock13
//  287 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _Z17getFiring_order_e14firing_order_e
          CFI NoCalls
        THUMB
//  288 const char *getFiring_order_e(firing_order_e value){
_Z17getFiring_order_e14firing_order_e:
        MOVS     R1,R0
//  289 switch(value) {
        MOVS     R0,R1
        CMP      R0,#+0
        BEQ.N    ??getFiring_order_e_0
        CMP      R0,#+1
        BEQ.N    ??getFiring_order_e_1
        CMP      R0,#+2
        BEQ.N    ??getFiring_order_e_2
        CMP      R0,#+3
        BEQ.N    ??getFiring_order_e_3
        CMP      R0,#+4
        BEQ.N    ??getFiring_order_e_4
        CMP      R0,#+5
        BEQ.N    ??getFiring_order_e_5
        CMP      R0,#+6
        BEQ.N    ??getFiring_order_e_6
        CMP      R0,#+7
        BEQ.N    ??getFiring_order_e_7
        CMP      R0,#+8
        BEQ.N    ??getFiring_order_e_8
        LDR.N    R2,??DataTable21  ;; 0x77359400
        CMP      R0,R2
        BEQ.N    ??getFiring_order_e_9
        B.N      ??getFiring_order_e_10
//  290 case FO_1_2_4_5_3:
//  291   return "FO_1_2_4_5_3";
??getFiring_order_e_6:
        LDR.W    R0,??DataTable23_19
        B.N      ??getFiring_order_e_11
//  292 case FO_1_THEN_2:
//  293   return "FO_1_THEN_2";
??getFiring_order_e_8:
        LDR.W    R0,??DataTable23_20
        B.N      ??getFiring_order_e_11
//  294 case FO_1_8_4_3_6_5_7_2:
//  295   return "FO_1_8_4_3_6_5_7_2";
??getFiring_order_e_5:
        LDR.W    R0,??DataTable23_21
        B.N      ??getFiring_order_e_11
//  296 case FO_1_THEN_2_THEN_4_THEN3:
//  297   return "FO_1_THEN_2_THEN_4_THEN3";
??getFiring_order_e_2:
        LDR.W    R0,??DataTable23_22
        B.N      ??getFiring_order_e_11
//  298 case FO_1_THEN_3_THEN_2_THEN4:
//  299   return "FO_1_THEN_3_THEN_2_THEN4";
??getFiring_order_e_3:
        LDR.W    R0,??DataTable23_23
        B.N      ??getFiring_order_e_11
//  300 case FO_1_THEN_3_THEN_4_THEN2:
//  301   return "FO_1_THEN_3_THEN_4_THEN2";
??getFiring_order_e_1:
        LDR.W    R0,??DataTable23_24
        B.N      ??getFiring_order_e_11
//  302 case FO_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4:
//  303   return "FO_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4";
??getFiring_order_e_4:
        LDR.W    R0,??DataTable23_25
        B.N      ??getFiring_order_e_11
//  304 case FO_1_THEN_4_THEN_2_THEN_5_THEN_3_THEN_6:
//  305   return "FO_1_THEN_4_THEN_2_THEN_5_THEN_3_THEN_6";
??getFiring_order_e_7:
        LDR.W    R0,??DataTable23_26
        B.N      ??getFiring_order_e_11
//  306 case FO_ONE_CYLINDER:
//  307   return "FO_ONE_CYLINDER";
??getFiring_order_e_0:
        LDR.W    R0,??DataTable23_27
        B.N      ??getFiring_order_e_11
//  308 case Force_4b_firing_order:
//  309   return "Force_4b_firing_order";
??getFiring_order_e_9:
        LDR.W    R0,??DataTable23_28
        B.N      ??getFiring_order_e_11
//  310   }
//  311  return NULL;
??getFiring_order_e_10:
        MOVS     R0,#+0
??getFiring_order_e_11:
        BX       LR               ;; return
          CFI EndBlock cfiBlock14
//  312 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function _Z19getOperation_mode_e16operation_mode_e
          CFI NoCalls
        THUMB
//  313 const char *getOperation_mode_e(operation_mode_e value){
_Z19getOperation_mode_e16operation_mode_e:
        MOVS     R1,R0
//  314 switch(value) {
        MOVS     R0,R1
        CMP      R0,#+0
        BEQ.N    ??getOperation_mode_e_0
        CMP      R0,#+1
        BEQ.N    ??getOperation_mode_e_1
        CMP      R0,#+2
        BEQ.N    ??getOperation_mode_e_2
        CMP      R0,#+3
        BEQ.N    ??getOperation_mode_e_3
        LDR.N    R2,??DataTable21  ;; 0x77359400
        CMP      R0,R2
        BEQ.N    ??getOperation_mode_e_4
        B.N      ??getOperation_mode_e_5
//  315 case FOUR_STROKE_CAM_SENSOR:
//  316   return "FOUR_STROKE_CAM_SENSOR";
??getOperation_mode_e_2:
        LDR.W    R0,??DataTable23_29
        B.N      ??getOperation_mode_e_6
//  317 case FOUR_STROKE_CRANK_SENSOR:
//  318   return "FOUR_STROKE_CRANK_SENSOR";
??getOperation_mode_e_1:
        LDR.W    R0,??DataTable23_30
        B.N      ??getOperation_mode_e_6
//  319 case TWO_STROKE:
//  320   return "TWO_STROKE";
??getOperation_mode_e_3:
        LDR.W    R0,??DataTable23_31
        B.N      ??getOperation_mode_e_6
//  321 case Force_4b_operation_mode_e:
//  322   return "Force_4b_operation_mode_e";
??getOperation_mode_e_4:
        LDR.W    R0,??DataTable23_32
        B.N      ??getOperation_mode_e_6
//  323 case OM_NONE:
//  324   return "OM_NONE";
??getOperation_mode_e_0:
        LDR.W    R0,??DataTable23_33
        B.N      ??getOperation_mode_e_6
//  325   }
//  326  return NULL;
??getOperation_mode_e_5:
        MOVS     R0,#+0
??getOperation_mode_e_6:
        BX       LR               ;; return
          CFI EndBlock cfiBlock15
//  327 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function _Z18getIgnition_mode_e15ignition_mode_e
          CFI NoCalls
        THUMB
//  328 const char *getIgnition_mode_e(ignition_mode_e value){
_Z18getIgnition_mode_e15ignition_mode_e:
        MOVS     R1,R0
//  329 switch(value) {
        MOVS     R0,R1
        CMP      R0,#+0
        BEQ.N    ??getIgnition_mode_e_0
        CMP      R0,#+1
        BEQ.N    ??getIgnition_mode_e_1
        CMP      R0,#+2
        BEQ.N    ??getIgnition_mode_e_2
        LDR.N    R2,??DataTable21  ;; 0x77359400
        CMP      R0,R2
        BNE.N    ??getIgnition_mode_e_3
//  330 case Force_4b_ignition_mode:
//  331   return "Force_4b_ignition_mode";
??getIgnition_mode_e_4:
        LDR.W    R0,??DataTable23_34
        B.N      ??getIgnition_mode_e_5
//  332 case IM_INDIVIDUAL_COILS:
//  333   return "IM_INDIVIDUAL_COILS";
??getIgnition_mode_e_1:
        LDR.W    R0,??DataTable23_35
        B.N      ??getIgnition_mode_e_5
//  334 case IM_ONE_COIL:
//  335   return "IM_ONE_COIL";
??getIgnition_mode_e_0:
        LDR.W    R0,??DataTable23_36
        B.N      ??getIgnition_mode_e_5
//  336 case IM_WASTED_SPARK:
//  337   return "IM_WASTED_SPARK";
??getIgnition_mode_e_2:
        LDR.W    R0,??DataTable23_37
        B.N      ??getIgnition_mode_e_5
//  338   }
//  339  return NULL;
??getIgnition_mode_e_3:
        MOVS     R0,#+0
??getIgnition_mode_e_5:
        BX       LR               ;; return
          CFI EndBlock cfiBlock16
//  340 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function _Z19getInjection_mode_e16injection_mode_e
          CFI NoCalls
        THUMB
//  341 const char *getInjection_mode_e(injection_mode_e value){
_Z19getInjection_mode_e16injection_mode_e:
        MOVS     R1,R0
//  342 switch(value) {
        MOVS     R0,R1
        CMP      R0,#+0
        BEQ.N    ??getInjection_mode_e_0
        CMP      R0,#+1
        BEQ.N    ??getInjection_mode_e_1
        CMP      R0,#+2
        BEQ.N    ??getInjection_mode_e_2
        LDR.N    R2,??DataTable21  ;; 0x77359400
        CMP      R0,R2
        BNE.N    ??getInjection_mode_e_3
//  343 case Force_4b_injection_mode:
//  344   return "Force_4b_injection_mode";
??getInjection_mode_e_4:
        LDR.W    R0,??DataTable23_38
        B.N      ??getInjection_mode_e_5
//  345 case IM_BATCH:
//  346   return "IM_BATCH";
??getInjection_mode_e_2:
        LDR.W    R0,??DataTable23_39
        B.N      ??getInjection_mode_e_5
//  347 case IM_SEQUENTIAL:
//  348   return "IM_SEQUENTIAL";
??getInjection_mode_e_1:
        LDR.W    R0,??DataTable23_40
        B.N      ??getInjection_mode_e_5
//  349 case IM_SIMULTANEOUS:
//  350   return "IM_SIMULTANEOUS";
??getInjection_mode_e_0:
        LDR.W    R0,??DataTable23_41
        B.N      ??getInjection_mode_e_5
//  351   }
//  352  return NULL;
??getInjection_mode_e_3:
        MOVS     R0,#+0
??getInjection_mode_e_5:
        BX       LR               ;; return
          CFI EndBlock cfiBlock17
//  353 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function _Z27getCranking_ignition_mode_e24cranking_ignition_mode_e
          CFI NoCalls
        THUMB
//  354 const char *getCranking_ignition_mode_e(cranking_ignition_mode_e value){
_Z27getCranking_ignition_mode_e24cranking_ignition_mode_e:
        MOVS     R1,R0
//  355 switch(value) {
        MOVS     R0,R1
        CMP      R0,#+0
        BEQ.N    ??getCranking_ignition_mode_e_0
        CMP      R0,#+1
        BEQ.N    ??getCranking_ignition_mode_e_1
        LDR.N    R2,??DataTable21  ;; 0x77359400
        CMP      R0,R2
        BEQ.N    ??getCranking_ignition_mode_e_2
        B.N      ??getCranking_ignition_mode_e_3
//  356 case CIM_DEFAULT:
//  357   return "CIM_DEFAULT";
??getCranking_ignition_mode_e_0:
        LDR.W    R0,??DataTable23_42
        B.N      ??getCranking_ignition_mode_e_4
//  358 case CIM_FIXED_ANGLE:
//  359   return "CIM_FIXED_ANGLE";
??getCranking_ignition_mode_e_1:
        LDR.W    R0,??DataTable23_43
        B.N      ??getCranking_ignition_mode_e_4
//  360 case Force_4b_cranking_ignition_mode:
//  361   return "Force_4b_cranking_ignition_mode";
??getCranking_ignition_mode_e_2:
        LDR.W    R0,??DataTable23_44
        B.N      ??getCranking_ignition_mode_e_4
//  362   }
//  363  return NULL;
??getCranking_ignition_mode_e_3:
        MOVS     R0,#+0
??getCranking_ignition_mode_e_4:
        BX       LR               ;; return
          CFI EndBlock cfiBlock18
//  364 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock19 Using cfiCommon0
          CFI Function _Z15getSpi_device_e12spi_device_e
          CFI NoCalls
        THUMB
//  365 const char *getSpi_device_e(spi_device_e value){
_Z15getSpi_device_e12spi_device_e:
        MOVS     R1,R0
//  366 switch(value) {
        MOVS     R0,R1
        CMP      R0,#+0
        BEQ.N    ??getSpi_device_e_0
        CMP      R0,#+1
        BEQ.N    ??getSpi_device_e_1
        CMP      R0,#+2
        BEQ.N    ??getSpi_device_e_2
        CMP      R0,#+3
        BEQ.N    ??getSpi_device_e_3
        CMP      R0,#+4
        BEQ.N    ??getSpi_device_e_4
        LDR.N    R2,??DataTable21  ;; 0x77359400
        CMP      R0,R2
        BNE.N    ??getSpi_device_e_5
//  367 case Force_4b_spi_device:
//  368   return "Force_4b_spi_device";
??getSpi_device_e_6:
        LDR.W    R0,??DataTable23_45
        B.N      ??getSpi_device_e_7
//  369 case SPI_DEVICE_1:
//  370   return "SPI_DEVICE_1";
??getSpi_device_e_1:
        LDR.W    R0,??DataTable23_46
        B.N      ??getSpi_device_e_7
//  371 case SPI_DEVICE_2:
//  372   return "SPI_DEVICE_2";
??getSpi_device_e_2:
        LDR.W    R0,??DataTable23_47
        B.N      ??getSpi_device_e_7
//  373 case SPI_DEVICE_3:
//  374   return "SPI_DEVICE_3";
??getSpi_device_e_3:
        LDR.W    R0,??DataTable23_48
        B.N      ??getSpi_device_e_7
//  375 case SPI_DEVICE_4:
//  376   return "SPI_DEVICE_4";
??getSpi_device_e_4:
        LDR.W    R0,??DataTable23_49
        B.N      ??getSpi_device_e_7
//  377 case SPI_NONE:
//  378   return "SPI_NONE";
??getSpi_device_e_0:
        LDR.W    R0,??DataTable23_50
        B.N      ??getSpi_device_e_7
//  379   }
//  380  return NULL;
??getSpi_device_e_5:
        MOVS     R0,#+0
??getSpi_device_e_7:
        BX       LR               ;; return
          CFI EndBlock cfiBlock19
//  381 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock20 Using cfiCommon0
          CFI Function _Z16getAdc_channel_e13adc_channel_e
          CFI NoCalls
        THUMB
//  382 const char *getAdc_channel_e(adc_channel_e value){
_Z16getAdc_channel_e13adc_channel_e:
        MOVS     R1,R0
//  383 switch(value) {
        MOVS     R0,R1
        CMP      R0,#+0
        BEQ.N    ??getAdc_channel_e_0
        CMP      R0,#+1
        BEQ.N    ??getAdc_channel_e_1
        CMP      R0,#+2
        BEQ.N    ??getAdc_channel_e_2
        CMP      R0,#+3
        BEQ.N    ??getAdc_channel_e_3
        CMP      R0,#+4
        BEQ.N    ??getAdc_channel_e_4
        CMP      R0,#+5
        BEQ.N    ??getAdc_channel_e_5
        CMP      R0,#+6
        BEQ.N    ??getAdc_channel_e_6
        CMP      R0,#+7
        BEQ.N    ??getAdc_channel_e_7
        CMP      R0,#+8
        BEQ.N    ??getAdc_channel_e_8
        CMP      R0,#+9
        BEQ.N    ??getAdc_channel_e_9
        CMP      R0,#+10
        BEQ.N    ??getAdc_channel_e_10
        CMP      R0,#+11
        BEQ.N    ??getAdc_channel_e_11
        CMP      R0,#+12
        BEQ.N    ??getAdc_channel_e_12
        CMP      R0,#+13
        BEQ.N    ??getAdc_channel_e_13
        CMP      R0,#+14
        BEQ.N    ??getAdc_channel_e_14
        CMP      R0,#+15
        BEQ.N    ??getAdc_channel_e_15
        CMP      R0,#+16
        BEQ.N    ??getAdc_channel_e_16
        MOVW     R2,#+999
        CMP      R0,R2
        BEQ.N    ??getAdc_channel_e_17
        LDR.N    R2,??DataTable21  ;; 0x77359400
        CMP      R0,R2
        BEQ.N    ??getAdc_channel_e_18
        B.N      ??getAdc_channel_e_19
//  384 case EFI_ADC_0:
//  385   return "EFI_ADC_0";
??getAdc_channel_e_0:
        LDR.W    R0,??DataTable23_51
        B.N      ??getAdc_channel_e_20
//  386 case EFI_ADC_1:
//  387   return "EFI_ADC_1";
??getAdc_channel_e_1:
        LDR.W    R0,??DataTable23_52
        B.N      ??getAdc_channel_e_20
//  388 case EFI_ADC_10:
//  389   return "EFI_ADC_10";
??getAdc_channel_e_10:
        LDR.W    R0,??DataTable23_53
        B.N      ??getAdc_channel_e_20
//  390 case EFI_ADC_11:
//  391   return "EFI_ADC_11";
??getAdc_channel_e_11:
        LDR.W    R0,??DataTable23_54
        B.N      ??getAdc_channel_e_20
//  392 case EFI_ADC_12:
//  393   return "EFI_ADC_12";
??getAdc_channel_e_12:
        LDR.W    R0,??DataTable23_55
        B.N      ??getAdc_channel_e_20
//  394 case EFI_ADC_13:
//  395   return "EFI_ADC_13";
??getAdc_channel_e_13:
        LDR.W    R0,??DataTable23_56
        B.N      ??getAdc_channel_e_20
//  396 case EFI_ADC_14:
//  397   return "EFI_ADC_14";
??getAdc_channel_e_14:
        LDR.W    R0,??DataTable23_57
        B.N      ??getAdc_channel_e_20
//  398 case EFI_ADC_15:
//  399   return "EFI_ADC_15";
??getAdc_channel_e_15:
        LDR.W    R0,??DataTable23_58
        B.N      ??getAdc_channel_e_20
//  400 case EFI_ADC_2:
//  401   return "EFI_ADC_2";
??getAdc_channel_e_2:
        LDR.W    R0,??DataTable23_59
        B.N      ??getAdc_channel_e_20
//  402 case EFI_ADC_3:
//  403   return "EFI_ADC_3";
??getAdc_channel_e_3:
        LDR.W    R0,??DataTable23_60
        B.N      ??getAdc_channel_e_20
//  404 case EFI_ADC_4:
//  405   return "EFI_ADC_4";
??getAdc_channel_e_4:
        LDR.W    R0,??DataTable23_61
        B.N      ??getAdc_channel_e_20
//  406 case EFI_ADC_5:
//  407   return "EFI_ADC_5";
??getAdc_channel_e_5:
        LDR.W    R0,??DataTable23_62
        B.N      ??getAdc_channel_e_20
//  408 case EFI_ADC_6:
//  409   return "EFI_ADC_6";
??getAdc_channel_e_6:
        LDR.W    R0,??DataTable23_63
        B.N      ??getAdc_channel_e_20
//  410 case EFI_ADC_7:
//  411   return "EFI_ADC_7";
??getAdc_channel_e_7:
        LDR.W    R0,??DataTable23_64
        B.N      ??getAdc_channel_e_20
//  412 case EFI_ADC_8:
//  413   return "EFI_ADC_8";
??getAdc_channel_e_8:
        LDR.W    R0,??DataTable23_65
        B.N      ??getAdc_channel_e_20
//  414 case EFI_ADC_9:
//  415   return "EFI_ADC_9";
??getAdc_channel_e_9:
        LDR.W    R0,??DataTable23_66
        B.N      ??getAdc_channel_e_20
//  416 case EFI_ADC_ERROR:
//  417   return "EFI_ADC_ERROR";
??getAdc_channel_e_17:
        LDR.W    R0,??DataTable23_67
        B.N      ??getAdc_channel_e_20
//  418 case EFI_ADC_NONE:
//  419   return "EFI_ADC_NONE";
??getAdc_channel_e_16:
        LDR.W    R0,??DataTable23_68
        B.N      ??getAdc_channel_e_20
//  420 case Force_4b_cranking_adc_channel:
//  421   return "Force_4b_cranking_adc_channel";
??getAdc_channel_e_18:
        LDR.W    R0,??DataTable23_69
        B.N      ??getAdc_channel_e_20
//  422   }
//  423  return NULL;
??getAdc_channel_e_19:
        MOVS     R0,#+0
??getAdc_channel_e_20:
        BX       LR               ;; return
          CFI EndBlock cfiBlock20
//  424 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock21 Using cfiCommon0
          CFI Function _Z15getEgo_sensor_e12ego_sensor_e
          CFI NoCalls
        THUMB
//  425 const char *getEgo_sensor_e(ego_sensor_e value){
_Z15getEgo_sensor_e12ego_sensor_e:
        MOVS     R1,R0
//  426 switch(value) {
        MOVS     R0,R1
        CMP      R0,#+0
        BEQ.N    ??getEgo_sensor_e_0
        CMP      R0,#+1
        BEQ.N    ??getEgo_sensor_e_1
        CMP      R0,#+2
        BEQ.N    ??getEgo_sensor_e_2
        CMP      R0,#+3
        BEQ.N    ??getEgo_sensor_e_3
        CMP      R0,#+4
        BEQ.N    ??getEgo_sensor_e_4
        LDR.N    R2,??DataTable21  ;; 0x77359400
        CMP      R0,R2
        BEQ.N    ??getEgo_sensor_e_5
        B.N      ??getEgo_sensor_e_6
//  427 case ES_14Point7_Free:
//  428   return "ES_14Point7_Free";
??getEgo_sensor_e_2:
        LDR.W    R0,??DataTable23_70
        B.N      ??getEgo_sensor_e_7
//  429 case ES_BPSX_D1:
//  430   return "ES_BPSX_D1";
??getEgo_sensor_e_0:
        LDR.W    R0,??DataTable23_71
        B.N      ??getEgo_sensor_e_7
//  431 case ES_Innovate_MTX_L:
//  432   return "ES_Innovate_MTX_L";
??getEgo_sensor_e_1:
        LDR.W    R0,??DataTable23_72
        B.N      ??getEgo_sensor_e_7
//  433 case ES_NarrowBand:
//  434   return "ES_NarrowBand";
??getEgo_sensor_e_3:
        LDR.W    R0,??DataTable23_73
        B.N      ??getEgo_sensor_e_7
//  435 case ES_PLX:
//  436 	return "ES_PLX";
??getEgo_sensor_e_4:
        LDR.W    R0,??DataTable23_74
        B.N      ??getEgo_sensor_e_7
//  437 case Force_4b_ego_sensor:
//  438   return "Force_4b_ego_sensor";
??getEgo_sensor_e_5:
        LDR.W    R0,??DataTable23_75
        B.N      ??getEgo_sensor_e_7
//  439   }
//  440  return NULL;
??getEgo_sensor_e_6:
        MOVS     R0,#+0
??getEgo_sensor_e_7:
        BX       LR               ;; return
          CFI EndBlock cfiBlock21
//  441 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21:
        DC32     0x77359400

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_1:
        DC32     `?<Constant "Internal_ForceMyEnumI...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_2:
        DC32     `?<Constant "TM_DYNAMIC">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_3:
        DC32     `?<Constant "TM_FIXED">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_4:
        DC32     `?<Constant "ACURA_RSX">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_5:
        DC32     `?<Constant "AUDI_AAN">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_6:
        DC32     `?<Constant "SACHS">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_7:
        DC32     `?<Constant "GM_2_2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_8:
        DC32     `?<Constant "DODGE_RAM">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_9:
        DC32     `?<Constant "BMW_E34">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_10:
        DC32     `?<Constant "CITROEN_TU3JP">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_11:
        DC32     `?<Constant "CUSTOM_ENGINE">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_12:
        DC32     `?<Constant "DODGE_NEON_1995">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_13:
        DC32     `?<Constant "DODGE_NEON_2003">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_14:
        DC32     `?<Constant "ET_UNUSED">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_15:
        DC32     `?<Constant "FORD_ASPIRE_1996">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_16:
        DC32     `?<Constant "FORD_ESCORT_GT">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_17:
        DC32     `?<Constant "FORD_FIESTA">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_18:
        DC32     `?<Constant "FORD_INLINE_6_1995">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_19:
        DC32     `?<Constant "Force_4b_engine_type">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_20:
        DC32     `?<Constant "GY6_139QMB">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_21:
        DC32     `?<Constant "HONDA_ACCORD_CD">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_22:
        DC32     `?<Constant "HONDA_ACCORD_CD_DIP">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_23:
        DC32     `?<Constant "HONDA_ACCORD_CD_TWO_W...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_24:
        DC32     `?<Constant "MAZDA_323">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_25:
        DC32     `?<Constant "MAZDA_626">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_26:
        DC32     `?<Constant "MAZDA_MIATA_NB">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_27:
        DC32     `?<Constant "MIATA_1990">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_28:
        DC32     `?<Constant "MIATA_1994_DEVIATOR">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_29:
        DC32     `?<Constant "MIATA_1994_SPAGS">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_30:
        DC32     `?<Constant "MIATA_1996">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_31:
        DC32     `?<Constant "MINI_COOPER_R50">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_32:
        DC32     `?<Constant "MITSU_4G93">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_33:
        DC32     `?<Constant "NISSAN_PRIMERA">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_34:
        DC32     `?<Constant "ROVER_V8">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_35:
        DC32     `?<Constant "SATURN_ION_2004">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_36:
        DC32     `?<Constant "SUBARU_2003_WRX">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_37:
        DC32     `?<Constant "TEST_ENGINE">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_38:
        DC32     `?<Constant "VW_ABA">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_39:
        DC32     `?<Constant "Force_4b_trigger_type">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_40:
        DC32     `?<Constant "TT_DODGE_NEON_1995">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_41:
        DC32     `?<Constant "TT_DODGE_NEON_2003">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_42:
        DC32     `?<Constant "TT_FORD_ASPIRE">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_43:
        DC32     `?<Constant "TT_MAZDA_SOHC_4">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_44:
        DC32     `?<Constant "TT_MAZDA_DOHC_1_4">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_45:
        DC32     `?<Constant "TT_ONE_PLUS_ONE">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_46:
        DC32     `?<Constant "TT_GM_7X">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_47:
        DC32     `?<Constant "TT_HONDA_ACCORD_CD">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_48:
        DC32     `?<Constant "TT_HONDA_ACCORD_1_24">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_49:
        DC32     `?<Constant "TT_HONDA_ACCORD_CD_DIP">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_50:
        DC32     `?<Constant "TT_HONDA_ACCORD_CD_TW...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_51:
        DC32     `?<Constant "TT_MAZDA_MIATA_NA">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_52:
        DC32     `?<Constant "TT_MAZDA_MIATA_NB">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_53:
        DC32     `?<Constant "TT_MINI_COOPER_R50">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_54:
        DC32     `?<Constant "TT_MITSU">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_55:
        DC32     `?<Constant "TT_TOOTHED_WHEEL">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_56:
        DC32     `?<Constant "TT_TOOTHED_WHEEL_36_1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_57:
        DC32     `?<Constant "TT_TOOTHED_WHEEL_60_2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_58:
        DC32     `?<Constant "TT_ONE_PLUS_TOOTHED_W...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_59:
        DC32     `?<Constant "TT_ONE">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_60:
        DC32     `?<Constant "TT_60_2_VW">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_61:
        DC32     `?<Constant "TT_DODGE_RAM">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_62:
        DC32     `?<Constant "ADC_FAST">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_63:
        DC32     `?<Constant "ADC_OFF">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_64:
        DC32     `?<Constant "ADC_SLOW">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_65:
        DC32     `?<Constant "TV_HIGH">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_66:
        DC32     `?<Constant "T_CHANNEL_3">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_67:
        DC32     `?<Constant "T_PRIMARY">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_68:
        DC32     `?<Constant "SHAFT_3RD_DOWN">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_69:
        DC32     `?<Constant "SHAFT_3RD_UP">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_70:
        DC32     `?<Constant "SHAFT_PRIMARY_DOWN">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_71:
        DC32     `?<Constant "SHAFT_PRIMARY_UP">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable21_72:
        DC32     `?<Constant "SHAFT_SECONDARY_DOWN">`

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock22 Using cfiCommon0
          CFI Function _Z14getBrain_pin_e11brain_pin_e
          CFI NoCalls
        THUMB
//  442 const char *getBrain_pin_e(brain_pin_e value){
_Z14getBrain_pin_e11brain_pin_e:
        MOVS     R1,R0
//  443 switch(value) {
        MOVS     R0,R1
        CMP      R0,#+0
        BEQ.W    ??getBrain_pin_e_0
        CMP      R0,#+1
        BEQ.W    ??getBrain_pin_e_1
        CMP      R0,#+2
        BEQ.W    ??getBrain_pin_e_2
        CMP      R0,#+3
        BEQ.W    ??getBrain_pin_e_3
        CMP      R0,#+4
        BEQ.W    ??getBrain_pin_e_4
        CMP      R0,#+5
        BEQ.W    ??getBrain_pin_e_5
        CMP      R0,#+6
        BEQ.W    ??getBrain_pin_e_6
        CMP      R0,#+7
        BEQ.W    ??getBrain_pin_e_7
        CMP      R0,#+8
        BEQ.W    ??getBrain_pin_e_8
        CMP      R0,#+9
        BEQ.W    ??getBrain_pin_e_9
        CMP      R0,#+10
        BEQ.W    ??getBrain_pin_e_10
        CMP      R0,#+11
        BEQ.W    ??getBrain_pin_e_11
        CMP      R0,#+12
        BEQ.W    ??getBrain_pin_e_12
        CMP      R0,#+13
        BEQ.W    ??getBrain_pin_e_13
        CMP      R0,#+14
        BEQ.W    ??getBrain_pin_e_14
        CMP      R0,#+15
        BEQ.W    ??getBrain_pin_e_15
        CMP      R0,#+16
        BEQ.W    ??getBrain_pin_e_16
        CMP      R0,#+17
        BEQ.W    ??getBrain_pin_e_17
        CMP      R0,#+18
        BEQ.W    ??getBrain_pin_e_18
        CMP      R0,#+19
        BEQ.W    ??getBrain_pin_e_19
        CMP      R0,#+20
        BEQ.W    ??getBrain_pin_e_20
        CMP      R0,#+21
        BEQ.W    ??getBrain_pin_e_21
        CMP      R0,#+22
        BEQ.W    ??getBrain_pin_e_22
        CMP      R0,#+23
        BEQ.W    ??getBrain_pin_e_23
        CMP      R0,#+24
        BEQ.W    ??getBrain_pin_e_24
        CMP      R0,#+25
        BEQ.W    ??getBrain_pin_e_25
        CMP      R0,#+26
        BEQ.W    ??getBrain_pin_e_26
        CMP      R0,#+27
        BEQ.W    ??getBrain_pin_e_27
        CMP      R0,#+28
        BEQ.W    ??getBrain_pin_e_28
        CMP      R0,#+29
        BEQ.W    ??getBrain_pin_e_29
        CMP      R0,#+30
        BEQ.W    ??getBrain_pin_e_30
        CMP      R0,#+31
        BEQ.W    ??getBrain_pin_e_31
        CMP      R0,#+32
        BEQ.W    ??getBrain_pin_e_32
        CMP      R0,#+33
        BEQ.W    ??getBrain_pin_e_33
        CMP      R0,#+34
        BEQ.W    ??getBrain_pin_e_34
        CMP      R0,#+35
        BEQ.W    ??getBrain_pin_e_35
        CMP      R0,#+36
        BEQ.W    ??getBrain_pin_e_36
        CMP      R0,#+37
        BEQ.W    ??getBrain_pin_e_37
        CMP      R0,#+38
        BEQ.W    ??getBrain_pin_e_38
        CMP      R0,#+39
        BEQ.W    ??getBrain_pin_e_39
        CMP      R0,#+40
        BEQ.W    ??getBrain_pin_e_40
        CMP      R0,#+41
        BEQ.W    ??getBrain_pin_e_41
        CMP      R0,#+42
        BEQ.W    ??getBrain_pin_e_42
        CMP      R0,#+43
        BEQ.W    ??getBrain_pin_e_43
        CMP      R0,#+44
        BEQ.W    ??getBrain_pin_e_44
        CMP      R0,#+45
        BEQ.W    ??getBrain_pin_e_45
        CMP      R0,#+46
        BEQ.W    ??getBrain_pin_e_46
        CMP      R0,#+47
        BEQ.W    ??getBrain_pin_e_47
        CMP      R0,#+48
        BEQ.W    ??getBrain_pin_e_48
        CMP      R0,#+49
        BEQ.W    ??getBrain_pin_e_49
        CMP      R0,#+50
        BEQ.W    ??getBrain_pin_e_50
        CMP      R0,#+51
        BEQ.W    ??getBrain_pin_e_51
        CMP      R0,#+52
        BEQ.W    ??getBrain_pin_e_52
        CMP      R0,#+53
        BEQ.W    ??getBrain_pin_e_53
        CMP      R0,#+54
        BEQ.W    ??getBrain_pin_e_54
        CMP      R0,#+55
        BEQ.W    ??getBrain_pin_e_55
        CMP      R0,#+56
        BEQ.W    ??getBrain_pin_e_56
        CMP      R0,#+57
        BEQ.W    ??getBrain_pin_e_57
        CMP      R0,#+58
        BEQ.W    ??getBrain_pin_e_58
        CMP      R0,#+59
        BEQ.W    ??getBrain_pin_e_59
        CMP      R0,#+60
        BEQ.W    ??getBrain_pin_e_60
        CMP      R0,#+61
        BEQ.W    ??getBrain_pin_e_61
        CMP      R0,#+62
        BEQ.W    ??getBrain_pin_e_62
        CMP      R0,#+63
        BEQ.W    ??getBrain_pin_e_63
        CMP      R0,#+64
        BEQ.W    ??getBrain_pin_e_64
        CMP      R0,#+65
        BEQ.W    ??getBrain_pin_e_65
        CMP      R0,#+66
        BEQ.W    ??getBrain_pin_e_66
        CMP      R0,#+67
        BEQ.W    ??getBrain_pin_e_67
        CMP      R0,#+68
        BEQ.W    ??getBrain_pin_e_68
        CMP      R0,#+69
        BEQ.W    ??getBrain_pin_e_69
        CMP      R0,#+70
        BEQ.W    ??getBrain_pin_e_70
        CMP      R0,#+71
        BEQ.W    ??getBrain_pin_e_71
        CMP      R0,#+72
        BEQ.W    ??getBrain_pin_e_72
        CMP      R0,#+73
        BEQ.W    ??getBrain_pin_e_73
        CMP      R0,#+74
        BEQ.W    ??getBrain_pin_e_74
        CMP      R0,#+75
        BEQ.W    ??getBrain_pin_e_75
        CMP      R0,#+76
        BEQ.W    ??getBrain_pin_e_76
        CMP      R0,#+77
        BEQ.W    ??getBrain_pin_e_77
        CMP      R0,#+78
        BEQ.W    ??getBrain_pin_e_78
        CMP      R0,#+79
        BEQ.W    ??getBrain_pin_e_79
        CMP      R0,#+80
        BEQ.W    ??getBrain_pin_e_80
        CMP      R0,#+81
        BEQ.W    ??getBrain_pin_e_81
        LDR.W    R2,??DataTable23_76  ;; 0x77359400
        CMP      R0,R2
        BNE.W    ??getBrain_pin_e_82
//  444 case Force_4b_cranking_brain_pin:
//  445   return "Force_4b_cranking_brain_pin";
??getBrain_pin_e_83:
        LDR.W    R0,??DataTable23_77
        B.N      ??getBrain_pin_e_84
//  446 case GPIOA_0:
//  447   return "GPIOA_0";
??getBrain_pin_e_0:
        LDR.W    R0,??DataTable23_78
        B.N      ??getBrain_pin_e_84
//  448 case GPIOA_1:
//  449   return "GPIOA_1";
??getBrain_pin_e_1:
        LDR.W    R0,??DataTable23_79
        B.N      ??getBrain_pin_e_84
//  450 case GPIOA_10:
//  451   return "GPIOA_10";
??getBrain_pin_e_10:
        LDR.W    R0,??DataTable23_80
        B.N      ??getBrain_pin_e_84
//  452 case GPIOA_11:
//  453   return "GPIOA_11";
??getBrain_pin_e_11:
        LDR.W    R0,??DataTable23_81
        B.N      ??getBrain_pin_e_84
//  454 case GPIOA_12:
//  455   return "GPIOA_12";
??getBrain_pin_e_12:
        LDR.W    R0,??DataTable23_82
        B.N      ??getBrain_pin_e_84
//  456 case GPIOA_13:
//  457   return "GPIOA_13";
??getBrain_pin_e_13:
        LDR.W    R0,??DataTable23_83
        B.N      ??getBrain_pin_e_84
//  458 case GPIOA_14:
//  459   return "GPIOA_14";
??getBrain_pin_e_14:
        LDR.W    R0,??DataTable23_84
        B.N      ??getBrain_pin_e_84
//  460 case GPIOA_15:
//  461   return "GPIOA_15";
??getBrain_pin_e_15:
        LDR.W    R0,??DataTable23_85
        B.N      ??getBrain_pin_e_84
//  462 case GPIOA_2:
//  463   return "GPIOA_2";
??getBrain_pin_e_2:
        LDR.W    R0,??DataTable23_86
        B.N      ??getBrain_pin_e_84
//  464 case GPIOA_3:
//  465   return "GPIOA_3";
??getBrain_pin_e_3:
        LDR.W    R0,??DataTable23_87
        B.N      ??getBrain_pin_e_84
//  466 case GPIOA_4:
//  467   return "GPIOA_4";
??getBrain_pin_e_4:
        LDR.W    R0,??DataTable23_88
        B.N      ??getBrain_pin_e_84
//  468 case GPIOA_5:
//  469   return "GPIOA_5";
??getBrain_pin_e_5:
        LDR.W    R0,??DataTable23_89
        B.N      ??getBrain_pin_e_84
//  470 case GPIOA_6:
//  471   return "GPIOA_6";
??getBrain_pin_e_6:
        LDR.W    R0,??DataTable23_90
        B.N      ??getBrain_pin_e_84
//  472 case GPIOA_7:
//  473   return "GPIOA_7";
??getBrain_pin_e_7:
        LDR.W    R0,??DataTable23_91
        B.N      ??getBrain_pin_e_84
//  474 case GPIOA_8:
//  475   return "GPIOA_8";
??getBrain_pin_e_8:
        LDR.W    R0,??DataTable23_92
        B.N      ??getBrain_pin_e_84
//  476 case GPIOA_9:
//  477   return "GPIOA_9";
??getBrain_pin_e_9:
        LDR.W    R0,??DataTable23_93
        B.N      ??getBrain_pin_e_84
//  478 case GPIOB_0:
//  479   return "GPIOB_0";
??getBrain_pin_e_16:
        LDR.W    R0,??DataTable23_94
        B.N      ??getBrain_pin_e_84
//  480 case GPIOB_1:
//  481   return "GPIOB_1";
??getBrain_pin_e_17:
        LDR.W    R0,??DataTable23_95
        B.N      ??getBrain_pin_e_84
//  482 case GPIOB_10:
//  483   return "GPIOB_10";
??getBrain_pin_e_26:
        LDR.W    R0,??DataTable23_96
        B.N      ??getBrain_pin_e_84
//  484 case GPIOB_11:
//  485   return "GPIOB_11";
??getBrain_pin_e_27:
        LDR.W    R0,??DataTable23_97
        B.N      ??getBrain_pin_e_84
//  486 case GPIOB_12:
//  487   return "GPIOB_12";
??getBrain_pin_e_28:
        LDR.W    R0,??DataTable23_98
        B.N      ??getBrain_pin_e_84
//  488 case GPIOB_13:
//  489   return "GPIOB_13";
??getBrain_pin_e_29:
        LDR.W    R0,??DataTable23_99
        B.N      ??getBrain_pin_e_84
//  490 case GPIOB_14:
//  491   return "GPIOB_14";
??getBrain_pin_e_30:
        LDR.W    R0,??DataTable23_100
        B.N      ??getBrain_pin_e_84
//  492 case GPIOB_15:
//  493   return "GPIOB_15";
??getBrain_pin_e_31:
        LDR.W    R0,??DataTable23_101
        B.N      ??getBrain_pin_e_84
//  494 case GPIOB_2:
//  495   return "GPIOB_2";
??getBrain_pin_e_18:
        LDR.W    R0,??DataTable23_102
        B.N      ??getBrain_pin_e_84
//  496 case GPIOB_3:
//  497   return "GPIOB_3";
??getBrain_pin_e_19:
        LDR.W    R0,??DataTable23_103
        B.N      ??getBrain_pin_e_84
//  498 case GPIOB_4:
//  499   return "GPIOB_4";
??getBrain_pin_e_20:
        LDR.W    R0,??DataTable23_104
        B.N      ??getBrain_pin_e_84
//  500 case GPIOB_5:
//  501   return "GPIOB_5";
??getBrain_pin_e_21:
        LDR.W    R0,??DataTable23_105
        B.N      ??getBrain_pin_e_84
//  502 case GPIOB_6:
//  503   return "GPIOB_6";
??getBrain_pin_e_22:
        LDR.W    R0,??DataTable23_106
        B.N      ??getBrain_pin_e_84
//  504 case GPIOB_7:
//  505   return "GPIOB_7";
??getBrain_pin_e_23:
        LDR.W    R0,??DataTable23_107
        B.N      ??getBrain_pin_e_84
//  506 case GPIOB_8:
//  507   return "GPIOB_8";
??getBrain_pin_e_24:
        LDR.W    R0,??DataTable23_108
        B.N      ??getBrain_pin_e_84
//  508 case GPIOB_9:
//  509   return "GPIOB_9";
??getBrain_pin_e_25:
        LDR.W    R0,??DataTable23_109
        B.N      ??getBrain_pin_e_84
//  510 case GPIOC_0:
//  511   return "GPIOC_0";
??getBrain_pin_e_32:
        LDR.W    R0,??DataTable23_110
        B.N      ??getBrain_pin_e_84
//  512 case GPIOC_1:
//  513   return "GPIOC_1";
??getBrain_pin_e_33:
        LDR.W    R0,??DataTable23_111
        B.N      ??getBrain_pin_e_84
//  514 case GPIOC_10:
//  515   return "GPIOC_10";
??getBrain_pin_e_42:
        LDR.W    R0,??DataTable23_112
        B.N      ??getBrain_pin_e_84
//  516 case GPIOC_11:
//  517   return "GPIOC_11";
??getBrain_pin_e_43:
        LDR.W    R0,??DataTable23_113
        B.N      ??getBrain_pin_e_84
//  518 case GPIOC_12:
//  519   return "GPIOC_12";
??getBrain_pin_e_44:
        LDR.N    R0,??DataTable23_114
        B.N      ??getBrain_pin_e_84
//  520 case GPIOC_13:
//  521   return "GPIOC_13";
??getBrain_pin_e_45:
        LDR.N    R0,??DataTable23_115
        B.N      ??getBrain_pin_e_84
//  522 case GPIOC_14:
//  523   return "GPIOC_14";
??getBrain_pin_e_46:
        LDR.N    R0,??DataTable23_116
        B.N      ??getBrain_pin_e_84
//  524 case GPIOC_15:
//  525   return "GPIOC_15";
??getBrain_pin_e_47:
        LDR.N    R0,??DataTable23_117
        B.N      ??getBrain_pin_e_84
//  526 case GPIOC_2:
//  527   return "GPIOC_2";
??getBrain_pin_e_34:
        LDR.N    R0,??DataTable23_118
        B.N      ??getBrain_pin_e_84
//  528 case GPIOC_3:
//  529   return "GPIOC_3";
??getBrain_pin_e_35:
        LDR.N    R0,??DataTable23_119
        B.N      ??getBrain_pin_e_84
//  530 case GPIOC_4:
//  531   return "GPIOC_4";
??getBrain_pin_e_36:
        LDR.N    R0,??DataTable23_120
        B.N      ??getBrain_pin_e_84
//  532 case GPIOC_5:
//  533   return "GPIOC_5";
??getBrain_pin_e_37:
        LDR.N    R0,??DataTable23_121
        B.N      ??getBrain_pin_e_84
//  534 case GPIOC_6:
//  535   return "GPIOC_6";
??getBrain_pin_e_38:
        LDR.N    R0,??DataTable23_122
        B.N      ??getBrain_pin_e_84
//  536 case GPIOC_7:
//  537   return "GPIOC_7";
??getBrain_pin_e_39:
        LDR.N    R0,??DataTable23_123
        B.N      ??getBrain_pin_e_84
//  538 case GPIOC_8:
//  539   return "GPIOC_8";
??getBrain_pin_e_40:
        LDR.N    R0,??DataTable23_124
        B.N      ??getBrain_pin_e_84
//  540 case GPIOC_9:
//  541   return "GPIOC_9";
??getBrain_pin_e_41:
        LDR.N    R0,??DataTable23_125
        B.N      ??getBrain_pin_e_84
//  542 case GPIOD_0:
//  543   return "GPIOD_0";
??getBrain_pin_e_48:
        LDR.N    R0,??DataTable23_126
        B.N      ??getBrain_pin_e_84
//  544 case GPIOD_1:
//  545   return "GPIOD_1";
??getBrain_pin_e_49:
        LDR.N    R0,??DataTable23_127
        B.N      ??getBrain_pin_e_84
//  546 case GPIOD_10:
//  547   return "GPIOD_10";
??getBrain_pin_e_58:
        LDR.N    R0,??DataTable23_128
        B.N      ??getBrain_pin_e_84
//  548 case GPIOD_11:
//  549   return "GPIOD_11";
??getBrain_pin_e_59:
        LDR.N    R0,??DataTable23_129
        B.N      ??getBrain_pin_e_84
//  550 case GPIOD_12:
//  551   return "GPIOD_12";
??getBrain_pin_e_60:
        LDR.N    R0,??DataTable23_130
        B.N      ??getBrain_pin_e_84
//  552 case GPIOD_13:
//  553   return "GPIOD_13";
??getBrain_pin_e_61:
        LDR.N    R0,??DataTable23_131
        B.N      ??getBrain_pin_e_84
//  554 case GPIOD_14:
//  555   return "GPIOD_14";
??getBrain_pin_e_62:
        LDR.N    R0,??DataTable23_132
        B.N      ??getBrain_pin_e_84
//  556 case GPIOD_15:
//  557   return "GPIOD_15";
??getBrain_pin_e_63:
        LDR.N    R0,??DataTable23_133
        B.N      ??getBrain_pin_e_84
//  558 case GPIOD_2:
//  559   return "GPIOD_2";
??getBrain_pin_e_50:
        LDR.N    R0,??DataTable23_134
        B.N      ??getBrain_pin_e_84
//  560 case GPIOD_3:
//  561   return "GPIOD_3";
??getBrain_pin_e_51:
        LDR.N    R0,??DataTable23_135
        B.N      ??getBrain_pin_e_84
//  562 case GPIOD_4:
//  563   return "GPIOD_4";
??getBrain_pin_e_52:
        LDR.N    R0,??DataTable23_136
        B.N      ??getBrain_pin_e_84
//  564 case GPIOD_5:
//  565   return "GPIOD_5";
??getBrain_pin_e_53:
        LDR.N    R0,??DataTable23_137
        B.N      ??getBrain_pin_e_84
//  566 case GPIOD_6:
//  567   return "GPIOD_6";
??getBrain_pin_e_54:
        LDR.N    R0,??DataTable23_138
        B.N      ??getBrain_pin_e_84
//  568 case GPIOD_7:
//  569   return "GPIOD_7";
??getBrain_pin_e_55:
        LDR.N    R0,??DataTable23_139
        B.N      ??getBrain_pin_e_84
//  570 case GPIOD_8:
//  571   return "GPIOD_8";
??getBrain_pin_e_56:
        LDR.N    R0,??DataTable23_140
        B.N      ??getBrain_pin_e_84
//  572 case GPIOD_9:
//  573   return "GPIOD_9";
??getBrain_pin_e_57:
        LDR.N    R0,??DataTable23_141
        B.N      ??getBrain_pin_e_84
//  574 case GPIOE_0:
//  575   return "GPIOE_0";
??getBrain_pin_e_64:
        LDR.N    R0,??DataTable23_142
        B.N      ??getBrain_pin_e_84
//  576 case GPIOE_1:
//  577   return "GPIOE_1";
??getBrain_pin_e_65:
        LDR.N    R0,??DataTable23_143
        B.N      ??getBrain_pin_e_84
//  578 case GPIOE_10:
//  579   return "GPIOE_10";
??getBrain_pin_e_74:
        LDR.N    R0,??DataTable23_144
        B.N      ??getBrain_pin_e_84
//  580 case GPIOE_11:
//  581   return "GPIOE_11";
??getBrain_pin_e_75:
        LDR.N    R0,??DataTable23_145
        B.N      ??getBrain_pin_e_84
//  582 case GPIOE_12:
//  583   return "GPIOE_12";
??getBrain_pin_e_76:
        LDR.N    R0,??DataTable23_146
        B.N      ??getBrain_pin_e_84
//  584 case GPIOE_13:
//  585   return "GPIOE_13";
??getBrain_pin_e_77:
        LDR.N    R0,??DataTable23_147
        B.N      ??getBrain_pin_e_84
//  586 case GPIOE_14:
//  587   return "GPIOE_14";
??getBrain_pin_e_78:
        LDR.N    R0,??DataTable23_148
        B.N      ??getBrain_pin_e_84
//  588 case GPIOE_15:
//  589   return "GPIOE_15";
??getBrain_pin_e_79:
        LDR.N    R0,??DataTable23_149
        B.N      ??getBrain_pin_e_84
//  590 case GPIOE_2:
//  591   return "GPIOE_2";
??getBrain_pin_e_66:
        LDR.N    R0,??DataTable23_150
        B.N      ??getBrain_pin_e_84
//  592 case GPIOE_3:
//  593   return "GPIOE_3";
??getBrain_pin_e_67:
        LDR.N    R0,??DataTable23_151
        B.N      ??getBrain_pin_e_84
//  594 case GPIOE_4:
//  595   return "GPIOE_4";
??getBrain_pin_e_68:
        LDR.N    R0,??DataTable23_152
        B.N      ??getBrain_pin_e_84
//  596 case GPIOE_5:
//  597   return "GPIOE_5";
??getBrain_pin_e_69:
        LDR.N    R0,??DataTable23_153
        B.N      ??getBrain_pin_e_84
//  598 case GPIOE_6:
//  599   return "GPIOE_6";
??getBrain_pin_e_70:
        LDR.N    R0,??DataTable23_154
        B.N      ??getBrain_pin_e_84
//  600 case GPIOE_7:
//  601   return "GPIOE_7";
??getBrain_pin_e_71:
        LDR.N    R0,??DataTable23_155
        B.N      ??getBrain_pin_e_84
//  602 case GPIOE_8:
//  603   return "GPIOE_8";
??getBrain_pin_e_72:
        LDR.N    R0,??DataTable23_156
        B.N      ??getBrain_pin_e_84
//  604 case GPIOE_9:
//  605   return "GPIOE_9";
??getBrain_pin_e_73:
        LDR.N    R0,??DataTable23_157
        B.N      ??getBrain_pin_e_84
//  606 case GPIO_INVALID:
//  607   return "GPIO_INVALID";
??getBrain_pin_e_81:
        LDR.N    R0,??DataTable23_158
        B.N      ??getBrain_pin_e_84
//  608 case GPIO_UNASSIGNED:
//  609   return "GPIO_UNASSIGNED";
??getBrain_pin_e_80:
        LDR.N    R0,??DataTable23_159
        B.N      ??getBrain_pin_e_84
//  610   }
//  611  return NULL;
??getBrain_pin_e_82:
        MOVS     R0,#+0
??getBrain_pin_e_84:
        BX       LR               ;; return
          CFI EndBlock cfiBlock22
//  612 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22:
        DC32     `?<Constant "TT_UNUSED">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22_1:
        DC32     `?<Constant "Force_4b_adc_channel_...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22_2:
        DC32     `?<Constant "TV_LOW">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22_3:
        DC32     `?<Constant "T_SECONDARY">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22_4:
        DC32     `?<Constant "SHAFT_SECONDARY_UP">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22_5:
        DC32     `?<Constant "Force_4b_engine_load_...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22_6:
        DC32     `?<Constant "LM_ALPHA_N">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22_7:
        DC32     `?<Constant "LM_PLAIN_MAF">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22_8:
        DC32     `?<Constant "LM_REAL_MAF">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22_9:
        DC32     `?<Constant "LM_MAP">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22_10:
        DC32     `?<Constant "DM_HD44780">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable22_11:
        DC32     `?<Constant "DM_HD44780_OVER_PCF8574">`

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock23 Using cfiCommon0
          CFI Function _Z29getAir_pressure_sensor_type_e26air_pressure_sensor_type_e
          CFI NoCalls
        THUMB
//  613 const char *getAir_pressure_sensor_type_e(air_pressure_sensor_type_e value){
_Z29getAir_pressure_sensor_type_e26air_pressure_sensor_type_e:
        MOVS     R1,R0
//  614 switch(value) {
        MOVS     R0,R1
        CMP      R0,#+0
        BEQ.N    ??getAir_pressure_sensor_type_e_0
        CMP      R0,#+1
        BEQ.N    ??getAir_pressure_sensor_type_e_1
        CMP      R0,#+2
        BEQ.N    ??getAir_pressure_sensor_type_e_2
        CMP      R0,#+3
        BEQ.N    ??getAir_pressure_sensor_type_e_3
        CMP      R0,#+4
        BEQ.N    ??getAir_pressure_sensor_type_e_4
        CMP      R0,#+5
        BEQ.N    ??getAir_pressure_sensor_type_e_5
        CMP      R0,#+6
        BEQ.N    ??getAir_pressure_sensor_type_e_6
        LDR.N    R2,??DataTable23_76  ;; 0x77359400
        CMP      R0,R2
        BNE.N    ??getAir_pressure_sensor_type_e_7
//  615 case Force_4b_cranking_map_type:
//  616   return "Force_4b_cranking_map_type";
??getAir_pressure_sensor_type_e_8:
        LDR.N    R0,??DataTable23_160
        B.N      ??getAir_pressure_sensor_type_e_9
//  617 case MT_CUSTOM:
//  618   return "MT_CUSTOM";
??getAir_pressure_sensor_type_e_0:
        LDR.N    R0,??DataTable23_161
        B.N      ??getAir_pressure_sensor_type_e_9
//  619 case MT_DENSO183:
//  620   return "MT_DENSO183";
??getAir_pressure_sensor_type_e_1:
        LDR.N    R0,??DataTable23_162
        B.N      ??getAir_pressure_sensor_type_e_9
//  621 case MT_DODGE_NEON_2003:
//  622   return "MT_DODGE_NEON_2003";
??getAir_pressure_sensor_type_e_4:
        LDR.N    R0,??DataTable23_163
        B.N      ??getAir_pressure_sensor_type_e_9
//  623 case MT_HONDA3BAR:
//  624   return "MT_HONDA3BAR";
??getAir_pressure_sensor_type_e_3:
        LDR.N    R0,??DataTable23_164
        B.N      ??getAir_pressure_sensor_type_e_9
//  625 case MT_MPX4250:
//  626   return "MT_MPX4250";
??getAir_pressure_sensor_type_e_2:
        LDR.N    R0,??DataTable23_165
        B.N      ??getAir_pressure_sensor_type_e_9
//  627 case MT_SUBY_DENSO:
//  628   return "MT_SUBY_DENSO";
??getAir_pressure_sensor_type_e_5:
        LDR.N    R0,??DataTable23_166
        B.N      ??getAir_pressure_sensor_type_e_9
//  629 case MT_GM_3_BAR:
//  630   return "MT_GM_3_BAR";
??getAir_pressure_sensor_type_e_6:
        LDR.N    R0,??DataTable23_167
        B.N      ??getAir_pressure_sensor_type_e_9
//  631   }
//  632  return NULL;
??getAir_pressure_sensor_type_e_7:
        MOVS     R0,#+0
??getAir_pressure_sensor_type_e_9:
        BX       LR               ;; return
          CFI EndBlock cfiBlock23
//  633 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23:
        DC32     `?<Constant "LM_SPEED_DENSITY">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_1:
        DC32     `?<Constant "DM_NONE">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_2:
        DC32     `?<Constant "Force_4b_display_mode">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_3:
        DC32     `?<Constant "Force_4b_log_format">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_4:
        DC32     `?<Constant "LF_NATIVE">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_5:
        DC32     `?<Constant "LM_MLV">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_6:
        DC32     `?<Constant "Force_4b_idle_mode">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_7:
        DC32     `?<Constant "IM_AUTO">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_8:
        DC32     `?<Constant "IM_MANUAL">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_9:
        DC32     `?<Constant "Force_4b_pin_output_mode">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_10:
        DC32     `?<Constant "OM_DEFAULT">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_11:
        DC32     `?<Constant "OM_INVERTED">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_12:
        DC32     `?<Constant "OM_OPENDRAIN">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_13:
        DC32     `?<Constant "OM_OPENDRAIN_INVERTED">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_14:
        DC32     `?<Constant "Force_4b_gpio_mode">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_15:
        DC32     `?<Constant "Force_4b_pin_input_mode">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_16:
        DC32     `?<Constant "PI_DEFAULT">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_17:
        DC32     `?<Constant "PI_PULLDOWN">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_18:
        DC32     `?<Constant "PI_PULLUP">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_19:
        DC32     `?<Constant "FO_1_2_4_5_3">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_20:
        DC32     `?<Constant "FO_1_THEN_2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_21:
        DC32     `?<Constant "FO_1_8_4_3_6_5_7_2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_22:
        DC32     `?<Constant "FO_1_THEN_2_THEN_4_THEN3">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_23:
        DC32     `?<Constant "FO_1_THEN_3_THEN_2_THEN4">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_24:
        DC32     `?<Constant "FO_1_THEN_3_THEN_4_THEN2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_25:
        DC32     `?<Constant "FO_1_THEN_5_THEN_3_TH...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_26:
        DC32     `?<Constant "FO_1_THEN_4_THEN_2_TH...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_27:
        DC32     `?<Constant "FO_ONE_CYLINDER">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_28:
        DC32     `?<Constant "Force_4b_firing_order">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_29:
        DC32     `?<Constant "FOUR_STROKE_CAM_SENSOR">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_30:
        DC32     `?<Constant "FOUR_STROKE_CRANK_SENSOR">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_31:
        DC32     `?<Constant "TWO_STROKE">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_32:
        DC32     `?<Constant "Force_4b_operation_mo...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_33:
        DC32     `?<Constant "OM_NONE">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_34:
        DC32     `?<Constant "Force_4b_ignition_mode">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_35:
        DC32     `?<Constant "IM_INDIVIDUAL_COILS">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_36:
        DC32     `?<Constant "IM_ONE_COIL">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_37:
        DC32     `?<Constant "IM_WASTED_SPARK">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_38:
        DC32     `?<Constant "Force_4b_injection_mode">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_39:
        DC32     `?<Constant "IM_BATCH">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_40:
        DC32     `?<Constant "IM_SEQUENTIAL">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_41:
        DC32     `?<Constant "IM_SIMULTANEOUS">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_42:
        DC32     `?<Constant "CIM_DEFAULT">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_43:
        DC32     `?<Constant "CIM_FIXED_ANGLE">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_44:
        DC32     `?<Constant "Force_4b_cranking_ign...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_45:
        DC32     `?<Constant "Force_4b_spi_device">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_46:
        DC32     `?<Constant "SPI_DEVICE_1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_47:
        DC32     `?<Constant "SPI_DEVICE_2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_48:
        DC32     `?<Constant "SPI_DEVICE_3">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_49:
        DC32     `?<Constant "SPI_DEVICE_4">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_50:
        DC32     `?<Constant "SPI_NONE">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_51:
        DC32     `?<Constant "EFI_ADC_0">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_52:
        DC32     `?<Constant "EFI_ADC_1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_53:
        DC32     `?<Constant "EFI_ADC_10">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_54:
        DC32     `?<Constant "EFI_ADC_11">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_55:
        DC32     `?<Constant "EFI_ADC_12">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_56:
        DC32     `?<Constant "EFI_ADC_13">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_57:
        DC32     `?<Constant "EFI_ADC_14">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_58:
        DC32     `?<Constant "EFI_ADC_15">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_59:
        DC32     `?<Constant "EFI_ADC_2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_60:
        DC32     `?<Constant "EFI_ADC_3">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_61:
        DC32     `?<Constant "EFI_ADC_4">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_62:
        DC32     `?<Constant "EFI_ADC_5">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_63:
        DC32     `?<Constant "EFI_ADC_6">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_64:
        DC32     `?<Constant "EFI_ADC_7">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_65:
        DC32     `?<Constant "EFI_ADC_8">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_66:
        DC32     `?<Constant "EFI_ADC_9">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_67:
        DC32     `?<Constant "EFI_ADC_ERROR">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_68:
        DC32     `?<Constant "EFI_ADC_NONE">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_69:
        DC32     `?<Constant "Force_4b_cranking_adc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_70:
        DC32     `?<Constant "ES_14Point7_Free">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_71:
        DC32     `?<Constant "ES_BPSX_D1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_72:
        DC32     `?<Constant "ES_Innovate_MTX_L">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_73:
        DC32     `?<Constant "ES_NarrowBand">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_74:
        DC32     `?<Constant "ES_PLX">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_75:
        DC32     `?<Constant "Force_4b_ego_sensor">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_76:
        DC32     0x77359400

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_77:
        DC32     `?<Constant "Force_4b_cranking_bra...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_78:
        DC32     `?<Constant "GPIOA_0">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_79:
        DC32     `?<Constant "GPIOA_1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_80:
        DC32     `?<Constant "GPIOA_10">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_81:
        DC32     `?<Constant "GPIOA_11">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_82:
        DC32     `?<Constant "GPIOA_12">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_83:
        DC32     `?<Constant "GPIOA_13">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_84:
        DC32     `?<Constant "GPIOA_14">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_85:
        DC32     `?<Constant "GPIOA_15">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_86:
        DC32     `?<Constant "GPIOA_2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_87:
        DC32     `?<Constant "GPIOA_3">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_88:
        DC32     `?<Constant "GPIOA_4">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_89:
        DC32     `?<Constant "GPIOA_5">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_90:
        DC32     `?<Constant "GPIOA_6">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_91:
        DC32     `?<Constant "GPIOA_7">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_92:
        DC32     `?<Constant "GPIOA_8">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_93:
        DC32     `?<Constant "GPIOA_9">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_94:
        DC32     `?<Constant "GPIOB_0">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_95:
        DC32     `?<Constant "GPIOB_1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_96:
        DC32     `?<Constant "GPIOB_10">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_97:
        DC32     `?<Constant "GPIOB_11">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_98:
        DC32     `?<Constant "GPIOB_12">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_99:
        DC32     `?<Constant "GPIOB_13">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_100:
        DC32     `?<Constant "GPIOB_14">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_101:
        DC32     `?<Constant "GPIOB_15">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_102:
        DC32     `?<Constant "GPIOB_2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_103:
        DC32     `?<Constant "GPIOB_3">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_104:
        DC32     `?<Constant "GPIOB_4">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_105:
        DC32     `?<Constant "GPIOB_5">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_106:
        DC32     `?<Constant "GPIOB_6">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_107:
        DC32     `?<Constant "GPIOB_7">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_108:
        DC32     `?<Constant "GPIOB_8">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_109:
        DC32     `?<Constant "GPIOB_9">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_110:
        DC32     `?<Constant "GPIOC_0">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_111:
        DC32     `?<Constant "GPIOC_1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_112:
        DC32     `?<Constant "GPIOC_10">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_113:
        DC32     `?<Constant "GPIOC_11">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_114:
        DC32     `?<Constant "GPIOC_12">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_115:
        DC32     `?<Constant "GPIOC_13">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_116:
        DC32     `?<Constant "GPIOC_14">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_117:
        DC32     `?<Constant "GPIOC_15">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_118:
        DC32     `?<Constant "GPIOC_2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_119:
        DC32     `?<Constant "GPIOC_3">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_120:
        DC32     `?<Constant "GPIOC_4">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_121:
        DC32     `?<Constant "GPIOC_5">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_122:
        DC32     `?<Constant "GPIOC_6">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_123:
        DC32     `?<Constant "GPIOC_7">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_124:
        DC32     `?<Constant "GPIOC_8">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_125:
        DC32     `?<Constant "GPIOC_9">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_126:
        DC32     `?<Constant "GPIOD_0">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_127:
        DC32     `?<Constant "GPIOD_1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_128:
        DC32     `?<Constant "GPIOD_10">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_129:
        DC32     `?<Constant "GPIOD_11">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_130:
        DC32     `?<Constant "GPIOD_12">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_131:
        DC32     `?<Constant "GPIOD_13">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_132:
        DC32     `?<Constant "GPIOD_14">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_133:
        DC32     `?<Constant "GPIOD_15">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_134:
        DC32     `?<Constant "GPIOD_2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_135:
        DC32     `?<Constant "GPIOD_3">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_136:
        DC32     `?<Constant "GPIOD_4">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_137:
        DC32     `?<Constant "GPIOD_5">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_138:
        DC32     `?<Constant "GPIOD_6">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_139:
        DC32     `?<Constant "GPIOD_7">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_140:
        DC32     `?<Constant "GPIOD_8">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_141:
        DC32     `?<Constant "GPIOD_9">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_142:
        DC32     `?<Constant "GPIOE_0">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_143:
        DC32     `?<Constant "GPIOE_1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_144:
        DC32     `?<Constant "GPIOE_10">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_145:
        DC32     `?<Constant "GPIOE_11">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_146:
        DC32     `?<Constant "GPIOE_12">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_147:
        DC32     `?<Constant "GPIOE_13">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_148:
        DC32     `?<Constant "GPIOE_14">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_149:
        DC32     `?<Constant "GPIOE_15">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_150:
        DC32     `?<Constant "GPIOE_2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_151:
        DC32     `?<Constant "GPIOE_3">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_152:
        DC32     `?<Constant "GPIOE_4">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_153:
        DC32     `?<Constant "GPIOE_5">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_154:
        DC32     `?<Constant "GPIOE_6">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_155:
        DC32     `?<Constant "GPIOE_7">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_156:
        DC32     `?<Constant "GPIOE_8">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_157:
        DC32     `?<Constant "GPIOE_9">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_158:
        DC32     `?<Constant "GPIO_INVALID">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_159:
        DC32     `?<Constant "GPIO_UNASSIGNED">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_160:
        DC32     `?<Constant "Force_4b_cranking_map...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_161:
        DC32     `?<Constant "MT_CUSTOM">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_162:
        DC32     `?<Constant "MT_DENSO183">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_163:
        DC32     `?<Constant "MT_DODGE_NEON_2003">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_164:
        DC32     `?<Constant "MT_HONDA3BAR">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_165:
        DC32     `?<Constant "MT_MPX4250">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_166:
        DC32     `?<Constant "MT_SUBY_DENSO">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable23_167:
        DC32     `?<Constant "MT_GM_3_BAR">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Internal_ForceMyEnumI...">`:
        DATA
        DC8 "Internal_ForceMyEnumIntSize_timing_mode"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TM_DYNAMIC">`:
        DATA
        DC8 "TM_DYNAMIC"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TM_FIXED">`:
        DATA
        DC8 "TM_FIXED"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ACURA_RSX">`:
        DATA
        DC8 "ACURA_RSX"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "AUDI_AAN">`:
        DATA
        DC8 "AUDI_AAN"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SACHS">`:
        DATA
        DC8 "SACHS"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GM_2_2">`:
        DATA
        DC8 "GM_2_2"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "DODGE_RAM">`:
        DATA
        DC8 "DODGE_RAM"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "BMW_E34">`:
        DATA
        DC8 "BMW_E34"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "CITROEN_TU3JP">`:
        DATA
        DC8 "CITROEN_TU3JP"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "CUSTOM_ENGINE">`:
        DATA
        DC8 "CUSTOM_ENGINE"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "DODGE_NEON_1995">`:
        DATA
        DC8 "DODGE_NEON_1995"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "DODGE_NEON_2003">`:
        DATA
        DC8 "DODGE_NEON_2003"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ET_UNUSED">`:
        DATA
        DC8 "ET_UNUSED"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FORD_ASPIRE_1996">`:
        DATA
        DC8 "FORD_ASPIRE_1996"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FORD_ESCORT_GT">`:
        DATA
        DC8 "FORD_ESCORT_GT"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FORD_FIESTA">`:
        DATA
        DC8 "FORD_FIESTA"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FORD_INLINE_6_1995">`:
        DATA
        DC8 "FORD_INLINE_6_1995"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Force_4b_engine_type">`:
        DATA
        DC8 "Force_4b_engine_type"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GY6_139QMB">`:
        DATA
        DC8 "GY6_139QMB"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "HONDA_ACCORD_CD">`:
        DATA
        DC8 "HONDA_ACCORD_CD"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "HONDA_ACCORD_CD_DIP">`:
        DATA
        DC8 "HONDA_ACCORD_CD_DIP"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "HONDA_ACCORD_CD_TWO_W...">`:
        DATA
        DC8 "HONDA_ACCORD_CD_TWO_WIRES"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MAZDA_323">`:
        DATA
        DC8 "MAZDA_323"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MAZDA_626">`:
        DATA
        DC8 "MAZDA_626"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MAZDA_MIATA_NB">`:
        DATA
        DC8 "MAZDA_MIATA_NB"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MIATA_1990">`:
        DATA
        DC8 "MIATA_1990"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MIATA_1994_DEVIATOR">`:
        DATA
        DC8 "MIATA_1994_DEVIATOR"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MIATA_1994_SPAGS">`:
        DATA
        DC8 "MIATA_1994_SPAGS"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MIATA_1996">`:
        DATA
        DC8 "MIATA_1996"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MINI_COOPER_R50">`:
        DATA
        DC8 "MINI_COOPER_R50"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MITSU_4G93">`:
        DATA
        DC8 "MITSU_4G93"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "NISSAN_PRIMERA">`:
        DATA
        DC8 "NISSAN_PRIMERA"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ROVER_V8">`:
        DATA
        DC8 "ROVER_V8"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SATURN_ION_2004">`:
        DATA
        DC8 "SATURN_ION_2004"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SUBARU_2003_WRX">`:
        DATA
        DC8 "SUBARU_2003_WRX"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TEST_ENGINE">`:
        DATA
        DC8 "TEST_ENGINE"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "VW_ABA">`:
        DATA
        DC8 "VW_ABA"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Force_4b_trigger_type">`:
        DATA
        DC8 "Force_4b_trigger_type"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TT_DODGE_NEON_1995">`:
        DATA
        DC8 "TT_DODGE_NEON_1995"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TT_DODGE_NEON_2003">`:
        DATA
        DC8 "TT_DODGE_NEON_2003"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TT_FORD_ASPIRE">`:
        DATA
        DC8 "TT_FORD_ASPIRE"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TT_MAZDA_SOHC_4">`:
        DATA
        DC8 "TT_MAZDA_SOHC_4"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TT_MAZDA_DOHC_1_4">`:
        DATA
        DC8 "TT_MAZDA_DOHC_1_4"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TT_ONE_PLUS_ONE">`:
        DATA
        DC8 "TT_ONE_PLUS_ONE"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TT_GM_7X">`:
        DATA
        DC8 "TT_GM_7X"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TT_HONDA_ACCORD_CD">`:
        DATA
        DC8 "TT_HONDA_ACCORD_CD"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TT_HONDA_ACCORD_1_24">`:
        DATA
        DC8 "TT_HONDA_ACCORD_1_24"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TT_HONDA_ACCORD_CD_DIP">`:
        DATA
        DC8 "TT_HONDA_ACCORD_CD_DIP"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TT_HONDA_ACCORD_CD_TW...">`:
        DATA
        DC8 "TT_HONDA_ACCORD_CD_TWO_WIRES"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TT_MAZDA_MIATA_NA">`:
        DATA
        DC8 "TT_MAZDA_MIATA_NA"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TT_MAZDA_MIATA_NB">`:
        DATA
        DC8 "TT_MAZDA_MIATA_NB"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TT_MINI_COOPER_R50">`:
        DATA
        DC8 "TT_MINI_COOPER_R50"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TT_MITSU">`:
        DATA
        DC8 "TT_MITSU"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TT_TOOTHED_WHEEL">`:
        DATA
        DC8 "TT_TOOTHED_WHEEL"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TT_TOOTHED_WHEEL_36_1">`:
        DATA
        DC8 "TT_TOOTHED_WHEEL_36_1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TT_TOOTHED_WHEEL_60_2">`:
        DATA
        DC8 "TT_TOOTHED_WHEEL_60_2"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TT_ONE_PLUS_TOOTHED_W...">`:
        DATA
        DC8 "TT_ONE_PLUS_TOOTHED_WHEEL_60_2"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TT_ONE">`:
        DATA
        DC8 "TT_ONE"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TT_60_2_VW">`:
        DATA
        DC8 "TT_60_2_VW"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TT_DODGE_RAM">`:
        DATA
        DC8 "TT_DODGE_RAM"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TT_UNUSED">`:
        DATA
        DC8 "TT_UNUSED"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ADC_FAST">`:
        DATA
        DC8 "ADC_FAST"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ADC_OFF">`:
        DATA
        DC8 "ADC_OFF"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ADC_SLOW">`:
        DATA
        DC8 "ADC_SLOW"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Force_4b_adc_channel_...">`:
        DATA
        DC8 "Force_4b_adc_channel_mode"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TV_HIGH">`:
        DATA
        DC8 "TV_HIGH"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TV_LOW">`:
        DATA
        DC8 "TV_LOW"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "T_CHANNEL_3">`:
        DATA
        DC8 "T_CHANNEL_3"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "T_PRIMARY">`:
        DATA
        DC8 "T_PRIMARY"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "T_SECONDARY">`:
        DATA
        DC8 "T_SECONDARY"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SHAFT_3RD_DOWN">`:
        DATA
        DC8 "SHAFT_3RD_DOWN"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SHAFT_3RD_UP">`:
        DATA
        DC8 "SHAFT_3RD_UP"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SHAFT_PRIMARY_DOWN">`:
        DATA
        DC8 "SHAFT_PRIMARY_DOWN"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SHAFT_PRIMARY_UP">`:
        DATA
        DC8 "SHAFT_PRIMARY_UP"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SHAFT_SECONDARY_DOWN">`:
        DATA
        DC8 "SHAFT_SECONDARY_DOWN"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SHAFT_SECONDARY_UP">`:
        DATA
        DC8 "SHAFT_SECONDARY_UP"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Force_4b_engine_load_...">`:
        DATA
        DC8 "Force_4b_engine_load_mode"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "LM_ALPHA_N">`:
        DATA
        DC8 "LM_ALPHA_N"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "LM_PLAIN_MAF">`:
        DATA
        DC8 "LM_PLAIN_MAF"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "LM_REAL_MAF">`:
        DATA
        DC8 "LM_REAL_MAF"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "LM_MAP">`:
        DATA
        DC8 "LM_MAP"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "LM_SPEED_DENSITY">`:
        DATA
        DC8 "LM_SPEED_DENSITY"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "DM_HD44780">`:
        DATA
        DC8 "DM_HD44780"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "DM_HD44780_OVER_PCF8574">`:
        DATA
        DC8 "DM_HD44780_OVER_PCF8574"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "DM_NONE">`:
        DATA
        DC8 "DM_NONE"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Force_4b_display_mode">`:
        DATA
        DC8 "Force_4b_display_mode"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Force_4b_log_format">`:
        DATA
        DC8 "Force_4b_log_format"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "LF_NATIVE">`:
        DATA
        DC8 "LF_NATIVE"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "LM_MLV">`:
        DATA
        DC8 "LM_MLV"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Force_4b_idle_mode">`:
        DATA
        DC8 "Force_4b_idle_mode"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "IM_AUTO">`:
        DATA
        DC8 "IM_AUTO"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "IM_MANUAL">`:
        DATA
        DC8 "IM_MANUAL"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Force_4b_pin_output_mode">`:
        DATA
        DC8 "Force_4b_pin_output_mode"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "OM_DEFAULT">`:
        DATA
        DC8 "OM_DEFAULT"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "OM_INVERTED">`:
        DATA
        DC8 "OM_INVERTED"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "OM_OPENDRAIN">`:
        DATA
        DC8 "OM_OPENDRAIN"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "OM_OPENDRAIN_INVERTED">`:
        DATA
        DC8 "OM_OPENDRAIN_INVERTED"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Force_4b_gpio_mode">`:
        DATA
        DC8 "Force_4b_gpio_mode"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Force_4b_pin_input_mode">`:
        DATA
        DC8 "Force_4b_pin_input_mode"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "PI_DEFAULT">`:
        DATA
        DC8 "PI_DEFAULT"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "PI_PULLDOWN">`:
        DATA
        DC8 "PI_PULLDOWN"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "PI_PULLUP">`:
        DATA
        DC8 "PI_PULLUP"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FO_1_2_4_5_3">`:
        DATA
        DC8 "FO_1_2_4_5_3"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FO_1_THEN_2">`:
        DATA
        DC8 "FO_1_THEN_2"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FO_1_8_4_3_6_5_7_2">`:
        DATA
        DC8 "FO_1_8_4_3_6_5_7_2"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FO_1_THEN_2_THEN_4_THEN3">`:
        DATA
        DC8 "FO_1_THEN_2_THEN_4_THEN3"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FO_1_THEN_3_THEN_2_THEN4">`:
        DATA
        DC8 "FO_1_THEN_3_THEN_2_THEN4"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FO_1_THEN_3_THEN_4_THEN2">`:
        DATA
        DC8 "FO_1_THEN_3_THEN_4_THEN2"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FO_1_THEN_5_THEN_3_TH...">`:
        DATA
        DC8 "FO_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FO_1_THEN_4_THEN_2_TH...">`:
        DATA
        DC8 "FO_1_THEN_4_THEN_2_THEN_5_THEN_3_THEN_6"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FO_ONE_CYLINDER">`:
        DATA
        DC8 "FO_ONE_CYLINDER"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Force_4b_firing_order">`:
        DATA
        DC8 "Force_4b_firing_order"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FOUR_STROKE_CAM_SENSOR">`:
        DATA
        DC8 "FOUR_STROKE_CAM_SENSOR"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FOUR_STROKE_CRANK_SENSOR">`:
        DATA
        DC8 "FOUR_STROKE_CRANK_SENSOR"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TWO_STROKE">`:
        DATA
        DC8 "TWO_STROKE"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Force_4b_operation_mo...">`:
        DATA
        DC8 "Force_4b_operation_mode_e"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "OM_NONE">`:
        DATA
        DC8 "OM_NONE"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Force_4b_ignition_mode">`:
        DATA
        DC8 "Force_4b_ignition_mode"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "IM_INDIVIDUAL_COILS">`:
        DATA
        DC8 "IM_INDIVIDUAL_COILS"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "IM_ONE_COIL">`:
        DATA
        DC8 "IM_ONE_COIL"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "IM_WASTED_SPARK">`:
        DATA
        DC8 "IM_WASTED_SPARK"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Force_4b_injection_mode">`:
        DATA
        DC8 "Force_4b_injection_mode"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "IM_BATCH">`:
        DATA
        DC8 "IM_BATCH"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "IM_SEQUENTIAL">`:
        DATA
        DC8 "IM_SEQUENTIAL"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "IM_SIMULTANEOUS">`:
        DATA
        DC8 "IM_SIMULTANEOUS"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "CIM_DEFAULT">`:
        DATA
        DC8 "CIM_DEFAULT"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "CIM_FIXED_ANGLE">`:
        DATA
        DC8 "CIM_FIXED_ANGLE"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Force_4b_cranking_ign...">`:
        DATA
        DC8 "Force_4b_cranking_ignition_mode"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Force_4b_spi_device">`:
        DATA
        DC8 "Force_4b_spi_device"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SPI_DEVICE_1">`:
        DATA
        DC8 "SPI_DEVICE_1"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SPI_DEVICE_2">`:
        DATA
        DC8 "SPI_DEVICE_2"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SPI_DEVICE_3">`:
        DATA
        DC8 "SPI_DEVICE_3"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SPI_DEVICE_4">`:
        DATA
        DC8 "SPI_DEVICE_4"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SPI_NONE">`:
        DATA
        DC8 "SPI_NONE"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EFI_ADC_0">`:
        DATA
        DC8 "EFI_ADC_0"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EFI_ADC_1">`:
        DATA
        DC8 "EFI_ADC_1"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EFI_ADC_10">`:
        DATA
        DC8 "EFI_ADC_10"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EFI_ADC_11">`:
        DATA
        DC8 "EFI_ADC_11"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EFI_ADC_12">`:
        DATA
        DC8 "EFI_ADC_12"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EFI_ADC_13">`:
        DATA
        DC8 "EFI_ADC_13"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EFI_ADC_14">`:
        DATA
        DC8 "EFI_ADC_14"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EFI_ADC_15">`:
        DATA
        DC8 "EFI_ADC_15"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EFI_ADC_2">`:
        DATA
        DC8 "EFI_ADC_2"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EFI_ADC_3">`:
        DATA
        DC8 "EFI_ADC_3"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EFI_ADC_4">`:
        DATA
        DC8 "EFI_ADC_4"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EFI_ADC_5">`:
        DATA
        DC8 "EFI_ADC_5"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EFI_ADC_6">`:
        DATA
        DC8 "EFI_ADC_6"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EFI_ADC_7">`:
        DATA
        DC8 "EFI_ADC_7"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EFI_ADC_8">`:
        DATA
        DC8 "EFI_ADC_8"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EFI_ADC_9">`:
        DATA
        DC8 "EFI_ADC_9"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EFI_ADC_ERROR">`:
        DATA
        DC8 "EFI_ADC_ERROR"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EFI_ADC_NONE">`:
        DATA
        DC8 "EFI_ADC_NONE"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Force_4b_cranking_adc...">`:
        DATA
        DC8 "Force_4b_cranking_adc_channel"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ES_14Point7_Free">`:
        DATA
        DC8 "ES_14Point7_Free"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ES_BPSX_D1">`:
        DATA
        DC8 "ES_BPSX_D1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ES_Innovate_MTX_L">`:
        DATA
        DC8 "ES_Innovate_MTX_L"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ES_NarrowBand">`:
        DATA
        DC8 "ES_NarrowBand"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ES_PLX">`:
        DATA
        DC8 "ES_PLX"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Force_4b_ego_sensor">`:
        DATA
        DC8 "Force_4b_ego_sensor"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Force_4b_cranking_bra...">`:
        DATA
        DC8 "Force_4b_cranking_brain_pin"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOA_0">`:
        DATA
        DC8 "GPIOA_0"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOA_1">`:
        DATA
        DC8 "GPIOA_1"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOA_10">`:
        DATA
        DC8 "GPIOA_10"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOA_11">`:
        DATA
        DC8 "GPIOA_11"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOA_12">`:
        DATA
        DC8 "GPIOA_12"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOA_13">`:
        DATA
        DC8 "GPIOA_13"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOA_14">`:
        DATA
        DC8 "GPIOA_14"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOA_15">`:
        DATA
        DC8 "GPIOA_15"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOA_2">`:
        DATA
        DC8 "GPIOA_2"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOA_3">`:
        DATA
        DC8 "GPIOA_3"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOA_4">`:
        DATA
        DC8 "GPIOA_4"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOA_5">`:
        DATA
        DC8 "GPIOA_5"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOA_6">`:
        DATA
        DC8 "GPIOA_6"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOA_7">`:
        DATA
        DC8 "GPIOA_7"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOA_8">`:
        DATA
        DC8 "GPIOA_8"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOA_9">`:
        DATA
        DC8 "GPIOA_9"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOB_0">`:
        DATA
        DC8 "GPIOB_0"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOB_1">`:
        DATA
        DC8 "GPIOB_1"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOB_10">`:
        DATA
        DC8 "GPIOB_10"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOB_11">`:
        DATA
        DC8 "GPIOB_11"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOB_12">`:
        DATA
        DC8 "GPIOB_12"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOB_13">`:
        DATA
        DC8 "GPIOB_13"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOB_14">`:
        DATA
        DC8 "GPIOB_14"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOB_15">`:
        DATA
        DC8 "GPIOB_15"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOB_2">`:
        DATA
        DC8 "GPIOB_2"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOB_3">`:
        DATA
        DC8 "GPIOB_3"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOB_4">`:
        DATA
        DC8 "GPIOB_4"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOB_5">`:
        DATA
        DC8 "GPIOB_5"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOB_6">`:
        DATA
        DC8 "GPIOB_6"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOB_7">`:
        DATA
        DC8 "GPIOB_7"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOB_8">`:
        DATA
        DC8 "GPIOB_8"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOB_9">`:
        DATA
        DC8 "GPIOB_9"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOC_0">`:
        DATA
        DC8 "GPIOC_0"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOC_1">`:
        DATA
        DC8 "GPIOC_1"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOC_10">`:
        DATA
        DC8 "GPIOC_10"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOC_11">`:
        DATA
        DC8 "GPIOC_11"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOC_12">`:
        DATA
        DC8 "GPIOC_12"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOC_13">`:
        DATA
        DC8 "GPIOC_13"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOC_14">`:
        DATA
        DC8 "GPIOC_14"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOC_15">`:
        DATA
        DC8 "GPIOC_15"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOC_2">`:
        DATA
        DC8 "GPIOC_2"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOC_3">`:
        DATA
        DC8 "GPIOC_3"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOC_4">`:
        DATA
        DC8 "GPIOC_4"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOC_5">`:
        DATA
        DC8 "GPIOC_5"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOC_6">`:
        DATA
        DC8 "GPIOC_6"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOC_7">`:
        DATA
        DC8 "GPIOC_7"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOC_8">`:
        DATA
        DC8 "GPIOC_8"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOC_9">`:
        DATA
        DC8 "GPIOC_9"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOD_0">`:
        DATA
        DC8 "GPIOD_0"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOD_1">`:
        DATA
        DC8 "GPIOD_1"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOD_10">`:
        DATA
        DC8 "GPIOD_10"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOD_11">`:
        DATA
        DC8 "GPIOD_11"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOD_12">`:
        DATA
        DC8 "GPIOD_12"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOD_13">`:
        DATA
        DC8 "GPIOD_13"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOD_14">`:
        DATA
        DC8 "GPIOD_14"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOD_15">`:
        DATA
        DC8 "GPIOD_15"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOD_2">`:
        DATA
        DC8 "GPIOD_2"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOD_3">`:
        DATA
        DC8 "GPIOD_3"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOD_4">`:
        DATA
        DC8 "GPIOD_4"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOD_5">`:
        DATA
        DC8 "GPIOD_5"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOD_6">`:
        DATA
        DC8 "GPIOD_6"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOD_7">`:
        DATA
        DC8 "GPIOD_7"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOD_8">`:
        DATA
        DC8 "GPIOD_8"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOD_9">`:
        DATA
        DC8 "GPIOD_9"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOE_0">`:
        DATA
        DC8 "GPIOE_0"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOE_1">`:
        DATA
        DC8 "GPIOE_1"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOE_10">`:
        DATA
        DC8 "GPIOE_10"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOE_11">`:
        DATA
        DC8 "GPIOE_11"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOE_12">`:
        DATA
        DC8 "GPIOE_12"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOE_13">`:
        DATA
        DC8 "GPIOE_13"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOE_14">`:
        DATA
        DC8 "GPIOE_14"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOE_15">`:
        DATA
        DC8 "GPIOE_15"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOE_2">`:
        DATA
        DC8 "GPIOE_2"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOE_3">`:
        DATA
        DC8 "GPIOE_3"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOE_4">`:
        DATA
        DC8 "GPIOE_4"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOE_5">`:
        DATA
        DC8 "GPIOE_5"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOE_6">`:
        DATA
        DC8 "GPIOE_6"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOE_7">`:
        DATA
        DC8 "GPIOE_7"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOE_8">`:
        DATA
        DC8 "GPIOE_8"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIOE_9">`:
        DATA
        DC8 "GPIOE_9"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIO_INVALID">`:
        DATA
        DC8 "GPIO_INVALID"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIO_UNASSIGNED">`:
        DATA
        DC8 "GPIO_UNASSIGNED"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Force_4b_cranking_map...">`:
        DATA
        DC8 "Force_4b_cranking_map_type"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MT_CUSTOM">`:
        DATA
        DC8 "MT_CUSTOM"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MT_DENSO183">`:
        DATA
        DC8 "MT_DENSO183"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MT_DODGE_NEON_2003">`:
        DATA
        DC8 "MT_DODGE_NEON_2003"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MT_HONDA3BAR">`:
        DATA
        DC8 "MT_HONDA3BAR"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MT_MPX4250">`:
        DATA
        DC8 "MT_MPX4250"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MT_SUBY_DENSO">`:
        DATA
        DC8 "MT_SUBY_DENSO"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MT_GM_3_BAR">`:
        DATA
        DC8 "MT_GM_3_BAR"

        END
// 
// 3 636 bytes in section .rodata
// 3 858 bytes in section .text
// 
// 3 858 bytes of CODE  memory
// 3 636 bytes of CONST memory
//
//Errors: none
//Warnings: none
