///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:54 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       mitsubishi.cpp                                       /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       mitsubishi.cpp -lCN F:\stuff\rusefi_sourceforge\firm /
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
//                       mitsubishi.s                                         /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME mitsubishi

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z12setEgoSensor12ego_sensor_e
        EXTERN _Z15setWholeFuelMapf
        EXTERN _Z18setCommonNTCSensorP14ThermistorConf
        EXTERN _Z7setFsioi11brain_pin_ePKc
        EXTERN boardConfiguration
        EXTERN engineConfiguration

        PUBLIC _Z26setMitsubishiConfigurationv
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\config\engines\mitsubishi.cpp
//    1 /**
//    2  * @file	mitsubishi.cpp
//    3  *
//    4  * MITSU_4G93 16
//    5  *
//    6  * set_engine_type 16
//    7  *
//    8  * @date Aug 5, 2014
//    9  * @author Andrey Belomutskiy, (c) 2012-2015
//   10  */
//   11 
//   12 #include "fsio_impl.h"
//   13 #include "mitsubishi.h"
//   14 #include "allsensors.h"
//   15 
//   16 EXTERN_ENGINE;
//   17 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z26setMitsubishiConfigurationv
        THUMB
//   18 void setMitsubishiConfiguration(DECLARE_ENGINE_PARAMETER_F) {
_Z26setMitsubishiConfigurationv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   19 	engineConfiguration->engineType = MITSU_4G93;
        LDR.N    R0,??setMitsubishiConfiguration_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+16
        STR      R1,[R0, #+0]
//   20 
//   21 	engineConfiguration->trigger.type = TT_MITSU;
        LDR.N    R0,??setMitsubishiConfiguration_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+11
        STR      R1,[R0, #+536]
//   22 
//   23 	engineConfiguration->specs.cylindersCount = 4;
        LDR.N    R0,??setMitsubishiConfiguration_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+4
        STR      R1,[R0, #+404]
//   24 	engineConfiguration->specs.displacement = 1.800;
        LDR.N    R0,??setMitsubishiConfiguration_0
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setMitsubishiConfiguration_0+0x4  ;; 0x3fe66666
        STR      R1,[R0, #+400]
//   25 
//   26 	// set_ignition_mode 2
//   27 	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
        LDR.N    R0,??setMitsubishiConfiguration_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+444]
//   28 	engineConfiguration->specs.firingOrder = FO_1_THEN_3_THEN_4_THEN2;
        LDR.N    R0,??setMitsubishiConfiguration_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+408]
//   29 
//   30 	// set_global_trigger_offset_angle 671
//   31 	engineConfiguration->globalTriggerAngleOffset = 671;
        LDR.N    R0,??setMitsubishiConfiguration_0
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setMitsubishiConfiguration_0+0x8  ;; 0x4427c000
        STR      R1,[R0, #+460]
//   32 
//   33 	// set_cranking_rpm 550
//   34 	engineConfiguration->cranking.rpm = 550;
        LDR.N    R0,??setMitsubishiConfiguration_0
        LDR      R0,[R0, #+0]
        MOVW     R1,#+550
        STRH     R1,[R0, #+96]
//   35 	// set_cranking_charge_angle 70
//   36 	engineConfiguration->crankingChargeAngle = 70;
        LDR.N    R0,??setMitsubishiConfiguration_0
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setMitsubishiConfiguration_0+0xC  ;; 0x428c0000
        STR      R1,[R0, #+108]
//   37 	// set_cranking_timing_angle 715
//   38 	engineConfiguration->crankingTimingAngle = -715;
        LDR.N    R0,??setMitsubishiConfiguration_0
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setMitsubishiConfiguration_0+0x10  ;; 0xc432c000
        STR      R1,[R0, #+440]
//   39 
//   40 	// set_whole_fuel_map 3
//   41 	setWholeFuelMap(3 PASS_ENGINE_PARAMETER);
        VMOV.F32 S0,#3.0
          CFI FunCall _Z15setWholeFuelMapf
        BL       _Z15setWholeFuelMapf
//   42 
//   43 	// set_cranking_fuel 4
//   44 	engineConfiguration->cranking.baseFuel = 4;
        LDR.N    R0,??setMitsubishiConfiguration_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1082130432
        STR      R1,[R0, #+92]
//   45 
//   46 //	/**
//   47 //	 * 29150 Ohm @ 40C
//   48 //	 * 10160 Ohm @ 70C
//   49 //	 * 1270 Ohm @ 150C
//   50 //	 */
//   51 //	setThermistorConfiguration(&engineConfiguration->clt, 40, 29150, 70, 10160, 150, 1270);
//   52 
//   53 	setCommonNTCSensor(&engineConfiguration->clt);
        LDR.N    R0,??setMitsubishiConfiguration_0
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+256
          CFI FunCall _Z18setCommonNTCSensorP14ThermistorConf
        BL       _Z18setCommonNTCSensorP14ThermistorConf
//   54 
//   55 
//   56 	engineConfiguration->clt.config.bias_resistor = 2700;
        LDR.N    R0,??setMitsubishiConfiguration_0
        LDR      R0,[R0, #+0]
        LDR.N    R1,??setMitsubishiConfiguration_0+0x14  ;; 0x4528c000
        STR      R1,[R0, #+280]
//   57 
//   58 	// Frankenstein: low side - out #1: PC14
//   59 	// Frankenstein: low side - out #2: PC15
//   60 	// Frankenstein: low side - out #3: PE6
//   61 	// Frankenstein: low side - out #4: PC13
//   62 	// Frankenstein: low side - out #5: PE4
//   63 	// Frankenstein: low side - out #6: PE5
//   64 	// Frankenstein: low side - out #7: PE2
//   65 	// Frankenstein: low side - out #8: PE3
//   66 	// Frankenstein: low side - out #9: PE0
//   67 	// Frankenstein: low side - out #10: PE1
//   68 	// Frankenstein: low side - out #11: PB8
//   69 	// Frankenstein: low side - out #12: PB9
//   70 
//   71 	boardConfiguration->injectionPins[0] = GPIOB_9; // Frankenstein: low side - out #12
        LDR.N    R0,??setMitsubishiConfiguration_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+25
        STR      R1,[R0, #+32]
//   72 	boardConfiguration->injectionPins[1] = GPIOB_8; // Frankenstein: low side - out #11
        LDR.N    R0,??setMitsubishiConfiguration_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+24
        STR      R1,[R0, #+36]
//   73 	boardConfiguration->injectionPins[2] = GPIOE_3; // Frankenstein: low side - out #8
        LDR.N    R0,??setMitsubishiConfiguration_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+67
        STR      R1,[R0, #+40]
//   74 	boardConfiguration->injectionPins[3] = GPIOE_5; // Frankenstein: low side - out #6
        LDR.N    R0,??setMitsubishiConfiguration_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+69
        STR      R1,[R0, #+44]
//   75 
//   76 
//   77 	// Frankenstein: high side #1: PE8
//   78 	// Frankenstein: high side #2: PE10
//   79 
//   80 	boardConfiguration->ignitionPins[0] = GPIOE_8; // Frankenstein: high side #1
        LDR.N    R0,??setMitsubishiConfiguration_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+72
        STR      R1,[R0, #+84]
//   81 	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
        LDR.N    R0,??setMitsubishiConfiguration_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+88]
//   82 	boardConfiguration->ignitionPins[2] = GPIOE_10; // // Frankenstein: high side #2
        LDR.N    R0,??setMitsubishiConfiguration_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+74
        STR      R1,[R0, #+92]
//   83 	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
        LDR.N    R0,??setMitsubishiConfiguration_0+0x18
        LDR      R0,[R0, #+0]
        MOVS     R1,#+80
        STR      R1,[R0, #+96]
//   84 
//   85 	engineConfiguration->HD44780width = 20;
        LDR.N    R0,??setMitsubishiConfiguration_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+20
        STR      R1,[R0, #+516]
//   86 	engineConfiguration->HD44780height = 4;
        LDR.N    R0,??setMitsubishiConfiguration_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+4
        STR      R1,[R0, #+520]
//   87 
//   88 	setEgoSensor(ES_Innovate_MTX_L PASS_ENGINE_PARAMETER);
        MOVS     R0,#+1
          CFI FunCall _Z12setEgoSensor12ego_sensor_e
        BL       _Z12setEgoSensor12ego_sensor_e
//   89 #if EFI_FSIO || defined(__DOXYGEN__)
//   90 	setFsio(0, GPIOD_11, "rpm 5500 >" PASS_ENGINE_PARAMETER);
        LDR.N    R2,??setMitsubishiConfiguration_0+0x1C
        MOVS     R1,#+59
        MOVS     R0,#+0
          CFI FunCall _Z7setFsioi11brain_pin_ePKc
        BL       _Z7setFsioi11brain_pin_ePKc
//   91 #endif
//   92 }
        POP      {R0,PC}          ;; return
        Nop      
        DATA
??setMitsubishiConfiguration_0:
        DC32     engineConfiguration
        DC32     0x3fe66666
        DC32     0x4427c000
        DC32     0x428c0000
        DC32     0xc432c000
        DC32     0x4528c000
        DC32     boardConfiguration
        DC32     `?<Constant "rpm 5500 >">`
          CFI EndBlock cfiBlock0

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "rpm 5500 >">`:
        DATA
        DC8 "rpm 5500 >"
        DC8 0

        END
//   93 
//   94 
// 
//  12 bytes in section .rodata
// 276 bytes in section .text
// 
// 276 bytes of CODE  memory
//  12 bytes of CONST memory
//
//Errors: none
//Warnings: 1
