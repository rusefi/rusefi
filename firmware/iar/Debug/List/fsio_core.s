///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:27 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\cor /
//                       e\fsio_core.cpp                                      /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\cor /
//                       e\fsio_core.cpp -lCN F:\stuff\rusefi_sourceforge\fir /
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
//                       fsio_core.s                                          /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME fsio_core

        RTMODEL "__CPP_Language", "EC++"
        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1
        #define SHT_INIT_ARRAY 0xe

        EXTERN _Z10getLEValueP6EngineP7FLStackIfLi32EE11le_action_e
        EXTERN __aeabi_memset
        EXTERN atoff
        EXTERN firmwareError
        EXTERN getRemainingStack
        EXTERN indexOf
        EXTERN maxF
        EXTERN minF
        EXTERN rlist
        EXTERN strEqualCaseInsensitive
        EXTERN strlen
        EXTERN strncpy
        EXTERN warning

        PUBLIC LE_FIRST
        PUBLIC _Z11parseActionPKc
        PUBLIC _Z12getNextTokenPKcPci
        PUBLIC _Z9isNumericPKc
        PUBLIC _ZN12LECalculator3addEP9LEElement
        PUBLIC _ZN12LECalculator3popE11le_action_e
        PUBLIC _ZN12LECalculator4pushE11le_action_ef
        PUBLIC _ZN12LECalculator5doJobEP6EngineP9LEElement
        PUBLIC _ZN12LECalculator5resetEP9LEElement
        PUBLIC _ZN12LECalculator5resetEv
        PUBLIC _ZN12LECalculator8getValueEP6Engine
        PUBLIC _ZN12LECalculator9getValue2EP9LEElementP6Engine
        PUBLIC _ZN12LECalculatorC1Ev
        PUBLIC _ZN12LECalculatorC2Ev
        PUBLIC _ZN13LEElementPool15parseExpressionEPKc
        PUBLIC _ZN13LEElementPool4nextEv
        PUBLIC _ZN13LEElementPool5resetEv
        PUBLIC _ZN13LEElementPool7getSizeEv
        PUBLIC _ZN13LEElementPoolC1EP9LEElementi
        PUBLIC _ZN13LEElementPoolC2EP9LEElementi
        PUBLIC _ZN17LENameOrdinalPairC1E11le_action_ePKc
        PUBLIC _ZN17LENameOrdinalPairC2E11le_action_ePKc
        PUBLIC _ZN7FLStackIfLi32EE3popEv
        PUBLIC _ZN7FLStackIfLi32EE4pushEf
        PUBLIC _ZN7FLStackIfLi32EE4sizeEv
        PUBLIC _ZN7FLStackIfLi32EE5resetEv
        PUBLIC _ZN7FLStackIfLi32EEC1Ev
        PUBLIC _ZN7FLStackIfLi32EEC2Ev
        PUBLIC _ZN9LEElement4initE11le_action_e
        PUBLIC _ZN9LEElement4initE11le_action_ef
        PUBLIC _ZN9LEElementC1Ev
        PUBLIC _ZN9LEElementC2Ev
        PUBLIC _ZZN7FLStackIfLi32EE3popEvEs
        PUBLIC _ZZN7FLStackIfLi32EE4pushEfEs
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\core\fsio_core.cpp
//    1 /**
//    2  * @file fsio_core.cpp
//    3  * @brief core FSUI handling logic
//    4  *
//    5  * Here we parse and evaluate logical expressions in
//    6  * http://en.wikipedia.org/wiki/Reverse_Polish_notation
//    7  *
//    8  * Once the expressions are parsed on startup (that's a heavy operation),
//    9  * evaluating those is relatively efficient.
//   10  *
//   11  *
//   12  * @date Oct 3, 2014
//   13  * @author Andrey Belomutskiy, (c) 2012-2015
//   14  */
//   15 
//   16 #include "main.h"

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP memset
          CFI Block cfiBlock0 Using cfiCommon0
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
          CFI EndBlock cfiBlock0
//   17 
//   18 #if EFI_FSIO || defined(__DOXYGEN__)
//   19 
//   20 #include "fsio_core.h"

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN7FLStackIfLi32EEC1Ev
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function _ZN7FLStackIfLi32EEC1Ev
        THUMB
// __code __interwork __softfp FLStack<float, 32>::FLStack()
_ZN7FLStackIfLi32EEC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN7FLStackIfLi32EE5resetEv
        BL       _ZN7FLStackIfLi32EE5resetEv
        MOVS     R2,#+128
        MOVS     R1,#+0
        ADDS     R0,R4,#+4
          CFI FunCall memset
        BL       memset
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock1

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN7FLStackIfLi32EE5resetEv
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function _ZN7FLStackIfLi32EE5resetEv
          CFI NoCalls
        THUMB
// __interwork __softfp void FLStack<float, 32>::reset()
_ZN7FLStackIfLi32EE5resetEv:
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock2

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN7FLStackIfLi32EE4pushEf
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function _ZN7FLStackIfLi32EE4pushEf
        THUMB
// __interwork __vfp void FLStack<float, 32>::push(float)
_ZN7FLStackIfLi32EE4pushEf:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8}
          CFI D8 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        VMOV.F32 S16,S0
        LDR      R0,[R4, #+0]
        CMP      R0,#+32
        BLT.N    ??push_1
        LDR.N    R0,??push_2
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??push_3
??push_1:
        LDR      R0,[R4, #+0]
        ADDS     R1,R0,#+1
        STR      R1,[R4, #+0]
        ADDS     R0,R4,R0, LSL #+2
        VSTR     S16,[R0, #+4]
??push_3:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
        Nop      
        DATA
??push_2:
        DC32     _ZZN7FLStackIfLi32EE4pushEfEs
          CFI EndBlock cfiBlock3

        SECTION `.text`:CODE:REORDER:NOROOT(2)
        SECTION_GROUP _ZN7FLStackIfLi32EE3popEv
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function _ZN7FLStackIfLi32EE3popEv
        THUMB
// __interwork __vfp float FLStack<float, 32>::pop()
_ZN7FLStackIfLi32EE3popEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        LDR      R0,[R4, #+0]
        CMP      R0,#+0
        BNE.N    ??pop_2
        LDR.N    R0,??pop_3
          CFI FunCall firmwareError
        BL       firmwareError
??pop_2:
        LDR      R0,[R4, #+0]
        SUBS     R0,R0,#+1
        STR      R0,[R4, #+0]
        ADDS     R0,R4,R0, LSL #+2
        VLDR     S0,[R0, #+4]
        POP      {R4,PC}          ;; return
        DATA
??pop_3:
        DC32     _ZZN7FLStackIfLi32EE3popEvEs
          CFI EndBlock cfiBlock4

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN7FLStackIfLi32EE4sizeEv
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function _ZN7FLStackIfLi32EE4sizeEv
          CFI NoCalls
        THUMB
// __interwork __softfp int FLStack<float, 32>::size()
_ZN7FLStackIfLi32EE4sizeEv:
        LDR      R0,[R0, #+0]
        BX       LR               ;; return
          CFI EndBlock cfiBlock5
//   21 #include "fsio_impl.h"
//   22 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   23 LENameOrdinalPair * LE_FIRST = NULL;
LE_FIRST:
        DS8 4
//   24 
//   25 /**
//   26  * the main point of these static fields is that their constructor would register
//   27  * them in the magic list of operator name/ordinal pairs
//   28  */

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   29 static LENameOrdinalPair leAnd(LE_OPERATOR_AND, "and");
leAnd:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   30 static LENameOrdinalPair leAnd2(LE_OPERATOR_AND, "&");
leAnd2:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   31 static LENameOrdinalPair leOr(LE_OPERATOR_OR, "or");
leOr:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   32 static LENameOrdinalPair leOr2(LE_OPERATOR_OR, "|");
leOr2:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   33 static LENameOrdinalPair leNot(LE_OPERATOR_NOT, "not");
leNot:
        DS8 12
//   34 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   35 static LENameOrdinalPair leAdd(LE_OPERATOR_ADDITION, "+");
leAdd:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   36 static LENameOrdinalPair leSub(LE_OPERATOR_SUBTRACTION, "-");
leSub:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   37 static LENameOrdinalPair leMul(LE_OPERATOR_MULTIPLICATION, "*");
leMul:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   38 static LENameOrdinalPair leDiv(LE_OPERATOR_DIVISION, "/");
leDiv:
        DS8 12
//   39 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   40 static LENameOrdinalPair leMore(LE_OPERATOR_MORE, ">");
leMore:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   41 static LENameOrdinalPair leMoreOrEqual(LE_OPERATOR_MORE_OR_EQUAL, ">=");
leMoreOrEqual:
        DS8 12
//   42 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   43 static LENameOrdinalPair leLess(LE_OPERATOR_LESS, "<");
leLess:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   44 static LENameOrdinalPair leLessOrEquals(LE_OPERATOR_LESS_OR_EQUAL, "<=");
leLessOrEquals:
        DS8 12
//   45 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   46 static LENameOrdinalPair leMax(LE_METHOD_MAX, "max");
leMax:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   47 static LENameOrdinalPair leMin(LE_METHOD_MIN, "min");
leMin:
        DS8 12

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//   48 static LENameOrdinalPair leIf(LE_METHOD_IF, "if");
leIf:
        DS8 12

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function _ZN9LEElementC2Ev
        THUMB
// __code __interwork __softfp LEElement::subobject LEElement()
_ZN9LEElementC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN9LEElementC1Ev
        BL       _ZN9LEElementC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock6

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function _ZN13LEElementPoolC2EP9LEElementi
        THUMB
// __code __interwork __softfp LEElementPool::subobject LEElementPool(LEElement *, int)
_ZN13LEElementPoolC2EP9LEElementi:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN13LEElementPoolC1EP9LEElementi
        BL       _ZN13LEElementPoolC1EP9LEElementi
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock7

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function _ZN12LECalculatorC2Ev
        THUMB
// __code __interwork __softfp LECalculator::subobject LECalculator()
_ZN12LECalculatorC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculatorC1Ev
        BL       _ZN12LECalculatorC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock8
//   49 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function _ZN17LENameOrdinalPairC2E11le_action_ePKc
        THUMB
// __code __interwork __softfp LENameOrdinalPair::subobject LENameOrdinalPair(le_action_e, char const *)
_ZN17LENameOrdinalPairC2E11le_action_ePKc:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
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
        ADR.N    R2,??DataTable2  ;; "and"
        MOVS     R1,#+6
        LDR.W    R0,??DataTable8_1
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
        ADR.N    R2,??DataTable2_1  ;; "&"
        MOVS     R1,#+6
        LDR.W    R0,??DataTable8_2
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
        ADR.N    R2,??DataTable2_2  ;; 0x6F, 0x72, 0x00, 0x00
        MOVS     R1,#+7
        LDR.W    R0,??DataTable8_3
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
        ADR.N    R2,??DataTable2_3  ;; "|"
        MOVS     R1,#+7
        LDR.W    R0,??DataTable8_4
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
        ADR.N    R2,??DataTable2_4  ;; "not"
        MOVS     R1,#+8
        LDR.W    R0,??DataTable8_5
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
        ADR.N    R2,??DataTable2_5  ;; "+"
        MOVS     R1,#+9
        LDR.W    R0,??DataTable8_6
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
        ADR.N    R2,??DataTable2_6  ;; "-"
        MOVS     R1,#+10
        LDR.W    R0,??DataTable8_7
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
        ADR.N    R2,??DataTable2_7  ;; "*"
        MOVS     R1,#+11
        LDR.W    R0,??DataTable8_8
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
        ADR.N    R2,??DataTable2_8  ;; "/"
        MOVS     R1,#+12
        LDR.W    R0,??DataTable8_9
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
        ADR.N    R2,??DataTable2_9  ;; ">"
        MOVS     R1,#+3
        LDR.W    R0,??DataTable8_10
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
        ADR.N    R2,??DataTable2_10  ;; 0x3E, 0x3D, 0x00, 0x00
        MOVS     R1,#+5
        LDR.W    R0,??DataTable8_11
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
        ADR.N    R2,??DataTable2_11  ;; "<"
        MOVS     R1,#+2
        LDR.W    R0,??DataTable8_12
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
        ADR.N    R2,??DataTable2_12  ;; 0x3C, 0x3D, 0x00, 0x00
        MOVS     R1,#+4
        LDR.W    R0,??DataTable8_13
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
        ADR.N    R2,??DataTable2_13  ;; "max"
        MOVS     R1,#+13
        LDR.W    R0,??DataTable8_14
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
        ADR.N    R2,??DataTable2_14  ;; "min"
        MOVS     R1,#+14
        LDR.W    R0,??DataTable8_15
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
        ADR.N    R2,??DataTable2_15  ;; 0x69, 0x66, 0x00, 0x00
        MOVS     R1,#+15
        LDR.W    R0,??DataTable8_16
          CFI FunCall _ZN17LENameOrdinalPairC1E11le_action_ePKc
        BL       _ZN17LENameOrdinalPairC1E11le_action_ePKc
        POP      {R0,PC}          ;; return
          CFI EndBlock cfiBlock10

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function _ZN17LENameOrdinalPairC1E11le_action_ePKc
          CFI NoCalls
        THUMB
//   50 LENameOrdinalPair::LENameOrdinalPair(le_action_e action, const char *name) {
//   51 	this->action = action;
_ZN17LENameOrdinalPairC1E11le_action_ePKc:
        STR      R1,[R0, #+4]
//   52 	this->name = name;
        STR      R2,[R0, #+8]
//   53 	this->next = LE_FIRST;
        LDR.W    R3,??DataTable8_17
        LDR      R3,[R3, #+0]
        STR      R3,[R0, #+0]
//   54 	LE_FIRST = this;
        LDR.W    R3,??DataTable8_17
        STR      R0,[R3, #+0]
//   55 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock11
//   56 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function _ZN9LEElementC1Ev
          CFI NoCalls
        THUMB
//   57 LEElement::LEElement() {
//   58 	action = LE_UNDEFINED;
_ZN9LEElementC1Ev:
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
//   59 	next = NULL;
        MOVS     R1,#+0
        STR      R1,[R0, #+12]
//   60 	fValue = NAN;
        MVNS     R1,#-2147483648
        STR      R1,[R0, #+4]
//   61 	iValue = 0;
        MOVS     R1,#+0
        STR      R1,[R0, #+8]
//   62 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock12
//   63 
//   64 //void LEElement::init(le_action_e action, int iValue) {
//   65 //	this->action = action;
//   66 //	this->iValue = iValue;
//   67 //}
//   68 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function _ZN9LEElement4initE11le_action_e
          CFI NoCalls
        THUMB
//   69 void LEElement::init(le_action_e action) {
//   70 	this->action = action;
_ZN9LEElement4initE11le_action_e:
        STR      R1,[R0, #+0]
//   71 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock13
//   72 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock14 Using cfiCommon0
          CFI Function _ZN9LEElement4initE11le_action_ef
          CFI NoCalls
        THUMB
//   73 void LEElement::init(le_action_e action, float fValue) {
//   74 	this->action = action;
_ZN9LEElement4initE11le_action_ef:
        STR      R1,[R0, #+0]
//   75 	this->fValue = fValue;
        VSTR     S0,[R0, #+4]
//   76 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock14
//   77 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock15 Using cfiCommon0
          CFI Function _ZN12LECalculatorC1Ev
        THUMB
//   78 LECalculator::LECalculator() {
_ZN12LECalculatorC1Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        ADDS     R0,R4,#+520
          CFI FunCall _ZN7FLStackIfLi32EEC1Ev
        BL       _ZN7FLStackIfLi32EEC1Ev
//   79 	reset();
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator5resetEv
        BL       _ZN12LECalculator5resetEv
//   80 }
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock15
//   81 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock16 Using cfiCommon0
          CFI Function _ZN12LECalculator5resetEv
        THUMB
//   82 void LECalculator::reset() {
_ZN12LECalculator5resetEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//   83 	first = NULL;
        MOVS     R0,#+0
        STR      R0,[R4, #+516]
//   84 	stack.reset();
        ADDS     R0,R4,#+520
          CFI FunCall _ZN7FLStackIfLi32EE5resetEv
        BL       _ZN7FLStackIfLi32EE5resetEv
//   85 	currentCalculationLogPosition = 0;
        MOVS     R0,#+0
        STR      R0,[R4, #+512]
//   86 }
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock16
//   87 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock17 Using cfiCommon0
          CFI Function _ZN12LECalculator5resetEP9LEElement
        THUMB
//   88 void LECalculator::reset(LEElement *element) {
_ZN12LECalculator5resetEP9LEElement:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   89 	reset();
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator5resetEv
        BL       _ZN12LECalculator5resetEv
//   90 	add(element);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3addEP9LEElement
        BL       _ZN12LECalculator3addEP9LEElement
//   91 }
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock17
//   92 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock18 Using cfiCommon0
          CFI Function _ZN12LECalculator3addEP9LEElement
          CFI NoCalls
        THUMB
//   93 void LECalculator::add(LEElement *element) {
//   94 	if (first == NULL) {
_ZN12LECalculator3addEP9LEElement:
        LDR      R2,[R0, #+516]
        CMP      R2,#+0
        BNE.N    ??add_0
//   95 		first = element;
        STR      R1,[R0, #+516]
        B.N      ??add_1
//   96 	} else {
//   97 		LEElement *last = first;
??add_0:
        LDR      R2,[R0, #+516]
//   98 		while (last->next != NULL) {
??add_2:
        LDR      R3,[R2, #+12]
        CMP      R3,#+0
        BEQ.N    ??add_3
//   99 			last = last->next;
        LDR      R2,[R2, #+12]
        B.N      ??add_2
//  100 		}
//  101 		last->next = element;
??add_3:
        STR      R1,[R2, #+12]
//  102 	}
//  103 }
??add_1:
        BX       LR               ;; return
          CFI EndBlock cfiBlock18
//  104 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock19 Using cfiCommon0
          CFI Function _Z10float2boolf
          CFI NoCalls
        THUMB
//  105 static bool float2bool(float v) {
//  106 	return v != 0;
_Z10float2boolf:
        VCMP.F32 S0,#0.0
        FMSTAT   
        BEQ.N    ??float2bool_0
        MOVS     R0,#+1
        B.N      ??float2bool_1
??float2bool_0:
        MOVS     R0,#+0
??float2bool_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return
          CFI EndBlock cfiBlock19
//  107 }
//  108 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock20 Using cfiCommon0
          CFI Function _ZN12LECalculator3popE11le_action_e
        THUMB
//  109 float LECalculator::pop(le_action_e action) {
_ZN12LECalculator3popE11le_action_e:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  110 	if (stack.size() == 0) {
        ADDS     R0,R4,#+520
          CFI FunCall _ZN7FLStackIfLi32EE4sizeEv
        BL       _ZN7FLStackIfLi32EE4sizeEv
        CMP      R0,#+0
        BNE.N    ??pop_0
//  111 		warning(OBD_PCM_Processor_Fault, "empty stack for %d", action);
        MOVS     R2,R5
        LDR.W    R1,??DataTable8_18
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  112 		return NAN;
        VLDR.W   S0,??DataTable2_16  ;; 0x7fffffff
        B.N      ??pop_1
//  113 	}
//  114 	return stack.pop();
??pop_0:
        ADDS     R0,R4,#+520
          CFI FunCall _ZN7FLStackIfLi32EE3popEv
        BL       _ZN7FLStackIfLi32EE3popEv
??pop_1:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock20
//  115 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2:
        DC8      "and"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_1:
        DC8      "&",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_2:
        DC8      0x6F, 0x72, 0x00, 0x00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_3:
        DC8      "|",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_4:
        DC8      "not"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_5:
        DC8      "+",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_6:
        DC8      "-",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_7:
        DC8      "*",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_8:
        DC8      "/",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_9:
        DC8      ">",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_10:
        DC8      0x3E, 0x3D, 0x00, 0x00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_11:
        DC8      "<",0x0,0x0

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_12:
        DC8      0x3C, 0x3D, 0x00, 0x00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_13:
        DC8      "max"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_14:
        DC8      "min"

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_15:
        DC8      0x69, 0x66, 0x00, 0x00

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable2_16:
        DC32     0x7fffffff
//  116 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock21 Using cfiCommon0
          CFI Function _ZN12LECalculator4pushE11le_action_ef
        THUMB
//  117 void LECalculator::push(le_action_e action, float value) {
_ZN12LECalculator4pushE11le_action_ef:
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
        VMOV.F32 S16,S0
//  118 	stack.push(value);
        VMOV.F32 S0,S16
        ADDS     R0,R4,#+520
          CFI FunCall _ZN7FLStackIfLi32EE4pushEf
        BL       _ZN7FLStackIfLi32EE4pushEf
//  119 	if (currentCalculationLogPosition < MAX_CALC_LOG) {
        LDR      R0,[R4, #+512]
        CMP      R0,#+64
        BGE.N    ??push_0
//  120 		calcLogAction[currentCalculationLogPosition] = action;
        LDR      R0,[R4, #+512]
        STR      R5,[R4, R0, LSL #+2]
//  121 		calcLogValue[currentCalculationLogPosition] = value;
        LDR      R0,[R4, #+512]
        ADDS     R0,R4,R0, LSL #+2
        VSTR     S16,[R0, #+256]
//  122 		currentCalculationLogPosition++;
        LDR      R0,[R4, #+512]
        ADDS     R0,R0,#+1
        STR      R0,[R4, #+512]
//  123 	}
//  124 }
??push_0:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+16
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock21
//  125 
//  126 /**
//  127  * @return true in case of error, false otherwise
//  128  */

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock22 Using cfiCommon0
          CFI Function _ZN12LECalculator5doJobEP6EngineP9LEElement
        THUMB
//  129 bool_t LECalculator::doJob(Engine *engine, LEElement *element) {
_ZN12LECalculator5doJobEP6EngineP9LEElement:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        VPUSH    {D8-D9}
          CFI D9 Frame(CFA, -32)
          CFI D8 Frame(CFA, -40)
          CFI CFA R13+40
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  130 #if EFI_PROD_CODE || defined(__DOXYGEN__)
//  131 	efiAssert(getRemainingStack(chThdSelf()) > 64, "FSIO logic", false);
        LDR.W    R0,??DataTable8_19
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+65
        BGE.N    ??doJob_0
        LDR.W    R0,??DataTable8_20
          CFI FunCall firmwareError
        BL       firmwareError
        MOVS     R0,#+0
        B.N      ??doJob_1
//  132 #endif
//  133 	switch (element->action) {
??doJob_0:
        LDR      R0,[R6, #+0]
        CMP      R0,#+0
        BEQ.W    ??doJob_2
        CMP      R0,#+1
        BEQ.N    ??doJob_3
        CMP      R0,#+2
        BEQ.N    ??doJob_4
        CMP      R0,#+3
        BEQ.W    ??doJob_5
        CMP      R0,#+4
        BEQ.W    ??doJob_6
        CMP      R0,#+5
        BEQ.W    ??doJob_7
        CMP      R0,#+6
        BEQ.N    ??doJob_8
        CMP      R0,#+7
        BEQ.N    ??doJob_9
        CMP      R0,#+8
        BEQ.W    ??doJob_10
        CMP      R0,#+9
        BEQ.W    ??doJob_11
        CMP      R0,#+10
        BEQ.W    ??doJob_12
        CMP      R0,#+11
        BEQ.W    ??doJob_13
        CMP      R0,#+12
        BEQ.W    ??doJob_14
        CMP      R0,#+13
        BEQ.W    ??doJob_15
        CMP      R0,#+14
        BEQ.W    ??doJob_16
        CMP      R0,#+15
        BEQ.W    ??doJob_17
        CMP      R0,#+111
        BEQ.W    ??doJob_18
        CMP      R0,#+112
        BEQ.W    ??doJob_19
        B.N      ??doJob_20
//  134 
//  135 	case LE_NUMERIC_VALUE:
//  136 		push(element->action, element->fValue);
??doJob_3:
        VLDR     S0,[R6, #+4]
        LDR      R1,[R6, #+0]
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator4pushE11le_action_ef
        BL       _ZN12LECalculator4pushE11le_action_ef
//  137 		break;
        B.N      ??doJob_21
//  138 	case LE_OPERATOR_AND: {
//  139 		float v1 = pop(LE_OPERATOR_AND);
??doJob_8:
        MOVS     R1,#+6
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S16,S0
//  140 		float v2 = pop(LE_OPERATOR_AND);
        MOVS     R1,#+6
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S17,S0
//  141 
//  142 		push(element->action, float2bool(v1) && float2bool(v2));
        VMOV.F32 S0,S16
          CFI FunCall _Z10float2boolf
        BL       _Z10float2boolf
        CMP      R0,#+0
        BEQ.N    ??doJob_22
        VMOV.F32 S0,S17
          CFI FunCall _Z10float2boolf
        BL       _Z10float2boolf
        B.N      ??doJob_23
??doJob_22:
        MOVS     R0,#+0
??doJob_23:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        VMOV     S0,R0
        VCVT.F32.U32 S0,S0
        LDR      R1,[R6, #+0]
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator4pushE11le_action_ef
        BL       _ZN12LECalculator4pushE11le_action_ef
//  143 	}
//  144 		break;
        B.N      ??doJob_21
//  145 	case LE_OPERATOR_OR: {
//  146 		float v1 = pop(LE_OPERATOR_OR);
??doJob_9:
        MOVS     R1,#+7
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S16,S0
//  147 		float v2 = pop(LE_OPERATOR_OR);
        MOVS     R1,#+7
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S17,S0
//  148 
//  149 		push(element->action, float2bool(v1) || float2bool(v2));
        VMOV.F32 S0,S16
          CFI FunCall _Z10float2boolf
        BL       _Z10float2boolf
        CMP      R0,#+0
        BNE.N    ??doJob_24
        VMOV.F32 S0,S17
          CFI FunCall _Z10float2boolf
        BL       _Z10float2boolf
        B.N      ??doJob_25
??doJob_24:
        MOVS     R0,#+1
??doJob_25:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        VMOV     S0,R0
        VCVT.F32.U32 S0,S0
        LDR      R1,[R6, #+0]
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator4pushE11le_action_ef
        BL       _ZN12LECalculator4pushE11le_action_ef
//  150 	}
//  151 		break;
        B.N      ??doJob_21
//  152 	case LE_OPERATOR_LESS: {
//  153 		// elements on stack are in reverse order
//  154 		float v2 = pop(LE_OPERATOR_LESS);
??doJob_4:
        MOVS     R1,#+2
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S16,S0
//  155 		float v1 = pop(LE_OPERATOR_LESS);
        MOVS     R1,#+2
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S17,S0
//  156 
//  157 		push(element->action, v1 < v2);
        VCMP.F32 S17,S16
        FMSTAT   
        BPL.N    ??doJob_26
        MOVS     R0,#+1
        B.N      ??doJob_27
??doJob_26:
        MOVS     R0,#+0
??doJob_27:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        VMOV     S0,R0
        VCVT.F32.U32 S0,S0
        LDR      R1,[R6, #+0]
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator4pushE11le_action_ef
        BL       _ZN12LECalculator4pushE11le_action_ef
//  158 	}
//  159 		break;
        B.N      ??doJob_21
//  160 	case LE_OPERATOR_NOT: {
//  161 		float v = pop(LE_OPERATOR_NOT);
??doJob_10:
        MOVS     R1,#+8
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S16,S0
//  162 		push(element->action, !float2bool(v));
        VMOV.F32 S0,S16
          CFI FunCall _Z10float2boolf
        BL       _Z10float2boolf
        EORS     R0,R0,#0x1
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        VMOV     S0,R0
        VCVT.F32.U32 S0,S0
        LDR      R1,[R6, #+0]
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator4pushE11le_action_ef
        BL       _ZN12LECalculator4pushE11le_action_ef
//  163 	}
//  164 		break;
        B.N      ??doJob_21
//  165 	case LE_OPERATOR_MORE: {
//  166 		// elements on stack are in reverse order
//  167 		float v2 = pop(LE_OPERATOR_MORE);
??doJob_5:
        MOVS     R1,#+3
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S16,S0
//  168 		float v1 = pop(LE_OPERATOR_MORE);
        MOVS     R1,#+3
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S17,S0
//  169 
//  170 		push(element->action, v1 > v2);
        VCMP.F32 S16,S17
        FMSTAT   
        BPL.N    ??doJob_28
        MOVS     R0,#+1
        B.N      ??doJob_29
??doJob_28:
        MOVS     R0,#+0
??doJob_29:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        VMOV     S0,R0
        VCVT.F32.U32 S0,S0
        LDR      R1,[R6, #+0]
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator4pushE11le_action_ef
        BL       _ZN12LECalculator4pushE11le_action_ef
//  171 	}
//  172 		break;
        B.N      ??doJob_21
//  173 	case LE_OPERATOR_ADDITION: {
//  174 		// elements on stack are in reverse order
//  175 		float v2 = pop(LE_OPERATOR_MORE);
??doJob_11:
        MOVS     R1,#+3
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S16,S0
//  176 		float v1 = pop(LE_OPERATOR_MORE);
        MOVS     R1,#+3
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S17,S0
//  177 
//  178 		push(element->action, v1 + v2);
        VADD.F32 S0,S17,S16
        LDR      R1,[R6, #+0]
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator4pushE11le_action_ef
        BL       _ZN12LECalculator4pushE11le_action_ef
//  179 	}
//  180 		break;
        B.N      ??doJob_21
//  181 	case LE_OPERATOR_SUBTRACTION: {
//  182 		// elements on stack are in reverse order
//  183 		float v2 = pop(LE_OPERATOR_MORE);
??doJob_12:
        MOVS     R1,#+3
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S16,S0
//  184 		float v1 = pop(LE_OPERATOR_MORE);
        MOVS     R1,#+3
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S17,S0
//  185 
//  186 		push(element->action, v1 - v2);
        VSUB.F32 S0,S17,S16
        LDR      R1,[R6, #+0]
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator4pushE11le_action_ef
        BL       _ZN12LECalculator4pushE11le_action_ef
//  187 	}
//  188 		break;
        B.N      ??doJob_21
//  189 	case LE_OPERATOR_MULTIPLICATION: {
//  190 		// elements on stack are in reverse order
//  191 		float v2 = pop(LE_OPERATOR_MORE);
??doJob_13:
        MOVS     R1,#+3
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S16,S0
//  192 		float v1 = pop(LE_OPERATOR_MORE);
        MOVS     R1,#+3
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S17,S0
//  193 
//  194 		push(element->action, v1 * v2);
        VMUL.F32 S0,S17,S16
        LDR      R1,[R6, #+0]
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator4pushE11le_action_ef
        BL       _ZN12LECalculator4pushE11le_action_ef
//  195 	}
//  196 		break;
        B.N      ??doJob_21
//  197 	case LE_OPERATOR_DIVISION: {
//  198 		// elements on stack are in reverse order
//  199 		float v2 = pop(LE_OPERATOR_MORE);
??doJob_14:
        MOVS     R1,#+3
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S16,S0
//  200 		float v1 = pop(LE_OPERATOR_MORE);
        MOVS     R1,#+3
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S17,S0
//  201 
//  202 		push(element->action, v1 / v2);
        VDIV.F32 S0,S17,S16
        LDR      R1,[R6, #+0]
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator4pushE11le_action_ef
        BL       _ZN12LECalculator4pushE11le_action_ef
//  203 	}
//  204 		break;
        B.N      ??doJob_21
//  205 	case LE_OPERATOR_LESS_OR_EQUAL: {
//  206 		// elements on stack are in reverse order
//  207 		float v2 = pop(LE_OPERATOR_LESS_OR_EQUAL);
??doJob_6:
        MOVS     R1,#+4
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S16,S0
//  208 		float v1 = pop(LE_OPERATOR_LESS_OR_EQUAL);
        MOVS     R1,#+4
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S17,S0
//  209 
//  210 		push(element->action, v1 <= v2);
        VCMP.F32 S16,S17
        FMSTAT   
        BLT.N    ??doJob_30
        MOVS     R0,#+1
        B.N      ??doJob_31
??doJob_30:
        MOVS     R0,#+0
??doJob_31:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        VMOV     S0,R0
        VCVT.F32.U32 S0,S0
        LDR      R1,[R6, #+0]
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator4pushE11le_action_ef
        BL       _ZN12LECalculator4pushE11le_action_ef
//  211 	}
//  212 		break;
        B.N      ??doJob_21
//  213 	case LE_OPERATOR_MORE_OR_EQUAL: {
//  214 		// elements on stack are in reverse order
//  215 		float v2 = pop(LE_OPERATOR_MORE_OR_EQUAL);
??doJob_7:
        MOVS     R1,#+5
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S16,S0
//  216 		float v1 = pop(LE_OPERATOR_MORE_OR_EQUAL);
        MOVS     R1,#+5
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S17,S0
//  217 
//  218 		push(element->action, v1 >= v2);
        VCMP.F32 S17,S16
        FMSTAT   
        BLT.N    ??doJob_32
        MOVS     R0,#+1
        B.N      ??doJob_33
??doJob_32:
        MOVS     R0,#+0
??doJob_33:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        VMOV     S0,R0
        VCVT.F32.U32 S0,S0
        LDR      R1,[R6, #+0]
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator4pushE11le_action_ef
        BL       _ZN12LECalculator4pushE11le_action_ef
//  219 	}
//  220 		break;
        B.N      ??doJob_21
//  221 	case LE_METHOD_IF: {
//  222 		// elements on stack are in reverse order
//  223 		float vFalse = pop(LE_METHOD_IF);
??doJob_17:
        MOVS     R1,#+15
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S16,S0
//  224 		float vTrue = pop(LE_METHOD_IF);
        MOVS     R1,#+15
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S17,S0
//  225 		float vCond = pop(LE_METHOD_IF);
        MOVS     R1,#+15
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S18,S0
//  226 		push(element->action, vCond != 0 ? vTrue : vFalse);
        VCMP.F32 S18,#0.0
        FMSTAT   
        BEQ.N    ??doJob_34
        VMOV.F32 S0,S17
        B.N      ??doJob_35
??doJob_34:
        VMOV.F32 S0,S16
??doJob_35:
        LDR      R1,[R6, #+0]
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator4pushE11le_action_ef
        BL       _ZN12LECalculator4pushE11le_action_ef
//  227 	}
//  228 		break;
        B.N      ??doJob_21
//  229 	case LE_METHOD_MAX: {
//  230 		float v2 = pop(LE_METHOD_MAX);
??doJob_15:
        MOVS     R1,#+13
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S16,S0
//  231 		float v1 = pop(LE_METHOD_MAX);
        MOVS     R1,#+13
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S17,S0
//  232 		push(element->action, maxF(v1, v2));
        VMOV.F32 S1,S16
        VMOV.F32 S0,S17
          CFI FunCall maxF
        BL       maxF
        LDR      R1,[R6, #+0]
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator4pushE11le_action_ef
        BL       _ZN12LECalculator4pushE11le_action_ef
//  233 	}
//  234 		break;
        B.N      ??doJob_21
//  235 	case LE_METHOD_MIN: {
//  236 		float v2 = pop(LE_METHOD_MIN);
??doJob_16:
        MOVS     R1,#+14
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S16,S0
//  237 		float v1 = pop(LE_METHOD_MIN);
        MOVS     R1,#+14
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S17,S0
//  238 		push(element->action, minF(v1, v2));
        VMOV.F32 S1,S16
        VMOV.F32 S0,S17
          CFI FunCall minF
        BL       minF
        LDR      R1,[R6, #+0]
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator4pushE11le_action_ef
        BL       _ZN12LECalculator4pushE11le_action_ef
//  239 	}
//  240 		break;
        B.N      ??doJob_21
//  241 	case LE_METHOD_FSIO_SETTING: {
//  242 		float i = pop(LE_METHOD_FSIO_SETTING);
??doJob_18:
        MOVS     R1,#+111
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator3popE11le_action_e
        BL       _ZN12LECalculator3popE11le_action_e
        VMOV.F32 S16,S0
//  243 		int index = (int) i;
        VCVT.S32.F32 S0,S16
        VMOV     R7,S0
//  244 		if (index >= 0 && index < LE_COMMAND_COUNT) {
        CMP      R7,#+0
        BMI.N    ??doJob_36
        CMP      R7,#+16
        BGE.N    ??doJob_36
//  245 			push(element->action, engine->engineConfiguration->bc.fsio_setting[index]);
        LDR      R0,[R5, #+36]
        ADDS     R0,R0,R7, LSL #+2
        LDR      R0,[R0, #+1284]
        VMOV     S0,R0
        LDR      R1,[R6, #+0]
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator4pushE11le_action_ef
        BL       _ZN12LECalculator4pushE11le_action_ef
        B.N      ??doJob_37
//  246 		} else {
//  247 			push(element->action, NAN);
??doJob_36:
        VLDR.W   S0,??DataTable8  ;; 0x7fffffff
        LDR      R1,[R6, #+0]
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator4pushE11le_action_ef
        BL       _ZN12LECalculator4pushE11le_action_ef
//  248 		}
//  249 	}
//  250 		break;
??doJob_37:
        B.N      ??doJob_21
//  251 	case LE_METHOD_KNOCK:
//  252 		push(element->action, engine->knockCount);
??doJob_19:
        VLDR     S0,[R5, #+620]
        VCVT.F32.S32 S0,S0
        LDR      R1,[R6, #+0]
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator4pushE11le_action_ef
        BL       _ZN12LECalculator4pushE11le_action_ef
//  253 		break;
        B.N      ??doJob_21
//  254 	case LE_UNDEFINED:
//  255 		warning(OBD_PCM_Processor_Fault, "FSIO undefined action");
??doJob_2:
        LDR.N    R1,??DataTable8_21
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  256 		return true;
        MOVS     R0,#+1
        B.N      ??doJob_1
//  257 	default:
//  258 		push(element->action, getLEValue(engine, &stack, element->action));
??doJob_20:
        LDR      R2,[R6, #+0]
        ADDS     R1,R4,#+520
        MOVS     R0,R5
          CFI FunCall _Z10getLEValueP6EngineP7FLStackIfLi32EE11le_action_e
        BL       _Z10getLEValueP6EngineP7FLStackIfLi32EE11le_action_e
        LDR      R1,[R6, #+0]
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator4pushE11le_action_ef
        BL       _ZN12LECalculator4pushE11le_action_ef
//  259 	}
//  260 	return false;
??doJob_21:
        MOVS     R0,#+0
??doJob_1:
        VPOP     {D8-D9}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI CFA R13+24
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock22
//  261 }
//  262 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock23 Using cfiCommon0
          CFI Function _ZN12LECalculator9getValue2EP9LEElementP6Engine
        THUMB
//  263 float LECalculator::getValue2(LEElement *element, Engine *engine) {
_ZN12LECalculator9getValue2EP9LEElementP6Engine:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  264 	reset(element);
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator5resetEP9LEElement
        BL       _ZN12LECalculator5resetEP9LEElement
//  265 	return getValue(engine);
        MOVS     R1,R6
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator8getValueEP6Engine
        BL       _ZN12LECalculator8getValueEP6Engine
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock23
//  266 }
//  267 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock24 Using cfiCommon0
          CFI Function _ZN12LECalculator8getValueEP6Engine
        THUMB
//  268 float LECalculator::getValue(Engine *engine) {
_ZN12LECalculator8getValueEP6Engine:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
//  269 	if (first == NULL) {
        LDR      R0,[R4, #+516]
        CMP      R0,#+0
        BNE.N    ??getValue_0
//  270 		warning(OBD_PCM_Processor_Fault, "no FSIO code");
        LDR.N    R1,??DataTable8_22
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  271 		return NAN;
        VLDR.W   S0,??DataTable8  ;; 0x7fffffff
        B.N      ??getValue_1
//  272 	}
//  273 	LEElement *element = first;
??getValue_0:
        LDR      R6,[R4, #+516]
//  274 
//  275 	stack.reset();
        ADDS     R0,R4,#+520
          CFI FunCall _ZN7FLStackIfLi32EE5resetEv
        BL       _ZN7FLStackIfLi32EE5resetEv
//  276 
//  277 	int counter = 0;
        MOVS     R7,#+0
//  278 	while (element != NULL) {
??getValue_2:
        CMP      R6,#+0
        BEQ.N    ??getValue_3
//  279 		efiAssert(counter < 200, "FSIOcount", NAN); // just in case
        CMP      R7,#+200
        BLT.N    ??getValue_4
        LDR.N    R0,??DataTable8_23
          CFI FunCall firmwareError
        BL       firmwareError
        VLDR.W   S0,??DataTable8  ;; 0x7fffffff
        B.N      ??getValue_1
//  280 
//  281 		bool_t isError = doJob(engine, element);
??getValue_4:
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall _ZN12LECalculator5doJobEP6EngineP9LEElement
        BL       _ZN12LECalculator5doJobEP6EngineP9LEElement
//  282 		if (isError) {
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+0
        BEQ.N    ??getValue_5
//  283 			// error already reported
//  284 			return NAN;
        VLDR.W   S0,??DataTable8  ;; 0x7fffffff
        B.N      ??getValue_1
//  285 		}
//  286 		element = element->next;
??getValue_5:
        LDR      R6,[R6, #+12]
//  287 		counter++;
        ADDS     R7,R7,#+1
        B.N      ??getValue_2
//  288 	}
//  289 	if (stack.size() != 1) {
??getValue_3:
        ADDS     R0,R4,#+520
          CFI FunCall _ZN7FLStackIfLi32EE4sizeEv
        BL       _ZN7FLStackIfLi32EE4sizeEv
        CMP      R0,#+1
        BEQ.N    ??getValue_6
//  290 		warning(OBD_PCM_Processor_Fault, "unexpected FSIO stack size: %d", stack.size());
        ADDS     R0,R4,#+520
          CFI FunCall _ZN7FLStackIfLi32EE4sizeEv
        BL       _ZN7FLStackIfLi32EE4sizeEv
        MOVS     R2,R0
        LDR.N    R1,??DataTable8_24
        MOVW     R0,#+606
          CFI FunCall warning
        BL       warning
//  291 		return NAN;
        VLDR.W   S0,??DataTable8  ;; 0x7fffffff
        B.N      ??getValue_1
//  292 	}
//  293 	return stack.pop();
??getValue_6:
        ADDS     R0,R4,#+520
          CFI FunCall _ZN7FLStackIfLi32EE3popEv
        BL       _ZN7FLStackIfLi32EE3popEv
??getValue_1:
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock24
//  294 }
//  295 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock25 Using cfiCommon0
          CFI Function _ZN13LEElementPoolC1EP9LEElementi
        THUMB
//  296 LEElementPool::LEElementPool(LEElement *pool, int size) {
_ZN13LEElementPoolC1EP9LEElementi:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//  297 	this->pool = pool;
        STR      R5,[R4, #+0]
//  298 	this->size = size;
        STR      R6,[R4, #+8]
//  299 	reset();
        MOVS     R0,R4
          CFI FunCall _ZN13LEElementPool5resetEv
        BL       _ZN13LEElementPool5resetEv
//  300 }
        MOVS     R0,R4
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock25
//  301 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock26 Using cfiCommon0
          CFI Function _ZN13LEElementPool5resetEv
          CFI NoCalls
        THUMB
//  302 void LEElementPool::reset() {
//  303 	index = 0;
_ZN13LEElementPool5resetEv:
        MOVS     R1,#+0
        STR      R1,[R0, #+4]
//  304 }
        BX       LR               ;; return
          CFI EndBlock cfiBlock26
//  305 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock27 Using cfiCommon0
          CFI Function _ZN13LEElementPool7getSizeEv
          CFI NoCalls
        THUMB
//  306 int LEElementPool::getSize() {
//  307 	return index;
_ZN13LEElementPool7getSizeEv:
        LDR      R0,[R0, #+4]
        BX       LR               ;; return
          CFI EndBlock cfiBlock27
//  308 }
//  309 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock28 Using cfiCommon0
          CFI Function _ZN13LEElementPool4nextEv
        THUMB
//  310 LEElement *LEElementPool::next() {
_ZN13LEElementPool4nextEv:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
//  311 	if (index >= size) {
        LDR      R0,[R4, #+4]
        LDR      R1,[R4, #+8]
        CMP      R0,R1
        BLT.N    ??next_0
//  312 		// todo: this should not be a fatal error, just an error
//  313 		firmwareError("LE_ELEMENT_POOL_SIZE overflow");
        LDR.N    R0,??DataTable8_25
          CFI FunCall firmwareError
        BL       firmwareError
//  314 		return NULL;
        MOVS     R0,#+0
        B.N      ??next_1
//  315 	}
//  316 	return &pool[index++];
??next_0:
        LDR      R0,[R4, #+4]
        ADDS     R1,R0,#+1
        STR      R1,[R4, #+4]
        LDR      R1,[R4, #+0]
        ADDS     R0,R1,R0, LSL #+4
??next_1:
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock28
//  317 }
//  318 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock29 Using cfiCommon0
          CFI Function _Z9isNumericPKc
          CFI NoCalls
        THUMB
//  319 bool isNumeric(const char* line) {
//  320 	return line[0] >= '0' && line[0] <= '9';
_Z9isNumericPKc:
        LDRB     R1,[R0, #+0]
        CMP      R1,#+48
        BLT.N    ??isNumeric_0
        LDRB     R0,[R0, #+0]
        CMP      R0,#+58
        BGE.N    ??isNumeric_0
        MOVS     R0,#+1
        B.N      ??isNumeric_1
??isNumeric_0:
        MOVS     R0,#+0
??isNumeric_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return
          CFI EndBlock cfiBlock29
//  321 }
//  322 
//  323 /**
//  324  * @return pointer at the position after the consumed token, null if no token consumed
//  325  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock30 Using cfiCommon0
          CFI Function _Z12getNextTokenPKcPci
        THUMB
//  326 const char *getNextToken(const char *line, char *buffer, const int bufferSize) {
_Z12getNextTokenPKcPci:
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
//  327 	while (line[0] != 0 && line[0] == ' ') {
??getNextToken_0:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+0
        BEQ.N    ??getNextToken_1
        LDRB     R0,[R4, #+0]
        CMP      R0,#+32
        BNE.N    ??getNextToken_1
//  328 		line++;
        ADDS     R4,R4,#+1
        B.N      ??getNextToken_0
//  329 	}
//  330 	if (line[0] == 0) {
??getNextToken_1:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+0
        BNE.N    ??getNextToken_2
//  331 		return NULL;
        MOVS     R0,#+0
        B.N      ??getNextToken_3
//  332 	}
//  333 	int tokenLen = indexOf(line, ' ');
??getNextToken_2:
        MOVS     R1,#+32
        MOVS     R0,R4
          CFI FunCall indexOf
        BL       indexOf
        MOVS     R7,R0
//  334 	if (tokenLen == -1) {
        CMN      R7,#+1
        BNE.N    ??getNextToken_4
//  335 		// no space - the whole remaining line is the token
//  336 		tokenLen = strlen(line);
        MOVS     R0,R4
          CFI FunCall strlen
        BL       strlen
        MOVS     R7,R0
//  337 	}
//  338 	efiAssert(tokenLen < bufferSize, "token overflow", NULL);
??getNextToken_4:
        CMP      R7,R6
        BLT.N    ??getNextToken_5
        LDR.N    R0,??DataTable8_26
          CFI FunCall firmwareError
        BL       firmwareError
        MOVS     R0,#+0
        B.N      ??getNextToken_3
//  339 	strncpy(buffer, line, tokenLen);
??getNextToken_5:
        MOVS     R2,R7
        MOVS     R1,R4
        MOVS     R0,R5
          CFI FunCall strncpy
        BL       strncpy
//  340 	buffer[tokenLen] = 0;
        MOVS     R0,#+0
        STRB     R0,[R7, R5]
//  341 	line += tokenLen;
        ADDS     R4,R7,R4
//  342 	return line;
        MOVS     R0,R4
??getNextToken_3:
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock30
//  343 }
//  344 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock31 Using cfiCommon0
          CFI Function _Z11parseActionPKc
        THUMB
//  345 le_action_e parseAction(const char * line) {
_Z11parseActionPKc:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  346 	LENameOrdinalPair *pair = LE_FIRST;
        LDR.N    R0,??DataTable8_17
        LDR      R5,[R0, #+0]
//  347 	while (pair != NULL) {
??parseAction_0:
        CMP      R5,#+0
        BEQ.N    ??parseAction_1
//  348 		if (strEqualCaseInsensitive(pair->name, line)) {
        MOVS     R1,R4
        LDR      R0,[R5, #+8]
          CFI FunCall strEqualCaseInsensitive
        BL       strEqualCaseInsensitive
        CMP      R0,#+0
        BEQ.N    ??parseAction_2
//  349 			return pair->action;
        LDR      R0,[R5, #+4]
        B.N      ??parseAction_3
//  350 		}
//  351 		pair = pair->next;
??parseAction_2:
        LDR      R5,[R5, #+0]
        B.N      ??parseAction_0
//  352 	}
//  353 	return LE_UNDEFINED;
??parseAction_1:
        MOVS     R0,#+0
??parseAction_3:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock31
//  354 }
//  355 

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  356 static char parsingBuffer[64];
parsingBuffer:
        DS8 64
//  357 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock32 Using cfiCommon0
          CFI Function _ZN13LEElementPool15parseExpressionEPKc
        THUMB
//  358 LEElement *LEElementPool::parseExpression(const char * line) {
_ZN13LEElementPool15parseExpressionEPKc:
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
//  359 
//  360 	LEElement *first = NULL;
        MOVS     R6,#+0
//  361 	LEElement *last = NULL;
        MOVS     R7,#+0
//  362 
//  363 	while (true) {
//  364 		line = getNextToken(line, parsingBuffer, sizeof(parsingBuffer));
??parseExpression_0:
        MOVS     R2,#+64
        LDR.N    R1,??DataTable8_27
        MOVS     R0,R5
          CFI FunCall _Z12getNextTokenPKcPci
        BL       _Z12getNextTokenPKcPci
        MOVS     R5,R0
//  365 
//  366 		if (line == NULL) {
        CMP      R5,#+0
        BNE.N    ??parseExpression_1
//  367 			/**
//  368 			 * No more tokens in this line
//  369 			 */
//  370 			return first;
        MOVS     R0,R6
        B.N      ??parseExpression_2
//  371 		}
//  372 
//  373 		LEElement *n = next();
??parseExpression_1:
        MOVS     R0,R4
          CFI FunCall _ZN13LEElementPool4nextEv
        BL       _ZN13LEElementPool4nextEv
        MOV      R8,R0
//  374 		if (n == NULL) {
        CMP      R8,#+0
        BNE.N    ??parseExpression_3
//  375 			return first;
        MOVS     R0,R6
        B.N      ??parseExpression_2
//  376 		}
//  377 
//  378 		if (isNumeric(parsingBuffer)) {
??parseExpression_3:
        LDR.N    R0,??DataTable8_27
          CFI FunCall _Z9isNumericPKc
        BL       _Z9isNumericPKc
        CMP      R0,#+0
        BEQ.N    ??parseExpression_4
//  379 			n->init(LE_NUMERIC_VALUE, atoff(parsingBuffer));
        LDR.N    R0,??DataTable8_27
          CFI FunCall atoff
        BL       atoff
        MOVS     R1,#+1
        MOV      R0,R8
          CFI FunCall _ZN9LEElement4initE11le_action_ef
        BL       _ZN9LEElement4initE11le_action_ef
        B.N      ??parseExpression_5
//  380 		} else {
//  381 			le_action_e action = parseAction(parsingBuffer);
??parseExpression_4:
        LDR.N    R0,??DataTable8_27
          CFI FunCall _Z11parseActionPKc
        BL       _Z11parseActionPKc
        MOV      R9,R0
//  382 			if (action == LE_UNDEFINED) {
        CMP      R9,#+0
        BNE.N    ??parseExpression_6
//  383 				/**
//  384 				 * Cannot recognize token
//  385 				 */
//  386 				warning((obd_code_e) 0, "unrecognized [%s]", parsingBuffer);
        LDR.N    R2,??DataTable8_27
        LDR.N    R1,??DataTable8_28
        MOVS     R0,#+0
          CFI FunCall warning
        BL       warning
//  387 				return NULL;
        MOVS     R0,#+0
        B.N      ??parseExpression_2
//  388 			}
//  389 			n->init(action);
??parseExpression_6:
        MOV      R1,R9
        MOV      R0,R8
          CFI FunCall _ZN9LEElement4initE11le_action_e
        BL       _ZN9LEElement4initE11le_action_e
//  390 		}
//  391 
//  392 		if (first == NULL) {
??parseExpression_5:
        CMP      R6,#+0
        BNE.N    ??parseExpression_7
//  393 			first = n;
        MOV      R6,R8
//  394 			last = n;
        MOV      R7,R8
        B.N      ??parseExpression_0
//  395 		} else {
//  396 			last->next = n;
??parseExpression_7:
        STR      R8,[R7, #+12]
//  397 			last = last->next;
        LDR      R7,[R7, #+12]
        B.N      ??parseExpression_0
//  398 		}
//  399 	}
??parseExpression_2:
        POP      {R1,R4-R9,PC}    ;; return
          CFI EndBlock cfiBlock32
//  400 	return first;
//  401 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8:
        DC32     0x7fffffff

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_1:
        DC32     leAnd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_2:
        DC32     leAnd2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_3:
        DC32     leOr

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_4:
        DC32     leOr2

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_5:
        DC32     leNot

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_6:
        DC32     leAdd

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_7:
        DC32     leSub

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_8:
        DC32     leMul

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_9:
        DC32     leDiv

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_10:
        DC32     leMore

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_11:
        DC32     leMoreOrEqual

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_12:
        DC32     leLess

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_13:
        DC32     leLessOrEquals

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_14:
        DC32     leMax

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_15:
        DC32     leMin

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_16:
        DC32     leIf

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_17:
        DC32     LE_FIRST

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_18:
        DC32     `?<Constant "empty stack for %d">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_19:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_20:
        DC32     `?<Constant "FSIO logic">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_21:
        DC32     `?<Constant "FSIO undefined action">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_22:
        DC32     `?<Constant "no FSIO code">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_23:
        DC32     `?<Constant "FSIOcount">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_24:
        DC32     `?<Constant "unexpected FSIO stack...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_25:
        DC32     `?<Constant "LE_ELEMENT_POOL_SIZE ...">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_26:
        DC32     `?<Constant "token overflow">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_27:
        DC32     parsingBuffer

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable8_28:
        DC32     `?<Constant "unrecognized [%s]">`

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP _ZN7FLStackIfLi32EEC2Ev
          CFI Block cfiBlock33 Using cfiCommon0
          CFI Function _ZN7FLStackIfLi32EEC2Ev
        THUMB
// __code __interwork __softfp FLStack<float, 32>::subobject FLStack()
_ZN7FLStackIfLi32EEC2Ev:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        MOVS     R4,R0
        MOVS     R0,R4
          CFI FunCall _ZN7FLStackIfLi32EEC1Ev
        BL       _ZN7FLStackIfLi32EEC1Ev
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock33

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
        SECTION_GROUP _ZZN7FLStackIfLi32EE4pushEfEs
// __absolute char const <_ZZN7FLStackIfLi32EE4pushEfEs>[17]
_ZZN7FLStackIfLi32EE4pushEfEs:
        DATA
        DC8 "FLstack overflow"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        SECTION_GROUP _ZZN7FLStackIfLi32EE3popEvEs
// __absolute char const <_ZZN7FLStackIfLi32EE3popEvEs>[17]
_ZZN7FLStackIfLi32EE3popEvEs:
        DATA
        DC8 "FLStack is empty"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "token overflow">`:
        DATA
        DC8 "token overflow"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "LE_ELEMENT_POOL_SIZE ...">`:
        DATA
        DC8 "LE_ELEMENT_POOL_SIZE overflow"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "unrecognized [%s]">`:
        DATA
        DC8 "unrecognized [%s]"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "no FSIO code">`:
        DATA
        DC8 "no FSIO code"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FSIOcount">`:
        DATA
        DC8 "FSIOcount"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "unexpected FSIO stack...">`:
        DATA
        DC8 "unexpected FSIO stack size: %d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FSIO logic">`:
        DATA
        DC8 "FSIO logic"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "FSIO undefined action">`:
        DATA
        DC8 "FSIO undefined action"
        DC8 0, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "empty stack for %d">`:
        DATA
        DC8 "empty stack for %d"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "and"

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 "&"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "or"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 "|"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "not"

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 "+"

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 "-"

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 "*"

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 "/"

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 ">"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 ">="
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(1)
        DATA
        DC8 "<"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "<="
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "max"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "min"

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
        DATA
        DC8 "if"
        DC8 0

        END
//  402 
//  403 #endif /* EFI_FSIO */
// 
//   260 bytes in section .bss
//     4 bytes in section .init_array
//   272 bytes in section .rodata
// 2 406 bytes in section .text
// 
// 2 254 bytes of CODE  memory (+ 156 bytes shared)
//   232 bytes of CONST memory (+  40 bytes shared)
//   260 bytes of DATA  memory
//
//Errors: none
//Warnings: 2
