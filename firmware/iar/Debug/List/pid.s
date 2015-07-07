///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:57 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\mat /
//                       h\pid.cpp                                            /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\mat /
//                       h\pid.cpp -lCN F:\stuff\rusefi_sourceforge\firmware\ /
//                       iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge\firm /
//                       ware\iar\Debug\List\ -o F:\stuff\rusefi_sourceforge\ /
//                       firmware\iar\Debug\Obj\ --no_cse --no_unroll         /
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
//                       pid.s                                                /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME pid

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        PUBLIC _ZN3Pid13updateFactorsEfff
        PUBLIC _ZN3Pid14getIntegrationEv
        PUBLIC _ZN3Pid4getDEv
        PUBLIC _ZN3Pid4getIEv
        PUBLIC _ZN3Pid4getPEv
        PUBLIC _ZN3Pid5resetEv
        PUBLIC _ZN3Pid8getValueEfff
        PUBLIC _ZN3PidC1Efffff
        PUBLIC _ZN3PidC2Efffff
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\math\pid.cpp
//    1 /**
//    2  * @file pid.cpp
//    3  *
//    4  * http://en.wikipedia.org/wiki/PID_controller
//    5  *
//    6  * @date Sep 16, 2014
//    7  * @author Andrey Belomutskiy, (c) 2012-2015
//    8  */
//    9 
//   10 #include "pid.h"
//   11 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function _ZN3PidC2Efffff
        THUMB
// __code __interwork __vfp Pid::subobject Pid(float, float, float, float, float)
_ZN3PidC2Efffff:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN3PidC1Efffff
        BL       _ZN3PidC1Efffff
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock0

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _ZN3PidC1Efffff
          CFI NoCalls
        THUMB
//   12 Pid::Pid(float pFactor, float iFactor, float dFactor, float minResult, float maxResult) {
//   13 	this->pFactor = pFactor;
_ZN3PidC1Efffff:
        VSTR     S0,[R0, #0]
//   14 	this->iFactor = iFactor;
        VSTR     S1,[R0, #+4]
//   15 	this->dFactor = dFactor;
        VSTR     S2,[R0, #+8]
//   16 	this->minResult = minResult;
        VSTR     S3,[R0, #+12]
//   17 	this->maxResult = maxResult;
        VSTR     S4,[R0, #+16]
//   18 
//   19 	iTerm = 0;
        MOVS     R1,#+0
        STR      R1,[R0, #+20]
//   20 	prevError = 0;
        MOVS     R1,#+0
        STR      R1,[R0, #+24]
//   21 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock1
//   22 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _ZN3Pid8getValueEfff
          CFI NoCalls
        THUMB
//   23 float Pid::getValue(float target, float input, float dTime) {
_ZN3Pid8getValueEfff:
        VMOV.F32 S3,S0
//   24 	float error = target - input;
        VSUB.F32 S4,S3,S1
//   25 
//   26 	float pTerm = pFactor * error;
        VLDR     S0,[R0, #0]
        VMUL.F32 S5,S0,S4
//   27 	iTerm += iFactor * dTime * error;
        VLDR     S0,[R0, #+4]
        VMUL.F32 S0,S0,S2
        VLDR     S6,[R0, #+20]
        VMLA.F32 S6,S0,S4
        VSTR     S6,[R0, #+20]
//   28 	float dTerm = dFactor / dTime * (error - prevError);
        VLDR     S0,[R0, #+8]
        VDIV.F32 S0,S0,S2
        VLDR     S6,[R0, #+24]
        VSUB.F32 S6,S4,S6
        VMUL.F32 S6,S0,S6
//   29 
//   30 	prevError = error;
        VSTR     S4,[R0, #+24]
//   31 
//   32 	float result = pTerm + iTerm + dTerm;
        VLDR     S0,[R0, #+20]
        VADD.F32 S0,S5,S0
        VADD.F32 S0,S0,S6
//   33 	if (result > maxResult) {
        VLDR     S7,[R0, #+16]
        VCMP.F32 S7,S0
        FMSTAT   
        BPL.N    ??getValue_0
//   34 //		iTerm -= result - maxResult;
//   35 		result = maxResult;
        VLDR     S7,[R0, #+16]
        VMOV.F32 S0,S7
        B.N      ??getValue_1
//   36 	} else if (result < minResult) {
??getValue_0:
        VLDR     S7,[R0, #+12]
        VCMP.F32 S0,S7
        FMSTAT   
        BPL.N    ??getValue_1
//   37 //		iTerm += minResult - result;
//   38 		result = minResult;
        VLDR     S7,[R0, #+12]
        VMOV.F32 S0,S7
//   39 	}
//   40 	return result;
??getValue_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock2
//   41 }
//   42 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _ZN3Pid13updateFactorsEfff
        THUMB
//   43 void Pid::updateFactors(float pFactor, float iFactor, float dFactor) {
_ZN3Pid13updateFactorsEfff:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8-D9}
          CFI D9 Frame(CFA, -16)
          CFI D8 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
        VMOV.F32 S16,S0
        VMOV.F32 S17,S1
        VMOV.F32 S18,S2
//   44 	this->pFactor = pFactor;
        VSTR     S16,[R4, #0]
//   45 	this->iFactor = iFactor;
        VSTR     S17,[R4, #+4]
//   46 	this->dFactor = dFactor;
        VSTR     S18,[R4, #+8]
//   47 	reset();
        MOVS     R0,R4
          CFI FunCall _ZN3Pid5resetEv
        BL       _ZN3Pid5resetEv
//   48 }
        VPOP     {D8-D9}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock3
//   49 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _ZN3Pid5resetEv
          CFI NoCalls
        THUMB
//   50 void Pid::reset(void) {
//   51 	iTerm = 0;
_ZN3Pid5resetEv:
        MOVS     R1,#+0
        STR      R1,[R0, #+20]
//   52 	prevError = 0;
        MOVS     R1,#+0
        STR      R1,[R0, #+24]
//   53 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//   54 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _ZN3Pid4getPEv
          CFI NoCalls
        THUMB
//   55 float Pid::getP(void) {
//   56 	return pFactor;
_ZN3Pid4getPEv:
        VLDR     S0,[R0, #0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock5
//   57 }
//   58 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _ZN3Pid4getIEv
          CFI NoCalls
        THUMB
//   59 float Pid::getI(void) {
//   60 	return iFactor;
_ZN3Pid4getIEv:
        VLDR     S0,[R0, #+4]
        BX       LR               ;; return
          CFI EndBlock cfiBlock6
//   61 }
//   62 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _ZN3Pid14getIntegrationEv
          CFI NoCalls
        THUMB
//   63 float Pid::getIntegration(void) {
//   64 	return iTerm;
_ZN3Pid14getIntegrationEv:
        VLDR     S0,[R0, #+20]
        BX       LR               ;; return
          CFI EndBlock cfiBlock7
//   65 }
//   66 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _ZN3Pid4getDEv
          CFI NoCalls
        THUMB
//   67 float Pid::getD(void) {
//   68 	return dFactor;
_ZN3Pid4getDEv:
        VLDR     S0,[R0, #+8]
        BX       LR               ;; return
          CFI EndBlock cfiBlock8
//   69 }

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        END
//   70 
//   71 
// 
// 240 bytes in section .text
// 
// 240 bytes of CODE memory
//
//Errors: none
//Warnings: none
