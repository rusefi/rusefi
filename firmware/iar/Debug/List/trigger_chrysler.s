///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:57 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\tri /
//                       gger\trigger_chrysler.cpp                            /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\tri /
//                       gger\trigger_chrysler.cpp -lCN                       /
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
//                       trigger_chrysler.s                                   /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME trigger_chrysler

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN _Z28addSkippedToothTriggerEvents15trigger_wheel_eP12TriggerShapeiifffff
        EXTERN _ZN12TriggerShape28setTriggerSynchronizationGapEf
        EXTERN _ZN12TriggerShape29setTriggerSynchronizationGap2Eff
        EXTERN _ZN12TriggerShape5resetE16operation_mode_eb
        EXTERN _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e

        PUBLIC _Z12initDodgeRamP12TriggerShape
        PUBLIC _Z29configureNeon1995TriggerShapeP12TriggerShape
        PUBLIC _Z29configureNeon2003TriggerShapeP12TriggerShape
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\trigger\trigger_chrysler.cpp
//    1 /**
//    2  * @file	trigger_chrysler.cpp
//    3  *
//    4  * @date Mar 24, 2014
//    5  * @author Andrey Belomutskiy, (c) 2012-2015
//    6  */
//    7 
//    8 #include "trigger_chrysler.h"
//    9 #include "trigger_decoder.h"
//   10 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _Z12initDodgeRamP12TriggerShape
        THUMB
//   11 void initDodgeRam(TriggerShape *s) {
_Z12initDodgeRamP12TriggerShape:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   12 
//   13 	s->reset(FOUR_STROKE_CAM_SENSOR, true);
        MOVS     R2,#+1
        MOVS     R1,#+2
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape5resetE16operation_mode_eb
        BL       _ZN12TriggerShape5resetE16operation_mode_eb
//   14 	s->useRiseEdge = true;
        MOVS     R0,#+1
        STRB     R0,[R4, #+2916]
//   15 
//   16 	s->isSynchronizationNeeded = false;
        MOVS     R0,#+0
        STRB     R0,[R4, #+0]
//   17 
//   18 	addSkippedToothTriggerEvents(T_SECONDARY, s, 8, 0, 0.06, -25, 360, 0, 720);
        VLDR.W   S4,??DataTable1  ;; 0x44340000
        VLDR.W   S3,??DataTable1_1  ;; 0x0
        VLDR.W   S2,??DataTable1_2  ;; 0x43b40000
        VMOV.F32 S1,#-25.0
        VLDR.W   S0,??DataTable1_3  ;; 0x3d75c28f
        MOVS     R3,#+0
        MOVS     R2,#+8
        MOVS     R1,R4
        MOVS     R0,#+1
          CFI FunCall _Z28addSkippedToothTriggerEvents15trigger_wheel_eP12TriggerShapeiifffff
        BL       _Z28addSkippedToothTriggerEvents15trigger_wheel_eP12TriggerShapeiifffff
//   19 
//   20 	s->addEvent(360, T_PRIMARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable1_2  ;; 0x43b40000
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   21 
//   22 	addSkippedToothTriggerEvents(T_SECONDARY, s, 8, 0, 0.06, 360 - 25, 360, 0, 720);
        VLDR.W   S4,??DataTable1  ;; 0x44340000
        VLDR.W   S3,??DataTable1_1  ;; 0x0
        VLDR.W   S2,??DataTable1_2  ;; 0x43b40000
        VLDR.W   S1,??DataTable1_4  ;; 0x43a78000
        VLDR.W   S0,??DataTable1_3  ;; 0x3d75c28f
        MOVS     R3,#+0
        MOVS     R2,#+8
        MOVS     R1,R4
        MOVS     R0,#+1
          CFI FunCall _Z28addSkippedToothTriggerEvents15trigger_wheel_eP12TriggerShapeiifffff
        BL       _Z28addSkippedToothTriggerEvents15trigger_wheel_eP12TriggerShapeiifffff
//   23 
//   24 	s->addEvent(720, T_PRIMARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable1  ;; 0x44340000
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   25 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0
//   26 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z29configureNeon2003TriggerShapeP12TriggerShape
        THUMB
//   27 void configureNeon2003TriggerShape(TriggerShape *s) {
_Z29configureNeon2003TriggerShapeP12TriggerShape:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        VPUSH    {D8}
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
//   28 	s->reset(FOUR_STROKE_CAM_SENSOR, false);
        MOVS     R2,#+0
        MOVS     R1,#+2
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape5resetE16operation_mode_eb
        BL       _ZN12TriggerShape5resetE16operation_mode_eb
//   29 
//   30 	// voodoo magic - we always need 720 at the end
//   31 	int base = 10;
        MOVS     R5,#+10
//   32 	float b = 0;
        VLDR.W   S16,??DataTable1_1  ;; 0x0
//   33 	s->useRiseEdge = true;
        MOVS     R0,#+1
        STRB     R0,[R4, #+2916]
//   34 
//   35 	s->gapBothDirections = true;
        MOVS     R0,#+1
        STRB     R0,[R4, #+2917]
//   36 
//   37 	// are these non-default values really needed here now that the gap is finally precise?
//   38 	s->setTriggerSynchronizationGap2(0.5 * CHRYSLER_NGC_GAP, 1.5 * CHRYSLER_NGC_GAP);
        VLDR.W   S1,??DataTable1_5  ;; 0x40bf4c98
        VLDR.W   S0,??DataTable1_6  ;; 0x3fff10cb
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape29setTriggerSynchronizationGap2Eff
        BL       _ZN12TriggerShape29setTriggerSynchronizationGap2Eff
//   39 
//   40 	/*
//   41 	s->addEvent(base + 26, T_PRIMARY, TV_HIGH);
//   42 	s->addEvent(base + 62, T_PRIMARY, TV_LOW);
//   43 
//   44 	s->addEvent(base + 98, T_PRIMARY, TV_HIGH);
//   45 	s->addEvent(base + 134, T_PRIMARY, TV_LOW);
//   46 
//   47 	s->addEvent(base + 180, T_PRIMARY, TV_HIGH);
//   48 	s->addEvent(base + 314, T_PRIMARY, TV_LOW);
//   49 
//   50 	s->addEvent(base + 350, T_PRIMARY, TV_HIGH);
//   51 	s->addEvent(base + 386, T_PRIMARY, TV_LOW);
//   52 
//   53 	s->addEvent(base + 422, T_PRIMARY, TV_HIGH);
//   54 	s->addEvent(base + 458, T_PRIMARY, TV_LOW);
//   55 
//   56 	s->addEvent(base + 494, T_PRIMARY, TV_HIGH);
//   57 	s->addEvent(base + 530, T_PRIMARY, TV_LOW);
//   58 
//   59 	s->addEvent(base + 674, T_PRIMARY, TV_HIGH);
//   60 	s->addEvent(base + 710, T_PRIMARY, TV_LOW);
//   61 	 *
//   62 	 */
//   63 
//   64 	s->addEvent(143.0712499, T_PRIMARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable1_7  ;; 0x430f123d
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   65 	s->addEvent(182.684791, T_PRIMARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable1_8  ;; 0x4336af4e
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   66 
//   67 	s->addEvent(215.424166, T_PRIMARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable1_9  ;; 0x43576c96
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   68 	s->addEvent(256.247395, T_PRIMARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable1_10  ;; 0x43801fab
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   69 
//   70 	s->addEvent(288.659999, T_PRIMARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable1_11  ;; 0x4390547b
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   71 	s->addEvent(326.778229, T_PRIMARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable1_12  ;; 0x43a3639d
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   72 
//   73 	s->addEvent(361.522083, T_PRIMARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable1_13  ;; 0x43b4c2d4
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   74 	s->addEvent(508.960416, T_PRIMARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable1_14  ;; 0x43fe7aef
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   75 
//   76 	s->addEvent(545.635, T_PRIMARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable1_15  ;; 0x440868a4
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   77 	s->addEvent(582.98864, T_PRIMARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable1_16  ;; 0x4411bf46
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   78 
//   79 	s->addEvent(617.22989, T_PRIMARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable1_17  ;; 0x441a4eb7
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   80 	s->addEvent(653.38625, T_PRIMARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable2  ;; 0x442358b8
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   81 
//   82 	s->addEvent(684.1080, T_PRIMARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable2_1  ;; 0x442b06e9
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   83 	s->addEvent(720, T_PRIMARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+0
        VLDR.W   S0,??DataTable1  ;; 0x44340000
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   84 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1:
        DC32     0x44340000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_1:
        DC32     0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_2:
        DC32     0x43b40000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_3:
        DC32     0x3d75c28f

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_4:
        DC32     0x43a78000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_5:
        DC32     0x40bf4c98

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_6:
        DC32     0x3fff10cb

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_7:
        DC32     0x430f123d

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_8:
        DC32     0x4336af4e

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_9:
        DC32     0x43576c96

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_10:
        DC32     0x43801fab

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_11:
        DC32     0x4390547b

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_12:
        DC32     0x43a3639d

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_13:
        DC32     0x43b4c2d4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_14:
        DC32     0x43fe7aef

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_15:
        DC32     0x440868a4

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_16:
        DC32     0x4411bf46

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_17:
        DC32     0x441a4eb7
//   85 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z29configureNeon1995TriggerShapeP12TriggerShape
        THUMB
//   86 void configureNeon1995TriggerShape(TriggerShape *s) {
_Z29configureNeon1995TriggerShapeP12TriggerShape:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//   87 	s->reset(FOUR_STROKE_CAM_SENSOR, true);
        MOVS     R2,#+1
        MOVS     R1,#+2
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape5resetE16operation_mode_eb
        BL       _ZN12TriggerShape5resetE16operation_mode_eb
//   88 
//   89 	s->setTriggerSynchronizationGap(0.72);
        VLDR.W   S0,??DataTable2_2  ;; 0x3f3851ec
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape28setTriggerSynchronizationGapEf
        BL       _ZN12TriggerShape28setTriggerSynchronizationGapEf
//   90 
//   91 	s->useRiseEdge = false;
        MOVS     R0,#+0
        STRB     R0,[R4, #+2916]
//   92 
//   93 	// voodoo magic - we always need 720 at the end
//   94 	int base = 720 - 560;
        MOVS     R5,#+160
//   95 
//   96 	s->initialState[T_PRIMARY] = 1;
        MOVS     R0,#+1
        STR      R0,[R4, #+2944]
//   97 
//   98 	s->addEvent(base - 720 + 600, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        SUBS     R0,R5,#+120
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//   99 	s->addEvent(base - 720 + 604, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        SUBS     R0,R5,#+116
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  100 	s->addEvent(base - 720 + 616, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        SUBS     R0,R5,#+104
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  101 	s->addEvent(base - 720 + 620, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        SUBS     R0,R5,#+100
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  102 	s->addEvent(base - 720 + 643, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        SUBS     R0,R5,#+77
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  103 	s->addEvent(base - 720 + 648, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        SUBS     R0,R5,#+72
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  104 	s->addEvent(base - 720 + 671, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        SUBS     R0,R5,#+49
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  105 	s->addEvent(base - 720 + 676, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        SUBS     R0,R5,#+44
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  106 
//  107 	s->addEvent(base + 0, T_PRIMARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+0
        VMOV     S0,R5
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  108 
//  109 	s->addEvent(base + 20, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        ADDS     R0,R5,#+20
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  110 	s->addEvent(base + 60, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        ADDS     R0,R5,#+60
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  111 	s->addEvent(base + 75, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        ADDS     R0,R5,#+75
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  112 	s->addEvent(base + 79, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        ADDS     R0,R5,#+79
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  113 	s->addEvent(base + 101, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        ADDS     R0,R5,#+101
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  114 	s->addEvent(base + 106, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        ADDS     R0,R5,#+106
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  115 	s->addEvent(base + 130, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        ADDS     R0,R5,#+130
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  116 	s->addEvent(base + 135, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        ADDS     R0,R5,#+135
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  117 
//  118 	s->addEvent(base + 200, T_PRIMARY, TV_HIGH); // width = 150
        MOVS     R2,#+1
        MOVS     R1,#+0
        ADDS     R0,R5,#+200
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  119 
//  120 	s->addEvent(base + 236, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        ADDS     R0,R5,#+236
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  121 	s->addEvent(base + 239, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        ADDS     R0,R5,#+239
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  122 	s->addEvent(base + 250, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        ADDS     R0,R5,#+250
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  123 	s->addEvent(base + 255, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        ADDS     R0,R5,#+255
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  124 	s->addEvent(base + 277, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        ADDW     R0,R5,#+277
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  125 	s->addEvent(base + 282, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        ADDS     R0,R5,#+282
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  126 	s->addEvent(base + 305, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        ADDW     R0,R5,#+305
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  127 	s->addEvent(base + 310, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        ADDS     R0,R5,#+310
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  128 
//  129 	s->addEvent(base + 374, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        ADDS     R0,R5,#+374
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  130 
//  131 	s->addEvent(base + 395, T_PRIMARY, TV_LOW); // width =
        MOVS     R2,#+0
        MOVS     R1,#+0
        ADDW     R0,R5,#+395
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  132 
//  133 	s->addEvent(base + 418, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        ADDS     R0,R5,#+418
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  134 	s->addEvent(base + 436, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        ADDS     R0,R5,#+436
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  135 	s->addEvent(base + 441, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        ADDW     R0,R5,#+441
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  136 	s->addEvent(base + 463, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        ADDW     R0,R5,#+463
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  137 	s->addEvent(base + 468, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        ADDS     R0,R5,#+468
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  138 	s->addEvent(base + 492, T_SECONDARY, TV_HIGH);
        MOVS     R2,#+1
        MOVS     R1,#+1
        ADDS     R0,R5,#+492
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  139 	s->addEvent(base + 497, T_SECONDARY, TV_LOW);
        MOVS     R2,#+0
        MOVS     R1,#+1
        ADDW     R0,R5,#+497
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  140 
//  141 	s->addEvent(base + 560, T_PRIMARY, TV_HIGH); // width =
        MOVS     R2,#+1
        MOVS     R1,#+0
        ADDS     R0,R5,#+560
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        MOVS     R0,R4
          CFI FunCall _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
        BL       _ZN12TriggerShape8addEventEf15trigger_wheel_e15trigger_value_e
//  142 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2:
        DC32     0x442358b8

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_1:
        DC32     0x442b06e9

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_2:
        DC32     0x3f3851ec

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
//  143 
// 
// 1 282 bytes in section .text
// 
// 1 282 bytes of CODE memory
//
//Errors: none
//Warnings: 3
