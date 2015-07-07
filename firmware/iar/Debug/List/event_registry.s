///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:48 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\alg /
//                       o\event_registry.cpp                                 /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\alg /
//                       o\event_registry.cpp -lCN                            /
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
//                       event_registry.s                                     /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME event_registry

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _ZN12scheduling_sC1Ev
        EXTERN __aeabi_vec_ctor_nocookie_nodtor

        PUBLIC _ZN13IgnitionEventC1Ev
        PUBLIC _ZN13IgnitionEventC2Ev
        PUBLIC _ZN14InjectionEventC1Ev
        PUBLIC _ZN14InjectionEventC2Ev
        PUBLIC _ZN19OutputSignal_structC1Ev
        PUBLIC _ZN24event_trigger_position_sC1Ev
        PUBLIC _ZN24event_trigger_position_sC2Ev
        
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
        

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _ZN24event_trigger_position_sC2Ev
        THUMB
// __code __interwork __softfp event_trigger_position_s::subobject event_trigger_position_s()
_ZN24event_trigger_position_sC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN24event_trigger_position_sC1Ev
        BL       _ZN24event_trigger_position_sC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0
// F:\stuff\rusefi_sourceforge\firmware\controllers\algo\event_registry.cpp
//    1 /**
//    2  * @file	event_registry.cpp
//    3  * @brief	This data structure knows when to do what
//    4  *
//    5  * @date Nov 27, 2013
//    6  * @author Andrey Belomutskiy, (c) 2012-2015
//    7  *
//    8  *
//    9  * This file is part of rusEfi - see http://rusefi.com
//   10  *
//   11  * rusEfi is free software; you can redistribute it and/or modify it under the terms of
//   12  * the GNU General Public License as published by the Free Software Foundation; either
//   13  * version 3 of the License, or (at your option) any later version.
//   14  *
//   15  * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
//   16  * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   17  * GNU General Public License for more details.
//   18  *
//   19  * You should have received a copy of the GNU General Public License along with this program.
//   20  * If not, see <http://www.gnu.org/licenses/>.
//   21  */
//   22 
//   23 #include "event_registry.h"
//   24 #include "main.h"
//   25 #include "engine_math.h"
//   26 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _ZN14InjectionEventC2Ev
        THUMB
// __code __interwork __softfp InjectionEvent::subobject InjectionEvent()
_ZN14InjectionEventC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN14InjectionEventC1Ev
        BL       _ZN14InjectionEventC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _ZN13IgnitionEventC2Ev
        THUMB
// __code __interwork __softfp IgnitionEvent::subobject IgnitionEvent()
_ZN13IgnitionEventC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN13IgnitionEventC1Ev
        BL       _ZN13IgnitionEventC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _ZN14InjectionEventC1Ev
        THUMB
//   27 InjectionEvent::InjectionEvent() {
_ZN14InjectionEventC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN24event_trigger_position_sC1Ev
        BL       _ZN24event_trigger_position_sC1Ev
        ADDS     R0,R4,#+16
          CFI FunCall _ZN19OutputSignal_structC1Ev
        BL       _ZN19OutputSignal_structC1Ev
//   28 	isSimultanious = false;
        MOVS     R0,#+0
        STRB     R0,[R4, #+120]
//   29 }
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//   30 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _ZN24event_trigger_position_sC1Ev
          CFI NoCalls
        THUMB
//   31 event_trigger_position_s::event_trigger_position_s() {
//   32 	eventIndex = 0;
_ZN24event_trigger_position_sC1Ev:
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//   33 	eventAngle = 0;
        MOVS     R1,#+0
        STR      R1,[R0, #+4]
//   34 	angleOffset = 0;
        MOVS     R1,#+0
        STR      R1,[R0, #+8]
//   35 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock4

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _ZN13IgnitionEventC1Ev
        THUMB
_ZN13IgnitionEventC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        ADDS     R0,R4,#+8
          CFI FunCall _ZN12scheduling_sC1Ev
        BL       _ZN12scheduling_sC1Ev
        ADDS     R0,R4,#+32
          CFI FunCall _ZN12scheduling_sC1Ev
        BL       _ZN12scheduling_sC1Ev
        ADDS     R0,R4,#+60
          CFI FunCall _ZN24event_trigger_position_sC1Ev
        BL       _ZN24event_trigger_position_sC1Ev
        ADDS     R0,R4,#+72
          CFI FunCall _ZN24event_trigger_position_sC1Ev
        BL       _ZN24event_trigger_position_sC1Ev
        MOVS     R0,#+0
        STR      R0,[R4, #+88]
        MOVS     R0,#+0
        STR      R0,[R4, #+84]
        MOVS     R0,#+0
        STR      R0,[R4, #+0]
        MVNS     R0,#-2147483648
        STR      R0,[R4, #+56]
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock5

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN19OutputSignal_structC1Ev
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _ZN19OutputSignal_structC1Ev
        THUMB
// __code __interwork __softfp OutputSignal_struct::OutputSignal_struct()
_ZN19OutputSignal_structC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R3,#+2
        MOVS     R2,#+24
        LDR.N    R1,??OutputSignal_struct_0
        ADDS     R0,R4,#+8
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
        MOVS     R3,#+2
        MOVS     R2,#+24
        LDR.N    R1,??OutputSignal_struct_0
        ADDS     R0,R4,#+56
          CFI FunCall __aeabi_vec_ctor_nocookie_nodtor
        BL       __aeabi_vec_ctor_nocookie_nodtor
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
        DATA
??OutputSignal_struct_0:
        DC32     _ZN12scheduling_sC1Ev
          CFI EndBlock cfiBlock6

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
//   36 
//   37 IgnitionEvent::IgnitionEvent() {
//   38 	name = NULL;
//   39 	next = NULL;
//   40 	output = NULL;
//   41 	advance = NAN;
//   42 }
//   43 
//   44 //void registerActuatorEventWhat(InjectionEventList *list, int eventIndex, OutputSignal *actuator, float angleOffset) {
//   45 //	ActuatorEvent *e = list->getNextActuatorEvent();
//   46 //	if (e == NULL)
//   47 //		return; // error already reported
//   48 //	e->position.eventIndex = eventIndex;
//   49 //	e->actuator = actuator;
//   50 //	e->position.angleOffset = angleOffset;
//   51 //}
// 
// 176 bytes in section .text
// 
// 136 bytes of CODE memory (+ 40 bytes shared)
//
//Errors: none
//Warnings: 1
