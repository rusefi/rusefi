///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:00 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\tri /
//                       gger\trigger_decoder.cpp                             /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\tri /
//                       gger\trigger_decoder.cpp -lCN                        /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                        -lA F:\stuff\rusefi_sourceforge\firmware\iar\Debug\ /
//                       List\ -o F:\stuff\rusefi_sourceforge\firmware\iar\De /
//                       bug\Obj\ --no_cse --no_unroll --no_inline            /
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
//                       trigger_decoder.s                                    /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME trigger_decoder

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z12initDodgeRamP12TriggerShape
        EXTERN _Z18setVwConfigurationP12TriggerShape
        EXTERN _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
        EXTERN _Z22configureHondaAccordCDP12TriggerShapebb15trigger_wheel_eS1_f
        EXTERN _Z23configureMazdaProtegeLxP12TriggerShape
        EXTERN _Z24initializeMitsubishi4g18P12TriggerShape
        EXTERN _Z25configureHondaAccordCDDipP12TriggerShape
        EXTERN _Z25configureMazdaProtegeSOHCP12TriggerShape
        EXTERN _Z27initializeMazdaMiataNaShapeP12TriggerShape
        EXTERN _Z27initializeMazdaMiataNbShapeP12TriggerShape
        EXTERN _Z28setToothedWheelConfigurationP12TriggerShapeii16operation_mode_e
        EXTERN _Z29configureNeon1995TriggerShapeP12TriggerShape
        EXTERN _Z29configureNeon2003TriggerShapeP12TriggerShape
        EXTERN _Z31configureFordAspireTriggerShapeP12TriggerShape
        EXTERN _Z31configureMiniCooperTriggerShapeP12TriggerShape
        EXTERN _ZN12TriggerShape26calculateTriggerSynchPointEv
        EXTERN _ZN12TriggerShape28setTriggerSynchronizationGapEf
        EXTERN _ZN12TriggerShape5clearEv
        EXTERN _ZN12TriggerShape5resetE16operation_mode_eb
        EXTERN _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        EXTERN _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_eff
        EXTERN _ZN12TriggerState25getTotalRevolutionCounterEv
        EXTERN _ZN12TriggerState5clearEv
        EXTERN _ZN12TriggerStateC1Ev
        EXTERN _ZN12multi_wave_s16checkSwitchTimesEi
        EXTERN _ZN9OutputPin8setValueEi
        EXTERN _ZN9OutputPinC1Ev
        EXTERN _ZNK12TriggerShape7getSizeEv
        EXTERN _ZNK12multi_wave_s13getSwitchTimeEi
        EXTERN _ZNK12multi_wave_s15getChannelStateEii
        EXTERN __aeabi_atexit
        EXTERN __aeabi_d2f
        EXTERN __aeabi_ddiv
        EXTERN __aeabi_dmul
        EXTERN __aeabi_f2d
        EXTERN __aeabi_i2d
        EXTERN __aeabi_l2f
        EXTERN __aeabi_memcpy
        EXTERN __aeabi_memset
        EXTERN __aeabi_ui2d
        EXTERN __dso_handle
        EXTERN _engine
        EXTERN boardConfiguration
        EXTERN engineConfiguration
        EXTERN firmwareError
        EXTERN getTimeNowNt
        EXTERN scheduleMsg
        EXTERN warning

        PUBLIC _Z14getEngineCycle16operation_mode_e
        PUBLIC _Z18initTriggerDecoderv
        PUBLIC _Z21isTriggerDecoderErrorv
        PUBLIC _Z24initTriggerDecoderLoggerP7Logging
        PUBLIC _Z25findTriggerZeroEventIndexP12TriggerShapePK16trigger_config_s
        PUBLIC _Z28addSkippedToothTriggerEvents15trigger_wheel_eP12TriggerShapeiifffff
        PUBLIC _Z37initializeSkippedToothTriggerShapeExtP12TriggerShapeii16operation_mode_e
        PUBLIC _ZN12TriggerShape22initializeTriggerShapeEP7Logging
        PUBLIC _ZN12TriggerState18decodeTriggerEventE15trigger_event_ex
        PUBLIC _ZN12TriggerState19getTriggerDutyCycleEi
        PUBLIC _ZN13cyclic_bufferIiE3addEi
        PUBLIC _ZN13cyclic_bufferIiE3sumEi
        PUBLIC _ZN13cyclic_bufferIiE5baseCEi
        PUBLIC _ZN13cyclic_bufferIiE5clearEv
        PUBLIC _ZN13cyclic_bufferIiE7setSizeEi
        PUBLIC _ZN13cyclic_bufferIiEC1Ev
        PUBLIC _ZN13cyclic_bufferIiEC2Ev
        PUBLIC _ZN13cyclic_bufferIiED1Ev
        PUBLIC _ZN13cyclic_bufferIiED2Ev
        PUBLIC _ZN23TriggerStimulatorHelper8nextStepEP12TriggerStateP12TriggerShapeiPK16trigger_config_s
        PUBLIC _ZN23TriggerStimulatorHelperC1Ev
        PUBLIC _ZN23TriggerStimulatorHelperC2Ev
        PUBLIC lastDecodingErrorTime
        PUBLIC memcpy
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\trigger\trigger_decoder.cpp
//    1 /**
//    2  * @file	trigger_decoder.cpp
//    3  *
//    4  * @date Dec 24, 2013
//    5  * @author Andrey Belomutskiy, (c) 2012-2015
//    6  *
//    7  * This file is part of rusEfi - see http://rusefi.com
//    8  *
//    9  * rusEfi is free software; you can redistribute it and/or modify it under the terms of
//   10  * the GNU General Public License as published by the Free Software Foundation; either
//   11  * version 3 of the License, or (at your option) any later version.
//   12  *
//   13  * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
//   14  * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   15  * GNU General Public License for more details.
//   16  *
//   17  * You should have received a copy of the GNU General Public License along with this program.
//   18  * If not, see <http://www.gnu.org/licenses/>.
//   19  */
//   20 
//   21 #include "main.h"

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

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memset
          CFI Block cfiBlock1 Using cfiCommon0
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
          CFI EndBlock cfiBlock1
//   22 
//   23 #if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
//   24 
//   25 #include "obd_error_codes.h"
//   26 #include "trigger_decoder.h"
//   27 #include "cyclic_buffer.h"

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIiEC1Ev
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIiEC1Ev
        THUMB
// __code __interwork __softfp cyclic_buffer<int>::cyclic_buffer()
_ZN13cyclic_bufferIiEC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R1,#+64
        MOVS     R0,R4
          CFI FunCall _ZN13cyclic_bufferIiE5baseCEi
        BL       _ZN13cyclic_bufferIiE5baseCEi
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIiE5baseCEi
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIiE5baseCEi
        THUMB
// __interwork __softfp void cyclic_buffer<int>::baseC(int)
_ZN13cyclic_bufferIiE5baseCEi:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R0,#+0
        STR      R0,[R4, #+256]
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _ZN13cyclic_bufferIiE7setSizeEi
        BL       _ZN13cyclic_bufferIiE7setSizeEi
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock3

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIiED1Ev
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIiED1Ev
          CFI NoCalls
        THUMB
// __code __interwork __softfp cyclic_buffer<int>::~cyclic_buffer()
_ZN13cyclic_bufferIiED1Ev:
        BX       LR               ;; return
          CFI EndBlock cfiBlock4

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIiE3addEi
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIiE3addEi
          CFI NoCalls
        THUMB
// __interwork __softfp void cyclic_buffer<int>::add(int)
_ZN13cyclic_bufferIiE3addEi:
        LDR      R2,[R0, #+256]
        ADDS     R2,R2,#+1
        STR      R2,[R0, #+256]
        LDR      R2,[R0, #+256]
        LDR      R3,[R0, #+264]
        CMP      R2,R3
        BNE.N    ??add_0
        MOVS     R2,#+0
        STR      R2,[R0, #+256]
??add_0:
        LDR      R2,[R0, #+256]
        STR      R1,[R0, R2, LSL #+2]
        LDR      R2,[R0, #+260]
        ADDS     R2,R2,#+1
        STR      R2,[R0, #+260]
        BX       LR               ;; return
          CFI EndBlock cfiBlock5

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIiE7setSizeEi
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIiE7setSizeEi
        THUMB
// __interwork __softfp void cyclic_buffer<int>::setSize(int)
_ZN13cyclic_bufferIiE7setSizeEi:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R0,R4
          CFI FunCall _ZN13cyclic_bufferIiE5clearEv
        BL       _ZN13cyclic_bufferIiE5clearEv
        CMP      R5,#+64
        BGE.N    ??setSize_0
        MOVS     R0,R5
        B.N      ??setSize_1
??setSize_0:
        MOVS     R0,#+64
??setSize_1:
        STR      R0,[R4, #+264]
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock6

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIiE3sumEi
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIiE3sumEi
          CFI NoCalls
        THUMB
// __interwork __softfp int cyclic_buffer<int>::sum(int)
_ZN13cyclic_bufferIiE3sumEi:
        PUSH     {R4-R6}
          CFI R6 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+12
        MOVS     R2,R0
        LDR      R0,[R2, #+260]
        CMP      R0,R1
        BGE.N    ??sum_0
        LDR      R0,[R2, #+260]
        MOVS     R1,R0
??sum_0:
        LDR      R3,[R2, #+256]
        MOVS     R0,#+0
        MOVS     R4,#+0
??sum_1:
        CMP      R4,R1
        BGE.N    ??sum_2
        SUBS     R5,R3,R4
??sum_3:
        CMP      R5,#+0
        BPL.N    ??sum_4
        LDR      R6,[R2, #+264]
        ADDS     R5,R6,R5
        B.N      ??sum_3
??sum_4:
        LDR      R5,[R2, R5, LSL #+2]
        ADDS     R0,R5,R0
        ADDS     R4,R4,#+1
        B.N      ??sum_1
??sum_2:
        POP      {R4-R6}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R6 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock7

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIiE5clearEv
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIiE5clearEv
        THUMB
// __interwork __softfp void cyclic_buffer<int>::clear()
_ZN13cyclic_bufferIiE5clearEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOV      R2,#+256
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall memset
        BL       memset
        MOVS     R0,#+0
        STR      R0,[R4, #+260]
        MOVS     R0,#+0
        STR      R0,[R4, #+260]
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock8
//   28 #include "trigger_mazda.h"
//   29 #include "trigger_chrysler.h"
//   30 #include "trigger_gm.h"
//   31 #include "trigger_bmw.h"
//   32 #include "trigger_mitsubishi.h"
//   33 #include "auto_generated_enums.h"
//   34 #include "trigger_structure.h"
//   35 #include "efiGpio.h"
//   36 #include "engine.h"
//   37 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   38 static OutputPin triggerDecoderErrorPin;
triggerDecoderErrorPin:
        DS8 16
//   39 
//   40 EXTERN_ENGINE
//   41 ;
//   42 
//   43 // todo: better name for this constant
//   44 #define HELPER_PERIOD 100000
//   45 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   46 static cyclic_buffer<int> errorDetection;
errorDetection:
        DS8 268
//   47 
//   48 #if ! EFI_PROD_CODE
//   49 bool printTriggerDebug = false;
//   50 float actualSynchGap;
//   51 #endif /* ! EFI_PROD_CODE */
//   52 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   53 static Logging * logger;
logger:
        DS8 4
//   54 

        SECTION `.data`:DATA:REORDER:NOROOT(3)
//   55 efitick_t lastDecodingErrorTime = US2NT(-10000000LL);
lastDecodingErrorTime:
        DATA
        DC32 2614967296, 4294967295

        SECTION `.data`:DATA:REORDER:NOROOT(2)
eventIndex:
        DATA
        DC8 0, 0, 1, 1, 2, 2, 0, 0

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _ZN23TriggerStimulatorHelperC2Ev
        THUMB
// __code __interwork __softfp TriggerStimulatorHelper::subobject TriggerStimulatorHelper()
_ZN23TriggerStimulatorHelperC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN23TriggerStimulatorHelperC1Ev
        BL       _ZN23TriggerStimulatorHelperC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock9

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        LDR.W    R0,??DataTable14_1
          CFI FunCall _ZN9OutputPinC1Ev
        BL       _ZN9OutputPinC1Ev
        LDR.W    R0,??DataTable14_2
          CFI FunCall _ZN13cyclic_bufferIiEC1Ev
        BL       _ZN13cyclic_bufferIiEC1Ev
        LDR.W    R2,??DataTable14_3
        LDR.W    R1,??DataTable14_4
          CFI FunCall __aeabi_atexit
        BL       __aeabi_atexit
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock10
//   56 
//   57 /**
//   58  * @return TRUE is something is wrong with trigger decoding
//   59  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _Z21isTriggerDecoderErrorv
        THUMB
//   60 bool_t isTriggerDecoderError(void) {
_Z21isTriggerDecoderErrorv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   61 	return errorDetection.sum(6) > 4;
        MOVS     R1,#+6
        LDR.W    R0,??DataTable14_2
          CFI FunCall _ZN13cyclic_bufferIiE3sumEi
        BL       _ZN13cyclic_bufferIiE3sumEi
        CMP      R0,#+5
        BLT.N    ??isTriggerDecoderError_0
        MOVS     R0,#+1
        B.N      ??isTriggerDecoderError_1
??isTriggerDecoderError_0:
        MOVS     R0,#+0
??isTriggerDecoderError_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R1,PC}          ;; return
          CFI EndBlock cfiBlock11
//   62 }
//   63 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _ZN12TriggerState19getTriggerDutyCycleEi
        THUMB
//   64 float TriggerState::getTriggerDutyCycle(int index) {
_ZN12TriggerState19getTriggerDutyCycleEi:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R1
//   65 	float time = prevTotalTime[index];
        ADDS     R1,R0,R4, LSL #+2
        VLDR     S0,[R1, #+36]
        VCVT.F32.U32 S0,S0
//   66 
//   67 	return 100 * time / prevCycleDuration;
        VLDR.W   S1,??DataTable2  ;; 0x42c80000
        VMUL.F32 S16,S0,S1
        LDRD     R2,R3,[R0, #+168]
        MOVS     R0,R2
        MOVS     R1,R3
          CFI FunCall __aeabi_l2f
        BL       __aeabi_l2f
        VMOV     S0,R0
        VDIV.F32 S0,S16,S0
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock12
//   68 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2:
        DC32     0x42c80000
//   69 
//   70 static trigger_wheel_e eventIndex[6] = { T_PRIMARY, T_PRIMARY, T_SECONDARY, T_SECONDARY, T_CHANNEL_3, T_CHANNEL_3 };
//   71 static trigger_value_e eventType[6] = { TV_LOW, TV_HIGH, TV_LOW, TV_HIGH, TV_LOW, TV_HIGH };
//   72 
//   73 #define getCurrentGapDuration(nowNt) \ 
//   74 	(isFirstEvent ? 0 : (nowNt) - toothed_previous_time)
//   75 
//   76 #define nextTriggerEvent() \ 
//   77  { \ 
//   78 	efitime_t prevTime = timeOfPreviousEventNt[triggerWheel]; \ 
//   79 	if (prevTime != 0) { \ 
//   80 		/* even event - apply the value*/ \ 
//   81 		totalTimeNt[triggerWheel] += (nowNt - prevTime); \ 
//   82 		timeOfPreviousEventNt[triggerWheel] = 0; \ 
//   83 	} else { \ 
//   84 		/* odd event - start accumulation */ \ 
//   85 		timeOfPreviousEventNt[triggerWheel] = nowNt; \ 
//   86 	} \ 
//   87 	if (engineConfiguration->useOnlyFrontForTrigger) {current_index++;} \ 
//   88 	current_index++; \ 
//   89 }
//   90 
//   91 #define nextRevolution() { \ 
//   92 	if (cycleCallback != NULL) { \ 
//   93 		cycleCallback(this); \ 
//   94 	} \ 
//   95 	memcpy(prevTotalTime, totalTimeNt, sizeof(prevTotalTime)); \ 
//   96 	prevCycleDuration = nowNt - startOfCycleNt; \ 
//   97 	startOfCycleNt = nowNt; \ 
//   98 	clear(); \ 
//   99 	totalRevolutionCounter++; \ 
//  100 	runningRevolutionCounter++; \ 
//  101 	totalEventCountBase += TRIGGER_SHAPE(size); \ 
//  102 }
//  103 
//  104 /**
//  105  * @brief Trigger decoding happens here
//  106  * This method changes the state of trigger_state_s data structure according to the trigger event
//  107  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _ZN12TriggerState18decodeTriggerEventE15trigger_event_ex
        THUMB
//  108 void TriggerState::decodeTriggerEvent(trigger_event_e const signal, efitime_t nowNt DECLARE_ENGINE_PARAMETER_S) {
_ZN12TriggerState18decodeTriggerEventE15trigger_event_ex:
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
        SUB      SP,SP,#+24
          CFI CFA R13+72
        MOVS     R6,R0
        MOV      R8,R1
        MOVS     R4,R2
        MOVS     R5,R3
//  109 	efiAssertVoid(signal <= SHAFT_3RD_UP, "unexpected signal");
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        CMP      R8,#+6
        BLT.N    ??decodeTriggerEvent_0
        LDR.W    R0,??DataTable14_5
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??decodeTriggerEvent_1
//  110 
//  111 	trigger_wheel_e triggerWheel = eventIndex[signal];
??decodeTriggerEvent_0:
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        LDR.W    R0,??DataTable14_6
        LDRB     R7,[R8, R0]
//  112 
//  113 	if (!engineConfiguration->useOnlyFrontForTrigger && curSignal == prevSignal) {
        LDR.W    R0,??DataTable14_7
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+6
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BNE.N    ??decodeTriggerEvent_2
        LDRB     R0,[R6, #+96]
        LDRB     R1,[R6, #+97]
        CMP      R0,R1
        BNE.N    ??decodeTriggerEvent_2
//  114 		orderingErrorCounter++;
        LDR      R0,[R6, #+68]
        ADDS     R0,R0,#+1
        STR      R0,[R6, #+68]
//  115 	}
//  116 
//  117 	prevSignal = curSignal;
??decodeTriggerEvent_2:
        LDRB     R0,[R6, #+96]
        STRB     R0,[R6, #+97]
//  118 	curSignal = signal;
        STRB     R8,[R6, #+96]
//  119 
//  120 	eventCount[triggerWheel]++;
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        ADDS     R0,R6,R7, LSL #+2
        LDR      R0,[R0, #+84]
        ADDS     R0,R0,#+1
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        ADDS     R1,R6,R7, LSL #+2
        STR      R0,[R1, #+84]
//  121 	eventCountExt[signal]++;
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        ADDS     R0,R6,R8, LSL #+2
        LDR      R0,[R0, #+100]
        ADDS     R0,R0,#+1
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        ADDS     R1,R6,R8, LSL #+2
        STR      R0,[R1, #+100]
//  122 
//  123 	efitime_t currentDurationLong = getCurrentGapDuration(nowNt);
        LDRB     R0,[R6, #+164]
        CMP      R0,#+0
        BEQ.N    ??decodeTriggerEvent_3
        MOVS     R10,#+0
        MOVS     R11,#+0
        B.N      ??decodeTriggerEvent_4
??decodeTriggerEvent_3:
        LDRD     R0,R1,[R6, #+16]
        SUBS     R10,R4,R0
        SBCS     R11,R5,R1
//  124 
//  125 	/**
//  126 	 * For performance reasons, we want to work with 32 bit values. If there has been more then
//  127 	 * 10 seconds since previous trigger event we do not really care.
//  128 	 */
//  129 	currentDuration =
//  130 			currentDurationLong > 10 * US2NT(US_PER_SECOND_LL) ? 10 * US2NT(US_PER_SECOND_LL) : currentDurationLong;
??decodeTriggerEvent_4:
        LDR.W    R0,??DataTable14_8  ;; 0x6422c401
        MOVS     R1,#+0
        CMP      R11,R1
        BLT.N    ??decodeTriggerEvent_5
        BGT.N    ??decodeTriggerEvent_6
        CMP      R10,R0
        BCC.N    ??decodeTriggerEvent_5
??decodeTriggerEvent_6:
        LDR.W    R0,??DataTable14_9  ;; 0x6422c400
        B.N      ??decodeTriggerEvent_7
??decodeTriggerEvent_5:
        MOV      R0,R10
??decodeTriggerEvent_7:
        STR      R0,[R6, #+12]
//  131 
//  132 	if (isLessImportant(signal)) {
        LDR.W    R0,??DataTable14_10
        LDRB     R0,[R0, #+3560]
        CMP      R0,#+0
        BEQ.N    ??decodeTriggerEvent_8
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        CMP      R8,#+1
        BNE.N    ??decodeTriggerEvent_9
??decodeTriggerEvent_8:
        LDR.W    R0,??DataTable14_10
        LDRB     R0,[R0, #+3560]
        CMP      R0,#+0
        BNE.N    ??decodeTriggerEvent_10
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        CMP      R8,#+0
        BEQ.N    ??decodeTriggerEvent_10
//  133 #if EFI_UNIT_TEST
//  134 		if (printTriggerDebug) {
//  135 			printf("%s isLessImportant %s\r\n",
//  136 					getTrigger_type_e(engineConfiguration->trigger.type),
//  137 					getTrigger_event_e(signal));
//  138 		}
//  139 #endif
//  140 
//  141 		/**
//  142 		 * For less important events we simply increment the index.
//  143 		 */
//  144 		nextTriggerEvent()
??decodeTriggerEvent_9:
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        ADDS     R12,R6,R7, LSL #+3
        LDRD     R2,R3,[R12, #+128]
        MOVS     R0,R2
        MOVS     R1,R3
        CMP      R1,#+0
        BNE.N    ??decodeTriggerEvent_11
        CMP      R0,#+0
        BEQ.N    ??decodeTriggerEvent_12
??decodeTriggerEvent_11:
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        ADDS     R2,R6,R7, LSL #+2
        LDR      R2,[R2, #+24]
        ADDS     R2,R4,R2
        SUBS     R2,R2,R0
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        ADDS     R3,R6,R7, LSL #+2
        STR      R2,[R3, #+24]
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        ADDS     R12,R6,R7, LSL #+3
        MOVS     R2,#+0
        MOVS     R3,#+0
        STRD     R2,R3,[R12, #+128]
        B.N      ??decodeTriggerEvent_13
??decodeTriggerEvent_12:
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        ADDS     R2,R6,R7, LSL #+3
        STRD     R4,R5,[R2, #+128]
??decodeTriggerEvent_13:
        LDR.W    R2,??DataTable14_7
        LDR      R2,[R2, #+0]
        LDRB     R2,[R2, #+1509]
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        LSRS     R2,R2,#+6
        ANDS     R2,R2,#0x1
        CMP      R2,#+0
        BEQ.N    ??decodeTriggerEvent_14
        LDR      R2,[R6, #+76]
        ADDS     R2,R2,#+1
        STR      R2,[R6, #+76]
??decodeTriggerEvent_14:
        LDR      R2,[R6, #+76]
        ADDS     R2,R2,#+1
        STR      R2,[R6, #+76]
//  145 		;
//  146 		if (TRIGGER_SHAPE(gapBothDirections)) {
        LDR.W    R2,??DataTable14_10
        LDRB     R2,[R2, #+3561]
        CMP      R2,#+0
        BEQ.N    ??decodeTriggerEvent_15
//  147 			toothed_previous_duration = currentDuration;
        LDR      R0,[R6, #+12]
        STR      R0,[R6, #+8]
//  148 			isFirstEvent = false;
        MOVS     R0,#+0
        STRB     R0,[R6, #+164]
//  149 			toothed_previous_time = nowNt;
        STRD     R4,R5,[R6, #+16]
//  150 		}
//  151 		return;
??decodeTriggerEvent_15:
        B.N      ??decodeTriggerEvent_1
//  152 	}
//  153 
//  154 	isFirstEvent = false;
??decodeTriggerEvent_10:
        MOVS     R0,#+0
        STRB     R0,[R6, #+164]
//  155 // todo: skip a number of signal from the beginning
//  156 
//  157 #if EFI_PROD_CODE
//  158 //	scheduleMsg(&logger, "from %f to %f %d %d", triggerConfig->syncRatioFrom, triggerConfig->syncRatioTo, currentDuration, shaftPositionState->toothed_previous_duration);
//  159 //	scheduleMsg(&logger, "ratio %f", 1.0 * currentDuration/ shaftPositionState->toothed_previous_duration);
//  160 #else
//  161 	if (toothed_previous_duration != 0) {
//  162 //		printf("ratio %f: cur=%d pref=%d\r\n", 1.0 * currentDuration / shaftPositionState->toothed_previous_duration,
//  163 //				currentDuration, shaftPositionState->toothed_previous_duration);
//  164 	}
//  165 #endif
//  166 
//  167 	bool_t isSynchronizationPoint;
//  168 
//  169 	if (TRIGGER_SHAPE(isSynchronizationNeeded)) {
        LDR.W    R0,??DataTable14_10
        LDRB     R0,[R0, #+644]
        CMP      R0,#+0
        BEQ.N    ??decodeTriggerEvent_16
//  170 		isSynchronizationPoint = currentDuration > toothed_previous_duration * TRIGGER_SHAPE(syncRatioFrom)
//  171 				&& currentDuration < toothed_previous_duration * TRIGGER_SHAPE(syncRatioTo);
        VLDR     S0,[R6, #+12]
        VCVT.F32.U32 S0,S0
        VLDR     S1,[R6, #+8]
        VCVT.F32.U32 S1,S1
        LDR.W    R0,??DataTable14_11
        VLDR     S2,[R0, #0]
        VMUL.F32 S1,S1,S2
        VCMP.F32 S1,S0
        FMSTAT   
        BPL.N    ??decodeTriggerEvent_17
        VLDR     S0,[R6, #+8]
        VCVT.F32.U32 S0,S0
        LDR.W    R0,??DataTable14_12
        VLDR     S1,[R0, #0]
        VMUL.F32 S0,S0,S1
        VLDR     S1,[R6, #+12]
        VCVT.F32.U32 S1,S1
        VCMP.F32 S1,S0
        FMSTAT   
        BPL.N    ??decodeTriggerEvent_17
        MOVS     R0,#+1
        B.N      ??decodeTriggerEvent_18
??decodeTriggerEvent_17:
        MOVS     R0,#+0
??decodeTriggerEvent_18:
        STRB     R0,[SP, #+20]
//  172 
//  173 #if EFI_PROD_CODE
//  174 		if (engineConfiguration->isPrintTriggerSynchDetails) {
        LDR.W    R0,??DataTable14_7
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+3
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??decodeTriggerEvent_19
//  175 #else
//  176 		if (printTriggerDebug) {
//  177 #endif /* EFI_PROD_CODE */
//  178 			float gap = 1.0 * currentDuration / toothed_previous_duration;
        LDR      R0,[R6, #+12]
          CFI FunCall __aeabi_ui2d
        BL       __aeabi_ui2d
        MOVS     R2,#+0
        LDR.W    R3,??DataTable14_13  ;; 0x3ff00000
          CFI FunCall __aeabi_dmul
        BL       __aeabi_dmul
        STRD     R0,R1,[SP, #+0]
        LDR      R0,[R6, #+8]
          CFI FunCall __aeabi_ui2d
        BL       __aeabi_ui2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDRD     R0,R1,[SP, #+0]
          CFI FunCall __aeabi_ddiv
        BL       __aeabi_ddiv
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        VMOV     S16,R0
//  179 #if EFI_PROD_CODE
//  180 			scheduleMsg(logger, "gap=%f @ %d", gap, current_index);
        LDR      R0,[R6, #+76]
        STR      R0,[SP, #+0]
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.W    R1,??DataTable14_14
        LDR.W    R0,??DataTable14_15
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
        B.N      ??decodeTriggerEvent_19
//  181 #else
//  182 			actualSynchGap = gap;
//  183 			print("current gap %f\r\n", gap);
//  184 #endif /* EFI_PROD_CODE */
//  185 		}
//  186 
//  187 	} else {
//  188 		/**
//  189 		 * in case of noise the counter could be above the expected number of events
//  190 		 */
//  191 		int d = engineConfiguration->useOnlyFrontForTrigger ? 2 : 1;
??decodeTriggerEvent_16:
        LDR.W    R0,??DataTable14_7
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+6
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??decodeTriggerEvent_20
        MOVS     R0,#+2
        B.N      ??decodeTriggerEvent_21
??decodeTriggerEvent_20:
        MOVS     R0,#+1
//  192 		isSynchronizationPoint = !shaft_is_synchronized || (current_index >= TRIGGER_SHAPE(size) - d);
??decodeTriggerEvent_21:
        LDRB     R1,[R6, #+4]
        CMP      R1,#+0
        BEQ.N    ??decodeTriggerEvent_22
        LDR      R1,[R6, #+76]
        LDR.W    R2,??DataTable14_16
        LDR      R2,[R2, #+0]
        SUBS     R0,R2,R0
        CMP      R1,R0
        BCC.N    ??decodeTriggerEvent_23
??decodeTriggerEvent_22:
        MOVS     R0,#+1
        B.N      ??decodeTriggerEvent_24
??decodeTriggerEvent_23:
        MOVS     R0,#+0
??decodeTriggerEvent_24:
        STRB     R0,[SP, #+20]
//  193 
//  194 	}
//  195 
//  196 #if EFI_UNIT_TEST
//  197 		if (printTriggerDebug) {
//  198 			printf("%s isSynchronizationPoint=%d index=%d %s\r\n",
//  199 					getTrigger_type_e(engineConfiguration->trigger.type),
//  200 					isSynchronizationPoint, current_index,
//  201 					getTrigger_event_e(signal));
//  202 		}
//  203 #endif
//  204 
//  205 	if (isSynchronizationPoint) {
??decodeTriggerEvent_19:
        LDRB     R0,[SP, #+20]
        CMP      R0,#+0
        BEQ.W    ??decodeTriggerEvent_25
//  206 
//  207 		/**
//  208 		 * We can check if things are fine by comparing the number of events in a cycle with the expected number of event.
//  209 		 */
//  210 		bool isDecodingError = eventCount[0] != TRIGGER_SHAPE(expectedEventCount[0])
//  211 				|| eventCount[1] != TRIGGER_SHAPE(expectedEventCount[1])
//  212 				|| eventCount[2] != TRIGGER_SHAPE(expectedEventCount[2]);
        LDR      R0,[R6, #+84]
        LDR.W    R1,??DataTable14_10
        LDR      R1,[R1, #+3564]
        CMP      R0,R1
        BNE.N    ??decodeTriggerEvent_26
        LDR      R0,[R6, #+88]
        LDR.W    R1,??DataTable14_10
        LDR      R1,[R1, #+3568]
        CMP      R0,R1
        BNE.N    ??decodeTriggerEvent_26
        LDR      R0,[R6, #+92]
        LDR.W    R1,??DataTable14_10
        LDR      R1,[R1, #+3572]
        CMP      R0,R1
        BEQ.N    ??decodeTriggerEvent_27
??decodeTriggerEvent_26:
        MOVS     R9,#+1
        B.N      ??decodeTriggerEvent_28
??decodeTriggerEvent_27:
        MOVS     R9,#+0
//  213 
//  214 		triggerDecoderErrorPin.setValue(isDecodingError);
??decodeTriggerEvent_28:
        UXTB     R9,R9            ;; ZeroExt  R9,R9,#+24,#+24
        MOV      R1,R9
        LDR.W    R0,??DataTable14_1
          CFI FunCall _ZN9OutputPin8setValueEi
        BL       _ZN9OutputPin8setValueEi
//  215 		if (isDecodingError) {
        UXTB     R9,R9            ;; ZeroExt  R9,R9,#+24,#+24
        CMP      R9,#+0
        BEQ.N    ??decodeTriggerEvent_29
//  216 			lastDecodingErrorTime = getTimeNowNt();
          CFI FunCall getTimeNowNt
        BL       getTimeNowNt
        LDR.W    R2,??DataTable14_17
        STRD     R0,R1,[R2, #+0]
//  217 			totalTriggerErrorCounter++;
        LDR      R0,[R6, #+60]
        ADDS     R0,R0,#+1
        STR      R0,[R6, #+60]
//  218 			if (engineConfiguration->isPrintTriggerSynchDetails) {
        LDR.W    R0,??DataTable14_7
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+3
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??decodeTriggerEvent_29
//  219 #if EFI_PROD_CODE
//  220 				scheduleMsg(logger, "error: synchronizationPoint @ index %d expected %d/%d/%d got %d/%d/%d", current_index,
//  221 						TRIGGER_SHAPE(expectedEventCount[0]), TRIGGER_SHAPE(expectedEventCount[1]),
//  222 						TRIGGER_SHAPE(expectedEventCount[2]), eventCount[0], eventCount[1], eventCount[2]);
        LDR      R0,[R6, #+92]
        STR      R0,[SP, #+16]
        LDR      R0,[R6, #+88]
        STR      R0,[SP, #+12]
        LDR      R0,[R6, #+84]
        STR      R0,[SP, #+8]
        LDR.W    R0,??DataTable14_10
        LDR      R0,[R0, #+3572]
        STR      R0,[SP, #+4]
        LDR.W    R0,??DataTable14_10
        LDR      R0,[R0, #+3568]
        STR      R0,[SP, #+0]
        LDR.W    R0,??DataTable14_10
        LDR      R3,[R0, #+3564]
        LDR      R2,[R6, #+76]
        LDR.W    R1,??DataTable14_18
        LDR.W    R0,??DataTable14_15
        LDR      R0,[R0, #+0]
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  223 #endif /* EFI_PROD_CODE */
//  224 			}
//  225 		}
//  226 
//  227 		errorDetection.add(isDecodingError);
??decodeTriggerEvent_29:
        UXTB     R9,R9            ;; ZeroExt  R9,R9,#+24,#+24
        MOV      R1,R9
        LDR.W    R0,??DataTable14_2
          CFI FunCall _ZN13cyclic_bufferIiE3addEi
        BL       _ZN13cyclic_bufferIiE3addEi
//  228 
//  229 		if (isTriggerDecoderError()) {
          CFI FunCall _Z21isTriggerDecoderErrorv
        BL       _Z21isTriggerDecoderErrorv
        CMP      R0,#+0
        BEQ.N    ??decodeTriggerEvent_30
//  230 			warning(OBD_PCM_Processor_Fault, "trigger decoding issue. expected %d/%d/%d got %d/%d/%d",
//  231 					TRIGGER_SHAPE(expectedEventCount[0]), TRIGGER_SHAPE(expectedEventCount[1]),
//  232 					TRIGGER_SHAPE(expectedEventCount[2]), eventCount[0], eventCount[1], eventCount[2]);
        LDR      R0,[R6, #+92]
        STR      R0,[SP, #+12]
        LDR      R0,[R6, #+88]
        STR      R0,[SP, #+8]
        LDR      R0,[R6, #+84]
        STR      R0,[SP, #+4]
        LDR.W    R0,??DataTable14_10
        LDR      R0,[R0, #+3572]
        STR      R0,[SP, #+0]
        LDR.W    R0,??DataTable14_10
        LDR      R3,[R0, #+3568]
        LDR.W    R0,??DataTable14_10
        LDR      R2,[R0, #+3564]
        LDR.W    R1,??DataTable14_19
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  233 		}
//  234 
//  235 		shaft_is_synchronized = true;
??decodeTriggerEvent_30:
        MOVS     R0,#+1
        STRB     R0,[R6, #+4]
//  236 		// this call would update duty cycle values
//  237 		nextTriggerEvent()
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        ADDS     R12,R6,R7, LSL #+3
        LDRD     R2,R3,[R12, #+128]
        MOVS     R0,R2
        MOVS     R1,R3
        CMP      R1,#+0
        BNE.N    ??decodeTriggerEvent_31
        CMP      R0,#+0
        BEQ.N    ??decodeTriggerEvent_32
??decodeTriggerEvent_31:
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        ADDS     R2,R6,R7, LSL #+2
        LDR      R2,[R2, #+24]
        ADDS     R2,R4,R2
        SUBS     R2,R2,R0
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        ADDS     R3,R6,R7, LSL #+2
        STR      R2,[R3, #+24]
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        ADDS     R12,R6,R7, LSL #+3
        MOVS     R2,#+0
        MOVS     R3,#+0
        STRD     R2,R3,[R12, #+128]
        B.N      ??decodeTriggerEvent_33
??decodeTriggerEvent_32:
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        ADDS     R2,R6,R7, LSL #+3
        STRD     R4,R5,[R2, #+128]
??decodeTriggerEvent_33:
        LDR.W    R2,??DataTable14_7
        LDR      R2,[R2, #+0]
        LDRB     R2,[R2, #+1509]
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        LSRS     R2,R2,#+6
        ANDS     R2,R2,#0x1
        CMP      R2,#+0
        BEQ.N    ??decodeTriggerEvent_34
        LDR      R2,[R6, #+76]
        ADDS     R2,R2,#+1
        STR      R2,[R6, #+76]
??decodeTriggerEvent_34:
        LDR      R2,[R6, #+76]
        ADDS     R2,R2,#+1
        STR      R2,[R6, #+76]
//  238 		;
//  239 
//  240 		nextRevolution();
        LDR      R2,[R6, #+0]
        CMP      R2,#+0
        BEQ.N    ??decodeTriggerEvent_35
        MOVS     R0,R6
        LDR      R1,[R6, #+0]
          CFI FunCall
        BLX      R1
??decodeTriggerEvent_35:
        MOVS     R2,#+12
        ADDS     R1,R6,#+24
        ADDS     R0,R6,#+36
          CFI FunCall memcpy
        BL       memcpy
        LDRD     R0,R1,[R6, #+176]
        SUBS     R0,R4,R0
        SBCS     R1,R5,R1
        STRD     R0,R1,[R6, #+168]
        STRD     R4,R5,[R6, #+176]
        MOVS     R0,R6
          CFI FunCall _ZN12TriggerState5clearEv
        BL       _ZN12TriggerState5clearEv
        LDR      R0,[R6, #+160]
        ADDS     R0,R0,#+1
        STR      R0,[R6, #+160]
        LDR      R0,[R6, #+80]
        ADDS     R0,R0,#+1
        STR      R0,[R6, #+80]
        LDRD     R2,R3,[R6, #+152]
        LDR.W    R0,??DataTable14_16
        LDR      R0,[R0, #+0]
        ASRS     R1,R0,#+31
        ADDS     R0,R2,R0
        ADCS     R1,R3,R1
        STRD     R0,R1,[R6, #+152]
        B.N      ??decodeTriggerEvent_36
//  241 	} else {
//  242 		nextTriggerEvent()
??decodeTriggerEvent_25:
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        ADDS     R12,R6,R7, LSL #+3
        LDRD     R2,R3,[R12, #+128]
        MOVS     R0,R2
        MOVS     R1,R3
        CMP      R1,#+0
        BNE.N    ??decodeTriggerEvent_37
        CMP      R0,#+0
        BEQ.N    ??decodeTriggerEvent_38
??decodeTriggerEvent_37:
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        ADDS     R2,R6,R7, LSL #+2
        LDR      R2,[R2, #+24]
        ADDS     R2,R4,R2
        SUBS     R2,R2,R0
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        ADDS     R3,R6,R7, LSL #+2
        STR      R2,[R3, #+24]
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        ADDS     R12,R6,R7, LSL #+3
        MOVS     R2,#+0
        MOVS     R3,#+0
        STRD     R2,R3,[R12, #+128]
        B.N      ??decodeTriggerEvent_39
??decodeTriggerEvent_38:
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        ADDS     R2,R6,R7, LSL #+3
        STRD     R4,R5,[R2, #+128]
??decodeTriggerEvent_39:
        LDR.W    R2,??DataTable14_7
        LDR      R2,[R2, #+0]
        LDRB     R2,[R2, #+1509]
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        LSRS     R2,R2,#+6
        ANDS     R2,R2,#0x1
        CMP      R2,#+0
        BEQ.N    ??decodeTriggerEvent_40
        LDR      R2,[R6, #+76]
        ADDS     R2,R2,#+1
        STR      R2,[R6, #+76]
??decodeTriggerEvent_40:
        LDR      R2,[R6, #+76]
        ADDS     R2,R2,#+1
        STR      R2,[R6, #+76]
//  243 		;
//  244 	}
//  245 
//  246 	toothed_previous_duration = currentDuration;
??decodeTriggerEvent_36:
        LDR      R0,[R6, #+12]
        STR      R0,[R6, #+8]
//  247 	toothed_previous_time = nowNt;
        STRD     R4,R5,[R6, #+16]
//  248 }
??decodeTriggerEvent_1:
        ADD      SP,SP,#+24
          CFI CFA R13+48
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+40
        POP      {R0,R4-R11,PC}   ;; return
          CFI EndBlock cfiBlock13
//  249 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _Z14getEngineCycle16operation_mode_e
          CFI NoCalls
        THUMB
//  250 float getEngineCycle(operation_mode_e operationMode) {
//  251 	return operationMode == TWO_STROKE ? 360 : 720;
_Z14getEngineCycle16operation_mode_e:
        CMP      R0,#+3
        BNE.N    ??getEngineCycle_0
        VLDR.W   S0,??DataTable8  ;; 0x168
        B.N      ??getEngineCycle_1
??getEngineCycle_0:
        VLDR.W   S0,??DataTable8_1  ;; 0x2d0
??getEngineCycle_1:
        VCVT.F32.S32 S0,S0
        BX       LR               ;; return
          CFI EndBlock cfiBlock14
//  252 }
//  253 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function _Z28addSkippedToothTriggerEvents15trigger_wheel_eP12TriggerShapeiifffff
        THUMB
//  254 void addSkippedToothTriggerEvents(trigger_wheel_e wheel, TriggerShape *s,
//  255 		int totalTeethCount, int skippedCount,
//  256 		float toothWidth,
//  257 		float offset, float engineCycle, float filterLeft, float filterRight) {
_Z28addSkippedToothTriggerEvents15trigger_wheel_eP12TriggerShapeiifffff:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        VPUSH    {D8-D11}
          CFI D11 Frame(CFA, -32)
          CFI D10 Frame(CFA, -40)
          CFI D9 Frame(CFA, -48)
          CFI D8 Frame(CFA, -56)
          CFI CFA R13+56
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
        VMOV.F32 S18,S0
        VMOV.F32 S19,S1
        VMOV.F32 S20,S2
        VMOV.F32 S16,S3
        VMOV.F32 S17,S4
//  258 	efiAssertVoid(totalTeethCount > 0, "total count");
        CMP      R6,#+1
        BGE.N    ??addSkippedToothTriggerEvents_0
        LDR.W    R0,??DataTable14_20
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??addSkippedToothTriggerEvents_1
//  259 	efiAssertVoid(skippedCount >= 0, "skipped count");
??addSkippedToothTriggerEvents_0:
        CMP      R7,#+0
        BPL.N    ??addSkippedToothTriggerEvents_2
        LDR.W    R0,??DataTable14_21
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??addSkippedToothTriggerEvents_1
//  260 
//  261 	for (int i = 0; i < totalTeethCount - skippedCount - 1; i++) {
??addSkippedToothTriggerEvents_2:
        MOVS     R8,#+0
??addSkippedToothTriggerEvents_3:
        SUBS     R0,R6,R7
        SUBS     R0,R0,#+1
        CMP      R8,R0
        BGE.N    ??addSkippedToothTriggerEvents_4
//  262 		float angleDown = engineCycle / totalTeethCount * (i + (1 - toothWidth));
        VMOV     S0,R6
        VCVT.F32.S32 S0,S0
        VDIV.F32 S0,S20,S0
        VMOV     S1,R8
        VCVT.F32.S32 S1,S1
        VMOV.F32 S2,#1.0
        VSUB.F32 S2,S2,S18
        VADD.F32 S1,S1,S2
        VMUL.F32 S21,S0,S1
//  263 		float angleUp = engineCycle / totalTeethCount * (i + 1);
        VMOV     S0,R6
        VCVT.F32.S32 S0,S0
        VDIV.F32 S0,S20,S0
        ADDS     R0,R8,#+1
        VMOV     S1,R0
        VCVT.F32.S32 S1,S1
        VMUL.F32 S22,S0,S1
//  264 		s->addEvent(offset + angleDown, wheel, TV_HIGH, filterLeft, filterRight);
        VMOV.F32 S2,S17
        VMOV.F32 S1,S16
        MOVS     R2,#+1
        MOVS     R1,R4
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        VADD.F32 S0,S19,S21
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_eff
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_eff
//  265 		s->addEvent(offset + angleUp, wheel, TV_LOW, filterLeft, filterRight);
        VMOV.F32 S2,S17
        VMOV.F32 S1,S16
        MOVS     R2,#+0
        MOVS     R1,R4
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        VADD.F32 S0,S19,S22
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_eff
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_eff
//  266 	}
        ADDS     R8,R8,#+1
        B.N      ??addSkippedToothTriggerEvents_3
//  267 
//  268 	float angleDown = engineCycle / totalTeethCount * (totalTeethCount - skippedCount - 1 + (1 - toothWidth) );
??addSkippedToothTriggerEvents_4:
        VMOV     S0,R6
        VCVT.F32.S32 S0,S0
        VDIV.F32 S0,S20,S0
        SUBS     R0,R6,R7
        SUBS     R0,R0,#+1
        VMOV     S1,R0
        VCVT.F32.S32 S1,S1
        VMOV.F32 S2,#1.0
        VSUB.F32 S2,S2,S18
        VADD.F32 S1,S1,S2
        VMUL.F32 S21,S0,S1
//  269 	s->addEvent(offset + angleDown, wheel, TV_HIGH, filterLeft, filterRight);
        VMOV.F32 S2,S17
        VMOV.F32 S1,S16
        MOVS     R2,#+1
        MOVS     R1,R4
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        VADD.F32 S0,S19,S21
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_eff
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_eff
//  270 	s->addEvent(offset + engineCycle, wheel, TV_LOW, filterLeft, filterRight);
        VMOV.F32 S2,S17
        VMOV.F32 S1,S16
        MOVS     R2,#+0
        MOVS     R1,R4
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        VADD.F32 S0,S19,S20
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_eff
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_eff
//  271 }
??addSkippedToothTriggerEvents_1:
        VPOP     {D8-D11}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI D10 SameValue
          CFI D11 SameValue
          CFI CFA R13+24
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock15
//  272 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function _Z37initializeSkippedToothTriggerShapeExtP12TriggerShapeii16operation_mode_e
        THUMB
//  273 void initializeSkippedToothTriggerShapeExt(TriggerShape *s, int totalTeethCount, int skippedCount,
//  274 		operation_mode_e operationMode) {
_Z37initializeSkippedToothTriggerShapeExtP12TriggerShapeii16operation_mode_e:
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
//  275 	efiAssertVoid(totalTeethCount > 0, "totalTeethCount is zero");
        CMP      R5,#+1
        BGE.N    ??initializeSkippedToothTriggerShapeExt_0
        LDR.W    R0,??DataTable14_22
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??initializeSkippedToothTriggerShapeExt_1
//  276 
//  277 	s->totalToothCount = totalTeethCount;
??initializeSkippedToothTriggerShapeExt_0:
        STR      R5,[R4, #+4]
//  278 	s->skippedToothCount = skippedCount;
        STR      R6,[R4, #+8]
//  279 
//  280 	s->setTriggerSynchronizationGap(skippedCount + 1);
        ADDS     R0,R6,#+1
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape28setTriggerSynchronizationGapEf
        BL       _ZN12TriggerShape28setTriggerSynchronizationGapEf
//  281 	s->isSynchronizationNeeded = (skippedCount != 0);
        CMP      R6,#+0
        BEQ.N    ??initializeSkippedToothTriggerShapeExt_2
        MOVS     R0,#+1
        B.N      ??initializeSkippedToothTriggerShapeExt_3
??initializeSkippedToothTriggerShapeExt_2:
        MOVS     R0,#+0
??initializeSkippedToothTriggerShapeExt_3:
        STRB     R0,[R4, #+0]
//  282 
//  283 	efiAssertVoid(s != NULL, "TriggerShape is NULL");
        CMP      R4,#+0
        BNE.N    ??initializeSkippedToothTriggerShapeExt_4
        LDR.W    R0,??DataTable14_23
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??initializeSkippedToothTriggerShapeExt_1
//  284 	s->reset(operationMode, false);
??initializeSkippedToothTriggerShapeExt_4:
        MOVS     R2,#+0
        MOVS     R1,R7
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape5resetE16operation_mode_eb
        BL       _ZN12TriggerShape5resetE16operation_mode_eb
//  285 
//  286 	addSkippedToothTriggerEvents(T_PRIMARY, s, totalTeethCount, skippedCount, 0.5, 0, getEngineCycle(operationMode),
//  287 			NO_LEFT_FILTER, NO_RIGHT_FILTER);
        MOVS     R0,R7
          CFI FunCall _Z14getEngineCycle16operation_mode_e
        BL       _Z14getEngineCycle16operation_mode_e
        VMOV.F32 S2,S0
        VLDR.W   S4,??DataTable9  ;; 0x447a0000
        VMOV.F32 S3,#-1.0
        VLDR.W   S1,??DataTable9_1  ;; 0x0
        VMOV.F32 S0,#0.5
        MOVS     R3,R6
        MOVS     R2,R5
        MOVS     R1,R4
        MOVS     R0,#+0
          CFI FunCall _Z28addSkippedToothTriggerEvents15trigger_wheel_eP12TriggerShapeiifffff
        BL       _Z28addSkippedToothTriggerEvents15trigger_wheel_eP12TriggerShapeiifffff
//  288 }
??initializeSkippedToothTriggerShapeExt_1:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock16
//  289 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function _Z19configureOnePlusOneP12TriggerShape16operation_mode_e
        THUMB
//  290 static void configureOnePlusOne(TriggerShape *s, operation_mode_e operationMode) {
_Z19configureOnePlusOneP12TriggerShape16operation_mode_e:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        VPUSH    {D8}
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  291 	float engineCycle = getEngineCycle(operationMode);
        MOVS     R0,R5
          CFI FunCall _Z14getEngineCycle16operation_mode_e
        BL       _Z14getEngineCycle16operation_mode_e
        VMOV.F32 S16,S0
//  292 
//  293 	s->reset(FOUR_STROKE_CAM_SENSOR, true);
        MOVS     R2,#+1
        MOVS     R1,#+2
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape5resetE16operation_mode_eb
        BL       _ZN12TriggerShape5resetE16operation_mode_eb
//  294 
//  295 	s->addEvent(180, T_PRIMARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable9_2  ;; 0x43340000
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  296 	s->addEvent(360, T_PRIMARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable9_3  ;; 0x43b40000
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  297 
//  298 	s->addEvent(540, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        VLDR.W   S0,??DataTable9_4  ;; 0x44070000
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  299 	s->addEvent(720, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        VLDR.W   S0,??DataTable9_5  ;; 0x44340000
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  300 
//  301 	s->isSynchronizationNeeded = false;
        MOVS     R0,#+0
        STRB     R0,[R4, #+0]
//  302 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock17
//  303 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function _Z20configureOnePlus60_2P12TriggerShape16operation_mode_e
        THUMB
//  304 static void configureOnePlus60_2(TriggerShape *s, operation_mode_e operationMode) {
_Z20configureOnePlus60_2P12TriggerShape16operation_mode_e:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  305 	s->reset(FOUR_STROKE_CAM_SENSOR, true);
        MOVS     R2,#+1
        MOVS     R1,#+2
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape5resetE16operation_mode_eb
        BL       _ZN12TriggerShape5resetE16operation_mode_eb
//  306 
//  307 	int totalTeethCount = 60;
        MOVS     R6,#+60
//  308 	int skippedCount = 2;
        MOVS     R7,#+2
//  309 
//  310 	s->addEvent(2, T_PRIMARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+0
        VMOV.F32 S0,#2.0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  311 	addSkippedToothTriggerEvents(T_SECONDARY, s, totalTeethCount, skippedCount, 0.5, 0, 360, 2, 20);
        VMOV.F32 S4,#20.0
        VMOV.F32 S3,#2.0
        VLDR.W   S2,??DataTable9_3  ;; 0x43b40000
        VLDR.W   S1,??DataTable9_1  ;; 0x0
        VMOV.F32 S0,#0.5
        MOVS     R3,R7
        MOVS     R2,R6
        MOVS     R1,R4
        MOVS     R0,#+1
          CFI FunCall _Z28addSkippedToothTriggerEvents15trigger_wheel_eP12TriggerShapeiifffff
        BL       _Z28addSkippedToothTriggerEvents15trigger_wheel_eP12TriggerShapeiifffff
//  312 	s->addEvent(20, T_PRIMARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+0
        VMOV.F32 S0,#20.0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  313 	addSkippedToothTriggerEvents(T_SECONDARY, s, totalTeethCount, skippedCount, 0.5, 0, 360, 20, NO_RIGHT_FILTER);
        VLDR.W   S4,??DataTable9  ;; 0x447a0000
        VMOV.F32 S3,#20.0
        VLDR.W   S2,??DataTable9_3  ;; 0x43b40000
        VLDR.W   S1,??DataTable9_1  ;; 0x0
        VMOV.F32 S0,#0.5
        MOVS     R3,R7
        MOVS     R2,R6
        MOVS     R1,R4
        MOVS     R0,#+1
          CFI FunCall _Z28addSkippedToothTriggerEvents15trigger_wheel_eP12TriggerShapeiifffff
        BL       _Z28addSkippedToothTriggerEvents15trigger_wheel_eP12TriggerShapeiifffff
//  314 
//  315 	addSkippedToothTriggerEvents(T_SECONDARY, s, totalTeethCount, skippedCount, 0.5, 360, 360, NO_LEFT_FILTER,
//  316 			NO_RIGHT_FILTER);
        VLDR.W   S4,??DataTable9  ;; 0x447a0000
        VMOV.F32 S3,#-1.0
        VLDR.W   S2,??DataTable9_3  ;; 0x43b40000
        VLDR.W   S1,??DataTable9_3  ;; 0x43b40000
        VMOV.F32 S0,#0.5
        MOVS     R3,R7
        MOVS     R2,R6
        MOVS     R1,R4
        MOVS     R0,#+1
          CFI FunCall _Z28addSkippedToothTriggerEvents15trigger_wheel_eP12TriggerShapeiifffff
        BL       _Z28addSkippedToothTriggerEvents15trigger_wheel_eP12TriggerShapeiifffff
//  317 
//  318 	s->isSynchronizationNeeded = false;
        MOVS     R0,#+0
        STRB     R0,[R4, #+0]
//  319 }
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock18

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8:
        DC32     0x168

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_1:
        DC32     0x2d0
//  320 
//  321 /**
//  322  * External logger is needed because at this point our logger is not yet initialized
//  323  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock19 Using cfiCommon0
          CFI Function _ZN12TriggerShape22initializeTriggerShapeEP7Logging
        THUMB
//  324 void TriggerShape::initializeTriggerShape(Logging *logger DECLARE_ENGINE_PARAMETER_S) {
_ZN12TriggerShape22initializeTriggerShapeEP7Logging:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  325 	TriggerShape *triggerShape = this;
        MOVS     R6,R4
//  326 
//  327 #if EFI_PROD_CODE
//  328 	scheduleMsg(logger, "initializeTriggerShape()");
        LDR.W    R1,??DataTable14_24
        MOVS     R0,R5
          CFI FunCall scheduleMsg
        BL       scheduleMsg
//  329 #endif
//  330 	const trigger_config_s *triggerConfig = &engineConfiguration->trigger;
        LDR.W    R0,??DataTable14_7
        LDR      R0,[R0, #+0]
        ADDS     R7,R0,#+536
//  331 
//  332 	clear();
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape5clearEv
        BL       _ZN12TriggerShape5clearEv
//  333 
//  334 	switch (triggerConfig->type) {
        LDR      R0,[R7, #+0]
        CMP      R0,#+0
        BEQ.N    ??initializeTriggerShape_0
        CMP      R0,#+2
        BEQ.N    ??initializeTriggerShape_1
        BCC.N    ??initializeTriggerShape_2
        CMP      R0,#+4
        BEQ.N    ??initializeTriggerShape_3
        BCC.N    ??initializeTriggerShape_4
        CMP      R0,#+6
        BEQ.N    ??initializeTriggerShape_5
        BCC.N    ??initializeTriggerShape_6
        CMP      R0,#+8
        BEQ.N    ??initializeTriggerShape_7
        BCC.N    ??initializeTriggerShape_8
        CMP      R0,#+10
        BEQ.W    ??initializeTriggerShape_9
        BCC.N    ??initializeTriggerShape_10
        CMP      R0,#+12
        BEQ.N    ??initializeTriggerShape_11
        BCC.W    ??initializeTriggerShape_12
        CMP      R0,#+14
        BEQ.N    ??initializeTriggerShape_13
        BCC.W    ??initializeTriggerShape_14
        CMP      R0,#+16
        BEQ.N    ??initializeTriggerShape_15
        BCC.N    ??initializeTriggerShape_16
        CMP      R0,#+18
        BEQ.N    ??initializeTriggerShape_17
        BCC.N    ??initializeTriggerShape_18
        CMP      R0,#+20
        BEQ.N    ??initializeTriggerShape_19
        BCC.W    ??initializeTriggerShape_20
        CMP      R0,#+21
        BEQ.N    ??initializeTriggerShape_21
        B.N      ??initializeTriggerShape_22
//  335 
//  336 	case TT_TOOTHED_WHEEL:
//  337 		initializeSkippedToothTriggerShapeExt(triggerShape, triggerConfig->customTotalToothCount,
//  338 				triggerConfig->customSkippedToothCount, engineConfiguration->operationMode);
??initializeTriggerShape_0:
        LDR.W    R0,??DataTable14_7
        LDR      R0,[R0, #+0]
        LDR      R3,[R0, #+500]
        LDR      R2,[R7, #+12]
        LDR      R1,[R7, #+8]
        MOVS     R0,R6
          CFI FunCall _Z37initializeSkippedToothTriggerShapeExtP12TriggerShapeii16operation_mode_e
        BL       _Z37initializeSkippedToothTriggerShapeExtP12TriggerShapeii16operation_mode_e
//  339 		break;
        B.N      ??initializeTriggerShape_23
//  340 
//  341 	case TT_MAZDA_MIATA_NA:
//  342 		initializeMazdaMiataNaShape(triggerShape);
??initializeTriggerShape_4:
        MOVS     R0,R6
          CFI FunCall _Z27initializeMazdaMiataNaShapeP12TriggerShape
        BL       _Z27initializeMazdaMiataNaShapeP12TriggerShape
//  343 		break;
        B.N      ??initializeTriggerShape_23
//  344 
//  345 	case TT_MAZDA_MIATA_NB:
//  346 		initializeMazdaMiataNbShape(triggerShape);
??initializeTriggerShape_3:
        MOVS     R0,R6
          CFI FunCall _Z27initializeMazdaMiataNbShapeP12TriggerShape
        BL       _Z27initializeMazdaMiataNbShapeP12TriggerShape
//  347 		break;
        B.N      ??initializeTriggerShape_23
//  348 
//  349 	case TT_DODGE_NEON_1995:
//  350 		configureNeon1995TriggerShape(triggerShape);
??initializeTriggerShape_1:
        MOVS     R0,R6
          CFI FunCall _Z29configureNeon1995TriggerShapeP12TriggerShape
        BL       _Z29configureNeon1995TriggerShapeP12TriggerShape
//  351 		break;
        B.N      ??initializeTriggerShape_23
//  352 
//  353 	case TT_DODGE_NEON_2003:
//  354 		configureNeon2003TriggerShape(triggerShape);
??initializeTriggerShape_13:
        MOVS     R0,R6
          CFI FunCall _Z29configureNeon2003TriggerShapeP12TriggerShape
        BL       _Z29configureNeon2003TriggerShapeP12TriggerShape
//  355 		break;
        B.N      ??initializeTriggerShape_23
//  356 
//  357 	case TT_FORD_ASPIRE:
//  358 		configureFordAspireTriggerShape(triggerShape);
??initializeTriggerShape_2:
        MOVS     R0,R6
          CFI FunCall _Z31configureFordAspireTriggerShapeP12TriggerShape
        BL       _Z31configureFordAspireTriggerShapeP12TriggerShape
//  359 		break;
        B.N      ??initializeTriggerShape_23
//  360 
//  361 	case TT_GM_7X:
//  362 		// todo: fix this configureGmTriggerShape(triggerShape);
//  363 		configureFordAspireTriggerShape(triggerShape);
??initializeTriggerShape_6:
        MOVS     R0,R6
          CFI FunCall _Z31configureFordAspireTriggerShapeP12TriggerShape
        BL       _Z31configureFordAspireTriggerShapeP12TriggerShape
//  364 		break;
        B.N      ??initializeTriggerShape_23
//  365 
//  366 	case TT_MAZDA_DOHC_1_4:
//  367 		configureMazdaProtegeLx(triggerShape);
??initializeTriggerShape_16:
        MOVS     R0,R6
          CFI FunCall _Z23configureMazdaProtegeLxP12TriggerShape
        BL       _Z23configureMazdaProtegeLxP12TriggerShape
//  368 		break;
        B.N      ??initializeTriggerShape_23
//  369 
//  370 	case TT_ONE_PLUS_ONE:
//  371 		configureOnePlusOne(triggerShape, engineConfiguration->operationMode);
??initializeTriggerShape_15:
        LDR.W    R0,??DataTable14_7
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+500]
        MOVS     R0,R6
          CFI FunCall _Z19configureOnePlusOneP12TriggerShape16operation_mode_e
        BL       _Z19configureOnePlusOneP12TriggerShape16operation_mode_e
//  372 		break;
        B.N      ??initializeTriggerShape_23
//  373 
//  374 	case TT_ONE_PLUS_TOOTHED_WHEEL_60_2:
//  375 		configureOnePlus60_2(triggerShape, engineConfiguration->operationMode);
??initializeTriggerShape_18:
        LDR.W    R0,??DataTable14_7
        LDR      R0,[R0, #+0]
        LDR      R1,[R0, #+500]
        MOVS     R0,R6
          CFI FunCall _Z20configureOnePlus60_2P12TriggerShape16operation_mode_e
        BL       _Z20configureOnePlus60_2P12TriggerShape16operation_mode_e
//  376 		break;
        B.N      ??initializeTriggerShape_23
//  377 
//  378 	case TT_ONE:
//  379 		setToothedWheelConfiguration(triggerShape, 1, 0, engineConfiguration->operationMode);
??initializeTriggerShape_17:
        LDR.W    R0,??DataTable14_7
        LDR      R0,[R0, #+0]
        LDR      R3,[R0, #+500]
        MOVS     R2,#+0
        MOVS     R1,#+1
        MOVS     R0,R6
          CFI FunCall _Z28setToothedWheelConfigurationP12TriggerShapeii16operation_mode_e
        BL       _Z28setToothedWheelConfigurationP12TriggerShapeii16operation_mode_e
//  380 		break;
        B.N      ??initializeTriggerShape_23
//  381 
//  382 	case TT_MAZDA_SOHC_4:
//  383 		configureMazdaProtegeSOHC(triggerShape);
??initializeTriggerShape_8:
        MOVS     R0,R6
          CFI FunCall _Z25configureMazdaProtegeSOHCP12TriggerShape
        BL       _Z25configureMazdaProtegeSOHCP12TriggerShape
//  384 		break;
        B.N      ??initializeTriggerShape_23
//  385 
//  386 	case TT_MINI_COOPER_R50:
//  387 		configureMiniCooperTriggerShape(triggerShape);
??initializeTriggerShape_5:
        MOVS     R0,R6
          CFI FunCall _Z31configureMiniCooperTriggerShapeP12TriggerShape
        BL       _Z31configureMiniCooperTriggerShapeP12TriggerShape
//  388 		break;
        B.N      ??initializeTriggerShape_23
//  389 
//  390 	case TT_TOOTHED_WHEEL_60_2:
//  391 		setToothedWheelConfiguration(triggerShape, 60, 2, engineConfiguration->operationMode);
??initializeTriggerShape_7:
        LDR.W    R0,??DataTable14_7
        LDR      R0,[R0, #+0]
        LDR      R3,[R0, #+500]
        MOVS     R2,#+2
        MOVS     R1,#+60
        MOVS     R0,R6
          CFI FunCall _Z28setToothedWheelConfigurationP12TriggerShapeii16operation_mode_e
        BL       _Z28setToothedWheelConfigurationP12TriggerShapeii16operation_mode_e
//  392 		break;
        B.N      ??initializeTriggerShape_23
//  393 
//  394 	case TT_60_2_VW:
//  395 		setVwConfiguration(triggerShape);
??initializeTriggerShape_19:
        MOVS     R0,R6
          CFI FunCall _Z18setVwConfigurationP12TriggerShape
        BL       _Z18setVwConfigurationP12TriggerShape
//  396 		break;
        B.N      ??initializeTriggerShape_23
//  397 
//  398 	case TT_TOOTHED_WHEEL_36_1:
//  399 		setToothedWheelConfiguration(triggerShape, 36, 1, engineConfiguration->operationMode);
??initializeTriggerShape_10:
        LDR.W    R0,??DataTable14_7
        LDR      R0,[R0, #+0]
        LDR      R3,[R0, #+500]
        MOVS     R2,#+1
        MOVS     R1,#+36
        MOVS     R0,R6
          CFI FunCall _Z28setToothedWheelConfigurationP12TriggerShapeii16operation_mode_e
        BL       _Z28setToothedWheelConfigurationP12TriggerShapeii16operation_mode_e
//  400 		break;
        B.N      ??initializeTriggerShape_23
//  401 
//  402 	case TT_HONDA_ACCORD_CD_TWO_WIRES:
//  403 		configureHondaAccordCD(triggerShape, false, true, T_CHANNEL_3, T_PRIMARY, 0);
??initializeTriggerShape_11:
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        VLDR.W   S0,??DataTable9_1  ;; 0x0
        MOVS     R3,#+2
        MOVS     R2,#+1
        MOVS     R1,#+0
        MOVS     R0,R6
          CFI FunCall _Z22configureHondaAccordCDP12TriggerShapebb15trigger_wheel_eS1_f
        BL       _Z22configureHondaAccordCDP12TriggerShapebb15trigger_wheel_eS1_f
//  404 		break;
        B.N      ??initializeTriggerShape_23
//  405 
//  406 	case TT_HONDA_ACCORD_CD:
//  407 		configureHondaAccordCD(triggerShape, true, true, T_CHANNEL_3, T_PRIMARY, 0);
??initializeTriggerShape_9:
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        VLDR.W   S0,??DataTable9_1  ;; 0x0
        MOVS     R3,#+2
        MOVS     R2,#+1
        MOVS     R1,#+1
        MOVS     R0,R6
          CFI FunCall _Z22configureHondaAccordCDP12TriggerShapebb15trigger_wheel_eS1_f
        BL       _Z22configureHondaAccordCDP12TriggerShapebb15trigger_wheel_eS1_f
//  408 		break;
        B.N      ??initializeTriggerShape_23
//  409 
//  410 	case TT_HONDA_ACCORD_1_24:
//  411 		configureHondaAccordCD(triggerShape, true, false, T_PRIMARY, T_PRIMARY, 10);
??initializeTriggerShape_21:
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        VMOV.F32 S0,#10.0
        MOVS     R3,#+0
        MOVS     R2,#+0
        MOVS     R1,#+1
        MOVS     R0,R6
          CFI FunCall _Z22configureHondaAccordCDP12TriggerShapebb15trigger_wheel_eS1_f
        BL       _Z22configureHondaAccordCDP12TriggerShapebb15trigger_wheel_eS1_f
//  412 		break;
        B.N      ??initializeTriggerShape_23
//  413 
//  414 	case TT_HONDA_ACCORD_CD_DIP:
//  415 		configureHondaAccordCDDip(triggerShape);
??initializeTriggerShape_14:
        MOVS     R0,R6
          CFI FunCall _Z25configureHondaAccordCDDipP12TriggerShape
        BL       _Z25configureHondaAccordCDDipP12TriggerShape
//  416 		break;
        B.N      ??initializeTriggerShape_23
//  417 
//  418 	case TT_MITSU:
//  419 		initializeMitsubishi4g18(triggerShape);
??initializeTriggerShape_12:
        MOVS     R0,R6
          CFI FunCall _Z24initializeMitsubishi4g18P12TriggerShape
        BL       _Z24initializeMitsubishi4g18P12TriggerShape
//  420 		break;
        B.N      ??initializeTriggerShape_23
//  421 
//  422 	case TT_DODGE_RAM:
//  423 		initDodgeRam(triggerShape);
??initializeTriggerShape_20:
        MOVS     R0,R6
          CFI FunCall _Z12initDodgeRamP12TriggerShape
        BL       _Z12initDodgeRamP12TriggerShape
//  424 		break;
        B.N      ??initializeTriggerShape_23
//  425 
//  426 	default:
//  427 		firmwareError("initializeTriggerShape() not implemented: %d", triggerConfig->type);
??initializeTriggerShape_22:
        LDR      R1,[R7, #+0]
        LDR.W    R0,??DataTable14_25
          CFI FunCall firmwareError
        BL       firmwareError
//  428 		;
//  429 		return;
        B.N      ??initializeTriggerShape_24
//  430 	}
//  431 	wave.checkSwitchTimes(getSize());
??initializeTriggerShape_23:
        MOVS     R0,R4
          CFI FunCall _ZNK12TriggerShape7getSizeEv
        BL       _ZNK12TriggerShape7getSizeEv
        MOVS     R1,R0
        ADDW     R0,R4,#+2932
          CFI FunCall _ZN12multi_wave_s16checkSwitchTimesEi
        BL       _ZN12multi_wave_s16checkSwitchTimesEi
//  432 	calculateTriggerSynchPoint(PASS_ENGINE_PARAMETER_F);
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape26calculateTriggerSynchPointEv
        BL       _ZN12TriggerShape26calculateTriggerSynchPointEv
//  433 }
??initializeTriggerShape_24:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock19

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9:
        DC32     0x447a0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_1:
        DC32     0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_2:
        DC32     0x43340000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_3:
        DC32     0x43b40000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_4:
        DC32     0x44070000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable9_5:
        DC32     0x44340000
//  434 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock20 Using cfiCommon0
          CFI Function _ZN23TriggerStimulatorHelperC1Ev
          CFI NoCalls
        THUMB
//  435 TriggerStimulatorHelper::TriggerStimulatorHelper() {
//  436 }
_ZN23TriggerStimulatorHelperC1Ev:
        BX       LR               ;; return
          CFI EndBlock cfiBlock20
//  437 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock21 Using cfiCommon0
          CFI Function _ZN23TriggerStimulatorHelper8nextStepEP12TriggerStateP12TriggerShapeiPK16trigger_config_s
        THUMB
//  438 void TriggerStimulatorHelper::nextStep(TriggerState *state, TriggerShape * shape, int i,
//  439 		trigger_config_s const*triggerConfig DECLARE_ENGINE_PARAMETER_S) {
_ZN23TriggerStimulatorHelper8nextStepEP12TriggerStateP12TriggerShapeiPK16trigger_config_s:
        PUSH     {R0,R3-R11,LR}
          CFI R14 Frame(CFA, -4)
          CFI R11 Frame(CFA, -8)
          CFI R10 Frame(CFA, -12)
          CFI R9 Frame(CFA, -16)
          CFI R8 Frame(CFA, -20)
          CFI R7 Frame(CFA, -24)
          CFI R6 Frame(CFA, -28)
          CFI R5 Frame(CFA, -32)
          CFI R4 Frame(CFA, -36)
          CFI CFA R13+44
        SUB      SP,SP,#+4
          CFI CFA R13+48
        VPUSH    {D8}
          CFI D8 Frame(CFA, -56)
          CFI CFA R13+56
        SUB      SP,SP,#+8
          CFI CFA R13+64
        MOV      R11,R1
        MOVS     R4,R2
//  440 	int stateIndex = i % shape->getSize();
        MOVS     R0,R4
          CFI FunCall _ZNK12TriggerShape7getSizeEv
        BL       _ZNK12TriggerShape7getSizeEv
        LDR      R1,[SP, #+24]
        SDIV     R2,R1,R0
        MLS      R5,R0,R2,R1
//  441 	int prevIndex = (stateIndex + shape->getSize() - 1 ) % shape->getSize();
        MOVS     R0,R4
          CFI FunCall _ZNK12TriggerShape7getSizeEv
        BL       _ZNK12TriggerShape7getSizeEv
        MOVS     R6,R0
        MOVS     R0,R4
          CFI FunCall _ZNK12TriggerShape7getSizeEv
        BL       _ZNK12TriggerShape7getSizeEv
        ADDS     R1,R6,R5
        SUBS     R1,R1,#+1
        SDIV     R2,R1,R0
        MLS      R6,R0,R2,R1
//  442 
//  443 
//  444 	int loopIndex = i / shape->getSize();
        MOVS     R0,R4
          CFI FunCall _ZNK12TriggerShape7getSizeEv
        BL       _ZNK12TriggerShape7getSizeEv
        LDR      R1,[SP, #+24]
        SDIV     R0,R1,R0
        VMOV     S16,R0
//  445 
//  446 	int time = (int) (HELPER_PERIOD * (loopIndex + shape->wave.getSwitchTime(stateIndex)));
        VCVT.F32.S32 S17,S16
        MOVS     R1,R5
        ADDW     R0,R4,#+2932
          CFI FunCall _ZNK12multi_wave_s13getSwitchTimeEi
        BL       _ZNK12multi_wave_s13getSwitchTimeEi
        VADD.F32 S0,S17,S0
        VLDR.W   S1,??DataTable14  ;; 0x47c35000
        VMUL.F32 S0,S0,S1
        VCVT.S32.F32 S17,S0
//  447 
//  448 	bool_t primaryWheelState = shape->wave.getChannelState(0, prevIndex);
        MOVS     R2,R6
        MOVS     R1,#+0
        ADDW     R0,R4,#+2932
          CFI FunCall _ZNK12multi_wave_s15getChannelStateEii
        BL       _ZNK12multi_wave_s15getChannelStateEii
        CMP      R0,#+0
        BEQ.N    ??nextStep_0
        MOVS     R10,#+1
        B.N      ??nextStep_1
??nextStep_0:
        MOVS     R10,#+0
//  449 	bool_t newPrimaryWheelState = shape->wave.getChannelState(0, stateIndex);
??nextStep_1:
        MOVS     R2,R5
        MOVS     R1,#+0
        ADDW     R0,R4,#+2932
          CFI FunCall _ZNK12multi_wave_s15getChannelStateEii
        BL       _ZNK12multi_wave_s15getChannelStateEii
        CMP      R0,#+0
        BEQ.N    ??nextStep_2
        MOVS     R0,#+1
        B.N      ??nextStep_3
??nextStep_2:
        MOVS     R0,#+0
??nextStep_3:
        STRB     R0,[SP, #+2]
//  450 
//  451 	bool_t secondaryWheelState = shape->wave.getChannelState(1, prevIndex);
        MOVS     R2,R6
        MOVS     R1,#+1
        ADDW     R0,R4,#+2932
          CFI FunCall _ZNK12multi_wave_s15getChannelStateEii
        BL       _ZNK12multi_wave_s15getChannelStateEii
        CMP      R0,#+0
        BEQ.N    ??nextStep_4
        MOVS     R7,#+1
        B.N      ??nextStep_5
??nextStep_4:
        MOVS     R7,#+0
//  452 	bool_t newSecondaryWheelState = shape->wave.getChannelState(1, stateIndex);
??nextStep_5:
        MOVS     R2,R5
        MOVS     R1,#+1
        ADDW     R0,R4,#+2932
          CFI FunCall _ZNK12multi_wave_s15getChannelStateEii
        BL       _ZNK12multi_wave_s15getChannelStateEii
        CMP      R0,#+0
        BEQ.N    ??nextStep_6
        MOVS     R0,#+1
        B.N      ??nextStep_7
??nextStep_6:
        MOVS     R0,#+0
??nextStep_7:
        STRB     R0,[SP, #+1]
//  453 
//  454 	bool_t thirdWheelState = shape->wave.getChannelState(2, prevIndex);
        MOVS     R2,R6
        MOVS     R1,#+2
        ADDW     R0,R4,#+2932
          CFI FunCall _ZNK12multi_wave_s15getChannelStateEii
        BL       _ZNK12multi_wave_s15getChannelStateEii
        CMP      R0,#+0
        BEQ.N    ??nextStep_8
        MOVS     R8,#+1
        B.N      ??nextStep_9
??nextStep_8:
        MOVS     R8,#+0
//  455 	bool_t new3rdWheelState = shape->wave.getChannelState(2, stateIndex);
??nextStep_9:
        MOVS     R2,R5
        MOVS     R1,#+2
        ADDW     R0,R4,#+2932
          CFI FunCall _ZNK12multi_wave_s15getChannelStateEii
        BL       _ZNK12multi_wave_s15getChannelStateEii
        CMP      R0,#+0
        BEQ.N    ??nextStep_10
        MOVS     R0,#+1
        B.N      ??nextStep_11
??nextStep_10:
        MOVS     R0,#+0
??nextStep_11:
        STRB     R0,[SP, #+0]
//  456 
//  457 	if (primaryWheelState != newPrimaryWheelState) {
        LDRB     R0,[SP, #+2]
        UXTB     R10,R10          ;; ZeroExt  R10,R10,#+24,#+24
        CMP      R10,R0
        BEQ.N    ??nextStep_12
//  458 		primaryWheelState = newPrimaryWheelState;
        LDRB     R0,[SP, #+2]
        MOV      R10,R0
//  459 		trigger_event_e s = primaryWheelState ? SHAFT_PRIMARY_UP : SHAFT_PRIMARY_DOWN;
//  460 		state->decodeTriggerEvent(s, time PASS_ENGINE_PARAMETER);
        VMOV     R2,S17
        ASRS     R3,R2,#+31
        MOV      R1,R10
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOV      R0,R11
          CFI FunCall _ZN12TriggerState18decodeTriggerEventE15trigger_event_ex
        BL       _ZN12TriggerState18decodeTriggerEventE15trigger_event_ex
//  461 	}
//  462 
//  463 	if (secondaryWheelState != newSecondaryWheelState) {
??nextStep_12:
        LDRB     R0,[SP, #+1]
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,R0
        BEQ.N    ??nextStep_13
//  464 		secondaryWheelState = newSecondaryWheelState;
        LDRB     R0,[SP, #+1]
        MOVS     R7,R0
//  465 		trigger_event_e s = secondaryWheelState ? SHAFT_SECONDARY_UP : SHAFT_SECONDARY_DOWN;
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+0
        BEQ.N    ??nextStep_14
        MOVS     R9,#+3
        B.N      ??nextStep_15
??nextStep_14:
        MOVS     R9,#+2
//  466 		state->decodeTriggerEvent(s, time PASS_ENGINE_PARAMETER);
??nextStep_15:
        VMOV     R2,S17
        ASRS     R3,R2,#+31
        MOV      R1,R9
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOV      R0,R11
          CFI FunCall _ZN12TriggerState18decodeTriggerEventE15trigger_event_ex
        BL       _ZN12TriggerState18decodeTriggerEventE15trigger_event_ex
//  467 	}
//  468 
//  469 	if (thirdWheelState != new3rdWheelState) {
??nextStep_13:
        LDRB     R0,[SP, #+0]
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        CMP      R8,R0
        BEQ.N    ??nextStep_16
//  470 		thirdWheelState = new3rdWheelState;
        LDRB     R0,[SP, #+0]
        MOV      R8,R0
//  471 		trigger_event_e s = thirdWheelState ? SHAFT_3RD_UP : SHAFT_3RD_DOWN;
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        CMP      R8,#+0
        BEQ.N    ??nextStep_17
        MOVS     R9,#+5
        B.N      ??nextStep_18
??nextStep_17:
        MOVS     R9,#+4
//  472 		state->decodeTriggerEvent(s, time PASS_ENGINE_PARAMETER);
??nextStep_18:
        VMOV     R2,S17
        ASRS     R3,R2,#+31
        MOV      R1,R9
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOV      R0,R11
          CFI FunCall _ZN12TriggerState18decodeTriggerEventE15trigger_event_ex
        BL       _ZN12TriggerState18decodeTriggerEventE15trigger_event_ex
//  473 	}
//  474 }
??nextStep_16:
        ADD      SP,SP,#+8
          CFI CFA R13+56
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+48
        POP      {R0-R2,R4-R11,PC}  ;; return
          CFI EndBlock cfiBlock21
//  475 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock22 Using cfiCommon0
          CFI Function _Z11onFindIndexP12TriggerState
          CFI NoCalls
        THUMB
//  476 static void onFindIndex(TriggerState *state) {
//  477 	for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++) {
_Z11onFindIndexP12TriggerState:
        MOVS     R1,#+0
??onFindIndex_0:
        CMP      R1,#+3
        BGE.N    ??onFindIndex_1
//  478 		// todo: that's not the best place for this intermediate data storage, fix it!
//  479 		state->expectedTotalTime[i] = state->totalTimeNt[i];
        ADDS     R2,R0,R1, LSL #+2
        ADDS     R3,R0,R1, LSL #+2
        LDR      R3,[R3, #+24]
        STR      R3,[R2, #+48]
//  480 	}
        ADDS     R1,R1,#+1
        B.N      ??onFindIndex_0
//  481 }
??onFindIndex_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock22
//  482 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock23 Using cfiCommon0
          CFI Function _Z13doFindTriggerP23TriggerStimulatorHelperP12TriggerShapePK16trigger_config_sP12TriggerState
        THUMB
//  483 static uint32_t doFindTrigger(TriggerStimulatorHelper *helper, TriggerShape * shape,
//  484 		trigger_config_s const*triggerConfig, TriggerState *state DECLARE_ENGINE_PARAMETER_S) {
_Z13doFindTriggerP23TriggerStimulatorHelperP12TriggerShapePK16trigger_config_sP12TriggerState:
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
//  485 	for (int i = 0; i < 4 * PWM_PHASE_MAX_COUNT; i++) {
        MOVS     R8,#+0
??doFindTrigger_0:
        CMP      R8,#+1008
        BGE.N    ??doFindTrigger_1
//  486 		helper->nextStep(state, shape, i, triggerConfig PASS_ENGINE_PARAMETER);
        STR      R6,[SP, #+0]
        MOV      R3,R8
        MOVS     R2,R5
        MOVS     R1,R7
        MOVS     R0,R4
          CFI FunCall _ZN23TriggerStimulatorHelper8nextStepEP12TriggerStateP12TriggerShapeiPK16trigger_config_s
        BL       _ZN23TriggerStimulatorHelper8nextStepEP12TriggerStateP12TriggerShapeiPK16trigger_config_s
//  487 
//  488 		if (state->shaft_is_synchronized)
        LDRB     R0,[R7, #+4]
        CMP      R0,#+0
        BEQ.N    ??doFindTrigger_2
//  489 			return i;
        MOV      R0,R8
        B.N      ??doFindTrigger_3
//  490 	}
??doFindTrigger_2:
        ADDS     R8,R8,#+1
        B.N      ??doFindTrigger_0
//  491 	firmwareError("findTriggerZeroEventIndex() failed");
??doFindTrigger_1:
        LDR.N    R0,??DataTable14_26
          CFI FunCall firmwareError
        BL       firmwareError
//  492 	return EFI_ERROR_CODE;
        MOVS     R0,#-1
??doFindTrigger_3:
        POP      {R1,R2,R4-R8,PC}  ;; return
          CFI EndBlock cfiBlock23
//  493 }
//  494 
//  495 /**
//  496  * Trigger shape is defined in a way which is convenient for trigger shape definition
//  497  * On the other hand, trigger decoder indexing begins from synchronization event.
//  498  *
//  499  * This function finds the index of synchronization event within TriggerShape
//  500  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock24 Using cfiCommon0
          CFI Function _Z25findTriggerZeroEventIndexP12TriggerShapePK16trigger_config_s
        THUMB
//  501 uint32_t findTriggerZeroEventIndex(TriggerShape * shape, trigger_config_s const*triggerConfig
//  502 DECLARE_ENGINE_PARAMETER_S) {
_Z25findTriggerZeroEventIndexP12TriggerShapePK16trigger_config_s:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        SUB      SP,SP,#+192
          CFI CFA R13+216
        MOVS     R4,R0
        MOVS     R5,R1
//  503 
//  504 	// todo: should this variable be declared 'static' to reduce stack usage?
//  505 	TriggerState state;
        ADD      R0,SP,#+8
          CFI FunCall _ZN12TriggerStateC1Ev
        BL       _ZN12TriggerStateC1Ev
//  506 	errorDetection.clear();
        LDR.N    R0,??DataTable14_2
          CFI FunCall _ZN13cyclic_bufferIiE5clearEv
        BL       _ZN13cyclic_bufferIiE5clearEv
//  507 
//  508 	// todo: should this variable be declared 'static' to reduce stack usage?
//  509 	TriggerStimulatorHelper helper;
        ADD      R0,SP,#+4
          CFI FunCall _ZN23TriggerStimulatorHelperC1Ev
        BL       _ZN23TriggerStimulatorHelperC1Ev
//  510 
//  511 	uint32_t index = doFindTrigger(&helper, shape, triggerConfig, &state PASS_ENGINE_PARAMETER);
        ADD      R3,SP,#+8
        MOVS     R2,R5
        MOVS     R1,R4
          CFI FunCall _Z13doFindTriggerP23TriggerStimulatorHelperP12TriggerShapePK16trigger_config_sP12TriggerState
        BL       _Z13doFindTriggerP23TriggerStimulatorHelperP12TriggerShapePK16trigger_config_sP12TriggerState
        MOVS     R6,R0
//  512 	if (index == EFI_ERROR_CODE) {
        CMN      R6,#+1
        BNE.N    ??findTriggerZeroEventIndex_0
//  513 		return index;
        MOVS     R0,R6
        B.N      ??findTriggerZeroEventIndex_1
//  514 	}
//  515 	efiAssert(state.getTotalRevolutionCounter() == 1, "totalRevolutionCounter", EFI_ERROR_CODE);
??findTriggerZeroEventIndex_0:
        ADD      R0,SP,#+8
          CFI FunCall _ZN12TriggerState25getTotalRevolutionCounterEv
        BL       _ZN12TriggerState25getTotalRevolutionCounterEv
        CMP      R0,#+1
        BEQ.N    ??findTriggerZeroEventIndex_2
        LDR.N    R0,??DataTable14_27
          CFI FunCall firmwareError
        BL       firmwareError
        MOVS     R0,#-1
        B.N      ??findTriggerZeroEventIndex_1
//  516 
//  517 	/**
//  518 	 * Now that we have just located the synch point, we can simulate the whole cycle
//  519 	 * in order to calculate expected duty cycle
//  520 	 *
//  521 	 * todo: add a comment why are we doing '2 * shape->getSize()' here?
//  522 	 */
//  523 	state.cycleCallback = onFindIndex;
??findTriggerZeroEventIndex_2:
        LDR.N    R0,??DataTable14_28
        STR      R0,[SP, #+8]
//  524 
//  525 	int startIndex = engineConfiguration->useOnlyFrontForTrigger ? index + 2 : index + 1;
        LDR.N    R0,??DataTable14_7
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+6
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??findTriggerZeroEventIndex_3
        ADDS     R8,R6,#+2
        B.N      ??findTriggerZeroEventIndex_4
??findTriggerZeroEventIndex_3:
        ADDS     R8,R6,#+1
//  526 
//  527 	for (uint32_t i = startIndex; i <= index + 2 * shape->getSize(); i++) {
??findTriggerZeroEventIndex_4:
        MOV      R7,R8
??findTriggerZeroEventIndex_5:
        MOVS     R0,R4
          CFI FunCall _ZNK12TriggerShape7getSizeEv
        BL       _ZNK12TriggerShape7getSizeEv
        ADDS     R0,R6,R0, LSL #+1
        CMP      R0,R7
        BCC.N    ??findTriggerZeroEventIndex_6
//  528 		helper.nextStep(&state, shape, i, triggerConfig PASS_ENGINE_PARAMETER);
        STR      R5,[SP, #+0]
        MOVS     R3,R7
        MOVS     R2,R4
        ADD      R1,SP,#+8
        ADD      R0,SP,#+4
          CFI FunCall _ZN23TriggerStimulatorHelper8nextStepEP12TriggerStateP12TriggerShapeiPK16trigger_config_s
        BL       _ZN23TriggerStimulatorHelper8nextStepEP12TriggerStateP12TriggerShapeiPK16trigger_config_s
//  529 		if (engineConfiguration->useOnlyFrontForTrigger)
        LDR.N    R0,??DataTable14_7
        LDR      R0,[R0, #+0]
        LDRB     R0,[R0, #+1509]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+6
        ANDS     R0,R0,#0x1
        CMP      R0,#+0
        BEQ.N    ??findTriggerZeroEventIndex_7
//  530 			i++;
        ADDS     R7,R7,#+1
//  531 	}
??findTriggerZeroEventIndex_7:
        ADDS     R7,R7,#+1
        B.N      ??findTriggerZeroEventIndex_5
//  532 	efiAssert(state.getTotalRevolutionCounter() == 3, "totalRevolutionCounter2 expected 3", EFI_ERROR_CODE);
??findTriggerZeroEventIndex_6:
        ADD      R0,SP,#+8
          CFI FunCall _ZN12TriggerState25getTotalRevolutionCounterEv
        BL       _ZN12TriggerState25getTotalRevolutionCounterEv
        CMP      R0,#+3
        BEQ.N    ??findTriggerZeroEventIndex_8
        LDR.N    R0,??DataTable14_29
          CFI FunCall firmwareError
        BL       firmwareError
        MOVS     R0,#-1
        B.N      ??findTriggerZeroEventIndex_1
//  533 
//  534 	for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++) {
??findTriggerZeroEventIndex_8:
        MOVS     R7,#+0
??findTriggerZeroEventIndex_9:
        CMP      R7,#+3
        BGE.N    ??findTriggerZeroEventIndex_10
//  535 		shape->dutyCycle[i] = 1.0 * state.expectedTotalTime[i] / HELPER_PERIOD;
        ADD      R0,SP,#+8
        ADDS     R0,R0,R7, LSL #+2
        LDR      R0,[R0, #+48]
          CFI FunCall __aeabi_i2d
        BL       __aeabi_i2d
        MOVS     R2,#+0
        LDR.N    R3,??DataTable14_13  ;; 0x3ff00000
          CFI FunCall __aeabi_dmul
        BL       __aeabi_dmul
        MOVS     R2,#+0
        LDR.N    R3,??DataTable14_30  ;; 0x40f86a00
          CFI FunCall __aeabi_ddiv
        BL       __aeabi_ddiv
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        ADDS     R1,R4,R7, LSL #+2
        STR      R0,[R1, #+12]
//  536 	}
        ADDS     R7,R7,#+1
        B.N      ??findTriggerZeroEventIndex_9
//  537 
//  538 	return index % shape->getSize();
??findTriggerZeroEventIndex_10:
        MOVS     R0,R4
          CFI FunCall _ZNK12TriggerShape7getSizeEv
        BL       _ZNK12TriggerShape7getSizeEv
        UDIV     R1,R6,R0
        MLS      R0,R0,R1,R6
??findTriggerZeroEventIndex_1:
        ADD      SP,SP,#+192
          CFI CFA R13+24
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock24
//  539 }
//  540 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock25 Using cfiCommon0
          CFI Function _Z24initTriggerDecoderLoggerP7Logging
          CFI NoCalls
        THUMB
//  541 void initTriggerDecoderLogger(Logging *sharedLogger) {
//  542 	logger = sharedLogger;
_Z24initTriggerDecoderLoggerP7Logging:
        LDR.N    R1,??DataTable14_15
        STR      R0,[R1, #+0]
//  543 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock25
//  544 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock26 Using cfiCommon0
          CFI Function _Z18initTriggerDecoderv
        THUMB
//  545 void initTriggerDecoder(void) {
_Z18initTriggerDecoderv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  546 #if (EFI_PROD_CODE || EFI_SIMULATOR)
//  547 	outputPinRegisterExt2("trg_err", &triggerDecoderErrorPin, boardConfiguration->triggerErrorPin, &boardConfiguration->triggerErrorPinMode);
        LDR.N    R0,??DataTable14_31
        LDR      R0,[R0, #+0]
        ADDS     R3,R0,#+604
        LDR.N    R0,??DataTable14_31
        LDR      R0,[R0, #+0]
        LDR      R2,[R0, #+600]
        LDR.N    R1,??DataTable14_1
        LDR.N    R0,??DataTable14_32
          CFI FunCall _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
        BL       _Z21outputPinRegisterExt2PKcP9OutputPin11brain_pin_eP17pin_output_mode_e
//  548 #endif
//  549 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock26

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14:
        DC32     0x47c35000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_1:
        DC32     triggerDecoderErrorPin

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_2:
        DC32     errorDetection

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_3:
        DC32     __dso_handle

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_4:
        DC32     _ZN13cyclic_bufferIiED1Ev

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_5:
        DC32     `?<Constant "unexpected signal">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_6:
        DC32     eventIndex

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_7:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_8:
        DC32     0x6422c401

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_9:
        DC32     0x6422c400

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_10:
        DC32     _engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_11:
        DC32     _engine+0xDDC

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_12:
        DC32     _engine+0xDE0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_13:
        DC32     0x3ff00000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_14:
        DC32     `?<Constant "gap=%f @ %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_15:
        DC32     logger

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_16:
        DC32     _engine+0x16F0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_17:
        DC32     lastDecodingErrorTime

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_18:
        DC32     `?<Constant "error: synchronizatio...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_19:
        DC32     `?<Constant "trigger decoding issu...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_20:
        DC32     `?<Constant "total count">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_21:
        DC32     `?<Constant "skipped count">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_22:
        DC32     `?<Constant "totalTeethCount is zero">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_23:
        DC32     `?<Constant "TriggerShape is NULL">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_24:
        DC32     `?<Constant "initializeTriggerShape()">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_25:
        DC32     `?<Constant "initializeTriggerShap...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_26:
        DC32     `?<Constant "findTriggerZeroEventI...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_27:
        DC32     `?<Constant "totalRevolutionCounter">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_28:
        DC32     _Z11onFindIndexP12TriggerState

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_29:
        DC32     `?<Constant "totalRevolutionCounte...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_30:
        DC32     0x40f86a00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_31:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable14_32:
        DC32     `?<Constant "trg_err">`

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIiEC2Ev
          CFI Block cfiBlock27 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIiEC2Ev
        THUMB
// __code __interwork __softfp cyclic_buffer<int>::subobject cyclic_buffer()
_ZN13cyclic_bufferIiEC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN13cyclic_bufferIiEC1Ev
        BL       _ZN13cyclic_bufferIiEC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock27

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
`?<Constant "total count">`:
        DATA
        DC8 "total count"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "skipped count">`:
        DATA
        DC8 "skipped count"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "totalTeethCount is zero">`:
        DATA
        DC8 "totalTeethCount is zero"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "TriggerShape is NULL">`:
        DATA
        DC8 "TriggerShape is NULL"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "findTriggerZeroEventI...">`:
        DATA
        DC8 "findTriggerZeroEventIndex() failed"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "totalRevolutionCounter">`:
        DATA
        DC8 "totalRevolutionCounter"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "totalRevolutionCounte...">`:
        DATA
        DC8 "totalRevolutionCounter2 expected 3"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "trg_err">`:
        DATA
        DC8 "trg_err"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "initializeTriggerShape()">`:
        DATA
        DC8 "initializeTriggerShape()"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "initializeTriggerShap...">`:
        DATA
        DC8 "initializeTriggerShape() not implemented: %d"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "unexpected signal">`:
        DATA
        DC8 "unexpected signal"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "gap=%f @ %d">`:
        DATA
        DC8 "gap=%f @ %d"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "error: synchronizatio...">`:
        DATA
        DC8 65H, 72H, 72H, 6FH, 72H, 3AH, 20H, 73H
        DC8 79H, 6EH, 63H, 68H, 72H, 6FH, 6EH, 69H
        DC8 7AH, 61H, 74H, 69H, 6FH, 6EH, 50H, 6FH
        DC8 69H, 6EH, 74H, 20H, 40H, 20H, 69H, 6EH
        DC8 64H, 65H, 78H, 20H, 25H, 64H, 20H, 65H
        DC8 78H, 70H, 65H, 63H, 74H, 65H, 64H, 20H
        DC8 25H, 64H, 2FH, 25H, 64H, 2FH, 25H, 64H
        DC8 20H, 67H, 6FH, 74H, 20H, 25H, 64H, 2FH
        DC8 25H, 64H, 2FH, 25H, 64H, 0
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "trigger decoding issu...">`:
        DATA
        DC8 74H, 72H, 69H, 67H, 67H, 65H, 72H, 20H
        DC8 64H, 65H, 63H, 6FH, 64H, 69H, 6EH, 67H
        DC8 20H, 69H, 73H, 73H, 75H, 65H, 2EH, 20H
        DC8 65H, 78H, 70H, 65H, 63H, 74H, 65H, 64H
        DC8 20H, 25H, 64H, 2FH, 25H, 64H, 2FH, 25H
        DC8 64H, 20H, 67H, 6FH, 74H, 20H, 25H, 64H
        DC8 2FH, 25H, 64H, 2FH, 25H, 64H, 0
        DC8 0

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN13cyclic_bufferIiED2Ev
          CFI Block cfiBlock28 Using cfiCommon0
          CFI Function _ZN13cyclic_bufferIiED2Ev
        THUMB
// __code __interwork __softfp cyclic_buffer<int>::subobject ~cyclic_buffer()
_ZN13cyclic_bufferIiED2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN13cyclic_bufferIiED1Ev
        BL       _ZN13cyclic_bufferIiED1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock28

        END
//  550 
//  551 #endif /* EFI_SHAFT_POSITION_INPUT */
// 
//   288 bytes in section .bss
//    16 bytes in section .data
//     4 bytes in section .init_array
//   416 bytes in section .rodata
// 3 668 bytes in section .text
// 
// 3 400 bytes of CODE  memory (+ 272 bytes shared)
//   416 bytes of CONST memory
//   304 bytes of DATA  memory
//
//Errors: none
//Warnings: 2
