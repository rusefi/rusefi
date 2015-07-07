///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:19 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\max318 /
//                       55.cpp                                               /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\max318 /
//                       55.cpp -lCN F:\stuff\rusefi_sourceforge\firmware\iar /
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
//                       max31855.s                                           /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME max31855

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z10hwPortname11brain_pin_e
        EXTERN _Z13printSpiStateP7LoggingP21board_configuration_s
        EXTERN __aeabi_d2f
        EXTERN __aeabi_ddiv
        EXTERN __aeabi_f2d
        EXTERN __aeabi_i2d
        EXTERN addConsoleAction
        EXTERN boardConfiguration
        EXTERN initSpiCs
        EXTERN scheduleMsg
        EXTERN spiReceive
        EXTERN spiSelect
        EXTERN spiStart
        EXTERN spiStop
        EXTERN spiUnselect

        PUBLIC _Z11getEgtValuei
        PUBLIC _Z12initMax31855P7LoggingP9SPIDriverP11brain_pin_e
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\hw_layer\max31855.cpp
//    1 /**
//    2  * @file max31855.cpp
//    3  * @brief MAX31855 Thermocouple-to-Digital Converter driver
//    4  *
//    5  *
//    6  * http://datasheets.maximintegrated.com/en/ds/MAX31855.pdf
//    7  *
//    8  *
//    9  * Read-only communication over 5MHz SPI
//   10  *
//   11  * @date Sep 17, 2014
//   12  * @author Andrey Belomutskiy, (c) 2012-2015
//   13  */
//   14 
//   15 #include "main.h"
//   16 #include "max31855.h"
//   17 
//   18 #include "hardware.h"
//   19 #include "mpu_util.h"
//   20 
//   21 #if EFI_PROD_CODE
//   22 #include "settings.h"
//   23 #include "pin_repository.h"
//   24 #endif /* EFI_PROD_CODE */
//   25 
//   26 #if EFI_MAX_31855
//   27 
//   28 #define EGT_ERROR_VALUE -1000
//   29 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   30 static SPIDriver *driver;
driver:
        DS8 4
//   31 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   32 static Logging* logger;
logger:
        DS8 4
//   33 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   34 static SPIConfig spiConfig[EGT_CHANNEL_COUNT];
spiConfig:
        DS8 96
//   35 
//   36 EXTERN_ENGINE;
//   37 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z11showEgtInfov
        THUMB
//   38 static void showEgtInfo(void) {
_Z11showEgtInfov:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//   39 #if EFI_PROD_CODE
//   40 	printSpiState(logger, boardConfiguration);
        LDR.N    R0,??DataTable5_1
        LDR      R1,[R0, #+0]
        LDR.N    R0,??DataTable5_2
        LDR      R0,[R0, #+0]
          CFI FunCall _Z13printSpiStateP7LoggingP21board_configuration_s
        BL       _Z13printSpiStateP7LoggingP21board_configuration_s
//   41 
//   42 	scheduleMsg(logger, "EGT spi: %d", boardConfiguration->max31855spiDevice);
        LDR.N    R0,??DataTable5_1
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+448]
        LDR.N    R1,??DataTable5_3
        LDR.N    R0,??DataTable5_2
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   43 
//   44 	for (int i = 0; i < EGT_CHANNEL_COUNT; i++) {
        MOVS     R4,#+0
??showEgtInfo_0:
        CMP      R4,#+8
        BGE.N    ??showEgtInfo_1
//   45 		if (boardConfiguration->max31855_cs[i] != GPIO_UNASSIGNED) {
        LDR.N    R0,??DataTable5_1
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R4, LSL #+2
        LDR      R0,[R0, #+416]
        CMP      R0,#+80
        BEQ.N    ??showEgtInfo_2
//   46 			scheduleMsg(logger, "%d ETG @ %s", i, hwPortname(boardConfiguration->max31855_cs[i]));
        LDR.N    R0,??DataTable5_1
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,R4, LSL #+2
        LDR      R0,[R0, #+416]
          CFI FunCall _Z10hwPortname11brain_pin_e
        BL       _Z10hwPortname11brain_pin_e
        MOVS     R3,R0
        MOVS     R2,R4
        LDR.N    R1,??DataTable5_4
        LDR.N    R0,??DataTable5_2
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   47 		}
//   48 	}
??showEgtInfo_2:
        ADDS     R4,R4,#+1
        B.N      ??showEgtInfo_0
//   49 #endif
//   50 }
??showEgtInfo_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0
//   51 
//   52 // bits D17 and D3 are always expected to be zero
//   53 #define MC_RESERVED_BITS 0x20008
//   54 #define MC_OPEN_BIT 1
//   55 #define MC_GND_BIT 2
//   56 #define MC_VCC_BIT 4
//   57 
//   58 typedef enum {
//   59 	MC_OK = 0, MC_INVALID = 1, MC_OPEN = 2, MC_SHORT_GND = 3, MC_SHORT_VCC = 4,
//   60 } max_32855_code;
//   61 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z9getMcCode14max_32855_code
          CFI NoCalls
        THUMB
//   62 static const char * getMcCode(max_32855_code code) {
_Z9getMcCode14max_32855_code:
        MOVS     R1,R0
//   63 	switch (code) {
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R1
        CMP      R0,#+0
        BEQ.N    ??getMcCode_0
        CMP      R0,#+2
        BEQ.N    ??getMcCode_1
        CMP      R0,#+3
        BEQ.N    ??getMcCode_2
        CMP      R0,#+4
        BEQ.N    ??getMcCode_3
        B.N      ??getMcCode_4
//   64 	case MC_OK:
//   65 		return "Ok";
??getMcCode_0:
        ADR.N    R0,??DataTable5  ;; 0x4F, 0x6B, 0x00, 0x00
        B.N      ??getMcCode_5
//   66 	case MC_OPEN:
//   67 		return "Open";
??getMcCode_1:
        LDR.N    R0,??DataTable5_5
        B.N      ??getMcCode_5
//   68 	case MC_SHORT_GND:
//   69 		return "short gnd";
??getMcCode_2:
        LDR.N    R0,??DataTable5_6
        B.N      ??getMcCode_5
//   70 	case MC_SHORT_VCC:
//   71 		return "short VCC";
??getMcCode_3:
        LDR.N    R0,??DataTable5_7
        B.N      ??getMcCode_5
//   72 	default:
//   73 		return "invalid";
??getMcCode_4:
        LDR.N    R0,??DataTable5_8
??getMcCode_5:
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   74 	}
//   75 }
//   76 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z13getResultCodej
          CFI NoCalls
        THUMB
//   77 static max_32855_code getResultCode(uint32_t egtPacket) {
_Z13getResultCodej:
        MOVS     R1,R0
//   78 	if ((egtPacket & MC_RESERVED_BITS) != 0) {
        LDR.N    R0,??DataTable5_9  ;; 0x20008
        TST      R1,R0
        BEQ.N    ??getResultCode_0
//   79 		return MC_INVALID;
        MOVS     R0,#+1
        B.N      ??getResultCode_1
//   80 	} else if ((egtPacket & MC_OPEN_BIT) != 0) {
??getResultCode_0:
        LSLS     R0,R1,#+31
        BPL.N    ??getResultCode_2
//   81 		return MC_OPEN;
        MOVS     R0,#+2
        B.N      ??getResultCode_1
//   82 	} else if ((egtPacket & MC_GND_BIT) != 0) {
??getResultCode_2:
        LSLS     R0,R1,#+30
        BPL.N    ??getResultCode_3
//   83 		return MC_SHORT_GND;
        MOVS     R0,#+3
        B.N      ??getResultCode_1
//   84 	} else if ((egtPacket & MC_VCC_BIT) != 0) {
??getResultCode_3:
        LSLS     R0,R1,#+29
        BPL.N    ??getResultCode_4
//   85 		return MC_SHORT_VCC;
        MOVS     R0,#+4
        B.N      ??getResultCode_1
//   86 	} else {
//   87 		return MC_OK;
??getResultCode_4:
        MOVS     R0,#+0
??getResultCode_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock2
//   88 	}
//   89 }
//   90 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z13readEgtPacketi
        THUMB
//   91 static uint32_t readEgtPacket(int egtChannel) {
_Z13readEgtPacketi:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        SUB      SP,SP,#+8
          CFI CFA R13+16
        MOVS     R4,R0
//   92 	uint32_t egtPacket;
//   93 	if (driver == NULL) {
        LDR.N    R0,??DataTable5_10
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??readEgtPacket_0
//   94 		return 0xFFFFFFFF;
        MOVS     R0,#-1
        B.N      ??readEgtPacket_1
//   95 	}
//   96 
//   97 	spiStart(driver, &spiConfig[egtChannel]);
??readEgtPacket_0:
        MOVS     R0,#+12
        LDR.N    R1,??DataTable5_11
        MLA      R1,R0,R4,R1
        LDR.N    R0,??DataTable5_10
        LDR      R0,[R0, #+0]
          CFI FunCall spiStart
        BL       spiStart
//   98 	spiSelect(driver);
        LDR.N    R0,??DataTable5_10
        LDR      R0,[R0, #+0]
          CFI FunCall spiSelect
        BL       spiSelect
//   99 
//  100 	spiReceive(driver, sizeof(egtPacket), &egtPacket);
        ADD      R2,SP,#+0
        MOVS     R1,#+4
        LDR.N    R0,??DataTable5_10
        LDR      R0,[R0, #+0]
          CFI FunCall spiReceive
        BL       spiReceive
//  101 
//  102 	spiUnselect(driver);
        LDR.N    R0,??DataTable5_10
        LDR      R0,[R0, #+0]
          CFI FunCall spiUnselect
        BL       spiUnselect
//  103 	spiStop(driver);
        LDR.N    R0,??DataTable5_10
        LDR      R0,[R0, #+0]
          CFI FunCall spiStop
        BL       spiStop
//  104 	egtPacket = SWAP_UINT32(egtPacket);
        LDR      R0,[SP, #+0]
        LDR      R1,[SP, #+0]
        LSLS     R1,R1,#+8
        ANDS     R1,R1,#0xFF0000
        ORRS     R0,R1,R0, LSR #+24
        LDR      R1,[SP, #+0]
        LSRS     R1,R1,#+8
        ANDS     R1,R1,#0xFF00
        ORRS     R0,R1,R0
        LDR      R1,[SP, #+0]
        ORRS     R0,R0,R1, LSL #+24
        STR      R0,[SP, #+0]
//  105 	return egtPacket;
        LDR      R0,[SP, #+0]
??readEgtPacket_1:
        POP      {R1,R2,R4,PC}    ;; return
          CFI EndBlock cfiBlock3
//  106 }
//  107 
//  108 #define GET_TEMPERATURE_C(x) (((x) >> 18) / 4)
//  109 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z11getEgtValuei
        THUMB
//  110 uint16_t getEgtValue(int egtChannel) {
_Z11getEgtValuei:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  111 	uint32_t packet = readEgtPacket(egtChannel);
        MOVS     R0,R4
          CFI FunCall _Z13readEgtPacketi
        BL       _Z13readEgtPacketi
        MOVS     R5,R0
//  112 	max_32855_code code = getResultCode(packet);
        MOVS     R0,R5
          CFI FunCall _Z13getResultCodej
        BL       _Z13getResultCodej
        MOVS     R1,R0
//  113 	if (code != MC_OK) {
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+0
        BEQ.N    ??getEgtValue_0
//  114 		return EGT_ERROR_VALUE + code;
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        SUBS     R0,R1,#+1000
        UXTH     R0,R0            ;; ZeroExt  R0,R0,#+16,#+16
        B.N      ??getEgtValue_1
//  115 	} else {
//  116 		return GET_TEMPERATURE_C(packet);
??getEgtValue_0:
        LSRS     R0,R5,#+20
        UXTH     R0,R0            ;; ZeroExt  R0,R0,#+16,#+16
??getEgtValue_1:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock4
//  117 	}
//  118 }
//  119 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z7egtReadv
        THUMB
//  120 static void egtRead(void) {
_Z7egtReadv:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        VPUSH    {D8}
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        SUB      SP,SP,#+8
          CFI CFA R13+32
//  121 
//  122 	if (driver == NULL) {
        LDR.N    R0,??DataTable5_10
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??egtRead_0
//  123 		scheduleMsg(logger, "No SPI selected for EGT");
        LDR.N    R1,??DataTable5_12
        LDR.N    R0,??DataTable5_2
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  124 		return;
        B.N      ??egtRead_1
//  125 	}
//  126 
//  127 	scheduleMsg(logger, "Reading egt");
??egtRead_0:
        LDR.N    R1,??DataTable5_13
        LDR.N    R0,??DataTable5_2
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  128 
//  129 	uint32_t egtPacket = readEgtPacket(0);
        MOVS     R0,#+0
          CFI FunCall _Z13readEgtPacketi
        BL       _Z13readEgtPacketi
        MOVS     R4,R0
//  130 
//  131 	max_32855_code code = getResultCode(egtPacket);
        MOVS     R0,R4
          CFI FunCall _Z13getResultCodej
        BL       _Z13getResultCodej
        MOVS     R5,R0
//  132 
//  133 	scheduleMsg(logger, "egt %x code=%d %s", egtPacket, code, getMcCode(code));
        MOVS     R0,R5
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
          CFI FunCall _Z9getMcCode14max_32855_code
        BL       _Z9getMcCode14max_32855_code
        STR      R0,[SP, #+0]
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        MOVS     R3,R5
        MOVS     R2,R4
        LDR.N    R1,??DataTable5_14
        LDR.N    R0,??DataTable5_2
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  134 
//  135 	if (code != MC_INVALID) {
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+1
        BEQ.N    ??egtRead_2
//  136 		int refBits = ((egtPacket & 0xFFFF) / 16); // bits 15:4
        UBFX     R6,R4,#+4,#+12
//  137 		float refTemp = refBits / 16.0;
        MOVS     R0,R6
          CFI FunCall __aeabi_i2d
        BL       __aeabi_i2d
        MOVS     R2,#+0
        LDR.N    R3,??DataTable5_15  ;; 0x40300000
          CFI FunCall __aeabi_ddiv
        BL       __aeabi_ddiv
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        VMOV     S16,R0
//  138 		scheduleMsg(logger, "reference temperature %f", refTemp);
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R1,??DataTable5_16
        LDR.N    R0,??DataTable5_2
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  139 
//  140 		scheduleMsg(logger, "EGT temperature %d", GET_TEMPERATURE_C(egtPacket));
        LSRS     R2,R4,#+20
        LDR.N    R1,??DataTable5_17
        LDR.N    R0,??DataTable5_2
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  141 	}
//  142 }
??egtRead_2:
??egtRead_1:
        ADD      SP,SP,#+8
          CFI CFA R13+24
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock5
//  143 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z12initMax31855P7LoggingP9SPIDriverP11brain_pin_e
        THUMB
//  144 void initMax31855(Logging *sharedLogger, SPIDriver *drv, egt_cs_array_t max31855_cs) {
_Z12initMax31855P7LoggingP9SPIDriverP11brain_pin_e:
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
//  145 	logger = sharedLogger;
        LDR.N    R0,??DataTable5_2
        STR      R4,[R0, #+0]
//  146 
//  147 	// todo:spi device is now enabled separately - should probably be enabled here
//  148 	driver = drv;
        LDR.N    R0,??DataTable5_10
        STR      R5,[R0, #+0]
//  149 
//  150 	addConsoleAction("egtinfo", (Void) showEgtInfo);
        LDR.N    R1,??DataTable5_18
        LDR.N    R0,??DataTable5_19
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  151 
//  152 	addConsoleAction("egtread", (Void) egtRead);
        LDR.N    R1,??DataTable5_20
        LDR.N    R0,??DataTable5_21
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  153 
//  154 	for (int i = 0; i < EGT_CHANNEL_COUNT; i++) {
        MOVS     R7,#+0
??initMax31855_0:
        CMP      R7,#+8
        BGE.N    ??initMax31855_1
//  155 		if (max31855_cs[i] != GPIO_UNASSIGNED) {
        LDR      R0,[R6, R7, LSL #+2]
        CMP      R0,#+80
        BEQ.N    ??initMax31855_2
//  156 
//  157 			initSpiCs(&spiConfig[i], max31855_cs[i]);
        LDR      R1,[R6, R7, LSL #+2]
        MOVS     R0,#+12
        LDR.N    R2,??DataTable5_11
        MLA      R0,R0,R7,R2
          CFI FunCall initSpiCs
        BL       initSpiCs
//  158 
//  159 			spiConfig[i].cr1 = SPI_BaudRatePrescaler_8;
        MOVS     R0,#+12
        LDR.N    R1,??DataTable5_11
        MLA      R0,R0,R7,R1
        MOVS     R1,#+16
        STRH     R1,[R0, #+10]
//  160 		}
//  161 	}
??initMax31855_2:
        ADDS     R7,R7,#+1
        B.N      ??initMax31855_0
//  162 }
??initMax31855_1:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock6

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5:
        DC8      0x4F, 0x6B, 0x00, 0x00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_1:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_2:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_3:
        DC32     `?<Constant "EGT spi: %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_4:
        DC32     `?<Constant "%d ETG @ %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_5:
        DC32     `?<Constant "Open">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_6:
        DC32     `?<Constant "short gnd">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_7:
        DC32     `?<Constant "short VCC">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_8:
        DC32     `?<Constant "invalid">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_9:
        DC32     0x20008

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_10:
        DC32     driver

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_11:
        DC32     spiConfig

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_12:
        DC32     `?<Constant "No SPI selected for EGT">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_13:
        DC32     `?<Constant "Reading egt">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_14:
        DC32     `?<Constant "egt %x code=%d %s">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_15:
        DC32     0x40300000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_16:
        DC32     `?<Constant "reference temperature %f">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_17:
        DC32     `?<Constant "EGT temperature %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_18:
        DC32     _Z11showEgtInfov

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_19:
        DC32     `?<Constant "egtinfo">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_20:
        DC32     _Z7egtReadv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_21:
        DC32     `?<Constant "egtread">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EGT spi: %d">`:
        DATA
        DC8 "EGT spi: %d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "%d ETG @ %s">`:
        DATA
        DC8 "%d ETG @ %s"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "Ok"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Open">`:
        DATA
        DC8 "Open"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "short gnd">`:
        DATA
        DC8 "short gnd"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "short VCC">`:
        DATA
        DC8 "short VCC"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid">`:
        DATA
        DC8 "invalid"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "No SPI selected for EGT">`:
        DATA
        DC8 "No SPI selected for EGT"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Reading egt">`:
        DATA
        DC8 "Reading egt"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "egt %x code=%d %s">`:
        DATA
        DC8 "egt %x code=%d %s"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "reference temperature %f">`:
        DATA
        DC8 "reference temperature %f"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "EGT temperature %d">`:
        DATA
        DC8 "EGT temperature %d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "egtinfo">`:
        DATA
        DC8 "egtinfo"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "egtread">`:
        DATA
        DC8 "egtread"

        END
//  163 
//  164 #endif /* EFI_MAX_31855 */
// 
// 104 bytes in section .bss
// 188 bytes in section .rodata
// 646 bytes in section .text
// 
// 646 bytes of CODE  memory
// 188 bytes of CONST memory
// 104 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
