///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:50 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\tri /
//                       gger\trigger_bmw.cpp                                 /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\tri /
//                       gger\trigger_bmw.cpp -lCN                            /
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
//                       trigger_bmw.s                                        /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME trigger_bmw

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _ZN12TriggerShape5resetE16operation_mode_eb
        EXTERN _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        EXTERN absF
        EXTERN firmwareError

        PUBLIC _Z31configureMiniCooperTriggerShapeP12TriggerShape
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\trigger\trigger_bmw.cpp
//    1 /**
//    2  * @file	trigger_bmw.cpp
//    3  *
//    4  * @date May 11, 2014
//    5  * @author Andrey Belomutskiy, (c) 2012-2015
//    6  */
//    7 
//    8 #include "trigger_bmw.h"
//    9 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z7addPairP12TriggerShapeff
        THUMB
//   10 static inline float addPair(TriggerShape *s, float a, float w) {
_Z7addPairP12TriggerShapeff:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        VMOV.F32 S17,S0
        VMOV.F32 S16,S1
//   11 	s->addEvent(a, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   12 	a += w;
        VADD.F32 S17,S17,S16
//   13 	s->addEvent(a, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   14 	a += w;
        VADD.F32 S17,S17,S16
//   15 	return a;
        VMOV.F32 S0,S17
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0
//   16 }
//   17 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z31configureMiniCooperTriggerShapeP12TriggerShape
        THUMB
//   18 void configureMiniCooperTriggerShape(TriggerShape *s) {
_Z31configureMiniCooperTriggerShapeP12TriggerShape:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        VPUSH    {D8-D9}
          CFI D9 Frame(CFA, -24)
          CFI D8 Frame(CFA, -32)
          CFI CFA R13+32
        MOVS     R4,R0
//   19 	s->reset(FOUR_STROKE_CAM_SENSOR, true);
        MOVS     R2,#+1
        MOVS     R1,#+2
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape5resetE16operation_mode_eb
        BL       _ZN12TriggerShape5resetE16operation_mode_eb
//   20 
//   21 //	s->initialState[0] = 1;
//   22 
//   23 	float w = 360.0 / 121;
        VLDR.W   S16,??DataTable0  ;; 0x403e69c9
//   24 	float a = w / 2;
        VMOV.F32 S0,#2.0
        VDIV.F32 S17,S16,S0
//   25 	s->addEvent(a, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   26 	a += w;
        VADD.F32 S17,S17,S16
//   27 
//   28 	for (int i = 0; i <= 22; i++) {
        MOVS     R5,#+0
??configureMiniCooperTriggerShape_0:
        CMP      R5,#+23
        BGE.N    ??configureMiniCooperTriggerShape_1
//   29 		a = addPair(s, a, w);
        VMOV.F32 S1,S16
        VMOV.F32 S0,S17
        MOVS     R0,R4
          CFI FunCall _Z7addPairP12TriggerShapeff
        BL       _Z7addPairP12TriggerShapeff
        VMOV.F32 S17,S0
//   30 	}
        ADDS     R5,R5,#+1
        B.N      ??configureMiniCooperTriggerShape_0
//   31 
//   32 	a += 3 * w;
??configureMiniCooperTriggerShape_1:
        VMOV.F32 S0,#3.0
        VMLA.F32 S17,S16,S0
//   33 
//   34 	float firstGapAngle = a;
        VMOV.F32 S18,S17
//   35 
//   36 	s->addEvent(a, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   37 	a += 3 * w;
        VMOV.F32 S0,#3.0
        VMLA.F32 S17,S16,S0
//   38 	s->addEvent(a, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   39 	a += w;
        VADD.F32 S17,S17,S16
//   40 
//   41 	for (int i = 0; i < 36; i++) {
        MOVS     R5,#+0
??configureMiniCooperTriggerShape_2:
        CMP      R5,#+36
        BGE.N    ??configureMiniCooperTriggerShape_3
//   42 		a = addPair(s, a, w);
        VMOV.F32 S1,S16
        VMOV.F32 S0,S17
        MOVS     R0,R4
          CFI FunCall _Z7addPairP12TriggerShapeff
        BL       _Z7addPairP12TriggerShapeff
        VMOV.F32 S17,S0
//   43 	}
        ADDS     R5,R5,#+1
        B.N      ??configureMiniCooperTriggerShape_2
//   44 
//   45 	s->addEvent(376, T_PRIMARY, TV_HIGH);
??configureMiniCooperTriggerShape_3:
        MOVS     R2,#+1
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable0_1  ;; 0x43bc0000
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   46 
//   47 	for (int i = 0; i < 21; i++) {
        MOVS     R5,#+0
??configureMiniCooperTriggerShape_4:
        CMP      R5,#+21
        BGE.N    ??configureMiniCooperTriggerShape_5
//   48 		a = addPair(s, a, w);
        VMOV.F32 S1,S16
        VMOV.F32 S0,S17
        MOVS     R0,R4
          CFI FunCall _Z7addPairP12TriggerShapeff
        BL       _Z7addPairP12TriggerShapeff
        VMOV.F32 S17,S0
//   49 	}
        ADDS     R5,R5,#+1
        B.N      ??configureMiniCooperTriggerShape_4
//   50 	a += 3 * w;
??configureMiniCooperTriggerShape_5:
        VMOV.F32 S0,#3.0
        VMLA.F32 S17,S16,S0
//   51 
//   52 	efiAssertVoid(absF(firstGapAngle + 360 - a) < 0.1, "shape constraint");
        VLDR.W   S0,??DataTable0_2  ;; 0x43b40000
        VADD.F32 S0,S18,S0
        VSUB.F32 S0,S0,S17
          CFI FunCall absF
        BL       absF
        VLDR.W   S1,??DataTable0_3  ;; 0x3dcccccd
        VCMP.F32 S0,S1
        FMSTAT   
        BMI.N    ??configureMiniCooperTriggerShape_6
        LDR.N    R0,??DataTable0_5
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??configureMiniCooperTriggerShape_7
//   53 
//   54 	s->addEvent(a, T_SECONDARY, TV_HIGH);
??configureMiniCooperTriggerShape_6:
        MOVS     R2,#+1
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   55 	a += 3 * w;
        VMOV.F32 S0,#3.0
        VMLA.F32 S17,S16,S0
//   56 	s->addEvent(a, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   57 	a += w;
        VADD.F32 S17,S17,S16
//   58 
//   59 	for (int i = 0; i < 33; i++) {
        MOVS     R5,#+0
??configureMiniCooperTriggerShape_8:
        CMP      R5,#+33
        BGE.N    ??configureMiniCooperTriggerShape_9
//   60 		a = addPair(s, a, w);
        VMOV.F32 S1,S16
        VMOV.F32 S0,S17
        MOVS     R0,R4
          CFI FunCall _Z7addPairP12TriggerShapeff
        BL       _Z7addPairP12TriggerShapeff
        VMOV.F32 S17,S0
//   61 	}
        ADDS     R5,R5,#+1
        B.N      ??configureMiniCooperTriggerShape_8
//   62 
//   63 	efiAssertVoid(absF(720 - w / 2 - a) < 0.1, "shape constraint");
??configureMiniCooperTriggerShape_9:
        VMOV.F32 S0,#2.0
        VDIV.F32 S0,S16,S0
        VLDR.W   S1,??DataTable0_4  ;; 0x44340000
        VSUB.F32 S0,S1,S0
        VSUB.F32 S0,S0,S17
          CFI FunCall absF
        BL       absF
        VLDR.W   S1,??DataTable0_3  ;; 0x3dcccccd
        VCMP.F32 S0,S1
        FMSTAT   
        BMI.N    ??configureMiniCooperTriggerShape_10
        LDR.N    R0,??DataTable0_5
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??configureMiniCooperTriggerShape_7
//   64 	s->addEvent(a, T_SECONDARY, TV_HIGH);
??configureMiniCooperTriggerShape_10:
        MOVS     R2,#+1
        MOVS     R1,#+1
        VMOV.F32 S0,S17
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   65 
//   66 	s->addEvent(720.0, T_PRIMARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable0_4  ;; 0x44340000
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   67 
//   68 	/**
//   69 	 * With just one tooth on camshaft synchronization is not needed
//   70 	 */
//   71 	s->isSynchronizationNeeded = false;
        MOVS     R0,#+0
        STRB     R0,[R4, #+0]
//   72 }
??configureMiniCooperTriggerShape_7:
        VPOP     {D8-D9}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI CFA R13+16
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0:
        DC32     0x403e69c9

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_1:
        DC32     0x43bc0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_2:
        DC32     0x43b40000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_3:
        DC32     0x3dcccccd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_4:
        DC32     0x44340000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_5:
        DC32     `?<Constant "shape constraint">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "shape constraint">`:
        DATA
        DC8 "shape constraint"
        DC8 0, 0, 0

        END
// 
//  20 bytes in section .rodata
// 482 bytes in section .text
// 
// 482 bytes of CODE  memory
//  20 bytes of CONST memory
//
//Errors: none
//Warnings: 1
