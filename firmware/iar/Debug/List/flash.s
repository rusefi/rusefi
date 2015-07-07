///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:24 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\flash. /
//                       c                                                    /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\flash. /
//                       c -lCN F:\stuff\rusefi_sourceforge\firmware\iar\Debu /
//                       g\List\ -lA F:\stuff\rusefi_sourceforge\firmware\iar /
//                       \Debug\List\ -o F:\stuff\rusefi_sourceforge\firmware /
//                       \iar\Debug\Obj\ --no_cse --no_unroll --no_inline     /
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
//                       -On                                                  /
//    MISRA C version =  2004                                                 /
//      Enabled       =  1.2-1.4 2.3 3.4 4.2 6.5 7 8.2 8.3 9.3 12.3 13.6      /
//                       13.7 15.5 17.3 19.17 20.7-20.9 20.11                 /
//      Checked       =  1.4 2.3 4.2 6.5 7 8.2 8.3 9.3 12.3 13.7 15.5         /
//                       20.7-20.9 20.11                                      /
//      Not checked   =  1.1-1.3 1.5-2.2 2.4-4.1 5-6.4 8.1 8.4-9.2 10-12.2    /
//                       12.4-13.6 14-15.4 16-20.6 20.10 20.12 21             /
//    List file       =  F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                       flash.s                                              /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME flash

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN __aeabi_memcpy

        PUBLIC flashCompare
        PUBLIC flashErase
        PUBLIC flashIsErased
        PUBLIC flashRead
        PUBLIC flashSectorAt
        PUBLIC flashSectorBegin
        PUBLIC flashSectorEnd
        PUBLIC flashSectorErase
        PUBLIC flashSectorSize
        PUBLIC flashWrite
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
        
// F:\stuff\rusefi_sourceforge\firmware\hw_layer\flash.c
//    1 /**
//    2  * @file    flash_main.c
//    3  * @brief	Lower-level code related to internal flash memory
//    4  */
//    5 
//    6 #include "main.h"

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memcpy
          CFI Block cfiBlock0 Using cfiCommon0
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
          CFI EndBlock cfiBlock0
//    7 
//    8 #if EFI_INTERNAL_FLASH || defined(__DOXYGEN__)
//    9 
//   10 #include "flash.h"
//   11 #include <string.h>
//   12 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function flashSectorSize
          CFI NoCalls
        THUMB
//   13 size_t flashSectorSize(flashsector_t sector) {
flashSectorSize:
        MOVS     R1,R0
//   14 	if (sector <= 3)
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+4
        BGE.N    ??flashSectorSize_0
//   15 		return 16 * 1024;
        MOV      R0,#+16384
        B.N      ??flashSectorSize_1
//   16 	else if (sector == 4)
??flashSectorSize_0:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+4
        BNE.N    ??flashSectorSize_2
//   17 		return 64 * 1024;
        MOVS     R0,#+65536
        B.N      ??flashSectorSize_1
//   18 	else if (sector >= 5 && sector <= 11)
??flashSectorSize_2:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+5
        BLT.N    ??flashSectorSize_3
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+12
        BGE.N    ??flashSectorSize_3
//   19 		return 128 * 1024;
        MOVS     R0,#+131072
        B.N      ??flashSectorSize_1
//   20 	return 0;
??flashSectorSize_3:
        MOVS     R0,#+0
??flashSectorSize_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   21 }
//   22 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function flashSectorBegin
        THUMB
//   23 flashaddr_t flashSectorBegin(flashsector_t sector) {
flashSectorBegin:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//   24 	flashaddr_t address = FLASH_BASE;
        MOVS     R5,#+134217728
//   25 	while (sector > 0) {
??flashSectorBegin_0:
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+0
        BEQ.N    ??flashSectorBegin_1
//   26 		--sector;
        SUBS     R4,R4,#+1
//   27 		address += flashSectorSize(sector);
        MOVS     R0,R4
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall flashSectorSize
        BL       flashSectorSize
        ADDS     R0,R0,R5
        MOVS     R5,R0
        B.N      ??flashSectorBegin_0
//   28 	}
//   29 	return address;
??flashSectorBegin_1:
        MOVS     R0,R5
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2
//   30 }
//   31 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function flashSectorEnd
        THUMB
//   32 flashaddr_t flashSectorEnd(flashsector_t sector) {
flashSectorEnd:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   33 	return flashSectorBegin(sector + 1);
        ADDS     R0,R4,#+1
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall flashSectorBegin
        BL       flashSectorBegin
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//   34 }
//   35 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function flashSectorAt
        THUMB
//   36 flashsector_t flashSectorAt(flashaddr_t address) {
flashSectorAt:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//   37 	flashsector_t sector = 0;
        MOVS     R5,#+0
//   38 	while (address >= flashSectorEnd(sector))
??flashSectorAt_0:
        MOVS     R0,R5
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall flashSectorEnd
        BL       flashSectorEnd
        CMP      R4,R0
        BCC.N    ??flashSectorAt_1
//   39 		++sector;
        ADDS     R5,R5,#+1
        B.N      ??flashSectorAt_0
//   40 	return sector;
??flashSectorAt_1:
        MOVS     R0,R5
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock4
//   41 }
//   42 
//   43 /**
//   44  * @brief Wait for the flash operation to finish.
//   45  */
//   46 #define flashWaitWhileBusy() { while (FLASH->SR & FLASH_SR_BSY) {} }
//   47 
//   48 /**
//   49  * @brief Unlock the flash memory for write access.
//   50  * @return CH_SUCCESS  Unlock was successful.
//   51  * @return CH_FAILED    Unlock failed.
//   52  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function flashUnlock
          CFI NoCalls
        THUMB
//   53 static bool flashUnlock(void) {
//   54 	/* Check if unlock is really needed */
//   55 	if (!(FLASH->CR & FLASH_CR_LOCK))
flashUnlock:
        LDR.N    R0,??DataTable3  ;; 0x40023c10
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BMI.N    ??flashUnlock_0
//   56 		return CH_SUCCESS;
        MOVS     R0,#+0
        B.N      ??flashUnlock_1
//   57 
//   58 	/* Write magic unlock sequence */
//   59 	FLASH->KEYR = 0x45670123;
??flashUnlock_0:
        LDR.N    R0,??DataTable3_1  ;; 0x40023c04
        LDR.N    R1,??DataTable3_2  ;; 0x45670123
        STR      R1,[R0, #+0]
//   60 	FLASH->KEYR = 0xCDEF89AB;
        LDR.N    R0,??DataTable3_1  ;; 0x40023c04
        LDR.N    R1,??DataTable3_3  ;; 0xcdef89ab
        STR      R1,[R0, #+0]
//   61 
//   62 	/* Check if unlock was successful */
//   63 	if (FLASH->CR & FLASH_CR_LOCK)
        LDR.N    R0,??DataTable3  ;; 0x40023c10
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BPL.N    ??flashUnlock_2
//   64 		return CH_FAILED;
        MOVS     R0,#+1
        B.N      ??flashUnlock_1
//   65 	return CH_SUCCESS;
??flashUnlock_2:
        MOVS     R0,#+0
??flashUnlock_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock5
//   66 }
//   67 
//   68 /**
//   69  * @brief Lock the flash memory for write access.
//   70  */
//   71 #define flashLock() { FLASH->CR |= FLASH_CR_LOCK; }
//   72 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function flashSectorErase
        THUMB
//   73 int flashSectorErase(flashsector_t sector) {
flashSectorErase:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//   74 	/* Unlock flash for write access */
//   75 	if (flashUnlock() == CH_FAILED)
          CFI FunCall flashUnlock
        BL       flashUnlock
        CMP      R0,#+0
        BEQ.N    ??flashSectorErase_0
//   76 		return FLASH_RETURN_NO_PERMISSION;
        MOVS     R0,#-1
        B.N      ??flashSectorErase_1
//   77 
//   78 	/* Wait for any busy flags. */
//   79 	flashWaitWhileBusy();
??flashSectorErase_0:
        LDR.N    R0,??DataTable3_4  ;; 0x40023c0c
        LDR      R0,[R0, #+0]
        LSLS     R0,R0,#+15
        BMI.N    ??flashSectorErase_0
//   80 
//   81 	/* Setup parallelism before any program/erase */
//   82 	FLASH->CR &= ~FLASH_CR_PSIZE_MASK;
        LDR.N    R0,??DataTable3  ;; 0x40023c10
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x100
        LDR.N    R1,??DataTable3  ;; 0x40023c10
        STR      R0,[R1, #+0]
//   83 	FLASH->CR |= FLASH_CR_PSIZE_VALUE;
        LDR.N    R0,??DataTable3  ;; 0x40023c10
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x200
        LDR.N    R1,??DataTable3  ;; 0x40023c10
        STR      R0,[R1, #+0]
//   84 
//   85 	/* Start deletion of sector.
//   86 	 * SNB(3:1) is defined as:
//   87 	 * 0000 sector 0
//   88 	 * 0001 sector 1
//   89 	 * ...
//   90 	 * 1011 sector 11
//   91 	 * others not allowed */
//   92 	FLASH->CR &= ~(FLASH_CR_SNB_0 | FLASH_CR_SNB_1 | FLASH_CR_SNB_2 | FLASH_CR_SNB_3);
        LDR.N    R0,??DataTable3  ;; 0x40023c10
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x78
        LDR.N    R1,??DataTable3  ;; 0x40023c10
        STR      R0,[R1, #+0]
//   93 	if (sector & 0x1)
        LSLS     R0,R4,#+31
        BPL.N    ??flashSectorErase_2
//   94 		FLASH->CR |= FLASH_CR_SNB_0;
        LDR.N    R0,??DataTable3  ;; 0x40023c10
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x8
        LDR.N    R1,??DataTable3  ;; 0x40023c10
        STR      R0,[R1, #+0]
//   95 	if (sector & 0x2)
??flashSectorErase_2:
        LSLS     R0,R4,#+30
        BPL.N    ??flashSectorErase_3
//   96 		FLASH->CR |= FLASH_CR_SNB_1;
        LDR.N    R0,??DataTable3  ;; 0x40023c10
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x10
        LDR.N    R1,??DataTable3  ;; 0x40023c10
        STR      R0,[R1, #+0]
//   97 	if (sector & 0x4)
??flashSectorErase_3:
        LSLS     R0,R4,#+29
        BPL.N    ??flashSectorErase_4
//   98 		FLASH->CR |= FLASH_CR_SNB_2;
        LDR.N    R0,??DataTable3  ;; 0x40023c10
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x20
        LDR.N    R1,??DataTable3  ;; 0x40023c10
        STR      R0,[R1, #+0]
//   99 	if (sector & 0x8)
??flashSectorErase_4:
        LSLS     R0,R4,#+28
        BPL.N    ??flashSectorErase_5
//  100 		FLASH->CR |= FLASH_CR_SNB_3;
        LDR.N    R0,??DataTable3  ;; 0x40023c10
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x40
        LDR.N    R1,??DataTable3  ;; 0x40023c10
        STR      R0,[R1, #+0]
//  101 	FLASH->CR |= FLASH_CR_SER;
??flashSectorErase_5:
        LDR.N    R0,??DataTable3  ;; 0x40023c10
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x2
        LDR.N    R1,??DataTable3  ;; 0x40023c10
        STR      R0,[R1, #+0]
//  102 	FLASH->CR |= FLASH_CR_STRT;
        LDR.N    R0,??DataTable3  ;; 0x40023c10
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x10000
        LDR.N    R1,??DataTable3  ;; 0x40023c10
        STR      R0,[R1, #+0]
//  103 
//  104 	/* Wait until it's finished. */
//  105 	flashWaitWhileBusy();
??flashSectorErase_6:
        LDR.N    R0,??DataTable3_4  ;; 0x40023c0c
        LDR      R0,[R0, #+0]
        LSLS     R0,R0,#+15
        BMI.N    ??flashSectorErase_6
//  106 
//  107 	/* Sector erase flag does not clear automatically. */
//  108 	FLASH->CR &= ~FLASH_CR_SER;
        LDR.N    R0,??DataTable3  ;; 0x40023c10
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x2
        LDR.N    R1,??DataTable3  ;; 0x40023c10
        STR      R0,[R1, #+0]
//  109 
//  110 	/* Lock flash again */
//  111 	flashLock()
        LDR.N    R0,??DataTable3  ;; 0x40023c10
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x80000000
        LDR.N    R1,??DataTable3  ;; 0x40023c10
        STR      R0,[R1, #+0]
//  112 	;
//  113 
//  114 	/* Check deleted sector for errors */
//  115 	if (flashIsErased(flashSectorBegin(sector), flashSectorSize(sector)) == FALSE)
        MOVS     R0,R4
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall flashSectorSize
        BL       flashSectorSize
        MOVS     R5,R0
        MOVS     R0,R4
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall flashSectorBegin
        BL       flashSectorBegin
        MOVS     R1,R5
          CFI FunCall flashIsErased
        BL       flashIsErased
        CMP      R0,#+0
        BNE.N    ??flashSectorErase_7
//  116 		return FLASH_RETURN_BAD_FLASH; /* Sector is not empty despite the erase cycle! */
        MVNS     R0,#+10
        B.N      ??flashSectorErase_1
//  117 
//  118 	/* Successfully deleted sector */
//  119 	return FLASH_RETURN_SUCCESS;
??flashSectorErase_7:
        MOVS     R0,#+0
??flashSectorErase_1:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock6
//  120 }
//  121 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function flashErase
        THUMB
//  122 int flashErase(flashaddr_t address, size_t size) {
flashErase:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  123 	while (size > 0) {
??flashErase_0:
        CMP      R5,#+0
        BEQ.N    ??flashErase_1
//  124 		flashsector_t sector = flashSectorAt(address);
        MOVS     R0,R4
          CFI FunCall flashSectorAt
        BL       flashSectorAt
        MOVS     R6,R0
//  125 		int err = flashSectorErase(sector);
        MOVS     R0,R6
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall flashSectorErase
        BL       flashSectorErase
        MOVS     R7,R0
//  126 		if (err != FLASH_RETURN_SUCCESS)
        CMP      R7,#+0
        BEQ.N    ??flashErase_2
//  127 			return err;
        MOVS     R0,R7
        B.N      ??flashErase_3
//  128 		address = flashSectorEnd(sector);
??flashErase_2:
        MOVS     R0,R6
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall flashSectorEnd
        BL       flashSectorEnd
        MOVS     R4,R0
//  129 		size_t sector_size = flashSectorSize(sector);
        MOVS     R0,R6
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall flashSectorSize
        BL       flashSectorSize
//  130 		if (sector_size >= size)
        CMP      R0,R5
        BCS.N    ??flashErase_1
//  131 			break;
//  132 		size -= sector_size;
??flashErase_4:
        SUBS     R5,R5,R0
        B.N      ??flashErase_0
//  133 	}
//  134 
//  135 	return FLASH_RETURN_SUCCESS;
??flashErase_1:
        MOVS     R0,#+0
??flashErase_3:
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock7
//  136 }
//  137 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function flashIsErased
          CFI NoCalls
        THUMB
//  138 bool flashIsErased(flashaddr_t address, size_t size) {
flashIsErased:
        MOVS     R2,R0
//  139 	/* Check for default set bits in the flash memory
//  140 	 * For efficiency, compare flashdata_t values as much as possible,
//  141 	 * then, fallback to byte per byte comparison. */
//  142 	while (size >= sizeof(flashdata_t)) {
??flashIsErased_0:
        CMP      R1,#+4
        BCC.N    ??flashIsErased_1
//  143 		if (*(volatile flashdata_t*) address != (flashdata_t) (-1)) // flashdata_t being unsigned, -1 is 0xFF..FF
        LDR      R0,[R2, #+0]
        CMN      R0,#+1
        BEQ.N    ??flashIsErased_2
//  144 			return false;
        MOVS     R0,#+0
        B.N      ??flashIsErased_3
//  145 		address += sizeof(flashdata_t);
??flashIsErased_2:
        ADDS     R2,R2,#+4
//  146 		size -= sizeof(flashdata_t);
        SUBS     R1,R1,#+4
        B.N      ??flashIsErased_0
//  147 	}
//  148 	while (size > 0) {
??flashIsErased_1:
        CMP      R1,#+0
        BEQ.N    ??flashIsErased_4
//  149 		if (*(char*) address != 0xFF)
        LDRB     R0,[R2, #+0]
        CMP      R0,#+255
        BEQ.N    ??flashIsErased_5
//  150 			return false;
        MOVS     R0,#+0
        B.N      ??flashIsErased_3
//  151 		++address;
??flashIsErased_5:
        ADDS     R2,R2,#+1
//  152 		--size;
        SUBS     R1,R1,#+1
        B.N      ??flashIsErased_1
//  153 	}
//  154 
//  155 	return TRUE;
??flashIsErased_4:
        MOVS     R0,#+1
??flashIsErased_3:
        BX       LR               ;; return
          CFI EndBlock cfiBlock8
//  156 }
//  157 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function flashCompare
          CFI NoCalls
        THUMB
//  158 bool flashCompare(flashaddr_t address, const char* buffer, size_t size) {
flashCompare:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
        MOVS     R3,R0
//  159 	/* For efficiency, compare flashdata_t values as much as possible,
//  160 	 * then, fallback to byte per byte comparison. */
//  161 	while (size >= sizeof(flashdata_t)) {
??flashCompare_0:
        CMP      R2,#+4
        BCC.N    ??flashCompare_1
//  162 		if (*(volatile flashdata_t*) address != *(flashdata_t*) buffer)
        LDR      R0,[R3, #+0]
        LDR      R4,[R1, #+0]
        CMP      R0,R4
        BEQ.N    ??flashCompare_2
//  163 			return FALSE;
        MOVS     R0,#+0
        B.N      ??flashCompare_3
//  164 		address += sizeof(flashdata_t);
??flashCompare_2:
        ADDS     R3,R3,#+4
//  165 		buffer += sizeof(flashdata_t);
        ADDS     R1,R1,#+4
//  166 		size -= sizeof(flashdata_t);
        SUBS     R2,R2,#+4
        B.N      ??flashCompare_0
//  167 	}
//  168 	while (size > 0) {
??flashCompare_1:
        CMP      R2,#+0
        BEQ.N    ??flashCompare_4
//  169 		if (*(volatile char*) address != *buffer)
        LDRB     R0,[R3, #+0]
        LDRB     R4,[R1, #+0]
        CMP      R0,R4
        BEQ.N    ??flashCompare_5
//  170 			return FALSE;
        MOVS     R0,#+0
        B.N      ??flashCompare_3
//  171 		++address;
??flashCompare_5:
        ADDS     R3,R3,#+1
//  172 		++buffer;
        ADDS     R1,R1,#+1
//  173 		--size;
        SUBS     R2,R2,#+1
        B.N      ??flashCompare_1
//  174 	}
//  175 
//  176 	return TRUE;
??flashCompare_4:
        MOVS     R0,#+1
??flashCompare_3:
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock9
//  177 }
//  178 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function flashRead
        THUMB
//  179 int flashRead(flashaddr_t address, char* buffer, size_t size) {
flashRead:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  180 	memcpy(buffer, (char*) address, size);
        MOVS     R2,R6
        MOVS     R1,R4
        MOVS     R0,R5
          CFI FunCall memcpy
        BL       memcpy
//  181 	return FLASH_RETURN_SUCCESS;
        MOVS     R0,#+0
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock10
//  182 }
//  183 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function flashWriteData
          CFI NoCalls
        THUMB
//  184 static void flashWriteData(flashaddr_t address, const flashdata_t data) {
//  185 	/* Enter flash programming mode */
//  186 	FLASH->CR |= FLASH_CR_PG;
flashWriteData:
        LDR.N    R2,??DataTable3  ;; 0x40023c10
        LDR      R2,[R2, #+0]
        ORRS     R2,R2,#0x1
        LDR.N    R3,??DataTable3  ;; 0x40023c10
        STR      R2,[R3, #+0]
//  187 
//  188 	/* Write the data */
//  189 	*(flashdata_t*) address = data;
        STR      R1,[R0, #+0]
//  190 
//  191 	/* Wait for completion */
//  192 	flashWaitWhileBusy();
??flashWriteData_0:
        LDR.N    R2,??DataTable3_4  ;; 0x40023c0c
        LDR      R2,[R2, #+0]
        LSLS     R2,R2,#+15
        BMI.N    ??flashWriteData_0
//  193 
//  194 	/* Exit flash programming mode */
//  195 	FLASH->CR &= ~FLASH_CR_PG;
        LDR.N    R2,??DataTable3  ;; 0x40023c10
        LDR      R2,[R2, #+0]
        LSRS     R2,R2,#+1
        LSLS     R2,R2,#+1
        LDR.N    R3,??DataTable3  ;; 0x40023c10
        STR      R2,[R3, #+0]
//  196 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock11
//  197 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function flashWrite
        THUMB
//  198 int flashWrite(flashaddr_t address, const char* buffer, size_t size) {
flashWrite:
        PUSH     {R4-R9,LR}
          CFI R14 Frame(CFA, -4)
          CFI R9 Frame(CFA, -8)
          CFI R8 Frame(CFA, -12)
          CFI R7 Frame(CFA, -16)
          CFI R6 Frame(CFA, -20)
          CFI R5 Frame(CFA, -24)
          CFI R4 Frame(CFA, -28)
          CFI CFA R13+28
        SUB      SP,SP,#+12
          CFI CFA R13+40
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  199 	/* Unlock flash for write access */
//  200 	if (flashUnlock() == CH_FAILED)
          CFI FunCall flashUnlock
        BL       flashUnlock
        CMP      R0,#+0
        BEQ.N    ??flashWrite_0
//  201 		return FLASH_RETURN_NO_PERMISSION;
        MOVS     R0,#-1
        B.N      ??flashWrite_1
//  202 
//  203 	/* Wait for any busy flags */
//  204 	flashWaitWhileBusy();
??flashWrite_0:
        LDR.N    R0,??DataTable3_4  ;; 0x40023c0c
        LDR      R0,[R0, #+0]
        LSLS     R0,R0,#+15
        BMI.N    ??flashWrite_0
//  205 
//  206 	/* Setup parallelism before any program/erase */
//  207 	FLASH->CR &= ~FLASH_CR_PSIZE_MASK;
        LDR.N    R0,??DataTable3  ;; 0x40023c10
        LDR      R0,[R0, #+0]
        BICS     R0,R0,#0x100
        LDR.N    R1,??DataTable3  ;; 0x40023c10
        STR      R0,[R1, #+0]
//  208 	FLASH->CR |= FLASH_CR_PSIZE_VALUE;
        LDR.N    R0,??DataTable3  ;; 0x40023c10
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x200
        LDR.N    R1,??DataTable3  ;; 0x40023c10
        STR      R0,[R1, #+0]
//  209 
//  210 	/* Check if the flash address is correctly aligned */
//  211 	size_t alignOffset = address % sizeof(flashdata_t);
        MOVS     R0,#+4
        UDIV     R7,R4,R0
        MLS      R7,R7,R0,R4
//  212 //	print("flash alignOffset=%d\r\n", alignOffset);
//  213 	if (alignOffset != 0) {
        CMP      R7,#+0
        BEQ.N    ??flashWrite_2
//  214 		/* Not aligned, thus we have to read the data in flash already present
//  215 		 * and update them with buffer's data */
//  216 
//  217 		/* Align the flash address correctly */
//  218 		flashaddr_t alignedFlashAddress = address - alignOffset;
        SUBS     R8,R4,R7
//  219 
//  220 		/* Read already present data */
//  221 		flashdata_t tmp = *(volatile flashdata_t*) alignedFlashAddress;
        LDR      R0,[R8, #+0]
        STR      R0,[SP, #+4]
//  222 
//  223 		/* Compute how much bytes one must update in the data read */
//  224 		size_t chunkSize = sizeof(flashdata_t) - alignOffset;
        RSBS     R9,R7,#+4
//  225 		if (chunkSize > size)
        CMP      R6,R9
        BCS.N    ??flashWrite_3
//  226 			chunkSize = size; // this happens when both address and address + size are not aligned
        MOV      R9,R6
//  227 
//  228 		/* Update the read data with buffer's data */
//  229 		memcpy((char*) &tmp + alignOffset, buffer, chunkSize);
??flashWrite_3:
        MOV      R2,R9
        MOVS     R1,R5
        ADD      R0,SP,#+4
        ADDS     R0,R7,R0
          CFI FunCall memcpy
        BL       memcpy
//  230 
//  231 		/* Write the new data in flash */
//  232 		flashWriteData(alignedFlashAddress, tmp);
        LDR      R1,[SP, #+4]
        MOV      R0,R8
          CFI FunCall flashWriteData
        BL       flashWriteData
//  233 
//  234 		/* Advance */
//  235 		address += chunkSize;
        ADDS     R4,R9,R4
//  236 		buffer += chunkSize;
        ADDS     R5,R9,R5
//  237 		size -= chunkSize;
        SUBS     R6,R6,R9
//  238 	}
//  239 
//  240 	/* Now, address is correctly aligned. One can copy data directly from
//  241 	 * buffer's data to flash memory until the size of the data remaining to be
//  242 	 * copied requires special treatment. */
//  243 	while (size >= sizeof(flashdata_t)) {
??flashWrite_2:
        CMP      R6,#+4
        BCC.N    ??flashWrite_4
//  244 //		print("flash write size=%d\r\n", size);
//  245 		flashWriteData(address, *(const flashdata_t*) buffer);
        LDR      R1,[R5, #+0]
        MOVS     R0,R4
          CFI FunCall flashWriteData
        BL       flashWriteData
//  246 		address += sizeof(flashdata_t);
        ADDS     R4,R4,#+4
//  247 		buffer += sizeof(flashdata_t);
        ADDS     R5,R5,#+4
//  248 		size -= sizeof(flashdata_t);
        SUBS     R6,R6,#+4
        B.N      ??flashWrite_2
//  249 	}
//  250 
//  251 	/* Now, address is correctly aligned, but the remaining data are to
//  252 	 * small to fill a entier flashdata_t. Thus, one must read data already
//  253 	 * in flash and update them with buffer's data before writing an entire
//  254 	 * flashdata_t to flash memory. */
//  255 	if (size > 0) {
??flashWrite_4:
        CMP      R6,#+0
        BEQ.N    ??flashWrite_5
//  256 		flashdata_t tmp = *(volatile flashdata_t*) address;
        LDR      R0,[R4, #+0]
        STR      R0,[SP, #+0]
//  257 		memcpy(&tmp, buffer, size);
        MOVS     R2,R6
        MOVS     R1,R5
        ADD      R0,SP,#+0
          CFI FunCall memcpy
        BL       memcpy
//  258 		flashWriteData(address, tmp);
        LDR      R1,[SP, #+0]
        MOVS     R0,R4
          CFI FunCall flashWriteData
        BL       flashWriteData
//  259 	}
//  260 
//  261 	/* Lock flash again */
//  262 	flashLock()
??flashWrite_5:
        LDR.N    R0,??DataTable3  ;; 0x40023c10
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x80000000
        LDR.N    R1,??DataTable3  ;; 0x40023c10
        STR      R0,[R1, #+0]
//  263 	;
//  264 
//  265 	return FLASH_RETURN_SUCCESS;
        MOVS     R0,#+0
??flashWrite_1:
        POP      {R1-R9,PC}       ;; return
          CFI EndBlock cfiBlock12
//  266 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3:
        DC32     0x40023c10

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_1:
        DC32     0x40023c04

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_2:
        DC32     0x45670123

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_3:
        DC32     0xcdef89ab

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_4:
        DC32     0x40023c0c

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
//  267 
//  268 #endif /* EFI_INTERNAL_FLASH */
// 
// 844 bytes in section .text
// 
// 822 bytes of CODE memory (+ 22 bytes shared)
//
//Errors: none
//Warnings: 1
