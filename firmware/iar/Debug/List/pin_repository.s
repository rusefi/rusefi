///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:57 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\pin_re /
//                       pository.cpp                                         /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\pin_re /
//                       pository.cpp -lCN F:\stuff\rusefi_sourceforge\firmwa /
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
//                       pin_repository.s                                     /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME pin_repository

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z8getHwPin11brain_pin_e
        EXTERN _Z9getHwPort11brain_pin_e
        EXTERN _ZN18LoggingWithStorageC1EPKc
        EXTERN _pal_lld_setgroupmode
        EXTERN addConsoleAction
        EXTERN appendPrintf
        EXTERN atoi
        EXTERN chvprintf
        EXTERN firmwareError
        EXTERN icuStart
        EXTERN msObjectInit
        EXTERN print
        EXTERN printLine
        EXTERN scheduleMsg
        EXTERN strEqual

        PUBLIC PIN_USED
        PUBLIC _Z10hwPortname11brain_pin_e
        PUBLIC _Z11efiIcuStartP9ICUDriverPK9ICUConfig
        PUBLIC _Z12getInputMode16pin_input_mode_e
        PUBLIC _Z12mySetPadModePKcP12GPIO_TypeDefjj
        PUBLIC _Z13mySetPadMode2PKc11brain_pin_ej
        PUBLIC _Z13parseBrainPinPKc
        PUBLIC _Z14getPinFunction11brain_pin_e
        PUBLIC _Z17initPinRepositoryv
        PUBLIC _Z25registedFundamentralIoPinPcP12GPIO_TypeDefjj
        PUBLIC _Z8portnameP12GPIO_TypeDef
        PUBLIC _Z9unmarkPin11brain_pin_e
        
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
        

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
PIN_USED:
        DS8 448

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
initialized:
        DS8 4

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        LDR.W    R1,??DataTable12_1
        LDR.W    R0,??DataTable12_2
          CFI FunCall _ZN18LoggingWithStorageC1EPKc
        BL       _ZN18LoggingWithStorageC1EPKc
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0
// F:\stuff\rusefi_sourceforge\firmware\hw_layer\pin_repository.cpp
//    1 /**
//    2  * @file    pin_repository.cpp
//    3  * @brief   I/O pin registry code
//    4  *
//    5  * This job of this class is to make sure that we are not using same hardware pin for two
//    6  * different purposes.
//    7  *
//    8  * @date Jan 15, 2013
//    9  * @author Andrey Belomutskiy, (c) 2012-2015
//   10  */
//   11 
//   12 #include "main.h"

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z8chprintfP20BaseSequentialStreamPKcz
        THUMB
// static __interwork __softfp void chprintf(BaseSequentialStream *, char const *, ...)
_Z8chprintfP20BaseSequentialStreamPKcz:
        PUSH     {R2,R3}
          CFI CFA R13+8
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        ADD      R0,SP,#+16
        MOVS     R6,R0
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall chvprintf
        BL       chvprintf
        POP      {R4-R6}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R6 SameValue
          CFI CFA R13+12
        LDR      PC,[SP], #+12    ;; return
          CFI EndBlock cfiBlock1
//   13 #include "pin_repository.h"
//   14 #include "eficonsole.h"
//   15 #include "memstreams.h"
//   16 #include "chprintf.h"
//   17 #include "rusefi.h"
//   18 
//   19 #define PIN_REPO_SIZE 7 * PORT_SIZE
//   20 const char *PIN_USED[PIN_REPO_SIZE];
//   21 static int initialized = FALSE;
//   22 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   23 static LoggingWithStorage logger("pin repos");
logger:
        DS8 260

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   24 static int totalPinsUsed = 0;
totalPinsUsed:
        DS8 4
//   25 

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   26 static GPIO_TypeDef* ports[7] = {GPIOA,
ports:
        DATA
        DC32 40020000H, 40020400H, 40020800H, 40020C00H, 40021000H, 40021400H
        DC32 40021C00H
//   27 		GPIOB,
//   28 		GPIOC,
//   29 		GPIOD,
//   30 		GPIOE,
//   31 		GPIOF,
//   32 		GPIOH,
//   33 };
//   34 
//   35 /**
//   36  * @deprecated - use hwPortname() instead
//   37  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z8portnameP12GPIO_TypeDef
          CFI NoCalls
        THUMB
//   38 const char *portname(GPIO_TypeDef* GPIOx) {
_Z8portnameP12GPIO_TypeDef:
        MOVS     R1,R0
//   39 	if (GPIOx == GPIOA)
        LDR.W    R0,??DataTable12_3  ;; 0x40020000
        CMP      R1,R0
        BNE.N    ??portname_0
//   40 		return "PA";
        ADR.N    R0,??DataTable7  ;; 0x50, 0x41, 0x00, 0x00
        B.N      ??portname_1
//   41 	if (GPIOx == GPIOB)
??portname_0:
        LDR.W    R0,??DataTable12_4  ;; 0x40020400
        CMP      R1,R0
        BNE.N    ??portname_2
//   42 		return "PB";
        ADR.N    R0,??DataTable7_1  ;; 0x50, 0x42, 0x00, 0x00
        B.N      ??portname_1
//   43 	if (GPIOx == GPIOC)
??portname_2:
        LDR.W    R0,??DataTable12_5  ;; 0x40020800
        CMP      R1,R0
        BNE.N    ??portname_3
//   44 		return "PC";
        ADR.N    R0,??DataTable7_2  ;; 0x50, 0x43, 0x00, 0x00
        B.N      ??portname_1
//   45 	if (GPIOx == GPIOD)
??portname_3:
        LDR.W    R0,??DataTable12_6  ;; 0x40020c00
        CMP      R1,R0
        BNE.N    ??portname_4
//   46 		return "PD";
        ADR.N    R0,??DataTable7_3  ;; 0x50, 0x44, 0x00, 0x00
        B.N      ??portname_1
//   47 #if defined(STM32F4XX)
//   48 	if (GPIOx == GPIOE)
??portname_4:
        LDR.W    R0,??DataTable12_7  ;; 0x40021000
        CMP      R1,R0
        BNE.N    ??portname_5
//   49 		return "PE";
        ADR.N    R0,??DataTable7_4  ;; 0x50, 0x45, 0x00, 0x00
        B.N      ??portname_1
//   50 	if (GPIOx == GPIOH)
??portname_5:
        LDR.W    R0,??DataTable12_8  ;; 0x40021c00
        CMP      R1,R0
        BNE.N    ??portname_6
//   51 		return "PH";
        ADR.N    R0,??DataTable8  ;; 0x50, 0x48, 0x00, 0x00
        B.N      ??portname_1
//   52 #endif
//   53 	if (GPIOx == GPIOF)
??portname_6:
        LDR.W    R0,??DataTable12_9  ;; 0x40021400
        CMP      R1,R0
        BNE.N    ??portname_7
//   54 		return "PF";
        ADR.N    R0,??DataTable8_1  ;; 0x50, 0x46, 0x00, 0x00
        B.N      ??portname_1
//   55 	return "unknown";
??portname_7:
        LDR.W    R0,??DataTable12_10
??portname_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock2
//   56 }
//   57 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z12getPortIndexP12GPIO_TypeDef
        THUMB
//   58 static int getPortIndex(GPIO_TypeDef* port) {
_Z12getPortIndexP12GPIO_TypeDef:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   59 	if (port == GPIOA)
        LDR.W    R0,??DataTable12_3  ;; 0x40020000
        CMP      R4,R0
        BNE.N    ??getPortIndex_0
//   60 		return 0;
        MOVS     R0,#+0
        B.N      ??getPortIndex_1
//   61 	if (port == GPIOB)
??getPortIndex_0:
        LDR.W    R0,??DataTable12_4  ;; 0x40020400
        CMP      R4,R0
        BNE.N    ??getPortIndex_2
//   62 		return 1;
        MOVS     R0,#+1
        B.N      ??getPortIndex_1
//   63 	if (port == GPIOC)
??getPortIndex_2:
        LDR.W    R0,??DataTable12_5  ;; 0x40020800
        CMP      R4,R0
        BNE.N    ??getPortIndex_3
//   64 		return 2;
        MOVS     R0,#+2
        B.N      ??getPortIndex_1
//   65 	if (port == GPIOD)
??getPortIndex_3:
        LDR.W    R0,??DataTable12_6  ;; 0x40020c00
        CMP      R4,R0
        BNE.N    ??getPortIndex_4
//   66 		return 3;
        MOVS     R0,#+3
        B.N      ??getPortIndex_1
//   67 #if defined(STM32F4XX)
//   68 	if (port == GPIOE)
??getPortIndex_4:
        LDR.W    R0,??DataTable12_7  ;; 0x40021000
        CMP      R4,R0
        BNE.N    ??getPortIndex_5
//   69 		return 4;
        MOVS     R0,#+4
        B.N      ??getPortIndex_1
//   70 #endif /* defined(STM32F4XX) */
//   71 	if (port == GPIOF)
??getPortIndex_5:
        LDR.W    R0,??DataTable12_9  ;; 0x40021400
        CMP      R4,R0
        BNE.N    ??getPortIndex_6
//   72 		return 5;
        MOVS     R0,#+5
        B.N      ??getPortIndex_1
//   73 #if defined(STM32F4XX)
//   74 	if (port == GPIOH)
??getPortIndex_6:
        LDR.W    R0,??DataTable12_8  ;; 0x40021c00
        CMP      R4,R0
        BNE.N    ??getPortIndex_7
//   75 		return 6;
        MOVS     R0,#+6
        B.N      ??getPortIndex_1
//   76 #endif /* defined(STM32F4XX) */
//   77 	firmwareError("portindex");
??getPortIndex_7:
        LDR.W    R0,??DataTable12_11
          CFI FunCall firmwareError
        BL       firmwareError
//   78 	return -1;
        MOVS     R0,#-1
??getPortIndex_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//   79 }
//   80 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z10reportPinsv
        THUMB
//   81 static void reportPins(void) {
_Z10reportPinsv:
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
//   82 	for (int i = 0; i < PIN_REPO_SIZE; i++) {
        MOVS     R4,#+0
??reportPins_0:
        CMP      R4,#+112
        BGE.N    ??reportPins_1
//   83 		const char *name = PIN_USED[i];
        LDR.W    R0,??DataTable12_12
        LDR      R5,[R0, R4, LSL #+2]
//   84 		int portIndex = i / PORT_SIZE;
        MOVS     R0,#+16
        SDIV     R6,R4,R0
//   85 		int pin = i % PORT_SIZE;
        MOVS     R0,#+16
        SDIV     R7,R4,R0
        MLS      R7,R7,R0,R4
//   86 		GPIO_TypeDef* port = ports[portIndex];
        LDR.W    R0,??DataTable12_13
        LDR      R8,[R0, R6, LSL #+2]
//   87 		if (name != NULL) {
        CMP      R5,#+0
        BEQ.N    ??reportPins_2
//   88 			scheduleMsg(&logger, "pin %s%d: %s", portname(port), pin, name);
        MOV      R0,R8
          CFI FunCall _Z8portnameP12GPIO_TypeDef
        BL       _Z8portnameP12GPIO_TypeDef
        STR      R5,[SP, #+0]
        MOVS     R3,R7
        MOVS     R2,R0
        LDR.W    R1,??DataTable12_14
        LDR.N    R0,??DataTable12_2
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   89 		}
//   90 	}
??reportPins_2:
        ADDS     R4,R4,#+1
        B.N      ??reportPins_0
//   91 
//   92 	scheduleMsg(&logger, "Total pins count: %d", totalPinsUsed);
??reportPins_1:
        LDR.N    R0,??DataTable12_15
        LDR      R2,[R0, #+0]
        LDR.N    R1,??DataTable12_16
        LDR.N    R0,??DataTable12_2
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   93 }
        POP      {R0,R1,R4-R8,PC}  ;; return
          CFI EndBlock cfiBlock4
//   94 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   95 static MemoryStream portNameStream;
portNameStream:
        DS8 20

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   96 static char portNameBuffer[20];
portNameBuffer:
        DS8 20
//   97 
//   98 /**
//   99  * Parse string representation of physical pin into brain_pin_e ordinal.
//  100  *
//  101  * @return GPIO_UNASSIGNED for "none", GPIO_INVALID for invalid entry
//  102  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z13parseBrainPinPKc
        THUMB
//  103 brain_pin_e parseBrainPin(const char *str) {
_Z13parseBrainPinPKc:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
//  104 	if (strEqual(str, "none"))
        LDR.N    R1,??DataTable12_17
        MOVS     R0,R4
          CFI FunCall strEqual
        BL       strEqual
        CMP      R0,#+0
        BEQ.N    ??parseBrainPin_0
//  105 		return GPIO_UNASSIGNED;
        MOVS     R0,#+80
        B.N      ??parseBrainPin_1
//  106 	// todo: create method toLowerCase?
//  107 	if (str[0] != 'p' && str[0] != 'P') {
??parseBrainPin_0:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+112
        BEQ.N    ??parseBrainPin_2
        LDRB     R0,[R4, #+0]
        CMP      R0,#+80
        BEQ.N    ??parseBrainPin_2
//  108 		return GPIO_INVALID;
        MOVS     R0,#+81
        B.N      ??parseBrainPin_1
//  109 	}
//  110 	char port = str[1];
??parseBrainPin_2:
        LDRB     R5,[R4, #+1]
//  111 	brain_pin_e basePin;
//  112 	if (port >= 'a' && port <= 'z') {
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+97
        BLT.N    ??parseBrainPin_3
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+123
        BGE.N    ??parseBrainPin_3
//  113 		basePin = (brain_pin_e) ((int) GPIOA_0 + 16 * (port - 'a'));
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        SUBS     R0,R5,#+97
        LSLS     R0,R0,#+4
        MOVS     R6,R0
        B.N      ??parseBrainPin_4
//  114 	} else if (port >= 'A' && port <= 'Z') {
??parseBrainPin_3:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+65
        BLT.N    ??parseBrainPin_5
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+91
        BGE.N    ??parseBrainPin_5
//  115 		basePin = (brain_pin_e) ((int) GPIOA_0 + 16 * (port - 'A'));
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        SUBS     R0,R5,#+65
        LSLS     R0,R0,#+4
        MOVS     R6,R0
        B.N      ??parseBrainPin_4
//  116 	} else {
//  117 		return GPIO_INVALID;
??parseBrainPin_5:
        MOVS     R0,#+81
        B.N      ??parseBrainPin_1
//  118 	}
//  119 	const char *pinStr = str + 2;
??parseBrainPin_4:
        ADDS     R7,R4,#+2
//  120 	int pin = atoi(pinStr);
        MOVS     R0,R7
          CFI FunCall atoi
        BL       atoi
//  121 	return (brain_pin_e)(basePin + pin);
        ADDS     R0,R0,R6
??parseBrainPin_1:
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock5
//  122 }
//  123 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z10hwPortname11brain_pin_e
        THUMB
//  124 const char *hwPortname(brain_pin_e brainPin) {
_Z10hwPortname11brain_pin_e:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  125 	if (brainPin == GPIO_INVALID) {
        CMP      R4,#+81
        BNE.N    ??hwPortname_0
//  126 		return "INVALID";
        LDR.N    R0,??DataTable12_18
        B.N      ??hwPortname_1
//  127 	}
//  128 	GPIO_TypeDef *hwPort = getHwPort(brainPin);
??hwPortname_0:
        MOVS     R0,R4
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOVS     R5,R0
//  129 	if (hwPort == GPIO_NULL) {
        CMP      R5,#+0
        BNE.N    ??hwPortname_2
//  130 		return "NONE";
        LDR.N    R0,??DataTable12_19
        B.N      ??hwPortname_1
//  131 	}
//  132 	int hwPin = getHwPin(brainPin);
??hwPortname_2:
        MOVS     R0,R4
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        MOVS     R6,R0
//  133 	portNameStream.eos = 0; // reset
        LDR.N    R0,??DataTable12_20
        MOVS     R1,#+0
        STR      R1,[R0, #+12]
//  134 	chprintf((BaseSequentialStream *) &portNameStream, "%s%d", portname(hwPort), hwPin);
        MOVS     R0,R5
          CFI FunCall _Z8portnameP12GPIO_TypeDef
        BL       _Z8portnameP12GPIO_TypeDef
        MOVS     R3,R6
        MOVS     R2,R0
        LDR.N    R1,??DataTable12_21
        LDR.N    R0,??DataTable12_20
          CFI FunCall _Z8chprintfP20BaseSequentialStreamPKcz
        BL       _Z8chprintfP20BaseSequentialStreamPKcz
//  135 	portNameStream.buffer[portNameStream.eos] = 0; // need to terminate explicitly
        LDR.N    R0,??DataTable12_20
        LDR      R0,[R0, #+12]
        LDR.N    R1,??DataTable12_20
        LDR      R1,[R1, #+4]
        MOVS     R2,#+0
        STRB     R2,[R0, R1]
//  136 	return portNameBuffer;
        LDR.N    R0,??DataTable12_22
??hwPortname_1:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock6
//  137 }
//  138 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z17initPinRepositoryv
        THUMB
//  139 void initPinRepository(void) {
_Z17initPinRepositoryv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  140 	/**
//  141 	 * this method cannot use console because this method is invoked before console is initialized
//  142 	 */
//  143 
//  144 	msObjectInit(&portNameStream, (uint8_t*) portNameBuffer, sizeof(portNameBuffer), 0);
        MOVS     R3,#+0
        MOVS     R2,#+20
        LDR.N    R1,??DataTable12_22
        LDR.N    R0,??DataTable12_20
          CFI FunCall msObjectInit
        BL       msObjectInit
//  145 
//  146 	for (int i = 0; i < PIN_REPO_SIZE; i++)
        MOVS     R0,#+0
??initPinRepository_0:
        CMP      R0,#+112
        BGE.N    ??initPinRepository_1
//  147 		PIN_USED[i] = 0;
        LDR.N    R1,??DataTable12_12
        MOVS     R2,#+0
        STR      R2,[R1, R0, LSL #+2]
        ADDS     R0,R0,#+1
        B.N      ??initPinRepository_0
//  148 	initialized = true;
??initPinRepository_1:
        LDR.N    R0,??DataTable12_23
        MOVS     R1,#+1
        STR      R1,[R0, #+0]
//  149 	addConsoleAction("pins", reportPins);
        LDR.N    R1,??DataTable12_24
        LDR.N    R0,??DataTable12_25
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  150 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock7
//  151 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z8markUsediPKc
          CFI NoCalls
        THUMB
//  152 static inline void markUsed(int index, const char *msg) {
//  153 	PIN_USED[index] = msg;
_Z8markUsediPKc:
        LDR.N    R2,??DataTable12_12
        STR      R1,[R2, R0, LSL #+2]
//  154 	totalPinsUsed++;
        LDR.N    R2,??DataTable12_15
        LDR      R2,[R2, #+0]
        ADDS     R2,R2,#+1
        LDR.N    R3,??DataTable12_15
        STR      R2,[R3, #+0]
//  155 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7:
        DC8      0x50, 0x41, 0x00, 0x00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_1:
        DC8      0x50, 0x42, 0x00, 0x00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_2:
        DC8      0x50, 0x43, 0x00, 0x00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_3:
        DC8      0x50, 0x44, 0x00, 0x00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_4:
        DC8      0x50, 0x45, 0x00, 0x00
//  156 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z13mySetPadMode2PKc11brain_pin_ej
        THUMB
//  157 void mySetPadMode2(const char *msg, brain_pin_e pin, iomode_t mode) {
_Z13mySetPadMode2PKc11brain_pin_ej:
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
//  158 	mySetPadMode(msg, getHwPort(pin), getHwPin(pin), mode);
        MOVS     R0,R5
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        MOVS     R7,R0
        MOVS     R0,R5
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOVS     R3,R6
        MOVS     R2,R7
        MOVS     R1,R0
        MOVS     R0,R4
          CFI FunCall _Z12mySetPadModePKcP12GPIO_TypeDefjj
        BL       _Z12mySetPadModePKcP12GPIO_TypeDefjj
//  159 }
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock9
//  160 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z12getInputMode16pin_input_mode_e
          CFI NoCalls
        THUMB
//  161 iomode_t getInputMode(pin_input_mode_e mode) {
_Z12getInputMode16pin_input_mode_e:
        MOVS     R1,R0
//  162 	switch (mode) {
        MOVS     R0,R1
        CMP      R0,#+1
        BEQ.N    ??getInputMode_0
        CMP      R0,#+2
        BEQ.N    ??getInputMode_1
        B.N      ??getInputMode_2
//  163 	case PI_PULLUP:
//  164 		return PAL_MODE_INPUT_PULLUP;
??getInputMode_0:
        MOVS     R0,#+32
        B.N      ??getInputMode_3
//  165 	case PI_PULLDOWN:
//  166 		return PAL_MODE_INPUT_PULLDOWN;
??getInputMode_1:
        MOVS     R0,#+64
        B.N      ??getInputMode_3
//  167 	case PI_DEFAULT:
//  168 	default:
//  169 		return PAL_MODE_INPUT;
??getInputMode_2:
        MOVS     R0,#+0
??getInputMode_3:
        BX       LR               ;; return
          CFI EndBlock cfiBlock10
//  170 	}
//  171 }
//  172 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z8getIndexP12GPIO_TypeDefj
        THUMB
//  173 static int getIndex(ioportid_t port, ioportmask_t pin) {
_Z8getIndexP12GPIO_TypeDefj:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  174 	int portIndex = getPortIndex(port);
        MOVS     R0,R4
          CFI FunCall _Z12getPortIndexP12GPIO_TypeDef
        BL       _Z12getPortIndexP12GPIO_TypeDef
//  175 	return portIndex * PORT_SIZE + pin;
        ADDS     R0,R5,R0, LSL #+4
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock11
//  176 }
//  177 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _Z14getPinFunction11brain_pin_e
        THUMB
//  178 const char * getPinFunction(brain_input_pin_e brainPin) {
_Z14getPinFunction11brain_pin_e:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  179 	ioportid_t port = getHwPort(brainPin);
        MOVS     R0,R4
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOVS     R5,R0
//  180 	ioportmask_t pin = getHwPin(brainPin);
        MOVS     R0,R4
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        MOVS     R6,R0
//  181 
//  182 	int index = getIndex(port, pin);
        MOVS     R1,R6
        MOVS     R0,R5
          CFI FunCall _Z8getIndexP12GPIO_TypeDefj
        BL       _Z8getIndexP12GPIO_TypeDefj
//  183 	return PIN_USED[index];
        LDR.N    R1,??DataTable12_12
        LDR      R0,[R1, R0, LSL #+2]
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock12
//  184 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8:
        DC8      0x50, 0x48, 0x00, 0x00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_1:
        DC8      0x50, 0x46, 0x00, 0x00
//  185 
//  186 /**
//  187  * This method would set an error condition if pin is already used
//  188  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z12mySetPadModePKcP12GPIO_TypeDefjj
        THUMB
//  189 void mySetPadMode(const char *msg, ioportid_t port, ioportmask_t pin, iomode_t mode) {
_Z12mySetPadModePKcP12GPIO_TypeDefjj:
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
        MOVS     R6,R2
        MOVS     R7,R3
//  190 	if (!initialized) {
        LDR.N    R0,??DataTable12_23
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??mySetPadMode_0
//  191 		firmwareError("repository not initialized");
        LDR.N    R0,??DataTable12_26
          CFI FunCall firmwareError
        BL       firmwareError
//  192 		return;
        B.N      ??mySetPadMode_1
//  193 	}
//  194 	if (port == GPIO_NULL)
??mySetPadMode_0:
        CMP      R5,#+0
        BEQ.N    ??mySetPadMode_1
//  195 		return;
//  196 	print("%s on %s:%d\r\n", msg, portname(port), pin);
??mySetPadMode_2:
        MOVS     R0,R5
          CFI FunCall _Z8portnameP12GPIO_TypeDef
        BL       _Z8portnameP12GPIO_TypeDef
        MOVS     R3,R6
        MOVS     R2,R0
        MOVS     R1,R4
        LDR.N    R0,??DataTable12_27
          CFI FunCall print
        BL       print
//  197 
//  198 	appendPrintf(&logger, "msg,%s", msg);
        MOVS     R2,R4
        LDR.N    R1,??DataTable12_28
        LDR.N    R0,??DataTable12_2
          CFI FunCall appendPrintf
        BL       appendPrintf
//  199 	appendPrintf(&logger, " on %s%d%s", portname(port), pin, DELIMETER);
        MOVS     R0,R5
          CFI FunCall _Z8portnameP12GPIO_TypeDef
        BL       _Z8portnameP12GPIO_TypeDef
        MOVS     R2,R0
        ADR.N    R0,??DataTable12  ;; ","
        STR      R0,[SP, #+0]
        MOVS     R3,R6
        LDR.N    R1,??DataTable12_29
        LDR.N    R0,??DataTable12_2
          CFI FunCall appendPrintf
        BL       appendPrintf
//  200 	printLine(&logger);
        LDR.N    R0,??DataTable12_2
          CFI FunCall printLine
        BL       printLine
//  201 
//  202 	int index = getIndex(port, pin);
        MOVS     R1,R6
        MOVS     R0,R5
          CFI FunCall _Z8getIndexP12GPIO_TypeDefj
        BL       _Z8getIndexP12GPIO_TypeDefj
        MOV      R8,R0
//  203 
//  204 	if (PIN_USED[index] != NULL) {
        LDR.N    R0,??DataTable12_12
        LDR      R0,[R0, R8, LSL #+2]
        CMP      R0,#+0
        BEQ.N    ??mySetPadMode_3
//  205 		/**
//  206 		 * todo: the problem is that this warning happens before the console is even
//  207 		 * connected, so the warning is never displayed on the console and that's quite a problem!
//  208 		 */
//  209 //		warning(OBD_PCM_Processor_Fault, "%s%d req by %s used by %s", portname(port), pin, msg, PIN_USED[index]);
//  210 		firmwareError("%s%d req by %s used by %s", portname(port), pin, msg, PIN_USED[index]);
        MOVS     R0,R5
          CFI FunCall _Z8portnameP12GPIO_TypeDef
        BL       _Z8portnameP12GPIO_TypeDef
        MOVS     R1,R0
        LDR.N    R0,??DataTable12_12
        LDR      R0,[R0, R8, LSL #+2]
        STR      R0,[SP, #+0]
        MOVS     R3,R4
        MOVS     R2,R6
        LDR.N    R0,??DataTable12_30
          CFI FunCall firmwareError
        BL       firmwareError
//  211 		return;
        B.N      ??mySetPadMode_1
//  212 	}
//  213 	markUsed(index, msg);
??mySetPadMode_3:
        MOVS     R1,R4
        MOV      R0,R8
          CFI FunCall _Z8markUsediPKc
        BL       _Z8markUsediPKc
//  214 
//  215 	palSetPadMode(port, pin, mode);
        MOVS     R2,R7
        MOVS     R0,#+1
        LSLS     R1,R0,R6
        MOVS     R0,R5
          CFI FunCall _pal_lld_setgroupmode
        BL       _pal_lld_setgroupmode
//  216 }
??mySetPadMode_1:
        POP      {R0,R1,R4-R8,PC}  ;; return
          CFI EndBlock cfiBlock13
//  217 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _Z9unmarkPin11brain_pin_e
        THUMB
//  218 void unmarkPin(brain_pin_e brainPin) {
_Z9unmarkPin11brain_pin_e:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  219 	ioportid_t port = getHwPort(brainPin);
        MOVS     R0,R4
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOVS     R5,R0
//  220 	ioportmask_t pin = getHwPin(brainPin);
        MOVS     R0,R4
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        MOVS     R6,R0
//  221 
//  222 	int index = getIndex(port, pin);
        MOVS     R1,R6
        MOVS     R0,R5
          CFI FunCall _Z8getIndexP12GPIO_TypeDefj
        BL       _Z8getIndexP12GPIO_TypeDefj
//  223 
//  224 	if (PIN_USED[index] != NULL) {
        LDR.N    R1,??DataTable12_12
        LDR      R1,[R1, R0, LSL #+2]
        CMP      R1,#+0
        BEQ.N    ??unmarkPin_0
//  225 		PIN_USED[index] = NULL;
        LDR.N    R1,??DataTable12_12
        MOVS     R2,#+0
        STR      R2,[R1, R0, LSL #+2]
//  226 		totalPinsUsed--;
        LDR.N    R1,??DataTable12_15
        LDR      R1,[R1, #+0]
        SUBS     R1,R1,#+1
        LDR.N    R2,??DataTable12_15
        STR      R1,[R2, #+0]
//  227 	}
//  228 }
??unmarkPin_0:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock14
//  229 
//  230 /**
//  231  * This method would crash the program if pin is already in use
//  232  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function _Z25registedFundamentralIoPinPcP12GPIO_TypeDefjj
        THUMB
//  233 void registedFundamentralIoPin(char *msg, ioportid_t port, ioportmask_t pin, iomode_t mode) {
_Z25registedFundamentralIoPinPcP12GPIO_TypeDefjj:
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
//  234 	efiAssertVoid(initialized, "repo not initialized");
        LDR.N    R0,??DataTable12_23
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??registedFundamentralIoPin_0
        LDR.N    R0,??DataTable12_31
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??registedFundamentralIoPin_1
//  235 
//  236 	int index = getIndex(port, pin);
??registedFundamentralIoPin_0:
        MOVS     R1,R6
        MOVS     R0,R5
          CFI FunCall _Z8getIndexP12GPIO_TypeDefj
        BL       _Z8getIndexP12GPIO_TypeDefj
        MOV      R8,R0
//  237 
//  238 	if (PIN_USED[index] != NULL) {
        LDR.N    R0,??DataTable12_12
        LDR      R0,[R0, R8, LSL #+2]
        CMP      R0,#+0
        BEQ.N    ??registedFundamentralIoPin_2
//  239 		print("!!!!!!!!!!!!! Already used [%s] %d\r\n", msg, pin);
        MOVS     R2,R6
        MOVS     R1,R4
        LDR.N    R0,??DataTable12_32
          CFI FunCall print
        BL       print
//  240 		print("!!!!!!!!!!!!! Already used by [%s]\r\n", PIN_USED[index]);
        LDR.N    R0,??DataTable12_12
        LDR      R1,[R0, R8, LSL #+2]
        LDR.N    R0,??DataTable12_33
          CFI FunCall print
        BL       print
//  241 		firmwareError("pin already used");
        LDR.N    R0,??DataTable12_34
          CFI FunCall firmwareError
        BL       firmwareError
//  242 		return;
        B.N      ??registedFundamentralIoPin_1
//  243 	}
//  244 	markUsed(index, msg);
??registedFundamentralIoPin_2:
        MOVS     R1,R4
        MOV      R0,R8
          CFI FunCall _Z8markUsediPKc
        BL       _Z8markUsediPKc
//  245 	palSetPadMode(port, pin, mode);
        MOVS     R2,R7
        MOVS     R0,#+1
        LSLS     R1,R0,R6
        MOVS     R0,R5
          CFI FunCall _pal_lld_setgroupmode
        BL       _pal_lld_setgroupmode
//  246 }
??registedFundamentralIoPin_1:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock15
//  247 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function _Z11efiIcuStartP9ICUDriverPK9ICUConfig
        THUMB
//  248 void efiIcuStart(ICUDriver *icup, const ICUConfig *config) {
_Z11efiIcuStartP9ICUDriverPK9ICUConfig:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  249 	  efiAssertVoid((icup->state == ICU_STOP) || (icup->state == ICU_READY),
//  250 	              "input already used?");
        LDRB     R0,[R4, #+0]
        CMP      R0,#+1
        BEQ.N    ??efiIcuStart_0
        LDRB     R0,[R4, #+0]
        CMP      R0,#+2
        BEQ.N    ??efiIcuStart_0
        LDR.N    R0,??DataTable12_35
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??efiIcuStart_1
//  251 
//  252 	icuStart(icup, config);
??efiIcuStart_0:
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall icuStart
        BL       icuStart
//  253 }
??efiIcuStart_1:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock16

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12:
        DC8      ",",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_1:
        DC32     `?<Constant "pin repos">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_2:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_3:
        DC32     0x40020000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_4:
        DC32     0x40020400

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_5:
        DC32     0x40020800

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_6:
        DC32     0x40020c00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_7:
        DC32     0x40021000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_8:
        DC32     0x40021c00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_9:
        DC32     0x40021400

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_10:
        DC32     `?<Constant "unknown">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_11:
        DC32     `?<Constant "portindex">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_12:
        DC32     PIN_USED

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_13:
        DC32     ports

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_14:
        DC32     `?<Constant "pin %s%d: %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_15:
        DC32     totalPinsUsed

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_16:
        DC32     `?<Constant "Total pins count: %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_17:
        DC32     `?<Constant "none">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_18:
        DC32     `?<Constant "INVALID">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_19:
        DC32     `?<Constant "NONE">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_20:
        DC32     portNameStream

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_21:
        DC32     `?<Constant "%s%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_22:
        DC32     portNameBuffer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_23:
        DC32     initialized

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_24:
        DC32     _Z10reportPinsv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_25:
        DC32     `?<Constant "pins">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_26:
        DC32     `?<Constant "repository not initia...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_27:
        DC32     `?<Constant "%s on %s:%d\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_28:
        DC32     `?<Constant "msg,%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_29:
        DC32     `?<Constant " on %s%d%s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_30:
        DC32     `?<Constant "%s%d req by %s used b...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_31:
        DC32     `?<Constant "repo not initialized">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_32:
        DC32     `?<Constant "!!!!!!!!!!!!! Already...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_33:
        DC32     `?<Constant "!!!!!!!!!!!!! Already...">_1`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_34:
        DC32     `?<Constant "pin already used">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable12_35:
        DC32     `?<Constant "input already used?">`

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
        DATA
        DC8 "PA"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "PB"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "PC"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "PD"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "PE"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "PH"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "PF"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "unknown">`:
        DATA
        DC8 "unknown"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "portindex">`:
        DATA
        DC8 "portindex"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "pin %s%d: %s">`:
        DATA
        DC8 "pin %s%d: %s"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Total pins count: %d">`:
        DATA
        DC8 "Total pins count: %d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "none">`:
        DATA
        DC8 "none"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "INVALID">`:
        DATA
        DC8 "INVALID"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "NONE">`:
        DATA
        DC8 "NONE"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "%s%d">`:
        DATA
        DC8 "%s%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "pins">`:
        DATA
        DC8 "pins"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "repository not initia...">`:
        DATA
        DC8 "repository not initialized"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "%s on %s:%d\\r\\n">`:
        DATA
        DC8 "%s on %s:%d\015\012"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "msg,%s">`:
        DATA
        DC8 "msg,%s"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant " on %s%d%s">`:
        DATA
        DC8 " on %s%d%s"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 ","

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "%s%d req by %s used b...">`:
        DATA
        DC8 "%s%d req by %s used by %s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "repo not initialized">`:
        DATA
        DC8 "repo not initialized"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "!!!!!!!!!!!!! Already...">`:
        DATA
        DC8 "!!!!!!!!!!!!! Already used [%s] %d\015\012"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "!!!!!!!!!!!!! Already...">_1`:
        DATA
        DC8 "!!!!!!!!!!!!! Already used by [%s]\015\012"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "pin already used">`:
        DATA
        DC8 "pin already used"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "input already used?">`:
        DATA
        DC8 "input already used?"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "pin repos">`:
        DATA
        DC8 "pin repos"
        DC8 0, 0

        END
// 
//   756 bytes in section .bss
//    28 bytes in section .data
//     4 bytes in section .init_array
//   378 bytes in section .rodata
// 1 222 bytes in section .text
// 
// 1 226 bytes of CODE  memory
//   378 bytes of CONST memory
//   784 bytes of DATA  memory
//
//Errors: none
//Warnings: none
