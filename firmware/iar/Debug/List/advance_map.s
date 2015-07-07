///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:13 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\alg /
//                       o\advance_map.cpp                                    /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\alg /
//                       o\advance_map.cpp -lCN F:\stuff\rusefi_sourceforge\f /
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
//                       advance_map.s                                        /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME advance_map

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z12setTableBin2Pfifff
        EXTERN _Z13interpolate3dfPfifS_iPS_
        EXTERN _Z15copyTimingTablePA16_KfPA16_f
        EXTERN __aeabi_memcpy
        EXTERN __aeabi_memset
        EXTERN __iar_FDtest
        EXTERN boardConfiguration
        EXTERN config
        EXTERN engine
        EXTERN engineConfiguration
        EXTERN firmwareError
        EXTERN persistentState
        EXTERN warning

        PUBLIC _Z10getAdvanceif
        PUBLIC _Z16isStep1Conditioni
        PUBLIC _Z16prepareTimingMapv
        PUBLIC _Z29setDefaultIatTimingCorrectionv
        PUBLIC _ZN5Map3DILi16ELi16EE4initEPA16_fPfS3_
        PUBLIC _ZN5Map3DILi16ELi16EE8getValueEff
        PUBLIC _ZN5Map3DILi16ELi16EEC1Ev
        PUBLIC _ZN5Map3DILi16ELi16EEC2Ev
        PUBLIC _ZZN5Map3DILi16ELi16EE8getValueEffEs
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\algo\advance_map.cpp
//    1 /**
//    2  * @file	advance_map.cpp
//    3  *
//    4  * @date Mar 27, 2013
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
//   22 #include "advance_map.h"
//   23 #include "interpolation.h"
//   24 #include "efilib2.h"
//   25 #include "engine_configuration.h"
//   26 #include "engine_math.h"
//   27 
//   28 EXTERN_ENGINE;
//   29 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function __sti__routine
        THUMB
// static __intrinsic __interwork __softfp void __sti__routine()
__sti__routine:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   30 static ign_Map3D_t advanceMap;
        LDR.N    R0,??DataTable5_3
          CFI FunCall _ZN5Map3DILi16ELi16EEC1Ev
        BL       _ZN5Map3DILi16ELi16EEC1Ev
//   31 static ign_Map3D_t iatAdvanceCorrectionMap;
        LDR.N    R0,??DataTable5_4
          CFI FunCall _ZN5Map3DILi16ELi16EEC1Ev
        BL       _ZN5Map3DILi16ELi16EEC1Ev
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
advanceMap:
        DS8 76

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
iatAdvanceCorrectionMap:
        DS8 76

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        SECTION_GROUP _ZZN5Map3DILi16ELi16EE8getValueEffEs
// __absolute char const <_ZZN5Map3DILi16ELi16EE8getValueEffEs>[20]
_ZZN5Map3DILi16ELi16EE8getValueEffEs:
        DATA
        DC8 "map not initialized"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "NaN engine load">`:
        DATA
        DC8 "NaN engine load"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid el">`:
        DATA
        DC8 "invalid el"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid rpm">`:
        DATA
        DC8 "invalid rpm"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "zero engineCycle">`:
        DATA
        DC8 "zero engineCycle"
        DC8 0, 0, 0
//   32 

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//   33 static const float iatTimingRpmBins[IGN_LOAD_COUNT] = {880,	1260,	1640,	2020,	2400,	2780,	3000,	3380,	3760,	4140,	4520,	5000,	5700,	6500,	7200,	8000};
iatTimingRpmBins:
        DATA
        DC32 445C0000H, 449D8000H, 44CD0000H, 44FC8000H, 45160000H, 452DC000H
        DC32 453B8000H, 45534000H, 456B0000H, 45816000H, 458D4000H, 459C4000H
        DC32 45B22000H, 45CB2000H, 45E10000H, 45FA0000H
//   34 
//   35 //880	1260	1640	2020	2400	2780	3000	3380	3760	4140	4520	5000	5700	6500	7200	8000

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//   36 static const ignition_table_t defaultIatTiming = {
defaultIatTiming:
        DATA
        DC32 40800000H, 40800000H, 40800000H, 40800000H, 40800000H, 40800000H
        DC32 40800000H, 40800000H, 40800000H, 40800000H, 40800000H, 40000000H
        DC32 40000000H, 40000000H, 40000000H, 40000000H, 40800000H, 40800000H
        DC32 40800000H, 40800000H, 40800000H, 40800000H, 40800000H, 40800000H
        DC32 40800000H, 40800000H, 40800000H, 40000000H, 40000000H, 40000000H
        DC32 40000000H, 40000000H, 40800000H, 40800000H, 40800000H, 40800000H
        DC32 40800000H, 40800000H, 40800000H, 40800000H, 40800000H, 40800000H
        DC32 40800000H, 40000000H, 40000000H, 40000000H, 40000000H, 40000000H
        DC32 40800000H, 40800000H, 40800000H, 40800000H, 40800000H, 40800000H
        DC32 40800000H, 40800000H, 40800000H, 40800000H, 40800000H, 40000000H
        DC32 40000000H, 40000000H, 40000000H, 40000000H, 40600000H, 40600000H
        DC32 40600000H, 40600000H, 40600000H, 40600000H, 40600000H, 40600000H
        DC32 40600000H, 40600000H, 40600000H, 40000000H, 40000000H, 40000000H
        DC32 40000000H, 40000000H, 40400000H, 40400000H, 40400000H, 40400000H
        DC32 40400000H, 40400000H, 40400000H, 40400000H, 40400000H, 40400000H
        DC32 40400000H, 40000000H, 40000000H, 40000000H, 40000000H, 40000000H
        DC32 40000000H, 40000000H, 40000000H, 40000000H, 40000000H, 40000000H
        DC32 40000000H, 40000000H, 40000000H, 40000000H, 40000000H, 0H, 0H, 0H
        DC32 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H
        DC32 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H, 0H
        DC32 0H, 0H, 0BF666666H, 0BF666666H, 0BF666666H, 0BF666666H, 0BF666666H
        DC32 0BF666666H, 0BF666666H, 0BF666666H, 0BF666666H, 0BF666666H
        DC32 0BF666666H, 0BF666666H, 0BF666666H, 0BF666666H, 0C0533333H
        DC32 0C059999AH, 0C09CCCCDH, 0C09CCCCDH, 0C09CCCCDH, 0C09CCCCDH
        DC32 0C08CCCCDH, 0C08CCCCDH, 0C08CCCCDH, 0C08CCCCDH, 0C08CCCCDH
        DC32 0BF666666H, 0BF666666H, 0BF666666H, 0BF666666H, 0BF666666H
        DC32 0C08CCCCDH, 0C09CCCCDH, 0C0BCCCCDH, 0C0BCCCCDH, 0C0BCCCCDH
        DC32 0C0BCCCCDH, 0C09CCCCDH, 0C09CCCCDH, 0C09CCCCDH, 0C09CCCCDH
        DC32 0C09CCCCDH, 0C019999AH, 0C019999AH, 0C019999AH, 0C019999AH
        DC32 0C019999AH, 0C08CCCCDH, 0C09CCCCDH, 0C0BCCCCDH, 0C0BCCCCDH
        DC32 0C0BCCCCDH, 0C0BCCCCDH, 0C09CCCCDH, 0C09CCCCDH, 0C09CCCCDH
        DC32 0C09CCCCDH, 0C09CCCCDH, 0C039999AH, 0C039999AH, 0C039999AH
        DC32 0C039999AH, 0C039999AH, 0C08CCCCDH, 0C09CCCCDH, 0C0BCCCCDH
        DC32 0C0BCCCCDH, 0C0BCCCCDH, 0C0BCCCCDH, 0C09CCCCDH, 0C09CCCCDH
        DC32 0C09CCCCDH, 0C09CCCCDH, 0C09CCCCDH, 0C079999AH, 0C079999AH
        DC32 0C079999AH, 0C079999AH, 0C079999AH, 0C08CCCCDH, 0C09CCCCDH
        DC32 0C0BCCCCDH, 0C0BCCCCDH, 0C0BCCCCDH, 0C0BCCCCDH, 0C09CCCCDH
        DC32 0C09CCCCDH, 0C09CCCCDH, 0C09CCCCDH, 0C09CCCCDH, 0C079999AH
        DC32 0C079999AH, 0C079999AH, 0C079999AH, 0C079999AH, 0C08CCCCDH
        DC32 0C09CCCCDH, 0C0BCCCCDH, 0C0BCCCCDH, 0C0BCCCCDH, 0C0BCCCCDH
        DC32 0C09CCCCDH, 0C09CCCCDH, 0C09CCCCDH, 0C09CCCCDH, 0C09CCCCDH
        DC32 0C079999AH, 0C079999AH, 0C079999AH, 0C079999AH, 0C079999AH
//   37 		{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2},
//   38 		{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2},
//   39 		{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2},
//   40 		{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2},
//   41 		{3.5, 3.5, 3.5, 3.5, 3.5, 3.5, 3.5, 3.5, 3.5, 3.5, 3.5, 2, 2, 2, 2, 2},
//   42 		{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2},
//   43 		{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0},
//   44 		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//   45 		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//   46 		{ 0, 0, -0.9, -0.9, -0.9, -0.9, -0.9, -0.9, -0.9, -0.9, -0.9, -0.9, -0.9, -0.9, -0.9, -0.9},
//   47 		{ -3.3, -3.4, -4.9, -4.9, -4.9, -4.9, -4.4, -4.4, -4.4, -4.4, -4.4, -0.9, -0.9, -0.9, -0.9, -0.9},
//   48 		{ -4.4, -4.9, -5.9, -5.9, -5.9, -5.9, -4.9, -4.9, -4.9, -4.9, -4.9, -2.4, -2.4, -2.4, -2.4, -2.4},
//   49 		{ -4.4, -4.9, -5.9, -5.9, -5.9, -5.9, -4.9, -4.9, -4.9, -4.9, -4.9, -2.9, -2.9, -2.9, -2.9, -2.9},
//   50 		{-4.4, -4.9, -5.9, -5.9, -5.9, -5.9, -4.9, -4.9, -4.9, -4.9, -4.9, -3.9, -3.9, -3.9, -3.9, -3.9},
//   51 		{-4.4, -4.9, -5.9, -5.9, -5.9, -5.9, -4.9, -4.9, -4.9, -4.9, -4.9, -3.9, -3.9, -3.9, -3.9, -3.9},
//   52 		{-4.4, -4.9, -5.9, -5.9, -5.9, -5.9, -4.9, -4.9, -4.9, -4.9, -4.9, -3.9, -3.9, -3.9, -3.9, -3.9},
//   53 };
//   54 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _Z16isStep1Conditioni
          CFI NoCalls
        THUMB
//   55 bool_t isStep1Condition(int rpm DECLARE_ENGINE_PARAMETER_S) {
//   56 	return  boardConfiguration->enabledStep1Limiter && rpm >= engineConfiguration->step1rpm;
_Z16isStep1Conditioni:
        LDR.N    R1,??DataTable5_5
        LDR      R1,[R1, #+0]
        LDRB     R1,[R1, #+385]
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LSRS     R1,R1,#+2
        ANDS     R1,R1,#0x1
        CMP      R1,#+0
        BEQ.N    ??isStep1Condition_0
        LDR.N    R1,??DataTable5_6
        LDR      R1,[R1, #+0]
        LDR      R1,[R1, #+320]
        CMP      R0,R1
        BLT.N    ??isStep1Condition_0
        MOVS     R0,#+1
        B.N      ??isStep1Condition_1
??isStep1Condition_0:
        MOVS     R0,#+0
??isStep1Condition_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   57 }
//   58 
//   59 /**
//   60  * @return ignition timing angle advance before TDC
//   61  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _Z17getRunningAdvanceif
        THUMB
//   62 static angle_t getRunningAdvance(int rpm, float engineLoad DECLARE_ENGINE_PARAMETER_S) {
_Z17getRunningAdvanceif:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        VPUSH    {D8-D9}
          CFI D9 Frame(CFA, -16)
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        VMOV     S16,R0
        VMOV.F32 S17,S0
//   63 	engine->m.beforeAdvance = GET_TIMESTAMP();
        MOVW     R0,#+9192
        LDR.N    R1,??DataTable5_7
        LDR      R1,[R1, #+0]
        LDR.N    R2,??DataTable5_8  ;; 0xe0001004
        LDR      R2,[R2, #+0]
        STR      R2,[R0, R1]
//   64 	if (cisnan(engineLoad)) {
        VMOV.F32 S0,S17
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BNE.N    ??getRunningAdvance_0
//   65 		warning(OBD_PCM_Processor_Fault, "NaN engine load");
        LDR.N    R1,??DataTable5_9
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//   66 		return NAN;
        VLDR.W   S0,??DataTable5  ;; 0x7fffffff
        B.N      ??getRunningAdvance_1
//   67 	}
//   68 	efiAssert(!cisnan(engineLoad), "invalid el", NAN);
??getRunningAdvance_0:
        VMOV.F32 S0,S17
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BNE.N    ??getRunningAdvance_2
        LDR.N    R0,??DataTable5_10
          CFI FunCall firmwareError
        BL       firmwareError
        VLDR.W   S0,??DataTable5  ;; 0x7fffffff
        B.N      ??getRunningAdvance_1
//   69 	efiAssert(!cisnan(engineLoad), "invalid rpm", NAN);
??getRunningAdvance_2:
        VMOV.F32 S0,S17
          CFI FunCall __iar_FDtest
        BL       __iar_FDtest
        CMP      R0,#+2
        BNE.N    ??getRunningAdvance_3
        LDR.N    R0,??DataTable5_11
          CFI FunCall firmwareError
        BL       firmwareError
        VLDR.W   S0,??DataTable5  ;; 0x7fffffff
        B.N      ??getRunningAdvance_1
//   70 	engine->m.beforeZeroTest = GET_TIMESTAMP();
??getRunningAdvance_3:
        MOVW     R0,#+9184
        LDR.N    R1,??DataTable5_7
        LDR      R1,[R1, #+0]
        LDR.N    R2,??DataTable5_8  ;; 0xe0001004
        LDR      R2,[R2, #+0]
        STR      R2,[R0, R1]
//   71 	engine->m.zeroTestTime = GET_TIMESTAMP() - engine->m.beforeZeroTest;
        LDR.N    R0,??DataTable5_8  ;; 0xe0001004
        LDR      R0,[R0, #+0]
        MOVW     R1,#+9184
        LDR.N    R2,??DataTable5_7
        LDR      R2,[R2, #+0]
        LDR      R1,[R1, R2]
        SUBS     R0,R0,R1
        MOVW     R1,#+9188
        LDR.N    R2,??DataTable5_7
        LDR      R2,[R2, #+0]
        STR      R0,[R1, R2]
//   72 
//   73 	if (isStep1Condition(rpm PASS_ENGINE_PARAMETER)) {
        VMOV     R0,S16
          CFI FunCall _Z16isStep1Conditioni
        BL       _Z16isStep1Conditioni
        CMP      R0,#+0
        BEQ.N    ??getRunningAdvance_4
//   74 		return engineConfiguration->step1timing;
        LDR.N    R0,??DataTable5_6
        LDR      R0,[R0, #+0]
        VLDR     S0,[R0, #+324]
        VCVT.F32.S32 S0,S0
        B.N      ??getRunningAdvance_1
//   75 	}
//   76 
//   77 	float iatCorrection = iatAdvanceCorrectionMap.getValue(engine->engineState.clt, (float) rpm);
??getRunningAdvance_4:
        VCVT.F32.S32 S1,S16
        LDR.N    R0,??DataTable5_7
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+7680
        ADDS     R0,R0,#+84
        VLDR     S0,[R0, #0]
        LDR.N    R0,??DataTable5_4
          CFI FunCall _ZN5Map3DILi16ELi16EE8getValueEff
        BL       _ZN5Map3DILi16ELi16EE8getValueEff
        VMOV.F32 S18,S0
//   78 
//   79 	float result = advanceMap.getValue(engineLoad, (float) rpm) + iatCorrection
//   80 			// todo: uncomment once we get useable knock   - engine->knockCount
//   81 			;
        VCVT.F32.S32 S1,S16
        VMOV.F32 S0,S17
        LDR.N    R0,??DataTable5_3
          CFI FunCall _ZN5Map3DILi16ELi16EE8getValueEff
        BL       _ZN5Map3DILi16ELi16EE8getValueEff
        VADD.F32 S0,S0,S18
//   82 	engine->m.advanceLookupTime = GET_TIMESTAMP() - engine->m.beforeAdvance;
        LDR.N    R0,??DataTable5_8  ;; 0xe0001004
        LDR      R0,[R0, #+0]
        MOVW     R1,#+9192
        LDR.N    R2,??DataTable5_7
        LDR      R2,[R2, #+0]
        LDR      R1,[R1, R2]
        SUBS     R0,R0,R1
        MOVW     R1,#+9196
        LDR.N    R2,??DataTable5_7
        LDR      R2,[R2, #+0]
        STR      R0,[R1, R2]
//   83 	return result;
??getRunningAdvance_1:
        VPOP     {D8-D9}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI CFA R13+8
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock2
//   84 }
//   85 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _Z10getAdvanceif
        THUMB
//   86 angle_t getAdvance(int rpm, float engineLoad DECLARE_ENGINE_PARAMETER_S) {
_Z10getAdvanceif:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        VMOV.F32 S16,S0
//   87 	angle_t angle;
//   88 	if (isCrankingR(rpm)) {
        CMP      R4,#+1
        BLT.N    ??getAdvance_0
        LDR.N    R0,??DataTable5_12
        LDRSH    R0,[R0, #+104]
        CMP      R4,R0
        BGE.N    ??getAdvance_0
//   89 		angle = engineConfiguration->crankingTimingAngle;
        LDR.N    R0,??DataTable5_6
        LDR      R0,[R0, #+0]
        VLDR     S0,[R0, #+440]
        VMOV.F32 S17,S0
        B.N      ??getAdvance_1
//   90 	} else {
//   91 		angle = getRunningAdvance(rpm, engineLoad PASS_ENGINE_PARAMETER);
??getAdvance_0:
        VMOV.F32 S0,S16
        MOVS     R0,R4
          CFI FunCall _Z17getRunningAdvanceif
        BL       _Z17getRunningAdvanceif
        VMOV.F32 S17,S0
//   92 	}
//   93 	angle -= engineConfiguration->ignitionOffset;
??getAdvance_1:
        LDR.N    R0,??DataTable5_6
        LDR      R0,[R0, #+0]
        VLDR     S0,[R0, #+448]
        VSUB.F32 S17,S17,S0
//   94 	fixAngle(angle);
        LDR.N    R0,??DataTable5_12
        LDR      R0,[R0, #+424]
        CMP      R0,#+0
        BNE.N    ??getAdvance_2
        LDR.N    R0,??DataTable5_13
          CFI FunCall firmwareError
        BL       firmwareError
??getAdvance_2:
        VCMP.F32 S17,#0.0
        FMSTAT   
        BPL.N    ??getAdvance_3
        LDR.N    R0,??DataTable5_12
        VLDR     S0,[R0, #+424]
        VCVT.F32.S32 S0,S0
        VADD.F32 S17,S0,S17
        B.N      ??getAdvance_2
??getAdvance_3:
        LDR.N    R0,??DataTable5_12
        VLDR     S0,[R0, #+424]
        VCVT.F32.S32 S0,S0
        VCMP.F32 S17,S0
        FMSTAT   
        BLT.N    ??getAdvance_4
        LDR.N    R0,??DataTable5_12
        VLDR     S0,[R0, #+424]
        VCVT.F32.S32 S0,S0
        VSUB.F32 S17,S17,S0
        B.N      ??getAdvance_3
//   95 	return angle;
??getAdvance_4:
        VMOV.F32 S0,S17
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//   96 }
//   97 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _Z29setDefaultIatTimingCorrectionv
        THUMB
//   98 void setDefaultIatTimingCorrection(DECLARE_ENGINE_PARAMETER_F) {
_Z29setDefaultIatTimingCorrectionv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//   99 	setTableBin2(config->ignitionIatCorrLoadBins, IGN_LOAD_COUNT, -40, 110, 1);
        VMOV.F32 S2,#1.0
        VLDR.W   S1,??DataTable5_1  ;; 0x42dc0000
        VLDR.W   S0,??DataTable5_2  ;; 0xc2200000
        MOVS     R1,#+16
        LDR.N    R0,??DataTable5_14
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+9216
        ADDS     R0,R0,#+184
          CFI FunCall _Z12setTableBin2Pfifff
        BL       _Z12setTableBin2Pfifff
//  100 	memcpy(config->ignitionIatCorrRpmBins, iatTimingRpmBins, sizeof(iatTimingRpmBins));
        MOVS     R2,#+64
        LDR.N    R1,??DataTable5_15
        LDR.N    R0,??DataTable5_14
        LDR      R0,[R0, #+0]
        ADD      R0,R0,#+9216
        ADDS     R0,R0,#+248
          CFI FunCall memcpy
        BL       memcpy
//  101 	copyTimingTable(defaultIatTiming, config->ignitionIatCorrTable);
        LDR.N    R0,??DataTable5_14
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+8192
        ADDS     R1,R1,#+184
        LDR.N    R0,??DataTable5_16
          CFI FunCall _Z15copyTimingTablePA16_KfPA16_f
        BL       _Z15copyTimingTablePA16_KfPA16_f
//  102 }
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock4

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _Z16prepareTimingMapv
        THUMB
_Z16prepareTimingMapv:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
        LDR.N    R0,??DataTable5_14
        LDR      R0,[R0, #+0]
        ADD      R3,R0,#+12800
        ADDS     R3,R3,#+120
        LDR.N    R0,??DataTable5_14
        LDR      R0,[R0, #+0]
        ADD      R2,R0,#+12800
        ADDS     R2,R2,#+56
        LDR.N    R0,??DataTable5_14
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+11776
        ADDS     R1,R1,#+56
        LDR.N    R0,??DataTable5_3
          CFI FunCall _ZN5Map3DILi16ELi16EE4initEPA16_fPfS3_
        BL       _ZN5Map3DILi16ELi16EE4initEPA16_fPfS3_
        LDR.N    R0,??DataTable5_14
        LDR      R0,[R0, #+0]
        ADD      R3,R0,#+9216
        ADDS     R3,R3,#+248
        LDR.N    R0,??DataTable5_14
        LDR      R0,[R0, #+0]
        ADD      R2,R0,#+9216
        ADDS     R2,R2,#+184
        LDR.N    R0,??DataTable5_14
        LDR      R0,[R0, #+0]
        ADD      R1,R0,#+8192
        ADDS     R1,R1,#+184
        LDR.N    R0,??DataTable5_4
          CFI FunCall _ZN5Map3DILi16ELi16EE4initEPA16_fPfS3_
        BL       _ZN5Map3DILi16ELi16EE4initEPA16_fPfS3_
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock5

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5:
        DC32     0x7fffffff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_1:
        DC32     0x42dc0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_2:
        DC32     0xc2200000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_3:
        DC32     advanceMap

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_4:
        DC32     iatAdvanceCorrectionMap

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_5:
        DC32     boardConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_6:
        DC32     engineConfiguration

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_7:
        DC32     engine

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_8:
        DC32     0xe0001004

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_9:
        DC32     `?<Constant "NaN engine load">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_10:
        DC32     `?<Constant "invalid el">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_11:
        DC32     `?<Constant "invalid rpm">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_12:
        DC32     persistentState

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_13:
        DC32     `?<Constant "zero engineCycle">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_14:
        DC32     config

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_15:
        DC32     iatTimingRpmBins

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable5_16:
        DC32     defaultIatTiming

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memcpy
          CFI Block cfiBlock6 Using cfiCommon0
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
          CFI EndBlock cfiBlock6

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memset
          CFI Block cfiBlock7 Using cfiCommon0
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
          CFI EndBlock cfiBlock7

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN5Map3DILi16ELi16EE4initEPA16_fPfS3_
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _ZN5Map3DILi16ELi16EE4initEPA16_fPfS3_
          CFI NoCalls
        THUMB
// __interwork __softfp void Map3D<16, 16>::init(float (*)[16], float *, float *)
_ZN5Map3DILi16ELi16EE4initEPA16_fPfS3_:
        PUSH     {R4,R5}
          CFI R5 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,#+0
??init_0:
        CMP      R4,#+16
        BGE.N    ??init_1
        MOVS     R5,#+64
        MLA      R5,R5,R4,R1
        STR      R5,[R0, R4, LSL #+2]
        ADDS     R4,R4,#+1
        B.N      ??init_0
??init_1:
        LDR.N    R4,??init_2      ;; 0x923f558
        STR      R4,[R0, #+72]
        STR      R2,[R0, #+64]
        STR      R3,[R0, #+68]
        POP      {R4,R5}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
        Nop      
        DATA
??init_2:
        DC32     0x923f558
          CFI EndBlock cfiBlock8

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN5Map3DILi16ELi16EE8getValueEff
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _ZN5Map3DILi16ELi16EE8getValueEff
        THUMB
// __interwork __vfp float Map3D<16, 16>::getValue(float, float)
_ZN5Map3DILi16ELi16EE8getValueEff:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+8
          CFI CFA R13+24
        MOVS     R4,R0
        VMOV.F32 S16,S0
        VMOV.F32 S17,S1
        LDR      R0,[R4, #+72]
        LDR.N    R1,??getValue_0+0x4  ;; 0x923f558
        CMP      R0,R1
        BEQ.N    ??getValue_1
        LDR.N    R0,??getValue_0+0x8
          CFI FunCall firmwareError
        BL       firmwareError
        VLDR.W   S0,??getValue_0  ;; 0x7fffffff
        B.N      ??getValue_2
??getValue_1:
        STR      R4,[SP, #+0]
        MOVS     R3,#+16
        LDR      R2,[R4, #+68]
        VMOV.F32 S1,S17
        MOVS     R1,#+16
        LDR      R0,[R4, #+64]
        VMOV.F32 S0,S16
          CFI FunCall _Z13interpolate3dfPfifS_iPS_
        BL       _Z13interpolate3dfPfifS_iPS_
??getValue_2:
        ADD      SP,SP,#+8
          CFI CFA R13+16
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
        DATA
??getValue_0:
        DC32     0x7fffffff
        DC32     0x923f558
        DC32     _ZZN5Map3DILi16ELi16EE8getValueEffEs
          CFI EndBlock cfiBlock9

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN5Map3DILi16ELi16EEC1Ev
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _ZN5Map3DILi16ELi16EEC1Ev
        THUMB
// __code __interwork __softfp Map3D<16, 16>::Map3D()
_ZN5Map3DILi16ELi16EEC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,#+0
        STR      R0,[R4, #+72]
        MOVS     R2,#+64
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall memset
        BL       memset
        MOVS     R0,#+0
        STR      R0,[R4, #+64]
        MOVS     R0,#+0
        STR      R0,[R4, #+68]
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock10

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

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN5Map3DILi16ELi16EEC2Ev
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _ZN5Map3DILi16ELi16EEC2Ev
        THUMB
// __code __interwork __softfp Map3D<16, 16>::subobject Map3D()
_ZN5Map3DILi16ELi16EEC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN5Map3DILi16ELi16EEC1Ev
        BL       _ZN5Map3DILi16ELi16EEC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock11

        END
//  103 
//  104 void prepareTimingMap(DECLARE_ENGINE_PARAMETER_F) {
//  105 	advanceMap.init(config->ignitionTable, config->ignitionLoadBins,
//  106 			config->ignitionRpmBins);
//  107 	iatAdvanceCorrectionMap.init(config->ignitionIatCorrTable, config->ignitionIatCorrLoadBins,
//  108 			config->ignitionIatCorrRpmBins);
//  109 }
// 
//   152 bytes in section .bss
//     4 bytes in section .init_array
// 1 168 bytes in section .rodata
//   876 bytes in section .text
// 
//   674 bytes of CODE  memory (+ 206 bytes shared)
// 1 148 bytes of CONST memory (+  20 bytes shared)
//   152 bytes of DATA  memory
//
//Errors: none
//Warnings: 1
