///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:21 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\vari /
//                       ous\chprintf.c                                       /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\vari /
//                       ous\chprintf.c -lCN F:\stuff\rusefi_sourceforge\firm /
//                       ware\iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge /
//                       \firmware\iar\Debug\List\ -o                         /
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
//                       -On                                                  /
//    MISRA C version =  2004                                                 /
//      Enabled       =  1.2-1.4 2.3 3.4 4.2 6.5 7 8.2 8.3 9.3 12.3 13.6      /
//                       13.7 15.5 17.3 19.17 20.7-20.9 20.11                 /
//      Checked       =  1.4 2.3 4.2 6.5 7 8.2 8.3 9.3 12.3 13.7 15.5         /
//                       20.7-20.9 20.11                                      /
//      Not checked   =  1.1-1.3 1.5-2.2 2.4-4.1 5-6.4 8.1 8.4-9.2 10-12.2    /
//                       12.4-13.6 14-15.4 16-20.6 20.10 20.12 21             /
//    List file       =  F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                       chprintf.s                                           /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME chprintf

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN __aeabi_cdcmple
        EXTERN __aeabi_d2f
        EXTERN __aeabi_d2iz
        EXTERN __aeabi_dmul
        EXTERN __aeabi_dsub
        EXTERN __aeabi_f2d
        EXTERN __aeabi_i2d
        EXTERN __aeabi_ui2d
        EXTERN __iar_Dtest
        EXTERN firmwareError
        EXTERN getRemainingStack
        EXTERN rlist

        PUBLIC chvprintf
        PUBLIC ftoa
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\various\chprintf.c
//    1 /*
//    2     ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
//    3 
//    4     Licensed under the Apache License, Version 2.0 (the "License");
//    5     you may not use this file except in compliance with the License.
//    6     You may obtain a copy of the License at
//    7 
//    8         http://www.apache.org/licenses/LICENSE-2.0
//    9 
//   10     Unless required by applicable law or agreed to in writing, software
//   11     distributed under the License is distributed on an "AS IS" BASIS,
//   12     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   13     See the License for the specific language governing permissions and
//   14     limitations under the License.
//   15 */
//   16 
//   17 /*
//   18    Concepts and parts of this file have been contributed by Fabio Utzig,
//   19    chvprintf() added by Brent Roman.
//   20  */
//   21 
//   22 /**
//   23  * @file    chprintf.c
//   24  * @brief   Mini printf-like functionality.
//   25  *
//   26  * @addtogroup chprintf
//   27  * @{
//   28  */
//   29 
//   30 #include "ch.h"
//   31 #include "chprintf.h"
//   32 
//   33 #define MAX_FILLER 11
//   34 /**
//   35  * That's out default %f precision here. Two digits should be fine?
//   36  * That's important on the lcd screen
//   37  */
//   38 #define FLOAT_PRECISION 100

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function long_to_string_with_divisor
          CFI NoCalls
        THUMB
//   39 static char *long_to_string_with_divisor(char *p,
//   40                                          long num,
//   41                                          unsigned radix,
//   42                                          long divisor) {
long_to_string_with_divisor:
        PUSH     {R4-R7}
          CFI R7 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
//   43   int i;
//   44   char *q;
//   45   long l, ll;
//   46 
//   47   l = num;
        MOVS     R6,R1
//   48   if (divisor == 0) {
        CMP      R3,#+0
        BNE.N    ??long_to_string_with_divisor_0
//   49     ll = num;
        MOVS     R7,R1
        B.N      ??long_to_string_with_divisor_1
//   50   } else {
//   51     ll = divisor;
??long_to_string_with_divisor_0:
        MOVS     R7,R3
//   52   }
//   53 
//   54   q = p + MAX_FILLER;
??long_to_string_with_divisor_1:
        ADDW     R12,R0,#+11
        MOV      R5,R12
//   55   do {
//   56     i = (int)(l % radix);
??long_to_string_with_divisor_2:
        UDIV     R12,R6,R2
        MLS      R12,R2,R12,R6
        MOV      R4,R12
//   57     i += '0';
        ADDS     R4,R4,#+48
//   58     if (i > '9')
        CMP      R4,#+58
        BLT.N    ??long_to_string_with_divisor_3
//   59       i += 'A' - '0' - 10;
        ADDS     R4,R4,#+7
//   60     *--q = i;
??long_to_string_with_divisor_3:
        SUBS     R5,R5,#+1
        STRB     R4,[R5, #+0]
//   61     l /= radix;
        UDIV     R6,R6,R2
//   62   } while ((ll /= radix) != 0);
        UDIV     R7,R7,R2
        CMP      R7,#+0
        BNE.N    ??long_to_string_with_divisor_2
//   63 
//   64   i = (int)(p + MAX_FILLER - q);
        ADDS     R12,R0,#+11
        SUBS     R12,R12,R5
        MOV      R4,R12
//   65   do
//   66     *p++ = *q++;
??long_to_string_with_divisor_4:
        LDRB     R12,[R5, #+0]
        STRB     R12,[R0, #+0]
        ADDS     R5,R5,#+1
        ADDS     R0,R0,#+1
//   67   while (--i);
        SUBS     R4,R4,#+1
        CMP      R4,#+0
        BNE.N    ??long_to_string_with_divisor_4
//   68 
//   69   return p;
        POP      {R4-R7}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R6 SameValue
          CFI R7 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//   70 }
//   71 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function ltoa
        THUMB
//   72 static char *ltoa(char *p, long num, unsigned radix) {
ltoa:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
//   73 
//   74   return long_to_string_with_divisor(p, num, radix, 0);
        MOVS     R3,#+0
        MOVS     R2,R6
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall long_to_string_with_divisor
        BL       long_to_string_with_divisor
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock1
//   75 }
//   76 
//   77 #if CHPRINTF_USE_FLOAT

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function ftoa
        THUMB
//   78 char *ftoa(char *p, double num, unsigned long precision) {
ftoa:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        VPUSH    {D8}
          CFI D8 Frame(CFA, -32)
          CFI CFA R13+32
        MOVS     R7,R0
        VMOV.F32 S16,S0
        VMOV.F32 S17,S1
        MOVS     R6,R1
//   79     if (num < 0) {
        VMOV     R0,R1,D8
        MOVS     R2,#+0
        MOVS     R3,#+0
          CFI FunCall __aeabi_cdcmple
        BL       __aeabi_cdcmple
        BCS.N    ??ftoa_0
//   80       *p++ = '-';
        MOVS     R0,#+45
        STRB     R0,[R7, #+0]
        ADDS     R7,R7,#+1
//   81       return ftoa(p, -num, precision);
        MOVS     R1,R6
        VMOV     R2,R3,D8
        EORS     R3,R3,#0x80000000
        VMOV     D0,R2,R3
        MOVS     R0,R7
          CFI FunCall ftoa
        BL       ftoa
        B.N      ??ftoa_1
//   82     }
//   83   long l;
//   84   if (isnan(num)) {
??ftoa_0:
        VMOV.F32 S0,S16
        VMOV.F32 S1,S17
          CFI FunCall __iar_Dtest
        BL       __iar_Dtest
        CMP      R0,#+2
        BNE.N    ??ftoa_2
//   85     *p++ = 'N';
        MOVS     R0,#+78
        STRB     R0,[R7, #+0]
        ADDS     R7,R7,#+1
//   86     *p++ = 'a';
        MOVS     R0,#+97
        STRB     R0,[R7, #+0]
        ADDS     R7,R7,#+1
//   87     *p++ = 'N';
        MOVS     R0,#+78
        STRB     R0,[R7, #+0]
        ADDS     R7,R7,#+1
//   88     return p;
        MOVS     R0,R7
        B.N      ??ftoa_1
//   89   }
//   90 
//   91   if (precision == 0)
??ftoa_2:
        CMP      R6,#+0
        BNE.N    ??ftoa_3
//   92     precision = FLOAT_PRECISION;
        MOVS     R0,#+100
        MOVS     R6,R0
//   93 
//   94   l = (long)num;
??ftoa_3:
        VMOV     R0,R1,D8
          CFI FunCall __aeabi_d2iz
        BL       __aeabi_d2iz
        MOVS     R4,R0
//   95   p = long_to_string_with_divisor(p, l, 10, 0);
        MOVS     R3,#+0
        MOVS     R2,#+10
        MOVS     R1,R4
        MOVS     R0,R7
          CFI FunCall long_to_string_with_divisor
        BL       long_to_string_with_divisor
        MOVS     R7,R0
//   96   *p++ = '.';
        MOVS     R0,#+46
        STRB     R0,[R7, #+0]
        ADDS     R7,R7,#+1
//   97   l = (long)((num - l) * precision);
        MOVS     R0,R4
          CFI FunCall __aeabi_i2d
        BL       __aeabi_i2d
        MOVS     R2,R0
        MOVS     R3,R1
        VMOV     R0,R1,D8
          CFI FunCall __aeabi_dsub
        BL       __aeabi_dsub
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R0,R6
          CFI FunCall __aeabi_ui2d
        BL       __aeabi_ui2d
        MOVS     R2,R4
        MOVS     R3,R5
          CFI FunCall __aeabi_dmul
        BL       __aeabi_dmul
          CFI FunCall __aeabi_d2iz
        BL       __aeabi_d2iz
        MOVS     R4,R0
//   98   return long_to_string_with_divisor(p, l, 10, precision / 10);
        MOVS     R0,#+10
        UDIV     R3,R6,R0
        MOVS     R2,#+10
        MOVS     R1,R4
        MOVS     R0,R7
          CFI FunCall long_to_string_with_divisor
        BL       long_to_string_with_divisor
??ftoa_1:
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+24
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock2
//   99 }
//  100 #endif
//  101 
//  102 #include "error_handling.h"
//  103 int getRemainingStack(Thread *otp);
//  104 
//  105 /**
//  106  * @brief   System formatted output function.
//  107  * @details This function implements a minimal @p vprintf()-like functionality
//  108  *          with output on a @p BaseSequentialStream.
//  109  *          The general parameters format is: %[-][width|*][.precision|*][l|L]p.
//  110  *          The following parameter types (p) are supported:
//  111  *          - <b>x</b> hexadecimal integer.
//  112  *          - <b>X</b> hexadecimal long.
//  113  *          - <b>o</b> octal integer.
//  114  *          - <b>O</b> octal long.
//  115  *          - <b>d</b> decimal signed integer.
//  116  *          - <b>D</b> decimal signed long.
//  117  *          - <b>u</b> decimal unsigned integer.
//  118  *          - <b>U</b> decimal unsigned long.
//  119  *          - <b>c</b> character.
//  120  *          - <b>s</b> string.
//  121  *          .
//  122  *
//  123  * @param[in] chp       pointer to a @p BaseSequentialStream implementing object
//  124  * @param[in] fmt       formatting string
//  125  * @param[in] ap        list of parameters
//  126  *
//  127  * @api
//  128  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function chvprintf
        THUMB
//  129 void chvprintf(BaseSequentialStream *chp, const char *fmt, va_list ap) {
chvprintf:
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
        SUB      SP,SP,#+40
          CFI CFA R13+88
        MOV      R11,R0
        MOVS     R4,R1
        MOVS     R5,R2
//  130   char *p, *s, c, filler;
//  131   int i, precision, width;
//  132   bool_t is_long, left_align;
//  133   long l;
//  134 #if CHPRINTF_USE_FLOAT
//  135   float f;
//  136   char tmpbuf[2*MAX_FILLER + 1];
//  137 #else
//  138   char tmpbuf[MAX_FILLER + 1];
//  139 #endif
//  140 
//  141   efiAssertVoid(getRemainingStack(chThdSelf()) > 64, "lowstck#1c");
        LDR.N    R0,??DataTable0
        LDR      R0,[R0, #+24]
          CFI FunCall getRemainingStack
        BL       getRemainingStack
        CMP      R0,#+65
        BGE.N    ??chvprintf_0
        LDR.N    R0,??DataTable0_1
          CFI FunCall firmwareError
        BL       firmwareError
        B.N      ??chvprintf_1
//  142 
//  143 
//  144   while (TRUE) {
//  145     c = *fmt++;
??chvprintf_0:
        LDRB     R0,[R4, #+0]
        MOVS     R7,R0
        ADDS     R4,R4,#+1
//  146     if (c == 0)
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+0
        BEQ.W    ??chvprintf_1
//  147       return;
//  148     if (c != '%') {
??chvprintf_2:
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+37
        BEQ.N    ??chvprintf_3
//  149       chSequentialStreamPut(chp, (uint8_t)c);
        MOVS     R1,R7
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOV      R0,R11
        LDR      R2,[R11, #+0]
        LDR      R2,[R2, #+8]
          CFI FunCall
        BLX      R2
//  150       continue;
        B.N      ??chvprintf_0
//  151     }
//  152     // we are here if c == '%' meaning we have a control sequence
//  153     p = tmpbuf;
??chvprintf_3:
        ADD      R0,SP,#+12
        MOVS     R6,R0
//  154     s = tmpbuf;
        ADD      R0,SP,#+12
        MOV      R10,R0
//  155     left_align = FALSE;
        MOVS     R0,#+0
        STRB     R0,[SP, #+2]
//  156     if (*fmt == '-') {
        LDRB     R0,[R4, #+0]
        CMP      R0,#+45
        BNE.N    ??chvprintf_4
//  157       fmt++;
        ADDS     R4,R4,#+1
//  158       left_align = TRUE;
        MOVS     R0,#+1
        STRB     R0,[SP, #+2]
//  159     }
//  160     filler = ' ';
??chvprintf_4:
        MOVS     R0,#+32
        STRB     R0,[SP, #+0]
//  161     if ((*fmt == '.') || (*fmt == '0')) {
        LDRB     R0,[R4, #+0]
        CMP      R0,#+46
        BEQ.N    ??chvprintf_5
        LDRB     R0,[R4, #+0]
        CMP      R0,#+48
        BNE.N    ??chvprintf_6
//  162       fmt++;
??chvprintf_5:
        ADDS     R4,R4,#+1
//  163       filler = '0';
        MOVS     R0,#+48
        STRB     R0,[SP, #+0]
//  164     }
//  165     width = 0;
??chvprintf_6:
        MOVS     R0,#+0
        MOV      R9,R0
//  166     while (TRUE) {
//  167       c = *fmt++;
??chvprintf_7:
        LDRB     R0,[R4, #+0]
        MOVS     R7,R0
        ADDS     R4,R4,#+1
//  168       if (c >= '0' && c <= '9')
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+48
        BLT.N    ??chvprintf_8
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+58
        BGE.N    ??chvprintf_8
//  169         c -= '0';
        SUBS     R7,R7,#+48
        B.N      ??chvprintf_9
//  170       else if (c == '*')
??chvprintf_8:
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+42
        BNE.N    ??chvprintf_10
//  171         c = va_arg(ap, int);
        LDR      R0,[R5, #+0]
        MOVS     R7,R0
        ADDS     R5,R5,#+4
        B.N      ??chvprintf_9
//  172       else
//  173         break;
//  174       width = width * 10 + c;
//  175     }
//  176     precision = 0;
??chvprintf_10:
        MOVS     R0,#+0
        MOV      R8,R0
//  177     if (c == '.') {
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+46
        BNE.N    ??chvprintf_11
        B.N      ??chvprintf_12
??chvprintf_9:
        MOVS     R0,#+10
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        MLA      R9,R0,R9,R7
        B.N      ??chvprintf_7
//  178       while (TRUE) {
//  179         c = *fmt++;
??chvprintf_12:
        LDRB     R0,[R4, #+0]
        MOVS     R7,R0
        ADDS     R4,R4,#+1
//  180         if (c >= '0' && c <= '9')
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+48
        BLT.N    ??chvprintf_13
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+58
        BGE.N    ??chvprintf_13
//  181           c -= '0';
        SUBS     R7,R7,#+48
        B.N      ??chvprintf_14
//  182         else if (c == '*')
??chvprintf_13:
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+42
        BNE.N    ??chvprintf_15
//  183           c = va_arg(ap, int);
        LDR      R0,[R5, #+0]
        MOVS     R7,R0
        ADDS     R5,R5,#+4
        B.N      ??chvprintf_14
//  184         else
//  185           break;
??chvprintf_15:
        B.N      ??chvprintf_11
//  186         precision *= 10;
??chvprintf_14:
        MOVS     R0,#+10
        MUL      R8,R0,R8
//  187         precision += c;
        UXTAB    R8,R8,R7
        B.N      ??chvprintf_12
//  188       }
//  189     }
//  190     /* Long modifier.*/
//  191     if (c == 'l' || c == 'L') {
??chvprintf_11:
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+108
        BEQ.N    ??chvprintf_16
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+76
        BNE.N    ??chvprintf_17
//  192       is_long = TRUE;
??chvprintf_16:
        MOVS     R0,#+1
        STRB     R0,[SP, #+1]
//  193       if (*fmt)
        LDRB     R0,[R4, #+0]
        CMP      R0,#+0
        BEQ.N    ??chvprintf_18
//  194         c = *fmt++;
        LDRB     R0,[R4, #+0]
        MOVS     R7,R0
        ADDS     R4,R4,#+1
        B.N      ??chvprintf_18
//  195     }
//  196     else
//  197       is_long = (c >= 'A') && (c <= 'Z');
??chvprintf_17:
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+65
        BLT.N    ??chvprintf_19
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+91
        BGE.N    ??chvprintf_19
        MOVS     R0,#+1
        B.N      ??chvprintf_20
??chvprintf_19:
        MOVS     R0,#+0
??chvprintf_20:
        STRB     R0,[SP, #+1]
//  198 
//  199     /* Command decoding.*/
//  200     switch (c) {
??chvprintf_18:
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        MOVS     R0,R7
        CMP      R0,#+68
        BEQ.N    ??chvprintf_21
        CMP      R0,#+73
        BEQ.N    ??chvprintf_21
        CMP      R0,#+79
        BEQ.N    ??chvprintf_22
        CMP      R0,#+85
        BEQ.N    ??chvprintf_23
        CMP      R0,#+88
        BEQ.N    ??chvprintf_24
        CMP      R0,#+99
        BEQ.N    ??chvprintf_25
        CMP      R0,#+100
        BEQ.N    ??chvprintf_21
        CMP      R0,#+102
        BEQ.N    ??chvprintf_26
        CMP      R0,#+105
        BEQ.N    ??chvprintf_21
        CMP      R0,#+111
        BEQ.N    ??chvprintf_22
        CMP      R0,#+115
        BEQ.N    ??chvprintf_27
        CMP      R0,#+117
        BEQ.N    ??chvprintf_23
        CMP      R0,#+120
        BEQ.N    ??chvprintf_24
        B.N      ??chvprintf_28
//  201     case 'c':
//  202       filler = ' ';
??chvprintf_25:
        MOVS     R0,#+32
        STRB     R0,[SP, #+0]
//  203       *p++ = va_arg(ap, int);
        LDR      R0,[R5, #+0]
        STRB     R0,[R6, #+0]
        ADDS     R5,R5,#+4
        ADDS     R6,R6,#+1
//  204       break;
        B.N      ??chvprintf_29
//  205     case 's':
//  206       filler = ' ';
??chvprintf_27:
        MOVS     R0,#+32
        STRB     R0,[SP, #+0]
//  207       if ((s = va_arg(ap, char *)) == 0)
        LDR      R0,[R5, #+0]
        ADDS     R5,R5,#+4
        MOV      R10,R0
        CMP      R0,#+0
        BNE.N    ??chvprintf_30
//  208         s = "(null)";
        LDR.N    R0,??DataTable0_2
        MOV      R10,R0
//  209       if (precision == 0)
??chvprintf_30:
        CMP      R8,#+0
        BNE.N    ??chvprintf_31
//  210         precision = 32767;
        MOVW     R0,#+32767
        MOV      R8,R0
//  211       for (p = s; *p && (--precision >= 0); p++)
??chvprintf_31:
        MOV      R6,R10
??chvprintf_32:
        LDRB     R0,[R6, #+0]
        CMP      R0,#+0
        BEQ.N    ??chvprintf_33
        SUBS     R8,R8,#+1
        CMP      R8,#+0
        BMI.N    ??chvprintf_33
        ADDS     R6,R6,#+1
        B.N      ??chvprintf_32
//  212         ;
//  213       break;
??chvprintf_33:
        B.N      ??chvprintf_29
//  214     case 'D':
//  215     case 'd':
//  216     case 'I':
//  217     case 'i':
//  218       if (is_long)
??chvprintf_21:
        LDRB     R0,[SP, #+1]
        CMP      R0,#+0
        BEQ.N    ??chvprintf_34
//  219         l = va_arg(ap, long);
        LDR      R0,[R5, #+0]
        STR      R0,[SP, #+4]
        ADDS     R5,R5,#+4
        B.N      ??chvprintf_35
//  220       else
//  221         l = va_arg(ap, int);
??chvprintf_34:
        LDR      R0,[R5, #+0]
        STR      R0,[SP, #+4]
        ADDS     R5,R5,#+4
//  222       if (l < 0) {
??chvprintf_35:
        LDR      R0,[SP, #+4]
        CMP      R0,#+0
        BPL.N    ??chvprintf_36
//  223         *p++ = '-';
        MOVS     R0,#+45
        STRB     R0,[R6, #+0]
        ADDS     R6,R6,#+1
//  224         l = -l;
        LDR      R0,[SP, #+4]
        RSBS     R0,R0,#+0
        STR      R0,[SP, #+4]
//  225       }
//  226       p = ltoa(p, l, 10);
??chvprintf_36:
        MOVS     R2,#+10
        LDR      R1,[SP, #+4]
        MOVS     R0,R6
          CFI FunCall ltoa
        BL       ltoa
        MOVS     R6,R0
//  227       break;
        B.N      ??chvprintf_29
//  228 #if CHPRINTF_USE_FLOAT
//  229     case 'f':
//  230       f = (float) va_arg(ap, double);
??chvprintf_26:
        ADDS     R5,R5,#+7
        BICS     R5,R5,#0x7
        LDRD     R0,R1,[R5, #+0]
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        VMOV     S16,R0
        ADDS     R5,R5,#+8
//  231       if (f < 0) {
        VCMP.F32 S16,#0.0
        FMSTAT   
        BPL.N    ??chvprintf_37
//  232         *p++ = '-';
        MOVS     R0,#+45
        STRB     R0,[R6, #+0]
        ADDS     R6,R6,#+1
//  233         f = -f;
        VNEG.F32 S16,S16
//  234       }
//  235       p = ftoa(p, f, precision);
??chvprintf_37:
        MOV      R1,R8
        MOV      R8,R1
        VMOV     R0,S16
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        VMOV     D0,R0,R1
        MOV      R1,R8
        MOVS     R0,R6
          CFI FunCall ftoa
        BL       ftoa
        MOVS     R6,R0
//  236       break;
        B.N      ??chvprintf_29
//  237 #endif
//  238     case 'X':
//  239     case 'x':
//  240       c = 16;
??chvprintf_24:
        MOVS     R0,#+16
        MOVS     R7,R0
//  241       goto unsigned_common;
        B.N      ??chvprintf_38
//  242     case 'U':
//  243     case 'u':
//  244       c = 10;
??chvprintf_23:
        MOVS     R0,#+10
        MOVS     R7,R0
//  245       goto unsigned_common;
        B.N      ??chvprintf_38
//  246     case 'O':
//  247     case 'o':
//  248       c = 8;
??chvprintf_22:
        MOVS     R0,#+8
        MOVS     R7,R0
//  249 unsigned_common:
//  250       if (is_long)
??chvprintf_38:
        LDRB     R0,[SP, #+1]
        CMP      R0,#+0
        BEQ.N    ??chvprintf_39
//  251         l = va_arg(ap, unsigned long);
        LDR      R0,[R5, #+0]
        STR      R0,[SP, #+4]
        ADDS     R5,R5,#+4
        B.N      ??chvprintf_40
//  252       else
//  253         l = va_arg(ap, unsigned int);
??chvprintf_39:
        LDR      R0,[R5, #+0]
        STR      R0,[SP, #+4]
        ADDS     R5,R5,#+4
//  254       p = ltoa(p, l, c);
??chvprintf_40:
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        MOVS     R2,R7
        LDR      R1,[SP, #+4]
        MOVS     R0,R6
          CFI FunCall ltoa
        BL       ltoa
        MOVS     R6,R0
//  255       break;
        B.N      ??chvprintf_29
//  256     default:
//  257       *p++ = c;
??chvprintf_28:
        STRB     R7,[R6, #+0]
        ADDS     R6,R6,#+1
//  258       break;
//  259     }
//  260     i = (int)(p - s);
??chvprintf_29:
        SUBS     R0,R6,R10
        STR      R0,[SP, #+8]
//  261     if ((width -= i) < 0)
        LDR      R0,[SP, #+8]
        SUBS     R9,R9,R0
        CMP      R9,#+0
        BPL.N    ??chvprintf_41
//  262       width = 0;
        MOVS     R0,#+0
        MOV      R9,R0
//  263     if (left_align == FALSE)
??chvprintf_41:
        LDRB     R0,[SP, #+2]
        CMP      R0,#+0
        BNE.N    ??chvprintf_42
//  264       width = -width;
        RSBS     R9,R9,#+0
//  265     if (width < 0) {
??chvprintf_42:
        CMP      R9,#+0
        BPL.N    ??chvprintf_43
//  266       if (*s == '-' && filler == '0') {
        LDRB     R0,[R10, #+0]
        CMP      R0,#+45
        BNE.N    ??chvprintf_44
        LDRB     R0,[SP, #+0]
        CMP      R0,#+48
        BNE.N    ??chvprintf_44
//  267         chSequentialStreamPut(chp, (uint8_t)*s++);
        LDRB     R1,[R10, #+0]
        MOV      R0,R11
        LDR      R2,[R11, #+0]
        LDR      R2,[R2, #+8]
          CFI FunCall
        BLX      R2
        ADDS     R10,R10,#+1
//  268         i--;
        LDR      R0,[SP, #+8]
        SUBS     R0,R0,#+1
        STR      R0,[SP, #+8]
//  269       }
//  270       do {
//  271         chSequentialStreamPut(chp, (uint8_t)filler);
??chvprintf_44:
        LDRB     R1,[SP, #+0]
        MOV      R0,R11
        LDR      R2,[R11, #+0]
        LDR      R2,[R2, #+8]
          CFI FunCall
        BLX      R2
//  272       } while (++width != 0);
        ADDS     R9,R9,#+1
        CMP      R9,#+0
        BNE.N    ??chvprintf_44
//  273     }
//  274     if (i > 0) {
??chvprintf_43:
        LDR      R0,[SP, #+8]
        CMP      R0,#+1
        BLT.N    ??chvprintf_45
//  275     	chSequentialStreamWrite(chp, (uint8_t*)s, i);
        LDR      R2,[SP, #+8]
        MOV      R1,R10
        MOV      R0,R11
        LDR      R3,[R11, #+0]
        LDR      R3,[R3, #+0]
          CFI FunCall
        BLX      R3
//  276     }
//  277     s += i;
??chvprintf_45:
        LDR      R0,[SP, #+8]
        ADDS     R10,R0,R10
//  278 
//  279     while (width) {
??chvprintf_46:
        CMP      R9,#+0
        BEQ.W    ??chvprintf_0
//  280       chSequentialStreamPut(chp, (uint8_t)filler);
        LDRB     R1,[SP, #+0]
        MOV      R0,R11
        LDR      R2,[R11, #+0]
        LDR      R2,[R2, #+8]
          CFI FunCall
        BLX      R2
//  281       width--;
        SUBS     R9,R9,#+1
        B.N      ??chvprintf_46
//  282     }
//  283   }
??chvprintf_1:
        ADD      SP,SP,#+40
          CFI CFA R13+48
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+40
        POP      {R0,R4-R11,PC}   ;; return
          CFI EndBlock cfiBlock3
//  284 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0:
        DC32     rlist

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_1:
        DC32     `?<Constant "lowstck#1c">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_2:
        DC32     `?<Constant "(null)">`

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "lowstck#1c">`:
        DATA
        DC8 "lowstck#1c"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "(null)">`:
        DATA
        DC8 "(null)"
        DC8 0

        END
//  285 
//  286 /** @} */
// 
//    20 bytes in section .rodata
// 1 090 bytes in section .text
// 
// 1 090 bytes of CODE  memory
//    20 bytes of CONST memory
//
//Errors: none
//Warnings: none
