///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:52 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\lcd /
//                       _controller.cpp                                      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\lcd /
//                       _controller.cpp -lCN F:\stuff\rusefi_sourceforge\fir /
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
//                       lcd_controller.s                                     /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME lcd_controller

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z10getRealMafv
        EXTERN _Z10stopEnginev
        EXTERN _Z12hasMafSensorv
        EXTERN _Z13fuelPumpBenchv
        EXTERN _Z13hasBaroSensorv
        EXTERN _Z15getBaroPressurev
        EXTERN _Z18getIgnition_mode_e15ignition_mode_e
        EXTERN _Z19getInjection_mode_e16injection_mode_e
        EXTERN _Z20getConfigurationName13engine_type_e
        EXTERN _Z21getCoolantTemperaturev
        EXTERN _Z21getEngine_load_mode_e18engine_load_mode_e
        EXTERN _Z23getIntakeAirTemperaturev
        EXTERN _Z6getAfrv
        EXTERN _Z6getMafv
        EXTERN _Z6getMapv
        EXTERN _Z6getTPSv
        EXTERN _Z8fanBenchv
        EXTERN _Z8getVBattv
        EXTERN _ZN13RpmCalculator3rpmEv
        EXTERN _ZN8MenuItemC1EPS_10lcd_line_e
        EXTERN _ZN8MenuItemC1EPS_PKc
        EXTERN _ZN8MenuItemC1EPS_PKcPFvvE
        EXTERN _ZN8MenuTree12enterSubMenuEv
        EXTERN _ZN8MenuTree4backEv
        EXTERN _ZN8MenuTree4initEP8MenuItemi
        EXTERN _ZN8MenuTree8nextItemEv
        EXTERN _ZN8MenuTreeC1EP8MenuItem
        EXTERN __aeabi_f2d
        EXTERN __aeabi_memcpy
        EXTERN chvprintf
        EXTERN efiStrlen
        EXTERN engine
        EXTERN engineConfiguration
        EXTERN getCurrentHD44780column
        EXTERN getTimeNowSeconds
        EXTERN getWarninig
        EXTERN lcd_HD44780_print_char
        EXTERN lcd_HD44780_print_string
        EXTERN lcd_HD44780_set_position
        EXTERN msObjectInit

        PUBLIC _Z10onJoystick17joystick_button_e
        PUBLIC _Z16updateHD44780lcdP6Engine
        PUBLIC _Z17initLcdControllerv
        PUBLIC _Z9appendStrPcPKc
        PUBLIC memcpy
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\lcd_controller.cpp
//    1 /**
//    2  * @file lcd_controller.cpp
//    3  *
//    4  * @date Aug 14, 2014
//    5  * @author Andrey Belomutskiy, (c) 2012-2015
//    6  */
//    7 
//    8 #include "main.h"
//    9 #include "lcd_controller.h"
//   10 #include "lcd_HD44780.h"
//   11 #include "efilib.h"
//   12 #include "rpm_calculator.h"
//   13 #include "allsensors.h"
//   14 #include "engine.h"
//   15 #include "rtc_helper.h"
//   16 #include "io_pins.h"
//   17 #include "efiGpio.h"
//   18 #include "svnversion.h"
//   19 #include "joystick.h"
//   20 #include "utlist.h"
//   21 #include "lcd_menu_tree.h"
//   22 #include "memstreams.h"
//   23 #include "settings.h"
//   24 #include "injector_central.h"
//   25 
//   26 #if EFI_HD44780_LCD || defined(__DOXYGEN__)
//   27 
//   28 EXTERN_ENGINE
//   29 ;
//   30 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   31 static MenuItem ROOT(NULL, NULL);
        MOVS     R2,#+0
        MOVS     R1,#+0
        LDR.W    R0,??DataTable5_1
          CFI FunCall _ZN8MenuItemC1EPS_PKc
        BL       _ZN8MenuItemC1EPS_PKc
        MOVS     R1,R0
//   32 
//   33 static MenuTree tree(&ROOT);
        LDR.W    R0,??DataTable5_2
          CFI FunCall _ZN8MenuTreeC1EP8MenuItem
        BL       _ZN8MenuTreeC1EP8MenuItem
//   34 
//   35 static MenuItem miRpm(tree.root, LL_RPM);
        MOVS     R2,#+7
        LDR.W    R0,??DataTable5_2
        LDR      R1,[R0, #+0]
        LDR.W    R0,??DataTable5_3
          CFI FunCall _ZN8MenuItemC1EPS_10lcd_line_e
        BL       _ZN8MenuItemC1EPS_10lcd_line_e
//   36 static MenuItem miSensors(tree.root, "sensors");
        LDR.W    R2,??DataTable5_4
        LDR.W    R0,??DataTable5_2
        LDR      R1,[R0, #+0]
        LDR.W    R0,??DataTable5_5
          CFI FunCall _ZN8MenuItemC1EPS_PKc
        BL       _ZN8MenuItemC1EPS_PKc
//   37 static MenuItem miBench(tree.root, "bench test");
        LDR.W    R2,??DataTable5_6
        LDR.W    R0,??DataTable5_2
        LDR      R1,[R0, #+0]
        LDR.W    R0,??DataTable5_7
          CFI FunCall _ZN8MenuItemC1EPS_PKc
        BL       _ZN8MenuItemC1EPS_PKc
//   38 static MenuItem miAbout(tree.root, "about");
        LDR.W    R2,??DataTable5_8
        LDR.W    R0,??DataTable5_2
        LDR      R1,[R0, #+0]
        LDR.W    R0,??DataTable5_9
          CFI FunCall _ZN8MenuItemC1EPS_PKc
        BL       _ZN8MenuItemC1EPS_PKc
//   39 
//   40 static MenuItem miTriggerErrors(&miRpm, LL_TRIGGER_ERRORS);
        MOVS     R2,#+8
        LDR.W    R1,??DataTable5_3
        LDR.W    R0,??DataTable5_10
          CFI FunCall _ZN8MenuItemC1EPS_10lcd_line_e
        BL       _ZN8MenuItemC1EPS_10lcd_line_e
//   41 static MenuItem miTriggerDuty(&miRpm, LL_TRIGGER_DUTY);
        MOVS     R2,#+9
        LDR.W    R1,??DataTable5_3
        LDR.W    R0,??DataTable5_11
          CFI FunCall _ZN8MenuItemC1EPS_10lcd_line_e
        BL       _ZN8MenuItemC1EPS_10lcd_line_e
//   42 
//   43 static MenuItem miClt(&miSensors, LL_CLT_TEMPERATURE);
        MOVS     R2,#+10
        LDR.W    R1,??DataTable5_5
        LDR.W    R0,??DataTable5_12
          CFI FunCall _ZN8MenuItemC1EPS_10lcd_line_e
        BL       _ZN8MenuItemC1EPS_10lcd_line_e
//   44 static MenuItem miIat(&miSensors, LL_IAT_TEMPERATURE);
        MOVS     R2,#+11
        LDR.W    R1,??DataTable5_5
        LDR.W    R0,??DataTable5_13
          CFI FunCall _ZN8MenuItemC1EPS_10lcd_line_e
        BL       _ZN8MenuItemC1EPS_10lcd_line_e
//   45 static MenuItem miTps(&miSensors, LL_TPS);
        MOVS     R2,#+13
        LDR.W    R1,??DataTable5_5
        LDR.W    R0,??DataTable5_14
          CFI FunCall _ZN8MenuItemC1EPS_10lcd_line_e
        BL       _ZN8MenuItemC1EPS_10lcd_line_e
//   46 static MenuItem miVBatt(&miSensors, LL_VBATT);
        MOVS     R2,#+14
        LDR.W    R1,??DataTable5_5
        LDR.W    R0,??DataTable5_15
          CFI FunCall _ZN8MenuItemC1EPS_10lcd_line_e
        BL       _ZN8MenuItemC1EPS_10lcd_line_e
//   47 static MenuItem miMap(&miSensors, LL_MAP);
        MOVS     R2,#+16
        LDR.W    R1,??DataTable5_5
        LDR.W    R0,??DataTable5_16
          CFI FunCall _ZN8MenuItemC1EPS_10lcd_line_e
        BL       _ZN8MenuItemC1EPS_10lcd_line_e
//   48 static MenuItem miAfr(&miSensors, LL_AFR);
        MOVS     R2,#+12
        LDR.W    R1,??DataTable5_5
        LDR.W    R0,??DataTable5_17
          CFI FunCall _ZN8MenuItemC1EPS_10lcd_line_e
        BL       _ZN8MenuItemC1EPS_10lcd_line_e
//   49 static MenuItem miBaro(&miSensors, LL_BARO);
        MOVS     R2,#+20
        LDR.W    R1,??DataTable5_5
        LDR.W    R0,??DataTable5_18
          CFI FunCall _ZN8MenuItemC1EPS_10lcd_line_e
        BL       _ZN8MenuItemC1EPS_10lcd_line_e
//   50 static MenuItem miMapV(&miSensors, LL_MAF_V);
        MOVS     R2,#+17
        LDR.W    R1,??DataTable5_5
        LDR.W    R0,??DataTable5_19
          CFI FunCall _ZN8MenuItemC1EPS_10lcd_line_e
        BL       _ZN8MenuItemC1EPS_10lcd_line_e
//   51 static MenuItem miMapKgHr(&miSensors, LL_MAF_KG_HR);
        MOVS     R2,#+18
        LDR.W    R1,??DataTable5_5
        LDR.W    R0,??DataTable5_20
          CFI FunCall _ZN8MenuItemC1EPS_10lcd_line_e
        BL       _ZN8MenuItemC1EPS_10lcd_line_e
//   52 
//   53 static MenuItem miStopEngine(&miBench, "stop engine", stopEngine);
        LDR.W    R3,??DataTable5_21
        LDR.W    R2,??DataTable5_22
        LDR.W    R1,??DataTable5_7
        LDR.W    R0,??DataTable5_23
          CFI FunCall _ZN8MenuItemC1EPS_PKcPFvvE
        BL       _ZN8MenuItemC1EPS_PKcPFvvE
//   54 static MenuItem miTestFan(&miBench, "test fan", fanBench);
        LDR.W    R3,??DataTable5_24
        LDR.W    R2,??DataTable5_25
        LDR.W    R1,??DataTable5_7
        LDR.W    R0,??DataTable5_26
          CFI FunCall _ZN8MenuItemC1EPS_PKcPFvvE
        BL       _ZN8MenuItemC1EPS_PKcPFvvE
//   55 static MenuItem miTestFuelPump(&miBench, "test pump", fuelPumpBench);
        LDR.W    R3,??DataTable5_27
        LDR.W    R2,??DataTable5_28
        LDR.W    R1,??DataTable5_7
        LDR.W    R0,??DataTable5_29
          CFI FunCall _ZN8MenuItemC1EPS_PKcPFvvE
        BL       _ZN8MenuItemC1EPS_PKcPFvvE
//   56 static MenuItem miTestSpark1(&miBench, "test spark1");
        LDR.W    R2,??DataTable5_30
        LDR.W    R1,??DataTable5_7
        LDR.W    R0,??DataTable5_31
          CFI FunCall _ZN8MenuItemC1EPS_PKc
        BL       _ZN8MenuItemC1EPS_PKc
//   57 static MenuItem miTestSpark2(&miBench, "test spark2");
        LDR.W    R2,??DataTable5_32
        LDR.W    R1,??DataTable5_7
        LDR.W    R0,??DataTable5_33
          CFI FunCall _ZN8MenuItemC1EPS_PKc
        BL       _ZN8MenuItemC1EPS_PKc
//   58 static MenuItem miTestSpark3(&miBench, "test spark3");
        LDR.W    R2,??DataTable5_34
        LDR.W    R1,??DataTable5_7
        LDR.W    R0,??DataTable5_35
          CFI FunCall _ZN8MenuItemC1EPS_PKc
        BL       _ZN8MenuItemC1EPS_PKc
//   59 static MenuItem miTestSpark4(&miBench, "test spark4");
        LDR.W    R2,??DataTable5_36
        LDR.W    R1,??DataTable5_7
        LDR.W    R0,??DataTable5_37
          CFI FunCall _ZN8MenuItemC1EPS_PKc
        BL       _ZN8MenuItemC1EPS_PKc
//   60 static MenuItem miTestInj1(&miBench, "test injector1");
        LDR.W    R2,??DataTable5_38
        LDR.W    R1,??DataTable5_7
        LDR.W    R0,??DataTable5_39
          CFI FunCall _ZN8MenuItemC1EPS_PKc
        BL       _ZN8MenuItemC1EPS_PKc
//   61 static MenuItem miTestInj2(&miBench, "test injector2");
        LDR.W    R2,??DataTable5_40
        LDR.W    R1,??DataTable5_7
        LDR.W    R0,??DataTable5_41
          CFI FunCall _ZN8MenuItemC1EPS_PKc
        BL       _ZN8MenuItemC1EPS_PKc
//   62 static MenuItem miTestInj3(&miBench, "test injector3");
        LDR.W    R2,??DataTable5_42
        LDR.W    R1,??DataTable5_7
        LDR.W    R0,??DataTable5_43
          CFI FunCall _ZN8MenuItemC1EPS_PKc
        BL       _ZN8MenuItemC1EPS_PKc
//   63 static MenuItem miTestInj4(&miBench, "test injector4");
        LDR.W    R2,??DataTable5_44
        LDR.W    R1,??DataTable5_7
        LDR.W    R0,??DataTable5_45
          CFI FunCall _ZN8MenuItemC1EPS_PKc
        BL       _ZN8MenuItemC1EPS_PKc
//   64 
//   65 static MenuItem miVersion(&miAbout, LL_VERSION);
        MOVS     R2,#+1
        LDR.W    R1,??DataTable5_9
        LDR.W    R0,??DataTable5_46
          CFI FunCall _ZN8MenuItemC1EPS_10lcd_line_e
        BL       _ZN8MenuItemC1EPS_10lcd_line_e
//   66 static MenuItem miConfig(&miAbout, LL_CONFIG);
        MOVS     R2,#+2
        LDR.W    R1,??DataTable5_9
        LDR.W    R0,??DataTable5_47
          CFI FunCall _ZN8MenuItemC1EPS_10lcd_line_e
        BL       _ZN8MenuItemC1EPS_10lcd_line_e
//   67 static MenuItem miAlgo(&miAbout, LL_ALGORITHM);
        MOVS     R2,#+3
        LDR.W    R1,??DataTable5_9
        LDR.W    R0,??DataTable5_48
          CFI FunCall _ZN8MenuItemC1EPS_10lcd_line_e
        BL       _ZN8MenuItemC1EPS_10lcd_line_e
//   68 static MenuItem miInjection(&miAbout, LL_INJECTION);
        MOVS     R2,#+4
        LDR.W    R1,??DataTable5_9
        LDR.W    R0,??DataTable5_49
          CFI FunCall _ZN8MenuItemC1EPS_10lcd_line_e
        BL       _ZN8MenuItemC1EPS_10lcd_line_e
//   69 static MenuItem miIgnition(&miAbout, LL_IGNITION);
        MOVS     R2,#+5
        LDR.W    R1,??DataTable5_9
        LDR.W    R0,??DataTable5_50
          CFI FunCall _ZN8MenuItemC1EPS_10lcd_line_e
        BL       _ZN8MenuItemC1EPS_10lcd_line_e
//   70 static MenuItem miInjFlow(&miAbout, LL_ING_FLOW);
        MOVS     R2,#+6
        LDR.W    R1,??DataTable5_9
        LDR.W    R0,??DataTable5_51
          CFI FunCall _ZN8MenuItemC1EPS_10lcd_line_e
        BL       _ZN8MenuItemC1EPS_10lcd_line_e
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
ROOT:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
tree:
        DS8 16

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miRpm:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miSensors:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miBench:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miAbout:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miTriggerErrors:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miTriggerDuty:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miClt:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miIat:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miTps:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miVBatt:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miMap:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miAfr:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miBaro:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miMapV:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miMapKgHr:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miStopEngine:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miTestFan:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miTestFuelPump:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miTestSpark1:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miTestSpark2:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miTestSpark3:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miTestSpark4:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miTestInj1:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miTestInj2:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miTestInj3:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miTestInj4:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miVersion:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miConfig:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miAlgo:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miInjection:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miIgnition:
        DS8 36

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
miInjFlow:
        DS8 36
//   71 
//   72 #define DISP_LINES (engineConfiguration->HD44780height - 1)
//   73 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   74 static char lcdLineBuffer[30];
lcdLineBuffer:
        DS8 32

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   75 static MemoryStream lcdLineStream;
lcdLineStream:
        DS8 20
//   76 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z10onJoystick17joystick_button_e
        THUMB
//   77 void onJoystick(joystick_button_e button) {
_Z10onJoystick17joystick_button_e:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   78 	/**
//   79 	 * this method is invoked on EXTI IRQ thread
//   80 	 */
//   81 	if (button == JB_CENTER) {
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+0
        BNE.N    ??onJoystick_0
//   82 		tree.enterSubMenu();
        LDR.W    R0,??DataTable5_2
          CFI FunCall _ZN8MenuTree12enterSubMenuEv
        BL       _ZN8MenuTree12enterSubMenuEv
        B.N      ??onJoystick_1
//   83 	} else if (button == JB_BUTTON_D) {
??onJoystick_0:
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+4
        BNE.N    ??onJoystick_2
//   84 		tree.nextItem();
        LDR.W    R0,??DataTable5_2
          CFI FunCall _ZN8MenuTree8nextItemEv
        BL       _ZN8MenuTree8nextItemEv
        B.N      ??onJoystick_1
//   85 	} else if (button == JB_BUTTON_A) {
??onJoystick_2:
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+1
        BNE.N    ??onJoystick_1
//   86 		tree.back();
        LDR.W    R0,??DataTable5_2
          CFI FunCall _ZN8MenuTree4backEv
        BL       _ZN8MenuTree4backEv
//   87 	}
//   88 	// actual repaint happends in the repaint loop
//   89 }
??onJoystick_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1
//   90 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z9appendStrPcPKc
        THUMB
//   91 char * appendStr(char *ptr, const char *suffix) {
_Z9appendStrPcPKc:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   92 	for (uint32_t i = 0; i < efiStrlen(suffix); i++) {
        MOVS     R6,#+0
??appendStr_0:
        MOVS     R0,R5
          CFI FunCall efiStrlen
        BL       efiStrlen
        CMP      R6,R0
        BCS.N    ??appendStr_1
//   93 		*ptr++ = suffix[i];
        LDRB     R0,[R6, R5]
        STRB     R0,[R4, #+0]
        ADDS     R4,R4,#+1
//   94 	}
        ADDS     R6,R6,#+1
        B.N      ??appendStr_0
//   95 	return ptr;
??appendStr_1:
        MOVS     R0,R4
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock2
//   96 }
//   97 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z17initLcdControllerv
        THUMB
//   98 void initLcdController(void) {
_Z17initLcdControllerv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   99 	tree.init(&miRpm, engineConfiguration->HD44780height - 1);
        LDR.W    R0,??DataTable5_52
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+520]
        SUBS     R2,R0,#+1
        LDR.W    R1,??DataTable5_3
        LDR.W    R0,??DataTable5_2
          CFI FunCall _ZN8MenuTree4initEP8MenuItemi
        BL       _ZN8MenuTree4initEP8MenuItemi
//  100 	msObjectInit(&lcdLineStream, (uint8_t *) lcdLineBuffer, sizeof(lcdLineBuffer), 0);
        MOVS     R3,#+0
        MOVS     R2,#+30
        LDR.W    R1,??DataTable5_53
        LDR.W    R0,??DataTable5_54
          CFI FunCall msObjectInit
        BL       msObjectInit
//  101 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock3
//  102 
//  103 static char * prepareVBattMapLine(engine_configuration_s *engineConfiguration, char *buffer) {
//  104 	char *ptr = buffer;
//  105 	*ptr++ = 'V';
//  106 	ptr = ftoa(ptr, getVBatt(PASS_ENGINE_PARAMETER_F), 10.0f);
//  107 
//  108 	ptr = appendStr(ptr, " M");
//  109 	ptr = ftoa(ptr, getRawMap(), 10.0f);
//  110 	return ptr;
//  111 }
//  112 
//  113 static char * prepareCltIatTpsLine(Engine *engine, char *buffer) {
//  114 	char *ptr = buffer;
//  115 	*ptr++ = 'C';
//  116 
//  117 	ptr = appendStr(ptr, " TP");
//  118 	ptr = itoa10(ptr, (int) getTPS(PASS_ENGINE_PARAMETER_F));
//  119 	return ptr;
//  120 }
//  121 
//  122 static const char* ignitionModeStr[] = { "1C", "IND", "WS" };
//  123 static const char* injectionModeStr[] = { "Sim", "Seq", "Bch" };
//  124 static const char* idleModeStr[] = { "I:A", "I:M" };
//  125 
//  126 //static const char *getPinShortName(io_pin_e pin) {
//  127 //	switch (pin) {
//  128 //	case ALTERNATOR_SWITCH:
//  129 //		return "AL";
//  130 //	case FUEL_PUMP_RELAY:
//  131 //		return "FP";
//  132 //	case FAN_RELAY:
//  133 //		return "FN";
//  134 //	case O2_HEATER:
//  135 //		return "O2H";
//  136 //	default:
//  137 //		firmwareError("No short name for %d", (int) pin);
//  138 //		return "";
//  139 //	}
//  140 //}
//  141 
//  142 //char * appendPinStatus(char *buffer, io_pin_e pin) {
//  143 //	char *ptr = appendStr(buffer, getPinShortName(pin));
//  144 //	int state = getOutputPinValue(pin);
//  145 //	// todo: should we handle INITIAL_PIN_STATE?
//  146 //	if (state) {
//  147 //		return appendStr(ptr, ":Y ");
//  148 //	} else {
//  149 //		return appendStr(ptr, ":n ");
//  150 //	}
//  151 //}
//  152 
//  153 static char * prepareInfoLine(engine_configuration_s *engineConfiguration, char *buffer) {
//  154 	char *ptr = buffer;
//  155 
//  156 	ptr = appendStr(ptr, " ");
//  157 	ptr = appendStr(ptr, ignitionModeStr[engineConfiguration->ignitionMode]);
//  158 
//  159 	ptr = appendStr(ptr, " ");
//  160 	ptr = appendStr(ptr, injectionModeStr[engineConfiguration->injectionMode]);
//  161 
//  162 	ptr = appendStr(ptr, " ");
//  163 	ptr = appendStr(ptr, idleModeStr[engineConfiguration->idleMode]);
//  164 
//  165 	ptr = appendStr(ptr, " ");
//  166 	return ptr;
//  167 }
//  168 
//  169 //static char * prepareStatusLine(char *buffer) {
//  170 //	char *ptr = buffer;
//  171 //
//  172 //	ptr = appendPinStatus(ptr, FUEL_PUMP_RELAY);
//  173 //	ptr = appendPinStatus(ptr, FAN_RELAY);
//  174 //	ptr = appendPinStatus(ptr, O2_HEATER);
//  175 //	return ptr;
//  176 //}
//  177 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  178 static char buffer[MAX_LCD_WIDTH + 4];
buffer:
        DS8 24
//  179 static char dateBuffer[30];
//  180 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z9lcdPrintfPKcz
        THUMB
//  181 static void lcdPrintf(const char *fmt, ...) {
_Z9lcdPrintfPKcz:
        PUSH     {R1-R3}
          CFI CFA R13+12
        PUSH     {R4,R5,LR}
          CFI R14 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
//  182 	va_list ap;
//  183 	va_start(ap, fmt);
        ADD      R0,SP,#+12
        MOVS     R5,R0
//  184 	lcdLineStream.eos = 0; // reset
        LDR.W    R0,??DataTable5_54
        MOVS     R1,#+0
        STR      R1,[R0, #+12]
//  185 	chvprintf((BaseSequentialStream *) &lcdLineStream, fmt, ap);
        MOVS     R2,R5
        MOVS     R1,R4
        LDR.W    R0,??DataTable5_54
          CFI FunCall chvprintf
        BL       chvprintf
//  186 	lcdLineStream.buffer[lcdLineStream.eos] = 0; // terminator
        LDR.W    R0,??DataTable5_54
        LDR      R0,[R0, #+12]
        LDR.W    R1,??DataTable5_54
        LDR      R1,[R1, #+4]
        MOVS     R2,#+0
        STRB     R2,[R0, R1]
//  187 	va_end(ap);
//  188 	lcd_HD44780_print_string(lcdLineBuffer);
        LDR.W    R0,??DataTable5_53
          CFI FunCall lcd_HD44780_print_string
        BL       lcd_HD44780_print_string
//  189 }
        POP      {R4,R5}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI CFA R13+16
        LDR      PC,[SP], #+16    ;; return
          CFI EndBlock cfiBlock4
//  190 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z8showLine10lcd_line_ei
        THUMB
//  191 static void showLine(lcd_line_e line, int screenY) {
_Z8showLine10lcd_line_ei:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  192 
//  193 	switch (line) {
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        MOVS     R0,R4
        CMP      R0,#+1
        BEQ.N    ??showLine_0
        CMP      R0,#+2
        BEQ.N    ??showLine_1
        CMP      R0,#+3
        BEQ.N    ??showLine_2
        CMP      R0,#+4
        BEQ.N    ??showLine_3
        CMP      R0,#+5
        BEQ.W    ??showLine_4
        CMP      R0,#+6
        BEQ.N    ??showLine_5
        CMP      R0,#+7
        BEQ.N    ??showLine_6
        CMP      R0,#+8
        BEQ.W    ??showLine_7
        CMP      R0,#+9
        BEQ.W    ??showLine_8
        CMP      R0,#+10
        BEQ.N    ??showLine_9
        CMP      R0,#+11
        BEQ.N    ??showLine_10
        CMP      R0,#+12
        BEQ.W    ??showLine_11
        CMP      R0,#+13
        BEQ.N    ??showLine_12
        CMP      R0,#+14
        BEQ.W    ??showLine_13
        CMP      R0,#+16
        BEQ.W    ??showLine_14
        CMP      R0,#+17
        BEQ.W    ??showLine_15
        CMP      R0,#+18
        BEQ.W    ??showLine_16
        CMP      R0,#+20
        BEQ.W    ??showLine_17
        B.N      ??showLine_18
//  194 	case LL_VERSION:
//  195 		lcdPrintf("version %s", VCS_VERSION);
??showLine_0:
        LDR.W    R1,??DataTable5_55
        LDR.W    R0,??DataTable5_56
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
//  196 		return;
        B.N      ??showLine_19
//  197 	case LL_CONFIG:
//  198 		lcdPrintf("config %s", getConfigurationName(engineConfiguration->engineType));
??showLine_1:
        LDR.W    R0,??DataTable5_52
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+0]
          CFI FunCall _Z20getConfigurationName13engine_type_e
        BL       _Z20getConfigurationName13engine_type_e
        MOVS     R1,R0
        LDR.W    R0,??DataTable5_57
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
//  199 		return;
        B.N      ??showLine_19
//  200 	case LL_RPM:
//  201 		lcdPrintf("RPM %d", getRpmE(engine));
??showLine_6:
        LDR.W    R0,??DataTable5_58
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN13RpmCalculator3rpmEv
        BL       _ZN13RpmCalculator3rpmEv
        MOVS     R1,R0
        LDR.W    R0,??DataTable5_59
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
//  202 		{
//  203 			int seconds = getTimeNowSeconds();
          CFI FunCall getTimeNowSeconds
        BL       getTimeNowSeconds
        MOVS     R6,R0
//  204 			if (seconds < 10000) {
        MOVW     R0,#+10000
        CMP      R6,R0
        BGE.N    ??showLine_20
//  205 				lcdPrintf("  %d", seconds);
        MOVS     R1,R6
        LDR.W    R0,??DataTable5_60
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
//  206 			}
//  207 		}
//  208 		return;
??showLine_20:
        B.N      ??showLine_19
//  209 	case LL_CLT_TEMPERATURE:
//  210 		lcdPrintf("Coolant %f", getCoolantTemperature(PASS_ENGINE_PARAMETER_F));
??showLine_9:
          CFI FunCall _Z21getCoolantTemperaturev
        BL       _Z21getCoolantTemperaturev
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R0,??DataTable5_61
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
//  211 		return;
        B.N      ??showLine_19
//  212 	case LL_IAT_TEMPERATURE:
//  213 		lcdPrintf("Intake Air %f", getIntakeAirTemperature(PASS_ENGINE_PARAMETER_F));
??showLine_10:
          CFI FunCall _Z23getIntakeAirTemperaturev
        BL       _Z23getIntakeAirTemperaturev
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R0,??DataTable5_62
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
//  214 		return;
        B.N      ??showLine_19
//  215 	case LL_ALGORITHM:
//  216 		lcdPrintf(getEngine_load_mode_e(engineConfiguration->algorithm));
??showLine_2:
        LDR.N    R0,??DataTable5_52
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+424]
          CFI FunCall _Z21getEngine_load_mode_e18engine_load_mode_e
        BL       _Z21getEngine_load_mode_e18engine_load_mode_e
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
//  217 		return;
        B.N      ??showLine_19
//  218 	case LL_INJECTION:
//  219 		lcdPrintf(getInjection_mode_e(engineConfiguration->injectionMode));
??showLine_3:
        LDR.N    R0,??DataTable5_52
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+432]
          CFI FunCall _Z19getInjection_mode_e16injection_mode_e
        BL       _Z19getInjection_mode_e16injection_mode_e
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
//  220 		return;
        B.N      ??showLine_19
//  221 	case LL_ING_FLOW:
//  222 		lcdPrintf("Inj %fcc", engineConfiguration->injector.flow);
??showLine_5:
        LDR.N    R0,??DataTable5_52
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+8]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R0,??DataTable5_63
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
//  223 		return;
        B.N      ??showLine_19
//  224 	case LL_IGNITION:
//  225 		lcdPrintf(getIgnition_mode_e(engineConfiguration->ignitionMode));
??showLine_4:
        LDR.N    R0,??DataTable5_52
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+444]
          CFI FunCall _Z18getIgnition_mode_e15ignition_mode_e
        BL       _Z18getIgnition_mode_e15ignition_mode_e
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
//  226 		return;
        B.N      ??showLine_19
//  227 	case LL_TPS:
//  228 		lcdPrintf("Throttle %f%%", getTPS());
??showLine_12:
          CFI FunCall _Z6getTPSv
        BL       _Z6getTPSv
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R0,??DataTable5_64
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
//  229 		return;
        B.N      ??showLine_19
//  230 	case LL_VBATT:
//  231 		lcdPrintf("Battery %fv", getVBatt(PASS_ENGINE_PARAMETER_F));
??showLine_13:
          CFI FunCall _Z8getVBattv
        BL       _Z8getVBattv
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R0,??DataTable5_65
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
//  232 		return;
        B.N      ??showLine_19
//  233 #if	EFI_ANALOG_SENSORS || defined(__DOXYGEN__)
//  234 	case LL_BARO:
//  235 		if (hasBaroSensor()) {
??showLine_17:
          CFI FunCall _Z13hasBaroSensorv
        BL       _Z13hasBaroSensorv
        CMP      R0,#+0
        BEQ.N    ??showLine_21
//  236 			lcdPrintf("Baro: %f", getBaroPressure());
          CFI FunCall _Z15getBaroPressurev
        BL       _Z15getBaroPressurev
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R0,??DataTable5_66
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
        B.N      ??showLine_22
//  237 		} else {
//  238 			lcdPrintf("Baro: none");
??showLine_21:
        LDR.N    R0,??DataTable5_67
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
//  239 		}
//  240 		return;
??showLine_22:
        B.N      ??showLine_19
//  241 #endif
//  242 	case LL_AFR:
//  243 		if (engineConfiguration->hasAfrSensor) {
??showLine_11:
        LDR.N    R0,??DataTable5_52
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+3
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??showLine_23
//  244 			lcdPrintf("AFR: %f", getAfr());
          CFI FunCall _Z6getAfrv
        BL       _Z6getAfrv
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R0,??DataTable5_68
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
        B.N      ??showLine_24
//  245 		} else {
//  246 			lcdPrintf("AFR: none");
??showLine_23:
        LDR.N    R0,??DataTable5_69
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
//  247 		}
//  248 		return;
??showLine_24:
        B.N      ??showLine_19
//  249 	case LL_MAP:
//  250 		if (engineConfiguration->hasMapSensor) {
??showLine_14:
        LDR.N    R0,??DataTable5_52
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1496]
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??showLine_25
//  251 			lcdPrintf("MAP %f", getMap());
          CFI FunCall _Z6getMapv
        BL       _Z6getMapv
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R0,??DataTable5_70
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
        B.N      ??showLine_26
//  252 		} else {
//  253 			lcdPrintf("MAP: none");
??showLine_25:
        LDR.N    R0,??DataTable5_71
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
//  254 		}
//  255 		return;
??showLine_26:
        B.N      ??showLine_19
//  256 	case LL_MAF_V:
//  257 		if (hasMafSensor()) {
??showLine_15:
          CFI FunCall _Z12hasMafSensorv
        BL       _Z12hasMafSensorv
        CMP      R0,#+0
        BEQ.N    ??showLine_27
//  258 			lcdPrintf("MAF: %fv", getMaf());
          CFI FunCall _Z6getMafv
        BL       _Z6getMafv
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R0,??DataTable5_72
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
        B.N      ??showLine_28
//  259 		} else {
//  260 			lcdPrintf("MAF: none");
??showLine_27:
        LDR.N    R0,??DataTable5_73
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
//  261 		}
//  262 		return;
??showLine_28:
        B.N      ??showLine_19
//  263 	case LL_MAF_KG_HR:
//  264 		if (hasMafSensor()) {
??showLine_16:
          CFI FunCall _Z12hasMafSensorv
        BL       _Z12hasMafSensorv
        CMP      R0,#+0
        BEQ.N    ??showLine_29
//  265 			lcdPrintf("MAF: %f kg/hr", getRealMaf());
          CFI FunCall _Z10getRealMafv
        BL       _Z10getRealMafv
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R0,??DataTable5_74
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
        B.N      ??showLine_30
//  266 		} else {
//  267 			lcdPrintf("MAF: none");
??showLine_29:
        LDR.N    R0,??DataTable5_73
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
//  268 		}
//  269 		return;
??showLine_30:
        B.N      ??showLine_19
//  270 	case LL_TRIGGER_ERRORS:
//  271 		lcdPrintf("Errors");
??showLine_7:
        LDR.N    R0,??DataTable5_75
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
//  272 		return;
        B.N      ??showLine_19
//  273 	case LL_TRIGGER_DUTY:
//  274 		lcdPrintf("Duty");
??showLine_8:
        LDR.N    R0,??DataTable5_76
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
//  275 		return;
        B.N      ??showLine_19
//  276 	default:
//  277 		lcdPrintf("()");
??showLine_18:
        ADR.N    R0,??DataTable5  ;; 0x28, 0x29, 0x00, 0x00
          CFI FunCall _Z9lcdPrintfPKcz
        BL       _Z9lcdPrintfPKcz
//  278 	}
//  279 }
??showLine_19:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock5
//  280 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z14fillWithSpacesv
        THUMB
//  281 static void fillWithSpaces(void) {
_Z14fillWithSpacesv:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
//  282 	int column = getCurrentHD44780column();
          CFI FunCall getCurrentHD44780column
        BL       getCurrentHD44780column
        MOVS     R4,R0
//  283 	for (int r = column; r < 20; r++) {
        MOVS     R5,R4
??fillWithSpaces_0:
        CMP      R5,#+20
        BGE.N    ??fillWithSpaces_1
//  284 		lcd_HD44780_print_char(' ');
        MOVS     R0,#+32
          CFI FunCall lcd_HD44780_print_char
        BL       lcd_HD44780_print_char
//  285 	}
        ADDS     R5,R5,#+1
        B.N      ??fillWithSpaces_0
//  286 }
??fillWithSpaces_1:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock6

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z16updateHD44780lcdP6Engine
        THUMB
_Z16updateHD44780lcdP6Engine:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        LDR.N    R0,??DataTable5_2
        LDR      R5,[R0, #+12]
        MOVS     R6,#+0
??updateHD44780lcd_0:
        LDR.N    R0,??DataTable5_2
        LDR      R0,[R0, #+4]
        CMP      R6,R0
        BGE.N    ??updateHD44780lcd_1
        CMP      R5,#+0
        BEQ.N    ??updateHD44780lcd_1
        MOVS     R1,#+0
        MOVS     R0,R6
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall lcd_HD44780_set_position
        BL       lcd_HD44780_set_position
        LDR.N    R0,??DataTable5_2
        LDR      R0,[R0, #+8]
        CMP      R5,R0
        BNE.N    ??updateHD44780lcd_2
        LDR      R0,[R5, #+32]
        CMP      R0,#+0
        BEQ.N    ??updateHD44780lcd_3
        MOVS     R0,#+33
        MOVS     R7,R0
        B.N      ??updateHD44780lcd_4
??updateHD44780lcd_3:
        LDR      R0,[R5, #+20]
        CMP      R0,#+0
        BNE.N    ??updateHD44780lcd_5
        MOVS     R7,#+42
        B.N      ??updateHD44780lcd_4
??updateHD44780lcd_5:
        MOVS     R7,#+62
        B.N      ??updateHD44780lcd_4
??updateHD44780lcd_2:
        MOVS     R0,#+32
        MOVS     R7,R0
??updateHD44780lcd_4:
        MOVS     R0,R7
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall lcd_HD44780_print_char
        BL       lcd_HD44780_print_char
        LDRB     R0,[R5, #+4]
        CMP      R0,#+0
        BNE.N    ??updateHD44780lcd_6
        LDR      R0,[R5, #+0]
          CFI FunCall lcd_HD44780_print_string
        BL       lcd_HD44780_print_string
        B.N      ??updateHD44780lcd_7
??updateHD44780lcd_6:
        MOVS     R1,R6
        LDRB     R0,[R5, #+4]
          CFI FunCall _Z8showLine10lcd_line_ei
        BL       _Z8showLine10lcd_line_ei
??updateHD44780lcd_7:
          CFI FunCall _Z14fillWithSpacesv
        BL       _Z14fillWithSpacesv
        LDR      R5,[R5, #+28]
        ADDS     R6,R6,#+1
        B.N      ??updateHD44780lcd_0
??updateHD44780lcd_1:
        LDR.N    R0,??DataTable5_2
        LDR      R0,[R0, #+4]
        CMP      R6,R0
        BGE.N    ??updateHD44780lcd_8
        MOVS     R1,#+0
        MOVS     R0,R6
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall lcd_HD44780_set_position
        BL       lcd_HD44780_set_position
          CFI FunCall _Z14fillWithSpacesv
        BL       _Z14fillWithSpacesv
        ADDS     R6,R6,#+1
        B.N      ??updateHD44780lcd_1
??updateHD44780lcd_8:
          CFI FunCall getWarninig
        BL       getWarninig
        LDR.N    R1,??DataTable5_52
        LDR      R1,[R1, #+0]
        LDR      R2,[R1, #+516]
        MOVS     R1,R0
        LDR.N    R0,??DataTable5_77
          CFI FunCall memcpy
        BL       memcpy
        LDR.N    R0,??DataTable5_52
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+516]
        LDR.N    R1,??DataTable5_77
        MOVS     R2,#+0
        STRB     R2,[R0, R1]
        MOVS     R1,#+0
        LDR.N    R0,??DataTable5_52
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+520]
        SUBS     R0,R0,#+1
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall lcd_HD44780_set_position
        BL       lcd_HD44780_set_position
        LDR.N    R0,??DataTable5_77
          CFI FunCall lcd_HD44780_print_string
        BL       lcd_HD44780_print_string
          CFI FunCall _Z14fillWithSpacesv
        BL       _Z14fillWithSpacesv
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock7

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5:
        DC8      0x28, 0x29, 0x00, 0x00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_1:
        DC32     ROOT

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_2:
        DC32     tree

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_3:
        DC32     miRpm

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_4:
        DC32     `?<Constant "sensors">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_5:
        DC32     miSensors

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_6:
        DC32     `?<Constant "bench test">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_7:
        DC32     miBench

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_8:
        DC32     `?<Constant "about">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_9:
        DC32     miAbout

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_10:
        DC32     miTriggerErrors

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_11:
        DC32     miTriggerDuty

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_12:
        DC32     miClt

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_13:
        DC32     miIat

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_14:
        DC32     miTps

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_15:
        DC32     miVBatt

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_16:
        DC32     miMap

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_17:
        DC32     miAfr

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_18:
        DC32     miBaro

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_19:
        DC32     miMapV

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_20:
        DC32     miMapKgHr

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_21:
        DC32     _Z10stopEnginev

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_22:
        DC32     `?<Constant "stop engine">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_23:
        DC32     miStopEngine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_24:
        DC32     _Z8fanBenchv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_25:
        DC32     `?<Constant "test fan">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_26:
        DC32     miTestFan

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_27:
        DC32     _Z13fuelPumpBenchv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_28:
        DC32     `?<Constant "test pump">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_29:
        DC32     miTestFuelPump

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_30:
        DC32     `?<Constant "test spark1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_31:
        DC32     miTestSpark1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_32:
        DC32     `?<Constant "test spark2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_33:
        DC32     miTestSpark2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_34:
        DC32     `?<Constant "test spark3">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_35:
        DC32     miTestSpark3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_36:
        DC32     `?<Constant "test spark4">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_37:
        DC32     miTestSpark4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_38:
        DC32     `?<Constant "test injector1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_39:
        DC32     miTestInj1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_40:
        DC32     `?<Constant "test injector2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_41:
        DC32     miTestInj2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_42:
        DC32     `?<Constant "test injector3">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_43:
        DC32     miTestInj3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_44:
        DC32     `?<Constant "test injector4">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_45:
        DC32     miTestInj4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_46:
        DC32     miVersion

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_47:
        DC32     miConfig

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_48:
        DC32     miAlgo

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_49:
        DC32     miInjection

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_50:
        DC32     miIgnition

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_51:
        DC32     miInjFlow

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_52:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_53:
        DC32     lcdLineBuffer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_54:
        DC32     lcdLineStream

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_55:
        DC32     `?<Constant "8529">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_56:
        DC32     `?<Constant "version %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_57:
        DC32     `?<Constant "config %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_58:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_59:
        DC32     `?<Constant "RPM %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_60:
        DC32     `?<Constant "  %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_61:
        DC32     `?<Constant "Coolant %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_62:
        DC32     `?<Constant "Intake Air %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_63:
        DC32     `?<Constant "Inj %fcc">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_64:
        DC32     `?<Constant "Throttle %f%%">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_65:
        DC32     `?<Constant "Battery %fv">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_66:
        DC32     `?<Constant "Baro: %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_67:
        DC32     `?<Constant "Baro: none">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_68:
        DC32     `?<Constant "AFR: %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_69:
        DC32     `?<Constant "AFR: none">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_70:
        DC32     `?<Constant "MAP %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_71:
        DC32     `?<Constant "MAP: none">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_72:
        DC32     `?<Constant "MAF: %fv">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_73:
        DC32     `?<Constant "MAF: none">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_74:
        DC32     `?<Constant "MAF: %f kg/hr">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_75:
        DC32     `?<Constant "Errors">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_76:
        DC32     `?<Constant "Duty">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_77:
        DC32     buffer

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memcpy
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function memcpy
        THUMB
// __intrinsic __nounwind __interwork __softfp void *memcpy(void *, void const *, size_t)
memcpy:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall __aeabi_memcpy
        BL       __aeabi_memcpy
        MOVS     R0,R4
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock8

        SECTION `.init_array`:CODE:ROOT(2)
        SECTION_TYPE SHT_INIT_ARRAY, 0
        DATA
        DC32    RELOC_ARM_TARGET1 __sti__routine

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "version %s">`:
        DATA
        DC8 "version %s"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "8529">`:
        DATA
        DC8 "8529"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "config %s">`:
        DATA
        DC8 "config %s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "RPM %d">`:
        DATA
        DC8 "RPM %d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "  %d">`:
        DATA
        DC8 "  %d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Coolant %f">`:
        DATA
        DC8 "Coolant %f"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Intake Air %f">`:
        DATA
        DC8 "Intake Air %f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Inj %fcc">`:
        DATA
        DC8 "Inj %fcc"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Throttle %f%%">`:
        DATA
        DC8 "Throttle %f%%"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Battery %fv">`:
        DATA
        DC8 "Battery %fv"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Baro: %f">`:
        DATA
        DC8 "Baro: %f"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Baro: none">`:
        DATA
        DC8 "Baro: none"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "AFR: %f">`:
        DATA
        DC8 "AFR: %f"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "AFR: none">`:
        DATA
        DC8 "AFR: none"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MAP %f">`:
        DATA
        DC8 "MAP %f"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MAP: none">`:
        DATA
        DC8 "MAP: none"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MAF: %fv">`:
        DATA
        DC8 "MAF: %fv"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MAF: none">`:
        DATA
        DC8 "MAF: none"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MAF: %f kg/hr">`:
        DATA
        DC8 "MAF: %f kg/hr"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Errors">`:
        DATA
        DC8 "Errors"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Duty">`:
        DATA
        DC8 "Duty"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "sensors">`:
        DATA
        DC8 "sensors"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "bench test">`:
        DATA
        DC8 "bench test"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "about">`:
        DATA
        DC8 "about"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "stop engine">`:
        DATA
        DC8 "stop engine"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "test fan">`:
        DATA
        DC8 "test fan"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "test pump">`:
        DATA
        DC8 "test pump"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "test spark1">`:
        DATA
        DC8 "test spark1"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "test spark2">`:
        DATA
        DC8 "test spark2"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "test spark3">`:
        DATA
        DC8 "test spark3"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "test spark4">`:
        DATA
        DC8 "test spark4"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "test injector1">`:
        DATA
        DC8 "test injector1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "test injector2">`:
        DATA
        DC8 "test injector2"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "test injector3">`:
        DATA
        DC8 "test injector3"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "test injector4">`:
        DATA
        DC8 "test injector4"
        DC8 0

        END
//  287 
//  288 void updateHD44780lcd(Engine *engine) {
//  289 	MenuItem *p = tree.topVisible;
//  290 	int screenY = 0;
//  291 	for (; screenY < tree.linesCount && p != NULL; screenY++) {
//  292 		lcd_HD44780_set_position(screenY, 0);
//  293 		char firstChar;
//  294 		if (p == tree.current) {
//  295 			if (p->callback != NULL) {
//  296 				firstChar = '!';
//  297 			} else {
//  298 				firstChar = p->firstChild == NULL ? '*' : '>';
//  299 			}
//  300 		} else {
//  301 			firstChar = ' ';
//  302 		}
//  303 		lcd_HD44780_print_char(firstChar);
//  304 		if (p->lcdLine == LL_STRING) {
//  305 			lcd_HD44780_print_string(p->text);
//  306 		} else {
//  307 			showLine(p->lcdLine, screenY);
//  308 		}
//  309 		fillWithSpaces();
//  310 		p = p->next;
//  311 	}
//  312 
//  313 	for (; screenY < tree.linesCount; screenY++) {
//  314 		lcd_HD44780_set_position(screenY, 0);
//  315 		fillWithSpaces();
//  316 	}
//  317 
//  318 	memcpy(buffer, getWarninig(), engineConfiguration->HD44780width);
//  319 	buffer[engineConfiguration->HD44780width] = 0;
//  320 	lcd_HD44780_set_position(engineConfiguration->HD44780height - 1, 0);
//  321 	lcd_HD44780_print_string(buffer);
//  322 	fillWithSpaces();
//  323 
//  324 //
//  325 //	lcd_HD44780_set_position(0, 9);
//  326 //	/**
//  327 //	 * this would blink so that we know the LCD is alive
//  328 //	 */
//  329 //	if (isEven) {
//  330 //		lcd_HD44780_print_char('R');
//  331 //	} else {
//  332 //		lcd_HD44780_print_char(' ');
//  333 //	}
//  334 //	lcd_HD44780_set_position(0, 10);
//  335 //
//  336 //	char * ptr = itoa10(buffer, getRpmE(engine));
//  337 //	ptr[0] = 0;
//  338 //	int len = ptr - buffer;
//  339 //	for (int i = 0; i < 6 - len; i++) {
//  340 //		lcd_HD44780_print_char(' ');
//  341 //	}
//  342 //	lcd_HD44780_print_string(buffer);
//  343 //
//  344 //	if (hasFirmwareError()) {
//  345 //		memcpy(buffer, getFirmwareError(), LCD_WIDTH);
//  346 //		buffer[LCD_WIDTH] = 0;
//  347 //		lcd_HD44780_set_position(1, 0);
//  348 //		lcd_HD44780_print_string(buffer);
//  349 //		return;
//  350 //	}
//  351 //
//  352 //	lcd_HD44780_set_position(1, 0);
//  353 //	memset(buffer, ' ', LCD_WIDTH);
//  354 //	memcpy(buffer, getWarninig(), LCD_WIDTH);
//  355 //	buffer[LCD_WIDTH] = 0;
//  356 //	lcd_HD44780_print_string(buffer);
//  357 //
//  358 //	if (engineConfiguration->HD44780height < 3) {
//  359 //		return;
//  360 //	}
//  361 //
//  362 //	int index = (getTimeNowSeconds() / 2) % (NUMBER_OF_DIFFERENT_LINES / 2);
//  363 //
//  364 //	prepareCurrentSecondLine(engine, index);
//  365 //	buffer[LCD_WIDTH] = 0;
//  366 //	lcd_HD44780_set_position(2, 0);
//  367 //	lcd_HD44780_print_string(buffer);
//  368 //
//  369 //	prepareCurrentSecondLine(engine, index + NUMBER_OF_DIFFERENT_LINES / 2);
//  370 //	buffer[LCD_WIDTH] = 0;
//  371 //	lcd_HD44780_set_position(3, 0);
//  372 //	lcd_HD44780_print_string(buffer);
//  373 //
//  374 //#if EFI_PROD_CODE
//  375 //	dateToString(dateBuffer);
//  376 //	lcd_HD44780_set_position(1, 0);
//  377 //	lcd_HD44780_print_string(dateBuffer);
//  378 //#endif /* EFI_PROD_CODE */
//  379 }
//  380 
//  381 #endif
// 
// 1 280 bytes in section .bss
//     4 bytes in section .init_array
//   416 bytes in section .rodata
// 1 874 bytes in section .text
// 
// 1 856 bytes of CODE  memory (+ 22 bytes shared)
//   416 bytes of CONST memory
// 1 280 bytes of DATA  memory
//
//Errors: none
//Warnings: 5
