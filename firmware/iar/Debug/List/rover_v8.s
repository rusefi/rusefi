///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:25 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       rover_v8.cpp                                         /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       rover_v8.cpp -lCN F:\stuff\rusefi_sourceforge\firmwa /
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
//                       rover_v8.s                                           /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME rover_v8

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z15setWholeFuelMapf
        EXTERN _Z16setOperationModeP22engine_configuration_s16operation_mode_e
        EXTERN boardConfiguration
        EXTERN engineConfiguration

        PUBLIC _Z10setRoverv8v
        PUBLIC _Z22setFrankenstein_01_LCDP21board_configuration_s
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\config\engines\rover_v8.cpp
//    1 /**
//    2  * @file	rover_v8.cpp
//    3  *
//    4  * V8, firing order 18436572
//    5  *
//    6  * This config overrides some values of the default configuration which is set by setDefaultConfiguration() method
//    7  *
//    8  * set_engine_type 10
//    9  *
//   10  * ROVER_V8 = 10
//   11  *
//   12  * @date Jun 27, 2014
//   13  * @author Andrey Belomutskiy, (c) 2012-2015
//   14  */
//   15 
//   16 #include "main.h"
//   17 #include "rover_v8.h"
//   18 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z22setFrankenstein_01_LCDP21board_configuration_s
          CFI NoCalls
        THUMB
//   19 void setFrankenstein_01_LCD(board_configuration_s *boardConfiguration) {
//   20 	boardConfiguration->HD44780_rs = GPIOE_9;
_Z22setFrankenstein_01_LCDP21board_configuration_s:
        MOVS     R1,#+73
        STR      R1,[R0, #+176]
//   21 	boardConfiguration->HD44780_e = GPIOE_11;
        MOVS     R1,#+75
        STR      R1,[R0, #+180]
//   22 	boardConfiguration->HD44780_db4 = GPIOE_13;
        MOVS     R1,#+77
        STR      R1,[R0, #+184]
//   23 	boardConfiguration->HD44780_db5 = GPIOE_15;
        MOVS     R1,#+79
        STR      R1,[R0, #+188]
//   24 	boardConfiguration->HD44780_db6 = GPIOB_11;
        MOVS     R1,#+27
        STR      R1,[R0, #+192]
//   25 	boardConfiguration->HD44780_db7 = GPIOB_13;
        MOVS     R1,#+29
        STR      R1,[R0, #+196]
//   26 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//   27 
//   28 EXTERN_ENGINE;
//   29 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z10setRoverv8v
        THUMB
//   30 void setRoverv8(DECLARE_ENGINE_PARAMETER_F) {
_Z10setRoverv8v:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   31 	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
        MOVS     R1,#+1
        LDR.N    R0,??DataTable0
        LDR      R0,[R0, #+0]
          CFI FunCall _Z16setOperationModeP22engine_configuration_s16operation_mode_e
        BL       _Z16setOperationModeP22engine_configuration_s16operation_mode_e
//   32 	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_36_1;
        LDR.N    R0,??DataTable0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+9
        STR      R1,[R0, #+536]
//   33 
//   34 	engineConfiguration->specs.displacement = 3.528;
        LDR.N    R0,??DataTable0
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable0_1  ;; 0x4061cac1
        STR      R1,[R0, #+400]
//   35 	engineConfiguration->specs.cylindersCount = 8;
        LDR.N    R0,??DataTable0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+8
        STR      R1,[R0, #+404]
//   36 	engineConfiguration->specs.firingOrder = FO_1_8_4_3_6_5_7_2;
        LDR.N    R0,??DataTable0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+5
        STR      R1,[R0, #+408]
//   37 
//   38 	// set_rpm_hard_limit 4000
//   39 	engineConfiguration->rpmHardLimit = 4000; // yes, 4k. let's play it safe for now
        LDR.N    R0,??DataTable0
        LDR      R0,[R0, #+0]
        MOV      R1,#+4000
        STR      R1,[R0, #+420]
//   40 	// set_cranking_rpm 550
//   41 	engineConfiguration->cranking.rpm = 550;
        LDR.N    R0,??DataTable0
        LDR      R0,[R0, #+0]
        MOVW     R1,#+550
        STRH     R1,[R0, #+96]
//   42 
//   43 	// set_whole_fuel_map 3
//   44 	setWholeFuelMap(3 PASS_ENGINE_PARAMETER);
        VMOV.F32 S0,#3.0
          CFI FunCall _Z15setWholeFuelMapf
        BL       _Z15setWholeFuelMapf
//   45 
//   46 	// set_cranking_injection_mode 0
//   47 	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
        LDR.N    R0,??DataTable0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+428]
//   48 	// set_injection_mode 1
//   49 	engineConfiguration->injectionMode = IM_SEQUENTIAL;
        LDR.N    R0,??DataTable0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+432]
//   50 
//   51 	// set_ignition_mode 2
//   52 	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
        LDR.N    R0,??DataTable0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+444]
//   53 
//   54 	// Frankenstein: low side - out #1: PC14
//   55 	// Frankenstein: low side - out #2: PC15
//   56 	// Frankenstein: low side - out #3: PE6
//   57 	// Frankenstein: low side - out #4: PC13
//   58 	// Frankenstein: low side - out #5: PE4
//   59 	// Frankenstein: low side - out #6: PE5
//   60 	// Frankenstein: low side - out #7: PE2
//   61 	// Frankenstein: low side - out #8: PE3
//   62 	// Frankenstein: low side - out #9: PE0
//   63 	// Frankenstein: low side - out #10: PE1
//   64 	// Frankenstein: low side - out #11: PB8
//   65 	// Frankenstein: low side - out #12: PB9
//   66 
//   67 	boardConfiguration->injectionPins[0] = GPIOB_9; // Frankenstein: low side - out #12
        LDR.N    R0,??DataTable0_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+25
        STR      R1,[R0, #+32]
//   68 	boardConfiguration->injectionPins[1] = GPIOB_8; // Frankenstein: low side - out #11
        LDR.N    R0,??DataTable0_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+24
        STR      R1,[R0, #+36]
//   69 	boardConfiguration->injectionPins[2] = GPIOE_3; // Frankenstein: low side - out #8
        LDR.N    R0,??DataTable0_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+67
        STR      R1,[R0, #+40]
//   70 	boardConfiguration->injectionPins[3] = GPIOE_5; // Frankenstein: low side - out #6
        LDR.N    R0,??DataTable0_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+69
        STR      R1,[R0, #+44]
//   71 
//   72 	boardConfiguration->fuelPumpPin = GPIOC_13; // Frankenstein: low side - out #4
        LDR.N    R0,??DataTable0_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+45
        STR      R1,[R0, #+24]
//   73 	boardConfiguration->fuelPumpPinMode = OM_DEFAULT;
        LDR.N    R0,??DataTable0_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+28]
//   74 
//   75 	// set_injection_pin_mode 0
//   76 	boardConfiguration->injectionPinMode = OM_DEFAULT;
        LDR.N    R0,??DataTable0_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+0
        STR      R1,[R0, #+80]
//   77 
//   78 	boardConfiguration->canTxPin = GPIOB_6;
        LDR.N    R0,??DataTable0_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+22
        STR      R1,[R0, #+344]
//   79 	boardConfiguration->canRxPin = GPIOB_12;
        LDR.N    R0,??DataTable0_2
        LDR      R0,[R0, #+0]
        MOVS     R1,#+28
        STR      R1,[R0, #+348]
//   80 	engineConfiguration->canWriteEnabled = true;
        LDR.N    R0,??DataTable0
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        ORRS     R0,R0,#0x2
        LDR.N    R1,??DataTable0
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1497]
//   81 	engineConfiguration->canReadEnabled = false;
        LDR.N    R0,??DataTable0
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        ANDS     R0,R0,#0xFE
        LDR.N    R1,??DataTable0
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1497]
//   82 	engineConfiguration->isCanEnabled = true;
        LDR.N    R0,??DataTable0
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        ORRS     R0,R0,#0x40
        LDR.N    R1,??DataTable0
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1496]
//   83 	engineConfiguration->canNbcType = CAN_BUS_MAZDA_RX8;
        LDR.N    R0,??DataTable0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+3
        STR      R1,[R0, #+492]
//   84 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_1:
        DC32     0x4061cac1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_2:
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
// 312 bytes in section .text
// 
// 312 bytes of CODE memory
//
//Errors: none
//Warnings: 1
