///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:33 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\fla /
//                       sh_main.cpp                                          /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\fla /
//                       sh_main.cpp -lCN F:\stuff\rusefi_sourceforge\firmwar /
//                       e\iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge\fi /
//                       rmware\iar\Debug\List\ -o                            /
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
//                       flash_main.s                                         /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME flash_main

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z11requestBurnv
        EXTERN _Z21resetConfigurationExtP7Logging13engine_type_e
        EXTERN _Z31applyNonPersistentConfigurationP7Logging
        EXTERN addConsoleAction
        EXTERN calc_crc
        EXTERN currentTimeMillis
        EXTERN engineConfiguration
        EXTERN flashErase
        EXTERN flashRead
        EXTERN flashWrite
        EXTERN getRusEfiVersion
        EXTERN maxLockTime
        EXTERN persistentState
        EXTERN printMsg
        EXTERN scheduleMsg

        PUBLIC _Z13flashStateCrcP29persistent_config_container_s
        PUBLIC _Z13readFromFlashv
        PUBLIC _Z15writeToFlashNowv
        PUBLIC _Z21writeToFlashIfPendingv
        PUBLIC _Z27getNeedToWriteConfigurationv
        PUBLIC _Z27setNeedToWriteConfigurationv
        PUBLIC _Z9initFlashP7Logging
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\flash_main.cpp
//    1 /**
//    2  * @file    flash_main.cpp
//    3  * @brief	Higher-level logic of saving data into internal flash memory
//    4  *
//    5  *
//    6  * @date Sep 19, 2013
//    7  * @author Andrey Belomutskiy, (c) 2012-2015
//    8  */
//    9 
//   10 #include <main.h>
//   11 
//   12 #include "flash_main.h"
//   13 #include "eficonsole.h"
//   14 #include "flash.h"
//   15 #include "rusefi.h"
//   16 
//   17 #if EFI_TUNER_STUDIO || defined(__DOXYGEN__)
//   18 #include "tunerstudio.h"
//   19 #endif
//   20 
//   21 #if EFI_INTERNAL_FLASH || defined(__DOXYGEN__)
//   22 
//   23 #include "engine_controller.h"
//   24 #include "datalogging.h"
//   25 #include "engine.h"
//   26 

        SECTION `.bss`:DATA:REORDER:NOROOT(0)
//   27 static bool needToWriteConfiguration = false;
needToWriteConfiguration:
        DS8 1
//   28 
//   29 EXTERN_ENGINE;

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   30 static Logging* logger;
logger:
        DS8 4
//   31 
//   32 extern persistent_config_container_s persistentState;
//   33 
//   34 extern engine_configuration_s *engineConfiguration;
//   35 
//   36 #define FLASH_ADDR 0x08060000
//   37 
//   38 #define PERSISTENT_SIZE sizeof(persistent_config_container_s)
//   39 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z13flashStateCrcP29persistent_config_container_s
        THUMB
//   40 crc_t flashStateCrc(persistent_config_container_s *state) {
_Z13flashStateCrcP29persistent_config_container_s:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   41 	return calc_crc((const crc_t*) &state->persistentConfiguration, sizeof(persistent_config_s));
        MOVW     R1,#+15288
        ADDS     R0,R4,#+8
          CFI FunCall calc_crc
        BL       calc_crc
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0
//   42 }
//   43 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z27setNeedToWriteConfigurationv
        THUMB
//   44 void setNeedToWriteConfiguration(void) {
_Z27setNeedToWriteConfigurationv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   45 	scheduleMsg(logger, "Scheduling configuration write");
        LDR.N    R1,??DataTable8
        LDR.N    R0,??DataTable8_1
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   46 	needToWriteConfiguration = true;
        LDR.N    R0,??DataTable8_2
        MOVS     R1,#+1
        STRB     R1,[R0, #+0]
//   47 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock1
//   48 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z27getNeedToWriteConfigurationv
          CFI NoCalls
        THUMB
//   49 bool getNeedToWriteConfiguration(void) {
//   50 	return needToWriteConfiguration;
_Z27getNeedToWriteConfigurationv:
        LDR.N    R0,??DataTable8_2
        LDRB     R0,[R0, #+0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock2
//   51 }
//   52 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z21writeToFlashIfPendingv
        THUMB
//   53 void writeToFlashIfPending() {
_Z21writeToFlashIfPendingv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   54 	if (!getNeedToWriteConfiguration()) {
          CFI FunCall _Z27getNeedToWriteConfigurationv
        BL       _Z27getNeedToWriteConfigurationv
        CMP      R0,#+0
        BEQ.N    ??writeToFlashIfPending_0
//   55 		return;
//   56 	}
//   57 	// todo: technically we need a lock here, realistically we should be fine.
//   58 	needToWriteConfiguration = false;
??writeToFlashIfPending_1:
        LDR.N    R0,??DataTable8_2
        MOVS     R1,#+0
        STRB     R1,[R0, #+0]
//   59 	scheduleMsg(logger, "Writing pending configuration");
        LDR.N    R1,??DataTable8_3
        LDR.N    R0,??DataTable8_1
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   60 	writeToFlashNow();
          CFI FunCall _Z15writeToFlashNowv
        BL       _Z15writeToFlashNowv
//   61 }
??writeToFlashIfPending_0:
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock3
//   62 
//   63 extern uint32_t maxLockTime;
//   64 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z15writeToFlashNowv
        THUMB
//   65 void writeToFlashNow(void) {
_Z15writeToFlashNowv:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
//   66 	scheduleMsg(logger, " !!!!!!!!!!!!!!!!!!!! BE SURE NOT WRITE WITH IGNITION ON !!!!!!!!!!!!!!!!!!!!");
        LDR.N    R1,??DataTable8_4
        LDR.N    R0,??DataTable8_1
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   67 	persistentState.size = PERSISTENT_SIZE;
        LDR.N    R0,??DataTable8_5
        MOVW     R1,#+15300
        STR      R1,[R0, #+4]
//   68 	persistentState.version = FLASH_DATA_VERSION;
        LDR.N    R0,??DataTable8_5
        MOVW     R1,#+8400
        STR      R1,[R0, #+0]
//   69 	scheduleMsg(logger, "flash compatible with %d", persistentState.version);
        LDR.N    R0,??DataTable8_5
        LDR      R2,[R0, #+0]
        LDR.N    R1,??DataTable8_6
        LDR.N    R0,??DataTable8_1
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   70 	crc_t result = flashStateCrc(&persistentState);
        LDR.N    R0,??DataTable8_5
          CFI FunCall _Z13flashStateCrcP29persistent_config_container_s
        BL       _Z13flashStateCrcP29persistent_config_container_s
        MOVS     R4,R0
//   71 	persistentState.value = result;
        LDR.N    R0,??DataTable8_7
        STRB     R4,[R0, #+0]
//   72 	scheduleMsg(logger, "Reseting flash: size=%d", PERSISTENT_SIZE);
        MOVW     R2,#+15300
        LDR.N    R1,??DataTable8_8
        LDR.N    R0,??DataTable8_1
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   73 	flashErase(FLASH_ADDR, PERSISTENT_SIZE);
        MOVW     R1,#+15300
        LDR.N    R0,??DataTable8_9  ;; 0x8060000
          CFI FunCall flashErase
        BL       flashErase
//   74 	scheduleMsg(logger, "Flashing with CRC=%d", result);
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        MOVS     R2,R4
        LDR.N    R1,??DataTable8_10
        LDR.N    R0,??DataTable8_1
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   75 	efitimems_t nowMs = currentTimeMillis();
          CFI FunCall currentTimeMillis
        BL       currentTimeMillis
        MOVS     R5,R0
//   76 	result = flashWrite(FLASH_ADDR, (const char *) &persistentState, PERSISTENT_SIZE);
        MOVW     R2,#+15300
        LDR.N    R1,??DataTable8_5
        LDR.N    R0,??DataTable8_9  ;; 0x8060000
          CFI FunCall flashWrite
        BL       flashWrite
        MOVS     R4,R0
//   77 	scheduleMsg(logger, "Flash programmed in (ms): %d", currentTimeMillis() - nowMs);
          CFI FunCall currentTimeMillis
        BL       currentTimeMillis
        SUBS     R2,R0,R5
        LDR.N    R1,??DataTable8_11
        LDR.N    R0,??DataTable8_1
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   78 	scheduleMsg(logger, "Flashing result: %d", result);
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        MOVS     R2,R4
        LDR.N    R1,??DataTable8_12
        LDR.N    R0,??DataTable8_1
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   79 	maxLockTime = 0;
        LDR.N    R0,??DataTable8_13
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//   80 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock4
//   81 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z10isValidCrcP29persistent_config_container_s
        THUMB
//   82 static bool isValidCrc(persistent_config_container_s *state) {
_Z10isValidCrcP29persistent_config_container_s:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//   83 	crc_t result = flashStateCrc(state);
        MOVS     R0,R4
          CFI FunCall _Z13flashStateCrcP29persistent_config_container_s
        BL       _Z13flashStateCrcP29persistent_config_container_s
        MOVS     R5,R0
//   84 	int isValidCrc_b = result == state->value;
        ADDS     R0,R4,#+15296
        LDRB     R0,[R0, #+0]
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,R0
        BNE.N    ??isValidCrc_0
        MOVS     R6,#+1
        B.N      ??isValidCrc_1
??isValidCrc_0:
        MOVS     R6,#+0
??isValidCrc_1:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
//   85 	if (!isValidCrc_b) {
        CMP      R6,#+0
        BNE.N    ??isValidCrc_2
//   86 		scheduleMsg(logger, "CRC got %d while %d expected", result, state->value);
        ADDS     R0,R4,#+15296
        LDRB     R3,[R0, #+0]
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        MOVS     R2,R5
        LDR.N    R1,??DataTable8_14
        LDR.N    R0,??DataTable8_1
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//   87 	}
//   88 	return isValidCrc_b;
??isValidCrc_2:
        CMP      R6,#+0
        BEQ.N    ??isValidCrc_3
        MOVS     R0,#+1
        B.N      ??isValidCrc_4
??isValidCrc_3:
        MOVS     R0,#+0
??isValidCrc_4:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock5
//   89 }
//   90 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z20doResetConfigurationv
        THUMB
//   91 static void doResetConfiguration(void) {
_Z20doResetConfigurationv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   92 	resetConfigurationExt(logger, engineConfiguration->engineType PASS_ENGINE_PARAMETER);
        LDR.N    R0,??DataTable8_15
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+0]
        LDR.N    R0,??DataTable8_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z21resetConfigurationExtP7Logging13engine_type_e
        BL       _Z21resetConfigurationExtP7Logging13engine_type_e
//   93 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock6
//   94 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z18hasValidEngineTypeP22engine_configuration_s
          CFI NoCalls
        THUMB
//   95 static bool hasValidEngineType(engine_configuration_s *engineConfiguration) {
//   96 	uint32_t ordinal = (uint32_t)engineConfiguration->engineType;
_Z18hasValidEngineTypeP22engine_configuration_s:
        LDR      R1,[R0, #+0]
//   97 	return ordinal < ET_UNUSED && engineConfiguration->headerMagicValue == HEADER_MAGIC_NUMBER;
        CMP      R1,#+33
        BCS.N    ??hasValidEngineType_0
        LDR      R0,[R0, #+4]
        LDR.N    R1,??DataTable8_16  ;; 0x1221239
        CMP      R0,R1
        BNE.N    ??hasValidEngineType_0
        MOVS     R0,#+1
        B.N      ??hasValidEngineType_1
??hasValidEngineType_0:
        MOVS     R0,#+0
??hasValidEngineType_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return
          CFI EndBlock cfiBlock7
//   98 }
//   99 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z13readFromFlashv
        THUMB
//  100 void readFromFlash(void) {
_Z13readFromFlashv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  101 	printMsg(logger, "readFromFlash()");
        LDR.N    R1,??DataTable8_17
        LDR.N    R0,??DataTable8_1
        LDR      R0,[R0, #+0]
          CFI FunCall printMsg
        BL       printMsg
//  102 	flashRead(FLASH_ADDR, (char *) &persistentState, PERSISTENT_SIZE);
        MOVW     R2,#+15300
        LDR.N    R1,??DataTable8_5
        LDR.N    R0,??DataTable8_9  ;; 0x8060000
          CFI FunCall flashRead
        BL       flashRead
//  103 
//  104 	if (!isValidCrc(&persistentState)) {
        LDR.N    R0,??DataTable8_5
          CFI FunCall _Z10isValidCrcP29persistent_config_container_s
        BL       _Z10isValidCrcP29persistent_config_container_s
        CMP      R0,#+0
        BNE.N    ??readFromFlash_0
//  105 		printMsg(logger, "Need to reset flash to default due to CRC");
        LDR.N    R1,??DataTable8_18
        LDR.N    R0,??DataTable8_1
        LDR      R0,[R0, #+0]
          CFI FunCall printMsg
        BL       printMsg
//  106 		resetConfigurationExt(logger, DEFAULT_ENGINE_TYPE PASS_ENGINE_PARAMETER);
        MOVS     R1,#+0
        LDR.N    R0,??DataTable8_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z21resetConfigurationExtP7Logging13engine_type_e
        BL       _Z21resetConfigurationExtP7Logging13engine_type_e
        B.N      ??readFromFlash_1
//  107 	} else if (persistentState.version == FLASH_DATA_VERSION && persistentState.size == PERSISTENT_SIZE) {
??readFromFlash_0:
        LDR.N    R0,??DataTable8_5
        LDR      R0,[R0, #+0]
        MOVW     R1,#+8400
        CMP      R0,R1
        BNE.N    ??readFromFlash_2
        LDR.N    R0,??DataTable8_5
        LDR      R0,[R0, #+4]
        MOVW     R1,#+15300
        CMP      R0,R1
        BNE.N    ??readFromFlash_2
//  108 		printMsg(logger, "Got valid configuration from flash!");
        LDR.N    R1,??DataTable8_19
        LDR.N    R0,??DataTable8_1
        LDR      R0,[R0, #+0]
          CFI FunCall printMsg
        BL       printMsg
//  109 		applyNonPersistentConfiguration(logger PASS_ENGINE_PARAMETER);
        LDR.N    R0,??DataTable8_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z31applyNonPersistentConfigurationP7Logging
        BL       _Z31applyNonPersistentConfigurationP7Logging
        B.N      ??readFromFlash_1
//  110 	} else if (hasValidEngineType(engineConfiguration)) {
??readFromFlash_2:
        LDR.N    R0,??DataTable8_15
        LDR      R0,[R0, #+0]
          CFI FunCall _Z18hasValidEngineTypeP22engine_configuration_s
        BL       _Z18hasValidEngineTypeP22engine_configuration_s
        CMP      R0,#+0
        BEQ.N    ??readFromFlash_3
//  111 		printMsg(logger, "Resetting but saving engine type [%d]", engineConfiguration->engineType);
        LDR.N    R0,??DataTable8_15
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+0]
        LDR.N    R1,??DataTable8_20
        LDR.N    R0,??DataTable8_1
        LDR      R0,[R0, #+0]
          CFI FunCall printMsg
        BL       printMsg
//  112 		resetConfigurationExt(logger, engineConfiguration->engineType PASS_ENGINE_PARAMETER);
        LDR.N    R0,??DataTable8_15
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+0]
        LDR.N    R0,??DataTable8_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z21resetConfigurationExtP7Logging13engine_type_e
        BL       _Z21resetConfigurationExtP7Logging13engine_type_e
        B.N      ??readFromFlash_1
//  113 	} else {
//  114 		printMsg(logger, "Need to reset flash to default due to version change");
??readFromFlash_3:
        LDR.N    R1,??DataTable8_21
        LDR.N    R0,??DataTable8_1
        LDR      R0,[R0, #+0]
          CFI FunCall printMsg
        BL       printMsg
//  115 		resetConfigurationExt(logger, DEFAULT_ENGINE_TYPE PASS_ENGINE_PARAMETER);
        MOVS     R1,#+0
        LDR.N    R0,??DataTable8_1
        LDR      R0,[R0, #+0]
          CFI FunCall _Z21resetConfigurationExtP7Logging13engine_type_e
        BL       _Z21resetConfigurationExtP7Logging13engine_type_e
//  116 	}
//  117 	// we can only change the state after the CRC check
//  118 	engineConfiguration->firmwareVersion = getRusEfiVersion();
??readFromFlash_1:
          CFI FunCall getRusEfiVersion
        BL       getRusEfiVersion
        LDR.N    R1,??DataTable8_15
        LDR      R1,[R1, #+0]
        STR      R0,[R1, #+512]
//  119 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock8
//  120 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _Z13rewriteConfigv
        THUMB
//  121 static void rewriteConfig(void) {
_Z13rewriteConfigv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  122 	doResetConfiguration();
          CFI FunCall _Z20doResetConfigurationv
        BL       _Z20doResetConfigurationv
//  123 	writeToFlashNow();
          CFI FunCall _Z15writeToFlashNowv
        BL       _Z15writeToFlashNowv
//  124 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock9
//  125 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _Z9initFlashP7Logging
        THUMB
//  126 void initFlash(Logging *sharedLogger) {
_Z9initFlashP7Logging:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  127 	logger = sharedLogger;
        LDR.N    R0,??DataTable8_1
        STR      R4,[R0, #+0]
//  128 
//  129 	addConsoleAction("readconfig", readFromFlash);
        LDR.N    R1,??DataTable8_22
        LDR.N    R0,??DataTable8_23
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  130 	/**
//  131 	 * This would write NOW (you should not be doing this while connected to real engine)
//  132 	 */
//  133 	addConsoleAction("writeconfig", writeToFlashNow);
        LDR.N    R1,??DataTable8_24
        LDR.N    R0,??DataTable8_25
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  134 #if EFI_TUNER_STUDIO || defined(__DOXYGEN__)
//  135 	/**
//  136 	 * This would schedule write to flash once the engine is stopped
//  137 	 */
//  138 	addConsoleAction("burnconfig", requestBurn);
        LDR.N    R1,??DataTable8_26
        LDR.N    R0,??DataTable8_27
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  139 #endif
//  140 	addConsoleAction("resetconfig", doResetConfiguration);
        LDR.N    R1,??DataTable8_28
        LDR.N    R0,??DataTable8_29
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  141 	addConsoleAction("rewriteconfig", rewriteConfig);
        LDR.N    R1,??DataTable8_30
        LDR.N    R0,??DataTable8_31
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  142 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock10

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8:
        DC32     `?<Constant "Scheduling configurat...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_1:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_2:
        DC32     needToWriteConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_3:
        DC32     `?<Constant "Writing pending confi...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_4:
        DC32     `?<Constant " !!!!!!!!!!!!!!!!!!!!...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_5:
        DC32     persistentState

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_6:
        DC32     `?<Constant "flash compatible with %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_7:
        DC32     persistentState+0x3BC0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_8:
        DC32     `?<Constant "Reseting flash: size=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_9:
        DC32     0x8060000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_10:
        DC32     `?<Constant "Flashing with CRC=%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_11:
        DC32     `?<Constant "Flash programmed in (...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_12:
        DC32     `?<Constant "Flashing result: %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_13:
        DC32     maxLockTime

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_14:
        DC32     `?<Constant "CRC got %d while %d e...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_15:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_16:
        DC32     0x1221239

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_17:
        DC32     `?<Constant "readFromFlash()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_18:
        DC32     `?<Constant "Need to reset flash t...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_19:
        DC32     `?<Constant "Got valid configurati...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_20:
        DC32     `?<Constant "Resetting but saving ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_21:
        DC32     `?<Constant "Need to reset flash t...">_1`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_22:
        DC32     _Z13readFromFlashv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_23:
        DC32     `?<Constant "readconfig">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_24:
        DC32     _Z15writeToFlashNowv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_25:
        DC32     `?<Constant "writeconfig">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_26:
        DC32     _Z11requestBurnv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_27:
        DC32     `?<Constant "burnconfig">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_28:
        DC32     _Z20doResetConfigurationv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_29:
        DC32     `?<Constant "resetconfig">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_30:
        DC32     _Z13rewriteConfigv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_31:
        DC32     `?<Constant "rewriteconfig">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Scheduling configurat...">`:
        DATA
        DC8 "Scheduling configuration write"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Writing pending confi...">`:
        DATA
        DC8 "Writing pending configuration"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant " !!!!!!!!!!!!!!!!!!!!...">`:
        DATA
        DC8 20H, 21H, 21H, 21H, 21H, 21H, 21H, 21H
        DC8 21H, 21H, 21H, 21H, 21H, 21H, 21H, 21H
        DC8 21H, 21H, 21H, 21H, 21H, 20H, 42H, 45H
        DC8 20H, 53H, 55H, 52H, 45H, 20H, 4EH, 4FH
        DC8 54H, 20H, 57H, 52H, 49H, 54H, 45H, 20H
        DC8 57H, 49H, 54H, 48H, 20H, 49H, 47H, 4EH
        DC8 49H, 54H, 49H, 4FH, 4EH, 20H, 4FH, 4EH
        DC8 20H, 21H, 21H, 21H, 21H, 21H, 21H, 21H
        DC8 21H, 21H, 21H, 21H, 21H, 21H, 21H, 21H
        DC8 21H, 21H, 21H, 21H, 21H, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "flash compatible with %d">`:
        DATA
        DC8 "flash compatible with %d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Reseting flash: size=%d">`:
        DATA
        DC8 "Reseting flash: size=%d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Flashing with CRC=%d">`:
        DATA
        DC8 "Flashing with CRC=%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Flash programmed in (...">`:
        DATA
        DC8 "Flash programmed in (ms): %d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Flashing result: %d">`:
        DATA
        DC8 "Flashing result: %d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "CRC got %d while %d e...">`:
        DATA
        DC8 "CRC got %d while %d expected"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "readFromFlash()">`:
        DATA
        DC8 "readFromFlash()"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Need to reset flash t...">`:
        DATA
        DC8 "Need to reset flash to default due to CRC"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Got valid configurati...">`:
        DATA
        DC8 "Got valid configuration from flash!"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Resetting but saving ...">`:
        DATA
        DC8 "Resetting but saving engine type [%d]"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Need to reset flash t...">_1`:
        DATA
        DC8 4EH, 65H, 65H, 64H, 20H, 74H, 6FH, 20H
        DC8 72H, 65H, 73H, 65H, 74H, 20H, 66H, 6CH
        DC8 61H, 73H, 68H, 20H, 74H, 6FH, 20H, 64H
        DC8 65H, 66H, 61H, 75H, 6CH, 74H, 20H, 64H
        DC8 75H, 65H, 20H, 74H, 6FH, 20H, 76H, 65H
        DC8 72H, 73H, 69H, 6FH, 6EH, 20H, 63H, 68H
        DC8 61H, 6EH, 67H, 65H, 0
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "readconfig">`:
        DATA
        DC8 "readconfig"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "writeconfig">`:
        DATA
        DC8 "writeconfig"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "burnconfig">`:
        DATA
        DC8 "burnconfig"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "resetconfig">`:
        DATA
        DC8 "resetconfig"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "rewriteconfig">`:
        DATA
        DC8 "rewriteconfig"
        DC8 0, 0

        END
//  143 
//  144 #endif /* EFI_INTERNAL_FLASH */
// 
//   5 bytes in section .bss
// 560 bytes in section .rodata
// 706 bytes in section .text
// 
// 706 bytes of CODE  memory
// 560 bytes of CONST memory
//   5 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
