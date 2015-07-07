///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:50 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\io_pin /
//                       s.cpp                                                /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\io_pin /
//                       s.cpp -lCN F:\stuff\rusefi_sourceforge\firmware\iar\ /
//                       Debug\List\ -lA F:\stuff\rusefi_sourceforge\firmware /
//                       \iar\Debug\List\ -o F:\stuff\rusefi_sourceforge\firm /
//                       ware\iar\Debug\Obj\ --no_cse --no_unroll             /
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
//                       io_pins.s                                            /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME io_pins

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z16initOutputPinExtPKcP9OutputPinP12GPIO_TypeDefjj
        EXTERN _ZN18LoggingWithStorageC1EPKc
        EXTERN _ZN9OutputPin18setDefaultPinStateEP17pin_output_mode_e
        EXTERN _ZN9OutputPin8setValueEi
        EXTERN _ZN9OutputPinC1Ev
        EXTERN boardConfiguration
        EXTERN checkEnginePin
        EXTERN enginePins
        EXTERN firmwareError

        PUBLIC DEFAULT_OUTPUT
        PUBLIC _Z14initOutputPinsv
        PUBLIC _Z14turnAllPinsOffv
        PUBLIC _Z15initPrimaryPinsv
        PUBLIC _Z17outputPinRegisterPKcP9OutputPinP12GPIO_TypeDefj
        PUBLIC _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
        PUBLIC _Z8getHwPin11brain_pin_e
        PUBLIC _Z9getHwPort11brain_pin_e
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\hw_layer\io_pins.cpp
//    1 /**
//    2  * @file	io_pins.cpp
//    3  * @brief	It could be that the main purpose of this file is the status LED blinking
//    4  *
//    5  * @date Jan 24, 2013
//    6  * @author Andrey Belomutskiy, (c) 2012-2015
//    7  */
//    8 
//    9 #include <board.h>
//   10 #include "main.h"
//   11 #include "io_pins.h"
//   12 #include "efiGpio.h"
//   13 
//   14 #include "pin_repository.h"
//   15 #include "gpio_helper.h"
//   16 #include "status_loop.h"
//   17 #include "engine_configuration.h"
//   18 #include "console_io.h"
//   19 
//   20 #if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
//   21 #include "main_trigger_callback.h"
//   22 #endif /* EFI_ENGINE_CONTROL */
//   23 
//   24 extern board_configuration_s *boardConfiguration;
//   25 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   26 static LoggingWithStorage logger("io_pins");
        LDR.N    R1,??DataTable7
        LDR.N    R0,??DataTable7_1
          CFI FunCall _ZN18LoggingWithStorageC1EPKc
        BL       _ZN18LoggingWithStorageC1EPKc
//   27 
//   28 static OutputPin sdCsPin;
        LDR.N    R0,??DataTable7_2
          CFI FunCall _ZN9OutputPinC1Ev
        BL       _ZN9OutputPinC1Ev
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
logger:
        DS8 260

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
sdCsPin:
        DS8 16
//   29 
//   30 extern engine_pins_s enginePins;
//   31 
//   32 #if defined(STM32F4XX)

        SECTION `.data`:DATA:REORDER:NOROOT(2)
//   33 static GPIO_TypeDef *PORTS[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH };
PORTS:
        DATA
        DC32 40020000H, 40020400H, 40020800H, 40020C00H, 40021000H, 40021400H
        DC32 40021800H, 40021C00H
//   34 #else
//   35 static GPIO_TypeDef *PORTS[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOF};
//   36 #endif
//   37 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   38 pin_output_mode_e DEFAULT_OUTPUT = OM_DEFAULT;
DEFAULT_OUTPUT:
        DS8 4
//   39 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z20outputPinRegisterExtPKcP9OutputPinP12GPIO_TypeDefjP17pin_output_mode_e
        THUMB
//   40 static void outputPinRegisterExt(const char *msg, OutputPin *output, GPIO_TypeDef *port, uint32_t pin,
//   41 		pin_output_mode_e *outputMode) {
_Z20outputPinRegisterExtPKcP9OutputPinP12GPIO_TypeDefjP17pin_output_mode_e:
        PUSH     {R3-R9,LR}
          CFI R14 Frame(CFA, -4)
          CFI R9 Frame(CFA, -8)
          CFI R8 Frame(CFA, -12)
          CFI R7 Frame(CFA, -16)
          CFI R6 Frame(CFA, -20)
          CFI R5 Frame(CFA, -24)
          CFI R4 Frame(CFA, -28)
          CFI CFA R13+32
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
        LDR      R8,[SP, #+32]
//   42 #if EFI_GPIO
//   43 	if (port == GPIO_NULL) {
        CMP      R6,#+0
        BNE.N    ??outputPinRegisterExt_0
//   44 		// that's for GRIO_NONE
//   45 		output->port = port;
        STR      R6,[R5, #+0]
//   46 		return;
        B.N      ??outputPinRegisterExt_1
//   47 	}
//   48 
//   49 	assertOMode(*outputMode);
??outputPinRegisterExt_0:
        LDR      R0,[R8, #+0]
        CMP      R0,#+4
        BLT.N    ??outputPinRegisterExt_2
        LDR.N    R0,??DataTable7_3
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??outputPinRegisterExt_1
//   50 	iomode_t mode = (*outputMode == OM_DEFAULT || *outputMode == OM_INVERTED) ?
//   51 	PAL_MODE_OUTPUT_PUSHPULL :
//   52 																				PAL_MODE_OUTPUT_OPENDRAIN;
??outputPinRegisterExt_2:
        LDR      R0,[R8, #+0]
        CMP      R0,#+0
        BEQ.N    ??outputPinRegisterExt_3
        LDR      R0,[R8, #+0]
        CMP      R0,#+1
        BNE.N    ??outputPinRegisterExt_4
??outputPinRegisterExt_3:
        MOVS     R9,#+1
        B.N      ??outputPinRegisterExt_5
??outputPinRegisterExt_4:
        MOVS     R9,#+5
//   53 
//   54 	initOutputPinExt(msg, output, port, pin, mode);
??outputPinRegisterExt_5:
        STR      R9,[SP, #+0]
        MOVS     R3,R7
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z16initOutputPinExtPKcP9OutputPinP12GPIO_TypeDefjj
        BL       _Z16initOutputPinExtPKcP9OutputPinP12GPIO_TypeDefjj
//   55 
//   56 	output->setDefaultPinState(outputMode);
        MOV      R1,R8
        MOVS     R0,R5
          CFI FunCall _ZN9OutputPin18setDefaultPinStateEP17pin_output_mode_e
        BL       _ZN9OutputPin18setDefaultPinStateEP17pin_output_mode_e
//   57 #endif
//   58 }
??outputPinRegisterExt_1:
        POP      {R0,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock1
//   59 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z9getHwPort11brain_pin_e
        THUMB
//   60 GPIO_TypeDef * getHwPort(brain_pin_e brainPin) {
_Z9getHwPort11brain_pin_e:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   61 	if (brainPin == GPIO_UNASSIGNED)
        CMP      R4,#+80
        BNE.N    ??getHwPort_0
//   62 		return GPIO_NULL;
        MOVS     R0,#+0
        B.N      ??getHwPort_1
//   63 	if (brainPin > GPIO_UNASSIGNED || brainPin < 0) {
??getHwPort_0:
        CMP      R4,#+81
        BGE.N    ??getHwPort_2
        CMP      R4,#+0
        BPL.N    ??getHwPort_3
//   64 		firmwareError("Invalid brain_pin_e: %d", brainPin);
??getHwPort_2:
        MOVS     R1,R4
        LDR.N    R0,??DataTable7_4
          CFI FunCall firmwareError
        BL       firmwareError
//   65 		return GPIO_NULL;
        MOVS     R0,#+0
        B.N      ??getHwPort_1
//   66 	}
//   67 	return PORTS[brainPin / PORT_SIZE];
??getHwPort_3:
        MOVS     R0,#+16
        SDIV     R0,R4,R0
        LDR.N    R1,??DataTable7_5
        LDR      R0,[R1, R0, LSL #+2]
??getHwPort_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2
//   68 }
//   69 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z8getHwPin11brain_pin_e
        THUMB
//   70 ioportmask_t getHwPin(brain_pin_e brainPin) {
_Z8getHwPin11brain_pin_e:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   71 	if (brainPin == GPIO_UNASSIGNED)
        CMP      R4,#+80
        BNE.N    ??getHwPin_0
//   72 		return EFI_ERROR_CODE;
        MOVS     R0,#-1
        B.N      ??getHwPin_1
//   73 	if (brainPin > GPIO_UNASSIGNED || brainPin < 0) {
??getHwPin_0:
        CMP      R4,#+81
        BGE.N    ??getHwPin_2
        CMP      R4,#+0
        BPL.N    ??getHwPin_3
//   74 		firmwareError("Invalid brain_pin_e: %d", brainPin);
??getHwPin_2:
        MOVS     R1,R4
        LDR.N    R0,??DataTable7_4
          CFI FunCall firmwareError
        BL       firmwareError
//   75 		return EFI_ERROR_CODE;
        MOVS     R0,#-1
        B.N      ??getHwPin_1
//   76 	}
//   77 	return brainPin % PORT_SIZE;
??getHwPin_3:
        MOVS     R1,#+16
        SDIV     R0,R4,R1
        MLS      R0,R0,R1,R4
??getHwPin_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//   78 }
//   79 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
        THUMB
//   80 void outputPinRegisterExt2(const char *msg, OutputPin *output, brain_pin_e brainPin, pin_output_mode_e *outputMode) {
_Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e:
        PUSH     {R3-R9,LR}
          CFI R14 Frame(CFA, -4)
          CFI R9 Frame(CFA, -8)
          CFI R8 Frame(CFA, -12)
          CFI R7 Frame(CFA, -16)
          CFI R6 Frame(CFA, -20)
          CFI R5 Frame(CFA, -24)
          CFI R4 Frame(CFA, -28)
          CFI CFA R13+32
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
//   81 	if (brainPin == GPIO_UNASSIGNED)
        CMP      R6,#+80
        BEQ.N    ??outputPinRegisterExt2_0
//   82 		return;
//   83 	GPIO_TypeDef *hwPort = getHwPort(brainPin);
??outputPinRegisterExt2_1:
        MOVS     R0,R6
          CFI FunCall _Z9getHwPort11brain_pin_e
        BL       _Z9getHwPort11brain_pin_e
        MOV      R8,R0
//   84 	int hwPin = getHwPin(brainPin);
        MOVS     R0,R6
          CFI FunCall _Z8getHwPin11brain_pin_e
        BL       _Z8getHwPin11brain_pin_e
        MOV      R9,R0
//   85 
//   86 	outputPinRegisterExt(msg, output, hwPort, hwPin, outputMode);
        STR      R7,[SP, #+0]
        MOV      R3,R9
        MOV      R2,R8
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z20outputPinRegisterExtPKcP9OutputPinP12GPIO_TypeDefjP17pin_output_mode_e
        BL       _Z20outputPinRegisterExtPKcP9OutputPinP12GPIO_TypeDefjP17pin_output_mode_e
//   87 }
??outputPinRegisterExt2_0:
        POP      {R0,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock4
//   88 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z17outputPinRegisterPKcP9OutputPinP12GPIO_TypeDefj
        THUMB
//   89 void outputPinRegister(const char *msg, OutputPin *output, GPIO_TypeDef *port, uint32_t pin) {
_Z17outputPinRegisterPKcP9OutputPinP12GPIO_TypeDefj:
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
//   90 	outputPinRegisterExt(msg, output, port, pin, &DEFAULT_OUTPUT);
        LDR.N    R0,??DataTable7_6
        STR      R0,[SP, #+0]
        MOVS     R3,R7
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _Z20outputPinRegisterExtPKcP9OutputPinP12GPIO_TypeDefjP17pin_output_mode_e
        BL       _Z20outputPinRegisterExtPKcP9OutputPinP12GPIO_TypeDefjP17pin_output_mode_e
//   91 }
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock5
//   92 
//   93 extern OutputPin checkEnginePin;
//   94 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _Z15initPrimaryPinsv
        THUMB
//   95 void initPrimaryPins(void) {
_Z15initPrimaryPinsv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   96 	outputPinRegister("LED_ERROR", &enginePins.errorLedPin, LED_ERROR_PORT, LED_ERROR_PIN);
        MOVS     R3,#+14
        LDR.N    R2,??DataTable7_7  ;; 0x40020c00
        LDR.N    R1,??DataTable7_8
        LDR.N    R0,??DataTable7_9
          CFI FunCall _Z17outputPinRegisterPKcP9OutputPinP12GPIO_TypeDefj
        BL       _Z17outputPinRegisterPKcP9OutputPinP12GPIO_TypeDefj
//   97 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock6
//   98 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _Z14initOutputPinsv
        THUMB
//   99 void initOutputPins(void) {
_Z14initOutputPinsv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  100 	/**
//  101 	 * want to make sure it's all zeros so that we can compare in initOutputPinExt() method
//  102 	 */
//  103 // todo: it's too late to clear now? this breaks default status LEDs
//  104 // todo: fix this?
//  105 //	memset(&outputs, 0, sizeof(outputs));
//  106 //	outputPinRegister("ext led 1", LED_EXT_1, EXTRA_LED_1_PORT, EXTRA_LED_1_PIN);
//  107 //	outputPinRegister("ext led 2", LED_EXT_2, EXTRA_LED_2_PORT, EXTRA_LED_2_PIN);
//  108 //	outputPinRegister("ext led 3", LED_EXT_3, EXTRA_LED_2_PORT, EXTRA_LED_3_PIN);
//  109 //	outputPinRegister("alive1", LED_DEBUG, GPIOD, 6);
//  110 	outputPinRegisterExt2("MalfunctionIndicator", &checkEnginePin, boardConfiguration->malfunctionIndicatorPin, &DEFAULT_OUTPUT);
        LDR.N    R3,??DataTable7_6
        LDR.N    R0,??DataTable7_10
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+136]
        LDR.N    R1,??DataTable7_11
        LDR.N    R0,??DataTable7_12
          CFI FunCall _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
        BL       _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
//  111 
//  112 // todo: are these needed here? todo: make configurable
//  113 //	outputPinRegister("spi CS1", SPI_CS_1, SPI_CS1_PORT, SPI_CS1_PIN);
//  114 //	outputPinRegister("spi CS2", SPI_CS_2, SPI_CS2_PORT, SPI_CS2_PIN);
//  115 //	outputPinRegister("spi CS3", SPI_CS_3, SPI_CS3_PORT, SPI_CS3_PIN);
//  116 //	outputPinRegister("spi CS4", SPI_CS_4, SPI_CS4_PORT, SPI_CS4_PIN);
//  117 #if HAL_USE_SPI || defined(__DOXYGEN__)
//  118 	outputPinRegisterExt2("spi CS5", &sdCsPin, boardConfiguration->sdCardCsPin, &DEFAULT_OUTPUT);
        LDR.N    R3,??DataTable7_6
        LDR.N    R0,??DataTable7_10
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+324]
        LDR.N    R1,??DataTable7_2
        LDR.N    R0,??DataTable7_13
          CFI FunCall _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
        BL       _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
//  119 #endif
//  120 
//  121 	// todo: should we move this code closer to the fuel pump logic?
//  122 	outputPinRegisterExt2("fuel pump relay", &enginePins.fuelPumpRelay, boardConfiguration->fuelPumpPin, &DEFAULT_OUTPUT);
        LDR.N    R3,??DataTable7_6
        LDR.N    R0,??DataTable7_10
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+24]
        LDR.N    R1,??DataTable7_14
        LDR.N    R0,??DataTable7_15
          CFI FunCall _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
        BL       _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
//  123 
//  124 	outputPinRegisterExt2("main relay", &enginePins.mainRelay, boardConfiguration->mainRelayPin, &boardConfiguration->mainRelayPinMode);
        LDR.N    R0,??DataTable7_10
        LDR      R0,[R0, #+0]
        ADDS     R3,R0,#+412
        LDR.N    R0,??DataTable7_10
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+308]
        LDR.N    R1,??DataTable7_16
        LDR.N    R0,??DataTable7_17
          CFI FunCall _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
        BL       _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
//  125 
//  126 	outputPinRegisterExt2("fan relay", &enginePins.fanRelay, boardConfiguration->fanPin, &DEFAULT_OUTPUT);
        LDR.N    R3,??DataTable7_6
        LDR.N    R0,??DataTable7_10
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+144]
        LDR.N    R1,??DataTable7_18
        LDR.N    R0,??DataTable7_19
          CFI FunCall _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
        BL       _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
//  127 	outputPinRegisterExt2("o2 heater", &enginePins.o2heater, boardConfiguration->o2heaterPin, &DEFAULT_OUTPUT);
        LDR.N    R3,??DataTable7_6
        LDR.N    R0,??DataTable7_10
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+376]
        LDR.N    R1,??DataTable7_20
        LDR.N    R0,??DataTable7_21
          CFI FunCall _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
        BL       _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
//  128 	outputPinRegisterExt2("A/C relay", &enginePins.acRelay, boardConfiguration->acRelayPin, &boardConfiguration->acRelayPinMode);
        LDR.N    R0,??DataTable7_10
        LDR      R0,[R0, #+0]
        ADDS     R3,R0,#+612
        LDR.N    R0,??DataTable7_10
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+608]
        LDR.N    R1,??DataTable7_22
        LDR.N    R0,??DataTable7_23
          CFI FunCall _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
        BL       _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
//  129 
//  130 	// digit 1
//  131 	/*
//  132 	 ledRegister(LED_HUGE_0, GPIOB, 2);
//  133 	 ledRegister(LED_HUGE_1, GPIOE, 7);
//  134 	 ledRegister(LED_HUGE_2, GPIOE, 8);
//  135 	 ledRegister(LED_HUGE_3, GPIOE, 9);
//  136 	 ledRegister(LED_HUGE_4, GPIOE, 10);
//  137 	 ledRegister(LED_HUGE_5, GPIOE, 11);
//  138 	 ledRegister(LED_HUGE_6, GPIOE, 12);
//  139 
//  140 	 // digit 2
//  141 	 ledRegister(LED_HUGE_7, GPIOE, 13);
//  142 	 ledRegister(LED_HUGE_8, GPIOE, 14);
//  143 	 ledRegister(LED_HUGE_9, GPIOE, 15);
//  144 	 ledRegister(LED_HUGE_10, GPIOB, 10);
//  145 	 ledRegister(LED_HUGE_11, GPIOB, 11);
//  146 	 ledRegister(LED_HUGE_12, GPIOB, 12);
//  147 	 ledRegister(LED_HUGE_13, GPIOB, 13);
//  148 
//  149 	 // digit 3
//  150 	 ledRegister(LED_HUGE_14, GPIOE, 0);
//  151 	 ledRegister(LED_HUGE_15, GPIOE, 2);
//  152 	 ledRegister(LED_HUGE_16, GPIOE, 4);
//  153 	 ledRegister(LED_HUGE_17, GPIOE, 6);
//  154 	 ledRegister(LED_HUGE_18, GPIOE, 5);
//  155 	 ledRegister(LED_HUGE_19, GPIOE, 3);
//  156 	 ledRegister(LED_HUGE_20, GPIOE, 1);
//  157 	 */
//  158 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock7
//  159 
//  160 #if EFI_GPIO
//  161 
//  162 /**
//  163  * This method is part of fatal error handling.
//  164  * Please note that worst case scenario the pins might get re-enabled by some other code :(
//  165  * The whole method is pretty naive, but that's at least something.
//  166  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _Z14turnAllPinsOffv
        THUMB
//  167 void turnAllPinsOff(void) {
_Z14turnAllPinsOffv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
//  168 	for (int i = 0; i < INJECTION_PIN_COUNT; i++) {
        MOVS     R4,#+0
??turnAllPinsOff_0:
        CMP      R4,#+12
        BGE.N    ??turnAllPinsOff_1
//  169 		enginePins.injectors[i].setValue(false);
        MOVS     R1,#+0
        MOVS     R0,#+20
        LDR.N    R2,??DataTable7_16
        MLA      R0,R0,R4,R2
        ADDS     R0,R0,#+96
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
//  170 	}
        ADDS     R4,R4,#+1
        B.N      ??turnAllPinsOff_0
//  171 	for (int i = 0; i < IGNITION_PIN_COUNT; i++) {
??turnAllPinsOff_1:
        MOVS     R4,#+0
??turnAllPinsOff_2:
        CMP      R4,#+12
        BGE.N    ??turnAllPinsOff_3
//  172 		enginePins.coils[i].setValue(false);
        MOVS     R1,#+0
        MOVS     R0,#+20
        LDR.N    R2,??DataTable7_16
        MLA      R0,R0,R4,R2
        ADDS     R0,R0,#+336
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
//  173 	}
        ADDS     R4,R4,#+1
        B.N      ??turnAllPinsOff_2
//  174 }
??turnAllPinsOff_3:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7:
        DC32     `?<Constant "io_pins">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_1:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_2:
        DC32     sdCsPin

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_3:
        DC32     `?<Constant "invalid pin_output_mo...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_4:
        DC32     `?<Constant "Invalid brain_pin_e: %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_5:
        DC32     PORTS

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_6:
        DC32     DEFAULT_OUTPUT

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_7:
        DC32     0x40020c00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_8:
        DC32     enginePins+0x50

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_9:
        DC32     `?<Constant "LED_ERROR">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_10:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_11:
        DC32     checkEnginePin

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_12:
        DC32     `?<Constant "MalfunctionIndicator">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_13:
        DC32     `?<Constant "spi CS5">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_14:
        DC32     enginePins+0x30

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_15:
        DC32     `?<Constant "fuel pump relay">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_16:
        DC32     enginePins

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_17:
        DC32     `?<Constant "main relay">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_18:
        DC32     enginePins+0x10

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_19:
        DC32     `?<Constant "fan relay">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_20:
        DC32     enginePins+0x40

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_21:
        DC32     `?<Constant "o2 heater">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_22:
        DC32     enginePins+0x20

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable7_23:
        DC32     `?<Constant "A/C relay">`

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
`?<Constant "invalid pin_output_mo...">`:
        DATA
        DC8 "invalid pin_output_mode_e"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "Invalid brain_pin_e: %d">`:
        DATA
        DC8 "Invalid brain_pin_e: %d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "LED_ERROR">`:
        DATA
        DC8 "LED_ERROR"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "MalfunctionIndicator">`:
        DATA
        DC8 "MalfunctionIndicator"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "spi CS5">`:
        DATA
        DC8 "spi CS5"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fuel pump relay">`:
        DATA
        DC8 "fuel pump relay"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "main relay">`:
        DATA
        DC8 "main relay"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "fan relay">`:
        DATA
        DC8 "fan relay"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "o2 heater">`:
        DATA
        DC8 "o2 heater"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "A/C relay">`:
        DATA
        DC8 "A/C relay"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "io_pins">`:
        DATA
        DC8 "io_pins"

        END
//  175 #endif
// 
// 280 bytes in section .bss
//  32 bytes in section .data
//   4 bytes in section .init_array
// 168 bytes in section .rodata
// 594 bytes in section .text
// 
// 598 bytes of CODE  memory
// 168 bytes of CONST memory
// 312 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
