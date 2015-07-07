///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:43 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       ford_fiesta.cpp                                      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\config\engines\ /
//                       ford_fiesta.cpp -lCN F:\stuff\rusefi_sourceforge\fir /
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
//                       ford_fiesta.s                                        /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME ford_fiesta

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z16setOperationModeP22engine_configuration_s16operation_mode_e
        EXTERN boardConfiguration
        EXTERN engineConfiguration

        PUBLIC _Z39setFordFiestaDefaultEngineConfigurationv
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\config\engines\ford_fiesta.cpp
//    1 /**
//    2  * @file	ford_fiesta.cpp
//    3  * @brief	European 1990 Ford Fiesta
//    4  *
//    5  * FORD_FIESTA = 4
//    6  * set_engine_type 4
//    7  *
//    8  * @date Nov 22, 2013
//    9  * @author Andrey Belomutskiy, (c) 2012-2015
//   10  */
//   11 
//   12 #include "main.h"
//   13 
//   14 #if EFI_SUPPORT_FORD_FIESTA || defined(__DOXYGEN__)
//   15 
//   16 #include "ford_fiesta.h"
//   17 #include "engine_math.h"
//   18 
//   19 EXTERN_ENGINE;
//   20 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z39setFordFiestaDefaultEngineConfigurationv
        THUMB
//   21 void setFordFiestaDefaultEngineConfiguration(DECLARE_ENGINE_PARAMETER_F) {
_Z39setFordFiestaDefaultEngineConfigurationv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   22 	engineConfiguration->rpmHardLimit = 7000;
        LDR.N    R0,??setFordFiestaDefaultEngineConfiguration_0
        LDR      R0,[R0, #+0]
        MOVW     R1,#+7000
        STR      R1,[R0, #+420]
//   23 	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
        MOVS     R1,#+1
        LDR.N    R0,??setFordFiestaDefaultEngineConfiguration_0
        LDR      R0,[R0, #+0]
          CFI FunCall _Z16setOperationModeP22engine_configuration_s16operation_mode_e
        BL       _Z16setOperationModeP22engine_configuration_s16operation_mode_e
//   24 
//   25 	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_36_1;
        LDR.N    R0,??setFordFiestaDefaultEngineConfiguration_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+9
        STR      R1,[R0, #+536]
//   26 
//   27 	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
        LDR.N    R0,??setFordFiestaDefaultEngineConfiguration_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+2
        STR      R1,[R0, #+444]
//   28 	engineConfiguration->specs.firingOrder = FO_1_THEN_3_THEN_4_THEN2;
        LDR.N    R0,??setFordFiestaDefaultEngineConfiguration_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+408]
//   29 	engineConfiguration->hasMafSensor = true;
        LDR.N    R0,??setFordFiestaDefaultEngineConfiguration_0
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1497]
        ORRS     R0,R0,#0x40
        LDR.N    R1,??setFordFiestaDefaultEngineConfiguration_0
        LDR      R1,[R1, #+0]
        STRB     R0,[R1, #+1497]
//   30 	engineConfiguration->mafAdcChannel = EFI_ADC_NONE;
        LDR.N    R0,??setFordFiestaDefaultEngineConfiguration_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+16
        STR      R1,[R0, #+580]
//   31 
//   32 
//   33 	// etb testing
//   34 //	boardConfiguration->clutchUpPin
//   35 	boardConfiguration->etbControlPin1 = GPIOD_3;
        LDR.N    R0,??setFordFiestaDefaultEngineConfiguration_0+0x4
        LDR      R0,[R0, #+0]
        MOVS     R1,#+51
        STR      R1,[R0, #+240]
//   36 	engineConfiguration->pedalPositionChannel = EFI_ADC_1;
        LDR.N    R0,??setFordFiestaDefaultEngineConfiguration_0
        LDR      R0,[R0, #+0]
        MOVS     R1,#+1
        STR      R1,[R0, #+604]
//   37 
//   38 	engineConfiguration->tpsMin = 337;
        LDR.N    R0,??setFordFiestaDefaultEngineConfiguration_0
        LDR      R0,[R0, #+0]
        MOVW     R1,#+337
        STRH     R1,[R0, #+82]
//   39 	engineConfiguration->tpsMax = 896;
        LDR.N    R0,??setFordFiestaDefaultEngineConfiguration_0
        LDR      R0,[R0, #+0]
        MOV      R1,#+896
        STRH     R1,[R0, #+84]
//   40 }
        POP      {R0,PC}          ;; return
        Nop      
        DATA
??setFordFiestaDefaultEngineConfiguration_0:
        DC32     engineConfiguration
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
//   41 
//   42 #endif /* EFI_SUPPORT_FORD_FIESTA */
// 
// 140 bytes in section .text
// 
// 140 bytes of CODE memory
//
//Errors: none
//Warnings: 1
