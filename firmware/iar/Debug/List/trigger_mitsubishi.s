///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:46 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\tri /
//                       gger\trigger_mitsubishi.cpp                          /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\tri /
//                       gger\trigger_mitsubishi.cpp -lCN                     /
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
//                       trigger_mitsubishi.s                                 /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME trigger_mitsubishi

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _ZN12TriggerShape28setTriggerSynchronizationGapEf
        EXTERN _ZN12TriggerShape5resetE16operation_mode_eb
        EXTERN _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        EXTERN __aeabi_d2f
        EXTERN __aeabi_dsub
        EXTERN __aeabi_i2d

        PUBLIC _Z24initializeMitsubishi4g18P12TriggerShape
        PUBLIC _Z31configureFordAspireTriggerShapeP12TriggerShape
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\trigger\trigger_mitsubishi.cpp
//    1 /**
//    2  * @file	trigger_mitsubishi.cpp
//    3  *
//    4  * @date Aug 5, 2014
//    5  * @author Andrey Belomutskiy, (c) 2012-2015
//    6  */
//    7 
//    8 #include "trigger_mitsubishi.h"
//    9 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z31configureFordAspireTriggerShapeP12TriggerShape
        THUMB
//   10 void configureFordAspireTriggerShape(TriggerShape * s) {
_Z31configureFordAspireTriggerShapeP12TriggerShape:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
//   11 	s->isSynchronizationNeeded = false;
        MOVS     R0,#+0
        STRB     R0,[R4, #+0]
//   12 	s->reset(FOUR_STROKE_CAM_SENSOR, true);
        MOVS     R2,#+1
        MOVS     R1,#+2
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape5resetE16operation_mode_eb
        BL       _ZN12TriggerShape5resetE16operation_mode_eb
//   13 
//   14 
//   15 	float x = 121.90;
        VLDR.W   S16,??DataTable1  ;; 0x42f3cccd
//   16 	float y = 110.86;
        VLDR.W   S17,??DataTable1_1  ;; 0x42ddb852
//   17 
//   18 	s->addEvent(x + y - 180, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        VADD.F32 S0,S16,S17
        VLDR.W   S1,??DataTable1_2  ;; 0xc3340000
        VADD.F32 S0,S0,S1
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   19 	s->addEvent(x, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   20 	s->addEvent(x + y, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        VADD.F32 S0,S16,S17
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   21 	s->addEvent(x + 180, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        VLDR.W   S0,??DataTable1_3  ;; 0x43340000
        VADD.F32 S0,S16,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   22 	s->addEvent(360, T_PRIMARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable1_4  ;; 0x43b40000
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   23 
//   24 	s->addEvent(x + 180 + y, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        VLDR.W   S0,??DataTable1_3  ;; 0x43340000
        VADD.F32 S0,S16,S0
        VADD.F32 S0,S0,S17
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   25 	s->addEvent(x + 360, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        VLDR.W   S0,??DataTable1_4  ;; 0x43b40000
        VADD.F32 S0,S16,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   26 	s->addEvent(x + 360 + y, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        VLDR.W   S0,??DataTable1_4  ;; 0x43b40000
        VADD.F32 S0,S16,S0
        VADD.F32 S0,S0,S17
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   27 	s->addEvent(x + 540, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        VLDR.W   S0,??DataTable1_5  ;; 0x44070000
        VADD.F32 S0,S16,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   28 	s->addEvent(720, T_PRIMARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable1_6  ;; 0x44340000
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   29 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0
//   30 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z24initializeMitsubishi4g18P12TriggerShape
        THUMB
//   31 void initializeMitsubishi4g18(TriggerShape *s) {
_Z24initializeMitsubishi4g18P12TriggerShape:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R5,R0
//   32 	s->reset(FOUR_STROKE_CAM_SENSOR, true);
        MOVS     R2,#+1
        MOVS     R1,#+2
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape5resetE16operation_mode_eb
        BL       _ZN12TriggerShape5resetE16operation_mode_eb
//   33 	s->useRiseEdge = false;
        MOVS     R0,#+0
        STRB     R0,[R5, #+2916]
//   34 
//   35 	s->setTriggerSynchronizationGap(1.6666);
        VLDR.W   S0,??DataTable1_7  ;; 0x3fd55326
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape28setTriggerSynchronizationGapEf
        BL       _ZN12TriggerShape28setTriggerSynchronizationGapEf
//   36 
//   37 	int secondaryWidth = 70;
        MOVS     R6,#+70
//   38 
//   39 	s->addEvent(180.0 - 131 + 5, T_PRIMARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable1_8  ;; 0x42580000
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   40 	s->addEvent(180.0 - secondaryWidth, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        MOVS     R4,R1
        MOVS     R7,R2
        MOVS     R0,R6
          CFI FunCall __aeabi_i2d
        BL       __aeabi_i2d
        MOVS     R2,R0
        MOVS     R3,R1
        MOVS     R0,#+0
        LDR.N    R1,??DataTable1_12  ;; 0x40668000
          CFI FunCall __aeabi_dsub
        BL       __aeabi_dsub
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        VMOV     S0,R0
        MOVS     R2,R7
        MOVS     R1,R4
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   41 	s->addEvent(180.0, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        VLDR.W   S0,??DataTable1_3  ;; 0x43340000
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   42 	s->addEvent(180.0 + 49 + 5, T_PRIMARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable1_9  ;; 0x436a0000
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   43 
//   44 	s->addEvent(360.0 - secondaryWidth, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        MOVS     R4,R1
        MOVS     R7,R2
        MOVS     R0,R6
          CFI FunCall __aeabi_i2d
        BL       __aeabi_i2d
        MOVS     R2,R0
        MOVS     R3,R1
        MOVS     R0,#+0
        LDR.N    R1,??DataTable1_13  ;; 0x40768000
          CFI FunCall __aeabi_dsub
        BL       __aeabi_dsub
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        VMOV     S0,R0
        MOVS     R2,R7
        MOVS     R1,R4
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   45 	s->addEvent(360.0, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        VLDR.W   S0,??DataTable1_4  ;; 0x43b40000
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   46 
//   47 	s->addEvent(540.0 - 131 + 5, T_PRIMARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable1_10  ;; 0x43cf0000
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   48 	s->addEvent(540.0 - secondaryWidth, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        MOVS     R4,R1
        MOVS     R7,R2
        MOVS     R0,R6
          CFI FunCall __aeabi_i2d
        BL       __aeabi_i2d
        MOVS     R2,R0
        MOVS     R3,R1
        MOVS     R0,#+0
        LDR.N    R1,??DataTable1_14  ;; 0x4080e000
          CFI FunCall __aeabi_dsub
        BL       __aeabi_dsub
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        VMOV     S0,R0
        MOVS     R2,R7
        MOVS     R1,R4
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   49 	s->addEvent(540.0 - 131 + 5 + 90, T_PRIMARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable1_11  ;; 0x43fc0000
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   50 	s->addEvent(540.0, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        VLDR.W   S0,??DataTable1_5  ;; 0x44070000
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   51 
//   52 	s->addEvent(720.0 - secondaryWidth, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        MOVS     R4,R1
        MOVS     R7,R2
        MOVS     R0,R6
          CFI FunCall __aeabi_i2d
        BL       __aeabi_i2d
        MOVS     R2,R0
        MOVS     R3,R1
        MOVS     R0,#+0
        LDR.N    R1,??DataTable1_15  ;; 0x40868000
          CFI FunCall __aeabi_dsub
        BL       __aeabi_dsub
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        VMOV     S0,R0
        MOVS     R2,R7
        MOVS     R1,R4
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   53 	s->addEvent(720.0, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        VLDR.W   S0,??DataTable1_6  ;; 0x44340000
        MOVS     R0,R5
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   54 }
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1:
        DC32     0x42f3cccd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_1:
        DC32     0x42ddb852

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_2:
        DC32     0xc3340000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_3:
        DC32     0x43340000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_4:
        DC32     0x43b40000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_5:
        DC32     0x44070000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_6:
        DC32     0x44340000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_7:
        DC32     0x3fd55326

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_8:
        DC32     0x42580000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_9:
        DC32     0x436a0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_10:
        DC32     0x43cf0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_11:
        DC32     0x43fc0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_12:
        DC32     0x40668000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_13:
        DC32     0x40768000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_14:
        DC32     0x4080e000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_15:
        DC32     0x40868000

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
// 
// 598 bytes in section .text
// 
// 598 bytes of CODE memory
//
//Errors: none
//Warnings: 1
