///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:55 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\development\tri /
//                       gger_emulator.cpp                                    /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\development\tri /
//                       gger_emulator.cpp -lCN F:\stuff\rusefi_sourceforge\f /
//                       irmware\iar\Debug\List\ -lA                          /
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
//                       trigger_emulator.s                                   /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME trigger_emulator

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z20getConfigurationName13engine_type_e
        EXTERN _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
        EXTERN _Z24initTriggerEmulatorLogicP7LoggingP6Engine
        EXTERN _ZN9OutputPinC1Ev
        EXTERN __aeabi_vec_ctor_nocookie_nodtor
        EXTERN print
        EXTERN triggerSignal

        PUBLIC _Z19initTriggerEmulatorP7LoggingP6Engine
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\development\trigger_emulator.cpp
//    1 /**
//    2  * @file    trigger_emulator.cpp
//    3  * @brief   Position sensor(s) emulation code
//    4  *
//    5  * This file is mostly about initialization, the configuration is
//    6  * provided by the configureShaftPositionEmulatorShape method
//    7  *
//    8  * @date Dec 9, 2012
//    9  * @author Andrey Belomutskiy, (c) 2012-2015
//   10  */
//   11 
//   12 #include "trigger_emulator.h"
//   13 #include "eficonsole.h"
//   14 #include "main_trigger_callback.h"
//   15 #include "datalogging.h"
//   16 #include "engine_configuration.h"
//   17 #if EFI_PROD_CODE
//   18 #include "pwm_generator.h"
//   19 #include "pin_repository.h"
//   20 #endif
//   21 #include "io_pins.h"
//   22 #include "trigger_emulator_algo.h"
//   23 #include "trigger_central.h"
//   24 #include "settings.h"
//   25 
//   26 extern PwmConfig triggerSignal;
//   27 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   28 static OutputPin emulatorOutputs[3];
emulatorOutputs:
        DS8 48

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        MOVS     R3,#+3
        MOVS     R2,#+16
        LDR.N    R1,??DataTable1
        LDR.N    R0,??DataTable1_1
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0
//   29 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z19initTriggerEmulatorP7LoggingP6Engine
        THUMB
//   30 void initTriggerEmulator(Logging *sharedLogger, Engine *engine) {
_Z19initTriggerEmulatorP7LoggingP6Engine:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//   31 
//   32 	engine_configuration_s *engineConfiguration = engine->engineConfiguration;
        LDR      R6,[R5, #+36]
//   33 	board_configuration_s *boardConfiguration = &engineConfiguration->bc;
        ADDS     R7,R6,#+624
//   34 
//   35 #if EFI_EMULATE_POSITION_SENSORS || defined(__DOXYGEN__)
//   36 	print("Emulating %s\r\n", getConfigurationName(engineConfiguration->engineType));
        LDR      R0,[R6, #+0]
          CFI FunCall _Z20getConfigurationName13engine_type_e
        BL       _Z20getConfigurationName13engine_type_e
        MOVS     R1,R0
        LDR.N    R0,??DataTable1_2
          CFI FunCall print
        BL       print
//   37 
//   38 	triggerSignal.outputPins[0] = &emulatorOutputs[0];
        LDR.N    R0,??DataTable1_3
        LDR.N    R1,??DataTable1_1
        STR      R1,[R0, #+0]
//   39 	triggerSignal.outputPins[1] = &emulatorOutputs[1];
        LDR.N    R0,??DataTable1_3
        LDR.N    R1,??DataTable1_4
        STR      R1,[R0, #+4]
//   40 	triggerSignal.outputPins[2] = &emulatorOutputs[2];
        LDR.N    R0,??DataTable1_3
        LDR.N    R1,??DataTable1_5
        STR      R1,[R0, #+8]
//   41 
//   42 #if EFI_PROD_CODE
//   43 	// todo: refactor, make this a loop
//   44 	outputPinRegisterExt2("distributor ch1", triggerSignal.outputPins[0], boardConfiguration->triggerSimulatorPins[0],
//   45 			&boardConfiguration->triggerSimulatorPinModes[0]);
        ADDS     R3,R7,#+364
        LDR      R2,[R7, #+352]
        LDR.N    R0,??DataTable1_3
        LDR      R1,[R0, #+0]
        LDR.N    R0,??DataTable1_6
          CFI FunCall _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
        BL       _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
//   46 
//   47 	outputPinRegisterExt2("distributor ch2", triggerSignal.outputPins[1], boardConfiguration->triggerSimulatorPins[1],
//   48 			&boardConfiguration->triggerSimulatorPinModes[1]);
        ADDS     R3,R7,#+368
        LDR      R2,[R7, #+356]
        LDR.N    R0,??DataTable1_3
        LDR      R1,[R0, #+4]
        LDR.N    R0,??DataTable1_7
          CFI FunCall _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
        BL       _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
//   49 
//   50 	outputPinRegisterExt2("distributor ch3", triggerSignal.outputPins[2], boardConfiguration->triggerSimulatorPins[2],
//   51 			&boardConfiguration->triggerSimulatorPinModes[2]);
        ADDS     R3,R7,#+372
        LDR      R2,[R7, #+360]
        LDR.N    R0,??DataTable1_3
        LDR      R1,[R0, #+8]
        LDR.N    R0,??DataTable1_8
          CFI FunCall _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
        BL       _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
//   52 #endif /* EFI_PROD_CODE */
//   53 
//   54 	initTriggerEmulatorLogic(sharedLogger, engine);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z24initTriggerEmulatorLogicP7LoggingP6Engine
        BL       _Z24initTriggerEmulatorLogicP7LoggingP6Engine
//   55 #else
//   56 	print("No position sensor(s) emulation\r\n");
//   57 #endif /* EFI_EMULATE_POSITION_SENSORS */
//   58 }
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1:
        DC32     _ZN9OutputPinC1Ev

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_1:
        DC32     emulatorOutputs

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_2:
        DC32     `?<Constant "Emulating %s\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_3:
        DC32     triggerSignal

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_4:
        DC32     emulatorOutputs+0x10

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_5:
        DC32     emulatorOutputs+0x20

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_6:
        DC32     `?<Constant "distributor ch1">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_7:
        DC32     `?<Constant "distributor ch2">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_8:
        DC32     `?<Constant "distributor ch3">`

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
`?<Constant "Emulating %s\\r\\n">`:
        DATA
        DC8 "Emulating %s\015\012"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "distributor ch1">`:
        DATA
        DC8 "distributor ch1"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "distributor ch2">`:
        DATA
        DC8 "distributor ch2"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "distributor ch3">`:
        DATA
        DC8 "distributor ch3"

        END
// 
//  48 bytes in section .bss
//   4 bytes in section .init_array
//  64 bytes in section .rodata
// 160 bytes in section .text
// 
// 164 bytes of CODE  memory
//  64 bytes of CONST memory
//  48 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
