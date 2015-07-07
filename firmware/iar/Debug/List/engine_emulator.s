///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:46 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\development\eng /
//                       ine_emulator.cpp                                     /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\development\eng /
//                       ine_emulator.cpp -lCN F:\stuff\rusefi_sourceforge\fi /
//                       rmware\iar\Debug\List\ -lA                           /
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
//                       engine_emulator.s                                    /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME engine_emulator

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z10setFullLogi
        EXTERN _Z18initPotentiometersP7LoggingP21board_configuration_s
        EXTERN _Z19initTriggerEmulatorP7LoggingP6Engine
        EXTERN _Z21setTriggerEmulatorRPMiP6Engine
        EXTERN chThdSleep
        EXTERN hasFirmwareErrorFlag
        EXTERN print

        PUBLIC _Z13startEmulatorv
        PUBLIC _Z18initEngineEmulatorP7LoggingP6Engine
        PUBLIC _Z7emulateP6Engine
        PUBLIC _Z7setDiagi
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\development\engine_emulator.cpp
//    1 /**
//    2  * @file	engine_emulator.cpp
//    3  * @brief	Entry point for all the emulation and analysis code
//    4  *
//    5  * @date Mar 15, 2013
//    6  * @author Andrey Belomutskiy, (c) 2012-2015
//    7  */
//    8 
//    9 #include "main.h"
//   10 #include "engine_emulator.h"
//   11 
//   12 #include "advance_map.h"
//   13 #include "fuel_math.h"
//   14 
//   15 #include "status_loop.h"
//   16 #include "wave_analyzer.h"
//   17 #include "pin_repository.h"
//   18 #include "pwm_generator_logic.h"
//   19 
//   20 #include "poten.h"
//   21 #include "trigger_emulator.h"
//   22 
//   23 extern bool hasFirmwareErrorFlag;
//   24 
//   25 static THD_WORKING_AREA(eeThreadStack, UTILITY_THREAD_STACK_SIZE);
//   26 
//   27 #define DIAG_PORT GPIOD
//   28 #define DIAG_PIN 0
//   29 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z7setDiagi
        THUMB
//   30 void setDiag(int value) {
_Z7setDiagi:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   31 	print("Setting diag: %d\r\n", value);
        MOVS     R1,R4
        LDR.N    R0,??DataTable3
          CFI FunCall print
        BL       print
//   32 	palWritePad(DIAG_PORT, DIAG_PIN, value);
        MVNS     R0,R4
        ANDS     R0,R0,#0x1
        ANDS     R1,R4,#0x1
        ORRS     R0,R1,R0, LSL #+16
        LDR.N    R1,??DataTable3_1  ;; 0x40020c18
        STR      R0,[R1, #+0]
//   33 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0
//   34 
//   35 #define PERIOD 3000
//   36 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z7emulateP6Engine
        THUMB
//   37 void emulate(Engine *engine) {
_Z7emulateP6Engine:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//   38 	print("Emulating...\r\n");
        LDR.N    R0,??DataTable3_2
          CFI FunCall print
        BL       print
//   39 	setDiag(1);
        MOVS     R0,#+1
          CFI FunCall _Z7setDiagi
        BL       _Z7setDiagi
//   40 	chThdSleep(1);
        MOVS     R0,#+1
          CFI FunCall chThdSleep
        BL       chThdSleep
//   41 	setFullLog(1);
        MOVS     R0,#+1
          CFI FunCall _Z10setFullLogi
        BL       _Z10setFullLogi
//   42 
//   43 	for (int i = 400; i <= 1300; i++) {
        MOV      R5,#+400
??emulate_0:
        MOVW     R0,#+1301
        CMP      R5,R0
        BGE.N    ??emulate_1
//   44 		if (i % 50 != 0)
        MOVS     R0,#+50
        SDIV     R1,R5,R0
        MLS      R0,R0,R1,R5
        CMP      R0,#+0
        BNE.N    ??emulate_2
//   45 			continue;
//   46 		setTriggerEmulatorRPM(i, engine);
??emulate_3:
        MOVS     R1,R4
        MOVS     R0,R5
          CFI FunCall _Z21setTriggerEmulatorRPMiP6Engine
        BL       _Z21setTriggerEmulatorRPMiP6Engine
//   47 		chThdSleepMilliseconds(PERIOD);
        MOVW     R0,#+3000
          CFI FunCall chThdSleep
        BL       chThdSleep
//   48 	}
??emulate_2:
        ADDS     R5,R5,#+1
        B.N      ??emulate_0
//   49 
//   50 	setTriggerEmulatorRPM(0, engine);
??emulate_1:
        MOVS     R1,R4
        MOVS     R0,#+0
          CFI FunCall _Z21setTriggerEmulatorRPMiP6Engine
        BL       _Z21setTriggerEmulatorRPMiP6Engine
//   51 
//   52 	setFullLog(0);
        MOVS     R0,#+0
          CFI FunCall _Z10setFullLogi
        BL       _Z10setFullLogi
//   53 	setDiag(0);
        MOVS     R0,#+0
          CFI FunCall _Z7setDiagi
        BL       _Z7setDiagi
//   54 	chThdSleep(1);
        MOVS     R0,#+1
          CFI FunCall chThdSleep
        BL       chThdSleep
//   55 	print("Emulation DONE!\r\n");
        LDR.N    R0,??DataTable3_3
          CFI FunCall print
        BL       print
//   56 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock1
//   57 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   58 static int flag = FALSE;
flag:
        DS8 4
//   59 
//   60 static msg_t eeThread(Engine *engine) {
//   61 	chRegSetThreadName("Engine");
//   62 
//   63 	while (TRUE) {
//   64 		while (!flag)
//   65 			chThdSleepMilliseconds(200);
//   66 		flag = FALSE;
//   67 		emulate(engine);
//   68 	}
//   69 #if defined __GNUC__
//   70 	return (msg_t)NULL;
//   71 #endif
//   72 }
//   73 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z13startEmulatorv
          CFI NoCalls
        THUMB
//   74 void startEmulator(void) {
//   75 	flag = TRUE;
_Z13startEmulatorv:
        LDR.N    R0,??DataTable3_4
        MOVS     R1,#+1
        STR      R1,[R0, #+0]
//   76 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock2
//   77 
//   78 //static void printAdvance(int rpm, int maf100) {
//   79 //	float advance = getAdvance(rpm, maf100 / 100.0);
//   80 //	print("advance for %d rpm %d maf100: %f\r\n", rpm, maf100, advance);
//   81 //}
//   82 
//   83 static void initECUstimulator(Engine *engine) {
//   84 	mySetPadMode("TEN", DIAG_PORT, DIAG_PIN,
//   85 	PAL_MODE_OUTPUT_PUSHPULL);
//   86 
//   87 	addConsoleActionI("diag", setDiag);
//   88 	addConsoleAction("emu", startEmulator);
//   89 //	addConsoleActionII("ad", printAdvance);
//   90 
//   91 	setDiag(1);
//   92 
//   93 	chThdCreateStatic(eeThreadStack, sizeof(eeThreadStack), NORMALPRIO, (tfunc_t) eeThread, engine);
//   94 }
//   95 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z18initEngineEmulatorP7LoggingP6Engine
        THUMB
//   96 void initEngineEmulator(Logging *sharedLogger, Engine *engine) {
_Z18initEngineEmulatorP7LoggingP6Engine:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   97 	if (hasFirmwareError())
        LDR.N    R0,??DataTable3_5
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??initEngineEmulator_0
//   98 		return;
//   99 
//  100 #if EFI_POTENTIOMETER
//  101 	initPotentiometers(sharedLogger, &engine->engineConfiguration->bc);
??initEngineEmulator_1:
        LDR      R0,[R5, #+36]
        ADDS     R1,R0,#+624
        MOVS     R0,R4
          CFI FunCall _Z18initPotentiometersP7LoggingP21board_configuration_s
        BL       _Z18initPotentiometersP7LoggingP21board_configuration_s
//  102 #endif /* EFI_POTENTIOMETER */
//  103 
//  104 	//initECUstimulator();
//  105 	initTriggerEmulator(sharedLogger, engine);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z19initTriggerEmulatorP7LoggingP6Engine
        BL       _Z19initTriggerEmulatorP7LoggingP6Engine
//  106 }
??initEngineEmulator_0:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock3

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3:
        DC32     `?<Constant "Setting diag: %d\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_1:
        DC32     0x40020c18

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_2:
        DC32     `?<Constant "Emulating...\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_3:
        DC32     `?<Constant "Emulation DONE!\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_4:
        DC32     flag

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_5:
        DC32     hasFirmwareErrorFlag

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Setting diag: %d\\r\\n">`:
        DATA
        DC8 "Setting diag: %d\015\012"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Emulating...\\r\\n">`:
        DATA
        DC8 "Emulating...\015\012"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Emulation DONE!\\r\\n">`:
        DATA
        DC8 "Emulation DONE!\015\012"
        DC8 0, 0

        END
// 
//   4 bytes in section .bss
//  56 bytes in section .rodata
// 208 bytes in section .text
// 
// 208 bytes of CODE  memory
//  56 bytes of CONST memory
//   4 bytes of DATA  memory
//
//Errors: none
//Warnings: 2
