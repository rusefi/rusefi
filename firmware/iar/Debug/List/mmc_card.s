///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:55 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\mmc_ca /
//                       rd.cpp                                               /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\mmc_ca /
//                       rd.cpp -lCN F:\stuff\rusefi_sourceforge\firmware\iar /
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
//                       mmc_card.s                                           /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME mmc_card

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        RTMODEL "__dlib_file_descriptor", "0"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN SPID3
        EXTERN _Z10hwPortname11brain_pin_e
        EXTERN _Z12boolToStringb
        EXTERN _Z12writeLogLinev
        EXTERN _Z7lockSpi12spi_device_e
        EXTERN _Z8getHwPin11brain_pin_e
        EXTERN _Z9getHwPort11brain_pin_e
        EXTERN _Z9unlockSpiv
        EXTERN _ZN18LoggingWithStorageC1EPKc
        EXTERN __aeabi_memset
        EXTERN absI
        EXTERN addConsoleAction
        EXTERN addConsoleActionS
        EXTERN atoi
        EXTERN boardConfiguration
        EXTERN chThdCreateStatic
        EXTERN chThdSleep
        EXTERN f_close
        EXTERN f_lseek
        EXTERN f_mount
        EXTERN f_open
        EXTERN f_opendir
        EXTERN f_read
        EXTERN f_readdir
        EXTERN f_sync
        EXTERN f_unlink
        EXTERN f_write
        EXTERN itoa10
        EXTERN mmcConnect
        EXTERN mmcDisconnect
        EXTERN mmcObjectInit
        EXTERN mmcStart
        EXTERN mmcStop
        EXTERN rlist
        EXTERN scheduleMsg
        EXTERN strcat
        EXTERN strcpy
        EXTERN strlen
        EXTERN strncmp
        EXTERN warning

        PUBLIC MMCD1
        PUBLIC appendToLog
        PUBLIC initMmcCard
        PUBLIC isSdCardAlive
        PUBLIC memset
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\hw_layer\mmc_card.cpp
//    1 /**
//    2  * @file	mmc_card.cpp
//    3  *
//    4  * @date Dec 28, 2013
//    5  * @author Kot_dnz
//    6  * @author Andrey Belomutskiy, (c) 2012-2015
//    7  *
//    8  * default pinouts in case of SPI2 connected to MMC: PB13 - SCK, PB14 - MISO, PB15 - MOSI, PD4 - CS, 3.3v
//    9  * default pinouts in case of SPI3 connected to MMC: PB3  - SCK, PB4  - MISO, PB5  - MOSI, PD4 - CS, 3.3v
//   10  *
//   11  *
//   12  * todo: extract some logic into a controller file
//   13  */
//   14 
//   15 #include "main.h"
//   16 
//   17 #if EFI_FILE_LOGGING || defined(__DOXYGEN__)
//   18 
//   19 #include <stdio.h>
//   20 #include <string.h>
//   21 #include "mmc_card.h"
//   22 #include "pin_repository.h"
//   23 #include "ff.h"
//   24 #include "hardware.h"
//   25 #include "engine_configuration.h"
//   26 #include "status_loop.h"
//   27 
//   28 #define LOG_INDEX_FILENAME "index.txt"
//   29 #define RUSEFI_LOG_PREFIX "rus"
//   30 #define LS_RESPONSE "ls_result"
//   31 #define FILE_LIST_MAX_COUNT 20
//   32 
//   33 extern board_configuration_s *boardConfiguration;
//   34 
//   35 #define PUSHPULLDELAY 500
//   36 

        SECTION `.bss`:DATA:REORDER:NOROOT(3)
//   37 static THD_WORKING_AREA(mmcThreadStack,UTILITY_THREAD_STACK_SIZE);		// MMC monitor thread
mmcThreadStack:
        DS8 712
//   38 
//   39 /**
//   40  * MMC driver instance.
//   41  */

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   42 MMCDriver MMCD1;
MMCD1:
        DS8 52
//   43 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   44 static SPIConfig hs_spicfg = { NULL, NULL, 0,
hs_spicfg:
        DATA
        DC32 0H, 0H
        DC16 0, 16
//   45 SPI_BaudRatePrescaler_8 };

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   46 static SPIConfig ls_spicfg = { NULL, NULL, 0,
ls_spicfg:
        DATA
        DC32 0H, 0H
        DC16 0, 56
//   47 SPI_BaudRatePrescaler_256 };
//   48 
//   49 /* MMC/SD over SPI driver configuration.*/
//   50 // don't forget check if STM32_SPI_USE_SPI2 defined and spi has init with correct GPIO in hardware.c

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   51 static MMCConfig mmccfg = { &MMC_CARD_SPI, &ls_spicfg, &hs_spicfg };
mmccfg:
        DATA
        DC32 SPID3, ls_spicfg, hs_spicfg
//   52 

        SECTION `.bss`:DATA:REORDER:NOROOT(0)
//   53 static bool fs_ready = false;
fs_ready:
        DS8 1
//   54 
//   55 #define FILE_LOG_DELAY 200
//   56 
//   57 /**
//   58  * fatfs MMC/SPI
//   59  */

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   60 static FATFS MMC_FS;
MMC_FS:
        DS8 560
//   61 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   62 static LoggingWithStorage logger("mmcCard");
        LDR.W    R1,??DataTable13_2
        LDR.W    R0,??DataTable13_3
          CFI FunCall _ZN18LoggingWithStorageC1EPKc
        BL       _ZN18LoggingWithStorageC1EPKc
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
logger:
        DS8 260
//   63 
//   64 // print FAT error function

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z10printErrorPKc7FRESULT
        THUMB
//   65 static void printError(const char *str, FRESULT f_error) {
_Z10printErrorPKc7FRESULT:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   66 	scheduleMsg(&logger, "FATfs Error \"%s\" %d", str, f_error);
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        MOVS     R3,R5
        MOVS     R2,R4
        LDR.W    R1,??DataTable13_4
        LDR.W    R0,??DataTable13_3
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   67 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock1
//   68 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   69 static FIL FDLogFile;
FDLogFile:
        DS8 548

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   70 static FIL FDCurrFile;
FDCurrFile:
        DS8 548

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   71 static int logFileIndex = 1;
logFileIndex:
        DATA
        DC32 1

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   72 static char logName[15];
logName:
        DS8 16
//   73 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   74 static int totalLoggedBytes = 0;
totalLoggedBytes:
        DS8 4
//   75 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z14printMmcPinoutv
        THUMB
//   76 static void printMmcPinout(void) {
_Z14printMmcPinoutv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   77 	scheduleMsg(&logger, "MMC CS %s", hwPortname(boardConfiguration->sdCardCsPin));
        LDR.W    R0,??DataTable13_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+324]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R2,R0
        LDR.W    R1,??DataTable13_6
        LDR.W    R0,??DataTable13_3
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   78 	// todo: we need to figure out the right SPI pinout, not just SPI2
//   79 //	scheduleMsg(&logger, "MMC SCK %s:%d", portname(EFI_SPI2_SCK_PORT), EFI_SPI2_SCK_PIN);
//   80 //	scheduleMsg(&logger, "MMC MISO %s:%d", portname(EFI_SPI2_MISO_PORT), EFI_SPI2_MISO_PIN);
//   81 //	scheduleMsg(&logger, "MMC MOSI %s:%d", portname(EFI_SPI2_MOSI_PORT), EFI_SPI2_MOSI_PIN);
//   82 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock2
//   83 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z12sdStatisticsv
        THUMB
//   84 static void sdStatistics(void) {
_Z12sdStatisticsv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   85 	printMmcPinout();
          CFI FunCall _Z14printMmcPinoutv
        BL       _Z14printMmcPinoutv
//   86 	scheduleMsg(&logger, "SD enabled: %s [%s]", boolToString(boardConfiguration->isSdCardEnabled),
//   87 			logName);
        LDR.W    R0,??DataTable13_5
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+3
        ANDS     R0,R0,#0x1
          CFI FunCall _Z12boolToStringb
        BL       _Z12boolToStringb
        LDR.W    R3,??DataTable13_7
        MOVS     R2,R0
        LDR.W    R1,??DataTable13_8
        LDR.W    R0,??DataTable13_3
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   88 	scheduleMsg(&logger, "fs_ready=%d totalLoggedBytes=%d %d", fs_ready, totalLoggedBytes, logFileIndex);
        LDR.W    R0,??DataTable13_9
        LDR      R0,[R0, #+0]
        STR      R0,[SP, #+0]
        LDR.W    R0,??DataTable13_10
        LDR      R3,[R0, #+0]
        LDR.W    R0,??DataTable13_11
        LDRB     R2,[R0, #+0]
        LDR.W    R1,??DataTable13_12
        LDR.W    R0,??DataTable13_3
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   89 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock3
//   90 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z14incLogFileNamev
        THUMB
//   91 static void incLogFileName(void) {
_Z14incLogFileNamev:
        PUSH     {R0-R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+24
//   92 	lockSpi(SPI_NONE);
        MOVS     R0,#+0
          CFI FunCall _Z7lockSpi12spi_device_e
        BL       _Z7lockSpi12spi_device_e
//   93 	memset(&FDCurrFile, 0, sizeof(FIL));						// clear the memory
        MOV      R2,#+548
        MOVS     R1,#+0
        LDR.W    R0,??DataTable13_13
          CFI FunCall memset
        BL       memset
//   94 	FRESULT err = f_open(&FDCurrFile, LOG_INDEX_FILENAME, FA_READ);				// This file has the index for next log file name
        MOVS     R2,#+1
        LDR.W    R1,??DataTable13_14
          CFI FunCall f_open
        BL       f_open
        MOVS     R4,R0
//   95 
//   96 	char data[10];
//   97 	UINT result = 0;
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
//   98 	if (err != FR_OK && err != FR_EXIST) {
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+0
        BEQ.N    ??incLogFileName_0
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+8
        BEQ.N    ??incLogFileName_0
//   99 			logFileIndex = 1;
        LDR.W    R0,??DataTable13_9
        MOVS     R1,#+1
        STR      R1,[R0, #+0]
//  100 			scheduleMsg(&logger, "%s: not found or error: %d", LOG_INDEX_FILENAME, err);
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        MOVS     R3,R4
        LDR.W    R2,??DataTable13_14
        LDR.W    R1,??DataTable13_15
        LDR.W    R0,??DataTable13_3
          CFI FunCall scheduleMsg
        BL       scheduleMsg
        B.N      ??incLogFileName_1
//  101 	} else {
//  102 		f_read(&FDCurrFile, (void*)data, sizeof(data), &result);
??incLogFileName_0:
        ADD      R3,SP,#+0
        MOVS     R2,#+10
        ADD      R1,SP,#+4
        LDR.W    R0,??DataTable13_13
          CFI FunCall f_read
        BL       f_read
//  103 
//  104 		scheduleMsg(&logger, "Got content [%s] size %d", data, result);
        LDR      R3,[SP, #+0]
        ADD      R2,SP,#+4
        LDR.W    R1,??DataTable13_16
        LDR.W    R0,??DataTable13_3
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  105 		f_close(&FDCurrFile);
        LDR.W    R0,??DataTable13_13
          CFI FunCall f_close
        BL       f_close
//  106 		if (result < 5) {
        LDR      R0,[SP, #+0]
        CMP      R0,#+5
        BCS.N    ??incLogFileName_2
//  107                       data[result] = 0;
        LDR      R0,[SP, #+0]
        ADD      R1,SP,#+4
        MOVS     R2,#+0
        STRB     R2,[R0, R1]
//  108 			logFileIndex = atoi(data);
        ADD      R0,SP,#+4
          CFI FunCall atoi
        BL       atoi
        LDR.W    R1,??DataTable13_9
        STR      R0,[R1, #+0]
//  109 			if (absI(logFileIndex) == ERROR_CODE) {
        LDR.W    R0,??DataTable13_9
        LDR      R0,[R0, #+0]
          CFI FunCall absI
        BL       absI
        LDR.W    R1,??DataTable13_17  ;; 0x128ce430
        CMP      R0,R1
        BNE.N    ??incLogFileName_3
//  110 				logFileIndex = 1;
        LDR.W    R0,??DataTable13_9
        MOVS     R1,#+1
        STR      R1,[R0, #+0]
        B.N      ??incLogFileName_1
//  111 			} else {
//  112 				logFileIndex++; // next file would use next file name
??incLogFileName_3:
        LDR.W    R0,??DataTable13_9
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.W    R1,??DataTable13_9
        STR      R0,[R1, #+0]
        B.N      ??incLogFileName_1
//  113 			}
//  114 		} else {
//  115 			logFileIndex = 1;
??incLogFileName_2:
        LDR.W    R0,??DataTable13_9
        MOVS     R1,#+1
        STR      R1,[R0, #+0]
//  116 		}
//  117 	}
//  118 
//  119 	err = f_open(&FDCurrFile, LOG_INDEX_FILENAME, FA_OPEN_ALWAYS | FA_WRITE);
??incLogFileName_1:
        MOVS     R2,#+18
        LDR.W    R1,??DataTable13_14
        LDR.W    R0,??DataTable13_13
          CFI FunCall f_open
        BL       f_open
        MOVS     R4,R0
//  120 	itoa10(data, logFileIndex);
        LDR.W    R0,??DataTable13_9
        LDR      R1,[R0, #+0]
        ADD      R0,SP,#+4
          CFI FunCall itoa10
        BL       itoa10
//  121 	f_write(&FDCurrFile, (void*)data, strlen(data), &result);
        ADD      R0,SP,#+4
          CFI FunCall strlen
        BL       strlen
        MOVS     R2,R0
        ADD      R3,SP,#+0
        ADD      R1,SP,#+4
        LDR.W    R0,??DataTable13_13
          CFI FunCall f_write
        BL       f_write
//  122 	f_close(&FDCurrFile);
        LDR.W    R0,??DataTable13_13
          CFI FunCall f_close
        BL       f_close
//  123 	scheduleMsg(&logger, "Done %d", logFileIndex);
        LDR.W    R0,??DataTable13_9
        LDR      R2,[R0, #+0]
        LDR.W    R1,??DataTable13_18
        LDR.W    R0,??DataTable13_3
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  124 	unlockSpi();
          CFI FunCall _Z9unlockSpiv
        BL       _Z9unlockSpiv
//  125 }
        POP      {R0-R4,PC}       ;; return
          CFI EndBlock cfiBlock4
//  126 
//  127 /**
//  128  * @brief Create a new file with the specified name
//  129  *
//  130  * This function saves the name of the file in a global variable
//  131  * so that we can later append to that file
//  132  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z13createLogFilev
        THUMB
//  133 static void createLogFile(void) {
_Z13createLogFilev:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
//  134 	lockSpi(SPI_NONE);
        MOVS     R0,#+0
          CFI FunCall _Z7lockSpi12spi_device_e
        BL       _Z7lockSpi12spi_device_e
//  135 	memset(&FDLogFile, 0, sizeof(FIL));						// clear the memory
        MOV      R2,#+548
        MOVS     R1,#+0
        LDR.W    R0,??DataTable13_19
          CFI FunCall memset
        BL       memset
//  136 	strcpy(logName, RUSEFI_LOG_PREFIX);
        ADR.N    R1,??DataTable9  ;; "rus"
        LDR.W    R0,??DataTable13_7
          CFI FunCall strcpy
        BL       strcpy
//  137 	char *ptr = itoa10(&logName[3], logFileIndex);
        LDR.W    R0,??DataTable13_9
        LDR      R1,[R0, #+0]
        LDR.W    R0,??DataTable13_20
          CFI FunCall itoa10
        BL       itoa10
        MOVS     R4,R0
//  138 	strcat(ptr, ".log");
        LDR.W    R1,??DataTable13_21
        MOVS     R0,R4
          CFI FunCall strcat
        BL       strcat
//  139 
//  140 	FRESULT err = f_open(&FDLogFile, logName, FA_OPEN_ALWAYS | FA_WRITE);				// Create new file
        MOVS     R2,#+18
        LDR.W    R1,??DataTable13_7
        LDR.W    R0,??DataTable13_19
          CFI FunCall f_open
        BL       f_open
        MOVS     R5,R0
//  141 	if (err != FR_OK && err != FR_EXIST) {
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BEQ.N    ??createLogFile_0
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+8
        BEQ.N    ??createLogFile_0
//  142 		unlockSpi();
          CFI FunCall _Z9unlockSpiv
        BL       _Z9unlockSpiv
//  143 		printError("FS mount failed", err);	// else - show error
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR.W    R0,??DataTable13_22
          CFI FunCall _Z10printErrorPKc7FRESULT
        BL       _Z10printErrorPKc7FRESULT
//  144 		return;
        B.N      ??createLogFile_1
//  145 	}
//  146 
//  147 	err = f_lseek(&FDLogFile, f_size(&FDLogFile)); // Move to end of the file to append data
??createLogFile_0:
        LDR.W    R0,??DataTable13_19
        LDR      R1,[R0, #+12]
        LDR.W    R0,??DataTable13_19
          CFI FunCall f_lseek
        BL       f_lseek
        MOVS     R5,R0
//  148 	if (err) {
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BEQ.N    ??createLogFile_2
//  149 		unlockSpi();
          CFI FunCall _Z9unlockSpiv
        BL       _Z9unlockSpiv
//  150 		printError("Seek error", err);
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR.W    R0,??DataTable13_23
          CFI FunCall _Z10printErrorPKc7FRESULT
        BL       _Z10printErrorPKc7FRESULT
//  151 		return;
        B.N      ??createLogFile_1
//  152 	}
//  153 	f_sync(&FDLogFile);
??createLogFile_2:
        LDR.W    R0,??DataTable13_19
          CFI FunCall f_sync
        BL       f_sync
//  154 	fs_ready = true;						// everything Ok
        LDR.W    R0,??DataTable13_11
        MOVS     R1,#+1
        STRB     R1,[R0, #+0]
//  155 	unlockSpi();
          CFI FunCall _Z9unlockSpiv
        BL       _Z9unlockSpiv
//  156 }
??createLogFile_1:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock5
//  157 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z10removeFilePKc
        THUMB
//  158 static void removeFile(const char *pathx) {
_Z10removeFilePKc:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  159 	if (!fs_ready) {
        LDR.N    R0,??DataTable13_11
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??removeFile_0
//  160 		scheduleMsg(&logger, "Error: No File system is mounted");
        LDR.W    R1,??DataTable13_24
        LDR.N    R0,??DataTable13_3
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  161 		return;
        B.N      ??removeFile_1
//  162 	}
//  163 	lockSpi(SPI_NONE);
??removeFile_0:
        MOVS     R0,#+0
          CFI FunCall _Z7lockSpi12spi_device_e
        BL       _Z7lockSpi12spi_device_e
//  164 	f_unlink(pathx);
        MOVS     R0,R4
          CFI FunCall f_unlink
        BL       f_unlink
//  165 
//  166 	unlockSpi();
          CFI FunCall _Z9unlockSpiv
        BL       _Z9unlockSpiv
//  167 }
??removeFile_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock6
//  168 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z13listDirectoryPKc
        THUMB
//  169 static void listDirectory(const char *path) {
_Z13listDirectoryPKc:
        PUSH     {R4-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+20
        SUB      SP,SP,#+68
          CFI CFA R13+88
        MOVS     R4,R0
//  170 
//  171 	if (!fs_ready) {
        LDR.N    R0,??DataTable13_11
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??listDirectory_0
//  172 		scheduleMsg(&logger, "Error: No File system is mounted");
        LDR.N    R1,??DataTable13_24
        LDR.N    R0,??DataTable13_3
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  173 		return;
        B.N      ??listDirectory_1
//  174 	}
//  175 	lockSpi(SPI_NONE);
??listDirectory_0:
        MOVS     R0,#+0
          CFI FunCall _Z7lockSpi12spi_device_e
        BL       _Z7lockSpi12spi_device_e
//  176 
//  177 	DIR dir;
//  178 	FRESULT res = f_opendir(&dir, path);
        MOVS     R1,R4
        ADD      R0,SP,#+32
          CFI FunCall f_opendir
        BL       f_opendir
        MOVS     R7,R0
//  179 
//  180 	if (res != FR_OK) {
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+0
        BEQ.N    ??listDirectory_2
//  181 		scheduleMsg(&logger, "Error opening directory %s", path);
        MOVS     R2,R4
        LDR.N    R1,??DataTable13_25
        LDR.N    R0,??DataTable13_3
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  182 		unlockSpi();
          CFI FunCall _Z9unlockSpiv
        BL       _Z9unlockSpiv
//  183 		return;
        B.N      ??listDirectory_1
//  184 	}
//  185 
//  186 	scheduleMsg(&logger, LS_RESPONSE);
??listDirectory_2:
        LDR.N    R1,??DataTable13_26
        LDR.N    R0,??DataTable13_3
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  187 
//  188 	int i = strlen(path);
        MOVS     R0,R4
          CFI FunCall strlen
        BL       strlen
        MOVS     R5,R0
//  189 	for (int count = 0;count < FILE_LIST_MAX_COUNT;) {
        MOVS     R6,#+0
??listDirectory_3:
        CMP      R6,#+20
        BGE.N    ??listDirectory_4
//  190 		FILINFO fno;
//  191 		res = f_readdir(&dir, &fno);
        ADD      R1,SP,#+0
        ADD      R0,SP,#+32
          CFI FunCall f_readdir
        BL       f_readdir
        MOVS     R7,R0
//  192 		if (res != FR_OK || fno.fname[0] == 0)
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+0
        BNE.N    ??listDirectory_5
        LDRB     R0,[SP, #+9]
        CMP      R0,#+0
        BNE.N    ??listDirectory_6
//  193 			break;
??listDirectory_5:
        B.N      ??listDirectory_4
//  194 		if (fno.lfname[0] == '.')
??listDirectory_6:
        LDR      R0,[SP, #+24]
        LDRB     R0,[R0, #+0]
        CMP      R0,#+46
        BEQ.N    ??listDirectory_3
//  195 			continue;
//  196 		if ((fno.fattrib & AM_DIR) || strncmp(RUSEFI_LOG_PREFIX, fno.fname, sizeof(RUSEFI_LOG_PREFIX) - 1)) {
??listDirectory_7:
        LDRB     R0,[SP, #+8]
        LSLS     R0,R0,#+27
        BMI.N    ??listDirectory_8
        MOVS     R2,#+3
        ADD      R1,SP,#+9
        ADR.N    R0,??DataTable9  ;; "rus"
          CFI FunCall strncmp
        BL       strncmp
        CMP      R0,#+0
        BEQ.N    ??listDirectory_9
//  197 			continue;
??listDirectory_8:
        B.N      ??listDirectory_3
//  198 		}
//  199 		scheduleMsg(&logger, "logfile%lu:%s", fno.fsize, fno.fname);
??listDirectory_9:
        ADD      R3,SP,#+9
        LDR      R2,[SP, #+0]
        LDR.N    R1,??DataTable13_27
        LDR.N    R0,??DataTable13_3
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  200 		count++;
        ADDS     R6,R6,#+1
        B.N      ??listDirectory_3
//  201 
//  202 //			scheduleMsg(&logger, "%c%c%c%c%c %u/%02u/%02u %02u:%02u %9lu  %-12s", (fno.fattrib & AM_DIR) ? 'D' : '-',
//  203 //					(fno.fattrib & AM_RDO) ? 'R' : '-', (fno.fattrib & AM_HID) ? 'H' : '-',
//  204 //					(fno.fattrib & AM_SYS) ? 'S' : '-', (fno.fattrib & AM_ARC) ? 'A' : '-', (fno.fdate >> 9) + 1980,
//  205 //					(fno.fdate >> 5) & 15, fno.fdate & 31, (fno.ftime >> 11), (fno.ftime >> 5) & 63, fno.fsize,
//  206 //					fno.fname);
//  207 	}
//  208 	unlockSpi();
??listDirectory_4:
          CFI FunCall _Z9unlockSpiv
        BL       _Z9unlockSpiv
//  209 }
??listDirectory_1:
        ADD      SP,SP,#+68
          CFI CFA R13+20
        POP      {R4-R7,PC}       ;; return
          CFI EndBlock cfiBlock7
//  210 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  211 static int errorReported = FALSE; // this is used to report the error only once
errorReported:
        DS8 4
//  212 
//  213 /**
//  214  * @brief Appends specified line to the current log file
//  215  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function appendToLog
        THUMB
//  216 void appendToLog(const char *line) {
appendToLog:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+8
          CFI CFA R13+24
        MOVS     R4,R0
//  217 	UINT bytesWrited;
//  218 
//  219 	if (!fs_ready) {
        LDR.N    R0,??DataTable13_11
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??appendToLog_0
//  220 		if (!errorReported)
        LDR.N    R0,??DataTable13_28
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??appendToLog_1
//  221 			scheduleMsg(&logger, "appendToLog Error: No File system is mounted");
        LDR.N    R1,??DataTable13_29
        LDR.N    R0,??DataTable13_3
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  222 		errorReported = TRUE;
??appendToLog_1:
        LDR.N    R0,??DataTable13_28
        MOVS     R1,#+1
        STR      R1,[R0, #+0]
//  223 		return;
        B.N      ??appendToLog_2
//  224 	}
//  225 	UINT lineLength = strlen(line);
??appendToLog_0:
        MOVS     R0,R4
          CFI FunCall strlen
        BL       strlen
        MOVS     R5,R0
//  226 	totalLoggedBytes += lineLength;
        LDR.N    R0,??DataTable13_10
        LDR      R0,[R0, #+0]
        ADDS     R0,R5,R0
        LDR.N    R1,??DataTable13_10
        STR      R0,[R1, #+0]
//  227 	lockSpi(SPI_NONE);
        MOVS     R0,#+0
          CFI FunCall _Z7lockSpi12spi_device_e
        BL       _Z7lockSpi12spi_device_e
//  228 	FRESULT err = f_write(&FDLogFile, line, lineLength, &bytesWrited);
        ADD      R3,SP,#+0
        MOVS     R2,R5
        MOVS     R1,R4
        LDR.N    R0,??DataTable13_19
          CFI FunCall f_write
        BL       f_write
        MOVS     R6,R0
//  229 	if (bytesWrited < lineLength) {
        LDR      R0,[SP, #+0]
        CMP      R0,R5
        BCS.N    ??appendToLog_3
//  230 		printError("write error or disk full", err); // error or disk full
        MOVS     R1,R6
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR.N    R0,??DataTable13_30
          CFI FunCall _Z10printErrorPKc7FRESULT
        BL       _Z10printErrorPKc7FRESULT
//  231 	}
//  232 	f_sync(&FDLogFile);
??appendToLog_3:
        LDR.N    R0,??DataTable13_19
          CFI FunCall f_sync
        BL       f_sync
//  233 	unlockSpi();
          CFI FunCall _Z9unlockSpiv
        BL       _Z9unlockSpiv
//  234 }
??appendToLog_2:
        POP      {R0,R1,R4-R6,PC}  ;; return
          CFI EndBlock cfiBlock8
//  235 
//  236 /*
//  237  * MMC card umount.
//  238  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z9MMCumountv
        THUMB
//  239 static void MMCumount(void) {
_Z9MMCumountv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  240 	if (!fs_ready) {
        LDR.N    R0,??DataTable13_11
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??MMCumount_0
//  241 		scheduleMsg(&logger, "Error: No File system is mounted. \"mountsd\" first");
        LDR.N    R1,??DataTable13_31
        LDR.N    R0,??DataTable13_3
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  242 		return;
        B.N      ??MMCumount_1
//  243 	}
//  244 	f_close(&FDLogFile);						// close file
??MMCumount_0:
        LDR.N    R0,??DataTable13_19
          CFI FunCall f_close
        BL       f_close
//  245 	f_sync(&FDLogFile);							// sync ALL
        LDR.N    R0,??DataTable13_19
          CFI FunCall f_sync
        BL       f_sync
//  246 	mmcDisconnect(&MMCD1);						// Brings the driver in a state safe for card removal.
        LDR.N    R0,??DataTable13_32
          CFI FunCall mmcDisconnect
        BL       mmcDisconnect
//  247 	mmcStop(&MMCD1);							// Disables the MMC peripheral.
        LDR.N    R0,??DataTable13_32
          CFI FunCall mmcStop
        BL       mmcStop
//  248 	f_mount(0, NULL);							// FATFS: Unregister work area prior to discard it
        MOVS     R1,#+0
        MOVS     R0,#+0
          CFI FunCall f_mount
        BL       f_mount
//  249 	memset(&FDLogFile, 0, sizeof(FIL));			// clear FDLogFile
        MOV      R2,#+548
        MOVS     R1,#+0
        LDR.N    R0,??DataTable13_19
          CFI FunCall memset
        BL       memset
//  250 	fs_ready = false;							// status = false
        LDR.N    R0,??DataTable13_11
        MOVS     R1,#+0
        STRB     R1,[R0, #+0]
//  251 	scheduleMsg(&logger, "MMC/SD card removed");
        LDR.N    R1,??DataTable13_33
        LDR.N    R0,??DataTable13_3
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  252 }
??MMCumount_1:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock9

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9:
        DC8      "rus"
//  253 
//  254 /*
//  255  * MMC card mount.
//  256  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z8MMCmountv
        THUMB
//  257 static void MMCmount(void) {
_Z8MMCmountv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  258 //	printMmcPinout();
//  259 
//  260 	if (fs_ready) {
        LDR.N    R0,??DataTable13_11
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??MMCmount_0
//  261 		scheduleMsg(&logger, "Error: Already mounted. \"umountsd\" first");
        LDR.N    R1,??DataTable13_34
        LDR.N    R0,??DataTable13_3
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  262 		return;
        B.N      ??MMCmount_1
//  263 	}
//  264 	// start to initialize MMC/SD
//  265 	mmcObjectInit(&MMCD1);						// Initializes an instance.
??MMCmount_0:
        LDR.N    R0,??DataTable13_32
          CFI FunCall mmcObjectInit
        BL       mmcObjectInit
//  266 	mmcStart(&MMCD1, &mmccfg);					// Configures and activates the MMC peripheral.
        LDR.N    R1,??DataTable13_35
        LDR.N    R0,??DataTable13_32
          CFI FunCall mmcStart
        BL       mmcStart
//  267 
//  268 	// Performs the initialization procedure on the inserted card.
//  269 	lockSpi(SPI_NONE);
        MOVS     R0,#+0
          CFI FunCall _Z7lockSpi12spi_device_e
        BL       _Z7lockSpi12spi_device_e
//  270 	if (mmcConnect(&MMCD1) != CH_SUCCESS) {
        LDR.N    R0,??DataTable13_32
          CFI FunCall mmcConnect
        BL       mmcConnect
        CMP      R0,#+0
        BEQ.N    ??MMCmount_2
//  271 		warning(OBD_PCM_Processor_Fault, "Can't connect or mount MMC/SD");
        LDR.N    R1,??DataTable13_36
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  272 		unlockSpi();
          CFI FunCall _Z9unlockSpiv
        BL       _Z9unlockSpiv
//  273 		return;
        B.N      ??MMCmount_1
//  274 
//  275 	}
//  276 	unlockSpi();
??MMCmount_2:
          CFI FunCall _Z9unlockSpiv
        BL       _Z9unlockSpiv
//  277 	// if Ok - mount FS now
//  278 	memset(&MMC_FS, 0, sizeof(FATFS));
        MOV      R2,#+560
        MOVS     R1,#+0
        LDR.N    R0,??DataTable13_37
          CFI FunCall memset
        BL       memset
        MOVS     R1,R0
//  279 	if (f_mount(0, &MMC_FS) == FR_OK) {
        MOVS     R0,#+0
          CFI FunCall f_mount
        BL       f_mount
        CMP      R0,#+0
        BNE.N    ??MMCmount_3
//  280 		incLogFileName();
          CFI FunCall _Z14incLogFileNamev
        BL       _Z14incLogFileNamev
//  281 		createLogFile();
          CFI FunCall _Z13createLogFilev
        BL       _Z13createLogFilev
//  282 		scheduleMsg(&logger, "MMC/SD mounted!");
        LDR.N    R1,??DataTable13_38
        LDR.N    R0,??DataTable13_3
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  283 	}
//  284 }
??MMCmount_3:
??MMCmount_1:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock10
//  285 
//  286 #if defined __GNUC__
//  287 __attribute__((noreturn))        static msg_t MMCmonThread(void)
//  288 #else

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z12MMCmonThreadv
        THUMB
//  289 static msg_t MMCmonThread(void)
//  290 #endif
//  291 {
_Z12MMCmonThreadv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  292 	chRegSetThreadName("MMC_Monitor");
        LDR.N    R0,??DataTable13_39
        LDR      R0,[R0, #+24]
        LDR.N    R1,??DataTable13_40
        STR      R1,[R0, #+24]
//  293 
//  294 	while (true) {
//  295 		// this returns TRUE if SD module is there, even without an SD card?
//  296 		if (blkIsInserted(&MMCD1)) {
??MMCmonThread_0:
        LDR.N    R0,??DataTable13_32
        LDR.N    R1,??DataTable13_32
        LDR      R1,[R1, #+0]
        LDR      R1,[R1, #+0]
          CFI FunCall
        BLX      R1
        CMP      R0,#+0
        BEQ.N    ??MMCmonThread_1
//  297 
//  298 			if (!fs_ready) {
        LDR.N    R0,??DataTable13_11
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??MMCmonThread_1
//  299 				MMCmount();
          CFI FunCall _Z8MMCmountv
        BL       _Z8MMCmountv
//  300 			}
//  301 		}
//  302 
//  303 		if (isSdCardAlive())
??MMCmonThread_1:
          CFI FunCall isSdCardAlive
        BL       isSdCardAlive
        CMP      R0,#+0
        BEQ.N    ??MMCmonThread_2
//  304 			writeLogLine();
          CFI FunCall _Z12writeLogLinev
        BL       _Z12writeLogLinev
//  305 
//  306 		chThdSleepMilliseconds(FILE_LOG_DELAY);
??MMCmonThread_2:
        MOVS     R0,#+200
          CFI FunCall chThdSleep
        BL       chThdSleep
        B.N      ??MMCmonThread_0
          CFI EndBlock cfiBlock11
//  307 	}
//  308 }
//  309 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function isSdCardAlive
          CFI NoCalls
        THUMB
//  310 bool isSdCardAlive(void) {
//  311 	return fs_ready;
isSdCardAlive:
        LDR.N    R0,??DataTable13_11
        LDRB     R0,[R0, #+0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock12
//  312 }

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function initMmcCard
        THUMB
initMmcCard:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        LDR.N    R0,??DataTable13_7
        MOVS     R1,#+0
        STRB     R1,[R0, #+0]
        LDR.N    R1,??DataTable13_41
        LDR.N    R0,??DataTable13_42
          CFI FunCall addConsoleAction
        BL       addConsoleAction
        LDR.N    R0,??DataTable13_5
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+384]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+3
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??initMmcCard_0
??initMmcCard_1:
        LDR.N    R0,??DataTable13_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+324]
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        LDR.N    R1,??DataTable13_43
        STR      R0,[R1, #+4]
        LDR.N    R1,??DataTable13_44
        STR      R0,[R1, #+4]
        LDR.N    R0,??DataTable13_5
        LDR      R0,[R0, #+0]
        LDR      R0,[R0, #+324]
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        LDR.N    R1,??DataTable13_43
        STRH     R0,[R1, #+8]
        LDR.N    R1,??DataTable13_44
        STRH     R0,[R1, #+8]
        LDR.N    R0,??DataTable13_32
          CFI FunCall mmcObjectInit
        BL       mmcObjectInit
        LDR.N    R1,??DataTable13_35
        LDR.N    R0,??DataTable13_32
          CFI FunCall mmcStart
        BL       mmcStart
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        LDR.N    R3,??DataTable13_45
        MOVS     R2,#+2
        MOV      R1,#+712
        LDR.N    R0,??DataTable13_46
          CFI FunCall chThdCreateStatic
        BL       chThdCreateStatic
        LDR.N    R1,??DataTable13_47
        LDR.N    R0,??DataTable13_48
          CFI FunCall addConsoleAction
        BL       addConsoleAction
        LDR.N    R1,??DataTable13_49
        LDR.N    R0,??DataTable13_50
          CFI FunCall addConsoleActionS
        BL       addConsoleActionS
        LDR.N    R1,??DataTable13_51
        LDR.N    R0,??DataTable13_52
          CFI FunCall addConsoleAction
        BL       addConsoleAction
        LDR.N    R1,??DataTable13_53
        ADR.N    R0,??DataTable13  ;; 0x6C, 0x73, 0x00, 0x00
          CFI FunCall addConsoleActionS
        BL       addConsoleActionS
        LDR.N    R1,??DataTable13_54
        ADR.N    R0,??DataTable13_1  ;; "del"
          CFI FunCall addConsoleActionS
        BL       addConsoleActionS
        LDR.N    R1,??DataTable13_55
        LDR.N    R0,??DataTable13_56
          CFI FunCall addConsoleAction
        BL       addConsoleAction
??initMmcCard_0:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock13

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13:
        DC8      0x6C, 0x73, 0x00, 0x00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_1:
        DC8      "del"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_2:
        DC32     `?<Constant "mmcCard">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_3:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_4:
        DC32     `?<Constant "FATfs Error \\"%s\\" %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_5:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_6:
        DC32     `?<Constant "MMC CS %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_7:
        DC32     logName

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_8:
        DC32     `?<Constant "SD enabled: %s [%s]">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_9:
        DC32     logFileIndex

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_10:
        DC32     totalLoggedBytes

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_11:
        DC32     fs_ready

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_12:
        DC32     `?<Constant "fs_ready=%d totalLogg...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_13:
        DC32     FDCurrFile

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_14:
        DC32     `?<Constant "index.txt">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_15:
        DC32     `?<Constant "%s: not found or erro...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_16:
        DC32     `?<Constant "Got content [%s] size %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_17:
        DC32     0x128ce430

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_18:
        DC32     `?<Constant "Done %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_19:
        DC32     FDLogFile

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_20:
        DC32     logName+0x3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_21:
        DC32     `?<Constant ".log">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_22:
        DC32     `?<Constant "FS mount failed">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_23:
        DC32     `?<Constant "Seek error">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_24:
        DC32     `?<Constant "Error: No File system...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_25:
        DC32     `?<Constant "Error opening directo...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_26:
        DC32     `?<Constant "ls_result">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_27:
        DC32     `?<Constant "logfile%lu:%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_28:
        DC32     errorReported

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_29:
        DC32     `?<Constant "appendToLog Error: No...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_30:
        DC32     `?<Constant "write error or disk full">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_31:
        DC32     `?<Constant "Error: No File system...">_1`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_32:
        DC32     MMCD1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_33:
        DC32     `?<Constant "MMC/SD card removed">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_34:
        DC32     `?<Constant "Error: Already mounte...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_35:
        DC32     mmccfg

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_36:
        DC32     `?<Constant "Can\\'t connect or moun...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_37:
        DC32     MMC_FS

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_38:
        DC32     `?<Constant "MMC/SD mounted!">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_39:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_40:
        DC32     `?<Constant "MMC_Monitor">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_41:
        DC32     _Z12sdStatisticsv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_42:
        DC32     `?<Constant "sdinfo">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_43:
        DC32     ls_spicfg

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_44:
        DC32     hs_spicfg

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_45:
        DC32     _Z12MMCmonThreadv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_46:
        DC32     mmcThreadStack

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_47:
        DC32     _Z8MMCmountv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_48:
        DC32     `?<Constant "mountsd">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_49:
        DC32     appendToLog

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_50:
        DC32     `?<Constant "appendToLog">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_51:
        DC32     _Z9MMCumountv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_52:
        DC32     `?<Constant "umountsd">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_53:
        DC32     _Z13listDirectoryPKc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_54:
        DC32     _Z10removeFilePKc

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_55:
        DC32     _Z14incLogFileNamev

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable13_56:
        DC32     `?<Constant "incfilename">`

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memset
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function memset
        THUMB
// __intrinsic __nounwind __interwork __softfp void *memset(void *, int, size_t)
memset:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R2,R5
        MOVS     R1,R6
        MOVS     R0,R4
          CFI FunCall __aeabi_memset
        BL       __aeabi_memset
        MOVS     R0,R4
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock14

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
`?<Constant "FATfs Error \\"%s\\" %d">`:
        DATA
        DC8 "FATfs Error \"%s\" %d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MMC CS %s">`:
        DATA
        DC8 "MMC CS %s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "SD enabled: %s [%s]">`:
        DATA
        DC8 "SD enabled: %s [%s]"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fs_ready=%d totalLogg...">`:
        DATA
        DC8 "fs_ready=%d totalLoggedBytes=%d %d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "index.txt">`:
        DATA
        DC8 "index.txt"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "%s: not found or erro...">`:
        DATA
        DC8 "%s: not found or error: %d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Got content [%s] size %d">`:
        DATA
        DC8 "Got content [%s] size %d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Done %d">`:
        DATA
        DC8 "Done %d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "rus"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant ".log">`:
        DATA
        DC8 ".log"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FS mount failed">`:
        DATA
        DC8 "FS mount failed"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Seek error">`:
        DATA
        DC8 "Seek error"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Error: No File system...">`:
        DATA
        DC8 "Error: No File system is mounted"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Error opening directo...">`:
        DATA
        DC8 "Error opening directory %s"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "ls_result">`:
        DATA
        DC8 "ls_result"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "logfile%lu:%s">`:
        DATA
        DC8 "logfile%lu:%s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "appendToLog Error: No...">`:
        DATA
        DC8 "appendToLog Error: No File system is mounted"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "write error or disk full">`:
        DATA
        DC8 "write error or disk full"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Error: No File system...">_1`:
        DATA
        DC8 "Error: No File system is mounted. \"mountsd\" first"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MMC/SD card removed">`:
        DATA
        DC8 "MMC/SD card removed"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Error: Already mounte...">`:
        DATA
        DC8 "Error: Already mounted. \"umountsd\" first"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Can\\'t connect or moun...">`:
        DATA
        DC8 "Can't connect or mount MMC/SD"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MMC/SD mounted!">`:
        DATA
        DC8 "MMC/SD mounted!"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MMC_Monitor">`:
        DATA
        DC8 "MMC_Monitor"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "sdinfo">`:
        DATA
        DC8 "sdinfo"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "mountsd">`:
        DATA
        DC8 "mountsd"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "appendToLog">`:
        DATA
        DC8 "appendToLog"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "umountsd">`:
        DATA
        DC8 "umountsd"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "ls"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "del"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "incfilename">`:
        DATA
        DC8 "incfilename"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "mmcCard">`:
        DATA
        DC8 "mmcCard"

        END
//  313 
//  314 void initMmcCard(void) {
//  315 	logName[0] = 0;
//  316 	addConsoleAction("sdinfo", sdStatistics);
//  317 	if (!boardConfiguration->isSdCardEnabled) {
//  318 		return;
//  319 	}
//  320 
//  321 	hs_spicfg.ssport = ls_spicfg.ssport = getHwPort(boardConfiguration->sdCardCsPin);
//  322 	hs_spicfg.sspad = ls_spicfg.sspad = getHwPin(boardConfiguration->sdCardCsPin);
//  323 
//  324 	/**
//  325 	 * FYI: SPI does not work with CCM memory, be sure to have main() stack in RAM, not in CCMRAM
//  326 	 */
//  327 
//  328 	// start to initialize MMC/SD
//  329 	mmcObjectInit(&MMCD1);
//  330 	mmcStart(&MMCD1, &mmccfg);
//  331 
//  332 	chThdCreateStatic(mmcThreadStack, sizeof(mmcThreadStack), LOWPRIO, (tfunc_t) MMCmonThread, NULL);
//  333 
//  334 	addConsoleAction("mountsd", MMCmount);
//  335 	addConsoleActionS("appendToLog", appendToLog);
//  336 	addConsoleAction("umountsd", MMCumount);
//  337 	addConsoleActionS("ls", listDirectory);
//  338 	addConsoleActionS("del", removeFile);
//  339 	addConsoleAction("incfilename", incLogFileName);
//  340 }
//  341 
//  342 #endif /* EFI_FILE_LOGGING */
// 
// 2 705 bytes in section .bss
//    40 bytes in section .data
//     4 bytes in section .init_array
//   616 bytes in section .rodata
// 1 578 bytes in section .text
// 
// 1 560 bytes of CODE  memory (+ 22 bytes shared)
//   616 bytes of CONST memory
// 2 745 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
