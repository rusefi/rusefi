///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:58 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\development\hw_ /
//                       layer\poten.cpp                                      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\development\hw_ /
//                       layer\poten.cpp -lCN F:\stuff\rusefi_sourceforge\fir /
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
//                       poten.s                                              /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME poten

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z12getSpiDevice12spi_device_e
        EXTERN _Z7lockSpi12spi_device_e
        EXTERN _Z9turnOnSpi12spi_device_e
        EXTERN _Z9unlockSpiv
        EXTERN addConsoleActionI
        EXTERN addConsoleActionII
        EXTERN initSpiCs
        EXTERN scheduleMsg
        EXTERN spiSelect
        EXTERN spiSend
        EXTERN spiStart
        EXTERN spiStop
        EXTERN spiUnselect

        PUBLIC _Z16setPotResistanceP14Mcp42010Driverii
        PUBLIC _Z17initPotentiometerP14Mcp42010DriverP9SPIDriver11brain_pin_e
        PUBLIC _Z18initPotentiometersP7LoggingP21board_configuration_s
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\development\hw_layer\poten.cpp
//    1 /**
//    2  * @file	poten.c
//    3  * @brief	MCP42010 digital potentiometer driver
//    4  *
//    5  * @date Mar 16, 2013
//    6  * @author Andrey Belomutskiy, (c) 2012-2015
//    7  */
//    8 
//    9 #include "main.h"
//   10 #include "poten.h"
//   11 #include "eficonsole.h"
//   12 #include "pin_repository.h"
//   13 #include "engine_configuration.h"
//   14 #include "hardware.h"
//   15 #include "mpu_util.h"
//   16 
//   17 /**
//   18  * MCP42010 digital potentiometer driver
//   19  *
//   20  *
//   21  * 1	CS		pin select						PB12		PA10
//   22  * 2	SCK		serial clock					PA5			PC10
//   23  * 3	SI		serial input		(MOSI)		PA7			PC12
//   24  * 4	Vss 	ground
//   25  * 5	PB1
//   26  * 6	PW1
//   27  * 7	PA1
//   28  * 8	PA0
//   29  * 9	PW0
//   30  * 10	PB0
//   31  * 11	RS		Reset
//   32  *
//   33  * 14	Vdd 	V input
//   34  *
//   35  * Rwa = 10000 * (256 - d) / 256 + 52
//   36  * d = 256 - (Rwa - 52) * 256 / 10000
//   37  *
//   38  */
//   39 
//   40 /* Low speed SPI configuration (281.250kHz, CPHA=0, CPOL=0, MSb first).*/
//   41 #define SPI_POT_CONFIG SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_DFF
//   42 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   43 static Logging * logger;
logger:
        DS8 4
//   44 
//   45 #if EFI_POTENTIOMETER || defined(__DOXYGEN__)

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   46 static Mcp42010Driver potConfig[DIGIPOT_COUNT];
potConfig:
        DS8 64
//   47 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z17initPotentiometerP14Mcp42010DriverP9SPIDriver11brain_pin_e
        THUMB
//   48 void initPotentiometer(Mcp42010Driver *driver, SPIDriver *spi, brain_pin_e csPin) {
_Z17initPotentiometerP14Mcp42010DriverP9SPIDriver11brain_pin_e:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   49 	driver->spiConfig.cr1 = SPI_POT_CONFIG;
        MOV      R0,#+2096
        STRH     R0,[R4, #+14]
//   50 	driver->spi = spi;
        STR      R5,[R4, #+0]
//   51 	initSpiCs(&driver->spiConfig, csPin);
        MOVS     R1,R6
        ADDS     R0,R4,#+4
          CFI FunCall initSpiCs
        BL       initSpiCs
//   52 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock0
//   53 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z10getPotStepi
          CFI NoCalls
        THUMB
//   54 static int getPotStep(int resistanceWA) {
//   55 	return 256 - (int) ((resistanceWA - 52) * 256 / 10000);
_Z10getPotStepi:
        SUBS     R0,R0,#+52
        MOV      R1,#+256
        MULS     R0,R1,R0
        MOVW     R1,#+10000
        SDIV     R0,R0,R1
        RSBS     R0,R0,#+256
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   56 }
//   57 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z9sendToPotP14Mcp42010Driverii
        THUMB
//   58 static void sendToPot(Mcp42010Driver *driver, int channel, int value) {
_Z9sendToPotP14Mcp42010Driverii:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+8
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   59 	lockSpi(SPI_NONE);
        MOVS     R0,#+0
          CFI FunCall _Z7lockSpi12spi_device_e
        BL       _Z7lockSpi12spi_device_e
//   60 	spiStart(driver->spi, &driver->spiConfig);
        ADDS     R1,R4,#+4
        LDR      R0,[R4, #+0]
          CFI FunCall spiStart
        BL       spiStart
//   61 	spiSelect(driver->spi);
        LDR      R0,[R4, #+0]
          CFI FunCall spiSelect
        BL       spiSelect
//   62 	int word = (17 + channel) * 256 + value;
        ADDS     R0,R5,#+17
        MOV      R1,#+256
        MLA      R0,R1,R0,R6
        STR      R0,[SP, #+0]
//   63 	spiSend(driver->spi, 1, &word);
        ADD      R2,SP,#+0
        MOVS     R1,#+1
        LDR      R0,[R4, #+0]
          CFI FunCall spiSend
        BL       spiSend
//   64 	spiUnselect(driver->spi);
        LDR      R0,[R4, #+0]
          CFI FunCall spiUnselect
        BL       spiUnselect
//   65 	spiStop(driver->spi);
        LDR      R0,[R4, #+0]
          CFI FunCall spiStop
        BL       spiStop
//   66 	unlockSpi();
          CFI FunCall _Z9unlockSpiv
        BL       _Z9unlockSpiv
//   67 }
        POP      {R0,R1,R4-R6,PC}  ;; return
          CFI EndBlock cfiBlock2
//   68 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z16setPotResistanceP14Mcp42010Driverii
        THUMB
//   69 void setPotResistance(Mcp42010Driver *driver, int channel, int resistance) {
_Z16setPotResistanceP14Mcp42010Driverii:
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
//   70 	int value = getPotStep(resistance);
        MOVS     R0,R6
          CFI FunCall _Z10getPotStepi
        BL       _Z10getPotStepi
        MOVS     R7,R0
//   71 
//   72 	scheduleMsg(logger, "Sending to potentiometer%d: %d for R=%d", channel, value, resistance);
        STR      R6,[SP, #+0]
        MOVS     R3,R7
        MOVS     R2,R5
        LDR.N    R1,??DataTable3_1
        LDR.N    R0,??DataTable3_2
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   73 	sendToPot(driver, channel, value);
        MOVS     R2,R7
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z9sendToPotP14Mcp42010Driverii
        BL       _Z9sendToPotP14Mcp42010Driverii
//   74 }
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock3
//   75 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z23setPotResistanceCommandii
        THUMB
//   76 static void setPotResistanceCommand(int index, int value) {
_Z23setPotResistanceCommandii:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   77 	setPotResistance(&potConfig[index / 2], index % 2, value);
        MOVS     R2,R5
        MOVS     R0,#+2
        SDIV     R1,R4,R0
        MLS      R1,R1,R0,R4
        MOVS     R0,#+2
        SDIV     R0,R4,R0
        LDR.N    R3,??DataTable3_3
        ADDS     R0,R3,R0, LSL #+4
          CFI FunCall _Z16setPotResistanceP14Mcp42010Driverii
        BL       _Z16setPotResistanceP14Mcp42010Driverii
//   78 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock4
//   79 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z12setPotValue1i
        THUMB
//   80 static void setPotValue1(int value) {
_Z12setPotValue1i:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   81 	sendToPot(&potConfig[0], 1, value);
        MOVS     R2,R4
        MOVS     R1,#+1
        LDR.N    R0,??DataTable3_3
          CFI FunCall _Z9sendToPotP14Mcp42010Driverii
        BL       _Z9sendToPotP14Mcp42010Driverii
//   82 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock5
//   83 
//   84 #endif /* EFI_POTENTIOMETER */
//   85 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z18initPotentiometersP7LoggingP21board_configuration_s
        THUMB
//   86 void initPotentiometers(Logging *sharedLogger, board_configuration_s *boardConfiguration) {
_Z18initPotentiometersP7LoggingP21board_configuration_s:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//   87 	logger = sharedLogger;
        LDR.N    R0,??DataTable3_2
        STR      R4,[R0, #+0]
//   88 #if EFI_POTENTIOMETER
//   89 	if (boardConfiguration->digitalPotentiometerSpiDevice == SPI_NONE) {
        LDR      R0,[R5, #+212]
        CMP      R0,#+0
        BNE.N    ??initPotentiometers_0
//   90 		scheduleMsg(logger, "digiPot spi disabled");
        LDR.N    R1,??DataTable3_4
        LDR.N    R0,??DataTable3_2
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   91 		return;
        B.N      ??initPotentiometers_1
//   92 	}
//   93 	turnOnSpi(boardConfiguration->digitalPotentiometerSpiDevice);
??initPotentiometers_0:
        LDR      R0,[R5, #+212]
          CFI FunCall _Z9turnOnSpi12spi_device_e
        BL       _Z9turnOnSpi12spi_device_e
//   94 
//   95 	for (int i = 0; i < DIGIPOT_COUNT; i++) {
        MOVS     R6,#+0
??initPotentiometers_2:
        CMP      R6,#+4
        BGE.N    ??initPotentiometers_3
//   96 		brain_pin_e csPin = boardConfiguration->digitalPotentiometerChipSelect[i];
        ADDS     R0,R5,R6, LSL #+2
        LDR      R7,[R0, #+216]
//   97 		if (csPin == GPIO_UNASSIGNED) {
        CMP      R7,#+80
        BEQ.N    ??initPotentiometers_4
//   98 			continue;
//   99                 }
//  100 
//  101 		initPotentiometer(&potConfig[i], getSpiDevice(boardConfiguration->digitalPotentiometerSpiDevice),
//  102 				csPin);
??initPotentiometers_5:
        LDR      R0,[R5, #+212]
          CFI FunCall _Z12getSpiDevice12spi_device_e
        BL       _Z12getSpiDevice12spi_device_e
        MOVS     R2,R7
        MOVS     R1,R0
        LDR.N    R0,??DataTable3_3
        ADDS     R0,R0,R6, LSL #+4
          CFI FunCall _Z17initPotentiometerP14Mcp42010DriverP9SPIDriver11brain_pin_e
        BL       _Z17initPotentiometerP14Mcp42010DriverP9SPIDriver11brain_pin_e
//  103 	}
??initPotentiometers_4:
        ADDS     R6,R6,#+1
        B.N      ??initPotentiometers_2
//  104 
//  105 	addConsoleActionII("pot", setPotResistanceCommand);
??initPotentiometers_3:
        LDR.N    R1,??DataTable3_5
        ADR.N    R0,??DataTable3  ;; "pot"
          CFI FunCall addConsoleActionII
        BL       addConsoleActionII
//  106 
//  107 	addConsoleActionI("potd1", setPotValue1);
        LDR.N    R1,??DataTable3_6
        LDR.N    R0,??DataTable3_7
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  108 
//  109 	setPotResistance(&potConfig[0], 0, 3000);
        MOVW     R2,#+3000
        MOVS     R1,#+0
        LDR.N    R0,??DataTable3_3
          CFI FunCall _Z16setPotResistanceP14Mcp42010Driverii
        BL       _Z16setPotResistanceP14Mcp42010Driverii
//  110 	setPotResistance(&potConfig[0], 1, 7000);
        MOVW     R2,#+7000
        MOVS     R1,#+1
        LDR.N    R0,??DataTable3_3
          CFI FunCall _Z16setPotResistanceP14Mcp42010Driverii
        BL       _Z16setPotResistanceP14Mcp42010Driverii
//  111 #else
//  112 	print("digiPot logic disabled\r\n");
//  113 #endif
//  114 }
??initPotentiometers_1:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock6

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3:
        DC8      "pot"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_1:
        DC32     `?<Constant "Sending to potentiome...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_2:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_3:
        DC32     potConfig

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_4:
        DC32     `?<Constant "digiPot spi disabled">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_5:
        DC32     _Z23setPotResistanceCommandii

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_6:
        DC32     _Z12setPotValue1i

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_7:
        DC32     `?<Constant "potd1">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Sending to potentiome...">`:
        DATA
        DC8 "Sending to potentiometer%d: %d for R=%d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "digiPot spi disabled">`:
        DATA
        DC8 "digiPot spi disabled"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "pot"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "potd1">`:
        DATA
        DC8 "potd1"
        DC8 0, 0

        END
// 
//  68 bytes in section .bss
//  76 bytes in section .rodata
// 372 bytes in section .text
// 
// 372 bytes of CODE  memory
//  76 bytes of CONST memory
//  68 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
