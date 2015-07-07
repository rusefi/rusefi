///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:48 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\cor /
//                       e\fsio_impl.cpp                                      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\cor /
//                       e\fsio_impl.cpp -lCN F:\stuff\rusefi_sourceforge\fir /
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
//                       fsio_impl.s                                          /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME fsio_impl

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z10hwPortname11brain_pin_e
        EXTERN _Z11getAcTogglev
        EXTERN _Z12getInputMode16pin_input_mode_e
        EXTERN _Z13applyPinStateP9PwmConfigi
        EXTERN _Z13mySetPadMode2PKc11brain_pin_ej
        EXTERN _Z13parseBrainPinPKc
        EXTERN _Z17startSimplePwmExtP9PwmConfigPKc11brain_pin_eP9OutputPinffPFvS0_iE
        EXTERN _Z18isRunningBenchTestv
        EXTERN _Z21getCoolantTemperaturev
        EXTERN _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
        EXTERN _Z23getIntakeAirTemperaturev
        EXTERN _Z8getVBattv
        EXTERN _ZN12LECalculator9getValue2EP9LEElementP6Engine
        EXTERN _ZN12LECalculatorC1Ev
        EXTERN _ZN13LEElementPool15parseExpressionEPKc
        EXTERN _ZN13LEElementPool5resetEv
        EXTERN _ZN13LEElementPool7getSizeEv
        EXTERN _ZN13LEElementPoolC1EP9LEElementi
        EXTERN _ZN13RpmCalculator3rpmEv
        EXTERN _ZN13RpmCalculator9isRunningEv
        EXTERN _ZN17LENameOrdinalPairC1E11le_action_ePKc
        EXTERN _ZN9LEElementC1Ev
        EXTERN _ZN9OutputPin13getLogicValueEv
        EXTERN _ZN9OutputPin13isInitializedEv
        EXTERN _ZN9OutputPin8setValueEi
        EXTERN _ZN9OutputPinC1Ev
        EXTERN _ZN9SimplePwm21setSimplePwmDutyCycleEf
        EXTERN _ZN9SimplePwmC1Ev
        EXTERN __aeabi_f2d
        EXTERN __aeabi_vec_ctor_nocookie_nodtor
        EXTERN __iar_FDtest
        EXTERN addConsoleAction
        EXTERN addConsoleActionFF
        EXTERN addConsoleActionII
        EXTERN addConsoleActionSP
        EXTERN addConsoleActionSS
        EXTERN addConsoleActionSSP
        EXTERN atoi
        EXTERN boardConfiguration
        EXTERN config
        EXTERN engine
        EXTERN engineConfiguration
        EXTERN engineConfiguration2
        EXTERN enginePins
        EXTERN firmwareError
        EXTERN getTimeNowSeconds
        EXTERN itoa10
        EXTERN scheduleMsg
        EXTERN strcpy
        EXTERN strlen
        EXTERN unquote
        EXTERN warning

        PUBLIC _Z10getLEValueP6EngineP7FLStackIfLi32EE11le_action_e
        PUBLIC _Z10setFsioExti11brain_pin_ePKci
        PUBLIC _Z12initFsioImplP7LoggingP6Engine
        PUBLIC _Z22applyFsioConfigurationv
        PUBLIC _Z7runFsiov
        PUBLIC _Z7setFsioi11brain_pin_ePKc
        PUBLIC sysPool
        PUBLIC userPool
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\core\fsio_impl.cpp
//    1 /**
//    2  * @file fsio_impl.cpp
//    3  * @brief FSIO as it's used for GPIO
//    4  *
//    5  * @date Oct 5, 2014
//    6  * @author Andrey Belomutskiy, (c) 2012-2015
//    7  */
//    8 
//    9 #include "main.h"
//   10 
//   11 #if EFI_FSIO || defined(__DOXYGEN__)
//   12 
//   13 #include "fsio_impl.h"
//   14 #include "allsensors.h"
//   15 #include "rpm_calculator.h"
//   16 #include "efiGpio.h"
//   17 
//   18 #define NO_PWM 0
//   19 
//   20 /**
//   21  * Here we define all rusEfi-specific methods
//   22  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   23 static LENameOrdinalPair leRpm(LE_METHOD_RPM, "rpm");
        ADR.N    R2,??DataTable1  ;; "rpm"
        MOVS     R1,#+100
        LDR.W    R0,??DataTable15_1
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
//   24 static LENameOrdinalPair leTps(LE_METHOD_TPS, "tps");
        ADR.N    R2,??DataTable1_1  ;; "tps"
        MOVS     R1,#+106
        LDR.W    R0,??DataTable15_2
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
//   25 static LENameOrdinalPair leMaf(LE_METHOD_MAF, "maf");
        ADR.N    R2,??DataTable1_2  ;; "maf"
        MOVS     R1,#+107
        LDR.W    R0,??DataTable16_1
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
//   26 static LENameOrdinalPair leVBatt(LE_METHOD_VBATT, "vbatt");
        LDR.W    R2,??DataTable16_2
        MOVS     R1,#+109
        LDR.W    R0,??DataTable16_3
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
//   27 static LENameOrdinalPair leFan(LE_METHOD_FAN, "fan");
        ADR.N    R2,??DataTable2  ;; "fan"
        MOVS     R1,#+102
        LDR.W    R0,??DataTable16_4
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
//   28 static LENameOrdinalPair leCoolant(LE_METHOD_COOLANT, "coolant");
        LDR.W    R2,??DataTable16_5
        MOVS     R1,#+101
        LDR.W    R0,??DataTable16_6
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
//   29 static LENameOrdinalPair leAcToggle(LE_METHOD_AC_TOGGLE, "ac_on_switch");
        LDR.W    R2,??DataTable16_7
        MOVS     R1,#+110
        LDR.W    R0,??DataTable16_8
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
//   30 static LENameOrdinalPair leFanOnSetting(LE_METHOD_FAN_ON_SETTING, "fan_on_setting");
        LDR.W    R2,??DataTable16_9
        MOVS     R1,#+104
        LDR.W    R0,??DataTable16_10
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
//   31 static LENameOrdinalPair leFanOffSetting(LE_METHOD_FAN_OFF_SETTING, "fan_off_setting");
        LDR.W    R2,??DataTable16_11
        MOVS     R1,#+105
        LDR.W    R0,??DataTable16_12
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
//   32 static LENameOrdinalPair leTimeSinceBoot(LE_METHOD_TIME_SINCE_BOOT, "time_since_boot");
        LDR.W    R2,??DataTable16_13
        MOVS     R1,#+103
        LDR.W    R0,??DataTable16_14
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
//   33 static LENameOrdinalPair leFsioSetting(LE_METHOD_FSIO_SETTING, "fsio_setting");
        LDR.W    R2,??DataTable16_15
        MOVS     R1,#+111
        LDR.W    R0,??DataTable16_16
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
//   34 static LENameOrdinalPair leKnock(LE_METHOD_KNOCK, "knock");
        LDR.W    R2,??DataTable16_17
        MOVS     R1,#+112
        LDR.W    R0,??DataTable16_18
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
//   35 
//   36 #define LE_EVAL_POOL_SIZE 32
//   37 
//   38 extern engine_pins_s enginePins;
//   39 
//   40 static LECalculator evalCalc;
        LDR.W    R0,??DataTable16_19
          CFI FunCall _ZN12LECalculatorC1Ev
        BL       _ZN12LECalculatorC1Ev
//   41 static LEElement evalPoolElements[LE_EVAL_POOL_SIZE];
        MOVS     R3,#+32
        MOVS     R2,#+16
        LDR.W    R1,??DataTable16_20
        LDR.W    R0,??DataTable16_21
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
//   42 static LEElementPool evalPool(evalPoolElements, LE_EVAL_POOL_SIZE);
        MOVS     R2,#+32
        LDR.W    R1,??DataTable16_21
        LDR.W    R0,??DataTable16_22
          CFI FunCall _ZN13LEElementPoolC1EP9LEElementi
        BL       _ZN13LEElementPoolC1EP9LEElementi
//   43 
//   44 #define SYS_ELEMENT_POOL_SIZE 128
//   45 #define UD_ELEMENT_POOL_SIZE 128
//   46 
//   47 static LEElement sysElements[SYS_ELEMENT_POOL_SIZE];
        MOVS     R3,#+128
        MOVS     R2,#+16
        LDR.W    R1,??DataTable16_20
        LDR.W    R0,??DataTable16_23
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
//   48 LEElementPool sysPool(sysElements, SYS_ELEMENT_POOL_SIZE);
        MOVS     R2,#+128
        LDR.W    R1,??DataTable16_23
        LDR.W    R0,??DataTable16_24
          CFI FunCall _ZN13LEElementPoolC1EP9LEElementi
        BL       _ZN13LEElementPoolC1EP9LEElementi
//   49 
//   50 static LEElement userElements[UD_ELEMENT_POOL_SIZE];
        MOVS     R3,#+128
        MOVS     R2,#+16
        LDR.W    R1,??DataTable16_20
        LDR.W    R0,??DataTable16_25
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
//   51 LEElementPool userPool(userElements, UD_ELEMENT_POOL_SIZE);
        MOVS     R2,#+128
        LDR.W    R1,??DataTable16_25
        LDR.W    R0,??DataTable16_26
          CFI FunCall _ZN13LEElementPoolC1EP9LEElementi
        BL       _ZN13LEElementPoolC1EP9LEElementi
        MOVS     R3,#+16
        MOVS     R2,#+112
        LDR.W    R1,??DataTable16_27
        LDR.W    R0,??DataTable16_28
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
        LDR.W    R0,??DataTable16_29
          CFI FunCall _ZN12LECalculatorC1Ev
        BL       _ZN12LECalculatorC1Ev
        MOVS     R3,#+16
        MOVS     R2,#+16
        LDR.W    R1,??DataTable16_30
        LDR.W    R0,??DataTable17_1
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
leRpm:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
leTps:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
leMaf:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
leVBatt:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
leFan:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
leCoolant:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
leAcToggle:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
leFanOnSetting:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
leFanOffSetting:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
leTimeSinceBoot:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
leFsioSetting:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
leKnock:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
evalCalc:
        DS8 652

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
evalPoolElements:
        DS8 512

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
evalPool:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
sysElements:
        DS8 2048

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
sysPool:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
userElements:
        DS8 2048

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
userPool:
        DS8 12

        SECTION `.ccm`:DATA:REORDER:NOROOT(2)
//   52 static LEElement * fsioLogics[LE_COMMAND_COUNT] CCM_OPTIONAL;
fsioLogics:
        DS8 64
//   53 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   54 static LEElement * acRelayLogic;
acRelayLogic:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   55 static LEElement * fuelPumpLogic;
fuelPumpLogic:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   56 static LEElement * radiatorFanLogic;
radiatorFanLogic:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   57 static LEElement * alternatorLogic;
alternatorLogic:
        DS8 4
//   58 
//   59 EXTERN_ENGINE
//   60 ;
//   61 
//   62 #if EFI_PROD_CODE || EFI_SIMULATOR

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   63 static Logging *logger;
logger:
        DS8 4
//   64 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z10getLEValueP6EngineP7FLStackIfLi32EE11le_action_e
        THUMB
//   65 float getLEValue(Engine *engine, calc_stack_t *s, le_action_e action) {
_Z10getLEValueP6EngineP7FLStackIfLi32EE11le_action_e:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   66 	engine_configuration_s *engineConfiguration = engine->engineConfiguration;
        LDR      R7,[R4, #+36]
//   67 	efiAssert(engine!=NULL, "getLEValue", NAN);
        CMP      R4,#+0
        BNE.N    ??getLEValue_0
        LDR.W    R0,??DataTable16_31
          CFI FunCall firmwareError
        BL       firmwareError
        VLDR.W   S0,??DataTable6  ;; 0x7fffffff
        B.N      ??getLEValue_1
//   68 	switch (action) {
??getLEValue_0:
        MOVS     R0,R6
        CMP      R0,#+100
        BEQ.N    ??getLEValue_2
        CMP      R0,#+101
        BEQ.N    ??getLEValue_3
        CMP      R0,#+102
        BEQ.N    ??getLEValue_4
        CMP      R0,#+103
        BEQ.N    ??getLEValue_5
        CMP      R0,#+104
        BEQ.N    ??getLEValue_6
        CMP      R0,#+105
        BEQ.N    ??getLEValue_7
        CMP      R0,#+108
        BEQ.N    ??getLEValue_8
        CMP      R0,#+109
        BEQ.N    ??getLEValue_9
        CMP      R0,#+110
        BEQ.N    ??getLEValue_10
        B.N      ??getLEValue_11
//   69 	case LE_METHOD_FAN:
//   70 		return enginePins.fanRelay.getLogicValue();
??getLEValue_4:
        LDR.W    R0,??DataTable16_32
          CFI FunCall _ZN9OutputPin13getLogicValueEv
        BL       _ZN9OutputPin13getLogicValueEv
        VMOV     S0,R0
        VCVT.F32.U32 S0,S0
        B.N      ??getLEValue_1
//   71 	case LE_METHOD_AC_TOGGLE:
//   72 		return getAcToggle(PASS_ENGINE_PARAMETER_F);
??getLEValue_10:
          CFI FunCall _Z11getAcTogglev
        BL       _Z11getAcTogglev
        VMOV     S0,R0
        VCVT.F32.U32 S0,S0
        B.N      ??getLEValue_1
//   73 	case LE_METHOD_COOLANT:
//   74 		return getCoolantTemperature(PASS_ENGINE_PARAMETER_F);
??getLEValue_3:
          CFI FunCall _Z21getCoolantTemperaturev
        BL       _Z21getCoolantTemperaturev
        B.N      ??getLEValue_1
//   75 	case LE_METHOD_INTAKE_AIR:
//   76 		return getIntakeAirTemperature(PASS_ENGINE_PARAMETER_F);
??getLEValue_8:
          CFI FunCall _Z23getIntakeAirTemperaturev
        BL       _Z23getIntakeAirTemperaturev
        B.N      ??getLEValue_1
//   77 	case LE_METHOD_RPM:
//   78 		return engine->rpmCalculator.rpm();
??getLEValue_2:
        MOVS     R0,R4
          CFI FunCall _ZN13RpmCalculator3rpmEv
        BL       _ZN13RpmCalculator3rpmEv
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        B.N      ??getLEValue_1
//   79 	case LE_METHOD_TIME_SINCE_BOOT:
//   80 		return getTimeNowSeconds();
??getLEValue_5:
          CFI FunCall getTimeNowSeconds
        BL       getTimeNowSeconds
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        B.N      ??getLEValue_1
//   81 	case LE_METHOD_FAN_OFF_SETTING:
//   82 		return engineConfiguration->fanOffTemperature;
??getLEValue_7:
        VLDR     S0,[R7, #+480]
        B.N      ??getLEValue_1
//   83 	case LE_METHOD_FAN_ON_SETTING:
//   84 		return engineConfiguration->fanOnTemperature;
??getLEValue_6:
        VLDR     S0,[R7, #+476]
        B.N      ??getLEValue_1
//   85 	case LE_METHOD_VBATT:
//   86 		return getVBatt(PASS_ENGINE_PARAMETER_F);
??getLEValue_9:
          CFI FunCall _Z8getVBattv
        BL       _Z8getVBattv
        B.N      ??getLEValue_1
//   87 	default:
//   88 		warning(OBD_PCM_Processor_Fault, "FSIO unexpected %d", action);
??getLEValue_11:
        MOVS     R2,R6
        LDR.W    R1,??DataTable16_33
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//   89 		return NAN;
        VLDR.W   S0,??DataTable6  ;; 0x7fffffff
??getLEValue_1:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock1
//   90 	}
//   91 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1:
        DC8      "rpm"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_1:
        DC8      "tps"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_2:
        DC8      "maf"
//   92 
//   93 #endif
//   94 
//   95 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//   96 
//   97 #include "pin_repository.h"
//   98 #include "pwm_generator.h"
//   99 // todo: that's about bench test mode, wrong header for sure!
//  100 #include "injector_central.h"
//  101 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z15setFsioInputPinPKcS0_
        THUMB
//  102 static void setFsioInputPin(const char *indexStr, const char *pinName) {
_Z15setFsioInputPinPKcS0_:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  103 	int index = atoi(indexStr) - 1;
        MOVS     R0,R4
          CFI FunCall atoi
        BL       atoi
        SUBS     R6,R0,#+1
//  104 	if (index < 0 || index >= LE_COMMAND_COUNT) {
        CMP      R6,#+0
        BMI.N    ??setFsioInputPin_0
        CMP      R6,#+16
        BLT.N    ??setFsioInputPin_1
//  105 		scheduleMsg(logger, "invalid index %d", index);
??setFsioInputPin_0:
        MOVS     R2,R6
        LDR.W    R1,??DataTable17_2
        LDR.W    R0,??DataTable17_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  106 		return;
        B.N      ??setFsioInputPin_2
//  107 	}
//  108 	brain_pin_e pin = parseBrainPin(pinName);
??setFsioInputPin_1:
        MOVS     R0,R5
          CFI FunCall _Z13parseBrainPinPKc
        BL       _Z13parseBrainPinPKc
        MOVS     R7,R0
//  109 	// todo: extract method - code duplication with other 'set_xxx_pin' methods?
//  110 	if (pin == GPIO_INVALID) {
        CMP      R7,#+81
        BNE.N    ??setFsioInputPin_3
//  111 		scheduleMsg(logger, "invalid pin name [%s]", pinName);
        MOVS     R2,R5
        LDR.W    R1,??DataTable17_4
        LDR.W    R0,??DataTable17_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  112 		return;
        B.N      ??setFsioInputPin_2
//  113 	}
//  114 	boardConfiguration->fsioDigitalInputs[index] = pin;
??setFsioInputPin_3:
        LDR.W    R0,??DataTable17_5
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R6, LSL #+2
        STR      R7,[R0, #+804]
//  115 	scheduleMsg(logger, "FSIO input pin #%d [%s]", (index + 1), hwPortname(pin));
        MOVS     R0,R7
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R3,R0
        ADDS     R2,R6,#+1
        LDR.W    R1,??DataTable17_6
        LDR.W    R0,??DataTable17_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  116 }
??setFsioInputPin_2:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2:
        DC8      "fan"
//  117 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z16setFsioOutputPinPKcS0_
        THUMB
//  118 static void setFsioOutputPin(const char *indexStr, const char *pinName) {
_Z16setFsioOutputPinPKcS0_:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  119 	int index = atoi(indexStr) - 1;
        MOVS     R0,R4
          CFI FunCall atoi
        BL       atoi
        SUBS     R6,R0,#+1
//  120 	if (index < 0 || index >= LE_COMMAND_COUNT) {
        CMP      R6,#+0
        BMI.N    ??setFsioOutputPin_0
        CMP      R6,#+16
        BLT.N    ??setFsioOutputPin_1
//  121 		scheduleMsg(logger, "invalid index %d", index);
??setFsioOutputPin_0:
        MOVS     R2,R6
        LDR.W    R1,??DataTable17_2
        LDR.W    R0,??DataTable17_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  122 		return;
        B.N      ??setFsioOutputPin_2
//  123 	}
//  124 	brain_pin_e pin = parseBrainPin(pinName);
??setFsioOutputPin_1:
        MOVS     R0,R5
          CFI FunCall _Z13parseBrainPinPKc
        BL       _Z13parseBrainPinPKc
        MOVS     R7,R0
//  125 	// todo: extract method - code duplication with other 'set_xxx_pin' methods?
//  126 	if (pin == GPIO_INVALID) {
        CMP      R7,#+81
        BNE.N    ??setFsioOutputPin_3
//  127 		scheduleMsg(logger, "invalid pin name [%s]", pinName);
        MOVS     R2,R5
        LDR.W    R1,??DataTable17_4
        LDR.W    R0,??DataTable17_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  128 		return;
        B.N      ??setFsioOutputPin_2
//  129 	}
//  130 	boardConfiguration->fsioPins[index] = pin;
??setFsioOutputPin_3:
        LDR.W    R0,??DataTable17_5
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R6, LSL #+2
        STR      R7,[R0, #+452]
//  131 	scheduleMsg(logger, "FSIO output pin #%d [%s]", (index + 1), hwPortname(pin));
        MOVS     R0,R7
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R3,R0
        ADDS     R2,R6,#+1
        LDR.W    R1,??DataTable17_7
        LDR.W    R0,??DataTable17_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  132 }
??setFsioOutputPin_2:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock3
//  133 #endif
//  134 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z10setFsioExti11brain_pin_ePKci
        THUMB
//  135 void setFsioExt(int index, brain_pin_e pin, const char * exp, int freq DECLARE_ENGINE_PARAMETER_S) {
_Z10setFsioExti11brain_pin_ePKci:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
//  136 	boardConfiguration->fsioPins[index] = pin;
        LDR.W    R0,??DataTable17_5
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R4, LSL #+2
        STR      R5,[R0, #+452]
//  137 	int len = strlen(exp);
        MOVS     R0,R6
          CFI FunCall strlen
        BL       strlen
        MOV      R8,R0
//  138 	if (len >= LE_COMMAND_LENGTH) {
        CMP      R8,#+200
        BGE.N    ??setFsioExt_0
//  139 		return;
//  140 	}
//  141 	strcpy(config->le_formulas[index], exp);
??setFsioExt_1:
        MOVS     R1,R6
        MOVS     R0,#+200
        LDR.W    R2,??DataTable17_8
        LDR      R2,[R2, #+0]
        MLA      R0,R0,R4,R2
        ADDW     R0,R0,#+2216
          CFI FunCall strcpy
        BL       strcpy
//  142 	boardConfiguration->fsioFrequency[index] = freq;
        LDR.W    R0,??DataTable17_5
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R4, LSL #+1
        STRH     R7,[R0, #+616]
//  143 }
??setFsioExt_0:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock4
//  144 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z7setFsioi11brain_pin_ePKc
        THUMB
//  145 void setFsio(int index, brain_pin_e pin, const char * exp DECLARE_ENGINE_PARAMETER_S) {
_Z7setFsioi11brain_pin_ePKc:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  146 	setFsioExt(index, pin, exp, NO_PWM PASS_ENGINE_PARAMETER);
        MOVS     R3,#+0
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z10setFsioExti11brain_pin_ePKci
        BL       _Z10setFsioExti11brain_pin_ePKci
//  147 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock5
//  148 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z22applyFsioConfigurationv
        THUMB
//  149 void applyFsioConfiguration(DECLARE_ENGINE_PARAMETER_F) {
_Z22applyFsioConfigurationv:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
//  150 	for (int i = 0; i < LE_COMMAND_COUNT; i++) {
        MOVS     R4,#+0
??applyFsioConfiguration_0:
        CMP      R4,#+16
        BGE.N    ??applyFsioConfiguration_1
//  151 		brain_pin_e brainPin = boardConfiguration->fsioPins[i];
        LDR.W    R0,??DataTable17_5
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R4, LSL #+2
        LDR      R5,[R0, #+452]
//  152 
//  153 		if (brainPin != GPIO_UNASSIGNED) {
        CMP      R5,#+80
        BEQ.N    ??applyFsioConfiguration_2
//  154 			const char *formula = config->le_formulas[i];
        MOVS     R0,#+200
        LDR.W    R1,??DataTable17_8
        LDR      R1,[R1, #+0]
        MLA      R0,R0,R4,R1
        ADDW     R6,R0,#+2216
//  155 			LEElement *logic = userPool.parseExpression(formula);
        MOVS     R1,R6
        LDR.W    R0,??DataTable16_26
          CFI FunCall _ZN13LEElementPool15parseExpressionEPKc
        BL       _ZN13LEElementPool15parseExpressionEPKc
        MOVS     R7,R0
//  156 			if (logic == NULL) {
        CMP      R7,#+0
        BNE.N    ??applyFsioConfiguration_3
//  157 				warning(OBD_PCM_Processor_Fault, "parsing [%s]", formula);
        MOVS     R2,R6
        LDR.W    R1,??DataTable17_9
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  158 			}
//  159 
//  160 			fsioLogics[i] = logic;
??applyFsioConfiguration_3:
        LDR.W    R0,??DataTable17_10
        STR      R7,[R0, R4, LSL #+2]
//  161 		}
//  162 	}
??applyFsioConfiguration_2:
        ADDS     R4,R4,#+1
        B.N      ??applyFsioConfiguration_0
//  163 }
??applyFsioConfiguration_1:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock6
//  164 
//  165 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//  166 

        SECTION `.ccm`:DATA:REORDER:NOROOT(3)
//  167 static SimplePwm fsioPwm[LE_COMMAND_COUNT] CCM_OPTIONAL;
fsioPwm:
        DS8 1792
//  168 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  169 static LECalculator calc;
calc:
        DS8 652
//  170 extern LEElement * fsioLogics[LE_COMMAND_COUNT];
//  171 
//  172 // that's crazy, but what's an alternative? we need const char *, a shared buffer would not work for pin repository

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z14getGpioPinNamei
          CFI NoCalls
        THUMB
//  173 static const char *getGpioPinName(int index) {
_Z14getGpioPinNamei:
        MOVS     R1,R0
//  174 	switch (index) {
        CMP      R1,#+0
        BEQ.N    ??getGpioPinName_0
        CMP      R1,#+2
        BEQ.N    ??getGpioPinName_1
        BCC.N    ??getGpioPinName_2
        CMP      R1,#+4
        BEQ.N    ??getGpioPinName_3
        BCC.N    ??getGpioPinName_4
        CMP      R1,#+6
        BEQ.N    ??getGpioPinName_5
        BCC.N    ??getGpioPinName_6
        CMP      R1,#+8
        BEQ.N    ??getGpioPinName_7
        BCC.N    ??getGpioPinName_8
        CMP      R1,#+10
        BEQ.N    ??getGpioPinName_9
        BCC.N    ??getGpioPinName_10
        CMP      R1,#+12
        BEQ.N    ??getGpioPinName_11
        BCC.N    ??getGpioPinName_12
        CMP      R1,#+14
        BEQ.N    ??getGpioPinName_13
        BCC.N    ??getGpioPinName_14
        CMP      R1,#+15
        BEQ.N    ??getGpioPinName_15
        B.N      ??getGpioPinName_16
//  175 	case 0:
//  176 		return "GPIO_0";
??getGpioPinName_0:
        LDR.W    R0,??DataTable17_11
        B.N      ??getGpioPinName_17
//  177 	case 1:
//  178 		return "GPIO_1";
??getGpioPinName_2:
        LDR.W    R0,??DataTable17_12
        B.N      ??getGpioPinName_17
//  179 	case 10:
//  180 		return "GPIO_10";
??getGpioPinName_9:
        LDR.W    R0,??DataTable17_13
        B.N      ??getGpioPinName_17
//  181 	case 11:
//  182 		return "GPIO_11";
??getGpioPinName_12:
        LDR.W    R0,??DataTable17_14
        B.N      ??getGpioPinName_17
//  183 	case 12:
//  184 		return "GPIO_12";
??getGpioPinName_11:
        LDR.W    R0,??DataTable17_15
        B.N      ??getGpioPinName_17
//  185 	case 13:
//  186 		return "GPIO_13";
??getGpioPinName_14:
        LDR.W    R0,??DataTable17_16
        B.N      ??getGpioPinName_17
//  187 	case 14:
//  188 		return "GPIO_14";
??getGpioPinName_13:
        LDR.W    R0,??DataTable17_17
        B.N      ??getGpioPinName_17
//  189 	case 15:
//  190 		return "GPIO_15";
??getGpioPinName_15:
        LDR.W    R0,??DataTable17_18
        B.N      ??getGpioPinName_17
//  191 	case 2:
//  192 		return "GPIO_2";
??getGpioPinName_1:
        LDR.W    R0,??DataTable17_19
        B.N      ??getGpioPinName_17
//  193 	case 3:
//  194 		return "GPIO_3";
??getGpioPinName_4:
        LDR.W    R0,??DataTable17_20
        B.N      ??getGpioPinName_17
//  195 	case 4:
//  196 		return "GPIO_4";
??getGpioPinName_3:
        LDR.W    R0,??DataTable17_21
        B.N      ??getGpioPinName_17
//  197 	case 5:
//  198 		return "GPIO_5";
??getGpioPinName_6:
        LDR.W    R0,??DataTable17_22
        B.N      ??getGpioPinName_17
//  199 	case 6:
//  200 		return "GPIO_6";
??getGpioPinName_5:
        LDR.W    R0,??DataTable17_23
        B.N      ??getGpioPinName_17
//  201 	case 7:
//  202 		return "GPIO_7";
??getGpioPinName_8:
        LDR.W    R0,??DataTable17_24
        B.N      ??getGpioPinName_17
//  203 	case 8:
//  204 		return "GPIO_8";
??getGpioPinName_7:
        LDR.W    R0,??DataTable17_25
        B.N      ??getGpioPinName_17
//  205 	case 9:
//  206 		return "GPIO_9";
??getGpioPinName_10:
        LDR.W    R0,??DataTable17_26
        B.N      ??getGpioPinName_17
//  207 	}
//  208 	return NULL;
??getGpioPinName_16:
        MOVS     R0,#+0
??getGpioPinName_17:
        BX       LR               ;; return
          CFI EndBlock cfiBlock7
//  209 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable6:
        DC32     0x7fffffff
//  210 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  211 static OutputPin fsioOutputs[LE_COMMAND_COUNT];
fsioOutputs:
        DS8 256
//  212 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z10handleFsioP6Enginei
        THUMB
//  213 static void handleFsio(Engine *engine, int index) {
_Z10handleFsioP6Enginei:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        VPUSH    {D8}
          CFI D8 Frame(CFA, -32)
          CFI CFA R13+32
        MOVS     R4,R0
        MOVS     R5,R1
//  214 	if (boardConfiguration->fsioPins[index] == GPIO_UNASSIGNED)
        LDR.W    R0,??DataTable17_5
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R5, LSL #+2
        LDR      R0,[R0, #+452]
        CMP      R0,#+80
        BEQ.N    ??handleFsio_0
//  215 		return;
//  216 
//  217 	bool_t isPwmMode = boardConfiguration->fsioFrequency[index] != NO_PWM;
??handleFsio_1:
        LDR.W    R0,??DataTable17_5
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R5, LSL #+1
        LDRSH    R0,[R0, #+616]
        CMP      R0,#+0
        BEQ.N    ??handleFsio_2
        MOVS     R6,#+1
        B.N      ??handleFsio_3
??handleFsio_2:
        MOVS     R6,#+0
//  218 
//  219 	float fvalue = calc.getValue2(fsioLogics[index], engine);
??handleFsio_3:
        MOVS     R2,R4
        LDR.W    R0,??DataTable17_10
        LDR      R1,[R0, R5, LSL #+2]
        LDR.W    R0,??DataTable16_29
          CFI FunCall _ZN12LECalculator9getValue2EP9LEElementP6Engine
        BL       _ZN12LECalculator9getValue2EP9LEElementP6Engine
        VMOV.F32 S16,S0
//  220 	engine->engineConfiguration2->fsioLastValue[index] = fvalue;
        LDR      R0,[R4, #+40]
        ADDS     R0,R0,R5, LSL #+2
        ADD      R0,R0,#+12032
        ADDS     R0,R0,#+16
        VSTR     S16,[R0, #0]
//  221 
//  222 	if (isPwmMode) {
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+0
        BEQ.N    ??handleFsio_4
//  223 		fsioPwm[index].setSimplePwmDutyCycle(fvalue);
        VMOV.F32 S0,S16
        MOVS     R0,#+112
        LDR.W    R1,??DataTable16_28
        MLA      R0,R0,R5,R1
          CFI FunCall _ZN9SimplePwm21setSimplePwmDutyCycleEf
        BL       _ZN9SimplePwm21setSimplePwmDutyCycleEf
        B.N      ??handleFsio_5
//  224 	} else {
//  225 		int value = (int) fvalue;
??handleFsio_4:
        VCVT.S32.F32 S0,S16
        VMOV     R7,S0
//  226 		if (value != fsioOutputs[index].getLogicValue()) {
        LDR.W    R0,??DataTable17_1
        ADDS     R0,R0,R5, LSL #+4
          CFI FunCall _ZN9OutputPin13getLogicValueEv
        BL       _ZN9OutputPin13getLogicValueEv
        CMP      R7,R0
        BEQ.N    ??handleFsio_5
//  227 			//		scheduleMsg(logger, "setting %s %s", getIo_pin_e(pin), boolToString(value));
//  228 			fsioOutputs[index].setValue(value);
        MOVS     R1,R7
        LDR.W    R0,??DataTable17_1
        ADDS     R0,R0,R5, LSL #+4
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
//  229 		}
//  230 	}
//  231 }
??handleFsio_5:
??handleFsio_0:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+24
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock8
//  232 
//  233 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z13action2String11le_action_e
        THUMB
//  234 static const char * action2String(le_action_e action) {
_Z13action2String11le_action_e:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  235 	static char buffer[10];
//  236 	switch(action) {
        MOVS     R0,R4
        CMP      R0,#+100
        BEQ.N    ??action2String_0
        CMP      R0,#+101
        BEQ.N    ??action2String_1
        CMP      R0,#+102
        BEQ.N    ??action2String_2
        CMP      R0,#+104
        BEQ.N    ??action2String_3
        CMP      R0,#+105
        BEQ.N    ??action2String_4
        B.N      ??action2String_5
//  237 		case LE_METHOD_RPM:
//  238 			return "RPM";
??action2String_0:
        ADR.N    R0,??DataTable10  ;; "RPM"
        B.N      ??action2String_6
//  239 		case LE_METHOD_COOLANT:
//  240 			return "CLT";
??action2String_1:
        ADR.N    R0,??DataTable10_1  ;; "CLT"
        B.N      ??action2String_6
//  241 		case LE_METHOD_FAN_ON_SETTING:
//  242 			return "fan_on";
??action2String_3:
        LDR.W    R0,??DataTable17_27
        B.N      ??action2String_6
//  243 		case LE_METHOD_FAN_OFF_SETTING:
//  244 			return "fan_off";
??action2String_4:
        LDR.W    R0,??DataTable17_28
        B.N      ??action2String_6
//  245 		case LE_METHOD_FAN:
//  246 			return "fan";
??action2String_2:
        ADR.N    R0,??DataTable11  ;; "fan"
        B.N      ??action2String_6
//  247 		default: {
//  248 			// this is here to make compiler happy
//  249 		}
//  250 	}
//  251 	itoa10(buffer, (int)action);
??action2String_5:
        MOVS     R1,R4
        LDR.W    R0,??DataTable17_29
          CFI FunCall itoa10
        BL       itoa10
//  252 	return buffer;
        LDR.W    R0,??DataTable17_29
??action2String_6:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock9
//  253 }

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
??buffer:
        DS8 12
//  254 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z11setPinStatePKcP9OutputPinP9LEElementP6Engine
        THUMB
//  255 static void setPinState(const char * msg, OutputPin *pin, LEElement *element, Engine *engine) {
_Z11setPinStatePKcP9OutputPinP9LEElementP6Engine:
        PUSH     {R2-R10,LR}
          CFI R14 Frame(CFA, -4)
          CFI R10 Frame(CFA, -8)
          CFI R9 Frame(CFA, -12)
          CFI R8 Frame(CFA, -16)
          CFI R7 Frame(CFA, -20)
          CFI R6 Frame(CFA, -24)
          CFI R5 Frame(CFA, -28)
          CFI R4 Frame(CFA, -32)
          CFI CFA R13+40
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
//  256 	if (element == NULL) {
        CMP      R6,#+0
        BNE.N    ??setPinState_0
//  257 		warning(OBD_PCM_Processor_Fault, "invalid expression for %s", msg);
        MOVS     R2,R4
        LDR.W    R1,??DataTable17_30
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
        B.N      ??setPinState_1
//  258 	} else {
//  259 		int value = calc.getValue2(element, engine);
??setPinState_0:
        MOVS     R2,R7
        MOVS     R1,R6
        LDR.W    R0,??DataTable16_29
          CFI FunCall _ZN12LECalculator9getValue2EP9LEElementP6Engine
        BL       _ZN12LECalculator9getValue2EP9LEElementP6Engine
        VCVT.S32.F32 S0,S0
        VMOV     R8,S0
//  260 		if (pin->isInitialized() && value != pin->getLogicValue()) {
        MOVS     R0,R5
          CFI FunCall _ZN9OutputPin13isInitializedEv
        BL       _ZN9OutputPin13isInitializedEv
        CMP      R0,#+0
        BEQ.N    ??setPinState_1
        MOVS     R0,R5
          CFI FunCall _ZN9OutputPin13getLogicValueEv
        BL       _ZN9OutputPin13getLogicValueEv
        CMP      R8,R0
        BEQ.N    ??setPinState_1
//  261 			if (isRunningBenchTest()) {
          CFI FunCall _Z18isRunningBenchTestv
        BL       _Z18isRunningBenchTestv
        CMP      R0,#+0
        BNE.N    ??setPinState_2
//  262 				return; // let's not mess with bench testing
//  263 			}
//  264 
//  265 			for (int i = 0;i < calc.currentCalculationLogPosition;i++) {
??setPinState_3:
        MOVS     R9,#+0
??setPinState_4:
        LDR.W    R0,??DataTable16_29
        LDR      R0,[R0, #+512]
        CMP      R9,R0
        BGE.N    ??setPinState_5
//  266 				scheduleMsg(logger, "calc %d: action %s value %f", i, action2String(calc.calcLogAction[i]), calc.calcLogValue[i]);
        LDR.W    R0,??DataTable16_29
        LDR      R0,[R0, R9, LSL #+2]
          CFI FunCall _Z13action2String11le_action_e
        BL       _Z13action2String11le_action_e
        MOV      R10,R0
        LDR.W    R0,??DataTable16_29
        ADDS     R0,R0,R9, LSL #+2
        LDR      R0,[R0, #+256]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        MOV      R3,R10
        MOV      R2,R9
        LDR.W    R1,??DataTable17_31
        LDR.W    R0,??DataTable17_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  267 			}
        ADDS     R9,R9,#+1
        B.N      ??setPinState_4
//  268 
//  269 			scheduleMsg(logger, "setPin %s %s", msg, value ? "on" : "off");
??setPinState_5:
        CMP      R8,#+0
        BEQ.N    ??setPinState_6
        ADR.N    R3,??DataTable13  ;; 0x6F, 0x6E, 0x00, 0x00
        B.N      ??setPinState_7
??setPinState_6:
        ADR.N    R3,??DataTable13_1  ;; "off"
??setPinState_7:
        MOVS     R2,R4
        LDR.W    R1,??DataTable17_32
        LDR.W    R0,??DataTable17_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  270 			pin->setValue(value);
        MOV      R1,R8
        MOVS     R0,R5
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
//  271 		}
//  272 	}
//  273 }
??setPinState_1:
??setPinState_2:
        POP      {R0,R1,R4-R10,PC}  ;; return
          CFI EndBlock cfiBlock10
//  274 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z8showFsioPKcP9LEElement
        THUMB
//  275 static void showFsio(const char *msg, LEElement *element) {
_Z8showFsioPKcP9LEElement:
        PUSH     {R1-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  276 	if (msg != NULL)
        CMP      R4,#+0
        BEQ.N    ??showFsio_0
//  277 		scheduleMsg(logger, "%s:", msg);
        MOVS     R2,R4
        ADR.N    R1,??DataTable13_2  ;; "%s:"
        LDR.W    R0,??DataTable17_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  278 	while (element != NULL) {
??showFsio_0:
        CMP      R5,#+0
        BEQ.N    ??showFsio_1
//  279 		scheduleMsg(logger, "action %d: fValue=%f iValue=%d", element->action, element->fValue, element->iValue);
        LDR      R0,[R5, #+8]
        STR      R0,[SP, #+8]
        LDR      R0,[R5, #+4]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        LDR      R2,[R5, #+0]
        LDR.W    R1,??DataTable17_33
        LDR.W    R0,??DataTable17_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  280 		element = element->next;
        LDR      R5,[R5, #+12]
        B.N      ??showFsio_0
//  281 	}
//  282 	scheduleMsg(logger, "<end>");
??showFsio_1:
        LDR.W    R1,??DataTable17_34
        LDR.W    R0,??DataTable17_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  283 }
        POP      {R0-R2,R4,R5,PC}  ;; return
          CFI EndBlock cfiBlock11

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10:
        DC8      "RPM"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable10_1:
        DC8      "CLT"
//  284 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _Z12showFsioInfov
        THUMB
//  285 static void showFsioInfo(void) {
_Z12showFsioInfov:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        VPUSH    {D8}
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        SUB      SP,SP,#+16
          CFI CFA R13+40
//  286 	scheduleMsg(logger, "sys used %d/user used %d", sysPool.getSize(), userPool.getSize());
        LDR.W    R0,??DataTable16_26
          CFI FunCall _ZN13LEElementPool7getSizeEv
        BL       _ZN13LEElementPool7getSizeEv
        MOVS     R4,R0
        LDR.W    R0,??DataTable16_24
          CFI FunCall _ZN13LEElementPool7getSizeEv
        BL       _ZN13LEElementPool7getSizeEv
        MOVS     R2,R0
        MOVS     R3,R4
        LDR.W    R1,??DataTable17_35
        LDR.W    R0,??DataTable17_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  287 	showFsio("a/c", acRelayLogic);
        LDR.W    R0,??DataTable17_36
        LDR      R1,[R0, #+0]
        ADR.N    R0,??DataTable14  ;; "a/c"
          CFI FunCall _Z8showFsioPKcP9LEElement
        BL       _Z8showFsioPKcP9LEElement
//  288 	showFsio("fuel", fuelPumpLogic);
        LDR.W    R0,??DataTable17_37
        LDR      R1,[R0, #+0]
        LDR.W    R0,??DataTable17_38
          CFI FunCall _Z8showFsioPKcP9LEElement
        BL       _Z8showFsioPKcP9LEElement
//  289 	showFsio("fan", radiatorFanLogic);
        LDR.W    R0,??DataTable17_39
        LDR      R1,[R0, #+0]
        ADR.N    R0,??DataTable11  ;; "fan"
          CFI FunCall _Z8showFsioPKcP9LEElement
        BL       _Z8showFsioPKcP9LEElement
//  290 	showFsio("alt", alternatorLogic);
        LDR.W    R0,??DataTable17_40
        LDR      R1,[R0, #+0]
        ADR.N    R0,??DataTable15  ;; "alt"
          CFI FunCall _Z8showFsioPKcP9LEElement
        BL       _Z8showFsioPKcP9LEElement
//  291 
//  292 	for (int i = 0; i < LE_COMMAND_COUNT; i++) {
        MOVS     R4,#+0
??showFsioInfo_0:
        CMP      R4,#+16
        BGE.N    ??showFsioInfo_1
//  293 		char * exp = config->le_formulas[i];
        MOVS     R0,#+200
        LDR.W    R1,??DataTable17_8
        LDR      R1,[R1, #+0]
        MLA      R0,R0,R4,R1
        ADDW     R5,R0,#+2216
//  294 		if (exp[0] != 0) {
        LDRB     R0,[R5, #+0]
        CMP      R0,#+0
        BEQ.N    ??showFsioInfo_2
//  295 			/**
//  296 			 * in case of FSIO user interface indexes are starting with 0, the argument for that
//  297 			 * is the fact that the target audience is more software developers
//  298 			 */
//  299 			scheduleMsg(logger, "FSIO #%d [%s] at %s@%dHz value=%f", i, exp,
//  300 					hwPortname(boardConfiguration->fsioPins[i]), boardConfiguration->fsioFrequency[i],
//  301 					engineConfiguration2->fsioLastValue[i]);
        LDR.W    R0,??DataTable17_5
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R4, LSL #+2
        LDR      R0,[R0, #+452]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R6,R0
        MOVW     R0,#+12048
        LDR.W    R1,??DataTable17_41
        LDR      R1,[R1, #+0]
        ADDS     R1,R1,R4, LSL #+2
        LDR      R0,[R0, R1]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+8]
        LDR.W    R0,??DataTable17_5
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R4, LSL #+1
        LDRSH    R0,[R0, #+616]
        STR      R0,[SP, #+4]
        STR      R6,[SP, #+0]
        MOVS     R3,R5
        MOVS     R2,R4
        LDR.W    R1,??DataTable17_42
        LDR.W    R0,??DataTable17_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  302 //			scheduleMsg(logger, "user-defined #%d value=%f", i, engine->engineConfiguration2->fsioLastValue[i]);
//  303 			showFsio(NULL, fsioLogics[i]);
        LDR.W    R0,??DataTable17_10
        LDR      R1,[R0, R4, LSL #+2]
        MOVS     R0,#+0
          CFI FunCall _Z8showFsioPKcP9LEElement
        BL       _Z8showFsioPKcP9LEElement
//  304 		}
//  305 	}
??showFsioInfo_2:
        ADDS     R4,R4,#+1
        B.N      ??showFsioInfo_0
//  306 	for (int i = 0; i < LE_COMMAND_COUNT; i++) {
??showFsioInfo_1:
        MOVS     R4,#+0
??showFsioInfo_3:
        CMP      R4,#+16
        BGE.N    ??showFsioInfo_4
//  307 		float v = boardConfiguration->fsio_setting[i];
        LDR.W    R0,??DataTable17_5
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R4, LSL #+2
        VLDR     S16,[R0, #+660]
//  308 		if (!cisnan(v)) {
        VMOV.F32 S0,S16
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BEQ.N    ??showFsioInfo_5
//  309 			scheduleMsg(logger, "user property #%d: %f", i + 1, v);
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        ADDS     R2,R4,#+1
        LDR.W    R1,??DataTable17_43
        LDR.W    R0,??DataTable17_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  310 		}
//  311 	}
??showFsioInfo_5:
        ADDS     R4,R4,#+1
        B.N      ??showFsioInfo_3
//  312 	for (int i = 0; i < LE_COMMAND_COUNT; i++) {
??showFsioInfo_4:
        MOVS     R4,#+0
??showFsioInfo_6:
        CMP      R4,#+16
        BGE.N    ??showFsioInfo_7
//  313 		brain_pin_e inputPin = boardConfiguration->fsioDigitalInputs[i];
        LDR.W    R0,??DataTable17_5
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R4, LSL #+2
        LDR      R5,[R0, #+804]
//  314 		if (inputPin != GPIO_UNASSIGNED) {
        CMP      R5,#+80
        BEQ.N    ??showFsioInfo_8
//  315 			scheduleMsg(logger, "FSIO digital input #%d: %s", i, hwPortname(inputPin));
        MOVS     R0,R5
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R3,R0
        MOVS     R2,R4
        LDR.W    R1,??DataTable17_44
        LDR.W    R0,??DataTable17_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  316 		}
//  317 	}
??showFsioInfo_8:
        ADDS     R4,R4,#+1
        B.N      ??showFsioInfo_6
//  318 
//  319 }
??showFsioInfo_7:
        ADD      SP,SP,#+16
          CFI CFA R13+24
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock12

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable11:
        DC8      "fan"
//  320 
//  321 /**
//  322  * set_fsio_setting 0 0.11
//  323  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z14setFsioSettingff
        THUMB
//  324 static void setFsioSetting(float indexF, float value) {
_Z14setFsioSettingff:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        VMOV.F32 S16,S0
        VMOV.F32 S17,S1
//  325 	int index = indexF;
        VCVT.S32.F32 S0,S16
        VMOV     R4,S0
//  326 	if (index < 0 || index >= LE_COMMAND_COUNT) {
        CMP      R4,#+0
        BMI.N    ??setFsioSetting_0
        CMP      R4,#+16
        BLT.N    ??setFsioSetting_1
//  327 		scheduleMsg(logger, "invalid index %d", index);
??setFsioSetting_0:
        MOVS     R2,R4
        LDR.W    R1,??DataTable17_2
        LDR.W    R0,??DataTable17_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  328 		return;
        B.N      ??setFsioSetting_2
//  329 	}
//  330 	engineConfiguration->bc.fsio_setting[index] = value;
??setFsioSetting_1:
        LDR.W    R0,??DataTable17_45
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R4, LSL #+2
        ADDW     R0,R0,#+1284
        VSTR     S17,[R0, #0]
//  331 	showFsioInfo();
          CFI FunCall _Z12showFsioInfov
        BL       _Z12showFsioInfov
//  332 }
??setFsioSetting_2:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock13
//  333 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _Z16setFsioFrequencyii
        THUMB
//  334 static void setFsioFrequency(int index, int frequency) {
_Z16setFsioFrequencyii:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  335 	index--;
        SUBS     R4,R4,#+1
//  336 	if (index < 0 || index >= LE_COMMAND_COUNT) {
        CMP      R4,#+0
        BMI.N    ??setFsioFrequency_0
        CMP      R4,#+16
        BLT.N    ??setFsioFrequency_1
//  337 		scheduleMsg(logger, "invalid index %d", index);
??setFsioFrequency_0:
        MOVS     R2,R4
        LDR.W    R1,??DataTable17_2
        LDR.W    R0,??DataTable17_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  338 		return;
        B.N      ??setFsioFrequency_2
//  339 	}
//  340 	boardConfiguration->fsioFrequency[index] = frequency;
??setFsioFrequency_1:
        LDR.W    R0,??DataTable17_5
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R4, LSL #+1
        STRH     R5,[R0, #+616]
//  341 	scheduleMsg(logger, "Setting FSIO frequency %d on #%d", frequency, index + 1);
        ADDS     R3,R4,#+1
        MOVS     R2,R5
        LDR.W    R1,??DataTable17_46
        LDR.W    R0,??DataTable17_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  342 }
??setFsioFrequency_2:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock14

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13:
        DC8      0x6F, 0x6E, 0x00, 0x00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_1:
        DC8      "off"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_2:
        DC8      "%s:"
//  343 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function _Z17setFsioExpressionPKcS0_P6Engine
        THUMB
//  344 static void setFsioExpression(const char *indexStr, const char *quotedLine, Engine *engine) {
_Z17setFsioExpressionPKcS0_P6Engine:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  345 	int index = atoi(indexStr) - 1;
        MOVS     R0,R4
          CFI FunCall atoi
        BL       atoi
        SUBS     R7,R0,#+1
//  346 	if (index < 0 || index >= LE_COMMAND_COUNT) {
        CMP      R7,#+0
        BMI.N    ??setFsioExpression_0
        CMP      R7,#+16
        BLT.N    ??setFsioExpression_1
//  347 		scheduleMsg(logger, "invalid index %d", index);
??setFsioExpression_0:
        MOVS     R2,R7
        LDR.W    R1,??DataTable17_2
        LDR.W    R0,??DataTable17_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  348 		return;
        B.N      ??setFsioExpression_2
//  349 	}
//  350 	char * l = unquote((char*) quotedLine);
??setFsioExpression_1:
        MOVS     R0,R5
          CFI FunCall unquote
        BL       unquote
        MOV      R8,R0
//  351 	if (strlen(l) > LE_COMMAND_LENGTH - 1) {
        MOV      R0,R8
          CFI FunCall strlen
        BL       strlen
        CMP      R0,#+200
        BCC.N    ??setFsioExpression_3
//  352 		scheduleMsg(logger, "Too long %d", strlen(l));
        MOV      R0,R8
          CFI FunCall strlen
        BL       strlen
        MOVS     R2,R0
        LDR.W    R1,??DataTable17_47
        LDR.N    R0,??DataTable17_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  353 		return;
        B.N      ??setFsioExpression_2
//  354 	}
//  355 
//  356 	scheduleMsg(logger, "setting user out #%d to [%s]", index + 1, l);
??setFsioExpression_3:
        MOV      R3,R8
        ADDS     R2,R7,#+1
        LDR.W    R1,??DataTable17_48
        LDR.N    R0,??DataTable17_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  357 	strcpy(engine->config->le_formulas[index], l);
        MOV      R1,R8
        MOVS     R0,#+200
        LDR      R2,[R6, #+32]
        MLA      R0,R0,R7,R2
        ADDW     R0,R0,#+2216
          CFI FunCall strcpy
        BL       strcpy
//  358 	// this would apply the changes
//  359 	applyFsioConfiguration(PASS_ENGINE_PARAMETER_F);
          CFI FunCall _Z22applyFsioConfigurationv
        BL       _Z22applyFsioConfigurationv
//  360 	showFsioInfo();
          CFI FunCall _Z12showFsioInfov
        BL       _Z12showFsioInfov
//  361 }
??setFsioExpression_2:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock15

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14:
        DC8      "a/c"
//  362 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function _Z4evalPcP6Engine
        THUMB
//  363 static void eval(char *line, Engine *engine) {
_Z4evalPcP6Engine:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        VPUSH    {D8}
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  364 	line = unquote(line);
        MOVS     R0,R4
          CFI FunCall unquote
        BL       unquote
        MOVS     R4,R0
//  365 	scheduleMsg(logger, "Parsing [%s]", line);
        MOVS     R2,R4
        LDR.W    R1,??DataTable17_49
        LDR.N    R0,??DataTable17_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  366 	evalPool.reset();
        LDR.N    R0,??DataTable16_22
          CFI FunCall _ZN13LEElementPool5resetEv
        BL       _ZN13LEElementPool5resetEv
//  367 	LEElement * e = evalPool.parseExpression(line);
        MOVS     R1,R4
        LDR.N    R0,??DataTable16_22
          CFI FunCall _ZN13LEElementPool15parseExpressionEPKc
        BL       _ZN13LEElementPool15parseExpressionEPKc
        MOVS     R6,R0
//  368 	if (e == NULL) {
        CMP      R6,#+0
        BNE.N    ??eval_0
//  369 		scheduleMsg(logger, "parsing failed");
        LDR.W    R1,??DataTable17_50
        LDR.N    R0,??DataTable17_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
        B.N      ??eval_1
//  370 	} else {
//  371 		float result = evalCalc.getValue2(e, engine);
??eval_0:
        MOVS     R2,R5
        MOVS     R1,R6
        LDR.N    R0,??DataTable16_19
          CFI FunCall _ZN12LECalculator9getValue2EP9LEElementP6Engine
        BL       _ZN12LECalculator9getValue2EP9LEElementP6Engine
        VMOV.F32 S16,S0
//  372 		scheduleMsg(logger, "Eval result: %f", result);
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable17_51
        LDR.N    R0,??DataTable17_3
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  373 	}
//  374 }
??eval_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock16

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15:
        DC8      "alt"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_1:
        DC32     leRpm

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable15_2:
        DC32     leTps
//  375 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function _Z7runFsiov
        THUMB
//  376 void runFsio(void) {
_Z7runFsiov:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  377 	for (int i = 0; i < LE_COMMAND_COUNT; i++) {
        MOVS     R4,#+0
??runFsio_0:
        CMP      R4,#+16
        BGE.N    ??runFsio_1
//  378 		handleFsio(engine, i);
        MOVS     R1,R4
        LDR.N    R0,??DataTable17_52
        LDR      R0,[R0, #+0]
          CFI FunCall _Z10handleFsioP6Enginei
        BL       _Z10handleFsioP6Enginei
//  379 	}
        ADDS     R4,R4,#+1
        B.N      ??runFsio_0
//  380 
//  381 #if EFI_FUEL_PUMP
//  382 	if (boardConfiguration->fuelPumpPin != GPIO_UNASSIGNED && engineConfiguration->isFuelPumpEnabled) {
??runFsio_1:
        LDR.N    R0,??DataTable17_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+24]
        CMP      R0,#+80
        BEQ.N    ??runFsio_2
        LDR.N    R0,??DataTable17_45
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1508]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+7
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+0
        BEQ.N    ??runFsio_2
//  383 		setPinState("pump", &enginePins.fuelPumpRelay, fuelPumpLogic, engine);
        LDR.N    R0,??DataTable17_52
        LDR      R3,[R0, #+0]
        LDR.N    R0,??DataTable17_37
        LDR      R2,[R0, #+0]
        LDR.N    R1,??DataTable17_53
        LDR.N    R0,??DataTable17_54
          CFI FunCall _Z11setPinStatePKcP9OutputPinP9LEElementP6Engine
        BL       _Z11setPinStatePKcP9OutputPinP9LEElementP6Engine
//  384 	}
//  385 #endif
//  386 
//  387 	/**
//  388 	 * main relay is always on if ECU is on, that's a good enough initial implementation
//  389 	 */
//  390 	if (boardConfiguration->mainRelayPin != GPIO_UNASSIGNED)
??runFsio_2:
        LDR.N    R0,??DataTable17_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+308]
        CMP      R0,#+80
        BEQ.N    ??runFsio_3
//  391 		enginePins.mainRelay.setValue(true);
        MOVS     R1,#+1
        LDR.N    R0,??DataTable17_55
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
//  392 
//  393 	enginePins.o2heater.setValue(engine->rpmCalculator.isRunning());
??runFsio_3:
        LDR.N    R0,??DataTable17_52
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN13RpmCalculator9isRunningEv
        BL       _ZN13RpmCalculator9isRunningEv
        MOVS     R1,R0
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR.N    R0,??DataTable17_56
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
//  394 
//  395 	if (boardConfiguration->acRelayPin != GPIO_UNASSIGNED) {
        LDR.N    R0,??DataTable17_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+608]
        CMP      R0,#+80
        BEQ.N    ??runFsio_4
//  396 		setPinState("A/C", &enginePins.acRelay, acRelayLogic, engine);
        LDR.N    R0,??DataTable17_52
        LDR      R3,[R0, #+0]
        LDR.N    R0,??DataTable17_36
        LDR      R2,[R0, #+0]
        LDR.N    R1,??DataTable17_57
        ADR.N    R0,??DataTable16  ;; "A/C"
          CFI FunCall _Z11setPinStatePKcP9OutputPinP9LEElementP6Engine
        BL       _Z11setPinStatePKcP9OutputPinP9LEElementP6Engine
//  397 	}
//  398 
//  399 //	if (boardConfiguration->alternatorControlPin != GPIO_UNASSIGNED) {
//  400 //		setPinState("alternator", &enginePins.alternatorField, alternatorLogic, engine);
//  401 //	}
//  402 
//  403 	if (boardConfiguration->fanPin != GPIO_UNASSIGNED) {
??runFsio_4:
        LDR.N    R0,??DataTable17_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+144]
        CMP      R0,#+80
        BEQ.N    ??runFsio_5
//  404 		setPinState("fan", &enginePins.fanRelay, radiatorFanLogic, engine);
        LDR.N    R0,??DataTable17_52
        LDR      R3,[R0, #+0]
        LDR.N    R0,??DataTable17_39
        LDR      R2,[R0, #+0]
        LDR.N    R1,??DataTable16_32
        ADR.N    R0,??DataTable17  ;; "fan"
          CFI FunCall _Z11setPinStatePKcP9OutputPinP9LEElementP6Engine
        BL       _Z11setPinStatePKcP9OutputPinP9LEElementP6Engine
//  405 	}
//  406 
//  407 }
??runFsio_5:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock17

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16:
        DC8      "A/C"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_1:
        DC32     leMaf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_2:
        DC32     `?<Constant "vbatt">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_3:
        DC32     leVBatt

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_4:
        DC32     leFan

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_5:
        DC32     `?<Constant "coolant">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_6:
        DC32     leCoolant

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_7:
        DC32     `?<Constant "ac_on_switch">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_8:
        DC32     leAcToggle

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_9:
        DC32     `?<Constant "fan_on_setting">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_10:
        DC32     leFanOnSetting

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_11:
        DC32     `?<Constant "fan_off_setting">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_12:
        DC32     leFanOffSetting

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_13:
        DC32     `?<Constant "time_since_boot">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_14:
        DC32     leTimeSinceBoot

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_15:
        DC32     `?<Constant "fsio_setting">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_16:
        DC32     leFsioSetting

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_17:
        DC32     `?<Constant "knock">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_18:
        DC32     leKnock

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_19:
        DC32     evalCalc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_20:
        DC32     _ZN9LEElementC1Ev

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_21:
        DC32     evalPoolElements

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_22:
        DC32     evalPool

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_23:
        DC32     sysElements

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_24:
        DC32     sysPool

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_25:
        DC32     userElements

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_26:
        DC32     userPool

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_27:
        DC32     _ZN9SimplePwmC1Ev

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_28:
        DC32     fsioPwm

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_29:
        DC32     calc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_30:
        DC32     _ZN9OutputPinC1Ev

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_31:
        DC32     `?<Constant "getLEValue">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_32:
        DC32     enginePins+0x10

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable16_33:
        DC32     `?<Constant "FSIO unexpected %d">`
//  408 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  409 static pin_output_mode_e defa = OM_DEFAULT;
defa:
        DS8 4
//  410 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function _Z12initFsioImplP7LoggingP6Engine
        THUMB
//  411 void initFsioImpl(Logging *sharedLogger, Engine *engine) {
_Z12initFsioImplP7LoggingP6Engine:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        SUB      SP,SP,#+8
          CFI CFA R13+32
        MOVS     R4,R0
        MOVS     R5,R1
//  412 	logger = sharedLogger;
        LDR.N    R0,??DataTable17_3
        STR      R4,[R0, #+0]
//  413 	for (int i = 0; i < LE_COMMAND_COUNT; i++) {
        MOVS     R0,#+0
??initFsioImpl_0:
        CMP      R0,#+16
        BGE.N    ??initFsioImpl_1
//  414 		fsioLogics[i] = NULL;
        LDR.N    R1,??DataTable17_10
        MOVS     R2,#+0
        STR      R2,[R1, R0, LSL #+2]
//  415 	}
        ADDS     R0,R0,#+1
        B.N      ??initFsioImpl_0
//  416 
//  417 
//  418 #if EFI_FUEL_PUMP
//  419 	fuelPumpLogic = sysPool.parseExpression(FUEL_PUMP_LOGIC);
??initFsioImpl_1:
        LDR.N    R1,??DataTable17_58
        LDR.N    R0,??DataTable17_59
          CFI FunCall _ZN13LEElementPool15parseExpressionEPKc
        BL       _ZN13LEElementPool15parseExpressionEPKc
        LDR.N    R1,??DataTable17_37
        STR      R0,[R1, #+0]
//  420 #endif
//  421 
//  422 	acRelayLogic = sysPool.parseExpression(AC_RELAY_LOGIC);
        LDR.N    R1,??DataTable17_60
        LDR.N    R0,??DataTable17_59
          CFI FunCall _ZN13LEElementPool15parseExpressionEPKc
        BL       _ZN13LEElementPool15parseExpressionEPKc
        LDR.N    R1,??DataTable17_36
        STR      R0,[R1, #+0]
//  423 	radiatorFanLogic = sysPool.parseExpression(FAN_CONTROL_LOGIC);
        LDR.N    R1,??DataTable17_61
        LDR.N    R0,??DataTable17_59
          CFI FunCall _ZN13LEElementPool15parseExpressionEPKc
        BL       _ZN13LEElementPool15parseExpressionEPKc
        LDR.N    R1,??DataTable17_39
        STR      R0,[R1, #+0]
//  424 
//  425 	alternatorLogic = sysPool.parseExpression(ALTERNATOR_LOGIC);
        LDR.N    R1,??DataTable17_62
        LDR.N    R0,??DataTable17_59
          CFI FunCall _ZN13LEElementPool15parseExpressionEPKc
        BL       _ZN13LEElementPool15parseExpressionEPKc
        LDR.N    R1,??DataTable17_40
        STR      R0,[R1, #+0]
//  426 
//  427 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//  428 	for (int i = 0; i < LE_COMMAND_COUNT; i++) {
        MOVS     R6,#+0
??initFsioImpl_2:
        CMP      R6,#+16
        BGE.N    ??initFsioImpl_3
//  429 		brain_pin_e brainPin = boardConfiguration->fsioPins[i];
        LDR.N    R0,??DataTable17_5
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R6, LSL #+2
        LDR      R7,[R0, #+452]
//  430 
//  431 		if (brainPin != GPIO_UNASSIGNED) {
        CMP      R7,#+80
        BEQ.N    ??initFsioImpl_4
//  432 			int frequency = boardConfiguration->fsioFrequency[i];
        LDR.N    R0,??DataTable17_5
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R6, LSL #+1
        LDRSH    R8,[R0, #+616]
//  433 			if (frequency == 0) {
        CMP      R8,#+0
        BNE.N    ??initFsioImpl_5
//  434 				outputPinRegisterExt2(getGpioPinName(i), &fsioOutputs[i], boardConfiguration->fsioPins[i], &defa);
        MOVS     R0,R6
          CFI FunCall _Z14getGpioPinNamei
        BL       _Z14getGpioPinNamei
        LDR.N    R3,??DataTable17_63
        LDR.N    R1,??DataTable17_5
        LDR      R1,[R1, #+0]
        ADDS     R1,R1,R6, LSL #+2
        LDR      R2,[R1, #+452]
        LDR.N    R1,??DataTable17_1
        ADDS     R1,R1,R6, LSL #+4
          CFI FunCall _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
        BL       _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
        B.N      ??initFsioImpl_4
//  435 			} else {
//  436 				startSimplePwmExt(&fsioPwm[i], "FSIO", brainPin, &fsioOutputs[i], frequency, 0.5f, applyPinState);
??initFsioImpl_5:
        LDR.N    R0,??DataTable17_64
        STR      R0,[SP, #+0]
        VMOV.F32 S1,#0.5
        VMOV     S0,R8
        VCVT.F32.S32 S0,S0
        LDR.N    R0,??DataTable17_1
        ADDS     R3,R0,R6, LSL #+4
        MOVS     R2,R7
        LDR.N    R1,??DataTable17_65
        MOVS     R0,#+112
        LDR.W    R12,??DataTable17_66
        MLA      R0,R0,R6,R12
          CFI FunCall _Z17startSimplePwmExtP9PwmConfigPKc11brain_pin_eP9OutputPinffPFvS0_iE
        BL       _Z17startSimplePwmExtP9PwmConfigPKc11brain_pin_eP9OutputPinffPFvS0_iE
//  437 			}
//  438 		}
//  439 	}
??initFsioImpl_4:
        ADDS     R6,R6,#+1
        B.N      ??initFsioImpl_2
//  440 
//  441 	for (int i = 0; i < LE_COMMAND_COUNT; i++) {
??initFsioImpl_3:
        MOVS     R6,#+0
??initFsioImpl_6:
        CMP      R6,#+16
        BGE.N    ??initFsioImpl_7
//  442 		brain_pin_e inputPin = boardConfiguration->fsioDigitalInputs[i];
        LDR.N    R0,??DataTable17_5
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R6, LSL #+2
        LDR      R7,[R0, #+804]
//  443 
//  444 		if (inputPin != GPIO_UNASSIGNED) {
        CMP      R7,#+80
        BEQ.N    ??initFsioImpl_8
//  445 			mySetPadMode2("FSIO input", inputPin, getInputMode(engineConfiguration->fsioInputModes[i]));
        LDR.N    R0,??DataTable17_45
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R6, LSL #+2
        LDR      R0,[R0, #+1556]
          CFI FunCall _Z12getInputMode16pin_input_mode_e
        BL       _Z12getInputMode16pin_input_mode_e
        MOVS     R2,R0
        MOVS     R1,R7
        LDR.N    R0,??DataTable17_67
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
//  446 		}
//  447 	}
??initFsioImpl_8:
        ADDS     R6,R6,#+1
        B.N      ??initFsioImpl_6
//  448 
//  449 #endif /* EFI_PROD_CODE */
//  450 
//  451 	addConsoleActionSSP("set_fsio_expression", (VoidCharPtrCharPtrVoidPtr) setFsioExpression, engine);
??initFsioImpl_7:
        MOVS     R2,R5
        LDR.N    R1,??DataTable17_68
        LDR.N    R0,??DataTable17_69
          CFI FunCall addConsoleActionSSP
        BL       addConsoleActionSSP
//  452 	addConsoleActionSS("set_fsio_output_pin", (VoidCharPtrCharPtr) setFsioOutputPin);
        LDR.N    R1,??DataTable17_70
        LDR.N    R0,??DataTable17_71
          CFI FunCall addConsoleActionSS
        BL       addConsoleActionSS
//  453 	addConsoleActionII("set_fsio_output_frequency", (VoidIntInt) setFsioFrequency);
        LDR.N    R1,??DataTable17_72
        LDR.N    R0,??DataTable17_73
          CFI FunCall addConsoleActionII
        BL       addConsoleActionII
//  454 
//  455 	addConsoleActionFF("set_fsio_setting", setFsioSetting);
        LDR.N    R1,??DataTable17_74
        LDR.N    R0,??DataTable17_75
          CFI FunCall addConsoleActionFF
        BL       addConsoleActionFF
//  456 
//  457 	addConsoleActionSS("set_fsio_input_pin", (VoidCharPtrCharPtr) setFsioInputPin);
        LDR.N    R1,??DataTable17_76
        LDR.N    R0,??DataTable17_77
          CFI FunCall addConsoleActionSS
        BL       addConsoleActionSS
//  458 	addConsoleAction("fsioinfo", showFsioInfo);
        LDR.N    R1,??DataTable17_78
        LDR.N    R0,??DataTable17_79
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  459 
//  460 	addConsoleActionSP("eval", (VoidCharPtrVoidPtr) eval, engine);
        MOVS     R2,R5
        LDR.N    R1,??DataTable17_80
        LDR.N    R0,??DataTable17_81
          CFI FunCall addConsoleActionSP
        BL       addConsoleActionSP
//  461 }
        POP      {R0,R1,R4-R8,PC}  ;; return
          CFI EndBlock cfiBlock18

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17:
        DC8      "fan"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_1:
        DC32     fsioOutputs

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_2:
        DC32     `?<Constant "invalid index %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_3:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_4:
        DC32     `?<Constant "invalid pin name [%s]">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_5:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_6:
        DC32     `?<Constant "FSIO input pin #%d [%s]">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_7:
        DC32     `?<Constant "FSIO output pin #%d [%s]">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_8:
        DC32     config

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_9:
        DC32     `?<Constant "parsing [%s]">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_10:
        DC32     fsioLogics

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_11:
        DC32     `?<Constant "GPIO_0">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_12:
        DC32     `?<Constant "GPIO_1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_13:
        DC32     `?<Constant "GPIO_10">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_14:
        DC32     `?<Constant "GPIO_11">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_15:
        DC32     `?<Constant "GPIO_12">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_16:
        DC32     `?<Constant "GPIO_13">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_17:
        DC32     `?<Constant "GPIO_14">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_18:
        DC32     `?<Constant "GPIO_15">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_19:
        DC32     `?<Constant "GPIO_2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_20:
        DC32     `?<Constant "GPIO_3">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_21:
        DC32     `?<Constant "GPIO_4">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_22:
        DC32     `?<Constant "GPIO_5">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_23:
        DC32     `?<Constant "GPIO_6">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_24:
        DC32     `?<Constant "GPIO_7">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_25:
        DC32     `?<Constant "GPIO_8">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_26:
        DC32     `?<Constant "GPIO_9">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_27:
        DC32     `?<Constant "fan_on">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_28:
        DC32     `?<Constant "fan_off">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_29:
        DC32     ??buffer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_30:
        DC32     `?<Constant "invalid expression fo...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_31:
        DC32     `?<Constant "calc %d: action %s va...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_32:
        DC32     `?<Constant "setPin %s %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_33:
        DC32     `?<Constant "action %d: fValue=%f ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_34:
        DC32     `?<Constant "<end>">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_35:
        DC32     `?<Constant "sys used %d/user used %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_36:
        DC32     acRelayLogic

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_37:
        DC32     fuelPumpLogic

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_38:
        DC32     `?<Constant "fuel">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_39:
        DC32     radiatorFanLogic

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_40:
        DC32     alternatorLogic

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_41:
        DC32     engineConfiguration2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_42:
        DC32     `?<Constant "FSIO #%d [%s] at %s@%...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_43:
        DC32     `?<Constant "user property #%d: %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_44:
        DC32     `?<Constant "FSIO digital input #%...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_45:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_46:
        DC32     `?<Constant "Setting FSIO frequenc...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_47:
        DC32     `?<Constant "Too long %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_48:
        DC32     `?<Constant "setting user out #%d ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_49:
        DC32     `?<Constant "Parsing [%s]">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_50:
        DC32     `?<Constant "parsing failed">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_51:
        DC32     `?<Constant "Eval result: %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_52:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_53:
        DC32     enginePins+0x30

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_54:
        DC32     `?<Constant "pump">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_55:
        DC32     enginePins

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_56:
        DC32     enginePins+0x40

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_57:
        DC32     enginePins+0x20

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_58:
        DC32     `?<Constant "time_since_boot 4 < r...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_59:
        DC32     sysPool

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_60:
        DC32     `?<Constant "ac_on_switch">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_61:
        DC32     `?<Constant "fan coolant fan_off_s...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_62:
        DC32     `?<Constant "vbatt 14.5 <">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_63:
        DC32     defa

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_64:
        DC32     _Z13applyPinStateP9PwmConfigi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_65:
        DC32     `?<Constant "FSIO">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_66:
        DC32     fsioPwm

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_67:
        DC32     `?<Constant "FSIO input">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_68:
        DC32     _Z17setFsioExpressionPKcS0_P6Engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_69:
        DC32     `?<Constant "set_fsio_expression">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_70:
        DC32     _Z16setFsioOutputPinPKcS0_

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_71:
        DC32     `?<Constant "set_fsio_output_pin">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_72:
        DC32     _Z16setFsioFrequencyii

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_73:
        DC32     `?<Constant "set_fsio_output_frequ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_74:
        DC32     _Z14setFsioSettingff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_75:
        DC32     `?<Constant "set_fsio_setting">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_76:
        DC32     _Z15setFsioInputPinPKcS0_

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_77:
        DC32     `?<Constant "set_fsio_input_pin">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_78:
        DC32     _Z12showFsioInfov

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_79:
        DC32     `?<Constant "fsioinfo">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_80:
        DC32     _Z4evalPcP6Engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable17_81:
        DC32     `?<Constant "eval">`

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
`?<Constant "getLEValue">`:
        DATA
        DC8 "getLEValue"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FSIO unexpected %d">`:
        DATA
        DC8 "FSIO unexpected %d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid index %d">`:
        DATA
        DC8 "invalid index %d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid pin name [%s]">`:
        DATA
        DC8 "invalid pin name [%s]"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FSIO input pin #%d [%s]">`:
        DATA
        DC8 "FSIO input pin #%d [%s]"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FSIO output pin #%d [%s]">`:
        DATA
        DC8 "FSIO output pin #%d [%s]"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "parsing [%s]">`:
        DATA
        DC8 "parsing [%s]"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIO_0">`:
        DATA
        DC8 "GPIO_0"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIO_1">`:
        DATA
        DC8 "GPIO_1"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIO_10">`:
        DATA
        DC8 "GPIO_10"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIO_11">`:
        DATA
        DC8 "GPIO_11"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIO_12">`:
        DATA
        DC8 "GPIO_12"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIO_13">`:
        DATA
        DC8 "GPIO_13"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIO_14">`:
        DATA
        DC8 "GPIO_14"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIO_15">`:
        DATA
        DC8 "GPIO_15"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIO_2">`:
        DATA
        DC8 "GPIO_2"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIO_3">`:
        DATA
        DC8 "GPIO_3"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIO_4">`:
        DATA
        DC8 "GPIO_4"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIO_5">`:
        DATA
        DC8 "GPIO_5"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIO_6">`:
        DATA
        DC8 "GPIO_6"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIO_7">`:
        DATA
        DC8 "GPIO_7"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIO_8">`:
        DATA
        DC8 "GPIO_8"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "GPIO_9">`:
        DATA
        DC8 "GPIO_9"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "RPM"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "CLT"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fan_on">`:
        DATA
        DC8 "fan_on"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fan_off">`:
        DATA
        DC8 "fan_off"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "fan"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid expression fo...">`:
        DATA
        DC8 "invalid expression for %s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "calc %d: action %s va...">`:
        DATA
        DC8 "calc %d: action %s value %f"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "setPin %s %s">`:
        DATA
        DC8 "setPin %s %s"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "on"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "off"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "%s:"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "action %d: fValue=%f ...">`:
        DATA
        DC8 "action %d: fValue=%f iValue=%d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "<end>">`:
        DATA
        DC8 "<end>"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "sys used %d/user used %d">`:
        DATA
        DC8 "sys used %d/user used %d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "a/c"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fuel">`:
        DATA
        DC8 "fuel"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "alt"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FSIO #%d [%s] at %s@%...">`:
        DATA
        DC8 "FSIO #%d [%s] at %s@%dHz value=%f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "user property #%d: %f">`:
        DATA
        DC8 "user property #%d: %f"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FSIO digital input #%...">`:
        DATA
        DC8 "FSIO digital input #%d: %s"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Setting FSIO frequenc...">`:
        DATA
        DC8 "Setting FSIO frequency %d on #%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Too long %d">`:
        DATA
        DC8 "Too long %d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "setting user out #%d ...">`:
        DATA
        DC8 "setting user out #%d to [%s]"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Parsing [%s]">`:
        DATA
        DC8 "Parsing [%s]"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "parsing failed">`:
        DATA
        DC8 "parsing failed"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Eval result: %f">`:
        DATA
        DC8 "Eval result: %f"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "pump">`:
        DATA
        DC8 "pump"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "A/C"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "time_since_boot 4 < r...">`:
        DATA
        DC8 "time_since_boot 4 < rpm 0 > OR"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ac_on_switch">`:
        DATA
        DC8 "ac_on_switch"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fan coolant fan_off_s...">`:
        DATA
        DC8 66H, 61H, 6EH, 20H, 63H, 6FH, 6FH, 6CH
        DC8 61H, 6EH, 74H, 20H, 66H, 61H, 6EH, 5FH
        DC8 6FH, 66H, 66H, 5FH, 73H, 65H, 74H, 74H
        DC8 69H, 6EH, 67H, 20H, 3EH, 20H, 26H, 20H
        DC8 63H, 6FH, 6FH, 6CH, 61H, 6EH, 74H, 20H
        DC8 66H, 61H, 6EH, 5FH, 6FH, 6EH, 5FH, 73H
        DC8 65H, 74H, 74H, 69H, 6EH, 67H, 20H, 3EH
        DC8 20H, 4FH, 52H, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "vbatt 14.5 <">`:
        DATA
        DC8 "vbatt 14.5 <"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FSIO">`:
        DATA
        DC8 "FSIO"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FSIO input">`:
        DATA
        DC8 "FSIO input"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_fsio_expression">`:
        DATA
        DC8 "set_fsio_expression"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_fsio_output_pin">`:
        DATA
        DC8 "set_fsio_output_pin"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_fsio_output_frequ...">`:
        DATA
        DC8 "set_fsio_output_frequency"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_fsio_setting">`:
        DATA
        DC8 "set_fsio_setting"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "set_fsio_input_pin">`:
        DATA
        DC8 "set_fsio_input_pin"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fsioinfo">`:
        DATA
        DC8 "fsioinfo"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "eval">`:
        DATA
        DC8 "eval"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "rpm"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "tps"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "maf"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "vbatt">`:
        DATA
        DC8 "vbatt"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "coolant">`:
        DATA
        DC8 "coolant"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fan_on_setting">`:
        DATA
        DC8 "fan_on_setting"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fan_off_setting">`:
        DATA
        DC8 "fan_off_setting"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "time_since_boot">`:
        DATA
        DC8 "time_since_boot"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fsio_setting">`:
        DATA
        DC8 "fsio_setting"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "knock">`:
        DATA
        DC8 "knock"
        DC8 0, 0

        END
//  462 
//  463 #endif
//  464 
//  465 #endif /* EFI_FSIO */
// 
// 6 384 bytes in section .bss
// 1 856 bytes in section .ccm
//     4 bytes in section .init_array
// 1 068 bytes in section .rodata
// 3 268 bytes in section .text
// 
// 3 272 bytes of CODE  memory
// 1 068 bytes of CONST memory
// 8 240 bytes of DATA  memory
//
//Errors: none
//Warnings: 6
