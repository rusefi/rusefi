///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:08:45 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\controllers\alg /
//                       o\nmea.c                                             /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\controllers\alg /
//                       o\nmea.c -lCN F:\stuff\rusefi_sourceforge\firmware\i /
//                       ar\Debug\List\ -lA F:\stuff\rusefi_sourceforge\firmw /
//                       are\iar\Debug\List\ -o F:\stuff\rusefi_sourceforge\f /
//                       irmware\iar\Debug\Obj\ --no_cse --no_unroll          /
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
//                       -On                                                  /
//    MISRA C version =  2004                                                 /
//      Enabled       =  1.2-1.4 2.3 3.4 4.2 6.5 7 8.2 8.3 9.3 12.3 13.6      /
//                       13.7 15.5 17.3 19.17 20.7-20.9 20.11                 /
//      Checked       =  1.4 2.3 4.2 6.5 7 8.2 8.3 9.3 12.3 13.7 15.5         /
//                       20.7-20.9 20.11                                      /
//      Not checked   =  1.1-1.3 1.5-2.2 2.4-4.1 5-6.4 8.1 8.4-9.2 10-12.2    /
//                       12.4-13.6 14-15.4 16-20.6 20.10 20.12 21             /
//    List file       =  F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                       nmea.s                                               /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME nmea

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN __aeabi_d2f
        EXTERN __aeabi_d2iz
        EXTERN __aeabi_ddiv
        EXTERN __aeabi_dmul
        EXTERN __aeabi_f2d
        EXTERN __aeabi_i2d
        EXTERN __aeabi_memcpy
        EXTERN __iar_Strchr
        EXTERN __iar_Strrchr
        EXTERN __iar_Strstr
        EXTERN atoff
        EXTERN atoi
        EXTERN modf
        EXTERN round
        EXTERN strlen

        PUBLIC gps_convert_deg_to_dec
        PUBLIC gps_deg_dec
        PUBLIC gps_location
        PUBLIC hex2int
        PUBLIC memcpy
        PUBLIC nmea_get_message_type
        PUBLIC nmea_parse_gpgga
        PUBLIC nmea_parse_gprmc
        PUBLIC nmea_valid_checksum
        PUBLIC str2int
        PUBLIC str_till_comma
        PUBLIC strchr
        PUBLIC strrchr
        PUBLIC strstr
        
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
        
// F:\stuff\rusefi_sourceforge\firmware\controllers\algo\nmea.c
//    1 /**
//    2  * @date Dec 20, 2013
//    3  *
//    4  * @author Andrey Belomutskiy, (c) 2012-2015
//    5  * @author Kot_dnz
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
//   19  *
//   20  * see #testGpsParser
//   21  */
//   22 #include <math.h>
//   23 #include <time.h>
//   24 #include "main.h"

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
        SECTION_GROUP strchr
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function strchr
        THUMB
// __intrinsic __nounwind __interwork __softfp char *strchr(char const *, int)
strchr:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall __iar_Strchr
        BL       __iar_Strchr
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock1

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP strrchr
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function strrchr
        THUMB
// __intrinsic __nounwind __interwork __softfp char *strrchr(char const *, int)
strrchr:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall __iar_Strrchr
        BL       __iar_Strrchr
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock2

        SECTION `.text`:CODE:REORDER:NOROOT(1)
        SECTION_GROUP strstr
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function strstr
        THUMB
// __intrinsic __nounwind __interwork __softfp char *strstr(char const *, char const *)
strstr:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R1,R5
        MOVS     R0,R4
          CFI FunCall __iar_Strstr
        BL       __iar_Strstr
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock3
//   25 #include "nmea.h"
//   26 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function hex2int
          CFI NoCalls
        THUMB
//   27 long hex2int(char *a, int len) {
hex2int:
        PUSH     {R4-R6}
          CFI R6 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+12
        MOVS     R2,R0
//   28 	int i;
//   29 	long val = 0;
        MOVS     R0,#+0
//   30 
//   31 	for (i = 0; i < len; i++)
        MOVS     R4,#+0
        MOVS     R3,R4
??hex2int_0:
        CMP      R3,R1
        BGE.N    ??hex2int_1
//   32 		if (a[i] <= 57)
        LDRB     R4,[R3, R2]
        CMP      R4,#+58
        BGE.N    ??hex2int_2
//   33 			val += (a[i] - 48) * (1 << (4 * (len - 1 - i))); // it's number
        LDRB     R4,[R3, R2]
        SUBS     R4,R4,#+48
        MOVS     R5,#+1
        MOVS     R6,R1
        SUBS     R6,R6,#+1
        SUBS     R6,R6,R3
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        LSLS     R6,R6,#+2
        LSLS     R5,R5,R6
        MLA      R0,R5,R4,R0
        B.N      ??hex2int_3
//   34 		else
//   35 			val += (a[i] - 87) * (1 << (4 * (len - 1 - i))); // it's a-f -> work only with low case hex
??hex2int_2:
        LDRB     R4,[R3, R2]
        SUBS     R4,R4,#+87
        MOVS     R5,#+1
        MOVS     R6,R1
        SUBS     R6,R6,#+1
        SUBS     R6,R6,R3
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        LSLS     R6,R6,#+2
        LSLS     R5,R5,R6
        MLA      R0,R5,R4,R0
??hex2int_3:
        ADDS     R3,R3,#+1
        B.N      ??hex2int_0
//   36 	return val;
??hex2int_1:
        POP      {R4-R6}
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R6 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//   37 }
//   38 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function str2int
          CFI NoCalls
        THUMB
//   39 int str2int(char *a, int len) {
str2int:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
        MOVS     R2,R0
//   40 	 int i = 0,  k = 0;
        MOVS     R3,#+0
        MOVS     R0,#+0
//   41 	while (i<len) {
??str2int_0:
        CMP      R3,R1
        BGE.N    ??str2int_1
//   42 		k = (k<<3)+(k<<1)+(*a)-'0';
        LSLS     R4,R0,#+1
        ADDS     R0,R4,R0, LSL #+3
        LDRB     R4,[R2, #+0]
        UXTAB    R0,R0,R4
        SUBS     R0,R0,#+48
//   43 		a++;
        ADDS     R2,R2,#+1
//   44 		i++;
        ADDS     R3,R3,#+1
        B.N      ??str2int_0
//   45 	}
//   46 	return k;
??str2int_1:
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock5
//   47 }
//   48 

        SECTION `.text`:CODE:NOROOT(2)
          CFI Block cfiBlock6 Using cfiCommon0
          CFI Function gps_deg_dec
        THUMB
//   49 float gps_deg_dec(float deg_point) {
gps_deg_dec:
        PUSH     {R4,LR}
          CFI R14 Frame(CFA, -4)
          CFI R4 Frame(CFA, -8)
          CFI CFA R13+8
        VPUSH    {D8-D11}
          CFI D11 Frame(CFA, -16)
          CFI D10 Frame(CFA, -24)
          CFI D9 Frame(CFA, -32)
          CFI D8 Frame(CFA, -40)
          CFI CFA R13+40
        SUB      SP,SP,#+8
          CFI CFA R13+48
        VMOV.F32 S18,S0
//   50 	double ddeg;
//   51 	double sec = modf(deg_point, &ddeg) * 60;
        ADD      R0,SP,#+0
        MOVS     R4,R0
        VMOV     R0,S18
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        VMOV     D0,R0,R1
        MOVS     R0,R4
          CFI FunCall modf
        BL       modf
        VMOV     R2,R3,D0
        MOVS     R0,#+0
        LDR.W    R1,??DataTable1  ;; 0x404e0000
          CFI FunCall __aeabi_dmul
        BL       __aeabi_dmul
        VMOV     D8,R0,R1
//   52 	int deg = (int) (ddeg / 100);
        LDRD     R0,R1,[SP, #+0]
        MOVS     R2,#+0
        LDR.W    R3,??DataTable1_1  ;; 0x40590000
          CFI FunCall __aeabi_ddiv
        BL       __aeabi_ddiv
          CFI FunCall __aeabi_d2iz
        BL       __aeabi_d2iz
        MOVS     R4,R0
//   53 	int min = (int) (deg_point - (deg * 100));
        MOVS     R0,#+100
        MUL      R0,R0,R4
        VMOV     S0,R0
        VCVT.F32.S32 S0,S0
        VSUB.F32 S0,S18,S0
        VCVT.S32.F32 S19,S0
//   54 
//   55 	float absdlat = round(deg * 1000000.);
        MOVS     R0,R4
          CFI FunCall __aeabi_i2d
        BL       __aeabi_i2d
        MOVS     R2,#+0
        LDR.W    R3,??DataTable1_2  ;; 0x412e8480
          CFI FunCall __aeabi_dmul
        BL       __aeabi_dmul
        VMOV     D0,R0,R1
          CFI FunCall round
        BL       round
        VMOV     R0,R1,D0
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        VMOV     S20,R0
//   56 	float absmlat = round(min * 1000000.);
        VMOV     R0,S19
          CFI FunCall __aeabi_i2d
        BL       __aeabi_i2d
        MOVS     R2,#+0
        LDR.W    R3,??DataTable1_2  ;; 0x412e8480
          CFI FunCall __aeabi_dmul
        BL       __aeabi_dmul
        VMOV     D0,R0,R1
          CFI FunCall round
        BL       round
        VMOV     R0,R1,D0
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        VMOV     S21,R0
//   57 	float absslat = round(sec * 1000000.);
        VMOV     R2,R3,D8
        MOVS     R0,#+0
        LDR.W    R1,??DataTable1_2  ;; 0x412e8480
          CFI FunCall __aeabi_dmul
        BL       __aeabi_dmul
        VMOV     D0,R0,R1
          CFI FunCall round
        BL       round
        VMOV     R0,R1,D0
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        VMOV     S22,R0
//   58 
//   59 	return round(absdlat + (absmlat / 60) + (absslat / 3600)) / 1000000;
        VLDR.W   S0,??DataTable0  ;; 0x42700000
        VDIV.F32 S0,S21,S0
        VADD.F32 S0,S0,S20
        VLDR.W   S1,??DataTable0_1  ;; 0x45610000
        VDIV.F32 S1,S22,S1
        VADD.F32 S0,S0,S1
        VMOV     R0,S0
          CFI FunCall __aeabi_f2d
        BL       __aeabi_f2d
        VMOV     D0,R0,R1
          CFI FunCall round
        BL       round
        VMOV     R0,R1,D0
        MOVS     R2,#+0
        LDR.W    R3,??DataTable1_2  ;; 0x412e8480
          CFI FunCall __aeabi_ddiv
        BL       __aeabi_ddiv
          CFI FunCall __aeabi_d2f
        BL       __aeabi_d2f
        VMOV     S0,R0
        ADD      SP,SP,#+8
          CFI CFA R13+40
        VPOP     {D8-D11}
          CFI D8 SameValue
          CFI D9 SameValue
          CFI D10 SameValue
          CFI D11 SameValue
          CFI CFA R13+8
        POP      {R4,PC}          ;; return
          CFI EndBlock cfiBlock6
//   60 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0:
        DC32     0x42700000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable0_1:
        DC32     0x45610000
//   61 
//   62 // Convert lat e lon to decimals (from deg)

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock7 Using cfiCommon0
          CFI Function gps_convert_deg_to_dec
        THUMB
//   63 void gps_convert_deg_to_dec(float *latitude, char ns, float *longitude, char we) {
gps_convert_deg_to_dec:
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
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
//   64 	float lat = (ns == 'N') ? *latitude : -1 * (*latitude);
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+78
        BNE.N    ??gps_convert_deg_to_dec_0
        VLDR     S16,[R4, #0]
        B.N      ??gps_convert_deg_to_dec_1
??gps_convert_deg_to_dec_0:
        VLDR     S0,[R4, #0]
        VMOV.F32 S1,#-1.0
        VMUL.F32 S16,S0,S1
//   65 	float lon = (we == 'E') ? *longitude : -1 * (*longitude);
??gps_convert_deg_to_dec_1:
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+69
        BNE.N    ??gps_convert_deg_to_dec_2
        VLDR     S17,[R6, #0]
        B.N      ??gps_convert_deg_to_dec_3
??gps_convert_deg_to_dec_2:
        VLDR     S0,[R6, #0]
        VMOV.F32 S1,#-1.0
        VMUL.F32 S17,S0,S1
//   66 
//   67 	*latitude = gps_deg_dec(lat);
??gps_convert_deg_to_dec_3:
        VMOV.F32 S0,S16
          CFI FunCall gps_deg_dec
        BL       gps_deg_dec
        VSTR     S0,[R4, #0]
//   68 	*longitude = gps_deg_dec(lon);
        VMOV.F32 S0,S17
          CFI FunCall gps_deg_dec
        BL       gps_deg_dec
        VSTR     S0,[R6, #0]
//   69 }
        VPOP     {D8}
          CFI D8 SameValue
          CFI CFA R13+24
        POP      {R0,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock7
//   70 
//   71 // in string collect all char till comma and convert to float

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock8 Using cfiCommon0
          CFI Function str_till_comma
        THUMB
//   72 int str_till_comma(char *a, char *dStr) {
str_till_comma:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//   73 
//   74 	int i = 0, sLen = strlen(a);
        MOVS     R6,#+0
        MOVS     R0,R4
          CFI FunCall strlen
        BL       strlen
        MOVS     R1,R0
//   75 	if (sLen > GPS_MAX_STRING)
        CMP      R1,#+256
        BLE.N    ??str_till_comma_0
//   76 		sLen = GPS_MAX_STRING;
        MOV      R0,#+256
        MOVS     R1,R0
//   77 
//   78 	while (a[i] != 44 && i < sLen) {	// while not comma or end
??str_till_comma_0:
        LDRB     R0,[R6, R4]
        CMP      R0,#+44
        BEQ.N    ??str_till_comma_1
        CMP      R6,R1
        BGE.N    ??str_till_comma_1
//   79 		dStr[i] = a[i];
        LDRB     R0,[R6, R4]
        STRB     R0,[R6, R5]
//   80 		i++;
        ADDS     R6,R6,#+1
        B.N      ??str_till_comma_0
//   81 	}
//   82 	dStr[i] = '\0';
??str_till_comma_1:
        MOVS     R0,#+0
        STRB     R0,[R6, R5]
//   83 	return i;
        MOVS     R0,R6
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock8
//   84 }
//   85 
//   86 /*
//   87 GxGGA - name code
//   88 Parameter	Value	Unit	Description
//   89 UTC					hhmmss.sss	Universal time coordinated
//   90 Lat					ddmm.mmmm	Latitude
//   91 Northing Indicator			N=North, S=South
//   92 Lon					dddmm.mmmm	Longitude
//   93 Easting Indicator			E=East, W=West
//   94 Status				0			0=Invalid, 1=2D/3D, 2=DGPS, 6=Dead Reckoning
//   95 SVs Used			00			Number of SVs used for Navigation
//   96 HDOP				99.99		Horizontal Dilution of Precision
//   97 Alt (MSL)			m	Altitude (above means sea level)
//   98 Unit				M=Meters
//   99 Geoid Sep.			m			Geoid Separation = Alt(HAE) - Alt(MSL)
//  100 Unit				M=Meters
//  101 Age of DGPS Corr	s			Age of Differential Corrections
//  102 DGPS Ref Station				ID of DGPS Reference Station
//  103 */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock9 Using cfiCommon0
          CFI Function nmea_parse_gpgga
        THUMB
//  104 void nmea_parse_gpgga(char *nmea, loc_t *loc) {
nmea_parse_gpgga:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+256
          CFI CFA R13+272
        MOVS     R4,R0
        MOVS     R5,R1
//  105 	char *p = nmea;
        MOVS     R6,R4
//  106 	char dStr[GPS_MAX_STRING];
//  107 
//  108 	p = strchr(p, ',') + 1; 				//skip time - we read date&time if Valid in GxRMC
        MOVS     R1,#+44
        MOVS     R0,R6
          CFI FunCall strchr
        BL       strchr
        ADDS     R0,R0,#+1
        MOVS     R6,R0
//  109 
//  110 	p = strchr(p, ',') + 1;					// in p string started with searching address
        MOVS     R1,#+44
        MOVS     R0,R6
          CFI FunCall strchr
        BL       strchr
        ADDS     R0,R0,#+1
        MOVS     R6,R0
//  111 	str_till_comma(p, dStr);				// str to float till comma saved modified string
        ADD      R1,SP,#+0
        MOVS     R0,R6
          CFI FunCall str_till_comma
        BL       str_till_comma
//  112 	if(strlen(p) == 0) return;				// if no data in field - empty data - we return
        MOVS     R0,R6
          CFI FunCall strlen
        BL       strlen
        CMP      R0,#+0
        BEQ.N    ??nmea_parse_gpgga_0
//  113 	
//  114 	loc->latitude = atoff(dStr);				// fulfil data
??nmea_parse_gpgga_1:
        ADD      R0,SP,#+0
          CFI FunCall atoff
        BL       atoff
        VSTR     S0,[R5, #0]
//  115 
//  116 	p = strchr(p, ',') + 1;					// see above
        MOVS     R1,#+44
        MOVS     R0,R6
          CFI FunCall strchr
        BL       strchr
        ADDS     R0,R0,#+1
        MOVS     R6,R0
//  117 	switch (p[0]) {
        LDRB     R0,[R6, #+0]
        CMP      R0,#+44
        BEQ.N    ??nmea_parse_gpgga_2
        CMP      R0,#+78
        BEQ.N    ??nmea_parse_gpgga_3
        CMP      R0,#+83
        BEQ.N    ??nmea_parse_gpgga_4
        B.N      ??nmea_parse_gpgga_5
//  118 	case 'N':
//  119 		loc->lat = 'N';
??nmea_parse_gpgga_3:
        MOVS     R0,#+78
        STRB     R0,[R5, #+76]
//  120 		break;
        B.N      ??nmea_parse_gpgga_5
//  121 	case 'S':
//  122 		loc->lat = 'S';
??nmea_parse_gpgga_4:
        MOVS     R0,#+83
        STRB     R0,[R5, #+76]
//  123 		break;
        B.N      ??nmea_parse_gpgga_5
//  124 	case ',':
//  125 		loc->lat = '\0';
??nmea_parse_gpgga_2:
        MOVS     R0,#+0
        STRB     R0,[R5, #+76]
//  126 		break;
//  127 	}
//  128 
//  129 	p = strchr(p, ',') + 1;
??nmea_parse_gpgga_5:
        MOVS     R1,#+44
        MOVS     R0,R6
          CFI FunCall strchr
        BL       strchr
        ADDS     R0,R0,#+1
        MOVS     R6,R0
//  130 	str_till_comma(p, dStr);				// str to float till comma saved modified string
        ADD      R1,SP,#+0
        MOVS     R0,R6
          CFI FunCall str_till_comma
        BL       str_till_comma
//  131 	loc->longitude = atoff(dStr);
        ADD      R0,SP,#+0
          CFI FunCall atoff
        BL       atoff
        VSTR     S0,[R5, #+4]
//  132 
//  133 	p = strchr(p, ',') + 1;
        MOVS     R1,#+44
        MOVS     R0,R6
          CFI FunCall strchr
        BL       strchr
        ADDS     R0,R0,#+1
        MOVS     R6,R0
//  134 	switch (p[0]) {
        LDRB     R0,[R6, #+0]
        CMP      R0,#+44
        BEQ.N    ??nmea_parse_gpgga_6
        CMP      R0,#+69
        BEQ.N    ??nmea_parse_gpgga_7
        CMP      R0,#+87
        BNE.N    ??nmea_parse_gpgga_8
//  135 	case 'W':
//  136 		loc->lon = 'W';
??nmea_parse_gpgga_9:
        MOVS     R0,#+87
        STRB     R0,[R5, #+77]
//  137 		break;
        B.N      ??nmea_parse_gpgga_8
//  138 	case 'E':
//  139 		loc->lon = 'E';
??nmea_parse_gpgga_7:
        MOVS     R0,#+69
        STRB     R0,[R5, #+77]
//  140 		break;
        B.N      ??nmea_parse_gpgga_8
//  141 	case ',':
//  142 		loc->lon = '\0';
??nmea_parse_gpgga_6:
        MOVS     R0,#+0
        STRB     R0,[R5, #+77]
//  143 		break;
//  144 	}
//  145 
//  146 	p = strchr(p, ',') + 1;
??nmea_parse_gpgga_8:
        MOVS     R1,#+44
        MOVS     R0,R6
          CFI FunCall strchr
        BL       strchr
        ADDS     R0,R0,#+1
        MOVS     R6,R0
//  147 	str_till_comma(p, dStr);				// str to float till comma saved modified string
        ADD      R1,SP,#+0
        MOVS     R0,R6
          CFI FunCall str_till_comma
        BL       str_till_comma
//  148 	loc->quality = atoi(dStr);
        ADD      R0,SP,#+0
          CFI FunCall atoi
        BL       atoi
        STR      R0,[R5, #+68]
//  149 
//  150 	p = strchr(p, ',') + 1;
        MOVS     R1,#+44
        MOVS     R0,R6
          CFI FunCall strchr
        BL       strchr
        ADDS     R0,R0,#+1
        MOVS     R6,R0
//  151 	str_till_comma(p, dStr);				// str to float till comma saved modified string
        ADD      R1,SP,#+0
        MOVS     R0,R6
          CFI FunCall str_till_comma
        BL       str_till_comma
//  152 	loc->satellites = atoi(dStr);
        ADD      R0,SP,#+0
          CFI FunCall atoi
        BL       atoi
        STR      R0,[R5, #+72]
//  153 
//  154 	p = strchr(p, ',') + 1;
        MOVS     R1,#+44
        MOVS     R0,R6
          CFI FunCall strchr
        BL       strchr
        ADDS     R0,R0,#+1
        MOVS     R6,R0
//  155 
//  156 	p = strchr(p, ',') + 1;
        MOVS     R1,#+44
        MOVS     R0,R6
          CFI FunCall strchr
        BL       strchr
        ADDS     R0,R0,#+1
        MOVS     R6,R0
//  157 	str_till_comma(p, dStr);				// str to float till comma saved modified string
        ADD      R1,SP,#+0
        MOVS     R0,R6
          CFI FunCall str_till_comma
        BL       str_till_comma
//  158 	loc->altitude = atoff(dStr);
        ADD      R0,SP,#+0
          CFI FunCall atoff
        BL       atoff
        VSTR     S0,[R5, #+12]
//  159 }
??nmea_parse_gpgga_0:
        ADD      SP,SP,#+256
          CFI CFA R13+16
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock9
//  160 
//  161 /*
//  162 GxRMC - nmea code
//  163 Parameter	Value		Unit			Description
//  164 UTC						hhmmss.sss		Universal time coordinated
//  165 Status		V		A=Valid, V=Invalid
//  166 Lat			ddmm.mmmm					Latitude
//  167 Northing Indicator			N=North, S=South
//  168 Lon			dddmm.mmmm					Longitude
//  169 Easting Indicator			E=East, W=West
//  170 SOG						nots			Speed Over Ground
//  171 COG (true)				°				Course Over Ground (true)
//  172 Date					ddmmyy			Universal time coordinated
//  173 Magnetic Variation		°				Magnetic Variation
//  174 Magnetic Variation			E=East,W=West
//  175 Mode Indicator	N		A=Autonomous, D=Differential, E=Dead Reckoning, N=None
//  176 Navigational Status			S=Safe C=Caution U=Unsafe V=Not valid
//  177 
//  178 */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock10 Using cfiCommon0
          CFI Function nmea_parse_gprmc
        THUMB
//  179 void nmea_parse_gprmc(char *nmea, loc_t *loc) {
nmea_parse_gprmc:
        PUSH     {R4-R6,LR}
          CFI R14 Frame(CFA, -4)
          CFI R6 Frame(CFA, -8)
          CFI R5 Frame(CFA, -12)
          CFI R4 Frame(CFA, -16)
          CFI CFA R13+16
        SUB      SP,SP,#+304
          CFI CFA R13+320
        MOVS     R4,R0
        MOVS     R5,R1
//  180 	char *p = nmea;
        MOVS     R6,R4
//  181 	char dStr[GPS_MAX_STRING];
//  182 	struct tm timp;
//  183 
//  184 	p = strchr(p, ',') + 1; 				//read time
        MOVS     R1,#+44
        MOVS     R0,R6
          CFI FunCall strchr
        BL       strchr
        ADDS     R0,R0,#+1
        MOVS     R6,R0
//  185 	str_till_comma(p, dStr);
        ADD      R1,SP,#+44
        MOVS     R0,R6
          CFI FunCall str_till_comma
        BL       str_till_comma
//  186 	if(strlen(dStr) > 5){
        ADD      R0,SP,#+44
          CFI FunCall strlen
        BL       strlen
        CMP      R0,#+6
        BCC.N    ??nmea_parse_gprmc_0
//  187 		timp.tm_hour = str2int(dStr,2);
        MOVS     R1,#+2
        ADD      R0,SP,#+44
          CFI FunCall str2int
        BL       str2int
        STR      R0,[SP, #+8]
//  188 		timp.tm_min  = str2int(dStr+2,2);
        MOVS     R1,#+2
        ADD      R0,SP,#+46
          CFI FunCall str2int
        BL       str2int
        STR      R0,[SP, #+4]
//  189 		timp.tm_sec  = str2int(dStr+4,2);
        MOVS     R1,#+2
        ADD      R0,SP,#+48
          CFI FunCall str2int
        BL       str2int
        STR      R0,[SP, #+0]
//  190 	}
//  191 	
//  192 	p = strchr(p, ',') + 1; 				//read field Valid status
??nmea_parse_gprmc_0:
        MOVS     R1,#+44
        MOVS     R0,R6
          CFI FunCall strchr
        BL       strchr
        ADDS     R0,R0,#+1
        MOVS     R6,R0
//  193 	str_till_comma(p, dStr);
        ADD      R1,SP,#+44
        MOVS     R0,R6
          CFI FunCall str_till_comma
        BL       str_till_comma
//  194 
//  195 	if(dStr[0] == 'V') {					// if field is invalid
        LDRB     R0,[SP, #+44]
        CMP      R0,#+86
        BNE.N    ??nmea_parse_gprmc_1
//  196 		loc->quality = 0;
        MOVS     R0,#+0
        STR      R0,[R5, #+68]
//  197 		return;
        B.N      ??nmea_parse_gprmc_2
//  198 	}
//  199 	
//  200 	loc->quality = 4;						// this is declaration that last receive field VALID
??nmea_parse_gprmc_1:
        MOVS     R0,#+4
        STR      R0,[R5, #+68]
//  201 	
//  202 	p = strchr(p, ',') + 1;					// latitude
        MOVS     R1,#+44
        MOVS     R0,R6
          CFI FunCall strchr
        BL       strchr
        ADDS     R0,R0,#+1
        MOVS     R6,R0
//  203 	str_till_comma(p, dStr);				// str to float till comma saved modified string
        ADD      R1,SP,#+44
        MOVS     R0,R6
          CFI FunCall str_till_comma
        BL       str_till_comma
//  204 	loc->latitude = atoff(dStr);
        ADD      R0,SP,#+44
          CFI FunCall atoff
        BL       atoff
        VSTR     S0,[R5, #0]
//  205 
//  206 	p = strchr(p, ',') + 1;
        MOVS     R1,#+44
        MOVS     R0,R6
          CFI FunCall strchr
        BL       strchr
        ADDS     R0,R0,#+1
        MOVS     R6,R0
//  207 	switch (p[0]) {
        LDRB     R0,[R6, #+0]
        CMP      R0,#+44
        BEQ.N    ??nmea_parse_gprmc_3
        CMP      R0,#+78
        BEQ.N    ??nmea_parse_gprmc_4
        CMP      R0,#+83
        BEQ.N    ??nmea_parse_gprmc_5
        B.N      ??nmea_parse_gprmc_6
//  208 	case 'N':
//  209 		loc->lat = 'N';
??nmea_parse_gprmc_4:
        MOVS     R0,#+78
        STRB     R0,[R5, #+76]
//  210 		break;
        B.N      ??nmea_parse_gprmc_6
//  211 	case 'S':
//  212 		loc->lat = 'S';
??nmea_parse_gprmc_5:
        MOVS     R0,#+83
        STRB     R0,[R5, #+76]
//  213 		break;
        B.N      ??nmea_parse_gprmc_6
//  214 	case ',':
//  215 		loc->lat = '\0';
??nmea_parse_gprmc_3:
        MOVS     R0,#+0
        STRB     R0,[R5, #+76]
//  216 		break;
//  217 	}
//  218 
//  219 	p = strchr(p, ',') + 1; 				// longitude
??nmea_parse_gprmc_6:
        MOVS     R1,#+44
        MOVS     R0,R6
          CFI FunCall strchr
        BL       strchr
        ADDS     R0,R0,#+1
        MOVS     R6,R0
//  220 	str_till_comma(p, dStr);				// str to float till comma saved modified string
        ADD      R1,SP,#+44
        MOVS     R0,R6
          CFI FunCall str_till_comma
        BL       str_till_comma
//  221 	loc->longitude = atoff(dStr);
        ADD      R0,SP,#+44
          CFI FunCall atoff
        BL       atoff
        VSTR     S0,[R5, #+4]
//  222 
//  223 	p = strchr(p, ',') + 1;
        MOVS     R1,#+44
        MOVS     R0,R6
          CFI FunCall strchr
        BL       strchr
        ADDS     R0,R0,#+1
        MOVS     R6,R0
//  224 	switch (p[0]) {
        LDRB     R0,[R6, #+0]
        CMP      R0,#+44
        BEQ.N    ??nmea_parse_gprmc_7
        CMP      R0,#+69
        BEQ.N    ??nmea_parse_gprmc_8
        CMP      R0,#+87
        BNE.N    ??nmea_parse_gprmc_9
//  225 	case 'W':
//  226 		loc->lon = 'W';
??nmea_parse_gprmc_10:
        MOVS     R0,#+87
        STRB     R0,[R5, #+77]
//  227 		break;
        B.N      ??nmea_parse_gprmc_9
//  228 	case 'E':
//  229 		loc->lon = 'E';
??nmea_parse_gprmc_8:
        MOVS     R0,#+69
        STRB     R0,[R5, #+77]
//  230 		break;
        B.N      ??nmea_parse_gprmc_9
//  231 	case ',':
//  232 		loc->lon = '\0';
??nmea_parse_gprmc_7:
        MOVS     R0,#+0
        STRB     R0,[R5, #+77]
//  233 		break;
//  234 	}
//  235 
//  236 	p = strchr(p, ',') + 1;
??nmea_parse_gprmc_9:
        MOVS     R1,#+44
        MOVS     R0,R6
          CFI FunCall strchr
        BL       strchr
        ADDS     R0,R0,#+1
        MOVS     R6,R0
//  237 	str_till_comma(p, dStr);				// str to float till comma saved modified string
        ADD      R1,SP,#+44
        MOVS     R0,R6
          CFI FunCall str_till_comma
        BL       str_till_comma
//  238 	loc->speed = atoff(dStr);
        ADD      R0,SP,#+44
          CFI FunCall atoff
        BL       atoff
        VSTR     S0,[R5, #+8]
//  239 	
//  240 	p = strchr(p, ',') + 1;
        MOVS     R1,#+44
        MOVS     R0,R6
          CFI FunCall strchr
        BL       strchr
        ADDS     R0,R0,#+1
        MOVS     R6,R0
//  241 	str_till_comma(p, dStr);				// str to float till comma saved modified string
        ADD      R1,SP,#+44
        MOVS     R0,R6
          CFI FunCall str_till_comma
        BL       str_till_comma
//  242 	loc->course = atoff(dStr);
        ADD      R0,SP,#+44
          CFI FunCall atoff
        BL       atoff
        VSTR     S0,[R5, #+16]
//  243 
//  244 	p = strchr(p, ',') + 1; 				//read date
        MOVS     R1,#+44
        MOVS     R0,R6
          CFI FunCall strchr
        BL       strchr
        ADDS     R0,R0,#+1
        MOVS     R6,R0
//  245 	str_till_comma(p, dStr);
        ADD      R1,SP,#+44
        MOVS     R0,R6
          CFI FunCall str_till_comma
        BL       str_till_comma
//  246 	if(strlen(dStr) > 5){
        ADD      R0,SP,#+44
          CFI FunCall strlen
        BL       strlen
        CMP      R0,#+6
        BCC.N    ??nmea_parse_gprmc_11
//  247 		timp.tm_mday = str2int(dStr,2);
        MOVS     R1,#+2
        ADD      R0,SP,#+44
          CFI FunCall str2int
        BL       str2int
        STR      R0,[SP, #+12]
//  248 		timp.tm_mon  = str2int(dStr+2,2);
        MOVS     R1,#+2
        ADD      R0,SP,#+46
          CFI FunCall str2int
        BL       str2int
        STR      R0,[SP, #+16]
//  249 		timp.tm_year = str2int(dStr+4,2)+100;	// we receive -200, but standard wait -1900 = add correction
        MOVS     R1,#+2
        ADD      R0,SP,#+48
          CFI FunCall str2int
        BL       str2int
        ADDS     R0,R0,#+100
        STR      R0,[SP, #+20]
//  250 	}
//  251 
//  252 	if( timp.tm_year > 0 ) {				// check if date field is valid  
??nmea_parse_gprmc_11:
        LDR      R0,[SP, #+20]
        CMP      R0,#+1
        BLT.N    ??nmea_parse_gprmc_12
//  253 		memcpy(&loc->GPStm, &timp, sizeof(timp));
        MOVS     R2,#+44
        ADD      R1,SP,#+0
        ADDS     R0,R5,#+20
          CFI FunCall memcpy
        BL       memcpy
//  254 	}
//  255 }
??nmea_parse_gprmc_12:
??nmea_parse_gprmc_2:
        ADD      SP,SP,#+304
          CFI CFA R13+16
        POP      {R4-R6,PC}       ;; return
          CFI EndBlock cfiBlock10
//  256 
//  257 /**
//  258  * Get the message type (GPGGA, GPRMC, etc..)
//  259  *
//  260  * This function filters out also wrong packages (invalid checksum)
//  261  *
//  262  * @param message The NMEA message
//  263  * @return The type of message if it is valid
//  264  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock11 Using cfiCommon0
          CFI Function nmea_get_message_type
        THUMB
//  265 nmea_message_type nmea_get_message_type(const char *message) {
nmea_get_message_type:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//  266 	int checksum = nmea_valid_checksum(message);
        MOVS     R0,R4
          CFI FunCall nmea_valid_checksum
        BL       nmea_valid_checksum
        MOVS     R5,R0
//  267 	if (checksum != _EMPTY) {
        CMP      R5,#+0
        BEQ.N    ??nmea_get_message_type_0
//  268 		return checksum;
        MOVS     R0,R5
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        B.N      ??nmea_get_message_type_1
//  269 	}
//  270 
//  271 	if (strstr(message, NMEA_GPGGA_STR) != NULL) {
??nmea_get_message_type_0:
        LDR.N    R1,??DataTable1_3
        MOVS     R0,R4
          CFI FunCall strstr
        BL       strstr
        CMP      R0,#+0
        BEQ.N    ??nmea_get_message_type_2
//  272 		return NMEA_GPGGA;
        MOVS     R0,#+2
        B.N      ??nmea_get_message_type_1
//  273 	}
//  274 
//  275 	if (strstr(message, NMEA_GPRMC_STR) != NULL) {
??nmea_get_message_type_2:
        LDR.N    R1,??DataTable1_4
        MOVS     R0,R4
          CFI FunCall strstr
        BL       strstr
        CMP      R0,#+0
        BEQ.N    ??nmea_get_message_type_3
//  276 		return NMEA_GPRMC;
        MOVS     R0,#+1
        B.N      ??nmea_get_message_type_1
//  277 	}
//  278 
//  279 	return NMEA_UNKNOWN;
??nmea_get_message_type_3:
        MOVS     R0,#+0
??nmea_get_message_type_1:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock11
//  280 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1:
        DC32     0x404e0000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_1:
        DC32     0x40590000

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_2:
        DC32     0x412e8480

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_3:
        DC32     `?<Constant "$GPGGA">`

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_4:
        DC32     `?<Constant "$GPRMC">`
//  281 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock12 Using cfiCommon0
          CFI Function nmea_valid_checksum
        THUMB
//  282 int nmea_valid_checksum(const char *message) {
nmea_valid_checksum:
        PUSH     {R4-R8,LR}
          CFI R14 Frame(CFA, -4)
          CFI R8 Frame(CFA, -8)
          CFI R7 Frame(CFA, -12)
          CFI R6 Frame(CFA, -16)
          CFI R5 Frame(CFA, -20)
          CFI R4 Frame(CFA, -24)
          CFI CFA R13+24
        MOVS     R4,R0
//  283 	char p;
//  284 	int sum = 0;
        MOVS     R6,#+0
//  285 	char *starPtr = strrchr(message, '*');
        MOVS     R1,#+42
        MOVS     R0,R4
          CFI FunCall strrchr
        BL       strrchr
        MOVS     R7,R0
//  286 	if (starPtr == NULL)
        CMP      R7,#+0
        BNE.N    ??nmea_valid_checksum_0
//  287 		return NMEA_CHECKSUM_ERR;
        MOVS     R0,#+128
        B.N      ??nmea_valid_checksum_1
//  288 	char *int_message = starPtr + 1;
??nmea_valid_checksum_0:
        ADDS     R8,R7,#+1
//  289 	long checksum = hex2int(int_message, 2);
        MOVS     R1,#+2
        MOV      R0,R8
          CFI FunCall hex2int
        BL       hex2int
        MOVS     R1,R0
//  290 
//  291 	++message;
        ADDS     R4,R4,#+1
//  292 	while ((p = *message++) != '*') {
??nmea_valid_checksum_2:
        LDRB     R0,[R4, #+0]
        ADDS     R4,R4,#+1
        MOVS     R5,R0
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+42
        BEQ.N    ??nmea_valid_checksum_3
//  293 		sum ^= p;
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        EORS     R6,R5,R6
        B.N      ??nmea_valid_checksum_2
//  294 	}
//  295 
//  296 	if (sum != checksum) {
??nmea_valid_checksum_3:
        CMP      R6,R1
        BEQ.N    ??nmea_valid_checksum_4
//  297 		return NMEA_CHECKSUM_ERR;
        MOVS     R0,#+128
        B.N      ??nmea_valid_checksum_1
//  298 	}
//  299 	return _EMPTY;
??nmea_valid_checksum_4:
        MOVS     R0,#+0
??nmea_valid_checksum_1:
        POP      {R4-R8,PC}       ;; return
          CFI EndBlock cfiBlock12
//  300 }
//  301 
//  302 // Compute the GPS location using decimal scale

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock13 Using cfiCommon0
          CFI Function gps_location
        THUMB
//  303 void gps_location(loc_t *coord, char *buffer) {
gps_location:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  304 
//  305 	coord->type = nmea_get_message_type(buffer);
        MOVS     R0,R5
          CFI FunCall nmea_get_message_type
        BL       nmea_get_message_type
        STRB     R0,[R4, #+64]
//  306 
//  307 	switch (coord->type) {
        LDRB     R0,[R4, #+64]
        CMP      R0,#+0
        BEQ.N    ??gps_location_0
        CMP      R0,#+2
        BEQ.N    ??gps_location_1
        BCC.N    ??gps_location_2
        B.N      ??gps_location_3
//  308 	case NMEA_GPGGA:
//  309 		nmea_parse_gpgga(buffer, coord);
??gps_location_1:
        MOVS     R1,R4
        MOVS     R0,R5
          CFI FunCall nmea_parse_gpgga
        BL       nmea_parse_gpgga
//  310 		gps_convert_deg_to_dec(&(coord->latitude), coord->lat, &(coord->longitude), coord->lon);
        LDRB     R3,[R4, #+77]
        ADDS     R2,R4,#+4
        LDRB     R1,[R4, #+76]
        MOVS     R0,R4
          CFI FunCall gps_convert_deg_to_dec
        BL       gps_convert_deg_to_dec
//  311 		break;
        B.N      ??gps_location_3
//  312 	case NMEA_GPRMC:
//  313 		nmea_parse_gprmc(buffer, coord);
??gps_location_2:
        MOVS     R1,R4
        MOVS     R0,R5
          CFI FunCall nmea_parse_gprmc
        BL       nmea_parse_gprmc
//  314 		break;
        B.N      ??gps_location_3
//  315 	case NMEA_UNKNOWN:
//  316 		// unknown message type
//  317 		break;
//  318 	}
//  319 
//  320 }
??gps_location_0:
??gps_location_3:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock13

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "$GPGGA">`:
        DATA
        DC8 "$GPGGA"
        DC8 0

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
`?<Constant "$GPRMC">`:
        DATA
        DC8 "$GPRMC"
        DC8 0

        END
// 
//    16 bytes in section .rodata
// 1 534 bytes in section .text
// 
// 1 464 bytes of CODE  memory (+ 70 bytes shared)
//    16 bytes of CONST memory
//
//Errors: none
//Warnings: 2
