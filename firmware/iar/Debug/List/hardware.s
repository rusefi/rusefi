///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:00 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\hardwa /
//                       re.cpp                                               /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\hardwa /
//                       re.cpp -lCN F:\stuff\rusefi_sourceforge\firmware\iar /
//                       \Debug\List\ -lA F:\stuff\rusefi_sourceforge\firmwar /
//                       e\iar\Debug\List\ -o F:\stuff\rusefi_sourceforge\fir /
//                       mware\iar\Debug\Obj\ --no_cse --no_unroll            /
//                       --no_inline --no_code_motion --no_tbaa               /
//                       --no_clustering --no_scheduling --debug              /
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
//                       hardware.s                                           /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME hardware

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN I2CD1
        EXTERN SPID1
        EXTERN SPID2
        EXTERN SPID3
        EXTERN _Z11initHip9011P7Logging
        EXTERN _Z12initJoystickP7Logging
        EXTERN _Z12initMax31855P7LoggingP9SPIDriverP11brain_pin_e
        EXTERN _Z12mySetPadModePKcP12GPIO_TypeDefjj
        EXTERN _Z13initAdcInputsb
        EXTERN _Z13initBoardTestv
        EXTERN _Z13mySetPadMode2PKc11brain_pin_ej
        EXTERN _Z13readFromFlashv
        EXTERN _Z14hipAdcCallbackt
        EXTERN _Z14initOutputPinsv
        EXTERN _Z15initPrimaryPinsv
        EXTERN _Z15writeToFlashNowv
        EXTERN _Z16initVehicleSpeedP7Logging
        EXTERN _Z16stopIgnitionPinsv
        EXTERN _Z17initWboControllerv
        EXTERN _Z17startIgnitionPinsv
        EXTERN _Z17stopInjectionPinsv
        EXTERN _Z18initDataStructuresv
        EXTERN _Z18initTriggerCentralP7LoggingP6Engine
        EXTERN _Z18initTriggerDecoderv
        EXTERN _Z18startInjectionPinsv
        EXTERN _Z20mapAveragingCallbackt
        EXTERN _Z21resetConfigurationExtP7Logging13engine_type_e
        EXTERN _Z22turnOnTriggerInputPinsP7Logging
        EXTERN _Z24applyNewTriggerInputPinsv
        EXTERN _Z24initTriggerDecoderLoggerP7Logging
        EXTERN _Z7initCanv
        EXTERN _Z7initRtcv
        EXTERN _Z8getHwPin11brain_pin_e
        EXTERN _Z9getHwPort11brain_pin_e
        EXTERN _Z9initFlashP7Logging
        EXTERN _Z9turnOnSpi12spi_device_e
        EXTERN _ZN6Engine24addConfigurationListenerEPFvPS_E
        EXTERN _pal_lld_setgroupmode
        EXTERN addConsoleActionII
        EXTERN boardConfiguration
        EXTERN chMtxInit
        EXTERN chMtxLock
        EXTERN chMtxUnlock
        EXTERN engine
        EXTERN engineConfiguration
        EXTERN fastAdc
        EXTERN firmwareError
        EXTERN getRemainingStack
        EXTERN hasFirmwareErrorFlag
        EXTERN i2cInit
        EXTERN i2cStart
        EXTERN initGps
        EXTERN initMmcCard
        EXTERN lcd_HD44780_init
        EXTERN lcd_HD44780_print_string
        EXTERN print
        EXTERN printMsg
        EXTERN rlist
        EXTERN tpsFastAdc

        PUBLIC _Z12getSpiDevice12spi_device_e
        PUBLIC _Z12initHardwareP7Logging
        PUBLIC _Z13initI2Cmodulev
        PUBLIC _Z14turnOnHardwareP7Logging
        PUBLIC _Z17adc_callback_fastP9ADCDriverPtj
        PUBLIC _Z24applyNewHardwareSettingsv
        PUBLIC _Z7lockSpi12spi_device_e
        PUBLIC _Z9unlockSpiv
        PUBLIC maxNesting
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\hw_layer\hardware.cpp
//    1 /**
//    2  * @file    hardware.cpp
//    3  * @brief   Hardware package entry point
//    4  *
//    5  * @date May 27, 2013
//    6  * @author Andrey Belomutskiy, (c) 2012-2015
//    7  */
//    8 
//    9 #include "main.h"
//   10 #include "adc_inputs.h"
//   11 #include "can_hw.h"
//   12 #include "console_io.h"
//   13 #include "hardware.h"
//   14 #include "io_pins.h"
//   15 #include "pin_repository.h"
//   16 #include "rtc_helper.h"
//   17 #include "rfiutil.h"
//   18 #include "injector_central.h"
//   19 #include "vehicle_speed.h"
//   20 
//   21 #include "trigger_input.h"
//   22 #include "eficonsole.h"
//   23 #include "max31855.h"
//   24 
//   25 #if EFI_PROD_CODE
//   26 #include "AdcConfiguration.h"
//   27 #include "board_test.h"
//   28 #include "mcp3208.h"
//   29 #include "HIP9011.h"
//   30 #include "histogram.h"
//   31 #include "mmc_card.h"
//   32 #include "neo6m.h"
//   33 #include "lcd_HD44780.h"
//   34 #include "settings.h"
//   35 #include "algo.h"
//   36 #include "joystick.h"
//   37 #include "trigger_central.h"
//   38 #include "svnversion.h"
//   39 #include "engine_configuration.h"
//   40 #include "wbo.h"
//   41 #endif
//   42 
//   43 #if EFI_SPEED_DENSITY
//   44 #include "map_averaging.h"
//   45 #endif /* EFI_SPEED_DENSITY */
//   46 
//   47 #if EFI_INTERNAL_FLASH
//   48 #include "flash_main.h"
//   49 #endif /* EFI_INTERNAL_FLASH */
//   50 
//   51 EXTERN_ENGINE
//   52 ;
//   53 extern bool hasFirmwareErrorFlag;
//   54 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   55 static Mutex spiMtx;
spiMtx:
        DS8 16
//   56 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   57 int maxNesting = 0;
maxNesting:
        DS8 4
//   58 
//   59 #if HAL_USE_SPI || defined(__DOXYGEN__)
//   60 static bool isSpiInitialized[5] = { false, false, false, false, false };
//   61 
//   62 /**
//   63  * Only one consumer can use SPI bus at a given time
//   64  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z7lockSpi12spi_device_e
        THUMB
//   65 void lockSpi(spi_device_e device) {
_Z7lockSpi12spi_device_e:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   66 	efiAssertVoid(getRemainingStack(chThdSelf()) > 128, "lockSpi");
        LDR.W    R0,??DataTable5_1
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+129
        BGE.N    ??lockSpi_0
        LDR.W    R0,??DataTable5_2
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??lockSpi_1
//   67 	// todo: different locks for different SPI devices!
//   68 	chMtxLock(&spiMtx);
??lockSpi_0:
        LDR.W    R0,??DataTable5_3
          CFI FunCall chMtxLock
        BL       chMtxLock
//   69 }
??lockSpi_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0
//   70 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z9unlockSpiv
        THUMB
//   71 void unlockSpi(void) {
_Z9unlockSpiv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   72 	chMtxUnlock();
          CFI FunCall chMtxUnlock
        BL       chMtxUnlock
//   73 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock1
//   74 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z14initSpiModulesP21board_configuration_s
        THUMB
//   75 static void initSpiModules(board_configuration_s *boardConfiguration) {
_Z14initSpiModulesP21board_configuration_s:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   76 	if (boardConfiguration->is_enabled_spi_2) {
        LDRB     R0,[R4, #+384]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+1
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??initSpiModules_0
//   77 		turnOnSpi(SPI_DEVICE_2);
        MOVS     R0,#+2
          CFI FunCall _Z9turnOnSpi12spi_device_e
        BL       _Z9turnOnSpi12spi_device_e
//   78 	}
//   79 	if (boardConfiguration->is_enabled_spi_3) {
??initSpiModules_0:
        LDRB     R0,[R4, #+384]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+2
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??initSpiModules_1
//   80 		turnOnSpi(SPI_DEVICE_3);
        MOVS     R0,#+3
          CFI FunCall _Z9turnOnSpi12spi_device_e
        BL       _Z9turnOnSpi12spi_device_e
//   81 	}
//   82 }
??initSpiModules_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2
//   83 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z12getSpiDevice12spi_device_e
        THUMB
//   84 SPIDriver * getSpiDevice(spi_device_e spiDevice) {
_Z12getSpiDevice12spi_device_e:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   85 	if (spiDevice == SPI_NONE) {
        CMP      R4,#+0
        BNE.N    ??getSpiDevice_0
//   86 		return NULL;
        MOVS     R0,#+0
        B.N      ??getSpiDevice_1
//   87 	}
//   88 #if STM32_SPI_USE_SPI1 || defined(__DOXYGEN__)
//   89 	if (spiDevice == SPI_DEVICE_1) {
??getSpiDevice_0:
        CMP      R4,#+1
        BNE.N    ??getSpiDevice_2
//   90 		return &SPID1;
        LDR.N    R0,??DataTable5_4
        B.N      ??getSpiDevice_1
//   91 	}
//   92 #endif
//   93 #if STM32_SPI_USE_SPI2 || defined(__DOXYGEN__)
//   94 	if (spiDevice == SPI_DEVICE_2) {
??getSpiDevice_2:
        CMP      R4,#+2
        BNE.N    ??getSpiDevice_3
//   95 		return &SPID2;
        LDR.N    R0,??DataTable5_5
        B.N      ??getSpiDevice_1
//   96 	}
//   97 #endif
//   98 #if STM32_SPI_USE_SPI3 || defined(__DOXYGEN__)
//   99 	if (spiDevice == SPI_DEVICE_3) {
??getSpiDevice_3:
        CMP      R4,#+3
        BNE.N    ??getSpiDevice_4
//  100 		return &SPID3;
        LDR.N    R0,??DataTable5_6
        B.N      ??getSpiDevice_1
//  101 	}
//  102 #endif
//  103 	firmwareError("Unexpected SPI device: %d", spiDevice);
??getSpiDevice_4:
        MOVS     R1,R4
        LDR.N    R0,??DataTable5_7
          CFI FunCall firmwareError
        BL       firmwareError
//  104 	return NULL;
        MOVS     R0,#+0
??getSpiDevice_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//  105 }
//  106 #endif
//  107 
//  108 #if HAL_USE_I2C || defined(__DOXYGEN__)

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//  109 static I2CConfig i2cfg = { OPMODE_I2C, 100000, STD_DUTY_CYCLE, };
i2cfg:
        DATA
        DC8 1, 0, 0, 0
        DC32 100000
        DC8 1, 0, 0, 0
//  110 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z13initI2Cmodulev
        THUMB
//  111 void initI2Cmodule(void) {
_Z13initI2Cmodulev:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  112 	print("Starting I2C module\r\n");
        LDR.N    R0,??DataTable5_8
          CFI FunCall print
        BL       print
//  113 	i2cInit();
          CFI FunCall i2cInit
        BL       i2cInit
//  114 	i2cStart(&I2CD1, &i2cfg);
        LDR.N    R1,??DataTable5_9
        LDR.N    R0,??DataTable5_10
          CFI FunCall i2cStart
        BL       i2cStart
//  115 
//  116 	mySetPadMode("I2C clock", EFI_I2C_SCL_PORT, EFI_I2C_SCL_PIN,
//  117 	PAL_MODE_ALTERNATE(EFI_I2C_AF) | PAL_STM32_OTYPE_OPENDRAIN);
        MOVW     R3,#+518
        MOVS     R2,#+6
        LDR.N    R1,??DataTable5_11  ;; 0x40020400
        LDR.N    R0,??DataTable5_12
          CFI FunCall _Z12mySetPadModePKcP12GPIO_TypeDefjj
        BL       _Z12mySetPadModePKcP12GPIO_TypeDefjj
//  118 	mySetPadMode("I2C data", EFI_I2C_SDA_PORT, EFI_I2C_SDA_PIN,
//  119 	PAL_MODE_ALTERNATE(EFI_I2C_AF) | PAL_STM32_OTYPE_OPENDRAIN);
        MOVW     R3,#+518
        MOVS     R2,#+7
        LDR.N    R1,??DataTable5_11  ;; 0x40020400
        LDR.N    R0,??DataTable5_13
          CFI FunCall _Z12mySetPadModePKcP12GPIO_TypeDefjj
        BL       _Z12mySetPadModePKcP12GPIO_TypeDefjj
//  120 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock4
//  121 
//  122 //static char txbuf[1];
//  123 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z11sendI2Cbyteii
          CFI NoCalls
        THUMB
//  124 static void sendI2Cbyte(int addr, int data) {
//  125 //	i2cAcquireBus(&I2CD1);
//  126 //	txbuf[0] = data;
//  127 //	i2cMasterTransmit(&I2CD1, addr, txbuf, 1, NULL, 0);
//  128 //	i2cReleaseBus(&I2CD1);
//  129 }
_Z11sendI2Cbyteii:
        BX       LR               ;; return
          CFI EndBlock cfiBlock5
//  130 
//  131 #endif
//  132 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  133 static Logging *sharedLogger;
sharedLogger:
        DS8 4
//  134 
//  135 #if EFI_PROD_CODE
//  136 
//  137 extern AdcDevice fastAdc;
//  138 
//  139 #define TPS_IS_SLOW -1
//  140 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  141 static int fastMapSampleIndex;
fastMapSampleIndex:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  142 static int hipSampleIndex;
hipSampleIndex:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  143 static int tpsSampleIndex;
tpsSampleIndex:
        DS8 4
//  144 
//  145 extern int tpsFastAdc;
//  146 
//  147 /**
//  148  * This method is not in the adc* lower-level file because it is more business logic then hardware.
//  149  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z17adc_callback_fastP9ADCDriverPtj
        THUMB
//  150 void adc_callback_fast(ADCDriver *adcp, adcsample_t *buffer, size_t n) {
_Z17adc_callback_fastP9ADCDriverPtj:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  151 	efiAssertVoid(getRemainingStack(chThdSelf()) > 64, "lowstck12a");
        LDR.N    R0,??DataTable5_1
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+65
        BGE.N    ??adc_callback_fast_0
        LDR.N    R0,??DataTable5_14
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??adc_callback_fast_1
//  152 
//  153 	(void) buffer;
//  154 	(void) n;
//  155 	/**
//  156 	 * Note, only in the ADC_COMPLETE state because the ADC driver fires an
//  157 	 * intermediate callback when the buffer is half full.
//  158 	 * */
//  159 	if (adcp->state == ADC_COMPLETE) {
??adc_callback_fast_0:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+4
        BNE.N    ??adc_callback_fast_2
//  160 		/**
//  161 		 * this callback is executed 10 000 times a second, it needs to be as fast as possible
//  162 		 */
//  163 		efiAssertVoid(getRemainingStack(chThdSelf()) > 128, "lowstck#9b");
        LDR.N    R0,??DataTable5_1
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+129
        BGE.N    ??adc_callback_fast_3
        LDR.N    R0,??DataTable5_15
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??adc_callback_fast_1
//  164 
//  165 #if EFI_MAP_AVERAGING
//  166 		mapAveragingCallback(fastAdc.samples[fastMapSampleIndex]);
??adc_callback_fast_3:
        LDR.N    R0,??DataTable5_16
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable5_17
        ADDS     R0,R1,R0, LSL #+1
        LDRH     R0,[R0, #+88]
          CFI FunCall _Z20mapAveragingCallbackt
        BL       _Z20mapAveragingCallbackt
//  167 #endif /* EFI_MAP_AVERAGING */
//  168 #if EFI_HIP_9011 || defined(__DOXYGEN__)
//  169 		if (boardConfiguration->isHip9011Enabled) {
        LDR.N    R0,??DataTable5_18
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+6
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??adc_callback_fast_4
//  170 			hipAdcCallback(fastAdc.samples[hipSampleIndex]);
        LDR.N    R0,??DataTable5_19
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable5_17
        ADDS     R0,R1,R0, LSL #+1
        LDRH     R0,[R0, #+88]
          CFI FunCall _Z14hipAdcCallbackt
        BL       _Z14hipAdcCallbackt
//  171 		}
//  172 #endif
//  173 		if(tpsSampleIndex!=TPS_IS_SLOW) {
??adc_callback_fast_4:
        LDR.N    R0,??DataTable5_20
        LDR      R0,[R0, #+0]
        CMN      R0,#+1
        BEQ.N    ??adc_callback_fast_2
//  174 			tpsFastAdc = fastAdc.samples[tpsSampleIndex];
        LDR.N    R0,??DataTable5_20
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable5_17
        ADDS     R0,R1,R0, LSL #+1
        LDRH     R0,[R0, #+88]
        LDR.N    R1,??DataTable5_21
        STR      R0,[R1, #+0]
//  175 		}
//  176 	}
//  177 }
??adc_callback_fast_2:
??adc_callback_fast_1:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock6
//  178 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z18calcFastAdcIndexesv
          CFI NoCalls
        THUMB
//  179 static void calcFastAdcIndexes(void) {
//  180 	fastMapSampleIndex = fastAdc.internalAdcIndexByHardwareIndex[engineConfiguration->map.sensor.hwChannel];
_Z18calcFastAdcIndexesv:
        LDR.N    R0,??DataTable5_22
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+252]
        LDR.N    R1,??DataTable5_17
        LDR      R0,[R1, R0, LSL #+2]
        LDR.N    R1,??DataTable5_16
        STR      R0,[R1, #+0]
//  181 	hipSampleIndex =
//  182 			engineConfiguration->hipOutputChannel == EFI_ADC_NONE ?
//  183 					-1 : fastAdc.internalAdcIndexByHardwareIndex[engineConfiguration->hipOutputChannel];
        LDR.N    R0,??DataTable5_22
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+1500]
        CMP      R0,#+16
        BNE.N    ??calcFastAdcIndexes_0
        LDR.N    R0,??DataTable5_19
        MOVS     R1,#-1
        STR      R1,[R0, #+0]
        B.N      ??calcFastAdcIndexes_1
??calcFastAdcIndexes_0:
        LDR.N    R0,??DataTable5_22
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+1500]
        LDR.N    R1,??DataTable5_17
        LDR      R0,[R1, R0, LSL #+2]
        LDR.N    R1,??DataTable5_19
        STR      R0,[R1, #+0]
//  184 	if (engineConfiguration->tpsAdcChannel != EFI_ADC_NONE) {
??calcFastAdcIndexes_1:
        LDR.N    R0,??DataTable5_22
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+524]
        CMP      R0,#+16
        BEQ.N    ??calcFastAdcIndexes_2
//  185 		tpsSampleIndex = fastAdc.internalAdcIndexByHardwareIndex[engineConfiguration->tpsAdcChannel];
        LDR.N    R0,??DataTable5_22
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+524]
        LDR.N    R1,??DataTable5_17
        LDR      R0,[R1, R0, LSL #+2]
        LDR.N    R1,??DataTable5_20
        STR      R0,[R1, #+0]
        B.N      ??calcFastAdcIndexes_3
//  186 	} else {
//  187 		tpsSampleIndex = TPS_IS_SLOW;
??calcFastAdcIndexes_2:
        LDR.N    R0,??DataTable5_20
        MOVS     R1,#-1
        STR      R1,[R0, #+0]
//  188 	}
//  189 }
??calcFastAdcIndexes_3:
        BX       LR               ;; return
          CFI EndBlock cfiBlock7
//  190 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z17adcConfigListenerP6Engine
        THUMB
//  191 static void adcConfigListener(Engine *engine) {
_Z17adcConfigListenerP6Engine:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  192 	calcFastAdcIndexes();
          CFI FunCall _Z18calcFastAdcIndexesv
        BL       _Z18calcFastAdcIndexesv
//  193 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock8
//  194 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z14turnOnHardwareP7Logging
        THUMB
//  195 void turnOnHardware(Logging *sharedLogger) {
_Z14turnOnHardwareP7Logging:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  196 #if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
//  197 	turnOnTriggerInputPins(sharedLogger);
        MOVS     R0,R4
          CFI FunCall _Z22turnOnTriggerInputPinsP7Logging
        BL       _Z22turnOnTriggerInputPinsP7Logging
//  198 #endif /* EFI_SHAFT_POSITION_INPUT */
//  199 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock9
//  200 
//  201 extern engine_configuration_s activeConfiguration;
//  202 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z24applyNewHardwareSettingsv
        THUMB
//  203 void applyNewHardwareSettings(void) {
_Z24applyNewHardwareSettingsv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  204 #if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
//  205 	applyNewTriggerInputPins();
          CFI FunCall _Z24applyNewTriggerInputPinsv
        BL       _Z24applyNewTriggerInputPinsv
//  206 #endif /* EFI_SHAFT_POSITION_INPUT */
//  207 	stopInjectionPins();
          CFI FunCall _Z17stopInjectionPinsv
        BL       _Z17stopInjectionPinsv
//  208 	stopIgnitionPins();
          CFI FunCall _Z16stopIgnitionPinsv
        BL       _Z16stopIgnitionPinsv
//  209 
//  210 	startInjectionPins();
          CFI FunCall _Z18startInjectionPinsv
        BL       _Z18startInjectionPinsv
//  211 	startIgnitionPins();
          CFI FunCall _Z17startIgnitionPinsv
        BL       _Z17startIgnitionPinsv
//  212 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock10
//  213 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z12initHardwareP7Logging
        THUMB
//  214 void initHardware(Logging *l) {
_Z12initHardwareP7Logging:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
//  215 	sharedLogger = l;
        LDR.N    R0,??DataTable5_23
        STR      R4,[R0, #+0]
//  216 	engine_configuration_s *engineConfiguration = engine->engineConfiguration;
        LDR.N    R0,??DataTable5_24
        LDR      R0,[R0, #+0]
        LDR      R5,[R0, #+36]
//  217 	efiAssertVoid(engineConfiguration!=NULL, "engineConfiguration");
        CMP      R5,#+0
        BNE.N    ??initHardware_0
        LDR.N    R0,??DataTable5_25
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??initHardware_1
//  218 	board_configuration_s *boardConfiguration = &engineConfiguration->bc;
??initHardware_0:
        ADDS     R6,R5,#+624
//  219 
//  220 	printMsg(sharedLogger, "initHardware()");
        LDR.N    R1,??DataTable5_26
        LDR.N    R0,??DataTable5_23
        LDR      R0,[R0, #+0]
          CFI FunCall printMsg
        BL       printMsg
//  221 	// todo: enable protection. it's disabled because it takes
//  222 	// 10 extra seconds to re-flash the chip
//  223 	//flashProtect();
//  224 
//  225 	chMtxInit(&spiMtx);
        LDR.N    R0,??DataTable5_3
          CFI FunCall chMtxInit
        BL       chMtxInit
//  226 
//  227 #if EFI_HISTOGRAMS
//  228 	/**
//  229 	 * histograms is a data structure for CPU monitor, it does not depend on configuration
//  230 	 */
//  231 	initHistogramsModule();
//  232 #endif /* EFI_HISTOGRAMS */
//  233 
//  234 #if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
//  235 	/**
//  236 	 * This is so early because we want to init logger
//  237 	 * which would be used while finding trigger synch index
//  238 	 * while reading configuration
//  239 	 */
//  240 	initTriggerDecoderLogger(sharedLogger);
        LDR.N    R0,??DataTable5_23
        LDR      R0,[R0, #+0]
          CFI FunCall _Z24initTriggerDecoderLoggerP7Logging
        BL       _Z24initTriggerDecoderLoggerP7Logging
//  241 #endif
//  242 
//  243 	/**
//  244 	 * We need the LED_ERROR pin even before we read configuration
//  245 	 */
//  246 	initPrimaryPins();
          CFI FunCall _Z15initPrimaryPinsv
        BL       _Z15initPrimaryPinsv
//  247 
//  248 	if (hasFirmwareError()) {
        LDR.N    R0,??DataTable5_27
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BNE.W    ??initHardware_1
//  249 		return;
//  250 	}
//  251 
//  252 	initDataStructures(PASS_ENGINE_PARAMETER_F);
??initHardware_2:
          CFI FunCall _Z18initDataStructuresv
        BL       _Z18initDataStructuresv
//  253 
//  254 #if EFI_INTERNAL_FLASH
//  255 
//  256 	palSetPadMode(CONFIG_RESET_SWITCH_PORT, CONFIG_RESET_SWITCH_PIN, PAL_MODE_INPUT_PULLUP);
        MOVS     R2,#+32
        MOVS     R1,#+64
        LDR.N    R0,??DataTable5_28  ;; 0x40020c00
          CFI FunCall _pal_lld_setgroupmode
        BL       _pal_lld_setgroupmode
//  257 
//  258 	initFlash(sharedLogger);
        LDR.N    R0,??DataTable5_23
        LDR      R0,[R0, #+0]
          CFI FunCall _Z9initFlashP7Logging
        BL       _Z9initFlashP7Logging
//  259 	/**
//  260 	 * this call reads configuration from flash memory or sets default configuration
//  261 	 * if flash state does not look right.
//  262 	 */
//  263 	if (SHOULD_INGORE_FLASH()) {
        LDR.N    R0,??DataTable5_29  ;; 0x40020c10
        LDR      R0,[R0, #+0]
        UBFX     R0,R0,#+6,#+1
        CMP      R0,#+0
        BNE.N    ??initHardware_3
//  264 		engineConfiguration->engineType = DEFAULT_ENGINE_TYPE;
        MOVS     R0,#+0
        STR      R0,[R5, #+0]
//  265 		resetConfigurationExt(sharedLogger, engineConfiguration->engineType PASS_ENGINE_PARAMETER);
        LDR      R1,[R5, #+0]
        LDR.N    R0,??DataTable5_23
        LDR      R0,[R0, #+0]
          CFI FunCall _Z21resetConfigurationExtP7Logging13engine_type_e
        BL       _Z21resetConfigurationExtP7Logging13engine_type_e
//  266 		writeToFlashNow();
          CFI FunCall _Z15writeToFlashNowv
        BL       _Z15writeToFlashNowv
        B.N      ??initHardware_4
//  267 	} else {
//  268 		readFromFlash();
??initHardware_3:
          CFI FunCall _Z13readFromFlashv
        BL       _Z13readFromFlashv
//  269 	}
//  270 #else
//  271 	engineConfiguration->engineType = DEFAULT_ENGINE_TYPE;
//  272 	resetConfigurationExt(sharedLogger, engineConfiguration->engineType PASS_ENGINE_PARAMETER);
//  273 #endif /* EFI_INTERNAL_FLASH */
//  274 
//  275 	if (hasFirmwareError()) {
??initHardware_4:
        LDR.N    R0,??DataTable5_27
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??initHardware_1
//  276 		return;
//  277 	}
//  278 
//  279 #if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
//  280 	initTriggerDecoder();
??initHardware_5:
          CFI FunCall _Z18initTriggerDecoderv
        BL       _Z18initTriggerDecoderv
//  281 #endif
//  282 
//  283 	mySetPadMode2("board test", boardConfiguration->boardTestModeJumperPin,
//  284 	PAL_MODE_INPUT_PULLUP);
        MOVS     R2,#+32
        LDR      R1,[R6, #+336]
        LDR.N    R0,??DataTable5_30
          CFI FunCall _Z13mySetPadMode2PKc11brain_pin_ej
        BL       _Z13mySetPadMode2PKc11brain_pin_ej
//  285 	bool isBoardTestMode_b = GET_BOARD_TEST_MODE_VALUE();
        LDR      R0,[R6, #+336]
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        LDR      R7,[R0, #+16]
        LDR      R0,[R6, #+336]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        LSRS     R0,R7,R0
        ANDS     R0,R0,#0x1
        EORS     R7,R0,#0x1
//  286 
//  287 #if HAL_USE_ADC || defined(__DOXYGEN__)
//  288 	initAdcInputs(isBoardTestMode_b);
        MOVS     R0,R7
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z13initAdcInputsb
        BL       _Z13initAdcInputsb
//  289 #endif
//  290 
//  291 	if (isBoardTestMode_b) {
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+0
        BEQ.N    ??initHardware_6
//  292 		// this method never returns
//  293 		initBoardTest();
          CFI FunCall _Z13initBoardTestv
        BL       _Z13initBoardTestv
//  294 	}
//  295 
//  296 	initRtc();
??initHardware_6:
          CFI FunCall _Z7initRtcv
        BL       _Z7initRtcv
//  297 
//  298 	initOutputPins();
          CFI FunCall _Z14initOutputPinsv
        BL       _Z14initOutputPinsv
//  299 
//  300 #if EFI_MAX_31855
//  301 	initMax31855(sharedLogger, getSpiDevice(boardConfiguration->max31855spiDevice), boardConfiguration->max31855_cs);
        LDR      R0,[R6, #+448]
          CFI FunCall _Z12getSpiDevice12spi_device_e
        BL       _Z12getSpiDevice12spi_device_e
        MOVS     R1,R0
        ADDS     R2,R6,#+416
        LDR.N    R0,??DataTable5_23
        LDR      R0,[R0, #+0]
          CFI FunCall _Z12initMax31855P7LoggingP9SPIDriverP11brain_pin_e
        BL       _Z12initMax31855P7LoggingP9SPIDriverP11brain_pin_e
//  302 #endif /* EFI_MAX_31855 */
//  303 
//  304 #if EFI_CAN_SUPPORT
//  305 	initCan();
          CFI FunCall _Z7initCanv
        BL       _Z7initCanv
//  306 #endif /* EFI_CAN_SUPPORT */
//  307 
//  308 //	init_adc_mcp3208(&adcState, &SPID2);
//  309 //	requestAdcValue(&adcState, 0);
//  310 
//  311 #if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
//  312 	// todo: figure out better startup logic
//  313 	initTriggerCentral(sharedLogger, engine);
        LDR.N    R0,??DataTable5_24
        LDR      R1,[R0, #+0]
        LDR.N    R0,??DataTable5_23
        LDR      R0,[R0, #+0]
          CFI FunCall _Z18initTriggerCentralP7LoggingP6Engine
        BL       _Z18initTriggerCentralP7LoggingP6Engine
//  314 #endif /* EFI_SHAFT_POSITION_INPUT */
//  315 
//  316 	turnOnHardware(sharedLogger);
        LDR.N    R0,??DataTable5_23
        LDR      R0,[R0, #+0]
          CFI FunCall _Z14turnOnHardwareP7Logging
        BL       _Z14turnOnHardwareP7Logging
//  317 
//  318 #if HAL_USE_SPI || defined(__DOXYGEN__)
//  319 	initSpiModules(boardConfiguration);
        MOVS     R0,R6
          CFI FunCall _Z14initSpiModulesP21board_configuration_s
        BL       _Z14initSpiModulesP21board_configuration_s
//  320 #endif
//  321 
//  322 #if EFI_HIP_9011
//  323 	initHip9011(sharedLogger);
        LDR.N    R0,??DataTable5_23
        LDR      R0,[R0, #+0]
          CFI FunCall _Z11initHip9011P7Logging
        BL       _Z11initHip9011P7Logging
//  324 #endif /* EFI_HIP_9011 */
//  325 
//  326 #if EFI_FILE_LOGGING
//  327 	initMmcCard();
          CFI FunCall initMmcCard
        BL       initMmcCard
//  328 #endif /* EFI_FILE_LOGGING */
//  329 
//  330 //	initFixedLeds();
//  331 
//  332 	//	initBooleanInputs();
//  333 
//  334 #if EFI_UART_GPS
//  335 	initGps();
          CFI FunCall initGps
        BL       initGps
//  336 #endif
//  337 
//  338 #if ADC_SNIFFER
//  339 	initAdcDriver();
//  340 #endif
//  341 
//  342 #if EFI_HD44780_LCD
//  343 //	initI2Cmodule();
//  344 	lcd_HD44780_init(sharedLogger);
        LDR.N    R0,??DataTable5_23
        LDR      R0,[R0, #+0]
          CFI FunCall lcd_HD44780_init
        BL       lcd_HD44780_init
//  345 	if (hasFirmwareError())
        LDR.N    R0,??DataTable5_27
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??initHardware_1
//  346 		return;
//  347 
//  348 	lcd_HD44780_print_string(VCS_VERSION);
??initHardware_7:
        LDR.N    R0,??DataTable5_31
          CFI FunCall lcd_HD44780_print_string
        BL       lcd_HD44780_print_string
//  349 
//  350 #endif /* EFI_HD44780_LCD */
//  351 
//  352 #if HAL_USE_I2C || defined(__DOXYGEN__)
//  353 	addConsoleActionII("i2c", sendI2Cbyte);
        LDR.N    R1,??DataTable5_32
        ADR.N    R0,??DataTable5  ;; "i2c"
          CFI FunCall addConsoleActionII
        BL       addConsoleActionII
//  354 #endif
//  355 
//  356 //	while (true) {
//  357 //		for (int addr = 0x20; addr < 0x28; addr++) {
//  358 //			sendI2Cbyte(addr, 0);
//  359 //			int err = i2cGetErrors(&I2CD1);
//  360 //			print("I2C: err=%x from %d\r\n", err, addr);
//  361 //			chThdSleepMilliseconds(5);
//  362 //			sendI2Cbyte(addr, 255);
//  363 //			chThdSleepMilliseconds(5);
//  364 //		}
//  365 //	}
//  366 
//  367 #if EFI_VEHICLE_SPEED || defined(__DOXYGEN__)
//  368 	initVehicleSpeed(sharedLogger);
        LDR.N    R0,??DataTable5_23
        LDR      R0,[R0, #+0]
          CFI FunCall _Z16initVehicleSpeedP7Logging
        BL       _Z16initVehicleSpeedP7Logging
//  369 #endif
//  370 
//  371 	initJoystick(sharedLogger);
        LDR.N    R0,??DataTable5_23
        LDR      R0,[R0, #+0]
          CFI FunCall _Z12initJoystickP7Logging
        BL       _Z12initJoystickP7Logging
//  372 
//  373 	calcFastAdcIndexes();
          CFI FunCall _Z18calcFastAdcIndexesv
        BL       _Z18calcFastAdcIndexesv
//  374 	engine->addConfigurationListener(adcConfigListener);
        LDR.N    R1,??DataTable5_33
        LDR.N    R0,??DataTable5_24
        LDR      R0,[R0, #+0]
          CFI FunCall _ZN6Engine24addConfigurationListenerEPFvPS_E
        BL       _ZN6Engine24addConfigurationListenerEPFvPS_E
//  375 
//  376 	initWboController();
          CFI FunCall _Z17initWboControllerv
        BL       _Z17initWboControllerv
//  377 
//  378 	printMsg(sharedLogger, "initHardware() OK!");
        LDR.N    R1,??DataTable5_34
        LDR.N    R0,??DataTable5_23
        LDR      R0,[R0, #+0]
          CFI FunCall printMsg
        BL       printMsg
//  379 }
??initHardware_1:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock11

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5:
        DC8      "i2c"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_1:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_2:
        DC32     `?<Constant "lockSpi">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_3:
        DC32     spiMtx

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_4:
        DC32     SPID1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_5:
        DC32     SPID2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_6:
        DC32     SPID3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_7:
        DC32     `?<Constant "Unexpected SPI device...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_8:
        DC32     `?<Constant "Starting I2C module\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_9:
        DC32     i2cfg

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_10:
        DC32     I2CD1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_11:
        DC32     0x40020400

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_12:
        DC32     `?<Constant "I2C clock">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_13:
        DC32     `?<Constant "I2C data">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_14:
        DC32     `?<Constant "lowstck12a">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_15:
        DC32     `?<Constant "lowstck#9b">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_16:
        DC32     fastMapSampleIndex

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_17:
        DC32     fastAdc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_18:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_19:
        DC32     hipSampleIndex

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_20:
        DC32     tpsSampleIndex

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_21:
        DC32     tpsFastAdc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_22:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_23:
        DC32     sharedLogger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_24:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_25:
        DC32     `?<Constant "engineConfiguration">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_26:
        DC32     `?<Constant "initHardware()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_27:
        DC32     hasFirmwareErrorFlag

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_28:
        DC32     0x40020c00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_29:
        DC32     0x40020c10

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_30:
        DC32     `?<Constant "board test">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_31:
        DC32     `?<Constant "8529">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_32:
        DC32     _Z11sendI2Cbyteii

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_33:
        DC32     _Z17adcConfigListenerP6Engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_34:
        DC32     `?<Constant "initHardware() OK!">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lockSpi">`:
        DATA
        DC8 "lockSpi"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Unexpected SPI device...">`:
        DATA
        DC8 "Unexpected SPI device: %d"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Starting I2C module\\r\\n">`:
        DATA
        DC8 "Starting I2C module\015\012"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "I2C clock">`:
        DATA
        DC8 "I2C clock"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "I2C data">`:
        DATA
        DC8 "I2C data"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lowstck12a">`:
        DATA
        DC8 "lowstck12a"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lowstck#9b">`:
        DATA
        DC8 "lowstck#9b"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "engineConfiguration">`:
        DATA
        DC8 "engineConfiguration"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "initHardware()">`:
        DATA
        DC8 "initHardware()"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "board test">`:
        DATA
        DC8 "board test"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "8529">`:
        DATA
        DC8 "8529"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "i2c"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "initHardware() OK!">`:
        DATA
        DC8 "initHardware() OK!"
        DC8 0

        END
//  380 
//  381 #endif /* EFI_PROD_CODE */
// 
//  36 bytes in section .bss
//  12 bytes in section .data
// 188 bytes in section .rodata
// 972 bytes in section .text
// 
// 972 bytes of CODE  memory
// 188 bytes of CONST memory
//  48 bytes of DATA  memory
//
//Errors: none
//Warnings: 2
