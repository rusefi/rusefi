///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:41 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\cor /
//                       e\EfiWave.cpp                                        /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\cor /
//                       e\EfiWave.cpp -lCN F:\stuff\rusefi_sourceforge\firmw /
//                       are\iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge\ /
//                       firmware\iar\Debug\List\ -o                          /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\Debug\Obj\  /
//                       --no_cse --no_unroll --no_inline --no_code_motion    /
//                       --no_tbaa --no_clustering --no_scheduling --debug    /
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
//                       EfiWave.s                                            /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME EfiWave

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN __aeabi_f2d
        EXTERN firmwareError

        PUBLIC _Z17checkSwitchTimes2iPf
        PUBLIC _ZN12multi_wave_s15baseConstructorEv
        PUBLIC _ZN12multi_wave_s4initEPfP13single_wave_s
        PUBLIC _ZN12multi_wave_s5resetEv
        PUBLIC _ZN12multi_wave_sC1EPfP13single_wave_s
        PUBLIC _ZN12multi_wave_sC1Ev
        PUBLIC _ZN12multi_wave_sC2EPfP13single_wave_s
        PUBLIC _ZN12multi_wave_sC2Ev
        PUBLIC _ZN13single_wave_s4initEPa
        PUBLIC _ZN13single_wave_sC1EPa
        PUBLIC _ZN13single_wave_sC1Ev
        PUBLIC _ZN13single_wave_sC2EPa
        PUBLIC _ZN13single_wave_sC2Ev
        PUBLIC _ZNK12multi_wave_s13getSwitchTimeEi
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\core\EfiWave.cpp
//    1 /**
//    2  * @file	EfiWave.cpp
//    3  *
//    4  * @date May 18, 2014
//    5  * @author Andrey Belomutskiy, (c) 2012-2015
//    6  */
//    7 
//    8 #include "main.h"
//    9 #include "EfiWave.h"
//   10 #include "trigger_structure.h"
//   11 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _ZN13single_wave_sC2Ev
        THUMB
// __code __interwork __softfp single_wave_s::subobject single_wave_s()
_ZN13single_wave_sC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN13single_wave_sC1Ev
        BL       _ZN13single_wave_sC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _ZN13single_wave_sC2EPa
        THUMB
// __code __interwork __softfp single_wave_s::subobject single_wave_s(int8_t *)
_ZN13single_wave_sC2EPa:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN13single_wave_sC1EPa
        BL       _ZN13single_wave_sC1EPa
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _ZN12multi_wave_sC2Ev
        THUMB
// __code __interwork __softfp multi_wave_s::subobject multi_wave_s()
_ZN12multi_wave_sC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN12multi_wave_sC1Ev
        BL       _ZN12multi_wave_sC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock2

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _ZN12multi_wave_sC2EPfP13single_wave_s
        THUMB
// __code __interwork __softfp multi_wave_s::subobject multi_wave_s(float *, single_wave_s *)
_ZN12multi_wave_sC2EPfP13single_wave_s:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN12multi_wave_sC1EPfP13single_wave_s
        BL       _ZN12multi_wave_sC1EPfP13single_wave_s
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _ZN13single_wave_sC1Ev
        THUMB
//   12 single_wave_s::single_wave_s() {
_ZN13single_wave_sC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   13 	init(NULL);
        MOVS     R1,#+0
        MOVS     R0,R4
          CFI FunCall _ZN13single_wave_s4initEPa
        BL       _ZN13single_wave_s4initEPa
//   14 }
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock4
//   15 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _ZN13single_wave_sC1EPa
        THUMB
//   16 single_wave_s::single_wave_s(pin_state_t *ps) {
_ZN13single_wave_sC1EPa:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   17 	init(ps);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _ZN13single_wave_s4initEPa
        BL       _ZN13single_wave_s4initEPa
//   18 }
        MOVS     R0,R4
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock5
//   19 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _ZN13single_wave_s4initEPa
          CFI NoCalls
        THUMB
//   20 void single_wave_s::init(pin_state_t *pinStates) {
//   21 	this->pinStates = pinStates;
_ZN13single_wave_s4initEPa:
        STR      R1,[R0, #+0]
//   22 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock6
//   23 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _ZN12multi_wave_s15baseConstructorEv
        THUMB
//   24 void multi_wave_s::baseConstructor() {
_ZN12multi_wave_s15baseConstructorEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   25 	waves = NULL;
        MOVS     R0,#+0
        STR      R0,[R4, #+4]
//   26 	switchTimes = NULL;
        MOVS     R0,#+0
        STR      R0,[R4, #+8]
//   27 	reset();
        MOVS     R0,R4
          CFI FunCall _ZN12multi_wave_s5resetEv
        BL       _ZN12multi_wave_s5resetEv
//   28 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock7
//   29 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _ZN12multi_wave_sC1Ev
        THUMB
//   30 multi_wave_s::multi_wave_s() {
_ZN12multi_wave_sC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   31 	baseConstructor();
        MOVS     R0,R4
          CFI FunCall _ZN12multi_wave_s15baseConstructorEv
        BL       _ZN12multi_wave_s15baseConstructorEv
//   32 }
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock8
//   33 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _ZN12multi_wave_sC1EPfP13single_wave_s
        THUMB
//   34 multi_wave_s::multi_wave_s(float *switchTimes, single_wave_s *waves) {
_ZN12multi_wave_sC1EPfP13single_wave_s:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   35 	baseConstructor();
        MOVS     R0,R4
          CFI FunCall _ZN12multi_wave_s15baseConstructorEv
        BL       _ZN12multi_wave_s15baseConstructorEv
//   36 	init(switchTimes, waves);
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _ZN12multi_wave_s4initEPfP13single_wave_s
        BL       _ZN12multi_wave_s4initEPfP13single_wave_s
//   37 }
        MOVS     R0,R4
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock9
//   38 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function _ZN12multi_wave_s4initEPfP13single_wave_s
          CFI NoCalls
        THUMB
//   39 void multi_wave_s::init(float *switchTimes, single_wave_s *waves) {
//   40 	this->switchTimes = switchTimes;
_ZN12multi_wave_s4initEPfP13single_wave_s:
        STR      R1,[R0, #+8]
//   41 	this->waves = waves;
        STR      R2,[R0, #+4]
//   42 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock10
//   43 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _ZN12multi_wave_s5resetEv
          CFI NoCalls
        THUMB
//   44 void multi_wave_s::reset(void) {
//   45 	waveCount = 0;
_ZN12multi_wave_s5resetEv:
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//   46 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock11
//   47 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _ZNK12multi_wave_s13getSwitchTimeEi
          CFI NoCalls
        THUMB
//   48 float multi_wave_s::getSwitchTime(int index) const {
//   49 	return switchTimes[index];
_ZNK12multi_wave_s13getSwitchTimeEi:
        LDR      R0,[R0, #+8]
        ADDS     R0,R0,R1, LSL #+2
        VLDR     S0,[R0, #0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock12
//   50 }
//   51 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _Z17checkSwitchTimes2iPf
        THUMB
//   52 void checkSwitchTimes2(int size, float *switchTimes) {
_Z17checkSwitchTimes2iPf:
        PUSH     {R2-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//   53 	if (switchTimes[size - 1] != 1) {
        ADDS     R0,R5,R4, LSL #+2
        SUBS     R0,R0,#+4
        VLDR     S0,[R0, #0]
        VMOV.F32 S1,#1.0
        VCMP.F32 S0,S1
        FMSTAT   
        BEQ.N    ??checkSwitchTimes2_0
//   54 		firmwareError("last switch time has to be 1 not %f", switchTimes[size - 1]);
        ADDS     R0,R5,R4, LSL #+2
        LDR      R0,[R0, #-4]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        LDR.N    R0,??DataTable0
          CFI FunCall firmwareError
        BL       firmwareError
//   55 		return;
        B.N      ??checkSwitchTimes2_1
//   56 	}
//   57 	for (int i = 0; i < size - 1; i++) {
??checkSwitchTimes2_0:
        MOVS     R6,#+0
??checkSwitchTimes2_2:
        SUBS     R0,R4,#+1
        CMP      R6,R0
        BGE.N    ??checkSwitchTimes2_3
//   58 		if (switchTimes[i] >= switchTimes[i + 1]) {
        ADDS     R0,R5,R6, LSL #+2
        VLDR     S0,[R0, #0]
        ADDS     R0,R5,R6, LSL #+2
        VLDR     S1,[R0, #+4]
        VCMP.F32 S0,S1
        FMSTAT   
        BLT.N    ??checkSwitchTimes2_4
//   59 			firmwareError("invalid switchTimes @%d: %f/%f", i, switchTimes[i], switchTimes[i + 1]);
        ADDS     R0,R5,R6, LSL #+2
        LDR      R0,[R0, #+4]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        STRD     R0,R1,[SP, #+0]
        LDR      R0,[R5, R6, LSL #+2]
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        MOVS     R2,R0
        MOVS     R3,R1
        MOVS     R1,R6
        LDR.N    R0,??DataTable0_1
          CFI FunCall firmwareError
        BL       firmwareError
//   60 		}
//   61 	}
??checkSwitchTimes2_4:
        ADDS     R6,R6,#+1
        B.N      ??checkSwitchTimes2_2
//   62 }
??checkSwitchTimes2_3:
??checkSwitchTimes2_1:
        POP      {R0,R1,R4-R6,PC}  ;; return
          CFI EndBlock cfiBlock13

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0:
        DC32     `?<Constant "last switch time has ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_1:
        DC32     `?<Constant "invalid switchTimes @...">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "last switch time has ...">`:
        DATA
        DC8 "last switch time has to be 1 not %f"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "invalid switchTimes @...">`:
        DATA
        DC8 "invalid switchTimes @%d: %f/%f"
        DC8 0

        END
// 
//  68 bytes in section .rodata
// 308 bytes in section .text
// 
// 308 bytes of CODE  memory
//  68 bytes of CONST memory
//
//Errors: none
//Warnings: 1
