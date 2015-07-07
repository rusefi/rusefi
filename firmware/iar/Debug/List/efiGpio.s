///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:28 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\sys /
//                       tem\efiGpio.cpp                                      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\sys /
//                       tem\efiGpio.cpp -lCN F:\stuff\rusefi_sourceforge\fir /
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
//                       efiGpio.s                                            /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME efiGpio

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN __aeabi_vec_ctor_nocookie_nodtor
        EXTERN firmwareError

        PUBLIC OUTPUT_MODE_DEFAULT
        PUBLIC _ZN13engine_pins_sC1Ev
        PUBLIC _ZN14NamedOutputPinC1EPKc
        PUBLIC _ZN14NamedOutputPinC1Ev
        PUBLIC _ZN14NamedOutputPinC2EPKc
        PUBLIC _ZN14NamedOutputPinC2Ev
        PUBLIC _ZN9OutputPin10unregisterEv
        PUBLIC _ZN9OutputPin13getLogicValueEv
        PUBLIC _ZN9OutputPin13isInitializedEv
        PUBLIC _ZN9OutputPin18setDefaultPinStateEP17pin_output_mode_e
        PUBLIC _ZN9OutputPin8setValueEi
        PUBLIC _ZN9OutputPinC1Ev
        PUBLIC _ZN9OutputPinC2Ev
        PUBLIC enginePins
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\system\efiGpio.cpp
//    1 /**
//    2  * @file	efiGpio.cpp
//    3  *
//    4  * @date Sep 26, 2014
//    5  * @author Andrey Belomutskiy, (c) 2012-2015
//    6  */
//    7 
//    8 #include "main.h"
//    9 #if EFI_GPIO
//   10 #include "efiGpio.h"
//   11 #include "io_pins.h"
//   12 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//   13 #include "gpio_helper.h"
//   14 #endif
//   15 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   16 pin_output_mode_e OUTPUT_MODE_DEFAULT = OM_DEFAULT;
OUTPUT_MODE_DEFAULT:
        DS8 4
//   17 
//   18 // todo: clean this mess, this should become 'static'/private

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   19 engine_pins_s enginePins;
enginePins:
        DS8 576

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _ZN9OutputPinC2Ev
        THUMB
// __code __interwork __softfp OutputPin::subobject OutputPin()
_ZN9OutputPinC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN9OutputPinC1Ev
        BL       _ZN9OutputPinC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0
//   20 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _ZN14NamedOutputPinC2Ev
        THUMB
// __code __interwork __softfp NamedOutputPin::subobject NamedOutputPin()
_ZN14NamedOutputPinC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN14NamedOutputPinC1Ev
        BL       _ZN14NamedOutputPinC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _ZN14NamedOutputPinC2EPKc
        THUMB
// __code __interwork __softfp NamedOutputPin::subobject NamedOutputPin(char const *)
_ZN14NamedOutputPinC2EPKc:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN14NamedOutputPinC1EPKc
        BL       _ZN14NamedOutputPinC1EPKc
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        LDR.N    R0,??DataTable3
          CFI FunCall _ZN13engine_pins_sC1Ev
        BL       _ZN13engine_pins_sC1Ev
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock3

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _ZN14NamedOutputPinC1Ev
        THUMB
//   21 NamedOutputPin::NamedOutputPin() : OutputPin() {
_ZN14NamedOutputPinC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN9OutputPinC2Ev
        BL       _ZN9OutputPinC2Ev
//   22 	name = NULL;
        MOVS     R0,#+0
        STR      R0,[R4, #+16]
//   23 }
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock4
//   24 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _ZN14NamedOutputPinC1EPKc
        THUMB
//   25 NamedOutputPin::NamedOutputPin(const char *name) : OutputPin() {
_ZN14NamedOutputPinC1EPKc:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R0,R4
          CFI FunCall _ZN9OutputPinC2Ev
        BL       _ZN9OutputPinC2Ev
//   26 	this->name = name;
        STR      R5,[R4, #+16]
//   27 }
        MOVS     R0,R4
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock5
//   28 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _ZN9OutputPinC1Ev
          CFI NoCalls
        THUMB
//   29 OutputPin::OutputPin() {
//   30 	modePtr = &OUTPUT_MODE_DEFAULT;
_ZN9OutputPinC1Ev:
        LDR.N    R1,??DataTable3_1
        STR      R1,[R0, #+8]
//   31 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//   32 	port = NULL;
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//   33 	pin = 0;
        MOVS     R1,#+0
        STR      R1,[R0, #+4]
//   34 	currentLogicValue = INITIAL_PIN_STATE;
        MOVS     R1,#-1
        STR      R1,[R0, #+12]
//   35 #endif
//   36 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock6
//   37 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _ZN9OutputPin13isInitializedEv
          CFI NoCalls
        THUMB
//   38 bool_t OutputPin::isInitialized() {
//   39 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//   40 	return port != NULL;
_ZN9OutputPin13isInitializedEv:
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??isInitialized_0
        MOVS     R0,#+1
        B.N      ??isInitialized_1
??isInitialized_0:
        MOVS     R0,#+0
??isInitialized_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return
          CFI EndBlock cfiBlock7
//   41 #else
//   42 	return false;
//   43 #endif
//   44 }
//   45 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _ZN9OutputPin8setValueEi
        THUMB
//   46 void OutputPin::setValue(int logicValue) {
_ZN9OutputPin8setValueEi:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//   47 	doSetOutputPinValue2(this, logicValue);
        LDR      R0,[R4, #+0]
        CMP      R0,#+0
        BEQ.N    ??setValue_0
        LDR      R0,[R4, #+8]
        CMP      R0,#+0
        BNE.N    ??setValue_1
        LDR.N    R0,??DataTable3_2
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??setValue_2
??setValue_1:
        LDR      R0,[R4, #+8]
        LDR      R0,[R0, #+0]
        MOVS     R6,R0
        CMP      R6,#+4
        BLT.N    ??setValue_3
        LDR.N    R0,??DataTable3_3
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??setValue_2
??setValue_3:
        CMP      R5,#+0
        BEQ.N    ??setValue_4
        CMP      R6,#+0
        BEQ.N    ??setValue_5
        CMP      R6,#+2
        BNE.N    ??setValue_6
??setValue_5:
        MOVS     R7,#+1
        B.N      ??setValue_7
??setValue_6:
        MOVS     R7,#+0
??setValue_7:
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        B.N      ??setValue_8
??setValue_4:
        CMP      R6,#+1
        BEQ.N    ??setValue_9
        CMP      R6,#+3
        BNE.N    ??setValue_10
??setValue_9:
        MOVS     R7,#+1
        B.N      ??setValue_11
??setValue_10:
        MOVS     R7,#+0
??setValue_11:
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
??setValue_8:
        LDR      R0,[R4, #+12]
        CMP      R0,R5
        BEQ.N    ??setValue_0
        MVNS     R0,R7
        ANDS     R0,R0,#0x1
        LDR      R1,[R4, #+4]
        ADDS     R1,R1,#+16
        LSLS     R0,R0,R1
        ANDS     R1,R7,#0x1
        LDR      R2,[R4, #+4]
        LSLS     R1,R1,R2
        ORRS     R0,R1,R0
        LDR      R1,[R4, #+0]
        STR      R0,[R1, #+24]
        STR      R5,[R4, #+12]
//   48 }
??setValue_0:
??setValue_2:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock8
//   49 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _ZN9OutputPin13getLogicValueEv
          CFI NoCalls
        THUMB
//   50 bool_t OutputPin::getLogicValue() {
//   51 	return currentLogicValue;
_ZN9OutputPin13getLogicValueEv:
        LDR      R0,[R0, #+12]
        CMP      R0,#+0
        BEQ.N    ??getLogicValue_0
        MOVS     R0,#+1
        B.N      ??getLogicValue_1
??getLogicValue_0:
        MOVS     R0,#+0
??getLogicValue_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return
          CFI EndBlock cfiBlock9
//   52 }
//   53 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _ZN9OutputPin10unregisterEv
          CFI NoCalls
        THUMB
//   54 void OutputPin::unregister() {
//   55 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//   56 	port = NULL;
_ZN9OutputPin10unregisterEv:
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//   57 #endif
//   58 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock10

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _ZN9OutputPin18setDefaultPinStateEP17pin_output_mode_e
        THUMB
_ZN9OutputPin18setDefaultPinStateEP17pin_output_mode_e:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        LDR      R6,[R5, #+0]
        CMP      R6,#+4
        BLT.N    ??setDefaultPinState_0
        LDR.N    R0,??DataTable3_3
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??setDefaultPinState_1
??setDefaultPinState_0:
        STR      R5,[R4, #+8]
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
??setDefaultPinState_1:
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock11

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3:
        DC32     enginePins

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_1:
        DC32     OUTPUT_MODE_DEFAULT

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_2:
        DC32     `?<Constant "pin mode not initialized">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable3_3:
        DC32     `?<Constant "invalid pin_output_mo...">`

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN13engine_pins_sC1Ev
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _ZN13engine_pins_sC1Ev
        THUMB
// __code __interwork __softfp engine_pins_s::<_ZN13engine_pins_sC1Ev>()
_ZN13engine_pins_sC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN9OutputPinC1Ev
        BL       _ZN9OutputPinC1Ev
        ADDS     R0,R4,#+16
          CFI FunCall _ZN9OutputPinC1Ev
        BL       _ZN9OutputPinC1Ev
        ADDS     R0,R4,#+32
          CFI FunCall _ZN9OutputPinC1Ev
        BL       _ZN9OutputPinC1Ev
        ADDS     R0,R4,#+48
          CFI FunCall _ZN9OutputPinC1Ev
        BL       _ZN9OutputPinC1Ev
        ADDS     R0,R4,#+64
          CFI FunCall _ZN9OutputPinC1Ev
        BL       _ZN9OutputPinC1Ev
        ADDS     R0,R4,#+80
          CFI FunCall _ZN9OutputPinC1Ev
        BL       _ZN9OutputPinC1Ev
        MOVS     R3,#+12
        MOVS     R2,#+20
        LDR.N    R1,`?<Constructor (complete)>_0`
        ADDS     R0,R4,#+96
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
        MOVS     R3,#+12
        MOVS     R2,#+20
        LDR.N    R1,`?<Constructor (complete)>_0`
        ADDS     R0,R4,#+336
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
        Nop      
        DATA
`?<Constructor (complete)>_0`:
        DC32     _ZN14NamedOutputPinC1Ev
          CFI EndBlock cfiBlock12

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
`?<Constant "pin mode not initialized">`:
        DATA
        DC8 "pin mode not initialized"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid pin_output_mo...">`:
        DATA
        DC8 "invalid pin_output_mode_e"
        DC8 0, 0

        END
//   59 
//   60 void OutputPin::setDefaultPinState(pin_output_mode_e *outputMode) {
//   61 #if EFI_GPIO || defined(__DOXYGEN__)
//   62 	pin_output_mode_e mode = *outputMode;
//   63 	assertOMode(mode);
//   64 	this->modePtr = outputMode;
//   65 #endif
//   66 	setValue(false); // initial state
//   67 }
//   68 
//   69 #endif /* EFI_GPIO */
// 
// 580 bytes in section .bss
//   4 bytes in section .init_array
//  56 bytes in section .rodata
// 394 bytes in section .text
// 
// 310 bytes of CODE  memory (+ 88 bytes shared)
//  56 bytes of CONST memory
// 580 bytes of DATA  memory
//
//Errors: none
//Warnings: none
