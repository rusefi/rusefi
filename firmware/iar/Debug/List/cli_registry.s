///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:47 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\util\cli_regist /
//                       ry.cpp                                               /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\util\cli_regist /
//                       ry.cpp -lCN F:\stuff\rusefi_sourceforge\firmware\iar /
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
//                       cli_registry.s                                       /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME cli_registry

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN absI
        EXTERN atoff
        EXTERN atoi
        EXTERN firmwareError
        EXTERN indexOf
        EXTERN isBoardTestMode
        EXTERN print
        EXTERN printBoardTestState
        EXTERN scheduleMsg
        EXTERN strEqual
        EXTERN strcat
        EXTERN strcpy
        EXTERN strlen
        EXTERN strncmp
        EXTERN strncpy

        PUBLIC _Z25handleActionWithParameterP13TokenCallbackPc
        PUBLIC addConsoleAction
        PUBLIC addConsoleActionF
        PUBLIC addConsoleActionFF
        PUBLIC addConsoleActionFFP
        PUBLIC addConsoleActionI
        PUBLIC addConsoleActionII
        PUBLIC addConsoleActionIIP
        PUBLIC addConsoleActionIP
        PUBLIC addConsoleActionP
        PUBLIC addConsoleActionS
        PUBLIC addConsoleActionSP
        PUBLIC addConsoleActionSS
        PUBLIC addConsoleActionSSP
        PUBLIC addConsoleActionSSS
        PUBLIC addConsoleActionSSSSS
        PUBLIC findEndOfToken
        PUBLIC handleConsoleLine
        PUBLIC helpCommand
        PUBLIC initConsoleLogic
        PUBLIC resetConsoleActions
        PUBLIC tokenLength
        PUBLIC unquote
        PUBLIC validateSecureLine
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\util\cli_registry.cpp
//    1 /**
//    2  * @file    cli_registry.cpp
//    3  * @brief   Command-line interface commands registry
//    4  *
//    5  * Here we have a data structure which holds all the dynamically-registered
//    6  * command line interface action names & callback. This logic is invoked in
//    7  * user context by the console thread - see consoleThreadThreadEntryPoint
//    8  *
//    9  * TODO: there is too much copy-paste here, this class needs some refactoring :)
//   10  *
//   11  * see testConsoleLogic()
//   12  *
//   13  * @date Nov 15, 2012
//   14  * @author Andrey Belomutskiy, (c) 2012-2015
//   15  */
//   16 
//   17 #include <stdarg.h>
//   18 #include <string.h>
//   19 #include <stdbool.h>
//   20 #include "main.h"
//   21 #include "cli_registry.h"
//   22 #include "efilib.h"
//   23 
//   24 #if EFI_PROD_CODE
//   25 #include "board_test.h"
//   26 #endif
//   27 
//   28 #if ! EFI_UNIT_TEST
//   29 #include "eficonsole.h"
//   30 #endif /* ! EFI_UNIT_TEST */
//   31 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   32 static Logging * logging;
logging:
        DS8 4
//   33 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   34 static int consoleActionCount = 0;
consoleActionCount:
        DS8 4

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   35 static TokenCallback consoleActions[CONSOLE_MAX_ACTIONS];
consoleActions:
        DS8 3136
//   36 
//   37 #define SECURE_LINE_PREFIX "sec!"
//   38 #define SECURE_LINE_PREFIX_LENGTH 4
//   39 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function resetConsoleActions
          CFI NoCalls
        THUMB
//   40 void resetConsoleActions(void) {
//   41 	consoleActionCount = 0;
resetConsoleActions:
        LDR.W    R0,??DataTable9_1
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//   42 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//   43 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z11doAddActionPKc13action_type_ePFvvEPv
        THUMB
//   44 static void doAddAction(const char *token, action_type_e type, Void callback, void *param) {
_Z11doAddActionPKc13action_type_ePFvvEPv:
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
        MOVS     R7,R3
//   45 	efiAssertVoid(consoleActionCount < CONSOLE_MAX_ACTIONS, "Too many console actions");
        LDR.W    R0,??DataTable9_1
        LDR      R0,[R0, #+0]
        CMP      R0,#+196
        BLT.N    ??doAddAction_0
        LDR.W    R0,??DataTable9_2
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??doAddAction_1
//   46 	TokenCallback *current = &consoleActions[consoleActionCount++];
??doAddAction_0:
        LDR.W    R0,??DataTable9_1
        LDR      R0,[R0, #+0]
        LDR.W    R1,??DataTable9_3
        ADDS     R0,R1,R0, LSL #+4
        LDR.W    R1,??DataTable9_1
        LDR      R1,[R1, #+0]
        ADDS     R1,R1,#+1
        LDR.W    R2,??DataTable9_1
        STR      R1,[R2, #+0]
//   47 	current->token = token;
        STR      R4,[R0, #+0]
//   48 	current->parameterType = type;
        STRB     R5,[R0, #+4]
//   49 	current->callback = callback;
        STR      R6,[R0, #+8]
//   50 	current->param = param;
        STR      R7,[R0, #+12]
//   51 }
??doAddAction_1:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock1
//   52 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function addConsoleActionP
        THUMB
//   53 void addConsoleActionP(const char *token, VoidPtr callback, void *param) {
addConsoleActionP:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   54 	doAddAction(token, NO_PARAMETER_P, (Void) callback, param);
        MOVS     R3,R6
        MOVS     R2,R5
        MOVS     R1,#+1
        MOVS     R0,R4
          CFI FunCall _Z11doAddActionPKc13action_type_ePFvvEPv
        BL       _Z11doAddActionPKc13action_type_ePFvvEPv
//   55 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock2
//   56 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function addConsoleActionSSP
        THUMB
//   57 void addConsoleActionSSP(const char *token, VoidCharPtrCharPtrVoidPtr callback, void *param) {
addConsoleActionSSP:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   58 	doAddAction(token, STRING2_PARAMETER_P, (Void) callback, param);
        MOVS     R3,R6
        MOVS     R2,R5
        MOVS     R1,#+9
        MOVS     R0,R4
          CFI FunCall _Z11doAddActionPKc13action_type_ePFvvEPv
        BL       _Z11doAddActionPKc13action_type_ePFvvEPv
//   59 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock3
//   60 
//   61 /**
//   62  * @brief	Register console action without parameters
//   63  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function addConsoleAction
        THUMB
//   64 void addConsoleAction(const char *token, Void callback) {
addConsoleAction:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   65 	doAddAction(token, NO_PARAMETER, callback, NULL);
        MOVS     R3,#+0
        MOVS     R2,R5
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall _Z11doAddActionPKc13action_type_ePFvvEPv
        BL       _Z11doAddActionPKc13action_type_ePFvvEPv
//   66 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock4
//   67 
//   68 /**
//   69  * @brief	Register a console command with one Integer parameter
//   70  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function addConsoleActionI
        THUMB
//   71 void addConsoleActionI(const char *token, VoidInt callback) {
addConsoleActionI:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   72 	doAddAction(token, ONE_PARAMETER, (Void) callback, NULL);
        MOVS     R3,#+0
        MOVS     R2,R5
        MOVS     R1,#+2
        MOVS     R0,R4
          CFI FunCall _Z11doAddActionPKc13action_type_ePFvvEPv
        BL       _Z11doAddActionPKc13action_type_ePFvvEPv
//   73 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock5
//   74 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function addConsoleActionIP
        THUMB
//   75 void addConsoleActionIP(const char *token, VoidIntVoidPtr callback, void *param) {
addConsoleActionIP:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   76 	doAddAction(token, ONE_PARAMETER_P, (Void) callback, param);
        MOVS     R3,R6
        MOVS     R2,R5
        MOVS     R1,#+3
        MOVS     R0,R4
          CFI FunCall _Z11doAddActionPKc13action_type_ePFvvEPv
        BL       _Z11doAddActionPKc13action_type_ePFvvEPv
//   77 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock6
//   78 
//   79 /**
//   80  * @brief	Register a console command with two Integer parameters
//   81  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function addConsoleActionII
        THUMB
//   82 void addConsoleActionII(const char *token, VoidIntInt callback) {
addConsoleActionII:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   83 	doAddAction(token, TWO_INTS_PARAMETER, (Void) callback, NULL);
        MOVS     R3,#+0
        MOVS     R2,R5
        MOVS     R1,#+12
        MOVS     R0,R4
          CFI FunCall _Z11doAddActionPKc13action_type_ePFvvEPv
        BL       _Z11doAddActionPKc13action_type_ePFvvEPv
//   84 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock7
//   85 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function addConsoleActionIIP
        THUMB
//   86 void addConsoleActionIIP(const char *token, VoidIntIntVoidPtr callback, void *param) {
addConsoleActionIIP:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   87 	doAddAction(token, TWO_INTS_PARAMETER_P, (Void) callback, param);
        MOVS     R3,R6
        MOVS     R2,R5
        MOVS     R1,#+13
        MOVS     R0,R4
          CFI FunCall _Z11doAddActionPKc13action_type_ePFvvEPv
        BL       _Z11doAddActionPKc13action_type_ePFvvEPv
//   88 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock8
//   89 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function addConsoleActionS
        THUMB
//   90 void addConsoleActionS(const char *token, VoidCharPtr callback) {
addConsoleActionS:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   91 	doAddAction(token, STRING_PARAMETER, (Void) callback, NULL);
        MOVS     R3,#+0
        MOVS     R2,R5
        MOVS     R1,#+6
        MOVS     R0,R4
          CFI FunCall _Z11doAddActionPKc13action_type_ePFvvEPv
        BL       _Z11doAddActionPKc13action_type_ePFvvEPv
//   92 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock9
//   93 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function addConsoleActionSP
        THUMB
//   94 void addConsoleActionSP(const char *token, VoidCharPtrVoidPtr callback, void *param) {
addConsoleActionSP:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   95 	doAddAction(token, STRING_PARAMETER_P, (Void) callback, param);
        MOVS     R3,R6
        MOVS     R2,R5
        MOVS     R1,#+7
        MOVS     R0,R4
          CFI FunCall _Z11doAddActionPKc13action_type_ePFvvEPv
        BL       _Z11doAddActionPKc13action_type_ePFvvEPv
//   96 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock10
//   97 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function addConsoleActionSS
        THUMB
//   98 void addConsoleActionSS(const char *token, VoidCharPtrCharPtr callback) {
addConsoleActionSS:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   99 	doAddAction(token, STRING2_PARAMETER, (Void) callback, NULL);
        MOVS     R3,#+0
        MOVS     R2,R5
        MOVS     R1,#+8
        MOVS     R0,R4
          CFI FunCall _Z11doAddActionPKc13action_type_ePFvvEPv
        BL       _Z11doAddActionPKc13action_type_ePFvvEPv
//  100 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock11
//  101 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function addConsoleActionSSS
        THUMB
//  102 void addConsoleActionSSS(const char *token, VoidCharPtrCharPtrCharPtr callback) {
addConsoleActionSSS:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  103 	doAddAction(token, STRING3_PARAMETER, (Void) callback, NULL);
        MOVS     R3,#+0
        MOVS     R2,R5
        MOVS     R1,#+10
        MOVS     R0,R4
          CFI FunCall _Z11doAddActionPKc13action_type_ePFvvEPv
        BL       _Z11doAddActionPKc13action_type_ePFvvEPv
//  104 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock12
//  105 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function addConsoleActionSSSSS
        THUMB
//  106 void addConsoleActionSSSSS(const char *token, VoidCharPtrCharPtrCharPtrCharPtrCharPtr callback) {
addConsoleActionSSSSS:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  107 	doAddAction(token, STRING5_PARAMETER, (Void) callback, NULL);
        MOVS     R3,#+0
        MOVS     R2,R5
        MOVS     R1,#+11
        MOVS     R0,R4
          CFI FunCall _Z11doAddActionPKc13action_type_ePFvvEPv
        BL       _Z11doAddActionPKc13action_type_ePFvvEPv
//  108 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock13
//  109 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function addConsoleActionF
        THUMB
//  110 void addConsoleActionF(const char *token, VoidFloat callback) {
addConsoleActionF:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  111 	doAddAction(token, FLOAT_PARAMETER, (Void) callback, NULL);
        MOVS     R3,#+0
        MOVS     R2,R5
        MOVS     R1,#+4
        MOVS     R0,R4
          CFI FunCall _Z11doAddActionPKc13action_type_ePFvvEPv
        BL       _Z11doAddActionPKc13action_type_ePFvvEPv
//  112 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock14
//  113 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function addConsoleActionFF
        THUMB
//  114 void addConsoleActionFF(const char *token, VoidFloatFloat callback) {
addConsoleActionFF:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  115 	doAddAction(token, FLOAT_FLOAT_PARAMETER, (Void) callback, NULL);
        MOVS     R3,#+0
        MOVS     R2,R5
        MOVS     R1,#+14
        MOVS     R0,R4
          CFI FunCall _Z11doAddActionPKc13action_type_ePFvvEPv
        BL       _Z11doAddActionPKc13action_type_ePFvvEPv
//  116 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock15
//  117 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function addConsoleActionFFP
        THUMB
//  118 void addConsoleActionFFP(const char *token, VoidFloatFloatVoidPtr callback, void *param) {
addConsoleActionFFP:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  119 	doAddAction(token, FLOAT_FLOAT_PARAMETER_P, (Void) callback, param);
        MOVS     R3,R6
        MOVS     R2,R5
        MOVS     R1,#+15
        MOVS     R0,R4
          CFI FunCall _Z11doAddActionPKc13action_type_ePFvvEPv
        BL       _Z11doAddActionPKc13action_type_ePFvvEPv
//  120 }
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock16
//  121 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function _Z17getParameterCount13action_type_e
          CFI NoCalls
        THUMB
//  122 static int getParameterCount(action_type_e parameterType) {
_Z17getParameterCount13action_type_e:
        MOVS     R1,R0
//  123 	switch (parameterType) {
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R1
        CMP      R0,#+0
        CMP      R0,#+1
        BLS.N    ??getParameterCount_0
        SUBS     R0,R0,#+2
        CMP      R0,#+2
        BLS.N    ??getParameterCount_1
        SUBS     R0,R0,#+4
        BEQ.N    ??getParameterCount_1
        SUBS     R0,R0,#+2
        CMP      R0,#+1
        BLS.N    ??getParameterCount_2
        SUBS     R0,R0,#+2
        BEQ.N    ??getParameterCount_3
        SUBS     R0,R0,#+1
        BEQ.N    ??getParameterCount_4
        SUBS     R0,R0,#+1
        CMP      R0,#+3
        BLS.N    ??getParameterCount_2
        B.N      ??getParameterCount_5
//  124 	case NO_PARAMETER:
//  125 	case NO_PARAMETER_P:
//  126 		return 0;
??getParameterCount_0:
        MOVS     R0,#+0
        B.N      ??getParameterCount_6
//  127 	case ONE_PARAMETER:
//  128 	case ONE_PARAMETER_P:
//  129 	case FLOAT_PARAMETER:
//  130 	case STRING_PARAMETER:
//  131 		return 1;
??getParameterCount_1:
        MOVS     R0,#+1
        B.N      ??getParameterCount_6
//  132 	case FLOAT_FLOAT_PARAMETER:
//  133 	case FLOAT_FLOAT_PARAMETER_P:
//  134 	case STRING2_PARAMETER:
//  135 	case STRING2_PARAMETER_P:
//  136 	case TWO_INTS_PARAMETER:
//  137 	case TWO_INTS_PARAMETER_P:
//  138 		return 2;
??getParameterCount_2:
        MOVS     R0,#+2
        B.N      ??getParameterCount_6
//  139 	case STRING3_PARAMETER:
//  140 		return 3;
??getParameterCount_3:
        MOVS     R0,#+3
        B.N      ??getParameterCount_6
//  141 	case STRING5_PARAMETER:
//  142 		return 5;
??getParameterCount_4:
        MOVS     R0,#+5
        B.N      ??getParameterCount_6
//  143 	default:
//  144 		return -1;
??getParameterCount_5:
        MOVS     R0,#-1
??getParameterCount_6:
        BX       LR               ;; return
          CFI EndBlock cfiBlock17
//  145 	}
//  146 }
//  147 
//  148 /**
//  149  * @brief This function prints out a list of all available commands
//  150  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function helpCommand
        THUMB
//  151 void helpCommand(void) {
helpCommand:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
//  152 #if EFI_PROD_CODE
//  153 	if (isBoardTestMode()) {
          CFI FunCall isBoardTestMode
        BL       isBoardTestMode
        CMP      R0,#+0
        BEQ.N    ??helpCommand_0
//  154 		printBoardTestState();
          CFI FunCall printBoardTestState
        BL       printBoardTestState
//  155 		return;
        B.N      ??helpCommand_1
//  156 	}
//  157 #endif /* EFI_PROD_CODE */
//  158 
//  159 #if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
//  160 	scheduleMsg(logging, "%d actions available", consoleActionCount);
??helpCommand_0:
        LDR.W    R0,??DataTable9_1
        LDR      R2,[R0, #+0]
        LDR.W    R1,??DataTable9_4
        LDR.W    R0,??DataTable9_5
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  161 	for (int i = 0; i < consoleActionCount; i++) {
        MOVS     R4,#+0
??helpCommand_2:
        LDR.W    R0,??DataTable9_1
        LDR      R0,[R0, #+0]
        CMP      R4,R0
        BGE.N    ??helpCommand_3
//  162 		TokenCallback *current = &consoleActions[i];
        LDR.W    R0,??DataTable9_3
        ADDS     R5,R0,R4, LSL #+4
//  163 		scheduleMsg(logging, "  %s: %d parameters", current->token, getParameterCount(current->parameterType));
        LDRB     R0,[R5, #+4]
          CFI FunCall _Z17getParameterCount13action_type_e
        BL       _Z17getParameterCount13action_type_e
        MOVS     R3,R0
        LDR      R2,[R5, #+0]
        LDR.W    R1,??DataTable9_6
        LDR.W    R0,??DataTable9_5
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  164 	}
        ADDS     R4,R4,#+1
        B.N      ??helpCommand_2
//  165 #endif
//  166 	scheduleMsg(logging, "For more visit http://rusefi.com/wiki/index.php?title=Manual:Software:dev_console_commands");
??helpCommand_3:
        LDR.W    R1,??DataTable9_7
        LDR.W    R0,??DataTable9_5
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  167 }
??helpCommand_1:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock18
//  168 
//  169 /**
//  170  * @brief This is just a test function
//  171  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock19 Using cfiCommon0
          CFI Function _Z4echoi
        THUMB
//  172 static void echo(int value) {
_Z4echoi:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  173 	print("got value: %d\r\n", value);
        MOVS     R1,R4
        LDR.W    R0,??DataTable9_8
          CFI FunCall print
        BL       print
//  174 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock19
//  175 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock20 Using cfiCommon0
          CFI Function unquote
        THUMB
//  176 char *unquote(char *line) {
unquote:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  177 	if (line[0] == '"') {
        LDRB     R0,[R4, #+0]
        CMP      R0,#+34
        BNE.N    ??unquote_0
//  178 		int len = strlen(line);
        MOVS     R0,R4
          CFI FunCall strlen
        BL       strlen
        MOVS     R1,R0
//  179 		if (line[len - 1] == '"') {
        ADDS     R0,R1,R4
        LDRB     R0,[R0, #-1]
        CMP      R0,#+34
        BNE.N    ??unquote_0
//  180 			line[len - 1] = 0;
        ADDS     R0,R1,R4
        MOVS     R2,#+0
        STRB     R2,[R0, #-1]
//  181 			return line + 1;
        ADDS     R0,R4,#+1
        B.N      ??unquote_1
//  182 		}
//  183 	}
//  184 	return line;
??unquote_0:
        MOVS     R0,R4
??unquote_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock20
//  185 }
//  186 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock21 Using cfiCommon0
          CFI Function findEndOfToken
        THUMB
//  187 int findEndOfToken(const char *line) {
findEndOfToken:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  188 	if (line[0] == '"') {
        LDRB     R0,[R4, #+0]
        CMP      R0,#+34
        BNE.N    ??findEndOfToken_0
//  189 		/**
//  190 		 * Looks like this is a quoted token
//  191 		 */
//  192 		int v = indexOf(line + 1, '"');
        MOVS     R1,#+34
        ADDS     R0,R4,#+1
          CFI FunCall indexOf
        BL       indexOf
        MOVS     R1,R0
//  193 		if (v == -1) {
        CMN      R1,#+1
        BNE.N    ??findEndOfToken_1
//  194 			/**
//  195 			 * Matching closing quote not found
//  196 			 */
//  197 			return -1;
        MOVS     R0,#-1
        B.N      ??findEndOfToken_2
//  198 		}
//  199 		/**
//  200 		 * Skipping first quote and the symbol after closing quote
//  201 		 */
//  202 		return v + 2;
??findEndOfToken_1:
        ADDS     R0,R1,#+2
        B.N      ??findEndOfToken_2
//  203 	}
//  204 	return indexOf(line, ' ');
??findEndOfToken_0:
        MOVS     R1,#+32
        MOVS     R0,R4
          CFI FunCall indexOf
        BL       indexOf
??findEndOfToken_2:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock21
//  205 }
//  206 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock22 Using cfiCommon0
          CFI Function _Z25handleActionWithParameterP13TokenCallbackPc
        THUMB
//  207 void handleActionWithParameter(TokenCallback *current, char *parameter) {
_Z25handleActionWithParameterP13TokenCallbackPc:
        PUSH     {R3-R11,LR}
          CFI R14 Frame(CFA, -4)
          CFI R11 Frame(CFA, -8)
          CFI R10 Frame(CFA, -12)
          CFI R9 Frame(CFA, -16)
          CFI R8 Frame(CFA, -20)
          CFI R7 Frame(CFA, -24)
          CFI R6 Frame(CFA, -28)
          CFI R5 Frame(CFA, -32)
          CFI R4 Frame(CFA, -36)
          CFI CFA R13+40
        VPUSH    {D8}
          CFI D8 Frame(CFA, -48)
          CFI CFA R13+48
        SUB      SP,SP,#+8
          CFI CFA R13+56
        MOVS     R4,R0
        MOV      R11,R1
//  208 	if (current->parameterType == STRING_PARAMETER) {
        LDRB     R0,[R4, #+4]
        CMP      R0,#+6
        BNE.N    ??handleActionWithParameter_0
//  209 		VoidCharPtr callbackS = (VoidCharPtr) current->callback;
        LDR      R5,[R4, #+8]
//  210 		(*callbackS)(parameter);
        MOV      R0,R11
          CFI FunCall
        BLX      R5
//  211 		return;
        B.N      ??handleActionWithParameter_1
//  212 	}
//  213 
//  214 	if (current->parameterType == STRING_PARAMETER_P) {
??handleActionWithParameter_0:
        LDRB     R0,[R4, #+4]
        CMP      R0,#+7
        BNE.N    ??handleActionWithParameter_2
//  215 		VoidCharPtrVoidPtr callbackS = (VoidCharPtrVoidPtr) current->callback;
        LDR      R5,[R4, #+8]
//  216 		(*callbackS)(parameter, current->param);
        LDR      R1,[R4, #+12]
        MOV      R0,R11
          CFI FunCall
        BLX      R5
//  217 		return;
        B.N      ??handleActionWithParameter_1
//  218 	}
//  219 
//  220 	// todo: refactor this hell!
//  221 	if (current->parameterType == STRING2_PARAMETER || current->parameterType == STRING2_PARAMETER_P) {
??handleActionWithParameter_2:
        LDRB     R0,[R4, #+4]
        CMP      R0,#+8
        BEQ.N    ??handleActionWithParameter_3
        LDRB     R0,[R4, #+4]
        CMP      R0,#+9
        BNE.N    ??handleActionWithParameter_4
//  222 		int spaceIndex = findEndOfToken(parameter);
??handleActionWithParameter_3:
        MOV      R0,R11
          CFI FunCall findEndOfToken
        BL       findEndOfToken
        MOVS     R5,R0
//  223 		if (spaceIndex == -1) {
        CMN      R5,#+1
        BEQ.W    ??handleActionWithParameter_1
//  224 			return;
//  225 		}
//  226 		parameter[spaceIndex] = 0;
??handleActionWithParameter_5:
        MOVS     R0,#+0
        STRB     R0,[R5, R11]
//  227 		char * param0 = parameter;
        MOV      R6,R11
//  228 
//  229 		parameter += spaceIndex + 1;
        ADDS     R0,R5,R11
        ADDS     R11,R0,#+1
//  230 		char * param1 = parameter;
        MOV      R7,R11
//  231 
//  232 		if (current->parameterType == STRING2_PARAMETER) {
        LDRB     R0,[R4, #+4]
        CMP      R0,#+8
        BNE.N    ??handleActionWithParameter_6
//  233 			VoidCharPtrCharPtr callbackS = (VoidCharPtrCharPtr) current->callback;
        LDR      R8,[R4, #+8]
//  234 			(*callbackS)(param0, param1);
        MOVS     R1,R7
        MOVS     R0,R6
          CFI FunCall
        BLX      R8
        B.N      ??handleActionWithParameter_7
//  235 		} else {
//  236 			VoidCharPtrCharPtrVoidPtr callbackS = (VoidCharPtrCharPtrVoidPtr) current->callback;
??handleActionWithParameter_6:
        LDR      R8,[R4, #+8]
//  237 			(*callbackS)(param0, param1, current->param);
        LDR      R2,[R4, #+12]
        MOVS     R1,R7
        MOVS     R0,R6
          CFI FunCall
        BLX      R8
//  238 		}
//  239 		return;
??handleActionWithParameter_7:
        B.N      ??handleActionWithParameter_1
//  240 	}
//  241 
//  242 	if (current->parameterType == STRING3_PARAMETER) {
??handleActionWithParameter_4:
        LDRB     R0,[R4, #+4]
        CMP      R0,#+10
        BNE.N    ??handleActionWithParameter_8
//  243 		int spaceIndex = findEndOfToken(parameter);
        MOV      R0,R11
          CFI FunCall findEndOfToken
        BL       findEndOfToken
        MOVS     R5,R0
//  244 		if (spaceIndex == -1) {
        CMN      R5,#+1
        BEQ.W    ??handleActionWithParameter_1
//  245 			return;
//  246 		}
//  247 		parameter[spaceIndex] = 0;
??handleActionWithParameter_9:
        MOVS     R0,#+0
        STRB     R0,[R5, R11]
//  248 		char * param0 = parameter;
        MOV      R6,R11
//  249 
//  250 		parameter += spaceIndex + 1;
        ADDS     R0,R5,R11
        ADDS     R11,R0,#+1
//  251 		spaceIndex = findEndOfToken(parameter);
        MOV      R0,R11
          CFI FunCall findEndOfToken
        BL       findEndOfToken
        MOVS     R5,R0
//  252 		if (spaceIndex == -1)
        CMN      R5,#+1
        BEQ.W    ??handleActionWithParameter_1
//  253 			return;
//  254 		parameter[spaceIndex] = 0;
??handleActionWithParameter_10:
        MOVS     R0,#+0
        STRB     R0,[R5, R11]
//  255 		char * param1 = parameter;
        MOV      R7,R11
//  256 		parameter += spaceIndex + 1;
        ADDS     R0,R5,R11
        ADDS     R11,R0,#+1
//  257 		char * param2 = parameter;
//  258 
//  259 		VoidCharPtrCharPtrCharPtr callbackS = (VoidCharPtrCharPtrCharPtr) current->callback;
        LDR      R8,[R4, #+8]
//  260 		(*callbackS)(param0, param1, param2);
        MOV      R2,R11
        MOVS     R1,R7
        MOVS     R0,R6
          CFI FunCall
        BLX      R8
//  261 		return;
        B.N      ??handleActionWithParameter_1
//  262 
//  263 	}
//  264 
//  265 	// todo: refactor this hell!
//  266 	if (current->parameterType == STRING5_PARAMETER) {
??handleActionWithParameter_8:
        LDRB     R0,[R4, #+4]
        CMP      R0,#+11
        BNE.N    ??handleActionWithParameter_11
//  267 		int spaceIndex = findEndOfToken(parameter);
        MOV      R0,R11
          CFI FunCall findEndOfToken
        BL       findEndOfToken
        MOVS     R5,R0
//  268 		if (spaceIndex == -1) {
        CMN      R5,#+1
        BEQ.W    ??handleActionWithParameter_1
//  269 			return;
//  270 		}
//  271 		parameter[spaceIndex] = 0;
??handleActionWithParameter_12:
        MOVS     R0,#+0
        STRB     R0,[R5, R11]
//  272 		char * param0 = parameter;
        MOV      R6,R11
//  273 
//  274 		parameter += spaceIndex + 1;
        ADDS     R0,R5,R11
        ADDS     R11,R0,#+1
//  275 		spaceIndex = findEndOfToken(parameter);
        MOV      R0,R11
          CFI FunCall findEndOfToken
        BL       findEndOfToken
        MOVS     R5,R0
//  276 		if (spaceIndex == -1)
        CMN      R5,#+1
        BEQ.W    ??handleActionWithParameter_1
//  277 			return;
//  278 		parameter[spaceIndex] = 0;
??handleActionWithParameter_13:
        MOVS     R0,#+0
        STRB     R0,[R5, R11]
//  279 		char * param1 = parameter;
        MOV      R7,R11
//  280 
//  281 		parameter += spaceIndex + 1;
        ADDS     R0,R5,R11
        ADDS     R11,R0,#+1
//  282 		spaceIndex = findEndOfToken(parameter);
        MOV      R0,R11
          CFI FunCall findEndOfToken
        BL       findEndOfToken
        MOVS     R5,R0
//  283 		if (spaceIndex == -1)
        CMN      R5,#+1
        BEQ.W    ??handleActionWithParameter_1
//  284 			return;
//  285 		parameter[spaceIndex] = 0;
??handleActionWithParameter_14:
        MOVS     R0,#+0
        STRB     R0,[R5, R11]
//  286 		char * param2 = parameter;
        MOV      R8,R11
//  287 
//  288 		parameter += spaceIndex + 1;
        ADDS     R0,R5,R11
        ADDS     R11,R0,#+1
//  289 		spaceIndex = findEndOfToken(parameter);
        MOV      R0,R11
          CFI FunCall findEndOfToken
        BL       findEndOfToken
        MOVS     R5,R0
//  290 		if (spaceIndex == -1)
        CMN      R5,#+1
        BEQ.W    ??handleActionWithParameter_1
//  291 			return;
//  292 		parameter[spaceIndex] = 0;
??handleActionWithParameter_15:
        MOVS     R0,#+0
        STRB     R0,[R5, R11]
//  293 		char * param3 = parameter;
        MOV      R9,R11
//  294 
//  295 		parameter += spaceIndex + 1;
        ADDS     R0,R5,R11
        ADDS     R11,R0,#+1
//  296 		char * param4 = parameter;
//  297 
//  298 		VoidCharPtrCharPtrCharPtrCharPtrCharPtr callbackS = (VoidCharPtrCharPtrCharPtrCharPtrCharPtr) current->callback;
        LDR      R10,[R4, #+8]
//  299 		(*callbackS)(param0, param1, param2, param3, param4);
        STR      R11,[SP, #+0]
        MOV      R3,R9
        MOV      R2,R8
        MOVS     R1,R7
        MOVS     R0,R6
          CFI FunCall
        BLX      R10
//  300 		return;
        B.N      ??handleActionWithParameter_1
//  301 
//  302 	}
//  303 
//  304 	if (current->parameterType == TWO_INTS_PARAMETER) {
??handleActionWithParameter_11:
        LDRB     R0,[R4, #+4]
        CMP      R0,#+12
        BNE.N    ??handleActionWithParameter_16
//  305 		int spaceIndex = findEndOfToken(parameter);
        MOV      R0,R11
          CFI FunCall findEndOfToken
        BL       findEndOfToken
        MOVS     R5,R0
//  306 		if (spaceIndex == -1)
        CMN      R5,#+1
        BEQ.W    ??handleActionWithParameter_1
//  307 			return;
//  308 		parameter[spaceIndex] = 0;
??handleActionWithParameter_17:
        MOVS     R0,#+0
        STRB     R0,[R5, R11]
//  309 		int value1 = atoi(parameter);
        MOV      R0,R11
          CFI FunCall atoi
        BL       atoi
        MOVS     R6,R0
//  310 		if (absI(value1) == ERROR_CODE) {
        MOVS     R0,R6
          CFI FunCall absI
        BL       absI
        LDR.N    R1,??DataTable9_9  ;; 0x128ce430
        CMP      R0,R1
        BNE.N    ??handleActionWithParameter_18
//  311 #if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
//  312 			scheduleMsg(logging, "not an integer [%s]", parameter);
        MOV      R2,R11
        LDR.N    R1,??DataTable9_10
        LDR.N    R0,??DataTable9_5
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  313 #endif
//  314 			return;
        B.N      ??handleActionWithParameter_1
//  315 		}
//  316 		parameter += spaceIndex + 1;
??handleActionWithParameter_18:
        ADDS     R0,R5,R11
        ADDS     R11,R0,#+1
//  317 		int value2 = atoi(parameter);
        MOV      R0,R11
          CFI FunCall atoi
        BL       atoi
        MOVS     R7,R0
//  318 		if (absI(value2) == ERROR_CODE) {
        MOVS     R0,R7
          CFI FunCall absI
        BL       absI
        LDR.N    R1,??DataTable9_9  ;; 0x128ce430
        CMP      R0,R1
        BNE.N    ??handleActionWithParameter_19
//  319 #if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
//  320 			scheduleMsg(logging, "not an integer [%s]", parameter);
        MOV      R2,R11
        LDR.N    R1,??DataTable9_10
        LDR.N    R0,??DataTable9_5
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  321 #endif
//  322 			return;
        B.N      ??handleActionWithParameter_1
//  323 		}
//  324 		VoidIntInt callbackS = (VoidIntInt) current->callback;
??handleActionWithParameter_19:
        LDR      R8,[R4, #+8]
//  325 		(*callbackS)(value1, value2);
        MOVS     R1,R7
        MOVS     R0,R6
          CFI FunCall
        BLX      R8
//  326 		return;
        B.N      ??handleActionWithParameter_1
//  327 	}
//  328 
//  329 	if (current->parameterType == FLOAT_PARAMETER) {
??handleActionWithParameter_16:
        LDRB     R0,[R4, #+4]
        CMP      R0,#+4
        BNE.N    ??handleActionWithParameter_20
//  330 		float value = atoff(parameter);
        MOV      R0,R11
          CFI FunCall atoff
        BL       atoff
        VMOV.F32 S16,S0
//  331 		VoidFloat callbackF = (VoidFloat) current->callback;
        LDR      R5,[R4, #+8]
//  332 
//  333 		// invoke callback function by reference
//  334 		(*callbackF)(value);
        VMOV.F32 S0,S16
          CFI FunCall
        BLX      R5
//  335 		return;
        B.N      ??handleActionWithParameter_1
//  336 	}
//  337 
//  338 	if (current->parameterType == FLOAT_FLOAT_PARAMETER || current->parameterType == FLOAT_FLOAT_PARAMETER_P) {
??handleActionWithParameter_20:
        LDRB     R0,[R4, #+4]
        CMP      R0,#+14
        BEQ.N    ??handleActionWithParameter_21
        LDRB     R0,[R4, #+4]
        CMP      R0,#+15
        BNE.N    ??handleActionWithParameter_22
//  339 		int spaceIndex = findEndOfToken(parameter);
??handleActionWithParameter_21:
        MOV      R0,R11
          CFI FunCall findEndOfToken
        BL       findEndOfToken
        MOVS     R5,R0
//  340 		if (spaceIndex == -1)
        CMN      R5,#+1
        BEQ.N    ??handleActionWithParameter_1
//  341 			return;
//  342 		parameter[spaceIndex] = 0;
??handleActionWithParameter_23:
        MOVS     R0,#+0
        STRB     R0,[R5, R11]
//  343 		float value1 = atoff(parameter);
        MOV      R0,R11
          CFI FunCall atoff
        BL       atoff
        VMOV.F32 S16,S0
//  344 		parameter += spaceIndex + 1;
        ADDS     R0,R5,R11
        ADDS     R11,R0,#+1
//  345 		float value2 = atoff(parameter);
        MOV      R0,R11
          CFI FunCall atoff
        BL       atoff
        VMOV.F32 S17,S0
//  346 		if (current->parameterType == FLOAT_FLOAT_PARAMETER) {
        LDRB     R0,[R4, #+4]
        CMP      R0,#+14
        BNE.N    ??handleActionWithParameter_24
//  347 			VoidFloatFloat callbackS = (VoidFloatFloat) current->callback;
        LDR      R6,[R4, #+8]
//  348 			(*callbackS)(value1, value2);
        VMOV.F32 S1,S17
        VMOV.F32 S0,S16
          CFI FunCall
        BLX      R6
        B.N      ??handleActionWithParameter_25
//  349 		} else {
//  350 			VoidFloatFloatVoidPtr callbackS = (VoidFloatFloatVoidPtr) current->callback;
??handleActionWithParameter_24:
        LDR      R6,[R4, #+8]
//  351 			(*callbackS)(value1, value2, current->param);
        LDR      R0,[R4, #+12]
        VMOV.F32 S1,S17
        VMOV.F32 S0,S16
          CFI FunCall
        BLX      R6
//  352 		}
//  353 		return;
??handleActionWithParameter_25:
        B.N      ??handleActionWithParameter_1
//  354 	}
//  355 
//  356 	int value = atoi(parameter);
??handleActionWithParameter_22:
        MOV      R0,R11
          CFI FunCall atoi
        BL       atoi
        MOVS     R5,R0
//  357 	if (absI(value) == ERROR_CODE) {
        MOVS     R0,R5
          CFI FunCall absI
        BL       absI
        LDR.N    R1,??DataTable9_9  ;; 0x128ce430
        CMP      R0,R1
        BNE.N    ??handleActionWithParameter_26
//  358 		print("invalid integer [%s]\r\n", parameter);
        MOV      R1,R11
        LDR.N    R0,??DataTable9_11
          CFI FunCall print
        BL       print
//  359 		return;
        B.N      ??handleActionWithParameter_1
//  360 	}
//  361 
//  362 	if (current->parameterType == ONE_PARAMETER_P) {
??handleActionWithParameter_26:
        LDRB     R0,[R4, #+4]
        CMP      R0,#+3
        BNE.N    ??handleActionWithParameter_27
//  363 		VoidIntVoidPtr callback1 = (VoidIntVoidPtr) current->callback;
        LDR      R6,[R4, #+8]
//  364 		// invoke callback function by reference
//  365 		(*callback1)(value, current->param);
        LDR      R1,[R4, #+12]
        MOVS     R0,R5
          CFI FunCall
        BLX      R6
        B.N      ??handleActionWithParameter_28
//  366 
//  367 	} else {
//  368 		VoidInt callback1 = (VoidInt) current->callback;
??handleActionWithParameter_27:
        LDR      R6,[R4, #+8]
//  369 		// invoke callback function by reference
//  370 		(*callback1)(value);
        MOVS     R0,R5
          CFI FunCall
        BLX      R6
//  371 	}
//  372 
//  373 }
??handleActionWithParameter_28:
??handleActionWithParameter_1:
        ADD      SP,SP,#+8
          CFI CFA R13+48
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+40
        POP      {R0,R4-R11,PC}   ;; return
          CFI EndBlock cfiBlock22
//  374 
//  375 /**
//  376  * @return Number of space-separated tokens in the string
//  377  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock23 Using cfiCommon0
          CFI Function tokenLength
          CFI NoCalls
        THUMB
//  378 int tokenLength(const char *msgp) {
tokenLength:
        MOVS     R1,R0
//  379 	int result = 0;
        MOVS     R0,#+0
//  380 	while (*msgp) {
??tokenLength_0:
        LDRB     R2,[R1, #+0]
        CMP      R2,#+0
        BEQ.N    ??tokenLength_1
//  381 		char ch = *msgp++;
        LDRB     R2,[R1, #+0]
        ADDS     R1,R1,#+1
//  382 		if (ch == ' ') {
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        CMP      R2,#+32
        BEQ.N    ??tokenLength_1
//  383 			break;
//  384 		}
//  385 		result++;
??tokenLength_2:
        ADDS     R0,R0,#+1
        B.N      ??tokenLength_0
//  386 	}
//  387 	return result;
??tokenLength_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock23
//  388 }
//  389 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock24 Using cfiCommon0
          CFI Function initConsoleLogic
        THUMB
//  390 void initConsoleLogic(Logging *sharedLogger) {
initConsoleLogic:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  391 	logging = sharedLogger;
        LDR.N    R0,??DataTable9_5
        STR      R4,[R0, #+0]
//  392 //	resetConsoleActions();
//  393 	addConsoleAction("help", helpCommand);
        LDR.N    R1,??DataTable9_12
        LDR.N    R0,??DataTable9_13
          CFI FunCall addConsoleAction
        BL       addConsoleAction
//  394 	addConsoleActionI("echo", echo);
        LDR.N    R1,??DataTable9_14
        LDR.N    R0,??DataTable9_15
          CFI FunCall addConsoleActionI
        BL       addConsoleActionI
//  395 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock24
//  396 
//  397 /**
//  398  * @return NULL if input line validation failed, reference to line payload if validation succeeded.
//  399  * @see sendOutConfirmation() for command confirmation processing.
//  400  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock25 Using cfiCommon0
          CFI Function validateSecureLine
        THUMB
//  401 char *validateSecureLine(char *line) {
validateSecureLine:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
//  402 	if (line == NULL)
        CMP      R4,#+0
        BNE.N    ??validateSecureLine_0
//  403 		return NULL;
        MOVS     R0,#+0
        B.N      ??validateSecureLine_1
//  404 	if (strncmp(SECURE_LINE_PREFIX, line, SECURE_LINE_PREFIX_LENGTH) == 0) {
??validateSecureLine_0:
        MOVS     R2,#+4
        MOVS     R1,R4
        LDR.N    R0,??DataTable9_16
          CFI FunCall strncmp
        BL       strncmp
        CMP      R0,#+0
        BNE.N    ??validateSecureLine_2
//  405 		// COM protocol looses bytes, this is a super-naive error detection
//  406 
//  407 //		print("Got secure mode request header [%s]\r\n", line);
//  408 		line += SECURE_LINE_PREFIX_LENGTH;
        ADDS     R4,R4,#+4
//  409 //		print("Got secure mode request command [%s]\r\n", line);
//  410 
//  411 		char *divider = line;
        MOVS     R5,R4
//  412 		while (*divider != '!') {
??validateSecureLine_3:
        LDRB     R0,[R5, #+0]
        CMP      R0,#+33
        BEQ.N    ??validateSecureLine_4
//  413 			if (*divider == '\0') {
        LDRB     R0,[R5, #+0]
        CMP      R0,#+0
        BNE.N    ??validateSecureLine_5
//  414 				print("Divider not found [%s]\r\n", line);
        MOVS     R1,R4
        LDR.N    R0,??DataTable9_17
          CFI FunCall print
        BL       print
//  415 				return NULL;
        MOVS     R0,#+0
        B.N      ??validateSecureLine_1
//  416 			}
//  417 			divider++;
??validateSecureLine_5:
        ADDS     R5,R5,#+1
        B.N      ??validateSecureLine_3
//  418 		}
//  419 		*divider++ = 0; // replacing divider symbol with zero
??validateSecureLine_4:
        MOVS     R0,#+0
        STRB     R0,[R5, #+0]
        ADDS     R5,R5,#+1
//  420 		int expectedLength = atoi(line);
        MOVS     R0,R4
          CFI FunCall atoi
        BL       atoi
        MOVS     R6,R0
//  421 		line = divider;
        MOVS     R4,R5
//  422 		int actualLength = strlen(line);
        MOVS     R0,R4
          CFI FunCall strlen
        BL       strlen
        MOVS     R7,R0
//  423 		if (expectedLength != actualLength) {
        CMP      R6,R7
        BEQ.N    ??validateSecureLine_2
//  424 			print("Error detected: expected %d but got %d in [%s]\r\n", expectedLength, actualLength, line);
        MOVS     R3,R4
        MOVS     R2,R7
        MOVS     R1,R6
        LDR.N    R0,??DataTable9_18
          CFI FunCall print
        BL       print
//  425 			return NULL;
        MOVS     R0,#+0
        B.N      ??validateSecureLine_1
//  426 		}
//  427 	}
//  428 	return line;
??validateSecureLine_2:
        MOVS     R0,R4
??validateSecureLine_1:
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock25
//  429 }
//  430 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  431 static char confirmation[200];
confirmation:
        DS8 200

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  432 static char handleBuffer[200];
handleBuffer:
        DS8 200
//  433 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock26 Using cfiCommon0
          CFI Function _Z25handleConsoleLineInternalPKci
        THUMB
//  434 static bool handleConsoleLineInternal(const char *commandLine, int lineLength) {
_Z25handleConsoleLineInternalPKci:
        PUSH     {R4-R10,LR}
          CFI R14 Frame(CFA, -4)
          CFI R10 Frame(CFA, -8)
          CFI R9 Frame(CFA, -12)
          CFI R8 Frame(CFA, -16)
          CFI R7 Frame(CFA, -20)
          CFI R6 Frame(CFA, -24)
          CFI R5 Frame(CFA, -28)
          CFI R4 Frame(CFA, -32)
          CFI CFA R13+32
        MOVS     R4,R0
        MOVS     R5,R1
//  435 	strncpy(handleBuffer, commandLine, sizeof(handleBuffer) - 1);
        MOVS     R2,#+199
        MOVS     R1,R4
        LDR.N    R0,??DataTable9_19
          CFI FunCall strncpy
        BL       strncpy
//  436 	handleBuffer[sizeof(handleBuffer) - 1] = 0; // we want this to be null-terminated for sure
        LDR.N    R0,??DataTable9_19
        MOVS     R1,#+0
        STRB     R1,[R0, #+199]
//  437 	char *line = handleBuffer;
        LDR.N    R6,??DataTable9_19
//  438 	int firstTokenLength = tokenLength(line);
        MOVS     R0,R6
          CFI FunCall tokenLength
        BL       tokenLength
        MOVS     R7,R0
//  439 
//  440 //	print("processing [%s] with %d actions\r\n", line, consoleActionCount);
//  441 
//  442 	if (firstTokenLength == lineLength) {
        CMP      R7,R5
        BNE.N    ??handleConsoleLineInternal_0
//  443 		// no-param actions are processed here
//  444 		for (int i = 0; i < consoleActionCount; i++) {
        MOVS     R8,#+0
??handleConsoleLineInternal_1:
        LDR.N    R0,??DataTable9_1
        LDR      R0,[R0, #+0]
        CMP      R8,R0
        BGE.N    ??handleConsoleLineInternal_2
//  445 			TokenCallback *current = &consoleActions[i];
        LDR.N    R0,??DataTable9_3
        ADDS     R9,R0,R8, LSL #+4
//  446 			if (strEqual(line, current->token)) {
        LDR      R1,[R9, #+0]
        MOVS     R0,R6
          CFI FunCall strEqual
        BL       strEqual
        CMP      R0,#+0
        BEQ.N    ??handleConsoleLineInternal_3
//  447 				if (current->parameterType == NO_PARAMETER) {
        LDRB     R0,[R9, #+4]
        CMP      R0,#+0
        BNE.N    ??handleConsoleLineInternal_4
//  448 					(*current->callback)();
        LDR      R0,[R9, #+8]
          CFI FunCall
        BLX      R0
        B.N      ??handleConsoleLineInternal_5
//  449 				} else if (current->parameterType == NO_PARAMETER_P) {
??handleConsoleLineInternal_4:
        LDRB     R0,[R9, #+4]
        CMP      R0,#+1
        BNE.N    ??handleConsoleLineInternal_5
//  450 					VoidPtr cb = (VoidPtr) current->callback;
        LDR      R10,[R9, #+8]
//  451 					(*cb)(current->param);
        LDR      R0,[R9, #+12]
          CFI FunCall
        BLX      R10
//  452 				}
//  453 				return true;
??handleConsoleLineInternal_5:
        MOVS     R0,#+1
        B.N      ??handleConsoleLineInternal_6
//  454 			}
//  455 		}
??handleConsoleLineInternal_3:
        ADDS     R8,R8,#+1
        B.N      ??handleConsoleLineInternal_1
//  456 	} else {
//  457 		char *ptr = line + firstTokenLength;
??handleConsoleLineInternal_0:
        ADDS     R8,R7,R6
//  458 		ptr[0] = 0; // change space into line end
        MOVS     R0,#+0
        STRB     R0,[R8, #+0]
//  459 		ptr++; // start from next symbol
        ADDS     R8,R8,#+1
//  460 
//  461 		for (int i = 0; i < consoleActionCount; i++) {
        MOVS     R9,#+0
??handleConsoleLineInternal_7:
        LDR.N    R0,??DataTable9_1
        LDR      R0,[R0, #+0]
        CMP      R9,R0
        BGE.N    ??handleConsoleLineInternal_2
//  462 			TokenCallback *current = &consoleActions[i];
        LDR.N    R0,??DataTable9_3
        ADDS     R10,R0,R9, LSL #+4
//  463 			if (strEqual(line, current->token)) {
        LDR      R1,[R10, #+0]
        MOVS     R0,R6
          CFI FunCall strEqual
        BL       strEqual
        CMP      R0,#+0
        BEQ.N    ??handleConsoleLineInternal_8
//  464 				handleActionWithParameter(current, ptr);
        MOV      R1,R8
        MOV      R0,R10
          CFI FunCall _Z25handleActionWithParameterP13TokenCallbackPc
        BL       _Z25handleActionWithParameterP13TokenCallbackPc
//  465 				return true;
        MOVS     R0,#+1
        B.N      ??handleConsoleLineInternal_6
//  466 			}
//  467 		}
??handleConsoleLineInternal_8:
        ADDS     R9,R9,#+1
        B.N      ??handleConsoleLineInternal_7
//  468 	}
//  469 	return false;
??handleConsoleLineInternal_2:
        MOVS     R0,#+0
??handleConsoleLineInternal_6:
        POP      {R4-R10,PC}      ;; return
          CFI EndBlock cfiBlock26
//  470 }
//  471 
//  472 #if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock27 Using cfiCommon0
          CFI Function _Z19sendOutConfirmationPKci
        THUMB
//  473 static void sendOutConfirmation(const char *command, int length) {
_Z19sendOutConfirmationPKci:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  474 	scheduleMsg(logging, "%s%d", command, length);
        MOVS     R3,R5
        MOVS     R2,R4
        LDR.N    R1,??DataTable9_20
        LDR.N    R0,??DataTable9_5
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  475 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock27
//  476 #endif
//  477 
//  478 /**
//  479  * @brief This function takes care of one command line once we have it
//  480  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock28 Using cfiCommon0
          CFI Function handleConsoleLine
        THUMB
//  481 void handleConsoleLine(char *line) {
handleConsoleLine:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//  482 	line = validateSecureLine(line);
        MOVS     R0,R4
          CFI FunCall validateSecureLine
        BL       validateSecureLine
        MOVS     R4,R0
//  483 	if (line == NULL)
        CMP      R4,#+0
        BEQ.N    ??handleConsoleLine_0
//  484 		return; // error detected
//  485 
//  486 	int lineLength = strlen(line);
??handleConsoleLine_1:
        MOVS     R0,R4
          CFI FunCall strlen
        BL       strlen
        MOVS     R5,R0
//  487 	if (lineLength > 100) {
        CMP      R5,#+101
        BLT.N    ??handleConsoleLine_2
//  488 		// todo: better max size logic
//  489 		// todo: better reaction to excessive line
//  490 		print("Long line?\r\n");
        LDR.N    R0,??DataTable9_21
          CFI FunCall print
        BL       print
//  491 		return;
        B.N      ??handleConsoleLine_0
//  492 	}
//  493 
//  494 	strcpy(confirmation, "confirmation_");
??handleConsoleLine_2:
        LDR.N    R1,??DataTable9_22
        LDR.N    R0,??DataTable9_23
          CFI FunCall strcpy
        BL       strcpy
//  495 	strcat(confirmation, line);
        MOVS     R1,R4
          CFI FunCall strcat
        BL       strcat
//  496 	strcat(confirmation, ":");
        ADR.N    R1,??DataTable9  ;; ":"
        LDR.N    R0,??DataTable9_23
          CFI FunCall strcat
        BL       strcat
//  497 
//  498 #if EFI_PROD_CODE || EFI_SIMULATOR
//  499 	sendOutConfirmation(confirmation, lineLength);
        MOVS     R1,R5
          CFI FunCall _Z19sendOutConfirmationPKci
        BL       _Z19sendOutConfirmationPKci
//  500 #endif
//  501 
//  502 	bool isKnownComman = handleConsoleLineInternal(line, lineLength);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z25handleConsoleLineInternalPKci
        BL       _Z25handleConsoleLineInternalPKci
        MOVS     R6,R0
//  503 
//  504 	if (!isKnownComman) {
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+0
        BNE.N    ??handleConsoleLine_3
//  505 		scheduleMsg(logging, "unknown [%s]", line);
        MOVS     R2,R4
        LDR.N    R1,??DataTable9_24
        LDR.N    R0,??DataTable9_5
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  506 		helpCommand();
          CFI FunCall helpCommand
        BL       helpCommand
//  507 	}
//  508 }
??handleConsoleLine_3:
??handleConsoleLine_0:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock28

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9:
        DC8      ":",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_1:
        DC32     consoleActionCount

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_2:
        DC32     `?<Constant "Too many console actions">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_3:
        DC32     consoleActions

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_4:
        DC32     `?<Constant "%d actions available">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_5:
        DC32     logging

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_6:
        DC32     `?<Constant "  %s: %d parameters">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_7:
        DC32     `?<Constant "For more visit http:/...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_8:
        DC32     `?<Constant "got value: %d\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_9:
        DC32     0x128ce430

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_10:
        DC32     `?<Constant "not an integer [%s]">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_11:
        DC32     `?<Constant "invalid integer [%s]\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_12:
        DC32     helpCommand

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_13:
        DC32     `?<Constant "help">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_14:
        DC32     _Z4echoi

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_15:
        DC32     `?<Constant "echo">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_16:
        DC32     `?<Constant "sec!">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_17:
        DC32     `?<Constant "Divider not found [%s]\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_18:
        DC32     `?<Constant "Error detected: expec...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_19:
        DC32     handleBuffer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_20:
        DC32     `?<Constant "%s%d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_21:
        DC32     `?<Constant "Long line?\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_22:
        DC32     `?<Constant "confirmation_">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_23:
        DC32     confirmation

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_24:
        DC32     `?<Constant "unknown [%s]">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Too many console actions">`:
        DATA
        DC8 "Too many console actions"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "%d actions available">`:
        DATA
        DC8 "%d actions available"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "  %s: %d parameters">`:
        DATA
        DC8 "  %s: %d parameters"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "For more visit http:/...">`:
        DATA
        DC8 46H, 6FH, 72H, 20H, 6DH, 6FH, 72H, 65H
        DC8 20H, 76H, 69H, 73H, 69H, 74H, 20H, 68H
        DC8 74H, 74H, 70H, 3AH, 2FH, 2FH, 72H, 75H
        DC8 73H, 65H, 66H, 69H, 2EH, 63H, 6FH, 6DH
        DC8 2FH, 77H, 69H, 6BH, 69H, 2FH, 69H, 6EH
        DC8 64H, 65H, 78H, 2EH, 70H, 68H, 70H, 3FH
        DC8 74H, 69H, 74H, 6CH, 65H, 3DH, 4DH, 61H
        DC8 6EH, 75H, 61H, 6CH, 3AH, 53H, 6FH, 66H
        DC8 74H, 77H, 61H, 72H, 65H, 3AH, 64H, 65H
        DC8 76H, 5FH, 63H, 6FH, 6EH, 73H, 6FH, 6CH
        DC8 65H, 5FH, 63H, 6FH, 6DH, 6DH, 61H, 6EH
        DC8 64H, 73H, 0
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "got value: %d\\r\\n">`:
        DATA
        DC8 "got value: %d\015\012"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "not an integer [%s]">`:
        DATA
        DC8 "not an integer [%s]"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid integer [%s]\\r\\n">`:
        DATA
        DC8 "invalid integer [%s]\015\012"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "help">`:
        DATA
        DC8 "help"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "echo">`:
        DATA
        DC8 "echo"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "sec!">`:
        DATA
        DC8 "sec!"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Divider not found [%s]\\r\\n">`:
        DATA
        DC8 "Divider not found [%s]\015\012"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Error detected: expec...">`:
        DATA
        DC8 "Error detected: expected %d but got %d in [%s]\015\012"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "%s%d">`:
        DATA
        DC8 "%s%d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Long line?\\r\\n">`:
        DATA
        DC8 "Long line?\015\012"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "confirmation_">`:
        DATA
        DC8 "confirmation_"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 ":"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "unknown [%s]">`:
        DATA
        DC8 "unknown [%s]"
        DC8 0, 0, 0

        END
// 
// 3 544 bytes in section .bss
//   386 bytes in section .rodata
// 1 890 bytes in section .text
// 
// 1 890 bytes of CODE  memory
//   386 bytes of CONST memory
// 3 544 bytes of DATA  memory
//
//Errors: none
//Warnings: none
